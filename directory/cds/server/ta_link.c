/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_link.c,v $
 * Revision 1.1.6.2  1996/02/18  19:36:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:40  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:33:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:11  root]
 * 
 * Revision 1.1.4.6  1994/09/07  20:02:18  mccann
 * 	fix version handling OT:12004
 * 	[1994/09/07  18:42:17  mccann]
 * 
 * Revision 1.1.4.5  1994/08/26  21:55:37  zee
 * 	Prevent replicas < 4.0 from joining a 4.0 replica set.
 * 	[1994/08/26  21:49:53  zee]
 * 
 * Revision 1.1.4.4  1994/08/03  19:04:52  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:31:36  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:44:04  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:41  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:51:35  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:22:27  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:13:08  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:47:01  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:24:19  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:25:00  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:26:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module ta_link.c
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
 */
#include <server.h>
#include <clerk.h>
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <replica.h>
#include <security.h>
#include <sets.h>
#include <tlog.h>
#include <uid.h>


/*
 * Local prototypes
 */

/*
 *   RINGPOINTER is obsolete
 * ta_link_replica
 *     This routine adds or removes a replica of a directory.  It
 * is used to complete the creation of a new replica by adding the
 * replica to the cds_REPLICAS attribute of the directory entry and 
 * modifying the directory ring pointer which is used by Skulk operation.
 *
 * Input:
 *     client_p = address of clearinghouse client
 *     onbehalfof = address of real client
 *     progress_p = progress record 
 *     diruid_p = UID of directory
 *     update_p = pointer to update
 *     
 * Output:
 *     epoch_p = address to return a epoch
 *     ring_p = address to return ringpointer  (OBSOLETE)
 *     version_p = address to return version of directory
 *
 * Value:
 *     status
 */
dns_status_t 
ta_link_replica (struct user_descriptor  *client_p,
                 struct user_descriptor  *onbehalfof_p,
                 Progress_t              *progress_p,
                 ObjUID_t                *diruid_p,
                 Update_t                *update_p,
                 int                     updlen,
                 ObjUID_t                *epoch_p,
                 ObjUID_t                *ring_p,
                 version_t               *version_p)
{
    Update_u           updbuf[2];
    struct tlog_dsc    tlog;
    struct db_stream  *stream_p;
    DBSet_t           *set_p;
    int                set_index;
    VersionNumber_t   *max_version;
    dns_status_t       status;
    VersionNumber_t   dir_version;

    status = db_open_stream(client_p->ud_chuid_p, &stream_p, OP_ModifyAttribute);
    if (status != DNS_SUCCESS)
        return(status);

    status = walktree(client_p, ET_directory, WALKTREE_LINK_OP,
			progress_p, stream_p);
    if (status != DNS_SUCCESS) return(status);

    /*
     * Assure that the server attempting to link the new replica
     * into this replica set is running at or above the 
     * max(DirectoryVersion,UpgradeTo).
     */
    db_dir_get_version(stream_p, (version_t *)&dir_version);

    max_version = &dir_version;
    if (find_attribute(stream_p->db_dir.data_p,
			dns_AttributeName(DNS_UPGRADETO),
			&set_p, &set_index) == DNS_SUCCESS)
        {
	    register DBSetMember_t *member_p = (DBSetMember_t *)DATA_DBSet(set_p);
            if (GT_VersionNumber(((VersionNumber_t *)member_p->dm_data), max_version))
                {
                max_version = (VersionNumber_t *)member_p->dm_data;
                }
        }

    /*
     * Because of additional rdacl formats introduced in this version
     * (OSF DCE V1.1), we cannot allow old replicas to replicate new
     * directories (defined as DirectoryVersion = 4.0) since the old
     * replicas do not recognize the new rdacl formats.  So, new servers
     * stick a special value in the update_p->ud_timestamp field to let
     * the master replica know it is a V4.0 replica.
     */
    /* Check for the special V4 replica-identifying timestamp */
    status = DNS_SUCCESS;
    if (EXT8(dir_version.vn_major) == 4) {
	Timestamp_t  v4_ts;
	Timestamp_t *upd_ts_p;

	get_v4_replica_ts(&v4_ts);
	if (!EQ_Timestamp(&v4_ts, (Timestamp_t *)update_p->ud_timestamp)) {
	    status = CDS_NEWVERSIONSKEW;
	}
    }

    if (status == DNS_SUCCESS) {
      if (NE_ObjUID(stream_p->db_dir.data_uid, diruid_p))
	  /* What we found is not the correct directory uid */
	  status = DNS_NOTAREPLICA;
      else if (security_check(client_p, stream_p, Super_check, 
			      ET_directory, NULL) != DNS_SUCCESS)
	  {
	    status = DNS_UNTRUSTEDCH;
	    security_log(client_p,
		(FullName_t *)SKIP_FullName(progress_p->pr_unresolvedName),
		stream_p->db_file_p->file_ch_p);
	  }
      /* 
       * Onbehalfof check is done by the calling server prior to the 
       * link call.  Servers trust each other to do the right thing.
       */
      else
	  {
	    ZERO_ObjUID(ring_p);
	    
	    /*
	     * copy the epoch to the output area
	     */
	    if (find_attribute(stream_p->db_dir.data_p,
			       dns_AttributeName(DNS_EPOCH),
			       &set_p, &set_index) == DNS_SUCCESS)
		{
		  register DBSetMember_t *const member_p =
		      (DBSetMember_t *)DATA_DBSet(set_p);

		  COPY_ObjUID(member_p->dm_data, epoch_p);
		}
	    else
		ZERO_ObjUID(epoch_p);

	    /*
	     * Copy the replica version to the output area
	     */		
	    if (find_attribute(stream_p->db_dir.data_p,
			       dns_AttributeName(DNS_REPLICAVERSION),
			       &set_p, &set_index) == DNS_SUCCESS)
		{
		  register DBSetMember_t *const member_p =
		      (DBSetMember_t *)DATA_DBSet(set_p);
		  
		  COPY_version(member_p->dm_data, version_p);
		}
	    else
		{
		  get_directory_version((VersionNumber_t *)version_p);
		}
	    
	    tlog.buf_p = (byte_t *)updbuf;
	    tlog.len = 0;
	    tlog.cnt = 0;

	    
	    status = ts_new((Timestamp_t *)update_p->ud_timestamp);
	    if (status == DNS_SUCCESS)
		{
		  COPY_bytes(update_p, SKIP_bytes(tlog.buf_p, tlog.len), 
			     updlen);
		  tlog.len += updlen;
		  tlog.cnt++;
		  status = applyupdate(stream_p, ET_directory, update_p, 
			       dns_Attribute(DNS_REPLICAS), TRUE, &tlog);
		}
	  }
    }
    db_close_stream(stream_p);
    return(status);
}


