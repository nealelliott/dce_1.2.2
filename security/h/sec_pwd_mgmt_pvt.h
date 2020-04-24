/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_pwd_mgmt_pvt.h,v $
 * Revision 1.1.4.2  1996/02/18  23:01:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:40  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:29:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:25  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:49  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:06  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  15:00:02  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:48:56  mdf]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */
/* 
 * sec_pwd_mgmt_pvt.h
 *
 * Private/internal structures used by sec_pwd_mgmt
 * routines.
 */

#ifndef __sec_pwd_mgmt_pvt_h_included__
#define __sec_pwd_mgmt_pvt_h_included__

#include <dce/rgynbase.h>
#include <rs_pwd_mgmt.h>
#include <dce/sec_attr_base.h>
#include <dce/sec_login.h>

/*
 * currently supported password validation types
 */
enum sec_pwd_mgmt_val_type_t {
    none,
    user_select,
    user_can_select,
    generation_required
};


/* s e c _ p w d _ m g m t _ i n f o _ t
 *
 * This structure holds the information necessary for
 * generating passwords.
 */
typedef struct {
    sec_rgy_login_name_t  login_name;
    sec_login_handle_t    your_lc;
    sec_attr_bind_info_t  *pwd_mgmt_bind_info;
    signed32              pwd_val_type;
    rs_pwd_mgmt_plcy_t    *plcy_p;
} sec_pwd_mgmt_info_t;

#endif
