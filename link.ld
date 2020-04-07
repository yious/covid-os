ENTRY(start)

SECTIONS {
	. = 1M;

	.boot :
	{
		*(.multiboot)
	}


	.text ALIGN(0x1000) :
	{
		*(.text)
	}

	.rodata ALIGN (0x1000) : 
    {
        *(.rodata*)         /* all read-only data sections from all files */
    }

    /* align at 4 KB and load at 1 MB + . */
    .data ALIGN (0x1000) : 
    {
        *(.data)            /* all data sections from all files */
    }

    /* align at 4 KB and load at 1 MB + . */
    .bss ALIGN (0x1000) : 
    {
        *(COMMON)           /* all COMMON sections from all files */
        *(.bss)             /* all bss sections from all files */
    }

	.pde ALIGN (0x1000000) : 
	{
		*(.pde)
	}
}
