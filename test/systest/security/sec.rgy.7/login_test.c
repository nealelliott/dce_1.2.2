/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: login_test.c,v $
 * Revision 1.1.65.2  1996/02/17  23:28:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:44  marty]
 *
 * Revision 1.1.65.1  1995/12/11  22:46:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:22:11  root]
 * 
 * Revision 1.1.63.2  1994/06/10  20:50:54  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:54  devsrc]
 * 
 * Revision 1.1.63.1  1994/02/23  21:44:01  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:40:40  pellis]
 * 
 * Revision 1.1.61.2  1993/10/14  17:37:15  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:30  cmckeen]
 * 
 * Revision 1.1.61.1  1993/09/09  19:56:15  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/09  19:51:03  cmckeen]
 * 
 * Revision 1.1.4.2  1993/07/02  16:34:08  eheller
 * 	GAMMERA version which runs under TET.
 * 	[1993/07/02  16:15:31  eheller]
 * 
 * 	include <systest.h> to pick up the definition of TRUE and
 * 	FALSE.
 * 	[1993/05/03  20:49:56  sekhar]
 * 
 * 	Added else to log_out sequence to avoid inadvertent deletion of the machines
 * 	creds, also added the log_in context to log_in,out and more code cleanup.
 * 	[1993/03/25  19:12:43  eperkins]
 * 
 * 	Initial version
 * 	[1993/03/11  00:24:26  eperkins]
 * 
 * Revision 1.1.1.5  1993/04/21  14:14:43  root
 * 	    Loading drop DCE1_0_2B23
 * 
 * $EndLog$
 */
#ifdef SNI_SVR4
#include <pthread.h>
#include <sys/signal.h>
#include <fcntl.h>
#else
#include <sys/signal.h>
#include <pthread.h>
#include <sys/file.h>
#endif /* SNI_SVR4 */
#include <stdlib.h>

#include <stdio.h>
#include <sys/time.h>

#include <dce/dce.h>

#include <dce/rgybase.h>

#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/passwd.h>
#include <dce/rgybase.h>
#include <string.h>

#include <dce/rpc.h>
#include <dce/acct.h>

#include <systest.h>

#ifdef ST_DEBUG
#define dbprintf(A) printf A
#else
#define dbprintf(A)
#endif

error_status_t main (int argc, char **argv )
{
   char *			name = argv[1];
   char *			pw = argv[2];
   ndr_long_int 		valid_cert_f = atol(argv[5]);
   error_status_t		status;
   sec_passwd_rec_t		prec;
   boolean32			reset, val;
   sec_login_auth_src_t		auth_src;
   char				*p;
   sec_login_handle_t		auth_id;
   sec_rgy_handle_t		reg_context;
   status=error_status_ok;

       	/*
       	* Loggin by setting up the user's network identity.
       	* This operation will create any local context necessary
       	* to perform authenticated network operations.
       	*/
   if (sec_login_setup_identity ((unsigned_char_p_t) name, 
		sec_login_no_flags, 
		&auth_id, &status)){  
       	if (status) {
		err_prt ("sec_login_setup_identity", status);
		return (status);
	}
	/* sec_login_validate_id will nullify the password */
	p = (char *) malloc (strlen ((char *) pw) +1);
	if ( p == (char *) NULL)
       		 return (status = rpc_s_no_memory);
       	strcpy ((char *)p,(char *)pw);

	/* Set the password information for validate_identity */
	prec.key.key_type = sec_passwd_plain;
	prec.key.tagged_union.plain = (idl_char *) p;
	prec.pepper = NULL;
	prec.version_number = sec_passwd_c_version_none;

	if ( valid_cert_f == TRUE ){
		val=sec_login_valid_and_cert_ident (
		   auth_id, 
		   &prec, 
		   &reset, &auth_src, &status); 
		if (status) {
			err_prt("sec_login_valid_and_cert_ident",status);
			return (status);
		}
	}
	else {
		if (val=sec_login_validate_identity (auth_id, 
				&prec, 
				&reset, 
				&auth_src, 
				&status)) {
			if (status){
				err_prt("sec_login_validate_identity", status);
				return (status);
			}
			if ( ! sec_login_certify_identity(auth_id,
				&status ))
				err_prt("sec_login_certify_identity", status);
			}
		else {
			fprintf( stderr, "Failed validate identity\n");
			return(status);
		}
	}
        if ( val == 1) {
		dbprintf(("\tsecrgy_server: Valid and certified id\n"));
		if (val && auth_src == sec_login_auth_src_network) {
			dbprintf(("\tsecrgy_server: network wide creds\n"));
			sec_login_set_context (auth_id, &status);
			if (status){
				err_prt("sec_login_set_context", status);
				return(status);
			}
		}
		else if (val && auth_src == sec_login_auth_src_local) {
			 dbprintf(("\tsecrgy_server: val and local creds\n"));
			 return(error_status_ok);
	        }	
	}
   }
   else {
	printf("failed to sec_login_setup_identity\n"); 
	err_prt("sec_login_setup_identity", status);
	return (status);
   }
   printf("\nDCE logged in as %s\n", 
		name);
	log_out (&auth_id); 
	
	printf("\nDCE logged out as %s\n", 
			name);
	return(error_status_ok);
}
