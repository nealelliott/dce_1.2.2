/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: iskey.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:18  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:55  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:15  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:58:40  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:05:10  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:05:41  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:50:35  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:51  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:22:56  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  17:47:58  marrek]
 * 
 * Revision 1.1.2.3  1992/06/01  23:37:30  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:40:55  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/09  20:06:51  jim
 * 	Comment strings after #endif and #else statements.
 * 	[1992/04/09  19:32:05  jim]
 * 
 * Revision 1.1  1992/01/19  15:12:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: iskey.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:16 $";
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
 *  Title:	iskey.c
 *  Description:
 *		Key description node management routines.
 *		Operates on key descriptions and key nodes.
 *
 ************************************************************************
 */


/*
 * key description node management routines
 *  operates on key descriptions and key nodes
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 */

#include <isdec.c>

static int typelen[]  =
    {
	0		/* CHARTYPE */
	,INTSIZE	/* INTTYPE */
	,LONGSIZE	/* LONGTYPE */
#ifndef NOFLOAT
	,DOUBLESIZE	/* DOUBTYPE */
	,FLOATSIZE	/* FLOTYPE */
#endif
    };

/*
 * search through key descriptions in index file
 *    keynum =  0  => find matching keydesc
 *    keynum = -1  => find and delete keydesc
 *    keynum = 1..n  => find nth keydesc
 * returns  n => key decription found, rootnode loaded, n = keynum
 *	   -1 => not found
 *	    n => delete performed
 */

STATIC int kysearch(keyd, keynum)
struct keydesc *keyd;
int keynum;
{
	register struct keyn *keyn;
	struct keydesc tmpkey;
	register int offset, used;
	register struct buff *bp;
	register int size, num;
	long node;

	if (keynum <= 0 && kylegal(keyd, filep->f_datsize) == -1)
	    return(-1);

	num = 0;
	for (node = ld4(dictp->d_keysnode); node != 0; )
	    {
	    bp = bfread(node);			/* read key node	*/
	    if (bp == NULL) break;
#ifndef VMS
	    keyn = bp->bf_un.bf_keyn;
#else /* VMS */
	    keyn = (struct keyn *) GSADDRESS(bp->bf_un.bf_keyn);
#endif /* VMS */
	    used = bp->bf_used - 6;
	    offset = 0;
	    while (offset != used)
		{
		num++;				/* next key number	*/
		size = ld2(keyn->kn_keys+offset);
		kyunpack(keyn->kn_keys+offset, &tmpkey);
		if (keynum <= 0)
		    {
		    if (kycompare(keyd, &tmpkey) == 0)
			{
			if (keynum == 0)
			    {			/* found => return info	*/
			    keyd->k_flags = tmpkey.k_flags;
			    keyd->k_rootnode = tmpkey.k_rootnode;
			    keyd->k_len = tmpkey.k_len;
			    bfrelease(bp);
			    return(num);
			    }
			else
			    {			/* delete key desc	*/
			    used -= size;	/* and recompact node	*/
			    bycopy(keyn->kn_keys+offset+size,
				keyn->kn_keys+offset, used-offset);
			    byfill(keyn->kn_keys+used, size, 0);
			    bp->bf_used = used+6;
			    bfdwrite(bp);
			    offset = ld2(dictp->d_nkeys) - 1;
			    st2(offset, dictp->d_nkeys);
			    dictmodify();
			    return(num);
			    }
			}
		    }
		else
		    {
		    if (keynum == num)
			{
			kycopy(&tmpkey, keyd);
			bfrelease(bp);
			return(keynum);
			}
		    }
		offset += size;
		}
	    node = ld4(keyn->kn_next);
	    bfrelease(bp);
	    }
	return(-1);
}

/*
 * compare two key descriptions
 * returns 0 => keys identical
 *	  -1 => keys different
 */

STATIC int kycompare(kd1, kd2)

register struct keydesc *kd1;
struct keydesc *kd2;
{
	register struct keypart *kp1;
	register struct keypart *kp2;
	register int i;

	if (kd1->k_nparts != kd2->k_nparts)	/* num parts must match	*/
	    return(-1);

	kp1 = &kd1->k_part[0];
	kp2 = &kd2->k_part[0];

	for (i=0; i<kd1->k_nparts; i++)
	    {
	    if ((kp1->kp_start != kp2->kp_start) ||
		(kp1->kp_leng  != kp2->kp_leng)  ||
		(kp1->kp_type  != kp2->kp_type))
			return(-1);
	    ++kp1;
	    ++kp2;
	    }
	return(0);
}

/*
 * pack a key description into a key node
 */

STATIC int kypack(keyd, keyp)
register struct keydesc *keyd;
register char *keyp;
{
	register struct keypart *kp;
	register int i;

	st2((keyd->k_nparts*5) + 7, keyp);
	st4(keyd->k_rootnode, keyp+2);
	st1(keyd->k_flags >> 1, keyp+6);
	keyp += 7;

	if (keyd->k_flags & ISDUPS)		/* store dups flag	*/
	    keyd->k_part[0].kp_leng |= 0x8000;

	kp = &keyd->k_part[0];
	for (i=0; i<keyd->k_nparts; i++)	/* store each part	*/
	    {
	    st2(kp->kp_leng, keyp);
	    st2(kp->kp_start, keyp+2);
	    st1(kp->kp_type, keyp+4);
	    keyp += 5;
	    ++kp;
	    }
	keyd->k_part[0].kp_leng &= ~0x8000;	/* restore dups flag	*/
}

/*
 * unpack a key description from a key node
 */

STATIC int kyunpack(keyp, keyd)
register char *keyp;
register struct keydesc *keyd;
{
	register struct keypart *kp;
	register int i;

	keyd->k_nparts = (ld2(keyp)-7) / 5;
	keyd->k_rootnode = ld4(keyp+2);
	keyd->k_len = 0;
	keyd->k_flags = (ld1(keyp+6)) << 1;
	keyp += 7;

	kp = &keyd->k_part[0];
	for (i=0; i<keyd->k_nparts; i++)
	    {
	    kp->kp_leng  = ld2(keyp);
	    keyd->k_len += (kp->kp_leng & 0x7FFF);
	    kp->kp_start = ld2(keyp+2);
	    kp->kp_type  = ld1(keyp+4);
	    keyp += 5;
	    ++kp;
	    }
	if (keyd->k_nparts)
	    if (keyd->k_part[0].kp_leng & 0x8000)
		keyd->k_flags |= ISDUPS;
	keyd->k_part[0].kp_leng &= 0x7FFF;
}

/*
 * check for legal key description
 *  returns  0 => good key description
 * 	    -1 => illegal key description
 */

STATIC int kylegal(keyd, datsize)
register struct keydesc *keyd;
register int datsize;
{
	register struct keypart *kp;
	register int i, type;

	if (keyd->k_nparts <= 0 || keyd->k_nparts > NPARTS)
	    goto bad;				/* too many parts ?	*/

	kp = &keyd->k_part[0];
	keyd->k_len = 0;

	for (i=0; i<keyd->k_nparts; i++)
	    {
	    if (kp->kp_start < 0 || kp->kp_start > datsize ||
		kp->kp_leng < 0 || kp->kp_leng > datsize ||
#ifndef SINIX
		kp->kp_leng < 0 || kp->kp_leng > datsize ||
#else /* SINIX: do not allow length 0; Schafer */
		kp->kp_leng <= 0 || kp->kp_leng > datsize ||
#endif /* SINIX */
		kp->kp_start+kp->kp_leng > datsize)
		goto bad;			/* out of bounds	*/
	    type = kp->kp_type & TYPEMASK;
	    if (
#ifdef CUSTCOLL
		(!clchktype(type)) &&
#endif /* CUSTCOLL */
		(type >= ((sizeof(typelen))/sizeof(int)) ||
		! (typelen[type] == 0 ||
		   typelen[type] == kp->kp_leng)))
		goto bad;			/* bad type		*/
	    keyd->k_len += kp->kp_leng;
	    kp++;
	    }
	if (keyd->k_len > KEYSIZE)		/* key too big		*/
	    goto bad;
	return(0);
bad:
	iserrno = EBADKEY;
	return(-1);
}

#ifndef SINIX	/* kycopy() as macro in btree.h */
/*
 * copy key description
 */

STATIC int kycopy(src, dst)
struct keydesc *src;
struct keydesc *dst;
{
	bycopy(CASTCP src, CASTCP dst, sizeof(struct keydesc));
}
#endif /* SINIX */

/*
 * add a key description
 */

STATIC int kyadd(keyd)
struct keydesc *keyd;
{
	register struct keyn *keyn;
	register int used, size;
	register struct buff *bp;
	register long node;

	if (keyd->k_nparts != 0)
	    {
	    if (kysearch(keyd, 0) != -1)	/* already exist ?	*/
		{
		iserrno = EKEXISTS;		/* yes, error		*/
		return(-1);
		}

	    keyd->k_rootnode = allocrec(INDXREC);/* allocate a rootnode	*/
	    bp = bfgetclr(keyd->k_rootnode);
	    bp->bf_used = 2;
	    bp->bf_level = 0;
	    bp->bf_type = 0;
	    bfdwrite(bp);
	    }
	else keyd->k_rootnode = 0;

	node = ld4(dictp->d_keysnode);
	size = keyd->k_nparts*5 + 7;

	while(1)
	    {
	    bp = bfread(node);			/* read key node	*/
	    if (bp == NULL) break;
#ifndef VMS
	    keyn = bp->bf_un.bf_keyn;
#else /* VMS */
	    keyn = (struct keyn *) GSADDRESS(bp->bf_un.bf_keyn);
#endif /* VMS */
	    used = bp->bf_used - 6;
	    if (bp->bf_used+size < filep->f_idxsize-2)
		{				/* will it fit ?	*/
		kypack(keyd, keyn->kn_keys+used);
		used += size;			/* put it at end of node*/
		bp->bf_used = used+6;
		bfdwrite(bp);
		used = ld2(dictp->d_nkeys) + 1;	/* inc number of keys	*/
		st2(used, dictp->d_nkeys);
		dictmodify();
		return(0);
		}
	    node = ld4(keyn->kn_next);
	    if (node == 0)			/* need a new node ?	*/
		{
		node = allocrec(INDXREC);	/* yes, allocate one	*/
		st4(node, keyn->kn_next);
		bfdwrite(bp);
		bp = bfgetclr(node);
		bp->bf_used = 6;
		bp->bf_level = 0x7E;
		bp->bf_type = 0xFF;
		bfdwrite(bp);
		}
	    else bfrelease(bp);
	    }
	return(-1);
}

/*
 * delete a key description
 *   returns 0 => key deleted
 *	    -1 => error (not found)
 */

STATIC int kydelete(keyd)
struct keydesc *keyd;
{
	if (kysearch(keyd, -1) > 0)		/* find and delete key	*/
	    return(0);

	iserrno = EBADKEY;
	return(-1);				/* error		*/
}

