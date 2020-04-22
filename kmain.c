#include "loader.h"
#include "math.h"
#include "framebuffer.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "multiboot.h"
#include "paging.h"
#include "timer.h"
#include "pfa.h"
#include "kmalloc.h"
#include "log.h"

typedef int (*call_module_t)(void);

extern void switch_to_um(unsigned int esp, unsigned int code);

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

static void jump_usermode(module_t * mod_info)
{
	unsigned int stack_paddr = pfa_alloc(1);
	unsigned int user_mode_stack_addr = 0xBFFFFFFB;
	unsigned int user_mode_code_addr = 0;
	unsigned int code_size = mod_info->mod_end - mod_info->mod_start;
	unsigned int physical_code_paddr = pfa_alloc(div_ceil(code_size, PAGE_SIZE));

	paging_commit_mem(user_mode_stack_addr & 0xFFFFF000, stack_paddr, PAGE_SIZE, PAGING_FLAGS_PRIV_USER | PAGING_FLAGS_READ_WRITE);
	paging_commit_mem(user_mode_code_addr &0xFFFFF000, physical_code_paddr, code_size, PAGING_FLAGS_PRIV_USER | PAGING_FLAGS_READ_WRITE);

	paging_commit_mem(mod_info->mod_start, mod_info->mod_start-0xc0000000, code_size, PAGING_FLAGS_PRIV_KERNEL | PAGING_FLAGS_READ_ONLY);
	memcpy((void *)user_mode_code_addr, (void *)mod_info->mod_start, code_size);
	paging_free_mem(mod_info->mod_start, code_size);

	log_info("Jumping to program: \"%s\"\n", mod_info->string);
	log_info("stack paddr: %u, stack vaddr: %p, stack size: %u\n", stack_paddr, user_mode_stack_addr, PAGE_SIZE);
	log_info("code paddr: %u, code vaddr: %p, code size: %u\n", stack_paddr, user_mode_stack_addr, PAGE_SIZE);
	switch_to_um(user_mode_stack_addr, user_mode_code_addr);
}

int kmain(multiboot_info_t * multiboot_info, kernel_memory_info_t mem, unsigned int pdt_vaddr, unsigned int pt_vaddr)
{
	// we should fix the multiboot info
	multiboot_info = remap_multiboot(multiboot_info);

	fb_init();
	fb_puts("Framebuffer done.\n");

	serial_init_port(SERIAL_COM1);
	fb_puts("Serial done.\n");

	log_init();

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

	jump_usermode((module_t *)multiboot_info->mods_addr);

	fb_puts("This should never be reached \n");
	return 0xDEADBEEF;
}