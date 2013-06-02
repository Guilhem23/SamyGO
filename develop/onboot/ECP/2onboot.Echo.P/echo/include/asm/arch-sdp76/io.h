/*
 *  linux/include/asm-arm/arch-s5h2111/io.h
 *
 *  Copyright (C) 2003 ARM Limited
 *  Copyright (C) 2006 Samsung elctronics co.
 *  Author : tukho.kim@samsung.com
 *
 * modification history
 * --------------------
 * 15,Sep,2006 ij.jang : Add __mem_pci macro
 * 9 ,Nov,2006 ij.jang : modify __io macro, Add some macro for PCI
 */

#ifndef __ASM_ARM_ARCH_IO_H
#define __ASM_ARM_ARCH_IO_H

#define PCI_IO_ADDR                     0x5C000000

#define __io(a)         ((void *)(PCI_IO_ADDR + (a)))
#if 0
#define __mem_pci(a)    ((void *)(a))
#endif
#endif

