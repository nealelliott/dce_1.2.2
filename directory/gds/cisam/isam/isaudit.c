/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isaudit.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:08  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:47  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:00  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:57:27  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:57:43  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:04:16  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:49:49  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:19  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:36:22  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:40:06  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isaudit.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:06 $";
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
 *  Title:	isaudit.c
 *  Description:
 *		Audit trail management routine for C-ISAM.
 *
 ************************************************************************
 */


/*
 * Audit trail management routines
 *
 *  Relational Database Systems, Inc.
 *  Roy Harrington	January 5, 1982
 */

#include <isdec.c>

#ifndef NOAUDIT		/* no audit code at all, if NOAUDIT defined */

isaudit(isfd, fname, mode)
int isfd;
char *fname;
int mode;
{
	register struct buff *bp;
	register long audnode;

#ifdef VMS
	int afd;
#endif /* VMS */

	if (isenter(isfd, 		/* open mode depends on "mode"	*/
	    (mode == AUDGETNAME || mode == AUDINFO) ? ISINPUT : ISINOUT)
		== -1) return(-1);

	switch(mode)
	    {
	    case AUDSETNAME:
		if (ld1(dictp->d_audit) & 0x80)		/* must be stopped */
		    goto bad;
		if (stleng(fname) >= PATHSIZE)
		    goto bad;
		audnode = ld4(dictp->d_audit);
		if (fname[0] == 0)
		    {
		    if (audnode != 0L)
			{
			freerec(audnode & 0x7FFFFFFF, INDXREC);
			st4(0L, dictp->d_audit);
			dictmodify();
			}
		    break;
		    }
#ifndef VMS
#ifndef ISKERNEL
		if (makefile(fname, DATMODE) && iserrno != EEXIST)
		    goto bad;
#endif  /* ISKERNEL */
#else /* VMS */
		if((afd = rmscreat(fname, 0600)) < 0)
		     goto bad;
		rmsclose(afd);
#endif /* VMS */
		iserrno = 0;
		if (audnode == 0L)
		    {
		    audnode = allocrec(INDXREC);
		    bp = bfgetclr(audnode);
		    st4(audnode, dictp->d_audit);
		    dictmodify();
		    }
		else bp = bfread(audnode & 0x7FFFFFFF);
		if (bp == NULL) break;
#ifndef VMS
		stcopy(fname, bp->bf_un.bf_audt->an_filename);
#else /* VMS */
       stcopy(fname,((struct audt *)GSADDRESS(bp->bf_un.bf_audt))->an_filename);
#endif /* VMS */
		bp->bf_used = 5 + stleng(fname);
		bfdwrite(bp);
		break;

	    case AUDGETNAME:
		if ((audnode = ld4(dictp->d_audit)) == 0L)
		    goto bad;
		bp = bfread(audnode & 0x7FFFFFFF);
		if (bp == NULL) break;
#ifndef VMS
		stcopy(bp->bf_un.bf_audt->an_filename, fname);
#else /* VMS */
       stcopy(((struct audt *)GSADDRESS(bp->bf_un.bf_audt))->an_filename,fname);
#endif /* VMS */
		bfrelease(bp);
		break;

	    case AUDSTART:
		if (ld4(dictp->d_audit) == 0L)
		    goto bad;
		dictp->d_audit[0] |= 0x80;
		dictmodify();
		break;

	    case AUDSTOP:
		if ((ld1(dictp->d_audit) & 0x80) &&
			(filep->f_flags & FAUDOPEN))
		    {
#ifndef VMS
		    VOID close(filep->f_audfd);
#else /* VMS */
		    VOID rmsclose(filep->f_audfd);
#endif /* VMS */
		    filep->f_flags &= ~FAUDOPEN;
		    }
		dictp->d_audit[0] &= ~0x80;
		dictmodify();
		break;

	    case AUDINFO:
		*fname = ((ld1(dictp->d_audit) & 0x80) != 0);
		break;
	    }

	return(isexit());
bad:
	if (iserrno == 0) iserrno = EBADARG;
	return(isexit());
}



/*
 * open audit trail
 */

STATIC int audopen()
{
	register long audnode;
	register struct buff *bp;
	register struct audt *audp;

	if (filep->f_flags & FAUDOPEN)
	    return(0);			/* audit trail already opened	*/
	if ((ld1(dictp->d_audit) & 0x80) == 0)
	    return(0);			/* no audit trail defined	*/

	audnode = ld4(dictp->d_audit);
	bp = bfread(audnode & 0x7FFFFFFF);
	if (bp == NULL) return(-1);
#ifndef VMS
	audp = bp->bf_un.bf_audt;
#else /* VMS */
	audp = (struct audt *) GSADDRESS(bp->bf_un.bf_audt);
#endif /* VMS */
	sysmemory();

#ifndef VMS
	if ((filep->f_audfd = open(audp->an_filename, WMODE)) == OPENERR
#else /* VMS */
	if ((filep->f_audfd = rmsopen(audp->an_filename, RWMODE)) == OPENERR
#endif /* VMS */
		&& errno == EMFILE)
	    {
	    vclose();
#ifndef VMS
	    filep->f_audfd = open(audp->an_filename, WMODE);
#else /* VMS */
	    filep->f_audfd = rmsopen(audp->an_filename, RWMODE);
#endif /* VMS */
	    }

	if (filep->f_audfd == OPENERR)
	    {
	    iserrno = errno;
	    iserrio = IO_OPEN+IO_AUD;
	    }
	else filep->f_flags |= FAUDOPEN;

	bfrelease(bp);
	return(iserrno ? -1 : 0);
}


/*
 * output an audit trail record
 */

STATIC int audoutput(type, recnum, record)

char type;
long recnum;
char *record;

{
	struct audhead head;
	long autime;
#ifndef ISKERNEL
	long time();
#endif
	register int datsize;
	register int savebyte;

	if ( ! (ld1(dictp->d_audit) & 0x80))
	    return;				/* audit trail off	*/
	if ( ! (filep->f_flags & FAUDOPEN))
	    return;
	
#ifndef VMS
	if (lseek(filep->f_audfd, 0L, 2) == -1L)/* seek to end of file	*/
#else /* VMS */
	if (rmslseek(filep->f_audfd, 0L, 2) == -1L)
#endif /* VMS */
	    {
	    iserrno = errno;
	    iserrio = IO_SEEK+IO_AUD;
	    return;
	    }

#ifndef ISKERNEL
	autime = time(&autime);		/* else function call		*/
#else
	autime = time;			/* global variable in kernel	*/
#endif

	st1(type, head.au_type);
	st1(type, head.au_type+1);
	st4(autime, head.au_time);
	st2(openp->o_procid, head.au_procid);
	st2(openp->o_userid, head.au_userid);
	st4(recnum, head.au_recnum);

	datsize = filep->f_datsize;
	savebyte = fubyte(record+datsize);
	subyte(record+datsize, '\n');

	sysmemory();			/* from system memory	*/
#ifndef VMS
	if (write(filep->f_audfd, CASTCP &head, AUDHEADSIZE) != AUDHEADSIZE)
#else /* VMS */
	if (rmswrite(filep->f_audfd, CASTCP &head, AUDHEADSIZE) != AUDHEADSIZE)
#endif /* VMS */
	    goto bad;

	usrmemory();			/* from user memory	*/
#ifndef VMS
	if (write(filep->f_audfd, record, datsize+1) != datsize+1)
#else /* VMS */
	if (rmswrite(filep->f_audfd, record, datsize+1) != datsize+1)
#endif /* VMS */
	    {
bad:	    iserrno = errno;
	    iserrio = IO_WRIT+IO_AUD;
	    }

	subyte(record+datsize, savebyte);
}

#endif		/* NOAUDIT */
