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
 * Revision 1.1.4.2  1996/02/17  23:15:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:19  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:13  root]
 * 
 * Revision 1.1.2.3  1993/01/13  21:49:23  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:36:40  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <op_results.h>
#include <test_common.h>

typedef struct
{
    long l;
} mgr_ctx_t;

/*
 * Operations returning base types.
 */
error_status_t              _comm_status_t_op()
{
    error_status_t st;
    st = error_status_ok;
    return st;
}

context_handle_t            _context_handle_op()
{
    mgr_ctx_t * ctx_p = (mgr_ctx_t *)malloc(sizeof(mgr_ctx_t));

    ctx_p->l = long_return_val;

    return (context_handle_t)ctx_p;
}

void context_handle_t_rundown(context_handle)
    rpc_ss_context_t context_handle;
{
    free(context_handle);
    return;
}

ndr_boolean                     _boolean_op()
{
    return boolean_return_val;
}

ndr_char                        _char_op()
{
    return char_return_val;
}

ndr_byte                        _byte_op()
{
    return byte_return_val;
}

ndr_short_float                       _float_op()
{
    return float_return_val;
}

ndr_long_float _double_op()
{
    return double_return_val;
}

ndr_small_int                       _small_op()
{
    return small_return_val;
}

ndr_short_int                       _short_op()
{
    return short_return_val;
}

ndr_long_int                        _long_op(context_handle)
    context_handle_t context_handle;
{
    mgr_ctx_t * ctx_p = (mgr_ctx_t *)context_handle;

    return ctx_p->l;
}

ndr_hyper_int                       _hyper_op()
{
    ndr_hyper_int h;
    h.low = hyper_low_return_val;
    h.high = hyper_high_return_val;
    return h;
}

ndr_usmall_int              _usmall_op()
{
    return usmall_return_val;
}

ndr_ushort_int              _ushort_op()
{
    return ushort_return_val;
}

ndr_ulong_int               _ulong_op()
{
    return ulong_return_val;
}

ndr_uhyper_int              _uhyper_op()
{
    ndr_uhyper_int h;
    h.low = uhyper_low_return_val;
    h.high = uhyper_high_return_val;
    return h;
}

color_t                     _enum_op()
{
    return enum_return_val;
}


/*
 * Operations returning pointers to base types.
 */
/*
context_handle_t          * _p_context_handle_op()
{
    context_handle_t * p = (context_handle_t *)rpc_ss_allocate(sizeof(context_handle_t));
    *p = NULL;
    return p;
}
*/
ndr_boolean                   * _p_boolean_op()
{
    ndr_boolean * p = (ndr_boolean *)rpc_ss_allocate(sizeof(ndr_boolean));
    *p = boolean_return_val;
    return p;
}

ndr_char                      * _p_char_op()
{
    ndr_char * p = (ndr_char *)rpc_ss_allocate(sizeof(ndr_char));
    *p = char_return_val;
    return p;
}

ndr_byte                      * _p_byte_op()
{
    ndr_byte * p = (ndr_byte *)rpc_ss_allocate(sizeof(ndr_byte));
    *p = byte_return_val;
    return p;
}

ndr_short_float                     * _p_float_op()
{
    ndr_short_float * p = (ndr_short_float *)rpc_ss_allocate(sizeof(ndr_short_float));
    *p = float_return_val;
    return p;
}

ndr_long_float * _p_double_op()
{
    ndr_long_float * p = (ndr_long_float *)rpc_ss_allocate(sizeof(ndr_long_float));
    *p = double_return_val;
    return p;
}

ndr_small_int                     * _p_small_op()
{
    ndr_small_int * p = (ndr_small_int *)rpc_ss_allocate(sizeof(ndr_small_int));
    *p = small_return_val;
    return p;
}

ndr_short_int                     * _p_short_op()
{
    ndr_short_int * p = (ndr_short_int *)rpc_ss_allocate(sizeof(ndr_short_int));
    *p = short_return_val;
    return p;
}

ndr_long_int                      * _p_long_op()
{
    ndr_long_int * p = (ndr_long_int *)rpc_ss_allocate(sizeof(ndr_long_int));
    *p = long_return_val;
    return p;
}

ndr_hyper_int                     * _p_hyper_op()
{
    ndr_hyper_int * p = (ndr_hyper_int *)rpc_ss_allocate(sizeof(ndr_hyper_int));
    p->low = hyper_low_return_val;
    p->high = hyper_high_return_val;
    return p;
}

ndr_usmall_int            * _p_usmall_op()
{
    ndr_usmall_int * p = (ndr_usmall_int *)rpc_ss_allocate(sizeof(ndr_usmall_int));
    *p = usmall_return_val;
    return p;
}

ndr_ushort_int            * _p_ushort_op()
{
    ndr_ushort_int * p = (ndr_ushort_int *)rpc_ss_allocate(sizeof(ndr_ushort_int));
    *p = ushort_return_val;
    return p;
}

ndr_ulong_int             * _p_ulong_op()
{
    ndr_ulong_int * p = (ndr_ulong_int *)rpc_ss_allocate(sizeof(ndr_ulong_int));
    *p = ulong_return_val;
    return p;
}

ndr_uhyper_int            * _p_uhyper_op()
{
    ndr_uhyper_int * p = (ndr_uhyper_int *)rpc_ss_allocate(sizeof(ndr_uhyper_int));
    p->low = uhyper_low_return_val;
    p->high = uhyper_high_return_val;
    return p;
}

color_t                   * _p_enum_op()
{
    color_t * p = (color_t *)rpc_ss_allocate(sizeof(color_t));
    *p = enum_return_val;
    return p;
}


/*
 * Operations returning complicated results.
 */
fixed_array_t *_fixed_array_t_op()
{
    int i;
    fixed_array_t *fixed_array_val = (fixed_array_t *)rpc_ss_allocate(
          sizeof(fixed_array_t));
    for (i = 0; i < fixed_array_size; i++) (*fixed_array_val)[i] = i;
    return fixed_array_val;
}

varying_array_t *_varying_array_t_op()
{
    int i;
    varying_array_t *varying_array_val = (varying_array_t *)rpc_ss_allocate(
      sizeof(long) * varying_array_size);
    for (i = 0; i < varying_array_size; i++) (*varying_array_val)[i] = i;
    return varying_array_val;
}

conformant_array_t *_conformant_array_t_op()
{
    int i;
    conformant_array_t *conformant_array_val = (conformant_array_t *)rpc_ss_allocate(
      conformant_array_size * sizeof(ndr_long_int));
    for (i = 0; i < conformant_array_size; i++) (*conformant_array_val)[i] = i;
    return conformant_array_val;
}

varying_conformant_array_t *_varying_conformant_array_t_op()
{
    int i;
    varying_conformant_array_t *varying_conformant_array_val = 
          (varying_conformant_array_t *)rpc_ss_allocate(
              conformant_array_size * sizeof(ndr_long_int));
    for (i = 0; i < varying_array_size; i++) 
          (*varying_conformant_array_val)[i] = i;
    return varying_conformant_array_val;
}

fixed_struct_t *_fixed_struct_t_op()
{
    int i;
    fixed_struct_t *fixed_struct_val = (fixed_struct_t *)rpc_ss_allocate(
          sizeof(fixed_struct_t));
    fixed_struct_val->size = fixed_array_size;
    for (i = 0; i < fixed_array_size; i++) 
        (*fixed_struct_val).arr[i] = i;
    return fixed_struct_val;
}

varying_struct_t *_varying_struct_t_op()
{
    int i;
    varying_struct_t *varying_struct_val = (varying_struct_t *)rpc_ss_allocate(
          sizeof(varying_struct_t) + (sizeof(long)*varying_array_size));
    varying_struct_val->size = varying_array_size;
    for (i = 0; i < varying_array_size; i++) 
          (*varying_struct_val).arr[i] = i;
    return varying_struct_val;
}

conf_struct_t *_conformant_struct_t_op()
{
    int i;
    conf_struct_t *conformant_struct_val = (conf_struct_t *)rpc_ss_allocate(
      sizeof(ndr_long_int) + (conformant_array_size * sizeof(ndr_long_int)));
    conformant_struct_val->size = conformant_array_size;
    for (i = 0; i < conformant_array_size; i++) 
        (*conformant_struct_val).arr[i] = i;
    return conformant_struct_val;
}

var_conf_struct_t *_varying_conformant_struct_t_op()
{
    int i;
    var_conf_struct_t *varying_conformant_struct_val = (var_conf_struct_t *)
          rpc_ss_allocate(sizeof(var_conf_struct_t) + 
               conformant_array_size * sizeof(ndr_long_int));
    varying_conformant_struct_val->size = conformant_array_size;
    varying_conformant_struct_val->used = varying_array_size;
    for (i = 0; i < varying_array_size; i++) 
          (*varying_conformant_struct_val).arr[i] = i;
    return varying_conformant_struct_val;
}



globaldef op_results_v0_0_epv_t op_results_v0_0_m_epv = {
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
    _enum_op,
/*    _p_context_handle_op,*/
    _p_boolean_op,
    _p_char_op,
    _p_byte_op,           
    _p_float_op,          
    _p_double_op,                   
    _p_small_op,                    
    _p_short_op,                    
    _p_long_op,                     
    _p_hyper_op,                    
    _p_usmall_op,           
    _p_ushort_op,           
    _p_ulong_op,            
    _p_uhyper_op,           
    _p_enum_op,
    _fixed_array_t_op,
    _fixed_struct_t_op,
    _varying_struct_t_op,
    _conformant_struct_t_op,
    _varying_conformant_struct_t_op,
};
