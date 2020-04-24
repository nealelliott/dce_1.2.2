/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved
 */

#include <dcedfs/param.h>
#include <osi.h>

#ifdef KERNEL
#include <sys/lockl.h>
#include <sys/sleep.h>
#include <sys/pri.h>
#include <sys/uio.h>
#include <sys/vnode.h>
#include <sys/vfs.h>
#include <sys/device.h>
#include <sys/user.h>

#include <dcedfs/osi_cred.h>
#include <dcedfs/osi_buf.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/osi/RIOS/RCS/osi_port_aix.c,v 4.53 1996/04/19 16:40:06 ota Exp $")

/*
 * Items to support the AIX esleep interface
 */
#define ESLEEP_QSIZE	101
#define esleep_qhash(X) (&esleep_q[((u_int)((X)&0x7fffffff) % ESLEEP_QSIZE)])
static int			esleep_q[ESLEEP_QSIZE];
static char		osi_aix_init_flag;

/*
 * Initialize "OSI" cred pointer here, to anticipate uses before
 * any OSI entry points are called.
 */
extern osi_cred_t osi_cred, *osi_credp;

osi_init()
{
    int i;

    osi_aix_init_flag = 1;

    /*
     * Init elements of the event list anchor for esleep calls
     */
    pin(&esleep_q, sizeof(esleep_q));
    for (i = 0; i < ESLEEP_QSIZE; i++)
	esleep_q[i] = EVENT_NULL;

    /*
     * Init cred (and pointer thereto) used by cm (et al.) for
     * unauthenticated access.
     */
    bzero(&osi_cred, sizeof(osi_cred_t));
    crhold(&osi_cred);	/* don't let it evaporate, since it is static */
    osi_credp = &osi_cred;

    osi_InitGeneric();
}

extern lock_t kernel_lock;

/* modify these babies so they don't allow recursive locking */
osi_PreemptionOff() {
    register int code;

    code = lockl(&kernel_lock, LOCK_SHORT);
    assert(code != LOCK_NEST);
    return code;
}

void osi_RestorePreemption(x)
register int x; {
#if 0
    if (x != LOCK_NEST)
#endif
	unlockl(&kernel_lock);
    return;
}

/*
 * Some extra handling is needed when calling AIX's version of the local
 * RDWR module, particularly the usage of the uio structure to the lower
 * routine.  Of significant importance to the AFS port is the offset in/out
 * parameter, which in two cases returns a new value back. The cases are:
 * (1) when it's a FIFO file (it's reset to zero) which we don't yet support in
 * AFS and (2) in a regular case when we write (i.e. rw == UIO_WRITE) and are
 * in appending mode (i.e. FAPPEND bit on) where offset is set to the file's
 * size. None of these cases apply to us currently so no problems occur;
 * caution if things change!
 */
int
osi_rdwr(rw, vp, base, len, offset, segflg, unit, aresid)
    enum uio_rw		rw;
    struct vnode	*vp;
    caddr_t		base;
    off_t		*offset;
    int			len, segflg, *aresid, unit;
{
    struct uio	uio_struct;
    struct iovec uiovector;
    register int code;

    /*
     * Set up the uio structure
     */
    uiovector.iov_base = (caddr_t) base;
    uiovector.iov_len = len;

    uio_struct.uio_iov = &uiovector;
    uio_struct.uio_iovcnt = 1;
    uio_struct.uio_offset = *offset;
    uio_struct.uio_segflg = UIO_SYSSPACE;
    uio_struct.uio_resid = len;
    uio_struct.uio_fmode = (rw == UIO_READ ? FREAD : FWRITE);

    code = vtognop(vp, vn_rdwr)(vp, rw, (rw == UIO_READ ? FREAD : FWRITE),
	&uio_struct, NULL, NULL);
    *aresid = uio_struct.uio_resid;
    return code;
}


/*
 * Since in AIX a vnode is included in linked lists of its associated vfs and
 * gnode we need to move these links when removing a vnode (i.e. scache entry).
 * Note that since the accompanied gnode was alloced during scache creation,
 * we have to free it here too.  We don't bother with the vnode itself since
 * it's part of the scache entry and it's handled fine by default.
 */
aix_gnode_rele(vp)
    struct vnode *vp;
{
#include <sys/syspest.h>

    register struct vnode *tvp;
    register struct vfs *vfsp = vp->v_vfsp;

    /*
     * Unlink the vnode from the list the vfs has hanging of it
     */
    tvp = vfsp->vfs_vnodes;
    if (tvp == vp)
	vfsp->vfs_vnodes = vp->v_vfsnext;
    if (vp->v_vfsnext != NULL)
	vp->v_vfsnext->v_vfsprev = vp->v_vfsprev;
    if (vp->v_vfsprev != NULL)
	vp->v_vfsprev->v_vfsnext = vp->v_vfsnext;

    /*
     * Free the allocated gnode that was accompanying the scache's vnode
     */
    if (vp->v_gnode) {
	osi_Free(vp->v_gnode, sizeof(struct gnode));
	vp->v_gnode = 0;
    }
    return 0;
}

/*
 * osi_Sleep -	sleeping in AIX 3.1
 *
 * Use the AIX e_sleep() service, it is more efficient.
 * NOTE: the caller must be holding the kernel lock!
 * NOTE: This can be called with interrupts dissabled, so don't add
 *       any calls to pageable text or data.
 */

osi_aix_Sleep (x, flags)
    caddr_t   x;
    int     flags;
{

    int xflags, eflags;
    u_int ix = *(u_int *)&x;
    /*
     * Convert flags properly for esleep
     */
    xflags = flags & PCATCH | ((flags & PMASK) > PZERO ? SWAKEONSIG : 0);
    if (xflags & SWAKEONSIG) {
	eflags = (xflags & PCATCH) ? EVENT_SIGRET: EVENT_SIGWAKE;
    } else
	eflags = EVENT_SHORT;

    return (e_sleep(esleep_qhash(ix), eflags) == EVENT_SIG);
}

/*
 * osi_Wakeup
 */
osi_aix_Wakeup(x)
    caddr_t x;
{
    u_int ix = *(u_int *)&x;
    e_wakeup(esleep_qhash(ix));
}

#include <stdarg.h>
#include <sys/fp_io.h>

void osi_uprintf(char *fmt, ...)
{
    int len, code, count;
    struct file *fd;
    char buffer[400];
    va_list ap;

    va_start(ap, fmt);
    len = osi_fprf(buffer, fmt, ap);
    va_end(ap);

    if (len == 0)
	return;
    code = fp_open("/dev/tty", O_NDELAY|O_WRONLY|O_NOCTTY,
		   0777,
		   0,
		   FP_SYS, &fd);
    if (code)
	return;
    fp_write(fd, buffer, len, 0, UIO_SYSSPACE, &count);
    fp_close(fd);
    return;
}

int osi_vprintf(char *fmt, va_list vargs)
{
    int len;    
    char buffer[400];
    
    len = osi_fprf(buffer, fmt, vargs);
    return printf("%s", buffer);
}

char *mem_getbytes(size)
{
    return osi_Alloc(size);
}

mem_freebytes(p, size)
    char *p;
{
    osi_Free(p, size);
}

osi_cred_t *
osi_aix_getucred()
{
    return u.u_cred;
}

void
osi_aix_setucred(osi_cred_t * cred)
{
    u.u_cred = cred;
}

osi_aix_suser()
{
    return suser(&u.u_error);
}

osi_aix_GetMachineName(abuffer, alen)
char *abuffer;
int alen;
{
    long tlen;
    tlen = alen;
    return kgethostname(abuffer, &tlen);
}

int osi_getattr(vp, vattrp, credp)
    struct vnode *vp;
    struct vattr *vattrp;
    osi_cred_t *credp;
{
    return (vtognop(vp, vn_getattr)(vp, vattrp));
}

/*
 * System call to return a file handle for a given file name.
 * On many systems this is the getfh() system call.
 */
int osi_getfh(fname, fhp)
    char *fname;
    osi_fhandle_t *fhp;
{
    int rc, ltype;
    label_t jbuf;
    extern kernel_lock;
    struct vnode *vp;
    struct filehandle fhandle;
    char *bp;
    int len;

    ltype = lockl(&kernel_lock, LOCK_SHORT);
    bp = (char *)osi_AllocBufferSpace();
    if (!osi_aix_suser()) {
	osi_setuerror(EPERM);
	goto out;
    }
    if (setjmpx(&jbuf) == 0) {
	copyinstr(fname, bp, osi_BUFFERSIZE, &len);
	rc = osi_lookupname(bp, OSI_UIOSYS, NO_FOLLOW, (struct vnode *)0, &vp);
	if (rc == 0) {
	    rc = osi_vptofid(vp, (struct fid *)&fhandle.fh_fid);
	    if (rc == 0) {
		fhandle.fh_fsid.val[0] = vp->v_vfsp->vfs_fsid.val[0];
		fhandle.fh_fsid.val[1] = vp->v_vfsp->vfs_fsid.val[1];
		copyout((char *)&fhandle, (char *)fhp, sizeof(fhandle));
	    }
	    VN_RELE(vp);
	}
	clrjmpx(&jbuf);
    } else {
	rc = EINTR;
    }

out:
    osi_FreeBufferSpace((struct osi_buffer *)bp);
    if (ltype != LOCK_NEST)
	unlockl(&kernel_lock);

    osi_setuerror(rc);
    return (rc? -1 : 0);
}

/*
 * Set the real time
 */
long osi_SetTime(atv)
    struct timeval *atv;
{

    struct timestruc_t t;

    t.tv_sec = atv->tv_sec;
    t.tv_nsec = atv->tv_usec;
    settimer(TIMEOFDAY, &t);
    return 0;
}

int
osi_Panic(int lineno, char *fileid)
{
    printf("assertion failure at line %d\n", lineno);
    panic(fileid);
    return 0;
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
    bp->b_bcount = length;
    bp->b_blkno = dblkno;
    osi_set_bdev(bp, device);
    bp->b_vp = vp;
    bp->b_resid = bp->b_error = 0;
    osi_set_iodone(bp, iodone);

    bp->b_event = EVENT_NULL;

    bp->b_flags |= (B_BUSY | flags);
    bp->b_flags &= ~B_DONE;
}

/*
 * Allocate/deallocate non pageable memory 
 *
 * XXX Could do better by just allocating on the pinned heap but that
 * implies modification to osi_Alloc at this late stage. Hence choose
 * the following method for now.
 */
opaque
osi_AllocPinned(size_t asize)
{
    int code;
    opaque ptr;

    ptr = osi_Alloc(asize);
    code = pin(ptr, asize);
    assert(code == 0);
    return ptr;
}

void
osi_FreePinned(opaque ptr, size_t asize)
{
    int code;

    code = unpin(ptr, asize);
    assert(code == 0);
    osi_Free(ptr, asize);
}

void 
osi_Pin(opaque ptr, size_t asize)
{
    int code;

    code = pin(ptr, asize);
    assert(code == 0);
}

void
osi_Unpin(opaque ptr, size_t asize)
{
    int code;
    
    code = unpin(ptr, asize);
    assert(code == 0);
}

#endif	/* KERNEL */
