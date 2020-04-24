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
 * $Log: aud_testec_rpc_manager.c,v $
 * Revision 1.1.6.1  1996/07/09  15:14:31  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:30 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/17  23:26:16  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/17  23:26:16  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:09  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  21:52:06  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/08/18  20:10 UTC  kline_s
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
 * 	[1995/12/11  20:53:58  root]
 * 
 * Revision 1.1.2.4  1994/10/04  21:23:44  weisz
 * 	CR 12167: api_filter test suite fails.
 * 	[1994/10/04  21:22:09  weisz]
 * 
 * Revision 1.1.2.3  1994/09/28  00:19:22  weisz
 * 	CR 10703: updated to take into consideration new audit events added.
 * 	[1994/09/28  00:18:25  weisz]
 * 
 * Revision 1.1.2.2  1994/06/10  20:46:24  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:17:35  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/20  18:09:21  weisz
 * 	initial submission
 * 	[1994/04/20  18:04:40  weisz]
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
#include <aud_testec.h>
#include <aud_testp.h>
#include <error_statusp.h>

#include <tet_api.h>
#define first_event_number 0x300
#define number_of_events 12
#define delete_filter 0x300
#define show_filter 0x301
#define list_filter 0x302
#define add_filter 0x303
#define remove_filter 0x304
#define new_event 0x30b

/*
static char msg[256];
*/

#define CHECK_STATUS(where, status) \
if (*status != 0) { \
    printf("\n%s returns bad status= %lx\n", (char *)where, *status); \
    return; \
}

extern boolean32 debug_mode;

extern boolean32 audit;

void
aud_testec_case1(
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
    dce_aud_hdr_t     *header;
    dce_aud_ev_info_t *out_info;
    char *in_buff,    *out_buff;
    sec_rgy_handle_t   rgy_context;
    sec_rgy_name_t     subject_name;

    printf("Start aud_testec_case1\n");

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
    printf("client is %s\n", subject_name);

 /* using auditd audit events */
    dce_aud_open(aud_c_trl_open_write, TRAIL_FILE, first_event_number,
                                             number_of_events, &in_trail, stp);  


    CHECK_STATUS("dce_aud_open", stp);

    printf("\ndce_aud_start\n");
                                     
    /* event class dce_audit_filter_query has been modified to contain the */
    /* event new_event, and the world filter is set for this class and     */
    /* condition success, therefore the ard should be non-NULL.            */

    dce_aud_start(new_event, h, aud_c_evt_all_info, aud_c_esl_cond_success,
                                                                 &in_ard, stp);

    if (in_ard == NULL)
    {
	printf("aud_testec_case1 failed\n");
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

