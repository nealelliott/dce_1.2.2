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
 * $Log: aud_test.c,v $
 * Revision 1.1.6.1  1996/07/09  15:08:29  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:28 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/17  23:26:06  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/17  23:26:06  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:03  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  21:51:27  root
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
 * 	HP revision /main/kline_s_mothra/1  1995/06/27  02:07 UTC  kline_s
 * 	Clean up functions to actually provide return values (and thereby
 * 	fixing all the compile-time warnings.
 * 	[1995/12/11  20:53:39  root]
 * 
 * Revision 1.1.2.6  1994/09/27  17:32:37  weisz
 * 	CR 10703: added tests for trail full situation.
 * 	[1994/09/27  17:31:22  weisz]
 * 
 * Revision 1.1.2.5  1994/08/30  17:21:39  weisz
 * 	CR 11884: added new API - dce_aud_start_with_uuid
 * 	[1994/08/30  17:18:18  weisz]
 * 
 * Revision 1.1.2.4  1994/08/17  19:56:58  weisz
 * 	CR 11443: added PENDING as a new valid outcome.
 * 	[1994/08/17  19:55:26  weisz]
 * 
 * Revision 1.1.2.3  1994/06/10  20:45:48  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:17:12  devsrc]
 * 
 * Revision 1.1.2.2  1994/04/21  16:49:39  zeliff
 * 	ot10419: include stdio.h
 * 	[1994/04/21  16:48:33  zeliff]
 * 
 * Revision 1.1.2.1  1994/04/20  13:27:54  weisz
 * 	initial submission
 * 	[1994/04/20  13:26:08  weisz]
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
#include <dce/dceaudmsg.h>
#include <dce/dceaudmac.h>
#include <dce/dceaudsvc.h>
#include <aud_testp.h>
#include <aud_test.h>

#include <error_statusp.h>

#include <tet_api.h>

#define TASK_STACK_SIZE     64*1024
#define MAX_HOSTNAME_LEN    255 

#define tp1_error           0x00000001
#define tp2_error           0x00000002
#define tp3_error           0x00000004
#define tp4_error           0x00000008
#define tp5_error           0x00000010
#define tp6_error           0x00000020
#define tp7_error           0x00000040
#define tp8_error           0x00000080
#define tp9_error           0x00000100
#define tp10_error          0x00000200
#define tp11_error          0x00000400
#define tp12_error          0x00000800
#define tp13_error          0x00001000
#define tp14_error          0x00002000
#define tp15_error          0x00004000
#define tp16_error          0x00008000
#define tp17_error          0x00010000
#define tp18_error          0x00020000
#define tp19_error          0x00040000
#define tp20_error          0x00080000
#define tp21_error          0x00100000
#define tp22_error          0x00200000
#define tp23_error          0x00400000

int tp1() , tp2() , tp3(), tp4(), tp5(), tp6(), tp7(), tp8(), \
    tp9(), tp10(), tp11(), tp12(), tp13(), tp14(), tp15(), tp16(), \
    tp17(), tp18(), tp19(), tp20(), tp21(), tp22(), tp23();



/*
*/
static char msg[256];   

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
 int   rc;
 void (*test_case1) () = aud_test_v1_0_c_epv.aud_test_case1;
 rc = aud_client_server(test_case1); 
 if (rc != 0)
   overall_status |= tp1_error;
 printf("tp1: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp2()
{
 int   rc;
 void (*test_case2) () = aud_test_v1_0_c_epv.aud_test_case2;
 rc = run_new_test(test_case2); 
 if (rc != 0)
   overall_status |= tp2_error;
 printf("tp2: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}


int tp3()
{
 int   rc;
 void (*test_case3) () = aud_test_v1_0_c_epv.aud_test_case3;
 rc = run_new_test(test_case3); 
 if (rc != 0)
   overall_status |= tp3_error;
 printf("tp3: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp4()
{
 int   rc;
 void (*test_case4) () = aud_test_v1_0_c_epv.aud_test_case4;
 rc = run_new_test(test_case4); 
 if (rc != 0)
   overall_status |= tp4_error;
 printf("tp4: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp5()
{
 int   rc;
 void (*test_case5) () = aud_test_v1_0_c_epv.aud_test_case5;
 rc = run_new_test(test_case5); 
 if (rc != 0)
   overall_status |= tp5_error;
 printf("tp5: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}


int tp6()
{
 int   rc;
 void (*test_case6) () = aud_test_v1_0_c_epv.aud_test_case6;
 rc = run_new_test(test_case6); 
 if (rc != 0)
   overall_status |= tp6_error;
 printf("tp6: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp7()
{
 int   rc;
 void (*test_case7) () = aud_test_v1_0_c_epv.aud_test_case7;
 rc = run_new_test(test_case7); 
 if (rc != 0)
   overall_status |= tp7_error;
 printf("tp7: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}        

int tp8()
{
 int   rc;
 void (*test_case8) () = aud_test_v1_0_c_epv.aud_test_case8;
 rc = run_new_test(test_case8); 
 if (rc != 0)
   overall_status |= tp8_error;
 printf("tp8: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp9()
{
 int   rc;
 void (*test_case9) () = aud_test_v1_0_c_epv.aud_test_case9;
 rc = run_new_test(test_case9); 
 if (rc != 0)
   overall_status |= tp9_error;
 printf("tp9: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp10()
{
 int   rc;
 void (*test_case10) () = aud_test_v1_0_c_epv.aud_test_case10;
 rc = run_new_test(test_case10); 
 if (rc != 0)
   overall_status |= tp10_error;
 printf("tp10: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp11()
{
 int   rc;
 void (*test_case11) () = aud_test_v1_0_c_epv.aud_test_case11;
 rc = run_new_test(test_case11); 
 if (rc != 0)
   overall_status |= tp11_error;
 printf("tp11: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp12()
{
 int   rc;
 void (*test_case12) () = aud_test_v1_0_c_epv.aud_test_case12;
 rc = run_new_test(test_case12); 
 if (rc != 0)
   overall_status |= tp12_error;
 printf("tp12: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}


int tp13()
{
 int   rc;
 void (*test_case13) () = aud_test_v1_0_c_epv.aud_test_case13;
 rc = run_new_test(test_case13); 
 if (rc != 0)
   overall_status |= tp13_error;
 printf("tp13: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}


int tp14()
{
 int   rc;
 void (*test_case14) () = aud_test_v1_0_c_epv.aud_test_case14;
 rc = run_new_test(test_case14); 
 if (rc != 0)
   overall_status |= tp14_error;
 printf("tp14: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp15()
{
 int   rc;
 void (*test_case15) () = aud_test_v1_0_c_epv.aud_test_case15;
 rc = run_new_test(test_case15); 
 if (rc != 0)
   overall_status |= tp15_error;
 printf("tp15: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp16()
{
 int   rc;
 void (*test_case16) () = aud_test_v1_0_c_epv.aud_test_case16;
 rc = run_new_test(test_case16); 
 if (rc != 0)
   overall_status |= tp16_error;
 printf("tp16: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp17()
{
 int   rc;
 void (*test_case17) () = aud_test_v1_0_c_epv.aud_test_case17;
 rc = run_new_test(test_case17); 
 if (rc != 0)
   overall_status |= tp17_error;
 printf("tp17: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp18()
{
 int   rc;
 void (*test_case18) () = aud_test_v1_0_c_epv.aud_test_case18;
 rc = run_new_test(test_case18); 
 if (rc != 0)
   overall_status |= tp18_error;
 printf("tp18: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp19()
{
 int   rc;
 void (*test_case19) () = aud_test_v1_0_c_epv.aud_test_case19;
 rc = run_new_test(test_case19); 
 if (rc != 0)
   overall_status |= tp19_error;
 printf("tp19: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp20()
{
 int   rc;
 void (*test_case20) () = aud_test_v1_0_c_epv.aud_test_case20;
 rc = run_new_test(test_case20); 
 if (rc != 0)
   overall_status |= tp20_error;
 printf("tp20: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}

int tp21()
{
 int   rc;
 void (*test_case21) () = aud_test_v1_0_c_epv.aud_test_case21;
 rc = run_new_test(test_case21); 
 if (rc != 0)
   overall_status |= tp21_error;
 printf("tp21: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}


int tp22()
{
 int   rc;
 void (*test_case22) () = aud_test_v1_0_c_epv.aud_test_case22;
 rc = run_new_test(test_case22); 
 if (rc != 0)
   overall_status |= tp22_error;
 printf("tp22: rc= %i, overall_status= %lx\n", rc, overall_status);
 return(0);
}


int tp23()
{
 int   rc;
 void (*test_case23) () = aud_test_v1_0_c_epv.aud_test_case23;
 rc = run_new_test(test_case23); 
 if (rc != 0)
   overall_status |= tp23_error;
 printf("tp23: rc= %i, overall_status= %lx\n", rc, overall_status);
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

        rpc_server_register_if(aud_test_v1_0_s_ifspec, 
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

	aud_test_case_n(bindings->binding_h[0], &st); 

        if (st != 0)
           return(1);
        else
           return(0);

}

unsigned32 main()
{
printf("Start api_log test suite\n");
tp1();
tp2();
tp3();
tp4();
tp5();
tp6();
tp7();
tp8();
tp9();
tp10();
tp11();
tp12();
tp13();
tp14();
tp15();
tp16();
tp17();
tp18();
tp19();
tp20();
tp21();
tp22();
tp23();
printf("End api_log test suite - overall_status= %lx\n", overall_status);
return(overall_status);
}
