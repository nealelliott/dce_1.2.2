/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: binlogin.c,v $
 * Revision 1.1.7.2  1996/02/17  23:25:17  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:27  marty]
 *
 * Revision 1.1.7.1  1995/12/11  21:48:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:09  root]
 * 
 * Revision 1.1.5.3  1992/12/31  17:28:32  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:54:01  htf]
 * 
 * Revision 1.1.5.2  1992/12/10  14:25:32  cuti
 * 	Submit sekhar's change.
 * 	[1992/12/09  21:06:58  cuti]
 * 
 * Revision 1.1.3.2  1992/12/08  15:56:44  sekhar
 * 	[OT 6364]  include pwd.h rather than sec_pwd.h so that the definition for
 * 	           passwd structure is picked up from from the header file pwd.h
 * 	           rather than from sec_pwd.h. Also did some minor cleanup.
 * 
 * Revision 1.1.2.6  1992/05/22  20:17:21  cuti
 * 	Explicitly exit(0) in the end.
 * 	[1992/05/22  20:09:01  cuti]
 * 
 * Revision 1.1.2.5  1992/05/22  14:06:55  cuti
 * 	Add print_unix command option to print out unix info for override..
 * 	[1992/05/22  02:41:49  cuti]
 * 
 * Revision 1.1.2.4  1992/05/21  21:41:19  cuti
 * 	 Add boolean to store validate_and_cert_ident result and get uix info.
 * 	[1992/05/21  21:12:48  cuti]
 * 
 * Revision 1.1.2.3  1992/05/19  20:46:18  cuti
 * 	Use dce_error_inq_text to print out error msg.
 * 	[1992/05/19  16:59:37  cuti]
 * 
 * Revision 1.1.2.2  1992/05/14  21:28:07  cuti
 * 	Initial file creation for local registry and passwd_import testing.
 * 	[1992/05/12  18:05:50  cuti]
 * 	Revision 1.2  1992/01/19  22:14:34  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */


/*
 * This program will do local rgy login, calling sec_login_setup_identity and
 * sec_login_valid_and_cert_ident.  But it didn't exe a shell, so continuous
 * login can be done through a shell script.
 */

#include <stdio.h>
#include <dce/sec_login.h>     
#include <dce/dce_error.h>
#include <pwd.h>
#include <string.h>
 
/* Args required by the test program. */

#define ARGS_REQUIRED 3

main (argc, argv)

/* Formal parameters */
    int     argc;
    char    *argv[];
{

/* Local variables */

   error_status_t	estatus;	/* argv[1], expected return from test */
   char             *client_name = NULL; 
   char 			*passwd = NULL;
   sec_login_handle_t	auth_identity;	/* sec_login_setup_identity */
   boolean32		reset_passwd;	/* sec_login_validate_identity */
   sec_login_auth_src_t	auth_src;	/* sec_login_validate_identity */
   boolean32		passwd_valid;
   boolean32		print_unix;
   error_status_t	tstatus;	/* test status */
   sec_passwd_rec_t      prec;     /* sec_login_validate_identity */
    dce_error_string_t  err_string;
    int                 lst;
    struct  passwd       *pwd;




/* Check the number of arguments passed in.  They should be exactly the
   required number when you come into this routine. */

/*   if (argc != ARGS_REQUIRED)
     {
       printf("Expecting %d arguments, got %d\n",
                  ARGS_REQUIRED, argc);
       exit(1);
     } 
*/

/* Obtain the input arguments from the .tsh command line. */

   estatus = 0; 
   print_unix = 0;
   client_name = argv[1];   
   passwd = argv[2];
   if (argc == 4)
	print_unix = 1;



/* Log this client in. */

   sec_login_setup_identity ((unsigned char *)client_name,
                             sec_login_no_flags,
                             &auth_identity,
                             &tstatus);
    if (tstatus) {
        dce_error_inq_text(tstatus, err_string, &lst);
        printf("sec_login_setup_identity failed: %s\n", err_string);
		exit (1);
    }

    prec.key.key_type = sec_passwd_plain;  
    prec.key.tagged_union.plain = (unsigned char *)passwd;
    prec.pepper = NULL;
    prec.version_number = sec_passwd_c_version_none;

   passwd_valid= sec_login_valid_and_cert_ident (auth_identity,
                                       &prec,
                                       &reset_passwd,
                                       &auth_src,
                                       &tstatus);    

    if (tstatus) {
        dce_error_inq_text(tstatus, err_string, &lst);
        printf("sec_login_valid_and_cert_ident failed: %s\n", err_string);
		exit (1);
    }      
	
	if (passwd_valid != 1) {
		printf("sec_login_valid_and_cert_ident failed \n");
		exit(1);
	}       
	
	if (print_unix) {
    	printf("\nauth_src : %d\n", auth_src);  
        if (auth_src != sec_login_auth_src_network) {
    		printf("credentials obtained from LOCAL authority\n");
        }
           
        sec_login_get_pwent(auth_identity, (sec_login_passwd_t) &pwd, &tstatus);
        if (tstatus != 0) {
    /*        set_local_id = false;   */
            if (tstatus ==  sec_login_s_not_certified) {
                printf("Unix info is NOT certified\n", tstatus);
            }
        }
        if (pwd == NULL) {
            printf("Unix info is not available\n");
        } else {
            printf("unix info:\n");
            printf("    pw_name  : %s\n", pwd->pw_name);
            printf("    pw_passwd: %s\n", pwd->pw_passwd);
            printf("    pw_uid   : %d\n", pwd->pw_uid);
            printf("    pw_gid   : %d\n", pwd->pw_gid);
            printf("    pw_gecos : %s\n", pwd->pw_gecos);
            printf("    pw_dir   : %s\n", pwd->pw_dir);
            printf("    pw_shell : %s\n", pwd->pw_shell);
        } 
	}
      
	exit(0);
}
 
