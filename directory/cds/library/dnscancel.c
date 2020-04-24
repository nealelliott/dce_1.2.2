/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnscancel.c,v $
 * Revision 1.1.8.2  1996/02/18  19:32:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:23  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:21:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:13  root]
 * 
 * Revision 1.1.6.4  1994/06/30  19:16:34  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:59:06  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:41:00  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:19  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:52:22  peckham
 * 	Change dns*() to cds*().
 * 	[1994/04/29  14:15:42  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:06:03  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:26:04  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:54:24  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:08:06  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:16:55  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:30:38  weisman]
 * 
 * Revision 1.1  1992/01/19  15:17:44  devrcs
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


#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_clerk.h>

/* Cancels all outstanding dns requests by unbinding to the child. */
/* Bits of memory may be left lying around.                        */
/* In the future, this routine should deal with memory, as well as */
/* unbind.                                                         */

int 
cdsCancel (void)
{
    int cancelstat;

    cancelstat = cds_unbind();

    return(cancelstat);
}
