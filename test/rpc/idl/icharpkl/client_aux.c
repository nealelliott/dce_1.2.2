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
 * Revision 1.1.4.2  1996/02/17  23:14:42  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:59  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:08:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:32:30  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:27:21  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:15:14  ganni]
 * 
 * $EndLog$
 */
/*
*/

#include <stdio.h>
#include <icharpkl.h>
#include <test_common.h>

/* Remove these definitions if any non-pickling operations in the interface */
#include <dce/stubbase.h>
icharpkl_v0_0_epv_t icharpkl_v0_0_m_epv;
static rpc_if_rep_t test_ifspec;
rpc_if_handle_t icharpkl_v0_0_s_ifspec = (rpc_if_handle_t)&test_ifspec;

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int failures = 0;
    int i;
    idl_ulong_int encoding_size;
    idl_long_int sz;
    idl_short_int ln;
    idl_byte *dyn_buff_start;
    idl_es_handle_t es_h;
    error_status_t status;
    ltype hb0[ASIZE];
    ltype hb1[ASIZE];
    ltype hb2[VSIZE];
    open_t *p_o;
    conf_t *p_c;
    varying_t v;

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
    sz = ASIZE;
    for (i=0; i<sz/2; i++)
    {
        hb0[2*i] = i;
        hb0[2*i + 1] = 15 - i;
    }
    op0(es_h, hb0, &sz);
    idl_es_handle_free(&es_h, &status);
    idl_es_decode_buffer(dyn_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    op0(es_h, hb0, &sz);
    idl_es_handle_free(&es_h, &status);
    free(dyn_buff_start);
    if (sz != ASIZE)
    {
        printf("op0 - sz = %d\n", sz);
        failures++;
    }
    for (i=0; i<sz/2; i++)
    {
        if (hb0[2*i] != i)
        {
            printf("hb0[%d] = %d\n", 2*i, hb0[2*i]);
            failures++;
        }
        if (hb0[2*i + 1] != 15 - i)
        {
            printf("hb0[%d] = %d\n", 2*i+1, hb0[2*i+1]);
            failures++;
        }
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
    sz = ASIZE;
    ln = sz - 2;
    for (i=0; i<ln/2; i++)
    {
        hb1[2*i] = i;
        hb1[2*i + 1] = 15 - i;
    }
    op1(es_h, hb1, &sz, &ln);
    idl_es_handle_free(&es_h, &status);
    idl_es_decode_buffer(dyn_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    op1(es_h, hb1, &sz, &ln);
    idl_es_handle_free(&es_h, &status);
    free(dyn_buff_start);
    if (sz != ASIZE)
    {
        printf("op1 - sz = %d\n", sz);
        failures++;
    }
    if (ln != ASIZE - 2)
    {
        printf("op1 - ln = %d\n", ln);
        failures++;
    }
    for (i=0; i<ln/2; i++)
    {
        if (hb1[2*i] != i)
        {
            printf("hb1[%d] = %d\n", 2*i, hb1[2*i]);
            failures++;
        }
        if (hb1[2*i + 1] != 15 - i)
        {
            printf("hb1[%d] = %d\n", 2*i+1, hb1[2*i+1]);
            failures++;
        }
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
    sz = ASIZE;
    ln = sz - 2;
    for (i=0; i<ln/2; i++)
    {
        hb2[2*i] = i;
        hb2[2*i + 1] = 15 - i;
    }
    op2(es_h, hb2, &ln);
    idl_es_handle_free(&es_h, &status);
    idl_es_decode_buffer(dyn_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    op2(es_h, hb2, &ln);
    idl_es_handle_free(&es_h, &status);
    free(dyn_buff_start);
    if (ln != ASIZE - 2)
    {
        printf("op2 - ln = %d\n", ln);
        failures++;
    }
    for (i=0; i<ln/2; i++)
    {
        if (hb2[2*i] != i)
        {
            printf("hb2[%d] = %d\n", 2*i, hb2[2*i]);
            failures++;
        }
        if (hb2[2*i + 1] != 15 - i)
        {
            printf("hb2[%d] = %d\n", 2*i+1, hb2[2*i+1]);
            failures++;
        }
    }

    /* Dynamic buffer encode - fixed buffer decode */
    p_c = (conf_t *)malloc(sizeof(conf_t) + (ASIZE-1)*sizeof(ltype));
    p_c->sz = ASIZE;
    for (i=0; i < (p_c->sz)/2; i++)
    {
        p_c->hb[2*i] = i;
        p_c->hb[2*i + 1] = 15 - i;
    }
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
    op3(es_h, p_c);
    idl_es_handle_free(&es_h, &status);
    idl_es_decode_buffer(dyn_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    op3(es_h, p_c);
    idl_es_handle_free(&es_h, &status);
    free(dyn_buff_start);
    if (p_c->sz != ASIZE)
    {
        printf("op3 - p_c->sz = %d\n", p_c->sz);
        failures++;
    }
    for (i=0; i<p_c->sz/2; i++)
    {
        if (p_c->hb[2*i] != i)
        {
            printf("p_c->hb[%d] = %d\n", 2*i, p_c->hb[2*i]);
            failures++;
        }
        if (p_c->hb[2*i + 1] != 15 - i)
        {
            printf("p_c->hb[%d] = %d\n", 2*i+1, p_c->hb[2*i+1]);
            failures++;
        }
    }
    free(p_c);

    /* Dynamic buffer encode - fixed buffer decode */
    p_o = (open_t *)malloc(sizeof(conf_t) + (ASIZE-1)*sizeof(ltype));
    p_o->sz = ASIZE;
    p_o->ln = p_o->sz - 2;
    for (i=0; i<p_o->ln/2; i++)
    {
        p_o->hb[2*i] = i;
        p_o->hb[2*i + 1] = 15 - i;
    }
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
    op4(es_h, p_o);
    idl_es_handle_free(&es_h, &status);
    idl_es_decode_buffer(dyn_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    op4(es_h, p_o);
    idl_es_handle_free(&es_h, &status);
    free(dyn_buff_start);
    if (p_o->sz != ASIZE)
    {
        printf("op4 - p_o->sz = %d\n", p_o->sz);
        failures++;
    }
    if (p_o->ln != ASIZE - 2)
    {
        printf("op4 - p_o->ln = %d\n", p_o->ln);
        failures++;
    }
    for (i=0; i<p_o->ln/2; i++)
    {
        if (p_o->hb[2*i] != i)
        {
            printf("p_o->hb[%d] = %d\n", 2*i, p_o->hb[2*i]);
            failures++;
        }
        if (p_o->hb[2*i + 1] != 15 - i)
        {
            printf("p_o->hb[%d] = %d\n", 2*i+1, p_o->hb[2*i+1]);
            failures++;
        }
    }
    free(p_o);

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
    v.ln = ASIZE - 2;
    for (i=0; i<v.ln/2; i++)
    {
        v.hb[2*i] = i;
        v.hb[2*i + 1] = 15 - i;
    }
    op5(es_h, &v);
    idl_es_handle_free(&es_h, &status);
    idl_es_decode_buffer(dyn_buff_start, encoding_size, &es_h, &status);
    if (status != error_status_ok)
    {
        printf("Error %08x from idl_es_decode_buffer\n", status);
        failures++;
        return failures;
    }
    op6(es_h, &v);
    idl_es_handle_free(&es_h, &status);
    free(dyn_buff_start);
    if (v.ln != ASIZE - 2)
    {
        printf("op2 - v.ln = %d\n", v.ln);
        failures++;
    }
    for (i=0; i<v.ln/2; i++)
    {
        if (v.hb[2*i] != i)
        {
            printf("v.hb[%d] = %d\n", 2*i, v.hb[2*i]);
            failures++;
        }
        if (v.hb[2*i + 1] != 15 - i)
        {
            printf("v.hb[%d] = %d\n", 2*i+1, v.hb[2*i+1]);
            failures++;
        }
    }

    return failures;
}
