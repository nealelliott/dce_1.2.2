/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: format_chstate_for_print.c,v $
 * Revision 1.1.6.2  1996/02/18  19:29:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:04  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:09:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:34  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:56  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:49:01  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:57  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:46  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:59:12  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:01:44  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:05:53  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:36:50  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:21:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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

/* format the value of a CDS_CHState attribute */

#include <cdscp.h>

void
format_chstate_for_print (
    char		*bufp,		/* ptr to output buffer */
    int			chstate)	/* the chstate field */

    /* caller supplies output buffer, and it better be big enough. */
    /* 100 bytes should be enough in any language. */
    /* to check, measure all the chstate messages in the message file */

{
    static unsigned32 codes[] = {CDSCP_CHSTATE1, CDSCP_CHSTATE2,
	CDSCP_CHSTATE3};
    unsigned32 code = ((1 <= chstate) && (chstate <= 3))
	? codes[chstate-1] : CDSCP_CHSTATE_BAD;

    cds_sprintf(bufp, code,
		"<chstate value msg not found> value = %d", chstate);
    return;				/* no value from void routine */
}					/* end of format_chstate_for_print */



