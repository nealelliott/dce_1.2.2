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
 * Revision 1.1.4.2  1996/02/17  23:13:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:36  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:04:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:57  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:24:57  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:13:41  ganni]
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <es_arrayc.h>
#include <cstrop.h>
#include <test_common.h>

/* Remove these definitions if any non-pickling operations in the interface */
#include <dce/stubbase.h>
es_arrayc_v0_0_epv_t es_arrayc_v0_0_m_epv;
static rpc_if_rep_t test_ifspec;
rpc_if_handle_t es_arrayc_v0_0_s_ifspec = (rpc_if_handle_t)&test_ifspec;


#define FIXED_BUFF_STORE 20000
static idl_byte fixed_buff_area[FIXED_BUFF_STORE];

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int                 i, failures = 0;
    array_v1_string_t   a[AO_ARRAY_SIZE];    /* Array of strings */
    array_v1_string_t   out_a[AO_ARRAY_SIZE];    /* Array of strings */
    idl_char *aptr;
    idl_long_int last, m, out_m;
    idl_byte *fixed_buff_start;
    idl_ulong_int fixed_buff_size;
    idl_ulong_int encoding_size, op_num;
    idl_byte *dyn_buff_start;
    idl_es_handle_t es_h;
    error_status_t status;
    idl_byte good_byte;
    rpc_if_id_t if_id;
    idl_ulong_int op;

    for (i = 0 ; i < AO_ARRAY_SIZE ; i++)
        setup_array(a[i]);
    m = AO_ARRAY_SIZE-1;
    fixed_buff_start = (idl_byte *)(((fixed_buff_area - (idl_byte *)0) + 7)
                                                                     & (~7));
    fixed_buff_size = (FIXED_BUFF_STORE - (fixed_buff_start - fixed_buff_area))
                                                                     & (~7);

    /* Error tests - damaged pickle headers */
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
    idl_es_inq_encoding_id(es_h, &if_id, &op, &status);
    if (status != rpc_s_ss_bad_es_action)
    {
        printf(
            "%lx is not rpc_s_ss_bad_es_action from idl_es_inq_encoding_id\n",
             status);
        failures++;
        return failures;
    }
    fn_op(&es_h, &m, a);
    idl_es_handle_free(&es_h, &status);

    /* Damage the pickle header version */
    good_byte = *fixed_buff_start;
    *fixed_buff_start = 0xaa;
    idl_es_decode_buffer(fixed_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    TRY
    {
        fn_op(&es_h, &out_m, out_a);
    }
    CATCH (rpc_x_ss_wrong_es_version)
        printf("Damaged pickle header - caught expected exception\n");
        *fixed_buff_start = good_byte;
    CATCH_ALL
        printf("****Damaged pickle header - wrong exception ****\n");
    ENDTRY
    idl_es_handle_free(&es_h, &status);

    /* Damage the pickle interface UUID */
    good_byte = *(fixed_buff_start + 34);
    *(fixed_buff_start + 34) = 0xaa;
    idl_es_decode_buffer(fixed_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    TRY
    {
        fn_op(&es_h, &out_m, out_a);
    }
    CATCH (rpc_x_unknown_if)
        printf("Damaged interface UUID - caught expected exception\n");
        *(fixed_buff_start + 34) = good_byte;
    CATCH_ALL
        printf("****Damaged interface UUID - wrong exception ****\n");
    ENDTRY
    idl_es_handle_free(&es_h, &status);

    /* Damage the pickle interface version */
    good_byte = *(fixed_buff_start + 41);
    *(fixed_buff_start + 41) = 0xaa;
    idl_es_decode_buffer(fixed_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    TRY
    {
        fn_op(&es_h, &out_m, out_a);
    }
    CATCH (rpc_x_unknown_if)
        printf("Damaged interface version - caught expected exception\n");
        *(fixed_buff_start + 41) = good_byte;
    CATCH_ALL
        printf("****Damaged interface version - wrong exception ****\n");
    ENDTRY
    idl_es_handle_free(&es_h, &status);

    /* Damage the transfer syntax UUID */
    good_byte = *(fixed_buff_start + 14);
    *(fixed_buff_start + 14) = 0xaa;
    idl_es_decode_buffer(fixed_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    TRY
    {
        fn_op(&es_h, &out_m, out_a);
    }
    CATCH (rpc_x_tsyntaxes_unsupported)
        printf("Damaged transfer syntax UUID - caught expected exception\n");
        *(fixed_buff_start + 14) = good_byte;
    CATCH_ALL
        printf("****Damaged transfer syntax UUID - wrong exception ****\n");
    ENDTRY
    idl_es_handle_free(&es_h, &status);

    /* Damage the transfer syntax version */
    good_byte = *(fixed_buff_start + 21);
    *(fixed_buff_start + 21) = 0xaa;
    idl_es_decode_buffer(fixed_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    TRY
    {
        fn_op(&es_h, &out_m, out_a);
    }
    CATCH (rpc_x_tsyntaxes_unsupported)
        printf("Damaged transfer syntax version - caught expected exception\n");
        *(fixed_buff_start + 21) = good_byte;
    CATCH_ALL
        printf("****Damaged transfer syntax version - wrong exception ****\n");
    ENDTRY
    idl_es_handle_free(&es_h, &status);

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
    in_fn_sub(&es_h);
    idl_es_handle_free(&es_h, &status);
    idl_es_decode_buffer(dyn_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    out_fn_sub(&es_h, &failures);
    idl_es_handle_free(&es_h, &status);
    free(dyn_buff_start);

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
    in_cfmt_sub(&es_h);
    idl_es_handle_free(&es_h, &status);
    idl_es_decode_buffer(dyn_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    out_cfmt_sub(&es_h, &failures);
    idl_es_handle_free(&es_h, &status);
    free(dyn_buff_start);

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
    in_open_sub(&es_h);
    idl_es_handle_free(&es_h, &status);
    idl_es_decode_buffer(dyn_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    out_open_sub(&es_h, &failures);
    idl_es_handle_free(&es_h, &status);
    free(dyn_buff_start);

    
    return failures;
}
