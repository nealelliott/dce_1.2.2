/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ndrmi5.c,v $
 * Revision 1.1.9.2  1996/02/18  18:55:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:18  marty]
 *
 * Revision 1.1.9.1  1995/12/07  22:30:13  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:07 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:15:56  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:49:25  bfc
 * 	oct 95 idl drop
 * 	[1995/10/22  23:36:08  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/22  22:57:38  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE R1.1
 * 	[1995/10/21  18:25:42  bfc]
 * 
 * Revision 1.1.4.2  1994/07/13  18:56:23  tom
 * 	Bug 10103 - Reduce little-endian bias of stubs
 * 	[1994/07/13  18:55:03  tom]
 * 
 * Revision 1.1.4.1  1994/06/21  21:01:28  cbrooks
 * 	CR10496 - I-char marshalling and unmarshalling fail for varying array
 * 	[1994/06/21  20:58:11  cbrooks]
 * 
 * Revision 1.1.2.3  1993/08/20  16:02:18  hinxman
 * 	OT 8484 - Large arrays of [cs_char]s
 * 	[1993/08/20  16:01:29  hinxman]
 * 
 * Revision 1.1.2.2  1993/07/07  20:09:26  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:38:33  ganni]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) Digital Equipment Corporation, 1993
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
**      ndrmi5.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      NDR marshalling routines for - International characters
**
*/

#include <dce/idlddefs.h>
#include <ndrmi.h>
#include <lsysdep.h>

#ifndef _KERNEL

/******************************************************************************/
/*                                                                            */
/*  Fill in the cs-shadow element associated with an array of [cs_char]       */
/*  Note that all such arrays have dimension 1 and lower bound and data limit */
/*      fixed                                                                 */
/*                                                                            */
/******************************************************************************/

static void rpc_ss_ndr_m_array_shadow(
#ifdef IDL_PROTOTYPES
    rpc_void_p_t struct_addr,   /* [in] Address of struct array is a field of.
                                    NULL if array is a parameter */
    idl_ulong_int *struct_offset_vec_ptr,   /* [in] Base of offset vector for
                                structure.  NULL if array is parameter */
    idl_ulong_int *offset_vec_ptr,  /* [in] NULL if array is parameter */
    IDL_cs_shadow_elt_t *cs_shadow, /* [in] */
    idl_ulong_int shadow_index,     /* [in] index into cs-shadow */
    idl_byte **p_defn_vec_ptr,      /* [in] Points after IDL_DT_CS_ARRAY
                                    /* [out] Points after array defn indices */
    IDL_msp_t IDL_msp
)
#else
struct_addr, struct_offset_vec_ptr, offset_vec_ptr, cs_shadow, shadow_index,
 p_defn_vec_ptr, IDL_msp)
    rpc_void_p_t struct_addr;
    idl_ulong_int *struct_offset_vec_ptr;
    idl_ulong_int *offset_vec_ptr;
    IDL_cs_shadow_elt_t *cs_shadow;
    idl_ulong_int shadow_index;
    idl_byte **p_defn_vec_ptr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_byte *defn_vec_ptr;
    idl_byte array_type;
    idl_ulong_int array_defn_index;
    idl_byte *array_defn_ptr;
    idl_boolean allocate;       /* TRUE => [in] size for [out] array */
    IDL_bound_pair_t *bounds_list;
    idl_ulong_int cs_type_defn_index;
    idl_byte *cs_type_defn_ptr;
    idl_ulong_int routine_index;
    void (**routine_ptr)();
    idl_byte ln_type;           /* Data type of [length_is] item */
    idl_ulong_int ln_index;     /* Index in shadow of [length_is] item */
    idl_byte sz_type;           /* Data type of [size_is] item */
    idl_ulong_int sz_index;     /* Index in shadow of [size_is] item */
    /* Parameters for ..._net_size */
    idl_ulong_int l_storage_len;
    idl_ulong_int w_storage_len;
    idl_cs_convert_t convert_type;
    /* Parameters for ..._to_netcs */
    rpc_void_p_t ldata;
    idl_ulong_int l_data_len;
    rpc_void_p_t wdata;
    idl_ulong_int w_data_len;

    /* begin */

    defn_vec_ptr = *p_defn_vec_ptr;
    cs_shadow[shadow_index].IDL_release = idl_false;
    array_type = *defn_vec_ptr;
    defn_vec_ptr++;
    allocate = (array_type == IDL_DT_ALLOCATE);

    if (allocate)
    {
        array_type = *defn_vec_ptr;
        defn_vec_ptr++;
    }

    defn_vec_ptr++;         /* Properties byte */
    IDL_DISCARD_LONG_FROM_VECTOR(defn_vec_ptr);    /* Full array definition */
    IDL_GET_LONG_FROM_VECTOR(array_defn_index, defn_vec_ptr);

    if (array_type == IDL_DT_FIXED_ARRAY)
    {
        /* No shadow for fixed array of [cs_char] */
        *p_defn_vec_ptr = defn_vec_ptr;
        return;
    }

    /* Address of local form of array */
    if (struct_addr != NULL)
        ldata = (rpc_void_p_t)((idl_byte *)struct_addr + *offset_vec_ptr);
    else
        ldata = IDL_msp->IDL_param_vec[shadow_index + 1];

    array_defn_ptr = IDL_msp->IDL_type_vec + array_defn_index;
    array_defn_ptr++;       /* dimensionality */

    /* Bounds information */

    if (array_type == IDL_DT_VARYING_ARRAY)
    {
      if (IDL_msp->IDL_type_vec[TVEC_INT_REP_OFFSET] != NDR_LOCAL_INT_REP)
        rpc_ss_fixed_bounds_from_vector(1, array_defn_ptr, &bounds_list,
                                        IDL_msp);
      else
        bounds_list = (IDL_bound_pair_t *)array_defn_ptr;
        array_defn_ptr += IDL_FIXED_BOUND_PAIR_WIDTH;
        l_storage_len = bounds_list[0].upper - bounds_list[0].lower + 1;
    }
    else    /* Conformant or open */
    {
        /* Skip over lower bound entirely and upper bound kind */
        array_defn_ptr += IDL_CONF_BOUND_PAIR_WIDTH/2 + 1;
        sz_type = *array_defn_ptr;
        IDL_GET_LONG_FROM_VECTOR(sz_index, array_defn_ptr);
        if (struct_addr != NULL)
        {
            l_storage_len = rpc_ss_get_typed_integer(sz_type,
                     (rpc_void_p_t)((idl_byte *)struct_addr 
                                        + *(struct_offset_vec_ptr + sz_index)),
                     IDL_msp);
        }
        else
        {
            l_storage_len = rpc_ss_get_typed_integer(sz_type,
                                     IDL_msp->IDL_param_vec[sz_index], IDL_msp);
        }
        sz_index--;     /* Shadow has one less elt than param or offset vec */
    }

    /* Data limit information */

    if ((array_type == IDL_DT_VARYING_ARRAY)
        || (array_type == IDL_DT_OPEN_ARRAY))
    {
        /* Skip over lower data limit entirely and upper data limit kind */
        array_defn_ptr += IDL_DATA_LIMIT_PAIR_WIDTH/2 + 1;
        ln_type = *array_defn_ptr;
        IDL_GET_LONG_FROM_VECTOR(ln_index, array_defn_ptr);
        if (struct_addr != NULL)
        {
            l_data_len = rpc_ss_get_typed_integer(ln_type,
                     (rpc_void_p_t)((idl_byte *)struct_addr 
                                        + *(struct_offset_vec_ptr + ln_index)),
                     IDL_msp);
        }
        else
        {
            l_data_len = rpc_ss_get_typed_integer(ln_type,
                                     IDL_msp->IDL_param_vec[ln_index], IDL_msp);
        }
        ln_index--;     /* Shadow has one less elt than param or offset vec */
    }
    else
        l_data_len = l_storage_len;

    /* array_defn_ptr is now pointing to the base type, which has [cs_char] */

    array_defn_ptr += 2;       /* IDL_DT_CS_TYPE and properties byte */
    IDL_GET_LONG_FROM_VECTOR(cs_type_defn_index, array_defn_ptr);
    cs_type_defn_ptr = IDL_msp->IDL_type_vec + cs_type_defn_index;
    IDL_DISCARD_LONG_FROM_VECTOR(cs_type_defn_ptr); /* Size of local type */
    IDL_GET_LONG_FROM_VECTOR(routine_index, cs_type_defn_ptr);
    routine_ptr = IDL_msp->IDL_rtn_vec + routine_index;

    /* Call ..._net_size */

    (*(routine_ptr + IDL_RTN_NET_SIZE_INDEX))(IDL_msp->IDL_h,
            *(IDL_msp->IDL_cs_tags_p->p_marsh_tag),
            l_storage_len,
            &convert_type,
            (array_type == IDL_DT_VARYING_ARRAY) ? NULL : &w_storage_len,
            &IDL_msp->IDL_status);

    if (IDL_msp->IDL_status != error_status_ok)
        RAISE(rpc_x_ss_pipe_comm_error);

    if ((array_type == IDL_DT_CONF_ARRAY)
        || (array_type == IDL_DT_OPEN_ARRAY))
    {
        rpc_ss_put_typed_integer(w_storage_len, sz_type,
                          (rpc_void_p_t)&cs_shadow[sz_index].IDL_data, IDL_msp);
    }

    if (allocate)
        goto common_return;

    if (convert_type == idl_cs_no_convert)
    {
        cs_shadow[shadow_index].IDL_data.IDL_storage_p = ldata;
	w_data_len = l_data_len ; /* wire data length same as local */
        goto alloc_return;
    }

    if (array_type == IDL_DT_VARYING_ARRAY)
    {
        w_storage_len = l_storage_len;
    }

    /* Allocate a conversion buffer
                 - cs_type_defn_ptr now points at network type */

    wdata = (idl_void_p_t)rpc_ss_mem_alloc(&IDL_msp->IDL_mem_handle,
                 w_storage_len * rpc_ss_type_size(cs_type_defn_ptr, IDL_msp));
    cs_shadow[shadow_index].IDL_data.IDL_storage_p = wdata;
    cs_shadow[shadow_index].IDL_release = true;

    /* Call ..._to_netcs */

    (*(routine_ptr + IDL_RTN_TO_NETCS_INDEX))(IDL_msp->IDL_h,
            *(IDL_msp->IDL_cs_tags_p->p_marsh_tag),
            ldata,
            l_data_len,
            wdata,
            (array_type == IDL_DT_CONF_ARRAY) ? NULL : &w_data_len,
            &IDL_msp->IDL_status);
    if (IDL_msp->IDL_status != error_status_ok)
        RAISE(rpc_x_ss_pipe_comm_error);

  alloc_return:;

    if ((array_type == IDL_DT_VARYING_ARRAY)
        || (array_type == IDL_DT_OPEN_ARRAY))
    {
        rpc_ss_put_typed_integer(w_data_len, ln_type,
                          (rpc_void_p_t)&cs_shadow[ln_index].IDL_data, IDL_msp);
    }

  common_return:
    ;

    *p_defn_vec_ptr = defn_vec_ptr;

    if (IDL_msp->IDL_type_vec[TVEC_INT_REP_OFFSET] != NDR_LOCAL_INT_REP)
    if (array_type == IDL_DT_VARYING_ARRAY)
        rpc_ss_mem_item_free(&IDL_msp->IDL_mem_handle, (byte_p_t)bounds_list);
    return ;
}

/******************************************************************************/
/*                                                                            */
/*  Build a cs-shadow for marshalling a structure                             */
/*                                                                            */
/******************************************************************************/

void rpc_ss_ndr_m_struct_cs_shadow(
#ifdef IDL_PROTOTYPES

    rpc_void_p_t struct_addr,           /* [in] Address of struct */
    idl_byte struct_type,               /* [in] FIXED_STRUCT or CONF_STRUCT */
    idl_ulong_int shadow_length,        /* [in] Number of structure fields */
    idl_ulong_int offset_index,         /* [in] Start of struct's offset vec */
    idl_byte *defn_vec_ptr,             /* [in] Posn following shadow length */
    IDL_cs_shadow_elt_t **p_cs_shadow,  /* [out] Address of cs-shadow */
    IDL_msp_t IDL_msp
)
#else
struct_addr, struct_type, shadow_length, offset_index, defn_vec_ptr,
 p_cs_shadow, IDL_msp)
    rpc_void_p_t struct_addr;
    idl_byte struct_type;
    idl_ulong_int shadow_length;
    idl_ulong_int offset_index;
    idl_byte *defn_vec_ptr;
    IDL_cs_shadow_elt_t **p_cs_shadow;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int *struct_offset_vec_ptr; /* Start of offsets for this struct */
    idl_ulong_int *offset_vec_ptr;
    idl_byte type_byte;
    IDL_cs_shadow_elt_t *cs_shadow;
    idl_ulong_int shadow_index;     /* Index into cs-shadow */

    /* Allocate the cs-shadow */
    cs_shadow = (IDL_cs_shadow_elt_t *)rpc_ss_mem_alloc
        (&IDL_msp->IDL_mem_handle, shadow_length * sizeof(IDL_cs_shadow_elt_t));

    struct_offset_vec_ptr = IDL_msp->IDL_offset_vec + offset_index;
    offset_vec_ptr = struct_offset_vec_ptr + 1;
                                        /* Skip over size at start of offsets */

    shadow_index = 0;
    do {
        type_byte = *defn_vec_ptr;
        defn_vec_ptr++;
        switch(type_byte)
        {
            case IDL_DT_CS_ARRAY:
                rpc_ss_ndr_m_array_shadow(struct_addr, struct_offset_vec_ptr,
                        offset_vec_ptr, cs_shadow, shadow_index, &defn_vec_ptr,
                        IDL_msp);
                shadow_index++;
                offset_vec_ptr++;
                break;
            /* For fields that are not array of [cs_char],
             advance the definition and offset pointers and shadow index 
             in step. And note they have no translation storage */
            case IDL_DT_BYTE:
            case IDL_DT_CHAR:
            case IDL_DT_BOOLEAN:
            case IDL_DT_DOUBLE:
            case IDL_DT_ENUM:
            case IDL_DT_FLOAT:
            case IDL_DT_SMALL:
            case IDL_DT_SHORT:
            case IDL_DT_LONG:
            case IDL_DT_HYPER:
            case IDL_DT_USMALL:
            case IDL_DT_USHORT:
            case IDL_DT_ULONG:
            case IDL_DT_UHYPER:
            case IDL_DT_IGNORE:
            case IDL_DT_V1_ENUM:
            case IDL_DT_ERROR_STATUS:
                offset_vec_ptr++;
                cs_shadow[shadow_index].IDL_release = idl_false;
                shadow_index++;
                break;
            case IDL_DT_FIXED_ARRAY:
            case IDL_DT_VARYING_ARRAY:
            case IDL_DT_CONF_ARRAY:
            case IDL_DT_OPEN_ARRAY:
                /* Properties byte */
                defn_vec_ptr++;
                IDL_DISCARD_LONG_FROM_VECTOR(defn_vec_ptr);
                                                    /* Full array definition */
                IDL_DISCARD_LONG_FROM_VECTOR(defn_vec_ptr);
                                                /* Flattened array definition */
                offset_vec_ptr++;
                cs_shadow[shadow_index].IDL_release = idl_false;
                shadow_index++;
                break;
            case IDL_DT_ENC_UNION:
            case IDL_DT_N_E_UNION:
            case IDL_DT_FULL_PTR:
            case IDL_DT_UNIQUE_PTR:
            case IDL_DT_REF_PTR:
            case IDL_DT_TRANSMIT_AS:
            case IDL_DT_REPRESENT_AS:
            case IDL_DT_CS_TYPE:
                /* Properties byte */
                defn_vec_ptr++;
                IDL_DISCARD_LONG_FROM_VECTOR(defn_vec_ptr);
                offset_vec_ptr++;
                cs_shadow[shadow_index].IDL_release = idl_false;
                shadow_index++;
                break;
            case IDL_DT_STRING:
            case IDL_DT_NDR_ALIGN_2:
            case IDL_DT_NDR_ALIGN_4:
            case IDL_DT_NDR_ALIGN_8:
            case IDL_DT_BEGIN_NESTED_STRUCT:
            case IDL_DT_END_NESTED_STRUCT:
            case IDL_DT_V1_ARRAY:
            case IDL_DT_V1_STRING:
            case IDL_DT_CS_ATTRIBUTE:
            case IDL_DT_CS_RLSE_SHADOW:
            case IDL_DT_EOL:
                break;
            default:
#ifdef DEBUG_INTERP
                printf("rpc_ss_ndr_m_struct_cs_shadow:unrecognized type %d\n",
                        type_byte);
                exit(0);
#endif
                RAISE(rpc_x_coding_error);
        }
    } while (type_byte != IDL_DT_EOL);


    /* Give the caller the address of the cs-shadow */
    *p_cs_shadow = cs_shadow;
}

/******************************************************************************/
/*                                                                            */
/*  Marshall a fixed array of [cs_char] or a [cs_char] which is not in an     */
/*  array                                                                     */
/*                                                                            */
/******************************************************************************/
static void rpc_ss_ndr_m_cs_farr_or_single
#ifdef IDL_PROTOTYPES
(
    rpc_void_p_t data_addr,             /* [in] Address of array or char */
    IDL_bound_pair_t *bounds_list,      /* [in] - for array or char treated
                                                    as array */
    idl_ulong_int cs_type_defn_index,   /* [in] */
    idl_ulong_int l_storage_len,        /* [in] number of elements */
    IDL_msp_t IDL_msp
)
#else
(data_addr, bounds_list, cs_type_defn_index, l_storage_len, IDL_msp)
    rpc_void_p_t data_addr;
    IDL_bound_pair_t *bounds_list;
    idl_ulong_int cs_type_defn_index;
    idl_ulong_int l_storage_len;
    IDL_msp_t IDL_msp;
#endif
{
    idl_byte *cs_type_defn_ptr;
    idl_ulong_int routine_index;
    void (**routine_ptr)();
    /* Parameters for ..._net_size */
    idl_cs_convert_t convert_type;
    /* Parameters for ..._to_netcs */
    rpc_void_p_t wdata;

    cs_type_defn_ptr = IDL_msp->IDL_type_vec + cs_type_defn_index;
    IDL_DISCARD_LONG_FROM_VECTOR(cs_type_defn_ptr); /* Size of local type */
    IDL_GET_LONG_FROM_VECTOR(routine_index, cs_type_defn_ptr);
    routine_ptr = IDL_msp->IDL_rtn_vec + routine_index;

    /* Call ..._net_size */
    (*(routine_ptr + IDL_RTN_NET_SIZE_INDEX))(IDL_msp->IDL_h,
            *(IDL_msp->IDL_cs_tags_p->p_marsh_tag),
            l_storage_len,
            &convert_type,
            NULL,
            &IDL_msp->IDL_status);
    if (IDL_msp->IDL_status != error_status_ok)
        RAISE(rpc_x_ss_pipe_comm_error);

    if (convert_type == idl_cs_no_convert)
    {
        rpc_ss_ndr_m_fix_or_conf_arr(data_addr, 1, bounds_list,
                                        cs_type_defn_ptr, 0, IDL_msp);
    }
    else
    {
        /* Allocate a conversion buffer
                 - cs_type_defn_ptr now points at network type */
        wdata = (idl_void_p_t)rpc_ss_mem_alloc(&IDL_msp->IDL_mem_handle,
                 l_storage_len * rpc_ss_type_size(cs_type_defn_ptr, IDL_msp));
        /* Call ..._to_netcs */
        (*(routine_ptr + IDL_RTN_TO_NETCS_INDEX))(IDL_msp->IDL_h,
                *(IDL_msp->IDL_cs_tags_p->p_marsh_tag),
                data_addr,
                l_storage_len,
                wdata,
                NULL,
                &IDL_msp->IDL_status);
        if (IDL_msp->IDL_status != error_status_ok)
            RAISE(rpc_x_ss_pipe_comm_error);
        /* Marshall the converted data */
        rpc_ss_ndr_m_fix_or_conf_arr(wdata, 1, bounds_list,
                                 cs_type_defn_ptr, IDL_M_DO_NOT_POINT, IDL_msp);
        /* Release conversion buffer */
        rpc_ss_mem_item_free(&IDL_msp->IDL_mem_handle, (byte_p_t)wdata);
    }
}

/******************************************************************************/
/*                                                                            */
/*  Marshall a fixed array of [cs_char]                                       */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_m_fixed_cs_array
#ifdef IDL_PROTOTYPES
(
    rpc_void_p_t array_addr,        /* [in] Address of array */
    idl_byte **p_defn_vec_ptr,      /* [in] Points at DT_FIXED_ARRAY */
                                    /* [out] Points after array defn */
    IDL_msp_t IDL_msp
)
#else
(array_addr, p_defn_vec_ptr, IDL_msp)
    rpc_void_p_t array_addr;
    idl_byte **p_defn_vec_ptr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_byte *defn_vec_ptr;
    idl_ulong_int array_defn_index;
    idl_byte *array_defn_ptr;
    IDL_bound_pair_t *bounds_list;
    idl_ulong_int cs_type_defn_index;

    defn_vec_ptr = *p_defn_vec_ptr;
    defn_vec_ptr += 2;      /* DT_FIXED_ARRAY and properties byte */
    IDL_DISCARD_LONG_FROM_VECTOR(defn_vec_ptr);    /* Full array definition */
    IDL_GET_LONG_FROM_VECTOR(array_defn_index, defn_vec_ptr);
    array_defn_ptr = IDL_msp->IDL_type_vec + array_defn_index;
    array_defn_ptr++;       /* dimensionality */
    if (IDL_msp->IDL_type_vec[TVEC_INT_REP_OFFSET] != NDR_LOCAL_INT_REP)
     rpc_ss_fixed_bounds_from_vector(1, array_defn_ptr, &bounds_list,
                                        IDL_msp);
    else
     bounds_list = (IDL_bound_pair_t *)array_defn_ptr;
    array_defn_ptr += IDL_FIXED_BOUND_PAIR_WIDTH;
    /* array_defn_ptr is now pointing to the base type, which has [cs_char] */
    array_defn_ptr++;       /* IDL_DT_CS_TYPE */
    IDL_GET_LONG_FROM_VECTOR(cs_type_defn_index, array_defn_ptr);

    rpc_ss_ndr_m_cs_farr_or_single(array_addr, bounds_list, cs_type_defn_index,
                                bounds_list[0].upper - bounds_list[0].lower + 1,
                                     IDL_msp);

    *p_defn_vec_ptr = defn_vec_ptr;
    if (IDL_msp->IDL_type_vec[TVEC_INT_REP_OFFSET] != NDR_LOCAL_INT_REP)
    rpc_ss_mem_item_free(&IDL_msp->IDL_mem_handle, (byte_p_t)bounds_list);
}

/******************************************************************************/
/*                                                                            */
/*  Marshall a [cs_char] which is not an array element                        */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_marsh_cs_char
#ifdef IDL_PROTOTYPES
(
    rpc_void_p_t char_addr,             /* [in] Address */
    idl_ulong_int cs_type_defn_index,   /* [in] */
    IDL_msp_t IDL_msp
)
#else
(char_addr, cs_type_defn_index, IDL_msp)
    rpc_void_p_t char_addr;
    idl_ulong_int cs_type_defn_index;
    IDL_msp_t IDL_msp;
#endif
{
    IDL_bound_pair_t bound_pair;    /* To treat the character as an array */

    bound_pair.upper = 0;
    bound_pair.lower = 0;
    rpc_ss_ndr_m_cs_farr_or_single(char_addr, &bound_pair, cs_type_defn_index,
                                    1, IDL_msp);
}

/******************************************************************************/
/*                                                                            */
/*  Marshall an array of [cs_char]                                            */
/*                                                                            */
/******************************************************************************/

void rpc_ss_ndr_marsh_cs_array(
#ifdef IDL_PROTOTYPES

    rpc_void_p_t array_addr,    /* [in] Used only for fixed arrays */
    IDL_cs_shadow_elt_t *cs_shadow,  /* [in] Address of cs-shadow 
                                            (Not used for fixed arrays) */
    idl_ulong_int shadow_index, /* [in] Index in cs_shadow of array */
    idl_boolean in_struct,      /* [in] TRUE => array is structure field */
    idl_byte **p_defn_vec_ptr,  /* [in] Type following DT_CS_ARRAY
                                   [out] After array indirection words */
    IDL_msp_t IDL_msp
)
#else
array_addr, cs_shadow, shadow_index, in_struct, p_defn_vec_ptr, IDL_msp)
    rpc_void_p_t array_addr;
    IDL_cs_shadow_elt_t *cs_shadow;
    idl_ulong_int shadow_index;
    idl_boolean in_struct;
    idl_byte **p_defn_vec_ptr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_byte array_type;
    idl_byte *defn_vec_ptr = 0 ;
    idl_ulong_int array_defn_index;
    idl_byte *array_defn_ptr = 0 ;
    IDL_bound_pair_t bound_pair;
    IDL_bound_pair_t *bounds_list;
    idl_ulong_int Z_value;
    IDL_bound_pair_t range_pair;
    idl_byte ln_type;           /* Data type of [length_is] item */
    idl_ulong_int ln_index;     /* Index in shadow of [length_is] item */
    idl_byte sz_type;           /* Data type of [size_is] item */
    idl_ulong_int sz_index;     /* Index in shadow of [size_is] item */
    idl_ulong_int cs_type_defn_index;
    idl_byte *cs_type_defn_ptr =0 ;

    /* begin */

    array_type = **p_defn_vec_ptr;
    if (array_type == IDL_DT_FIXED_ARRAY)
    {
        rpc_ss_ndr_m_fixed_cs_array(array_addr, p_defn_vec_ptr, IDL_msp);
        return;
    }

    defn_vec_ptr = *p_defn_vec_ptr;
    defn_vec_ptr += 2;      /* Array type and properties byte */

    IDL_DISCARD_LONG_FROM_VECTOR(defn_vec_ptr);    /* Full array definition */
    IDL_GET_LONG_FROM_VECTOR(array_defn_index, defn_vec_ptr);

    *p_defn_vec_ptr = defn_vec_ptr;

    if (array_type == IDL_DT_ALLOCATE)
    {
        /* Nothing to marshall */
        return;
    }

    /* Marshall from shadow */

    array_defn_ptr = IDL_msp->IDL_type_vec + array_defn_index;
    array_defn_ptr++;       /* dimensionality */

    /* Bounds information */

    if (array_type == IDL_DT_VARYING_ARRAY)
    {
      if (IDL_msp->IDL_type_vec[TVEC_INT_REP_OFFSET] != NDR_LOCAL_INT_REP)
	rpc_ss_fixed_bounds_from_vector(1, array_defn_ptr, &bounds_list,
                                        IDL_msp);
      else
	bounds_list = (IDL_bound_pair_t *)array_defn_ptr;
        array_defn_ptr += IDL_FIXED_BOUND_PAIR_WIDTH;
    }
    else    /* Conformant or varying */
    {
        bound_pair.lower = 1;       /* Fake the bounds as 1..Z */

        /* Skip over lower bound entirely and upper bound kind */

        array_defn_ptr += IDL_CONF_BOUND_PAIR_WIDTH/2 + 1;

        sz_type = *array_defn_ptr;

        IDL_GET_LONG_FROM_VECTOR(sz_index, array_defn_ptr);

        sz_index--;     /* Shadow has one less elt than param or offset vec */

        bound_pair.upper = rpc_ss_get_typed_integer(sz_type,
                            (rpc_void_p_t)&cs_shadow[sz_index].IDL_data,
                            IDL_msp);

        bounds_list = &bound_pair;
        if ( ! in_struct )
        {
            /* Marshall the Z-value */
            IDL_MARSH_LONG(&bound_pair.upper);
        }
    }

    /* Data limit information */

    if ((array_type == IDL_DT_VARYING_ARRAY)
        || (array_type == IDL_DT_OPEN_ARRAY))
    {
        array_defn_ptr++;       /* We know the lower data limit is fixed */
        IDL_GET_LONG_FROM_VECTOR(range_pair.lower, array_defn_ptr);
        array_defn_ptr++;       /* We know upper data limit is [length_is] */
        ln_type = *array_defn_ptr;  /* A */
        IDL_GET_LONG_FROM_VECTOR(ln_index, array_defn_ptr);
        ln_index--;     /* Shadow has one less elt than param or offset vec */
        range_pair.upper = rpc_ss_get_typed_integer(ln_type,
                            (rpc_void_p_t)&cs_shadow[ln_index].IDL_data,
                            IDL_msp);   /* B */
        range_pair.upper += range_pair.lower;   /* A + B */
    }

    /* array_defn_ptr is now pointing to the base type, which has [cs_char] */

    array_defn_ptr++;       /* IDL_DT_CS_TYPE */

    IDL_GET_LONG_FROM_VECTOR(cs_type_defn_index, array_defn_ptr);

    cs_type_defn_ptr = IDL_msp->IDL_type_vec + cs_type_defn_index;

    IDL_DISCARD_LONG_FROM_VECTOR(cs_type_defn_ptr); /* Size of local type */
    IDL_DISCARD_LONG_FROM_VECTOR(cs_type_defn_ptr); /* Routine index */

    /* Now pointing at network type definition */

    /* Marshall the data */

    if (array_type == IDL_DT_CONF_ARRAY)
    {
        rpc_ss_ndr_m_fix_or_conf_arr(
                                cs_shadow[shadow_index].IDL_data.IDL_storage_p,
                                1, bounds_list, cs_type_defn_ptr,
                                IDL_M_DO_NOT_POINT, IDL_msp);
    }
    else
    {
        Z_value = bounds_list[0].upper - bounds_list[0].lower + 1;
        rpc_ss_ndr_m_var_or_open_arr(
                                cs_shadow[shadow_index].IDL_data.IDL_storage_p,
                                &Z_value, 1, &range_pair,
                                cs_type_defn_ptr, IDL_M_DO_NOT_POINT, IDL_msp);
    }

    if (IDL_msp->IDL_type_vec[TVEC_INT_REP_OFFSET] != NDR_LOCAL_INT_REP)
      if (array_type == IDL_DT_VARYING_ARRAY)
        rpc_ss_mem_item_free(&IDL_msp->IDL_mem_handle, (byte_p_t)bounds_list);

    return ;
}

/******************************************************************************/
/*                                                                            */
/*  Release translation buffers pointed to by a cs_shadow, then release the   */
/*  shadow                                                                    */
/*                                                                            */
/******************************************************************************/
void rpc_ss_ndr_m_rlse_cs_shadow
#ifdef IDL_PROTOTYPES
(
    IDL_cs_shadow_elt_t *cs_shadow,  /* [in] Address of cs-shadow */
    idl_ulong_int shadow_length,     /* [in] Number of structure fields */
    IDL_msp_t IDL_msp
)
#else
(cs_shadow, shadow_length, IDL_msp)
    IDL_cs_shadow_elt_t *cs_shadow;
    idl_ulong_int shadow_length;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int i;

    for (i=0; i<shadow_length; i++)
    {
        if (cs_shadow[i].IDL_release)
        {
            rpc_ss_mem_item_free(&IDL_msp->IDL_mem_handle,
                                 (byte_p_t)cs_shadow[i].IDL_data.IDL_storage_p);
        }
    }
    rpc_ss_mem_item_free(&IDL_msp->IDL_mem_handle, (byte_p_t)cs_shadow);
}

/******************************************************************************/
/*                                                                            */
/*  Build a cs-shadow for marshalling a parameter list                        */
/*                                                                            */
/******************************************************************************/

void rpc_ss_ndr_m_param_cs_shadow
#ifdef IDL_PROTOTYPES
(
    idl_byte *type_vec_ptr,     /* [in] After shadow length */
    idl_ulong_int param_index,  /* [in] Index for first parameter */
                                /* Later used for index of current parameter */
    idl_ulong_int shadow_length,    /* [in] Size of cs-shadow */
    IDL_cs_shadow_elt_t **p_cs_shadow,  /* [out] Address of cs-shadow */
    IDL_msp_t IDL_msp
)
#else
(type_vec_ptr, param_index, shadow_length, p_cs_shadow,
 IDL_msp)
    idl_byte *type_vec_ptr;
    idl_ulong_int param_index;
    idl_ulong_int shadow_length;
    IDL_cs_shadow_elt_t **p_cs_shadow;
    IDL_msp_t IDL_msp;
#endif
{
    idl_byte type_byte;
    IDL_cs_shadow_elt_t *cs_shadow;
    idl_ulong_int i;

    /* Allocate the cs-shadow */
    cs_shadow = (IDL_cs_shadow_elt_t *)rpc_ss_mem_alloc
                    (&IDL_msp->IDL_mem_handle, shadow_length
                                                 * sizeof(IDL_cs_shadow_elt_t));
    /* Initialize its "release" fields */
    for (i=0; i<shadow_length; i++)
        cs_shadow[i].IDL_release = idl_false;

    /* Loop over parameters. Exit when DT_RLSE_SHADOW encountered */
    for (i = 0; ; i++)
    {
        if (i != 0)
        {
            /* On entry we already have the parameter index the DT_CS_SHADOW
                was attached to */
            IDL_GET_LONG_FROM_VECTOR(param_index,type_vec_ptr);
        }
        do {
            type_byte = *type_vec_ptr;
            type_vec_ptr++;
            switch(type_byte)
            {
                case IDL_DT_CS_ARRAY:
                    rpc_ss_ndr_m_array_shadow(NULL, NULL, NULL,  cs_shadow, 
                            param_index - 1, &type_vec_ptr, IDL_msp);
                    break;
                /* For parameters that are not array of [cs_char],
                 advance the definition pointer.
                 And note they have no translation storage */
                case IDL_DT_BYTE:
                case IDL_DT_CHAR:
                case IDL_DT_BOOLEAN:
                case IDL_DT_DOUBLE:
                case IDL_DT_ENUM:
                case IDL_DT_FLOAT:
                case IDL_DT_SMALL:
                case IDL_DT_SHORT:
                case IDL_DT_LONG:
                case IDL_DT_HYPER:
                case IDL_DT_USMALL:
                case IDL_DT_USHORT:
                case IDL_DT_ULONG:
                case IDL_DT_UHYPER:
                case IDL_DT_IGNORE:
                case IDL_DT_V1_ENUM:
                case IDL_DT_ERROR_STATUS:
                case IDL_DT_IN_CONTEXT:
                case IDL_DT_IN_OUT_CONTEXT:
                case IDL_DT_OUT_CONTEXT:
                    break;
                case IDL_DT_FIXED_ARRAY:
                case IDL_DT_VARYING_ARRAY:
                case IDL_DT_CONF_ARRAY:
                case IDL_DT_OPEN_ARRAY:
                    /* Properties byte */
                    type_vec_ptr++;
                    IDL_DISCARD_LONG_FROM_VECTOR(type_vec_ptr);
                                                    /* Full array definition */
                    IDL_DISCARD_LONG_FROM_VECTOR(type_vec_ptr);
                                                /* Flattened array definition */
                    break;
                case IDL_DT_FIXED_STRUCT:
                case IDL_DT_CONF_STRUCT:
                case IDL_DT_V1_CONF_STRUCT:
                case IDL_DT_ENC_UNION:
                case IDL_DT_N_E_UNION:
                case IDL_DT_FULL_PTR:
                case IDL_DT_UNIQUE_PTR:
                case IDL_DT_REF_PTR:
                case IDL_DT_TRANSMIT_AS:
                case IDL_DT_REPRESENT_AS:
                case IDL_DT_PIPE:
                case IDL_DT_CS_TYPE:
                    /* Properties byte */
                    type_vec_ptr++;
                    IDL_DISCARD_LONG_FROM_VECTOR(type_vec_ptr);
                    break;
                case IDL_DT_ALLOCATE_REF:
                    rpc_ss_discard_allocate_ref(&type_vec_ptr);
                    break;
                case IDL_DT_FREE_REP:
                    IDL_DISCARD_LONG_FROM_VECTOR(type_vec_ptr);
                    break;
                case IDL_DT_PASSED_BY_REF:
                case IDL_DT_STRING:
                case IDL_DT_ALLOCATE:
                case IDL_DT_V1_ARRAY:
                case IDL_DT_V1_STRING:
                case IDL_DT_DELETED_NODES:
                case IDL_DT_CS_ATTRIBUTE:
                case IDL_DT_EOL:
                    break;
                case IDL_DT_CS_RLSE_SHADOW:
                    *p_cs_shadow = cs_shadow;
                    return;
                default:
#ifdef DEBUG_INTERP
                    printf("rpc_ss_ndr_m_param_cs_shadow:unrecognized type %d\n",
                                type_byte);
                    exit(0);
#endif
                    RAISE(rpc_x_coding_error);
            }
        } while (type_byte != IDL_DT_EOL);
    }
}

/******************************************************************************/
/*                                                                            */
/*  Build the bounds list for a structure with a conformant [cs_char] array   */
/*  field                                                                     */
/*                                                                            */
/******************************************************************************/
void rpc_ss_conf_struct_cs_bounds
#ifdef IDL_PROTOTYPES
(
    idl_byte *defn_vec_ptr,     /* [in] Points at bounds info*/
    IDL_cs_shadow_elt_t *cs_shadow,  /* [in] Address of cs-shadow*/
    IDL_bound_pair_t *bounds_list,   /* [out]*/
    IDL_msp_t IDL_msp
)
#else
(defn_vec_ptr, cs_shadow, bounds_list, IDL_msp)
    idl_byte *defn_vec_ptr;
    IDL_cs_shadow_elt_t *cs_shadow;
    IDL_bound_pair_t *bounds_list;
    IDL_msp_t IDL_msp;
#endif
{
    idl_byte sz_type;           /* Data type of [size_is] item*/
    idl_ulong_int sz_index;     /* Index in shadow of [size_is] item*/

    bounds_list[0].lower = 1;       /* Fake the bounds as 1..Z*/
    /* Skip over lower bound entirely and upper bound kind*/
    defn_vec_ptr += IDL_CONF_BOUND_PAIR_WIDTH/2 + 1;
    sz_type = *defn_vec_ptr;
    IDL_GET_LONG_FROM_VECTOR(sz_index, defn_vec_ptr);
    sz_index--;     /* Shadow has one less elt than offset vec*/
    bounds_list[0].upper = rpc_ss_get_typed_integer(sz_type,
                            (rpc_void_p_t)&cs_shadow[sz_index].IDL_data,
                            IDL_msp);
}

#endif
