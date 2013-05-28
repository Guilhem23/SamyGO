/*
	send image download sequence

	Host					Module
	ready					ready

	send start block number 'xxxx\r'
	send filesize           'FSxxxxxxxx\r'
	send load_adress			'LAxxxxxxxx\r'
	send flash_command		'FLxxxxxxxx\r'
	send offset xxxxxxxx\r + 2kb + cksm(xxxx)
							send '00000000\r' if ok
							 'ffffffff\r' if error

	...........

	send 'ffffffff\r' to terminateEND
	
							send '55aa\r'

*/

#include <linux/ctype.h>
#include <config.h>
#define	RETRY_COUNT			2
#define BLOCK_DATA 	2048
unsigned int start_block;


void Delay(int a) {
	int i;
	for (i = 0; i < a; i++) {
	
	}	
}

unsigned atox(unsigned char *p)
{
	unsigned val;

	val = 0;

	while(*p){
		val <<= 4;
		if(*p >= '0' && *p <= '9')
			val |= *p - '0';
		else if(*p >= 'A' && *p <= 'F')
			val |= *p - 'A' + 10;
		else if(*p >= 'a' && *p <= 'f')
			val |= *p - 'a' + 10;
		p++;
	}
	return val;
}

int check_host( void )
{
	unsigned char c;
	int i, j, step;

	serial_putc(' ');
	serial_putc(' ');

	for(i = 0; i < RETRY_COUNT; i++){
		serial_puts("echo_p\r");
		step = 0;

		for(j = 0; j < 100; j++){
			c = serial_getc();
			{ 
				switch(c){
				case '>':
					step = 1;
					break;

				case 'e':
					if(step == 1)
						step++;
					else
						step = 0;
					break;

				case 'c':
					if(step == 2)
						step++;
					else
						step = 0;
					break;

				case 'h':
					if(step == 3)

						step++;
					else
						step = 0;
					break;

				case 'o':
					if(step == 4)
						step++;
					else
						step = 0;
					break;

				case '_':
					if(step == 5)
						step++;
					else
						step = 0;
					break;

				case 'p':
					if(step == 6){
						serial_puts("[ECHO_P] DOWN_START\n");
						
						return 1;
					}
					else
						step = 0;
					break;

				default:
					step = 0;
					break;
				}
			/* reset counter because of the slow sio performance */
				if(step)
					j = 1;
			}
		}
		Delay(200);	
	}

	return 0;
}

void dump_hex(unsigned char *d, int l) 
{
  int i;

	serial_puts("\n");
	for (i = 0; i < l; i++)
		printf("%02X", d[i]);
 	serial_puts("\n");
}

void req_block(unsigned offset)
{
	unsigned c;
	int i;

	serial_putc( 'R');
	serial_putc( 'B');
	serial_putc( 'L');
	serial_putc( 'K');

	for(i = 0; i < 8; i++){
		c = (offset >> 28) & 0x0f;
		if(c < 10)
			c += '0';
		else
			c = c - 10 + 'A';
		serial_putc( c);
		offset <<= 4;
	}
	serial_putc( '\r');
}

unsigned short ccitt_crc(const unsigned char *data, int len)
{
	unsigned short crc=0xFFFF;
	int i;
	
	for( i=0;i<len;i++)
	{
		crc = (unsigned char) (crc >> 8) | (crc << 8);
		crc ^= data[i];
		crc ^= (unsigned char) ((crc & 0xff) >> 4);
		crc ^= (crc << 8) << 4;
		crc ^= ((crc & 0xff) << 4) << 1;
	}
	return crc;
}

unsigned int byte_to_short(unsigned char *d) {

	return (d[0] << 8) | d[1];   // << 16) | (d[2] << 8) | d[3];
}

unsigned int byte_to_int(unsigned char *d) {

	return (d[0] << 24) | (d[1] << 16) | (d[2] << 8) | d[3];
}

int receive_data(void)
{
	static unsigned char tbf[1024];
	
	unsigned char crcb[4];
	unsigned char c, *p;
	int i, cnt, a , b;
	unsigned int offset , file_size;
	unsigned int l;
	unsigned char rb[2048];
	unsigned int next_offset = 0;
	unsigned int os_kernel_addr = 0;
	volatile unsigned char *pSrc;
	/*
		receive section
		01 - 10 mmc area 
	*/
	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");
	os_kernel_addr = get_load_adress();	/* temporary ddr ram area of kernel !! */
	pSrc = os_kernel_addr;
	serial_puts("ECHO_DOWN_ST\r");
	while(1) {
		if((cnt = mdm_readline(tbf, 1024)) > 0){
			if(tbf[0] == 'F' && tbf[1] == 'S') { // total file size
				tbf[10] = 0;
				file_size = atox(tbf + 2);
				req_block( next_offset );
				continue;
			} else if (tbf[0] == 'L' && tbf[1] == 'A') {
				tbf[10] = 0;
				set_load_adress( atox(tbf + 2) ) ;
				__asm__ __volatile__("": : :"memory");
				os_kernel_addr = get_load_adress();	/* temporary ddr ram area of kernel !! */
				pSrc = os_kernel_addr;
				continue;
			}else if (tbf[0] == 'S' && tbf[1] == 'B') {
				tbf[6] = 0;
				start_block = atox(tbf + 2);
				continue;
			} else if (tbf[0] == 'E' && tbf[1] == 'N' && tbf[2] == 'D') {
				unsigned short crc = ccitt_crc((void*)pSrc, file_size);
				sprintf(tbf, "ECRC%04x\r", crc);

				printf("ECRC %04x - %08x @ %08x\n", crc , file_size , get_load_adress() );
				continue;
			} else if (tbf[0] == 'F' && tbf[1] == 'C' && tbf[2] == 'R') {
				return file_size;
			} else if (tbf[0] == 'B' && tbf[1] == 'L' ) {
				unsigned short val1, val2;
				unsigned char *d;
				unsigned int *e;
				#if 0
				//STR2BIN (tbf+2 ,rb ,&l);
				#else
				mdm_readarray( rb,BLOCK_DATA + 2 + 4 );
				#endif
				val1	 = ccitt_crc(rb+4, BLOCK_DATA);
				val2	 = byte_to_short(rb+4+BLOCK_DATA);
				offset = byte_to_int(rb);
				e = pSrc;
				if( val1 == val2) {
					memcpy( (unsigned char *) pSrc + next_offset , rb+4, BLOCK_DATA);
					next_offset += BLOCK_DATA;
				}  else {
					printf("[ECHO_P] crc fail : %x %x %x\n",val1, val2, next_offset ); 
				}
			} else {
			}
		}

		if(offset == -1){
			break;
		}
		req_block( next_offset );
	}

	return next_offset;
}



