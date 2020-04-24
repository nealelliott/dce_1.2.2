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
 * $Log: acl_test_utils.h,v $
 * Revision 1.1.4.2  1996/02/18  23:03:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:25  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:06:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:42:42  root]
 * 
 * Revision 1.1.2.6  1994/06/10  20:18:29  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:57:53  devsrc]
 * 
 * Revision 1.1.2.5  1994/06/01  21:27:32  baum
 * 	Changed so they will compile on RIOS
 * 	[1994/06/01  21:27:13  baum]
 * 
 * Revision 1.1.2.4  1994/05/25  15:50:59  baum
 * 	Changes to allow comp[ilation on HP
 * 	[1994/05/25  15:50:30  baum]
 * 
 * Revision 1.1.2.3  1994/05/19  14:46:33  baum
 * 	Resync with DB & ACL API changes
 * 	[1994/05/19  14:46:05  baum]
 * 
 * Revision 1.1.2.2  1994/04/22  19:34:39  baum
 * 	Changed OUTPUT macro to not always do tet_result
 * 	[1994/04/22  19:28:36  baum]
 * 
 * Revision 1.1.2.1  1994/04/22  06:19:36  baum
 * 	Initial Checkin
 * 	[1994/04/22  06:18:50  baum]
 * 
 * $EndLog$
 */
#ifndef __ACL_TEST_UTILS_H__
#define __ACL_TEST_UTILS_H__

#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <tet_api.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/sec_login.h>
#include <dce/binding.h>
#include <dce/rpcexc.h>

#include "acl_test.h"

/*
 * Global variables for convenience used by most modules.
 */
#define LONG_STRING_LEN 		240
char outbuf[dce_c_error_string_len + LONG_STRING_LEN]; 
char actual_output[dce_c_error_string_len + LONG_STRING_LEN];

int	tet_return;

#ifndef _USE_TET
#define ACL_TEST_PRINC1 		"test_princ1"
#define CELL_ADMIN			"cell_admin"
#define CELL_ADMIN_PWD			"-dce-"
#else
#define ACL_TEST_PRINC1			tet_getvar ( "T_ACL_TEST_PRINC1" )
#define CELL_ADMIN			tet_getvar ( "T_CELL_ADMIN" )
#define CELL_ADMIN_PWD			tet_getvar ( "T_CELL_ADMIN_PWD" )
#endif

#define ACL_TEST_PRINC2			CELL_ADMIN

#define TEST_SERVER 			"acl_test_server"
#define ACL_TEST_SERVER_CDS_ENTRY	"/.:/hosts/%s/acl_test"
#define OBJ1				"acl_test_object1"
#define OBJ2				"acl_test_object2"
#define ACL_TEST_OBJECT1          	ACL_TEST_SERVER_CDS_ENTRY "/" OBJ1
#define ACL_EDIT_LIST			"acl_edit %s -l"
#define ACL_EDIT_MODIFY		        "acl_edit %s -m user:%s:rw"
#define PRINC1_PERMS_STRING		"user:%s:rwdct-"
#define PRINC2_PERMS_STRING		"user:%s:rw----"
#define ACL_EDIT_DELETE			"acl_edit %s -d user:%s:rw"
#define ACL_EDIT_GRANT			"echo \"g\" | acl_edit %s"
#define ACL_EDIT_TEST_READ		"echo \"test r\" | acl_edit %s"
#define ACL_EDIT_TEST_DUMP		"echo \"test D\" | acl_edit %s"
#define ACL_EDIT_TEST_WRITE		"echo \"test w\" | acl_edit %s"

#define ACCEPT_ANY_LINE			"++ACCEPT ANY LINE++"

#define INIT_ACL_TEST_UUID 						\
{ /* 001b49b4-180f-1cf2-85f1-0000c09ce054 */				\
	0x001b49b4, 0x180f, 0x1cf2, 0x85, 0xf1,				\
	{0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54} }


/**
 ** Macros Used by Clients
 **/
#define CHECK_DCE_ERROR( m_api_name, m_dce_status )			\
  if ( m_dce_status != error_status_ok )				\
  {									\
    int inq_status;							\
    unsigned char dce_error_buf [dce_c_error_string_len]; 		\
                                                                        \
    dce_error_inq_text ( m_dce_status, dce_error_buf, &inq_status );	\
    sprintf (outbuf, "API Failed (%s):%s\n", m_api_name, 		\
	     dce_error_buf);						\
    ret = 0;								\
    goto done;								\
  }

#define CHECK_EXPECTED_STATUS( m_api_name, m_dce_status, m_exp_status ) \
  if ( m_dce_status != m_exp_status )					\
  {									\
    int inq_status;							\
    unsigned char dce_error_buf [dce_c_error_string_len]; 		\
    unsigned char dce_error_buf2 [dce_c_error_string_len]; 		\
                                                                        \
    dce_error_inq_text ( m_dce_status, dce_error_buf, &inq_status );	\
    dce_error_inq_text ( m_exp_status, dce_error_buf2, &inq_status );	\
    sprintf (outbuf, "Failed.  Expected: [%s:%s], received [%s:%s]",	\
	     m_api_name, dce_error_buf2, m_api_name, dce_error_buf );	\
    tet_return = TET_FAIL;						\
    ret = 0;								\
    goto done;								\
  }		

#ifdef _USE_TET
#define OUTPUT( m_str, m_tet_return )					\
{									\
  if ( m_tet_return != -1 )						\
    tet_result ( tet_return );						\
  tet_infoline ( m_str );						\
}
#else
#define OUTPUT( m_str, m_unused )					\
  printf ( m_str );						       
#endif


#define REPORT_RESULT( m_ret )						\
{									\
  if ( m_ret == 1 )							\
  {									\
    tet_return = TET_PASS;						\
    sprintf ( outbuf, "%s: Success\n", p_test_name );			\
  }									\
  else									\
  {									\
    tet_return = TET_FAIL;						\
    sprintf ( outbuf, "%s: Failed [%s]\n", 				\
	      p_test_name, actual_output );				\
  }									\
  OUTPUT( outbuf, tet_return );						\
}

#define REPORT_RESULT2( m_ret )						\
{									\
  char outbuf2[dce_c_error_string_len + LONG_STRING_LEN];		\
  if ( m_ret == 1 )							\
  {									\
    tet_return = TET_PASS;						\
    sprintf ( outbuf2, "%s: Success\n", p_test_name );			\
  }									\
  else									\
  {									\
    tet_return = TET_FAIL;						\
    sprintf ( outbuf2, "%s: Failed [%s]\n", p_test_name, outbuf );	\
  }									\
  OUTPUT( outbuf2, tet_return );					\
}

#define CHECK_CHILD_ERROR( m_ret )					\
  if ( m_ret != 1 )							\
    goto done;		

#define ACL_OK( m_acl, m_i, m_perms, m_type, m_id )			\
  m_acl.sec_acl_entries[m_i].perms != m_perms ||			\
  m_acl.sec_acl_entries[m_i].entry_info.entry_type != m_type ||		\
  ( uuid_equal (&m_acl.sec_acl_entries[m_i].entry_info.tagged_union.id.uuid, \
		&m_id, &st ) == FALSE )

#define ACL_FOREIGN_OK( m_acl, m_i, m_perms, m_type, m_id, m_realm )	\
  m_acl.sec_acl_entries[m_i].perms != m_perms ||			\
  m_acl.sec_acl_entries[m_i].entry_info.entry_type != m_type ||		\
  ( uuid_equal (&m_acl.sec_acl_entries[m_i].entry_info.tagged_union.foreign_id.id.uuid, \
		&m_id, &st ) == FALSE ) ||				\
  ( uuid_equal (&m_acl.sec_acl_entries[m_i].entry_info.tagged_union.foreign_id.realm.uuid, \
		&m_realm, &st ) == FALSE )

#define ACL_OBJ_OK( m_acl, m_i, m_perms, m_type )			\
  m_acl.sec_acl_entries[m_i].perms != m_perms ||			\
  m_acl.sec_acl_entries[m_i].entry_info.entry_type != m_type
  
/**
 ** MACROS Used by acl_test_server
 **/

#define KEYTAB_FILE "acl_test.keytab"
#define KEYTAB "FILE:" KEYTAB_FILE

#define CHK_CONT	0
#define CHK_EXIT	1
#define CHK_RET		2

#define CHECK_STATUS( m_st, m_s, m_x )					\
 if ( ( m_st ) != error_status_ok )					\
 { 									\
   fprintf ( stdout, "PROBLEM:%s: ", m_s );				\
   dce_fprintf ( stdout, (m_st) );					\
   fflush ( stdout );							\
   if ( m_x == CHK_EXIT ) 						\
     exit( -1 );							\
  else									\
    if ( m_x == CHK_RET )						\
      return; 								\
 }

#define CHECK_AND_REPORT( m_st, m_x )					\
  if ( ( m_st ) != error_status_ok ) 					\
  { 									\
    fprintf ( stdout, "PROBLEM:" );					\
    dce_fprintf ( stdout, ( m_st ) ); 					\
   fflush ( stdout );							\
    if ( m_x == CHK_EXIT )						\
      exit( -1 ); 							\
  }

extern int
get_binding_to_test_server
(
 handle_t	*p_hnd
);

extern int
start_acl_test_server
(
 FILE	**pp_svr_hnd,
 char	*principal,
 char	*pwd
);

extern int
stop_acl_test_server
(
 FILE	*p_svr_hnd
);

extern int
child_start
(
 FILE	**pp_child_hnd,
 char	*p_child_cmd,
 char	*p_mode,
 char	*p_expected_output,
 char	*p_actual_output
);

extern int
child_wait
(
 FILE	*hnd,
 char	*p_expected_output,
 char	*p_actual_output
);

extern void
child_close
(
 FILE *p_hnd
);
#endif
