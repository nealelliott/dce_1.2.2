/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client.c,v $
 * Revision 1.1.4.2  1996/02/17  23:14:05  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:39  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:04:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:05  root]
 * 
 * Revision 1.1.2.1  1994/05/10  18:03:30  tom
 * 	Test for ecoding services get/set attribute.
 * 	[1994/05/10  16:02:48  tom]
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <fcntl.h>
#include <es_attr_encode.h>
#include <es_attr_decode.h>
#include <test_common.h>

extern void print_exception();


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
    unsigned32 flags;


    for (i = 0; i < N; i++)
    {
        arr[i] = random()%10000;
    }

    es_state.wrong_behavior = none;
    /* Incremental encode/decode */
    /* Encode data using one operation */
    es_state.file_handle = open("es_attr_1.dat", O_CREAT|O_TRUNC|O_WRONLY,
                                                                         0777);
    if (es_state.file_handle < 0)
    {
        printf("Can't open es_attr_1.dat\n");
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
    idl_es_set_attrs(es_h, IDL_ES_NO_ENCODING_CHECK, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_set_attrs\n", status);
        failures++;
        return failures;
    }
    idl_es_inq_attrs(es_h, &flags, &status);
    if (flags != IDL_ES_NO_ENCODING_CHECK)
    {
        printf("idl_es_inq_attrs: flags = 0x%x - expected 0x%x\n", 
		flags, IDL_ES_NO_ENCODING_CHECK);
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
    es_state.file_handle = open("es_attr_1.dat", O_RDONLY, 0);
    if (es_state.file_handle < 0)
    {
        printf("Can't open es_attr_1.dat for reading\n");
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
        out_array_op1(es_h, out_arr);
    CATCH(rpc_x_unknown_if)
        printf("Caught expected exception\n");
    CATCH_ALL
        print_exception(THIS_CATCH);
        failures++;
        return failures;
    ENDTRY

    /*
     * Now set the no check attribute and try again
     */
    idl_es_set_attrs(es_h, IDL_ES_NO_ENCODING_CHECK, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_set_attrs\n", status);
        failures++;
        return failures;
    }
    idl_es_inq_attrs(es_h, &flags, &status);
    if (flags != IDL_ES_NO_ENCODING_CHECK)
    {
        printf("idl_es_inq_attrs: flags = 0x%x - expected 0x%x\n", 
		flags, IDL_ES_NO_ENCODING_CHECK);
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

    printf("Test Complete\n");
    return failures;
}


main(ac, av)
int ac;
char *av[];
{
    error_status_t st;
    rpc_binding_handle_t h = NULL;
    ndr_char *string_binding;
    static ndr_char nil_string[] = "";
    int pass, passes=10, failures = 0;


    TRY
    if (ac != 2) 
    {
        fprintf(stderr, "usage: %s passes\n", av[0]);
        exit(2);
    }

    passes = atoi(av[1]);

    for (pass = 0; pass < passes; pass++)
    {
        printf("PASS %d\n", pass);
        failures += client_aux(h, passes);
    }

    printf("Summary: %d passes, %d failures\n", passes, failures);

    CATCH_ALL
      print_exception(THIS_CATCH);
    ENDTRY

    exit ( failures ? 1 : 0 );
}

