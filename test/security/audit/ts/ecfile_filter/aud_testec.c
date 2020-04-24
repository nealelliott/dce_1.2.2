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
 * $Log: aud_testec.c,v $
 * Revision 1.1.6.1  1996/07/09  15:14:08  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:30 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/17  23:26:15  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/17  23:26:15  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:08  marty]
 * 
 * Revision 1.1.4.1  1995/12/11  21:52:01  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/18  20:10 UTC  kline_s
 * 	Merge fix for CHFts16115
 * 
 * 	HP revision /main/kline_s_mothra/3  1995/08/17  21:31 UTC  kline_s
 * 	CHFts16115: Remove DES_HIDDEN conditional - only using integrity
 * 	protection level instead of using the privacy protection level
 * 	for Domestic.  Required to avoid conflicts due to singular
 * 	Domestic/Export release.
 * 
 * 	HP revision /main/kline_s_mothra/2  1995/07/06  19:45 UTC  kline_s
 * 	Enable tests to function properly using international (ie. non-DES) bits.
 * 
 * 	HP revision /main/kline_s_mothra/1  1995/06/27  02:17 UTC  kline_s
 * 	$TET_ROOT incorrectly assumes path of /tet/test instead of /tet.
 * 	Modify all impacted paths so that /test is included in path specifications.
 * 	[1995/12/11  20:53:57  root]
 * 
 * Revision 1.1.2.5  1994/06/29  13:16:38  weisz
 * 	CR 10764: allocate memory for prec.
 * 	[1994/06/28  20:37:16  weisz]
 * 
 * Revision 1.1.2.4  1994/06/10  22:35:50  zeliff
 * 	ot 10764: fixing problem with core dump
 * 	[1994/06/10  22:34:50  zeliff]
 * 
 * Revision 1.1.4.2  1994/06/10  21:50:22  weisz
 * 	CR 10764: added return code checking after security calls.
 * 
 * Revision 1.1.2.2  1994/04/21  16:49:43  zeliff
 * 	ot10419: include stdio.h
 * 	[1994/04/21  16:49:09  zeliff]
 * 
 * Revision 1.1.2.1  1994/04/20  18:09:17  weisz
 * 	initial submission
 * 	[1994/04/20  18:04:29  weisz]
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
#include <aud_testp.h>
#include <aud_testec.h>

#include <error_statusp.h>

#include <tet_api.h>

#define TASK_STACK_SIZE     64*1024
#define MAX_HOSTNAME_LEN    255

#define tp1_error           0x00000001

int tp1();
/*
void startup(), cleanup();
void (*tet_startup) () = startup;
void (*tet_cleanup) () = cleanup;
struct tet_testlist tet_testlist[] =
{
  { tp1, 1} ,
  { NULL, 0} 
};    

 void startup()
{
   tet_infoline("Start ecfile_filter test suite");
   printf("Start ecfile_filter test suite\n");
   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      tet_infoline("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      exit(1);
   }    
   system("$TET_ROOT/test/functional/security/audit/ts/ecfile_filter/startauditd"); 
}

 void cleanup()
{
   system("$TET_ROOT/test/functional/security/audit/ts/ecfile_filter/stopauditd"); 
   system("rm /opt/dcelocal/var/audit/esl-* ");
   tet_infoline("End ecfile_filter test suite");
   printf("End ecfile_filter test suite\n");
   return(o);
}
*/

pthread_attr_t  attr;
boolean32       non_dt_fatal_error = false;
pthread_t       distinguished_task;
unsigned32      overall_status;

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



 int tp1()
{
  int rc;
  void (*test_case1) () = aud_testec_v1_0_c_epv.aud_testec_case1;

  system("$TET_ROOT/test/functional/security/audit/ts/ecfile_filter/addevent.toec"); 
  system("$TET_ROOT/test/functional/security/audit/ts/ecfile_filter/addfilter.world"); 
  rc = aud_client_server(test_case1); 
  if (rc != 0)
     overall_status |= tp1_error;
  system("$TET_ROOT/test/functional/security/audit/ts/ecfile_filter/restore.ec"); 
  return(0);
}

 int aud_client_server
    (
       void (*aud_test_case_n) ()
    )
{
    error_status_t       st, remote_st;
    void               **thread_status;
    int			 i;
    pthread_t            listener_handle, keymgmt_handle;
    sec_login_handle_t   login_context;
    boolean32		 reset_passwd;
    sec_passwd_rec_t	*prec;
    sec_login_auth_src_t auth_src;
    char                *servername; 
    char                 pw_princ1[] = "princ1";
    char                 plain[9];

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

        rpc_server_register_if(aud_testec_v1_0_s_ifspec, 
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
        sec_login_setup_identity((unsigned_char_t *)"aud_test_princ1",
                                  sec_login_no_flags, &login_context, &st);
        if (st != 0) return(1);

        prec = calloc(1, sizeof(sec_passwd_rec_t));
        if (prec == NULL)
        {
           printf("ERROR: cannot allocate memory \n");
           exit(1);
        }
        else
        {
           printf("OK: memory allocated \n");
        }

        prec->key.tagged_union.plain = (unsigned char *)strcpy(plain, pw_princ1);

	prec->key.key_type = sec_passwd_plain;
        prec->pepper = NULL;
        prec->version_number = sec_passwd_c_version_none;

	sec_login_validate_identity(login_context, prec, &reset_passwd,
					&auth_src, &st); 
        free(prec);
        if (st != 0) return(1);

	rpc_binding_set_auth_info(bindings->binding_h[0], 
				  (unsigned_char_t *) servername,
				  rpc_c_authn_level_pkt_integrity,
                                  rpc_c_authn_dce_private,
                                  (rpc_auth_identity_handle_t) login_context,
                                  rpc_c_authz_dce, &st);
        if (st != 0) return(1);

	aud_test_case_n(bindings->binding_h[0], &st); 
        remote_st = st;
	sec_login_purge_context(&login_context, &st);

        system("$TET_ROOT/test/functional/security/audit/ts/ecfile_filter/delfilter.world&"); 
        if (remote_st != 0)
          return(1);
        else
          return(0);

/* END OF THE LINES THAT MAY VARY BY TEST CASES */
    } CATCH_ALL {
	printf("caught exception\n");
	printf("exception = %lx\n", THIS_CATCH);
	exit(1);
    } ENDTRY;
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

unsigned32 main()
{
   printf("Start ecfile_filter test suite\n");
   if ((char *) getenv("DCEAUDITFILTERON") == NULL)
   {
      printf("ERROR: filtering environment variable DCEAUDITFILTERON not set\n");
      exit(1);
   }    
   system("$TET_ROOT/test/functional/security/audit/ts/ecfile_filter/startauditd"); 

tp1();

printf("End ecfile_filter test suite\n");
return(overall_status);
}
