/*
 * Board specific setup info
 *
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 *
 * Author :
 *	Aneesh V	<aneesh@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */


.globl lowlevel_init
lowlevel_init:

mov sp, lr

#include "sdp_init_basic.S"


/************************************************************
 * Secureboot - auth 'boot0'
 ************************************************************/  
secure_boot_auth1:
    ldr r0, =0x303e8000
    ldr r1, =0x303f0000
    ldr r2, [r1, #0x38]
    ands    r2, r2, #1
    b secure_boot_auth1_bypass

    rsetl   r0, #0x404, r1, 0x1ff0
    rsetl   r0, #0x408, r1, 0x11 

    ldr r2, =0x22220002
    ldr r1, [r0, #0x400]
    cmp r1, r2
    bne secure_boot_auth1

    rsetl   r0, #0x414, r1, 0x01161703

secure_boot_auth1_bypass:


/************************************************************
 * DDR init, SMC sync setting
 ************************************************************/
	mov r3, #0x0 /* cold boot */

#if defined(BOARD_PV)
#include "sdp_init_ddr_sp_es2.S"
#elif defined(BOARD_OD)
#include "sdp_init_ddr_sp_es2.S"
#else /* Pre1, Pre2, DV */
#include "sdp_init_ddr_sp.S"
#endif

#include "v7_inv_dcache_all.S"

@Finish the lowlevel initialization

	mov lr, sp
	mov pc, lr



.ltorg

.balignl 0x400, 0xffffffff
sdp_mmc_8Kbytes_In_Card :
