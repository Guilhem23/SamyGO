#include <version.h>
#include <sp_setup.h>
#include <types.h>
#include <sdp_devmmc.h>
#include <config.h>
#include <stdarg.h>

char        console_buffer[CFG_CBSIZE];		/* console I/O buffer	*/
unsigned int load_addr;
//"root=/dev/mmcblk0p14 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot=053 quiet"
//const char *bootargs[] = {
//	"root=/dev/mmcblk0p3 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot=055 quiet",
//	"root=/dev/mmcblk0p6 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot=055 quiet",
//	"root=/dev/mmcblk0p14 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot=055 quiet",
//	"root=/dev/sda1 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot=055 quiet",
//	"root=/dev/nfs rw nfsroot=192.168.0.130:/mnt/disk1/TV_rootfs rootpath=/home/root ip=192.168.0.11:192.168.0.130:192.168.0.252:255.255.255.0:debian:eth0:off console=ttyS1,115200n8"
//};
#define MAX_CMDBUF_SIZE		256
#define putnstr(str,n)	do {			\
		printf ("%.*s", n, str);	\
	} while (0)

#define CTL_CH(c)		((c) - 'a' + 1)

#define MAX_CMDBUF_SIZE		256

#define CTL_BACKSPACE		('\b')
#define DEL			((char)255)
#define DEL7			((char)127)
#define CREAD_HIST_CHAR		('!')

#define getcmd_putch(ch)	putc(ch)
#define getcmd_getch()		getc()
#define getcmd_cbeep()		getcmd_putch('\a')

#define HIST_MAX		20
#define HIST_SIZE		MAX_CMDBUF_SIZE

static int hist_max = 0;
static int hist_add_idx = 0;
static int hist_cur = -1;
unsigned hist_num = 0;

char* hist_list[HIST_MAX];
char hist_lines[HIST_MAX][HIST_SIZE];

#define add_idx_minus_one() ((hist_add_idx == 0) ? hist_max : hist_add_idx-1)
#define BEGINNING_OF_LINE() {			\
	while (num) {				\
		getcmd_putch(CTL_BACKSPACE);	\
		num--;				\
	}					\
}

#define ERASE_TO_EOL() {				\
	if (num < eol_num) {				\
		int tmp;				\
		for (tmp = num; tmp < eol_num; tmp++)	\
			getcmd_putch(' ');		\
		while (tmp-- > num)			\
			getcmd_putch(CTL_BACKSPACE);	\
		eol_num = num;				\
	}						\
}

#define REFRESH_TO_EOL() {			\
	if (num < eol_num) {			\
		wlen = eol_num - num;		\
		putnstr(buf + num, wlen);	\
		num = eol_num;			\
	}					\
}

unsigned int get_load_adress( void ) {
	
	return load_addr;	

}
void set_load_adress(unsigned int val) {
	load_addr = val;
}

void printf (const char *fmt, ...)
{
	va_list args;
	uint i;
	char printbuffer[CFG_PBSIZE];

	va_start (args, fmt);

	/* For this to work, printbuffer must be larger than
	 * anything we ever want to print.
	 */
	i = vsprintf (printbuffer, fmt, args);
	va_end (args);

	/* Print the string */
	puts (printbuffer);

}

static void hist_init(void)
{
	int i;

	hist_max = 0;
	hist_add_idx = 0;
	hist_cur = -1;
	hist_num = 0;

	for (i = 0; i < HIST_MAX; i++) {
		hist_list[i] = hist_lines[i];
		hist_list[i][0] = '\0';
	}
}

static void cread_add_to_hist(char *line)
{
	strcpy(hist_list[hist_add_idx], line);

	if (++hist_add_idx >= HIST_MAX)
		hist_add_idx = 0;

	if (hist_add_idx > hist_max)
		hist_max = hist_add_idx;

	hist_num++;
}

static char* hist_prev(void)
{
	char *ret;
	int old_cur;

	if (hist_cur < 0)
		return NULL;

	old_cur = hist_cur;
	if (--hist_cur < 0)
		hist_cur = hist_max;

	if (hist_cur == hist_add_idx) {
		hist_cur = old_cur;
		ret = NULL;
	} else
		ret = hist_list[hist_cur];

	return (ret);
}

static char* hist_next(void)
{
	char *ret;

	if (hist_cur < 0)
		return NULL;

	if (hist_cur == hist_add_idx)
		return NULL;

	if (++hist_cur > hist_max)
		hist_cur = 0;

	if (hist_cur == hist_add_idx) {
		ret = "";
	} else
		ret = hist_list[hist_cur];

	return (ret);
}
static void cread_add_char(char ichar, int insert, unsigned long *num,
	       unsigned long *eol_num, char *buf, unsigned long len)
{
	unsigned long wlen;

	/* room ??? */
	if (insert || *num == *eol_num) {
		if (*eol_num > len - 1) {
			getcmd_cbeep();
			return;
		}
		(*eol_num)++;
	}

	if (insert) {
		wlen = *eol_num - *num;
		if (wlen > 1) {
			memmove(&buf[*num+1], &buf[*num], wlen-1);
		}

		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
		while (--wlen) {
			getcmd_putch(CTL_BACKSPACE);
		}
	} else {
		/* echo the character */
		wlen = 1;
		buf[*num] = ichar;
		putnstr(buf + *num, wlen);
		(*num)++;
	}
}

static void cread_add_str(char *str, int strsize, int insert, unsigned long *num,
	      unsigned long *eol_num, char *buf, unsigned long len)
{
	while (strsize--) {
		cread_add_char(*str, insert, num, eol_num, buf, len);
		str++;
	}
}

static int cread_line(char *buf, unsigned int *len)
{
	unsigned long num = 0;
	unsigned long eol_num = 0;
	unsigned long rlen;
	unsigned long wlen;
	char ichar;
	int insert = 1;
	int esc_len = 0;
	int rc = 0;
	char esc_save[8];

	while (1) {
		rlen = 1;
		ichar = getcmd_getch();

		if ((ichar == '\n') || (ichar == '\r')) {
			putc('\n');
			break;
		}

		/*
		 * handle standard linux xterm esc sequences for arrow key, etc.
		 */
		if (esc_len != 0) {
			if (esc_len == 1) {
				if (ichar == '[') {
					esc_save[esc_len] = ichar;
					esc_len = 2;
				} else {
					cread_add_str(esc_save, esc_len, insert,
						      &num, &eol_num, buf, *len);
					esc_len = 0;
				}
				continue;
			}

			switch (ichar) {

			case 'D':	/* <- key */
				ichar = CTL_CH('b');
				esc_len = 0;
				break;
			case 'C':	/* -> key */
				ichar = CTL_CH('f');
				esc_len = 0;
				break;	/* pass off to ^F handler */
			case 'H':	/* Home key */
				ichar = CTL_CH('a');
				esc_len = 0;
				break;	/* pass off to ^A handler */
			case 'A':	/* up arrow */
				ichar = CTL_CH('p');
				esc_len = 0;
				break;	/* pass off to ^P handler */
			case 'B':	/* down arrow */
				ichar = CTL_CH('n');
				esc_len = 0;
				break;	/* pass off to ^N handler */
			default:
				esc_save[esc_len++] = ichar;
				cread_add_str(esc_save, esc_len, insert,
					      &num, &eol_num, buf, *len);
				esc_len = 0;
				continue;
			}
		}

		switch (ichar) {
		case 0x1b:
			if (esc_len == 0) {
				esc_save[esc_len] = ichar;
				esc_len = 1;
			} else {
				puts("impossible condition #876\n");
				esc_len = 0;
			}
			break;

		case CTL_CH('a'):
			BEGINNING_OF_LINE();
			break;
		case CTL_CH('c'):	/* ^C - break */
			*buf = '\0';	/* discard input */
			return (-1);
		case CTL_CH('f'):
			if (num < eol_num) {
				getcmd_putch(buf[num]);
				num++;
			}
			break;
		case CTL_CH('b'):
			if (num) {
				getcmd_putch(CTL_BACKSPACE);
				num--;
			}
			break;
		case CTL_CH('d'):
			if (num < eol_num) {
				wlen = eol_num - num - 1;
				if (wlen) {
					memmove(&buf[num], &buf[num+1], wlen);
					putnstr(buf + num, wlen);
				}

				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('k'):
			ERASE_TO_EOL();
			break;
		case CTL_CH('e'):
			REFRESH_TO_EOL();
			break;
		case CTL_CH('o'):
			insert = !insert;
			break;
		case CTL_CH('x'):
			BEGINNING_OF_LINE();
			ERASE_TO_EOL();
			break;
		case DEL:
		case DEL7:
		case 8:
			if (num) {
				wlen = eol_num - num;
				num--;
				memmove(&buf[num], &buf[num+1], wlen);
				getcmd_putch(CTL_BACKSPACE);
				putnstr(buf + num, wlen);
				getcmd_putch(' ');
				do {
					getcmd_putch(CTL_BACKSPACE);
				} while (wlen--);
				eol_num--;
			}
			break;
		case CTL_CH('p'):
		case CTL_CH('n'):
		{
			char * hline;

			esc_len = 0;

			if (ichar == CTL_CH('p'))
				hline = hist_prev();
			else
				hline = hist_next();

			if (!hline) {
				getcmd_cbeep();
				continue;
			}

			/* nuke the current line */
			/* first, go home */
			BEGINNING_OF_LINE();

			/* erase to end of line */
			ERASE_TO_EOL();

			/* copy new line into place and display */
			strcpy(buf, hline);
			eol_num = strlen(buf);
			REFRESH_TO_EOL();
			continue;
		}
		default:
			cread_add_char(ichar, insert, &num, &eol_num, buf, *len);
			break;
		}
	}
	*len = eol_num;
	buf[eol_num] = '\0';	/* lose the newline */

	if (buf[0] && buf[0] != CREAD_HIST_CHAR)
		cread_add_to_hist(buf);
	hist_cur = hist_add_idx;

	return (rc);
}

int readline (const char *const prompt)
{
#ifdef CONFIG_CMDLINE_EDITING
	char *p = console_buffer;
	unsigned int len=MAX_CMDBUF_SIZE;
	int rc;
	static int initted = 0;

	if (!initted) {
		hist_init();
		initted = 1;
	}

	puts (prompt);

	rc = cread_line(p, &len);
	return rc < 0 ? rc : len;
#else
	char   *p = console_buffer;
	int	n = 0;				/* buffer index		*/
	int	plen = 0;			/* prompt length	*/
	int	col;				/* output column cnt	*/
	char	c;

	/* print prompt */
	if (prompt) {
		plen = strlen (prompt);
		puts (prompt);
	}
	col = plen;

	for (;;) {
#ifdef CONFIG_BOOT_RETRY_TIME
		while (!tstc()) {	/* while no incoming data */
			if (retry_time >= 0 && get_ticks() > endtime)
				return (-2);	/* timed out */
		}
#endif
		WATCHDOG_RESET();		/* Trigger watchdog, if needed */

#ifdef CONFIG_SHOW_ACTIVITY
		while (!tstc()) {
			extern void show_activity(int arg);
			show_activity(0);
		}
#endif
		c = getc();

		/*
		 * Special character handling
		 */
		switch (c) {
		case '\r':				/* Enter		*/
		case '\n':
			*p = '\0';
			puts ("\r\n");
			return (p - console_buffer);

		case '\0':				/* nul			*/
			continue;

		case 0x03:				/* ^C - break		*/
			console_buffer[0] = '\0';	/* discard input */
			return (-1);

		case 0x15:				/* ^U - erase line	*/
			while (col > plen) {
				puts (erase_seq);
				--col;
			}
			p = console_buffer;
			n = 0;
			continue;

		case 0x17:				/* ^W - erase word 	*/
			p=delete_char(console_buffer, p, &col, &n, plen);
			while ((n > 0) && (*p != ' ')) {
				p=delete_char(console_buffer, p, &col, &n, plen);
			}
			continue;

		case 0x08:				/* ^H  - backspace	*/
		case 0x7F:				/* DEL - backspace	*/
			p=delete_char(console_buffer, p, &col, &n, plen);
			continue;

		default:
			/*
			 * Must be a normal character then
			 */
			if (n < CFG_CBSIZE-2) {
				if (c == '\t') {	/* expand TABs		*/
#ifdef CONFIG_AUTO_COMPLETE
					/* if auto completion triggered just continue */
					*p = '\0';
					if (cmd_auto_complete(prompt, console_buffer, &n, &col)) {
						p = console_buffer + n;	/* reset */
						continue;
					}
#endif
					puts (tab_seq+(col&07));
					col += 8 - (col&07);
				} else {
					++col;		/* echo input		*/
					putc (c);
				}
				*p++ = c;
				++n;
			} else {			/* Buffer full		*/
				putc ('\a');
			}
		}
	}
#endif /* CONFIG_CMDLINE_EDITING */
}

static uchar env_get_char_init (int index);
uchar (*env_get_char)(int) = env_get_char_init;

unsigned char *env_get_addr (int index)
{
	return ( ((unsigned char *)(OS_ONBOOT_ADDR2 + index)) );
}

unsigned char env_get_char_spec (int index)
{
	/* ij.jang */
	//DECLARE_GLOBAL_DATA_PTR;

	return (*((uchar *)(OS_ONBOOT_ADDR2 + index)));
}

static uchar env_get_char_init (int index)
{
	uchar c;
	c = env_get_char_spec(index);
	return (c);
}

static int envmatch (uchar *s1, int i2)
{

	while (*s1 == env_get_char(i2++))
		if (*s1++ == '=')
			return(i2);
	if (*s1 == '\0' && env_get_char(i2-1) == '=')
		return(i2);
	return(-1);
}
int _do_setenv (int flag, int argc, char *argv[])
{
	int   i, len, oldval;
	int   console = -1;
	unsigned char *env, *nxt = NULL;
	char *name;


	unsigned char *env_data = env_get_addr(0);

	if (!env_data)	/* need copy in RAM */
		return 1;

	name = argv[1];

	if (strchr(name, '=')) {
		printf ("## Error: illegal character '=' in variable name \"%s\"\n", name);
		return 1;
	}

	/*
	 * search if variable with this name already exists
	 */
	oldval = -1;
	for (env=env_data; *env; env=nxt+1) {
		for (nxt=env; *nxt; ++nxt)
			;
		if ((oldval = envmatch((uchar *)name, env-env_data)) >= 0)
			break;
	}

	/*
	 * Delete any existing definition
	 */
	if (oldval >= 0) {
#ifndef CONFIG_ENV_OVERWRITE

		/*
		 * Ethernet Address and serial# can be set only once,
		 * ver is readonly.
		 */
		if ( (strcmp (name, "serial#") == 0) ||
		    ((strcmp (name, "ethaddr") == 0)
		    ) ) {
			printf ("Can't overwrite \"%s\"\n", name);
			return 1;
		}
#endif
		if (*++nxt == '\0') {
			if (env > env_data) {
				env--;
			} else {
				*env = '\0';
			}
		} else {
			for (;;) {
				*env = *nxt++;
				if ((*env == '\0') && (*nxt == '\0'))
					break;
				++env;
			}
		}
		*++env = '\0';
	}

	/* Delete only ? */
	if ((argc < 3) || argv[2] == NULL) {
		//env_crc_update ();
		return 0;
	}

	/*
	 * Append new definition at the end
	 */
	for (env=env_data; *env || *(env+1); ++env)
		;
	if (env > env_data)
		++env;
	/*
	 * Overflow when:
	 * "name" + "=" + "val" +"\0\0"  > ENV_SIZE - (env-env_data)
	 */
	len = strlen(name) + 2;
	/* add '=' for first arg, ' ' for all others */
	for (i=2; i<argc; ++i) {
		len += strlen(argv[i]) + 1;
	}
	if (len > (&env_data[ENV_SIZE]-env)) {
		printf ("## Error: environment overflow, \"%s\" deleted\n", name);
		return 1;
	}
	while ((*env = *name++) != '\0')
		env++;
	for (i=2; i<argc; ++i) {
		char *val = argv[i];

		*env = (i==2) ? '=' : ' ';
		while ((*++env = *val++) != '\0')
			;
	}

	/* end is marked with double '\0' */
	*++env = '\0';

	/* Update CRC */
//	env_crc_update ();

	if (strcmp(argv[1],"loadaddr") == 0) {
		load_addr = simple_strtoul(argv[2], NULL, 16);
		return 0;
	}
	return 0;
}

void setenv (char *varname, char *varvalue)
{
	char *argv[4] = { "setenv", varname, varvalue, NULL };
	_do_setenv (0, 3, argv);
}

char *getenv (char *name)
{
	int i, nxt;

	//WATCHDOG_RESET();

	for (i=0; env_get_char(i) != '\0'; i=nxt+1) {
		int val;

		for (nxt=i; env_get_char(nxt) != '\0'; ++nxt) {
			if (nxt >= CFG_ENV_SIZE) {
				return (NULL);
			}
		}
		if ((val=envmatch((uchar *)name, i)) < 0)
			continue;
		return ((char *)env_get_addr(val));
	}

	return (NULL);
}

int saveenv(int part)
{
	
	int err=sdp_mmc_RW(WRITE_MMC_MODE, CMAC_1*BLK_SIZE_MMC, OS_ONBOOT_ADDR2, 8192);
}

int backenv(int part)
{	
int err;
	 err=sdp_mmc_RW(READ_MMC_MODE , CMAC_1*BLK_SIZE_MMC, OS_ONBOOT_ADDR2, 8192);
	 err=sdp_mmc_RW(WRITE_MMC_MODE, CMAC_2*BLK_SIZE_MMC, OS_ONBOOT_ADDR2, 8192);
}

int restenv(int part)
{	
	int err;
	err=sdp_mmc_RW(READ_MMC_MODE , CMAC_2*BLK_SIZE_MMC, OS_ONBOOT_ADDR2, 8192);
	err=sdp_mmc_RW(WRITE_MMC_MODE, CMAC_1*BLK_SIZE_MMC, OS_ONBOOT_ADDR2, 8192);
}

int env_init(int part)
{
	
	int err=sdp_mmc_RW(READ_MMC_MODE, CMAC_1*BLK_SIZE_MMC, OS_ONBOOT_ADDR2, 8192);
	return 0;
}

int help_setup(void) {
	
	printf("---------------------------------------------------------------\n");
	printf("---------------------- 2nd OnBoot Menue -----------------------\n");
	printf("---------------------------------------------------------------\n");	
	printf("env          - Environment Setup\n");
	printf("bootd        - Boot Kernel from Environment\n");
	printf("bootm			- Run Application from Memory\n");
	printf("loadaddr     - Set base address\n");
	printf("toggle       - Toogle Kernel parition\n");
	printf("reboot       - Reboot Device\n");
	printf("down         - Shutdown Device\n");
	printf("usb          - Start OHCI USB Driver (Test)\n");
	printf("mem          - Memory Play\n");
	printf("micom        - Listen to Micom\n");
	printf("serial       - Serial Download\n");
	printf("---------------------------------------------------------------\n");	
	
}

int bootcommand_setup(void)
{
	char *buffer1;
	unsigned char data[255];
	
	while(1)
	{
		int i;

		printf("\n");
		printf("------------------------------\n");
		printf("-     Micom                  -\n");
		printf("------------------------------\n");
		printf("1. Read\n");
		printf("x. EXIT\n");
		printf("------------------------------\n");

		readline("SELECT COMMAND: ");
		if(console_buffer[1] != NULL)
			continue;
		i = (int)console_buffer[0];
		printf("\n");

		switch(i)
		{
			case '1':
				micom_get(data, 50 );
				dump_hex(data, 50);	
				break;

			case 'x':
				return 1;

			default:
				;
		}
	}
}

int env_setup(void)
{
	char buffer1[1000];
	char *s;
	while(1)
	{
		int i;

		printf("\n");
		printf("------------------------------\n");
		printf("-     ENVIRONMENT SETUP      -\n");
		printf("------------------------------\n");
		printf("1. BOOTING COMMAND      = %s\n", getenv("bootcmd"));
		printf("2. ROOTFS DEVICE        = %s\n", getenv("rootdev"));
		printf("3. BOOTING DELAY        = %s\n", getenv("bootdelay"));
		printf("4. TARGET IP            = %s\n", getenv("ipaddr"));
		printf("5. SERVER IP            = %s\n", getenv("serverip"));
		printf("6. GATEWAY IP           = %s\n", getenv("gatewayip"));
		printf("7. ETHERNET ADDRESS     = %s\n", getenv("ethaddr"));
		printf("8. ROOT PATH            = %s\n", getenv("rootpath"));
		printf("9. BOOTING ARGS         = %s\n", getenv("bootargs"));
		printf("a. KERNEL PARITION      = %s\n", getenv("bootparition"));
		printf("b. KERNEL LOAD ADDRESS  = %s\n", getenv("loadaddr"));
		printf("c. UPDATING BOOTING ARGS\n");
		printf("------------------------------\n");
		printf("s. SAVE    ENVIRONMENT\n");
		printf("f. BACKUP  ENVIRONMENT\n");
		printf("r. RESTORE ENVIRONMENT\n");
		printf("x. EXIT\n");
		printf("------------------------------\n");

		readline("SELECT COMMAND: ");
		if(console_buffer[1] != NULL)
			continue;
		i = (int)console_buffer[0];
		printf("\n");
		switch(i){
			case '1':
				readline("INPUT BOOTING COMMAND : ");
				setenv("bootcmd",console_buffer);
				break;
			
			case '2':
				readline("INPUT ROOTFS DEVICE : ");
				setenv("rootdev",console_buffer);
				break;
			case '3':
				readline("INPUT BOOTING DELAY : ");
				setenv("bootdelay",console_buffer);
				break;
			case '4':
				readline("INPUT TARGET IP : ");
				setenv("ipaddr",console_buffer);	
				break;
			case '5':
				readline("INPUT SERVER IP : ");
				setenv("serverip",console_buffer);
				break;
			case '6':
				readline("INPUT GATEWAY IP : ");
				setenv("gatewayip",console_buffer);
				break;
			case '7':
				readline("INPUT ETHERNET ADDRESS : ");
				setenv("ethaddr",console_buffer);
				break;
			case '8':
				readline("INPUT ROOT PATH : ");
				setenv("rootpath",console_buffer);
				break;
			case '9':
				readline("INPUT BOOTING ARGS : ");
				setenv("bootargs",console_buffer);
				markKernel( console_buffer );
				break;
			case 'a':
				readline("INPUT KERNEL BOOT PARITION : ");
				setenv("bootparition",console_buffer);
				break;
			case 'b':
				readline("INPUT KERNEL BOOT ADRESS : ");
				setenv("loadaddr",console_buffer);
				break;
			case 'c':
					s = getenv("rootdev");
					printf("UPDATING BOOTING ARGS...\n");
					
					//char buffer1[1000];// = (char *)malloc(500 * sizeof(char));
					// "root=/dev/nfs rw nfsroot=192.168.0.130:/mnt/disk1/TV_rootfs rootpath=/home/root ip=192.168.0.11:192.168.0.130:192.168.0.252:255.255.255.0:debian:eth0:off console=ttyS1,115200n8"
					if (strstr( s  , "/dev/nfs")) {
						strcpy(buffer1,"root=/dev/nfs rw nfsroot=");//139:6 rw ip=");
						
						strcat(buffer1,getenv("serverip"));
						strcat(buffer1,":");
						strcat(buffer1,getenv("rootpath"));
						strcat(buffer1," ip=");
						strcat(buffer1,getenv("ipaddr"));
						strcat(buffer1,":");
						strcat(buffer1,getenv("serverip"));
						strcat(buffer1,":");
						strcat(buffer1,getenv("gatewayip"));
						strcat(buffer1,":255.255.255.0::eth0:off console=ttyS1,115200n8 ethaddr=");
						strcat(buffer1,getenv("ethaddr"));
						setenv("bootargs",buffer1);
						markKernel( getenv("bootargs") );
						
					} else if (strstr( s  , "/dev/sda")) {
						strcpy(buffer1,"root=");
						strcat(buffer1,getenv("rootdev"));
						strcat(buffer1," rootwait rootfstype=ext3 console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot=055 quiet");
						setenv("bootargs",buffer1);
						markKernel( getenv("bootargs") );
					
					} else if (strstr( s  , "/dev/mmc")) {
						strcpy(buffer1,"root=");
						strcat(buffer1,getenv("rootdev"));
						strcat(buffer1," console=ttyS1,115200n8 SELP_ENABLE=20102011 onboot=055 quiet");
						setenv("bootargs",buffer1);
						markKernel( getenv("bootargs") );
					}
				break;
				//printf("PLEASE USE 'UPDATING BOOTING ARGS' COMMAND\n");
			case 'f' :
				backenv(1);
				printf("env backed up...\n");
				break;
			case 'r' :
				restenv(1);
				printf("env restored...\n");
				break;
			case 's':
				saveenv(0);
				printf("env saved...\n");
				break;

			case 'x':
				return 1;

			default:
				;
		}
	}
}

int micom_setup(void)
{
	char *buffer1;
	unsigned char data[255];
	
	while(1)
	{
		int i;

		printf("\n");
		printf("------------------------------\n");
		printf("-     Micom                  -\n");
		printf("------------------------------\n");
		printf("1. Read\n");
		printf("x. EXIT\n");
		printf("------------------------------\n");

		readline("SELECT COMMAND: ");
		if(console_buffer[1] != NULL)
			continue;
		i = (int)console_buffer[0];
		printf("\n");

		switch(i)
		{
			case '1':
				micom_get(data, 50 );
				dump_hex(data, 50);	
				break;

			case 'x':
				return 1;

			default:
				;
		}
	}
}

int reg_setup(void)
{
	char *buffer1;
	unsigned int adr, size, i, val;
	
	while(1)
	{
		int i;

		printf("\n");
		printf("------------------------------\n");
		printf("-     Memory                 -\n");
		printf("------------------------------\n");
		printf("1. Read\n");
		printf("2. Write\n");
		printf("x. EXIT\n");
		printf("------------------------------\n");

		readline("SELECT COMMAND: ");
		if(console_buffer[1] != NULL)
			continue;
		i = (int)console_buffer[0];
		printf("\n");

		switch(i)
		{
			case '1':
				readline("INPUT ADRESS : ");
				adr = (int)simple_strtol(console_buffer, NULL, 10);
				readline("INPUT SIZE   : ");
				size = (int)simple_strtol(console_buffer, NULL, 10);
				for ( i = 0; i < size; i++) {
					printf("%08x -> %08x\n",adr,  *(volatile unsigned int*)adr+i )	;
				}
				break;

			case '2':
				readline("INPUT ADRESS : ");
				adr = (int)simple_strtol(console_buffer, NULL, 10);
				readline("INPUT VALUE  : ");
				val = (int)simple_strtol(console_buffer, NULL, 10);
				*(volatile unsigned int*)adr = val;
				break;

			case 'x':
				return 1;

			default:
				;
		}
	}
}
int abortboot(int bootdelay)
{
	int abort = 0;

	printf("Hit Enter key to stop autoboot\n");

	while ((bootdelay > 0) && (!abort)) {
		int i;

		--bootdelay;
		/* delay 100 * 10ms */
		for (i=0; !abort && i<100; ++i) {
			if (tstc() && (getc() == '\r')) /* we got a key press	*/
			{	
				abort  = 1;		/* don't auto boot	*/
				bootdelay = 0;	/* no more delay	*/
				break;
			}
			udelay (10000);
		}
	}
	return abort;
}

void command_line(void) {
	
	int len = 0;
	for (;;) {
		len = 0;
		len = readline (CONFIG_SYS_PROMPT);
		if (len > 0) {
			if (strstr("exit", console_buffer)) {
				break;
			}else if (strstr("env", console_buffer)) {
				env_setup();
			}else if (strstr("usb", console_buffer)) {
				usb_init();	
			}else if (strstr("reboot", console_buffer)) {
				printf("Rebooting.....\n");
				micom_cmd(0x1d, 0, 0); 	
			}else if (strstr("power", console_buffer)) {
				printf("Shuting down.....\n");
				micom_cmd(0x12, 0, 0); 
			}else if (strstr("serial", console_buffer)) {
				printf("Serial Download.....\n");
				if ( check_host() ) {
					int file_size = receive_data();
					printf("Serial Download %x bytes\n", file_size);
				} else {
					printf("Skip Serial Download\n");
				}
			} else if (strstr("help", console_buffer)) {
				help_setup();	
			} else if (strstr("toggle", console_buffer)) {
				int part = getPartFlag();
				if (part == 0) {
					micom_cmd(0x35, 0, 1); 
					printf("Toogle to parition 1 sended\n");
				} else {
					micom_cmd(0x35,0, 0); 
					printf("Toogle to parition 0 sended\n");
				}
			} else if (strstr("bootd", console_buffer)) {
				boot_kernel();
			} else if (strstr("bootm", console_buffer)) {
				printf("[ECHO_P] Run code %08x\n", load_addr);
				exec_memory(load_addr);
			}else if (strstr("loadaddr", console_buffer)) {
				readline("INPUT ADRESS : ");
				setenv("loadaddr",console_buffer);
				printf("load_addr = %08x\n", load_addr);
			}else if (strstr("mem", console_buffer)) {
				reg_setup();
			}else if (strstr("micom", console_buffer)) {
				micom_setup();
			} 
			else {
				printf("unknown command : %s\n", console_buffer);	
			}
		}

	}	
}