#include "i386.h"
#include "int.h"
#include "irq.h"
#include "console.h"
#include "task.h"
#include "timer.h"

// programmable interval timer //
// timer has clock @ 1193180Hz
#define pit_data1 0x40		// data channel 1 register - generate irq0
#define pit_data2 0x41		// data channel 2 register - sys specific
#define pit_data3 0x42		// data channel 3 register - speaker
#define pit_cmmnd 0x43		// command register

static void set_timer_phase(int hz) 
{
    int divisor = 1193180 / hz;		// compute divisor //
    outb(pit_cmmnd, 0x36);
    outb(pit_data1, divisor & 0xFF); // set low byte of divisor
    outb(pit_data1, divisor >> 8);	 // high byte of divisor
}

/**
 * Timer interrupt handler
 * This will also call the scheduller
 */
void timer_handler(struct iregs *r) 
{
    (void) r;
    timer_ticks++;
    if (current_task)
        task_switch_int();
}

/**
 * 100 interrupts per second
 */
void timer_install(void) 
{
    timer_ticks = 0;
    irq_install_handler(0, timer_handler);  // set PIT to generate irq0
    set_timer_phase(1000 / MS_PER_TICK);    // one interrupt every 10ms
}

