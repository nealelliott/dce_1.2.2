/*
 * @OSF_COPYRIGHT@
 * 
 */
/*
 * HISTORY
 * $Log: svrUtils.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:07:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:51  root]
 * 
 * Revision 1.1.2.2  1994/06/01  21:27:34  baum
 * 	Changed so they will compile on RIOS
 * 	[1994/06/01  21:27:14  baum]
 * 
 * Revision 1.1.2.1  1994/04/22  06:19:57  baum
 * 	Initial Checkin
 * 	[1994/04/22  06:18:58  baum]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________
 *
 * Copyright (c) 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 * ______________________________________________________________________ 
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

#include <dce/uuid.h>
#include <dce/dceacl.h>
#include <dce/aclif.h>
#include <dce/rdaclifv0.h>

#include <acl_test_utils.h>

extern rdaclif_v0_0_epv_t dce_acl_v0_0_epv;

/* ______________________________________________________________________ */
/*  Do the initialization stuff for starting up a server.
 */

void
setup_server
(
 unsigned_char_t		*prin_name,
 uuid_t				mgr_type,
 unsigned_char_t		*keytab,
 unsigned_char_t		*servername,
 rpc_if_handle_t		ifspec,
 unsigned_char_t		*annotation,
 boolean32			use_rdacl,
 boolean32			(*ph_auth)(),
 rpc_binding_vector_t		**bind_vec,
 error_status_t			*st
)
{
  uuid_vector_t		obj_vec;
  unsigned32			i;
  
  obj_vec.count   = 1;
  obj_vec.uuid[0] = &mgr_type;
  
  rpc_server_register_auth_info(prin_name, rpc_c_authn_default, NULL,
				keytab, st);
  CHECK_STATUS ( *st, "rpc_server_register_auth_info", CHK_EXIT );
  
  rpc_server_register_if(ifspec, NULL, NULL, st);
  CHECK_STATUS(*st, "rpc_server_register_if", CHK_EXIT);
  if (use_rdacl) 
  {
    rpc_server_register_if(rdaclif_v0_0_s_ifspec, NULL,
			   (rpc_mgr_epv_t) &dce_acl_v0_0_epv, st);
    CHECK_STATUS(*st, "rpc_server_register_if (rdaclif)", CHK_EXIT);
  }
  
  rpc_server_use_all_protseqs(rpc_c_protseq_max_reqs_default, st);
  CHECK_STATUS(*st, "rpc_server_use_all_protseqs", CHK_EXIT);
  rpc_server_inq_bindings(bind_vec, st);
  CHECK_STATUS(*st, "rpc_server_inq_bindings", CHK_EXIT);
  
  rpc_ep_register(ifspec, *bind_vec, &obj_vec, annotation, st);
  CHECK_STATUS(*st, "rpc_ep_register", CHK_EXIT);

  if (use_rdacl) 
  {
    rpc_ep_register(rdaclif_v0_0_s_ifspec, *bind_vec, &obj_vec,
		    (unsigned_char_t *)"RDACL part of server", st);
    CHECK_STATUS(*st, "rpc_ep_register (rdaclif)", CHK_EXIT);
  }
  
  rpc_ns_binding_export(rpc_c_ns_syntax_default, servername,
			ifspec, *bind_vec, &obj_vec, st);
  CHECK_STATUS(*st, "rpc_ns_binding_export", CHK_EXIT);
  
  rpc_mgmt_set_authorization_fn(ph_auth, st);
}

/* ______________________________________________________________________ */

void
takedown_server
(
 uuid_t			mgr_type,	/* in */
 rpc_binding_vector_t	**bind_vec,	/* in */
 unsigned_char_t	*servername,	/* in */
 rpc_if_handle_t	ifspec,		/* in */
 error_status_t		*st		/* out */
)
{
  uuid_vector_t		obj_vec;
  
  obj_vec.count   = 1;
  obj_vec.uuid[0] = &mgr_type;
  
  rpc_ns_binding_unexport(rpc_c_ns_syntax_default, servername,
			  ifspec, &obj_vec, st);
  CHECK_STATUS(*st, "rpc_ns_binding_unexport", CHK_CONT);
  
  rpc_ep_unregister(ifspec, *bind_vec, &obj_vec, st);
  CHECK_STATUS(*st, "rpc_ep_unregister", CHK_CONT);
  
  rpc_binding_vector_free(bind_vec, st);
  CHECK_STATUS(*st, "rpc_binding_vector_free", CHK_CONT);
}

/* ______________________________________________________________________ */
