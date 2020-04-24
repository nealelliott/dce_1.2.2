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
 * $Log: aud_testd.c,v $
 * Revision 1.1.6.1  1996/07/09  15:09:51  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:29 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/17  23:26:11  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/17  23:26:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:06  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  21:51:36  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/18  20:10 UTC  kline_s
 * 	Merge fix for CHFts16115
 * 
 * 	HP revision /main/kline_s_mothra/3  1995/08/17  21:33 UTC  kline_s
 * 	CHFts16115: Remove DES_HIDDEN conditional - only using integrity
 * 	protection level instead of using the privacy protection level
 * 	for Domestic.  Required to avoid conflicts due to singular
 * 	Domestic/Export release.
 * 
 * 	HP revision /main/kline_s_mothra/2  1995/07/06  19:45 UTC  kline_s
 * 	Enable tests to function properly using international (ie. non-DES) bits.
 * 
 * 	HP revision /main/kline_s_mothra/1  1995/06/27  02:30 UTC  kline_s
 * 	$TET_ROOT incorrectly assumes path of /tet/test instead of /tet.
 * 	Modify all impacted paths so that /test is included in path specifications.
 * 	[1995/12/11  20:53:44  root]
 * 
 * Revision 1.1.2.5  1994/09/20  14:32:07  weisz
 * 	CR 12248: restore original auditd ACL at end of test.
 * 	[1994/09/20  14:31:38  weisz]
 * 
 * Revision 1.1.2.4  1994/06/10  20:45:57  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:17:18  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  18:18:57  weisz
 * 	CR 10596: set auditd ACL for log permission from start
 * 	[1994/05/10  18:18:28  weisz]
 * 
 * Revision 1.1.2.2  1994/04/21  16:49:41  zeliff
 * 	ot10419: include stdio.h
 * 	[1994/04/21  16:48:51  zeliff]
 * 
 * Revision 1.1.2.1  1994/04/20  19:36:43  weisz
 * 	initial submission
 * 	[1994/04/20  19:27:50  weisz]
 * 
 * $EndLog$
 */
#include <stdio.h>	
#include <fcntl.h>
#include <locale.h>

#include <pthread.h>
#include <dce/dce.h>
#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <dce/sec_login.h>

#include <dce/audit.h>
#include <aud_testd.h>
#include <aud_testdp.h>

#include <error_statusp.h>

#include <tet_api.h>

#define TASK_STACK_SIZE     64*1024
#define MAX_HOSTNAME_LEN    255

#define tp1_error           0x00000001
#define tp2_error           0x00000002
#define tp3_error           0x00000004
#define tp4_error           0x00000008
#define tp5_error           0x00000010

int tp1() , tp2() , tp3(), tp4(), tp5();

/*
static void startup(), cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] =
{
  { tp1, 1} ,
  { tp2, 2} ,
  { tp3, 3} ,
  { tp4, 4} ,
  { tp5, 5} ,
  { NULL, 0} 
};

static char msg[256];

static void startup()
{
   tet_infoline("Start api_log_to_daemon test suite");
}

static void cleanup()
{
   tet_infoline("End api_log_to_daemon test suite");
}
*/

pthread_attr_t  attr;
boolean32       non_dt_fatal_error = false;
pthread_t       distinguished_task;
unsigned32      overall_status = 0;

boolean32 audit = FALSE;

/*
 * Global (shared) variables
 */

boolean32 debug_mode = false;
rpc_binding_vector_p_t  bindings;

void *listener
  (
    void    *arg
  )
{
    error_status_t  st;

    CLEAR_STATUS(&st);
    rpc_server_listen(CONCURRENCY_LEVEL, &st);
    non_dt_fatal_error = TRUE;
    pthread_cancel(distinguished_task);
    pthread_exit((void *)-1);
    return(0);
}

compose_server_name
   (
     char *servername
   )
{
    char                name[MAX_HOSTNAME_LEN]; 
    char                *temp_ptr;

        strcpy(servername, "hosts/");
        gethostname(name, MAX_HOSTNAME_LEN); 
        temp_ptr = (char *) strchr(name, '.');
        if (temp_ptr != NULL)
           *temp_ptr = '\0';
        strcat(servername, name);
        strcat(servername, "/self");
        printf("servername= %s\n", servername);
        return(0);
}


int tp1()
{
 int rc;
 void (*test_case1) () = aud_testd_v1_0_c_epv.aud_testd_case1;

    unlink(DAEMON_TRAIL_FILE);
    unlink(DAEMON_TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);

    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/startauditd");
    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/restorelogperm");
                         
    rc = aud_client_server(test_case1); 
    if (rc != 0)
      overall_status |= tp1_error;
    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/stopauditd");

    /* Test Environment Cleanup - assumption is that auditd is not running */

    unlink(DAEMON_TRAIL_FILE);
    unlink(DAEMON_TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);
    return(0);
}


int tp2()
{
 int rc;
 void (*test_case2) () = aud_testd_v1_0_c_epv.aud_testd_case2;


    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/startauditd");

    rc = run_new_test(test_case2); 
    if (rc != 0)
      overall_status |= tp2_error;

    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/stopauditd");

    unlink(DAEMON_TRAIL_FILE);
    unlink(DAEMON_TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);
    return(0);
}


int tp3()
{
 int rc;
 void (*test_case3) () = aud_testd_v1_0_c_epv.aud_testd_case3;

    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/startauditd");

    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/disableauditd");

    rc = run_new_test(test_case3); 
    if (rc != 0)
      overall_status |= tp3_error;

    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/stopauditd");

    unlink(DAEMON_TRAIL_FILE);
    unlink(DAEMON_TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);
    return(0);
}


int tp4()
{
 int rc;
 void (*test_case4) () = aud_testd_v1_0_c_epv.aud_testd_case4;

    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/startauditd");

    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/setaclnologperm");

    rc = run_new_test(test_case4); 
    if (rc != 0)
      overall_status |= tp4_error;

    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/restorelogperm");
    system("$TET_ROOT/test/functional/security/audit/ts/api_log_to_daemon/stopauditd");

    unlink(DAEMON_TRAIL_FILE);
    unlink(DAEMON_TRAIL_INDEX_FILE);
    unlink(BINDING_FILE);
    return(0);
}


int tp5()
{
 int rc;
 void (*test_case5) () = aud_testd_v1_0_c_epv.aud_testd_case5;

    rc = run_new_test(test_case5); 
    if (rc != 0)
      overall_status |= tp5_error;
    return(0);
}


int aud_client_server
    (
       void (*aud_test_case_n) () 
    )
{
    error_status_t      st;
    void              **thread_status;
    int			i;
    pthread_t           listener_handle, keymgmt_handle;
    sec_login_handle_t  login_context;
    char               *servername; 

    if (chdir(WORKING_DIR) != 0) {
	perror("chdir");
    }


   distinguished_task = pthread_self();

   if (pthread_attr_create(&attr) == -1) {
       perror("Unable to create task attributes object.");
       return false;
   }

   if (chdir(WORKING_DIR) != 0) {
       perror("chdir");
   }


   distinguished_task = pthread_self();

   if (pthread_attr_create(&attr) == -1) {
        perror("Unable to create task attributes object.");
   }

   if (pthread_attr_setstacksize(&attr, TASK_STACK_SIZE) == -1) {
        perror("Unable to set task stack size attribute.");
        return false;
   }

   TRY { 
    
        pthread_testcancel();

	/* Must support IP */
        rpc_server_use_all_protseqs(CONCURRENCY_LEVEL, &st);
        if (BAD_STATUS(&st)) {
	    printf("rpc_server_use_protseq failed\n");
        }

        rpc_server_inq_bindings(&bindings, &st);
        if (BAD_STATUS(&st)) {
	    printf("rpc_server_inq_bindings failed\n");
        }

        rpc_server_register_if(aud_testd_v1_0_s_ifspec, 
                  (uuid_t *) NULL, (rpc_mgr_epv_t) NULL, &st);
        if (BAD_STATUS(&st)) {
	    printf("rpc_server_inq_bindings failed\n");
        }

        dce_cf_prin_name_from_host(NULL, &servername, &st);
        printf("servername= %s\n", servername);
        if (BAD_STATUS(&st)) {
	    printf("dce_cf_prin_name_from_host failed\n");
        }

        rpc_server_register_auth_info((unsigned_char_p_t) servername,
		rpc_c_authn_dce_private, (rpc_auth_key_retrieval_fn_t) NULL,
                (void *) NULL, &st);

        if (BAD_STATUS(&st)) {
	    printf("register_auth_info failed\n");
        }
 
        if (pthread_create(&listener_handle, attr, listener, NULL) == -1) {
	    printf("pthread_create listener_handle failed\n");
        } 
/* STARTING OF THE LINES THAT MAY VARY BY TEST CASES */

	sec_login_get_current_context(&login_context, &st);

	rpc_binding_set_auth_info(bindings->binding_h[0], 
				  (unsigned_char_t *) servername,
				  rpc_c_authn_level_pkt_integrity,
                                  rpc_c_authn_dce_private,
                                  (rpc_auth_identity_handle_t) login_context,
                                  rpc_c_authz_dce, &st);

	aud_test_case_n(bindings->binding_h[0], &st); 

        if (st != 0)
          return(1);
        else
          return(0);

/* END OF THE LINES THAT MAY VARY BY TEST CASES */
    } CATCH_ALL {
	printf("caught exception\n");
	exit(1);
    } ENDTRY;
}

int run_new_test
    (
       void (*aud_test_case_n) () 
    )
{
    error_status_t      st;
    void              **thread_status;
    int			i;
    pthread_t           listener_handle, keymgmt_handle;
    sec_login_handle_t  login_context;
    char               *servername; 

/* STARTING OF THE LINES THAT MAY VARY BY TEST CASES */

        printf("Start run_new_test\n");

	sec_login_get_current_context(&login_context, &st);

        dce_cf_prin_name_from_host(NULL, &servername, &st);

	rpc_binding_set_auth_info(bindings->binding_h[0], 
				  (unsigned_char_t *) servername,
				  rpc_c_authn_level_pkt_integrity,
                                  rpc_c_authn_dce_private,
                                  (rpc_auth_identity_handle_t) login_context,
                                  rpc_c_authz_dce, &st);

 TRY
 { 
	aud_test_case_n(bindings->binding_h[0], &st); 

        if (st != 0)
          return(1);
        else
          return(0);
 }
 CATCH_ALL
 {
	printf("caught exception\n");
	exit(1);
 }
 ENDTRY;

}


unsigned32 main() 
{
printf("Start api_log_to_daemon test suite\n");
tp1();
tp2();
tp3();
tp4();
tp5();
printf("End api_log_to_daemon test suite\n");
return(overall_status);
}
