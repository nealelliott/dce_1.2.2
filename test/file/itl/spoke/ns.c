/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns.c,v $
 * Revision 1.1.316.1  1996/10/17  18:28:38  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:23  damon]
 *
 * Revision 1.1.311.1  1994/02/04  20:46:18  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:58  devsrc]
 * 
 * Revision 1.1.309.1  1993/12/07  17:45:22  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:23:18  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: ns.c,v 1.1.316.1 1996/10/17 18:28:38 damon Exp $
 *
 * COMPONENT_NAME: Distributed ITL Development Infrastructure
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */

#include <dce/rpc.h>
#include <defs.h>

pthread_t listen_thread;

int ns_Cleanup();
void itlSpoke_Cleanup();

pointer_t ListenHelper(char * n)
{
    int			st;
    rpc_server_listen((unsigned32)rpc_c_listen_max_calls_default,(unsigned32*) &st);
    if (st)
	DCE_ERROR(st, "server_listen FAILED");
}

int ns_Connect(
	       char * name,
	       rpc_if_handle_t ifspec,
	       rpc_binding_handle_t * handle
	       )
{
    rpc_ns_lookup_handle_t lc;
    unsigned32 		st, result;
    rpc_binding_vector_t * bvP = NULL;

    result = rpc_s_entry_not_found;

    rpc_ns_mgmt_set_exp_age(0, &st);

    rpc_ns_binding_lookup_begin((unsigned32) rpc_c_ns_syntax_dce, 
                (unsigned_char_p_t) name, 
                ifspec, NULL, (unsigned32) rpc_c_binding_max_count_default, 
                (rpc_ns_handle_t *)&lc, (unsigned32 *)&st);

    if (st != rpc_s_ok) {
	DCE_ERROR(st, "rpc_ns_binding_lookup_begin FAILED");
	return st;
    }
    
    while (st == rpc_s_ok) {
	rpc_ns_binding_lookup_next(lc, &bvP, &st);
	if (st == rpc_s_ok) {
	    rpc_ns_binding_select(bvP, handle, &st);
	    result = st;
	    break;
	} else if (st != rpc_s_no_more_bindings ){	
	    result = st;
	    break;
        }
        if (bvP)
       	    rpc_binding_vector_free(&bvP, &st);
    }
    if ( !st ) {
        rpc_mgmt_set_com_timeout(*handle, rpc_c_binding_max_timeout, &st);
	if (st != rpc_s_ok) {
	    DCE_ERROR(st, "rpc_mgmt_set_com_timeout FAILED");
	    result = st;
	}
    }
	    
    rpc_ns_binding_lookup_done(&lc, &st);
    return result;
}

int listening;

int ns_Listen(
	       char * name,
	       rpc_if_handle_t ifspec,
	       rpc_mgr_epv_t epv
	       )
{
    unsigned32		st;
    rpc_binding_vector_t * bvP;
    uuid_t		uuid;
    uuid_vector_t	uuid_vector;
    handle_t		h;
    rpc_ns_handle_t       nsHandle;

    rpc_server_use_all_protseqs(0, &st);
    if ( st ) {
         DCE_ERROR(st, "rpc_server_use_all_protseqs FAILED");
         return st;
    }
    
    rpc_server_register_if(ifspec, NULL, epv, &st); /* YUJS */
    if (st && st != rpc_s_type_already_registered ) {
	DCE_ERROR(st, "register_if FAILED");
        return st;
    }
    
    if (name) {
        if ( ns_Connect(name, ifspec, &h) == rpc_s_ok && h ) {
          if( rpc_mgmt_is_server_listening(h,&st) == TRUE && 
            st == rpc_s_ok ) {
            fprintf(stderr, "name %s already in use!\n", name);
            return TRUE;
          }
          rpc_ns_entry_object_inq_begin( (unsigned32) rpc_c_ns_syntax_dce, 
                 (unsigned char *)name, (rpc_ns_handle_t *)&nsHandle, &st);
          uuid_vector.count = 1;
          while ( st == rpc_s_ok ) {
             rpc_ns_entry_object_inq_next((rpc_ns_handle_t ) nsHandle, 
                 (uuid_t *)  &uuid, &st);
             if ( st == rpc_s_no_more_members ) break;
             uuid_vector.uuid[0] = &uuid;
             rpc_ns_binding_unexport((unsigned32) rpc_c_ns_syntax_dce,
                 (unsigned_char_p_t) name,
                 ifspec, (uuid_vector_p_t)  &uuid_vector,
                 (unsigned32*) &st);
          }
          rpc_ns_entry_object_inq_done( (rpc_ns_handle_t *)&nsHandle, &st);
        }
        rpc_server_inq_bindings(&bvP, &st);
        uuid_create(&uuid, &st);
        if (st) {
		DCE_ERROR(st, "uuid_create FAILED");
		return st;
        }

        uuid_vector.count = 1;
        uuid_vector.uuid[0] = &uuid;
	
        rpc_ep_register(ifspec, (rpc_binding_vector_p_t) bvP, 
             (uuid_vector_p_t) &uuid_vector, 
             (unsigned_char_p_t) name, (unsigned32 *) &st);
        if (st) {
	    DCE_ERROR(st, "rpc_ep_register FAILED");
	    return st;
        }
	
        rpc_ns_binding_export((unsigned32) rpc_c_ns_syntax_dce, 
                 (unsigned_char_p_t) name, 
                 ifspec, bvP, (uuid_vector_p_t)  &uuid_vector,
                 (unsigned32*) &st);

        if (st) {
	    DCE_ERROR(st, "rpc_ns_binding_export FAILED");
	    return st;
        }

        rpc_binding_vector_free(&bvP, &st);
        if ( st ) return st;
   }

    if (!listening) {
	listening = TRUE;	
	st = pthread_create(&listen_thread, pthread_attr_default, 
                (pthread_startroutine_t) ListenHelper,
		(pthread_addr_t) name);
        return st;
    }
}

int ns_Cleanup( name, ifspec )
    char * name;
    rpc_if_handle_t ifspec;
{
    uuid_t              uuid;
    uuid_vector_t       uuid_vector;
    handle_t            h;
    rpc_binding_vector_p_t bvec;
    unsigned32          st;
       
    if (ns_Connect(name, ifspec, &h) == rpc_s_ok) {
        rpc_server_inq_bindings(&bvec, &st);
        if ( st ) {
            DCE_ERROR(st, "rpc_server_inq_bindings FAILED");
            return st;
        }
        
        rpc_binding_inq_object(h, &uuid, &st);
        if (st) {
            DCE_ERROR(st, "rpc_binding_inq_object FAILED");
            return st;
        }
        uuid_vector.count = 1;
        uuid_vector.uuid[0] = &uuid;
        rpc_ep_unregister(ifspec, bvec, &uuid_vector, &st);
        if ( st ) {
            DCE_ERROR(st, "rpc_ep_unregister FAILED");
            return st;
        }
        rpc_ns_binding_unexport((unsigned32) rpc_c_ns_syntax_dce,
                 (unsigned_char_p_t) name,
                 ifspec, (uuid_vector_p_t)  &uuid_vector,
                 (unsigned32*) &st);
        if (st) {
            DCE_ERROR(st, "rpc_ns_binding_unexport FAILED");
            return st;
        }
        rpc_server_unregister_if(ifspec, NULL, &st); /* YUJS */
        if (st) {
            DCE_ERROR(st, "rpc_server_unregister_if FAILED");
            return st;
        }
        rpc_binding_vector_free(&bvec, &st);
    }
    return FALSE;
}
