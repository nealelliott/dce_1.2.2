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
 * Revision 1.1.4.2  1996/02/17  23:15:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:27  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:29  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:38:00  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:38:09  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:26:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <parray.h>
#include <stdio.h>
#include <test_common.h>

static a_t arr;

int client_aux(h)
      handle_t h;
{
    a_t *old_atp, *atp = &arr;
    int i, j;
    int failures = 0; 
    ndr_long_int checksum, check2;

    checksum = 0;
    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            checksum += arr[i][j] = random();
    
    printf("Calling op1\n");

    if (op1(h, &atp, checksum))
    {
        printf("op1 failed.\n");
        failures++;
    }

    old_atp = atp;
    printf("Calling op2\n");
    if (op2(h, &atp, &checksum))
    {
        printf("op2 failed.\n");
        failures++;
    }

    if (atp != old_atp)
    {
        printf("Wrong array returned from op2!\n");
        failures++;
    }
    else printf("Correct return from op2\n");

    check2 = 0;
    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            check2 += (*atp)[i][j];

    if (check2 != checksum)
    {
        printf("op2 Checksum doesn't match! (%d != %d)\n", check2, checksum);
        failures++;
    }
    else printf("op2 Checksum matches\n");

    old_atp = atp;
    printf("Calling op3\n");
    if (op3(h, &atp, &checksum))
    {
        printf("op3 failed.\n");
        failures++;
    }

    if (atp == old_atp)
    {
        printf("Wrong array returned from op3!\n");
        failures++;
    }
    else printf("Correct return from op3\n");

    check2 = 0;
    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            check2 += (*atp)[i][j];

    if (check2 != checksum)
    {
        printf("op3 Checksum doesn't match! (%d != %d)\n", check2, checksum);
        failures++;
    }
    else printf("op3 Checksum matches\n");

    free((char *)atp);

    atp = NULL;
    checksum = 0;
    printf("Calling op4\n");
    if (op4(h, &atp, &checksum))
    {
        printf("op4 failed.\n");
        failures++;
    }

    if (atp == NULL)
    {
        printf("Wrong array returned from op4!\n");
        failures++;
    }
    else printf("Correct return from op4\n");

    check2 = 0;
    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            check2 += (*atp)[i][j];

    if (check2 != checksum)
    {
        printf("op4 Checksum doesn't match! (%d != %d)\n", check2, checksum);
        failures++;
    }
    else printf("op4 Checksum matches\n");

    checksum = 0;
    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            checksum += arr[i][j] = random();

    free((char *)atp);

    atp = &arr;
    printf("Calling op5\n");
    if (op5(h, &atp, &checksum))
    {
        printf("op5 failed.\n");
        failures++;
    }

    if (atp != NULL)
    {
        printf("Wrong array returned from op5!\n");
        failures++;
    }
    else printf("Correct return from op5\n");

    if (checksum != 0)
    {
        printf("op5 Checksum doesn't match! (0 != %d)\n", checksum);
        failures++;
    }
    else printf("op5 Checksum matches\n");

    printf("Calling op6\n");
    op6(h, &atp, &checksum);

    check2 = 0;
    for (i = 0; i < A; i++)
        for (j = 0; j < B; j++)
            check2 += (*atp)[i][j];

    if (check2 != checksum)
    {
        printf("op6 Checksum doesn't match! (%d != %d)\n", check2, checksum);
        failures++;
    }
    else printf("op6 Checksum matches\n");

    return failures;
}
