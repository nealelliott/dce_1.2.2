/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/*
 * HISTORY
 * $Log: efs_hpux_subr.c,v $
 * Revision 1.1.94.1  1996/10/02  17:42:54  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:18  damon]
 *
 * Revision 1.1.89.3  1994/07/13  22:20:24  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:51  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  20:13:01  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:57:31  mbs]
 * 
 * 	fix compilation error
 * 	[1994/03/22  21:25:48  rsarbo]
 * 
 * 	integrating unintegrated fix from 103a.  Here are original
 * 	comments from Bruce Leverett:
 * 	Many changes, for hole detection and a cleaner fit into HP/UX:
 * 	- efs_mapdbd now uses efsdbd_t, not dbd_t; likewise for members of the
 * 	  efsargs_t structure.
 * 	- efs_pagein calls efs_mapdbd rather than doing it by hand.
 * 	- efs_mapdbd detects holes (by calling epia_Map) and fills in the dbd_type
 * 	  to DBD_HOLE accordingly.
 * 	- when efs_pagein calls vn_rdwr to read the data, if there is a hole in the
 * 	  file and write access is requested it calls vn_rdwr again to cause
 * 	  allocation.  This can fail due to quota or space shortage, and the failure
 * 	  triggers a SIGBUS signal to the faulting process.
 * 	- when efs_pagein uses an existing page to satisfy a fault, if there is a
 * 	  hole in the file and write access is requested it calls vfs_alloc_hole to
 * 	  cause allocation.  This can fail due to quota or space shortage, and the
 * 	  failure triggers a SIGBUS signal to the faulting process.
 * 	More coherent strategy for setting dbd_blkno and for calling vfs_mark_dbds.
 * 	[1994/03/01  17:02:52  rsarbo]
 * 
 * Revision 1.1.89.2  1994/06/09  14:04:42  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:39:24  annie]
 * 
 * Revision 1.1.89.2  1994/06/09  14:04:42  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:39:24  annie]
 * 
 * Revision 1.1.89.1  1994/02/04  20:16:07  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:48  devsrc]
 * 
 * Revision 1.1.87.1  1993/12/07  17:21:59  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:04:42  jaffe]
 * 
 * Revision 1.1.2.5  1993/01/21  18:25:31  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  16:47:07  zeliff]
 * 
 * Revision 1.1.2.4  1992/11/24  16:58:20  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  17:52:37  bolinger]
 * 
 * Revision 1.1.2.3  1992/09/25  16:28:44  jaffe
 * 	Sync with Transarc by cleaned duplicate HISTORY and Log info
 * 	[1992/09/24  17:52:00  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  20:50:06  jaffe
 * 	Transarc delta: mason-add-hp800-osi-routines 1.15
 * 	  Selected comments:
 * 	    This delta is misnamed - these are really porting fixes.
 * 	    Need to checkpoint to fix a different bug.
 * 	    Checkpoint the first system where test_anode works.  Only one change is not
 * 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
 * 	    is -lBSD.  This should be fixed more generally.
 * 	    More HP-UX changes.
 * 	    More HP-UX related changes.
 * 	    Fix compile problem on the RIOS.
 * 	    Fix more compilation problems.
 * 	    Checkpoint.
 * 	    Checpoint.
 * 	    In this snap we turned all malloc() calls into osi_Alloc calls.
 * 	    Also turned free calls into osi_Free.
 * 	    HP-UX EOF detection fix.
 * 	    Another checkpoint.
 * 	    This is a checkpoint of the first working HP-UX Episode system.
 * 	    Needed to add the vnops routines specific to HP-UX.
 * 	    Misc. routines required on the HP (getmount)
 * 	    Fix compilation problems.
 * 	[1992/08/30  03:06:57  jaffe]
 * 
 * Revision 1.1.1.2  1992/08/30  03:06:57  jaffe
 * 	Transarc delta: mason-add-hp800-osi-routines 1.15
 * 	  Selected comments:
 * 	    This delta is misnamed - these are really porting fixes.
 * 	    Need to checkpoint to fix a different bug.
 * 	    Checkpoint the first system where test_anode works.  Only one change is not
 * 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
 * 	    is -lBSD.  This should be fixed more generally.
 * 	    More HP-UX changes.
 * 	    More HP-UX related changes.
 * 	    Fix compile problem on the RIOS.
 * 	    Fix more compilation problems.
 * 	    Checkpoint.
 * 	    Checpoint.
 * 	    In this snap we turned all malloc() calls into osi_Alloc calls.
 * 	    Also turned free calls into osi_Free.
 * 	    HP-UX EOF detection fix.
 * 	    Another checkpoint.
 * 	    This is a checkpoint of the first working HP-UX Episode system.
 * 	    Needed to add the vnops routines specific to HP-UX.
 * 	    Misc. routines required on the HP (getmount)
 * 	    Fix compilation problems.
 * 
 * 	$TALog: efs_hpux_subr.c,v $
 * 	Revision 1.12  1994/11/01  21:33:06  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.9 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.9  1993/11/18  18:28:20  bwl
 * 	Fix truncation, by adding an HP version of vnvm_truncate.
 * 
 * 	Add vnvm_truncate.
 * 	[from r1.8 by delta bwl-o-ot9098-hp-episode-pageout-panic, r1.2]
 * 
 * Revision 1.8  1993/11/16  21:58:48  bwl
 * 	Various changes to get mmap to work for locally mounted Episode, in HP/UX.
 * 
 * 	Create efs_mapdbd, efs_vfdcheck, and efs_pageout, based on the corresponding
 * 	vfs_ functions from the HP/UX kernel;
 * 	efs_pagein must catch references to pages beyond EOF and return SIGBUS;
 * 	get rid of some superfluous definitions/declarations left over from previous
 * 	work.
 * 	[from r1.6 by delta bwl-o-ot9098-hp-episode-pageout-panic, r1.1]
 * 
 * Revision 1.6  1993/10/21  21:17:08  bwl
 * 	Port vnvm_PrepareUserBuffer and vnvm_ReleaseUserBuffer to HP.
 * 
 * 	Add HP versions of two functions.
 * 	[from r1.5 by delta bwl-o-hp-prepare-user-buffer, r1.1]
 * 
 * Revision 1.5  1993/10/12  16:19:51  bwl
 * 	Various fixes for the HP port, picked up from HP via OSF.
 * 
 * 	Use mount pathname saved in efsMOUNT data structure in efs_getmount.
 * 	[from r1.4 by delta bwl-o-ot8885-hp-various-port-fixes, r1.1]
 * 
 * Revision 1.4  1993/09/08  15:34:07  berman
 * 	Changes corresponding to HP's second drop, with Episode.
 * 	Milestone:  Episode builds cleanly, test_vnodeops works on all_tests.
 * 
 * 	Clean up #include's;
 * 	add efs_hpstrategy, efs_hpustrategy, efs_pagein for HP VM support.
 * 	[from r1.3 by delta bwl-o-db3961-port-103-to-HP, r1.16]
 * 
 * Revision 1.3  1993/01/29  14:54:28  jaffe
 * 	Pick up files from the OSF up to the 2.4 submission.
 * 	[from r1.2 by delta osf-revdrop-01-27-93, r1.1]
 * 
 * 	Revision 1.1  1992/06/23  15:19:54  mason
 * 	Needed to add the vnops routines specific to HP-UX.
 * 
 * 	Misc. routines required on the HP (getmount)
 * 	[added by delta mason-add-hp800-osi-routines, revision 1.14]
 * 
 * $EndLog$
 */

/*
 * (C) Copyright 1992 Transarc Corporation - All Rights Reserved.
 */

/*
 * HP-UX specific code for Episode.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>			/* osi_Alloc */
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/debug.h>		/* dmprintf (for EFS_TRACE_VNOPS) */
#include <dcedfs/lock.h>		/* struct lock_data */
#include <dcedfs/queue.h>		/* struct squeue */
#include <dcedfs/xvfs_vnode.h>		/* struct xvfs_vfsops */

#include <dcedfs/episode/async.h>	/* struct asevent */
#include <dcedfs/episode/volume.h>	/* epi_volume_t, epiv_XXX functions */
#include <dcedfs/episode/anode.h>	/* epi_anode_t, epia_XXX functions */
#include <efs_evnode.h>			/* struct evnode */
#include <efsmount.h>			/* struct efsMOUNT */


RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/HP800/efs_hpux_subr.c,v 1.1.94.1 1996/10/02 17:42:54 damon Exp $")

#ifndef	_KERNEL
extern struct user u;
#endif

extern int epi_Initted;
extern int efs_vfs_mtype;

#ifdef GETMOUNT

efs_getmount(vfsp, fsmntdir, mdp)
    struct vfs *vfsp;
    caddr_t fsmntdir;
    struct mount_data *mdp;
{
    struct efsMOUNT *mp;		/* Point to mounted Volume */
    epi_volume_t volHandle;		/* Volume Handle */
    struct epiv_status volStatus;	/* status of volume */
    long code;
    int len;

    if (!epi_Initted)
	return EINVAL;
    
    if (efs_vfs_mtype < 0) 
    {
	panic("efs_getmount: efs_vfs_mtype not initialized yet");
    }
    mp = VFSTOEFS(vfsp);
    
    /* only fields specific to Episode are set */
    mdp->md_fstype = vfsp->vfs_mtype;
    mdp->md_fsopts = 0;
    mdp->md_msite = 0;			/* episode not site specific */
    mdp->md_rdev = mdp->md_dev =
	makedev(mp->deviceP->majdev, mp->deviceP->mindev);

    return (copyoutstr(mp->mntpath, fsmntdir, mp->mntpathlen, &len));
}

#endif	/* GETMOUNT */

/* 
 * Basic strategy routine, assuming stuff mapped in. 
 * 
 * XXX	This is an expedient hack.  It is synchronous, and it goes through
 *	the buffer system (rather than directly to disk), so its performance
 *	leaves much to be desired.
 */
efs_hpustrategy(bufp)
    register struct buf *bufp; 
{
    register long code;
    struct uio tuio;
    struct iovec tiovec[1];

    register long len = bufp->b_bcount;

    tuio.osi_uio_iov = tiovec;
    tuio.osi_uio_iovcnt = 1;
    tuio.osi_uio_offset = DEV_BSIZE * bufp->b_blkno;
    tuio.osi_uio_seg = OSI_UIOSYS;
    tuio.osi_uio_resid = len;
    tiovec[0].iov_base = bufp->b_un.b_addr;
    tiovec[0].iov_len = len;
    
    /* 
     * read/write b_bcount bytes into kernel address b_un.b_addr starting
     * at byte DEV_BSIZE * b_blkno.  Bzero anything we can't read,
     * and finally call iodone(bufp).  File is in bufp->b_vp.  Credentials
     * are from u area 
     * !!! Are user's credentials valid here?  probably, but this sure seems 
     * like the wrong thing to do. 
     */
    
    code = efs_rdwr (bufp->b_vp, &tuio, 
	((bufp->b_flags & B_READ) == B_READ) ? UIO_READ:UIO_WRITE, 
	0, osi_getucred());
    
    if (code == 0 
	  && ((bufp->b_flags & B_READ) == B_READ) 
	  && (tuio.osi_uio_resid > 0) )
	bzero(bufp->b_un.b_addr+bufp->b_bcount-tuio.osi_uio_resid, 
		      tuio.osi_uio_resid);

    return code;
}

/* 
 * Strategy call
 * Copied from cm_strategy and trimmed down for (my) sanity.
 * (Bill Sommerfeld)
 */
int efs_hpstrategy (bufp)
    register struct buf *bufp; 
{
    return osi_MapStrategy(efs_hpustrategy, bufp);
}

#ifdef _KERNEL
/*
 * a subset of the following include files are required for virtual memory 
 * operations.
 */
#include <sys/debug.h>
#include <sys/vmmac.h>
#include <sys/fs.h>
#include <sys/dbd.h>
#include <sys/vfd.h>
#include <sys/region.h>
#include <sys/pregion.h>
#include <sys/vmmeter.h>
#include <sys/sysinfo.h>
#include <sys/pfdat.h>
#include <sys/tuneable.h>
#include <sys/sar.h>

/*
 * max number of pages that efs_pagein may bring in at a time
 */
#define EFS_MAXPGIN	1

/*
 *  Episode variant of the DBD format.  Simply the DBD type, and a block number
 *  for reading the data off the devvp for the vnode.
 */
typedef struct efsdbd {
	uint dbd_type : 4;
	uint dbd_blkno : 28;
} efsdbd_t;

/* Forward decl */
efsdbd_t efs_mapdbd();

/*
 * Efs pagein routine.
 * 
 * Cribbed from cdfs_pagein with help from vfs_pagein.
 * 
 * Because we are reading from Episode buffers rather than straight from
 * disk, we don't get a disk block number by calling efs_bmap, but just
 * use the logical block number.
 */
int
efs_pagein(prp, wrt, space, vaddr, ret_startindex)
	preg_t *prp;
	int wrt;
	space_t space;
	caddr_t vaddr;
	int *ret_startindex;
{
	int errorfl;
	int resid;
	int mmf;
	int test_alloc = 0;
	off_t	offset;
	dbd_t *dbd2;
	vfd_t *vfd2;
	register efsdbd_t *dbd;
	register vfd_t *vfd;
	int pgindx;
	int bsize;
	reg_t *rp = prp->p_reg;
	struct vnode *filevp;
	pfd_t *pfd;
	int count;
	caddr_t nvaddr;
	space_t nspace;
	struct evnode *evp;
	int code;
	long filelen;
	
	vm_sema_state;		/* semaphore save state */
	void memunreserve();
	vmemp_lockx();		/* lock down VM empire */

	/*	
	 * Find vfd and dbd as mapped by region
	 */
	pgindx = *ret_startindex;

	FINDENTRY(rp, pgindx, &vfd2, &dbd2);
	vfd = vfd2;
	dbd = (efsdbd_t *)dbd2;

	/* Memory mapped file? */
	mmf = (rp->r_fstore == rp->r_bstore);

	/*
	 * Which vnode did I fault for?
	 *
	 * Note:  Code in hdl_pfault (S800) depends on the algorithm
	 * used here for hashing pages in the page cache.  This
	 * should probably be a future VOP function so that the
	 * details aren't exported to that routine.
	 */
	if (dbd->dbd_type == DBD_FSTORE)
	    filevp = rp->r_fstore;
	else
	    filevp = rp->r_bstore;

	if (filevp->v_vfsp->vfs_mtype != efs_vfs_mtype) 
	{
	    panic("efs_pagein: not episode");
	}

	evp = VTOEV(filevp);
	if (evp->vd_flags & VD_PHANTOM) 
	    if (code = vnm_RestartUse(evp)) 
	    {
		printf("efs_pagein: dephantom via vnm_RestartUse failed, code %d\n", code);
		/* sayonara.. */
		goto backout;
	    }
	bsize = epia_BlockSize(EVTOA(evp));

	/*
	 * If reference is to a page beyond the end of the file,
	 * send the process a SIGBUS.
	 */
	offset = vnodindx(rp, pgindx);
	efs_getlength (filevp, &filelen, 0);
	if (offset >= filelen) {
	    vmemp_returnx(-SIGBUS);
	}

	/*
	 * Make sure that the dbd contains a valid block
	 * number.
	 */
	if(dbd->dbd_type == DBD_HOLE || dbd->dbd_blkno == DBD_DINVAL) {
		int flag = ((wrt && mmf) ? B_WRITE : B_READ);
		*dbd = efs_mapdbd (pgindx, rp, filevp, flag);
		if (dbd->dbd_type == DBD_HOLE) {
			if (flag == B_WRITE)
				test_alloc = 1;
			if (!mmf) {
				dbd->dbd_type = DBD_FSTORE;
				dbd->dbd_blkno = DBD_DINVAL;
			}
		}
               /*
                * Now we fall through.  We don't have any special code
                * to create a new page and zero it, because the general
                * case code will create the new page and vn_rdwr will zero it.
                * If we use something lower-level than vn_rdwr there,
                * we may have to fix this.
                *
                * Note that we are also relying on the general case code
                * to allocate storage for holes for us, i.e. to look at
                * test_alloc.
                */
	}

	/*	
	 * If the page we want is in memory already, take it
	 */
	if (pfd = pageincache(filevp, dbd->dbd_blkno)) {
		VASSERT((pfd->pf_data == dbd->dbd_blkno) &&
			     (pfd->pf_devvp == filevp));
		vfd->pgm.pg_pfn = pfd->pf_pfn;
		vfd->pgm.pg_v = 1;
		rp->r_nvalid++;

               /*
                * There might be a hole in the file.  If they want write
                * access to the page, force allocation for the hole.
                */
               if (wrt && dbd->dbd_type == DBD_HOLE) {
                   /*
                    * Protect the page from the user while we (possibly)
                    * perform the I/O.
                    */
                   hdl_user_protect(prp, space, vaddr, 1, &nspace, &nvaddr, 0);

                   /*
                    * Now try to allocate disk space for this page.
                    */
                   errorfl = vfs_alloc_hole(rp, nspace, nvaddr, filevp, 
				dbd->dbd_blkno);

                   /*
                    * The I/O is finished, unprotect the page now.
                    */
                   hdl_user_unprotect(nspace, nvaddr, 1, 0);

                   /*
                    * If some sort of I/O error occurred we generate a
                    * SIGBUS for the process that caused the write,
                    * undo our page locks, etc., and return.
                    */
                   if (errorfl || rp->r_zomb) {
                      if (PAGEINHASH(pfd))
                      pageremove(pfd);
                      pfdatunlock(pfd);
                      vmemp_returnx(errorfl ? -SIGBUS : 0);
                   }
                   vfs_mark_dbds(rp, pgindx, bsize, dbd->dbd_blkno);
               }

               /* Update atime now, because we aren't going to call vn_rdwr */
               if (prp->p_type == PT_MMAP)
                   vnm_SetAtime (evp, EFS_RWFLAGS_LOCK);

		if (!hdl_cwfault(wrt, prp, pgindx)) {
			vmemp_returnx(0);     /* free up VM empire and return */
		} else {
			minfo.cache++;
			cnt.v_pgrec++;
                        cnt.v_pgfrec++;
			u.u_ru.ru_minflt++;	/* count reclaims */
                        if (prp->p_type == PT_TEXT)
                        	cnt.v_xifrec++;
			vmemp_returnx(1);     /* free up VM empire and return */
		}
	}
	u.u_ru.ru_majflt++;		/* It's a real fault, not a reclaim */

	/*
	 * How many pages to bring in at once?
	 * Temporary expedient hack: read one page at a time.
	 */
	count = 1;

	/*
	 * First reserve memory for the vfdfill below.  We must do this 
	 * now to prevent problems when we go to insert the page in the 
	 * hash.
	 */
	if (memreserve(rp, (unsigned int)count)) {
		/*
		 * The vfd/dbd pointers could have become stale when
		 * we slept without the region lock in memreserve()
		 */
		FINDENTRY(rp, pgindx, &vfd2, &dbd2);
		vfd = vfd2;
		dbd = (efsdbd_t *)dbd2;
		/*	
		 * We went to sleep waiting for memory.
		 * check if the page we're after got loaded in
		 * the mean time.  If so, give back the memory
		 * and return
		 */
		if (vfd->pgm.pg_v) {
			memunreserve((unsigned int)count);
			vmemp_returnx(1);    /* free up VM empire and return */
		}
		if (pfd = pageincache(filevp, dbd->dbd_blkno)) {
			VASSERT((pfd->pf_data == dbd->dbd_blkno) &&
			     (pfd->pf_devvp == filevp));
			memunreserve((unsigned int)count);
			vfd->pgm.pg_pfn = pfd->pf_pfn;
			vfd->pgm.pg_v = 1;
			rp->r_nvalid++;
			if (!hdl_cwfault(wrt, prp, pgindx)) {
				vmemp_returnx(0);
			} else {
				minfo.cache++;
				cnt.v_pgrec++;
                                cnt.v_pgfrec++;
                                if (prp->p_type == PT_TEXT)
                                        cnt.v_xifrec++;
				vmemp_returnx(1);
			}
		}
	}

	vfdfill(pgindx, (unsigned) count, prp);

	/*
	 * We now have a vfd with a page frame
	 * assigned, whose rightful contents are not in the
	 * page cache. Insert the page frame, and read from
	 * disk.
	 */
	pfd = addtocache((int)vfd->pgm.pg_pfn, filevp, dbd->dbd_blkno);
	if (pfd->pf_pfn != vfd->pgm.pg_pfn) {
		VASSERT((hdl_getbits((int)pfd->pf_pfn) & VPG_MOD) == 0);
		freepfd(&pfdat[vfd->pgm.pg_pfn]);
		vfd->pgm.pg_pfn = pfd->pf_pfn;
	} else {
		VASSERT((pfd->pf_data == dbd->dbd_blkno) &&
		        (pfd->pf_devvp == filevp));
	}

	/* 
	 * Protect the pages from the user during
	 *  the I/O operation.
	 */
	hdl_user_protect(prp, space, vaddr, count, &nspace, &nvaddr, 0);

	/*
	 * Do the I/O.  We don't even bother with the strategy interface,
	 * but go straight to the rdwr interface.  If we rewrote this to
	 * do I/O straight to disk rather than via Episode buffers, this
	 * would be a call to asyncpageio.
	 */
	errorfl = vn_rdwr(UIO_READ, filevp, nvaddr, NBPG, offset, UIOSEG_PAGEIN,
			  IO_UNIT, &resid, 0);
        /*
         * If we need to allocate for a hole in the file, do it here
         */
        if (!errorfl && test_alloc) {
               errorfl = vn_rdwr(UIO_WRITE, filevp, nvaddr, DEV_BSIZE, offset,
                               UIOSEG_PAGEIN, IO_UNIT, &resid, 0);
               if (!errorfl) {
                      *dbd = efs_mapdbd(pgindx, rp, filevp, B_WRITE);
                      vfs_mark_dbds(rp, pgindx, bsize, dbd->dbd_blkno);
               }
        }

	if (errorfl || rp->r_zomb)
                goto backout;

	/*
	 * We must detect end-of-file and only read in one page worth.
	 * This requires zeroing out the end of the page for mmf's
	 */
	efs_getlength (filevp, &filelen, 0);
	if ((offset + ptob(count)) > filelen) {
	    int diff;
	    
	    diff = (offset + ptob(count)) - filelen;
	    VASSERT((diff / NBPG) == 0);
	    hdl_zero_page(nspace, nvaddr + ptob(count)-diff, diff);
	}

        /*
         * clear the modification bit
         */
	hdl_unsetbits(vfd->pgm.pg_pfn, VPG_MOD);

	/*
	 * Unprotect the pages.
	 */
	hdl_user_unprotect(nspace, nvaddr, count, 0);

	/* 	
	 * Mark the I/O done, and awaken anyone
	 * waiting for pfdats
	 */
	pfd = &pfdat[vfd->pgm.pg_pfn];
	pfdatunlock(pfd);
	cnt.v_exfod += count;	/* number of pages from file */
	vmemp_unlockx();	/* free up VM empire */
	return(count);


backout:
	/*
	 * if error during vn_rdwr, unhash the page and delete 
	 * any translations, set count to 0 and set r_zomb in the
	 * region so the process gets killed.
	 */
	VASSERT(PAGEINHASH(pfd));
	pageremove(pfd);
	hdl_user_unprotect(nspace, nvaddr, count, 1);
	pfdatunlock(pfd);
	rp->r_zomb = 1;
	vmemp_unlockx();	/* free up VM empire */
	return(0);
}

/*
 * efs_vfdcheck, efs_mapdbd -- support for efs_pageout
 *
 * Episode cannot use the standard vfs_pageout because there are various
 * switch statements in vfs_dbdcheck and vfs_mapdbd that panic if they
 * don't recognize the vnode's v_fstype, and they don't recognize VEFS.
 * So we write our own.  Ugly, but it works.
 */

/* max number of pages that efs_pageout may write out at a time */
#define EFS_MAXPGOUT	16

extern int freemem, lotsfree;
extern int stealvisited;
extern int maxpendpageouts;
extern int parolemem;

extern struct buf *bswalloc();

/*
 * efs_mapdbd() --
 *    Computes a dbd_data for a page within an MMF.  Write requests to
 *    local files cause space to be allocated (for sparse files).  Space
 *    reservation for remote files is handled by vfs_pagein().
 *
 *    Cribbed from vfs_mapdbd.
 *
 *    Because we are using Episode buffers rather than writing straight
 *    to disk, we do not compute a disk offset using efs_bmap, but just
 *    adjust the units of the logical offset.
 */
efsdbd_t
efs_mapdbd(pgindx, rp, filevp, rwflg)
int pgindx;
reg_t *rp;
struct vnode *filevp;
int rwflg;				/* B_READ or B_WRITE */
{
    daddr_t lbn, bn;
    int on;
    long offset, bsize;
    efsdbd_t ret_dbd;
    epi_anode_t ap;
    daddr_t epbn;
    int error;


    /*
     * lbn = <logical_block_number> for this part of
     *	 the file (bytes)
     * on = offset in the block (bytes)
     */
    offset = vnodindx(rp, pgindx);

    if (filevp->v_fstype != VEFS)
	panic("efs_mapdbd: not Episode file system type");

    ap = EVTOA(VTOEV(filevp));
    bsize = epia_BlockSize(ap);
    lbn = offset / bsize;
    on = offset % bsize;
    VASSERT ((on % NBPG) == 0);
    bn = lbn * (bsize / DEV_BSIZE);
    ret_dbd.dbd_blkno = bn + btodb(on);

    error = epia_Map (ap, lbn, &epbn, (struct async_device **)0);
    if (error && error != EPI_E_NOTBLOCKEDDATA) {
        ret_dbd.dbd_type = DBD_HOLE;
        return ret_dbd;
    }

    ret_dbd.dbd_type = DBD_FSTORE;
    return ret_dbd;
}

/*
 * The arguments passed to our sparse walk routine
 *
 * This is cribbed from the vfsargs_t structure.  We have eliminated the
 * ok_dbd_limit, remote, and remote_down members because the code that
 * used them can, instead, just make assumptions appropriate to Episode.
 */
typedef struct {
    preg_t *prp;
    int    flags;
    struct vnode *vp;	 		/* file vnode pointer */
    u_long isize;	 		/* file size */
    u_long bsize;	 		/* file system block size */
    long   maxpgs;	 		/* maximum length a run can be */
    size_t start;	 		/* start of a run */
    efsdbd_t  dbd_start; 		/* dbd for start */
    size_t end;		 		/* end of a run */
    efsdbd_t  dbd_end;	 		/* dbd for end */
    int    run;		 		/* a run has been detected */
} efsargs_t;

/*
 * lastpageinblock() --
 *    Given a dbd, and a file system block size, this macro determines
 *    if the dbd is the last page of the block.
 */
#define lastpageinblock(dbd, bsize) \
	((dbtob(dbd) + NBPG) % (bsize) == 0)

/*
 * Sparse walk function to free memory in the file's mapping
 *
 * Return code 0 = continue looping in calling routine.
 * Return code 1 = stop looping in calling routine.
 *
 * Cribbed from vfs_vfdcheck
 */
static int
efs_vfdcheck(rindex, vfd, dbd, args)
int rindex;
vfd_t *vfd;
dbd_t *dbd;
efsargs_t *args;
{
    preg_t *prp = args->prp;
    reg_t *rp = prp->p_reg;
    int pfn = vfd->pgm.pg_pfn;
    pfd_t *pfd = &pfdat[pfn];
    int pgbits;
    efsdbd_t cur_dbd;
    struct vnode *filevp;
    int vhand;		/* commonly used flag */
    int hard;		/* commonly used flag */
    int steal;		/* commonly used flag */
    int purge;		/* commonly used flag */

    /*
     * If we have a run and this page is not contiguous with the
     * end of our run, terminate the run.
     */
    if (args->run && rindex != (args->end + 1))
	return 1;

    /*
     * Initialize commonly used flags
     */
    vhand = (args->flags & PAGEOUT_VHAND);
    hard  = (args->flags & PAGEOUT_HARD);
    steal = (args->flags & PAGEOUT_FREE);
    purge = (args->flags & PAGEOUT_PURGE);

#ifdef OSDEBUG
    /*
     * If PAGEOUT_PURGE was set, PAGEOUT_HARD and PAGEOUT_FREE
     * must also be set.
     */
    if (purge)
	VASSERT(hard && steal);
#endif /* OSDEBUG */

    /*
     * If vhand is trying to steal pages and we have succeeded in
     * freeing enough memory for now, stop scanning for more things
     * to free.
     */
    if (vhand && steal && !hard && freemem >= lotsfree)
	return 1;

    /*
     * Keep "end" up to date so that vhand can tell the last page
     * that we visited.
     */
    if (args->run == 0)
	args->end = rindex;

    /*
     * Skip locked vfds.
     *
     * If we have a run, stop and process it.  If no run has been
     * found yet, keep looking for a run.
     */
    if (vfd->pgm.pg_lock)
	return args->run;

    /*
     * Take the lock on the page.  If it's already locked and we're
     * not doing a hard pageout, just give up on this page.
     */
    if (!cpfdatlock(pfd)) {
	if (hard == 0) {
	    /*
	     * If we have a run, stop and process it.  If no run
	     * has been found yet, keep looking for a run.
	     */
	    return args->run;
	}
	pfdatlock(pfd);
    }

    if (vhand && steal)
	++stealvisited;

    /*
     * We have the page locked.  If we are want to steal this page, we
     * see if it has been referenced.  We do not steal referenced pages
     * unless PAGEOUT_HARD is set.  If we are not going to steal this
     * page, we do not care if it has been referenced, we still want
     * to write it out if it is dirty.
     */
    pgbits = hdl_getbits(pfn);
    if (!hard && steal && (pgbits & VPG_REF)) {
	pfdatunlock(pfd);
	return args->run;
    }

    /*
     * A clean page.  Steal it if PAGEOUT_FREE is set, otherwise
     * leave it alone.
     */
    if (!(pgbits & VPG_MOD)) {
	if (!steal) {
	    pfdatunlock(pfd);
	    return args->run;
	}

    freeit:
	/*
	 * Clear HDL translation for it, return it to the free pool
	 */
	vfd->pgm.pg_v = 0;
	rp->r_nvalid--;

	/*
	 * Tell HDL, as otherwise we've cleared the VFD
	 * but potentially left an HDL translation.
	 */
#ifdef __hp9000s800
	/*
	 * Oh, what a kludge.  On PA-RISC, we really do want to delete
	 * whatever translation that exists for this page.  On 68k,
	 * hdl_deletetrans() does this, but the PA-RISC version only
	 * deletes the translation if the page is translated to the
	 * (space, vaddr) that we give it.  Since we are paging out
	 * through the psuedo-pregion, this is not really the correct
	 * thing.  The easiest way to fix this (for now) is to simply
	 * call hdl_unvirtualize() and delete whatever translation
	 * exists for this page.
	 */
	hdl_unvirtualize(pfn);
#else
	hdl_deletetrans(prp, prp->p_space,
			prp->p_vaddr + ptob(rindex - prp->p_off), pfn);
#endif /* __hp9000s800 */
	if (purge && PAGEINHASH(pfd))
	    pageremove(pfd);
	if (vhand)
	    cnt.v_dfree++;
#ifdef FSD_KI
	memfree(vfd, rp);
#else  /* ! FSD_KI */
	memfree(vfd);
#endif /* ! FSD_KI */

	return args->run;
    }

    /*
     * A dirty page.  If we have not yet determined the file size and
     * other attributes that we need to write out pages (the block
     * size), get that information now.
     */
    if (args->bsize == 0) {
	u_long isize;
	long bsize;
	struct vattr va;

	/*
	 * If this region has been marked dead, just throw away the
	 * dirty page.
	 */
	if (rp->r_zomb) {
	    hdl_unsetbits(pfn, VPG_MOD);
	    if (PAGEINHASH(pfd))
		 pageremove(pfd);
	    goto freeit;
	}

	/*
	 * Get the various attributes about the file.  Store them
	 * in args for the next time around.
	 */
	filevp = rp->r_bstore;
	if (filevp->v_fstype != VEFS)
	    panic("File type in efs_vfdcheck is not VEFS");
	bsize = epia_BlockSize(EVTOA(VTOEV(filevp)));
	args->maxpgs = EFS_MAXPGOUT;
	efs_getlength (filevp, (long *)&isize, 0);

	args->isize = isize;
	args->bsize = bsize;
    }
    else {
	filevp = rp->r_bstore;
    }

    /*
     * If this region has been marked dead, just throw away the
     * dirty page.
     */
    if (rp->r_zomb) {
	hdl_unsetbits(pfn, VPG_MOD);
	if (PAGEINHASH(pfd))
	     pageremove(pfd);
	goto freeit;
    }

    /*
     * Make sure that we have a valid dbd.  In most cases, this
     * is filled in when the page was faulted in.
     */
    cur_dbd = *((efsdbd_t *)dbd);
    VASSERT(cur_dbd.dbd_type != DBD_HOLE);
    if (cur_dbd.dbd_blkno == DBD_DINVAL) {
	cur_dbd = efs_mapdbd(rindex, prp->p_reg, args->vp, B_READ);
	VASSERT(cur_dbd.dbd_type != DBD_HOLE);
    }

    /*
     * A new run, fill in the start and end points and set the run flag.
     */
    if (args->run == 0) {
	args->start = args->end = rindex;
	args->dbd_start = args->dbd_end = cur_dbd;
	args->run = 1;

	/*
	 * We will be sending this page out.  If we will also be
	 * stealing the translations for the page, mark the page
	 * invalid since it is on its way out now.
	 */
	if (steal) {
	    vfd->pgm.pg_v = 0;
	    rp->r_nvalid--;
	}
	if (purge && PAGEINHASH(pfd))
	    pageremove(pfd);
	dbd->dbd_type = DBD_FSTORE;

	return 0;
    }

    /*
     * We already have a run, see if this page is contiguous on disk
     * with the end of our current run.
     */
    if (cur_dbd.dbd_blkno == (args->dbd_end.dbd_blkno + btodb(NBPG))) {
	args->end = rindex;
	args->dbd_end = cur_dbd;

	/*
	 * We will be sending this page out.  If we will also be
	 * stealing the translations for the page, mark the page
	 * invalid since it is on its way out now.
	 */
	if (steal) {
	    vfd->pgm.pg_v = 0;
	    rp->r_nvalid--;
	}
	if (purge && PAGEINHASH(pfd))
	    pageremove(pfd);
	dbd->dbd_type = DBD_FSTORE;

	/*
	 * See if we have hit our maximum run length.  If so, stop
	 * scanning vfds and return to efs_pageout() so that it can
	 * process this run.
	 */
	VASSERT((args->end + 1 - args->start) <= args->maxpgs);
	if ((args->end + 1 - args->start) == args->maxpgs)
	    return 1;
	else
	    return 0;
    }

    /*
     * This page is not contiguous with the current run.  Stop scanning
     * vfds and return to efs_pageout() so that it can process this run.
     */
    pfdatunlock(pfd);
    return 1;
}

/*
 * efs_pageout() --
 *   This function performs several functions, based on the
 *   setting of the 'flags' parameter.  The basic operations
 *   are:
 *     . optionally write out dirty pages (optionally free)
 *     . optionally free clean pages
 *     . optionally wait until all scheduled I/O is complete
 *
 * flags --
 *   PAGEOUT_FREE  -- free the pages when I/O complete
 *   PAGEOUT_HARD  -- we really want this page to go out. sleep for page
 *		      lock, pageout/free even if referenced.
 *   PAGEOUT_WAIT  -- wait for I/O to complete
 *   PAGEOUT_VHAND -- vhand initiated this VOP_PAGEOUT
 *   PAGEOUT_SWAP  -- swapping rather than paging (statistics)
 *
 * Cribbed from vfs_pageout.
 *
 * We are not doing I/O straight to disk, but are going via Episode's
 * buffers (i.e. efs_hpstrategy calls efs_hpustrategy calls efs_rdwr which
 * uses the regular buffers).
 *
 * Because efs_rdwr adjusts the mtime/ctime, we do not bother with that here.
 */
void
efs_pageout(prp, start, end, flags)
preg_t *prp;
size_t start, end;
int flags;
{
    vm_sema_state;			/* semaphore save state */
    reg_t *rp = prp->p_reg;
    struct vnode *filevp;
    struct vnode *devvp;
    int i;
    efsargs_t args;
    int steal;
    int vhand;
    int hard;
    int *piocnt;		/* wakeup counter used if PAGEOUT_WAIT */
    struct ucred *old_cred;

    VASSERT(rp->r_fstore == rp->r_bstore);
    steal = (flags & PAGEOUT_FREE);
    vhand = (flags & PAGEOUT_VHAND);
    hard  = (flags & PAGEOUT_HARD);

    vmemp_lockx();

    /*
     * If the region is marked "don't swap", then don't steal any pages
     * from it.  We can, however, write dirty pages out to disk (only if
     * PAGEOUT_FREE is not set).
     */
    if (prp->p_reg->r_mlockcnt && steal) {
	vmemp_unlockx();
	return;
    }

    /*
     * If the caller wants to wait until the I/O is complete, we
     * allocate a counter to count the I/Os that we schedule.
     * biodone() will decrement this counter as the I/Os complete,
     * when the counter gets to 0, biodone() will issue a wakeup
     * on the address of the counter.
     *
     * We pre-initialize the counter to 1 so that it does not hit
     * zero until we are ready for it.
     */
    if (flags & PAGEOUT_WAIT) {
	piocnt = &u.u_procp->p_wakeup_cnt;
	*piocnt = 1;
    }
    else
	piocnt = (int *)0;

    filevp = rp->r_bstore;	/* always page out to back store */

    devvp = filevp;

    /*
     * Initialize args.  We set bsize to 0 to tell vfs_vfdcheck() that
     * it must get the file size and other attributes if it comes across
     * a dirty page.
     */
    args.prp = prp;
    args.flags = flags;
    args.bsize = 0;
    args.vp = filevp;
    i = start;

    while (i <= end) {
	extern int pageiodone();
	u_int context;
	struct buf *bp;
	space_t nspace;
	caddr_t nvaddr;
	long start;
	int npages;
	long nbytes;

       /*
	* If we get into vfs_vfdcheck with a good page, end will be
	* reset so we know how far we got, otherwise there are no pages
	* we can page, so we've visited all of the ones we can page,
	* so initialize end to represent this.
	*/
	args.end = prp->p_off + end;
	args.run = 0;
	foreach_valid(rp, (int)prp->p_off + i, (int)end-i+1,
		      efs_vfdcheck, (caddr_t)&args);

	if (args.run == 0)
	    break;

	/*
	 * We have a run of dirty pages [args.start...args.end].
	 */
	VASSERT((filevp->v_vfsp->vfs_flag & VFS_RDONLY) == 0);

	context = sleep_lock();
	rp->r_poip++;
	sleep_unlock(context);

	/*
	 * Okay, get set to perform the I/O.
	 */
	npages = args.end + 1 - args.start;

	/*
	 * Allocate and initialize an I/O buffer.
	 */
	bp = bswalloc();
	bp->b_rp = rp;
	if (piocnt) {
	    context = spl6();
	    (*piocnt)++;
	    splx(context);
	}
	bp->b_pcnt = piocnt;
#ifdef FSD_KI
	/*
	 * Set the b_apid/b_upid fields to the pid (this process' pid)
	 * that last allocated/used this buffer.
	 */
	bp->b_apid = bp->b_upid = u.u_procp->p_pid;

 	/* Identify this buffer for KI */
	bp->b_bptype = B_vfs_pageout|B_pagebf;

 	/* Save site(cnode) that last used this buffer */
	bp->b_site = u.u_procp->p_faddr;
#endif /* FSD_KI */

	if (steal)
	    bp->b_flags = B_CALL|B_BUSY|B_PAGEOUT;    /* steal pages */
	else
	    bp->b_flags = B_CALL|B_BUSY;	      /* keep pages */

	bp->b_iodone = pageiodone;

	/*
	 * Make sure we do not write past the end of the file.
	 */
	nbytes = ptob(npages);
	start = vnodindx(rp, args.start);
	if (start + nbytes > args.isize) {
#ifdef OSDEBUG
	    /*
	     * The amount we are off better not be bigger than a
	     * filesystem block.
	     */
	    if (start + nbytes - args.isize >= args.bsize) {
		printf("start = %d, nbytes = %d, isize = %d, bsize = %d\n",
		    start, nbytes, args.isize, args.bsize);
		panic("efs_pageout: remainder too large");
	    }
#endif
	    /*
	     * Reset the size of the I/O as necessary.  For Episode,
	     * files, we set the size to the exact number of bytes to
	     * the end of the file.  If we were writing straight to disk, we
	     * would round up to the nearest DEV_BSIZE chunk.
	     */
	    nbytes = args.isize - start;
	}

	/*
	 * Now get ready to perform the I/O
	 */
	hdl_user_protect(prp, prp->p_space,
			 prp->p_vaddr + ptob(args.start - prp->p_off),
			 npages, &nspace, &nvaddr, steal);
	blkflush(devvp, args.dbd_start.dbd_blkno, nbytes, 1, rp);

	/*
	 * Really do the I/O.
	 */
	asyncpageio(bp, args.dbd_start.dbd_blkno, nspace, nvaddr, nbytes,
		    B_WRITE, devvp);

	/*
	 * Update statistics
	 */
	if (steal) {
	    if (flags & PAGEOUT_SWAP) {
		syswait.swap--;
		cnt.v_pswpout += npages;
		sar_bswapout += ptod(npages);
	    }
	    else if (vhand) {
		cnt.v_pgout++;
		cnt.v_pgpgout += npages;
	    }
	}

	/*
	 * If time and patience have delivered enough
	 * pages, then quit now while we are ahead.
	 */
	if (vhand && !hard && parolemem >= maxpendpageouts)
	    break;

	i = args.end - prp->p_off + 1;
    }

    if (vhand)
	prp->p_stealscan = args.end - prp->p_off + 1;

    vmemp_unlockx();

    /*
     * If we wanted to wait for the I/O to complete, sleep on piocnt.
     * We must decrement it by one first, and then make sure that it
     * is non-zero before going to sleep.
     */
    if (piocnt) {
	regrele(rp);
	i = spl6();
	if (--(*piocnt) > 0)
	    sleep(piocnt, PRIBIO+1);
	splx(i);
	reglock(rp);
    }
}

/*
 * vnvm_truncate -- invalidate/delete VM pages beyond the end of a file
 */
int
vnvm_truncate(struct vnode *vp, u_long len, osi_cred_t *credp)
{
    long filelen;
    int code;

    efs_getlength (vp, &filelen, credp);
    if (len < filelen && (vp->v_flag & VMMF))
	mtrunc (vp, len);
    return 0;
}

/*
 * vnvm_PrepareUserBuffer -- arcane interface for doing user I/O from vol ops
 *
 * RETURN VALUE --
 *	a buf whose address may be used to do the I/O.  In fact the only
 *	field we use on the HP platform at this time is b_un.b_addr,
 *	but we fill in some others to make it look like the AIX and SunOS
 *	versions.
 */
struct buf *
vnvm_PrepareUserBuffer(caddr_t uaddr, size_t length, int is_write)
{
    struct buf *bp = osi_GetBuf();

    bp->b_flags |= (is_write) ? B_WRITE : B_READ;
    bp->b_un.b_addr = uaddr;
    bp->b_bcount = length;

    return (bp);
}

/*
 * vnvm_ReleaseUserBuffer -- release resources after doing vol ops I/O
 */
void
vnvm_ReleaseUserBuffer(struct buf *bp, caddr_t uaddr, size_t length)
{
    bp->b_flags |= B_DONE;
    osi_ReleaseBuf (bp);
}
#endif /* _KERNEL */
