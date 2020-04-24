/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_attr_srch.c,v $
 * Revision 1.1.4.2  1996/02/18  00:17:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:48  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/20  20:34 UTC  cuti
 * 	Mothra release
 * 
 * 	HP revision /main/cuti_mothra10_fix/1  1995/07/19  13:42 UTC  cuti
 * 	CHFts15455: Initialize unintialized variables unveil by +O2
 * 	[1995/12/08  17:20:16  root]
 * 
 * Revision 1.1.2.2  1994/06/17  18:42:47  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:15:38  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/02  17:55:43  hanfei
 * 	Initial security drop
 * 	[1994/02/18  20:37:41  hanfei]
 * 
 * $EndLog$
 */
#include <rs_attr_srch.h>
#include <rgymacro.h>

void rs_attr_srch_cursor_t_rundown 
(
    rs_attr_srch_cursor_t	cursor_object
)
{
    return;
}

void rs_attr_srch_cursor_init
(
    handle_t                    h,                   /* [in] */ 
    unsigned32                  num_search_attrs,    /* [in] */ 
    sec_attr_t                  search_attrs[],      /* [in] */ 
    rs_attr_srch_cursor_t       *cursor,             /* [out] */ 
    rs_cache_data_t             *cache_info,         /* [out] */
    error_status_t              *st                  /* [out] */ 
)
{
	SET_STATUS(st, sec_rgy_not_implemented);
}

void rs_attr_srch_cursor_release
(
    handle_t                    h,                   /* [in] */ 
    rs_attr_srch_cursor_t       *cursor,             /* [in, out] */ 
    rs_cache_data_t             *cache_info,         /* [out] */
    error_status_t              *st                  /* [out] */ 
)
{

	SET_STATUS(st, sec_rgy_not_implemented);

}

void rs_attr_srch_names
(
    handle_t                    h,                       /* [in] */ 
    rs_attr_srch_cursor_t       *cursor,                 /* [in, out] */ 
    unsigned32                  num_names_avail,         /* [in] */ 
    unsigned32                  *num_names_returned,     /* [out] */ 
    sec_attr_component_name_t   component_names[],       /* [out] */ 
    unsigned32                  total_component_attrs[], /* [out] */ 
    rs_cache_data_t             *cache_info,             /* [out] */
    error_status_t              *st                      /* [out] */ 
)
{
        *num_names_returned = 0;

	SET_STATUS(st, sec_rgy_not_implemented);

}

void rs_attr_srch_names_attrs
(
    handle_t                    h,                        /* [in] */ 
    rs_attr_srch_cursor_t       *cursor,                  /* [in, out] */ 
    unsigned32                  num_desired_attrs,        /* [in] */ 
    uuid_t                      desired_attrs[],          /* [in] */ 
    unsigned32                  num_names_avail,          /* [in] */ 
    unsigned32                  num_attrs_avail,          /* [in] */ 
    unsigned32                  *num_names_returned,      /* [out] */ 
    sec_attr_component_name_t   component_names[],        /* [out] */ 
    unsigned32                  component_attr_num[],     /* [out] */ 
    unsigned32                  *num_attrs_returned,      /* [out] */ 
    sec_attr_t                  attrs[],                  /* [out] */ 
    rs_cache_data_t             *cache_info,              /* [out] */
    error_status_t              *st                       /* [out] */ 
)
{
    *num_names_returned = 0;
    *num_attrs_returned = 0;
    

    SET_STATUS(st, sec_rgy_not_implemented);

}
