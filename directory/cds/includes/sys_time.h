/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sys_time.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:35  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:18:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:52  root]
 * 
 * Revision 1.1.4.6  1994/08/03  19:02:46  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:12  mccann]
 * 
 * Revision 1.1.4.5  1994/06/09  18:39:37  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:18  devsrc]
 * 
 * Revision 1.1.4.4  1994/05/12  21:11:57  peckham
 * 	Eliminate unnecessary conditionals.
 * 	Add {INIT,EVEN,ODD}_time_quad() macros.
 * 	[1994/05/12  19:16:47  peckham]
 * 
 * Revision 1.1.4.3  1994/04/28  17:19:25  ohara
 * 	change _SYS_TIME_H to CDS_SYS_TIME_H since _SYS_TIME_H is defined in
 * 	/usr/include/sys/time.h on SVR4
 * 	[1994/04/25  20:02:09  ohara]
 * 
 * Revision 1.1.4.2  1994/04/07  10:22:26  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:18:12  kevins]
 * 
 * Revision 1.1.4.1  1994/03/12  22:04:28  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:20:05  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:34:46  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:54:41  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef CDS_SYS_TIME_H
#define CDS_SYS_TIME_H
/*
 * Module sys_time.h
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 *
 */
#include <pthread.h>
#include <dce/dns_record.h>
#include <sys/time.h>

#if !defined(CMA_PX)
# include <dce/utc.h>
# define CMA_PX	/* avoid conflict between utc.h and cma_px.h */
#endif
#if defined(__osf__)
# if !defined(_TIMESPEC_T_)
#  define _TIMESPEC_T_
# endif
#endif

/*
 * UTC definition for time
 *
 *  Test the size of an unsigned long it. If its 64 bits, then define
 *  time_quad_t to be unsigned long int (64 bits) and redefine any of
 *  the associated macros that are affected.
 *
 */

#if LONG_BIT == 64
typedef unsigned long int time_quad_t;
#else
typedef struct time_quad	/* typedef for 64 bit time	*/
	{
	unsigned int tq_lo;	/* low-order longword		*/
	unsigned int tq_hi;	/* and high			*/
	} time_quad_t;
#endif

/*
 *  NOTE : qarith.h requires the 64 bit structure to be defined before it is 
 *         included so that prototyping can be achieved
 */
#include <dce/qarith.h>

#if LONG_BIT == 64

#define INIT_time_quad(a,b) ((((unsigned long)a)<<32) + (b))

#define COPY_time_quad(from_p,to_p) *(to_p) = *(from_p)
#define COPY_time_quad_to_hyper(from_p,to_p) *(to_p) = *(from_p)

/* tests */

#define	NEG_time_quad(a) (*(a) & 0x8000000000000000)
#define	ZERO_time_quad(a) (*(a) == 0)
#define	GT_time_quad(a,b) (*(a) >  *(b))
#define	GE_time_quad(a,b) (*(a) >= *(b))
#define	EQ_time_quad(a,b) (*(a) == *(b))
#define	LE_time_quad(a,b) (*(a) <= *(b))
#define	LT_time_quad(a,b) (*(a) <  *(b))
#define	NE_time_quad(a,b) (*(a) != *(b))

/* t3 = t1 + t2 */
#define	ADD_time_quad(t1,t2,t3) { *(t3) = *(t1) + *(t2); }

/* t3 = t1 - t2 */
#define	SUB_time_quad(t1,t2,t3) { *(t3) = *(t1) - *(t2); }

/* t1 = l2 * l3 + l4 */
#define	DIV_time_quad(t1,l2,l3,l4) \
    { *(l3) = *(l2) / *(t1); *(l4) = *(l2) % *(t1); }

#define EVEN_time_quad(p) p = (++p & ~1L)
#define ODD_time_quad(p) p |= 1L

#else

#define INIT_time_quad(a,b) {b, a}

#define COPY_time_quad(from_p,to_p) COPY_bytes(from_p,to_p,sizeof(time_quad_t))
#define COPY_time_quad_to_hyper(from_p,to_p) \
	{ (to_p)->low = (from_p)->tq_lo; (to_p)->high = (from_p)->tq_hi; }

/* tests */

#define	NEG_time_quad(a) ((a)->tq_hi & 0x80000000)
#define	ZERO_time_quad(a) (((a)->tq_hi == 0) && ((a)->tq_lo == 0))
#define	GT_time_quad(a,b) (((a)->tq_hi > (b)->tq_hi) || \
    (((a)->tq_hi == (b)->tq_hi) && ((a)->tq_lo > (b)->tq_lo)))
#define	GE_time_quad(a,b) (((a)->tq_hi >= (b)->tq_hi) || \
    (((a)->tq_hi == (b)->tq_hi) && ((a)->tq_lo > (b)->tq_lo)))
#define	EQ_time_quad(a,b) \
    (((a)->tq_hi == (b)->tq_hi) && ((a)->tq_lo == (b)->tq_lo))
#define	LE_time_quad(a,b) (((a)->tq_hi < (b)->tq_hi) || \
    (((a)->tq_hi == (b)->tq_hi) && ((a)->tq_lo <= (b)->tq_lo)))
#define	LT_time_quad(a,b) (((a)->tq_hi < (b)->tq_hi) || \
    (((a)->tq_hi == (b)->tq_hi) && ((a)->tq_lo < (b)->tq_lo)))
#define	NE_time_quad(a,b) \
    (((a)->tq_hi != (b)->tq_hi) || ((a)->tq_lo != (b)->tq_lo))

/* t3 = t1 + t2 */
#define	ADD_time_quad(t1,t2,t3) \
    dns_qadd(t1, t2, t3)

/* t3 = t1 - t2 */
#define	SUB_time_quad(t1,t2,t3) \
    dns_qsub(t1, t2, t3)

/* t1 = l2 * l3 + l4 */
#define	DIV_time_quad(t1,l2,l3,l4) \
    dns_ediv((int *)t1,(time_quad_t *)l2,(int *)l3,(int *)l4)

#define EVEN_time_quad(p) p.tq_lo &= ~1
#define ODD_time_quad(p) p.tq_lo |= 1

#endif


/*
 * for pthreads, time_local_t becomes timespec
 */
typedef struct timespec time_local_t;

#define	ZERO_time_local(a) (((a)->tv_sec == 0) && ((a)->tv_nsec == 0))

#define	GT_time_local(a,b) (((a)->tv_sec > (b)->tv_sec) || \
    (((a)->tv_sec == (b)->tv_sec) && ((a)->tv_nsec > (b)->tv_nsec)))
#define	GE_time_local(a,b) (((a)->tv_sec >= (b)->tv_sec) || \
    (((a)->tv_sec == (b)->tv_sec) && ((a)->tv_nsec > (b)->tv_nsec)))
#define	EQ_time_local(a,b) \
    (((a)->tv_sec == (b)->tv_sec) && ((a)->tv_nsec == (b)->tv_nsec))
#define	LE_time_local(a,b) (((a)->tv_sec < (b)->tv_sec) || \
    (((a)->tv_sec == (b)->tv_sec) && ((a)->tv_nsec <= (b)->tv_nsec)))
#define	LT_time_local(a,b) (((a)->tv_sec < (b)->tv_sec) || \
    (((a)->tv_sec == (b)->tv_sec) && ((a)->tv_nsec < (b)->tv_nsec)))
#define	NE_time_local(a,b) \
    (((a)->tv_sec != (b)->tv_sec) || ((a)->tv_nsec != (b)->tv_nsec))

/* HACK until people use sys_time_add() */
#define	SUB_time_local(t1,t2,t3) sys_time_add(t3, t1, t2)

void
sys_timezone_to_utc (
    time_quad_t		*);

void
sys_time_from_interval (
    time_local_t	*,
    int 		,
    int 		,
    int 		);

void
sys_time_from_utc (
    time_local_t	*,
    time_quad_t		*);

void
sys_time_to_utc (
    time_local_t	*,
    time_quad_t		*);

void
sys_time_to_absolute (
    time_local_t	*,
    time_local_t	*);

void
sys_time_add (
    time_local_t	*,
    time_local_t	*,
    time_local_t	*);

void
sys_time_to_timespec (
    time_local_t	*,
    struct timespec	*);

void
sys_time_to_string (
    time_local_t	*,
    char		*,
    int			);

void
sys_utc_to_string (
    time_quad_t		*,
    char		*,
    int			);

void
dtos (
    double		,
    char		*);

#endif  /* #ifndef CDS_SYS_TIME_H */
