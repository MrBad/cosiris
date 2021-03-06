#include <sys/types.h>
#include <string.h>
#include "int.h"
#include "console.h"
#include "syscall.h"
#include "task.h"
#include "mem.h"
#include "sysfile.h"
#include "sysproc.h"
#include "cofs.h"

void exit(int status)
{
    task_exit(status);
}
extern pid_t fork_int();
static void *syscalls[] = {
    0,

    &fork_int,
    &task_wait,
    &task_exit,
    &getpid,
    &sys_exec,
    &ps,

    &sys_sbrk,

    &sys_open,
    &sys_close,
    &sys_stat,
    &sys_fstat,
    &sys_read,
    &sys_write,
    &sys_chdir,
    &sys_chroot,
    &sys_chmod,
    &sys_chown,
    &sys_mkdir,
    &cofs_dump_cache,
    &sys_isatty,
    &sys_lseek,
    &sys_opendir,
    &sys_closedir,
    &sys_readdir,
    &sys_lstat,
    &sys_readlink,
    &sys_getcwd,
    &sys_unlink,
    &sys_dup,
    &sys_pipe,
    0,
    &sys_link,
    &sys_rename,
    &sys_kill,
    &sys_signal,
    &sys_ioctl,
    &sys_setsid,
    &sys_time,
    &sys_ftruncate,
};

/**
 * Validates if a user can access a zone of memory
 *  starting at ptr and having size;
 */
void validate_usr_ptr(void *ptr)
{
    uint32_t p = (uint32_t) ptr;
    if (p < USER_STACK_LOW) {
        kill(current_task->pid, SIGSEGV);
        task_switch();
    }
}

static uint32_t syscall_handler(struct iregs *r)
{
    uint32_t num_syscalls = sizeof(syscalls) / sizeof(*syscalls);

    if (r->eax >= num_syscalls) {
        kprintf("No such syscall: %d\n", r->eax);
        r->eax = 0;
        return r->eax;
    }
    void *func = syscalls[r->eax];
    r->eax = call_sys(func, r->ebx, r->ecx, r->edx, r->esi, r->edi);

    return r->eax;
}

void syscall_init()
{
    kprintf("Syscall Init\n");
    int_install_handler(0x80, &syscall_handler);
}
