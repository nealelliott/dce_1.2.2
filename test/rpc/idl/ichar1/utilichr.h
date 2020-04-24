/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utilichr.h,v $
 * Revision 1.1.4.2  1996/02/18  23:05:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:57  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:07:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:07  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:26:09  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:14:20  ganni]
 * 
 * $EndLog$
 */
/*
*/

/* This test deals with the simple case where the local type and wire type
    are both byte. */

typedef idl_byte ltype;

#define RTN_STAG 16
/* Convert type is "in place".
    When marshalling add 3 to the byte value
    When unmarshalling add 5 to the byte value */
#define RTN_DRTAG 17
/* Invalid. Gets converted to */
#define RTN_RTAG 18
/* Convert type is "in place".
    When marshalling add 2 to the byte value
    When unmarshalling add 6 to the byte value */
