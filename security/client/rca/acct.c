/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acct.c,v $
 * Revision 1.1.45.1  1996/10/03  14:45:54  arvind
 * 	clean up some compiler warnings.
 * 	[1996/09/16  20:30 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.43.3  1996/02/18  00:07:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:16  marty]
 * 
 * Revision 1.1.43.2  1995/12/08  18:06:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:32:00  root]
 * 
 * Revision 1.1.39.3  1994/10/24  13:37:10  cuti
 * 	Free string only if good status.
 * 	[1994/10/24  13:34:23  cuti]
 * 
 * Revision 1.1.39.2  1994/10/21  17:32:12  cuti
 * 	Free string unconditionally after homedir canonicalization.
 * 	[1994/10/21  14:45:15  cuti]
 * 
 * Revision 1.1.39.1  1994/10/21  13:54:46  cuti
 * 	Canonicalized homedir.
 * 	[1994/10/21  13:53:48  cuti]
 * 
 * Revision 1.1.37.1  1993/10/05  22:30:08  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:05:36  mccann]
 * 
 * Revision 1.1.2.3  1993/02/05  22:44:05  sommerfeld
 * 	[OT5061] Deal with change to CHECK_CACHE macro.
 * 	[1993/02/04  22:30:29  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/12/29  13:02:32  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:34  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:47:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  acct.c V=23 11/08/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
**      Registry Client Agent Interface - Account management
**
** Copyright Hewlett-Packard Company 1989, 1990
** Copyright Apollo Computer Inc. 1987, 1988
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)acct.c	23 - 11/08/91";
#endif

#include <u_str.h>
#include <rca_pvt.h>  
#include <rgymacro.h>

#include <acct.h>
#include <rs_acct.h>

#include <sysdep.h>
#include <rca_pwd.h>

#include <un_strng.h>
#include <bstring.h>

#define Crs_acct_add(CH,b,c,d,e,f,g,h,CI,i)\
        (*rs_acct_v1_0_c_epv.rs_acct_add) (HANDLE(CH),b,c,d,e,f,g,h,CI,i)

#define Crs_acct_delete(CH,b,CI,c)\
        (*rs_acct_v1_0_c_epv.rs_acct_delete) (HANDLE(CH),b,CI,c)

#define Crs_acct_lookup(CH,b,c,CI,d,STATUS)\
        (*rs_acct_v1_0_c_epv.rs_acct_lookup) (HANDLE(CH),b,c,CI,d,STATUS)

#define Crs_acct_lookup_check(CH,b,c,CI,d,STATUS)\
        (*rs_acct_v1_0_c_epv.rs_acct_lookup) (HANDLE(CH),b,c,CI,d,STATUS)

#define Crs_acct_replace(CH,b,c,d,e,f,g,h,i,CI,j)\
        (*rs_acct_v1_0_c_epv.rs_acct_replace) (HANDLE(CH),b,c,d,e,f,g,h,i,CI,j)

#define Crs_acct_get_projlist(CH,b,c,d,e,f,g,h,CI,i)\
        (*rs_acct_v1_0_c_epv.rs_acct_get_projlist) (HANDLE(CH),b,c,d,e,f,g,h,\
                                                      CI,i)
#define Crs_acct_rename(CH,b,c,d,e)\
        (*rs_acct_v1_0_c_epv.rs_acct_rename) (HANDLE(CH),b,c,d,e)

#define ACCT_RESULT(R) (R).tagged_union.result

#ifdef GLOBAL_LIBRARY
#   include <set_sect_pvt.c>
#endif


#ifdef NOTDEF_CHECK_ACCOUNT_ADD
/*
** Check to see if an account was succesfully added (invoked when an add
** operation yields the sec_rgy_object_exists error status.  This will check to
** see if the data that already exists is equivalent to the new data, and if 
** so return status_$ok)
*/
PRIVATE void check_account_add
  (
    sec_rgy_handle_t       context,
    sec_rgy_login_name_t   *login_name,    /* [in, ref] */
    sec_rgy_acct_key_t     *kp,            /* [in, out] */
    sec_rgy_acct_user_t    *up,            /* [in] */
    sec_rgy_acct_admin_t   *ap,            /* [in] */
    error_status_t         *(status_out)
  )
{
    sec_rgy_login_name_t   real_login_name;
    error_status_t         *status;
    error_status_t         lst;
    sec_rgy_cursor_t       cursor;
    boolean32              done = false;
    rs_acct_info_t         result;
    sec_rgy_acct_user_t    *rup;
    sec_rgy_acct_admin_t   *rap;
    boolean32              same;
    rs_cache_data_t        cache_info;

    COPY_LOGIN_NAME(&real_login_name, login_name);

    status = &lst;
    bzero(&result, sizeof(result));

    /*
    ** Retrieve account information from the "update" registry site.
    */

    SETUP_RETRY(rca_op_write, status) {
        same = true;
        Crs_acct_lookup_check(context, &real_login_name, &cursor,
                                &cache_info, &result, status);
        NORMAL_CASE {
            /*
             * No communication failure, so capture the operation status
             */
            CHECK_CACHE(context, &cache_info);
            SET_STATUS(status, result.status);
            rup = &(ACCT_RESULT(result).user_part);

            /*AUTH - compare id of last changer with id of
                     caller.  How do we get the caller's id???
                     if (bcmp(&(rup->last_changer), caller,
                         sizeof(*caller)))
                     same = false;
            */
                
            if (same && strcmp((char *) rup->homedir,
                                (char *) up->homedir))
                same = false;
            if (same && strcmp((char *) rup->shell, 
                               (char *) up->shell))
                same = false;
            if (same && !same_passwd(&(rup->passwd), &(up->passwd)))
                same = false;
            if (same && (rup->flags != up->flags))
                same = false;
            rap = &(ACCT_RESULT(result).admin_part);

/*AUTH - creator and changer sids become foreign_id_t(s)
                    if (same && bcmp(&(rap->creator), caller, sizeof(*caller)))
                        same = false;
                    if (same && bcmp(&(rap->last_changer), caller,
                            sizeof(*caller))) 
                        same = false;
*/
            if (same && (rap->expiration_date != ap->expiration_date))
                same = false;
            if (same && (rap->flags != ap->flags))
                same = false;
        } /* NORMAL_CASE */

        if (same) {
            SET_STATUS(status_out, error_status_ok);
            *kp = ACCT_RESULT(result).key_parts;
        }
        RETRY_CASE;
    }
    RETRY_END;
}
#endif /* NOTDEF_CHECK_ACCOUNT_ADD */


PRIVATE  void  gen_key_transmit_rep
  (
    sec_rgy_handle_t       context,      /* [in] */
    sec_passwd_rec_t       *caller_key,  /* [in] */
    sec_passwd_rec_t       *new_key,     /* [in] */
    rs_acct_key_transmit_t **trep,       /* [out] */
    error_status_t         *stp
  )
{
    sec_rgy_bind_auth_info_t   auth_info;

    CLEAR_STATUS(stp);
    
    sec_rgy_site_binding_get_info(context, NULL, NULL, NULL, &auth_info, stp);
    if (GOOD_STATUS(stp)) {
	if (auth_info.info_type == sec_rgy_bind_auth_none) {
	    SET_STATUS(stp, sec_rgy_cant_authenticate);
	} else if (auth_info.tagged_union.dce_info.authz_svc != rpc_c_authz_dce) {
	    SET_STATUS(stp, sec_rgy_cant_authenticate);
	}
    }
    
    if (GOOD_STATUS(stp)) {
	rca_pwd_gen_transmit_rep(auth_info.tagged_union.dce_info.identity,
				 caller_key, new_key, trep, stp);
    }
}

PUBLIC void sec_rgy_acct_add
  (
    sec_rgy_handle_t          context,                 /* [in] */
    sec_rgy_login_name_t      *login_name,             /* [in] */
    sec_rgy_acct_key_t        *key_parts,              /* [in, out] */
    sec_rgy_acct_user_t       *(user_part),            /* [in] */
    sec_rgy_acct_admin_t      *(admin_part),           /* [in] */
    sec_passwd_rec_t          *(caller_key),           /* [in] */
    sec_passwd_rec_t          *(new_key),              /* [in] */
    sec_passwd_type_t         new_keytype,             /* [in] */
    sec_passwd_version_t      *new_key_version,        /* [out] */
    error_status_t            *(status)                /* [out] */
  )                    
{                        
    int                        num_tries = 0;
    rs_cache_data_t            cache_info;
    rs_acct_key_transmit_t     *pickled_key;
    unsigned_char_p_t          ucp;
    error_status_t              xst;

    CLEAR_STATUS(status);

    /* Canonicalize homedir */
    rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, user_part->homedir, &ucp, status);
    if (GOOD_STATUS(status)) {
	/* check the homedir len */
	if (u_strlen(ucp) <= sec_rgy_pname_max_len) {
	    u_strcpy(user_part->homedir, ucp);
	}
	else {
	    SET_STATUS(status, sec_rgy_homedir_too_long);
	    return;
	}
        rpc_string_free(&ucp, &xst);
    }
    /* Bad status means not DCE naming compliance, so pass through. */

    /*
     * make sure user_part->passwd is a zero-length string so update clients don't
     * have to worry about it
     */
    user_part->passwd[0] = '\0';

    if (!(CHECK_LOGIN_NAME(login_name)
            && CHECK_ACCT_KEY_PARTS(key_parts)   /* enforce person abbrev */
            && CHECK_ACCT_USER_DATA(user_part)) ) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    gen_key_transmit_rep(context, caller_key, new_key, &pickled_key, status);

    if (BAD_STATUS(status)) {
        return;
    }

    /*XXX DON'T allow multiple tries here (see comment below) */

    SETUP_RETRY(rca_op_write, status) {
        Crs_acct_add(context, login_name, key_parts, user_part,
                     admin_part, pickled_key, new_keytype, new_key_version,
                     &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;  

    rca_pwd_free_transmit_rep(&pickled_key);


    /*
    ** A communication failure above could have forced us to retry the add
    ** with the possible result of getting a sec_rgy_object_exists error (if the
    ** server actually completed the initial request, but the client saw a 
    ** comm failure).  If we get this error status, we simply retrieve the
    ** account record and see if it is equivalent to the one we were adding -
    ** if so, we go ahead and assert that the operation succeeded.
    **
    ** [11/28/90 - greg] Because we no longer put encrypted keys on the
    ** wire, we can't tell if it's the "same" account.  This means that
    ** two successve password changes might appear to succeed when, in
    ** fact the second fails.  We opt instead to try once and return
    ** the comm failure status, as this is closer to what is understood
    ** as a "general" problem with weak consistency.  We leave the old
    ** code here for documentation purposes.
    **
    **    if ((num_tries > 1) && (STATUS(status) == sec_rgy_object_exists)) {
    **    check_account_add(context, login_name, key_parts, user_part,
    **                            admin_part, status);
    **    }
    */

}

PUBLIC void sec_rgy_acct_delete
  (
    sec_rgy_handle_t       context,        /* [in] */
    sec_rgy_login_name_t   *login_name,    /* [in] */
    error_status_t         *(status)
  )
{
    rs_cache_data_t        cache_info;

    if (!CHECK_LOGIN_NAME(login_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_write, status) {
        Crs_acct_delete(context, login_name, &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}


PUBLIC void sec_rgy_acct_lookup
  (
    sec_rgy_handle_t       context,           /* [in] */
    sec_rgy_login_name_t   *name_key,         /* [in, out] */
    sec_rgy_cursor_t       *(account_cursor), /* [in, out] */
    sec_rgy_login_name_t   *name_result,      /* [out] */
    sec_rgy_sid_t          *(id_sid),         /* [out] */
    sec_rgy_unix_sid_t     *(unix_sid),       /* [out] */
    sec_rgy_acct_key_t     *(key_parts),      /* [out] */
    sec_rgy_acct_user_t    *(user_part),      /* [out] */
    sec_rgy_acct_admin_t   *(admin_part),     /* [out] */
    error_status_t         *(status)
  )
{
    rs_acct_info_t         result;
    rs_cache_data_t        cache_info;

    if (!CHECK_LOGIN_NAME(name_key)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    /* preserve the (possibly wildcarded) name_key unchanged so the
     * caller doesn't have to
     */
    COPY_LOGIN_NAME(name_result, name_key);

    SETUP_RETRY(rca_op_read, status) {
        Crs_acct_lookup(context, name_result, account_cursor,
                        &cache_info, &result, status);
        NORMAL_CASE {
            /*
             * No communication failure, so capture the operation status
             */
            CHECK_CACHE(context, &cache_info);
            SET_STATUS(status, result.status);
            if (id_sid) {
                *id_sid     = ACCT_RESULT(result).sid;
            }
            if (unix_sid) {
                *unix_sid   = ACCT_RESULT(result).unix_sid;
            }
            if (key_parts) {
                *key_parts  = ACCT_RESULT(result).key_parts;
            }
            if (user_part) {
                *user_part  = ACCT_RESULT(result).user_part;
            }
            if (admin_part) {
                *admin_part = ACCT_RESULT(result).admin_part;
            }
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PRIVATE void rgy_acct_replace
  (
    sec_rgy_handle_t       context,                  /* [in] */
    sec_rgy_login_name_t   *login_name,              /* [in] */
    sec_rgy_acct_key_t     *key_parts,               /* [in, out] */
    rs_acct_parts_t        modify_parts,             /* [in] */
    sec_rgy_acct_user_t    *(user_part),             /* [in] */
    sec_rgy_acct_admin_t   *(admin_part),            /* [in] */
    sec_passwd_rec_t       *(caller_key),            /* [in] */
    sec_passwd_rec_t       *(new_key),               /* [in] */
    sec_passwd_type_t      new_keytype,              /* [in] */
    sec_passwd_version_t   *(new_key_version),       /* [out] */
    error_status_t         *(status)
  )
{
    volatile boolean32         change_passwd;
    boolean32                  passwd_valid = true;
    boolean32                  key_parts_valid = true;
    rs_cache_data_t            cache_info;
    rs_acct_key_transmit_t     *pickled_key = NULL;

    change_passwd = FLAG_SET(modify_parts, rs_acct_part_passwd);

    if (modify_parts & rs_acct_part_login_name) {
        key_parts_valid = CHECK_ACCT_KEY_PARTS(key_parts);
    }

    if (!(CHECK_LOGIN_NAME(login_name)
            && key_parts_valid           /* enforce person abbrev */
            && CHECK_ACCT_USER_DATA(user_part)) ) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    if (change_passwd) {
	gen_key_transmit_rep(context, caller_key, new_key, &pickled_key, status);
        if (BAD_STATUS(status)) {
            return;
        }
    }

    /*
     * make sure user_part->passwd is a zero-length string so API clients don't
     * have to worry about it
     */
    user_part->passwd[0] = '\0';

    SETUP_RETRY(rca_op_write, status) {
        Crs_acct_replace(context, login_name, key_parts, modify_parts,
                         user_part, admin_part, pickled_key, new_keytype,
			 new_key_version, &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;

    if (change_passwd) {
	rca_pwd_free_transmit_rep(&pickled_key);
    }

}

PUBLIC void sec_rgy_acct_passwd
  (
    sec_rgy_handle_t       context,                /* [in] */
    sec_rgy_login_name_t   *login_name,            /* [in] */
    sec_passwd_rec_t       *caller_key,            /* [in] */
    sec_passwd_rec_t       *new_key,               /* [in] */
    sec_passwd_type_t      new_keytype,            /* [out] */
    sec_passwd_version_t   *new_key_version,       /* [out] */
    error_status_t         *(status)
  )
{
    rs_acct_parts_t        modify_parts;
    sec_rgy_acct_admin_t   admin_part;
    sec_rgy_acct_user_t    user_part;
    sec_rgy_acct_key_t     key_parts;

    /*
    ** argument length checking handled in rgy_acct_replace
    */

    modify_parts = rs_acct_part_passwd;
    bzero(&admin_part, sizeof(admin_part));
    bzero(&user_part, sizeof(user_part));
    
    key_parts = sec_rgy_acct_key_none;
    rgy_acct_replace(context, login_name, &key_parts, modify_parts, 
		     &user_part, &admin_part, caller_key, new_key, 
		     new_keytype, new_key_version, status);
    
}

PUBLIC void sec_rgy_acct_user_replace
  (
    sec_rgy_handle_t       context,                  /* [in] */
    sec_rgy_login_name_t   *login_name,              /* [in] */
    sec_rgy_acct_user_t    *(user_part),             /* [in] */
    boolean32              set_password,             /* [in] */
    sec_passwd_rec_t       *caller_key,              /* [in] */
    sec_passwd_rec_t       *new_key,                 /* [in] */
    sec_passwd_type_t      new_keytype,              /* [out] */
    sec_passwd_version_t   *new_key_version,         /* [out] */
    error_status_t         *(status)                 /* [out] */
  )
{
    rs_acct_parts_t        modify_parts;
    sec_rgy_acct_admin_t   admin_part;
    sec_rgy_acct_key_t     key_parts;
    unsigned_char_p_t      ucp;
    error_status_t         xst;

    CLEAR_STATUS(status);

    /* Canonicalize homedir */
    rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, user_part->homedir, &ucp, status);
    if (GOOD_STATUS(status)) {
	/* check the homedir len */
	if (u_strlen(ucp) <= sec_rgy_pname_max_len) {
	    u_strcpy(user_part->homedir, ucp);
	}
	else {
	    SET_STATUS(status, sec_rgy_homedir_too_long);
	    return;
	}
        rpc_string_free(&ucp, &xst);
    }
    /* Bad status means not DCE naming compliance, so pass through. */

    /*
    ** argument length checking handled in rgy_acct_replace
    */

    modify_parts = rs_acct_part_user;
    if (set_password) {
        modify_parts |= rs_acct_part_passwd;
    }
    bzero(&admin_part, sizeof(admin_part));

    key_parts = sec_rgy_acct_key_none;
    rgy_acct_replace(context, login_name, &key_parts, modify_parts, 
		     user_part, &admin_part, caller_key, new_key,  
		     new_keytype, new_key_version, status);
}

PUBLIC void sec_rgy_acct_admin_replace
  (
    sec_rgy_handle_t       context,        /* [in] */
    sec_rgy_login_name_t   *login_name,    /* [in] */
    sec_rgy_acct_key_t     *key_parts,     /* [in, out] */
    sec_rgy_acct_admin_t   *(admin_part),  /* [in] */
    error_status_t         *(status)
  )
{
    rs_acct_parts_t        modify_parts;
    sec_rgy_acct_user_t    dummy_user_part;
    sec_passwd_rec_t       *dummy_key = NULL;
    sec_passwd_version_t   dummy_key_version;

    /*
    ** argument length checking handled in rgy_acct_replace
    */

    modify_parts = rs_acct_part_admin | rs_acct_part_login_name;
    bzero(&dummy_user_part, sizeof(dummy_user_part));

    rgy_acct_replace(context, login_name, key_parts, modify_parts,
		     &dummy_user_part, admin_part, NULL, NULL, 
		     sec_passwd_none, &dummy_key_version, status);

}


PUBLIC void sec_rgy_acct_replace_all
  (                                      
    sec_rgy_handle_t       context,                  /* [in] */
    sec_rgy_login_name_t   *login_name,              /* [in] */
    sec_rgy_acct_key_t     *key_parts,               /* [in, out] */
    sec_rgy_acct_user_t    *(user_part),             /* [in] */
    sec_rgy_acct_admin_t   *(admin_part),            /* [in] */
    boolean32              set_password,             /* [in] */
    sec_passwd_rec_t       *caller_key,              /* [in] */
    sec_passwd_rec_t       *new_key,                 /* [in] */
    sec_passwd_type_t      new_keytype,              /* [out] */
    sec_passwd_version_t   *new_key_version,         /* [out] */
    error_status_t         *(status)
  )
{
    rs_acct_parts_t        modify_parts;
    unsigned_char_p_t      ucp;
    error_status_t         xst;

    CLEAR_STATUS(status);



    /* Canonicalize homedir */
    rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, user_part->homedir, &ucp, status);
    if (GOOD_STATUS(status)) {
	/* check the homedir len */
	if (u_strlen(ucp) <= sec_rgy_pname_max_len) {
	    u_strcpy(user_part->homedir, ucp);
	}
	else {
	    SET_STATUS(status, sec_rgy_homedir_too_long);
	    return;
	}
        rpc_string_free(&ucp, &xst);
    }
    /* Bad status means not DCE naming compliance, so pass through. */

    /*
    ** argument length checking handled in rgy_acct_replace
    */

    modify_parts = rs_acct_part_user | rs_acct_part_admin
                    | rs_acct_part_login_name;
    if (set_password) {
        modify_parts |= rs_acct_part_passwd;
    }

    rgy_acct_replace(context, login_name,  key_parts, modify_parts,
		     user_part, admin_part, caller_key, new_key, 
		     new_keytype, new_key_version, status);
}


PUBLIC void sec_rgy_acct_get_projlist
  (
    sec_rgy_handle_t       context,           /* [in] */
    sec_rgy_login_name_t   *login_name,       /* [in] */
    sec_rgy_cursor_t       *projlist_cursor,  /* [in, out] */
    volatile signed32      max_number,        /* [in] */
    signed32               *supplied_number,  /* [out] */
    uuid_t                 id_projlist[],     /* [out] */
    signed32               unix_projlist[],   /* [out] */
    signed32               *num_projects,     /* [out] */
    error_status_t         *(status)
  )
{
    boolean32              done = false;
    rs_cache_data_t        cache_info;

    if (!CHECK_LOGIN_NAME(login_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    if (max_number > 512) {
        max_number = 512;
    }

    SETUP_RETRY(rca_op_read, status) {
        Crs_acct_get_projlist(context, login_name, projlist_cursor,
                        max_number, supplied_number, id_projlist, 
                        unix_projlist,  num_projects, &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}
       
PUBLIC void sec_rgy_acct_rename
  (
    sec_rgy_handle_t       context,          /* [in] */
    sec_rgy_login_name_t   *old_login_name,  /* [in] */
    sec_rgy_login_name_t   *new_login_name,  /* [in] */
    sec_rgy_acct_key_t     *new_key_parts,   /* [in, out] */
    error_status_t         *status
  )
{
    boolean32              done = false;

    if (!CHECK_LOGIN_NAME(old_login_name) || 
	!CHECK_LOGIN_NAME(new_login_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_read, status) {
        Crs_acct_rename(context, old_login_name, new_login_name,
                        new_key_parts, status);
        NORMAL_CASE;
        RETRY_CASE;
    }
    RETRY_END;
}
