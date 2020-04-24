/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: attrgroup_actions.c,v $
 * Revision 1.1.6.2  1996/02/18  19:27:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:53  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:06:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:13  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:06  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:35:55  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  16:08:17  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:53:04  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:57:26  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:55:33  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:00:56  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:30:24  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:33  devrcs
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

/* this module is incomplete */
/* attribute blocks are built bottom up by the parser. */
/* when an attribute block is complete it is copied into the attribute list */
/* there is only one attribute block available to the parser action routines. */

#include <cdscp.h>
#include <prsdef.h>

static struct attrblk ab;		/* only one of these */

int					/* always success */
set_attr(				/* record which attribute in entblk */
    struct prs *pcbp)			/* ptr to parser context block */
{
    ab.grpattr = pcbp->av1;		/* store the code */
    return (ACT_SUCCESS);		/* CLAIM SUCCESS */
}					/* end of set_attr */    

struct attrblk *
attrblk_pointer ()			/* provide pointer to attribute block */
{
    return (&ab);
}

void
attrblk_init (
	struct attrblk	*p)
{
    p->kind_of_structure_code = ATTRIBUTE_NAME_BLOCK;
    p->version = 1;
    p->fwd = p;				/* next is self */
    p->bck = p;				/* previous is self */
    p->head = p;			/* head of list is self */
					/* do not touch p->blkalloc */
    p->grpattr = NONE_ATTR;		/* no attribute seen yet */
    COPY_AttributeName(NullAttributeName, &p->bigattr);
    p->relop = RELOP_NO;		/* no relational operator yet */
    p->cmprules = 0;			/* no compare rules defined yet */
    p->av = 0;				/* no single value available yet */
}

int
save_attr (
	struct prs *pcbp)		/* ptr to parser context block */
{
    struct attrblk *const hp = get_attrblk_ptr();

    /* copy the block that is here into the list */
    int status = copyattr_before(&ab, hp);

    return (status ? ACT_SUCCESS : ACT_ERROR);
}
