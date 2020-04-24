/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_rgy_attr_sch.c,v $
 * Revision 1.1.8.1  1996/10/03  14:48:28  arvind
 * 	Silence compiler warnings
 * 	[1996/09/16  23:37 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.6.2  1996/02/18  00:07:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:42  marty]
 * 
 * Revision 1.1.6.1  1995/12/13  16:23:57  root
 * 	Submit
 * 	[1995/12/11  15:13:49  root]
 * 
 * Revision 1.1.2.4  1994/08/04  16:12:41  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:09:19  mdf]
 * 
 * Revision 1.1.2.2  1994/06/17  18:41:48  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:24  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/02  17:54:43  hanfei
 * 	Initial ERA drop
 * 	[1994/02/18  19:08:42  hanfei]
 * 
 * 	Inital ERA drop
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1993, 1994
** Unpublished work. All Rights Reserved.
**
**      Attributes Manipulation Interface
**
**
*/

#include <rca_pvt.h>	/* must be first cause it includes pbase.h */
#include <rgymacro.h>

#include <dce/sec_rgy_attr_sch.h>
#include <rs_attr_schema.h>

#include <sysdep.h>
#include <rca_pwd.h>

#include <un_strng.h>
#include <bstring.h>

#define Crs_attr_schema_create_entry(CH,b,c,CI,d)\
    (*rs_attr_schema_v0_0_c_epv.rs_attr_schema_create_entry)\
    (HANDLE(CH),b,c,CI,d)

#define Crs_attr_schema_delete_entry(CH,b,c,CI,d)\
    (*rs_attr_schema_v0_0_c_epv.rs_attr_schema_delete_entry)\
    (HANDLE(CH),b,c,CI,d)

#define Crs_attr_schema_update_entry(CH,b,c,d,CI,e)\
    (*rs_attr_schema_v0_0_c_epv.rs_attr_schema_update_entry)\
    (HANDLE(CH),b,c,d,CI,e)

#define Crs_attr_schema_cursor_init(CH,b,c,d,CI,e)\
    (*rs_attr_schema_v0_0_c_epv.rs_attr_schema_cursor_init)\
    (HANDLE(CH),b,c,d,CI,e)

#define Crs_attr_schema_scan(CH,b,c,d,e,f,CI,g)\
    (*rs_attr_schema_v0_0_c_epv.rs_attr_schema_scan)\
    (HANDLE(CH),b,c,d,e,f,CI,g)

#define Crs_attr_schema_lookup_by_name(CH,b,c,d,CI,e)\
    (*rs_attr_schema_v0_0_c_epv.rs_attr_schema_lookup_by_name)\
    (HANDLE(CH),b,c,d,CI,e)

#define Crs_attr_schema_lookup_by_id(CH,b,c,d,CI,e)\
    (*rs_attr_schema_v0_0_c_epv.rs_attr_schema_lookup_by_id)\
    (HANDLE(CH),b,c,d,CI,e)

#define Crs_attr_schema_get_acl_mgrs(CH,b,c,d,e,f,CI,g)\
    (*rs_attr_schema_v0_0_c_epv.rs_attr_schema_get_acl_mgrs)\
    (HANDLE(CH),b,c,d,e,f,CI,g)

#define Crs_attr_schema_aclmgr_strings(CH,b,c,d,e,f,g,h,i,j,CI,k)\
    (*rs_attr_schema_v0_0_c_epv.rs_attr_schema_aclmgr_strings)\
    (HANDLE(CH),b,c,d,e,f,g,h,i,j,CI,k)

PUBLIC void sec_rgy_attr_sch_create_entry
(
    sec_rgy_handle_t                    context,        /* [in] */ 
    sec_attr_component_name_t           schema_name,    /* [in] */ 
    sec_attr_schema_entry_t             *schema_entry,  /* [in] */ 
    error_status_t                      *st_p           /* [out] */ 
)
{
    rs_cache_data_t    cache_info;

    SETUP_RETRY(rca_op_write, st_p) {
        Crs_attr_schema_create_entry(context, schema_name, schema_entry, 
                                     &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_attr_sch_delete_entry
(
    sec_rgy_handle_t                context,            /* [in] */
    sec_attr_component_name_t       schema_name,        /* [in] */ 
    uuid_t                          *attr_id,           /* [in] */ 
    error_status_t                  *st_p               /* [out] */ 
)
{
    rs_cache_data_t    cache_info;

    SETUP_RETRY(rca_op_write, st_p) {
        Crs_attr_schema_delete_entry(context, schema_name, attr_id, 
                                     &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_attr_sch_update_entry
(
sec_rgy_handle_t                  context,             /* [in] */ 
sec_attr_component_name_t         schema_name,         /* [in] */ 
sec_attr_schema_entry_parts_t     modify_parts,        /* [in] */ 
sec_attr_schema_entry_t           *schema_entry,       /* [in] */ 
error_status_t                    *st_p                /* [out] */ 
)
{
    rs_cache_data_t    cache_info;

    SETUP_RETRY(rca_op_write, st_p) {
        Crs_attr_schema_update_entry(context, schema_name, modify_parts, 
                                     schema_entry, &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_attr_sch_cursor_init
(
    sec_rgy_handle_t                 context,             /* [in] */ 
    sec_attr_component_name_t        schema_name,         /* [in] */ 
    unsigned32                       *cur_num_entries,    /* [out] */ 
    sec_attr_cursor_t                *cursor,             /* [out] */ 
    error_status_t                   *st_p                /* [out] */ 
)
{
    rs_cache_data_t    cache_info;

    CLEAR_STATUS(st_p);

    /* allocate a cursor */
    if(!(*cursor = (sec_attr_cursor_t)malloc(sizeof(rs_attr_cursor_t)))) {
         SET_STATUS(st_p, sec_attr_no_memory);
        return;
    }

    SETUP_RETRY(rca_op_read, st_p) {
        Crs_attr_schema_cursor_init(context, schema_name, cur_num_entries, 
                                    (rs_attr_cursor_t *)(*cursor), &cache_info, 
                                    st_p);
        NORMAL_CASE {
             CHECK_CACHE(context, &cache_info); 
        }
        RETRY_CASE;
    }
    RETRY_END;
}


PUBLIC void sec_rgy_attr_sch_cursor_alloc
(
    sec_attr_cursor_t	*cursor,	/* [out] */
    error_status_t	*st_p
)
{
    CLEAR_STATUS(st_p);

    /* allocate a cursor */
    if(!(*cursor = (sec_attr_cursor_t)malloc(sizeof(rs_attr_cursor_t)))) {
	SET_STATUS(st_p, sec_attr_no_memory);
	return;
    }
    memset(*cursor, 0, sizeof(rs_attr_cursor_t));
}


PUBLIC void sec_rgy_attr_sch_cursor_reset
(
    sec_attr_cursor_t	*cursor,	/* [in, out] */
    error_status_t	*st_p
)
{
    CLEAR_STATUS(st_p);

    if(!(*cursor)) {
	SET_STATUS(st_p, sec_attr_bad_cursor);
	return;
    }
    memset(*cursor, 0, sizeof(rs_attr_cursor_t));
}


PUBLIC void sec_rgy_attr_sch_cursor_release
(
    sec_attr_cursor_t    *cursor,        /* [in, out] */ 
    error_status_t       *st_p           /* [out] */ 
)
{
    CLEAR_STATUS(st_p);

    if(*cursor) {
        free(*cursor);
        *cursor = NULL;
    }
}

PUBLIC void sec_rgy_attr_sch_scan
(
    sec_rgy_handle_t                context,             /* [in] */
    sec_attr_component_name_t       schema_name,         /* [in] */ 
    sec_attr_cursor_t               *cursor,             /* [in, out] */ 
    unsigned32                      num_to_read,         /* [in] */ 
    unsigned32                      *num_read,           /* [out] */ 
    sec_attr_schema_entry_t         schema_entries[],    /* [out] */ 
    error_status_t                  *st_p                /* [out] */ 
)
{
    rs_cache_data_t    cache_info;

    CLEAR_STATUS(st_p);

    SETUP_RETRY(rca_op_read, st_p) {
        Crs_attr_schema_scan(context, schema_name, (rs_attr_cursor_t *)
                             (*cursor), num_to_read, num_read, schema_entries, 
                             &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_attr_sch_lookup_by_name
(
    sec_rgy_handle_t                context,            /* [in] */ 
    sec_attr_component_name_t       schema_name,        /* [in] */ 
    unsigned char                   *attr_name,	        /* [in] */ 
    sec_attr_schema_entry_t         *schema_entry,      /* [out] */ 
    error_status_t                  *st_p               /* [out] */ 
)
{
    rs_cache_data_t    cache_info;

    SETUP_RETRY(rca_op_read, st_p) {
        Crs_attr_schema_lookup_by_name(context, schema_name, attr_name, 
                                       schema_entry, &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_attr_sch_lookup_by_id
(
    sec_rgy_handle_t             context,              /* [in] */ 
    sec_attr_component_name_t    schema_name,          /* [in] */ 
    uuid_t                       *attr_id,             /* [in] */ 
    sec_attr_schema_entry_t      *schema_entry,        /* [out] */ 
    error_status_t               *st_p                 /* [out] */ 
)
{
    rs_cache_data_t    cache_info;

    SETUP_RETRY(rca_op_read, st_p) {
        Crs_attr_schema_lookup_by_id(context, schema_name, attr_id, 
                                     schema_entry, &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}


PUBLIC void sec_rgy_attr_sch_get_acl_mgrs
(
    sec_rgy_handle_t		context,		/* [in] */
    sec_attr_component_name_t	schema_name,		/* [in] */ 
    unsigned32                  size_avail,		/* [in] */
    unsigned32                  *size_used,		/* [out] */
    unsigned32                  *num_acl_mgr_types,	/* [out] */
    uuid_t                      acl_mgr_types[],	/* [out] */
    error_status_t		*st_p			/* [out] */ 
)
{
    rs_cache_data_t    cache_info;

    CLEAR_STATUS(st_p);

    SETUP_RETRY(rca_op_read, st_p) {
        Crs_attr_schema_get_acl_mgrs(context, schema_name, size_avail,
		size_used, num_acl_mgr_types, acl_mgr_types,
		&cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}


PUBLIC void sec_rgy_attr_sch_aclmgr_strings
(
    sec_rgy_handle_t		context,		/* [in] */
    sec_attr_component_name_t	schema_name,		/* [in] */ 
    uuid_t                      *acl_mgr_type,          /* [in] */
    unsigned32                  size_avail,             /* [in] */
    uuid_t                      *acl_mgr_type_chain,    /* [out] */
    sec_acl_printstring_t       *acl_mgr_info,          /* [out] */
    boolean32                   *tokenize,              /* [out] */
    unsigned32                  *total_num_printstrings, /* [out] */
    unsigned32                  *size_used,		/* [out] */
    sec_acl_printstring_t	permstrings[],		/* [out] */
    error_status_t		*st_p			/* [out] */ 
)
{
    rs_cache_data_t    cache_info;

    CLEAR_STATUS(st_p);

    SETUP_RETRY(rca_op_read, st_p) {
        Crs_attr_schema_aclmgr_strings(context, schema_name, acl_mgr_type,
		size_avail, acl_mgr_type_chain, acl_mgr_info, tokenize,
		total_num_printstrings, size_used, permstrings,
		&cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

