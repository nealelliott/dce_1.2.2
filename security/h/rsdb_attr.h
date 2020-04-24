/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_attr.h,v $
 * Revision 1.1.6.1  1996/10/03  14:51:11  arvind
 * 	add lookup1_by_uuid to allow "easy" fetches by
 * 	attribute UUID.
 * 	[1996/09/16  23:41 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.4.2  1996/02/18  22:59:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:28  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:27:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:42  root]
 * 
 * Revision 1.1.2.4  1994/08/08  14:23:30  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/04  19:23:29  mdf]
 * 
 * Revision 1.1.2.3  1994/06/17  18:42:04  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:41  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/11  19:06:35  ahop
 * 	hp_sec_to_osf_2 drop
 * 	 Support for logging attribute updates, added rsdb_attr_export_value,
 * 	 handle BND_SVRNAME, better binding checking
 * 	 change import_info to import_value, change calc_size_db_inst to calc_size_value
 * 	 add rsdb_attr_import_info, support for get_effective
 * 	[1994/04/29  20:49:15  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	 Support for logging attribute updates, added rsdb_attr_export_value,
 * 	 handle BND_SVRNAME, better binding checking
 * 	 change import_info to import_value, change calc_size_db_inst to calc_size_value
 * 	 add rsdb_attr_import_info, support for get_effective
 * 
 * 	hp_sec_to_osf_2 drop
 * 	 Support for logging attribute updates, added rsdb_attr_export_value,
 * 	 handle BND_SVRNAME, better binding checking
 * 	change import_info to import_value, change calc_size_db_inst to calc_size_value
 * 	 add rsdb_attr_import_info, support for get_effective
 * 
 * $EndLog$
 */
/*
 *
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 *
 */
/*
 *      Registry Database - attribute functions
 */

#ifndef rsdb_attr_h__included
#define rsdb_attr_h__included

/*
 *Include base datatype definitions
 */

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rs_attr_base.h>

#include <rsdb_pvt.h>
#include <rsdb_util.h>
#include <rsdb_attr_pvt.h>

/*
* Prototypes - public routines
*/

void rsdb_attr_calc_size_value (
    sec_attr_t                  *sec_attr_p,    /* [in] */
    signed32                    *size,          /* [out] */
    error_status_t              *st_p
);

void rsdb_attr_seq_list_create(
    rsdb_pvt_id_t               attr_type_id,    /* [in] */
    error_status_t              *st_p
);

void rsdb_attr_seq_list_delete(
    rsdb_pvt_id_t               attr_type_id	/* [in] */
);

void rsdb_attr_store_by_id(
    rsdb_pvt_id_t               attr_id,
    rsdb_attr_inst_t            *attr
);

boolean32 rsdb_attr_get_by_id(
    rsdb_pvt_id_t               attr_id,
    rsdb_attr_inst_t            **attr,         /* output */
    error_status_t              *st_p
);

boolean32 rsdb_attr_get_by_id_and_export(
    rsdb_pvt_id_t               attr_inst_id,           	/* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    sec_attr_t                  *sec_attr_p,            	/* [out] */
    error_status_t              *st_p
);

boolean32 rsdb_attr_list_get_by_id(
    rsdb_pvt_id_t               attr_list_id,   /* [in] */
    rsdb_max_attr_list_t        *attr_list
);

boolean32 rsdb_attr_list_is_member(
    rsdb_pvt_id_t      attr_list_id,
    rsdb_pvt_id_t      member,
    rsdb_pvt_id_t      *free_list_id
);

boolean32 rsdb_attr_list_get_inst_by_type(
    rsdb_pvt_id_t       attr_type_id,           /* [in] */
    rsdb_pvt_id_t       *attr_list_id,          /* [in, out] */
    signed32            *index,                 /* [out] */
    rsdb_pvt_id_t       *attr_inst_id           /* [out] */
);

void rsdb_attr_list_delete(
    rsdb_pvt_id_t               attr_list_head  /* [in] */
);

void rsdb_attr_store_new(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,  /* [in] */
    rsdb_attr_inst_t            *attr_p,        /* [in] */
    rsdb_pvt_id_t               *attr_inst_id,  /* [out] */
    error_status_t              *st_p
);

void rsdb_attr_import(
    sec_attr_t          *sec_attr_p,
    rsdb_attr_inst_t    **db_attr_p,
    error_status_t      *st_p
);

void rsdb_attr_import_value(
    sec_attr_t          *sec_attr_p,
    Pointer             bufp,
    error_status_t      *st_p
);

void rsdb_attr_export(
    rsdb_attr_inst_t            *db_attr_p,                     /* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    sec_attr_t                  *sec_attr_p,    		/* [out] */
    error_status_t              *st_p
);

void rsdb_attr_export_value(
    sec_attr_t                  *sec_attr_p,    		/* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    Pointer            		bufp,				/* [out] */
    error_status_t              *st_p
);

void rsdb_attr_cursor_init(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,          /* [in] */
    unsigned32                  *cur_num_attrs,         /* [out] */
    rs_attr_cursor_t            *cursor,                /* [out] */
    error_status_t              *st_p
);

void rsdb_attr_cursor_check(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,          /* [in] */
    rs_attr_cursor_t            *cursor,                /* [in, out] */
    error_status_t              *st_p
);

void rsdb_attr_semantic_check (
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,          /* [in] */
    unsigned32                  num_attrs,              /* [in] */
    sec_attr_t                  attrs[],                /* [in] */
    rsdb_attr_sch_list_t        *sch_list,              /* [in] */
    rsdb_attr_sch_attr_xref_t   xref,                   /* [in] */
    signed32                    *failure_index,         /* [out] */
    error_status_t              *st_p
);

void rsdb_attr_update (
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,          /* [in] */
    unsigned32                  num_attrs,              /* [in] */
    sec_attr_t                  attrs[],                /* [in] */
    rsdb_attr_sch_list_t        *sch_list,              /* [in] */
    rsdb_attr_sch_attr_xref_t   xref,                   /* [in] */
    signed32                    *failure_index,         /* [out] */
    error_status_t              *st_p
);

void rsdb_attr_delete (
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,          /* [in] */
    unsigned32                  num_attrs,              /* [in] */
    sec_attr_t                  attrs[],                /* [in] */
    rsdb_attr_sch_list_t        *sch_list,              /* [in] */
    rsdb_attr_sch_attr_xref_t   xref,                   /* [in] */
    signed32                    *failure_index,         /* [out] */
    error_status_t              *st_p
);

void rsdb_attr_delete_type_from_obj(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,  /* [in] */
    rsdb_pvt_id_t               attr_type_id,   /* [in] */
    signed32                    *num_deleted,   /* [out] */
    error_status_t              *st_p
);

void rsdb_attr_lookup_by_uuid(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,  /* [in] */
    rs_attr_cursor_t            *cursor,        /* [in, out] */
    unsigned32                  space_avail,    /* [in] */
    rsdb_attr_sch_list_t        *sch_list,      /* [in] */
    void                        *(*allocator)(unsigned32),      /* [in] */
    unsigned32                  *num_returned,  /* [out] */
    sec_attr_t                  attrs[],        /* [out] */
    unsigned32                  *num_left,      /* [out] */
    error_status_t              *st_p
);

void rsdb_attr_get_effective(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,  /* [in] */
    rsdb_attr_sch_list_t        *sch_list,      /* [in] */
    void                        *(*allocator)(unsigned32),      /* [in] */
    unsigned32                  *num_returned,  /* [out] */
    sec_attr_t                  **attrs,        /* [out] */
    error_status_t              *st_p
);

void rsdb_attr_lookup_by_name(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,          	/* [in] */
    char                        *attr_name,             	/* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    sec_attr_t                  *sec_attr_p,            	/* [out] */
    error_status_t              *st_p
);

void rsdb_attr_lookup1_by_uuid(
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,          	/* [in] */
    uuid_t                      *attr_name,             	/* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    sec_attr_t                  *sec_attr_p,            	/* [out] */
    error_status_t              *st_p
);


#endif /* rsdb_attr_h__included */

