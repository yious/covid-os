#include "framebuffer.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "multiboot.h"

#define MULTIBOOT_FLAGS_INVALID "Multiboot invalid flags"
#define MULTIBOOT_INVALID_MOD_COUNT "Multiboot invalid modules count"
#define IDT_INIT_MSG "init idt"

typedef int (*call_module_t)(void);

int kmain(multiboot_info_t * multiboot_info)
{
	unsigned char msg[] = "Framebuffer initialized!";
	unsigned char msg2[] = "Initializing serial";
	unsigned char msg3[] = "Initializing gdt";
	unsigned char msg4[] = "Serial done.";
	unsigned char msg_done[] = "Done.";
	module_t * current_module = { 0 };
	call_module_t program = 0;

	fb_init();
	fb_write(msg, sizeof(msg) - 1);
	fb_set_cursor_pos(FB_COLS * 1);

	
	fb_write(msg2, sizeof(msg2) - 1);
	fb_set_cursor_pos(FB_COLS * 2);
	serial_init_port(SERIAL_COM1);
	serial_write(SERIAL_COM1, msg4 , sizeof(msg4));


	fb_write(msg3, sizeof(msg3) - 1);
	fb_set_cursor_pos(FB_COLS * 3);
	gdt_init();

	fb_write((unsigned char *)IDT_INIT_MSG, sizeof(IDT_INIT_MSG) - 1);
	idt_init(); 

	fb_write(msg_done, sizeof(msg_done) - 1);
	fb_set_cursor_pos(FB_COLS * 4);
	
	/* Check if multiboot header is valid */
	if (check_multiboot_flags(multiboot_info->flags) != 1)
	{
		serial_write(SERIAL_COM1, (unsigned char*)MULTIBOOT_FLAGS_INVALID, sizeof(MULTIBOOT_FLAGS_INVALID));
		return -1;
	}
	if (0 == multiboot_info->mods_count)
	{
		serial_write(SERIAL_COM1, (unsigned char *)MULTIBOOT_INVALID_MOD_COUNT, sizeof(MULTIBOOT_INVALID_MOD_COUNT));
		return -2;
	}
	current_module = (module_t *)multiboot_info->mods_addr;
	program = (call_module_t)current_module->mod_start;
	program();
	return 0xDEADBEEF;
}