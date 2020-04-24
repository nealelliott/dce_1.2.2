/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: pkc_crypto_rsa.h,v $
 * Revision 1.1.2.1  1996/10/03  20:28:39  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:42:20  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:26 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:10 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */


#ifndef _PKC_CRYPTO_BASIC_
#define _PKC_CRYPTO_BASIC_

#ifdef __cplusplus
extern "C" {
#endif
#include "global.h"
#include "bsafe2.h"
#include "demochos.h"
#ifdef __cplusplus
}

#include <dce/dce.h>

#endif
#ifdef __cplusplus
extern "C" {
#endif

/*
 * p k c _ c r y p t o _ r s a _ e n c o d e _ p u b k e y 
 *
 * Perform an RSA encryption under a public-key
 * The encrypted data is formatted according to PKCS#1, block-type 02.
 * The data to be encrypted must be no larger than the key modulus length,
 * less eleven bytes.
 */
error_status_t pkc_crypto_rsa_encode_pubkey(
                        unsigned char * pubkey,
                        size_t pubkey_length,
                        unsigned char * data,
                        size_t data_length,
                        unsigned char ** res,
                        size_t * res_length);


/*
 * p k c _ c r y p t o _ r s a _ e n c o d e _ p r i v k e y
 *
 * Perform an RSA encryption under a private-key.
 * The encrypted data is formatted according to PKCS#1, block-type 01.
 * The data to be encrypted must be no larger than the key modulus length,
 * less eleven bytes.
 *
 */
error_status_t pkc_crypto_rsa_encode_privkey(
                        unsigned char * privkey,
                        size_t privkey_length,
                        unsigned char * data,
                        size_t data_length,
                        unsigned char ** res,
                        size_t * res_length);

/*
 * p k c _ c r y p t o _ r s a _ d e c o d e _ p u b k e y 
 *
 * Perform an RSA decryption using a public-key.  
 * Input data should be a PKCS#1 block, type 01.
 *
 */
error_status_t pkc_crypto_rsa_decode_pubkey(
                        unsigned char * pubkey,
                        size_t pubkey_length,
                        unsigned char * data,
                        size_t data_length,
                        unsigned char ** res,
                        size_t * res_length);


/*
 * p k c _ c r y p t o _ r s a _ d e c o d e _ p r i v k e y 
 *
 * Perform an RSA decryption using a private-key.
 * Input data should be a PKCS#1 block, type 02.
 *
 */
error_status_t crypto_rsa_decode_privkey(
                        unsigned char * privkey,
                        size_t privkey_length,
                        unsigned char * data,
                        size_t data_length,
                        unsigned char ** res,
                        size_t * res_length);


/*
 * p k c _ c r y p t o _ r s a _ g e n e r a t e _ k e y p a i r
 *
 * Generate an RSA keypair of the specified modulus length.
 */
error_status_t crypto_rsa_generate_keypair(
                        unsigned32 modulus_bits,
                        unsigned char ** privkey,
                        size_t * privkey_length,
                        unsigned char ** pubkey,
                        size_t * pubkey_length);



/*
 * p k c _ c r y p t o _ c a l c u l a t e _ h a s h _ m d 2 _ i n i t
 *
 * Initialize processing for an MD2 hash calculation.  See
 * routine pkc_crypto_calculate_hash_md2 for information on how to
 * call the pkc_crypto_calculate_hash_md2_init/update/final routines.
 */
error_status_t pkc_crypto_calculate_hash_md2_init(
			void ** hash_context);

/*
 * p k c _ c r y p t o _ c a l c u l a t e _ h a s h _ m d 2 _ u p d a t e
 *
 * Update an MD2 hash calculation.  See routine 
 * pkc_crypto_calculate_hash_md2 for information on how to
 * call the pkc_crypto_calculate_hash_md2_init/update/final routines.
 */
error_status_t pkc_crypto_calculate_hash_md2_update(
			void ** hash_context,
			unsigned char * data,
			size_t data_length);

/*
 * p k c _ c r y p t o _ c a l c u l a t e _ h a s h _ m d 2 _ f i n a l
 *
 * Finish an MD2 hash calculation.  See routine 
 * pkc_crypto_calculate_hash_md2 for information on how to
 * call the pkc_crypto_calculate_hash_md2_init/update/final routines.
 */
error_status_t pkc_crypto_calculate_hash_md2_final(
			void ** hash_context,
			unsigned char hash[16]);

/*
 * p k c _ c r y p t o _ c a l c u l a t e _ h a s h _ m d 2
 *
 * Calculate an MD2 hash.
 * This routine can be used to calculate a hash over a single
 * contiguous byte-array; if multiple data values are to be hashed
 * together, you must use the lower-level 
 * pkc_crypto_calculate_hash_md2_init/update/final routines.
 */
#ifdef __cplusplus
inline 
#endif
error_status_t pkc_crypto_calculate_hash_md2(
			unsigned char * data,
			size_t data_length,
			unsigned char hash[16]) {
    void * hash_context;
    error_status_t status;

    if (status = pkc_crypto_calculate_hash_md2_init(&hash_context)) 
	return status;
    if (status = pkc_crypto_calculate_hash_md2_update(&hash_context,
						      data,
						      data_length)) 
	return status;
    return pkc_crypto_calculate_hash_md2_final(&hash_context, hash);

}




/*
 * p k c _ c r y p t o _ c a l c u l a t e _ h a s h _ m d 5 _ i n i t
 *
 * Initialize processing for an MD5 hash calculation.  See
 * routine pkc_crypto_calculate_hash_md5 for information on how to
 * call the pkc_crypto_calculate_hash_md5_init/update/final routines.
 */
error_status_t pkc_crypto_calculate_hash_md5_init(
			void ** hash_context);

/*
 * p k c _ c r y p t o _ c a l c u l a t e _ h a s h _ m d 5 _ u p d a t e
 *
 * Update an MD5 hash calculation.  See routine 
 * pkc_crypto_calculate_hash_md5 for information on how to
 * call the pkc_crypto_calculate_hash_md5_init/update/final routines.
 */
error_status_t pkc_crypto_calculate_hash_md5_update(
			void ** hash_context,
			unsigned char * data,
			size_t data_length);

/*
 * p k c _ c r y p t o _ c a l c u l a t e _ h a s h _ m d 5 _ f i n a l
 *
 * Finish an MD5 hash calculation.  See routine 
 * pkc_crypto_calculate_hash_md5 for information on how to
 * call the pkc_crypto_calculate_hash_md5_init/update/final routines.
 */
error_status_t pkc_crypto_calculate_hash_md5_final(
			void ** hash_context,
			unsigned char hash[16]);

/*
 * p k c _ c r y p t o _ c a l c u l a t e _ h a s h _ m d 5
 *
 * Calculate an MD5 hash.
 * This routine can be used to calculate a hash over a single
 * contiguous byte-array; if multiple data values are to be hashed
 * together, you must use the lower-level 
 * pkc_crypto_calculate_hash_md5_init/update/final routines.
 */
#ifdef __cplusplus
inline 
#endif
error_status_t pkc_crypto_calculate_hash_md5(
			unsigned char * data,
			size_t data_length,
			unsigned char hash[16]) {
    void * hash_context;
    error_status_t status;

    if (status = pkc_crypto_calculate_hash_md5_init(&hash_context)) 
	return status;
    if (status = pkc_crypto_calculate_hash_md5_update(&hash_context,
                                           data,
					   data_length)) return status;
    return pkc_crypto_calculate_hash_md5_final(&hash_context, hash);

}




/*
 * p k c _ c r y p t o _ r s a _ m d 2 _ s i g n
 *
 * Perform an RSAwithMD2 signature.
 *
 */
error_status_t crypto_rsa_md2_sign(
                        unsigned char * privkey,
                        size_t privkey_length,
                        unsigned char * data,
                        size_t data_length,
                        unsigned char ** res,
                        size_t * res_length);

/*
 * p k c _ c r y p t o _ r s a _ m d 2 _ v e r i f y
 *
 * Verify an RSAwithMD2 signature.
 *
 */
error_status_t crypto_rsa_md2_verify(
                        unsigned char * pubkey,
                        size_t pubkey_length,
                        unsigned char * data,
                        size_t data_length,
			unsigned char * signature,
			size_t signature_length,
                        int * signature_ok);




/*
 * p k c _ c r y p t o _ r s a _ m d 5 _ s i g n
 *
 * Perform an RSAwithMD5 signature.
 *
 */
error_status_t crypto_rsa_md5_sign(
                        unsigned char * privkey,
                        size_t privkey_length,
                        unsigned char * data,
                        size_t data_length,
                        unsigned char ** res,
                        size_t * res_length);

/*
 * p k c _ c r y p t o _ r s a _ m d 5 _ v e r i f y
 *
 * Verify an RSAwithMD5 signature.
 *
 */
error_status_t crypto_rsa_md5_verify(
                        unsigned char * pubkey,
                        size_t pubkey_length,
                        unsigned char * data,
                        size_t data_length,
			unsigned char * signature,
			size_t signature_length,
                        int * signature_ok);



/*
 * p k c _ c r y p t o _ d i f f i e _ h e l l m a n _ p h a s e _ 1
 *
 * Generate public D-H value to send to peer.
 *
 * d_h_parameters should contain a BER-encoded algorithmIdentifier that
 * specifies Diffie-Hellman, and contains the public prime and base values
 * in its parameter field, as specified in PKCS#3.
 */
error_status_t pkc_crypto_diffie_hellman_phase_1 (
			void ** dh_context,
			unsigned char * dh_parameters,
			size_t dh_parameter_length,
			unsigned char ** public_value,
			size_t * public_value_length);

/*
 * p k c _ c r y p t o _ d i f f i e _ h e l l m a n _ p h a s e _ 2
 *
 * Process public value received from peer and generate key value.
 * Caller specifies how many bytes of key material are needed;
 * Routine calculates key value as follows:
 *   If key_bytes_required <= 16:
 *      calculate MD5 hash over entire Diffie-Hellman shared secret, 
 *      starting with most significant byte.  Return most 
 *      significant key_bytes_required bytes of resultant 128-bit value.
 *   If key_bytes_required > 16:
 *      Return the key_bytes_required least significant bytes
 *      of the Diffie-Hellman shared secret.
 *      
 * Note: Unlike most other pkc_crypto_ routines, the caller is responsible
 * for allocating a buffer to receive the returned key.
 */
error_status_t pkc_crypto_diffie_hellman_phase_2 (
			void ** dh_context,
			unsigned char * peer_public_value,
			size_t peer_public_value_length,
			size_t key_bytes_required,
			unsigned char * key_buffer);


#ifdef __cplusplus
}
#endif

#endif
