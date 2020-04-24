/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: uid.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:51  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:18:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:05  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:54  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:38  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:46  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:28  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:04:50  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:21:25  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:51:46  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:55:55  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _UID_H
#define _UID_H
/*
 * Module uid.h
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
 * MODULE DESCRIPTION:
 *
 * Networks & Communications Software Engineering
 */
#include <dce/sys_time.h>
#include <dce/dns_record.h>

/*
 * uid_lib.c
 */
void
ts_update_last_ts (
    Timestamp_t		*);

dns_status_t
ts_new (
    Timestamp_t		*);

dns_status_t
id_new (
    ObjUID_t		*);

int
ts_sanity (
    Timestamp_t		*,
    Timestamp_t		*);

int
ts_future_skew (
    Timestamp_t		*);

void
ts_to_string (
    Timestamp_t		*,
    char		*,
    int			);

#endif  /* #ifndef _UID_H */
