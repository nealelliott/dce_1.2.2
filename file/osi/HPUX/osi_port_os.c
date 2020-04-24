/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1992 Transarc Corporation - All rights reserved */

/* This is a placeholder for routines unique to the port of AFS to AIX */

#include <dcedfs/param.h>
#include <utime.h>
#include <osi.h>
#include <osi_uio.h>
#include <osi_intr.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/osi/HPUX/RCS/osi_port_os.c,v 1.11 1996/04/19 16:40:03 ota Exp $")

/*
 * Return number of groups in a struct ucred.
 */
osi_GetNGroups(credp)
struct ucred *credp;
{

  int i;

  for (i=0; i<OSI_MAXGROUPS; i++)
    if (credp->cr_groups[i]==OSI_NOGROUP)
      break;

  return(i);

}

#ifdef	_KERNEL

int
osi_GetMachineName(buf, len)
    char *buf;
    int len;
{
    extern char hostname[];

    strncpy (buf, hostname, len);
    return 0;
}

/*
 * Set the real time
 */
long osi_SetTime(atv)
    struct timeval *atv;
{
    int s;
    boottime.tv_sec += atv->tv_sec - time.tv_sec;
    s = osi_splclock(); time = *atv; (void) splx(s);
    resettodr();
    return 0;
}

/*
 * For kernels without their own vprintf
 */
int vprintf(char *fmt, va_list vargs)
{
    long *arg = (long *)vargs;
    return printf(fmt, arg[0], arg[1], arg[2], arg[3], arg[4], arg[5]);
}

#define MEMSET_LOOP_THRESHOLD 8
void *osi_Memset(void *ptr, int val, size_t len)
{
    unsigned char *cptr = (unsigned char *)ptr;
    unsigned char  valc = (unsigned char)val;
    unsigned long *lptr = NULL;
    unsigned long  vall = (valc<<24 | valc<<16 | valc<<8 | valc);
	
    if (len > MEMSET_LOOP_THRESHOLD) {
	/*
	 * Take care of non word aligned pointers.
	 * Fall throughs are intentional to catch remaining bytes.
	 */
	switch ((unsigned)cptr & (sizeof(long)-1)) {
	  case 1:			/* byte aligned on 1st byte */
	    *cptr++ = valc;
	    len--;
	  case 2:			/* short aligned */
	    *cptr++ = valc;
	    len--;
	  case 3:			/* byte aligned on 3rd byte */
	    *cptr++ = valc;
	    len--;
	  case 0:			/* cptr is double-word aligned */
	    break;
	  default:			/* add more cases for bigger words */
	    panic("osi_Memset: bad alignment");
	    break;
	}
	/*
	 * Now we are fully word-aligned. Use full-word stores until
	 * residual too small to store efficiently.  Convert our pointer
	 * first.
	 */
	lptr = (unsigned long *)cptr;
	while (len >= sizeof(long)) {
	    *lptr++ = vall;
	    len -= sizeof(long);
	}
	cptr = (unsigned char *)lptr;
    }
    /*
     * Clean up residual or fall through if len < THRESHOLD
     */
    while (len--)
	*cptr++ = valc;
}	

#endif	/* _KERNEL */

#ifndef	_KERNEL

/*
 * Return the maximum number of files that can be opened
 */
int osi_GetMaxNumberFiles()
{
    struct rlimit res;

    if (getrlimit(RLIMIT_NOFILE, &res) < 0)
	return FD_SETSIZE;	/* return the width of the select fd bitmask */

    if (res.rlim_cur == MAXFUPLIM)
	return FD_SETSIZE;	/* return the width of the select fd bitmask */
    else
	return res.rlim_cur;	/* return soft limit */
}

/*
 * Our friend utimes does not exist in HP/UX, so we have
 * convert BSD style utimes(char *path, struct timeval *tvp) to
 * HP style utime(char *path, struct utimbuf *timbufp)
 */
int utimes(path, tvp)
char		*path;
struct timeval	*tvp;
{
	struct utimbuf tmpbuf;

	tmpbuf.actime = tvp[0].tv_sec;
	tmpbuf.modtime = tvp[1].tv_sec;

	return utime(path, &tmpbuf);
}
#endif	/* _KERNEL */

#ifdef	_KERNEL

/*
 * The standard uiomove() doesn't properly handle the case where the source
 * is not in space #0 (e.g., it can be on the stack when cm_vdirent.c calls).
 * It would be nice to handle this inside uiomove() itself.
 */
osi_uiomove_ux(cp, n, flg, uio)
    caddr_t cp;
    int n;
    int flg;
    struct uio *uio;
{
	enum uio_rw rw;
	register struct iovec *iov;
	register u_int cnt;

	if (uio->uio_seg != UIOSEG_KERNEL)
	    return uiomove(cp, n,flg, uio);

	rw = (enum uio_rw)(flg & UIO_RW);
	while (n > 0 && uio->uio_resid) {
		iov = uio->uio_iov;
		cnt = iov->iov_len;
		if (cnt == 0) {
			uio->uio_iov++;
			uio->uio_iovcnt--;
			continue;
		}
		if (cnt > n)
			cnt = n;

		if (rw == UIO_READ)
			bcopy((caddr_t)cp, iov->iov_base, cnt);
		else
			bcopy(iov->iov_base, (caddr_t)cp, cnt);
		iov->iov_base += cnt;
		iov->iov_len -= cnt;
		uio->uio_resid -= cnt;
		uio->uio_offset += cnt;
		cp += cnt;
		n -= cnt;
	}
	return(0);
}

/*
 * Initialize buf bp for disk I/O.
 */
void
osi_BufInit(
  struct buf *bp,
  int flags,			/* buf flags */
  caddr_t addr,			/* in-core data area */
  size_t length,		/* transfer length */
  dev_t device,			/* disk device */
  daddr_t dblkno,		/* disk address */
  struct vnode *vp,		/* vnode for I/O */
  osi_iodone_t (*iodone)(struct buf *)) /* iodone function */
{
    bp->b_un.b_addr = addr;
    bp->b_bufsize = bp->b_bcount = length;
    bp->b_blkno = dblkno;
    osi_set_bdev(bp, device);
    bp->b_vp = vp;
    bp->b_resid = bp->b_error = 0;
    osi_set_iodone(bp, iodone);

    bp->b_offset = dbtob(bp->b_blkno);
    bp->b_proc = u.u_procp;
    bp->b_spaddr = KERNELSPACE; /* our buffers always in-kernel */
#ifdef	HPUX80
    bp->b_rtpri = (u.u_procp->p_flag & SRTPROC) ?
			u.u_procp->p_rtpri : RTPRIO_MAX + 1;
#endif	/* HPUX80 */
    bp->b_flags |= B_PHYS;

    bp->b_flags |= (B_BUSY | flags);
    bp->b_flags &= ~B_DONE;
}

#endif	/* _KERNEL */

void osi_init(void)
{
    /*
     * Init the pointer to the CM's global cred structure.
     */
    osi_credp = crget();
    crhold(osi_credp);	/* don't let it evaporate */

    osi_InitGeneric();
}
