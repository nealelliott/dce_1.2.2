/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cncasgsm.c,v $
 * Revision 1.1.36.2  1996/02/18  00:01:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:21  marty]
 *
 * Revision 1.1.36.1  1995/12/08  00:17:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:24  root]
 * 
 * Revision 1.1.34.2  1994/07/14  21:31:01  tom
 * 	 	4-apr-93   us  	Added performance changes to the state
 * 	                     machine.  Changes amount to moving the
 * 	                     predicate routines (sometimes converted
 * 			into macros)  into the action
 * 	                     routines and updating next_state within
 * 	                     the action routines themselves. This
 * 			amounts to reducing the struct rpc_c_sm_
 * 			state_entry_t to containing just an unsigned8
 * 			for the action routine. If no action is
 * 			appropriate, the field will contain next_state
 * 			which is always a value >= 100.   Action
 * 	                     routines now require 3 call parameters:
 * 	                     spc_struct, event_param, and (new) sm
 * 	                     which is the control block from the
 * 	                     event evaluation routine.
 * 	[1994/07/11  16:44:26  tom]
 * 
 * Revision 1.1.34.1  1994/01/21  22:34:00  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  21:53:40  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:58:19  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:58:51  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:14:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:30:39  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:05:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cncasgsm.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  NCA Connection (cn) Client (c) Association (as) Group (g) State Machine (sm).
**
**
 */


#include <commonp.h>    /* Common declarations for all RPC runtime */
#include <com.h>        /* Common communications services */
#include <cnp.h>        /* NCA Connection private declarations */
#include <cnid.h>       /* NCA Connection local ID service */
#include <cnpkt.h>	/* NCA Connection packet encoding */
#include <cnsm.h>	/* NCA Connection state machine declarations */
#include <cnassm.h>     /* NCA Connection association state machine */
#include <cnassoc.h>	/* NCA Connection association services */
#include <cnasgsm.h>    /* NCA Connection association group state machine */


/******************************************************************************/
/*
 * Global Definitions
 */
#ifdef DEBUG
GLOBAL char     *rpc_g_cn_grp_client_events [] =
{
    "NEW              ",
    "ADD_ASSOC        ",
    "REM_ASSOC        "
};

GLOBAL char     *rpc_g_cn_grp_client_states [] =
{
    "CLOSED           ",
    "OPEN             ",
    "ACTIVE           "
};
#endif


/******************************************************************************/
/*
 * Local defines
 */
/******************************************************************************/


/******************************************************************************/
/*
 * Internal function prototyes
 */
/******************************************************************************/


/***********************************************************************/
/*
 * C L I E N T   A S S O C   G R P  P R E D I C A T E   T A B L E
 *
 *  
 * The predicates. All predicates except those noted below are described
 * in the NCA Connection architecture spec.  As a performance enhancement,
 * we have revamped many predicate routines as macros and have absorbed
 * the predicates into the actions.  Thus, there is no longer a need
 * for the predicate table.
 */


/***********************************************************************/
/*
 * C L I E N T   A S S O C   G R P   A C T I O N   T A B L E
 *
 *
 * The actions. All actions except those noted below are described
 * in the NCA Connection architecture spec.
 */
#define INCR_ASSOC_COUNT        0
#define DECR_ASSOC_COUNT        1
#define PROTOCOL_ERROR          2

/*  
 * The action routine prototypes.
 */
INTERNAL unsigned32     incr_assoc_count_action_rtn _DCE_PROTOTYPE_ ((
    pointer_t /*spc_struct*/, 
    pointer_t /*event_param*/,
    pointer_t /*sm*/));

INTERNAL unsigned32     decr_assoc_count_action_rtn _DCE_PROTOTYPE_ ((
    pointer_t /*spc_struct*/, 
    pointer_t /*event_param*/, 
    pointer_t /*sm*/));

/*  
 * The action table itself.
 */
GLOBAL rpc_cn_sm_action_fn_t  rpc_g_cn_client_grp_action_tbl [] =
{
    incr_assoc_count_action_rtn,
    decr_assoc_count_action_rtn,
    rpc__cn_grp_sm_protocol_error
};


/***********************************************************************/
/*
 * C L I E N T   A S S O C   G R P   S T A T E   T A B L E
 *
 *
 * C L O S E D _ S T A T E
 *
 * state 0 - closed. No association group.
 */
INTERNAL rpc_cn_sm_state_tbl_entry_t closed_state =
{
    /* event 0 - new */
    RPC_C_ASSOC_GRP_OPEN,
    
    /* event 1 - add_assoc */
    ILLEGAL_TRANSITION,
    
    /* event 2 - rem_assoc */
    ILLEGAL_TRANSITION
};


/*
 * O P E N _ S T A T E
 *
 * state 1 - open. Association group defined.
 */
INTERNAL rpc_cn_sm_state_tbl_entry_t open_state =
{
    /* event 0 - new */
    ILLEGAL_TRANSITION,
    
    /* event 1 - add_assoc */
    INCR_ASSOC_COUNT,
    
    /* event 2 - rem_assoc */
    ILLEGAL_TRANSITION
};


/*
 * A C T I V E _ S T A T E
 *
 * state 2 - active. One or more associations are currently members
 * of this group.
 */
INTERNAL rpc_cn_sm_state_tbl_entry_t active_state =
{
    /* event 0 - new */
    ILLEGAL_TRANSITION,
    
    /* event 1 - add_assoc */
     INCR_ASSOC_COUNT,
    
    /* event 2 - rem_assoc */
     DECR_ASSOC_COUNT
};


/*
 * The state table containing the action routines.
 */
GLOBAL rpc_cn_sm_state_entry_p_t rpc_g_cn_client_grp_sm [] =
{
    closed_state,	        /* state 0 - closed */
    open_state,		        /* state 1 - open */
    active_state	        /* state 2 - active */
};


/***********************************************************************/
/*
 *
 * C L I E N T   A S S O C   G R P   P R E D I C A T E   R O U T I N E S
 *
 */
/*
**++
**
**  MACRO NAME:		ASSOC_COUNT_PRED        
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:  	This is a macro version of the assoc_count_pred_rtn() 
**			predicate.  We added the macro version to avoid some 
**			of the overhead associated with calling the predicate 
**			function from within the action routines.
**			Functionally, the macro will determine how many
**			current associations there are in the
**			association group and yields the function values
**			shown below.        
**  INPUTS:
**
**      spc_struct      The association group. Note that this is passed in as
**                      the special structure which is passed to the
**                      state machine event evaluation routine.
**
**      event_param     The special event related parameter which is
**                      passed to the state machine event evaluation
**                      routine.
**                      This input argument is ignored.
**
**	status		Instead of returning a value from the macro,
**			write the value calculated in the macro to
**			status.  Status' scope includes the routine
**			calling the macro.  Check status in the calling
**			routine to determine next state and in cases,
**			flow through the action routine. 
** 
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**	status		See explanation above.  
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     0 if number of associations is > 1
**                      1 if number of associations is 1
**
**  SIDE EFFECTS:       none
**
**--
**/
#define ASSOC_COUNT_PRED(spc_struct, event_param, status)\
{\
    RPC_CN_DBG_RTN_PRINTF(CLIENT assoc_count_pred_macro);\
    assoc_grp = (rpc_cn_assoc_grp_t *) spc_struct;\
    if (assoc_grp->grp_cur_assoc == 1)\
    {\
        status = 1;\
    }\
    else\
    {\
        status = 0;\
    }\
}


/***********************************************************************/
/*
 * C L I E N T   A S S O C   G R P   A C T I O N   R O U T I N E S
 */
/*
**++
**
**  ROUTINE NAME:       incr_assoc_count_action_rtn
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  Action routine to increment the number of associations counter.
**
**  INPUTS:
**
**      spc_struct      The association group.  Note that this is passed in as
**                      the special structure which is passed to the
**                      state machine event evaluation routine.
**
**      event_param     The association to be added. This is passed in as the
**			special event related parameter which was
**			passed to the state machine evaluation routine.
**
**  INPUTS/OUTPUTS:   
**
**	sm              The control block from the event
**                      evaluation routine.  Input is the current
**                      status and event for the control block.
**                      Output is the next state or updated
**                      current state, for the control block.
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     completion status, one of:
**                      rpc_s_ok
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL unsigned32     incr_assoc_count_action_rtn 
#ifdef _DCE_PROTO_
(
  pointer_t       spc_struct,
  pointer_t       event_param,
  pointer_t       sm
)
#else
(spc_struct, event_param, sm)

pointer_t       spc_struct;
pointer_t       event_param;
pointer_t       sm;

#endif
{
    rpc_cn_assoc_grp_t          *assoc_grp;
    rpc_cn_sm_ctlblk_t		*sm_p; 

    RPC_CN_DBG_RTN_PRINTF(CLIENT incr_assoc_count_action_rtn);
    
    assoc_grp = (rpc_cn_assoc_grp_t *) spc_struct;
    sm_p = (rpc_cn_sm_ctlblk_t *)sm; 
    
    /*
     * Increment the group association count and add the association
     * to the group's association list.
     */
    assoc_grp->grp_cur_assoc++;
    RPC_LIST_ADD_HEAD (assoc_grp->grp_assoc_list, 
                       (rpc_cn_assoc_t *) event_param, 
                       rpc_cn_assoc_p_t);
    sm_p->cur_state = RPC_C_ASSOC_GRP_ACTIVE;
    return (assoc_grp->grp_status);
}


/*
**++
**
**  ROUTINE NAME:       decr_assoc_count_action_rtn
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  Action routine to decrement the number of associations counter.
**
**  INPUTS:
**
**      spc_struct      The association group.  Note that this is passed in as
**                      the special structure which is passed to the
**                      state machine event evaluation routine.
**
**      event_param     The association to be removed. This is passed in as the
**			special event related parameter which was
**			passed to the state machine evaluation routine.
**
**  INPUTS/OUTPUTS:     
**
**	sm              The control block from the event
**                      evaluation routine.  Input is the current
**                      status and event for the control block.
**                      Output is the next state or updated
**                      current state, for the control block.
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     completion status, one of:
**                      rpc_s_ok
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL unsigned32     decr_assoc_count_action_rtn 
#ifdef _DCE_PROTO_
(
  pointer_t       spc_struct,
  pointer_t       event_param,
  pointer_t	  sm 
)
#else
(spc_struct, event_param, sm)
pointer_t       spc_struct;
pointer_t       event_param;
pointer_t       sm;
#endif
{
    rpc_cn_assoc_grp_t          *assoc_grp;
    rpc_cn_assoc_t              *assoc;
    rpc_cn_sm_ctlblk_t 		*sm_p;
    unsigned32			status;

    assoc_grp = (rpc_cn_assoc_grp_t *) spc_struct;
    sm_p = (rpc_cn_sm_ctlblk_t *)sm; 

    ASSOC_COUNT_PRED(spc_struct, event_param, status);
    if (status == 1)  /* (assoc_grp->grp_cur_assoc == 1) */
    {
   	sm_p->cur_state = RPC_C_ASSOC_GRP_CLOSED;   
    }
    else /* (assoc_grp->grp_cur_assoc > 1) */
    {
        /*
         * There is more than one association on the group.
         */
   	sm_p->cur_state = RPC_C_ASSOC_GRP_ACTIVE;
    }
 
    RPC_CN_DBG_RTN_PRINTF(CLIENT decr_assoc_count_action_rtn);
    
    assoc = (rpc_cn_assoc_t *) event_param;

    /*
     * Clear the group pointer in the association, decrement the
     * association count in the group and remove the association from
     * the group's association list.
     */
    RPC_CN_LOCAL_ID_CLEAR (assoc->assoc_grp_id);
    assoc_grp->grp_cur_assoc--;
    RPC_LIST_REMOVE (assoc_grp->grp_assoc_list, assoc);

    return (assoc_grp->grp_status);
}

