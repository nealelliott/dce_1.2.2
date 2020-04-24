/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: desglue.c,v $
 * Revision 1.1.10.1  1996/06/04  22:02:34  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:30 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	If these sources are built with -DMIT_KRB5 compiler define, adjust
 * 	routines and structures to build with include files in
 * 	/dce/src/mit_krb5/include.
 * 	[1994/09/21  22:01:29  burati  1.1.6.1]
 *
 * Revision 1.1.8.2  1996/02/18  00:09:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:34  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  17:48:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:10  root]
 * 
 * Revision 1.1.6.1  1994/09/21  22:01:29  burati
 * 	CR10924 Change memcpy() to memmove() in mit_des_decrypt_func()
 * 	[1994/09/21  19:16:51  burati]
 * 
 * Revision 1.1.4.2  1992/12/29  14:22:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:04:05  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/15  20:57:20  pato
 * 	Initial revision
 * 	[1992/05/14  15:22:26  pato]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
*/

/* desglue.c - provides MIT calling interface to Dennis Ferguson's DESTOO */
/* Mark W. Eichin - HP/Apollo [eichin:19901120.0625EST] */

/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/crypto/common/desglue.c,v $
 * $Author: arvind $
 *
 * Copyright 1985, 1986, 1987, 1988, 1990, 1991 by the Massachusetts Institute
 * of Technology.
 * All Rights Reserved.
 *
 * Under U.S. law, this software may not be exported outside the US
 * without license from the U.S. Commerce department.
 *
 * These routines form the library interface to the DES facilities.
 *
 * Based on code originally written 8/85 by Steve Miller, MIT Project Athena.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * DES encryption interface file
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)desglue.c	6 - 10/19/91";
#endif

#include <krb5/copyright.h>

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include <krb5/crc-32.h>

#include <krb5/mit-des.h>
#include <krb5/des_int.h>

/* krb5_error_code mit_des_encrypt_func (in, out, sz, eb, iv) */
static krb5_error_code
mit_des_encrypt_f(DECLARG(krb5_const_pointer, in),
                  DECLARG(krb5_pointer, out),
                  DECLARG(const size_t, size),
                  DECLARG(krb5_encrypt_block *, key),
                  DECLARG(krb5_pointer, ivec))
OLDDECLARG(krb5_const_pointer, in)
OLDDECLARG(krb5_pointer, out)
OLDDECLARG(const size_t, size)
OLDDECLARG(krb5_encrypt_block *, key)
OLDDECLARG(krb5_pointer, ivec)
{
    krb5_octet  *iv;
    
    if ( ivec == 0 )
        iv = key->key->contents;
    else
        iv = (krb5_octet *)ivec;

    return (mit_des_cbc_encrypt((krb5_octet *)in, 
                            (krb5_octet *)out,
                            size, 
                            (struct mit_des_ks_struct *)key->priv, 
                            iv,
                            MIT_DES_ENCRYPT));
}    

krb5_error_code mit_des_encrypt_func(DECLARG(krb5_const_pointer, in),
                                     DECLARG(krb5_pointer, out),
                                     DECLARG(const size_t, size),
                                     DECLARG(krb5_encrypt_block *, key),
                                     DECLARG(krb5_pointer, ivec))
OLDDECLARG(krb5_const_pointer, in)
OLDDECLARG(krb5_pointer, out)
OLDDECLARG(const size_t, size)
OLDDECLARG(krb5_encrypt_block *, key)
OLDDECLARG(krb5_pointer, ivec)
{
    krb5_checksum cksum;
    krb5_octet  contents[CRC32_CKSUM_LENGTH];
    int sumsize;
    krb5_error_code retval;

/*    if ( size < sizeof(mit_des_cblock) )
        return KRB5_BAD_MSIZE; */

    /* caller passes data size, and saves room for the padding. */
    /* format of ciphertext, per RFC is:
      +-----------+----------+-------------+-----+
      |confounder |   check  |   msg-seq   | pad |
      +-----------+----------+-------------+-----+
      
      our confounder is 8 bytes (one cblock);
      our checksum is CRC32_CKSUM_LENGTH
     */
    sumsize =  krb5_roundup(size+CRC32_CKSUM_LENGTH+sizeof(mit_des_cblock),
                            sizeof(mit_des_cblock));

    /* assemble crypto input into the output area, then encrypt in place. */

    memset((char *)out, 0, sumsize);

    /* put in the confounder */
    if (retval = krb5_random_confounder(sizeof(mit_des_cblock), out))
        return retval;

    memcpy((char *)out+sizeof(mit_des_cblock)+CRC32_CKSUM_LENGTH, (char *)in,
           size);

    cksum.contents = contents; 
#ifdef MIT_KRB5
    /*
     * Use newer prototype if being built from /dce/src/mit_krb5/lib/krb5
     */ 
    if (retval = krb5_calculate_checksum(NULL, CKSUMTYPE_CRC32,
                                         (krb5_pointer) out,
                                         sumsize,
                                         (krb5_pointer)key->key->contents,
                                         sizeof(mit_des_cblock),
                                         &cksum)) 
#else
    if (retval = krb5_calculate_checksum(CKSUMTYPE_CRC32,
                                         (krb5_pointer) out,
                                         sumsize,
                                         (krb5_pointer)key->key->contents,
                                         sizeof(mit_des_cblock),
                                         &cksum)) 
#endif
        return retval;

    memcpy((char *)out+sizeof(mit_des_cblock), (char *)contents,
           CRC32_CKSUM_LENGTH);

    /* We depend here on the ability of this DES implementation to
       encrypt plaintext to ciphertext in-place. */
    return (mit_des_encrypt_f(out, out, sumsize, key, ivec));
}

/* krb5_error_code mit_des_decrypt_func (in, out, sz, eb, iv) */
/*

        decrypts "size" bytes at "in", storing result in "out".
        "eblock" points to an encrypt block which has been initialized
        by process_key().

        "out" must be preallocated by the caller to contain sufficient
        storage to hold the output; this is guaranteed to be no more than
        the input size.

        returns: errors

 */
static krb5_error_code
mit_des_decrypt_f(DECLARG(krb5_const_pointer, in),
                  DECLARG(krb5_pointer, out),
                  DECLARG(const size_t, size),
                  DECLARG(krb5_encrypt_block *, key),
                  DECLARG(krb5_pointer, ivec))
OLDDECLARG(krb5_const_pointer, in)
OLDDECLARG(krb5_pointer, out)
OLDDECLARG(const size_t, size)
OLDDECLARG(krb5_encrypt_block *, key)
OLDDECLARG(krb5_pointer, ivec)
{
    krb5_octet  *iv;

    if ( ivec == 0 )
        iv = key->key->contents;
    else
        iv = (krb5_octet *)ivec;

    return (mit_des_cbc_encrypt ((krb5_octet *)in, 
                             (krb5_octet *)out, 
                             size, 
                             (struct mit_des_ks_struct *)key->priv, 
                             iv,
                             MIT_DES_DECRYPT));
}    

krb5_error_code mit_des_decrypt_func(DECLARG(krb5_const_pointer, in),
                                     DECLARG(krb5_pointer, out),
                                     DECLARG(const size_t, size),
                                     DECLARG(krb5_encrypt_block *, key),
                                     DECLARG(krb5_pointer, ivec))
OLDDECLARG(krb5_const_pointer, in)
OLDDECLARG(krb5_pointer, out)
OLDDECLARG(const size_t, size)
OLDDECLARG(krb5_encrypt_block *, key)
OLDDECLARG(krb5_pointer, ivec)
{
    krb5_checksum cksum;
    krb5_octet  contents_prd[CRC32_CKSUM_LENGTH];
    krb5_octet  contents_get[CRC32_CKSUM_LENGTH];
    char        *p;
    krb5_error_code   retval;

    if ( size < 2*sizeof(mit_des_cblock) )
        return KRB5_BAD_MSIZE;

    if (retval = mit_des_decrypt_f(in, out, size, key, ivec))
        return retval;

    cksum.contents = contents_prd;
    p = (char *)out + sizeof(mit_des_cblock);
    memcpy((char *)contents_get, p, CRC32_CKSUM_LENGTH);
    memset(p, 0, CRC32_CKSUM_LENGTH);

#ifdef MIT_KRB5
    /*
     * Use newer prototype if being built from /dce/src/mit_krb5/lib/krb5
     */
    if (retval = krb5_calculate_checksum(NULL, CKSUMTYPE_CRC32,
                                         out,
                                         size,
                                         (krb5_pointer)key->key->contents,
                                         sizeof(mit_des_cblock),
                                         &cksum)) 
#else
    if (retval = krb5_calculate_checksum(CKSUMTYPE_CRC32,
                                         out,
                                         size,
                                         (krb5_pointer)key->key->contents,
                                         sizeof(mit_des_cblock),
                                         &cksum)) 
#endif
        return retval;

    if (memcmp((char *)contents_get, (char *)contents_prd, CRC32_CKSUM_LENGTH) )
        return KRB5KRB_AP_ERR_BAD_INTEGRITY;

    memmove((char *)out, (char *)out +
           sizeof(mit_des_cblock) + CRC32_CKSUM_LENGTH,
           size - (sizeof(mit_des_cblock) + CRC32_CKSUM_LENGTH));
    memset((char *)(out) + size - (sizeof(mit_des_cblock) + CRC32_CKSUM_LENGTH),
           0, sizeof(mit_des_cblock) + CRC32_CKSUM_LENGTH);

    return 0;
}

krb5_cryptosystem_entry mit_des_cryptosystem_entry = {
#ifdef MIT_KRB5
    0,
#endif
    mit_des_encrypt_func,
    mit_des_decrypt_func, 
    mit_des_process_key,
    mit_des_finish_key,
    mit_des_string_to_key,
    mit_des_init_random_key,
    mit_des_finish_random_key,
    mit_des_random_key,
    sizeof(mit_des_cblock),
    CRC32_CKSUM_LENGTH+sizeof(mit_des_cblock),
    sizeof(mit_des_cblock),
    ETYPE_DES_CBC_CRC,
    KEYTYPE_DES
    };

krb5_cs_table_entry krb5_des_cst_entry = {
#ifdef MIT_KRB5
    0,
#endif
    &mit_des_cryptosystem_entry,
    0
    };

extern krb5_error_code mit_des_cbc_checksum PROTOTYPE ((krb5_pointer ,
                                                        size_t ,
                                                        krb5_pointer ,
                                                        size_t ,
                                                        krb5_checksum * ));

krb5_checksum_entry mit_des_cbc_cksumtable_entry = {
#ifdef MIT_KRB5
    0,
#endif
    mit_des_cbc_checksum,
    sizeof(mit_des_cblock),
    1,                                  /* is collision proof */
    1,                                  /* is keyed */
};

int mit_des_key_sched(
    DECLARG(mit_des_cblock, k),
    DECLARG(mit_des_key_schedule, s))
OLDDECLARG(mit_des_cblock, k)
OLDDECLARG(mit_des_key_schedule, s)
{
#ifndef DES_NEUTER

    if (!mit_des_check_key_parity(k))   /* bad parity --> return -1 */
        return(-1);

    if (mit_des_is_weak_key(k))
        return(-2);

#endif

    return make_key_sched(k,s);
}
