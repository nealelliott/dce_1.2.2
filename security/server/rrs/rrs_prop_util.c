/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_util.c,v $
 * Revision 1.1.6.2  1996/02/18  00:17:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:01  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:48:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:40  root]
 * 
 * Revision 1.1.3.4  1992/12/29  16:33:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:22:00  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:11:25  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:48:11  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  22:32:33  emartin
 * 	bl6c: replication updates
 * 
 * Revision 1.1.3.2  1992/10/07  20:30:50  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:14:42  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:55:26  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Propagation Client - Utilities
 */
    
#include <macros.h>
#include <dce/rpc.h>
#include <dce/rplbase.h>

#include <rs_rep_pvt.h>
#include <rs_rep_bind.h>
#include <rs_task.h>

#include <rrs_prop_util.h>


/* rrs_prop_util_retry_handler
 *
 * Try to recover from communication and authentication failures:
 * the former by rebinding to the server and the latter by 
 * "pinging"  the replica for its replica auth info and refreshing 
 * the virtual memory credential cache for that replica.
 *
 * Return false if the operation is not done (ie. if the calling
 * routine should retry the operation).
 * Return true if the operation is done (ie. if the operation 
 * succeeded or encountered a "fatal" error).
 */
PUBLIC boolean32 rrs_prop_util_retry_handler
#ifndef __STDC__
    (rep_binding, st)
    rs_rep_binding_t        *rep_binding;
    error_status_t          *st;
#else
  (
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
  )
#endif
{
    boolean32       retval;

    /* if everything is fine
     * this update is done
     */
    if (GOOD_STATUS(st)) 
        return true;

    /* try to recover from auth failures by
     * getting the slave's new credentials
     */
    if (STATUS_EQUAL(st, rpc_s_auth_bad_integrity) ||
        STATUS_EQUAL(st, rpc_s_auth_tkt_expired)) {
        
        rs_rep_bind_auth_to_slave_rfrsh(rep_binding, st);
        if (GOOD_STATUS(st))
            /* try again immediately w. new credentials
             */
            return false;   
    }

    if ( ((*st) & rpc_s_mod) == rpc_s_mod) {
        switch(*st) {

            /* server reboot is fatal during init
             */
            case rpc_s_wrong_boot_time: 
                retval = true;
                break;

            /* faults are fatal during init
              */
            case rpc_s_fault_addr_error:
            case rpc_s_fault_context_mismatch:
            case rpc_s_fault_fp_div_by_zero:
            case rpc_s_fault_fp_error:
            case rpc_s_fault_fp_overflow:
            case rpc_s_fault_fp_underflow:
            case rpc_s_fault_ill_inst:
            case rpc_s_fault_int_div_by_zero:
            case rpc_s_fault_int_overflow:
            case rpc_s_fault_invalid_bound:
            case rpc_s_fault_invalid_tag:
            case rpc_s_fault_pipe_closed:
            case rpc_s_fault_pipe_comm_error:
            case rpc_s_fault_pipe_discipline:
            case rpc_s_fault_pipe_empty:
            case rpc_s_fault_pipe_memory:
            case rpc_s_fault_pipe_order:
            case rpc_s_fault_remote_comm_failure:
            case rpc_s_fault_remote_no_memory:
            case rpc_s_fault_unspec:
                retval = true;
                break;

            /* Misc rpc error, try again later.
             *
             * During init, we may want to save the replica's 
             * tower in rep_binding so we can rebind before
             * trying again.
             */
            default:
                retval = false;
                break;
        }
    }   
    else {
        /* all other errors are fatal
         */
        retval = true;
    }

    if (retval == false) {
        /* not done with this update.
         * sleep for a while before the
         * next try
         */
        rs_task_sleep(rrs_c_init_sleep_secs);
    }

    return retval;
}

