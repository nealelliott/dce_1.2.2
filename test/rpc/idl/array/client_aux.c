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
 * Revision 1.1.4.2  1996/02/17  23:12:16  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:33  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:55:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:58  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:02:21  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:25:08  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <array.h>
#include <test_common.h>

static ndr_long_int arr[N];
static s_t sarr[N];
static t_t tarr[N];
static ndr_long_int (*oarr)[M];

int client_aux(handle_t h)
{
    ndr_long_int a,b,c;
    int i, j, checksum, failures = 0;
    ndr_long_int last, max, checkin, checkout;
    char buf[100];

    max = random()%N;
    oarr = malloc((max+1)*M*sizeof(ndr_long_int));

    for (i = 0; i < N; i++)
    {
        arr[i] = random();
        sarr[i].b = i & 0xff;
        sarr[i].l = random();
        tarr[i].b = i & 0xff;
        for (j = 0; j < 7; j++) tarr[i].a[j] = random();
    }

    for (i = 0; i < max; i++)
        for (j = 0; j < M; j++) oarr[i][j] = random();

    checksum = 0;
    for (i = 0; i < N; i++) checksum += arr[i];

    printf("Calling array_op1()\n");
    if (checksum == array_op1(h, arr)) ;
    else 
    {
        printf("*** Checksum 1 doesn't match! ***\n");
        failures++;
    }

    checksum = 0;
    for (i = 0; i < N; i++) checksum += sarr[i].l;

    printf("Calling array_op2()\n");
    if (checksum == array_op2(h, sarr)) printf("Checksum 2 matches.\n"); 
    else 
    {
        printf("*** Checksum 2 doesn't match! ***\n");
        failures++;
    }

    checksum = 0;
    for (i = 0; i < N; i++)
        for (j = 0; j < 7; j++) checksum += tarr[i].a[j];

    printf("Calling array_op3()\n");
    if (checksum == array_op3(h, tarr)) printf("Checksum 3 matches.\n");
    else 
    {
        printf("*** Checksum 3 doesn't match! ***\n");
        failures++;
    }

    last = random()%N;
    checksum = 0;
    for (i = 0; i <= last; i++) checksum += arr[i];
#ifdef DEBUG
    printf("Calling array_op4(last = %d)\n", last);
#endif
    if (checksum == array_op4(h, arr, last)) printf("Checksum 4 matches.\n");
    else 
    {
        printf("*** Checksum 4 doesn't match! ***\n");
        failures++;
    }

    checksum = 0;
    for (i = 0; i <= max; i++)
        for (j = 0; j < M; j++) 
        {
              checksum += oarr[i][j];
#ifdef DEBUG
              printf("c[%d,%d]: %d,%d\n", i,j,oarr[i][j], checksum);
#endif
        }
#ifdef DEBUG
    printf("Calling array_op5(max = %d)\n", max);
#endif
    if (checksum == array_op5(h, oarr, max)) printf("Checksum 5 matches.\n");
    else 
    {
        printf("*** Checksum 5 doesn't match! ***\n");
        failures++;
    }

    last = random()%max;
    checksum = 0;
    for (i = 0; i <= last; i++)
        for (j = 0; j < M; j++) checksum += oarr[i][j];
#ifdef DEBUG
    printf("Calling array_op6(last = %d, max = %d)\n", last, max);
#endif
    array_op6(h, oarr, &last, max, &checkin, &checkout);

    if (checksum == checkin) printf("Checkin 6 matches.\n");
    else 
    {
        printf("*** Checkin6 doesn't match! ***\n");
        failures++;
    }
    if (last == max) printf("last == max\n");
    else 
    {
        printf("*** last != max! ***\n");
        failures++;
    }

    checksum = 0;
    for (i = 0; i <= last; i++)
        for (j = 0; j < M; j++) checksum += oarr[i][j];

    if (checksum == checkout) printf("Checkout 6 matches.\n");
    else 
    {
        printf("*** Checkout6 doesn't match! ***\n");
        failures++;
    }

    free(oarr);

    return failures;
}
