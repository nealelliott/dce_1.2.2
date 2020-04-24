/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_dns.h,v $
 * Revision 1.1.4.2  1996/02/18  22:58:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:28  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:25:07  root
 * 	Submit
 * 	[1995/12/11  15:14:40  root]
 * 
 * Revision 1.1.2.3  1992/12/29  13:07:51  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:09  zeliff]
 * 
 * Revision 1.1.2.2  1992/08/31  18:17:39  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:36:30  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_dns.h V=12 12/16/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - mainline
 */

#ifndef rs_dns_h__included
#define rs_dns_h__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>

void rs_dns_attr_add_value (
#ifdef __STDC__
    unsigned_char_p_t   name,
    unsigned_char_p_t   attr_name,
    unsigned_char_p_t   nsattr_value,
    unsigned32          nsattr_value_len,
    unsigned32          *status
#endif
);

void rs_dns_attr_remove_value (
#ifdef __STDC__
    unsigned_char_p_t   name,
    unsigned_char_p_t   attr_name,
    unsigned_char_p_t   nsattr_value,
    unsigned32          nsattr_value_len,
    unsigned32          *status
#endif
);

void rs_dns_attr_read_value (
#ifdef __STDC__
    unsigned_char_p_t   name,
    unsigned_char_p_t   attr_name,
    unsigned_char_p_t   *nsattr_value,
    unsigned32          *nsattr_value_len,
    unsigned32          *status
#endif
);

#endif

