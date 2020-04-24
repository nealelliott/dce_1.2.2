/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_policy.c,v $
 * Revision 1.1.13.3  1996/02/18  00:21:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:38  marty]
 *
 * Revision 1.1.13.2  1995/12/08  18:00:23  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:11 UTC  greg
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	merge
 * 	[1995/03/29  15:05 UTC  hanfei  /main/SEC_migrate/3]
 * 
 * 	add rsdb_activate_xattrschema_acl_dce11() for dce1.1 migration :
 * 	when software compat version is set then restoring
 * 	xattrschema acl so xattrschema is visible.
 * 	[1995/03/29  14:49 UTC  hanfei  /main/hanfei_migrate_bl2/3]
 * 
 * Revision 1.1.7.5  1994/08/25  16:30:45  hanfei
 * 	dce1.1 rgy database backward compatibility work
 * 	[1994/08/25  14:42:35  hanfei]
 * 
 * Revision 1.1.7.4  1994/08/04  16:14:53  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:14:05  mdf]
 * 
 * Revision 1.1.7.2  1994/04/22  19:14:23  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:28:11  rps]
 * 
 * Revision 1.1.8.2  1994/03/29  23:48:52  rps
 * 	Handle rsdb_pgodir_type for rgy polymorphism
 * 
 * Revision 1.1.7.1  1994/03/02  17:56:10  hanfei
 * 	Initial security drop
 * 	[1994/02/18  20:52:47  hanfei]
 * 
 * 	HP revision /main/ODESSA_2/2  1994/02/10  04:51  ahop
 * 	 merge ODESSA_ERA4 up to ODESSA_2
 * 
 * 	HP revision /main/ODESSA_2/ODESSA_ERA4/1  1994/02/07  19:44  mdf
 * 	merge from ODESSA_ERA3 to ODESSA_ERA4.
 * 
 * 	HP revision /main/ODESSA_2/ODESSA_ERA3/1  1994/02/07  01:48  ahop
 * 	 merge it up
 * 
 * 	HP revision /main/ODESSA_2/ahop_odss_era_bl8/2  1994/02/05  22:21  ahop
 * 	initialize attr_list_ids to END_OF_LIST at create time
 * 
 * 	HP revision /main/ODESSA_2/ahop_odss_era_bl8/1  1994/02/05  20:58  ahop
 * 	add attr_list_ids to rgy_info
 * 
 * 	HP revision /main/ODESSA_ERA2/2  1994/01/31  14:29  ahop
 * 	 merge forward
 * 
 * 	HP revision /main/ODESSA_ERA1/ahop_odss_era_bl4/2  1994/01/31  02:57  ahop
 * 	add rsdb_policy_get_num_attrs
 * 
 * 	HP revision /main/ODESSA_ERA1/ahop_odss_era_bl4/1  1994/01/30  01:02  ahop
 * 	add num_attrs to policy object (rgy_info)
 * 
 * Revision 1.1.4.7  1993/03/05  18:31:07  frisco
 * 	add function to set server name for sec_salvage_db
 * 	[1993/03/03  20:11:00  frisco]
 * 
 * Revision 1.1.4.6  1993/02/19  17:15:44  ahop
 * 	dbcp: add rsdb_init_rgy_info_dce102 for backwards compatibility.
 * 	[1993/02/19  08:46:14  ahop]
 * 
 * Revision 1.1.4.5  1992/12/29  16:39:25  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:29:00  zeliff]
 * 
 * Revision 1.1.4.4  1992/11/20  15:55:05  ahop
 * 	Pass file_version by reference in get_rgy_info_file_version
 * 	[1992/11/18  21:40:29  ahop]
 * 
 * 	Remove unused old_enc_master_key and get_master_key_by_vno
 * 	Add rsdb_get_rgy_info_file_version.
 * 	[1992/11/18  19:32:56  ahop]
 * 
 * Revision 1.1.4.3  1992/09/29  20:14:18  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:41:05  sekhar]
 * 
 * Revision 1.1.4.2  1992/08/31  18:28:43  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:13:11  sekhar]
 * 
 * Revision 1.1.2.2  1992/03/30  16:36:02  ahop
 * 	 support locksmith mode
 * 	[1992/03/28  00:02:01  ahop]
 * 
 * Revision 1.1  1992/01/19  14:44:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* 
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/* 
 *      Registry Server - database interlude - policy and properties management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
	"$Id: rsdb_policy.c,v 1.1.13.3 1996/02/18 00:21:52 marty Exp $";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <un_strng.h>
#include <bstring.h>
#include <un_io.h>
#include <rsdb_compat_pvt.h> 
#include <rs_reserved.h>

#include <dce/dce.h>
#include <rsdb_policy.h>
#include <rsdb_util.h>
#include <rsdb_pgo_util.h>
#include <rsdb_pgo.h>
#include <rsdb_acl.h>
#include <rsdb_acct.h>
#include <rsdb_pvt.h> 
#include <rsdb_compat.h> 
#include <rs_base.h>
#include <rs_util.h>
#include <sec_svc.h>

#define CURRENT_RGY_FILE_VERSION 3
#define DCE102_RGY_FILE_VERSION  2
#define DCE11_RGY_FILE_VERSION   3

PRIVATE struct rsdb_rgy_file_info {
    long                           file_version;
    sec_rgy_properties_t           properties;
    sec_rgy_plcy_t                 policy;
    sec_rgy_plcy_auth_t            auth_policy;
    rsdb_pvt_id_t                  sequential_key;
    sec_timeval_sec_t              domain_cache_state[sec_rgy_domain_last];
    rsdb_pvt_id_t                  policy_acl_id;
    rsdb_pvt_id_t                  replist_acl_id;
    rsdb_pvt_id_t                  attr_schema_acl_id;
    signed32			   policy_num_attrs; 
    signed32			   replist_num_attrs; 
    signed32			   attr_schema_num_attrs; 
    rsdb_pvt_id_t		   policy_attr_list_id;
    rsdb_pvt_id_t		   replist_attr_list_id;
    rsdb_pvt_id_t		   attr_schema_attr_list_id;
    sec_rgy_name_t                 myname; /* my server name */
    rsdb_encrypt_key_t             enc_master_key;
    rsdb_pvt_id_t                  login_activity_id;
} rgy_info;

PUBLIC void rsdb_get_rgy_info_file_version
#ifndef __STDC__
    ( file_version )
    long            *file_version;        /* [out] */
#else
    (
    long            *file_version
    )
#endif
{
    *file_version = rgy_info.file_version;
}

PUBLIC void rsdb_get_cache_state
#ifndef __STDC__
    ( p_dtm, g_dtm, o_dtm ) 
    sec_timeval_sec_t  *p_dtm;
    sec_timeval_sec_t  *g_dtm;
    sec_timeval_sec_t  *o_dtm;
#else
  (
    sec_timeval_sec_t  *p_dtm,
    sec_timeval_sec_t  *g_dtm,
    sec_timeval_sec_t  *o_dtm
  )
#endif
{
    *p_dtm = rgy_info.domain_cache_state[sec_rgy_domain_person];
    *g_dtm = rgy_info.domain_cache_state[sec_rgy_domain_group];
    *o_dtm = rgy_info.domain_cache_state[sec_rgy_domain_org];
}

PUBLIC void rsdb_set_domain_cache_state
#ifndef __STDC__
    ( domain, dtm ) 
    sec_rgy_domain_t   domain;
    sec_timeval_sec_t  *dtm;
#else
  (
    sec_rgy_domain_t   domain,
    sec_timeval_sec_t  *dtm
  )
#endif
{
    if ((domain >= 0) && (domain < sec_rgy_domain_last)) {
        rgy_info.domain_cache_state[domain] = *dtm;
    }
}

PUBLIC void rsdb_get_domain_cache_state
#ifndef __STDC__
    ( domain, dtm ) 
    sec_rgy_domain_t   domain;
    sec_timeval_sec_t  *dtm;
#else
  (
    sec_rgy_domain_t   domain,
    sec_timeval_sec_t  *dtm
  )
#endif
{
    if ((domain >= 0) && (domain < sec_rgy_domain_last)) {
        *dtm = rgy_info.domain_cache_state[domain];
    }
}

PRIVATE boolean32 rsdb_must_init_reps = false;

PUBLIC void rsdb_set_init_replicas
#ifndef __STDC__
    ( state ) 
    boolean32   state;
#else
  (
    boolean32   state
  )
#endif
{
    rsdb_must_init_reps = state;
}

PUBLIC boolean32 rsdb_must_init_replicas (
#ifdef __STDC__
    void
#endif
)
{
    return rsdb_must_init_reps;
}

PUBLIC void rsdb_init_rgy_info
#ifndef __STDC__
    ( rgy_creator, rgy_local_cell, myname )
    sec_id_t            *rgy_creator;
    sec_id_t            *rgy_local_cell;
    unsigned_char_p_t   myname;
#else
   (
    sec_id_t            *rgy_creator,
    sec_id_t            *rgy_local_cell,
    unsigned_char_p_t   myname
   )
#endif
{
    bzero((Pointer) &rgy_info, sizeof(rgy_info));
    rgy_info.file_version = CURRENT_RGY_FILE_VERSION; 
    rgy_info.policy_acl_id = rsdb_acl_create_default_acl(
        rgy_creator, rgy_local_cell, rsdb_acl_mgr_type_rgy);
    rgy_info.replist_acl_id = rsdb_acl_create_default_acl(
        rgy_creator, rgy_local_cell, rsdb_acl_mgr_type_replist);
    rgy_info.attr_schema_acl_id = rsdb_acl_create_default_acl(
        rgy_creator, rgy_local_cell, rsdb_acl_mgr_type_attr_schema);
    rgy_info.policy_attr_list_id	= END_OF_LIST;
    rgy_info.replist_attr_list_id	= END_OF_LIST;
    rgy_info.attr_schema_attr_list_id	= END_OF_LIST;
    rgy_info.login_activity_id = 0; 
    u_strncpy(rgy_info.myname, myname, sizeof(rgy_info.myname) - 1);
}

/*
 *  rsdb_init_rgy_info_dce102
 *
 *  This is a backwards compatibility operation.
 *  It is called when a version dce1.0.2 secd is
 *  started on top of a dce1.0.1 registry database.
 *  Due to its dependencies on rsdb_acl operations,
 *  this routine should be called after rsdb_acl_init
 *  has been called.
 */
PUBLIC void rsdb_init_rgy_info_dce102
#ifndef __STDC__
    ( properties, policy, auth_policy, sequential_key,
      p_dtm, g_dtm, o_dtm, policy_acl_id,
      myname, enc_master_key, st_p )
    sec_rgy_properties_t    *properties;
    sec_rgy_plcy_t          *policy;
    sec_rgy_plcy_auth_t     *auth_policy;
    rsdb_pvt_id_t           sequential_key;
    sec_timeval_sec_t       *p_dtm;
    sec_timeval_sec_t       *g_dtm;
    sec_timeval_sec_t       *o_dtm;
    rsdb_pvt_id_t           policy_acl_id;
    sec_rgy_name_t          myname;
    rsdb_encrypt_key_t      *enc_master_key;
    error_status_t          *st_p;
#else
   (
    sec_rgy_properties_t    *properties,
    sec_rgy_plcy_t          *policy,
    sec_rgy_plcy_auth_t     *auth_policy,
    rsdb_pvt_id_t           sequential_key,
    sec_timeval_sec_t       *p_dtm,
    sec_timeval_sec_t       *g_dtm,
    sec_timeval_sec_t       *o_dtm,
    rsdb_pvt_id_t           policy_acl_id,
    sec_rgy_name_t          myname,
    rsdb_encrypt_key_t      *enc_master_key,
    error_status_t          *st_p
   )
#endif
{
    sec_id_t                local_cell;

    bzero((Pointer) &rgy_info, sizeof(rgy_info));
    rgy_info.file_version = DCE102_RGY_FILE_VERSION;
    rgy_info.properties = *properties;
    u_strncpy(rgy_info.properties.realm, properties->realm,
              sizeof(rgy_info.properties.realm) - 1);
    rgy_info.policy = *policy;
    rgy_info.auth_policy = *auth_policy;
    rgy_info.sequential_key = sequential_key;
    rgy_info.domain_cache_state[sec_rgy_domain_person] = *p_dtm;
    rgy_info.domain_cache_state[sec_rgy_domain_group] = *g_dtm;
    rgy_info.domain_cache_state[sec_rgy_domain_org] = *o_dtm;
    rgy_info.policy_acl_id = policy_acl_id;

    /* Make the policy acl conform to dce102 semantics and
     * create the new replist acl.
     * rgy_info.sequential_key must be set before calling
     * rsdb_compat_policy_replist_acls.
     */
    local_cell.uuid = properties->realm_uuid;
    local_cell.name = 0;

    rsdb_compat_policy_replist_acls(&local_cell, policy_acl_id,
                                    &rgy_info.replist_acl_id,
                                    st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }
    u_strncpy(rgy_info.myname, myname, sizeof(rgy_info.myname) - 1);
    rgy_info.enc_master_key = *enc_master_key;
    bcopy(enc_master_key->contents, rgy_info.enc_master_key.contents,
          sizeof(rgy_info.enc_master_key.contents));
}


/*
 *  rsdb_init_rgy_info_dce11
 *  This is a backwards compatibility operation.
 *  It is called when a version dce1.1 secd is
 *  started on top of a dce1.0.2 registry database.
 *  This routines should be called after 
 *  rsdb_load_relation() is called.
 */
PUBLIC void rsdb_init_rgy_info_dce11 (
    rsdb_rgy_file_info_dce102_t     rgy_info_dce102,
    error_status_t                  *st_p
  )
{
    sec_id_t                local_cell;
    sec_id_t                rgy_creator;
    long                    rgy_creator_unix_id;
    rsdb_acl_t      	    *xattrschema_acl_p;
    int			    i;

    bzero((Pointer) &rgy_info, sizeof(rgy_info));
    rgy_info.file_version = DCE11_RGY_FILE_VERSION;
    rgy_info.properties = rgy_info_dce102.properties;
    u_strncpy(rgy_info.properties.realm, rgy_info_dce102.properties.realm,
              sizeof(rgy_info.properties.realm) - 1);
    rgy_info.policy = rgy_info_dce102.policy;
    rgy_info.auth_policy = rgy_info_dce102.auth_policy;
    rgy_info.sequential_key = rgy_info_dce102.sequential_key;
    rgy_info.domain_cache_state[sec_rgy_domain_person] = 
		rgy_info_dce102.domain_cache_state[sec_rgy_domain_person];
    rgy_info.domain_cache_state[sec_rgy_domain_group] = 
		rgy_info_dce102.domain_cache_state[sec_rgy_domain_group];
    rgy_info.domain_cache_state[sec_rgy_domain_org] = 
		rgy_info_dce102.domain_cache_state[sec_rgy_domain_org];
    rgy_info.policy_acl_id = rgy_info_dce102.policy_acl_id;
    rgy_info.replist_acl_id = rgy_info_dce102.replist_acl_id;

    dce_cf_get_cell_name((char **)&local_cell.name, st_p);
    if ( BAD_STATUS(st_p) ) {
	return;
    }
    local_cell.uuid = rgy_info.properties.realm_uuid;

    rgy_creator.name = (unsigned_char_t *)rs_default_creator;
    rgy_creator_unix_id = rgyc_uxid_creator;
    rs_setup_creator(&rgy_creator, rgy_creator_unix_id, st_p);
    if (BAD_STATUS(st_p)) {
        return;
    }

    rgy_info.attr_schema_acl_id = rsdb_acl_create_default_acl(
        &rgy_creator, &local_cell, rsdb_acl_mgr_type_attr_schema);

    /* set all xattrschema's acl entries to be null so nobody */
    /* can access it until software compat version is set right */
    rsdb_acl_get_by_id(rgy_info.attr_schema_acl_id, &xattrschema_acl_p,
	st_p);
    if (BAD_STATUS(st_p)) {
        return;
    }
    for ( i= 0; i<xattrschema_acl_p->num_entries; i++ ) {
        xattrschema_acl_p->entries[i].perms        = 0;
    }
    rsdb_acl_store_by_id(rgy_info.attr_schema_acl_id, xattrschema_acl_p);
    free(xattrschema_acl_p);

    rgy_info.policy_attr_list_id	= END_OF_LIST;
    rgy_info.replist_attr_list_id	= END_OF_LIST;
    rgy_info.attr_schema_attr_list_id	= END_OF_LIST;
    rgy_info.policy_num_attrs = 0;
    rgy_info.replist_num_attrs = 0;
    rgy_info.attr_schema_num_attrs = 0;

    u_strncpy(rgy_info.myname, rgy_info_dce102.myname, 
		sizeof(rgy_info_dce102.myname) - 1);
    rgy_info.enc_master_key = rgy_info_dce102.enc_master_key;
    bcopy(rgy_info_dce102.enc_master_key.contents, 
		rgy_info.enc_master_key.contents,
          	sizeof(rgy_info.enc_master_key.contents));
    rgy_info.login_activity_id = 0;
}


/*
 *  rsdb_activate_xattrschema_acl_dce11
 *  This is for migrating rgy database from dce 1.0.3 to dce 1.1.
 *  Xattrschema's acl manager was created when dce 1.1 bits is
 *  load and when rgy database is converted.  But compat software
 *  version is not set right yet.  So at database conversion, all
 *  permission bits are taken out to prevent xattrschema visible.  
 *  And this function is invoked when software version is set right 
 *  so the default bits are put back in.
 */
PUBLIC void rsdb_activate_xattrschema_acl_dce11 (
    sec_id_foreign_t                rgy_creator,
    error_status_t                  *st_p
  )
{
    rsdb_acl_t      	    *xattrschema_acl_p;
    int			    i;

    /* add xattrschema's acl's with their default permission bits */
    rsdb_acl_get_by_id(rgy_info.attr_schema_acl_id, &xattrschema_acl_p,
	st_p);
    if (BAD_STATUS(st_p)) {
        return;
    }
    for ( i= 0; i<xattrschema_acl_p->num_entries; i++ ) {
	switch (xattrschema_acl_p->entries[i].entry_type) {
	    case sec_acl_e_type_user:
    		if ( uuid_equal(&rgy_creator.realm.uuid, 
				&xattrschema_acl_p->default_realm.uuid, st_p ) ) {
		/* creator is local to this cell */
		    xattrschema_acl_p->entries[i].entry_info.id.uuid = 
				rgy_creator.id.uuid;
		}
		else { /* a foreign cell user is the initiator */
		    xattrschema_acl_p->entries[i].entry_type = 
				sec_acl_e_type_foreign_user;
		    xattrschema_acl_p->entries[i].entry_info.foreign_id.id.uuid = 
				rgy_creator.id.uuid;
		    xattrschema_acl_p->entries[i].entry_info.foreign_id.realm.uuid = 
				rgy_creator.realm.uuid;
		}
	    	xattrschema_acl_p->entries[i].perms = 
					      sec_acl_perm_delete	|
					      sec_acl_perm_insert	|
					      sec_acl_perm_mgmt_info	|
					      sec_acl_perm_read		|
					      sec_acl_perm_owner;
		break;
            case sec_acl_e_type_any_other:
            	xattrschema_acl_p->entries[i].perms = sec_acl_perm_read;
		break;
	    case sec_acl_e_type_other_obj:
            	xattrschema_acl_p->entries[i].perms = sec_acl_perm_read;
		break;
	    case sec_acl_e_type_unauthenticated:
		xattrschema_acl_p->entries[i].perms = sec_acl_perm_read;
		break;
	}
    }
    rsdb_acl_store_by_id(rgy_info.attr_schema_acl_id, xattrschema_acl_p);
    free(xattrschema_acl_p);
}


PUBLIC void rsdb_load_rgy_info
#ifndef __STDC__
    ( fd )  
    int  fd;
#else
  (
    int  fd
  )
#endif
{
    int len;
    int i;

    len = read(fd, (char *) &rgy_info, sizeof(rgy_info));
    if (len != sizeof(rgy_info)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_file_read_error,
                       fd);
    }
}


PUBLIC void rsdb_store_rgy_info
#ifndef __STDC__
    ( fd )  
    int  fd;
#else
  (
    int  fd
  )
#endif
{
    write(fd, (char *) &rgy_info, sizeof(rgy_info));
    fsync(fd);
}                                                 

PUBLIC rsdb_pvt_id_t rsdb_get_rgy_acl_id (
#ifdef __STDC__
    void
#endif
)
{
    return(rgy_info.policy_acl_id);
}

PUBLIC rsdb_pvt_id_t rsdb_get_replist_acl_id (
#ifdef __STDC__
    void
#endif
)
{
    return(rgy_info.replist_acl_id);
}
PUBLIC rsdb_pvt_id_t rsdb_get_attr_schema_acl_id (
#ifdef __STDC__
    void
#endif
)
{
    return(rgy_info.attr_schema_acl_id);
}

PUBLIC void rsdb_properties_get_info
#ifndef __STDC__
    ( properties, status ) 
    sec_rgy_properties_t   *(properties);  /* [out] */
    error_status_t         *(status);      /* [out] */
#else
  (
    sec_rgy_properties_t   *(properties),  /* [out] */
    error_status_t         *(status)
  )
#endif
{
    *properties = rgy_info.properties;
    CLEAR_STATUS(status);
}

PUBLIC void rsdb_properties_set_info
#ifndef __STDC__
    ( properties, status ) 
    sec_rgy_properties_t   *(properties);  /* [in] */
    error_status_t         *(status);      /* [out] */
#else
  (
    sec_rgy_properties_t   *(properties),  /* [in] */
    error_status_t         *(status)
  )
#endif
{
    sec_rgy_domain_t        d;
    rsdb_pvt_id_t           item_id;
    rsdb_named_item_t       item;  
    int                     unix_id;

    CLEAR_STATUS(status);

    /* Don't allow max_unix_id to be lower than any 
     * of the low_unix_ids.
     */
    if( properties->max_unix_id < properties->low_unix_id_person ||
        properties->max_unix_id < properties->low_unix_id_group  ||
        properties->max_unix_id < properties->low_unix_id_org ) { 
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }
    /* If the low_unix id has changed,
     * reset the next assigned unix id for the domain.
     * Only perform this loop if pgo_create or pgo_init has been run
     * (test that domain_info[sec_rgy_domain_person].db == rsdb_person_db).
     */                        
    if(domain_info[sec_rgy_domain_person].db == rsdb_person_db) {
        for (d = sec_rgy_domain_person; d < sec_rgy_domain_last; d++) {
            switch(d) {
                case sec_rgy_domain_person:
                    if(properties->low_unix_id_person !=
                       rgy_info.properties.low_unix_id_person) {
                        unix_id = properties->low_unix_id_person;
                        break; 
                    } else {
                        continue;
                    }
                case sec_rgy_domain_group:
                    if(properties->low_unix_id_group !=
                           rgy_info.properties.low_unix_id_group) {
                        unix_id = properties->low_unix_id_group;
                        break; 
                    } else {
                        continue;
                    }
                case sec_rgy_domain_org:
                    if(properties->low_unix_id_org !=
                       rgy_info.properties.low_unix_id_org) {
                        unix_id = properties->low_unix_id_org;
                        break; 
                    } else {
                        continue;
                    }
            }
	    RSDB_PGO(&domain_info[d].last).unix_id = unix_id;
            item_id = END_OF_LIST;
            if(!rsdb_pgo_util_get_by_id(d, item_id, &item)) {
                dce_svc_printf(SEC_RSDB_CANT_GET_ITEM_MSG);
            }
            RSDB_PGO(&item).unix_id = unix_id;
            rsdb_util_store_item_by_id(d, item_id, &item);
        }
    }

    /*
     * don't allow realm name/uuid changes.  realm can only be 
     * set with rsdb_properties_set_realm function.  We don't
     * flag an error, we just make sure that it stays the same. 
     * Embedded_unix_id flag must always be set in DCE 1.0
     */
    strncpy( (char *) properties->realm, (char *) rgy_info.properties.realm,
	    sizeof(rgy_info.properties.realm) - 1);
    properties->realm[sizeof(properties->realm) - 1] = '\0';
    properties->realm_uuid = rgy_info.properties.realm_uuid;

    rgy_info.properties = *properties;
    rgy_info.properties.flags |= sec_rgy_prop_embedded_unix_id;
 }

/*
 * Sets the realm name and uuid in the rgy properties.  Should be used
 * sparingly, and not by anything coming over the wire until we
 * have all the issues related to realm name changes sorted out
 * If incoming realm name has length 0, don't set the name.
 * If incoming uuid is nil, don't set the uuid.
 */
PUBLIC void rsdb_properties_set_realm
#ifndef __STDC__
    ( realm_name, realm_uuid, status ) 
    sec_rgy_name_t   (realm_name);      /* [in] */
    uuid_t           *realm_uuid;       /* [in] */
    error_status_t   *(status);
#else
  (
    sec_rgy_name_t   (realm_name),      /* [in] */
    uuid_t           *realm_uuid,       /* [in] */
    error_status_t   *(status)
  )
#endif
{
    CLEAR_STATUS(status);

    if (u_strlen(realm_name) > (size_t) 0) {
        u_strncpy(rgy_info.properties.realm, realm_name,
	        sizeof(rgy_info.properties.realm) - 1);
    }
    rgy_info.properties.realm[sizeof(rgy_info.properties.realm) - 1] = '\0';
    if (!(uuid_is_nil(realm_uuid, status))) {
        rgy_info.properties.realm_uuid = *realm_uuid;
    }
}    
 
PUBLIC void rsdb_properties_get_realm_name
#ifndef __STDC__
    ( realm_name )
    sec_rgy_name_t   (realm_name);      /* [in, out] */
#else
   (
    sec_rgy_name_t   (realm_name)
   )
#endif
{
    u_strncpy( realm_name, rgy_info.properties.realm,
              sizeof(rgy_info.properties.realm) - 1);
    realm_name[sizeof(rgy_info.properties.realm) - 1] = '\0';
}    

 
PUBLIC void rsdb_get_my_server_name
#ifndef __STDC__
    ( my_name )
    sec_rgy_name_t   (my_name);      /* [out] */
#else
   (
    sec_rgy_name_t   (my_name)
   )
#endif
{
    u_strcpy( my_name, rgy_info.myname );
}    


PUBLIC void rsdb_set_my_server_name
#ifndef __STDC__
    ( my_name )
    sec_rgy_name_t   (my_name);      /* [out] */
#else
   (
    sec_rgy_name_t   (my_name)
   )
#endif
{
    u_strcpy( rgy_info.myname, my_name );
}    


PUBLIC boolean32 rsdb_is_readonly (
#ifdef __STDC__
    void
#endif
)
{
    if (FLAG_SET(rgy_info.properties.flags, sec_rgy_prop_readonly)) {
        return true;
    }

    return false;
}
                      

PUBLIC boolean32 rsdb_prop_flag_is_set ( properties_flag )
    unsigned32  properties_flag;
{
    if (FLAG_SET(rgy_info.properties.flags, properties_flag)) {
        return true;
    }

    return false;
}


PRIVATE unsigned long most_restrictive_policy
#ifndef __STDC__
    ( p1, p2 ) 
    unsigned long   p1;
    unsigned long   p2;
#else
  (
    unsigned long   p1,
    unsigned long   p2
  )
#endif
{
    if (p1 == 0)
        return p2;

    if (p2 == 0)
        return p1;

    if (p1 < p2)
        return p1;

    return p2;
}

PRIVATE unsigned long least_restrictive_policy
#ifndef __STDC__
    ( p1, p2 )
    unsigned long   p1;
    unsigned long   p2;
#else
  (
    unsigned long   p1,
    unsigned long   p2
  )
#endif
{
    if (p1 == 0)
        return p1;

    if (p2 == 0)
        return p2;

    if (p1 > p2)
        return p1;

    return p2;
}


/* For each field in in_policy, set the
 * equivalent field in out_policy
 * to the in_policy value if the in_policy value is less
 * restrictive than the out_policy value.
 * If in_policy value == rsdb_policy_field_not_set (-1),
 * don't change out_policy field.
 */
PRIVATE void set_least_restrictive_policy
#ifndef __STDC__
    ( in_policy, out_policy )
    sec_rgy_plcy_t  *(in_policy);   /* [in] */
    sec_rgy_plcy_t  *(out_policy);  /* [in, out] */
#else
  (
    sec_rgy_plcy_t  *(in_policy),   /* [in] */
    sec_rgy_plcy_t  *(out_policy)   /* [in,out] */
  )
#endif
{
    if(in_policy->passwd_min_len != rsdb_policy_field_not_set)
        out_policy->passwd_min_len  = MIN(out_policy->passwd_min_len,
                                          in_policy->passwd_min_len);
    if(in_policy->passwd_lifetime != rsdb_policy_field_not_set)
        out_policy->passwd_lifetime =
            least_restrictive_policy(out_policy->passwd_lifetime,
                                     in_policy->passwd_lifetime);
    if(in_policy->passwd_exp_date != rsdb_policy_field_not_set)
        out_policy->passwd_exp_date =
            least_restrictive_policy(out_policy->passwd_exp_date,
                                     in_policy->passwd_exp_date);
    if(in_policy->acct_lifespan != rsdb_policy_field_not_set)
        out_policy->acct_lifespan   =
            least_restrictive_policy(out_policy->acct_lifespan,
                                     in_policy->acct_lifespan);
    if(in_policy->passwd_flags != rsdb_policy_field_not_set)
        out_policy->passwd_flags    = out_policy->passwd_flags &
                                      in_policy->passwd_flags;
}

PUBLIC void rsdb_policy_get_effective
#ifndef __STDC__
    ( org_name, policy, status ) 
    sec_rgy_name_t  org_name;       /* [in] */
    sec_rgy_plcy_t  *(policy);      /* [out] */
    error_status_t  *(status);      /* [out] */
#else
  (
    sec_rgy_name_t  org_name,       /* [in] */
    sec_rgy_plcy_t  *(policy),      /* [out] */
    error_status_t  *(status)
  )
#endif
{
    sec_rgy_plcy_t  sec_rgy_plcy;
    sec_rgy_plcy_t  org_policy;
    error_status_t  st;

    rsdb_policy_get_info( (unsigned_char_p_t)"", &sec_rgy_plcy, status);
    if (BAD_STATUS(status))
        return;

    rsdb_policy_get_info(org_name, &org_policy, &st);
    if (BAD_STATUS(&st)) {
        *policy = sec_rgy_plcy;
    } else {
        /*
        * Calculate the most restrictive policy
        */
        policy->passwd_min_len  = MAX(sec_rgy_plcy.passwd_min_len,
                                        org_policy.passwd_min_len);
        policy->passwd_lifetime = 
            most_restrictive_policy(sec_rgy_plcy.passwd_lifetime,
                                        org_policy.passwd_lifetime);
        policy->passwd_exp_date = 
            most_restrictive_policy(sec_rgy_plcy.passwd_exp_date,
                                        org_policy.passwd_exp_date);
        policy->acct_lifespan   = 
            most_restrictive_policy(sec_rgy_plcy.acct_lifespan,
                                        org_policy.acct_lifespan);
        policy->passwd_flags    = sec_rgy_plcy.passwd_flags 
                                    | org_policy.passwd_flags;
    }
}

PUBLIC void rsdb_policy_get_info
#ifndef __STDC__
    ( organization, policy_data, status ) 
    sec_rgy_name_t  organization;   /* [in] */
    sec_rgy_plcy_t  *(policy_data); /* [out] */
    error_status_t  *(status);      /* [out] */
#else
  (
    sec_rgy_name_t  organization,   /* [in] */
    sec_rgy_plcy_t  *(policy_data), /* [out] */
    error_status_t  *(status)
  )
#endif
{
    CLEAR_STATUS(status);

    if (RGY_POLICY(organization)) {
        *policy_data = rgy_info.policy;
    } else {
        rsdb_pgo_policy_get_info(organization, policy_data, status);
    }
}

PUBLIC void rsdb_policy_set_info
#ifndef __STDC__
    ( organization, policy_data, status ) 
    sec_rgy_name_t  organization;   /* [in] */
    sec_rgy_plcy_t  *(policy_data); /* [in] */
    error_status_t  *(status);      /* [in] */
#else
  (
    sec_rgy_name_t  organization,   /* [in] */
    sec_rgy_plcy_t  *(policy_data), /* [in] */
    error_status_t  *(status)
  )
#endif
{
    CLEAR_STATUS(status);

    if (RGY_POLICY(organization)) {
        rgy_info.policy = *policy_data;
    } else {
        rsdb_pgo_policy_set_info(organization, policy_data, status);
    }
}

/* r s d b _ p o l i c y _ u p d _ n u m _ a t t r s 
 *
 * Add num to rgy_info.*_num_attrs for the object
 * identified by obj_id.
 * To subtract, num must be negative.
 */
PUBLIC void rsdb_policy_upd_num_attrs
(
    rsdb_pvt_id_t	obj_id,		/* [in] */
    signed32		num		/* [in] */
)
{
    signed32		*num_attrs_p;

    if(obj_id == RSDB_POLICY_OBJ_ID) {
	num_attrs_p = &rgy_info.policy_num_attrs;
    } else if(obj_id == RSDB_REPLIST_OBJ_ID) {
	num_attrs_p = &rgy_info.replist_num_attrs;
    } else if(obj_id == RSDB_ATTR_SCHEMA_OBJ_ID) {
	num_attrs_p = &rgy_info.attr_schema_num_attrs;
    }
    *num_attrs_p += num;
    if(*num_attrs_p < 0) {
        dce_svc_printf(SEC_ATTR_NUM_ATTR_LTZERO_MSG);
    }
}

/* r s d b _ p o l i c y _ g e t _ n u m _ a t t r s 
 *
 * Return rgy_info.*_num_attrs for the object
 * identified by obj_id.
 */
PUBLIC void rsdb_policy_get_num_attrs
(
    rsdb_pvt_id_t	obj_id,		/* [in] */
    signed32		*num		/* [out] */
)
{
    signed32		*num_attrs_p;

    if(obj_id == RSDB_POLICY_OBJ_ID) {
	num_attrs_p = &rgy_info.policy_num_attrs;
    } else if(obj_id == RSDB_REPLIST_OBJ_ID) {
	num_attrs_p = &rgy_info.replist_num_attrs;
    } else if(obj_id == RSDB_ATTR_SCHEMA_OBJ_ID) {
	num_attrs_p = &rgy_info.attr_schema_num_attrs;
    }
    *num = *num_attrs_p;
}

/* r s d b _ p o l i c y _ s e t _ a t t r _ l i s t _ i d
 *
 * Set rgy_info.*_attr_list_id to input attr_list_id
 * for the object identified by obj_id.
 */
PUBLIC void rsdb_policy_set_attr_list_id
(
    rsdb_pvt_id_t	obj_id,		/* [in] */
    rsdb_pvt_id_t	attr_list_id	/* [in] */
)
{
    rsdb_pvt_id_t	*list_id_p;

    if(obj_id == RSDB_POLICY_OBJ_ID) {
	list_id_p = &rgy_info.policy_attr_list_id;
    } else if(obj_id == RSDB_REPLIST_OBJ_ID) {
	list_id_p = &rgy_info.replist_attr_list_id;
    } else if(obj_id == RSDB_ATTR_SCHEMA_OBJ_ID) {
	list_id_p = &rgy_info.attr_schema_attr_list_id;
    }
    *list_id_p = attr_list_id;
}

/* r s d b _ p o l i c y _ g e t _ a t t r _ l i s t _ i d
 *
 * Get rgy_info.*_attr_list_id 
 * for the object identified by obj_id and
 * return in attr_list_id.
 */
PUBLIC void rsdb_policy_get_attr_list_id
(
    rsdb_pvt_id_t	obj_id,		/* [in] */
    rsdb_pvt_id_t	*attr_list_id	/* [out] */
)
{
    rsdb_pvt_id_t	*list_id_p;

    if(obj_id == RSDB_POLICY_OBJ_ID) {
	list_id_p = &rgy_info.policy_attr_list_id;
    } else if(obj_id == RSDB_REPLIST_OBJ_ID) {
	list_id_p = &rgy_info.replist_attr_list_id;
    } else if(obj_id == RSDB_ATTR_SCHEMA_OBJ_ID) {
	list_id_p = &rgy_info.attr_schema_attr_list_id;
    }
    *attr_list_id = *list_id_p;
}

/* For each field in input policy, set the
 * equivalent field in the rgy and/or org policies
 * to the input value if the input value is less
 * restrictive than the previously stored value.
 * If input value == rsdb_policy_field_not_set (-1),
 * don't change existing field.
 */
PUBLIC void rsdb_policy_set_less_restrictive
#ifndef __STDC__
    ( org_name, policy, status )
    sec_rgy_name_t  org_name;       /* [in] */
    sec_rgy_plcy_t  *(policy);      /* [in] */
    error_status_t  *(status);      /* [out] */
#else
  (
    sec_rgy_name_t  org_name,       /* [in] */
    sec_rgy_plcy_t  *(policy),      /* [in] */
    error_status_t  *(status)
  )
#endif
{
    sec_rgy_plcy_t  sec_rgy_plcy;
    sec_rgy_plcy_t  org_plcy;

    CLEAR_STATUS(status);

    /* Get the registry policy and set least restrictive values */
    rsdb_policy_get_info( (unsigned_char_p_t)"", &sec_rgy_plcy, status);
    if (BAD_STATUS(status))
        return;
    set_least_restrictive_policy(policy, &sec_rgy_plcy);
    rsdb_policy_set_info((unsigned_char_p_t)"", &sec_rgy_plcy, status);
    if (BAD_STATUS(status))
        return;

    if (RGY_POLICY(org_name))
        /* No org policy to check */
        return;

    /* Get the organization policy and set least restrictive values */
    rsdb_policy_get_info(org_name, &org_plcy, status);
    if (BAD_STATUS(status)) {
        if(STATUS_EQUAL(status, sec_rgy_object_not_found))
            CLEAR_STATUS(status);
        return;
    }
    set_least_restrictive_policy(policy, &org_plcy);
    rsdb_policy_set_info(org_name, &org_plcy, status);
    return;

}


/*STUB rsdb_policy_set_override_info */
/*OVERRIDE */
PUBLIC void rsdb_policy_set_override_info
#ifndef __STDC__
    ( category, p_d, st ) 
    sec_rgy_name_t               category;
    rs_policy_override_flags_t   p_d;
    error_status_t               *st;
#else
  (
    sec_rgy_name_t               category,
    rs_policy_override_flags_t   p_d,
    error_status_t               *st
  )
#endif
{
    SET_STATUS(st, sec_rgy_not_implemented);
    return;
}

/*STUB */
/*OVERRIDE */
PUBLIC void rsdb_policy_get_override_info
#ifndef __STDC__
    ( category, p_d, st ) 
    sec_rgy_name_t              category;
    rs_policy_override_flags_t  *p_d;
    error_status_t              *st;
#else
  (
    sec_rgy_name_t              category,
    rs_policy_override_flags_t  *p_d,
    error_status_t              *st
  )
#endif
{
    SET_STATUS(st, sec_rgy_not_implemented);
    return;
}


PUBLIC void rsdb_sequential_key_get
#ifndef __STDC__
    ( key ) 
    rsdb_pvt_id_t  *key;
#else
  (
    rsdb_pvt_id_t  *key
  )
#endif
{
    *key = rgy_info.sequential_key;
}

void rsdb_sequential_key_store
#ifndef __STDC__
    ( key ) 
    rsdb_pvt_id_t  key;
#else
  (
    rsdb_pvt_id_t  key
  )
#endif
{
    rgy_info.sequential_key = key;
}

rsdb_pvt_id_t  rsdb_new_sequential_id (
#ifdef __STDC__
    void
#endif
)
{
    ++rgy_info.sequential_key;

    return rgy_info.sequential_key;
}

rsdb_pvt_id_t  rsdb_new_primary_key
#ifndef __STDC__
    ( db, data, data_len ) 
    db_handle   db;
    Pointer     data;
    int         data_len;
#else
  (
    db_handle   db,
    Pointer     data,
    int         data_len
  )
#endif
{
    rsdb_sequential_key_t  sequential_key;
    error_status_t          st;

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = rsdb_new_sequential_id();

    rsdb_store(db, (Pointer) &sequential_key, sizeof(sequential_key), data,
	       data_len, &st);

    return sequential_key.id;
}

void rsdb_purge_sequential_key
#ifndef __STDC__
    ( db, id ) 
    db_handle       db;
    rsdb_pvt_id_t  id;
#else
  (
    db_handle       db,
    rsdb_pvt_id_t  id
  )
#endif
{
    rsdb_sequential_key_t  sequential_key;
    error_status_t          st;

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = id;

    rsdb_delete(db, (Pointer) &sequential_key, sizeof(sequential_key), &st);
}


void rsdb_auth_policy_get_effective
#ifndef __STDC__
    ( account, auth_policy, status ) 
    sec_rgy_login_name_t  *account;        /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy);  /* [out] */
    error_status_t        *(status) ;      /* [out] */
#else
  (
    sec_rgy_login_name_t  *account,        /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy),  /* [out] */
    error_status_t        *(status) 
  )
#endif
{
    sec_rgy_login_name_t    wild_login_name;
    sec_rgy_plcy_auth_t     rgy_auth_plcy;
    sec_rgy_plcy_auth_t     acct_auth_plcy;
    error_status_t          st;

    CLEAR_STATUS(status);   
    CLEAR_LOGIN_NAME(&wild_login_name);
                                              
    rsdb_auth_policy_get_info(&wild_login_name, &rgy_auth_plcy, status);
    if (BAD_STATUS(status))
        return;

    rsdb_auth_policy_get_info(account, &acct_auth_plcy, &st);
    if (BAD_STATUS(&st)) {
        *auth_policy = rgy_auth_plcy;
    } else {
        /* Calculate the most restrictive auth_policy. */
        auth_policy->max_ticket_lifetime = 
            most_restrictive_policy(rgy_auth_plcy.max_ticket_lifetime,
                                    acct_auth_plcy.max_ticket_lifetime);
        auth_policy->max_renewable_lifetime = 
            most_restrictive_policy(rgy_auth_plcy.max_renewable_lifetime,
                                    acct_auth_plcy.max_renewable_lifetime);
    }
}

void rsdb_auth_policy_get_info
#ifndef __STDC__
    ( account, auth_policy, status ) 
    sec_rgy_login_name_t  *account;        /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy);  /* [out] */
    error_status_t        *(status) ;      /* [out] */
#else
  (
    sec_rgy_login_name_t  *account,        /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy),  /* [out] */
    error_status_t        *(status) 
  )
#endif
{
    CLEAR_STATUS(status);

    if(RGY_AUTH_POLICY(account)) {
        *auth_policy = rgy_info.auth_policy;
    } else {
        rsdb_acct_auth_policy_get_info(account, auth_policy, status);
    }
}

void rsdb_auth_policy_set_info
#ifndef __STDC__
    ( account, auth_policy, status ) 
    sec_rgy_login_name_t  *account;        /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy);  /* [in] */
    error_status_t        *(status);       /* [out] */
#else
  (
    sec_rgy_login_name_t  *account,        /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy),  /* [in] */
    error_status_t        *(status) 
  )
#endif
{
     CLEAR_STATUS(status);

    if(RGY_AUTH_POLICY(account)) {
        rgy_info.auth_policy = *auth_policy;
    } else {
        rsdb_acct_auth_policy_set_info(account, auth_policy, status);
    }
}



PUBLIC void rsdb_store_enc_master_key
#ifndef __STDC__
    ( encrypted_mkey, st )
    rsdb_encrypt_key_t  *encrypted_mkey;    /* [in] */
    error_status_t      *st;                /* [out] */
#else
   (
    rsdb_encrypt_key_t  *encrypted_mkey,
    error_status_t      *st
   )
#endif
{
    CLEAR_STATUS(st);
    rgy_info.enc_master_key.mkey_version = encrypted_mkey->mkey_version;
    rgy_info.enc_master_key.keytype = encrypted_mkey->keytype;
    rgy_info.enc_master_key.length = encrypted_mkey->length;
    bcopy((char *)(encrypted_mkey->contents), 
          (char *)(rgy_info.enc_master_key.contents), 
          encrypted_mkey->length);
}    
 
PUBLIC void rsdb_get_enc_master_key
#ifndef __STDC__
    ( encrypted_mkey, st )
    rsdb_encrypt_key_t  *encrypted_mkey;    /* [out] */
    error_status_t      *st;                /* [out] */
#else
   (
    rsdb_encrypt_key_t  *encrypted_mkey,
    error_status_t      *st
   )
#endif
{
    CLEAR_STATUS(st);
    encrypted_mkey->mkey_version = rgy_info.enc_master_key.mkey_version;
    encrypted_mkey->keytype = rgy_info.enc_master_key.keytype;
    encrypted_mkey->length = rgy_info.enc_master_key.length;
    bcopy((char *)(rgy_info.enc_master_key.contents), 
          (char *)(encrypted_mkey->contents), 
          encrypted_mkey->length);
}    

PUBLIC void rsdb_get_master_key_version
#ifndef __STDC__
    ( mkey_version, st )
    krb5_kvno       *mkey_version;      /* [out] */
    error_status_t  *st;                /* [out] */
#else
   (
    krb5_kvno       *mkey_version,
    error_status_t  *st
   )
#endif
{
    CLEAR_STATUS(st);
    *mkey_version = rgy_info.enc_master_key.mkey_version;
}


PUBLIC void rsdb_store_new_enc_master_key
#ifndef __STDC__
    ( encrypted_mkey, st )
    rsdb_encrypt_key_t  *encrypted_mkey;    /* [in] */
    error_status_t      *st;                /* [out] */
#else
   (
    rsdb_encrypt_key_t  *encrypted_mkey,
    error_status_t      *st
   )
#endif
{
    CLEAR_STATUS(st);
    /* Copy new master key into rgy_info master key. */
    rgy_info.enc_master_key.mkey_version = encrypted_mkey->mkey_version;
    rgy_info.enc_master_key.keytype = encrypted_mkey->keytype;
    rgy_info.enc_master_key.length = encrypted_mkey->length;
    bcopy((char *)(encrypted_mkey->contents), 
          (char *)(rgy_info.enc_master_key.contents), 
          encrypted_mkey->length);
}    
 

PUBLIC void rsdb_get_new_master_key_version
#ifndef __STDC__
    ( new_mkey_version, st )
    krb5_kvno       *new_mkey_version;  /* [out] */
    error_status_t  *st;                /* [out] */
#else
   (
    krb5_kvno       *new_mkey_version,
    error_status_t  *st
   )
#endif
{
    CLEAR_STATUS(st);
    if(rgy_info.enc_master_key.mkey_version == rsdb_auth_key_max_version_number) {
        *new_mkey_version = 1;
    } else {
        *new_mkey_version = rgy_info.enc_master_key.mkey_version + 1;
    }
}

/* r s d b _ n e w _ l o g i n _ i d
 * 
 * generate new login ids
 * for tracking records in the login activity database
 */

rsdb_pvt_id_t  rsdb_new_login_id (
#ifdef __STDC__
    void
#endif
)
{
    ++rgy_info.login_activity_id;

    return rgy_info.login_activity_id;
}

    
