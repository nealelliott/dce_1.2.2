/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isbuf.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:10  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:49  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:57:47  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:59:33  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:04:37  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:50:03  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:27  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:36:41  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:40:17  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isbuf.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:09 $";
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
 *  Title:	isbuf.c
 *  Description:
 *		Buffer management routines which operate on the
 *		index file record buffers.
 *
 ************************************************************************
 */


/*
 * buffer management routines
 *  operates on index file record buffers
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 *
 */

#include <isdec.c>

#ifndef SHMBUFF
static int initflag = 0;			/* initialization flag	*/

#ifdef ISXBATCH
int isiocnt;
#endif /* ISXBATCH */
 
#ifdef ISBUFHASH
short nhbufs;
struct hbuf *hbufs;
#define BUFHASH(f,b) ((struct buff *)&hbufs[(unsigned)(f->f_inum+b)%nhbufs])
#endif /* ISBUFHASH */

/*
 * buffer initialization
 *  must be called once at start up
 */

STATIC int bfinit()
{
	register struct buff *bp;
	register struct hbuf *hbp;
	register char *cp;
	register int i;
	extern char *getenv();

#ifdef SINIX	/* TP 10.11.87 */
#ifdef ISXBATCH
	extern int batchmax;
	if ( (cp = getenv("ISAMBATCH")) && (i = atoi(cp) ) >= 1  && i <= 10)
		batchmax = i;
#endif /* ISXBATCH */
#endif /* SINIX */

	avbuffs.bf_forw = avbuffs.bf_back = &avbuffs; /* set up avail list */

#ifdef  ISALLOC
	if ( (cp = getenv("ISAMBUFS")) && (i = atoi(cp) ) >= 4 )
		nbuffs = i;
#ifdef ISBUFHASH
	nhbufs = nbuffs/3;
	hbufs = (struct hbuf *) malloc(CASTUN nhbufs*sizeof(struct hbuf));
#endif /* ISBUFHASH */
	buffs = (struct buff *) malloc(CASTUN nbuffs*sizeof(struct buff));
	buffers = malloc(CASTUN nbuffs*NODESIZE);
	if (buffs == NULL || buffers == NULL)
	    error("cannot allocate buffers");
#endif  /* ISALLOC */

#ifdef ISBUFHASH
	hbp = &hbufs[0];
	for (i=0; i < nhbufs; i++, hbp++)
	    {
	    hbp->bh_forw = hbp->bh_back = (struct buff *) hbp;
	    }
#endif /* ISBUFHASH */

	bp = &buffs[0];
	cp = &buffers[0];

	for (i=0; i < nbuffs; i++, bp++)
	    {					/* set up each buffer	*/
	    bp->bf_flags = BFBUSY;
	    bp->bf_filep = NULL;
#ifdef ISBUFHASH
	    bp->bh_forw = bp->bh_back = bp;
#endif /* ISBUFHASH */
	    bp->bf_un.bf_addr = cp;
	    cp += NODESIZE;
	    bfrelease(bp);
	    }
	initflag = 1;
}

/*
 * buffer read
 * access an index node
 */

STATIC struct buff *bfread(daddr)
long daddr;
{
	register struct buff *bp;
	register char *addr;
	int fd, size; register int count;
	long offset;

#ifndef ISBUFDATA
	if (daddr <= 0L)
	    {
	    iserrno = EBADFILE;			/* bad file format	*/
	    return(NULL);
	    }
#endif  /* ISBUFDATA */

	bp = bfgetblk(daddr);
	if ( ! (bp->bf_flags & BFDONE))
	    {
	    bfmapaddr(bp, &fd, &offset, &size);
	    if (lseek(fd, offset, 0) != offset)
		{
		iserrio = IO_SEEK+IO_IDX;
		goto bad;
		}
	    addr = bp->bf_un.bf_addr;
#ifdef ISXBATCH
	    isiocnt++;
#endif /* ISXBATCH */
	    count = read(fd, addr, size);
	    if (count != size)
		{
		if (fd == filep->f_idxfd || count < 0)
		    {				/* idx must be full blk	*/
		    iserrio = IO_READ+IO_IDX;
		    goto bad;
		    }
		else				/* clear rest of data	*/
		    byfill(addr+count, size-count, 0);
		}
	    bp->bf_used  = ld2(addr) & 0x7FFF;	/* load common data	*/
	    bp->bf_type  = ld1(addr+filep->f_idxsize-2);
	    bp->bf_level = ld1(addr+filep->f_idxsize-1) & 0x7F;
	    bp->bf_flags |= BFDONE;
	    }
	return(bp);

bad:	iserrno = errno;
	bfrelease(bp);
	return(NULL);
}

/*
 * get clear buffer
 */

STATIC struct buff *bfgetclr(daddr)
long daddr;
{
	register struct buff *bp;
	register int *intp, cnt;

	bp = bfgetblk(daddr);
	intp = bp->bf_un.bf_words;
	cnt = NODESIZE / sizeof(int);
	do
	    *intp++ = 0;
	while (--cnt);
	bp->bf_flags |= (BFMODF|BFDONE);
	return(bp);
}

/*
 * buffer get block
 *  allocate a buffer to hold block
 */

STATIC struct buff *bfgetblk(daddr)
long daddr;
{
	register struct buff *bp, *hbp;
#ifndef ISBUFHASH
	register int i;		/* SINIX TP 11.11.86 */
#endif /* ISBUFHASH */

	if (initflag == 0) bfinit();		/* initialize buffers	*/

#ifdef ISBUFHASH
	hbp = BUFHASH(filep, daddr);
	for (bp = hbp->bh_forw; bp != hbp; bp = bp->bh_forw)
	    {
	    if (bp->bf_daddr == daddr && bp->bf_filep == filep)
		goto found;			/* match baddr & filep	*/
	    }
#else  /* ISBUFHASH */
	bp = &buffs[0];
	for (i=0; i < nbuffs; i++, bp++)
	    {
	    if (bp->bf_daddr == daddr && bp->bf_filep == filep)
		goto found;			/* match baddr & filep	*/
	    }
#endif /* ISBUFHASH */

	bp = avbuffs.bf_back;			/* get available buffer	*/
	if (bp == &avbuffs)
	    error("no more buffers");
	if (bp->bf_flags & BFMODF)		/* write it if modified */
	    bfwrite(bp);
	bp->bf_daddr = daddr;			/* give it new identity	*/
	bp->bf_filep = filep;
	bp->bf_flags = 0;

#ifdef ISBUFHASH
	bp->bh_back->bh_forw = bp->bh_forw;	/* unlink from hash list */
	bp->bh_forw->bh_back = bp->bh_back;
	hbp = BUFHASH(filep, daddr);
	bp->bh_forw = hbp->bh_forw;		/* link to hash list	*/
	bp->bh_forw->bh_back = bp;
	hbp->bh_forw = bp;
	bp->bh_back = hbp;
#endif /* ISBUFHASH */

found:
	if (bp->bf_flags & BFBUSY)
	    error("buffer not free");
	bp->bf_flags |= BFBUSY;
	bp->bf_back->bf_forw = bp->bf_forw;	/* unlink from avail list */
	bp->bf_forw->bf_back = bp->bf_back;
	return(bp);
}


/*
 * release buffer to buffer pool
 */

STATIC int bfrelease(bp)
register struct buff *bp;
{
	register char *addr;

	if (bp == NULL) return;
	if ( ! (bp->bf_flags & BFBUSY))
	    error("buffer not busy");
	bp->bf_flags &= ~BFBUSY;
	if ((bp->bf_flags & BFMODF) && (bp->bf_daddr & DATABUF) == 0)
	    {			/* if modified	*/
	    addr = bp->bf_un.bf_addr;
	    st2(bp->bf_used, addr);
	    st1(bp->bf_type, addr+filep->f_idxsize-2);
	    st1(bp->bf_level, addr+filep->f_idxsize-1);
	    }

	if (bp->bf_flags & BFMODF)
	    filep->f_flags |= FMODF;

	bp->bf_forw = avbuffs.bf_forw;		/* link to avail list	*/
	bp->bf_forw->bf_back = bp;
	avbuffs.bf_forw = bp;
	bp->bf_back = &avbuffs;
}

/*
 * delayed write of buffer
 *  just mark buffer for writing
 */

STATIC int bfdwrite(bp)
register struct buff *bp;
{
	bp->bf_flags |= BFMODF|BFDONE;
	bfrelease(bp);
}

/*
 * write out buffer
 */

STATIC int bfwrite(bp)
register struct buff *bp;
{
	int fd, size;
	long offset;
	register int cc = 0;

	bp->bf_flags |= BFMODF|BFDONE;
	if (bp->bf_flags & BFBUSY)
	    bfrelease(bp);

#ifdef ISXBATCH
	isiocnt++;
#endif /* ISXBATCH */
	bfmapaddr(bp, &fd, &offset, &size);
	if (lseek(fd, offset, 0) != offset)
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_IDX;
	    cc = -1;
	    }
	else if (write(fd, bp->bf_un.bf_addr, size) != size)
	    {
	    iserrno = errno;
	    iserrio = IO_WRIT+IO_IDX;
	    cc = -1;
	    }

	bp->bf_flags &= ~BFMODF;

	return cc;
}


/*
 * map buffer address into file desc and byte offset
 */

bfmapaddr(bp, fd, offset, size)
    struct buff *bp;
    int *fd;
    long *offset;
    int *size;
{
#ifdef ISBUFDATA
    if (bp->bf_daddr & DATABUF)			/* data record	*/
	{
	*size = NODESIZE;
	*offset = (bp->bf_daddr & ~DATABUF) * NODESIZE;
	*fd = bp->bf_filep->f_datfd;
	}
    else					/* index record	*/
#endif /* ISBUFDATA */
	{
	*size = (bp->bf_filep->f_idxsize+1);
	*offset = (bp->bf_daddr-1) * (*size);
	*fd = bp->bf_filep->f_idxfd;
	}
}

/*
 * flush out any modified buffer
 */

STATIC int bfflush(fp)
struct ifile *fp;
{
	register struct buff *bp;
	register int i;

	if (initflag == 0) return;
	bp = &buffs[0];
	for (i=0; i < nbuffs; i++, bp++)
	    {
	    if ( ! (bp->bf_flags & BFBUSY) && bp->bf_filep == fp &&
		    (bp->bf_flags & BFMODF))	/* write out any	*/
		bfwrite(bp);			/*  modified buffers	*/
	    }
}

/*
 * ignor buffers already containing data
 */

STATIC int bfignor(fp)
struct ifile *fp;
{
	register struct buff *bp;
	register int i;

	if (initflag == 0) return;
	bp = &buffs[0];
	for (i=0; i < nbuffs; i++, bp++)
	    {
	    if ( ! (bp->bf_flags & BFBUSY) && bp->bf_filep == fp)
		{				/* match filep ?	*/
		if (bp->bf_flags & BFMODF)
		    error("ignoring modified buffer");
		bp->bf_filep = NULL;		/* yes, erase fileid	*/
#ifdef ISBUFHASH
		bp->bh_back->bh_forw = bp->bh_forw;
		bp->bh_forw->bh_back = bp->bh_back;
		bp->bh_forw = bp->bh_back = bp;
#endif /* ISBUFHASH */
		}
	    }
}

#endif /* SHMBUFF */
