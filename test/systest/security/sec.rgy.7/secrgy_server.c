/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: secrgy_server.c,v $
 * Revision 1.1.82.2  1996/02/17  23:28:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:47  marty]
 *
 * Revision 1.1.82.1  1995/12/11  22:46:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/05  17:36 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 	[1995/12/11  22:22:16  root]
 * 
 * Revision 1.1.79.1  1994/02/23  21:44:04  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:40:56  pellis]
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/31  15:52  bissen
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.5.2  1993/07/09  14:34:13  root
 * 	Initial King Kong branch
 * 	[1993/07/09  14:33:32  root]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/04  22:18:10  cjd]
 * 
 * 	SVR4 porting changes - 8596
 * 	[1993/10/15  16:35:31  ohara]
 * 
 * Revision 1.1.77.3  1993/10/14  17:37:18  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:36  cmckeen]
 * 
 * Revision 1.1.77.2  1993/10/07  20:49:10  dassarma
 * 	The function del_acct calls remove_acct but it did not
 * 	do a site_open_update and do_site_open close. Added
 * 	these calls in the appropiate places.
 * 	[1993/10/07  20:42:16  dassarma]
 * 
 * Revision 1.1.77.1  1993/09/09  19:56:22  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/09  19:51:09  cmckeen]
 * 
 * Revision 1.1.6.2  1993/07/02  16:38:33  eheller
 * 	GAMMERA version which runs under TET.
 * 	[1993/07/02  16:16:54  eheller]
 * 
 * Revision 1.1.3.2  1993/05/04  20:37:17  sekhar
 * 	Removed debugging print stmnts in add_acct() and del_acct().
 * 	[1993/05/04  20:10:47  sekhar]
 * 
 * 	1. redecalared passwd to be of type sec_passwd_str_t .
 * 
 * 	2. in main() null terminate the passwd variable. and
 * 	   copy only sec_passwd_str_max_len (currently 512) chars
 * 
 * 	3. free passwd_temp on error paths as well.
 * 
 * 	4. removed incorrect call to rpc_binding_vector_free()
 * 	   immediately after rpc_register_if (rpc_binding_inq_
 * 	   bindings was never called prior to this).
 * 
 * 	5. in get_pwent() removed extraneous (unused) variables
 * 	   and calls to bind to a master registry site. Also added
 * 	   call to sec_login_get_current_context() to initialize
 * 	   login_ctx .
 * 	[1993/05/03  20:09:59  sekhar]
 * 
 * 	Updated usage and increased error checks on server_name option.
 * 	[1993/03/31  18:14:48  eperkins]
 * 
 * 	Added login context to the log_in, out calls and cleaned up the status reporting
 * 	[1993/03/25  19:06:24  eperkins]
 * 
 * 	This required alot of rework to work around having multiple login contexts
 * 	cached within the runtime, problems with memory consumption during the
 * 	login-logout operation again because these contexts are cached. The server
 * 	registration is now done by a separate process and all previous cell_admin
 * 	context switching has been removed.
 * 	[1993/03/11  00:21:48  eperkins]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/05  14:47:06  cjd]
 * 
 * 	Initial version 1.0.2 system test sec.rgy.7
 * 	[1993/01/04  18:46:34  eperkins]
 * 
 * Revision 1.1.1.4  1993/04/21  20:28:27  root
 * 	    Loading drop DCE1_0_2B23
 * 
 * Revision 1.1.4.2  1993/02/04  22:18:10  cjd
 * 	Embedded copyright notice
 * 
 * $EndLog$
 */
#ifdef SNI_SVR4
#include <pthread.h>
#include <sys/signal.h>
#include <fcntl.h>
#else
#include <pthread.h>
#include <sys/signal.h>
#include <sys/file.h>
#endif /* SNI_SVR4 */
#include <stdlib.h>

#include <stdio.h>
#include <sys/time.h>

#include <dce/dce.h>
#include <dce/rgybase.h>

#include <pwd.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/passwd.h>
#include <string.h>

#include <krb5/krb5.h>
#include <krb5/los-proto.h>     /* for krb5_read_password */
#include <dce/rpc.h>
#include <dce/acct.h>

#include "rgy_test.h"
#include "rgy_clt.h"

rpc_mgmt_authorization_fn_t authz_fn_p;
boolean32 authz_func();


#define MANAGER_BUFFER_SIZE 2048
unsigned char manager_buffer[MANAGER_BUFFER_SIZE];
/* Maximum number of concurrent call requests that server can accept */
#define MAX_CALL_REQ 10

static unsigned char pipe_in[] = {"/opt/dcelocal/etc/passwd_override"};
static unsigned char *s;
static unsigned char server_name[MAX_NS_NAME] = {NULL};
sec_passwd_str_t   passwd;

static unsigned char 	cell_rel_name[MAX_NS_NAME]={"/.:"};	
static rpc_binding_vector_t *bindings;

typedef struct ref_thread_args {
	signed32		exp_int;
	char			*password;
	sec_login_handle_t   	*auth_id; /* server login context/credentials */
} ref_thread_args_t;
	

#ifdef ST_DEBUG
#define dbprintf(A) printf A
#else
#define dbprintf(A)
#endif

#define SERVER_UNIX_NUM     999
#define CLIENT_UNIX_NUM         998
#define KEY_TAB "/usr/tmp/v5srvtab"
#define MAX_SERVERS 20
#define SERVER_MAX_ARGS 25

char *protseqs[] = {
        "ncadg_ip_udp",
        "ncacn_ip_tcp"
        };

/* refresh thread synchronization datatypes */
pthread_cond_t refr_id_cond;
pthread_mutex_t refr_id_mutex;



extern error_status_t log_out ();
extern error_status_t do_site_open_query ();
extern error_status_t do_account_prep();
extern error_status_t remove_acct ();
extern error_status_t log_in ();
extern void err_prt();
sec_login_handle_t   auth_id;       /* server login context/credentials */
void maintain_id();
static char login_command[1024];

main (argc, argv)
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
   error_status_t    	tstatus = error_status_ok; /* returned from rpc */
   unsigned_char_p_t 	str_binding_p;     /* rpc_binding_to_string_binding */
   ndr_long_int 	fd;        /* open, write, close system calls */
   sec_passwd_str_t     upasswd;
   int                  c;
   extern char          *optarg;
   extern int           optind;
   short                argcount = 0;
   short                auth=0;
   short 		bind_to_entry = TRUE;
   signed32	 	id_exp = 0;
   signed32 		reg_id_exp = 0; /*credential expiration from rgy*/
   char			*cellname = NULL;
   rpc_ns_handle_t      import_context;
   unsigned_char_p_t    import_protseq;
   unsigned_char_p_t    string_binding;
   call_args_t          call_args[MAX_SERVERS];
   idl_char             mesg[MAX_SERVERS][80];
   int  		ret;
   int                  client_number = 1;
   unsigned_char_p_t    protseq = ( unsigned_char_p_t ) NULL;
   char			*passwd_temp;	
   long			cert_expire;
   long 		refresh_interval;
   pthread_t		thread_id;
   ref_thread_args_t	ref_thread_params;
   sec_passwd_rec_t     prec;
   boolean32		reset;
   sec_login_auth_src_t auth_src;
   boolean32            val;

	
   setbuf ((FILE*)stdout, (char *)NULL);
   setbuf ((FILE*)stderr, (char *)NULL); 

   /* Setup some pthread init. */
   pthread_setasynccancel (CANCEL_OFF);

   while((c = (int) getopt(argc, (char **)argv, "adc:n:p:t:i:")) != EOF) {
      switch (c) {
           case 'd' : /* debug */
                rpc__dbg_set_switches("0-3.5", &tstatus);
                break;
           case 'n' : /* server name */
                strncpy((char *) server_name, (char *) optarg, MAX_NS_NAME);
                argcount += 2;
                break;
           case 'c' : /* cell name */
                cellname = optarg;
                argcount += 2;
                break;
           case 'p':  /* server password */
                strncpy((char *)passwd, optarg, sec_passwd_str_max_len);
		passwd[sec_passwd_str_max_len] = '\0';
                argcount += 2;
                break;
           case 't':  /* client-server protocol */ 
                id_exp =  atol((char *) optarg);
                argcount += 2;
                break;
           case 'i':  /* client_server protocol */
                protseq = (unsigned_char_p_t) optarg;
                argcount += 2;
                break;
           case 'a' : /* authenticated */
		auth = 1;
                break;
        }
   }
   if (argcount == 0 || server_name == NULL || passwd == NULL) {
        printf("Usage: secrgy_server -n server_name -p server_password [ -d ]\n"
);
        printf ("\t[ -c cell name ] [ -a authenticated RPC ] \n");
        printf ("\t[ -i protocol sequence]\n");
        exit(1);
   }
	
   
	
   if ( (char *) cellname != (char *) NULL )
	strcpy ((char *) cell_rel_name,(char *) cellname);
   strcat((char *) cell_rel_name, "/subsys/");
   strcat ((char *) cell_rel_name, (char *) server_name);

   if (protseq == (unsigned_char_p_t) NULL) {
        protseq = (unsigned_char_p_t) protseqs[0];
   }
/*   dbprintf(("\tsecrgy_server: Login as server %s beginning\n", server_name));
   if ( tstatus = log_in (&auth_id, server_name, passwd) ){
        err_prt("log_in", tstatus);
        exit (tstatus);
   }*/

   /*
    * Add server to security data base.
    */

   dbprintf (("\tsecrgy_server: Calling rpc_server_use_all_protseqs\n"));

   /*
    * Register all supported protocol
    * sequences with the RPC runtime.
    */

   rpc_server_use_all_protseqs (MAX_CALL_REQ, &tstatus);
   if (tstatus != rpc_s_ok) {
	err_prt("rpc_server_use_all_protseqs", tstatus);
        do_acct_cleanup(server_name, &auth_id, &tstatus);
        exit (tstatus);
   }

   /*
    * Register an interface with the rpc runtime.
    * The if_spec was obtained from rgy_test.h
    * which was generated from rgy_test.idl.
    */

   dbprintf (("\tsecrgy_server: Calling rpc_server_register_if\n"));

   rpc_server_register_if (rgy_test_v1_0_s_ifspec, (uuid_p_t) NULL,
                           (rpc_mgr_epv_t) NULL, &tstatus);

   if (tstatus != rpc_s_ok) {
	err_prt("rpc_server_register_if", tstatus);
        do_acct_cleanup(server_name, &auth_id, &tstatus);
        exit (tstatus);
   }

   /*
    * Obtain this server's vector of binding handles.
    */

   dbprintf (("\tsecrgy_server: Calling rpc_server_inq_bindings\n"));

   rpc_server_inq_bindings (&bindings, &tstatus);

   if (tstatus != rpc_s_ok) {
	err_prt("rpc_server_inq_bindings", tstatus);
        do_acct_cleanup(server_name, &auth_id, &tstatus);
        exit (tstatus);
   }
 

   /* Register endpoints specified by rpc_server_use_all_protseqs. */

   dbprintf (("\tsecrgy_server: Calling rpc_ep_register\n"));

   rpc_ep_register (rgy_test_v1_0_s_ifspec, 
	bindings, 
	(uuid_vector_t *) NULL , 
	(unsigned_char_p_t) "secrgy_server", 
	&tstatus);

   if (tstatus != rpc_s_ok) {
	err_prt("rpc_ep_register", tstatus);
        rpc_binding_vector_free (&bindings, &tstatus);
        do_acct_cleanup(server_name, &auth_id, &tstatus);
        exit (tstatus);
   }



   if ( pthread_cond_init(&refr_id_cond, pthread_condattr_default) != 0 )
   {
      perror(" \n    Error initializing condition variable:");
      exit(1);
   }
   if ( pthread_mutex_init(&refr_id_mutex, pthread_mutexattr_default) != 0 )
   {
      perror(" \n    Error initializing mutex:");
      exit(1);
   }

   dbprintf(("\tsecrgy_server: Login as server %s beginning\n", server_name));
   if (sec_login_setup_identity (server_name, sec_login_no_flags,
			             &auth_id, &tstatus)){
   	if (tstatus) { 
		err_prt ("sec_login_setup_identity", tstatus);
		exit (tstatus);
	}
        /* sec_login_validate_id will nullify the password */
        passwd_temp = (char *) malloc (strlen ((char *) passwd) +1);
        if ( passwd_temp == (char *) NULL)
                return (tstatus = rpc_s_no_memory);
	strcpy( (char *)passwd_temp, (char *)passwd);

        /* Set the password information for validate_identity */
        prec.key.key_type = sec_passwd_plain;
        prec.key.tagged_union.plain = (idl_char *) passwd_temp;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

	val = sec_login_validate_identity (
                           auth_id,
                           &prec,
                           &reset, &auth_src, &tstatus);

	free(passwd_temp);
	if (val) {
                if (tstatus){
			err_prt("sec_login_valid_and_cert_ident", tstatus);
                        exit (tstatus);
                }
		if (!sec_login_certify_identity( auth_id, &tstatus))
			exit(-1); 
                if (auth_src == sec_login_auth_src_network) {
                        dbprintf(("\tsecrgy_server: network wide creds\n"));

                        sec_login_set_context (auth_id, &tstatus);
                        if (tstatus){
                                err_prt("sec_login_set_context", tstatus);
				exit (tstatus);
                        } 
                        sec_login_get_expiration (auth_id,
                                &cert_expire,
                                &tstatus);
                        if (tstatus){
                                err_prt ("sec_login_get_expiration", tstatus);
				exit (tstatus);
                        }
		}
	}
   }
   dbprintf(("\tsecrgy_server: Beginning NSI export entry:%s\n",cell_rel_name));
   rpc_ns_binding_export(rpc_c_ns_syntax_default, 
		(unsigned char *) cell_rel_name, 
		rgy_test_v1_0_s_ifspec, 
		bindings, 
		(uuid_vector_t *) NULL, 
		&tstatus); 
   if (tstatus) {
   	err_prt("rpc_ns_binding_export",tstatus);
        do_acct_cleanup(server_name, &auth_id, &tstatus);
        rpc_ep_unregister (rgy_test_v1_0_s_ifspec,
                          bindings,
                          (uuid_vector_t *) NULL,
                          &tstatus);
        rpc_binding_vector_free (&bindings, &tstatus);
        if (tstatus != rpc_s_ok) {
		err_prt("rpc_binding_vector_free", tstatus);
                exit (tstatus);
        }
        exit (tstatus);
   }
   rpc_mgmt_set_authorization_fn (authz_func, &tstatus);
   if (tstatus != rpc_s_ok) {
	err_prt("rpc_binding_vector_free", tstatus);
        exit (tstatus);
   }    

   /* Register for private key authentication. */

   if ( auth == 1) {
	dbprintf (("\tsecrgy_server: Calling rpc_server_register_auth_info\n"));
	rpc_server_register_auth_info ( (unsigned_char_t *) server_name,
		rpc_c_authn_dce_secret,
		(rpc_auth_key_retrieval_fn_t) NULL,
		(void *)KEY_TAB,
		&tstatus); 
	if (tstatus)
		err_prt("rpc_server_register_auth_info", tstatus);
   }

   rpc_binding_vector_free (&bindings, &tstatus);
   if (tstatus != rpc_s_ok) {
		err_prt("rpc_binding_vector_free", tstatus);
                exit (tstatus);
        }
   cert_expire -= time((time_t) NULL);
   /* set refresh interval to high of credential life */
	refresh_interval = cert_expire/2;
	printf("\tsecrgy_server: Credential life: %ld mins., refresh interval: %ld\n",
			cert_expire/60, refresh_interval/60);
	ref_thread_params.exp_int = refresh_interval;
	ref_thread_params.password = (char *) passwd;
	ref_thread_params.auth_id=&auth_id;
	
       if (pthread_create(&thread_id,
           pthread_attr_default,
          (pthread_startroutine_t)maintain_id,
          (pthread_addr_t)&ref_thread_params) != 0 ) {
          	perror(" \n    Error creating refresh thread:");
          	exit(1);
       }


   /*
    * Start listening for client requests.
    */

   dbprintf (("\tsecrgy_server: Calling rpc_server_listen\n"));

   TRY
      rpc_server_listen (MAX_CALL_REQ, &tstatus);

      CATCH_ALL
	printf("Server exiting\n");
	log_out(&auth_id);
	exit(1);

   ENDTRY

   /* Unregister endpoints before returning. */
   dbprintf (("\tsecrgy_server: Calling rpc_server_inq_bindings \n"));
   rpc_server_inq_bindings (&bindings, &tstatus);

   if (tstatus != rpc_s_ok) {
	err_prt("rpc_server_inq_bindings", tstatus);
        do_acct_cleanup(server_name, &auth_id, &tstatus);
        exit (tstatus);
   }

   dbprintf (("\tsecrgy_server: Calling rpc_ep_unregister\n"));

   rpc_ep_unregister (rgy_test_v1_0_s_ifspec, bindings,
        (uuid_vector_t *) NULL, &tstatus); 
   if(tstatus != error_status_ok){
	err_prt("rpc_ep_unregister", tstatus);
   }

   dbprintf (("\tsecrgy_server: Exiting with status %d\n", tstatus)); 

   exit (tstatus);
}

error_status_t 
rgy_test_login 
( bind_handle, name, pw, callid, duration, delay, valid_cert_f, message, status) 
   rpc_binding_handle_t		bind_handle;
   unsigned_char_p_t		name;
   unsigned_char_p_t		pw;
   signed32			callid;
   signed32			duration;
   signed32			delay;		
   ndr_long_int 		valid_cert_f;
   unsigned_char_p_t		message;
   error_status_t		*status;
{
   sec_passwd_rec_t		prec;
   boolean32			reset, val;
   sec_login_auth_src_t		auth_src;
   int				ret_stat;
   unsigned long		secs; 
   unsigned long		microsecs;
   unsigned long 		rt_secs;
   unsigned long		rt_duration;
   long				cert_expire;
   int				oper_count = 0;
   struct timeval		tm;


   *status = error_status_ok;
   start();
   getimer ( &secs, &microsecs);
   rt_secs=( microsecs / 1000000 + secs );
   rt_duration = duration * 60;
   rt_duration += rt_secs;
   sprintf(login_command, "login_test %s %s /.: 0 %d", 
	server_name, 
	passwd, 
	valid_cert_f);

	
   while ( rt_duration >= rt_secs ){
	if ( rt_duration - rt_secs > 0 && rt_duration != 0 )
		printf("Runtime left:%lu sec.\n", rt_duration - rt_secs);
	if (ret_stat = system( login_command )){
		err_prt ("login_test", ret_stat);
		return(*status = ret_stat);
	}
	else {
		oper_count++;
		getimer ( &secs, &microsecs);
		rt_secs=( microsecs / 1000000 + secs );
		if ( oper_count == 1 && rt_duration == 0 )
			break;
		if (delay){
			tm.tv_sec=delay;
			tm.tv_usec=0;
#ifdef SNI_SVR4
                        select(0, 0, 0, 0, &tm);
#else
			cma_select(0, 0, 0, 0, &tm);
#endif
		}

	}
   } /* end runtime duration loop */
   sprintf((char *) message, 
	"Client %s id: %d OK, operations: %d elapsed time: %lu.%03u sec.\n",
	name, 
	callid,
	oper_count, 
	secs, microsecs % 1000
	);
   return(*status = error_status_ok);
}


void override_snd (
    /* [in] */		handle_t 		h,
    /* [in] */		pipe_of_chars 		in_pipe,
    /* [in,out] */	idl_char		*message
)
{
    int file_handle;
    unsigned long data_count;

    file_handle = open((char *) pipe_in, O_CREAT | O_TRUNC | O_WRONLY, 0777 );
    if (file_handle == -1)
    {
        perror("Cannot create override file\n");
    }
    while (true)
    {
        (*in_pipe.pull)(in_pipe.state,manager_buffer,MANAGER_BUFFER_SIZE,
                        &data_count);
        if (data_count == 0)
        {
            close(file_handle);
            break;
        }
        write(file_handle,manager_buffer,data_count);
    }
    sprintf ( (char *) message, "Completed override file modification\n");
}

error_status_t
get_pwent( handle_t		h,
        unsigned_char_p_t       name,
        unsigned_char_p_t       passwd,
	idl_char		*message,
        error_status_t  	*status
)
{
	sec_login_handle_t	login_ctx; 
	struct passwd		*pwent;
	char			line_buff[81];
        error_status_t  	stat;

	sec_login_get_current_context(&login_ctx, &stat);
	if (stat) 
	    return(*status = stat);

	dbprintf(("\tsecrgy_server: Calling sec_login_get_pwent for %s principal\n", name)); 
	sec_login_get_pwent ( ( sec_login_handle_t ) login_ctx, 
		(sec_login_passwd_t) &pwent, &stat);  
	err_prt("sec_login_get_pwent", stat);
	if(stat)
		return(*status = stat);
	if (((struct passwd *)pwent) != NULL){
		sprintf ((char *) message, "Completed get_pwent %s:%s \n", 
			(*((struct passwd *)pwent)).pw_name,
			(*((struct passwd *)pwent)).pw_passwd);
	}
	line_buff[1]='\0';
	strcat(line_buff, (*((struct passwd *)pwent)).pw_name);
	strcat(line_buff, ":");
	strcat(line_buff, (*((struct passwd *)pwent)).pw_passwd);
	return(error_status_ok);
}

error_status_t
add_acct (handle_t 		h, 
	unsigned_char_p_t 	princ, 
	unsigned_char_p_t 	password,
	long			add_number,
	idl_char 		*message,
	error_status_t          *status)
{
	sec_rgy_handle_t	rgy_ctx;
	char *alpha_lst[] = 	{ "1", "2", "3", "4", "5", "6", "7", "8", 
					"9", "0" };
	char 			*name_p, *tmp_name_p ;
	int                     princ_slen ;
	int			i;
	int			index;
   	unsigned long		secs; 
   	unsigned long		microsecs;
	error_status_t          stat;

	start();
	if ( add_number > 1 ){
	  dbprintf(("\tsecrgy_server: Adding %d accounts, basename: %s\n", add_number, princ));
              princ_slen = strlen((char *) princ);
              name_p = (char *) malloc (princ_slen + 1) ;
              if (name_p == NULL)
	          return ( *status = rpc_s_no_memory);	        
              strcpy ( (char *) name_p, (char *) princ);

              for ( i = 0, index = 0; i < add_number; i++, index++) {
                  if (i % 10 == 0 ) {
       	              index = 0 ;
                      princ_slen = strlen (name_p);
                      tmp_name_p = (char *) malloc(princ_slen + 2) ;
                      if (tmp_name_p == NULL) 
                          return (*status = rpc_s_no_memory);
                      strcpy(tmp_name_p, name_p) ;
                      free(name_p);
                      name_p = tmp_name_p ;
	 	  }

                  strcpy ( &name_p[princ_slen], alpha_lst[index] );
	
		  add_account_user(name_p, password, passwd, &stat);
		  if (stat) {
		    err_prt("add_account_user", stat);
		    return (*status = stat);
		  }
             }

	}
	else {
	  dbprintf(("\tsecrgy_server: Adding account %s\n", princ));
	  add_account_user (princ, password, passwd, &stat);
	  if (stat) {
		err_prt("add_account_user", stat);
		return (*status = stat);
	  }
	}
	getimer ( &secs, &microsecs );	 
	if ( add_number <= 1)
    		sprintf ((char *) message, 
			"Added account %s, elapsed time: %lu.%03u sec.\n", 
			princ,
			secs, 
			microsecs % 1000 );
	else  
		sprintf ((char *) message, 
			"Added %d accounts, elapsed time: %lu.%03u sec.\n",
			add_number,
			secs,
			microsecs % 1000);
	return(*status = error_status_ok); 
}

error_status_t
del_acct (handle_t 		h,
	unsigned_char_p_t	princ,
	long			add_number,
	idl_char		*message,
        error_status_t          *status)
{
	sec_rgy_handle_t        rgy_ctx;
	char *alpha_lst[] = 	{ "1", "2", "3", "4", "5", "6", "7", "8", 
					"9", "0" };
	char 			*name_p, *tmp_name_p ;
	int                     princ_slen ;
	int			i;
	int			index;
   	unsigned long		secs; 
   	unsigned long		microsecs;
        error_status_t          stat;
	
        if (stat = do_site_open_update (&rgy_ctx)) {
                err_prt("do_site_open_update", stat);
                return (*status = stat);
        }
	start();
	if ( add_number > 1 ){
	     dbprintf(("\tsecrgy_server: Removing %d accounts, basename: %s\n", add_number, princ));
             princ_slen = strlen((char *) princ);
             name_p = (char *) malloc (princ_slen + 1) ;
             if (name_p == NULL)
	         return ( *status = rpc_s_no_memory);	        
             strcpy ( (char *) name_p, (char *) princ);

             for ( i = 0, index = 0; i < add_number; i++, index++) {
                 if (i % 10 == 0 ) {
     	             index = 0 ;
                     princ_slen = strlen (name_p);
                     tmp_name_p = (char *) malloc(princ_slen + 2) ;
                     if (tmp_name_p == NULL) 
                         return (*status = rpc_s_no_memory);
                     strcpy(tmp_name_p, name_p) ;
                     free(name_p);
                     name_p = tmp_name_p ;
		 }

                 strcpy ( &name_p[princ_slen], alpha_lst[index] );
                 if (stat = remove_acct ( &rgy_ctx, name_p)) {
                     err_prt("remove_acct", stat);
		     do_site_close(rgy_ctx);
                     return (*status = stat);
                 }
	    }
	}
	else {
	  dbprintf(("\tsecrgy_server: Doing remove_acct %s\n", princ));
	  if ( stat = remove_acct ( &rgy_ctx, princ) ){
		err_prt("remove_acct", stat);
		do_site_close(rgy_ctx);
		return (*status = stat);
	  }
	}
	getimer ( &secs, &microsecs);
	if (add_number > 1){
	  sprintf ((char *) message, 
			"Deleted %d accounts, elapsed time: %lu.%03u sec.\n",
			add_number,
			secs,
			microsecs % 1000);
	}
	else {
    		sprintf ((char *) message, 
			"Deleted account %s, elapsed time: %lu.%03u sec.\n", 
			princ,
			secs, 
			microsecs % 1000);
	}
	do_site_close(rgy_ctx);
	return (*status = error_status_ok); 

}

boolean32 authz_func (client_binding, requested_mgmt_operation, status)

   rpc_binding_handle_t         client_binding;
   unsigned32                   requested_mgmt_operation;
   unsigned32                   *status;
{

/* Return the authorization status */

   switch (requested_mgmt_operation)
     {

       case rpc_c_mgmt_inq_if_ids:
         return ((boolean32) FALSE);

       case rpc_c_mgmt_inq_stats:
         return ((boolean32) FALSE);
 
       case rpc_c_mgmt_is_server_listen:
         return ((boolean32) TRUE);
 
       case rpc_c_mgmt_stop_server_listen:
         return ((boolean32) TRUE);

       case rpc_c_mgmt_inq_princ_name:
         return ((boolean32) TRUE);

       default:
         return ((boolean32) FALSE);
     }

} /* End authz_func */

void 
maintain_id(ref_thread_args_t *parms)
{
   unsigned32           status =0 ;     
   signed32             id_exp_time; /* when id expires from current time */
   signed32             lifetime;    /* when id expires, absolute */
   struct timeval 	now;	
   struct timespec      wakeup;      /* time for wakeup of refresh thread */
   char                 *p;          /* temporary password holder */	 
   char                 *pw;         /* real password */	 
   boolean32            reset_pw;    /* password reset */
   sec_login_auth_src_t auth_src;    /* Returns auth source */
   int pid=getpid();
   int i;
   sec_login_handle_t   login_context; 
   sec_passwd_rec_t     prec;
   char                 *passwd_temp;
   long			cert_expire;
   int			refresh_fails = 0;

   lifetime = parms->exp_int;
   pw = parms->password;
   login_context = *(parms->auth_id);
	
   /*---* get the current time... *---*/
   GETTIMEOFDAY(&now);
   id_exp_time = now.tv_sec + lifetime; 
   wakeup.tv_sec = id_exp_time;
   wakeup.tv_nsec = 0;

   pthread_mutex_lock(&refr_id_mutex);
	
   while( status == 0 ) {
      while ( now.tv_sec < id_exp_time ) {
         pthread_cond_timedwait(&refr_id_cond, &refr_id_mutex, &wakeup);
         GETTIMEOFDAY(&now);
      }
      
      id_exp_time = now.tv_sec;

      sec_login_refresh_identity(login_context, &status);
      if (status){ 
	 err_prt("refreshing identity", status); 
	 refresh_fails++;
	 if ( refresh_fails <= 2 )
		status = 0;
	 else  
	        pthread_exit ((pthread_addr_t) NULL);
      }	
      else {	
         /* sec_login_valididate_id will nullify the password */
         p = (char *) malloc (strlen ((char *) pw) +1);
         if ( p == (char *) NULL)
                exit (status = rpc_s_no_memory);
         strcpy ((char *)p,(char *)pw);

         /* Set the password information for validate_identity */
         prec.key.key_type = sec_passwd_plain;
         prec.key.tagged_union.plain = (idl_char *) p;
         prec.pepper = NULL;
         prec.version_number = sec_passwd_c_version_none;
   
         sec_login_validate_identity(login_context, &prec, &reset_pw,
                                                        &auth_src, &status);
         if (status) 
	    err_prt("refreshing identity", status); 
         sec_login_get_expiration (login_context,
            &cert_expire,
            &status);
         if (status && status != sec_login_s_not_certified)
            err_prt ("sec_login_get_expiration", status);
	 else {
		status = 0;
	 	printf("\nNew credential expiration: %s", ctime((time_t *)&cert_expire));
	 }

	 GETTIMEOFDAY(&now);
         /*---* update id refresh tracking variables *---*/
         id_exp_time = now.tv_sec + lifetime;
         wakeup.tv_sec = id_exp_time;
         wakeup.tv_nsec = 0;
      }
   }
}


