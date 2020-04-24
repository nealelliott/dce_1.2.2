/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: round.c,v $
 * Revision 1.1.8.2  1996/02/17  23:58:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:52:48  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:32:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:16:43  root]
 * 
 * Revision 1.1.4.2  1993/07/07  20:11:09  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:39:38  ganni]
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
**      round.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      This module is an include file.
**
**	This module rounds CVT floating point data to any specified position.
**	Any of the following rounding modes can be applied:
**
**      Note: None of the following implementations ever perform true truncation
**            on their values.  Whenever truncation becomes necessary - either
**            by being specified directly or by being required indirectly
**            through rounding - values are actually left untouched.  Users
**            of this routine must zero out fractional fields themselves if
**            true truncation is needed.
**
**         VAX ROUNDING
**
**              Input data are rounded such that the representable value nearest
**              the infinitely precise result is delivered; if two representable
**              values are equally near, the one greatest in magnitude is
**              delivered.
**
**         ROUND TO NEAREST
**
**              Input data are rounded such that the representable value nearest
**              the infinitely precise result is delivered; if two representable
**              values are equally near, the one with its least significant bit
**              zero is delivered.
**
**         ROUND TO POSITIVE INFINITY
**
**              Input data are rounded such that the representable value closest
**              to and no less than the infinitely precise result is delivered.
**
**         ROUND TO NEGATIVE INFINITY
**
**              Input data are rounded such that the representable value closest
**              to and no greater than the infinitely precise result is
**              delivered.
**
**         TRUNCATION (ROUND TOWARDS ZERO)
**
**              True truncation is not implemented here.  Input values are
**              delivered in their original, untouched form.
**
**              A definition of "true" truncation follows:  Truncation, or
**              rounding towards zero, implies input data are rounded such
**              that the representable value closest to and no greater in
**              magnitude than the infinitely precise result is delivered.
**
**  VERSION: DCE 1.0
**
*/

/*
**
**  Implicit input/output:
**
**	r			On input, a valid CVT floating point number.
**				On output, a rounded representation of the
**				input.
**
**
**  Implicit input:
**
**	round_bit_position	An integer specifying the position to round to.
**				0 <= round_bit_position <= 127.
**
**				Note: Valid CVT mantissa bits are addressed as 1
**				through 128.  Accordingly, specifying 0 as a
**				position to round to implies an exponent
**				increase whenever rounding occurs.  As for
**				truncation: truncation allways leaves a CVT
**				number untouched.
**
**	options			A valid CVT options bit mask in which at least
**				one, and only one, CVT rounding mode is
**				specified.  If no rounding mode is specified,
**				results are unpredictable.  Rounding is
**				performed in accordance with this mask.
**
**	i			An uninitialized integer used for indexing.
**
**
**  Note: for efficiency this routine performs no explicit error checking.
**
*/


{
  int roundup, more_bits;
  unsigned long  bit_mask;


      /* Check TRUNCATE option */

  if ( ! (options & CVT_C_TRUNCATE) ) {

           /* Determine which word the round bit resides in */

      i = (round_bit_position >> 5) + 1;

           /* Create a mask isolating the round bit */

      bit_mask = 0x1L << (31 - (round_bit_position & 0x1FL));

           /* Check VAX ROUNDING option */

      if (options & CVT_C_VAX_ROUNDING)
          roundup = r[i] & bit_mask;

      else {
          roundup = 0;
          switch ( r[i] & bit_mask ) {

                /* If round bit is clear, and ROUND TO NEAREST option */
                /* is selected we truncate */

          case  0 : if (options & CVT_C_ROUND_TO_NEAREST)
                      break;

                /* Otherwise, make note of wheather there are any bits set */
                /* after the round bit, and then check the remaining cases */

          default : if ( ! (more_bits = r[i] & (bit_mask - 1)) )
                      switch ( i ) {
                        case  1 : more_bits = r[2];
                        case  2 : more_bits |= r[3];
                        case  3 : more_bits |= r[4];
                        default : break;
                      }

                /* Re-check ROUND TO NEAREST option.  NOTE: if we've reached  */
                /* this point and ROUND TO NEAREST has been selected, the     */
                /* round bit is set. */

                    if (options & CVT_C_ROUND_TO_NEAREST) {
                        if ( ! ( roundup = more_bits ) )
                            if ( bit_mask << 1 )
                               roundup = r[i] & (bit_mask << 1);
                            else if (i != 1)
                               roundup = r[i-1] & 1;

                /* Check ROUND TO POSITIVE INFINITY option */

                    } else if (options & CVT_C_ROUND_TO_POS) {
                        if ( !(r[U_R_FLAGS] & U_R_NEGATIVE) )
                          roundup = (r[i] & bit_mask) | more_bits;

                /* Check ROUND TO NEGITIVE INFINITY option */

                    } else if (r[U_R_FLAGS] & U_R_NEGATIVE)
                        roundup = (r[i] & bit_mask) | more_bits;
          }
      }


      if ( roundup ) {          /* Perform rounding if necessary */

               /* Add 1 at round position */

         bit_mask <<= 1;
         r[i] = (r[i] & ~(bit_mask - 1)) + bit_mask;

               /* Propagate any carry */

       	 while ( ! r[i] )
           r[--i] += 1;

               /* If carry reaches exponent MSB gets zeroed and must be reset */

         if ( ! i )
           r[1] = 0x80000000L;
      }
  }
}
