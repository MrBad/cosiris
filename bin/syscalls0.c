#include "../include/cosiris.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "include/dirent.h"
#include "sys.h"

#ifndef _uint32_t
#define _uint32_t
typedef unsigned int uint32_t;
#endif

// processes
pid_t fork()
{
	return _syscall0(SYS_FORK);
}

pid_t wait(int *status)
{
	return _syscall1(SYS_WAIT, (uint32_t)status);
}

void exit(int status)
{
	_syscall1(SYS_EXIT, (uint32_t) status);
}

pid_t getpid()
{
	return (pid_t) _syscall0(SYS_GETPID);
}

int exec(char *path, char **argv)
{
	return _syscall2(SYS_EXEC, (uint32_t) path, (uint32_t) argv);
}
void ps()
{
	_syscall0(SYS_PS);
}

// memory
void * sbrk(int increment)
{
	return (void *) _syscall1(SYS_SBRK, increment);
}


// files
int open(char *filename, int flag, int mode) {
	return _syscall3(SYS_OPEN, (uint32_t)filename, flag, mode);
}
int close(unsigned int fd) {
	return _syscall1(SYS_CLOSE, fd);
}
int stat(const char *pathname, struct stat *buf) {
	return _syscall2(SYS_STAT, (uint32_t)pathname, (uint32_t)buf);
}
int fstat(int fd, struct stat *buf) {
	return _syscall2(SYS_FSTAT, fd, (uint32_t)buf);
}
int read(int fd, void *buf, unsigned int count) {
	return _syscall3(SYS_READ, fd, (uint32_t)buf, count);
}
int write(int fd, void *buf, unsigned int count) {
	return _syscall3(SYS_WRITE, fd, (uint32_t)buf, count);
}
int chdir(char *filename) {
	return _syscall1(SYS_CHDIR, (uint32_t)filename);
}
int chroot(char *filename) {
	return _syscall1(SYS_CHROOT, (uint32_t)filename);
}
int chmod(const char *filename, mode_t mode) {
	return _syscall2(SYS_CHMOD, (uint32_t)filename, mode);
}
int chown(char *filename, int uid, int gid) {
	return _syscall3(SYS_CHOWN, (uint32_t)filename, uid, gid);
}
int mkdir(const char *pathname, mode_t mode) {
	return _syscall2(SYS_MKDIR, (uint32_t)pathname, mode);
}
int isatty(int fd) {
	return _syscall1(SYS_ISATTY, fd);
}
off_t lseek(int fd, off_t offset, int whence) {
	return _syscall3(SYS_LSEEK, fd, offset, whence);
}

DIR *sys_opendir(char *dirname) {
	return _syscall1(SYS_OPENDIR, dirname);
}
int sys_closedir(DIR *dir) {
	return _syscall1(SYS_CLOSEDIR, dir);
}
struct dirent *sys_readdir(DIR *dir) {
	return _syscall1(SYS_READDIR, dir);
}


void lstree() {
	_syscall0(SYS_LSTREE);
}
void cofs_dump_cache()
{
	_syscall0(SYS_COFS_DUMP_CACHE);
}