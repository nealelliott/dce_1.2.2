/* Copyright (c) 1994 Transarc Corp. - All rights reserved */

#include <dcedfs/param.h>
#include <signal.h>
#include <errno.h>
#include <dcedfs/syscall.h>

/*
 * Main afs syscall entry point.
 */
afs_syscall(asyscall, parm1, parm2, parm3, parm4, parm5)
    int asyscall, parm1, parm2, parm3, parm4, parm5;
{
    volatile int errcode = -1;

    extern int afs_syscall_installed();
    static first_call_done = 0;
    static syscall_exists = 0;

    /*
     * Check to see if the system call is installed.
     * If so call it, otherwise return error
     */

    if (syscall_exists) {
        errcode = kafs_syscall(asyscall, parm1, parm2, parm3, parm4, parm5);
    } else if (!first_call_done) {
	first_call_done++;
	if (syscall_exists = afs_syscall_installed()) {
            errcode = kafs_syscall(asyscall, parm1, parm2, parm3, 
				parm4, parm5);
	} else {
	    errno = ENOSYS;
	}
    } else {
	errno = ENOSYS;
    }

    return errcode;
}
