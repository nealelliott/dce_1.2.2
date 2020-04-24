/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utils.c,v $
 * Revision 1.1.73.2  1996/02/17  23:29:03  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:49  marty]
 *
 * Revision 1.1.73.1  1995/12/11  22:47:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/05  17:40 UTC  bissen
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/31  16:02  bissen
 * 	merge kk and hpdce01
 * 	[1995/12/11  22:22:19  root]
 * 
 * Revision 1.1.5.2  1993/07/09  14:36:49  root
 * 	Initial King Kong branch
 * 	[1993/07/09  14:36:19  root]
 * 
 * Revision 1.1.70.1  1993/09/09  19:56:25  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/09  19:51:13  cmckeen]
 * 
 * Revision 1.1.5.2  1993/07/02  16:40:28  eheller
 * 	GAMMERA version which runs under TET.
 * 	[1993/07/02  16:17:20  eheller]
 * 
 * Revision 1.1.3.2  1993/05/04  20:40:17  sekhar
 * 	1. In remove_acct() removed the call to do_site_open_update() .
 * 	   (the caller of remove_acct() is now responsible for doing this).
 * 
 * 	2. added a new function do_site_close() to close the registry site
 * 	   (using sec_rgy_site_close() )>
 * 
 * 	3. in add_account_adminserv call do_site_close() on both correct
 * 	   and error paths. (before the registry site was never being closed).
 * 
 * 	4. in do_account_prep() removed the superfluous call to
 * 	   do_site_open_update() .
 * 
 * 	5. in log_in() free p on error paths as well. (p is the string
 * 	   allocated to hold passwd for the duration of the call to
 * 	   sec_login_validate_identity() ).
 * 	[1993/05/03  20:23:47  sekhar]
 * 
 * 	Added login context to log_in, log_out calls and code cleanup.
 * 	[1993/03/25  18:40:43  eperkins]
 * 
 * 	Added the timer routines, and modified the log_out routine to use the
 * 	sec_login_get_current_context and sec_login_purge_context APIs.
 * 	[1993/03/11  00:06:02  eperkins]
 * 
 * 	Embedded copyright notice
 * 	[1993/02/05  14:47:13  cjd]
 * 
 * 	Initial version 1.0.2 system test sec.rgy.7
 * 	[1993/01/04  18:38:44  eperkins]
 * 
 * Revision 1.1.1.4  1993/04/21  20:28:30  root
 * 	    Loading drop DCE1_0_2B23
 * 
 * Revision 1.1.3.2  1993/02/04  22:18:17  cjd
 * 	Embedded copyright notice
 * 
 * $EndLog$
 */
#ifdef SNI_SVR4
#include <pthread.h>
#include <sys/signal.h>
#else
#include <sys/signal.h>
#include <pthread.h>
#endif /* SNI_SVR4 */
#include <stdlib.h>

#include <un_maloc.h>
#include <stdio.h>
#include <time.h>

#include <dce/rgybase.h>

#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/passwd.h>
#include <string.h>

#include <krb5/krb5.h>
#include <krb5/los-proto.h>     /* for krb5_read_password */
#include <dce/rpc.h>
#include <dce/daclif.h>

#include "rgy_test.h"
#include "rgy_clt.h"


#define KEY_TAB "/usr/tmp/v5srvtab"
#define MAXNAME 1024
#define MAX_SERVERS 20
#define MGR_ARR_SZ 10
#define ACL_ENT_ARR_SZ 1024 


static struct timeval tval1,tval2;

void err_prt(char * caller, error_status_t st)
{
	dce_error_string_t estr;
	int lst;

	dce_error_inq_text(st, estr, &lst);
	fprintf(stderr,"%s: %s\n", caller, estr);
}

void start(void)
{
	struct timezone tz;

        gettimeofday(&tval1, &tz);
}

void getimer(long *sec, long *usec)
{
        long tsec;
	struct timezone tz;

        gettimeofday(&tval2, &tz);
	if ( tval2.tv_usec < tval1.tv_usec ){
		tval2.tv_sec -= 1;
		tval2.tv_usec += 1000000;	
	}
		 
        tsec = (tval2.tv_sec - tval1.tv_sec) * 1000000 +
                tval2.tv_usec - tval1.tv_usec;
        *usec = tsec % 1000000;
        *sec = tsec / 1000000;
}


error_status_t
log_out (sec_login_handle_t *log_ctx)
{
	error_status_t st;

	sec_login_purge_context (log_ctx, &st);
	if (st){
		err_prt("sec_login_purge_context", st);
		return (st);
	}
	else  
		return (error_status_ok) ;
}

error_status_t log_in ( sec_login_handle_t *log_ctx, 
	char *acct, 
	char *passwd )
{
	error_status_t          st;
        sec_passwd_rec_t        prec;
        boolean32               reset, val;
        sec_login_auth_src_t    auth_src;
        error_status_t          lst;
	idl_char		*p;

        /*
         * Log-in by setting up the user's network identity.
         * This operation will create any local context necessary
         * to perform authenticated network operations.
         */

    st = error_status_ok ; /* initialize to default value */

    if (sec_login_setup_identity ((unsigned_char_p_t) acct, sec_login_no_flags, log_ctx, &st)){  
        if (st) {
	    err_prt("sec_login_setup_identity", st);
            return (st);
        }
        /* store the password away so validate_id does not munge it */
        p = (idl_char *)malloc(strlen((char *)passwd) + 1);
        if (p == NULL)
            return (st = rpc_s_no_memory);
        strcpy ((char *)p,(char *)passwd);

        /* Set the password information for validate_identity */
        prec.key.key_type = sec_passwd_plain;
        prec.key.tagged_union.plain = (idl_char *)p;
        prec.pepper = NULL;
        prec.version_number = sec_passwd_c_version_none;

        val = sec_login_validate_identity ((sec_login_handle_t) *log_ctx, 
             	&prec, &reset, &auth_src, &st) ;

        free(p);

        if (val) { 
	    if(!sec_login_certify_identity((sec_login_handle_t) *log_ctx, &st))
		   exit(sec_login_s_context_invalid);
            if(st){
	        err_prt("sec_login_validate_identity", st);
		return (st);
            }
            if (val && auth_src == sec_login_auth_src_network) {
                sec_login_set_context ((sec_login_handle_t) *log_ctx, &st);
        	if (st) {
                    err_prt("sec_login_set_context", st);
              	    return (st);
		}
		printf("Valid and certified with network creds\n");
            }
            else if (val && auth_src == sec_login_auth_src_local) {
	        printf("Valid with local creds\n");
	    }
        }
    }
    return(st);
}

error_status_t 
do_site_open_update ( sec_rgy_handle_t *rgy_context )
{
	error_status_t status;
        unsigned_char_t *site = (unsigned_char_t *)  NULL;

	sec_rgy_site_open_update ((unsigned_char_t *) site, 
			rgy_context, &status);
	if (status) {
		err_prt("sec_rgy_site_open_update", status);
	   	return (status);
	}
	else return (status = error_status_ok);
}

error_status_t 
do_site_close (sec_rgy_handle_t rgy_context)
{
        error_status_t  status ;

        sec_rgy_site_close (rgy_context, &status);
        if (status) {
                err_prt("sec_rgy_site_close", status);
                return (status);
        } 
        return (error_status_ok);
}

error_status_t
remove_acct (context, pname)
sec_rgy_handle_t        *context;
sec_rgy_name_t          pname;
{

	error_status_t status;

	/*
	 * Removing the principal.
	 * will also remove the account.
	 */

        sec_rgy_pgo_delete(*context, sec_rgy_domain_person, pname, &status);
        if (status == sec_rgy_object_not_found)
                status = error_status_ok;

        else if (status != error_status_ok) {
		err_prt("sec_rgy_pgo_delete", status);
                return (status);
        }

        return (status = error_status_ok);
}

error_status_t
add_account_adminserv ( pgo_name, unix_num, passwd, upasswd, st )
sec_rgy_name_t     pgo_name;
long               unix_num;
sec_passwd_str_t   passwd;
sec_passwd_str_t   upasswd;
error_status_t		*st;
{
	sec_rgy_login_name_t    account;
	sec_rgy_pgo_item_t      pgo_item;
        sec_rgy_cursor_t        marker;
        sec_rgy_login_name_t    name_result;
        sec_rgy_sid_t           sid;
        sec_rgy_unix_sid_t      unix_sid;
        sec_rgy_acct_key_t      key_parts;
        sec_rgy_acct_user_t     user_part;
        sec_rgy_acct_admin_t    admin_part;
        sec_passwd_rec_t        caller_key;
        sec_passwd_rec_t        new_key;
        sec_passwd_type_t       new_key_type;
        sec_passwd_version_t    new_key_version;
        unsigned_char_p_t       object;
	unsigned_char_p_t	*registry;
	char		group[256], org[256];
	sec_rgy_handle_t        reg_context; 
	error_status_t		status;

	if (status = do_site_open_update (&reg_context)) {
		err_prt("do_site_open_update", status);
		return (*st = status);
	} 

	/* Setup group and org for CDS object export */

	strcpy ((char *) group, "subsys/dce/cds-admin");
	strcpy ((char *) org, "none");

	bzero ((char *) &pgo_item, sizeof (pgo_item));

	/*
	 * Add the test principal.
	 */

	
	/* let the registry create a new uuid for principal */
	uuid_create_nil (&pgo_item.id, &status);
	if (status)
		err_prt("uuid_create_nil", status);

	pgo_item.unix_num = -1;
	pgo_item.quota = -1;
	pgo_item.quota = -1;
	strcpy ((char *) pgo_item.fullname,
		(char *) "RGY test admin server");  
	pgo_item.flags = sec_rgy_pgo_projlist_ok;

	sec_rgy_pgo_add (reg_context, 
		sec_rgy_domain_person,
		pgo_name,
		&pgo_item, 
		&status);

	/*
	 * If the principal is already there (sec_rgy_name_exists)
	 * set the status to ok and continue.
	 */
	if (status)
		err_prt("sec_rgy_pgo_add", status);
	if (status == sec_rgy_name_exists || status == sec_rgy_object_exists)
		*st = status = error_status_ok;

	if (status != error_status_ok){ 
		printf( "sec_rgy_pgo_add failed:%d\n", status); 
		return (*st = status);
	}

	/*
	 * Add principal as a member of the group "subsys/dce/cds-admin"
	 */
	if (! sec_rgy_pgo_is_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status)) {

		sec_rgy_pgo_add_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status);

		if (status != error_status_ok) { 
			err_prt( "sec_rgy_pgo_add_member", status);
			remove_acct (&reg_context, pgo_name);
			do_site_close(reg_context);
			return ( *st = status );
		}
	}
	strcpy ((char *) group, "subsys/dce/cds-server");

	/*
	 * Add principal as a member of the group "subsys/dce/cds-server"
	 */
	if (! sec_rgy_pgo_is_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status)) {

		sec_rgy_pgo_add_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status);

		if (status != error_status_ok) { 
			err_prt( "sec_rgy_pgo_add_member", status);
			remove_acct (&reg_context, pgo_name);
			do_site_close(&reg_context);
			return ( *st = status );
		}
	}

	strcpy ((char *) group, "subsys/dce/sec-admin");

	/*
	 * Add principal as a member of the group subsys/dce/sec-admin
	 */
	if (! sec_rgy_pgo_is_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status)) {

		sec_rgy_pgo_add_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status);

		if (status != error_status_ok) { 
			err_prt( "sec_rgy_pgo_add_member", status);
			remove_acct (&reg_context, pgo_name);
			do_site_close(&reg_context);
			return ( *st = status );
		}
	}

	strcpy ((char *) group, "acct-admin");
	/*
	 * Add principal as a member of the group "acct-admin"
	 */
	if (! sec_rgy_pgo_is_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status)) {

		sec_rgy_pgo_add_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status);

		if (status != error_status_ok) { 
			err_prt( "sec_rgy_pgo_add_member", status);
			remove_acct (&reg_context, pgo_name);
			do_site_close(&reg_context);
			return ( *st = status );
		}
	}

	/*
	 * Add principal as a member of the organization "none"
	 */
	if (! sec_rgy_pgo_is_member(reg_context, sec_rgy_domain_org, org, pgo_name, &status)) {

		sec_rgy_pgo_add_member(reg_context, sec_rgy_domain_org, org, pgo_name, &status);

		if (status != error_status_ok) {
			err_prt("sec_rgy_pgo_add_member org", status );  
			remove_acct (&reg_context, pgo_name);
			do_site_close(&reg_context);
			return ( *st = status );
		}
	}

	/*
	 * Add the principal as an account.
	 */
        key_parts = (sec_rgy_acct_key_t)sec_rgy_acct_key_person;

        bzero ((char *)&user_part, sizeof (user_part));
        user_part.passwd_version_number = 1;
        user_part.passwd_dtm = time(NULL);
        user_part.flags = sec_rgy_acct_user_passwd_valid;

        bzero ((char *)&admin_part, sizeof (admin_part));
        admin_part.creation_date = time(NULL);
        admin_part.change_date = time(NULL);
        admin_part.expiration_date = time(NULL) + 2*24*60*60;
        admin_part.good_since_date = time(NULL);
	admin_part.flags = sec_rgy_acct_admin_valid |
		sec_rgy_acct_admin_client | 
		sec_rgy_acct_admin_server ;
        admin_part.authentication_flags = sec_rgy_acct_auth_tgt |
		sec_rgy_acct_auth_renewable;

        bzero ((char *)&caller_key, sizeof (sec_passwd_rec_t));
        caller_key.version_number = sec_passwd_c_version_none;
        caller_key.key.key_type = sec_passwd_plain;
        caller_key.pepper=NULL;
        caller_key.key.tagged_union.plain = (idl_char *)upasswd;

        bzero ((char *)&new_key, sizeof (sec_passwd_rec_t));
        new_key.key.key_type = sec_passwd_plain;
        new_key.key.tagged_union.plain = passwd;

        bzero ((char *)&account, sizeof (sec_rgy_login_name_t));
	strcpy ((char *) account.pname, (char *) pgo_name);
	strcpy ((char *) account.gname, "subsys/dce/sec-admin");
	strcpy ((char *) account.oname, "none");

        sec_rgy_acct_add (reg_context, &account, &key_parts, &user_part,
                &admin_part, &caller_key, &new_key, sec_passwd_des,
                &new_key_version, &status);

	if (status == sec_rgy_object_exists || status == sec_rgy_name_exists)
		status = error_status_ok;

        if (status != error_status_ok) {
		err_prt("sec_rgy_acct_add", status);
		remove_acct (&reg_context, pgo_name);
		do_site_close(&reg_context);
                return ( *st = status );
        }
	do_site_close(&reg_context);
        return (*st = error_status_ok);
}

error_status_t
add_account_user (pgo_name, passwd, upasswd, st)
sec_rgy_name_t     pgo_name;
sec_passwd_str_t   passwd;
sec_passwd_str_t   upasswd;
error_status_t		*st;
{
	sec_rgy_login_name_t    account;
	sec_rgy_pgo_item_t      pgo_item;
        sec_rgy_cursor_t        marker;
        sec_rgy_login_name_t    name_result;
        sec_rgy_sid_t           sid;
        sec_rgy_unix_sid_t      unix_sid;
        sec_rgy_acct_key_t      key_parts;
        sec_rgy_acct_user_t     user_part;
        sec_rgy_acct_admin_t    admin_part;
        sec_passwd_rec_t        caller_key;
        sec_passwd_rec_t        new_key;
        sec_passwd_type_t       new_key_type;
        sec_passwd_version_t    new_key_version;
        unsigned_char_p_t       object;
	sec_rgy_name_t		group, org;
	sec_rgy_handle_t	reg_context;
	error_status_t		status;

	if (status = do_site_open_update (&reg_context)) {
		err_prt("do_site_open_update", status);
		return (*st = status);
	}

	/* Setup group for CDS object export */

	strcpy ((char *) group, "none");
	strcpy ((char *) org, "none");

	bzero ((char *) &pgo_item, sizeof (pgo_item));

	/*
	 * Add the test principal.
	 */

	
	uuid_create_nil (&pgo_item.id, &status);
	if (status)
		err_prt("uuid_create_nil", status);
	pgo_item.unix_num = -1;
	pgo_item.quota = -1;
	pgo_item.quota = -1;
	strcpy ((char *) pgo_item.fullname,
		(char *) "RGY test user");  
	pgo_item.flags = sec_rgy_pgo_projlist_ok;

	sec_rgy_pgo_add (reg_context, 
		sec_rgy_domain_person, pgo_name, &pgo_item, &status);

	/*
	 * If the principal is already there (sec_rgy_name_exists)
	 * set the status to ok and continue.
	 */

	if (status)
		err_prt("sec_rgy_pgo_add", status);
	if (status == sec_rgy_name_exists || status == sec_rgy_object_exists)
		*st = status = error_status_ok;

	if (status != error_status_ok){ 
		printf( "sec_rgy_pgo_add failed:%d\n", status); 
		return (*st = status);
	}

	/*
	 * Add principal as a member of the group "none"
	 */
	if (! sec_rgy_pgo_is_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status)) {

		sec_rgy_pgo_add_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status);

		if (status != error_status_ok) { 
			err_prt( "sec_rgy_pgo_add_member", status);
			remove_acct (&reg_context, pgo_name);
			do_site_close(&reg_context);
			return (*st = status);
		}
	}

	strcpy ((char *) group, "subsys/dce/cds-admin");
	/*
	 * Add principal as a member of the group "subsys/dce/cds-admin
	 * This group membership is needed so the server binding is readable.
	 */
	if (! sec_rgy_pgo_is_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status)) {

		sec_rgy_pgo_add_member(reg_context, sec_rgy_domain_group, group, pgo_name, &status);

		if (status != error_status_ok) { 
			err_prt( "sec_rgy_pgo_add_member", status);
			remove_acct (&reg_context, pgo_name);
			do_site_close(&reg_context);
			return (*st = status);
		}
	}

	/*
	 * Add principal as a member of the organization "none"
	 */
	if (! sec_rgy_pgo_is_member(reg_context, sec_rgy_domain_org, org, pgo_name, &status)) {

		sec_rgy_pgo_add_member(reg_context, sec_rgy_domain_org, org, pgo_name, &status);

		if (status != error_status_ok) {
			printf("sec_rgy_pgo_add_member org failed\n"); 
			remove_acct (&reg_context, pgo_name);
			do_site_close(&reg_context);
			return (*st = status);
		}
	}

	/*
	 * Add the principal as an account.
	 */
        key_parts = (sec_rgy_acct_key_t)sec_rgy_acct_key_person;

        bzero ((char *)&user_part, sizeof (user_part));
        user_part.passwd_version_number = 1;
        user_part.passwd_dtm = time(NULL);
        user_part.flags = sec_rgy_acct_user_passwd_valid;

        bzero ((char *)&admin_part, sizeof (admin_part));
        admin_part.creation_date = time(NULL);
        admin_part.change_date = time(NULL);
        admin_part.expiration_date = time(NULL) + 24*60*60;
        admin_part.good_since_date = time(NULL);
	admin_part.flags = sec_rgy_acct_admin_valid |
		sec_rgy_acct_admin_client | sec_rgy_acct_admin_server;
        admin_part.authentication_flags = sec_rgy_acct_auth_tgt;

        bzero ((char *)&caller_key, sizeof (sec_passwd_rec_t));
        caller_key.version_number = sec_passwd_c_version_none;
        caller_key.key.key_type = sec_passwd_plain;
        caller_key.pepper=NULL;
        caller_key.key.tagged_union.plain = (idl_char *)upasswd;

        bzero ((char *)&new_key, sizeof (sec_passwd_rec_t));
        new_key.key.key_type = sec_passwd_plain;
        new_key.key.tagged_union.plain = passwd;
	strcpy ((char *) account.pname, (char *) pgo_name);
	strcpy ((char *) account.gname, (char *) group );
	strcpy ((char *) account.oname, "none");

        sec_rgy_acct_add (reg_context, &account, &key_parts, &user_part,
                &admin_part, &caller_key, &new_key, sec_passwd_des,
                &new_key_version, &status);

	if (status == sec_rgy_object_exists || status == sec_rgy_name_exists)
		status = error_status_ok;

        if (status != error_status_ok) {
		err_prt("sec_rgy_acct_add", status);
		remove_acct (&reg_context, pgo_name);
		do_site_close(&reg_context);
                return (*st = status);
        }
        sec_rgy_site_close (reg_context, &status);
        if (status){
                err_prt("sec_rgy_site_close", status);
                return (*st = status);
        } 

        return (*st = error_status_ok);
}


error_status_t
do_account_prep (pgo_name, passwd)
sec_rgy_name_t     pgo_name;
sec_passwd_str_t   passwd;
{
   sec_passwd_rec_t   key;
   error_status_t       status;
   sec_rgy_handle_t   reg_context;

   /*
    * Add the admin server to the security data base.
    */
   if (status = add_account_adminserv ( pgo_name, 
		NULL, passwd, "-dce-", &status)) {
        err_prt("add_account_adminserv", status);
        return (status) ;
   } 

   printf("\n Added account: %s\n", pgo_name);

   /*  add key to key table */
   key.pepper = NULL;
   key.version_number = sec_passwd_c_version_none;
   key.key.key_type = sec_passwd_plain;
   key.key.tagged_union.plain = passwd;

   sec_key_mgmt_set_key(rpc_c_authn_dce_private, (void *)KEY_TAB,
        pgo_name, 1, &key, &status);
   if ( status == sec_key_mgmt_e_key_version_ex ) {
   		sec_key_mgmt_delete_key (rpc_c_authn_dce_private, 
			(void *)KEY_TAB, pgo_name, 1, &status); 
   		if (status) {
			err_prt ("sec_key_mgmt_del_key: ", status);
			return (status);
   		}
   		sec_key_mgmt_set_key(rpc_c_authn_dce_private, 
			(void *)KEY_TAB, pgo_name, 1, &key, &status);
   }
   if (status) {
		err_prt ("sec_key_mgmt_set_key: ", status);
		return (status);
   }
   printf ("\n Added key to keytab file\n");
   return (error_status_ok);
}

int
do_acct_cleanup (pgo_name, l_context, tstatus)
sec_rgy_name_t   pgo_name;
sec_login_handle_t *l_context;
error_status_t   *tstatus;
{

   error_status_t ret;
   sec_rgy_handle_t reg_context;

   if (ret = log_in (l_context, (char *)"cell_admin", (char *)"-dce-")) {
      err_prt("log_in", ret);
      return (*tstatus = ret);
   }

   if (ret = do_site_open_update ( &reg_context)) {
      log_out (l_context);
      return (*tstatus = ret);
   }

   /* remove server account */
   if(ret=remove_acct ( &reg_context, pgo_name)){
        log_out (l_context);
	do_site_close(&reg_context);
      	return (*tstatus = ret);
   }

   printf ("\n Removed account: %s.\n", pgo_name);

   /* delete key from keytab file */
   sec_key_mgmt_delete_key (rpc_c_authn_dce_private, (void *)KEY_TAB,
	 pgo_name, 1, &ret ); 
   if(ret){
	err_prt("sec_key_mgmt_delete_key", ret);
	return (*tstatus = ret );
   }
		

   printf ("\n Removed key from keytab file\n");
   sec_rgy_site_close ( reg_context, &ret);
   if ( ret ){
		err_prt ("sec_rgy_site_close",  ret );
		return (*tstatus = ret );
   	}

   return (*tstatus = error_status_ok);

}

error_status_t
do_site_open_query (sec_rgy_handle_t *rgy_context)
{
	error_status_t status;
	unsigned_char_t site_name[1024] = "";

	sec_rgy_site_open_query ((char *) site_name, rgy_context, &status);

	if (status) {
	   	return status;
	}
	return (error_status_ok);
}









