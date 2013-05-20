/*
 * linux/include/asm-arm/arch-s5h2150/uart.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: ij.jang@samsung.com
 *
 */

#ifndef _S5H2150_UART_H_
#define _S5H2150_UART_H_

/* UART */
#define V_UART(dt,  n, offset)	\
    (*(volatile dt *)(V_UART_BASE+(0x40*n)+(offset)))

#define V_UART_0(offset)	V_UART(unsigned, 0, offset)
#define V_UART_1(offset)	V_UART(unsigned, 1, offset)
#define V_UART_2(offset)	V_UART(unsigned, 2, offset)

#define V_UART_0_B(offset)	V_UART(char, 0, offset)
#define V_UART_1_B(offset)	V_UART(char, 1, offset)
#define V_UART_2_B(offset)	V_UART(char, 2, offset)

#define O_ULCON  		(0x00)		/* UART Line control */      
#define O_UCON                  (0x04)          /* UART Control */
#define O_UFCON                 (0x08)          /* UART FIFO control */
#define O_UMCON                 (0x0c)          /* UART Modem control */
#define O_UTRSTAT               (0x10)          /* UART Tx/Rx status */
#define O_UERSTAT               (0x14)          /* UART Rx error status */
#define O_UFSTAT                (0x18)          /* UART FIFO status */
#define O_UMSTAT                (0x1c)          /* UART Modem status */
#define O_UBRDIV                (0x28)          /* UART Baud rate divisor */
#ifdef __BIG_ENDIAN
#define O_UTXH  		(0x23)		/* UART Transmission Hold */
#define O_URXH  		(0x27) 		/* UART Receive buffer */
#else /* Little Endian */
#define O_UTXH			(0x20) 		/* UART Transmission Hold */
#define O_URXH			(0x24) 		/* UART Receive buffer */
#endif

#define R_ULCON0    		V_UART_0(O_ULCON  ) /* UART 0 Line control */
#define R_UCON0     		V_UART_0(O_UCON   ) /* UART 0 Control */
#define R_UFCON0    		V_UART_0(O_UFCON  ) /* UART 0 FIFO control */
#define R_UMCON0    		V_UART_0(O_UMCON  ) /* UART 0 Modem control */
#define R_UTRSTAT0  		V_UART_0(O_UTRSTAT) /* UART 0 Tx/Rx status */
#define R_UERSTAT0  		V_UART_0(O_UERSTAT) /* UART 0 Rx error status */
#define R_UFSTAT0   		V_UART_0(O_UFSTAT ) /* UART 0 FIFO status */
#define R_UMSTAT0   		V_UART_0(O_UMSTAT ) /* UART 0 Modem status */
#define R_UBRDIV0   		V_UART_0(O_UBRDIV ) /* UART 0 Baud rate divisor */
#define R_UTXH0  		V_UART_0_B(O_UTXH) /* UART 0 Transmission Hold */
#define R_URXH0  		V_UART_0_B(O_URXH) /* UART 0 Receive buffer */

#define R_ULCON1     		V_UART_1(O_ULCON  ) /* UART 1 Line control */
#define R_UCON1      		V_UART_1(O_UCON   ) /* UART 1 Control */
#define R_UFCON1     		V_UART_1(O_UFCON  ) /* UART 1 FIFO control */
#define R_UMCON1     		V_UART_1(O_UMCON  ) /* UART 1 Modem control */
#define R_UTRSTAT1   		V_UART_1(O_UTRSTAT) /* UART 1 Tx/Rx status */
#define R_UERSTAT1   		V_UART_1(O_UERSTAT) /* UART 1 Rx error status */
#define R_UFSTAT1    		V_UART_1(O_UFSTAT ) /* UART 1 FIFO status */
#define R_UMSTAT1    		V_UART_1(O_UMSTAT ) /* UART 1 Modem status */
#define R_UBRDIV1    		V_UART_1(O_UBRDIV ) /* UART 1 Baud rate divisor */
#define R_UTXH0  		V_UART_0_B(O_UTXH) /* UART 0 Transmission Hold */
#define R_URXH0  		V_UART_0_B(O_URXH) /* UART 0 Receive buffer */

#define R_ULCON2     		V_UART_2(O_ULCON  ) /* UART 2 Line control */
#define R_UCON2      		V_UART_2(O_UCON   ) /* UART 2 Control */
#define R_UFCON2     		V_UART_2(O_UFCON  ) /* UART 2 FIFO control */
#define R_UMCON2     		V_UART_2(O_UMCON  ) /* UART 2 Modem control */
#define R_UTRSTAT2   		V_UART_2(O_UTRSTAT) /* UART 2 Tx/Rx status */
#define R_UERSTAT2   		V_UART_2(O_UERSTAT) /* UART 2 Rx error status */
#define R_UFSTAT2    		V_UART_2(O_UFSTAT ) /* UART 2 FIFO status */
#define R_UMSTAT2    		V_UART_2(O_UMSTAT ) /* UART 2 Modem status */
#define R_UBRDIV2    		V_UART_2(O_UBRDIV ) /* UART 2 Baud rate divisor */
#define R_UTXH0  		V_UART_0_B(O_UTXH) /* UART 0 Transmission Hold */
#define R_URXH0  		V_UART_0_B(O_URXH) /* UART 0 Receive buffer */

#endif
