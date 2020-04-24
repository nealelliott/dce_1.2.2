/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_attr_schema.c,v $
 * Revision 1.1.10.1  1996/07/08  18:38:14  arvind
 * 	[OT13319] secd failure caused by sec_rgy_attr_sch_test
 * 	[1996/06/14  20:09 UTC  pdd  /main/pdd_dce_1_2/1]
 *
 * 	[OT13319] secd failure caused by sec_rgy_attr_sch_test
 * 	[1996/02/18  00:18:01  marty  1.1.8.3]
 *
 * Revision 1.1.8.3  1996/02/18  00:18:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:52  marty]
 * 
 * Revision 1.1.8.2  1995/12/08  17:51:45  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/10/30  19:40 UTC  psn
 * 	Fix OSF OT 13160, CHFts16737
 * 	[1995/10/30  18:15 UTC  psn  /main/psn_ot13160/1]
 * 
 * Revision 1.1.4.1  1994/10/25  21:14:04  burati
 * 	CR12713 Attribute schema updates not logged or propagated
 * 	[1994/10/25  19:55:43  burati]
 * 
 * Revision 1.1.2.6  1994/09/22  20:08:16  cuti
 * 	Fix p for schema_name in _sch_replay_create().
 * 	[1994/09/22  02:58:06  cuti]
 * 
 * 	OT12106: Fix total_size in alloc_attr_sch_create() and p for schema_name in _schema_create().
 * 	[1994/09/21  17:03:37  cuti]
 * 
 * Revision 1.1.2.5  1994/07/15  15:01:29  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:19:43  mdf]
 * 
 * Revision 1.1.2.4  1994/06/17  18:42:50  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:16:14  devsrc]
 * 
 * Revision 1.1.2.3  1994/06/02  21:25:15  mdf
 * 	enable and disable rcp_ss_allocate on propagation calls
 * 	[1994/06/01  20:48:47  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:35  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	HP revision /main/ODESSA_2/6  1994/04/26  19:57 UTC  mdf
 * 	Support for ERA propagation.
 * 
 * 	/main/ODESSA_2/mdf_odessa_era_bl33/2  1994/04/26  19:33 UTC  mdf
 * 	propagation support for ERA's
 * 
 * 	HP revision /main/ODESSA_2/mdf_odessa_era_bl33/1  1994/04/13  13:05 UTC  mdf
 * 	Added propagation support, and call rsdb_attr_schema_call_size instead
 * 	of having duplicate code.
 * 
 * 	HP revision /main/ODESSA_2/5  1994/04/01  13:06 UTC  mdf
 * 	 Support for logging attribute updates
 * 
 * 	HP revision /main/ODESSA_2/mdf_odessa_era_bl29/1  1994/03/31  18:42 UTC  mdf
 * 	bug fixes.
 * 
 * 	/main/ODESSA_2/4  1994/03/29  19:42 UTC  mdf
 * 	 Logging support of the create and delete ERA functions
 * 
 * 	/main/ODESSA_2/mdf_odessa_era_bl28/1  1994/03/29  19:34 UTC  mdf
 * 	New functionality to support attribute schema (create and delete) logging
 * 	operations (except for prop routines).
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

/*
 *     Registry Server - Attribute Schema Log operations
 */

#include <rs_log.h>
#include <rsdb_attr_pvt.h>              /* macros */
#include <rsdb_attr_schema.h>           /* prototypes */
#include <rrs_prop_attr_schema.h>
#include <sec_attr_tools.h> 

/*
 * Local types 
 */
typedef struct rs_log_attr_sch_create_t {
    rs_log_hdr_t		upd_hdr;
    unsigned32			schema_name_len;
    rsdb_attr_schema_entry_t	schema_entry;
} rs_log_attr_sch_create_t;

typedef struct rs_log_attr_sch_delete_t {
    rs_log_hdr_t		upd_hdr;
    unsigned32			schema_name_len;
    uuid_t			attr_id;
    char 			buf[VARYING];
} rs_log_attr_sch_delete_t;

typedef struct rs_log_attr_sch_update_t {
    rs_log_hdr_t		    upd_hdr;
    sec_attr_schema_entry_parts_t   modify_parts;
    unsigned32			    schema_name_len;
    rsdb_attr_schema_entry_t	    schema_entry;
} rs_log_attr_sch_update_t;


PRIVATE PROP_FUNCTION(rs_log_attr_sch_prop_create);
PRIVATE PROP_FUNCTION(rs_log_attr_sch_prop_delete);
PRIVATE PROP_FUNCTION(rs_log_attr_sch_prop_update);

PRIVATE REPLAY_FUNCTION(rs_log_attr_sch_replay_create);
PRIVATE REPLAY_FUNCTION(rs_log_attr_sch_replay_delete);
PRIVATE REPLAY_FUNCTION(rs_log_attr_sch_replay_update);

/*
 * Define Module opcodes and EPV
 */
#define rs_log_attr_sch_create_op	0
#define rs_log_attr_sch_delete_op	1
#define rs_log_attr_sch_update_op	2
#define rs_log_attr_sch_last		3   /* The number of operations */

static rs_log_ops_t rs_log_attr_sch_epv[rs_log_attr_sch_last] = {

    {	GLOBAL_UPDATE,
	rs_log_attr_sch_prop_create,
	rs_log_attr_sch_replay_create,
	"create" },

    {	GLOBAL_UPDATE,
	rs_log_attr_sch_prop_delete,
	rs_log_attr_sch_replay_delete,
	"delete" },

    {	GLOBAL_UPDATE,
	rs_log_attr_sch_prop_update,
	rs_log_attr_sch_replay_update,
	"update" },
};

PUBLIC void
rs_log_attr_sch_init(error_status_t  *st)
{
    int num_ops = rs_log_attr_sch_last;
    rs_log_init_module(RS_LOG_MODULE_ATTR_SCH, num_ops, rs_log_attr_sch_epv, st);
}

PRIVATE void
alloc_attr_sch_create(
    unsigned32 			name_len,		/* in */
    sec_attr_schema_entry_t	*sec_sch_entry_p,	/* in */
    rs_log_attr_sch_create_t	**info_p,		/* out */
    unsigned32			*size,			/* out */
    unsigned32			*size_schema_entry,	/* out */
    error_status_t		*st_p)			/* out */
{
    int                 i, j;
    unsigned32          sch_entry_len, total_size;
    Pointer             new_buf;

    CLEAR_STATUS(st_p);
    *info_p = (rs_log_attr_sch_create_t *)NULL;

    total_size = sizeof(rs_log_attr_sch_create_t);
    rsdb_attr_sch_calc_size(sec_sch_entry_p, &sch_entry_len, st_p);
    if (BAD_STATUS(st_p)) return;
    total_size += (sch_entry_len - sizeof(rsdb_attr_schema_entry_t) + name_len);

    if(!(new_buf = rpc_ss_allocate(total_size))) {
        SET_STATUS(st_p, sec_attr_no_memory);
        return;
    }
    *size = total_size;
    *size_schema_entry = sch_entry_len;
    *info_p = (rs_log_attr_sch_create_t *) new_buf;
    memset((Pointer)(*info_p), NULL, total_size);
    return;
}

PRIVATE void alloc_attr_sch_update(
    unsigned32 			name_len,		/* in */
    sec_attr_schema_entry_t	*sec_sch_entry_p,	/* in */
    rs_log_attr_sch_update_t	**info_p,		/* out */
    long			*size,			/* out */
    long			*size_schema_entry,	/* out */
    error_status_t		*st_p)			/* out */
{
    int                 i, j;
    unsigned32          sch_entry_len, total_size;
    unsigned32          length;
    Pointer             new_buf;

    CLEAR_STATUS(st_p);
    *info_p = (rs_log_attr_sch_update_t *)NULL;

    total_size = sizeof(rs_log_attr_sch_update_t);
    rsdb_attr_sch_calc_size(sec_sch_entry_p, &sch_entry_len, st_p);
    if (BAD_STATUS(st_p)) return;
    total_size += (sch_entry_len - sizeof(rsdb_attr_schema_entry_t) + name_len);

    if(!(new_buf = rpc_ss_allocate(total_size))) {
        SET_STATUS(st_p, sec_attr_no_memory);
        return;
    }
    *size = total_size;
    *size_schema_entry = sch_entry_len;
    *info_p = (rs_log_attr_sch_update_t *) new_buf;
    memset((Pointer)(*info_p), NULL, total_size);
    return;
}

#define WRITE_LOG    								\
if (master_info != NULL) {							\
    rs_log_write(&(info->upd_hdr),						\
	&master_info->update_seqno, &master_info->update_ts);			\
} else {									\
    rs_log_write(&(info->upd_hdr), NULL, NULL);					\
}

PUBLIC void
rs_log_attr_schema_create
(
    sec_attr_component_name_t	schema_name,    /* [in] */
    sec_attr_schema_entry_t	*schema_entry,  /* [in] */
    rs_replica_master_info_t    *master_info	/* [in] */
)
{
    unsigned32			size, schema_entry_size;
    rs_log_attr_sch_create_t	*info;
    unsigned32			schema_name_len;
    error_status_t		st, *st_p = &st;
    unsigned_char_t		*p;

    CLEAR_STATUS(st_p);

    size = 0;
    p = (unsigned_char_t *)schema_name;
    schema_name_len = u_strlen(p);

    alloc_attr_sch_create(schema_name_len, schema_entry, &info, &size,
	&schema_entry_size, st_p);
    if (BAD_STATUS(st_p)) {
	return;
    }
    info->schema_name_len = schema_name_len;
    info->schema_entry.sch_entry_len = schema_entry_size;
    rsdb_attr_sch_import_info(schema_entry, &(info->schema_entry), st_p);
    if (BAD_STATUS(st_p)) {
	return;
    }
  
    /* copy schema_name into buffer at end of schema_entry */
    if (schema_name_len != 0 ) {
        p = (unsigned_char_t *) info;
        p += (sizeof(rs_log_attr_sch_create_t) + info->schema_entry.sch_entry_len -
                   sizeof(rsdb_attr_schema_entry_t));
	u_strncpy(p, schema_name, schema_name_len);
    }

    rs_log_setup_log_header(&(info->upd_hdr), size, RS_LOG_MODULE_ATTR_SCH,
                rs_log_attr_sch_create_op);

    WRITE_LOG;
}

PRIVATE void
rs_log_attr_sch_prop_create
(
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st_p
)
{
    rs_log_attr_sch_create_t	*log_info = (rs_log_attr_sch_create_t *) info;
    rs_prop_attr_sch_create_data_t	schema[1];
    unsigned32			num_schemas = 1;
    unsigned_char_t		*p;
    sec_attr_component_name_t	schema_name;


    CLEAR_STATUS(st_p);
    rpc_ss_enable_allocate();


    rsdb_attr_sch_export(&log_info->schema_entry, rpc_ss_allocate,
	&schema[0].schema_entry, st_p);
    if (BAD_STATUS(st_p)) {
        return;
    }
    if (log_info->schema_name_len != 0 ) {
	if (!(schema[0].schema_name = (unsigned_char_p_t) 
	    rpc_ss_allocate(log_info->schema_name_len))) {
		SET_STATUS(st_p, sec_attr_no_memory);
		return;
	}
        p = (unsigned_char_t *)log_info;
        p += sizeof(rs_log_attr_sch_create_t) +
		log_info->schema_entry.sch_entry_len
                - sizeof(rsdb_attr_schema_entry_t);
        u_strncpy(schema[0].schema_name, p, log_info->schema_name_len);
        schema[0].schema_name[log_info->schema_name_len] = '\0';
    }

    rrs_prop_attr_schema_create(handle, num_schemas, schema,
	master_info, propq_only, st_p);

    rpc_ss_disable_allocate();
    return;
}

PRIVATE void
rs_log_attr_sch_replay_create
(
    rs_log_rec_t *info,
    error_status_t *st_p
)
{
    unsigned_char_t		*p;
    sec_attr_schema_entry_t 	schema_entry;
    sec_attr_component_name_t	schema_name = NULL;
    rs_log_attr_sch_create_t	*log_info = (rs_log_attr_sch_create_t *) info;

    CLEAR_STATUS(st_p);
    rsdb_attr_sch_export(&log_info->schema_entry, rpc_ss_allocate,
	 &schema_entry, st_p);
    if (BAD_STATUS(st_p)) {
        return;
    }
    if (log_info->schema_name_len != 0 ) {
	if (!(schema_name = (unsigned_char_p_t) 
	    rpc_ss_allocate(log_info->schema_name_len))) {
		SET_STATUS(st_p, sec_attr_no_memory);
		return;
	}
        p = (unsigned_char_t *)info;
        p += (sizeof(rs_log_attr_sch_create_t) +
	     log_info->schema_entry.sch_entry_len
             - sizeof(rsdb_attr_schema_entry_t));
        u_strncpy(schema_name, p, log_info->schema_name_len);
        schema_name[log_info->schema_name_len] = '\0';
    }

    rsdb_attr_sch_add(schema_name, &schema_entry, st_p);
    return;
}

PRIVATE void
alloc_attr_sch_delete(
    unsigned32 			name_len,		/* in */
    rs_log_attr_sch_delete_t	**info,			/* out */
    long			*size,			/* out */
    error_status_t		*st_p)			/* out */
{
    unsigned_char_p_t   new_buf;

    CLEAR_STATUS(st_p);
    *info = (rs_log_attr_sch_delete_t *)NULL;
    *size = sizeof(rs_log_attr_sch_delete_t) + name_len;

    if (!(new_buf = (unsigned_char_p_t) rpc_ss_allocate(*size))) {
	SET_STATUS(st_p, sec_attr_no_memory);
        return;
    }
    *info = (rs_log_attr_sch_delete_t *) new_buf;
    memset((Pointer)(*info), NULL, *size);
}

PUBLIC void
rs_log_attr_schema_delete
(
    sec_attr_component_name_t	schema_name,	/* [in] */
    uuid_t			*attr_id, 	/* [in] */
    rs_replica_master_info_t    *master_info	/* [in] */
)
{
    long			size;
    rs_log_attr_sch_delete_t	*info;
    unsigned32			schema_name_len;
    error_status_t		st, *st_p = &st;
    unsigned_char_t		*p;

    CLEAR_STATUS(st_p);

    size = 0;
    p = (unsigned_char_t *)schema_name;
    schema_name_len = u_strlen(p);

    alloc_attr_sch_delete(schema_name_len, &info, &size, st_p);
    if (BAD_STATUS(st_p)) {
	return;
    }

    info->schema_name_len = schema_name_len;
    info->attr_id = *attr_id;
  
    /* copy schema_name into buffer at end of schema_entry */
    if (schema_name_len != 0 ) {
        p = (unsigned_char_t *)info;
        p += sizeof(rs_log_attr_sch_delete_t);
        u_strncpy(p, schema_name, schema_name_len);
    }
    rs_log_setup_log_header(&(info->upd_hdr), size, RS_LOG_MODULE_ATTR_SCH,
                rs_log_attr_sch_delete_op);
    WRITE_LOG;
}


PRIVATE void
rs_log_attr_sch_prop_delete
(
    handle_t                    handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st_p
)
{
    rs_log_attr_sch_delete_t	*log_info = (rs_log_attr_sch_delete_t *) info;
    unsigned_char_t		*p;
    sec_attr_component_name_t	schema_name = NULL;
    uuid_t			*attr_id;

    CLEAR_STATUS(st_p);
    rpc_ss_enable_allocate();

    if (log_info->schema_name_len != 0 ) {
	if (!(schema_name = (unsigned_char_p_t)
	    rpc_ss_allocate(log_info->schema_name_len))) {
		SET_STATUS(st_p, sec_attr_no_memory);
		return;
	}
        p = (unsigned_char_t *)log_info;
        p += sizeof(rs_log_attr_sch_delete_t);
        u_strncpy(schema_name, p, log_info->schema_name_len);
        schema_name[log_info->schema_name_len] = '\0';
    }

    rrs_prop_attr_schema_delete(handle, schema_name, &log_info->attr_id,
	master_info, propq_only, st_p);
    
    rpc_ss_disable_allocate();
    return;
}

PRIVATE void
rs_log_attr_sch_replay_delete
(
    rs_log_rec_t *info,
    error_status_t *st_p
)
{
    unsigned_char_t		*p;
    sec_attr_component_name_t	schema_name;
    rs_log_attr_sch_delete_t	*log_info = (rs_log_attr_sch_delete_t *) info;

    CLEAR_STATUS(st_p);
    if (log_info->schema_name_len != 0 ) {
	if (!(schema_name = (unsigned_char_p_t)
	    rpc_ss_allocate(log_info->schema_name_len))) {
		SET_STATUS(st_p, sec_attr_no_memory);
		return;
	}
        p = (unsigned_char_t *)log_info;
        p += sizeof(rs_log_attr_sch_delete_t);
        u_strncpy(schema_name, p, log_info->schema_name_len);
        schema_name[log_info->schema_name_len] = '\0';
    }
    rsdb_attr_sch_delete(schema_name, &log_info->attr_id, st_p);
    return;
}

PUBLIC void rs_log_attr_schema_update
(
    sec_attr_component_name_t       schema_name,    /* [in] */
    sec_attr_schema_entry_t         *schema_entry,  /* [in] */
    sec_attr_schema_entry_parts_t   modify_parts,   /* [in] */
    rs_replica_master_info_t        *master_info    /* [in] */
)
{
    long			size, schema_entry_size;
    rs_log_attr_sch_update_t	*info;
    unsigned32			schema_name_len;
    error_status_t		st, *st_p = &st;
    unsigned_char_t		*p;

    CLEAR_STATUS(st_p);

    size = 0;
    p = (unsigned_char_t *)schema_name;
    schema_name_len = u_strlen(p);

    alloc_attr_sch_update(schema_name_len, schema_entry, &info, &size,
	&schema_entry_size, st_p);
    if (BAD_STATUS(st_p)) {
	return;
    }
    info->schema_name_len = schema_name_len;
    info->schema_entry.sch_entry_len = schema_entry_size;
    info->modify_parts = modify_parts;
    rsdb_attr_sch_import_info(schema_entry, &(info->schema_entry), st_p);
    if (BAD_STATUS(st_p)) {
	return;
    }

    /* copy schema_name into buffer at end of schema_entry */
    if (schema_name_len != 0 ) {
        p = (unsigned_char_t *) info;
        p += (sizeof(rs_log_attr_sch_update_t) +
	    info->schema_entry.sch_entry_len -
	    sizeof(rsdb_attr_schema_entry_t));
	u_strncpy(p, schema_name, schema_name_len);
    }

    rs_log_setup_log_header(&(info->upd_hdr), size, RS_LOG_MODULE_ATTR_SCH,
                rs_log_attr_sch_update_op);

    WRITE_LOG;
}

PRIVATE void rs_log_attr_sch_replay_update
(
    rs_log_rec_t *info,
    error_status_t *st_p
)
{
    unsigned_char_t		*p;
    sec_attr_schema_entry_t 	schema_entry;
    sec_attr_component_name_t	schema_name = NULL;
    rs_log_attr_sch_update_t	*log_info = (rs_log_attr_sch_update_t *) info;

    CLEAR_STATUS(st_p);
    rsdb_attr_sch_export(&log_info->schema_entry, rpc_ss_allocate,
	 &schema_entry, st_p);
    if (BAD_STATUS(st_p)) {
        return;
    }
    if (log_info->schema_name_len != 0 ) {
	if (!(schema_name = (unsigned_char_p_t) 
	    rpc_ss_allocate(log_info->schema_name_len))) {
		SET_STATUS(st_p, sec_attr_no_memory);
		return;
	}
        p = (unsigned_char_t *)info;
        p += (sizeof(rs_log_attr_sch_update_t) +
	     log_info->schema_entry.sch_entry_len
             - sizeof(rsdb_attr_schema_entry_t));
        u_strncpy(schema_name, p, log_info->schema_name_len);
        schema_name[log_info->schema_name_len] = '\0';
    }
    rsdb_attr_sch_replace(schema_name, &schema_entry, log_info->modify_parts,
        st_p);
}

PRIVATE void rs_log_attr_sch_prop_update
(
    handle_t             	handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st_p
)
{
    rs_log_attr_sch_update_t	*log_info = (rs_log_attr_sch_update_t *) info;
    rs_prop_attr_sch_update_data_t  schema = {0};
    unsigned_char_t		    *p;
    sec_attr_component_name_t	    schema_name;

    CLEAR_STATUS(st_p);
    rpc_ss_enable_allocate();

    schema.modify_parts = log_info->modify_parts;
    rsdb_attr_sch_export(&log_info->schema_entry, rpc_ss_allocate,
	&schema.schema_entry, st_p);
    if (BAD_STATUS(st_p)) {
        return;
    }
    if (log_info->schema_name_len != 0 ) {
	if (!(schema.schema_name = (unsigned_char_p_t)
	    rpc_ss_allocate(log_info->schema_name_len))) {
		SET_STATUS(st_p, sec_attr_no_memory);
		return;
	}
        p = (unsigned_char_t *)log_info;
        p += sizeof(rs_log_attr_sch_update_t) +
		log_info->schema_entry.sch_entry_len
                - sizeof(rsdb_attr_schema_entry_t);
        u_strncpy(schema.schema_name, p, log_info->schema_name_len);
        schema.schema_name[log_info->schema_name_len] = '\0';
    }

    rrs_prop_attr_schema_update(handle, &schema, master_info, propq_only,st_p);

    rpc_ss_disable_allocate();
    return;
}
