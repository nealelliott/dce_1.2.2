/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_port_mach.h,v $
 * Revision 1.1.483.1  1996/10/02  17:58:35  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:05  damon]
 *
 * $EndLog$
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/osi/OSF1/osi_port_mach.h,v 1.1.483.1 1996/10/02 17:58:35 damon Exp $ */

/*
 *      Copyright (C) 1996, 1992 Transarc Corporation
 *      All rights reserved.
 */

/* OSF/1 port information */

#ifndef TRANSARC_OSI_PORT_MACH_H
#define	TRANSARC_OSI_PORT_MACH_H

/* 
 * This file contains port related issues between OSF and standard BSD VFS
 * kernels. Note that only simple (i.e. defines) OS differences are handled here; 
 * the remaining are handled in cm_subr_aix.c, cm_vops_aix.c and even in the 
 * remaining cm modules.
 */

/*
 * Have osi.h define constants to convert sockaddr's from 
 * pre-bsd4.4 style to bsd4.4 
 */
#define COMPAT_43 1

#include <sys/types.h>
#include <sys/param.h>
#ifndef _KERNEL
#include <dirent.h>
#include <sys/fs/nfs/nfs.h>
#endif /* !_KERNEL */
#ifdef _KERNEL
#include <dce/ker/pthread.h>
#endif /* _KERNEL */

#define OSI_MAXNAMLEN	NAME_MAX
#define OSI_MAXHOSTNAMELEN	MAXHOSTNAMELEN

typedef off_t osi_off_t;
typedef int   osi_timeout_t;
typedef struct fhandle osi_fhandle_t;
typedef struct fid osi_fid_t;
typedef struct buf osi_buf_t;
typedef struct statfs osi_statfs_t;

#define osi_statfs_devnum(svfs) ((svfs)->f_fsid.val[0])
#define osi_statfs_vfstype(svfs) ((svfs)->f_fsid.val[1])

#ifndef	KERNEL
typedef struct direct osi_direct_t;
#endif

/*
 * BSD-style timeout()
 */
#define	OSI_BSD_TIMEOUT		1

/*
 * Traditional single-threaded sleep/wakeup synchronization
 */
#define OSI_SINGLE_THREADED	1

/* IO_APPEND and similar flags */
#define OSI_FAPPEND	IO_APPEND
#define OSI_FEXEC	0	/* disables it */
#define OSI_FSYNC	IO_SYNC

/*
 * Access time fields in vattr structure
 */
#define osi_sec			tv_sec
#define osi_subsec		tv_usec	/* name of subsecond field of timeval */
typedef struct timeval osi_timeval_t;	/* the one with subsec as a field */
#define osi_UTimeFromSub(a,b)	((a).sec = (b).tv_sec, (a).usec = (b).tv_usec)
#define osi_SubFromUTime(a,b)	((a).tv_sec = (b).sec, (a).tv_usec = (b).usec)
#define osi_TvalFromSub(a,b)	((a) = (b))
#define osi_SubFromTval(a,b)	((a) = (b))
#define osi_ToUTime(a)
#define osi_ToSubTime(a)
#define osi_SubUnit	1000000

/*
 * Macros that deal with vfs/vnode fields
 */
#define osi_vInitLocks(vc)	(vc)->v_shlockc = (vc)->v_exlockc = 0
#define osi_vfs			mount
#define osi_vfs_op              m_op
#ifdef _KERNEL
#define osi_vSetVfsp(vc,vfsp) \
	insmntque((vc), ((vfsp) == NULLMOUNT ? DEADMOUNT : (vfsp)))
#else
#define osi_vSetVfsp(vc,vfsp)   (vc)->v_mount = (vfsp)
#endif /* _KERNEL */
#define osi_vnodeops		vnodeops	/* type for v_op */
#define osi_ufsVnodeops		ufs_vnodeops	/* ufs's vnode operations */
#define osi_vfsops		vfsops
#define	osi_vType(vc)		(vc)->v_type
#define	osi_vSetType(vc,type)   (vc)->v_type = (type)
#define	osi_IsAfsVnode(vc)	((vc)->v_op == (struct osi_vnodeops *) xvfs_ops)
#define	osi_SetAfsVnode(vc)	(vc)->v_op = xvfs_ops
#define osi_SetVnodeOps(vc,op) \
	((struct vnode *)(vc))->v_op = (struct osi_vnodeops *)(op)
#define osi_vptofid(vp, fidp, code) \
	VFS_VPTOFH(vp, fidp, code)

#define osi_getfh(path, fhp)	getfh(path, fhp)

#define OSI_VP_TO_VFSP(VP)      (VP)->v_mount
#define OSI_IS_MOUNTED_ON(VP) \
	((VP)->v_vfsmountedhere != (struct osi_vfs *) 0)
#define OSI_ISFIFO(VP)		((VP)->v_type == VFIFO)
#define OSI_ISVDEV(VP) \
	((VP)->v_type == VBLK || (VP)->v_type == VCHR || OSI_ISFIFO(VP))
#define OSI_ISDIR(VP)		((VP)->v_type == VDIR)
#define OSI_ISREG(VP)		((VP)->v_type == VREG)
#define OSI_ISLNK(VP)		((VP)->v_type == VLNK)
#define OSI_IS_SWAPVP(vp)	0

#define OSI_VN_RELE(VP)		VN_RELE(VP)
#define OSI_VN_HOLD(VP)         VN_HOLD(VP)

/*
 * Locking for R/W vnode members (v_count, v_flags)
 */
#define osi_lockvp(vp)
#define osi_unlockvp(vp)

/*
 * dnlc interfaces
 */
#define osi_dnlc_enter(dvp, fname, vp)
#define osi_dnlc_lookup(dvp, fname)	((struct vnode *)0)
#define osi_dnlc_fs_purge1(vop)		0
#define osi_dnlc_purge_vp(vp)
#define osi_dnlc_iter_purge(func, arg)	0
#define osi_dnlc_purge_vfsp(vfs, count)	0
#define osi_dnlc_remove(dvp, fname)

/* Define parameters that gives this platform's highest file address: ours is
 * still 0x7fffffff. */

#define OSI_MAX_FILE_PARM_CLIENT	OSI_MAX_FILE_LIMIT(31,0)
#define OSI_MAX_FILE_PARM_SERVER	OSI_MAX_FILE_LIMIT(31,0)

/*
 * Generic calls to vnode kernel routines
 */
#define OSI_VN_INIT(VP, VFSP, TYPE, DEV, EVN)   { \
        (VP)->v_flag = 0; \
        (VP)->v_usecount = 1; \
        osi_vInitLocks(VP); \
        osi_vSetVfsp(VP, (VFSP)); \
        osi_vSetType(VP, (TYPE)); \
        (EVN)->vd_rdev = (DEV); \
}

#define	osi_lookupname(p, seg, f, dirvpp, compvpp) \
    osi_osflookupname(p, seg, f, dirvpp, compvpp)
#define	osi_rdwr(rw, vp, base, len, off, seg, flag, cred, residp) \
	vn_rdwr(rw, vp, base, len, off, seg, flag, cred, residp)

#define osi_vattr_init(vap, mask)	vattr_null(vap)
#define osi_vattr_add(vap, mask)
#define osi_vattr_sub(vap, mask)

/*
 * The vattr mask is ignored on this system, so all the
 * symbols related to it can be set to zero.
 */
#define	OSI_VA_TYPE		0
#define OSI_VA_MODE		0
#define OSI_VA_UID		0
#define OSI_VA_GID		0
#define OSI_VA_FSID		0
#define OSI_VA_NODEID		0
#define OSI_VA_NLINK		0
#define OSI_VA_SIZE		0
#define OSI_VA_ATIME		0
#define OSI_VA_MTIME		0
#define OSI_VA_CTIME		0
#define OSI_VA_RDEV		0
#define OSI_VA_BLKSIZE		0
#define OSI_VA_NBLOCKS		0
#define OSI_VA_VCODE		0
#define	OSI_VA_ALL		0
#define OSI_VA_STAT		0
#define OSI_VA_TIMES		0

#define osi_setting_mode(vap)		((vap)->va_mode != (u_short) VNOVAL)
#define osi_set_mode_flag(vap)

#define osi_setting_uid(vap)		((vap)->va_uid != (uid_t) VNOVAL)
#define osi_set_uid_flag(vap)

#define osi_setting_gid(vap)		((vap)->va_gid != (gid_t) VNOVAL)
#define osi_set_gid_flag(vap)

#define osi_setting_uid_or_gid(vap)	((vap)->va_uid != (uid_t) VNOVAL || \
					 (vap)->va_gid != (gid_t) VNOVAL)

#define osi_setting_size(vap)		((vap)->va_size != VNOVAL)
#define osi_set_size_flag(vap)

#define osi_setting_atime(vap)		((vap)->va_atime.tv_sec != VNOVAL)
#define osi_st_atime_flag(vap)

#define osi_setting_mtime(vap)		((vap)->va_mtime.tv_sec != VNOVAL)
#define osi_set_mtime_flag(vap)

#define osi_setting_ctime(vap)		((vap)->va_ctime.tv_sec != VNOVAL)
#define osi_set_ctime_flag(vap)

#define osi_setting_link(vap)		((vap)->va_nlink != (short) VNOVAL)
#define osi_set_link_flag(vap)

#define osi_setuerror(e)   (osi_uerror = (e))
#define osi_getuerror()    (osi_uerror)

/*
 * Return address of calling function, if available.
 */
#define osi_caller()		0

#ifdef _KERNEL

#define osi_printf 		printf
#define osi_vprintf 		vprintf
#define osi_uprintf 		uprintf

#ifdef DFS_NOASSERTS
#define osi_assert(x)  0
#else
#define osi_assert(x) \
    ((x) || (panic(__FILE__ ": " #x), 0))
#endif

/*
 * Default macros for sleep/wakeup calls on osi_WaitHandle structures
 * (these just take pointers, not wait handles -ota 900316)
 */
#define	osi_Sleep(x)		sleep((caddr_t) (x), PZERO - 2)
#define osi_SleepInterruptably(x) \
    mpsleep(x, (PZERO + 1), "AfsWait", 0, (void *)NULL, 0)
#define	osi_Wakeup(x)		wakeup((caddr_t) (x))

/*
 * Macros for suspending execution for a given number of seconds
 */
#define osi_Pause(seconds)	\
  (void) osi_Wait(((seconds) * 1000), \
	(struct osi_WaitHandle *)NULL, 0 /* =>no interrupts */)

#define osi_copyin(src, dst, len)	copyin(src, dst, len)
#define osi_copyinstr(src, dst, len, copied) copyinstr(src, dst, len, copied)
#define osi_copyout(src, dst, len)	copyout(src, dst, len)

/*
 * Generic process stuff
 */
#define osi_curproc()		(u.u_procp)
#define osi_ThreadUnique()	osi_curproc()
#define osi_ThreadID()		(osi_curproc()->p_pid)

/*
 * OSI malloc/free definitions (see osi.h).
 */
#define	osi_kalloc_r(size)	kalloc(size)
#define	osi_kfree_r(ptr, size)	kfree(ptr, size)

/*
 * Allocate/deallocate non pageable memory 
 */
#define osi_AllocPinned(asize)  	osi_Alloc(asize)
#define osi_FreePinned(ptr, asize) 	osi_Free(ptr, asize)

/*
 * Prevent paging out of memory
 */
#define osi_Pin(ptr, asize) 	/* NULL */
#define osi_Unpin(ptr, asize) 	/* NULL */

/*
 * osi_GetTime (Get system time) definitions
 * osi_GetTime always returns (seconds, microseconds) in two 32-bit words.
 * This is conventionally a ``struct timeval''.
 * Someday, this will be typedef'ed as something like an osi_secusec_t type,
 * and we'll have special macros for it.  It happens to be the same layout as
 * an afsTimeval structure, but we can't know that in this file/osi layer.
 * In another day, OS-dependent macros to convert between osi_GetTime
 * results, osi_secusec_t values, an osi_secnsec_t type, and the values that
 * are used for vattr times will exist.  The ones that refer to any
 * IDL-based structures, such as the afsTimeval structure, will have to
 * exist outside file/osi, but will probably be placed in file/config
 * so that those macros can know about the definitions of the IDL-based
 * types without forcing that knowledge onto the file/osi files.
 */
#define	osi_GetTime(x)		microtime(x)

/*
 * Quick way to read the clock for callers who only care about seconds;
 * on some platforms this returns seconds and nanoseconds.
 */
#define osi_GetTimeForSeconds(x) microtime(x)

#else /* !_KERNEL */
/*
 * osi_GetTime (Get system time) definitions
 */
#define osi_GetTime(x)		gettimeofday(x, (struct timezone *)0)
#define	osi_Time()		time((time_t *)0)

#define osi_gettimeofday(tp,tzp) gettimeofday((tp), (tzp))

/* some architectures don't support wait3 */
#define osi_wait3(st, opt, ru)	wait3(st, opt, ru)

#define osi_GetMaxNumberFiles() getdtablesize()
#endif /* _KERNEL */

enum vcexcl {NONEXCL, EXCL};	/* (non)exclusive create */

#define	NO_FOLLOW	0
#define	FOLLOW_LINK	1

#define va_nodeid	va_fileid
#define v_count		v_usecount

#ifdef _KERNEL
#define VN_HOLD(vp) VREF(vp)
#define VN_RELE(vp) VUNREF(vp)
#define xrele(x)
#endif /* _KERNEL */

#define osi_ThreadCreate(start_routine, args, blockPreemption, restorePreemption, threadname, code)	\
    { \
	pthread_t new_thread;	\
	code = pthread_create( \
	&new_thread, pthread_attr_default, \
	(pthread_startroutine_t)start_routine, args); \
    } 

#ifdef _KERNEL
#define osi_suser(cr)	(suser(u.u_cred, &u.u_acflag) == 0)
#else /* ! _KERNEL */
#define osi_suser(cr)   (suser() == 0)
#endif /* _KERNEL */

#define DEFINE_OSI_UERROR  int osi_uerror;

#ifdef KERNEL
extern osi_GetMachineName _TAKES((char *buf, int len));

#define osi_PreemptionOff()		0
#define osi_RestorePreemption(x)

#ifndef _KERNEL
#define osi_ExclusiveLockNoBlock(fid)	flock((fid), LOCK_EX | LOCK_NB)
#define osi_UnLock(fid)			flock((fid), LOCK_UN)
#endif /* _KERNEL */

extern void Debugger();
#undef osi_assert
#define osi_assert(x) \
    (!(x) ? (printf ("assertion failed: line %d, file %s\n",\
                    __LINE__,__FILE__), Debugger("assertion failed"), 0) : 0)
#endif
/*
 * make a declaration of these functions available at any place that might use
 * the above macros
 */
extern int gettimeofday _TAKES((struct timeval *tp, struct timezone *tzp));
extern int bcmp _TAKES(( /* char *ptr1, char *ptr2, int len */ ));

#ifndef AFS_OSF11_ENV
extern void bcopy _TAKES(( /* char *src, char *dest, int len */ ));
extern void bzero _TAKES(( /* char *ptr, int len */ ));
#endif /* AFS_OSF11_ENV */
#ifdef	_KERNEL
extern void *osi_Memset _TAKES((void *ptr, int val, size_t len));
#endif


#ifndef KERNEL                          /* somethings don't exist in kernel */
extern int fflush  _TAKES(( FILE *stream ));
extern int fclose  _TAKES(( FILE *stream ));
extern void *malloc _TAKES((size_t size));
extern void abort _TAKES(( void ));
extern void exit _TAKES(( int status ));
#endif

#endif /* TRANSARC_OSI_PORT_MACH_H */
