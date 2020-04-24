/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rnd_confoun.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:04  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:40  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:31:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:18:48  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rnd_confoun.c V=3 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/rnd_confoun.c,v $
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
 * krb5_random_confounder()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_rnd_counfoun_c[] =
"$Id: rnd_confoun.c,v 1.1.4.2 1996/02/18 00:15:46 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

/*
 * Generate a random confounder
 */
krb5_error_code
krb5_random_confounder(size, fillin)
int size;
krb5_pointer fillin;
{
    static int seeded = 0;
    register krb5_octet *real_fill; 

#ifdef __STDC__
    /* Use the srand/rand calls, see X3.159-1989, section 4.10.2 */
    if (!seeded) {
	/* time() defined in 4.12.2.4, but returns a time_t, which is an
	   "arithmetic type" (4.12.1) */
	srand((unsigned int) time(0));
	seeded = 1;
    }
#else
    /* assume Berkeley srandom...after all, this is libos! */
    if (!seeded) {
	srandom(time(0));
	seeded = 1;
    }
#endif
    real_fill = (krb5_octet *)fillin;
    while (size > 0) {

#ifdef __STDC__
	int rval;
	rval = rand();
	/* RAND_MAX is at least 32767, so we assume we can use the lower 16 bits
	   of the value of rand(). */
#else
	long rval;
	rval = random();
	/* BSD random number generator generates "in the range from
	   0 to (2**31)-1" (random(3)).  So we can use the bottom 16 bits. */
#endif
	*real_fill = rval & 0xff;
	real_fill++;
	size--;
	if (size) {
	    *real_fill = (rval >> 8) & 0xff;
	    real_fill++;
	    size--;
	}
    }
    return 0;
}
