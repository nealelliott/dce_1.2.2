/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: dcecred.c,v $
 * Revision 1.1.6.1  1996/10/02  17:49:48  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:11  damon]
 *
 * Revision 1.1.2.3  1994/09/16  19:48:05  maunsell_c
 * 	if tgt exists, use it w/o -dceid prompt
 * 	[1994/09/16  15:26:47  maunsell_c]
 * 
 * Revision 1.1.2.2  1994/08/08  18:54:34  mckeen
 * 	Added s12y support
 * 	[1994/08/08  17:46:51  mckeen]
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:01  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:42  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:01:49  mckeen]
 * 
 * $EndLog$
 */
/* 
 * dcecred.c - Implementation of dcecred 
 */
/*
 * this modules role is to represent DCE credentials and
 * provide attributes of it.  It can be initialized with
 * a DCE username.   After validation, a PAG representing the cred
 * can be obtained, as well as expiration time info.
 */
#include <dce/dce.h>
#include <stdio.h>
#include <time.h>
#include <com_err.h>
#include <pwd.h>
#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>

#include <dcedfs/param.h> 	/* To get macro AFS_SUNOS5_ENV defined */
#ifdef AFS_SUNOS5_ENV
#undef MIN
#undef MAX
#endif

#include <dcedfs/osi.h>
#include <dcedfs/syscall.h>
#include <dcecred.h>
#include <dfsgwymac.h>
#include <dfsgwysvc.h>
#include <dfsgwymsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgw/dcecred.c,v 1.1.6.1 1996/10/02 17:49:48 damon Exp $")

extern char *progname;
extern boolean32 dceid_supplied;

static char *dcred_username = NULL;
static sec_login_handle_t lcontext = NULL;
static boolean32 validated = NULL;
static dcecred_t dcred;
static dcecred_p dce_cred;
static int refreshed_identity = 0;
static int preexistingId = 0;

int dcecred_validate(char *, char *);
long dcecred_pag();
time_t dcecred_expiration_time();

/*
 * valid - returns 1 if string non-null, otherwise 0
 */
static valid(char *s)
{
if (s)
   return(1);
else
   return(0);
}

/*
 * invalidate - sets string NULL 
 */
static void
invalidate(char **s)
{
   *s = NULL;
}

/*
 * valid_ticket - determine if valid ticket  exists already
 */ 
int valid_ticket(unsigned long pag, int refresh)
{
    error_status_t  st;
    int code = 0;

    sec_login_context_from_pag(pag, &lcontext, &st);
    if (st)
	return 0;

    if (refresh) {
	code = sec_login_refresh_identity(lcontext, &st);
	if (code == 0 || st) 
	    return 0;
	refreshed_identity = 1;
    }
    return 1;
}

/*
 * init_context - Initialize login context for this cred. 
 */
void
init_context(unsigned long pag, int *existing)
{

   error_status_t st;                              /*  DCE status */
   struct passwd *pw;
   uid_t uid;

   *existing = 0;
   validated = false;
   preexistingId = 0;
   uid = getuid();
   if ((uid == 0) || 
       ((pw = getpwuid(uid)) && (strcmp(dcred_username, pw->pw_name) == NULL)))
       if (valid_ticket(pag, 1 /* refresh existing ticket */)) {
	   *existing = 1;
	   preexistingId = 1;
	   return;
       }
   
   /* if dceid was supplied, go ahead and setup identity */
   if (dceid_supplied) {
      if ((!sec_login_setup_identity((unsigned_char_p_t)dcred_username, sec_login_proxy_cred, &lcontext, &st)) || (st)) {
         /*
          * Error on identity - ignore username.
          */ 
         invalidate(&dcred_username);
      }
   } else {
      invalidate(&dcred_username);
   } 
   return;
}
   
/*
 * dcecred_validate - Validate login context.
 *
 * This routine will prompt for needed pieces.  Returns -1 on failure.
 */

dcecred_validate(char *username, char *password) 
{
   char pass_store[sec_passwd_str_t_size]; /* Storage for password */
   dce_error_string_t err_string;          /* DCE error string */
   sec_passwd_rec_t prec;                  /* Password record */
   sec_login_auth_src_t auth_src;
   boolean32 reset_password;
   error_status_t st;
   int lst; 
   char read_username[sec_passwd_str_t_size];
   uid_t uid;
   int success;

   /*
    * if ticket found during init_context already validated 
    */

   if (validated) 
       return(0);

   /*
    * Process username and password
    */
   if (refreshed_identity)
       osi_assert(valid(username) || valid(dcred_username));
   else {
       if (!valid(username)) {
	   if (!valid(dcred_username)) {
	       /* read user input */
	       dce_printf(gwy_s_enterprinc);
	       gets(read_username); 
	       fflush(NULL);
	       dcred_username = read_username;
	       /* now setup login identity */
	       if ((!sec_login_setup_identity
		    ((unsigned_char_p_t) dcred_username, 
		     sec_login_proxy_cred, &lcontext, &st)) || (st)) {
		   dce_error_inq_text(st, err_string, &lst);
		   if (lst)
		       sprintf((char *)err_string, "%d", st);
		   dce_svc_printf(GWY_S_SETUPIDENTITYERR_MSG, progname, 
				  err_string);
		   return (-1);
	       }
	   }
       }
   }

   if (!valid(password)) {
      lst = sizeof(pass_store) - 1;
      if (krb5_read_password("Enter Password: ", (char *)0, 
			     (char *)pass_store, &lst)) {
         invalidate(&dcred_username);
         dce_svc_printf(GWY_S_DCEPASSWDREADERR_MSG, progname);
         return(-1);
      }
      password = pass_store;
   }
   /*
    * Validate password
    */
   prec.key.key_type = sec_passwd_plain;
   prec.key.tagged_union.plain = (unsigned char *)password;
   prec.pepper = 0;
   prec.version_number = sec_passwd_c_version_none;

#ifdef DFSGW_DCECRED_ROOT_CERT
   if ((getuid() == 0) && !preexistingId)
       success = 
	   sec_login_valid_and_cert_ident(lcontext, &prec, 
					  &reset_password, &auth_src, &st);
   else 
#endif
       success = 
	   sec_login_validate_identity(lcontext, &prec, &reset_password, 
				       &auth_src, &st);
   
   if (success) {
       if (st) {
	  dce_error_inq_text(st, err_string, &lst);
	  if (lst) {
	      sprintf((char *)err_string, "%d", st);
	  }
	  dce_svc_printf(GWY_S_VALIDATEIDENTITYERR_MSG, progname, err_string);
	  return(-1);
      }
      if (reset_password) {
         dce_svc_printf(GWY_S_CHANGEDCEPASSWD_MSG, progname);
      }
      if (auth_src == sec_login_auth_src_local) {
          dce_svc_printf(GWY_S_CREDLOCALREG_MSG, progname);
      }
      if (auth_src == sec_login_auth_src_overridden) {
         dce_svc_printf(GWY_S_NOCRED_MSG, progname);
         return(-1);
      } else {
	  if (!refreshed_identity) {
	      /*
	       * Set context
	       */
	      sec_login_set_context(lcontext, &st);
	      if (st) {
		  dce_error_inq_text(st, err_string, &lst);
		  if (lst) {
		      sprintf((char *)err_string, "%d", st);
		  }
		  dce_svc_printf(GWY_S_SETCONTEXTERR_MSG, progname, 
				 err_string); 
		  return(-1);
	      }
	  }
	  validated = true;
      } 
   } else {
      dce_error_inq_text(st, err_string, &lst);
      if (lst) {
         sprintf((char *)err_string, "%d", st);
      }
        dce_svc_printf(GWY_S_PASSWDVALERR_MSG, progname, err_string);
        return(-1);
      }
   return(0);
}

/*
 * dcecred_init - store stuff away.
 */
dcecred_p
dcecred_init(char *username, unsigned long pag, int *existing_cred) 
{
    dcred.validate = dcecred_validate; 
    dcred.pag = dcecred_pag;
    dcred.expiration_time = dcecred_expiration_time;
    dce_cred = &dcred;
    if (username) {
	dcred_username = username;
	(void) init_context(pag, existing_cred);
    } else {
	dcred_username = NULL;
    }
    return(dce_cred);
}

/*
 * dcecred_pag - Return PAG for given creds.  Returns -1 on error.  Optionally
 * validates identity if not already done.
 */ 
long
dcecred_pag() 
{
   error_status_t st;                          /* DCE status */
   dce_error_string_t err_string;              /* DCE error string */
   long pag;
   int lst;
 
  
   if (!validated) {
      if (dcecred_validate(0, 0) < 0) {
         return(-1);
      }
   }
   pag = sec_login_inq_pag((sec_login_handle_t *)lcontext, &st);
   if ((st) || (!pag)) {
      dce_error_inq_text(st, err_string, &lst);
      if (lst) {
         sprintf((char *)err_string, "%d", st);
      }
      dce_svc_printf(GWY_S_GETPAGERR_MSG, progname, err_string); 
      pag = -1;
   }  
   return(pag);
}

/*
 * dcecred_expiration_time - Return expiration time of cred.
 *
 * Returns 0 on failure. 
 */
time_t
dcecred_expiration_time()
{
   sec_login_net_info_t net_info;
   time_t result;
   error_status_t st;

 
   sec_login_inquire_net_info(lcontext, &net_info, &st);
   if ((st) && (st != sec_login_s_not_certified)) {
      result = 0;
   } else {
      result = net_info.identity_expiration_date;
   }
   sec_login_free_net_info(&net_info);
   return(result);
}

