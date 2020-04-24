/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: secrgy_client.c,v $
 * Revision 1.1.71.2  1996/02/17  23:28:56  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:45  marty]
 *
 * Revision 1.1.71.1  1995/12/11  22:46:56  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/23  15:07 UTC  bissen
 * 	Merge from Hpdce02_01
 * 	[1995/12/11  22:22:15  root]
 * 
 * Revision 1.1.68.2  1994/07/19  19:09:39  dassarma
 * 	Fixed to CR# 11273
 * 	[1994/07/19  19:09:16  dassarma]
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/05  15:56 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.68.1  1994/02/23  21:44:02  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:40:52  pellis]
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/31  15:41  bissen
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.5.2  1993/07/09  14:32:37  root
 * 	Initial King Kong branch
 * 	[1993/07/09  14:32:08  root]
 * 
 * 	if statements must be scalar
 * 	[1993/10/06  15:01:18  ohara]
 * 
 * Revision 1.1.66.2  1993/10/14  17:37:17  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:33  cmckeen]
 * 
 * Revision 1.1.66.1  1993/09/09  19:56:20  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/09  19:51:08  cmckeen]
 * 
 * Revision 1.1.5.2  1993/07/02  16:29:13  eheller
 * 	GAMMERA version which runs under TET.
 * 	[1993/07/02  16:13:47  eheller]
 * 
 * Revision 1.1.3.2  1993/05/04  20:36:16  sekhar
 * 	1. changed rpc_ns_binding_import_begin() and rpc_ns_binding_import_done()
 * 	   to be called outside the for loop only once.
 * 
 * 	2. removed arguments (and the code used to initialize them) to
 * 	   add_acct() . The args were acct_key, user_part, admin_part.
 * 
 * 	3. changed the synchronization between client threads and
 * 	   main() . The last client thread to exit now does a
 * 	   pthread_cond_signal() and main() waits on this condition.
 * 
 * 	4. fill and null terminate the array mesg[][].
 * 
 * 	5. null terminate the passwd when copied from cmd line.
 * 	[1993/05/03  20:48:28  sekhar]
 * 
 * 	Updated usage statement and corrected checks on server_name option.
 * 	[1993/03/31  18:19:20  eperkins]
 * 
 * 	Removed unused operations and added login context to the log_in-out calls.
 * 	[1993/03/25  19:02:14  eperkins]
 * 
 * 	Cleaned alot of superfluous code, added the ping and multiple principal +
 * 	account operations
 * 	[1993/03/11  00:15:35  eperkins]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/05  14:46:59  cjd]
 * 
 * 	Initial version 1.0.2 system test sec.rgy.7
 * 	[1993/01/04  18:43:59  eperkins]
 * 
 * Revision 1.1.1.4  1993/04/21  20:28:25  root
 * 	    Loading drop DCE1_0_2B23
 * 
 * Revision 1.1.3.2  1993/02/04  22:18:03  cjd
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

#include <un_maloc.h>
#include <stdio.h>
#include <time.h>

#include <dce/dce.h>
#include <dce/rgybase.h>

#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/passwd.h>
#include <string.h>

#include <krb5/krb5.h>
#include <krb5/los-proto.h>     /* for krb5_read_password */
#include <dce/rpc.h>

#include "rgy_test.h"
#include "rgy_clt.h"



#ifdef ST_DEBUG
#   define dbprintf(A) printf A
#else
#   define dbprintf(A)
#endif

static sec_login_handle_t	log_id_ctx;

#define MAX_SERVERS 20
#define CLIENT_BUFFER_SIZE 2048
#define MAX_BUF_SIZE 1025

int clients;
pthread_mutex_t client_mutex;
pthread_cond_t	client_cond;

char *protseqs[] = {
	"ncadg_ip_udp",
	"ncacn_ip_tcp"
	};

static char server_buf[MAX_BUF_SIZE] = { "/.:" };
static char client_buf[MAX_BUF_SIZE] = { "/.:" };

extern error_status_t do_site_open_update ();
extern error_status_t do_site_open_query ();
extern error_status_t add_account ();
extern error_status_t do_account_prep();
extern error_status_t remove_acct();
extern error_status_t log_in ();
extern error_status_t log_out ();
extern error_status_t rgy_test_login ();
extern error_status_t bye_server ();
extern void err_prt();
void client_pull();
void client_alloc();
int client_aux();
void do_test();
void do_del_acct();

typedef struct client_pipe_state_t {
	ndr_boolean file_open;
	int file_handle;
} client_pipe_state_t;

client_pipe_state_t client_in_pipe_state = { false, 0 };
client_pipe_state_t client_out_pipe_state = { false, 0 };

char *filename = {""};
char *s;

typedef unsigned char secrgy_byte;


int main (argc, argv)
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
   error_status_t    	tstatus;           /* actual value returned from rpc */
   rpc_binding_vector_t *bindings;              /* rpc_server_inq_bindings */
   unsigned_char_p_t 	str_binding_p;     /* rpc_binding_to_string_binding */
   unsigned_char_p_t    client_name = (unsigned_char_p_t) NULL;
   unsigned_char_p_t    server_name = (unsigned_char_p_t)NULL;
   unsigned_char_p_t    userkey = NULL;
   unsigned32           authn_svc = rpc_c_authn_dce_private;
   sec_rgy_acct_key_t   acct_key;
   sec_rgy_acct_user_t  user_part;
   sec_rgy_acct_admin_t admin_part;
   sec_passwd_str_t     passwd;
   unsigned32		duration = 0;
   unsigned32		add_number = 0;
   unsigned32		delay;
   unsigned_char_p_t	protseq = ( unsigned_char_p_t ) NULL;
   unsigned_char_p_t	import_protseq;
   unsigned32		expire_tm, cur_tm, exc_exp;
   int                  c;
   extern char          *optarg;
   extern int           optind;
   int			test_number;
   int	                argcount = 0;
   int			auth = FALSE;
   int			server_exit_f = FALSE;
   int			get_pwent_f = FALSE;
   int			del_princ_f = FALSE;
   int			ping_f = FALSE;
   ndr_long_int 	valid_cert = FALSE;
   int                  i , j ;
   int                  client_number = 1;
   int			ret;
   rpc_ns_handle_t	import_context;
   rpc_binding_handle_t binding_handle;
   unsigned_char_p_t	string_binding;
   unsigned_char_p_t 	cellname;
   unsigned_char_p_t 	ret_line;
   pthread_t thd_id[MAX_SERVERS];
   call_args_t	call_args[MAX_SERVERS];
   idl_char mesg[MAX_SERVERS][80];
	
   /* Setup some pthread stuff. */

   pthread_setasynccancel (CANCEL_OFF);
   cellname = (unsigned_char_t *) &server_buf[0];

   while((c = (int) getopt(argc, (char **)argv, "dPwoxn:s:p:t:i:c:af:r:u:k:l:j:")) != EOF) {
        switch (c) {
           case 'd' : /* debug */
                rpc__dbg_set_switches("0-4.10", &tstatus);
                break;
           case 'P' : /* ping server */
		ping_f = TRUE;
                break;
           case 'w' : /* get passwd entry*/
		get_pwent_f = TRUE;
                break;
           case 'o' : /* use valid_cert started by root */
                valid_cert = TRUE;
                break;
           case 'x' : /* call server cleanup */
                server_exit_f = TRUE;
                break;
           case 'n' : /* client name */
                client_name =  (unsigned_char_p_t) optarg;
                argcount += 2;
                break;
           case 's' : /* server name */
                server_name = (unsigned_char_p_t) optarg;
                argcount += 2;
                break;
           case 'p' :  /* client password */
                strncpy((char *)passwd, optarg, sec_passwd_str_max_len);
		passwd[sec_passwd_str_max_len] = '\0';
                argcount += 2;
                break;
           case 't' :  
                duration =  atol(optarg);
                argcount += 2;
                break;
	   case 'i':
		protseq = (unsigned_char_p_t) optarg;
                argcount += 2;
                break;
           case 'c' :  
                cellname =  (unsigned_char_p_t) optarg;
                argcount += 2;
                break;
           case 'f' :  
                filename = (char *) optarg;
                argcount += 2;
                break;
           case 'a' : /* authenticated */
		auth = TRUE;
                break;
           case 'r' :  
                 client_number = atol(optarg);
                argcount += 2;
                break;
           case 'u' :  
                userkey = (unsigned_char_p_t) optarg;
                argcount += 2;
                break;
           case 'k' :  
		del_princ_f = TRUE;
                userkey = (unsigned_char_p_t) optarg;
                argcount += 2;
                break;
           case 'l' :  
                delay = atol(optarg);
                argcount += 2;
                break;
           case 'j' :  
                add_number = atol(optarg);
                argcount += 2;
                break;
        }
   }
	
   if (argcount == 0 || server_name == (unsigned char *) NULL) {
        printf("Usage: secrgy_client -n client_name -p client_password -s server_name [ -d ]\n");
	printf("\t[-w get_pwent operation ] [-r call threads ] \n");
        printf("\t[-o sec_login_validate_and_certify for login-out operation ]\n"); 
        printf("\t[-x server exit operation ] [ -c cell name ] \n");
        printf("\t[-u account/principal name to add ] [ -k account/principal name to delete ]\n");
	printf("\t[-a authenticated RPC ] [ -f filename ] [ -i protocol sequence ]\n");
	printf("\t[-t duration of login-logout operation in minutes ]\n");
	printf("\t[-l delay between login-logout operations ]\n");
	printf("\t[-j number of aggregate accounts to add/delete from regsistry]\n");
	printf("\t[-P ping the server] \n");

        exit(1);
   }
   
   setbuf ((FILE*)stdout, (char *)NULL);
   setbuf ((FILE*)stderr, (char *)NULL); 
   
   if ( add_number > 1023 ){
	fprintf (stderr, "The maximum resgisty PGO name size would be exceeded if this number is used\n");
	exit(1);
   }
   else if ( add_number == 0 )
	add_number = 1;


   if (protseq == (unsigned_char_p_t) NULL) {
	protseq = (unsigned_char_p_t) protseqs[0]; 
	printf("Client calls default to: %s transport protocol\n",protseq);
   }

   if (duration == 0)
	exc_exp = cur_tm; 

   if (test_number == 0 )
	test_number = 1;
   if ( (strchr ((char *)server_name, '/')) == (char  *) NULL) {
   	if ( (char *) cellname != (char *) server_buf )
		strcpy((char *) server_buf, (char *) cellname);
  	strcat((char *) server_buf, "/subsys/");
   	strcat ((char *) server_buf, (char *) server_name);
   }

   /*
    * Add server to security data base.
    */

   /*
    * Log_in as client_name pricipal
    * assumes principal has been added to the registry
   */
   if (tstatus = log_in ( &log_id_ctx, client_name, passwd)) {
       fprintf(stderr,"Failed to login as %s\n", client_name, tstatus);
       if (tstatus)
	  err_prt("log_in", tstatus);
       exit (tstatus);
   }

   dbprintf (("\tsecrgy_client: Logged in as %s.\n", client_name));

   dbprintf(("\tsecrgy_client: Begining NSI import of:%s\n", server_buf));

   /* Import the server partial binding */

   rpc_ns_binding_import_begin(rpc_c_ns_syntax_default,
	(unsigned_char_t *) server_buf,
	rgy_test_v1_0_c_ifspec,
	(uuid_t *) NULL,
	&import_context,
	&tstatus);	
   if(tstatus)
	err_prt("rpc_ns_binding_import_begin", tstatus);
	
   while (1){
	rpc_ns_binding_import_next(import_context,
		&binding_handle,
		&tstatus);
	if(tstatus){
		err_prt("rpc_ns_binding_import_next", tstatus);
		return(tstatus);
	}
	rpc_binding_to_string_binding( binding_handle, 
		&string_binding, 
		&tstatus);
	if(tstatus)
		err_prt("rpc_binding_to_string_binding", tstatus);

	rpc_string_binding_parse(string_binding, NULL, &import_protseq, 
		NULL, NULL, NULL, &tstatus);
	if(tstatus)
		err_prt("rpc_string_binding_parse", tstatus);
	ret = strcmp((char *) import_protseq, (char *) protseq);

	rpc_string_free(&import_protseq, &tstatus);
	if(tstatus)
		err_prt("rpc_string_free", tstatus);
	if(ret == 0)
		break;


   } /* end import bindings loop */

   rpc_ns_binding_import_done ( &import_context, &tstatus); 
   if(tstatus)
	err_prt("rpc_binding_import_done", tstatus);

   for (i = 0; i < client_number; ++i) {
   	call_args[i].server_num = i;
	call_args[i].client = client_name;
	call_args[i].password = passwd;
	call_args[i].binding = binding_handle;
	call_args[i].duration = duration; 
	call_args[i].delay = delay; /* operation delay in seconds */ 
	call_args[i].val_cert_flag = valid_cert;
	call_args[i].msg = (idl_char *) mesg[i];
	for (j = 0 ; j < MAX_MSG_SIZE ; j++)
	    mesg[i][j] = '\0' ;
	call_args[i].status = 0; 
   }
   if (ping_f == TRUE){
        rpc_ep_resolve_binding ( binding_handle,
				 rgy_test_v1_0_c_ifspec,
				 &tstatus);
	if(tstatus)
		err_prt("rpc_ep_resolve_binding", tstatus);

	rpc_mgmt_is_server_listening( binding_handle,
				      &tstatus); 
	if(tstatus)
		err_prt("rpc_mgmt_is_server_listening", tstatus);
        printf("Server ping was successful\n");
   }
		

   dbprintf(("\tsecrgy_client: Binding imported:\n%s\n",string_binding));

   rpc_string_free(&string_binding, &tstatus);

   if ( auth == TRUE ) {
	  dbprintf(("\tsecrgy_client: Calling rpc_binding_set_auth_info\n"));
 	  rpc_binding_set_auth_info( binding_handle, server_name, 
		rpc_c_protect_level_pkt,
		rpc_c_authn_dce_secret, log_id_ctx,
		rpc_c_authz_name,
		&tstatus); 
	   if(tstatus)
		err_prt("rpc_binding_set_auth_info", tstatus);
   }
   ret = pthread_mutex_init (&client_mutex, pthread_mutexattr_default);
   if (ret == -1)
	fprintf(stderr, "Error return from pthread_mutex_init\n");
   ret = pthread_cond_init (&client_cond, pthread_condattr_default);
   if (ret == -1)
	fprintf(stderr, "Error return from pthread_cond_init\n");
   clients = client_number;

   dbprintf(("\tsecrgy_client: Calling server %s\n", server_buf));

   if (userkey != NULL && del_princ_f == FALSE){
	add_acct ( binding_handle,
		userkey,
		passwd,
		add_number,
		(idl_char *) mesg[0],
		&tstatus);
	if (tstatus)
		err_prt("add_acct", tstatus);
	if ((int)mesg[1])
		printf("%s\n",mesg[0]);
   }
   if (userkey != NULL && del_princ_f == TRUE){
	do_del_acct (binding_handle, userkey, add_number);
   }
			
   if (*filename !=  NULL){
	client_aux(binding_handle);
   }

   if (get_pwent_f == TRUE){
	dbprintf(( "\tsecrgy_client: Calling get_pwent for %s %s\n",
		client_name, passwd));
   	if ( (char *) cellname != (char *) server_buf )
   		strcpy((char *) server_buf, (char *) cellname);
        strcat((char *) server_buf, "/");
        strcat ((char *) server_buf, (char *) client_name);
	get_pwent(binding_handle, 
		client_name, 
		passwd, 
		(idl_char *) mesg[0], 
		&tstatus);
	if (tstatus)
		err_prt("get_pwent", tstatus);
	if ((int)mesg[0])
		printf("%s\n",mesg[0]);
   } 

   if ( server_exit_f != TRUE && userkey == NULL ){	
   	for (i = 0; i < client_number; ++i) {
		ret = pthread_create ( &thd_id[i], 
			pthread_attr_default, 
			(pthread_startroutine_t) do_test,
			(pthread_addr_t) &call_args[i]); 
   	}

	/*
	 *   Wait until all threads created above have
	 *   exited. The last thread to exit will do a
	 *   pthread_cond_signal() on client_cond variable
	 */

	ret = pthread_mutex_lock (&client_mutex);
        if (clients > 0) 
	    ret = pthread_cond_wait(&client_cond, &client_mutex);
	ret = pthread_mutex_unlock (&client_mutex);

	for (i = 0; i < client_number; i++) {
	    if (call_args[i].status != 0){
		fprintf(stderr,"Failure client number %d\n",i);
		err_prt("client ", call_args[i].status);
		tstatus = log_out (&log_id_ctx);
		exit (call_args[i].status);
	    }	
	    else if (*(call_args[i].msg) !=  NULL) {
		printf("%s\n",call_args[i].msg);
		*call_args[i].msg =  NULL;
	    } 
	}
   } 
   else if ( server_exit_f == TRUE ) {
	rpc_ep_resolve_binding ( binding_handle,
				 rgy_test_v1_0_c_ifspec,
				 &tstatus);		 
        if (tstatus)
                err_prt ("rpc_mgmt_stop_server_listening", tstatus);
        rpc_mgmt_stop_server_listening ( binding_handle, &tstatus);
        if (tstatus){
                err_prt ("rpc_mgmt_stop_server_listening", tstatus);
        }
   }
	
   tstatus = log_out (&log_id_ctx);

   if (tstatus)	
	exit(tstatus);
   else
	exit (0);
}
void client_pull (rpc_ss_pipe_state_t state_arg,
		secrgy_byte *buf,
		unsigned long esize,
		unsigned long *ecount)
{
	client_pipe_state_t *state = state_arg;

	if ( !state->file_open)
	{
		state->file_handle = open (filename, O_RDONLY);
		if (state->file_handle == -1)
		{
			fprintf(stderr,"Client couldn't open %s\n", filename);
			exit(0);
		}
		state->file_open = true;
	}
	*ecount = read (state->file_handle, buf, esize);
	if (*ecount == 0)
	{
		close(state->file_handle);
		state->file_open = false;
	}
}
 
                      
static secrgy_byte client_buffer[CLIENT_BUFFER_SIZE];

void client_alloc (rpc_ss_pipe_state_t state,
		   unsigned long bsize,
		   secrgy_byte **buf,
		   unsigned long *bcount)
{
    *buf = client_buffer;
    *bcount = CLIENT_BUFFER_SIZE;
}

int client_aux(handle_t h)
{
    pipe_of_chars client_in_pipe;
    idl_char mesg[80];

    client_in_pipe.pull = client_pull ;
    client_in_pipe.alloc = client_alloc;
    client_in_pipe.state = &client_in_pipe_state;

   override_snd ( h,  client_in_pipe, mesg);
   if (mesg !=  NULL)
	printf("%s\n", mesg);
                     
}


void do_test ( call_args_t *args_p)
{
	int 			server_num;
	unsigned_char_t		*client_name;
	unsigned_char_t		*pw;
	rpc_binding_handle_t	binding;
	int			duration;
	int 			delay;
	int 			call_id;
	short			flag;
	error_status_t		status;
	int			ret;			
	idl_char		*msg;

	server_num = args_p->server_num;
	client_name = args_p->client;
 	pw = args_p->password;	
	binding = args_p->binding;
	duration = args_p->duration; 
	flag = args_p->val_cert_flag;
	msg = args_p->msg;
	delay = args_p->delay; /* this becomes the delay between operations */
	
	dbprintf(("\tsecrgy_client: Call thread client %d starting\n", 
		server_num ));
	rgy_test_login(binding, 
			client_name, 
			pw, 
			server_num,
			duration, 
			delay, 
			flag, 
			msg, 
			&status);    
	if (status){
		err_prt("rgy_test_login", status);
		exit(status);
	}
	ret = pthread_mutex_lock (&client_mutex);
	if (ret)
		fprintf(stderr, "pthread_mutex_lock failure\n");
	clients -= 1;
	if (clients == 0) 
	    ret = pthread_cond_signal(&client_cond);
	if (ret)
		fprintf(stderr, "pthread_cond_signal failure\n");
	ret = pthread_mutex_unlock (&client_mutex);
	if (ret)
		fprintf(stderr, "pthread_mutex_unlock failure\n");
	pthread_exit(NULL);
}

void do_del_acct (rpc_binding_handle_t handle, 
	unsigned_char_p_t    user, 
	unsigned32 number ) 
{
   	error_status_t    status;           /* actual value returned from rpc */
   	error_status_t    ret;           
	char mesg[80]= {'\0'};

	dbprintf(("\tsecrgy_client: Calling del_acct %s\n",user));
	del_acct ( handle,
		user,
		number,
		(idl_char *) mesg,
		&status);
	if (status)
		err_prt("del_acct status:", status);
	if (mesg[1] != NULL)
		printf("%s\n",mesg);
}









