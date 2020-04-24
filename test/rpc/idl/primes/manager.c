/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:16:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:59  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:28:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:07  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:43:39  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:40:52  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <primes.h>
#include <test_common.h>

void _primes_gen(h, last, max, status, values)
handle_t h;
error_status_t *status;
ndr_long_int *last, max, values[];
{
    ndr_long_int n, highest = values[0], index = 0;

    for (n = 2; n <= highest; n++)
        if (is_prime(n)) 
        {
            values[index++] = n;
            if (index > max) break;
        }

    *last = index-1;
    *status = error_status_ok;
    return;
}

static int is_prime(n)
ndr_long_int n;
{
    int i;

    for (i = n/2; i > 1; i--)
        if (i*(n/i) == n) return 0;

    return 1;
}

globaldef primes_v0_0_epv_t primes_v0_0_m_epv = {_primes_gen};
