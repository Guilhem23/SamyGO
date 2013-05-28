/*
 * (C) Copyright 2002
 * Wolfgang Denk, DENX Software Engineering, <wd@denx.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * Copyright (C) 1999 2000 2001 Erik Mouw (J.A.K.Mouw@its.tudelft.nl)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
#include <echo-reg.h>
#include <config.h>
#include <types.h>

/* registers */
#if defined(CONFIG_SERIAL0)
#define UART_PORT	0
#elif defined(CONFIG_SERIAL1)
#define UART_PORT	1
#elif defined(CONFIG_SERIAL2)
#define UART_PORT	2
#elif defined(CONFIG_SERIAL3)
#define UART_PORT	3
#else
//#error "Set Uart Port\n"
#endif 

#ifdef CONFIG_MICOM_UART
/* ms.park -start */
#define UART_PORT_MICOM		    0
#define	R_ULCON_MICOM		    VA_UART(unsigned, UART_PORT_MICOM, 0x00)
#define	R_UCON_MICOM		    VA_UART(unsigned, UART_PORT_MICOM, 0x04)
#define	R_UFCON_MICOM		    VA_UART(unsigned, UART_PORT_MICOM, 0x08)
#define R_UMCON_MICOM		    VA_UART(unsigned, UART_PORT_MICOM, 0x0C)
#define	R_UTRSTAT_MICOM		    VA_UART(unsigned, UART_PORT_MICOM, 0x10)
#define	R_UERSTAT_MICOM		    VA_UART(unsigned, UART_PORT_MICOM, 0x14)
#define	R_UFSTAT_MICOM		    VA_UART(unsigned, UART_PORT_MICOM, 0x18)
#define R_UMSTAT_MICOM		    VA_UART(unsigned, UART_PORT_MICOM, 0x1C)
#define	R_UBRDIV_MICOM		    VA_UART(unsigned, UART_PORT_MICOM, 0x28)
#define	R_UTXH_MICOM		    VA_UART(char, UART_PORT_MICOM, 0x20)
#define	R_URXH_MICOM		    VA_UART(char, UART_PORT_MICOM, 0x24)
/* ms.park -end */
#define CONFIG_BAUDRATE_MICOM   9600
#endif

#define R_ULCON 		        VA_UART(unsigned, UART_PORT, 0x00)
#define R_UCON				    VA_UART(unsigned, UART_PORT, 0x04)
#define R_UFCON 		        VA_UART(unsigned, UART_PORT, 0x08)
#define R_UMCON 			    VA_UART(unsigned, UART_PORT, 0x0C)
#define R_UTRSTAT			    VA_UART(unsigned, UART_PORT, 0x10)
#define R_UERSTAT			    VA_UART(unsigned, UART_PORT, 0x14)
#define R_UFSTAT			    VA_UART(unsigned, UART_PORT, 0x18)
#define R_UMSTAT			    VA_UART(unsigned, UART_PORT, 0x1C)
#define R_UBRDIV			    VA_UART(unsigned, UART_PORT, 0x28)

#define R_UTXH				    VA_UART(char, UART_PORT, 0x20)
#define R_URXH				    VA_UART(char, UART_PORT, 0x24)


/* UART register initial values */
#define ULCON_INIT_VAL			(0x03)
#define UMCON_INIT_VAL			(0)
#define UCON_INIT_VAL			(0x3085)
#define R_UBRDIV_INIT_VAL		(0x51)
#define R_UBRDIV_INIT_VAL_MICOM (0x3d7)

#ifdef CONFIG_USE_UART_FIFO
#define UFCON_INIT_VAL			(0x01)
#define POLLING_RX()			((REG_GET(R_UFSTAT) & 0x10F) > 0)
#define POLLING_TX()			((REG_GET(R_UFSTAT) & (1<<9)) == 0 )
#ifdef CONFIG_MICOM_UART
#define POLLING_RX_MICOM()		((REG_GET(R_UFSTAT_MICOM) & 0x10F) > 0)
#define POLLING_TX_MICOM()		((REG_GET(R_UFSTAT_MICOM) & (1<<9)) == 0 )
#endif
#else
#define UFCON_INIT_VAL			(0x0)
#define POLLING_RX()			((REG_GET(R_UTRSTAT) & 0x1))
#define POLLING_TX()			((REG_GET(R_UTRSTAT) & 0x4))
#ifdef CONFIG_MICOM_UART
#define POLLING_RX_MICOM()		((REG_GET(R_UTRSTAT_MICOM) & 0x1))
#define POLLING_TX_MICOM()		((REG_GET(R_UTRSTAT_MICOM) & 0x4))
#endif
#endif


//DECLARE_GLOBAL_DATA_PTR;

/* see speed.c */
extern unsigned long get_clock_apb(void);
extern void memset(void *pDst, UINT8 nV, UINT32 nLen);
void serial_setbrg_micom(void);

void serial_setbrg (void)
{
	volatile int i;
	unsigned int reg = 0;
	
	reg = (unsigned int)( (get_clock_apb() / (CONFIG_BAUDRATE / 10 * 16) -5) / 10);
	
	REG_PUT(R_ULCON, ULCON_INIT_VAL);
	REG_PUT(R_UCON, UCON_INIT_VAL);
	REG_PUT(R_UFCON, UFCON_INIT_VAL);
	REG_PUT(R_UMCON, UMCON_INIT_VAL);
	REG_PUT(R_UBRDIV, reg);

	/* delay */
	i = 100;
	while (i--);
	
}

/*
 * Initialise the serial port with the given baudrate. The settings
 * are always 8 data bits, no parity, 1 stop bit, no start bits.
 *
 */
int serial_init (void)
{
	serial_setbrg ();
#ifdef CONFIG_MICOM_UART
    serial_setbrg_micom();
#endif

	return (0);
}


/*
 * Output a single byte to the serial port.
 */
void serial_putc (const char c)
{
	while( POLLING_TX() == 0 )
	{
	}
	REG_PUT(R_UTXH, c);

	if( c == '\n' )
	{
		serial_putc('\r');
	}
}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise. When the function is succesfull, the character read is
 * written into its argument c.
 */
int serial_tstc (void)
{
	return POLLING_RX();
}

/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise. When the function is succesfull, the character read is
 * written into its argument c.
 */
int serial_getc (void)
{
	for (;;) {
		if ( POLLING_RX() )
			break;
	}
	
	return (REG_GET(R_URXH) & 0xff);
}
int serial_getc_check (int waitTime)
{
	int count = 0;	
	while(1)
	{
		if ( POLLING_RX() )
			break;
        else
        {
			count++;
			if(count == waitTime)
				break;
        }
	}
	return (REG_GET(R_URXH) & 0xff);
}

void serial_puts (const char *s)
{
	while (*s)
	{
		serial_putc(*s);
		s++;
	}

}
#ifdef CONFIG_MICOM_UART
/*********************************************************/
/************************Micom Uart Code******************/
/*********************************************************/

/**
 * serial_setbrg_MICOM()
 * 		: Initialize the UART controller
 * @author		ij.jang
 * @version		$Revison$
 */
void serial_setbrg_micom(void)
{
	volatile int i;
	unsigned int reg = 0;

	reg = ((unsigned int)(get_clock_apb() / (CONFIG_BAUDRATE_MICOM * 16))) -1; 

	REG_PUT(R_ULCON_MICOM, ULCON_INIT_VAL);
	REG_PUT(R_UCON_MICOM, UCON_INIT_VAL);
	REG_PUT(R_UFCON_MICOM, UFCON_INIT_VAL);
	REG_PUT(R_UMCON_MICOM, UMCON_INIT_VAL);

	REG_PUT(R_UBRDIV_MICOM, reg);

	/* delay */
	i = 100;
	while (i--);
}

/**
 * micom_getc()
 * 		: Read a single byte from the serial port.
 * @author		
 * @return		Returns 1 on success, 0 otherwise.
 * @version		$Revison$
 */
int micom_getc (void)
{
	for (;;) {
		if ( POLLING_RX_MICOM() )
			break;
	}

	return (REG_GET(R_URXH_MICOM) & 0xff);
}

/**
 * micom_putc()
 * 		: Output a single byte to the serial port.
 * @version    $Revison$
 */
void micom_putc (const char c)
{
	while( POLLING_TX_MICOM() == 0 )
	{
	}
	REG_PUT(R_UTXH_MICOM, c);
}

void micom_get(unsigned char *buf, int len) {
	
	unsigned int i;
	
	for (i = 0; i < len; i++)
		buf[i] = micom_getc();
		
		
}

int micom_cmd(int command, int option1, int option2)
{
    unsigned char w_databuff[9];
    unsigned char r_databuff[255];
	int retry = 2;
	int i = 0;

	while(retry-- > 0)
	{
		memset(w_databuff, 0, 9);
		memset(r_databuff, 0, 255);

		w_databuff[0] = 0xff;
		w_databuff[1] = 0xff;
		w_databuff[2] = command;
		w_databuff[3] = option1;
		w_databuff[4] = option2;
		
		w_databuff[8] = w_databuff[2];
		w_databuff[8] += w_databuff[3];
		w_databuff[8] += w_databuff[4];

		//Send command to MICOM
		for(i=0; i<9; i++)
		{
			micom_putc(w_databuff[i]);
		}

	}

	return 0;
}
/* ms.park -end */
#endif

void PRINTF(const char  *pFmt, unsigned int wVal)
{
    unsigned char ucBff, ucDisp;
    unsigned int bNotZero=FALSE, bHex=FALSE;
    unsigned int wDivider=1000000000;
    
    while(ucBff = *(pFmt++))
    {
        if(ucBff == '%') // check special case
        {
            switch( *(pFmt++)) // check next character
            {
                case 'x': // hexadecimal number
                    /* FALL THROUGH */                                                                                          
                case 'X':
                    wDivider = 0x1000;
                    bHex = TRUE;
                    /* FALL THROUGH */                                                                                                              
                case 'd': // decimal number
                    /* FALL THROUGH */                                                                                          
                case 'i':
                    if(wVal)
                    {
                        while(wDivider)
                        {
                            ucDisp = wVal/wDivider;
                            wVal = wVal%wDivider;

                            if(ucDisp)
                            {
                                bNotZero=TRUE;
                            }

                            if(bNotZero)
                            {
                                if(ucDisp>9)
                                {
                                    serial_putc(ucDisp-0x0a+'A');
                                }
                                else
                                {
                                    serial_putc(ucDisp+'0');
                                }
                            }

                            if(bHex)
                            {
                                wDivider /= 0x10;
                            }
                            else
                            {
                                wDivider /= 10;
                            }
                        }
                    }
                    else
                    {
                        serial_putc('0');
                    }
                    break;
            } // switch
        }
        else // general
        {
            serial_putc(ucBff); // put a character
        }
    } // while
} 


void putc (const char c)
{
	serial_putc (c);
}
void puts (const char *s)
{
	serial_puts (s);
}
int tstc (void)
{
	return serial_tstc ();
}

inline void mdm_readline(char *buf, int bufsiz)
{
	char c;
	char *p;
	int n;

	n = 0;
	p = buf;
	for(;;) {
		c = serial_getc();

		/*		dbg("(%c)", c); */

		switch(c) {
		case '\r':
			break;
		case '\n':
			*p = '\0';
			return;

		default:
			if(n++ > bufsiz) {
				*p = '\0';
				return; /* sanity check */
			}
			*p = c;
			p++;
			break;
		}
	}
}

inline void mdm_readarray(char *buf, int bufsiz)
{
	char c;
	char *p;
	int n;

	n = 0;
	p = buf;
	for(n = 0;n < bufsiz;n++) {
		c = serial_getc();
		*p = c;
		p++;
		
	}
}