/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: tst_dce_login.c,v $
 * Revision 1.1.5.2  1996/02/17  23:11:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:26  marty]
 *
 * Revision 1.1.5.1  1995/12/11  19:54:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:40  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:44:47  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:27  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/10  22:38:25  bhaim
 * 	Common TET functions
 * 	[1994/03/10  22:36:03  bhaim]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <errno.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/passwd.h>
#include <string.h>

void
tst_dce_login(char *user, char *password, error_status_t *status)
{
    boolean32           pflag = false, cflag = false;
    sec_passwd_str_t    pbuf;
    boolean32           reset_passwd, valid;
    sec_login_auth_src_t auth_src;
    char                buf[2048];
    int                 ret, i, len, retval, lst;
    sec_login_handle_t  lcontext;
    error_status_t      st;
    sec_passwd_rec_t    prec;
    dce_error_string_t  err_string;
    struct rlimit              *rlp;

    strncpy((char *)pbuf, password, sec_passwd_str_max_len);
    pbuf[sec_passwd_str_max_len] = '\0';


    if (sec_login_setup_identity((unsigned_char_p_t) user,
                                    sec_login_no_flags, &lcontext, &st)) {


      prec.key.key_type = sec_passwd_plain;
      prec.key.tagged_union.plain = (unsigned char *) pbuf;
      prec.pepper = NULL;
      prec.version_number = sec_passwd_c_version_none;
      
      valid = sec_login_validate_identity(lcontext, &prec, &reset_passwd,
                                                &auth_src, &st);
      if (valid) {
	
	if (st) {
	  dce_error_inq_text(st, err_string, &lst);
	  fprintf(stderr,"Validation error: %s\n", err_string);
	}
	
	if (reset_passwd) {
	  fprintf(stderr, "Password must be changed!\n");
	}
	if (auth_src == sec_login_auth_src_local) {
	  fprintf(stderr, "Credentials obtained from local registry.\n");
	}
	if (auth_src == sec_login_auth_src_overridden) {
	  fprintf(stderr, "Validated from local override entry, no network credentials obtained.\n");
	} else {
	  sec_login_set_context(lcontext, &st);
	  if (st) {
	    dce_error_inq_text(st, err_string, &lst);
	    fprintf(stderr,"Unable to set context: %s\n", err_string);
	    *status= (error_status_t) 1;
	    return;
	  }
	}
      } else {
	fprintf(stderr,"Sorry.\nPassword Validation Failure.");
	if (st) {
	  dce_error_inq_text(st, err_string, &lst);
	  fprintf(stderr," - %s\n", err_string);
	} else {
	  fprintf(stderr,"\n");
	}
	*status= (error_status_t) 1;
	return;
      }
    } else {
      fprintf(stderr,"Sorry.\nUser Identification Failure.");
      if (st) {
	dce_error_inq_text(st, err_string, &lst);
	fprintf(stderr," - %s\n", err_string);
      } else {
	fprintf(stderr,"\n");
      }
      *status= (error_status_t) 1;
      return;
    }
    *status=error_status_ok;
    return;
  }

