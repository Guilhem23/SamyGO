/* # stty -F /dev/ttyS0
 * speed 9600 baud;
 * intr = <undef>; quit = <undef>; erase = <undef>; kill = <undef>; eof = <undef>; start = <undef>; stop = <undef>; susp = <undef>;
 * rprnt = <undef>; werase = <undef>; lnext = <undef>; flush = <undef>; min = 1; time = 0;
 * -brkint -icrnl -imaxbel
 *  -onlcr
 *  -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke
 */

#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <asm/types.h>

#define BAUDRATE B9600
#define MODEMDEVICE "/dev/ttyS0"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

/* http://sourceforge.net/apps/phpbb/samygo/viewtopic.php?f=6&t=1506&p=13535&hilit=micom+ttys0#p13574 */
struct micom_event{
	__u8 header;    /* 0xff */
	__u8 type;      /* 0xff: "normal" packet, 0xfe: "hdmi cec" packet, 0xfd: "clone buffer" packet */
	__u8 event;     /* MICO_KEY_EVENT */
	__u8 code;      /* keycode or 0x42 for KEY_TYPE_RELEASE event */
	__u8 flag;      /* key repetition flag (0|1|2) */
	__u8 unk1;     /* not sure what it is (0xE2-0x0C,0x00; time elapsed from previous keypress ?) */
	__u8 unk2;    /* no idea (0x00 0x00 for key type events ?) */
	__u8 unk3;    /* no idea (0x00 0x00 for key type events ?) */
	__u8 crc;       /* checksum sans header: (b2+b3+b4+b5+b6+b7) && 0xFF */
}__attribute__((packed)); 

volatile int STOP=FALSE; 
int fd = -1;
int in_work = FALSE;

void signal_handler_IO (int status);   /* definition of signal handler */
int wait_flag=TRUE;                    /* TRUE while no signal received */
struct micom_event ev_ir[64];

int main()
{
	int c, res, i;
	int ser_status;
	struct termios oldtio,newtio;
	struct sigaction saio;           /* definition of signal action */
	char buf[255];

	/* open the device to be non-blocking (read will return immediatly) */
	// fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK | O_ASYNC);
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NONBLOCK );
	if (fd <0) {perror(MODEMDEVICE); exit(-1); }


	/* install the signal handler before making the device asynchronous */
	sigemptyset(&saio.sa_mask);
	sigaddset(&saio.sa_mask, SIGIO);
	saio.sa_handler = signal_handler_IO;
	saio.sa_flags = 0;
	saio.sa_restorer = NULL;
	sigaction(SIGIO,&saio,NULL); /* add read and decode procedure?? */

	tcgetattr(fd,&oldtio); /* save current port settings */
	memcpy(&newtio,&oldtio,sizeof(newtio));

	/* allow the process to receive SIGIO */
	fcntl(fd, F_SETOWN, getpid());
	/* Make the file descriptor asynchronous (the manual page says only 
	 *            O_APPEND and O_NONBLOCK, will work with F_SETFL...) */
	fcntl(fd, F_SETFL, FASYNC);

	/* set new port settings for canonical input processing */
	//	newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
	//	newtio.c_iflag = IGNPAR | ICRNL;
	//	newtio.c_oflag = 0;
	//	newtio.c_lflag = ICANON;
	newtio.c_cc[VMIN]=sizeof(struct micom_event);
	newtio.c_cc[VTIME] = 0;
	// newtio.c_cc[VTIME] = 10;
	//	tcflush(fd, TCIFLUSH);
	tcsetattr(fd,TCSANOW,&newtio);

	/* loop while waiting for input. normally we would do something
	 *            useful here */ 
	while (STOP==FALSE) {
		/* after receiving SIGIO, wait_flag = FALSE, input is available
		 *              and can be read */
		ioctl(fd, FIONREAD, &ser_status);
		printf(",", ser_status);
		if (wait_flag==FALSE) { 
			ioctl(fd, FIONREAD, &ser_status);
			printf("-", ser_status);

			//			usleep(1000);
			// if (c > 3) STOP=TRUE; /*  */
			wait_flag = TRUE;      /* wait for new input */

		}
		ioctl(fd, FIONREAD, &ser_status);
		printf(".", ser_status);
		//		usleep(1000);
	}
	/* restore old port settings */
	tcsetattr(fd,TCSANOW,&oldtio);
	close(fd);
	return 1;
}

/***************************************************************************
 * signal handler. sets wait_flag to FALSE, to indicate above loop that     *
 * characters have been received.                                           *
 ***************************************************************************/

int Pack_len = 1;

void signal_handler_IO2ndtest (int status)
{
	int res = 0, rest = 0, i = 0, ser_status = 0;
	__u8 crc;
	char buf[255] = {0};
	char buf2[255] = {0};

	printf("received SIGIO signal. (0x%04x)\n",status);

	ioctl(fd, FIONREAD, &ser_status);
	printf("avail: %d chars in serial buffer\n", ser_status);

	Pack_len = 9;
	while(read(fd, buf, Pack_len)){
		/* crc check */
		if( buf[1] == 0xfe){
			crc = buf[2] + buf[3] + buf[4];
			Pack_len = 6;
		} else {
			crc = buf[2] + buf[3] + buf[4] + buf[5] + buf[6] + buf[7];
			Pack_len = 9;
		}

		for(i=0;i<Pack_len;i++)
			printf("0x%02x ", buf[i]);
		printf(" -> crc 0x%02x.\n", crc);

	}
	in_work = FALSE;
	return;
}

// void signal_handler_IO_1sttime (int status)
void signal_handler_IO (int status)
{
	int res = 0, rest = 0, i = 0, ser_status = 0;
	__u8 crc;
	char buf[255] = {0};
	char buf2[255] = {0};

	printf("received SIGIO signal. (0x%04x)\n",status);

	/*	if(in_work)
		return;
		else
		in_work = TRUE;
		*/
	ioctl(fd, FIONREAD, &ser_status);
	printf("avail: %d chars in serial buffer\n", ser_status);

	if(ser_status < 3)
		res = read(fd, buf, ser_status);
	else
		res = read(fd, buf, 3);

	printf("grap read %d:",res);
	for(i=0;i<res;i++)
		printf("0x%02x ", buf[i]);
	printf(".\n");

	if(res == 3){
		printf("header in buf? (from grap)\n");
	} else {
		res = read(fd, buf, 3);
	}

	if(res == 3){
		if(buf[0] == 0xff){
			ev_ir[0].header = buf[0];
			ev_ir[0].type = buf[1];
			ev_ir[0].event = buf[2];
			if(ev_ir[0].type == 0xfe)	Pack_len = 3;
			else if (ev_ir[0].type == 0xff)	Pack_len = 6;
			else				Pack_len = 1;
			printf("\ntrailer found: 0x%02x pack 0x%02x next len: %d\n",ev_ir[0].header, ev_ir[0].type, Pack_len);
			//	if(Pack_len != 1)
			//		break;

			printf("found: 0x%02x type: 0x%02x data to read: %d\n", ev_ir[0].header, ev_ir[0].type, Pack_len);
			if(ev_ir[0].event == 0x83) Pack_len += 1;
			if(ev_ir[0].event == 0x84) Pack_len += 2;
		}
	} else {
		printf("Fatal, short read!!!!!!!!!!\n");
		printf("read %d data but should 3!!",res);
		for(i=0;i<res;i++)
			printf("0x%02x ", buf[i]);
		printf(".\n");
		return;
	}

	ioctl(fd, FIONREAD, &ser_status);
	printf("avail: %d chars in serial buffer -> need %d\n", ser_status, Pack_len);

	res = read(fd, buf2, Pack_len);

	if(res == Pack_len)
		printf("got all %d == %d -> ",res,Pack_len);
	else
		printf("f.... %d != %d -> ",res,Pack_len);

	for(i=0;i<res;i++)
		printf("0x%02x ", buf2[i]);
	printf(".\n");

	ev_ir[0].code = buf2[0];
	ev_ir[0].flag = buf2[1];
	ev_ir[0].unk1 = buf2[2];
	ev_ir[0].unk2 = buf2[3];
	ev_ir[0].unk3 = buf2[4];
	ev_ir[0].crc = buf2[res - 1];


	ioctl(fd, FIONREAD, &ser_status);
	if(ser_status)
		printf("remain: %d chars in serial buffer, bad.\n", ser_status);
	else
		printf("pack complete?\n", ev_ir[0].header, ev_ir[0].type);

	i=0;
	/* crc check */
	crc = ev_ir[i].event + ev_ir[i].code + ev_ir[i].flag + ev_ir[i].unk1 + ev_ir[i].unk2 + ev_ir[i].unk3;
	if(ev_ir[i].type == 0xfe){
		// add extra handle for event 0x83, 0x84 ?
		crc -= ev_ir[i].crc; /* it's allready in one of unk. packages*/
	}
	printf("with calculated crc (0x%02x): %d. pack header: 0x%02x type: 0x%02x event: 0x%02x code: 0x%02x flag: 0x%02x u1: 0x%02x u2: 0x%02x u3: 0x%02x crc: 0x%02x \n",crc,i,
			ev_ir[i].header,
			ev_ir[i].type,
			ev_ir[i].event,
			ev_ir[i].code,
			ev_ir[i].flag,
			ev_ir[i].unk1,
			ev_ir[i].unk2,
			ev_ir[i].unk3,
			ev_ir[i].crc);

	in_work = FALSE;
	return;
}

