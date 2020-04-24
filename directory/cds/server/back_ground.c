/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_ground.c,v $
 * Revision 1.1.18.1  1996/08/09  11:59:15  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/08/06  14:43 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/2]
 *
 * 	Part of fix for OT 13208:
 * 	Change use of file_back_inhibit to inhibit running background for
 * 	period of time.  Useful during mgmt_create_clh(), also db_diags.
 * 	background_activator() now only waits 2 seconds.
 * 	*
 * 	Be sure that background doesn't assume file_root_dir_p is filled
 * 	yet.  Won't be true during clearinghouse creation: the background
 * 	is started before the root directory replica is created.
 * 	*
 * 	Update clearinghouse version automatically when root at version 4.0.
 * 	*
 * 	Cleanup multiple requests of forcing background.  If a request occurs
 * 	while the background thread is already running, the background thread
 * 	will get run again.
 * 	[1996/02/18  19:35:25  marty  1.1.16.3]
 *
 * Revision 1.1.16.3  1996/02/18  19:35:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:39  marty]
 * 
 * Revision 1.1.16.2  1995/12/08  15:28:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:19  root]
 * 
 * Revision 1.1.12.1  1994/10/25  16:17:36  proulx
 * 	Cause immediate updates when clearinghouse towers change.
 * 	Include utc timers and flags to check clearinghouse relocation,
 * 	as well as a request to cause a skulk. Use utc throughout.
 * 	background_activator() starts background() initially after 4 secs,
 * 	at which time towers are checked against CHLastAddress.
 * 	background() calls check_*_replica_ptr() if relocating.
 * 	Force ParentPointer checks in back_on() when relocating.
 * 	[1994/10/25  16:09:15  proulx]
 * 
 * Revision 1.1.7.9  1994/09/07  20:02:07  mccann
 * 	   fix version handling OT:12004
 * 	   [1994/09/07  18:41:25  mccann]
 * 
 * Revision 1.1.7.8  1994/08/26  21:55:22  zee
 * 	Add check of UpgradeTo of pseudo directory in background.  Also add check
 * 	for root directory version at 4.0 and set the UpgradeTo attribute on all
 * 	masters stored here.
 * 	[1994/08/26  21:48:13  zee]
 * 
 * Revision 1.1.7.7  1994/08/16  20:04:32  jd
 * 	11676 From shu@hp. Check for NULL stream_p->db_dir.data_p before calling
 * 	do_skulk in background().
 * 	[1994/08/13  18:28:08  jd]
 * 
 * Revision 1.1.7.6  1994/08/03  19:03:29  mccann
 * 	final sams drop
 * 	[1994/08/02  14:08:50  mccann]
 * 
 * Revision 1.1.7.5  1994/08/01  15:07:30  zee
 * 	     If set, reset file_p->file_back_force to zero in background.  Also,
 * 	     broadcast on file_back_done condition variable.
 * 	[1994/07/29  21:03:44  zee]
 * 
 * Revision 1.1.7.4  1994/06/09  18:42:47  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:03  devsrc]
 * 
 * Revision 1.1.7.3  1994/04/14  14:51:03  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:18:31  peckham]
 * 
 * Revision 1.1.7.2  1994/03/22  20:00:16  griffin
 * 	HCell BL3 support: Add support for detecting a new (preferred)
 * 	cell name and transforming the current cell to use the new
 * 	name.
 * 	[1994/03/22  17:59:52  griffin]
 * 
 * Revision 1.1.7.1  1994/03/12  22:09:50  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:37:50  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:18:58  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:19:05  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:26:58  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:34:01  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module back_ground.c
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
 * Abstract:
 *	All functions associated with the BACKGROUND process of the server.
 *
 */

#include <server.h>
#include <stdio.h>
#include <dce/assert.h>
#include <errno.h>

#include <back.h>
#include <dbdef.h>
#include <find.h>
#include <events.h>
#include <names.h>
#include <replica.h>
#include <sets.h>
#include <tlog.h>
#include <uid.h>
#include <hcell.h>

extern time_quad_t bkgdonehour;
extern utc_t       bkgdonehour_utc;
extern time_quad_t bkgdskulk;
extern time_quad_t bkgdskulkhalf;
extern time_quad_t bkgdtimer;

/*
 * A background thread runs on a timer to force directory skulks
 *
 * Some items are included here because the entire structure
 * is zeroed out at the beginning anyway.
 */
typedef struct back_dsc
    {
    utc_t               tm_now_utc;             /* time background being run */
    utc_t               tm_then_utc;            /* next invocation */
    utc_t               tm_tickle_utc;          /* first active after this */
    utc_t               tm_hour_utc;            /* then activation is hourly */
    utc_t               tm_skulk_utc;           /* skulk time */
    utc_t               tm_skulkhalf_utc;       /* half the skulk time */
    reltimespec_t       tm_reltimespec;         /* reltimespec work area */
    timespec_t          tm_inacc;               /* inaccuracy work area */
    file_info_t         *tm_file_p;
    long                tm_tdf;                 /* timezone work area */
    int                 tm_chlevel;
    int                 tm_uppointer_cnt;
    short               tm_roothere;
    char                tm_relocate;            /* force childPointer updates */
    char                tm_relocate_failed;     /* to schedule retry */
    char                tm_do_skulk;
    } back_dsc_t;

static timespec_t zero_inacc = {0};

static char *
deb_ascii_back_dsc (
    char *const         ,
    const void *const   );
#define DEB_ASCII_LEN_back_dsc DEB_ASCII_LEN_file_info
#define DEB_ASCII_BUF_back_dsc(nam) \
        DEB_ASCII_BUF(nam,DEB_ASCII_LEN_back_dsc)
#define LOG_ASCII_BUF_back_dsc(nam) \
        LOG_ASCII_BUF(nam,DEB_ASCII_LEN_back_dsc)

static char *
deb_ascii_back_dsc(char *const          buf,
                   const void *const    ptr)
{
    const back_dsc_t *const tm_p = ptr;

    if (tm_p)
        return deb_ascii_file_info(buf, tm_p->tm_file_p);
    else
        return("NULL_back_dsc");
}

/*
 * Local prototypes
 */
static dns_status_t
check_master_replica_ptr (
    back_dsc_t          *,
    db_stream_t         *,
    Set_t               *);

static void
back_on (
    db_stream_t		*,
    back_dsc_t		*,
    int		      	);

static dns_status_t
back_gdapointer_check (
    db_stream_t		*,
    gdaPointer_t	*,
    utc_t		*);


static void
force_upgrade(
    db_stream_t *stream_p);

static void
do_skulk (
    int			,
    db_stream_t		*,
    back_dsc_t		*);

static dthread_address_t
background (
    back_dsc_t		*);

static void
background_unlock (
    dthread_mutex_t	*);

static int
towerset_compare_exact_mx (
    DBSet_t             *,
    Set_t               *);

/*
 * check_master_replica_ptr
 */
static dns_status_t
check_master_replica_ptr (tm_p, stream_p, mytower_p)
    back_dsc_t          *tm_p;
    db_stream_t         *stream_p;
    Set_t               *mytower_p;
{
    DEB_ASCII_BUF_back_dsc(tmBuf)
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_handle handle = cdsGetHandle();
    dns_status_t status = CDS_ERROR;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >check_master_replica_ptr(%s,%s,)",
        deb_ascii_back_dsc(tmBuf,tm_p),
        deb_ascii_db_stream(streamBuf,stream_p)));

    if (handle)
        {
        dir_hash_entry_t *const hash_p = stream_p->db_dir_hash_p;
        nsgbl_ch_t *const ch_p = stream_p->db_file_p->file_ch_p;
        unsigned char *replica_p = NULL;        /* master replica */
        cdsFlagStat_t flags;

        ZERO_bytes(&flags, sizeof(flags));
        flags.fsConf = cdsConfHi;
        flags.fsDontCache = TRUE;
        flags.fsNameType = cdsOpqDNA;

        /*
         * GO through the CDS_REPLICAS attribute for the directory
         */
        for (;;)
            {
            cds_attrvalue_t *val_p;

            status = cdsReadAttrValue(
                (cds_full_name_t *)hash_p->dir_name,
                handle,
                (cds_attr_name_t *)dns_AttributeName(DNS_REPLICAS),
                cdsDir,
                NULL,
                &val_p, NULL,
                NULL, NULL,
                &flags);

            if (status == CDS_SUCCESS)
                {
                DEB_ASCII_BUF_FullName(chnameBuf)
                ReplicaPointer_t *const rp_p =
                            (ReplicaPointer_t *)val_p->val_u.val_byte.byte_p;
                dce_assert(cds__svc_handle,
                    (val_p->val_syntax == cds_ReplicaPointer));
                dce_assert(cds__svc_handle,
                    (LEN_ReplicaPointer(rp_p) == val_p->val_u.val_byte.length));

                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug3,
                    "  check_master_replica_ptr(%s,%s,) clearinghouse(%s)",
                    deb_ascii_back_dsc(tmBuf,tm_p),
                    deb_ascii_db_stream(streamBuf,stream_p),
                    deb_ascii_FullName(chnameBuf,FLD_ReplicaPointer_CHName(rp_p)
)));

                /*
                 * If this is our clearinghouse,
                 * check if the address is correct.
                 */
                if (EQ_ObjUID(rp_p->rp_CHID, &ch_p->ch_uid))
                    {
                    Set_t *const st_p =
                            (Set_t *)dns_malloc(LEN_Set(rp_p->rp_addressHint));

                    if (st_p)
                        {
                        int match;

                        COPY_Set(rp_p->rp_addressHint, st_p);
                        match = towerset_compare_exact(mytower_p, st_p);
                        dns_free((char *)st_p);

                        if (match)
                            break;
                        }

                        DCE_SVC_DEBUG((
                            cds__svc_handle,
                            cds_svc_server,
                            svc_c_debug3,
                            "  check_master_replica_ptr(%s,%s,) towers do not match",
                            deb_ascii_back_dsc(tmBuf,tm_p),
                            deb_ascii_db_stream(streamBuf,stream_p)));
                    }
                /*
                 * Otherwise get the ReplicaPointer for the master.
                 */
                else if (EXT8(FLD_ReplicaPointer_replicaType(rp_p)) == RT_master)
                    {
                    DEB_ASCII_BUF_FullName(chnameBuf)
                    const int len = val_p->val_u.val_byte.length;

                    DCE_SVC_DEBUG((
                        cds__svc_handle,
                        cds_svc_server,
                        svc_c_debug3,
                        "  check_master_replica_ptr(%s,%s,) found master(%s)",
                        deb_ascii_back_dsc(tmBuf,tm_p),
                        deb_ascii_db_stream(streamBuf,stream_p),
                        deb_ascii_FullName(chnameBuf,
                                           FLD_ReplicaPointer_CHName(rp_p))));

                    replica_p = (unsigned char *)dns_malloc(len);
                    if (!replica_p)
                        {
                        status = CDS_NONSRESOURCES;
                        break;
                        }

                    COPY_bytes(rp_p, replica_p, len);
                    }
                }
            else
                break;
            }

        /*
         * If we went through the entire set, then we didn't
         * find our ReplicaPointer with the right address.
         * In that case, we should have gleaned the master
         * from the set, and we can update his CDS_REPLICAS.
         *
         * If we hit a correct ReplicaPointer, it will drop out
         * of the previous loop with CDS_SUCCESS.
         */
        if (status == CDS_EMPTY)        /* all SetMembers read */
            if (replica_p)
                {
                Update_u update;
                AtomicValue_t *const av_p =
                    (AtomicValue_t *)SKIP_bytes(update, update_dns_build(
                    (Update_t *)update, UD_present, NULL,
                    dns_Attribute(DNS_REPLICAS), BL_true, 0, NULL));

                INS16(av_p->av_length, replica_build(
                    (ReplicaPointer_t *)av_p->av_value,
                    &ch_p->ch_uid, mytower_p,
                    db_dir_get_replica_type(stream_p->db_dir_hash_p),
                    &stream_p->db_file_p->file_ch_p->ch_name));

                status = cdsModifyReplica(
                    (dns_id *)stream_p->db_dir.data_uid,
                    replica_p, LEN_ReplicaPointer(replica_p),
                    update, LEN_Update(update),
                    &flags);

                /*
                 * If we did the change, make sure a skulk is done.
                 */
                if (status == CDS_SUCCESS)
                    tm_p->tm_do_skulk = TRUE;
                }
            else
                /* missing master replica */
                status = CDS_NAMESERVERBUG;

        if (replica_p)
            dns_free((char *)replica_p);

        cdsFreeHandle(handle);
        }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <check_master_replica_ptr(%s,%s,) return(%s)",
        deb_ascii_back_dsc(tmBuf,tm_p),
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * back_on
 *     	This routine checks parentpointer and gdapointer timeouts.
 * 
 * Input:
 *  	stream_p = pointer to stream descriptor
 *	tm_p = pointer to background descriptor
 *	rptype = type of replica
 */
static void 
back_on (db_stream_t  *stream_p,
         back_dsc_t   *tm_p,
         int          rptype)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DBSet_t		*set_p;			/* current set in use  */
    ReplicaPointer_t	*rp_p;			/* ptr to replicas attribute */
    int			len, 			/* Len of replicas attribute */
			cnt, 			/* count of elements in set  */
			index;			/* index into set  */
    dns_status_t	status;			/* Current status  */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >back_on(%s,,%d)",
        deb_ascii_db_stream(streamBuf,stream_p), rptype));

    /* 
     * If this is the root directory, don't need to continue.
     * Root directory doesn't have a corresponding child pointer to check
     */
    if (find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_PARENTPOINTER),
		&set_p, &index) == DNS_SUCCESS)
	/* Find parent pointer time out */
        {
        /* Point at this directories parent pointer */
	DBSetMember_t *const av_p = find_last_value(set_p);

        if (av_p) 
            {
	    ParentPointer_t *pname_p = (ParentPointer_t *)av_p->dm_data;

            if (rptype == RT_master)
	        {
		Timeout_t *const timeout_p = (Timeout_t *)pname_p->pp_timeout;

		/*
		 * If this is the master and the parent pointer has expired,
		 * update it 
		 */        
                if (!GT_utc(timeout_p->to_expiration,
			    (byte_t *)&tm_p->tm_then_utc)
		    || tm_p->tm_relocate)
                    {
                    status = back_parentpointer_check(stream_p,
						      &tm_p->tm_now_utc);
		    if (status != DNS_SUCCESS)
			tm_p->tm_relocate_failed = TRUE;
                    }
		} /* End if replica type is master */
            } /* End if parentpointer data is not null */
        } /* End if found parentpointer attribute */
    else
	{
	tm_p->tm_roothere = TRUE;
	/* check GDAPointers */
	if (stream_p->db_dir_hash_p->dir_root
		&& (find_attribute(stream_p->db_dir.data_p,
			dns_AttributeName(DNS_GDAPOINTERS), &set_p, &index)
				== DNS_SUCCESS))
	    {
	    /* Save out buffer in case gdaPointers are updated */
	    data_record_t *save_data_p = stream_p->db_dir.data_p;
	    stream_p->db_dir.data_p = NULL;

	    cnt = NUM_DBSet(set_p);
	    for (index = 0; index < cnt; ++index)
		{
		DBSetMember_t *const av_p =
				(DBSetMember_t *)INDEX_DBSet(set_p, index);
		gdaPointer_t *gp_p;

		if (!EXT8(av_p->dm_valid) || !EXT8(av_p->dm_value_present))
		    continue;
		gp_p = (gdaPointer_t *)av_p->dm_data;
		if (rptype == RT_master)
		    {
		    Timeout_t *const to_p = (Timeout_t *)gp_p->gp_timeout;

		    /*
		     * If this is the master and the GDAPointer has expired,
		     * check if GDA still exists.
		     */        
		    if (!GT_utc(to_p->to_expiration,
				(byte_t *)&tm_p->tm_then_utc))
			{
			if (back_gdapointer_check(stream_p, gp_p,
				&tm_p->tm_now_utc) != DNS_SUCCESS)
			    continue;
			}
		    } /* End if replica type is master */
		} /* End loop through set of gdaPointers */
	    /*
	     * If modifications have happened to gdaPointers,
	     * the directory will have been re-read and modified.
	     * Toss our current buffer if that has happened,
	     * otherwise re-instate the buffer we have been using.
	     */
	    if (stream_p->db_dir.data_p) 
		dns_free((char *)save_data_p);
	    else
	        stream_p->db_dir.data_p = save_data_p;
	    } /* End found dns_gdaointer attribute */
	} /* End did not find parentpointer attribute */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <back_on(%s,,%d)",
        deb_ascii_db_stream(streamBuf,stream_p), rptype));
}

/*
 * back_gdapointer_check
 *	This routine checks if the GDA is still alive
 *	by trying a ModifyReplica() on it. If it is there,
 *	we will get a DNS_NOTIMPLEMENTED.
 *
 * Input:
 *     stream_p = pointer to stream descriptor
 *     gp_p = gdaPointer being checked
 *     now_p = address of time to be used to update
 *             the GDAPointer expiration
 *
 */

static dns_status_t 
back_gdapointer_check (db_stream_t   *stream_p,
                       gdaPointer_t  *gp_p,
                       utc_t         *now_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    static dns_id	NullID;	/* guaranteed to be zero by C */
    static byte_t	NullUpdate[] = {
				UD_present,
				INITIALIZE_Timestamp,
				AT_null,
				BL_false
			};
    tlog_dsc_t		tlog;
    dnsFlagStat_t	flags;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >back_gdapointer_check(%s,,)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    ZERO_bytes(&flags, sizeof(flags));

    status = cdsModifyReplica(&NullID,
		gp_p->gp_replica, LEN_ReplicaPointer(gp_p->gp_replica),
		NullUpdate, sizeof(NullUpdate), &flags);
    dce_assert(cds__svc_handle,
	       (status != DNS_SUCCESS));

    /* make sure all is clear */
    db_dir_release(stream_p);

    status = db_dir_read_for_update(stream_p);
    if (status != DNS_SUCCESS)
	goto leave_back_gdapointer_check;

    if (flags.fsLocStat != DNS_NOTIMPLEMENTED)
	status = flags.fsLocStat;

    tlog.cnt = 1;
    tlog.len = 0;
    tlog.buf_p = (byte_t *)dns_malloc((sizeof(Update_u) - MAXATTRIBUTE)
    			+ LEN_gdaPointer(gp_p));
    if (!tlog.buf_p)
        {
        status = DNS_NONSRESOURCES;
        goto leave_back_gdapointer_check;
        }

    tlog.len = update_dns_build((Update_t *)tlog.buf_p,
		((status == DNS_SUCCESS) ? UD_present : UD_absent),
		NULL, dns_Attribute(DNS_GDAPOINTERS),
		BL_true, LEN_gdaPointer(gp_p), (byte_t *)gp_p);

    if (status == DNS_SUCCESS)
	{
	/* Get pointer to av_value */
	gdaPointer_t *const gp2_p =
		(gdaPointer_t *)&tlog.buf_p[ tlog.len - LEN_gdaPointer(gp_p) ];

	/* Extend the timeout */
	Timeout_t *const to_p = (Timeout_t *)gp2_p->gp_timeout;

	ADD_utc((byte_t *)now_p, to_p->to_extension, to_p->to_expiration);
	}

    (void)set_insert(&stream_p->db_dir, (Update_t *)tlog.buf_p,
		dns_Attribute(DNS_GDAPOINTERS), TRUE);
    /* Update timestamps */
    db_dir_set_lastupdate(stream_p->db_dir_hash_p,
		(Timestamp_t *)((Update_t *)tlog.buf_p)->ud_timestamp);
    (void)set_timestamp_set(&stream_p->db_dir, 
		(Timestamp_t *)((Update_t *)tlog.buf_p)->ud_timestamp,
		dns_Attribute(DNS_UTS), NULL, NULL);

    (void)db_dir_update(stream_p, &tlog);
    /* Exit with new copy of directory in stream */
    dns_free(tlog.buf_p);
    db_dir_release_lock(stream_p);

leave_back_gdapointer_check:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <back_gdapointer_check(%s,,) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * Force upgrade version to 4.0 for both the pseudo directory and 
 * directories.
 */
static void
force_upgrade(db_stream_t *stream_p)
{

  version_t dir_version;
  VersionNumber_t upgrade_version;
  Update_u update_block;
  int index;
  DBSet_t *set_p;

  /* If upgrade attribute not already present, add it */
  if (find_attribute(stream_p->db_dir.data_p,
		     dns_AttributeName(DNS_UPGRADETO),
		     &set_p, &index) != CDS_SUCCESS) {
    db_dir_get_version (stream_p, &dir_version);
    if (EXT8(dir_version.vr_major) < 4) {

      /* add DNS_UpgradeTo attribute = 4.0 */
      INS8(upgrade_version.vn_major, 4);
      INS8(upgrade_version.vn_minor, 0);
      update_dns_build((Update_t *)update_block,
				  UD_present,
				  NULL,
				  dns_Attribute(DNS_UPGRADETO),
				  BL_true,
				  LEN_VersionNumber(&upgrade_version),
				  (byte_t *)&upgrade_version);
      set_insert(&stream_p->db_dir,
		 (Update_t *)update_block,
		 dns_Attribute(DNS_UPGRADETO),
		 FALSE);
    }
  }

}
                      
/*
 * do_skulk
 *     This routine determines whether a skulk is needed for this
 * 	directory.  
 *
 * Input:
 *	state = directory state
 *	stream_p = pointer to stream descriptor
 *	tm_p = pointer to background descriptor
 */
static void 
do_skulk (int          state,
          db_stream_t  *stream_p,
          back_dsc_t   *tm_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_back_dsc(tmBuf)
    dir_hash_entry_t *const hash_p = stream_p->db_dir_hash_p;
    DBSet_t		*set_p;
    int			index;
    dns_status_t	status;
    unsigned int	do_schedule_skulk;	

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug3,
	" >do_skulk(%d,%s,%s)",
	state, deb_ascii_db_stream(streamBuf,stream_p),
	deb_ascii_back_dsc(tmBuf,tm_p)));

    /* 
     * Only check convergence if directory is on, 
     * otherwise always force a skulk
     */

    if ((state == RS_on) && !tm_p->tm_do_skulk)
        {
	status = find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_LASTSKULK), &set_p, &index);
	if (status == DNS_SUCCESS)
	    {
	    time_quad_t lastskulk;
	    time_local_t lastskulk_local;
	    utc_t lastskulk_utc;
	    utc_t sinceskulk;
	    DBSetMember_t *const dm_p = (DBSetMember_t *)DATA_DBSet(set_p);
	    Timestamp_t *const lastskulk_p = (Timestamp_t *)dm_p->dm_data;
	    enum utc_cmptype diff = utc_greaterThan;
	    byte_t rpconvergence = CG_high;

	    /*
	     * For comparison: get time since last skulk
	     */
	    COPY_Time(lastskulk_p->ts_time, &lastskulk); /* aligned */
	    sys_time_from_utc(&lastskulk_local, &lastskulk);
	    status = utc_mkbintime(&lastskulk_utc,
				   &lastskulk_local, &zero_inacc, 0);
	    dce_assert(cds__svc_handle, (0 <= status));
	    status = utc_subtime(&sinceskulk,
				 &tm_p->tm_then_utc, &lastskulk_utc);
	    dce_assert(cds__svc_handle, (0 <= status));

	    /*
	     * Get the real convergence, if we can.
	     */
	    status = find_attribute(stream_p->db_dir.data_p,
				    dns_AttributeName(DNS_CONVERGENCE),
				    &set_p, &index);
	    if (status == CDS_SUCCESS)
		{
		DBSetMember_t *const dm_p = (DBSetMember_t *)DATA_DBSet(set_p);

		rpconvergence = EXT8(dm_p->dm_data);
		}

	    /*
	     * Now see whether enough time has passed
	     * to warrant a skulk for the given convergence.
	     */
	    switch (rpconvergence)
	    {
	    case CG_low:		/* For low priority convergence  */
		status = utc_cmpmidtime(&diff,
					&sinceskulk, &tm_p->tm_skulk_utc);
		dce_assert(cds__svc_handle, (0 <= status));
		break;
	    case CG_high:		/* For high priority convergence  */
		/*
		 * Note that this code will be executed hourly (see
		 * background_activator), but a skulk will occur only
		 * if we've had an unsuccessful propagate or skulk since
		 * the last transaction
		 */
		if ((hash_p->dir_prop_status != CDS_SUCCESS) ||
		    (hash_p->dir_skulk_last_status != CDS_SUCCESS))
		    break;

		/* Fall through */
	    case CG_medium:		/* For medium priority convergence  */
		status = utc_cmpmidtime(&diff,
					&sinceskulk, &tm_p->tm_skulkhalf_utc);
		dce_assert(cds__svc_handle, (0 <= status));
		break;
	    } /* end of case */

	    if (diff == utc_lessThan)		/* If not time for a skulk  */
		goto leave_do_skulk;		/*  continue.  */
	    }
	} /* end of if (state == RS_on) */

    /* Make thread look like an rpc manager so rpc_ss_allocate will work */
    rpc_ss_enable_allocate();
    (void)skulk_sched(stream_p);			/* Run a skulk. */
    rpc_ss_disable_allocate();

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
	"  do_skulk(%d,%s,%s) SkulkLastStatus(%d)",
	state, deb_ascii_db_stream(streamBuf,stream_p),
	deb_ascii_back_dsc(tmBuf,tm_p),
        hash_p->dir_skulk_last_status));
 
   db_dir_release(stream_p);
   if ((status = db_dir_read( stream_p))      /* If couldn't read */
		!= DNS_SUCCESS)		      /* probably directory gone.   */
	{				      /* Force a skulk next time    */
	hash_p->dir_prop_status = status;     /* enter this routine on high */
	goto leave_do_skulk;		      /* convergence, now, return.  */
	}
    if ((status = find_attribute(	      /* Get time last skulk began. */
		stream_p->db_dir.data_p,
		dns_AttributeName(DNS_LASTSKULK),
		&set_p, &index)) == DNS_SUCCESS)
        {
        DBSetMember_t *const dm_p = (DBSetMember_t *)DATA_DBSet(set_p);
        dns_cts *const lastskulk_p = (dns_cts *)dm_p->dm_data;

	if (cdsCmpCTS( lastskulk_p, (dns_cts *)hash_p->dir_prop_time) > 0)
	    {					/* than propagate  */
	    hash_p->dir_prop_status = 		/*  reset propagate status.  */
		    hash_p->dir_skulk_last_status;
	    }
        }
    else
        {
        dns_cts *const lastskulk_p = (dns_cts *)NullTimestamp;

	if (cdsCmpCTS(lastskulk_p, (dns_cts *)hash_p->dir_prop_time) > 0)
	    {					/* than propagate  */
	    hash_p->dir_prop_status = 		/*  reset propagate status.  */
		    hash_p->dir_skulk_last_status;
	    }
        }

leave_do_skulk:

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug3,
	" <do_skulk(%d,%s,%s)",
	state, deb_ascii_db_stream(streamBuf,stream_p),
	deb_ascii_back_dsc(tmBuf,tm_p)));
}

/*
 * background 
 *
 * This routine is scheduled to run on a timer.  A thread is
 * is forked when the first clearinghouse is enabled.  It
 * reads through all directories, skulks them if necessary,
 * and checks parent pointers.  When it finishes a new timer is set.
 *
 * Input:
 *    file_p (in/out) - Pointer to clearinghouse file information
 *
 * Returns:
 *	2	- to reschedule
 *	1 	- file not open
 *	0	- otherwise
 *	or aborts process
 *
 */
static dthread_address_t 
background (back_dsc_t *tm_p)
{
    DEB_ASCII_BUF_back_dsc(tmBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    bytes_u             mytower[READ_BUF_SIZE];
    file_info_t         *const file_p = tm_p->tm_file_p;
    nsgbl_ch_t          *const ch_p = file_p->file_ch_p;
    DBSet_t		*set_p, *chset_p;
    data_record_t	*pseudo_buf_p;		 /* Save pseudo data buffer */
    db_stream_t		*stream_p;		 /* Working stream ptr  */
    int			index, pos;		 /* Working indices  */
    int                 back_status = 0;
    dns_status_t	status;			 /* Current status  */
    int thstatus;

    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >background(%s) - file state is %d",
        deb_ascii_back_dsc(tmBuf,tm_p),
        file_p->file_state));

    /* If background should not run, return */
    SERVER_LOCK(file_p->file_mutex)
      /* Beginning of the 'protected code' */
      {
	if (file_p->file_state != file_open) {
	  back_status = 1;
	}
      }
      /* End of the 'protected code' */

    SERVER_END_LOCK(file_p->file_mutex)

    if (back_status != 0) {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            "  background(%s) - signalling death (file not open)",
             deb_ascii_back_dsc(tmBuf,tm_p)));

        goto leave_background;
    }

    /* Get stream for modify so writers count != 0 and checkpoint must wait */
    status = db_open_stream((ObjUID_t *)&ch_p->ch_uid,
			    &stream_p, OP_ModifyAttribute);

    /* If this fails, reschedule request */
    if (status != DNS_SUCCESS) {
      DCE_SVC_DEBUG((
		     cds__svc_handle,
		     cds_svc_server,
		     svc_c_debug5,
		     "  background(%s): Rescheduling (db_open_stream)",
		     deb_ascii_back_dsc(tmBuf,tm_p)));
      
      back_status = 2;
      goto leave_background;
    }

    tm_p->tm_roothere = FALSE;

    /* Get pseudo directory so we can locate list of directories */
    status = db_pseudo_read_for_update(stream_p);
    if (status != DNS_SUCCESS) {
      DCE_SVC_DEBUG((
		     cds__svc_handle,
		     cds_svc_server,
		     svc_c_debug5,
		     "  background(%s): Rescheduling (db_pseudo_read)",
		     deb_ascii_back_dsc(tmBuf,tm_p)));
      
      back_status = 3;
      goto close_stream;
    }


    /*
     * If root directory is at 4.0, upgrade clearinghouse automatically.
     * Note this is the first step, this change gets passed into
     * pseudodir_update and written then.  Don't have to log it,
     * we can re-create. - the directory version will get
     * upgraded after pseudodir_update when allupto has been advanced.
     *
     * Test the "present" condition first, in case we're in the middle
     * of creating this clearinghouse.
     */
    if (file_p->file_root_dir_present) {
	if ((EXT8(file_p->file_root_dir_p->dir_version_p->vn_major) == 4) &&
	    (EXT8(file_p->file_root_dir_p->dir_version_p->vn_minor) == 0)) {
	    force_upgrade(stream_p);
	}
    }

    /* Get list of directories in this clearinghouse */
    status = find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_CHDIRECTORIES), &chset_p, &index);
    if (status != DNS_SUCCESS) {
      DCE_SVC_DEBUG((
		     cds__svc_handle,
		     cds_svc_server,
		     svc_c_debug5,
		     "  background 1(%s): Rescheduling (find_attribute)",
		     deb_ascii_back_dsc(tmBuf,tm_p)));
      
      back_status = 4;
      goto close_stream;
    }

    /* Read CHDirectories attribute and update if needed. */
    status = back_pseudodir_updates( stream_p, file_p, chset_p);
    if (status != DNS_SUCCESS)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            "  background(%s) failed (%d) to update pseudo directories.",
            deb_ascii_back_dsc(tmBuf,tm_p),
            deb_ascii_dns_status(statusBuf,status)));
    } else {
	/*
	 * Check for existence of DNS_UPGRADETO attribute on pseudo directory
	 * and if so, upgrade the directory version.  Do after allupto
	 * has moved forward.
	 */
	status = find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_UPGRADETO), &set_p, &index);
	if (status == DNS_SUCCESS) {
	  VersionNumber_t  server_version;

	  status = db_pseudo_read_for_update(stream_p);
	  if (status != DNS_SUCCESS) {
	    DCE_SVC_DEBUG((
			   cds__svc_handle,
			   cds_svc_server,
			   svc_c_debug5,
			   "  background(%s): Rescheduling (db_pseudo_read)",
			   deb_ascii_back_dsc(tmBuf,tm_p)));
	    back_status = 3;
	    goto close_stream;
	  }

	  get_server_version(&server_version);
	  back_upgrade_directory_actions(stream_p, &server_version,
					 ET_clearinghouse);
	}
      }


    /* See if address of this clearinghouse has changed */
    if (tm_p->tm_relocate)
    {
	nsgbl_ns_t *const ns_p = ch_p->ch_nameserver;

	if (0 < cds_LocalTowers(ns_p->ns_bind_vector, mytower)) {
	    /* Make sure chlastaddress is up to date */
	    status = find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_CHLASTADDRESS), &set_p, &index);
	    if (status == DNS_SUCCESS)
	    {
		/* do a comparison - destroying mytower */
		if (towerset_compare_exact_mx (set_p, (Set_t *)mytower))
		{
		    tm_p->tm_relocate = FALSE;
		}

		/* reconstruct destroyed tower */
		cds_LocalTowers(ns_p->ns_bind_vector, mytower);
	    }
	    else
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_server,
		    svc_c_debug3,
		    "E background(%s): Could not find DNS$CHLastAddress: %s",
		    deb_ascii_back_dsc(tmBuf,tm_p),
		    deb_ascii_dns_status(statusBuf,status)));
	    }
	    if (tm_p->tm_relocate && !net_re_export_ch(ch_p))
	    {
		tm_p->tm_relocate_failed = TRUE;
	    } 
	    else 
	      if (tm_p->tm_relocate) { /* JPF added else for skulk */
		cdsFlagStat_t flags;
		
		ZERO_bytes(&flags, sizeof(flags));
		flags.fsConf = cdsConfHi;
		flags.fsNameType = cdsOpqDNA;
		
		(void) cdsSkulkDir((cds_full_name_t *)file_p->file_root_dir_p->dir_name, &flags);
	      }
	} else
	  tm_p->tm_relocate_failed = TRUE;
    }

    /* May have modified list, so must get new/changed list of directories 
     * in this clearinghouse.
     */
    status = find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_CHDIRECTORIES), &chset_p, &index);
    if (status != DNS_SUCCESS)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            "  background 2(%s): Rescheduling (find_attribute)",
            deb_ascii_back_dsc(tmBuf,tm_p)));

	back_status = 5;
	goto close_stream;
    }

    /* Save pseudo data buffer so we can re-use stream to read directories */
    pseudo_buf_p = stream_p->db_dir.data_p;
    stream_p->db_dir.data_p = NULL;
 
    /* Loop through list of directories */
    for (pos = 0; pos < NUM_DBSet(chset_p); pos++)
	{
	DEB_ASCII_BUF_FullName(dirnameBuf)
	DBSetMember_t *value_p = (DBSetMember_t *)INDEX_DBSet(chset_p, pos);
	byte_t rpstate, rptype;

	if (!EXT8(value_p->dm_valid)) continue;	    

	tm_p->tm_do_skulk = FALSE;
        /*
         * Here we check in on the clearinghouse state.
         * While we've been merrily running through the list of directories
         * mgmt may have disabled this clearinhouse and this thread is
         * still running.   If so, we want to stop what we are doing and
         * let the master control determine what should
         * happen.  If they decide to kill us off, so be it -- we don't
         * decide this here.  Instead we just reschedule the background
         * thread, which will recheck the pre-conditions.
         */
	if (ch_p->ch_state != dns_On) 
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug5,
                "  background(%s): has noted that CH state != ON (%d)...",
                deb_ascii_back_dsc(tmBuf,tm_p),
                ch_p->ch_state));

            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug5,
                "  background(%s): Rescheduling (directory loop)",
                deb_ascii_back_dsc(tmBuf,tm_p)));

	    back_status = 6;
	    goto free_pseudo;
	}

   	/* 
         * Build key to read directory 
         * 
         * The value from the set is a architecturally a CHDirectoryPointer, 
         * which consists of the directory ID followed by the fullname 
         * of the directory.  The database only stores the ID.
         */
	names_new((ObjUID_t *)value_p->dm_data, NULL,
		(FullName_t *)stream_p->db_key);

	stream_p->db_dir_hash_p = NULL;
	status = db_dir_read_for_update(stream_p);
	/* 
	 * Remove directory ID from DNS_CHDIRECTORIES attribute
	 * if could not find the directory entry in the clearinghouse.
	 */
  	if (status != DNS_SUCCESS) 
	{					 /* Remove directory  */
	    (void)db_pseudo_chdirectory(stream_p->db_file_p, 
				  (ObjUID_t *)value_p->dm_data, UD_absent);
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug5,
                "  background(%s) removing directory from CHDIRECTORIES.",
                deb_ascii_back_dsc(tmBuf,tm_p)));

	    continue;
	}					 /* Removed directory  */

   	rptype = db_dir_get_replica_type(stream_p->db_dir_hash_p);
   	rpstate = db_dir_get_replica_state(stream_p->db_dir_hash_p);

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug2,
            "  background(%s) directory(%s) type(%d) state(%d)",
            deb_ascii_back_dsc(tmBuf,tm_p),
            deb_ascii_FullName(dirnameBuf,stream_p->db_dir_hash_p->dir_name),
            rptype, rpstate));

	/* 
	 * Any new states mean the system may have crashed before 
	 * the operation completed.  Need to check this out now 
 	 * and either complete it or backout the directory.
	 */
	if (rpstate == RS_newDir)
	{
	    /* Let skulk_newdir check everything out */
	}
    	else if (rpstate == RS_newReplica)
	{
	    /* See if replica was linked correctly */
	    ta_add_replica_check(stream_p);

	    /* If hash_p is null, link failed and directory was backed out */
	    if (!stream_p->db_dir_hash_p)
		continue;

	    /* if clearinghouse address changed, make sure to do a skulk */
	    if (tm_p->tm_relocate)
		tm_p->tm_do_skulk = TRUE;
	}
	else if (find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_ALLUPTO),
		&set_p, &index) == DNS_SUCCESS)
	{
	  DBSetMember_t *dm_p = (DBSetMember_t *)DATA_DBSet(set_p);

	  /* Remove deleted directory data from record */
	  db_cleanup_record(&stream_p->db_dir, (Timestamp_t *)dm_p->dm_data);

	  /*
	   * Check to see if the root directory version is at 4.0.
	   * If so, then the administrators have determined that they
	   * wish to use new functionality with OSF DCE V1.1, so
	   * set the UpgradeTo = 4.0 on all directories mastered here
	   * if their DirectoryVersion is not already at 4.0.
	   */
	  if (rptype == RT_master &&
	      (EXT8(file_p->file_root_dir_p->dir_version_p->vn_major) == 4) &&
	      (EXT8(file_p->file_root_dir_p->dir_version_p->vn_minor) == 0)
	      ) {

	    force_upgrade(stream_p);
	  }

	  /* Record in database.  Log not needed, this can be redone. */
	  (void)db_dir_update(stream_p, NULL);	
	}

	/*
	 * Check our own address to make sure we're still where
	 * we were the last time around
	 */
	if (tm_p->tm_relocate)
	{
	    if (rptype == RT_master)
	    {
		tlog_dsc_t tlog;
		Update_u update;

		tlog.cnt = 1;
		tlog.buf_p = update;

		/*
		 * If the lock has already been released above
		 * (usual case), get it again.
		 */
		if (stream_p->db_dir.data_hash_p == NULL) {
		   (void)db_dir_read_for_update(stream_p);
		}

		/*
		 * Did we move since updating the replica ptr?
		 */
		if (check_own_replica_ptr (stream_p, (Set_t *)mytower,
					   (Update_t *)update, &tlog.len))
		{
		  /*
		   * If returns true, we moved otherwise we didn't.
		   */
		  status = db_dir_update(stream_p, &tlog);
		  
		  if (status == CDS_SUCCESS)
		    tm_p->tm_do_skulk = TRUE;
		  else
		    tm_p->tm_relocate_failed = TRUE;
	       }
	    }
	    else
	    {
	       /*
		* Check with the master of this directory to make
		* sure it's got an up to date replica pointer.
		*/
		status = check_master_replica_ptr(tm_p, stream_p,
						  (Set_t *)mytower);
		if (status != CDS_SUCCESS)
		    tm_p->tm_relocate_failed = TRUE;
	    }
	}

        /* Make sure update locks are not still held */
  	db_dir_release_lock(stream_p);
   	/*
	 * The only state of interest is ON directories.  Dead
 	 * directories are cleaned out when the clearinghouse is
	 * is enabled to guarentee that no outstanding pointers
	 * to the hash tables exist
	 */
        if (rpstate == RS_on)
	{
	    back_on(stream_p, tm_p, rptype);

	     /* Did check parentpointer have an error ? */
	    if (!stream_p->db_dir.data_p)
	        continue;

            /*
             * Check to see if we should upgrade this replica and
             * perform the upgrade if all the conditions are met.
             * The result of this operation is irrelevant to the immediate
             * code stream.
             */
            back_upgrade_replica_actions(stream_p);
	}

	/* Do a skulk if necessary */
        if ((rptype == RT_master) &&
	    ((rpstate == RS_on) || (rpstate == RS_newDir) ||
	     (rpstate == RS_dyingDir) ))
        {	
            /* Skip do_skulk() if the associated directory data
              has been released for any reason.  Otherwise
              function do_skulk() may cause a segmentation
              violation due to invalid null pointer data_p */

            if (!stream_p->db_dir.data_p)
                 continue;
            do_skulk(rpstate, stream_p, tm_p);
        }
        else if (tm_p->tm_do_skulk)
        {
            dir_hash_entry_t *const hash_p = stream_p->db_dir_hash_p;
            cdsFlagStat_t flags;

            ZERO_bytes(&flags, sizeof(flags));
            flags.fsConf = cdsConfHi;
            flags.fsNameType = cdsOpqDNA;

            (void) cdsSkulkDir((cds_full_name_t *)hash_p->dir_name, &flags);
        }

	db_dir_release(stream_p);    
    } /* End looping through list of directories */

   if (tm_p->tm_roothere == FALSE)
       evl_RootLost(ch_p);

    /*
     * If relocating and all succeeded, then update CHLastAddress
     */
    if (tm_p->tm_relocate && !tm_p->tm_relocate_failed)
    {
	stream_p->db_dir_hash_p = NULL;
	status = db_pseudo_read_for_update(stream_p);

	if (status == DNS_SUCCESS)
	{
	    status = write_chlastaddress(stream_p, (Set_t *)mytower);

	    if (status != DNS_SUCCESS)
		tm_p->tm_relocate_failed = TRUE;

	    db_dir_release_lock(stream_p);
	}
	else
	    tm_p->tm_relocate_failed = TRUE;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        "  background(%s): Rescheduling (normal reschedule)",
        deb_ascii_back_dsc(tmBuf,tm_p)));

    /*
     * Hierarchical Cell Support: Transmogrify clearinghouse if
     * necessary.
     */

    transmogrify_ch(file_p);


    /*
     * Periodic Checkpoint support
     */

    if (db_checkpoint_thresholds_check(file_p) ||
        (tm_p->tm_relocate && !tm_p->tm_relocate_failed))
    {
        db_checkpoint_schedule(file_p);
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            "  background(%s): scheduled a periodic checkpoint",
            deb_ascii_back_dsc(tmBuf,tm_p)));

    }
    else
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            "  background(%s): no periodic checkpoint scheduled",
            deb_ascii_back_dsc(tmBuf,tm_p)));
        }

    /* Broadcast on the background_done condition variable. */
    if ((thstatus = dthread_broadcast(&(file_p->file_back_done))) < 0) {
	dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
    }

free_pseudo:
    dns_free((char *)pseudo_buf_p);

close_stream:
    (void)db_close_stream(stream_p);

leave_background:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <background(%s) return(%d)",
        deb_ascii_back_dsc(tmBuf,tm_p), back_status));

    return((dthread_address_t)back_status);
}

/*
 * ROUTINE - background_activator
 *
 * This routine waits until a background pass should run and then creates
 * the background thread itself.  Thus the stack for this watchdog thread
 * can be a small one, and the less-active background thread doesn't
 * hog resources all the time.
 *
 * Arguments:
 *	file_p (in/out)	- contains thread information
 *
 * Returns:
 *	0
 *	or process aborts
 */

static dthread_address_t 
background_activator (back_dsc_t *tm_p)
{
    DEB_ASCII_BUF_back_dsc(tmBuf)
    file_info_t *const file_p = tm_p->tm_file_p;
    nsgbl_ch_t *const ch_p = file_p->file_ch_p;
    int thstatus,errnum;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >background_activator(%s)",
        deb_ascii_back_dsc(tmBuf,tm_p)));

    /* First invocation is two seconds away... */
    tm_p->tm_reltimespec.tv_sec = 2;
    utc_mkbinreltime(&tm_p->tm_tickle_utc, &tm_p->tm_reltimespec, &zero_inacc);

    /* and then every hour. */
    tm_p->tm_reltimespec.tv_sec = 60 * 60;
    utc_mkbinreltime(&tm_p->tm_hour_utc, &tm_p->tm_reltimespec, &zero_inacc);

    /* Set up skulk times */
    tm_p->tm_reltimespec.tv_sec = 24 * 60 * 60; /* skulk time */
    utc_mkbinreltime(&tm_p->tm_skulk_utc, &tm_p->tm_reltimespec, &zero_inacc);
    tm_p->tm_reltimespec.tv_sec >>= 1;          /* half the skulk time */
    utc_mkbinreltime(&tm_p->tm_skulkhalf_utc, &tm_p->tm_reltimespec, &zero_inacc);

    /* Get number of levels in clearinhouse name for building uppointers */
    tm_p->tm_chlevel = cdsCountSimple((dns_opq_fname *)&ch_p->ch_name);

    /* Set first time interval */
    utc_gettime(&tm_p->tm_now_utc);
    utc_addtime(&tm_p->tm_then_utc, &tm_p->tm_now_utc,
	tm_p->tm_relocate ? &tm_p->tm_tickle_utc : &tm_p->tm_hour_utc);

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{  /* lock ! */
	    dthread_cleanup_push(background_unlock, &file_p->file_mutex);

	    while (file_p->file_state != file_rundown) 
	    { /* file_rundown */
		dthread_date_time_t timeout;
		dthread_address_t join_status;
		int status;

		/*
		 * Wait for file state to be on
		 */
		while (file_p->file_state != file_open)
		{
		    if ((thstatus = dthread_wait(&(file_p->file_state_changed),
						 &(file_p->file_mutex))) < 0)
		    {
		      dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, 
				     (long)thstatus);
		    }
		}

		/* wake up is needed then for high priority */
		/* convergence and periodic checkpointing to work.  */
		status = utc_bintime(&timeout, &tm_p->tm_inacc, &tm_p->tm_tdf,
                                     &tm_p->tm_then_utc);
                dce_assert(cds__svc_handle, (0 <= status));

		while (file_p->file_state == file_open)
		{ /* file_open */
		    extern dthread_attr_t background_attr;

		    if (!file_p->file_back_force)
		    {
			if ((status = 		/* If signaled */
			     dthread_timed_wait(&file_p->file_state_changed,
						&file_p->file_mutex, &timeout)) >= 0)
			  {			/* loop or do */
			      if (!file_p->file_back_force)
				continue; /* diagnostic induced path. */
			  }
		    
			if ((status < 0) && 	/* Check for timeout */
			    (dthread_get_errno() != EAGAIN))
			  {
			      /*
			       * if no timeout, have error 
			       */
			      dce_svc_printf(CDS_S_PTHREADCONDTIMEDWAIT_A_MSG, 
					     (long)status);
			  }

			/*
			 * Have we been asked to delay the start of the background
			 * processing?  If so, it's either because someone's running
			 * db_diag, or because the server is completing the creation
			 * of this clearinghouse.
			 */
			while (file_p->file_back_inhibit) {
			    if ((thstatus = dthread_wait(&(file_p->file_state_changed),
							 &(file_p->file_mutex))) < 0)
			      {
				  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, 
						 (long)thstatus);
			      }
			}
		    } /* end !file_back_force */

                    /* Schedule wake up for one hour hence;  */
                    status = utc_gettime(&tm_p->tm_now_utc);
                    dce_assert(cds__svc_handle, (0 <= status));
                    status = utc_addtime(&tm_p->tm_then_utc, &tm_p->tm_now_utc,
                                         &tm_p->tm_hour_utc);
                    dce_assert(cds__svc_handle, (0 <= status));

		    /*
		     * dequeue file_back_force request
		     */
		    file_p->file_back_force = 0;

		    thstatus = dthread_create(&file_p->file_back_thread,
					  &background_attr,
					  (dthread_start_routine)background,
					  (dthread_address_t)tm_p);
		    if (thstatus < 0)
		    {
			/* 
			 * If low on memory, reschedule thread.
			 * Bugcheck for all other errors.
			 */
			if ((errnum = dthread_get_errno()) == ENOMEM)
			    break;
			else
			{
			    dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG,
				(long)thstatus);
			}
		    }
		    
		    /*
		     * Temporary un-lock !
		     */
		    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
		    {
		      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
				     (long)thstatus);
		    }
		    (void)dthread_join(file_p->file_back_thread, &join_status);

		    /*
		     * Re lock
		     */
		    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
		    {
		      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, 
				     (long)thstatus);
		    }
		
		    (void)dthread_detach(&file_p->file_back_thread);
		    dthread_assign(dthread_null, &file_p->file_back_thread);

                    if (tm_p->tm_relocate)
                        if (tm_p->tm_relocate_failed)
                            {
                            tm_p->tm_relocate_failed = FALSE;
                            status = utc_addtime(&tm_p->tm_then_utc,
                                                 &tm_p->tm_now_utc,
                                                 &tm_p->tm_tickle_utc);
                            dce_assert(cds__svc_handle, (0 <= status));
                            }
                        else
                            tm_p->tm_relocate = FALSE;

		    break;

		} /* End while file state is file_open */
	    } /* End while file state isn't file_rundown */
	    dthread_cleanup_pop(0);
	}/*/EndSERVER_LOCK*/

    /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <background_activator(%s) return(%d)",
        deb_ascii_back_dsc(tmBuf,tm_p), 0));

    return(0);    /* Static result for threads package (CMA) */
}
/*
 * background_unlock - unlocks mutex for owner
 *
 * Arguments:
 *	mutex_p (in)	- Pointer to mutex handle
 *
 */
static void 
background_unlock (dthread_mutex_t *mutex_p)
{
    int thstatus;

    
    if ((thstatus = dthread_unlock(&(*mutex_p))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
}

/*
 * background_start - start background thread
 *
 * Arguments:
 *	file_p (in/out)	- contains thread information
 *
 * Returns:
 * 	DNS_SUCCESS
 *	or aborts process
 */
dns_status_t 
background_start (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status = DNS_SUCCESS;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug2,
	" >background_start(%s)",
	deb_ascii_file_info(fileBuf,file_p)));

    if (!file_p->file_back_data_p) {
	extern dthread_attr_t background_activator_attr;
	back_dsc_t *const tm_p = (back_dsc_t *)dns_malloc(sizeof(back_dsc_t));
	int thstatus;

	if (tm_p) {
	    ZERO_bytes(tm_p, sizeof(*tm_p));
	    tm_p->tm_file_p = file_p;
	    tm_p->tm_relocate = TRUE;
	    file_p->file_back_data_p = tm_p;

	    if ((thstatus = dthread_create(&file_p->file_back_activator, 
					   &background_activator_attr,
					   (dthread_start_routine)background_activator, 
					   (dthread_address_t)tm_p)) < 0)
	    {
		dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
	    }
	} else
	    status = DNS_NONSRESOURCES;
    }

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug2,
	" <background_start(%s) return(%s)",
	deb_ascii_file_info(fileBuf,file_p),
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * background_stop - stop background thread
 *
 * Arguments:
 *	file_p (in/out)	- contains thread information
 *
 * Returns:
 * 	DNS_SUCCESS
 *	or aborts process
 */
dns_status_t 
background_stop (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    back_dsc_t *const tm_p = file_p->file_back_data_p;
    dns_status_t status = DNS_SUCCESS;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug2,
	" >background_stop(%s)",
	deb_ascii_file_info(fileBuf,file_p)));

    if (tm_p) {
	dthread_t background_thread; 	/* background thread info  */
	dthread_address_t join_status; 	/* exit stat of killed thread*/
	int thstatus;


	if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	}

	    /* Beginning of the 'protected code' */
	    {
		dthread_assign(file_p->file_back_thread, &background_thread);
	    }
	    /* End of the 'protected code' */

	if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}

	if (!dthread_equal(dthread_null, background_thread))
	{
	    (void)dthread_join(background_thread, &join_status);
	}
    }

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug2,
	" <background_stop(%s) return(%s)",
	deb_ascii_file_info(fileBuf,file_p),
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * background_kill - kill background thread
 *
 * Arguments:
 *	file_p (in/out)	- contains thread information
 */
void 
background_kill (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    back_dsc_t *const tm_p = file_p->file_back_data_p;

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug2,
	" >background_kill(%s)",
	deb_ascii_file_info(fileBuf,file_p)));

    if (tm_p) {
	dthread_address_t join_status; 	/* exit stat of killed thread */
	int thstatus;


	if (!dthread_equal(dthread_null, file_p->file_back_activator))
	{
	    (void)dthread_cancel(file_p->file_back_activator);
	    (void)dthread_join(file_p->file_back_activator, &join_status);
	    (void)dthread_detach(&file_p->file_back_activator);
	    dthread_assign(dthread_null, &file_p->file_back_activator);
	}

	if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	}

        /* Beginning of the 'protected code' */
	{
	    if (!dthread_equal(dthread_null, file_p->file_back_thread))
	    {

		/* Temporarily unlock.. */
		if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
		{
		  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
				 (long)thstatus);
		}
		
		(void)dthread_join(file_p->file_back_thread, &join_status);

		/* Re-lock */
		  
		if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
		{
		  dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
		}
		(void)dthread_detach(&file_p->file_back_thread);
		dthread_assign(dthread_null, &file_p->file_back_thread);
	    }
	    file_p->file_back_data_p = NULL;
	}

	/* End of the 'protected code' */
	if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}

	dns_free((char *)tm_p);
    }

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_server,
	svc_c_debug2,
	" <background_kill(%s)",
	deb_ascii_file_info(fileBuf,file_p)));
}

/*
 * towerset_compare_exact_mx
 *    checks that the two towers have the same members.  Ignores
 *    timestamps.  This is done using one USER version of a set and
 *    one internal database version of a set.
 *
 * Input:
 *     towerset1_p = pointer to first tower set internal form
 *     towerset2_p = pointer to second tower set external form
 * Output:
 *
 * Value:
 *     returns TRUE if the towers are equivalent except for
 *     order and timestamps.
 *
 * Side-effect:
 *     writes in the second towerset - marks things absent that match.
 */
static int 
towerset_compare_exact_mx (DBSet_t  *towerset1_p,
                           Set_t    *towerset2_p)
{
    DEB_ASCII_BUF_DBSet(towerset1Buf)
    DEB_ASCII_BUF_Set(towerset2Buf)
    const int             num_mem1 = NUM_DBSet(towerset1_p);
    const int             num_mem2 = NUM_Set(towerset2_p);
    int                   av_len;
    int                   i,j,found=FALSE;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " >towerset_compare_exact_mx(%s,%s)",
        deb_ascii_DBSet(towerset1Buf,towerset1_p),
        deb_ascii_Set(towerset2Buf,towerset2_p)));

    for (i = 0; i < num_mem1; ++i)
        {
	DBSetMember_t *const member1_p =
			(DBSetMember_t *)INDEX_DBSet(towerset1_p, i);

        if (EXT8(member1_p->dm_flag) == DM_absent)
            continue;
        if (!EXT8(member1_p->dm_valid))
            continue;
        av_len = LEN_DBSetMemberData(member1_p);
        for (j = 0; j < num_mem2; ++j)
            {
	    MemberValue_t *const member2_p =
			(MemberValue_t *)INDEX_Set(towerset2_p, j);

            if (FLAG_IS_CLEAR(member2_p->mv_flag, MV_present))
                continue;
            if (av_len != EXT16(member2_p->mv_length))
                continue;
            if (EXT8(member2_p->mv_valuetype) != EXT8(member1_p->dm_valuetype))
              continue;
            if (NE_bytes(member2_p->mv_value, member1_p->dm_data, av_len))
                continue;

            /* mark the member in the second set absent */
            CLEAR_FLAG(member2_p->mv_flag, MV_present);
            found = TRUE;
            break;
            }
        if (!found) break;
        }

    if (found)
        /* check for any present members in the second list */
        for (j = 0; j < num_mem2; ++j)
            {
	    MemberValue_t *const member2_p =
			(MemberValue_t *)INDEX_Set(towerset2_p, j);

            if (FLAG_IS_SET(member2_p->mv_flag, MV_present)) found = FALSE;
            }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug2,
        " <towerset_compare_exact_mx(%s,%s) return(%d)",
        deb_ascii_DBSet(towerset1Buf,towerset1_p),
        deb_ascii_Set(towerset2Buf,towerset2_p),
        found));
    return (found);
}
