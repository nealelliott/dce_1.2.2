/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isopen.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:22  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:58  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:21  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:59:18  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:08:35  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:06:27  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:56:56  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:05  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:23:16  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  17:48:14  marrek]
 * 
 * Revision 1.1.2.3  1992/06/01  23:38:01  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:41:19  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/09  20:07:10  jim
 * 	Comment strings after #endif and #else statements.
 * 	[1992/04/09  19:32:25  jim]
 * 
 * Revision 1.1  1992/01/19  15:12:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isopen.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:19 $";
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
 *  Title:	isopen.c
 *  Description:
 *		isopen()	-- open a C-ISAM file
 *		isclose()	-- close a C-ISAM file
 *		isrelease()	-- release locks on C-ISAM file
 *		iscleanup()	-- close all C-ISAM files
 *		isuniqueid()	-- get next unique identifier
 *		issetunique()	-- set next unique identifier
 *		isindexinfo()	-- get index information
 *		iststindex()	-- test for existence of index
 *
 ************************************************************************
 */


/*
 * isam open and close routines
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 *
 *  06-07-83  RVH	V104 -- added kernel modifications
 */

#include <isdec.c>
#ifdef VMS
#include <errno.h>
#endif /* VMS */

#ifdef	PCDOS
#include <fcntl.h>
#endif

extern short txflag;
extern struct filelist *openfile;

/*
 * open isam file
 */
 
isopen(filename, mode)
char *filename;		/* V104 - NOTE: for kernel this is array of ptrs */
register int mode;
{
	register int lockmode;
	register int isfd;
	register struct filelist *next;
	struct keydesc key;
	char *buff;
	extern int real(), demo(), wrong(), snbadmsg();

#ifdef	SHMLOCK
	if (!shminit)
	    if (shmsetup()) goto reterr;
#endif	/* SHMLOCK */

	snverify(real, demo, wrong, snbadmsg);	/* valid serial number? */

	if (issingleuser) mode = ISEXCLLOCK | (mode & IOMASK);

        if (txflag == NOTRANS)
	    {					/* no transaction */
	    if (mode & (ISTRANS | ISNOLOG))
		{
		iserrno = EBADARG;
		goto reterr;
		}
	    }
	else					/* has transaction */
	    if (mode & ISTRANS && mode & ISNOLOG)
		{
		/* ISTRANS and ISNOLOG are mutually exclusive all operations
		 * on files that participate in a transaction (ISTRANS)
		 * must be logged.
		 */
		iserrno = EBADARG;
		goto reterr;
		}
#ifdef ISXACTION
	/* files opened with ISTRANS for output are left open until
	 * commit or rollback time, so if it is opened again with
	 * the same mode just return the old isfd
	 */
	if (mode & ISTRANS)
	    for (next = openfile; next; next = next->fl_next)
		{
		if (next->fl_flag & FL_CLOSED)	/* logically closed	*/
#ifdef SINIX	/* TP */
		    if (next->fl_path != NULL)
#endif /* SINIX */
		    if ((stcmpr(next->fl_path, filename) == 0) &&
			    (mode == next->fl_mode))
			{
			if (isindexinfo(next->fl_oisfd, &key, 1))
			    goto reterr;
			if (isstart(next->fl_oisfd, &key, 0, buff, ISFIRST))
			    goto reterr;
			next->fl_flag &= ~FL_CLOSED;
			return next->fl_oisfd;
			}
		}
#endif /* ISXACTION */

#ifndef ISKERNEL	/* V104 */
	if (ckfilname(filename)) goto reterr;
#endif
	snchk();	/* take action against bad serial number */
	isbatch(2);

	if ((isfd = allocopen()) < 0)		/* allocate open struct	*/
	    goto reterr;

	openp->o_tmpfree = NULL;		/* init temp free list	*/
	openp->o_mode = mode & IOMASK;
	lockmode = mode & LOCKMASK;
	switch (lockmode)			/* check for legal mode	*/
	    {
	    case OLD_AUTO:
		lockmode = ISAUTOLOCK; break;
	    case OLD_EXCL:
		lockmode = ISEXCLLOCK; break;
	    case ISAUTOLOCK:
	    case ISMANULOCK:
	    case ISEXCLLOCK:
		break;
	    default:
		iserrno = EBADARG;
		goto openerr;
	    }

	switch (openp->o_mode & 3)		/* 3 = ISINOUT + ISOUTPUT */
	    {
	    case ISINPUT:
	    case ISOUTPUT:
	    case ISINOUT:
		break;
	    default:
		iserrno = EBADARG;
		goto openerr;
	    }
						/* allocate file struct	*/
	filep = openp->o_filep = allocfile(filename, openp->o_mode);
	if (filep == NULL)
	    goto openerr;
#ifdef ISKERNEL
	dictp = &filep->f_dict;			/* read dictionary	*/
	if (dictread())
	    goto openerr;
#endif /* ISKERNEL */

	PROFILE(pf_isopen);			/* count isopen's	*/
#ifndef SINIX
	if (isenter(isfd, openp->o_mode))
#else /* isopen() doesn't write anything, so use shared lock. TP 13.07.88 */
	if (isenter(isfd, openp->o_mode+LOCKSHARE))
#endif /* SINIX */
	    goto openerr;

	openp->o_mode += lockmode;		/* add in lock mode now	*/

	if (lkopenlock(lockmode))
	    goto opener2;

	filep->f_idxsize = ld2(dictp->d_idxsize);
	filep->f_datsize = ld2(dictp->d_recsize);
	if (mktemprec(filep->f_datsize))
	    goto opener3;

#ifdef MPM86
	filep->f_datsize |= 0177;		/* make 128 byte records*/
#endif /* MPM86 */

	if ((ld2(dictp->d_magic) & 0177777) != ISMAGIC ||
	    filep->f_idxsize < 127 || filep->f_idxsize >= NODESIZE ||
	    ld1(dictp->d_dflag) != 2 ||
	    kysearch(&openp->o_curkey, 1) < 0)
	    {					/* illegal ISAM file	*/
	    iserrno = EBADFILE;
	    }
	else kposition(temprecord, 0, ISFIRST);

	if (iserrno == 0)
	    {
#ifdef ISXACTION
	    /* only log files that are opened for output and
	     * require logging
	     */
	    if (txflag != NOTRANS &&
		    (openp->o_mode & (ISINOUT | ISOUTPUT)) &&
		    !(openp->o_mode & ISNOLOG))
		if (islogfile(ISFOPEN, isfd, filename, mode))
		    goto opener3;
#endif /* ISXACTION */
	    VOID isexit();
	    return(isfd);			/* return is file desc	*/
	    }

opener3:
	VOID lkopenlock(UNLOCK);
opener2:
	VOID isexit();
openerr:
	if (openp->o_flags & OSLOCK)		/* semaphore lock on ?	*/
	    VOID lksemaphore(UNLOCK);		/* yes, release it	*/
	freeopen(openp);
reterr:
	isstat1 = '9';
	return(-1);				/* return error	code	*/
}



/*
 * set issingleuser on or off
 */

issetsu(mode)
register mode;
{
	issingleuser = mode;
}



/*
 * cleanup routine, closes all isam files
 */

iscleanup()
{
	register int i;
#ifdef ISPCLOSE
	register struct ifile *fp;
	int sav;
	extern int ismaxfds;
#endif /* ISPCLOSE */

#ifdef VMS
	cleanup_flag = 1;
#endif /* VMS */

	for (i=0; i<nopens; i++)
	    VOID isclose(i);

#ifdef ISPCLOSE
	sav = ismaxfds ;
	ismaxfds = 0;
	for (i=0; i<nfiles; i++)
	    if ( (fp = files[i])&& fp->f_ucount >= 0 )
			freefile(fp);
	ismaxfds = sav;
#endif /* ISPCLOSE */

#ifdef SHMLOCK
	shmfinish();
#endif /* SHMLOCK */

#ifdef VMS
	cleanup_flag = 0;
#endif /* VMS */
	return(0);
}

#ifdef ISXBATCH
extern int isiocnt;
#ifndef SINIX
static int batchmax = 5;
#else /* SINIX: environment variable set in isbuf.c. TP */
int batchmax = 6;
#endif /* SINIX */
static short lockcnt = 0;
#endif /* ISXBATCH */

/*
 * close isam file
 */

isclose(isfd)
int isfd;
{
	register int cc = 0;
	register struct filelist *next;

#ifndef SINIX
	if (isenter(isfd, -1) == -1)
	    return(-1);

#else /* SINIX */

	if ((!(isfd < 0 && isfd >= nopens)) && (opens[isfd] != NULL) &&
	    (!(opens[isfd]->o_mode & (ISINOUT|ISOUTPUT))))
	    {
	    if (isenter(isfd, -1 + LOCKSHARE) == -1)
		return(-1);
	    }
	else
	    {
	    if (isenter(isfd, -1) == -1)
		return(-1);
	    }
#endif /* SINIX */

	if (tempisfd == isfd) tempisfd = -1;

#ifdef ISXACTION
	/* if in transaction and file was opened for input/output
	 * then leave it open until commit or rollback time
	 */
	if (txflag != NOTRANS)
	    {					/* has transaction */
	    if (openp->o_mode & ISTRANS &&
			openp->o_mode & (ISINOUT|ISOUTPUT))
		{
		/* input files are not logged.   output files opened with
		 * ISTRANS remains open until commit or rollback time.
		 * ISTRANS and ISNOLOG are mutually exclusive. openfile
		 * contains a list of all output files with ISTRANS
		 */
		for (next = openfile; next; next = next->fl_next)
		    {
		    if (isfd == next->fl_oisfd)
			{
			/* mark it closed so if it is reopened with the
			 * same mode it will be given the same isfd
			 */
			next->fl_flag |= FL_CLOSED;
			return isexit();
			}
		    }
		}
	    if (!(openp->o_mode & ISNOLOG) &&
			    openp->o_mode & (ISINOUT+ISOUTPUT))
		if (islogfile(ISFCLOSE, isfd,
			openp->o_filep->f_fname, openp->o_mode))
		    return isexit();
	    }
#endif /* ISXACTION */

	if (! (openp->o_flags & OSLOCK))	/* force semaphore lock	*/
	    VOID lksemaphore(LOCKUNCOND);
	VOID lkfile(UNLOCK);
	VOID lkopenlock(UNLOCK);
	openp->o_flags &= ~OSLOCK;		/* save semaphore lock	*/
	cc = isexit();
#ifndef SHMBUFF
	bfflush(filep);
#endif  /* SHMBUFF */
#ifdef ISXBATCH
	lockcnt--;
#endif /* ISXBATCH */
	VOID lksemaphore(UNLOCK);
	freeopen(openp);
	return(cc);
}

/*
 * release locks on isam file
 */

isrelease(isfd)
int isfd;
{
	if (isenter(isfd, ISINPUT+ISUNLOCK+LOCKSHARE) == -1) return(-1);

	if (isfd == tempisfd) tempisfd = -1;	/* mark temprecord gone */

	if (openp->o_mode & ISTRANS)
	     lkrelease();
	else lkfile(UNLOCK);

	return(isexit());
}

#ifndef ISKERNEL	/* V104 */
/*
 * flush all buffers for isam file
 */

isflush(isfd)
int isfd;
{
	if (isenter(isfd, ISOUTPUT) == -1) return(-1);
	VOID isexit();
#ifndef SHMBUFF
	bfflush(filep);
#endif  /* SHMBUFF */
	return(iserrno? -1: 0);
}
#endif  /* ISKERNEL */


short batchflag = 0;

/*
 * enter into isam routine
 */

STATIC int isenter(isfd, mode)
register int isfd;
register int mode;
{
	register long ver;
#ifdef ISVOPEN
	char filename[PATHSIZE+4];
	static short vorefcnt = 0;
#endif /* ISVOPEN */

	iserrno = iserrio = 0;			/* clear error codes	*/
	isstat1 = isstat2 = '0';
	mode++;
#ifdef ISXBATCH
	isiocnt = 0;
#endif /* ISXBATCH */
	if (isfd < 0 || isfd >= nopens)
	    goto starterr;
#ifdef VMS
	if (shminit && (shmem->sh_signal & GSNOTAVAIL) && !cleanup_flag)
	    {
	    iserrno = EGBLSECT;
	    goto reterr;
	    }
#endif /* VMS */
#ifdef ISKERNEL
	{
	register struct kobuf *kopenp;
	kopenp = &kotab[isfd];
	if (kopenp->ko_pid != getpid())		/* correct process ?	*/
	    goto starterr;
	if (kopenp->ko_buf == NULL)
	    kopenp->ko_buf = bread(pipedev, kopenp->ko_blkno);
	openp = (struct iopen *) kopenp->ko_buf->b_un.b_addr;
	openp->o_kopenp = kopenp;		/* cross-link them	*/
	}
#else
#ifdef ISXBATCH
	/*
         *   unlock  if
	 *		already locked and
	 */
	if (batchflag && lockcnt && 
	     (     /* current or old is exclusive */
		   ( openp && openp != opens[isfd] && 
			    ( lockcnt < 0 || !(mode & LOCKSHARE )))

	     ||   /* or have shared lock */
	          ( lockcnt > 0 && 
		      (   /* need exclusive */
			  !(mode & LOCKSHARE) 
		      || /* or whould be unlocked  in isexit */
			 ( batchflag >= batchmax &&
				!(opens[isfd]->o_flags & OSLOCK)) ))
		) )
	    isbatch(2);
#else /* ISXBATCH */
	if (batchflag && openp && openp != opens[isfd])
	    {
	    if (openp->o_flags & OSLOCK)
	        VOID lksemaphore(UNLOCK);
	    batchflag = 1;
	    }
#endif /* ISXBATCH */

	openp = opens[isfd];			/* point to open file	*/
	if (openp == NULL || ! (openp->o_flags & OBUSY))
	    goto starterr;			/* file open ?		*/
#endif /* ISKERNEL */
	filep = openp->o_filep;
#ifdef SHMLOCK
	dictp = &filep->f_smfile->fl_dict;
#else  /* SHMLOCK */
	dictp = &filep->f_dict;
#endif /* SHMLOCK */
	filep->f_flags &= ~FMODF;
	openp->o_flags &= ~ODUPL;

	if ((mode & (openp->o_mode+1) & 3) != (mode & 3))
	    goto starterr;
	if (filep->f_xlkcnt == 0 && ! (openp->o_flags & OSLOCK))
			/* lock isam file if necessary	*/
	    {
#ifdef ISXBATCH
	    if (batchflag) 
	    	if ( mode & LOCKSHARE )
			lockcnt++;
		else
			lockcnt = -1;
#else /* ISXBATCH */
	    if (batchflag) mode &= ~LOCKSHARE;
#endif /* ISXBATCH */
	    if (lksemaphore(LOCKUNCOND+(mode&LOCKSHARE)))
		goto reterr;
#ifndef SHMLOCK
#ifndef ISKERNEL
		{
		if (dictread())			/* read dictionary	*/
		    {
		    VOID lksemaphore(UNLOCK);
		    goto reterr;
		    }
		}
#endif  /* ISKERNEL */
#endif  /* SHMLOCK */
	    }

	PROFILE(pf_iscalls);			/* count isam calls	*/
	treelev = openp->o_trlev;		/* restore tree globals	*/
	treeleaf = openp->o_trleaf;
	treeflags = openp->o_trflags;

	ver = ld4(dictp->d_transaction);
	if (openp->o_version != ver)		/* versions match ?	*/
	    {
	    openp->o_version = ver;		/* no, ignor all saved	*/
#ifndef SHMBUFF
	    if (filep->f_version != ver)
		{
		filep->f_version = ver;
		bfignor(filep);			/* ignor buffers	*/
		}
#endif  /* SHMBUFF */
	    treelev = -1;
	    }
	if (treelev >= 0)			/* init treeitem	*/
	    itcopy(&openp->o_curitem, &treeitem, &openp->o_curkey);

#ifdef ISVOPEN
	/* reopen dat file if needed */
	if ( filep->f_datfd == -1 )
	    {
	    mkdatname(filep->f_fname,filename);
	    if ((filep->f_datfd = open(filename, filep->f_omode))<0
			&& errno == EMFILE)
		{
		vclose();
		filep->f_datfd = open(filename, filep->f_omode);
		}
	    if (filep->f_datfd < 0)
		{
		iserrno = errno;	
		goto reterr;
		}
#ifdef SHMLOCK
	    mkidxname(filep->f_fname,filename);
	    if ((filep->f_idxfd = open(filename, RWMODE))<0
			&& errno == EMFILE)
		{
		vclose();
		filep->f_idxfd = open(filename, RWMODE);
		}
	    if (filep->f_idxfd < 0)
		{
		iserrno = errno;	
		goto reterr;
		}
#endif /* SHMLOCK */
	    filep->f_vrefcnt = ++vorefcnt;
#ifdef CREATLOCK
	    mklokname(filep->f_fname,filename);
	    if ((filep->f_lokfd = open(filename, RWMODE))<0 
			&& errno == EMFILE)
		{
		vclose();
		filep->f_lokfd = open(filename, RWMODE);
		}
	    if (filep->f_lokfd < 0)
		{
		iserrno = errno;	
		goto reterr;
		}
#endif
	    }	
	else
	    if ( filep->f_vrefcnt != vorefcnt )
			filep->f_vrefcnt = ++vorefcnt;
#endif /* ISVOPEN */
	if (openp->o_mode & ISAUTOLOCK)
	    {
	    if (openp->o_mode & ISTRANS)	/* unlock records	*/
		 lkrelease();
	    else lkfile(UNLOCK);
	    }

	return(0);

starterr:
	iserrno = ENOTOPEN;			/* file not open	*/
reterr:
	isstat1 = '9';
	return(-1);
}

/*
 * exit from isam routine
 */

STATIC int isexit()
{
	openp->o_trlev = treelev;		/* save tree globals	*/
	openp->o_trleaf = treeleaf;
	openp->o_trflags = treeflags;

	if (filep->f_flags & FMODF)		/* file modified ?	*/
	    {					/* yes			*/
	    filep->f_flags &= ~FMODF;		/* clear modified bit	*/
	    openp->o_version = ++filep->f_version;
	    st4(openp->o_version, dictp->d_transaction);
						/* inc version number	*/
	    dictmodify();
#ifndef MEMLOCK
#ifndef NOLOCK
#ifndef ISKERNEL
#ifndef SHMBUFF
	    if (! (openp->o_mode & ISEXCLLOCK))	/* if not exclusive	*/
		bfflush(filep);			/* flush node buffers	*/
#endif
#endif
#endif
#endif
	    }

	if (filep->f_flags & FDICTMOD)
	    dictwrite();			/* release dict buffer	*/


	switch(iserrno)				/* set status flags	*/
	    {
	    case 0:
		isstat1 = '0';
		isstat2 = (openp->o_flags & ODUPL) ? '2' : '0';
		break;
	    case EDUPL:
		isstat1 = '2'; isstat2 = '2'; break;
	    case EENDFILE:
		isstat1 = '1'; isstat2 = '0'; break;
	    case ENOREC:
		isstat1 = '2'; isstat2 = '3'; break;
	    case ENOCURR:
		isstat1 = '2'; isstat2 = '1'; break;
	/*  case ENOTOPEN:
	    case EBADARG:
	    case EBADKEY:
	    case ETOOMANY:
	    case EBADFILE:
	    case ENOTEXCL:
	    case ELOCKED:
	    case EKEXISTS:
	    case EPRIMKEY:
	*/
	    default:
		isstat1 = '9'; isstat2 = '0'; break;
	    }

#ifdef ISXBATCH
	if (batchflag == 0 || batchflag > batchmax)
	    isbatch(2);
	else if ( batchflag && (isiocnt || lockcnt < 0 ) ) batchflag++;
#else /* ISXBATCH */
	if (batchflag == 0 || ++batchflag > 6)
	    {
	    if (openp->o_flags & OSLOCK)	/* semaphore set ?	*/
		VOID lksemaphore(UNLOCK);	/* yes, release it	*/
	    if (batchflag) batchflag = 1;
	    }
#endif /* ISXBATCH */

#ifdef ISKERNEL
	if (openp->o_kopenp->ko_buf)
	    bdwrite(openp->o_kopenp->ko_buf);
	openp->o_kopenp->ko_buf = NULL;
#endif /* ISKERNEL */
	return(iserrno? -1: 0);
}

#ifdef VMS
STATIC int isexit2() /* VMS alternate isexit with dictwrite removed */
{
	openp->o_trlev = treelev;		/* save tree globals	*/
	openp->o_trleaf = treeleaf;
	openp->o_trflags = treeflags;

	if (filep->f_flags & FMODF)		/* file modified ?	*/
	    {					/* yes			*/
	    filep->f_flags &= ~FMODF;		/* clear modified bit	*/
	    openp->o_version = ++filep->f_version;
	    st4(openp->o_version, dictp->d_transaction);
						/* inc version number	*/
	    dictmodify();
	    }

	switch(iserrno)				/* set status flags	*/
	    {
	    case 0:
		isstat1 = '0';
		isstat2 = (openp->o_flags & ODUPL) ? '2' : '0';
		break;
	    case EDUPL:
		isstat1 = '2'; isstat2 = '2'; break;
	    case EENDFILE:
		isstat1 = '1'; isstat2 = '0'; break;
	    case ENOREC:
		isstat1 = '2'; isstat2 = '3'; break;
	    case ENOCURR:
		isstat1 = '2'; isstat2 = '1'; break;
	    default:
		isstat1 = '9'; isstat2 = '0'; break;
	    }

	if (batchflag == 0 || ++batchflag > 6)
	    {
	    if (openp->o_flags & OSLOCK)	/* semaphore set ?	*/
		VOID lksemaphore(UNLOCK);	/* yes, release it	*/
	    if (batchflag) batchflag = 1;
	    }

	return(iserrno? -1: 0);
}
#endif /* VMS */

isbatch(flag)
    int flag;
{
#ifndef SHMLOCK
#ifdef ISXBATCH
    struct ifile *savfp;
    struct iopen *savopenp;
    register int i;

    if ( flag != 2)
	    batchflag = (flag != 0);
    else if ( batchflag ) batchflag = 1;
    savfp = filep;
    savopenp = openp;
    for ( i = 0;  i< nopens; i++ )
	{
	openp = opens[i];
	if (openp && openp->o_flags & OSLOCK)	/* semaphore set ?	*/
	    {
	    filep = openp->o_filep;
	    VOID lksemaphore(UNLOCK);		/* yes, release it	*/
	    }
	}
    filep = savfp;
    openp = savopenp;
    lockcnt = 0;
#else /* ISXBATCH */
    if (flag == 0 || flag == 1)
	batchflag = flag;
    if (openp && openp->o_flags & OSLOCK)	/* semaphore set ?	*/
	VOID lksemaphore(UNLOCK);		/* yes, release it	*/
#endif /* ISXBATCH */
#endif /* SHMLOCK */
}


/*
 * return unique id
 */

isuniqueid(isfd, uniqid)
int isfd;
long *uniqid;
{
	register long id;

	if (isenter(isfd, ISOUTPUT) == -1) return(-1);

	id = ld4(dictp->d_uniqid);		/* get unique id	*/
	*uniqid = id++;
	st4(id, dictp->d_uniqid);		/* update unique id	*/
	dictmodify();

#ifdef ISXACTION
	if (txflag != NOTRANS && !(openp->o_mode & ISNOLOG))
	    if (isloguid(isfd, *uniqid, 0))
		iserrno = ELOGWRIT;
#endif /* ISXACTION */

	return(isexit());
}


/*
 * return serial value from file (do not increment serial value stored
 * in file)
 */

iscurruniq(isfd, uniqid)
int isfd;
long *uniqid;
{
	if (isenter(isfd, -1+LOCKSHARE) == -1) return(-1);

	*uniqid = ld4(dictp->d_uniqid);		/* get unique id	*/
	
	return(isexit());
}


/*
 * set unique id
 */

issetunique(isfd, uniqid)
int isfd;
long uniqid;
{
	if (isenter(isfd, ISOUTPUT) == -1) return(-1);

	if (uniqid > ld4(dictp->d_uniqid))	/* disallow lesser id	*/
	    {
	    st4(uniqid, dictp->d_uniqid);	/* store new unique id	*/
	    dictmodify();
#ifdef ISXACTION
	    if (txflag != NOTRANS && !(openp->o_mode & ISNOLOG))
		if (isloguid(isfd, uniqid, 1))	/* log it		*/
		    iserrno = ELOGWRIT;
#endif /* ISXACTION */
	    }

	return(isexit());
}

/*
 * return index info
 */

isindexinfo(isfd, keyd, mode)
int isfd;
struct keydesc *keyd;
register int mode;
{
	union
	    {
	    struct keydesc *key;
	    struct dictinfo *dic;
	    } info;
	register struct buff *bp;
	register long nrecs, node;

	if (isenter(isfd, -1+LOCKSHARE) == -1) return(-1);

	info.key = keyd;
	if (mode == 0)
	    {
	    info.dic->di_nkeys   = ld2(dictp->d_nkeys);
	    info.dic->di_recsize = ld2(dictp->d_recsize);
	    info.dic->di_idxsize = ld2(dictp->d_idxsize);
	    nrecs = ld4(dictp->d_nextdata);	/* number of records	*/
	    node  = ld4(dictp->d_freedata);
	    while (node != 0)			/* subtract free ones	*/
		{
		bp = bfread(node);
		if (bp == NULL) break;
		nrecs -= (bp->bf_used-6) / 4;
#ifndef VMS
		node = ld4(bp->bf_un.bf_free->fn_next);
#else /* VMS */
	    node = ld4(((struct free *) GSADDRESS(bp->bf_un.bf_free))->fn_next);
#endif /* VMS */
		bfrelease(bp);
		}
	    info.dic->di_nrecords = nrecs;
	    }
	else if (mode > 0)
	    {
	    if (kysearch(info.key, mode) < 0)
		iserrno = EBADKEY;
	    }
	else iserrno = EBADARG;

	return(isexit());
}

/*
 * test for index existence
 */

iststindex(isfd, keyd)
int isfd;
struct keydesc *keyd;
{
	if (isenter(isfd, -1+LOCKSHARE) == -1) return(-1);

	if (kysearch(keyd, 0) < 0)
	    iserrno = EBADKEY;
	
	return(isexit());
}

/*
 * dictionary management routines
 *  operates on dictionary node of index file
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	March 22, 1984
 *
 */

/*
 * read dictionary info into file table
 */

#define DICTSIZE (sizeof(struct dict))
#define DICTADDR ((char*) dictp)

dictread()
{
	sysmemory();
	if (lseek(filep->f_idxfd, 0L, 0) != 0L)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_IDX;
	    return(-1);
	    }
	if (read(filep->f_idxfd, DICTADDR, DICTSIZE) != DICTSIZE)
	    {
	    iserrno = errno;
	    iserrio = IO_READ+IO_IDX;
	    return(-1);
	    }
	return(0);
}

/*
 * write dictionary info out to file
 */

dictwrite()
{
	sysmemory();
	if (lseek(filep->f_idxfd, 0L, 0) != 0L)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_IDX;
	    return(-1);
	    }
#ifndef VMS
	if (write(filep->f_idxfd, DICTADDR, DICTSIZE) != DICTSIZE)
#else /* VMS */
	if (write(filep->f_idxfd, DICTADDR, 512) != 512)
#endif /* VMS */
	    {
	    iserrno = errno;
	    iserrio = IO_WRIT+IO_IDX;
	    return(-1);
	    }
	filep->f_flags &= ~FDICTMOD;	/* clear modified bit	*/
	return(0);
}

/*
 * mark dictionary info as modified
 */

dictmodify()
{
	filep->f_flags |= FDICTMOD;	/* mark it as modified	*/
}

/*
 * mktemprec -- allocate temporary record
 *
 *  make sure temprecord is at least one byte larger than recsize
 *  also round up to next 256 byte boundary so there won't
 *  be too many allocates for gradually increasing record sizes
 */

STATIC int mktemprec(recsize)
int recsize;
{
	if (temprsize <= recsize)
	    {
	    temprsize = (recsize + 0400) & ~0377;
	    tempisfd = -1;			/* temp rec is destroyed */
	    if (temprecord) free(temprecord);
	    if ((temprecord = malloc((unsigned) temprsize)) == NULL)
		{
		iserrno = ENOMEM;
		return(-1);
		}
	    }
	return(0);
}
