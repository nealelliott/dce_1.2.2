/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: adver_ncl_defs.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:53  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:14:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:05  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:01:54  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:55:26  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:38:27  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:52  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/19  17:06:04  mccann
 * 	More code cleanup
 * 	[1994/04/19  16:35:50  mccann]
 * 
 * Revision 1.1.4.1  1994/03/12  22:02:03  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:11:15  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:28:02  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:45:13  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _ADVER_NCL_DEFS_H
#define _ADVER_NCL_DEFS_H
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */

/* attribute codes */
#define CDS_CLERK_CREATE_TIME 0         /* Creation time                    */
#define CDS_CLERK_PROTOCOL_ERROR_COUNT 1 /* Protocol error counter          */
#define CDS_CLERK_AUTHENTICATION_COUNT 2 /* Authentication failure counter  */
#define CDS_CLERK_READ_OPS_COUNT 3     /* Cache read ops counter           */
#define CDS_CLERK_CACHE_HITS_COUNT 4   /* Cache hits counter               */
#define CDS_CLERK_CACHE_BYPASS_COUNT 5 /* Cache bypassed counter           */
#define CDS_CLERK_WRITE_OPS_COUNT 6    /* Cache writes counter             */
#define CDS_CLERK_MISC_OPS_COUNT 7     /* Cache misc. operations counter   */

/* Clearinghouse attribute codes   */
#define REMOTE_CH_CREATE_TIME 2         /* Creation Time                    */
#define REMOTE_CH_READ_OPS_COUNT 10     /* C'house read operations counter  */
#define REMOTE_CH_WRITE_OPS_COUNT 13    /* C'house write operations counter */
#define REMOTE_CH_MISC_OPS_COUNT 14     /* C'house misc. operations counter */
#endif  /* #ifndef _ADVER_NCL_DEFS_H */
