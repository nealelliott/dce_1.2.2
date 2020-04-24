/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: emit_parentcts_line.c,v $
 * Revision 1.1.6.2  1996/02/18  19:28:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:42  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:08:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:19  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:41  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:42:09  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:44  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:26  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:58:39  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:59:33  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:03:52  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:34:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:21:10  devrcs
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
emit_parentcts_line (
	struct op_blk	*outbp,		/* ptr to output block */
	unsigned char   *tsp)		/* ptr to the timestamp */
{
    char valuebuf[100];			/* printable cts built here */

    fmt_uid_for_print(		        /* format the right side of the line */
	tsp,				/* ptr to the timestamp */
	valuebuf);			/* ptr to output buffer */

    emit_attr_string(outbp,
	CDSCP_PARENTCTS_LINE, "Parent's UUID", valuebuf);
}
