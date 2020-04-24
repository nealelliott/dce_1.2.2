/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isnode.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:21  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:57  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:19  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:59:04  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:07:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:06:12  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:50:49  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:00  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:37:56  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:41:13  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isnode.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:18 $";
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
 *  Title:	isnode.c
 *  Description:
 *		Low level B-tree node modification routines.
 *
 ************************************************************************
 */


/*
 * btree node modification management routines
 *  operates on items and btree nodes
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 */

#include <isdec.c>

struct item tempi1, tempi2;
char tempbuf[NODESIZE+KEYSIZE+8];
short tempcnt = 0;

#ifdef ISKERNEL		/* make critical section for kernel */
#define CRITICAL(v)	{while (v++ > 0) sleep((caddr_t)&v,PRIBIO);}
#define UNCRITICAL(v)	{if (v>1) wakeup((caddr_t)&v); v=0;}
#else
#define CRITICAL(v)
#define UNCRITICAL(v)
#endif /* ISKERNEL */

/*
 * btadditem(ip, keydesc) -- add item to b-tree
 */

STATIC int btadditem(ip, keydesc)
struct item *ip;
register struct keydesc *keydesc;
{
	register struct tree *tp;

	if (keydesc->k_nparts == 0)		/* physical key ?	*/
	    {
	    treelev = -1;
	    return(0);
	    }
	btsearch(ip, keydesc, 0, 2);		/* position into tree	*/
	if (treelev < 0) return(-1);
	tp = &openp->o_trees[treelev];

	if (tp->t_offset == 0 && ! (treeflags & BEGFILE))
	    {
	    btmove(keydesc, ISPREV);		/* move back		*/
	    if (itcompare(ip, &treeitem, keydesc, 0) == 0)
		{
		if (keydesc->k_flags & ISDUPS)	/* duplicates allowed ?	*/
		    {
		    openp->o_flags |= ODUPL;
		    ip->it_dupnum = treeitem.it_dupnum+1;
		    }
		else
		    {
		    iserrno = EDUPL;
		    treelev = -1;
		    return(-1);
		    }
		}
	    btmove(keydesc, ISNEXT);
	    }
	if (treelev >= 0)
	    {
	    CRITICAL(tempcnt);			/* share temp vars	*/
	    itcopy(ip, &tempi1, keydesc); 	/* insert into tree	*/
	    btinsert(&tempi1, keydesc, &tempi2);
	    UNCRITICAL(tempcnt);		/* release temp vars	*/
	    treelev = -1;
	    return(0);
	    }
	treelev = -1;
	return(-1);
}

/*
 * delete item from b-tree
 */

STATIC int btdelitem(ip, keydesc)
register struct item *ip;
register struct keydesc *keydesc;
{
	if (keydesc->k_nparts == 0)		/* physical key ?	*/
	    {
	    treelev = -1;
	    return(0);
	    }
	btsearch(ip, keydesc, 0, 0);		/* position into tree	*/

	while (treelev >= 0)			/* search dups chain	*/
	    {
	    if ((treeflags & ENDFILE) ||	/* item not found	*/
		itcompare(ip, &treeitem, keydesc, 0) != 0)
		{
		iserrno = ENOREC;
		break;
		}
	    if (ip->it_ptr == treeitem.it_ptr)	/* got the right one ?	*/
		{
		CRITICAL(tempcnt);		/* share temp vars	*/
		btremove(keydesc, &tempi1, &tempi2);/* remove from tree	*/
		UNCRITICAL(tempcnt);		/* release temp vars	*/
		treelev = -1;
		return(0);
		}
	    btmove(keydesc, ISNEXT);		/* move to next item	*/
	    }

	treelev = -1;
	return(-1);
}


/*
 * insert an item into btree node
 *   insert item "ip"
 *   nip and tempbuf are just temporaries
 */

STATIC int btinsert(ip, keydesc, nip)

struct item *ip;		/* item to insert		*/
register struct keydesc *keydesc;
register struct item *nip;
{
	register struct tree *tp;
	struct buff *bp, *bp2;
	register char *np;
	register int offset, used;
	char *np2;
	int imin, imax, nextoff, used2;
	long node2, allocrec();
/*
 * initialize variables etc.
 */
	ip->it_flags = 0;
	tp = &openp->o_trees[treelev];
	bp = bfread(tp->t_node);
	if (bp == NULL) return;
#ifndef VMS
	np = bp->bf_un.bf_node->n_items;	/* get node pointer	*/
#else /* VMS */
	np = ((struct node *) GSADDRESS(bp->bf_un.bf_node))->n_items;
#endif /* VMS */
	imax = filep->f_idxsize - 4;		/* max usable bytes	*/
/*
 * get previous item
 */
	used = offset = tp->t_offset;
	if (offset > 0)
	    {
	    bycopy(np, tempbuf, offset);	/* copy preceding items	*/
	    VOID itprevious(np, offset, nip, keydesc);
	    itpress(nip, ip, keydesc, 0);	/* compress new item	*/
	    }
	else itpress(ip, ip, keydesc, 1);	/* first item in node	*/
/*
 * do insert item
 */
	used = itpack(ip, tempbuf, used, keydesc);
/*
 * get next item
 */
	if (offset < tp->t_used)
	    {
	    offset = itunpack(np, offset, nip, keydesc);
	    if (offset == tp->t_used && treelev < treeleaf)
		bycopy(ip->it_key, nip->it_key, keydesc->k_len);
	    itpress(ip, nip, keydesc, 0);
	    used = itpack(nip, tempbuf, used, keydesc);
	    bycopy(np+offset, tempbuf+used, tp->t_used-offset);
	    used += tp->t_used-offset;
	    }
/*
 * if everything fits, move item over and insert new one
 */
	if (used <= imax)			/* room in this node ?	*/
	    {
	    bycopy(tempbuf, np, used);
	    bp->bf_used = used + 2;
	    bfdwrite(bp);
	    return;
	    }
/*
 * not enough room, split node
 * new node goes to the left of current node
 */
	node2 = allocrec(INDXREC);
	bp2 = bfgetclr(node2);
#ifndef ISKERNEL
	bfwrite(bp2);
	if (iserrno) return;
	bp2 = bfread(node2);
#endif
#ifndef VMS
	byfill(bp->bf_un.bf_addr, NODESIZE, 0);
	np2 = bp2->bf_un.bf_node->n_items;
#else /* VMS */
	byfill((char *) GSADDRESS(bp->bf_un.bf_addr), NODESIZE, 0);
	np2 = ((struct node *) GSADDRESS(bp2->bf_un.bf_node))->n_items;
#endif /* VMS */
	imin = (btlarge()) ? tp->t_used : (imax>>1);

	for (offset = 0; ; offset = nextoff)
	    {
	    nextoff = itunpack(tempbuf, offset, ip, keydesc);
	    if (nextoff >= imin) break;
	    }

	bycopy(tempbuf, np2, offset);
	used2 = offset;

	if (treelev < treeleaf)			/* internal node ?	*/
	    {
	    itpress(ip, ip, keydesc, 2);	/* yes, last item	*/
	    }
	used2 = itpack(ip, np2, used2, keydesc);

	itcopy(ip, nip, keydesc);		/* get new first item	*/
	offset = itunpack(tempbuf, nextoff, nip, keydesc);
	itpress(nip, nip, keydesc, 1);		/* expand first one	*/
	nextoff = itpack(nip, np, 0, keydesc);	/* insert first item	*/

	bycopy(tempbuf+offset, np+nextoff, used-offset);
	used += nextoff - offset;		/* move rest of right	*/

	bp->bf_used = used + 2;
	bp2->bf_used = used2 + 2;
	bp2->bf_level = bp->bf_level;
	bp2->bf_type = bp->bf_type;
	bfdwrite(bp2);
						/* pass up last key	*/
	ip->it_ptr = node2;			/* of left node		*/

	if (treelev == 0)			/* splitting root ?	*/
	    {					/* yes, keep same root	*/
	    node2 = allocrec(INDXREC);		/* get a new right half	*/
	    bp2 = bfgetclr(node2);
#ifndef VMS
	    bycopy(bp->bf_un.bf_addr, bp2->bf_un.bf_addr, NODESIZE);
#else /* VMS */
	    bycopy((char *) GSADDRESS(bp->bf_un.bf_addr),
		   (char *) GSADDRESS(bp2->bf_un.bf_addr), NODESIZE);
#endif /* VMS */
	    bp2->bf_used = bp->bf_used;
	    bp2->bf_level = bp->bf_level;
	    bp2->bf_type = bp->bf_type;
#ifndef VMS
	    byfill(bp->bf_un.bf_addr, NODESIZE, 0);
#else /* VMS */
	    byfill((char *) GSADDRESS(bp->bf_un.bf_addr), NODESIZE, 0);
#endif /* VMS */
	    itpress(ip, ip, keydesc, 1);	/* first item in root	*/
	    used = itpack(ip, np, 0, keydesc);
	    ip->it_ptr = node2;
	    itpress(ip, ip, keydesc, 2);	/* second item in root	*/
	    used = itpack(ip, np, used, keydesc);
	    bp->bf_level++;
	    bp->bf_used = used + 2;
	    bfdwrite(bp);
	    bfdwrite(bp2);
	    }
	else
	    {					/* pass middle item up	*/
	    bfdwrite(bp);
	    treelev--;
	    btinsert(ip, keydesc, nip);
	    treelev++;
	    }
}

/*
 * btlarge -- check that item to be inserted is largest in file
 *   node splitting is done differently when largest item
 *   is added, since your probably writing sequentially.
 *      returns (TRUE/FALSE)
 */

btlarge()
{
	register struct tree *tp;
	register int i;

	tp = &openp->o_trees[0];		/* check all levels	*/
	for (i=0; i<treeleaf; i++, tp++)
	    {
	    if (tp->t_nextoff != tp->t_used)	/* check internal nodes	*/
		return(0);
	    }
	return(tp->t_offset == tp->t_used);	/* check leaf node	*/
}


/*
 * remove item from btree node
 */

STATIC int btremove(keydesc, pip, nip)

register struct keydesc *keydesc;
register struct item *nip, *pip;
{
	struct buff *bp;
	register struct tree *tp;
	register char *np;
	register int offset, used;
/*
 * initialize variables etc.
 */
	tp = &openp->o_trees[treelev];		/* get tree pointer	*/
	bp = bfread(tp->t_node);
	if (bp == NULL) return;
#ifndef VMS
	np = bp->bf_un.bf_node->n_items;	/* get node pointer	*/
#else /* VMS */
	np = ((struct node *) GSADDRESS(bp->bf_un.bf_node))->n_items;
#endif /* VMS */
/*
 * get previous item
 */
	if (tp->t_offset != 0)			/* get previous item	*/
	    {
	    offset = itprevious(np, tp->t_offset, pip, keydesc);
	    itcopy(pip, nip, keydesc);
	    }
/*
 * get item to delete
 */
	VOID itunpack(np, tp->t_offset, nip, keydesc);
/*
 * get next item
 */
	used = tp->t_offset;
	if (tp->t_nextoff != tp->t_used)	/* next item exists ?	*/
	    {
	    offset = itunpack(np, tp->t_nextoff, nip, keydesc);
/*
 * recompress next item (note: it may be first in node now)
 */
	    if (tp->t_offset == 0)
		 itpress(nip, nip, keydesc, 1);	/* first item in node	*/
	    else itpress(pip, nip, keydesc, 0);
	    used = itpack(nip, np, used, keydesc);
	    bycopy(np+offset, np+used, tp->t_used-offset);
	    used += tp->t_used - offset;
	    }
	byfill(np+used, tp->t_used-used, 0);
/*
 * recalculate number of used bytes in node
 */
	bp->bf_used = used+2;			/* new used byte count	*/
	if (used == 0) bp->bf_level = 0;
	bfdwrite(bp);

	if (treelev == 0) return;		/* root node ?		*/

	if (used == 0)				/* empty node ?		*/
	    {
	    freerec(tp->t_node, INDXREC);	/* free index record	*/
	    treelev--;				/* delete upper level	*/
	    btremove(keydesc, pip, nip);
	    treelev++;
	    }
}

