/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_acl.c,v $
 * Revision 1.1.9.3  1996/02/18  00:17:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:49  marty]
 *
 * Revision 1.1.9.2  1995/12/08  17:51:35  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:06 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:57 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Merge work from greg_sec_migrate_acls branch.
 * 	[1995/04/18  18:19 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Include <dce/sec_daclmgr.h>.
 * 	[1995/04/13  17:10 UTC  greg  /main/SEC_migrate/greg_sec_migrate_acls/1]
 * 
 * 	Move some acl logging routines from rs_log_acl.c into
 * 	rs_log_util.c since they are now common to both the
 * 	acl and software version logging modules.
 * 	[1995/04/03  18:40 UTC  greg  /main/SEC_migrate/1]
 * 
 * Revision 1.1.3.6  1993/02/19  17:13:18  ahop
 * 	dbcp: change log_acl_item_t to be an rsdb_acl_fixed_t
 * 	[1993/02/19  08:42:30  ahop]
 * 
 * Revision 1.1.3.5  1993/01/26  21:40:03  ahop
 * 	bl6d: implement rs_log_acl_prop_replace
 * 	[1993/01/26  21:34:04  ahop]
 * 
 * Revision 1.1.3.4  1992/12/29  16:35:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:52  zeliff]
 * 
 * Revision 1.1.3.3  1992/11/04  19:36:17  ahop
 * 	Third replication drop: bl6a
 * 	[1992/11/03  18:36:17  ahop]
 * 
 * Revision 1.1.3.2  1992/10/07  20:33:16  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:16:45  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  16:59:43  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1  1992/01/19  14:44:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Registry Server - ACL Log operations
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_log_acl.c,v 1.1.9.3 1996/02/18 00:17:56 marty Exp $";
#endif

#include <dce/sec_daclmgr.h>
#include <rs_log.h>
#include <rs_prop_acl.h>
#include <rs_server.h>
#include <rs_log_acl.h>
#include <rrs_prop_acl.h> 
#include <rsdb_pvt.h>
#include <rsdb_acl.h>
#include <rs_log_util.h>

#include <bstring.h>
#include <un_strng.h>


/*
 * Local types
 */

typedef struct rs_log_acl_replace_t {
    rs_log_hdr_t            upd_hdr;
    /* currently, sec_acl_mgr is not used for anything */
    sec_acl_mgr_handle_t    sec_acl_mgr;
    unsigned32              component_name_len;
    uuid_t                  manager_type;
    sec_acl_type_t          sec_acl_type;
    rs_log_acl_item_t       item;
} rs_log_acl_replace_t;


/*
 * PRIVATE local routines
 */
PRIVATE PROP_FUNCTION(rs_log_acl_prop_replace);
PRIVATE REPLAY_FUNCTION(rs_log_acl_replay_replace);

/*
 * END OF PRIVATE ROUTINES
 */


/*
 * Define Module opcodes and EPV
 */
#define rs_log_acl_replace_op   0
#define rs_log_acl_last         1   /* Must be equal to the number of operations */

static rs_log_ops_t rs_log_acl_epv[rs_log_acl_last] = {
    { GLOBAL_UPDATE, rs_log_acl_prop_replace,   rs_log_acl_replay_replace, "replace" } ,
};


PUBLIC void rs_log_acl_init
#ifndef __STDC__
    ( st )
    error_status_t  *st;
#else
  (
    error_status_t  *st
  )
#endif
{
    int             num_ops;

    num_ops = rs_log_acl_last;
    rs_log_init_module(RS_LOG_MODULE_ACL, num_ops, rs_log_acl_epv, st);
}


PUBLIC void rs_log_acl_replace
#ifndef __STDC__
    ( sec_acl_mgr, component_name, manager_type,
      sec_acl_type, sec_acl_list, master_info )
    sec_acl_mgr_handle_t        sec_acl_mgr;        /* [in] */
    sec_acl_component_name_t    component_name;     /* [in] */
    uuid_t                      *manager_type;      /* [in] */
    sec_acl_type_t              sec_acl_type;       /* [in] */
    sec_acl_list_t              *sec_acl_list;      /* [in] */
    rs_replica_master_info_t    *master_info;       /* [in] */
#else
  (
    sec_acl_mgr_handle_t        sec_acl_mgr,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type,
    sec_acl_type_t              sec_acl_type,
    sec_acl_list_t              *sec_acl_list,
    rs_replica_master_info_t    *master_info
  )
#endif
{
    long                    size, acl_len;
    rs_log_acl_replace_t    *info;
    error_status_t          status;
    sec_acl_t               *sec_acl;
    unsigned32              component_name_len;
    unsigned_char_t         *p;

    size = 0;
    sec_acl = sec_acl_list->sec_acls[0];
    p = (unsigned_char_t *)component_name;
    component_name_len = u_strlen(p);

    size = sizeof(rs_log_acl_replace_t) - sizeof(rs_log_acl_item_t)
           + component_name_len;

    /* Allocate info sized for the sec_acl to be stored in it */
     rs_log_util_alloc_acl_item_log_rec(sec_acl, &size, &acl_len, 
					(void **) &info, &status);
    if(BAD_STATUS(&status)) {
        return;
    }
    info->item.acl_len = acl_len;

    rs_log_util_import_acl_item(sec_acl_list, &(info->item), &status);
    if(BAD_STATUS(&status)) {
        free(info);
        return;
    }
    info->sec_acl_mgr  = sec_acl_mgr;
    info->component_name_len = component_name_len;
    info->manager_type = *manager_type;
    info->sec_acl_type = sec_acl_type;
    /* copy component_name into buffer at end of item */
    p = (unsigned_char_t *)info;
    p += sizeof(rs_log_acl_replace_t) - sizeof(rs_log_acl_item_t) +
         info->item.acl_len;
    u_strncpy(p, component_name, component_name_len);

    rs_log_setup_log_header(&(info->upd_hdr), size, RS_LOG_MODULE_ACL,
                rs_log_acl_replace_op);

    if (master_info != NULL) {
        rs_log_write(&(info->upd_hdr),
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&(info->upd_hdr), NULL, NULL);
    }
    free(info);
}

PRIVATE void rs_log_acl_prop_replace
#ifndef __STDC__
    ( handle, info, master_info, propq_only, st )
    handle_t                    handle;
    rs_log_rec_t                *info;
    rs_replica_master_info_t    *master_info;
    boolean32                   propq_only;
    error_status_t              *st;
#else
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
#endif
{
    rs_log_acl_replace_t        *log_info = (rs_log_acl_replace_t *) info;
    rs_prop_acl_data_t          acl[1];
    unsigned32                  num_acls = 1;
    sec_acl_list_t              *sec_acl_list;
    unsigned_char_t             *p;
    sec_rgy_name_t              component_name;

    CLEAR_STATUS(st);

    /* allocate sec_acl_list and copy acl info into it */
    rs_log_util_export_acl_item(&log_info->item, &sec_acl_list, st);
    if(BAD_STATUS(st)) {
        return;
    }

    p = (unsigned_char_t *)log_info;
    p += sizeof(rs_log_acl_replace_t) - sizeof(rs_log_acl_item_t) +
         log_info->item.acl_len;
    u_strncpy(component_name, p, log_info->component_name_len);
    component_name[log_info->component_name_len] = '\0';

    acl[0].component_name = (sec_acl_component_name_t)component_name;
    acl[0].manager_type = log_info->manager_type;
    acl[0].acl_type = log_info->sec_acl_type;
    acl[0].acl_list = sec_acl_list;

    rrs_prop_acl_replace(handle, num_acls, acl, master_info, propq_only, st);

    free(sec_acl_list->sec_acls[0]);
    free((char *)sec_acl_list);
    return;
}

PRIVATE void rs_log_acl_replay_replace
#ifndef __STDC__
    ( info, st )
    rs_log_rec_t        *info;
    error_status_t      *st;
#else
  (
    rs_log_rec_t        *info,
    error_status_t      *st
  )
#endif
{
    rs_log_acl_replace_t        *log_info = (rs_log_acl_replace_t *) info;
    sec_acl_list_t              *sec_acl_list;
    unsigned_char_t             *p;
    sec_id_t                    user_obj, group_obj, *user_p, *group_p;
    rsdb_acl_ids_t              key_ids;
    sec_rgy_name_t              component_name;


    CLEAR_STATUS(st);
    /* allocate sec_acl_list and copy acl info into it */
    rs_log_util_export_acl_item(&log_info->item, &sec_acl_list, st);
    if(BAD_STATUS(st)) {
        return;
    }
    /* Convert name to key_ids (sec_acl_key) 
     */
    p = (unsigned_char_t *)log_info;
    p += sizeof(rs_log_acl_replace_t) - sizeof(rs_log_acl_item_t) +
         log_info->item.acl_len;
    u_strncpy(component_name, p, log_info->component_name_len);
    component_name[log_info->component_name_len] = '\0';
    user_p = &user_obj; group_p = &group_obj;
    rsdb_acl_get_ids_from_name((sec_acl_component_name_t)component_name, 
                               log_info->sec_acl_type,
                               &(log_info->manager_type),
                               &user_p, &group_p,
                               &key_ids, st);
    if(BAD_STATUS(st)) {
        return;
    }
    sec_acl_mgr_replace(log_info->sec_acl_mgr,
                        (sec_acl_key_t)&key_ids,
                        &(log_info->manager_type), log_info->sec_acl_type,
                        sec_acl_list, st);

    free(sec_acl_list->sec_acls[0]);
    free((char *)sec_acl_list);
    return;
}



