#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <sys/stat.h>

void print(char *str);
pid_t fork();
pid_t wait(int *status);
void exit(int status);
pid_t getpid();
int exec(char *path, char **argv);
int execvp(char *path, char **argv);	   
void ps();
void lstree();

void * sbrk(int increment);

// files //
int open(const char *filename, int flag, int mode);
int close(unsigned int fd);
int stat(const char *pathname, struct stat *buf);
int fstat(int fd, struct stat *buf);
int read(int fd, void *buf, unsigned int count);
int write(int fd, void *buf, unsigned int count);
int chdir(char *filename);
int chroot(char *filename);
//int chmod(char *filename, mode_t mode);
int chown(char *filename, int uid, int gid);
int mkdir(const char *pathname, mode_t mode);
int unlink(char *filename);
int dup(int oldfd);
int isatty(int fd);
off_t lseek(int fd, off_t offset, int whence);
int getcwd(char *buf, size_t size);

void cofs_dump_cache();
int pipe(int fd[2]);
int link(char *oldname, char *newname);
int rename(char *oldname, char *newname);

#endif
