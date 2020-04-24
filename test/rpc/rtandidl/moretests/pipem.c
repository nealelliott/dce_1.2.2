/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pipem.c,v $
 * Revision 1.1.4.2  1996/02/17  23:21:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:40:53  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:08:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:29  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:06:58  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:42:45  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:38:39  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:11:44  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:36:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#include <pipe.h>
#include <stdio.h>

/*
 * p i p e t e s t _ o p 1
 */
void pipetest_op1(h, foo, value)
handle_t h;
pipe_t foo;
ndr_long_int *value;
{
    ndr_long_int i, j;
    ndr_ulong_int count;
    ndr_long_int anarray[5000];
    ndr_long_int *myarray;

    i = count = 0;
    *value = 0;
    myarray = anarray;
    do
    {
        (*(foo.pull))(foo.state, myarray, 2048, &count);
        myarray += count;
        i += count;
    }while (count > 0);
    for (myarray = anarray, j = 0; j < i; ++myarray, ++j)
        *value += *myarray;
}

/*
 * p i p e t e s t _ o p 2
 */
void pipetest_op2(h, foo, value)
handle_t h;
pipe_t *foo;
ndr_long_int *value;
{
    ndr_long_int i, j;
    ndr_ulong_int count;
    ndr_long_int anarray[5000];
    ndr_long_int *myarray;

    i = count = 0;
    *value = 0;
    myarray = anarray;
    do
    {
        (*(foo->pull))(foo->state, myarray, 2048, &count);
        myarray += count;
        i += count;
    }while (count > 0);
    for (myarray = anarray, j = 0; j < i; ++myarray, ++j)
        *value += *myarray;
}

/*
 * p i p e t e s t _ o p 3
 */
void pipetest_op3(h, foo)
handle_t h;
pipe_t foo;
{
    ndr_long_int i;
    ndr_long_int count;
    ndr_long_int anarray[4090];
    ndr_long_int *myarray;

    for (i = 0; i < 4090; ++i)
        anarray[i] = 4089 - i;
    myarray = anarray;
    i = 4090;
    count = 2048;
    while (i > 0)
    {
        (*(foo.push))(foo.state, myarray, count);
        myarray += count;
        i -= count;
        if (i < count)
            count = i;
    }
    (*(foo.push))(foo.state, myarray, 0);
}

/*
 * p i p e t e s t _ o p 4
 */
void pipetest_op4(h, foo)
handle_t h;
pipe_t *foo;
{
    ndr_long_int i;
    ndr_long_int count;
    ndr_long_int anarray[4090];
    ndr_long_int *myarray;

    for (i = 0; i < 4090; ++i)
        anarray[i] = 4089 - i;
    myarray = anarray;
    i = 4090;
    count = 2048;
    while (i > 0)
    {
        (*(foo->push))(foo->state, myarray, count);
        myarray += count;
        i -= count;
        if (i < count)
            count = i;
    }
    (*(foo->push))(foo->state, myarray, 0);
}

/*
 * p i p e t e s t _ o p 5
 */
void pipetest_op5(h, foo, value)
handle_t h;
pipe_t foo;
ndr_long_int *value;
{
    ndr_long_int i, j;
    ndr_ulong_int count;
    ndr_long_int anarray[5000];
    ndr_long_int *myarray;

    i = count = 0;
    *value = 0;
    myarray = anarray;
    do
    {
        (*(foo.pull))(foo.state, myarray, 2048, &count);
        myarray += count;
        i += count;
    }while (count > 0);
    for (myarray = anarray, j = 0; j < i; ++myarray, ++j)
        *value += *myarray;

    for (i = 0; i < 4090; ++i)
        anarray[i] = 4089 - i;
    myarray = anarray;
    i = 4090;
    count = 2048;
    while (i > 0)
    {
        (*(foo.push))(foo.state, myarray, count);
        myarray += count;
        i -= count;
        if (i < count)
            count = i;
    }
    (*(foo.push))(foo.state, myarray, 0); 
}

/*
 * p i p e t e s t _ o p 6
 */
void pipetest_op6(h, foo, value)
handle_t h;
pipe_t foo;
ndr_long_int *value;
{
    ndr_long_int i, j;
    ndr_ulong_int count;
    ndr_long_int anarray[5000];
    ndr_long_int *myarray;

    myarray = anarray;
    i = 4090;
    count = 2048;
    while (i > 0)
    {
        (*(foo.push))(foo.state, myarray, count);
        myarray += count;
        i -= count;
        if (i < count)
            count = i;
    }
}

/*
 * p i p e t e s t _ o p 7
 */
void pipetest_op7(h, foo)
handle_t h;
pipe_t foo;
{
    ndr_long_int i;
    ndr_long_int count;
    ndr_long_int anarray[4090];
    ndr_long_int *myarray;

    for (i = 0; i < 4090; ++i)
        anarray[i] = 4089 - i;
    myarray = anarray;
    i = 4090;
    count = 2048;
    while (i > 0)
    {
        (*(foo.push))(foo.state, myarray, count);
        myarray += count;
        i -= count;
        if (i < count)
            count = i;
        (*(foo.push))(foo.state, myarray, 0);
    }
    (*(foo.push))(foo.state, myarray, count);
}

/*
 * p i p e t e s t _ o p 8
 */
void pipetest_op8(h, foo, value)
handle_t h;
pipe_t foo;
ndr_long_int *value;
{
    ndr_long_int i, j;
    ndr_ulong_int count;
    ndr_long_int anarray[5000];
    ndr_long_int *myarray;

    i = count = 0;
    *value = 0;
    myarray = anarray;
    do
    {
        (*(foo.pull))(foo.state, myarray, 2048, &count);
        myarray += count;
        i += count;
    }while (count > 0);
    count = 20;
    (*(foo.pull))(foo.state, myarray, 2048, &count);
}

/*
 * p i p e t e s t _ o p 9
 */
void pipetest_op9(h, foo, value)
handle_t h;
pipe_t foo;
ndr_long_int *value;
{
    ndr_ulong_int count;
    ndr_long_int anarray[5000];
    ndr_long_int *myarray;

    count = 0;
    myarray = anarray;
    (*(foo.pull))(foo.state, myarray, 0, &count);
}

/*
 * p i p e t e s t _ o p 8
 */
/*
void pipetest_op8(h, foo)
handle_t h;
pipe_t foo;
{
    ndr_long_int i;
    ndr_long_int count;
    ndr_long_int anarray[4090];
    ndr_long_int *myarray;

    for (i = 0; i < 4090; ++i)
        anarray[i] = 4089 - i;
    myarray = anarray;
    i = 4090;
    count = 2048;
    while (i > 0)
    {
        (*(foo.push))(foo.state, myarray, count);
        myarray += count;
        i -= count;
        if (i < count)
            count = i;
    }
}
*/
