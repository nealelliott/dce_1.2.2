/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: report_parse_error.c,v $
 * Revision 1.1.9.2  1996/02/18  19:30:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:54  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:11:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:07  root]
 * 
 * Revision 1.1.7.5  1994/08/03  19:01:23  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:43:57  mccann]
 * 
 * Revision 1.1.7.4  1994/06/23  18:29:52  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:18:42  mccann]
 * 
 * Revision 1.1.7.3  1994/06/09  18:37:48  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:51  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/04  20:06:15  peckham
 * 	Fixes for sams file cleanup
 * 	[1994/04/04  18:08:34  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:01:03  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:07:04  peckham]
 * 
 * Revision 1.1.5.1  1993/09/24  16:55:54  peckham
 * 	OT#8390: Purge unused status codes
 * 	    Removed stale program code.
 * 	[1993/09/24  16:47:20  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:09:49  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:41:58  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:22:13  devrcs
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
#include <prsdef.h>
#include <cmdblk.h>

void
report_parse_error (
	struct cmdblk	*cbp,
	int		code)
{
    struct op_blk *const obp = get_op_blk_ptr();

    switch (code)
	{
	case RET_ERROR:			/* THE NORMAL REASON TO BE HERE */
	    {
	    dce_fprintf(stderr, CDSCP_PARSE_ERROR, cbp->cmdbuf,
			spaces(cbp->prsblk->maxcur - cbp->prsblk->cmd));
	    break;			/* do not fall into next case */
	    }
	default:
	    {
	    dce_svc_printf(CDSCP_MSG_PARSE_STATUS_MSG, code, cbp->cmdbuf);
	    break;			/* do not fall into next case */
	    }
	}				/* end of switch */

    return;				/* no status to report */

}					/* end of report_parse_error */
