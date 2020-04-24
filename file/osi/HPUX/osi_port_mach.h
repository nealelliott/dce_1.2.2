/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/osi/HPUX/RCS/osi_port_mach.h,v 1.44 1996/04/26 18:20:21 ota Exp $ */

/*
 *      Copyright (C) 1996, 1992 Transarc Corporation
 *      All rights reserved.
 */

/* HP800 port information */

#ifndef TRANSARC_OSI_PORT_MACH_H
#define	TRANSARC_OSI_PORT_MACH_H

#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#ifndef	_KERNEL
#include <dirent.h>
#endif

typedef off_t osi_off_t;
typedef int   osi_timeout_t;
typedef struct fid osi_fid_t;
typedef struct osi_handle {
    struct fid fh_fid;	/* the only field we really need */
} osi_fhandle_t;
typedef struct buf osi_buf_t;
typedef struct statfs osi_statfs_t;

/*
 * Although not traditional osi definitions, HP will
 * compile BSD style code better with these definitions.
 * This also helps remove #ifdefs from common code.
 */
/*
 * Our friend utimes does not exist in HP/UX, so we have
 * convert BSD style utimes(char *path, struct timeval *tvp) to
 * HP style utime(char *path, struct utimbuf *timbufp)
 */
extern int utimes(char *path, struct timeval *tvp);

#define ulong u_long
#define ushort u_short

#define osi_statfs_devnum(svfs) ((svfs)->f_fsid[0])
#define osi_statfs_vfstype(svfs) ((svfs)->f_fsid[1])

#ifndef	KERNEL
typedef struct dirent osi_direct_t;
#endif

#define DEFINE_OSI_UERROR

/*
 * define macros for encoding major/minor devices for DFS protocol
 *
 *	The DFS protocol encodes a device with major number J minor number N
 *	in two words.  deviceNumber contains the lower 16 bits of J in its
 *	upper 16 bits and the lower 16 bits of N in the lower 16 bits.  
 *	deviceNumberHighBits contains the upper 16 bits of J in its 
 *	upper 16 bits and the upper 16 bits of N in the lower 16 bits. 
 *
 *	osi_EncodeDeviceNumber(statP, dev) places dev into statP status structure
 *	osi_DecodeDeviceNumber(statP, devP) takes major/minor info from stat
 *		structure and puts it in '*devP'.  This returns 1 is the device
 *		is representable on the local architecture.
 *
 *	In HP/UX 9.0, major numbers are 8 bits and minor numbers are 24.
 */
#define osi_EncodeDeviceNumber(statP, dev)	\
MACRO_BEGIN \
    unsigned int _maj = (unsigned)(major(dev)); \
    unsigned int _min = (unsigned)(minor(dev)); \
    (statP)->deviceNumber = (_maj << 16) | (_min & 0xffff); \
    (statP)->deviceNumberHighBits = (_min >> 16); \
MACRO_END

/* check for more than 8 bits of major and more than 24 bits of minor */
#define osi_DecodeDeviceNumber(statP, devP) \
    ((((statP)->deviceNumberHighBits & 0xffffff00) || \
      ((statP)->deviceNumber & 0xff000000)) ? ((*(devP) = 0), 0) : \
     ((*(devP) = makedev(((statP)->deviceNumber >> 16), \
			((statP)->deviceNumberHighBits << 16) | \
			 ((statP)->deviceNumber & 0xffff))), 1))

/* Define parameters that gives this platform's highest file address: ours is
 * still 0x7fffffff. */

#define OSI_MAX_FILE_PARM_CLIENT	OSI_MAX_FILE_LIMIT(31,0)
#define OSI_MAX_FILE_PARM_SERVER	OSI_MAX_FILE_LIMIT(31,0)

#define	osi_hz			hz
#define osi_sec			tv_sec
#define osi_subsec		tv_usec	/* subsecond field of timeval */
typedef struct timeval osi_timeval_t;	/* the one with subsec as a field */
#define osi_UTimeFromSub(a,b)	((a).sec = (b).tv_sec, (a).usec = (b).tv_usec)
#define osi_SubFromUTime(a,b)	((a).tv_sec = (b).sec, (a).tv_usec = (b).usec)
#define osi_TvalFromSub(a,b)	((a) = (b))
#define osi_SubFromTval(a,b)	((a) = (b))
#define osi_ToUTime(a)		((a).tv_usec)
#define osi_ToSubTime(a)	((a).tv_usec)
#define osi_SubUnit		1000000
#define osi_SubIncr		1	/* min increment for version # */

/*
 * BSD-style timeout()
 */
#define	OSI_BSD_TIMEOUT		1

/*
 * Traditional single-threaded sleep/wakeup synchronization
 */
#define OSI_SINGLE_THREADED	1

#define OSI_MAXNAMLEN		MAXNAMLEN
#define OSI_MAXHOSTNAMELEN	MAXHOSTNAMELEN

/* IO_APPEND and similar flags */
#define OSI_FAPPEND	IO_APPEND
#define OSI_FEXEC	0	/* disables it */
#define OSI_FSYNC	IO_SYNC

/*
 * Macros that deal with vfs/vnode fields
 */
#define osi_vInitLocks(vc)	(vc)->v_shlockc = (vc)->v_exlockc = 0
#define osi_vfs			vfs
#define osi_vfs_op		vfs_op
#define osi_vSetVfsp(vc,vfsp)	(vc)->v_vfsp = (vfsp)
#define osi_vnodeops		vnodeops	/* type for v_op */
#define osi_ufsVnodeops		ufs_vnodeops	/* ufs's vnode operations */
#define osi_vfsops		vfsops
#define	osi_vType(vc)		(vc)->v_type
#define	osi_vSetType(vc,type)   (vc)->v_type = (type)
#define	osi_IsAfsVnode(vc)	((vc)->v_op == (struct osi_vnodeops *) xvfs_ops)
#define	osi_SetAfsVnode(vc)	(vc)->v_op = xvfs_ops
#define osi_SetVnodeOps(vc,op)	((struct vnode *)(vc))->v_op = (struct osi_vnodeops *)(op)

#ifndef	_KERNEL
#define osi_getfh(path, fhp)	getfh(path, fhp)
#endif

#define OSI_VP_TO_VFSP(VP)	(VP)->v_vfsp
#define OSI_IS_MOUNTED_ON(VP)	((VP)->v_vfsmountedhere != (struct osi_vfs *) 0)
#define OSI_ISFIFO(VP)		((VP)->v_type == VFIFO)
#define OSI_ISVDEV(VP)	\
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
#define osi_dnlc_fs_purge(vop, level)   0
#define osi_dnlc_purge_vp(vp)
#define osi_dnlc_purge_vfsp(vfs, count)	0
#define osi_dnlc_remove(dvp, fname)

/*
 * Generic calls to vnode kernel routines
 */
#ifdef _KERNEL
#define OSI_VN_INIT(VP, VFSP, TYPE, DEV, EVN) \
	VN_INIT(VP, VFSP, TYPE, DEV)
#else
#define OSI_VN_INIT(VP, VFSP, TYPE, DEV, EVN)	{ \
	(VP)->v_flag = 0; \
	(VP)->v_count = 1; \
	osi_vInitLocks(VP); \
	osi_vSetVfsp(VP, (VFSP)); \
	osi_vSetType(VP, (TYPE)); \
	(VP)->v_rdev = (DEV); \
}
#endif	/* _KERNEL */

/* HPUX passes two extra arguments to support DUX */
#define	osi_lookupname(pathname, segment, followlink, parentvp, compvp) \
    lookupname(pathname, segment, followlink, parentvp, compvp, \
	LKUP_LOOKUP, NULL)

#define osi_rdwr(rw,vp,base,len,offset,segflg,unit,resid) \
	 vn_rdwr(rw,vp,base,len,offset,segflg,unit,resid,0)

#ifdef _KERNEL
#define osi_vattr_null(vap)		vattr_null(vap)
#endif
#define osi_vattr_init(vap, mask)	osi_vattr_null(vap)
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

#define osi_setting_mode(vap)		((vap)->va_mode != (u_short) -1)
#define osi_set_mode_flag(vap)
#define osi_clear_mode_flag(vap)	((vap)->va_mode = (u_short) -1)

#define osi_setting_uid(vap)	((vap)->va_uid != (ushort) (UID_NO_CHANGE))
#define osi_set_uid_flag(vap)
#define osi_clear_uid_flag(vap)	((vap)->va_uid = (ushort) (UID_NO_CHANGE))

#define osi_setting_gid(vap)	((vap)->va_gid != (ushort) (GID_NO_CHANGE))
#define osi_set_gid_flag(vap)
#define osi_clear_gid_flag(vap)	((vap)->va_gid = (ushort) (GID_NO_CHANGE))

#define osi_setting_uid_or_gid(vap) \
				((vap)->va_uid != (ushort) (UID_NO_CHANGE) || \
				 (vap)->va_gid != (ushort) (GID_NO_CHANGE))

#define osi_setting_size(vap)		((vap)->va_size != (u_long) -1)
#define osi_set_size_flag(vap)
#define osi_clear_size_flag(vap)	((vap)->va_size = (u_long) -1)

#define osi_setting_atime(vap)		((vap)->va_atime.tv_sec != -1)
#define osi_set_atime_flag(vap)
#define osi_clear_atime_flag(vap)	((vap)->va_atime.tv_sec = -1)

#define osi_setting_mtime(vap)		((vap)->va_mtime.tv_sec != -1)
#define osi_set_mtime_flag(vap)
#define osi_clear_mtime_flag(vap)	((vap)->va_mtime.tv_sec = -1)

#define osi_setting_ctime(vap)		((vap)->va_ctime.tv_sec != -1)
#define osi_set_ctime_flag(vap)
#define osi_clear_ctime_flag(vap)	((vap)->va_ctime.tv_sec = -1)

#define osi_setting_link(vap)		((vap)->va_nlink != -1)
#define osi_set_link_flag(vap)
#define osi_clear_link_flag(vap)	((vap)->va_nlink = -1)

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
 * Get machine's hostname
 */
extern int osi_GetMachineName(char *buf, int len);

#define osi_setuerror(e) u.u_error = e
#define osi_getuerror() u.u_error

/*
 * Default macros for sleep/wakeup calls on osi_WaitHandle structures
 * (these just take pointers, not wait handles -ota 900316)
 */
#define	osi_Sleep(x)		sleep((caddr_t) (x), PZERO - 2)
#define osi_SleepInterruptably(x) sleep((caddr_t) (x), PCATCH | (PZERO + 1))
#define	osi_Wakeup(x)		wakeup((caddr_t) (x))

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
#define osi_ThreadUnique()	osi_curproc()
#define osi_ThreadID()		(osi_curproc()->p_pid)
#define osi_GetPid()		(osi_curproc()->p_pid)

#define osi_copyin(src, dst, len)	copyin(src, dst, len)
#define osi_copyinstr(src, dst, len, copied) copyinstr(src, dst, len, copied)
#define osi_copyout(src, dst, len)	copyout(src, dst, len)

/*
 * OSI malloc/free definitions (see osi.h).
 */
#define	osi_kalloc_r(size)	kmalloc(size, M_DYNAMIC, M_WAITOK|M_ALIGN)
#define	osi_kfree_r(ptr,size)	kfree(ptr, M_DYNAMIC)

/*
 * Allocate/deallocate non pageable memory 
 */
#define osi_AllocPinned(asize)  	osi_Alloc(asize)
#define osi_FreePinned(ptr, asize) 	osi_Free(ptr, asize)

/*
 * Prevent paging out of memory
 */
#define osi_Pin(ptr, asize) 	/* NULL */
#define osi_Unpin(ptr, asize)	/* NULL */

/*
 * osi_GetTime (Get system time) definitions
 */
#define	osi_GetTime(x)		uniqtime(x)

/*
 * Quick way to read the clock for callers who only care about seconds;
 * on some platforms this returns seconds and nanoseconds.
 */
#define osi_GetTimeForSeconds(x) uniqtime(x)

/*
 * Awful.  See osi_ThreadCreate() below.
 */
#include <sys/pstat.h>

#define osi_PreemptionOff()		0
#define osi_RestorePreemption(x)
#else /* !_KERNEL */
/*
 * osi_GetTime (Get system time) definitions
 */
#define osi_GetTime(x)		gettimeofday(x, (struct timezone *)0)
#define	osi_Time()		time((time_t *)0)
#define osi_gettimeofday(tp,tzp) gettimeofday((tp),(tzp))

/* some architectures don't support wait3 */
/*
 * HP's prototype for wait3 is bogus (types of actual parameters are
 * different from those of formal parameters).  We temporize here by
 * using casts.
 */
#define osi_wait3(st, opt, ru) 	wait3((int *)(st), opt, (int *)(ru))

extern int osi_GetMaxNumberFiles();

#endif /* _KERNEL */

/*
 * Return address of calling function.
 *
 * Is there a way to get this?
 */
#define osi_caller()	((caddr_t) 0)

#define osi_suser(cr)	suser()

#ifndef _KERNEL
#define osi_ExclusiveLockNoBlock(fid)	lockf((fid), F_TLOCK, 0)
#define osi_UnLock(fid)			lockf((fid), F_ULOCK, 0)
#endif /* !_KERNEL */

extern int osi_vptofid(struct vnode *vp, osi_fid_t *fidp);
extern caddr_t osi_caller _TAKES(( void ));

#define osi_MakeInitChild()

/*
 * This macro uses symbols defined in <dce/ker/pthread.h>.
 */
#ifdef KERNEL
#define osi_ThreadCreate(start_routine, args, blockPreemption, restorePreemption, threadname, code)	\
	{ extern char	*pst_cmds;								\
	  struct proc	*new_proc;								\
	  char		*cp;								\
	  /* for hpux: pthread_create() really returns a proc_t in new_thread! */		\
	  /* threadname: a 4 char, non-terminated char array */					\
	  code = pthread_create((pthread_t *)&new_proc, pthread_attr_default, 			\
				(pthread_startroutine_t)start_routine, args);			\
	  if (code == 0) {									\
	      /* This is awful.  I was never able to get the hpux pstat_xx routines to do	\
	       * what I wanted.  */								\
	      cp = pst_cmds + ((new_proc - proc) * PST_CLEN);					\
	      bzero(cp, PST_CLEN);								\
	      bcopy("[DFS] ", cp, 6);								\
	      bcopy(threadname, cp+6, 4);							\
	      cp[10] = '\0';									\
	      }											\
	  }
#else	/* KERNEL */
#define osi_ThreadCreate(start_routine,args,blockPreemption,restorePreemption,threadname,code) \
	{ pthread_t new_thread; \
	code = pthread_create(&new_thread, pthread_attr_default, \
			      (pthread_startroutine_t)start_routine, args); }
#endif	/* KERNEL */

/*
 * make a declaration of these functions available at any place that might use
 * the above macros
 */
extern int gettimeofday _TAKES((struct timeval *tp, struct timezone *tzp));
extern int bcmp _TAKES(( /* char *ptr1, char *ptr2, int len */ ));
extern void bcopy _TAKES(( /* char *src, char *dest, int len */ ));
extern void bzero _TAKES(( /* char *ptr, int len */ ));
#ifdef	_KERNEL
extern void *osi_Memset _TAKES((void *ptr, int val, size_t len));
#endif

#ifndef KERNEL                          /* some things don't exist in kernel */
extern void *malloc _TAKES((size_t size));
extern void abort _TAKES(( void ));
extern void exit _TAKES(( int status ));
#endif

#endif /* TRANSARC_OSI_PORT_MACH_H */
