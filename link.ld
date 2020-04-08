ENTRY(start)

SECTIONS {
	. = 0xC0100000;
    _kernel_start = .; 
    .boot ALIGN(0x1000) : AT (ADDR (.boot) - 0xC0000000)
	{
		*(.multiboot)
	}
	.text ALIGN(0x1000) : AT (ADDR (.text) - 0xC0000000)
	{
		*(.text)
	}

	.rodata ALIGN (0x1000) : AT (ADDR (.rodata) - 0xC0000000)
    {
        *(.rodata*)         /* all read-only data sections from all files */
    }

    /* align at 4 KB and load at 1 MB + . */
    .data ALIGN (0x1000) : AT (ADDR (.data) - 0xC0000000)
    {
        *(.data)            /* all data sections from all files */
    }

    /* align at 4 KB and load at 1 MB + . */
    .bss ALIGN (0x1000) : AT (ADDR (.bss) - 0xC0000000)
    {
        *(COMMON)           /* all COMMON sections from all files */
        *(.bss)             /* all bss sections from all files */
    }
    _kernel_end = .;
}
