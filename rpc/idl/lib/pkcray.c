/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pkcray.c,v $
 * Revision 1.1.10.2  1996/02/17  23:58:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:52:42  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:31:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:16:27  root]
 * 
 * Revision 1.1.6.2  1993/07/07  20:10:36  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:39:13  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1990, 1991 by
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
**      pkcray.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      This module contains code to extract information from an
**      UNPACKED_REAL structure and to create a CRAY floating number
**      with those bits.
**
**              This module is meant to be used as an include file.
**
**  VERSION: DCE 1.0
**
*/

/*
**++
**  Functional Description:
**
**  This module contains code to extract information from an
**  UNPACKED_REAL structure and to create a CRAY floating number
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
**      options: a word of flags, see include files.
**
**      output_value: a pointer to the input parameter.
**
**      r: an UNPACKED_REAL structure.
**
**--
*/



if (r[U_R_FLAGS] & U_R_UNUSUAL) {

        if (r[U_R_FLAGS] & U_R_ZERO)

                if (r[U_R_FLAGS] & U_R_NEGATIVE)
                        memcpy(output_value, CRAY_NEG_ZERO, 8);
                else
                        memcpy(output_value, CRAY_POS_ZERO, 8);

        else if (r[U_R_FLAGS] & U_R_INFINITY) {

                if (r[U_R_FLAGS] & U_R_NEGATIVE) {
                        memcpy(output_value, CRAY_NEG_INFINITY, 8);
                        RAISE(exc_e_aritherr);    /* Negative infinity */
                } else {
                        memcpy(output_value, CRAY_POS_INFINITY, 8);
                        RAISE(exc_e_aritherr);    /* Positive infinity */
                }

        } else if (r[U_R_FLAGS] & U_R_INVALID) {

                memcpy(output_value, CRAY_INVALID, 8);
                RAISE(exc_e_aritherr);    /* Invalid value */

        }

} else {

        round_bit_position = 48;

#include "round.c"

        if (r[U_R_EXP] < (U_R_BIAS - 8192)) {

                /* Underflow */

                if (r[U_R_FLAGS] & U_R_NEGATIVE)
                        memcpy(output_value, CRAY_NEG_ZERO, 8);
                else
                        memcpy(output_value, CRAY_POS_ZERO, 8);
                if (options & CVT_C_ERR_UNDERFLOW) {
                        RAISE(exc_e_fltund);  /* Underflow */
                }

        } else if (r[U_R_EXP] > (U_R_BIAS + 8191)) {

                /* Overflow */

                if (options & CVT_C_TRUNCATE) {

                        if (r[U_R_FLAGS] & U_R_NEGATIVE)
                                memcpy(output_value, CRAY_NEG_HUGE, 8);
                        else
                                memcpy(output_value, CRAY_POS_HUGE, 8);

                } else if ((options & CVT_C_ROUND_TO_POS)
                                        && (r[U_R_FLAGS] & U_R_NEGATIVE)) {

                                memcpy(output_value, CRAY_NEG_HUGE, 8);

                } else if ((options & CVT_C_ROUND_TO_NEG)
                                        && !(r[U_R_FLAGS] & U_R_NEGATIVE)) {

                                memcpy(output_value, CRAY_POS_HUGE, 8);

                } else {

                        memcpy(output_value, CRAY_INVALID, 8);

                }

                RAISE(exc_e_fltovf);  /* Overflow */


        } else {

                /* Adjust bias of exponent */

                r[U_R_EXP] -= (U_R_BIAS - 16384);

                /* Make room for exponent and sign bit */

                r[2] >>= 16;
                r[2] |= (r[1] << 16);
                r[1] >>= 16;

                /* OR in exponent and sign bit */

                r[1] |= (r[U_R_EXP] << 16);
                r[1] |= (r[U_R_FLAGS] << 31);

                /* Shuffle bytes to big endian format */
#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)
		if (options & CVT_C_BIG_ENDIAN) {

			r[0] = r[1];
			r[1] = r[2];

		} else {

	                r[0]  = ((r[1] << 24) | (r[1] >> 24));
        	        r[0] |= ((r[1] << 8) & 0x00FF0000L);
                	r[0] |= ((r[1] >> 8) & 0x0000FF00L);
	                r[1]  = ((r[2] << 24) | (r[2] >> 24));
        	        r[1] |= ((r[2] << 8) & 0x00FF0000L);
                	r[1] |= ((r[2] >> 8) & 0x0000FF00L);

		}
#else

                r[0]  = ((r[1] << 24) | (r[1] >> 24));
                r[0] |= ((r[1] << 8) & 0x00FF0000L);
                r[0] |= ((r[1] >> 8) & 0x0000FF00L);
                r[1]  = ((r[2] << 24) | (r[2] >> 24));
                r[1] |= ((r[2] << 8) & 0x00FF0000L);
                r[1] |= ((r[2] >> 8) & 0x0000FF00L);
#endif
                memcpy(output_value, r, 8);

        }

}

