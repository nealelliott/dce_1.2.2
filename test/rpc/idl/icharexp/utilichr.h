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
 * Revision 1.1.4.2  1996/02/18  23:05:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:59  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:07:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:28  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:27:11  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:15:07  ganni]
 * 
 * $EndLog$
 */
/*
*/

/* This test deals with the simple case where the local type and wire type
    are both byte. */

typedef idl_byte ltype;

#define RTN_STAG 16
/* High nibble precedes low nibble */
#define RTN_DRTAG 17
/* Invalid. Gets converted to */
#define RTN_RTAG 18
/* Low nibble precedes high nibble */
