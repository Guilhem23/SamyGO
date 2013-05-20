/*
 * linux/include/asm-arm/arch-s5h2150/pci.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: ij.jang@samsung.com
 *
 */

#ifndef __S5H2150_PCI_H
#define __S5H2150_PCI_H

#define V_PCI(offset)  (*(volatile unsigned *)(V_PCI_BASE+(offset)))

#define O_PCIHID        (0x00)
#define O_PCIHSC        (0x04)
#define O_PCIHCODE      (0x08)
#define O_PCIHLINE      (0x0c)
#define O_PCIHBAR0      (0x10)
#define O_PCIHBAR1      (0x14)
#define O_PCIHBAR2      (0x18)
#define O_PCIHCISP      (0x28)
#define O_PCIHSSID      (0x2c)
#define O_PCIHCAP 	(0x34)
#define O_PCIHLTIT      (0x3c)
#define O_PCIHTIMER     (0x40)
#define O_PCIHPMR0      (0xdc)
#define O_PCIHPMR1      (0xe0)
#define O_PCICON 	(0x100)
#define O_PCISET 	(0x104)
#define O_PCIINTEN      (0x108)
#define O_PCIINTST      (0x10c)
#define O_PCIINTAD      (0x110)
#define O_PCIBATAPM     (0x114)
#define O_PCIBATAPI     (0x118)
#define O_PCIRCC	(0x11c)
#define O_PCIDIAG0      (0x120)
#define O_PCIDIAG1      (0x124)
#define O_PCIBELAP      (0x128)
#define O_PCIBELPA      (0x12c)
#define O_PCIMAIL0      (0x130)
#define O_PCIMAIL1      (0x134)
#define O_PCIMAIL2      (0x138)
#define O_PCIMAIL3      (0x13c)
#define O_PCIBATPA0     (0x140)
#define O_PCIBAM0       (0x144)
#define O_PCIBATPA1     (0x148)
#define O_PCIBAM1       (0x14c)
#define O_PCIBATPA2     (0x150)
#define O_PCIBAM2       (0x154)
#define O_PCISWAP       (0x158)

/* PC Card mode */
#define O_PDMACON       (0x180)
#define O_PDMASRC       (0x184)
#define O_PDMADST       (0x188)
#define O_PDMACNT       (0x18c)
#define O_PDMARUN       (0x190)

#define R_PCIHID        V_PCI(O_PCIHID)
#define R_PCIHSC        V_PCI(O_PCIHSC)
#define R_PCIHCODE      V_PCI(O_PCIHCODE)
#define R_PCIHLINE      V_PCI(O_PCIHLINE)
#define R_PCIHBAR0      V_PCI(O_PCIHBAR0)
#define R_PCIHBAR1      V_PCI(O_PCIHBAR1)
#define R_PCIHBAR2      V_PCI(O_PCIHBAR2)
#define R_PCIHCISP      V_PCI(O_PCIHCISP)
#define R_PCIHSSID      V_PCI(O_PCIHSSID)
#define R_PCIHCAP 	V_PCI(O_PCIHCAP)
#define R_PCIHLTIT      V_PCI(O_PCIHLTIT)
#define R_PCIHTIMER     V_PCI(O_PCIHTIMER)
#define R_PCIHPMR0      V_PCI(O_PCIHPMR0)
#define R_PCIHPMR1      V_PCI(O_PCIHPMR1)
#define R_PCICON 	V_PCI(O_PCICON)
#define R_PCISET 	V_PCI(O_PCISET)
#define R_PCIINTEN      V_PCI(O_PCIINTEN)
#define R_PCIINTST      V_PCI(O_PCIINTST)
#define R_PCIINTAD      V_PCI(O_PCIINTAD)
#define R_PCIBATAPM     V_PCI(O_PCIBATAPM)
#define R_PCIBATAPI     V_PCI(O_PCIBATAPI)
#define R_PCIRCC	V_PCI(O_PCIRCC)
#define R_PCIDIAG0      V_PCI(O_PCIDIAG0)
#define R_PCIDIAG1      V_PCI(O_PCIDIAG1)
#define R_PCIBELAP      V_PCI(O_PCIBELAP)
#define R_PCIBELPA      V_PCI(O_PCIBELPA)
#define R_PCIMAIL0      V_PCI(O_PCIMAIL0)
#define R_PCIMAIL1      V_PCI(O_PCIMAIL1)
#define R_PCIMAIL2      V_PCI(O_PCIMAIL2)
#define R_PCIMAIL3      V_PCI(O_PCIMAIL3)
#define R_PCIBATPA0     V_PCI(O_PCIBATPA0)
#define R_PCIBAM0       V_PCI(O_PCIBAM0)
#define R_PCIBATPA1     V_PCI(O_PCIBATPA1)
#define R_PCIBAM1       V_PCI(O_PCIBAM1)
#define R_PCIBATPA2     V_PCI(O_PCIBATPA2)
#define R_PCIBAM2       V_PCI(O_PCIBAM2)
#define R_PCISWAP       V_PCI(O_PCISWAP)

/* PC Card mode */
#define R_PDMACON       V_PCI(O_PDMACON)
#define R_PDMASRC       V_PCI(O_PDMASRC)
#define R_PDMADST       V_PCI(O_PDMADST)
#define R_PDMACNT       V_PCI(O_PDMACNT)
#define R_PDMARUN       V_PCI(O_PDMARUN)

#endif