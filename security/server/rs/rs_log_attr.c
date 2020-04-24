/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_attr.c,v $
 * Revision 1.1.4.2  1996/02/18  00:17:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:51:39  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/30  14:52 UTC  greg
 * 	Fix for CHFts15374: Remove local ERA schema lookups from rs_log_attr_prop
 * 	functions.  The lookups are unecessary and can prevent propagations
 * 	under certain conditions.
 * 
 * 	HP revision /main/greg_mothra_debug/1  1995/05/26  17:19 UTC  greg
 * 	Fix for CHFts15374.
 * 	[1995/12/08  17:20:31  root]
 * 
 * Revision 1.1.2.4  1994/09/02  17:47:28  mdf
 * 	fix bogus check ins
 * 	[1994/08/31  19:16:48  mdf]
 * 
 * 	CR 11868  Initial checkin of missing functionality that propagates removal of attributes
 * 	[1994/08/30  13:36:14  mdf]
 * 
 * Revision 1.1.2.3  1994/06/17  18:42:49  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:15:40  devsrc]
 * 
 * Revision 1.1.2.2  1994/06/02  21:25:13  mdf
 * 	enable and disable rcp_ss_allocate on propagation calls
 * 	[1994/06/01  20:48:30  mdf]
 * 
 * 	fixed propagation problem
 * 	[1994/06/01  20:18:17  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:32  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  12:00:37  mdf]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

/*
 *     Registry Server - Attribute Log operations
 */

#include <rs_log.h>
#include <sec_attr_tools.h> 
#include <dce/sec_attr_util.h>

#include <rs_ch_lock.h>
#include <rs_auth.h>
#include <rs_util.h>
#include <rs_attr_pvt.h>
#include <rrs_prop_attr.h>

#include <rsdb_pvt.h>
#include <rsdb_acl.h>
#include <rsdb_util.h>
#include <rsdb_create.h>
#include <rsdb_attr.h>
#include <rsdb_attr_schema.h>

/*
 * Local types 
 */
typedef struct rs_log_attr_t {
    rs_log_hdr_t	upd_hdr;
    unsigned32		component_name_len;
    unsigned32		number;
    char	 	buf[VARYING]; 
/*					    
    {					   * These values are off the end of 
	encoding type, 			   * the structure,  this comment
	attr_inst_len, 			   * is meant to serve as a guide.
	attr_id, 			     
	sec_attr_t			    
    } * number;			    
  component_name;			    
*/
						
} rs_log_attr_t;


PRIVATE PROP_FUNCTION(rs_log_attr_prop_update);
PRIVATE PROP_FUNCTION(rs_log_attr_prop_delete);

PRIVATE REPLAY_FUNCTION(rs_log_attr_replay_update);
PRIVATE REPLAY_FUNCTION(rs_log_attr_replay_delete);

/*
 * Define Module opcodes and EPV
 */
#define rs_log_attr_update_op	0
#define rs_log_attr_delete_op	1
#define rs_log_attr_last	2   /* The number of operations */

static rs_log_ops_t rs_log_attr_epv[rs_log_attr_last] = {

    {	GLOBAL_UPDATE,
	rs_log_attr_prop_update,
	rs_log_attr_replay_update,
	"update" },

    {	GLOBAL_UPDATE,
	rs_log_attr_prop_delete,
	rs_log_attr_replay_delete,
	"delete" },

};

PUBLIC void
rs_log_attr_init(error_status_t  *st)
{
    int num_ops = rs_log_attr_last;
    rs_log_init_module(RS_LOG_MODULE_ATTR, num_ops, rs_log_attr_epv, st);
}

PRIVATE void
alloc_attr(
	long size,
	rs_log_attr_t **info_p,
	error_status_t *st_p
)
{
     Pointer new_buf;
     CLEAR_STATUS(st_p);
     *info_p = (rs_log_attr_t *)NULL;
     if(!(new_buf = rpc_ss_allocate(size))) {
        SET_STATUS(st_p, sec_attr_no_memory);
        return;
    }
    *info_p = (rs_log_attr_t *) new_buf;
    memset((Pointer)(*info_p), NULL, size);
}

#define WRITE_LOG                                                               \
if (master_info != NULL) {                                                      \
    rs_log_write(&(info->upd_hdr),                                              \
        &master_info->update_seqno, &master_info->update_ts);                   \
} else {                                                                        \
    rs_log_write(&(info->upd_hdr), NULL, NULL);                                 \
}



PUBLIC void
rs_log_attr_update
(
    sec_attr_component_name_t	component_name,
    unsigned32			number,
    sec_attr_t			attrs[],
    rs_replica_master_info_t    *master_info 
)
{
    long			size;
    long			*attr_inst_len;
    unsigned32			component_name_len;
    sec_attr_t			*sec_attr_p;
    error_status_t              st, *st_p = &st;
    rs_log_attr_t	*info;
    Pointer 			bufp;
    Pointer 			ptr;
    unsigned_char_t		*p;
    int		i;

    if(!(attr_inst_len = (long *)rpc_ss_allocate(sizeof(long) * number))) {
        return;
    }
    p = (unsigned_char_t *)component_name;
    component_name_len = u_strlen(p);

    size = sizeof(rs_log_attr_t);
    size += component_name_len;

    /*
     * Calculate how much space will be required.  
     * Also, save the sizes of each attr_inst_len, so that we don't 
     * have to regenerate the sizes for storage.
     */
    for ( i = 0; i < number; i++) {
	sec_attr_p = &attrs[i];
    	rsdb_attr_calc_size_value(sec_attr_p, &attr_inst_len[i], st_p);
	size += attr_inst_len[i] +
		sizeof(SA_ATTR_ENCODING(sec_attr_p)) +
		sizeof(attr_inst_len[i]) +
		sizeof(uuid_t);
        if (BAD_STATUS(st_p)) {
            return;
        }
    }

    alloc_attr(size, &info, st_p);

    if (BAD_STATUS(st_p))
	return;

    info->number = number;
    info->component_name_len = component_name_len;

    bufp = (Pointer) (info)->buf;
    for( i = 0; i < number; i++) {
	sec_attr_p = &attrs[i];

	RSDB_ATTR_COPY_ADV_TGT(bufp, &SA_ATTR_ENCODING(sec_attr_p),
		sizeof(SA_ATTR_ENCODING(sec_attr_p)));
	RSDB_ATTR_COPY_ADV_TGT(bufp, &attr_inst_len[i], 
		sizeof(attr_inst_len[i]));
	RSDB_ATTR_COPY_ADV_TGT(bufp, &sec_attr_p->attr_id,
		sizeof(sec_attr_p->attr_id));

	rsdb_attr_import_value(sec_attr_p, bufp, st_p);
	bufp += attr_inst_len[i];

	if (BAD_STATUS(st_p)) {
	    return;
	}
    }

    if (component_name_len != 0 ) {
        RSDB_ATTR_COPY_ADV_TGT(bufp, component_name, component_name_len);
    }

    rs_log_setup_log_header(&(info->upd_hdr), size,
	RS_LOG_MODULE_ATTR, rs_log_attr_update_op);
    WRITE_LOG;
}

PRIVATE void
rs_log_attr_replay_update
(
    rs_log_rec_t *info,
    error_status_t *st_p
)
{
    rsdb_util_rgy_obj_info_t    rgy_obj_info;
    uuid_t                      obj_uuid;
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32                    *xref = NULL;
    signed32			failure_index;
    signed32                    i, j;

    Pointer 		 	bufp;
    sec_attr_t		 	*sec_attr, *sec_attr_p;
    unsigned_char_t	 	*p;
    rs_log_attr_t *rs_log_attr_p = (rs_log_attr_t *)info;
    sec_attr_component_name_t	component_name;
    long			attr_inst_len;

    CLEAR_STATUS(st_p);

    sec_attr = (sec_attr_t *) rpc_ss_allocate(sizeof(sec_attr_t) *
		rs_log_attr_p->number );


    bufp = (Pointer) (rs_log_attr_p)->buf;
    for( i = 0; i < rs_log_attr_p->number; i++) {
	sec_attr_p = &sec_attr[i];

	RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_ENCODING(sec_attr_p), bufp,
		sizeof(SA_ATTR_ENCODING(sec_attr_p)));

	RSDB_ATTR_COPY_ADV_SRC(&attr_inst_len, bufp, sizeof(attr_inst_len));

	RSDB_ATTR_COPY_ADV_SRC(&sec_attr_p->attr_id, bufp,
		sizeof(sec_attr_p->attr_id));

	rsdb_attr_export_value(sec_attr_p, rpc_ss_allocate, bufp, st_p);
	bufp += attr_inst_len;

	if (BAD_STATUS(st_p)) {
	    return;
	}
    }

    if (rs_log_attr_p->component_name_len != 0 ) {
	if (!(component_name = (unsigned_char_p_t)
              rpc_ss_allocate(rs_log_attr_p->component_name_len))) {
            SET_STATUS(st_p, sec_attr_no_memory);
            return;
        }
        RSDB_ATTR_COPY_ADV_SRC(component_name, bufp,
		rs_log_attr_p->component_name_len);
	component_name[rs_log_attr_p->component_name_len] = '\0';
    }

    rsdb_util_get_obj_info_by_name(component_name, 
	    rsdb_pgo_type, sec_acl_type_object,
            &rgy_obj_info, &obj_uuid, st_p);

    if(BAD_STATUS(st_p)) {
	return;
    }

    rsdb_attr_sch_get_entries((sec_attr_component_name_t)"",
        rs_log_attr_p->number, sec_attr, &rgy_obj_info,
	&sch_list, &xref, &failure_index, st_p);

    if(BAD_STATUS(st_p)) {
	return;
    }
    
    rsdb_attr_update(&rgy_obj_info, rs_log_attr_p->number,
	    sec_attr, sch_list, xref, &failure_index, st_p);

    if(sch_list) {
        /* free internal ptrs of each schema_entry allocated */
        for(i = 0; i < sch_list->count; i++) {
            sec_attr_util_sch_ent_free_ptrs(&(sch_list->entries[i].sch_entry));
        }
        free(sch_list);
    }
    if(xref)
        free(xref);
}

PRIVATE void
rs_log_attr_prop_update
(
    handle_t             	handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st_p
)
{
    signed32                    i, j;
    Pointer 		 	bufp;
    sec_attr_t		 	*sec_attr_p;
    unsigned_char_t	 	*p;
    rs_log_attr_t 	        *rs_log_attr_p =
					(rs_log_attr_t *)info;
    rs_prop_attr_data_t		prop_attrs[1];
    sec_attr_component_name_t	component_name;
    long			attr_inst_len;

    CLEAR_STATUS(st_p);
    rpc_ss_enable_allocate();

    
    if (!(prop_attrs[0].attr_list = (rs_prop_attr_list_t *)
	    rpc_ss_allocate((sizeof(rs_prop_attr_list_t)) * 
		rs_log_attr_p->number)))  {
            SET_STATUS(st_p, sec_attr_no_memory);
            return;
    }
    prop_attrs[0].attr_list->num_attrs = rs_log_attr_p->number;

    bufp = (Pointer) (rs_log_attr_p)->buf;
    for( i = 0; i < prop_attrs[0].attr_list->num_attrs; i++) {
	sec_attr_p = &(prop_attrs[0].attr_list->attrs[i]);

	RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_ENCODING(sec_attr_p), bufp,
		sizeof(SA_ATTR_ENCODING(sec_attr_p)));

	RSDB_ATTR_COPY_ADV_SRC(&attr_inst_len, bufp, sizeof(attr_inst_len));

	RSDB_ATTR_COPY_ADV_SRC(&sec_attr_p->attr_id, bufp,
		sizeof(sec_attr_p->attr_id));

	rsdb_attr_export_value(sec_attr_p, rpc_ss_allocate, bufp, st_p);
	bufp += attr_inst_len;

	if (BAD_STATUS(st_p)) {
	    return;
	}
    }

    if (rs_log_attr_p->component_name_len != 0 ) {
	if (!(component_name = (unsigned_char_p_t)
              rpc_ss_allocate(rs_log_attr_p->component_name_len))) {
            SET_STATUS(st_p, sec_attr_no_memory);
            return;
        }
        RSDB_ATTR_COPY_ADV_SRC(prop_attrs[0].component_name, bufp,
		rs_log_attr_p->component_name_len);
	prop_attrs[0].component_name[rs_log_attr_p->component_name_len]
		 = '\0';
    }

    rrs_prop_attr_update(handle, 1, prop_attrs, master_info, propq_only, st_p);

    /*
     * Release any storage allocated by unpickling routines.
     */
    rpc_ss_disable_allocate();


}


PUBLIC void
rs_log_attr_delete
(
    sec_attr_component_name_t	component_name,
    unsigned32			number,
    sec_attr_t			attrs[],
    rs_replica_master_info_t    *master_info 
)
{
    long		size;
    long		*attr_inst_len;
    unsigned32		component_name_len;
    sec_attr_t		*sec_attr_p;
    error_status_t      st, *st_p = &st;
    rs_log_attr_t	*info;
    Pointer 		bufp;
    Pointer 		ptr;
    unsigned_char_t	*p;
    int			i;

    if(!(attr_inst_len = (long *)rpc_ss_allocate(sizeof(long) * number)))
    {
        return;
    }
    p = (unsigned_char_t *)component_name;
    component_name_len = u_strlen(p);

    size = sizeof(rs_log_attr_t);
    size += component_name_len;

    /*
     * Calculate how much space will be required.  
     * Also, save the sizes of each attr_inst_len, so that we don't 
     * have to regenerate the sizes for storage.
     */
    for ( i = 0; i < number; i++) {
	sec_attr_p = &attrs[i];
    	rsdb_attr_calc_size_value(sec_attr_p, &attr_inst_len[i], st_p);
	size += attr_inst_len[i] +
		sizeof(SA_ATTR_ENCODING(sec_attr_p)) +
		sizeof(attr_inst_len[i]) +
		sizeof(uuid_t);
        if (BAD_STATUS(st_p)) {
            return;
        }
    }

    alloc_attr(size, &info, st_p);

    if (BAD_STATUS(st_p))
	return;

    info->number = number;
    info->component_name_len = component_name_len;

    bufp = (Pointer) (info)->buf;
    for( i = 0; i < number; i++) {
	sec_attr_p = &attrs[i];

	RSDB_ATTR_COPY_ADV_TGT(bufp, &SA_ATTR_ENCODING(sec_attr_p),
		sizeof(SA_ATTR_ENCODING(sec_attr_p)));
	RSDB_ATTR_COPY_ADV_TGT(bufp, &attr_inst_len[i], 
		sizeof(attr_inst_len[i]));
	RSDB_ATTR_COPY_ADV_TGT(bufp, &sec_attr_p->attr_id,
		sizeof(sec_attr_p->attr_id));

	rsdb_attr_import_value(sec_attr_p, bufp, st_p);
	bufp += attr_inst_len[i];

	if (BAD_STATUS(st_p)) {
	    return;
	}
    }

    if (component_name_len != 0 ) {
        RSDB_ATTR_COPY_ADV_TGT(bufp, component_name, component_name_len);
    }

    rs_log_setup_log_header(&(info->upd_hdr), size,
	RS_LOG_MODULE_ATTR, rs_log_attr_delete_op);
    WRITE_LOG;
}


PRIVATE void
rs_log_attr_prop_delete
(
    handle_t             	handle,
    rs_log_rec_t                *info,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st_p
)
{
    signed32                    i, j;
    Pointer 		 	bufp;
    sec_attr_t		 	*sec_attr_p;
    unsigned_char_t	 	*p;
    rs_log_attr_t 		*rs_log_attr_p = (rs_log_attr_t *)info;
    rs_prop_attr_data_t		prop_attrs[1];
    sec_attr_component_name_t	component_name;
    long			attr_inst_len;

    CLEAR_STATUS(st_p);
    rpc_ss_enable_allocate();

    if (!(prop_attrs[0].attr_list = (rs_prop_attr_list_t *)
	    rpc_ss_allocate((sizeof(rs_prop_attr_list_t)) * 
		rs_log_attr_p->number)))  {
            SET_STATUS(st_p, sec_attr_no_memory);
            return;
    }
    prop_attrs[0].attr_list->num_attrs = rs_log_attr_p->number;

    bufp = (Pointer) (rs_log_attr_p)->buf;
    for( i = 0; i < prop_attrs[0].attr_list->num_attrs; i++) {
	sec_attr_p = &(prop_attrs[0].attr_list->attrs[i]);

	RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_ENCODING(sec_attr_p), bufp,
		sizeof(SA_ATTR_ENCODING(sec_attr_p)));

	RSDB_ATTR_COPY_ADV_SRC(&attr_inst_len, bufp, sizeof(attr_inst_len));

	RSDB_ATTR_COPY_ADV_SRC(&sec_attr_p->attr_id, bufp,
		sizeof(sec_attr_p->attr_id));

	rsdb_attr_export_value(sec_attr_p, rpc_ss_allocate, bufp, st_p);
	bufp += attr_inst_len;

	if (BAD_STATUS(st_p)) {
	    return;
	}
    }

    if (rs_log_attr_p->component_name_len != 0 ) {
	if (!(component_name = (unsigned_char_p_t)
              rpc_ss_allocate(rs_log_attr_p->component_name_len))) {
            SET_STATUS(st_p, sec_attr_no_memory);
            return;
        }
        RSDB_ATTR_COPY_ADV_SRC(prop_attrs[0].component_name, bufp,
		rs_log_attr_p->component_name_len);
	prop_attrs[0].component_name[rs_log_attr_p->component_name_len]
		 = '\0';
    }

    rrs_prop_attr_delete(handle, 1, prop_attrs, master_info, propq_only, st_p);

    /*
     * Release any storage allocated by unpickling routines.
     */
    rpc_ss_disable_allocate();
}

PRIVATE void
rs_log_attr_replay_delete
(
    rs_log_rec_t *info,
    error_status_t *st_p
)
{
    rsdb_util_rgy_obj_info_t    rgy_obj_info;
    uuid_t                      obj_uuid;
    rsdb_attr_sch_list_t        *sch_list = NULL;
    signed32                    *xref = NULL;
    signed32			failure_index;
    signed32                    i, j;

    Pointer 		 	bufp;
    sec_attr_t		 	*sec_attr, *sec_attr_p;
    unsigned_char_t	 	*p;
    rs_log_attr_t *rs_log_attr_p = (rs_log_attr_t *)info;
    sec_attr_component_name_t	component_name;
    long			attr_inst_len;

    CLEAR_STATUS(st_p);

    sec_attr = (sec_attr_t *) rpc_ss_allocate(sizeof(sec_attr_t) *
		rs_log_attr_p->number );


    bufp = (Pointer) (rs_log_attr_p)->buf;
    for( i = 0; i < rs_log_attr_p->number; i++) {
	sec_attr_p = &sec_attr[i];

	RSDB_ATTR_COPY_ADV_SRC(&SA_ATTR_ENCODING(sec_attr_p), bufp,
		sizeof(SA_ATTR_ENCODING(sec_attr_p)));

	RSDB_ATTR_COPY_ADV_SRC(&attr_inst_len, bufp, sizeof(attr_inst_len));

	RSDB_ATTR_COPY_ADV_SRC(&sec_attr_p->attr_id, bufp,
		sizeof(sec_attr_p->attr_id));

	rsdb_attr_export_value(sec_attr_p, rpc_ss_allocate, bufp, st_p);
	bufp += attr_inst_len;

	if (BAD_STATUS(st_p)) {
	    return;
	}
    }

    if (rs_log_attr_p->component_name_len != 0 ) {
	if (!(component_name = (unsigned_char_p_t)
              rpc_ss_allocate(rs_log_attr_p->component_name_len))) {
            SET_STATUS(st_p, sec_attr_no_memory);
            return;
        }
        RSDB_ATTR_COPY_ADV_SRC(component_name, bufp,
		rs_log_attr_p->component_name_len);
	component_name[rs_log_attr_p->component_name_len] = '\0';
    }

    rsdb_util_get_obj_info_by_name(component_name, 
	    rsdb_pgo_type, sec_acl_type_object,
            &rgy_obj_info, &obj_uuid, st_p);

    if(BAD_STATUS(st_p)) {
	return;
    }

    rsdb_attr_sch_get_entries((sec_attr_component_name_t)"",
        rs_log_attr_p->number, sec_attr, &rgy_obj_info,
	&sch_list, &xref, &failure_index, st_p);

    if(BAD_STATUS(st_p)) {
	return;
    }
    
    rsdb_attr_delete(&rgy_obj_info, rs_log_attr_p->number,
	    sec_attr, sch_list, xref, &failure_index, st_p);

    if(sch_list) {
        /* free internal ptrs of each schema_entry allocated */
        for(i = 0; i < sch_list->count; i++) {
            sec_attr_util_sch_ent_free_ptrs(&(sch_list->entries[i].sch_entry));
        }
        free(sch_list);
    }
    if(xref)
        free(xref);
}
