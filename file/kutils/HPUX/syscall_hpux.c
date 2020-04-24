/* Copyright (c) 1994 Transarc Corp. - All Rights Reserved */

#include <dcedfs/param.h>		/* Should always be first */
#include <dcedfs/osi.h>
#include <dcedfs/syscall.h>
#include <dcedfs/osi_sysconfig.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/kutils/HPUX/RCS/syscall_hpux.c,v 1.3 1994/09/22 16:45:46 bwl Exp $")

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

afs_syscall ()
{
    register struct args *uap = (struct args *)u.u_ap;

    int *retval = &u.u_r.r_val1;

    DEFINE_OSI_UERROR

    int sc = uap->syscall;
    int (*fun)(long, long, long, long, long, int *);

    if (sc < 0 || (sc > AFSCALL_LAST && sc != 31)) {
	    osi_setuerror(EINVAL);
	    return(EINVAL);
    }

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

/*
 * Call into kernel to do dynamic load stuff.
 */
int
afscall_kload(p1, p2, p3, p4, p5, retval)
int p1,p2,p3,p4,p5;
int *retval;
{

   if (p1<0 || p1>KLOAD_LAST)
     return EINVAL;
   else
     return (*(kload_sysent[p1].kload_call))(p2, p3, p4, p5, retval);

}
