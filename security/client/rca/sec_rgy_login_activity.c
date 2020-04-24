/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_rgy_login_activity.c,v $
 * Revision 1.1.6.2  1996/02/18  00:07:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:48  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:23:59  root
 * 	Submit
 * 	[1995/12/11  15:13:51  root]
 * 
 * Revision 1.1.2.3  1994/08/29  15:51:27  hondo
 * 	OT 11919 --login activity idl change to support different address types
 * 	[1994/08/28  21:46:41  hondo]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:43  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:01  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  14:58:56  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:15:34  mdf]
 * 
 * 	/main/ODESSA_2/1  1994/06/29  21:25 UTC  hondo
 * 	invalid login tracking
 * 
 * 	/main/hondo_il3/1  1994/06/29  16:19 UTC  hondo
 * 	login activity tracking.
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
**
**
*/

#include <rca_pvt.h>	/* must be first cause it includes pbase.h */
#include <rgymacro.h>

#include <dce/sec_rgy_login_activity.h>
#include <rs_login_activity.h>

#include <sysdep.h>

#include <un_strng.h>
#include <bstring.h>


#define Crs_login_activity_lookup(CH,b,c,d)\
    (*rs_login_activity_v1_0_c_epv.rs_login_activity_lookup)\
    (HANDLE(CH),b,c,d)

#define Crs_login_activity_update(CH,b,c,d,e)\
    (*rs_login_activity_v1_0_c_epv.rs_login_activity_update)\
    (HANDLE(CH),b,c,d,e)


PUBLIC void sec_rgy_login_activity_update
(
sec_rgy_handle_t             context,             /* [in] */ 
sec_rgy_login_name_t         *login_name,         /* [in] */ 
sec_rgy_login_activity_t     *login_part,         /* [in] */ 
uuid_t     		     *client,             /* [in] */ 
error_status_t               *st_p                /* [out] */ 
)
{
    rs_cache_data_t    cache_info;

    SETUP_RETRY(rca_op_write, st_p) {
        Crs_login_activity_update(context, login_name, login_part, client, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_login_activity_lookup
(
    sec_rgy_handle_t                context,           /* [in] */ 
    sec_rgy_login_name_t            *login_name,        /* [in] */ 
    sec_rgy_login_activity_t        *login_part,       /* [out] */ 
    error_status_t                  *st_p              /* [out] */ 
)
{
    rs_cache_data_t    cache_info;

    SETUP_RETRY(rca_op_read, st_p) {
        Crs_login_activity_lookup(context, login_name, login_part, st_p);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

