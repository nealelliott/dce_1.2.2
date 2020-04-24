/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: scd_loc.h,v $
 * Revision 1.1.39.2  1996/02/18  23:00:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:10  marty]
 *
 * Revision 1.1.39.1  1995/12/08  17:28:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:09  root]
 * 
 * Revision 1.1.37.2  1994/08/31  19:54:27  rsalz
 * 	Add scd bindings file to oride_base.idl and use it (OT CR 10976).
 * 	[1994/08/31  19:31:33  rsalz]
 * 
 * Revision 1.1.37.1  1994/05/05  20:44:34  hasbrouc
 * 	add group override support (RFC 55)
 * 	[1994/05/05  20:09:13  hasbrouc]
 * 
 * Revision 1.1.35.1  1993/10/05  22:30:34  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:11:14  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  13:27:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:50  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  scd_loc.h V=3 10/29/91 //littl/prgy/src/h
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
**   sec_clientd file location strings
*/

#ifndef _scd_fileloc_h__included_
#define _scd_fileloc_h__included_

#ifndef NULL
#   define NULL 0L
#endif

#ifdef _scd_ALLOCATE_GLOBALS_
#   define EXTERN
#   define INITVAL(X) X
#else
#   define EXTERN extern
#   define INITVAL(X)
#endif

#include <initstring.h>
#include <dce/override.h>

/*
** Global Definitions
*/

#if defined(__STDC__) && !defined(_K_R_PORT_CHECK_)
#   define OVERRIDE_FILE_LOC    DCELOCAL_PATH OVERRIDE_FILE

#else

EXTERN char *override_file_loc INITVAL (= NULL);
#   define OVERRIDE_FILE_LOC \
    STRING_CONCAT(override_file_loc, DCELOCAL_PATH,OVERRIDE_FILE,"")
#endif

#if defined(__STDC__) && !defined(_K_R_PORT_CHECK_)
#   define GROUP_OVERRIDE_FILE_LOC    DCELOCAL_PATH GROUP_OVERRIDE_FILE

#else

EXTERN char *group_override_file_loc INITVAL (= NULL);
#   define GROUP_OVERRIDE_FILE_LOC \
    STRING_CONCAT(group_override_file_loc, DCELOCAL_PATH,GROUP_OVERRIDE_FILE,"")
#endif

#if defined(__STDC__) && !defined(_K_R_PORT_CHECK_)
#   define SEC_CLIENTD_BINDING     DCELOCAL_PATH SCD_BINDINGS_FILE

#else

EXTERN char *scd_binding_path INITVAL (= NULL);
#   define SEC_CLIENTD_BINDING \
    STRING_CONCAT(scd_binding_path, DCELOCAL_PATH,SCD_BINDINGS_FILE,"")
#endif

#endif /* _scd_fileloc_h__included_ */
