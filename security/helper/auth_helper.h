/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: auth_helper.h,v $
 * Revision 1.1.33.2  1996/02/18  23:01:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:11  marty]
 *
 * Revision 1.1.33.1  1995/12/08  17:30:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:41  root]
 * 
 * Revision 1.1.31.1  1993/10/07  13:12:06  mccann
 * 	CR8651 64bit porting changes
 * 	[1993/10/07  13:03:48  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  13:28:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:16:27  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:55:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  auth_helper.h V=2 10/18/91 //littl/prgy/src/helper
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca.
**
**   Copyright (c) Hewlett-Packard Company 1991
**   Unpublished work. All Rights Reserved.
**  
**
**  NAME
**
**      auth_helper.h
**
**  FACILITY:
**
**      DCE Security
**
**  ABSTRACT:
**
**  	Definitions useful in authentication helper process.
**
**    
**
*/


#define PUT_LONG(ptr, l) { signed32 long_temp = l; \
    memcpy(ptr, &long_temp, sizeof(signed32)); \
        ptr += sizeof(signed32); }
#define GET_LONG(ptr, l) { signed32 long_temp; \
    memcpy(&long_temp, ptr, sizeof(signed32)); \
        (l) = long_temp; ptr += sizeof(signed32); }
