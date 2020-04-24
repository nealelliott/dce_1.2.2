/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_rgy_attr_srch.c,v $
 * Revision 1.1.8.1  1996/10/03  14:48:42  arvind
 * 	Fix types at interface.
 * 	[1996/09/16  23:37 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.6.2  1996/02/18  00:07:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:45  marty]
 * 
 * Revision 1.1.6.1  1995/12/13  16:23:58  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/20  20:27 UTC  cuti
 * 	Created for Mothra, Mothra10
 * 
 * 	HP revision /main/cuti_mothra10_fix/1  1995/07/19  13:42 UTC  cuti
 * 	CHFts15455: Initialize unintialized variables unveil by +O2
 * 	[1995/12/11  15:13:50  root]
 * 
 * Revision 1.1.2.2  1994/06/17  18:41:49  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:26  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/02  17:54:44  hanfei
 * 	Change comp_names[] to *comp_names
 * 	[1994/02/25  20:55:06  hanfei]
 * 
 * 	Initial ERA drop
 * 	[1994/02/18  19:08:43  hanfei]
 * 
 * 	Inital ERA drop
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1993, 1994
** Unpublished work. All Rights Reserved.
**
**      Attributes Search Interface
**
**
*/

#include <rca_pvt.h>
#include <rgymacro.h>

#include <dce/sec_rgy_attr_srch.h>
#include <rs_attr_srch.h>

#include <sysdep.h>
#include <rca_pwd.h>

#include <un_strng.h>
#include <bstring.h>

#define Crs_attr_srch_cursor_init(CH,b,c,d,CI,e)\
        (*rs_attr_srch_v0_0_c_epv.rs_attr_srch_cursor_init)\
	(HANDLE(CH),b,c,d,CI,e)
#define Crs_attr_srch_cursor_release(CH,b,CI,c)\
        (*rs_attr_srch_v0_0_c_epv.rs_attr_srch_cursor_release)\
	(HANDLE(CH),b,CI,c)
#define Crs_attr_srch_names(CH,b,c,d,e,f,CI,g)\
        (*rs_attr_srch_v0_0_c_epv.rs_attr_srch_names)\
	(HANDLE(CH),b,c,d,e,f,CI,g)
#define Crs_attr_srch_names_attrs(CH,b,c,d,e,f,g,h,i,j,k,CI,l)\
        (*rs_attr_srch_v0_0_c_epv.rs_attr_srch_names_attrs)\
        (HANDLE(CH),b,c,d,e,f,g,h,i,j,k,CI,l)

void sec_rgy_attr_srch_cursor_init
(
	/* [in] */ sec_rgy_handle_t		context,
	/* [in] */ sec_rgy_domain_t		name_domain,
	/* [in] */ unsigned32			num_search_attrs,
	/* [in] */ sec_attr_t			search_attrs[],
	/* [out] */ sec_attr_srch_cursor_t	*cursor,
	/* [out] */ error_status_t		*st_p
)
{
    rs_cache_data_t        cache_info;

    CLEAR_STATUS(st_p);

    /* allocate a cursor */
    if(!(*cursor = (sec_attr_srch_cursor_t)malloc(sizeof(rs_attr_srch_cursor_t)))) {
         SET_STATUS(st_p, sec_attr_no_memory);
        return;
    }


    SETUP_RETRY(rca_op_read, st_p) {
	Crs_attr_srch_cursor_init(context, num_search_attrs, 
		search_attrs, (rs_attr_srch_cursor_t *)(*cursor), &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

void sec_rgy_attr_srch_cursor_rel
(
	/* [in] */ sec_rgy_handle_t		context,
	/* [in, out] */ sec_attr_srch_cursor_t	*cursor,
	/* [out] */ error_status_t		*st_p
)
{
    rs_cache_data_t        cache_info;

    SETUP_RETRY(rca_op_read, st_p) {
	Crs_attr_srch_cursor_release(context,  (rs_attr_srch_cursor_t *)(*cursor), &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

void sec_rgy_attr_srch_names
(
	/* [in] */ sec_rgy_handle_t		context,
	/* [in, out] */ sec_attr_srch_cursor_t	*cursor,
	/* [in] */ unsigned32			num_names_avail,
	/* [out] */ unsigned32			*num_names_returned,
	/* [out] */ sec_attr_component_name_t   *names,
	/* [out] */ unsigned32			total_component_attrs[],
	/* [out] */ error_status_t		*st_p
)
{
    rs_cache_data_t        cache_info;

    SETUP_RETRY(rca_op_read, st_p) {
	Crs_attr_srch_names(context, (rs_attr_srch_cursor_t *)(*cursor), num_names_avail, 
		num_names_returned, names, total_component_attrs, 
		&cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

void sec_rgy_attr_srch_names_attrs
(
	/* [in] */ sec_rgy_handle_t		context,
	/* [in, out] */ sec_attr_srch_cursor_t	*cursor,
	/* [in] */ unsigned32			num_desired_attrs,
	/* [in] */ uuid_t			desired_attrs[],
	/* [in] */ unsigned32			num_names_avail,
	/* [in] */ unsigned32			num_attrs_avail,
	/* [out] */ unsigned32			*num_names_returned,
	/* [out] */ sec_attr_component_name_t   *names,
	/* [out] */ unsigned32			component_attr_num[],
	/* [out] */ unsigned32			*num_attrs_returned,
	/* [out] */ sec_attr_t			attrs[],
	/* [out] */ error_status_t		*st_p
)
{
    rs_cache_data_t        cache_info;

    SETUP_RETRY(rca_op_read, st_p) {
	Crs_attr_srch_names_attrs(context, cursor, num_desired_attrs, 
		desired_attrs, num_names_avail, num_attrs_avail, 
		num_names_returned, names, component_attr_num,
		num_attrs_returned, attrs, &cache_info, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}
