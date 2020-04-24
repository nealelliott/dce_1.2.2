/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: miabermi.c,v $
 * Revision 1.1.2.2  1996/03/11  13:25:47  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:55  marty]
 *
 * Revision 1.1.2.1  1995/12/13  17:24:48  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:06 UTC  dat
 * 	New file for second XIDL drop for DCE 1.2.1
 * 	[1995/12/13  17:23:55  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:49:09  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  01:25:35  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/22  22:58:05  bfc]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      miabermi.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Interpreter for marshalling with MIA protocol rules over BER
**
*/

#include <dce/idlddefs.h>
#include <lsysdep.h>
#ifdef VMS
#include <ddis$def.h>
#include <ddis$msg.h>
#include <lib$routines.h>

/* This should be in a mia header, but currently doesn't exist */
typedef idl_byte TPS_STDL_TS_KANJI;
void rpc_ss_kanji_dec_to_iso(TPS_STDL_TS_KANJI *, 
                             ISO_MULTI_LINGUAL *, 
                             idl_ulong_int);
void rpc_ss_kanji_iso_to_dec(ISO_MULTI_LINGUAL *, 
                             TPS_STDL_TS_KANJI *, 
                             idl_ulong_int);
#else
#include <ddis_def.h>
#include <ddis_msg.h>
#endif
#include <ddissupp.h>
#include <stdlparams.h>
#ifdef DEBUG_ALLOC
#include <stdio.h>
#endif

static idl_byte IDL_ddis_dummy_buffer[2];

extern idl_void_p_t IDL_ddis_syntax_table_list[];

idl_boolean IDL_space_strip_ev_xlated = idl_false;
idl_boolean IDL_space_stripping;

/* 
 *  Forward function references
 */
static void rpc_ss_mia_ber_m_fixed_arr_dim
(
#ifdef IDL_PROTOTYPES
    idl_ulong_int dimensionality,
    idl_byte *defn_vec_ptr,
    rpc_void_p_t *p_current_element_addr,
    IDL_msp_t IDL_msp
#endif
);

static void rpc_ss_mia_ber_marsh_fixed_arr
(
#ifdef IDL_PROTOTYPES
    idl_ulong_int defn_index,
    rpc_void_p_t array_addr,
    IDL_msp_t IDL_msp
#endif
);

static void rpc_ss_mia_ber_m_varying_arr
(
#ifdef IDL_PROTOTYPES
    idl_ulong_int defn_index,       /* Posn of full array desc in defn vec */
    rpc_void_p_t array_addr,
    rpc_void_p_t struct_addr,       /* Base address of structure of which
                            array is a field. For MIA it can't be a parameter */
    idl_ulong_int *struct_offset_vec_ptr,   /* Pointer to base of offset vec
                                                for structure */
    IDL_msp_t IDL_msp
#endif
);

/******************************************************************************/
/*                                                                            */
/* Marshall a scalar                                                          */
/*                                                                            */
/******************************************************************************/
static void rpc_ss_mia_ber_marsh_scalar
#ifdef IDL_PROTOTYPES
(
    /* [in] */  idl_byte type_byte,
    /* [in] */  rpc_void_p_t param_addr,    /* address of scalar */
    IDL_msp_t IDL_msp
)
#else
(type_byte, param_addr, IDL_msp)
    idl_byte type_byte;
    rpc_void_p_t param_addr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int stat;
    idl_ulong_int ddis_status;
    idl_ulong_int ddis_type;
    idl_ulong_int ddis_length;
    ISO_MULTI_LINGUAL transmitted_kanji;
    IDL_ddis_ulong_buff_t ddis_ulong_buff;
    IDL_ddis_ushort_buff_t ddis_ushort_buff;
    IDL_ddis_usmall_buff_t ddis_usmall_buff;

    switch(type_byte)
    {
        case IDL_DT_LONG:
            ddis_type = K_T_integer;
            ddis_length = sizeof(idl_long_int);
            break;
        case IDL_DT_ULONG:
            ddis_ulong_buff.uint = *(idl_ulong_int *)param_addr;
            ddis_ulong_buff.extension = 0;
            param_addr = (rpc_void_p_t)&ddis_ulong_buff;
            ddis_type = K_T_integer;
            ddis_length = sizeof(idl_ulong_int) + 1;
            break;
        case IDL_DT_USHORT:
            ddis_ushort_buff.uint = *(idl_ushort_int *)param_addr;
            ddis_ushort_buff.extension = 0;
            param_addr = (rpc_void_p_t)&ddis_ushort_buff;
            ddis_type = K_T_integer;
            ddis_length = sizeof(idl_ushort_int) + 1;
            break;
        case IDL_DT_USMALL:
            ddis_usmall_buff.uint = *(idl_usmall_int *)param_addr;
            ddis_usmall_buff.extension = 0;
            param_addr = (rpc_void_p_t)&ddis_usmall_buff;
            ddis_type = K_T_integer;
            ddis_length = sizeof(idl_usmall_int) + 1;
            break;
        case IDL_DT_STDL_OCTET:
            ddis_type = K_T_octet;
            ddis_length = sizeof(idl_byte);
            break;
        case IDL_DT_STDL_TS_SLATIN:
        case IDL_DT_STDL_TS_SLATIN_ELT:
        case IDL_DT_STDL_TS_LATIN1:
        case IDL_DT_STDL_TS_LATIN1_ELT:
            ddis_type = K_T_latin;
            ddis_length = sizeof(idl_byte);
            break;
        case IDL_DT_STDL_TS_KANA:
        case IDL_DT_STDL_TS_KANA_ELT:
            ddis_type = K_T_katakana;
            ddis_length = sizeof(idl_byte);
            break;
        case IDL_DT_STDL_TS_KANJI:
        case IDL_DT_STDL_TS_KANJI_ELT:
            rpc_ss_kanji_dec_to_iso( param_addr, &transmitted_kanji, 1 );
            param_addr = (rpc_void_p_t)&transmitted_kanji;
            ddis_type = K_T_kanji;
            ddis_length = 2;    /* One Kanji character consists of two bytes */
            break;
        case IDL_DT_STDL_TS_DECIMAL:
        case IDL_DT_STDL_TS_DECIMAL_ELT:
            ddis_type = K_T_decimal_string;
            ddis_length = sizeof(idl_char);
            break;
        default:
#ifdef DEBUG_INTERP
            printf("rpc_ss_mia_ber_marshall_scalar: unrecognized type %d\n",
                        type_byte);
            exit(0);
#endif
            RAISE(rpc_x_coding_error);
    }
#ifdef VMS
    if (ddis_g_entries[IDL_DDIS_PUT] == 0)
    {
   	stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
   	if ((stat & 1) != 1) 
	   RAISE (rpc_x_coding_error);
    }
    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
    				    &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, &ddis_length, param_addr, 0 );
#else
    ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, &ddis_length, param_addr, 0 );
#endif
    if (ddis_status != DDIS$_NORMAL)
        rpc_ss_raise_ddis_error( ddis_status );
}

/******************************************************************************/
/*                                                                            */
/*  Marshall a structure                                                      */
/*                                                                            */
/******************************************************************************/
static void rpc_ss_mia_ber_marsh_struct
#ifdef IDL_PROTOTYPES
(
    /* [in] */  idl_ulong_int defn_index,   /* Index of structure definition */
    /* [in] */  rpc_void_p_t struct_addr,   /* Address of structure */
    /* [in] */  idl_boolean union_hack,     /* TRUE => union arm
            This means that the "start of sequence" for the record has already
            been output, and the first fixed array definition in the structure
            should be ignored since it is the remaining 15 bytes of the UUID
            that has already been marshalled */
    IDL_msp_t IDL_msp
)
#else
(defn_index, struct_addr, union_hack, IDL_msp)
    idl_ulong_int defn_index;
    rpc_void_p_t struct_addr;
    idl_boolean union_hack;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int stat;
    idl_ulong_int ddis_status;
    idl_ulong_int offset_index;
    idl_byte *defn_vec_ptr;
    idl_ulong_int *struct_offset_vec_ptr; /* Start of offsets for this struct */
    idl_ulong_int *offset_vec_ptr;
    idl_byte type_byte;
    idl_ulong_int offset;
    idl_ulong_int ddis_type;
    idl_ulong_int array_defn_index;  /* Index of array definition in defn vec */

    /* Output the "start of sequence" for the structure */
    if ( ! union_hack )
    {
        ddis_type = K_T_stdl_record_type;

#ifdef VMS
        if (ddis_g_entries[IDL_DDIS_PUT] == 0)
        {
   	   stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
   	   if ((stat & 1) != 1) 
	      RAISE (rpc_x_coding_error);
        }
        ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
        			    &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#else
        ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#endif
        if ( ddis_status != DDIS$_NORMAL )
            rpc_ss_raise_ddis_error( ddis_status );
    }

    defn_vec_ptr = IDL_msp->IDL_type_vec + defn_index;
    IDL_GET_LONG_FROM_VECTOR(offset_index,defn_vec_ptr);
    struct_offset_vec_ptr = IDL_msp->IDL_offset_vec + offset_index;
    offset_vec_ptr = struct_offset_vec_ptr + 1;
                                        /* Skip over size at start of offsets */
    do {
        type_byte = *defn_vec_ptr;
        defn_vec_ptr++;
        switch(type_byte)
        {
            case IDL_DT_LONG:
            case IDL_DT_ULONG:
            case IDL_DT_USHORT:
            case IDL_DT_USMALL:
                offset = *offset_vec_ptr;
                offset_vec_ptr++;
                rpc_ss_mia_ber_marsh_scalar(type_byte,
                                    (idl_byte *)struct_addr+offset, IDL_msp);
                break;
            case IDL_DT_NDR_ALIGN_2:
            case IDL_DT_NDR_ALIGN_4:
            case IDL_DT_NDR_ALIGN_8:
                break;
            case IDL_DT_FIXED_ARRAY:
                defn_vec_ptr++;     /* Skip over properties byte */
                IDL_GET_LONG_FROM_VECTOR(array_defn_index, defn_vec_ptr);
                offset = *offset_vec_ptr;
                offset_vec_ptr++;
                if (union_hack)
                    union_hack = false;
                else
                    rpc_ss_mia_ber_marsh_fixed_arr(array_defn_index,
                                    (idl_byte *)struct_addr+offset, IDL_msp);
                IDL_DISCARD_LONG_FROM_VECTOR(defn_vec_ptr);
                                            /* Discard flattened definition */
                break;
            case IDL_DT_VARYING_ARRAY:
                defn_vec_ptr++;     /* Skip over properties byte */
                IDL_GET_LONG_FROM_VECTOR(array_defn_index, defn_vec_ptr);
                offset = *offset_vec_ptr;
                offset_vec_ptr++;
                rpc_ss_mia_ber_m_varying_arr(array_defn_index,
                                    (idl_byte *)struct_addr+offset,
                                   struct_addr, struct_offset_vec_ptr, IDL_msp);
                IDL_DISCARD_LONG_FROM_VECTOR(defn_vec_ptr);
                                              /* Discard flattened definition */
                break;
            case IDL_DT_BEGIN_NESTED_STRUCT:
                /* union_hack is true until we have seen the end of the UUID */
                if ( ! union_hack )
                {
                    ddis_type = K_T_stdl_record_type;
#ifdef VMS
        	    if (ddis_g_entries[IDL_DDIS_PUT] == 0)
        	    {
   	   		stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
   	   		if ((stat & 1) != 1) 
	      		   RAISE(rpc_x_coding_error);
        	    }
        	    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
        			    &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#else
                    ddis_status = ddis$put(&IDL_msp->IDL_ddis_stream_handle,
                                                &ddis_type, 0, 0, 0);
#endif
                    if ( ddis_status != DDIS$_NORMAL )
                        rpc_ss_raise_ddis_error( ddis_status );
                }
                break;
            case IDL_DT_END_NESTED_STRUCT:
                union_hack = idl_false;
                ddis_type = DDIS$K_T_EOC;
#ifdef VMS
        	    if (ddis_g_entries[IDL_DDIS_PUT] == 0)
        	    {
   	   		stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
   	   		if ((stat & 1) != 1) 
	      		   RAISE( rpc_x_coding_error);
        	    }
        	    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
        			    &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#else
                ddis_status = ddis$put(&IDL_msp->IDL_ddis_stream_handle,
                                                &ddis_type, 0, 0, 0);
#endif
                if ( ddis_status != DDIS$_NORMAL )
                    rpc_ss_raise_ddis_error( ddis_status );
                break;
            case IDL_DT_MODIFIED:
                defn_vec_ptr++;   /* Skip over DT_BYTE, DT_CHAR,
                                     or DT_BEGIN_NESTED_STRUCT */
                type_byte = *defn_vec_ptr;  /* Get modifier byte */
                defn_vec_ptr++;
                offset = *offset_vec_ptr;
                offset_vec_ptr++;
                rpc_ss_mia_ber_marsh_scalar(type_byte,
                                    (idl_byte *)struct_addr+offset, IDL_msp);
                if (type_byte == IDL_DT_STDL_TS_KANJI)
                {
                    /* Need to skip over the substructure machinery */
                    defn_vec_ptr += 3;  /* DT_BYTE, DT_BYTE,
                                             DT_END_NESTED_STRUCT */
                    offset_vec_ptr++; /* Address of second byte of Kanji char */
                }
                break;
            case IDL_DT_EOL:
                break;
            default:
#ifdef DEBUG_INTERP
                printf("rpc_ss_mia_ber_marsh_struct:unrecognized type %d\n",
                        type_byte);
                exit(0);
#endif
                RAISE(rpc_x_coding_error);
        }
    } while (type_byte != IDL_DT_EOL);

    /* Output the "end of sequence" for the structure */
    ddis_type = DDIS$K_T_EOC;
#ifdef VMS
    if (ddis_g_entries[IDL_DDIS_PUT] == 0)
    {
   	stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
   	if ((stat & 1) != 1) 
	   RAISE(rpc_x_coding_error);
    }
    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
				&IDL_msp->IDL_ddis_stream_handle,
                                &ddis_type, 0, 0, 0 );
#else
    ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#endif
    if ( ddis_status != DDIS$_NORMAL )
        rpc_ss_raise_ddis_error( ddis_status );
}

/******************************************************************************/
/*                                                                            */
/*  Marshall 1-dimensional array                                              */
/*                                                                            */
/******************************************************************************/
static void rpc_ss_mia_ber_m_arr_dim_1
#ifdef IDL_PROTOTYPES
(
    /* [in] */  idl_byte base_type,
    /* [in] */  long lower_bound,
    /* [in] */  long upper_bound,
    /* [in] */  idl_byte *defn_vec_ptr,
    /* [in,out] */ rpc_void_p_t *p_current_element_addr,
    IDL_msp_t IDL_msp
)
#else
(base_type, lower_bound, upper_bound, defn_vec_ptr, p_current_element_addr,
 IDL_msp)
    idl_byte base_type;
    long lower_bound;
    long upper_bound;
    idl_byte *defn_vec_ptr;
    rpc_void_p_t *p_current_element_addr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int stat;
    idl_ulong_int struct_defn_index;
    idl_ulong_int struct_offset_index;
    idl_ulong_int struct_size;
    idl_byte *struct_defn_ptr;
    idl_ulong_int array_defn_index;
    idl_byte *array_defn_ptr;
    idl_ulong_int base_array_dim;   /* Dimensionality of array base type
                                        if this is an array */
    long i;
    idl_ulong_int ddis_type;
    idl_ulong_int ddis_length;
    idl_ulong_int ddis_status;
    rpc_void_p_t p_transmitted_data;    /* Address of data to be transmitted */
    idl_byte *stripped_string_base;
    idl_ushort_int *stripped_kanji_base;

    if (base_type == IDL_DT_MODIFIED)
    {
        /* The base type is followed by DT_CHAR, DT_BYTE or DT_STRUCT
            and then a modifier */
        base_type = *(defn_vec_ptr+2);
        ddis_length = upper_bound - lower_bound + 1;
        switch(base_type)
        {
            case IDL_DT_STDL_OCTET:
                ddis_type = K_T_octet_array;
                break;
            case IDL_DT_STDL_TS_SLATIN:
            case IDL_DT_STDL_TS_LATIN1:
                if (IDL_MIA_SPACE_STRIPPING)
                {
                    /* Calculate length without trailing spaces */
                    stripped_string_base = (idl_byte *)(*p_current_element_addr);
                    while ( (*(stripped_string_base + ddis_length - 1) == ' ')
                            && (ddis_length > 0) )
                    {
                        ddis_length--;
                    }
                }
                ddis_type = K_T_latin;
                break;
            case IDL_DT_STDL_TS_KANA:
                if (IDL_MIA_SPACE_STRIPPING)
                {
                    /* Calculate length without trailing spaces */
                    stripped_string_base = (idl_byte *)(*p_current_element_addr);
                    while ( (*(stripped_string_base + ddis_length - 1) == ' ')
                            && (ddis_length > 0) )
                    {
                        ddis_length--;
                    }
                }
                ddis_type = K_T_katakana;
                break;
            case IDL_DT_STDL_TS_KANJI:
                if (IDL_MIA_SPACE_STRIPPING)
                {
                    /* Calculate length without trailing spaces */
                    stripped_kanji_base = (idl_ushort_int *)(*p_current_element_addr);
                    while ( (*(stripped_kanji_base + ddis_length - 1) == 0xA1A1)
                            && (ddis_length > 0) )
                    {
                        ddis_length--;
                    }
                }
                ddis_type = K_T_kanji;
                p_transmitted_data = (rpc_void_p_t)rpc_ss_mem_alloc
                                (&IDL_msp->IDL_mem_handle, ddis_length * 2);
                rpc_ss_kanji_dec_to_iso( *p_current_element_addr,
                                            p_transmitted_data, ddis_length );
                ddis_length *= 2;
                break;
            case IDL_DT_STDL_TS_DECIMAL:
                ddis_type = K_T_decimal_string;
                break;
            case IDL_DT_STDL_TS_SLATIN_ELT:
            case IDL_DT_STDL_TS_LATIN1_ELT:
            case IDL_DT_STDL_TS_KANA_ELT:
            case IDL_DT_STDL_TS_KANJI_ELT:
            case IDL_DT_STDL_TS_DECIMAL_ELT:
                goto marshall_array_by_element;
            default:
#ifdef DEBUG_INTERP
                printf("rpc_ss_mia_ber_m_fixed_arr_dim: unrecognized octet type %d\n",
                            base_type);
                exit(0);
#endif
                RAISE(rpc_x_coding_error);
        }
        if (base_type != IDL_DT_STDL_TS_KANJI)
            p_transmitted_data = *p_current_element_addr;
#ifdef VMS
        if (ddis_g_entries[IDL_DDIS_PUT] == 0)
        {
   	   stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
   	   if ((stat & 1) != 1) 
	      RAISE(rpc_x_coding_error);
        }
        ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, &ddis_length, p_transmitted_data, 0);
#else
        ddis_status = ddis$put(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, &ddis_length, p_transmitted_data, 0);
#endif
        if ( ddis_status != DDIS$_NORMAL )
            rpc_ss_raise_ddis_error( ddis_status );
        if (base_type == IDL_DT_STDL_TS_KANJI)
            rpc_ss_mem_item_free(&IDL_msp->IDL_mem_handle,
                                                (byte_p_t) p_transmitted_data);
        if (  ( base_type == IDL_DT_STDL_TS_SLATIN
		|| base_type == IDL_DT_STDL_TS_LATIN1
                || base_type == IDL_DT_STDL_TS_KANA
                || base_type == IDL_DT_STDL_TS_KANJI )
            && IDL_MIA_SPACE_STRIPPING )
        {
            /* Length may have been changed to stripped length.  Reset it to
                 field length, in case we have an array of stripped strings */
            ddis_length = upper_bound - lower_bound + 1;
            if (base_type == IDL_DT_STDL_TS_KANJI)
                ddis_length *= 2;
        }
        *p_current_element_addr = (rpc_void_p_t)
                          ((idl_byte *)(*p_current_element_addr) + ddis_length);
        return;
    }
    else if (base_type == IDL_DT_BYTE)
    {
        /* Unmodified byte array, e.g. node field of UUID */
        ddis_length = upper_bound - lower_bound + 1;
        ddis_type = K_T_octet_array;
#ifdef VMS
        if (ddis_g_entries[IDL_DDIS_PUT] == 0)
        {
   	   stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
   	   if ((stat & 1) != 1) 
	      RAISE(rpc_x_coding_error);
        }
        ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, &ddis_length, *p_current_element_addr, 0);
#else
        ddis_status = ddis$put(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, &ddis_length, *p_current_element_addr, 0);
#endif
        if ( ddis_status != DDIS$_NORMAL )
            rpc_ss_raise_ddis_error( ddis_status );
        *p_current_element_addr = (rpc_void_p_t)
                          ((idl_byte *)(*p_current_element_addr) + ddis_length);
        return;
    }

marshall_array_by_element:
    /* Output the "start of sequence" for the array */
    ddis_type = K_T_stdl_array_type;
#ifdef VMS
    if (ddis_g_entries[IDL_DDIS_PUT] == 0)
    {
       stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
       if ((stat & 1) != 1) 
          RAISE(rpc_x_coding_error);
    }
    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, 0, 0, 0);
#else
    ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#endif
    if ( ddis_status != DDIS$_NORMAL )
        rpc_ss_raise_ddis_error( ddis_status );
    if ( base_type == IDL_DT_FIXED_STRUCT )
    {
        defn_vec_ptr++;
        IDL_GET_LONG_FROM_VECTOR( struct_defn_index, defn_vec_ptr );
        struct_defn_ptr =  IDL_msp->IDL_type_vec + struct_defn_index;
        IDL_GET_LONG_FROM_VECTOR(struct_offset_index, struct_defn_ptr);
        struct_size = *(IDL_msp->IDL_offset_vec + struct_offset_index);
    }
    else if ( base_type == IDL_DT_FIXED_ARRAY )
    {
        defn_vec_ptr++;
        IDL_GET_LONG_FROM_VECTOR( array_defn_index, defn_vec_ptr );
        array_defn_ptr = IDL_msp->IDL_type_vec + array_defn_index;
        base_array_dim = (idl_ulong_int)*(array_defn_ptr);
        array_defn_ptr++;
    }
    for (i=lower_bound; i<=upper_bound; i++)
    {
        switch (base_type)
        {
            case IDL_DT_LONG:
                rpc_ss_mia_ber_marsh_scalar( base_type,
                                    *p_current_element_addr, IDL_msp);
                *p_current_element_addr = (rpc_void_p_t)
                           ((idl_long_int *)(*p_current_element_addr) + 1);
                break;
            case IDL_DT_ULONG:
                rpc_ss_mia_ber_marsh_scalar( base_type,
                                    *p_current_element_addr, IDL_msp);
                *p_current_element_addr = (rpc_void_p_t)
                           ((idl_ulong_int *)(*p_current_element_addr) + 1);
                break;
            case IDL_DT_USHORT:
                rpc_ss_mia_ber_marsh_scalar( base_type,
                                    *p_current_element_addr, IDL_msp);
                *p_current_element_addr = (rpc_void_p_t)
                           ((idl_ushort_int *)(*p_current_element_addr) + 1);
                break;
            case IDL_DT_USMALL:
                rpc_ss_mia_ber_marsh_scalar( base_type,
                                    *p_current_element_addr, IDL_msp);
                *p_current_element_addr = (rpc_void_p_t)
                           ((idl_usmall_int *)(*p_current_element_addr) + 1);
                break;
            case IDL_DT_FIXED_STRUCT:
                rpc_ss_mia_ber_marsh_struct( struct_defn_index,
                                   *p_current_element_addr, idl_false, IDL_msp);
                *p_current_element_addr = (rpc_void_p_t)
                          ((idl_byte *)(*p_current_element_addr) + struct_size);
                break;
            case IDL_DT_FIXED_ARRAY:
                rpc_ss_mia_ber_m_fixed_arr_dim(base_array_dim, array_defn_ptr,
                                               p_current_element_addr, IDL_msp);
                break;
            case IDL_DT_STDL_TS_SLATIN_ELT:
            case IDL_DT_STDL_TS_LATIN1_ELT:
            case IDL_DT_STDL_TS_KANA_ELT:
            case IDL_DT_STDL_TS_DECIMAL_ELT:
                rpc_ss_mia_ber_marsh_scalar( base_type,
                                        *p_current_element_addr, IDL_msp);
                *p_current_element_addr = (rpc_void_p_t)
                           ((idl_byte *)(*p_current_element_addr) + 1);
                break;
            case IDL_DT_STDL_TS_KANJI_ELT:
                rpc_ss_mia_ber_marsh_scalar( base_type,
                                        *p_current_element_addr, IDL_msp);
                *p_current_element_addr = (rpc_void_p_t)
                           ((idl_byte *)(*p_current_element_addr) + 2);
                break;
            default:
#ifdef DEBUG_INTERP
                printf("rpc_ss_mia_ber_m_fixed_arr_dim: unrecognized type %d\n",
                            base_type);
                exit(0);
#endif
                RAISE(rpc_x_coding_error);
        }
    }
    /* Output the "end of sequence" for the array */
    ddis_type = DDIS$K_T_EOC;
#ifdef VMS
    if (ddis_g_entries[IDL_DDIS_PUT] == 0)
    {
       stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
       if ((stat & 1) != 1) 
          RAISE(rpc_x_coding_error);
    }
    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, 0, 0, 0);
#else
    ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                        &ddis_type, 0, 0, 0 );
#endif
    if ( ddis_status != DDIS$_NORMAL )
        rpc_ss_raise_ddis_error( ddis_status );
}

/******************************************************************************/
/*                                                                            */
/* Marshall a dimension of a fixed array                                      */
/*                                                                            */
/******************************************************************************/
static void rpc_ss_mia_ber_m_fixed_arr_dim
#ifdef IDL_PROTOTYPES
(
    /* [in] */  idl_ulong_int dimensionality,
    /* [in] */  idl_byte *defn_vec_ptr,
    /* [in,out] */ rpc_void_p_t *p_current_element_addr,
        /* Pointer to address of next array element to be marshalled.
           Note that the cell being updated starts out as being a (parameter)
           stack copy of the array address */
    IDL_msp_t IDL_msp
)
#else
( dimensionality, defn_vec_ptr, p_current_element_addr, IDL_msp )
    idl_ulong_int dimensionality;
    idl_byte *defn_vec_ptr;
    rpc_void_p_t *p_current_element_addr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int stat;
    idl_ulong_int ddis_status;
    long lower_bound;
    long upper_bound;
    long i;
    idl_ulong_int ddis_type;
    idl_ulong_int ddis_length;
    idl_byte base_type;

    IDL_GET_LONG_FROM_VECTOR(lower_bound,defn_vec_ptr);
    IDL_GET_LONG_FROM_VECTOR(upper_bound,defn_vec_ptr);
    if (dimensionality > 1)
    {
        /* Output the "start of sequence" for the array */
        ddis_type = K_T_stdl_array_type;
#ifdef VMS
        if (ddis_g_entries[IDL_DDIS_PUT] == 0)
        {
           stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
           if ((stat & 1) != 1) 
              RAISE(rpc_x_coding_error);
        }
        ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, 0, 0, 0);
#else
        ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#endif
        if ( ddis_status != DDIS$_NORMAL )
            rpc_ss_raise_ddis_error( ddis_status );
        for (i=lower_bound; i<=upper_bound; i++)
        {
            rpc_ss_mia_ber_m_fixed_arr_dim(dimensionality-1, defn_vec_ptr, 
                                        p_current_element_addr, IDL_msp);
        }
        /* Output the "end of sequence" for the array */
        ddis_type = DDIS$K_T_EOC;
#ifdef VMS
        if (ddis_g_entries[IDL_DDIS_PUT] == 0)
        {
           stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
           if ((stat & 1) != 1) 
              RAISE(rpc_x_coding_error);
        }
        ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, 0, 0, 0);
#else
        ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#endif
        if ( ddis_status != DDIS$_NORMAL )
            rpc_ss_raise_ddis_error( ddis_status );
    }
    else
    {
        base_type = *defn_vec_ptr;
        rpc_ss_mia_ber_m_arr_dim_1(base_type, lower_bound, upper_bound, 
                        defn_vec_ptr, p_current_element_addr, IDL_msp);

    }
}

/******************************************************************************/
/*                                                                            */
/*  Marshall a fixed array                                                    */
/*                                                                            */
/******************************************************************************/
static void rpc_ss_mia_ber_marsh_fixed_arr
#ifdef IDL_PROTOTYPES
(
    /* [in] */  idl_ulong_int defn_index,
    /* [in] */  rpc_void_p_t array_addr,
    IDL_msp_t IDL_msp
)
#else
(defn_index, array_addr, IDL_msp)
    idl_ulong_int defn_index;
    rpc_void_p_t array_addr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int dimensionality;
    idl_byte *defn_vec_ptr;

    defn_vec_ptr = IDL_msp->IDL_type_vec + defn_index;
    dimensionality = (idl_ulong_int)*defn_vec_ptr;
    defn_vec_ptr++;
    rpc_ss_mia_ber_m_fixed_arr_dim(dimensionality, defn_vec_ptr, &array_addr, 
                                                                       IDL_msp);
}

/******************************************************************************/
/*                                                                            */
/* Marshall a dimension of a varying array                                    */
/*                                                                            */
/******************************************************************************/
/*
 */
static void rpc_ss_mia_ber_m_varying_arr_dim
#ifdef IDL_PROTOTYPES
(
    /* [in] */  idl_ulong_int dimensionality,
    /* [in] */  long lower_data_limit,
    /* [in] */  long upper_data_limit,
    /* [in] */  idl_byte *defn_vec_ptr,     /* If dimensionality > 1 points at
                               next data limit pair, else points to base type */
    /* [in,out] */ rpc_void_p_t *p_current_element_addr,
        /* Pointer to address of next array element to be marshalled.
           Note that the cell being updated starts out as being a (parameter)
           stack copy of the array address */
    IDL_msp_t IDL_msp
)
#else
(dimensionality, lower_data_limit, upper_data_limit, defn_vec_ptr,
   p_current_element_addr, IDL_msp )
    idl_ulong_int dimensionality;
    long lower_data_limit;
    long upper_data_limit;
    idl_byte *defn_vec_ptr;
    rpc_void_p_t *p_current_element_addr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int stat;
    long i;
    idl_ulong_int ddis_type;
    idl_ulong_int ddis_length;
    idl_ulong_int ddis_status;
    long next_dim_lower_data_limit;
    long next_dim_upper_data_limit;
    idl_byte base_type;

    if (dimensionality > 1)
    {
        /* Output the "start of sequence" for the array */
        ddis_type = K_T_stdl_array_type;
#ifdef VMS
        if (ddis_g_entries[IDL_DDIS_PUT] == 0)
        {
           stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
           if ((stat & 1) != 1) 
              RAISE(rpc_x_coding_error);
        }
        ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, 0, 0, 0);
#else
        ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#endif
        if ( ddis_status != DDIS$_NORMAL )
            rpc_ss_raise_ddis_error( ddis_status );
        /* Get the data limits for the next dimension */
        defn_vec_ptr += 4;  /* lower data limit known to be fixed */
        IDL_GET_LONG_FROM_VECTOR(next_dim_lower_data_limit, defn_vec_ptr);
        defn_vec_ptr += 4;  /* upper data limit known to be fixed */
        IDL_GET_LONG_FROM_VECTOR(next_dim_upper_data_limit, defn_vec_ptr);
        for (i=lower_data_limit; i<=upper_data_limit; i++)
        {
            rpc_ss_mia_ber_m_varying_arr_dim(dimensionality-1, 
                next_dim_lower_data_limit, next_dim_upper_data_limit,
                defn_vec_ptr, p_current_element_addr, IDL_msp);
        }
        /* Output the "end of sequence" for the array */
        ddis_type = DDIS$K_T_EOC;
#ifdef VMS
        if (ddis_g_entries[IDL_DDIS_PUT] == 0)
        {
           stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
           if ((stat & 1) != 1) 
              RAISE(rpc_x_coding_error);
        }
        ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, 0, 0, 0);
#else
        ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#endif
        if ( ddis_status != DDIS$_NORMAL )
            rpc_ss_raise_ddis_error( ddis_status );
    }
    else
    {
        base_type = *defn_vec_ptr;
        rpc_ss_mia_ber_m_arr_dim_1(base_type, lower_data_limit,
               upper_data_limit, defn_vec_ptr, p_current_element_addr, IDL_msp);
    }
}

/******************************************************************************/
/*                                                                            */
/*  Marshall a varying array                                                  */
/*                                                                            */
/******************************************************************************/
static void rpc_ss_mia_ber_m_varying_arr
#ifdef IDL_PROTOTYPES
(
    /* [in] */  idl_ulong_int defn_index,
                                       /* Posn of full array desc in defn vec */
    /* [in] */  rpc_void_p_t array_addr,
    /* [in] */  rpc_void_p_t struct_addr,  /* Base address of structure of which
                            array is a field. For MIA it can't be a parameter */
    /* [in] */  idl_ulong_int *struct_offset_vec_ptr,   /* Pointer to base of
                                                     offset vec for structure */
    IDL_msp_t IDL_msp
)
#else
(defn_index, array_addr, struct_addr, struct_offset_vec_ptr, IDL_msp)
    idl_ulong_int defn_index;
    rpc_void_p_t array_addr;
    rpc_void_p_t struct_addr;
    idl_ulong_int *struct_offset_vec_ptr;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int dimensionality;
    idl_byte *defn_vec_ptr;
    long first_dim_lower_data_limit;
    long first_dim_upper_data_limit;
    idl_ulong_int data_limit_index;
    idl_ulong_int data_limit_offset;
    long *data_limit_addr;
    IDL_bound_pair_t *array_bounds;

    defn_vec_ptr = IDL_msp->IDL_type_vec + defn_index;
    dimensionality = (idl_ulong_int)*defn_vec_ptr;
    defn_vec_ptr++;
    /* For MIA we assume only varying data limit is upper on first dimension */
    first_dim_lower_data_limit = 0;
    /* Set up a pointer to the array bounds */
    IDL_ALIGN_POINTER_TO_LONG(defn_vec_ptr);
#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)
    rpc_ss_fixed_bounds_from_vector(dimensionality, defn_vec_ptr,
                                                        &array_bounds,IDL_msp);
#else
    array_bounds = (IDL_bound_pair_t *)defn_vec_ptr;
#endif
    /* Skip over the bounds and the lower data limit */
    /* And we know the upper data limit type/kind is long/length_is */
    defn_vec_ptr += dimensionality * IDL_FIXED_BOUND_PAIR_WIDTH + 12;
    IDL_GET_LONG_FROM_VECTOR(data_limit_index, defn_vec_ptr);
    data_limit_offset = *(struct_offset_vec_ptr + data_limit_index);
    first_dim_upper_data_limit = *(idl_long_int *)
                              ((idl_byte *)struct_addr + data_limit_offset) - 1;
    if (first_dim_upper_data_limit > array_bounds[0].upper)
        RAISE( rpc_x_invalid_bound );
#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)
    rpc_ss_mem_item_free(&IDL_msp->IDL_mem_handle, (byte_p_t)array_bounds);
#endif

    rpc_ss_mia_ber_m_varying_arr_dim(dimensionality, first_dim_lower_data_limit,
                first_dim_upper_data_limit, defn_vec_ptr, &array_addr, IDL_msp);
}

/******************************************************************************/
/*                                                                            */
/*  Marshall an encapsulated union according to the "union_hack"              */
/*                                                                            */
/******************************************************************************/
static void rpc_ss_mia_ber_m_fake_union
#ifdef IDL_PROTOTYPES
(
    /* [in] */ rpc_void_p_t param_addr,
    /* [in] */ idl_ulong_int defn_index,    /* Index to switch type */
    IDL_msp_t IDL_msp
)
#else
( param_addr, defn_index, IDL_msp )
    rpc_void_p_t param_addr;
    idl_ulong_int defn_index;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int stat;
    idl_ulong_int ddis_type;
    idl_ulong_int ddis_length;
    idl_ulong_int ddis_status;
    idl_byte *defn_vec_ptr;
    idl_ulong_int offset_index;
    idl_ulong_int *offset_vec_ptr;
    rpc_void_p_t body_addr;    /* Address of the union body */
    rpc_void_p_t aligned_addr; /* Address of the aligned UUID */
    idl_byte switch_type;
    idl_ulong_int arm_count;
    idl_ulong_int switch_value;
    idl_byte arm_type;
    idl_ulong_int arm_defn_index;

    /* Output the "start of sequence" for the containing structure */
    ddis_type = K_T_stdl_record_type;
#ifdef VMS
    if (ddis_g_entries[IDL_DDIS_PUT] == 0)
    {
       stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
       if ((stat & 1) != 1) 
          RAISE(rpc_x_coding_error);
    }
    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, 0, 0, 0);
#else
    ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#endif
    if ( ddis_status != DDIS$_NORMAL )
        rpc_ss_raise_ddis_error( ddis_status );

    /* The union is discriminated by the first byte of a 16-byte big-endian
        UUID. Marshall this UUID as an octet string */
    ddis_type = K_T_octet_array;
    ddis_length = 16;

    /* Currently what is done, is to use the union discrimintor as the
     * first byte of the UUID, and then the next field is a 15byte octet
     * array containing the remaining 15bytes of the UUID.
     * So what we do is to dump into ddis the 16bytes following from the 
     * address of the request_context.  On any machine that is byte aligned
     * (vax/vms) this approach is flawless.
     */

    defn_vec_ptr = IDL_msp->IDL_type_vec + defn_index;
    IDL_GET_LONG_FROM_VECTOR(offset_index, defn_vec_ptr);
    switch_type = *defn_vec_ptr;
    defn_vec_ptr++;
    switch_value = rpc_ss_get_typed_integer(switch_type, param_addr, IDL_msp);

    offset_vec_ptr = IDL_msp->IDL_offset_vec + offset_index + 1;
                                            /* + 1 to skip over union size */
    body_addr = (rpc_void_p_t)((idl_byte *)param_addr + *offset_vec_ptr);

    /* Discr = 53 UUID = E67AC09D3A11CA80AB08002B14B188 
     * So due to padding we have 5300000000000000E67AC09D3A11CA80AB08002B14B188
     * Sending just the first 16bytes will not only truncate our data, but 
     * transmit the padding over the wire.
     * 
     * Since we can't break compatiblity with non-dec platforms, we can't
     * change the ddis sending of 16bytes, so we have to adjust the
     * alignment ourselves.
     *
     * One easy way, would be to move the discr over to the end of the
     * padding and then send the following 16bytes from there...
     */
    aligned_addr = (rpc_void_p_t)((idl_byte*)body_addr-1);
    memmove(aligned_addr, param_addr, 1);


#ifdef VMS
    if (ddis_g_entries[IDL_DDIS_PUT] == 0)
    {
       stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
       if ((stat & 1) != 1) 
          RAISE(rpc_x_coding_error);
    }
    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
                          &IDL_msp->IDL_ddis_stream_handle,
                          &ddis_type, &ddis_length, aligned_addr, 0);
#else
    ddis_status = ddis$put(&IDL_msp->IDL_ddis_stream_handle,
                           &ddis_type, &ddis_length, aligned_addr, 0);
#endif
    if ( ddis_status != DDIS$_NORMAL )
        rpc_ss_raise_ddis_error( ddis_status );

    
    IDL_GET_LONG_FROM_VECTOR(arm_count, defn_vec_ptr);
    if ( ! rpc_ss_find_union_arm_defn(defn_vec_ptr, arm_count, switch_value,
                                        &defn_vec_ptr, IDL_msp) )
    {
#ifdef DEBUG_INTERP
        printf("rpc_ss_mia_ber_m_fake_union:unmatched switch_value %08x\n",
                    switch_value);
        exit(0);
#endif
        RAISE(rpc_x_coding_error);
    }
    arm_type = *defn_vec_ptr;
    defn_vec_ptr++;
    if (arm_type == IDL_DT_FIXED_STRUCT)
    {
        defn_vec_ptr++;     /* Skip over properties byte */
        IDL_GET_LONG_FROM_VECTOR(arm_defn_index, defn_vec_ptr);
        rpc_ss_mia_ber_marsh_struct(arm_defn_index, body_addr, idl_true, 
                                                                     IDL_msp);
    }
    else
    {
#ifdef DEBUG_INTERP
        printf("rpc_ss_mia_ber_m_fake_union:unrecognized type %d\n",
                    arm_type);
        exit(0);
#endif
        RAISE(rpc_x_coding_error);
    }
}

/******************************************************************************/
/*                                                                            */
/*  Marshall an encapsulated union - assuming it is a transaction context     */
/*      block                                                                 */
/*                                                                            */
/******************************************************************************/
static void rpc_ss_mia_ber_m_tx_ctx
#ifdef IDL_PROTOTYPES
(
    /* [in] */ rpc_void_p_t param_addr,
    /* [in] */ idl_ulong_int defn_index,    /* Index to switch type */
    IDL_msp_t IDL_msp
)
#else
( param_addr, defn_index, IDL_msp )
    rpc_void_p_t param_addr;
    idl_ulong_int defn_index;
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int stat;
    idl_ulong_int ddis_type;
    idl_ulong_int ddis_length;
    idl_ulong_int ddis_status;
    idl_byte *defn_vec_ptr;
    idl_ulong_int offset_index;
    idl_ulong_int *offset_vec_ptr;
    rpc_void_p_t body_addr;    /* Address of the union body */
    idl_byte switch_type;
    idl_ulong_int arm_count;
    idl_long_int switch_value;
    idl_byte arm_type;
    idl_ulong_int arm_defn_index;

    /* Output the "start of sequence" for a transaction context block */
    ddis_type = K_T_tx_rpc_context;
#ifdef VMS
    if (ddis_g_entries[IDL_DDIS_PUT] == 0)
    {
   	stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
   	if ((stat & 1) != 1) 
	   RAISE(rpc_x_coding_error);
    }
#endif

#ifdef VMS
    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
    				    &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0);
#else
    ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#endif
    if ( ddis_status != DDIS$_NORMAL )
        rpc_ss_raise_ddis_error( ddis_status );

    /* Marshall the switch value */
    defn_vec_ptr = IDL_msp->IDL_type_vec + defn_index;
    IDL_GET_LONG_FROM_VECTOR(offset_index, defn_vec_ptr);
    switch_type = *defn_vec_ptr;
    defn_vec_ptr++;
    switch_value = rpc_ss_get_typed_integer(switch_type, param_addr, IDL_msp);
    ddis_type = K_T_tx_ctx;
    ddis_length = sizeof(idl_long_int);
#ifdef VMS
    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
    				    &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, &ddis_length, &switch_value, 0 );
#else
    ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                   &ddis_type, &ddis_length, &switch_value, 0 );
#endif
    if (ddis_status != DDIS$_NORMAL)
        rpc_ss_raise_ddis_error( ddis_status );

    offset_vec_ptr = IDL_msp->IDL_offset_vec + offset_index + 1;
                                            /* + 1 to skip over union size */
    body_addr = (rpc_void_p_t)((idl_byte *)param_addr + *offset_vec_ptr);
    
    IDL_GET_LONG_FROM_VECTOR(arm_count, defn_vec_ptr);
    if ( ! rpc_ss_find_union_arm_defn(defn_vec_ptr, arm_count, switch_value,
                                        &defn_vec_ptr, IDL_msp) )
    {
#ifdef DEBUG_INTERP
        printf("rpc_ss_mia_ber_m_tx_ctx:unmatched switch_value %08x\n",
                    switch_value);
        exit(0);
#endif
        RAISE(rpc_x_coding_error);
    }
    arm_type = *defn_vec_ptr;
    defn_vec_ptr++;
    if (arm_type == IDL_DT_FIXED_STRUCT)
    {
        defn_vec_ptr++;     /* Skip over properties byte */

        /* Start of sequence for tx_xoctx */
        ddis_type = K_T_xopen_ctx;
#ifdef VMS
    	ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
    				    &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#else
        ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                                        &ddis_type, 0, 0, 0 );
#endif
        if ( ddis_status != DDIS$_NORMAL )
            rpc_ss_raise_ddis_error( ddis_status );

        IDL_GET_LONG_FROM_VECTOR(arm_defn_index, defn_vec_ptr);
        defn_vec_ptr = IDL_msp->IDL_type_vec + arm_defn_index;
        IDL_DISCARD_LONG_FROM_VECTOR(defn_vec_ptr); /* Offset vector index */
        if ( *defn_vec_ptr != IDL_DT_USMALL )
        {
#ifdef DEBUG_INTERP
            printf("rpc_ss_mia_ber_m_tx_ctx:wrong first struct field %d\n",
                    *defn_vec_ptr);
            exit(0);
#endif
            RAISE(rpc_x_coding_error);
        }
        defn_vec_ptr++;

        /* Marshall rollback_indicator */
        ddis_type = K_T_rollback_indicator;
        ddis_length = sizeof(idl_usmall_int);
#ifdef VMS
    	ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
    				    &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, &ddis_length, body_addr, 0 );
#else
        ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                   &ddis_type, &ddis_length, body_addr, 0 );
#endif
        if (ddis_status != DDIS$_NORMAL)
            rpc_ss_raise_ddis_error( ddis_status );

        if ( *defn_vec_ptr != IDL_DT_EOL )
        {
#ifdef DEBUG_INTERP
            printf("rpc_ss_mia_ber_m_tx_ctx:more than one struct field %d\n",
                    *defn_vec_ptr);
            exit(0);
#endif
            RAISE(rpc_x_coding_error);
        }

        /* Output the "end of sequence" for tx_xoctx */
        ddis_type = DDIS$K_T_EOC;
#ifdef VMS
    	ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
    				    &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#else
        ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                                        &ddis_type, 0, 0, 0 );
#endif
        if ( ddis_status != DDIS$_NORMAL )
            rpc_ss_raise_ddis_error( ddis_status );
    }
    else
    {
#ifdef DEBUG_INTERP
        printf("rpc_ss_mia_ber_m_tx_ctx:unrecognized arm type %d\n",
                    arm_type);
        exit(0);
#endif
        RAISE(rpc_x_coding_error);
    }

    /* Output the "end of sequence" for the transaction context block */
    ddis_type = DDIS$K_T_EOC;
#ifdef VMS
    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))(
    				    &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#else
    ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
                                    &ddis_type, 0, 0, 0 );
#endif
    if ( ddis_status != DDIS$_NORMAL )
        rpc_ss_raise_ddis_error( ddis_status );
}

/******************************************************************************/
/*                                                                            */
/*  Control for marshalling                                                   */
/*                                                                            */
/*  Uses data structure and routines from ddissupp.c as arguments to DDIS     */
/*                                                                            */
/******************************************************************************/
void rpc_ss_mia_ber_marsh_interp
#ifdef IDL_PROTOTYPES
(
    idl_ulong_int IDL_parameter_count, /* [in] -- Number of parameters to   */
                                  /* marshall in this call to the           */
                                  /* interpreter                            */

    idl_ulong_int IDL_type_index,    /* [in] -- Offset into the type vector */
                                  /* for the description of the type to be  */
                                  /* marshalled                             */

    rpc_void_p_t IDL_param_vector[], /* [in,out] -- The addresses of each of */
                                  /* the the parameters thus it's size is   */
                                  /* the number of parameters in the        */
                                  /* signature of the operation             */

    IDL_msp_t IDL_msp          /* [in,out] -- Pointer to marshalling state   */
)
#else
( IDL_parameter_count, IDL_type_index, IDL_param_vector, IDL_msp)
    idl_ulong_int IDL_parameter_count;
    idl_ulong_int IDL_type_index;
    rpc_void_p_t IDL_param_vector[];
    IDL_msp_t IDL_msp;
#endif
{
    idl_ulong_int stat;
    idl_ulong_int ddis_status;
    idl_ulong_int ddis_type;
    idl_ulong_int ddis_length;
    long ddis_buf_len;
    idl_byte *ddis_buf_addr;
    idl_byte *type_vec_ptr;
    idl_byte type_byte;
    idl_ulong_int param_index;
    rpc_void_p_t param_addr;
    idl_ulong_int defn_index;
    idl_short_int interp_major_version, interp_minor_version;

    /*
     * The first buffer is a dummy one just large enough for the
     * "start of sequence", which is not part of the wire data.
     */
    ddis_buf_len = sizeof(IDL_ddis_dummy_buffer);
    ddis_buf_addr = IDL_ddis_dummy_buffer;
    IDL_msp->IDL_ddis_state = IDL_ddis_first_marshall;

#ifdef VMS
    if (ddis_g_entries[IDL_DDIS_CREATE_STREAM] == 0)
    {
   	stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_create_stream_desc,
				     &ddis_g_entries[IDL_DDIS_CREATE_STREAM],
				     0);
   	if ((stat & 1) != 1) 
	   RAISE(rpc_x_coding_error);
    }
#endif

    /*
     * After encoding into the dummy buffer rpc_ss_ddis_data_put will be called
     * by DDIS to get the first "real" buffer in which to place encoded data.
     */
#ifdef VMS
    ddis_status = (*(ddis_g_entries[IDL_DDIS_CREATE_STREAM]))( rpc_ss_ddis_mem_alloc,
#else
    ddis_status = ddis$create_stream( rpc_ss_ddis_mem_alloc,
#endif
                        rpc_ss_ddis_mem_free,
                        0,
                        rpc_ss_ddis_data_put,
                        IDL_msp,
                        &ddis_buf_len,
                        ddis_buf_addr,
                        IDL_ddis_syntax_table_list,
                        &IDL_msp->IDL_ddis_stream_handle );
    if ( ddis_status != DDIS$_NORMAL )
        rpc_ss_raise_ddis_error( ddis_status );

    /* Output the "start of sequence" for the parameter list */
    ddis_type = K_T_Stdl_Params;
#ifdef VMS
    if (ddis_g_entries[IDL_DDIS_PUT] == 0)
    {
   	stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_put_desc,
				     &ddis_g_entries[IDL_DDIS_PUT],
				     0);
   	if ((stat & 1) != 1) 
	   RAISE(rpc_x_coding_error);
    }
    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))( &IDL_msp->IDL_ddis_stream_handle,
#else
    ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
#endif
                                    &ddis_type, 0, 0, 0 );
    if ( ddis_status != DDIS$_NORMAL )
        rpc_ss_raise_ddis_error( ddis_status );

    /* Loop over parameters */
    type_vec_ptr = (IDL_msp->IDL_type_vec) + IDL_type_index;
    for ( ; IDL_parameter_count > 0; IDL_parameter_count-- )
    {
        IDL_GET_LONG_FROM_VECTOR(param_index,type_vec_ptr);
        param_addr = IDL_param_vector[param_index];
        do {
            type_byte = *type_vec_ptr;
            type_vec_ptr++;
            switch(type_byte)
            {
                case IDL_DT_LONG:
                case IDL_DT_ULONG:
                case IDL_DT_USHORT:
                case IDL_DT_USMALL:
                    rpc_ss_mia_ber_marsh_scalar(type_byte, param_addr, IDL_msp);
                    break;
                case IDL_DT_FIXED_STRUCT:
                    type_vec_ptr++;     /* Discard properties byte */
                    IDL_GET_LONG_FROM_VECTOR(defn_index,type_vec_ptr);
                    rpc_ss_mia_ber_marsh_struct(defn_index, param_addr,
                                                            idl_false, IDL_msp);
                    break;
                case IDL_DT_FIXED_ARRAY:
                    type_vec_ptr++;     /* Discard properties byte */
                    IDL_GET_LONG_FROM_VECTOR(defn_index,type_vec_ptr);
                    rpc_ss_mia_ber_marsh_fixed_arr(defn_index, param_addr,
                                                                     IDL_msp);
                    IDL_DISCARD_LONG_FROM_VECTOR(type_vec_ptr);
                                            /* Discard flattened definition */
                    break;
                case IDL_DT_ENC_UNION:
                    type_vec_ptr++;     /* Discard properties byte */
                    IDL_GET_LONG_FROM_VECTOR(defn_index,type_vec_ptr);
                    rpc_ss_mia_ber_m_tx_ctx(param_addr, defn_index,
                                                                     IDL_msp);
                    break;
                case IDL_DT_PASSED_BY_REF:
                    break;
                case IDL_DT_MODIFIED:
                    if (*type_vec_ptr == IDL_DT_ENC_UNION)
                    {
                        /* ACMS request context block */
                        type_vec_ptr += 2;  /* DT_ENC_UNION and properties */
                        IDL_GET_LONG_FROM_VECTOR(defn_index,type_vec_ptr);
                        rpc_ss_mia_ber_m_fake_union(param_addr, defn_index,
                                                                     IDL_msp);
                        break;
                    }
                    type_vec_ptr++;  /* Discard DT_BYTE, DT_CHAR or DT_STRUCT */
                    type_byte = *type_vec_ptr;  /* Get the modifier */
                    type_vec_ptr++;
                    rpc_ss_mia_ber_marsh_scalar(type_byte, param_addr, IDL_msp);
                    if (type_byte == IDL_DT_STDL_TS_KANJI)
                    {
                        /* Skip over the structure machinery */
                        type_vec_ptr++;     /* Discard properties byte */
                        IDL_DISCARD_LONG_FROM_VECTOR(type_vec_ptr);
                    }
                    break;
                case IDL_DT_EOL:
                    break;
                default:
#ifdef DEBUG_INTERP
                    printf("rpc_ss_mia_ber_marsh_interp:unrecognized type %d\n",
                        type_byte);
                    exit(0);
#endif
                    RAISE(rpc_x_coding_error);
            }
        } while (type_byte != IDL_DT_EOL);
    }

    /* Output the "end of sequence" for the parameter list */
#if 0
    /*
     * We don't want the "end of sequence" to be part of the wire data.
     * By not outputing it, DDIS views the stream as incomplete, but
     * it is correct for our purposes.
     */
    ddis_type = DDIS$K_T_EOC;
#ifdef VMS
    ddis_status = (*(ddis_g_entries[IDL_DDIS_PUT]))( &IDL_msp->IDL_ddis_stream_handle,
#else
    ddis_status = ddis$put( &IDL_msp->IDL_ddis_stream_handle,
#endif
                                    &ddis_type, 0, 0, 0 );
    if ( ddis_status != DDIS$_TOPCTX )
        rpc_ss_raise_ddis_error( ddis_status );
#endif

    /* Close the DDIS data stream */
    IDL_msp->IDL_ddis_state = IDL_ddis_marshall_done;
#ifdef VMS
    if (ddis_g_entries[IDL_DDIS_CLOSE_STREAM] == 0)
    {
   	stat = lib$find_image_symbol(&ddis_shareable_desc, 
   				     &ddis_close_stream_desc,
				     &ddis_g_entries[IDL_DDIS_CLOSE_STREAM],
				     0);
   	if ((stat & 1) != 1) 
	   RAISE(rpc_x_coding_error);
    }
    ddis_status = (*(ddis_g_entries[IDL_DDIS_CLOSE_STREAM]))(
    				    &IDL_msp->IDL_ddis_stream_handle);
#else
    ddis_status = ddis$close_stream(  &IDL_msp->IDL_ddis_stream_handle );
#endif
    if ( IDL_DDIS_FAILURE(ddis_status) )
        rpc_ss_raise_ddis_error( ddis_status );
    if (IDL_msp->IDL_pickling_handle == NULL
        && IDL_msp->IDL_buff_addr != NULL)
#ifdef DEBUG_ALLOC
    {
        fprintf(stderr, "rpc_ss_mia_ber_marsh_interp: free %lx\n",
                IDL_msp->IDL_buff_addr);
        fflush(stderr);
#endif
	free( IDL_msp->IDL_buff_addr );  /* The last "put" allocated a new buffer */
#ifdef DEBUG_ALLOC
    }
#endif
    IDL_msp->IDL_ddis_state = IDL_ddis_not_in_use;

}

