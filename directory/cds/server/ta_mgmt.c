/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_mgmt.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:44  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:33:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:15  root]
 * 
 * Revision 1.1.8.7  1994/09/06  17:37:12  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:31:03  proulx]
 * 
 * Revision 1.1.8.6  1994/08/16  18:28:10  jd
 * 	drop from janet
 * 	[1994/08/13  16:07:07  jd]
 * 
 * Revision 1.1.8.5  1994/08/03  19:04:55  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:31:57  mccann]
 * 
 * Revision 1.1.8.4  1994/06/09  18:44:07  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:44  devsrc]
 * 
 * Revision 1.1.8.3  1994/04/25  19:57:25  ohara
 * 	add stdio.h
 * 	[1994/04/21  15:42:29  ohara]
 * 
 * Revision 1.1.8.2  1994/04/14  14:51:36  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:22:36  peckham]
 * 
 * Revision 1.1.8.1  1994/03/12  22:13:16  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:47:27  peckham]
 * 
 * Revision 1.1.6.1  1993/10/12  21:10:49  pwang
 * 	In mgmt_delete_ch(), (1) check return status of mgmt_remove_dir()
 * 	and restore the On state to the clearinghouse if DNS_NOTEMPTY
 * 	is returned; (2) restore and enable the clearinghouse if someone
 * 	added a master replica to this clh during the delete process and
 * 	the db_disable has been called. [OT#4982]
 * 	[1993/10/12  21:10:01  pwang]
 * 
 * Revision 1.1.4.3  1992/12/30  14:24:32  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:25:15  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/04  20:47:26  keegan
 * 	Fix bug reported in OT#4919 (and DECdns QARs U475, U491)
 * 	Nagging NotEmpty during delete clearinghouse.
 * 	Solution courtesy of John Grober: stream improperly reused by
 * 	mgmt_remove_dir() before calling ta_replica_remove().  We now
 * 	release stream buffers and clear db_dir_hash_p before reusing.
 * 	[1992/12/04  20:46:00  keegan]
 * 
 * Revision 1.1.2.3  1992/03/22  22:37:31  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:37:09  weisman]
 * 
 * Revision 1.1.2.2  1992/01/22  23:02:48  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:18:22  melman]
 * 
 * $EndLog$
 */
/*
 * Module ta_mgmt.c
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
 * Routines that correspond to management operations
 */
#include <server.h>
#include <address.h>
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <replica.h>
#include <sets.h>
#include <security.h>
#include <tlog.h>
#include <uid.h>
#include <clerk.h>

#include <stdio.h>
#include <dce/dce_cf.h>

#define INITIAL_RECORD_SIZE 4096
extern bytes_u RPC_ClassVersion[];
extern unsigned char RPCversion[2];

/*
 * Local prototypes
 */
static dns_status_t
mgmt_create_root_object (
    db_stream_t		*,
    nsgbl_ch_t		*,
    user_descriptor_t	*);

static dns_status_t
mgmt_create_root (
    db_stream_t		*,
    nsgbl_ch_t		*,
    user_descriptor_t	*,
    version_t           *);

static dns_status_t
mgmt_remove_dir (
    db_stream_t		*,
    data_record_t	*,
    user_descriptor_t	*);


/*
 * mgmt_find_chuid
 *	Locate the nsgbl_ch by clearinghouse uid
 *      assumes that ns_p->ns_mutex is locked during call
 */
struct nsgbl_ch 
*mgmt_find_chuid (nsgbl_ns_t  *ns_p,
                  ObjUID_t    *uid_p)
{
    nsgbl_ch_t		*ch_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >mgmt_find_chuid"));

    /* Loop until find match or no more clearinghouses */
    for (ch_p = (nsgbl_ch_t *)ns_p->ns_clearinghouses.next_p;
	    ch_p != (nsgbl_ch_t *)&ns_p->ns_clearinghouses;
	    ch_p = (nsgbl_ch_t *)ch_p->ch_clearinghouses.next_p)
	{
	    if (EQ_ObjUID(uid_p, &ch_p->ch_uid))
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_server,
		    svc_c_debug5,
		    " <mgmt_find_chuid: return(ch_p=0x%x)",
		    ch_p));

		return(ch_p);
	    }
	    
	/* Try next */
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <mgmt_find_chuid: @ end - return(NULL)"));

    return(NULL);
}

/*
 * mgmt_find_chname
 *	Locate the nsgbl_ch by clearinghouse name
 *      assumes that ns_p->ns_mutex is locked during call
 *
 */
struct nsgbl_ch 
*mgmt_find_chname (nsgbl_ns_t  *ns_p,
                   FullName_t  *name_p)
{
    nsgbl_ch_t		*ch_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >mgmt_find_chname"));

    /* Loop until find match or no more clearinhouses */
    for (ch_p = (nsgbl_ch_t *)ns_p->ns_clearinghouses.next_p;
	    ch_p != (nsgbl_ch_t *)&ns_p->ns_clearinghouses;
	    ch_p = (nsgbl_ch_t *)ch_p->ch_clearinghouses.next_p)
	{
	    if (names_fullmatch(name_p, &ch_p->ch_name))
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_server,
		    svc_c_debug5,
		    " <mgmt_find_chname: return(ch_p=0x%x)",
		    ch_p));

		return(ch_p);
	    }
	    
	/* Try next */
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <mgmt_find_chname: @ end - return(NULL)"));

    return(NULL);
}

/*
 * mgmt_find_chdata
 *	This function expects the clearinghouse data to exist on disk or not at 
 *	all.  If the clearinghouse data is found in memory, returns DNS_ERROR.
 *	If found on disk, returns DNS_SUCCESS, otherwise returns DNS_FAILURE.
 *
 */
dns_status_t 
mgmt_find_chdata (nsgbl_ns_t  *ns_p,
                  char        *filespec_p)
{
    nsgbl_ch_t		*ch_p;
    dns_status_t	status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >mgmt_find_chdata"));

    /* See if clearinghouse file is in memory. */
    for (ch_p = (nsgbl_ch_t *)ns_p->ns_clearinghouses.next_p;
	    ch_p != (nsgbl_ch_t *)&ns_p->ns_clearinghouses;
	    ch_p = (nsgbl_ch_t *)ch_p->ch_clearinghouses.next_p)
	{
	    if (!strcmp(filespec_p, (char *)ch_p->ch_filename))
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_server,
		    svc_c_debug5,
		    " <mgmt_find_chdata: return(DNS_SUCCESS)"));

		return(DNS_SUCCESS);
	    }	    

	/* Try next */
	}

    /* See if checkpoint file for clearinghouse is on disk. */
    status = sys_find_versionfile( filespec_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <mgmt_find_chdata: @ end - return(status=%d)",status));

    return(status);	
}

/*
 * mgmt_create_root_object
 *	Create the object in the root directory that describes
 *	this clearinghouse.
 */
static dns_status_t 
mgmt_create_root_object (db_stream_t        *stream_p,
                         nsgbl_ch_t         *ch_p,
                         user_descriptor_t  *user_p)
{
    ClassVersion_t	classversion;
    entrylist_t 	objlist[5], *objlist_p = objlist;
    DBSet_t		*set_p;
    int 		index;
    dns_status_t 	status;
#if !defined(DCE_SEC)
    bytes_u		tower[READ_BUF_SIZE];
    int                 i, num_mem;
    int                 len;
    Timestamp_u         ts;
    Set_t		*userset_p;
    Update_u		upd_buf;
#endif


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >mgmt_create_root_object"));

    objlist_p->el_type = DNS_CLASS;
    objlist_p->el_len = LEN_ClassName(cds_classlist[DNS_CLEARINGHOUSE].cname);
    objlist_p->el_value_p = cds_classlist[DNS_CLEARINGHOUSE].cname;
    objlist_p++;

    INS8(classversion.cv_major, 2);
    INS8(classversion.cv_minor, 0);
    objlist_p->el_type = DNS_CLASSVERSION;
    objlist_p->el_len = sizeof(classversion);
    objlist_p->el_value_p = (unsigned char *)&classversion;
    objlist_p++;

    objlist_p->el_type = DNS_CTS;
    objlist_p->el_len = sizeof(ch_p->ch_cts);
    objlist_p->el_value_p = (unsigned char *)&ch_p->ch_cts;
    objlist_p++;

    objlist_p->el_type = DNS_OBJECTUID;
    objlist_p->el_len = sizeof(ch_p->ch_uid);
    objlist_p->el_value_p = (unsigned char *)&ch_p->ch_uid;
    objlist_p++;

    /* Get a buffer for object record (cleaned up on failures in calling
     * routine - by closing the database streem )
     */
    stream_p->db_entry.data_p = (data_record_t *)dns_malloc_temp(INITIAL_RECORD_SIZE);
    if (!stream_p->db_entry.data_p)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          " <mgmt_create_root_object: return(DNS_NONSRESOURCES)"));

	return(DNS_NONSRESOURCES);
    }
    
    stream_p->db_entry.data_buf_len = INITIAL_RECORD_SIZE;

    /* Fill buffer with object attributes */
    status = create_record(&stream_p->db_entry, objlist_p-objlist, objlist);

    if (status == DNS_SUCCESS)
    {

#if !defined(DCE_SEC)

        /* add tower */
        len = towerset_build(tower);
	userset_p = (Set_t *)tower;
	num_mem = NUM_Set(userset_p);
        if (FLAG_SET(userset_p->st_flag, ST_present) && ((i = 0) < num_mem))
            do
                {
                MemberValue_t *userset_member_p =
			(MemberValue_t *)INDEX_Set(userset_p, i);

                if (EXT8(userset_member_p->mv_flag) == MV_present)
                    {
		      /* put it in */
		      status = ts_new((Timestamp_t *)ts);
		      if (status != DNS_SUCCESS) break;
		      update_dns_build((Update_t *)upd_buf, UD_present, 
				       (Timestamp_t *)ts,
				       dns_Attribute(DNA_TOWERS), 
				       BL_true, 
				       EXT16(userset_member_p->mv_length), 
				       userset_member_p->mv_value);
		      
		      status = set_insert(&stream_p->db_entry, 
					  (Update_t *)upd_buf,
					  dns_Attribute(DNA_TOWERS), FALSE);
		      if (status != DNS_SUCCESS)
			break;
                    }
	      }
	while (++i < num_mem);
#endif

        if (status == DNS_SUCCESS)
	    /* Add access control */
	    status = create_entry_acs(user_p, stream_p->db_dir_hash_p,
		&stream_p->db_entry, &stream_p->db_dir, ch_p);

	if (status == DNS_SUCCESS)
	    {
	    SimpleName_t *ch_name = names_pointlast(&ch_p->ch_name);

	    /* Create object */
	    status = db_entry_create(stream_p, ch_name, ET_object);
	    }

	if (status == DNS_SUCCESS)
	    {
	    /* Update directory dns$lastupdate value */
	    /* Last timestamp for object is new lastupdate */
	    find_attribute(stream_p->db_entry.data_p,
			   dns_AttributeName(DNS_UTS), &set_p, &index);
	    db_dir_set_lastupdate(stream_p->db_dir_hash_p,
		(Timestamp_t *)set_p->ds_ts);
	    }
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <mgmt_create_root_object: @ end - return(sts=%d)",
	status));

    return(status);
}

/*
 * mgmt_create_root
 *	Create the root directory for a new namespace.
 *
 */
static dns_status_t 
mgmt_create_root (db_stream_t        *stream_p,
                  nsgbl_ch_t         *ch_p,
                  user_descriptor_t  *user_p,
                  version_t          *version_p)
{
    ObjUID_u            diruid;
    byte_u		statevalue;
    byte_u		typevalue;
    byte_u		convalue;
    byte_u 		inchvalue;
    ReplicaPointer_u	replica;
    Timestamp_u		newts;
    struct entrylist 	elist[16], *elist_p = elist;
    dns_status_t	status;
    ObjUID_u            epoch;
    Update_t            upd_buf;


    DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_server,
       svc_c_debug5,
       " >mgmt_create_root"));

    status = ts_new((Timestamp_t *)newts);
    if (status != DNS_SUCCESS)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          " <mgmt_create_root: return(sts=%d)",
	  status));

	return(status);
    }
    

    /* Set up initial attribute list and their values */
    elist_p->el_type = DNS_ALLUPTO;
    elist_p->el_len = sizeof(NullTimestamp);
    elist_p->el_value_p = (byte_t *)NullTimestamp;
    elist_p++;

    INS8(convalue, CG_medium);
    elist_p->el_type = DNS_CONVERGENCE;
    elist_p->el_len = sizeof(convalue);
    elist_p->el_value_p = (byte_t *)convalue;
    elist_p++;

    elist_p->el_type = DNS_CTS;
    elist_p->el_len = sizeof(newts);
    elist_p->el_value_p = (byte_t *)newts;
    elist_p++;

    elist_p->el_type = DNS_DIRECTORYVERSION;
    elist_p->el_len = sizeof(*version_p);
    elist_p->el_value_p = (byte_t *)version_p;
    elist_p++;

    status = id_new((ObjUID_t *)epoch);
    if (status != DNS_SUCCESS) return(status);
    elist_p->el_type = DNS_EPOCH;
    elist_p->el_len = sizeof(epoch);
    elist_p->el_value_p = (unsigned char *)epoch;
    elist_p++;

    INS8(inchvalue, TRUE);
    elist_p->el_type = DNS_INCHNAME;
    elist_p->el_len = sizeof(inchvalue);
    elist_p->el_value_p = (byte_t *)inchvalue;
    elist_p++;

    elist_p->el_type = DNS_LASTSKULK;
    elist_p->el_len = sizeof(newts);
    elist_p->el_value_p = (byte_t *)newts;
    elist_p++;

    elist_p->el_type = DNS_LASTUPDATE;
    elist_p->el_len = sizeof(NullTimestamp);
    elist_p->el_value_p = (byte_t *)NullTimestamp;
    elist_p++;

    id_new((ObjUID_t *)diruid);
    elist_p->el_type = DNS_OBJECTUID;
    elist_p->el_len = sizeof(diruid);
    elist_p->el_value_p = (byte_t *)diruid;
    elist_p++;

    elist_p->el_type = DNS_REPLICAS;
    elist_p->el_len = replica_build((ReplicaPointer_t *)replica,
	&ch_p->ch_uid, NULL, RT_master, &ch_p->ch_name);
    elist_p->el_value_p = (byte_t *)replica;
    elist_p++;

    INS8(statevalue, RS_on);
    elist_p->el_type = DNS_REPLICASTATE;
    elist_p->el_len = sizeof(statevalue);
    elist_p->el_value_p = (byte_t *)statevalue;
    elist_p++;

    INS8(typevalue, RT_master);
    elist_p->el_type = DNS_REPLICATYPE;
    elist_p->el_len = sizeof(typevalue);
    elist_p->el_value_p = (byte_t *)typevalue;
    elist_p++;

    elist_p->el_type = DNS_REPLICAVERSION;
    elist_p->el_len = sizeof(*version_p);
    elist_p->el_value_p = (byte_t *)version_p;
    elist_p++;

    /* Allocate memory for directory record (cleaned up on failures in calling
     * routine - by closing the database streem )
     */
    stream_p->db_dir.data_p = (data_record_t *)dns_malloc_temp(INITIAL_RECORD_SIZE);
    if (!stream_p->db_dir.data_p)
	return(DNS_NONSRESOURCES);
    stream_p->db_dir.data_buf_len = INITIAL_RECORD_SIZE;

    /* Create directory record */
    status = create_record(&stream_p->db_dir, elist_p-elist, elist);
    if (status == DNS_SUCCESS)
        {
    	/* Add initial access control */
        status = create_directory_acs(user_p, stream_p, TRUE);

	/* add RPC attributes for sec_acl functions */
	update_build(&upd_buf, UD_present, NULL,
		     AT_set, (AttributeName_t *)RPC_ClassVersion, 
		     (unsigned int *)NULL, BL_true,
		     sizeof(RPCversion), VT_byte, RPCversion);
	set_insert(&stream_p->db_dir, &upd_buf, 
		   (attribute_descriptor_t *)NULL, TRUE);
	/* Add directory to database */
        if (status == DNS_SUCCESS)
            status = db_dir_create(stream_p);

        }

   DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_server,
       svc_c_debug5,
       " <mgmt_create_root: @end - return(sts=%d)",
       status));

    return(status);
}

/*
 * mgmt_create_namespace
 *	This routine is called as the result of a management request
 *	to create a namespace.
 */
dns_status_t 
mgmt_create_namespace (nsgbl_ns_t         *ns_p,
                       user_descriptor_t  *user_p,
                       nsgbl_ch_t         *ch_p,
                       SimpleName_t       *rootch_p)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_nsgbl_ch(chBuf)
    DEB_ASCII_BUF_SimpleName(nickBuf)
    DEB_ASCII_BUF_SimpleName(rootchBuf)
    Timestamp_u		rootcts;
    ObjUID_u		rootuid;
    version_u		dir_version;
    db_stream_t		*stream_p;
    dns_status_t 	status;
    unsigned32          uuid_status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >mgmt_create_namespace(%s,%s,%s,%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        deb_ascii_SimpleName(rootchBuf,rootch_p)));

    /* Pick up the default DirectoryVersion we are using */
    get_directory_version((VersionNumber_t *)dir_version);

    /* Initialize ch_p */
    ts_new(&ch_p->ch_cts);
    status = ta_cle_get_nsnickname(ch_p);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          " <mgmt_create_namespace: (1) return(sts=%d)",
          status));

	return(status);
    }
    

    /* the NSUID is WildRoot */
    COPY_ObjUID(ch_p->ch_cellname.fn_root, &ch_p->ch_nsuid);

    /* the clearinghouse is in the root of the cell */
    COPY_FullName(&ch_p->ch_cellname, &ch_p->ch_name);
    names_append(rootch_p, &ch_p->ch_name);
    
    id_new((ObjUID_t *)&ch_p->ch_uid);
    status = ts_new((Timestamp_t *)rootcts);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          " <mgmt_create_namespace: (2) return(sts=%d)",
          status));

	return(status);
    }
    
    id_new((ObjUID_t *)rootuid);

    /* Build file name */
    sys_bld_filename(&ch_p->ch_name, ch_p->ch_filename);

    /* Create file and pseudo directory */
    status = db_create(ch_p, user_p, (version_t *)dir_version);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          " <mgmt_create_namespace: (3) return(sts=%d)",
          status));

	return(status);
    }
    

    /* Set the file state to be DB_OPEN */
    status = db_enable(ch_p, FALSE);
    if (status != DNS_SUCCESS)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          " <mgmt_create_namespace: (4) return(sts=%d)",
          status));

	return(status);
    }
    

    /* Get a stream to the file */
    status = db_open_stream(&ch_p->ch_uid, &stream_p, CREATE_NS_OP);
    if (status != DNS_SUCCESS)
    {
	db_delete(ns_p, &ch_p->ch_uid);

	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          " <mgmt_create_namespace: (5) return(sts=%d)",
          status));

	return(status);
    }

    /* Create root directory */
    stream_p->db_function = OP_CreateDirectory;
    status = mgmt_create_root(stream_p, ch_p, user_p, (version_t *)dir_version);
    if (status != DNS_SUCCESS)
    {
	db_close_stream(stream_p);
	db_delete(ns_p, &ch_p->ch_uid);

	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          " <mgmt_create_namespace: (6) return(sts=%d)",
          status));

	return(status);
    }

    /* Create clearinghouse object */
    stream_p->db_function = OP_CreateObject;
    status = mgmt_create_root_object(stream_p, ch_p, user_p);

    /* Cleanup */
    db_close_stream(stream_p);
    if (status != DNS_SUCCESS)
	db_delete(ns_p, &ch_p->ch_uid);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <mgmt_create_namespace: @end (7) return(sts=%d)",
        status));

    return(status);
}

/*
 * mgmt_remove_dir
 *	This routine loops through all replicas in the clearinghouse
 *	and attempts to remove them.
 *
 */
static dns_status_t 
mgmt_remove_dir (db_stream_t        *stream_p,
                 data_record_t      *pseudo_p,
                 user_descriptor_t  *user_p)
{
    dns_status_t	status;
    dir_hash_entry_t	*hash_p;
    int 		pos,index;
    DBSet_t		*chset_p;
    DBSetMember_t	*chvalue_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >mgmt_remove_dir"));

    /* Get set of directories in this clearinghouse */
    status = find_attribute(pseudo_p,
			    dns_AttributeName(DNS_CHDIRECTORIES),
			    &chset_p, &index);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          " <mgmt_remove_dir: (1) return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
    }
    

    /*
     * Loop through directories looking for masters.  Cannot remove
     * masters, they must be explicitly deleted with delete_directory.
     * Masters in the dead state are allowed and won't be returned
     * in the hash_uid lookup.
     */
    for (pos = 0; pos < NUM_DBSet(chset_p); pos++)
    {
	chvalue_p = (DBSetMember_t *)INDEX_DBSet(chset_p, pos);
	if (!EXT8(chvalue_p->dm_valid)) continue;

	hash_p = db_hash_uid(stream_p->db_file_p,
	    (ObjUID_t *)chvalue_p->dm_data);
	if (!hash_p) continue;
	if (db_dir_get_replica_type(hash_p) == RT_master)
	{
	    DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_server,
              svc_c_debug5,
              " <mgmt_remove_dir: (2) return(DNS_NOTEMPTY)"));

	    return(DNS_NOTEMPTY);
	}
    }

    /*
     * Repeat the loop (if we got here masters are dead).  We do
     * this deletion process in two phases to avoid leaving 
     * the clearinghouse in a state where the root is unreachable 
     * because we removed all uppointers before deciding there were 
     * masters here.
     */

    for (pos = 0; pos < NUM_DBSet(chset_p); pos++)
	{
	chvalue_p = (DBSetMember_t *)INDEX_DBSet(chset_p, pos);
	if (!EXT8(chvalue_p->dm_valid)) continue;

	/*
         * Look up the directory by hashing its uid. The hash
	 * function will return null if it doesn't exist
	 * or its state is RS_dead which means it basically
	 * does not exist, we just haven't cleaned it out yet.
	 */

	hash_p = db_hash_uid(stream_p->db_file_p,
	    (ObjUID_t *)chvalue_p->dm_data);
	if (!hash_p) continue;


        /*
         * We have a readonly replica to remove (via the usual mechanism).
         * (This would be a dandy place for a soft assertion check).
         */

	ta_replica_remove(user_p, (ObjUID_t *)chvalue_p->dm_data,
	    TRUE, stream_p);

        /*
         * Reset stream for next replica.
         */

        db_dir_release(stream_p);
        stream_p->db_dir_hash_p = NULL;

	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <mgmt_remove_dir: @end - return(DNS_SUCCESS)"));

    return(DNS_SUCCESS);
}

/*
 * mgmt_delete_ch
 *    This routine tries to delete a clearinghouse.  To do this it
 * must first remove all replicas in the directory.
 *
 */
dns_status_t 
mgmt_delete_ch (nsgbl_ns_t         *ns_p,
                user_descriptor_t  *user_p,
                nsgbl_ch_t         *ch_p)
{
    Update_t		update;
    tlog_dsc_t		tlog;
    Timestamp_u		ts;
    db_stream_t		*stream_p;
    data_record_t	*pseudo_buf_p;
    file_info_t		*file_p;
    dir_hash_entry_t	*hash_p;
    DBSet_t		*chset_p;
    DBSetMember_t	*chvalue_p;
    int 		pos, index;
    byte_t 		state, clh_disable = FALSE;
    dns_status_t 	status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >mgmt_delete_ch"));

    /* Make sure clearinghouse is open */
    status = db_enable(ch_p, FALSE);
    if (status != DNS_SUCCESS)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            " <mgmt_delete_ch: (1) return(status=%d)",
	    status));

	return(status);
    }

    status = db_open_stream(&ch_p->ch_uid, &stream_p, DELETE_CH_OP);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            " <mgmt_delete_ch: (2) return(status=%d)",
	    status));

	return(status);
    }
    

    /* Read the pseudo directory */
    status = db_pseudo_read_for_update(stream_p);
    if (status == DNS_SUCCESS)
      status  = security_check(user_p, stream_p, Delete_Entry_check, 
			     ET_clearinghouse, NULL);
    if (status != DNS_SUCCESS)
    {
	db_close_stream(stream_p);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            " <mgmt_delete_ch: (3) return(status=%d)",
	    status));

	return(status);
    }

    /* Set clearinghouse state to dying */
    state = CS_dyingCH;
    ts_new((Timestamp_t *)ts);
    tlog.len = update_dns_build((Update_t *)&update, UD_present, (Timestamp_t *)ts,
		dns_Attribute(DNS_CHSTATE), BL_true, sizeof(state), &state);
    set_replace_value(&stream_p->db_dir, (Update_t *)&update,
	dns_Attribute(DNS_CHSTATE));
    tlog.cnt = 1;
    tlog.buf_p = (unsigned char *)&update;

    /* Record pseudo directory state */
    status = db_pseudo_update(stream_p, &tlog);
    if (status != DNS_SUCCESS)
    {
	db_close_stream(stream_p);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            " <mgmt_delete_ch: (4) return(status=%d)",
	    status));

	return(status);
    }

    db_dir_release_lock(stream_p);

    /* Save pointer to pseudo directory, re-use stream to remove replicas */
    pseudo_buf_p = (data_record_t *)stream_p->db_dir.data_p;
    stream_p->db_dir.data_p = NULL;

    /* Get rid of directories */
    status = mgmt_remove_dir(stream_p, pseudo_buf_p, user_p);

    if ((status == DNS_NOTEMPTY) &&
		(db_pseudo_read_for_update(stream_p) == DNS_SUCCESS))
	{
	 dns_free((char *)pseudo_buf_p);
	 goto restore_clh;
	}

    /* Save file pointer */
    file_p = stream_p->db_file_p;

    dns_free((char *)pseudo_buf_p);
    db_close_stream(stream_p);

    /* Disable file so no-one can create any new replicas */
    db_disable(&file_p->file_ch_p->ch_uid);
    clh_disable = TRUE;

    /*
     * File state is not opened, but it is still loaded in
     * memory.  Get_stream is the only way to get access  without
     * allowing others to get at the clearinghouse and potentially
     * create a new directory
     */
    status = db_get_stream_ignore(file_p, &stream_p, OP_ReadAttribute);
    if (status != DNS_SUCCESS)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            " <mgmt_delete_ch: (6) return(status=%d)",
	    status));

	return(status);
    }
    

    status = db_pseudo_read_for_update(stream_p);
    if (status == DNS_SUCCESS)
	{
       	/* Get set of directories in this clearinghouse */
	if (find_attribute(stream_p->db_dir.data_p,
			   dns_AttributeName(DNS_CHDIRECTORIES), 
			   &chset_p, &index) == DNS_SUCCESS)

	    /* 
             * Make sure each directory is dead
             */

	    for (pos = 0; pos < NUM_DBSet(chset_p); pos++)
		{
	    	chvalue_p = (DBSetMember_t *)INDEX_DBSet(chset_p, pos);
		if (!EXT8(chvalue_p->dm_valid)) continue;

 	        /*
                 * Look up the directory by hashing its uid. The hash
	         * function will return null if it doesn't exist
	         * or its state is RS_dead which means it basically
	         * does not exist, we just haven't cleaned it out yet.
	         */

		hash_p = db_hash_uid(stream_p->db_file_p,
		    (ObjUID_t *)chvalue_p->dm_data);
		if (!hash_p) continue;

		status = DNS_NOTEMPTY;
		break;
		}
	}

    if (status == DNS_SUCCESS)
    {
	db_close_stream(stream_p);
	db_delete(ns_p, &ch_p->ch_uid);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            " <mgmt_delete_ch: (7) return(status=%d)",
	    status));

	return(status);
    }

restore_clh:

    /* Failed, change state back */
    state = CS_on;
    ts_new((Timestamp_t *)ts);
    tlog.len = update_dns_build((Update_t *)&update, UD_present, (Timestamp_t *)ts,
		dns_Attribute(DNS_CHSTATE), BL_true, sizeof(state), &state);
    set_replace_value(&stream_p->db_dir, (Update_t *)&update,
	dns_Attribute(DNS_CHSTATE));
    db_pseudo_update(stream_p, &tlog);

    db_free_stream(stream_p);

    if (clh_disable) db_enable(ch_p, TRUE);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <mgmt_delete_ch: @end (8) return(status=%d)",
        status));

    return(status);
}

/*
 * mgmt_clear_ch
 *    This routine tries to clear a clearinghouse.  This is generally the first
 *    step to permit the clearinghouse to be moved to another location.  First
 *    must make sure that clearinghouse is now set up properly, and then it is
 *    disabled and the checkpoint and transaction log files cleared.
 *
 */
dns_status_t 
mgmt_clear_ch (user_descriptor_t  *user_p,
               nsgbl_ch_t         *ch_p)
{
    dns_status_t 	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <mgmt_clear_ch"));

    /* Make sure clearinghouse is open */
    status = db_enable(ch_p, FALSE);
    if (status != DNS_SUCCESS) 
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug5,
            " <mgmt_clear_ch: return(status=%d)",
	    status));

	return(status);
    }
    db_clear(&ch_p->ch_uid);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <mgmt_clear_ch: @ end - return( DNS_SUCCESS)"));

    return( DNS_SUCCESS);
}

/*
 * mgmt_add_ch
 *	This function checks a database to make sure it is a valid
 *	clearinghouse file.  If it is the uid is added to the
 *	clearinghouse descriptor.
 *
 */
dns_status_t 
mgmt_add_ch (nsgbl_ch_t *ch_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <mgmt_add_ch"));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <mgmt_add_ch: @ end - return( DNS_SUCCESS)"));

    return(DNS_SUCCESS);
}

/*
 * mgmt_check_linked
 *    This routine checks to see if the input full name contains a link.
 *    If it does, it resolves the input name and returns the resolved name
 *    in 'result_p'. Else, it just copies the input name into the output
 *    name and returns.
 *
 * Input:
 *    directory_p = address of new directory fullname
 *
 * Output:
 *    result_p = address to store resolved name into
 *
 * Returns
 *    TRUE if input name contains a link.
 */
dns_status_t 
mgmt_check_linked (FullName_t  *directory_p,
                   FullName_t  *result_p)
{
    SimpleName_u	saved_name;
    register SimpleName_t *last_p;
    register dns_status_t status, oldLength;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >mgmt_check_linked"));

    /* Copy the dir name into the resolved name */
    COPY_FullName(directory_p, result_p);

    last_p = names_pointlast(directory_p);
    /* Peel off last directory simple name */
    COPY_SimpleName(last_p, saved_name);
    COPY_SimpleName(NullSimpleName, last_p);
    oldLength = EXT16(directory_p->fn_length);
    INS16(directory_p->fn_length, oldLength - LEN_SimpleName(saved_name));

    status = clerk_resolve_name(directory_p, result_p);

    /* Restore directory simple name */
    COPY_SimpleName(saved_name, last_p);
    INS16(directory_p->fn_length, oldLength);

    /* Not linked, use original name */
    if (status != DNS_SUCCESS) return (status);

    /* Append original directory simple name to resolved name */
    last_p = (SimpleName_t *)names_pointlast(result_p);
    if (EMPTY_FullName(result_p))
        last_p = (SimpleName_t *)result_p->fn_name; /* no-op already here */
    else
	last_p = (SimpleName_t *)SKIP_SimpleName(last_p);

    /* Copy directory simple name and terminating name */
    COPY_SimpleName(saved_name, last_p);
    COPY_SimpleName(NullSimpleName, SKIP_SimpleName(last_p));

    /* Update length, terminating name already included */
    INS16(result_p->fn_length,
	EXT16(result_p->fn_length) + LEN_SimpleName(saved_name));

    return (DNS_SUCCESS);
}






