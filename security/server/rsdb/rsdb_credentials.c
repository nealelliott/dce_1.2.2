/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_credentials.c,v $
 * Revision 1.1.8.1  1996/05/10  13:24:36  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/27  17:48 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/hanfei_dce1.2_b1/2  1996/02/29  16:36 UTC  hanfei
 * 	change foreign credential field name uid to uuid.
 * 	change foreign credential index mechanism so only one copy of
 * 	credential data is stored.
 *
 * 	HP revision /main/hanfei_dce1.2_b1/1  1996/02/02  15:35 UTC  hanfei
 * 	multi-cell group work: add functions to handle foreign principal credential blocks.
 * 	[1996/05/09  20:55:41  arvind]
 *
 * Revision 1.1.4.1  1994/08/04  16:14:41  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:13:38  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  16:38:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:44:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsdb_credentials.c V=8 05/22/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * 
 *      Registry Server - database interlude - pgo credentials management
 *  
 *   Implementation Notes:   
 *
 *   The credentials block structure consists of three members:
 *       UID - uuid of its PGO item, also stored in the rgy and rsdb_pgo_item,
 *       Unix ID - Unix ID, also stored in the rgy_pgo_item, 
 *       ID - rsdb_pvt_id of its PGO, stored in rsdb_pgo_item.
 *
 *   Because credentials information needs to be retrieved when only the UID
 *   is known OR when only the Unix ID is known, each credentials block (CB)
 *   is stored in the database twice, once for each key:
 *
 *       Key                 Data
 *       ---                 ----
 *       UID                 Credentials Block
 *       Unix ID             Credentials Block  
 *
 *   As the code in this file demonstrates, when a credentials block is
 *   created (modified), two records are inserted (replaced), one for
 *   each of the two keys.  When a CB is retrieved, only one database
 *   fetch is required using whichever key was provided.
 *
 *   Following is the rationale for not implementing an alternative 
 *   design which, at first blush, seemed reasonable:
 *
 *   It seems foolish to store the same credentials block twice.
 *   Redundant storage wastes space and increases the likelihood 
 *   that the two CBs will get out of synch due to separate modifications.
 *   By creating a new ID to identify the credentials block, we could
 *   store a single copy of the CB with the following key-to-data mapping.
 *                 
 *       Key                 Data
 *       ---                 ----
 *       UID                 Credentials Block ID
 *       Unix ID             Credentials Block ID
 *       Cred Block ID       Credentials Block            
 * 
 *  or, even better:
 *
 *       Key                 Data
 *       ---                 ----
 *       UID                 Unix ID
 *       Unix ID             Credentials Block
 *
 *
 *   However, with either of these schemes, when a lookup occurs, often
 *   two records must be accessed (e.g., use UID to retrieve Unix ID, 
 *   then use Unix ID to retrieve CB).  Furthermore, since the credentials 
 *   block contains only a uuid_t, and three longs,
 *   it is not such a huge consumer of memory that we should be concerned 
 *   with inefficient use of space (caused by storing it twice).
 *
 *   In the current implementation the two credentials block records are
 *   always updated together in the same operation.  If the two records
 *   were updated separately in distinct places in the code, there would
 *   be more cause to worry that the two copies would get out of synch.
 *
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rsdb_credentials.c	8 - 05/22/91";
#endif

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/uuid.h>

#include <rsdb_acl.h>
#include <rsdb_pvt.h>
#include <rs_base.h>
#include <rs_util.h>
#include <bstring.h>
#include <sec_svc.h>

#include <stdio.h>

#ifndef NULL
#   define NULL 0
#endif

PRIVATE boolean32 get_credentials
#ifndef __STDC__
    ( db, key, key_len, credentials ) 
    db_handle           db;
    Pointer             key;
    int                 key_len;
    rsdb_credentials_t *credentials;
#else
  (
    db_handle           db,
    Pointer             key,
    int                 key_len,
    rsdb_credentials_t *credentials
  )
#endif
{
    char                *data;
    int                 data_len;
    error_status_t      st;

    rsdb_fetch(db, key, key_len, &data, &data_len, &st);
    if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
        return false;
    } else {
        if (data_len != sizeof(*credentials)) {
            dce_svc_printf(SEC_RSDB_INCONSISTENT_CREDS_MSG);
        } else {
            bcopy(data, (Pointer) credentials, data_len);
        }
    }
    return true;
}

PUBLIC void rsdb_change_credentials
#ifndef __STDC__
    ( db, old_cr, uid, unix_id, id, acl_id ) 
    db_handle           db;
    rsdb_credentials_t  *old_cr;
    uuid_t              *uid;
    int                 unix_id;
    rsdb_pvt_id_t       id;
    rsdb_pvt_id_t       acl_id;
#else
  (
    db_handle           db,
    rsdb_credentials_t  *old_cr,
    uuid_t              *uid,
    int                 unix_id,
    rsdb_pvt_id_t       id,
    rsdb_pvt_id_t       acl_id
  )
#endif
{
    rsdb_credentials_t new_cr;
    rsdb_uid_key_t     uid_key;
    rsdb_unix_id_key_t unix_id_key;
    error_status_t     st;

    bzero( (Pointer) &uid_key, sizeof(uid_key));
    bzero( (Pointer) &unix_id_key, sizeof(unix_id_key));

    uid_key.record_type = UUID_TYPE;
    uid_key.uid         = *uid;

    new_cr.uid = *uid;
    new_cr.id = id;
    new_cr.unix_id = unix_id;
    new_cr.acl_id = acl_id;

    rsdb_replace(db, (Pointer) &uid_key, sizeof(uid_key), (Pointer) &new_cr,
		 sizeof(new_cr), &st);

    unix_id_key.record_type = UNIX_ID_TYPE;
    if ((old_cr != NULL) && old_cr->unix_id != unix_id) {
        unix_id_key.unix_id = old_cr->unix_id;
        rsdb_delete(db, (Pointer) &unix_id_key, sizeof(unix_id_key), &st);
    }
    unix_id_key.unix_id = unix_id;
    rsdb_replace(db, (Pointer) &unix_id_key, sizeof(unix_id_key),
		 (Pointer) &new_cr, sizeof(new_cr), &st);
}

PUBLIC void rsdb_purge_credentials
#ifndef __STDC__
    ( db, credentials ) 
    db_handle           db;
    rsdb_credentials_t *credentials;
#else
  (
    db_handle           db,
    rsdb_credentials_t *credentials
  )
#endif
{
    rsdb_uid_key_t     uid_key;
    rsdb_unix_id_key_t unix_id_key;
   error_status_t      st;

    bzero( (Pointer) &uid_key, sizeof(uid_key));
    bzero( (Pointer) &unix_id_key, sizeof(unix_id_key));

    uid_key.record_type = UUID_TYPE;
    uid_key.uid         = credentials->uid;
    rsdb_delete(db, (Pointer) &uid_key, sizeof(uid_key), &st);

    unix_id_key.record_type = UNIX_ID_TYPE;
    unix_id_key.unix_id     = credentials->unix_id;
    rsdb_delete(db, (Pointer) &unix_id_key, sizeof(unix_id_key), &st);
                                         
    /* Delete the acl associated with this credentials block. */
    rsdb_acl_delete(credentials->acl_id, &st);
}

PUBLIC boolean32 rsdb_get_credentials_by_uid
#ifndef __STDC__
    ( db, uid, credentials ) 
    db_handle           db;
    uuid_t              *uid;
    rsdb_credentials_t *credentials;
#else
  (
    db_handle           db,
    uuid_t              *uid,
    rsdb_credentials_t *credentials
  )
#endif
{
    rsdb_uid_key_t     uid_key;

    bzero( (Pointer) &uid_key, sizeof(uid_key));

    uid_key.record_type = UUID_TYPE;
    uid_key.uid         = *uid;

    return get_credentials(db, (Pointer) &uid_key, sizeof(uid_key), credentials);
}

PUBLIC boolean32 rsdb_get_credentials_by_unix_id
#ifndef __STDC__
    ( db, unix_id, credentials ) 
    db_handle           db;
    int                 unix_id;
    rsdb_credentials_t *credentials;
#else
  (
    db_handle           db,
    int                 unix_id,
    rsdb_credentials_t *credentials
  )
#endif
{
    rsdb_unix_id_key_t unix_id_key;

    bzero( (Pointer) &unix_id_key, sizeof(unix_id_key));

    unix_id_key.record_type = UNIX_ID_TYPE;
    unix_id_key.unix_id     = unix_id;

    return get_credentials(db, (Pointer) &unix_id_key, sizeof(unix_id_key),
			   credentials);
}


/* 
 * Following are credential functions for handling foreign principal retrieving.
 * They use either indexed by cell uuid and principal uuid or indexed
 * by pvt_id.  When creating a froeign principal and when an EPAC request
 * is in, the first key method is used.  When deleting a foreign 
 * principal from this registry ( caused implicitly by its last memebership
 * deleted in this registry ), the second key method is used.
 * We only create one record of the credential data.  UUIDS are the one
 * indexes to the data and pvt_id is indexed to the UUIDS key.
 */

PRIVATE boolean32 get_credentials_foreign
  (
    db_handle           db,
    Pointer             key,
    int                 key_len,
    rsdb_credentials_foreign_t *credentials
  )
{
    char                *data;
    int                 data_len;
    error_status_t      st;

    rsdb_fetch(db, key, key_len, &data, &data_len, &st);
    if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
        return false;
    } else {
        if (data_len != sizeof(*credentials)) {
            dce_svc_printf(SEC_RSDB_INCONSISTENT_CREDS_MSG);
        } else {
            bcopy(data, (Pointer) credentials, data_len);
        }
    }
    return true;
}

PUBLIC void rsdb_change_credentials_by_foreign_uuids
  (
    db_handle           db,
    /* old_cr : for future unix_id change hook */
    rsdb_credentials_foreign_t  *old_cr,
    uuid_t              *cell_uuid,
    uuid_t              *princ_uuid,
    int                 unix_id,
    rsdb_pvt_id_t       id,
    rsdb_pvt_id_t       acl_id
  )
{
    rsdb_credentials_foreign_t       new_cr;
    rsdb_membership_list_key_t	     pvt_id_key;
    rsdb_foreign_uuids_key_t         uuid_key;
    error_status_t     		     st;

    if ( old_cr != NULL_PTR ) {
	/*
	 * Changing credential is not supported yet.
         * This is a hook for future usage, e.g.
	 * unix_id or acl_id changes.
	 */
	return;
    }

    /* adding a new credential */
    bzero( (Pointer) &uuid_key, sizeof(uuid_key));
    bzero( (Pointer) &pvt_id_key, sizeof(pvt_id_key));

    uuid_key.record_type = UUIDS_TYPE;
    uuid_key.cell_uuid	      = *cell_uuid;
    uuid_key.princ_uuid         = *princ_uuid;

    pvt_id_key.record_type = CREDENTIALS_ID_TYPE;
    pvt_id_key.id = id;

    new_cr.cell_uuid = *cell_uuid;
    new_cr.princ_uuid = *princ_uuid;
    new_cr.id = id;
    new_cr.unix_id = unix_id;
    new_cr.acl_id = acl_id;

    rsdb_replace(db, (Pointer) &uuid_key, sizeof(uuid_key), (Pointer) &new_cr,
		 sizeof(new_cr), &st);
    rsdb_replace(db, (Pointer) &pvt_id_key, sizeof(pvt_id_key), (Pointer) 
		&uuid_key, sizeof(uuid_key), &st);

}

PUBLIC void rsdb_purge_credentials_by_pvt_id
  (
    db_handle           db,
    rsdb_pvt_id_t	id
  )
{
    rsdb_credentials_foreign_t       credential_pvt;
    rsdb_membership_list_key_t	     pvt_id_key;
    rsdb_foreign_uuids_key_t         uuid_key;
    char			     *data;
    int				     data_len;
    error_status_t     		     st;

    bzero( (Pointer) &uuid_key, sizeof(uuid_key));
    bzero( (Pointer) &pvt_id_key, sizeof(pvt_id_key));

    pvt_id_key.record_type = CREDENTIALS_ID_TYPE;
    pvt_id_key.id = id;
    rsdb_fetch(db, (Pointer) &pvt_id_key, sizeof(pvt_id_key), 
	&data, &data_len, &st);
    if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
        dce_svc_printf(SEC_RSDB_INCONSISTENT_CREDS_MSG);
        return;
    } else {
        if (data_len != sizeof(uuid_key)) {
            dce_svc_printf(SEC_RSDB_INCONSISTENT_CREDS_MSG);
        } else {
            bcopy(data, (Pointer) &uuid_key, data_len);
        }
    }

    rsdb_delete(db, (Pointer) &uuid_key, sizeof(uuid_key), &st);
    rsdb_delete(db, (Pointer) &pvt_id_key, sizeof(pvt_id_key), &st);
    if ( credential_pvt.acl_id != 0 ) { 
    	rsdb_acl_delete(credential_pvt.acl_id, &st);
    }

}

PUBLIC boolean32 rsdb_get_credentials_by_foreign_uuids
  (
    db_handle           db,
    uuid_t		*cell_uuid,
    uuid_t              *princ_uuid,
    rsdb_credentials_foreign_t *credentials
  )
{
    rsdb_foreign_uuids_key_t    uuid_key;

    bzero( (Pointer) &uuid_key, sizeof(uuid_key));

    uuid_key.record_type = UUIDS_TYPE;
    uuid_key.cell_uuid          = *cell_uuid;
    uuid_key.princ_uuid 	= *princ_uuid;

    return get_credentials_foreign(db, (Pointer) &uuid_key, sizeof(uuid_key), credentials);

}

PUBLIC boolean32 rsdb_get_credentials_uuids_by_pvt_id
  (
    db_handle           db,
    rsdb_pvt_id_t	id,
    uuid_t		*cell_uuid,
    uuid_t              *princ_uuid
  )
{
    rsdb_credentials_foreign_t       credential_pvt;
    rsdb_membership_list_key_t	     pvt_id_key;
    rsdb_foreign_uuids_key_t         uuid_key;
    char			     *data;
    int				     data_len;
    error_status_t		     st;

    bzero( (Pointer) &pvt_id_key, sizeof(pvt_id_key));

    pvt_id_key.record_type = CREDENTIALS_ID_TYPE;
    pvt_id_key.id = id;

    rsdb_fetch(db, (Pointer) &pvt_id_key, sizeof(pvt_id_key), 
	&data, &data_len, &st);

    if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
        dce_svc_printf(SEC_RSDB_INCONSISTENT_CREDS_MSG);
        return;
    } else {
        if (data_len != sizeof(uuid_key)) {
            dce_svc_printf(SEC_RSDB_INCONSISTENT_CREDS_MSG);
	    return false;
        } else {
            bcopy(data, (Pointer) &uuid_key, data_len);
        }
    }

    *cell_uuid = uuid_key.cell_uuid;
    *princ_uuid = uuid_key.princ_uuid;
    
    return true;

}

