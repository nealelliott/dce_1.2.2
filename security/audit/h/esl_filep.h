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
 * $Log: esl_filep.h,v $
 * Revision 1.1.6.1  1996/07/08  18:27:23  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:25 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  22:57:35  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  22:57:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:27  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:39:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:51  root]
 * 
 * Revision 1.1.2.3  1994/03/03  18:58:21  luan
 * 	CR 9773: Make consistent prior-inclusion checks in header files.
 * 	[1994/03/03  18:57:23  luan]
 * 
 * 	CR 9773: Make consistent prior-inclusion checks in header files.
 * 
 * Revision 1.1.2.2  1994/01/25  04:59:38  luan
 * 	CR 9745 9746: Added new file name arrays
 * 	for dynamic file-name generation.
 * 	[1994/01/25  04:25:21  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:06  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:19  zeliff]
 * 
 * $EndLog$
 */

#ifndef _ESL_FILEP_H
#define _ESL_FILEP_H

#define ESL_UPDATE_ADD_FILTER		0
#define ESL_UPDATE_REMOVE_FILTER	1
#define ESL_UPDATE_DELETE_FILTER	2
#define ESL_UPDATE_FILE_SIZE_LIMIT	512
#define FILE_NAME_SIZE_LIMIT            256

extern char *aud_g_esl_update_fname[];
extern char *aud_g_esl_update_full_fname[];
extern char *aud_g_esl_fname[];
extern char *aud_g_esl_full_fname[];

#endif /*  _ESL_FILEP_H */
