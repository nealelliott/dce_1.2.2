/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_audit_trail.h,v $
 * Revision 1.1.4.2  1996/02/18  22:58:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:23  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:24:58  root
 * 	Submit
 * 	[1995/12/11  15:14:35  root]
 * 
 * Revision 1.1.2.3  1992/12/29  13:07:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:59  zeliff]
 * 
 * Revision 1.1.2.2  1992/08/31  18:17:15  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:56:51  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:42:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_audit_trail.h V=5 04/29/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Audit trail facility
 *
 */

#ifndef __rs_audit_trail_h__included
#define __rs_audit_trail_h__included

void rs_audit_trail_enable (
#ifdef __STDC__
    void
#endif
);

void rs_audit_trail_log (
#ifdef __STDC__
    handle_t    h,
    char        *info,
    char        *key
#endif
);

#endif /* rs_audit_trail_h__included */
