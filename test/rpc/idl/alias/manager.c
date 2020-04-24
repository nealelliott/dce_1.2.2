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
 * Revision 1.1.4.2  1996/02/17  23:12:09  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:30  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:55:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:50  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:01:18  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:24:38  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <alias.h>
#include <test_common.h>
#include <util_macros.h>

idl_long_int _op0
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ idl_long_int *a,
      /* [in, out] */ idl_long_int *b,
      /* [in, out] */ idl_long_int *c
)
#else
(h, a, b, c)
      /* [in] */ handle_t h;
      /* [in, out] */ idl_long_int *a;
      /* [in, out] */ idl_long_int *b;
      /* [in, out] */ idl_long_int *c;
#endif
{
    idl_long_int failures = 0;

    if (a != b || a != c)
    {
        fprintf(stderr, "Pointers passed to _op0 don't have same values!\n");
        failures++;
    }

    *a *= 2;
    *b *= 3;
    *c *= 4;
    return failures;
}

idl_long_int _op1
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ long_t *a,
      /* [in, out] */ long_t *b,
      /* [in, out] */ long_t *c
)
#else
(h, a, b, c)
      /* [in] */ handle_t h;
      /* [in, out] */ long_t *a;
      /* [in, out] */ long_t *b;
      /* [in, out] */ long_t *c;
#endif
{
    idl_long_int failures = 0;

    if (a != b || a != c)
    {
        fprintf(stderr, "Pointers passed to _op1 don't have same values!\n");
        failures++;
    }

    *a *= 2;
    *b *= 3;
    *c *= 4;
    return failures;
}

idl_long_int _op2
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ long_t *a,
      /* [in, out] */ long_t *b,
      /* [in, out] */ long_t *c,
      /* [in] */ idl_long_int s
)
#else
(h, a, b, c, s)
      /* [in] */ handle_t h;
      /* [in, out] */ long_t *a;
      /* [in, out] */ long_t *b;
      /* [in, out] */ long_t *c;
      /* [in] */ idl_long_int s;
#endif
{
    idl_long_int i, failures = 0;

    if (a != b || a != c)
    {
        fprintf(stderr, "Pointers passed to _op2 don't have same values!\n");
        failures++;
    }

    for (i = 0 ; i < s ; i++)
    {
        a[i] *= 2;
        b[i] *= 3;
        c[i] *= 4;
    }
    return failures;
}

idl_long_int _op3
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ node_t **plist
)
#else
(h, plist)
      /* [in] */ handle_t h;
      /* [in, out] */ node_t **plist;
#endif
{
    node_t *node_p, *temp_p;
    int toggle;

    /*
     * Reverse the circular doubly-linked list, creating new storage
     * for half the nodes.
     */
    if (*plist == NULL)
        return;

    /*
     * Make a first pass, replacing half the nodes with new nodes that
     * contain the same non-pointer data.
     */
    node_p = *plist;
    toggle = 0;
    do
    {
        if (toggle++)
        {
            temp_p = (node_t *)rpc_ss_allocate(sizeof(node_t));
            temp_p->l = node_p->l;
            temp_p->flink = node_p->flink;
            temp_p->blink = node_p->blink;
            node_p->blink->flink = temp_p;
            node_p->flink->blink = temp_p;
        }
        node_p = node_p->flink;
    } while (node_p != *plist);

    /*
     * Now reverse the list.
     */
    node_p = *plist;
    do
    {
        temp_p = node_p->flink;
        node_p->flink = node_p->blink;
        node_p->blink = temp_p;
        node_p = temp_p;
    } while (node_p != *plist);

    /*
     * Make the listhead point at the new first element.  node_p points at the
     * former first element which has been switched to be the last element, so
     * its forward link points at the new first element.
     */
    *plist = node_p->flink;

    return 0;
}

idl_long_int _op4
#ifdef NIDL_PROTOTYPES
(
      /* [in] */ handle_t h,
      /* [in, out] */ struct_list_t *list
)
#else
(h, list)
      /* [in] */ handle_t h;
      /* [in, out] */ struct_list_t *list;
#endif
{
    while (list != NULL)
    {
        if (list->struct_ptr) list->struct_ptr->l -= 7;
        list->struct_ref->l *= 3;
        list = list->next;
    }

    return 0;
}

globaldef alias_v0_0_epv_t alias_v0_0_m_epv = {
    _op0,
    _op1,
    _op2,
    _op3,
    _op4
};
