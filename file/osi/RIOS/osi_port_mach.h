/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/osi/RIOS/RCS/osi_port_mach.h,v 12.27 1996/04/26 18:20:28 ota Exp $ */

/*
 *      Copyright (C) 1996, 1992 Transarc Corporation
 *      All rights reserved.
 */

/* AIX 3.x port information */

#ifndef TRANSARC_OSI_PORT_MACH_H
#define	TRANSARC_OSI_PORT_MACH_H

/*
 * This file contains port related issues between AIX 3.1 & standard BSD VFS
 * kernels. Note that only simple (i.e. defines) OS differences are handled
 * here.
 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/pri.h>
#include <sys/lkup.h>
#include <sys/vnode.h>
#include <sys/uio.h>
#include <sys/user.h>
#ifndef _KERNEL
#include <dirent.h>
#endif /* !_KERNEL */

typedef off_t osi_off_t;
typedef int   osi_timeout_t;
typedef struct fid osi_fid_t;
typedef struct buf osi_buf_t;
typedef struct statfs osi_statfs_t;

#define osi_statfs_devnum(svfs) ((svfs)->f_fsid.val[0])
#define osi_statfs_vfstype(svfs) ((svfs)->f_fsid.val[1])

#ifndef	KERNEL
typedef struct dirent osi_direct_t;
#endif

/*
 * define macros for encoding major/minor devices for DFS protocol
 *
 *	The DFS protocol encodes a device with major number J minor number N
 *	in two words.  deviceNumber contains the lower 16 bits of J in its
 *	upper 16 bits and the lower 16 bits of N in the lower 16 bits.
 *	deviceNumberHighBits contains the upper 16 bits of J in its
 *	upper 16 bits and the upper 16 bits of N in the lower 16 bits.
 *	For the RIOS, this means deviceNumber corresponds to the native dev_t.
 *
 *	osi_EncodeDeviceNumber(statP, dev) places dev into statP status structure
 *	osi_DecodeDeviceNumber(statP, devP) takes major/minor info from stat
 *		structure and puts it in '*devP'.  This returns 1 is the device
 *		is representable on the local architecture.
 */
#define osi_EncodeDeviceNumber(statP, dev)	\
MACRO_BEGIN \
    (statP)->deviceNumber = dev; \
    (statP)->deviceNumberHighBits = 0; \
MACRO_END

#define osi_DecodeDeviceNumber(statP, devP) \
    ((statP)->deviceNumberHighBits ? ((*(devP) = 0), 0) \
     : ((*(devP) = (statP)->deviceNumber), 1))

/* Define parameters that gives this platform's highest file address: ours is
 * still 0x7fffffff. */

#define OSI_MAX_FILE_PARM_CLIENT	OSI_MAX_FILE_LIMIT(31,0)
#define OSI_MAX_FILE_PARM_SERVER	OSI_MAX_FILE_LIMIT(31,0)

/*
 * BSD-style timeout()
 */
#define	OSI_BSD_TIMEOUT		1

/*
 * Traditional single-threaded sleep/wakeup synchronization
 */
#define OSI_SINGLE_THREADED	1

/*
 * Macros that deal with vfs/vnode fields
 */
#define osi_vInitLocks(vc)	(vc)->v_lock[0] = (vc)->v_lock[1] = 0
#define osi_vfs			vfs
#define osi_vfs_op		vfs_ops
#define osi_vSetVfsp(vc,vfsp)	(vc)->v_vfsp = (vfsp)
#define osi_vnodeops		vnodeops	/* type for v_op */
#define osi_ufsVnodeops		ufs_vnodeops	/* ufs's vnode operations */
#define osi_vfsops		vfsops
#define	osi_vType(vc)		(vc)->v_type
#define	osi_vSetType(vc,type)   (vc)->v_type = (type)
#define	osi_IsAfsVnode(vc)	((vc)->v_op == (struct osi_vnodeops *) xvfs_ops)
#define	osi_SetAfsVnode(vc)	(vc)->v_op = xvfs_ops
#define osi_SetVnodeOps(vc,op)	((struct vnode *)(vc))->v_op = (struct osi_vnodeops *)(op)
#define OSI_VP_TO_VFSP(VP)	(VP)->v_vfsp
#define OSI_IS_MOUNTED_ON(VP)	((VP)->v_mvfsp != (struct osi_vfs *) 0)
#define OSI_ISFIFO(VP)		((VP)->v_type == VFIFO)
#define OSI_ISVDEV(VP)	((VP)->v_type == VBLK || (VP)->v_type == VCHR || \
			  (VP)->v_type == VMPC || OSI_ISFIFO(VP))
#define OSI_ISDIR(VP)		((VP)->v_type == VDIR)
#define OSI_ISREG(VP)		((VP)->v_type == VREG)
#define OSI_ISLNK(VP)		((VP)->v_type == VLNK)
#define OSI_IS_SWAPVP(vp)	0

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
#define osi_dnlc_fs_purge(vop, level)	0
#define osi_dnlc_iter_purge(func, arg)	0
#define osi_dnlc_purge_vp(vp)
#define osi_dnlc_purge_vfsp(vfs, count)	0
#define osi_dnlc_remove(dvp, fname)

/*
 * Generic calls to vnode kernel routines
 */
#define OSI_VN_INIT(VP, VFSP, TYPE, DEV, EVN)	{ \
	(VP)->v_flag = 0; \
	(VP)->v_count = 1; \
	osi_vInitLocks(VP); \
	osi_vSetVfsp(VP, (VFSP)); \
	osi_vSetType(VP, (TYPE)); \
	(VP)->v_rdev = (DEV); \
}

#define OSI_VN_RELE(VP)		VN_RELE(VP)
#define OSI_VN_HOLD(VP)		VN_HOLD(VP)

/* XXXXXXXXXXXX Watch out osi_lookupname for AIX31!! XXXXXXXXXX */
#define	osi_lookupname(fnamep, segflg, followlink, dirvpp, compvpp) \
    lookupvp(fnamep, followlink, compvpp)
/* We define a special function to do this in osi_port_aix.c */
extern osi_rdwr _TAKES((
	enum uio_rw rw,
	struct vnode *vp,
	caddr_t base,
	int len,
	osi_off_t *offset,
	int segflg,
	int unit,
	int *residp
));

#ifdef KERNEL
extern void osi_vattr_null _TAKES((struct vattr *vap));
#endif
#define osi_vattr_init(vap, mask)	osi_vattr_null(vap);
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

#define osi_setting_mode(vap)		((vap)->va_mode != (mode_t) -1)
#define osi_set_mode_flag(vap)
#define osi_clear_mode_flag(vap)	((vap)->va_mode = (mode_t) -1)

#define osi_setting_uid(vap)		((vap)->va_uid != (uid_t) -1)
#define osi_set_uid_flag(vap)
#define osi_clear_uid_flag(vap)		((vap)->va_uid = (uid_t) -1)

#define osi_setting_gid(vap)		((vap)->va_gid != (gid_t) -1)
#define osi_set_gid_flag(vap)
#define osi_clear_gid_flag(vap)		((vap)->va_gid = (gid_t) -1)

#define osi_setting_uid_or_gid(vap)	((vap)->va_uid != (uid_t) -1 || \
					 (vap)->va_gid != (gid_t) -1)

#define osi_setting_size(vap)		((vap)->va_size != -1)
#define osi_set_size_flag(vap)
#define osi_clear_size_flag(vap)	((vap)->va_size = -1)

#define osi_setting_atime(vap)		((vap)->va_atim != -1)
#define osi_set_atime_flag(vap)
#define osi_clear_atime_flag(vap)	((vap)->va_atim = -1)

#define osi_setting_mtime(vap)		((vap)->va_mtim != -1)
#define osi_set_mtime_flag(vap)
#define osi_clear_mtime_flag(vap)	((vap)->va_mtim = -1)

#define osi_setting_ctime(vap)		((vap)->va_ctim != -1)
#define osi_set_ctime_flag(vap)
#define osi_clear_ctime_flag(vap)	((vap)->va_ctim = -1)

#define osi_setting_link(vap)		((vap)->va_nlink != (short) -1)
#define osi_set_link_flag(vap)
#define osi_clear_link_flag(vap)	((vap)->va_nlink = (short) -1)

#if !defined(KERNEL)
#define	osi_suser(cr)		suser()
#else
extern int osi_aix_suser(void);
#define	osi_suser(cr)		osi_aix_suser()
#endif

#define osi_setuerror(e) setuerror(e)
#define osi_getuerror() getuerror()

#ifdef _KERNEL

#define osi_printf		printf
extern void osi_uprintf(char *fmt, ...);
extern int osi_vprintf(char *fmt, va_list vargs);

#ifdef DFS_NOASSERTS
#define osi_assert(x)  0
#else
#define osi_assert(x) \
    ((x) || osi_Panic(__LINE__, afsl_trace.fileid))
#endif

extern int osi_Panic(int lineno, char *fileid);

/*
 * Default macros for sleep/wakeup calls on osi_WaitHandle structures
 * (these just take pointers, not wait handles -ota 900316)
 */
extern int osi_aix_Wakeup _TAKES((caddr_t x));
extern int osi_aix_Sleep _TAKES((caddr_t x, int flags));

#define osi_Sleep(x)		osi_aix_Sleep((caddr_t)(x), PZERO - 2)
#define osi_SleepInterruptably(x) \
	osi_aix_Sleep((caddr_t)(x), PCATCH | (PZERO + 1))
#define osi_Wakeup(x)		osi_aix_Wakeup((caddr_t)(x))

#define osi_copyin(src, dst, len)	copyin(src, dst, len)
#define osi_copyinstr(src, dst, len, copied) copyinstr(src, dst, len, copied)
#define osi_copyout(src, dst, len)	copyout(src, dst, len)

/* Use kernel-internal memset since it is provided (declaration in string.h) */
#define osi_Memset(s, c, n)	memset((s), (c), (n))

/*
 * Macros for suspending execution for a given number of seconds
 */
#define osi_Pause(seconds)	\
  (void) osi_Wait(((seconds) * 1000), \
	(struct osi_WaitHandle *)NULL, 0 /* =>no interrupts */)

/*
 * Generic process stuff
 */
#define osi_curproc()		(u.u_procp)
/* osi_ThreadID returns a convenient handle for the current thread.
 * osi_ThreadUnique returns an identifier that is unique for "all" time.
 * osi_ProcIsNFS() on some architectures can tell us that this thread
 * is the NFS daemon.
 */
#define osi_ThreadUnique()	osi_ThreadID()
#define osi_ThreadID()		getpid()
#define osi_GetPid()		getpid()
#define osi_ProcIsNFS()		0

/*
 * OSI malloc/free definitions (see osi.h).
 */
#define osi_kalloc_r(size)	malloc(size)
#define osi_kfree_r(ptr,size)	free(ptr)

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
/* should be nsec/1000, but shifting (/1024) is close enough */
#define osi_GetTime(x) \
	do {curtime(x); (x)->tv_usec = (x)->tv_usec >> 10; } while (0)

/*
 * Quick way to read the clock for callers who only care about seconds;
 * on some platforms this returns seconds and nanoseconds.
 */
#define osi_GetTimeForSeconds(x) curtime((struct timestruc_t *)(x))

/*
 * Return address of calling function.
 */
extern caddr_t get_ret_addr(void);
#define osi_caller()	get_ret_addr()

#else /* !_KERNEL */

#define osi_ProcIsNFS()		0
/*
 * osi_GetTime (Get system time) definitions
 */
#define osi_GetTime(x)		gettimeofday(x, (struct timezone *)0)
#define	osi_Time()		time((time_t *)0)
#define osi_gettimeofday(tp, tzp) gettimeofday((tp), (tzp))

/*
 * Return address of calling function, if available.
 */
#define osi_caller()		0

/* some architectures don't support wait3 */
#define osi_wait3(st, opt, ru)	wait3(st, opt, ru)

#define osi_GetMaxNumberFiles() getdtablesize()
#endif /* _KERNEL */

#define VREAD		S_IREAD
#define VEXEC		S_IEXEC
#define VWRITE		S_IWRITE
#define	VSUID		S_ISUID
#define	VSGID		S_ISGID
#define	VSVTX		S_ISVTX
#define	VFS_RDONLY	VFS_READONLY

#define min(a, b)	(((a) < (b)) ? (a) : (b))
#define	bcopyin		copyin		/* Aux has identical bcopyin */

#define BLKDEV_IOSIZE	BSIZE
#define VROOT		V_ROOT
#define VTEXT		V_TEXT
#define	VTOI		VTOIP
#define v_op		v_gnode->gn_ops
#define iunlock(x)      irele(x)
#define	SBSIZE		BSIZE

#define OSI_MAXNAMLEN	MAXNAMLEN
#define OSI_MAXHOSTNAMELEN	MAXHOSTNAMELEN

/* These will be used in osi_kvnode.h outside of the KERNEL */
#define IO_UNIT		0x01
#define IO_APPEND	FAPPEND
#define	IO_SYNC		FSYNC
#define LOOKUP_DIR	0x08

enum vcexcl {NONEXCL, EXCL};	/* (non)exclusive create */

#define	NO_FOLLOW	L_NOFOLLOW
#define	FOLLOW_LINK	0

#define va_nodeid	va_serialno
#define vfs_vnodecovered vfs_mntdover
#define osi_vnodecovered(vp)	(vp)->v_vfsp->vfs_vnodecovered
typedef struct filehandle	osi_fhandle_t;
extern int osi_getfh(char *, osi_fhandle_t *);

#define	osi_hz		HZ

/* IO_APPEND and similar flags */
#define OSI_FAPPEND	IO_APPEND
#define OSI_FEXEC	FEXEC
#define OSI_FSYNC	IO_SYNC

/*
 * Access time fields in vattr structure
 */
#define osi_sec			tv_sec
#define osi_subsec		tv_usec		/* subsecond field of timeval */
typedef struct timeval osi_timeval_t;	/* the one with subsec as a field */
#define osi_UTimeFromSub(a,b)	((a).sec = (b).tv_sec, (a).usec = (b).tv_usec)
#define osi_SubFromUTime(a,b)	((a).tv_sec = (b).sec, (a).tv_usec = (b).usec)
#define osi_TvalFromSub(a,b)	((a) = (b))
#define osi_SubFromTval(a,b)	((a) = (b))
#define osi_ToUTime(a)	((a).tv_usec)
#define osi_ToSubTime(a) ((a).tv_usec)
#define osi_SubUnit	1000000
#define osi_SubIncr	1	/* min increment for version # */

#ifndef _KERNEL
#define osi_ExclusiveLockNoBlock(fid)	lockf((fid), F_TLOCK, 0)
#define osi_UnLock(fid)			lockf((fid), F_ULOCK, 0)
#endif	/* !_KERNEL */

#ifdef KERNEL
#define osi_vptofid(vp, fidp)	(vtognop(vp, vn_fid)(vp, fidp))
#define vtognop(vp, func)	(*(vp)->v_gnode->gn_ops->func)
#define vfstovfsop(vfsp, func)	(*(vfsp)->vfs_ops->func)
#define	VN_HOLD(vp)		vtognop((vp),vn_hold)(vp)
#define	VN_RELE(vp)		vtognop((vp),vn_rele)(vp)
#define xrele(x)

#define isdigit(c)   (((c) >= '0') & ((c) <= '9'))

#ifndef MIN
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

/*
 * Get machine's hostname
 */
extern int osi_aix_GetMachineName _TAKES((char *abuffer, int alen));
#define osi_GetMachineName(buf,len) osi_aix_GetMachineName(buf, len)

#endif /* KERNEL */

/* Exported by osi_{user,kernel}_newProc.c */
/* N.B. the thread name is only useful in the kernel on a RIOS */
extern int osi_NewProc _TAKES((
			       void	(*clientRoutine)(),
			       void *	clientArgs,
			       int	blockPreemptionBeforeClientRoutine,
			       int	restorePreemptionAfterClientRoutine,
			       char	threadName[4]
			     ));

/*
 * On AIX, the dependencies between the kernel extensions prevent us from
 * calling pthread_create().  So we call an equivalent routine in osi.
 */
#define osi_ThreadCreate(start_routine, args, blockPreemption, restorePreemption, threadname, code) \
	code = osi_NewProc(start_routine, args, blockPreemption, restorePreemption, threadname)

#define DEFINE_OSI_UERROR

#ifdef	KERNEL
extern int osi_PreemptionOff(void);
extern void osi_RestorePreemption(int s);
#endif	/* KERNEL */

/*
 * make a declaration of these functions available at any place that might use
 * the above macros
 */
extern int gettimeofday _TAKES((struct timeval *tp, struct timezone *tzp));
extern int bcmp _TAKES(( /* char *ptr1, char *ptr2, int len */ ));
extern void bcopy _TAKES(( /* char *src, char *dest, int len */ ));
extern void bzero _TAKES(( /* char *ptr, int len */ ));

#ifdef KERNEL
extern opaque osi_AllocPinned (size_t asize);
extern void osi_FreePinned(opaque ptr, size_t asize);
extern void osi_Pin(opaque ptr, size_t asize);
extern void osi_Unpin(opaque ptr, size_t asize);
#endif

#endif /* TRANSARC_OSI_PORT_MACH_H */
