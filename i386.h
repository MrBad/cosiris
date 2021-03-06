#ifndef _I386_H
#define _I386_H

#include <sys/types.h>
#include "int.h"
#include "kinfo.h"
#include "mem.h"

typedef uint32_t spin_lock_t;

/**
 * Gets CPU stack pointer
 */
uint32_t get_esp();

/**
 * Gets CPU base pointer
 */
uint32_t get_ebp();

/**
 * Gets CPU instruction pointer
 */
uint32_t get_eip();

/**
 * Gets faulty address (cr2)
 */
uint32_t get_fault_addr();

/**
 * Disable interrupts and halts the CPU
 */
void halt();

/**
 * Suspend the CPU until next interrupt accours
 */
void hlt();

/**
 * No op
 */
void nop();
/**
 * Clear interrupt flag. Ignore maskable interrupts
 */
void cli();

/**
 * Set interrupt flag. Enable maskable interrupts
 */
void sti();

/**
 * Outputs a byte to port
 */
void outb(uint16_t port, uint8_t byte);

/**
 * Outputs a word to port
 */
void outw(uint16_t port, uint16_t value);

/**
 * Outputs a long to port
 */
void outl(uint16_t port, uint32_t value);
/**
 * Inputs a byte from port
 */
uint8_t inb(uint16_t port);

/**
 * Inputs a word from port
 */
uint16_t inw(uint16_t port);

/**
 * Inputs a long to port
 */
uint32_t inl(uint16_t port);

/**
 * Fills kinfo structure with code, start, data, bss, end, stack, stack_size
 */
void get_kinfo(struct kinfo *kinfo);

/**
 * Loads global descriptor table from gdt_p and sets the segments
 */
void gdt_flush();

/**
 * Loads the interrupt descriptor table from idt_p
 */
void idt_load();

/**
 * Set a breakpoint for bochs
 */
void bochs_break();

/**
 * Switch to page directory dir
 *  (loads cr3 with dir value, and enable paging in cr0)
 */
void switch_pd(dir_t *dir);

/**
 * Switch to current_task page directory (context) and loads it's esp and ebp.
 * It returns to caller, with different context and stack, not jumping.
 * This means that after this call, no local variables can be used!
 * An int_return(&task->regs) must be done to jump to process eip.
 */
void switch_context();

/**
 * Loads registers pointed by r and iret to it's r->eip
 *  Does not return!
 */
void int_return(struct iregs *r);

/**
 * Flush current directory cache
 */
void flush_tlb();

/**
 * An atomically guaranteed lock.
 * If the lock is locked, it will wait until the lock is released.
 * TODO - put the process to sleep and not use infinite loop
 */
uint32_t spin_lock(spin_lock_t *lock);

/**
 * Unlocks a lock; release it
 */
#define spin_unlock(addr) (*(addr) = 0);


/**
 * Invalidates (flush) TLB entry for page that contains addr
 */
void invlpg(virt_t addr);

/**
 * Copy the stack to the new address, and patch it's values
 *  that refers to old stack
 * Resturns the new address
 */
uint32_t patch_stack(uint32_t new_address);

// Yeah, I know, we need to add isr and irq's, but i will define them in
// C files 

#endif
