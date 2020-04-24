
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
 * $Log: dtapi_impl.h,v $
 * Revision 1.1.6.1  1996/10/03  18:38:51  arvind
 * 	Submit fix for CHFts19700
 * 	[1996/08/14  19:45 UTC  mullan_s  /main/mullan_dce122_bugs_1/1]
 *
 * 	Merge in all changes to dced tests for new ACLs.
 * 	[1996/01/15  22:55 UTC  jss  /main/jss_dced_acl/1]
 *
 * 	Create DCED_SRVR_ENTRY dynamically.
 * 	[1994/09/30  21:15:43  baum  1.1.2.2]
 *
 * Revision 1.1.4.2  1996/02/18  23:05:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:26  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  15:18:12  root
 * 	Submit
 * 	[1995/12/11  14:33:05  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  23:18:18  root]
 * 
 * Revision 1.1.2.2  1994/09/30  21:15:43  baum
 * 	New secval tests plus the ability to run without TET (OT 11975)
 * 	[1994/09/30  21:14:43  baum]
 * 
 * Revision 1.1.2.1  1994/09/28  21:47:44  melman
 * 	moved admin tests to test/functional
 * 	[1994/09/28  21:15:23  melman]
 * 
 * Revision 1.1.2.4  1994/07/20  19:57:46  ganni
 * 	Merged with changes from 1.1.2.3
 * 	[1994/07/20  19:57:21  ganni]
 * 
 * 	introduce the tests for srvrconf and srvrexec services
 * 	[1994/07/20  19:37:53  ganni]
 * 
 * Revision 1.1.2.3  1994/06/17  18:44:47  devsrc
 * 	     cr10872 - fix copyright
 * 	     [1994/06/17  18:32:32  devsrc]
 * 	Revision 1.1.2.2  1994/06/03  14:05:32  ganni
 * 	create the principal needed for the tests through scripts
 * 	[1994/06/03  14:03:16  ganni]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:21  ganni
 * 	Initial Version
 * 	[1994/05/18  21:12:46  ganni]
 * 
 * Revision 1.1.2.3  1994/06/17  18:44:46  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:32:27  devsrc]
 * 
 * Revision 1.1.2.2  1994/06/03  14:05:32  ganni
 * 	create the principal needed for the tests through scripts
 * 	[1994/06/03  14:03:16  ganni]
 * 
 * Revision 1.1.2.1  1994/05/18  21:21:21  ganni
 * 	Initial Version
 * 	[1994/05/18  21:12:46  ganni]
 * 
 * $EndLog$
 */

/*
** Module: dtapi_impl.h
**
** FACILITY: DCE Host Daemon API
**
** ABSTRACT: This module implements the tests for dced api
**
** SCOPE: Public
**
** Date:
*/
#ifndef _DTAPI_IMPL_H
#define _DTAPI_IMPL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <tet_api.h>
#include <dce/dced.h>

#ifdef _NO_TET
#define tet_infoline(m_str) printf ( "TET_INFOLINE: %s\n", m_str )
#define tet_result(m_code) printf("TET_RESULT: %d\n",m_code)
#define tet_getvar getenv
#endif

/* tet message buffer */
#define DCED_TEST_MSG_LEN	250

/* max password length */
#define MAX_PASSWD_LEN		25

/* max hostname length */
#define MAX_HOSTNAME_LEN	25

#define CELL_ADMIN		tet_getvar ("T_CELL_ADMIN")
#define CELL_ADMIN_PWD		tet_getvar ("T_CELL_ADMIN_PWD")

/*	Info needed for secval tests -
	The principal DCED_TEST_PRIN need to be created
	in the registry.
*/
#define SECVAL_SVC		(dced_string_t)	dced_c_service_secval
#define DCED_TEST_PRINC		(dced_string_t) "dced_test_user"
#define DCED_TEST_PASSWD	(dced_string_t) "dced_test_user"


/*	Info needed for hostdata tests -
*/
#define HOST_SVC		(dced_string_t)	dced_c_service_hostdata
#define DCED_TEST_HOST          (dced_string_t) "dced_test_host"
#define DCED_HOST_DESC          (dced_string_t) "dced_hostdata_test"
#define DCED_HOST_FILE          (dced_string_t) "/tmp/dced_host_file"
#define DCED_HOST_ATTR0         (dced_string_t) "dced_test_attr0"
#define DCED_HOST_ATTR1         (dced_string_t) "dced_test_attr1"
#define DCED_HOST_ATTR2         (dced_string_t) "dced_test_attr2"


/*	Info needed for keytab tests -
	DO the principals DCED_KEY_PRIN_? need to be created
	in the registry????
*/
#define KEYTAB_SVC		(dced_string_t)	dced_c_service_keytab
#define MAX_API_KEYS		3

#define DCED_KEY_DESC          (dced_string_t) "dced_keytab_testing"
#define DCED_KEY_USER          (dced_string_t) "dced_key_usr"
#define DCED_KEY_FILE          (dced_string_t) "/tmp/dced_key_file"

#define DCED_KEY_PRINC_1       (dced_string_t) "dced_test_user_1"
#define DCED_KEY_PASSWD_1      (dced_string_t) "dced_test_user_1"

#define DCED_KEY_PRINC_2       (dced_string_t) "dced_test_user_2"
#define DCED_KEY_PASSWD_2      (dced_string_t) "dced_test_user_2"

#define DCED_KEY_PRINC_3       (dced_string_t) "dced_test_user_3"
#define DCED_KEY_PASSWD_3      (dced_string_t) "dced_test_user_3"

#define SRVRCONF_SVC		(dced_string_t)	dced_c_service_srvrconf
#define SRVREXEC_SVC		(dced_string_t)	dced_c_service_srvrexec

#define DCED_TEST_SRVR		(dced_string_t) "dced_test_server"
#define DCED_SRVR_PRINC_1       (dced_string_t) "dced_test_srvr_1"
#define DCED_SRVR_PASSWD_1      (dced_string_t) "dced_test_srvr_1"
#define DCED_SRVR_ANNO		(dced_string_t) "test server interface"
#define DCED_SRVR_ENTRY		(dced_string_t) "/.:/hosts/"
#define DCED_SRVR_PGM		tet_getvar ("T_PROGRAM_PATH")
#define DCED_SRVR_PROTO		tet_getvar ("T_DEF_PROTOCOL")
#define DCED_SRVR_IF1		(unsigned_char_t *)"002d2117-7062-1dd2-a9bb-0000c03cd04c"
#define DCED_SRVR_OBJ1		(unsigned_char_t *)"60cc7b6a-9e48-11cd-a5d7-0000c03cd04c"

#define CHECK_SETUP_CMD( m_str, m_st )					\
  if ( m_st != error_status_ok ) {					\
    char buf[100];							\
    sprintf ( buf, "%s fails:", m_str );				\
    tet_infoline ( buf );						\
    dced_util_printerr ( m_st ); 					\
    g_setup_failed = TRUE;						\
    return;								\
  }

#define CHECK_STATUS( m_str, m_st )					\
  if ( m_st != error_status_ok ) {					\
    char buf[100];							\
    sprintf ( buf, "%s fails:", m_str );				\
    tet_infoline ( buf );						\
    dced_util_printerr ( m_st ); 					\
    tet_result (TET_UNRESOLVED);					\
    goto cleanup;							\
  }


extern void  dced_util_printerr(
	/* [in] */	error_status_t error_status
);
extern void print_uuid(
        /* [in] */      char		*in_str,
        /* [in] */      uuid_t		*uuid_ptr
);
extern void test_format_2_name(
        /* [ in ] */    dced_string_t   svc_dced,
        /* [ Out ] */   char            **name_ptr
);
extern void test_format_3_name(
        /* [ in ] */    dced_string_t   svc_dced,
        /* [ out ] */   char            **name_ptr
);
extern void test_format_4_name(
        /* [ in ] */    dced_string_t   svc_dced,
        /* [ out ] */   char            **name_ptr
);
extern void  dced_util_binding_create(
        /* [in]  */     dced_string_t	service,
        /* [out] */     dced_binding_handle_t *h,
        /* [out] */     error_status_t	*st
);
extern void test_attr_init(
        /* [in, out] */ dced_attr_list_t *hattr_ptr,
        /* [in]  */	int             num_attrs,
        /* [out] */	error_status_t  *st
);
extern void test_attr_free(
        /* [in] */	dced_attr_list_t *hattr_ptr,
        /* [out] */	error_status_t  *st
);
extern boolean32 test_attr_verify(
        /* [in]  */	sec_attr_t      *in_ptr,
        /* [in]  */	sec_attr_t      *out_ptr
);
extern boolean32 test_attrlist_verify(
        /* [in]  */     dced_attr_list_t *in_ptr,
        /* [in]  */     dced_attr_list_t *out_ptr
);
extern void test_keyentry_init(
        /* [out] */	dced_entry_t	*key_entry_ptr,
        /* [out] */	error_status_t	*st
);
extern void print_key(
        /* [in] */      dced_key_t	*key_ptr
);
extern void test_key_init(
        /* [in]  */	int		key_num,
        /* [out] */	dced_key_t	*key_info_ptr
);
extern void test_keylist_init(
        /* [in, out] */	dced_key_list_t *klist_ptr,
        /* [in]  */	int             num_keys,
        /* [out] */	error_status_t  *st
);
extern boolean32 test_key_verify(
        /* [in]  */     dced_key_t      *key_in_ptr,
        /* [in]  */     dced_key_t      *key_out_ptr
);
extern boolean32 test_keylist_verify(
        /* [in]  */     dced_key_list_t *in_ptr,
        /* [in]  */     dced_key_list_t *out_ptr
);
extern void test_keylist_free(
        /* [in] */	dced_key_list_t *klist_ptr,
        /* [out] */	error_status_t  *st
);
extern void test_keytab_init(
        /* [in] */	dced_binding_handle_t *binding_ptr,
        /* [in, out] */ dced_entry_t    *key_entry_ptr,
        /* [in, out] */ dced_key_list_t *klist_ptr,
        /* [in]  */	int             num_keys,
        /* [out] */	error_status_t  *st
);
extern void test_keytab_free(
        /* [in] */	dced_binding_handle_t *binding_ptr,
        /* [in, out] */ dced_entry_t    *key_entry_ptr,
        /* [in, out] */ dced_key_list_t *klist_ptr,
        /* [out] */	error_status_t  *st
);
extern void test_hostentry_init(
        /* [out] */	dced_entry_t    *host_entry_ptr,
        /* [out] */	error_status_t  *st
);
extern void test_hostattr_init(
        /* [in, out] */ dced_attr_list_t *hattr_ptr,
        /* [in]  */	int             num_attrs,
        /* [out] */	error_status_t  *st
);
extern void test_hostattr_free(
        /* [in] */	dced_attr_list_t *hattr_ptr,
        /* [out] */	error_status_t  *st
);
extern boolean32 test_hostattr_verify(
        /* [in]  */	sec_attr_t      *in_ptr,
        /* [in]  */	sec_attr_t      *out_ptr
);
extern boolean32 test_hostdata_verify(
        /* [in]  */     dced_attr_list_t *in_ptr,
        /* [in]  */     dced_attr_list_t *out_ptr
);
extern void test_hostdata_init(
        /* [in] */	dced_binding_handle_t *binding_ptr,
        /* [in, out] */ dced_entry_t    *host_entry_ptr,
        /* [in, out] */ dced_attr_list_t *hattr_ptr,
        /* [in]  */	int             num_attrs,
        /* [out] */	error_status_t  *st
);
extern void test_hostdata_free(
        /* [in] */	dced_binding_handle_t *binding_ptr,
        /* [in, out] */ dced_entry_t    *host_entry_ptr,
	/* [in, out] */ dced_attr_list_t *hattr_ptr,
        /* [out] */	error_status_t  *st
);
extern void print_cfgentry(
        /* [in] */      dced_entry_t	*he_ptr
);
extern void print_cfgentry_list(
        /* [in] */      dced_entry_list_t *hel_ptr
);
extern boolean32 test_cfgentry_verify(
        /* [in]  */     dced_entry_t	*in_ptr,
        /* [in]  */     dced_entry_t	*out_ptr
);

extern void test_srvr_entry_init(
        /* [in, out] */ server_t        *srvr_entry_ptr,
        /* [out] */     error_status_t  *st
);
extern void test_srvr_entry_free(
        /* [in] */	server_t        *srvr_entry_ptr,
        /* [out] */     error_status_t  *st
);
extern boolean32 verify_server_t(
        /* [in]  */     server_t    *i_ptr,
        /* [in]  */     server_t    *o_ptr
);
extern void print_server_t(
        /* [in]  */     server_t    *o_ptr
);

extern void test_srvrconf_init(
        /* [in] */	dced_binding_handle_t *binding_ptr,
        /* [in, out] */ server_t        *srvr_entry_ptr,
        /* [out] */	error_status_t  *st
);
extern void test_srvrconf_free(
        /* [in] */	dced_binding_handle_t *binding_ptr,
        /* [in, out] */ server_t        *srvr_entry_ptr,
        /* [out] */	error_status_t  *st
);
extern void test_srvrexec_init(
        /* [in] */      dced_binding_handle_t *binding_ptr,
        /* [in, out] */ server_t        *srvr_entry_ptr,
        /* [out] */     uuid_t          *proc_id_ptr,
        /* [out] */     error_status_t  *st
);
extern void test_srvrexec_free(
        /* [in] */      dced_binding_handle_t *binding_ptr,
        /* [in, out] */ server_t        *srvr_entry_ptr,
        /* [out] */     uuid_t          *srvr_proc_id,
        /* [out] */     error_status_t  *st
);

extern boolean32 test_api_10( 
	/* [in] */	error_status_t expected_stat 
);

#endif
