ENTRY(start)

KERNEL_OFFSET = 0xc0000000;

SECTIONS {
	. = KERNEL_OFFSET + 0x00100000;
    _kernel_virtual_start = .;
    _kernel_physical_start = . - KERNEL_OFFSET; 
    .boot ALIGN(0x1000) : AT (ADDR (.boot) - KERNEL_OFFSET)
	{
		*(.multiboot)
	}
	.text ALIGN(0x1000) : AT (ADDR (.text) - KERNEL_OFFSET)
	{
		*(.text)
	}

	.rodata ALIGN (0x1000) : AT (ADDR (.rodata) - KERNEL_OFFSET)
    {
        *(.rodata*)         /* all read-only data sections from all files */
    }

    /* align at 4 KB and load at 1 MB + . */
    .data ALIGN (0x1000) : AT (ADDR (.data) - KERNEL_OFFSET)
    {
        *(.data)            /* all data sections from all files */
    }

    /* align at 4 KB and load at 1 MB + . */
    .bss ALIGN (0x1000) : AT (ADDR (.bss) - KERNEL_OFFSET)
    {
        *(COMMON)           /* all COMMON sections from all files */
        *(.bss)             /* all bss sections from all files */
    }
    _kernel_virtual_end = .;
    _kernel_physical_end = . - KERNEL_OFFSET;
}
