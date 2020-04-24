/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_psm.c,v $
 * Revision 1.1.2.6  1996/11/13  18:10:14  arvind
 * 	CHFts20737: Add stub for sec_pvtkey__era_fetch_bypass() to fix
 * 	international build.
 * 	[1996/11/12  17:33 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_6/2]
 *
 * 	Correct value of sec_psm_pkss_mechanism UUID for
 * 	DCEPKPrivateKeyStorage UUID.
 * 	[1996/08/23  14:44 UTC  aha  /main/DCE_1.2.2/4]
 *
 * Revision 1.1.2.5  1996/10/15  20:54:12  arvind
 * 	Merge of cuti_pk_fix
 * 	[1996/10/02  18:28 UTC  cuti  /main/DCE_1.2.2/5]
 * 
 * 	Make OID define works
 * 	[1996/10/02  17:46 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/2]
 * 
 * 	Add sec_psm_kdc_pk_init_mechanism
 * 	[1996/09/26  16:20 UTC  cuti  /main/DCE_1.2.2/cuti_pk_fix/1]
 * 
 * 	Correct value of sec_psm_pkss_mechanism UUID for
 * 	DCEPKPrivateKeyStorage UUID.
 * 	[1996/08/23  14:44 UTC  aha  /main/DCE_1.2.2/4]
 * 
 * Revision 1.1.2.4  1996/10/04  16:44:47  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  16:44:34  arvind]
 * 
 * Revision /main/DCE_1.2.2/5  1996/10/02  18:28 UTC  cuti
 * 	Merge of cuti_pk_fix
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_fix/2  1996/10/02  17:46 UTC  cuti
 * 	Make OID define works
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_fix/1  1996/09/26  16:20 UTC  cuti
 * 	Add sec_psm_kdc_pk_init_mechanism
 * 
 * Revision /main/DCE_1.2.2/4  1996/08/23  14:44 UTC  aha
 * 	Merge cuti_pk_bugfix, including sommerfeld_pk_kdc, to aha_pk9
 * 	[1996/08/13  21:34 UTC  aha  /main/DCE_1.2.2/aha_pk9/1]
 * 
 * 	Merge Bill's change
 * 	[1996/08/12  19:22 UTC  cuti  /main/DCE_1.2.2/cuti_pk_bugfix/1]
 * 
 * Revision /main/DCE_1.2.2/3  1996/07/17  13:26 UTC  cuti
 * 	Add missing symbol (sec_psm_pkss_mechansim, sec_psm_file_mechansim).
 * 
 * 	Fixed for international build.
 * 	[1996/07/12  14:01 UTC  cuti  /main/DCE_1.2.2/2]
 * 	Revision /main/DCE_1.2.2/cuti_pk_migration/5  1996/07/16  13:40 UTC  cuti
 * 	Add pkss mechansim
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_migration/4  1996/07/15  13:56 UTC  cuti
 * 	Change RSADSI to RSA_PKCS
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_migration/3  1996/07/15  13:53 UTC  cuti
 * 	Add CREATE_PK_DESCRIPTOR
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_migration/2  1996/07/12  13:59 UTC  cuti
 * 	Update the function comments
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_migration/1  1996/07/11  15:25 UTC  cuti
 * 	Change modulus_bit_size from [in,out] to [in]
 * 
 * Revision /main/DCE_1.2.2/1  1996/06/30  22:43 UTC  cuti
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 
 * Revision /main/cuti_pk_export/1  1996/06/27  14:08 UTC  cuti
 * 	Remove BSAFE
 * 
 * 	Bug fix on sec_psm_close()
 * 	[1996/06/14  15:10 UTC  cuti  /main/DCE12_PK/cuti_pk_new/7]
 * 
 * 	change vno to 0 for public key
 * 	[1996/06/10  15:43 UTC  cuti  /main/DCE12_PK/cuti_pk_new/6]
 * 
 * 	Change modulus_size to in/out
 * 	[1996/06/04  18:00 UTC  cuti  /main/DCE12_PK/cuti_pk_new/5]
 * 	Revision /main/DCE122_PK/2  1996/06/13  20:08 UTC  sommerfeld
 * 	deal with uninitialized vbl.
 * 	[1996/06/13  17:56 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/2]
 * 
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * Revision /main/DCE122_PK/1  1996/06/04  18:54 UTC  psn
 * 	DCE12_PK -> DCE122_PK
 * 
 * Revision /main/DCE12_PK/2  1996/05/28  18:00 UTC  cuti
 * 	Merge psm, pvtkey work
 * 
 * Revision /main/DCE12_PK/cuti_pk_new/4  1996/05/24  20:37 UTC  cuti
 * 	Update from review
 * 
 * Revision /main/DCE12_PK/cuti_pk_new/3  1996/05/24  19:24 UTC  cuti
 * 	Change local_algorithm_id to sec_pk_algorithm_id
 * 
 * Revision /main/DCE12_PK/cuti_pk_new/2  1996/05/23  18:42 UTC  cuti
 * 	Prepare for psm review.
 * 
 * $EndLog$
 */


#include <rgymacro.h>
#include <dce/rgynbase.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pvtkey.h>
#include <dce/sec_pvtkey_base.h>
#include <dce/sec_psm.h>
#include <sec_bsafe.h>
#include <sec_pk.h>
#include <sec_psm_base.h>

typedef struct sec_psm_context {
    void              *magic;
    char              *name;  /* canonical name */
    char              *pwd;
    unsigned32        machanism_index;  
    sec_pk_mechanism_handle_t   mechanism_handle;
    sec_pk_domain_t   domain_id;
} sec_psm_context_t, sec_psm_context_p_t;

uuid_t sec_psm_pkss_mechanism = {
    /* 72053e72-b01a-11cf-8bf5-0800090a5254 */
    0x72053e72,
    0xb01a,
    0x11cf,
    0x8b,
    0xf5,
    {0x08, 0x00, 0x09, 0x0a, 0x52, 0x54}
};


uuid_t sec_psm_file_mechanism = {
    /* 8687c5b8-b01a-11cf-b137-0800090a5254 */
    0x8687c5b8,
    0xb01a,
    0x11cf,
    0xb1,
    0x37,
    {0x08, 0x00, 0x09, 0x0a, 0x52, 0x54}
};

uuid_t sec_psm_kdc_pk_init_mechanism = {
    /* adb48ed4-e94d-11cf-ab4b-08000919ebb5 */
    0xadb48ed4,
    0xe94d,
    0x11cf,
    0xab,
    0x4b,
    {0x08, 0x00, 0x09, 0x19, 0xeb, 0xb5}
};

#define CREATE_PK_DESCRIPTOR(desc) sec_pk_data_init(desc)

/* RSA PKCS's key */
idl_byte rsa_pkcs_oid[] =
{0x2a,0x86,0x48,0x86,0xf7,0x0d};

/* signature algorithm  */
idl_byte  md5_rsa_oid[] =
{0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x05};

/* RSA encryption algorithm */
idl_byte  rsa_enc_oid[] =
{0x2a,0x86,0x48,0x86,0xf7,0x0d,0x01,0x01,0x01};

#define OID(XX) \
sec_pk_algorithm_id_t XX = { sizeof(XX##_oid), (idl_byte *)&XX##_oid}

#define rsapkcs OID(rsa_pkcs)
#define md5rsa OID(md5_rsa)
#define rsaenc OID(rsa_enc)

rsapkcs;
md5rsa;
rsaenc;


/*
sec_pk_algorithm_id_t rsa_pkcs = {sizeof(rsa_pkcs_oid), (idl_byte *)&rsa_pkcs_oid};
sec_pk_algorithm_id_t md5_rsa = {sizeof(md5_rsa_oid), (idl_byte *)&md5_rsa_oid};
sec_pk_algorithm_id_t rsa_enc = {sizeof(rsa_enc_oid), (idl_byte *)&rsa_enc_oid};
*/

/* sec_psm_open
 * 
 * Open the personal security mechanism using password.  
 * 
 * Return status
 *   error_status_ok:    Success.
 *   other (non-zero):   sec_pvtkey_privileged
 *                       sec_pvtkey_no_more_memory
 *                       sec_psm_no_more_memory
 *
 * Input
 *
 *   name: Pointer to the user's canonical name within the 
 *         specified domain. 
 *   pwd: Pointer to the user's password.
 *   domain_id: Pointer to the application domain the user 
 *              is operating on.
 *
 * Output
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data. 
 *
 */


error_status_t sec_psm_open(
    void               *name,
    idl_char           *pwd,
    sec_pk_domain_t    *domain_id,
    sec_psm_handle_t   *psm_handle
) {

    *psm_handle = NULL;

    return error_status_ok;

}

/* sec_psm_close
 * 
 * Close the personal security mechanism and cleanup the 
 * personal security context data.  It also ensures any
 * confidential information such as passwords or private
 * key are zeroed.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *         		sec_psm_invalid_handle
 * 	  		sec_psm_internal_error
 *	  		sec_pvtkey_invalid_handle
 *	  		sec_pvtkey_mechanism_not_init
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be 
 *               obtained through sec_psm_open().
 *
 */


error_status_t sec_psm_close(
   sec_psm_handle_t psm_handle
) {

     return error_status_ok;
 
}

/* sec_psm_gen_pub_key
 *
 *
 * Generate public key according to key type.  The routine
 * allocates memeory for the data returned in the key
 * parameters.  Users should call sec_pk_data_free() to
 * deallocate that memory.
 *
 * Currently it only supports RSA_PKCS's public key.
 *
 * Return status
 *   error_status_ok:    Success.
 *   other (non-zero):   sec_psm_wrong_pub_key_type 
 *                       sec_s_bsafe_alloc
 *                       sec_s_bsafe_mod_size_out_of_range
 *
 * Input
 *
 *    key_type: Only RSA_PKCS is supported.
 *    modulus_bit_size: Desired key-length.  The length of the desired key.
 *                 	Interpretation of this parameter is algorithm-dependent;
 *                 	For RSA, it shall be interpreted as the bit-length of
 *                 	the key. 
 *
 * Output
 *
 *    private_key:  Pointer to the encoded private key structure of the
 *                  newly-generated key.  
 *    public_key: Pointer to the encoded public key structure of the
 *                 newly-generated key.
 *
 */

error_status_t sec_psm_gen_pub_key(
    sec_pk_algorithm_id_t   *key_type,        /* [in] */
    unsigned32              modulus_bit_size, /* [in] */
    sec_pk_gen_data_t       *seed,            /* [in], maybe internal data */
    sec_pk_data_t           *public_key,     /* [out] */
    sec_pk_data_t           *private_key     /* [out] */
){

    CREATE_PK_DESCRIPTOR(public_key);
    CREATE_PK_DESCRIPTOR(private_key);
    
    return error_status_ok;

}


/* sec_psm_sign_data
 * 
 * Compute the signature of the input data using the
 * signature algorithm specified in the arguments.
 *
 * The routine allocates memory for the data returned in 
 * the signature parameter.  Users should call 
 * sec_pk_data_free() to deallocate that memory.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *          		sec_psm_invalid_handle
 *                      sec_psm_unsupported_algorithm_id
 *	  		sec_pvtkey_invalid_handle
 *                      sec_pvtkey_mechanism_not_init
 *                      sec_pvtkey_internal_error
 *                      sec_pvtkey_invalid_password
 *                      sec_pvtkey_multiple_key_usages
 *
 *
 * Input
 *   
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be 
 *               obtained through sec_psm_open().
 *   signature_alg_id: The ASN.1 DER-encoded object ID of the signature
 *                  algorithm, such as MD5WithRSAEncryption.
 *   key_usage: The usage of the private key which should be
 *              picked for this operation.
 *   data: Pointer to the ASN.1 DER-encoded data to be signed.
 *
 * Output
 *   
 *   signature: Pointer to a signature buffer pointer.
 *
 */


error_status_t sec_psm_sign_data(
    sec_psm_handle_t              psm_handle,
    sec_pk_algorithm_id_t         *signature_alg_id,
    sec_pk_usage_flags_t          key_usage,
    sec_pk_data_t                 *data,
    unsigned32                    *kvno,
    sec_pk_signed_t               *signature
)
{
    error_status_t   st;

    CLEAR_STATUS(&st);

    sec_pk_data_copy(data, signature);

    return error_status_ok;

}

/* sec_psm_verify_data
 * 
 * Verify the data.  Usually, you verify other people's signature.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *          		sec_psm_invalid_handle
 *                      sec_psm_unsupported_algorithm_id
 *          		sec_s_bsafe_decryption_failure (invalid key)
 *	  		sec_pk_e_domain_unsupported,
 *           		sec_pk_e_usage_unsupported,
 *           		sec_rgy_object_not_found,
 *           		sec_rgy_not_authorized,
 *           		sec_attr_unauthorized
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be
 *               obtained through sec_psm_open().
 *   signee_name: Pointer to the name of the principalhad singed
 *                the data.
 *   signature_alg_id: The ASN.1 DER-encoded object ID of the signature
 *                  algorithm, such as MD5WithRSAEncryption.
 *   key_usage: The usage of the ;public key which should be
 *              picked for this operation.
 *   data: Pointer to the data to be verified
 *   signature: Pointer to the signature to be verified.
 *
 */

error_status_t sec_psm_verify_data(
    sec_psm_handle_t        psm_handle,
    sec_pk_domain_t         *signer_domain,
    void                    *signer_name,
    unsigned32              kvno,
    sec_pk_algorithm_id_t   *signature_alg_id,
    sec_pk_usage_flags_t    key_usage,
    sec_pk_data_t           *data,
    sec_pk_signed_t         *signature
){

    return error_status_ok;

}

/* sec_psm_encrypt_data
 * 
 * Encrypt the data in the algorithm specified.  The routine
 * allocates memory for the data returned in the cipher_data
 * parameter.  Users should call sec_pk_data_free() to 
 * deallocate that memory.
 *
 * For reference implementation, only keyEncipherment key_usage
 * will be implemented.
 *
 * Usually you encrypt the data with other's public key.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *          		sec_psm_invalid_handle
 *                      sec_psm_unsupported_algorithm_id
 *	  		sec_pk_e_domain_unsupported,
 *           		sec_pk_e_usage_unsupported,
 *           		sec_rgy_object_not_found,
 *           		sec_rgy_not_authorized,
 *           		sec_attr_unauthorized
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be
 *               obtained through sec_psm_open().
 *   encryptee_name: Pointer to the name of the principal this data is
 *                   encrypted for.
 *   encryption_alg_id: The ASN.1 DER-encoded object ID of encryption
 *                   algorithm, such as RSA.
 *   key_usage: The usage of the public key this key pair 
 *              belongs to.
 *   clear_data: Pointer to the ASN.1 DER-encoded data to be encrypted.
 *
 * Output
 *
 *   cipher_data: Pointer to the encrypted output buffer.
 */

error_status_t sec_psm_encrypt_data(
    sec_psm_handle_t              psm_handle,
    sec_pk_domain_t               *encryptee_domain,
    void                          *encryptee_name,
    unsigned32                    *kvno,
    sec_pk_algorithm_id_t         *encryption_alg_id,
    sec_pk_usage_flags_t          key_usage,
    sec_pk_data_t                 *clear_data,
    sec_pk_data_t                 *cipher_data
)
{

    sec_pk_data_copy(clear_data, cipher_data);
   
    return error_status_ok;

}

/* sec_psm_decrypt_data
 * 
 * Decrypt the mechanism-specific encrypted data. The routine
 * allocates memory for the data returned in the clear_data
 * parameter.  Users should call sec_pk_data_free() to 
 * deallocate that memory. 
 *
 * For reference implementation, only keyEncipherment key_usage
 * will be implemented.
 * 
 * Usually you decrypted the data with your own private key.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *          		sec_psm_invalid_handle
 *                      sec_psm_unsupported_algorithm_id
 *	  		sec_s_bsafe_encryption_failure
 *	  		sec_pvtkey_invalid_handle
 *                      sec_pvtkey_mechanism_not_init
 *                      sec_pvtkey_internal_error
 *                      sec_pvtkey_invalid_password
 *                      sec_pvtkey_multiple_key_usages
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be
 *               obtained through sec_psm_open().
 *   encryption_alg_id: The ASN.1 DER-encoded object ID of encryption
 *                   algorithm, such as RSA.
 *   key_usage: The usage of the private key which should be
 *              picked for this operation.
 *   cipher_data: Pointer to encrypted cipher buffer.
 *
 *
 * Output
 *
 *   clear_data: Pointer to decrypted clear text buffer pointer.
 *
 */

error_status_t sec_psm_decrypt_data(
    sec_psm_handle_t              psm_handle,
    unsigned32                    kvno,
    sec_pk_algorithm_id_t         *encryption_alg_id,
    sec_pk_usage_flags_t          key_usage,
    sec_pk_data_t                 *cipher_data,
    sec_pk_data_t                 *clear_data
)
{

    sec_pk_data_copy(cipher_data, clear_data);	

    return error_status_ok;

}








/* sec_psm_put_pub_key
 * 
 * Store the public key pair with associated data into the personal 
 * security mechanism.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *         		sec_psm_invalid_handle
 *	  		sec_pk_e_domain_unsupported,
 *           		sec_pk_e_usage_unsupported,
 *           		sec_rgy_object_not_found,
 *           		sec_rgy_not_authorized,
 *           		sec_attr_unauthorized
 *    			sec_pvtkey_invalid_handle
 *                      sec_pvtkey_mechanism_not_init
 *                      sec_pvtkey_no_more_memory
 *                      sec_pvtkey_internal_error
 *                      sec_pvtkey_same_domain_and_usage_key_already_exists.
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be
 *               obtained through sec_psm_open().
 *   pwd:  Pointer to the user's password.
 *   key_usage: The usage of the public key this key pair 
 *              belongs to.
 *   pvtkey: Pointer to the ASN.1 DER-encoded private key buffer.
 *   pubkey: Pointer to the ASN.1 DER-encoded public key buffer.
 *
 */

error_status_t sec_psm_put_pub_key(
    sec_psm_handle_t       psm_handle,
    idl_char               *pwd,
    sec_pk_usage_flags_t   key_usage,
    sec_pk_pubkey_t        *public_key,
    sec_pk_pvtkey_t        *private_key
) {

    return error_status_ok;

}

/* sec_psm_update_pub_key
 * 
 * Update the user's own public key pair or passphrase. The oldpassphrase is 
 * specified to authenticate the user updating the key.
 *
 * In the reference implementation, only a single version of a key
 * with a given key usage will be maintained.  Any old key version
 * will be overwritten.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_psm_not_init
 *          		sec_psm_invalid_handle
 *	  		sec_pk_e_domain_unsupported,
 *           		sec_pk_e_usage_unsupported,
 *           		sec_rgy_object_not_found,
 *           		sec_rgy_not_authorized,
 *           		sec_attr_unauthorized
 *     			sec_pvtkey_invalid_handle
 *			sec_pvtkey_mechanism_not_init
 *			sec_pvtkey_private_key_is_not_supplied
 *			sec_pvtkey_new_password_required
 *			sec_pvtkey_no_more_memory
 *			sec_pvtkey_internal_error
 *			sec_pvtkey_no_matched_private_key
 *                      sec_pvtkey_Invalid_password.
 *
 * Input
 *
 *   psm_handle: Pointer to an opaque handle to the personal 
 *               security context data.  This handle should be
 *               obtained through sec_psm_open().
 *   oldpwd:  Pointer to the user's current password.
 *   newpwd:  Pointer to the user's new password.
 *   key_usage: The usage of the public key this key pair 
 *              belongs to.
 *   pvtkey: Pointer to the ASN.1 DER-encoded private key buffer.
 *   pubkey: Pointer to the ASN.1 DER-encoded public key buffer.
 *
 */

error_status_t sec_psm_update_pub_key(
    sec_psm_handle_t       psm_handle,
    idl_char               *oldpwd,
    idl_char               *newpwd,
    sec_pk_usage_flags_t   key_usage,
    sec_pk_pubkey_t        *public_key,
    sec_pk_pvtkey_t        *private_key
){

    return error_status_ok;

}


/* sec_pvtkey__era_fetch_bypass
 *
 * Called by secd to install a "bypass" routine to look directly in the
 * local database instead of doing an RPC.
 *
 * Since this version doesn't actually do private key fetches, this can
 * be a placebo.
 *
 * Input
 *
 *   cell: cell name for which to install bypass
 *   get:  function to call in place of doing remote fetch for that cell.
 */

void sec_pvtkey__era_fetch_bypass(unsigned char *cell,
				  sec_pvtkey_get_bypass_fn_t get)
{
    /* do nothing */
}
