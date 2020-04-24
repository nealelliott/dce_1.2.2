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
 * Revision 1.1.4.2  1996/02/17  23:14:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:02  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:08:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:38  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:58:32  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:34:43  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <ignore.h>
#include <test_common.h>

#define CALLS_PER_PASS 2

extern char *error_text();
globaldef uuid_t uuid_nil;
extern long time();

int client_aux(h,passes)
      handle_t h;
      int passes;
{
/*
 * Version of binopwk to test [ignore] attribute.
 */
    struct_t s;
    int failures = 0;
    ndr_long_int i;

    /* Put values in filler fields - stub code should not attempt access. */
    s.filler1 = (ndr_small_int *)0xAA;
    s.filler2 = (ndr_short_int *)0xBBBB;
    s.filler3 = (ndr_long_int *) 0xCCCCCCCC;
    s.filler4 = (ndr_hyper_int *)0xDDDDDDDD;

    for (i = 1; i <= CALLS_PER_PASS; i++) 
    {
        s.a = s.b = i;
        binopwk_add(h, &s);
        if (s.c != i+i)
        {
            printf("Two times %ld is NOT %ld\n", i, s.c);
            failures++;
        }
    }

    return failures;
}
