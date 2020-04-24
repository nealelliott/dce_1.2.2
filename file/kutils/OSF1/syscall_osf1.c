/* Copyright (C) 1994 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>		/* Should be always first */
#include <dcedfs/sysincludes.h>		/* Standard vendor system headers */
#include <dcedfs/osi.h>
#include <dcedfs/osi_sysconfig.h>
#include <dcedfs/syscall.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/kutils/OSF1/RCS/syscall_osf1.c,v 1.2 1994/09/22 16:13:54 bwl Exp $")

/* 
 * Main entry of all afs system calls 
 */
struct args {
    long syscall;
    long parm1;
    long parm2;
    long parm3;
    long parm4;
    long parm5;
};

afs_syscall (
        struct proc *p,
        void *args,
        int *retval
	)
{
    register struct args *uap = (struct args *)args;

    DEFINE_OSI_UERROR

    int sc = uap->syscall;
    int (*fun)(long, long, long, long, long, int *);

    if (sc < 0 || sc > AFSCALL_LAST) {
	    osi_setuerror(EINVAL);
	    return(EINVAL);
    }

#ifndef	AFS_DYNAMIC
    /*
     * Based on the syscall being made, we call the appropriate
     * configuration routine.  Currently there are three
     * independent config routines, one each for CM, PX, and EPISODE.
     * These config routines will install the correct entry points
     * in the afs syscall table, and install a VFS if required.
     * In the AFS_DYNAMIC case, these configuration routines will
     * have been called when the kernel extensions were loaded.
     */
    if (!config_done[sc]) {
	    if (config_rtn[sc] != NIL)
		(*config_rtn[sc])(SYSCONFIG_CONFIGURE, NULL, 0, NULL, 0);
	    config_done[sc]++;	/* only do this once */
    }
#endif	/* !AFS_DYNAMIC */
    
    /*
     * If the call fails, the return value contains the error code;
     * a 0 return is a successful call.  If a system call actually
     * needs to return something more, it would have to be passed
     * back via retval.
     */
    fun = afs_sysent[sc].afs_call;
    osi_setuerror((*fun)(uap->parm1, uap->parm2,
			 uap->parm3, uap->parm4,
			 uap->parm5, retval));
    return (osi_getuerror());

}
