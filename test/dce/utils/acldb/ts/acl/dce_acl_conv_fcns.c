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
 * $Log: dce_acl_conv_fcns.c,v $
 * Revision 1.1.4.2  1996/02/18  00:23:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:07:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:46  root]
 * 
 * Revision 1.1.2.4  1994/10/02  21:27:39  bowe
 * 	Shup up compiler warnings [CR 12363]
 * 	[1994/10/02  21:27:24  bowe]
 * 
 * Revision 1.1.2.3  1994/06/10  20:18:32  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:57:54  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/19  14:34:52  baum
 * 	Resync tests with ACL & DB API changes
 * 	[1994/05/19  14:34:16  baum]
 * 
 * Revision 1.1.2.1  1994/04/22  06:19:43  baum
 * 	Initial Checkin
 * 	[1994/04/22  06:18:52  baum]
 * 
 * $EndLog$
 */
/*
 * Module: 	dce_acl_conv_fcns.c
 *
 * Facility:	DCE ACL Convenience Function Implementations (No Server needed)
 *
 * Abstract:    This module implements the tests for the DCE ACL Library
 */

/*
 * Include files
 */
#include <acl_test_utils.h>

uuid_t	mgr_uuid = { /* 008167f9-7d33-1db4-a28a-0000c0dc0d4b */
        0x008167f9,
        0x7d33,
        0x1db4,
        0xa2,
        0x8a,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

sec_acl_printstring_t info = { "name","Help String" };

sec_acl_printstring_t printstr[] = { { "r", "read", sec_acl_perm_read },
				     { "c", "control", sec_acl_perm_control },
				     { "t", "test", sec_acl_perm_test } };


sec_acl_permset_t	perms[3] = { sec_acl_perm_read, sec_acl_perm_test,
				       sec_acl_perm_control };

sec_acl_entry_type_t	type[3] = { sec_acl_e_type_user, sec_acl_e_type_group,
				       sec_acl_e_type_foreign_other };

uuid_t		id[3] = {{ /* 003091b7-881d-1db4-a6a0-0000c0dc0d4b */
        0x003091b7, 0x881d, 0x1db4, 0xa6, 0xa0,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b} },
				 { /* 005850f3-8dfc-1db4-bfdf-0000c0dc0d4b */
        0x005850f3, 0x8dfc, 0x1db4, 0xbf, 0xdf,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b} },
				 { /* 0039a6e9-8e0d-1db4-a2b4-0000c0dc0d4b */
        0x0039a6e9, 0x8e0d, 0x1db4, 0xa2, 0xb4,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b} } };


void 
setup ()
{
  unsigned32		dce_status;

  /*
   * DCE login as ACL_TEST_PRINC1
   */
  tst_dce_login ( ACL_TEST_PRINC1, ACL_TEST_PRINC1, &dce_status );
  if ( dce_status != error_status_ok )
  {
    int inq_status;						       
    unsigned char dce_error_buf [dce_c_error_string_len]; 		
    char big_buf [dce_c_error_string_len * 2]; 		
    
    dce_error_inq_text ( dce_status, dce_error_buf, &inq_status );
    sprintf ( big_buf, "Could not DCE login as %s (%s}\n", ACL_TEST_PRINC1,
	      dce_error_buf );
    OUTPUT( big_buf, TET_FAIL );
    return;
  }

}

void
cleanup()
{
}

/*
 * Function: 	acl_register_ok
 *
 * Abstract:	Register an ACL object manager.
 */

void
acl_register_ok()
{
  char	*p_test_name = "acl_register_ok";
  int	ret = 1;
  error_status_t	st;

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

 done:
  REPORT_RESULT2( ret );

}

/*
 * Function: 	acl_copy_ok
 *
 * Abstract:	Copy an ACL.
 */

void
acl_copy_ok()
{
  char	*p_test_name = "acl_copy_ok";
  int	ret = 1, i;
  error_status_t	st;
  sec_acl_t		acl, acl2;
  char			iteration[LONG_STRING_LEN];

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  for ( i = 0; i < 3; i++ )
  {
    dce_acl_obj_add_id_entry ( &acl, type[i], perms[i], &id[i], &st);
    sprintf ( iteration, "(dce_acl_obj_add_id_entry(%d)", i );
    CHECK_DCE_ERROR ( iteration, st );
  }

  dce_acl_copy_acl ( &acl, &acl2, &st );
  CHECK_DCE_ERROR ( "dce_acl_copy_acl" , st );
  
  for ( i = 0; i < 3; i++ )
  {
    if ( ACL_OK( acl2, i, perms[i], type[i], id[i] ) )
    {
      ret = 0;
      sprintf ( outbuf, "ACL entry #%d not correct!", i );
      break;
    }
  }

  dce_acl_obj_free_entries ( &acl, &st );
  dce_acl_obj_free_entries ( &acl2, &st );

 done:
  REPORT_RESULT2( ret );

  
}

/*
 * Function: 	acl_obj_init_ok
 *
 * Abstract:	Create an ACL in memory.
 */

void
acl_obj_init_ok()
{
  char	*p_test_name = "acl_obj_init_ok";
  int	ret = 1;
  error_status_t	st;
  sec_acl_t		acl;

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  if ( acl.num_entries != 0 )
  {
    ret = 0;
    sprintf ( outbuf, "ACL Num Entries not initialized!" );
  }

 done:
  REPORT_RESULT2( ret );
}

/*
 * Function: 	acl_obj_free_entries_ok
 *
 * Abstract:	Free an ACL.
 */

void
acl_obj_free_entries_ok()
{
  char	*p_test_name = "acl_obj_free_entries_ok";
  int	ret = 1, i;
  error_status_t	st;
  sec_acl_t		acl;
  char			iteration[LONG_STRING_LEN];

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  for ( i = 0; i < 3; i++ )
  {
    dce_acl_obj_add_id_entry ( &acl, type[i], perms[i], &id[i], &st);
    sprintf ( iteration, "(dce_acl_obj_add_id_entry(%d)", i );
    CHECK_DCE_ERROR ( iteration, st );
  }

  for ( i = 0; i < 3; i++ )
  {
    if ( ACL_OK( acl, i, perms[i], type[i], id[i] ) )
    {
      ret = 0;
      sprintf ( outbuf, "ACL entry #%d not correct!", i );
      break;
    }
  }

  dce_acl_obj_free_entries ( &acl, &st );

 done:
  REPORT_RESULT2( ret );

}

/*
 * Function: 	acl_obj_add_id_ok
 *
 * Abstract:	Add a user. group or foreign_other entry to an ACL.
 */

void
acl_obj_add_id_ok()
{
  char	*p_test_name = "acl_obj_add_id_ok";
  int	ret = 1;
  error_status_t	st;
  sec_acl_t		acl;

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  dce_acl_obj_add_id_entry ( &acl, type[0], perms[0], &id[0], &st);
  CHECK_DCE_ERROR ( "dce_acl_obj_add_id_entry", st );

  if ( ACL_OK( acl, 0, perms[0], type[0], id[0] ) )
  {
    ret = 0;
    sprintf ( outbuf, "ACL entry not correct!" );
  }

 done:
  REPORT_RESULT2( ret );
  
}

/*
 * Function: 	acl_obj_add_id_several_ok
 *
 * Abstract:	Add several ACL entries to an ACL, check them afterwards.
 */

void
acl_obj_add_id_several_ok()
{
  char	*p_test_name = "acl_obj_add_id_several_ok";
  int	ret = 1, i;
  error_status_t	st;
  sec_acl_t		acl;
  char			iteration[LONG_STRING_LEN];

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  for ( i = 0; i < 3; i++ )
  {
    dce_acl_obj_add_id_entry ( &acl, type[i], perms[i], &id[i], &st);
    sprintf ( iteration, "(dce_acl_obj_add_id_entry(%d)", i );
    CHECK_DCE_ERROR ( iteration, st );
  }

  for ( i = 0; i < 3; i++ )
  {
    if ( ACL_OK( acl, i, perms[i], type[i], id[i] ) )
    {
      ret = 0;
      sprintf ( outbuf, "ACL entry #%d not correct!", i );
      break;
    }
  }

 done:
  REPORT_RESULT2( ret );

}

/*
 * Function: 	acl_obj_add_id_bad_type
 *
 * Abstract:	Attempt to add a invalid ACL entry type.
 */

void
acl_obj_add_id_bad_type()
{
  char	*p_test_name = "acl_obj_add_id_bad_type";
  int	ret = 1;
  error_status_t	st;
  sec_acl_t		acl;

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  dce_acl_obj_add_id_entry ( &acl, 99, perms[0], &id[0], &st);
  CHECK_EXPECTED_STATUS ( "dce_acl_obj_add_id_entry", st, 
			  sec_acl_invalid_entry_type );

 done:
  REPORT_RESULT2( ret );
  
}

/*
 * Function: 	acl_obj_add_user_ok
 *
 * Abstract:	Add a user entry to an ACL.
 */
void
acl_obj_add_user_ok()
{
  char	*p_test_name = "acl_obj_add_user_ok";
  int	ret = 1;
  error_status_t	st;
  sec_acl_t		acl;

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  dce_acl_obj_add_user_entry ( &acl, perms[0], &id[0], &st);
  CHECK_DCE_ERROR ( "dce_acl_obj_add_id_entry", st );

  if ( ACL_OK( acl, 0, perms[0], sec_acl_e_type_user, id[0] ) )
  {
    ret = 0;
    sprintf ( outbuf, "ACL entry not correct!" );
  }

 done:
  REPORT_RESULT2( ret );
  
}

/*
 * Function: 	acl_obj_add_group_ok
 *
 * Abstract:	Add a group entry to an ACL.
 */
void
acl_obj_add_group_ok()
{
  char	*p_test_name = "acl_obj_add_group_ok";
  int	ret = 1;
  error_status_t	st;
  sec_acl_t		acl;

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  dce_acl_obj_add_group_entry ( &acl, perms[0], &id[0], &st);
  CHECK_DCE_ERROR ( "dce_acl_obj_add_id_entry", st );

  if ( ACL_OK( acl, 0, perms[0], sec_acl_e_type_group, id[0] ) )
  {
    ret = 0;
    sprintf ( outbuf, "ACL entry not correct!" );
  }

 done:
  REPORT_RESULT2( ret );
  
}

/*
 * Function: 	acl_obj_add_obj_ok
 *
 * Abstract:	Add unauth or any_other to an ACL.
 */
void
acl_obj_add_obj_ok()
{
  char	*p_test_name = "acl_obj_add_obj_ok";
  int	ret = 1;
  error_status_t	st;
  sec_acl_t		acl;

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  dce_acl_obj_add_obj_entry ( &acl, sec_acl_e_type_any_other, perms[0], &st);
  CHECK_DCE_ERROR ( "dce_acl_obj_add_obj_entry", st );

  if ( ACL_OBJ_OK( acl, 0, perms[0], sec_acl_e_type_any_other ) )
  {
    ret = 0;
    sprintf ( outbuf, "ACL entry not correct!" );
  }

 done:
  REPORT_RESULT2( ret );
  
}

/*
 * Function: 	acl_obj_add_obj_bad_type
 *
 * Abstract:	Attempt to add a invalid ACL entry type.
 */

void
acl_obj_add_obj_bad_type()
{
  char	*p_test_name = "acl_obj_add_obj_bad_type";
  int	ret = 1;
  error_status_t	st;
  sec_acl_t		acl;

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  dce_acl_obj_add_obj_entry ( &acl, 99, perms[0], &st);
  CHECK_EXPECTED_STATUS ( "dce_acl_obj_add_id_entry", st, 
			  sec_acl_invalid_entry_type );

 done:
  REPORT_RESULT2( ret );
  
}

/*
 * Function: 	acl_obj_add_unauth_ok
 *
 * Abstract:	Add unauth entry to an ACL.
 */
void
acl_obj_add_unauth_ok()
{
  char	*p_test_name = "acl_obj_add_unauth_ok";
  int	ret = 1;
  error_status_t	st;
  sec_acl_t		acl;

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  dce_acl_obj_add_unauth_entry ( &acl, perms[0], &st);
  CHECK_DCE_ERROR ( "dce_acl_obj_add_obj_entry", st );

  if ( ACL_OBJ_OK( acl, 0, perms[0], sec_acl_e_type_unauthenticated ) )
  {
    ret = 0;
    sprintf ( outbuf, "ACL entry not correct!" );
  }

 done:
  REPORT_RESULT2( ret );
  
}

/*
 * Function: 	acl_obj_add_other_ok
 *
 * Abstract:	Add other entry to an ACL.
 */
void
acl_obj_add_other_ok()
{
  char	*p_test_name = "acl_obj_add_other_ok";
  int	ret = 1;
  error_status_t	st;
  sec_acl_t		acl;

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  dce_acl_obj_add_any_other_entry ( &acl, perms[0], &st);
  CHECK_DCE_ERROR ( "dce_acl_obj_add_obj_entry", st );

  if ( ACL_OBJ_OK( acl, 0, perms[0], sec_acl_e_type_any_other ) )
  {
    ret = 0;
    sprintf ( outbuf, "ACL entry not correct!" );
  }

 done:
  REPORT_RESULT2( ret );
  
}

/*
 * Function: 	acl_obj_add_foreign_ok
 *
 * Abstract:	Add foreign entry to an ACL.
 */
void
acl_obj_add_foreign_ok()
{
  char	*p_test_name = "acl_obj_add_foreign_ok";
  int	ret = 1;
  error_status_t	st;
  sec_acl_t		acl;
  uuid_t		realm    = { /* 0096931c-c03a-1db6-bfb7-0000c0dc0d4b */
        0x0096931c,
        0xc03a,
        0x1db6,
        0xbf,
        0xb7,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  dce_acl_obj_add_foreign_entry ( &acl, sec_acl_e_type_foreign_user,
				  perms[0], &realm, &id[0], &st);
  CHECK_DCE_ERROR ( "dce_acl_obj_add_obj_entry", st );

  if ( ACL_FOREIGN_OK( acl, 0, perms[0], sec_acl_e_type_foreign_user,
		       id[0], realm ) )
  {
    ret = 0;
    sprintf ( outbuf, "ACL entry not correct!" );
  }

 done:
  REPORT_RESULT2( ret );
}

/*
 * Function: 	acl_obj_add_foreign_bad_type
 *
 * Abstract:	Add foreign entry to an ACL with bad type.
 */
void
acl_obj_add_foreign_bad_type()
{
  char	*p_test_name = "acl_obj_add_foreign_bad_type";
  int	ret = 1;
  error_status_t	st;
  sec_acl_t		acl;
  uuid_t		realm    = { /* 0096931c-c03a-1db6-bfb7-0000c0dc0d4b */
        0x0096931c,
        0xc03a,
        0x1db6,
        0xbf,
        0xb7,
        {0x00, 0x00, 0xc0, 0xdc, 0x0d, 0x4b}
      };

  dce_acl_register_object_type ( NULL, &mgr_uuid, 3, printstr, &info,
				 sec_acl_perm_control, sec_acl_perm_test,
				 NULL, NULL, 0, &st );
  CHECK_DCE_ERROR( "dce_acl_register_object_type", st );

  dce_acl_obj_init ( &mgr_uuid, &acl, &st );
  CHECK_DCE_ERROR ( "dce_acl_obj_init", st );

  dce_acl_obj_add_foreign_entry ( &acl, 99, perms[0], &realm, &id[0], &st);
  CHECK_EXPECTED_STATUS ( "dce_acl_obj_add_foreign_entry", st, 
			  sec_acl_invalid_entry_type );

 done:
  REPORT_RESULT2( ret );
}

#ifndef _USE_TET
main ()
{
  setup();
  acl_register_ok();
  acl_obj_init_ok();
  acl_obj_add_id_ok();
  acl_obj_add_id_several_ok();
  acl_obj_free_entries_ok();
  acl_obj_add_id_bad_type();
  acl_obj_add_user_ok();
  acl_obj_add_group_ok();
  acl_obj_add_obj_ok();
  acl_obj_add_obj_bad_type();
  acl_obj_add_unauth_ok();
  acl_obj_add_other_ok();
  acl_obj_add_foreign_ok(); 
  acl_obj_add_foreign_bad_type(); 
  acl_copy_ok();
  cleanup();
}
#else
void (*tet_startup)() = setup;   /* Load the start routine into TET */
void (*tet_cleanup)() = cleanup; /* Load the clean routine into TET */

struct tet_testlist tet_testlist[] =
{
  { acl_register_ok, 1 },
  { acl_obj_init_ok, 1 },
  { acl_obj_add_id_ok, 1 },
  { acl_obj_add_id_several_ok, 1 },
  { acl_obj_free_entries_ok, 1 },
  { acl_obj_add_id_bad_type, 1 },
  { acl_obj_add_user_ok, 1 },
  { acl_obj_add_group_ok, 1 },
  { acl_obj_add_obj_ok, 1 },
  { acl_obj_add_obj_bad_type, 1 },
  { acl_obj_add_unauth_ok, 1 },
  { acl_obj_add_other_ok, 1 },
  { acl_copy_ok, 1 },
 
  { (void (*)())NULL, 0 }
};
#endif
