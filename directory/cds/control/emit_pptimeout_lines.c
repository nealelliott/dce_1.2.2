/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: emit_pptimeout_lines.c,v $
 * Revision 1.1.7.2  1996/02/18  19:28:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:45  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:08:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:23  root]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:45  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:29  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:58:43  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:59:44  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:04:01  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:34:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:21:12  devrcs
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
emit_pptimeout_lines (
	struct op_blk	*outbp,		/* ptr to output block */
	unsigned char	*timeoutp)	/* ptr to timeout */
{
    emit_attr_string(outbp, CDSCP_PPTIMEOUT_LINE, "Timeout", ":");
    emit_timeout_lines(			/* write timeout lines */
        outbp,				/* ptr to output block list */
        (dns_timeout *)timeoutp);	/* ptr to timeout */

    return;                             /* no value from void routine */
}
