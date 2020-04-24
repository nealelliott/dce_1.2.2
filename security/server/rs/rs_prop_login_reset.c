/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_prop_login_reset.c,v $
 * Revision 1.1.7.3  1996/02/18  00:19:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:33  marty]
 *
 * Revision 1.1.7.2  1995/12/08  17:54:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:09  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:11  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:27  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/26  20:01:04  hondo
 * 	changes to support propagation of login activity.
 * 	[1994/10/25  14:58:04  hondo]
 * 
 * 	changes for replication of login activity.
 * 
 * $EndLog$
 */


/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Propagation server manager routines - Login Activity
 */
    
#include <dce/uuid.h>
#include <dce/rgynbase.h>
#include <rs_server.h>
#include <rs_pvt.h>
#include <rs_log_acct.h>
#include <rs_prop_acct.h>
#include <rs_pwd.h>
#include <rs_ch_lock.h>
#include <rs_master.h>
#include <rs_rep_auth.h>
#include <rs_rep_mgmt.h>
#include <rsdb_util.h>
#include <rsdb_acct.h>
#include <rsdb_jrnl.h>
#include <rsdb_login.h>
#include <crypt.h>

      
/*
 * rs_prop_login_reset
 */
PUBLIC  void  rs_prop_login_reset
  (
    handle_t                  h,              /* [in] */
    sec_rgy_login_name_t      * login_name,   /* [in] */
    sec_rgy_login_activity_t  * login_part,   /* [in] */
    uuid_t		      * client,       /* [in] */
    sec_timeval_sec_t         * time_now,     /* [in, ref] */
    rs_replica_master_info_t  * master_info,  /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            * status       /* [out] */
  )
{
    boolean32               apply_update;
    sec_timeval_sec_t       now;


    CLEAR_STATUS(status);

    rs_rep_auth_check(h, status);
    if (BAD_STATUS(status)) {
        return;
    }
   
    if (! propq_only) {

       BEGIN_SLAVE_WRITE_EVENT(rs_c_event_slave_write, lock_db, master_info, status) 
	{ 
            rs_rep_mgmt_slave_chk_update(master_info, &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
		    WRITE_LOCK(lock_login_info)
                    rsdb_acct_reset_login_activity(login_name, login_part,
                            status);   
		    END_WRITE_LOCK
		    
                }
                if (GOOD_STATUS(status)) {
                    rs_log_login_reset_update(login_name,login_part,time_now,
                         master_info, client,status);
                }
            }
        } END_SLAVE_WRITE_EVENT;

    } 
    else {
        /* just add update to propq
         */
        BEGIN_RIW_EVENT(rs_c_event_propq_write, lock_db, status) { 
            rs_rep_mgmt_slave_chk_propq_upd(master_info, 
                                            &apply_update, status);
            if (apply_update) {
                if (GOOD_STATUS(status)) {
                    rs_log_login_reset_update(login_name,login_part,time_now,
                         master_info, client,status);
                } 
            }
        } END_RIW_EVENT;
    }
}

