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
 * $Log: sec_bsafe.h,v $
 * Revision 1.1.2.1  1996/07/09  14:03:32  arvind
 * 	OSF DCE 1.2.2 Drop 3
 * 	sec_bsafe.h,v
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 	[1996/06/27  19:54 UTC  cuti  /main/DCE122_PK/cuti_pk_export/3]
 *
 * 	Merge out from DCE122
 *
 * 	Change modulus_bit_size to in, not in/out
 * 	[1996/06/27  13:58 UTC  cuti  /main/DCE122_PK/cuti_pk_export/2]
 *
 * 	Merge out from cuti_pk_new to cuti_pk_export
 *
 * 	Remove bsafe include files
 * 	[1996/06/14  15:03 UTC  cuti  /main/DCE12_PK/cuti_pk_new/2]
 *
 * 	Incorporate John Wray's new routines
 * 	[1996/05/23  16:09 UTC  cuti  /main/DCE12_PK/cuti_pk_new/1]
 *
 * 	remove double indirection.
 * 	[1996/06/11  18:55 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 *
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:54 UTC  psn  /main/DCE122_PK/1]
 *
 * 	Merge PK work from DCE12_PK off of DCE_1.2 to DCE122_PK off of DCE_1.2.2.
 * 	[1996/05/28  17:59 UTC  cuti  /main/DCE12_PK/2]
 *
 * 	Merge psm, pvtkey work
 *
 * 	Incorporate John Wray's new routines
 * 	[1996/05/23  16:09 UTC  cuti  /main/DCE12_PK/cuti_pk_new/1]
 *
 * 	Move sec_bsafe.h from ../psm to ./h.
 * 	[1996/05/08  13:45 UTC  cuti  /main/DCE12_PK/1]
 *
 * 	Prepare for review of pvtkey.
 * 	[1996/05/08  13:45 UTC  cuti  /main/DCE12_PK/1]
 *
 * $EndLog$
 */

#ifndef SEC_BSAFE_H_
#define SEC_BSAFE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <dce/dce.h>
#include <dce/sec_pk_base.h>
#include <dce/dcepsmmsg.h>




/* Algorithm type defined by BSAFE */

typedef unsigned32 sec_bsafe_alg_type_t;

/*  hash algorithm */
#define sec_bsafe_hash_MD2       1  /* AI_MD2 */
#define sec_bsafe_hash_MD5       2  /* AI_MD5 */
#define sec_SHA_1                3  /* Even though RSADSI recommended using 
                                       SHA-1 over MD5 for internet PKI, but 
                                       it's not supported in BSAFE 2.1 */

/* signature algorith */

#define sec_sign_md2rsa         11  /* AI_MD2WithRSAEncryption */
#define sec_sign_md5rsa         12  /* AI_MD5WithRSAEncryption */

/* RSA encryption algorithm */

#define  sec_enc_rsa_priv       21  /* AI_PKCS_RSAPrivate */
#define  sec_enc_rsa_pub        22  /* AI_PKCS_RSAPublic */

/* DES encryption algorithm */


/* AI_RFC1113Recode the binary to printable recoding alg */

/* Password-based encryption algorithm */
#define sec_pwdb_md2despad      41  /* AI_MD2WithDES_CBCPad */
#define sec_pwdb_md2despadber   42  /* AI_MD2WithDES_CBCPadBER */
#define sec_pwdb_md5despad      43  /* AI_MD5WithDES_CBCPad */
#define sec_pwdb_md5despadber   44  /* AI_MD5WithDES_CBCPadBER */


/* Symmetric algorithms */
#define sec_bsafe_enc_des       51  /* AI_DES_CBCPadIV8 */
#define sec_bsafe_enc_des_PEM   52  /* AI_DES_CBCPadPEM */
#define sec_bsafe_enc_des_nopad 55  /* AI_DES_CBC_IV8 */

#define sec_bsafe_enc_des_ede3  61  /* AI_DES_EDE3_CBCPadIV8 */
#define sec_bsafe_enc_des_ede3_nopad    65  /* AI_DES_EDE3_CBC_IV8 */



/* DCE1.2.2's designated RSA algorithm types */

#define SEC_PK_SALT_CALCULATE_ALG        sec_bsafe_hash_MD5 
#define SEC_PK_PWD_BASED_ENCRYPTION_ALG  sec_pwdb_md5despad
#define SEC_PK_PWD_BASED_DECRYPTION_ALG  sec_pwdb_md5despadber


#define SEC_PK_HASH                      sec_bsafe_hash_MD5 /* default hash */
#define SEC_PK_PVT_ENCRYPTION            sec_enc_rsa_priv /* default pvtkey enc */
#define SEC_PK_PUB_ENCRYPTION            sec_enc_rsa_pub /* default pubkey enc */




/* 
 * sec_bsafe_gen_pub_key
 * 
 * Generate ASN.1 DER-encoded public key pairs with seed value provided for 
 * random number generator.
 *
 * The routine allocates memory for the returned key pair.  Users should 
 * call sec_pk_data_free() or sec_pk_data_zero_and_free() to free the data.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *
 * Input
 *
 *   modulus_bit_size: Size of modulus in bits.  The default value is 1024.
 *   seed: Seed data for random number generator.
 *
 * Output
 *
 *   pubkey:  Pointer to public key buffer pointer of the key pair. 
 *   pvtkey:  Pointer to private key buffer pointer of the key pair.
 *
 */

error_status_t sec_bsafe_gen_pub_key(
    unsigned int            modulus_bit_size, /* [in] */
    sec_pk_gen_data_t       *seed,            /* [in], maybe internal data */ 
    sec_pk_data_t           *pubkey,          /* [out] */
    sec_pk_data_t           *pvtkey           /* [out] */
);

/*
 * sec_bsafe_encrypt_pvtkey
 * 
 * Generates password-based encrypted private key which uses password 
 * generated DES key to encrypt private key.  It computes the hash value 
 * based on the name and public key part of the key pair for salt value. 
 * It uses RSA algorithm AI_MD2WithRC2_CBCPad or AI_MD5WithRC2_CBCPad to
 * turn password(passphrase) into a DES key (which is n time hashed value of
 * the password), then encrypt the private key with this DES key.
 * 
 * The routine allocates memory for the returned key and pbe_params.  Users
 * should call sec_pk_data_free() or sec_pk_data_zero_and_free() to free 
 * the data.
 * 
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *
 * Input
 *   name: Pointer to the user's canonical name within the specified domain.
 *   pwd: Pointer to the user's password.
 *   pvtkey: Pointer to the encoded private key 
 *                 structure.
 *
 * Output
 *   pbe_params: Pointer to encoded password-based encryption parameters,
 *               such as salt (for DES encryption) and iteration count (for
 *               generating DES key).  This value should be stored with 
 *               encrypted private key, so it can be retrived to
 *               decrypt the private key.
 *   encrypted_pvtkey: Pointer to DER encoded passwd-based encrypted 
 *                     private key.
 *   
 *
 */
 
error_status_t sec_bsafe_encrypt_pvtkey(
    sec_pk_gen_data_t       *pwd,              /* [in] */ 
    sec_pk_data_t           *pvtkey,           /* [in] */
    sec_pk_data_t           *pbe_params,       /* [out] */ 
    sec_pk_data_t           *encrypted_pvtkey  /* [out] */   
);

/*
 * sec_bsafe_decrypt_pvtkey
 * 
 * Generate the raw private key from password-based encrypted private key.
 * 
 * The routine allocates memory for the returned key.  Users
 * should call sec_pk_data_zero_and_free() to free the data.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *
 * Input
 *   name: Pointer to the user's canonical name within the specified domain.
 *   pwd: Pointer to the user's password.
 *   pvtkey: Pointer to the encoded password-based encrypted private key 
 *                 structure.
 *   pbe_params: Pointer to encoded password-based encryption parameters
 *               structure, such as salt (for DES encryption) and iteration
 *               count (for generating DES key). This value is used to 
 *               decrypt the private key.
 *
 * Output
 *
 *   decrypted_pvtkey: Pointer to DER encoded private key buffer.
 *
 */

error_status_t sec_bsafe_decrypt_pvtkey(
    sec_pk_gen_data_t       *pwd,              /* [in] */ 
    sec_pk_data_t           *pvtkey,           /* [in] */
    sec_pk_data_t           *pbe_params,       /* [in] */ 
    sec_pk_data_t           *decrypted_pvtkey  /* [out] */
);

/*
 * sec_bsafe_sign_data
 * 
 * Generate the signature of the data signed with the user's private key.
 * 
 * The routine allocates memory for the returned signature.  Users
 * should call sec_pk_data_free() to free the data.
 * 
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_s_algorithm_unknown
 *
 * Input
 *   signature_alg: The sigature algorithm type,  sec_sign_md2rsa or 
 *               sec_sign_md5rsa.
 *   data: Pointer to the data to be signed.
 *   pvtkey: Pointer to the user's DER encoded private key structure which
 *           is used to encrypt the hash value of the data.
 *
 * Output
 *   signature:  Pointer to DER encoded signature buffer.
 *
 */


error_status_t sec_bsafe_sign_data(
    sec_bsafe_alg_type_t    signature_alg,   /* [in] */
    sec_pk_gen_data_t       *data,             /* [in] */
    sec_pk_data_t           *pvtkey,           /* [in] */
    sec_pk_data_t           *signature          /* [out] */
);

/*
 * sec_bsafe_verify_data
 *
 * Verify the signature of the data which was signed with the user's
 * private key.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):  sec_s_algorithm_unknown
 *
 * Input
 *   verify_alg: The sigature algorithm type,  sec_sign_md2rsa or 
 *               sec_sign_md5rsa.
 *   data: Pointer to data to be verified.
 *   pubkey: Pointer to the user's DER encoded public key strucuture which
 *           is used to decrypt the signature.
 *   signature: Pointer to the DER encoded signature structure.
 *
 */

error_status_t sec_bsafe_verify_data(
    sec_bsafe_alg_type_t    signature_alg,   /* [in] */
    sec_pk_gen_data_t       *data,                /* [in] */
    sec_pk_data_t           *pubkey,              /* [in] */
    sec_pk_data_t           *signature            /* [in] */
);

/*
 * sec_bsafe_encrypt_data
 * 
 * Generate RSA public key encrypted cipher data.
 * 
 * The routine allocates memory for the returned cipher_text.  Users
 * should call sec_pk_data_free() to free the data.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *
 * Input
 *
 *   encrypt_alg: The RSA public key encryption algorithm type, 
 *                sec_enc_rsa_pub.
 *   seed: Seed data for random number generator.
 *   pubkey: Pointer to the user's DER encoded public key strucuture which
 *           is used to encrypt the plain text.
 *   plain_text: Pointer to the data structure to be public key encrypted.
 *
 * Output
 *
 *   cipher_text: Pointer to the encrypted data structure.  
 *
 */

error_status_t  sec_bsafe_encrypt_data(
    sec_bsafe_alg_type_t    encrypt_alg,  /* [in] */
    sec_pk_gen_data_t       *seed,             /* [in] */
    sec_pk_data_t           *pubkey,           /* [in] */
    sec_pk_gen_data_t       *plain_text,       /* [in] */
    sec_pk_gen_data_t       *cipher_text       /* [out] */
);

/*
 * sec_bsafe_decrypt_data
 *
 * Decrypt the public key encrypted cipher data.
 * 
 * The routine allocates memory for the returned plain_text.  Users
 * should call sec_pk_data_free() to free the data.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *
 * Input
 *   decrypt_alg: The RSA public key decryption algorithm type, 
 *                sec_enc_rsa_priv.
 *   pvtkey: Pointer to the user's DER encoded private key structure which
 *           is used to decrypt the cipher data.
 *   cipher_text: Pointer to the public key encrypted data structure. 
 *
 * Output
 *
 *   plain_text: Pointer to the decrytped plain text. 
 *
 */

error_status_t sec_bsafe_decrypt_data(
    sec_bsafe_alg_type_t  decrypt_alg,  /* [in] */    
    sec_pk_data_t    *pvtkey,           /* [in] */
    sec_pk_gen_data_t       *cipher_text,      /* [in] */
    sec_pk_gen_data_t       *plain_text        /* [out] */
);

/*
 * sec_bsafe_hash_init
 * 
 * Prepare for a hash computation
 *
 * The routine allocates memory for the returned hash-context.  Passing this
 * context to sec_bsafe_hash_final (with any number of intervening
 * calls to to sec_bsafe_hash_update) will free the hash context.  The
 * context is also released in the event of an error.
 *
 * Please note BSAFE 2.1 doesn't support SHA-1, althorugh it is 
 * suggested by PKIX for standard hash algorithm.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *
 * Input
 *   
 *   digest_alg: The RSA supplied hash (digest) algorithm type, 
 *                sec_bsafe_hash_MD2 or_bsafe_hash_MD5.
 * Output
 *
 *   hash_context : Pointer to a caller-provided (void *) object
 *                   where context information will be stored.
 *
 *
 */

error_status_t sec_bsafe_hash_init(
    sec_bsafe_alg_type_t digest_alg,   /* [in] */
    void                 ** hash_context
);

/*
 * sec_bsafe_hash_update
 * 
 * Add data into an ongoing hash computation
 *
 * The routine uses a hash-context that must have been previously
 * initalized by a call to sec_bsafe_hash_init.  Any error condition
 * will result in the release of any memory associated with this hash
 * context.  Passing this context to sec_bsafe_hash_final (with any 
 * number of intervening calls to to sec_bsafe_hash_update) will also
 * free the hash context.  
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *
 * Input
 *   
 *   hash_context : Pointer to (void *) object where context information 
 *                  was stored by sec_bsafe_hash_init.
 *
 *   data : the data to be included in the hash.
 *
 */

error_status_t sec_bsafe_hash_update(
    void              ** hash_context,
    sec_pk_gen_data_t * data
);

/*
 * sec_bsafe_hash_final
 * 
 * Complete a hash computation
 *
 * The routine uses a hash-context that must have been previously
 * initalized by a call to sec_bsafe_hash_init and delivers the hash
 * of all data provided to intervening calls to sec_bsafe_hash_update.  
 * The routine also frees any memory associated with the hash context.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *
 * Input
 *   
 *   hash_context : Pointer to (void *) object where context information 
 *                  was stored by sec_bsafe_hash_init.
 *
 *   data : the data to be included in the hash.
 *
 */

error_status_t sec_bsafe_hash_final(
    void              ** hash_context,
    sec_pk_gen_data_t * hash
);

/*
 * sec_bsafe_compute_hash
 * 
 * Generate the hash value of the data.
 *
 * The routine allocates memory for the returned hash value.  Users
 * should call sec_pk_data_free() to free the data.
 *
 * Please note BSAFE 2.1 doesn't support SHA-1, althorugh it is 
 * suggested by PKIX for standard hash algorithm.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *
 * Input
 *   
 *   digest_alg: The RSA supplied hash (digest) algorithm type, 
 *                sec_bsafe_hash_MD2 or_bsafe_hash_MD5.
 *   data: Pointer to the data to be computed.
 *
 * Output
 *
 *   digest: Pointer to the encoded digest data structure.
 *
 */

error_status_t sec_bsafe_compute_hash(
    sec_bsafe_alg_type_t         digest_alg,    /* [in] */
    sec_pk_gen_data_t            *data,         /* [in] */
    sec_pk_gen_data_t            *digest        /* [out] */
);

/*
 * sec_bsafe_diffie_hellman_phase_1
 * 
 * Initiate a Diffie-Hellman exchange.  D-H consists of two phases.
 * The first phase, implemented by this routine consists of generating
 * a random number and calculating from it a value tosend to the peer.
 * The second phase (implemented by sec_bsafe_diffie_hellman_phase_2)
 * consists of receiving the peer-generated value and combining it with
 * the locally-generated random number to derive the key value.
 *
 * The routine allocates memory for the context parameter.  This memory is
 * released upon succesful completion of sec_bsafe_diffie_hellmna_phase_2.
 * Any error condition will also release this memory.
 *
 * Callers must have previously generated appropriate Diffie-Hellman
 * parameters (i.e. the shared prime value and the base value).  These values
 * are presented to sec_bsafe_diffie_hellman_phase_1 encoded in BER, using
 * the dh_parameters argument.
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *
 * Input
 *   
 *   dh_parameters: The BER-encoded Diffie-Hellman parameters.
 *
 *   seed : random bytes.
 *
 * Output
 *
 *   context : Pointer to caller-provided (void *) object to receive
 *            a context object.
 *
 *   public_value : The value that must be sent to the peer.
 *
 */

error_status_t sec_bsafe_diffie_hellman_phase_1 (
                        void              ** dh_context,
                        sec_pk_gen_data_t * seed,
                        sec_pk_gen_data_t * dh_parameters,
                        sec_pk_gen_data_t * public_value
);

/*
 * sec_bsafe_diffie_hellman_phase_2
 * 
 * Complete a Diffie-Hellman exchange.  D-H consists of two phases.
 * The first phase (implemented by sec_bsafe_diffie_hellman_phase_1) 
 * consists of generating a random number and calculating from it a 
 * value to send to the peer. The second phase, implemented by this routine,
 * consists of receiving the peer-generated value and combining it with
 * the locally-generated random number to derive the key value.
 *
 * The routine frees any memory associated with the context parameter.  
 *
 * 
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero): 
 *
 * Input
 *   
 *   dh_context : Pointer to caller-provided (void *) object that contains 
 *                a valid context object.
 *
 *   peer_public_value : The public value received from the peer party.
 *
 * Output
 * 
 *   key_buffer : A sec_pk_gen_data_t object describing a caller-allocated 
 *                buffer, whose length field indicates the number of key bytes
 *                required.  The least significant N bytes of the negotiated
 *                Diffie-Hellman key will be copied into this buffer.
 *
 *   
 */

error_status_t sec_bsafe_diffie_hellman_phase_2 (
                        void              ** dh_context,
                        sec_pk_gen_data_t * peer_public_value,
                        sec_pk_gen_data_t * key_buffer
);


/*
 * sec_bsafe_symmetric_alginfo
 *
 * Return information about a symmetric algorithm.  Information includes
 * the length of the key and IV to use with the algorithm as well as the
 * chunksize and maxpad amounts.  The chunksize of an algorithm is the unit
 * of encryption - data must be presented to the encryption algorithm as
 * string of bytes whose length is an integer multiple of the chunksize.
 * Thus, pure stream ciphers and block ciphers that perform internal padding
 * have a chunksize of 1, while block ciphers that don't do padding have
 * chunksize equal to their blocksize.  The maxpad amount for an algorithm
 * is the maximum number of bytes that may be added during the encryption
 * process.  Any parameter may be passed as NULL if the corresponding value
 * is not required.
 *
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):
 *
 * Input
 *
 *   alg : Specifies the symmetric algorithm about which information is
 *         required.
 *
 *
 * Output
 *
 *   key_length : The number of bytes that must be supplied as keying material
 *                for the algorithm.
 *
 *   iv_length : The number of bytes that must be supplied as an initialization
 *               vector for the algorithm.
 *
 *   chunksize : The chunksize of the algorithm
 *
 *   maxpad : The maximum number of bytes that may be added during encryption
 *
 *
 */
 
error_status_t sec_bsafe_symmetric_alginfo (
     sec_bsafe_alg_type_t  alg,          /* [in] */
     unsigned32  *key_length,            /* [out] */
     unsigned32  *iv_length,             /* [out] */
     unsigned32  *chunksize,             /* [out] */
     unsigned32  *maxpad                 /* [out] */
);
  
/*
 * sec_bsafe_symmetric_encrypt
 *
 * Encrypt data using a symmetric algorithm.  This version of symmetric
 * encryption (currently the only one supported) encrypts an entire message
 * in a single call.  It takes an algorithm, a key, an IV, and a block of
 * data to encrypt, and returns a malloced block of data containing the
 * ciphertext.  The required lengths of the key and IV and the chunksize
 * can be determined by calling sec_bsafe_symmetric_alginfo().
 *
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):
 *
 *
 * Input
 *
 *   alg : Specifies the symmetric algorithm to be used.
 *
 *   key : Specifies the key value to be used.
 *
 *   iv : The initialization vector to use.
 *
 *   plaintext : The data to be encrypted.
 *
 *
 * Output
 *
 *   ciphertext : The result of the encryption.  The routine allocates
 *                memory for this object.
 *
 *
 */
 
error_status_t sec_bsafe_symmetric_encrypt (
    sec_bsafe_alg_type_t    alg,          /* [in] */
    sec_pk_gen_data_t       *key,         /* [in] */
    sec_pk_gen_data_t       *iv,          /* [in] */
    sec_pk_gen_data_t       *plaintext,   /* [in] */
    sec_pk_gen_data_t       *ciphertext   /* [out] */
);

/*
 * sec_bsafe_symmetric_decrypt
 *
 * Decrypt data using a symmetric algorithm.  This version of symmetric
 * decryption (currently the only one supported) decrypts an entire message
 * in a single call.  It takes an algorithm, a key, an IV, and a block of
 * data to decrypt, and returns a malloced block of data containing the
 * plaintext.  The required lengths of the key and IV and the chunksize
 * can be determined by calling sec_bsafe_symmetric_alginfo().
 *
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):
 *
 * Input
 *
 *   alg : Specifies the symmetric algorithm to be used.
 *
 *   key : Specifies the key value to be used.
 *
 *   iv : The initialization vector to use.
 *
 *   ciphertext : The data to be decrypted.
 *
 *
 * Output
 *
 *    plaintext : The result of the encryption.  The routine allocates
 *                memory for this object.
 *
 *
 */

error_status_t sec_bsafe_symmetric_decrypt (
    sec_bsafe_alg_type_t    alg,          /* [in] */
    sec_pk_gen_data_t       *key,         /* [in] */
    sec_pk_gen_data_t       *iv,          /* [in] */
    sec_pk_gen_data_t       *ciphertext,  /* [in] */
    sec_pk_gen_data_t       *plaintext    /* [out] */
);

/*
 * sec_bsafe_random_bytes
 *
 * Fill a user-provided buffer with random data.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):
 *
 *
 * Input
 *
 *   seed : seed value for data.
 *
 *
 * Output
 *
 *   buffer : user-allocated buffer to be filled.
 *
 *
 */

error_status_t sec_bsafe_random_bytes (
    sec_pk_gen_data_t       *seed,         /* [in] */
    sec_pk_gen_data_t       *buffer        /* [out] */
);

/*
 * sec_bsafe_randomize
 *
 * Allows an application to supply random data for the crypto layer's use.
 *
 * Return status
 *   error_status_ok:   Success.
 *   other (non-zero):
 *
 *
 * Input
 *
 *   seed : seed value for data.
 *
 *
 * Output
 *
 *   none
 *
 *
 */

error_status_t sec_bsafe_randomize (
    sec_pk_gen_data_t       *seed         /* [in] */
);


#ifdef __cplusplus
}
#endif

#endif
