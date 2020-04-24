/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: issupp.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:26  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:30:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:01  root]
 * 
 * Revision 1.1.8.2  1994/06/21  14:44:50  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:29:10  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:59:41  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:10:51  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:06:56  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:57:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:16:13  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:23:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  17:48:46  marrek]
 * 
 * Revision 1.1.2.3  1992/06/01  23:38:11  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:41:30  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/09  20:07:38  jim
 * 	Comment strings after #endif and #else statements.
 * 	[1992/04/09  19:33:09  jim]
 * 
 * Revision 1.1  1992/01/19  15:12:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: issupp.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:22 $";
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
 *  Title:	issupp.c
 *  Description:
 *		Support routines for C-ISAM routines.
 *
 ************************************************************************
 */


/*
 * support routines
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 *
 *  06-07-83  RVH	V104 -- added kernel modifications
 *  08-24-83  RVH	V105 -- dynamic memory allocation
 *  03-02-84  RVH	V202 -- close files on exec()
 *  05-11-84  DKE	moved isam-specific functions here from isutil.c
 */

#include <isdec.c>

#ifdef ISPCLOSE
int ismaxfds = 0;
static int isopenfds = 0;
#endif /* ISPCLOSE */
/*
 * allocate an open file entry
 * returns isfd
 */

STATIC int allocopen()
{
	register int isfd;

	for (isfd=0; isfd<nopens; isfd++)
	    {
#ifdef ISKERNEL
	    register struct kobuf *kopenp;
	    kopenp = &kotab[isfd];
	    if (kopenp->ko_pid != 0)
		continue;
	    if ((kopenp->ko_buf = alloc(pipedev)) == NULL)
		goto bad;			/* allocate block	*/
	    kopenp->ko_pid = getpid();
	    kopenp->ko_blkno = kopenp->ko_buf->b_blkno;
	    openp = (struct iopen *) kopenp->ko_buf->b_un.b_addr;
	    openp->o_kopenp = kopenp;
	    openp->o_procid = getpid();
	    openp->o_userid = getuid();
	    openp->o_flags = OBUSY;
	    return(isfd);
#else
	    openp = opens[isfd];
#ifdef ISALLOC					/* dynamic allocation	*/
	    if (openp == NULL)			/* V105	*/
		{
		opens[isfd] = openp =
			(struct iopen *) malloc(sizeof(struct iopen));
		if (openp == NULL)
		    goto bad;
		openp->o_flags = 0;
		}
#else
	    if (openp == NULL)			/* static allocation	*/
		opens[isfd] = openp = &opentab[isfd];
#endif /* ISALLOC */
	    if (openp->o_flags == 0)		/* find free open	*/
		{
		byfill(CASTCP openp, sizeof(struct iopen), 0);
		openp->o_procid = getpid();
		openp->o_userid = getuid();
		openp->o_flags = OBUSY;
		return(isfd);
		}
#endif /* ISKERNEL */
	    }
	iserrno = ETOOMANY;
	return(-1);
bad:
	iserrno = EBADMEM;
	return(-1);
}

STATIC int freeopen(op)
register struct iopen *op;
{
	if (op->o_filep != NULL)
	    freefile(op->o_filep);		/* free file struct	*/
	op->o_flags = 0;
#ifdef ISKERNEL
	{
	register struct kobuf *kop;

	kop = op->o_kopenp;
	kop->ko_pid = 0;
	if (kop->ko_buf) brelse(kop->ko_buf);
	kop->ko_buf = NULL;
	if (kop->ko_blkno) free(pipedev, kop->ko_blkno);
	kop->ko_blkno = 0L;
	}
#endif /* ISKERNEL */
}


/*
 * allocate a isam file block
 */

STATIC struct ifile *allocfile(filename, mode)
char *filename;		/* V104 -- NOTE: for kernel this is array of ptrs */
register int mode;
{
	register struct ifile *fp, *zfp;
	register int i;
	dev_t filedev;
	ino_t fileino;
#ifdef VMS
	char newname[PATHSIZE], dirname[PATHSIZE];
	struct stat dir_stat;
#endif /* VMS */
#ifdef ISVOPEN
 	int secondtry;
#endif /* ISVOPEN */
#ifdef ISKERNEL
	char *fname;
#else
	char fname[PATHSIZE];
#endif /* ISKERNEL */

	secondtry = 0;

	mkidxname(filename, fname);

#ifndef PCDOS
	for (i=0; i<nfiles && (fp = files[i]) != NULL; i++)
		if (fp->f_ucount >= 0 &&  stcmpr(filename,fp->f_fname) == 0 )
			goto found;	/* found file name */

	if (getfileid(fname, &filedev, &fileino))
	    return(NULL);			/* get file identifier	*/

#ifdef BERK4_2
#ifndef SYS5LOCK
	if ( filedev < 0 )
	    {
	    iserrno = ENONFS;
	    return NULL;
	    }
#endif
#endif /* BERK4_2 */

#endif /* PCDOS */

retryfind:
	zfp = NULL;

	for (i=0; i<nfiles; i++)
	    {
	    fp = files[i];			/* search for existing	*/
#ifdef ISALLOC
	    if (fp == NULL && zfp == NULL)	/* dynamic allocation	*/
		{
		files[i] = fp =
			(struct ifile *) malloc(sizeof(struct ifile));
		if (fp == NULL)
		    {
		    iserrno = EBADMEM;
		    return(NULL);
		    }
		fp->f_ucount = -1;
		}
#else
	    if (fp == NULL)			/* static allocation	*/
		files[i] = fp = &filetab[i];
#endif /* ISALLOC */
#ifndef PCDOS
#ifdef VMS
	    if (fp && fp->f_ucount >= 0 &&
		fp->f_inum.ino_t_array[0] == fileino.ino_t_array[0] &&
		fp->f_inum.ino_t_array[1] == fileino.ino_t_array[1] &&
		fp->f_inum.ino_t_array[2] == fileino.ino_t_array[2] &&
		stcmpr(fp->f_dev.dev_t_array, filedev.dev_t_array))
#else /* VMS */
	    if (fp && fp->f_ucount >= 0 && fp->f_inum == fileino &&
		    fp->f_dev == filedev)
#endif /* VMS */
#else /* PCDOS */
	    if (fp && fp->f_ucount >= 0 &&  stcmpr(filename,fp->f_fname) == 0 )
#endif /* PCDOS */
		goto found;			/* match fileid		*/
	    if (zfp == NULL && fp->f_ucount  == -1)
		zfp = fp;			/* save empty one	*/
	    }

	if (zfp == NULL)
	    {
#ifdef ISPCLOSE
	    if ( !secondtry )
		{
		vclose();
		secondtry = 1;
		goto retryfind;
		}
#endif /* ISPCLOSE */
	    iserrno = ETOOMANY;
	    return(NULL);
	    }

	fp = zfp;				/* use empty one	*/
	byfill(CASTCP fp, sizeof(struct ifile), 0);
	fp->f_ucount = -1;

	i = RWMODE;
#ifdef ISVOPEN
	secondtry = 0;
#endif /* ISVOPEN */
idxagain:
	usrmemory();
	if ((fp->f_idxfd = open(fname, i)) == OPENERR)
	    {
#ifndef ZILOG				/* zilog locking requires RWMODE */
#ifndef SYS5LOCK			/* fcntl locking requires RWMODE */
	    if (errno == EACCES && i == RWMODE && mode == ISINPUT)
		{
		i = RMODE;
		goto idxagain;
		}
#endif /* not SYS5LOCK */
#endif /* not ZILOG */
#ifdef ISVOPEN
	    if (errno == EMFILE && !secondtry) 
		{
		vclose();
		secondtry = 1;
		goto idxagain;
		}
#endif /* ISVOPEN */
	    iserrno = errno;
	    iserrio = IO_OPEN+IO_IDX;
	    return(NULL);
	    }
	CLOSEXEC(fp->f_idxfd);		/* V202 - close file on exec()	*/
#ifdef ISPCLOSE
	isopenfds++;
#endif /* ISPCLOSE */

#ifdef ISKERNEL
	if (klock(fp->f_idxfd))		/* insure that no one has file	*/
	    {
	    iserrno = EFLOCKED;
	    VOID close(fp->f_idxfd);
	    return(NULL);
	    }
#endif /* ISKERNEL */

	mkdatname(filename, fname);
	i = RWMODE;
#ifdef ISVOPEN
	secondtry = 0;
#endif /* ISVOPEN */
datagain:
	usrmemory();
	if ((fp->f_datfd = open(fname, i)) == OPENERR)
	    {
	    if (errno == EACCES && i == RWMODE && mode == ISINPUT)
		{
		i = RMODE;
		goto datagain;
		}
#ifdef ISVOPEN
	    if (errno == EMFILE && !secondtry) 
		{
		vclose();
		secondtry = 1;
		goto datagain;
		}
#endif /* ISVOPEN */
	    iserrno = errno;
	    iserrio = IO_OPEN+IO_DAT;
#ifdef ISKERNEL
	    kunlock(fp->f_idxfd);
#endif /* ISKERNEL */
	    VOID close(fp->f_idxfd);
	    return(NULL);
	    }
#ifdef ISVOPEN
	stcopy(filename,fp->f_fname);
	fp->f_omode = i;
#endif /* ISVOPEN */
	CLOSEXEC(fp->f_datfd);		/* V202 - close file on exec()	*/

#ifdef CREATLOCK
	mklokname(filename, fname);
	usrmemory();
#ifdef ISVOPEN
	secondtry = 0;
lokagain:
#endif /* ISVOPEN */
	if ((fp->f_lokfd = open(fname, RWMODE)) == -1)
	    if (errno == ENOENT) 
		{
		if ((i = creat(fname, 0777)) != -1 )
		    {
		    close(i);
		    fp->f_lokfd = open(fname, RWMODE);
		    }
		}
	if (fp->f_lokfd  == -1)
	    {
#ifdef ISVOPEN
	    if (errno == EMFILE && !secondtry) 
		{
		vclose();
		secondtry = 1;
		goto lokagain;
		}
#endif /* ISVOPEN */
	    iserrno = errno;
	    iserrio = IO_OPEN+IO_LOK;
	    VOID close(fp->f_idxfd);
	    VOID close(fp->f_datfd);
	    return(NULL);
	    }
	CLOSEXEC(fp->f_lokfd);		/* V202 - close file on exec()	*/
#endif               

#ifndef VMS
	fp->f_dev  = filedev;
	fp->f_inum = fileino;
#else /* VMS */
	strip(fname, &newname, &dirname);
	dirstat(&dirname, &dir_stat);
        fp->f_st_ino.ino_t_array[0] = dir_stat.st_ino.ino_t_array[0];
        fp->f_st_ino.ino_t_array[1] = dir_stat.st_ino.ino_t_array[1];
        fp->f_st_ino.ino_t_array[2] = dir_stat.st_ino.ino_t_array[2];
      	stcopy(filedev.dev_t_array, fp->f_dev.dev_t_array);
	fp->f_inum.ino_t_array[0] = fileino.ino_t_array[0];
	fp->f_inum.ino_t_array[1] = fileino.ino_t_array[1];
	fp->f_inum.ino_t_array[2] = fileino.ino_t_array[2];
	getfileid(fname,&filedev,&fileino);
	fp->f_datinum.ino_t_array[0] = fileino.ino_t_array[0];
	fp->f_datinum.ino_t_array[1] = fileino.ino_t_array[1];
	fp->f_datinum.ino_t_array[2] = fileino.ino_t_array[2];
#endif /* VMS */
	fp->f_ucount = 0;
#ifdef SHMLOCK
#ifndef VMS
	smflalloc(fp);
#else /* VMS */
	if (smflalloc(fp)) 
          {
            fp->f_smfile = 0;
            return (NULL);
          }
#endif /* VMS */
#endif /* SHMLOCK */
#ifdef SEMLOCK
	initsem(fp);
#endif /* SEMLOCK */

found:
	fp->f_ucount++;
	return(fp);
}

STATIC int freefile(fp)
register struct ifile *fp;
{
	if (--fp->f_ucount > 0) return;		/* still in use		*/
#ifdef ISPCLOSE
	if ( isopenfds < ismaxfds && fp->f_ucount == 0)
			return;
	isopenfds--;
#endif /* ISPCLOSE */

#ifdef SHMLOCK
#ifdef VMS
        fp->f_dev.dev_t_array[0] = '\0';
	fp->f_inum.ino_t_array[0] = 0;
	fp->f_inum.ino_t_array[1] = 0;
	fp->f_inum.ino_t_array[2] = 0;
        filep = fp;
    if (fp->f_smfile)
      {
        dictp = &fp->f_smfile->fl_dict;
	VOID close(fp->f_idxfd);
	VOID close(fp->f_datfd);
#endif /* VMS */
	smflfree(fp);
#ifdef VMS
      }   
#endif /* VMS */
#endif /* SHMLOCK */
#ifndef SHMBUFF
	bfignor(fp);
#endif  /* SHMBUFF */
	if (fp->f_flags & FAUDOPEN)
#ifndef VMS
	    VOID close(fp->f_audfd);
#else /* VMS */
	    VOID rmsclose(fp->f_audfd);
#endif /* VMS */
	fp->f_ucount = -1;
	fp->f_flags = 0;			/* deallocate table	*/
#ifndef VMS
	fp->f_dev   = -1;
	fp->f_inum  = -1;
#else /* VMS */
        fp->f_dev.dev_t_array[0] = '\0';
	fp->f_inum.ino_t_array[0] = 0;
	fp->f_inum.ino_t_array[1] = 0;
	fp->f_inum.ino_t_array[2] = 0;
#endif /* VMS */
#ifdef ISKERNEL
	kunlock(fp->f_idxfd);
#endif /* ISKERNEL */
#ifndef VMS
	VOID close(fp->f_idxfd);
	VOID close(fp->f_datfd);
#endif /* VMS */
#ifdef CREATLOCK
	VOID close(fp->f_lokfd);
#endif
}

#ifdef SEMLOCK
/*
 * for SYSTEM V semaphores, initialize info
 */

initsem(fp)
struct ifile *fp;
{
	register int semid, i;
	union semun {
		int val;
		struct semid_ds *buf;
		unsigned16 *array;
	} semctl_arg;

retry:
	semid = semget(SEMKEY, NUMSEMS+1, 0666);
	if (semid == -1 && errno == ENOENT)
	    {
	    semid = semget(SEMKEY, NUMSEMS+1, IPC_CREAT|IPC_EXCL|0666);
	    if (semid == -1 && errno == EEXIST)
		goto retry;
	    semctl_arg.val = 1;
	    for (i=0; i<NUMSEMS+1; i++)
		if (semid >= 0 && semctl(semid, i, SETVAL, semctl_arg) == -1)
		    semid = -1;
	    }
	fp->f_semid = semid;
}
#endif /* SEMLOCK */


/*
 * allocate index or data record
 */

STATIC long allocrec(flag)
int flag;
{
	register struct buff *bp;
	register char *freep, *nextp;
	register long node, node2;

#ifdef VMS
        int datsize,daddr,offset,cnt;
        char *ptr;
#endif /* VMS */

	if (flag == INDXREC)			/* INDXREC or DATAREC	*/
	    {
	    freep = dictp->d_freenode;
	    nextp = dictp->d_nextnode;
	    }
	else
	    {
	    freep = dictp->d_freedata;
	    nextp = dictp->d_nextdata;
	    }

next:
	node = ld4(freep);
	if (node == 0)				/* no free nodes	*/
	    {
extend:	    node = ld4(nextp) + 1L;
	    st4(node, nextp);			/* extend file		*/
	    dictmodify();
#ifdef VMS
            if (flag == DATAREC)
              {
                datsize = filep->f_datsize + 1;
                daddr = (node-1) * datsize;
                offset = daddr & (NODESIZE - 1);
                daddr = (daddr/NODESIZE) + DATABUF;
                while (datsize > 0)
                  {
                    cnt = NODESIZE - offset;
                    if (cnt < NODESIZE)
                      bp = bfread(daddr);
                    else
                      bp = bfgetblk(daddr);
                    if (bp == NULL)
                      break;
                    ptr = ((char *) GSADDRESS(bp->bf_un.bf_addr)) + offset;
                    byfill(ptr,cnt,'\0');
                    bfdwrite(bp);
                    datsize -= cnt;
                    offset = 0;
                    daddr++;
                  }
              }
            else
              {
                bp = bfgetclr(node);
                bfdwrite(bp);
              }
#endif /* VMS */
	    }
	else
	    {
	    bp = bfread(node);			/* get free list	*/
	    if (bp == NULL) goto extend;
	    bp->bf_used -= 4;			/* get next free node	*/
#ifdef VMS
	    node2 = ld4(((char *) GSADDRESS(bp->bf_un.bf_addr))+bp->bf_used);
#else /* VMS */
	    node2 = ld4(bp->bf_un.bf_addr+bp->bf_used);
#endif /* VMS */
	    if (bp->bf_used <= 2)
		{
		st4(node2, freep);
		dictmodify();
		if (flag != INDXREC)
		    {
		    bfrelease(bp);
		    freerec(node, INDXREC);	/* free index record	*/
		    goto next;
		    }
		}
	    else
		{
		node = node2;
		}
	    bfdwrite(bp);
	    }
	return(node);
}

/*
 * free index or data record
 */

STATIC int freerec(node, flag)
long node;
int flag;
{
	register struct buff *bp;
	register char *freep, *nextp;
	register long fnode, node2;

	if (flag == INDXREC)			/* INDXREC or DATAREC	*/
	    {
	    freep = dictp->d_freenode;
	    nextp = dictp->d_nextnode;
	    }
	else
	    {
	    freep = dictp->d_freedata;
	    nextp = dictp->d_nextdata;
	    }

	if (node == ld4(nextp))			/* freeing last record ?*/
	    {
	    node--;
	    st4(node, nextp);			/* yes, just shrink file*/
	    dictmodify();
	    return;
	    }

	fnode = ld4(freep);
	if (fnode == 0)				/* no free list ?	*/
	    {
newfree:
	    if (flag != INDXREC)
		node2 = allocrec(INDXREC);
	    else node2 = node;

	    st4(node2, freep);			/* create free list	*/
	    dictmodify();
	    bp = bfgetclr(node2);
	    bp->bf_used = 6;
	    bp->bf_level = 0x7F;
	    bp->bf_type = (flag == INDXREC)? 0xFE: 0xFF;
#ifdef VMS
	    st4(fnode, ((struct free *) GSADDRESS(bp->bf_un.bf_free))->fn_next);
#else /* VMS */
	    st4(fnode, bp->bf_un.bf_free->fn_next);
#endif /* VMS */
	    if (flag != INDXREC)
		{
#ifdef VMS
	  	st4(node, ((char *) GSADDRESS(bp->bf_un.bf_addr))+6);
#else /* VMS */
                st4(node, bp->bf_un.bf_addr+6);	/* store free node	*/
#endif /* VMS */
		bp->bf_used = 10;
		}
	    bfdwrite(bp);
	    }
	else
	    {
	    bp = bfread(fnode);			/* get free list	*/
	    if (bp == NULL) return;
	    if (bp->bf_used+4 < filep->f_idxsize-2)
		{
#ifdef VMS
		st4(node, ((char *) GSADDRESS(bp->bf_un.bf_addr))+bp->bf_used);
#else /* VMS */
	       	st4(node, bp->bf_un.bf_addr+bp->bf_used);
#endif /* VMS */
		bp->bf_used += 4;
		}
	    else
		{
		bfrelease(bp);
		goto newfree;
		}
	    bfdwrite(bp);
	    }
}

#ifndef ISKERNEL	/* V104 */
/*
 * make a new file
 */

STATIC int makefile(fname, mode)
register char *fname;
int mode;
{
	register int fd;
#ifdef ISVOPEN
 	int secondtry =0;
tryagain:
#endif /* ISVOPEN */

	errno = EEXIST;
	if ((fd = open(fname, RWMODE)) != -1 || errno != ENOENT)
#ifdef ISVOPEN
	    if (errno == EMFILE && !secondtry) 
		{
		vclose();
		secondtry = 1;
		goto tryagain;
		}
	    else
#endif /* ISVOPEN */
	    goto bad;				/* file already exist ?	*/

#ifdef ISVOPEN
 	secondtry =0;
creagain:
#endif /* ISVOPEN */
	if ((fd = creat(fname, mode)) == -1)
#ifdef ISVOPEN
	    if (errno == EMFILE && !secondtry) 
		{
		vclose();
		secondtry = 1;
		goto creagain;
		}
	    else
#endif /* ISVOPEN */
	    goto bad;				/* create file		*/

#ifndef VMS
	VOID close(fd);
#else /* VMS */
        qioclosenew(fd);
#endif /* VMS */
	return(0);
bad:
	iserrno = errno;			/* system error		*/
	if (fd != -1) VOID close(fd);
	return(-1);
}
#endif


#ifndef MPM86
#ifndef PCDOS
#ifndef ISKERNEL

/*
 * return unique file identifier
 */

STATIC int getfileid(fname, devp, inump)
char *fname;
dev_t *devp;
ino_t *inump;
{
	struct stat st;

	if (stat(fname, &st))		/* get device & inode num */
	    {
	    iserrno = errno;
	    return(-1);
	    }
#ifndef VMS
	*devp  = st.st_dev;
	*inump = st.st_ino;
#else /* VMS */
        stcopy(st.st_dev, devp->dev_t_array);
	inump->ino_t_array[0] = st.st_ino[0];
	inump->ino_t_array[1] = st.st_ino[1];
	inump->ino_t_array[2] = st.st_ino[2];
#endif /* VMS */
	return(0);
}

#else /* ISKERNEL */

STATIC int getfileid(fname, devp, inump)
char *fname;
dev_t *devp;
ino_t *inump;
{
	register struct inode *fd;

	usrmemory();
	if ((fd = open(fname, RMODE)) == OPENERR)
	    return(-1);
	*devp  = fd->i_dev;
	*inump = fd->i_number;
	close(fd);
	return(0);
}
#endif /* ISKERNEL */
#endif /* not PCDOS */
#endif /* not MPM86 */


#ifdef ISBUFDATA
/*
 * read a data record into "record"
 */

dataread(record, recnum)
    char *record;
    long recnum;
{
    return dataxfer(0, record, recnum);
}

/*
 * write a data record from "record"
 */

datawrite(record, recnum)
    char *record;
    long recnum;
{
    return dataxfer(1, record, recnum);
}

/*
 * delete a data record
 */

datadelete(recnum)
    long recnum;
{
    return dataxfer(2, temprecord, recnum);
}

/*
 * check a data record
 */

datacheck(recnum)
    long recnum;
{
    return dataxfer(3, temprecord, recnum);
}


/*
 * tranfer data record to/from cached buffers
 */

dataxfer(flag, record, recnum)
    int flag;		/* 0=read, 1=write, 2=delete, 3=check */
    char *record;
    long recnum;
{
    register char *ptr;
    register int cnt, n, datsize, offset;
    register struct buff *bp;
    register long daddr;
    register int cc = 0;

    datsize = filep->f_datsize + 1;

    daddr = (recnum-1) * datsize;
    if (flag == 3)			/* datacheck */
	{
	daddr += datsize - 1;		/* point to terminating byte	*/
	datsize = 1;
	}
    offset = daddr & (NODESIZE-1);
    daddr = (daddr / NODESIZE) + DATABUF;
    while (datsize)
	{
	/* cnt = # bytes to be transferred
	 */
	cnt = NODESIZE - offset;
	if (cnt > datsize) cnt = datsize;
	/*
	 * get the buffer -- no need to read if whole
	 *  block will be overwritten
	 */
	if (flag == 0 || flag == 3 || cnt < NODESIZE)
	     bp = bfread(daddr);
	else bp = bfgetblk(daddr);
	if (bp == NULL)
	    {
	    cc = -1;
	    break;
	    }
	/*
	 * do actual transfers
	 */
#ifdef VMS
	ptr = ((char *) GSADDRESS(bp->bf_un.bf_addr))+offset;
#else /* VMS */
	ptr = bp->bf_un.bf_addr+offset;
#endif /* VMS */
	switch (flag)
	    {
	    case 0:			/* dataread	*/
		    n = cnt;
		    if ( cnt == datsize )
			{
			n--;
			cc = *(ptr+n) == '\n' ? 0  : -1;
		    	}
		    bycopy(ptr, record, n);
		    bfrelease(bp);
		    break;
	    case 1:			/* datawrite	*/
		    n = cnt;
		    if ( cnt == datsize )
			{
			n--;
			*(ptr+n) = '\n';
		    	}
		    bycopy(record, ptr, n);
		    cc = bfwrite(bp);
		    break;
	    case 2:			/* datadelete	*/
		    byfill(ptr, cnt, 0);
		    bfdwrite(bp);
		    break;
	    case 3:			/* datacheck	*/
		    cc = (*ptr == '\n');
		    bfrelease(bp);
		    break;
	    }
	record += cnt;
	datsize -= cnt;
	offset = 0;
	daddr++;
	}
ret:
    return(cc);
}


#else /* not ISBUFDATA */
/*
 * read a data record into "record"
 */

STATIC int dataread(record, recnum)

register char *record;
long recnum;
{
	register long offset;
	register int datsize;
	register int savebyte, cc;

	datsize = filep->f_datsize + 1;
	offset = longmult((recnum-1), (long)datsize);
	usrmemory();

	if (lseek(filep->f_datfd, offset, 0) != offset)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_DAT;
	    return(-1);
	    }

	savebyte = fubyte(record+datsize-1);
	cc = read(filep->f_datfd, record, datsize);
	if (cc == -1)
	    {
	    iserrno = errno;
	    iserrio = IO_READ+IO_DAT;
	    }
			 			/* deleted record ?	*/
	if (cc == datsize)
	    cc = ((fubyte(record+datsize-1) == '\n') ? 0 : -1);
	else cc = -1;
	subyte(record+datsize-1, savebyte);

	return(cc);
}

/*
 * write a data record from "record"
 */

STATIC int datawrite(record, recnum)

register char *record;
long recnum;
{
	register long offset;
	register int datsize;
	register int savebyte;

	datsize = filep->f_datsize + 1;

	savebyte = fubyte(record+datsize-1);
	subyte(record+datsize-1, '\n');		/* insert record term	*/
	usrmemory();

	offset = longmult((recnum-1), (long)datsize);

	if (lseek(filep->f_datfd, offset, 0) != offset)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_DAT;
	    }
	else if (write(filep->f_datfd, record, datsize) != datsize)
	    {
	    iserrno = errno;
	    iserrio = IO_WRIT+IO_DAT;
	    }

	subyte(record+datsize-1, savebyte);
	return(iserrno ? -1 : 0);
}

/*
 * delete a data record
 */

STATIC int datadelete(recnum)

long recnum;
{
	register long offset;
	register int datsize;
#ifdef ISKERNEL
	register int zerosiz;
	char zeros[128];
#endif /* ISKERNEL */

	tempisfd = -1;			/* mark temprecord destroyed	*/
	datsize = filep->f_datsize + 1;

#ifdef ISKERNEL
	byfill(zeros, 128, 0);
	for (zerosiz = 0; zerosiz < datsize; zerosiz += 128)
	    copyout(zeros, temprecord+zerosiz, 128);
	usrmemory();
#else
	byfill(temprecord, (datsize+1)&~1, 0);	/* clear temp record	*/
#endif /* ISKERNEL */

	offset = longmult((recnum-1), (long)datsize);

	if (lseek(filep->f_datfd, offset, 0) != offset)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_DAT;
	    }
	else if (write(filep->f_datfd, temprecord, datsize) != datsize)
	    {
	    iserrno = errno;
	    iserrio = IO_WRIT+IO_DAT;
	    }
	return(iserrno ? -1 : 0);
}
#ifdef SINIX	/* TP 18.06.1986 */
datacheck(recnum)
    long recnum;
{
    return 0;
}
#endif /* SINIX */

#endif /* not ISBUFDATA */


/*
 * fatal system error
 */

static char fatalmsg[] =  "\r\nFatal system error: ";

#ifdef ISKERNEL
#define wrterr(x)	printf(x)
#endif

#ifdef PCDOS
#define wrterr(x)	write(2, x, stleng(x))
#endif

#ifdef MPM86
#define wrterr(x)	printf(x)
#endif

#ifndef wrterr
#define wrterr(x)	VOID write(2, x, stleng(x))
#endif

STATIC int error(msg)
char *msg;
{
	wrterr(fatalmsg);
	wrterr(msg);
	wrterr("\r\n");
	abort();
}



/* define for number of reclist entries to allocate at a time */
#define RLALLOC	32

/*
 * record list management routines
 *
 *  rlinsert(list, recnum)	insert record number into list
 *
 *	returns:
 *		-1 error - out of memory
 *		0  OK
 */

rlinsert(list, recnum)
    struct reclist **list;
    register long recnum;
{
    register struct reclist *rlp;

    /* if none free allocate another chunk of structures
     */
    if (rlfree == NULL)
	{
	rlfree = (struct reclist *) malloc(RLALLOC*sizeof(struct reclist));
	if (rlfree == NULL) return -1;
	for (rlp = rlfree; rlp < rlfree+RLALLOC-1; rlp++)
	    rlp->rl_next = rlp+1;
	rlp->rl_next = NULL;
	}
    /* pull off first in list
     */
    rlp = rlfree;
    rlfree = rlp->rl_next;
    rlp->rl_next = *list;
    rlp->rl_recnum = recnum;
    *list = rlp;
    return 0;
}


/*
 *  rldelete(list, recnum)	delete record number from list
 *
 *	returns:
 *		1  record number found and deleted
 *		0  record number not found
 */

rldelete(list, recnum)
    struct reclist **list;
    register long recnum;
{
    register struct reclist *rlp, *last;

    last = NULL;
    for (rlp = *list; rlp; rlp = rlp->rl_next)
	{
	if (rlp->rl_recnum == recnum)
	    {
	    if (last == NULL)
		*list = rlp->rl_next;
	    else last->rl_next = rlp->rl_next;
	    rlp->rl_next = rlfree;
	    rlfree = rlp;
	    return 1;
	    }
	last = rlp;
	}
    return 0;
}


/*
 *  rlcheck(list, recnum)	check for record number int list
 *
 *	returns:
 *		1  record number found
 *		0  record number not found
 */

rlcheck(list, recnum)
    struct reclist **list;
    register long recnum;
{
    register struct reclist *rlp;

    for (rlp = *list; rlp; rlp = rlp->rl_next)
	{
	if (rlp->rl_recnum == recnum)
	    return 1;
	}
    return 0;
}


#ifndef ISKERNEL	/* V104 */
/*
 * filename handling routines
 *   converts C-ISAM filenames to operating system names
 */

/*
 * check for legal file name
 *	checks length whole path < PATHSIZE-5   --  ".idx" + NULL
 *	checks length last part <= 10 and > 0   --  14 - 4
 *   returns:
 *	 0 => OK file name
 *      -1 => bad file name (iserrno == EFNAME)
 */

STATIC int ckfilname(name)
char *name;
{
	register char *fp, *last;

#ifdef PCDOS
	for (fp = name; *fp; fp++)
	    if (*fp == '<' || *fp == '>' || *fp == '|')  /* don't chk '.' */
		goto bad;

	last = (name[1] == ':') ? name+2 : name;
	for (fp = last; *fp != 0; fp++)
	    if (*fp == '/' || *fp == '\\') last = fp+1;
	if (*last == 0 || stleng(last) > 8 ||
	    stleng(name) >= PATHSIZE -5)
	    goto bad;
#else
	for (fp = last = name; *fp; )
	    if (*fp++ == '/') last = fp;

	if (stleng(name) >= PATHSIZE-5 || stleng(last) > 10 || *last == 0)
	    goto bad;
#endif /* PCDOS */
	return(0);

bad:	iserrno = EFNAME;
	return(-1);
}


/*
 * make index file name
 */

STATIC int mkidxname(iname, oname)
register char *iname, *oname;
{
	stcopy(iname, oname);
	stcat(IDXNAME, oname);
}

/*
 * make data file name
 */

STATIC int mkdatname(iname, oname)
register char *iname, *oname;
{
	stcopy(iname, oname);
	stcat(DATNAME, oname);
}

#ifdef CREATLOCK
/*
 * make lock file name
 */

STATIC int mklokname(iname, oname)
register char *iname, *oname;
{
	stcopy(iname, oname);
	stcat(LOKNAME, oname);
}

/*
 * make semaphore file name
 *   make filename /isam/lockXXXXYYYY
 *    where XXXX = device number, YYYY = inode number
 */

STATIC int mksemname(filedev, fileino, oname)
dev_t filedev;
ino_t fileino;
register char *oname;
{
	register int i;
	register char *hexp = "0123456789abcdef";
	union {
	    long fileid;
	    struct {
		short devn;
		short inum;
		} id;
	    } un;

	un.id.devn = filedev;
	un.id.inum = fileino;
	stcopy(SEMNAME, oname);
	oname += 8 + stleng(oname);
	*oname-- = 0;
	for (i=0; i<8; i++, oname--)
	    {
	    *oname = hexp[un.fileid & 017];
	    un.fileid >>= 4;
	    }
}
#endif /* CREATLOCK */
#endif /* ISKERNEL  */


#ifdef ISVOPEN
#ifdef ISPCLOSE
isvclose(n)
int n;
{
	ismaxfds = n;
	if ( isopenfds > ismaxfds )
		vclose();
}

#endif /* ISPCLOSE */
vclose()
{
	register struct ifile *fp, *lfp;
	register int i;
#ifdef ISPCLOSE
	int cnt;

	do
	{
	cnt = 0;
#endif /* ISPCLOSE */

	lfp = NULL;
	for (i=0; i<nfiles; i++)
	    {
	    fp = files[i];		/* search for file to close	*/
	    if ( fp != NULL && fp->f_ucount != -1 )
		{
#ifdef ISPCLOSE
		cnt++;
		if (fp->f_ucount == 0 ) 
		    {
		    if (lfp == NULL ||  fp->f_vrefcnt < lfp->f_vrefcnt 
			    ||  lfp->f_ucount != 0 )
			lfp = fp;
		    }
	        else 
#endif /* ISPCLOSE */
	        if ( fp->f_datfd >= 0 )
		    {
#ifdef CREATLOCK
		    if (fp->f_lokfd >= 0 )   /* lock file must also be open */
#else
#ifndef SHMLOCK
		    if ((fp->f_flags & FLOCKS) == 0)
#endif  /* SHMLOCK */
#endif /* CREATLOCK */
			if (lfp == NULL ||
				( fp->f_vrefcnt < lfp->f_vrefcnt
					&& lfp->f_ucount > 0 ) )
			    lfp = fp;
		    }
		}
	    }
	if ( lfp )
#ifdef ISPCLOSE
	    if ( lfp->f_ucount == 0 )
		freefile(lfp);
	    else
#endif /* ISPCLOSE */
		{
#ifndef SHMBUFF
		bfflush(lfp);
#endif  /* SHMBUFF */
#ifdef  SHMLOCK
		close(lfp->f_idxfd);
		lfp->f_idxfd = -1;
#endif  /* SHMLOCK */
		close(lfp->f_datfd);
		lfp->f_datfd = -1;
		if (filep->f_flags & FAUDOPEN)
		    {
#ifndef VMS
	   	    close(lfp->f_audfd);
#else /* VMS */
                    rmsclose(lfp->f_audfd);
#endif /* VMS */
		    lfp->f_audfd = -1;
		    filep->f_flags &= ~FAUDOPEN;
		    }
#ifdef CREATLOCK
		close(lfp->f_lokfd);
		lfp->f_lokfd = -1;
#endif
		}
#ifdef ISPCLOSE
	} while ( cnt >= ismaxfds && lfp );
	isopenfds = cnt;
#endif /* ISPCLOSE */
}
#endif /* ISVOPEN */

