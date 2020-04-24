/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_access_control.h,v $
 * Revision 1.1.4.2  1996/02/18  22:58:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:20  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:24:56  root
 * 	Submit
 * 	[1995/12/11  15:14:34  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:07:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:41  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_access_control.h V=7 04/29/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *     Registry Server - Access Control management
 * 
 */

#ifndef __rs_access_control_included 
#define __rs_access_control_included 

#include <dce/rgynbase.h>
#include <dce/nbase.h>

typedef enum access_mode_t { access_mode_owner, access_mode_user } access_mode_t;

boolean32 is_registry_owner (
#ifdef __STDC__
    handle_t   h,
    sec_rgy_sid_t  *caller
#endif
);

boolean32 is_domain_owner (
#ifdef __STDC__
    handle_t       h,
    sec_rgy_domain_t   domain,
    sec_rgy_sid_t      *caller
#endif
);

boolean32 is_pgo_authorized (
#ifdef __STDC__
    handle_t        h,
    sec_rgy_sid_t       *caller,
    sec_rgy_domain_t    domain,
    sec_rgy_name_t      name,
    access_mode_t   access_mode,
    error_status_t  *(status)
#endif
);

#endif
