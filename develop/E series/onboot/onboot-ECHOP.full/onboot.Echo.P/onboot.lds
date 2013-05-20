/*
 * One boot link script
 */
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = ALIGN(4);
	.text :
	{
		onboot.o (.text)
		./echo/init_sdp_mmc_core.o	(.text)
		./echo/init_sdp_mmc.o	(.text)
		./echo/lowlevel_init.o	(.text) 
		*(.text)
	}

	. = ALIGN(4);
	.rodata :
	{
		*(.rodata)
	}

	. = ALIGN(4);
	.data :
	{
		*(.data)
	}

	. = ALIGN(4);
	.got :
	{
		*(.got)
	}


	oneboot_prg_end = .;

	. = ALIGN(4);
	__bss_start = .;
	.bss :
	{
		*(.bss)
		*(COMMON)
	}
	_end = .;
}

