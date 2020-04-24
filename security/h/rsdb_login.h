/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_login.h,v $
 * Revision 1.1.9.1  1996/07/08  18:33:24  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:52 UTC  psn  /main/DCE122_PK/1]
 *
 * 	header file spaghetti cleanup.
 *
 * 	add prototypes for formerly-protoless functions.
 *
 * 	Ones under #if 0 should be here, but aren't due to dependancy of types
 * 	not definitely included by this source file.
 * 	[1996/05/08  19:41 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/1]
 *
 * Revision 1.1.7.3  1996/02/18  23:00:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:48  marty]
 * 
 * Revision 1.1.7.2  1995/12/08  17:28:16  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:14 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:35 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	changes for dynamic allocation of login activity records
 * 	[1995/04/03  22:12 UTC  hondo  /main/SEC_migrate/1]
 * 
 * 	changes to pack/unpack names in login activity records.
 * 	[1995/03/28  19:30 UTC  hondo  /main/hondo_mig1/1]
 * 
 * Revision 1.1.2.3  1994/08/29  15:51:32  hondo
 * 	OT 11919 --login activity idl change to support different address types
 * 	[1994/08/28  21:48:13  hondo]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:47  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:04  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  14:59:48  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:59:37  mdf]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1994
 * Unpublished work. All Rights Reserved.
 */
/*
 * Registry Server - login information
 */

#ifndef rsdb_login_h__included
#define rsdb_login_h__included

#include <lm_login.h>

typedef struct rsdb_acct_login_activity_t {
    sec_timeval_sec_t      dis_time;
    signed32               cur_inv_attempts;
    sec_timeval_sec_t      last_bad_time;
    sec_timeval_sec_t      last_good_time;
    lm_address_t           good_origin;
    lm_address_t           bad_origin;
} rsdb_acct_login_activity_t;

typedef struct rsdb_login_activity_rec_t {
    sec_timeval_sec_t      dis_time;
    signed32               cur_inv_attempts;
    sec_timeval_sec_t      last_bad_time;
    sec_timeval_sec_t      last_good_time;
    krb5_int32             good_origin_addrtype;
    krb5_int32             good_origin_length;
    krb5_int32             bad_origin_addrtype;
    krb5_int32             bad_origin_length;
} rsdb_login_activity_rec_t;

typedef struct login_addr_t{
    union{
        unsigned32              orig_ip_addr;
        unsigned char           string_binding[sec_rgy_name_max_len];
        sec_attr_twr_set_t      twr_set;
    } l_addr;
} login_addr_t;

#define STILL_DISABLED(x,y) ( (x) > (y) ? true : false )

void rsdb_login_create ();
boolean32 rsdb_login_init ();

void import_login_to_rsdb(
    rsdb_acct_login_activity_t   *activity,
    char                         *buf,
    error_status_t               *st
);

void export_rsdb_to_login(
    rsdb_acct_login_activity_t   *activity,
    char                         *data,
    error_status_t               *st
);
void rsdb_purge_login_activity_by_id(rsdb_pvt_id_t id);
void rsdb_store_login_activity_by_id(
    rsdb_pvt_id_t      id,
    rsdb_acct_login_activity_t   *activity
);
boolean32 rsdb_get_login_activity_by_id(
    rsdb_pvt_id_t id,
    rsdb_acct_login_activity_t   *activity
);

boolean32 rsdb_acct_lookup_login_activity(
    sec_rgy_login_name_t   *login_name,
    sec_rgy_cursor_t       *(cursor),
    sec_rgy_sid_t          *(sid),
    sec_rgy_unix_sid_t     *(unix_sid),
    sec_rgy_acct_key_t     *(key_parts),
    sec_rgy_login_activity_t    *(login_part),
    sec_rgy_acct_user_t    *(user_part),
    sec_rgy_acct_admin_t   *(admin_part),
    error_status_t         *(status)
);

boolean32 rsdb_acct_replace_login_activity(
    sec_rgy_login_name_t   *login_name,
    sec_rgy_login_activity_t    *(login_part),
    error_status_t         *(status)
);

void lm_get_login_info
(    sec_rgy_login_name_t   *login_name,     /* [in, out] */
    sec_rgy_cursor_t       *(cursor),       /* [in, out] */
    sec_rgy_login_activity_t    *(login_part),   /* [out] */
    error_status_t         *(status)            /* [out] */
);

void lm_disable_acct (
    rsdb_pvt_id_t          id,
    sec_rgy_login_name_t   *login_name,     
    error_status_t         *st
);

void lm_reset_acct (
    rsdb_pvt_id_t          id,
    sec_rgy_login_name_t   *login_name,     
    lm_upd_action_t 	   *action,	    
    error_status_t         *st
);

void lm_log_invalid_attempt(
    rsdb_pvt_id_t          id,
    lm_upd_action_t 	   *action,	    
    error_status_t         *st
);

void lm_log_valid_attempt(
    rsdb_pvt_id_t          id,
    lm_upd_action_t 	   *action,	    
    error_status_t         *st
);

boolean32 lm_calculate_disable_time (
    sec_rgy_login_name_t   *login_name,
    error_status_t          *status
);

#if 0
void lm_copy_journal (
    jrnl_rec_t 		*from,	 	/* [in]*/
    sec_origin_t    	*to            /* [in,out] */
);

void lm_consolidate_data ( 
    sec_rgy_login_activity_t    *new_data,       /* [in,out] */
    rsdb_acct_journal_t 	*journal,	 /* [in,out]*/
    rsdb_acct_login_activity_t  *login_activity /* [in,out]*/
);

void lm_copy_login (
    lm_address_t  		*from,  /* [in]*/
    sec_origin_t    		*to    /* [out] */
)

#endif

void lm_log_login_activity_update (
    sec_rgy_login_name_t        *login_name,            /* [in]*/
    rsdb_acct_login_activity_t  *login,                 /* [in]*/
    error_status_t              *st
);



#endif

