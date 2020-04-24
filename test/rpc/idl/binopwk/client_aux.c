/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.4.2  1996/02/17  23:12:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:59  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:58:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:57  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:08:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:28:17  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <binopwk.h>
#include <test_common.h>

#ifdef PRT
#define CALLS_PER_PASS 1
#else
#define CALLS_PER_PASS 1000
#endif

extern char *error_text();
globaldef uuid_t uuid_nil;
extern long time();

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    int failures = 0;
    error_status_t st;
    unsigned long llen;
    unsigned long port;
    ndr_long_int i, n;
    int k;
    long start_time, stop_time;

    for (k = 1; k <= 1; k++) 
    {
        start_time = time(NULL);

        for (i = 1; i <= CALLS_PER_PASS; i++) 
        {
            binopwk_add(h, i, i, &n);
            if (n != i+i)
            {
                printf("Two times %ld is NOT %ld\n", i, n);
                failures++;
            }
        }

        stop_time = time(NULL);

#ifdef DEBUG
        printf("pass %3d; real/call: %2ld ms\n", 
            k, ((stop_time - start_time) * 1000) / CALLS_PER_PASS);
#endif
    }

      return failures;
}
