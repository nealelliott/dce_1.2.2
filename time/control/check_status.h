/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY 
 * $Log: check_status.h,v $
 * Revision 1.1.4.2  1996/02/18  23:12:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:09:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:16  root]
 * 
 * Revision 1.1.2.3  1994/08/23  20:32:09  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:43  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:29:12  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/18  18:03:14  cbrooks]
 * 
 * Revision 1.1.2.2  1994/06/10  20:51:55  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:45  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/21  22:05:26  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:46:59  cbrooks]
 * 
 * $EndLog$
 */

#ifndef _CHECK_STATUS_H 
#define _CHECK_STATUS_H 1

#include <dce/dce.h> 


enum {
   dts_e_dtscp_resume=0,
   dts_e_dtscp_abort=1
};

void dts_dtscp_check_status(const unsigned32, const char *, const int ) ;

#endif /* _CHECK_STATUS_H */

