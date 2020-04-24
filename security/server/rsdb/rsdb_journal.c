/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_journal.c,v $
 * Revision 1.1.7.3  1996/02/18  00:21:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:22  marty]
 *
 * Revision 1.1.7.2  1995/12/08  17:59:43  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/08/31  20:03 UTC  mullan_s
 * 	Merge fix for CHFts16183
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16183/2  1995/08/31  13:19 UTC  root
 * 	wrong pointer type for export
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16183/2  1995/08/31  13:18 UTC  mullan_s
 * 	wrong pointer type for export
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16183/1  1995/08/30  19:00 UTC  mullan_s
 * 	Fix potential corruption in rsdb journal records.
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16183/1  1995/08/30  18:56 UTC  mullan_s
 * 	Fix potential corruption in rsdb journal records.
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:12 UTC  greg
 * 	fix for improper free of data pointer.
 * 	[1995/05/09  18:53 UTC  hondo  /main/SEC_migrate_merge/2]
 * 
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	change journal records to be allocated dynamically
 * 	[1995/04/03  22:18 UTC  hondo  /main/SEC_migrate/1]
 * 
 * 	allocation of journal rec changed from static to dynamic
 * 	[1995/03/28  19:25 UTC  hondo  /main/hondo_mig1/1]
 * 
 * Revision 1.1.2.4  1994/08/29  15:51:55  hondo
 * 	OT 11919 - login activity idl change to support different address types
 * 	[1994/08/28  21:42:36  hondo]
 * 
 * Revision 1.1.2.3  1994/08/19  17:59:38  mdf
 * 	1994/08/18  sekhar
 * 	Log messages using SVC calls.
 * 	[1994/08/19  13:11:22  mdf]
 * 
 * Revision 1.1.2.2  1994/08/10  19:22:08  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:26  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  15:02:49  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  18:00:26  mdf]
 * 
 * $EndLog$
 */
/*  rsdb_journal.c 
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
#include <stdio.h>

#include <rs_util.h>
#include <rs_pvt.h>
#include <rs_log.h>
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


/*
 * Local Global Variables
 */
static rsdb_acct_journal_t    first;
static rsdb_acct_journal_t    last;

/*
 *   Journal Activity contains information local to the replica
 *   about login activity for a principal.
 *   
 *   The information tracked is kept in two "types" of records   
 *   good_login and bad_login.
 *   The login information is time of attempt, number of attempts
 *   and origin (this is a union of  orig_ip_addr(unsigned32 )
 *   string_binding(char[sec_rgy_name_max_len]) and twr_set
 *   (sec_attr_twr_set_t).
 *   The twr_set type is currently not supported.
 *   The complete model for tracking login activity is that the
 *   journal records keep local activity and periodically update 
 *   the master with the information they have collected.
 *   This is why the data structure supports a SENT_TO_MASTER flag 
 *   and a seqno. If a "good_login" record has the SENT_TO_MASTER
 *   flag set, the design was to add another "good_login" record.
 *   Since the full design was not implemented the current activity
 *   is limited to 2 records, one good and one bad.
 *   (The pertinent data structures are in security/h/rsdb_jrnl.h)
 *   
 *   As of 1.1 the update to the master has not been implemented.
 *   
 *   This code currently tracks the local activity and stores it
 *   in the journal record, but it is never sent to the master.
 *   
 *   
 */

/*
 *    initialize first and last records in memory
 */

PUBLIC boolean32 rsdb_journal_init ()
{

    if(!rsdb_get_journal_activity_by_id(START_OF_LIST, &first)) {
        fatal("Couldn't initialize journal list");
    }
    if(!rsdb_get_journal_activity_by_id(END_OF_LIST, &last)) {
        fatal("Couldn't initialize journal list");
    }
}


/*
 *    initialize first and last records in database
 */
PUBLIC void rsdb_journal_create(
#ifdef __STDC__
    void
#endif
)
{
    bzero((Pointer) &first, sizeof(first));
    rsdb_store_journal_activity_by_id(START_OF_LIST, &first);

    bzero((Pointer) &last, sizeof(last));
    rsdb_store_journal_activity_by_id(END_OF_LIST, &last);
}


/*
 *      Store the journal record whose sequential id is supplied as
 *      an input argument
 *
 *      LOCKING should occurr before this is called.
 *              READ_INTEND_WRITE lock should have been taken on lock_login_info
 *              and promoted to WRITE lock
 */


PUBLIC void rsdb_store_journal_activity_by_id
    (id, activity)
    rsdb_pvt_id_t      id;
    rsdb_acct_journal_t    *activity;
{
    rsdb_sequential_key_t  sequential_key;
    rsdb_jrnl_rec_t        *data=NULL;
    int                    data_len=0,i=0;
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
    data_len=sizeof(rsdb_jrnl_rec_t);
    for (i = 0; i<=activity->num_items ; i++){
         data_len+=(activity->member[i].origin.length);
    }

    if ( ! (data=(rsdb_jrnl_rec_t *)malloc(data_len)) ){
        SET_STATUS(&st, sec_s_no_memory);
        return;
    }
    memset((char *)data, NULL,data_len);

    /*
     *  The data coming in is in a fixed format record and must
     *  be converted to the database format to save space.
     */
    data_len=import_jrnl_to_rsdb(activity,data,&st);
    if (GOOD_STATUS(&st)) {
    	rsdb_replace(rsdb_journal_db, (Pointer) &sequential_key,
           sizeof(sequential_key), (Pointer) data, data_len, &st);
    }
    if ( data  != NULL)
	free((rsdb_jrnl_rec_t *)data);
}


PUBLIC int import_jrnl_to_rsdb
    (activity, data, st)
    rsdb_acct_journal_t   *activity;
    rsdb_jrnl_rec_t       *data;
    error_status_t        *st;     
{
    char           *addrs=NULL;
    int num,i,len;

    CLEAR_STATUS(st);
    num=i=len=0;
    /*
     * this function constructs a variable length record (data)
     * from fixed format  (activity)
     *
     * {type seqno flags count time origin(addrtype,length,address) reserved}[1]
     * {type seqno flags count time origin(addrtype,length,address) reserved}[2]
     * 
     * to
     * 
     * {type seqno flags count time 
     *    origin_addrtype origin_length origin_ip_addr}[1]
     * {type seqno flags count time 
     *    origin_addrtype origin_length origin_ip_addr}[2]
     * address[1]adress[2]....
     * 
     * so that space for DCE addresses are only allocated in the database when
     * needed
     *
     * NOTE:
     * 1) to avoid allignment issues sec_ip_addr is stored in the fixed portion 
     * 2) towers have not been implemented
     * 
     *  addrs ....calculate where the characters addresses will start 
     * "len" is used to get exact size of the record, since the 
     *  calculation for the variable buffer (done by calling function)
     *  did not include the logic for determining wheter or not
     *  the address type was a DCE or IP form.  IP forms don't need
     *  additional space allocated since they are stored in the
     *  fixed portion of the record. So "data" is longer than it need be.
     */
    data->num_items=activity->num_items;
    addrs=(((char *)data)+ (sizeof(signed32)));
    len+=(sizeof(signed32));
    for (i = 0; i <= data->num_items; i++){
        addrs+= (sizeof(db_jrnl_rec_t));
        len+=(sizeof(db_jrnl_rec_t));
    }


    /* 
     *  If num_items is greater than zero, then there is more than one
     *  entry in the journal record so loop through the entries moving
     *  data to new format
     */
    if (data->num_items >= 0 ){
        for (num=0; num <= data->num_items; num++){

        data->member[num].type=activity->member[num].type;
        data->member[num].seqno=activity->member[num].seqno;
        data->member[num].flags=activity->member[num].flags;
        data->member[num].time=activity->member[num].time;
        data->member[num].count= activity->member[num].count;

        switch (activity->member[num].origin.addrtype){
        case  sec_ip_addr:
            data->member[num].origin_addrtype=sec_ip_addr;
            data->member[num].origin_ip_addr=
		activity->member[num].origin.contents.orig_ip_addr;
	    data->member[num].origin_length=0;
            break;
        case  sec_dce_string:
            data->member[num].origin_addrtype=sec_dce_string;
            if (activity->member[num].origin.length <= sec_rgy_name_max_len){
               bcopy(
                 (char *)(activity->member[num].origin.contents.string_binding),
                 addrs, activity->member[num].origin.length);
	       data->member[num].origin_length=
                 activity->member[num].origin.length;
              /* 
               * increment the address buffer pointer
               * only when copying a DCE type address
               */
               addrs+=activity->member[num].origin.length;
               len+=activity->member[num].origin.length;
            }
	    else{
                SET_STATUS(st, sec_id_e_name_too_long);
	        data->member[num].origin_length= 0;
	    }
            break;
        default:
                data->member[num].origin_addrtype=
                    activity->member[num].origin.addrtype;
	        data->member[num].origin_length= 0;
        }
        }
    }
    return len;
}
 
/*
 *      Read the journal database and return
 *      the record whose sequential id is supplied as
 *      an input argument
 *
 *      LOCKING should occurr before this is called.
 *              READ lock should be taken on lock_login_info
 */
PUBLIC boolean32 rsdb_get_journal_activity_by_id
    (id, activity)
    rsdb_pvt_id_t      id;
    rsdb_acct_journal_t    *activity;
{
    rsdb_sequential_key_t  sequential_key;
    Pointer                data;
    int                    data_len=0;
    error_status_t         st;
    rsdb_jrnl_rec_t        *jdata;

    CLEAR_STATUS(&st);
    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = id;

    rsdb_fetch(rsdb_journal_db, (Pointer)&sequential_key, 
               sizeof(sequential_key), &data, &data_len, &st);
    if (BAD_STATUS(&st)) {
        return false;
    }
    CLEAR_STATUS(&st);
    jdata=(rsdb_jrnl_rec_t *)data;
    if (data_len >0) {
    	export_rsdb_to_jrnl(activity,jdata,&st);
    	if (BAD_STATUS(&st)) {
       	   return false;
    	}
        else
        	return true;
    }
    else
        return false;
}

PUBLIC void export_rsdb_to_jrnl
    (activity, jrnl_rec, st)
    rsdb_acct_journal_t   *activity;
    rsdb_jrnl_rec_t       *jrnl_rec;
    error_status_t               *st;
{
    char           *addrs=NULL;
    int num,i,len=0;

    CLEAR_STATUS(st);
    /*
     * this function constructs a fixed record
     * from a variable format  
     *
     * {type seqno flags count time 
     *    origin_addrtype origin_length origin_ip_addr}[1]
     * {type seqno flags count time 
     *    origin_addrtype origin_length origin_ip_addr}[2]
     * address[1]adress[2]....
     * 
     * to
     * 
     * {type seqno flags count time origin(addrtype,length,address) }[1]
     * {type seqno flags count time origin(addrtype,length,address) }[2]
     * 
     */

    /*
     * calculate where the characters addresses will start
     */
    addrs=(char *)jrnl_rec+ (sizeof(signed32));
    for (i = 0; i<=jrnl_rec->num_items; i++){
        addrs+= (sizeof(db_jrnl_rec_t));
    }

    /* 
     *  If num_items is greater than zero, then there are entries
     *  in the journal record so loop through the entries looking
     *  for a match on type (good or bad attempt).
     */
    activity->num_items = jrnl_rec->num_items;

    if (activity->num_items >= 0 ){
        for (num=0; num <= jrnl_rec->num_items; num++){

        activity->member[num].type=jrnl_rec->member[num].type;
        activity->member[num].seqno=jrnl_rec->member[num].seqno;
        activity->member[num].flags=jrnl_rec->member[num].flags;
        activity->member[num].time=jrnl_rec->member[num].time;
        activity->member[num].count=jrnl_rec->member[num].count;

        switch (jrnl_rec->member[num].origin_addrtype){
        case  sec_ip_addr:
            activity->member[num].origin.addrtype=sec_ip_addr;
	    activity->member[num].origin.contents.orig_ip_addr=
                jrnl_rec->member[num].origin_ip_addr;
	    activity->member[num].origin.length=sizeof(unsigned32);
            break;
        case  sec_dce_string:
            activity->member[num].origin.addrtype=sec_dce_string;
            if (jrnl_rec->member[num].origin_length <= sec_rgy_name_max_len){
               bcopy( (char *)(addrs),
                 (char *)(activity->member[num].origin.contents.string_binding),
                 jrnl_rec->member[num].origin_length);
               activity->member[num].origin.length=
                 jrnl_rec->member[num].origin_length;
              /* 
               * move the buffer pointer
               */
               addrs+=jrnl_rec->member[num].origin_length;
            }
	    else{
                SET_STATUS(st, sec_id_e_name_too_long);
	        activity->member[num].origin.length= 0;
	    }
            break;
        default:
                activity->member[num].origin.addrtype=
                    jrnl_rec->member[num].origin_addrtype;
	        activity->member[num].origin.length= 0;
        }
        }
    }
}


/*
 *      Purge the journal record whose sequential id is supplied as
 *      an input argument
 *
 *      LOCKING should occurr before this is called.
 *              READ_INTEND_WRITE lock should have been taken on lock_login_info
 *              and promoted to WRITE lock
 */


PUBLIC void rsdb_purge_journal_activity_by_id
    (id )
    rsdb_pvt_id_t      id;
{
    rsdb_sequential_key_t  sequential_key;
    error_status_t         st;

    CLEAR_STATUS(&st);
    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = id;

    rsdb_delete(rsdb_journal_db, (Pointer) &sequential_key,
                sizeof(sequential_key), &st);
}

/*
 *      rsdb_get_journal_entry
 *
 *	the journal record consists of multiple entries....
 *          at least one for each of types....
 *		good_login
 *		bad_login
 *	multiple entries can be present if the replica which is managing
 *      the journal has attempted to communicate with the master.
 *      such entries are marked in the flags field with SENT_TO_MASTER.
 *      LOCKING should occurr before this is called.
 *              READ_INTEND_WRITE lock should be taken on lock_login_info
 *
 */

PUBLIC void rsdb_get_journal_entry
    (rec, id, entry, st)
    rsdb_acct_journal_t    *rec;
    j_item_type_t           id;
    jrnl_rec_t              *entry;
    error_status_t          *st;
{

    int	num;
    CLEAR_STATUS(st);
	
    /*  
     *  If num_items is greater than zero, then there are entries
     *  in the journal record so loop through the entries looking
     *  for a match on type (good or bad attempt).
     */
    if (rec->num_items >= 0 ){
	for (num=0; num <= rec->num_items; num++){
            if (rec->member[num].type == id) {
		entry->time=rec->member[num].time;
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
			entry->count=rec->member[num].count;
			*st=good_record_found;
		        break;
                    case bad_login:
			entry->count+=rec->member[num].count;
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

