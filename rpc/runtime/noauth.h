/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: noauth.h,v $
 * Revision 1.1.612.2  1996/02/18  22:56:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:37  marty]
 *
 * Revision 1.1.612.1  1995/12/08  00:21:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:59:50  root]
 * 
 * Revision 1.1.610.2  1994/01/28  23:09:41  burati
 * 	EPAC changes - add client_creds to rpc_noauth_info_t (dlg_bl1)
 * 	[1994/01/24  23:44:56  burati]
 * 
 * Revision 1.1.610.1  1994/01/21  22:38:17  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:44  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  23:53:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:09:33  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  21:12:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:41:18  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  16:38:23  rsalz
 * 	  9-mar-92 wh              DCE 1.0.1 merge.
 * 	[1992/05/01  16:30:03  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:11:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NOAUTH_H
#define _NOAUTH_H	1
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      noauthp.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**      Types and routines private to the "noauth" pseudo-authentication
**      module.
**
**
*/

#include <commonp.h>
#include <com.h>
#include <comp.h>
    
#include <dce/id_base.h>
#include <dce/sec_authn.h>

/*
 * We allow a little flexibility in whether we support one or both RPC
 * protocols for Dummy authentication.  To simplify Makefiles, etc.
 * if neither RPC-protocol-specific symbol is defined, just assume we
 * want both.
 */

#if !defined(AUTH_DUMMY_DG) && !defined(AUTH_DUMMY_CN)

#define AUTH_DUMMY_DG
#define AUTH_DUMMY_CN

#endif /* !defined(AUTH_DUMMY_DG) && !defined(AUTH_DUMMY_CN) */

/*
 * Max number of keys kept at once on each end of the conversation.
 * This assumes that keys are changed in an interval >> than the round
 * trip time between client and server.
 */

#define RPC__NOAUTH_NKEYS 3

/*
 * State block containing all the state of one end of an authenticated
 * connection.
 */

typedef struct rpc_noauth_info_t {
    rpc_auth_info_t auth_info;  /* This must be the first element. */
    rpc_mutex_t lock;
    unsigned32 status;          /* "poison" status. */

    unsigned_char_p_t client_name; /* client string name, if any */
    sec_id_pac_t client_pac;   /* client PAC */

    /* FAKE-EPAC */
    rpc_authz_cred_handle_t  client_creds;  /* 1.1 epac-style cred handle */
    
    int creds_valid: 1;         /* credentials valid */
    int level_valid: 1;         /* level valid */
    int client_valid: 1;        /* is client valid? */

    /* put addl flags here. */
    
} rpc_noauth_info_t, *rpc_noauth_info_p_t;

/*
 * Locking macros.
 */

#define RPC_KRB_INFO_LOCK(info) RPC_MUTEX_LOCK ((info)->lock)
#define RPC_KRB_INFO_UNLOCK(info) RPC_MUTEX_UNLOCK ((info)->lock)

/*
 * Prototypes for PRIVATE routines.
 */

PRIVATE rpc_protocol_id_t       rpc__noauth_cn_init _DCE_PROTOTYPE_ ((
         rpc_auth_rpc_prot_epv_p_t      * /*epv*/,
         unsigned32                     * /*st*/
    ));

PRIVATE rpc_protocol_id_t       rpc__noauth_dg_init _DCE_PROTOTYPE_ ((
         rpc_auth_rpc_prot_epv_p_t      * /*epv*/,
         unsigned32                     * /*st*/
    ));


/*
 * Prototypes for API EPV routines.
 */

void rpc__noauth_bnd_set_auth _DCE_PROTOTYPE_ ((
        unsigned_char_p_t                   /* in  */    /*server_princ_name*/,
        rpc_authn_level_t                   /* in  */    /*authn_level*/,
        rpc_auth_identity_handle_t          /* in  */    /*auth_identity*/,
        rpc_authz_protocol_id_t             /* in  */    /*authz_protocol*/,
        rpc_binding_handle_t                /* in  */    /*binding_h*/,
        rpc_auth_info_p_t                   /* out */   * /*auth_info*/,
        unsigned32                          /* out */   * /*st*/
    ));

void rpc__noauth_srv_reg_auth _DCE_PROTOTYPE_ ((
        unsigned_char_p_t                   /* in  */    /*server_princ_name*/,
        rpc_auth_key_retrieval_fn_t         /* in  */    /*get_key_func*/,
        pointer_t                           /* in  */    /*arg*/,
        unsigned32                          /* out */   * /*st*/
    ));

void rpc__noauth_mgt_inq_def _DCE_PROTOTYPE_ ((
        unsigned32                          /* out */   * /*authn_level*/,
        unsigned32                          /* out */   * /*st*/
    ));

void rpc__noauth_inq_my_princ_name _DCE_PROTOTYPE_ ((
        unsigned32                          /* in */     /*princ_name_size*/,
        unsigned_char_p_t                   /* out */    /*princ_name*/,
        unsigned32                          /* out */   * /*st*/
    ));

void rpc__noauth_free_info _DCE_PROTOTYPE_((                         
        rpc_auth_info_p_t                   /* in/out */ * /*info*/
    ));

/*
 * Miscellaneous internal entry points.
 */

sec_id_pac_t *rpc__noauth_get_pac _DCE_PROTOTYPE_((void));


/* From sec_id_pickle.h */

typedef struct pickle_handle_s * pickle_handle_t;

/*
 * Functions
 */

/* s e c _ p i c k l e _ c r e a t e
 *
 * Create a pickling context.  This must be called to obtain a pickling
 * context before any pickling calls can be performed.
 */
pickle_handle_t sec_pickle_create _DCE_PROTOTYPE_ (( void ));


/* s e c _ p i c k l e _ r e l e a s e
 *
 * Terminate a pickling context.  This function will release any storage
 * associated with the pickling context.
 */
void sec_pickle_release _DCE_PROTOTYPE_ (( pickle_handle_t * /*p*/));


/* s e c _ i d _ p a c _ f r e e
 * 
 * Release dynamic storage associated with a PAC.
 */

void sec_id_pac_free _DCE_PROTOTYPE_ (( sec_id_pac_t *)) ;

/* s e c _ i d _ p a c _ p i c k l e
 *
 * Pickle a pac.
 */
extern void     sec_id_pac_pickle _DCE_PROTOTYPE_ ((
        /* [in] */      pickle_handle_t          /*pickle_handle*/,
        /* [in] */      sec_id_pac_t            *  /*pac*/,
        /* [out] */     sec_id_pickled_pac_t    **  /*pickled_pac*/
  ));

/* s e c _ i d _ p a c _ u n p i c k l e
 *
 * unpickle a pac 
 */

extern void     sec_id_pac_unpickle _DCE_PROTOTYPE_ ((
        /* [in] */      sec_id_pickled_pac_t    *  /*pickled_pac*/,
        /* [out] */     sec_id_pac_t            *  /*pac*/
  ));

#endif /* _NOAUTH_H */
