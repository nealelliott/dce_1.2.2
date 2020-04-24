/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gssapi-int.h,v $
 * Revision 1.1.4.2  1996/02/18  22:57:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:42:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:23  root]
 * 
 * Revision 1.1.2.5  1994/08/22  13:26:59  wray
 * 	CRs 11376, 11762, 11763, 11765
 * 	[1994/08/22  13:23:57  wray]
 * 
 * Revision 1.1.2.4  1994/06/28  20:00:42  wray
 * 	GSSAPI BL3 (OT 10824)
 * 	[1994/06/28  19:24:16  wray]
 * 
 * Revision 1.1.2.3  1994/06/17  18:41:50  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:27  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  16:47:24  wray
 * 	GSSAPI BL2 (OT#10179)
 * 	[1994/03/23  16:43:02  wray]
 * 
 * 	GSSAPI BL2 (OT#10179)
 * 
 * Revision 1.1.2.1  1994/03/09  15:50:47  wray
 * 	Initial submission (OT#10019)
 * 	[1994/03/09  15:36:20  wray]
 * 
 * $EndLog$
 */

#ifndef GSSAPI_INT_H_
#define GSSAPI_INT_H_

/* The following symbol should be removed once the build environment	    */
/* includes the OSF-supplied serviceability code.			    */
/* #define NO_OSF_SVC_CODE						    */


/*	  Internal definitions for DCE GSSAPI implementation		    */
/*									    */
/*	             This document is OSF-Confidential			    */

#include <dce/idlbase.h>
#include <dce/gssapi.h>
#include <dce/sec_login.h>
#include <dce/rgynbase.h>
#include <dce/rgybase.h>
#include <dce/rpcbase.h>
#include <pthread.h>
#include <dce/utc.h>
#define PTHREAD_EXC
#define PTHRED_EXC_H
#include <dce/sec_authn.h>
#include <krb5/krb5.h>
#include <krb5/rsa-md5.h>
#include <dce/stubbase.h>
/*
 #include <sec_authnp.h>
*/
#ifdef NO_OSF_SVC_CODE
#define GSS_SVC_CODE
#else
#include <dce/dce_svc.h>
#include <dcegsssvc.h>
#include <dcegssmac.h>
extern dce_svc_handle_t gss__svc_handle;
#endif



#define LOW_WINDOW_SIZE 15
#define HIGH_WINDOW_SIZE 10
/* LOW_WINDOW_SIZE is the number of packets (including the most recent) for */
/* which we remember the sequence numbers, and therefore over which we will */
/* be able to detect duplicates (as opposed to just saying that the packet  */
/* is old).								    */
/*									    */
/* HIGH_WINDOW_SIZE is the sequence number range we 'look forward' - IE if  */
/* we get a packet whose sequence number is ahead of where we're expecting, */
/* but less than HIGH_WINDOW_SIZE ahead, then we advance the window.	    */
/* Packets further ahead than HIGH_WINDOW_SIZE will be flagged as severely  */
/* out-of-order, and won't move the window.  This means that if a	    */
/* sequential block of more than HIGH_WINDOW_SIZE packets is dropped, the   */
/* window will cease to operate, and all packets will be flagged as	    */
/* out-of-order.							    */

typedef unsigned32 SEQUENCE_NO;

#define LIST_DEF(type)                       \
typedef struct type##_list_struct {          \
    struct type##_list_struct * prev;        \
    struct type##_list_struct * next;        \
    OM_uint32 id;			     \
    type rec;                                \
} type##_list;                               \
extern type##_list * type##_list_head

typedef struct cred_info_struct {
    pthread_mutex_t mutex;
    int	delete_pending;
    OM_uint32  flags;
    utc_t	expiration_time;
    int	principal_name_valid;
    sec_rgy_name_t principal_name;
    sec_rgy_name_t principal_realm_name;
    sec_rgy_name_t principal_local_name;
    int	principal_uuid_valid;
    uuid_t	principal_uuid;
    uuid_t	principal_realm_uuid;
    sec_login_handle_t lookup_login_context; /*	Used for registry lookups   */
    int		lookup_login_context_valid;
    sec_rgy_handle_t	lookup_registry_context;
    int 	lookup_registry_context_valid;
    sec_login_handle_t login_context;
    int		context_owner;	/* Set if the login context is 'owned' by   */
				/* the credential - ie if the context	    */
				/* should be destroyed when the credential  */
				/* is.					    */
} cred_info;

LIST_DEF(cred_info);

#define CRED_C_FL_INITIATE   1
#define CRED_C_FL_ACCEPT     2
#define CRED_C_FL_DCE1_MECH  256
#define CRED_C_FL_KRB5_MECH  512


typedef struct ctx_info_struct {
    pthread_mutex_t mutex;
    int		    delete_pending;
    OM_uint32	    state;
    unsigned32	    authn_protocol;
    unsigned32	    authz_protocol;
    SEQUENCE_NO     snd_seq;
    SEQUENCE_NO     rcv_seq;
    OM_uint32	    flags;
    int		    delegated;
    gss_cred_id_t   local_cred;
    gss_cred_id_t   deleg_cred;  /* Present for acceptor only */
    krb5_keyblock * auth_session_key;  /* Always present once context is    */
				       /* established			    */
    krb5_keyblock * priv_session_key;
    sec_des_block * priv_key_schedule; /* Present if per-message services   */
				       /* are available			    */
    krb5_keyblock * safe_session_key;
    sec_des_block * safe_key_schedule; /* Present if per-message services   */
				       /* are available			    */
    unsigned	    md_init_length;
    unsigned char * md_init;
    unsigned32	    expiration;
    unsigned32	    context_flags;
    sec_krb_timestamp ctime;     /* Present for acceptor only */
    unsigned32	    cusec;	 /* Present for acceptor only */
    gss_OID	    actual_mech_type;
    unsigned char * client_name; /* Present for acceptor only */
    unsigned char * ticketted_name;  /* Present for acceptor only */
    sec_id_pac_t client_pac;     /* Present for acceptor only */
    rpc_authz_cred_handle_t client_creds;  /* Present for acceptor only */
    sec_krb_parsed_name	server_name; /* Present for acceptor only */
    unsigned char   window_valid;
    SEQUENCE_NO     window_seq;
    unsigned char   low_window[LOW_WINDOW_SIZE];
#ifdef TRACK_HIGH_WINDOW
    unsigned char   cur_window;
    unsigned char   high_window[HIGH_WINDOW_SIZE];    
#endif
    int		    one_time_cred;  /* Set if the local_cred was created    */
				    /* specially for this context, and	    */
				    /* therefore should be deleted with the */
				    /* context.				    */
} ctx_info;

LIST_DEF(ctx_info);

#define CTX_C_ST_CLOSED      0
#define CTX_C_ST_INITIATING  1
#define CTX_C_ST_OPEN_I      4
#define CTX_C_ST_OPEN_A      6
#define CTX_M_ST_OPEN	     4

#define CTX_C_FL_DCE1_MECH  CRED_C_FL_DCE1_MECH
#define CTX_C_FL_KRB5_MECH  CRED_C_FL_KRB5_MECH


/* State diagram:
 *  Initiator's context                        Acceptor's context
 * ===============================================================
 *    CTX_C_ST_CLOSED                           CTX_C_ST_CLOSED
 *          |                                          |
 *  gss_init_sec_context                               |
 *          |           \    -token->                  |
 *   CTX_ST_INITIATING   \===============\             |
 *          |                             \            |
 *          |                              gss_accept_sec_context
 *          |                <-token-     /            |
 *          |            /===============/       CTX_C_ST_OPEN_A
 *          |           /
 *  gss_init_sec_context
 *          |
 *    CTX_C_ST_OPEN_I
 *
 * ===============================================================
 *
 */


/* The following section defines various protocol constants used by GSSAPI. */
/*  Changing these values will render the protocol non-interoperable!!!	    */

#define GSSAPI_CHECKSUM_TYPE 0x8003
#define TOKEN_TYPE_INIT   ((unsigned char *)("\01\00"))
#define TOKEN_TYPE_MUTUAL ((unsigned char *)("\02\00"))
#define TOKEN_TYPE_ERROR  ((unsigned char *)("\03\00"))
#define TOKEN_TYPE_SIGN   ((unsigned char *)("\01\01"))
#define TOKEN_TYPE_DELETE ((unsigned char *)("\01\02"))
#define TOKEN_TYPE_SEAL   ((unsigned char *)("\02\01"))
#define SEAL_ALG_DES 0
#define SEAL_ALG_NONE 0x0000fffful
#define SIGN_ALG_DES_MD5 0
#define SIGN_ALG_MD5 1
#define SIGN_ALG_DES_MAC 2

/* End of protocol constants						    */


#ifdef NO_OSF_SVC_CODE
extern void gss__svc_debug_routing(char * where, error_status_t *st);
extern void gss_svc_printf(int sev, char * format, ...);

#define LSTR(x) LVAL(x)
#define LVAL(x) #x	
#define GSS_MSG(s) __FILE__ "(" LSTR(__LINE__) "): \n        " s

#ifdef svc_c_sev_off
#undef svc_c_sev_off
#endif
#define svc_c_sev_off		    0

#ifdef svc_c_sev_fatal_error
#undef svc_c_sev_fatal_error
#endif
#define svc_c_sev_fatal_error	    10

#ifdef svc_c_sev_error
#undef svc_c_sev_error
#endif
#define svc_c_sev_error		    15

#ifdef svc_c_sev_warning
#undef svc_c_sev_warning
#endif
#define svc_c_sev_warning	    20

#ifdef svc_c_sev_notice
#undef svc_c_sev_notice
#endif
#define svc_c_sev_notice	    25

#ifdef svc_c_sev_notice_verbose
#undef svc_c_sev_notice_verbose
#endif
#define svc_c_sev_notice_verbose    30

#ifdef svc_c_debug1
#undef svc_c_debug1
#endif
#define svc_c_debug1	    41

#ifdef svc_c_debug2
#undef svc_c_debug2
#endif
#define svc_c_debug2	    42

#ifdef svc_c_debug3
#undef svc_c_debug3
#endif
#define svc_c_debug3	    43

#ifdef svc_c_debug4
#undef svc_c_debug4
#endif
#define svc_c_debug4	    44

#ifdef svc_c_debug5
#undef svc_c_debug5
#endif
#define svc_c_debug5	    45

#ifdef svc_c_debug6
#undef svc_c_debug6
#endif
#define svc_c_debug6	    46

#ifdef svc_c_debug7
#undef svc_c_debug7
#endif
#define svc_c_debug7	    47

#ifdef svc_c_debug8
#undef svc_c_debug8
#endif
#define svc_c_debug8	    48

#ifdef svc_c_debug9
#undef svc_c_debug9
#endif
#define svc_c_debug9	    49

#else

#include <dcegsssvc.h>
#ifdef GSS_SVC_CODE
#undef GSS_SVC_CODE
#endif

#endif


extern gss_OID GSSDCE_C_OID_KRBV5_DES_RFC;
extern gss_OID GSSDCE_C_OID_KRBV5_DES_ID;


#endif /* GSSAPI_INT_H_ */
/*  DEC/CMS REPLACEMENT HISTORY, Element GSSAPI-INT.H */
/*  *12    8-JUL-1994 15:04:10 WRAY "GSSAPI BL3" */
/*  *11   22-MAR-1994 10:08:36 WRAY "" */
/*  *10   18-MAR-1994 13:53:14 WRAY "For BL2" */
/*  *9    25-FEB-1994 11:46:41 WRAY "" */
/*  *8    17-JAN-1994 12:37:55 WRAY "" */
/*  *7     6-DEC-1993 11:23:19 WRAY "Move GSSAPI_CHECKSUM_TYPE defn from gssapi-support.c" */
/*  *6    18-NOV-1993 15:29:44 WRAY "" */
/*  *5    18-NOV-1993 14:41:14 WRAY "With tracing" */
/*  *4     9-NOV-1993 15:27:17 WRAY "" */
/*  *3    22-SEP-1993 16:42:05 WRAY "Incorporate into ODE" */
/*  *2    19-AUG-1993 08:31:19 WRAY "Misc bugfixes" */
/*  *1     1-MAR-1993 10:32:48 WRAY "Internal defs for DCE/GSSAPI" */
/*  DEC/CMS REPLACEMENT HISTORY, Element GSSAPI-INT.H */
