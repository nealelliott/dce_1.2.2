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
 * Revision 1.1.4.2  1996/02/17  23:13:57  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:34  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:04:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:49  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:24:20  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:13:20  ganni]
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <fcntl.h>
#include <es_arrayb.h>
#include <test_common.h>

/* Remove these definitions if any non-pickling operations in the interface */
#include <dce/stubbase.h>
es_arrayb_v0_0_epv_t es_arrayb_v0_0_m_epv;
static rpc_if_rep_t test_ifspec;
rpc_if_handle_t es_arrayb_v0_0_s_ifspec = (rpc_if_handle_t)&test_ifspec;

typedef enum {none, bad_alignment, bad_size} alloc_err_t;

typedef struct es_state_t {
    idl_byte *malloced_addr;
    int file_handle;
    alloc_err_t wrong_behavior;
} es_state_t;

static es_state_t es_state;

#define OUT_BUFF_SIZE 2048
static idl_byte out_buff[OUT_BUFF_SIZE];
static idl_byte *out_data_addr;
static idl_ulong_int out_data_size;

void es_allocate(state, buf, size)
idl_void_p_t state;
idl_byte **buf;
idl_ulong_int *size;
{
    idl_byte *malloced_addr;
    es_state_t *p_es_state = (es_state_t *)state;

    malloced_addr = (idl_byte *)malloc(*size);
    p_es_state->malloced_addr = malloced_addr;
    *buf = (idl_byte *)(((malloced_addr - (idl_byte *)0) + 7) & (~7));
    if (p_es_state->wrong_behavior == bad_alignment)
        (*buf)++;
    *size = (*size - (*buf - malloced_addr)) & (~7);
    if (p_es_state->wrong_behavior == bad_size)
        (*size)--;
}

void es_write(state, buf, size)
idl_void_p_t state;
idl_byte *buf;
idl_ulong_int size;
{
    es_state_t *p_es_state = (es_state_t *)state;

    write(p_es_state->file_handle, buf, size);
    free(p_es_state->malloced_addr);
}

void es_read(state, buf, size)
idl_void_p_t state;
idl_byte **buf;
idl_ulong_int *size;
{
    es_state_t *p_es_state = (es_state_t *)state;

    read(p_es_state->file_handle, out_data_addr, out_data_size);
    *buf = out_data_addr;
    if (p_es_state->wrong_behavior == bad_alignment)
        (*buf)++;
    *size = out_data_size;
    if (p_es_state->wrong_behavior == bad_size)
        (*size)--;
}

static l_t l_in;
static l_t l_out;

#define FIXED_BUFF_STORE 20000
static idl_byte fixed_buff_area[FIXED_BUFF_STORE];

int client_aux(h)
      handle_t h;
{
    int i, j, failures = 0;
    idl_es_handle_t es_h;
    error_status_t status;
    idl_long_int in_long, out_long;
    idl_long_int in_size, out_size;
    s_t *in_s_t_a, *out_s_t_a;
    idl_byte *fixed_buff_start;
    idl_ulong_int fixed_buff_size;
    idl_ulong_int encoding_size, op_num;
    idl_byte *dyn_buff_start;
    rpc_if_id_t if_id;
    idl_char *string_uuid;
    int fh;

    es_state.wrong_behavior = none;

    /* Call decoding operation with encoding handle */
    idl_es_encode_incremental((idl_void_p_t)&es_state, es_allocate, es_write,
                                &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_encode_incremental\n", status);
        exit(0);
    }
    TRY
    {
        out_op0(es_h, &l_in);
    }
    CATCH (rpc_x_ss_bad_es_action)
        printf(
"Calling decoding stub with encoding handle - caught expected exception\n");
    CATCH_ALL
        printf(
"****Calling decoding stub with encoding handle  - wrong exception ****\n");
    ENDTRY
    idl_es_handle_free(&es_h, &status);
    if (es_h != NULL)
    {
        printf("Handle not freed correctly\n");
        exit(0);
    }

    /* Call encoding operation with decoding handle */
    idl_es_decode_incremental((idl_void_p_t)&es_state, es_read,
                                &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_incremental\n", status);
        exit(0);
    }
    TRY
    {
        in_op0(es_h, &l_in, in_long);
    }
    CATCH (rpc_x_ss_bad_es_action)
        printf(
"Calling encoding stub with decoding handle - caught expected exception\n");
    CATCH_ALL
        printf(
"****Calling encoding stub with decoding handle  - wrong exception ****\n");
    ENDTRY
    idl_es_handle_free(&es_h, &status);

    l_in.length = random() % N;
    for (i = 0; i < l_in.length; i++)
    {
        l_in.arr[i] = random()%10000;
    }
    in_long = random() % 128;

    /* Incremental encode/decode */
    /* Encode data using one operation */
    idl_es_encode_incremental((idl_void_p_t)&es_state, es_allocate, es_write,
                                &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_encode_incremental\n", status);
        exit(0);
    }
    es_state.file_handle = open("es_array_1.dat", O_CREAT|O_TRUNC|O_WRONLY,
                                                                         0777);
    if (es_state.file_handle < 0)
    {
        printf("Can't open es_array_1.dat\n");
        failures++;
        return failures;
    }
    /* Now specify the transfer syntax */
    idl_es_set_transfer_syntax(es_h, idl_es_transfer_syntax_ndr, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_set_transfer_syntax\n", status);
        failures++;
        return failures;
    }
    in_op0(es_h, &l_in, in_long);
    close(es_state.file_handle);
    idl_es_handle_free(&es_h, &status);

    /* Decode the data using another operation with the same signature */
    out_data_addr = (idl_byte *)(((out_buff - (idl_byte *)0) + 7) & (~7));
    out_data_size = (OUT_BUFF_SIZE - (out_data_addr - out_buff)) & (~7);
    es_state.file_handle = open("es_array_1.dat", O_RDONLY, 0);
    if (es_state.file_handle < 0)
    {
        printf("Can't open es_array_1.dat for reading\n");
        failures++;
        return failures;
    }
    idl_es_decode_incremental((idl_void_p_t)&es_state, es_read,
                                &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_incremental\n", status);
        failures++;
        return failures;
    }
    out_long = out_op0(es_h, &l_out);
    close(es_state.file_handle);
    idl_es_handle_free(&es_h, &status);

    /* Check the input and output are the same */
    if (out_long != in_long)
    {
        printf("Function result (op0) incorrect\n");
        failures++;
    }
    if (l_out.length != l_in.length)
    {
        printf("value of length %d - expected %d\n", l_out.length, l_in.length);
        failures++;
        return failures;
    }
    for (i = 0; i < l_out.length; i++)
    {
        if (l_out.arr[i] != l_in.arr[i])
        {
            printf("l_out.arr[%d] - found %d - expecting %d\n",
                    i, l_out.arr[i], l_in.arr[i]);
            failures++;
        }
    }

    in_size = random() % 500;
    in_s_t_a = (s_t *)malloc(in_size * sizeof(s_t));
    out_s_t_a = (s_t *)malloc(in_size * sizeof(s_t));
    for (i=0; i<in_size; i++)
    {
        in_s_t_a[i].l = random() % 10000;
        in_s_t_a[i].b = i & 0x7f;
    }
    out_long = 0;
    /* Dynamic buffer encode - fixed buffer decode */
    idl_es_encode_dyn_buffer(&dyn_buff_start, &encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_encode_dyn_buffer\n", status);
        failures++;
        return failures;
    }
    idl_es_set_transfer_syntax(es_h, idl_es_transfer_syntax_ndr, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_set_transfer_syntax\n", status);
        failures++;
        return failures;
    }
    op1(es_h, &in_size, in_s_t_a);
    op2(es_h, &in_long);
    idl_es_handle_free(&es_h, &status);
    idl_es_decode_buffer(dyn_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    op1(es_h, &out_size, out_s_t_a);
    op2(es_h, &out_long);
    idl_es_handle_free(&es_h, &status);
    free(dyn_buff_start);
    if (out_size != in_size)
    {
        printf("op1 - returned size %d - expected %d\n", in_size, out_size);
        failures++;
        return failures;
    }    
    for (i = 0; i < in_size; i++)
    {
        if (out_s_t_a[i].l != in_s_t_a[i].l)
        {
            printf("out_s_t_a[%d].l = %d - expected %d\n",
                    i, out_s_t_a[i].l, in_s_t_a[i].l);
            failures++;
        }
        if (out_s_t_a[i].b != in_s_t_a[i].b)
        {
            printf("out_s_t_a[%d].b = %d - expected %d\n",
                    i, out_s_t_a[i].b, in_s_t_a[i].b);
            failures++;
        }
    }
    if (out_long != in_long)
    {
        printf("out_long = %d - expected %d\n", out_long, in_long);
        failures++;
    }
    free(in_s_t_a);
    free(out_s_t_a);

    /* Multiple encode/decode on one handle - incremental */
    in_size = random() % 500;
    in_s_t_a = (s_t *)malloc(in_size * sizeof(s_t));
    out_s_t_a = (s_t *)malloc(in_size * sizeof(s_t));
    for (i=0; i<in_size; i++)
    {
        in_s_t_a[i].l = random() % 10000;
        in_s_t_a[i].b = i & 0x7f;
    }
    out_long = 0;
    idl_es_encode_incremental((idl_void_p_t)&es_state, es_allocate, es_write,
                                &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_encode_incremental\n", status);
        exit(0);
    }
    es_state.file_handle = open("es_array_1.dat", O_CREAT|O_TRUNC|O_WRONLY,
                                                                         0777);
    if (es_state.file_handle < 0)
    {
        printf("Can't open es_array_1.dat\n");
        failures++;
        return failures;
    }
    /* Now specify the transfer syntax */
    idl_es_set_transfer_syntax(es_h, idl_es_transfer_syntax_ndr, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_set_transfer_syntax\n", status);
        failures++;
        return failures;
    }
    op1(es_h, &in_size, in_s_t_a);
    memset(&if_id.uuid, 0, sizeof(uuid_t));
    if_id.vers_major = 17;
    if_id.vers_minor = 33;
    idl_es_inq_encoding_id(es_h, &if_id, &op_num, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_inq_encoding_id\n", status);
        failures++;
        return failures;
    }
    uuid_to_string(&if_id.uuid, &string_uuid, &status);
    printf("op1 encoding - %s %d.%d operation %d\n",
             string_uuid, if_id.vers_major, if_id.vers_minor, op_num);
    rpc_string_free(&string_uuid, &status);
    op2(es_h, &in_long);
    memset(&if_id.uuid, 0, sizeof(uuid_t));
    if_id.vers_major = 17;
    if_id.vers_minor = 33;
    idl_es_inq_encoding_id(es_h, &if_id, &op_num, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_inq_encoding_id\n", status);
        failures++;
        return failures;
    }
    uuid_to_string(&if_id.uuid, &string_uuid, &status);
    printf("op2 encoding - %s %d.%d operation %d\n",
             string_uuid, if_id.vers_major, if_id.vers_minor, op_num);
    rpc_string_free(&string_uuid, &status);
    close(es_state.file_handle);
    idl_es_handle_free(&es_h, &status);
    out_data_addr = (idl_byte *)(((out_buff - (idl_byte *)0) + 7) & (~7));
    out_data_size = (OUT_BUFF_SIZE - (out_data_addr - out_buff)) & (~7);
    es_state.file_handle = open("es_array_1.dat", O_RDONLY, 0);
    if (es_state.file_handle < 0)
    {
        printf("Can't open es_array_1.dat for reading\n");
        failures++;
        return failures;
    }
    idl_es_decode_incremental((idl_void_p_t)&es_state, es_read,
                                &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_incremental\n", status);
        failures++;
        return failures;
    }
    memset(&if_id.uuid, 0, sizeof(uuid_t));
    if_id.vers_major = 17;
    if_id.vers_minor = 33;
    idl_es_inq_encoding_id(es_h, &if_id, &op_num, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_inq_encoding_id\n", status);
        failures++;
        return failures;
    }
    uuid_to_string(&if_id.uuid, &string_uuid, &status);
    printf("op1 decoding - %s %d.%d operation %d\n",
             string_uuid, if_id.vers_major, if_id.vers_minor, op_num);
    rpc_string_free(&string_uuid, &status);
    op1(es_h, &out_size, out_s_t_a);
    memset(&if_id.uuid, 0, sizeof(uuid_t));
    if_id.vers_major = 17;
    if_id.vers_minor = 33;
    idl_es_inq_encoding_id(es_h, &if_id, &op_num, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_inq_encoding_id\n", status);
        failures++;
        return failures;
    }
    uuid_to_string(&if_id.uuid, &string_uuid, &status);
    printf("op2 decoding - %s %d.%d operation %d\n",
             string_uuid, if_id.vers_major, if_id.vers_minor, op_num);
    rpc_string_free(&string_uuid, &status);
    op2(es_h, &out_long);
    close(es_state.file_handle);
    idl_es_handle_free(&es_h, &status);
    if (out_size != in_size)
    {
        printf("op1 - returned size %d - expected %d\n", in_size, out_size);
        failures++;
        return failures;
    }    
    for (i = 0; i < in_size; i++)
    {
        if (out_s_t_a[i].l != in_s_t_a[i].l)
        {
            printf("out_s_t_a[%d].l = %d - expected %d\n",
                    i, out_s_t_a[i].l, in_s_t_a[i].l);
            failures++;
        }
        if (out_s_t_a[i].b != in_s_t_a[i].b)
        {
            printf("out_s_t_a[%d].b = %d - expected %d\n",
                    i, out_s_t_a[i].b, in_s_t_a[i].b);
            failures++;
        }
    }
    if (out_long != in_long)
    {
        printf("out_long = %d - expected %d\n", out_long, in_long);
        failures++;
    }
    free(in_s_t_a);
    free(out_s_t_a);

    /* Fixed buffer encode/decode with two operations */
    in_size = random() % 500;
    in_s_t_a = (s_t *)malloc(in_size * sizeof(s_t));
    out_s_t_a = (s_t *)malloc(in_size * sizeof(s_t));
    for (i=0; i<in_size; i++)
    {
        in_s_t_a[i].l = random() % 10000;
        in_s_t_a[i].b = i & 0x7f;
    }
    out_long = 0;
    fixed_buff_start = (idl_byte *)(((fixed_buff_area - (idl_byte *)0) + 7)
                                                                     & (~7));
    fixed_buff_size = (FIXED_BUFF_STORE - (fixed_buff_start - fixed_buff_area))
                                                                     & (~7);
    idl_es_encode_fixed_buffer(fixed_buff_start, fixed_buff_size,
                                &encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_encode_fixed_buffer\n", status);
        failures++;
        return failures;
    }
    idl_es_set_transfer_syntax(es_h, idl_es_transfer_syntax_ndr, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_set_transfer_syntax\n", status);
        failures++;
        return failures;
    }
    op1(es_h, &in_size, in_s_t_a);
    op2(es_h, &in_long);
    idl_es_handle_free(&es_h, &status);
    idl_es_decode_buffer(fixed_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    op1(es_h, &out_size, out_s_t_a);
    op2(es_h, &out_long);
    idl_es_handle_free(&es_h, &status);
    if (out_size != in_size)
    {
        printf("op1 - returned size %d - expected %d\n", in_size, out_size);
        failures++;
        return failures;
    }    
    for (i = 0; i < in_size; i++)
    {
        if (out_s_t_a[i].l != in_s_t_a[i].l)
        {
            printf("out_s_t_a[%d].l = %d - expected %d\n",
                    i, out_s_t_a[i].l, in_s_t_a[i].l);
            failures++;
        }
        if (out_s_t_a[i].b != in_s_t_a[i].b)
        {
            printf("out_s_t_a[%d].b = %d - expected %d\n",
                    i, out_s_t_a[i].b, in_s_t_a[i].b);
            failures++;
        }
    }
    if (out_long != in_long)
    {
        printf("out_long = %d - expected %d\n", out_long, in_long);
        failures++;
    }
    free(in_s_t_a);
    free(out_s_t_a);


    return failures;
}
