/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rca_fileloc.h,v $
 * Revision 1.1.36.1  1996/07/08  18:33:14  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:50 UTC  psn  /main/DCE122_PK/1]
 *
 * 	Merge bugfixes for dced and sec_pubkey based on code review
 * 	[1996/05/21  13:06 UTC  aha  /main/DCE12_PK/aha_pk3/1]
 *
 * 	Add KDC_PUBKEY_CACHE_DIR_LEN, KDC_PUBKEY_CACHE_FILE, KDC_PUBKEY_CACHE_FILE_LEN
 * 	[14:29:05  14:29:05  aha (Anne Anderson)  1.2  96/05/16]
 *
 * 	Includes KDC_PUBKEY_CACHE_DIR_LEN, etc.
 * 	[1996/05/16  14:56 UTC  aha  /main/aha_pk2/2]
 *
 * 	Add KDC_PUBKEY_CACHE_FILE
 * 	[1996/05/03  12:55 UTC  aha  /main/DCE12_PK/1]
 *
 * 	Additions for Public Key Login
 * 	[1996/05/02  15:55 UTC  aha  /main/aha_pk2/1]
 *
 * 	Additions for public key login: define KDC_PUBKEY_CACHE_DIR
 * 	[1993/10/05  22:30:27  mccann  1.1.32.1]
 *
 * Revision 1.1.34.2  1996/02/18  22:58:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:00  marty]
 * 
 * Revision 1.1.34.1  1995/12/13  16:24:39  root
 * 	Submit
 * 	[1995/12/11  15:14:22  root]
 * 
 * Revision 1.1.32.1  1993/10/05  22:30:27  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:10:59  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  13:07:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:34  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rca_fileloc.h V=3 07/15/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
**   RCA file location strings
*/

#ifndef _rca_fileloc_h__included_
#define _rca_fileloc_h__included_

/* Include rca_fileloc.h after a header file that defines
 * EXTERN and INITVAL (e.g., rca_pvt.h, rsdb_pvt.h).
 */
#include <initstring.h>

#ifndef NULL
#   define NULL 0L
#endif

/*
** Global Variables
*/

/*
 * Define constants for file locations.
 */
#define BOOT_TIME_FILE          "/tmp/pe_boot_time" 
#define KDC_PUBKEY_CACHE_FILE   ".key"
#define KDC_PUBKEY_CACHE_FILE_LEN 4

#if defined(__STDC__) && !defined(_K_R_PORT_CHECK_)
#   define LOCATION_CACHE_FILE     DCELOCAL_PATH "/etc/security/.peloc"
#   define RGY_CONFIG_FILE         DCELOCAL_PATH "/etc/security/pe_site"

/* full path of cache file is KDC_PUBKEY_CACHE_DIR/cell_name/KDC_PUBKEY_CACHE_FILE */
#   define KDC_PUBKEY_CACHE_DIR    DCELOCAL_PATH "/etc/security/kdc_pk_auth/"
#   define KDC_PUBKEY_CACHE_DIR_LEN (strlen(KDC_PUBKEY_CACHE_DIR))

#else

EXTERN char *rca_peloc_path INITVAL (= NULL);
#   define LOCATION_CACHE_FILE \
    STRING_CONCAT(rca_peloc_path, DCELOCAL_PATH,"/etc/security/.peloc","")

EXTERN char *rca_pesite_path INITVAL (= NULL);
#   define RGY_CONFIG_FILE \
    STRING_CONCAT(rca_pesite_path, DCELOCAL_PATH,"/etc/security/pe_site","")

EXTERN char *rca_kdcpubkey_dir INITVAL (= NULL);
#   define KDC_PUBKEY_CACHE_DIR \
    STRING_CONCAT(rca_kdcpubkey_dir, DCELOCAL_PATH, "/etc/security/kdc_pk_auth/","")
#   define KDC_PUBKEY_CACHE_DIR_LEN \
    (strlen(KDC_PUBKEY_CACHE_DIR))

#endif

#endif /* _rca_fileloc_h__included_ */
