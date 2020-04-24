/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnsm.h,v $
 * Revision 1.1.34.2  1996/02/18  22:55:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:48  marty]
 *
 * Revision 1.1.34.1  1995/12/08  00:17:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:53  root]
 * 
 * Revision 1.1.32.2  1994/07/14  21:31:18  tom
 * 	     01-jun-93	us  Added tbl_id to rpc__cn_sm_init;
 * 			    optimization to help bypass event
 * 			    evaluation routines and go directly
 * 			    to the action routines.  Moved
 * 			    RPC_CN_SM_GET_NEXT_EVENT here so it
 * 			    could be externally accessible; moved
 * 			    client and server incr_active_action_rtn
 * 			    and server assoc decr_active_action_rtn
 * 			    function here as macros so they, too can
 * 			    be externally accessed. Done as optimizations.
 * 	27-apr-93	us  Removed the predicate table as last
 * 			    step to the performance enhancements
 * 			    to rpc__cn_sm_event_eval.  See cnsm.c
 * 			    comments for full details.
 * 	[1994/07/11  16:52:16  tom]
 * 
 * Revision 1.1.32.1  1994/01/21  22:34:50  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  21:54:32  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:59:30  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:00:49  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:18:11  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:32:27  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:06:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CNSM_H
#define _CNSM_H	1
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnsm.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Interface to the NCA Connection Protocol State Machine Service
**
**
*/

#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif

/*
 * R P C _ _ C N _ S M _ I N I T
 */

void rpc__cn_sm_init _DCE_PROTOTYPE_ ((
    rpc_cn_sm_state_entry_p_t   */* state_tbl */,
    rpc_cn_sm_action_fn_p_t     /* action_tbl */,
    rpc_cn_sm_ctlblk_p_t         /* sm */,
    unsigned32			 /* tbl_id */));
                       
/*
 * R P C _ _ C N _ S M _ E V A L _ E V E N T
 */

unsigned32     rpc__cn_sm_eval_event _DCE_PROTOTYPE_ ((
    unsigned32                  /* event_id */,
    pointer_t                   /* event_parameter */,
    pointer_t                   /* spc_struct */,
    rpc_cn_sm_ctlblk_p_t         /* sm */));


/*
 * R P C _ _ C N _ S M _ I N I T _ E V E N T _ L I S T
 */

void rpc__cn_sm_init_event_list _DCE_PROTOTYPE_ ((rpc_cn_sm_ctlblk_t  *));


/*
 * R P C _ _ C N _ S M _ I N S E R T _ E V E N T
 */

void rpc__cn_sm_insert_event _DCE_PROTOTYPE_ ((
    rpc_cn_sm_event_entry_p_t   /* event */,
    rpc_cn_sm_ctlblk_t          * /* sm */));


/*
**++
**
**  MACRO NAME:       RPC_CN_INCR_ACTIVE_SVR_ACTION
**
**  SCOPE:            Internal to the rpc;  used here and in cnassoc.c.
**
**  DESCRIPTION:
**      
**  MACRO to set the active predicate to true. The server
**  runtime allocated the association for the new call and its
**  callbacks. Only one call and its related callbacks may allocate an
**  association at a time.  This macro includes the essence of
**  incr_active_action_rtn.  
**
**  INPUTS:
**
**      assoc		Pointer to the association.
** 
**   	sm 		The control block from the event evaluation
**                      routine.  Input is the current state and
**                      event for the control block.  Output is the
**                      next state or updated current state, for the
**                      control block.
**
**  INPUTS/OUTPUTS:     none  
**
**  OUTPUTS:            Modifies the association reference count and the
**			current state of the control block.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none      
**
**  SIDE EFFECTS:       none
**
**--
**/
#define RPC_CN_INCR_ACTIVE_SVR_ACTION(assoc, sm)\
{\
    	RPC_CN_DBG_RTN_PRINTF(SERVER rpc_cn_incr_active_svr_action_macro); \
	assoc->assoc_ref_count++;\
	sm->cur_state = RPC_C_SERVER_ASSOC_OPEN;\
}



/*
**++
**
**  MACRO NAME:         RPC_CN_INCR_ACTIVE_CL_ACTION  
**
**  SCOPE:              GLOBAL
**
**  DESCRIPTION:
**      
**  Action client side macro, to set the active predicate to true. The client
**  runtime allocated the association for the new call and its
**  callbacks. Only one call and its related callbacks may allocate an
**  association at a time.
**
**  INPUTS:
**
**      assoc		A pointer to the association.
**
**   	sm 		The control block from the event evaluation
**                      routine.  Input is the current state and
**                      event for the control block.  Output is the
**                      next state or updated current state, for the
**                      control block.
**
**  INPUTS/OUTPUTS:     none  
**
**  OUTPUTS:            Modifies the association reference count and the
**			current state of the control block.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/ 
#define RPC_CN_INCR_ACTIVE_CL_ACTION(assoc, sm)\
{\
    RPC_CN_DBG_RTN_PRINTF(CLIENT rpc_cn_incr_active_cl_action_macro); \
    assoc->assoc_ref_count++;\
    sm->cur_state = RPC_C_CLIENT_ASSOC_ACTIVE;\
}
/*
**++
**
**  MACRO NAME:       RPC_CN_DECR_ACTIVE_SVR_ACTION
**
**  SCOPE:            Internal to the rpc;  used here and in cnassoc.c.
**
**  DESCRIPTION:
**      
**  MACRO to set the active predicate to true. The server
**  runtime allocated the association for the new call and its
**  callbacks. Only one call and its related callbacks may allocate an
**  association at a time.  This macro includes the essence of
**  decr_active_action_rtn.  
**
**  INPUTS:
**
**      assoc		Pointer to the association.
** 
**   	sm 		The control block from the event evaluation
**                      routine.  Input is the current state and
**                      event for the control block.  Output is the
**                      next state or updated current state, for the
**                      control block.
**
**  INPUTS/OUTPUTS:     none  
**
**  OUTPUTS:            Modifies the association reference count and the
**			current state of the control block.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none      
**
**  SIDE EFFECTS:       none
**
**--
**/
#define RPC_CN_DECR_ACTIVE_SVR_ACTION(assoc, sm)\
{\
    	RPC_CN_DBG_RTN_PRINTF(SERVER rpc_cn_decr_active_svr_action_macro); \
	assoc->assoc_ref_count--;\
	sm->cur_state = RPC_C_SERVER_ASSOC_OPEN;\
}



#define RPC_CN_SM_GET_NEXT_EVENT(sm, event, more)\
{\
    if ((sm)->event_list_state == RPC_C_CN_SM_EVENT_LIST_EMPTY)\
    {\
        more = false;\
    }\
    else\
    {\
        (event)->event_id = (sm)->event_list[(sm)->event_list_hindex].event_id;\
        (event)->event_param = (sm)->event_list[(sm)->event_list_hindex].event_param;\
        (sm)->event_list_hindex = ((sm)->event_list_hindex + 1) &\
        (RPC_C_CN_SM_EVENT_LIST_MAX_ENTRIES - 1); \
        if ((sm)->event_list_hindex == (sm)->event_list_tindex)\
        {\
            (sm)->event_list_state = RPC_C_CN_SM_EVENT_LIST_EMPTY;\
        }\
        more = true;\
    }\
}

#endif
