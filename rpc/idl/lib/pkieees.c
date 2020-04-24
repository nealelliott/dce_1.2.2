/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pkieees.c,v $
 * Revision 1.1.10.2  1996/02/17  23:58:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:52:44  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:32:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:16:34  root]
 * 
 * Revision 1.1.6.2  1993/07/07  20:10:51  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:39:24  ganni]
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
**      pkieees.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      This module contains code to extract information from an
**      UNPACKED_REAL structure and to create an IEEE single floating number
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
**  UNPACKED_REAL structure and to create an IEEE single number
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
                        memcpy(output_value, IEEE_S_NEG_ZERO, 4);
                else
                        memcpy(output_value, IEEE_S_POS_ZERO, 4);

        else if (r[U_R_FLAGS] & U_R_INFINITY) {

                if (r[U_R_FLAGS] & U_R_NEGATIVE)
                        memcpy(output_value, IEEE_S_NEG_INFINITY, 4);
                else
                        memcpy(output_value, IEEE_S_POS_INFINITY, 4);

        } else if (r[U_R_FLAGS] & U_R_INVALID) {

                memcpy(output_value, IEEE_S_INVALID, 4);
                RAISE(exc_e_aritherr);    /* Invalid value */

        }

} else {

        /* Precision varies if value will be a denorm */
        /* So, figure out where to round (0 <= i <= 24). */

        round_bit_position = r[U_R_EXP] - ((U_R_BIAS - 126) - 23);
        if (round_bit_position < 0)
                round_bit_position = 0;
        else if (round_bit_position > 24)
                round_bit_position = 24;

#include "round.c"

        if (r[U_R_EXP] < (U_R_BIAS - 125)) {

                /* Denorm or underflow */

                if (r[U_R_EXP] < ((U_R_BIAS - 125) - 23)) {

                        /* Value is too small for a denorm, so underflow */

                        if (r[U_R_FLAGS] & U_R_NEGATIVE)
                                memcpy(output_value, IEEE_S_NEG_ZERO, 4);
                        else
                                memcpy(output_value, IEEE_S_POS_ZERO, 4);
                        if (options & CVT_C_ERR_UNDERFLOW) {
                                RAISE(exc_e_fltund);  /* Underflow */
                        }

                } else {

                        /* Figure leading zeros for denorm and right-justify fraction */

                        i = 32 - (r[U_R_EXP] - ((U_R_BIAS - 126) - 23));
                        r[1] >>= i;

                        /* Set sign bit */

                        r[1] |= (r[U_R_FLAGS] << 31);

                        if (options & CVT_C_BIG_ENDIAN) {

                                r[0]  = ((r[1] << 24) | (r[1] >> 24));
                                r[0] |= ((r[1] << 8) & 0x00FF0000L);
                                r[0] |= ((r[1] >> 8) & 0x0000FF00L);
                                memcpy(output_value, r, 4);

                        } else {

                                memcpy(output_value, &r[1], 4);

                        }
                }

        } else if (r[U_R_EXP] > (U_R_BIAS + 128)) {

                /* Overflow */

                if (options & CVT_C_TRUNCATE) {

                        if (r[U_R_FLAGS] & U_R_NEGATIVE)
                                memcpy(output_value, IEEE_S_NEG_HUGE, 4);
                        else
                                memcpy(output_value, IEEE_S_POS_HUGE, 4);

                } else if ((options & CVT_C_ROUND_TO_POS)
                                        && (r[U_R_FLAGS] & U_R_NEGATIVE)) {

                                memcpy(output_value, IEEE_S_NEG_HUGE, 4);

                } else if ((options & CVT_C_ROUND_TO_NEG)
                                        && !(r[U_R_FLAGS] & U_R_NEGATIVE)) {

                                memcpy(output_value, IEEE_S_POS_HUGE, 4);

                } else {

                        if (r[U_R_FLAGS] & U_R_NEGATIVE)
                                memcpy(output_value, IEEE_S_NEG_INFINITY, 4);
                        else
                                memcpy(output_value, IEEE_S_POS_INFINITY, 4);

                }

                RAISE(exc_e_fltovf);  /* Overflow */

        } else {

                /* Adjust bias of exponent */

                r[U_R_EXP] -= (U_R_BIAS - 126);

                /* Make room for exponent and sign bit */

                r[1] >>= 8;

                /* Clear implicit bit */

                r[1] &= 0x007FFFFFL;

                /* OR in exponent and sign bit */

                r[1] |= (r[U_R_EXP] << 23);
                r[1] |= (r[U_R_FLAGS] << 31);

#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)

                memcpy(output_value, &r[1], 4);

#else
                if (options & CVT_C_BIG_ENDIAN) {

                        r[0]  = ((r[1] << 24) | (r[1] >> 24));
                        r[0] |= ((r[1] << 8) & 0x00FF0000L);
                        r[0] |= ((r[1] >> 8) & 0x0000FF00L);
                        memcpy(output_value, r, 4);

                } else {

                        memcpy(output_value, &r[1], 4);

                }
#endif
        }

}
