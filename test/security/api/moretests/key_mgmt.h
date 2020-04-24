/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: key_mgmt.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:21  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:48:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:19  root]
 * 
 * Revision 1.1.2.2  1992/12/31  17:29:00  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:54:33  htf]
 * 
 * Revision 1.1  1992/01/19  04:08:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* 
*/ 
/* 
 * Copyright 1991 by Apollo Computer, Inc.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 * 
 *
 *   
 *	Extracted	1/4/92 17:13:27
 *	from delta on	12/10/91 16:53:43
 *
 *  $Log: key_mgmt.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:21  marty]
 *
 * Revision 1.1.5.2  1996/02/18  22:28:21  marty
 * 	Update OSF copyright years
 *
 * Revision 1.1.4.1  1995/12/11  21:48:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:19  root]
 *
 * Revision 1.1.3.2  1995/12/11  20:52:19  root
 * 	Submit OSF/DCE 1.2.1
 *
 * Revision 1.1.2.2  1992/12/31  17:29:00  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:54:33  htf]
 *
 * Revision 1.1.1.2  1992/12/31  16:54:33  htf
 * 	Embedding copyright notice
 *
 * Revision 1.1  1992/01/19  04:08:38  devrcs
 * Initial revision
 *
 *
 *
*/

#include "rgymacro.h"

#define KEY_MGMT_GET_KEY                 100
#define KEY_MGMT_SET_KEY                 200
#define KEY_MGMT_CHANGE_KEY              300
#define KEY_MGMT_GET_NTH_KEY             400
#define KEY_MGMT_DELETE_KEY              500
#define KEY_MGMT_GEN_RAND_KEY            600
#define KEY_MGMT_MANAGE_KEY              700
#define KEY_MGMT_FREE_KEY                800
#define KEY_MGMT_GARBAGE_COLLECT         900
#define KEY_MGMT_DELETE_KEY_TYPE        1000
#define KEY_MGMT_INITIALIZE_CURSOR      1100
#define KEY_MGMT_GET_NEXT_KEY           1300
#define KEY_MGMT_RELEASE_CURSOR         1400
#define KEY_MGMT_GET_NEXT_KVNO          1500
#define KEY_MGMT_GEN_RAND_NO_SEED_KEY   1600


#define CREATE_PRINC        1
#define NO_CREATE_PRINC     0

