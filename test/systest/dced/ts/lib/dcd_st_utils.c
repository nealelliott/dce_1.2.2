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
 * $Log: dcd_st_utils.c,v $
 * Revision 1.1.4.2  1996/02/17  23:26:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:21  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:55:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:55:12  root]
 * 
 * Revision 1.1.2.4  1994/07/21  21:26:54  fadden
 * 	Set expiration age to 0 in case ns_grp_mbr_add failure is
 * 	  due to a cache problem
 * 	[1994/07/21  21:00:57  fadden]
 * 
 * Revision 1.1.2.3  1994/06/24  19:53:59  fadden
 * 	Merged with changes from 1.1.2.2
 * 	[1994/06/24  19:53:17  fadden]
 * 
 * 	Modify error reporting for use without DTET
 * 	Add some cleanup for errors during server setup
 * 	[1994/06/24  18:24:19  fadden]
 * 
 * Revision 1.1.2.2  1994/06/17  18:46:06  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:36:46  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/26  22:44:19  fadden
 * 	Move DTET-based routines here and make them TET-independent
 * 	[1994/05/26  20:10:32  fadden]
 * 
 * 	Freeze working dtet version
 * 	[1994/05/13  15:41:14  fadden]
 * 
 * 	Initial Version
 * 	[1994/04/25  19:58:50  fadden]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include <dce/dce.h>
#include <dce/uuid.h>
#include <dce/aclif.h>
#include <dcd_st_utils.h>

/* ______________________________________________________________________ */

/* server_setup()
 *   Perform basic server steps 
 */
void
server_setup(
  /* in */
    unsigned_char_t		*servername,
    unsigned_char_t             *groupname,
    rpc_if_handle_t		ifspec,
    uuid_t                      *obj_uuid,
    unsigned_char_t		*annotation,
    boolean32			(*authzn)(),
  /* out */
    rpc_binding_vector_t	**bind_vec,
    error_status_t		*st
)
{
    uuid_vector_t		obj_vec;
    int                         step;
    unsigned32			exp_age=0;

    obj_vec.count   = 1;
    obj_vec.uuid[0] = obj_uuid;

    step	    = 0;  /* track how far we get successfully */

    rpc_server_register_if(ifspec, NULL, NULL, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_server_register_if", CHK_RET);

    if (*st == error_status_ok) step += 1;

    if (step == 1)
    {
	rpc_server_use_all_protseqs(rpc_c_protseq_max_reqs_default, st);
	TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_server_use_all_protseqs",
			  CHK_CONT);
	if (*st == error_status_ok) step += 1;
    }

    if (step == 2)
    {
	rpc_server_inq_bindings(bind_vec, st);
	TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_server_inq_bindings", CHK_CONT);
	if (*st == error_status_ok) step += 1;
    }

    if (step == 3)
    {
	rpc_ep_register(ifspec, *bind_vec, &obj_vec, annotation, st);
	TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_ep_register", CHK_CONT);
	if (*st == error_status_ok) step += 1;
    }

    if (step == 4)
    {
	rpc_ns_mgmt_set_exp_age(exp_age, st);
        TST_CK_DCE_STATUS(*st, DBG_WARN, "rpc_ns_mgmt_set_exp_age", CHK_CONT);
	rpc_ns_binding_export(rpc_c_ns_syntax_default, servername,
			      ifspec, *bind_vec, &obj_vec, st);
	TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_ns_binding_export", CHK_CONT);
	if (*st == error_status_ok) step += 1;
    }

    if (step == 5)
    {
	rpc_ns_group_mbr_add(rpc_c_ns_syntax_default, groupname,
			     rpc_c_ns_syntax_default, servername, st);
	TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_ns_group_mbr_add", CHK_CONT);
	if (*st == error_status_ok) step += 1;
    }

    if (step == 6)
    {
	rpc_mgmt_set_authorization_fn(authzn, st);
	TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_mgmt_set_authorization_fn", 
			  CHK_CONT);
	if (*st == error_status_ok) step += 1;
    }
    return;
}

/* server_takedown()
 *   Perform basic server shutdown steps
 *   Note at present that the return status is just the result of 
 *     the rpc_binding_vector_free call, and probably should be
 *     dropped from the interface
 */
void
server_takedown(
    uuid_t			*obj_uuid,	/* in */
    rpc_binding_vector_t	**bind_vec,	/* in */
    unsigned_char_t		*servername,	/* in */
    rpc_if_handle_t		ifspec,		/* in */
    error_status_t		*st		/* out */
)
{
    uuid_vector_t		obj_vec;

    obj_vec.count   = 1;
    obj_vec.uuid[0] = obj_uuid;

    rpc_ns_binding_unexport(rpc_c_ns_syntax_default, servername,
			    ifspec, &obj_vec, st); 
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_ns_binding_unexport", CHK_CONT);

    rpc_ep_unregister(ifspec, *bind_vec, &obj_vec, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_ep_unregister", CHK_CONT);

    rpc_binding_vector_free(bind_vec, st);
    TST_CK_DCE_STATUS(*st, DBG_WARN, "rpc_binding_vector_free", CHK_CONT);
    return;
    
}

void
create_new_bh_from_old(
    rpc_binding_handle_t     old_bh,            /* in  */
    uuid_t                   *uuid,             /* in  */
    rpc_binding_handle_t     *new_bh,           /* in, out */
    error_status_t           *st		/* out */    
)
{
    rpc_binding_copy(old_bh, new_bh, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_binding_copy",CHK_RET);
    rpc_binding_set_object(*new_bh, uuid, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_binding_set_object",CHK_RET);
    rpc_binding_reset(*new_bh, st);
    TST_CK_DCE_STATUS(*st, DBG_FATAL, "rpc_binding_reset",CHK_RET);
    return;
}



void
dcd_st_alloc_uuid_vec(
    int                  count,             /* in  */
    uuid_vector_t        **ublock,          /* in, out */
    error_status_t       *st                /* out */
)
{
    int i;
    uuid_p_t puuid;   /* points to uuid */
    uuid_p_t *uuids;  /* start of UUID pointer array */
    
    *st = error_status_ok;
    *ublock = (uuid_vector_t*) calloc(count+1, sizeof(uuid_p_t));
    if (*ublock == NULL) 
    {
	TSTDBG((DBG_FATAL,"Can't malloc memory for uuid pointer array\n"));
	*st = -1;
	return;
    }
    (*ublock)->count = count;
    uuids = (*ublock)->uuid;
    for (i=0; i < count; i++) 
    {
	uuids[i] = (uuid_p_t) malloc(sizeof(uuid_t));
	if (uuids[i] == NULL) 
	{
	    TSTDBG((DBG_FATAL, "Can't malloc memory for uuid\n"));
	    *st = -1;
	    return;
	}
    }
    return;
}

/* dcd_st_load_uuid_vec
 *
 *   Load a uuid_vector with new uuids
 */
void
dcd_st_load_uuid_vec(
    uuid_vector_t *ublock,  /* in, out */
    error_status_t *st      /* out */
)
{
    unsigned32 count,i;
    uuid_p_t *uuids;
    
    count = ublock->count;
    uuids = ublock->uuid;
    for (i=0; i<count; i++)
    {
        uuid_create(uuids[i], st);
        TST_CK_DCE_STATUS(*st, DBG_FATAL, "uuid_create", CHK_RET);
    }
    return;
}

void
dcd_st_free_uuid_vec(
     uuid_vector_t        *ublock,          /* in, out */
     error_status_t       *st                /* out */
)
{
    int i;
    uuid_p_t *uuids;  /* start of UUID pointer array */
    
    *st = error_status_ok;
    uuids = ublock->uuid;
    for (i=0; i<ublock->count; i++) 
    {
	free(uuids[i]);
    }
    free(ublock);
    return;
}


