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
 * $Log: dcdrel001a_smgr.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:26  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:55:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:29  root]
 * 
 * Revision 1.1.2.1  1994/07/21  21:27:13  fadden
 * 	Initial version after renaming
 * 	[1994/07/21  20:37:41  fadden]
 * 
 * $EndLog$
 */
/* 
 * NAME
 * 
 *   dcdrel001a_smgr.c
 *
 * COMPONENT
 *   
 *   DCE dced system test
 *   
 * ABSTRACT
 *
 *   This module provides the server manager routines for the 
 *   interface defined in the IDL file dcdrel001a.idl.
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <dce/dce.h>
#include <dce/uuid.h>

#include <dcd_st_utils.h>
#include <dcdrel001a.h>

/*----- Globals -------------------------------------------------------------*/

/*----- Prototypes ----------------------------------------------------------*/

void
register_uuid_all_protseqs_a(
    /* [in]            */	handle_t	h,
    /* [in,string]     */	uuidst_a  	ustring,
    /* [out]           */	error_status_t	*st
);
void
register_uuid_vec_all_protseqsa(
    /* [in]            */	handle_t	h,
    /* [in]            */	uuid_vec_a_t   	*obj_vec,
    /* [out]           */	error_status_t	*st
);
void
unregister_obj_all_protseqs_a(
    /* [in]            */	handle_t	h,
    /* [out]           */	error_status_t	*st
);
void
unregister_uuid_all_protseqs_a(
    /* [in]            */	handle_t	h,
    /* [in,string]     */	uuidst_a  	ustring,
    /* [out]           */	error_status_t	*st
);


/* ---- Functions ---------------------------------------------------------*/

/* register_uuid_all_protseqs_a
 *
 * Register with the endpoint mapper an object uuid passed by the caller,
 * on all protseqs in use by this server.
 */

void
register_uuid_all_protseqs_a(
    /* [in]            */	handle_t	h,
    /* [in,string]     */	uuidst_a  	ustring,
    /* [out]           */	error_status_t	*st
)
{
    
    rpc_binding_vector_t  *bind_vec;
    uuid_vector_t         obj_vec;
    uuid_t                obj_uuid;
    rpc_if_handle_t       *ifspec;
    error_status_t        lstat;
    
    *st = error_status_ok;
    ifspec = &dcdrel001a_v1_0_s_ifspec;
    obj_vec.count = 1;
    obj_vec.uuid[0] = &obj_uuid;
    
    TSTDBG((DBG_LEV2,
	    "register_uuid_all_protseqs_a called with string %s\n",
	    ustring));
    uuid_from_string(ustring, &obj_uuid, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "uuid_from_string", CHK_RET);
    rpc_server_inq_bindings(&bind_vec, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_server_inq_bindings", CHK_RET);
    rpc_ep_register(*ifspec, bind_vec, &obj_vec, ustring, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_ep_register", CHK_CONT);
    /* (skips registering with runtime because its already registered) */
    rpc_binding_vector_free(&bind_vec, &lstat);
    TST_CK_DCE_STATUS(lstat, DBG_WARN, "rpc_binding_vector_free", CHK_CONT);
    return;
}

/* register_uuid_vec_all_protseqsa
 *
 * Register with the endpoint mapper an object uuid vector received from
 * the client, on all protseqs in use by thi server.
 */

void
register_uuid_vec_all_protseqsa(
    /* [in]            */	handle_t	h,
    /* [in]            */	uuid_vec_a_t   	*obj_vec,
    /* [out]           */	error_status_t	*st
)
{
    
    rpc_binding_vector_t  *bind_vec;
    rpc_if_handle_t       *ifspec;
    error_status_t        lstat;
    unsigned32            count;
    uuid_vector_t         *objects;
    
    count = obj_vec->count;
    /* the next message must always be printed in order for the test to pass */
    fprintf(stdout, "register_uuid_vec_all_protseqsa called with array of %d uuids\n",
	    count);
    objects = (uuid_vector_t *) obj_vec;
    ifspec = &dcdrel001a_v1_0_s_ifspec;
    rpc_server_inq_bindings(&bind_vec, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_server_inq_bindings", CHK_RET);
    rpc_ep_register(*ifspec, bind_vec, objects, "DCDREL001", st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_ep_register", CHK_CONT);
    /* (skips registering with runtime because its already registered) */
    rpc_binding_vector_free(&bind_vec, &lstat);
    TST_CK_DCE_STATUS(lstat, DBG_WARN, "rpc_binding_vector_free", CHK_CONT);
    
}

/* unregister_obj_all_protseqs_a
 *
 * Unregister from the endpoint mapper the object uuid associated
 * with this binding, on all protseqs in use by this server.
 */

void
unregister_obj_all_protseqs_a(
    /* [in]            */	handle_t	h,
    /* [out]           */	error_status_t	*st
)
{
    rpc_binding_vector_t  *bind_vec;
    uuid_vector_t         obj_vec;
    uuid_t                obj_uuid;
    rpc_if_handle_t       *ifspec;
    error_status_t        lstat;
    
    *st = error_status_ok;
    ifspec = &dcdrel001a_v1_0_s_ifspec;
    obj_vec.count = 1;
    obj_vec.uuid[0] = &obj_uuid;
        
    TSTDBG((DBG_LEV6, 
	    "unregister_obj_all_protseqs_a called\n"));
    rpc_binding_inq_object(h, &obj_uuid, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_binding_inq_object", CHK_RET);
    rpc_server_inq_bindings(&bind_vec, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_server_inq_bindings", CHK_RET);
    rpc_ep_unregister(*ifspec, bind_vec, &obj_vec, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_ep_unregister", CHK_CONT);
    rpc_binding_vector_free(&bind_vec, &lstat);
    TST_CK_DCE_STATUS(lstat, DBG_WARN, "rpc_binding_vector_free", CHK_CONT);
    return;
}


/* unregister_uuid_all_protseqs_a
 *
 * Unregister from the endpoint mapper an object uuid passed by the caller,
 * on all protseqs in use by this server.
 */

void
unregister_uuid_all_protseqs_a(
    /* [in]            */	handle_t	h,
    /* [in,string]     */	uuidst_a  	ustring,
    /* [out]           */	error_status_t	*st
)
{
    
    rpc_binding_vector_t  *bind_vec;
    uuid_vector_t         obj_vec;
    uuid_t                obj_uuid;
    rpc_if_handle_t       *ifspec;
    error_status_t        lstat;
    
    *st = error_status_ok;
    ifspec = &dcdrel001a_v1_0_s_ifspec;
    obj_vec.count = 1;
    obj_vec.uuid[0] = &obj_uuid;

    TSTDBG((DBG_LEV6, 
	    "unregister_uuid_all_protseqs_a called with string %s\n",
	    ustring));
    uuid_from_string(ustring, &obj_uuid, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "uuid_from_string", CHK_RET);
    rpc_server_inq_bindings(&bind_vec, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_server_inq_bindings", CHK_RET);
    rpc_ep_unregister(*ifspec, bind_vec, &obj_vec, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_ep_unregister", CHK_CONT);
    rpc_binding_vector_free(&bind_vec, &lstat);
    TST_CK_DCE_STATUS(lstat, DBG_WARN, "rpc_binding_vector_free", CHK_CONT);
    return;
}



