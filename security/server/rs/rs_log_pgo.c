/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_pgo.c,v $
 * Revision 1.1.10.3  1996/11/13  18:10:28  arvind
 * 	Fix the replay of deleted group members.
 * 	[1996/11/12  18:48 UTC  jrr  /main/DCE_1.2.2/jrr_122_7/1]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/07/13  21:20 UTC  burati  /main/HPDCE02/1]
 *
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	HP 	[1995/07/13  20:47 UTC  burati  /main/mb_mothra5/2]
 *
 * 	More cleanup
 * 	HP 	[1995/07/05  22:35 UTC  burati  /main/mb_mothra5/1]
 * 	[1996/11/13  17:51:07  arvind]
 *
 * Revision 1.1.10.2  1996/10/03  15:09:25  arvind
 * 	Add rs_log_pgo_add_global_member_op to the epv to support global
 * 	group members. CHFts19707.
 * 	[1996/08/08  14:09 UTC  jrr  /main/DCE_1.2.2/jrr_122_3/1]
 * 
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:46 UTC  hanfei  /main/DCE_1.2/2]
 * 
 * 	add cell_uuid and princ_uuid for log and prop data since
 * 	add_member need this information when creating foreign principal
 * 	locally.
 * 	[1996/02/29  18:59 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 * 
 * 	multi-cell group work: add NULL_PTR as cell_uuid for rs_util_pgo_add() call.
 * 	[1996/02/01  16:26 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 * 
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:57  marty]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/07/13  21:20 UTC  burati  /main/HPDCE02/1]
 * 
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	HP 	[1995/07/13  20:47 UTC  burati  /main/mb_mothra5/2]
 * 
 * 	More cleanup
 * 	HP 	[1995/07/05  22:35 UTC  burati  /main/mb_mothra5/1]
 * 
 * 	Alias ACL fix
 * 	[1995/12/08  17:20:36  root]
 * 
 * Revision 1.1.10.1  1996/05/10  13:22:05  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:46 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/29  18:59 UTC  hanfei
 * 	add cell_uuid and princ_uuid for log and prop data since
 * 	add_member need this information when creating foreign principal
 * 	locally.
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/01  16:26 UTC  hanfei
 * 	multi-cell group work: add NULL_PTR as cell_uuid for rs_util_pgo_add() call.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:16 UTC  psn
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	[1995/07/13  21:20 UTC  burati  /main/HPDCE02/1]
 * 
 * 	More cleanup
 * 	[1995/07/13  20:47 UTC  burati  /main/mb_mothra5/2]
 * 
 * 	Alias ACL fix
 * 	[1995/07/05  22:35 UTC  burati  /main/mb_mothra5/1]
 * 
 * Revision 1.1.3.5  1993/03/19  22:20:45  emartin
 * 	CR 7472  rs_log_pgo_replay_rename - rsdb_acct_store_default_pepper
 * 	[1993/03/19  18:57:04  emartin]
 * 
 * Revision 1.1.3.4  1993/01/26  23:06:01  emartin
 * 	rs_log_pgo_prop_delete:  pass null-terminated pgo name
 * 	[1993/01/20  21:32:37  emartin]
 * 
 * Revision 1.1.3.3  1992/12/29  16:35:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:58  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:33:30  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:16:51  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  17:00:05  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1  1992/01/19  14:43:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1995
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Registry Server - PGO Log operations
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: rs_log_pgo.c,v 1.1.10.3 1996/11/13 18:10:28 arvind Exp $";
#endif

#include <rs_log.h>
#include <rs_prop_pgo.h>

#include <rs_base.h>
#include <rs_lock.h>
#include <rs_log_pgo.h>
#include <rrs_prop_pgo.h>

#include <rsdb_pgo.h>
#include <rsdb_dir.h>
#include <rsdb_util.h>
#include <rsdb_name_util.h>

#include <un_strng.h>

/*
 * Local types
 */
typedef struct rs_log_pgo_item_t {
    uuid_t              uid;
    long                unix_num;
    long                quota;
    sec_rgy_pgo_flags_t flags;
    short               fullname_len;
    short               name_len;
    char                string_buf[2*sizeof(sec_rgy_name_t)];
        /* storage for variable length name and fullname strings */
} rs_log_pgo_item_t;

typedef struct rs_log_pgo_add_t {
    rs_log_hdr_t            upd_hdr;
    sec_rgy_domain_t        domain;
    sec_rgy_foreign_id_t    client_ids;
    rs_log_pgo_item_t       item;
} rs_log_pgo_add_t;

typedef struct rs_log_pgo_delete_t {
    rs_log_hdr_t       upd_hdr;
    sec_rgy_domain_t   domain;
    sec_timeval_sec_t  cache_info;
    short              name_len;
    sec_rgy_name_t     name;
} rs_log_pgo_delete_t;

typedef struct rs_log_pgo_replace_t {
    rs_log_hdr_t       upd_hdr;
    sec_rgy_domain_t   domain;
    sec_timeval_sec_t  cache_info;
    rs_log_pgo_item_t  item;
} rs_log_pgo_replace_t;

typedef struct rs_log_pgo_rename_t {
    rs_log_hdr_t       upd_hdr;
    sec_rgy_domain_t   domain;
    sec_timeval_sec_t  cache_info;
    short              old_name_len;
    short              new_name_len;
    char               string_buf[2*sizeof(sec_rgy_name_t)];
        /* storage for variable length old_name and new_name strings */
} rs_log_pgo_rename_t;


typedef struct rs_log_pgo_member_t {
    rs_log_hdr_t       upd_hdr;
    sec_rgy_domain_t   domain;
    short              go_name_len;
    short              person_name_len;
    char               string_buf[2*sizeof(sec_rgy_name_t)];
        /* storage for variable length group and person strings */
} rs_log_pgo_member_t;

typedef struct rs_log_pgo_global_member_t {
    rs_log_hdr_t       upd_hdr;
    sec_rgy_domain_t   domain;
    short              go_name_len;
    uuid_t	       person_cell_uuid;
    uuid_t	       person_princ_uuid;
    short              person_name_len;
    char               string_buf[2*sizeof(sec_rgy_name_t)];
        /* storage for variable length group and person strings */
} rs_log_pgo_global_member_t;


/*
 * PRIVATE local routines
 */
PRIVATE PROP_FUNCTION(rs_log_pgo_prop_add);
PRIVATE PROP_FUNCTION(rs_log_pgo_prop_delete);
PRIVATE PROP_FUNCTION(rs_log_pgo_prop_replace);
PRIVATE PROP_FUNCTION(rs_log_pgo_prop_rename);
PRIVATE PROP_FUNCTION(rs_log_pgo_prop_add_member);
PRIVATE PROP_FUNCTION(rs_log_pgo_prop_del_member);

PRIVATE REPLAY_FUNCTION(rs_log_pgo_replay_add);
PRIVATE REPLAY_FUNCTION(rs_log_pgo_replay_delete);
PRIVATE REPLAY_FUNCTION(rs_log_pgo_replay_replace);
PRIVATE REPLAY_FUNCTION(rs_log_pgo_replay_rename);
PRIVATE REPLAY_FUNCTION(rs_log_pgo_replay_add_member);
PRIVATE REPLAY_FUNCTION(rs_log_pgo_replay_add_global_member);
PRIVATE REPLAY_FUNCTION(rs_log_pgo_replay_del_member);
PRIVATE REPLAY_FUNCTION(rs_log_pgo_replay_del_global_member);

/*
 * END OF PRIVATE ROUTINES
 */


/*
 * Define Module opcodes and EPV
 */
#define rs_log_pgo_add_op      0
#define rs_log_pgo_delete_op   1
#define rs_log_pgo_replace_op  2
#define rs_log_pgo_rename_op   3
#define rs_log_pgo_add_member_op 4
#define rs_log_pgo_del_member_op 5
#define rs_log_pgo_add_global_member_op 6
#define rs_log_pgo_del_global_member_op 7
#define rs_log_pgo_last        8   /* Must be equal to the number of operations */

static rs_log_ops_t rs_log_pgo_epv[rs_log_pgo_last] = {
    { GLOBAL_UPDATE, rs_log_pgo_prop_add,
      rs_log_pgo_replay_add, "add" } ,
    { GLOBAL_UPDATE, rs_log_pgo_prop_delete,
      rs_log_pgo_replay_delete, "delete" } ,
    { GLOBAL_UPDATE, rs_log_pgo_prop_replace,
      rs_log_pgo_replay_replace, "replace" } ,
    { GLOBAL_UPDATE, rs_log_pgo_prop_rename,
      rs_log_pgo_replay_rename, "rename" } ,
    { GLOBAL_UPDATE, rs_log_pgo_prop_add_member,
      rs_log_pgo_replay_add_member, "add member" } ,
    { GLOBAL_UPDATE, rs_log_pgo_prop_del_member,
      rs_log_pgo_replay_del_member, "delete member" } ,
    { GLOBAL_UPDATE, rs_log_pgo_prop_add_member,
      rs_log_pgo_replay_add_global_member, "add global member" } ,
    { GLOBAL_UPDATE, rs_log_pgo_prop_del_member,
      rs_log_pgo_replay_del_global_member, "delete global member" } ,
};


/*
 * r s _ l o g _ p g o _ i n i t
 *
 * Initialize the PGO logging module
 */
PUBLIC void rs_log_pgo_init ( error_status_t *st )
{
    int         num_ops;

    num_ops = rs_log_pgo_last;
    rs_log_init_module(RS_LOG_MODULE_PGO, num_ops, rs_log_pgo_epv, st);
}


/*
 * i m p o r t _ p g o _ i t e m
 *
 */
PRIVATE void import_pgo_item
  (
    sec_rgy_name_t      pgo_name,
    sec_rgy_pgo_item_t  *pgo_item,
    rs_log_pgo_item_t   *pgo_log_item,
    long                *size
  )
{
    long    pgo_name_len, fullname_len;

    pgo_name_len = strlen( (char *) pgo_name);
    fullname_len = strlen( (char *) pgo_item->fullname);

    pgo_log_item->name_len      = pgo_name_len;
    pgo_log_item->uid           = pgo_item->id;
    pgo_log_item->unix_num      = pgo_item->unix_num;
    pgo_log_item->fullname_len  = fullname_len;
    pgo_log_item->flags         = pgo_item->flags;
    pgo_log_item->quota         = pgo_item->quota;
    strncpy(&(pgo_log_item->string_buf[0]), (char *) pgo_name, pgo_name_len);
    strncpy(&(pgo_log_item->string_buf[pgo_name_len]),
                (char *) pgo_item->fullname, fullname_len);

    *size = sizeof(*pgo_log_item) - sizeof(pgo_log_item->string_buf)
                + pgo_name_len + fullname_len;
}


PRIVATE void export_pgo_item
  (
    rs_log_pgo_item_t   *pgo_log_item, /* [in] */
    sec_rgy_name_t      pgo_name,      /* [out] */
    sec_rgy_pgo_item_t  *pgo_item
  )
{
    pgo_item->id            = pgo_log_item->uid;
    pgo_item->unix_num      = pgo_log_item->unix_num;
    pgo_item->flags         = pgo_log_item->flags;
    pgo_item->quota         = pgo_log_item->quota;
    strncpy( (char *) pgo_name, &(pgo_log_item->string_buf[0]),
        pgo_log_item->name_len);
    pgo_name[pgo_log_item->name_len] = NULL;
    strncpy( (char *) pgo_item->fullname,
            &(pgo_log_item->string_buf[pgo_log_item->name_len]),
                pgo_log_item->fullname_len);
    pgo_item->fullname[pgo_log_item->fullname_len] = NULL;
}


PUBLIC void rs_log_pgo_add
  (
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              pgo_name,
    sec_rgy_pgo_item_t          *pgo_item,
    sec_rgy_foreign_id_t        *client_ids,
    rs_replica_master_info_t    *master_info
  )
{
    long                    size;
    rs_log_pgo_add_t        info;

    info.domain = domain;
    info.client_ids = *client_ids;
    import_pgo_item(pgo_name, pgo_item, &info.item, &size);
    size = size + sizeof(info) - sizeof(info.item);
    rs_log_setup_log_header(&info.upd_hdr, size, RS_LOG_MODULE_PGO,
                rs_log_pgo_add_op);

    if (master_info != NULL) {
        rs_log_write(&info.upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&info.upd_hdr, NULL, NULL);
    }
}


PRIVATE void rs_log_pgo_prop_add
  (
    handle_t                   handle,
    rs_log_rec_t               *info,
    rs_replica_master_info_t   *master_info,
    boolean32                  propq_only,
    error_status_t             *st
  )
{
    rs_log_pgo_add_t           *log_info = (rs_log_pgo_add_t *) info;
    rs_prop_pgo_add_data_t     item[1];
    sec_rgy_name_t             name;

    item[0].client = log_info->client_ids;

    export_pgo_item(&log_info->item, item[0].name, &item[0].item);
    rrs_prop_pgo_add(handle, log_info->domain, 1, item,
                     master_info, propq_only, st);
}


PRIVATE void rs_log_pgo_replay_add
  (
    rs_log_rec_t       *info,
    error_status_t      *st
  )
{
    rs_log_pgo_add_t        *log_info = (rs_log_pgo_add_t *) info;
    sec_rgy_pgo_item_t      item;
    sec_rgy_name_t          name, parent_name, tmp_name;
    long                    parent_name_len, tmp_name_len;
    boolean32               dummy;

    CLEAR_STATUS(st);
    export_pgo_item(&log_info->item, name, &item);
    rs_util_pgo_add(log_info->domain, name, &item, NULL_PTR,
                    &(log_info->client_ids), false, &dummy, st);
    if(GOOD_STATUS(st)) {
        rsdb_util_quota_decrement(&(log_info->client_ids.principal),
                    &(log_info->client_ids.cell), st);
    }
}


PUBLIC void rs_log_pgo_delete
  (
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              name,
    rs_replica_master_info_t    *master_info
  )
{
    long                    size;
    long                    name_len;
    rs_log_pgo_delete_t     info;

    name_len = strlen( (char *) name);

    info.domain     = domain;
    info.name_len   = name_len;
    strncpy( (char *) info.name, (char *) name, name_len);
    size = sizeof(info) - sizeof(info.name) + name_len;
    rsdb_get_domain_cache_state(domain, &info.cache_info);

    rs_log_setup_log_header(&info.upd_hdr, size, RS_LOG_MODULE_PGO,
                rs_log_pgo_delete_op);

    if (master_info != NULL) {
        rs_log_write(&info.upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&info.upd_hdr, NULL, NULL);
    }
}


PRIVATE void rs_log_pgo_prop_delete
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    sec_rgy_name_t		name;
    rs_log_pgo_delete_t        *log_info = (rs_log_pgo_delete_t *) info;

    strncpy( (char *) name, (char *) log_info->name, log_info->name_len);
    name[log_info->name_len] = NULL;

    rrs_prop_pgo_delete(handle, log_info->domain, name,
            log_info->cache_info, master_info, propq_only, st);
}


PRIVATE void rs_log_pgo_replay_delete
  (
    rs_log_rec_t        *info,
    error_status_t      *st
  )
{
    rs_log_pgo_delete_t *log_info = (rs_log_pgo_delete_t *) info;
    sec_rgy_name_t      name;

    CLEAR_STATUS(st);
    strncpy( (char *) name, (char *) log_info->name, log_info->name_len);
    name[log_info->name_len] = NULL;
    rs_util_pgo_delete(log_info->domain, name, st);

    rsdb_set_domain_cache_state(log_info->domain, &(log_info->cache_info));
}


PUBLIC void rs_log_pgo_replace
  (
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              name,
    sec_rgy_pgo_item_t          *item,
    rs_replica_master_info_t    *master_info
  )
{
    long                    size;
    rs_log_pgo_replace_t    info;

    info.domain     = domain;
    import_pgo_item(name, item, &info.item, &size);
    rsdb_get_domain_cache_state(domain, &info.cache_info);
    size = size + sizeof(info) - sizeof(info.item);

    rs_log_setup_log_header(&info.upd_hdr, size, RS_LOG_MODULE_PGO,
                rs_log_pgo_replace_op);

    if (master_info != NULL) {
        rs_log_write(&info.upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&info.upd_hdr, NULL, NULL);
    }
}


PRIVATE void rs_log_pgo_prop_replace
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_pgo_replace_t       *log_info = (rs_log_pgo_replace_t *) info;
    sec_rgy_pgo_item_t         item;
    sec_rgy_name_t             name;

    export_pgo_item(&log_info->item, name, &item);
    rrs_prop_pgo_replace(handle, log_info->domain, name, &item,
             log_info->cache_info, master_info, propq_only, st);
}


PRIVATE void rs_log_pgo_replay_replace
  (
    rs_log_rec_t        *info,
    error_status_t      *st
  )
{
    rs_log_pgo_replace_t    *log_info = (rs_log_pgo_replace_t *) info;
    sec_rgy_pgo_item_t      item;
    sec_rgy_name_t          name;

    CLEAR_STATUS(st);
    export_pgo_item(&log_info->item, name, &item);
    rsdb_pgo_replace(log_info->domain, name, &item, st);
    rsdb_set_domain_cache_state(log_info->domain, &(log_info->cache_info));
}


PUBLIC void rs_log_pgo_rename
  (
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              old_name,
    sec_rgy_name_t              new_name,
    rs_replica_master_info_t    *master_info
  )
{
    long                    size;
    long                    old_name_len;
    long                    new_name_len;
    rs_log_pgo_rename_t     info;

    old_name_len = strlen( (char *) old_name);
    new_name_len = strlen( (char *) new_name);

    info.domain         = domain;
    info.old_name_len   = old_name_len;
    info.new_name_len   = new_name_len;
    strncpy(info.string_buf, (char *) old_name, old_name_len);
    strncpy(&(info.string_buf[old_name_len]), (char *) new_name,
        new_name_len);
    size = sizeof(info) - sizeof(info.string_buf) + old_name_len
       + new_name_len;
    rsdb_get_domain_cache_state(domain, &info.cache_info);

    rs_log_setup_log_header(&info.upd_hdr, size, RS_LOG_MODULE_PGO,
                rs_log_pgo_rename_op);

    if (master_info != NULL) {
        rs_log_write(&info.upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&info.upd_hdr, NULL, NULL);
    }

}


PRIVATE void rs_log_pgo_prop_rename
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_pgo_rename_t        *log_info = (rs_log_pgo_rename_t *) info;
    sec_rgy_name_t             old_name;
    sec_rgy_name_t             new_name;

    strncpy( (char *) old_name, log_info->string_buf, log_info->old_name_len);
    old_name[log_info->old_name_len] = NULL;
    strncpy((char *) new_name, &(log_info->string_buf[log_info->old_name_len]),
        log_info->new_name_len);
    new_name[log_info->new_name_len] = NULL;

    rrs_prop_pgo_rename(handle, log_info->domain, old_name, new_name,
            log_info->cache_info, master_info, propq_only, st);
}


PRIVATE void rs_log_pgo_replay_rename
  (
    rs_log_rec_t        *info,
    error_status_t      *st
  )
{
    rs_log_pgo_rename_t     *log_info = (rs_log_pgo_rename_t *) info;
    sec_rgy_name_t          old_name;
    sec_rgy_name_t          new_name;
    sec_rgy_name_t          old_parent_name;
    sec_rgy_name_t          new_parent_name;
    boolean32               move_required = true;

    CLEAR_STATUS(st);

    strncpy( (char *) old_name, log_info->string_buf, log_info->old_name_len);
    old_name[log_info->old_name_len] = NULL;
    strncpy((char *) new_name, &(log_info->string_buf[log_info->old_name_len]),
        log_info->new_name_len);
    new_name[log_info->new_name_len] = NULL;

    /* Determine if rename or move operation is required. */
    old_parent_name[0] = '\0';
    new_parent_name[0] = '\0';
    rsdb_name_util_get_parent(old_name, old_parent_name);
    rsdb_name_util_get_parent(new_name, new_parent_name);
    if ( ustrcmp(old_parent_name, new_parent_name) == 0 ) {
            move_required = false;
    }

    rsdb_acct_store_default_pepper(old_name, st);

    if (move_required)
        rsdb_pgo_move(log_info->domain, old_name, new_name, st);
    else
        rsdb_pgo_rename(log_info->domain, old_name, new_name, st);

    rsdb_set_domain_cache_state(log_info->domain, &(log_info->cache_info));
}


PRIVATE void rs_log_pgo_member_op
  (
    int                         op,
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              go_name,
    sec_rgy_name_t              person_name,
    uuid_p_t			cell_uuid_p,
    uuid_p_t			princ_uuid_p,
    rs_replica_master_info_t    *master_info
  )
{
    long                        size;
    long                        go_name_len;
    long                        person_name_len;
    rs_log_pgo_global_member_t  info;
    error_status_t              xst;

    go_name_len = strlen( (char *) go_name);
    person_name_len = strlen( (char *) person_name);

    info.domain             = domain;
    info.go_name_len        = go_name_len;
    
    /* 
     * If this member is a foreign principal, then its
     * cell and principal uuids should be passed in ( uuid pointer
     * is not NULL_PTR), so save them in the log info.
     * When log is replayed, the secd of a foreign principal
     * member may not be available at the time.  To ensure
     * the success of log replay, we would need to save these
     * uuids after the foreign principal member was successfullly
     * added.
     */
    if ( cell_uuid_p == NULL_PTR ) {
	uuid_create_nil(&info.person_cell_uuid, &xst);
    } else {
    	info.person_cell_uuid = *cell_uuid_p;
    }
    if ( princ_uuid_p == NULL_PTR ) {
	uuid_create_nil(&info.person_princ_uuid, &xst);
    } else {
	info.person_princ_uuid = *princ_uuid_p;
    }

    info.person_name_len    = person_name_len;
    strncpy(&info.string_buf[0], (char *) go_name, go_name_len);
    strncpy(&info.string_buf[go_name_len], (char *) person_name,
        person_name_len);
    size = sizeof(info) - sizeof(info.string_buf)
                + go_name_len + person_name_len;

    rs_log_setup_log_header(&info.upd_hdr, size, RS_LOG_MODULE_PGO, op);

    if (master_info != NULL) {
        rs_log_write(&info.upd_hdr,
            &master_info->update_seqno, &master_info->update_ts);
    }
    else {
        rs_log_write(&info.upd_hdr, NULL, NULL);
    }
}


PUBLIC void rs_log_pgo_add_member
  (
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              go_name,
    sec_rgy_name_t              person_name,
    uuid_p_t			cell_uuid_p,
    uuid_p_t			princ_uuid_p,
    rs_replica_master_info_t    *master_info
  )
{
    rs_log_pgo_member_op(rs_log_pgo_add_global_member_op, domain, go_name,
                         person_name, cell_uuid_p, princ_uuid_p, master_info);
}


PRIVATE void rs_log_pgo_prop_add_member
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_pgo_global_member_t *log_info = (rs_log_pgo_global_member_t *) info;
    sec_rgy_name_t             go_name;
    sec_rgy_member_t           member[1];
    uuid_p_t		       cell_uuids[1];
    uuid_p_t		       princ_uuids[1];

    strncpy((char *) go_name, log_info->string_buf, log_info->go_name_len);
    go_name[log_info->go_name_len] = NULL;
    strncpy((char *) member[0], &(log_info->string_buf[log_info->go_name_len]),
        log_info->person_name_len);
    member[0][log_info->person_name_len] = NULL;
    cell_uuids[0] = &log_info->person_cell_uuid;
    princ_uuids[0] = &log_info->person_princ_uuid;

    rrs_prop_pgo_add_member(handle, log_info->domain, go_name, 1, member,
	cell_uuids, princ_uuids, master_info, propq_only, st);
}


PRIVATE void rs_log_pgo_replay_add_member
  (
    rs_log_rec_t    *info,
    error_status_t  *st
  )
{
    rs_log_pgo_member_t *log_info = (rs_log_pgo_member_t *) info;
    sec_rgy_name_t      go_name;
    sec_rgy_name_t      person_name;

    CLEAR_STATUS(st);

    strncpy( (char *) go_name, log_info->string_buf, log_info->go_name_len);
    go_name[log_info->go_name_len] = NULL;
    strncpy( (char *) person_name,
        &(log_info->string_buf[log_info->go_name_len]),
        log_info->person_name_len);
    person_name[log_info->person_name_len] = NULL;

    rsdb_pgo_add_member(log_info->domain, go_name, person_name, st);
}


PRIVATE void rs_log_pgo_replay_add_global_member
  (
    rs_log_rec_t    *info,
    error_status_t  *st
  )
{
    rs_log_pgo_global_member_t *log_info = (rs_log_pgo_global_member_t *) info;
    sec_rgy_name_t      go_name;
    sec_rgy_name_t      person_name;

    CLEAR_STATUS(st);

    strncpy( (char *) go_name, log_info->string_buf, log_info->go_name_len);
    go_name[log_info->go_name_len] = NULL;
    strncpy( (char *) person_name,
        &(log_info->string_buf[log_info->go_name_len]),
        log_info->person_name_len);
    person_name[log_info->person_name_len] = NULL;

    /* 
     * if person_cell_uuid is not a nil uuid, then this add member is for
     * a foreign principal.  Try to add this principal locally if it does
     * not yet exist.
     */
    if ( ! uuid_is_nil(&log_info->person_cell_uuid, st) ) {
	if ( STATUS_OK(st) ) {
    	    rs_util_pgo_add_foreign_principal(person_name, 
		&log_info->person_cell_uuid, 
		&log_info->person_princ_uuid, st);
	}
    }

    if ( STATUS_OK(st) ) {
    	rsdb_pgo_add_member(log_info->domain, go_name, person_name, st);
    }
}


PUBLIC void rs_log_pgo_delete_member
  (
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              go_name,
    sec_rgy_name_t              person_name,
    rs_replica_master_info_t    *master_info
  )
{
    rs_log_pgo_member_op(rs_log_pgo_del_global_member_op, domain, go_name,
        person_name, NULL_PTR, NULL_PTR, master_info);
}


PRIVATE void rs_log_pgo_prop_del_member
  (
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_pgo_member_t        *log_info = (rs_log_pgo_member_t *) info;
    sec_rgy_name_t             go_name;
    sec_rgy_name_t             person_name;

    CLEAR_STATUS(st);

    strncpy( (char *) go_name, log_info->string_buf, log_info->go_name_len);
    go_name[log_info->go_name_len] = NULL;
    strncpy( (char *) person_name,
        &(log_info->string_buf[log_info->go_name_len]),
        log_info->person_name_len);
    person_name[log_info->person_name_len] = NULL;

    rrs_prop_pgo_delete_member(handle, log_info->domain, go_name,
                               person_name, master_info, propq_only, st);
}


PRIVATE void rs_log_pgo_replay_del_member
  (
    rs_log_rec_t        *info,
    error_status_t      *st
  )
{
    rs_log_pgo_member_t    *log_info = (rs_log_pgo_member_t *) info;
    sec_rgy_name_t         go_name;
    sec_rgy_name_t         person_name;

    CLEAR_STATUS(st);

    strncpy( (char *) go_name, log_info->string_buf, log_info->go_name_len);
    go_name[log_info->go_name_len] = NULL;
    strncpy( (char *) person_name,
        &(log_info->string_buf[log_info->go_name_len]),
        log_info->person_name_len);
    person_name[log_info->person_name_len] = NULL;

    rsdb_pgo_delete_member(log_info->domain, go_name, person_name, st);
}


/* 
 * Although we don't care about the contents of the local foreign-principal
 * database on a member delete, we need this function to make things work
 * for the log replay.
 */
PRIVATE void rs_log_pgo_replay_del_global_member
  (
    rs_log_rec_t        *info,
    error_status_t      *st
  )
{
    rs_log_pgo_global_member_t    *log_info = (rs_log_pgo_global_member_t *) info;
    sec_rgy_name_t         go_name;
    sec_rgy_name_t         person_name;

    CLEAR_STATUS(st);

    strncpy( (char *) go_name, log_info->string_buf, log_info->go_name_len);
    go_name[log_info->go_name_len] = NULL;
    strncpy( (char *) person_name,
        &(log_info->string_buf[log_info->go_name_len]),
        log_info->person_name_len);
    person_name[log_info->person_name_len] = NULL;

    rsdb_pgo_delete_member(log_info->domain, go_name, person_name, st);
}
