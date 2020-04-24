/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: iswrite.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:32  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:30:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:08  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:35  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:04:53  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:36:51  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:08:18  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:57:35  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:41  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:40:37  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:42:04  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: iswrite.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:28 $";
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
 *  Title:	iswrite.c
 *  Description:
 *		iswrite()	-- write record to C-ISAM file
 *		iswrcurr()	-- write record and make it current
 *		isrewrite()	-- update record of C-ISAM file
 *		isrewcurr()	-- update current record
 *		isrewrec()	-- update record by record number
 *		isdelete()	-- delete record of C-ISAM file
 *		isdelcurr()	-- delete current record
 *		isdelrec()	-- delete record by record number
 *
 ************************************************************************
 */


/*
 * isam write routines
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 *
 * Modification History:
 *   10/18/83  V201  RVH  set treelev = -1 on getrecord()
 */

#include <isdec.c>


/*
 * write new record
 */

iswrite(isfd, record)

int isfd;
char record[];
{
	long recnum = 0;

	if (isenter(isfd, ISOUTPUT) == -1) return(-1);

	wrtrecord(isfd, record, recnum, 0);

	return(isexit());
}

/*
 * write new current record
 */

iswrcurr(isfd, record)

int isfd;
char record[];
{
	long recnum = 0;

	if (isenter(isfd, ISOUTPUT) == -1) return(-1);

	wrtrecord(isfd, record, recnum, 1);

	if (!iserrno) openp->o_flags |= (OCURPOS|OCURREAD);

	return(isexit());
}



#ifndef SINIX
static char dmsg1[] =
    "\r\nThe limits of the INFORMIX Demo Version have been exceeded.\r\n";
static char dmsg2[] =
    "Please call Informix Software, Inc.\r\n";
static char dmsg3[] =
    "at (415) 322-4100 for licensing information.\r\n\n";
#endif /* SINIX */
#define wrterr(x)	write(2, x, stleng(x))

/*
 * write out record, update indexes
 */

STATIC int wrtrecord(isfd, record, recnum, currflag)

int isfd;
char record[];
long recnum;
int currflag;
{
	struct item item;
	struct keydesc key;
	register struct item *itemp = &item;
	register struct keydesc *keyp = &key;
	int duperr;
	register int keynum, lastnum;
	extern short demoversion;
	int flag = 0;

	PROFILE(pf_iswrite);
#ifndef SINIX
if (demoversion)
    if (ld4(dictp->d_nextdata) * filep->f_datsize > 7*1024)
	{
	wrterr(dmsg1);
	wrterr(dmsg2);
	wrterr(dmsg3);
	iserrno = EBADARG;
	return;
	}
#endif /* SINIX */

#ifndef NOAUDIT
	if (audopen()) return;			/* open audit trail	*/
#endif

#ifdef ISXACTION
	if (recnum)
	    flag = 1;
	else
#endif /* ISXACTION */
	    {
	    do
		{
		recnum = allocrec(DATAREC);	/* allocate data record	*/
		duperr = datacheck(recnum);	/* check for record used*/
		if (duperr == -1)
		    return;
		}
	    while (duperr);
	    }

#ifdef ISXACTION
	if (openp->o_mode & ISTRANS)
	    {
	    if (lkrecord(recnum, LOCKCOND))	/* leave record locked 	*/
		{
		freerec(recnum, DATAREC);
		return;
		}
	    }
	else
#endif /* ISXACTION */
	if (lkrecord(recnum, LOCKTEST))		/* record locked ?	*/
	    {
	    freerec(recnum, DATAREC);
	    return;
	    }

	duperr = 0;
	lastnum = ld2(dictp->d_nkeys);

redokeys:
	for (keynum=1; keynum<=lastnum ; keynum++)
	    {					/* go thru each key	*/
	    if (kysearch(keyp, keynum) <= 0)
		break;				/* done ?		*/
	    itmake(record, itemp, keyp);
	    itemp->it_ptr = recnum;
	    if (duperr == 0)
		{
		if (btadditem(itemp, keyp))
		    {
		    lastnum = keynum - 1;	/* duplicate, delete	*/
		    duperr = 1;			/*  items already added	*/
		    goto redokeys;
		    }
		if (currflag && kycompare(keyp, &openp->o_curkey) == 0)
		    itcopy(itemp, &openp->o_curitem, keyp);
		}
	    else
		{
		VOID btdelitem(itemp, keyp);
		}
	    }
#ifdef SINIX	/* physical order: set current item pointer. TP 20.10.87 */
	if (currflag && openp->o_curkey.k_nparts==0)
	    openp->o_curitem.it_ptr = recnum;
#endif /* SINIX */

	if (iserrno)
	    {
#ifdef ISXACTION
	    if (!flag)
#endif /* ISXACTION */
		freerec(recnum, DATAREC);	/* error, free record	*/
	    return;
	    }
#ifdef ISXACTION
	else
	    {
	    if (openp->o_mode & ISTRANS)	/* has transaction	*/
		VOID rldelete(&openp->o_tmpfree, recnum);
	    if (txflag != NOTRANS && !(openp->o_mode & ISNOLOG))
		if (iswimage(ISINSERT, isfd, recnum, filep->f_datsize,
			(char * )NULL, record))	/* log inserted record	*/
		    {
		    /* failed to write log record - must fix up btree
		     */
		    freerec(recnum, DATAREC);
		    goto redokeys;
		    }
	    }
#endif /* ISXACTION */
	if (datawrite(record, recnum))		/* ok, write out record	*/
	    {
	    VOID datadelete(recnum);
	    duperr = 1;				/* error, remove items	*/
	    goto redokeys;
	    }

	isrecnum = recnum;			/* return record number	*/
#ifndef NOAUDIT
	audoutput('a', recnum, record);		/* do audit trail	*/
#endif
}

/*
 * rewrite record (by primary key)
 */

isrewrite(isfd, record)

int isfd;
char record[];
{
	if (isenter(isfd, ISINOUT) == -1) return(-1);

	if (getrecord(record, isfd) != 0)
	    return(isexit());

	rewrecord(isfd, temprecord, record, treeitem.it_ptr, 0);
	return(isexit());
}

/*
 * rewrite current record
 */

isrewcurr(isfd, record)

int isfd;
char record[];
{
	if (isenter(isfd, ISINOUT) == -1) return(-1);

	if (getcurrent(isfd, 0L) != 0)
	    return(isexit());

	rewrecord(isfd, temprecord, record, openp->o_curitem.it_ptr, 1);
	return(isexit());
}

/*
 * rewrite record by record number
 */

isrewrec(isfd, recnum, record)

int isfd;
long recnum;
char record[];
{
	if (isenter(isfd, ISINOUT) == -1) return(-1);

	if (getcurrent(isfd, recnum) != 0)
	    return(isexit());

	rewrecord(isfd, temprecord, record, recnum, 0);
	return(isexit());
}

/*
 * rewrite record in isam file
 */

STATIC int rewrecord(isfd, record1, record2, recnum, currflag)

int isfd;
char *record1;
char *record2;
long recnum;
int currflag;
{
	struct item item1, item2;
	struct keydesc key;
	register struct keydesc *keyp = &key;
	register struct item *item1p = &item1;
	register struct item *item2p = &item2;
	register int keynum, lastnum;
	char *temprec;
	int duperr;	/* 0 = rewrite unique indexes		*/
			/* 1 = rewrite duplicate indexes	*/
			/* 2 = change back unique indexes	*/

	PROFILE(pf_isrewrite);
#ifndef NOAUDIT
	if (audopen()) return;			/* open audit trail	*/
#endif

	duperr = 0;
	lastnum = ld2(dictp->d_nkeys);

redokeys:
	for (keynum=1; keynum<=lastnum || duperr == 0; keynum++)
	    {					/* go thru each key	*/
	    if (keynum > lastnum)
		duperr = keynum = 1;

	    if (kysearch(keyp, keynum) <= 0)
		continue;			/* done ?		*/

	    if (((keyp->k_flags & ISDUPS) != 0) != (duperr == 1))
		continue;
	    
	    itmake(record1, item1p, keyp);
	    itmake(record2, item2p, keyp);
	    if (itcompare(item1p, item2p, keyp, 0) != 0)
		{				/* keys different ?	*/
		item1.it_ptr = recnum;		/* yes, replace them	*/
		item2.it_ptr = recnum;
		if (btadditem(item2p, keyp))
		    {
		    duperr = 2;			/* set error mode	*/
		    lastnum = keynum - 1;
		    temprec = record1;		/* exchange records 1,2	*/
		    record1 = record2;
		    record2 = temprec;
		    goto redokeys;
		    }
		VOID btdelitem(item1p, keyp);
		if (currflag && kycompare(keyp, &openp->o_curkey) == 0)
		    itcopy(item2p, &openp->o_curitem, keyp);
		}
	    }

	if (duperr != 2)
	    {
#ifdef ISXACTION
	    if (txflag != NOTRANS && !(openp->o_mode & ISNOLOG))
		if (iswimage(ISUPDATE, isfd, recnum, filep->f_datsize,
			record1, record2))	/* log update record	*/
		    {
		    duperr = 2;
		    goto redokeys;
		    }
#endif /* ISXACTION */
	    VOID datawrite(record2, recnum);	/* write out new record	*/
#ifndef NOAUDIT
	    audoutput('r', recnum, record1);	/* audit before image	*/
	    audoutput('w', recnum, record2);	/* audit after image	*/
#endif
	    }
}

/*
 * delete record (by primary key)
 */

isdelete(isfd, record)

int isfd;
char record[];
{
	if (isenter(isfd, ISINOUT) == -1) return(-1);

	if (getrecord(record, isfd) != 0)
	    return(isexit());

	delrecord(isfd, temprecord, treeitem.it_ptr);
	return(isexit());			/* remove from b-trees	*/
}

/*
 * delete current record
 */

isdelcurr(isfd)

int isfd;
{
	if (isenter(isfd, ISINOUT) == -1) return(-1);

	if (getcurrent(isfd, 0L) != 0)
	    return(isexit());

	delrecord(isfd, temprecord, openp->o_curitem.it_ptr);
	return(isexit());
}

/*
 * delete record by record number
 */

isdelrec(isfd, recnum)

int isfd;
long recnum;
{
	if (isenter(isfd, ISINOUT) == -1) return(-1);

	if (getcurrent(isfd, recnum) != 0)
	    return(isexit());

	delrecord(isfd, temprecord, recnum);
	return(isexit());
}

/*
 * delete record from isam file
 */

STATIC int delrecord(isfd, record, recnum)

int isfd;
char record[];
long recnum;
{
	struct keydesc key;
	struct item item;
	register struct keydesc *keyp = &key;
	register struct item *itemp = &item;
	register int keynum, lastnum;

	PROFILE(pf_isdelete);
#ifndef NOAUDIT
	if (audopen()) return;			/* open audit trail	*/
#endif

	lastnum = ld2(dictp->d_nkeys);

	for (keynum=1; keynum<=lastnum ; keynum++)
	    {					/* go thru each key	*/
	    if (kysearch(keyp, keynum) <= 0)
		break;				/* done ?		*/
	    itmake(record, itemp, keyp);
	    item.it_ptr = recnum;
	    VOID btdelitem(itemp, keyp);
	    }

#ifndef NOAUDIT
	audoutput('d', recnum, record);		/* do audit trail	*/
#endif

#ifdef ISXACTION
	if (txflag != NOTRANS)			/* has transaction 	*/
	    {
	    if (openp->o_mode & ISTRANS)	/* needed in transaction*/
		{
		/* when file is needed in transaction the record is
		 * not returned to global free list until commit time
		 */
		if (rlinsert(&openp->o_tmpfree, recnum))
		    {
		    iserrno = EBADMEM;
		    return;
		    }
		}
	    if (!(openp->o_mode & ISNOLOG))
		if (iswimage(ISDELETE, isfd, recnum, filep->f_datsize,
			record, (char *) NULL))	/* log deleted record	*/
		{
		iserrno = ELOGWRIT;
		return;
		}
	    }
#endif /* ISXACTION */
		
	VOID datadelete(recnum);
#ifdef ISXACTION
	if (!(openp->o_mode & ISTRANS))
#endif /* ISXACTION */
	    {
	    lkrecord(recnum, UNLOCK);		/* unlock the record	*/
	    freerec(recnum, DATAREC);		/* free data record	*/
	    }
}

/*
 * get record by primary key
 */

STATIC int getrecord(record, isfd)
char *record;
int isfd;
{
	struct keydesc key;
	struct item item;

	if (kysearch(&key, 1) <= 0)		/* get primary key	*/
	    goto bad;
	if ((key.k_flags & ISDUPS) || key.k_nparts == 0)
	    goto bad;
	itmake(record, &item, &key);		/* make item		*/

	if (isfd == tempisfd)		/* see if already in temprecord */
	    {
	    itmake(temprecord, &treeitem, &key);/* get key of temprecord*/
	    treeitem.it_ptr = tempnum;
	    if (itcompare(&item, &treeitem, &key, 0) == 0)
		goto good;			/* yes, keys match	*/
	    }
	btsearch(&item, &key, 0, 0);
	if (treeflags || treelev < 0)		/* V201 */
	    goto bad;
	treelev = -1;
	if (itcompare(&item, &treeitem, &key, 0) != 0)
	    goto bad;
	if (dataread(temprecord, treeitem.it_ptr) != 0)
	    goto bad;
good:
	isrecnum = treeitem.it_ptr;		/* get record number	*/
	if (lkrecord(isrecnum, (openp->o_mode&ISTRANS ? LOCKCOND : LOCKTEST)))
	    return(-1);				/* record locked ?	*/
	tempisfd = -1;
	return(0);
bad:
	if (!iserrno) iserrno = ENOREC;
	return(-1);
}


/*
 * get current record
 */

STATIC int getcurrent(isfd, recnum)
int isfd;
long recnum;
{
	if (recnum == 0L)
	    {				/* get current record	*/
	    if ((openp->o_flags & (OCURPOS|OCURREAD)) != (OCURPOS|OCURREAD))
		goto bad;
	    isrecnum = openp->o_curitem.it_ptr;
	    }
	else
	    {				/* get specified record	*/
	    if (recnum <= 0 || recnum > ld4(dictp->d_nextdata))
		goto bad;
	    isrecnum = recnum;
	    }

	if (lkrecord(isrecnum, (openp->o_mode&ISTRANS ? LOCKCOND : LOCKTEST)))
	    return(-1);			/* record locked ?	*/

	/* is record already in temprecord ?
	 *  if not read it in
	 */
	if (isfd != tempisfd || isrecnum != tempnum)
	    if (dataread(temprecord, isrecnum) != 0)
		goto bad;

	tempisfd = -1;			/* mark temprecord destroyed	*/
	return(0);
bad:
	if (!iserrno) iserrno = (recnum == 0L ? ENOCURR : ENOREC);
	return(-1);
}

