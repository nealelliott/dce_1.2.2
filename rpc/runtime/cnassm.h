/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */ 
/*
 * HISTORY
 * $Log: cnassm.h,v $
 * Revision 1.1.7.2  1996/02/18  22:55:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:35  marty]
 *
 * Revision 1.1.7.1  1995/12/08  00:16:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:15  root]
 * 
 * Revision 1.1.5.6  1994/08/12  21:30:29  tatsu_s
 * 	Made rpc_g_cn_assoc_client_events, rpc_g_cn_assoc_client_states,
 * 	rpc_g_cn_assoc_server_events and rpc_g_cn_assoc_server_states
 * 	available for non-debugging build.
 * 	[1994/08/11  16:36:41  tatsu_s]
 * 
 * Revision 1.1.5.5  1994/07/28  18:28:32  tom
 * 	Bug 11450 - Now that events and states start at 100, subtract
 * 	 RPC_C_CN_STATEBASE when indexing to in rpc_g_cn_assoc_server_states
 * 	 and rpc_g_cn_assoc_server_events.
 * 	[1994/07/28  18:25:58  tom]
 * 
 * Revision 1.1.5.4  1994/07/14  21:30:54  tom
 * 	4-apr-93        us      Changed the state defines by adding
 * 	                             value rpc_c_cn_statebase (100) to each
 * 	                             state as part of runtime performance
 * 	                             changes.  The state value was
 * 				incremented so that we could distinguish
 * 				between states and action routine indexes
 * 				in the state tables, making the protocol
 * 				engine (rpc_cn_sm_eval_event) work more
 * 				efficiently.  State values are now >=
 * 				100, and action routine indexes are < 100.
 * 				Removed predicate table references as the
 * 				predicate functions are now part of the
 * 				action routines.
 * 	[1994/07/11  16:44:13  tom]
 * 
 * Revision 1.1.5.3  1994/03/17  23:06:29  tom
 * 	Big PAC:
 * 	  Move rpc_cn_assoc_sm_work_t to cnp.h.
 * 	[1994/03/17  23:03:53  tom]
 * 
 * Revision 1.1.5.2  1994/01/21  22:33:45  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:53:26  cbrooks]
 * 
 * Revision 1.1.5.1  1994/01/14  20:31:00  tom
 * 	Big PAC work:
 * 	 Add new state to server asm: rpc_c_server_assoc_wait
 * 	 Add new event to server asm: rpc_c_assoc_assoc_complete_resp
 * 	[1994/01/13  22:50:45  tom]
 * 
 * Revision 1.1.2.3  1993/01/03  22:57:51  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:58:17  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:12:34  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:29:59  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:08:03  devrcs
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
**      cnassm.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Definitions of types/constants internal to the NCA Connection
**  RPC Protocol Service association state machine.
**
**
*/

#ifndef _CNASSM_H
#define _CNASSM_H	1

/*
 * The default fragment size all implementations of the NCA
 * Connection protocol must be able to receive as defined in the NCA
 * Architecture spec.
 */
#define RPC_C_ASSOC_MUST_RECV_FRAG_SIZE         1432


/***********************************************************************/
/*
 * R P C _ C N _ A S S O C _ C H E C K _ S T
 *
 * This macro will check the given status. If not rpc_s_ok a local
 * error event will be inserted into the association event list.
 */

#define RPC_CN_ASSOC_CHECK_ST(assoc, st)\
{\
    rpc_cn_sm_event_entry_t     _event;\
\
    if (*(st) != rpc_s_ok)\
    {\
        (assoc)->assoc_local_status = *(st);\
        _event.event_id = RPC_C_ASSOC_LOCAL_ERROR;\
        RPC_CN_ASSOC_INSERT_EVENT ((assoc), &_event);\
        return (*(st));\
    }\
}



/***********************************************************************/
/*
 * R P C _ C N _ A S S O C _ S M _ T R C
 */
#ifdef DEBUG
#define RPC_CN_ASSOC_SM_TRC(assoc, event_id)\
{\
    if ((assoc)->assoc_flags & RPC_C_CN_ASSOC_CLIENT)\
    {\
        RPC_DBG_PRINTF (rpc_e_dbg_cn_state, RPC_C_CN_DBG_ASSOC_SM_TRACE, \
                        ("STATE CLIENT ASSOC: %x state->%s event->%s\n",\
                         assoc,\
                         rpc_g_cn_assoc_client_states[(assoc)->assoc_state.cur_state-RPC_C_CN_STATEBASE],\
                         rpc_g_cn_assoc_client_events[event_id-RPC_C_CN_STATEBASE]));\
    }\
    else\
    {\
        RPC_DBG_PRINTF (rpc_e_dbg_cn_state, RPC_C_CN_DBG_ASSOC_SM_TRACE, \
                        ("STATE SERVER ASSOC: %x state->%s event->%s\n",\
                         assoc,\
                         rpc_g_cn_assoc_server_states[(assoc)->assoc_state.cur_state-RPC_C_CN_STATEBASE],\
                         rpc_g_cn_assoc_server_events[event_id-RPC_C_CN_STATEBASE]));\
    }\
}
#else
#define RPC_CN_ASSOC_SM_TRC(assoc, event_id)
#endif /* DEBUG */


/***********************************************************************/
/*
 * R P C _ C N _ A S S O C _ S M _ T R C _ S T A T E
 */
#ifdef DEBUG
#define RPC_CN_ASSOC_SM_TRC_STATE(assoc)\
{\
    if ((assoc)->assoc_flags & RPC_C_CN_ASSOC_CLIENT)\
    {\
        RPC_DBG_PRINTF (rpc_e_dbg_cn_state, RPC_C_CN_DBG_ASSOC_SM_TRACE, \
                        ("STATE CLIENT ASSOC: %x new state->%s\n",\
                         assoc, \
                         rpc_g_cn_assoc_client_states[(assoc)->assoc_state.cur_state-RPC_C_CN_STATEBASE])); \
    }\
    else\
    {\
        RPC_DBG_PRINTF (rpc_e_dbg_cn_state, RPC_C_CN_DBG_ASSOC_SM_TRACE, \
                        ("STATE SERVER ASSOC: %x new state->%s\n",\
                         assoc, \
                         rpc_g_cn_assoc_server_states[(assoc)->assoc_state.cur_state-RPC_C_CN_STATEBASE])); \
    }\
}
#else
#define RPC_CN_ASSOC_SM_TRC_STATE(assoc)
#endif /* DEBUG */


/***********************************************************************/
/*
 * R P C _ C N _ A S S O C _ E V A L _ N E T W O R K _ E V E N T 
 *
 * This macro will be called by the network receiver thread when an
 * association network event is detected. The "scanned" bit in
 * the association is turned off. This bit is used in finding
 * associations to reclaim. The fragbuf is freed if provided as an
 * event parameter. 
 */
#define RPC_CN_ASSOC_EVAL_NETWORK_EVENT(assoc, event_id, fragbuf, st)\
{\
    RPC_CN_ASSOC_SM_TRC (assoc, event_id);\
    st = rpc__cn_sm_eval_event ((event_id),\
                                (pointer_t) (fragbuf),\
                                (pointer_t) (assoc),\
                                &((assoc)->assoc_state));\
    assoc->assoc_flags &= ~RPC_C_CN_ASSOC_SCANNED;\
    if ((fragbuf) != NULL)\
    {\
        (*(fragbuf)->fragbuf_dealloc)((fragbuf));\
    }\
    RPC_CN_ASSOC_SM_TRC_STATE (assoc); \
}


/***********************************************************************/
/*
 * R P C _ C N _ A S S O C _ E V A L _ U S E R _ E V E N T
 *
 * This macro will be called when user level events are detected. If
 * the association status is bad then don't evaluate the user event.
 * The "scanned" bit in the association is turned off. 
 */
#define RPC_CN_ASSOC_EVAL_USER_EVENT(assoc, event_id, event_param, st)\
{\
    RPC_CN_ASSOC_SM_TRC (assoc, event_id);\
    st = assoc->assoc_status;\
    if (st == rpc_s_ok)\
    {\
        st = rpc__cn_sm_eval_event ((event_id),\
                                    (pointer_t) (event_param),\
                                    (pointer_t) (assoc),\
                                    &((assoc)->assoc_state));\
        assoc->assoc_flags &= ~RPC_C_CN_ASSOC_SCANNED;\
    }\
    RPC_CN_ASSOC_SM_TRC_STATE (assoc); \
}


/***********************************************************************/
/*
 * R P C _ C N _ A S S O C _ I N S E R T _ E V E N T
 *
 * This macro will be called when an event is generated inside an
 * action routine of the association state machine.
 */
#define RPC_CN_ASSOC_INSERT_EVENT(assoc, event)\
{\
    RPC_DBG_PRINTF (rpc_e_dbg_cn_state, RPC_C_CN_DBG_ASSOC_SM_TRACE, \
                    ("STATE INSERT EVENT ")); \
    RPC_CN_ASSOC_SM_TRC ((assoc), (event)->event_id);\
    rpc__cn_sm_insert_event ((event),\
                             &((assoc)->assoc_state));\
}


/***********************************************************************/
/*
 * A S S O C   E V E N T S
 */

/*
 * Events common to both client and server state machines and a
 * comment as to who generated them: the user of the association
 * services or the network.
 *
 * Note: local_error is not defined in the architecture. It is an
 * event indicating a fatal error has occured while processing an
 * event in the state machine.
 *
 * State values are incremented by 100 to distinguish them from
 * action routine indexes which are all < 100.  This was done as
 * an efficiency measure to the engine, rpc__cn_sm_eval_event().
 */ 
#define RPC_C_ASSOC_ABORT_REQ  		101  /* user         */
#define RPC_C_ASSOC_NO_CONN_IND         104  /* network      */
#define RPC_C_ASSOC_ALLOCATE_REQ        109  /* user         */
#define RPC_C_ASSOC_DEALLOCATE_REQ      110  /* user         */
#define RPC_C_ASSOC_LOCAL_ERROR         112  /* internal     */
#define RPC_C_ASSOC_SHUTDOWN_REQ        111   /* user */

/*
 * Events only applicable to client state machine
 *
 * Note: calls_done is 12 in the architecture. I'm
 * making it 13 here so local_error will be 12 and therefore the same
 * as the server local_error event. 
 *
 * Note: shutdown_ind is 11 in the architecture. I'm
 * making it 14 here so shutdown_req will be 11 and therefore the same
 * as the server shutdown_req event. 
 */
#define RPC_C_ASSOC_REQ		        100  /* user         */
#define RPC_C_ASSOC_REQUEST_CONN_ACK    102  /* network      */
#define RPC_C_ASSOC_REQUEST_CONN_NACK   103  /* network      */
#define RPC_C_ASSOC_ACCEPT_CONF         105  /* network      */
#define RPC_C_ASSOC_REJECT_CONF         106  /* network      */
#define RPC_C_ASSOC_ALTER_CONTEXT_REQ   107  /* user         */
#define RPC_C_ASSOC_ALTER_CONTEXT_CONF  108  /* network      */
#define RPC_C_ASSOC_CALLS_DONE          113  /* user         */
#define RPC_C_ASSOC_SHUTDOWN_IND        114  /* network      */

/*
 * Events only applicable to server state machine
 */
/*
 * Note: alter_context_resp is 4 in the architecture. I'm
 * making it 5 here so no_conn_ind will be 4 and therefore the same
 * as the client no_conn_ind event. 
 *
 * Note: accept_resp is 1 in the architecture. I'm making
 * it 13 here so abort_req will be 1 and therefore the same as the
 * client abort_req event.
 */
#define RPC_C_ASSOC_IND           	100  /* network      */
#define RPC_C_ASSOC_REJECT_RESP         102  /* user         */
#define RPC_C_ASSOC_ALTER_CONTEXT_IND   103  /* network      */
#define RPC_C_ASSOC_ALTER_CONTEXT_RESP  105  /* user         */
#define RPC_C_ASSOC_AUTH3_IND           106  /* network      */
#define RPC_C_ASSOC_AUTH3_ACK           107  /* user         */
#define RPC_C_ASSOC_AUTH3_NACK          108  /* user         */
#define RPC_C_ASSOC_ACCEPT_RESP         113   /* user         */
#define RPC_C_ASSOC_ASSOC_COMPLETE_RESP 114   /* user	      */


/***********************************************************************/
/*
 * C L I E N T   A S S O C   S T A T E S
 */

#define	RPC_C_CLIENT_ASSOC_CLOSED	        100
#define RPC_C_CLIENT_ASSOC_CONNECT_WAIT         101 
#define RPC_C_CLIENT_ASSOC_INIT_WAIT            102
#define RPC_C_CLIENT_ASSOC_OPEN                 103
#define RPC_C_CLIENT_ASSOC_ACTIVE               104 
#define RPC_C_CLIENT_ASSOC_CALL_DONE_WAIT       105 
#define RPC_C_CLIENT_ASSOC_STATES	        106 

/***********************************************************************/
/*
 * C L I E N T   A S S O C   T A B L E S
 */
EXTERNAL rpc_cn_sm_state_entry_p_t rpc_g_cn_client_assoc_sm [];
EXTERNAL rpc_cn_sm_action_fn_t     rpc_g_cn_client_assoc_act_tbl [];

EXTERNAL char   *rpc_g_cn_assoc_client_events [];
EXTERNAL char   *rpc_g_cn_assoc_client_states [];


/***********************************************************************/
/*
 * S E R V E R   A S S O C   S T A T E S
 */
#define RPC_C_SERVER_ASSOC_CLOSED               100 
#define RPC_C_SERVER_ASSOC_REQUESTED            101 
#define RPC_C_SERVER_ASSOC_AUTH3_WAIT           102 
#define RPC_C_SERVER_ASSOC_AUTH3                103 
#define RPC_C_SERVER_ASSOC_OPEN                 104 
#define RPC_C_SERVER_ASSOC_WAIT			105 
#define RPC_C_SERVER_ASSOC_STATES               106 

/***********************************************************************/
/*
 * S E R V E R   A S S O C   T A B L E S
 */
EXTERNAL rpc_cn_sm_state_entry_p_t rpc_g_cn_server_assoc_sm [];
EXTERNAL rpc_cn_sm_action_fn_t     rpc_g_cn_server_assoc_act_tbl [];

EXTERNAL char   *rpc_g_cn_assoc_server_events [];
EXTERNAL char   *rpc_g_cn_assoc_server_states [];

#endif /* _CNASSM_H */
