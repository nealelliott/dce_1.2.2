/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsgetmsg.c,v $
 * Revision 1.1.12.2  1996/02/18  19:34:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:11  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:24:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:46:17  root]
 * 
 * Revision 1.1.10.4  1994/06/30  19:17:11  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:01:20  mccann]
 * 
 * Revision 1.1.10.3  1994/06/09  18:41:47  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:58  devsrc]
 * 
 * Revision 1.1.10.2  1994/04/29  15:53:55  peckham
 * 	Remove unnecessary conditionals.
 * 	Eliminate dependency on cds_events.c routines.
 * 	[1994/04/29  14:20:08  peckham]
 * 
 * Revision 1.1.10.1  1994/03/12  22:07:00  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:29:56  peckham]
 * 
 * Revision 1.1.8.1  1993/09/24  16:55:59  peckham
 * 	OT#8390: Purge unused status codes
 * 	    Removed stale conditionalized program code.
 * 	[1993/09/24  16:54:18  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  13:57:04  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:12:05  zeliff]
 * 
 * Revision 1.1.5.2  1992/12/15  15:55:06  tom
 * 	Bug 6409 - add setlocale call for i18n.
 * 	[1992/12/15  15:17:31  tom]
 * 
 * Revision 1.1.2.4  1992/06/26  07:14:47  jim
 * 	Added setlocale call for AIX 3.2.
 * 	[1992/05/29  18:53:01  jim]
 * 
 * Revision 1.1.2.2  1992/03/22  22:21:53  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:32:04  weisman]
 * 
 * Revision 1.1  1992/01/19  15:18:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnsgetmsg.c
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

#include <dce/dce.h>
#include <dce/cdsclerk.h>
#include <dce/dce_error.h>
#include <dcecdssvc.h>
#include <pthread.h>
#include <string.h>
#include <nl_types.h>
#include <uc_clerk.h>

static pthread_once_t getmsg_once = pthread_once_init;
static pthread_mutex_t getmsg_mutex;

/*
 * Initialize this module's statics
 */
static void 
getmsg_init (void)
{
       pthread_mutex_init(&getmsg_mutex, pthread_mutexattr_default);
}

/* ----------------------------------------------------------------------
 *  Return address of string for given local status
 * ----------------------------------------------------------------------
 */
int
cdsGetMsg (int   status,
           char  *out_p,
           int   *outLen_p)
{
    int	 outLen = 0;
    int	 msgLen;

    pthread_once(&getmsg_once, getmsg_init);

    if (out_p) {
	if (!outLen_p) return(DNS_INVALIDARGUMENT);

	outLen = *outLen_p;
    } else if (outLen_p)
	*outLen_p = 0;

    pthread_mutex_lock(&getmsg_mutex); {
	char err_buf[256];

	dce_error_inq_text(status, (unsigned char *)err_buf, NULL);
	msgLen = strlen(err_buf);
	if (0 < (outLen -= msgLen)) {
		COPY_bytes(err_buf, out_p, msgLen+1);
	} else {
	    pthread_mutex_unlock(&getmsg_mutex);
	    return(CDS_CL_READ);
	}
    } pthread_mutex_unlock(&getmsg_mutex);

    if (outLen_p)
	*outLen_p -= outLen;

    return((out_p && (outLen < 0)) ? CDS_NOROOM : CDS_SUCCESS);
}

