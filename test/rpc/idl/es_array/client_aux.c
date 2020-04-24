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
 * Revision 1.1.4.2  1996/02/17  23:13:54  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:33  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:44  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:23:37  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:13:02  ganni]
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <fcntl.h>
#include <es_array.h>
#include <test_common.h>

/* Remove these definitions if any non-pickling operations in the interface */
#include <dce/stubbase.h>
es_array_v0_0_epv_t es_array_v0_0_m_epv;
static rpc_if_rep_t test_ifspec;
rpc_if_handle_t es_array_v0_0_s_ifspec = (rpc_if_handle_t)&test_ifspec;

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

static idl_long_int arr[N];
static idl_long_int out_arr[N];
static s_t sarr[N];
static s_t ref_sarr[N];
static s_t out_sarr[N];
static t_t tarr[N];
static t_t ref_tarr[N];
static t_t out_tarr[N];
static idl_long_int (*oarr)[M];

#define FIXED_BUFF_STORE (8*N+64)
static idl_byte fixed_buff_area[FIXED_BUFF_STORE];

int client_aux(h)
    handle_t h;
{
    idl_es_handle_t es_h;
    idl_byte *fixed_buff_start;
    idl_ulong_int fixed_buff_size, encoding_size;
    idl_byte *dyn_buff_start;
    error_status_t status;
    int i,j,failures = 0;
    idl_byte *use_for_misaligned, *misaligned_buff;


    for (i = 0; i < N; i++)
    {
        arr[i] = random()%10000;
        sarr[i].b = i & 0x7f;
        sarr[i].l = random()%10000;
        ref_sarr[i] = sarr[i];
        tarr[i].b = i & 0x7f;
        for (j = 0; j < 7; j++) tarr[i].a[j] = random()%10000;
        ref_tarr[i] = tarr[i];
    }

    es_state.wrong_behavior = none;
    /* Incremental encode/decode */
    /* Encode data using one operation */
    es_state.file_handle = open("es_array_1.dat", O_CREAT|O_TRUNC|O_WRONLY,
                                                                         0777);
    if (es_state.file_handle < 0)
    {
        printf("Can't open es_array_1.dat\n");
        failures++;
        return failures;
    }
    idl_es_encode_incremental((idl_void_p_t)&es_state, es_allocate, es_write,
                                &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_encode_incremental\n", status);
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
    in_array_op1(es_h, arr);
    close(es_state.file_handle);
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }

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
    out_array_op1(es_h, out_arr);
    close(es_state.file_handle);
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }

    /* Check the input and output are the same */
    for (i = 0; i < N; i++)
    {
        if (out_arr[i] != arr[i])
        {
            printf("out_arr[%d] - found %d - expecting %d\n",
                    i, out_arr[i], arr[i]);
            failures++;
        }
    }

    /* Fixed buffer encode/decode */
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
    array_op2(es_h, sarr);
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }
    idl_es_decode_buffer(fixed_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    array_op2(es_h, out_sarr);
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }
    for (i = 0; i < N; i++)
    {
        if (out_sarr[i].b != ref_sarr[i].b)
        {
            printf("array_op2 - out_sarr[%d].b = %c\n", i, out_sarr[i].b);
            failures++;
        }
        out_sarr[i].b = 0;
        if (out_sarr[i].l != ref_sarr[i].l)
        {
            printf("array_op2 - out_sarr[%d].l = %d\n", i, out_sarr[i].l);
            failures++;
        }
        out_sarr[i].l = 0;
    }
    /* Check the decode works if the buffer is deliberately misaligned */
    use_for_misaligned = (idl_byte *)malloc(encoding_size + 1);
    misaligned_buff = use_for_misaligned + 1;
    memcpy(misaligned_buff, fixed_buff_start, encoding_size);
    idl_es_decode_buffer(misaligned_buff, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    array_op2(es_h, out_sarr);
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }
    for (i = 0; i < N; i++)
    {
        if (out_sarr[i].b != ref_sarr[i].b)
        {
            printf(
"array_op2(misaligned) - out_sarr[%d].b = %c\n", i, out_sarr[i].b);
            failures++;
        }
        if (out_sarr[i].l != ref_sarr[i].l)
        {
            printf(
"array_op2(misaligned) - out_sarr[%d].l = %d\n", i, out_sarr[i].l);
            failures++;
        }
    }
    free(use_for_misaligned);

    /* But the decode should fail if the buffer is incomplete */
    idl_es_decode_buffer(fixed_buff_start, encoding_size-8, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    TRY
    {
        array_op2(es_h, out_sarr);
    }
    CATCH (rpc_x_ss_bad_buffer)
        printf(
"Buffer decoding, incomplete buffer - caught expected exception\n");
    CATCH_ALL
        printf(
"****Buffer decoding, incomplete buffer  - wrong exception ****\n");
        failures++;
    ENDTRY
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }

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
    array_op3(es_h, tarr);
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }
    idl_es_decode_buffer(dyn_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    array_op3(es_h, out_tarr);
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }
    for (i = 0; i < N; i++)
    {
        if (out_tarr[i].b != ref_tarr[i].b)
        {
            printf("array_op3 - out_tarr[%d].b = %c\n", i, out_tarr[i].b);
            failures++;
        }
        for (j=0; j<7; j++)
        {
            if (out_tarr[i].a[j] != ref_tarr[i].a[j])
            {
                printf("array_op3 - out_tarr[%d].a[%d] = %d\n",
                        i, j, out_tarr[i].a[j]);
                failures++;
            }
        }
    }

    /* Check for incremental encoding with incorrectly aligned buffer */
    es_state.wrong_behavior = bad_alignment;
    idl_es_encode_incremental((idl_void_p_t)&es_state, es_allocate, es_write,
                                &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_encode_incremental\n", status);
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
    TRY
    {
        in_array_op1(es_h, arr);
    }
    CATCH (rpc_x_ss_bad_buffer)
        printf(
"Incremental encoding, wrong buffer alignment - caught expected exception\n");
    CATCH_ALL
        printf(
"****Incremental encoding, wrong buffer alignment - wrong exception ****\n");
        failures++;
    ENDTRY
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }

    /* Check for incremental encoding with incorrectly sized buffer */
    es_state.wrong_behavior = bad_size;
    idl_es_encode_incremental((idl_void_p_t)&es_state, es_allocate, es_write,
                                &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_encode_incremental\n", status);
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
    TRY
    {
        in_array_op1(es_h, arr);
    }
    CATCH (rpc_x_ss_bad_buffer)
        printf(
"Incremental encoding, wrong buffer size - caught expected exception\n");
    CATCH_ALL
        printf(
"****Incremental encoding, wrong buffer size - wrong exception ****\n");
        failures++;
    ENDTRY
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }

    /* Fixed buffer encode - bad buffer alignment */
    fixed_buff_start = (idl_byte *)(((fixed_buff_area - (idl_byte *)0) + 7)
                                                                    & (~7)) + 1;
    fixed_buff_size = (FIXED_BUFF_STORE - (fixed_buff_start - fixed_buff_area))
                                                                     & (~7);
    idl_es_encode_fixed_buffer(fixed_buff_start, fixed_buff_size,
                                &encoding_size, &es_h, &status);
    if (status != rpc_s_ss_bad_buffer)
    {
        printf("Wrong error - received %08x for bad fixed buffer alignment\n");
        failures++;
    }

    /* Fixed buffer encode - bad buffer size */
    fixed_buff_start = (idl_byte *)(((fixed_buff_area - (idl_byte *)0) + 7)
                                                                    & (~7));
    fixed_buff_size = ((FIXED_BUFF_STORE - (fixed_buff_start - fixed_buff_area))
                                                                    & (~7)) - 1;
    idl_es_encode_fixed_buffer(fixed_buff_start, fixed_buff_size,
                                &encoding_size, &es_h, &status);
    if (status != rpc_s_ss_bad_buffer)
    {
        printf("Wrong error - received %08x for bad fixed buffer alignment\n");
        failures++;
    }
    /* Fixed buffer encode - buffer too small */
    fixed_buff_start = (idl_byte *)(((fixed_buff_area - (idl_byte *)0) + 7)
                                                                     & (~7));
    fixed_buff_size = (FIXED_BUFF_STORE - (fixed_buff_start - fixed_buff_area))
                                                                   & (~7);
    fixed_buff_size -= 64;
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
    TRY
    {
        array_op2(es_h, sarr);
    }
    CATCH (rpc_x_no_memory)
        printf(
"Fixed buffer encoding, buffer too small - caught expected exception\n");
    CATCH_ALL
        printf(
"****Fixed buffer encoding, buffer too small - wrong exception ****\n");
        failures++;
    ENDTRY
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }

    /* Check for incremental decoding with incorrectly aligned buffer */
    es_state.wrong_behavior = bad_alignment;
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
    TRY
    {
        out_array_op1(es_h, out_arr);
    }
    CATCH (rpc_x_ss_bad_buffer)
        printf(
"Incremental decoding, wrong buffer alignment - caught expected exception\n");
    CATCH_ALL
        printf(
"****Incremental decoding, wrong buffer alignment - wrong exception ****\n");
        failures++;
    ENDTRY
    close(es_state.file_handle);
    idl_es_handle_free(&es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_handle_free\n", status);
        failures++;
        return failures;
    }

    printf("Test Complete\n");
    return failures;
}
