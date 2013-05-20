/**************************************************
 * SDP1002 basics and PMU
 * This file should be embedded within onboot.S
 **************************************************/
#include "sdp_macros.S"
#include "./include/clock.h"
	ldr	r0, =0x30090000
	rsetl	r0, #0x604, r1, 0xa5  @Disable watchdog timer

#ifdef CONFIG_SDP_MAC_RGMII_INTERFACE
	@GMAC set
	rsetl	r0, #0xcd8, r1, 0x1
#endif

/**************************************************
 * PMU setup 
 **************************************************/
	nop
	nop
	nop
#if defined(SILENT_KERNEL)
	ldr r0, =0x30b70000
	ldr   r1, [r0, #0x30]
	bic   r1, r1, #0xf
	str   r1, [r0, #0x30]
#endif
	ldr	r0, =CLKRST_BASE

	@ PLL PMS Setting
	rsetl	r0, #0x000, r1, PMU_PMS_ARM
	rsetl	r0, #0x004, r1, PMU_PLL1_PMS_BUS
	rsetl	r0, #0x008, r1, PMU_PLL2_PMS_GPU
	rsetl	r0, #0x00c, r1, PMU_PLL3_PMS_DSP
	rsetl	r0, #0x010, r1, PMU_PLL4_PMS_AUD0
	rsetl	r0, #0x014, r1, PMU_PLL5_PMS_AUD1
	rsetl	r0, #0x018, r1, PMU_PLL6_PMS_LVDS
	rsetl	r0, #0x01c, r1, PMU_PLL7_PMS_PULL
	rsetl	r0, #0x020, r1, PMU_PMS_DDR
	rsetl	r0, #0x024, r1, PMU_PLL9_PMS_3D
	rsetl	r0, #0x028, r1, PMU_PMS_BUS
	
	@ PLL_K
	rsetl	r0, #0x02c, r1, PLL3_K_DSP
	rsetl	r0, #0x030, r1, PLL4_K_AUD0
	rsetl	r0, #0x034, r1, PLL5_K_AUD1
	rsetl	r0, #0x038, r1, PLL6_K_LVDS
	rsetl	r0, #0x03c, r1, PLL7_K_PULL
	rsetl	r0, #0x040, r1, PMU_K_DDR
	rsetl	r0, #0x044, r1, PLL9_K_3D
	
	@PLL Ctrl
	rsetl	r0, #0x048, r1, PLL0_CTRL
	rsetl	r0, #0x04c, r1, PLL1_CTRL
	rsetl	r0, #0x050, r1, PLL2_CTRL
	rsetl	r0, #0x054, r1, PLL3_CTRL
	rsetl	r0, #0x058, r1, PLL4_CTRL
	rsetl	r0, #0x05c, r1, PLL5_CTRL
	rsetl	r0, #0x060, r1, PLL6_CTRL
	rsetl	r0, #0x064, r1, PLL7_CTRL
	rsetl	r0, #0x068, r1, PMU_CTL_DDR
	rsetl	r0, #0x06c, r1, PLL9_CTRL
	rsetl	r0, #0x070, r1, PLL10_CTRL

	@ PLL 7 ETC
	rsetl	r0, #0x074, r1, PLL7_ETC

	@PLL Lock counter setting
	rsetl	r0, #0x078, r1, PLL0_LOCKCNT
	rsetl	r0, #0x07c, r1, PLL1_LOCKCNT
	rsetl	r0, #0x080, r1, PLL2_LOCKCNT
	rsetl	r0, #0x084, r1, PLL3_LOCKCNT
	rsetl	r0, #0x088, r1, PLL4_LOCKCNT
	rsetl	r0, #0x08c, r1, PLL5_LOCKCNT
	rsetl	r0, #0x090, r1, PLL6_LOCKCNT
	rsetl	r0, #0x09c, r1, PLL7_LOCKCNT
	rsetl	r0, #0x0a0, r1, PLL8_LOCKCNT
	rsetl	r0, #0x0a4, r1, PLL9_LOCKCNT
	rsetl	r0, #0x0a8, r1, PLL10_LOCKCNT
	
	@PLL bypass off
	rsetl	r0, #0x0b4, r1, 0x0

	@PLL power on (off the PWRDN bit) GPU PLL off
	rsetl	r0, #0x0b0, r1, 0x5ff

	@Wait for locking time...
Locking_PLL1 :
	ldr	r1, [r0, #0xac]
	ldr	r2, =0xff
	and	r1, r2
	cmp	r1, r2
	bne	Locking_PLL1 

	nop


/********************11.07.20 18:00 request modify Audio Team*************************/
	@Insert Audio PLL Sequence
	
	ldr	r0, =CLKRST_BASE
	
	ldr	r1, [r0, #0x0a4]
	bic	r1, r1, #0x32
	str	r1, [r0, #0x0a4]				@ [1], [5:4] bit을 0으로 setting. PLL power down

	rsetl	r0, #0x004, r1, 0x00308c00 	@ AE PLL
	rsetl	r0, #0x010, r1, 0x00308303 	@ PLL AUD0 PMS, 48KHz
	rsetl	r0, #0x030, r1, 0x0000126F 	@ PLL AUD0 K, 48KHz
	rsetl	r0, #0x014, r1, 0x00308303 	@ PLL AUD1 PMS, 48KHz
	rsetl	r0, #0x034, r1, 0x0000126F 	@ PLL AUD1 K, 48KHz

	rsetl	r0, #0x04C, r1, 0x10380001	@ AE PLL ctrl

	rsetl	r0, #0x058, r1, 0x00000008 	@ PLL AUD0 ctrl
	rsetl	r0, #0x05C, r1, 0x00000008 	@ PLL AUD1 ctrl
	rsetl	r0, #0x078, r1, 0x00007FFF 	@ AE PLL lock cnt(약 2.7mS)
	rsetl	r0, #0x084, r1, 0x00007FFF 	@ PLL AUD0 lock cnt(약 2.7mS)
	rsetl	r0, #0x088, r1, 0x00007FFF 	@ PLL AUD1 lock cnt(약 2.7mS)

	ldr	r1, [r0, #0x0a8]
	bic	r1, r1, #0x32
	str	r1, [r0, #0x0a8]				@ [1], [5:4] bit을 0으로 setting. clk을 bypass하지 않고 분주함
	
	ldr	r1, [r0, #0x0a4]
	orr	r1, r1, #0x32
	str	r1, [r0, #0x0a4]				@ [1], [5:4] bit을 1으로 setting. PLL power on

/********************11.07.20 18:00 request modify Audio Team*************************/


	@Clock source selection. for front mux ( set to PLL output )
	ldr	r0, =CLKRST_SEL_BASE

	rsetl	r0, #0x014, r1, 0x00008000 @ MUX_SEL1
	rsetl	r0, #0x038, r1, 0xB8008FF7 @ MUX_SEL10
	rsetl	r0, #0x034, r1, 0x0011FF8E @ MUX_SEL9
	rsetl	r0, #0x030, r1, 0xFFFFFFFF @ MUX_SEL8

	@clkrst_prescl_on, val0~2
	rsetl	r0, #0x000, r1, 0x0007ffff 
	rsetl	r0, #0x004, r1, 0x00000000
	rsetl	r0, #0x008, r1, 0x00012299
	rsetl	r0, #0x00c, r1, 0x00004222

	@SW Reset
	rsetl	r0, #0x054, r1, 0x67fdffff @s/w reset0	HCI off, PCIe Off
	rsetl	r0, #0x058, r1, 0x9ffffff9 @s/w reset1	 AIO/AEC reset 풀어줌, AE는 reset 상태, PCIe Off

	@LVDS Power Down ;김성환 책임 요청(11/07/20 14:04)
	ldr	r0, =0x30090000
	rsetl	r0, #0xc84, r1, 0x0000c0c5 @LVDS PWDN

	@unused LVDS clock off
	ldr	r1, =0x3FF
	str	r1, [r0, #0x950]

	@PCIe clock off
	ldr	r1, [r0, #0x94c]
	bic	r1, r1, #0x0C000000
	str	r1, [r0, #0x94c]	

	@I2C5_SEL ;나덕균 책임 요청(2011/08/29)
	@I2C6_SEL enable requested by jongpo.lee (2011/10/05)
    rsetl   r0, #0xc50, r1, 0x000000C0 

/**************************************************
 * USB ohci/ehci settings 
 **************************************************/
	@ PHY tune register
 	ldr r0, =0x30090000
	rsetl	r0, #0xcb0, r1, 0x7b30dc95 @ 24MHz CLKCORE
	rsetl	r0, #0xcb4, r1, 0x7b30dc95 @ 24MHz CLKCORE
	rsetl	r0, #0xcbc, r1, 0x00000909 @pico_phy

	@ host controller register
	rsetl	r0, #0xcb8, r1, 0x0e050e05 @ burst 16, 8, 4

	@ Top S/W reset (see default value, [20]:usb0, [21]:usb1)
	@ldr r1, [r0, #0x954]
	@orr r1, r1, #0x00300000
	@str r1, [r0, #0x954]

	@ Buffer threshold
	@ldr r0, =0x30070000
	@rsetl	r0, #0x0094 , r1,  0x00800020
	@ldr r0, =0x30080000
	@rsetl	r0, #0x0094 , r1,  0x00800020

	@ USB VBUS VALID
	ldr r0, =0x30090000
	ldr r1, [r0, #0xdb4]
	orr r1, r1, #0x33000000
	str r1, [r0, #0xdb4]

	ldr r1, [r0, #0xdb8]
	orr r1, r1, #0x000000c0
	str r1, [r0, #0xdb8]

	@Audio I/O pad strength ; requested by wert
	ldr r0, =0x30090000
	ldr r1, [r0, #0xc5c]
	bic	r1, r1, #0x3
	str	r1, [r0, #0xc5c]
	
    /* gpio0.0 : LAN NRESET low, gpio0.1 : USB_ENABLE low*/
    ldr r1, [r0, #0xd00]
    orr r1, r1, #0x33
    str r1, [r0, #0xd00]

    ldr r1, [r0, #0xd04]
    bic r1, r1, #0x3
    str r1, [r0, #0xd04]	

    /* gpio1.1 : USB_NRESET, high */
    ldr r1, [r0, #0xd0c]
    orr r1, r1, #0x30
    str r1, [r0, #0xd0c]

    ldr r1, [r0, #0xd10]
    orr r1, r1, #0x2
    str r1, [r0, #0xd10]

#if defined(COUNTRY_JP)
    /* gpio5.2 : USB2_NRESET, high */
    ldr r1, [r0, #0xd3C]
    orr r1, r1, #0x300
    str r1, [r0, #0xd3C]

    ldr r1, [r0, #0xd40]
    orr r1, r1, #0x4
    str r1, [r0, #0xd40]
#else
    /* gpio6.0 : USB2_NRESET, high */
    ldr r1, [r0, #0xd48]
    orr r1, r1, #0x3
    str r1, [r0, #0xd48]

    ldr r1, [r0, #0xd4c]
    orr r1, r1, #0x1
    str r1, [r0, #0xd4c]
#endif

#if !defined(COUNTRY_JP)
    /* gpio6.1 : ATS_5V_EN, low */
    ldr r1, [r0, #0xd48]
    orr r1, r1, #0x30
    str r1, [r0, #0xd48]

    ldr r1, [r0, #0xd4c]
    bic r1, r1, #0x2
    str r1, [r0, #0xd4c]
#endif

#if defined(USE_PTM)
	/* ETM_SEL set to 1 */
	ldr    r1, [r0, #0xc50]
	orr    r1, r1, #0x800000
	str    r1, [r0, #0xc50]	
#endif

	/* HI-Z off for All Out-Port */
    rsetl   r0, #0xC54, r1, 0x00000001 @ HI_Z mode setting	
