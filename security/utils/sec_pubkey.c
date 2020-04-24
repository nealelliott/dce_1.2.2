/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_pubkey.c,v $
 * Revision 1.1.2.4  1996/11/13  18:12:23  arvind
 * 	CHFts20136: Change find_domain to sec_pubkey_find_domain() and make
 * 	it public.
 * 	[1996/10/15  19:54 UTC  aha  /main/DCE_1.2.2/aha_pk10/1]
 *
 * 	CHFts20360: free of uninit pointer.
 * 	[1996/10/16  23:26 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_3/1]
 *
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:45 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	CHFts19679: keep the retry on connection closed.
 * 	[1996/08/12  18:55 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 *
 * 	CHFts19763: Change "== 0" to "= 0" in a non-conditional statement in
 * 	sec_pubkey_write_keyfile()
 * 	[1996/07/18  17:35 UTC  aha  /main/DCE_1.2.2/3]
 *
 * 	Bug fixes for rsec_pk_cache and sec_pubkey
 * 	[1996/07/16  18:52 UTC  aha  /main/DCE122_PK/aha_pk6/11]
 *
 * 	Define malloc for Kernel and otherwise
 * 	[1996/07/16  17:34 UTC  aha  /main/DCE122_PK/aha_pk6/10]
 *
 * 	Eliminate duplicate malloc_shim symbol
 * 	[1996/07/16  15:20 UTC  aha  /main/DCE122_PK/aha_pk6/9]
 *
 * 	Remove include of sec_encode.h
 * 	[1996/07/16  13:58 UTC  aha  /main/DCE122_PK/aha_pk6/8]
 *
 * 	Include rca_pvt.h rather than rca_fileloc.h in order to pick up
 * 	correct definition of EXTERN
 * 	[1996/07/15  19:43 UTC  aha  /main/DCE122_PK/aha_pk6/7]
 *
 * Revision 1.1.2.3  1996/10/03  15:15:57  arvind
 * 	don't fall off the end of a function.
 * 	[1996/09/15  23:09 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 * 
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:45 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 * 
 * 	Fix syntax error
 * 	[1996/09/10  21:09 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/3]
 * 
 * 	Support key versions in sec_pubkey_retrieve_kdc_key
 * 	[1996/09/06  18:20 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 * 
 * 	Support versions for public keys in sec_pubkey_retrieve_attr and
 * 	sec_pubkey_store_attr
 * 	[1996/09/06  16:22 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 * 
 * 	Fix test for version# in sec_pubkey_retrieve_attr()
 * 	[1996/08/15  20:14 UTC  aha  /main/DCE_1.2.2/aha_pk9/3]
 * 
 * 	Debug problem with correct address of sec_pubkey_*_attr in
 * 	known_domains[] table.
 * 	[1996/08/15  19:21 UTC  aha  /main/DCE_1.2.2/aha_pk9/2]
 * 
 * 	Pubkey domains.
 * 
 * 	clean up direction of kvno flow in the interface.
 * 	[1996/08/09  17:15 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/1]
 * 
 * 	Add retry on call to rsec_pk_cache_kdc_key() as workaround for
 * 	CN-RPC bug CHFts19679
 * 	[1996/07/31  13:06 UTC  aha  /main/DCE_1.2.2/4]
 * 
 * 	CHFts19679: keep the retry on connection closed.
 * 	[1996/08/12  18:55 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 * 
 * 	CHFts19763: Change "== 0" to "= 0" in a non-conditional statement in
 * 	sec_pubkey_write_keyfile()
 * 	[1996/07/18  17:35 UTC  aha  /main/DCE_1.2.2/3]
 * 
 * 	Bug fixes for rsec_pk_cache and sec_pubkey
 * 	[1996/07/16  18:52 UTC  aha  /main/DCE122_PK/aha_pk6/11]
 * 
 * 	Define malloc for Kernel and otherwise
 * 	[1996/07/16  17:34 UTC  aha  /main/DCE122_PK/aha_pk6/10]
 * 
 * 	Eliminate duplicate malloc_shim symbol
 * 	[1996/07/16  15:20 UTC  aha  /main/DCE122_PK/aha_pk6/9]
 * 
 * 	Remove include of sec_encode.h
 * 	[1996/07/16  13:58 UTC  aha  /main/DCE122_PK/aha_pk6/8]
 * 
 * 	Include rca_pvt.h rather than rca_fileloc.h in order to pick up
 * 	correct definition of EXTERN
 * 	[1996/07/15  19:43 UTC  aha  /main/DCE122_PK/aha_pk6/7]
 * 
 * Revision 1.1.2.2  1996/08/09  12:13:23  arvind
 * 	Add retry on call to rsec_pk_cache_kdc_key() as workaround for
 * 	CN-RPC bug CHFts19679
 * 	[1996/07/18  17:35 UTC  aha  /main/DCE_1.2.2/3]
 * 
 * 	Bug fixes for rsec_pk_cache and sec_pubkey
 * 	[1996/07/16  18:52 UTC  aha  /main/DCE122_PK/aha_pk6/11]
 * 
 * 	Define malloc for Kernel and otherwise
 * 	[1996/07/16  17:34 UTC  aha  /main/DCE122_PK/aha_pk6/10]
 * 
 * 	Eliminate duplicate malloc_shim symbol
 * 	[1996/07/16  15:20 UTC  aha  /main/DCE122_PK/aha_pk6/9]
 * 
 * 	Remove include of sec_encode.h
 * 	[1996/07/16  13:58 UTC  aha  /main/DCE122_PK/aha_pk6/8]
 * 
 * 	Include rca_pvt.h rather than rca_fileloc.h in order to pick up
 * 	correct definition of EXTERN
 * 	[1996/07/15  19:43 UTC  aha  /main/DCE122_PK/aha_pk6/7]
 * 
 * 	Changes for Public Key Login; fix rsec_pk_cache_kdc problems
 * 	[1996/06/19  20:33 UTC  aha  /main/DCE122_PK/aha_pk6/5]
 * 
 * 	Temporary checkout for debugging
 * 	[1996/06/07  19:07 UTC  aha  /main/DCE122_PK/aha_pk6/3]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  18:00 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * Revision 1.1.2.1  1996/07/08  20:21:45  arvind
 * 	OSF DCE1.2.2 Drop3
 * 	sec_pubkey.c,v
 * 	Revision /main/DCE_1.2.2/2  1996/06/13  21:30 UTC  sommerfeld
 * 	Fix obvious bugs in previous change; simplify some code paths.
 * 	[1996/06/11  19:48 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/06/11  18:00 UTC  aha
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 
 * Revision /main/DCE122_PK/2  1996/06/10  14:58 UTC  aha
 * 	Merge to DCE122_PK
 * 
 * Revision /main/DCE122_PK/aha_pk6/2  1996/06/06  18:52 UTC  aha
 * 	Revert to original version, except for including rca_pvt.h
 * 	instead of rca_fileloc.h (rca_pvt.h provides necessary context
 * 	for including rca_fileloc.h).
 * 
 * Revision /main/DCE122_PK/1  1996/06/04  18:53 UTC  psn
 * 	Merge PK work from DCE12_PK off of DCE_1.2 to DCE122_PK off of DCE_1.2.2.
 * 
 * Revision /main/DCE12_PK/4  1996/05/21  18:34 UTC  aha
 * 	Merge bugfixes for dced and sec_pubkey based on code review
 * 
 * Revision /main/DCE12_PK/aha_pk3/1  1996/05/21  13:05 UTC  aha
 * 	KDC key cache file in DCELOCAL/KDC_PK_CACHE_DIR/<cell_name>/KDC_PK_CACHE_FILE,
 * 	where <cell_name> can be hierarchical.
 * 
 * Revision /main/DCE12_PK/3  1996/05/16  14:20 UTC  aha
 * 	Additions for Public Key Login
 * 
 * 	 *
 * 	Merge sec_pubkey.c
 * 	[1996/04/26  20:46 UTC  aha  /main/DCE12_PK/1]
 * 
 * Revision /main/aha_pk2/10  1996/05/13  19:42 UTC  aha
 * 	Debug fixes
 * 
 * Revision /main/aha_pk2/9  1996/05/10  15:50 UTC  aha
 * 	Debug fixes
 * 
 * Revision /main/aha_pk2/8  1996/05/08  20:50 UTC  aha
 * 	Need error status from unsuccessful read in sec_pubkey_read_cached_key
 * 
 * Revision /main/aha_pk2/7  1996/05/08  20:31 UTC  aha
 * 	Use sec_pubkey_read_cached_kdc_key in sec_pubkey_retrieve_kdc_key
 * 
 * Revision /main/aha_pk2/6  1996/05/07  21:16 UTC  aha
 * 	Update with most recent version from pubkey_test
 * 
 * Revision /main/aha_pk2/5  1996/05/07  20:06 UTC  aha
 * 	Pass canonical cell name to rsec_pk_cache_kdc_key()
 * 
 * Revision /main/aha_pk2/4  1996/05/02  15:48 UTC  aha
 * 	Fix bugs - now passes component tests
 * 
 * Revision /main/aha_pk2/3  1996/04/24  17:18 UTC  aha
 * 	Fix use of input parameter "id" in attr_write_setup(); fix
 * 	handling of reply status and failure_id.
 * 
 * Revision /main/aha_pk2/2  1996/04/23  19:19 UTC  aha
 * 	Change num_enc_bytes to len, encoded_data_p to data
 * 
 * Revision /main/aha_pk2/1  1996/04/18  22:56 UTC  aha
 * 	Implementation of sec_pubkey.idl
 * 
 * Revision 1.2  96/04/12  09:29:04  09:29:04  aha (Anne Anderson)
 * 	update
 * 
 * Revision 1.1  96/04/11  15:47:06  15:47:06  aha (Anne Anderson)
 * 	Initial revision
 * 
 * Revision /main/aha_pk1/1  1996/04/11  13:46 UTC  aha
 * 	Implementation of sec_pubkey interface
 * 
 * Revision /main/aha_mothra12/4  1996/04/01  19:28 UTC  aha
 * 	Make types conform to idl files
 * 
 * Revision /main/aha_mothra12/3  1996/03/07  20:46 UTC  aha
 * 	minor fixes
 * 
 * Revision /main/aha_mothra12/2  1996/03/07  18:11 UTC  aha
 * 	Make ERA uuid_t's external
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
** Implementation of sec_pk_base.idl and sec_pubkey.idl
*/

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <u_str.h>
#include <malloc.h>
#include <sys/stat.h>
#include <dce/dce_error.h>
#include <dce/uuid.h>
#include <dce/idlbase.h>
#include <dce/binding.h>
#include <dce/rgybase.h>
#include <rgymacro.h>
#include <dce/sec_attr_base.h>
#include <dce/sec_attr_util.h>
#include <dce/sec_rgy_attr.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pubkey.h>
#include <sec_attr_tools.h>
#include <sec_encode.h>
#include <dce/rsec_pk_cache.h>
#include <sec_pk.h>
#include <macros.h>
#include <rca_pvt.h>
#include <dce/passwd.h>

static error_status_t sec_pubkey_retrieve_attr(
    void 		    *principal,
    sec_pk_domain_t         *domain,
    sec_pk_usage_flags_t    usage,       /* [in] */
    unsigned32              *kvno,        /* [in,out] */
    sec_pk_pubkey_t         *pubkey_p    /* [out] */
);

static error_status_t sec_pubkey_store_attr (
    void                  *principal,   /* [in] */
    sec_pk_domain_t       *domain,    /* [in] */
    sec_pk_usage_flags_t  usage,     /* [in] */
    unsigned32            *kvno,      /* [in,out] */
    sec_pk_pubkey_t       *pubkey_p   /* [in] */
);

static error_status_t sec_pubkey_retrieve_kdc_key(
    void                   *cell,       /* [in] */
    sec_pk_domain_t        *domain,     /* [in] */
    sec_pk_usage_flags_t   usage,       /* [in] */
    unsigned32             *kvno,       /* [in,out] */
    sec_pk_pubkey_t        *pubkey_p    /* [out] */
);

static error_status_t sec_pubkey_store_unsupported (
    void                  *principal,   /* [in] */
    sec_pk_domain_t       *domain,    /* [in] */
    sec_pk_usage_flags_t  usage,     /* [in] */
    unsigned32            *kvno,      /* [in,out] */
    sec_pk_pubkey_t       *pubkey_p   /* [in] */
);

#define MAX_WRITE_ATTRS 2

extern char *		mkmalloc();

/*
**  ROUTINE NAME:       malloc_shim_local
**
**  DESCRIPTION:
**
**  A simple malloc wrapper function that takes a size
**  argument of type idl_size_t and coerces it to
**  size_t in a call to the native malloc() routine.
**
**  Use this shim function when you want to pass the native
**  malloc to a routine that takes a memory allocation function
**  parameter whose signature is typed using idl_size_t.
**  In such cases, if you pass the native malloc directly and
**  a size_t is of a difference precision than an idl_size_t,
**  the compiler will issue a warning (a warning that, for
**  portability to architectures of arbitrary word size, should
**  be taken seriously).
**
**  INPUTS:
**
**      size            The number of bytes to be allocated
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     idl_void_p_t
**
**  pointer to allocated memory, or NULL, if the allocation failed
**  for any reason.
**
**  SIDE EFFECTS:       none
**
**/

idl_void_p_t malloc_shim_local (
    idl_size_t size
)
{
    /* just call the native malloc and coerce
     * the idl_size_t to a size_t.  This avoids any possible
     * precision problems.
     */

#ifdef KERNEL
    return mkmalloc((unsigned int) size);
#else
    return (idl_void_p_t)malloc((size_t) size);
#endif
}

#define MAX_DOMAINS 10

static sec_pk_domain_def known_domains[MAX_DOMAINS] = {
     {  /* sec_pk_domain_dce_general ae991638-5cbe-11cf-a22d-08000919ebb5 */
         {0xae991638,0x5cbe,0x11cf,0xa2,0x2d,{0x08,0x00,0x09,0x19,0xeb,0xb5}},
         sec_pubkey_retrieve_attr,
         sec_pubkey_store_attr
     },
     {  /* sec_pk_domain_kdc_cache e925f8b4-0a3a-11d0-94f1-08000919ebb5 */
         {0xe925f8b4,0x0a3a,0x11d0,0x94,0xf1,{0x08,0x00,0x09,0x19,0xeb,0xb5}},
         sec_pubkey_retrieve_kdc_key,
         sec_pubkey_store_unsupported
     },
     {  /* sec_pk_domain_kdc_cache_refresh f087b8e0-0a3a-11d0-9113-08000919ebb5 */
         {0xf087b8e0,0x0a3a,0x11d0,0x91,0x13,{0x08,0x00,0x09,0x19,0xeb,0xb5}},
         sec_pubkey_retrieve_kdc_key,
         sec_pubkey_store_unsupported
     }
};

PUBLIC sec_pk_domain_def *sec_pubkey_find_domain(
    sec_pk_domain_t *domain
) 
{
    int i;
    error_status_t xst;

    for (i=0; i<MAX_DOMAINS; i++) {
        if (uuid_equal(domain, &known_domains[i].domain, &xst))
            return &known_domains[i];
    }
    return NULL;
}

static sec_pk_domain_def *find_empty_domain(void) 
{
    int i;
    
    for (i=0; i<MAX_DOMAINS; i++) {
	if (known_domains[i].r)
	    continue;
	if (known_domains[i].s)
	    continue;
	return &known_domains[i];
    }
    return NULL;
}

error_status_t sec_pubkey_define_domain (
    sec_pk_domain_t *domain,
    sec_pk_key_retrieval_fn_t retrieve,
    sec_pk_key_retrieval_fn_t store
) 
{
    sec_pk_domain_def *pkd;

    pkd = sec_pubkey_find_domain(domain);
    if (pkd == NULL)
	pkd = find_empty_domain();
    if (pkd == NULL)
	return sec_pk_e_no_memory;
    pkd->domain = *domain;
    pkd->r = retrieve;
    pkd->s = store;

    return error_status_ok;
}

/* s e c _ p u b k e y _ s t o r e
 *
 * Function:
 *     Store a public key value for a principal
 *
 *     Stores an X.509 DER-encoded public key value of type
 *     SubjectPublicKeyInfo in the public key storage
 *     facility.  This operation overwrites any
 *     existing public key storage for <principal>, and
 *     creates public key storage if none exists.
 *
 *     In the reference implementation:
 *     - "domain" must always be "sec_pk_domain_dce_general"
 *     - "usage" must be either "sec_pk_usage_digitalSignature",
 *       "sec_pk_usage_keyEncipherment", or both.
 *     - If "usage" is "sec_pk_usage_digitalSignature" the public 
 *       key is stored in the "DCEPKAuthentication" ERA attached
 *       to the principal.
 *     - If "usage" is "sec_pk_usage_keyEncipherment", the public key is
 *       stored in the "DCEPKKeyEncipherment" ERA.
 *
 *     Default ACLs for the reference implementation on the
 *     underlying ERA storage locations will allow only a
 *     principal with "sec_admin" privileges to perform
 *     this operation.  Sites may choose to modify the ACLs
 *     on the underlying ERAs to allow the owner principal
 *     to modify the public key as well, although sites
 *     should be aware that ability to modify a principal's
 *     public key equates to the ability to impersonate the
 *     principal.
 *
 * In Parameters:
 *     principal  - the canonical name in the specified
 *                  domain of the principal whose public
 *                  key is being stored (sec_rgy_name_t
 *                  containing /.../cell/principal in
 *                  reference implementation)
 *     domain     - a UUID identifying the domain in which
 *                  the key is used
 *                  (sec_pk_domain_dce_general in
 *                  reference implementation)
 *     usage      - usage(s) of the key being stored
 *                  (sec_pk_usage_digitalSignature,
 *                  sec_pk_usage_keyEncipherment, or both
 *                  in reference implementation)
 *     kvno       - key version; if 0,
 *                  retrieves newest version.
 *                  (must be 0 in the reference implementation)
 *     pubkey_p   - pointer to an X.509 DER-encoded public key
 *                  value of type SubjectPublicKeyInfo
 *
 * Out Parameters:
 *     None
 *
 * Errors:
 *     error_status_ok
 *     sec_pk_e_domain_unsupported,
 *     sec_pk_e_usage_unsupported,
 *     sec_pk_e_key_vers_unsupported
 *     sec_pk_e_no_memory
 *     sec_pk_e_no_key_value   - pubkey_p does not contain a key value
 *
 * Errors passed through from:
 *     uuid_equal
 *     sec_rgy_attr_update
 */
static error_status_t sec_pubkey_store_attr (
    void                  *principal,   /* [in] */
    sec_pk_domain_t       *domain,    /* [in] */
    sec_pk_usage_flags_t  usage,     /* [in] */
    unsigned32            *kvno,      /* [in,out] */
    sec_pk_pubkey_t       *pubkey_p   /* [in] */
)
{
    error_status_t           st,lst;

    /* DCE PK Login Domain */

    unsigned32               num_to_write = 0, i;
    sec_attr_t               attrs[MAX_WRITE_ATTRS];
    unsigned32               num_returned;
    unsigned32               num_left;
    signed32                 failure_index;
    sec_rgy_name_t           global_rgy_name;
    sec_rgy_name_t           canon_rgy_principal;
    sec_rgy_name_t           canon_rgy_cell;
    sec_rgy_handle_t         rgy_context;

    CLEAR_STATUS(&st);

    /* verify other parameters */
    /* key version verified in sec_pk_attr_write_setup() */

    if (!(pubkey_p && pubkey_p->len > 0 && pubkey_p->data))
        return sec_pk_e_no_key_value;

    if ((usage & ~(sec_pk_usage_keyEncipherment | sec_pk_usage_digitalSignature)))
        return sec_pk_e_usage_unsupported;

    /* set up ERA parameters */
    if (usage & sec_pk_usage_keyEncipherment) {
        if (st=sec_pk_attr_write_setup(malloc_shim_local,
                                       free,
                                       &attrs[num_to_write],
                                       DCEPKKeyEncipherment_id,
                                       kvno,
                                       pubkey_p))
            return st;
        else
            num_to_write++;
    }
    if (usage & sec_pk_usage_digitalSignature) {
        if (st=sec_pk_attr_write_setup(malloc_shim_local,
                                       free,
                                       &attrs[num_to_write],
                                       DCEPKAuthentication_id,
                                       kvno,
                                       pubkey_p))
            return st;
        else
            num_to_write++;
    }
    if (num_to_write == 0)
        return sec_pk_e_usage_unsupported;

    /* Convert global_name to canonical cell and principal name */

    u_strncpy(global_rgy_name, (char *)principal, sizeof(sec_rgy_name_t)-1);
    sec_id_global_parse_name(NULL,
			     global_rgy_name,
			     canon_rgy_cell,
			     NULL,
			     canon_rgy_principal,
			     NULL,
			     &st);
    if (BAD_STATUS(&st))
	return st;

    /* Bind to the registry for update */
    sec_rgy_site_open_update(canon_rgy_cell, &rgy_context, &st);
    if (BAD_STATUS(&st))
	return st;

    /* write the ERA value(s) */
    sec_rgy_attr_update(rgy_context,
			sec_rgy_domain_person,
			canon_rgy_principal,
			num_to_write,
			(unsigned32) 0,
			attrs, 
			&num_returned,
			NULL,
			&num_left,
			&failure_index,
			&st);

    for (i=0; i<num_to_write; i++)
        sec_attr_util_free(rpc_ss_client_free, &attrs[i]);
    sec_rgy_site_close(rgy_context, &lst);
    return st;

}

/* s e c _ p u b k e y_ r e t r i e v e _ a t t r
 *
 * PRIVATE
 *
 * Function:
 *      makes the actual ERA calls to obtain a public key;
 *      also checks for valid usage and kvno values.
 *
 * In Parameters:
 *     principal  - principal name
 *     usage      - must be sec_pk_usage_digitalSignature
 *                  or sec_pk_usage_keyEncipherment
 *                  in the reference implementation
 *     kvno       - key version to be retrieved; if 0,
 *                  retrieves newest version.
 *                  (must be 0 in the reference implementation)
 *
 * Out Parameters:
 *     pubkey_p - pointer to pointer to a public key
 *                  structure
 * Errors:
 *     error_status_ok
 *     sec_pk_e_key_vers_unsupported
 *     sec_pk_e_usage_unsupported
 *     sec_pk_e_key_attr_read_failed
 *     sec_pk_e_no_memory
 *
 * Errors passed through from:
 *     sec_id_global_parse_name
 *     sec_rgy_cell_bind
 *     sec_rgy_attr_cursor_alloc
 *     sec_rgy_attr_lookup_by_id
 */
static error_status_t sec_pubkey_retrieve_attr(
    void 		    *principal,
    sec_pk_domain_t         *domain,
    sec_pk_usage_flags_t    usage,       /* [in] */
    unsigned32              *kvno,        /* [in] */
    sec_pk_pubkey_t         *pubkey_p    /* [out] */
)
{
    error_status_t           st,lst;
    sec_rgy_handle_t         rgy_context;
    sec_attr_cursor_t        attr_cursor;
    sec_attr_t               attr_keys[1];
    sec_attr_t               out_attrs[1];
    unsigned32               num_returned=0;
    unsigned32               num_left=0;
    sec_rgy_name_t           canon_rgy_cell;
    sec_rgy_name_t           canon_rgy_principal;
    char                     *baddr;
    sec_rgy_name_t           global_rgy_name;
    sec_passwd_rec_t         passwd;

    /* Initialize out parameters */
    CLEAR_STATUS(&st);
    sec_pk_data_init(pubkey_p);

    u_strncpy(global_rgy_name, (char *)principal, sizeof(sec_rgy_name_t)-1);

    /* Check usage parameter and set up key for the 
     * attribute retrieval
     */

    if ((usage & sec_pk_usage_keyEncipherment) &&
        !(usage & ~sec_pk_usage_keyEncipherment))
        attr_keys[0].attr_id=DCEPKKeyEncipherment_id;
    else if ((usage & sec_pk_usage_digitalSignature) &&
        !(usage & ~sec_pk_usage_digitalSignature))
        attr_keys[0].attr_id=DCEPKAuthentication_id;
    else
        return sec_pk_e_usage_unsupported;

    attr_keys[0].attr_value.attr_encoding=sec_attr_enc_void;

    /* Convert global_name to canonical cell and principal name */

    sec_id_global_parse_name(NULL,
                             global_rgy_name,
                             canon_rgy_cell,
                             NULL,
                             canon_rgy_principal,
                             NULL,
                             &st);
    if (BAD_STATUS(&st))
        return st;

    sec_rgy_site_open_query (canon_rgy_cell, &rgy_context, &st);
    if (BAD_STATUS(&st))
        return st;

    /*
     * There should only be one instance of DCEPKKeyEncipherment or
     * DCEPKAuthentication attached to the principal, so we do not need
     * to use sec_rgy_attr_cursor_init() (which
     * is a remote interface) to initialize the cursor.
     */

    sec_rgy_attr_cursor_alloc(&attr_cursor, &st);
    if (BAD_STATUS(&st)) {
        sec_rgy_site_close(rgy_context, &lst);
        return st;
    }

    sec_rgy_attr_lookup_by_id(rgy_context,
                              sec_rgy_domain_person,
                              canon_rgy_principal,
                              &attr_cursor,
                              (unsigned32)1,
                              (unsigned32)1,
                              attr_keys,
                              &num_returned,
                              out_attrs,
                              &num_left,
                              &st);

    sec_rgy_attr_cursor_release(&attr_cursor, &lst);

    sec_rgy_site_close(rgy_context, &lst);

    /* ignore errors from cursor_release, site_close */

    if (BAD_STATUS(&st)) {
        return st;
    } else if ((num_returned != 1)
             || !uuid_equal(&attr_keys[0].attr_id,
			    &out_attrs[0].attr_id, &lst)) {
        if (num_returned >= 1)
            sec_attr_util_free(rpc_ss_client_free, &out_attrs[0]);
        return sec_pk_e_key_attr_read_failed;
    } else {

        /* Successfully read public key attribute */

        /* IDL-decode the attribute value */
        sec_pwd_decode(malloc_shim_local,
                       free,
                       out_attrs[0].attr_value.tagged_union.bytes->length,
                       out_attrs[0].attr_value.tagged_union.bytes->data,
                       &passwd,
                       &st);

        /* free attribute data structures */
        sec_attr_util_free(rpc_ss_client_free, &out_attrs[0]);
        if (BAD_STATUS(&st))
            return st;

        /* Check version number */
        if (kvno) {
            if ((*kvno != 0) && (passwd.version_number != *kvno)) {
                free(PUBKEY_PASSWD_DATA(&passwd));
                return sec_pk_e_key_vers_unavailable;
            } else {
                *kvno=passwd.version_number;
            }
        }

        /* allocate return parameter data */
        st = sec_pk_data_create(
            PUBKEY_PASSWD_DATA(&passwd),
            PUBKEY_PASSWD_LEN(&passwd),
            pubkey_p);

        /* free password data and return */
        free(PUBKEY_PASSWD_DATA(&passwd));
        return st;
    }
}

error_status_t sec_pubkey_store (
    void                   *principal,    /* [in] */
    sec_pk_domain_t        *domain,        /* [in] */
    sec_pk_usage_flags_t   usage,         /* [in] */
    unsigned32             *kvno,          /* [in,out] */
    sec_pk_pubkey_t        *pubkey_p      /* [in] */
)
{
    error_status_t         st;
    sec_pk_domain_def *pkd;
    
    CLEAR_STATUS(&st);

    /* Determine domain */

    pkd = sec_pubkey_find_domain(domain);
    if ((pkd == NULL) || (pkd->s == NULL)) {
        return sec_pk_e_domain_unsupported;
    } else {
	return ((pkd->s)(principal, domain, usage, kvno, pubkey_p));
    }
}



/* s e c _ p u b k e y _ r e t r i e v e
 *
 * Function:
 *     Retrieves an X.509 DER-encoded public key value with
 *     type SubjectPublicKeyInfo from the public key
 *     storage facility for a given principal.
 *
 *     In the reference implementation:
 *     - "domain" must always be "sec_pk_domain_dce_general"
 *     - "usage" must be either "sec_pk_usage_digitalSignature" or
 *       "sec_pk_usage_keyEncipherment"
 *     - If "usage" is "sec_pk_usage_digitalSignature" the
 *       public key is retrieved from the
 *       "DCEPKAuthentication" ERA attached to the
 *       principal.
 *     - If "usage" is "sec_pk_usage_keyEncipherment", the
 *       public key is retrieved from the
 *       "DCEPKKeyEncipherment" ERA.
 *
 *     Default ACLs for the reference implementation allow
 *     any principal to perform this operation.
 *
 * In Parameters:
 *     principal  - the canonical name in the specified
 *                  domain of the principal whose public
 *                  key is being retrieved
 *                  (sec_rgy_name_t in reference
 *                   implementation)
 *     domain     - a UUID identifying the domain in which
 *                  the key is used
 *                  (sec_pk_domain_dce_general in reference implementation)
 *     usage      - usage of the key being retrieved
 *                  (sec_pk_usage_digitalSignature, or
 *                  sec_pk_usage_keyEncipherment, but not
 *                   both, in reference implementation)
 *     kvno       - key version to be retrieved; if 0,
 *                  retrieves newest version.
 *                  (must be 0 in the reference implementation)
 *
 * Out Parameters:
 *     pubkey_p - pointer to an X.509 DER-encoded
 *                  public key value of type
 *                  SubjectPublicKeyInfo; storage for this
 *                  structure must be freed using the
 *                  "sec_pk_data_free()" function.
 *
 * Errors:
 *     error_status_ok
 *     sec_pk_e_key_vers_unsupported
 *     sec_pk_e_usage_unsupported
 *     sec_pk_e_no_memory
 *
 * Errors passed through from:
 *     sec_id_global_parse_name
 *     sec_rgy_cell_bind
 *     sec_rgy_attr_cursor_alloc
 *     sec_rgy_attr_lookup_by_id
 *     uuid_equal
 */
error_status_t sec_pubkey_retrieve (
    void                   *principal,    /* [in] */
    sec_pk_domain_t        *domain,        /* [in] */
    sec_pk_usage_flags_t   usage,         /* [in] */
    unsigned32             *kvno,          /* [in,out] */
    sec_pk_pubkey_t        *pubkey_p      /* [out] */
)
{
    error_status_t         st;
    sec_pk_domain_def *pkd;
    
    /* Initialize return parameters */


    CLEAR_STATUS(&st);
    sec_pk_data_init(pubkey_p);

    /* Determine domain */

    pkd = sec_pubkey_find_domain(domain);
    if ((pkd == NULL) || (pkd->r == NULL)) {
        return sec_pk_e_domain_unsupported;
    } else {
	return ((pkd->r)(principal, domain, usage, kvno, pubkey_p));
    }
}

/* s e c _ p u b k e y _ r e a d _ k e y f i l e
 *
 * Function:
 *    Read a key from a keyfile.  Keyfile is assumed
 *    to have format as follows:
 *    unsigned32  'P' 'K' 'E' 'Y'
 *    unsigned32  file format version# (must be 1 or 2)
 *    unsigned32  key_version (file format version#2 only)
 *    unsigned32  key_length
 *    byte        key[key_length]
 *
 * In Parameters:
 *    keyfilepath      -path to keyfile
 *    kvno             -pointer to desired version number (or NULL)
 *
 * Out Parameters:
 *    file_fmt_ver_p   -address of file format version (NULL if
 *                      no output file format desired).  This will
 *                      be returned if it can be read, even if
 *                      version does not match or key can't be read.
 *    kvno             -pointer to returned version number (or NULL)
 *    key_ret_p        -address of sec_pk_data_t key; caller must
 *                      allocate the sec_pk_data_t before calling
 *                      this function.  The caller must
 *                      deallocate key.data (and, if malloc'd,
 *                      key) when finished.  In the case of a
 *                      private key, key.data should be zeroed
 *                      before being deallocated.
 *
 * Errors:
 *     sec_pk_e_kf_param_not_alloc - A parameter other than file_fmt_ver_p NULL
 *     sec_pk_e_kf_open_err        - error opening the keyfile
 *     sec_pk_e_kf_read_err        - error reading the keyfile
 *     sec_pk_e_kf_format_err      - keyfile did not start with 'P''K''E''Y'
 *     sec_pk_e_kf_version_err     - keyfile version not 1
 *     sec_pk_e_no_memory          - unable to allocate memory for key data
 */
PUBLIC error_status_t sec_pubkey_read_keyfile(
    idl_char        *keyfilepath,     /* [in] */
    unsigned32      *kvno,            /* [in, out] */
    unsigned32      *file_fmt_ver_p,  /* [out] */
    sec_pk_data_t   *key_ret_p        /* [out] */
)
{
    FILE                 *tfp=NULL;
    sec_pk_data_t        key;
    int                  est=0;
    error_status_t       st;
    sec_pk_keyfile_hdr_t file_hdr;
    unsigned32           kvo=0;  /* key version read */
    unsigned32           fver=0;   /* file format version read */

    /* Initialize return parameters */
    CLEAR_STATUS(&st);
    if (key_ret_p)
        sec_pk_data_init(key_ret_p);
    if (file_fmt_ver_p)
        *file_fmt_ver_p=0;
    sec_pk_data_init(&key);

    /* Verify return parameters */
    if (!key_ret_p || !keyfilepath) {
        SET_STATUS(&st, sec_pk_e_kf_param_not_alloc);

    /* Open the file */
    } else if ((tfp = fopen((char *)keyfilepath, "r")) == (FILE *)NULL) {
        est=errno;
        st=sec_pk_e_kf_open_err;

    /* Read the magic from the keyfile header */
    } else if (fread(&file_hdr, sizeof(file_hdr), 1, tfp) != 1) {
        est=errno;
        st=sec_pk_e_kf_read_err;

    /* Check the file record ID */
    } else if (memcmp(&sec_pk_keyfile_hdr.keyfile_id,
                      &file_hdr.keyfile_id, sizeof(file_hdr.keyfile_id))) {
        st=sec_pk_e_kf_format_err;

    /* return file format version if requested */
    } else {
        fver=file_hdr.keyfile_fmt_ver;
        if (file_fmt_ver_p)
            *file_fmt_ver_p=fver;
    }

    if (BAD_STATUS(&st))
        goto cleanup;

    /* Check the file format version */
    if (fver != 1 && fver != 2) {
        st=sec_pk_e_kf_version_err;
        goto cleanup;

    /* Read the key version from the file (no key version in fver 1) */
    } else if ((fver == 2) && fread(&kvo, sizeof(unsigned32), 1, tfp) != 1) {
        est=errno;
        st=sec_pk_e_kf_read_err;
        goto cleanup;

    /* Read the length of the key data from the file */
    } else if (fread(&key.len, sizeof(unsigned32), 1, tfp) != 1) {
        est=errno;
        st=sec_pk_e_kf_read_err;

    /* Allocate space for the key data */
    } else if ((key.data=(idl_byte *)malloc(key.len))==NULL) {
        st=sec_pk_e_no_memory;

    /* Read the key data */
    } else if (fread(key.data, 1, key.len, tfp) != key.len) {
        est=errno;
        st=sec_pk_e_kf_read_err;

    /* successful read */
    } else {
        /* Check version number */
        if (kvno) {
            if (*kvno != 0 && *kvno != kvo)
                st=sec_pk_e_key_vers_unavailable;
            else
                *kvno=kvo;
        }
        if (STATUS_OK(&st)) {
            key_ret_p->len=key.len;
            key_ret_p->data=key.data;
        }
    }
cleanup:
    if (tfp != NULL)
        (void) fclose(tfp);
    if (BAD_STATUS(&st) && key.data != NULL)
        free(key.data);
    return st;
}

/* s e c _ p u b k e y _ w r i t e _ k e y f i l e
 *
 * Function:
 *    Write a key value to a keyfile.  Keyfile 
 *    has following format:
 *    unsigned32  'P' 'K' 'E' 'Y'
 *    unsigned32  file format version# (2)
 *    unsigned32  key_version
 *    unsigned32  key_length
 *    byte        key[key_length]
 *
 * In Parameters:
 *    keyfilepath      -path to keyfile
 *    key_p            -address of sec_pk_data_t structure holding
 *                      the key to be written
 *    key_version_p    -NULL (0) or pointer to key version
 *
 * Out Parameters:
 *    file_fmt_ver_p   -address of file format version written
 *                      (NULL if you don't want this)
 *    key_version_p    -NULL or pointer to key version written
 *
 * Errors:
 *     sec_pk_e_kf_param_not_alloc - parameter other than file_fmt_ver_p NULL
 *     sec_pk_e_kf_open_err        - error opening the keyfile for writing
 *     sec_pk_e_kf_write_err       - error writing the keyfile
 */
error_status_t sec_pubkey_write_keyfile(
    idl_char       *keyfilepath,   /* [in] */
    sec_pk_data_t  *key_p,         /* [in] */
    unsigned32     *key_version_p, /* [in,out] */
    unsigned32     *file_fmt_ver_p /* [out] */
)
{
    FILE                *tfp=NULL;
    int                 est=0;
    error_status_t      st;
    unsigned32          kvo;

    /* Initialize return parameters */
    CLEAR_STATUS(&st);
    if (file_fmt_ver_p)
        *file_fmt_ver_p = 0;

    /* Verify parameters */
    kvo = (key_version_p ? *key_version_p : 0);

    if (!keyfilepath || !key_p || !key_p->data)
        st=sec_pk_e_kf_param_not_alloc;

    else if (kvo > 255)
        st=sec_pk_e_key_vers_unsupported;

    /* Open the file */
    else if ((tfp = fopen((char *)keyfilepath, "w")) == (FILE *)NULL) {
        est=errno;
        st=sec_pk_e_kf_open_err;

    /* Write the record ID and file format version from the keyfile header */
    } else if (fwrite(&sec_pk_keyfile_hdr, sizeof(sec_pk_keyfile_hdr_t), 1, tfp) != 1) {
        est=errno;
        st=sec_pk_e_kf_write_err;

    /* Write the key version to the file */
    } else if (fwrite(&kvo, sizeof(unsigned32), 1, tfp) != 1) {
        est=errno;
        st=sec_pk_e_kf_write_err;

    /* Write the length of the key data to the file */
    } else if (fwrite(&key_p->len, sizeof(unsigned32), 1, tfp) != 1) {
        est=errno;
        st=sec_pk_e_kf_write_err;

    /* Write the key data to the file */
    } else if (fwrite(key_p->data, 1, key_p->len, tfp) != key_p->len) {
        est=errno;
        st=sec_pk_e_kf_write_err;

    /* successful write */
    } else if (file_fmt_ver_p != NULL) {
        *file_fmt_ver_p=sec_pk_keyfile_hdr.keyfile_fmt_ver;
    }

    /* Flush buffer to file and close */
    if (tfp != NULL)
        if (fclose(tfp) != 0)
            st=sec_pk_e_kf_write_err;

    /* Return resulting status */
    return st;
}

/* sec_pubkey_read_cached_kdc_key
 *
 * Function:
 *     read the public key for a target cell's KDC from the
 *     dced-maintained cache file.
 *
 *     For use by sec_pubkey_retrieve_kdc_key()
 *     Clients should call sec_pubkey_retrieve()
 *     with domain sec_pk_domain_kdc_cache.
 *
 * In Parameters:
 *     cell       - canonical cell name of cell
 *                  whose KDC key is desired
 *     kvno       - may be NULL or point to the
 *                  key version requested.  If
 *                  key version is 0, the best
 *                  available key will be returned.
 *
 * Out Parameters:
 *     kvno     - if not NULL, points to the key
 *                version being returned
 *     pubkey_p - pointer to an X.509 DER-encoded
 *                  public key value of type
 *                  SubjectPublicKeyInfo
 *
 * Errors:
 *     error_status_ok
 *     sec_pk_e_cell_too_long
 *     sec_pk_e_no_memory
 *
 * Errors passed through from:
 *     sec_pubkey_read_keyfile()
 */
static error_status_t sec_pubkey_read_cached_key(
    sec_rgy_name_t       cell,      /* [in] */
    unsigned32           *kvno,     /* [in, out] */
    sec_pk_pubkey_t      *pubkey_p  /* [out] */
)
{
    FILE                *tfp;
    char                file_name[sec_rgy_name_t_size];
    error_status_t      st;
    int                 est;
    signed32            flen;

    /* Initialize return parameter */
    sec_pk_data_init(pubkey_p);

    /* Check input cell name */
    /* File name will be: /opt/dcelocal/etc/security/kdc_pk_auth/<cell>/.key */
    flen=KDC_PUBKEY_CACHE_DIR_LEN + u_strlen(cell) - GLOBAL_DIR_ROOT_LEN + \
         strlen("/") + KDC_PUBKEY_CACHE_FILE_LEN;
    if (flen > sec_rgy_name_max_len)
        return sec_pk_e_cell_too_long;

    /* Construct file name */
    u_strcpy(file_name, KDC_PUBKEY_CACHE_DIR);
    u_strcat(file_name, cell+GLOBAL_DIR_ROOT_LEN);
    u_strcat(file_name, "/");
    u_strcat(file_name, KDC_PUBKEY_CACHE_FILE);
    if (u_strlen(file_name) != flen)
        return sec_pk_e_cell_too_long;

    return (st=sec_pubkey_read_keyfile((idl_char *)file_name, kvno, NULL, pubkey_p));
}

/* s e c _ p u b k e y _ r e t r i e v e _ k d c _ k e y
 *
 * Function:
 *     Retrieve the public key for a Kerberos Key
 *     Distribution Center (krbtgt principal of a cell).
 *     Unauthenticated clients can call this function,
 *     such as during login, and still receive a trusted
 *     value for the requested KDC's public key.
 *
 *     Returns an X.509 DER-encoded public key value of
 *     type SubjectPublicKeyInfo.
 *
 *     In the reference implementation:
 *       - "usage" must be "sec_pk_usage_digitalSignature"
 *       - The key is read from the dced-maintained KDC key
 *         cache file.  If not there, dced is contacted to
 *         obtain the key.
 *       - If "refresh" is TRUE, then dced is contacted to
 *         obtain a fresh key, even if the cache contains a
 *         key.
 *
 * In Parameters:
 *     cell       - the canonical name of the DCE cell for
 *                  which a KDC public key is requested
 *     usage      - usage of the key being retrieved
 *                  (must be sec_pk_usage_digitalSignature
 *                   in reference implementation)
 *     refresh    - if TRUE, request refresh of an existing
 *                  KDC key for this cell.
 *
 * Out Parameters:
 *     pubkey_p - an X.509 DER-encoded public key;
 *                  storage for this structure must be freed
 *                  using the "sec_pk_data_free()" function.
 *
 * Errors:
 *     error_status_ok
 *     sec_pk_usage_unsupported
 *
 * Errors passed through from:
 *     sec_id_global_parse_name
 *     sec_login_util_bind_to_sec_clientd
 *     rsec_pk_cache_kdc_key
 */
static error_status_t sec_pubkey_retrieve_kdc_key (
    void                   *cell,       /* [in] */
    sec_pk_domain_t        *domain,     /* [in] */
    sec_pk_usage_flags_t   usage,       /* [in] */
    unsigned32             *kvno,       /* [in,out] */
    sec_pk_pubkey_t        *pubkey_p    /* [out] */
)
{
    handle_t            handle;
    error_status_t      st,lst;
    FILE                *tfp;
    sec_rgy_name_t      canonical_cell;
    char                file_name[1024];
    boolean32           refresh;
    
    CLEAR_STATUS(&st);
    sec_pk_data_init(pubkey_p);

    if (uuid_equal(domain, &sec_pk_domain_kdc_cache_refresh, &lst))
        refresh=true;
    else if (uuid_equal(domain, &sec_pk_domain_kdc_cache, &lst))
        refresh=false;
    else
        return sec_pk_e_domain_unsupported;

    /* Verify parameters */
    if (usage != sec_pk_usage_digitalSignature)
        return sec_pk_e_usage_unsupported;

    /* Convert "cell" to canonical form */

    sec_id_global_parse_name(NULL, cell, canonical_cell, NULL, NULL, NULL, &st);
    if (BAD_STATUS(&st))
        return st;

    /* Read cached key (unless refresh requested) */
    if (!refresh) {
        st = sec_pubkey_read_cached_key(canonical_cell, kvno, pubkey_p);
        if (GOOD_STATUS(&st))
            return st;
    }

    /* Request key from dced */

    /* bind to dced sec_clientd */

    sec_login_util_bind_to_sec_clientd(&handle, &st);
    if (BAD_STATUS(&st))
        return st;

    /* request public key from dced using canonical cell name */

    st=(*rsec_pk_cache_v1_1_c_epv.rsec_pk_cache_kdc_key)
       (handle, canonical_cell, usage, kvno, pubkey_p);

    /* CHFts19679: connection closed often reported if there
     * has been a delay since previous call.
     * Workaround: retry once
     */
    if (STATUS_EQUAL(&st, rpc_s_connection_closed))
        st=(*rsec_pk_cache_v1_1_c_epv.rsec_pk_cache_kdc_key)
           (handle, canonical_cell, usage, kvno, pubkey_p);

    rpc_binding_free(&handle, &lst);

    /* ignore status from rpc_binding_free */

    return st;
}

/* 
 */
static error_status_t sec_pubkey_store_unsupported (
    void                  *principal,   /* [in] */
    sec_pk_domain_t       *domain,    /* [in] */
    sec_pk_usage_flags_t  usage,     /* [in] */
    unsigned32            *kvno,      /* [in,out] */
    sec_pk_pubkey_t       *pubkey_p   /* [in] */
)
{
    return sec_pk_e_domain_unsupported;
}
