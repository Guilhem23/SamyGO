#include "sdp_i2c.h"

#if defined(SILENT_KERNEL)
#define printf 
#else
#define printf PRINTF
#endif

#define GENOA_rIIC_INTPEND(port)		 (1<<(port))
#define GENOA_rIIC_LAST_RECEIV_BIT       (1<<0)
#define GENOA_rIIC_INTERRUPT_ENABLE      (1<<6)
#define GENOA_IIC_TIMEOUT 				 (100)
#define GENOA_IIC_SERIAL_OUTPUT_ENABLE 	 (1 << 5)
#define CLEAR_PEND_INTERRUPT(port)		 (IICSTATP = IICSTATP | GENOA_rIIC_INTPEND(port))
#define GENOA_IIC_GEN_ACK(port)			 (IICCON(port) = IICCON(port) | 0x100)
#define GENOA_IIC_GEN_NACK(port)		 (IICCON(port) = IICCON(port) & ~(1<<8))
#define IIC_WRITE_FLAG					 (0)
#define IIC_READ_FLAG					 (1)



void send_start_packet(unsigned char port, unsigned char chip, unsigned char rwflag)
{
	volatile unsigned int rIICSTAT=0;

	if(rwflag == IIC_WRITE_FLAG)
			rIICSTAT = 0xD0;
	else
			rIICSTAT = 0x90;

	IICCON(port) |= (1 << 6); 	/*Enable interrupt */
	IICSTAT(port) = rIICSTAT;

	IICDS(port) = chip | (rwflag & 0x1);	

	rIICSTAT |= GENOA_IIC_SERIAL_OUTPUT_ENABLE;
	IICSTAT(port) = rIICSTAT;
	IICSTATP = IICSTATP | GENOA_rIIC_INTPEND(port);
}

void send_stop_packet(unsigned char port, unsigned char rwflag)
{
	volatile unsigned int rIICSTAT=0;

	if(rwflag == IIC_WRITE_FLAG)
			rIICSTAT = 0xD0;
	else
			rIICSTAT = 0x90;

	IICCON(port) &= ~(1 << 6); 	/*Disable interrupt */
	IICSTAT(port) = rIICSTAT;

	CLEAR_PEND_INTERRUPT(port);
}


int wait_interrupt(unsigned char port)
{
	unsigned int temp = 0;
	int k;

	for(k=0; k<GENOA_IIC_TIMEOUT; k++) {
		temp = IICSTATP;
		if( (temp & GENOA_rIIC_INTPEND(port)) == GENOA_rIIC_INTPEND(port))
		break;
	
		__udelay(10); //  App => 4.7us
	}

	if (k==GENOA_IIC_TIMEOUT){
		return -1;
	}

	return 0;
}


int check_port_ack(unsigned char port)
{
	if ((IICSTAT(port) & GENOA_rIIC_LAST_RECEIV_BIT) == GENOA_rIIC_LAST_RECEIV_BIT ){
		return -1;
	}
	return 0;
}

void i2c_init(unsigned char port, int slaveaddr,int khz)
{
	
	/*
		setting up I2C support
	*/
	unsigned int rIICCON,rPCONA;
	extern unsigned int get_clock_apb(void);
	unsigned int apb = get_clock_apb() / 1000;
	/*
		Enable ACK, IICCLK=MCLK/16, enable interrupt
		50MHz/16/(12+1) = 390625 Hz
	*/

	IICEN = 0x7f;
	IICCONE(port) = 0x401;

	if(port == 5 || port ==6)
	{
		/* Setting GPIO */
		*(volatile unsigned int *)(0x30090C50) |= (1 << (1 + (port)));
	}

	int regVal = IICCON(port);
	regVal = IICCON(port) = regVal & 0x17F;
	unsigned int prescaler = (apb >> 4) / khz;
	if (prescaler > 63) {
			prescaler = (apb >> 8) / khz;
			regVal = regVal | 0x080;
	}

	rIICCON = ((regVal & 0x3C0) | (prescaler & 0x3F));


	rIICCON= rIICCON | ( 1<<8) | ( 1 << 6);
	
	IICCON(port) = rIICCON;



}


/*
 * Read/Write interface:
 *   port:    I2C channel, range 0 to 5
 *   chip:    I2C chip address, range 0..127
 *   addr:    Memory (register) address within the chip
 *   buffer:  Where to read/write the data
 *   len:     How many bytes to read/write
 *
 *   Returns: 0 on success, not 0 on failure
 */

int i2c_read(unsigned char port, unsigned char chip, unsigned int addr, unsigned char *buffer, int len)
{

	int k, j, temp;
	volatile unsigned int rIICSTAT,rIICCON;


	IICCON(port) = (IICCON(port) & 0xFF);

	/*
		send the device offset
	*/
	send_start_packet(port,chip,IIC_WRITE_FLAG);
	if(wait_interrupt(port) == -1){
			printf("IIC : Port %d Timeout.... \n", port);
			goto error;
	}
	
	/* wait and check ACK */
	if(check_port_ack(port) == -1){
			printf("IIC : Port %d doesn't send ack.... \n", port);
			goto error;
	}
#if 1

	/* Send subaddr for two byes...*/
	IICDS(port) = (addr >> 8) & 0xff;
	CLEAR_PEND_INTERRUPT(port);
	if(wait_interrupt(port) == -1){
			printf("IIC : Port %d Timeout.... \n", port);
			goto error;
	}
	
	/* wait and check ACK */
	if(check_port_ack(port) == -1){
			printf("IIC : Port %d doesn't send ack.... \n", port);
			goto error;
	}


	IICDS(port) = addr & 0xff;
	CLEAR_PEND_INTERRUPT(port);
	if(wait_interrupt(port) == -1){
			printf("IIC : Port %d Timeout.... \n", port);
			goto error;
	}
	
	/* wait and check ACK */
	if(check_port_ack(port) == -1){
			printf("IIC : Port %d doesn't send ack.... \n", port);
			goto error;
	}

	/* sending subaddr ended... */
#endif



	/*
		now we can start with the read operation...
	*/

	send_start_packet(port, chip, IIC_READ_FLAG);
	CLEAR_PEND_INTERRUPT(port);
	if(wait_interrupt(port) == -1){
			printf("IIC : Port %d Timeout.... \n", port);
			goto error;
	}
	
	/* wait and check ACK */
	if(check_port_ack(port) == -1){
			printf("IIC : Port %d doesn't send ack.... \n", port);
			goto error;
	}


	for (j=0; j<len-1; j++) {

			GENOA_IIC_GEN_ACK(port);

			/*clear pending bit to resume */
			CLEAR_PEND_INTERRUPT(port);

			if(wait_interrupt(port) == -1){
					printf("IIC : Port %d Timeout.... \n", port);
					goto error;
			}

			/* wait and check ACK */
			if(check_port_ack(port) == -1){
					printf("IIC : Port %d doesn't send ack.... \n", port);
					goto error;
			}

			buffer[j] = IICDS(port); /*save readed data*/
			

	} /*end for(j)*/
	

	/*
		reading the last data
		unset ACK generation
	*/
	GENOA_IIC_GEN_NACK(port);
	CLEAR_PEND_INTERRUPT(port);
	
	if(wait_interrupt(port) == -1){
			printf("IIC : Port %d Timeout.... \n", port);
			goto error;
	}

	buffer[j] = IICDS(port); /*save readed data*/
	CLEAR_PEND_INTERRUPT(port);

	send_stop_packet(port,IIC_READ_FLAG);
	CLEAR_PEND_INTERRUPT(port);

	GENOA_IIC_GEN_ACK(port); /*restore ACK generation*/ 
	return 0;

error:
	/* sending stop to terminate */
	send_stop_packet(port,IIC_READ_FLAG);
	/*Clear Int Pending Bit to RESUME*/
	CLEAR_PEND_INTERRUPT(port);

	return -1;		
}

int i2c_write(unsigned char port, unsigned char chip, unsigned int addr, unsigned char *buffer, int len)
{

	unsigned int addr_size=1;

	/*
		send the device offset
	*/
	int k, j, temp;
	volatile unsigned int rIICSTAT,rIICCON;


	IICCON(port) = (IICCON(port) & 0xFF);

	/*
		send the device offset
	*/
	send_start_packet(port,chip,IIC_WRITE_FLAG);
	if(wait_interrupt(port) == -1){
			printf("IIC : Port %d:%x Timeout....1 \r\n", port, addr);
			goto error;
	}
	
	/* wait and check ACK */
	if(check_port_ack(port) == -1){
			printf("IIC : Port %d:%x doesn't send ack....1 \r\n", port,addr);
			goto error;
	}
#if 1

	/* Send subaddr for two byes...*/
	IICDS(port) = (addr >> 8) & 0xff;
	CLEAR_PEND_INTERRUPT(port);
	if(wait_interrupt(port) == -1){
			printf("IIC : Port %d:%x Timeout....2 \r\n", port, addr);
			goto error;
	}
	
	/* wait and check ACK */
	if(check_port_ack(port) == -1){
			printf("IIC : Port %d doesn't send ack....2 \r\n", port);
			goto error;
	}


	IICDS(port) = (addr) & 0xff;
	CLEAR_PEND_INTERRUPT(port);
	if(wait_interrupt(port) == -1){
			printf("IIC : Port %d:%x Timeout....3 \r\n", port, addr);
			goto error;
	}
	
	/* wait and check ACK */
	if(check_port_ack(port) == -1){
			printf("IIC : Port %d doesn't send ack....3 \r\n", port);
			goto error;
	}

	/* sending subaddr ended...*/
#endif


	/*
		now we can start with the read write operation
	*/
	for (j=0; j<len; j++) {

		GENOA_IIC_GEN_ACK(port);

		IICDS(port) = buffer[j]; /*prerare data to write*/
		CLEAR_PEND_INTERRUPT(port);

		/* wait but NOT check ACK */
		if(wait_interrupt(port) == -1){
				printf("IIC : Port %d Timeout.... \n", port);
				goto error;
		}
	if(check_port_ack(port) == -1){
			printf("IIC : Port %d doesn't send ack.... \n", port);
			goto error;
	}
	} /* end for(j) */

	/* sending stop to terminate */
	send_stop_packet(port,IIC_WRITE_FLAG);
	/*Clear Int Pending Bit to RESUME*/
	CLEAR_PEND_INTERRUPT(port);

	return 0;

error:
	/* sending stop to terminate */
	send_stop_packet(port,IIC_WRITE_FLAG);
	/*Clear Int Pending Bit to RESUME*/
	CLEAR_PEND_INTERRUPT(port);

	return -1;
}


