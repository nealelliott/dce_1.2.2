/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cninit.c,v $
 * Revision 1.1.65.2  1996/02/18  00:01:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:35  marty]
 *
 * Revision 1.1.65.1  1995/12/08  00:17:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:35  root]
 * 
 * Revision 1.1.63.2  1994/05/19  21:14:25  hopkins
 * 	Merge with DCE1_1.
 * 	[1994/05/04  19:36:44  hopkins]
 * 
 * 	Serviceability work.
 * 	[1994/05/03  20:17:46  hopkins]
 * 
 * Revision 1.1.63.1  1994/01/21  22:34:20  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:54:09  cbrooks]
 * 
 * Revision 1.1.5.2  1993/08/26  18:06:09  tatsu_s
 * 	Bug 8104 - Added rpc__cn_binding_cross_fork() entry in
 * 	cn_binding_epv.
 * 	[1993/08/24  16:07:04  tatsu_s]
 * 
 * Revision 1.1.2.3  1993/01/03  22:58:45  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:59:35  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:15:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:31:19  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:09:25  devrcs
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
**      cninit.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  The NCA Connection Protocol Initialization Service.
**
**
*/

#include <commonp.h>    /* Common declarations for all RPC runtime */
#include <com.h>        /* Common communications services */
#include <comprot.h>    /* Common protocol services */
#include <cnp.h>        /* NCA connection private declarations */
#include <cncall.h>     /* NCA connection call service */
#include <cnid.h>       /* NCA Connection local ID service */
#include <cnbind.h>     /* NCA connection binding service */
#include <cnmgmt.h>     /* NCA connection management service */
#include <cnnet.h>      /* NCA connection network service */
#include <cnfbuf.h>     /* NCA connection fragment buffer service */
#include <cnpkt.h>	/* NCA connection packet layout */
#include <cnassoc.h>    /* NCA connection association service */

void rpc__cn_minute_system_time _DCE_PROTOTYPE_ ((void));

/*
 * INTERNAL variables.
 */

INTERNAL rpc_prot_call_epv_t cn_call_epv =
{
    rpc__cn_call_start,
    rpc__cn_call_transmit,
    rpc__cn_call_transceive,
    rpc__cn_call_receive,
    rpc__cn_call_end,
    rpc__cn_call_block_until_free,
    rpc__cn_call_transmit_fault,
    rpc__cn_call_alert,
    rpc__cn_call_receive_fault,
    rpc__cn_call_did_mgr_execute
};

INTERNAL rpc_prot_mgmt_epv_t cn_mgmt_epv = 
{
    rpc__cn_mgmt_inq_calls_sent,
    rpc__cn_mgmt_inq_calls_rcvd,
    rpc__cn_mgmt_inq_pkts_sent,
    rpc__cn_mgmt_inq_pkts_rcvd,
};

INTERNAL rpc_prot_binding_epv_t cn_binding_epv =
{
    rpc__cn_binding_alloc,
    rpc__cn_binding_init,
    rpc__cn_binding_reset,
    rpc__cn_binding_changed,
    rpc__cn_binding_free,
    rpc__cn_binding_inq_addr,
    rpc__cn_binding_inq_client,
    rpc__cn_binding_copy,
    rpc__cn_binding_cross_fork
};

INTERNAL rpc_prot_network_epv_t cn_network_epv =
{
    rpc__cn_network_use_protseq,
    rpc__cn_network_mon,
    rpc__cn_network_stop_mon,
    rpc__cn_network_maint,
    rpc__cn_network_stop_maint,
    rpc__cn_network_select_dispatch,
    rpc__cn_network_inq_prot_vers
};

/***********************************************************************/


/*
**++
**
**  ROUTINE NAME:       rpc__ncacn_init
**
**  SCOPE:              PRIVATE - declared in comprot.h
**
**  DESCRIPTION:
**
**  This routine will handle one-time initialization for the NCA
**  Connection Protocol Service. It will also return the Entry Point
**  Vectors which comprise the external interface to the NCA
**  Connection Protocol Service.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      call_epv        The Call Service interface.
**      mgmt_epv        The Manangement Service interface.
**      binding_epv     The Binding Service interface.
**      network_epv     The Network Service interface.
**      fork_handler    The fork handler routine.
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     
**
**      rpc_s_coding_error
**      rpc_s_ok
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__ncacn_init 
#ifdef _DCE_PROTO_
(
    rpc_prot_call_epv_p_t           *call_epv,
    rpc_prot_mgmt_epv_p_t           *mgmt_epv,
    rpc_prot_binding_epv_p_t        *binding_epv,
    rpc_prot_network_epv_p_t        *network_epv,
    rpc_prot_fork_handler_fn_t      *fork_handler,
    unsigned32                      *st
)
#else
(call_epv, mgmt_epv, binding_epv, network_epv, fork_handler, st)
rpc_prot_call_epv_p_t           *call_epv;
rpc_prot_mgmt_epv_p_t           *mgmt_epv;
rpc_prot_binding_epv_p_t        *binding_epv;
rpc_prot_network_epv_p_t        *network_epv;
rpc_prot_fork_handler_fn_t      *fork_handler;
unsigned32                      *st;
#endif
{

    CODING_ERROR (st);

    /*
     * Initialize the management counters.
     */
    rpc__cn_mgmt_init ();

    /*
     * Initialize the CN lookaside list condition variable.
     */
    RPC_COND_INIT (rpc_g_cn_lookaside_cond,
                   rpc_g_global_mutex);

    /*
     * Initialize the global sequence number cell.
     */
    rpc__cn_init_seqnum ();

    /*
     * Initialize the global call_id cell.
     */
    rpc_g_cn_call_id = 0;
    
    /*
     * Initialize the call rep lookaside list.
     */
    rpc__list_desc_init (&rpc_g_cn_call_lookaside_list,
                         RPC_C_CN_CALL_LOOKASIDE_MAX,
                         sizeof (rpc_cn_call_rep_t),
                         RPC_C_MEM_CN_CALL_REP,
                         (rpc_list_element_alloc_fn_t) rpc__cn_call_ccb_create,
                         (rpc_list_element_alloc_fn_t) rpc__cn_call_ccb_free,
                         &rpc_g_global_mutex,
                         &rpc_g_cn_lookaside_cond);
    /*
     * Initialize binding_rep lookaside list.
     */
    rpc__list_desc_init (&rpc_g_cn_binding_lookaside_list,               
                         RPC_C_CN_BINDING_LOOKASIDE_MAX,
                         sizeof (rpc_cn_binding_rep_t),
                         RPC_C_MEM_CN_BINDING_REP,
                         NULL,                     
                         NULL,
                         &rpc_g_global_mutex,
                         &rpc_g_cn_lookaside_cond);
    /*
     * Initialize fragment buffer lookaside lists. Note that 7 extra
     * bytes will be allocated per structure. This is so that we can
     * adjust the data pointer to be on an 8 byte boundary and still
     * have the same size data area. Note that the CN global mutex
     * cannot be used to protect the large fragbuf lookaside list,
     * hence the NULL input args for mutex and cond to
     * rpc__list_desc_init. This is because large fragbufs are given
     * to stubs which indirectly call the fragbuf free routine which
     * calls the list free routine. Since this fragbuf free routine is
     * also called internally, where the CN global mutex is held,
     * it would be difficult to determine whether the CN global mutex
     * needed to be acquired in the fragbuf free routine.
     */
    rpc__list_desc_init (&rpc_g_cn_lg_fbuf_lookaside_list,
                         RPC_C_CN_FRAGBUF_LOOKASIDE_MAX,		
                         RPC_C_CN_LG_FRAGBUF_ALLOC_SIZE + 7,
                         RPC_C_MEM_CN_LG_FRAGBUF,
                         NULL,	
                         NULL,
                         NULL,
                         NULL);	
    rpc__list_desc_init (&rpc_g_cn_sm_fbuf_lookaside_list,
                         RPC_C_CN_FRAGBUF_LOOKASIDE_MAX,	  
                         RPC_C_CN_SM_FRAGBUF_ALLOC_SIZE + 7,
                         RPC_C_MEM_CN_SM_FRAGBUF,
                         NULL,                    
                         NULL,
                         &rpc_g_global_mutex,
                         &rpc_g_cn_lookaside_cond);                   
    /*
     * Initialize the association control block lookaside list.
     */
    rpc__list_desc_init (&rpc_g_cn_assoc_lookaside_list,
                         RPC_C_CN_ASSOC_LOOKASIDE_MAX,
                         sizeof(rpc_cn_assoc_t),
                         RPC_C_MEM_CN_ASSOC,
                         (rpc_list_element_alloc_fn_t) rpc__cn_assoc_acb_create,
                         (rpc_list_element_alloc_fn_t) rpc__cn_assoc_acb_free,
                         &rpc_g_global_mutex,
                         &rpc_g_cn_lookaside_cond); 
    /*
     * Initialize the association syntax element lookaside list.
     */
    rpc__list_desc_init (&rpc_g_cn_syntax_lookaside_list,
                         RPC_C_CN_SYNTAX_LOOKASIDE_MAX,
                         sizeof(rpc_cn_syntax_t),
                         RPC_C_MEM_CN_SYNTAX,
                         NULL,
                         NULL,
                         &rpc_g_global_mutex,
                         &rpc_g_cn_lookaside_cond); 
    /*
     * Initialize the association security context element lookaside list.
     */
    rpc__list_desc_init (&rpc_g_cn_sec_lookaside_list,
                         RPC_C_CN_SEC_LOOKASIDE_MAX,
                         sizeof(rpc_cn_sec_context_t),
                         RPC_C_MEM_CN_SEC_CONTEXT,
                         NULL,
                         NULL,
                         &rpc_g_global_mutex,
                         &rpc_g_cn_lookaside_cond); 

    /*
     * Initialize the association group table.
     */
    rpc__cn_assoc_grp_tbl_init ();

    /*
     * Return the interface to the NCA Connection Protocol Service in the four
     * EPVs.
     */
    *call_epv = &cn_call_epv;
    *mgmt_epv = &cn_mgmt_epv;
    *binding_epv = &cn_binding_epv;
    *network_epv = &cn_network_epv;

    if (RPC_DBG(rpc_es_dbg_stats, 5))
    {
        atexit (rpc__cn_stats_print);
    }

    *fork_handler = NULL;
    *st = rpc_s_ok;
}


