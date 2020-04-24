/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: Pointer.h,v $
 * Revision 1.1.4.2  1996/02/18  22:57:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:42  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:24:23  root
 * 	Submit
 * 	[1995/12/11  15:14:08  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:05:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:03  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:41:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  Pointer.h V=3 10/19/89 //nbs/dds_tools/src
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* Pointer.h, perry, 05/24/89
 *      Define the Pointer datatype.  A Pointer can hold an opaque pointer
 *      to any datatype.
 *
 * 
 *  Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 *
 *
 *  VersionID = @(#)Pointer.h	3 - 10/19/89
 */

#ifndef Pointer_d
#   define Pointer_d
#   if !defined(apollo) || defined(CHARPTR)
        typedef char * Pointer;
#   else
        typedef void * Pointer;
#   endif

#   define NULL_PTR (void *) 0

#endif
