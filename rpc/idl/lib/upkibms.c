/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: upkibms.c,v $
 * Revision 1.1.10.2  1996/02/17  23:58:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:52:53  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:33:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:16:57  root]
 * 
 * Revision 1.1.6.2  1993/07/07  20:11:37  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:39:58  ganni]
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
**      upkibms.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      This module contains code to extract information from an IBM
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
**  This module contains code to extract information from an IBM
**  single floating number and to initialize an UNPACKED_REAL structure
**  with those bits.
**
**  See the header files for a description of the UNPACKED_REAL
**  structure.
**
**  A normalized IBM short floating number looks like:
**
**      Sign bit, 7 exp bits (bias 64), 24 fraction bits
**
**      0.0625 <= fraction < 1.0, from 0 to 3 leading zeros
**      to compensate for the hexadecimal exponent.
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


#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)

        memcpy(&r[1], input_value, 4); 

#else
	memcpy(r, input_value, 4);

	/* Shuffle bytes to little endian format */

	r[1]  = ((r[0] << 24) | (r[0] >> 24));
	r[1] |= ((r[0] << 8) & 0x00FF0000L);
	r[1] |= ((r[0] >> 8) & 0x0000FF00L);

#endif

	/* Initialize FLAGS and perhaps set NEGATIVE bit */

	r[U_R_FLAGS] = (r[1] >> 31);

	/* Clear sign bit */

	r[1] &= 0x7FFFFFFFL;

	if (r[1] == 0) {

		r[U_R_FLAGS] |= U_R_ZERO;

	} else {

		/* Get unbiased hexadecimal exponent and convert it to binary */

		r[U_R_EXP] = U_R_BIAS + (((r[1] >> 24) - 64) * 4);

		/* Count leading zeros */

		i = 0;
		while (!(r[1] & 0x00800000L)) {
			i += 1;
			if (i > 3)
				break;
			r[1] <<= 1;
		}

		if (i > 3) {

			r[U_R_FLAGS] |= U_R_INVALID;

		} else {

			/* Adjust exponent to compensate for leading zeros */

			r[U_R_EXP] -= i;

			/* Left justify fraction bits */

			r[1] <<= 8;

			/* Clear uninitialized part of unpacked real */

			r[2] = 0;
			r[3] = 0;
			r[4] = 0;
		}

	}
