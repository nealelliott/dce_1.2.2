/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utilichr.c,v $
 * Revision 1.1.4.2  1996/02/17  23:14:40  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:58  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:07:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:27  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:27:07  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:15:03  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <icharexp.h>
#include <test_common.h>

void ltype_net_size
#ifdef IDL_PROTOTYPES
(
  rpc_binding_handle_t h,
  idl_ulong_int tag,
  idl_ulong_int l_storage_len,
  idl_cs_convert_t *p_convert_type,
  idl_ulong_int *p_w_storage_len,
  error_status_t *p_st
)
#else
(h, tag, l_storage_len, p_convert_type, p_w_storage_len, p_st)
  rpc_binding_handle_t h;
  idl_ulong_int tag;
  idl_ulong_int l_storage_len;
  idl_cs_convert_t *p_convert_type;
  idl_ulong_int *p_w_storage_len;
  error_status_t *p_st;
#endif
{
    *p_st = error_status_ok;
    switch (tag)
    {
        case RTN_STAG:
        case RTN_RTAG:
            *p_convert_type = idl_cs_new_buffer_convert;
            if (p_w_storage_len != NULL)
                *p_w_storage_len = 2 * l_storage_len;
            return;
        default:
            *p_st = rpc_s_ss_incompatible_codesets;
            return;
    }
}


void ltype_to_netcs
#ifdef IDL_PROTOTYPES
(
  rpc_binding_handle_t h,
  idl_ulong_int tag,
  ltype *ldata,
  idl_ulong_int l_data_len,
  elvish *wdata,
  idl_ulong_int *p_w_data_len,
  error_status_t *p_st
)
#else
(h, tag, ldata, l_data_len, wdata, p_w_data_len, p_st)
  rpc_binding_handle_t h;
  idl_ulong_int tag;
  ltype *ldata;
  idl_ulong_int l_data_len;
  elvish *wdata;
  idl_ulong_int *p_w_data_len;
  error_status_t *p_st;
#endif
{
    int i;

    *p_st = error_status_ok;
    switch (tag)
    {
        case RTN_STAG:
            for (i=0; i<l_data_len; i++)
            {
                wdata[2 * i] = ldata[i] & 0xF0;
                wdata[2 * i + 1] = ldata[i] & 0xF;
            }
            break;
        case RTN_RTAG:
            for (i=0; i<l_data_len; i++)
            {
                wdata[2 * i] = ldata[i] & 0xF;
                wdata[2 * i + 1] = ldata[i] & 0xF0;
            }
            break;
        default:
            *p_st = rpc_s_ss_incompatible_codesets;
            return;
    }
    if (p_w_data_len != NULL)
        *p_w_data_len = l_data_len * 2;
}

void ltype_local_size
#ifdef IDL_PROTOTYPES
(
  rpc_binding_handle_t h,
  idl_ulong_int tag,
  idl_ulong_int w_storage_len,
  idl_cs_convert_t *p_convert_type,
  idl_ulong_int *p_l_storage_len,
  error_status_t *p_st
)
#else
(h, tag, w_storage_len, p_convert_type, p_l_storage_len, p_st)
  rpc_binding_handle_t h;
  idl_ulong_int tag;
  idl_ulong_int w_storage_len;
  idl_cs_convert_t *p_convert_type;
  idl_ulong_int *p_l_storage_len;
  error_status_t *p_st;
#endif
{
    *p_st = error_status_ok;
    switch (tag)
    {
        case RTN_STAG:
        case RTN_RTAG:
            *p_convert_type = idl_cs_new_buffer_convert;
            if (p_l_storage_len != NULL)
                *p_l_storage_len = w_storage_len / 2;
            return;
        default:
            *p_st = rpc_s_ss_incompatible_codesets;
            return;
    }
}

void ltype_from_netcs
#ifdef IDL_PROTOTYPES
(
  rpc_binding_handle_t h,
  idl_ulong_int tag,
  elvish *wdata,
  idl_ulong_int w_data_len,
  idl_ulong_int l_storage_len,
  ltype *ldata,
  idl_ulong_int *p_l_data_len,
  error_status_t *p_st
)
#else
(h, tag, wdata, w_data_len, l_storage_len, ldata, p_l_data_len, p_st)
  rpc_binding_handle_t h;
  idl_ulong_int tag;
  elvish *wdata;
  idl_ulong_int w_data_len;
  idl_ulong_int l_storage_len;
  ltype *ldata;
  idl_ulong_int *p_l_data_len;
  error_status_t *p_st;
#endif
{
    int i;

    if (w_data_len > 2 * l_storage_len)
    {
        *p_st = rpc_s_fault_invalid_bound;
        return;
    }
    else
        *p_st = error_status_ok;
    switch (tag)
    {
        case RTN_STAG:
        case RTN_RTAG:
            for (i=0; i<w_data_len/2; i++)
                ldata[i] = wdata[2 * i] | wdata[2 * i + 1];
            break;
        default:
            *p_st = rpc_s_ss_incompatible_codesets;
            return;
    }
    if (p_l_data_len != NULL)
        *p_l_data_len = w_data_len / 2;
}

void set_tags
#ifdef IDL_PROTOTYPES
(
  rpc_binding_handle_t h,
  idl_boolean server_side,
  idl_ulong_int *p_stag,
  idl_ulong_int *p_drtag,
  idl_ulong_int *p_rtag,
  error_status_t *p_st
)
#else
(h, server_side, p_stag, p_drtag, p_rtag, p_st)
  rpc_binding_handle_t h;
  idl_boolean server_side;
  idl_ulong_int *p_stag;
  idl_ulong_int *p_drtag;
  idl_ulong_int *p_rtag;
  error_status_t *p_st;
#endif
{
    if (server_side)
    {
        if (*p_drtag == 0)
            *p_rtag = RTN_RTAG;
        else
            *p_rtag = *p_drtag + 1;
    }
    else
    {
        *p_stag = RTN_STAG;
        *p_drtag = RTN_DRTAG;
    }
    *p_st = error_status_ok;
}

