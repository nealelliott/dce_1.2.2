/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dts.h,v $
 * Revision 1.1.4.2  1996/02/18  23:10:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:35:49  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:04:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:31  root]
 * 
 * Revision 1.1.2.5  1994/08/23  20:31:44  cbrooks
 * 	Code Cleanup
 * 	[1994/08/23  14:00:50  cbrooks]
 * 
 * Revision 1.1.2.4  1994/08/05  20:47:34  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:32  cbrooks]
 * 
 * Revision 1.1.2.3  1994/06/30  19:45:20  cbrooks
 * 	CR 11112 - new SAMS messages
 * 	[1994/06/28  20:45:35  cbrooks]
 * 
 * Revision 1.1.2.2  1994/05/16  18:51:39  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:10  rhw]
 * 
 * Revision 1.1.2.1  1994/05/12  15:16:44  rhw
 * 	Initial revision, part of code cleanup.
 * 	[1994/05/09  20:11:39  rhw]
 * 
 * $EndLog$
 */

/*
 * 	Module: DTS.H
 *
 *	component-wide header file, as specified by code cleanup
 *	RFC. The platform-independant version of this file will reside
 *	in time/common/POSIX. With time, platform-specific versions
 *	will be added to platform-specific directories. The contents of 
 *	dtss_includes.h will migrate to this file over time.
 */

#ifndef _DTS_H
#define _DTS_H 1

/*
 */

#include <dce/dce.h>
#include <dce/dce_error.h>

#if defined(DCE_DTS_DEBUG) && !defined(DCE_DEBUG)
#  define DCE_DEBUG
#endif                     /* defined(DCE_DTS_DEBUG) */

#ifdef DCE_DEBUG
#include <dcedtssvc.h>
#endif	/* #ifdef DCE_DEBUG */

char * dce_dts_error_text( error_status_t, dce_error_string_t) ;

boolean32 dts_dtsd_auth_function( rpc_binding_handle_t, 
					unsigned32, error_status_t *) ;


#endif  /* #ifndef _DTS_H */
