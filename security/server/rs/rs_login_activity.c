/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_login_activity.c,v $
 * Revision 1.1.9.3  1996/02/18  00:18:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:05  marty]
 *
 * Revision 1.1.9.2  1995/12/08  17:52:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:20:43  root]
 * 
 * Revision 1.1.5.1  1994/10/26  20:01:02  hondo
 * 	changes for propagation of login activity.
 * 	[1994/10/26  19:52:04  hondo]
 * 
 * Revision 1.1.2.3  1994/08/29  15:51:46  hondo
 * 	OT 11919 --login activity idl change to support different address types
 * 	[1994/08/28  21:45:46  hondo]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:07  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:25  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  15:01:33  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  18:00:22  mdf]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

#include <dce/sec_attr_util.h>

#include <rs_auth.h>
#include <rs_acct.h>
#include <rs_util.h>
#include <dce/rsbase.h>
#include <bstring.h>

#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <dce/rgynbase.h>


#include <rsdb_pvt.h>
#include <rsdb_acl.h>
#include <rsdb_util.h>
#include <rsdb_create.h>
#include <rsdb_jrnl.h>
#include <rsdb_login.h>
#include <dce/rs_login_base.h>
#include <dce/rs_login_activity.h>

/* PRIVATE FUNCTIONS
 */
PRIVATE uuid_t  *per_mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_person]);

/* PUBLIC FUNCTIONS
 */


/* r s _ l o g i n _ a c t i v i t y _ l o o k u p
 *
 * INPUT
 * INPUT/OUTPUT:
 * OUTPUT:
 *
 * Access Control: caller must have read permission on the object
 */

#define ACCT_RESULT(R) (R).tagged_union.result
PUBLIC void rs_login_activity_lookup
(
    handle_t			h,
    sec_rgy_login_name_t        *login_name,       /* [in] */
    sec_rgy_login_activity_t	*login_part,	   /* [out] */
    error_status_t		*st_p
)
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms =NULL;
    rs_acct_info_t          result;
    sec_rgy_cursor_t        cursor;     
    sec_rgy_foreign_id_t    client_ids;
    error_status_t	    status;


    CLEAR_STATUS(st_p);
    cursor.valid = false;
    memset(login_part,0,sizeof(sec_rgy_login_activity_t));

    if (!CHECK_LOGIN_NAME(login_name)) {
        SET_STATUS(&status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_acct_login_activity_lookup", &status);
    }

    if (GOOD_STATUS(&status)) {
        rsdb_name_canonicalize_and_convert(login_name->pname, &status);
    }

    requested_perms = sec_acl_perm_read;

    /*
     * need to read lock account database because the way
     * to index the login activity record is to lookup the
     * account record and get the login_activity id
     *
     * don't want an account to be deleted out from under us here
     */

    BEGIN_READ(lock_db,st_p) {
        READ_LOCK(lock_login_info) 
        if( rs_auth_is_authorized(h, requested_perms, parent_perms,
          	sec_rgy_domain_person, login_name->pname, per_mgr_type,
              	false, &client_ids, st_p)) {
        		rsdb_acct_lookup_login_activity(login_name, &cursor,
                                &(ACCT_RESULT(result).sid),
                                &(ACCT_RESULT(result).unix_sid),
                                &(ACCT_RESULT(result).key_parts),
				login_part,
                                &(ACCT_RESULT(result).user_part),
                                &(ACCT_RESULT(result).admin_part),
                                st_p);
		}
        END_READ_LOCK
    } END_READ

    return;
}



/*
 * r s _ l o g i n _ a c t i v i t y _ u p d a t e
 *
 * Access Control: caller must have update permission on the object
 */
PUBLIC void rs_login_activity_update
(
    handle_t			h,
    sec_rgy_login_name_t        *login_name,       /* [in] */
    sec_rgy_login_activity_t	*login_part,	   /* [in] */
    uuid_t			*client,	   /* [in] */
    error_status_t		*st_p
)
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms =NULL;
    char                    name_buf[3 * sizeof(sec_rgy_name_t) + 1];
    char                    buf[4 * sizeof(sec_rgy_name_t)];
    sec_rgy_foreign_id_t    client_ids;
    sec_timeval_sec_t       now; 


    rsdb_name_canonicalize_and_convert(login_name->pname, st_p);
    if (GOOD_STATUS(st_p)) {
        sprintf(name_buf, "%s [%s] [%s]",
                (char *) login_name->pname,
                (char *) login_name->gname,
                (char *) login_name->oname);
        sprintf(buf, "Attempt login activity update %s",name_buf);
    }

    BEGIN_MASTER_RIW(lock_login_info,st_p) {
    READ_LOCK(lock_db) {
	/* Check if caller is authorized for updates */
        requested_perms = sec_acl_perm_mgmt_info;
        if (rs_auth_is_authorized(h, requested_perms,
                             parent_perms, sec_rgy_domain_person,
                             login_name->pname, per_mgr_type,
                             false, &client_ids, st_p)){
        	rs_util_apply_timestamp(&now);
        	RIW_PROMOTE(lock_login_info);
		rsdb_acct_reset_login_activity(login_name,login_part,st_p);
	        if(STATUS_OK(st_p)) {
       		      sprintf(buf, "Updated login activity for %s",
			    (char *)login_name->pname);
			rs_log_login_reset_update(login_name,login_part,&now,
 			(rs_replica_master_info_t *) NULL, client,st_p);
		}
	}
    } END_READ_LOCK
    }END_MASTER_RIW

    return;
}

PRIVATE boolean32 string_to_rgy_name (
        char                    *principal,
        sec_rgy_login_name_t    *login_name
)
{
    boolean32           retval;
    char                *name;
    char                *p;
    signed32            pname_len;

    retval=0;
    if(p = index(principal, '@')) {
        int i, j;

        pname_len = p - principal;
        strncpy((char *)(login_name->pname), principal, pname_len);
        /*
         * !!! replace "\/" with  "/".
         */
        for (i=0; i < pname_len; i++)
        {
            if ((login_name->pname[i] == '\\') &&
                (login_name->pname[i+1] == '/'))
            {
                for (j=i; j<pname_len; j++) {
                    login_name->pname[j] = login_name->pname[j+1];
                }
                pname_len--;
            }
        }

        login_name->pname[pname_len] = '\0';
        login_name->gname[0]         = '\0';
        login_name->oname[0]         = '\0';
        retval = 0;
        rsdb_name_canonicalize(login_name->pname, (error_status_t *)&retval);
    } else {
        retval = 1;
    }
    return retval;
}

