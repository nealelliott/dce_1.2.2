/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: find.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:42  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:12  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:24  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:58:06  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:10  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:48  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:25  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:16:28  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:31:53  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:50:49  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:14:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _FIND_H
#define _FIND_H
/*
 * Module find.h
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
#include <dce/attributes.h>
#include <dbdef.h>

/*
 * find_lib.c
 */
DBSetMember_t *
find_last_value (
    const DBSet_t	*const );

attribute_descriptor_t *
find_known_attribute (
    const AttributeName_t *const );

dns_status_t
find_attribute (
    const data_record_t	*const   ,
    const AttributeName_t *const ,
    DBSet_t		         **,
    int			         *);

void 
find_ts_position (
    const DBSet_t	*const ,
    const Timestamp_t  	*const ,
    DBSetMember_t	       **,
    int			       *);

#endif  /* #ifndef _FIND_H */
