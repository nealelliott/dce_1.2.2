/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: audit_alloc.h,v $
 * Revision 1.1.4.1  1996/07/08  18:26:48  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:24 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  22:57:32  marty  1.1.2.2]
 *
 * Revision 1.1.2.2  1996/02/18  22:57:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:25  marty]
 * 
 * Revision 1.1.2.1  1995/12/08  17:39:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:49  root]
 * 
 * $EndLog$
 */

#ifndef _AUDIT_ALLOC_H
#define _AUDIT_ALLOC_H

#define AUD_SET_ALLOC_FREE(ea) {                        \
    volatile rpc_ss_thread_handle_t      thread_handle; \
    TRY                                                 \
        thread_handle = rpc_ss_get_thread_handle();     \
    CATCH(pthread_badparam_e);                          \
        thread_handle = NULL;                           \
    ENDTRY;                                             \
    if (thread_handle == NULL) {                        \
        ea = idl_true;                                  \
        rpc_ss_enable_allocate();                       \
    }                                                   \
    else                                                \
        ea = idl_false;                                 \
}

#define AUD_RESET_ALLOC_FREE(ea, data) {                \
    error_status_t st;                                  \
    if (ea)                                             \
        rpc_ss_disable_allocate();                      \
    else                                                \
        rpc_sm_client_free(data, &st);                  \
}

#endif
