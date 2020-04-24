/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1995, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_sw_version.c,v $
 * Revision 1.1.6.1  1996/08/09  12:09:20  arvind
 * 	DCE1.2.2
 * 	[1996/08/05  16:15 UTC  cuti  /main/cuti_pk_migration/2]
 *
 * 	Make number of pk schema mnemonic name
 * 	[1996/08/05  13:34 UTC  cuti  /main/cuti_pk_migration/1]
 *
 * 	Merge 1.2.2's version change with 1.1's
 * 	[1996/02/18  00:18:15  marty  1.1.4.3]
 *
 * Revision 1.1.4.3  1996/02/18  00:18:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:02  marty]
 * 
 * Revision 1.1.4.2  1995/12/08  17:52:24  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:35 UTC  greg
 * 	security migration merge
 * 	[1995/12/08  17:20:40  root]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1995
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Registry Server - software version Log operations
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_log_sw_version.c,v 1.1.6.1 1996/08/09 12:09:20 arvind Exp $";
#endif

#include <dce/sec_daclmgr.h>
#include <rs_log.h>
#include <rs_prop_acl.h>
#include <rs_server.h>
#include <rs_log_acl.h>
#include <rrs_prop_acl.h> 
#include <rsdb_pvt.h>
#include <rsdb_attr_pvt.h>
#include <rsdb_acl.h>
#include <rs_log_util.h>
#include <rs_log_sw_version.h>

#include <bstring.h>
#include <un_strng.h>

#define DCE_122_SW_PK_SCHEMA_NUM 3
#define ALIGN_SIZE 32

/*
 * Local types
 */
typedef struct rs_log_sw_version_t {
    rs_log_hdr_t            upd_hdr;
    unsigned32              old_sw_version;
    unsigned32              new_sw_version;
    /* currently, sec_acl_mgr is not used for anything real */
    sec_acl_mgr_handle_t    acl_mgr_handle;
    unsigned32              component_name_len;
    uuid_t                  manager_type;
    sec_acl_type_t          sec_acl_type;
    rs_log_acl_item_t       item;
} rs_log_sw_version_t;

typedef struct rs_log_pk_schema_t {
    unsigned32                  schema_entry_len;
    rsdb_attr_schema_entry_t	schema_entry;
} rs_log_pk_schema_t;

typedef struct rs_log_pk_sch_add_t {
    rs_log_hdr_t	    upd_hdr;
    unsigned32              old_sw_version;
    unsigned32              new_sw_version;
    rs_log_pk_schema_t      schema[DCE_122_SW_PK_SCHEMA_NUM];
} rs_log_pk_sch_add_t;

/*
 * PRIVATE local routines
 */
PRIVATE PROP_FUNCTION(rs_log_sw_version_prop_set);
PRIVATE REPLAY_FUNCTION(rs_log_sw_version_replay_set);
PRIVATE REPLAY_FUNCTION(rs_log_122_sw_version_replay_set);

/*
 * END OF PRIVATE ROUTINES
 */


/*
 * Define Module opcodes and EPV
 */
#define rs_log_sw_version_set_op     0
#define rs_log_122_sw_version_set_op 1
#define rs_log_sw_version_last       2 /* Must be equal to the number of operations */

static rs_log_ops_t rs_log_sw_version_epv[rs_log_sw_version_last] = {
    { GLOBAL_UPDATE, rs_log_sw_version_prop_set, rs_log_sw_version_replay_set, "setversion" },
    { LOCAL_UPDATE, rs_log_prop_nop, rs_log_122_sw_version_replay_set, "add PK schema" }
};

void rs_log_sw_version_init (
    error_status_t  *st
)
{
    int             num_ops;

    num_ops = rs_log_sw_version_last;
    rs_log_init_module(RS_LOG_MODULE_SW_VERS, num_ops, rs_log_sw_version_epv, st);
}


void rs_log_sw_version_set (
    unsigned32                  old_sw_version,
    unsigned32                  new_sw_version,
    sec_acl_mgr_handle_t        acl_mgr_handle,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type,
    sec_acl_type_t              sec_acl_type,
    sec_acl_list_t              *sec_acl_list,
    rs_replica_master_info_t    *master_info
)
{
    long                    size, acl_len;
    rs_log_sw_version_t     *info;
    error_status_t          status;
    sec_acl_t               *sec_acl;
    unsigned32              component_name_len;
    unsigned_char_t         *p;

    size = 0;
    sec_acl = sec_acl_list->sec_acls[0];
    p = (unsigned_char_t *)component_name;
    component_name_len = u_strlen(p);

    size = sizeof(rs_log_sw_version_t) - sizeof(rs_log_acl_item_t)
           + component_name_len;

    /* Allocate info sized for the sec_acl to be stored in it */
     rs_log_util_alloc_acl_item_log_rec(sec_acl, &size, &acl_len, 
					(void **) &info, &status);
    if (BAD_STATUS(&status)) {
        return;
    }
    info->item.acl_len = acl_len;

    rs_log_util_import_acl_item(sec_acl_list, &(info->item), &status);
    if (BAD_STATUS(&status)) {
        free(info);
        return;
    }
    info->new_sw_version  = new_sw_version;
    info->acl_mgr_handle  = acl_mgr_handle;
    info->component_name_len = component_name_len;
    info->manager_type = *manager_type;
    info->sec_acl_type = sec_acl_type;
    /* copy component_name into buffer at end of item */
    p = (unsigned_char_t *)info;
    p += sizeof(rs_log_sw_version_t) - sizeof(rs_log_acl_item_t) +
         info->item.acl_len;
    u_strncpy(p, component_name, component_name_len);

    rs_log_setup_log_header(&(info->upd_hdr), size, RS_LOG_MODULE_SW_VERS,
                rs_log_sw_version_set_op);

    if (master_info != NULL) {
        rs_log_write(&(info->upd_hdr),
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&(info->upd_hdr), NULL, NULL);
    }
    free(info);
}

/*
 * As of DCE 1.1 the only supported software versions are
 * 1.0.2 and 1.1.  Converting from 1.0.2 to 1.1 requires
 * fixing up the xattrschema acl and propagating that
 * fixed acl to all replicas.  The new sw_version itself is not
 * propagated; software version propagations occur out-of-band
 * using the stop_til_compat_sw() interface;
 */
static void rs_log_sw_version_prop_set (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
)
{
    rs_log_sw_version_t         *log_info = (rs_log_sw_version_t *) info;
    rs_prop_acl_data_t          acl[1];
    unsigned32                  num_acls = 1;
    sec_acl_list_t              *sec_acl_list;
    unsigned_char_t             *p;
    sec_rgy_name_t              component_name;

    CLEAR_STATUS(st);

    /* allocate sec_acl_list and copy acl info into it */
    rs_log_util_export_acl_item(&log_info->item, &sec_acl_list, st);
    if (BAD_STATUS(st)) {
        return;
    }

    p = (unsigned_char_t *)log_info;
    p += sizeof(rs_log_sw_version_t) - sizeof(rs_log_acl_item_t) +
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

static void rs_log_sw_version_replay_set (
    rs_log_rec_t        *info,
    error_status_t      *st
)
{
    rs_log_sw_version_t         *log_info = (rs_log_sw_version_t *) info;
    sec_acl_list_t              *sec_acl_list;
    unsigned_char_t             *p;
    sec_id_t                    user_obj, group_obj, *user_p, *group_p;
    rsdb_acl_ids_t              key_ids;
    sec_rgy_name_t              component_name;
    sec_acl_mgr_handle_t        acl_mgr_handle;

    CLEAR_STATUS(st);
    /* allocate sec_acl_list and copy acl info into it */
    rs_log_util_export_acl_item(&log_info->item, &sec_acl_list, st);
    if (BAD_STATUS(st)) {
        return;
    }

    /* 1.0.2 --> 1.1 conversion
     * Need to reconfigure the acl manager for the new software
     * version, or the sec_acl_manager_replace() call will 
     * fail with "sec_acl_unknown_manager_type" 'cause the
     * xattrschema acl mgr type isn't known at 1.0.2.
     */
    sec_acl_mgr_configure(sec_acl_mgr_config_stable, log_info->new_sw_version,
                          (unsigned_char_p_t)NULL, &acl_mgr_handle, st);
    if (BAD_STATUS(st)) {
        return;
    }

    /* Convert name to key_ids (sec_acl_key) 
     */
    p = (unsigned_char_t *)log_info;
    p += sizeof(rs_log_sw_version_t) - sizeof(rs_log_acl_item_t) +
         log_info->item.acl_len;
    u_strncpy(component_name, p, log_info->component_name_len);
    component_name[log_info->component_name_len] = '\0';
    user_p = &user_obj; group_p = &group_obj;
    rsdb_acl_get_ids_from_name((sec_acl_component_name_t)component_name, 
                               log_info->sec_acl_type,
                               &(log_info->manager_type),
                               &user_p, &group_p,
                               &key_ids, st);
    if (BAD_STATUS(st)) {
        return;
    }
    sec_acl_mgr_replace(log_info->acl_mgr_handle,
                        (sec_acl_key_t)&key_ids,
                        &(log_info->manager_type), log_info->sec_acl_type,
                        sec_acl_list, st);

    free(sec_acl_list->sec_acls[0]);
    free((char *)sec_acl_list);

    if (GOOD_STATUS(st)) {
	rs_master_info_set_sw_rev(log_info->new_sw_version, st);
    }
}



PUBLIC void rs_log_122_sw_version_set(
    unsigned32                  old_sw_version,
    unsigned32                  new_sw_version,
    rs_replica_master_info_t    *master_info)

{
    rs_log_pk_sch_add_t *info;
    int                 i, j; 
    unsigned32          total_size = 0, size = 0;
    unsigned32          sch_entry_len[3];
    unsigned32          real_sch_entry_len[3];
    sec_attr_schema_entry_t	*sch_entry_p[3];
    error_status_t      st;
    unsigned_char_t	*p;
    rsdb_attr_schema_entry_t    schema_entry;

    total_size = sizeof(rs_log_pk_sch_add_t);

    for (i = 0; i < DCE_122_SW_PK_SCHEMA_NUM; i ++) {
      get_1resv_pk_schema(&sch_entry_p[i], i, &st);
      if (BAD_STATUS(&st))
	return;
      rsdb_attr_sch_calc_size(sch_entry_p[i], &real_sch_entry_len[i], &st);
      j = real_sch_entry_len[i] % ALIGN_SIZE;
      /* Add to become 4 words aligned */
      sch_entry_len[i] = real_sch_entry_len[i] + (ALIGN_SIZE - j);  
      total_size += (sch_entry_len[i] - sizeof(rsdb_attr_schema_entry_t));
    }

    if (!(info = malloc(total_size))) {
        SET_STATUS(&st, sec_attr_no_memory);
        return;
    }
    memset(info, 0, total_size);

    info->new_sw_version = new_sw_version;
    info->old_sw_version = old_sw_version;
    
    p = (unsigned_char_t *) info;
    /* advance pointer to pass log header, new_version and old_version */
    p += sizeof(rs_log_hdr_t) + 2 * sizeof(unsigned32);

    for (i = 0; i < DCE_122_SW_PK_SCHEMA_NUM; i++) {
      /* copy schema_entry_len */
      memcpy(p, &sch_entry_len[i], sizeof(unsigned32));
      p += sizeof(unsigned32);   /* advance pointer */
      /* copy schema_entry  */
      rsdb_attr_sch_import_info(sch_entry_p[i], p, &st);
      if (BAD_STATUS(&st)) {
	return;
      }
      p += sch_entry_len[i];     /* advance pointer */
      free(sch_entry_p[i]);
    }

    rs_log_setup_log_header(&(info->upd_hdr), total_size, 
			    RS_LOG_MODULE_SW_VERS, rs_log_122_sw_version_set_op);

     if (master_info != NULL) {
        rs_log_write(&(info->upd_hdr),
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&(info->upd_hdr), NULL, NULL);
    }

    free(info);
}
     

PRIVATE void rs_log_122_sw_version_replay_set(
    rs_log_rec_t               *info,
    error_status_t             *st_p)
{

    rs_log_pk_sch_add_t         *log_info = (rs_log_pk_sch_add_t *)info;
    sec_attr_schema_entry_t 	schema_entry;
    unsigned32                  schema_entry_len;
    unsigned32                  i;
    unsigned_char_t	*p;
    

    CLEAR_STATUS(st_p);
    p = (unsigned_char_t *) log_info->schema;
    /* advance pointer to pass log header, new_version and old_version /
    p += sizeof(rs_log_hdr_t) + 2 * sizeof(unsigned32); */

    for (i = 0; i < DCE_122_SW_PK_SCHEMA_NUM; i++ ) {
      memcpy(&schema_entry_len, p, sizeof(unsigned32));
      p += sizeof(unsigned32); /* advance pointer */
      rsdb_attr_sch_export(p, rpc_ss_allocate, &schema_entry, st_p);

      if (BAD_STATUS(st_p)) {
        return;
      }

      rsdb_attr_sch_add(NULL, &schema_entry, st_p);
      if (BAD_STATUS(st_p)) {
        return;
      }
      p += schema_entry_len;  /* rs_log_pk_schema_t */

    }

    rs_master_info_set_sw_rev(log_info->new_sw_version, st_p);

}



