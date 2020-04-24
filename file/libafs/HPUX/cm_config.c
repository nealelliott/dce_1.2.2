/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/* Copyright (C) 1996 Transarc Corporation - All rights reserved. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>			/* Standard vendor system headers */
#include <dcedfs/osi_sysconfig.h>
#include <sys/user.h>
#include <sys/mount.h>
#include <sys/systm.h>
#include <sys/syscall.h>
#include <dcedfs/syscall.h>

#include <dcedfs/cm_trace.h>
#include <dcedfs/icl.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/libafs/HPUX/RCS/cm_config.c,v 1.8 1996/04/19 16:40:00 ota Exp $")

/*
 * Declare all the AFS system calls we'll need to hook up.
 */

extern int afscall_cm(), afscall_cm_pioctl(), afscall_setpag(),
	   afs_nosys(), afscall_getpag(), afscall_cm_newtgt(),
           afscall_resetpag(), at_translate_creds();

  /*
   * Declare all the system calls we'll need to hook up.
   */
extern int
      afs_xsetgroups(), setgroups();

/*
 * Structure of the system-entry table
 */
extern int nsysent;

extern struct vfsops afs_vfsops;

struct ucred *osi_credp;

extern int (*xdfs_vn_rele_op)();
extern int (*xglue_cm_dcache_delete_op)();
extern int xglue_vn_rele();

/*
 * Function pointer provided for alien file systems to
 * translate credentials based on user/network ID.
 *
 * storage for this function pointer is provided in hp-ux, nfs_server.c
 *
 * Why is this here?  It doesn't appear to be used.
 */
/* should be int not void, dfs_translate_creds is in nfs_server.c for HPUX */
/* and is init here with at_translate_creds in cm_post_config */
extern int (*dfs_translate_creds)();

/*
 * cm_dcache_delete - Remove a vnode from the cm dnamecache.
 *
 * The DFS cm dnamecache is a cache of tuples <dir, file>
 * that speeds up lookups.  We clean this cache by removing
 * tuples whose directory vnode corresponds to the vnode we
 * are flushing.
 */
int cm_dcache_delete(vp)
register struct vnode *vp;
{
      if (vp->v_type != VDIR) {
              return(EINVAL);
      } else {
              return(nh_delete_dvp(vp));
      }
}

/*
 * cm_post_config -- Initialization postponed until CM is ready to roll.
 */
int
cm_post_config()
{
      xdfs_vn_rele_op = xglue_vn_rele;
      xglue_cm_dcache_delete_op = cm_dcache_delete;

         /* link cred translator mechanism */
      dfs_translate_creds = at_translate_creds;
      at_configure();

      return 0;
}

int
cm_configure(op)
int op;
{
	int	ret;

	switch (op) {
		case KLOAD_INIT_CONFIG:
#ifndef	AFS_HPUX80_ENV
			if ((ret = register_vfs_type("hp-dfs")) == -1)
			        return(ret);
#endif	/* AFS_HPUX80_ENV */
			vfssw_assign(ret, afs_vfsops);
			break;

		case KLOAD_INIT_UNCONFIG:
			return EBUSY;
		default:
			return EINVAL;
	}
	set_afssyscalls(op);

	osi_init();

	return 0;
}


set_afssyscalls(op)
	int op;
{

#define SystemCall(index,proc) (((index) < nsysent) ? (sysent[(index)].sy_call = (proc)) : 0)
#define afson (op == KLOAD_INIT_CONFIG)

	SystemCall (SYS_setgroups,   afson ? afs_xsetgroups : setgroups);

	AfsCall (AFSCALL_CM,            afson ? afscall_cm        : afs_nosys);
	AfsCall (AFSCALL_PIOCTL,        afson ? afscall_cm_pioctl : afs_nosys);
	AfsCall (AFSCALL_SETPAG,        afson ? afscall_setpag : afs_nosys);
	AfsCall (AFSCALL_GETPAG,        afson ? afscall_getpag : afs_nosys);
	AfsCall (AFSCALL_RESETPAG,      afson ? afscall_resetpag : afs_nosys);
        AfsCall (AFSCALL_NEWTGT,        afson ? afscall_cm_newtgt : afs_nosys);

	return 0;

}

dfs_client_config(op)
int op;
{
	return(cm_configure(op));
}
