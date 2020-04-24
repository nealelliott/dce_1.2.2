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
 * Revision 1.1.4.2  1996/02/17  23:12:42  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:47  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:57:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:36  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:06:45  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:27:16  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <arrayv1.h>
#include <test_common.h>

static idl_long_int arr[N];

static idl_long_int gen_array(arr, last)
idl_long_int *arr;
idl_long_int last;
{
    int i;
    idl_long_int checksum = 0;

    for (i = 0; i <= last; i++)
        checksum += arr[i] = random();

    return checksum;
}

static idl_long_int check_array(arr, last)
idl_long_int *arr;
idl_long_int last;
{
    int i;
    idl_long_int checksum = 0;

    for (i = 0; i <= last; i++)
        checksum += arr[i];

    return checksum;
}

int client_aux(h)
      handle_t h;
{
    idl_long_int last, max, checksum, check2;
    idl_long_int *oarr;
    idl_usmall_int tmp;
    int i, failures = 0;

    checksum = gen_array(arr, N-1);
    printf("Calling av1_op0\n");
    if (tmp = av1_op0(h, arr, &checksum))
    {
        printf("av1_op0 failed!\n");
        failures += tmp;
    }
    else printf("av1_op0 succeeded\n");

    if ((check2 = check_array(arr, N-1)) != checksum)
    {
        printf("checksums don't match (%d != %d)!\n", checksum, check2);
        failures++;
    }
    else printf("checksums match\n", checksum, check2);

    last = random()%N;
    checksum = gen_array(arr, last);
#ifdef DEBUG
    printf("Calling av1_op1(last = %d)\n", last);
#endif
    if (tmp = av1_op1(h, arr, &last, &checksum))
    {
        printf("av1_op1 failed!\n");
        failures += tmp;
    }
    else printf("av1_op1 succeeded\n");

    if ((check2 = check_array(arr, last)) != checksum)
    {
        printf("checksums don't match (%d != %d)!\n", checksum, check2);
        failures++;
    }
    else printf("checksums match\n");

    last = -1;
    checksum = 123456789;
#ifdef DEBUG
    printf("Calling av1_op1(last = %d)\n", last);
#endif
    if (tmp = av1_op1(h, arr, &last, &checksum))
    {
        printf("av1_op1 failed!\n");
        failures += tmp;
    }
    else printf("av1_op1 succeeded\n");

    if (last != -1 || checksum != 987654321)
    {
        printf("bad last(%d) and/or checksum(%d)!\n", last, checksum);
        failures++;
    }
    else printf("good last and checksum\n");

    max = random()%10000;
    last = random()%max;
    oarr = (idl_long_int *)malloc((max+1)*sizeof(idl_long_int));

    checksum = gen_array(oarr, last);
#ifdef DEBUG
    printf("Calling av1_op2(last = %d)\n", last);
#endif
    if (tmp = av1_op2(h, oarr, &last, max, &checksum))
    {
        printf("av1_op2 failed!\n");
        failures += tmp;
    }
    else printf("av1_op2 succeeded\n");

    if ((check2 = check_array(oarr, last)) != checksum)
    {
        printf("checksums don't match (%d != %d)!\n", checksum, check2);
        failures++;
    }
    else printf("checksums match\n");

    last = -1;
    checksum = 123456789;
#ifdef DEBUG
    printf("Calling av1_op2(last = %d)\n", last);
#endif
    if (tmp = av1_op2(h, oarr, &last, max, &checksum))
    {
        printf("av1_op2 failed!\n");
        failures += tmp;
    }
    else printf("av1_op2 succeeded\n");

    if (last != -1 || checksum != 987654321)
    {
        printf("bad last(%d) and/or checksum(%d)!\n", last, checksum);
        failures++;
    }
    else printf("good last and checksum\n");
    free(oarr);

    last = random()%10000;
    oarr = (idl_long_int *)malloc((last+1)*sizeof(idl_long_int));

    checksum = gen_array(oarr, last);
#ifdef DEBUG
    printf("Calling av1_op3(last = %d)\n", last);
#endif
    if (tmp = av1_op3(h, oarr, &last, &checksum))
    {
        printf("av1_op3 failed!\n");
        failures += tmp;
    }
    else printf("av1_op3 succeeded\n");

    if ((check2 = check_array(oarr, last)) != checksum)
    {
        printf("checksums don't match (%d != %d)!\n", checksum, check2);
        failures++;
    }
    else printf("checksums match\n");

    last = -1;
    checksum = 123456789;
#ifdef DEBUG
    printf("Calling av1_op3(last = %d)\n", last);
#endif
    if (tmp = av1_op3(h, oarr, &last, &checksum))
    {
        printf("av1_op3 failed!\n");
        failures += tmp;
    }
    else printf("av1_op3 succeeded\n");

    if (last != -1 || checksum != 987654321)
    {
        printf("bad last(%d) and/or checksum(%d)!\n", last, checksum);
        failures++;
    }
    else printf("good last and checksum\n");

    return failures;
}
