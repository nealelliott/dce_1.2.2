/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dns_record_lib.c,v $
 * Revision 1.1.8.2  1996/02/18  19:32:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:15  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:21:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:57  root]
 * 
 * Revision 1.1.6.3  1994/06/30  19:16:30  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:49  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:40:51  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:13  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:05:52  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:25:17  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:53:49  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:07:17  zeliff]
 * 
 * Revision 1.1.2.3  1992/04/24  19:35:12  mfox
 * 	Non-portable UUID definition prevented IBM to PMAX interoperability
 * 	[1992/04/24  19:34:21  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:50:48  weisman
 * 		New file from Digital (post 1.0)
 * 	[1992/03/22  20:58:06  weisman]
 * 
 * $EndLog$
 */
/*
 * Module: dns_record_lib.c
 *
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
 *
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/dns_record.h>

DNS_CONST Timestamp_t	 NullTimestamp[1]	= { INITIALIZE_Timestamp };

DNS_CONST NULLFULLNAME_t NullFullName[1]	= { INITIALIZE_FullName };

DNS_CONST NULLSIMPLENAME_t NullSimpleName[1]	= { INITIALIZE_SimpleName };

DNS_CONST NULLSIMPLENAME_t NullBinarySimpleName[1] =
					    { INITIALIZE_BinarySimpleName };

DNS_CONST ObjUID_u      cdsWildRoot[]           = {
#if BYTE_ORDER == LITTLE_ENDIAN
     0x86, 0x22, 0xED, 0x11, 0xBB, 0x49, 0xCA, 0x11,
     0x8F, 0x29, 0x08, 0x00, 0x2B, 0x0D, 0xC4, 0x6C
#else
     0x11, 0xED, 0x22, 0x86, 0x49, 0xBB, 0x11, 0xCA,
     0x8F, 0x29, 0x08, 0x00, 0x2B, 0x0D, 0xC4, 0x6C
#endif
};

