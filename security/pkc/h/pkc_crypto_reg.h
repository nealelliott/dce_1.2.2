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
 * $Log: pkc_crypto_reg.h,v $
 * Revision 1.1.2.2  1996/11/13  20:38:26  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:24 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 *
 * Revision 1.1.2.1  1996/10/03  20:28:27  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:41:46  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:25 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:10 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */



#ifndef PKC_CRYPTO_REG_
#define PKC_CRYPTO_REG_

extern "C" {
#include <stdlib.h>
#include <dce/dce.h>
#include <dce/dcepkcmsg.h>
#include <dce/gssapi.h>
#include <dce/sec_pk_base.h>
}

#include <dce/pkc_common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char threadsafe;    /* non-zero if sign() or verify() may be    */
                        /* may be called simultaneously (within a   */
                        /* single crypto session) by multiple       */
                        /* threads.                                 */
    char multi_session; /* non-zero if the crypto implementation    */
                        /* supports multiple simultaneous crypto    */
                        /* sessions.                                */
} pkc_alg_flags_t;


/* This data-type is used to register a new crypto-module with the  */
/* certification API.  It describes a crypto algorithm, and         */
/* provides entry-points to its sign/verify functions.              */

typedef struct {
    /* The version and alg_id fields are required for all versions  */
    /* of this data-structure.  Other fields may be                 */
    /* version-dependent.                                           */
    OM_uint32 version;      /* must be pkc_V1 for now               */
    gss_OID_desc alg_id;
    /* Object identifier that names the algorithm - the OID that    */
    /* appears in certificates signed by the algorithm.             */

    pkc_alg_flags_t flags;
    /* Describes the algorithm implementation capabilities          */

    char reserved[32 - sizeof(pkc_alg_flags_t)];
    /* Reserve space for 32 bytes of flag data.                     */

    char * (* name)(void);
    /* Returns the algorithm name.  This will be used in	    */
    /* diagnostic or auditing messages.  The name should be	    */
    /* returned in storage allocated using the pkc_alloc()	    */
    /* function defined in pkc_base.h.  This is the only	    */
    /* crypto-module routine that may be called without having	    */
    /* first called the <open> routine.				    */


    /* NULL may be supplied as the address of any of the following  */
    /* routines, if the crypto module does not provide or require   */
    /* the corresponding feature.  All crypto modules should        */
    /* provide a <verify> function; other functions are optional.   */

    unsigned32 (*open) (void** context);
    unsigned32 (*close) (void** context);
    /* Before invoking any encryption routines (eg <sign> or        */
    /* <verify>), a caller will establish a crypto session with the */
    /* algorithm implementation by invoking the implementation's    */
    /* open() function.  If the crypto module maintains state       */
    /* information between calls, it may use the context parameter. */
    /* This same context parameter will be passed to subsequent     */
    /* sign(), verify() and close() calls made by the certification */
    /* facility.  Note that once close() has been invoked, the      */
    /* certification facility will invoke open() again before       */
    /* making any further calls to the crypto module.  If the       */
    /* open() routine stores any state in the context parameter,    */
    /* the close() routine should free this storage.                */

    unsigned32 (*verify) (void ** context,
                          sec_pk_gen_data_t data,
                          sec_pk_data_t public_key,
                          sec_pk_data_t signature);
    /* A routine that checks the supplied signature against the     */
    /* supplied data.  <public_key> is a SubjectPublicKeyInfo data  */
    /* structure, encoded in BER, as found within an X.509          */
    /* certificate.  The routine should return 0 for a correct      */
    /* signature, pkc_invalid_signature for an incorrect signature, */
    /* or another DCE-defined error status to indicate any other    */
    /* errors.                                                      */


    unsigned32 (*sign) (void ** context,
                        sec_pk_gen_data_t data,
                        sec_pk_data_t private_key,
                        sec_pk_data_t * signature);
    /* A routine that calculates a signature over the supplied data */
    /* using the specified key.  The <private_key> parameter will   */
    /* be a BER-encoded PrivateKeyInfo data object, as defined in   */
    /* PKCS #7. The <signature> field should be returned by the     */
    /* function; storage allocation should be performed by calling  */
    /* the pkc_alloc() & pkc_free() functions defined above.        */
    /* Crypto modules are not required to provide a sign function.  */   


    unsigned32 (*generate_keypair) (void ** context,
                                    unsigned32 size,
                                    void * alg_info,
                                    sec_pk_data_t * private_key,
                                    sec_pk_data_t * public_key);
    /* A routine that generates a keypair.  The <size> parameter    */
    /* should be used by the routine to determine the key-size in   */
    /* some way (for the RSA algorithm, for example, it should be   */
    /* treated as the number of bits in the key modulus).  The      */
    /* <private_key> and <public-key> parameters should return      */
    /* BER-encoded PrivateKeyInfo and SubjectPublicKeyInfo data     */
    /* objects respectively.  The alg_info parameter can be used    */
    /* for algorithm-specific information to modify the             */
    /* key-generation process, but all crypto modules that offer    */
    /* this function should be prepared to operate when NULL is     */
    /* provided for this parameter.                                 */
    /* Crypto modules are not required to provide a                 */
    /* generate_keypair function.                                   */



} pkc_signature_algorithm_t;

#define PKC_REPLACE_NONE 0
#define PKC_REPLACE_ENTRYPOINTS 1
#define PKC_REPLACE_ALL 2

unsigned32 pkc_crypto_register_signature_alg(
                        pkc_signature_algorithm_t * alg,
                        int replacement_policy);

    /* Register an implementation of a crytptographic algorithm.    */
    /* The <replacement_policy> argumentdetermines how the PKC      */
    /* facility will handle the case where an implementation of the */
    /* algorithm is already registered:                             */
    /*                                                              */
    /*     PKC_REPLACE_NONE:    An error will be returned if an     */
    /*                          implemenation of the algorithm is   */
    /*                          already registered.                 */
    /*     PKC_REPLACE_ENTRYPOINTS: Only entrypoints that the       */
    /*                          original implementation did not     */
    /*                          provide will be replaced.           */
    /*     PKC_REPLACE_ALL:     The new implementation will replace */
    /*                          the existing one.                   */
    /* The use of PKC_REPLACE_ENTRYPOINTS is not currently          */
    /* supported.                                                   */


unsigned32 pkc_crypto_lookup_algorithm(
                            gss_OID oid,
                            pkc_signature_algorithm_t * details);
    /* The preferred way of obtaining information about a           */
    /* registered algorithm implementation, this function returns   */
    /* an algorithm implementation descriptor block for the         */
    /* specified algorithm, and leaves the algorithm list unlocked. */
    /* The complete list of registered algorithms may be obtained   */
    /* by invoking pkc_get_registered_algorithms().                 */

unsigned32 pkc_crypto_get_registered_algorithms(gss_OID_set * oid_set);
    /* Returns an OID-set describing the currently-registered       */
    /* algorithm implementations.                                   */



unsigned32 pkc_crypto_verify_signature(gss_OID algorithm,
                                       sec_pk_gen_data_t data,
                                       sec_pk_data_t public_key,
                                       sec_pk_data_t signature);

unsigned32 pkc_crypto_sign(gss_OID algorithm,
                           sec_pk_gen_data_t data,
                           sec_pk_data_t private_key,
                           sec_pk_data_t * signature);

unsigned32 pkc_crypto_generate_keypair (gss_OID algorithm,
                                        unsigned32 size,
                                        void * alg_info,
                                        sec_pk_data_t * private_key,
                                        sec_pk_data_t * public_key);

    /* The above functions search the list of registered algorithms */
    /* for an implementation of the specified algorithm.  If found, */
    /* the implementation will be opened if necessary and its sign  */
    /* or verify member function will be invoked.                   */


#ifdef __cplusplus
}
#endif

#endif



