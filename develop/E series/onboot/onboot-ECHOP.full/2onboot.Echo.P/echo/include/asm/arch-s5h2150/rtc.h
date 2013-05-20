/*
 * linux/include/asm-arm/arch-sdp75/rtc.h
 *
 *  Copyright (C) 2003-2007 Samsung Electronics
 *  Author: tukho.kim@samsung.com
 *
 */

#ifndef _SDP75_RTC_H_
#define _SDP75_RTC_H_

/* RTC */
#ifdef __BIG_ENDIAN
#define V_RTC(offset)  (*(volatile char *)(V_RTC_BASE+(offset)+3))
#else
#define V_RTC(offset)  (*(volatile char *)(V_RTC_BASE+(offset)))
#endif

#define R_RTCCON    		V_RTC(0x00) /* RTC control */
#define R_RTCALM    		V_RTC(0x04) /* RTC alarm control */
#define R_ALMSEC    		V_RTC(0x08) /* Alarm second */
#define R_ALMMIN    		V_RTC(0x0c) /* Alarm minute */
#define R_ALMHOUR   		V_RTC(0x10) /* Alarm Hour */
#define R_ALMDATE   		V_RTC(0x14) /* Alarm day */
#define R_ALMMON    		V_RTC(0x18) /* Alarm month */
#define R_ALMYEAR   		V_RTC(0x1c) /* Alarm year */
#define R_BCDSEC    		V_RTC(0x20) /* BCD second */
#define R_BCDMIN    		V_RTC(0x24) /* BCD minute */
#define R_BCDHOUR   		V_RTC(0x28) /* BCD hour */
#define R_BCDDATE   		V_RTC(0x2c) /* BCD day */
#define R_BCDDDAY   		V_RTC(0x30) /* BCD date */
#define R_BCDMON    		V_RTC(0x34) /* BCD month */
#define R_BCDYEAR   		V_RTC(0x38) /* BCD year */
#define R_TICNT     		V_RTC(0x40) /* Tick time count */
#define R_RTCRST    	      	V_RTC(0x44) /* RTC round reset */
#define R_RTCSTA    		V_RTC(0x48) /* RTC status */

/* Field */
#define fRTC_SEC                Fld(7,0)
#define fRTC_MIN                Fld(7,0)
#define fRTC_HOUR               Fld(6,0)
#define fRTC_DATE               Fld(6,0)
#define fRTC_DAY                Fld(3,0)
#define fRTC_MON                Fld(5,0)
#define fRTC_YEAR               Fld(8,0)
/* Mask */
#define Msk_RTCSEC              FMsk(fRTC_SEC)
#define Msk_RTCMIN              FMsk(fRTC_MIN)
#define Msk_RTCHOUR             FMsk(fRTC_HOUR)
#define Msk_RTCDAY              FMsk(fRTC_DAY)
#define Msk_RTCDATE             FMsk(fRTC_DATE)
#define Msk_RTCMON              FMsk(fRTC_MON)
#define Msk_RTCYEAR             FMsk(fRTC_YEAR)
/* bits */
#define RTCCON_EN               (1 << 0) /* RTC Control Enable */
#define RTCCON_CLKSEL           (1 << 1) /* BCD clock as XTAL 1/2^25 clock */
#define RTCCON_CNTSEL           (1 << 2) /* 0: Merge BCD counters */
#define RTCCON_CLKRST           (1 << 3) /* RTC clock count reset */

/* Round Reset Register */
#define RTCRST_SRSTEN           (1<<3)          /* Enable Round Reset   -one shot */
#define RTCRST_SECCR_30         (3)             /* Round Boundary       - 30 sec */
#define RTCRST_SECCR_40         (4)             /* Round Boundary       - 40 sec */
#define RTCRST_SECCR_50         (5)             /* Round Boundary       - 50 sec */
#define RTCRST_SECCR_ROUNDOFF   (0)             /* Just resets the seconds. */

/* Tick Time count register */
#define RTCALM_GLOBAL           (1 << 6) /* Global alarm enable */
#define RTCALM_YEAR             (1 << 5) /* Year alarm enable */
#define RTCALM_MON              (1 << 4) /* Month alarm enable */
#define RTCALM_DAY              (1 << 3) /* Day alarm enable */
#define RTCALM_HOUR             (1 << 2) /* Hour alarm enable */
#define RTCALM_MIN              (1 << 1) /* Minute alarm enable */
#define RTCALM_SEC              (1 << 0) /* Second alarm enable */
#define RTCALM_EN               (RTCALM_GLOBAL | RTCALM_YEAR | RTCALM_MON |\
				RTCALM_DAY | RTCALM_HOUR | RTCALM_MIN |\
				RTCALM_SEC)
#define RTCALM_DIS              (~RTCALM_EN)

#endif


