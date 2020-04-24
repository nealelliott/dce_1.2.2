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
 * Revision 1.1.4.2  1996/02/17  23:13:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:12  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:00:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:29:34  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:49:37  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:30:17  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:24:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <context2.h>
#include <test_common.h>

extern int Pass;

#define NUM_HANDLES 32

static struct
{
    ch_t handle;
    ndr_ulong_int mapping;
} Handles[NUM_HANDLES];

int client_aux(h)
      handle_t h;
{
    int i, count, which, failures = 0;
    ndr_ulong_int tmp;

    printf("Getting new handles\n");
    count = 0;
    for (i = 0; i < NUM_HANDLES; i++)
    {
        /* Get context from [out] or return value */
        if (i & 1)
            rd_new(h, &Handles[i].handle);
        else
            Handles[i].handle = rd_new_return(h);

        if (Handles[i].handle) count++;
    }
#ifdef DEBUG
    printf("Got %d %s%\n", count, count == 1 ? "handle" : "handles");
#else
    printf("Got some handles\n");
#endif
    printf("Inquiring handles\n");
    for (i = NUM_HANDLES; i; i--)
    {
        if (Handles[i-1].handle)
            Handles[i-1].mapping = rd_inq(Handles[i-1].handle);
    }

    printf("Re-checking handles\n");
    for (i = 0; i < 4*NUM_HANDLES; i++)
    {
        which = random()%NUM_HANDLES;
        if (Handles[which].handle && 
            (tmp = rd_inq(Handles[which].handle)) != Handles[which].mapping)
        {
            printf("Got wrong mapping (%lx != %lx) for %lx!\n",
                   tmp, Handles[which].mapping, Handles[which].handle);
            failures++;
        }
    }

    printf("Closing handles\n");
    for (i = 0; i < NUM_HANDLES - 1; i++)
    {
        if (Handles[i].handle && rd_done(&Handles[i].handle))
        {
            printf("Error closing handle!\n");
            failures++;
        }
    }
    if (Handles[NUM_HANDLES-1].handle)
#ifdef DEBUG
        printf("Intentionally left handle %lx open\n", 
               Handles[NUM_HANDLES-1].mapping);
#else
        printf("Intentionally left one handle open\n");
#endif
    return failures;
}
