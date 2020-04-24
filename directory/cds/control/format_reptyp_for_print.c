/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: format_reptyp_for_print.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:12  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:09:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:39  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:01:02  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:48:48  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:37:04  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:52  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/04  20:06:02  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:06:13  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:59:26  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:02:17  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:06:42  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:37:53  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:21:42  devrcs
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

void
format_reptyp_for_print (
	char		*bufp,		/* ptr to output buffer */
	unsigned int	reptyp)	/* the reptyp field */

    /* caller supplies output buffer, and it better be big enough. */
    /* 100 bytes should be enough in any language. */
    /* to check, measure all the reptyp messages in the message file */

{
    static unsigned32 codes[] = {CDSCP_REPTYP001, CDSCP_REPTYP002,
	CDSCP_REPTYP003, CDSCP_REPTYP004};
    unsigned32 code =
	((1 <= reptyp) && (reptyp <= 4)) ? codes[reptyp-1] : CDSCP_REPTYP_BAD;

    cds_sprintf(bufp, code,
		"<replica type value msg not found> value = %d", reptyp);
}
