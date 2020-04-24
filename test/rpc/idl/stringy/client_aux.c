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
 * Revision 1.1.4.2  1996/02/17  23:17:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:48  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:08  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:39:06  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:45:36  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stringy.h>
#include <stdio.h>
#include <test_common.h>

#define MAXLEN 2000


static int is_reversed(s, r)
char *s;
char *r;
{
    int i, len1 = strlen(s), len2 = strlen(r);

    if (len1 != len2) return 0;

    for (i = 0; i < len1; i++)
        if (s[i] != r[len1-i-1]) return 0;

    return 1;
}

static int is_palindrome(s, r)
char *s;
char *r;
{
    int i, len1 = strlen(s), len2 = strlen(r);

    if (len1 != 2*len2-1) return 0;
    for (i = 0; i < len2; i++)
        if (r[i] != s[i] || r[i] != s[len1-i-1]) return 0;

    return 1;
}

static char reference[1+MAXLEN];
static char arev[1+MAXLEN], apal[2*MAXLEN+1];

int client_aux(h)
handle_t h;
{
    int i, len, failures = 0;
    ndr_long_int size;
    char *prev, *ppal;

    len = random()%MAXLEN;
    size = 1+2*len;

    for (i = 0; i < len; i++) reference[i] = 'A' + random()%('z'-'A');
    reference[len] = '\0';

    prev = malloc(1+len);
    strcpy(prev, reference);

    printf("Calling op0\n");
    op0(h, (ndr_char *)prev);
    if (!is_reversed(prev, reference))
    {
        printf("op0 failed!\n");
        failures++;
    }

    
    ppal = malloc(size);
    strcpy(ppal, reference);

    printf("Calling op1\n");
    op1(h, (ndr_char *)ppal, size);
    if (!is_palindrome(ppal, reference))
    {
        printf("op1 failed!\n");
        failures++;
    }

    strcpy(prev, reference);
    printf("Calling op2\n");
    op2(h, (ndr_char *)prev);
    if (!is_reversed(prev, reference))
    {
        printf("op2 failed!\n");
        failures++;
    }

    strcpy(ppal, reference);
    printf("Calling op3\n");
    op3(h, (ndr_char *)ppal, size);
    if (!is_palindrome(ppal, reference))
    {
        printf("op3 failed!\n");
        failures++;
    }

    free(prev);
    free(ppal);

    strcpy(arev, reference);
    printf("Calling op4\n");
    op4(h, (ndr_char *)arev);
    if (!is_reversed(arev, reference))
    {
        printf("op4 failed!\n");
        failures++;
    }

    strcpy(apal, reference);
    printf("Calling op5\n");
    op5(h, (ndr_char *)apal, size);
    if (!is_palindrome(apal, reference))
    {
        printf("op5 failed!\n");
        failures++;
    }

    return failures;
}

