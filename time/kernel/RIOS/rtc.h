/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rtc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:12:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:11:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:48  root]
 * 
 * Revision 1.1.2.2  1992/12/30  20:41:00  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:09:30  htf]
 * 
 * Revision 1.1  1992/01/19  15:33:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
 * ALL RIGHTS RESERVED
*/
/* @(#)rtc.h	1.25  R2/inc/sys,3.1,9030ct 7/5/90 14:25:56 */
#ifndef	_H_RTC
#define	_H_RTC

/*
 * COMPONENT_NAME: SYSPROC
 *
 * ORIGIN: 27
 *
 * IBM CONFIDENTIAL -- (IBM Confidential Restricted when
 * combined with the aggregated modules for this product)
 * OBJECT CODE ONLY SOURCE MATERIALS
 * (C) COPYRIGHT International Business Machines Corp. 1989
 * Unpublished Work
 * All Rights Reserved
 * Licensed Material - Property of IBM
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */                                                                   

#include <sys/adspace.h> /* for the io_att() & io_det() macros          */
#include <sys/seg.h>	/* For the I/O segment register definition	*/
#include <sys/types.h>	/* Always need this one 			*/
#include <sys/time.h>

/* Platform-specific time constants  */
     
#define DEC		(100000000)	/* counter decrements per second */
#define NS_PER_DEC	(NS_PER_SEC/DEC)   /* nanoseconds per clock tick */

/*
 * National Semiconductor Corporation DP8570A Timer Clock Peripheral (TCP)
 *
 * Functions:  1. Time of day.
 *             2. Interval timer.
 *             3. Non-volatile RAM.
 *
 * I/O Bus Access: 1000 0010 0000 1010 0000 0000 1110 0000
 *                 (0x820C00E0)
 *
 *   Hex address range: 40 00 C0 -> 40 00 DF
 *
 *   I/O Offset    RTC Address  Meaning
 *
 *   00 40 00 C0       00       Main Status Register
 *                                  RS = 0               RS = 1
 *   00 40 00 C1       01       Timer 0 Control   -- Real Time Mode Reg
 *   00 40 00 C2       02       Timer 1 Control   -- Output Mode Reg
 *   00 40 00 C3       03       Periodic Flag Reg -- Intr Control R0
 *   00 40 00 C4       04       Intr Routing Reg  -- Intr Control R1
 *
 *   00 40 00 C5       05       1/100 Second Counter
 *   00 40 00 C6       06       Seconds Clock Counter
 *   00 40 00 C7       07       Minutes Clock Counter
 *   00 40 00 C8       08       Hours Clock Counter
 *   00 40 00 C9       09       Day of Month Clock Counter
 *   00 40 00 CA       0A       Months Clock Counter
 *   00 40 00 CB       0B       Years Clock Counter
 *   00 40 00 CC       0C       Units Julian Clock Counter
 *   00 40 00 CD       0D       100's Julian Clock Counter
 *   00 40 00 CE       0E       Day of Week Clock Counter
 *
 *   00 40 00 CF       0F       Timer 0 LSB
 *   00 40 00 D0       10       Timer 0 MSB
 *   00 40 00 D1       11       Timer 1 LSB
 *   00 40 00 D2       12       Timer 1 MSB
 *
 *   00 40 00 D3       13       Seconds Compare RAM
 *   00 40 00 D4       14       Minutes Compare RAM
 *   00 40 00 D5       15       Hours Compare RAM
 *   00 40 00 D6       16       Day of Month Compare RAM
 *   00 40 00 D7       17       Months Compare RAM
 *   00 40 00 D8       18       Day of Week Compare RAM
 *
 *   00 40 00 D9       19       Seconds Time Save RAM
 *   00 40 00 DA       1A       Minutes Time Save RAM
 *   00 40 00 DB       1B       Hours Time Save RAM
 *   00 40 00 DC       1C       Day of Month Time Save RAM
 *   00 40 00 DD       1D       Months Time Save RAM
 *
 *   00 40 00 DE       1E       RAM
 *   00 40 00 DF       1F       RAM/TEST Register
 */

/*
 * Access to the real time clock is to be via machine dependent
 * macro definitions.  An assumption is made in this abstraction that
 * the IOSEG will be used to access the IOCC thus allowing
 * the address of the rtc to be placed in a define. 
 * All the macros that manipulate access to the IOCC for the rtc will
 * loader the IOSEG with SYSREG_SEGVAL.
 */
#define RTC_OFFSET     0x004000c0	/* offset to real time clock - TCP */
#define RTC_ADDR 0xf0000000 | RTC_OFFSET /* address of the real time clock */
#define SYSREG_SEGVAL  0x820c00e0	/* segm valu to access iocc systm regs*/

struct rs0   /* register select 0 structure definition */
{ 
        uchar  _0_timer0;
        uchar  _0_timer1;
        uchar  _0_periodic;
        uchar  _0_intr_route;
};

struct rs1   /* register select 1 structure definition */
{
        uchar  _1_rtm_reg;
        uchar  _1_output_mode;
        uchar  _1_intr_r0;
        uchar  _1_intr_r1;
};

struct  tcp  
{
        union reg_sel
        {
                struct rs0 RS0; /* register select 0 */
                struct rs1 RS1; /* register select 1 */
	} rs;

        /*
	 * Time of day registers, and TOD comparator registers
	 */
        uchar   _rtc_ms;        	/* Milliseconds		*/
	uchar	_rtc_sec;		/* Seconds		*/
	uchar	_rtc_min;		/* Minutes		*/
	uchar	_rtc_hrs;		/* Hours		*/
	uchar	_rtc_dom;		/* Day of the month	*/
	uchar	_rtc_mon;		/* Months		*/
	uchar	_rtc_yrs;		/* Years		*/
	uchar	_rtc_jul_dig;		/* Julian day 1-99	*/
	uchar	_rtc_jul_100;		/* Julian day 1,2,3	*/
	uchar	_rtc_dow;		/* Day of the week	*/

	/*
	 * Timer Status Blocks
	 */
	uchar	_rtc_t0lsb;
	uchar	_rtc_t0msb;
	uchar	_rtc_t1lsb;
	uchar	_rtc_t1msb;

	/*
	 * Comparator RAM
	 */
	uchar	_rtc_sec_comp;
	uchar	_rtc_min_comp;
	uchar	_rtc_hour_comp;
	uchar	_rtc_dom_comp;
	uchar	_rtc_month_comp;
	uchar	_rtc_dow_comp;

	/*
	 * Time Save RAM
	 */
	uchar	_rtc_secs_tsave;
	uchar	_rtc_min_tsave;
	uchar	_rtc_hour_tsave;
	uchar	_rtc_dom_tsave;
	uchar	_rtc_month_tsave;

        /*
         * Additional Bytes
         */
         uchar   _rtc_ram;
         uchar   _rtc_ramtest;
};

volatile struct rtc  {	
        uchar   msr;       /* Main status register */
        union   uniontcp  
        {
                uchar   tcp_ram[31];
                struct  tcp tcp;
	} uniontcp;
};

volatile struct rtc *rtc;

#define  timer0		uniontcp.tcp.rs.RS0._0_timer0
#define  timer1		uniontcp.tcp.rs.RS0._0_timer1
#define  periodic	uniontcp.tcp.rs.RS0._0_periodic
#define  intr_route	uniontcp.tcp.rs.RS0._0_intr_route

#define  rtm		uniontcp.tcp.rs.RS1._1_rtm_reg
#define  output_mode	uniontcp.tcp.rs.RS1._1_output_mode
#define  intr_r0	uniontcp.tcp.rs.RS1._1_intr_r0
#define  intr_r1	uniontcp.tcp.rs.RS1._1_intr_r1

/*
 *  Timer save defines
 */
#define	ts_seconds	uniontcp.tcp._rtc_secs_tsave
#define	ts_minutes	uniontcp.tcp._rtc_min_tsave
#define	ts_hours	uniontcp.tcp._rtc_hour_tsave
#define	ts_date_month	uniontcp.tcp._rtc_dom_tsave
#define	ts_months	uniontcp.tcp._rtc_month_tsave

/*
 *  Clock defines
 */
#define	c_millisecs	uniontcp.tcp._rtc_ms
#define	c_seconds	uniontcp.tcp._rtc_sec
#define	c_minutes	uniontcp.tcp._rtc_min
#define	c_hours		uniontcp.tcp._rtc_hrs
#define	c_date_month	uniontcp.tcp._rtc_dom
#define	c_months	uniontcp.tcp._rtc_mon
#define	c_years		uniontcp.tcp._rtc_yrs
#define	c_julian_100	uniontcp.tcp._rtc_jul_100
#define	c_day_week	uniontcp.tcp._rtc_dow
#define	c_julian_digits	uniontcp.tcp._rtc_jul_dig

#define  T0LSB		uniontcp.tcp._rtc_t0lsb
#define  T0MSB		uniontcp.tcp._rtc_t0msb
#define  T1LSB		uniontcp.tcp._rtc_t1lsb
#define  T1MSB		uniontcp.tcp._rtc_t1msb

#define  comp_secs      uniontcp.tcp._rtc_sec_comp
#define  comp_min       uniontcp.tcp._rtc_min_comp
#define  comp_hour      uniontcp.tcp._rtc_hour_comp
#define  comp_dom       uniontcp.tcp._rtc_dom_comp
#define  comp_month     uniontcp.tcp._rtc_month_comp
#define  comp_dow       uniontcp.tcp._rtc_dow_comp

#define  p0_ram		uniontcp.tcp._rtc_ram
#define  p0_ramtest	uniontcp.tcp._rtc_ramtest

/*
 * Main Status Register:
 *
 * 1xxx xxxx - (PS)  Page Select Bit (RW, 0 = see above, 1 = 31 bytes RAM)
 * x1xx xxxx - (RS)  Register Select Bit 
 * xx1x xxxx - (T1)  Timer 1 Interrupt
 * xxx1 xxxx - (T0)  Timer 0 Interrupt
 * xxxx 1xxx - (AL)  Alarm Interrupt
 * xxxx x1xx - (PER) Period Interrupt 
 * xxxx xx1x - (PF)  Power Fail Interrupt (reset when power restored)
 * xxxx xxx1 - (INT) Interrupt Status (RO, 1 = interrupt pending)
 */
#define MSR_PS	0x80
#define MSR_RS	0x40
#define MSR_T1	0x20
#define MSR_T0	0x10
#define MSR_AL	0x08
#define MSR_PER	0x04
#define MSR_PF	0x02
#define MSR_INT	0x01

/* Timer 0 & 1 Control Register:
 *    PS = 0, RS = 0
 * 1xxx xxxx - (CHG) - Count/Hold Gate
 *                        Modes 0-2: suspend the timer (prescaler not reset)
 *                        Mode 3:    (re)trigger the count sequence
 * x1xx xxxx - (RD) -  Timer Read (used for latched reads of timer)
 * xx1x xxxx - (C2) -  Input Clock Select
 * xxx1 xxxx - (C1) -  Input Clock Select
 * xxxx 1xxx - (C0) -  Input Clock Select
 * xxxx x1xx - (M1) -  Mode Select
 * xxxx xx1x - (M0) -  Mode Select
 * xxxx xxx1 - (TSS) - Timer Start/Stop (1 = start, 0 = stop)
 */
#define TCR_CHG	0x80
#define TCR_RD	0x40
#define TCR_C2	0x20
#define TCR_C1	0x10
#define TCR_C0	0x08
#define TCR_M1	0x04
#define TCR_M0	0x02
#define TCR_TSS	0x01

/*
 * Periodic Flag Register
 *    PS = 0, RS = 0
 * 1xxx xxxx - (TM) -   Test Mode Enabled
 *                         enables test mode register at offset 1F
 * x1xx xxxx - (OSF) -  Oscillator Failed/Battery
 *                         1 - oscillator failure has occurred
 *                         automatically set on power-up
 *                         reset by setting CSS in Real Time Mode Reg
 * xx1x xxxx - (1ms) -  millisec flag   \
 * xxx1 xxxx - (10ms) - 10 millisec flag \
 * xxxx 1xxx - (hm) -   100 millisec flag \
 * xxxx x1xx - (1s) -   seconds flag      /    Rollover bits 
 * xxxx xx1x - (10s) -  10 second flag   /
 * xxxx xxx1 - (1min) - minutes flag    /
 */
#define PFR_TM  	0x80
#define PFR_OSF 	0x40
#define PFR_MS  	0x20
#define PFR_TENMS	0x10
#define PFR_HUNMS	0x08
#define PFR_SEC 	0x04
#define PFR_TENSEC	0x02
#define PFR_MIN	        0x01

/*
 * Interrupt Routing Register:
 *    PS = 0, RS = 0
 * 1xxx xxxx - (TS) -  Time Save Enable 
 *                        1 - Time Save RAM follow clock registers
 *                        0 - Freeze values in Time Save RAM
 * x1xx xxxx - (LB) -  Low Battery Flag
 *   For the following bits:  1 = interrupt signal to MFO pin
 *                            0 = interrupt signal to INTR pin
 * xx1x xxxx - (PFD) - PF Delay Enable 
 * xxx1 xxxx - (T1R) - Timer 1 Route
 * xxxx 1xxx - (T0R) - TImer 0 Route
 * xxxx x1xx - (ALR) - Alarm Route
 * xxxx xx1x - (PRR) - Periodic Route
 * xxxx xxx1 - (PFR) - Power Fail Route
 */
#define IRR_TS	0x80
#define IRR_LB	0x40
#define IRR_PFD	0x20
#define IRR_T1R	0x10
#define IRR_T0R 0x08
#define IRR_ALR 0x04
#define IRR_PRR	0x02
#define IRR_PFR	0x01

/*
 * Real Time Mode Register
 *    PS = 0, RS = 1
 * 1xxx xxxx - (XT1) - Crystal Frequency MSB \ 0 0 - 32.768KHz
 * x1xx xxxx - (XT0) - Crystal Frequency LSB / 0 1 - 4.194304MHz
 *                                             1 0 - 4.9152MHz
 *                                             1 1 - 32.000KHz
 * xx1x xxxx - (TPF) - Timer PF Operation
 * xxx1 xxxx - (IPF) - Interrupt PF Operation 
 * xxxx 1xxx - (CSS) - Clock Start/Stop (Master start/stop) 
 *                        1 = prescaler and counter enabled
 * xxxx x1xx - (12H) - 12/24 Hour Mode (1 = 12 hour mode)
 * xxxx xx1x - (LY1) - Leap Year MSB \ 0 0 = Leap year current year
 * xxxx xxx1 - (LY0) - Leap Year LSB / 0 1 = Leap year last year
 *                                     1 0 = Leap year 2 years ago
 *                                     1 1 = Leap year 3 years ago
 */
#define RTM_XT1	0x80
#define RTM_XT0	0x40
#define RTM_TPF	0x20
#define RTM_IPF	0x10
#define RTM_CSS 0x08
#define RTM_12H 0x04
#define RTM_LY1	0x02
#define RTM_LY0	0x01

/*
 * Output Mode Register:
 *    PS = 0, RS = 1
 * 1xxx xxxx - (M0) - MFO Pin as Oscillator
 * x1xx xxxx - (M1) - MFO Pin as Timer 1
 *                            1     /   0
 * xx1x xxxx - (MP) - MFO Push-pull /  open drain
 * xxx1 xxxx - (MM) - MFO Active hi /  low
 * xxxx 1xxx - (IP) - INTR Push-pull/  open drain
 * xxxx x1xx - (IH) - INTR Active hi/  low
 * xxxx xx1x - (TP) - T1 Push-pull  /  open drain
 * xxxx xxx1 - (TH) - T1 Active hi  /  low
 */
#define OMR_MO	0x80
#define OMR_MT	0x40
#define OMR_MP	0x20
#define OMR_MM	0x10
#define OMR_IP 0x08
#define OMR_IH 0x04
#define OMR_TP	0x02
#define OMR_TH	0x01

/*
 * Interrupt Control Register 0
 *    PS = 0, RS = 1
 * 1xxx xxxx - (T1) - Timer 1 Enable
 * x1xx xxxx - (T0) - Timer 0 Enable
 * xx1x xxxx - (1m) - millisec enable
 * xxx1 xxxx - (tm) - 10 millisec enable
 * xxxx 1xxx - (hm) - 100 millisec enable
 * xxxx x1xx - (S) -  seconds enable
 * xxxx xx1x - (TS) - 10 second enable
 * xxxx xxx1 - (MN) - minutes enable
 */
#define IC0_T1  	0x80
#define IC0_T0  	0x40
#define IC0_MS  	0x20
#define IC0_TENMS	0x10
#define IC0_HUNMS       0x08
#define IC0_SEC         0x04
#define IC0_TENSEC   	0x02
#define IC0_MIN 	0x01

/*
 * Interrupt Control Register 1:
 *    PS = 0, RS = 1
 * 1xxx xxxx - (PFe) - Power Fail Interrupt Enable
 * x1xx xxxx - (ALe) - Alarm Interrupt Enable
 * xx1x xxxx - (DOW) - Day of Week enable
 * xxx1 xxxx - (MO) -  Month Compare enable
 * xxxx 1xxx - (DOM) - Day of Month enable
 * xxxx x1xx - (HR) -  Hour Compare enable
 * xxxx xx1x - (MN) -  Minute Compare enable
 * xxxx xxx1 - (SC) -  Second Compare enable
 */
#define IC1_PFE 	0x80
#define IC1_ALE 	0x40
#define IC1_DOW 	0x20
#define IC1_MO_COM  	0x10
#define IC1_DOM_COM     0x08
#define IC1_HR_COM      0x04
#define IC1_MN_COM  	0x02
#define IC1_SC_COM  	0x01


/*
 *  Time conversion macros:
 *
 *  NS_TO_TICKS -
 *    Input parameters:
 *	int	x;	#Number of nanoseconds to convert to ticks
 *    Returns:
 *	int		#The number of ticks corresponding to the
 *			#number of nanoseconds input
 *
 *  TICKS_TO_TIME -
 *    Input parameters:
 *	struct	timestruct_t	rvp;	#POSIX time structure containing
 *					#fields for seconds and nanoseconds
 *	int			ticks;	#The number of ticks to convert to a
 *					#time value.
 *    Returns:
 *			#The rvp parameter is filled in with the time
 *			#equivalent of the ticks parameter
 *
 *  DECS_TO_TIME -
 *    Input parameters:
 *	struct	timestruct_t	rvp;	#POSIX time structure containing
 *					#fields for seconds and nanoseconds
 *	int			decs;	#The number of clock decrements to
 *					#convert to a time value.
 *    Returns:
 *			#The rvp parameter is filled in with the time
 *			#equivalent of the decs parameter
 *
 *  TS_TO_uS -
 *    Input parameters:
 *	struct	timestruct_t	ts;	#POSIX time structure containing
 *					#fields for seconds and nanoseconds
 *	unsigned int		us;	#The number of microseconds represented
 *					#by the ts parameter
 *    Returns:
 *			#The us parameter is filled in with the microsecond
 *			#equivalent of the ts parameter
 */
#define NS_TO_TICKS(x)	(((x) + (NS_PER_TICK - 1)) / NS_PER_TICK)

#define	TICKS_TO_TIME(rvp, ticks)				\
{								\
	int	copy;						\
								\
	copy = ticks;						\
	(rvp).tv_sec = (rvp).tv_nsec = 0;			\
	while(copy >= HZ)  {					\
		(rvp).tv_sec++;					\
		copy -= HZ;					\
	}							\
	(rvp).tv_nsec = (int)(copy * NS_PER_TICK);		\
}

#define	DECS_TO_TIME(rvp, decs)					\
{								\
	uint	copy;						\
								\
	copy = decs;						\
	(rvp).tv_sec = (rvp).tv_nsec = 0;			\
	while(copy >= DEC)  {					\
		(rvp).tv_sec++;					\
		copy -= DEC;					\
	}							\
	(rvp).tv_nsec = (int)(copy * NS_PER_DEC);		\
}

#define	TS_TO_uS(ts, us)						\
{									\
	if((ts).tv_sec > MAX_SECS_TO_uS || ((ts).tv_sec == MAX_SECS_TO_uS   \
		&& (ts).tv_nsec > MAX_NS_TO_uS))  {			\
		us = MAX_SECS_TO_uS * uS_PER_SECOND + 			\
		     (MAX_NS_TO_uS / NS_PER_uS) + 1;			\
	}								\
	else  {								\
		us = (ts).tv_sec * uS_PER_SECOND;			\
		us += (ts).tv_nsec / NS_PER_uS;				\
	}								\
}

/*
 * The following structure is used to communicate with a pair of
 * routines which convert time of day between "seconds-since-genesis"
 * format and mm/dd/yy hh:mm:ss format.
 */
struct  tms {
	int	secs;			/* Seconds since genesis	*/
        uchar   ms;			/* Milliseconds			*/
	uchar	no_secs;		/* Seconds			*/
	uchar	mins;			/* Minutes			*/
	uchar	hrs;			/* Hours			*/
	uchar	dom;			/* Day of the month		*/
	uchar	mths;			/* Months			*/
	uchar	yrs;			/* Years			*/
	uchar	jul_dig;		/* Julian day 1-99		*/
	uchar	jul_100;		/* Julian day 1,2,3		*/
	uchar	dow;			/* Day of the week		*/
};

extern struct timestruc_t tod;	/* memory mapped timerstructure		*/
extern struct tms	  tm;	/* date to secs conversion structure	*/
extern time_t time;		/* memory mapped time - secs since epoch*/
extern int    ticks_its;	/* number of ticks in the current second*/
extern time_t lbolt;		/* number 1/60 seconds since last boot	*/

    
#define	UPDATE_RTC(ts)	/* update real time clock register */		\
{									\
	mtrtcl(0);							\
	mtrtcu((ts).tv_sec);						\
	mtrtcl((ts).tv_nsec);						\
}

#define WRITE_CLOCK(unused) /* set memory mapped time and processor clock */ \
{									\
	register long	segval;						\
									\
	time = tod.tv_sec;						\
	segval = (long)io_att(SYSREG_SEGVAL, RTC_OFFSET);               \
	rtc->msr &= ~MSR_PS;						\
	rtc->msr |= MSR_RS;						\
	rtc->rtm &= ~RTM_CSS;		/* Do not alter leap year bits */\
	rtc->c_years = tm.yrs;						\
	switch(tm.yrs % 4)  {						\
		case 0:							\
			rtc->rtm &= (~(RTM_LY0 | RTM_LY1));		\
			break;						\
		case 1:							\
			rtc->rtm = (rtc->rtm & ~RTM_LY1) | RTM_LY0;	\
			break;						\
		case 2:							\
			rtc->rtm = (rtc->rtm & ~RTM_LY0) | RTM_LY1;	\
			break;						\
		case 3:							\
			rtc->rtm |= RTM_LY1 | RTM_LY0;			\
			break;						\
		default:						\
			assert(0);					\
	}								\
	rtc->c_months = tm.mths;					\
	rtc->c_date_month = tm.dom;					\
	rtc->c_hours = tm.hrs;						\
	rtc->c_minutes = tm.mins;					\
	rtc->c_seconds = tm.no_secs;					\
	rtc->c_julian_100 = tm.jul_100;					\
	rtc->c_julian_digits = tm.jul_dig;				\
	rtc->rtm |= RTM_CSS;		/* Do not alter leap year bits */\
	io_det(segval);       					\
} 

#define	SET_TIME()  {							\
	WRITE_CLOCK(tod);						\
	UPDATE_RTC(tod);						\
}
  
#define SET_COUNTER(val)	mtdec(val)
#define READ_COUNTER()		mfdec()

/* Nothing to do to initialize the decrementer */
#define	INIT_DEC()

#define	dec_imm()		mtdec(1)

#define	dec_set(tvp)							\
	{								\
		if(((tvp).tv_sec > MAX_DEC_SECS) || 			\
		   (((tvp).tv_sec == MAX_DEC_SECS) &&			\
		    ((tvp).tv_nsec > MAX_DEC_NS)))  {			\
			mtdec((MAX_DEC_SECS * NS_PER_SEC + MAX_DEC_NS));\
		}							\
		else  {							\
			mtdec((ulong)(((tvp).tv_sec * NS_PER_SEC) +	\
			      (tvp).tv_nsec));				\
		}							\
	}

/*
 *  Time constants
 */
#define	MAXTODSEC	0xFFFFFFFF	/* 1 zillion decimal seconds	*/
#define	MAXTODNSEC	0x3B9AC9FF      /* 999,999,999 dec nanoseconds	*/

/*
 *  Determine the smaller resolution of the clock.  This will be accomplished
 *  by reading a value (value A) from the clock and then continuously reading
 *  value B from the clock until value B is different than value A.  The
 *  difference of the two values is then the smallest resolution of the clock.
 */
#define	MIN_NS(ns)  {							\
	register int	mnipri;						\
	struct itimerstruc_t	tsA, tsB, tsC;				\
									\
	mnipri = i_disable(INTMAX);					\
	curtime(&(tsA.it_value));					\
	do  {								\
		curtime(&(tsB.it_value));				\
	}  while(!(ntimercmp(tsA.it_value, tsB.it_value, !=)));		\
	ntimersub(tsB.it_value, tsA.it_value, tsC.it_value);		\
	ns = tsC.it_value.tv_nsec;					\
	i_enable(mnipri);						\
}

#define	update_time()  {			\
	curtime(&tod);				\
	time = tm.secs = tod.tv_sec;		\
}

#define	ADJ_TIME(cts, dts)  {			\
	update_time();				\
	(cts).tv_sec = tod.tv_sec;		\
	(cts).tv_nsec = tod.tv_nsec;		\
}

#define	NEW_TIME(unused)

#endif	/* _H_RTC */

