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
 * $Log: sec_bsafe.c,v $
 * Revision 1.1.2.2  1996/10/03  15:07:28  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:33 UTC  arvind  /main/DCE_1.2.2/arvind_pkss/1]
 *
 * 	OSF DCE1.2.2 Drop3
 * 	sec_bsafe.c,v
 * 	[1996/07/08  20:20:59  arvind  1.1.2.1]
 *
 * 	Prepare for review of pvtkey.
 * 	[1996/05/08  13:44 UTC  cuti  /main/DCE12_PK/1]
 *
 * Revision 1.1.2.1  1996/07/08  20:20:59  arvind
 * 	OSF DCE1.2.2 Drop3
 * 	sec_bsafe.c,v
 * 	Revision /main/DCE_1.2.2/3  1996/06/30  22:00 UTC  cuti
 * 	Merge from cuti_pk_export to DCE_1.2.2
 * 	[1996/07/08  20:04:39  arvind]
 * 
 * Revision /main/DCE122_PK/cuti_pk_export/3  1996/06/27  19:42 UTC  cuti
 * 	Merge out from DCE122
 * 
 * 	Change modulus_bit_size from in/out to in only
 * 	[1996/06/27  14:04 UTC  cuti  /main/DCE122_PK/cuti_pk_export/2]
 * 
 * 	Merge out from cuti_pk_new to cuti_pk_export
 * 
 * 	Check the modular_size
 * 	[1996/06/14  15:10 UTC  cuti  /main/DCE12_PK/cuti_pk_new/5]
 * 
 * 	Change modulus_size to in/out
 * 	[1996/06/04  18:00 UTC  cuti  /main/DCE12_PK/cuti_pk_new/4]
 * 
 * 	Update with review
 * 	[1996/05/24  20:37 UTC  cuti  /main/DCE12_PK/cuti_pk_new/3]
 * 
 * 	Free res in sec_bsafe_decrypt_pvtkey
 * 	[1996/05/23  19:02 UTC  cuti  /main/DCE12_PK/cuti_pk_new/2]
 * 
 * 	Incorporate John Wray's new routines which include mutex lock around random algorithm.
 * 	[1996/05/23  16:13 UTC  cuti  /main/DCE12_PK/cuti_pk_new/1]
 * 	Revision /main/DCE_1.2.2/2  1996/06/13  21:30 UTC  sommerfeld
 * 	remove double indirection.
 * 	[1996/06/11  18:59 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/06/11  18:00 UTC  aha
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 
 * Revision /main/DCE122_PK/1  1996/06/04  18:53 UTC  psn
 * 	DCE12_PK -> DCE122_PK
 * 
 * Revision /main/DCE12_PK/2  1996/05/28  18:00 UTC  cuti
 * 	Merge psm, pvtkey work
 * 
 * 	Update with review
 * 	[1996/05/24  20:37 UTC  cuti  /main/DCE12_PK/cuti_pk_new/3]
 * 
 * 	Free res in sec_bsafe_decrypt_pvtkey
 * 	[1996/05/23  19:02 UTC  cuti  /main/DCE12_PK/cuti_pk_new/2]
 * 
 * 	Incorporate John Wray's new routines which include mutex lock around random algorithm.
 * 	[1996/05/23  16:13 UTC  cuti  /main/DCE12_PK/cuti_pk_new/1]
 * 	Revision /main/DCE12_PK/1  1996/05/08  13:44 UTC  cuti
 * 	Prepare for review of pvtkey.
 * 
 * $EndLog$
 */

#include  <pthread.h>
#include  <time.h>
#include  <string.h>
#include  <bsafe/global.h>
#include  <bsafe/bsafe2.h>
#include  <bsafe/atypes.h>
#include  <sec_bsafe.h>
#include  <sec_pk.h>

#define LOCK_RANDOMALG pthread_mutex_lock(&randalg_mutex)

#define UNLOCK_RANDOMALG pthread_mutex_unlock(&randalg_mutex)

/*******************************************
 *
 * NOT DONE LIST.
 *
 *  Need to have printable form of pub key pair (sec_bsafe_gen_pub_key)
 *
 ********************************************/


#define CREATE_DESCRIPTOR(desc) sec_pk_data_init(desc)

#define CREATE_PK_DESCRIPTOR(desc) sec_pk_data_init(desc)

#define CLEANUP_DESCRIPTOR(desc) sec_pk_data_free(desc)

#define CLEANUP_PK_DESCRIPTOR(desc) sec_pk_data_free(desc)

#define CREATE_STORAGE(desc, size) \
	if (((desc)->data = (unsigned char *)malloc(size)) == NULL) { \
	    status = BE_ALLOC; \
	    break; \
	}; \
        memset((desc)->data, 0, size);

#define CREATE_PK_STORAGE(desc, size) \
	if (((desc)->data = \
		    (unsigned char *)malloc(size)) == NULL) { \
	    status = BE_ALLOC; \
	    break; \
	}; \
        memset((desc)->data, 0, size);

#ifndef CLEAR_STATUS 
#define CLEAR_STATUS(x) *(x)=0
#endif

B_ALGORITHM_METHOD *RSA_ALGORITHM_CHOOSER[] = {
    &AM_DES_CBC_DECRYPT,/* DES DBD decryption for passwd-based enc pvtkey */
    &AM_DES_CBC_ENCRYPT,/* DES DBD decryption for passwd-based enc pvtkey */
    &AM_DES_EDE3_CBC_ENCRYPT,   /* 3xDES */
    &AM_DES_EDE3_CBC_DECRYPT,   /* 3xDES */
    &AM_MD2,            /* RSA MD2 digest */
    &AM_MD2_RANDOM,     /* RSA MD2 random number generator (RNG) */
    &AM_MD5,            /* RSA MD5 digest */
    &AM_MD5_RANDOM,     /* RSA MD5 random number generator (RNG) */
    &AM_RSA_CRT_DECRYPT,/* RSA pvtkey decryption, AI_PKCS_RSAPrivate */
    &AM_RSA_CRT_ENCRYPT,/* RSA pvtkey encryption, AI_PKCS_RSAPrivate, sign */
    &AM_RSA_DECRYPT,    /* RSA pubkey decryption, AI_PKCS_RSAPublic, sign */
    &AM_RSA_ENCRYPT,    /* RSA pubkey encryption, AI_PKCS_RSAPublic, */
    &AM_RSA_KEY_GEN,    /* RSA key gen */
    &AM_DH_KEY_AGREE,   /* Diffie-Hellman key negotiation */
    (B_ALGORITHM_METHOD *)NULL_PTR
};
  
static B_ALGORITHM_OBJ randomAlgorithm = NULL;

static void initialize_random_generator(void);
static void crypto_randomize(sec_pk_gen_data_t * seed);

static void crypto_initialization(void);
static int crypto_initialized = 0;
static pthread_once_t crypto_once_block = pthread_once_init;
static pthread_mutex_t randalg_mutex;

/* The following define values taken from demo bslite.h */

static unsigned char EXPONENT_F4[3] = {0x01, 0x00, 0x01};

/* define parameters for password-based encryption */

#define PBE_ITERATION_COUNT 1000

#define MAX_HASH_SIZE 16
#define DEFAULT_MOD_SIZE 1024
#define MAX_MOD_SIZE 1024
#define MIN_MOD_SIZE 256
#define DH_BUF_SIZE 1024

#define CRYPTO_INIT(s) \
{if (!crypto_initialized) \
     pthread_once(&crypto_once_block, crypto_initialization); \
 crypto_randomize(s); }

static unsigned32 sec_rsa_to_dce_status(int status) {
    if (status) {
	switch (status) {
	case BE_ALGORITHM_ALREADY_SET :
	    return sec_s_bsafe_algorithm_already_set;
	case BE_ALGORITHM_INFO :
	    return sec_s_bsafe_algorithm_info;
	case BE_ALGORITHM_NOT_INITIALIZED :
	    return sec_s_bsafe_algorithm_not_initialized;
	case BE_ALGORITHM_NOT_SET :
	    return sec_s_bsafe_algorithm_not_set;
	case BE_ALGORITHM_OBJ :
	    return sec_s_bsafe_algorithm_obj;
	case BE_ALG_OPERATION_UNKNOWN :
	    return sec_s_bsafe_alg_operation_unknown;
	case BE_ALLOC :
	    return sec_s_bsafe_alloc;
	case BE_CANCEL :
	    return sec_s_bsafe_cancel;
	case BE_DATA :
	    return sec_s_bsafe_data;
	case BE_EXPONENT_EVEN :
	    return sec_s_bsafe_exponent_even;
	case BE_EXPONENT_LEN :
	    return sec_s_bsafe_exponent_len;
	case BE_HARDWARE :
	    return sec_s_bsafe_hardware;
	case BE_INPUT_DATA :
	    return sec_s_bsafe_input_data;
	case BE_INPUT_LEN :
	    return sec_s_bsafe_input_len;
	case BE_KEY_ALREADY_SET :
	    return sec_s_bsafe_key_already_set;
	case BE_KEY_INFO :
	    return sec_s_bsafe_key_info;
	case BE_KEY_LEN :
	    return sec_s_bsafe_key_len;
	case BE_KEY_NOT_SET :
	    return sec_s_bsafe_key_not_set;
	case BE_KEY_OBJ :
	    return sec_s_bsafe_key_obj;
	case BE_KEY_OPERATION_UNKNOWN :
	    return sec_s_bsafe_key_operation_unknown;
	case BE_MEMORY_OBJ :
	    return sec_s_bsafe_memory_obj;
	case BE_MODULUS_LEN :
	    return sec_s_bsafe_modulus_len;
	case BE_NOT_INITIALIZED :
	    return sec_s_bsafe_not_initialized;
	case BE_NOT_SUPPORTED :
	    return sec_s_bsafe_not_supported;
	case BE_OUTPUT_LEN :
	    return sec_s_bsafe_output_len;
	case BE_OVER_32K :
	    return sec_s_bsafe_over_32k;
	case BE_RANDOM_NOT_INITIALIZED :
	    return sec_s_bsafe_random_not_initialized;
	case BE_RANDOM_OBJ :
	    return sec_s_bsafe_random_obj;
	case BE_SIGNATURE :
	    return sec_s_bsafe_signature;
	case BE_WRONG_ALGORITHM_INFO :
	    return sec_s_bsafe_wrong_algorithm_info;
	case BE_WRONG_KEY_INFO :
	    return sec_s_bsafe_wrong_key_info;
	case BE_INPUT_COUNT :
	    return sec_s_bsafe_input_count;
	case BE_METHOD_NOT_IN_CHOOSER :
	    return sec_s_bsafe_method_not_in_chooser;
	case BE_OUTPUT_COUNT :
	    return sec_s_bsafe_output_count;
	default: 
	    return sec_s_bsafe_unknown_error;
	};
    } else return 0;
}

static void crypto_randomize(
    sec_pk_gen_data_t * seed_data) 
{
    int status;
    unsigned char * seed;
    unsigned int seedLen;
    time_t absolute_clock_ticks;    
    clock_t process_clock_ticks;    
    

    LOCK_RANDOMALG;

    do {

	if (seed_data != NULL) {
    
	    if (status = B_RandomUpdate(randomAlgorithm,
    	                            seed_data->data,
                                    seed_data->len,
                                    (A_SURRENDER_CTX *)NULL_PTR)) break;
	};

	absolute_clock_ticks = time(NULL);
    
	seed = (unsigned char *)&absolute_clock_ticks;
	seedLen = sizeof(time_t);

	if (status = B_RandomUpdate(randomAlgorithm,
                                    seed,
                                    seedLen,
                                    (A_SURRENDER_CTX *)NULL_PTR)) break;

	process_clock_ticks = clock();
    
	seed = (unsigned char *)&process_clock_ticks;
	seedLen = sizeof(clock_t);


	if (status = B_RandomUpdate(randomAlgorithm,
                                    seed,
                                    seedLen,
                                    (A_SURRENDER_CTX *)NULL_PTR)) break;


    } while (0);

    UNLOCK_RANDOMALG;

}


static void initialize_random_generator(void)
{
    int status;
    do {

	if (status = B_CreateAlgorithmObject (&randomAlgorithm)) break;;

	if (status = B_SetAlgorithmInfo (randomAlgorithm,
	                                 AI_MD5Random,
                                         NULL_PTR)) break;

	if (status = B_RandomInit(randomAlgorithm,
                                  RSA_ALGORITHM_CHOOSER,
                                  (A_SURRENDER_CTX *)NULL_PTR)) break;

    } while (0);
    pthread_mutex_init(&randalg_mutex, pthread_mutexattr_default);
}




static void crypto_initialization(void) {
    initialize_random_generator();
}

void T_CALL T_memset (POINTER p, int c, unsigned int count)
{
  if (count != 0)
    memset (p, c, count);
}

void T_CALL T_memcpy (POINTER d, POINTER s, unsigned int count)
{
  if (count != 0)
    memcpy (d, s, count);
}

void T_CALL T_memmove (POINTER d, POINTER s, unsigned int count)
{
  if (count != 0)
    memmove (d, s, count);
}

int T_CALL T_memcmp (POINTER s1, POINTER s2, unsigned int count)
{
  if (count == 0)
    return (0);
  else
    return (memcmp (s1, s2, count));
}

POINTER T_CALL T_malloc (unsigned int size)
{
  return ((POINTER)malloc (size == 0 ? 1 : size));
}

POINTER T_CALL T_realloc (POINTER p, unsigned int size)
{
  POINTER result;

  if (p == NULL_PTR)
    return (T_malloc (size));

  if ((result = (POINTER)realloc (p, size == 0 ? 1 : size)) == NULL_PTR)
    free (p);
  return (result);
}

void T_CALL T_free (POINTER p)
{
  if (p != NULL_PTR)
    free (p);
}
 

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
 *   other (non-zero):  sec_s_bsafe_mod_size_out_of_range
 *
 * Input
 *
 *   seed: Seed data for random number generator.
 *
 * Input, Output
 *
 *   modulus_bit_size: Size of modulus in bits.  The default value is 1024.
 *                     Input value 0 indicates using default (DEFAULT_MOD_SIZE).
 *                     To avoid arbitrary modulus size, we bump the size to
 *                     half or double of default size.  Since the final
 *                     size might be different from the input, we output the
 *                     final size for reference.
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
    sec_pk_data_t           *privkey          /* [out] */
) {


    A_RSA_KEY_GEN_PARAMS keyGenParams;
    ITEM * keyItemPtr = NULL;
    B_KEY_OBJ privateKeyObj = NULL;
    B_KEY_OBJ publicKeyObj = NULL;
    B_ALGORITHM_OBJ generateAlgorithmObj = NULL;
    int status = 0;

    CRYPTO_INIT(seed);

    if (modulus_bit_size == 0) {
       modulus_bit_size = DEFAULT_MOD_SIZE;
    } else if (modulus_bit_size > MAX_MOD_SIZE || modulus_bit_size < MIN_MOD_SIZE) {
       status =  sec_s_bsafe_mod_size_out_of_range;
       return status;
    }

    do {

	CREATE_PK_DESCRIPTOR(pubkey);
	CREATE_PK_DESCRIPTOR(privkey);

	if (status = B_CreateAlgorithmObject (&generateAlgorithmObj)) break;

	keyGenParams.modulusBits = modulus_bit_size;
	keyGenParams.publicExponent.data = EXPONENT_F4;
	keyGenParams.publicExponent.len = sizeof(EXPONENT_F4);

	if (status = B_SetAlgorithmInfo(generateAlgorithmObj,
	                                AI_RSAKeyGen,
					(POINTER)&keyGenParams)) break;


	if (status = B_GenerateInit(generateAlgorithmObj,
                                    RSA_ALGORITHM_CHOOSER,
				    NULL)) break;

	if (status = B_CreateKeyObject(&privateKeyObj)) break;

	if (status = B_CreateKeyObject(&publicKeyObj)) break;

	LOCK_RANDOMALG;
	status = B_GenerateKeypair(generateAlgorithmObj,
                                   publicKeyObj,
                                   privateKeyObj,
                                   randomAlgorithm,
                                   NULL);
	UNLOCK_RANDOMALG;
        if (status) break;

	if (status = B_GetKeyInfo((POINTER *)&keyItemPtr,
				  publicKeyObj,
				  KI_RSAPublicBER)) break;

	CREATE_PK_STORAGE(pubkey, keyItemPtr->len);

	memcpy(pubkey->data, keyItemPtr->data, keyItemPtr->len);
	pubkey->len = keyItemPtr->len;

	if (status = B_GetKeyInfo((POINTER *)&keyItemPtr,
				  privateKeyObj,
				  KI_PKCS_RSAPrivateBER)) break;

	CREATE_PK_STORAGE(privkey, keyItemPtr->len);

	memcpy(privkey->data, keyItemPtr->data, keyItemPtr->len);
	privkey->len = keyItemPtr->len;

    } while (0);


    B_DestroyAlgorithmObject(&generateAlgorithmObj);
    B_DestroyKeyObject(&privateKeyObj);
    B_DestroyKeyObject(&publicKeyObj);
    if (status) {
	CLEANUP_PK_DESCRIPTOR(pubkey);
	CLEANUP_PK_DESCRIPTOR(privkey);
	return sec_rsa_to_dce_status(status);
    } else return 0;
}

/*
 * sec_bsafe_encrypt_pvtkey
 * 
 * Generates password-based encrypted private key which uses password 
 * generated DES key to encrypt private key.  It computes the hash value 
 * based on the name and public key part of the key pair for salt value. 
 * It uses RSA algorithm AI_MD2WithRC2_CBCPad or AI_MD5WithRC2_CBCPad to
 * turn password(passphrase) into a DES key (which is n time hashed value of
 * the password), then encrypt the private key with this DES key.
 * See PKCS #5.
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
)
{
    B_KEY_OBJ passwordKeyObj = NULL_PTR;
    B_ALGORITHM_OBJ saltDigestAlgorithmObj = NULL_PTR;
    B_ALGORITHM_OBJ pbEncryptionAlgorithmObj = NULL_PTR;
    B_PBE_PARAMS pbeParamsStruct;
    ITEM passwordItem, *pbeBERItemPtr;
    unsigned char pbeSaltBuf[16];
    unsigned int digestLen;
    int status;
    error_status_t st;
    unsigned char *res = NULL;
    size_t res_buffer_size;
    unsigned int out_len;
    unsigned int     modulus_bit_size; /* for calculating
					  encrypted private key */
    ITEM privateKeyItem = {0,0};
    B_KEY_OBJ privateKeyObj;				  
    A_RSA_KEY * privateKey = NULL;				

    /* initialize algorithm object to NULL pointer */
    CLEAR_STATUS(&st);
      

    do {

	CREATE_PK_DESCRIPTOR(pbe_params);
	CREATE_PK_DESCRIPTOR(encrypted_pvtkey);

	if (status = B_CreateAlgorithmObject (&saltDigestAlgorithmObj)) break;

	if (status = B_SetAlgorithmInfo (saltDigestAlgorithmObj, 
					 AI_MD5, 
					 NULL_PTR)) break;


	if (status = B_DigestInit (saltDigestAlgorithmObj, 
				   NULL_PTR, 
				   RSA_ALGORITHM_CHOOSER,
				   (A_SURRENDER_CTX *)NULL_PTR)) break;

	if (status = B_DigestUpdate (saltDigestAlgorithmObj, 
				     pwd->data, 
				     pwd->len,
				     (A_SURRENDER_CTX *)NULL_PTR)) break;
  
	if (status = B_DigestUpdate (saltDigestAlgorithmObj, 
				     pvtkey->data, 
				     pvtkey->len,
				     (A_SURRENDER_CTX *)NULL_PTR)) break;

	if (status = B_DigestFinal (saltDigestAlgorithmObj, 
				    pbeSaltBuf, 
				    &digestLen, 
				    sizeof(pbeSaltBuf),
				    (A_SURRENDER_CTX *)NULL_PTR)) break;

	if (status = B_CreateAlgorithmObject (&pbEncryptionAlgorithmObj)) break;

	pbeParamsStruct.salt = pbeSaltBuf;
	pbeParamsStruct.iterationCount = PBE_ITERATION_COUNT;


	if (status = B_SetAlgorithmInfo (pbEncryptionAlgorithmObj, 
					 AI_MD5WithDES_CBCPad,
					 (POINTER)&pbeParamsStruct)) break;

	/* get the PBE parameters in their BER encoded form */
	if (status = B_GetAlgorithmInfo ((POINTER *)&pbeBERItemPtr, 
					 pbEncryptionAlgorithmObj,
					 AI_MD5WithDES_CBCPadBER)) break;

	/* return encoded pbe parameters and length to the caller in the
	 * supplied args.
	 */

	CREATE_PK_STORAGE(pbe_params, pbeBERItemPtr->len);

	pbe_params->len = pbeBERItemPtr->len;
	memcpy(pbe_params->data, 
             pbeBERItemPtr->data, 
             pbe_params->len);
 

	if (status = B_CreateKeyObject (&passwordKeyObj)) break;

	/* setup password item and set key info */
	passwordItem.data = pwd->data;
	passwordItem.len = pwd->len;


	res_buffer_size = pvtkey->len + 8;  /* atleast 8 bytes bigger than input */
	if ((res = (unsigned char *)malloc(res_buffer_size)) == NULL) {
	    status = BE_ALLOC; 
	    break;
	}
	memset(res, 0, res_buffer_size); 
		    
	if (status = B_SetKeyInfo (passwordKeyObj, 
				   KI_Item, 
				   (POINTER)&passwordItem)) break;

	/* encrypt the private key DER */
	if (status = B_EncryptInit (pbEncryptionAlgorithmObj, 
				    passwordKeyObj,
				    RSA_ALGORITHM_CHOOSER, 
				    (A_SURRENDER_CTX *)NULL_PTR)) break;

	if (status = B_EncryptUpdate(pbEncryptionAlgorithmObj, 
				     res, 
				     &out_len,
				     res_buffer_size,
				     pvtkey->data, 
				     pvtkey->len,
				     (B_ALGORITHM_OBJ)NULL_PTR, 
				     (A_SURRENDER_CTX *)NULL_PTR)) break;

	encrypted_pvtkey->len = out_len;
	if (status = B_EncryptFinal(pbEncryptionAlgorithmObj, 
				    res + encrypted_pvtkey->len,
				    &out_len, 
				    res_buffer_size - encrypted_pvtkey->len,
				    (B_ALGORITHM_OBJ)NULL_PTR, 
				    (A_SURRENDER_CTX *)NULL_PTR)) break;

	encrypted_pvtkey->len += out_len;
	CREATE_PK_STORAGE(encrypted_pvtkey, encrypted_pvtkey->len);
	memcpy(encrypted_pvtkey->data, res, encrypted_pvtkey->len);

    }  while (0);

    /* cleanup objects */
    B_DestroyAlgorithmObject (&pbEncryptionAlgorithmObj);
    B_DestroyAlgorithmObject (&saltDigestAlgorithmObj);
    B_DestroyKeyObject (&passwordKeyObj);
    if (res)
        free(res);

    /* Convert BSAFE error status to DCE error status */
    if (status) {
	CLEANUP_PK_DESCRIPTOR(pbe_params);
	CLEANUP_PK_DESCRIPTOR(encrypted_pvtkey);
	return sec_rsa_to_dce_status(status);
    } else return 0;

}

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
)
{
    B_KEY_OBJ passwordKeyObj = NULL_PTR;
    B_ALGORITHM_OBJ pbEncryptionAlgorithmObj = NULL_PTR;
    ITEM passwordItem, pbEncryptionInfoItem;
    int status;
    error_status_t st;
    unsigned char *res = NULL;
    size_t res_buffer_size;
    unsigned int out_len;
  
    /* initialize algorithm object to NULL pointer */
    CLEAR_STATUS(&st);
      

    do {

	CREATE_PK_DESCRIPTOR(decrypted_pvtkey);

	if (status = B_CreateAlgorithmObject (&pbEncryptionAlgorithmObj)) break;

	/* set up the ITEM which supplies the password-based encryption
         * algorithm info.  This contains the salt and iteration count
         * which were specified when the private key was password-encrypted.
	 */
	pbEncryptionInfoItem.data = pbe_params->data;
	pbEncryptionInfoItem.len = pbe_params->len;

	if (status = B_SetAlgorithmInfo(pbEncryptionAlgorithmObj, 
					AI_MD5WithDES_CBCPadBER,
					(POINTER)&pbEncryptionInfoItem)) break;

	if (status = B_CreateKeyObject (&passwordKeyObj)) break;

	/* setup password item and set key info */
	passwordItem.data = pwd->data;
	passwordItem.len = pwd->len;
	if (status = B_SetKeyInfo(passwordKeyObj, 
				   KI_Item, 
				   (POINTER)&passwordItem)) break;

	if ((res = (unsigned char *)malloc(pvtkey->len)) == NULL) {
	    status = BE_ALLOC; 
	    break;
	};
 
        res_buffer_size = pvtkey->len;

	/* decrypt the private key DER */
	if (status = B_DecryptInit(pbEncryptionAlgorithmObj, 
				   passwordKeyObj, 
				   RSA_ALGORITHM_CHOOSER,
				   (A_SURRENDER_CTX *)NULL_PTR)) break;

	if (status = B_DecryptUpdate(pbEncryptionAlgorithmObj, 
				     res, 
				     &out_len, 
				     res_buffer_size,
				     pvtkey->data, 
				     pvtkey->len, 
				     (B_ALGORITHM_OBJ)NULL_PTR,
				     (A_SURRENDER_CTX *)NULL_PTR)) break;

	decrypted_pvtkey->len = out_len;
	if (status = B_DecryptFinal(pbEncryptionAlgorithmObj, 
				    res + decrypted_pvtkey->len,
				    &out_len, 
				    res_buffer_size - decrypted_pvtkey->len,
				    (B_ALGORITHM_OBJ)NULL_PTR, 
				    (A_SURRENDER_CTX *)NULL_PTR)) break;

	decrypted_pvtkey->len += out_len;
	CREATE_PK_STORAGE(decrypted_pvtkey, decrypted_pvtkey->len);
	memcpy(decrypted_pvtkey->data, res, decrypted_pvtkey->len);

    } while (0);

    /* cleanup objects */
    B_DestroyAlgorithmObject (&pbEncryptionAlgorithmObj);
    B_DestroyKeyObject (&passwordKeyObj);
    if (res)
      free(res);

    /* Convert BSAFE error status to DCE error status */
    if (status) {
	CLEANUP_PK_DESCRIPTOR(decrypted_pvtkey);
	return sec_rsa_to_dce_status(status);
    } else return 0;

}

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
    sec_pk_data_t           *privkey,          /* [in] */
    sec_pk_data_t           *signature         /* [out] */
) {


    B_ALGORITHM_OBJ signatureAlgorithm = NULL;
    B_KEY_OBJ privateKeyObj;
    A_RSA_KEY * privateKey = NULL;
    ITEM privateKeyItem = {0,0};
    unsigned int final_outlen;
    unsigned32 modulus_bytes = 0;
    size_t res_buffer_size;
    int status = 0;
    

    CRYPTO_INIT(NULL);  

    CREATE_PK_DESCRIPTOR(signature);

    do {

        /* set up an algorithm to create signature */
        if (status = B_CreateAlgorithmObject (&signatureAlgorithm)) break;

        /* set algorithm info */
        switch (signature_alg) {
        case sec_sign_md2rsa:
            status = B_SetAlgorithmInfo(signatureAlgorithm, 
                                        AI_MD2WithRSAEncryption, 
                                        NULL_PTR);
            break;
        case sec_sign_md5rsa:
            status = B_SetAlgorithmInfo(signatureAlgorithm, 
                                        AI_MD5WithRSAEncryption, 
                                        NULL_PTR);
            break;
        default:
            status = BE_ALG_OPERATION_UNKNOWN;
            break;
        }
        if (status != 0) break;

	if (status = B_CreateKeyObject(&privateKeyObj)) break;

	privateKeyItem.data = privkey->data;
	privateKeyItem.len = privkey->len;
	
	if (status = B_SetKeyInfo(privateKeyObj,
				  KI_PKCS_RSAPrivateBER,
				  (POINTER)&privateKeyItem)) {

            break;
	};

	privateKeyItem.data = NULL;
	privateKeyItem.len = 0;

        /* Retrieve the key to find out the modulus... */
	if (status = B_GetKeyInfo((POINTER *)&privateKey,
				  privateKeyObj,
				  KI_RSAPrivate)) break;

	modulus_bytes = privateKey->modulus.len;
	
	CREATE_PK_STORAGE(signature, modulus_bytes);
	res_buffer_size = modulus_bytes;

	if (status = B_SignInit(signatureAlgorithm,
			        privateKeyObj,
				RSA_ALGORITHM_CHOOSER,
				NULL)) break;

	if (status = B_SignUpdate(signatureAlgorithm,
				  data->data,
				  (unsigned int)data->len,
				  NULL)) break;

	if (status = B_SignFinal(signatureAlgorithm,
	                         signature->data,
				 &final_outlen,
				 res_buffer_size,
				 (B_ALGORITHM_OBJ)NULL_PTR,
				 NULL)) break;

	signature->len = final_outlen;
				      
    } while (0);

    B_DestroyAlgorithmObject(&signatureAlgorithm);

    B_DestroyKeyObject(&privateKeyObj);

    if (status == 0) return 0;

    CLEANUP_PK_DESCRIPTOR(signature);

    return sec_rsa_to_dce_status(status);

}			

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
)
{

    B_ALGORITHM_OBJ signatureAlgorithm = NULL;
    B_KEY_OBJ publicKeyObj;
    ITEM publicKeyItem = {0,0};
    int status = 0;
        
    CRYPTO_INIT(NULL);
    
    do {
    
        /* set up an algorithm to create signature */
        if (status = B_CreateAlgorithmObject (&signatureAlgorithm)) break;

        /* set algorithm info */
        switch (signature_alg) {
        case sec_sign_md2rsa:
            status = B_SetAlgorithmInfo(signatureAlgorithm, 
                                        AI_MD2WithRSAEncryption, 
                                        NULL_PTR);
            break;
        case sec_sign_md5rsa:
            status = B_SetAlgorithmInfo(signatureAlgorithm,
                                        AI_MD5WithRSAEncryption, 
                                        NULL_PTR);
            break;
        default:
            status = BE_ALG_OPERATION_UNKNOWN;
            break;
        }
        if (status != 0) break;

	if (status = B_CreateKeyObject(&publicKeyObj)) break;

	publicKeyItem.data = pubkey->data;
	publicKeyItem.len = pubkey->len;
			
	if (status = B_SetKeyInfo(publicKeyObj,
				  KI_RSAPublicBER,
				  (POINTER)&publicKeyItem)) break;

	publicKeyItem.data = NULL;
	publicKeyItem.len = 0;
	
	if (status = B_VerifyInit(signatureAlgorithm,
			          publicKeyObj,
				  RSA_ALGORITHM_CHOOSER,
				  NULL)) break;

	if (status = B_VerifyUpdate(signatureAlgorithm,
				    data->data,
				    (unsigned int)data->len,
				    NULL)) break;

	if (status = B_VerifyFinal(signatureAlgorithm,
	                           signature->data,
				   (unsigned int)signature->len,
				   (B_ALGORITHM_OBJ)NULL_PTR,
				   NULL)) break;
    } while (0);

    B_DestroyAlgorithmObject(&signatureAlgorithm);
    B_DestroyKeyObject(&publicKeyObj);

    return sec_rsa_to_dce_status(status);
}

static int sec__RSA_encode_pubkey(
			sec_pk_gen_data_t * seed,
                        sec_pk_data_t     * pubkey,
                        sec_pk_gen_data_t * data,
                        sec_pk_gen_data_t * res)
{

    
    B_ALGORITHM_OBJ rsaAlgorithm = NULL;
    B_KEY_OBJ publicKeyObj;
    ITEM publicKeyItem = {0,0};
    A_RSA_KEY * publicKey = NULL;
    unsigned int final_outlen = 0;
    unsigned32 modulus_bytes = 0;
    size_t res_buffer_size;
    int status = 0;
    unsigned int loc_res_length;
    
    CRYPTO_INIT(seed);
    
    CREATE_DESCRIPTOR(res);

    do {

	if (status = B_CreateAlgorithmObject(&rsaAlgorithm)) break;

	if (status = B_SetAlgorithmInfo(rsaAlgorithm,
                                        AI_PKCS_RSAPublic,
                                        NULL)) break;

	if (status = B_CreateKeyObject(&publicKeyObj)) break;

	publicKeyItem.data = pubkey->data;
	publicKeyItem.len = pubkey->len;
	
		
	if (status = B_SetKeyInfo(publicKeyObj,
				  KI_RSAPublicBER,
				  (POINTER)&publicKeyItem)) break;

	publicKeyItem.data = NULL;
	publicKeyItem.len = 0;

/* Retrieve the key to find out the modulus... */
	if (status = B_GetKeyInfo((POINTER *)&publicKey,
				  publicKeyObj,
				  KI_RSAPublic)) break;
	modulus_bytes = publicKey->modulus.len;

	publicKey = NULL;
	
	CREATE_STORAGE(res, modulus_bytes);
	res_buffer_size = modulus_bytes;

	if (status = B_EncryptInit(rsaAlgorithm,
                                   publicKeyObj,
                                   RSA_ALGORITHM_CHOOSER,
                                   NULL)) break;

	LOCK_RANDOMALG;
	status = B_EncryptUpdate(rsaAlgorithm,
                                 res->data,
                                 &loc_res_length,
                                 res_buffer_size,
                                 data->data,
                                 data->len,
                                 randomAlgorithm,
                                 NULL);
	UNLOCK_RANDOMALG;

        if (status) break;

	LOCK_RANDOMALG;
	status = B_EncryptFinal(rsaAlgorithm,
                                res->data+loc_res_length,
                                &final_outlen,
                                res_buffer_size-loc_res_length,
                                randomAlgorithm,
                                NULL);
	UNLOCK_RANDOMALG;
        if (status) break;

	res->len = loc_res_length+final_outlen;
				      


    } while (0);

    B_DestroyAlgorithmObject(&rsaAlgorithm);
    B_DestroyKeyObject(&publicKeyObj);
    if (status == 0) return 0;
    CLEANUP_DESCRIPTOR(res);
    return status;

}

static int sec__RSA_encode_privkey(
			sec_pk_gen_data_t * seed,
                        sec_pk_data_t     * privkey,
                        sec_pk_gen_data_t * data,
                        sec_pk_gen_data_t * res)
{
    
    B_ALGORITHM_OBJ rsaAlgorithm = NULL;
    B_KEY_OBJ privateKeyObj;
    A_RSA_KEY * privateKey = NULL;
    ITEM privateKeyItem = {0,0};
    unsigned int final_outlen;
    unsigned32 modulus_bytes = 0;
    size_t res_buffer_size;
    unsigned int loc_res_length = 0;
    int status = 0;
    

    CRYPTO_INIT(seed);

    CREATE_DESCRIPTOR(res);

    do {

	if (status = B_CreateAlgorithmObject(&rsaAlgorithm)) break;

	if (status = B_SetAlgorithmInfo(rsaAlgorithm,
                                        AI_PKCS_RSAPrivate,
                                        NULL)) break;

	if (status = B_CreateKeyObject(&privateKeyObj)) break;

	privateKeyItem.data = privkey->data;
	privateKeyItem.len = privkey->len;
	
	if (status = B_SetKeyInfo(privateKeyObj,
				  KI_PKCS_RSAPrivateBER,
				  (POINTER)&privateKeyItem)) break;

	privateKeyItem.data = NULL;
	privateKeyItem.len = 0;

/* Retrieve the key to find out the modulus... */
	if (status = B_GetKeyInfo((POINTER *)&privateKey,
				  privateKeyObj,
				  KI_RSAPrivate)) break;
	modulus_bytes = privateKey->modulus.len;

	privateKey = NULL;
	
	CREATE_STORAGE(res, modulus_bytes);
	res_buffer_size = modulus_bytes;

	if (status = B_EncryptInit(rsaAlgorithm,
				   privateKeyObj,
				   RSA_ALGORITHM_CHOOSER,
				   NULL)) break;

	LOCK_RANDOMALG;
	status = B_EncryptUpdate(rsaAlgorithm,
                                 res->data,
                                 &loc_res_length,
                                 res_buffer_size,
                                 data->data,
                                 (unsigned int)data->len,
                                 randomAlgorithm,
                                 NULL);
	UNLOCK_RANDOMALG;

        if (status) break;

	LOCK_RANDOMALG;
	status = B_EncryptFinal(rsaAlgorithm,
                                res->data+loc_res_length,
                                &final_outlen,
                                res_buffer_size-loc_res_length,
                                randomAlgorithm,
                                NULL);
	UNLOCK_RANDOMALG;

        if (status) break;

	res->len = loc_res_length + final_outlen;
				      

    } while (0);

    B_DestroyAlgorithmObject(&rsaAlgorithm);
    B_DestroyKeyObject(&privateKeyObj);

    if (status == 0) return 0;

    CLEANUP_DESCRIPTOR(res);
    return status;
}

static int sec__RSA_decode_pubkey(
			sec_pk_gen_data_t * seed,
                        sec_pk_data_t     * pubkey,
                        sec_pk_gen_data_t * data,
                        sec_pk_gen_data_t * res)
{

    B_ALGORITHM_OBJ rsaAlgorithm = NULL;
    B_KEY_OBJ publicKeyObj;
    ITEM publicKeyItem = {0,0};
    unsigned int final_outlen = 0;
    size_t res_buffer_size;
    int status = 0;
    unsigned int loc_res_length = 0;
        
    CRYPTO_INIT(seed);

    CREATE_DESCRIPTOR(res);

    do {

	if (status = B_CreateAlgorithmObject(&rsaAlgorithm)) break;

	if (status = B_SetAlgorithmInfo(rsaAlgorithm,
                                        AI_PKCS_RSAPublic,
                                        NULL)) break;

	if (status = B_CreateKeyObject(&publicKeyObj)) break;

	publicKeyItem.data = pubkey->data;
	publicKeyItem.len = pubkey->len;
			
	if (status = B_SetKeyInfo(publicKeyObj,
				  KI_RSAPublicBER,
				  (POINTER)&publicKeyItem)) break;

	publicKeyItem.data = NULL;
	publicKeyItem.len = 0;
	
	CREATE_STORAGE(res, data->len);
	res_buffer_size = data->len;

	if (status = B_DecryptInit(rsaAlgorithm,
				   publicKeyObj,
				   RSA_ALGORITHM_CHOOSER,
				   NULL)) break;

	LOCK_RANDOMALG;
	status = B_DecryptUpdate(rsaAlgorithm,
                                 res->data,
                                 &loc_res_length,
                                 res_buffer_size,
                                 data->data,
                                 data->len,
                                 randomAlgorithm,
                                 NULL);
	UNLOCK_RANDOMALG;

        if (status) break;

	LOCK_RANDOMALG;
	status = B_DecryptFinal(rsaAlgorithm,
                                res->data+loc_res_length,
                                &final_outlen,
                                res_buffer_size-loc_res_length,
                                randomAlgorithm,
                                NULL);
	UNLOCK_RANDOMALG;

        if (status) break;

	res->len = loc_res_length + final_outlen;
				      

    } while (0);

    B_DestroyAlgorithmObject(&rsaAlgorithm);
    B_DestroyKeyObject(&publicKeyObj);
    
    if (status == 0) return 0;

    CLEANUP_DESCRIPTOR(res);
    return status;

}

static int sec__RSA_decode_privkey(
			sec_pk_gen_data_t * seed,
                        sec_pk_data_t     * privkey,
                        sec_pk_gen_data_t * data,
                        sec_pk_gen_data_t * res)
{
    B_ALGORITHM_OBJ rsaAlgorithm = NULL;
    B_KEY_OBJ privateKeyObj;
    ITEM privateKeyItem = {0,0};
    unsigned int final_outlen = 0;
    size_t res_buffer_size;
    unsigned int loc_res_length = 0;
    int status = 0;
    
    CRYPTO_INIT(seed);

    CREATE_DESCRIPTOR(res);

    do {
	if (status = B_CreateAlgorithmObject(&rsaAlgorithm)) break;

	if (status = B_SetAlgorithmInfo(rsaAlgorithm,
                                        AI_PKCS_RSAPrivate,
                                        NULL)) break;

	if (status = B_CreateKeyObject(&privateKeyObj)) break;

	privateKeyItem.data = privkey->data;
	privateKeyItem.len = privkey->len;
			
	if (status = B_SetKeyInfo(privateKeyObj,
				  KI_PKCS_RSAPrivateBER,
				  (POINTER)&privateKeyItem)) break;

	privateKeyItem.data = NULL;
	privateKeyItem.len = 0;

	CREATE_STORAGE(res, data->len);
	res_buffer_size = data->len;

	if (status = B_DecryptInit(rsaAlgorithm,
				   privateKeyObj,
				   RSA_ALGORITHM_CHOOSER,
				   NULL)) break;

	LOCK_RANDOMALG;
	status = B_DecryptUpdate(rsaAlgorithm,
                                 res->data,
                                 &loc_res_length,
                                 res_buffer_size,
                                 data->data,
                                 data->len,
                                 randomAlgorithm,
                                 NULL);
	UNLOCK_RANDOMALG;

        if (status) break;

	LOCK_RANDOMALG;
	status = B_DecryptFinal(rsaAlgorithm,
                                res->data+loc_res_length,
                                &final_outlen,
                                res_buffer_size-loc_res_length,
                                randomAlgorithm,
                                NULL);
	UNLOCK_RANDOMALG;

        if (status) break;

	res->len = loc_res_length + final_outlen;
				      

    } while (0);

    B_DestroyAlgorithmObject(&rsaAlgorithm);
    B_DestroyKeyObject(&privateKeyObj);
    if (status == 0) return 0;
    CLEANUP_DESCRIPTOR(res);
    return status;
}			

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
) {

    int status = 0;

    CREATE_DESCRIPTOR(cipher_text);

     if (encrypt_alg != sec_enc_rsa_pub) {
        status = BE_ALG_OPERATION_UNKNOWN;
    } else  status = sec__RSA_encode_pubkey(seed,
                                           pubkey,
                                           plain_text,
                                           cipher_text);

    return sec_rsa_to_dce_status(status);
}

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
 *   plain_text: Pointer to the decrypted plain text. 

 *
 */

error_status_t  sec_bsafe_decrypt_data(
    sec_bsafe_alg_type_t    decrypt_alg,  /* [in] */    
    sec_pk_data_t           *pvtkey,           /* [in] */
    sec_pk_gen_data_t       *cipher_text,      /* [in] */
    sec_pk_gen_data_t       *plain_text        /* [out] */
) {

    int status = 0;

    if (decrypt_alg != sec_enc_rsa_priv) {
        status = BE_ALG_OPERATION_UNKNOWN;
    } else status = sec__RSA_decode_privkey(NULL,
                                            pvtkey,
                                            cipher_text,
                                            plain_text);

    return sec_rsa_to_dce_status(status);
}

error_status_t sec_bsafe_hash_init(
                        sec_bsafe_alg_type_t digest_alg	/* in */,
			void ** hash_context		/* out */
) {
    B_ALGORITHM_OBJ mdAlgorithm = NULL;
    int status = 0;

    CRYPTO_INIT(NULL);

    *hash_context = NULL;
    do {


	if (status = B_CreateAlgorithmObject (&mdAlgorithm)) break;

	switch (digest_alg) {
	case sec_bsafe_hash_MD2:
	  status = B_SetAlgorithmInfo (mdAlgorithm, AI_MD2, NULL_PTR);
	  break;
	case sec_bsafe_hash_MD5:
	  status = B_SetAlgorithmInfo (mdAlgorithm, AI_MD5, NULL_PTR);
	  break;
	default:
	  status = BE_ALG_OPERATION_UNKNOWN;
	  break;
	}
	if (status != 0) break;


	if (status = B_DigestInit(mdAlgorithm,
	                          NULL_PTR,
				  RSA_ALGORITHM_CHOOSER,
				  NULL)) break;
	*hash_context = (void *)mdAlgorithm;
    } while (0);    

    if (status == 0) return 0;

    B_DestroyAlgorithmObject(&mdAlgorithm);
    *hash_context = NULL;

    return sec_rsa_to_dce_status(status);
}			

error_status_t sec_bsafe_hash_update(
			void              ** hash_context	/* in */,
			sec_pk_gen_data_t * data	/* in */
) {
    B_ALGORITHM_OBJ mdAlgorithm = (B_ALGORITHM_OBJ)(*hash_context);
    int status = 0;

    CRYPTO_INIT(NULL);

    if (mdAlgorithm == NULL) return sec_rsa_to_dce_status(BE_ALGORITHM_OBJ);

    do {
	if (status = B_DigestUpdate(mdAlgorithm,
	                            data->data,
				    (unsigned int)data->len,
				    NULL)) break;
    } while (0);
    if (status == 0) return 0;

    B_DestroyAlgorithmObject(&mdAlgorithm);
    *hash_context = NULL;

    return sec_rsa_to_dce_status(status);

}

error_status_t sec_bsafe_hash_final(
			void              ** hash_context,	/* in */
			sec_pk_gen_data_t * hash	/* out */
) {
    unsigned int length;
    B_ALGORITHM_OBJ mdAlgorithm = (B_ALGORITHM_OBJ)(*hash_context);
    int status = 0;

    CRYPTO_INIT(NULL);

    CREATE_DESCRIPTOR(hash);

    if (mdAlgorithm == NULL) return sec_rsa_to_dce_status(BE_ALGORITHM_OBJ);

    do {

	CREATE_STORAGE(hash, MAX_HASH_SIZE);

	if (status = B_DigestFinal(mdAlgorithm,
	                           hash->data,
				   &length,
				   MAX_HASH_SIZE,
				   NULL)) break;
	              
	hash->len = length;
    } while (0);

    B_DestroyAlgorithmObject(&mdAlgorithm);
    *hash_context = NULL;

    if (status == 0) return 0;

    CLEANUP_DESCRIPTOR(hash);

    return sec_rsa_to_dce_status(status);
}

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
    sec_bsafe_alg_type_t         digest_alg,   /* [in] */
    sec_pk_gen_data_t            *data,         /* [in] */
    sec_pk_gen_data_t            *digest       /* [out] */
) {

    void * context;
    error_status_t st;

    if (st = sec_bsafe_hash_init(digest_alg,
                                 &context)) return st;
    if (st = sec_bsafe_hash_update(&context,
                                   data)) return st;
    if (st = sec_bsafe_hash_final(&context,
                                  digest)) return st;
    return 0;
}


error_status_t sec_bsafe_diffie_hellman_phase_1 (
			void ** dh_context,
                        sec_pk_gen_data_t * seed,
			sec_pk_gen_data_t * dh_parameters,
			sec_pk_gen_data_t * public_value
) {
    B_ALGORITHM_OBJ dh = NULL;
    int status = 0;
    unsigned int res_length;
    ITEM parameters;
        
    CRYPTO_INIT(seed);

    *dh_context = NULL;
    parameters.data = dh_parameters->data;
    parameters.len = dh_parameters->len;

    CREATE_DESCRIPTOR(public_value);

    do {

	if (status = B_CreateAlgorithmObject(&dh)) break;

	if (status = B_SetAlgorithmInfo(dh,
                                        AI_DHKeyAgreeBER,
                                        (POINTER)&parameters)) break;

	if (status = B_KeyAgreeInit(dh,
	                            (B_KEY_OBJ)NULL_PTR,
				    RSA_ALGORITHM_CHOOSER,
				    NULL)) break;

	CREATE_STORAGE(public_value, DH_BUF_SIZE);

	LOCK_RANDOMALG;
	status = B_KeyAgreePhase1(dh,
                                  public_value->data,
                                  &res_length,
                                  DH_BUF_SIZE,
                                  randomAlgorithm,
                                  NULL);
	UNLOCK_RANDOMALG;

        if (status) break;

	public_value->len = res_length;
    } while (0);

    *dh_context = dh;

    if (status == 0) return 0;
    
    B_DestroyAlgorithmObject(&dh);
    CLEANUP_DESCRIPTOR(public_value);    
    return sec_rsa_to_dce_status(status);

    
}

error_status_t sec_bsafe_diffie_hellman_phase_2 (
			void              ** dh_context,
			sec_pk_gen_data_t *peer_public_value,
			sec_pk_gen_data_t * key_buffer) {
    
    B_ALGORITHM_OBJ dh = NULL;
    int status = 0;
    unsigned int res_length;
    unsigned char temp_buffer[DH_BUF_SIZE];
            
    CRYPTO_INIT(NULL);

    dh = (B_ALGORITHM_OBJ)(*dh_context);
    if (dh == NULL) return sec_rsa_to_dce_status(BE_ALGORITHM_OBJ);
    
    do {
	if (status = B_KeyAgreePhase2(dh,
	                              temp_buffer,
				      &res_length,
				      DH_BUF_SIZE,
				      peer_public_value->data,
				      (unsigned int)peer_public_value->len,
				      NULL)) break;


	if (key_buffer->len <= res_length) {
	    memcpy(key_buffer->data,
		   temp_buffer+DH_BUF_SIZE-key_buffer->len,
		   key_buffer->len);
	} else {
	    status = BE_KEY_LEN;
	    break;
	};

    } while (0);

    B_DestroyAlgorithmObject(&dh);
    memset(temp_buffer, 0, sizeof(temp_buffer));
    *dh_context = NULL;
    
    return sec_rsa_to_dce_status(status);

}

error_status_t sec_bsafe_symmetric_alginfo (
    sec_bsafe_alg_type_t alg,
    unsigned32  *key_length,
    unsigned32  *iv_length,
    unsigned32  *chunksize,
    unsigned32  *maxpad
) {
    
    switch (alg) {
    case sec_bsafe_enc_des:
        if (key_length) *key_length = 8;
        if (iv_length) *iv_length = 8;
        if (chunksize) *chunksize = 1;
        if (maxpad) *maxpad = 8;
        return 0;
    case sec_bsafe_enc_des_PEM:
        if (key_length) *key_length = 8;
        if (iv_length) *iv_length = 25; /* "DEC-CDC, 0123456789ABCDEF" */
        if (chunksize) *chunksize = 1;
        if (maxpad) *maxpad = 8;
        return 0;
    case sec_bsafe_enc_des_nopad:
        if (key_length) *key_length = 8;
        if (iv_length) *iv_length = 8;
        if (chunksize) *chunksize = 8;
        if (maxpad) *maxpad = 0;
        return 0;
    case sec_bsafe_enc_des_ede3:
        if (key_length) *key_length = 24;
        if (iv_length) *iv_length = 8;
        if (chunksize) *chunksize = 1;
        if (maxpad) *maxpad = 8;
        return 0;
    case sec_bsafe_enc_des_ede3_nopad:
        if (key_length) *key_length = 24;
        if (iv_length) *iv_length = 8;
        if (chunksize) *chunksize = 8;
        if (maxpad) *maxpad = 0;
        return 0;
    default:
        if (key_length) *key_length = 0;
        if (iv_length) *iv_length = 0;
        if (chunksize) *chunksize = 0;
        if (maxpad) *maxpad = 0;
        return sec_s_bsafe_alg_operation_unknown;
    };
}

error_status_t sec_bsafe_symmetric_encrypt (
    sec_bsafe_alg_type_t    alg,         /* [in] */
    sec_pk_gen_data_t       *key,        /* [in] */
    sec_pk_gen_data_t       *iv,         /* [in] */
    sec_pk_gen_data_t       *plaintext,  /* [in] */
    sec_pk_gen_data_t       *ciphertext  /* [out] */
) {

    B_ALGORITHM_OBJ enc = NULL;
    ITEM keyItem = {0,0};
    B_KEY_OBJ keyObj = NULL;
    int status = 0;
    error_status_t estatus;
    unsigned int res_length;
    unsigned int final_outlen;
    size_t res_buffer_size;

    unsigned32 key_length;
    unsigned32 iv_length;
    unsigned32 chunksize;
    unsigned32 maxpad;


    CRYPTO_INIT(NULL);

    CREATE_DESCRIPTOR(ciphertext);
    
    estatus = sec_bsafe_symmetric_alginfo (alg,
                                           &key_length,
                                           &iv_length,
                                           &chunksize,
                                           &maxpad);
    if (estatus) return estatus;

    /* sanity-check input lengths... */

    if (key->len != key_length) return sec_s_bsafe_key_len;

    if (alg != sec_bsafe_enc_des_PEM) {
        if (iv->len != iv_length) return sec_s_bsafe_input_len;
    };
    /* Don't bother sanity-checking PEM length as it's not worth the effort */

    if ((plaintext->len % chunksize) != 0) return sec_s_bsafe_input_len;

    do {

        keyItem.data = key->data;
        keyItem.len = key->len;

        if (status = B_CreateAlgorithmObject(&enc)) break;

	if (status = B_CreateKeyObject(&keyObj)) break;

        switch (alg) {
        case sec_bsafe_enc_des:
            if (status = B_SetAlgorithmInfo (enc,
                                             AI_DES_CBCPadIV8,
                                             (POINTER)(iv->data))) break;
	    status = B_SetKeyInfo(keyObj,
                                  KI_DES8,
                                  (POINTER)(key->data));
            break;
        case sec_bsafe_enc_des_PEM:
            if (status = B_SetAlgorithmInfo (enc,
                                             AI_DES_CBCPadPEM,
                                             (POINTER)(iv->data))) break;
	    status = B_SetKeyInfo(keyObj,
                                  KI_DES8,
                                  (POINTER)(key->data));
            break;
        case sec_bsafe_enc_des_nopad:
            if (status = B_SetAlgorithmInfo (enc,
                                             AI_DES_CBC_IV8,
                                             (POINTER)(iv->data))) break;
	    status = B_SetKeyInfo(keyObj,
                                  KI_DES8,
                                  (POINTER)(key->data));
            break;
        case sec_bsafe_enc_des_ede3:
            if (status = B_SetAlgorithmInfo (enc,
                                             AI_DES_EDE3_CBCPadIV8,
                                             (POINTER)(iv->data))) break;
	    status = B_SetKeyInfo(keyObj,
                                  KI_24Byte,
                                  (POINTER)(key->data));

            break;
        case sec_bsafe_enc_des_ede3_nopad:
            if (status = B_SetAlgorithmInfo (enc,
                                             AI_DES_EDE3_CBC_IV8,
                                             (POINTER)(iv->data))) break;
	    status = B_SetKeyInfo(keyObj,
                                  KI_24Byte,
                                  (POINTER)(key->data));
            break;
        default:
            status = BE_ALG_OPERATION_UNKNOWN;
            break;
        }

        keyItem.data = NULL;
        keyItem.len = 0;

        if (status) break;

         res_buffer_size = plaintext->len + maxpad;
         CREATE_STORAGE(ciphertext, res_buffer_size);

          if (status = B_EncryptInit(enc,
                                     keyObj,
                                     RSA_ALGORITHM_CHOOSER,
                                     NULL)) break;

          LOCK_RANDOMALG;
          status = B_EncryptUpdate(enc,
                                   ciphertext->data,
                                   &res_length,
                                   res_buffer_size,
                                   plaintext->data,
                                   (unsigned int)plaintext->len,
                                   randomAlgorithm,
                                   NULL);
          UNLOCK_RANDOMALG;

          if (status) break;

          LOCK_RANDOMALG;
          status = B_EncryptFinal(enc,
                                  ciphertext->data+res_length,
                                  &final_outlen,
                                  res_buffer_size-res_length,
                                  randomAlgorithm,
                                  NULL);
          UNLOCK_RANDOMALG;

          if (status) break;

          ciphertext->len = res_length + final_outlen;
    } while (0);

    if (enc) B_DestroyAlgorithmObject(&enc);
    if (keyObj) B_DestroyKeyObject(&keyObj);
    if (status == 0) return 0;

    CLEANUP_DESCRIPTOR(ciphertext);
    return sec_rsa_to_dce_status(status);

}

error_status_t sec_bsafe_symmetric_decrypt (
    sec_bsafe_alg_type_t alg,            /* [in] */
    sec_pk_gen_data_t       *key,        /* [in] */
    sec_pk_gen_data_t       *iv,         /* [in] */
    sec_pk_gen_data_t       *ciphertext, /* [in] */
    sec_pk_gen_data_t       *plaintext   /* [out] */
) {

    B_ALGORITHM_OBJ enc = NULL;
    ITEM keyItem = {0,0};
    B_KEY_OBJ keyObj = NULL;
    int status = 0;
    error_status_t estatus;
    unsigned int res_length;
    unsigned int final_outlen;
    size_t res_buffer_size;

    unsigned32 key_length;
    unsigned32 iv_length;
    unsigned32 chunksize;

    CRYPTO_INIT(NULL);

    CREATE_DESCRIPTOR(plaintext);

    estatus = sec_bsafe_symmetric_alginfo (alg,
                                           &key_length,
                                           &iv_length,
                                           &chunksize,
                                           NULL);
    if (estatus) return estatus;

    /* sanity-check input lengths... */

    if (key->len != key_length) return sec_s_bsafe_key_len;

    if (alg != sec_bsafe_enc_des_PEM) {
        if (iv->len != iv_length) return sec_s_bsafe_input_len;
    };
    /* Don't bother sanity-checking PEM length as it's not worth the effort */

    if ((ciphertext->len % chunksize) != 0) return sec_s_bsafe_input_len;

    do {

        keyItem.data = key->data;
        keyItem.len = key->len;

        if (status = B_CreateAlgorithmObject(&enc)) break;

	if (status = B_CreateKeyObject(&keyObj)) break;

        switch (alg) {

        case sec_bsafe_enc_des:
            if (status = B_SetAlgorithmInfo (enc,
                                             AI_DES_CBCPadIV8,
                                             (POINTER)(iv->data))) break;
	    status = B_SetKeyInfo(keyObj,
                                  KI_DES8,
                                  (POINTER)(key->data));
            break;
        case sec_bsafe_enc_des_PEM:
            if (status = B_SetAlgorithmInfo (enc,
                                             AI_DES_CBCPadPEM,
                                             (POINTER)(iv->data))) break;
	    status = B_SetKeyInfo(keyObj,
                                  KI_DES8,
                                  (POINTER)(key->data));
            break;
        case sec_bsafe_enc_des_nopad:
            if (status = B_SetAlgorithmInfo (enc,
                                             AI_DES_CBC_IV8,
                                             (POINTER)(iv->data))) break;
	    status = B_SetKeyInfo(keyObj,
                                  KI_DES8,
                                  (POINTER)(key->data));
            break;
        case sec_bsafe_enc_des_ede3:
            if (status = B_SetAlgorithmInfo (enc,
                                             AI_DES_EDE3_CBCPadIV8,
                                             (POINTER)(iv->data))) break;
	    status = B_SetKeyInfo(keyObj,
                                  KI_24Byte,
                                  (POINTER)(key->data));

            break;
        case sec_bsafe_enc_des_ede3_nopad:
            if (status = B_SetAlgorithmInfo (enc,
                                             AI_DES_EDE3_CBC_IV8,
                                             (POINTER)(iv->data))) break;
	    status = B_SetKeyInfo(keyObj,
                                  KI_24Byte,
                                  (POINTER)(key->data));
            break;
        default:
            status = BE_ALG_OPERATION_UNKNOWN;
            break;
        }

        keyItem.data = NULL;
        keyItem.len = 0;

        if (status) break;

        res_buffer_size = ciphertext->len;
        CREATE_STORAGE(plaintext, res_buffer_size);

        if (status = B_DecryptInit(enc,
                                   keyObj,
                                   RSA_ALGORITHM_CHOOSER,
                                   NULL)) break;

        LOCK_RANDOMALG;
        status = B_DecryptUpdate(enc,
                                 plaintext->data,
                                 &res_length,
                                 res_buffer_size,
                                 ciphertext->data,
                                 (unsigned int)ciphertext->len,
                                 randomAlgorithm,
                                 NULL);
        UNLOCK_RANDOMALG;
        if (status) break;
        LOCK_RANDOMALG;
        status = B_DecryptFinal(enc,
                                plaintext->data+res_length,
                                &final_outlen,
                                res_buffer_size-res_length,
                                randomAlgorithm,
                                NULL);
        UNLOCK_RANDOMALG;
        if (status) break;
        plaintext->len = res_length + final_outlen;

    } while (0);

    if (enc) B_DestroyAlgorithmObject(&enc);
    if (keyObj) B_DestroyKeyObject(&keyObj);
    if (status == 0) return 0;
    CLEANUP_DESCRIPTOR(plaintext);

    return sec_rsa_to_dce_status(status);

}

error_status_t sec_bsafe_random_bytes (
    sec_pk_gen_data_t       *seed,
    sec_pk_gen_data_t       *buffer) {

    int status = 0;

    CRYPTO_INIT(seed);

    LOCK_RANDOMALG;
    status = B_GenerateRandomBytes(randomAlgorithm,
                                   buffer->data,
                                   buffer->len,
                                   NULL);

    UNLOCK_RANDOMALG;
    return sec_rsa_to_dce_status(status);
}

error_status_t sec_bsafe_randomize (
    sec_pk_gen_data_t       *seed) {

    CRYPTO_INIT(seed);
    return 0;
}

