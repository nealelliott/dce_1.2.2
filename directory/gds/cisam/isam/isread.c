/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isread.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:24  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:59  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:24  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:59:27  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:09:40  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:06:40  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:57:00  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:09  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:23:31  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  17:48:29  marrek]
 * 
 * Revision 1.1.2.3  1992/06/01  23:38:06  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:41:25  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/09  20:07:23  jim
 * 	Comment strings after #endif and #else statements.
 * 	[1992/04/09  19:32:44  jim]
 * 
 * Revision 1.1  1992/01/19  15:12:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isread.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:21 $";
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
 *  Title:	isread.c
 *  Description:
 *		isread()	-- read record from C-ISAM file
 *		isstart()	-- set index and position in C-ISAM file
 *
 ************************************************************************
 */


/*
 * isam read routines
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 */

#include <isdec.c>

/*
 * position current pointer and read record
 */

isread(isfd, record, mode)

int isfd;
char record[];
register int mode;
{
	register struct keydesc *curkey;
	register struct item *curitem;
	register int lockmode, dir, waited = 0;

retry:
	if (isenter(isfd, ISINPUT+LOCKSHARE+ISREAD) == -1) return(-1);

	PROFILE(pf_isread);
	lockmode = mode & LOCKMASK;		/* locking mode		*/
	switch(lockmode)
	    {
	    case 0:
		if (openp->o_mode & ISAUTOLOCK)
		    lockmode = ISLOCK;
		break;
	    case ISWAIT:
		if (openp->o_mode & ISAUTOLOCK)
		    lockmode += ISLOCK;
		break;
	    case ISLOCK:
	    case ISLCKW:
		break;
	    default:
		iserrno = EBADARG;
		return(isexit());
	    }

	curkey  = &openp->o_curkey;
	curitem = &openp->o_curitem;

	mode &= IOMASK;				/* sequence mode	*/
	switch(mode)
	    {
	    case ISFIRST:
	    case ISLAST:
	    case ISEQUAL:
	    case ISGREAT:
	    case ISGTEQ:
		kposition(record, 0, mode);	/* position first	*/
		break;

	    case ISNEXT:
	    case ISPREV:
	    case ISCURR:
		if (curkey->k_nparts == 0)	/* physical order ?	*/
		    {
		    treeflags = treelev = 0;
		    treeitem.it_ptr = curitem->it_ptr;
		    break;
		    }
		if (btcurrent() != 0)		/* setup current tree	*/
		    goto bad;
		break;

	    default:
		iserrno = EBADARG;
	    }

	if (iserrno) return(isexit());		/* any errors so far ?	*/

	if (mode == ISPREV || mode == ISLAST)
	    {					/* process previous	*/
	    dir = ISPREV;
	    if (mode == ISLAST || waited == 0)
		btmove(curkey, ISPREV);
	    }
	else dir = ISNEXT;

	if (mode == ISNEXT &&
	    ((openp->o_flags & OCURREAD) || (treeflags & BEGFILE)))
	    {					/* process next		*/
	    btmove(curkey, ISNEXT);
	    }

	openp->o_flags &= ~OCURREAD;		/* turn off curr read	*/

	if (treeflags & (BEGFILE|ENDFILE))
	    iserrno = EENDFILE;
	else
	    {
	    while (dataread(record, treeitem.it_ptr) != 0)
		{				/* find good record	*/
		if (mode == ISCURR) goto bad;
		if (mode == ISEQUAL) goto bad1;
		if (treeflags) goto bad3;
		btmove(curkey, dir);
		}

	    itcopy(&treeitem, curitem, curkey);	/* new current item	*/
	    if ((lockmode & ISLOCK) && lkrecord(curitem->it_ptr, LOCKCOND+ISREAD))
		{
#ifdef CREATLOCK
		goto bad2;
#else  /* CREATLOCK */
		if (! (lockmode & ISWAIT)) goto bad2;
		VOID isexit();
		VOID isbatch(2);
		if (lkrecord(curitem->it_ptr, LOCKUNCOND+ISREAD))
		    return(-1);
		waited = 1;
		mode += lockmode;		/* reset mode bits	*/
		goto retry;
#endif /* CREATLOCK */
		}
			/* check next key for duplicate - cobol necessity */
	    if (curkey->k_flags & ISDUPS)
		{
		btdups(curkey, dir);		/* check next key for dup */
		}

	    openp->o_flags |= OCURREAD;		/* read current record	*/
	    isrecnum = treeitem.it_ptr;		/* return record number	*/
	    if (lockmode & ISLOCK)
		{				/* save locked records	*/
#ifdef ISKERNEL
		tempisfd = -2;			/* special flag	to user	*/
		tempnum  = filep->f_datsize;	/* process, copy record	*/
#else
		bycopy(record, temprecord, filep->f_datsize);
		tempisfd = isfd;
		tempnum  = isrecnum;
#endif  /* ISKERNEL */
		}
	    }

	return(isexit());

bad:	if (!iserrno) iserrno = ENOCURR;
bad1:	if (!iserrno) iserrno = ENOREC;
bad2:	if (!iserrno) iserrno = ELOCKED;
bad3:	if (!iserrno) iserrno = EENDFILE;
	return(isexit());
}

/*
 * position current record
 */

isstart(isfd, keydesc, keylen, record, mode)

int isfd;
struct keydesc *keydesc;
int keylen;
char record[];
int mode;
{
	struct keydesc key;
	register struct keydesc *keyp = &key;

	if (isenter(isfd, ISINPUT+LOCKSHARE) == -1) return(-1);

	PROFILE(pf_isstart);
	kycopy(keydesc, keyp);
#ifdef SINIX	/* already current? TP */
    if (kycompare(keyp, &openp->o_curkey))
    {
#endif /* SINIX */
	if (keyp->k_nparts == 0)		/* physical key ?	*/
	    {
	    keyp->k_flags = 0;
	    keyp->k_len = 0;
	    }
	else if (kysearch(keyp, 0) <= 0)	/* search for key	*/
	    {
	    iserrno = EBADKEY;
	    goto err;
	    }

	kycopy(keyp, &openp->o_curkey);		/* set new current key	*/
#ifdef SINIX
    }
    else	/* return k_flags to user */
	keyp = &openp->o_curkey;
#endif /* SINIX */
	keydesc->k_flags = keyp->k_flags;	/* return k_flags to user */

	kposition(record, keylen, mode);	/* position current	*/

err:
	return(isexit());
}

/*
 * initialize tree position
 */

STATIC int kposition(record, keylen, mode)

char record[];
int keylen;
int mode;
{
	struct item item;
	register struct keydesc *keydesc;

	treeflags = treelev = 0;
	keydesc = &openp->o_curkey;

	switch(mode)
	    {
	    case ISFIRST:
	    case ISLAST:
		if (keydesc->k_nparts == 0)	/* physical read ?	*/
		    {
		    if (mode == ISLAST) dictread();
		    treeitem.it_ptr =
			(mode == ISFIRST) ? 1L :	/* first record	*/
			 ld4(dictp->d_nextdata) + 1L;	/* last record+1*/
		    }
		else btposition(keydesc, mode);
		break;
	    case ISEQUAL:
	    case ISGREAT:
	    case ISGTEQ:
		if (keydesc->k_nparts == 0)
		    {
		    treeitem.it_ptr = isrecnum + (mode == ISGREAT);
		    if (treeitem.it_ptr > ld4(dictp->d_nextdata))
			{
			dictread();		/* in case of old info	*/
			if (treeitem.it_ptr > ld4(dictp->d_nextdata))
			    iserrno = EENDFILE;
			}
		    break;
		    }
		itmake(record, &item, keydesc);
		btsearch(&item, keydesc, keylen, (mode == ISGREAT));
		if (treeflags & (BEGFILE|ENDFILE))
		    iserrno = ENOREC;
		if (mode == ISEQUAL &&
			itcompare(&item, &treeitem, keydesc, keylen) != 0)
		    iserrno = ENOREC;
		break;
	    default:
		iserrno = EBADARG;
		break;
	    }

	openp->o_flags &= ~(OCURPOS|OCURREAD);
	if (iserrno)
	    return;
	itcopy(&treeitem, &openp->o_curitem, keydesc);
					/* set new current item	*/
	openp->o_flags |= OCURPOS;
	isrecnum = treeitem.it_ptr;	/* return record number	*/
}
