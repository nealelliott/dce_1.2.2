/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: passwd_export.h,v $
 * Revision 1.1.8.2  1996/03/11  13:26:17  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:19  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:56:20  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:21 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  19:49  mdf
 * 	merge
 * 	[1995/12/08  16:28:51  root]
 * 
 * Revision 1.1.8.3  1993/12/22  21:15:26  hanfei
 * 	change temp_passwd file to be ptmp
 * 	[1993/12/22  21:05:16  hanfei]
 * 
 * Revision 1.1.8.2  1993/07/08  10:50:20  root
 * 	Initial King Kong branch
 * 	[1993/07/08  10:49:48  root]
 * 
 * Revision 1.1.6.2  1993/03/30  20:21:56  ahop
 * 	Merge in HP changes: add location for sys.group file
 * 	[1993/03/30  20:17:16  ahop]
 * 
 * Revision 1.1.1.8  1993/03/11  19:41:47  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.6.1  1994/08/04  16:12:26  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/28  11:41:01  mdf]
 * 
 * Revision 1.1.4.2  1992/12/29  12:37:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:33:38  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/03  22:28:30  burati
 * 	Added verbose flag
 * 	[1992/04/03  21:31:11  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1993
 * Unpublished work. All Rights Reserved.
 *
 */
/*
 *      Create passwd and group files from registry account information
 */

#ifndef passwd_export_included
#define passwd_export_included

#define MY_NAME  "passwd_export"

#define OPT_DIR         "-d"
#define LIMIT_ENTRIES   "-m"
#define GIVE_HELP       "-help"
#define QUERY_HELP      "?"
#define OMIT_FLAG       "-x"
#define SORT_FLAG       "-s"
#define NO_OVRD_FLAG    "-n"
#define VERBOSE_FLAG    "-v"

#define OMIT_ENTRY_STRING "OMIT"

#define DEFAULT_DIRECTORY  "/etc"

#define DEFAULT_PASSWD_FILE  "passwd"
#define DEFAULT_GROUP_FILE  "group"
#define SYS_GROUP_FILE "/opt/dcelocal/etc/sys.group"

#define TEMP_PASSWD_FILE  "ptmp"
#define SORT_PASSWD_FILE  "sort_passwd"
#define TEMP_GROUP_FILE  "temp_group"
#define TEMP_FILE  "temp_xxx"

#define PASSWD_BAK  "passwd.bak"
#define GROUP_BAK  "group.bak"
#define ORG_BAK  "org.bak"

#define MAX_MEMBERS 50
#define MAX_PWD_BUF 1024
#define MAX_ENTRIES 1000000
#define MAX_PATHLEN 5000
#define MAX_COLUMN 76
#define FILE_MARGIN 4
#define MAX_PWD_COUNT  10000
#define NAME_SPACE_ALLOC  (MAX_PWD_COUNT * 10)

#endif /* passwd_export_included */
