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
 * $Log: acl_test_server.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:52  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:06:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:41  root]
 * 
 * Revision 1.1.2.5  1994/06/10  20:18:28  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:57:51  devsrc]
 * 
 * Revision 1.1.2.4  1994/06/07  14:44:05  bowe
 * 	Added rpc_sm_enable_allocate(). [CR 10757]
 * 	[1994/06/07  14:37:52  bowe]
 * 
 * Revision 1.1.2.3  1994/05/26  00:08:01  baum
 * 	Changes to allow server to run on HP, (removed close_databases for now which segfaults)
 * 	[1994/05/26  00:07:47  baum]
 * 
 * Revision 1.1.2.2  1994/05/19  14:34:49  baum
 * 	Resync tests with ACL & DB API changes
 * 	[1994/05/19  14:34:13  baum]
 * 
 * Revision 1.1.2.1  1994/04/22  06:19:31  baum
 * 	Initial Checkin
 * 	[1994/04/22  06:18:48  baum]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <dce/dce.h>
#include <dce/uuid.h>
#include <dce/dce_msg.h>
#include <dce/dbif.h>
#include <dce/dceacl.h>
#include <dce/aclif.h>
#include <dce/rdaclifv0.h>
#include <dce/pgo.h>
#include <sys/utsname.h>

#include "acl_test_db.h"
#include "acl_test_utils.h"

#define OWNER_PERMS \
    ( sec_acl_perm_read | sec_acl_perm_write | sec_acl_perm_test | \
    sec_acl_perm_control | sec_acl_perm_delete )

char	*dbname="Database.db";
char	*dbname_i="index.db";
char	*dbname_acl="Acl.db";
int	verbose;

dce_db_handle_t		db_h, db_ih, db_acl;

void open_databases();
void close_databases();
void add_default_entries_on_startup();
uuid_t add_entry ( acl_test_data_t *data,  error_status_t *st );

uuid_t acl_test_object1_acl_uuid, acl_test_object2_acl_uuid;

dce_db_header_t	null_header;

#define OBJ_UUID  (unsigned_char_t *)"002a0882-9a3e-1cc5-be9e-0000c09ce054"

#define AllocAndDup(s) strcpy(rpc_ss_allocate(strlen(s)+1), s)

uuid_t	i_obj_acl_uuid = { /* 0046934f-6aee-1cf2-8d46-0000c09ce054 */
	0x0046934f, 0x6aee, 0x1cf2, 0x8d, 0x46,
	{0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54} };
uuid_t	db_acl_uuid = { /* 0046934f-6aef-1cf2-8d46-0000c09ce054 */
	0x0046934f, 0x6aef, 0x1cf2, 0x8d, 0x46,
	{0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54} };
unsigned_char_t		srv_name[80];

char	*principal;
char	*pwd;

/* ______________________________________________________________________ */

sec_acl_printstring_t acl_test_info = {
    "acl_test",	"ACL Test Manager"
};

sec_acl_printstring_t acl_test_printstr[] = {
    { "r",	"read",		sec_acl_perm_read	},
    { "w",	"write",	sec_acl_perm_write	},
    { "d",	"delete",	sec_acl_perm_delete	},
    { "c",	"control",	sec_acl_perm_control	},
    { "t",	"test",		sec_acl_perm_test	},
    { "D",	"dump",		sec_acl_perm_unused_00000080	}
};

/* ______________________________________________________________________ */

uuid_t acl_test_uuid = INIT_ACL_TEST_UUID;

extern rdaclif_v0_0_epv_t dce_acl_v0_0_epv;

void 
setup_server(unsigned_char_t *, uuid_t, unsigned_char_t *, unsigned_char_t *,
    rpc_if_handle_t, unsigned_char_t *, boolean32,
    boolean32 (*xx)(), rpc_binding_vector_t **, error_status_t *);

/* ______________________________________________________________________ */

boolean32 acl_test_auth ( rpc_binding_handle_t h, unsigned32 m, error_status_t *st)
{	return TRUE;	}

dce_acl_resolve_func_t
acl_test_resolve_by_name
(
 handle_t			h,
 sec_acl_component_name_t 	name,
 sec_acl_type_t			sec_acl_type,
 uuid_t				*manager_type,
 boolean32			writing,
 void				*resolver_arg,
 uuid_t				*acl_uuid,
 error_status_t			*st
)
{
  uuid_t		u, *up;
  acl_test_data_t		data;
  
  *st = error_status_ok;
  if  ( !name || ! *name ) 	
  {
    up = &db_acl_uuid;
  }
  else 	
  {
    dce_db_fetch_by_name ( db_ih, name, (void *)&u, st );
    if  ( *st != error_status_ok )
      return;
    up = &u;
  }
  
  dce_db_fetch_by_uuid ( db_h, up, (void *)&data, st );
  if  ( *st != error_status_ok )
    return;
  *acl_uuid = data.h.tagged_union.h.acl_uuid; 
}

dce_acl_resolve_func_t
acl_test_resolve_by_name2
(
 handle_t			h,
 sec_acl_component_name_t 	name,
 sec_acl_type_t			sec_acl_type,
 uuid_t				*manager_type,
 boolean32			writing,
 void				*resolver_arg,
 uuid_t				*acl_uuid,
 error_status_t			*st
)
{
  uuid_t		u, *up;
  acl_test_data_t		data;
  
  *st = error_status_ok;
  if  ( !name || ! *name ) 	
  {
    up = &db_acl_uuid;
  }
  else 	
  {
    dce_db_fetch_by_name ( db_ih, name, (void *)&u, st );
    if  ( *st != error_status_ok )
      return;
    up = &u;
  }
  
  dce_db_fetch_by_uuid ( db_h, up, (void *)&data, st );
  if  ( *st != error_status_ok )
    return;
  *acl_uuid = data.h.tagged_union.h.acl_uuid; 
}

/* ______________________________________________________________________ */

int
main ( int argc, char **argv )
{
  error_status_t		st;
  rpc_binding_vector_t		*bind_vec;
  char				*myhost;
  int				init_acl, ret;
  unsigned_char_t 		*prin;
  struct utsname		info;

  if ( argc != 3 )
  {
    printf ( "Usage: acl_test_server principal password\n" );
    exit ( -1 );
  }

  principal = argv[1];
  pwd = argv[2];
  dce_msg_define_msg_table ( dce_lib_g_table, 
			     sizeof dce_lib_g_table / sizeof(dce_msg_table_t), 
			     &st );
  CHECK_STATUS( st, "dce_msg_define_msg_table", CHK_EXIT );
  
  /*
   * DCE login as ACL_TEST_PRINC1
   */
  tst_dce_login ( principal, pwd, &st );
  CHECK_STATUS ( st, "tst_dce_login", CHK_EXIT );

  open_databases ( &init_acl, &st );

  uname ( &info );
  sprintf ( srv_name, ACL_TEST_SERVER_CDS_ENTRY, info.nodename );
  
  dce_acl_register_object_type ( db_acl, &acl_test_uuid,
		       sizeof acl_test_printstr / sizeof acl_test_printstr[0],
		       acl_test_printstr, &acl_test_info, sec_acl_perm_control,
		       sec_acl_perm_test,
		       (void *)acl_test_resolve_by_name, NULL, 0, &st );
  CHECK_STATUS ( st, "dce_acl_register_object_type (1)", CHK_EXIT );
  
  if ( init_acl )
    initialize_acls ( &st );
  
  setup_server ( principal, acl_test_uuid, KEYTAB, srv_name, 
		 acl_test_v1_0_s_ifspec,(unsigned_char_t *)"Big Red Server", 
		 TRUE, acl_test_auth, &bind_vec, &st );
  
  rpc_mgmt_inq_server_princ_name ( NULL, rpc_c_authn_default, &prin, &st );
  CHECK_STATUS ( st, "rpc_mgmt_inq_server_princ_name", CHK_EXIT );

  add_default_entries_on_startup();

  fprintf ( stdout, "Server listening...\n" ); fflush(stdout);
  rpc_server_listen ( rpc_c_listen_max_calls_default, &st );
  CHECK_STATUS ( st, "rpc_server_listen", CHK_EXIT );
  
  takedown_server ( acl_test_uuid, &bind_vec, srv_name, acl_test_v1_0_s_ifspec, 
		    &st );
  
/*  close_databases ( &st ); */
  fprintf ( stdout, "SERVER DONE OK\n" ); fflush(stdout);
  
  return 0;
}

/* ______________________________________________________________________ */

void
open_databases
(
 int		*init_acl,
 error_status_t	*st
)
{
  extern void		acl_convert();
  int	ret;

  *init_acl = 1;

  ret = unlink ( dbname );
  if ( ret != 0 && errno != ENOENT )
  {
    fprintf ( stdout, "PROBLEM:Could not remove existing DB files\n" );
    fflush ( stdout );
    exit ( -1 );
  }

  ret = unlink ( dbname_i );
  if ( ret != 0 && errno != ENOENT )
  {
    fprintf ( stdout, "PROBLEM:Could not remove existing DB files\n" );
    fflush ( stdout );
    exit ( -1 );
  }

  ret = unlink ( dbname_acl );
  if ( ret != 0 && errno != ENOENT )
  {
    fprintf ( stdout, "PROBLEM:Could not remove existing DB files\n" );
    fflush ( stdout );
    exit( -1 );
  }

  dce_db_open ( dbname, NULL, db_c_create|db_c_index_by_uuid|db_c_std_header,
	        (dce_db_convert_func_t)acl_test_data_convert, &db_h, st );
  CHECK_STATUS ( *st, "dce_db_open - main", CHK_EXIT );
  
  dce_db_open ( dbname_i, NULL, db_c_create|db_c_index_by_name,
	        (dce_db_convert_func_t)uu_convert, &db_ih, st );
  CHECK_STATUS ( *st, "dce_db_open - index", CHK_EXIT );
  
  dce_db_open ( dbname_acl, NULL,
	        db_c_create|db_c_index_by_uuid,
	        (dce_db_convert_func_t)dce_rdacl_convert, &db_acl, st );
  CHECK_STATUS ( *st, "dce_db_open - acl", CHK_EXIT );

  rpc_sm_enable_allocate( st );

}

/* 
 * When creating the databases, initialize the initial ACLs. 
 */
initialize_acls 
( 
 error_status_t *st 
) 
{
  sec_acl_t		i_obj_acl;
  sec_acl_id_t		realm;
  sec_acl_entry_t	ents[5], *e;
  sec_rgy_handle_t	rh;
  unsigned_char_t	*cellname;
  uuid_t		celluuid, u;
  
  dce_acl_obj_init ( &acl_test_uuid, &i_obj_acl, st );
  
  dce_cf_get_cell_name ( &cellname, st );
  CHECK_STATUS ( *st, "dce_cf_get_cell_name - initial obj acl", CHK_EXIT );

  sec_rgy_site_open ( cellname, &rh, st );
  CHECK_STATUS ( *st, "sec_rgy_site_open - initial obj acl", CHK_EXIT );

  sec_rgy_pgo_name_to_id ( rh, sec_rgy_domain_person, principal,
			   &u, st );
  CHECK_STATUS ( *st, "sec_rgy_pgo_name_to_id - acl_test owner", CHK_RET );

  dce_acl_obj_add_user_entry ( &i_obj_acl, OWNER_PERMS, &u, st );
  CHECK_STATUS ( *st, "dce_acl_obj_add_user_entry - i_obj", CHK_EXIT );

  sec_rgy_site_close ( rh, st );
  
  dce_acl_obj_add_any_other_entry ( &i_obj_acl,  sec_acl_perm_read |
				    sec_acl_perm_test, st );
  CHECK_STATUS ( *st, "dce_acl_obj_add_any_other_entry - i_obj", CHK_EXIT );

  dce_acl_obj_add_unauth_entry ( &i_obj_acl, sec_acl_perm_test |
				 sec_acl_perm_read , st );
  CHECK_STATUS ( *st, "dce_acl_obj_add_unauth_entry - i_obj", CHK_EXIT );
  
  dce_db_store_by_uuid ( db_acl, &i_obj_acl_uuid, &i_obj_acl, st );
  CHECK_STATUS ( *st, "dce_db_store_by_uuid - i_obj", CHK_EXIT );

  dce_db_store_by_uuid ( db_acl, &db_acl_uuid, &i_obj_acl, st );
  CHECK_STATUS ( *st, "dce_db_store_by_uuid - initial obj acl", CHK_EXIT );

}

void
close_databases
(
 error_status_t	*st
)
{
  if  ( db_h ) 
  {
    dce_db_close ( &db_h, st );
    CHECK_STATUS ( *st, "dce_db_close - main", CHK_CONT );
  }

  if  ( db_ih ) 
  {
    dce_db_close ( &db_ih, st );
    CHECK_STATUS ( *st, "dce_db_close - index", CHK_CONT );
  }

  if  ( db_acl ) 
  {
    dce_db_close ( &db_acl, st );
    CHECK_STATUS ( *st, "dce_db_close - acl", CHK_CONT );
  }

}

/*********************** FVT Backends ****************/

/*
 * This RPC implements the ACL FVT 'acl_inq_client_creds_ok'.  It is called
 * and returns its results to the acl_inq_client_creds_ok() routine found
 * in dce_acl_perm_fcns.c 
 */
void
acl_inq_client_creds_ok_fvt
(
 handle_t	h,
 uuid_t		client_principal,
 error_status_t	*p_status
)
{

  rpc_authz_cred_handle_t	creds;
  error_status_t		st;

  *p_status = error_status_ok;

  /*
   * Get the Creds
   */
  dce_acl_inq_client_creds ( h, &creds, p_status );
  CHECK_STATUS( *p_status, "dce_acl_inq_client_pac", CHK_RET );

  /*
   * Creds are opaque, no easy way to look into the structure.  Sec_creds()
   * calls can be made to interrogate the structure.  TBD
   */

  fprintf ( stdout, "Success\n" );
  fflush ( stdout );
  return;

}

/*
 * This RPC implements the ACL FVT 'acl_inq_client_permset_ok'.  It is called
 * and returns its results to the acl_inq_client_permset_ok() routine found
 * in dce_acl_perm_fcns.c 
 */
void
acl_inq_client_permset_ok_fvt
(
 handle_t	h,
 error_status_t	*p_status
)
{

  error_status_t	st;
  sec_acl_permset_t	perms;
  uuid_t		client_principal, client_group;

  *p_status = error_status_ok;

  /*
   * Test the client perms.  Client principal ACL_TEST_PRINC1 should
   * have OWNER_PERMS priviledge to 'acl_test_object1'
   */
  dce_acl_inq_prin_and_group ( h, &client_principal, &client_group, p_status );
  CHECK_STATUS( *p_status, "dce_acl_inq_prin_and_group", CHK_RET );

  dce_acl_inq_client_permset ( h, &acl_test_uuid, &acl_test_object1_acl_uuid,
			       &client_principal, &client_group, &perms, 
			       p_status );
  CHECK_STATUS( *p_status, "dce_acl_inq_client_permset", CHK_RET );

  /*
   * Check something to make sure the call did what it was supposed to do.
   * Should we look into the creds structure? (TBD)
   */
  if ( perms != OWNER_PERMS )
  {
    fprintf ( stdout, "PROBLEM:Perms for client principal != OWNER_PERMS\n" );
    fflush ( stdout );
    return;
  }
    
  fprintf ( stdout, "Success\n" );
  fflush ( stdout );
  return;

}

/*
 * This RPC implements the ACL FVT 'acl_is_client_authorized_ok'.  It is 
 * called and returns its results to the acl_is_client_authorized_ok() 
 * routine found in dce_acl_perm_fcns.c 
 */
void
acl_is_client_authorized_ok_fvt
(
 handle_t	h,
 error_status_t	*p_status
)
{

  error_status_t	st;
  sec_acl_permset_t	desired_perms = OWNER_PERMS;
  boolean32		authorized;
  uuid_t		client_principal, client_group;

  *p_status = error_status_ok;

  /*
   * Test the that the client principal (ACL_TEST_PRINC1) should
   * have OWNER_PERMS priviledge to 'acl_test_object1'
   */
  dce_acl_inq_prin_and_group ( h, &client_principal, &client_group, p_status );
  CHECK_STATUS( *p_status, "dce_acl_inq_prin_and_group", CHK_RET );

  dce_acl_is_client_authorized ( h, &acl_test_uuid, &acl_test_object1_acl_uuid,
				 &client_principal, &client_group, 
				 desired_perms, &authorized, p_status );
  CHECK_STATUS( *p_status, "dce_acl_is_client_authorized", CHK_RET );

  if ( authorized == FALSE )
  {
    fprintf ( stdout, "PROBLEM:Client principal does not have OWNER_PERMS\n" );
    fflush ( stdout );
    return;
  }
    
  fprintf ( stdout, "Success\n" );
  fflush ( stdout );
  return;

}

/*
 * This RPC implements the ACL FVT 'acl_inq_client_prin_and_group_ok'.  It is 
 * called and returns its results to the acl_inq_client_prin_and_group_ok() 
 * routine found in dce_acl_perm_fcns.c 
 */
void
acl_inq_client_p_and_g_ok_fvt
(
 handle_t	h,
 uuid_t		client_principal,
 uuid_t		client_group,
 error_status_t	*p_status
)
{
  uuid_t		loc_client_principal, loc_client_group;
  unsigned		st;

  *p_status = error_status_ok;

  /*
   * Get the prin and group locally and compare with sent by
   * the client.
   */
  dce_acl_inq_prin_and_group ( h, &loc_client_principal, &loc_client_group, 
			       p_status );
  CHECK_STATUS( *p_status, "dce_acl_inq_prin_and_group", CHK_RET );

  if ( (uuid_equal ( &client_principal, &loc_client_principal, &st) != TRUE) ||
       (uuid_equal ( &client_group, &loc_client_group, &st) != TRUE) )
  {
    fprintf ( stdout, "PROBLEM:Client principal or groups do not match\n" );
    fflush ( stdout );
    return;
  }
    
  fprintf ( stdout, "Success\n" );
  fflush ( stdout );
  return;
}

/*
 * This RPC implements the ACL FVT 'acl_inq_permset_for_creds_ok'.  It 
 * is called and returns its results to the acl_inq__permset_for_creds_ok() 
 * routine found in dce_acl_perm_fcns.c 
 */
void
acl_inq_prmset_for_creds_ok_fvt
(
 handle_t	h,
 error_status_t	*p_status
)
{

  rpc_authz_cred_handle_t	creds;
  sec_acl_t			acl;
  sec_acl_permset_t		perms;
  error_status_t		st;
  uuid_t			client_principal, client_group;

  *p_status = error_status_ok;

  /*
   * Get the Creds
   */
  dce_acl_inq_client_creds ( h, &creds, p_status );
  CHECK_STATUS( *p_status, "dce_acl_inq_client_pac", CHK_RET );

  /*
   * Test the client perms.  Client principal ACL_TEST_PRINC1 should
   * have OWNER_PERMS priviledge to 'acl_test_object1'
   */
  dce_acl_inq_prin_and_group ( h, &client_principal, &client_group, p_status );
  CHECK_STATUS( *p_status, "dce_acl_inq_prin_and_group", CHK_RET );

  /*
   * Get ACL from DB
   */
  dce_db_fetch_by_uuid ( db_acl, &acl_test_object1_acl_uuid, &acl, p_status );
  CHECK_STATUS( *p_status, "dce_db_fetch_by_uuid", CHK_RET );

  /*
   * Do the test
   */
  dce_acl_inq_permset_for_creds ( creds, &acl, &client_principal, 
				  &client_group, 0, &perms,  p_status );
  CHECK_STATUS( *p_status, "dce_acl_inq_permset_for_creds", CHK_RET );

  /*
   * Check something to make sure the call did what it was supposed to do.
   * Should we look into the creds structure? (TBD)
   */
  if ( perms != OWNER_PERMS )
  {
    fprintf ( stdout, "PROBLEM:Perms for client principal != OWNER_PERMS\n" );
    fflush ( stdout );
    return;
  }
    
  fprintf ( stdout, "Success\n" );
  fflush ( stdout );
  return;

}

uuid_t
add_entry
(
  acl_test_data_t		*data,
  error_status_t	*st
)
{
  uuid_t		u, au;
  dce_db_dataheader_t	*shdr;
  rpc_authz_handle_t	privs;
  unsigned_char_t	*prin_name;
  unsigned32		protect_level, authn_svc, authz_svc;
  unsigned_char_t	*ucp;
  sec_acl_t		i_obj_acl;

  *st = error_status_ok;

  /* get initial obj acl */
  dce_db_fetch_by_uuid ( db_acl, &i_obj_acl_uuid, &i_obj_acl, st );
  CHECK_STATUS ( *st, "dce_db_fetch_by_uuid - get initial obj acl", CHK_RET );

  uuid_create ( &u, st );	/* uuid of the db entry */
  uuid_create ( &au, st );	/* uuid of the acl */
  if  ( *st != error_status_ok ) 
    return;

#ifdef GONE
    dce_acl_obj_add_user_entry ( &i_obj_acl, OWNER_PERMS, &u, st );
#endif
  
  dce_db_store_by_uuid ( db_acl, &au, &i_obj_acl, st );
  CHECK_STATUS ( *st, "dce_db_store_by_uuid - acl", CHK_RET );
  
  dce_db_std_header_init ( db_h, &(data->h), &u, &au, &au, &db_acl_uuid, 0, st );
  CHECK_STATUS ( *st, "dce_db_std_header_init", CHK_CONT);
  
  dce_db_store_by_uuid ( db_h, &u, data, st );
  CHECK_STATUS ( *st, "dce_db_store_by_uuid - main", CHK_CONT );

  dce_db_store_by_name ( db_ih, data->ph.st1, &u, st );
  CHECK_STATUS ( *st, "dce_db_store_by_uuid - index", CHK_CONT );

  return ( au );
}

void
add_default_entries_on_startup()
{

  acl_test_data_t		data;
  error_status_t	st;

  data.h = null_header;

  data.ph.st1 = OBJ1;
  data.ph.st2 = "obj1_st2";
  data.ph.st3 = "obj1_st3";
  data.ph.st4 = "obj1_st4";

  acl_test_object1_acl_uuid = add_entry ( &data, &st );

  data.ph.st1 = OBJ2;
  data.ph.st2 = "obj2_st2";
  data.ph.st3 = "obj2_st3";
  data.ph.st4 = "obj2_st4";

  acl_test_object2_acl_uuid = add_entry ( &data, &st );


}
