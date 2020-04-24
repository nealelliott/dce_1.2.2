/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
*/
/*
 * HISTORY
 * $Log: fmul_common.h,v $
 * Revision 1.1.5.2  1996/02/18  23:07:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:24  marty]
 *
 * Revision 1.1.5.1  1995/12/11  22:00:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:59:28  root]
 * 
 * Revision 1.1.2.2  1994/09/01  19:37:38  rdl
 * 	Changes to fix problems with RPC where multiple servers on one machine
 * 	[1994/09/01  19:36:47  rdl]
 * 
 * Revision 1.1.2.1  1994/08/24  19:44:55  rdl
 * 	Initial submission for dfs.fmul test.
 * 	[1994/08/24  19:44:01  rdl]
 * 
 * $EndLog$
 */

/* fmul.common.h */

#define LOCK_DENIED 0
#define LOCK_GRANTED 1

#define NAMESERVER_GROUP_NAME "/.:/fmul_group"
#define NAMESERVER_MEMBER_NAME_BEGIN "/.:/fmul_"

#define MAX_SERVERS 20
#define MAX_LINE_LENGTH 256
