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
 * Revision 1.1.6.2  1996/02/17  23:12:30  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:42  marty]
 *
 * Revision 1.1.6.1  1995/12/11  19:56:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:20  root]
 * 
 * Revision 1.1.4.1  1994/02/01  22:00:23  cbrooks
 * 	OT 9858 - add dce.h to manager.c files
 * 	[1994/02/01  21:59:09  cbrooks]
 * 
 * Revision 1.1.2.2  1993/01/07  19:04:44  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:26:19  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:23:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <dce/dce.h>
#include <stdio.h>
#include <arrayptr.h>
#include <test_common.h>

static ndr_boolean arrayified_ref_iptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int s, ndr_long_int cs)
#else
 (h, pa, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int s;
ndr_long_int cs;
#endif
{
    ndr_boolean answer;
    ndr_long_int check2 = 0;
    int i;

    for (i = 0; i < s; i++) check2 += *(pa+i);
    if (answer = (check2 == cs)) ;
    else 
        printf("arrayified_ref_iptr_m: checksum doesn't match %d %d\n", cs, check2);

    return answer;
}

static void arrayified_ref_optr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int s, ndr_long_int *cs)
#else
 (h, pa, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int s;
ndr_long_int *cs;
#endif
{
    int i;

    *cs = 0;
    for (i = 0; i < s; i++)
        *cs += *(pa+i) = (random()&0x7fff); /* Make sure result fits in a short*/
#ifdef DEBUG
    printf ("arrayified_ref_optr_m, \*cs == %d\n", *cs);
#endif
}

static ndr_boolean arrayified_ref_ioptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int s, ndr_long_int *cs)
#else
 (h, pa, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int s;
ndr_long_int *cs;
#endif
{
    ndr_long_int check2 = 0;
    int i;

    if (pa == NULL) {
        printf ("arrayified_ref_iptr_m: pa == NULL\n");
        return false;
    }


    for (i = 0; i < s; i++) {
        check2 += *(pa+i);
    }

    if (check2 == *cs)
          ;
    else {
        printf("doesn't match %d %d\n", *cs, check2);
        return false;
    }

    *cs = 0;
    for (i = 0; i < s; i++)
        *cs += *(pa+i) = (random()&0x7fff);
    return true;
}

static ndr_boolean arrayified_null_iptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int s, ndr_long_int cs)
#else
 (h, pa, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int s;
ndr_long_int cs;
#endif
{
    ndr_boolean answer;

    if (!(answer = (pa == NULL))) 
        printf ("arrayified_null_iptr_m: pa != NULL\n");

    return answer;
}

static ndr_boolean arrayified_non_null_iptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int s, ndr_long_int cs)
#else
 (h, pa, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int s;
ndr_long_int cs;
#endif
{
    ndr_boolean answer;
    ndr_long_int check2 = 0;
    int i;

    if (!(answer = (pa != NULL))) 
        printf ("arrayified_non_null_iptr_m: pa == NULL\n");
    else
    {
#ifdef DEBUG
        printf ("arrayified_non_null_iptr_m: checksum ");
#endif
        for (i = 0; i < s; i++) check2 += *(pa+i);
        if (answer = (check2 == cs)) ;
        else printf("doesn't match\n");
    }

    return answer;
}

static ndr_boolean arrayified_null_ioptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int s, ndr_long_int *cs)
#else
 (h, pa, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int s;
ndr_long_int *cs;
#endif
{
    ndr_boolean answer;
    answer = (pa == NULL);
#ifdef DEBUG
    printf ("arrayified_null_ioptr_m: pa ");
    if (answer) 
        printf ("== NULL\n");
    else
        printf ("!= NULL\n");
#endif
    return answer;
}

static ndr_boolean arrayified_non_null_ioptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int s, ndr_long_int *cs)
#else
 (h, pa, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int s;
ndr_long_int *cs;
#endif
{
    ndr_long_int check2 = 0;
    int i;

    if (pa == NULL) {
        printf ("arrayified_non_null_iptr_m: pa == NULL\n");
        return false;
    }

#ifdef DEBUG
    printf ("arrayified_non_null_iptr_m: checksum ");
#endif
    for (i = 0; i < s; i++) check2 += *(pa+i);
    if (check2 == *cs)
          ;
    else {
        printf("doesn't match\n");
        return false;
    }

    *cs = 0;
    for (i = 0; i < s; i++)
        *cs += *(pa+i) = (random()&0x7fff);
#ifdef DEBUG
    printf ("arrayified_non_null_ioptr_m, \*cs == %d\n", *cs);
#endif
    return true;
}


static ndr_boolean varrayified_ref_iptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int l, ndr_short_int s, ndr_long_int cs)
#else
 (h, pa, l, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int l;
ndr_short_int s;
ndr_long_int cs;
#endif
{
    ndr_boolean answer;
    ndr_long_int check2 = 0;
    int i;

#ifdef DEBUG
    printf ("varrayified_ref_iptr_m: checksum ");
#endif
    for (i = 0; i < l; i++) check2 += *(pa+i);
    if (answer = (check2 == cs)) ;
    else 
        printf("doesn't match %d %d\n", cs, check2);

    return answer;
}

static void varrayified_ref_optr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int *l, ndr_short_int s, ndr_long_int *cs)
#else
 (h, pa, l, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int *l;
ndr_short_int s;
ndr_long_int *cs;
#endif
{
    int i;

    *l = (s == 0) ? 0 : random()%s;
    *cs = 0;
    for (i = 0; i < *l; i++)
        *cs += *(pa+i) = (random()&0x7fff);
#ifdef DEBUG
    printf ("varrayified_ref_optr_m, \*cs == %d\n", *cs);
#endif
}

static ndr_boolean varrayified_ref_ioptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int *l, ndr_short_int s, ndr_long_int *cs)
#else
 (h, pa, l, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int *l;
ndr_short_int s;
ndr_long_int *cs;
#endif
{
    ndr_long_int check2 = 0;
    int i;

    if (pa == NULL) {
        printf ("varrayified_ref_iptr_m: pa == NULL\n");
        return false;
    }

#ifdef DEBUG
    printf ("varrayified_ref_ioptr_m: checksum ");
#endif
    for (i = 0; i < *l; i++) {
        check2 += *(pa+i);
    }

    if (check2 == *cs)
        ;
    else {
        printf("doesn't match %d %d\n", *cs, check2);
        return false;
    }

    *cs = 0;
    *l = (s == 0) ? 0 : random()%s;
    for (i = 0; i < *l; i++)
        *cs += *(pa+i) = (random()&0x7fff);
#ifdef DEBUG
    printf ("varrayified_ref_ioptr_m, \*cs == %d\n", *cs);
#endif
    return true;
}

static ndr_boolean varrayified_null_iptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int l, ndr_short_int s, ndr_long_int cs)
#else
 (h, pa, l, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int l;
ndr_short_int s;
ndr_long_int cs;
#endif
{
    ndr_boolean answer;

    if (!(answer = (pa == NULL))) 
        printf ("varrayified_null_iptr_m: pa != NULL\n");

    return answer;
}

static ndr_boolean varrayified_non_null_iptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int l, ndr_short_int s, ndr_long_int cs)
#else
 (h, pa, l, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int l;
ndr_short_int s;
ndr_long_int cs;
#endif
{
    ndr_boolean answer;
    ndr_long_int check2 = 0;
    int i;

    if (!(answer = (pa != NULL))) 
        printf ("varrayified_non_null_iptr_m: pa == NULL\n");
    else
    {
#ifdef DEBUG
        printf ("varrayified_non_null_iptr_m: checksum ");
#endif
        for (i = 0; i < l; i++) check2 += *(pa+i);
        if (answer = (check2 == cs)) ;
        else printf("doesn't match\n");
    }

    return answer;
}

static ndr_boolean varrayified_null_ioptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int *l, ndr_short_int s, ndr_long_int *cs)
#else
 (h, pa, l, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int *l;
ndr_short_int s;
ndr_long_int *cs;
#endif
{
    ndr_boolean answer;
    answer = (pa == NULL);
#ifdef DEBUG
    printf ("varrayified_null_ioptr_m: pa ");
    if (answer) 
        printf ("== NULL\n");
    else
        printf ("!= NULL\n");
#endif
    return answer;
}

static ndr_boolean varrayified_non_null_ioptr_m
#ifdef _DCE_PROTO_
(handle_t h, ndr_short_int *pa, ndr_short_int *l, ndr_short_int s, ndr_long_int *cs)
#else
 (h, pa, l, s, cs)
handle_t h;
ndr_short_int *pa;
ndr_short_int *l;
ndr_short_int s;
ndr_long_int *cs;
#endif
{
    ndr_long_int check2 = 0;
    int i;

    if (pa == NULL) {
        printf ("varrayified_non_null_iptr_m: pa == NULL\n");
        return false;
    }

#ifdef DEBUG
    printf ("varrayified_non_null_iptr_m: checksum ");
#endif
    for (i = 0; i < *l; i++) check2 += *(pa+i);
    if (check2 == *cs)
          ;
    else {
        printf("doesn't match\n");
        return false;
    }

    *cs = 0;
    *l = random()%s;
    for (i = 0; i < *l; i++)
        *cs += *(pa+i) = (random()&0x7fff);
#ifdef DEBUG
    printf ("varrayified_non_null_ioptr_m, \*cs == %d\n", *cs);
#endif
    return true;
}


globaldef arrayptr_v0_0_epv_t arrayptr_v0_0_m_epv = {
    arrayified_ref_iptr_m,
    arrayified_ref_optr_m,
    arrayified_ref_ioptr_m,
    arrayified_null_iptr_m,
    arrayified_non_null_iptr_m,
    arrayified_null_ioptr_m,
    arrayified_non_null_ioptr_m,
    varrayified_ref_iptr_m,
    varrayified_ref_optr_m,
    varrayified_ref_ioptr_m,
    varrayified_null_iptr_m,
    varrayified_non_null_iptr_m,
    varrayified_null_ioptr_m,
    varrayified_non_null_ioptr_m
};

