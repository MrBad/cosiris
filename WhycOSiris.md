# Why cOSiris? #


Back in 2004 i started to write my first pet operating system which i called it OSiris, in order to understand the intimate mechanism of i386 CPU. It was coded in assembler, using NASM to compile all. After working few months on it, the project was abandonated because lack of time and because i was moving to web technologies.

Few months ago a friend of mine brought me an old 486/33Mhz SX computer. I said Wow! after booting it up. I remembered of my old OSiris project, i compiled it and tried that good old machine. After almoust 20 years, the ingenious 486 still runs. That ideea to understand the i386 cpu came again into my mind. I tried to read the old source code and i realized that assembler is a little bit too hard to understand, after 6 years of PHP/MySQL. So i started to write a small kernel, this time in C and few lines of assembler, and cOSiris (from C/C++ OSiris) was born.

The aim of this small kernel is to understand the i386 CPU. It will use as little as possible the assembler language. The kernel is not very well documented, a lot of comments are in Romanian language but i hope i will try to translate/remove them. Right now it can boot up by using Grub bootloader, it has a small libc like library, it sets up GDT, IDT, ISRs, IRQs and the timer. It uses paging mechanisms of i386 for memory protection.

It doesn't use multitasking yet, but that will be implemented in the next versions.
Inside sources there is a bochsrc file for running it in bochs emulator. Because i am coding it remote using ssh/mcedit to my home "server", bochs is configured to use textconfig interface.
For developing i am using debian lenny, and default developing packages in this distro: gcc (Debian 4.3.2-1.1) 4.3.2, nasm 2.03.01-1, make 3.81-5, bochs 2.3.7-1 with bochs-term 2.3.7-1.

In order to compile the sources, type "make" inside sources directory.
To run the bochs emulator, type "make run";
To write a 1.44 floppy disk, type "make fd" and reboot.
To clean up things, of corse, "make clean";

The current version was booted ok on:
- 486SX/33Mhz, 8MB RAM
- P1/90Mhz, 24MB RAM
- duron 800Mhz/256MB RAM
The cpu must be a min 386, and the machine must have at least 2MB RAM(although i did not test it on a 386)

I must apologise for my english - i am not a native speaker.
So, happy hacking!