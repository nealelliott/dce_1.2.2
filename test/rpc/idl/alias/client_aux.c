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
 * Revision 1.1.4.2  1996/02/17  23:12:04  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:30  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:54:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:49  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:01:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:24:35  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <math.h>
#include <test_common.h>
#include <alias.h>
#include <util_macros.h>

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int             i, failures = 0, mgr_failures = 0;
    idl_long_int    *longp1, *longp2, *longp3, aliased_long, output_long;
    long_t          *longtp1, *longtp2, *longtp3, aliased_longt, output_longt;
    long_t          *longap1, *longap2, *longap3, aliased_longa[ARRAY_SIZE],
                    output_longa[ARRAY_SIZE];
    node_t          *list = NULL, *node_p;
    struct_list_t   *struct_list_p, *temp;
    struct_t        *struct_ptr;

/*
 * Top-level pointers - pointers can't change but pointees can be aliased.
 * op0 tests the pre-generated routines for pointed-to long that ships with RTL
 * op1 tests the IDL-generated routines for pointed-to long from compiling this
 */
    longp1 = &aliased_long;
    longp2 = &aliased_long;
    longp3 = &aliased_long;
    aliased_long = random();
    output_long = aliased_long * 2 * 3 * 4;
    CALL(op0, (h, longp1, longp2, longp3), mgr_failures, failures);
    if (aliased_long != output_long)
        FAIL("Top-level pointer aliasing failed in op0\n", failures);

    longtp1 = &aliased_longt;
    longtp2 = &aliased_longt;
    longtp3 = &aliased_longt;
    aliased_longt = random();
    output_longt = aliased_longt * 2 * 3 * 4;
    CALL(op1, (h, longtp1, longtp2, longtp3), mgr_failures, failures);
    if (aliased_longt != output_longt)
        FAIL("Top-level pointer aliasing failed in op1\n", failures);

/*
 * Top-level pointers to aliased conformant objects.
 */
    longap1 = aliased_longa;
    longap2 = aliased_longa;
    longap3 = aliased_longa;
    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        aliased_longa[i] = random();
        output_longa[i] = aliased_longa[i] * 2 * 3 * 4;
    }
    CALL(op2, (h, longap1, longap2, longap3, ARRAY_SIZE), mgr_failures, failures);
    for (i = 0 ; i < ARRAY_SIZE ; i++)
    {
        if (aliased_longa[i] != output_longa[i])
            FAIL_BREAK("Top-level pointer aliasing failed in op2\n", failures);
    }

/*
 * A circular doubly-linked list.
 */
    for (i = NUM_NODES ; i > 0 ; i--)
    {
        /* Insert new entry at head. */
        MALLOC(node_p, sizeof(node_t));
        node_p->l = i * i;
        node_p->flink = list;
        if (list == NULL)
            node_p->blink = node_p->flink = node_p;
        else
        {
            /* Point forward link of new node at current first node. */
            node_p->flink = list;
            /* Point backward link of new node at current last node. */
            node_p->blink = list->blink;
            /* Point current last node forward link at new node. */
            list->blink->flink = node_p;
            /* Point current first node backward link at new node. */
            list->blink = node_p;
        }
        /* Finally, update listhead to point at new node. */
        list = node_p;
    }
    /* Manager routine reverses the list. */
    CALL(op3, (h, &list), mgr_failures, failures);
    /* Check accessibility via blinks. */
    if (list == NULL)
    {
        FAIL("Out value of ptr to doubly-linked list NULL in op3\n", failures);
    }
    else
    {
        /* Check accessibility via blinks. */
        i = 0;
        node_p = list->blink;
        do
        {
            i++;
            if (node_p->l != i * i)
                FAIL_BREAK("Aliasing in doubly-linked list failed in op3\n", failures);
            node_p = node_p->blink;
        } while (node_p != list->blink);

        /* Check accessibility via flinks. */
        i = NUM_NODES;
        node_p = list;
        do
        {
            if (node_p->l != i * i)
                FAIL_BREAK("Aliasing in doubly-linked list failed in op3\n", failures);
            node_p = node_p->flink;
            i--;
        } while (node_p != list);
    }

/*
 * Aliasing with mixed ref and ptr pointers to the same type.
 */
    MALLOC(struct_ptr, sizeof(struct_t));
    struct_ptr->l = 0;
    struct_list_p = NULL;
    for (i = LIST_SIZE ; i > 0 ; i--)
    {
        MALLOC(temp, sizeof(struct_list_t));
        temp->next = struct_list_p;
        MALLOC(temp->struct_ref, sizeof(struct_t));
        temp->struct_ref->l = i * 2;
        temp->struct_ptr = struct_ptr;
        struct_list_p = temp;
    }
    CALL(op4, (h, struct_list_p), mgr_failures, failures);
    i = 0;
    do
    {
        i++;
        if (struct_list_p->struct_ref->l != i * 2 * 3
            ||  struct_list_p->struct_ptr == NULL
            ||  struct_list_p->struct_ptr->l != LIST_SIZE * -7)
            FAIL_BREAK("ref/ptr aliasing failed in op4\n", failures);
        struct_list_p = struct_list_p->next;
    } while (struct_list_p != NULL);

    if (failures != 0)
        fprintf(stderr, "%d failures.\n", failures);

    return failures;
}
