/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: krpc_helper_mach.c,v $
 * Revision 1.1.58.2  1996/02/18  00:00:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:26  marty]
 *
 * Revision 1.1.58.1  1995/12/08  00:13:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/14  19:55 UTC  maunsell_c
 * 	merge Walnut Creek changes to HPDCE02 mainline
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/3  1995/09/12  13:03 UTC  maunsell_c
 * 	use kthreadp not procp for selwakeup, timeout, untimeout calls.
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/2  1995/08/22  17:49 UTC  maunsell_c
 * 	use krpc_ vs kt_wchan call
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/1  1995/07/13  22:20 UTC  maunsell_c
 * 	walnut creek port
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:33 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * 	HP revision /main/HPDCE01/2  1994/05/03  02:23 UTC  mort
 * 	Rodan: remove unnecessary NET_SPLNET/SOCKET_LOCK calls
 * 
 * 	HP revision /main/mort_10.0_port/3  1994/04/15  18:37 UTC  mort
 * 	Rodan: remove unnecessary NET_SPLNET/SOCKET_LOCK calls
 * 
 * 	HP revision /main/HPDCE01/1  1994/03/31  23:59 UTC  mort
 * 	Changes needed to get dce_krpc.ext to load into IC1
 * 	In 10.0, the macros NETMP_GO_EXCLUSIVE/NETMP_GO_UNEXCLUSIVE no longer
 * 	exist.  They are replaced by NET_SPLNET/SOCKET_LOCK and
 * 	SOCKET_UNLOCK/NET_SPLX, respectively.  SOCKET_{,UN}LOCK is defined in
 * 	<sys/socketvar.h> which includes <net/netmp.h>, which defines
 * 	NET_SPL{NET,X}.  The original code is taken from sys/sys_socket.c for
 * 	the soo_select() function.
 * 
 * 	HP revision /main/mort_10.0-port/2  1994/03/31  20:05 UTC  mort
 * 	conditionally include net/netmp.h
 * 
 * 	HP revision /main/mort_10.0-port/1  1994/03/30  21:51 UTC  mort
 * 	merge hpux sys/sys_socket.c changes
 * 	[1995/12/07  23:55:41  root]
 * 
 * Revision 1.1.56.2  1994/06/10  20:54:07  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:53  devsrc]
 * 
 * Revision 1.1.56.1  1994/01/21  22:31:22  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:33:00  cbrooks]
 * 
 * Revision 1.1.4.2  1993/06/10  19:22:44  sommerfeld
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:03:54  kissel]
 * 
 * 	Initial revision.
 * 	[1993/01/15  21:16:53  toml]
 * 
 * $EndLog$
 */

#include <commonp.h>
#include <com.h>

#include <krpc_helper.h>
#include <krpc_helper_data.h>

#include <sys/systm.h>
#include <sys/file.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <dce/ker/krpc_osi_mach.h>
#if ! __hpux1000p
#include <net/netmp.h>
#include <sys/socketvar.h>
#endif /* __hpux1000p */

extern struct krpc_helper *pending;

/*
 * Platform dependent routines for the krpc_helper module
 * live here
 */

int krpchdd_open
#ifdef _DCE_PROTO_
(
    dev_t dev
)
#else
(dev)
dev_t dev;
#endif
{
    return(krpch_open());
}


int krpchdd_close
#ifdef _DCE_PROTO_
(
    dev_t dev
)
#else
(dev)
dev_t dev;
#endif
{
    krpch_close();
    return (0);
}

int krpchdd_ioctl
#ifdef _DCE_PROTO_
(
    dev_t   dev,
    int     cmd,
    caddr_t data,
    int	flag
)
#else
(dev, cmd, data, flag)
dev_t   dev;
int     cmd;
caddr_t data;
int	flag;
#endif
{
    return (krpch_ioctl(cmd, data));
}

int krpchdd_read
#ifdef _DCE_PROTO_
(
    dev_t dev,
    struct uio *uio
)
#else
(dev, uio, flag)
dev_t dev;
struct uio *uio;
#endif
{
    return(krpch_read(uio));
}

int krpchdd_write
#ifdef _DCE_PROTO_
(
    dev_t dev,
    struct uio *uio
)
#else
(dev, uio)
dev_t dev;
struct uio *uio;
#endif
{
    return(krpch_write(uio));
}
#if __hpux1010p
struct kthread *krpc_thread;
#else
struct proc *krpc_proc;
#endif
int krpc_flags;
#define KRPC_COLL 1

int krpchdd_select
#ifdef _DCE_PROTO_
(
	dev_t fp,
    	int which
)
#else
(fp, which)
	dev_t fp;
	int which;
#endif
{
	int oldlevel;
	int ret = 0;

#if __hpux1000p
	/*
	 * N.B. We don't currently use fp.  There is just one caller,
	 * who holds a lock on this call.  The kernel isn't multithreaded,
	 * and only one process runs this code at a time.  Therefore, we
	 * don't need any locking.
	 */
#else /* not __hpux1000p */
	sv_sema_t savestate;		/* MPNET: MP save state */
	/*MPNET: turn MP protection on */
	NETMP_GO_EXCLUSIVE(oldlevel, savestate);
#endif /* __hpux1000p */

	switch (which) {
	case FREAD:
		if (!QUEUE_EMPTY(pending)) {
			ret++;
		} else {
#if __hpux1010p
                        struct kthread *k;
                        if ((k = krpc_thread) && krpc_osi_get_wchan(k) == (caddr_t)&selwait) {
                                krpc_flags |= KRPC_COLL;
                        }
                        else {
                                krpc_thread = u.u_kthreadp;
                        }
#else
			struct proc *p;
                        if ((p = krpc_proc) && krpc_osi_get_wchan(p) == (caddr_t)&selwait)
				krpc_flags |= KRPC_COLL;
			else
				krpc_proc = u.u_procp;
#endif
		}
		break;
	case FWRITE:
		ret++;
		break;
	case 0: /* Exception case on UX */
		break;        
	}

#if __hpux1000p
	/* 
	 * See note on locking above.
	 */
#else /* not __hpux1000p */
	NETMP_GO_UNEXCLUSIVE(oldlevel, savestate);
	/*MPNET: MP protection is now off. */
#endif /* __hpux1000p */
	return (ret);
}

/*
 * Wakeup the helper process waiting on a KRPC channel. 
 */
void krpc_wakeup(void)
{
#if __hpux1010p
        if (krpc_thread) {
                selwakeup(krpc_thread, krpc_flags & KRPC_COLL);
                krpc_thread = 0;
#else
        if (krpc_proc) {
		selwakeup(krpc_proc, krpc_flags & KRPC_COLL);
                krpc_proc = 0;
#endif
		krpc_flags &= ~KRPC_COLL;
	}

}


#ifdef USE_FILEOPS

int init_file_descrip
#ifdef _DCE_PROTO_
(
        int *fd
)
#else
(fd)
    int *fd;
#endif
{
    int indx, error=0;
    struct file *fp;

    if (error = falloc(&fp, &indx))
        return(error);
    ASSERT(u.u_ofile[indx] == U_FD_RESERVED);
    ASSERT(fp->f_count == 1 && fp->f_type == DTYPE_RESERVED);
    FP_LOCK(fp);
    fp->f_flag = FREAD|FWRITE;
    fp->f_type = DTYPE_DFS_SPECIAL;
    fp->f_ops = &krpc_helper_fops;
    fp->f_data = (caddr_t) NULL;
    FP_UNLOCK(fp);
    U_FD_SET(indx, fp, &u.u_file_state);
    *fd = indx;
    return(0);
}


#endif /* FILE_OPS */

