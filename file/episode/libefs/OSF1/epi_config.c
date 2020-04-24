/*
 ** @OSF_COPYRIGHT@
 ** COPYRIGHT NOTICE
 ** Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 ** ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 ** the full copyright text.
 */
/*
 * HISTORY
 * $Log: epi_config.c,v $
 * Revision 1.1.16.1  1996/10/02  17:24:22  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:32:02  damon]
 *
 * Revision 1.1.11.1  1994/06/09  14:00:31  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:46  annie]
 * 
 * Revision 1.1.7.5  1993/01/19  15:26:46  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:47:00  cjd]
 * 
 * Revision 1.1.7.4  1992/11/24  16:24:58  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:23:00  bolinger]
 * 
 * Revision 1.1.7.3  1992/11/18  19:00:46  jaffe
 * 	Transarc delta: rajesh-debug-efts-create-bad-syscall-error-on-OSF1-sefsvmunix 1.1
 * 	  Selected comments:
 * 	    Corresponds to OT# 5796.
 * 	    efts create fails with EINVAL and causes "Bad dfs call" console message.
 * 	    Fix the problem.
 * 	    On OSF/1, the problem is that even though the libvolume.a is being
 * 	    included in sefsvmunix, a pointer to the handler function
 * 	    afscall_volser() is never placed in the afs_sysent table by epiinit.
 * 	    Changed setup() which is called from epi_configure() which is called
 * 	    by the kernel as a result of executing epiinit to actually call
 * 	    AfsCall to install afscall_volser() in the afs_sysent table.
 * 	[1992/11/17  20:25:55  jaffe]
 * 
 * Revision 1.1.7.2  1992/10/13  17:23:38  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:10:50  marty]
 * 
 * Revision 1.1.5.2  1992/09/29  22:46:20  marty
 * 	OSC1.1 upgrade
 * 	[1992/09/29  22:38:35  marty]
 * 
 * Revision 1.1.3.2  1992/09/29  20:06:45  garyf
 * 	add OSF/1 R1.1 version of configuration
 * 
 * Revision 1.1.2.4  1992/06/15  14:50:19  garyf
 * 	cleanup unconfigure (still dummy)
 * 	must install aggregate ops for episode
 * 	[1992/06/15  14:48:16  garyf]
 * 
 * Revision 1.1.2.3  1992/05/05  19:07:26  jdp
 * 	delta jdp-epimerge-add-dynamic-configuration 1.1
 * 	[1992/05/04  21:41:42  jdp]
 * 
 * Revision 1.1.2.2  1992/04/02  17:58:14  garyf
 * 	initial version
 * 	[1992/04/02  17:56:24  garyf]
 * 
 * $EndLog$
 */
/*
 * Routines for dynamically loading and unloading Episode.
 */

#include <dce_afs.h>
#include <dcedfs/param.h>		/* Should be always first */
#include <dcedfs/syscall.h>
#include <sys/sysconfig.h>
#include <sys/mount.h>
#include <sys/errno.h>
#include <dcedfs/aggr.h>

extern struct vfsops efs_vfsops;
extern struct aggrops ag_efsops;
extern struct aggrops *agOpvec[];

extern int afscall_episode(), afs_nosys(), afscall_aggr(), afscall_volser();


int
epi_configure(op, indata, indatalen, outdata, outdatalen)
	sysconfig_op_t		op;
#ifndef AFS_OSF11_ENV
	filesys_config_t *	indata;
#else
	char	*		indata;
#endif /* AFS_OSF11_ENV */
	size_t	 		indatalen;
#ifndef AFS_OSF11_ENV
	filesys_config_t *	outdata;
#else
	char	*		outdata;
#endif /* AFS_OSF11_ENV */
        size_t          	outdatalen;
{
        int             ret = 0;
#ifdef AFS_OSF11_ENV
        static config_attr_t efs_attr[] = {
        {SUBSYS_NAME, STRTYPE, (caddr_t)"efs", CONSTANT, {0, NULL}},
        };
#define NUM_EFS_ATTRS sizeof(efs_attr)/sizeof(config_attr_t)
        int             size;
        struct subsystem_info info;
        short           configured;

        strcpy(info.subsystem_name, "efs");
        configured = (!subsys_reg(SUBSYS_GET_INFO, &info) && info.config_flag);
        if ((configured && (op == SYSCONFIG_CONFIGURE)) ||
            (!configured && (op != SYSCONFIG_CONFIGURE)))
            return(EALREADY);
#endif /* AFS_OSF11_ENV */

	switch (op) {
		case SYSCONFIG_CONFIGURE:
			if ((ret = vfssw_add(MOUNT_EFS, &efs_vfsops)) != 0)
				return(ret);
			setup(op);
			break;
		case SYSCONFIG_UNCONFIGURE:
			if ((ret = efs_unconfig()) != 0)
				return(ret);
			break;
#ifdef AFS_OSF11_ENV
                case SYSCONFIG_QUERYSIZE:
                        if (outdatalen >= sizeof(int)) {
                                *(int *)outdata = do_querysize(efs_attr,NUM_EFS_ATTRS);
                                ret = 0;
                        } else {
                                ret = ENOMEM;
                        }
                        break;

                case SYSCONFIG_QUERY:
                        size = do_querysize(efs_attr,NUM_EFS_ATTRS);
                        if (outdatalen < size)
                                ret = ENOMEM;
                        ret = do_query(efs_attr,NUM_EFS_ATTRS,outdata,outdatalen);
                        break;

                case SYSCONFIG_RECONFIGURE:
                        ret = EINVAL;
                        break;
#endif /* AFS_OSF11_ENV */

       		default:
			return EINVAL;
			break;
	}
#ifndef AFS_OSF11_ENV
        if (outdata != NULL && outdatalen == sizeof(filesys_config_t)) {
                outdata->fc_version = OSF_FILESYS_CONFIG_10;
                outdata->fc_type = MOUNT_EFS;
        }
#endif /* AFS_OSF11_ENV */
	return 0;
}

setup(op)
{
#define afson (op == SYSCONFIG_CONFIGURE)

	if (op == SYSCONFIG_CONFIGURE) {

		agOpvec[AG_TYPE_EPI] = &ag_efsops;

	}
	AfsCall (AFSCALL_EPISODE, afson ? afscall_episode: 	afs_nosys);
	AfsCall (AFSCALL_AGGR,    afson ? afscall_aggr: 	afs_nosys);
	AfsCall(AFSCALL_VOL,      afson ? afscall_volser :      afs_nosys);

	return 0;

}

efs_unconfig()
{
	return(EBUSY);
}
