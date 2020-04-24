/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crctest.c,v $
 * Revision 1.1.4.2  1996/02/18  00:09:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:48:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:08  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:21:50  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:03:42  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  crctest.c V=2 10/24/91 //littl/prgy/krb5/lib/crc-32
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/crc-32/crctest.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * CRC test driver program.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_crctest_c[] =
"$Id: crctest.c,v 1.1.4.2 1996/02/18 00:09:53 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/crc-32.h>
#include <stdio.h>

void
main()
{
    unsigned char ckout[4];
    krb5_checksum outck;

    char input[16], expected_crc[16];
    unsigned char inbytes[4], outbytes[4];
    int in_length;
    unsigned int expect;

    int bad = 0;

    outck.contents = ckout;

    while (scanf("%s %s", input, expected_crc) == 2) {
	in_length = strlen(input);
	if (in_length % 2) {
	    fprintf(stderr, "bad input '%s', not hex data\n", input);
	    exit(1);
	}
	in_length = in_length / 2;
	if (strlen(expected_crc) != 8) {
	    fprintf(stderr, "bad expectation '%s', not 8 chars\n",
		    expected_crc);
	    exit(1);
	}
	if (sscanf(expected_crc, "%lx",  &expect) != 1) {
	    fprintf(stderr, "bad expectation '%s', not 4bytes hex\n",
		    expected_crc);
	    exit(1);
	}
	outbytes[0] = expect & 0xff;
	outbytes[1] = (expect >> 8) & 0xff;
	outbytes[2] = (expect >> 16) & 0xff;
	outbytes[3] = (expect >> 24) & 0xff;

	if (sscanf(input, "%lx",  &expect) != 1) {
	    fprintf(stderr, "bad expectation '%s', not hex\n",
		    expected_crc);
	    exit(1);
	}
	inbytes[0] = expect & 0xff;
	inbytes[1] = (expect >> 8) & 0xff;
	inbytes[2] = (expect >> 16) & 0xff;
	inbytes[3] = (expect >> 24) & 0xff;

	(*crc32_cksumtable_entry.sum_func)((krb5_pointer)inbytes,
					   in_length, 0, 0, &outck);
	if (memcmp(outbytes, ckout, 4)) {
	    printf("mismatch: input '%s', output '%02x%02x%02x%02x', \
expected '%s'\n",
		   input, ckout[3], ckout[2], ckout[1], ckout[0],
		   expected_crc);
	    bad = 1;
	}	
    }
    if (bad) 
	printf("crctest: failed to pass the test\n");
    else
	printf("crctest: test is passed successfully\n");

    exit(bad);
}
