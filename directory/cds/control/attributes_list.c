/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: attributes_list.c,v $
 * Revision 1.1.6.2  1996/02/18  19:27:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:54  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:06:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:39:14  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:00:07  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:35:58  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  16:08:18  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:53:05  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:57:27  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:55:41  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:01:01  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:30:30  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:20:34  devrcs
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

void
init_singlevalueblk (
	struct singlevalueblk	*p)	/* ptr to single value block */
{
    p->kind_of_structure_code = SINGLE_VALUE_BLOCK;
    p->version = 1;
					/* do not touch the allocated codes */
    p->vp = 0;				/* it has no data yet */
    return;				/* void routines return no value */
}

struct attrblk *
copyattr_new (
	struct attrblk	*oldp)		/* ptr to source attr block */
{
    struct attrblk *newp;		/* ptr to new copy of attr block */
    int code = 1;			/* 0 if problem, 1 if none */

    newp = (struct attrblk *)
	malloc(sizeof(struct attrblk));	/* try to get space for new copy */
    if (newp != 0)			/* did the malloc succeed */
	{				/* yes */
	*newp = *oldp;			/* copy the attribute block */
	newp->blkalloc = 1;		/* but this one is malloced */
	if (oldp->av)
	    {
	    struct singlevalueblk *oldsvbp = oldp->av;
	    struct singlevalueblk *svbp = (struct singlevalueblk *)
                      malloc(sizeof(struct singlevalueblk));
	    if (svbp)
		{
		AtomicValue_t *oldvp = oldsvbp->vp;
		int size = LEN_AtomicValue(oldvp);
		AtomicValue_t *vp = 
		    (AtomicValue_t *)malloc(size);
		init_singlevalueblk(svbp);	/* make a fresh block */
		if (vp)
		    {
		    memcpy(vp, oldvp, size);	/* copy atomic value */
		    }
		else
		    {
		    free(svbp);		/* void in ansi c */
		    code = 0;		/* failure because of malloc */
		    }
		if (code)
		    {
		    /* copy singlevalue block */
		    memcpy(svbp, oldsvbp, sizeof(*oldsvbp));
		    svbp->valalloc = 1;	/* the atomic value is malloced */
		    svbp->blkalloc = 1;	/* the singlevalueblk is malloced */
		    svbp->vp = vp;	/* remember where to find data */
		    }
		}
	    else
		{
		free(newp);		/* give up this cuz we cant get more */
		code = 0;		/* failure because of malloc */
		}
	    newp->av = svbp;		/* remember where the data is now */
	    }				/* end of no value to copy */
	}				/* end of malloc attr blk worked */
    else
	{				/* could not malloc a new attr blk */
	code = 0;
	}

    return(code ? newp : 0);		/* ptr to new block or null */
}					/* end of copyattr_new */


void
linkattr_after (
	struct attrblk	*fp,		/* ptr to "from" attr block */
	struct attrblk	*tp)		/* ptr to "to" attr block */
{
					/* first fill in the new block */
    fp->head = tp->head;		/* every block points to head */
    fp->fwd  = tp->fwd;			/* next is what was next */
    fp->bck  = tp;			/* after the stated place */
					/* then link it in. carefully. */
    fp->bck->fwd = fp;			/* after the previous, is this */
    fp->fwd->bck = fp;			/* before the next is this */
}					/* end of linkattr_after */

void
linkattr_before (
	struct attrblk	*fp,		/* ptr to "from" attr block */
	struct attrblk	*tp)		/* ptr to "to" attr block */
{
					/* first fill in the new block */
    fp->head = tp->head;		/* every block points to head */
    fp->fwd  = tp;			/* next is what we are before */
    fp->bck  = tp->bck;			/* after what was before before */
					/* then link it in. carefully. */
    fp->bck->fwd = fp;			/* after the previous, is this */
    fp->fwd->bck = fp;			/* before the next is this */
}					/* end of linkattr_before */

int
copyattr_before (
	struct attrblk	*fp,		/* ptr to "from" attr block */
	struct attrblk	*tp)		/* ptr to "to" attr block */
{
    /* by copying before the head of the list we get a queue. */
    struct attrblk *newp = copyattr_new(fp); /* make a copy of the block */

    if (newp != 0)
	{
	linkattr_before(newp, tp);	/* insert it in the list */
	}

    return(newp ? 1 : 0);
}					/* end of copyattr_before */

void
delete_1_attr (
	struct attrblk	*ep)		/* ptr to attr block */
{
    struct singlevalueblk *p1 = ep->av;	/* ptr to singlevalue block, or 0 */

    ep->fwd->bck = ep->bck;		/* remove from backwards list */
    ep->bck->fwd = ep->fwd;		/* remove from forwards list */

    if (p1)
	{
	if (p1->valalloc)		/* if a value in malloced mem */
	    free(p1->vp);		/* give back the space */
	if (p1->blkalloc)		/* if single value block malloced */
	    free(p1);			/* give back that space */
	}

    if (ep->blkalloc) free(ep);		/* give back space for block */
}
    /* typical use has the head of the list in static storage. */
    /* the pointer manipulation is a slow nop if this is the head */


void
delete_attr_list (
	struct attrblk	*ep)		/* ptr to attr block */
{
    struct attrblk *p,*pp;		/* working pointers to traverse list */

    for (p=ep; p != p->head; p=p->fwd)
	;				/* advance to head of list */

    pp=p->fwd;
    while (pp != p)     		/* delete all but head */
    {
	struct attrblk *trash_pp = pp;  /* set element to be removed */
        pp = pp->fwd;                   /* point to next element */
	delete_1_attr(trash_pp);
    }
    delete_1_attr(p);			/* delete head of list */
    /* ok to try to delete it, since no harm done if head not allocated */
}

struct attrblk *
find_attr (
	struct attrblk	*atrbp,		/* ptr to head of attribute list */
	dns_opq_sname	*osnp)		/* ptr to opq simple name sought */
{
    struct attrblk *cp;			/* ptr to the current attr blk */
    struct attrblk *rp = NULL;		/* ptr to be returned */

    for (cp = atrbp->fwd; cp != atrbp; cp = cp->fwd)
	{				/* look at all but the head */
	if (dnsCmpSimple(
		osnp, 
		(dns_opq_sname *)
		    &(cp->bigattr)) == 0)	/* is this it ? */
	    {				/* yes */
	    rp = cp;			/* remember where we found it */
	    break;			/* finding one is enough */
	    }
	}				/* end of for loop */
    return(rp);				/* give calller good ptr or 0 */
}
