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
 * $Log: aud_testf_rpc_manager.c,v $
 * Revision 1.1.6.2  1996/07/09  15:06:26  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:53 UTC  arvind  /main/arvind_audit122/2]
 *
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:28 UTC  arvind  /main/arvind_audit122/1]
 *
 * Revision 1.1.6.1  1996/06/04  22:13:21  arvind
 * 	Fix errors caused by mismatched types caught by RIOS compiler
 * 	[1995/12/12  21:08 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
 * 
 * 	Merge fix for CHFts16115
 * 	[1995/08/18  20:11 UTC  kline_s  /main/HPDCE02/2]
 * 
 * 	CHFts16115: Remove DES_HIDDEN conditional - only using integrity
 * 	protection level instead of using the privacy protection level
 * 	for Domestic.  Required to avoid conflicts due to singular
 * 	Domestic/Export release.
 * 	[1995/08/17  21:32 UTC  kline_s  /main/kline_s_mothra/2]
 * 
 * 	Enable tests to function properly using international (ie. non-DES) bits.
 * 	[1995/07/06  19:45 UTC  kline_s  /main/kline_s_mothra/1]
 * 
 * Revision 1.1.4.2  1996/02/17  23:26:04  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:02  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  21:51:14  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/08/18  20:11 UTC  kline_s
 * 	Merge fix for CHFts16115
 * 
 * 	HP revision /main/kline_s_mothra/2  1995/08/17  21:32 UTC  kline_s
 * 	CHFts16115: Remove DES_HIDDEN conditional - only using integrity
 * 	protection level instead of using the privacy protection level
 * 	for Domestic.  Required to avoid conflicts due to singular
 * 	Domestic/Export release.
 * 
 * 	HP revision /main/kline_s_mothra/1  1995/07/06  19:45 UTC  kline_s
 * 	Enable tests to function properly using international (ie. non-DES) bits.
 * 	[1995/12/11  20:53:33  root]
 * 
 * Revision 1.1.2.3  1994/10/04  21:23:38  weisz
 * 	CR 12167: api_filter test suite fails.
 * 	[1994/10/04  21:21:29  weisz]
 * 
 * Revision 1.1.2.2  1994/06/10  20:45:30  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:17:04  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/28  18:33:23  weisz
 * 	CR 10402: initial submission.
 * 	[1994/04/28  18:05:13  weisz]
 * 
 * $EndLog$
 */
#include <stdio.h>

#include <dce/dce.h>
#include <dce/id_base.h>
#include <dce/secidmap.h>
#include <dce/rpc.h>
#include <dce/pgo.h>

#include <dce/audit.h>
#include <aud_testf.h>
#include <aud_testp.h>
#include <error_statusp.h>

#include <tet_api.h>
#define first_event_number 0x300
#define number_of_events 5
#define delete_filter 0x300
#define show_filter 0x301
#define list_filter 0x302
#define add_filter 0x303
#define remove_filter 0x304

#define CHECK_STATUS(where, status) \
if (*status != 0) { \
    printf("\n%s returns bad status= %lx\n", (char *)where, *status); \
    return; \
}

extern boolean32 debug_mode;

extern boolean32 audit;

void
aud_testf_case1(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testf_case1\n");
    fflush(stdout);

   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      fflush(stdout);
      exit(1);
   }    
    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("rgy_site_open", stp);


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
       printf("wrong authn_level or wrong authz_svc, test failed\n");
       return;
    }
    sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                 &((sec_id_pac_t *) privs)->principal.uuid, subject_name, stp);
    CHECK_STATUS("sec_rgy_pgo_id_to_name", stp);
    printf("\nclient is %s\n", subject_name);
    fflush(stdout);

    printf("\ncalling dce_aud_open \n"); 
    fflush(stdout);
    /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  

    if (*stp != 0)
    {
       printf("\ndce_aud_open returns bad status= %lx\n", *stp); 
       fflush(stdout);
    }

    printf("\ndce_aud_start\n");
    fflush(stdout);
                                     
    /* the aud_test_princ1 has been prepared with a filter for event class */
    /* modify, and delete_filter is an event in this class                 */

    dce_aud_start(delete_filter, h, aud_c_evt_all_info, aud_c_esl_cond_success,
                                                                 &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    if (in_ard == NULL)
    {
	printf("\naud_testf_case1 failed\n");
        dce_aud_close(in_trail, stp);  
        sec_rgy_site_close(rgy_context, stp);
        *stp = 1;
    }
    else
    {
        printf("TestCase1 succeeded\n");
        dce_aud_close(in_trail, stp);  
        sec_rgy_site_close(rgy_context, stp);
        *stp = 0;
     }

}

void
aud_testf_case2(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testf_case2\n");
    fflush(stdout);


   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      fflush(stdout);
      exit(1);
   }    
    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("rgy_site_open", stp);

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
      return;
    }
    sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                 &((sec_id_pac_t *) privs)->principal.uuid, subject_name, stp);
    CHECK_STATUS("sec_rgy_pgo_id_to_name", stp);
    printf("\nclient is %s\n", subject_name);

    printf("\ncalling dce_aud_open \n"); 
    fflush(stdout);
    /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  

    if (*stp != 0)
    {
      printf("\ndce_aud_open returns bad status= %lx\n", *stp); 
      fflush(stdout);
    }

    printf("\ndce_aud_start\n");     
                                     
    /* using auditd audit events */
    /* the aud_test_group1 has been prepared with a filter for event class */
    /* query, and list_filter is part of this class                        */

    dce_aud_start(list_filter, h, aud_c_evt_all_info,
                                      aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    if (in_ard == NULL)
    {
      printf("\naud_testf_case2 failed\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 1;
    }
    else
    {
      printf("TestCase2 succeeded\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 0;
     }
}


void
aud_testf_case3(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testf_case3\n");

   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      fflush(stdout);
      exit(1);
   }    

    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("rgy_site_open", stp);

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
 	return;
    }
    sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                 &((sec_id_pac_t *) privs)->principal.uuid, subject_name, stp);
    CHECK_STATUS("sec_rgy_pgo_id_to_name", stp);
    printf("\nclient is %s\n", subject_name);

    printf("\ncalling dce_aud_open \n"); 
    fflush(stdout);
    /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  

    if (*stp != 0)
    {
      printf("\ndce_aud_open returns bad status= %lx\n", *stp); 
      fflush(stdout);
    }

    printf("\ndce_aud_start\n");
                                     
    /* using auditd audit events */
    /* the cell has been prepared with a filter for event class query  */
    /* and show_filter is an event in this class                       */

    dce_aud_start(show_filter, h, aud_c_evt_all_info,
                                      aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    if (in_ard == NULL)
    {
      printf("\naud_testf_case3 failed\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 1;
    }
    else
    {
      printf("TestCase3 succeeded\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 0;
     }
}

void
aud_testf_case4(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testf_case4\n");

   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      fflush(stdout);
      exit(1);
   }    

    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("rgy_site_open", stp);

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
 	return;
    }
    sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                 &((sec_id_pac_t *) privs)->principal.uuid, subject_name, stp);
    CHECK_STATUS("sec_rgy_pgo_id_to_name", stp);
    printf("\nclient is %s\n", subject_name);

    printf("\ncalling dce_aud_open \n"); 
    fflush(stdout);
    /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  

    if (*stp != 0)
    {
      printf("\ndce_aud_open returns bad status= %lx\n", *stp); 
      fflush(stdout);
    }

    printf("\ndce_aud_start\n");
                                     
    /* using auditd audit events */
    /* the wolrd has been prepared with a filter for event class modify  */
    /* and delete_filter is an event in this class                         */

    dce_aud_start(delete_filter, h, aud_c_evt_all_info,
                                      aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    if (in_ard == NULL)
    {
      printf("\naud_testf_case4 failed\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 1;
    }
    else
    {
      printf("TestCase4 succeeded\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 0;
     }
}

void
aud_testf_case5(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testf_case5\n");

   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      fflush(stdout);
      exit(1);
   }    

    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("rgy_site_open", stp);

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
 	return;
    }
    sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                 &((sec_id_pac_t *) privs)->principal.uuid, subject_name, stp);
    CHECK_STATUS("sec_rgy_pgo_id_to_name", stp);
    printf("\nclient is %s\n", subject_name);

    printf("\ncalling dce_aud_open \n"); 
    fflush(stdout);
    /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  

    if (*stp != 0)
    {
      printf("\ndce_aud_open returns bad status= %lx\n", *stp); 
      fflush(stdout);
    }

    printf("\ndce_aud_start\n");
                                     
    /* using auditd audit events */
    /* a world overridable filter was set for event class query      */
    /* and show_filter is an event in this class                     */

    dce_aud_start(show_filter, h, aud_c_evt_all_info,
                                      aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    if (in_ard == NULL)
    {
      printf("\naud_testf_case5 failed\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 1;
    }
    else
    {
      printf("TestCase5 succeeded\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 0;
     }
}

void
aud_testf_case6(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testf_case6\n");

   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      fflush(stdout);
      exit(1);
   }    

    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("rgy_site_open", stp);

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
 	return;
    }
    sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                 &((sec_id_pac_t *) privs)->principal.uuid, subject_name, stp);
    CHECK_STATUS("sec_rgy_pgo_id_to_name", stp);
    printf("\nclient is %s\n", subject_name);

    printf("\ncalling dce_aud_open \n"); 
    fflush(stdout);
    /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  

    if (*stp != 0)
    {
      printf("\ndce_aud_open returns bad status= %lx\n", *stp); 
      fflush(stdout);
    }

    printf("\ndce_aud_start\n");
                                     
    /* using auditd audit events */
    /* a world overridable filter was set for event class query      */
    /* and show_filter is an event in this class                     */

    dce_aud_start(show_filter, h, aud_c_evt_all_info,
                                      aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    if (in_ard != NULL)
    {
      printf("\naud_testf_case6 failed\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 1;
    }
    else
    {
      printf("TestCase6 succeeded\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 0;
     }
}

void
aud_testf_case7(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testf_case7\n");

   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      fflush(stdout);
      exit(1);
   }    

    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("rgy_site_open", stp);

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
 	return;
    }
    sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                 &((sec_id_pac_t *) privs)->principal.uuid, subject_name, stp);
    CHECK_STATUS("sec_rgy_pgo_id_to_name", stp);
    printf("\nclient is %s\n", subject_name);

    printf("\ncalling dce_aud_open \n"); 
    fflush(stdout);
    /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  

    if (*stp != 0)
    {
      printf("\ndce_aud_open returns bad status= %lx\n", *stp); 
      fflush(stdout);
    }

    printf("\ndce_aud_start\n");
                                     
    /* using auditd audit events */
    /* a world overridable filter was set for event class query      */
    /* and show_filter is an event in this class                     */

    dce_aud_start(show_filter, h, aud_c_evt_all_info,
                                      aud_c_esl_cond_success, &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    if (in_ard != NULL)
    {
      printf("\naud_testf_case7 failed\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 1;
    }
    else
    {
      printf("TestCase7 succeeded\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 0;
     }

}


void
aud_testf_case8(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testf_case8\n");
    fflush(stdout);

   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      fflush(stdout);
      exit(1);
   }    
    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("rgy_site_open", stp);


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
       printf("wrong authn_level or wrong authz_svc, test failed\n");
       return;
    }
    sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                 &((sec_id_pac_t *) privs)->principal.uuid, subject_name, stp);
    CHECK_STATUS("sec_rgy_pgo_id_to_name", stp);
    printf("\nclient is %s\n", subject_name);
    fflush(stdout);

    printf("\ncalling dce_aud_open \n"); 
    fflush(stdout);
    /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  

    if (*stp != 0)
    {
      printf("\ndce_aud_open returns bad status= %lx\n", *stp); 
      fflush(stdout);
    }

    printf("\ndce_aud_start\n");
    fflush(stdout);
                                     
    /* the aud_test_princ1 has been prepared with a filter for event class */
    /* modify, and delete_filter is an event in this class                 */
    /* action demanded is alarm on condition success                       */
    /* alarm message should display on the screen                          */

    dce_aud_start(delete_filter, h, aud_c_evt_all_info, aud_c_esl_cond_success,
                                                                 &in_ard, stp);
    CHECK_STATUS("dce_aud_start", stp);

    dce_aud_commit(in_trail, in_ard, 0, 0, aud_c_esl_cond_success, stp);

    if (*stp != 0)
    {
      printf("\naud_testf_case8 failed\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 1;
    }
    else
    {
      printf("TestCase8 succeeded\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 0;
     }

}


void
aud_testf_case9(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testf_case9\n");
    fflush(stdout);

   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      fflush(stdout);
      exit(1);
   }    
    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("rgy_site_open", stp);


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
       printf("wrong authn_level or wrong authz_svc, test failed\n");
       return;
    }
    sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                 &((sec_id_pac_t *) privs)->principal.uuid, subject_name, stp);
    CHECK_STATUS("sec_rgy_pgo_id_to_name", stp);
    printf("\nclient is %s\n", subject_name);
    fflush(stdout);

    printf("\ncalling dce_aud_open \n"); 
    fflush(stdout);
    /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  

    if (*stp != 0)
    {
      printf("\ndce_aud_open returns bad status= %lx\n", *stp); 
      fflush(stdout);
    }

    printf("\ndce_aud_start\n");
    fflush(stdout);
                                     
    /* the aud_test_princ1 has been prepared with a filter for event class */
    /* modify, and delete_filter is an event in this class                 */
    /* action demanded is alarm on condition denial                        */
    /* alarm message should display on the screen                          */

    dce_aud_start(delete_filter, h, aud_c_evt_all_info, aud_c_esl_cond_denial,
                                                                 &in_ard, stp);

    CHECK_STATUS("dce_aud_start", stp);

    dce_aud_commit(in_trail, in_ard, 0, 0, aud_c_esl_cond_denial, stp);

    if (*stp != 0)
    {
      printf("\naud_testf_case9 failed\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 1;
    }
    else
    {
      printf("TestCase9 succeeded\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 0;
     }
}


void
aud_testf_case10(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testf_case10\n");
    fflush(stdout);

   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      fflush(stdout);
      exit(1);
   }    
    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("rgy_site_open", stp);


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
       printf("wrong authn_level or wrong authz_svc, test failed\n");
       return;
    }
    sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                 &((sec_id_pac_t *) privs)->principal.uuid, subject_name, stp);
    CHECK_STATUS("sec_rgy_pgo_id_to_name", stp);
    printf("\nclient is %s\n", subject_name);
    fflush(stdout);

    printf("\ncalling dce_aud_open \n"); 
    fflush(stdout);
    /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  

    if (*stp != 0)
    {
      printf("\ndce_aud_open returns bad status= %lx\n", *stp); 
      fflush(stdout);
    }

    printf("\ndce_aud_start\n");
    fflush(stdout);

/*                                     
    the aud_test_princ1 has been prepared with a filter for event class
    modify, and delete_filter is an event in this class             
    action demanded is alarm but condition is unknown at start time 
    audit record should be dumped to the screen, since conditions match
    at commit time
*/
    dce_aud_start(delete_filter, h, aud_c_evt_all_info, aud_c_esl_cond_unknown,
                                                                 &in_ard, stp);

    CHECK_STATUS("dce_aud_start", stp);

    dce_aud_commit(in_trail, in_ard, 0, 0, aud_c_esl_cond_denial, stp);

    if (*stp != 0)
    {
      printf("\naud_testf_case10 failed\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 1;
    }
    else
    {
      printf("TestCase10 succeeded\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 0;
     }
}


void
aud_testf_case11(
        handle_t	h,
        error_status_t  *stp
)
{
    int                i;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_trail_t    in_trail;
    dce_aud_ev_info_t  audit_evt_info;
    dce_aud_rec_t      in_ard, out_ard;
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testf_case11\n");
    fflush(stdout);

   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      fflush(stdout);
      exit(1);
   }    
    sec_rgy_site_open(NULL, &rgy_context, stp);
    CHECK_STATUS("rgy_site_open", stp);


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
       printf("wrong authn_level or wrong authz_svc, test failed\n");
       return;
    }
    sec_rgy_pgo_id_to_name(rgy_context, sec_rgy_domain_person,
                 &((sec_id_pac_t *) privs)->principal.uuid, subject_name, stp);
    CHECK_STATUS("sec_rgy_pgo_id_to_name", stp);
    printf("\nclient is %s\n", subject_name);
    fflush(stdout);

    printf("\ncalling dce_aud_open \n"); 
    fflush(stdout);
    /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  

    if (*stp != 0)
    {
      printf("\ndce_aud_open returns bad status= %lx\n", *stp); 
      fflush(stdout);
    }

    printf("\ndce_aud_start\n");
    fflush(stdout);
/*                                     
    the aud_test_princ1 has been prepared with a filter for event class 
    modify, and delete_filter is an event in this class              
    action demanded is alarm but condition is unknown at start time  
    audit record should not be dumped to the screen, since filter is set for
    condition denial.
*/
    dce_aud_start(delete_filter, h, aud_c_evt_all_info, aud_c_esl_cond_unknown,
                                                                 &in_ard, stp);

    CHECK_STATUS("dce_aud_start", stp);

    dce_aud_commit(in_trail, in_ard, 0, 0, aud_c_esl_cond_success, stp);

    if (*stp != 0)
    {
	printf("\naud_testf_case11 failed\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 1;
    }
    else
    {
      printf("TestCase11 succeeded\n");
      dce_aud_close(in_trail, stp);  
      sec_rgy_site_close(rgy_context, stp);
      *stp = 0;
     }

}


