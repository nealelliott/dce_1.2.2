/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_cvt.c,v $
 * Revision 1.1.9.2  1996/03/09  20:51:46  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:28  marty]
 *
 * Revision 1.1.9.1  1995/12/13  21:56:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:31  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:52:52  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:52:25  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:43:13  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.3  1992/11/04  13:16:52  mgm
 * 	Ported to HPUX - added random and srandom
 * 	[1992/11/04  13:16:11  mgm]
 * 
 * Revision 1.1.3.2  1992/06/11  20:00:53  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:07:41  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:15:29  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:37:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <repas.h>

#ifdef __hpux
long random()
{
        return( (long)rand() );
}
void srandom( seed )
int seed;
{
        srand( (unsigned int)seed );
        return;
}
#endif

static void free_list_recursively();         /* auxiliary function */

void trans_t_from_local(list, xmit_struct)
list_t *list;
trans_t **xmit_struct;
{
    int count = 0;
    list_t *l = list;

    /* allocate the structure */
    *xmit_struct = (trans_t *)malloc(sizeof(trans_t));

    /* copy the values from the list to the array */
    while (l)
    {
        (*xmit_struct)->values[count++] = l->value;
        l = l->next;
    }
    (*xmit_struct)->last = (ndr_long_int)(count-1);
}

void trans_t_to_local(xmit_struct, list)
trans_t *xmit_struct;
list_t *list;
{
    int index = 0;

    /* reconstruct the linked list from the array */
    do
    {
        list->value = xmit_struct->values[index++];

        if (index <= xmit_struct->last)
            list->next = (list_t *)malloc(sizeof(list_t));
        else list->next = NULL;

        list = list->next;
    } while (index <= xmit_struct->last);
}

void trans_t_free_local(list)
list_t *list;
{
    if (list->next != NULL) 
       free_list_recursively(list->next);
}

void trans_t_free_inst(xmit_struct)
trans_t *xmit_struct;
{
    free(xmit_struct);
}

static void free_list_recursively(l)
list_t *l;
{
    if (l->next) free_list_recursively(l->next);
    free(l);
}

