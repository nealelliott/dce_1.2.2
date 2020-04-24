/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: catime.c,v $
 * Revision 1.1.7.2  1996/02/18  19:24:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:23  marty]
 *
 * Revision 1.1.7.1  1995/12/08  00:11:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:44  root]
 * 
 * Revision 1.1.4.6  1994/06/23  18:28:35  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:46:31  mccann]
 * 
 * Revision 1.1.4.5  1994/06/09  16:07:18  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:53  devsrc]
 * 
 * Revision 1.1.4.4  1994/05/25  22:22:22  peckham
 * 	Avoid doing quad arithmetic on ts_time.
 * 	It is not aligned!!!
 * 	[1994/05/25  22:18:00  peckham]
 * 
 * Revision 1.1.4.3  1994/05/12  21:10:42  peckham
 * 	Use {INIT,EVEN,ODD}_time_quad() macros.
 * 	[1994/05/12  19:13:03  peckham]
 * 
 * Revision 1.1.4.2  1994/04/14  14:38:09  peckham
 * 	Remove VMS and UNIX conditionals.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Use CA_snapshot_cache() instead of CA_abort().
 * 	[1994/04/11  18:08:33  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:56:04  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:48:57  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:37:36  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:21:02  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:24:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module catime.c
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
 */
#include <cache.h>
#include <sys/time.h>
#include <dce/utc.h>
#include <sys_time.h>

/*
 * For tokens, we allocate a block of times and use them up internally.
 */
#define MAX_TICKS 500

static time_quad_t one = INIT_time_quad(0 ,1);
static time_quad_t two = INIT_time_quad(0, 1<<1);
static time_quad_t token_block = INIT_time_quad(0, MAX_TICKS<<1);

/*
 * Check if data has been cached longer then requested 
 */
int 
CAStale (unsigned int  seconds,
         utc_t         *InCacheTime_p)
{
    utc_t current, relative, newtime;
    timespec_t inacc;
    reltimespec_t reltime;
    enum utc_cmptype oper;

    /* if 0, don't check for staleness */
    if (seconds == 0)
	return(0);

    /* Set up relative time to be subtracted from current time */
    reltime.tv_sec = seconds;
    reltime.tv_nsec = 0;

    inacc.tv_sec= 0;
    inacc.tv_nsec = 0;

    (void)utc_mkbinreltime(&relative, &reltime, &inacc);
    (void)utc_gettime(&current);
    (void)utc_subtime(&newtime, &current, &relative);
    (void)utc_cmpmidtime(&oper, &newtime, InCacheTime_p);

    if (oper == utc_greaterThan)
	return(1);		/* Stale  */
  
    return(0);      
}

/*
 * Check if a timeout has occured
 */
int 
CATimedOut (void *time_p)
{
    utc_t now;

    /* Get current Time */
    utc_gettime(&now);

    return(LT_utc((byte_t *)time_p, (byte_t *)&now));

}

/*
 * Extend a timeout 
 */
void 
CASetExpiration (void *extend_p,
                 void *expire_p)
{
    utc_t now;

    utc_gettime(&now);
    ADD_utc((byte_t *)extend_p, (byte_t *)&now, (byte_t *)expire_p);

}

/*
 * This routine generates a Timestamp.
 */
dns_status_t 
CACreateTimestamp (Timestamp_t *ts_p)
{
    time_quad_t now;
    dns_status_t status = DNS_SUCCESS;

    /* Get time in UTC */
    sys_time_to_utc(NULL, &now);
    ADD_time_quad(&one, &now, &now);
    EVEN_time_quad(now);

    /* Lock cache header for update */
    CAHeaderLock();
	{
	if (GT_Time(&now, &CAfixed_p->ca_nexttime)) 
	{
	    /* Save newly generated uid */
	    COPY_time_quad(&now, &CAfixed_p->ca_lasttime);
	    COPY_Time(&now, ts_p->ts_time);
	    ADD_time_quad(&now, &two, &CAfixed_p->ca_nexttime);

	    /* Copy node address to Timestamp */
	    COPY_Node(&CAfixed_p->ca_802addr, ts_p->ts_node);
	} 
	else if (GE_Time(&now, &CAfixed_p->ca_lasttime)) 
	{
	    COPY_Time(&CAfixed_p->ca_nexttime, ts_p->ts_time);
	    /*
	     * if new uid is same as last generated uid, increment low order
	     * of the last used uid and use that instead of real time to
	     * ensure uniqueness.  We are running faster than clock ticks
	     * which has potential problems if it continues for very long.
	     */
	    ADD_time_quad(&CAfixed_p->ca_nexttime, &two, 
			  &CAfixed_p->ca_nexttime);

	    /* Copy node address to Timestamp */
	    COPY_Node(&CAfixed_p->ca_802addr, ts_p->ts_node);
	} else
	    status = DNS_BADCLOCK;
	}
    CAHeaderUnlock();

    return(status);
}

/*
 * Token generation must be fast, so allocate blocks of timestamps
 * to avoid the sys_time_to_utc() overhead for each call.
 *
 * Assume client lock to avoid conflict over internal statics.
 */
void 
CA_CreateToken (token_t *tk_p)
{
    static int ticks;
    static time_quad_t last_time;

#ifdef CACHE_PARANOID
  if (!CA_ClientLocked()) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_CLOCK_A_MSG);
    return;
  }
#endif

    if (--ticks <= 0) 
    {
	time_quad_t now;

	/* Get time in UTC */
	sys_time_to_utc(NULL, &now);
	ADD_time_quad(&one, &now, &now);
	EVEN_time_quad(now);

	/* Lock cache header for update */
	CAHeaderLock();
	    {
	    if (GT_Time(&now, &CAfixed_p->ca_nexttime)) 
	    {
		/* Save newly generated uid */
		COPY_time_quad(&now, &CAfixed_p->ca_lasttime);
		COPY_Time(&now, &last_time);
	    } 
	    else if (GE_Time(&now, &CAfixed_p->ca_lasttime)) 
	    {
		COPY_Time(&CAfixed_p->ca_nexttime, &last_time);
	    }
	    ADD_time_quad(&last_time, &token_block, &CAfixed_p->ca_nexttime);
	    }
	CAHeaderUnlock();

	ticks = MAX_TICKS;
    } else
	ADD_time_quad(&last_time, &two, &last_time);

    COPY_time_quad(&last_time, (time_quad_t *) tk_p);
}
