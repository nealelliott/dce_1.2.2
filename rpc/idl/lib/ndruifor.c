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
 * $Log: ndruifor.c,v $
 * Revision 1.1.2.2  1996/03/11  13:25:52  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:58  marty]
 *
 * Revision 1.1.2.1  1995/12/13  17:24:53  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:08 UTC  dat
 * 	New file for second XIDL drop for DCE 1.2.1
 * 	[1995/12/13  17:23:58  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:49:39  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  01:25:52  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/22  22:58:05  bfc]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      ndruifor.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      NDR unmarshalling interpreter - FORTRAN support
**
*/

#include <dce/idlddefs.h>
#include <ndrui.h>
#include <lsysdep.h>

/******************************************************************************/
/*                                                                            */
/* Unmarshall a scalar - special cases for FORTRAN                            */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_unmar_fortran_scalar
#ifdef IDL_PROTOTYPES
(
    /* [in] */  idl_byte type_byte,
    /* [in] */  rpc_void_p_t param_addr,  /* Address item is to be marshalled 
                                                into */
    IDL_msp_t IDL_msp
)
#else
(type_byte, param_addr, IDL_msp)
    idl_byte type_byte;
    rpc_void_p_t param_addr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_boolean boolean_val;
    idl_small_int small_val;
    idl_usmall_int usmall_val;
    idl_ushort_int ushort_val;

    switch(type_byte)
    {
        case IDL_DT_BOOLEAN:
            rpc_ss_ndr_unmar_check_buffer( IDL_msp );
            rpc_convert_boolean(IDL_msp->IDL_drep, ndr_g_local_drep,
                               IDL_msp->IDL_mp, boolean_val);
            *(idl_for_boolean *)param_addr = ((boolean_val != 0) ? 255 : 0);
                                    /* Return 255 if unmarshalled value not 0 */
            IDL_msp->IDL_mp += 1;
            IDL_msp->IDL_left_in_buff -= 1;
            return;
        case IDL_DT_SMALL:
            rpc_ss_ndr_unmar_check_buffer( IDL_msp );
            rpc_convert_small_int(IDL_msp->IDL_drep, ndr_g_local_drep,
                               IDL_msp->IDL_mp, small_val);
            *(idl_for_small *)param_addr = small_val;
            IDL_msp->IDL_mp += 1;
            IDL_msp->IDL_left_in_buff -= 1;
            return;
        case IDL_DT_USMALL:
            rpc_ss_ndr_unmar_check_buffer( IDL_msp );
            rpc_convert_usmall_int(IDL_msp->IDL_drep, ndr_g_local_drep,
                               IDL_msp->IDL_mp, usmall_val);
            *(idl_for_usmall *)param_addr = usmall_val;
            IDL_msp->IDL_mp += 1;
            IDL_msp->IDL_left_in_buff -= 1;
            return;
        case IDL_DT_USHORT:
            IDL_UNMAR_ALIGN_MP( IDL_msp, 2 );
            rpc_ss_ndr_unmar_check_buffer( IDL_msp );
            rpc_convert_ushort_int(IDL_msp->IDL_drep, ndr_g_local_drep,
                               IDL_msp->IDL_mp, ushort_val);
            *(idl_for_ushort *)param_addr = ushort_val;
            IDL_msp->IDL_mp += 2;
            IDL_msp->IDL_left_in_buff -= 2;
            return;
        default:
#ifdef DEBUG_INTERP
            printf("rpc_ss_ndr_unmar_fortran_scalar: unrecognized type %d\n",
                        type_byte);
            exit(0);
#endif
            RAISE(rpc_x_coding_error);
    }
}

/******************************************************************************/
/*                                                                            */
/* Unmarshall a FORTRAN array                                                 */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_u_fortran_arr
#ifdef IDL_PROTOTYPES
(
    /* [in] */  rpc_void_p_t array_addr,
    /* [in] */  idl_ulong_int dimensionality,
    /* [in] */  idl_ulong_int *Z_values,
    /* [in] */  IDL_bound_pair_t *range_list,
    /* [in] */  idl_byte base_type,
    /* [in] */  idl_ulong_int element_size,
                                /* Used if array of struct or array of string */
    /* [in] */  idl_ulong_int element_defn_index,
                                /* Used if array of struct or array of string */
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
    idl_ulong_int multipliers[IDL_FORTRAN_DIMS];
    idl_ulong_int index_values[IDL_FORTRAN_DIMS];
    idl_ulong_int array_elt_offset;     /* Offset, in units of base type, of
                                          current element from start of array */
    rpc_void_p_t array_elt_addr;    /* Address of current array element */
    idl_ulong_int node_number;
    idl_ulong_int unique_flag;  /* Wire form of [unique] pointer */
    idl_byte *element_defn_ptr;
    unsigned long xmit_data_size;   /* [transmit_as] - size of xmitted type */
    rpc_void_p_t xmit_data_buff = NULL;     /* Address of storage [transmit_as]
                                                type can be unmarshalled into */
    idl_ulong_int i, dim;

    if ( (base_type == IDL_DT_TRANSMIT_AS)
        || (base_type == IDL_DT_REPRESENT_AS) )
    {
        /* If possible, allocate a buffer into which elements of the transmitted
            type can be unmarshalled */
        element_defn_ptr = IDL_msp->IDL_type_vec + element_defn_index;
        IDL_DISCARD_LONG_FROM_VECTOR( element_defn_ptr );
                                             /* Presented type size index */
        IDL_DISCARD_LONG_FROM_VECTOR( element_defn_ptr );   /* Rtn vec index */
        if (*element_defn_ptr == IDL_DT_STRING)
            element_defn_ptr++;
        xmit_data_size = rpc_ss_type_size(element_defn_ptr, IDL_msp);
        if (xmit_data_size != 0)
            xmit_data_buff = (rpc_void_p_t)rpc_ss_mem_alloc(
                                    &IDL_msp->IDL_mem_handle, xmit_data_size);
    }

    multipliers[0] = 1;
    index_values[0] = range_list[0].lower;
    for (i=1; i<dimensionality; i++)
    {
        multipliers[i] = Z_values[i-1] * multipliers[i-1];
        index_values[i] = range_list[i].lower;
    }

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
                IDL_UNMAR_BOOLEAN( array_elt_addr );
                break;
            case IDL_DT_BYTE:
                array_elt_addr = (rpc_void_p_t)((idl_for_byte *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_BYTE( array_elt_addr );
                break;
            case IDL_DT_CHAR:
                array_elt_addr = (rpc_void_p_t)((idl_for_char *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_CHAR( array_elt_addr );
                break;
            case IDL_DT_DOUBLE:
                array_elt_addr = (rpc_void_p_t)((idl_for_double *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_DOUBLE( array_elt_addr );
                break;
            case IDL_DT_ENUM:
                array_elt_addr = (rpc_void_p_t)((idl_for_long *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_ENUM( array_elt_addr );
                break;
            case IDL_DT_V1_ENUM:
                array_elt_addr = (rpc_void_p_t)((idl_for_long *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_V1_ENUM( array_elt_addr );
                break;
            case IDL_DT_FLOAT:
                array_elt_addr = (rpc_void_p_t)((idl_for_float *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_FLOAT( array_elt_addr );
                break;
            case IDL_DT_SMALL:
                array_elt_addr = (rpc_void_p_t)((idl_for_small *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_SMALL( array_elt_addr );
                break;
            case IDL_DT_SHORT:
                array_elt_addr = (rpc_void_p_t)((idl_for_short *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_SHORT( array_elt_addr );
                break;
            case IDL_DT_LONG:
                array_elt_addr = (rpc_void_p_t)((idl_for_long *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_LONG( array_elt_addr );
                break;
            case IDL_DT_HYPER:
                array_elt_addr = (rpc_void_p_t)((idl_hyper_int *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_HYPER( array_elt_addr );
                break;
            case IDL_DT_USMALL:
                array_elt_addr = (rpc_void_p_t)((idl_for_usmall *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_USMALL( array_elt_addr );
                break;
            case IDL_DT_USHORT:
                array_elt_addr = (rpc_void_p_t)((idl_for_ushort *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_USHORT( array_elt_addr );
                break;
            case IDL_DT_ULONG:
                array_elt_addr = (rpc_void_p_t)((idl_for_ulong *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_ULONG( array_elt_addr );
                break;
            case IDL_DT_ERROR_STATUS:
                array_elt_addr = (rpc_void_p_t)((idl_for_ulong *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_ERROR_STATUS( array_elt_addr );
                break;
            case IDL_DT_UHYPER:
                array_elt_addr = (rpc_void_p_t)((idl_uhyper_int *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_UHYPER( array_elt_addr );
                break;
            case IDL_DT_FIXED_STRUCT:
                array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
                rpc_ss_ndr_unmar_struct( base_type,
                                     IDL_msp->IDL_type_vec + element_defn_index,
                                         array_elt_addr, NULL, NULL, IDL_msp);
                break;
            case IDL_DT_ENC_UNION:
                array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
                rpc_ss_ndr_u_enc_union_or_ptees( array_elt_addr,
                                     element_defn_index, idl_false, IDL_msp );
                break;
            case IDL_DT_FULL_PTR:
                IDL_UNMAR_ULONG( &node_number );
                array_elt_addr = (rpc_void_p_t)((rpc_void_p_t *)(array_addr)
                                                         + array_elt_offset);
                *(rpc_void_p_t *)(array_elt_addr) = (rpc_void_p_t)node_number;
                break;
            case IDL_DT_UNIQUE_PTR:
                array_elt_addr = (rpc_void_p_t)((rpc_void_p_t *)(array_addr)
                                                         + array_elt_offset);
                IDL_UNMAR_ULONG( &unique_flag );
                if (unique_flag == 0)
                    *(rpc_void_p_t *)array_elt_addr = NULL;
                else if ( *(rpc_void_p_t *)array_addr == NULL )
                    *(rpc_void_p_t *)array_elt_addr = IDL_NEW_NODE;
                break;
            case IDL_DT_STRING:
                {
                    idl_byte *element_defn_ptr;
                    idl_ulong_int A, B;
                    idl_ulong_int base_type_size;

                    array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
                    element_defn_ptr = IDL_msp->IDL_type_vec
                                        + element_defn_index
                                        + 1     /* Dimensionality */
                                        + IDL_FIXED_BOUND_PAIR_WIDTH
                                        + IDL_DATA_LIMIT_PAIR_WIDTH;
                    /* Now pointing at base type of string */
                    base_type_size = rpc_ss_type_size(element_defn_ptr,
                                                                     IDL_msp);
                    IDL_UNMAR_ALIGN_MP( IDL_msp, 4 );
                    rpc_ss_ndr_unmar_check_buffer( IDL_msp );
                    rpc_convert_ulong_int(IDL_msp->IDL_drep, ndr_g_local_drep,
                                 IDL_msp->IDL_mp, A);
                    IDL_msp->IDL_mp += 4;
                    IDL_msp->IDL_left_in_buff -= 4;
                    rpc_ss_ndr_unmar_check_buffer( IDL_msp );
                    rpc_convert_ulong_int(IDL_msp->IDL_drep, ndr_g_local_drep,
                                 IDL_msp->IDL_mp, B);
                    IDL_msp->IDL_mp += 4;
                    IDL_msp->IDL_left_in_buff -= 4;
                    if ( (*element_defn_ptr == IDL_DT_CHAR)
                        && (IDL_msp->IDL_drep.char_rep
                                                 != ndr_g_local_drep.char_rep) )
                    {
                        rpc_ss_ndr_unmar_by_looping(B, *element_defn_ptr,
                                                 array_elt_addr, 1, 0, IDL_msp);
                    }
                    else
                        rpc_ss_ndr_unmar_by_copying(B, base_type_size,
                                                    array_elt_addr, IDL_msp);
                }
                break;
            case IDL_DT_TRANSMIT_AS:
            case IDL_DT_REPRESENT_AS:
                array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
                rpc_ss_ndr_unmar_xmit_as( element_defn_index, array_elt_addr,
                                                      xmit_data_buff, IDL_msp );
                break;
#if 0       /* Interface references not supported for FORTRAN */
            case IDL_DT_INTERFACE:
                array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
                rpc_ss_ndr_unmar_interface( element_defn_index, array_elt_addr,
                                                      xmit_data_buff, IDL_msp );
                break;
#endif
            default:
#ifdef DEBUG_INTERP
                printf(
                      "rpc_ss_ndr_u_fortran_arr:unrecognized type %d\n",
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
    if (xmit_data_buff != NULL)
        rpc_ss_mem_item_free(&IDL_msp->IDL_mem_handle,
                                (byte_p_t)xmit_data_buff);
}

/******************************************************************************/
/*                                                                            */
/* Unmarshall a FORTRAN fixed or conformant array                             */
/* Used when the array is multi-dimensional, or the base type size is         */
/* different from C                                                           */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_u_for_foc_arr
#ifdef IDL_PROTOTYPES
(
    /* [in] */  rpc_void_p_t array_addr,
    /* [in] */  idl_ulong_int dimensionality,
    /* [in] */  idl_ulong_int *Z_values,
    /* [in] */  idl_byte base_type,
    /* [in] */  idl_ulong_int element_size,
                                /* Used if array of struct or array of string */
    /* [in] */  idl_ulong_int element_defn_index,
                                /* Used if array of struct or array of string */
    IDL_msp_t IDL_msp
)
#else
( array_addr, dimensionality, Z_values, base_type, element_size,
 element_defn_index, IDL_msp )
    rpc_void_p_t array_addr;
    idl_ulong_int dimensionality;
    idl_ulong_int *Z_values;
    idl_byte base_type;
    idl_ulong_int element_size;
    idl_ulong_int element_defn_index;
    IDL_msp_t IDL_msp;
#endif
{
    IDL_bound_pair_t range_list[IDL_FORTRAN_DIMS];

    rpc_ss_range_list_from_Z_values(dimensionality, Z_values, range_list);
    rpc_ss_ndr_u_fortran_arr(array_addr, dimensionality, Z_values, range_list,
                          base_type, element_size, element_defn_index, IDL_msp);
}

/******************************************************************************/
/*                                                                            */
/* Unmarshall the pointees of a FORTRAN array                                 */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_u_fortran_arr_ptees
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
    idl_ulong_int i, dim;

    multipliers[0] = 1;
    index_values[0] = range_list[0].lower;
    for (i=1; i<dimensionality; i++)
    {
        multipliers[i] = Z_values[i-1] * multipliers[i-1];
        index_values[i] = range_list[i].lower;
    }

    do {
        /* Calculate position of current element */
        array_elt_offset = 0;
        for (i=0; i<dimensionality; i++)
        array_elt_offset += index_values[i] * multipliers[i];

        if (base_type == IDL_DT_FIXED_STRUCT)
        {
            array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
            rpc_ss_ndr_u_struct_pointees( base_type, element_defn_index,
                                                array_elt_addr, NULL, IDL_msp );
        }
        else if (base_type == IDL_DT_ENC_UNION)
        {
            array_elt_addr = (rpc_void_p_t)((idl_byte *)(array_addr)
                                             + element_size * array_elt_offset);
            rpc_ss_ndr_u_enc_union_or_ptees(array_elt_addr,
                                                element_defn_index,
                                                idl_true, IDL_msp );
        }
        else
        {
            array_elt_addr = (rpc_void_p_t)((rpc_void_p_t *)(array_addr)
                                                         + array_elt_offset);
            rpc_ss_ndr_unmar_pointee( base_type, defn_vec_ptr, NULL,
                                          (rpc_void_p_t *)array_elt_addr,
                                          IDL_msp );
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
/* Unmarshall the pointees of a FORTRAN fixed or conformant array             */
/* Used when the array is multi-dimensional,                                  */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_u_for_foc_arr_ptees
#ifdef IDL_PROTOTYPES
(
    /* [in] */  rpc_void_p_t array_addr,
    /* [in] */  idl_ulong_int dimensionality,
    /* [in] */  idl_ulong_int *Z_values,
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
( array_addr, dimensionality, Z_values, base_type, element_size,
 element_defn_index, defn_vec_ptr, IDL_msp )
    rpc_void_p_t array_addr;
    idl_ulong_int dimensionality;
    idl_ulong_int *Z_values;
    idl_byte base_type;
    idl_ulong_int element_size;
    idl_ulong_int element_defn_index;
    idl_byte *defn_vec_ptr;
    IDL_msp_t IDL_msp;
#endif
{
    IDL_bound_pair_t range_list[IDL_FORTRAN_DIMS];

    rpc_ss_range_list_from_Z_values(dimensionality, Z_values, range_list);
    rpc_ss_ndr_u_fortran_arr_ptees(array_addr, dimensionality, Z_values,
                                   range_list, base_type, element_size,
                                   element_defn_index, defn_vec_ptr, IDL_msp);
}

#ifdef VMS

/******************************************************************************/
/*                                                                            */
/* Fill in the fields of a VMS character array descriptor                     */
/*                                                                            */
/******************************************************************************/
static void rpc_ss_complete_array_desc
(
    /* [in,out] */ IDL_array_desc_t *p_desc,       /* Pointer to descriptor */
    /* [in] */  idl_ulong_int dimensionality,
    /* [in] */  IDL_bound_pair_t *bounds_list,
    /* [in] */  idl_boolean is_string,   /* TRUE => string or array of string */
    /* [in] */  idl_byte *array_defn_ptr,    /* points at array base info */
    IDL_msp_t IDL_msp
)
{
    int i;
    unsigned long slice_size;
    long diff_between_A0_and_array_addr;
    idl_ulong_int *Z_values;

    p_desc->prototype.dsc$b_dtype = DSC$K_DTYPE_T;
    p_desc->prototype.dsc$b_scale = 0;
    p_desc->prototype.dsc$b_digits = 0;
    *(char *)&(p_desc->prototype.dsc$b_aflags) = 0;
    p_desc->prototype.dsc$b_aflags.dsc$v_fl_column = 1;
    p_desc->prototype.dsc$b_dimct = dimensionality;

    /* Descriptor multipliers are the same thing as our Z_values */
    Z_values = (idl_ulong_int *)(&(p_desc->optional[1]));
    rpc_ss_Z_values_from_bounds(bounds_list, dimensionality, &Z_values,
                                                                   IDL_msp);
    if (is_string)
    {
        /* The last Z_value becomes the string length. The cell it occupies
            in the descriptor will get overwritten or is not used */
        p_desc->prototype.dsc$w_length =
                                 IDL_DESC_MULT_FIELD(p_desc, dimensionality-1);
        /* Strings may have a base type other than char */
        p_desc->prototype.dsc$w_length *=
                                     rpc_ss_type_size(array_defn_ptr, IDL_msp);
        /* What is dimensionality of array of strings? */
        dimensionality--;
        if (dimensionality == 0)
        {
            /* String, not array of strings */
            p_desc->prototype.dsc$b_class = DSC$K_CLASS_S;
            return;
        }
    }
    else
        p_desc->prototype.dsc$w_length = 1;
    p_desc->prototype.dsc$b_class = DSC$K_CLASS_A;
    /* Calculate array size, copy bounds and calculate the A0 field, 
        using the equivalence at the bottom of page 2-25
        of Introduction to VMS System Routines */
    slice_size = p_desc->prototype.dsc$w_length;
    diff_between_A0_and_array_addr = 0;
    for (i=0; i<dimensionality; i++)
    {
        diff_between_A0_and_array_addr -= bounds_list[i].lower * slice_size;
        slice_size *= IDL_DESC_MULT_FIELD(p_desc, i);
        IDL_DESC_LOWER_BOUND(p_desc, dimensionality, i) = bounds_list[i].lower;
        IDL_DESC_UPPER_BOUND(p_desc, dimensionality, i) = bounds_list[i].upper;
    }
    p_desc->prototype.dsc$l_arsize = slice_size;
    IDL_DESC_A0_FIELD(p_desc) = (long)(p_desc->prototype.dsc$a_pointer
                                        + diff_between_A0_and_array_addr);
}

/******************************************************************************/
/*                                                                            */
/* Given the index in the type vector of the description of a fixed size      */
/*  character array parameter, allocate the array and fill in the descriptor  */
/*  that is the parameter                                                     */
/*                                                                            */
/******************************************************************************/
void rpc_ss_build_fixed_array_desc
(
    /* [in] */  idl_ulong_int type_vec_index,
    /* [in] */  rpc_void_p_t IDL_param_vector[],
    IDL_msp_t IDL_msp
)
{
    idl_byte *type_vec_ptr;
    idl_ulong_int param_index;
    rpc_void_p_t param_addr;
    idl_byte array_type;
    idl_ulong_int array_defn_index;
    idl_byte *array_defn_ptr;
    idl_ulong_int array_dims;
    idl_ulong_int Z_value_storage[IDL_FORTRAN_DIMS];
    idl_ulong_int *Z_values = Z_value_storage;
    IDL_bound_pair_t *bounds_list;
    idl_boolean is_string;   /* TRUE => string or array of string */

    type_vec_ptr = (IDL_msp->IDL_type_vec) + type_vec_index;
    IDL_GET_LONG_FROM_VECTOR(param_index,type_vec_ptr);
    param_addr = IDL_param_vector[param_index];
    array_type = *type_vec_ptr;
    if (array_type == IDL_DT_PASSED_BY_REF)
    {
        type_vec_ptr++;
        array_type = *type_vec_ptr;
    }
    is_string = (array_type == IDL_DT_STRING);
    if (is_string)
    {
        type_vec_ptr++;
        array_type = *type_vec_ptr;
    }
    type_vec_ptr += 2;         /* Skip over DT...ARRAY, properties byte */
    IDL_DISCARD_LONG_FROM_VECTOR(type_vec_ptr);
                                            /* Discard full array definition */
    IDL_GET_LONG_FROM_VECTOR(array_defn_index,type_vec_ptr);
    array_defn_ptr = IDL_msp->IDL_type_vec + array_defn_index; 
    array_dims = (idl_ulong_int)*array_defn_ptr;
    array_defn_ptr++;
#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)
    rpc_ss_fixed_bounds_from_vector(dimensionality, array_defn_ptr,
                                                         &bounds_list, IDL_msp);
#else
    bounds_list = (IDL_bound_pair_t *)array_defn_ptr;
#endif
    rpc_ss_Z_values_from_bounds(bounds_list, array_dims, &Z_values, IDL_msp);
    array_defn_ptr += array_dims * IDL_FIXED_BOUND_PAIR_WIDTH;
    if (array_type == IDL_DT_VARYING_ARRAY)
        array_defn_ptr += array_dims * IDL_DATA_LIMIT_PAIR_WIDTH;

    /* Allocate the array and put its address into the descriptor */
    rpc_ss_ndr_alloc_storage( 0, array_dims, Z_values, array_defn_ptr,
           (rpc_void_p_t *)&(((IDL_array_desc_t *)IDL_param_vector[param_index])
                                ->prototype.dsc$a_pointer), IDL_msp );

    /* Fill in the rest of the descriptor */
    rpc_ss_complete_array_desc(IDL_param_vector[param_index], array_dims,
                               bounds_list, is_string, array_defn_ptr, IDL_msp);
#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)
    rpc_ss_mem_item_free(&IDL_msp->IDL_mem_handle, (byte_p_t)bounds_list);
#endif
}

#endif

/******************************************************************************/
/*                                                                            */
/*  The input list is the set of conformant array parameters whose            */
/*  base type is represented in FORTRAN as CHARACTER*n. For each member of    */
/*  the list,                                                                 */
/*            on VMS fill in the descriptor that is the parameter             */
/*            on OSF fill in the associated hidden parameter                  */
/*                                                                            */
/******************************************************************************/
void rpc_ss_complete_conf_arr_descs
#ifdef IDL_PROTOTYPES
(
    /* [in] */ idl_ulong_int num_conf_char_arrays,
                                 /* Number of conformant char arrays */
    /* [in] */ idl_byte **conf_char_array_list,    /* List of pointers to 
                            type vec entries for conformant character arrays */
    /* [in] */  rpc_void_p_t IDL_param_vector[],
    IDL_msp_t IDL_msp
)
#else
(num_conf_char_arrays, conf_char_array_list, IDL_param_vector, IDL_msp)
    idl_ulong_int num_conf_char_arrays;
    idl_byte **conf_char_array_list;
    rpc_void_p_t IDL_param_vector[];
    IDL_msp_t IDL_msp;
#endif
{
    idl_byte *type_vec_ptr;
    idl_ulong_int param_index;
    rpc_void_p_t param_addr;
    idl_ulong_int array_defn_index;
    idl_byte *array_defn_ptr;
    idl_ulong_int array_dims;
    IDL_bound_pair_t bounds_list_storage[IDL_FORTRAN_DIMS];
    IDL_bound_pair_t *bounds_list = bounds_list_storage;
    idl_ulong_int i;
    idl_byte array_type;
    idl_boolean is_string;   /* TRUE => string or array of string */
#ifndef VMS
    int charsize;           /* Value to be stored in hidden parameter */
    idl_ulong_int struct_defn_index;
    idl_byte *struct_defn_ptr;
    idl_ulong_int offset_index;
#endif

    for (i=0; i<num_conf_char_arrays; i++)
    {
        type_vec_ptr = conf_char_array_list[i];
        IDL_GET_LONG_FROM_VECTOR(param_index,type_vec_ptr);
        param_addr = IDL_param_vector[param_index];
        array_type = *type_vec_ptr;
        if (array_type == IDL_DT_PASSED_BY_REF)
        {
            type_vec_ptr++;
            array_type = *type_vec_ptr;
        }
        if (array_type == IDL_DT_ALLOCATE)
        {
            type_vec_ptr++;
            array_type = *type_vec_ptr;
        }
        is_string = (array_type == IDL_DT_STRING);
        if (is_string)
        {
            type_vec_ptr++;
            array_type = *type_vec_ptr;
        }
        type_vec_ptr += 2;         /* Skip over DT...ARRAY, properties byte */
        IDL_DISCARD_LONG_FROM_VECTOR(type_vec_ptr);
                                            /* Discard full array definition */
        IDL_GET_LONG_FROM_VECTOR(array_defn_index,type_vec_ptr);
        array_defn_ptr = IDL_msp->IDL_type_vec + array_defn_index; 
        array_dims = (idl_ulong_int)*array_defn_ptr;
        array_defn_ptr++;
        rpc_ss_build_bounds_list(&array_defn_ptr,
#ifdef VMS
            (rpc_void_p_t)(((IDL_array_desc_t *)IDL_param_vector[param_index])
                                                    ->prototype.dsc$a_pointer),
#else
            (rpc_void_p_t)(((IDL_fortran_conf_char_arr_t *)
                                    IDL_param_vector[param_index])->p_data),
#endif
            NULL, NULL, array_dims, &bounds_list, IDL_msp);
        if (array_type == IDL_DT_OPEN_ARRAY)
            array_defn_ptr += array_dims * IDL_DATA_LIMIT_PAIR_WIDTH;

#ifdef VMS
        /* Fill in the rest of the descriptor */
        rpc_ss_complete_array_desc(IDL_param_vector[param_index], array_dims,
                                   bounds_list, is_string, array_defn_ptr,
                                   IDL_msp);
#else
        charsize = bounds_list[array_dims - 1].upper
                    + bounds_list[array_dims - 1].lower + 1;
        if (*array_defn_ptr == IDL_DT_FIXED_STRUCT)
        {
            /* Multi-byte character */
            array_defn_ptr++;
            IDL_GET_LONG_FROM_VECTOR(struct_defn_index, array_defn_ptr);
            struct_defn_ptr = IDL_msp->IDL_type_vec + struct_defn_index;
            IDL_GET_LONG_FROM_VECTOR(offset_index, struct_defn_ptr);
            /* Scale size by character width (size of structure) */
            charsize *= *(IDL_msp->IDL_offset_vec + offset_index);
        }
        *(((IDL_fortran_conf_char_arr_t *)IDL_param_vector[param_index])
                                                        ->p_length) = charsize;
#endif
    }
}
