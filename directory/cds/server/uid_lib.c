/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: uid_lib.c,v $
 * Revision 1.1.11.2  1996/02/18  19:37:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:58  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:34:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:31  root]
 * 
 * Revision 1.1.6.8  1994/08/19  21:11:05  zee
 * 	Added ts_set_last_ts routine for BadClock fixer.
 * 	[1994/08/19  18:09:46  zee]
 * 
 * Revision 1.1.6.7  1994/08/16  20:03:57  jd
 * 	11677 From shu@hp. In ts_new get the system time within the safe region to
 * 	prevent lasttime from being updated while waiting for the SERVER_LOCK.
 * 	[1994/08/13  18:32:50  jd]
 * 
 * Revision 1.1.6.6  1994/08/03  19:05:06  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:33:02  mccann]
 * 
 * Revision 1.1.6.5  1994/06/30  19:18:21  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:49:17  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  18:44:17  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:56  devsrc]
 * 
 * Revision 1.1.6.3  1994/05/12  21:12:32  peckham
 * 	Use {INIT,EVEN,ODD}_time_quad() macros.
 * 	[1994/05/12  19:18:53  peckham]
 * 
 * Revision 1.1.6.2  1994/04/14  14:51:37  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:22:42  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:13:49  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:48:40  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:25:21  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:26:11  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/11  16:55:40  peckham
 * 	OT#4541: Make ts_new() thread-safe. Use dns_server_mutex to protect
 * 	the timestamp generator statics.
 * 	[1992/07/06  21:36:07  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module uid_lib.c
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
#include <server.h>
#include <dce/rpc.h>
#include <dce/dce_utils.h>
#include <time.h>
#include <events.h>
#include <uid.h>

extern struct nsgbl_ns dns_nameserver;

/*
 * Global structure containing last uid generated. Need to add ticks to
 * get last uid returned to a caller
 *
 * Protected by dns_server_mutex.
 */
static Node_u theNode;
static time_quad_t lasttime = INIT_time_quad(0, 0);
static time_quad_t nexttime = INIT_time_quad(0, 0);
static time_quad_t two = INIT_time_quad(0, 2);
static volatile int this_node_init = 0;
static dthread_once_t uid_once = dthread_once_init;
static dthread_cond_t uid_available;

/*
 * Initialize this module's statics
 */
static void 
uid_init (void)
{
  int thstatus;

  if ((thstatus = dthread_create_condition(&uid_available)) < 0)
      dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
}

/*
 * Get the 802 address and save it for uid/cts generation
 */
static void 
init_address (void)
{
  int do_init = FALSE;
  int thstatus;
  error_status_t error_st;

  dthread_once(&uid_once, uid_init);

  SERVER_LOCK(dns_server_mutex);

  /* Beginning of the 'protected code' */

  if (this_node_init == 0) {
    do_init = TRUE;
    this_node_init = -1;
  } 
  else 
      while (this_node_init < 0)
	{
	    if ((thstatus = dthread_wait(&(uid_available),
                                       &(dns_server_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
	    }
	}

  /* End of the 'protected code' */

  SERVER_END_LOCK(dns_server_mutex);

  if (do_init) 
  {
      (void)dce_get_802_addr((dce_802_addr_t *)theNode, &error_st);
      this_node_init = 1;

      if ((thstatus = dthread_broadcast(&(uid_available))) < 0)
      {
	dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
      }
  }
}


/*
 * ts_set_last_ts
 *	This routine sets the last dns_last_uid.uid_time.
 *      The current node id is used.
 *      WARNING: DANGER!  This routine exists only to fix BADCLOCK
 *      problems in the database.  Any other use of this routine
 *      is probably incorrect.
 *
 * Input:
 *	ts_p = a ts compare
 *
 */
void 
ts_set_last_ts (Timestamp_t *ts_p)
{
    time_quad_t		now;
    int thstatus;


    /* Get the 802 address if this is the first time through */
    if (this_node_init <= 0)
      init_address();

    SERVER_LOCK(dns_server_mutex)

     /* Beginning of the 'protected code' */

    COPY_time_quad(ts_p->ts_time, &lasttime);
    COPY_Time(ts_p->ts_time, &nexttime);

    /* End of the 'protected code' */

    SERVER_END_LOCK(dns_server_mutex)
}

/*
 * ts_update_last_ts
 *	This routine compares the input value with the current
 *	value of dns_last_uid.uid_time and maximizes it.  This
 *	routine should be called called before any calls to uid_new
 *	if the last_uid generated is important.  This would be
 *	the case for the server.  The value of uid_p->uid_node
 *	is not considered.  The current node id is used.
 *
 * Input:
 *	ts_p = a ts compare
 *
 */
void 
ts_update_last_ts (Timestamp_t *ts_p)
{
    int thstatus;


    /* Get the 802 address if this is the first time through */
    if (this_node_init <= 0)
      init_address();

    SERVER_LOCK(dns_server_mutex);

     /* Beginning of the 'protected code' */

	/* If ts is greater the current value of last_uid, update it */
	if (GT_Time(ts_p->ts_time, &nexttime)) {
	    COPY_time_quad(ts_p->ts_time, &lasttime);
	    COPY_Time(ts_p->ts_time, &nexttime);
	}

    /* End of the 'protected code' */

    SERVER_END_LOCK(dns_server_mutex)
}

/*
 * id_new
 *    generate a uuid_t.  For backwards compatiblily,
 * generate it from a cts instead of calling uid_create
 */
dns_status_t 
id_new (ObjUID_t *id_p)
{

  unsigned32 status;
  uuid_t id;

  uuid_create(&id, &status);
  if (status != uuid_s_ok)
    return(DNS_BADCLOCK);

  COPY_bytes(&id, id_p, sizeof(id));
  return(DNS_SUCCESS);
}

/*
 * ts_new
 *	This routine generates a unique ts.  If time
 *	has gone backwards, an error is returned.  The last_ts value must
 *	already have been initialized as part of startup.
 *
 * Inputs:
 * 		none.
 * Outputs:
 *             ts_p     = Timestamp
 *
 */
dns_status_t 
ts_new (Timestamp_t *ts_p)
{
    time_quad_t		now;
    dns_status_t	status = DNS_SUCCESS;
    int thstatus;



    /* Get the 802 address if this is the first time through */
    if (this_node_init <= 0)
      init_address();

    SERVER_LOCK(dns_server_mutex)
    /* Beginning of the 'protected code' */

        /* Get time in UTC */
        sys_time_to_utc(NULL, &now);
        ODD_time_quad(now);

	if (GT_Time(&now, &nexttime)) {
	    /* Save newly generated ts */
	    COPY_time_quad(&now, &lasttime);
	    COPY_time_quad(&now, &nexttime);
	} else if (GE_Time(&now, &lasttime)) {
	    /*
	     * if new ts is same as last generated ts, increment low order of
	     * the last used ts and use that instead of real time to
	     * ensure uniqueness.  We are running faster than clock ticks
	     * which has potential problems if it continues for very long.
	     */
	    ADD_time_quad(&two, &nexttime, &nexttime);
	} else {
	    dce_svc_printf(CDS_S_SERVER_TIMEBACK_MSG);
	    status = DNS_BADCLOCK;
	}

	/* Copy node address to ts */
	COPY_Node(theNode, ts_p->ts_node);
	COPY_Time(&nexttime, ts_p->ts_time);

    /* End of the 'protected code' */

    SERVER_END_LOCK(dns_server_mutex)

    return(status);
}

/*
 * ts_sanity
 *     Check a ts to ensure it is within the specified skew of the
 *     actual time.
 *
 * Input:
 *     ts_p = address of ts to check
 *     lower_p = address of ts containing lowerbound
 *
 * Value:
 *     True if okay, False otherwise
 */
int 
ts_sanity (Timestamp_t  *ts_p,
           Timestamp_t  *lower_p)
{
    time_quad_t		now;
    time_quad_t		future_time;

    sys_time_to_utc(NULL, &now);
    ADD_time_quad(&now, &dns_nameserver.ns_future_skew, &future_time);
    return(LE_Time(lower_p->ts_time, ts_p->ts_time) &&
		LE_Time(ts_p->ts_time, &future_time));
}

/*
 * ts_future_skew
 *     Check a ts to ensure it is within the specified future skew of the
 *     actual time. (does not check maxupdate_age)
 *
 * Input:
 *     ts_p = address of ts to check
 *
 * Value:
 *     True if okay, False otherwise
 */
int 
ts_future_skew (Timestamp_t *ts_p)
{
    time_quad_t		now;
    time_quad_t		future_time;

    sys_time_to_utc(NULL, &now);
    ADD_time_quad(&now, &dns_nameserver.ns_future_skew, &future_time);
    return(LE_Time(ts_p->ts_time, &future_time));
}

/*
 * ts_to_string
 *	Convert ts to printable string
 *
 * INPUTS:
 *	ts_p - pointer to ts to convert
 *	buf_p - pointer to buffer to receive text
 *	buflen - length of buffer
 *
 * OUTPUTS:
 *
 */
void 
ts_to_string (Timestamp_t  *ts_p,
              char         *buf_p,
              int          buflen)
{
    if (NE_Timestamp(ts_p, NullTimestamp)) {
	time_quad_t  time;
	char *ptr;

	COPY_Time(ts_p->ts_time, &time);
	sys_utc_to_string(&time, buf_p, buflen);
	ptr = &buf_p[strlen(buf_p)];
	if (ptr < &buf_p[buflen-18])
	    (void)sprintf(ptr, "/%02x-%02x-%02x-%02x-%02x-%02x",
		ts_p->ts_node[0], ts_p->ts_node[1],
		ts_p->ts_node[2], ts_p->ts_node[3],
		ts_p->ts_node[4], ts_p->ts_node[5]);
    } else if (2 <= buflen) {
	*buf_p++ = '0';
	*buf_p = 0;
    }
}
