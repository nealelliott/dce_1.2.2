/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_epoch.c,v $
 * Revision 1.1.10.2  1996/02/18  19:36:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:38  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:33:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:09  root]
 * 
 * Revision 1.1.8.4  1994/08/16  18:28:09  jd
 * 	drop from janet
 * 	[1994/08/13  16:08:00  jd]
 * 
 * Revision 1.1.8.3  1994/08/03  19:04:49  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:31:33  mccann]
 * 
 * Revision 1.1.8.2  1994/06/09  18:44:02  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:38  devsrc]
 * 
 * Revision 1.1.8.1  1994/03/12  22:13:00  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:46:47  peckham]
 * 
 * Revision 1.1.5.2  1992/12/30  14:24:09  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:24:46  zeliff]
 * 
 * Revision 1.1.2.3  1992/04/21  21:09:02  grober
 * 	Modified routine ta_epoch to ensure that db_close_stream was
 * 	called before returning.  If not, a potential hang may occur
 * 	when checkpointing.
 * 	[1992/04/21  18:45:01  grober]
 * 
 * Revision 1.1.2.2  1992/03/22  22:36:57  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:36:57  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: ta_epoch.c
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
 * Module Description
 *
 * Implements the NewEpoch transaction.
 * 
 */


/*
 *  Include files
 */
#include <server.h>
#include <clerk.h>
#include <find.h>
#include <security.h>
#include <sets.h>
#include <tlog.h>
#include <replica.h>
#include <uid.h>
#include <dnsnormal.h>
#include <back.h>

/*
 * Local prototypes
 */
static dns_status_t
bldreplicas (
    db_stream_t		*,
    int			,
    Set_t		*,
    tlog_dsc_t		*,
    byte_t		**);

static dns_status_t
fixup (
    db_stream_t		*,
    ObjUID_t		*,
    tlog_dsc_t		*,
    byte_t		*);

/*
 * ROUTINE - bldreplicas
 *
 * Build the replica updates by retrieving from the clerk the
 * clearinghouse address and id.  Make sure the replica exists.
 *
 * Arguments:
 *    stream_p (in)    - pointer to db stream (attached to directory)
 *    type     (in)    - replica type assigned to the set_p replicas
 *    set_p    (in)    - pointer to set of clearinghouses with replicas
 *    tlog_p   (in)    - pointer to tlog descriptor containing update buffers
 *    next_pp  (inout) - address of next postion in update buffer
 */
static dns_status_t 
bldreplicas (db_stream_t  *stream_p,
             int          type,
             Set_t        *set_p,
             tlog_dsc_t   *tlog_p,
             byte_t       **next_pp)
{
    MemberValue_t  *CHID_val_p;    /* Points to the CHID we want to save */
    ObjUID_u       saveCHID;        /* Holds the CHID we obtained */
    byte_t	   *outbuf_p;       /* Read buffer for clerk read ops */
    byte_t	   *outbuf2_p;      /* Read buffer for clerk read ops */
    MemberValue_t  *member_p;       /* Points to member within set_p */
    AtomicValue_t  *av_p;
    FullName_t     *name_p;         /* Points to FullName within member_p */
    Set_t          *address_set_p;  /* Towers for the clearinghouse */
    int             num_set;        /* How many replicas we're building */
    byte_t          linked;         /* Flag for softlink detection */
    int             i;
    int             len;            
    dns_status_t    status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >bldreplicas(0x%x,%d,0x%x, ...)",
        stream_p,
        type,
        set_p));

    /*
     * If there are no replicas in the set, we can just return 
     */
    if ((num_set = NUM_Set(set_p)) == 0)
        {

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug5,
                " <bldreplicas(0x%x,%d,0x%x,) -> DNS_SUCCESS (empty set)",
                stream_p,
                type,
                set_p));

	    return(DNS_SUCCESS);
        }

    /*
     * Allocate a buffer for clerk operations.   We have to retrieve
     * both the ID and the address of the clearinghouse.  We are
     * allocating two buffers in case we have to do a DNS$Address to
     * tower conversion (the address will be read into outbuf2_p and
     * the converted tower sent to the outbuf_p).
     */
    outbuf_p = (byte_t *)dns_malloc(READ_BUF_SIZE + READ_BUF_SIZE);
    if (!outbuf_p) 
        {
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug5,
                " <bldreplicas(0x%x,%d,0x%x,) -> DNS_NONSRESOURCES",
                stream_p,
                type,
                set_p));

	    return(DNS_NONSRESOURCES);
        }
    outbuf2_p = SKIP_bytes(outbuf_p, READ_BUF_SIZE);

    /*
     * For each member of the set of clearinghouses we have been given:
     *   1. Get the CHID.
     *   2. Get the CH address.  We try to get it from the towers attribute,
     *      but we can fall back to the DNS$Address attribute and convert
     *      it to a tower.
     *   3. Finally we make sure that the clearinghouse name we are given
     *      isn't a softlink.  If it is, we'll resolve it now and modify the
     *      the replica pointer.
     */

    for (i = 0; i < num_set; ++i)
        {
        /* Read clearinghouse object to get clearinghouse id */
	member_p = (MemberValue_t *)INDEX_Set(set_p, i);
	name_p = (FullName_t *)member_p->mv_value;

	status = clerk_read_attribute_conf(name_p, ET_object, 
		dns_AttributeName(DNS_OBJECTUID), outbuf_p, 
	        NULL, &linked, dnsConfMed);
	if (status != DNS_SUCCESS) break;

        /* Stow away the ID for later */
        CHID_val_p = (MemberValue_t *)SKIP_AttributeType(outbuf_p);
        COPY_ObjUID((ObjUID_t *)CHID_val_p->mv_value, saveCHID);

        /* Read clearinghouse object to get clearinghouse address */
        status = clerk_read_attribute_conf(name_p, ET_object,
	    dns_AttributeName(DNA_TOWERS), outbuf_p,
	    NULL, NULL, dnsConfMed);
	address_set_p = (Set_t *)SKIP_AttributeType(outbuf_p);
	if (status != DNS_SUCCESS) break;
	/* Build update without data itself */
	len = update_dns_build((Update_t *)*next_pp, UD_present, NullTimestamp,
		dns_Attribute(DNS_REPLICAS), BL_true, 0, NULL);

        /* Tag on the replica pointer to the end of the update */
	av_p = (AtomicValue_t *)SKIP_bytes(*next_pp, len);
	INS16(av_p->av_length, 
	      replica_build((ReplicaPointer_t *)av_p->av_value, 
			    (ObjUID_t *)saveCHID,
			    (Set_t *)(SKIP_AttributeType(outbuf_p)),
			    type, name_p));

	tlog_p->len += len + LEN_AtomicValue(av_p);

	dce_assert(cds__svc_handle,
		   (stream_p->db_dir_hash_p != 0));

	/* Make sure replica exists at this clearinghouse */
	status = clerk_read_replica((ObjUID_t *)stream_p->db_dir.data_uid, 
		(FullName_t *)stream_p->db_dir_hash_p->dir_name,
		dns_AttributeName(DNS_CTS),
		outbuf_p, (ReplicaPointer_t *)av_p->av_value, 
		(int)EXT16(av_p->av_length),
		NULL);
	if (status != DNS_SUCCESS) break;

	/* If the name is linked, need to resolve it */
  	if (linked)
	    {
	    FullName_t *newname_p = (FullName_t *)
		    SKIP_bytes(tlog_p->buf_p, 
                               tlog_p->len - LEN_FullName(name_p));

	    if (clerk_resolve_name(name_p, newname_p) == DNS_SUCCESS)
		{
	    	/* Fix length by removing old and adding new */
		INS16(av_p->av_length, EXT16(av_p->av_length)
		    - LEN_FullName(name_p) + LEN_FullName(newname_p));
		tlog_p->len = tlog_p->len -  LEN_FullName(name_p) +
			LEN_FullName(newname_p);
		}
	    }

	*next_pp = SKIP_bytes(tlog_p->buf_p, tlog_p->len);
	++tlog_p->cnt;
	}
	
    dns_free((char *)outbuf_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <bldreplicas(0x%x,%d,0x%x,) -> 0x%x (%d)",
        stream_p,
        type,
        set_p,
        status,
        status));

    return(status);
}

/*
 * ROUTINE - fixup
 *
 * This routine sets up allupto, epoch, lastskulk.
 * and replicatype.  It also builds the replica set.
 * 
 * Arguments:
 *    stream_p      (in) - pointer to db stream
 *    epoch_p       (in) - new epoch value 
 *    tlog_p        (in) - pointer to TLog descriptor for logging & buffering
 *    next_p        (in) - pointer to next buffer in tlog 
 *
 * When this routine is called, the TLog buffer contains a set of updates
 * for replica pointers.  We will be using these RP updates, and adding five
 * of our own.  Another update buffer is malloc'd by this routine: it holds
 * the ReplicaType update that is sent to the other clearinghouses (and thus
 * not recorded in the local transaction log).
 *
 * Upon entry, the updates in the TLog buffer are laid out like this:
 *
 * | DeleteRep | MasterRP | ReadOnlyRP | ... |
 *
 * There are 0 or more ReadOnly updates (all these were built by bldreplicas).
 *
 * This routine will append the following updates:
 *
 * ...| NewEpoch | ReplicaType=Master | AllUpTo | LastSkulk  
 *    ^epoch_upd_p                    ^allupto_upd_p        
 *                                              ^lastskulk_upd_p
 *
 * A second buffer (rptype_upd_p) holds an update for the ReplicaType
 * we wish to apply to the other clearinghouses (it is modified for each
 * replica as appropriate).
 */
static dns_status_t 
fixup (db_stream_t  *stream_p,
       ObjUID_t     *epoch_p,
       tlog_dsc_t   *tlog_p,
       byte_t       *next_p)
{
    int            rpcnt;           /* Count of replicas */
    int            len;
    int            index;
    DBSet_t       *set_p;
    Update_t      *allupto_upd_p;   /* Points to DNS$AllUpTo update */
    Update_t      *lastskulk_upd_p; /* Points to DNS$LastSkulk update */
    Update_t      *epoch_upd_p;     /* Points to DNS$Epoch update */
    Update_t      *rptype_upd_p;    /* Points to DNS$ReplicaType update */
    Update_t      *delete_upd_p;    /* Points to delete DNS$Replicas update */
    Update_t      *upd_p;
    int            rptype_len, delete_len, allupto_len,
                   lastskulk_len, epoch_len;
    byte_t        *typedata_p;      /* Points to replicatype in rptype_upd_p */
    dns_status_t   status;
    Timestamp_u    lastskulkvalue;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >fixup(0x%x,0x%x, ...)",
        stream_p,
        epoch_p));

   ts_new((Timestamp_t *)lastskulkvalue);
/*
 * The FIXUP_FAILURE instrumentation helps pinpoint which call failed
 * during fixup.  It also frees up any malloc'd areas.
 */
#ifdef DCE_CDS_DEBUG

#define FIXUP_FAILURE(point, status) \
     { \
         if (rptype_upd_p) dns_free((char *)rptype_upd_p); \
         \
         DCE_SVC_DEBUG(( \
             cds__svc_handle, \
             cds_svc_server, \
             svc_c_debug5, \
             " <fixup(0x%x,0x%x) - failure[%d]: 0x%x", \
             stream_p, \
             epoch_p, \
             point, \
             status)); \
         \
         return(status); \
     }

#else

#define FIXUP_FAILURE(point, status) \
     { \
         if (rptype_upd_p) \
         { \
	     dns_free((char *)rptype_upd_p); return(status); \
         } \
     }
    
#endif


    rptype_upd_p = NULL;   /* Needed for FIXUP_FAILURE */

    /*
     * First we will put all the DNS$Replicas updates into the local
     * clearinghouse.
     */
    upd_p = (Update_t *)tlog_p->buf_p;

    /* Insert each replica into the record */
    for (index = 0; index < tlog_p->cnt; index++)
        {
        ts_new((Timestamp_t *)upd_p->ud_timestamp);  
        status = set_insert(&stream_p->db_dir, upd_p, 
			dns_Attribute(DNS_REPLICAS), FALSE);
        if (status != DNS_SUCCESS) FIXUP_FAILURE(1, status);
	upd_p = (Update_t *)SKIP_Update(upd_p);
        }

    /* Find maximum timestamp of updates on replica set */
    find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_REPLICAS), &set_p, &index);
    set_timestamp_set(&stream_p->db_dir, (Timestamp_t *)set_p->ds_ts,
		dns_Attribute(DNS_UTS), NULL, NULL);
    db_dir_set_lastupdate(stream_p->db_dir_hash_p,
		(Timestamp_t *)set_p->ds_ts);


    /*
     * Now we prepare to update the other clearinghouses.
     *
     * First we calculate the number of replicas that have been supplied.
     * This is calculated by subtracting 2 from the TLog count (one for
     * the delete update, and one for the Master update (local)).
     *
     * Then we set up the updates for Epoch, AllUpTo, LastSkulk,
     * and the master ReplicaType.
     */

    /* Determing Replicas to talk to, first update is delete, 2nd is self */
    rpcnt = tlog_p->cnt - 2;

    /* Declare new epoch */
    epoch_len = update_dns_build((Update_t *)next_p, UD_present, 
		NullTimestamp, dns_Attribute(DNS_EPOCH),
		BL_true, sizeof(ObjUID_t), (byte_t *)epoch_p);
    status = set_replace_value(&stream_p->db_dir, (Update_t *)next_p,
	dns_Attribute(DNS_EPOCH));
    if (status != DNS_SUCCESS) FIXUP_FAILURE(2, status);

    /* Save pointer to modify replica */
    epoch_upd_p = (Update_t *)next_p;
    ++tlog_p->cnt;
    tlog_p->len += epoch_len;
    next_p = SKIP_bytes(tlog_p->buf_p, tlog_p->len);

    status = db_dir_set_type(stream_p, RT_master, (Update_t *)next_p, &len);
    if (status != DNS_SUCCESS) FIXUP_FAILURE(3, status);
    ++tlog_p->cnt;
    tlog_p->len += len;
    next_p = SKIP_bytes(tlog_p->buf_p, tlog_p->len);
    
    /* Now wind back allupto to the beginning of time */
    allupto_len = update_dns_build((Update_t *)next_p, UD_present, NULL,
		dns_Attribute(DNS_ALLUPTO),
		BL_true, sizeof(NullTimestamp), (byte_t *)NullTimestamp);
    status = set_insert(&stream_p->db_dir, (Update_t *)next_p,
	dns_Attribute(DNS_ALLUPTO), TRUE);
    if (status != DNS_SUCCESS) FIXUP_FAILURE(4, status);
    allupto_upd_p = (Update_t *)next_p;
    ++tlog_p->cnt;
    tlog_p->len += allupto_len;
    next_p = SKIP_bytes(tlog_p->buf_p, tlog_p->len);

    /* Force lastskulk */
    status = set_timestamp_set(&stream_p->db_dir, 
			       (Timestamp_t *)lastskulkvalue, 
	    dns_Attribute(DNS_LASTSKULK), (Update_t *)next_p, &lastskulk_len);
    if (status != DNS_SUCCESS) FIXUP_FAILURE(5, status);
    lastskulk_upd_p = (Update_t *)next_p;
    ++tlog_p->cnt;
    tlog_p->len += lastskulk_len;
    next_p = SKIP_bytes(tlog_p->buf_p, tlog_p->len);

    /* 
     * The ReplicaType update for the other clearinghouses is held
     * in a separate buffer (out of the TLog buffer).  This update will
     * be modified as needed during the walk of the replica set, so the
     * value we initialize it with here is irrelevant.
     */

    rptype_upd_p = (Update_t *)dns_malloc(sizeof(Update_u) - MAXATTRIBUTE + 1);
    rptype_len = update_dns_build(rptype_upd_p, UD_present, NullTimestamp,
	    dns_Attribute(DNS_REPLICATYPE),
	    BL_true, sizeof(char), (byte_t *)NullTimestamp);
    typedata_p = SKIP_bytes(rptype_upd_p, rptype_len - sizeof(char));


    /* Save pointer to first update (delete Replicas) */
    delete_upd_p = (Update_t *)tlog_p->buf_p;
    delete_len = LEN_Update(delete_upd_p);

    
    /*
     * Position ourselves within the set of replicas in the TLog buffer.
     * Because the loop below advances by one update per loop, we initialize
     * our position just before the first replica -- this happens to be
     * the master replica pointer, but we don't care.  Once the loop starts
     * it'll point to the first readonly replica update.
     */

    upd_p = (Update_t *)SKIP_Update(delete_upd_p);

    /* Loop through each replica */
    for (index = 0; index < rpcnt; ++index)
	{
	AtomicValue_t *av_p; /* Points to the ReplicaPointer data */

	upd_p = (Update_t *)SKIP_Update(upd_p);
	av_p = (AtomicValue_t *)SKIP_BOOLEAN(
			SKIP_AttributeSpecifier(upd_p->ud_attribute));

	/* Fix AllUpTo */
	status = clerk_modify_replica((ObjUID_t *)stream_p->db_dir.data_uid,
	      allupto_upd_p, allupto_len,
	      (ReplicaPointer_t *)av_p->av_value, (int)EXT16(av_p->av_length));
	if (status != DNS_SUCCESS) FIXUP_FAILURE(7, status);

	/* Fix Epoch */
	status = clerk_modify_replica((ObjUID_t *)stream_p->db_dir.data_uid,
		epoch_upd_p, epoch_len, (ReplicaPointer_t *)av_p->av_value, 
		(int)EXT16(av_p->av_length));
	if (status != DNS_SUCCESS) FIXUP_FAILURE(8, status);

	/*
	 * Send delete all replicas update 
	 */
	status = clerk_modify_replica((ObjUID_t *)stream_p->db_dir.data_uid,
				      delete_upd_p, 
				      delete_len, 
				      (ReplicaPointer_t *)av_p->av_value,
				      (int)EXT16(av_p->av_length));

	if (status != DNS_SUCCESS) FIXUP_FAILURE(9, status);

	/* Fix lastskulk */
	status = clerk_modify_replica((ObjUID_t *)stream_p->db_dir.data_uid,
	      lastskulk_upd_p, lastskulk_len,
	      (ReplicaPointer_t *)av_p->av_value, (int)EXT16(av_p->av_length));
	if (status != DNS_SUCCESS) FIXUP_FAILURE(10, status);

	INS8(typedata_p, RT_readOnly);
	status = clerk_modify_replica(
				      (ObjUID_t *)stream_p->db_dir.data_uid,
				      rptype_upd_p, rptype_len,
				      (ReplicaPointer_t *)av_p->av_value, 
				      (int)EXT16(av_p->av_length));

	if (status != DNS_SUCCESS) FIXUP_FAILURE(13, status);
      }
	
    dns_free((char *)rptype_upd_p);
    return(DNS_SUCCESS);
}

/*
 * Secondaries are obsolete, but still in protocol.
 * ta_epoch
 *    This routine declares a new epoch.  It rebuilds the replica
 * set from user input and updates the epoch attributes
 * of each replica.
 *
 * Input:
 *	user_p = pointer to user descriptor
 *	dir_p = pointer to directory fullname
 *	secondary_p = pointer to set of secondaries
 *	readonly_p = pointer to st of readonlies
 */
dns_status_t 
ta_epoch (user_descriptor_t  *user_p,
          FullName_t         *dir_p,
          Set_t              *secondary_p,
          Set_t              *readonly_p)
{

#ifdef LOG_ASCII_BUF_user_descriptor
    LOG_ASCII_BUF_user_descriptor(userBuf)
#else
    char userBuf[DEB_ASCII_LEN_user_descriptor];
#endif
    LOG_ASCII_BUF_FullName(directoryBuf)
    LOG_ASCII_BUF_dns_status(statusBuf)


    tlog_dsc_t		tlog;
    ObjUID_u		newepoch;
    db_stream_t		*stream_p;
    AtomicValue_t	*av_p;
    byte_t		*mem_p, *next_p;
    int			cnt, len;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >ta_epoch(0x%x,0x%x,0x%x,0x%x)",
        user_p,
        dir_p,
        secondary_p,
        readonly_p));

    /* Don't support in cds */
    if (NUM_Set(secondary_p) != 0)
	return(DNS_NOTSUPPORTED);

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ModifyAttribute);
    if (status != DNS_SUCCESS) return(status);

    /* Set up directory key */
    /* X.500 changing case isn't sufficient for typed names */
    status = dns_normalize_fname(dir_p,
		(FullName_t *)stream_p->db_key, sizeof(stream_p->db_key));
    if (status != DNS_SUCCESS) {
	db_close_stream(stream_p);
	return(status);
    }


    /* Read directory */
    status = db_dir_read_for_update(stream_p);
    if (status != DNS_SUCCESS)
	{
	db_close_stream(stream_p);
	return(DNS_NOTAREPLICA);
	}

    if (db_dir_get_replica_state(stream_p->db_dir_hash_p) != RS_on)
	{
	db_close_stream(stream_p);
	return(DNS_WRONGSTATE);
	}


    /*
     * Check for control access to the directory
     */
    status = security_check(user_p, stream_p,
                          Super_check, ET_directory, NULL);
    if (status != DNS_SUCCESS)
        {
	db_close_stream(stream_p);
        return(status);
        }


    /*
     * Make sure user has write access to the clearinghouse if we're
     * becoming the new master.
     */
    if (db_dir_get_replica_type(stream_p->db_dir_hash_p) != RT_master)
	{
	  /* Get a clearinghouse stream */
	  db_stream_t 	*clh_stream_p = NULL;
	  status = db_get_stream_ignore(stream_p->db_file_p,
					&clh_stream_p,
					OP_CreateDirectory);
	  if (status != DNS_SUCCESS) 
	      {
		db_close_stream(stream_p);
		return (status);
	      }		
	  clh_stream_p->db_dir_hash_p = NULL;
	  ZERO_bytes(&clh_stream_p->db_dir, sizeof(db_data_t));
	  status = db_pseudo_read(clh_stream_p);
	  if (status == DNS_SUCCESS)
	      {
		status = security_check (user_p, clh_stream_p, Modify_check,
					 ET_clearinghouse, NULL);
		if (status != DNS_SUCCESS)
		    {
		      db_close_stream(stream_p);
		      (void)db_close_stream(clh_stream_p);
		      return (status);
		    }
		(void)db_close_stream(clh_stream_p);
	      }
	}

    /* Determine number of replica pointers including master */
    cnt = 1 + NUM_Set(readonly_p); 

    /* 
     * Allocate enough memory to log all the updates we will make to this 
     * directory.  This includes deleting the existing replica pointer,
     * adding the new ones, plus epoch, replicatype, allupto, lastskulk,
     */
    mem_p = (byte_t *)dns_malloc(
	 (cnt+1)*(sizeof(Update_u) - MAXATTRIBUTE + sizeof(ReplicaPointer_u)) +
	 5*(sizeof(Update_u) - MAXATTRIBUTE + sizeof(ObjUID_u)));
    if (!mem_p)
        {
        db_close_stream(stream_p);
        return(DNS_NONSRESOURCES);
        }

    tlog.cnt = 2;
    tlog.len = 0;
    tlog.buf_p = (byte_t *)mem_p;
    next_p = mem_p;

    /* Build delete replica update */
    tlog.len = update_dns_build((Update_t *)next_p, UD_absent, NullTimestamp,
		dns_Attribute(DNS_REPLICAS),
		BL_false, 0, NULL);

    next_p = SKIP_bytes(next_p, tlog.len);

    /* Add master replica update */
    len = update_dns_build((Update_t *)next_p, UD_present, NullTimestamp,
		dns_Attribute(DNS_REPLICAS), BL_true, 0, NULL);

    av_p = (AtomicValue_t *)SKIP_bytes(next_p, len);

    INS16(av_p->av_length, 
	  replica_build((ReplicaPointer_t *)av_p->av_value,
                        &stream_p->db_file_p->file_ch_p->ch_uid, 
			NULL,
			RT_master, 
			&stream_p->db_file_p->file_ch_p->ch_name));
    
    tlog.len += len + LEN_AtomicValue(av_p);

    next_p = SKIP_bytes(mem_p, tlog.len);
    /* Build readonly replicas */
    if (status == DNS_SUCCESS)
        status = bldreplicas(stream_p, RT_readOnly, readonly_p, &tlog, &next_p);

    if 	(status == DNS_SUCCESS)
    {
	status = id_new((ObjUID_t *)newepoch);
	if (status == DNS_SUCCESS)
	{
            dns_status_t skulk_status;

            status = fixup(stream_p, (ObjUID_t *)newepoch, 
		&tlog, next_p);
            if (status == DNS_SUCCESS)
	    {
                status = db_dir_update(stream_p, &tlog);
	    }
            if (status == DNS_SUCCESS)
	    {
		skulk_status = skulk_sched(stream_p);
                if (skulk_status != DNS_SUCCESS)
		{
                    DCE_SVC_DEBUG((
                        cds__svc_handle,
                        cds_svc_server,
                        svc_c_debug3,
			"ta_epoch(%s,%s) - ignored skulk failure(%s)",
                        deb_ascii_user_descriptor(userBuf,user_p),
                        deb_ascii_FullName(directoryBuf,dir_p),
                        deb_ascii_dns_status(statusBuf,skulk_status)));
		}
	    }
	}
    }
    dns_free((char *)mem_p);
    db_close_stream(stream_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <ta_epoch(0x%x,0x%x,0x%x,0x%x) - 0x%x (%d)",
        user_p,
        dir_p,
        secondary_p,
        readonly_p,
        status,
        status));

    return(status);
}
