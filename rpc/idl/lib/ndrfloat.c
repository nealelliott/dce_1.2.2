/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndrfloat.c,v $
 * Revision 1.1.13.2  1996/02/18  18:54:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:05  marty]
 *
 * Revision 1.1.13.1  1995/12/07  22:29:26  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:07 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:15:38  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:49:14  bfc
 * 	oct 95 idl drop
 * 	[1995/10/22  23:35:58  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/22  22:57:12  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE R1.1
 * 	[1995/10/21  18:25:28  bfc]
 * 
 * Revision 1.1.9.2  1994/02/16  20:29:38  rico
 * 	Really enter previous fix for unmarshalling of CRAY big-endian format
 * 	double-precision floating-point numbers on AIX RS6000 and HPUX.
 * 	[1994/02/16  20:29:01  rico]
 * 
 * Revision 1.1.9.1  1994/02/11  13:52:34  rico
 * 	Fix for unmarshalling of IBM big-endian format double-precision
 * 	floating-point numbers on AIX RS6000 and HPUX.
 * 	[1994/02/09  20:37:39  rico]
 * 
 * Revision 1.1.6.2  1993/07/07  20:08:44  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:38:07  ganni]
 * 
 * $EndLog$
 */
/* 17-Dec-92 harrow Fix IBM->IEEE on OS/2 (OT 5569) */
/* 05-Jun-92 harrow Fix for CRAY->little endian IEEE, big endian VAX, & little endian IBM */
/* 06-May-92 harrow Incorporate floating point conversion fixes for IBM, CRAY, & IEEE */
/*
**
**  Copyright (c) 1989, 1990, 1991 by
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
**  NAME:
**
**      ndrfloat.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**  NDR floating point conversion routines.
**
**  Currently implemented:
**
**        To  || IEEE | VAX  | Cray | IBM
**      ==============+======+======+======
**   F   IEEE || Yes  | Yes  | Yes  | Yes
**      ------||------+------+------+------
**   r   VAX  || Yes  | Yes  | Yes  | Yes
**      ------||------+------+------+------
**   o   Cray || Yes  | Yes  | Yes  | Yes
**      ------||------+------+------+------
**   m   IBM  || Yes  | Yes  | Yes  | Yes
**
**
**  VERSION: DCE 1.0
**
*/

/*
 * VAX Floating point
 * ------------------
 *
 * VAX has four floating types: F, D, G, and H:
 *
 *     F is 32 bits with a 1-bit sign, an 8-bit exponent field (excess
 *     128), and a 23-bit mantissa representing a fraction in the range
 *     0.5 (inclusive) to 1.0 (exclusive).  (The redundant leading "1"
 *     bit is omitted.)
 *
 *     D is 64 bits with a 1 bit sign, an 8-bit exponent, and a 56-bit
 *     mantissa.
 *
 *     G is 64 bits with a 1 bit sign, an 11-bit exponent (excess 1024),
 *     and a 52-bit mantissa.
 *
 *     H is 128 bits. ...
 *
 *
 * IEEE Floating point
 * -------------------
 *
 * IEEE has two floating types: single and double:
 *
 *     Single is 32 bits with a 1-bit sign, an 8-bit exponent field
 *     (excess 127 = 16#7F), and a 23-bit mantissa representing a fraction
 *     in the range 1.0 (inclusive) to 2.0 (exclusive).  (The redundant,
 *     leading "1" bit is omitted.)
 *
 *                              -38         +38
 *                  range   = 10     ..   10
 *
 *                  MaxReal = 3.402823E+38
 *
 *     Double is 64 bits with a 1-bit sign, an 11-bit exponent field
 *     (excess 1023 = 16#3FF) and a 52-bit mantissa.
 *
 *                                -308        +308
 *                  range     = 10     ..   10
 *
 *                  MaxReal = 1.797693134862316E+308
 *
 * Note that (unlike all other allowed NDR floating point representations)
 * IEEE floating point numbers are generated on machines made by different
 * manufacturers (and with different architectures).  In particular, some
 * of those machines are little-endian (e.g. 80x86) and some are big-endian
 * (e.g. M680x0).  NDR defines that to interpret bytes in a stream tagged
 * as using IEEE floating point numbers, the integer representation tag
 * is used to determine the order of the bytes of the IEEE floating point
 * number.  This means that the bytes are in reverse order if sent from
 * machine that uses little-endian integers as compared to one that uses
 * big-endian integers.
 *
 *
 * CRAY Floating Point
 * -------------------
 *
 *     Cray-1, and X-MP's have one hardware floating-point type:  single.
 *     (95-bit double-precision routines are available in software.)
 *
 *     Single is 64 bits with a 1-bit sign, a 15-bit exponent field
 *     (biased by 16384 = 16#4000), and a 48-bit mantissa representing
 *     a fraction in the range of 0.0 (inclusive) to 1.0 (exclusive).
 *
 *                              -2466       +2466
 *                  range   = 10     ..   10
 *
 *                  MaxReal = 1.2650140831707E+2466  !! not correct !!
 *
 *     A zero value or an underflow result is not biased and is represented
 *     as 64 bits of zero.
 *
 *     An overflow result is represented by an exponent >= 24576 (=16#6000).
 *
 * Conversions
 * -----------
 *
 * NDR defines two floating types: single and double.  NDR defines a set
 * of possible representations of these types:  IEEE, VAX, Cray, IBM.  The
 * correspondences between the NDR types and the representations is shown
 * below:
 *
 *       NDR          IEEE     VAX      CRAY     IBM
 *                 +--------+--------+--------+--------+
 *      Single     | single |   F    | single |        |
 *                 +--------+--------+--------+--------+
 *      Double     | double |   G    | single |        |
 *                 +--------+--------+--------+--------+
 *
 * Note that the original VAX architecture defined only F and D, not G or H.
 * Most VAX compilers seem by default to interpret "double precision float"
 * as meaning D format; however, some compilers have an option telling
 * them to interpret it as G.  G seems to be the more "modern" and "popular"
 * flavor of double so that is why NDR uses it; also G corresponds better to
 * IEEE double.
 *
 * NOTE WELL:  The conversion routines as they currently exist are sure to
 * have bugs in the boundary cases.  Note also that the routines that
 * convert to VAX format are likely to work only on a VAX.
 *
 * The following diagrams show how the various representations appear in
 * an NDR byte stream:

    VAX F Floating
    ==============

        |<---- 8 bits ---->|   byte offset      field size

        +--+---------------+
        |X2|   FRAC1       |        0              1, 7
        +--+---------------+
        |SN|     X1        |        1              1, 7
        +--+---------------+
        |     FRAC3        |        2              8
        +------------------+
        |     FRAC2        |        3              8
        +------------------+

        In terms of the VAX architecture handbook:

                X1    = 8:14
                X2    = 7:7
                SN    = 15:15
                FRAC1 = 0:6
                FRAC2 = 24:31
                FRAC3 = 16:23

        FRACn are the segments of the mantissa.  In increasing order of
        significance they are:  FRAC3, FRAC2, FRAC1.  Xn are the segments
        of the exponent.  In increasing order of significance they are:  X2,
        X1.  SN is the sign bit.


    VAX D Floating (for illustration only)
    ==============

        |<---- 8 bits ---->|   byte offset      field size

        +--+---------------+
        |X2|   FRAC1       |        0              1, 7
        +--+---------------+
        |SN|     X1        |        1              1, 7
        +--+---------------+
        |     FRAC3        |        2              8
        +------------------+
        |     FRAC2        |        3              8
        +------------------+
        |     FRAC5        |        4              8
        +------------------+
        |     FRAC4        |        5              8
        +------------------+
        |     FRAC7        |        6              8
        +------------------+
        |     FRAC6        |        7              8
        +------------------+


    VAX G Floating
    ==============

        |<---- 8 bits ---->|    byte offset     field size

        +--------+---------+
        |   X2   | FRAC1   |        0              4, 4
        +--+-----+---------+
        |SN|     X1        |        1              1, 7
        +--+---------------+
        |     FRAC3        |        2              8
        +------------------+
        |     FRAC2        |        3              8
        +------------------+
        |     FRAC5        |        4              8
        +------------------+
        |     FRAC4        |        5              8
        +------------------+
        |     FRAC7        |        6              8
        +------------------+
        |     FRAC6        |        7              8
        +------------------+



    IEEE single
    ===========

            big-endian                         little-endian

        |<---- 8 bits ---->|                |<---- 8 bits ---->|

        +--+---------------+                +------------------+
        |SN|    X1         |    0           |       F3         |    0
        +--+---------------+                +------------------+
        |X2|      F1       |    1           |       F2         |    1
        +--+---------------+                +--+---------------+
        |       F2         |    2           |X2|      F1       |    2
        +------------------+                +--+---------------+
        |       F3         |    3           |SN|    X1         |    3
        +------------------+                +--+---------------+

    IEEE double
    ===========

            big-endian                         little-endian

        |<---- 8 bits ---->|                |<---- 8 bits ---->|

        +--+---------------+                +------------------+
        |SN|    X1         |    0           |       F7         |    0
        +--+-----+---------+                +------------------+
        |   X2   |   F1    |    1           |       F6         |    1
        +--------+---------+                +------------------+
        |       F2         |    2           |       F5         |    2
        +------------------+                +------------------+
        |       F3         |    3           |       F4         |    3
        +------------------+                +------------------+
        |       F4         |    4           |       F3         |    4
        +------------------+                +------------------+
        |       F5         |    5           |       F2         |    5
        +------------------+                +--+---------------+
        |       F6         |    6           |SN|    X1         |    6
        +------------------+                +--+-----+---------+
        |       F7         |    7           |   X2   |   F1    |    7
        +------------------+                +--------+---------+


    Cray single
    ===========


        |<---- 8 bits ---->|

        +--+---------------+
        |SN|    X1         |    0
        +--+-----+---------+
        |       X2         |    1
        +--------+---------+
        |       F1         |    2
        +------------------+
        |       F2         |    3
        +------------------+
        |       F3         |    4
        +------------------+
        |       F4         |    5
        +------------------+
        |       F5         |    6
        +------------------+
        |       F6         |    7
        +------------------+

 */

#include <cvt.h>
#include <cvt_pvt.h>

/* The ordering of the following 3 includes should NOT be changed! */
#include <dce/rpc.h>
#include <dce/stubbase.h>
#include <lsysdep.h>

#ifndef _KERNEL
#   ifdef __STDC__
#       include <stdlib.h>
#   endif
#endif

#define INTERNAL static
#define PUBLIC

#define IEEE_FLOAT (NDR_LOCAL_FLOAT_REP == ndr_c_float_ieee)
#define VAX_FLOAT  (NDR_LOCAL_FLOAT_REP == ndr_c_float_vax)
#define CRAY_FLOAT (NDR_LOCAL_FLOAT_REP == ndr_c_float_cray)
#define IBM_FLOAT  (NDR_LOCAL_FLOAT_REP == ndr_c_float_ibm)

#define SWAB_64(dst, src) ( \
    (dst)[0] = (src)[7], \
    (dst)[1] = (src)[6], \
    (dst)[2] = (src)[5], \
    (dst)[3] = (src)[4], \
    (dst)[4] = (src)[3], \
    (dst)[5] = (src)[2], \
    (dst)[6] = (src)[1], \
    (dst)[7] = (src)[0]  \
)

#define IBM_SWAB_64(dst, src) ( \
    (dst)[0] = (src)[4], \
    (dst)[1] = (src)[5], \
    (dst)[2] = (src)[6], \
    (dst)[3] = (src)[7], \
    (dst)[4] = (src)[0], \
    (dst)[5] = (src)[1], \
    (dst)[6] = (src)[2], \
    (dst)[7] = (src)[3]  \
)

#ifndef cray

#define SWAB_32(dst, src) ( \
    (dst)[0] = (src)[3], \
    (dst)[1] = (src)[2], \
    (dst)[2] = (src)[1], \
    (dst)[3] = (src)[0]  \
)

#define VAXF_BL_SWAP(dst, src) ( \
    (dst)[0] = (src)[1], \
    (dst)[1] = (src)[0], \
    (dst)[2] = (src)[3], \
    (dst)[3] = (src)[2]  \
)

#define VAXF_BL_SWAP1(dst) ( \
    temp = (dst)[0], \
    (dst)[0] = (dst)[1], \
    (dst)[1] = temp, \
    temp = (dst)[2], \
    (dst)[2] = (dst)[3], \
    (dst)[3] = temp \
)

#else

#define SWAB_32(dst, src) ( \
    (dst)[4] = (src)[7], \
    (dst)[5] = (src)[6], \
    (dst)[6] = (src)[5], \
    (dst)[7] = (src)[4]  \
)

#define VAXF_BL_SWAP(dst, src) ( \
    (dst)[4] = (src)[5], \
    (dst)[5] = (src)[4], \
    (dst)[6] = (src)[7], \
    (dst)[7] = (src)[6]  \
)

#endif

#define VAXG_BL_SWAP(dst, src) ( \
    (dst)[0] = (src)[1], \
    (dst)[1] = (src)[0], \
    (dst)[2] = (src)[3], \
    (dst)[3] = (src)[2], \
    (dst)[4] = (src)[5], \
    (dst)[5] = (src)[4], \
    (dst)[6] = (src)[7], \
    (dst)[7] = (src)[6]  \
)

#define VAXG_BL_SWAP1(dst) ( \
    temp = (dst)[0], \
    (dst)[0] = (dst)[1], \
    (dst)[1] = temp, \
    temp = (dst)[2], \
    (dst)[2] = (dst)[3], \
    (dst)[3] = temp, \
    temp = (dst)[4], \
    (dst)[4] = (dst)[5], \
    (dst)[5] = temp, \
    temp = (dst)[6], \
    (dst)[6] = (dst)[7], \
    (dst)[7] = temp \
)

#define COPY_64(dst, src) ( \
    *(long_float_p_t) (dst) = *(long_float_p_t) (src) \
)

#ifndef cray
#define COPY_32(dst, src) ( \
    *(short_float_p_t) (dst) = *(short_float_p_t) (src) \
)
#endif

/*
**  Floating point conversion routines:  --> IEEE
*/

/*
**
**  Routine:
**
**      cvt_vax_f_to_ieee_single
**
**  Functional Description:
**
**      This routine converts a VAX F_Float floating point number
**      into an IEEE single precision floating point number.
**
**  Formal Parameters:
**
**      input_value     A VAX F_Float floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN       - default is little endian
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_ROUND_TO_NEAREST.
**
**                      NOTE2: This routine can not underflow.  The underflow
**                      option may be selected but will be effectively ignored
**                      (not signaled).
**
**      output_value    The IEEE single precision representation of the VAX
**                      F_Float number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IEEE_FLOAT || defined(DEBUG)
INTERNAL void cvt_vax_f_to_ieee_single
#ifdef IDL_PROTOTYPES
(
    CVT_VAX_F input_value,
    CVT_SIGNED_INT options,
    CVT_IEEE_SINGLE output_value
)
#else
( input_value, options, output_value )
    CVT_VAX_F input_value;
    CVT_SIGNED_INT options;
    CVT_IEEE_SINGLE output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkvaxf.c>

#include <pkieees.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_vax_g_to_ieee_double
**
**  Functional Description:
**
**      This routine converts a VAX G_Float floating point number
**      into an IEEE double precision floating point number.
**
**  Formal Parameters:
**
**      input_value     A VAX G_Float floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN       - default is little endian
**                           CVT_C_ERR_UNDERFLOW    - Raise underflows
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_ROUND_TO_NEAREST.
**
**      output_value    The IEEE double precision representation of the VAX
**                      G_Float number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IEEE_FLOAT || defined(DEBUG)
INTERNAL void cvt_vax_g_to_ieee_double
#ifdef IDL_PROTOTYPES
(
    CVT_VAX_G input_value,
    CVT_SIGNED_INT options,
    CVT_IEEE_DOUBLE output_value
)
#else
( input_value, options, output_value )
    CVT_VAX_G input_value;
    CVT_SIGNED_INT options;
    CVT_IEEE_DOUBLE output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkvaxg.c>

#include <pkieeet.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_cray_to_ieee_single
**
**  Functional Description:
**
**      This routine converts a CRAY single precision floating point number
**      into an IEEE single precision floating point number.
**
**  Formal Parameters:
**
**      input_value     A CRAY single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN       - default is little endian
**                           CVT_C_ERR_UNDERFLOW    - Raise underflows
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_ROUND_TO_NEAREST.
**
**      output_value    The IEEE single precision representation of the CRAY
**                      number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IEEE_FLOAT || defined(DEBUG)
INTERNAL void cvt_cray_to_ieee_single
#ifdef IDL_PROTOTYPES
(
    CVT_CRAY input_value,
    CVT_SIGNED_INT options,
    CVT_IEEE_SINGLE output_value
)
#else
( input_value, options, output_value )
    CVT_CRAY input_value;
    CVT_SIGNED_INT options;
    CVT_IEEE_SINGLE output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkcray.c>

#include <pkieees.c>

}


/*
 * Note 32 bit NDR floats for a cray are represented as IEEE 32 bit values.
 */

/*
 * This routine is not "internal" because cray stubbase.h macros need to invoke
 * it directly.
 */
void ndr_cray64_to_ieee32
#ifdef IDL_PROTOTYPES
(
    unsigned32 *cray_flt,   /* 64 bit entities */
    unsigned32 *ieee_flt
)
#else
(cray_flt, ieee_flt)
    unsigned32 *cray_flt;   /* 64 bit entities */
    unsigned32 *ieee_flt;
#endif

{
    /*
     * Cray does not have a 32 bit floating point format so ndr_short_floats
     * must be represented in 64 bits on Cray.  The Cray floating point value
     * is converted to IEEE single for sending over the wire.  Since the Cray
     * word size is 64 bits, make sure we use the least significant 4 bytes of
     * the 8 byte word to represent the IEEE single.  Convert the IEEE single
     * in Cray's native big-endian format.
     */
    cvt_cray_to_ieee_single((CVT_BYTE *)cray_flt, CVT_C_BIG_ENDIAN,
                            (CVT_BYTE *)(&(((char *)ieee_flt)[4])));
}
#endif

/*
**
**  Routine:
**
**      cvt_cray_to_ieee_double
**
**  Functional Description:
**
**      This routine converts a CRAY single precision floating point number
**      into an IEEE double precision floating point number.
**
**  Formal Parameters:
**
**      input_value     A CRAY single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN       - default is little endian
**                           CVT_C_ERR_UNDERFLOW    - Raise underflows
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_ROUND_TO_NEAREST.
**
**      output_value    The IEEE double precision representation of the CRAY
**                      number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IEEE_FLOAT || defined(DEBUG)
INTERNAL void cvt_cray_to_ieee_double
#ifdef IDL_PROTOTYPES
(
    CVT_CRAY input_value,
    CVT_SIGNED_INT options,
    CVT_IEEE_DOUBLE output_value
)
#else
( input_value, options, output_value )
    CVT_CRAY input_value;
    CVT_SIGNED_INT options;
    CVT_IEEE_DOUBLE output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkcray.c>

#include <pkieeet.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_ibm_short_to_ieee_single
**
**  Functional Description:
**
**      This routine converts an IBM single precision floating point number
**      into an IEEE single precision floating point number.
**
**  Formal Parameters:
**
**      input_value     An IBM single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN       - default is little endian
**                           CVT_C_ERR_UNDERFLOW    - Raise underflows
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_ROUND_TO_NEAREST.
**
**      output_value    The IEEE single precision representation of the IBM
**                      number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IEEE_FLOAT || defined(DEBUG)
INTERNAL void cvt_ibm_short_to_ieee_single
#ifdef IDL_PROTOTYPES
(
    CVT_IBM_SHORT input_value,
    CVT_SIGNED_INT options,
    CVT_IEEE_SINGLE output_value
)
#else
( input_value, options, output_value )
    CVT_IBM_SHORT input_value;
    CVT_SIGNED_INT options;
    CVT_IEEE_SINGLE output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkibms.c>

#include <pkieees.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_ibm_long_to_ieee_double
**
**  Functional Description:
**
**      This routine converts an IBM double precision floating point number
**      into an IEEE double precision floating point number.
**
**  Formal Parameters:
**
**      input_value     An IBM double precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN       - default is little endian
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_ROUND_TO_NEAREST.
**
**                      NOTE2: This routine can not underflow.  The underflow
**                      option may be selected but will be effectively ignored
**                      (not signaled).
**
**      output_value    The IEEE double precision representation of the IBM
**                      number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IEEE_FLOAT || defined(DEBUG)
INTERNAL void cvt_ibm_long_to_ieee_double
#ifdef IDL_PROTOTYPES
(
    CVT_IBM_LONG input_value,
    CVT_SIGNED_INT options,
    CVT_IEEE_DOUBLE output_value
)
#else
( input_value, options, output_value )
    CVT_IBM_LONG input_value;
    CVT_SIGNED_INT options;
    CVT_IEEE_DOUBLE output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkibml.c>

#include <pkieeet.c>

}
#endif

/*
**  Floating point conversion routines:  --> VAX
*/

/*
**
**  Routine:
**
**      cvt_ieee_single_to_vax_f
**
**  Functional Description:
**
**      This routine converts an IEEE single precision floating point number
**      into a VAX F_Float floating point number.
**
**  Formal Parameters:
**
**      input_value     An IEEE single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN       - default is little endian
**                           CVT_C_ERR_UNDERFLOW    - Raise underflows
**
**                      NOTE: This routine does not require rounding and
**                      underflow is not possible.  Both options may be selected
**                      but will be effectively ignored (not signaled).
**
**      output_value    The VAX F_Float representation of the IEEE number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if VAX_FLOAT || defined(DEBUG)
INTERNAL void cvt_ieee_single_to_vax_f
#ifdef IDL_PROTOTYPES
(
    CVT_IEEE_SINGLE input_value,
    CVT_SIGNED_INT options,
    CVT_VAX_F output_value
)
#else
( input_value, options, output_value )
    CVT_IEEE_SINGLE input_value;
    CVT_SIGNED_INT options;
    CVT_VAX_F output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_VAX_ROUNDING;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkieees.c>

#include <pkvaxf.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_ieee_double_to_vax_g
**
**  Functional Description:
**
**      This routine converts an IEEE double precision floating point number
**      into a VAX G_Float floating point number.
**
**  Formal Parameters:
**
**      input_value     An IEEE double precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN       - default is little endian
**                           CVT_C_ERR_UNDERFLOW    - Raise underflows
**
**                      NOTE: This routine does not require rounding.  A
**                      rounding option may be selected but will be effectively
**                      ignored (not signaled).
**
**      output_value    The VAX G_Float representation of the IEEE number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if VAX_FLOAT || defined(DEBUG)
INTERNAL void cvt_ieee_double_to_vax_g
#ifdef IDL_PROTOTYPES
(
    CVT_IEEE_DOUBLE input_value,
    CVT_SIGNED_INT options,
    CVT_VAX_G output_value
)
#else
( input_value, options, output_value )
    CVT_IEEE_DOUBLE input_value;
    CVT_SIGNED_INT options;
    CVT_VAX_G output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_VAX_ROUNDING;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkieeet.c>

#include <pkvaxg.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_cray_to_vax_f
**
**  Functional Description:
**
**      This routine converts a CRAY single precision floating point number
**      into a VAX F_Float floating point number.
**
**  Formal Parameters:
**
**      input_value     A CRAY single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_ERR_UNDERFLOW    - Raise underflows
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_VAX_ROUNDING.
**
**      output_value    The VAX F_Float representation of the CRAY number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if VAX_FLOAT || defined(DEBUG)
INTERNAL void cvt_cray_to_vax_f
#ifdef IDL_PROTOTYPES
(
    CVT_CRAY input_value,
    CVT_SIGNED_INT options,
    CVT_VAX_F output_value
)
#else
( input_value, options, output_value )
    CVT_CRAY input_value;
    CVT_SIGNED_INT options;
    CVT_VAX_F output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_VAX_ROUNDING;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkcray.c>

#include <pkvaxf.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_cray_to_vax_g
**
**  Functional Description:
**
**      This routine converts a CRAY single precision floating point number
**      into a VAX G_Float floating point number.
**
**  Formal Parameters:
**
**      input_value     A CRAY single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_ERR_UNDERFLOW    - Raise underflows
**
**                      NOTE: This routine does not require rounding.  A
**                      rounding option may be selected but will be effectively
**                      ignored (not signaled).
**
**      output_value    The VAX G_Float representation of the CRAY number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if VAX_FLOAT || defined(DEBUG)
INTERNAL void cvt_cray_to_vax_g
#ifdef IDL_PROTOTYPES
(
    CVT_CRAY input_value,
    CVT_SIGNED_INT options,
    CVT_VAX_G output_value
)
#else
( input_value, options, output_value )
    CVT_CRAY input_value;
    CVT_SIGNED_INT options;
    CVT_VAX_G output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_VAX_ROUNDING;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkcray.c>

#include <pkvaxg.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_ibm_short_to_vax_f
**
**  Functional Description:
**
**      This routine converts an IBM single precision floating point number
**      into a VAX F_Float floating point number.
**
**  Formal Parameters:
**
**      input_value     An IBM single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_ERR_UNDERFLOW    - Raise underflows
**
**                      NOTE: This routine does not require rounding.  A
**                      rounding option may be selected but will be effectively
**                      ignored (not signaled).
**
**      output_value    The VAX F_Float representation of the IBM number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if VAX_FLOAT || defined(DEBUG)
INTERNAL void cvt_ibm_short_to_vax_f
#ifdef IDL_PROTOTYPES
(
    CVT_IBM_SHORT input_value,
    CVT_SIGNED_INT options,
    CVT_VAX_F output_value
)
#else
( input_value, options, output_value )
    CVT_IBM_SHORT input_value;
    CVT_SIGNED_INT options;
    CVT_VAX_F output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_VAX_ROUNDING;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkibms.c>

#include <pkvaxf.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_ibm_long_to_vax_g
**
**  Functional Description:
**
**      This routine converts an IBM double precision floating point number
**      into a VAX G_Float floating point number.
**
**  Formal Parameters:
**
**      input_value     An IBM double precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_VAX_ROUNDING.
**
**                      NOTE2: This routine can not underflow.  The underflow
**                      option may be selected but will be effectively ignored
**                      (not signaled).
**
**      output_value    The VAX G_Float representation of the IBM number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if VAX_FLOAT || defined(DEBUG)
INTERNAL void cvt_ibm_long_to_vax_g
#ifdef IDL_PROTOTYPES
(
    CVT_IBM_LONG input_value,
    CVT_SIGNED_INT options,
    CVT_VAX_G output_value
)
#else
( input_value, options, output_value )
    CVT_IBM_LONG input_value;
    CVT_SIGNED_INT options;
    CVT_VAX_G output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_VAX_ROUNDING;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkibml.c>

#include <pkvaxg.c>

}
#endif

/*
**  Floating point conversion routines:  --> CRAY
*/

/*
**
**  Routine:
**
**      cvt_ieee_single_to_cray
**
**  Functional Description:
**
**      This routine converts an IEEE single precision floating point number
**      into a CRAY floating point number.
**
**  Formal Parameters:
**
**      input_value     An IEEE single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN - default is little endian
**
**                      NOTE: This routine does not require rounding and
**                      underflow is not possible.  Both options may be selected
**                      but will be effectively ignored (not signaled).
**
**      output_value    The CRAY single precision representation of the IEEE
**                      number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if CRAY_FLOAT || defined(DEBUG)
INTERNAL void cvt_ieee_single_to_cray
#ifdef IDL_PROTOTYPES
(
    CVT_IEEE_SINGLE input_value,
    CVT_SIGNED_INT options,
    CVT_CRAY output_value
)
#else
( input_value, options, output_value )
    CVT_IEEE_SINGLE input_value;
    CVT_SIGNED_INT options;
    CVT_CRAY output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkieees.c>

#include <pkcray.c>

}


/*
 * This routine is not "internal" because cray idl_base.h macros need to invoke
 * it directly.
 */
void ndr_ieee32_to_cray64
#ifdef IDL_PROTOTYPES
(
    unsigned32 *ieee_flt,   /* 64 bit entities */
    unsigned32 *cray_flt
)
#else
(ieee_flt, cray_flt)
    unsigned32 *ieee_flt;   /* 64 bit entities */
    unsigned32 *cray_flt;
#endif

{
    /*
     * Cray does not have a 32 bit floating point format so ndr_short_floats
     * must be represented in 64 bits on Cray.  The Cray floating point value
     * is converted to IEEE single for sending over the wire.  If the callee
     * is also on a Cray, the IEEE single is converted back to a 64 bit Cray.
     * Assume the IEEE single is in Cray's native big-endian format and
     * resides in the least significant 32 bits of the 64 bit argument.
     */
    cvt_ieee_single_to_cray((CVT_BYTE *)(&(((char *)ieee_flt)[4])),
                            CVT_C_BIG_ENDIAN, (CVT_BYTE *)cray_flt);
}
#endif


/*
**
**  Routine:
**
**      cvt_ieee_double_to_cray
**
**  Functional Description:
**
**      This routine converts an IEEE double precision floating point number
**      into a CRAY single precision floating point number.
**
**  Formal Parameters:
**
**      input_value     An IEEE double precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN       - default is little endian
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_ROUND_TO_NEAREST.
**
**                      NOTE2: This routine can not underflow.  The underflow
**                      option may be selected but will be effectively ignored
**                      (not signaled).
**
**      output_value    The CRAY single precision representation of the IEEE
**                      number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if CRAY_FLOAT || defined(DEBUG)
INTERNAL void cvt_ieee_double_to_cray
#ifdef IDL_PROTOTYPES
(
    CVT_IEEE_DOUBLE input_value,
    CVT_SIGNED_INT options,
    CVT_CRAY output_value
)
#else
( input_value, options, output_value )
    CVT_IEEE_DOUBLE input_value;
    CVT_SIGNED_INT options;
    CVT_CRAY output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkieeet.c>

#include <pkcray.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_vax_f_to_cray
**
**  Functional Description:
**
**      This routine converts a VAX F_Float floating point number
**      into a CRAY floating point number.
**
**  Formal Parameters:
**
**      input_value     A VAX F_Float floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  There are no applicable
**                      options.
**
**                      NOTE: This routine does not require rounding and
**                      underflow is not possible.  Both options may be selected
**                      but will be effectively ignored (not signaled).
**
**      output_value    The CRAY single precision representation of the VAX
**                      F_Float number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if CRAY_FLOAT || defined(DEBUG)
INTERNAL void cvt_vax_f_to_cray
#ifdef IDL_PROTOTYPES
(
    CVT_VAX_F input_value,
    CVT_SIGNED_INT options,
    CVT_CRAY output_value
)
#else
( input_value, options, output_value )
    CVT_VAX_F input_value;
    CVT_SIGNED_INT options;
    CVT_CRAY output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkvaxf.c>

#include <pkcray.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_vax_g_to_cray
**
**  Functional Description:
**
**      This routine converts a VAX G_Float floating point number
**      into a CRAY floating point number.
**
**  Formal Parameters:
**
**      input_value     A VAX G_Float floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_ROUND_TO_NEAREST.
**
**                      NOTE2: This routine can not underflow.  The underflow
**                      option may be selected but will be effectively ignored
**                      (not signaled).
**
**      output_value    The CRAY single precision representation of the VAX
**                      G_Float number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if CRAY_FLOAT || defined(DEBUG)
INTERNAL void cvt_vax_g_to_cray
#ifdef IDL_PROTOTYPES
(
    CVT_VAX_G input_value,
    CVT_SIGNED_INT options,
    CVT_CRAY output_value
)
#else
( input_value, options, output_value )
    CVT_VAX_G input_value;
    CVT_SIGNED_INT options;
    CVT_CRAY output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkvaxg.c>

#include <pkcray.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_ibm_short_to_cray
**
**  Functional Description:
**
**      This routine converts an IBM single precision floating point number
**      into a CRAY floating point number.
**
**  Formal Parameters:
**
**      input_value     An IBM single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.
**
**      output_value    The CRAY single precision representation of the
**                      IBM number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if CRAY_FLOAT || defined(DEBUG)
INTERNAL void cvt_ibm_short_to_cray
#ifdef IDL_PROTOTYPES
(
    CVT_IBM_SHORT input_value,
    CVT_SIGNED_INT options,
    CVT_CRAY output_value
)
#else
( input_value, options, output_value )
    CVT_IBM_SHORT input_value;
    CVT_SIGNED_INT options;
    CVT_CRAY output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkibms.c>

#include <pkcray.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_ibm_long_to_cray
**
**  Functional Description:
**
**      This routine converts an IBM double precision floating point number
**      into a CRAY floating point number.
**
**  Formal Parameters:
**
**      input_value     An IBM double precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.
**
**      output_value    The CRAY single precision representation of the
**                      IBM number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified,
**                              or the result is +infinity or -infinity
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if CRAY_FLOAT || defined(DEBUG)
INTERNAL void cvt_ibm_long_to_cray
#ifdef IDL_PROTOTYPES
(
    CVT_IBM_LONG input_value,
    CVT_SIGNED_INT options,
    CVT_CRAY output_value
)
#else
( input_value, options, output_value )
    CVT_IBM_LONG input_value;
    CVT_SIGNED_INT options;
    CVT_CRAY output_value;
#endif

{
    int i, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkibml.c>

#include <pkcray.c>

}
#endif

/*
**  Floating point conversion routines:  --> IBM
*/

/*
**
**  Routine:
**
**      cvt_ieee_single_to_ibm_short
**
**  Functional Description:
**
**      This routine converts an IEEE single precision floating point number
**      into an IBM single precision floating point number.
**
**  Formal Parameters:
**
**      input_value     An IEEE single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN       - default is little endian
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_ROUND_TO_NEAREST.
**
**                      NOTE2: This routine can not underflow.  The underflow
**                      option may be selected but will be effectively ignored
**                      (not signaled).
**
**      output_value    The IBM single precision representation of the IEEE
**                      number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IBM_FLOAT || defined(DEBUG)
INTERNAL void cvt_ieee_single_to_ibm_short
#ifdef IDL_PROTOTYPES
(
    CVT_IEEE_SINGLE input_value,
    CVT_SIGNED_INT options,
    CVT_IBM_SHORT output_value
)
#else
( input_value, options, output_value )
    CVT_IEEE_SINGLE input_value;
    CVT_SIGNED_INT options;
    CVT_IBM_SHORT output_value;
#endif

{
    int i, j, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkieees.c>

#include <pkibms.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_ieee_double_to_ibm_long
**
**  Functional Description:
**
**      This routine converts an IEEE double precision floating point number
**      into a IBM double precision floating point number.
**
**  Formal Parameters:
**
**      input_value     An IEEE double precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_BIG_ENDIAN       - default is little endian
**                           CVT_C_ERR_UNDERFLOW    - Raise underflows
**
**                      NOTE: This routine does not require rounding.  A
**                      rounding option may be selected but will be effectively
**                      ignored (not signaled).
**
**      output_value    The IBM double precision representation of the IEEE
**                      number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IBM_FLOAT || defined(DEBUG)
INTERNAL void cvt_ieee_double_to_ibm_long
#ifdef IDL_PROTOTYPES
(
    CVT_IEEE_DOUBLE input_value,
    CVT_SIGNED_INT options,
    CVT_IBM_LONG output_value
)
#else
( input_value, options, output_value )
    CVT_IEEE_DOUBLE input_value;
    CVT_SIGNED_INT options;
    CVT_IBM_LONG output_value;
#endif

{
    int i, j, round_bit_position;
    UNPACKED_REAL r;

    switch ( options & ~(CVT_C_BIG_ENDIAN | CVT_C_ERR_UNDERFLOW) ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkieeet.c>

#include <pkibml.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_vax_f_to_ibm_short
**
**  Functional Description:
**
**      This routine converts a VAX F_Float floating point number
**      into an IBM single precision floating point number.
**
**  Formal Parameters:
**
**      input_value     A VAX F_Float floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_TRUNCATE         - truncate
**                           CVT_C_ROUND_TO_POS     - round to +infinity
**                           CVT_C_ROUND_TO_NEG     - round to -infinity
**                           CVT_C_ROUND_TO_NEAREST - round to nearest
**                           CVT_C_VAX_ROUNDING     - VAX rounding
**
**                      NOTE: If no rounding mode is selected the following
**                      default rounding mode is assumed:
**
**                              CVT_C_ROUND_TO_NEAREST.
**
**                      NOTE2: This routine can not underflow.  The underflow
**                      option may be selected but will be effectively ignored
**                      (not signaled).
**
**      output_value    The IBM single precision representation of the VAX
**                      F_Float number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IBM_FLOAT || defined(DEBUG)
INTERNAL void cvt_vax_f_to_ibm_short
#ifdef IDL_PROTOTYPES
(
    CVT_VAX_F input_value,
    CVT_SIGNED_INT options,
    CVT_IBM_SHORT output_value
)
#else
( input_value, options, output_value )
    CVT_VAX_F input_value;
    CVT_SIGNED_INT options;
    CVT_IBM_SHORT output_value;
#endif

{
    int i, j, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkvaxf.c>

#include <pkibms.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_vax_g_to_ibm_long
**
**  Functional Description:
**
**      This routine converts a VAX G_Float floating point number
**      into an IBM double precision floating point number.
**
**  Formal Parameters:
**
**      input_value     A VAX G_Float floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.  Applicable options are:
**
**                           CVT_C_ERR_UNDERFLOW    - Raise underflows
**
**                      NOTE: This routine does not require rounding.  A
**                      rounding option may be selected but will be effectively
**                      ignored (not signaled).
**
**      output_value    The IBM double precision representation of the VAX
**                      G_Float number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IBM_FLOAT || defined(DEBUG)
INTERNAL void cvt_vax_g_to_ibm_long
#ifdef IDL_PROTOTYPES
(
    CVT_VAX_G input_value,
    CVT_SIGNED_INT options,
    CVT_IBM_LONG output_value
)
#else
( input_value, options, output_value )
    CVT_VAX_G input_value;
    CVT_SIGNED_INT options;
    CVT_IBM_LONG output_value;
#endif

{
    int i, j, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkvaxg.c>

#include <pkibml.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_cray_to_ibm_short
**
**  Functional Description:
**
**      This routine converts a CRAY single precision floating point number
**      into an IBM single precision floating point number.
**
**  Formal Parameters:
**
**      input_value     A CRAY single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.
**
**      output_value    The IBM single precision representation of the
**                      CRAY number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IBM_FLOAT || defined(DEBUG)
INTERNAL void cvt_cray_to_ibm_short
#ifdef IDL_PROTOTYPES
(
    CVT_CRAY input_value,
    CVT_SIGNED_INT options,
    CVT_IBM_SHORT output_value
)
#else
( input_value, options, output_value )
    CVT_CRAY input_value;
    CVT_SIGNED_INT options;
    CVT_IBM_SHORT output_value;
#endif

{
    int i, j, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkcray.c>

#include <pkibms.c>

}
#endif

/*
**
**  Routine:
**
**      cvt_cray_to_ibm_long
**
**  Functional Description:
**
**      This routine converts a CRAY single precision floating point number
**      into an IBM double precision floating point number.
**
**  Formal Parameters:
**
**      input_value     A CRAY single precision floating point number.
**
**      options         An integer bit mask.  Set bits in the mask represent
**                      selected routine options.
**
**      output_value    The IBM double precision representation of the
**                      CRAY number.
**
**  Side Effects/Signaled Errors:
**
**      exc_e_aritherr            an invalid input value was specified
**      exc_e_fltovf_t          floating point overflow
**      exc_e_fltund_t          floating point underflow
**
*/

#if IBM_FLOAT || defined(DEBUG)
INTERNAL void cvt_cray_to_ibm_long
#ifdef IDL_PROTOTYPES
(
    CVT_CRAY input_value,
    CVT_SIGNED_INT options,
    CVT_IBM_LONG output_value
)
#else
( input_value, options, output_value )
    CVT_CRAY input_value;
    CVT_SIGNED_INT options;
    CVT_IBM_LONG output_value;
#endif

{
    int i, j, round_bit_position;
    UNPACKED_REAL r;

    switch ( options &  ~ CVT_C_ERR_UNDERFLOW ) {
        case 0                      : options |= CVT_C_ROUND_TO_NEAREST;
        case CVT_C_ROUND_TO_NEAREST :
        case CVT_C_TRUNCATE         :
        case CVT_C_ROUND_TO_POS     :
        case CVT_C_ROUND_TO_NEG     :
        case CVT_C_VAX_ROUNDING     : break;
        default : RAISE(exc_e_aritherr);
    }

#include <upkcray.c>

#include <pkibml.c>

}
#endif

/*
 * C V T _ F L O A T
 *
 * Internal common routine for "ndr_cvt_long/short_float".
 */

INTERNAL void cvt_float
#ifdef IDL_PROTOTYPES
(
    ndr_format_t sdrep,
    ndr_format_t ddrep,
    unsigned_char_t *srcp,
    unsigned_char_t *dstp,
    ndr_boolean longf
)
#else
(sdrep, ddrep, srcp, dstp, longf)
    ndr_format_t sdrep;
    ndr_format_t ddrep;
    unsigned_char_t *srcp;
    unsigned_char_t *dstp;
    ndr_boolean longf;
#endif

{
    unsigned_char_t buff[8];
    CVT_SIGNED_INT  cvt_options = 0;
    unsigned_char_t temp = 0;

/*
 * Conversion routines expect operands to be in their native format,
 * e.g. big-endian for CRAY floating point numbers.  Conversion routines
 * involving IEEE numbers accept a flag indicating little- or big-endian.
 */
    switch (ddrep.float_rep)
    {

#if IEEE_FLOAT
        case ndr_c_float_ieee:
            if (ddrep.int_rep == ndr_c_int_big_endian)
                cvt_options |= CVT_C_BIG_ENDIAN;

            switch (sdrep.float_rep)
            {
                case ndr_c_float_ieee:  /* IEEE -> IEEE */
                    if (ddrep.int_rep == sdrep.int_rep)
                        if (longf)
                            COPY_64(dstp, srcp);
                        else
                            COPY_32(dstp, srcp);
                    else
                        if (longf)
                            SWAB_64(dstp, srcp);
                        else
                            SWAB_32(dstp, srcp);
                    break;

                case ndr_c_float_vax:   /* VAX -> IEEE */
                    /*
                     *  Since the conversion routines expect to operation on a
                     *  little-endian machine, byte swap before, and tell it
                     *  the result is little endian in order to get the correct
                     *  result.
                     */
                    if (!((sdrep.int_rep == ndr_c_int_little_endian) &&
                          (ddrep.int_rep == ndr_c_int_little_endian)))
                    {
                        if (longf)
                        {
                            SWAB_32(buff, srcp);
                            SWAB_32(buff+4, srcp+4);
                        }
                        else
                            SWAB_32(buff, srcp);
                        srcp = buff;
                    }
                    if (longf)
                        cvt_vax_g_to_ieee_double(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    else
                        cvt_vax_f_to_ieee_single(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    break;

                case ndr_c_float_cray:  /* CRAY -> IEEE */
                    /* Cray short floats will be in IEEE 32 Bit format. */
                    if (longf)
                    {
                        /* Cray native format big-endian; swap if necessary */
#if !(defined(_IBMR2) && defined(_AIX)) && \
    !(defined(__hppa) && defined(__hpux))
                        if (!((sdrep.int_rep == ndr_c_int_big_endian) &&
                              (ddrep.int_rep == ndr_c_int_little_endian)))
                        {
                            SWAB_64(buff, srcp);
                            srcp = buff;
                        }
#endif
                        cvt_cray_to_ieee_double(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    }
                    else    /* Already in IEEE format but might have to swap */
                        if (sdrep.int_rep != ddrep.int_rep)
                            SWAB_32(dstp, srcp);
                    break;

                case ndr_c_float_ibm:   /* IBM -> IEEE */
                    /* IBM native format big-endian; swap if necessary */
                    if (sdrep.int_rep != ddrep.int_rep)
                    {
                        if (ddrep.int_rep != ndr_c_int_little_endian)
                        {
                            if (longf)
                                SWAB_64(buff, srcp);
                            else
                                SWAB_32(buff, srcp);
                            srcp = buff;
                        }
                        else
                        {
                            if (longf)
                            {
                                IBM_SWAB_64(buff, srcp);
                                srcp = buff;
                            }
                        }
                    }
                    if (longf)
                        cvt_ibm_long_to_ieee_double(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    else
                        cvt_ibm_short_to_ieee_single(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    break;

                default:
                    abort();
            }
            break;
#endif

#if VAX_FLOAT
        case ndr_c_float_vax:
            switch (sdrep.float_rep)
            {
                case ndr_c_float_ieee:  /* IEEE -> VAX */
                    if (sdrep.int_rep == ndr_c_int_big_endian)
                        cvt_options |= CVT_C_BIG_ENDIAN;
                    if (longf)
                        cvt_ieee_double_to_vax_g(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    else
                        cvt_ieee_single_to_vax_f(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    break;

                case ndr_c_float_vax:   /* VAX -> VAX */
                    if (ddrep.int_rep == sdrep.int_rep)
                        if (longf)
                            COPY_64(dstp, srcp);
                        else
                            COPY_32(dstp, srcp);
                    else
                        if (longf)
                            VAXG_BL_SWAP(dstp, srcp);
                        else
                            VAXF_BL_SWAP(dstp, srcp);
                    break;

                case ndr_c_float_cray:  /* CRAY -> VAX */
                    /* Cray short floats will be in IEEE 32 Bit format. */
                    if (longf)
                    {
                        /* Cray native format big-endian; swap if necessary */
                        if (sdrep.int_rep != ndr_c_int_big_endian)
                        {
                            SWAB_64(buff, srcp);
                            srcp = buff;
                        }
                        cvt_cray_to_vax_g(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    }
                    else    /* Source operand in IEEE format */
                    {
                        if (sdrep.int_rep == ndr_c_int_big_endian)
                            cvt_options |= CVT_C_BIG_ENDIAN;
                        cvt_ieee_single_to_vax_f(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    }
                    break;

                case ndr_c_float_ibm:   /* IBM -> VAX */
                    /* IBM native format big-endian; swap if necessary */
                    if (sdrep.int_rep != ndr_c_int_big_endian)
                    {
                        if (longf)
                            SWAB_64(buff, srcp);
                        else
                            SWAB_32(buff, srcp);
                        srcp = buff;
                    }
                    if (longf)
                        cvt_ibm_long_to_vax_g(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    else
                        cvt_ibm_short_to_vax_f(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    break;

                default:
                    abort();
            }

#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)
            /*
             * Conversion routines produce native VAX floating point format.
             * If destination integer rep is big-endian VAX, convert from VAX
             * native floating point format to true big-endian byte ordering.
             */
            if (sdrep.float_rep != ndr_c_float_vax
                &&  ddrep.int_rep == ndr_c_int_big_endian)
                if (longf)
                    VAXG_BL_SWAP1(dstp);
                else
                    VAXF_BL_SWAP1(dstp);
#endif

            break;
#endif

#if CRAY_FLOAT
        case ndr_c_float_cray:
            switch (sdrep.float_rep)
            {
                case ndr_c_float_ieee:  /* IEEE -> CRAY */
                    if (sdrep.int_rep == ndr_c_int_big_endian)
                        cvt_options |= CVT_C_BIG_ENDIAN;
                    if (longf)
                        cvt_ieee_double_to_cray(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    else
                        cvt_ieee_single_to_cray(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    break;

                case ndr_c_float_vax:   /* VAX -> CRAY */
                    /*
                     * Assume that little-endian integer format implies VAX
                     * native floating point format; big-endian integer format
                     * implies true big-endian byte ordering, so swap bytes
                     * within 16-bit words.
                     */
                    if (sdrep.int_rep == ndr_c_int_big_endian)
                    {
                        if (longf)
                            VAXG_BL_SWAP(buff, srcp);
                        else
                            VAXF_BL_SWAP(buff, srcp);
                        srcp = buff;
                    }
                    if (longf)
                        cvt_vax_g_to_cray(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    else
                        cvt_vax_f_to_cray(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    break;

                case ndr_c_float_cray:  /* CRAY -> CRAY */
                    /* Cray short floats will be in IEEE 32 Bit format. */
                    if (longf)
                        if (ddrep.int_rep == sdrep.int_rep)
                            COPY_64(dstp, srcp);
                        else
                            SWAB_64(dstp, srcp);
                    else    /* Source operand in IEEE format */
                    {
                        if (sdrep.int_rep == ndr_c_int_big_endian)
                            cvt_options |= CVT_C_BIG_ENDIAN;
                        cvt_ieee_single_to_cray(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    }
                    break;

                case ndr_c_float_ibm:   /* IBM -> CRAY */
                    if (sdrep.int_rep == ndr_c_int_big_endian)
                        cvt_options |= CVT_C_BIG_ENDIAN;
                    if (longf)
                        cvt_ibm_long_to_cray(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    else
                        cvt_ibm_short_to_cray(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    break;


                default:
                    abort();
            }
            break;
#endif

#if IBM_FLOAT
        case ndr_c_float_ibm:
            switch (sdrep.float_rep)
            {
                case ndr_c_float_ieee:  /* IEEE -> IBM */
                    if (sdrep.int_rep == ndr_c_int_big_endian)
                        cvt_options |= CVT_C_BIG_ENDIAN;
#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)
		    if (sdrep.int_rep != ddrep.int_rep)
		    {
			/* convert to big endian format for SAA */
			if (longf)
			    SWAB_64(buff,srcp);
			else
			    SWAB_32(buff,srcp);
			srcp = buff;
		    }
#endif
                    if (longf)
                        cvt_ieee_double_to_ibm_long(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    else
                        cvt_ieee_single_to_ibm_short(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    break;

                case ndr_c_float_vax:   /* VAX -> IBM */
                    /*
                     * Assume that little-endian integer format implies VAX
                     * native floating point format; big-endian integer format
                     * implies true big-endian byte ordering, so swap bytes
                     * within 16-bit words.
                     */
                    if (sdrep.int_rep == ndr_c_int_big_endian)
                    {
                        if (longf)
                            VAXG_BL_SWAP(buff, srcp);
                        else
                            VAXF_BL_SWAP(buff, srcp);
                        srcp = buff;
                    }
                    if (longf)
                        cvt_vax_g_to_ibm_long(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    else
                        cvt_vax_f_to_ibm_short(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    break;

                case ndr_c_float_cray:  /* CRAY -> IBM */
                    if (sdrep.int_rep == ndr_c_int_big_endian)
                        cvt_options |= CVT_C_BIG_ENDIAN;
                    if (longf)
                        cvt_cray_to_ibm_long(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    else
                        cvt_cray_to_ibm_short(
                            (CVT_BYTE *)srcp, cvt_options, (CVT_BYTE *)dstp);
                    break;

                case ndr_c_float_ibm:   /* IBM -> IBM */
                    if (ddrep.int_rep == sdrep.int_rep)
                        if (longf)
                            COPY_64(dstp, srcp);
                        else
                            COPY_32(dstp, srcp);
                    else
                        if (longf)
                            SWAB_64(dstp, srcp);
                        else
                            SWAB_32(dstp, srcp);
                    break;

                default:
                    abort();
            }
            break;
#endif

        default:
            abort();
    }
}

/*
 * N D R _ C V T _ S H O R T _ F L O A T
 *
 * Convert between short (32 bit) floating point representations.
 */

PUBLIC void ndr_cvt_short_float
#ifdef IDL_PROTOTYPES
(
    ndr_format_t source_drep,
    ndr_format_t dst_drep,
    short_float_p_t srcp,
    short_float_p_t dstp
)
#else
(source_drep, dst_drep, srcp, dstp)
    ndr_format_t source_drep;
    ndr_format_t dst_drep;
    short_float_p_t srcp;
    short_float_p_t dstp;
#endif

{
    cvt_float(source_drep, dst_drep,
              (unsigned_char_t *)srcp, (unsigned_char_t *)dstp,
              ndr_false);
}


/*
 * N D R _ C V T _ L O N G _ F L O A T
 *
 * Convert between long (64 bit) floating point representations.
 */

PUBLIC void ndr_cvt_long_float
#ifdef IDL_PROTOTYPES
(
    ndr_format_t source_drep,
    ndr_format_t dst_drep,
    long_float_p_t srcp,
    long_float_p_t dstp
)
#else
(source_drep, dst_drep, srcp, dstp)
    ndr_format_t source_drep;
    ndr_format_t dst_drep;
    long_float_p_t srcp;
    long_float_p_t dstp;
#endif

{
    cvt_float(source_drep, dst_drep,
              (unsigned_char_t *)srcp, (unsigned_char_t *)dstp,
              ndr_true);
}

#ifdef DEBUG
globaldef void (*cvt_vector[])() = {
    cvt_vax_f_to_ieee_single,
    cvt_vax_g_to_ieee_double,
    cvt_cray_to_ieee_single,
    cvt_cray_to_ieee_double,
    cvt_ibm_short_to_ieee_single,
    cvt_ibm_long_to_ieee_double,
    cvt_ieee_single_to_vax_f,
    cvt_ieee_double_to_vax_g,
    cvt_cray_to_vax_f,
    cvt_cray_to_vax_g,
    cvt_ibm_short_to_vax_f,
    cvt_ibm_long_to_vax_g,
    cvt_ieee_single_to_cray,
    cvt_ieee_double_to_cray,
    cvt_vax_f_to_cray,
    cvt_vax_g_to_cray,
    cvt_ieee_single_to_ibm_short,
    cvt_ieee_double_to_ibm_long,
    cvt_vax_f_to_ibm_short,
    cvt_vax_g_to_ibm_long
};
#endif
