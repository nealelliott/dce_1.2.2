/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: upkibml.c,v $
 * Revision 1.1.15.2  1996/02/17  23:58:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:52:52  marty]
 *
 * Revision 1.1.15.1  1995/12/07  22:32:57  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/20  22:57 UTC  jrr
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/11/17  17:08 UTC  dat  /main/dat_xidl2/1]
 * 
 * 	oct 95 idl drop
 * 	[1995/10/22  23:36:31  bfc]
 * 	 *
 * 	may 95 idl drop
 * 	[1995/10/22  22:58:25  bfc]
 * 	 *
 * 	DCE for DEC OSF/1: populate from OSF DCE R1.1
 * 	[1995/10/21  18:26:12  bfc]
 * 	 *
 * 
 * 	HP revision /main/HPDCE02/1  1995/10/18  15:14 UTC  jss
 * 	Merge fix for CHFts16293.
 * 
 * 	HP revision /main/jss_dced_acl/1  1995/10/13  20:19 UTC  jss
 * 	Fix for CHFts16293. upkibml.c fixes for IBM big endian.
 * 	[1995/12/07  21:16:54  root]
 * 
 * Revision 1.1.8.2  1994/02/16  20:31:45  rico
 * 	Really enter previous fix for unmarshalling of IBM big-endian format
 * 	double-precision floating-point numbers on AIX RS6000 and HPUX.
 * 	[1994/02/16  20:31:19  rico]
 * 
 * Revision 1.1.8.1  1994/02/11  13:54:30  rico
 * 	Fix for unmarshalling of IBM big-endian format double-precision
 * 	floating-point numbers on AIX RS6000 and HPUX.
 * 	[1994/02/09  20:40:12  rico]
 * 
 * Revision 1.1.6.2  1993/07/07  20:11:32  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:39:54  ganni]
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
**      upkibml.c
**
**  FACILITY:
**
**      IDL Stub Runtime Support
**
**  ABSTRACT:
**
**      This module contains code to extract information from an IBM
**      long floating number and to initialize an UNPACKED_REAL structure
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
**  long floating number and to initialize an UNPACKED_REAL structure
**  with those bits.
**
**  See the header files for a description of the UNPACKED_REAL
**  structure.
**
**  A normalized IBM long floating number looks like:
**
**      [0]: Sign bit, 7 exp bits (bias 64), 24 fraction bits
**      [1]: 32 low order fraction bits
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

        memcpy(&r[1], input_value, 8);
#else
	memcpy(r, input_value, 8);

	/* Shuffle bytes to little endian format */

	r[2]  = ((r[1] << 24) | (r[1] >> 24));
	r[2] |= ((r[1] << 8) & 0x00FF0000L);
	r[2] |= ((r[1] >> 8) & 0x0000FF00L);
	r[1]  = ((r[0] << 24) | (r[0] >> 24));
	r[1] |= ((r[0] << 8) & 0x00FF0000L);
	r[1] |= ((r[0] >> 8) & 0x0000FF00L);

        /* Do the final word swap for little endian */
        r[0]=r[1];
        r[1]=r[2];
        r[2]=r[0];
#endif

	/* Initialize FLAGS and perhaps set NEGATIVE bit */

	r[U_R_FLAGS] = (r[1] >> 31);

	/* Clear sign bit */

	r[1] &= 0x7FFFFFFFL;

	if ((r[1] == 0) && (r[2] == 0)) {

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
			i += 8;
			r[1] |= (r[2] >> (32 - i));
			r[2] <<= i;

			/* Clear uninitialized part of unpacked real */

			r[3] = 0;
			r[4] = 0;

		}

	}
