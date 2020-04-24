/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pretend_not_seen.c,v $
 * Revision 1.1.6.2  1996/02/18  19:30:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:41  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:11:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:00  root]
 * 
 * Revision 1.1.4.2  1994/06/09  18:37:39  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:09:40  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:00:44  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:06:04  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:08:52  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:40:39  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:19  devrcs
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

/* this parser action routine undoes a successful parse of one element. */
/* this is an example of its use.  suppose there is a way to parse an entity. */
/* an entity is dir name or obj name, or clh name, etc */
/* somewhere else you need to parse an entity, but it is restricted to a dir. */
/* simple parse the dir, and pretend to not see it, then parse the entity. */

/* if the first paramater is not zero, it is used to set the function */

#include <cdscp.h>
#include <prsdef.h>

int
pretend_not_seen (
	struct prs	*pcbp)		/* ptr to parser context block */
{
    int always_act_success;		/* constant status from routine */

    pcbp->nxt = pcbp->cur;		/* back up to the start of whatever */
    if (pcbp->av1 != 0)
	always_act_success = 
	    set_func(pcbp);
    return(ACT_SUCCESS);		/* allow the parse to resume */
}
