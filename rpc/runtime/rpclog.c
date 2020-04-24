/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpclog.c,v $
 * Revision 1.1.346.2  1996/02/18  00:05:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:21  marty]
 *
 * Revision 1.1.346.1  1995/12/08  00:22:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:25  root]
 * 
 * Revision 1.1.344.1  1994/01/21  22:39:09  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:54  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:54:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:12:09  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:15:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:43:52  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:11:31  devrcs
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
**      rpclog.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  Definitions of global variables.
**
**
*/
#ifdef LOGPTS
#include <rpclog.h>

#ifdef ultrix
#include <nlist.h>
#include <unistd.h>
#endif /* ultrix */

static rpc_logpt_t      logpt_invisible;
rpc_logpt_ptr_t         rpc_g_log_ptr = &logpt_invisible;

#ifndef KERNEL
/*
**++
**
**  ROUTINE NAME:       rpc__log_ptr_init
**
**  SCOPE:              PRIVATE - declared in rpclog.h
**
**  DESCRIPTION:
**
**  This routine will initialize the RPC logging service.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     
**
**      log pointer     pointer value to location to which codes to
**                      be timestamped are written.
**
**  SIDE EFFECTS:       none
**
**--
**/

rpc_logpt_ptr_t rpc__log_ptr_init (void)
#ifdef ultrix
{
    rpc_logpt_ptr_t     ptr;
    unsigned long       logpt_addr_in_virt_mem;

#define QMEM_X 0
    struct nlist symtab[QMEM_X + 2];

    symtab[QMEM_X].n_name = "_qmem";
    symtab[QMEM_X + 1].n_name = NULL;

    nlist ("/vmunix", symtab);
    logpt_addr_in_virt_mem = (symtab[QMEM_X].n_value + LOGPT_ADDR_IN_QMEM);
    ptr = (rpc_logpt_ptr_t) (logpt_addr_in_virt_mem);

    return (ptr);
}

#endif /* ultrix */

#endif /* _KERNEL */

#else
/*
 *  ANSI c does not allow a file to be compiled without declarations.  
 *  If LOGPTS is not defined, we need to declare a dummy variable to
 *  compile under strict ansi c standards.
 */
static  char    _rpclog_dummy_ = 0, *_rpclog_dummy_p = &_rpclog_dummy_;

#endif /* LOGOPTS */
