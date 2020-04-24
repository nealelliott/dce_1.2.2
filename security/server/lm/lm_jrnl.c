/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: lm_jrnl.c,v $
 * Revision 1.1.12.1  1996/08/09  12:08:12  arvind
 * 	OT13213 - fix call to rs_log_journal_activity_update
 * 	[1995/12/14  15:12 UTC  hinxman  /main/HPDCE02/hinxman_ot13213_10/1]
 *
 * Revision 1.1.9.3  1996/02/18  00:16:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:37  marty]
 * 
 * Revision 1.1.9.2  1995/12/08  17:46:48  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/08/31  20:03 UTC  mullan_s
 * 	Merge fix for CHFts16183
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16183/2  1995/08/31  14:25 UTC  hondo
 * 	lm_copy addr need to set to addrtype
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16183/1  1995/08/28  19:34 UTC  mullan_s
 * 	CHFts16183: lm_journal_invalid_attempts: fix array boundary
 * 	overwrite bug.
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:13 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:37 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	changes for dynamic allocation of records
 * 	[1995/04/03  22:13 UTC  hondo  /main/SEC_migrate/1]
 * 
 * 	changes to support dynamic allocation of records.
 * 	[1995/03/28  19:31 UTC  hondo  /main/hondo_mig1/1]
 * 
 * Revision 1.1.5.1  1994/10/26  20:00:43  hondo
 * 	changes for replication of login activity.
 * 	[1994/10/24  20:08:20  hondo]
 * 
 * Revision 1.1.2.3  1994/08/29  15:51:41  hondo
 * 	OT 11919 - login activity idl change to support different address types
 * 	[1994/08/28  21:43:37  hondo]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:57  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:14  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  15:00:41  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  18:00:02  mdf]
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
/*
 *      Journal  - primitives
 *
 */


#include <sys/types.h>
#include <sys/file.h>

#include <dce/rgynbase.h>
#include <dce/rs_login_base.h>
#include <stdio.h>

#include <rs_util.h>
#include <rs_pvt.h>
#include <rs_log.h>
#include <rs_lock.h>
#include <rsdb_sysdep.h>
#include <rsdb_pvt.h>
#include <rsdb_jrnl.h>
#include <rsdb_login.h>
#include <rsdb_log.h>
#include <rsdb_file.h>
#include <lm_login.h>

#include <un_io.h>
#include <bstring.h>

#define FILE_CREATION_PROTECTION 0600
#define isflagset(flagfield, flag) (flagfield & (flag))
#define setflag(flagfield, flag) (flagfield |= (flag))

void lm_log_journal_activity_update();
extern void rs_log_journal_activity_update();

PUBLIC void lm_copy_addr_to_addr
    ( from, to)
    lm_address_t                  *from;    /* [in] */
    lm_address_t  		  *to;         	/* [in,out]*/
{       
	switch (from->addrtype){
        case sec_ip_addr:
		to->addrtype=from->addrtype;
		to->contents.orig_ip_addr=from->contents.orig_ip_addr;
		to->length=from->length;
                break;
        case sec_dce_string:
		to->addrtype=from->addrtype;
                memcpy( (char *)to->contents.string_binding,
			(char *) from->contents.string_binding,
                        from->length);
		to->length=from->length;
                break;
        case sec_dce_twr:
                break;
        default:
                break;
        }
}

PUBLIC void lm_journal_get_entry
    (rec, id, entry, st)
    rsdb_acct_journal_t    *rec;
    j_item_type_t           id;
    jrnl_rec_t              *entry;
    error_status_t          *st;
{

    int	num;
	
    /*  
     *  If num_items is greater than zero, then there are entries
     *  in the journal record so loop through the entries looking
     *  for a match on type (good or bad attempt).
     */
    CLEAR_STATUS(st);
    if (rec->num_items >= 0 ){
	for (num=0; num <= rec->num_items; num++){
            if (REC[num].type == id) {
		entry->time=REC[num].time;
		entry->type=REC[num].type;
		entry->origin.addrtype=REC[num].origin.addrtype;
		lm_copy_addr_to_addr(&(REC[num]).origin,&(entry->origin));
		/* 
	         * When searching for good login records you always
		 * want the last time and origin and the count is set to 0
		 * (Because new records are added in increasing sequence, the
		 *  last one you find is the most recent of records that have
		 *  not been sent to the master).
		 * So each subsequent read overlays the previous....hence the
		 *  need for the memset of the origin name.
	         *
		 * For bad login records, you want the last time and origin
		 *  but the count is a summary of all the bad attempts thus
		 *  far.
		 * 
		 * There could be several entries, one or more of which have
	         *  been marked as SENT_TO_MASTER.
		 * These are added into the count until they are deleted from
		 *  the journal db....which happens when the master updates
		 *  the replicas (TBD). Sequence numbers are used to track
		 *  those records that have already been posted.
		 *  
		 * Until it is posted, an existing entry is updated by 
		 * rsdb_journal_invalid_attempt or rsdb_journal_valid_attempt.
		 *
		 */
	        switch (id){
	            case good_login:
			entry->count=REC[num].count;
			*st=good_record_found;
		        break;
                    case bad_login:
			entry->count+=REC[num].count;
			*st=bad_record_found;
			break;
               	    default:
			*st=unknown_record_found;
			break;
		}
	     }
	     continue;
        }
    }
}

PUBLIC void lm_journal_invalid_attempt
    (rec, action, st)
    rsdb_acct_journal_t    *rec;	    /* [in]  */
    lm_upd_action_t           *action;         /* [in]  */
    error_status_t          *st;	    /* [out]  */
{
	int	num;

        CLEAR_STATUS(st);
        if (rec->num_items >= 0 ){
	    for (num=0; num <= rec->num_items; num++)
                if (REC[num].type == bad_login) {
		    if (REC[num].flags!= 0) 
			   continue;
		    else{
			rs_util_apply_timestamp(&REC[num].time);
        		REC[num].count++;
	 		REC[num].origin.addrtype= ACTION.addrtype;
			lm_copy_addr_to_addr(&(ACTION),&(REC[num].origin));
			return;
		    }
		}
	}
        if (rec->num_items < MAX_NUM_JRNL_ENTRIES-1) {
		rec->num_items++;
		REC[rec->num_items].type=bad_login;
		REC[rec->num_items].count=1;
		REC[rec->num_items].seqno=rec->num_items;
		REC[rec->num_items].flags=0;
		rs_util_apply_timestamp(&REC[rec->num_items].time);
		REC[rec->num_items].origin.addrtype= ACTION.addrtype;
		lm_copy_addr_to_addr(&(ACTION),&(REC[rec->num_items].origin));
	}
}

PUBLIC void lm_journal_valid_attempt
    (rec, action, st)
    rsdb_acct_journal_t    *rec;	    /* [in]  */
    lm_upd_action_t           *action;         /* [in]  */
    error_status_t          *st;	    /* [out]  */
{
	int 	updated_existing_entry=0;
	int	num;
 
        CLEAR_STATUS(st);

        if (rec->num_items >= 0 ){
	    for (num=0; num <= rec->num_items; num++)
                if ((REC[num].type == bad_login) || 
		   (action->type==lm_reset)){
		    if (REC[num].flags == 0){
        		   REC[num].count=0;
		    }
	        }		
		else{
		    if (action->type==lm_valid_attempt){
		    	if (REC[num].flags == 0){
		       	    rs_util_apply_timestamp(&REC[num].time);
			    REC[rec->num_items].origin.addrtype= ACTION.addrtype;
			    lm_copy_addr_to_addr(&(ACTION),&(REC[num].origin));
		       	    updated_existing_entry=1;
		    	}
		    } 
		}
	}
	if ((updated_existing_entry) || (action->type ==lm_reset))
		return;
        if (rec->num_items < MAX_NUM_JRNL_ENTRIES-1) {
		rec->num_items++;
		REC[rec->num_items].type=good_login;
		REC[rec->num_items].seqno=rec->num_items;
		REC[rec->num_items].flags=0;
		rs_util_apply_timestamp(&REC[rec->num_items].time);
		REC[rec->num_items].origin.addrtype= ACTION.addrtype;
		lm_copy_addr_to_addr(&(ACTION),&(REC[rec->num_items].origin));
	}
}

PUBLIC void lm_log_journal_activity_update
    (id, journal, st)
    rsdb_pvt_id_t               id;            /* [in]*/
    rsdb_acct_journal_t         *journal;      /* [in]*/
    error_status_t              *st;
{
    sec_timeval_sec_t           now;
    rs_replica_master_info_t    *master_info_p = NULL;
    uuid_t                      client;
    rsdb_jrnl_rec_t             *data=NULL;
    int                         data_len=0,i=0;

    CLEAR_STATUS(st);
    rs_util_apply_timestamp(&now);

    for (i = 0; i<=journal->num_items ; i++){
         data_len+=sizeof(rsdb_jrnl_rec_t);
         data_len+=(journal->member[i].origin.length);
    }

    if ( ! (data=malloc(data_len)) ){
        SET_STATUS(st, sec_s_no_memory);
        return;
    }
    memset((char *)data, NULL,data_len);
    data_len=import_jrnl_to_rsdb(journal,data,st);
    uuid_create_nil(&client, st);
    rs_log_journal_activity_update(id, data, data_len, &now,
        master_info_p, &client, st);

    if (data != NULL)
    	free(data);
}

