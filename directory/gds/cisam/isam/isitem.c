/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isitem.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:17  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:54  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:13  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:58:31  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:04:13  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:05:31  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:50:33  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:47  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:37:25  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:40:48  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isitem.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:14 $";
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
 *  Title:	isitem.c
 *  Description:
 *		B-tree node item management routines.
 *
 ************************************************************************
 */


/*
 * btree node item management routines
 *  operates on items and btree nodes
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 */

#include <isdec.c>

/*
 * compare two items
 *  returns:
 *	-1  => item1 <  item2
 *	 0  => item1 == item2
 *	 1  => item1 >  item2
 */

STATIC int itcompare(item1, item2, keydesc, keylen)

struct item *item1;
struct item *item2;
struct keydesc *keydesc;
int keylen;
{
	register struct keypart *kp;	/* ptr to current key part	*/
	register int rel;
	register char *key1, *key2;
	register int i;
	int i1, i2;
	long l1, l2;
#ifndef NOFLOAT
	double f1, f2;
	double d1, d2;
	short nullf1, nullf2;
#endif

	if (keylen <= 0) keylen = keydesc->k_len;
	kp = &keydesc->k_part[0];
	rel = 0;
	key1 = item1->it_key;
	key2 = item2->it_key;

	for (i=keydesc->k_nparts; i > 0 && rel == 0 && keylen > 0; --i)
	    {					/* test each key part	*/
	    switch (kp->kp_type & TYPEMASK)
		{
		case CHARTYPE:
		    rel = bycmpr(key1, key2,	/* min(keylen, kp_leng)	*/
			((kp->kp_leng < keylen) ? kp->kp_leng : keylen));
		    break;
		case INTTYPE:
		    i1 = ldint(key1);
		    i2 = ldint(key2);
		    if      (i1 < i2) rel = -1;
		    else if (i1 > i2) rel = 1;
		    break;
		case LONGTYPE:
		    l1 = ldlong(key1);
		    l2 = ldlong(key2);
		    if      (l1 < l2) rel = -1;
		    else if (l1 > l2) rel = 1;
		    break;
#ifndef NOFLOAT
		case FLOATTYPE:
		    f1 = ldfltnull(key1, &nullf1);
		    f2 = ldfltnull(key2, &nullf2);
		    if (nullf1)	      rel = nullf2 ? 0 : -1;
		    else if (nullf2)  rel = 1;
		    else if (f1 < f2) rel = -1;
		    else if (f1 > f2) rel = 1;
		    break;
		case DOUBLETYPE:
		    d1 = lddblnull(key1, &nullf1);
		    d2 = lddblnull(key2, &nullf2);
		    if (nullf1)       rel = nullf2 ? 0 : -1;
		    else if (nullf2)  rel = 1;
		    else if (d1 < d2) rel = -1;
		    else if (d1 > d2) rel = 1;
		    break;
#endif /* NOFLOAT */
#ifdef CUSTCOLL
		default:		/* COLLATE[1234567]	*/
		    rel = clcompare(kp->kp_type & TYPEMASK, key1, key2,
			((kp->kp_leng < keylen) ? kp->kp_leng : keylen));
		    break;
#endif /* CUSTCOLL */
		}
	    if (kp->kp_type & ISDESC)	/* handle descending types	*/
		rel = -rel;

	    key1 += kp->kp_leng;
	    key2 += kp->kp_leng;
	    keylen -= kp->kp_leng;
	    ++kp;			/* point to next key part	*/
	    }
	
	return(rel);
}

/*
 * Pack item into dest
 */

STATIC int itpack(ip, np, offset, keydesc)

register struct item *ip;
char *np;
int offset;
register struct keydesc *keydesc;
{
	register char *dest;

	dest = np + offset;
	if ((ip->it_flags & DUP) == 0)		/* not a duplicate	*/
	    {
	    if (offset != 0) dest[-4] &= 0x7F;	/* turn off dup flag	*/
	    if (keydesc->k_flags & LCOMPRESS)	/* lead compression ?	*/
		st1(ip->it_leadcnt, dest++);	/* leading count	*/

	    if (keydesc->k_flags & TCOMPRESS)	/* trail compression ?	*/
		st1(ip->it_trailcnt, dest++);	/* trailing count	*/

	    bycopy(ip->it_key+ip->it_leadcnt, dest, ip->it_keylen);
	    dest += ip->it_keylen;
	    }
	else
	    {
	    if (offset != 0) dest[-4] |= 0x80;	/* turn on dup flag	*/
	    }

	if (keydesc->k_flags & ISDUPS)		/* duplicate number ?	*/
	    {
	    st2(CASTIN ip->it_dupnum, dest);
	    dest += 2;
	    }

	st4(ip->it_ptr, dest);			/* store pointer	*/
	if (ip->it_flags & NEXTDUP)		/* next one a dup ?	*/
	    *dest |= 0x80;

	return(offset + ip->it_totlen);		/* return next offset	*/
}

/*
 * Unpack source into item
 */

STATIC int itunpack(np, offset, ip, keydesc)

char *np;
int offset;
register struct item *ip;
register struct keydesc *keydesc;
{
	register char *src;
	register char *savesrc;

	savesrc = src = np + offset;
	if (offset != 0 && (src[-4] & 0x80))	/* last item => dup ?	*/
	    {
	    ip->it_flags = DUP;			/* yes, mark as dup	*/
	    ip->it_keylen = 0;
	    ip->it_leadcnt = 0;
	    ip->it_trailcnt = 0;
	    }
	else					 /* not a duplicate	*/
	    {
	    ip->it_flags = 0;
	    ip->it_leadcnt  = (keydesc->k_flags & LCOMPRESS)? ld1(src++): 0;
	    ip->it_trailcnt = (keydesc->k_flags & TCOMPRESS)? ld1(src++): 0;

	    ip->it_keylen = keydesc->k_len - ip->it_leadcnt - ip->it_trailcnt;
	    if (ip->it_keylen != 0)
		{
		bycopy(src, ip->it_key+ip->it_leadcnt, ip->it_keylen);
		src += ip->it_keylen;
		}
	    if (ip->it_trailcnt != 0)
		byfill(ip->it_key+ip->it_leadcnt+ip->it_keylen,
			ip->it_trailcnt, ' ');
	    }

	if (keydesc->k_flags & ISDUPS)		/* duplicate number ?	*/
	    {
	    ip->it_dupnum = ld2(src);
	    src += 2;
	    }
	else ip->it_dupnum = 0;

	if (*src & 0x80) ip->it_flags |= NEXTDUP;
	ip->it_ptr = ld4(src) & 0x7FFFFFFF;	/* load pointer		*/
	src += 4;

	ip->it_totlen = src - savesrc;
	return(offset + ip->it_totlen);		/* return next offset	*/
}

/*
 *  Compress item given previous item;
 */

STATIC int itpress(pip, ip, keyd, flag)

register struct item *pip;	/* previous item	*/
register struct item *ip;	/* item to compress	*/
struct keydesc *keyd;
int flag;	/* special compression flag		*/
		/* 0 => regular				*/
		/* 1 => first item in node (no pip)	*/
		/* 2 => last item in internal node	*/
{
	register int i;
	register struct keydesc *keydesc = keyd;

	if ((keydesc->k_flags & DCOMPRESS) &&	/* DCOMPRESS enabled ?	*/
	    (flag == 2 ||			/* and last item	*/
	     (flag == 0 &&			/*  or reg/equal items	*/
	      bycmpr(pip->it_key, ip->it_key, keydesc->k_len) == 0)))
	    {
	    ip->it_flags |= DUP;		/* make it a dup	*/
	    if (flag == 0) pip->it_flags |= NEXTDUP;
	    ip->it_totlen = ip->it_keylen = 0;
	    ip->it_leadcnt = ip->it_trailcnt = 0;
	    }
	else
	    {
	    ip->it_flags &= ~DUP;
	    if (flag == 0) pip->it_flags &= ~NEXTDUP;
	    ip->it_keylen = ip->it_totlen = keydesc->k_len;
	    if (keydesc->k_flags & TCOMPRESS)	/* trailing compression?*/
		{
		for (i=ip->it_totlen; i!=0; --i)
		    if (ip->it_key[i-1] != ' ') break;
		ip->it_trailcnt = ip->it_totlen - i;
		ip->it_totlen = i+1;
		}
	    else ip->it_trailcnt = 0;

	    if (keydesc->k_flags & LCOMPRESS)	/* leading compression? */
		{
		if (flag != 1)
		    {
		    for (i=0; i<ip->it_totlen-1; i++)
			if (ip->it_key[i] != pip->it_key[i]) break;
		    }
		else i=0;
		ip->it_leadcnt = i;
				/* bad things happen if this lead comp	*/
				/* overlaps previous trail compression	*/
				/* so, following 8 lines prevent this	*/
		i = keydesc->k_len;
		if (pip->it_flags & DUP)
		    {
		    for ( ; i!=0; --i)
			if (pip->it_key[i-1] != ' ') break;
		    }
		else i -= pip->it_trailcnt;
		if (ip->it_leadcnt > i) ip->it_leadcnt = i;
		ip->it_totlen -= ip->it_leadcnt - 1;
		}
	    else ip->it_leadcnt = 0;
	    ip->it_keylen -= ip->it_leadcnt + ip->it_trailcnt;
	    }

	if (keydesc->k_flags & ISDUPS)		/* duplicate number ?	*/
	    ip->it_totlen += 2;
	ip->it_totlen += 4;			/* 4 bytes for pointer	*/
}

/*
 * move offset to previous item
 */

STATIC int itprevious(np, offset, ip, keydesc)

char *np;
int offset;
struct item *ip;
register struct keydesc *keydesc;
{
	register int nextoff, prevoff;

	if ((keydesc->k_flags & COMPRESS) == 0)	/* if no compression	*/
	    {					/* calc previous offset	*/
	    prevoff = offset - keydesc->k_len - 
		((keydesc->k_flags & ISDUPS) ? 6 : 4);
	    }
	else prevoff = 0;

	for (;;)
	    {					/* unpack next item	*/
	    nextoff = itunpack(np, prevoff, ip, keydesc);
	    if (nextoff >= offset)		/* until we get there	*/
		break;
	    prevoff = nextoff;
	    }
	return(prevoff);
}

#ifndef SINIX	/* itcopy() as macro in btree.h */
/*
 * Copy an item from sip to dip
 */

STATIC int itcopy(sip, dip, keyd)

struct item *sip;		/* source item		*/
struct item *dip;		/* destination item	*/
struct keydesc *keyd;
{
	bycopy(CASTCP sip, CASTCP dip,
		((sizeof(struct item)) - KEYSIZE) + keyd->k_len);
}
#endif /* SINIX */

/*
 * Make an item given data record and key description
 */

STATIC int itmake(record, ip, keydesc)

char *record;			/* data record		*/
struct item *ip;		/* item to make		*/
struct keydesc *keydesc;	/* key description	*/
{
	register char *itemp;
	register struct keypart *kp;		/* current key part	*/
	register int i;

	kp = &keydesc->k_part[0];
	ip->it_flags = 0;
	ip->it_dupnum = 0;
	ip->it_ptr = 0L;
	itemp = ip->it_key;

	for (i=keydesc->k_nparts; i != 0; --i)
	    {					/* get each key part	*/
#ifdef ISKERNEL
	    usrmemory();			/* setup for kernel xfer*/
	    u.u_base = record + kp->kp_start;
	    u.u_count = kp->kp_leng;
	    iomove(itemp, u.u_count, B_WRITE);
#else
	    bycopy(record + kp->kp_start, itemp, kp->kp_leng);
#endif /* ISKERNEL */
	    itemp += kp->kp_leng;
	    ++kp;
	    }
}

