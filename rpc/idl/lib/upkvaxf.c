/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: upkvaxf.c,v $
 * Revision 1.1.8.2  1996/02/17  23:58:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:52:56  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:33:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:17:05  root]
 * 
 * Revision 1.1.4.2  1993/07/07  20:11:51  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:40:08  ganni]
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
**      upkvaxf.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      This module contains code to extract information from a VAX
**      f_floating number and to initialize an UNPACKED_REAL structure
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
**  This module contains code to extract information from a VAX
**  f_floating number and to initialize an UNPACKED_REAL structure
**  with those bits.
**
**  See the header files for a description of the UNPACKED_REAL
**  structure.
**
**  A VAX f_floating number in (16 bit words) looks like:
**
**      [0]: Sign bit, 8 exp bits (bias 128), 7 fraction bits
**      [1]: 16 more fraction bits
**
**      0.5 <= fraction < 1.0, MSB implicit
**
**
**  Implicit parameters:
**
**  	input_value: a pointer to the input parameter.
**
**  	r: an UNPACKED_REAL structure
**
**--
*/



	memcpy(&r[1], input_value, 4);

	/* Initialize FLAGS and perhaps set NEGATIVE bit */

	r[U_R_FLAGS] = (r[1] >> 15) & U_R_NEGATIVE;

	/* Extract VAX biased exponent */

	r[U_R_EXP] = (r[1] >> 7) & 0x000000FFL;

	if (r[U_R_EXP] == 0) {

		if (r[U_R_FLAGS])
			r[U_R_FLAGS] |= U_R_INVALID;
		else
			r[U_R_FLAGS] = U_R_ZERO;

	} else {

		/* Adjust for VAX 16 bit floating format */

		r[1] = ((r[1] << 16) | (r[1] >> 16));

		/* Add unpacked real bias and subtract VAX bias */

		r[U_R_EXP] += (U_R_BIAS - 128);

		/* Set hidden bit */

		r[1] |= 0x00800000L;

		/* Left justify fraction bits */

		r[1] <<= 8;

		/* Clear uninitialized parts for unpacked real */

		r[2] = 0;
		r[3] = 0;
		r[4] = 0;

	}
