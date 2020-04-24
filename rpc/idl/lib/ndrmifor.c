/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: ndrmifor.c,v $
 * Revision 1.1.2.2  1996/03/11  13:25:50  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:57  marty]
 *
 * Revision 1.1.2.1  1995/12/13  17:24:52  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:07 UTC  dat
 * 	New file for second XIDL drop for DCE 1.2.1
 * 	[1995/12/13  17:23:57  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:49:27  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  01:25:45  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/22  22:58:05  bfc]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) Digital Equipment Corporation, 1991
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**  
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of 
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or 
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**  NAME:
**
**      ndrmifor.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      NDR marshalling interpreter - FORTRAN support
**
*/

#include <dce/idlddefs.h>
#include <ndrmi.h>
#include <lsysdep.h>

/******************************************************************************/
/*                                                                            */
/* Marshall a scalar - special cases for FORTRAN                              */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_marsh_fortran_scalar
#ifdef IDL_PROTOTYPES
(
    /* [in] */  idl_byte type_byte,
    /* [in] */  rpc_void_p_t param_addr,  /* Address of item to be marshalled */
    IDL_msp_t IDL_msp
)
#else
(type_byte, param_addr, IDL_msp)
    idl_byte type_byte;
    rpc_void_p_t param_addr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_boolean    boolean_val;
    idl_small_int  small_val;
    idl_usmall_int usmall_val;
    idl_ushort_int ushort_val;

    switch(type_byte)
    {
        case IDL_DT_BOOLEAN:
            boolean_val = *(idl_for_boolean *)param_addr & 1;
            rpc_ss_ndr_marsh_check_buffer( 1, IDL_msp );
            rpc_marshall_boolean(IDL_msp->IDL_mp, boolean_val);
            IDL_msp->IDL_mp += 1;
            IDL_msp->IDL_left_in_buff -= 1;
            return;
        case IDL_DT_SMALL:
            small_val = (idl_small_int)*(idl_for_small *)param_addr;
            rpc_ss_ndr_marsh_check_buffer( 1, IDL_msp );
            rpc_marshall_small_int(IDL_msp->IDL_mp, small_val);
            IDL_msp->IDL_mp += 1;
            IDL_msp->IDL_left_in_buff -= 1;
            return;
        case IDL_DT_USMALL:
            usmall_val = (idl_usmall_int)*(idl_for_usmall *)param_addr;
            rpc_ss_ndr_marsh_check_buffer( 1, IDL_msp );
            rpc_marshall_usmall_int(IDL_msp->IDL_mp, usmall_val);
            IDL_msp->IDL_mp += 1;
            IDL_msp->IDL_left_in_buff -= 1;
            return;
        case IDL_DT_USHORT:
            ushort_val = (idl_ushort_int)*(idl_for_ushort *)param_addr;
            IDL_MARSH_ALIGN_MP( IDL_msp, 2 );
            rpc_ss_ndr_marsh_check_buffer( 2, IDL_msp );
            rpc_marshall_ushort_int(IDL_msp->IDL_mp, ushort_val);
            IDL_msp->IDL_mp += 2;
            IDL_msp->IDL_left_in_buff -= 2;
            return;
        default:
#ifdef DEBUG_INTERP
            printf("rpc_ss_ndr_marsh_fortran_scalar: unrecognized type %d\n",
                        type_byte);
            exit(0);
#endif
            RAISE(rpc_x_coding_error);
    }
}

/******************************************************************************/
/*                                                                            */
/* Marshall a FORTRAN array                                                   */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_m_fortran_arr
#ifdef IDL_PROTOTYPES
(
    /* [in] */  rpc_void_p_t array_addr,
    /* [in] */  idl_ulong_int dimensionality,
    /* [in] */  idl_ulong_int *Z_values,
    /* [in] */  IDL_bound_pair_t *range_list,
    /* [in] */  idl_byte base_type,
    /* [in] */  idl_ulong_int element_size,
                                /* Used if array of constructed type */
    /* [in] */  idl_ulong_int element_defn_index,
                                /* Used if array of constructed_type */
    IDL_msp_t IDL_msp
)
#else
( array_addr, dimensionality, Z_values, range_list, base_type, element_size,
 element_defn_index, IDL_msp )
    rpc_void_p_t array_addr;
    idl_ulong_int dimensionality;
    idl_ulong_int *Z_values;
    IDL_bound_pair_t *range_list;
    idl_byte base_type;
    idl_ulong_int element_size;
    idl_ulong_int element_defn_index;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int multipliers[IDL_FORTRAN_DIMS];    /* Sizes of array slices
                        used in address calculations. Usage is similar to
                        multipliers in VMS descriptors */
    idl_ulong_int index_values[IDL_FORTRAN_DIMS];   /* Index set for array
                        element currently being marshalled */
    idl_ulong_int array_elt_offset;     /* Offset, in units of base type, of
                                          current element from start of array */
    rpc_void_p_t array_elt_addr;    /* Address of current array element */
    idl_ulong_int node_number;
    long already_marshalled;
    idl_ulong_int i, dim;

    multipliers[0] = 1;
    index_values[0] = range_list[0].lower;
    for (i=1; i<dimensionality; i++)
    {
        multipliers[i] = Z_values[i-1] * multipliers[i-1];
        index_values[i] = range_list[i].lower;
    }

    /* For each index set that defines an element to be marshalled */
    do {
        /* Calculate position of current element */
        array_elt_offset = 0;
        for (i=0; i<dimensionality; i++)
            array_elt_offset += index_values[i] * multipliers[i];

        /* Marshall the element */
        switch (base_type)
        {
            case IDL_DT_BOOLEAN:
                array_elt_addr = (rpc_void_p_t)((idl_for_boolean *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_BOOLEAN( array_elt_addr );
                break;
            case IDL_DT_BYTE:
                array_elt_addr = (rpc_void_p_t)((idl_for_byte *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_BYTE( array_elt_addr );
                break;
            case IDL_DT_CHAR:
                array_elt_addr = (rpc_void_p_t)((idl_for_char *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_CHAR( array_elt_addr );
                break;
            case IDL_DT_DOUBLE:
                array_elt_addr = (rpc_void_p_t)((idl_for_double *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_DOUBLE( array_elt_addr );
                break;
            case IDL_DT_ENUM:
                array_elt_addr = (rpc_void_p_t)((idl_for_long *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_ENUM( array_elt_addr );
                break;
            case IDL_DT_V1_ENUM:
                array_elt_addr = (rpc_void_p_t)((idl_for_long *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_V1_ENUM( array_elt_addr );
                break;
            case IDL_DT_FLOAT:
                array_elt_addr = (rpc_void_p_t)((idl_for_float *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_FLOAT( array_elt_addr );
                break;
            case IDL_DT_SMALL:
                array_elt_addr = (rpc_void_p_t)((idl_for_small *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_SMALL( array_elt_addr );
                break;
            case IDL_DT_SHORT:
                array_elt_addr = (rpc_void_p_t)((idl_for_short *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_SHORT( array_elt_addr );
                break;
            case IDL_DT_LONG:
                array_elt_addr = (rpc_void_p_t)((idl_for_long *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_LONG( array_elt_addr );
                break;
            case IDL_DT_HYPER:
                array_elt_addr = (rpc_void_p_t)((idl_hyper_int *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_HYPER( array_elt_addr );
                break;
            case IDL_DT_USMALL:
                array_elt_addr = (rpc_void_p_t)((idl_for_usmall *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_USMALL( array_elt_addr );
                break;
            case IDL_DT_USHORT:
                array_elt_addr = (rpc_void_p_t)((idl_for_ushort *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_USHORT( array_elt_addr );
                break;
            case IDL_DT_ULONG:
                array_elt_addr = (rpc_void_p_t)((idl_for_ulong *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_ULONG( array_elt_addr );
                break;
            case IDL_DT_ERROR_STATUS:
                array_elt_addr = (rpc_void_p_t)((idl_for_ulong *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_ERROR_STATUS( array_elt_addr );
                break;
            case IDL_DT_UHYPER:
                array_elt_addr = (rpc_void_p_t)((idl_uhyper_int *)(array_addr)
                                                         + array_elt_offset);
                IDL_MARSH_UHYPER( array_elt_addr );
                break;
            case IDL_DT_FIXED_STRUCT:
                array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
                rpc_ss_ndr_marsh_struct( base_type, element_defn_index,
                                                       array_elt_addr, IDL_msp);
                break;
            case IDL_DT_ENC_UNION:
                array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
                rpc_ss_ndr_m_enc_union_or_ptees( array_elt_addr,
                                     element_defn_index, idl_false, IDL_msp );
                break;
            case IDL_DT_FULL_PTR:
                array_elt_addr = (rpc_void_p_t)((rpc_void_p_t *)(array_addr)
                                                         + array_elt_offset);
                node_number = rpc_ss_register_node( IDL_msp->IDL_node_table,
                                            *(byte_p_t *)array_elt_addr,
                                            ndr_false, &already_marshalled );
                IDL_MARSH_ULONG( &node_number );
                break;
            case IDL_DT_UNIQUE_PTR:
                array_elt_addr = (rpc_void_p_t)((rpc_void_p_t *)(array_addr)
                                                         + array_elt_offset);
                /* Get a value of 0 if pointer is null, 1 otherwise */
                node_number = (*(byte_p_t *)array_elt_addr != NULL);
                IDL_MARSH_ULONG( &node_number );
                break;
            case IDL_DT_STRING:
                rpc_ss_ndr_marsh_varying_arr(element_defn_index, array_addr,
                                                 NULL, NULL, 0, IDL_msp);
                break;
            case IDL_DT_TRANSMIT_AS:
            case IDL_DT_REPRESENT_AS:
                array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
                rpc_ss_ndr_marsh_xmit_as( element_defn_index, array_elt_addr,
                                                                    IDL_msp );
                break;
#if 0       /* Interface references not supported for FORTRAN */
            case IDL_DT_INTERFACE:
                array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
                rpc_ss_ndr_marsh_interface( element_defn_index, array_elt_addr,
                                                                    IDL_msp );
                break;
#endif
            default:
#ifdef DEBUG_INTERP
                printf(
                      "rpc_ss_ndr_m_fortran_arr:unrecognized type %d\n",
                        base_type);
                exit(0);
#endif
                RAISE(rpc_x_coding_error);
        }

        /* Move to the next element */
        dim = dimensionality - 1;
        while (dim >= 0)
        {
            index_values[dim]++;
            if (index_values[dim] < (idl_ulong_int)range_list[dim].upper)
                break;
            index_values[dim] = range_list[dim].lower;
            dim--;
        }
    } while (dim >= 0);
}

/******************************************************************************/
/*                                                                            */
/* Marshall a FORTRAN fixed or conformant array                               */
/* Used when the array is multi-dimensional, or the local represeentation is  */
/* different from C                                                           */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_m_for_foc_arr
#ifdef IDL_PROTOTYPES
(
    /* [in] */  rpc_void_p_t array_addr,
    /* [in] */  idl_ulong_int dimensionality,
    /* [in] */  IDL_bound_pair_t *bounds_list,
    /* [in] */  idl_byte base_type,
    /* [in] */  idl_ulong_int element_size,
                                /* Used if array of constructed type */
    /* [in] */  idl_ulong_int element_defn_index,
                                /* Used if array of constructed type */
    IDL_msp_t IDL_msp
)
#else
( array_addr, dimensionality, bounds_list, base_type, element_size,
 element_defn_index, IDL_msp )
    rpc_void_p_t array_addr;
    idl_ulong_int dimensionality;
    IDL_bound_pair_t *bounds_list;
    idl_byte base_type;
    idl_ulong_int element_size;
    idl_ulong_int element_defn_index;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int fortran_Z_values[IDL_FORTRAN_DIMS];
    idl_ulong_int *Z_values = fortran_Z_values;
    IDL_bound_pair_t range_list[IDL_FORTRAN_DIMS];

    rpc_ss_Z_values_from_bounds(bounds_list, dimensionality, &Z_values,
                                                                     IDL_msp);
    rpc_ss_range_list_from_Z_values(dimensionality, Z_values, range_list);
    rpc_ss_ndr_m_fortran_arr(array_addr, dimensionality, Z_values, range_list,
                          base_type, element_size, element_defn_index, IDL_msp);
}

/******************************************************************************/
/*                                                                            */
/* Marshall the pointees of a FORTRAN array                                   */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_m_fortran_arr_ptees
#ifdef IDL_PROTOTYPES
(
    /* [in] */  rpc_void_p_t array_addr,
    /* [in] */  idl_ulong_int dimensionality,
    /* [in] */  idl_ulong_int *Z_values,
    /* [in] */  IDL_bound_pair_t *range_list,
    /* [in] */  idl_byte base_type,
    /* [in] */  idl_ulong_int element_size,
                                /* Used if array of struct or union */
    /* [in] */  idl_ulong_int element_defn_index,
                                /* Used if array of struct or union */
    /* [in] */  idl_byte *defn_vec_ptr, /* For array of pointers. Points at
                                                pointee description */
    IDL_msp_t IDL_msp
)
#else
( array_addr, dimensionality, Z_values, range_list, base_type, element_size,
 element_defn_index, defn_vec_ptr, IDL_msp )
    rpc_void_p_t array_addr;
    idl_ulong_int dimensionality;
    idl_ulong_int *Z_values;
    IDL_bound_pair_t *range_list;
    idl_byte base_type;
    idl_ulong_int element_size;
    idl_ulong_int element_defn_index;
    idl_byte *defn_vec_ptr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int multipliers[IDL_FORTRAN_DIMS];
    idl_ulong_int index_values[IDL_FORTRAN_DIMS];
    idl_ulong_int array_elt_offset;     /* Offset, in units of base type, of
                                          current element from start of array */
    rpc_void_p_t array_elt_addr;    /* Address of current array element */
    IDL_pointee_desc_t pointee_desc;
    idl_ulong_int i, dim;

    multipliers[0] = 1;
    index_values[0] = range_list[0].lower;
    for (i=1; i<dimensionality; i++)
    {
        multipliers[i] = Z_values[i-1] * multipliers[i-1];
        index_values[i] = range_list[i].lower;
    }

    pointee_desc.dimensionality = 0;
    do {
        /* Calculate position of current element */
        array_elt_offset = 0;
        for (i=0; i<dimensionality; i++)
        array_elt_offset += index_values[i] * multipliers[i];

        if (base_type == IDL_DT_FIXED_STRUCT)
        {
            array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
            rpc_ss_ndr_m_struct_pointees( base_type, element_defn_index,
                                                array_elt_addr, IDL_msp );
        }
        else if (base_type == IDL_DT_ENC_UNION)
        {
            array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
            rpc_ss_ndr_m_enc_union_or_ptees(array_elt_addr,
                                                element_defn_index,
                                                idl_true, IDL_msp );
        }
        else
        {
            array_elt_addr = (rpc_void_p_t)((rpc_void_p_t *)(array_addr)
                                                         + array_elt_offset);
            if (*(rpc_void_p_t *)array_elt_addr != NULL)
            {
                rpc_ss_pointee_desc_from_data( defn_vec_ptr, 
                                           *(rpc_void_p_t *)array_elt_addr,
                                           NULL, NULL, &pointee_desc, IDL_msp );
                rpc_ss_ndr_marsh_pointee( defn_vec_ptr,
                                   *(rpc_void_p_t *)array_elt_addr,
                                   (idl_boolean)(base_type == IDL_DT_FULL_PTR),
                                   &pointee_desc, IDL_msp );
            }
        }

        /* Move to the next element */
        dim = dimensionality - 1;
        while (dim >= 0)
        {
            index_values[dim]++;
            if (index_values[dim] < (idl_ulong_int)range_list[dim].upper)
                break;
            index_values[dim] = range_list[dim].lower;
            dim--;
        }
    } while (dim >= 0);
    rpc_ss_rlse_data_pointee_desc( &pointee_desc, IDL_msp );
}

/******************************************************************************/
/*                                                                            */
/* Marshall the pointees of a FORTRAN fixed or conformant array               */
/* Used when the array is multi-dimensional                                   */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_m_for_foc_arr_ptees
#ifdef IDL_PROTOTYPES
(
    /* [in] */  rpc_void_p_t array_addr,
    /* [in] */  idl_ulong_int dimensionality,
    /* [in] */  IDL_bound_pair_t *bounds_list,
    /* [in] */  idl_byte base_type,
    /* [in] */  idl_ulong_int element_size,
                                /* Used if array of struct or union */
    /* [in] */  idl_ulong_int element_defn_index,
                                /* Used if array of struct or union */
    /* [in] */  idl_byte *defn_vec_ptr, /* For array of pointers. Points at
                                                pointee description */
    IDL_msp_t IDL_msp
)
#else
( array_addr, dimensionality, bounds_list, base_type, element_size,
 element_defn_index, defn_vec_ptr, IDL_msp )
    rpc_void_p_t array_addr;
    idl_ulong_int dimensionality;
    IDL_bound_pair_t *bounds_list;
    idl_byte base_type;
    idl_ulong_int element_size;
    idl_ulong_int element_defn_index;
    idl_byte *defn_vec_ptr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int fortran_Z_values[IDL_FORTRAN_DIMS];
    idl_ulong_int *Z_values = fortran_Z_values;
    IDL_bound_pair_t range_list[IDL_FORTRAN_DIMS];

    rpc_ss_Z_values_from_bounds(bounds_list, dimensionality, &Z_values,
                                                                   IDL_msp);
    rpc_ss_range_list_from_Z_values(dimensionality, Z_values, range_list);
    rpc_ss_ndr_m_fortran_arr_ptees(array_addr, dimensionality, Z_values,
                                     range_list, base_type, element_size,
                                     element_defn_index, defn_vec_ptr, IDL_msp);
}

/******************************************************************************/
/*                                                                            */
/* String parameter - if it is [string] conformant:                           */
/*      On VMS use the descriptor to control the marshalling                  */
/*      On OSF/1 read the hidden parameter through the special machinery      */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_m_for_conf_string
#ifdef IDL_PROTOTYPES
(
    /* [in,out] */ idl_byte **p_type_vec_ptr,
    /* [in] */ rpc_void_p_t param_addr,
    /* [in,out] */ idl_boolean *p_is_string,
    IDL_msp_t IDL_msp
)
#else
(p_type_vec_ptr, param_addr, p_is_string, IDL_msp)
    idl_byte **p_type_vec_ptr;
    rpc_void_p_t param_addr;
    idl_boolean *p_is_string;
    IDL_msp_t IDL_msp;
#endif
{
    idl_byte *type_vec_ptr;
    idl_ulong_int array_defn_index;
    idl_byte *array_defn_ptr;
    IDL_bound_pair_t bounds_list[1];
    idl_byte bound_kind, bound_type;
    rpc_void_p_t bound_addr;
    idl_ulong_int attribute_index;
    idl_ulong_int element_size;     /* Size of base type of string */
    idl_ulong_int Z_value;
    rpc_void_p_t data_addr;         /* Address of string data */
    idl_boolean add_null;

    /* Assume we have this case, escape at any time we find this assumption
        is false */
    type_vec_ptr = *p_type_vec_ptr;
    if (*type_vec_ptr != IDL_DT_OPEN_ARRAY)
        return;
    type_vec_ptr++;
    IDL_DISCARD_LONG_FROM_VECTOR( type_vec_ptr );
                                /* Discard full array definition */
    IDL_GET_LONG_FROM_VECTOR( array_defn_index, type_vec_ptr );
    array_defn_ptr = IDL_msp->IDL_type_vec + array_defn_index;
    array_defn_ptr++;   /* Skip dimensionality - must be 1 */
    /* Get lower bound */
    bound_kind = *array_defn_ptr;
    array_defn_ptr++;
    if (bound_kind == IDL_BOUND_FIXED)
    {
        IDL_GET_LONG_FROM_VECTOR(bounds_list[0].lower, array_defn_ptr);
    }
    else
    {
        /* Lower bound is [min_is] */
        bound_type = *array_defn_ptr;
        array_defn_ptr++;
        IDL_GET_LONG_FROM_VECTOR(attribute_index, array_defn_ptr);
        bound_addr = IDL_msp->IDL_param_vec[attribute_index];
        bounds_list[0].lower = rpc_ss_get_typed_integer(bound_type, bound_addr,
                                                        IDL_msp);
    }

    /* Get upper bound */
    bound_kind = *array_defn_ptr;
    if (bound_kind != IDL_BOUND_STRING)
        return;
    array_defn_ptr++;
    element_size = (idl_ulong_int)*array_defn_ptr;
#ifdef VMS
    Z_value = ((struct dsc$descriptor *)param_addr)->dsc$w_length
                                                             / element_size;
#else
    Z_value = *(((IDL_fortran_conf_char_arr_t *)param_addr)->p_length)
                                                                 / element_size;
#endif
    bounds_list[0].upper = bounds_list[0].lower + Z_value - 1;
    /* Marshall Z_value */
    IDL_MARSH_ULONG( &Z_value );
    IDL_DISCARD_LONG_FROM_VECTOR(array_defn_ptr); /* Align to limit info */
    array_defn_ptr += IDL_DATA_LIMIT_PAIR_WIDTH;    /* Move to base type desc */
    /* Marshall A-B pair and data */
#ifdef VMS
    data_addr = (rpc_void_p_t)
                        (((struct dsc$descriptor *)param_addr)->dsc$a_pointer);
#else
    data_addr = (rpc_void_p_t)
                        (((IDL_fortran_conf_char_arr_t *)param_addr)->p_data);
#endif
    rpc_ss_nstrsiz((idl_char *)data_addr, element_size, Z_value - 1, &add_null);
    rpc_ss_ndr_m_var_or_open_arr(data_addr, &Z_value, 1, bounds_list,
                                 array_defn_ptr,
                                 (add_null ? IDL_M_ADD_NULL : 0),
                                 IDL_msp);

    /* If we marshalled the string, update the caller's control variables */
    *p_is_string = idl_false;
    *p_type_vec_ptr = type_vec_ptr;
}

