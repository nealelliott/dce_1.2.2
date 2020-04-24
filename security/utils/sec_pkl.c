/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_pkl.c,v $
 * Revision 1.1.12.2  1996/10/03  15:15:43  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:43 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Change sec_passwd_privkey -> sec_passwd_pubkey
 * 	[1996/09/10  21:08 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/13  20:50 UTC  aha  /main/aha_pk6/1]
 *
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/02/18  00:23:05  marty  1.1.10.2]
 *
 * Revision 1.1.12.1  1996/08/09  12:13:11  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/13  20:50 UTC  aha  /main/aha_pk6/1]
 * 
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/02/18  00:23:05  marty  1.1.10.2]
 * 
 * Revision 1.1.10.2  1996/02/18  00:23:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:38  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  18:03:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:14  root]
 * 
 * Revision 1.1.8.1  1994/09/06  15:07:55  max
 * 	Fixes for CR 9939.  Bad assignement line and 64 bit issues.
 * 	[1994/09/06  15:06:54  max]
 * 
 * Revision 1.1.6.1  1993/10/05  22:25:52  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/05  20:25:51  mccann]
 * 
 * 	CR8651 64 bit porting changes
 * 
 * Revision 1.1.4.2  1992/12/29  16:41:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:32:26  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/10  19:45:24  dineen
 * 	OT 4080 fix: check type uuid in pickle before unpickling
 * 	[1992/07/08  20:10:31  dineen]
 * 
 * Revision 1.1  1992/01/19  14:41:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sec_pkl.c V=6 12/04/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca.
**
**   Copyright (c) Hewlett-Packard Company 1991
**   Unpublished work. All Rights Reserved.
**  
**
**  NAME:
**
**      sec_pkl.c
**
**  FACILITY:
**
**      DCE Security
**
**  ABSTRACT:
**
**      Pickling routines for three sec_ types.
**
**  
**
*/

#include "sec_pkl.h"

#include <dce/pkl.h>
#include <dce/idlbase.h>
#include <dce/rpc.h>
#include <dce/stubbase.h>
#include <stddef.h>
#include <sec_encode.h>
#include <dce/passwd_encode.h>

/*
 * Ideally, this constant would be expressed in pkl.h but, since
 * pkl.h is derived from pkl.idl, it can't be done.
 */
static rpc_syntax_id_t ndr_syntax_id = {
{0x8a885d04, 0x1ceb, 0x11c9, 0x9f, 0xe8, {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}},
2};

extern ndr_format_t ndr_g_local_drep;

#define pkl_align_psp(p, base, alignment)\
    p = base + (((p-base) + (alignment-1)) & ~(alignment-1))

static void anon_str_ptr_pickle(data, psp, bsp)
idl_char *data;
idl_byte **psp;
idl_byte *bsp;
{
    idl_ulong_int Z;

    if (data != NULL) {
        Z = rpc_ss_strsiz(data, 1);
        pkl_align_psp((*psp), bsp, 4);
        rpc_marshall_ulong_int((*psp), Z);
        rpc_advance_mp((*psp), 4);
        rpc_marshall_ulong_int((*psp), 0);
        rpc_advance_mp((*psp), 4);
        rpc_marshall_ulong_int((*psp), Z);
        rpc_advance_mp((*psp), 4);

        for (; Z; Z--)
        {
            rpc_marshall_char((*psp), (*data));
            rpc_advance_mp((*psp), 1);
            data++;
        }
    }
}

static void adjust_len_for_string(stream_len, str)
unsigned32 *stream_len;
idl_char *str;
{
    if (str != NULL)
    {
        pkl_align_psp((*stream_len), 0, 4);
        *stream_len += 12 + rpc_ss_strsiz(str, 1);
    }
}

static void anon_str_ptr_unpickle(psp, bsp, drep, allocator, data, st)
idl_byte **psp;
idl_byte *bsp;
ndr_format_t drep;
void *(*allocator)(unsigned32);
idl_char **data;
error_status_t *st;
{
    idl_ulong_int B;
    idl_ulong_int A;
    idl_ulong_int Z;
    idl_char *element;
    unsigned32 node_number;

    node_number = (unsigned32)*data;
    if (node_number == 0)
    {
        *data = NULL;  /* gratuitous hyper-correctness in case NULL != 0 */
        return;
    }
    pkl_align_psp((*psp), bsp, 4);
    rpc_convert_ulong_int(drep, ndr_g_local_drep, (*psp), Z);
    rpc_advance_mp((*psp), 4);
    *data = (idl_char (*))(*allocator)((Z==0) ? 1 : Z);
    if (*data == NULL)
    {
        *st = rpc_s_no_memory;
        return;
    }

    rpc_convert_ulong_int(drep, ndr_g_local_drep, (*psp), A);
    rpc_advance_mp((*psp), 4);
    rpc_convert_ulong_int(drep, ndr_g_local_drep, (*psp), B);
    rpc_advance_mp((*psp), 4);
    if (B > Z)
    {
        *st = rpc_s_fault_invalid_bound;
        return;
    }
    element = *data;
    while (B)
    {
        rpc_convert_char(drep, ndr_g_local_drep, (*psp), (*element));
        rpc_advance_mp((*psp), 1);
        element++;
        B--;
    }
}

static unsigned32 next_non_null_node_number;

static void init_ptr_pickling(void)
{
    next_non_null_node_number = 1;
}

static void ptr_pickle(data, psp, bsp)
void *data;
idl_char **psp;
idl_char *bsp;
{
    pkl_align_psp((*psp), bsp, 4);
    if (data == NULL)
        rpc_marshall_ulong_int((*psp), 0);
    else
    {
        rpc_marshall_ulong_int((*psp), next_non_null_node_number);
        next_non_null_node_number++;
    }
    rpc_advance_mp((*psp), 4);
}

static void sec_id_t_pickle(data, psp, bsp)
sec_id_t *data;
idl_char **psp;
idl_char *bsp;
{
    idl_char *element;
    idl_ulong_int index;

    pkl_align_psp((*psp), bsp, 4);
    rpc_marshall_ulong_int((*psp), data->uuid.time_low);
    rpc_advance_mp((*psp), 4);
    rpc_marshall_ushort_int((*psp), data->uuid.time_mid);
    rpc_advance_mp((*psp), 2);
    rpc_marshall_ushort_int((*psp), data->uuid.time_hi_and_version);
    rpc_advance_mp((*psp), 2);
    rpc_marshall_usmall_int((*psp), data->uuid.clock_seq_hi_and_reserved);
    rpc_advance_mp((*psp), 1);
    rpc_marshall_usmall_int((*psp), data->uuid.clock_seq_low);
    rpc_advance_mp((*psp), 1);
    element = &data->uuid.node[0];
    for (index = 6; index; index--)
    {
        rpc_marshall_byte((*psp), (*element));
        rpc_advance_mp((*psp), 1);
        element++;
    }
    ptr_pickle(data->name, psp, bsp);
}

static void sec_id_tP_pickle(data, psp, bsp)
sec_id_t *data;
idl_char **psp;
idl_char *bsp;
{
    anon_str_ptr_pickle(
        (idl_char *) data->name,
        psp, bsp);
}

static void ptr_unpickle(psp, bsp, drep, allocator, data, st)
idl_char **psp;
idl_char *bsp;
ndr_format_t drep;
void *(*allocator)(unsigned32);
void **data;
error_status_t *st;
{
    idl_ulong_int node_number; 

    pkl_align_psp((*psp), bsp, 4);
    rpc_convert_ulong_int(drep, ndr_g_local_drep, (*psp), node_number);
    *data = (void*)node_number;
    rpc_advance_mp((*psp), 4);
}

static void sec_id_t_unpickle(psp, bsp, drep, allocator, data, st)
idl_char **psp;
idl_char *bsp;
ndr_format_t drep;
void *(*allocator)(unsigned32);
sec_id_t *data;
error_status_t *st;
{
    idl_char *element;
    idl_ulong_int index;

    pkl_align_psp((*psp), bsp, 4);
    rpc_convert_ulong_int(drep, ndr_g_local_drep, (*psp), data->uuid.time_low);
    rpc_advance_mp((*psp), 4);
    rpc_convert_ushort_int(drep, ndr_g_local_drep, (*psp), data->uuid.time_mid);
    rpc_advance_mp((*psp), 2);
    rpc_convert_ushort_int(drep, ndr_g_local_drep, (*psp), data->uuid.time_hi_and_version);
    rpc_advance_mp((*psp), 2);
    rpc_convert_usmall_int(drep, ndr_g_local_drep, (*psp), data->uuid.clock_seq_hi_and_reserved);
    rpc_advance_mp((*psp), 1);
    rpc_convert_usmall_int(drep, ndr_g_local_drep, (*psp), data->uuid.clock_seq_low);
    rpc_advance_mp((*psp), 1);
    element = &data->uuid.node[0];
    for (index = 6; index; index--)
    {
        rpc_convert_byte(drep, ndr_g_local_drep, (*psp), (*element));
        rpc_advance_mp((*psp), 1);
        element++;
    }
    ptr_unpickle(psp, bsp, drep, allocator, &data->name, st);
}

static void sec_id_tP_unpickle(psp, bsp, drep, allocator, data, st)
idl_char **psp;
idl_char *bsp;
ndr_format_t drep;
void *(*allocator)(unsigned32);
sec_id_t *data;
error_status_t *st;
{
    anon_str_ptr_unpickle(
        psp, bsp, drep,
        allocator,
        &data->name, st);
}

/*===================================================================*/

void sec_chksum_t_pickle(data, syntax, allocator, alloc_mod, pickle, pickle_len, st)
sec_chksum_t *data;
rpc_syntax_id_t *syntax;
void *(*allocator)(unsigned32);
unsigned32 alloc_mod;
idl_pkl_t **pickle;
unsigned32 *pickle_len;
error_status_t *st;
{
    unsigned32 stream_len = 8; /* ndr format label and fill bytes */
    idl_byte *psp, *bsp; /* pickle data pointers */
    idl_byte *bp;
    unsigned32 i;

    *st = error_status_ok;

    stream_len +=   2 /* chksum_type (enum) */
                 + 2 /* alignment padding */
                 + 4 /* len (unsigned32) */
                 + 4;/* node number (unsigned32) */
    if (data->chksum != NULL)
    {
        stream_len += 4 /* Z value (unsigned32) */ + data->len;
    }

    *pickle_len = stream_len + idl_pkl_data_offset;
    *pickle_len = (*pickle_len + (alloc_mod-1)) & ~(alloc_mod-1);
    *pickle = (*allocator)(*pickle_len);    

    /* ought to check input syntax, but ndr is currently the only choice */

    idl_pkl_set_header (**pickle, 0, stream_len, &ndr_syntax_id, &sec_chksum_t_typeid);

    psp = bsp = **pickle + idl_pkl_data_offset;

    /*
     * This code represents an architectural decision:
     *
     *    In a byte stream in an (NDR-flavored) pickle the first
     *    four bytes are the format label and the next four bytes are
     *    0.  The pickled data starts at byte 8 of the data part.
     */
    *(ndr_format_t *)psp = ndr_g_local_drep;
    rpc_advance_mp(psp, 4);
    rpc_marshall_ulong_int(psp, 0);
    rpc_advance_mp(psp, 4);

    rpc_marshall_enum (psp, data->chksum_type);
    rpc_advance_mp(psp, 2);
    pkl_align_psp(psp, bsp, 4);
    rpc_marshall_ulong_int(psp, data->len);
    rpc_advance_mp(psp, 4);
    if (data->chksum == NULL)
    {
        /* node number for a NULL ptr */
        rpc_marshall_ulong_int(psp, 0);
        rpc_advance_mp(psp, 4);
    }
    else
    {
        /* assuming no aliasing, chose non-0 node number */
        rpc_marshall_ulong_int(psp, 1);
        rpc_advance_mp(psp, 4);
        /* Z */
        rpc_marshall_ulong_int(psp, data->len);
        rpc_advance_mp(psp, 4);
        /* data->chksum */
        bp = data->chksum;
        for (i = data->len; i; i--)
        {
            rpc_marshall_byte(psp, *bp);
            rpc_advance_mp(psp, 1);
            bp++;
        }
    }
}

void sec_chksum_t_unpickle(pickle, allocator, data, st)
idl_pkl_t *pickle;
void *(*allocator)(unsigned32);
sec_chksum_t *data;
error_status_t *st;
{
    idl_byte *psp, *bsp;
    ndr_format_t drep;
    idl_ulong_int node_number, Z;
    idl_byte *element;
    uuid_t typeid;

    idl_pkl_get_type_uuid (*pickle, &typeid);
    if (!uuid_equal(&sec_chksum_t_typeid, &typeid, st))
    {
        *st = rpc_s_wrong_pickle_type;
        return;
    }

    *st = error_status_ok;

    psp = bsp = *pickle + idl_pkl_data_offset;
    drep = *(ndr_format_t *)psp;
    rpc_advance_mp(psp, 8);

    rpc_convert_enum(drep, ndr_g_local_drep, psp, data->chksum_type);
    rpc_advance_mp(psp, 2);
    pkl_align_psp(psp, bsp, 4);
    rpc_convert_ulong_int(drep, ndr_g_local_drep, psp, data->len);
    rpc_advance_mp(psp, 4);
    rpc_convert_ulong_int(drep, ndr_g_local_drep, psp, node_number);
    rpc_advance_mp(psp, 4);

    if (node_number == 0)
    {
        data->chksum = NULL;
        return;
    }
    pkl_align_psp(psp, bsp, 4);
    rpc_convert_ulong_int(drep, ndr_g_local_drep, psp, Z);
    rpc_advance_mp(psp, 4);
    data->chksum = (idl_byte *)(*allocator)((Z==0) ? 1 : Z);
    if (data->chksum == NULL)
    {
        *st = rpc_s_no_memory;
        return;
    }
    element = data->chksum;
    while (Z)
    {
        rpc_convert_byte(drep, ndr_g_local_drep, psp, (*element));
        rpc_advance_mp(psp, 1);
        element++;
        Z--;
    }
}

void sec_passwd_rec_t_pickle(data, syntax, allocator, alloc_mod, pickle, pickle_len, st)
sec_passwd_rec_t *data;
rpc_syntax_id_t *syntax;
void *(*allocator)(unsigned32);
unsigned32 alloc_mod;
idl_pkl_t **pickle;
unsigned32 *pickle_len;
error_status_t *st;
{
    unsigned32 stream_len = 8; /* ndr format label and fill bytes */
    idl_byte *psp, *bsp; /* pickle data pointers */
    idl_ulong_int Z0, Z1, index;
    idl_byte *element;

    *st = error_status_ok;
    /* Use IDL encoding functions for new password types.
     * Can eventually migrate all types to new encoding functions
     */
    switch (data->key.key_type) {
    case sec_passwd_pubkey:
    case sec_passwd_genprivkey:
        sec_pwd_encode(malloc_shim, free, data, pickle_len, (idl_byte **)pickle, st);
        return;
        break;
    default:
        break;
    }

    stream_len +=   4 /* version_number (unsigned32) */
                 + 4 /* pepper node number (unsigned32) */
                 + 2;/* key_type (enum) */


    switch (data->key.key_type)
    {
        case sec_passwd_plain:
            stream_len +=   2  /* alignment gap following enum */
                          + 4; /* plain node number (unsigned32) */
            break;
        case sec_passwd_des:
            stream_len += 8; /*des_key */
            break;
        default: 
            *st = rpc_s_fault_invalid_tag;
            return;
    } 

    if (data->pepper != NULL)
        adjust_len_for_string (&stream_len, data->pepper);

    if ((data->key.key_type == sec_passwd_plain)
    &&  (data->key.tagged_union.plain != NULL))
        adjust_len_for_string (&stream_len, data->key.tagged_union.plain);

    *pickle_len = stream_len + idl_pkl_data_offset;
    *pickle_len = (*pickle_len + (alloc_mod-1)) & ~(alloc_mod-1);
    *pickle = (*allocator)(*pickle_len);

    /* ought to check input syntax, but ndr is currently the only choice */

    idl_pkl_set_header (**pickle, 0, stream_len, &ndr_syntax_id, &sec_passwd_rec_t_typeid);

    psp = bsp = **pickle + idl_pkl_data_offset;

    init_ptr_pickling();

    /*
     * This code represents an architectural decision:
     *
     *    In a byte stream in an (NDR-flavored) pickle the first
     *    four bytes are the format label and the next four bytes are
     *    0.  The pickled data starts at byte 8.
     */
    *(ndr_format_t *)psp = ndr_g_local_drep;
    rpc_advance_mp(psp, 4);
    rpc_marshall_ulong_int(psp, 0);
    rpc_advance_mp(psp, 4);

    rpc_marshall_ulong_int(psp, data->version_number);
    rpc_advance_mp(psp, 4);
    ptr_pickle (data->pepper, &psp, bsp);
    rpc_marshall_enum(psp, data->key.key_type);
    rpc_advance_mp(psp, 2);
    switch (data->key.key_type)
    {
        case sec_passwd_plain:
            ptr_pickle(data->key.tagged_union.plain, &psp, bsp);
            break;
        case sec_passwd_des:
            element = data->key.tagged_union.des_key;
            for (index = 8; index; index--)
            {
                rpc_marshall_byte(psp, (*element));
                rpc_advance_mp(psp, 1);
                element++;
            }
            break;
        default:
            *st = rpc_s_fault_invalid_tag;
            return;
    }

    anon_str_ptr_pickle(data->pepper, &psp, bsp);

    if (data->key.key_type == sec_passwd_plain)
        anon_str_ptr_pickle(data->key.tagged_union.plain, &psp, bsp);
}

void sec_passwd_rec_t_decode(num_bytes, bytes_p, alloc, dealloc, pwd_p, st_p)
unsigned32        num_bytes;
idl_byte          *bytes_p;
idl_void_p_t      (*alloc)(idl_size_t size);
void              (*dealloc)(idl_void_p_t ptr);
sec_passwd_rec_t  *pwd_p;
error_status_t    *st_p;
{
    sec_encode_type_t       encode_type;
    void *(*allocator)(unsigned32);

    encode_type=sec_encode_get_encoding_type(num_bytes, bytes_p);
    if (encode_type == sec_encode_type_pwd)
    {
        sec_pwd_decode(alloc, dealloc, num_bytes, bytes_p, pwd_p, st_p);
    }
    else
    {
        allocator=alloc;
        sec_passwd_rec_t_unpickle((idl_pkl_t *)bytes_p, allocator, pwd_p, st_p);
    }
}

void sec_passwd_rec_t_unpickle(pickle, allocator, data, st)
idl_pkl_t *pickle;
void *(*allocator)(unsigned32);
sec_passwd_rec_t *data;
error_status_t *st;
{
    idl_byte *psp, *bsp;
    ndr_format_t drep;
    idl_ulong_int node_number0;
    idl_ulong_int node_number1;
    idl_ulong_int count;
    idl_ulong_int *pepper_as_ulong_p;
    idl_ulong_int *plain_as_ulong_p;
    idl_byte *element;
    uuid_t typeid;

    idl_pkl_get_type_uuid(*pickle, &typeid);

    if (!uuid_equal(&sec_passwd_rec_t_typeid, &typeid, st))
    {
        /* Note: signature must be changed to take a length
         * input parameter if sec_passwd_pubkey and sec_passwd_genprivkey
         * are to be handled here
         */
        *st = rpc_s_wrong_pickle_type;
        return;
    }

    *st = error_status_ok;

    psp = bsp = *pickle + idl_pkl_data_offset;
    drep = *(ndr_format_t *)psp;
    rpc_advance_mp(psp, 8);

    rpc_convert_ulong_int(drep, ndr_g_local_drep, psp, data->version_number);
    rpc_advance_mp(psp, 4);
    pepper_as_ulong_p = (idl_ulong_int *)&data->pepper;
    rpc_convert_ulong_int(drep, ndr_g_local_drep, psp, (*pepper_as_ulong_p));
    rpc_advance_mp(psp, 4);
    rpc_convert_enum(drep, ndr_g_local_drep, psp, data->key.key_type);
    rpc_advance_mp(psp, 2);

    switch (data->key.key_type)
    {
        case 1:
            pkl_align_psp(psp, bsp, 4);
            plain_as_ulong_p = (idl_ulong_int *)&data->key.tagged_union.plain;
            rpc_convert_ulong_int(drep, ndr_g_local_drep, psp, (*plain_as_ulong_p));
            rpc_advance_mp(psp, 4);
            break;
        case 2:
            count = (7-0+1);
            element = data->key.tagged_union.des_key;
            while (count)
            {
                rpc_convert_byte(drep, ndr_g_local_drep, psp, (*element));
                rpc_advance_mp(psp, 1);
                element++;
                count--;
            }
            break;
        default:
            *st = rpc_s_fault_invalid_tag;
            return;
    }

    anon_str_ptr_unpickle(
        &psp, bsp, drep, allocator, &data->pepper, st);

    switch (data->key.key_type)
    {
        case 1:
            anon_str_ptr_unpickle(
                &psp, bsp, drep, allocator, &data->key.tagged_union.plain, st);
            break;
        case 2:
            break;
    }
}

void sec_id_pac_t_pickle(data, syntax, allocator, alloc_mod, pickle, pickle_len, st)
sec_id_pac_t *data;
rpc_syntax_id_t *syntax;
void *(*allocator)(unsigned32);
unsigned32 alloc_mod;
idl_pkl_t **pickle;
unsigned32 *pickle_len;
error_status_t *st;
{
    unsigned32 stream_len = 8; /* ndr format label and fill bytes */
    idl_byte *psp, *bsp; /* pickle data pointers */
    idl_byte *bp;
    unsigned16 index;
    sec_id_t *groups_element;
    sec_id_foreign_t *foreign_groups_element;

    *st = error_status_ok;

    stream_len +=  2  /* data->pac_type (enum) */
                 + 2  /* gap */
                 + 4  /* data->authenticated */
                 + 20 /* data->realm (uuid and node #) */
                 + 20 /* data->principal (uuid and node #) */
                 + 20 /* data->group (uuid and node #) */
                 + 2  /* data->num_groups */
                 + 2  /* data->num_foreign_groups */
                 + 4  /* data->groups node number */
                 + 4; /* data->foreign groups node number */

    adjust_len_for_string(&stream_len, data->realm.name);
    adjust_len_for_string(&stream_len, data->principal.name);
    adjust_len_for_string(&stream_len, data->group.name);

    if (data->groups != NULL)
    {
        pkl_align_psp(stream_len, 0, 4);
        stream_len += 4; /* Z for *groups */
        stream_len += data->num_groups * 20; /* uuid and node # */
        for (index = 0; index < data->num_groups; index++)
            adjust_len_for_string(
                &stream_len,
                data->groups[index].name);
    }
    if (data->foreign_groups != NULL)
    {
        pkl_align_psp(stream_len, 0, 4);
        stream_len += 4; /* Z for *foreign_groups */
        stream_len += data->num_foreign_groups * 2 * 20;
        for (index = 0; index < data->num_foreign_groups; index++)
        {
            adjust_len_for_string(
                &stream_len,
                data->foreign_groups[index].id.name);
            adjust_len_for_string(
                &stream_len,
                data->foreign_groups[index].realm.name);
        }
    }

    *pickle_len = stream_len + idl_pkl_data_offset;
    *pickle_len = (*pickle_len + (alloc_mod-1)) & ~(alloc_mod-1);
    *pickle = (*allocator)(*pickle_len);

    /* ought to check input syntax, but ndr is currently the only choice */

    idl_pkl_set_header (**pickle, 0, stream_len, &ndr_syntax_id, &sec_id_pac_t_typeid);

    psp = bsp = **pickle + idl_pkl_data_offset;

    init_ptr_pickling();

    /*
     * This code represents an architectural decision:
     *
     *    In a byte stream in an (NDR-flavored) pickle the first
     *    four bytes are the format label and the next four bytes are
     *    0.  The pickled data starts at byte 8 of the data part.
     */
    *(ndr_format_t *)psp = ndr_g_local_drep;
    rpc_advance_mp(psp, 4);
    rpc_marshall_ulong_int(psp, 0);
    rpc_advance_mp(psp, 4);


    rpc_marshall_enum(psp, data->pac_type);
    rpc_advance_mp(psp, 2);
    pkl_align_psp(psp, bsp, 4);
    rpc_marshall_ulong_int(psp, data->authenticated);
    rpc_advance_mp(psp, 4);

    sec_id_t_pickle(&data->realm, &psp, bsp);
    sec_id_t_pickle(&data->principal, &psp, bsp);
    sec_id_t_pickle(&data->group, &psp, bsp);
 
    rpc_marshall_ushort_int(psp, (data->num_groups));
    rpc_advance_mp(psp, 2);
    rpc_marshall_ushort_int(psp, (data->num_foreign_groups));
    rpc_advance_mp(psp, 2);

    ptr_pickle(data->groups, &psp, bsp);
    ptr_pickle(data->foreign_groups, &psp, bsp);

    sec_id_tP_pickle(&data->realm, &psp, bsp);
    sec_id_tP_pickle(&data->principal, &psp, bsp);
    sec_id_tP_pickle(&data->group, &psp, bsp);

    if (data->groups != NULL)
    {
        pkl_align_psp(psp, bsp, 4);
        rpc_marshall_ulong_int(psp, data->num_groups);
        rpc_advance_mp(psp, 4);
        groups_element = data->groups;
        for (index = data->num_groups; index; index--)
        {
            sec_id_t_pickle(groups_element, &psp, bsp);
            groups_element++;
        }
        groups_element = data->groups;
        for (index = data->num_groups; index; index--)
        {
            sec_id_tP_pickle(groups_element, &psp, bsp);
            groups_element++;
        }
    }

    if (data->foreign_groups != NULL)
    {
        pkl_align_psp(psp, bsp, 4);
        rpc_marshall_ulong_int(psp, data->num_foreign_groups);
        rpc_advance_mp(psp, 4);
        foreign_groups_element = data->foreign_groups;
        for (index = data->num_foreign_groups; index; index--)
        {
            sec_id_t_pickle(&foreign_groups_element->id, &psp, bsp);
            sec_id_t_pickle(&foreign_groups_element->realm, &psp, bsp);
            foreign_groups_element++;
        }
        foreign_groups_element = data->foreign_groups;
        for (index = data->num_foreign_groups; index; index--)
        {
            sec_id_tP_pickle(&foreign_groups_element->id, &psp, bsp);
            sec_id_tP_pickle(&foreign_groups_element->realm, &psp, bsp);
            foreign_groups_element++;
        }
    }
}

void sec_id_pac_t_unpickle(pickle, allocator, data, st)
idl_pkl_t *pickle;
void *(*allocator)(unsigned32);
sec_id_pac_t *data;
error_status_t *st;
{
    idl_byte *psp, *bsp;
    ndr_format_t drep;
    idl_ulong_int Z;
    unsigned16 index;
    sec_id_t *groups_element;
    sec_id_foreign_t *foreign_groups_element;
    uuid_t typeid;

    idl_pkl_get_type_uuid (*pickle, &typeid);
    if (!uuid_equal(&sec_id_pac_t_typeid, &typeid, st))
    {
        *st = rpc_s_wrong_pickle_type;
        return;
    }

    *st = error_status_ok;

    psp = bsp = *pickle + idl_pkl_data_offset;
    drep = *(ndr_format_t *)psp;
    rpc_advance_mp(psp, 8);

    rpc_convert_enum(drep, ndr_g_local_drep, psp, data->pac_type);
    rpc_advance_mp(psp, 2);
    pkl_align_psp(psp, bsp, 4);
    rpc_convert_ulong_int(drep, ndr_g_local_drep, psp, data->authenticated);
    rpc_advance_mp(psp, 4);

    sec_id_t_unpickle(&psp, bsp, drep, allocator, &data->realm, st);
    sec_id_t_unpickle(&psp, bsp, drep, allocator, &data->principal, st);
    sec_id_t_unpickle(&psp, bsp, drep, allocator, &data->group, st);

    rpc_convert_ushort_int(drep, ndr_g_local_drep, psp, (data->num_groups));
    rpc_advance_mp(psp, 2);
    rpc_convert_ushort_int(drep, ndr_g_local_drep, psp, (data->num_foreign_groups));
    rpc_advance_mp(psp, 2);

    ptr_unpickle(&psp, bsp, drep, allocator, &data->groups, st);
    ptr_unpickle(&psp, bsp, drep, allocator, &data->foreign_groups, st);

    sec_id_tP_unpickle(&psp, bsp, drep, allocator, &data->realm, st);
    sec_id_tP_unpickle(&psp, bsp, drep, allocator, &data->principal, st);
    sec_id_tP_unpickle(&psp, bsp, drep, allocator, &data->group, st);

    if (data->groups != NULL)
    {
        pkl_align_psp(psp, bsp, 4);
        rpc_convert_ulong_int(drep, ndr_g_local_drep, psp, Z);
        rpc_advance_mp(psp, 4);
        if (Z == 0)
        {
            data->groups = (*allocator)(1);
        }
        else
        {
            data->groups = (*allocator)(Z*sizeof(sec_id_t));
            groups_element = data->groups;
            for (index = Z; index; index--)
            {
                sec_id_t_unpickle(&psp, bsp, drep, allocator,
                    groups_element, st);
                groups_element++;
            }
            groups_element = data->groups;
            for (index = Z; index; index--)
            {
                sec_id_tP_unpickle(&psp, bsp, drep, allocator,
                    groups_element, st);
                groups_element++;
            }
        }
    }

    if (data->foreign_groups != NULL)
    {
        pkl_align_psp(psp, bsp, 4);
        rpc_convert_ulong_int(drep, ndr_g_local_drep, psp, Z);
        rpc_advance_mp(psp, 4);
        if (Z == 0)
        {
            data->foreign_groups = (*allocator)(1);
        }
        else
        {
            data->foreign_groups = (*allocator)(Z*sizeof(sec_id_foreign_t));
            foreign_groups_element = data->foreign_groups;
            for (index = Z; index; index--)
            {
                sec_id_t_unpickle(
                    &psp, bsp, drep, allocator,
                    &foreign_groups_element->id, st);
                sec_id_t_unpickle(
                    &psp, bsp, drep, allocator,
                    &foreign_groups_element->realm, st);
                foreign_groups_element++;
            }
            foreign_groups_element = data->foreign_groups;
            for (index = Z; index; index--)
            {
                sec_id_tP_unpickle(
                    &psp, bsp, drep, allocator,
                    &foreign_groups_element->id, st);
                sec_id_tP_unpickle(
                    &psp, bsp, drep, allocator,
                    &foreign_groups_element->realm, st);
                foreign_groups_element++;
            }
        }
    }
}
