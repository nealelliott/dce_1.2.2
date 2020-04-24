/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_do_updates.c,v $
 * Revision 1.1.8.2  1996/02/18  19:35:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:35  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:28:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:13  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:03:27  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:18:14  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:42:45  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:00  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:09:43  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:37:28  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  14:18:39  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:18:49  zeliff]
 * 
 * Revision 1.1.4.2  1992/11/13  15:39:04  peckham
 * 	OT#5047: We now agree that the fix in gen 36 (OT#4763) for the
 * 	childPointer update bug was bogus. Dave's checks to keep a childPointer
 * 	from being superceeded were too ambitious, and kept an old deleted
 * 	childPointer from being replaced by a new one.
 * 	[1992/11/13  15:38:12  peckham]
 * 
 * Revision 1.1.2.5  1992/08/05  15:42:05  peckham
 * 	OT#4871: This bug popped up with the do_update() fix for OT#4763.
 * 	A read_for_update() is done which locks an entry, after which it
 * 	is discovered that the entry is absent and control is passed to
 * 	check_exists() which tries to read_for_update() again and hangs.
 * 	The fix is to release the entry after it is discovered to be absent.
 * 	[1992/08/05  14:13:39  peckham]
 * 
 * Revision 1.1.2.4  1992/07/22  19:57:46  peckham
 * 	OT#4763: acl_edit problems. This one was subtle.
 * 	A ReadAttribute() of a directory was going to a readOnly replica
 * 	of the parent and failing with UnknownEntry. This was because
 * 	the childPointer was marked absent from a previous delete of
 * 	the directory, and thus could not provide a referral to the
 * 	master clearinghouse.
 * 	The bug is that an incoming do_updates finds the entry and
 * 	presumes it is present instead of checking. Later code drops
 * 	the do_update instead of applying it.
 * 	Fix is to check after the entry is found and reset status
 * 	to UnknownEntry if absent.
 * 	[1992/07/22  19:24:15  peckham]
 * 
 * Revision 1.1.2.3  1992/06/30  20:13:42  peckham
 * 	 OT#4485: Fixes for transmogrified names (e.g., a name goes from an object
 * 	 entry to a softlink and back), memory leaks, dangling locks, and a few
 * 	 other things.  do_updates was also instrumented.   This fix requires
 * 	 the db_btree fix which takes care of corruptions caused by
 * 	 db_btree_remove_entry().
 * 	[1992/06/30  20:13:08  peckham]
 * 
 * Revision 1.1.2.2  1992/03/22  22:26:22  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:33:50  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module back_do_updates.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 *
 * Module Description:
 *
 * Functions in this module perform the architectural do_update
 * module.  It applies update packet messages sent during spread
 * or propagate.
 *
 */
#include <server.h>
#include <dbdef.h>
#include <dnsnormal.h>
#include <events.h>
#include <find.h>
#include <names.h>
#include <security.h>
#include <sets.h>
#include <tlog.h>
#include <uid.h>
#include <back.h>

/*
 * Local prototypes
 */
static void
print_update_bug  (
    Timestamp_t         *,
    Update_t            *,
    db_data_t           *);

static dns_status_t
apply_update_packet (
    db_stream_t         *,
    db_data_t           *,
    Timestamp_t         *,
    Update_t            *,
    int                 ,
    FullName_t          *,
    int                 *);

static dns_status_t
create_entry (
    db_data_t           *,
    Timestamp_t         *);


/*
 * print_update_bug
 *
 * Input:
 *      allupto_p       pointer to allupto
 *      update_p        pointer to update
 *      rec_desc_p      pointer to data descriptor
 *
 */
static void 
print_update_bug (Timestamp_t  *allupto_p,
                  Update_t     *update_p,
                  db_data_t    *rec_p)
{
    byte_t              debug_buf[DNS_STR_FNAME_MAX];
    byte_t              dotty[DNS_STR_FNAME_MAX];
    char                alluptostr[100];
    char                updatestr[100];
    AttributeSpecifier_t *as_p =(AttributeSpecifier_t *)update_p->ud_attribute;
    int                 len,dottylen;

    len = sizeof(debug_buf);
    debug_buf[0] = 0;
    dottylen = sizeof(dotty);
    dotty[0] = 0;
    (void)dnsCvtObjIDToStr((dns_attr_name_t *)as_p->as_name,
                     (byte_t *)0, debug_buf, &len, dotty, &dottylen);
    if (strlen((char *)debug_buf) == 0) 
    {
	(void)strcpy((char *)debug_buf, (char *)dotty);
    }


    ts_to_string(allupto_p, (char *)alluptostr, sizeof(alluptostr));

    ts_to_string((Timestamp_t *)update_p->ud_timestamp, 
		 (char *)updatestr,  sizeof(updatestr));
    
    dce_svc_printf(CDS_S_SERVER_TIMESTAMP_MSG, debug_buf, 
		   alluptostr, updatestr);

}


/* -------------------------------------------------------------------------- */

/*
 * apply_update_packet
 *      This function applys a packet of updates to an entry
 *
 * Inputs:
 *      stream_p    = pointer to db stream
 *      rec_p       = pointer to the data description for the entry
 *      allupto_p   = pointer to directories allupto
 *      updateset_p = pointer to update packets
 *      updateCount = number of updates in updateset
 *      name_p      = full name
 *      delete_entry_packet_p = pointer to status containing flag for
 *                              delete entry packets (optional).
 */
static dns_status_t 
apply_update_packet (db_stream_t  *stream_p,
                     db_data_t    *rec_p,
                     Timestamp_t  *allupto_p,
                     Update_t     *updateset_p,
                     int          updateCount,
                     FullName_t   *name_p,
                     int          *delete_entry_packet_p)
{
    Update_t       *update_p = updateset_p;
    DBSet_t        *main_set_p;
    int             i;
    dns_status_t    status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
	" >apply_update_packet"));
    
    if (delete_entry_packet_p) 
    {
        *delete_entry_packet_p = FALSE;
    }

    /* loop thru the update packet and apply each update */
    for (i = 0; i < updateCount; i++) {
        AttributeSpecifier_t *as_p =
                        (AttributeSpecifier_t *)update_p->ud_attribute;

        /*
         * Check for old updates (AllUpTo is older than the update's 
         * timestamp).  Leave a message around in case something went
         * wrong.
         */

        if (dnsCmpCTS((dns_cts *)allupto_p,
                      (dns_cts *)update_p->ud_timestamp) > 0) 
	{
            print_update_bug(allupto_p, update_p, rec_p);
            return (DNS_NAMESERVERBUG);
        }

        /*
         * Now check for too new of an update...
         */

        if (!ts_future_skew((Timestamp_t *)update_p->ud_timestamp)) 
	{
            print_update_bug(allupto_p, update_p, rec_p);
            return (DNS_BADCLOCK);
        }

        /* Check for delete entry */
        if (EXT8(as_p->as_type) == AT_null) {
            main_set_p = (DBSet_t *)rec_p->data_p->dbr_data;
            ALIGN_CHECK(main_set_p);
            CLEAR_FLAG(main_set_p->ds_flag, DS_present);
            COPY_Timestamp(update_p->ud_timestamp, main_set_p->ds_ts);
            if (delete_entry_packet_p) {
                *delete_entry_packet_p = TRUE;
            }
        } else {
            AttributeSpecifier_t *const as_p =
                (AttributeSpecifier_t *)update_p->ud_attribute;
            AtomicValue_t *const av_p =
                (AtomicValue_t *)SKIP_BOOLEAN(SKIP_AttributeSpecifier(as_p));
            ParentPointer_t *const pp_p =
                (ParentPointer_t *)av_p->av_value;
            attribute_descriptor_t *const dns_p =
                find_known_attribute((AttributeName_t *)as_p->as_name);

            status = set_insert(rec_p, update_p, dns_p, TRUE);
            if (status != DNS_SUCCESS) {
                return(status);
            }

            /*
             * If directory name changes must update hash table.  This happens
             * when the directory name was linked.
             */
            if ((EXT8(update_p->ud_operation) == UD_present) &&
                    (dns_p != (attribute_descriptor_t *)0) &&
                    (rec_p->data_type == ET_directory) &&
                    (dns_p->atype == DNS_PARENTPOINTER) &&
                    (!names_fullmatch(
                        (FullName_t *)stream_p->db_dir_hash_p->dir_name,
                        (FullName_t *)pp_p->pp_myName))) {
                db_hash_new_name(stream_p->db_file_p, stream_p->db_dir_hash_p,
                    (FullName_t *)pp_p->pp_myName);
            }
        }

        /*
         * Get next update
         */

        update_p = (Update_t *)SKIP_Update(update_p);

    }

    return(DNS_SUCCESS);
}


/* -------------------------------------------------------------------------- */

/*
 * create_entry
 *      This routine alloctates space for a new entry and inserts the CTS.
 *
 * Inputs:
 *      rec_p = pointer to data descriptor
 *      cts_p = pointer to entry cts
 *
 */
static dns_status_t 
create_entry (db_data_t    *rec_p,
              Timestamp_t  *cts_p)
{
    struct entrylist elist[1];

    rec_p->data_buf_len = 5000;
    rec_p->data_p = (data_record_t *)dns_malloc(rec_p->data_buf_len);
    if (!rec_p->data_p) return(DNS_NONSRESOURCES);

    elist[0].el_type = DNS_CTS;
    elist[0].el_len = sizeof(*cts_p);
    elist[0].el_value_p = (unsigned char *)cts_p;

    return(create_record(rec_p, 1, elist));
}

/* -------------------------------------------------------------------------- */

/*
 * do_updates
 *      This function applies an update packet to an entry.
 *
 * Inputs:
 *      user_p             pointer to user descriptor
 *      epoch_p            pointer to the epoch id
 *      entity_fname_p     a pointer to the update packet from the message
 *                         which starts with the full name of the entity
 *      notused_p          a pointer to the update packet past full name
 *      len                length of update packet
 */
dns_status_t 
do_updates (user_descriptor_t  *user_p,
            ObjUID_t           *epoch_p,
            FullName_t         *entity_fname_p,
            UpdatePacket_t     *notused_p,
            int                len)
{
    LOG_ASCII_BUF_FullName(entityNameBuf)
    LOG_ASCII_BUF_Timestamp(tsBuf)
    DEB_ASCII_BUF_Timestamp(alluptoBuf)
    struct tlog_dsc     tlog;
    Timestamp_u         allupto_ts;
    Timestamp_t         *entryCTS_p;
    byte_t              *updateset_p;
    DBSet_t             *set_p;
    DBSetMember_t       *value_p;
    db_stream_t         *stream_p;
    dir_hash_entry_t    *hash_p;
    int                 cnt, index, entrytype, updateCount;
    dns_status_t        status;

    /* parse the UpdatePacket */
    entryCTS_p = (Timestamp_t *)SKIP_FullName(entity_fname_p);
    updateset_p = SKIP_Timestamp(entryCTS_p);
    entrytype = GET8(updateset_p);
    updateCount = GET8(updateset_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >do_update: [%d]%s (CTS=%s)",
        entrytype,
        deb_ascii_FullName(entityNameBuf,entity_fname_p),
        deb_ascii_Timestamp(tsBuf,entryCTS_p)));

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_DoUpdate);
    if (status != DNS_SUCCESS) return(status);

    tlog.cnt = updateCount;
    tlog.len = SKIP_bytes(entity_fname_p, len) - updateset_p;
    tlog.buf_p = updateset_p;

    /* Build directory name */
    names_new((ObjUID_t *)entity_fname_p->fn_root, NULL,
                (FullName_t *)stream_p->db_key);

    if (entrytype == ET_directory) {
        status = db_dir_read_for_update(stream_p);
    } else {
        status = db_dir_read(stream_p);
    }

    if (status != DNS_SUCCESS) {
        (void)db_close_stream(stream_p);
        if (status == DNS_UNKNOWNENTRY) {
            return (DNS_NOTAREPLICA);
        }
        return (status);
    }

    hash_p = stream_p->db_dir_hash_p;

    /*
     * Sending server needs control access to directory
     */

    status = security_check(user_p, stream_p, Super_check, ET_directory,
                            NULL);
    if (status != DNS_SUCCESS) {
        security_log(user_p, (FullName_t *)hash_p->dir_name,
                        stream_p->db_file_p->file_ch_p);
        (void)db_close_stream(stream_p);
        if (status == DNS_ACCESSVIOLATION) {
            return (DNS_UNTRUSTEDCH);
        }
        return (status);
    }


    /*
     * Make sure the update is for the same epoch we are in.
     */

    if (!set_dnstest(stream_p->db_dir.data_p,
        sizeof(*epoch_p), (byte_t *)epoch_p, dns_Attribute(DNS_EPOCH))) {
        (void)db_close_stream(stream_p);
        return (DNS_BADEPOCH);
    }

    /*
     * If a new replica, accept everything
     */

    if (db_dir_get_replica_state(hash_p) == RS_newReplica) {
        COPY_Timestamp(NullTimestamp, allupto_ts);
    } else {
        /* Locate the real CDS_AllUpTo attribute in the directory */
        status = find_attribute(stream_p->db_dir.data_p,
                    dns_AttributeName(DNS_ALLUPTO), &set_p, &index);
        if (status != DNS_SUCCESS) {
            evl_AttributeMissing(stream_p->db_file_p->file_ch_p,
                (FullName_t *)hash_p->dir_name, dns_AttributeName(DNS_ALLUPTO));
            (void)db_close_stream(stream_p);
            return (DNS_NAMESERVERBUG);
        }

        /*
         * CDS_ALLUPTO is always a single valued set with no history
         */

        value_p = (DBSetMember_t *)DATA_DBSet(set_p);
        COPY_Timestamp(value_p->dm_data, allupto_ts);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        "  do_update: AllUpTo: %s",
        deb_ascii_Timestamp(alluptoBuf,allupto_ts)));

    /*
     * If the update packet is for the directory itself, apply the update
     * packet and update the directory entry.   Otherwise, we have to
     * read the appropriate entry and update it (or create it if it doesn't
     * exist).
     */

    if (entrytype == ET_directory) {
        /* Apply the updates to the directory */
        status = apply_update_packet(stream_p, &stream_p->db_dir,
                (Timestamp_t *)allupto_ts, (Update_t *)updateset_p,
                updateCount, (FullName_t *)stream_p->db_key, NULL);
        if (status == DNS_SUCCESS) {
            status = db_dir_update(stream_p, &tlog);
        }
    } else {
        /* Set up entry name */
        /* X.500 - normalize name, neutral case no good for typed names */
        status = dns_normalize_fname(entity_fname_p,
                (FullName_t *)stream_p->db_key, sizeof(stream_p->db_key));


        /*
         * O.K., here we go.   First we optimistically try to read the
         * entry (type-specific) from the database.  This is a performance
         * booster which assumes names don't change types very often.
         * If we can't find the entry under the type specified by the
         * update packet, then we use the check_exists routine (it resides
         * in ta_lookup) to hunt for any entry with the specified name.
         */

        if (status == DNS_SUCCESS) {
            stream_p->db_entry.data_type = entrytype;
            status = db_entry_read_for_update(stream_p);
        }

        if (status == DNS_UNKNOWNENTRY)
            {
            status = check_exists(stream_p, WALKTREE_UPDATE_OP);
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug6,
                "  do_update: check_exists status: %d -> [%d]",
                status,
                stream_p->db_entry.data_type));
            }


        if (status == DNS_SUCCESS)
            {

            int destroy_entry = FALSE;

            /*
             * Well even though we found an entry, it doesn't mean that
             * want to update it.  Depending on the type and entry CTS's
             * involved we may want to ignore the update (it references
             * old (purged) data), or we may be attempting to supercede
             * an existing entry.
             */

            /*
             * If the entry CTS's don't match, then we should check
             * the delete holders in case we are attempting to update
             * a deleted entry -- which we can ignore.
             */

            if ((stream_p->db_entry.data_type == entrytype)
		&& NE_Timestamp(stream_p->db_entry.data_cts, entryCTS_p)) {
                set_p = (DBSet_t *)stream_p->db_entry.data_p->dbr_data;
                ALIGN_CHECK(set_p);

                if ((cnt = NUM_DBSet_Delete(set_p)) != 0) {
                    delete_holder_t *delete_p =
                        (delete_holder_t *)DATA_DBSet_Delete(set_p);

                    do {
                        if (EQ_Timestamp(delete_p->del_uid, entryCTS_p)) {
                            DCE_SVC_DEBUG((
                                cds__svc_handle,
                                cds_svc_server,
                                svc_c_debug6,
                                "  do_update: ignoring deleted update"));
                            (void)db_close_stream(stream_p);
                            return(DNS_SUCCESS);
                        }
                    } while (delete_p++, --cnt);
                }
            }


            /*
             * Now we check to see if we're attempting to replace an
             * existing entry -- and whether or not to let that happen.
             *
             * If the database has an older entry, we let the newer
             * update win UNLESS the database is holding an older
             * childpointer, which must always win.
             */

            if (dnsCmpCTS((dns_cts *)stream_p->db_entry.data_cts,
                          (dns_cts *)entryCTS_p) < 0) {

                if ((stream_p->db_entry.data_type == ET_childPointer)
		    && (entrytype != ET_childPointer)) {
                    /*
                     * If we found a childpointer in the database, then
                     * we let it win.
                     */
                    DCE_SVC_DEBUG((
                        cds__svc_handle,
                        cds_svc_server,
                        svc_c_debug6,
                        "  do_update: childpointer in database wins"));
                    (void)db_close_stream(stream_p);
                    return(DNS_SUCCESS);
                } else {
                   destroy_entry = TRUE;
                }

            } else if (entrytype == ET_childPointer) {
                /*
                 * If the incoming update is a childpointer, then we
                 * let it usurp whatever is in the database.  [Actually
                 * there are some corruption checks missing here.]
                 */
                if (stream_p->db_entry.data_type != ET_childPointer) {
                    destroy_entry = TRUE;
                }
            }


            if (destroy_entry) {

                /*
                 * Destroy the entry and synthesize a DNS_UNKNOWNENTRY
                 * status -- which will trigger the create code downstream.
                 */

                /*
                 * Delete the entry located by previous read or check_exists.
                 */

                status = db_entry_delete(stream_p);
                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug6,
                    "  do_update: destroy entry [%d] status: %d",
                    stream_p->db_entry.data_type,
                    status));
                db_entry_release(stream_p);
                if (status != DNS_SUCCESS) {
                    (void)db_close_stream(stream_p);
                    return(DNS_NAMESERVERBUG);
                }

                /*
                 * "Raise" the UnknownEntry exception...
                 */

                status = DNS_UNKNOWNENTRY;

            }

        }

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug6,
            "  do_update: entry lookup status: %d",
            status));

        /*
         * At this point we either update the entry or create it (depending
         * on the rather long-winded "read" above.
         */

        if (status == DNS_SUCCESS) {
            if (EQ_Timestamp(stream_p->db_entry.data_cts, entryCTS_p)) {
                status = apply_update_packet(stream_p, &stream_p->db_entry,
                        (Timestamp_t *)allupto_ts, (Update_t *)updateset_p,
                        updateCount, (FullName_t *)stream_p->db_key, NULL);
                if (status == DNS_SUCCESS) {
                    status = db_entry_update(stream_p, &tlog);
                }
                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug6,
                    "  do_update: updated packet applied: %d",
                    status));
            } else {
                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug6,
                    "  do_update: updated packet ignored (cts mismatch)"));
            }

        } 
	else 
        if (status == DNS_UNKNOWNENTRY) 
	{

            int delete_entry_packet_detected;

            /*
             * make sure the creation uid is greater than allupto,
             * and less than future skew, (if bad - return NameServerBug)
             */

            if (!ts_sanity(entryCTS_p, (Timestamp_t *)allupto_ts)) 
	    {
                Timestamp_u         now;

                DCE_SVC_DEBUG((
		    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug6,
		    "apply_update_packet: create entry TS sanity failure"));

                if (ts_new((Timestamp_t *)now) == DNS_SUCCESS) 
		{
                    DCE_SVC_DEBUG((
		        cds__svc_handle,
                        cds_svc_server,
                        svc_c_debug6,
		        "apply_update_packet:    UTC now:    %s",
                        deb_ascii_Timestamp(tsBuf,(Timestamp_t*)now)));
                }

		DCE_SVC_DEBUG((
		    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug6,
		    "apply_update_packet:   Entry ID:   %s",
                    deb_ascii_FullName(entityNameBuf,entity_fname_p)));

                DCE_SVC_DEBUG((
		    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug6,
		    "apply_update_packet:   Entry CTS:  %s",
                    deb_ascii_Timestamp(tsBuf,entryCTS_p)));

                DCE_SVC_DEBUG((
		    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug6,
		    "apply_update_packet:   AllUpto TS: %s",
                    deb_ascii_Timestamp(tsBuf,(Timestamp_t*)allupto_ts)));

                (void)db_close_stream(stream_p);
                return (DNS_NAMESERVERBUG);
            }

            /*
             * Reset type.  If check_exists() was called above, then the
             * entry type in the stream will have been modified (a side-
             * effect of check_exists).
             */
            stream_p->db_entry.data_type = entrytype;

            status = create_entry(&stream_p->db_entry, entryCTS_p);

            if (status == DNS_SUCCESS) {
                status = apply_update_packet(stream_p, &stream_p->db_entry,
                    (Timestamp_t *)allupto_ts, (Update_t *)updateset_p,
                    updateCount, (FullName_t *)stream_p->db_key,
                    &delete_entry_packet_detected);
                if (status == DNS_SUCCESS) {
                    if (delete_entry_packet_detected) {
                        DCE_SVC_DEBUG((
                            cds__svc_handle,
                            cds_svc_server,
                            svc_c_debug6,
                            "  do_update: redundant delete bypassed"));
                    } else {
                        status = db_entry_create(stream_p,
                                       (SimpleName_t *)entity_fname_p->fn_name,
                                       entrytype);
                        DCE_SVC_DEBUG((
                            cds__svc_handle,
                            cds_svc_server,
                            svc_c_debug6,
                            "  do_update: entry created: %d",
                            status));
                    }
                }
            }
        }
    }

    (void)db_close_stream(stream_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <do_update: done (%d)",
        status));

    return(status);

}
