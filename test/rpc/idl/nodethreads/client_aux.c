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
 * Revision 1.1.4.2  1996/02/17  23:15:12  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:12  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:25:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:13:56  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:31:14  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:35:31  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <test_common.h>
#include <nodethreads.h>


int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    list_element *p_list;
    list_element *new_elt;
    int i;

    p_list = NULL;
    for (i=1; i<=10; i++)
    {
        new_elt = (list_element *)malloc(sizeof(list_element));
        new_elt->link = p_list;
        new_elt->value = 256 * i;
        new_elt->p_value_1 = NULL;
        new_elt->p_value_2 = NULL;
        new_elt->p_value_3 = NULL;
        p_list = new_elt;
    }

   my_oper ( h, p_list );

    while (p_list != NULL)
    {
        printf("Value %d\n", p_list->value);
        if ( *(p_list->p_value_1) != p_list->value )
        {
            printf("Error at p_value_1\n");
            exit(0);
        }
        if ( *(p_list->p_value_2) != p_list->value )
        {
            printf("Error at p_value_2\n");
            exit(0);
        }
        if ( *(p_list->p_value_3) != p_list->value )
        {
            printf("Error at p_value_3\n");
            exit(0);
        }
        p_list = p_list->link;
    }
 
    printf("Test successful\n");

    return 0;
}
