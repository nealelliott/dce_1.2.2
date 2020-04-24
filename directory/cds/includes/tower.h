/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tower.h,v $
 * Revision 1.1.8.2  1996/02/18  23:34:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:42  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:18:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:59  root]
 * 
 * Revision 1.1.6.4  1994/08/03  19:02:50  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:24  mccann]
 * 
 * Revision 1.1.6.3  1994/06/30  19:15:44  mccann
 * 	sams cleanup drop 2
 * 	[1994/06/30  18:01:57  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:39:41  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:22  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:04:40  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:20:48  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:51:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:55:20  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:11:38  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:29:01  weisman]
 * 
 * Revision 1.1  1992/01/19  15:15:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _TOWER_H
#define _TOWER_H
/*
 * Module tower.h
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
#include <dce/dns_record.h>

#define TCP_ID 0x07
#define UDP_ID 0x08
#define IP_ID  0x09

/*
 * tower_lib.c
 */

int
towerset_compare_exact (
    Set_t		*,
    Set_t		*);

int
towerset_compare_contained (
    Set_t		*,
    Set_t		*);

/*
 * Server-specific towerset_compare_exact_mx()
 * moved to server/db_common.c as local routine.
 */

#include <dce/cds_clerkserver.h>

extern rpc_if_handle_t cds_ifspec;

int
cds_LocalTowers (
	rpc_binding_vector_t	*,
	byte_t			*);

int
cdsCvtStrToTower (
    	char	*[],
    	char	**);

int
cdsCvtTowerToStr (
	void	        *,
	unsigned char	**);

int
get_binding_string (
	byte_t		*,
	char		*);

int
get_cds_binding_string (
	byte_t	*,
	char	*);

#endif  /* #ifndef _TOWER_H */
