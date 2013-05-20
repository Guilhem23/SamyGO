/*
 *  linux/include/asm-arm/arch-s5h2150/io.h
 *
 *  Copyright (C) 2003 ARM Limited
 *  Copyright (C) 2006 Samsung elctronics co.
 *  Author : ij.jang@samsung.com
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
//#define __io(a)       ((void *)(a))
/* ij.jang : readw/writew macros are used by onenand driver */
//#define __mem_pci(a)    ((void *)(a))

#endif

