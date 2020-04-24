/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pkibms.c,v $
 * Revision 1.1.10.2  1996/02/17  23:58:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:52:43  marty]
 *
 * Revision 1.1.10.1  1995/12/07  22:31:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:16:32  root]
 * 
 * Revision 1.1.6.2  1993/07/07  20:10:46  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:39:20  ganni]
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
**      pkibms.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      This module contains code to extract information from an
**      UNPACKED_REAL structure and to create an IBM short floating number
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
**  UNPACKED_REAL structure and to create an IBM short floating number
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
**      options: a word of flags, see include files.
**
**      output_value: a pointer to the input parameter.
**
**      r: an UNPACKED_REAL structure.
**
**      i: temporary integer variable
**
**      j: temporary integer variable
**
**--
*/




if (r[U_R_FLAGS] & U_R_UNUSUAL) {

        if (r[U_R_FLAGS] & U_R_ZERO)

                if (r[U_R_FLAGS] & U_R_NEGATIVE)
                        memcpy(output_value, IBM_S_NEG_ZERO, 4);
                else
                        memcpy(output_value, IBM_S_POS_ZERO, 4);

        else if (r[U_R_FLAGS] & U_R_INFINITY) {

                if (r[U_R_FLAGS] & U_R_NEGATIVE)
                        memcpy(output_value, IBM_S_NEG_INFINITY, 4);
                else
                        memcpy(output_value, IBM_S_POS_INFINITY, 4);

        } else if (r[U_R_FLAGS] & U_R_INVALID) {

                memcpy(output_value, IBM_S_INVALID, 4);
                RAISE(exc_e_aritherr);    /* Invalid value */

        }

} else {

        /* Precision varies because exp must be multiple of 4 */
        /* (since we must convert it to a hexadecimal exponent). */
        /* So, figure out where to round (21 <= i <= 24). */

        i = (r[U_R_EXP] & 0x00000003L);
        if (i)
                round_bit_position = i + 20;
        else
                round_bit_position = 24;

#include "round.c"

        if (r[U_R_EXP] < (U_R_BIAS - 255)) {

                /* Underflow */

                if (r[U_R_FLAGS] & U_R_NEGATIVE)
                        memcpy(output_value, IBM_S_NEG_ZERO, 4);
                else
                        memcpy(output_value, IBM_S_POS_ZERO, 4);
                if (options & CVT_C_ERR_UNDERFLOW) {
                        RAISE(exc_e_fltund);  /* Underflow */
                }

        } else if (r[U_R_EXP] > (U_R_BIAS + 252)) {

                /* Overflow */

                if (options & CVT_C_TRUNCATE) {

                        if (r[U_R_FLAGS] & U_R_NEGATIVE)
                                memcpy(output_value, IBM_S_NEG_HUGE, 4);
                        else
                                memcpy(output_value, IBM_S_POS_HUGE, 4);

                } else if ((options & CVT_C_ROUND_TO_POS)
                                        && (r[U_R_FLAGS] & U_R_NEGATIVE)) {

                                memcpy(output_value, IBM_S_NEG_HUGE, 4);

                } else if ((options & CVT_C_ROUND_TO_NEG)
                                        && !(r[U_R_FLAGS] & U_R_NEGATIVE)) {

                                memcpy(output_value, IBM_S_POS_HUGE, 4);

                } else {

                        if (r[U_R_FLAGS] & U_R_NEGATIVE)
                                memcpy(output_value, IBM_S_NEG_INFINITY, 4);
                        else
                                memcpy(output_value, IBM_S_POS_INFINITY, 4);

                }

                RAISE(exc_e_fltovf);  /* Overflow */

        } else {

                /* Figure leading zeros (i) and biased exponent (j) */

                i = (r[U_R_EXP] & 0x00000003L);
                j = ((int)(r[U_R_EXP] - U_R_BIAS) / 4) + 64;

                if (i) {
                        if (r[U_R_EXP] > U_R_BIAS)
                                j += 1;
                        i = 12 - i;
                } else {
                        i = 8;
                }

                /* Make room for exponent and sign bit */

                r[1] >>= i;

                /* OR in exponent and sign bit */

                r[1] |= (j << 24);
                r[1] |= (r[U_R_FLAGS] << 31);

#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)

                memcpy(output_value, &r[1], 4);

#else
                /* Shuffle bytes to big endian format */

                r[0]  = ((r[1] << 24) | (r[1] >> 24));
                r[0] |= ((r[1] << 8) & 0x00FF0000L);
                r[0] |= ((r[1] >> 8) & 0x0000FF00L);

                memcpy(output_value, r, 4);
#endif
        }

}
