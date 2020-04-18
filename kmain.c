#include "loader.h"
#include "framebuffer.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "multiboot.h"
#include "paging.h"
#include "timer.h"
#include "pfa.h"

#define MULTIBOOT_FLAGS_INVALID "Multiboot invalid flags"
#define MULTIBOOT_INVALID_MOD_COUNT "Multiboot invalid modules count"
#define IDT_INIT_MSG "init idt"

typedef int (*call_module_t)(void);

multiboot_info_t * remap_multiboot(multiboot_info_t * multiboot_info)
{
	module_t * current_module = 0;
	// fixing only the neccassary fields
	multiboot_info_t * updated_info = (multiboot_info_t * )physical_to_virtual((unsigned int)multiboot_info);
	updated_info->mmap_addr = physical_to_virtual(updated_info->mmap_addr);
	updated_info->mods_addr = physical_to_virtual(updated_info->mods_addr);
	current_module = (module_t *)updated_info->mods_addr;
	for (unsigned int i = 0; i < updated_info->mods_count; i++)
	{
		current_module[i].mod_start = physical_to_virtual(current_module[i].mod_start);
		current_module[i].mod_end = physical_to_virtual(current_module[i].mod_end);
		current_module[i].string = physical_to_virtual(current_module[i].string);
	}
	
	return updated_info;
}

int kmain(multiboot_info_t * multiboot_info, kernel_memory_info_t mem, unsigned int pdt_vaddr, unsigned int pt_vaddr)
{
	module_t * current_module = { 0 };
	call_module_t program = 0;
	unsigned int free_vaddr = 0;
	unsigned int free_paddr = 0;
	// we should fix the multiboot info
	multiboot_info = remap_multiboot(multiboot_info);

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

	paging_init(pdt_vaddr, pt_vaddr);
	fb_puts("Paging enabled");
	pfa_init(multiboot_info, &mem); // there is depedency between pfa_alloc and paging_commit_mem 

	free_vaddr = 0x13371000; // paging_find_free_next_kernel_vaddr(PAGE_SIZE * 2);
	paging_commit_mem(free_vaddr, free_paddr, PAGE_SIZE, PAGING_FLAGS_READ_WRITE);

	fb_put_hex(pfa_alloc());
	fb_put_hex(pfa_alloc());
	fb_put_hex(pfa_alloc());


	current_module = (module_t *)multiboot_info->mods_addr;
	program = (call_module_t)current_module->mod_start;

	// we should map into virtual memory the module before jumping 
	fb_puts("Jumping to user mode program at ");
	fb_put_hex(current_module->mod_start);
	fb_puts("\n");
	(void)program();

	fb_puts("This should never be reached \n");
	return 0xDEADBEEF;
}