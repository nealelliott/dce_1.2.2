/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isbuild.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:11  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:50  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:06  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:57:57  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:00:35  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:04:48  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:50:12  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:31  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:36:52  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:40:28  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isbuild.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:10 $";
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
 *  Title:	isbuild.c
 *  Description:
 *		isbuild()	-- build C-ISAM file
 *		isaddindex()	-- add a new index to C-ISAM file
 *		isdelindex()	-- delete an existing index from C-ISAM file
 *		iserase()	-- erase C-ISAM file
 *		isrename()	-- rename C-ISAM file
 *		isremlock()	-- remove old locks from C-ISAM file
 *
 ************************************************************************
 */


/*
 * isam create and delete routines
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 *
 *  06-07-83 RVH  V104 -- added kernel modifications
 *  10-13-83 RVH  V201 -- prevent deletion of current key
 */

#include <isdec.c>

#ifdef VMS
#define unlink qiodelete
#endif /* VMS */
/*
 * build isam file
 */

isbuild(filename, reclength, keydesc, mode)
    char *filename;		/* V104 NOTE: for kernel, an array of ptrs */
    int reclength;
    register struct keydesc *keydesc;
    register int mode;
{
	register int lockmode;
	register struct buff *bp;
	register int isfd;
#ifndef ISKERNEL
	char fname[PATHSIZE];
#endif  /* ISKERNEL */
	extern int real(), demo(), wrong(), snbadmsg();

#ifdef	SHMLOCK
	if (!shminit)
	    if (shmsetup()) goto reterr;
#endif	/* SHMLOCK */

	snverify(real, demo, wrong, snbadmsg);	/* valid serial number? */

	iserrno = iserrio = 0;

	if (reclength <= 0 || reclength > 32511)
	    {
	    iserrno = EROWSIZE;
	    goto reterr;
	    }

	if (mktemprec(reclength))		/* allocate temprecord	*/
	    goto reterr;

#ifndef ISKERNEL	/* V104 */
	if (ckfilname(filename)) goto reterr;
#endif
	snchk();

	if (keydesc->k_nparts == 0)		/* no primary key ?	*/
	    keydesc->k_flags = keydesc->k_len = 0;
	else if (kylegal(keydesc, reclength) != 0)
	    goto reterr;

	isbatch(2);

	if ((isfd = allocopen()) < 0)
	    goto reterr;

	openp->o_mode = mode & IOMASK;
	lockmode = (issingleuser ? ISEXCLLOCK : (mode & LOCKMASK));

	switch(lockmode)
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
		goto createrr;
	    }
	switch(openp->o_mode & (ISINPUT+ISINOUT))
	    {
	    case ISINPUT:
	    case ISOUTPUT:
	    case ISINOUT:
		break;
	    default:
		iserrno = EBADARG;
		goto createrr;
	    }

#ifndef ISKERNEL	/* V104 */
	mkidxname(filename, fname);		/* make filename.idx	*/
	if (makefile(fname, IDXMODE) == -1)	/* create index file	*/
	    {
	    iserrio = IO_CREA+IO_IDX;
	    goto createrr;
	    }

	mkdatname(filename, fname);		/* make filename.dat	*/
	if (makefile(fname, DATMODE) == -1)	/* create data file	*/
	    {
	    iserrio = IO_CREA+IO_DAT;
	    goto createrr;
	    }

#ifdef CREATLOCK
	mklokname(filename, fname);		/* make filename.lok	*/
	if (makefile(fname, LOKMODE) == -1)	/* create lock file	*/
	    {
	    iserrio = IO_CREA+IO_LOK;
	    goto createrr;
	    }
#endif /* CREATLOCK */
#endif /* ISKERNEL  */
						/* setup fileid		*/
	filep = openp->o_filep = allocfile(filename, openp->o_mode);
	if (filep == NULL)
	    goto allocerr;

	openp->o_mode += lockmode;		/* add in lock mode	*/

#ifdef SHMLOCK
	filep->f_smfile->fl_flags |= FDICTVAL;
#endif /* SHMLOCK */

	if (lksemaphore(LOCKUNCOND) || lkopenlock(lockmode))
	    goto createrr;

	filep->f_idxsize = NODESIZE-1;
	filep->f_datsize = reclength;
#ifdef MPM86
	filep->f_datsize |= 0177;		/* make 128 byte records*/
#endif /* MPM86 */

#ifdef SHMLOCK
	dictp = &filep->f_smfile->fl_dict;	/* point to dictionary	*/
#else  /* SHMLOCK */
	dictp = &filep->f_dict;			/* point to dictionary	*/
#endif /* SHMLOCK */

	st2(ISMAGIC,	dictp->d_magic);	/* init dictionary	*/
	st1(2,		dictp->d_dummy1+0);
	st1(2,		dictp->d_dummy1+1);
	st1(4,		dictp->d_dummy1+2);
	st1(4,		dictp->d_dummy1+3);
	st2(NODESIZE-1,	dictp->d_idxsize);
	st2(0,		dictp->d_nkeys);
	st1(COMPRESS>>1,dictp->d_cflag);
	st1(2,		dictp->d_dflag);
	st2(reclength,	dictp->d_recsize);
	st4(1L,		dictp->d_uniqid);
	st4(2L,		dictp->d_keysnode);
	st4(2L,		dictp->d_nextnode);

	dictmodify();

	bp = bfgetclr(2L);			/* get keys node block	*/
	bp->bf_used = 6;			/* init used bytes	*/
	bp->bf_level = 0x7E;
	bp->bf_type = 0xFF;
	bfdwrite(bp);
#ifdef	PCDOS
	/* write a junk byte to a DOS files, so it is not zero-length */

	write(filep->f_datfd, (char *) bp, 1);
#endif
	if (kyadd(keydesc) != 0)
	    iserrno = EBADKEY;

	kycopy(keydesc, &openp->o_curkey);	/* make it current key	*/

	if (iserrno == 0)
	    {
#ifdef ISXACTION
	    if (txflag != NOTRANS)
		{
		if (islogbuild(filename, reclength, keydesc, mode))
		    goto logerr;
		else
		    if (!(mode & ISNOLOG))
			if (islogfile(ISFOPEN, isfd, filename, mode))
			    goto logerr;	/* failed logging	*/
		}
#endif /* ISXACTION */
	    VOID isexit();
	    return (isfd);
	    }

logerr:
	VOID lkopenlock(UNLOCK);
	VOID isexit();

allocerr:
	mkidxname(filename, fname);		/* make filename.idx	*/
	unlink(fname);
	mkdatname(filename, fname);		/* make filename.dat	*/
	unlink(fname);
#ifdef CREATLOCK
	mklokname(filename, fname);		/* make filename.lok	*/
	unlink(fname);
#endif /* CREATLOCK */

createrr:
	if (openp->o_flags & OSLOCK)		/* semaphore lock on ?	*/
	    VOID lksemaphore(UNLOCK);		/* yes, release it	*/
	freeopen(openp);			/* free tables		*/
reterr:
	isstat1 = '9';
	return(-1);				/* return error	code	*/
}


/*
 * add an index to ISAM file
 */

isaddindex(isfd, keydesc)
    int isfd;
    register struct keydesc *keydesc;
{
	struct item item;
	register long recnum, lastnum;

	if (isenter(isfd, ISINOUT) == -1)	/* initialize		*/
	    return(-1);
	
	if (! (openp->o_mode & ISEXCLLOCK))
	    {
	    iserrno = ENOTEXCL;
	    return(isexit());
	    }

	if (kylegal(keydesc, filep->f_datsize) != 0)
	    goto bad;
	
	if (keydesc->k_flags & ~(ld1(dictp->d_cflag)<<1) & COMPRESS)
	    goto bad;				/* compression ok ?	*/

	if (kyadd(keydesc) != 0)
	    goto bad;
	
	tempisfd = -1;			/* mark temprecord destroyed	*/
	lastnum = ld4(dictp->d_nextdata);	/* last record written	*/
	for (recnum = 1L; recnum <= lastnum && iserrno == 0; recnum++)
	    {
	    if (dataread(temprecord, recnum) < 0)  /* read record	*/
		continue;			/* skip this record	*/
	    itmake(temprecord, &item, keydesc);
	    item.it_ptr = recnum;
	    VOID btadditem(&item, keydesc);
	    }

	if (iserrno != 0)
	    {					/* unable to add index	*/
	    btrecover(keydesc->k_rootnode, keydesc);
	    VOID kydelete(keydesc);		/* delete the key	*/
	    }
#ifdef ISXACTION
	else					/* index added		*/
	    {
	    if (txflag != NOTRANS)		/* has transaction	*/
		if (!(openp->o_mode & ISNOLOG))	/* requires logging	*/
		    if (islogidx(ISCINDEX, isfd, keydesc))
			{
			btrecover(keydesc->k_rootnode, keydesc);
			VOID kydelete(keydesc);	/* delete the key	*/
			}
	    }
#endif /* ISXACTION */
	return(isexit());

bad:
	if (iserrno == 0) iserrno = EBADKEY;
	return(isexit());
}

/*
 * delete an index from ISAM file
 */

isdelindex(isfd, keydesc)
    int isfd;
    register struct keydesc *keydesc;
{
	if (isenter(isfd, ISINOUT) == -1)	/* initialize		*/
	    return(-1);
/*
 * check for key exists and not primary key
 */
	if (! (openp->o_mode & ISEXCLLOCK))
	    {
	    iserrno = ENOTEXCL;
	    goto bad;
	    }

	if (kysearch(keydesc, 0) == 1)		/* check for primary key */
	    {
	    iserrno = EPRIMKEY;
	    goto bad;
	    }

	if (kycompare(keydesc, &openp->o_curkey) == 0)  /* V201 */
	    {					/* check for current key */
#ifndef SINIX	/* current key can be deleted. TP 30.11.87 */
	    iserrno = EBADKEY;
	    goto bad;
#else /* SINIX: current position afterwards undefined */
	    openp->o_flags &= ~(OCURPOS|OCURREAD);
#endif /* SINIX */
	    }
	
#ifdef ISXACTION
	if (txflag != NOTRANS)			/* has transaction	*/
	    if (!(openp->o_mode & ISNOLOG))	/* requires logging	*/
		if (islogidx(ISDINDEX, isfd, keydesc))
		    goto bad;
#endif /* ISXACTION */

	if (kydelete(keydesc) != 0)		/* delete the key desc	*/
	    {
	    iserrno = EBADKEY;
	    goto bad;
	    }
	
	btrecover(keydesc->k_rootnode, keydesc);/* recover btree nodes	*/

bad:
	return(isexit());
}

#ifndef ISKERNEL	/* V104 iserase, isrename, isremlock not in kernel */
/*
 * erase ISAM file
 */

iserase(filename)
    register char *filename;
{
	char fname[PATHSIZE];
	register int isfd;
#ifdef ISPCLOSE
	int sav;
	extern int ismaxfds;
#endif /* ISPCLOSE */

	isfd = isopen(filename, ISEXCLLOCK+ISINOUT);
	if (isfd < 0) return(-1);		/* iserrno already set	*/

	if (isaudit(isfd, fname, AUDGETNAME))	/* get audit filename	*/
	    fname[0] = 0;			/* no audit trail	*/

#ifdef ISPCLOSE
	sav = ismaxfds;
	ismaxfds = 0;
#endif /* ISPCLOSE */
#ifdef VMS
	smpsema(&shmem->sh_flsema,userp);
	filep->f_smfile->fl_flags |= FLDELETE;
	smvsema(&shmem->sh_flsema);
#endif /* VMS */
	isclose(isfd);
#ifdef ISPCLOSE
	ismaxfds = sav;
#endif /* ISPCLOSE */

#ifdef ISXACTION
	if (txflag != NOTRANS)			/* has transaction	*/
	    if (islogerase(filename))		/* log for rollforward	*/
		return -1;
#endif /* ISXACTION */

	iserrno = iserrio = 0;
	isstat1 = isstat2 = '0';
						/* remove audit trail	*/
	if (fname[0] && unlink(fname)) iserrno = errno;

#ifndef VMS
	mkidxname(filename, fname);		/* remove filename.idx	*/
	if (unlink(fname)) iserrno = errno;

	mkdatname(filename, fname);		/* remove filename.dat	*/
	if (unlink(fname)) iserrno = errno;
#endif /* VMS */

#ifdef CREATLOCK
	mklokname(filename, fname);		/* remove filename.lok	*/
	if (unlink(fname)) iserrno = errno;
#endif
						/* remove audit trail	*/
	if (iserrno == 0)
	    return 0;
	isstat1 = '9';
	return -1;
}

/*
 * remove ISAM lock files
 */

isremlock(filename)
    register char *filename;
{
#ifdef CREATLOCK
	char fname[PATHSIZE];
	register int fd;
	dev_t filedev;
	ino_t fileino;

	iserrno = iserrio = 0;
	if (ckfilname(filename)) goto reterr;

	mklokname(filename, fname);		/* trunc filename.lok	*/
	if ((fd = creat(fname, LOKMODE)) == -1)
	    {
	    iserrno = errno;
	    iserrio = IO_CREA+IO_LOK;
	    goto reterr;
	    }
	VOID close(fd);

	mkidxname(filename, fname);
	getfileid(fname, &filedev, &fileino);
	mksemname(filedev, fileino, fname);
	VOID unlink(fname);
#else
	iserrno = iserrio = 0;
	if (ckfilname(filename)) goto reterr;
#endif
	return(0);
reterr:
	isstat1 = '9';
	return -1;
}

/*
 * rename ISAM file
 */

#ifndef VMS
#ifndef MPM86
#ifndef PCDOS
#define rename(o,n)	(link(o,n)||unlink(o))
#endif
#endif
#endif /* VMS */

isrename(oldfilename, newfilename)
    register char *oldfilename;
    register char *newfilename;
{
#ifdef VMS
	char *cwdir;
#endif /* VMS */
	char oldfname[PATHSIZE];
	char newfname[PATHSIZE];
	register int isfd, err;

	iserrno = iserrio = 0;

	if (ckfilname(oldfilename)) goto reterr;
	if (ckfilname(newfilename)) goto reterr;

	if ((isfd = isopen(oldfilename, ISEXCLLOCK+ISINOUT)) < 0 ) goto reterr;

#ifdef VMS
	smpsema(&shmem->sh_flsema,userp);
	filep->f_smfile->fl_flags |= FLRENAME;
	smvsema(&shmem->sh_flsema);
#endif /* VMS */
	mkidxname(oldfilename, oldfname);	/* rename filename.idx	*/
	mkidxname(newfilename, newfname);
	if (rename(oldfname, newfname)) goto bad;

	mkdatname(oldfilename, oldfname);	/* rename filename.dat	*/
	mkdatname(newfilename, newfname);
	if (rename(oldfname, newfname)) goto bad;

#ifdef CREATLOCK
	mklokname(oldfilename, oldfname);	/* rename filename.lok	*/
	mklokname(newfilename, newfname);
	if (rename(oldfname, newfname)) goto bad;
#endif

	stcopy(newfilename,filep->f_fname);

#ifdef ISXACTION
	if (txflag != NOTRANS)			/* has transaction	*/
	    if (!(openp->o_mode & ISNOLOG))	/* requires logging	*/
		if (islogrename(oldfilename, newfilename))
		    goto badlog;
#endif /* ISXACTION */

#ifdef VMS
	if (newfname[0] != '/') cwdir = getenv("PATH");
	smpsema(&shmem->sh_flsema,userp);
	if (newfname[0] != '/')
	  stcopy(cwdir,filep->f_smfile->fl_fname);
	else
	  filep->f_smfile->fl_fname[0] = 0;
	stcat(newfname,filep->f_smfile->fl_fname);
	filep->f_smfile->fl_flags &= ~FLRENAME;
	smvsema(&shmem->sh_flsema);
#endif /* VMS */

	isclose(isfd);
	return(0);
bad:
	iserrno = errno;
#ifdef ISXACTION
badlog:
#endif /* ISXACTION */
#ifdef VMS
	smpsema(&shmem->sh_flsema,userp);
	filep->f_smfile->fl_flags &= ~FLRENAME;
	smvsema(&shmem->sh_flsema);
#endif /* VMS */
	err = iserrno;
	isclose(isfd);
	iserrno = err;
reterr:
	isstat1 = '9';
	return(-1);
}
#endif /* ISKERNEL */

/*
 * recover btree nodes from deleted index
 */

STATIC int btrecover(node, keydesc)
    long node;
    register struct keydesc *keydesc;
{
	struct item tmpitem;
	register struct item *ip;
	register int offset, used;
	register struct buff *bp;
	register char *np;

	if (node == 0) return;			/* down to bottom ?	*/

	bp = bfread(node);
	if (bp == NULL) return;
#ifndef VMS
	np = bp->bf_un.bf_node->n_items;
#else /* VMS */
	np = ((struct node *) GSADDRESS(bp->bf_un.bf_node))->n_items;
#endif /* VMS */
	ip = &tmpitem;
	if (bp->bf_level != 0)			/* not a leaf node ?	*/
	    {
	    used = bp->bf_used - 2;
	    for (offset = 0; offset < used; )
		{
		offset = itunpack(np, offset, ip, keydesc);
		btrecover(ip->it_ptr, keydesc);
		}
	    }
	bfrelease(bp);
	freerec(node, INDXREC);			/* free index record	*/
}
