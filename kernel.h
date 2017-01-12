//#define assert(condition) do { if (!(condition)) { kprintf("assertion fail: " #condition); } } while(0)

//
//	Cateva informatii despre kernel, pasate de linker si extrase cu get_kernel_info din startup.asm
//
struct kinfo_t {
	unsigned int code;	// adresa unde incepe kernelul - 1M
	unsigned int start;	// adresa unde se afla rutina "start" -> entry point-ul in kernel
	unsigned int data;	// adresa unde se afla datele
	unsigned int bss;	// adresa unde se afla stiva
	unsigned int end;	// unde se termina kernelul
	unsigned int size;	// dimensiune kernel == end - code
};

struct kinfo_t kinfo;
extern int get_kernel_info(struct kinfo_t *kinfo);		// definit in startup.asm

