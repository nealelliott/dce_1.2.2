/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_pk.c,v $
 * Revision 1.1.2.3  1996/10/03  15:15:32  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:45 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Define new domains for KDC public key retrieval with and without refresh
 * 	[1996/09/10  21:09 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 *
 * 	Store public key values in ERA's as IDL-encoded sec_passwd_rec_t of
 * 	type sec_passwd_privkey; include version number
 * 	[1996/09/06  18:20 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	Merge initial portion of KDC pvtkey retrieval
 * 	[1996/08/14  12:51 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 *
 * 	merge DCEPKPrivateKeyStorage_id
 * 	[1996/08/12  16:17 UTC  aha  /main/DCE_1.2.2/aha_pk8/2]
 *
 * 	Changes for Public Key Login; define DCEPKPrivateKeyStorage_id
 * 	[1996/08/02  13:02 UTC  aha  /main/DCE_1.2.2/aha_pk8/1]
 *
 * 	Changes for Public Key Login; define a domain for KDC pk-init operations.
 * 	[1996/07/18  17:35 UTC  aha  /main/DCE_1.2.2/4]
 *
 * 	Bug fixes for rsec_pk_cache and sec_pubkey
 * 	[1996/07/13  20:51 UTC  aha  /main/DCE122_PK/aha_pk6/2]
 *
 * Revision 1.1.2.2  1996/08/09  12:12:59  arvind
 * 	Bug fixes for rsec_pk_cache and sec_pubkey
 * 	[1996/07/13  20:51 UTC  aha  /main/DCE122_PK/aha_pk6/2]
 * 
 * 	Changes for Public Key Login; define keyfile record ID and version.
 * 	[1996/06/18  20:57 UTC  aha  /main/DCE122_PK/aha_pk6/1]
 * 
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 	[1996/06/30  22:43 UTC  cuti  /main/DCE_1.2.2/3]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  18:00 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * Revision 1.1.2.1  1996/07/08  20:21:35  arvind
 * 	OSF DCE1.2.2 Drop3
 * 	sec_pk.c,v
 * 	Revision /main/DCE_1.2.2/3  1996/06/30  22:43 UTC  cuti
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 	[1996/07/08  20:07:12  arvind]
 * 
 * Revision /main/DCE122_PK/cuti_pk_export/2  1996/06/27  19:55 UTC  cuti
 * 	Merge out from DCE122
 * 
 * 	Remove get_key_usage()
 * 	[1996/06/27  14:08 UTC  cuti  /main/DCE122_PK/cuti_pk_export/1]
 * 	Revision /main/DCE_1.2.2/2  1996/06/13  21:30 UTC  sommerfeld
 * 	fix comments to match reality; add sec_pk_data_create
 * 	[1996/06/11  19:47 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/06/11  18:00 UTC  aha
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 
 * Revision /main/DCE122_PK/1  1996/06/04  18:53 UTC  psn
 * 	Merge PK work from DCE12_PK off of DCE_1.2 to DCE122_PK off of DCE_1.2.2.
 * 
 * Revision /main/DCE12_PK/4  1996/05/28  18:00 UTC  cuti
 * 	Merge psm, pvtkey work
 * 
 * 	Merge bugfixes for dced and sec_pubkey based on code review
 * 	[1996/05/21  18:34 UTC  aha  /main/DCE12_PK/3]
 * 
 * 	Remove BindKeyInfo_t stuff
 * 	[1996/05/21  13:05 UTC  aha  /main/DCE12_PK/aha_pk3/1]
 * 
 * 	Merge udpated pvtkey stuff
 * 
 * 	Complete free routines
 * 	[1996/05/13  20:45 UTC  cuti  /main/DCE12_PK/cuti_pk_new/1]
 * 
 * 	Wrote the free routines
 * 	[1996/05/08  14:52 UTC  cuti  /main/DCE12_PK/cuti_pk_work/1]
 * 	Revision /main/DCE12_PK/cuti_pk_new/2  1996/05/24  20:38 UTC  cuti
 * 	Move get_key_usage_count() and get_key_usage() from sec_pvtkey.c to sec_pk.c
 * 
 * Revision /main/DCE12_PK/cuti_pk_new/1  1996/05/13  20:45 UTC  cuti
 * 	Complete free routines
 * 
 * Revision /main/DCE12_PK/cuti_pk_work/1  1996/05/08  14:52 UTC  cuti
 * 	Wrote the free routines
 * 
 * Revision /main/DCE12_PK/1  1996/04/23  20:02 UTC  aha
 * 	Add sec_pk files
 * 
 * Revision /main/aha_pk2/1  1996/04/23  19:18 UTC  aha
 * 	Implementation of sec_pk.h
 * 
 * Revision 1.2  96/04/12  09:27:07  09:27:07  aha (Anne Anderson)
 * 	update
 * 
 * Revision 1.1  96/04/11  15:47:03  15:47:03  aha (Anne Anderson)
 * 	Initial revision
 * 
 * Revision /main/aha_pk1/1  1996/04/11  13:48 UTC  aha
 * 	Implementation of sec_pk interface
 * 
 * $EndLog$
 */

/*
**
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

/*
** Implementation of sec_pk_base.idl and sec_pk.h
*/

#include <dce/rgynbase.h>
#include <rgymacro.h>
#include <rsdb_pvt.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pk.h>
#include <dce/idlbase.h>
#include <dce/sec_attr_base.h>
#include <sec_encode.h>

/* UUID's of DCE public key domains
 */

sec_pk_domain_t sec_pk_domain_dce_general = { 
    /* ae991638-5cbe-11cf-a22d-08000919ebb5 */
    0xae991638,
    0x5cbe,
    0x11cf,
    0xa2,
    0x2d,
    {0x08, 0x00, 0x09, 0x19, 0xeb, 0xb5}
};

sec_pk_domain_t sec_pk_domain_kdc_cache = {
    /* e925f8b4-0a3a-11d0-94f1-08000919ebb5 */
    0xe925f8b4,
    0x0a3a,
    0x11d0,
    0x94,
    0xf1,
    {0x08, 0x00, 0x09, 0x19, 0xeb, 0xb5}
};

sec_pk_domain_t sec_pk_domain_kdc_cache_refresh = {
    /* f087b8e0-0a3a-11d0-9113-08000919ebb5 */
    0xf087b8e0,
    0x0a3a,
    0x11d0,
    0x91,
    0x13,
    {0x08, 0x00, 0x09, 0x19, 0xeb, 0xb5}
};

sec_pk_domain_t sec_pk_domain_kdc_pk_init = { 
    /* 7d2d9810-e721-11cf-9021-08000919ebb5 */
    0x7d2d9810,
    0xe721,
    0x11cf,
    0x90,
    0x21,
    {0x08, 0x00, 0x09, 0x19, 0xeb, 0xb5}
};

/* UUID's of DCE public key related ERA's
 */

uuid_t DCEPKKeyEncipherment_id = { 
    /* 76251f8c-6230-11cf-af89-08000919ebb5 */
    0x76251f8c,
    0x6230,
    0x11cf,
    0xaf,
    0x89,
    {0x08, 0x00, 0x09, 0x19, 0xeb, 0xb5}
};

uuid_t DCEPKAuthentication_id = { 
    /* d44dc60e-6230-11cf-98ed-08000919ebb5 */
    0xd44dc60e,
    0x6230,
    0x11cf,
    0x98,
    0xed,
    {0x08, 0x00, 0x09, 0x19, 0xeb, 0xb5}
};

uuid_t DCEPKPrivateKeyStorage_id = { 
    /* 1f79c78e-b026-11cf-b7c8-0800090a5254 */
    0x1f79c78e,
    0xb026,
    0x11cf,
    0xb7,
    0xc8,
    {0x08, 0x00, 0x09, 0x0a, 0x52, 0x54}
};


/* Following values are used in the header of a public
 * or private key keyfile.  The values are extern'd in
 * sec_pk.h
 */
const sec_pk_keyfile_hdr_t sec_pk_keyfile_hdr = {{(char)'P',(char)'K',(char)'E',(char)'Y'}, 2};

/* s e c _ p k _ d a t a _ c r e a t e
 *
 * Initializes a sec_pk_data_t or its type-specific variants.
 *
 *   In Parameters:
 *       data, len
 *                pointer to, and length of, memory to be copied
 *   Out Parameters:
 *       data_out_p  - pointer to a sec_pk_data_t
 *                 which recieves a pointer to the copied data.
 *
 *   Errors:
 *     error_status_ok    - success
 *     sec_pk_e_no_memory - malloc failed
 */
error_status_t sec_pk_data_create (
    unsigned8       *data,
    size_t	    len,
    sec_pk_data_t   *data_out_p	/* [out] */
)
{
    unsigned8 *outdata = malloc(len);
    if (outdata == NULL) {
	sec_pk_data_init(data_out_p);	
	return sec_pk_e_no_memory;
    }
    memcpy(outdata, data, len);
    data_out_p->len = len;
    data_out_p->data = outdata;
    return error_status_ok;
}

/* s e c _ p k _ d a t a _ c o p y
 *
 * Copies a sec_pk_data_t or its type-specific variants.
 *
 *   In Parameters:
 *       data_in_p  - pointer to a sec_pk_data_t
 *                 that points to the memory to be copied
 *   Out Parameters:
 *       data_out_p  - pointer to a sec_pk_data_t
 *                 which recieves a pointer to the copied data.
 *
 *   Errors:
 *     error_status_ok    - success
 *     sec_pk_e_no_memory - malloc failed
 */
error_status_t sec_pk_data_copy (
    sec_pk_data_t   *data_in_p,	/* [in] */
    sec_pk_data_t   *data_out_p	/* [out] */
)
{
    size_t len = data_in_p->len;
    unsigned8 *outdata = malloc(len);

    if (outdata == NULL) {
	sec_pk_data_init(data_out_p);	
	return sec_pk_e_no_memory;
    }
    
    memcpy(outdata, data_in_p->data, len);
    data_out_p->len = len;
    data_out_p->data = outdata;
    return error_status_ok;
}


/* s e c _ p k _ d a t a _ f r e e
 *
 * Frees memory associated with data of type
 * sec_pk_data_t or its type-specific variants.
 *
 *   In Parameters:
 *       data_p  - pointer to a sec_pk_data_p_t
 *                 that points to the memory
 *                 to be reclaimed.
 *                 sec_pk_data_p_t is returned
 *                 set to NULL.
 *
 *   Errors: none.
 */
void sec_pk_data_free (
    sec_pk_data_t   *data_p       /* [in,out] */
)
{
    if (data_p) { 
	unsigned8 *data = data_p->data;
        if (data) { 
            free(data);
	    sec_pk_data_init(data_p);
        }
    }
}


/* s e c _ p k _ d a t a _ z e r o _ a n d _ f r e e
 *
 * Zeroes out, then frees memory associated with data
 * of type sec_pk_data_t or its type-specific variants.
 *
 * This function, rather than sec_pk_data_free, should
 * be called for structures containing private or
 * secret keys.
 *
 *   In Parameters:
 *       data_p  - pointer to a sec_pk_data_p
 *                 that points to the memory
 *                 to be reclaimed.
 *                 sec_pk_data_p is returned
 *                 set to NULL.
 *
 *   Errors: none.
 */
void sec_pk_data_zero_and_free (
    sec_pk_data_t   *data_p         /* [in,out] */
)
{
    if (data_p) { 
	unsigned8 *data = data_p->data;
        if (data) { 
	    memset(data, 0, data_p->len);
            free(data);
	    sec_pk_data_init(data_p);	    
        }
    }
}

int sec_pk_get_key_usage_count(
    sec_pk_usage_flags_t  	key_usages)
{

    int i;

    for (i = 0; key_usages != 0; key_usages >>= 1)
	if (key_usages & 01)
	    i++;

    return i;

}

/* s e c _ p k _ a t t r _ w r i t e _ s e t u p
 *
 * Function:
 *     Set up an attribute value for call to rs_attr_update()
 *      -sets attr->attr_id to id
 *      -allocates attr->attr_value.tagged_union.bytes
 *      -copies pubkey_p->data to tagged_union.bytes->data
 *
 * In Parameters:
 *     alloc    - allocator (e.g. malloc or rpc_ss_allocate)
 *     attr     - pointer to an attribute to be set up
 *     id       - UUID of attribute to be written
 *     kvno     - key version number
 *     pk_data_p - pointer to a sec_pk_data_t
 *                containing the key value to
 *                be written as an attribute
 * Errors:
 *     error_status_ok
 *     sec_pk_e_no_memory - malloc failed
 *     sec_pk_e_no_key_value - input pk_data is empty
 */
error_status_t sec_pk_attr_write_setup(
    idl_void_p_t       (*alloc)(idl_size_t size),
    void               (*dealloc)(idl_void_p_t ptr),
    sec_attr_t         *attr,
    uuid_t             id,
    unsigned32         *kvno,
    sec_pk_data_t      *pk_data_p
)
{
    sec_passwd_rec_t           new_key;
    error_status_t             st;
    unsigned32                 num_bytes;
    idl_byte                   *bytes_p;

    attr->attr_id=id;
    attr->attr_value.attr_encoding=sec_attr_enc_bytes;
    if ((kvno == NULL) ||
        (*kvno < rsdb_auth_key_min_version_number) ||
        (*kvno > rsdb_auth_key_max_version_number))
        return sec_pk_e_key_vers_unsupported;

    if (pk_data_p != NULL && pk_data_p->data != NULL) {

        /* Create sec_passwd_pubkey from public key value */
        new_key.pepper=NULL;
        PUBKEY_PASSWD_DATA(&new_key)=(idl_byte *)alloc(pk_data_p->len);
        if (PUBKEY_PASSWD_DATA(&new_key) == NULL)
            return sec_pk_e_no_memory;
        PUBKEY_PASSWD_LEN(&new_key)=pk_data_p->len;
        PASSWD_TYPE(&new_key)=sec_passwd_pubkey;
        memcpy(PUBKEY_PASSWD_DATA(&new_key), pk_data_p->data, pk_data_p->len);
        new_key.version_number=*kvno;

        /* IDL-encode the new sec_passwd_pubkey value */
        sec_pwd_encode(alloc, dealloc, &new_key, &num_bytes, &bytes_p, &st);

        /* free the password data, since we are done with it */
        dealloc(PUBKEY_PASSWD_DATA(&new_key));
        PUBKEY_PASSWD_DATA(&new_key)=NULL;
        if (BAD_STATUS(&st))
            return st;

        /* allocate attribute data: key length field + key data field:
         * size of sec_attr_enc_bytes_t, which includes one byte of data,
         * plus length of the data array, minus one byte which was included
         * in the sec_attr_enc_bytes_t
         */
        attr->attr_value.tagged_union.bytes = (sec_attr_enc_bytes_t *)alloc(
            sizeof(sec_attr_enc_bytes_t) + num_bytes - 1);

        if (attr->attr_value.tagged_union.bytes == NULL) {
            dealloc(bytes_p);
            return sec_pk_e_no_memory;
        }

        /* copy public key length and data into attribute data
         * have to use memcpy because key value may contain '0'
         */
        (void)memcpy(&attr->attr_value.tagged_union.bytes->data,
                     bytes_p,
                     num_bytes);
        attr->attr_value.tagged_union.bytes->length = num_bytes;

        /* free the IDL-encoded passwd, since we are done with it */
        dealloc(bytes_p);

    } else {

        /* Input pubkey value is empty!! */
        attr->attr_value.tagged_union.bytes = NULL;
        return sec_pk_e_no_key_value;
    }

    return error_status_ok;
}

    
