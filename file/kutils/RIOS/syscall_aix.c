/* Copyright (C) 1994 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>		/* Should be always first */
#include <dcedfs/sysincludes.h>		/* Standard vendor system headers */
#include <dcedfs/osi.h>
#include <dcedfs/osi_sysconfig.h>
#include <dcedfs/syscall.h>
#include <sys/lockl.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/kutils/RIOS/RCS/syscall_aix.c,v 1.2 1994/09/22 16:13:54 bwl Exp $")

/*
 * kafs_syscall - the AIX 3 system call entry point.
 */
kafs_syscall(syscall, parm1, parm2, parm3, parm4, parm5)
    int syscall, parm1, parm2, parm3, parm4, parm5;
{
    register rval1;
    register monster;
    int		retval;
    extern lock_t kernel_lock;

    monster = lockl(&kernel_lock, LOCK_SHORT);
    if (syscall < 0 || syscall > AFSCALL_LAST) {
	printf("afs_syscall op = %d\n", syscall);
	osi_setuerror(EINVAL);
	rval1 = -1;
	goto out;
    }

    /*
     * We follow standard system call returns: if the call fails 
     * (i.e. returns -1) *u.u_error (i.e. errno) contains the error code; 
     * 0 return is a successful call. If a system call is to actually 
     * return something more it should be stored in "u.u_rval1" 
     */
    retval = 0;
    rval1 = (*(afs_sysent[syscall].afs_call))(parm1, parm2, parm3, parm4, 
					      parm5, &retval);
    osi_setuerror(0);		/* may have terminated fine */
    if (rval1) {
      osi_setuerror(rval1);
      rval1 = -1;
    }
    else {
      rval1 = retval;
    }
    
out:
    if (monster != LOCK_NEST)
	unlockl(&kernel_lock);
    return rval1;
}
