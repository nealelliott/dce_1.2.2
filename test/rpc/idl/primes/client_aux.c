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
 * Revision 1.1.4.2  1996/02/17  23:16:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:57  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:28:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:06  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:43:31  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:40:49  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <primes.h>
#include <test_common.h>

#define MAXVALS    1000

extern char *error_text();

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    unsigned long port;
    ndr_long_int values[MAXVALS], last;
    char buf[100];
    int i;
    
    if (passes != 0)
        values[0] = passes;
    else
    {
        printf("Generate primes up to what integer: ");
        gets(buf);
        values[0] = (ndr_long_int)atoi(buf);
    }

    last = 0;        /* marshall only the first element of the array */

    primes_gen(h, &last, MAXVALS-1, &st, values);

    /* check comm_status value */
    if (st != error_status_ok)
    {
        fprintf(stderr, "Error in rpc - %s\n", error_text(st));
        exit(1);
    }

    printf("Primes are:\n");
    for (i = 0; i <= last; i++) printf("%d ", values[i]);

    printf("\n");
    return 0;
}

