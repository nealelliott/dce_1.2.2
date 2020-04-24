/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: autohndl.c,v $
 * Revision 1.1.8.2  1996/02/18  18:52:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:59  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:24:01  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:05 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:13:43  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:53  bfc
 * 	oct 95 idl drop
 * 	[1995/10/22  23:35:42  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/22  22:56:38  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE R1.1
 * 	[1995/10/21  18:25:09  bfc]
 * 
 * Revision 1.1.4.2  1993/07/07  20:04:32  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:35:20  ganni]
 * 
 * $EndLog$
 */
/*
**  @OSF_COPYRIGHT@
**
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME:
**
**      autohndl.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      Support for [auto_handle] client
**
**  VERSION: DCE 1.0
**
**
*/

/* The ordering of the following 3 includes should NOT be changed! */
#include <dce/rpc.h>
#include <dce/stubbase.h>
#if defined(__VMS) || defined(_MSDOS) 
#include <stdio.h>
#endif
#include <dce/dce_cf.h>
#include <lsysdep.h>

/*******************************************************************************/
/*                                                                             */
/*   If there is not currently a valid import cursor, get one                  */
/*                                                                             */
/*******************************************************************************/
void rpc_ss_make_import_cursor_valid
#ifdef IDL_PROTOTYPES
(
    RPC_SS_THREADS_MUTEX_T *p_import_cursor_mutex,
    rpc_ns_handle_t *p_import_cursor,
    rpc_if_handle_t p_if_spec,
    error_status_t *p_import_cursor_status
)
#else
( p_import_cursor_mutex, p_import_cursor, p_if_spec, p_import_cursor_status )
    RPC_SS_THREADS_MUTEX_T *p_import_cursor_mutex;
    rpc_ns_handle_t *p_import_cursor;
    rpc_if_handle_t p_if_spec;
    error_status_t *p_import_cursor_status;
#endif
{
    char *p;				/* profile name (without cell part) */
    char profile_name[CELL_NAME_LEN];	/* full profile name		    */

    RPC_SS_THREADS_MUTEX_LOCK( p_import_cursor_mutex );
    TRY
    if ( *p_import_cursor_status != error_status_ok )
    {
        rpc_ns_binding_import_begin(rpc_c_ns_syntax_default, NULL,
                   p_if_spec, NULL, p_import_cursor, p_import_cursor_status);
    }
    if (*p_import_cursor_status == rpc_s_no_env_setup ) {
	dce_cf_profile_entry_from_host(NULL, &p, p_import_cursor_status);
	if (*p_import_cursor_status != rpc_s_ok) {
            RPC_SS_THREADS_MUTEX_UNLOCK( p_import_cursor_mutex );
	    return;
	}
	sprintf(profile_name, "%s%s", CELL_DIR_ROOT, p);
        rpc_ns_binding_import_begin(rpc_c_ns_syntax_default,
		(unsigned_char_t *) profile_name,
                p_if_spec, NULL, p_import_cursor, p_import_cursor_status);
    }
    FINALLY
    RPC_SS_THREADS_MUTEX_UNLOCK( p_import_cursor_mutex );
    ENDTRY
}

/*******************************************************************************/
/*                                                                             */
/*   Get next potential server                                                 */
/*                                                                             */
/*******************************************************************************/
void rpc_ss_import_cursor_advance
#ifdef IDL_PROTOTYPES
(
    RPC_SS_THREADS_MUTEX_T *p_import_cursor_mutex,
    ndr_boolean *p_cache_timeout_was_set_low, /* true if the cache time out */
                                              /* for this import context    */
                                              /* was set low at some point. */
    rpc_ns_handle_t *p_import_cursor,
    rpc_if_handle_t p_if_spec,
    ndr_boolean *p_binding_had_error,
        /* TRUE if an error occurred using the current binding */
    rpc_binding_handle_t *p_interface_binding,
        /* Ptr to binding currently being used for this interface */
    rpc_binding_handle_t *p_operation_binding,
      /* Ptr to location for binding operation is using, NULL if first attempt */
    error_status_t *p_import_cursor_status,
    error_status_t *p_st
)
#else
( p_import_cursor_mutex, p_cache_timeout_was_set_low,
  p_import_cursor, p_if_spec, p_binding_had_error, p_interface_binding,
  p_operation_binding, p_import_cursor_status, p_st )
    RPC_SS_THREADS_MUTEX_T *p_import_cursor_mutex;
    ndr_boolean *p_cache_timeout_was_set_low;  /* true if the cache time    */
                                               /* out for this import       */
                                               /* context was set low at    */
                                               /* some point.               */
    rpc_ns_handle_t *p_import_cursor;
    rpc_if_handle_t p_if_spec;
    ndr_boolean *p_binding_had_error;
    rpc_binding_handle_t *p_interface_binding;
        /* Ptr to binding currently being used for this interface */
    rpc_binding_handle_t *p_operation_binding;
      /* Ptr to location for binding operation is using, NULL if first attempt */
    error_status_t *p_import_cursor_status;
    error_status_t *p_st;
#endif
{
    error_status_t st2;

    RPC_SS_THREADS_MUTEX_LOCK( p_import_cursor_mutex );
    TRY
    if ( ( ! rpc_binding_handle_equal(*p_operation_binding,
                                      *p_interface_binding, p_st) )
            && ( ! *p_binding_had_error ) )
    {
        /* Another thread has advanced the cursor
           and no error has yet been detected */
        if ( *p_interface_binding != NULL)
        {
            rpc_binding_handle_copy( *p_interface_binding,
                                    p_operation_binding, p_st);

	    /* If we are on the first pass, use min timeout to improve response time */
	    if (*p_cache_timeout_was_set_low == idl_false)
		rpc_mgmt_set_com_timeout( *p_operation_binding,
				    rpc_c_binding_min_timeout,
				    &st2 );
        }
        else
        {
            /* No more servers to try */
            *p_st = rpc_s_no_more_bindings;
        }
        goto mutex_release;
    }

    /* If we currently have a binding, release it */
    if ( *p_interface_binding != NULL)
    {
        rpc_binding_free( p_interface_binding, p_st);
    }
    /* Any new binding will not yet have errors */
    *p_binding_had_error = ndr_false;

    /* Advance the cursor */
    rpc_ns_binding_import_next ( *p_import_cursor, p_interface_binding, p_st );
    while (( *p_st == rpc_s_no_more_bindings ) &&
           ( *p_cache_timeout_was_set_low == idl_false))
    {
        /* Make ready to restart */
        *p_interface_binding = NULL;
        rpc_ns_binding_import_done( p_import_cursor, p_import_cursor_status);
        rpc_ns_binding_import_begin(rpc_c_ns_syntax_default, NULL,
               p_if_spec, NULL, p_import_cursor, p_import_cursor_status);

        /*
         * If we have never lowered the cache timeout value, then we may
         * have gotten the no more bindings status because of a stale cache.
         * Lower the cache timeout and try to import again.
         */
        if (*p_cache_timeout_was_set_low == idl_false)
        {
            /* Note that we have reset the cache timeout */
            *p_cache_timeout_was_set_low = idl_true;

            /*
             *  Set a low cache timeout to force a refresh on this import
             *  context.
             */
            rpc_ns_mgmt_handle_set_exp_age(
                            (rpc_ns_handle_t) *p_import_cursor, 10, p_st );

            rpc_ns_binding_import_next
                           ( *p_import_cursor, p_interface_binding, p_st );
        }
    }


    if ( *p_interface_binding == NULL ) *p_operation_binding = NULL;
    else 
    {
	rpc_binding_handle_copy( *p_interface_binding,
				p_operation_binding, &st2);

	/* If we are on the first pass, use min timeout to improve response time */
	if (*p_cache_timeout_was_set_low == idl_false)
	    rpc_mgmt_set_com_timeout( *p_operation_binding,
				rpc_c_binding_min_timeout, &st2);
    }

 mutex_release:;
    FINALLY
    RPC_SS_THREADS_MUTEX_UNLOCK( p_import_cursor_mutex );
    ENDTRY
}

/*******************************************************************************/
/*                                                                             */
/*   Flag "error occurred when an operation used this binding"                 */
/*                                                                             */
/*******************************************************************************/
void rpc_ss_flag_error_on_binding
#ifdef IDL_PROTOTYPES
(
    RPC_SS_THREADS_MUTEX_T *p_import_cursor_mutex,
    ndr_boolean *p_binding_had_error,
    rpc_binding_handle_t *p_interface_binding,
        /* Ptr to binding currently being used for this interface */
    rpc_binding_handle_t *p_operation_binding
      /* Ptr to location for binding operation is using */
)
#else
( p_import_cursor_mutex,
  p_binding_had_error, p_interface_binding, p_operation_binding )
    RPC_SS_THREADS_MUTEX_T *p_import_cursor_mutex;
    ndr_boolean *p_binding_had_error;
    rpc_binding_handle_t *p_interface_binding;
    rpc_binding_handle_t *p_operation_binding;
#endif
{
    RPC_SS_THREADS_MUTEX_LOCK( p_import_cursor_mutex );
    if ( *p_interface_binding == *p_operation_binding )
    {
        /* Nobody has advanced the cursor
           Flag that error occurred using this binding */
        *p_binding_had_error = ndr_true;
    }
    RPC_SS_THREADS_MUTEX_UNLOCK( p_import_cursor_mutex );
}
