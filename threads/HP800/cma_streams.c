/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: cma_streams.c,v $
 * Revision 1.1.2.2  1996/10/03  15:17:32  arvind
 * 	Add OSF copyright
 * 	[1996/05/24  23:18 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.2.1  1996/06/05  14:20:31  arvind
 * 	code cleanup, removed  _CMA_PROTO_ from function definitions
 * 	[1994/07/06  16:48 UTC  jss  /main/HPDCE02/2]
 * 
 * 	Remove fattach/fdetach code ...
 * 	[1993/08/20  18:38:02  hopkins]
 * 
 * 	*
 * 	Add stubs for cma_f{at,de}tach().
 * 	[1993/08/10  20:43:38  hopkins]
 * 
 * 	*
 * 	First cut at STREAMS wrappers
 * 	[1993/08/10  20:09:28  hopkins]
 * 
 * $EndLog$
 */
/*
 *	Implement wrappers for STREAMS functions
 */

#include <cma.h>
#include <cma_thdio_defs.h>
#include <stropts.h>
#include <poll.h>

/* secs to wait between polls - currently set to the length of 1 time-slice */
/* this value is also defined in HP800/cma_hpux.c -- coordinate changes */
#define POLLWAIT	0.2

int
cma_poll (
	struct pollfd fds[],
	int nfds,
	int timeout)
    {
	cma_t_date_time time_now, time_delta, time_done;
	cma_t_interval interval_remaining;
	cma_t_interval interval_delay = POLLWAIT;
	int retval = 0;

	cma__assert_not_kernel();

	/*
	 * Check for invalid timeout, since the "real" poll()
	 * won't see the value passed by the caller ...
	 */
	if( timeout < -1 ) return(cma__set_errno(EINVAL), -1);

	/*
	 * Convert timeout (expressed as milliseconds) to
	 * something we can use.  If the CMA interfaces and/or
	 * types used for time ever change, this will have
	 * to be revisited.
	 */
	if( timeout > 0 ) {
	    time_delta.tv_sec = timeout/1000;
	    time_delta.tv_usec = (timeout%1000) * 1000;
	    cma__get_time(&time_now);
	    cma__add_time(&time_done, &time_now, &time_delta);
	}

	while( 1 ) {
	    retval = poll( fds, nfds, 0 );
	    if( retval || !timeout )
		break;
	    if( timeout > 0 ) {
		cma__get_time(&time_now);
		if(!cma__time_lss(&time_now, &time_done))
		    break;
		else {
		    cma__subtract_time(&time_delta, &time_done, &time_now);
		    interval_remaining =  (float)time_delta.tv_sec;
		    interval_remaining += (float)((float)time_delta.tv_usec * 0.000001);
		    interval_delay = (interval_remaining < POLLWAIT) ? interval_remaining : POLLWAIT ;
		}
	    }
	    cma_delay(interval_delay);
	}

	return( retval );
    }

#undef POLLWAIT

int
cma_getmsg (
	int fd,
	struct strbuf *ctlbuf,
	struct strbuf *databuf,
	long *options)
    {
        cma__ts_func (getmsg, fd, (fd, ctlbuf, databuf, options), cma__c_io_read, 0);
    }

int
cma_getpmsg (
	int fd,
	struct strbuf *ctlbuf,
	struct strbuf *databuf,
	int *band,
	long *options)
    {
        cma__ts_func (getpmsg, fd, (fd, ctlbuf, databuf, band, options), cma__c_io_read, 0);
    }

int
cma_putmsg (
	int fd,
	struct strbuf *ctlbuf,
	struct strbuf *databuf,
	long options)
    {
        cma__ts_func (putmsg, fd, (fd, ctlbuf, databuf, options), cma__c_io_write, 0);
    }

int
cma_putpmsg (
	int fd,
	struct strbuf *ctlbuf,
	struct strbuf *databuf,
	int band,
	long options)
    {
        cma__ts_func (putpmsg, fd, (fd, ctlbuf, databuf, band, options), cma__c_io_write, 0);
    }

