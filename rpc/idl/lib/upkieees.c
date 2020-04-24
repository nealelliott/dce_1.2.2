/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: upkieees.c,v $
 * Revision 1.1.10.2  1996/02/17  23:58:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:52:54  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:33:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:16:59  root]
 * 
 * Revision 1.1.6.2  1993/07/07  20:11:41  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:40:01  ganni]
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
**      upkieees.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      This module contains code to extract information from a IEEE
**      single floating number and to initialize an UNPACKED_REAL structure
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
**  This module contains code to extract information from a IEEE
**  single floating number and to initialize an UNPACKED_REAL structure
**  with those bits.
**
**  See the header files for a description of the UNPACKED_REAL
**  structure.
**
**  A normalized IEEE single precision floating number looks like:
**
**      Sign bit, 8 exp bits (bias 127), 23 fraction bits
**
**      1.0 <= fraction < 2.0, MSB implicit
**
**  For more details see "Mips R2000 Risc Architecture"
**  by Gerry Kane, page 6-8 or ANSI/IEEE Std 754-1985.
**
**
**  Implicit parameters:
**
**  	options: a word of flags, see include files.
**
**  	input_value: a pointer to the input parameter.
**
**  	r: an UNPACKED_REAL structure.
**
**  	i: a temporary integer variable.
**
**--
*/


#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)

	memcpy(&r[1], input_value, 4);

#else
	if (options & CVT_C_BIG_ENDIAN) {

		memcpy(r, input_value, 4);

		/* Shuffle bytes to little endian format */

		r[1]  = ((r[0] << 24) | (r[0] >> 24));
		r[1] |= ((r[0] << 8) & 0x00FF0000L);
		r[1] |= ((r[0] >> 8) & 0x0000FF00L);

	} else {

		memcpy(&r[1], input_value, 4);

	}
#endif

	/* Initialize FLAGS and perhaps set NEGATIVE bit */
	
	r[U_R_FLAGS] = (r[1] >> 31);

	/* Extract biased exponent */

	r[U_R_EXP] = (r[1] >> 23) & 0x000000FFL;

	/* Check for denormalized values */

	if (r[U_R_EXP] == 0) {

		/* Clear sign bit */

		r[1] &= 0x7FFFFFFFL;

		/* If fraction is non-zero then normalize it */

		if (r[1]) {

			/* Count leading zeros */

			i = 0;
			while (!(r[1] & 0x00400000L)) {
				r[1] <<= 1;
				i += 1;
			}

			/* Adjust exponent and normalize fraction */

			r[U_R_EXP] = U_R_BIAS - 126 - i;
			r[1] <<= 9;

			/* Clear uninitialized part of unpacked real */

			r[2] = 0;
			r[3] = 0;
			r[4] = 0;

		} else {

			r[U_R_FLAGS] |= U_R_ZERO;

		}


	/* Check for NANs and INFINITIES */

	} else if (r[U_R_EXP] == 255) {

		/* Clear sign and exponent */

		r[1] &= 0x007FFFFFL;

		if (r[1])
			r[U_R_FLAGS] |= U_R_INVALID;
		else
			r[U_R_FLAGS] |= U_R_INFINITY;

	} else {

		/* Adjust exponent bias */

		r[U_R_EXP] += (U_R_BIAS - 126);

		/* Set hidden bit */

		r[1] |= 0x00800000L;

		/* Left justify fraction bits */

		r[1] <<= 8;

		/* Clear uninitialized part of unpacked real */

		r[2] = 0;
		r[3] = 0;
		r[4] = 0;

	}
