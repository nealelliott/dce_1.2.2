/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>		/* Standard vendor system headers */
#include <dce_afs.h>
#include <sys/sysconfig.h>

#include <sys/mount.h>
#include <sys/errno.h>
#include <sys/systm.h>
#include <sys/syscall.h>
#include <dcedfs/syscall.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/libafs/OSF1/RCS/cm_config.c,v 1.15 1996/04/19 16:40:01 ota Exp $")

/*
 * Declare all the AFS system calls we'll need to hook up.
 */

extern int afscall_cm(), afscall_cm_pioctl(), afscall_setpag(),
	   afs_nosys(), afscall_getpag(), afscall_cm_newtgt();

/*
 * Structure of the system-entry table
 */
extern int nsysent;

extern struct vfsops afs_vfsops;

int
cm_configure(op, indata, indatalen, outdata, outdatalen)
	sysconfig_op_t		op;
#ifndef AFS_OSF11_ENV
	filesys_config_t *	indata;
#else
	char *			indata;
#endif /* AFS_OSF11_ENV */
	size_t			indatalen;
#ifndef AFS_OSF11_ENV
	filesys_config_t *	outdata;
#else
	char *			outdata;
#endif /* AFS_OSF11_ENV */
	size_t			outdatalen;
{
	int             ret = 0;
#ifdef AFS_OSF11_ENV
	static config_attr_t cm_attr[] = {
	{SUBSYS_NAME, STRTYPE, (caddr_t)"cm", CONSTANT, {0, NULL}},
	};
#define NUM_CM_ATTRS sizeof(cm_attr)/sizeof(config_attr_t)
	int             size;
	struct subsystem_info info;
	short           configured;

        strcpy(info.subsystem_name, "cm");
        configured = (!subsys_reg(SUBSYS_GET_INFO, &info) && info.config_flag);
        if ((configured && (op == SYSCONFIG_CONFIGURE)) ||
            (!configured && (op != SYSCONFIG_CONFIGURE)))
            return(EALREADY);
#endif /* AFS_OSF11_ENV */

	switch (op) {
		case SYSCONFIG_CONFIGURE:
			if ((ret = vfssw_add(MOUNT_AFS, &afs_vfsops)) != 0)
				return(ret);
			break;
		case SYSCONFIG_UNCONFIGURE:
			if ((ret = cm_unconfig()) != 0)
				return(ret);
			break;
#ifdef AFS_OSF11_ENV
                case SYSCONFIG_QUERYSIZE:
                        if (outdatalen >= sizeof(int)) {
                                *(int *)outdata = do_querysize(cm_attr,NUM_CM_ATTRS);
                                ret = 0;
                        } else {
                                ret = ENOMEM;
                        }
                        break;

                case SYSCONFIG_QUERY:
                        size = do_querysize(cm_attr,NUM_CM_ATTRS);
                        if (outdatalen < size)
                                ret = ENOMEM;
                        ret = do_query(cm_attr,NUM_CM_ATTRS,outdata,outdatalen
);
                        break;

		case SYSCONFIG_RECONFIGURE:
			return EINVAL;
			break;
#endif /* AFS_OSF11_ENV */
		default:
			return EINVAL;
			break;
	}
	set_afssyscalls(op);
#ifndef AFS_OSF11_ENV
	if (outdata != NULL && outdatalen == sizeof(filesys_config_t)) {
		outdata->fc_version = OSF_FILESYS_CONFIG_10;
		outdata->fc_type = MOUNT_AFS;
	}
#endif /* AFS_OSF_ENV */

	osi_init();

	return 0;
}


cm_unconfig()
{
	int error;

	return(EBUSY);
}


set_afssyscalls(op)
	int op;
{

#define SystemCall(index,proc) (((index) < nsysent) ? (sysent[(index)].sy_call = (proc)) : 0)
#define afson (op == SYSCONFIG_CONFIGURE)

	AfsCall (AFSCALL_CM,            afson ? afscall_cm        : afs_nosys);
	AfsCall (AFSCALL_PIOCTL,        afson ? afscall_cm_pioctl : afs_nosys);
	AfsCall (AFSCALL_SETPAG,        afson ? afscall_setpag : afs_nosys);
	AfsCall (AFSCALL_GETPAG,        afson ? afscall_getpag : afs_nosys);
	AfsCall (AFSCALL_NEWTGT,        afson ? afscall_cm_newtgt : afs_nosys);
	return 0;

}
