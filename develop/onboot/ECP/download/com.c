
/*
 * Building: cc -o com com.c
 * Usage   : ./com /dev/device [speed]
 * Example : ./com /dev/ttyS0 [115200]
 * Keys    : Ctrl-A - exit, Ctrl-X - display control lines status
 * Darcs   : darcs get http://tinyserial.sf.net/
 * Homepage: http://tinyserial.sourceforge.net
 * Version : 2009-03-05
 *
 * Ivan Tikhonov, http://www.brokestream.com, kefeer@brokestream.com
 * Patches by Jim Kou, Henry Nestler, Jon Miner, Alan Horstmann
 *
 */
/* Copyright (C) 2007 Ivan Tikhonov

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Ivan Tikhonov, kefeer@brokestream.com

*/

#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <ctype.h>


#define BLOCK_DATA 	2048

int transfer_byte(int from, int to, int is_control);
unsigned char send_download = 1;
typedef struct {char *name; int flag; } speed_spec;
                
void print_status(int fd) {
	int status;
	unsigned int arg;
	status = ioctl(fd, TIOCMGET, &arg);
	fprintf(stderr, "[STATUS]: ");
	if(arg & TIOCM_RTS) fprintf(stderr, "RTS ");
	if(arg & TIOCM_CTS) fprintf(stderr, "CTS ");
	if(arg & TIOCM_DSR) fprintf(stderr, "DSR ");
	if(arg & TIOCM_CAR) fprintf(stderr, "DCD ");
	if(arg & TIOCM_DTR) fprintf(stderr, "DTR ");
	if(arg & TIOCM_RNG) fprintf(stderr, "RI ");
	fprintf(stderr, "\r\n");
}

static	int comfd;
static 	int log_only = 1;
static	unsigned int  filesize;
static	unsigned char *file_data;
FILE *inputfp;
static char printf_data[0x1000] = {0,};
static int  datacount = 0;
#define	RETRY_COUNT			4
typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned int	uint;


static int	start_block, sent, data_sz;
static int	*status;
static char	blk_buf[2048];
static unsigned char debug_data[1048];
static unsigned int load_addr= 0x40008000;
unsigned char * xbuf;

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (*cp == '0') {
		cp++;
		if ((*cp == 'x') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

long simple_strtol(const char *cp,char **endp,unsigned int base)
{
	if(*cp=='-')
		return -simple_strtoul(cp+1,endp,base);
	return simple_strtoul(cp,endp,base);
}
int main(int argc, char *argv[])
{
	
	struct termios oldtio, newtio;       //place for old and new port settings for serial port
	struct termios oldkey, newkey;       //place tor old and new port settings for keyboard teletype
	char *devicename = argv[1];
	int need_exit = 0;
	speed_spec speeds[] =
	{
		{"1200", B1200},
		{"2400", B2400},
		{"4800", B4800},
		{"9600", B9600},
		{"19200", B19200},
		{"38400", B38400},
		{"57600", B57600},
		{"115200", B115200},
		{NULL, 0}
	};
	int speed = B115200;

	if(argc < 2) {
		fprintf(stderr, "Use    : %s comport file load_adress\n", argv[0]);
		fprintf(stderr, "example: %s /dev/ttyS0 image.img 0x40008000\n", argv[0]);
		exit(1);
	}

	comfd = open(devicename, O_RDWR  | O_NONBLOCK);
	if (comfd < 0)
	{
		perror(devicename);
		exit(-1);
	}
	if(argc > 2) {	
		inputfp  = fopen( argv[2] , "rb");

		if (inputfp) {
			fseek(inputfp,0,2); 
			filesize=ftell(inputfp); 
			rewind(inputfp);
			file_data = malloc( filesize + 100);
			fread(file_data, sizeof(unsigned char), filesize, inputfp);
			fclose(inputfp);
			fprintf(stderr, "File %s loaded, size %x bytes\n",argv[2], filesize);
			
		} else {
			return 0;	
		}
		
	}
	load_addr = (int)simple_strtol(argv[3], NULL, 10);
	fprintf(stderr, "C-a exit, C-x modem lines status\n");

            
	tcgetattr(STDIN_FILENO,&oldkey);
	newkey.c_cflag = B9600 | CRTSCTS | CS8 | CLOCAL | CREAD;
	newkey.c_iflag = IGNPAR;
	newkey.c_oflag = 0;
	newkey.c_lflag = 0;
	newkey.c_cc[VMIN]=1;
	newkey.c_cc[VTIME]=0;
	tcflush(STDIN_FILENO, TCIFLUSH);
	tcsetattr(STDIN_FILENO,TCSANOW,&newkey);


	tcgetattr(comfd,&oldtio); // save current port settings 
	newtio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;
	newtio.c_lflag = 0;
	newtio.c_cc[VMIN]=1;
	newtio.c_cc[VTIME]=0;
	tcflush(comfd, TCIFLUSH);
	tcsetattr(comfd,TCSANOW,&newtio);

	print_status(comfd);

	while(!need_exit) {
		fd_set fds;
		int ret;
		
		FD_ZERO(&fds);
		FD_SET(STDIN_FILENO, &fds);
		FD_SET(comfd, &fds);


		ret = select(comfd+1, &fds, NULL, NULL, NULL);
		if(ret == -1) {
			perror("select");
		} else if (ret > 0) {
			if(FD_ISSET(STDIN_FILENO, &fds)) {
				need_exit = transfer_byte(STDIN_FILENO, comfd, 1);
			}
			if(FD_ISSET(comfd, &fds)) {
				need_exit = transfer_byte(comfd, STDIN_FILENO, 0);
			}
		}
	}

	tcsetattr(comfd,TCSANOW,&oldtio);
	tcsetattr(STDIN_FILENO,TCSANOW,&oldkey);
	close(comfd);
	if (file_data) 
		free(file_data);
	return 0;
}

static void _dump(char *p, int cnt)
{
#if	DBG_SIO
	static char bf[128];
	int i;

	for(i = 0; i < cnt; i++){
		sprintf(bf, "%02x ", *p++ & 0xff);
		printf(bf);
	}
#endif
}

int rcv_1b(int h, unsigned char *c)
{
	while(1){
		if(read(h, c, 1) > 0)
			return 1;
	}
}

int rcv_1line(int h, unsigned char *p, int max)
{
	unsigned char c;
	int cnt = 0;

	while(1){
		if(rcv_1b(h, &c)){
			if(c == '\r')
				return cnt;
			if(cnt < max){
				*p++ = c;
				cnt++;
			}
		}
	}
}

unsigned int atox(unsigned char *p)
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

static unsigned char out_data(int h, unsigned char  *p, int len)
{

	write(h, p, len);

	return 1;
}

unsigned char sio_trigger(int  h, int start_blk)
{
	unsigned char c;
	int step;
	unsigned char got;

	start_block = start_blk;

	step = 0;
	got = 0;
	while(got == 0){
		if (read(h, &c, 1) ) {
			switch(c){
			case 'e':
				step = 1;
				break;
			case 'c':
				if(step == 1)
					step++;
				else
					step = 0;
				break;
			case 'h':
				if(step == 2)
					step++;
				else
					step = 0;
				break;
			case 'o':
				if(step == 3)
					step++;
				else
					step = 0;
				break;
			case '_':
				if(step == 4){
					step++;
				}
				else
					step = 0;
				break;
			case 'p':
				if(step == 5){
					step++;
				}
				else
					step = 0;
				break;

			case '\r':
				if(step == 6){
					got = 1;
					printf("\nRcvd Ack\n");
					out_data(h, ">echo_p", 8);
					rcv_1line(h, debug_data, 20 );
					printf ("%s\n",debug_data );
					return 1;
				} else{
					step = 0;
				}
				break;
			}
		}
	}
	return 0;
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

static void int_tobyte(unsigned char *data, unsigned long value)
{
	
	data[0] = (unsigned char)((value >> 8) & 0xFF);
	data[1] = (unsigned char)((value) & 0xFF);
}

static void INT32(unsigned char *data, unsigned long value)
{
	data[0] = (value & 0xFF000000) >> 24;
	data[1] = (value & 0x00FF0000) >> 16;
	data[2] = (value & 0x0000FF00) >> 8;
	data[3] = (value & 0x000000FF);
}

void BIN2STR(unsigned char *s, char *d, int l, int f)
{
  int   i;
  char  bb[5];

  d[0] = '\0';

  for (i = 0; i < l; i++) {
    if (f & 1) {
      sprintf(bb, "%02X", s[i]);
    } else {
      sprintf(bb, "%02x", s[i]);
    }
    strcat(d, bb);
    if (f & 2) {
      strcat(d, " ");
    }
  }

  return;
}

int transfer_byte(int from, int to, int is_control) {
	char c;

	int ret;
	do {
		ret = read(from, &c, 1);
	} while (ret < 0 && errno == EINTR);
	if(ret == 1) {
		if(is_control) {
			if(c == '\x01') { // C-a
					return -1;
			} else if(c == '\x18') { // C-x
					print_status(to);
					return 0;
			}
		}	
		while(write(to, &c, 1) == -1) {
			if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
		}
		#if 1
		if(is_control) {

		} else {
			printf_data[datacount++] = c;
			if(c == '\n') {
				//fprintf(stderr, "--> %s\n", data);
				if (strstr(printf_data,"SamyGO 0055")) {
					send_download = 1;	
				}
				if (strstr(printf_data,"SamyGO 3rd")) {
					send_download = 0;	
				}
				if (send_download) {
					if (strstr(printf_data, "Hit Enter")) {
						
							while(write(comfd, "\r", 1) == -1) {
								if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
							}		
							while(write(comfd, "\n", 1) == -1) {
								if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
							}		
							if (send_download) {
								while(write(comfd, "serial\n", 7) == -1) {
									if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
								}
							}				
					}
				} 
			}
			if (c == '\r') {
				if (strstr(printf_data,"echo_p")) {
					while(write(comfd,">echo_p", 7) == -1) {
						if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
					}		
				}
				if (strstr(printf_data,"ECRC")) {
					unsigned short end_crc = ccitt_crc(file_data, filesize);
					printf("\n\n" );
					printf("%s\n\n",printf_data );
					printf("%x\n\n",end_crc );
					while(write(comfd, "FCR\n", 4) == -1) {
						if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
					}
					send_download = 0;
				}
				if (strstr(printf_data,"ECHO_DOWN_ST")) {
					char out_data[255];
					
					while(write(comfd,"SB0000\n", 7) == -1) {
						if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
					}		
					sprintf (out_data, "LA%08x\n", load_addr);
					while(write(comfd,out_data,strlen(out_data) ) == -1) {
						if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
					}
					sprintf (out_data, "FS%08x\n", filesize);
					while(write(comfd,out_data , strlen(out_data) ) == -1) {
						if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
					}	
				}
				if (strstr(printf_data,"RBLK")) { // requestded block // RBLK00000000 04
				
					unsigned char send_data[2048];
					unsigned int block, i, check;
					unsigned short cksm = 0;
					unsigned char pp[1024];
					unsigned char cmd[4096] = {0,};
						
					printf_data[12] = 0;
					block = atox(printf_data + 4);
					if (block  < filesize ) {
						
						cksm = ccitt_crc(file_data + block, BLOCK_DATA);
						#if 0
						BIN2STR(file_data + block, pp, 128, 0);
			 			
			 			sprintf(cmd, "BL%08x%s%04x\n", i, pp, cksm); 
						while(write(comfd,cmd, strlen(cmd) ) == -1) {
							if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
						}
						#else 
						INT32(cmd , block );
						memcpy(cmd+4, file_data + block , BLOCK_DATA);
						int_tobyte(cmd + 4 + BLOCK_DATA , cksm);
						
						while(write(comfd,"BL\n", 3 ) == -1) {
							if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
						}
						while(write(comfd,cmd, 6 + BLOCK_DATA ) == -1) {
							if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
						}	

						#endif
						
					}				
					if (block  >= filesize ) {
						while(write(comfd, "END\n", 4) == -1) {
							if(errno!=EAGAIN && errno!=EINTR) { perror("write failed"); break; }
						}
					}
				}
				memset(printf_data, 0, sizeof(printf_data));	
				datacount = 0;
			}
		}
		#endif
	} else {
		fprintf(stderr, "\nnothing to read. probably port disconnected.\n");
		return -2;
	}
	return 0;
}
