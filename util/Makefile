INCLUDE=.
LIBS=
CC=gcc
OFLAGS=-c
CFLAGS=-g -Wall -Wextra -std=gnu99 -pedantic-errors -I../ -I$(INCLUDE)

OBJECTS=find_func.o

TARGET=find_func

all: find_func mkinitrd mkcofs

find_func: find_func.o
	$(CC) $(CFLAGS) -o find_func find_func.o

mkinitrd: mkinitrd.o
	$(CC) $(CFLAGS) -o mkinitrd mkinitrd.o

mkcofs:	mkcofs.o ../cofs.h
	$(CC) $(CFLAGS) -o mkcofs mkcofs.o

%.o: %.c Makefile *.h
	$(CC) $(CFLAGS) $(OFLAGS) -o $@ $<

clean:
	rm *.o find_func mkinitrd mkcofs
