/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: upkcray.c,v $
 * Revision 1.1.10.2  1996/02/17  23:58:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:52:52  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:32:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:16:52  root]
 * 
 * Revision 1.1.6.2  1993/07/07  20:11:28  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:39:51  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME:
**
**      upkcray.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      This module contains code to extract information from a cray
**      floating number and to initialize an UNPACKED_REAL structure
**      with those bits.
**
**		This module is meant to be used as an include file.
**
**  VERSION: DCE 1.0
**
*/

/*
**++
**  Functional Description:
**
**  This module contains code to extract information from a cray
**  floating number and to initialize an UNPACKED_REAL structure
**  with those bits.
**
**  See the header files for a description of the UNPACKED_REAL
**  structure.
**
**  A normalized CRAY floating number looks like:
**
**      [0]: Sign bit, 15 exp bits (bias 16384), 16 fraction bits
**      [1]: 32 low order fraction bits
**
**      0.5 <= fraction < 1.0, MSB explicit
**      Since CRAY has no hidden bits the MSB must always be set.
**
**  Some of the CRAY exponent range is not used.
**  Exponents < 0x2000 and >= 0x6000 are invalid.
**
**
**  Implicit parameters:
**
**  	input_value: a pointer to the input parameter.
**
**  	r: an UNPACKED_REAL structure.
**
**  	i: a temporary integer variable.
**
**--
*/



	memcpy(r, input_value, 8);

	/* Shuffle bytes to little endian format */
#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)
	if (options & CVT_C_BIG_ENDIAN) {
        
                r[2] = r[1];
                r[1] = r[0];
        
        } else {
        
		r[2] = r[0];
		r[1] = r[1];

	}
#else
	r[2]  = ((r[1] << 24) | (r[1] >> 24));
	r[2] |= ((r[1] << 8) & 0x00FF0000L);
	r[2] |= ((r[1] >> 8) & 0x0000FF00L);
	r[1]  = ((r[0] << 24) | (r[0] >> 24));
	r[1] |= ((r[0] << 8) & 0x00FF0000L);
	r[1] |= ((r[0] >> 8) & 0x0000FF00L);
#endif

	/* Initialize FLAGS and perhaps set NEGATIVE bit */

	r[U_R_FLAGS] = (r[1] >> 31);

	/* Clear sign bit */

	r[1] &= 0x7FFFFFFFL;

	/* Extract CRAY biased exponent */

	r[U_R_EXP] = r[1] >> 16;

	if ((r[1] == 0) && (r[2] == 0)) {

		r[U_R_FLAGS] |= U_R_ZERO;

	} else if (    (r[U_R_EXP] <  0x2000)
				|| (r[U_R_EXP] >= 0x6000)
				|| (!(r[1] & 0x00008000L)) ) {

		r[U_R_FLAGS] |= U_R_INVALID;

	} else {

		/* Adjust bias */

		r[U_R_EXP] += (U_R_BIAS - 16384);

		/* Left justify fraction bits */

		r[1] <<= 16;
		r[1] |= (r[2] >> 16);
		r[2] <<= 16;

		/* Clear uninitialized part of unpacked real */

		r[3] = 0;
		r[4] = 0;

	}
