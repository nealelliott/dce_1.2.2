/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: lm_login.c,v $
 * Revision 1.1.15.1  1996/07/08  18:37:49  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:52 UTC  psn  /main/DCE122_PK/1]
 *
 * 	add prototypes for journal-manipulation functions here.
 * 	change definition of lm_copy_login to match its use.
 * 	[1996/05/08  19:42 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/2]
 *
 * 	Cleanup; add prototype for princ_to_login_name() and change its
 * 	interface.
 * 	[1996/04/26  22:50 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/1]
 *
 * Revision 1.1.13.3  1996/02/18  00:16:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:38  marty]
 * 
 * Revision 1.1.13.2  1995/12/08  17:46:52  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:14 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:37 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	changes for dynamic allocation of records
 * 	[1995/04/03  22:13 UTC  hondo  /main/SEC_migrate/1]
 * 
 * 	changes to support dynamic allocation of records.
 * 	[1995/03/28  19:31 UTC  hondo  /main/hondo_mig1/1]
 * 
 * Revision 1.1.9.1  1994/10/26  20:00:45  hondo
 * 	changes for replication of login activity.
 * 	[1994/10/24  20:08:36  hondo]
 * 
 * Revision 1.1.2.5  1994/09/13  18:53:31  hondo
 * 	memory leak fixes
 * 	[1994/09/13  18:53:12  hondo]
 * 
 * Revision 1.1.2.4  1994/09/12  21:16:02  hondo
 * 	fix for memory allocation....should be rpc_ss_allocate instead of malloc as per CR10924
 * 	[1994/09/12  21:14:55  hondo]
 * 
 * Revision 1.1.2.3  1994/08/29  15:51:43  hondo
 * 	OT 11919 - login activity idl change to support different address types
 * 	[1994/08/28  21:44:09  hondo]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:58  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:15  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  15:00:43  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  18:00:05  mdf]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server -  Login activity management
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
#include <sec_encode.h>
#include <rs_base.h>
#include <rs_util.h>
#include <rs_mkey.h>
#include <rs_lock.h>

#include <lm_login.h>

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
#include <rs_ch_lock.h>
#include <sec_attr_tools.h>


#include <bstring.h>
#include <sec_krb_util.h>

/* Kerberos header files */
#include <krb5/kdb.h>
#include <krb5/des_int.h>
#include <krb5/func-proto.h>

#define setflag(flagfield, flag) (flagfield |= (flag))

boolean32 lm_calculate_disable_time();
void lm_consolidate_data();
void lm_disable_acct();
void lm_reset_acct();
void lm_log_invalid_attempt();
void lm_log_valid_attempt();
void lm_get_origin_from_addr();
void lm_log_login_activity_update();
extern void lm_log_journal_activity_update();

extern krb5_error_code princ_to_login_name
  (
    krb5_principal          principal,
    sec_rgy_login_name_t    *login_name
  );

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
);


/*
 *  The origin for a login is derived from the address passed in
 *  on the AS_REQ.
 */
PUBLIC void lm_get_origin_from_addr(action,from)
    lm_upd_action_t 	   *action;	    /* [in,out]  */
    krb5_address          *from;           /* [in] */
{
	char 	unk_string[]={"unknown"};
        int     unk_strlen=7;

	if (from == NULL)
		return;
        switch (from->addrtype){
        case  ADDRTYPE_INET:
	    ACTION.addrtype=sec_ip_addr;
    	    ACTION.contents.orig_ip_addr=from->contents; 
	    ACTION.length=from->length;
	    break;
        case  ADDRTYPE_DCE:
	    ACTION.addrtype=sec_dce_string;
	    if (from->length <= sec_rgy_name_max_len){
    	 	memcpy(ACTION.contents.string_binding,from->contents, 
			from->length);
	        ACTION.length=from->length;
	    }
	    break;
	case ADDRTYPE_CHAOS:
	case ADDRTYPE_XNS :
	case ADDRTYPE_ISO:
	case ADDRTYPE_DDP:
	case ADDRTYPE_UNSPECIFIED:
	default:
	        ACTION.addrtype=from->addrtype;
        	strncpy((char *)(ACTION.contents.string_binding),
			(char *)&unk_string,unk_strlen);
	        ACTION.length=unk_strlen;
    }
}


/*
 *  In the current design, login information is stored in two
 *  databases per server. The journal database contains the information
 *  which is local to the master or replica. The login_activity database
 *  is the replicated database reflecting the current state of the master.
 *
 *  To return the correct information regarding login attempts, the
 *  following calls will provide a higher level interface and consolidate
 *  the values retrieved from both databases.
 *
 *  The journal record is the record for this principal in
 *  the journal database, keeping the local counts.
 *  the record is composed of entries, of the format:
 *  typedef struct rsdb_acct_journal_t{
 *            signed32            num_items;
 *            jrnl_rec_t          member[MAX_MEMBER_JRNL_ENTRIES];
 *            } rsdb_acct_journal_t;
 *
 *  MAX_MEMBER_JRNL_ENTRIES is currently 2
 *  (since updates are not propagated yet)
 *
 *        typedef struct {
 *            unsigned32              seqno;
 *            j_item_type_t           type;
 *            unsigned32              count;
 *            sec_timeval_sec_t       time;
 *            unsigned32              origin_len;
 *            sec_rgy_name_t          origin;
 *            uuid_t                  pad;
 *        } jrnl_rec_t;
 * 
 *  The login_activity  part is the single record in the replicated
 *  database containing the sum of the information as it is known to
 *  the master.
 */
PUBLIC void lm_get_login_info
    ( login_name, cursor, login_part,  status )
    sec_rgy_login_name_t   *login_name;     /* [in, out] */
    sec_rgy_cursor_t       *(cursor);       /* [in, out] */
    sec_rgy_login_activity_t    *(login_part);   /* [out] */
    error_status_t         *(status);            /* [out] */
{
    rsdb_named_item_t      person;
    rsdb_named_item_t      group;
    rsdb_named_item_t      org;
    sec_rgy_acct_key_t     name_parts;
    rsdb_acct_item_t       account;
    rsdb_pvt_id_t          account_id;
    unsigned char          *p;

    rsdb_acct_login_activity_t    login_activity;
    rsdb_acct_journal_t journal;

    memset(&login_activity,0,sizeof(rsdb_acct_login_activity_t));
    memset(&journal,0,sizeof(rsdb_acct_journal_t));

    CLEAR_STATUS(status);
    rs_util_clear_cursor(cursor);

    if (rsdb_acct_lookup_acct_id (login_name,  cursor,
                                  &name_parts, &person,
                                  &group, &org,
                                  &account, status)) {

          READ_LOCK(lock_login_info) 
    /*
     * no return code is checked, because if there is no record
     * in the database, the record passed in is used.
     * The record passed in has been initialized to 0.
     */
		rsdb_get_login_activity_by_id(account.login_activity_id,
			&login_activity);
		rsdb_get_journal_activity_by_id(account.login_activity_id,
			&journal);
          END_READ_LOCK
        	lm_consolidate_data(login_part,&journal,&login_activity);

    } else if (STATUS_OK(status)) {
        SET_STATUS(status, sec_rgy_object_not_found);
    }
}

PUBLIC void lm_update_login_info
    (searchfor, action, status)
    krb5_principal 	   searchfor;       /* [in]  */
    lm_upd_action_t 	   *action;	    /* [in]  */
    error_status_t         *(status);       /* [out] */
{
    rsdb_named_item_t      person;
    rsdb_named_item_t      group;
    rsdb_named_item_t      org;
    sec_rgy_cursor_t       cursor;
    sec_rgy_acct_key_t     name_parts;
    rsdb_acct_item_t       account;
    rsdb_pvt_id_t          account_id;
    unsigned char          *p;
    jrnl_rec_t              last_entry;
    sec_rgy_login_name_t   login_name;     
    unsigned32		   flags;
    error_status_t         st;
    boolean32               master;
    boolean32               rc;


    CLEAR_STATUS(status);
    CLEAR_STATUS(&st);
    memset(&login_name,0,sizeof(sec_rgy_login_name_t));

    *status = princ_to_login_name(searchfor, &login_name);
    if (BAD_STATUS(status)) 
        return;

    if (login_name.pname[0] == '\0')
    {
        return;
    }

    master = rs_state_master();

    rs_util_clear_cursor(&cursor);
    if (rsdb_acct_lookup_acct_id (&login_name,  &cursor,
                                  &name_parts, &person,
                                  &group, &org,
                                  &account, status)) {
        WRITE_LOCK(lock_login_info) 
        switch (action->type){
	case lm_disable:
	    lm_disable_acct(account.login_activity_id,&login_name,&st);
	case lm_invalid_attempt:
	    lm_log_invalid_attempt(account.login_activity_id,action,&st);
	    break;
	case lm_valid_attempt:
	    lm_log_valid_attempt(account.login_activity_id,action,&st);
	    break;
        case lm_reset:
	    lm_reset_acct(account.login_activity_id,&login_name,
		action,&st);
	    break;
	default:
	    break;
        }
	END_WRITE_LOCK
        if (BAD_STATUS(&st)) 
        	COPY_STATUS(&st,status);
    } else if (STATUS_OK(status)) {
        SET_STATUS(status, sec_rgy_object_not_found);
	return ;
    }
    return;
}

PUBLIC void lm_disable_acct(id,login_name,st)
    rsdb_pvt_id_t          id;
    sec_rgy_login_name_t   *login_name;     
    error_status_t         *st;
{
    rsdb_acct_login_activity_t db_login;

    CLEAR_STATUS(st);
    memset(&db_login,0,sizeof(rsdb_acct_login_activity_t));

    rsdb_get_login_activity_by_id(id, &db_login);
    db_login.dis_time=lm_calculate_disable_time(login_name,st);
    if (GOOD_STATUS(st)){
        rsdb_store_login_activity_by_id(id, &db_login);
        lm_log_login_activity_update(login_name, &db_login, st);
    }
    return;
} 

PUBLIC void lm_reset_acct(id,login_name,action,st)
    rsdb_pvt_id_t          id;
    sec_rgy_login_name_t   *login_name;     
    lm_upd_action_t 	   *action;	    
    error_status_t         *st;
{
    rsdb_acct_login_activity_t  db_login;
    rsdb_acct_journal_t 	db_journal;	


    CLEAR_STATUS(st);
    memset(&db_login,0,sizeof(rsdb_acct_login_activity_t));
    memset(&db_journal,0,sizeof(rsdb_acct_journal_t));

    rsdb_get_login_activity_by_id(id, &db_login);
    db_login.dis_time=0;
    rsdb_store_login_activity_by_id(id, &db_login);
    lm_log_login_activity_update(login_name, &db_login, st);
    rsdb_get_journal_activity_by_id(id, &db_journal);
    lm_journal_valid_attempt(&db_journal,action,st);
    if (GOOD_STATUS(st)){
       rsdb_store_journal_activity_by_id(id, &db_journal);
       lm_log_journal_activity_update(id, &db_journal, st);
    }
    return;
} 

PUBLIC void lm_log_invalid_attempt(id,action,st)
    rsdb_pvt_id_t          id;
    lm_upd_action_t 	   *action;	    
    error_status_t         *st;
{
    rsdb_acct_journal_t 	db_journal;	

    CLEAR_STATUS(st);
    memset(&db_journal,0,sizeof(rsdb_acct_journal_t));

    rsdb_get_journal_activity_by_id(id, &db_journal);
    lm_journal_invalid_attempt(&db_journal,action,st);
    if GOOD_STATUS(st){
       	rsdb_store_journal_activity_by_id(id, &db_journal);
    	lm_log_journal_activity_update(id, &db_journal, st);
    }
    return;
}

PUBLIC void lm_log_valid_attempt(id,action,st)
    rsdb_pvt_id_t          id;
    lm_upd_action_t 	   *action;	    
    error_status_t         *st;
{
    rsdb_acct_journal_t 	db_journal;	

    CLEAR_STATUS(st);
    memset(&db_journal,0,sizeof(rsdb_acct_journal_t));

    rsdb_get_journal_activity_by_id(id, &db_journal);
    lm_journal_valid_attempt(&db_journal,action,st);
    if GOOD_STATUS(st){
        rsdb_store_journal_activity_by_id(id, &db_journal);
    	lm_log_journal_activity_update(id, &db_journal, st);
    }
    return;
}

PUBLIC boolean32 lm_calculate_disable_time (login_name, status)
    sec_rgy_login_name_t   *login_name;
    error_status_t          *status;
{
    sec_attr_t              sec_attr;          
    sec_rgy_name_t          fullname;         
    uuid_t                  obj_uuid;       
    rsdb_util_rgy_obj_info_t obj_info;       
    sec_timeval_sec_t       curtime;
    sec_timeval_sec_t       time;
    sec_timeval_sec_t       disable_interval;

    memset(&sec_attr,0,sizeof(sec_attr_t));
    CLEAR_STATUS(status);

    rsdb_name_util_complete_name(sec_rgy_domain_person, login_name,
		fullname,status);
    if (BAD_STATUS(status))
        return 0;

    rsdb_util_get_obj_info_by_name(fullname, rsdb_pgo_type,sec_acl_type_object, 
	&obj_info, &obj_uuid, status);
    if (BAD_STATUS(status))
        return 0;

    rsdb_attr_lookup_by_name(&obj_info, "disable_time_interval", 
	NULL, &sec_attr, status);

    if (GOOD_STATUS(status)) {
        rs_util_apply_timestamp(&curtime);
        if (disable_interval=SA_ATTR_INTEGER(&sec_attr))
		time=curtime+disable_interval;
	return time;
                
    }
    return 0;
}

PUBLIC void lm_copy_login (from, to)
    lm_address_t  		*from;  /* [in]*/
    sec_origin_t    		*to;    /* [out] */
{
	if (from ==NULL)
		return;
	memset((char*)to,0,sizeof(sec_origin_t));
	switch (from->addrtype){
	case sec_ip_addr:
		to->data.addrtype=sec_ip_addr;
		to->data.tagged_union.o_ip_addr=from->contents.orig_ip_addr;
		to->length=from->length;
		break;
	case sec_dce_string:
		to->data.addrtype=sec_dce_string;
		if (to->data.tagged_union.o_string_binding == NULL){
		    if ((to->data.tagged_union.o_string_binding=
			 rpc_ss_allocate(from->length)) == NULL){
			        return;
		    }
		}
		memcpy((char *)to->data.tagged_union.o_string_binding,
			(char *) from->contents.string_binding, 
			from->length);
		to->length=from->length;
		break;
	case sec_dce_twr:
		to->data.addrtype=sec_dce_twr;
		break;
	default:
		break;
	}
}


PUBLIC void lm_copy_journal
    ( from, to)
    jrnl_rec_t 		*from;	 	/* [in]*/
    sec_origin_t    	*to;            /* [in,out] */
{
	if (from ==NULL)
		return;
	memset((char*)to,0,sizeof(sec_origin_t));
	switch (from->origin.addrtype){
	case sec_ip_addr:
		to->data.addrtype=sec_ip_addr;
		to->data.tagged_union.o_ip_addr=
			from->origin.contents.orig_ip_addr; 
		to->length=from->origin.length;
		break;
	case sec_dce_string:
		to->data.addrtype=sec_dce_string;
		if (to->data.tagged_union.o_string_binding == NULL){
			to->data.tagged_union.o_string_binding=
				malloc(from->origin.length);
		}
		memcpy((char *)to->data.tagged_union.o_string_binding,
			(char *) from->origin.contents.string_binding, 
			from->origin.length);
		break;
	case sec_dce_twr:
		break;
	default:
		break;
	}
}

PUBLIC void lm_consolidate_data
    ( new_data, journal, login_activity)
    sec_rgy_login_activity_t    *new_data;       /* [in,out] */
    rsdb_acct_journal_t 	*journal;	 /* [in,out]*/
    rsdb_acct_login_activity_t  *login_activity; /* [in,out]*/
{
	jrnl_rec_t	good_entry;
	jrnl_rec_t	bad_entry;
	error_status_t  st;

	memset(new_data,0,sizeof(sec_rgy_login_activity_t));
	memset(&good_entry,0,sizeof(jrnl_rec_t));
	memset(&bad_entry,0,sizeof(jrnl_rec_t));
	CLEAR_STATUS(&st);
	/* 
	 *  The new_data is a clean record allocated by the calling function
	 *  and filled with the relevant data here.
	 *  The disable time is kept in the replicated login activity
	 *      database only
	 */

        new_data->dis_time=login_activity->dis_time;

	/* 
	 *  "good login" data could be most recent in either the login_activity
	 *      database or in the local journal database....so get the local
	 *      record and compare based on date.
	 *  Return the most recent "good login" data.
	 */
	rsdb_get_journal_entry(journal,good_login,&good_entry,&st);
        if (GOOD_RECORD_FOUND(st)){
	    if (good_entry.time > LOGIN->last_good_time){
		new_data->last_good_time=good_entry.time;
		/*
                 * the origin data is not copied since it is
                 * not used for decisions on login
		 * lm_copy_journal(&good_entry,&NEW_GOOD);
		 */
	    }
	    else{
		new_data->last_good_time=LOGIN->last_good_time;
		/*
                 * the origin data is not copied since it is
                 * not used for decisions on login
		 * lm_copy_login(&LOGIN_GOOD,&NEW_GOOD);
		 */
	    }
	}
	else{
	    new_data->last_good_time=LOGIN->last_good_time;
	    /*
             * the origin data is not copied since it is
             * not used for decisions on login
	     * lm_copy_login(&LOGIN_GOOD,&NEW_GOOD);
	     */
	}
		

	/* 
	 *  "bad login" data could be most recent in either the login_activity
	 *      database or in the local journal database....so get the local
	 *      record and compare based on date.
	 *  Return the most recent "bad login" data.
	 */
	rsdb_get_journal_entry(journal,bad_login,&bad_entry,&st);
        if (BAD_RECORD_FOUND(st)){
	    if (bad_entry.time >LOGIN->last_bad_time){
			new_data->last_bad_time=bad_entry.time;
			/*
                         * the origin data is not copied since it is
                         * not used for decisions on login
		 	 * lm_copy_journal(&bad_entry,&NEW_BAD);
		 	 */
	    }
	    else{
		new_data->last_bad_time=LOGIN->last_bad_time;
	        /*
                 * the origin data is not copied since it is
                 * not used for decisions on login
		 * lm_copy_login(&LOGIN_BAD,&NEW_BAD);
		 */
	    }
	    /* 
	     *  the current attempts must be calculated from both sources
	     *  if there is data in journal database
	     */
            new_data->cur_inv_attempts=(bad_entry.count + 
				LOGIN->cur_inv_attempts);
	}
	else{
	    new_data->last_bad_time=LOGIN->last_bad_time;
	    /*
             * the origin data is not copied since it is
             * not used for decisions on login
  	     * lm_copy_login(&LOGIN_BAD,&NEW_BAD);
	     */
            new_data->cur_inv_attempts=LOGIN->cur_inv_attempts;
	}
}

PUBLIC void lm_log_login_activity_update
    (login_name, login, st)
    sec_rgy_login_name_t        *login_name;            /* [in]*/
    rsdb_acct_login_activity_t  *login;                 /* [in]*/
    error_status_t              *st;
{
    sec_timeval_sec_t           now;
    rs_replica_master_info_t    *master_info_p = NULL;
    uuid_t                      *client=NULL;
    sec_rgy_login_activity_t    login_part;       

    CLEAR_STATUS(st);
    memset(&login_part,0,sizeof(sec_rgy_login_activity_t));
    rs_util_apply_timestamp(&now);
    login_part.dis_time=login->dis_time;
    login_part.cur_inv_attempts=login->cur_inv_attempts;
    login_part.last_bad_time=login->last_bad_time;
    login_part.last_good_time=login->last_good_time;
    /* 
     * the origin data is not copied since it is
     * not updated in the login activity records
     * (when propagation of journal records is implemented this will change)
     * lm_copy_login(&(login->good_origin),&(login_part.good_origin));
     * lm_copy_login(&(login->bad_origin),&(login_part.bad_origin));
     */
    rs_log_login_activity_update(login_name, &login_part, &now, 
	master_info_p, &client, st);
}

