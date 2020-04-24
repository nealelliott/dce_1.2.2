/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: rnd_confoun.c,v $
 * Revision 1.1.2.1  1996/06/05  20:31:17  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:30:18  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/crypto/os/rnd_confoun.c
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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

#include "k5-int.h"

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#ifdef TIME_WITH_SYS_TIME
#include <time.h>
#endif
#else
#include <time.h>
#endif

#ifdef HAVE_SRAND48
#define SRAND	srand48
#define RAND	lrand48
#define RAND_TYPE	long
#endif

#if !defined(RAND_TYPE) && defined(HAVE_SRAND)
#define SRAND	srand
#define RAND	rand
#define RAND_TYPE	int
#endif

#if !defined(RAND_TYPE) && defined(HAVE_SRANDOM)	
#define SRAND	srandom
#define RAND	random
#define RAND_TYPE	long
#endif

#if !defined(RAND_TYPE)
You need a random number generator!
#endif

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
    RAND_TYPE	rval;

    if (!seeded) {
	/* time() defined in 4.12.2.4, but returns a time_t, which is an
	   "arithmetic type" (4.12.1) */
	rval = (RAND_TYPE) time(0);
	SRAND(rval);
#ifdef HAVE_GETPID
	rval = RAND();
	rval ^= getpid();
	SRAND(rval);
#endif
	seeded = 1;
    }

    real_fill = (krb5_octet *)fillin;
    while (size > 0) {
	rval = RAND();
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
