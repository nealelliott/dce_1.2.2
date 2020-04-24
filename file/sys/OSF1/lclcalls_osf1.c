/* Copyright (c) 1994 Transarc Corp. - All rights reserved */

#include <dcedfs/param.h>
#include <signal.h>
#include <errno.h>
#include <dcedfs/syscall.h>
#include <dce/exc_handling.h>

/*
 * Main afs syscall entry point.
 */
afs_syscall(asyscall, parm1, parm2, parm3, parm4, parm5)
    int asyscall, parm1, parm2, parm3, parm4, parm5;
{
    volatile int errcode = -1;

    static first_call_done = 0;
    static syscall_exists = 0;

    if (syscall_exists) {
        errcode = syscall(AFS_SYSCALL, asyscall, parm1, parm2, 
					parm3, parm4, parm5);
    } else if (!first_call_done) {
        TRY
           errcode = syscall(AFS_SYSCALL, asyscall, parm1, parm2, 
        					parm3, parm4, parm5);
           syscall_exists++;
        CATCH (exc_SIGSYS_e)

        ENDTRY
        /* Proper location for first_call_done setting */
        first_call_done++;
    } else 
        errno = ENOSYS;

    return errcode;
}
