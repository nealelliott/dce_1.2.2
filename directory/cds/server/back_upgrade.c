/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_upgrade.c,v $
 * Revision 1.1.6.2  1996/02/18  19:35:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:58  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:29:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:36  root]
 * 
 * Revision 1.1.4.6  1994/09/07  20:02:12  mccann
 * 	fix version handling OT:12004
 * 	[1994/09/07  18:41:42  mccann]
 * 
 * Revision 1.1.4.5  1994/08/26  21:55:26  zee
 * 	Bump max directory version to 4.0 instead of 3.1.  In
 * 	back_upgrade_directory_actions, add directory_type parameter and also
 * 	call routine to change DEFOBJECTACL manager type to cat.uuid.
 * 	[1994/08/26  21:48:38  zee]
 * 
 * Revision 1.1.4.4  1994/08/03  19:03:41  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:20:05  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:57  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:15  devsrc]
 * 
 * Revision 1.1.4.2  1994/05/06  16:04:19  zee
 * 	     Bump maximum directory version to 3.1.   Also add some
 * 	     serviceability messages to the upgrade process.
 * 	[1994/05/05  21:19:34  zee]
 * 
 * Revision 1.1.4.1  1994/03/12  22:10:17  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:38:55  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:19:48  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:19:47  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:25:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module back_upgrade.c
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
 * This module contains the routines required to detect and perform a
 * replica upgrade.
 *
 */
#include <server.h>
#include <back.h>
#include <dbdef.h>
#include <events.h>
#include <find.h>
#include <tlog.h>
#include <sets.h>
#include <uid.h>


/*
 * upgrade_replica
 *
 * This routine actually does the dirty work associated with upgrading
 * a replica.
 *
 */
static dns_status_t 
upgrade_replica (db_stream_t  *stream_p,
                 int          upgrade_to_major,
                 int          upgrade_to_minor)
{
    VersionNumber_t new_replica_version;    /* Work buffer */
    Update_u        update_block;           /* Work buffer */
    tlog_dsc_t      tlog;
    dns_status_t    status;
    DEB_ASCII_BUF_db_stream(streamBuf)


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >upgrade_replica(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    /*
     * So the only thing left to do here is set the replica version.
     */

    if ((upgrade_to_major > 4) || ((upgrade_to_major == 4) &&
                                   (upgrade_to_minor > 0)) ) {
        return (DNS_NAMESERVERBUG);
    }


    /*
     * Set the ReplicaVersion to the UpgradeTo value.
     */

    INS8(new_replica_version.vn_major, upgrade_to_major);
    INS8(new_replica_version.vn_minor, upgrade_to_minor);

    tlog.cnt = 1;
    tlog.buf_p = update_block;
    tlog.len = update_dns_build((Update_t *)update_block,
                      UD_present,
                      NULL,
                      dns_Attribute(DNS_REPLICAVERSION),
                      BL_true,
                      LEN_VersionNumber(&new_replica_version),
                      (byte_t *)&new_replica_version);

    status = set_replace_value(&stream_p->db_dir,
                               (Update_t *)update_block,
                               dns_Attribute(DNS_REPLICAVERSION));

    if (status == DNS_SUCCESS) {
        status = db_dir_update(stream_p, &tlog);
    }

    return (status);
}


/* -------------------------------------------------------------------------- */


static dns_status_t 
initialize_version (db_stream_t             *stream_p,
                    attribute_descriptor_t  *attribute_p)
{
    VersionNumber_t version_block;           /* Work buffer */
    Update_u        update_block;            /* Work buffer */
    dns_status_t    status;


    get_directory_version(&version_block);

    (void)update_dns_build((Update_t *)update_block,
                      UD_present,
                      NULL,
                      attribute_p,
                      BL_true,
                      LEN_VersionNumber(&version_block),
                      (byte_t *)&version_block);

    status = set_replace_value(&stream_p->db_dir,
                               (Update_t *)update_block,
                               attribute_p);

    return (status);
}


/* -------------------------------------------------------------------------- */

/*
 * back_upgrade_replica_actions
 *
 * This routine determines if we should upgrade a replica.   It is called
 * by the background routine (back_ground.background) after performing
 * the processing for a replica in the "on" state.  If all the pre-conditions
 * are met for a replica upgrade, it is attempted.
 *
 * This routine assumes that the stream passed in is pointing to the
 * directory we are to work on, that modifies are allowed.
 *
 * When the routine completes the stream will have the updated directory
 * record, and no lock held (pretty much what we assume the input conditions
 * will look like).
 *
 */
void 
back_upgrade_replica_actions (db_stream_t *stream_p)
{

    DBSet_t          *set_p;                       /* Work pointer */
    DBSetMember_t    *member_p;                    /* Work pointer */
    VersionNumber_t  *replica_version_p;
    VersionNumber_t  *upgrade_to_p;
    Timestamp_t      *upgrade_to_ts_p;
    VersionNumber_t  *directory_version_p;
    Timestamp_t      *directory_version_ts_p;
    Timestamp_t      *all_up_to_p;
    Timestamp_t      *maximized_ts_p;
    VersionNumber_t   software_version;            /* Obtained from globals */
    int               index;
    int               modifies_have_occurred = FALSE;
    dns_status_t      status;
    DEB_ASCII_BUF_db_stream(streamBuf)
    int               preconditions_met = 0;        /* For diagnostics */


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >back_upgrade_replica_actions(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    db_dir_release(stream_p);                   /* Free up prior locks, etc. */

    status = db_dir_read_for_update(stream_p);
    if (status != DNS_SUCCESS) 
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug6,
            " <back_upgrade_check(%s) - dir_read_failed: 0x%x",
            deb_ascii_db_stream(streamBuf, stream_p), 
	    status));
        
	return;
    }

    /*
     * Determine if the ReplicaVersion attribute is absent.  If so,
     * set it to 1.0
     */

    if (find_attribute(stream_p->db_dir.data_p,
                       dns_AttributeName(DNS_REPLICAVERSION),
                       &set_p, &index) != DNS_SUCCESS) {
        /* Update it with a 3.0 */
        status = initialize_version(stream_p, dns_Attribute(DNS_REPLICAVERSION));
        if (status == DNS_SUCCESS) {
            modifies_have_occurred = TRUE;
        }
    }


    /*
     * Determine if the DirectoryVersion attribute is absent.  If so,
     * set it to 3.0
     */

    if (find_attribute(stream_p->db_dir.data_p,
                       dns_AttributeName(DNS_DIRECTORYVERSION),
                       &set_p, &index) != DNS_SUCCESS) {
        /* Update it with a 3.0 */
        status = initialize_version(stream_p, dns_Attribute(DNS_DIRECTORYVERSION));
        if (status == DNS_SUCCESS) {
            modifies_have_occurred = TRUE;
        }
    }

    /*
     * Write out changes to the directory record (if any).
     */

    if (modifies_have_occurred) {
      /* this is all expected to work */
        status = db_dir_update(stream_p, NULL);
	dce_assert(cds__svc_handle, status == DNS_SUCCESS);

        db_dir_release(stream_p);
        status = db_dir_read_for_update(stream_p);
	dce_assert(cds__svc_handle, status == DNS_SUCCESS);
    }


    /*
     * Locate ReplicaVersion, DirectoryVersion, and AllUpTo in the 
     * directory record.
     */

    status = find_attribute(stream_p->db_dir.data_p,
                 dns_AttributeName(DNS_REPLICAVERSION),
                 &set_p, &index);
    /* just added it so this should work */
    dce_assert(cds__svc_handle, status == DNS_SUCCESS);

    member_p = (DBSetMember_t *)DATA_DBSet(set_p);
    replica_version_p = (VersionNumber_t *)member_p->dm_data;

    status = find_attribute(stream_p->db_dir.data_p,
                 dns_AttributeName(DNS_DIRECTORYVERSION),
                 &set_p, &index);
    /* just added it so this should work */
    dce_assert(cds__svc_handle, status == DNS_SUCCESS);

    member_p = (DBSetMember_t *)DATA_DBSet(set_p);
    directory_version_p = (VersionNumber_t *)member_p->dm_data;
    directory_version_ts_p = (Timestamp_t *)member_p->dm_ts;

    status = find_attribute(stream_p->db_dir.data_p,
                 dns_AttributeName(DNS_ALLUPTO),
                 &set_p, &index);
    if (status != DNS_SUCCESS)
    {
	evl_AttributeMissing(stream_p->db_file_p->file_ch_p, 
			     (FullName_t *)stream_p->db_dir_hash_p->dir_name,
			     dns_AttributeName(DNS_ALLUPTO));
	db_dir_release_lock(stream_p);
	return;
    }

    member_p = (DBSetMember_t *)DATA_DBSet(set_p);
    all_up_to_p = (Timestamp_t *)member_p->dm_data;


    /*
     * Upgrade predicates:
     *    UpgradeTo attribute present
     *    UpgradeTo > DirectoryVersion
     *    max(UpgradeTo.timestamp, DirectoryVersion.timestamp) < AllUpTo
     *    UpgradeTo > ReplicaVersion
     *    UpgradeTo <= Server Software Version
     *
     * The variable preconditions_met is bumped after each predicate
     * passes.  This is only used for diagnostic reporting.  When
     * a predicate fails, we break out for cleanup and return to the caller.
     */

    do {                                              /* Do this once only */

        if (find_attribute(stream_p->db_dir.data_p,
                           dns_AttributeName(DNS_UPGRADETO),
                           &set_p, &index) != DNS_SUCCESS) {
            break;
        }
        preconditions_met++;                                           /* 1 */

        member_p = (DBSetMember_t *)DATA_DBSet(set_p);
        upgrade_to_p = (VersionNumber_t *)member_p->dm_data;
        upgrade_to_ts_p = (Timestamp_t *)member_p->dm_ts;

        if (!GT_VersionNumber(upgrade_to_p, directory_version_p)) {
            break;
        }
        preconditions_met++;                                           /* 2 */

        if (dnsCmpCTS((dns_cts *)upgrade_to_ts_p,
				(dns_cts *)directory_version_ts_p) > 0) {
            maximized_ts_p = upgrade_to_ts_p;
        } else {
            maximized_ts_p = directory_version_ts_p;
        }

        if (dnsCmpCTS((dns_cts *)maximized_ts_p, (dns_cts *)all_up_to_p) >= 0) {
           break;
        }
        preconditions_met++;                                           /* 3 */

        if (!GT_VersionNumber(upgrade_to_p, replica_version_p)) {
           break;
        }
        preconditions_met++;                                           /* 4 */


        /*
         * Last check.  Make sure they are not requesting an upgrade that
         * exceeds the local software version.
         */

        get_server_version(&software_version);
	if (GT_VersionNumber(upgrade_to_p, (&software_version)))
	{
            /* Generate event UpgradeNotPossible */
            evl_UpgradeNotPossible(stream_p->db_file_p->file_ch_p,
			       (FullName_t *)stream_p->db_dir_hash_p->dir_name,
				DNS_NOTSUPPORTED);

            break;
        }
        preconditions_met++;                                           /* 5 */


        /*
         * Upgrade the replica.  This may not work - big deal.  We'll
         * try it again a little later.
         */

        status = upgrade_replica(stream_p,
                                 EXT8(upgrade_to_p->vn_major),
                                 EXT8(upgrade_to_p->vn_minor));

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug6, 
            "back_upgrade_replica_actions: upgrade_replica returns 0x%x for %s\n",
            status, 
            deb_ascii_db_stream(streamBuf, stream_p)));

    } while(0);  /* Execute loop once - take advantage of the breaks */

    db_dir_release_lock(stream_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <back_upgrade_replica_actions(%s) - preconditions met: %d",
        deb_ascii_db_stream(streamBuf,stream_p),
        preconditions_met));

}


/* -------------------------------------------------------------------------- */

/*
 * back_upgrade_directory_actions
 *
 * This routine determines if we should upgrade a directory.   It is called
 * by the background routine (back_skulk.skulk_dir) just prior to setting
 * AllUpTo.  Also called in background for the pseudo-directory.
 *
 * This routine assumes that the stream passed in is pointing to the
 * directory we are to work on, that modifies are allowed, and that it is
 * free to do a read_for_update.  It will leave with the directory record
 * read, but not locked.
 *
 */
void 
back_upgrade_directory_actions (db_stream_t      *stream_p,
                                VersionNumber_t  *min_version_p,
				int              directory_type)
{

    DBSet_t          *set_p;                       /* Work pointer */
    DBSetMember_t    *member_p;                    /* Work pointer */
    VersionNumber_t  *upgrade_to_p;
    Timestamp_t      *upgrade_to_ts_p;
    VersionNumber_t  *directory_version_p;
    VersionNumber_t  software_version;
    Timestamp_t      *all_up_to_p;
    int               index;
    dns_status_t      status;
    char              update_block[2*sizeof(Update_u)];
    char              *update_p;
    tlog_dsc_t        tlog;
    DEB_ASCII_BUF_db_stream(streamBuf)
    int               preconditions_met = 0;        /* For diagnostics */


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >back_upgrade_directory_actions(%s, %d.%d, %d)",
        deb_ascii_db_stream(streamBuf,stream_p),
        EXT8(min_version_p->vn_major),
        EXT8(min_version_p->vn_minor),
	directory_type));


    if (directory_type == ET_directory) {
	status = db_dir_read_for_update(stream_p);
	if (status != DNS_SUCCESS) 
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug6,
	        " <back_upgrade_directory(%s) - dir_read_failed: 0x%x",
                deb_ascii_db_stream(streamBuf, stream_p), 
                status));

	    return;
	}
    }


    /*
     * DirectoryVersion Upgrade predicates:
     *    UpgradeTo attribute present
     *    UpgradeTo.timestamp <= AllUpTo
     *    UpgradeTo > DirectoryVersion
     *    UpgradeTo >= minReplicaVersion
     *
     * The variable preconditions_met is bumped after each predicate
     * passes.  This is only used for diagnostic reporting.  When
     * a predicate fails, we break out for cleanup and return to the caller.
     */

    do {                                              /* Do this once only */

      /*
       * Locate DirectoryVersion and AllUpTo in the directory record.
       */
      
      status = find_attribute(stream_p->db_dir.data_p,
			      dns_AttributeName(DNS_DIRECTORYVERSION),
			      &set_p, &index);
      if (status != DNS_SUCCESS) 
	  {
	    /* Update it with a 3.0 */
	    (void)initialize_version(stream_p, 
					dns_Attribute(DNS_DIRECTORYVERSION));
	    status = find_attribute(stream_p->db_dir.data_p,
				    dns_AttributeName(DNS_DIRECTORYVERSION),
				    &set_p, &index);
	    if (status != DNS_SUCCESS) {
	      evl_AttributeMissing(stream_p->db_file_p->file_ch_p, 
			   (FullName_t *)stream_p->db_dir_hash_p->dir_name,
			   dns_AttributeName(DNS_DIRECTORYVERSION));
	      break;
	    }
	  }

      member_p = (DBSetMember_t *)DATA_DBSet(set_p);
      directory_version_p = (VersionNumber_t *)member_p->dm_data;

      status = find_attribute(stream_p->db_dir.data_p,
                     dns_AttributeName(DNS_ALLUPTO),
                     &set_p, &index);
      if (status != DNS_SUCCESS) 
	  {
	    evl_AttributeMissing(stream_p->db_file_p->file_ch_p, 
			     (FullName_t *)stream_p->db_dir_hash_p->dir_name,
			     dns_AttributeName(DNS_ALLUPTO));
	    break;
	  }

      member_p = (DBSetMember_t *)DATA_DBSet(set_p);
      all_up_to_p = (Timestamp_t *)member_p->dm_data;


      /*
       * Manditory data requirements met.  Now we assess the upgrade
       * preconditions.
       */
      
      if (find_attribute(stream_p->db_dir.data_p,
			 dns_AttributeName(DNS_UPGRADETO),
			 &set_p, &index) != DNS_SUCCESS) {
	break;
      }
      preconditions_met++;                                           /* 1 */
      member_p = (DBSetMember_t *)DATA_DBSet(set_p);
      upgrade_to_p = (VersionNumber_t *)member_p->dm_data;
      upgrade_to_ts_p = (Timestamp_t *)member_p->dm_ts;


      if (dnsCmpCTS((dns_cts *)upgrade_to_ts_p, (dns_cts *)all_up_to_p) >0) {
	break;
      }
      preconditions_met++;                                           /* 2 */


      get_server_version(&software_version);
      if ((!GT_VersionNumber(upgrade_to_p, directory_version_p)) ||
	  (GT_VersionNumber(upgrade_to_p, (&software_version)))) {
	/* Can't downgrade - remove the UpgradeTo attribute  
	 * Also remove after upgrade happens
	 */
	tlog.cnt = 1;
	tlog.buf_p = update_block;
	
	tlog.len = update_dns_build((Update_t *)update_block,
				    UD_absent,
				    NULL,
				    dns_Attribute(DNS_UPGRADETO),
				    BL_false,
				    0,
				    NULL);
	
	status = set_replace_value(&stream_p->db_dir,
                                   (Update_t *)update_block,
                                   dns_Attribute(DNS_UPGRADETO));
	
	if (status == DNS_SUCCESS) {
	  status = db_dir_update(stream_p, &tlog);
	}
	break;
      }
      preconditions_met++;                                           /* 3 */



      if (LT_VersionNumber(min_version_p, upgrade_to_p)) {
	break;
      }
      preconditions_met++;                                           /* 4 */


      /*
       * Upgrade the directory.
       */
      
      tlog.cnt = 1;
      tlog.buf_p = update_block;

      tlog.len = update_dns_build((Update_t *)update_block,
				  UD_present,
				  NULL,
				  dns_Attribute(DNS_DIRECTORYVERSION),
				  BL_true,
				  LEN_VersionNumber(upgrade_to_p),
				  (byte_t *)upgrade_to_p);

      status = set_replace_value(&stream_p->db_dir,
				 (Update_t *)update_block,
				 dns_Attribute(DNS_DIRECTORYVERSION));
      
      update_p  = update_block + tlog.len;

      /*
       * If pseudo-directory, also update the ReplicaVersion.
       */
      if (status == DNS_SUCCESS) {
	if (directory_type == ET_clearinghouse) {
	  ++tlog.cnt;
	  tlog.len += update_dns_build((Update_t *)update_p,
				       UD_present,
				       NULL,
				       dns_Attribute(DNS_REPLICAVERSION),
				       BL_true,
				       LEN_VersionNumber(upgrade_to_p),
				       (byte_t *)upgrade_to_p);
	  
	  status = set_replace_value(&stream_p->db_dir,
				     (Update_t *)update_p,
				     dns_Attribute(DNS_REPLICAVERSION));
	  
	} else {
	  /*
	   * If going to 4.0, change the DEFOBJECTACL manager type 
	   * to cat.uuid.
	   */
#if defined(DCE_SEC)
	  if ((EXT8(upgrade_to_p->vn_major) == 4) &&
	      (EXT8(upgrade_to_p->vn_minor) == 0)) {
	    
	    status = update_dir_defobjacl_mgr_type(&stream_p->db_dir,
					  (Update_t *)update_p);
	    if (status == DNS_SUCCESS) {
	      ++tlog.cnt;
	      tlog.len += LEN_Update(update_p);
	    }
#endif
	  }
	}
      
	if (status == DNS_SUCCESS) {
	  status = db_dir_update(stream_p, &tlog);
	}
      }
	
    DCE_SVC_DEBUG((
		   cds__svc_handle,
		   cds_svc_server,
		   svc_c_debug6, 
		   "I upgrade_directory: 0x%x for %s\n",
		   status, 
		   deb_ascii_db_stream(streamBuf, stream_p)));
    
  } while(0);  /* Execute loop once - take advantage of the breaks */

  db_dir_release_lock(stream_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <back_upgrade_directory_actions(%s) - preconditions met: %d",
        deb_ascii_db_stream(streamBuf,stream_p),
        preconditions_met));

}
