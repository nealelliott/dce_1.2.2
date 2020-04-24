/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: islock.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:20  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:56  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:17  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:58:54  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:06:20  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:05:57  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:50:43  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:55  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:37:43  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:41:08  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: islock.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:17 $";
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
 *  Title:	islock.c
 *  Description:
 *		File and record level locking routines.
 *
 ************************************************************************
 */


/*
 * file and record lock management routines
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 *
 *	This module provides four different locking facilities
 * using one of five different locking methods.
 *
 * Lock Facilities:
 *    Internal System level:
 *	 lksemaphore() -- ISAM file protection
 *			Semaphore provides serialization of all
 *			C-ISAM file accesses to insure file integrity.
 *    External User level:
 *	 lkopenlock() -- Exclusive open of ISAM file.
 *			 no other process can even open ISAM file.
 *	 lkfile()    --  File level lock.
 *			 no other process can add, delete, or update
 *			 any record in the file.
 *	 lkrecord()  --  Record level lock.
 *			 no other process can delete or update a
 *			 record which is locked.
 *
 * Lock Methods:
 *	NOLOCK	    --	no locking at all (single user)
 *	RDSLOCK     --  special RDS locking routines
 *	ONIXLOCK    --	"locking" system call provided by
 *			ONIX, PLEXUS, XENIX, ZILOG (slight variation)
 *	CREATLOCK   --	Vanilla UNIX - file creation => semaphore
 *			".lok" file => record lock information
 *	MEMLOCK     --	record lock information kept in memory
 *			only one process will access ISAM file
 *	SEMLOCK     --  System V semaphore locking
 *	SHMLOCK     --  System V shared memory locking
 *	SYS5LOCK    --  System V fcntl locking
 *	MSNET    --  MSDOS 3.1 locking
 */


#include <isdec.c>

#ifdef CREATLOCK		/* CREATMEM => CREATLOCK or MEMLOCK */
#define CREATMEM
#define TIMEAGED 120		/* aged semaphore lock in seconds */
#endif
#ifdef MEMLOCK
#define CREATMEM
#endif

#ifdef SYS5LOCK
#include <fcntl.h>
static struct flock flk;
#endif /* SYS5LOCK */

#ifdef MSNET
#define ONIXLOCK
#endif

extern short islogisfd;

/*
 * lock whole isam file
 */

islock(isfd)
int isfd;
{
	if (isenter(isfd, -1) == -1) return(-1);
	VOID lkfile(LOCKCOND);
	return(isexit());
}

/*
 * unlock whole isam file
 */

isunlock(isfd)
int isfd;
{
	if (isenter(isfd, -1+LOCKSHARE+ISUNLOCK) == -1) return(-1);

	if (isfd == tempisfd) tempisfd = -1;

	if (openp->o_mode & ISTRANS)
	     lkrelease();
	else lkfile(UNLOCK);

	return(isexit());
}



/*
 * semaphore locking -- system level lock
 *	provides serialization of C-ISAM calls
 *	must be called before accessing any files
 *	type:	UNLOCK     -- release semaphore
 *		LOCKUNCOND -- create semaphore
 */

/* ARGSUSED */
STATIC int lksemaphore(type)
int type;
{
#ifdef SHMLOCK
	 if ((filep->f_smfile->fl_flags & FDICTVAL) == 0)
	     {
	     dictread();
	     filep->f_smfile->fl_flags |= FDICTVAL;
	     }

	if (type == UNLOCK)
	    type = LOCK_NONE;
	else
	    type = (type & LOCKSHARE) ? LOCK_S : LOCK_X;
	/* note that the file pointer used is not the same
	 * as the file pointer for record/file locks, this insures
	 * that this lock will not be released by lkrelse()
	 */
	if (lkmanage(type, &filep->f_smfile->fl_flags, -2L))
	    {
	    iserrno = EFLOCKED;
	    return(-1);
	    }
#endif /* SHMLOCK */

#ifdef RDSLOCK
	if (lkmanage(type, filep->f_dev, filep->f_inum, -1L))
	    {
	    iserrno = errno;
	    iserrio = IO_LOCK+IO_IDX;
	    return(-1);
	    }
#endif /* RDSLOCK */

#ifdef SYS5LOCK
	flk.l_type = (type == UNLOCK ? F_UNLCK :
		     (type == LOCKUNCOND ? F_WRLCK : F_RDLCK));

	flk.l_whence = 0;	/* (UN)LOCK whole index file */
	flk.l_start  = 0;
	flk.l_len    = 0x3FFFFFFFL;

	if (fcntl(filep->f_idxfd, F_SETLKW, &flk) == -1)
	    {
	    iserrno = errno;
	    iserrio = IO_LOCK+IO_IDX;
	    return(-1);
	    }
#endif /* SYS5LOCK */

#ifdef ONIXLOCK
	register long lockbase, locksize;
#ifdef ZILOG
	lockbase = 0x200L;
	locksize = 0x3FFF0000L;
#else
	lockbase = 0x3FF00000L;		/* semaphores at end of file */
	locksize = 0x00000001L;
	if (! (type & LOCKSHARE))
	    locksize = 0x00010000L;
	else lockbase += (unsigned)openp->o_procid;
#endif /* ZILOG */
	type &= ~LOCKSHARE;
#ifdef MSNET
	if ( pclock(filep->f_idxfd, lockbase, locksize, type) )
#else
	if (lseek(filep->f_idxfd, lockbase, 0) != lockbase)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_IDX;
	    return(-1);
	    }
	if (locking(filep->f_idxfd, type, locksize) == -1)
#endif /* MSNET */
	    {
	    iserrno = errno;
	    iserrio = IO_LOCK+IO_IDX;
	    return(-1);
	    }
#endif /* ONIXLOCK */

#ifdef CREATLOCK
#ifdef SEMLOCK
	struct sembuf sop;
						/* setup semphore 	*/
	sop.sem_num = SEMHASH(filep->f_dev, filep->f_inum);
	sop.sem_op  = (type == UNLOCK ? 1 : -1);
	sop.sem_flg = SEM_UNDO;

	if (semop(filep->f_semid, &sop, 1) == -1) /* do semaphore	*/
	    {
	    iserrno = errno;
	    iserrio = IO_LOCK+IO_IDX;
	    return(-1);
	    }
#else
#ifdef BERK4_2
	if (flock(filep->f_idxfd, (type == UNLOCK ? 8 : 2)) == -1)
	    {
	    iserrno = errno;
	    iserrio = IO_LOCK+IO_IDX;
	    return(-1);
	    }
#else
	char fname[24];
	struct stat st;
	long dumtime, time();
	register int fd;

	mksemname(filep->f_dev, filep->f_inum, fname);
	if (type == UNLOCK)			/* unlock ?		*/
	    {
	    VOID unlink(fname);
	    }
	else
	    {
	    while ((fd = creat(fname, 0)) == -1) /* create lock file	*/
		{
		if (errno == EMFILE)		/* run out of open files ? */
		    break;			/* we did creat file	*/
		if ((errno != EEXIST && errno != EACCES) ||
			access("/isam", 07) == -1) /* access "/isam" ?	*/
		    {
		    iserrno = errno;
		    iserrio = IO_CREA+IO_SEM;
		    return(-1);
		    }
		if (stat(fname, &st) == 0 &&	/* aged semaphore ?	*/
			time(&dumtime)-st.st_ctime > TIMEAGED)
		    {
		    iserrno = EFLOCKED;		/* yes			*/
		    return(-1);
		    }
		sleep(1);
		}
	    VOID close(fd);
	    }
#endif /* BERK4_2 */
#endif /* SEMLOCK */
#endif /* CREATLOCK */

	if (type == UNLOCK)
	    openp->o_flags &= ~OSLOCK;
	else openp->o_flags |= OSLOCK;
	return(0);
}

/*
 * exclusive lock isam file
 *	type:	UNLOCK     - unlock file (closing file)
 *		LOCKCOND   - lock file (ordinary open)
 *		ISEXCLLOCK - lock file (exclusive open)
 */

STATIC int lkopenlock(type)
int type;
{
	register int convert = 0;	/* 1=ord to excl, 2=excl to ord	*/
	register long lockbase, locksize;

	if (type == UNLOCK)			/* close file		*/
	    {
	    if (openp->o_mode & ISEXCLLOCK)	/* close exclusive open	*/
		{
	        if (filep->f_xlkcnt > 1)
		    goto good;			/* keep lock		*/
		if (filep->f_ucount > 1)
		    convert = 2;		/* conv excl to ordin	*/
		}
	    else if (filep->f_ucount > 1)	/* close ordinary open	*/
		goto good;
	    }
	else if (type == ISEXCLLOCK)		/* exclusive open	*/
	    {
	    if (filep->f_xlkcnt)		/* already locked ?	*/
		goto good;
	    if (filep->f_ucount > 1)
		convert++;			/* conv ordin to excl	*/
	    }
	else					/* ordinary open	*/
	    {
	    if (filep->f_ucount > 1)		/* already locked ?	*/
		goto good;
	    }
convlock:					/* 2nd pass label	*/

#ifdef SHMLOCK
	if (type != UNLOCK)
	    type = (type == ISEXCLLOCK) ?
			(LOCK_X|LOCK_NOWAIT) : (LOCK_S|LOCK_NOWAIT);
	/* note that the file pointer used is not the same
	 * as the file pointer for record/file locks, this insures
	 * that this lock will not be released by lkrelse()
	 */
	if (lkmanage(type, &filep->f_smfile->fl_flags, -1L))
	    {
	    iserrno = EFLOCKED;
	    return(-1);
	    }
#endif /* SHMLOCK */

#ifdef RDSLOCK
	if (type != UNLOCK)
	    type = (type == ISEXCLLOCK) ? LOCKCOND : LOCKCOND|LOCKSHARE;
	if (lkmanage(type, filep->f_dev, filep->f_inum, -2L))
	    {
	    iserrno = EFLOCKED;
	    return(-1);
	    }
#endif /* RDSLOCK */

#ifdef SYS5LOCK
	flk.l_type = (type == UNLOCK ? F_UNLCK :
		     (type == ISEXCLLOCK ? F_WRLCK : F_RDLCK));

	flk.l_whence = 0;	/* (UN)LOCK whole index file */
	flk.l_start  = 0x40000000L;
	flk.l_len    = 0x3FFFFFFFL;

	if (fcntl(filep->f_idxfd, F_SETLK, &flk) == -1)
	    {
	    iserrno = EFLOCKED;
	    return(-1);
	    }
#endif /* SYS5LOCK */

#ifdef ONIXLOCK
#ifndef ZILOG
	lockbase = 0x40000000L;		/* semaphores at end of file */
	locksize = 0x00000001L;
	if (type == ISEXCLLOCK || type == UNLOCK)
	    locksize = 0x00010000L;
	else lockbase += (unsigned)openp->o_procid;
#else   /* ZILOG */
	lockbase = 200;			/* semaphores in dictionary */
	locksize = 1;
	if (type == ISEXCLLOCK || type == UNLOCK)
	    locksize = 200;
tryzilog:
#endif  /* ZILOG */

#ifdef MSNET
	if (pclock(filep->f_idxfd,lockbase, locksize,
		(type == UNLOCK)? UNLOCK: LOCKCOND))
#else
	if (lseek(filep->f_idxfd, lockbase, 0) != lockbase)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_IDX;
	    return(-1);
	    }
	if (locking(filep->f_idxfd,
		(type == UNLOCK)? UNLOCK: LOCKCOND, locksize) == -1)
#endif /* MSNET */
	    {
#ifdef ZILOG
	    if (errno == EACCES && locksize == 1 && (lockbase += 2) < 400)
		goto tryzilog;			/* try other semaphores	*/
#endif /* ZILOG */
	    if (errno != EACCES)
		{
		iserrno = errno;
		iserrio = IO_LOCK+IO_IDX;
		}
	    else iserrno = EFLOCKED;
	    return(-1);				/* just return error	*/
	    }
#endif /* ONIXLOCK */

#ifdef CREATMEM   /* => CREATLOCK or MEMLOCK */
	if (lkread()) return(-1);
	switch(type)
	    {
	    case UNLOCK:
		if (locks.l_nopens == -1)
		    {
		    locks.l_nopens = 0;
		    locks.l_filelock = 0;
		    }
		else if (locks.l_nopens > 0)
		    locks.l_nopens--;
		break;

	    case ISEXCLLOCK:
		if (locks.l_nopens == -1)
		    {
		    if (locks.l_filelock <= 0)
			goto bad;
		    if (kill(locks.l_filelock, 0) && errno == ESRCH)
			{
			locks.l_nopens = 0;
			locks.l_filelock = 0;
			}
		    else goto bad;
		    }
		if (convert ? (locks.l_nopens != 1) : (locks.l_nopens != 0))
		    goto bad;
		locks.l_nopens = -1;
		locks.l_filelock = openp->o_procid;
		break;

	    default:
		if (locks.l_nopens == -1)
		    {
		    if (locks.l_filelock <= 0)
			goto bad;
		    if (kill(locks.l_filelock, 0) && errno == ESRCH)
			{
			locks.l_nopens = 0;
			locks.l_filelock = 0;
			}
		    else goto bad;
		    }
		locks.l_nopens++;
		break;
	    }
	if (lkwrite() == 0) goto good;
	return(-1);
bad:
	iserrno = EFLOCKED;
	return(-1);
#endif  /* CREATMEM => CREATLOCK or MEMLOCK */

good:
	if (convert == 2)			/* conv excl to ordin	*/
	    {
	    if (type == UNLOCK)			/* 1st pass removed excl*/
		{
		type = LOCKCOND;		/* 2nd pass will add	*/
		goto convlock;			/*  ordinary lock	*/
		}
	    else type = UNLOCK;
	    }

	if (openp->o_mode & ISEXCLLOCK)		/* maintain lock count	*/
	    {
	    if (type == UNLOCK)
		 filep->f_xlkcnt--;		/* dec exclusive count	*/
	    else filep->f_xlkcnt++;		/* inc exclusive count	*/
	    }
	return(0);
}


/*
 * lock release -- release all 'read' locks
 *
 *  in a transaction 'write' locks must be held until
 *  commit time, however 'read' locks may be released
 *  earlier.  this routine releases the 'read' locks
 *  held on the current file.
 */

lkrelease()
{
#ifdef SHMLOCK
    lkrelse(1, filep->f_smfile);
#else  /* SHMLOCK */
    /* if there aren't any 'write' locks
     * then all the 'read' locks can be released
     * in one big chunk, else do them one-by-one
     */

    if (filep->f_wlock == NULL)		/* are there any 'write' locks	*/
	{
	if (filep->f_rlock)		/* are there any 'read' locks	*/
	    lkfile(UNLOCK);
	return;
	}

    /* loop thru all the 'read' locks releasing each one
     */

    while (filep->f_rlock)
	lkrecord(filep->f_rlock->rl_recnum, UNLOCK);
#endif /* SHMLOCK */
}


/*
 * lock a record
 *	recnum:	record number
 *	type:	UNLOCK   - unlock record
 *		LOCKCOND - lock record, return if locked
 *		LOCKUNCOND - lock record, wait if locked
 *		LOCKTEST - test for locked record
 *	    + ISREAD if this is a 'read' lock
 */

STATIC int lkrecord(recnum, type)
long recnum;
register int type;
{
	register long lockbase, locksize;
	register int i, readlock;

	if (filep->f_flkcnt  || (openp->o_mode & ISEXCLLOCK))
	    return(0);

	readlock = (type & ISREAD);
	type &= ~ISREAD;

#ifndef SHMLOCK
	if (type == UNLOCK)
	    {
	    /* remove record from lock lists
	     * if its not locked just return
	     */
	    if (rldelete(&filep->f_rlock, recnum) == 0 &&
		rldelete(&filep->f_wlock, recnum) == 0)
		    return 0;
	    }
	else
	    {
	    /* already 'write' locked ? 
	     *   just return
	     * already 'read' locked ?
	     *   convert to 'write' lock if necessary
	     *   then return
	     */
	    if (rlcheck(&filep->f_wlock, recnum))
		return 0;
	    if (rlcheck(&filep->f_rlock, recnum))
		{
		if (!readlock)
		    {
		    rldelete(&filep->f_rlock, recnum);
		    rlinsert(&filep->f_wlock, recnum);
		    }
		return 0;
		}
	    }
#endif /* SHMLOCK */

#ifdef SHMLOCK
	/* if want shared locks, use following code
	** if (readlock)
	**     i = (LOCK_IS|LOCK_NOWAIT);
	** else
	**/
	switch(type)
	    {
	    case LOCKCOND:
	    	i = (LOCK_IX|LOCK_NOWAIT);
		break;
	    case UNLOCK:
		i = UNLOCK;
		break;
	    default:
		i = LOCK_IX;
		if (type == LOCKTEST)
	    	    i |= LOCK_TEST;
	    }
	if (lkmanage(i, filep->f_smfile, 0L))
	    goto bad;

	/* if want shared locks, use following code
	** if (readlock)
	**     i = (LOCK_S|LOCK_NOWAIT);
	** else
	**/
	switch(type)
	    {
	    case LOCKCOND:
	    	i = (LOCK_X|LOCK_NOWAIT);
		break;
	    case UNLOCK:
		i = UNLOCK;
		break;
	    default:
	    	i = LOCK_X;
		if (type == LOCKTEST)
	    	    i |= LOCK_TEST;
	    }
	if (lkmanage(i, filep->f_smfile, recnum))
	    goto bad;
#endif /* SHMLOCK */

#ifdef RDSLOCK
	if (lkmanage(type, filep->f_dev, filep->f_inum, recnum))
	    goto bad;
#endif /* RDSLOCK */

#ifdef SYS5LOCK
	flk.l_type = (type == UNLOCK ? F_UNLCK : F_WRLCK);

	flk.l_whence = 0;
	flk.l_start  = 0x40000000L + recnum;
	flk.l_len    = 1;

	if (type == LOCKTEST)
		i = F_GETLK;
	else if (type == LOCKUNCOND)
		i = F_SETLKW;
	     else 
		i = F_SETLK;

	if (fcntl(filep->f_datfd, i, &flk) == -1)
	    goto bad;
	if (type == LOCKTEST && flk.l_type != F_UNLCK)
	    goto bad;
#endif /* SYS5LOCK */

#ifdef ONIXLOCK
#ifdef ZILOG
	locksize = filep->f_datsize;	/* zilog => lock actual records */
	lockbase = (recnum-1) * (locksize+1);
#else  /* ZILOG */
	lockbase = 0x40000000L+recnum;	/* onix => lock semaphores	*/
	locksize = 0x00000001L;
#endif /* ZILOG */

#ifdef MSNET
	if ( pclock(filep->f_datfd, lockbase, locksize, i) )
#else
	if (lseek(filep->f_datfd, lockbase, 0) != lockbase)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_DAT;
	    return(-1);
	    }

	if (type == UNLOCK)
		i = UNLOCK;
	else
		i = (type == LOCKUNCOND ? LOCKUNCOND : LOCKCOND);

	if (locking(filep->f_datfd, i, locksize) == -1)
#endif /* MSNET */
	    {
	    if (errno != EACCES)
		{
		iserrno = errno;
		iserrio = IO_LOCK+IO_DAT;
		}
	    goto bad;				/* just return error	*/
	    }

	if (type == LOCKTEST)
#ifdef MSNET
		VOID pclock(filep->f_datfd, lockbase, locksize, UNLOCK);
#else
	    VOID locking(filep->f_datfd, UNLOCK, locksize);
#endif /* MSNET */
#endif   /* ONIXLOCK */

#ifdef CREATMEM  /* => CREATLOCK or MEMLOCK */
	if (lkread()) goto bad;
	if (locks.l_filelock)
	    {
	    if (locks.l_filelock == -1) goto bad;
	    if (locks.l_filelock != openp->o_procid)
		{
		if (kill(locks.l_filelock, 0) && errno == ESRCH)
		    locks.l_filelock = 0;
		else goto bad;
		}
	    }

	for (i=0; i<NLOCKS; i++)
	    {					/* this does LOCKTEST	*/
	    if (locks.l_recnum[i] == recnum)
		{
		if (locks.l_procid[i] != openp->o_procid)
		    {
		    if (type == UNLOCK)
			return 0;
		    if (kill(locks.l_procid[i], 0) && errno == ESRCH)
						/* process id gone ?	*/
			{
			locks.l_procid[i] = 0;
			locks.l_recnum[i] = 0L;
			break;
			}
		    else goto bad;
		    }
		if (type == UNLOCK)
		    {
		    locks.l_procid[i] = 0;
		    locks.l_recnum[i] = 0L;
		    return lkwrite();
		    }
		return(0);			/* record already locked */
		}
	    }

	if (type == LOCKCOND || type == LOCKUNCOND)
	    {
	    for (i=0; i<NLOCKS; i++)		/* find empty lock slot	*/
		if (locks.l_procid[i] == 0) break;

	    if (i >= NLOCKS)
		goto bad;
	    locks.l_procid[i] = openp->o_procid;
	    locks.l_recnum[i] = recnum;		/* insert new lock	*/
	    if (lkwrite()) return(-1);
	    }
#endif  /* CREATMEM => CREATLOCK or MEMLOCK */

	if (type == LOCKCOND || type == LOCKUNCOND)
	    {
	    openp->o_flags |= ORLOCK;		/* some record locks	*/
	    filep->f_flags |= FLOCKS;
#ifndef SHMLOCK
	    rlinsert(readlock? &filep->f_rlock: &filep->f_wlock, recnum);
#endif  /* SHMLOCK */
	    }
	return(0);
bad:
	if (!iserrno) iserrno = ELOCKED;
	return(-1);
}


/*
 * whole file lock
 *	type:	UNLOCK   -- unlock whole file
 *		LOCKCOND -- lock whole file, return if locked
 */

STATIC int lkfile(type)
int type;
{
	register long lockbase, locksize;
#ifdef CREATMEM   /* => CREATLOCK or MEMLOCK */	/* TP */
	register int i;
#endif /* CREATMEM */

	if (openp->o_mode & ISEXCLLOCK)		/* exclusive mode ?	*/
	    return(0);				/* yes, no locking	*/
	
	if (type == UNLOCK)
	    {					/* should keep lock ?	*/
	    if ((openp->o_flags & (OFLOCK|ORLOCK)) == 0)
		return(0);			/* don't have any	*/
	    if (filep->f_flkcnt - ((openp->o_flags & OFLOCK) != 0))
		goto good;
	    }
	else
	    {
	    if (filep->f_flkcnt)		/* already locked ?	*/
		goto good;
	    }

#ifdef SHMLOCK
	if (type == UNLOCK)
	    lkrelse(0, filep->f_smfile);
	else
	    if (lkmanage(LOCK_X|LOCK_NOWAIT, filep->f_smfile, 0L))
		goto bad;
#endif /* SHMLOCK */

#ifdef RDSLOCK
	if (lkmanage(type, filep->f_dev, filep->f_inum, 0L))
	    goto bad;
#endif /* RDSLOCK */

#ifdef SYS5LOCK
	flk.l_type = (type == UNLOCK ? F_UNLCK : F_WRLCK);

	flk.l_whence = 0;
	flk.l_start  = 0x40000000L;
	flk.l_len    = 0x3FFFFFFFL;

	if (fcntl(filep->f_datfd, F_SETLK, &flk) == -1)
	    goto bad;
#endif /* SYS5LOCK */

#ifdef ONIXLOCK
#ifdef ZILOG
	lockbase = 0L;			/* zilog => lock actual records */
	locksize = 0L;
#else  /* ZILOG */
	lockbase = 0x40000000L;		/* onix => lock semaphores	*/
	locksize = 0x0FFFFFFFL;
#endif  /* ZILOG */

#ifdef MSNET
	if ( pclock(filep->f_datfd, lockbase, locksize, type) )
#else
	if (lseek(filep->f_datfd, lockbase, 0) != lockbase)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_DAT;
	    return(-1);
	    }

	if (locking(filep->f_datfd, type, locksize) == -1)
#endif /* MSNET */
	    {
	    if (errno != EACCES)
		{
		iserrno = errno;
		iserrio = IO_LOCK+IO_DAT;
		}
	    goto bad;
	    }
#endif /* ONIXLOCK */

#ifdef CREATMEM   /* => CREATLOCK or MEMLOCK */
	if (lkread()) return(-1);
	switch(type)
	    {
	    case UNLOCK:
		if (openp->o_flags & OFLOCK)
		    locks.l_filelock = 0;

		for (i=0; i<NLOCKS; i++)
		    if (locks.l_procid[i] == openp->o_procid)
			{
			locks.l_procid[i] = 0;
			locks.l_recnum[i] = 0L;
			}
		break;

	    case LOCKCOND:
		if (locks.l_filelock && !(openp->o_flags & OFLOCK))
		    {
		    if (locks.l_filelock == -1) goto bad;
		    if (kill(locks.l_filelock, 0) && errno == ESRCH)
			locks.l_filelock = 0;
		    else goto bad;
		    }

		for (i=0; i<NLOCKS; i++)
		    if (locks.l_procid[i] != 0 &&
			    locks.l_procid[i] != openp->o_procid)
			{
			if (kill(locks.l_procid[i], 0) && errno == ESRCH)
			    {
			    locks.l_procid[i] = 0;
			    locks.l_recnum[i] = 0L;
			    }
			else goto bad;
			}
		locks.l_filelock = openp->o_procid;
		break;
	    }
	if (lkwrite()) return(-1);
#endif  /* CREATMEM => CREATLOCK or MEMLOCK */

	if (type == UNLOCK)		/* save whether any locks */
	    {
	    filep->f_flags &= ~FLOCKS;
#ifndef SHMLOCK
	    while (filep->f_wlock)
		rldelete(&filep->f_wlock, filep->f_wlock->rl_recnum);
	    while (filep->f_rlock)
		rldelete(&filep->f_rlock, filep->f_rlock->rl_recnum);
#endif  /* SHMLOCK */
	    }
	else filep->f_flags |=  FLOCKS;

good:
	if (type == UNLOCK)
	    {
	    if (openp->o_flags & OFLOCK)
		filep->f_flkcnt--;
	    openp->o_flags &= ~(OFLOCK|ORLOCK);
	    }
	else
	    {
	    if ((openp->o_flags & OFLOCK) == 0)
		filep->f_flkcnt++;
	    openp->o_flags |= OFLOCK;
	    }
	return(0);
bad:
	if (!iserrno) iserrno = ELOCKED;
	return(-1);
}


#ifdef ISXACTION

/*
 * logical log locking -- system level lock
 *	provides serialization of logical logging calls
 *	must be called before writing to logical log file
 *	type:	UNLOCK     -- release semaphore
 *		LOCKUNCOND -- create semaphore
 */

lkloglog(type)
int type;
{
#ifdef SHMLOCK
	register smsema_t *sm = &shmem->sh_logsema;
	if (type != UNLOCK)
	     {smpsema(sm, userp);}
	else {smvsema(sm);}
			/* braces are needed for macro expansion */
#endif /* SHMLOCK */

#ifdef RDSLOCK
	if (lkmanage(type, filep->f_dev, filep->f_inum, -1L))
	    {
	    iserrno = errno;
	    iserrio = IO_LOCK+IO_IDX;
	    return(-1);
	    }
#endif /* RDSLOCK */

#ifdef SYS5LOCK
	flk.l_type = (type == UNLOCK ? F_UNLCK : F_WRLCK);

	flk.l_whence = 0;	/* (UN)LOCK whole index file */
	flk.l_start  = 0;
	flk.l_len    = 0;

	if (fcntl(islogisfd, F_SETLKW, &flk) == -1)
	    {
	    iserrno = errno;
	    iserrio = IO_LOCK+IO_IDX;
	    return(-1);
	    }
#endif /* SYS5LOCK */

#ifdef ONIXLOCK
#ifdef MSNET
	if ( pclock(islogisfd, 0L, 1L, type) )
#else
	if (lseek(islogisfd, 0L, 0) != 0L)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_IDX;
	    return(-1);
	    }
	if (locking(islogisfd, type, 0L) == -1)
#endif /* MSNET */
	    {
	    iserrno = errno;
	    iserrio = IO_LOCK+IO_IDX;
	    return(-1);
	    }
#endif /* ONIXLOCK */

#ifdef CREATLOCK
#ifdef SEMLOCK
	struct sembuf sop;
						/* setup semphore 	*/
	sop.sem_num = NUMSEMS;
	sop.sem_op  = (type == UNLOCK ? 1 : -1);
	sop.sem_flg = SEM_UNDO;

	if (semop(filep->f_semid, &sop, 1) == -1) /* do semaphore	*/
	    {
	    iserrno = errno;
	    iserrio = IO_LOCK+IO_IDX;
	    return(-1);
	    }
#else
#ifdef BERK4_2
	if (flock(islogisfd, (type == UNLOCK ? 8 : 2)) == -1)
	    {
	    iserrno = errno;
	    iserrio = IO_LOCK+IO_IDX;
	    return(-1);
	    }
#else
	cannot use CREATLOCK in transactions
#endif /* BERK4_2 */
#endif /* SEMLOCK */
#endif /* CREATLOCK */

	return(0);
}
#endif /* ISXACTION */


#ifdef CREATMEM /* => CREATLOCK or MEMLOCK */
/*
 * read lock file
 */

STATIC int lkread()
{
#ifdef CREATLOCK
	byfill(CASTCP &locks, sizeof(struct lock), 0);

	if (lseek(filep->f_lokfd, 0L, 0) != 0L)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_LOK;
	    return(-1);
	    }
	if (read(filep->f_lokfd, CASTCP &locks, sizeof(struct lock)) == -1)
	    {
	    iserrno = errno;
	    iserrio = IO_READ+IO_LOK;
	    return(-1);
	    }
#endif /* CREATLOCK */
	return(0);
}

/*
 * write lock file
 */

STATIC int lkwrite()
{
#ifdef CREATLOCK
	if (lseek(filep->f_lokfd, 0L, 0) != 0L)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_LOK;
	    return(-1);
	    }
	if (write(filep->f_lokfd, CASTCP &locks, sizeof(struct lock)) == -1)
	    {
	    iserrno = errno;
	    iserrio = IO_WRIT+IO_LOK;
	    return(-1);
	    }
#endif /* CREATLOCK */
	return(0);
}
#endif /* CREATMEM => CREATLOCK or MEMLOCK */


#ifdef  ZILOG
#ifndef ISKERNEL
/* 
 * convert ONIX locking to ZILOG locking
 */

STATIC int locking(fd, type, size)
int fd, type;
long size;
{
	struct lockblk
	    {
	    long lklen;
	    long lkoff;
	    int  lkwhnce;
	    } lkblk;
	long cc, unlk(), lkdata();

	lkblk.lklen = size;
	lkblk.lkoff = 0L;
	lkblk.lkwhnce = 0;

	switch(type)
	    {
	    case UNLOCK:
		cc = unlk(fd, 4, &lkblk);
		break;
	    case LOCKUNCOND:
		cc = lkdata(fd, 1, &lkblk);
		break;
	    case LOCKCOND:
		cc = lkdata(fd, 4, &lkblk);
		break;
	    }
	return(cc == -1L ? -1 : 0);
}
#endif  /* ISKERNEL */
#endif  /* ZILOG */

