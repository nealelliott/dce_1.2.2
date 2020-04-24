/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: secrgy_setup.c,v $
 * Revision 1.1.63.2  1996/02/17  23:29:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:48  marty]
 *
 * Revision 1.1.63.1  1995/12/11  22:47:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:22:18  root]
 * 
 * Revision 1.1.60.1  1994/06/10  20:50:56  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:20:01  devsrc]
 * 
 * Revision 1.1.58.1  1993/09/09  19:56:23  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/09  19:51:11  cmckeen]
 * 
 * Revision 1.1.4.2  1993/07/02  16:44:40  eheller
 * 	GAMMERA version which runs under TET.
 * 	[1993/07/02  16:20:52  eheller]
 * 
 * 	1. Use constants instead of hardcoding cell_admin and -dce- .
 * 	2. Null terminate the passwd when copied from cmd line
 * 	[1993/05/04  19:22:25  sekhar]
 * 
 * 	Removed alot of obsolete code and added the login context to the log_in and
 * 	log_out calls.
 * 	[1993/03/25  18:59:17  eperkins]
 * 
 * 	Initial version
 * 	[1993/03/11  00:23:17  eperkins]
 * 
 * Revision 1.1.1.5  1993/04/21  14:14:45  root
 * 	    Loading drop DCE1_0_2B23
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
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/acct.h>
#include <dce/dce_cf_const.h>
#include <dce/passwd.h>

#include "rgy_test.h"
#include "rgy_clt.h"


static unsigned char pipe_in[] = {"/opt/dcelocal/etc/passwd_override"};
static unsigned char *s;
static unsigned char        server_name[MAX_NS_NAME];
static sec_passwd_str_t     passwd;
static unsigned char 	    cell_rel_name[MAX_NS_NAME]={"/.:"};	
static rpc_binding_vector_t *bindings;

#ifdef ST_DEBUG
#define dbprintf(A) printf A
#else
#define dbprintf(A)
#endif

#define SERVER_UNIX_NUM     999
#define CLIENT_UNIX_NUM         998
#define KEY_TAB "/usr/tmp/v5srvtab"
#define SERVER_MAX_ARGS 25


extern error_status_t do_site_open_update ();
extern error_status_t do_site_open_query ();
extern error_status_t add_account_adminserv ();
extern error_status_t do_account_prep();
extern error_status_t remove_acct ();
extern error_status_t log_in ();
extern error_status_t log_out ();
extern void err_prt();

main (argc, argv)
ndr_long_int      argc;
unsigned_char_p_t argv[];
{
   error_status_t    	tstatus;           /* actual value returned from rpc */
   int                  c;
   extern char          *optarg;
   extern int           optind;
   short                argcount = 0;
   char			*cellname = (char *) NULL;
   sec_login_handle_t   auth_id;       /* server login context/credentials */
   sec_passwd_rec_t     prec;
	
   setbuf ((FILE*)stdout, (char *)NULL);
   setbuf ((FILE*)stderr, (char *)NULL); 

   /* Setup some pthread init. */
   pthread_setasynccancel (CANCEL_OFF);

   while((c = (int) getopt(argc, (char **)argv, "dc:n:p:")) != EOF) {
      switch (c) {
           case 'd' : /* debug */
                rpc__dbg_set_switches("0.1", &tstatus);
                break;
           case 'n' : /* server name */
                strncpy((char *) server_name, (char *) optarg, MAX_NS_NAME);
		/*cmd_to_exec[2] = (char *) server_name; */
                argcount += 2;
                break;
           case 'c' : /* cell name */
                cellname = optarg;
                argcount += 2;
                break;
           case 'p':  /* server password */
                strncpy((char *) passwd, (char *) optarg, sec_passwd_str_max_len);
		passwd[sec_passwd_str_max_len] = '\0' ;
                argcount += 2;
                break;
        }
   }
   if (argcount == 0) {
        printf("Usage: secrgy_setup -n server_name -p server_password [-d]\n");
        exit(1);
   }
	
   
	
   /* CHECK (sekhar) 
    *     The following won't work if -c is specified . The update site takes
    *     into account the cell name but the principal name i.e. cell_admin 
    *     does not. So do_account_prep can fail.
    */

   if ( (char *) cellname != (char *) NULL )
	/* strncpy ((char *) cell_rel_name,(char *) cellname, MAX_NAME_SIZE); */
        strncpy ((char *) cell_rel_name,(char *) cellname, MAX_NS_NAME);
   strcat((char *) cell_rel_name, "/subsys/");
   strcat ((char *) cell_rel_name, (char *) server_name);

   if ( tstatus = log_in ( &auth_id, CELLADMIN, CELLADMIN_PW) ){
        err_prt("log_in", tstatus);
        exit (tstatus);
   }

   /*
    * Add server to security data base.
    */

   tstatus=do_account_prep(server_name, passwd);
   if (tstatus){
	err_prt("do_account_prep", tstatus);
                exit (tstatus);
   }
   
   tstatus = log_out(&auth_id); 
   exit (tstatus);
}







