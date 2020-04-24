/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_xmit.c,v $
 * Revision 1.1.4.2  1996/02/17  23:18:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:20  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:46  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:40:47  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:46:53  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <test_common.h>
#include <xmit.h>


/* Transmit long_as_long_t as value + 5 */
void long_as_long_t_to_xmit(
	long_as_long_t *list,
	idl_ulong_int **value
)
{
      *value = malloc(sizeof(long_as_long_t));
      **value = *list + 5;
}

void long_as_long_t_from_xmit(
	idl_ulong_int *value,
	long_as_long_t *list
)
{
      *list = *value - 5;
}


void long_as_long_t_free_inst(long_as_long_t *list)
{
}

void long_as_long_t_free_xmit(idl_ulong_int *value)
{
      free(value);
}


/* Transmit long_as_struct_t as value + 5 */
void long_as_struct_t_to_xmit(
	long_as_struct_t *list,
	struct_t **value
)
{
      *value = (struct_t *)malloc(sizeof(struct_t));
      (*value)->next = NULL;
      (*value)->val = (long *)malloc(sizeof(long));
      *((*value)->val) = *list + 5;
}

void long_as_struct_t_from_xmit(value, list)
struct_t *value;
long_as_struct_t *list;
{
      *list = *(value->val) - 5;
}

void long_as_struct_t_free_inst(list)
long_as_struct_t *list;
{
}

void long_as_struct_t_free_xmit(value)
struct_t *value;
{
      free(value->val);
      free(value);
}


/* Transmit struct_as_long_t as value + 5 */
void struct_as_long_t_to_xmit(list, value)
struct_as_long_t *list;
long_t **value;
{
      *value = (long_t *)malloc(sizeof(long_t));
      **value = *list->val + 5;
}

void struct_as_long_t_from_xmit(value, list)
long_t *value;
struct_as_long_t *list;
{
      list->val = (long*)malloc(sizeof(long_t));
      *(list->val) = *value - 5;
      list->next = NULL;
}

void struct_as_long_t_free_inst(list)
struct_as_long_t *list;
{
}

void struct_as_long_t_free_xmit(value)
long_t *value;
{
      free(value);
}

