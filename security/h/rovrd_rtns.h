/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rovrd_rtns.h,v $
 * Revision 1.1.4.2  1996/02/18  22:58:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:05  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:24:44  root
 * 	Submit
 * 	[1995/12/11  15:14:26  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:07:10  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:37  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rovrd_rtns.h V=1 07/26/91 //littl/prgy/src/client/sec_clientd
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*  Remove Override Routines' header file
** 
*/

#ifndef rovrd__included
#define rovrd__included

PUBLIC void register_overrides (
#ifdef __STDC__
    error_status_t  *status
#endif
);

#endif /* rovrd__included */

