// hard drive queued commands //
#include <sys/types.h>
#include <stdlib.h>		// malloc
#include "task.h"
#include "x86.h"		// i/o
#include "console.h"	// panic, kprintf
#include "list.h"
#include "hd.h"
#include "hd_queue.h"

// maximum number of buffers that we should keep in memory //
#define MAX_HD_BUFS 100

// this will be the disk buffers queue //
struct hd_queue {
	list_t *list;
	spin_lock_t lock;
};

struct hd_queue * hd_queue;

// allocate a new buffer //
hd_buf_t *new_hd_buf()
{
	hd_buf_t *hdb;
	if(!(hdb = malloc(sizeof(*hdb)))) {
		panic("malloc");
	}
	hdb->block_no = hdb->is_dirty = hdb->ref_count = 0;
	return hdb;
}

// frees an allocated buffer
void free_hd_buf(void *data)
{
	hd_buf_t *hdb = (hd_buf_t *) data;
	if(hdb->ref_count != 0) {
		panic("block %d still in use\n", hdb->block_no);
	}
	free(hdb);
}

// gets the buffer of block_no //
hd_buf_t *get_hd_buf(int block_no)
{
	node_t *n = NULL;
	hd_buf_t * hdb = NULL;
	spin_lock(&hd_queue->lock);
	for(n = hd_queue->list->head; n; n = n->next) {
		if(((hd_buf_t *)n->data)->block_no == block_no) {
			hdb = (hd_buf_t *) n->data;
			hdb->ref_count++;
			break;
		}
	}
	spin_unlock(&hd_queue->lock);
	if(!hdb) {
		spin_lock(&hd_queue->lock);
		// create a new one //
		if(hd_queue->list->num_items < MAX_HD_BUFS) {
			hdb = new_hd_buf();
			list_add(hd_queue->list, hdb);
		}
		// not found, we are going to reuse one //
		else {
			for(n = hd_queue->list->head; n; n = n->next) {
				hdb = (hd_buf_t *) n->data;
				if(hdb->ref_count == 0 && !hdb->is_dirty && !hdb->lock) {
					break;
				}
			}
			if(!hdb) {
				panic("max_hd_bufs %d reached and cannot find any free buf!\n", MAX_HD_BUFS);
			}
		}
		hdb->lock = 0;
		hdb->block_no = block_no;
		hdb->is_dirty = false;
		hdb->ref_count = 1;
		spin_lock(&hdb->lock);

		spin_unlock(&hd_queue->lock);
		// and get the data from distk //
		kprintf("hd_start %d\n", block_no);
		hd_start(hdb);
	}
	return hdb;
}

void put_hd_buf(hd_buf_t *hdb)
{
	// if is dirty and no reference to it //
	// write it back to disk //
	hdb->ref_count--;
	if(hdb->is_dirty && hdb->ref_count == 0) {
		hd_start(hdb);
		hdb->is_dirty = 0;
	}
	spin_unlock(&hdb->lock);
}

// init the queue
int hd_queue_init()
{
	hd_init();

	if(!(hd_queue = malloc(sizeof(struct hd_queue)))) {
		panic("malloc");
	}
	if(!(hd_queue->list = list_open(free_hd_buf))) {
		panic("list_open");
	}
	kprintf("hd queue\n\n");
	if(fork() == 0) {
		kprintf("child reading\n");
		hd_buf_t *hdb = get_hd_buf(0);
		get_hd_buf(4);
		kprintf("after 0, get buf 2\n");
		hd_buf_t *hdb2 = get_hd_buf(1);
		kprintf("should be waked up\n");
		uint16_t *b = (uint16_t *) hdb->buf;
		int i;
		for(i=0; i<8; i++){
			kprintf("[%i:%x]\n", i, b[i]);
		}
		b[4]=0xBBAA;
		hdb->is_dirty = true;
		put_hd_buf(hdb);

		node_t *n;
		for(n=hd_queue->list->head; n; n=n->next) {
			kprintf("block %d in cache, dirty: %d\n",((hd_buf_t *)n->data)->block_no,((hd_buf_t *)n->data)->is_dirty);
		}

		task_exit(0);
	}
	// int i;
	// for(i=0; i<128;i++) {
		// kprintf("%x ", b[i]);
		// if(i % 4 == 0 ) kprintf("\n");
	// }

	return 0;
}
