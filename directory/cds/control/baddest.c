/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: baddest.c,v $
 * Revision 1.1.6.2  1996/02/18  19:27:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:56  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:06:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:18  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:09  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:36:05  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  16:08:20  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:53:08  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:57:30  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:55:56  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:01:13  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:30:41  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:36  devrcs
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

/* given two status codes return the one that indicates the worst news */

/* this is just a beginning.  much work is needed here. */

#include <cdscp.h>

	int
baddest (
	int	a,
	int	b)
{
    int x;				/* the worst of the two status codes */

    if      (a == DNS_SUCCESS) x=b;
    else if (b == DNS_SUCCESS) x=a;
    else				/* this is the interesting&hard part */
	{				/* for now, make a guess */
	x = b;
	}
    return(x);				/* return one of them */
    }
