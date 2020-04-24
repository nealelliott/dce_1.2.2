/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_pwd_mgmt_pvt.h,v $
 * Revision 1.1.4.2  1996/02/18  22:59:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:27:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:28  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:45  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:03  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  14:59:33  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:59:31  mdf]
 * 
 * $EndLog$
 */
/*
 * rs_pwd_mgmt internal (to rs layer) public interfaces
 */

#ifndef __rs_pwd_mgmt_pvt_h_included__
#define __rs_pwd_mgmt_pvt_h_included__

#include <dce/nbase.h>
#include <dce/rgynbase.h>

/*
 * rs_pwd_mgmt_strength_check determines if a 
 * user should have his password strength checked
 * by the pwd_mgmtd server. If it determines it
 * doesn't, the user's password will still be
 * checked against the effective policy.
 */
void rs_pwd_mgmt_strength_check (
    sec_rgy_login_name_t *login_name,  /* [in] */
    sec_passwd_rec_t     *pwd,         /* [in] */
    error_status_t       *stp          /* [out] */
);

/*
 * rs_pwd_mgmt_check_plain checks a plain text
 * password against the user's effective policy.
 *
 * errors :
 *  sec_rgy_passwd_too_short
 *  sec_rgy_passwd_non_alpha
 *  sec_rgy_passwd_spaces
 */
void rs_pwd_mgmt_check_plain (
    char                 *pwd_text,    /* [in] */
    signed32             pwd_textlen,  /* [in] */
    sec_rgy_plcy_t       rgy_plcy,     /* [in] */
    error_status_t       *stp          /* [out] */
);

#endif
