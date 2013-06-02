#ifndef __S4LJ003X_H__
#define __S4LJ003X_H__

#define S4LX_UART_CHANNELS	3
typedef enum
{
	S4LX_URART0,
	S4LX_URART1,
	S4LX_URART2
}S4LX_UART_NR;

#define S4LX_UART_BASE			0x30090A00
#define S4LX_TIMER_BASE			0x30090400
#define S4LX_PCI_BASE			0x30030000
#define S4LX_CLOCK_POWER_BASE	0x30090800
#define S4LX_WATCHDOG_BASE		0x30090600
#define S4LX_RTC_BASE			0x30090500

#include "s4lx.h"

static inline  S4LX_UART * S4LX_GetBase_UART(S4LX_UART_NR nr)
{
	return (S4LX_UART * const)(S4LX_UART_BASE+(nr*0x40));
}

static inline S4LX_PCI * S4LX_GetBase_PCI(void)
{
	return (S4LX_PCI *const)S4LX_PCI_BASE;
}

static  inline S4LX_TIMER * S4LX_GetBase_TIMER(void)
{
	return (S4LX_TIMER *)S4LX_TIMER_BASE;
}

static  inline S4LX_CLOCK_POWER * S4LX_GetBase_CLOCK_POWER(void)
{
	return (S4LX_CLOCK_POWER *const)S4LX_CLOCK_POWER_BASE;
}

static  inline S4LX_WATCHDOG * S4LX_GetBase_WATCHDOG(void)
{
	return (S4LX_WATCHDOG * const)S4LX_WATCHDOG_BASE;
}

static inline S4LX_RTC * S4LX_GetBase_RTC(void)
{
	return (S4LX_RTC* const)S4LX_RTC_BASE;
}

/* TODO: integrate these registers */
#define rTSD_STC_BASE		(0x30110080)
#define rSMCCLKSTOP		(0x300201E8)
#define rSMCSYNCEN		(0x300201EC)

/* mask */
#define USB_PCI_TXPREEMP	(0x1 << 17)
#define USB_PCI_TXVREF		(0xF << 19)
#define USB_PCI_REFCLKSEL	(0x3 << 25)

#endif /* __S4LJ003X_H__ */
