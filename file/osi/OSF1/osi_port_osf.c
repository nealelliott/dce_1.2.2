/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * (c) Copyright 1990, OPEN SOFTWARE FOUNDATION, INC.
 * ALL RIGHTS RESERVED
*/
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */


#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>		/* Standard vendor system headers */
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#if defined(AFS_OSF11_ENV)
#include <sys/malloc.h>
#include <mach/vm_param.h>
#include <vm/vm_kern.h>
#endif

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/osi/OSF1/RCS/osi_port_osf.c,v 4.14 1996/04/19 16:40:05 ota Exp $")

/* 
 * Emulates the lookupname() SunOS routine 
 */

int osi_osflookupname(pathname, segment, followlink, parentvp, compvp)
char * pathname;			/* name to look at */
int segment;				/* tell if path in user-land
                                         * (OSI_UIOUSER) or kernel-land
                                         * (OSI_UIOSYS) */
int followlink;				/* whether to follow sym-links */
struct vnode **parentvp;		/* if non-zero returns the parent vnode
                                         * pointer for the last component */
struct vnode **compvp;			/* non-zero => the component vnode */
{
    struct nameidata *ndp = &u.u_nd;
    int error;

    /*
     * Get vnode for file.
     */
    ndp->ni_nameiop = (followlink ? (LOOKUP|FOLLOW) : (LOOKUP));
    ndp->ni_segflg = (segment == OSI_UIOUSER) ? UIO_USERSPACE: UIO_SYSSPACE;
    ndp->ni_dirp = pathname;
    if (error = namei(ndp))
   	return (error);
    if (compvp != 0) 
      *compvp = ndp->ni_vp;
    if (parentvp != 0) 
      *parentvp = ndp->ni_dvp;
    return 0;
}


osi_GetMachineName(buf, len)
    char *buf;
    int   len;
{
    HOSTNAME_READ_LOCK();
    if (len < hostnamelen+1){
	HOSTNAME_READ_UNLOCK();
        return (-1);
    }
    bcopy(hostname, buf, hostnamelen+1);
    HOSTNAME_READ_UNLOCK();
    return (0);
}

long osi_mallocCnt = 0;
long osi_mmemUsage = 0;
long osi_munderway = 0;

long osi_bulkallocCnt = 0;
long osi_bulkmemUsage = 0;
long osi_bulkunderway = 0;

#if defined(AFS_OSF11_ENV)
/*
 * In OSF/1 1.1.1, we use two different memory allocation
 * interfaces.  For requests of MALLOCMAX or smaller, we
 * use the standard caching allocator, which is optimized
 * for small requests.  In fact, on a busy system the allocator
 * can try (and fail) indefinitely to collect enough memory to
 * satisfy a large request, where "large" means "more than a
 * small number of VM pages in length".  For such requests,
 * we interact directly with the Mach VM system, and
 * bypass the allocator.
 */

/*
 * We assume that we can reliably collect 16 or fewer pages
 * from the allocator at one time.
 */
#define	MALLOCMAX	(16*PAGE_SIZE)

/*
 * For debugging purposes, make the switchover point patchable.
 * Can be changed _only_ before DFS startup, of course.
 */
int osi_mallocmax = MALLOCMAX;

/*
 * For tuning purposes, make the limit for M_DFS memory allocation
 * patchable, too.  No need to set this before DFS startup, though
 * setting it after a hang will likely be too late.
 *
 * Note that this affects _only_ memory allocated through
 * malloc(), not memory allocated through osf_bulk_malloc().
 */
int osi_dfsmemlim = 4*1024*1024;

/*
 * OSF/1 bulk allocation/freeing.
 */
void *
osf_bulk_malloc(u_long msize)
{
	void *va = NULL;
	vm_size_t size;
	vm_offset_t addr;

	size = round_page((vm_size_t)msize);
	++osi_bulkunderway;
	if (kernel_memory_allocate(kernel_map, &addr, size,
	    VM_PROT_READ|VM_PROT_WRITE, 0) == KERN_SUCCESS) {
		va = (void *)addr;
		osi_bulkmemUsage += size;
		++osi_bulkallocCnt;
	}
	--osi_bulkunderway;
	return va;
}

void
osf_bulk_free(void *va, u_long msize)
{
	vm_offset_t addr;
	vm_size_t size;

	size = round_page((vm_size_t)msize);
	addr = (vm_offset_t)va;
	kmem_free(kernel_map, addr, size);
	osi_bulkmemUsage -= size;
	--osi_bulkallocCnt;
}
#endif /* AFS_OSF11_ENV */

void *
osi_kalloc(size)
	int size;
{
	void *addr;
	static int first = 1;

#if defined(AFS_OSF11_ENV)
    /* 
     * Use bulk_malloc(), which is more 
     * reliable for large requests since it
     * will block on VM if necessary to allocate 
     * large chunks of memory.
     *
     * This should only be used for large
     * allocation requests.  Ideally, it should 
     * also only be used for one-time only 
     * requests, though that information
     * is not available at this level.
     */
    if (size > osi_mallocmax)
		addr = (struct osimem *)osf_bulk_malloc(size);
    else {
		if (first) {
			kmemsetlimit(M_DFS, osi_dfsmemlim);
			first = 0;
		}
		++osi_munderway;
		MALLOC(addr, void *, size, M_DFS, M_WAITOK);
		--osi_munderway;
		++osi_mallocCnt;
		osi_mmemUsage += size;
	}
#else /* AFS_OSF11_ENV */
	addr = (void *)kalloc(size);
	++osi_mallocCnt;
	osi_mmemUsage += size;
#endif /* AFS_OSF11_ENV */
	return addr;
}

void
osi_kfree(addr, size)
	void *addr;
	int size;
{
#if defined(AFS_OSF11_ENV)
    /* 
     * Use ultra-reliable memory allocator for
     * large (presumably one time only) requests.
     */
    if (size > osi_mallocmax)
		osf_bulk_free(addr, size);
    else {
		FREE(addr, M_DFS);
		--osi_mallocCnt;
		osi_mmemUsage -= size;
	}
#else /* AFS_OSF11_ENV */
	kfree(addr, size);
	--osi_mallocCnt;
	osi_mmemUsage -= size;
#endif /* AFS_OSF11_ENV */
}

long osi_GetPid()
{
    long pids[2];

    getpid(osi_curproc(), (void *)0, pids);
    return pids[0];
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

#ifdef KERNEL
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

    bp->b_flags |= (B_BUSY | flags);
    bp->b_flags &= ~B_DONE;
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
	  case 0:			/* fully word aligned */
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

#endif KERNEL

/*
 * Initialize "OSI" cred pointer here, to anticipate uses before
 * any OSI entry points are called.
 */

void osi_init(void)
{
    /*
     * Init cred (and pointer thereto) used by cm (et al.) for
     * unauthenticated access.
     */
    osi_credp = crget();
    crhold(osi_credp);	/* don't let it evaporate */

    osi_InitGeneric();
}
