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
 * Revision 1.1.4.2  1996/02/17  23:15:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:23  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:20  root]
 * 
 * Revision 1.1.2.2  1993/01/07  20:36:16  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:37:26  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <out_refs.h>
#include <test_common.h>

void context_handle_t_rundown(ctx)
      context_handle_t ctx;
{
      printf("Running down context: %X\n",ctx);
}



void _comm_status_t_op(st)
      error_status_t     *st;
{
    *st = error_status_ok;
}

void _context_handle_op(a)
      context_handle_t            *a;
{
      *a = NULL;
}

void _boolean_op(a)
      ndr_boolean                     *a;
{
    *a = boolean_return_val;
}

void _char_op(a)
      ndr_char                        *a;
{
      *a = char_return_val;
}

void _byte_op(a)
      ndr_byte                        *a;
{
      *a = byte_return_val;
}

void _float_op(a)
      ndr_short_float                       *a;
{
      *a = float_return_val;
}

void _double_op(a)
      ndr_long_float *a;
{
      *a = double_return_val;
}

void _small_op(a)
      ndr_small_int                       *a;
{
      *a = small_return_val;
}

void _short_op(a)
      ndr_short_int                       *a;
{
      *a = short_return_val;
}

void _long_op(a)
      ndr_long_int                        *a;
{
      *a = long_return_val;
}

void _hyper_op(a)
      ndr_hyper_int                       *a;
{
    a->low = hyper_low_return_val;
    a->high = hyper_high_return_val;
}

void _usmall_op(a)
      ndr_usmall_int              *a;
{
      *a = usmall_return_val;
}

void _ushort_op(a)
      ndr_ushort_int              *a;
{
      *a = ushort_return_val;
}

void _ulong_op(a)
      ndr_ulong_int               *a;
{
      *a = ulong_return_val;
}

void _uhyper_op(a)
      ndr_uhyper_int              *a;
{
    a->low = uhyper_low_return_val;
    a->high = uhyper_high_return_val;
}

void _fixed_array_t_op(a)
      fixed_array_t *a;
{
    int i;
    for (i = 0; i < fixed_array_size; i++) (*a)[i] = i;
}

void _fixed_struct_t_op(a)
       fixed_struct_t *a;
{
    int i;
    a->size = fixed_array_size;
    for (i = 0; i < fixed_array_size; i++) a->arr[i] = i;
}

void _varying_struct_t_op(a)
      varying_struct_t *a;
{
    int i;
    a->size = varying_array_size;
    for (i = 0; i < varying_array_size; i++) a->arr[i] = i;
}

void _conformant_struct_t_op(a)
      conf_struct_t *a;
{
    int i;
    a->size = conformant_array_size;
    for (i = 0; i < conformant_array_size; i++) a->arr[i] = i;
}

void _varying_conformant_struct_t_op(a)
      var_conf_struct_t *a;
{
    int i;
    a->size = conformant_array_size;
    a->used = varying_array_size;
    for (i = 0; i < varying_array_size; i++) a->arr[i] = i;
}

/*    
 * Real pointers to various complicated types
void fixed_array_pt_op(a)
      fixed_array_t **a;
{
    int i;
    *a = (fixed_array_t *)rpc_ss_allocate(sizeof(fixed_array_t));
    for (i = 0; i < fixed_array_size; i++) *a[i] = i;
}

void fixed_struct_pt_op(a)
       fixed_struct_t **a;
{
    int i;
    *a = (fixed_struct_t *)rpc_ss_allocate(sizeof(fixed_struct_t));
    a->size = fixed_array_size;
    for (i = 0; i < fixed_array_size; i++) a->arr[i] = i;
}

void varying_struct_pt_op(a)
      varying_struct_t **a;
{
    int i;
    *a = (varying_struct_t *)rpc_ss_allocate(sizeof(varying_struct_t));
    a->size = varying_array_size;
    for (i = 0; i < varying_array_size; i++) a->arr[i] = i;
}

void conformant_struct_pt_op(a)
      conf_struct_t **a;
{
    int i;
    *a = (conformant_struct_t *)rpc_ss_allocate(sizeof(conformant_struct_t));
    a->size = conformant_array_size;
    for (i = 0; i < varying_array_size; i++) a->arr[i] = i;
}

void varying_conformant_struct_pt_op(a)
      var_conf_struct_t **a;
{
    int i;
    *a = varying_conformant_struct_t *)rpc_ss_allocate(sizeof(varying_conformant_struct_t));
    a->size = varying_array_size;
    for (i = 0; i < varying_array_size; i++) a->arr[i] = i;
}

 */


globaldef out_refs_v0_0_epv_t out_refs_v0_0_m_epv = {
    _comm_status_t_op,
    _context_handle_op,
    _boolean_op,
    _char_op,
    _byte_op,           
    _float_op,
    _double_op,                   
    _small_op,                    
    _short_op,                    
    _long_op,                     
    _hyper_op,                    
    _usmall_op,           
    _ushort_op,           
    _ulong_op,            
    _uhyper_op,           
    _fixed_array_t_op,
    _fixed_struct_t_op,
    _varying_struct_t_op,
    _conformant_struct_t_op,
    _varying_conformant_struct_t_op
};
