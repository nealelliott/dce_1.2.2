/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_pk.h,v $
 * Revision 1.1.2.4  1996/10/03  14:52:02  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:45 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Add new domains for KDC pubkey retrieval with and without refresh
 * 	[1996/09/10  21:09 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 *
 * 	Support versions for public keys
 * 	[1996/09/06  18:20 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	Merge Bill's change
 * 	[1996/08/13  13:49 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 *
 * 	Bug fixes for rsec_pk_cache and sec_pubkey
 * 	[1996/07/18  17:35 UTC  aha  /main/DCE_1.2.2/4]
 *
 * 	add prototypes (for some functions which we'll want to rename later).
 * 	[1996/06/20  03:54 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/1]
 *
 * 	Merge initial portion of KDC pvtkey retrieval
 * 	[1996/08/14  12:52 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 *
 * 	merge DCEPKPrivateKeyStorage_id
 * 	[1996/08/12  16:16 UTC  aha  /main/DCE_1.2.2/aha_pk8/2]
 *
 * 	Changes for Public Key Login; add declaration of
 * 	DCEPKPrivateKeyStorage_id
 * 	[1996/08/02  13:02 UTC  aha  /main/DCE_1.2.2/aha_pk8/1]
 *
 * 	Changes for Public Key Login; define UUID for KDC pk-init domain.
 * 	[1996/07/18  17:35 UTC  aha  /main/DCE_1.2.2/4]
 *
 * 	Bug fixes for rsec_pk_cache and sec_pubkey
 * 	[1996/07/13  20:51 UTC  aha  /main/DCE122_PK/aha_pk6/2]
 *
 * Revision 1.1.2.3  1996/08/09  12:05:45  arvind
 * 	Bug fixes for rsec_pk_cache and sec_pubkey
 * 	[1996/07/13  20:51 UTC  aha  /main/DCE122_PK/aha_pk6/2]
 * 
 * 	Changes for Public Key Login; define keyfile id and version for use
 * 	in sec_pubkey_read_keyfile().
 * 	[1996/06/18  20:57 UTC  aha  /main/DCE122_PK/aha_pk6/1]
 * 
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 	[1996/06/30  22:37 UTC  cuti  /main/DCE_1.2.2/3]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  18:00 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * Revision 1.1.2.2  1996/07/11  17:26:21  arvind
 * 	OSF DCE 1.2.2 Drop3
 * 	[1996/07/11  17:25:34  arvind]
 * 
 * Revision /main/DCE_1.2.2/3  1996/06/30  22:37 UTC  cuti
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 
 * Revision /main/DCE122_PK/cuti_pk_export/2  1996/06/27  19:54 UTC  cuti
 * 	Merge out from DCE122
 * 
 * 	Add sec_psm_base.h
 * 	[1996/06/19  15:11 UTC  cuti  /main/DCE122_PK/cuti_pk_export/1]
 * 	Revision /main/DCE_1.2.2/2  1996/06/13  21:30 UTC  sommerfeld
 * 	add sec_pk_data_create; correct comments prior to sec_pk_data_copy..
 * 	[1996/06/11  19:37 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
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
 * Revision /main/DCE12_PK/1  1996/04/23  20:02 UTC  aha
 * 	Add sec_pk files
 * 
 * Revision /main/aha_pk2/1  1996/04/16  19:54 UTC  aha
 * 	Public key local definitions
 * 
 * Revision 1.2  96/04/12  09:27:44  09:27:44  aha (Anne Anderson)
 * 	update
 * 
 * Revision 1.1  96/04/11  15:47:04  15:47:04  aha (Anne Anderson)
 * 	Initial revision
 * 
 * Revision /main/DCE_1.2/1  1996/04/11  13:40 UTC  aha
 * 	Header file for public key operations
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1996
 * Unpublished work. All Rights Reserved.
 */

#ifndef _SEC_PK_H_
#define _SEC_PK_H_

#include <dce/nbase.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_psm_base.h>
#include <dce/sec_attr_base.h>

/* The following sec_pk_domain_t UUIDs are
 * architectural.  They cannot be changed without
 * destroying interoperability.
 */

/* sec_pk_domain_dce_general: for use by
 * authenticated clients in retrieving and storing
 * public and private keys */
extern uuid_t sec_pk_domain_dce_general;
              /* ae991638-5cbe-11cf-a22d-08000919ebb5 */

/* sec_pk_domain_kdc_cache: for use by
 * unauthenticated clients in retrieving the
 * public key of a krbtgt from the dced-maintained
 * krbtgt key cache.
 */
extern uuid_t sec_pk_domain_kdc_cache;
              /* e925f8b4-0a3a-11d0-94f1-08000919ebb5 */

/* sec_pk_domain_pk_kdc_cache_refresh: for use by
 * unauthenticated clients in retrieving a
 * refreshed cache copy of the public key of a
 * krbtgt.
 */
extern uuid_t sec_pk_domain_kdc_cache_refresh;
              /* f087b8e0-0a3a-11d0-9113-08000919ebb5 */

/* sec_pk_domain_kdc_pk_init: for use by the KDC
 * in performing sec_psm operations using its own
 * private key.
 */
extern uuid_t sec_pk_domain_kdc_pk_init;
              /* 7d2d9810-e721-11cf-9021-08000919ebb5 */

/* UUID's of DCE public key-related ERA's
 *
 */

/* DCEPKAuthentication_id: UUID of the
 * DCEPKAuthentication ERA, used to hold a
 * principal's sec_pk_usage_digitalSignature
 * Public Key for use in the DCE Public Key Login
 * protocol
 */
extern uuid_t DCEPKAuthentication_id;
              /* d44dc60e-6230-11cf-98ed-08000919ebb5 */

/* DCEPKKeyEncipherment_id: UUID of the
 * DCEPKKeyEncipherment ERA, used to hold a
 * principal's sec_pk_usage_keyEncipherment Public
 * Key for use in the DCE Public Key Login
 * protocol
 */
extern uuid_t DCEPKKeyEncipherment_id;
              /* 76251f8c-6230-11cf-af89-08000919ebb5 */

/* DCEPKPrivateKeyStorage_id: UUID of the
 * DCEPKPrivateKeyStorage ERA, used to associate
 * a principal with a private key storage mechanism.
 * The UUID's of the mechanisms themselves are contained in
 * file sec_psm_base.h
 */
extern uuid_t DCEPKPrivateKeyStorage_id;
              /* 1f79c78e-b026-11cf-b7c8-0800090a5254 */

/* Following format is used in the header of a public
 * or private key keyfile.  A PK keyfile is used both
 * by the dced in caching the public keys of krbtgt
 * principals, and also by dcecp in reading public
 * or private key values from a file supplied by
 * the user.
 *
 * The values used in the header are defined in
 * sec_pk.c
 */
typedef struct {
    char         keyfile_id[4];
    unsigned32   keyfile_fmt_ver;
} sec_pk_keyfile_hdr_t;

extern const sec_pk_keyfile_hdr_t sec_pk_keyfile_hdr;
#define DCEPKKEYFILE_ID sec_pk_keyfile_hdr.keyfile_id;
#define DCEPKKEYFILE_VER sec_pk_keyfile_hdr.keyfile_fmt_ver;

/* s e c _ p k _ d a t a _ i n i t
 *
 * Initializes sec_pk_data_t or its type-specific variants.
 *
 *   In Parameters:
 *       data_p  - pointer to a sec_pk_data_t
 *                 that is to be initialized.
 *                 data_p is returned
 *                 with data_p->len set to 0 and
 *                 data_p->data set to NULL.
 *
 *   Errors: none.
 */
#define sec_pk_data_init(data_p) do { sec_pk_data_t *__d = (data_p); __d->data = NULL; __d->len = 0; } while (0)

/* s e c _ p k _ d a t a _ c r e a t e
 *
 * Copies data to a sec_pk_data_t or its type-specific variants.
 *
 *   In Parameters:
 *       data, len
 *                pointer to, and length of, memory to be copied
 *   Out Parameters:
 *       data_out_p  - pointer to a sec_pk_data_t
 *                 that receives a pointer to the copied data.
 *
 *   Errors:
 *     error_status_ok    - success
 *     sec_pk_e_no_memory - malloc failed
 */
error_status_t sec_pk_data_create (
    unsigned8       *data,	/* [in] */
    size_t	    len,	/* [in] */
    sec_pk_data_t   *data_out_p	/* [out] */
);


/* s e c _ p k _ d a t a _ c o p y
 *
 * Copies a sec_pk_data_t or its type-specific variants.
 *
 *   In Parameters:
 *       data_in_p  - pointer to a sec_pk_data_t
 *                 that points to the memory to be copied
 *   Out Parameters:
 *       data_out_p  - pointer to a sec_pk_data_t
 *                 which receives a pointer to the copied data.
 *
 *   Errors:
 *     error_status_ok    - success
 *     sec_pk_e_no_memory - malloc failed
 */
error_status_t sec_pk_data_copy (
    sec_pk_data_t   *data_in_p,	/* [in] */
    sec_pk_data_t   *data_out_p	/* [out] */
);


/* s e c _ p k _ d a t a _ f r e e
 *
 * Frees memory associated with data of type
 * sec_pk_data_t or its type-specific variants.
 *
 *   In Parameters:
 *       data_p  - pointer to a sec_pk_data_t
 *                 that points to the memory
 *                 to be reclaimed.
 *                 data_p is returned
 *                 with data_p->data set to NULL and
 *                 data_p->len set to 0.
 *
 *   Errors: None
 */
void sec_pk_data_free (
    sec_pk_data_t   *data_p       /* [in,out] */
);


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
 *       data_p  - pointer to a sec_pk_data_t
 *                 that points to the memory
 *                 to be reclaimed.
 *                 data_p is returned with
 *                 data_p->data set to NULL,
 *                 and data_p->len set to 0.
 *
 *   Errors: none
 */
void sec_pk_data_zero_and_free (
    sec_pk_data_t   *data_p         /* [in,out] */
);

/* sec_pk_get_key_usage_count
 *
 *   In Parameters:
 *       key_usages  - a value of type sec_pk_usage_flags_t
 *           containing key usage flag settings
 *   Return value:
 *       integer count of the number of key usage
 *       flags set in key_usages
 *
 *   Errors: none
 *
extern int sec_pk_get_key_usage_count(
    sec_pk_usage_flags_t     key_usages
);


/* s e c _ p k _ a t t r _ w r i t e _ s e t u p
 *
 * Function:
 *     Set up an attribute value with public key data
 *     for call to rs_attr_update()
 *      -sets attr->attr_id to id
 *      -IDL-encodes the public key data as a
 *       sec_passwd_rec_t of type sec_passwd_pubkey
 *      -allocates attr->attr_value.tagged_union.bytes
 *      -copies IDL-encoded public key data to 
 *       tagged_union.bytes->data
 *
 * In Parameters:
 *     alloc    - allocator (e.g. malloc or rpc_ss_allocate)
 *     dealloc  - deallocator (e.g. free or rpc_ss_free)
 *     attr     - pointer to an attribute to be set up
 *     id       - UUID of attribute to be written
 *     kvno     - pointer to key version number to be
 *                written; must not be 0!
 *     pk_data_p - pointer to a sec_pk_data_t
 *                containing the key value to
 *                be written as an attribute
 * Errors:
 *     error_status_ok
 *     sec_pk_e_no_memory - malloc failed
 *     sec_pk_e_no_key_value - input pk_data is empty
 *     sec_pk_e_key_vers_unsupported - input kvno is
 *         NULL, < 0, or > 255
 * Errors passed through from:
 *     sec_pwd_encode()
 */
error_status_t sec_pk_attr_write_setup(
    idl_void_p_t       (*alloc)(idl_size_t size),
    void               (*dealloc)(idl_void_p_t ptr),
    sec_attr_t         *attr,
    uuid_t             id,
    unsigned32         *kvno,
    sec_pk_data_t      *pk_data_p
);

#endif
