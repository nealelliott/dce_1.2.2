/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>		/* Should be always first */
#include <dcedfs/sysincludes.h>		/* Standard vendor system headers */
#include <dcedfs/osi.h>
#include <dcedfs/osi_sysconfig.h>
#include <dcedfs/syscall.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/kutils/RCS/syscall.c,v 4.69 1996/02/12 15:13:58 cfe Exp $")

/* for possible CM/PX communication. */
unsigned int afscall_timeSynchDistance = 0x7ffffff1;
unsigned int afscall_timeSynchDispersion = 0;

int afs_nosys(long, long, long, long, long, int *);
extern int afscall_vnode_ops(), afscall_plumber(), afscall_tkm_control();
extern int afscall_icl(), afscall_newtgt(), afscall_bomb();
#if	defined(AFS_OSF_ENV) || defined(AFS_HPUX_ENV)
extern int afscall_krpcdbg();
#endif
#ifdef	AFS_HPUX_ENV
extern int afscall_kload();
#endif
#ifdef	KTC_SYSCALL
extern int afscall_ktc();
#endif
#ifdef	TESTING_FP_SYSCALL
extern int afscall_fpkint();
#endif
#ifdef	TESTING_XCRED_SYSCALL
extern int afscall_xcredkint();
#endif

/*
 * AFS syscall table.
 *
 * The syscall table is initialized dynamically by 'configure'
 * routines - there are three: cm_configure, px_configure, and epi_configure.
 * cm and px are responsible for hooking their respective VFS file systems into
 * the system VFS switch table. In addition, cm, px, and epi are responsible
 * for installing their respective system call vectors into this system call
 * table below. In the AFS_DYNAMIC case, the configure routines are called at
 * the time the kernel extensions are loaded.
 *
 * In the non AFS_DYNAMIC case, afs_syscall() calls the appropriate configure
 * routine before the first call to the system call, installing it in the
 * system call table.
 */
struct afs_sysent afs_sysent[] = {
/*
 *  Entry Point          Config    Syscall            Real Entry Point
 *                       Routine   Number
 */
    afs_nosys,		/*  CM   AFSCALL_CM         : afscall_cm           */
    afs_nosys,		/*  CM   AFSCALL_PIOCTL     : afscall_cm_pioctl    */
    afs_nosys,		/*  CM   AFSCALL_SETPAG     : afscall_setpag    */
    afs_nosys,		/*  PX   AFSCALL_PX         : afscall_exporter     */
    afs_nosys,		/*  PX   AFSCALL_VOLSER     : afscall_volser       */
    afs_nosys,		/*  PX   AFSCALL_AGGR       : afscall_aggr         */
    afs_nosys,		/*  EPI  AFSCALL_EPISODE    : afscall_episode      */
    afs_nosys,		/*  OBS  AFSCALL_KTC        : afscall_ktc   [obs.] */
    afs_nosys,          /*  OBS  TESTING_FP_SYSCALL : afscall_fpkint[obs.] */
    afs_nosys,          /*  OBS  TESTING_XCRED_SYSCALL: afscall_xcredkint [obs.] */
    afs_nosys,		/*  ALL  AFSCALL_VNODE_OPS  : afscall_vnode_ops     */
    afs_nosys,		/*  CM   AFSCALL_GETPAG     : afscall_getpag     */
    afs_nosys,		/*  ALL  AFSCALL_PLUMBER    : afscall_plumber     : osi mem leak support */
    afs_nosys,		/*  ALL  AFSCALL_TKM        : afscall_tkm_control : sys admin hook to TKM*/
    afs_nosys,		/*  ALL  AFSCALL_ICL        : afscall_icl : deal with in core log */
    afs_nosys,          /*  ALL  AFSCALL_KRPCDBG    : afscall_krpcdbg : krpc debug hooks */
    afs_nosys,		/*  CM   AFSCALL_NEWTGT     : afscall_cm_newtgt */
    afs_nosys,		/*  ALL  AFSCALL_BOMB       : afscall_bomb */
    afs_nosys,		/*  ALL	 AFSCALL_KLOAD	    : afscall_kload (HP/UX) */
    afs_nosys,		/*  AT   AFSCALL_AT         : afscall_at */
    afs_nosys,		/*  CM   AFSCALL_RESETPAG   : afscall_resetpag */
#ifdef	AFS_HPUX_ENV
/* Add enough dummies to make the table at least 31 entries long. */
    afs_nosys,
    afs_nosys,
    afs_nosys,
    afs_nosys,
    afs_nosys,
    afs_nosys,
    afs_nosys,
    afs_nosys,
    afs_nosys,
    afs_nosys,
    afs_nosys,
    afs_nosys,
    afs_nosys,
    afs_nosys
#endif	/* AFS_HPUX_ENV */
};

#if	!defined(AFS_DYNAMIC) || defined(AFS_HPUX_ENV)

/*
 * For AFS_DYNAMIC, we assume the configure routines have already been called
 * at kernel extension load time.
 * For non AFS_DYNAMIC, we need afs_syscall() to be self-configuring.
 * The first time it is called with a certain AFS syscall number,
 * if calls the corresponding configuration routine in config_rtn[].
 */
extern int cm_configure(), epi_configure(), px_configure();
/*
 * Install the afs syscalls which are common to any configuration.
 */
int all_configure()
{
    AfsCall(AFSCALL_VNODE_OPS, afscall_vnode_ops);
    AfsCall(AFSCALL_PLUMBER, afscall_plumber);
    AfsCall(AFSCALL_TKM, afscall_tkm_control);
    AfsCall(AFSCALL_ICL, afscall_icl);
#if	defined(AFS_OSF_ENV) || defined(AFS_HPUX_ENV)
    AfsCall(AFSCALL_KRPCDBG, afscall_krpcdbg);
#endif
#ifdef	AFS_HPUX_ENV
    AfsCall(AFSCALL_KLOAD, afscall_kload);
/*
 * XXX 1.0.2 HP port defined AFSCALL_KLOAD as 31.  For now, we must link with
 * XXX files that make calls to afs_syscall and pass 31 to it.  So we must be
 * XXX able to cope with 31.
 */
    AfsCall(31, afscall_kload);
#endif
    AfsCall(AFSCALL_BOMB, afscall_bomb);
    return 0;
}

#endif

#ifndef	AFS_DYNAMIC
/*
 * Configure routines for each system call.
 * N.B. This table is modified at runtime.
 */
CFG_RTN config_rtn[AFSCALL_LAST + 1] = {
	    cm_configure,	/* AFSCALL_CM       */
	    cm_configure,	/* AFSCALL_PIOCTL   */
	    cm_configure,	/* AFSCALL_SETPAG   */
	    px_configure,	/* AFSCALL_PX       */
	    px_configure,	/* AFSCALL_VOLSER   */
	    px_configure,	/* AFSCALL_AGGR     */
	    epi_configure,	/* AFSCALL_EPISODE  */
	    0,
	    0,
	    0,
	    all_configure,	/* AFSCALL_VNODE_OPS */
	    cm_configure,	/* AFSCALL_GETPAG    */
	    all_configure,	/* AFSCALL_PLUMBER   */
	    all_configure,	/* AFSCALL_TKM       */
	    all_configure,	/* AFSCALL_ICL       */
            all_configure,      /* AFSCALL_KRPCDBG   */
	    cm_configure,       /* AFSCALL_NEWTGT    */
	    all_configure,	/* AFSCALL_BOMB	     */
	    all_configure,	/* AFSCALL_KLOAD     */
	    at_configure,	/* AFSCALL_AT        */
	    cm_configure        /* AFSCALL_RESETPAG  */
};

short config_done[AFSCALL_LAST + 1];

#endif	/* !AFS_DYNAMIC */

int afs_nosys(long parm1, long parm2, long parm3, long parm4, long parm5,
	      int *retvalP)
{

    DEFINE_OSI_UERROR;

    *retvalP = 0;	/* no special info to return */
    osi_setuerror(ENOSYS);
    return (osi_getuerror());
}

void afs_set_syscall(
    int entry,
    int (*call)(long, long, long, long, long, int *))
{
    afs_sysent[entry].afs_call = call;
}
