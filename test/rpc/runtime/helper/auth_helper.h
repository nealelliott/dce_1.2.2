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
 * Revision 1.1.4.2  1996/02/18  23:06:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:46  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:13:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:46  root]
 * 
 * Revision 1.1.2.2  1993/01/13  19:52:18  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:19:48  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:43:16  devrcs
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


#define PUT_LONG(ptr, l) { long long_temp = l; \
    memcpy(ptr, &long_temp, sizeof(long)); \
        ptr += sizeof(long); }
#define GET_LONG(ptr, l) { long long_temp; \
    memcpy(&long_temp, ptr, sizeof(long)); \
        (l) = long_temp; ptr += sizeof(long); }

