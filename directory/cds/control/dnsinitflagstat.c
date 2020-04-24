/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsinitflagstat.c,v $
 * Revision 1.1.8.2  1996/02/18  19:28:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:29  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:08:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:04  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:00:33  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:41:52  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:36:37  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:19  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:58:16  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:58:30  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:03:05  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:33:13  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 */

#include <cdscp.h>

#ifdef DCE_SEC
extern int Authentication;
#endif

void
dnsInitFlagStat (
	struct dnsFlagStat	*p)	/* ptr to flags and status struct */
{
    memset(p, '\0', sizeof(*p));
    p->fsMaybeMore = FALSE;		/* accept the default of FALSE */
    p->fsDontCache = TRUE;		/* tell clerk not to cache the result */
    p->fsNameType = dnsOpqDNA;  	/* default to opaque names */
#ifdef DCE_SEC
    p->fsUnAuth = !Authentication;	/* see mgmt_main.c */
    p->fsTrustAll = !Authentication;
#else
    p->fsUnAuth = TRUE;			/* default is no authentication */
    p->fsTrustAll = TRUE;		/* trust any server */
#endif
    p->fsCacheTimeout = 0;		/* use old stuff in cache */
    p->fsPartialRes = NULL_DNS_OPQ_FNAME; /* no partial resolution buffer provided */
    return;		   		/* no value from void routine */
}
