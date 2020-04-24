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
 * $Log: eslp.h,v $
 * Revision 1.1.6.1  1996/07/08  18:27:33  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:25 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  22:57:36  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  22:57:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:28  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:40:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:52  root]
 * 
 * Revision 1.1.2.2  1994/03/03  18:58:21  luan
 * 	CR 9773: Make consistent prior-inclusion checks in header files.
 * 	[1994/03/03  18:57:39  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:07  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:20  zeliff]
 * 
 * $EndLog$
 */

#ifndef _ESLP_H
#define _ESLP_H

#include <dce/uuid.h>
#include <dce/nbase.h>
#include <dce/audit_control.h>

/*
 * One guide is used for one combination of 
 * "audit condition" and * "audit action".  
 */

#define HASH_TABLE_SIZE 100
#define FOREIGN_TYPE(x) \
		x == aud_e_esl_foreign_princ || \
		x == aud_e_esl_foreign_group 

#define WORLD_TYPE(x) \
		x == aud_e_esl_world || \
		x == aud_e_esl_world_overridable

#endif /* _ESLP_H */
