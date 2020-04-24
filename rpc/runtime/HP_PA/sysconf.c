/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysconf.c,v $
 * Revision 1.1.4.2  1996/02/18  00:01:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:16:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:57  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:56:50  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:56:34  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:09:25  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:27:21  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:09:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      rpc/runtime/HP_PA/sysconf.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Include HP_OSF atfork grodiness.
**
**
*/

#include <commonp.h>

/*
 * This really doesn't belong here, but is here for pragmatic reasons.
 */

typedef void (*atfork_handler_ptr_t)
    (rpc_fork_stage_id_t stage);
/*
 * Save the address of the fork handler registered through atfork.  
 */
atfork_handler_ptr_t  atfork_handler_ptr;

/*
 * _ P R E _ F O R K
 *
 * This procedure is called by the Pthreads library prior to calling
 * the fork/vfork system call.
 */         
INTERNAL void _pre_fork()
{                      
    (*atfork_handler_ptr)(rpc_c_prefork);
}

/*
 * _ P O S T _ F O R K _ C H I L D
 *
 * This procedure is called in the child of a forked process immediately
 * after the fork is performed.
 */         
INTERNAL void _post_fork_child()
{                
    (*atfork_handler_ptr)(rpc_c_postfork_child);
}

/*
 * _ P O S T _ F O R K _ P A R E N T
 *
 * This procedure is called in the parent of a forked process immediately
 * after the fork is performed.
 */         
INTERNAL void _post_fork_parent()
{
    (*atfork_handler_ptr)(rpc_c_postfork_parent);
}


void rpc__hposf_atfork(handler)
pointer_t handler;
{
    /*
     * Save the address of the handler routine, and register our own
     * handlers. (see note above)
     */
    atfork_handler_ptr = handler;
    pthread_atfork(_pre_fork, _post_fork_parent, _post_fork_child);
}
