/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnassoc.h,v $
 * Revision 1.1.88.2  1996/02/18  22:55:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:36  marty]
 *
 * Revision 1.1.88.1  1995/12/08  00:16:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:19  root]
 * 
 * Revision 1.1.86.3  1994/07/14  21:30:59  tom
 * 	For CN performance:
 * 	 Add third argument to rpc__cn_grp_sm_protocol_error() and
 * 	 rpc__cn_assoc_sm_protocol_error().
 * 	[1994/07/14  18:02:44  tom]
 * 
 * Revision 1.1.86.2  1994/01/21  22:33:50  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:53:33  cbrooks]
 * 
 * 	code cleanup
 * 	[1994/01/17  20:49:07  cbrooks]
 * 
 * Revision 1.1.86.1  1994/01/14  20:31:07  tom
 * 	Added freebuf argument to rpc__cn_assoc_send_fragbuf().
 * 	[1994/01/13  22:50:54  tom]
 * 
 * Revision 1.1.6.2  1993/08/18  19:34:48  tom
 * 	Bug 8274 - Added call_rep as input argument to rpc__cn_assoc_dealloc().
 * 	[1993/08/18  19:16:43  tom]
 * 
 * Revision 1.1.4.4  1993/01/03  22:58:04  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:58:30  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/23  20:13:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:30:17  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/13  20:53:10  weisman
 * 	     25-sep-92    ko      Add rpc__cn_assoc_sec_free as PRIVATE
 * 	                          declaration.
 * 	     27-jul-92    dm      Add call_rep as input argument to
 * 	                          rpc__cn_assoc_pop_call().
 * 	[1992/10/13  20:45:58  weisman]
 * 
 * Revision 1.1.2.3  1992/07/07  21:27:55  rsalz
 * 	     25-jun-92    ko      Add RPC_CN_ASSOC_WAKEUP and
 * 	                          RPC_CN_ASSOC_CANCEL_AND_WAKEUP macros.
 * 	                          Add declaration of
 * 	                          rpc__cn_assoc_queue_dummy_frag.
 * 	                          waiting threads of results.
 * 	[1992/07/07  21:23:53  rsalz]
 * 
 * Revision 1.1.2.2  1992/05/01  18:37:32  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  18:25:54  rsalz]
 * 
 * 	 3-mar-92    wh      DCE 1.0.1 merge.
 * 	27-jan-92    ko      Add rpc_addr as input argument to
 * 	                     rpc__cn_assoc_grp_lkup_by_remid().
 * 	[1992/05/01  18:06:14  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:09:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CNASSOC_H
#define _CNASSOC_H 1
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnassoc.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  Interface to the NCA Connection Protocol Service's Association Service.
**
**
*/


/******************************************************************************/
/*
 * R P C _ C N _ A S S O C _ A C B _ I N C _ R E F
 */

#define RPC_CN_ASSOC_ACB_INC_REF(assoc) (assoc)->assoc_acb_ref_count++;

/*
 * R P C _ C N _ A S S O C _ A C B _ D E C _ R E F
 */

#define RPC_CN_ASSOC_ACB_DEC_REF(assoc) (assoc)->assoc_acb_ref_count--;

/*
 * R P C _ C N _ A S S O C _ G R P
 */

#define RPC_CN_ASSOC_GRP(grp_id)\
    (rpc_cn_assoc_grp_t *)(RPC_CN_LOCAL_ID_VALID (grp_id)) ?\
        &rpc_g_cn_assoc_grp_tbl.assoc_grp_vector[(grp_id).parts.id_index] : NULL;

/*
 * R P C _ C N _ A S S O C _ S Y N T A X _ E Q U A L
 */
#if (uuid_c_version == 1)
#define RPC_CN_ASSOC_SYNTAX_EQUAL(s1, s2, st)\
    ((memcmp (&((s1)->id), &((s2)->id), sizeof (uuid_t)) == 0) &&\
     (((s1)->version & 0xFFFF) == ((s2)->version & 0xFFFF)) &&\
     (((s1)->version >> 16) == ((s2)->version >> 16)))
#else
error "***Make sure memcmp works on this version of UUIDs***"
#endif

/*
 * R P C _ C N _ A S S O C _ C A L L
 */

#define RPC_CN_ASSOC_CALL(assoc)          (assoc)->call_rep

/*
 * R P C _ C N _ A S S O C _ M A X _ X M I T _ F R A G
 */

#define RPC_CN_ASSOC_MAX_XMIT_FRAG(assoc) (assoc)->assoc_max_xmit_frag

/*
 * R P C _ C N _ A S S O C _ M A X _ R E C V _ F R A G
 */

#define RPC_CN_ASSOC_MAX_RECV_FRAG(assoc) (assoc)->assoc_max_recv_frag

/*
 * R P C _ C N _ A S S O C _ C O N T E X T _ I D
 */

#define RPC_CN_ASSOC_CONTEXT_ID(assoc)     (assoc)->assoc_pres_context_id

/*
 * R P C _ C N _ A S S O C _ N D R _ F O R M A T
 */

#define RPC_CN_ASSOC_NDR_FORMAT(assoc)      (assoc)->assoc_remote_ndr_format

/*
 * R P C _ C N _ A S S O C _ S E C U R I T Y
 */

#define RPC_CN_ASSOC_SECURITY(assoc)        &(assoc)->security

/*
 * R P C _ C N _ A S S O C _ W A K E U P
 */

#define RPC_CN_ASSOC_WAKEUP(assoc)          rpc__cn_assoc_queue_dummy_frag(assoc);

/*
 * R P C _ C N _ A S S O C _ C A N C E L _ A N D _ W A K E U P
 */

#define RPC_CN_ASSOC_CANCEL_AND_WAKEUP(assoc)\
{\
    RPC_CALL_LOCK (((rpc_call_rep_t *) assoc->call_rep));\
    rpc__cthread_cancel (((rpc_call_rep_t *) assoc->call_rep));\
    rpc__cn_assoc_queue_dummy_frag(assoc);\
    RPC_CALL_UNLOCK (((rpc_call_rep_t *) assoc->call_rep));\
}
 

/******************************************************************************/
/*
 * R P C _ _ C N _ A S S O C _  R E Q U E S T
 */

rpc_cn_assoc_t *rpc__cn_assoc_request _DCE_PROTOTYPE_ ((
    rpc_cn_call_rep_p_t          /* call_r */,
    rpc_cn_binding_rep_p_t       /* binding_r */,
    rpc_if_rep_p_t               /* if_r */,
    rpc_transfer_syntax_t       * /* syntax */,
    unsigned16                  * /* context_id */,
    rpc_cn_sec_context_p_t      * /* sec */,
    unsigned32                  * /* st */ ));
    
/*
 * R P C _ _ C N _ A S S O C _ L I S T E N
 */

rpc_cn_assoc_t *rpc__cn_assoc_listen _DCE_PROTOTYPE_ ((
    rpc_socket_t                 /* newsock */,
    unsigned_char_p_t            /* endpoint */,
    unsigned32                  * /* st */ ));
    
/*
 * R P C _ _ C N _ A S S O C _ A L L O C
 */

PRIVATE void rpc__cn_assoc_alloc _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ D E A L L O C
 */

PRIVATE void rpc__cn_assoc_dealloc _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    rpc_cn_call_rep_p_t          /* call_rep */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ A B O R T 
 */

void rpc__cn_assoc_abort _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ P O P _ C A L L
 */

PRIVATE rpc_cn_call_rep_t *rpc__cn_assoc_pop_call _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    rpc_cn_call_rep_p_t         /* call_rep */ ));

/*
 * R P C _ _ C N _ A S S O C _ P U S H _ C A L L
 */

PRIVATE void rpc__cn_assoc_push_call _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    rpc_cn_call_rep_p_t         /* call_r */ ));

/*
 * R P C _ _ C N _ A S S O C _ Q U E U E _ F R A G
 */

PRIVATE void rpc__cn_assoc_queue_frag _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    rpc_cn_fragbuf_p_t           /* fragbuf */,
    boolean32                   /* signal */ ));

/*
 * R P C _ _ C N _ A S S O C _ Q U E U E _ D U M M Y _ F R A G
 */

PRIVATE void rpc__cn_assoc_queue_dummy_frag _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t            /* assoc */ ));

/*
 * R P C _ _ C N _ A S S O C _ R E C E I V E _ F R A G
 */

PRIVATE void rpc__cn_assoc_receive_frag _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    rpc_cn_fragbuf_p_t          * /* frag_buf */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ S E N D _ F R A G
 */

PRIVATE void rpc__cn_assoc_send_frag _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    rpc_iovector_p_t             /* iovector */,
    rpc_cn_sec_context_p_t       /* sec */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ S E N D _ F R A G B U F
 */

PRIVATE void rpc__cn_assoc_send_fragbuf _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    rpc_cn_fragbuf_p_t           /* fragbuf */,
    rpc_cn_sec_context_p_t       /* sec */,
    boolean32                    /* freebuf */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ S Y N T A X _ F R E E
 */

PRIVATE void rpc__cn_assoc_syntax_free _DCE_PROTOTYPE_ ((
    rpc_cn_syntax_p_t           */* syntax */ ));

/*
 * R P C _ _ C N _ A S S O C _ S Y N T A X _ N E G O T I A T E
 */

PRIVATE void rpc__cn_assoc_syntax_negotiate _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t                 /* assoc */,
    rpc_cn_pres_cont_list_p_t        /* pres_cont_list */,
    unsigned32                      * /* size */,
    rpc_cn_pres_result_list_t       * /* pres_result_list */,
    unsigned32                      * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ S Y N T A X _ L K U P _ B Y _ I D
 */

PRIVATE void rpc__cn_assoc_syntax_lkup_by_id _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    unsigned32                   /* context_id */,
    rpc_cn_syntax_p_t           * /* pres_context */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ S Y N T A X _ L K U P _ B Y _ C L
 */

PRIVATE void rpc__cn_assoc_syntax_lkup_by_cl _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    unsigned32                   /* call_id */,
    rpc_cn_syntax_p_t           * /* pres_context */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ S E C _ L K U P _ B Y _ I D
 */

PRIVATE void rpc__cn_assoc_sec_lkup_by_id _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    unsigned32                   /* key_id */,
    rpc_cn_sec_context_p_t      * /* sec */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ S E C _ A L L O C
 */

PRIVATE rpc_cn_sec_context_t *rpc__cn_assoc_sec_alloc _DCE_PROTOTYPE_ ((
    rpc_auth_info_p_t            /* info */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ S E C _ F R E E
 */

PRIVATE void rpc__cn_assoc_sec_free _DCE_PROTOTYPE_ ((
    rpc_cn_sec_context_p_t      */* sec */ ));

/*
 * R P C _ _ C N _ A S S O C _ P O S T _ E R R O R 
 */

PRIVATE void rpc__cn_assoc_post_error _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t             /* assoc */,
    unsigned32                  /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ S M _ P R O T O C O L _ E R R O R
 */

PRIVATE unsigned32  rpc__cn_assoc_sm_protocol_error _DCE_PROTOTYPE_ ((
    pointer_t                    /* spc_struct */, 
    pointer_t                   /* event_param */,
    pointer_t                   /* sm */ ));


/*
 * R P C _ _ C N _ A S S O C _ S T A T U S _ T O _ P R E J
 */

PRIVATE unsigned32  rpc__cn_assoc_status_to_prej _DCE_PROTOTYPE_ ((
    unsigned32                  /* prej */ ));


/*
 * R P C _ _ C N _ A S S O C _ P R E J _ T O _ S T A T U S
 */

PRIVATE unsigned32  rpc__cn_assoc_prej_to_status _DCE_PROTOTYPE_ ((
    unsigned32                  /* prej */ ));


/*
 * R P C _ _ C N _ A S S O C _ P P R O V _ T O _ S T A T U S
 */

PRIVATE unsigned32  rpc__cn_assoc_pprov_to_status _DCE_PROTOTYPE_ ((
    unsigned32                  /* pprov */ ));


/*
 * R P C _ _ C N _ A S S O C _ A C B _ C R E A T E
 */

void rpc__cn_assoc_acb_create _DCE_PROTOTYPE_ (( rpc_cn_assoc_p_t/* assoc */ ));

/*
 * R P C _ _ C N _ A S S O C _ A C B _ F R E E
 */

void rpc__cn_assoc_acb_free _DCE_PROTOTYPE_ (( rpc_cn_assoc_p_t /* assoc */ ));

/*
 * R P C _ _ C N _ A S S O C _ A C B _ D E A L L O C
 */

PRIVATE void rpc__cn_assoc_acb_dealloc _DCE_PROTOTYPE_ ((rpc_cn_assoc_p_t/* assoc */ ));


/*
 * R P C _ _ C N _ A S S O C _ G R P _ A L L O C
 */

PRIVATE rpc_cn_local_id_t rpc__cn_assoc_grp_alloc _DCE_PROTOTYPE_ ((
    rpc_addr_p_t             /* rpc_addr */,
    unsigned32               /* type */,
    unsigned32               /* rem_id */,                                                             
    unsigned32              * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ G R P _ D E A L L O C
 */

PRIVATE void rpc__cn_assoc_grp_dealloc _DCE_PROTOTYPE_ ((
    rpc_cn_local_id_t       /* grp_id */ ));

/*
 * R P C _ _ C N _ A S S O C _ G R P _ A D D _ A S S O C
 */

PRIVATE void rpc__cn_assoc_grp_add_assoc _DCE_PROTOTYPE_ ((
    rpc_cn_local_id_t        /* grp_id */,
    rpc_cn_assoc_p_t        /* assoc */ ));

/*
 * R P C _ _ C N _ A S S O C _ G R P _ R E M _ A S S O C
 */

PRIVATE void rpc__cn_assoc_grp_rem_assoc _DCE_PROTOTYPE_ ((
    rpc_cn_local_id_t        /* grp_id */,
    rpc_cn_assoc_p_t        /* assoc */ ));

/*
 * R P C _ _ C N _ A S S O C _ G R P _ L K U P _ B Y _ A D D R
 */

PRIVATE rpc_cn_local_id_t rpc__cn_assoc_grp_lkup_by_addr _DCE_PROTOTYPE_ ((
    rpc_addr_p_t                 /* rpc_addr */,
    unsigned32                   /* type */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ G R P _ L K U P _ B Y _ R E M I D
 */

PRIVATE rpc_cn_local_id_t rpc__cn_assoc_grp_lkup_by_remid _DCE_PROTOTYPE_ ((
    unsigned32                   /* rem_id */,
    unsigned32                   /* type */,
    rpc_addr_p_t                 /* rpc_addr */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ G R P _ L K U P _ B Y _ I D
 */

PRIVATE rpc_cn_local_id_t rpc__cn_assoc_grp_lkup_by_id _DCE_PROTOTYPE_ ((
    rpc_cn_local_id_t            /* grp_id */,
    unsigned32                   /* type */,
    unsigned32                  * /* st */ ));

/*
 * R P C _ _ C N _ A S S O C _ G R P _ T B L _ I N I T
 */

PRIVATE void rpc__cn_assoc_grp_tbl_init _DCE_PROTOTYPE_ ((void));

/*
 * R P C _ _ C N _ G R P _ S M _ P R O T O C O L _ E R R O R
 */

PRIVATE unsigned32  rpc__cn_grp_sm_protocol_error _DCE_PROTOTYPE_ ((
    pointer_t                    /* spc_struct */, 
    pointer_t                   /* event_param */,
    pointer_t                   /* sm */));

#endif
