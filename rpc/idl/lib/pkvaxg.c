/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pkvaxg.c,v $
 * Revision 1.1.8.2  1996/02/17  23:58:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:52:47  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:32:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:16:41  root]
 * 
 * Revision 1.1.4.2  1993/07/07  20:11:04  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:39:34  ganni]
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
**      pkvaxg.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      This module contains code to extract information from an
**      UNPACKED_REAL structure and to create a VAX g_floating number
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
**  UNPACKED_REAL structure and to create a VAX g_floating number
**  with those bits.
**
**  See the header files for a description of the UNPACKED_REAL
**  structure.
**
**  A VAX g_floating number in (16 bit words) looks like:
**
**      [0]: Sign bit, 11 exp bits (bias 1024), 4 fraction bits
**      [1]: 16 more fraction bits
**      [2]: 16 more fraction bits
**      [3]: 16 more fraction bits
**
**      0.5 <= fraction < 1.0, MSB implicit
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

                memcpy(output_value, VAX_G_ZERO, 8);

        else if (r[U_R_FLAGS] & U_R_INFINITY) {

                memcpy(output_value, VAX_G_INVALID, 8);
                if (r[U_R_FLAGS] & U_R_NEGATIVE) {
                        RAISE(exc_e_aritherr);    /* Negative infinity */
                } else {
                        RAISE(exc_e_aritherr);    /* Positive infinity */
                }

        } else if (r[U_R_FLAGS] & U_R_INVALID) {

                memcpy(output_value, VAX_G_INVALID, 8);
                RAISE(exc_e_aritherr);    /* Invalid value */

        }

} else {

        round_bit_position = 53;

#include "round.c"

        if (r[U_R_EXP] < (U_R_BIAS - 1023)) {

                /* Underflow */

                memcpy(output_value, VAX_G_ZERO, 8);
                if (options & CVT_C_ERR_UNDERFLOW) {
                        RAISE(exc_e_fltund);  /* Underflow */
                }

        } else if (r[U_R_EXP] > (U_R_BIAS + 1023)) {

                /* Overflow */

                if (options & CVT_C_TRUNCATE) {

                        if (r[U_R_FLAGS] & U_R_NEGATIVE)
                                memcpy(output_value, VAX_G_NEG_HUGE, 8);
                        else
                                memcpy(output_value, VAX_G_POS_HUGE, 8);

                } else if ((options & CVT_C_ROUND_TO_POS)
                                        && (r[U_R_FLAGS] & U_R_NEGATIVE)) {

                                memcpy(output_value, VAX_G_NEG_HUGE, 8);

                } else if ((options & CVT_C_ROUND_TO_NEG)
                                        && !(r[U_R_FLAGS] & U_R_NEGATIVE)) {

                                memcpy(output_value, VAX_G_POS_HUGE, 8);

                } else {

                        memcpy(output_value, VAX_G_INVALID, 8);

                }

                RAISE(exc_e_fltovf);  /* Overflow */

        } else {

                /* Adjust bias of exponent */

                r[U_R_EXP] -= (U_R_BIAS - 1024);

                /* Make room for exponent and sign bit */

                r[2] >>= 11;
                r[2] |= (r[1] << 21);
                r[1] >>= 11;

                /* Clear implicit bit */

                r[1] &= 0x000FFFFFL;

                /* OR in exponent and sign bit */

                r[1] |= (r[U_R_EXP] << 20);
                r[1] |= (r[U_R_FLAGS] << 31);

                /* Adjust for VAX 16 bit floating format */

                r[1] = ((r[1] << 16) | (r[1] >> 16));
                r[2] = ((r[2] << 16) | (r[2] >> 16));

                memcpy(output_value, &r[1], 8);

        }

}
