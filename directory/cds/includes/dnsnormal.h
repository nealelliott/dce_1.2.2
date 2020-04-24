/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsnormal.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:32  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:05  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:19  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:57:53  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:04  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:41  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:04  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:14:58  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:30:43  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:49:14  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _DNSNORMAL_H
#define _DNSNORMAL_H
/*
 * Module dnsnormal.h
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
 */
/*
 * dns_normal.c
 */
int
dns_normalize_fname (
    FullName_t *,
    FullName_t *,
    int        );

int
dns_normalize_sname (
    SimpleName_t          *,
    register SimpleName_t *,
    int                   );







#endif  /* #ifndef _DNSNORMAL_H */
