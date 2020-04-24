/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: istrans.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:27  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:30:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:02  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:28  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:15:40  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:12:18  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:07:18  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:57:06  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:19  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:24:23  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  17:49:09  marrek]
 * 
 * Revision 1.1.2.3  1992/06/01  23:38:19  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:41:36  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/09  20:07:56  jim
 * 	Comment strings after #endif and #else statements.
 * 	[1992/04/09  19:33:35  jim]
 * 
 * Revision 1.1  1992/01/19  15:12:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: istrans.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:23 $";
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
 *  Title:	isxaction.c
 *  Description:
 *		C-ISAM transaction routines
 *
 ***************************************************************************
 */


#include <isdec.c>
#include <istrans.h>


int xid = 0;				/* transaction id */
int uid;				/* user id */
int svptnum = 0;			/* save point number */
long prevlog;				/* previous record */
int prevlen;				/* previous length */
long logaddr;				/* log address */
struct filelist *openfile = NULL;	/* list of open files */
struct txlist *opentrans;		/* list of open transactions */
int lbuffsize;				/* log buffer size */
char *lbuff = NULL;			/* log buffer */

short txflag = NOTRANS;			/* transaction flag */
short svtxflag;				/* trans flag when suspending log */
#ifndef SINIX
short islogisfd;			/* log file's isfd */
#else /* SINIX, TP 11.03.88 */
short islogisfd = -1;			/* log file's isfd */
#endif /* SINIX */


long time();
char *realloc();
extern int iserrno;

#ifdef ISXACTION


/* getlogtype
 *    function: 1. returns the type of the log record
 *		2. returns -1 record type unknown
 */
getlogtype(logbuff)
    char *logbuff;
{
    switch (logbuff[1])
	{
	case 'C':
	    if (*logbuff == 'F')			/* close file */
		return LG_FCLOSE;
	    break;
	case 'E':
	    switch (*logbuff)
		{
		case 'D':
		    return LG_DELETE;			/* delete */
		case 'R':
		    return LG_RENAME;			/* rename */
		}
	    break;
	case 'I':
	    switch (*logbuff)
		{
		case 'C':
		    return LG_CINDEX;			/* create index */
		case 'D':
		    return LG_DINDEX;			/* drop index */
		}
	    break;
	case 'L':
	    if (*logbuff == 'C')
		return LG_CLUSIDX;
	    break;
	case 'N':
	    switch (*logbuff)
		{
		case 'E':				/* end of file */
		    return LG_EOF;
		case 'I':				/* insert */
		    return LG_INSERT;
		case 'U':				/* get unique id */
		    return LG_UNIQUEID;
		}
	    break;
	case 'O':
	    if (*logbuff == 'F')			/* open file */
		return LG_FOPEN;
	    break;
	case 'P':
	    switch (*logbuff)
		{
		case 'C': return LG_CKPOINT;		/* checkpoint */
		case 'S': return LG_SAVEPT;		/* savepoint */
		case 'U': return LG_UPDATE;		/* update */
		}
	    break;
	case 'R':
	    if (*logbuff == 'E')
		return LG_ERASE;			/* erase file */
	    break;
	case 'S':
	    if (*logbuff == 'V')
		return LG_VERSION;			/* version */
	    break;
	case 'U':
	    switch (*logbuff)
		{
		case 'B':
		    return LG_BUILD;			/* build file */
		case 'S':
		    return LG_SETUNIQID;		/* set unique id */
		}
	    break;
	case 'W':
	    switch (*logbuff)
		{
		case 'B': return LG_BEGWORK;		/* begin work */
		case 'C': return LG_COMWORK;		/* commit work */
		case 'R': return LG_ROLWORK;		/* rollback work */
		}
	    break;
	}
    iserrno = EBADLOG;					/* bad log record */
    return LG_ERROR;					/* unknown log type */
}



/* writelog
 *    function: 1. writes log record out
 *
 *    return:  0 - aok
 *            -1 - error (see iserrno)
 */
writelog(buff, bsize, linked)
    char *buff;
    int bsize;
    int linked;
{
    register long loc;
#ifndef VMS
    long lseek();
#endif /* VMS */

    if (lkloglog(LOCKUNCOND) == -1)
	goto bad1;

#ifndef VMS
    if ((loc = lseek(islogisfd, 0L, 2)) == -1)		/* append */
#else /* VMS */
    if ((loc = rmslseek(islogisfd, 0L, 2)) == -1)
#endif /* VMS */
	goto bad;

    if (linked)
	{					/* rollback-able */
	stlong(prevlog, buff+LG_PREV);
	stint(prevlen, buff+LG_PREVLEN);
	prevlog = loc;				/* save for the next guy */
	prevlen = bsize;
	}
    else
	{					/* rollforward only */
	stlong(0L, buff+LG_PREV);
	stint(0, buff+LG_PREVLEN);
	}
    stint(bsize, buff+LG_LEN);			/* ptr for rollforward */
    stint(bsize, buff+bsize-2);			/* ptr for rollback */
#ifndef VMS
    if (write(islogisfd, buff, bsize) != bsize)
	goto bad;
#else /* VMS */
    if (rmswrite(islogisfd, buff, bsize) != bsize)
	goto bad;
    loc = rfatooff(islogisfd);
    if(linked)
	prevlog = loc;
#endif /* VMS */

    if (lkloglog(UNLOCK) == -1)
	goto bad;
    return 0;

bad:
    VOID lkloglog(UNLOCK);
bad1:
    iserrno = ELOGWRIT;
    return -1;
}



/* readlog
 *    function: 1. read in a buffer full of log records
 *
 *    return: -1 read failed, see iserrno
 *             n number of bytes actually read in
 */
readlog(buff, bsize, lstart)
    char *buff;
    int bsize;
    long lstart;
{
    int i;

#ifndef VMS
    lseek(islogisfd, lstart, 0);		/* position log file */
    if ((i = read(islogisfd, buff, bsize)) == -1)
#else /* VMS */
    if ((i = rmsfillbf(islogisfd, lstart, buff, bsize)) == -1)
#endif /* VMS */
	{					/* read error */
	iserrno = ELOGREAD;
	return -1;
	}
    return i;
}



/* getrisfd
 *    function: 1. goes through open file list to get isfd
 *
 *    return: isfd - found
 *	      -1 - not found
 *	      -2 - file erased during transaction
 */
getrisfd(isfd)
    int isfd;
{
    register struct filelist *next;

    /* if the database has been accessed through sql then systables
     * always has isfd of 0.  if the database has been accessed thru
     * an isam program then isfd 0 can be any file and must go thru
     * the openfile list to find the correct isam file to rollforward
     */
    for (next = openfile; next; next = next->fl_next)
	if (next->fl_oisfd == isfd)		/* found the file */
	    {
	    if (next->fl_flag & FL_ERASED)	/* erased in transaction */
		return -2;
	    return next->fl_risfd;
	    }
    if (isfd == 0)				/* systables */
	return 0;
    return -1;
}


#ifndef VMS /* VMS version of this routine is below */ 

/* isgetprev
 *    function: 1. reads in new log page (backwards) if current page 
 *		   does not contain requested log record
 *
 *    returns: LG_ERROR - error (see iserrno)
 *	       pointer to request log record in buffer
 */
char *isgetprev(prev, len)
    long prev;
    int len;
{
    if (prev < logaddr)
	{
	/* exhausted current log page
	 */
	if (len > lbuffsize)
	    {					/* needs a bigger buffer */
	    if ((lbuff = realloc(lbuff, (unsigned) len)) == NULL)
		{
		iserrno = EBADMEM;		/* no memory */
		return (char *) LG_ERROR;
		}
	    else
	       lbuffsize = len;
	    }
	logaddr = prev + len - lbuffsize;
	if (logaddr < 0)
	    logaddr = 0;
	if (readlog(lbuff, lbuffsize, logaddr) <= 0)
	    return LG_ERROR;
	}
    return lbuff + prev - logaddr;
}
#else /* VMS */

/* isgetprev
 *    function: 1. reads one record (backwards) if current record 
 *		   does not contain requested log record
 *
 *    returns: LG_ERROR - error (see iserrno)
 *	       pointer to request log record in buffer
 */
char *isgetprev(prev, len)
    long prev;
    int len;
{


	if (len > lbuffsize)
	    {					/* needs a bigger buffer */
	    if ((lbuff = realloc(lbuff, (unsigned) len)) == NULL)
		{
		iserrno = EBADMEM;		/* no memory */
		return (char *) LG_ERROR;
		}
	    else
	       lbuffsize = len;
	    }
	if(rmslseek(islogisfd, prev, 0))
		return LG_ERROR;

	if (rmsread(islogisfd, lbuff, len) <= 0)
	    return LG_ERROR;
    return lbuff;
}
#endif /* VMS */


/* txclose
 *    function: 1. closes all files in a transaction
 *		2. turns off transaction flag first so no logging is done
 *		3. free memory
 *
 *    returns:  0 - aok
 *	       -1 - error
 */
txclose(flag)
    register int flag;
{
    register struct filelist *next;

    for (next = openfile; next; next = next->fl_next)
	{
	if (next->fl_path == NULL)
	    continue;
	if (isenter(next->fl_oisfd, next->fl_mode))
	    return -1;
	while (openp->o_tmpfree)		/* fix up free list */
	    {
	    if (flag == LG_COMWORK)
		freerec(openp->o_tmpfree->rl_recnum, DATAREC);
	    VOID rldelete(&openp->o_tmpfree, openp->o_tmpfree->rl_recnum);
	    }
	VOID isexit();

	openp->o_mode &= ~ISTRANS;		/* immediate close */
	if (next->fl_flag & (FL_CLOSED + FL_ERASED))
	    {
	    /* if the file is closed during transaction it can
	     * be closed now
	     */
	    VOID isclose(next->fl_oisfd);
	    next->fl_path = NULL;		/* slot available */
	    next->fl_flag = 0;
	    }
	else
	    {
	    if (next->fl_path)			/* file still open */
		VOID isrelease(next->fl_oisfd);	/* just release locks */
	    openp->o_mode |= ISTRANS;
	    }
	}
    return 0;
}



/* stkeyparts
 *    function: 1. store key parts in loc record
 *
 */
stkeyparts(key, loc, klen)
    struct keydesc *key;
    char *loc;
    int *klen;
{
    register int i;

    for (i = 0; i < key->k_nparts; i++)
	{					/* store key parts */
	stint(key->k_part[i].kp_start, loc);
	loc += INTSIZE;
	stint(key->k_part[i].kp_leng, loc);
	loc += INTSIZE;
	stint(key->k_part[i].kp_type, loc);
	loc += INTSIZE;
	}
    *klen = key->k_nparts * 6;			/* 6 bytes per part */
}



/* ldkeyparts
 *    function: 1. load key description from log record
 */
ldkeyparts(rec, keyp)
    char *rec;
    struct keydesc *keyp;
{
    register int i;

    for (i = 0; i < keyp->k_nparts; i++)
	{
	keyp->k_part[i].kp_start = ldint(rec);
	rec += INTSIZE;
	keyp->k_part[i].kp_leng = ldint(rec);
	rec += INTSIZE;
	keyp->k_part[i].kp_type = ldint(rec);
	rec += INTSIZE;
	}
}



/* islogopen
 *    function: 1. opens log file for database
 *
 *    return:  0 - aok
 *	      -1 - error (see iserrno)
 */
islogopen(logname)
    char *logname;
{
    register struct filelist *opflp;

    vclose();
#ifndef VMS
    if ((islogisfd = open(logname, 2)) == -1)
#else /* VMS */
    if ((islogisfd = rmsopen(logname, 2)) == -1)
#endif /* VMS */
	{
	iserrno = ELOGOPEN;			/* cannot open log	*/
	return -1;
	}
    lbuffsize = LG_PAGESIZE;
    if (!lbuff)
	if ((lbuff = malloc(LG_PAGESIZE)) == NULL)
	    {
	    iserrno = EBADMEM;			/* no memory		*/
	    return -1;
	    }

    /* allocate list structure for storing all open files during
     * a transaction.  files opened for output are not closed
     * until end of transaction.  this list keeps track of all such
     * files
     */
    if (!openfile)
	{
	if ((openfile = (struct filelist *)
		malloc(sizeof(struct filelist) * NOPNFL)) == NULL)
	    {
	    iserrno = EBADMEM;			/* no memory */
	    return -1;
	    }
	byfill(openfile, sizeof(struct filelist) * NOPNFL, 0);
	for (opflp = openfile; opflp < openfile+NOPNFL-1; opflp++)
	    {
	    opflp->fl_path = NULL;
	    opflp->fl_next = opflp+1;
	    }
	opflp->fl_path = NULL;
	}

    xid = getpid();				/* new transaction id	*/
    stint(xid, lbuff+LG_XID);
    uid = getuid();
    stint(uid, lbuff+LG_USER);			/* store user id	*/
    txflag = STRANS;				/* singleton transactio	*/
    return 0;
}



/* islogclose
 *    function: 1. closes any open files left behind by the program
 *		2. closes log file
 *		3. reset transaction flag
 *
 */
islogclose()
{
    register struct filelist *oplp;
#ifdef SINIX	/* log file not opened. TP 22.07.88 */
    int retval;
    if (islogisfd == -1)
    {
	iserrno = ENOTRANS; return -1;
    }
#endif /* SINIX */

    for (oplp = openfile; oplp; oplp = oplp->fl_next)
	{
	if (oplp->fl_path)			/* file still open */
	    {
	    VOID isenter(oplp->fl_oisfd, oplp->fl_mode);
	    openp->o_mode &= ~ISTRANS;		/* immediate close */
	    VOID isexit();
	    VOID isclose(oplp->fl_oisfd);
	    }
	oplp->fl_path = NULL;			/* slot available */
	}
    txflag = NOTRANS;
#ifndef VMS
#ifndef SINIX	
    return close(islogisfd);
#else /* SINIX: mark log file closed. TP 22.07.88 */
    retval = close(islogisfd);
    islogisfd = -1;
    return retval;
#endif /* SINIX */
#else /* VMS */
    return rmsclose(islogisfd);
#endif /* VMS */
}


/* issusplog
 *    function: 1. suspends logging
 */
issusplog()
{
    svtxflag = txflag;
    txflag = NOTRANS;
}


/* isresumlog
 *    function: 1. resumes logging
 */
isresumlog()
{
    txflag = svtxflag;
}



/* isbegin
 *    function: 1. generate transaction id
 *		2. write "BEGIN WORK" record in log file
 *
 *    return: 0 - aok
 *	      -1 - error (see iserrno)
 */
isbegin()
{
    long t;

#ifdef	SHMLOCK
    if ((!shminit) && (shmsetup()))
	return(-1);
#endif	/* SHMLOCK */
    switch (txflag)
	{
	case NOTRANS:				/* no transaction */
	    iserrno = ENOTRANS;
	    return -1;
	case STRANS:				/* singleton transaction */
	    txflag = TRANS;
	/* case TRANS: ignore for multiple begin works
	 */
	}
    stchar("BW", lbuff+LG_TYPE, 2);		/* record type */
    time(&t);
    stlong(t, lbuff+LG_TIME);			/* time stamp */
    return writelog(lbuff, LG_TXSIZE, TRUE);
}



/* iscommit
 *    function: 1. get time
 *		2. write commit work record
 *
 *    return: 0  - aok
 *	      -1 - error (see iserrno)
 */
iscommit()
{
    long t;

    switch (txflag)
	{
	case NOTRANS:				/* no transaction */
	    iserrno = ENOTRANS;
	    return -1;
	case STRANS:				/* nothing to commit */
	    iserrno = ENOBEGIN;
	    return -1;
	}
    txflag = STRANS;
    PROFILE(pf_iscommit);
    if (txclose(LG_COMWORK))
	return -1;
    stchar("CW", lbuff+LG_TYPE, 2);		/* record type */
    time(&t);
    stlong(t, lbuff+LG_TIME);			/* time stamp */
    return writelog(lbuff, LG_TXSIZE, TRUE);
}



/* isrollback
 *    function: 1. get time
 *		2. write rollback work record
 *
 *    return: 0  - aok
 *	      -1 - error (see iserrno)
 */
isrollback()
{
    register int len;				/* previous length */
    register long prev;				/* previous pointer */
    long t;					/* time */
    register char *curr;			/* current log position */
    int logtype;				/* log record type */

    switch (txflag)
	{
	case NOTRANS:				/* no transaction */
	    iserrno = ENOTRANS;
	    return -1;
	case STRANS:				/* no transaction started */
	    iserrno = ENOBEGIN;
	    return -1;
	}

    PROFILE(pf_isrollback);
    /* save previous pointer and length for threading
     * back to BEGIN WORK
     */
    prev = prevlog;
    len = prevlen;
    logaddr = prev + 1;				/* init log address */
    txflag = NOTRANS;				/* no logging for rollback */

    for ( ; ; )
	{
	curr = isgetprev(prev, len);
	if (curr == LG_ERROR)			/* read err or BOF before BW */
	    goto bad;

	logtype = getlogtype(curr + LG_TYPE);
	switch (logtype)
	    {
	    case LG_ERROR:
		goto bad;
	    case LG_BEGWORK:			/* BEGIN WORK */
		/* write ROLLBACK WORK record
		 */
		txflag = STRANS;		/* back to singleton trans */
		stint(xid, lbuff+LG_XID);
		stint(uid, lbuff+LG_USER);
		time(&t);
		stlong(t, lbuff+LG_TIME);	/* time stamp */
		if (txclose(LG_ROLWORK))
		    goto bad;
		stchar("RW", lbuff+LG_TYPE, 2);	/* record type */
		if (writelog(lbuff, LG_TXSIZE, TRUE)) 
		    goto bad;
		return 0;			/* all done */
	    case LG_DELETE:			/* deleted record */
		if (lginsert(curr, LG_ROLWORK))	/* reinsert it */
		    goto bad;
		break;
	    case LG_INSERT:			/* newly inserted record */
		if (lgdelete(curr))		/* delete it */
		    goto bad;
		break;
	    case LG_UPDATE:			/* updated record */
		if (lgupdate(curr, LG_ROLWORK))	/* un-update it */
		    goto bad;
#ifdef ISSVPT
	    case LG_SVPOINT:
		/* savepoint record see if it is the right one to stop
		 */
		if (spnum == -1)		/* rollback to begin work */
		    break;
		if (lgsavept(spnum, curr))
		    {
		    /* must put in rollback to savepoint record
		     * for rollforward log
		     */
		    stchar("RS", lbuff+LG_TYPE, 2);	/* record type */
		    stint(xid, lbuff+LG_XID);
		    stint(uid, lbuff+LG_USER);
		    time(&t);
		    stlong(t, lbuff+LG_TIME);		/* time stamp */
		    stint(spnum, lbuff+LG_SAVEPT);	/* savepoint number */
		    if (writelog(lbuff, LG_SSIZE, TRUE)) 
			goto bad;
		    txflag = TRANS;		/* turn transaction back on */
		    return 0;			/* found savepoint - stop */
		    }
		break;
#endif /* ISSVPT */
	    case LG_FOPEN:			/* open file */
		break;
	    default:
		/* should not happen - log corrupted
		 */
		goto bad;
	    }
	prev = ldlong(curr + LG_PREV);
	len = ldint(curr + LG_PREVLEN);
	}
bad:
    txflag = STRANS;				/* turn tansaction back on */
    return -1;
}



/* islogbuild
 *    function: 1. log new file
 *
 *    return: 0 - aok
 *	      -1 - error
 */
islogbuild(fname, reclen, kdesc, mode)
    char *fname;
    int reclen;
    struct keydesc *kdesc;
    int mode;
{
    int klen, lrec;

    stchar("BU", lbuff+LG_TYPE, 2);
    stint(mode, lbuff+LG_FMODE);		/* file mode */
    stint(reclen, lbuff+LG_RECLEN);		/* record length */
    stint(kdesc->k_flags, lbuff+LG_KFLAGS);	/* key flags */
    stint(kdesc->k_nparts, lbuff+LG_NPARTS);	/* parts in key */
    stint(kdesc->k_len, lbuff+LG_KLEN);		/* key length */
    stkeyparts(kdesc, lbuff+LG_KLEN+INTSIZE, &klen);	/* store key desc */
    stcopy(fname, lbuff+LG_KLEN+INTSIZE+klen);	/* store filename */
    lrec = LG_KLEN + INTSIZE + klen + stleng(fname) + 3;

    return writelog(lbuff, lrec, FALSE);
}



/* islogerase
 *    function: 1. log erased file
 *		2. set erased flag if file is used during transaction
 *
 *    return: 0 - aok
 *            -1 - error
 */
islogerase(fname)
    char *fname;
{
    register struct filelist *next;

    for (next = openfile; next; next = next->fl_next)
	{
	if (next->fl_path && (stcmpr(fname, next->fl_path) == 0))
	    next->fl_flag |= FL_ERASED;
	}
    stchar("ER", lbuff+LG_TYPE, 2);
    stcopy(fname, lbuff+LG_FNAME);
    /* for DDL statements only log for rollforward purpose
     * log it as an orphan so rollback does not know about it
     */
    return writelog(lbuff, LG_FNAME+stleng(fname)+1+INTSIZE, FALSE);
}



/* islogidx
 *    function: 1. log create or drop index
 *
 *    return: 0 - aok
 *	     -1 - error
 */
islogidx(ltype, isfd, key)
    int ltype;
    int isfd;
    struct keydesc *key;
{
    int keylen;

    if (ltype == ISCLUSIDX)
	stchar("CL", lbuff+LG_TYPE, 2);
    else
    	stchar(ltype == ISCINDEX ? "CI" : "DI", lbuff+LG_TYPE, 2);
    stint(isfd, lbuff+LG_ISFD);
    stint(key->k_flags, lbuff+LG_IFLAGS);
    stint(key->k_nparts, lbuff+LG_INPARTS);
    stint(key->k_len, lbuff+LG_IKLEN);
    stkeyparts(key, lbuff+LG_IKLEN+INTSIZE, &keylen);

    return writelog(lbuff, LG_IKLEN+INTSIZE+keylen+INTSIZE, FALSE);
}



/* iswimage
 *    function: 1. write before or after image
 *
 *    return: 0 - aok
 *	      -1 - error (see iserrno)
 */
iswimage(itype, isfd, recno, rowsize, before, after)
    int itype, isfd;
    long recno;
    char *before, *after;
    int rowsize;
{
    register int lrec;

    lrec = LG_RECORD + rowsize + INTSIZE;	/* header + record + len */
    if (itype == ISUPDATE)
	lrec = lrec + rowsize + INTSIZE;	/* extra image and image len */
    if (lrec > lbuffsize)
	{
	if ((lbuff = realloc(lbuff, (unsigned) lrec)) == NULL)
	    {
	    iserrno = EBADMEM;			/* no memory		*/
	    return LG_ERROR;
	    }
	else
	    lbuffsize = lrec;
	}

    /* compose log record
     */
    stint(isfd, lbuff+LG_ISFD);
    stlong(recno, lbuff+LG_RECNO);
    switch (itype)
	{
	case ISDELETE:
	    stchar("DE", lbuff+LG_TYPE, 2);
	    bycopy(before, lbuff+LG_RECORD, rowsize);
	    break;
	case ISINSERT:
	    stchar("IN", lbuff+LG_TYPE, 2);
	    bycopy(after, lbuff+LG_RECORD, rowsize);
	    break;
	case ISUPDATE:
	    stchar("UP", lbuff+LG_TYPE, 2);
	    stint(rowsize, lbuff+LG_ILEN);
	    bycopy(before, lbuff+LG_BUPDATE, rowsize);
	    bycopy(after, lbuff+LG_BUPDATE+rowsize, rowsize);
	    break;
	}

    if (writelog(lbuff, lrec, openp->o_mode & ISTRANS))
	return -1;
#ifndef SINIX
    return (txflag == STRANS) ? iscommit() : 0;
#else /* SINIX: nothing to commit. TP 01.09.87 */
    return 0;
#endif /* SINIX */
}


#ifdef ISSVPT

/* issavept
 *    function: get a new savepoint number
 *
 *    return: new savepoint number
 */
issavept()
{
    svptnum++;					/* new savepoint number */
    stint(svptnum, lbuff+LG_SAVEPT);
    if (writelog(lbuff, LG_SSIZE, TRUE))
	return -1;				/* failed to write sp rec */
    return svptnum;
}

#endif /* ISSVPT */


/* lgopfile
 *    function: 1. add to open file list
 *
 *    return: -1 - no memory
 *	       0 - aok
 */
lgopfile(isfd, fname, mode, flag)
    int isfd;
    char *fname;
    int mode, flag;
{
    register struct filelist *opflp, *flp, *newfl;
/*  struct keydesc key;	unused. TP */
/*  char *tbuff;	unused. TP */
    int needspace;

    /* store in open file list for rollback work
     */
    for (opflp = openfile; opflp; opflp = opflp->fl_next)
	{
	if (!opflp->fl_path)
	    {					/* empty slot */
	    needspace = FALSE;
	    break;
	    }
	if (!opflp->fl_next)
	    {
	    needspace = TRUE;			/* need to alloc more memory */
	    break;				/* last slot */
	    }
	}
    if (needspace)
	{
	if ((newfl = (struct filelist *)
		malloc(sizeof(struct filelist) * NOPNFL)) == NULL)
	    {
	    iserrno = EBADMEM;			/* no memory */
	    return -1;
	    }
	byfill(newfl, sizeof(struct filelist) * NOPNFL, 0);
	for (flp = newfl; flp < newfl+NOPNFL-1; flp++)
	    flp->fl_next = flp+1;
	opflp->fl_next = newfl;			/* add to list */
	opflp = newfl;
	}
    if (flag)
	{
	if ((opflp->fl_risfd = isopen(fname, ISINOUT+ISEXCLLOCK)) == -1)
	    return -1;
	}
    else
	opflp->fl_risfd = isfd;
    opflp->fl_oisfd = isfd;
    opflp->fl_path = filep->f_fname;
    opflp->fl_flag = 0;
    opflp->fl_mode = mode;
    return 0;
}



/* isloguid
 *    function: 1. log set unique id
 *
 *    return: 0 - aok
 *           -1 - error
 */
isloguid(isfd, uniqid, flag)
    int isfd, flag;
    long uniqid; 
{
    stchar(flag ? "SU" : "UN", lbuff+LG_TYPE, 2);
    stint(isfd, lbuff+LG_ISFD);
    stlong(uniqid, lbuff+LG_UNIQID);
    return writelog(lbuff, LG_UNIQID+LONGSIZE+INTSIZE, FALSE);
}



/* islogfile
 *    function: 1. logs open or close file
 *
 *    return: 0 - aok
 *	      -1 - error
 */
islogfile(ftype, isfd, fname, mode)
    int ftype, isfd;
    char *fname;
    int mode;
{
    int reclen;

    reclen = LG_FPATH +stleng(fname) + 1 + INTSIZE;
    switch (ftype)
	{
	case ISFOPEN:
	    /* files opened with ISTRANS for output are kept open
	     * until commit or rollback time.  lgopfile saves the
	     * file name so when it is closed during the transaction
	     * it is marked as closed and if it is reopened again in
	     * the same transaction the original isfd is returned
	     */
	    if (mode & ISTRANS && mode & (ISINOUT|ISOUTPUT))
		if (lgopfile(isfd, fname, mode, FALSE))/* add to open list */
		    return -1;
	    stchar("FO", lbuff+LG_TYPE, 2);
	    break;
	case ISFCLOSE:
	    stchar("FC", lbuff+LG_TYPE, 2);
	}
    stint(isfd, lbuff+LG_ISFD);
    stcopy(fname, lbuff+LG_FPATH);
    /* open and close log records are only needed for rollforward
     * since files are not closed during transaction rollback does
     * not need to be in the back link
     */
    return writelog(lbuff, reclen, FALSE);
}



/* islogrename
 *    function: 1. log rename
 *
 *    return: 0 - aok
 *           -1 - error
 */
islogrename(oldname, newname)
    char *oldname, *newname;
{
    register int oldlen, newlen;

    stchar("RE", lbuff+LG_TYPE, 2);
    oldlen = stleng(oldname) + 1;
    newlen = stleng(newname) + 1;
    stint(oldlen, lbuff+LG_OLEN);
    stint(newlen, lbuff+LG_NLEN);
    stcopy(oldname, lbuff+LG_ONAME);
    stcopy(newname, lbuff+LG_ONAME+oldlen);
    return writelog(lbuff, LG_ONAME+oldlen+newlen+INTSIZE, FALSE);
}



/* lgsetuniqid
 *    function: 1. set unique id
 *
 *    return: 0 - aok
 *           -1 - error
 */
lgsetuniqid(curr)
    register char *curr;
{
    int isfd;

    if ((isfd = getrisfd(ldint(curr+LG_ISFD))) == -1)
	return -1;
    if (issetunique(isfd, ldlong(curr+LG_UNIQID)))
	return -1;
    return 0;

}



/* lgaddidx
 *    function: 1. redo add index
 *		2. redo delete index
 *
 *    return: 0 - aok
 *           -1 - error
 */
lgredoidx(curr, flag)
    char *curr;
    int flag;
{
    register int isfd;
    struct keydesc key;

    if ((isfd = getrisfd(ldint(curr+LG_ISFD))) == -1)
	return -1;
    key.k_flags = ldint(curr+LG_IFLAGS);
    key.k_nparts = ldint(curr+LG_INPARTS);
    key.k_len = ldint(curr+LG_IKLEN);
    ldkeyparts(curr+LG_IKLEN+INTSIZE, &key);
    if (flag == TRUE+1)
	{
	if (iscluster(isfd, &key) < 0)
	    return -1;
	else
	    return 0;
	}
    else if (flag)				/* add index */
	     return isaddindex(isfd, &key);
         else   				/* delete index */
	     return isdelindex(isfd, &key);
}



/* lgupdate
 *    function: 1. undo an update
 *
 *    return: 0 - aok
 *	      -1 - error (see iserrno)
 */
lgupdate(curr, flag)
    register char *curr;
    int flag;
{
    register int risfd;
    register char *oldrec, *newrec;

    /* the before and after image must be for the same record
     */
    switch (risfd = getrisfd(ldint(curr+LG_ISFD)))
	{
	case -2:				/* file erased */
	    break;
	case -1:				/* error */
	    return -1;
	default:
	    /* if ROLLFORWARD then use after image else use before image
	     */
	    if (isenter(risfd, ISOUTPUT) == -1)
		return -1;
	    newrec = oldrec = curr + LG_BUPDATE;
	    if (flag == LG_RFORWARD)
		newrec += ldint(curr + LG_ILEN);
	    else
		oldrec += ldint(curr + LG_ILEN);
	    openp->o_mode &= ~ISTRANS;
	    rewrecord(risfd, oldrec, newrec, ldlong(curr+LG_RECNO), 0);
	    return isexit();
	}
    return 0;
}



/* lginsert
 *    function: 1. insert deleted record
 *
 *    Note: assume no delayed deletes
 *
 *    return: 0 - aok
 *	      -1 - error (see iserrno)
 */
lginsert(curr, flag)
    register char *curr;
    int flag;
{
    register int risfd;
    register long recnum;

    switch (risfd = getrisfd(ldint(curr+LG_ISFD)))
	{
	case -2:				/* file erased */
	    return 0;
	case -1:				/* error */
	    return -1;
	default:
	    if (isenter(risfd, ISOUTPUT) == -1)
		return -1;
	    recnum = ldlong(curr+LG_RECNO);
	    openp->o_mode &= ~ISTRANS;
	    if ((flag == LG_RFORWARD) && (getdatrec(recnum) == -1))
		return(-1);
	    wrtrecord(risfd, curr+LG_RECORD, recnum, 1);
	    return isexit();
	}
}



/* getdatrec
 *    function: 1. get a specific record from the free list
 *		2. allocate new record if not in free list or free list empty
 *
 *    return: 0 - no error
 *	     -1 - record not in free list
 */
getdatrec(recnum)
    register long recnum;
{
    register struct buff *bp;
    register char *freep, *nextp;
    register long node, node2;
    register int i;

    freep = dictp->d_freedata;
    nextp = dictp->d_nextdata;

    node = ld4(freep);
    node2 = ld4(nextp);				/* last used record */
    if (recnum > node2)
	{
	/* no free list or requested record is new
	 * extend file to correct size according to
	 * recnum and add all in between records to free list
	 */
	for ( ; ; )
	    {
	    node2++;
	    if (node2 == recnum)		/* exclude current one 	*/
		break;
	    freerec(node2, DATAREC);		/* put in free list	*/
	    }
	st4(recnum, nextp);			/* update used list	*/
	dictmodify();
	return 0;
	}
    else
	/* remove recnum from free list
	 */
	for ( ; ; )
	    {
	    if (node == 0)			/* no more nodes */
		break;
	    bp = bfread(node);
	    if (bp == NULL) break;		/* should never happen	*/
	    i = 6L;				/* 1st 6 bytes are resv */
	    for ( ; ; )
		{
		if (i >= bp->bf_used)
		    {				/* exhausted current page */
#ifndef VMS
		    node = ld4(bp->bf_un.bf_addr+2);	/* next page	*/
#else /* VMS */
		    node = ld4(((char *) GSADDRESS(bp->bf_un.bf_addr))+2);
#endif /* VMS */
		    bfrelease(bp);
		    break;			/* on with the next page */
		    }
#ifndef VMS
		if (recnum == (node2 = ld4(bp->bf_un.bf_addr+i)))
#else /* VMS */
	      if (recnum==
		  (node2 = ld4(((char *)GSADDRESS(bp->bf_un.bf_addr))+i)))
#endif /* VMS */
		    {				/* found node in free list */
		    bp->bf_used -= 4;		/* decrement size */
		    if (bp->bf_used <= 2)
			{			/* used last free one */
			st4(node, freep);	/* rm node from free list */
			dictmodify();		/* mark dict as modified */
			freerec(node, INDXREC);	/* free index record */
			}
		    else			/* compress */
#ifndef VMS
			bycopy(CASTCP (bp->bf_un.bf_addr+i+4),
			    CASTCP (bp->bf_un.bf_addr+i), bp->bf_used-i);
#else /* VMS */
		bycopy(CASTCP (((char *)
			GSADDRESS(bp->bf_un.bf_addr))+i+4), CASTCP
			(((char *) GSADDRESS(bp->bf_un.bf_addr))+i),
			bp->bf_used-i);
#endif /* VMS */
		    bfdwrite(bp);
		    return 0;			/* found record */
		    }
		i += 4;
		}
	    }
    iserrno = EBADLOG;
    return -1;
}



/* lgdelete
 *    function: 1. delete record
 *
 *    return: 0 - aok
 *	      -1 - error (see iserrno)
 */
lgdelete(curr)
    register char *curr;
{
    register int risfd;
    register long recnum;

    switch (risfd = getrisfd(ldint(curr+LG_ISFD)))
	{
	case -2:				/* file erased */
	    return 0;
	case -1:				/* error */
	    return -1;
	default:
	    if (isenter(risfd, ISOUTPUT) == -1) return(-1);

	    recnum = ldlong(curr+LG_RECNO);
	    if (getcurrent(risfd, recnum) != 0)
		return(isexit());

	    openp->o_mode &= ~ISTRANS;
	    delrecord(risfd, temprecord, recnum);
	    return(isexit());
	}
}



/* lgrename
 *    function: 1. redo rename
 *
 *    return: 0 - aok
 *	     -1 - error
 */
lgrename(lrec)
    char *lrec;
{
    register int newloc;

    newloc = ldint(lrec+LG_OLEN);
    return isrename(lrec+LG_ONAME, lrec+LG_ONAME+newloc);
}


#ifdef ISSVPT

/* lgsavept
 *    function: 1. check savepoint and rollback work number
 *
 *    return: TRUE - found savepoint
 *	      FALSE - savepoint not found
 */
lgsavept(spnum, curr)
    short spnum;
    char *curr;
{
    short snum;

    snum = ldint(curr+LG_SAVEPT);
    return snum == spnum;
}
#endif /* ISSVPT */
#endif /* ISXACTION */

#ifdef SINIX	/* added by TP */
#ifndef ISXACTION
islogopen(logname)
    char *logname;
{
    return 0;
}

islogclose()
{
    return 0;
}
#endif /* ISXACTION */
#endif /* SINIX */
