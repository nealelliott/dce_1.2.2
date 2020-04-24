/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: aud_test_rpc_manager.c,v $
 * Revision 1.1.10.1  1996/07/09  15:08:58  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:29 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/17  23:26:08  marty  1.1.8.3]
 *
 * Revision 1.1.8.3  1996/02/17  23:26:08  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:05  marty]
 * 
 * Revision 1.1.8.2  1995/12/11  21:51:29  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/08/18  20:10 UTC  kline_s
 * 	Merge fix for CHFts16115
 * 
 * 	HP revision /main/kline_s_mothra/2  1995/08/17  21:33 UTC  kline_s
 * 	CHFts16115: Remove DES_HIDDEN conditional - only using integrity
 * 	protection level instead of using the privacy protection level
 * 	for Domestic.  Required to avoid conflicts due to singular
 * 	Domestic/Export release.
 * 
 * 	HP revision /main/kline_s_mothra/1  1995/07/06  19:45 UTC  kline_s
 * 	Enable tests to function properly using international (ie. non-DES) bits.
 * 	[1995/12/11  20:53:41  root]
 * 
 * Revision 1.1.4.4  1994/10/26  20:35:48  weisz
 * 	CR 12598: increased test coverage.  add some casts to get rid of warnings on HP.
 * 	[1994/10/26  20:35:26  weisz]
 * 
 * Revision 1.1.4.3  1994/10/26  20:19:34  weisz
 * 	CR 12598: increased test coverage.
 * 	[1994/10/26  20:18:24  weisz]
 * 
 * Revision 1.1.4.2  1994/10/25  19:18:18  weisz
 * 	CR 12598: increase test coverage.
 * 	[1994/10/25  19:18:01  weisz]
 * 
 * Revision 1.1.4.1  1994/10/25  14:20:33  weisz
 * 	CR 12598: increase test coverage.
 * 	[1994/10/25  14:19:10  weisz]
 * 
 * Revision 1.1.2.11  1994/09/29  23:12:56  weisz
 * 	CR 10703: increased trail file reading test.
 * 	[1994/09/29  23:12:27  weisz]
 * 
 * Revision 1.1.2.10  1994/09/27  17:32:39  weisz
 * 	CR 10703: added tests for trail full situation.
 * 	[1994/09/27  17:31:44  weisz]
 * 
 * Revision 1.1.2.9  1994/08/30  17:21:41  weisz
 * 	CR 11884: added new API - dce_aud_start_with_uuid
 * 	[1994/08/30  17:18:34  weisz]
 * 
 * Revision 1.1.2.8  1994/08/17  19:57:00  weisz
 * 	CR 11443: added PENDING as a new valid outcome.
 * 	[1994/08/17  19:55:43  weisz]
 * 
 * Revision 1.1.2.5  1994/06/13  20:28:18  weisz
 * 	R 10751: remove printfs of some binary data.
 * 	[1994/06/13  20:25:02  weisz]
 * 
 * Revision 1.1.2.4  1994/06/13  18:26:36  weisz
 * 	CR 10751: moved printf of groups info from testcase 10 to testcase 9.
 * 	[1994/06/13  18:23:18  weisz]
 * 
 * Revision 1.1.2.3  1994/06/10  20:45:52  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:17:14  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/06  17:05:51  weisz
 * 	CR 10529: test api_log07 was wrongly expecting no log to be produced.
 * 	[1994/05/06  17:05:06  weisz]
 * 
 * Revision 1.1.2.1  1994/04/20  13:27:58  weisz
 * 	initial submission
 * 	[1994/04/20  13:26:22  weisz]
 * 
 * $EndLog$
 */
#include <stdio.h>

#include <dce/dce.h>
#include <dce/id_base.h>
#include <dce/secidmap.h>
#include <dce/rpc.h>
#include <dce/pgo.h>
#include <dce/sec_login.h>

#include <dce/audit.h>
#include <aud_test.h>
#include <aud_testp.h>
#include <error_statusp.h>

#include <tet_api.h>
#include <string.h>

#define SEC_ACL_MGR_TYPES_SIZE_AVAIL 10
#define END_OF_REC_STR_LEN	     26
#define END_OF_REC_STR		     "End of Event record number"

#define TestCase1  999
#define TestCase2  998
#define TestCase3  997
#define TestCase4  996
#define TestCase5  995
#define TestCase6  994
#define TestCase7  993
#define TestCase8  992
#define TestCase9  991
#define TestCase10 990
#define TestCase11 989
#define TestCase12 988
#define TestCase13 988
#define TestCase14 988
#define TestCase20 988
#define TestCase21 988
#define TestCase22 988
#define TestCase23 987

#define CHECK_STATUS(where, status) \
if (*status != 0) { \
    printf("\n%s returns bad status= %lx\n", where, *status); \
    exit(1); \
}

extern boolean32 debug_mode;

extern boolean32 audit;

char *end_in_buff, *end_out_buff;

void
aud_test_case1(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    char	      *byte_string = "byte_string";
    char	      *char_string = "char_string";
    uuid_t	       new_uuid;
    idl_hyper_int      hyper_int;
    idl_uhyper_int     uhyper_int;
    boolean32	       info_remaining;
    char	       predicate[2048];
    unsigned_char_t   *string_uuid;
    uuid_t	       manager_types[SEC_ACL_MGR_TYPES_SIZE_AVAIL];
    uuid_t	       manager_type;
    sec_acl_handle_t   acl_handle = NULL;
    sec_acl_list_t     acl_list;
    sec_acl_t	       sec_acl;
    unsigned32	       size_used;
    unsigned32	       num_types;
    char	      *entry_name = "/.:/sec/group";
    boolean32	       bind_to_entry = 0;


    printf("Start aud_test_case1\n");
    fflush(stdout);

    strcpy(predicate, "garbage");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    printf("after rpc_binding_inq_auth_client\n");
    fflush(stdout);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
 		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start\n");

    dce_aud_start(TestCase1, h, aud_c_evt_all_info, aud_c_esl_cond_success,
	&in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_small_int;
    audit_evt_info.data.small_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_short_int;
    audit_evt_info.data.short_int = 20;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 30;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    hyper_int.high = 1111;
    hyper_int.low  = 2222;

    audit_evt_info.format = aud_c_evt_info_hyper_int;
    audit_evt_info.data.hyper_int = hyper_int;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_usmall_int;
    audit_evt_info.data.usmall_int = 50;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_ushort_int;
    audit_evt_info.data.ushort_int = 60;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_ulong_int;
    audit_evt_info.data.ulong_int = 70;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    uhyper_int.high = 3333;
    uhyper_int.low  = 4444;

    audit_evt_info.format = aud_c_evt_info_uhyper_int;
    audit_evt_info.data.uhyper_int = uhyper_int;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_short_float;
    audit_evt_info.data.short_float = 90;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_long_float;
    audit_evt_info.data.long_float = 100;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_boolean;
    audit_evt_info.data.boolean = 1;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    uuid_create(&new_uuid, stp);
    CHECK_STATUS("uuid_create", stp);

    audit_evt_info.format = aud_c_evt_info_uuid;
    audit_evt_info.data.uuid = new_uuid;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_byte_string;
    audit_evt_info.data.byte_string = (idl_byte *)byte_string;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_char_string;
    audit_evt_info.data.char_string = (idl_char *)char_string;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    sec_acl_bind((unsigned_char_t *)entry_name, bind_to_entry, &acl_handle, stp);
    CHECK_STATUS("sec_acl_bind", stp);

    sec_acl_get_manager_types (acl_handle,
			       sec_acl_type_object,
			       SEC_ACL_MGR_TYPES_SIZE_AVAIL,
			       &size_used,
			       &num_types,
			       manager_types,
			       stp);
    CHECK_STATUS("sec_acl_get_manager_types", stp);

    memcpy ((char *)&manager_type, (char *)&manager_types[0], sizeof (uuid_t));

    sec_acl_lookup (acl_handle,
		    &manager_type,
		    sec_acl_type_object,
		    &acl_list,
		    stp);
    CHECK_STATUS("sec_acl_lookup", stp);

    audit_evt_info.format = aud_c_evt_info_acl;
    audit_evt_info.data.acl = acl_list.sec_acls[0];
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, aud_c_evt_raw_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_print(in_ard, aud_c_evt_groups_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_print(in_ard, aud_c_evt_delegates_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_print(in_ard, aud_c_evt_address_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_print(in_ard, aud_c_evt_specific_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_print(in_ard, aud_c_evt_all_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    /* negative test, use NULL ard */
    dce_aud_length(NULL, stp);
    if (*stp == 0)
    {
	printf("aud_test_case1 failed, dce_aud_length accepts NULL ard\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);
    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    /* negative test, use NULL trail file pointer */
    dce_aud_next(NULL, NULL, 0, &out_ard, stp);
    if (*stp == 0)
    {
	printf("aud_test_case1 failed, dce_aud_next accepts NULL at\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    dce_aud_print(out_ard, aud_c_evt_all_info, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    /* negative test, use NULL audit record descriptor */
    dce_aud_print(NULL, aud_c_evt_all_info, &out_buff, stp);
    if (*stp == 0)
    {
	printf("aud_test_case1 failed, dce_aud_print accepts NULL ard\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case1 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    printf("\naudit record:\n %s\n", out_buff);

    dce_aud_get_header(out_ard, &header, stp);
    CHECK_STATUS("dce_aud_get_header", stp);

    dce_aud_free_header(header, stp);
    CHECK_STATUS("dce_aud_free_header", stp);

    /* negative test, use NULL audit record descriptor */
    dce_aud_get_header(NULL, &header, stp);
    if (*stp == 0)
    {
	printf("aud_test_case1 failed, dce_aud_get_header accepts NULL ard\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    /* negative test, use NULL, shouldn't crash */
    dce_aud_free_header(NULL, stp);

    info_remaining = TRUE;
    while (info_remaining)
    {
       dce_aud_get_ev_info(out_ard, &out_info, stp);
       CHECK_STATUS("dce_aud_get_ev_info", stp);

       if(out_info == NULL)
       {
	   info_remaining = FALSE;
       }
       else
       {
	   dce_aud_free_ev_info(out_info, stp);
	   CHECK_STATUS("dce_aud_free_ev_info", stp);
       }
    }

    /* negative test, use NULL audit record descriptor */
    dce_aud_get_ev_info(NULL, &out_info, stp);
    if (*stp == 0)
    {
	printf("aud_test_case1 failed, dce_aud_get_ev_info accepts NULL ard\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    /* negative test, use NULL, shouldn't crash... */
    dce_aud_free_ev_info(NULL, stp);

    dce_aud_discard(out_ard, stp);

    /* negative test, use NULL audit record descriptor */
    dce_aud_discard(NULL, stp);  /* shouldn't crash... */

    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    /* test predicate input for dce_aud_next */
    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "EVENT:200");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "OUTCOME:SUCCESS");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "OUTCOME:DENIAL");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "OUTCOME:FAILURE");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "OUTCOME:PENDING");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "OUTCOME:PENDING,OUTCOME:DENIAL,OUTCOME:FAILURE");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "OUTCOME:GARBAGE");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "STATUS:NAME");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "STATUS:DCE");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "STATUS:GARBAGE");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "TIME:>20");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "TIME:<20");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    uuid_to_string(&new_uuid, &string_uuid, stp);
    CHECK_STATUS("uuid_to_string", stp);

    strcpy(predicate, "SERVER:");
    strcat(predicate, (char *)string_uuid);
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "CLIENT:");
    strcat(predicate, (char *)string_uuid);
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "CELL:");
    strcat(predicate, (char *)string_uuid);
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "GROUP:");
    strcat(predicate, (char *)string_uuid);
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    strcpy(predicate, "ADDR:HERE");
    dce_aud_next(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    *stp = (error_status_t) 0;
    printf("TestCase1 succeeded\n");
}

void
aud_test_case2(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;

   printf("Start aud_test_case2\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("dce_aud_start_with_pac\n");

    dce_aud_start_with_pac(TestCase2, (sec_id_pac_t *) privs, NULL,
		     aud_c_evt_all_info, aud_c_esl_cond_denial, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, 0, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_denial, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, 0, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    printf("\naudit record:\n %s\n", out_buff);

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case1 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
    printf("TestCase2 succeeded\n");
}


void
aud_test_case3(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    char	      *principal_name = "cell_admin";

   printf("Start aud_test_case3\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("dce_aud_start_with_name\n");

    dce_aud_start_with_name(TestCase3, (unsigned_char_t *)principal_name, NULL,
		     aud_c_evt_all_info, aud_c_esl_cond_failure, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, aud_c_evt_all_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_failure, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, aud_c_evt_all_info, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    printf("\naudit record:\n %s\n", out_buff);

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case1 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
    printf("TestCase3 succeeded\n");
}

void
aud_test_case4(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;

   printf("Start aud_test_case4\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    /* flags=0, i.e. not write, not read, therefore invalid   */
    dce_aud_open((unsigned32)0, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    printf("dce_aud_open stp= %lx\n", *stp);

    if (*stp == aud_s_ok)
    {
      printf("aud_test_case4 failed\n");
      *stp = (error_status_t) 1;
    }
    else
    {
      printf("aud_test_case4 succeeded\n");
      *stp = (error_status_t) 0;
    }

    if (in_trail != NULL)
    {
      dce_aud_close(in_trail, stp);
      CHECK_STATUS("dce_aud_close", stp);
      printf("aud_test_case4 failed - trail file allocated\n");
      *stp = (error_status_t) 1;
    }
}


void
aud_test_case5(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;

   printf("Start aud_test_case5\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    /* flags= both write and read, which is not allowed */
    dce_aud_open((aud_c_trl_open_read | aud_c_trl_open_write), TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    printf("dce_aud_open stp= %lx\n", *stp);

    if (*stp == aud_s_ok)
    {
      printf("aud_test_case5 failed\n");
      *stp = (error_status_t) 1;
    }
    else
    {
      printf("aud_test_case5 succeeded\n");
      *stp = (error_status_t) 0;
    }

    if (in_trail != NULL)
    {
      dce_aud_close(in_trail, stp);
      CHECK_STATUS("dce_aud_close", stp);
      printf("aud_test_case5 failed - trail file allocated\n");
      *stp = (error_status_t) 1;
    }
}


void
aud_test_case6(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i, rc;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    long int	       trail_size;
    FILE	      *trail_fp;

   printf("Start aud_test_case6\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start with flag aud_c_evt_always_log\n");

    /* set flag always log, which should cause logging even with no */
    /* filters set */
    dce_aud_start(TestCase6, h, aud_c_evt_always_log, aud_c_esl_cond_pending,
	&in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, 0, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_pending, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    trail_fp = fopen(TRAIL_FILE, "r");
    rc = fseek(trail_fp, 0, SEEK_END);
    printf("rc from fseek = %d\n", rc);
    trail_size = ftell(trail_fp);
    printf("trail_size = %d\n", trail_size);
    fclose(trail_fp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, 0, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    printf("\naudit record:\n %s\n", out_buff);

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case1 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
    printf("aud_test_case6 succeeded\n");
}

void
aud_test_case7(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    FILE	      *trail_fp;
    long int	       trail_size;

   printf("Start aud_test_case7\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start with flag aud_c_evt_always_alarm\n");

    dce_aud_start(TestCase7, h, aud_c_evt_always_alarm, aud_c_esl_cond_success,
	&in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, 0, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    /* after the commit, the audit record should show on the console */
    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    if (*stp == aud_s_ok)
    {
       dce_aud_close(in_trail, stp);
       *stp = (error_status_t) 0;
       printf("aud_test_case7 succeeded\n");
    }
    else
    {
       dce_aud_close(in_trail, stp);
       *stp = (error_status_t) 1;
       printf("aud_test_case7 failed\n");
    }

}

void
aud_test_case8(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    long int	       trail_size;
    FILE	      *trail_fp;

   printf("Start aud_test_case8\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
	         FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start with log and alarm flags set\n");

    /* set flag always log and alarm, which should cause logging even with no */
    /* filters set, as well as audit record will be dumped to the console  */
    dce_aud_start(TestCase8, h, aud_c_evt_always_log | aud_c_evt_always_alarm,
					  aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, 0, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, 0, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case1 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
    printf("aud_test_case8 succeeded\n");
}


void
aud_test_case9(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    long int	       trail_size;
    FILE	      *trail_fp;

   printf("Start aud_test_case9\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start with flag aud_c_evt_groups_info\n");

    /* set flag always log and groups info, which should cause logging with */
    /* optional groups information recorded in the audit header  */
    dce_aud_start(TestCase9, h, aud_c_evt_always_log | aud_c_evt_groups_info,
					  aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, aud_c_evt_groups_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    printf("dce_aud_print (in) event: %s", in_buff);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, aud_c_evt_groups_info, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    printf("dce_aud_print (out) event: %s", out_buff);

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case1 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    dce_aud_get_header(out_ard, &header, stp);
    CHECK_STATUS("dce_aud_get_header", stp);

    printf("\naudit record header.client_id[0].num_groups:\n %lx\n", \
	   header->client_id[0].num_groups);
    printf("\naudit record:\n %s\n", out_buff);

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
    printf("aud_test_case9 succeeded\n");
}

void
aud_test_case10 (
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    long int	       trail_size;
    FILE	      *trail_fp;

   printf("Start aud_test_case10\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start with flag aud_c_evt_address_info\n");

    /* set flag always log and groups info, which should cause logging with */
    /* optional address information recorded in the audit header  */
    dce_aud_start(TestCase10, h, aud_c_evt_always_log | aud_c_evt_address_info,
					  aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, aud_c_evt_address_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    printf("dce_aud_print (in) event: %s", in_buff);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, aud_c_evt_address_info, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    printf("dce_aud_print (out) event: %s", out_buff);

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case1 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    dce_aud_get_header(out_ard, &header, stp);
    CHECK_STATUS("dce_aud_get_header", stp);

    printf("\naudit record header.addr:\n %s\n", header->addr);
    printf("\naudit record:\n %s\n", out_buff);

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
    printf("aud_test_case10 succeeded\n");
}

void
aud_test_case11 (
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    long int	       trail_size;
    FILE	      *trail_fp;

   printf("Start aud_test_case11\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start with groups and address info\n");
    /*
     * set flag always log, groups info and address info, which should cause
     * logging with optional address and groups information recorded in
     * the audit header
     */
    dce_aud_start(TestCase11, h,
	aud_c_evt_always_log | aud_c_evt_address_info | aud_c_evt_groups_info,
					 aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, aud_c_evt_address_info | aud_c_evt_groups_info,
								&in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, aud_c_evt_address_info | aud_c_evt_groups_info,
							       &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case1 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    printf("\naudit record:\n %s\n", out_buff);

    dce_aud_get_header(out_ard, &header, stp);
    CHECK_STATUS("dce_aud_get_header", stp);

    printf("\naudit record header.addr:\n %s\n", header->addr);
    printf("\naudit record header.client_id[0].num_groups:\n %lx\n", \
	    header->client_id[0].num_groups);

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
    printf("aud_test_case11 succeeded\n");
}

void
aud_test_case12 (
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    long int	       trail_size;
    FILE	      *trail_fp;

   printf("Start aud_test_case12\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start with groups and address info\n");
    /*
     * set flag always log and aud_c_evt_all_info, which should cause
     * logging with optional address and groups information recorded in
     * the audit header
     */
    dce_aud_start(TestCase12, h, aud_c_evt_always_log | aud_c_evt_all_info,
				       aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, aud_c_evt_all_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, aud_c_evt_all_info, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case1 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    printf("\naudit record:\n %s\n", out_buff);

    dce_aud_get_header(out_ard, &header, stp);
    CHECK_STATUS("dce_aud_get_header", stp);

    printf("\naudit record header.addr:\n %s\n", header->addr);
    printf("\naudit record header.client_id[0].num_groups:\n %lx\n", \
	    header->client_id[0].num_groups);

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
    printf("aud_test_case12 succeeded\n");
}


void
aud_test_case13 (
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_rec_t      in_ard;

   printf("Start aud_test_case13\n");

    /* Test Environment Cleanup - keep previous trail file */
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }
    /*
     * open trail for read, yet try and write to it: should fail
     */
    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    dce_aud_start(TestCase13, h, aud_c_evt_always_log,
				  aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    dce_aud_commit(in_trail, in_ard, 0, 0,
		   aud_c_esl_cond_success, stp);

    printf("\ndce_aud_commit stp= %lx\n",*stp); \

    if (*stp == 0)
    {
	dce_aud_close(in_trail, stp);
	CHECK_STATUS("dce_aud_close", stp);
	*stp = (error_status_t) 1;
	printf("aud_test_case13 failed\n");

    }
    else
    {
	dce_aud_close(in_trail, stp);
	CHECK_STATUS("dce_aud_close", stp);
	*stp = (error_status_t) 0;
	printf("aud_test_case13 succeeded\n");
    }
}


void
aud_test_case14 (
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_rec_t      out_ard;

   printf("Start aud_test_case14\n");

    /* Test Environment Cleanup - keep previous trail file*/
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }
    /*
     * open trail for write, yet try and read it: should fail
     */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    dce_aud_next(in_trail, NULL, 0, &out_ard, stp);

    printf("\ndce_aud_next stp= %lx\n",*stp);

    if (out_ard != NULL)
    {
	dce_aud_close(in_trail, stp);
	CHECK_STATUS("dce_aud_close", stp);
	*stp = (error_status_t) 1;
	printf("aud_test_case14 failed\n");
    }
    else
    {
	dce_aud_close(in_trail, stp);
	CHECK_STATUS("dce_aud_close", stp);
	*stp = (error_status_t) 0;
	printf("aud_test_case14 succeeded\n");
    }
}

void
aud_test_case15 (
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_rec_t      in_ard;

   printf("Start aud_test_case15\n");

    /* Test Environment Cleanup */
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    dce_aud_start(TestCase13, h, aud_c_evt_always_log,
				  aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

/*
    supply different outcome than the one supplied at dce_aud_start.
    should fail.
*/
    dce_aud_commit(in_trail, in_ard, 0, 0,
		   aud_c_esl_cond_denial, stp);

    printf("\ndce_aud_commit stp= %lx\n",*stp);

    if (*stp == 0)
    {
	dce_aud_close(in_trail, stp);
	CHECK_STATUS("dce_aud_close", stp);
	*stp = (error_status_t) 1;
	printf("aud_test_case15 failed\n");

    }
    else
    {
	dce_aud_close(in_trail, stp);
	CHECK_STATUS("dce_aud_close", stp);
	*stp = (error_status_t) 0;
	printf("aud_test_case15 succeeded\n");
    }
}

void
aud_test_case16 (
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_rec_t      in_ard;

   printf("Start aud_test_case16\n");

    /* Test Environment Cleanup */
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    dce_aud_start(TestCase13, h, aud_c_evt_always_log,
				  aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);
/*
    supply invalid outcome - outside the range of valid ones.
    should fail.
*/
    dce_aud_commit(in_trail, in_ard, 0, 0,
		   0x12340001, stp);

    printf("\ndce_aud_commit stp= %lx\n",*stp);

    if (*stp == 0)
    {
	dce_aud_close(in_trail, stp);
	CHECK_STATUS("dce_aud_close", stp);
	*stp = (error_status_t) 1;
	printf("aud_test_case16 failed\n");

    }
    else
    {
	dce_aud_close(in_trail, stp);
	CHECK_STATUS("dce_aud_close", stp);
	*stp = (error_status_t) 0;
	printf("aud_test_case16 succeeded\n");
    }
}

void
aud_test_case17 (
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_rec_t      in_ard;

   printf("Start aud_test_case17\n");

    /* Test Environment Cleanup */
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    dce_aud_start(TestCase13, h, aud_c_evt_always_log,
				  aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);
/*
    supply two outcomes - this is invalid,
    should fail.
*/
    dce_aud_commit(in_trail, in_ard, 0, 0,
		   aud_c_esl_cond_success | aud_c_esl_cond_pending, stp);

    printf("\ndce_aud_commit stp= %lx\n",*stp);

    if (*stp == 0)
    {
	dce_aud_close(in_trail, stp);
	CHECK_STATUS("dce_aud_close", stp);
	*stp = (error_status_t) 1;
	printf("aud_test_case17 failed\n");

    }
    else
    {
	dce_aud_close(in_trail, stp);
	CHECK_STATUS("dce_aud_close", stp);
	*stp = (error_status_t) 0;
	printf("aud_test_case17 succeeded\n");
    }
}


void
aud_test_case18(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i, j;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    unsigned_char_t   *global_self = NULL;
    sec_rgy_handle_t   rgy_context;
    char	      *address = "Testcase18";
    uuid_t	       server_uuid;
    uuid_t	       client_uuid;
    uuid_t	       realm_uuid;


   printf("Start aud_test_case18\n");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("dce_aud_start_with_uuid\n");

    rpc_mgmt_inq_server_princ_name(NULL, authn_svc, &global_self, stp);
    CHECK_STATUS("rpc_mgmt_inq_server_princ_name", stp);

    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("sec_rgy_site_open", stp);

    sec_id_parse_name(rgy_context, global_self, NULL, &realm_uuid,
		      NULL, &client_uuid, stp);
    CHECK_STATUS("sec_id_parse_name", stp);

    server_uuid = client_uuid;

    dce_aud_start_with_uuid(TestCase3, server_uuid, client_uuid, realm_uuid,
			    (unsigned_char_t *)address, aud_c_evt_all_info,
			     aud_c_esl_cond_failure, &in_ard, stp);

    CHECK_STATUS("dce_aud_start_with_uuid", stp);

    dce_aud_print(in_ard, aud_c_evt_all_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_failure, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, aud_c_evt_all_info, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    printf("\naudit record:\n %s\n", out_buff);

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case1 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
    printf("TestCase18 succeeded\n");
}


void
aud_test_case19(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    handle_t	       server_binding;
    sec_login_handle_t login_context;
    unsigned_char_t   *server_princ_name = NULL;

    printf("Start aud_test_case19\n");
    fflush(stdout);

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    printf("after rpc_binding_inq_auth_client\n");
    fflush(stdout);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    rpc_binding_server_from_client(h, &server_binding, stp);
    CHECK_STATUS("rpc_binding_server_from_client", stp);

    sec_login_get_current_context(&login_context, stp);
    CHECK_STATUS("sec_login_get_current_context", stp);

    rpc_mgmt_inq_server_princ_name(NULL, authn_svc, &server_princ_name, stp);
    CHECK_STATUS("rpc_mgmt_inq_server_princ_name", stp);

    rpc_binding_set_auth_info(server_binding, server_princ_name,
			       rpc_c_protect_level_default, authn_svc,
			       login_context, authz_svc, stp);
    CHECK_STATUS("rpc_binding_set_auth_info", stp);

    printf("calling dce_aud_start_with_server_binding\n");

    dce_aud_start_with_server_binding(TestCase1, server_binding, aud_c_evt_all_info,
				       aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start_with_server_binding 1", stp);

    dce_aud_print(in_ard, 0, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);

    CHECK_STATUS("dce_aud_commit 1", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, 0, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case1 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    printf("\naudit record:\n %s\n", out_buff);

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);
    *stp = (error_status_t) 0;
    printf("TestCase19 succeeded\n");
}


void
aud_test_case20(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    char	      *byte_string = "byte_string";
    char	      *char_string = "char_string";
    uuid_t	      new_uuid;
    idl_hyper_int     hyper_int;
    idl_uhyper_int    uhyper_int;
    unsigned32	      trail_size_smaller = 100;
    char	     *value_string_ptr;

    printf("Start aud_test_case20\n");
    fflush(stdout);

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    printf("after rpc_binding_inq_auth_client\n");
    fflush(stdout);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE1,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    /*
     *	since DCEAUDITTRAILSIZE is not set, the following call
     *	should set the trail size limit, overriding the hardcoded default.
     */

    dce_aud_set_trail_size_limit(in_trail, trail_size_smaller, stp);
    CHECK_STATUS("dce_aud_set_trail_size_limit", stp);

    printf("calling dce_aud_start\n");

 /* write to trail in a loop, so as to generate backed up trails */

 for (i=0; i< 20; i++)
 {
    dce_aud_start(TestCase20, h, aud_c_evt_all_info, aud_c_esl_cond_success,
	&in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);
 }

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
}


void
aud_test_case21(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i, rc;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    char	      *byte_string = "byte_string";
    char	      *char_string = "char_string";
    uuid_t	       new_uuid;
    idl_hyper_int      hyper_int;
    idl_uhyper_int     uhyper_int;
    char	      *trail_size_bigger = "DCEAUDITTRAILSIZE=2000";
    unsigned32	       trail_size_smaller = 100;
    char	      *value_string_ptr;

    printf("Start aud_test_case21\n");
    fflush(stdout);

    /* Test Environment Cleanup */
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    printf("after rpc_binding_inq_auth_client\n");
    fflush(stdout);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    rc = putenv(trail_size_bigger);
    CHECK_STATUS("putenv", &rc);

    value_string_ptr = (char *) getenv("DCEAUDITTRAILSIZE");
    printf("DCEAUDITTRAILSIZE = %s\n", value_string_ptr);

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE2,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    /*
     *	since DCEAUDITTRAILSIZE is set, the following call
     *	should not affect the trail size limit set by DCEAUDITTRAILSIZE.
     */

    dce_aud_set_trail_size_limit(in_trail, trail_size_smaller, stp);
    CHECK_STATUS("dce_aud_set_trail_size_limit", stp);

    printf("calling dce_aud_start\n");

 /* write to trail in a loop, so as to generate backed up trails */

 for (i=0; i< 20; i++)
 {
    dce_aud_start(TestCase21, h, aud_c_evt_all_info, aud_c_esl_cond_success,
	&in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);
 }

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
}

void
aud_test_case22(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i, rc;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    char	      *byte_string = "byte_string";
    char	      *char_string = "char_string";
    uuid_t	       new_uuid;
    idl_hyper_int      hyper_int;
    idl_uhyper_int     uhyper_int;
    char	      *trail_size_bigger = "DCEAUDITTRAILSIZE=2000";
    unsigned32	       trail_size_smaller = 100;
    char	      *value_string_ptr;

    printf("Start aud_test_case22\n");
    fflush(stdout);

    /* Test Environment Cleanup */
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    printf("after rpc_binding_inq_auth_client\n");
    fflush(stdout);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    rc = putenv(trail_size_bigger);
    CHECK_STATUS("putenv", &rc);

    value_string_ptr = (char *) getenv("DCEAUDITTRAILSIZE");
    printf("DCEAUDITTRAILSIZE = %s\n", value_string_ptr);

    /*
     *	open trail file with "wrap" storage strategy
     *	should produce only one trail file, wrapping around when full
     */

    dce_aud_open(aud_c_trl_open_write | aud_c_trl_ss_wrap, TRAIL_FILE3,
		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start\n");

 /* write to trail in a loop, so as to ensure wraparound */

 for (i=0; i< 20; i++)
 {
    dce_aud_start(TestCase22, h, aud_c_evt_all_info, aud_c_esl_cond_success,
	&in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);
 }

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    *stp = (error_status_t) 0;
}

void
aud_test_case23(
	handle_t	h,
	error_status_t	*stp
)
{
    int 	       i;
    rpc_authz_handle_t privs;
    unsigned32	       authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    char	      *byte_string = "byte_string";
    char	      *char_string = "char_string";
    uuid_t	       new_uuid;
    idl_hyper_int      hyper_int;
    idl_uhyper_int     uhyper_int;
    boolean32	       info_remaining;
    char	       predicate[2048];
    unsigned_char_t   *string_uuid;
    uuid_t	       manager_types[SEC_ACL_MGR_TYPES_SIZE_AVAIL];
    uuid_t	       manager_type;
    sec_acl_handle_t   acl_handle = NULL;
    sec_acl_list_t     acl_list;
    sec_acl_t	       sec_acl;
    unsigned32	       size_used;
    unsigned32	       num_types;
    char	      *entry_name = "/.:/sec/group";
    boolean32	       bind_to_entry = 0;


    printf("Start aud_test_case23\n");
    fflush(stdout);

    strcpy(predicate, "garbage");

    /* Test Environment Cleanup */
    unlink(TRAIL_FILE);
    unlink(TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    printf("after rpc_binding_inq_auth_client\n");
    fflush(stdout);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce))
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE,
       		 FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start\n");

    dce_aud_start(TestCase23, h, aud_c_evt_all_info, aud_c_esl_cond_success,
	&in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_small_int;
    audit_evt_info.data.small_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_short_int;
    audit_evt_info.data.short_int = 20;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 30;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    hyper_int.high = 1111;
    hyper_int.low  = 2222;

    audit_evt_info.format = aud_c_evt_info_hyper_int;
    audit_evt_info.data.hyper_int = hyper_int;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_usmall_int;
    audit_evt_info.data.usmall_int = 50;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_ushort_int;
    audit_evt_info.data.ushort_int = 60;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_ulong_int;
    audit_evt_info.data.ulong_int = 70;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    uhyper_int.high = 3333;
    uhyper_int.low  = 4444;

    audit_evt_info.format = aud_c_evt_info_uhyper_int;
    audit_evt_info.data.uhyper_int = uhyper_int;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_short_float;
    audit_evt_info.data.short_float = 90;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_long_float;
    audit_evt_info.data.long_float = 100;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_boolean;
    audit_evt_info.data.boolean = 1;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    uuid_create(&new_uuid, stp);
    CHECK_STATUS("uuid_create", stp);

    audit_evt_info.format = aud_c_evt_info_uuid;
    audit_evt_info.data.uuid = new_uuid;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_byte_string;
    audit_evt_info.data.byte_string = (idl_byte *)byte_string;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    audit_evt_info.format = aud_c_evt_info_char_string;
    audit_evt_info.data.char_string = (idl_char *)char_string;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    sec_acl_bind((unsigned_char_t *)entry_name, bind_to_entry, &acl_handle, stp);
    CHECK_STATUS("sec_acl_bind", stp);

    sec_acl_get_manager_types (acl_handle,
			       sec_acl_type_object,
			       SEC_ACL_MGR_TYPES_SIZE_AVAIL,
			       &size_used,
			       &num_types,
			       manager_types,
			       stp);
    CHECK_STATUS("sec_acl_get_manager_types", stp);

    memcpy ((char *)&manager_type, (char *)&manager_types[0], sizeof (uuid_t));

    sec_acl_lookup (acl_handle,
		    &manager_type,
		    sec_acl_type_object,
		    &acl_list,
		    stp);
    CHECK_STATUS("sec_acl_lookup", stp);

    audit_evt_info.format = aud_c_evt_info_acl;
    audit_evt_info.data.acl = acl_list.sec_acls[0];
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, aud_c_evt_raw_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_print(in_ard, aud_c_evt_groups_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_print(in_ard, aud_c_evt_delegates_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_print(in_ard, aud_c_evt_address_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_print(in_ard, aud_c_evt_specific_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_print(in_ard, aud_c_evt_all_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    /* negative test, use NULL ard */
    dce_aud_length(NULL, stp);
    if (*stp == 0)
    {
	printf("aud_test_case23 failed, dce_aud_length accepts NULL ard\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
		   aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);
    dce_aud_prev(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    /* negative test, use NULL trail file pointer */
    dce_aud_prev(NULL, NULL, 0, &out_ard, stp);
    if (*stp == 0)
    {
	printf("aud_test_case23 failed, dce_aud_prev accepts NULL at\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    dce_aud_print(out_ard, aud_c_evt_all_info, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    /* negative test, use NULL audit record descriptor */
    dce_aud_print(NULL, aud_c_evt_all_info, &out_buff, stp);
    if (*stp == 0)
    {
	printf("aud_test_case23 failed, dce_aud_print accepts NULL ard\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    end_in_buff = strstr((char *)in_buff, END_OF_REC_STR);
    end_out_buff = strstr((char *)out_buff, END_OF_REC_STR);
    if (strncmp(end_in_buff, end_out_buff, END_OF_REC_STR_LEN) != 0) {
	printf("aud_test_case23 failed, record in differs from record out\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    printf("\naudit record:\n %s\n", out_buff);

    dce_aud_get_header(out_ard, &header, stp);
    CHECK_STATUS("dce_aud_get_header", stp);

    dce_aud_free_header(header, stp);
    CHECK_STATUS("dce_aud_free_header", stp);

    /* negative test, use NULL audit record descriptor */
    dce_aud_get_header(NULL, &header, stp);
    if (*stp == 0)
    {
	printf("aud_test_case23 failed, dce_aud_get_header accepts NULL ard\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    /* negative test, use NULL, shouldn't crash */
    dce_aud_free_header(NULL, stp);

    info_remaining = TRUE;
    while (info_remaining)
    {
       dce_aud_get_ev_info(out_ard, &out_info, stp);
       CHECK_STATUS("dce_aud_get_ev_info", stp);

       if(out_info == NULL)
       {
	   info_remaining = FALSE;
       }
       else
       {
	   dce_aud_free_ev_info(out_info, stp);
	   CHECK_STATUS("dce_aud_free_ev_info", stp);
       }
    }

    /* negative test, use NULL audit record descriptor */
    dce_aud_get_ev_info(NULL, &out_info, stp);
    if (*stp == 0)
    {
	printf("aud_test_case23 failed, dce_aud_get_ev_info accepts NULL ard\n");
	*stp = (error_status_t) 1;
	exit(1);
    }

    /* negative test, use NULL, shouldn't crash... */
    dce_aud_free_ev_info(NULL, stp);

    dce_aud_discard(out_ard, stp);

    /* negative test, use NULL audit record descriptor */
    dce_aud_discard(NULL, stp);  /* shouldn't crash... */

    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    /* test predicate input for dce_aud_prev */
    dce_aud_open(aud_c_trl_open_read, TRAIL_FILE,
		 0, 0, &out_trail, stp);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "EVENT=200");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "OUTCOME=SUCCESS");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "OUTCOME=DENIAL");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "OUTCOME=FAILURE");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "OUTCOME=PENDING");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "OUTCOME=PENDING,OUTCOME=DENIAL,OUTCOME=FAILURE");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "OUTCOME=GARBAGE");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "STATUS=NAME");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "STATUS=:DCE");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "STATUS=GARBAGE");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "TIME=>20");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "TIME=<20");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    uuid_to_string(&new_uuid, &string_uuid, stp);
    CHECK_STATUS("uuid_to_string", stp);

    strcpy(predicate, "SERVER=");
    strcat(predicate, (char *)string_uuid);
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "CLIENT=");
    strcat(predicate, (char *)string_uuid);
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "CELL=");
    strcat(predicate, (char *)string_uuid);
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "GROUP=");
    strcat(predicate, (char *)string_uuid);
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    strcpy(predicate, "ADDR=HERE");
    dce_aud_prev(out_trail, &predicate[0], 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_prev", stp);

    *stp = (error_status_t) 0;
    printf("TestCase23 succeeded\n");
}

