/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: error_statusp.h,v $
 * Revision 1.1.6.1  1996/07/08  18:27:12  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:25 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  22:57:34  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  22:57:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:27  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:39:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:50  root]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:05  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:18  zeliff]
 * 
 * $EndLog$
 */

#ifndef _ERROR_STATUSP_H
#define _ERROR_STATUSP_H

#include <dce/dceaudmsg.h>

#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == aud_s_ok)
#define BAD_STATUS(stp)           ((*stp) != aud_s_ok)
#define CLEAR_STATUS(stp)         (*stp) = aud_s_ok

#endif /* _ERROR_STATUSP_H */
