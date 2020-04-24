/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: data_test_xmit.c,v $
 * Revision 1.1.7.3  1996/02/18  18:26:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:16  marty]
 *
 * Revision 1.1.7.2  1995/12/08  20:54:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:41  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:06:37  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:38:58  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/21  19:11:42  weir
 * 	Submit sample program sources (CR 12635)
 * 	[1994/10/21  19:10:54  weir]
 * 
 * $EndLog$
 */
/****************************************************************************/
/* [27.VI.94]                                                               */
/*                                                                          */
/* data_test_xmit.c -- The routines required to implement a [transmit_as]   */
/*                     type.                                                */
/*                                                                          */
/*                                                                          */
/*                                                                          */
/*     The parameters that these routines receive are derived from the      */
/*      transmit_as definition itself in the .idl file; the first para-     */
/*      meter is the "raw" form of the data, and the second form is the     */
/*      form it's to be transmitted as; the duty of the routines here is    */
/*      to translate back and forth from these forms.                       */
/*                                                                          */
/*                                                                          */
/*                            -77 cols-                                     */
/****************************************************************************/

#include "data_test.h"


/******
*
*
* sparse_array_t_to_xmit -- 
*
*
*    The to_xmit routine must allocate all space for the transmitted
*     type. In general, the stubs have no way to determine how to allocate
*     space for the transmitted type. Here, for example, the to_xmit
*     routine determines the size of a conformant array.
* 
* 
*******/

void sparse_array_t_to_xmit(
sparse_array_t *s_array,
compact_array_t **c_array
)
{
	unsigned32 i,j;
	unsigned32 csize;

	/* Count up the number of non-zero elements in the sparse array...  */

	for (i = 0, csize = 0; i < S_ARRAY_SIZE; i++)
	{
		if ((*s_array)[i] != 0)
		{
			csize++;
		}
	}

	/* Allocate a structure to hold the compact array...                */

	*c_array = (compact_array_t*)calloc(csize * 2 + 1, sizeof(unsigned32));
	(*c_array)->size = csize;

	/* Fill in the compact array from the non-zero elements...          */

	for (i = 0, j = 0; i < S_ARRAY_SIZE; i++)
	{
		if ((*s_array)[i] != 0)
		{
			(*c_array)->array[j].value = (*s_array)[i]; 
			(*c_array)->array[j++].subscript = i; 
		}
	}
}



/******
*
*
* sparse_array_t_from_xmit -- 
*
*
*   The from_xmit routine may not have to allocate any space for the
*    presented type. The presented type is always of a definite size
*    (conformant, varying, etc. types are not permitted), so the stub
*    provides an instance of the top level of the presented type. In 
*    this case, for example, s_array points to an instance of a sparse
*    array. If the presented type contains any pointers, the from_xmit
*    routine needs to allocate space for the referents and the free_inst
*    routine needs to free them.
*
*
*******/

void sparse_array_t_from_xmit(
compact_array_t *c_array,
sparse_array_t *s_array)
{
	unsigned32 i,j;

	for (i = 0; i < c_array->size; i++)
	{
		j = c_array->array[i].subscript;
		(*s_array)[j] = c_array->array[i].value;
	}
}



/******
*
*
* sparse_array_t_free_xmit -- called to free anything allocated by the
*                             to_xmit routine.
*
*
*******/

void sparse_array_t_free_xmit(
compact_array_t *c_array
)
{
	free(c_array);
}




/******
*
*
* sparse_array_t_free_inst -- called to free anything allocated by the
*                             from_xmit routine.
*
*   Since from_xmit doesn't allocate anything this is a null routine.
*
*******/

void sparse_array_t_free_inst(
sparse_array_t *s_array
)
{
  
}

