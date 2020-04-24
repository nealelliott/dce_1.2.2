/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: misc.c,v $
 * Revision 1.1.13.1  1996/10/03  14:46:27  arvind
 * 	Silence compiler warnings.
 * 	Mark as volatile variables live across setjmp
 * 	[1996/09/16  20:30 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.11.2  1996/03/11  13:28:05  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:51  marty]
 * 
 * Revision 1.1.11.1  1995/12/08  18:06:48  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/10/30  15:22 UTC  mullan_s
 * 	Merge fix for CHFts15792
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15792/1  1995/10/30  14:43 UTC  mullan_s
 * 	sec_rgy_login_get_info: check info.status for binding retries.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/08  17:29 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl2/1  1994/12/07  19:31 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl1/2  1994/12/07  17:43 UTC  hanfei
 * 	change sec_rgy_checkpt_reset_interval interface
 * 
 * 	HP revision /main/hanfei_cpi_bl1/1  1994/12/05  14:48 UTC  hanfei
 * 	work for rgy checkpoint to be configurable
 * 	[1995/12/08  16:32:12  root]
 * 
 * Revision 1.1.7.1  1994/09/20  20:14:31  rps
 * 	[OT11944] fill in wait_until_consistent with polling code
 * 	[1994/09/20  20:07:46  rps]
 * 
 * Revision 1.1.4.3  1993/02/05  22:44:19  sommerfeld
 * 	[OT5061] Deal with change to CHECK_CACHE macro.
 * 	[1993/02/04  22:30:59  sommerfeld]
 * 
 * Revision 1.1.4.2  1992/12/29  13:02:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:40:36  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/14  18:57:17  burati
 * 	Mods to support overrides
 * 	[1992/05/14  01:50:22  burati]
 * 
 * Revision 1.1  1992/01/19  14:47:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  misc.c V=18 09/30/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 * Registry Client Agent Interface - miscellaneous
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)misc.c	18 - 09/30/91";
#endif

#include <rca_pvt.h> 
#include <rgymacro.h>

#include <dce/uuid.h>
#include <dce/rpc.h>
#include <rs_misc.h>
#include <dce/misc.h>
#include <dce/override.h>

#include <bstring.h>
#include <un_strng.h>
#include <u_str.h>

#define Crs_login_get_info(CH,b,CI,c,d,e,f,g,h,STATUS)\
        (*rs_misc_v1_0_c_epv.rs_login_get_info) (HANDLE(CH),b,CI,c,d,e,f,g,h,STATUS)

#define Crs_check_for_rep_consistency(CH,b,c,CI,d)\
        (*rs_misc_v1_0_c_epv.rs_check_consistency) (HANDLE(CH),b,c,CI,d)

#define Crs_checkpt_reset_interval(CH,b,c,d,CI,STATUS)\
        (*rs_misc_v1_0_c_epv.rs_checkpt_reset_interval) (HANDLE(CH),b,c,d,CI,STATUS)

#ifdef GLOBAL_LIBRARY
#   include <set_sect_pvt.c>
#endif

#define LOGIN_RESULT(R) (R).tagged_union.result

/*
 * get login info from the network authentication service.  Do not apply
 * overrides
 */
PUBLIC void sec_rgy_login_get_info
#ifndef __STDC__
    ( context, login_name, key_parts, sid, unix_sid, user_part, admin_part,
	 policy_data, max_number, supplied_number, id_projlist, 
         unix_projlist, num_projects, cell_name, cell_uuid, status )
    sec_rgy_handle_t       context;           /* [in] */
    sec_rgy_login_name_t   *login_name  ;     /* [in, out] */
    sec_rgy_acct_key_t     *key_parts;        /* [out] */
    sec_rgy_sid_t          *(sid);            /* [out] */
    sec_rgy_unix_sid_t     *(unix_sid);       /* [out] */
    sec_rgy_acct_user_t    *(user_part);      /* [out] */
    sec_rgy_acct_admin_t   *(admin_part);     /* [out] */
    sec_rgy_plcy_t         *(policy_data);    /* [out] */
    signed32               max_number;        /* [in] */
    signed32               *supplied_number;  /* [out] */
    uuid_t                 id_projlist[];     /* [out] */
    signed32               unix_projlist[];   /* [out] */
    signed32               *num_projects;     /* [out] */
    sec_rgy_name_t         cell_name;         /* [out] */
    uuid_t                 *cell_uuid;        /* [out] */
    error_status_t         *(status);         /* [out] */
#else
  (
    sec_rgy_handle_t       context,          /* [in] */
    sec_rgy_login_name_t   *login_name,      /* [in, out] */
    sec_rgy_acct_key_t     *key_parts,       /* [out] */
    sec_rgy_sid_t          *(sid),           /* [out] */
    sec_rgy_unix_sid_t     *(unix_sid),      /* [out] */
    sec_rgy_acct_user_t    *(user_part),     /* [out] */
    sec_rgy_acct_admin_t   *(admin_part),    /* [out] */
    sec_rgy_plcy_t         *(policy_data),   /* [out] */
    signed32               max_number,       /* [in] */
    signed32               *supplied_number, /* [out] */
    uuid_t                 id_projlist[],    /* [out] */
    signed32               unix_projlist[],  /* [out] */
    signed32               *num_projects,    /* [out] */
    sec_rgy_name_t         cell_name,        /* [out] */
    uuid_t                 *cell_uuid,       /* [out] */
    error_status_t         *(status)
  )
#endif
{
    rs_login_info_t        info;
    rs_cache_data_t        cache_info;

    if (!CHECK_LOGIN_NAME(login_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_read, status) {
        Crs_login_get_info(context, login_name, &cache_info, &info, 
                            max_number, supplied_number, id_projlist, 
                            unix_projlist, num_projects, status);
	/* 
	 * If status is good (which means there were no exceptions,
	 * including RPC communication failures), check if 
	 * info.status is an error status that we should try to 
	 * rebind on, such as secd being in a bad state 
	 * (sec_rgy_rep_bad_state).
	 */
	if (*status == error_status_ok)
	    SET_STATUS(status, info.status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
            if (sid) {
                *sid          = LOGIN_RESULT(info).sid;
            }
            if (unix_sid) {
                *unix_sid     = LOGIN_RESULT(info).unix_sid;
            }
            if (user_part) {
                *user_part    = LOGIN_RESULT(info).user_part;
            }
            if (admin_part) {
                *admin_part   = LOGIN_RESULT(info).admin_part;
            }
            if (policy_data) {
                *policy_data  = LOGIN_RESULT(info).policy_data; 
            }
            if (key_parts) {
                *key_parts    = LOGIN_RESULT(info).key_parts;  
            }
            if (cell_uuid) {
                *cell_uuid    = LOGIN_RESULT(info).cell_uuid;  
            }
            if (cell_name) {
                u_strcpy(cell_name, LOGIN_RESULT(info).cell_name);
            }
        }
        RETRY_CASE;
    }
    RETRY_END;

}


PUBLIC void sec_rgy_login_get_effective
#ifndef __STDC__
    ( context, login_name, key_parts, sid, unix_sid, user_part, admin_part,
	 policy_data, max_number, supplied_number, id_projlist, 
         unix_projlist, num_projects, cell_name, cell_uuid, overridden, status )
    sec_rgy_handle_t       context;           /* [in] */
    sec_rgy_login_name_t   *login_name  ;     /* [in, out] */
    sec_rgy_acct_key_t     *key_parts;        /* [out] */
    sec_rgy_sid_t          *(sid);            /* [out] */
    sec_rgy_unix_sid_t     *(unix_sid);       /* [out] */
    sec_rgy_acct_user_t    *(user_part);      /* [out] */
    sec_rgy_acct_admin_t   *(admin_part);     /* [out] */
    sec_rgy_plcy_t         *(policy_data);    /* [out] */
    signed32               max_number;        /* [in] */
    signed32               *supplied_number;  /* [out] */
    uuid_t                 id_projlist[];     /* [out] */
    signed32               unix_projlist[];   /* [out] */
    signed32               *num_projects;     /* [out] */
    sec_rgy_name_t         cell_name;         /* [out] */
    uuid_t                 *cell_uuid;        /* [out] */
    sec_override_fields_t  *overridden;       /* [out] */
    error_status_t         *(status);         /* [out] */
#else
  (
    sec_rgy_handle_t       context,          /* [in] */
    sec_rgy_login_name_t   *login_name,      /* [in, out] */
    sec_rgy_acct_key_t     *key_parts,       /* [out] */
    sec_rgy_sid_t          *(sid),           /* [out] */
    sec_rgy_unix_sid_t     *(unix_sid),      /* [out] */
    sec_rgy_acct_user_t    *(user_part),     /* [out] */
    sec_rgy_acct_admin_t   *(admin_part),    /* [out] */
    sec_rgy_plcy_t         *(policy_data),   /* [out] */
    signed32               max_number,       /* [in] */
    signed32               *supplied_number, /* [out] */
    uuid_t                 id_projlist[],    /* [out] */
    signed32               unix_projlist[],  /* [out] */
    signed32               *num_projects,    /* [out] */
    sec_rgy_name_t         cell_name,        /* [out] */
    uuid_t                 *cell_uuid,       /* [out] */
    sec_override_fields_t  *overridden,      /* [out] */
    error_status_t         *(status)
  )
#endif
{
    sec_rgy_unix_passwd_buf_t   pw_passwd;
    sec_rgy_name_t              pw_gecos;
    sec_rgy_name_t              pw_dir;
    sec_rgy_name_t              pw_shell;

    sec_rgy_login_get_info(context, login_name, key_parts, sid, unix_sid,
        user_part, admin_part, policy_data, max_number, supplied_number,
        id_projlist, unix_projlist, num_projects, cell_name, cell_uuid,
        status);

    if (GOOD_STATUS(status)) {
        override_get_login_info(login_name->pname, &unix_sid->person,
                                &unix_sid->group, pw_passwd, pw_gecos, pw_dir,
                                pw_shell, overridden, status);
        if ((GOOD_STATUS(status)) && *overridden) {
            if (FLAG_SET(*overridden, sec_override_pw_gecos)) {
                strncpy((char *)user_part->gecos, (char *)pw_gecos,
                        sec_rgy_pname_max_len);
            }
            if (FLAG_SET(*overridden, sec_override_pw_dir)) {
                strncpy((char *)user_part->homedir, (char *)pw_dir,
                        sec_rgy_pname_max_len);
            }
            if (FLAG_SET(*overridden, sec_override_pw_shell)) {
                strncpy((char *)user_part->shell, (char *)pw_shell,
                        sec_rgy_pname_max_len);
            }
            if (FLAG_SET(*overridden, sec_override_pw_passwd)) {
                strncpy((char *)user_part->passwd, (char *)pw_passwd,
                        sec_rgy_max_unix_passwd_len - 1);
            }
        } else if (STATUS_EQUAL(status, rpc_s_comm_failure)) {
            SET_STATUS(status, sec_login_s_override_failure);
        } else if (STATUS_EQUAL(status, sec_login_s_ovrd_ent_not_found)||
                   STATUS_EQUAL(status, sec_login_s_no_override_info) ) {
            CLEAR_STATUS(status);
        }
    }
}


PUBLIC boolean32 sec_rgy_wait_until_consistent
  (
    sec_rgy_handle_t    context,
    error_status_t      *(status)
  )
{
    volatile boolean32  result = false;
    boolean32		retry;
    rs_cache_data_t	cache_info;
    rs_update_seqno_t	seqno;
    volatile unsigned int	backoff = 0;

    seqno.high = 0;
    seqno.low = 0;

    while (1) {
	SETUP_RETRY(rca_op_write, status) {
	    result = Crs_check_for_rep_consistency(context, &retry, &seqno,
			&cache_info, status);
	    NORMAL_CASE {
		CHECK_CACHE(context, &cache_info);
	    }
	    RETRY_CASE;
	}
	RETRY_END;

	if (BAD_STATUS(status) || !retry)
	    break;

	sleep(backoff+=10);
    }

    /* Punt if talking to a 1.0.x sec master */
    if (STATUS_EQUAL(status, rpc_s_op_rng_error)) {
	SET_STATUS(status, sec_rgy_not_implemented);
    }

    return result;
}


PUBLIC void sec_rgy_checkpt_reset_interval
  (
    sec_rgy_handle_t    context,
    boolean32           do_checkpt_now,
    signed32		new_checkpt_interval,
    unsigned_char_p_t	at_time_str,
    error_status_t      *(status)
  )
{
    rs_cache_data_t	cache_info;
    
    SETUP_RETRY(rca_op_write, status) {
        Crs_checkpt_reset_interval(context, do_checkpt_now,
            new_checkpt_interval, at_time_str, &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;

}


/*
** Local Only Operations
*/

PUBLIC void sec_rgy_cursor_reset
#ifndef __STDC__
    ( cursor )  
    sec_rgy_cursor_t  *(cursor); /* [in, out] */
#else
  (
    sec_rgy_cursor_t  *(cursor)
  )
#endif
{
    if (cursor) {
        cursor->valid = false;
    }
}
