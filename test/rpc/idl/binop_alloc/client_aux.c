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
 * Revision 1.1.4.2  1996/02/17  23:12:50  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:54  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:58:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:53  root]
 * 
 * Revision 1.1.2.3  1993/02/26  21:26:39  weisman
 * 	Removed binop_alloc_fail test.
 * 	[1993/02/26  21:10:25  weisman]
 * 
 * Revision 1.1.2.2  1993/01/07  19:08:37  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:28:03  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <binop_alloc.h>
#include <test_common.h>

#define CALLS_PER_PASS 1

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

    for (k = 1; k <= passes; k++) 
    {
        start_time = time(NULL);

        for (i = 1; i <= CALLS_PER_PASS; i++) 
        {
            binop_alloc_enable(h, i, i, &n);
            if (n != i+i)
            {
                printf("Two times %ld is NOT %ld\n", i, n);
                failures++;
            }

            binop_alloc_ss_enable(h, i, i, &n);
            if (n != i+i)
            {
                printf("Two times %ld is NOT %ld\n", i, n);
                failures++;
            }

        }                               /* for i */

        stop_time = time(NULL);

#ifdef DEBUG
        printf("pass %3d; real/call: %2ld ms\n", 
            k, ((stop_time - start_time) * 1000) / CALLS_PER_PASS);
#endif
    }

      return failures;
}
