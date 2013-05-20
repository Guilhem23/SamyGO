#include <types.h>

typedef volatile unsigned int ssdtv_reg_t;
struct sdp_uart {
	ssdtv_reg_t	ulcon;
	ssdtv_reg_t	ucon;
	ssdtv_reg_t	ufcon;
	ssdtv_reg_t	umcon;

	ssdtv_reg_t	utrstat;
	ssdtv_reg_t	uerstat;
	ssdtv_reg_t	ufstat;
	ssdtv_reg_t	umstat;

	ssdtv_reg_t	utxh;
	ssdtv_reg_t	urxh;
	ssdtv_reg_t	ubrdiv;
};


static struct sdp_uart *micom_uart;	//Genoa-P, Micom, uart1
static struct sdp_uart *uart;		//Genoa-S, debugging uart , uart3

extern void memset(void *pDst, UINT8 nV, UINT32 nLen);
extern ulong get_clock_apb(void);
extern void __udelay(unsigned long usec);

#define ULCON0	0x30090A00
#define ULCON1	0x30090A40
#define ULCON2	0x30090A80
#define ULCON3	0x30090AC0

static void uart_init(void) __attribute__((noinline));
static void uart_init(void)
{
	ssdtv_reg_t apb, val, val2, pms, regval;

	uart->ulcon = 0x3;
	uart->ucon = 0x3085;
	uart->ufcon = 0x1;	/* fifo disabled */
	uart->umcon = 0x0;

	apb = get_clock_apb();

	/* PCLK / 16 / 115200 -1 */
	val = (apb /16 / 11520 -5)/10;
	val2 = ((unsigned int)apb / (9600 * 16)) -1;
	uart->ubrdiv = val;

	/* debugging uart init */
	if(uart != 0)
	{
		uart->ulcon = 0x3;
		uart->ucon = 0x3085;
		uart->ufcon = 0x1;	/* fifo disabled */
		uart->umcon = 0x0;
		uart->ubrdiv = val;
	}
}

static void uart_wait_rx(void)
{
	while ( (uart->utrstat & 0x1) == 0) {
		/* nop */
	}
}

static void uart_wait_tx(void)
{
	while ( (uart->utrstat & 0x2) == 0) {
		/* nop */
	}
}

/* exposed function */
void sdp_peri_init(void)
{
#if defined(FAST_LOGO)
	/* port 0 */
	micom_uart = (struct sdp_uart *) ULCON0; //Echo-P, Micom, uart0
#endif //defined(FAST_LOGO)

	/* port 3 */
	//uart = (struct sdp_uart *)(0x30090a00); /* UART 0 */
	uart = (struct sdp_uart *)(0x30090a80);
	uart_init();
}

#if 0
/*******************************/
/* For Micom Control  : Genoa-S : uart 2 */
/*******************************/
char micom_getc(void)
{
	while ( (micom_uart->utrstat & 0x1) == 0) {
		/* nop */
	}
	return (micom_uart->urxh & 0xff);
}

void micom_putc(char c)
{
	while ( (micom_uart->utrstat & 0x4) == 0) {
		/* nop */
	}
	micom_uart->utxh = c;
}

/*	
	Command List

	toggle   : 0x35
	shutdown : 0x12
	reboot   : 0x1d
	rollback : 0x34
*/
int micom_cmd(int command, int option)
{
	unsigned char w_databuff[9];
	unsigned char r_databuff[255];
	int ack = 0x6d;
	int retry = 2; //5
	int cnt = 0;
	int i = 0;

	while(retry-- > 0)
	{
		memset(w_databuff, 0, 9);
		memset(r_databuff, 0, 255);

		w_databuff[0] = 0xff;
		w_databuff[1] = 0xff;
		w_databuff[2] = command;
		w_databuff[3] = option;

		w_databuff[8] = w_databuff[2];
		w_databuff[8] += w_databuff[3];

		//Send command to MICOM
		for(i=0; i<9; i++)
		{
			micom_putc(w_databuff[i]);
		}
		__udelay(10 * 1000); // 10ms
	}

}
#endif

