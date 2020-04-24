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
 * $Log: aud_testf.c,v $
 * Revision 1.1.6.1  1996/07/09  15:06:02  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:27 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/17  23:26:02  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/17  23:26:02  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:01  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  21:51:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/18  20:11 UTC  kline_s
 * 	Merge fix for CHFts16115
 * 
 * 	HP revision /main/kline_s_mothra/3  1995/08/17  21:32 UTC  kline_s
 * 	CHFts16115: Remove DES_HIDDEN conditional - only using integrity
 * 	protection level instead of using the privacy protection level
 * 	for Domestic.  Required to avoid conflicts due to singular
 * 	Domestic/Export release.
 * 
 * 	HP revision /main/kline_s_mothra/2  1995/07/06  19:45 UTC  kline_s
 * 	Enable tests to function properly using international (ie. non-DES) bits.
 * 
 * 	HP revision /main/kline_s_mothra/1  1995/06/27  01:58 UTC  kline_s
 * 	$TET_ROOT incorrectly assumes path of /tet/test instead of /tet.
 * 	Modify all impacted paths so that /test is included in path specifications.
 * 	[1995/12/11  20:53:32  root]
 * 
 * Revision 1.1.2.4  1994/10/04  21:23:36  weisz
 * 	CR 12167: api_filter test suite fails.
 * 	[1994/10/04  21:21:20  weisz]
 * 
 * Revision 1.1.2.3  1994/06/30  19:37:14  ohara
 * 	Merged with changes from 1.1.2.2
 * 	[1994/06/30  19:36:45  ohara]
 * 
 * 	include <stdio.h>
 * 	[1994/06/27  19:58:50  ohara]
 * 
 * Revision 1.1.2.2  1994/06/10  20:45:28  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:17:02  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/28  18:33:18  weisz
 * 	CR 10402: initial submission
 * 	[1994/04/28  17:37:13  weisz]
 * 
 * $EndLog$
 */
#include <fcntl.h>
#include <locale.h>

#include <pthread.h>
#include <dce/dce.h>
#include <stdio.h>
#include <dce/dce_cf.h>
#include <dce/rpc.h>
#include <dce/sec_login.h>

#include <dce/audit.h>
#include <aud_testp.h>
#include <aud_testf.h>

#include <error_statusp.h>

#include <tet_api.h>

#define TASK_STACK_SIZE    64*1024
#define MAX_HOSTNAME_LEN   255 


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


int tp1(), tp2(), tp3(), tp4(), tp5(), tp6(), tp7(), tp8(), tp9(), tp10(), tp11();
/*
void startup(), cleanup();

void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] =
{
  { tp1, 1} ,
  { tp2, 2} ,
  { tp3, 3} ,
  { tp4, 4} ,
  { tp5, 5} ,
  { tp6, 6} ,
  { tp7, 7} ,
  { tp8, 8} ,
  { tp9, 9} ,
  { tp10, 10} ,
  { tp11, 11} ,
  { NULL, 0} 
};

 void startup()
{
   printf("Start api_filter test suite\n");
   fflush(stdout);
   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      tet_infoline("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      exit(1);
   }    
   system("$TET_ROOT/test/functional/security/audit/ts/api_filter/startauditd"); 
   return(0);
}

 void cleanup()
{
   system("$TET_ROOT/test/functional/security/audit/ts/api_filter/stopauditd");
#   system("rm /opt/dcelocal/var/audit/esl-* ");
   tet_infoline("End api_filter test suite");
   printf("End api_filter test suite\n");
   fflush(stdout);
   return(0);
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
    printf("listener START\n");
    rpc_server_listen(CONCURRENCY_LEVEL, &st);
    if (st != rpc_s_ok)
    {
      printf("rpc_server_listen error st= %lx\n", st);
      exit(1);
    }
    non_dt_fatal_error = TRUE;
    pthread_cancel(distinguished_task);
    pthread_exit((void *)-1);
    return(0);
}



 int tp1()
{                                               
  int   princ = 1; 
  int   status, pid, wpid; 
  int   i, j;
  int   rc;
  FILE *fptr;
  void (*test_case1) () = aud_testf_v1_0_c_epv.aud_testf_case1;

  system("$TET_ROOT/test/functional/security/audit/ts/api_filter/addfilter.princ1");       

  /* aud_client_server spins off a listener thread for the whole suite */
  rc = aud_client_server(test_case1, princ);
  if (rc != 0)
    overall_status |= tp1_error;

  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/delfilter.princ1)&");       

  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY)
       i++;
    j++; 
  }
    printf("deleting filter princ1 took: %i\n", j);
   fclose (fptr);
   remove (HANDSHAKE_FILE);
   return(0);
}


 int tp2()
{
  int   princ = 1; 
  int   status, pid, wpid; 
  int   i, j;
  int   rc;
  FILE *fptr;
  void (*test_case2) () = aud_testf_v1_0_c_epv.aud_testf_case2;


  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/addfilter.group1)&");       
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;   
    j++;
  }
    printf("adding filter group1 took: %i\n", j);
   fclose (fptr);
    remove (HANDSHAKE_FILE);

  rc = run_new_test(test_case2, princ); 
  if (rc != 0)
    overall_status |= tp2_error;


  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/delfilter.group1)&");
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }      
    printf("deleting filter group1 took: %i\n", j);
    fclose (fptr);
    remove (HANDSHAKE_FILE);
    return(0);

}


 int tp3()
{
 int    princ = 2; 
  int   status, pid, wpid; 
  int   i, j;
  int   rc;
  FILE *fptr;
 void (*test_case3) () = aud_testf_v1_0_c_epv.aud_testf_case3;


  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/addfilter.cell)&");

  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }
  printf("adding filter cell took: %i\n", j);
    fclose (fptr);
  remove (HANDSHAKE_FILE);


  rc = run_new_test(test_case3, princ); 
  if (rc != 0)
    overall_status |= tp3_error;


  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/delfilter.cell)&");
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }
    printf("deleting filter cell took: %i\n", j);
  fclose (fptr);
  remove (HANDSHAKE_FILE);

  return(0);

}


 int tp4()
{                                               
  int   princ = 1; 
  int   status, pid, wpid; 
  int   i, j;
  int   rc;
  FILE *fptr;
  void (*test_case4) () = aud_testf_v1_0_c_epv.aud_testf_case4;


  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/addfilter.world)&");
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }
    printf("adding filter world took: %i\n", j);
  fclose (fptr);
  remove (HANDSHAKE_FILE);


  rc = run_new_test(test_case4, princ); 
  if (rc != 0)
    overall_status |= tp4_error;

  
  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/delfilter.world)&");
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }
     printf("deleting filter world took: %i\n", j);
  fclose (fptr);
  remove (HANDSHAKE_FILE);
  return(0);

}


 int tp5()
{                                               
  int   princ = 1; 
  int   status, pid, wpid; 
  int   i, j;
  int   rc;
  FILE *fptr;
  void (*test_case5) () = aud_testf_v1_0_c_epv.aud_testf_case5;

  

  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/addfilter.worldover)&");

  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }
    printf("adding filter world overridable took: %i\n", j);
  fclose (fptr);
  remove (HANDSHAKE_FILE);


  rc = run_new_test(test_case5, princ); 
  if (rc != 0)
    overall_status |= tp5_error;

/*
   not removing the filter since needed in next test
*/
  return(0);
}


 int tp6()
{                                               
  int   princ = 1; 
  int   status, pid, wpid; 
  int   i, j;
  int   rc;
  FILE *fptr;
  void (*test_case6) () = aud_testf_v1_0_c_epv.aud_testf_case6;

/*
  overridable world filter carried over from previous test 
  just adding principal filter with no action
*/

  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/addfilter.princ1noac)&");
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }
     printf("adding filter princ1 with no action took: %i\n", j);
  fclose (fptr);
  remove (HANDSHAKE_FILE);


  rc = run_new_test(test_case6, princ); 
  if (rc != 0)
    overall_status |= tp6_error;


  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/delfilter.princ1)&");
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }
      printf("deleting filter princ1 took: %i\n", j);
  fclose (fptr);
  remove (HANDSHAKE_FILE);
  return(0);

}


 int tp7()
{                                               
  int   princ = 1; 
  int   status, pid, wpid; 
  int   i, j;
  int   rc;
  FILE *fptr;
  void (*test_case7) () = aud_testf_v1_0_c_epv.aud_testf_case7;

/*
  overridable world filter carried over from previous test 
  just adding group filter with no action, should override the world filter
*/
  
  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/addfilter.group1noac)&");
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }
      printf("adding filter group1 took: %i\n", j);
  fclose (fptr);
  remove (HANDSHAKE_FILE);


  rc = run_new_test(test_case7, princ); 
  if (rc != 0)
    overall_status |= tp7_error;



  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/delfilter.group1)&");
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }
     printf("deleting filter group1 took: %i\n", j);
  fclose (fptr);
  remove (HANDSHAKE_FILE);


  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/delfilter.worldover)&");
  fptr = NULL;
  j = 0; 
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }     
      printf("deleting filter world overridable took: %i\n", j);
  fclose (fptr);
  remove (HANDSHAKE_FILE);
  return(0);

}




 int tp8()
{
  int   princ = 1; 
  int   status, pid, wpid; 
  int   i, j;
  int   rc;
  FILE *fptr;
  void (*test_case8) () = aud_testf_v1_0_c_epv.aud_testf_case8;


  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/addfilter.princ1succalarm)&");       
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;   
       j++;
  }
    printf("adding filter princ1succalarm took: %i\n", j);
   fclose (fptr);
    remove (HANDSHAKE_FILE);

  rc = run_new_test(test_case8, princ); 
  if (rc != 0)
    overall_status |= tp8_error;


  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/delfilter.princ1)&");
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }      
     printf("deleting filter princ1 took: %i\n", j);
    fclose (fptr);
    remove (HANDSHAKE_FILE);
    return(0);

}



 int tp9()
{
  int   princ = 1; 
  int   status, pid, wpid; 
  int   i, j;
  int   rc;
  FILE *fptr;
  void (*test_case9) () = aud_testf_v1_0_c_epv.aud_testf_case9;


  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/addfilter.princ1denialarm)&");
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;   
    j++;
  }
    printf("adding filter princ1denialarm took: %i\n", j);
   fclose (fptr);
    remove (HANDSHAKE_FILE);
    i = 0;
    while (i < AUD_DELAY) 
       i++;   

  rc = run_new_test(test_case9, princ); 
  if (rc != 0)
    overall_status |= tp9_error;


/*  not removing princ1 denial alarm filter since it is used by next test  */
  return(0);

}



 int tp10()
{
  int   princ = 1; 
  int   status, pid, wpid; 
  int   i, j;
  int   rc;
  FILE *fptr;
  void (*test_case10) () = aud_testf_v1_0_c_epv.aud_testf_case10;


/* previous filter used here too */

  rc = run_new_test(test_case10, princ); 
  if (rc != 0)
    overall_status |= tp10_error;


/*  not removing princ1 denial alarm filter since it is used by next test  */
  return(0);

}



 int tp11()
{
  int   princ = 1; 
  int   status, pid, wpid; 
  int   i, j;
  int   rc;
  FILE *fptr;
  void (*test_case11) () = aud_testf_v1_0_c_epv.aud_testf_case11;


/* previous filter used here too */

  rc = run_new_test(test_case11, princ); 
  if (rc != 0)
    overall_status |= tp11_error;


  system("($TET_ROOT/test/functional/security/audit/ts/api_filter/delfilter.princ1)&");
  fptr = NULL;
  j = 0;
  while (fptr == NULL)
  {          
    i = 0;
    fptr = fopen(HANDSHAKE_FILE, "r");
    /* give some time to the listener thread to work */
    while (i < AUD_DELAY) 
       i++;
    j++;
  }      
     printf("deleting filter princ1 took: %i\n", j);
    fclose (fptr);
    remove (HANDSHAKE_FILE);
    return(0);

}




 int aud_client_server
    (
       void (*aud_test_case_n) () ,
       int princ
    )
{
    error_status_t       st;
    void               **thread_status;
    int			 i;
    pthread_t            listener_handle, keymgmt_handle;
    sec_login_handle_t   login_context;
    boolean32		 reset_passwd;
    sec_passwd_rec_t    *prec;
    char                 plain[9];
    char                 pw_princ1[] = "princ1";  
    char                 pw_princ2[] = "princ2";  
    sec_login_auth_src_t auth_src;
    dce_aud_trail_t      in_trail;
    char                *servername; 
    
                                                          
  struct timespec timer;
  int a = 0;
 timer.tv_nsec = 0; 
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

        rpc_server_register_if(aud_testf_v1_0_s_ifspec, 
                  (uuid_t *) NULL, (rpc_mgr_epv_t) NULL, &st);
        if (BAD_STATUS(&st)) {
	    printf("rpc_server_inq_bindings failed\n");
        }

        dce_cf_prin_name_from_host(NULL, &servername, &st);
        printf("servername= %s\n", servername);
        if (BAD_STATUS(&st)) {
	    printf("dce_cf_prin_name_from_name failed\n");
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

        printf("aud_client_server pw_princ1= %s, pw_princ2= %s\n", pw_princ1, pw_princ2);
         fflush(stdout);
                         
  prec = calloc(1, sizeof(sec_passwd_rec_t));
  if (prec == NULL) 
   {
      printf("ERROR: cannot allocate memory \n");
      exit(1);
   }
  else
   {
      printf("OK: memory allocated\n");
   }


        if (princ == 1)
        {
           sec_login_setup_identity((unsigned_char_t *)"aud_test_princ1",
                                     sec_login_no_flags, &login_context, &st);
           prec->key.tagged_union.plain = (unsigned_char_t *)strcpy(plain, pw_princ1);  
        }
        else
        { 
           sec_login_setup_identity((unsigned_char_t *)"aud_test_princ2",
                                     sec_login_no_flags, &login_context, &st);
           prec->key.tagged_union.plain = (unsigned_char_t *)strcpy(plain, pw_princ2);
        }

        if (BAD_STATUS(&st)) {
	    printf("sec_login_setup_identity , princ= %i,st= %lx\n", princ, st);
            fflush(stdout); 
        }
 

	prec->key.key_type = sec_passwd_plain;
        prec->pepper = NULL;
        prec->version_number = sec_passwd_c_version_none;

        printf("prec->key.tagged_union.plain = %s, plain= %s\n", prec->key.tagged_union.plain, plain);
            fflush(stdout); 

	sec_login_validate_identity(login_context, prec, &reset_passwd,
					&auth_src, &st); 

        free(prec);

        if (BAD_STATUS(&st)) {
	    printf("sec_login_validate_identity st= %lx\n", st);
            fflush(stdout); 
        }
 

	rpc_binding_set_auth_info(bindings->binding_h[0], 
				  (unsigned_char_t *) servername,
				  rpc_c_authn_level_pkt_integrity,
                                  rpc_c_authn_dce_private,
                                  (rpc_auth_identity_handle_t) login_context,
                                  rpc_c_authz_dce, &st);

        if (BAD_STATUS(&st)) {
	    printf("rpc_binding_set_auth_info st= %lx\n", st);
            fflush(stdout); 
        }
 

	aud_test_case_n(bindings->binding_h[0], &st); 
       
        if (st != 0)
        {
          sec_login_purge_context(&login_context, &st);
          return(1);
        }
        else
        {
	  sec_login_purge_context(&login_context, &st);
          return(0);
        }
 

/* END OF THE LINES THAT MAY VARY BY TEST CASES */
    } CATCH_ALL {
	printf("caught exception\n");
	printf("exception = %lx\n", THIS_CATCH);
	exit(1);
    } ENDTRY;
}

 int run_new_test
    (
       void (*aud_test_case_n) () ,
       int princ
    )
{
    error_status_t       st;
    void               **thread_status;
    int			 i;
    pthread_t            listener_handle, keymgmt_handle;
    sec_login_handle_t   login_context;
    boolean32		 reset_passwd;
    sec_passwd_rec_t	*prec;
    char                 plain[9];
    char                 pw_princ1[] = "princ1";  
    char                 pw_princ2[] = "princ2";  
    sec_login_auth_src_t auth_src;
    dce_aud_trail_t      in_trail;
    char                *servername; 

                         
  prec = calloc(1, sizeof(sec_passwd_rec_t));
  if (prec == NULL) 
   {
      printf("ERROR: cannot allocate memory \n");
      fflush(stdout);
      exit(1);
   }


/* STARTING OF THE LINES THAT MAY VARY BY TEST CASES */
        printf("Start run_new_test\n");

   TRY 
   { 


        printf("aud_client_server pw_princ1= %s, pw_princ2= %s\n", pw_princ1, pw_princ2);
        fflush(stdout);


        if (princ == 1)
        {
           sec_login_setup_identity((unsigned_char_t *)"aud_test_princ1",
                                     sec_login_no_flags, &login_context, &st);
           
           prec->key.tagged_union.plain = (unsigned_char_t *)strcpy(plain, pw_princ1);  
           printf("\n princ1");
           fflush(stdout); 
        }
        else 
          if (princ == 2)
          { 
             sec_login_setup_identity((unsigned_char_t *)"aud_test_princ2",
                                       sec_login_no_flags, &login_context, &st);
             prec->key.tagged_union.plain = (unsigned_char_t *)strcpy(plain, pw_princ2);
             printf("\n princ2");
             fflush(stdout); 
          }
          else
          {
             printf("\nerror: principal= %d", princ);
             exit(1);
          } 

        if (BAD_STATUS(&st)) {
	    printf("sec_login_setup_identity , princ= %i,st= %lx\n", princ, st);
            fflush(stdout); 
        }
 
	prec->key.key_type = sec_passwd_plain;
        prec->pepper = NULL;
        prec->version_number = sec_passwd_c_version_none;


        printf("prec->key.tagged_union.plain = %s, plain= %s\n", prec->key.tagged_union.plain, plain);
            fflush(stdout); 

        sec_login_validate_identity(login_context, prec, &reset_passwd,
					&auth_src, &st); 

        free(prec);

        if (BAD_STATUS(&st)) {
	    printf("sec_login_validate_identity princ=%i, st= %lx\n", princ, st);
            fflush(stdout); 
        }

        dce_cf_prin_name_from_host(NULL, &servername, &st);
        printf("servername= %s\n", servername);
        if (BAD_STATUS(&st)) {
	    printf("dce_cf_prin_name_from_host failed st= %lx\n", st);
            fflush(stdout); 
        }

	rpc_binding_set_auth_info(bindings->binding_h[0], 
				  (unsigned_char_t *) servername,
				  rpc_c_authn_level_pkt_integrity,
                                  rpc_c_authn_dce_private,
                                  (rpc_auth_identity_handle_t) login_context,
                                  rpc_c_authz_dce, &st);

        if (BAD_STATUS(&st)) {
	    printf("rpc_binding_set_auth_info st= %lx\n", st);
            fflush(stdout); 
        }
 

        printf("\ninside the TRY-CATCH: principal= %d", princ);
	aud_test_case_n(bindings->binding_h[0], &st); 
	
        if (st != 0)
        {
          sec_login_purge_context(&login_context, &st);
          return(1);
        }
        else
        {
	  sec_login_purge_context(&login_context, &st);
          return(0);
        }

    }
    CATCH_ALL 
    {

	printf("\ncaught exception\n");
	printf("\nexception = %lx\n", THIS_CATCH);
	exit(1);

    }
    ENDTRY;
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
}

unsigned32 main()
{
printf("Start api_filter test suite\n");
if ((char *) getenv("DCEAUDITFILTERON") == NULL)
 {
   printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
   exit(1);
 }    
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
printf("End api_filter test suite overall_status= %lx\n", overall_status);
return(overall_status);
}
