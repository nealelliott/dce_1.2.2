/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_attr_schema.h,v $
 * Revision 1.1.4.2  1996/02/18  23:00:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:27:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/10/30  19:39 UTC  psn
 * 	Fix DCE OT 13160, CHFts16737
 * 	[1995/10/30  19:33 UTC  psn  /main/psn_ot13160/1]
 * 
 * Revision 1.1.2.4  1994/08/08  14:23:30  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/04  19:23:39  mdf]
 * 
 * Revision 1.1.2.3  1994/06/17  18:42:05  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:42  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/11  19:07:27  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  20:41:29  ahop]
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
 *      Registry Database - attribute schema functions
 */

#ifndef rsdb_attr_sch_h__included
#define rsdb_attr_sch_h__included

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

void rsdb_attr_sch_calc_size (
    sec_attr_schema_entry_t     *sec_sch_entry_p,       /* [in] */
    unsigned32                  *size,                  /* [out] */
    error_status_t              *st_p
);

void rsdb_attr_sch_create (
    void
);

void rsdb_attr_sch_init (
    void
);

void rsdb_attr_sch_store_by_id(
    rsdb_pvt_id_t               sch_entry_id,
    rsdb_attr_schema_entry_t    *sch_entry_p,
    error_status_t              *st_p
);

boolean32 rsdb_attr_sch_get_id_by_uuid (
        uuid_t                  *attr_id_p,             /* [in] */
        rsdb_pvt_id_t           *sch_entry_id_p,        /* [out] */
        error_status_t          *st_p
);

boolean32 rsdb_attr_sch_get_id_by_name (
        char                    *attr_name,             /* [in] */
        rsdb_pvt_id_t           *sch_entry_id_p,        /* [out] */
        error_status_t          *st_p
);

boolean32 rsdb_attr_sch_get_by_id (
    rsdb_pvt_id_t                   sch_entry_id,	/* [in] */
    rsdb_attr_schema_entry_t        **sch_entry_p,	/* [out] */
    error_status_t                  *st_p
);

boolean32 rsdb_attr_sch_get_export_by_id (
    rsdb_pvt_id_t               sch_entry_id,                   /* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    sec_attr_schema_entry_t     *sec_sch_entry_p,               /* [out] */
    rsdb_pvt_id_t               *next_sch_entry_id,             /* [out] */
    error_status_t              *st_p
);

boolean32 rsdb_attr_sch_get_by_uuid (
        uuid_t                          *attr_id_p,     /* [in] */
        rsdb_attr_schema_entry_t        **sch_entry_p,  /* [out] */
        error_status_t                  *st_p
);

boolean32 rsdb_attr_sch_get_by_name (
        char                            *attr_name,     /* [in] */
        rsdb_attr_schema_entry_t        **sch_entry_p,  /* [out] */
        error_status_t                  *st_p
);

void rsdb_attr_sch_store_new(
    rsdb_attr_schema_entry_t    *sch_entry_p,   /* [in] */
    rsdb_pvt_id_t               *sch_entry_id,  /* [out] */
    error_status_t		*st_p
);

void rsdb_attr_sch_import(
    sec_attr_schema_entry_t     *sec_sch_entry_p,
    rsdb_attr_schema_entry_t    **db_sch_entry_p,
    error_status_t              *st_p
);

void rsdb_attr_sch_import_info(
    sec_attr_schema_entry_t     *sec_sch_entry_p,
    rsdb_attr_schema_entry_t    *db_sch_entry_p,
    error_status_t              *st_p
);

void rsdb_attr_sch_export(
    rsdb_attr_schema_entry_t    *db_sch_entry_p,		/* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    sec_attr_schema_entry_t     *sec_sch_entry_p,       	/* [out] */
    error_status_t              *st_p
);

void rsdb_attr_sch_cursor_init (
    sec_attr_component_name_t	schema_name,		/* [in] */
    unsigned32			*cur_num_entries,	/* [out] */
    rs_attr_cursor_t		*cursor,		/* [out] */
    error_status_t		*st_p
);

void rsdb_attr_sch_cursor_check(
    sec_attr_component_name_t	schema_name,		/* [in] */
    rs_attr_cursor_t            *cursor,                /* [in, out] */
    error_status_t              *st_p
);

void rsdb_attr_sch_add(
    sec_attr_component_name_t   schema_name,            /* [in] */
    sec_attr_schema_entry_t     *sec_sch_entry_p,       /* [in] */
    error_status_t              *st_p
);

void rsdb_attr_sch_delete(
    sec_attr_component_name_t   schema_name,    /* [in] */
    uuid_t                      *attr_id_p,     /* [in] */
    error_status_t  *st_p
);

void rsdb_attr_sch_replace(
    sec_attr_component_name_t           schema_name,            /* [in] */
    sec_attr_schema_entry_t             *sec_sch_entry_p,       /* [in] */
    sec_attr_schema_entry_parts_t       modify_parts,           /* [in] */
    error_status_t                      *st_p
);

void rsdb_attr_sch_lookup_by_uuid(
    sec_attr_component_name_t   schema_name,            /* [in] */
    uuid_t                      *attr_id,               /* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    sec_attr_schema_entry_t     *sec_sch_entry_p,       /* [in, out] */
    error_status_t              *st_p
);

void rsdb_attr_sch_lookup_by_name(
    sec_attr_component_name_t   schema_name,            	/* [in] */
    char                        *attr_name,             	/* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    sec_attr_schema_entry_t     *sec_sch_entry_p,       	/* [out] */
    error_status_t              *st_p
);

void rsdb_attr_sch_get_next(
    sec_attr_component_name_t   schema_name,            /* [in] */
    void                        *(*allocator)(unsigned32),	/* [in] */
    rs_attr_cursor_t            *cursor,                /* [in, out] */
    sec_attr_schema_entry_t     *sec_sch_entry_p,       /* [out] */
    unsigned32                  *num_entries_left,      /* [out] */
    error_status_t              *st_p
);

void rsdb_attr_sch_get_entries(
    sec_attr_component_name_t   schema_name,            /* [in] */
    unsigned32                  num_attrs,              /* [in] */
    sec_attr_t                  attrs[],                /* [in] */
    rsdb_util_rgy_obj_info_t    *rgy_obj_info,          /* [in] */
    rsdb_attr_sch_list_t        **sch_list,             /* [in, out] */
    signed32   			**xref,                  /* [in, out] */
    signed32                    *failure_index,         /* [out] */
    error_status_t              *st_p
);

boolean32 rsdb_attr_sch_type_id_exists(
    uuid_t                      *attr_id                /* [in] */
);

boolean32 rsdb_attr_sch_name_exists(
    char                        *attr_name             /* [in] */
);



#endif /* rsdb_attr_sch_h__included */
