#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include "console.h"
#include "task.h"
#include "arp.h"
#include "ip.h"
#include "net.h"
#include "sock.h"
#include "udp.h"
#include "dhcp.h"
#include "route.h"
#include "dns.h"

#define NB_MIN_SIZE 64
#define NB_MAX_SIZE 2000    /* Maximum size of a network buffer, in bytes */
#define NETQ_MAX_ITEMS 256  /* Maximum number of buffers in network queue */

/* Global network queue */
struct net_queue *netq;

/* Global array of network interfaces */
netif_t netifs[MAX_NET_IFACES];

/* Global number of network interfaces */
int nics;

struct net_buf *net_buf_alloc(int size, int nic_id)
{
    struct net_buf *buf = NULL;

    if (netq->list->num_items >= NETQ_MAX_ITEMS) {
        kprintf("net_buf_alloc: queue full\n");
        return NULL;
    }
    if (size > NB_MAX_SIZE) {
        kprintf("net_buf_alloc: buffer size too big (%d)\n", NB_MAX_SIZE);
        return NULL;
    }
    //size = size < NB_MIN_SIZE ? NB_MIN_SIZE : size;
    if (!(buf = calloc(1, sizeof(*buf) + size))) {
        kprintf("net_buf_alloc: out of memory\n");
        return NULL;
    }
    buf->nic = nic_id;
    buf->size = size;
    buf->data = buf + 1;

    return buf;
}

void net_buf_free(void *buf)
{
    free(buf);
}

node_t *netq_push(struct net_buf *buf)
{
    node_t *node = NULL;

    if (!netq)
        return NULL;

    spin_lock(&netq->lock);
    if (netq->list->num_items >= NETQ_MAX_ITEMS) {
        kprintf("Too many net_bufs\n");
        goto ret;
    }
    node = list_add(netq->list, buf);

ret:
    spin_unlock(&netq->lock);
    wakeup(&netq);

    return node;
}

struct net_buf *netq_shift()
{
    struct net_buf *buf = NULL;

    spin_lock(&netq->lock);
    if (netq->list->num_items == 0)
        goto ret;

    buf = netq->list->head->data;
    list_del(netq->list, netq->list->head);

ret:
    spin_unlock(&netq->lock);
    return buf;
}

/* Converts a network buffer to eth_hdr */
eth_hdr_t *buf2eth(void *buf)
{
    eth_hdr_t *eth = buf;
    eth->eth_type = ntohs(eth->eth_type);
    return eth;
}

/* Converts eth_hdr to raw data */
void *eth2buf(eth_hdr_t *eth)
{
    eth->eth_type = htons(eth->eth_type);
    return eth;
}

/* Process a network buffer */
static int net_process(struct net_buf *buf)
{
    eth_hdr_t *eth = (eth_hdr_t *) buf->data;
    uint16_t eth_type = ntohs(eth->eth_type);

    if (buf->size < sizeof(eth_hdr_t))
        return -1;

    switch(eth_type) {
        case ETH_P_ARP:
            arp_process(buf);
            break;
        case ETH_P_IP:
            ip_process(buf);
            break;
        default:
            kprintf("Unsupported ether type: %02x\n", eth_type);
            break;
    }
    return 0;
}

static void net_task()
{
    struct net_buf *buf;
    free(current_task->name);
    current_task->name = strdup("net");

    for (;;) {
        if (!netq->list->num_items)
            sleep_on(&netq);
        buf = netq_shift();
        net_process(buf);
        /* If buffer was not captured, drop it */
        if (!buf->captured)
            net_buf_free(buf);
    }
}

uint32_t make_ip(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return (a << 24) | (b << 16) | (c << 8) | d;
}

char *ip2str(uint32_t ip, char *str, int size)
{
    uint8_t d[4];
    *(uint32_t *)d = ip;
    snprintf(str, size, "%d.%d.%d.%d", d[3], d[2], d[1], d[0]);

    return str;
}

void print_ip(uint32_t ip)
{
    uint8_t d[4];
    *(uint32_t *)d = ip;
    kprintf("%d.%d.%d.%d", d[3], d[2], d[1], d[0]);
}

void print_mac(uint8_t mac[6])
{
    kprintf("%02x:%02x:%02x:%02x:%02x:%02x",
        mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

int iface_add_ip(struct netif *iface, uint32_t ip)
{
    int i;

    for (i = 0; i < IFACE_MAX_IPS; i++)
        if (iface->ips[i] == 0)
            break;
    if (i == IFACE_MAX_IPS)
        return -1;
    iface->ips[i] = ip;

    return 0;
}

void set_endianess()
{
    int test = 0x1;
    char c = *(char *)&test;
    is_bige = (c == 0);
}

int net_init()
{
    set_endianess();
    nics = 0;
    /* Hardcode 2 IP addresses for eth0 for now: 10.0.0.3,4 XXX
     * TODO: add a configuration file
     */
    if (netifs[0].priv) {
        iface_add_ip(&netifs[0], make_ip(10, 0, 2, 3));
        iface_add_ip(&netifs[0], make_ip(10, 0, 2, 4));
    }
    /* Init network queue */
    if (!(netq = calloc(1, sizeof(*netq)))) {
        kprintf("netq_init: out of memory\n");
        return -1;
    }
    if (!(netq->list = list_open(NULL))) {
        kprintf("netq_init: can't initialize queue\n");
        return -1;
    }

    if (fork() == 0)
        net_task();

    route_init();
    arp_init();
    sock_init();
    udp_init();
    if (netifs[0].priv) {
        dhcp_init(&netifs[0]);
    }
    dns_init();

    return 0;
}

