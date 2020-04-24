/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isbtree.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:09  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:48  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:02  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:57:38  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:58:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:04:26  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:49:56  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:23  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:36:31  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:40:11  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isbtree.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:07 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/***************************************************************************
 *
 *			   INFORMIX SOFTWARE, INC.
 *
 *			      PROPRIETARY DATA
 *
 *	THIS DOCUMENT CONTAINS TRADE SECRET DATA WHICH IS THE PROPERTY OF 
 *	INFORMIX SOFTWARE, INC.  THIS DOCUMENT IS SUBMITTED TO RECIPIENT IN
 *	CONFIDENCE.  INFORMATION CONTAINED HEREIN MAY NOT BE USED, COPIED OR 
 *	DISCLOSED IN WHOLE OR IN PART EXCEPT AS PERMITTED BY WRITTEN AGREEMENT 
 *	SIGNED BY AN OFFICER OF INFORMIX SOFTWARE, INC.
 *
 *	THIS MATERIAL IS ALSO COPYRIGHTED AS AN UNPUBLISHED WORK UNDER
 *	SECTIONS 104 AND 408 OF TITLE 17 OF THE UNITED STATES CODE. 
 *	UNAUTHORIZED USE, COPYING OR OTHER REPRODUCTION IS PROHIBITED BY LAW.
 *
 *
 *  Title:	isbtree.c
 *  Description:
 *		High level B-tree management routines.
 *
 ************************************************************************
 */


/*
 * B-tree management routines
 *  operates mainly on the global variables:
 *	openp->o_trees[] represents path down b-tree
 *	treelev		number of b-tree levels
 *	treeitem	item at bottom of tree
 *	treeflags	BEGFILE and ENDFILE flags
 *
 *  Relational Database Systems, Inc.
 *  Roy Harrington	November 4, 1981
 */

#include <isdec.c>

/*
 * btposition(keyd, flag) -- position at first or last item in b-tree
 *
 *	flag =  ISFIRST => first item
 *	     =  ISLAST  => last item
 *	treelev = number levels
 *	openp->o_trees[] = path down tree
 *	treeitem = first or last item of tree
 */

STATIC int btposition(keyd, flag)

struct keydesc *keyd;
int flag;
{
	register struct tree *tp;
	register struct buff *bp;
	register struct item *ip;
	register char *np;
	register long node;

	node = keyd->k_rootnode;
	ip = &treeitem;				/* tree item ptr	*/
	tp = &openp->o_trees[0];		/* tree pointer		*/
	treeflags = 0;

	for (treelev = 0; ; treelev++, tp++)
	    {					/* search down tree	*/
	    tp->t_offset = 0;
	    tp->t_node = node;
	    bp = bfread(node);			/* get node buffer	*/
	    if (bp == NULL)
		{ treelev = -1; break; }
#ifndef VMS
	    np = bp->bf_un.bf_node->n_items;
#else /* VMS */
	    np = ((struct node *) GSADDRESS(bp->bf_un.bf_node))->n_items;
#endif /* VMS */
	    tp->t_nextoff = tp->t_used = bp->bf_used - 2;

	    if (tp->t_used == 0)		/* empty tree ?		*/
		{				/* yes			*/
		treeflags = (BEGFILE|ENDFILE);
		}
	    else
		{
		if (flag == ISLAST)		/* get first or last item */
		     tp->t_offset  = itprevious(np, tp->t_used, ip, keyd);
		else tp->t_nextoff = itunpack(np, 0, ip, keyd);
		node = ip->it_ptr;
		}
	    bfrelease(bp);
	    if (bp->bf_level == 0)		/* leaf level ?		*/
		break;				/* yes, return		*/
	    }

	if (flag == ISLAST)
	    {
	    tp->t_offset = tp->t_used;
	    treeflags = ENDFILE;
	    }
	treeleaf = treelev;
}

/*
 * search b-tree for current position
 */

STATIC int btcurrent()
{
	register struct keydesc *keyd;
	register struct item *ip;

	if ( ! (openp->o_flags & OCURPOS))	/* current position ?	*/
	    return(-1);				/* none, error		*/
	
	ip = &openp->o_curitem;
	keyd = &openp->o_curkey;

	if (treelev < 0)
	    btsearch(ip, keyd, 0, 0);		/* search down tree	*/

	if (treelev < 0) return(-1);
	if (itcompare(&treeitem, ip, keyd, 0) != 0 ||
		treeitem.it_ptr != ip->it_ptr)
	    {
	    openp->o_flags &= ~OCURREAD;
	    }
	return(0);
}

/*
 * search for given item in b-tree
 *	gtflag: -- find item greater than
 *	treelev = number levels
 *	openp->o_trees[] = path down tree
 *	treeitem = given item of tree
 *
 *  if compression is used in B-tree then a linear search is
 *	necessary within each B-tree node
 *  if no compression is used then a binary search is used
 *	within each B-tree node
 */

STATIC int btsearch (sip, keyd, keylen, gtflag)

struct item *sip;			/* search item		*/
register struct keydesc *keyd;	/* key description	*/
int keylen;			/* key length (can be short)	*/
int gtflag;			/* greater than flag		*/
				/* 0 => find first value >= key	*/
				/* 1 => find first value > key	*/
				/* 2 => position for inserting	*/
{
	register struct tree *tp;
	register struct item *ip;
	register struct buff *bp;
	register char *np;
	register int rel, itemlen, binsrch, left, right, mid;
	register long node;

	node = keyd->k_rootnode;
	ip = &treeitem;				/* tree item ptr	*/
	tp = &openp->o_trees[0];		/* tree pointer		*/
	treeflags = 0;

/*
 * search down tree one level at a time
 */
	for (treelev = 0; ; treelev++, tp++)
	    {
	    tp->t_node = node;
	    bp = bfread(node);			/* get node buffer	*/
	    if (bp == NULL)
		{ treelev = -1; break; }
#ifndef VMS
	    np = bp->bf_un.bf_node->n_items;	/* get node pointer	*/
#else /* VMS */
	    np = ((struct node *) GSADDRESS(bp->bf_un.bf_node))->n_items;
#endif /* VMS */
	    tp->t_offset = tp->t_nextoff = 0;
	    tp->t_used = bp->bf_used - 2;	/* 2 bytes for n_used	*/

	    if (tp->t_used == 0)		/* empty file ?		*/
		{
		treeflags = (BEGFILE|ENDFILE);	/* yes, set variables	*/
		bfrelease(bp);
		break;
		}

	    if ((keyd->k_flags & COMPRESS) == 0)/* any compression ?	*/
		{				/* no, do binary search	*/
		itemlen = keyd->k_len + ((keyd->k_flags & ISDUPS) ? 6 : 4);
		left  = 0;			/* leftmost item #	*/
		right = (tp->t_used/itemlen)-1;	/* rightmost item #	*/
		binsrch = (right >= 3);
		}
	    else binsrch = 0;
/*
 * search for appropriate item in this b-tree node
 */
	    for (;;)
		{
		if (binsrch)
		    {				/* binary search stuff	*/
		    mid = (left+right) >> 1;
		    tp->t_offset = mid * itemlen;
		    }
		else tp->t_offset = tp->t_nextoff; /* linear search	*/

						/* get next item	*/
		tp->t_nextoff = itunpack(np, tp->t_offset, ip, keyd);
						/* compare items	*/
		rel = itcompare(sip, ip, keyd, keylen);

		if (rel == 0 && gtflag == 2)	/* match & inserting	*/
		    {
		    if (sip->it_dupnum <= ip->it_dupnum)  /* fix dupnum	*/
			sip->it_dupnum  = ip->it_dupnum+1;

		    if (bp->bf_level == 0)	/* at leaf level ?	*/
			{
			if (keyd->k_flags & ISDUPS)   /* dups allowed ?	*/
			    openp->o_flags |= ODUPL;  /* yes, ok	*/
			else
			    {
			    iserrno = EDUPL;	      /* no, error	*/
			    treelev = -1;
			    break;
			    }
			}
		    }

		if (rel == 0 && gtflag == 0)	/* match & want first	*/
		    {				/* compare dupnum's	*/
		    if ((keyd->k_flags & ISDUPS) &&
			    sip->it_dupnum > ip->it_dupnum)
			 rel = 1;
		    else rel = -1;
		    }

		if (binsrch)
		    {
		    if (rel < 0)
			right = mid;
		    else left = mid + 1;
		    if ((right - left) < 3)	/* just do linear now ?	*/
			{
			binsrch = 0;
			tp->t_nextoff = left * itemlen;
			}
		    }
		else
		    {
		    if (rel < 0 || tp->t_nextoff >= tp->t_used)
			break;		/* found closest match or end	*/
		    }
		}

	    node = ip->it_ptr;
	    bfrelease(bp);
	    if (bp->bf_level == 0)		/* leaf level ?		*/
		break;				/* yes, return		*/
	    }
	treeleaf = treelev;
	if (rel >= 0)		/* did item qualify or just last one	*/
	    {
	    if (gtflag == 2)			/* inserting ?		*/
		tp->t_offset = tp->t_used;	/* yes, pt to end	*/
	    else btmove(keyd, ISNEXT);		/* no, move to next	*/
	    }
}

/*
 * btmove(keyd, flag) -- move to next or previous item in b-tree
 *
 *	flag = ISNEXT  => move to next item
 *	     = ISPREV  => move to previous item
 *	treelev = number levels
 *	openp->o_trees[] = path down tree
 *	treeitem = next or previous item of tree
 */

STATIC int btmove(keyd, flag)

struct keydesc *keyd;
int flag;
{
	register struct tree *tp;
	register struct item *ip;
	register struct buff *bp;
	register int direction;
	register char *np;

	direction = (flag == ISNEXT ? ENDFILE : BEGFILE);
	if (treeflags & direction)
	    return;

	treeflags = 0;
	if (treelev < 0)
	    {
	    treeflags = direction;		/* at end of file	*/
	    return;
	    }
	ip = &treeitem;				/* tree item ptr	*/
	if (keyd->k_nparts == 0)		/* physical read ?	*/
	    {
	    if (flag == ISNEXT)
		{
		if (++ip->it_ptr > ld4(dictp->d_nextdata)) /* NEXT	*/
		    {
		    dictread();			/* insure updated info	*/
		    if (ip->it_ptr > ld4(dictp->d_nextdata))
			treeflags = direction;
		    }
		}
	    else if (--ip->it_ptr <= 0L)	/* PREVIOUS		*/
		treeflags = direction;
	    return;
	    }
	tp = &openp->o_trees[treelev];		/* tree pointer		*/

	if (flag == ISNEXT ?	 		/* another item in node	*/
		tp->t_nextoff >= tp->t_used :	/*  for NEXT		*/
		tp->t_offset  == 0)		/*  for PREVIOUS	*/
	    {
	    if (treelev > 0)			/* no, any more levels?	*/
		{
		treelev--;
		btmove(keyd, flag);		/* yes, go up a level	*/
		if (treelev < 0) return;
		treelev++;
		}
	    else treeflags = direction;		/* top level => EOF	*/

	    if (treeflags & direction)
		{
		if (treelev == treeleaf)	/* leaf node ?		*/
		    {
		    if (flag == ISNEXT)		/* yes, point to end	*/
			 tp->t_offset = tp->t_used;  /* for NEXT	*/
		    else tp->t_nextoff = 0;	     /* for PREVIOUS	*/
		    }
		return;
		}
	    tp->t_node = ip->it_ptr;
	    tp->t_offset = tp->t_nextoff = 0;
	    }
	else
	    {
	    if (flag == ISNEXT)
		 tp->t_offset  = tp->t_nextoff;	   /* for NEXT		*/
	    else tp->t_nextoff = tp->t_offset;	   /* for PREVIOUS	*/
	    }

	bp = bfread(tp->t_node);		/* get node buffer	*/
	if (bp == NULL)
	    { treelev = -1; return; }
#ifndef VMS
	np = bp->bf_un.bf_node->n_items;
#else /* VMS */
	np = ((struct node *) GSADDRESS(bp->bf_un.bf_node))->n_items;
#endif /* VMS */
	tp->t_used = bp->bf_used - 2;
	if (tp->t_nextoff == 0) tp->t_nextoff = tp->t_used;

	if (flag == ISNEXT)			/* unpack next item	*/
	     tp->t_nextoff = itunpack(np,  tp->t_offset, ip, keyd);
	else tp->t_offset = itprevious(np, tp->t_nextoff, ip, keyd);

	bfrelease(bp);
}

/*
 * btdups -- check next key in order for a duplicate
 *		set isstat2 flag for cobol usage
 *
 *	if duplicate compression is in use then we can check
 *		compression flags for fast test
 *	else move to next item and compare
 */

btdups(keyd, mode)
register struct keydesc *keyd;
register int mode;
{
	register struct tree *tp;

	tp = &openp->o_trees[treeleaf];
	if (keyd->k_flags & DCOMPRESS)		/* dup compression ?	*/
	    {
	    if (mode == ISNEXT && tp->t_nextoff < tp->t_used)
		{
		if (treeitem.it_flags & NEXTDUP)
		    openp->o_flags |= ODUPL;
		return;
		}
	    if (mode == ISPREV && tp->t_offset > 0)
		{
		if (treeitem.it_flags & DUP)
		    openp->o_flags |= ODUPL;
		return;
		}
	    }

	btmove(keyd, mode);
	if ((treeflags & (BEGFILE|ENDFILE)) == 0 &&
		itcompare(&openp->o_curitem, &treeitem, keyd, 0) == 0)
	    openp->o_flags |= ODUPL;
	btmove(keyd, (mode == ISPREV ? ISNEXT : ISPREV));
}
