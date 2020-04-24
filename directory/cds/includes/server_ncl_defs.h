/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: server_ncl_defs.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:25  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:18:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:45  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:41  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:03  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:31  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:12  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:04:14  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:19:24  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:34:14  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:53:57  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:17:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _SERVER_NCL_DEFS_H
#define _SERVER_NCL_DEFS_H
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1989-1994. ALL RIGHTS RESERVED.
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
 
/* ARCHITECTED server attribute codes   */
#define DNS_CREATE_TIME 0
#define DNS_FUTURE_SKEW 6
#define DNS_READ_ACCESS_COUNT 11
#define DNS_WRITE_ACCESS_COUNT 12
#define DNS_SKULKS_INITIATED_COUNT 13
#define DNS_SKULKS_COMPLETED_COUNT 14
#define DNS_TREE_BROKEN_COUNT 15
#define DNS_CRUCIAL_REPLICA_COUNT 17
#define DNS_CHILD_POINTER_COUNT 18
#define DNS_SECURITY_COUNT 19
#define DNS_KNOWN_CLEARINGHOUSES 21

/* ARCHITECTED codes for clearinghouse attributes  */

#define CH_READ_ACCESS_COUNT 11
#define CH_WRITE_ACCESS_COUNT 12
#define CH_REFERENCES_RETURNED_COUNT 13
#define CH_ROOT_NOT_REACHABLE_COUNT 14
#define CH_DATA_CORRUPTION_COUNT 15
#define CH_SKULK_FAILURES_COUNT 16
#define CH_ENTRY_MISSING_COUNT 17
#define CH_UPGRADES_FAILED_COUNT 18
#define CH_ENABLE_COUNT 20
#define CH_DISABLE_COUNT 21
/*                                                                          */
/*	Codes for server DISABLE directive MODE argument,                   */
/*	and for the values this argument can take.                          */
/*                                                                          */
#define DNS_DISABLE_MODE 0
#define DNS_DISABLE_GRACEFUL 0
#define DNS_DISABLE_ABORT 1


#endif  /* #ifndef _SERVER_NCL_DEFS_H */
