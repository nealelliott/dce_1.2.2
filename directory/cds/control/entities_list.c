/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: entities_list.c,v $
 * Revision 1.1.6.2  1996/02/18  19:28:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:10:51  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:08:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:26  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:46  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:43:30  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:36:48  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:33  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:58:56  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:00:47  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:04:51  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:35:35  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:22:29  devrcs
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
 *
 */

#include <cdscp.h>

struct entblk *
copyent_new (
	struct entblk	*fp)		/* ptr to source entity block */
{
    struct entblk *newp;		/* ptr to new copy of block */
    int status;				/* from copy name operation */
    struct entblk *return_code;		/* what we tell the caller */

    return_code = 0;			/* assume failure until success */
    newp = (struct entblk *)
	malloc(sizeof(struct entblk));	/* try to get space for new copy */
    if (newp != 0)			/* did the malloc succeed */
	{				/* yes */
	*newp = *fp;			/* copy the block */
	newp->blk_alloc = 1;		/* only this one is allocated */
	newp->name_alloc = 0;		/* but pretend no name in it */
	newp->name_p = 0;		/* and pretend no name in it */
	status = 
	    move_name_to_entblk(fp->name_p, newp);  /* try to copy the name */
	if (status)			/* did the copy succeed? */
	    {				/* yes */
	    return_code = newp;		/* all is well */
	    }
	else
	    {				/* no, the copy operation failed */
	    free(newp);			/* give back the space for new block */
	    newp = 0;			/* prevent access to it */
	    }
	}
    return(return_code);		/* ptr, unless some problem happened */
}					/* end of copyent_new */


void
linkent_after (
	struct entblk	*fp,		/* ptr to "from" entity block */
	struct entblk	*tp)		/* ptr to "to" entity block */
{
					/* first fill in the new block */
    fp->head = tp->head;		/* every block points to head */
    fp->nxt  = tp->nxt;			/* next is what was next */
    fp->prv  = tp;			/* after the stated place */
					/* then link it in. carefully. */
    fp->prv->nxt = fp;			/* after the previous, is this */
    fp->nxt->prv = fp;			/* before the next is this */
}					/* end of linkent_after */

void
linkent_before (
	struct entblk	*fp,		/* ptr to "from" entity block */
	struct entblk	*tp)		/* ptr to "to" entity block */
{
					/* first fill in the new block */
    fp->head = tp->head;		/* every block points to head */
    fp->nxt  = tp;			/* next is what we are before */
    fp->prv  = tp->prv;			/* after what was before before */
					/* then link it in. carefully. */
    fp->prv->nxt = fp;			/* after the previous, is this */
    fp->nxt->prv = fp;			/* before the next is this */
}					/* end of linkent_before */

int
copyent_before (
	struct entblk	*fp,		/* ptr to "from" entity block */
	struct entblk	*tp)		/* ptr to "to" entity block */
{
    /* by copying before the head of the list we get a queue. */
    struct entblk *newp;		/* ptr to new copy of block */

    newp = copyent_new(fp);		/* make a copy of the block */
    if (newp != 0)
	{
	linkent_before(newp, tp);	/* insert it in the list */
	}
    return(newp ? 1 : 0);
}					/* end of copyent_before */

void
delete_1_ent (
	struct entblk	*ep)		/* ptr to entity block */
{					/* can not reference it after free */
    ep->nxt->prv = ep->prv;		/* remove from backwards list */
    ep->prv->nxt = ep->nxt;		/* remove from forwards list */
    if (ep->name_alloc)
	free(ep->name_p);		/* give back space for opaque name */
    if (ep->strname_alloc)
	free(ep->strname_p);		/* give back space for string name */
    if (ep->blk_alloc)
	free (ep);			/* give back space for block */
}
    /* typical use has the head of the list in static storage. */
    /* the pointer manipulation is a slow nop if eb points to head of list */

void
delete_ent_list (
        struct entblk	*ep)		/* ptr to entity block */
{
    struct entblk *p,*pp;		/* working pointers to traverse list */
    struct entblk *trash_pp;

    for (p=ep; p != p->head; p=p->nxt)
	;                               /* advance to head of list */

    pp=p->nxt;
    while (pp != p)		        /* delete all but head */
    {
        trash_pp = pp;                  /* set element to be removed */
        pp = pp->nxt;                   /* point to next element */
	delete_1_ent(trash_pp);
    }
    delete_1_ent(p);			/* delete head of list */
    /* ok to try to delete it, since no harm done if head not allocated */
}
