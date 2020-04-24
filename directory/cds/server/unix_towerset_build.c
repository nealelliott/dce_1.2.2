/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: unix_towerset_build.c,v $
 * Revision 1.1.8.2  1996/02/18  19:37:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:08  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:34:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:43  root]
 * 
 * Revision 1.1.6.4  1994/08/03  19:05:13  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:33:26  mccann]
 * 
 * Revision 1.1.6.3  1994/06/30  19:18:23  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:49:37  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:44:22  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:03  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:14:09  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:49:10  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:25:42  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:26:44  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:40:12  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:38:04  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module unix_towerset_build.c
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
 *
 */
#include <server.h>
#include <address.h>
#include <tower.h>


/*
 * towerset_build
 *    gets local tower from session and fills it into the buffer.
 *    assumes that there is enough room.  Should be at least 4000 bytes.
 *    This is the max for one attribute, which this might have to fit
 *    into... e.g. for a replica_pointer
 *
 * Input:
 *     tower_p = pointer to tower
 * Output:
 * 
 * Value:
 *     length of tower
 *
 * Note:
 *      remove acquiring an association as soon as VMS stops requiring it
 */

extern struct nsgbl_ns dns_nameserver;

int 
towerset_build (byte_t *tower_p)
{
    return cds_LocalTowers ( dns_nameserver.ns_bind_vector, tower_p );
}









