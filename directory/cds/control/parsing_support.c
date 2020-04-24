/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: parsing_support.c,v $
 * Revision 1.1.6.2  1996/02/18  19:30:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:39  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:11:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:58  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:01:16  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:05  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:37  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:37  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:00:40  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:05:50  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:08:43  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:40:27  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:22:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*************************************************************************
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
 *************************************************************************/



#include <cdscp.h>
#include <ctype.h>	/* definitions of character types */
#include <prsdef.h> 	/* definitions for the parser */


void
prsblk_init (
	struct prs	*pbp,		/* ptr to parser context block */
	char		*bufp,		/* ptr to buffer for command */
	int		bufsiz)		/* size of command buffer */
{
    /* ok to change values later. use 0 as parm on call. */
    /* initialize the command parser context block */
    memset(pbp, '\0', sizeof(*pbp));
    pbp->cmd	= bufp;			/* ptr to command to be parsed */
    pbp->cms	= bufsiz;		/* how big the command can be */
}					/* end of void prsblk_init() */
