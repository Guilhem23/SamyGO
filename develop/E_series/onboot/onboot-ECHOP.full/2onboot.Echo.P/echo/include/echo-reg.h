#ifndef	_SDP_1004_H_
#define	_SDP_1004_H_

/*
 * linux/include/asm-arm/arch-ssdtv/sdp92.h
 *
 *	Copyright (C) 2003-2008 Samsung Electronics
 *	Author: tukho.kim@samsung.com - 06.16.2008
 *
 */



#define REG_PUT(ADDR, DATA) 	ADDR = DATA
#define REG_GET(ADDR)			ADDR

#define DIFF_IO_BASE0		0
/*
 * SDP83 internal registers
 *
 * Be careful !!!!
 * PAGE_OFFSET + VMALLOC_END = 0xD0000000
 */
/* Static Memory Controller Register */
#define PA_SMC_BASE 		  (0x30028000)
#define VA_SMC_BASE 		  (PA_SMC_BASE + DIFF_IO_BASE0)

/* USB EHCI0 host controller register */
#define PA_EHCI0_BASE		  (0x30070000)
#define VA_EHCI0_BASE		  (PA_EHCI0_BASE + DIFF_IO_BASE0)

/* USB OHCI0 host controller register */
#define PA_OHCI0_BASE		  (PA_EHCI0_BASE + 0x8000)
#define VA_OHCI0_BASE		  (PA_OHCI0_BASE + DIFF_IO_BASE0)

/* USB EHCI1 host controller register */
#define PA_EHCI1_BASE		  (0x30080000)
#define VA_EHCI1_BASE		  (PA_EHCI1_BASE + DIFF_IO_BASE0)

/* USB OHCI1 host controller register */
#define PA_OHCI1_BASE		  (PA_EHCI1_BASE + 0x8000)
#define VA_OHCI1_BASE		  (PA_OHCI1_BASE + DIFF_IO_BASE0)

/* Timer Register */
#define PA_TIMER_BASE		  (0x30090400)
#define VA_TIMER_BASE		  (PA_TIMER_BASE + DIFF_IO_BASE0)

/* Watchdog Register */
#define PA_WDT_BASE 		  (0x30090600)
#define VA_WDT_BASE 		  (PA_WDT_BASE + DIFF_IO_BASE0)

/* Power Management unit & PLL Register */
#define PA_PMU_BASE 		  (0x30090800)
#define VA_PMU_BASE 		  (PA_PMU_BASE + DIFF_IO_BASE0)

/* UART Register */
#define PA_UART_BASE		  (0x30090A00)
#define VA_UART_BASE		  (PA_UART_BASE + DIFF_IO_BASE0)

/* Pad Control Register */
#define PA_PADCTRL_BASE 	  (0x30090C00)
#define VA_PADCTRL_BASE 	  (PA_GPIO_BASE + DIFF_IO_BASE0)

/* Interrupt controller register */
#define PA_INTC_BASE		  (0x30090F00)
#define VA_INTC_BASE		  (PA_INTC_BASE + DIFF_IO_BASE0)

/* UART */
/* Don't need register define */
/* Need Physical Base address */
#define VA_UART(dt,  n, offset) \
	(*(volatile dt *)(VA_UART_BASE+(0x40*n)+(offset)))

#define VA_UART_0(offset)	VA_UART(unsigned, 0, offset)
#define VA_UART_1(offset)	VA_UART(unsigned, 1, offset)
#define VA_UART_2(offset)	VA_UART(unsigned, 2, offset)
#define VA_UART_3(offset)	VA_UART(unsigned, 3, offset)

#define VA_UART_0_B(offset) VA_UART(char, 0, offset)
#define VA_UART_1_B(offset) VA_UART(char, 1, offset)
#define VA_UART_2_B(offset) VA_UART(char, 2, offset)
#define VA_UART_3_B(offset) VA_UART(char, 3, offset)

#define R_ULCON0			VA_UART_0(0x00) /* UART 0 Line control */
#define R_UCON0 			VA_UART_0(0x04) /* UART 0 Control */
#define R_UFCON0			VA_UART_0(0x08) /* UART 0 FIFO control */
#define R_UMCON0			VA_UART_0(0x0c) /* UART 0 Modem control */
#define R_UTRSTAT0			VA_UART_0(0x10) /* UART 0 Tx/Rx status */
#define R_UERSTAT0			VA_UART_0(0x14) /* UART 0 Rx error status */
#define R_UFSTAT0			VA_UART_0(0x18) /* UART 0 FIFO status */
#define R_UMSTAT0			VA_UART_0(0x1c) /* UART 0 Modem status */
#define R_UBRDIV0			VA_UART_0(0x28) /* UART 0 Baud rate divisor */

#define R_ULCON1			VA_UART_1(0x00) /* UART 1 Line control */
#define R_UCON1 			VA_UART_1(0x04) /* UART 1 Control */
#define R_UFCON1			VA_UART_1(0x08) /* UART 1 FIFO control */
#define R_UMCON1			VA_UART_1(0x0c) /* UART 1 Modem control */
#define R_UTRSTAT1			VA_UART_1(0x10) /* UART 1 Tx/Rx status */
#define R_UERSTAT1			VA_UART_1(0x14) /* UART 1 Rx error status */
#define R_UFSTAT1			VA_UART_1(0x18) /* UART 1 FIFO status */
#define R_UMSTAT1			VA_UART_1(0x1c) /* UART 1 Modem status */
#define R_UBRDIV1			VA_UART_1(0x28) /* UART 1 Baud rate divisor */

#define R_ULCON2			VA_UART_2(0x00) /* UART 2 Line control */
#define R_UCON2 			VA_UART_2(0x04) /* UART 2 Control */
#define R_UFCON2			VA_UART_2(0x08) /* UART 2 FIFO control */
#define R_UMCON2			VA_UART_2(0x0c) /* UART 2 Modem control */
#define R_UTRSTAT2			VA_UART_2(0x10) /* UART 2 Tx/Rx status */
#define R_UERSTAT2			VA_UART_2(0x14) /* UART 2 Rx error status */
#define R_UFSTAT2			VA_UART_2(0x18) /* UART 2 FIFO status */
#define R_UMSTAT2			VA_UART_2(0x1c) /* UART 2 Modem status */
#define R_UBRDIV2			VA_UART_2(0x28) /* UART 2 Baud rate divisor */

#define R_ULCON3			VA_UART_3(0x00) /* UART 3 Line control */
#define R_UCON3 			VA_UART_3(0x04) /* UART 3 Control */
#define R_UFCON3			VA_UART_3(0x08) /* UART 3 FIFO control */
#define R_UMCON3			VA_UART_3(0x0c) /* UART 3 Modem control */
#define R_UTRSTAT3			VA_UART_3(0x10) /* UART 3 Tx/Rx status */
#define R_UERSTAT3			VA_UART_3(0x14) /* UART 3 Rx error status */
#define R_UFSTAT3			VA_UART_3(0x18) /* UART 3 FIFO status */
#define R_UMSTAT3			VA_UART_3(0x1c) /* UART 3 Modem status */
#define R_UBRDIV3			VA_UART_3(0x28) /* UART 3 Baud rate divisor */

#define R_UTXH0 		VA_UART_0_B(0x20) /* UART 0 Transmission Hold */
#define R_URXH0 		VA_UART_0_B(0x24) /* UART 0 Receive buffer */
#define R_UTXH1 		VA_UART_1_B(0x20) /* UART 1 Transmission Hold */
#define R_URXH1 		VA_UART_1_B(0x24) /* UART 1 Receive buffer */
#define R_UTXH2 		VA_UART_2_B(0x20) /* UART 2 Transmission Hold */
#define R_UTXH2 		VA_UART_2_B(0x20) /* UART 2 Transmission Hold */
#define R_URXH3 		VA_UART_3_B(0x24) /* UART 3 Receive buffer */
#define R_URXH3 		VA_UART_3_B(0x24) /* UART 3 Receive buffer */

/* timer */
#define VA_TIMER(t, offset) \
	(*(volatile unsigned *)(VA_TIMER_BASE+(0x10*t)+(offset)))

#define R_TMDMASEL		VA_TIMER(0, 0x0c) /* dma or interrupt mode selection */

#define VA_TIMER_0(offset)	VA_TIMER(0, offset)
#define VA_TIMER_1(offset)	VA_TIMER(1, offset)
#define VA_TIMER_2(offset)	VA_TIMER(2, offset)

#define R_TMCON0		VA_TIMER_0(0x00)
#define R_TMDATA0		VA_TIMER_0(0x04)
#define R_TMCNT0		VA_TIMER_0(0x08)

#define R_TMCON1		VA_TIMER_1(0x00)
#define R_TMDATA1		VA_TIMER_1(0x04)
#define R_TMCNT1		VA_TIMER_1(0x08)

#define R_TMCON2		VA_TIMER_2(0x00)
#define R_TMDATA2		VA_TIMER_2(0x04)
#define R_TMCNT2		VA_TIMER_2(0x08)

#define R_TMSTATUS		VA_TIMER_0(0x30)

/* watch dog timer */
#define VA_WDT(offset) (*(volatile unsigned *)(VA_WDT_BASE+(offset)))

/*-----------------------------------------------------*/
/* PAD Controller */
#define VA_PADCTRL_PORT(offset)  (*(volatile unsigned *)(VA_PADCTRL_BASE+(offset)))

#ifndef MHZ /* define MHZ */
#define MHZ 	1000000
#endif






#endif
