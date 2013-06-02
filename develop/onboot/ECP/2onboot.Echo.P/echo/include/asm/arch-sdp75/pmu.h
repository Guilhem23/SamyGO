/*
 * linux/include/asm-arm/arch-sdp75/pmu.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: tukho.kim@samsung.com
 *
 */

#ifndef _SDP75_PMU_H_
#define _SDP75_PMU_H_

/* clock & power management */
#define V_PMU(offset)    (*(volatile unsigned *)(V_PMU_BASE+(offset)))

#define	O_CLK_EN0		(0x00)
#define O_CLK_EN1		(0x04)
#define O_SW_RST0		(0x08)
#define O_SW_RST1		(0x0C)
#define O_CLK_SRC_SEL		(0x10)
/* reserved 0x14 	*/
#define O_TSD_CLK_MUX		(0x18)
#define O_AUD_PS0_5_ON		(0x1C)
#define	O_AUD_PS0_1_VAL         (0x20)
#define	O_AUD_PS2_3_VAL         (0x24)
#define	O_AUD_PS4_5_VAL         (0x28)
#define	O_AUD_MCLK_SEL          (0x2C)
#define O_MODULE_CLK_SEL		(0x30)
#define O_CAPTURE_CON           (0x34)
/* reserved 0x38, 0x3C 	*/
#define O_CLKRST_GDR0			(0x40)
#define O_CLKRST_GDR1           (0x44)
#define O_PLL0_PMS_CON          (0x48)
#define O_PLL1_PMS_CON          (0x4C)
#define O_PLL2_PMS_CON          (0x50)
#define O_PLL3_PMS_CON			(0x54)
#define O_PLL4_PMS_CON          (0x58)
#define O_PLL5_PMS              (0x5C)
#define O_PLL5_K_CON            (0x60)
#define O_PLL6_PMS              (0x64)
#define O_PLL6_K_CON		(0x68)
#define O_PLL7_PMS              (0x6C)
#define O_PLL7_K_CON            (0x70)
/* reserved 0x74 	*/
#define O_MCS_VAL		(0x78)
#define O_EMA_VAL0              (0x7C)
#define O_EMA_VAL1              (0x80)
#define O_EMA_VAL2              (0x84)
#define O_EMA_VAL3              (0x88)
/* reserved 0x8C ~ 0x94	*/
#define O_PAD_CD_VAL0		(0x98)
#define O_PAD_CD_VAL1           (0x9C)
#define O_USB0_PHY_TUNE         (0xA0)
#define O_USB1_PHY_TUNE         (0xA4)
#define O_PLL0_1_LOCK_CNT       (0xA8)
#define O_PLL2_3_LOCK_CNT	(0xAC)	
#define O_PLL4_5_LOCK_CNT       (0xB0)
#define O_PLL6_7_LOCK_CNT       (0xB4)
#define O_PLL0_7_LOCK_ST        (0xB8)
#define O_DDR_PHY_DLL_CON       (0xBC)

/*************************************
 * define for 'C'
 */
#define	R_CLK_EN0			V_PMU(O_CLK_EN0    )
#define R_CLK_EN1			V_PMU(O_CLK_EN1    )
#define R_SW_RST0			V_PMU(O_SW_RST0    )
#define R_SW_RST1			V_PMU(O_SW_RST1    )
#define R_CLK_SRC_SEL		V_PMU(O_CLK_SRC_SEL)
/* reserved 0x14 	*/
#define R_TSD_CLK_MUX		V_PMU(O_TSD_CLK_MUX  )
#define R_AUD_PS0_5_ON		V_PMU(O_AUD_PS0_5_ON  )
#define	R_AUD_PS0_1_VAL     V_PMU(O_AUD_PS0_1_VAL )
#define	R_AUD_PS2_3_VAL     V_PMU(O_AUD_PS2_3_VAL )
#define	R_AUD_PS4_5_VAL     V_PMU(O_AUD_PS4_5_VAL )
#define	R_AUD_MCLK_SEL     	 V_PMU(O_AUD_MCLK_SEL  )
#define R_MODULE_CLK_SEL	V_PMU(O_MODULE_CLK_SEL)
#define R_CAPTURE_CON      	 V_PMU(O_CAPTURE_CON   )
/* reserved 0x38, 0x3C 	*/
#define R_CLKRST_GDR0		V_PMU(O_CLKRST_GDR0 )
#define R_CLKRST_GDR1           V_PMU(O_CLKRST_GDR1 )
#define R_PLL0_PMS_CON          V_PMU(O_PLL0_PMS_CON)
#define R_PLL1_PMS_CON          V_PMU(O_PLL1_PMS_CON)
#define R_PLL2_PMS_CON          V_PMU(O_PLL2_PMS_CON)
#define R_PLL3_PMS_CON		V_PMU(O_PLL3_PMS_CON)
#define R_PLL4_PMS_CON          V_PMU(O_PLL4_PMS_CON)
#define R_PLL5_PMS              V_PMU(O_PLL5_PMS    )
#define R_PLL5_K_CON            V_PMU(O_PLL5_K_CON  )
#define R_PLL6_PMS              V_PMU(O_PLL6_PMS    )
#define R_PLL6_K_CON		V_PMU(O_PLL6_K_CON  )
#define R_PLL7_PMS              V_PMU(O_PLL7_PMS    )
#define R_PLL7_K_CON            V_PMU(O_PLL7_K_CON  )
/* reserved 0x74 	*/
#define R_MCS_VAL			V_PMU(O_MCS_VAL )
#define R_EMA_VAL0              V_PMU(O_EMA_VAL0)
#define R_EMA_VAL1              V_PMU(O_EMA_VAL1)
#define R_EMA_VAL2              V_PMU(O_EMA_VAL2)
#define R_EMA_VAL3              V_PMU(O_EMA_VAL3)
/* reserved 0x8C ~ 0x94	*/
#define R_PAD_CD_VAL0			V_PMU(O_PAD_CD_VAL0  	)
#define R_PAD_CD_VAL1           V_PMU(O_PAD_CD_VAL1     )
#define R_USB0_PHY_TUNE         V_PMU(O_USB0_PHY_TUNE   )
#define R_USB1_PHY_TUNE         V_PMU(O_USB1_PHY_TUNE   )
#define R_PLL0_1_LOCK_CNT       V_PMU(O_PLL0_1_LOCK_CNT )
#define R_PLL2_3_LOCK_CNT	V_PMU(O_PLL2_3_LOCK_CNT )	
#define R_PLL4_5_LOCK_CNT       V_PMU(O_PLL4_5_LOCK_CNT )
#define R_PLL6_7_LOCK_CNT       V_PMU(O_PLL6_7_LOCK_CNT )
#define R_PLL0_7_LOCK_ST        V_PMU(O_PLL0_7_LOCK_ST  )
#define R_DDR_PHY_DLL_CON       V_PMU(O_DDR_PHY_DLL_CON )

/* P [0:5], M [8:17], S [20:22] */

#define GET_PLL_P(x)			(x & 0x1F)
#define GET_PLL_M(x)			((x >> 8) & 0x3FF)
#define GET_PLL_S(x)			((x >> 20) & 0x7)
#define PLL_ON(x)			((x >> 24) & 0x1) 
#define PLL_BYPASS(x)			((x >> 25) & 0x1)

#define PLL1_PMS		PLL6156_PMS_333_0
#define PLL2_PMS		PLL6156_PMS_225
#define PLL3_PMS		PLL6156_PMS_400_0
#define PLL4_PMS		PLL6156_PMS_500


#define PLL6156_PMS_210			((1 << 24) | (6 << 0) | (186 << 8) | (2 << 20))  
#define PLL6156_PMS_225			((1 << 24) | (6 << 0) | (200 << 8) | (2 << 20))  
#define PLL6156_PMS_270			((1 << 24) | (6 << 0) | (240 << 8) | (2 << 20))  
#define PLL6156_PMS_277_0		((1 << 24) | (6 << 0) | (246 << 8) | (2 << 20))  
#define PLL6156_PMS_277_1		((1 << 24) | (6 << 0) | (247 << 8) | (2 << 20))  

#define PLL6156_PMS_333_0		((1 << 24) | (7 << 0) | (345 << 8) | (2 << 20))  
#define PLL6156_PMS_333_1		((1 << 24) | (6 << 0) | (296 << 8) | (2 << 20))  
#define PLL6156_PMS_333_2		((1 << 24) | (7 << 0) | (346 << 8) | (2 << 20))  

#define PLL6156_PMS_400_0		((1 << 24) | (6 << 0) | (355 << 8) | (2 << 20))  
#define PLL6156_PMS_400_1		((1 << 24) | (6 << 0) | (178 << 8) | (1 << 20))  
#define PLL6156_PMS_500_0		((1 << 24) | (7 << 0) | (259 << 8) | (1 << 20))  
#define PLL6156_PMS_500_1		((1 << 24) | (6 << 0) | (222 << 8) | (1 << 20))  

#define PLL6156_PWRDN			(1 << 24)


#define PLL6504_PMS_74P25		((2 << 0) | (44 << 8) | (3 << 16))
#define PLL6504_K_74P25			((1 << 16) | (0 << 0))

#define PLL6504_PMS_147P456		((2 << 0) | (43 << 8) | (2 << 16))
#define PLL6504_K_147P456		((1 << 16) | (45264 << 0))

#define PLL6504_PMS_27			((2 << 0) | (32 << 8) | (4 << 16))
#define PLL6504_K_27			((1 << 16) | (0 << 0))

#define PLL6504_PWRDN			(1 << 16)

#endif
