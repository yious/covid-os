#include "framebuffer.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "multiboot.h"
#include "paging.h"
#include "timer.h"

#define MULTIBOOT_FLAGS_INVALID "Multiboot invalid flags"
#define MULTIBOOT_INVALID_MOD_COUNT "Multiboot invalid modules count"
#define IDT_INIT_MSG "init idt"

typedef int (*call_module_t)(void);

int kmain(multiboot_info_t * multiboot_info)
{
	module_t * current_module = { 0 };
	call_module_t program = 0;

	fb_init();
	fb_puts("Framebuffer done.\n");
	
	serial_init_port(SERIAL_COM1);
	fb_puts("Serial done.\n");

	gdt_init();
	fb_puts("gdt done.\n");

	idt_init();
	fb_puts("idt done. Should be accepting interrupts now\n");

	keyboard_init();
	fb_puts("keyboard done. keystorkes should be recognized\n");

	timer_init(50);
	fb_puts("timer done.\n");

	paging_init();
	fb_puts("Paging enabled");

	// trigger page fault
	*(unsigned int *)(0x80000000) = 1;

	if (check_multiboot_flags(multiboot_info->flags) != 1)
	{
		fb_puts("Invalid flags");
		fb_put_hex(multiboot_info->flags);
		fb_puts("\n");
		serial_write(SERIAL_COM1, (unsigned char*)MULTIBOOT_FLAGS_INVALID, sizeof(MULTIBOOT_FLAGS_INVALID));
		return -1;
	}
	if (0 == multiboot_info->mods_count)
	{
		fb_puts("Invalid module count");
		fb_put_hex(multiboot_info->mods_count);
		fb_puts("\n");
		serial_write(SERIAL_COM1, (unsigned char *)MULTIBOOT_INVALID_MOD_COUNT, sizeof(MULTIBOOT_INVALID_MOD_COUNT));
		return -2;
	}

	current_module = (module_t *)multiboot_info->mods_addr;
	program = (call_module_t)current_module->mod_start;

	fb_puts("Jumping to user mode program at ");
	fb_put_hex(current_module->mod_start);
	fb_puts("\n");
	program();

	fb_puts("This should never be reached");
	return 0xDEADBEEF;
}