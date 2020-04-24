/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems
 * Burlington, MA, USA
 * All Rights Reserved
 * (c) Copyright 1990, 1991, 1992
 */
/*
 * HISTORY
 * $Log: cma_pyrmips.c,v $
 * Revision 1.1.8.2  1996/02/17  23:33:18  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:33  marty]
 *
 * Revision 1.1.8.1  1995/12/07  21:52:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:02:01  root]
 * 
 * Revision 1.1.6.1  1994/06/09  13:36:08  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:26:33  devsrc]
 * 
 * Revision 1.1.3.3  1992/12/15  22:23:54  alan
 * 	Insert copyright notices
 * 	[1992/12/07  16:08:01  alan]
 * 
 * Revision 1.1.3.2  1992/09/29  21:25:45  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/17  20:38:43  sekhar]
 * 
 * $EndLog$
 */


/*
**  This is a pyramid specific file to provide locking and tracing primitives
**  for the threads code that is not in the generic cma threads code
*/


#include <sys/types.h>
#include <signal.h>
#include <sys/sysmips.h>
#include <sys/syscall.h>

#include <cma.h>
#include <cma_host.h>
#include <cma_timer.h>
#include <cma_queue.h>
#include <cma_tcb_defs.h>
#include <cma_tcb.h>
#include <cma_signal.h>
#include <cma_assem.h>

#undef printf

#define sysmips(a,b,c,d) cma_pyrmips_syscall(SYS_sysmips,a,b,c,d)
#define write(a,b,c) cma_pyrmips_syscall(SYS_write,a,b,c,0)
#define exit(n)  cma_pyrmips_syscall(SYS_exit,n,0,0,0)

#if !defined(sigmask)
#define sigmask(n)              ((unsigned long)1 << ((n) - 1))
#endif

#define mask2set(mask, setp)  \
((mask) == -1 ? sigfillset(setp): (((setp)->sigbits[0]) = (mask)))

volatile sigset_t pyrmips_sigbits ;
int pyrmips_siginited;

static char setup_error[]="sysmips(PMIPS_SIGMASK,...) failed.\n";

cma__blk_tst_n_set( int *addr)
{
	int result;
	sigset_t old;
	static pyrmips_siginited;


	if ( !pyrmips_siginited) {
		pyrmips_siginited = 1;  /* if we lose, we just lose ... */
		if (sysmips(PMIPS_SIGMASK, &pyrmips_sigbits,0,0)) 
		{
			(void) write(2,setup_error,sizeof(setup_error));
		        exit(255);
		}
	}
            
	/*
	 * Block signals (except SIGTRAP for debuggers) while
	 * reading/writing the word.
	 */

	old.sigbits[0] = pyrmips_sigbits.sigbits[0];
	pyrmips_sigbits.sigbits[0] = (-1 & ~sigmask(SIGTRAP));

	result = *addr;
	*addr=1;             /* or other non zero value */

	/*
	 * Restore whatever was there (like 0 ?)
	 */
	pyrmips_sigbits.sigbits[0] = old.sigbits[0];
	return(result);
}


cma__pyrmips_kclear( int *addr)
{
	*addr = 0;
	return(0);
}

void
cma__transfer_main_abort()
{
        cma__pyrmips_abort(cma__g_abort_signal,
                           cma__g_def_ctx_buf->sp,
                           cma__abort_process);
}

void
cma__do_async_alert()
{
#ifdef _CMA_TRACE_KERNEL_
        cma__exit_kern_record(__LINE__,__FILE__);
#else
        cma__undefer();
        cma__kernel_unset(&cma__g_kernel_critical);
#endif
        cma__async_delivery();
        cma__bugcheck("do_async_alert:1");
}


#if (_CMA_REENTRANT_CLIB_ == 0)
#ifdef SNI_SVR4_TAINTED
#endif	/* SNI_SVR4_TAINTED */
#endif	/* (_CMA_REENTRANT_CLIB_ == 0) */
