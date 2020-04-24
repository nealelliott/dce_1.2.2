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
 * $Log: aud_testd_rpc_manager.c,v $
 * Revision 1.1.6.1  1996/07/09  15:10:17  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:29 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/17  23:26:13  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/17  23:26:13  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:07  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  21:51:38  root
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
 * 	[1995/12/11  20:53:46  root]
 * 
 * Revision 1.1.2.4  1994/09/29  18:18:38  weisz
 * 	CR 12406: with the advent of 9834, server field might differ, so don't compare
 * 	[1994/09/29  18:18:08  weisz]
 * 
 * Revision 1.1.2.3  1994/06/10  20:45:59  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:17:21  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  18:18:59  weisz
 * 	CR 10596: set auditd ACL for log permission from start
 * 	[1994/05/10  18:18:35  weisz]
 * 
 * Revision 1.1.2.1  1994/04/20  19:36:45  weisz
 * 	initial submission
 * 	[1994/04/20  19:28:01  weisz]
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <string.h>

#include <dce/dce.h>
#include <dce/id_base.h>
#include <dce/secidmap.h>
#include <dce/rpc.h>
#include <dce/pgo.h>

#include <dce/audit.h>
#include <aud_testd.h>
#include <aud_testdp.h>
#include <error_statusp.h>

#include <tet_api.h>

#define FIRST_EVT_NUMBER 995
#define NUMBER_OF_EVTS     5

#define TestCase1        999
#define TestCase2        998
#define TestCase3        997
#define TestCase4        996
#define TestCase5        995

#define CHECK_STATUS(where, status) \
if (*status != 0) { \
    printf("\n%s returns bad status= %lx\n", where, *status); \
    exit(1); \
}

extern boolean32 debug_mode;

extern boolean32 audit;

void
aud_testd_case1(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    int                fail = 1;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_trail_t    out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    char               client[] = "Client";
    char              *client_in_buff, *client_out_buff;

   printf("Start aud_testd_case1\n");

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
     * invoking dce_aud_open without a trail file name, indicates to the 
     * auditd to use its central trail for auditing this process. 
     */
    dce_aud_open(aud_c_trl_open_write, NULL, 
		FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    printf("dce_aud_open returns in_trail ptr = %lx\n", in_trail);

    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start\n");

    dce_aud_start(TestCase1, h, aud_c_evt_all_info | aud_c_evt_always_log,
                                        aud_c_esl_cond_success, &in_ard, stp);
    printf("dce_aud_start returns in_ard ptr = %lx\n", in_ard);
    CHECK_STATUS("dce_aud_start", stp);

    dce_aud_print(in_ard, 0, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);    

    printf("\ndce_aud_print (in) event:\n %s\n", in_buff);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync_no_wait, 0,
                                                 aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, DAEMON_TRAIL_FILE, 
                                               0, 0, &out_trail, stp);
    printf("dce_aud_open returns out_trail ptr = %lx\n", out_trail);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    printf("dce_aud_next returns out_ard ptr = %lx\n", out_ard);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, 0, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    printf("\ndce_aud_print (out) event record:\n %s\n", out_buff);

    client_in_buff = strstr((char *)in_buff, client);
    client_out_buff = strstr((char *)out_buff, client);

    if (strncmp(client_in_buff, client_out_buff, strlen(client_in_buff) - 10) == 0) 
    {  
       fail = 0;
    }

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    if (fail != 0) 
    {  
       printf("aud_testd_case1 failed, record in differs from record out\n");
       *stp = 1;
    }
    else
    {  
       printf("aud_testd_case1 succeeded\n");
       *stp = 0;
    }                     
    
}

void
aud_testd_case2(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    int                fail = 1;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_trail_t    out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;
    char              *client_in_buff, *client_out_buff;
    char               client[] = "Client";

   printf("Start aud_testd_case2\n");

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce)) 
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
 	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, NULL, 
		FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    printf("dce_aud_open returns in_trail ptr = %lx\n", in_trail);
    CHECK_STATUS("dce_aud_open", stp);

    dce_aud_start(TestCase2, h, aud_c_evt_all_info | aud_c_evt_always_log,
                                        aud_c_esl_cond_success, &in_ard, stp);
    printf("dce_aud_start returns in_ard ptr = %lx\n", in_ard);
    CHECK_STATUS("dce_aud_start", stp);

    audit_evt_info.format = aud_c_evt_info_long_int;
    audit_evt_info.data.long_int = 10;
    dce_aud_put_ev_info(in_ard, audit_evt_info, stp);
    CHECK_STATUS("dce_aud_put_ev_info", stp);

    dce_aud_print(in_ard, aud_c_evt_all_info, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);

    printf("\ndce_aud_print (in) event:\n %s\n", in_buff);

    dce_aud_length(in_ard, stp);
    CHECK_STATUS("dce_aud_length", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync, 0,
			aud_c_esl_cond_success, stp);
    CHECK_STATUS("dce_aud_commit", stp);

    dce_aud_close(in_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    dce_aud_open(aud_c_trl_open_read, DAEMON_TRAIL_FILE, 
                                                  0, 0, &out_trail, stp);
    printf("dce_aud_open returns in_trail ptr = %lx\n", out_trail);
    CHECK_STATUS("dce_aud_open (for read)", stp);

    dce_aud_next(out_trail, NULL, 0, &out_ard, stp);
    CHECK_STATUS("dce_aud_next", stp);

    dce_aud_print(out_ard, aud_c_evt_all_info, &out_buff, stp);
    CHECK_STATUS("dce_aud_print (out)", stp);

    printf("\ndce_aud_print (out) event record:\n %s\n", out_buff);

    client_in_buff = strstr((char *)in_buff, client);
    client_out_buff = strstr((char *)out_buff, client);

    if (strncmp(client_in_buff, client_out_buff, strlen(client_in_buff) - 10) == 0) 
    {  
       fail = 0;
    }

    dce_aud_discard(out_ard, stp);
    CHECK_STATUS("dce_aud_discard", stp);

    dce_aud_close(out_trail, stp);
    CHECK_STATUS("dce_aud_close", stp);

    if (fail != 0) 
    {  
       printf("aud_testd_case2 failed, record in differs from record out\n");
       *stp = 1;
    }
    else
    {  
       printf("aud_testd_case2 succeeded\n");
       *stp = 0;
    }                     

}


void
aud_testd_case3(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;

   printf("Start aud_testd_case3\n");

    rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    CHECK_STATUS("rpc_binding_inq_auth_client", stp);
    if ((authn_level != rpc_c_protect_level_pkt_integ) &&
	    (authz_svc != rpc_c_authz_dce)) 
    {
	printf("wrong authn_level or wrong authz_svc, test failed, exiting\n");
 	exit(1);
    }

    dce_aud_open(aud_c_trl_open_write, NULL, 
		FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    printf("dce_aud_open returns in_trail ptr = %lx\n", in_trail);
    printf("dce_aud_open stp= %lx", *stp);                              
    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start\n");

    dce_aud_start(TestCase1, h, aud_c_evt_all_info | aud_c_evt_always_log,
                                        aud_c_esl_cond_success, &in_ard, stp);
    printf("dce_aud_start stp= %lx\n", *stp);                              
    CHECK_STATUS("dce_aud_start", stp);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync_no_wait, 0,
                                                 aud_c_esl_cond_success, stp);

    if (*stp == 0)
    {
       printf("aud_testd_case3 failed\n");
       *stp = 1;
    }
    else
    {
       printf("dce_aud_commit stp= %lx\n", *stp);                              
       printf("aud_testd_case3 succeeded\n");
       *stp = 0;
    }
}


void
aud_testd_case4(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    int                fail = 1;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;

   printf("Start aud_testd_case4\n");

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
     * invoking dce_aud_open without a trail file name, indicates to the 
     * auditd to use its central trail for auditing this process. 
     */
    dce_aud_open(aud_c_trl_open_write, NULL, 
		FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    printf("dce_aud_open returns in_trail ptr = %lx\n", in_trail);

    CHECK_STATUS("dce_aud_open", stp);

    printf("calling dce_aud_start\n");

    dce_aud_start(TestCase4, h, aud_c_evt_all_info | aud_c_evt_always_log,
                                        aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    dce_aud_print(in_ard, 0, &in_buff, stp);
    CHECK_STATUS("dce_aud_print (in)", stp);    

    printf("\ndce_aud_print (in) event:\n %s\n", in_buff);

    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync_no_wait, 0,
                                                 aud_c_esl_cond_success, stp);

    if (*stp == 0) 
    {  
       printf("aud_testd_case4 failed - record logged despite acl\n");
       dce_aud_close(in_trail, stp);
       *stp = 1;
    }
    else
    {  
       printf("aud_testd_case4 succeeded\n");
       dce_aud_close(in_trail, stp);
       *stp = 0;
    }                     
    
}


void
aud_testd_case5(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    int                fail = 1;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail, out_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    unsigned_char_t   *in_buff, *out_buff;

   printf("Start aud_testd_case5\n");

    /* assumption is that auditd is not running */

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
     * invoking dce_aud_open without a trail file name, indicates to the 
     * auditd to use its central trail for auditing this process. 
     */
    printf("calling dce_aud_open\n");
    dce_aud_open(aud_c_trl_open_write, NULL, 
		FIRST_EVT_NUMBER, NUMBER_OF_EVTS, &in_trail, stp);
    printf("dce_aud_open returns in_trail ptr = %lx\n", in_trail);
    printf("dce_aud_open stp= %lx\n", *stp);

    printf("calling dce_aud_start\n");
    dce_aud_start(TestCase5, h, aud_c_evt_all_info | aud_c_evt_always_log,
                                        aud_c_esl_cond_success, &in_ard, stp);
    printf("dce_aud_start stp= %lx\n", *stp);

    printf("calling dce_aud_commit\n");
    dce_aud_commit(in_trail, in_ard, aud_c_evt_commit_sync_no_wait, 0,
                                                 aud_c_esl_cond_success, stp);
    printf("dce_aud_commit stp= %lx\n", *stp);

    if (*stp == 0) 
    {  
       printf("aud_testd_case5 failed\n");
       *stp = 1;
    }
    else
    {  
       printf("aud_testd_case5 succeeded\n");
       *stp = 0;
    }                     
}

