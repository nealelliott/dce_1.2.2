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
 * Revision 1.1.4.2  1996/02/18  23:05:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:58  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:07:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:16  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:26:41  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:14:43  ganni]
 * 
 * $EndLog$
 */
/*
*/


typedef idl_byte ltype;

#define RTN_STAG 16
/* On wire high nibble is in row, low nibble is in column */
#define RTN_DRTAG 17
/* Invalid. Gets converted to */
#define RTN_RTAG 18
/* On wire high nibble is in column, low nibble is in row */


typedef struct {
    idl_byte row;
    idl_byte column;
} mtype;

#define MSTAG 24
/* Locally high nibble is in row, low nibble is in column */
#define MRTAG 26
/* Locally high nibble is in column, low nibble is in row */


typedef idl_byte ntype;

#define NCTAG 32
/* No conversion */

