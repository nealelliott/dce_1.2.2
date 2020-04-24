/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_login.c,v $
 * Revision 1.1.10.3  1996/02/18  00:21:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:26  marty]
 *
 * Revision 1.1.10.2  1995/12/08  17:59:51  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/17  23:48 UTC  rps
 * 	resolve conflict with hierarchical cells.
 * 	[1995/05/08  21:36 UTC  hondo  /main/hondo_m1/1]
 * 
 * 	SEC migration Warranty Patch merge.
 * 	[1995/04/26  19:49 UTC  jrr  /main/DCE1_1_WP/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:12 UTC  greg
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	change to allocate login activity records dynamically
 * 	[1995/04/03  22:18 UTC  hondo  /main/SEC_migrate/1]
 * 
 * 	changes to pack/unpack names in login activity records.
 * 	[1995/03/28  19:24 UTC  hondo  /main/hondo_mig1/1]
 * 
 * Revision 1.1.6.1  1994/10/26  20:01:06  hondo
 * 	changes for replication of login activity.
 * 	[1994/10/24  20:07:08  hondo]
 * 
 * Revision 1.1.2.4  1994/08/29  15:51:57  hondo
 * 	OT 11919 - login activity idl change to support different address types
 * 	[1994/08/28  21:42:51  hondo]
 * 
 * Revision 1.1.2.3  1994/08/19  17:59:39  mdf
 * 	1994/08/18  sekhar
 * 	[ OT 11757 ]  Fix bad ptr references in rsdb_update_login_data().
 * 	              Log messages using SVC calls.
 * 	[1994/08/19  13:11:25  mdf]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:09  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:28  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  15:02:52  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  18:00:30  mdf]
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
*/
/*
 *      Registry Server - database interlude - Login activity management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$ID: $";
#endif

#include <stdio.h>
#include <un_strng.h>
#include <un_time.h>
#include <un_maloc.h>

#include <dce/id_base.h>
#include <rs_base.h>
#include <rs_util.h>
#include <rs_mkey.h>

#include <rsdb_pvt.h>
#include <rsdb_jrnl.h>
#include <rsdb_login.h>
#include <rsdb_acct.h>
#include <rsdb_auth.h>
#include <rsdb_pgo.h>
#include <rsdb_pgo_util.h>
/*UNIX #include <rsdb_unix.h> */
#include <rsdb_util.h>
#include <rsdb_policy.h>
#include <rsdb_attr.h>
#include <sec_attr_tools.h>
#include <dce/rs_login_base.h>


#include <bstring.h>
#include <sec_krb_util.h>

/* Kerberos header files */
#include <krb5/kdb.h>
#include <krb5/des_int.h>

#define setflag(flagfield, flag) (flagfield |= (flag))


/*   Login Activity contains information common across replicas
 *   related to login activity for a principal.
 *
 *   The information tracked is propagted from the master to
 *   the replicas.  On each replica this common information is
 *   combined with local information(from the journal activity)
 *   and used to allow or disallow logins.
 *   
 */

static rsdb_acct_login_activity_t    first;
static rsdb_acct_login_activity_t    last;

/*
 *	Initialize the internal login_activity list
 *      by reading two records, first and last
 */
PUBLIC boolean32 rsdb_login_init ()
{

    if(!rsdb_get_login_activity_by_id(START_OF_LIST, &first)) {
        fatal("Couldn't initialize login list");
    }
    if(!rsdb_get_login_activity_by_id(END_OF_LIST, &last)) {
        fatal("Couldn't initialize login list");
    }
}


/*
 *	Read the login activity database and return
 *      the record whose sequential id is supplied as
 *      an input argument
 *
 *      LOCKING should occurr before this is called.
 *      	READ lock should be taken on lock_login_info
 */

PUBLIC boolean32 rsdb_get_login_activity_by_id
    (id, activity)
    rsdb_pvt_id_t      id;
    rsdb_acct_login_activity_t   *activity;
{
    rsdb_sequential_key_t  sequential_key;
    char                   *data=NULL;
    int                    data_len=0;
    error_status_t         st;

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = id;
    CLEAR_STATUS(&st);

    rsdb_fetch(rsdb_login_db, (Pointer) &sequential_key, sizeof(sequential_key),
                        &data, &data_len, &st);
    if (BAD_STATUS(&st)) {
        /* Signal end of list */
        return false;
    }
    CLEAR_STATUS(&st);
    if (data_len > 0){
    	export_rsdb_to_login(activity,data,&st);
    	if (BAD_STATUS(&st)) {
        	return false;
    	}
        else
                return true;
    }
    else
    	return false;
}

PUBLIC void export_rsdb_to_login
    (activity, data, st)
    rsdb_acct_login_activity_t   *activity;
    char *data;
    error_status_t               *st;
{
    rsdb_login_activity_rec_t   *login_rec;
    login_addr_t                *addrs;

    CLEAR_STATUS(st);
    login_rec=(rsdb_login_activity_rec_t *)data;
    addrs=(login_addr_t *)data;
    addrs+=sizeof(rsdb_login_activity_rec_t);

    activity->dis_time=login_rec->dis_time;
    activity->cur_inv_attempts=login_rec->cur_inv_attempts;
    activity->last_bad_time=login_rec->last_bad_time;
    activity->last_good_time=login_rec->last_good_time;
    switch (login_rec->good_origin_addrtype){
        case  ADDRTYPE_INET:
            activity->good_origin.addrtype=sec_ip_addr;
	    activity->good_origin.contents.orig_ip_addr=addrs->l_addr.orig_ip_addr;
            activity->good_origin.length=login_rec->good_origin_length;
            break;
        case  ADDRTYPE_DCE:
            activity->good_origin.addrtype=sec_dce_string;
            if (login_rec->good_origin_length <= sec_rgy_name_max_len){
                  bcopy((char *)(addrs->l_addr.string_binding),
	 	     (char *)(activity->good_origin.contents.string_binding),
                     login_rec->good_origin_length);
                  activity->good_origin.length=login_rec->good_origin_length;
            }
            break;
        case ADDRTYPE_CHAOS:
        case ADDRTYPE_XNS :
        case ADDRTYPE_ISO:
        case ADDRTYPE_DDP:
        case ADDRTYPE_UNSPECIFIED:
        default:
                activity->good_origin.addrtype=login_rec->good_origin_addrtype;
                activity->good_origin.length=0;
    }
    addrs+=login_rec->good_origin_length;
    switch (login_rec->bad_origin_addrtype){
        case  ADDRTYPE_INET:
            activity->bad_origin.addrtype=sec_ip_addr;
	    activity->bad_origin.contents.orig_ip_addr=addrs->l_addr.orig_ip_addr;
            activity->bad_origin.length=login_rec->good_origin_length;
            break;
        case  ADDRTYPE_DCE:
            activity->bad_origin.addrtype=sec_dce_string;
            if (login_rec->bad_origin_length <= sec_rgy_name_max_len){
                  bcopy((char*)(addrs->l_addr.string_binding[0]),
	 	      (char *)(activity->bad_origin.contents.string_binding[0]),
                      login_rec->bad_origin_length);
                  activity->bad_origin.length=login_rec->bad_origin_length;
            }
            break;
        case ADDRTYPE_CHAOS:
        case ADDRTYPE_XNS :
        case ADDRTYPE_ISO:
        case ADDRTYPE_DDP:
        case ADDRTYPE_UNSPECIFIED:
        default:
                activity->bad_origin.addrtype=login_rec->bad_origin_addrtype;
                activity->bad_origin.length=0;
    }

}

/*
 *	Store the login activity record whose sequential id is supplied as
 *      an input argument
 *
 *      LOCKING should occurr before this is called.
 *      	READ_INTEND_WRITE lock should have been taken on lock_login_info
 *      	and promoted to WRITE lock
 */

PUBLIC void rsdb_store_login_activity_by_id
    (id, activity)
    rsdb_pvt_id_t      id;
    rsdb_acct_login_activity_t   *activity;
{
    rsdb_sequential_key_t  sequential_key;
    char                   *data=NULL;
    int                    data_len=0;
    error_status_t         st;     

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    CLEAR_STATUS(&st);
    sequential_key.record_type = SEQUENTIAL_TYPE;
    /*
     *  The id for a record is initialized in the acct record
     *  when it is created for the principal and remains the same
     *  until the account is deleted.
     */
    sequential_key.id = id;

    /*
     *  Since the record stored in the database is of variable
     *  length, calculate the size based on the number and
     *  length of origins, allocate the space, and clear it.
     */
    data_len=sizeof(rsdb_login_activity_rec_t);
    data_len+=(activity->good_origin.length + activity->bad_origin.length);
    if ( ! (data=malloc(data_len)) ){
        SET_STATUS(&st, sec_s_no_memory);
        return;
    }
    memset(data, NULL,data_len);

    /*
     *  The data coming in is in a fixed format record and must
     *  be converted to the database format to save space.
     */
    import_login_to_rsdb(activity,data,&st);
    if (GOOD_STATUS(&st)) {
    	rsdb_replace(rsdb_login_db, (Pointer) &sequential_key, 
    		sizeof(sequential_key), (Pointer) data, data_len, &st);
    }
    if ( data != NULL )
	free(data);
}

PUBLIC void import_login_to_rsdb
    (activity, buf, st)
    rsdb_acct_login_activity_t   *activity;
    char *buf;
    error_status_t         *st;     
{
    rsdb_login_activity_rec_t   *login_rec;
    login_addr_t                *addrs;

    CLEAR_STATUS(st);
    login_rec=(rsdb_login_activity_rec_t *)buf;
    addrs=(login_addr_t *)buf;
    addrs+=sizeof(rsdb_login_activity_rec_t);

    login_rec->dis_time=activity->dis_time;
    login_rec->cur_inv_attempts= activity->cur_inv_attempts;
    login_rec->last_bad_time= activity->last_bad_time;
    login_rec->last_good_time= activity->last_good_time;

    switch (activity->good_origin.addrtype){
        case  ADDRTYPE_INET:
            login_rec->good_origin_addrtype=sec_ip_addr;
            addrs->l_addr.orig_ip_addr=activity->good_origin.contents.orig_ip_addr;
	    login_rec->good_origin_length=activity->good_origin.length;
            break;
        case  ADDRTYPE_DCE:
            login_rec->good_origin_addrtype=sec_dce_string;
            if (login_rec->good_origin_length <= sec_rgy_name_max_len){
               bcopy((char *)(activity->good_origin.contents.string_binding),
                    (char *)(addrs->l_addr.string_binding),
                    activity->good_origin.length);
	        login_rec->good_origin_length= activity->good_origin.length;
            }
	    else{
                SET_STATUS(st, sec_id_e_name_too_long);
	        login_rec->good_origin_length= 0;
	    }
            break;
        case ADDRTYPE_CHAOS:
        case ADDRTYPE_XNS :
        case ADDRTYPE_ISO:
        case ADDRTYPE_DDP:
        case ADDRTYPE_UNSPECIFIED:
        default:
                login_rec->good_origin_addrtype=activity->good_origin.addrtype;
	        login_rec->good_origin_length= 0;
    }
    addrs+=activity->good_origin.length;
    switch (activity->bad_origin.addrtype){
        case  ADDRTYPE_INET:
            login_rec->bad_origin_addrtype=sec_ip_addr;
            addrs->l_addr.orig_ip_addr=activity->bad_origin.contents.orig_ip_addr;
            login_rec->bad_origin_length= activity->bad_origin.length;
            break;
        case  ADDRTYPE_DCE:
            activity->bad_origin.addrtype=sec_dce_string;
            if (login_rec->bad_origin_length <= sec_rgy_name_max_len){
                  bcopy((char *)(activity->bad_origin.contents.string_binding),
                      (char *)(addrs->l_addr.string_binding),
                      activity->bad_origin.length);
                  login_rec->bad_origin_length= activity->bad_origin.length;
            }
	    else{
                SET_STATUS(st, sec_rgy_bad_name);
	        login_rec->bad_origin_length= 0;
	    }
            break;
        case ADDRTYPE_CHAOS:
        case ADDRTYPE_XNS :
        case ADDRTYPE_ISO:
        case ADDRTYPE_DDP:
        case ADDRTYPE_UNSPECIFIED:
        default:
                login_rec->bad_origin_addrtype=activity->bad_origin.addrtype;
	        login_rec->bad_origin_length= 0;
    }
}


/*
 *	Create the login_activity database by
 *      loading two records, first and last
 *      All others will be inserted between these two.
 */

PUBLIC void rsdb_login_create()
{
    bzero((Pointer) &first, sizeof(rsdb_acct_login_activity_t));
    rsdb_store_login_activity_by_id(START_OF_LIST, &first);

    bzero((Pointer) &last, sizeof(rsdb_acct_login_activity_t));
    rsdb_store_login_activity_by_id(END_OF_LIST, &last);
}

/*
 *	DELETE the login activity record whose sequential id is supplied as
 *      an input argument
 *
 *      LOCKING should occurr before this is called.
 *      	READ_INTEND_WRITE lock should have been taken on lock_login_info
 *      	and promoted to WRITE lock
 */
PUBLIC void rsdb_purge_login_activity_by_id
    (id)
    rsdb_pvt_id_t      id;
{
    rsdb_sequential_key_t  sequential_key;
    error_status_t         st;

    CLEAR_STATUS(&st);
    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = id;

    rsdb_delete(rsdb_login_db, (Pointer) &sequential_key, 
		sizeof(sequential_key), &st);
}


/*
 * 	rsdb_acct_lookup_login_activity
 *
 *      The lookup of the login activity data in the login database
 *      will require that a lookup be done on the principal name first
 *      and then the id in the acct record is used to do a second lookup
 *      of the account activity record.
 *
 *      LOCKING should occurr before this is called.
 *      	READ_INTEND_WRITE lock should be taken on lock_login_info
 *
 */

PUBLIC boolean32 rsdb_acct_lookup_login_activity
    ( login_name, cursor, sid, unix_sid, key_parts,
	login_part, user_part, admin_part, status )
    sec_rgy_login_name_t   *login_name;     /* [in, out] */
    sec_rgy_cursor_t       *(cursor);      /* [in, out] */
    sec_rgy_sid_t          *(sid);         /* [out] */
    sec_rgy_unix_sid_t     *(unix_sid);    /* [out] */
    sec_rgy_acct_key_t     *(key_parts);   /* [out] */
    sec_rgy_login_activity_t *(login_part);   /* [out] */
    sec_rgy_acct_user_t    *(user_part);   /* [out] */
    sec_rgy_acct_admin_t   *(admin_part);  /* [out] */
    error_status_t         *(status);      /* [out] */
{
    rsdb_named_item_t      person;
    rsdb_named_item_t      group;
    rsdb_named_item_t      org;
    sec_rgy_acct_key_t     name_parts;
    rsdb_acct_item_t       account;
    rsdb_pvt_id_t          account_id;
    unsigned char          *p;
    rsdb_acct_login_activity_t    login;

    CLEAR_STATUS(status);
    memset(&login,0,sizeof(rsdb_acct_login_activity_t));

    if (rsdb_acct_lookup_acct_id (login_name,  cursor,
                                  &name_parts, &person,
                                  &group, &org,
                                  &account, status)) {
        p = login_name->pname;
        get_name_component(sec_rgy_domain_person, account.person_id, name_parts,
                                (char **) &p, &person);

        p = login_name->gname;
        get_name_component(sec_rgy_domain_group, account.group_id, name_parts,
                                (char **) &p, &group);

        p = login_name->oname;
        get_name_component(sec_rgy_domain_org, account.org_id, name_parts,
                                (char **) &p, &org);

        *key_parts      = account.key_parts;
        *admin_part     = account.admin_part;
        export_acct_user_part(&account, user_part);

        rsdb_util_get_sid(&RSDB_PGO(&person), &RSDB_PGO(&group),
                         &RSDB_PGO(&org), unix_sid, sid);
	rsdb_get_login_activity_by_id(account.login_activity_id,&login);
        login_part->dis_time=login.dis_time;
        login_part->cur_inv_attempts=login.cur_inv_attempts;
        login_part->last_bad_time=login.last_bad_time;
        login_part->last_good_time=login.last_good_time;
	lm_copy_login(&(login.good_origin),&(login_part->good_origin));
	lm_copy_login(&(login.bad_origin),&(login_part->bad_origin));
    } else if (STATUS_OK(status)) {
        SET_STATUS(status, sec_rgy_object_not_found);
    }
}

PUBLIC void rsdb_login_copy_in
    (from, to)
    sec_origin_t                *from;    /* [in] */
    lm_address_t                *to;      /* [out]*/
{

	if (from == NULL)
		return;
    	memset(to,0,sizeof(lm_address_t));
        switch (from->data.addrtype){
        case sec_ip_addr:
                to->addrtype=sec_ip_addr;
                to->contents.orig_ip_addr=from->data.tagged_union.o_ip_addr;
                to->length=from->length;
                break;
        case sec_dce_string:
                to->addrtype=sec_dce_string;
                memcpy((char *) from->data.tagged_union.o_string_binding,
                        (char *)to->contents.string_binding,
                        from->length);
                to->length=from->length;
                break;
        case sec_dce_twr:
                to->addrtype=sec_dce_twr;
                break;
        default:
                break;
        }
}

/*
 * rsdb_acct_replace_login_activity
 *
 *      The update of  a login activity record
 *      will require that a lookup be done on the principal name first
 *      and then the id in the acct record is used to do a secondary
 *      lookup of the account activity record.
 *      Then the data fields must be updated and the record stored
 *
 *
 *      LOCKING should occurr before this is called.
 *      	READ_INTEND_WRITE lock should have been taken on lock_login_info
 *      	and promoted to WRITE lock
 */

PUBLIC boolean32 rsdb_acct_replace_login_activity
    ( login_name, login_part, status )
    sec_rgy_login_name_t   *login_name;          /* [in, out] */
    sec_rgy_login_activity_t    *(login_part);   /* [in, out] */
    error_status_t         *(status);      /* [out] */
{
    rsdb_named_item_t      person;
    rsdb_named_item_t      group;
    rsdb_named_item_t      org;
    sec_rgy_cursor_t       cursor;
    sec_rgy_acct_key_t     name_parts;
    rsdb_acct_item_t       account;
    rsdb_pvt_id_t          account_id;
    rsdb_acct_login_activity_t    login;
    unsigned char          *p;

    CLEAR_STATUS(status);
    memset(&login,0,sizeof(rsdb_acct_login_activity_t));
    rs_util_clear_cursor(&cursor);

    if (rsdb_acct_lookup_acct_id (login_name,  &cursor,
                                  &name_parts, &person,
                                  &group, &org,
                                  &account, status)) {
	rsdb_get_login_activity_by_id(account.login_activity_id,&login);
        login.dis_time=login_part->dis_time;
        login.cur_inv_attempts=login_part->cur_inv_attempts;
        login.last_bad_time=login_part->last_bad_time;
        login.last_good_time=login_part->last_good_time;
	rsdb_login_copy_in(&(login_part->good_origin),&(login.good_origin));
	rsdb_login_copy_in(&(login_part->bad_origin),&(login.bad_origin));
        rsdb_store_login_activity_by_id(account.login_activity_id,&login);
    } else if (STATUS_OK(status)) {
        SET_STATUS(status, sec_rgy_object_not_found);
    }
}

/*
 * rsdb_acct_reset_login_activity
 *
 *      The reset of  a login activity record
 *      is caused by an admin initiating a sec_rgy_login_activity_update()
 *      and it first does a lookup on the principal name 
 *      and then the id in the acct record is used to do a secondary
 *      lookup of the account activity record.
 *      Then the number of invalid attempts, disable time and last good date
 *      are updated and the record stored.
 *
 *      The journal record is also cleared by this call.
 *      The journal record is retrieved with the same account id and
 *      the number of invalid attempts is set to zero and the last good
 *      login date is updated.
 *      LOCKING should occurr before this is called.
 *      	READ_INTEND_WRITE lock should have been taken on lock_login_info
 *      	and promoted to WRITE lock
 */

PUBLIC boolean32 rsdb_acct_reset_login_activity
    ( login_name, login_part, status )
    sec_rgy_login_name_t   *login_name;          /* [in, out] */
    sec_rgy_login_activity_t    *(login_part);   /* [in, out] */
    error_status_t         *(status);      /* [out] */
{
    rsdb_named_item_t      person;
    rsdb_named_item_t      group;
    rsdb_named_item_t      org;
    sec_rgy_cursor_t       cursor;
    sec_rgy_acct_key_t     name_parts;
    rsdb_acct_item_t       account;
    rsdb_pvt_id_t          account_id;
    rsdb_acct_login_activity_t    login;
    unsigned char          *p;

    lm_upd_action_t            action;
    rsdb_acct_journal_t         db_journal;

    memset(&db_journal,0,sizeof(rsdb_acct_journal_t));
    memset(&action,0,sizeof(lm_upd_action_t));

    CLEAR_STATUS(status);
    memset(&login,0,sizeof(rsdb_acct_login_activity_t));
    memset(&cursor,0,sizeof(sec_rgy_cursor_t));
    rs_util_clear_cursor(&cursor);

    if (rsdb_acct_lookup_acct_id (login_name,  &cursor,
                                  &name_parts, &person,
                                  &group, &org,
                                  &account, status)) {
	rsdb_get_login_activity_by_id(account.login_activity_id,&login);
        login.dis_time=login_part->dis_time;
        login.cur_inv_attempts=login_part->cur_inv_attempts;
        login.last_bad_time=login_part->last_bad_time;
        login.last_good_time=login_part->last_good_time;
        rsdb_store_login_activity_by_id(account.login_activity_id,&login);
        if (rsdb_get_journal_activity_by_id(account.login_activity_id, 
			&db_journal)){
		action.type=lm_reset;
                lm_journal_valid_attempt(&db_journal,action,status);
                if (GOOD_STATUS(status))
                    rsdb_store_journal_activity_by_id(account.login_activity_id,
			&db_journal);
        }
    } else if (STATUS_OK(status)) {
        SET_STATUS(status, sec_rgy_object_not_found);
    }
}

