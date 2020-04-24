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
 * Revision 1.1.4.2  1996/02/17  23:17:53  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:56  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:15  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:39:26  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:45:51  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <test_common.h>
#include <struct_ct_union.h>

#define CALLS_PER_PASS 20

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int i, failures;

    /*
     * Do a bunch of calls with various combos of discriminators.
     * The manager routine simply returns a copy of the input structure.
     */
    for (i = 0 ; i < CALLS_PER_PASS ; i++)
    {
#ifdef DEBUG
        printf("%d\r", i);
#endif
        failures = call_st_dup(h);
        if (failures != 0)
            break;
    }

    return failures;
}

int call_st_dup(h)
    handle_t h;
{
    /* Different structs cut out on the stack each time ct random junk data. */
    struct_t    is;
    struct_t    bs;
    struct_t    os;
    int failures = 0;
    int sw;

    /*
     * Fill benchmark struct with data.
     */
    sw = rand() % 3;
    bs.c = rand() % 256;
    bs.mu.sw = rand() % 2;
    bs.d = rand() % 10000000;
    bs.lu.sw = rand() % 4;

    switch (sw)
    {
    case 0: bs.fu.sw = 'l'; bs.fu.tagged_union.l = rand();         break;
    case 1: bs.fu.sw = 's'; bs.fu.tagged_union.s = rand() % 65536; break;
    case 2: bs.fu.sw = 'c'; bs.fu.tagged_union.c = rand() % 256;   break;
    }

    switch (bs.mu.sw)
    {
    case 0: break;
    case 1:
        bs.mu.tagged_union.struc.l = rand();
        sw = rand() % 3;
        bs.mu.tagged_union.struc.s = rand() % 65536;
        bs.mu.tagged_union.struc.lu.sw = rand() % 4;
        bs.mu.tagged_union.struc.d = rand() % 10000000;

        switch (sw)
        {
        case 0:
            bs.mu.tagged_union.struc.fu.sw = 'l';
            bs.mu.tagged_union.struc.fu.tagged_union.l = rand();
            break;
        case 1:
            bs.mu.tagged_union.struc.fu.sw = 's';
            bs.mu.tagged_union.struc.fu.tagged_union.s = rand() % 65536;
            break;
        case 2:
            bs.mu.tagged_union.struc.fu.sw = 'c';
            bs.mu.tagged_union.struc.fu.tagged_union.c = rand() % 256;
            break;
        }

        switch (bs.mu.tagged_union.struc.lu.sw)
        {
        case 0: bs.mu.tagged_union.struc.lu.tagged_union.c = rand() % 256;    break;
        case 1: bs.mu.tagged_union.struc.lu.tagged_union.s = rand() % 65536;  break;
        case 2: bs.mu.tagged_union.struc.lu.tagged_union.l = rand();          break;
        case 3: bs.mu.tagged_union.struc.lu.tagged_union.d = rand()%10000000; break;
        }
    }

    switch (bs.lu.sw)
    {
    case 0: bs.lu.tagged_union.c = rand() % 256;      break;
    case 1: bs.lu.tagged_union.s = rand() % 65536;    break;
    case 2: bs.lu.tagged_union.l = rand();            break;
    case 3: bs.lu.tagged_union.d = rand() % 10000000; break;
    }

    /*
     * Copy benchmark struct to input struct.
     */
    memcpy(&is, &bs, sizeof(is));

    /*
     * Remote call to duplicate the input struct.
     */
    st_dup(h, &is, &os);

    /*
     * Input struct should be unchanged.
     */
    if (memcmp(&is, &bs, sizeof(is)) != 0)
    {
        fprintf(stderr, "Failure: input structure was overwritten\n");
        failures++;
    }

    /*
     * Output struct should be field for field equal to input struct.
     */
    if (os.fu.sw != is.fu.sw)
    {
        fprintf(stderr, "Discriminators of first union do not agree.\n");
        failures++;
    }

    if (os.c != is.c)
    {
        fprintf(stderr, "Character fields of struct do not agree.\n");
        failures++;
    }

    if (os.mu.sw != is.mu.sw)
    {
        fprintf(stderr, "Discriminators of middle union do not agree.\n");
        failures++;
    }

    if (os.d != is.d)
    {
        fprintf(stderr, "Double fields of struct do not agree.\n");
        failures++;
    }

    if (os.lu.sw != is.lu.sw)
    {
        fprintf(stderr, "Discriminators of last union do not agree.\n");
        failures++;
    }

    switch (is.fu.sw)
    {
    case 'l':
        if (os.fu.tagged_union.l != is.fu.tagged_union.l)
        {
            fprintf(stderr, "long arms of first union do not agree.\n");
            failures++;
        }
        break;
    case 's':
        if (os.fu.tagged_union.s != is.fu.tagged_union.s)
        {
            fprintf(stderr, "short arms of first union do not agree.\n");
            failures++;
        }
        break;
    case 'c':
        if (os.fu.tagged_union.c != is.fu.tagged_union.c)
        {
            fprintf(stderr, "char arms of first union do not agree.\n");
            failures++;
        }
        break;
    }

    switch (is.mu.sw)
    {
    case 0: break;
    case 1:
        if (os.mu.tagged_union.struc.l != is.mu.tagged_union.struc.l)
        {
            fprintf(stderr, "long fields of embedded struct do not agree.\n");
            failures++;
        }

        if (os.mu.tagged_union.struc.fu.sw != is.mu.tagged_union.struc.fu.sw)
        {
            fprintf(stderr, "Discriminators of embedded first union do not agree.\n");
            failures++;
        }

        if (os.mu.tagged_union.struc.s != is.mu.tagged_union.struc.s)
        {
            fprintf(stderr, "short fields of embedded struct do not agree.\n");
            failures++;
        }

        if (os.mu.tagged_union.struc.lu.sw != is.mu.tagged_union.struc.lu.sw)
        {
            fprintf(stderr, "Discriminators of embedded last union do not agree.\n");
            failures++;
        }

        if (os.mu.tagged_union.struc.d != is.mu.tagged_union.struc.d)
        {
            fprintf(stderr, "double fields of embedded struct do not agree.\n");
            failures++;
        }
    }

    switch (is.lu.sw)
    {
    case 0:
        if (os.lu.tagged_union.c != is.lu.tagged_union.c)
        {
            fprintf(stderr, "char arms of last union do not agree.\n");
            failures++;
        }
        break;
    case 1:
        if (os.lu.tagged_union.s != is.lu.tagged_union.s)
        {
            fprintf(stderr, "short arms of last union do not agree.\n");
            failures++;
        }
        break;
    case 2:
        if (os.lu.tagged_union.l != is.lu.tagged_union.l)
        {
            fprintf(stderr, "long arms of last union do not agree.\n");
            failures++;
        }
        break;
    case 3:
        if (os.lu.tagged_union.d != is.lu.tagged_union.d)
        {
            fprintf(stderr, "double arms of last union do not agree.\n");
            failures++;
        }
        break;
    }

    return failures;
}
