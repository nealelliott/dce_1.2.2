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
 * Revision 1.1.8.2  1996/02/17  23:12:27  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:39  marty]
 *
 * Revision 1.1.8.1  1995/12/11  19:56:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:28:13  root]
 * 
 * Revision 1.1.6.2  1993/06/15  21:04:49  sommerfeld
 * 	Do not declare random(); let test_common.h do it.
 * 	[1993/06/14  20:48:06  sommerfeld]
 * 
 * Revision 1.1.2.2  1993/01/07  19:04:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:26:01  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <arrayp.h>
#include <stdio.h>
#include <test_common.h>

extern int Pass;

int client_aux(h)
      handle_t h;
{
    int i, j, k, tmp, failures = 0;
    char name[MAXNAME];
    ndr_long_int size, check1, check2, sum1, sum2;
    ndr_short_int namelen;
    cp_t *carray;
    long_t **larray;
    s_t *sarray;
    char_t *savec;
    long_t *savel;

    size = 2+random()%1000;
#ifdef DEBUG
    printf("Generating arrays with %d elements\n", size);
#endif

    carray = (cp_t *)malloc(size*sizeof(cp_t));
    larray = (long_t **)malloc(size*sizeof(long_t *));

    check1 = check2 = 0;
    for (i = 1; i < size; i++)
    {
        carray[i].one = random();
        check1 += carray[i].one;
        carray[i].two = (char_t *)malloc(sizeof(char_t));
        carray[i].two->c = random();
        check1 += carray[i].two->c;
        carray[i].three = random();
        check1 += carray[i].three;

        larray[i] = (long_t *)malloc(sizeof(long_t));
        larray[i]->l = random();
        check2 += larray[i]->l;
    }

    carray[0].two = NULL;
    larray[0] = NULL;

    savec = carray[size-1].two;
    savel = larray[size-1];

#ifdef DEBUG
    printf("Calling op0()\n");
#endif
    if (tmp = op0(h, size, check1, check2, carray, larray)) 
    {
        printf("*** op0 failed! ***\n");
        failures += tmp;
    }

    if (carray[size-1].two != NULL)
    {
        printf("*** carray[%d].two != NULL!\n", size-1);
        failures++;
    }
    if (larray[size-1] != NULL)
    {
        printf("*** larray[%d] != NULL!\n", size-1);
        failures++;
    }

    if (carray[0].two != savec)
    {
        printf("*** carray[0].two != savec!\n");
        failures++;
    }

    if (larray[0]!= savel)
    {
        printf("*** carray[0].two != savel!\n");
        failures++;
    }

    sum1 = sum2 = 0;
    for (i = 0; i < size-1; i++)
    {
        sum1 += carray[i].one;
        sum1 += carray[i].two->c;
        sum1 += carray[i].three;
        sum2 += larray[i]->l;
        free(carray[i].two);
        free(larray[i]);
    }

    if (sum1 != check1)
    {
        printf("sum1 != check1 (%d != %d)\n", sum1, check1);
        failures++;
    }

    if (sum2 != check2)
    {
        printf("sum2 != check2 (%d != %d)\n", sum2, check2);
        failures++;
    }

    sarray = (s_t *)malloc(size*sizeof(s_t));
    for (i = 0; i < size; i++)
    {
        namelen = 1+random()%(MAXNAME-1);
        sarray[i].namelen = namelen;
        sarray[i].checksum = 0;

        for (k = 0; k < namelen; k++) name[k] = 'A' + random()%('z'-'A');
        name[namelen-1] = '\0';
        for (k = 0; k < namelen; k++) sarray[i].checksum += name[k];

        sarray[i].body.i = i%3;
        switch (sarray[i].body.i)
        {
            case 0:
                strcpy(sarray[i].body.tag.name, name);
                break;
            case 1:
                for (k = 0; k < MAXNAME; k++)
                    if (k < namelen)
                    {
                        sarray[i].body.tag.namep[k] = (ndr_char *)malloc(1);
                        *(sarray[i].body.tag.namep[k]) = name[k];
                    }
                    else sarray[i].body.tag.namep[k] = NULL;
                break;
        }
    }
#ifdef DEBUG
    printf("Calling op1()\n");
#endif
    if (tmp = op1(h, size, sarray))
    {
        printf("*** op1 failed! ***\n");
        failures += tmp;
    }

    for (i = 0; i < size; i++)
    {
        switch (sarray[i].body.i)
        {
            case 1:
                for (k = 0; k < MAXNAME; k++)
                    if (sarray[i].body.tag.namep[k])
                        free(sarray[i].body.tag.namep[k]);
        }
    }

	free(carray);
	free(larray);

    return failures;
}
