/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bindtest.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:19  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:56  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:03:56  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:35:15  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  22:12:14  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:04:21  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:34:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#ifndef BINDTEST_H
#define BINDTEST_H

/* #include <cominit.h> */
typedef unsigned32       rpc_protseq_id_t, *rpc_protseq_id_p_t;
typedef unsigned32       rpc_protocol_id_t, *rpc_protocol_id_p_t;
typedef void *pointer_t; 

/* from common/rpcclock.h */
typedef unsigned32  rpc_clock_t, *rpc_clock_p_t;
     
/* from com/com.h */
typedef unsigned32 rpc_authn_protocol_id_t;
typedef unsigned32 rpc_authz_protocol_id_t;
typedef unsigned32 rpc_authn_level_t;

typedef pointer_t rpc_prot_auth_info_p_t;

            
/* from common/rpclist.h */
typedef struct
{
    pointer_t   next;   /* next element of list                     */
    pointer_t   last;   /* last element of list in a descriptor or  */
                        /* pointer to the prior element in an element */
} rpc_list_t, *rpc_list_p_t;


/* from com.h */

typedef struct
{
    rpc_protseq_id_t        rpc_protseq_id;
    unsigned32              len;
    sockaddr_t              sa;
} *rpc_addr_p_t;
  

typedef struct
{
    unsigned_char_p_t       server_princ_name;
    rpc_authn_level_t       authn_level;
    rpc_authn_protocol_id_t authn_protocol;
    rpc_authz_protocol_id_t authz_protocol;
    rpc_prot_auth_info_p_t  prot_auth_info;
    unsigned                is_server: 1;
    union
    {
        rpc_auth_identity_handle_t  auth_identity;
        rpc_authz_handle_t          privs;
    } u;
} rpc_auth_info_t, *rpc_auth_info_p_t;


typedef struct
{
    /*
     * The following fields are meaningful all the time.
     */
    rpc_list_t                  link;               /* This must be first! */
    rpc_protocol_id_t           protocol_id;
    signed8                     refcnt;
    uuid_t                      obj;
    rpc_addr_p_t                rpc_addr;
    unsigned                    is_server: 1;
    unsigned                    addr_is_dynamic: 1;
    rpc_auth_info_p_t           auth_info;
    /*
     * The following fields are not meaningful for binding reps
     * that are passed to server stubs.
     */
    unsigned                    bound_server_instance: 1;
    unsigned                    addr_has_endpoint: 1;
    unsigned32                  timeout;    /* com timeout */
    signed8                     calls_in_progress;
    pointer_t                   ns_specific;
    rpc_clock_t                 call_timeout_time;  /* max execution time */
    /*
     *
     */
} rpc_binding_rep_t, *rpc_binding_rep_p_t;

#endif /* BINDTEST_H */
