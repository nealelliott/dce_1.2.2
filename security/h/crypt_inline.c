/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crypt_inline.c,v $
 * Revision 1.1.51.2  1996/02/18  00:08:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:28  marty]
 *
 * Revision 1.1.51.1  1995/12/13  16:24:32  root
 * 	Submit
 * 	[1995/12/11  15:14:16  root]
 * 
 * Revision 1.1.49.3  1994/06/02  21:57:38  mdf
 * 	Merged with changes from 1.1.49.2
 * 	[1994/06/02  21:56:46  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:14:30  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * Revision 1.1.49.2  1994/06/02  20:21:25  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:14:30  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * Revision 1.1.47.1  1993/10/05  22:30:20  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:08:47  mccann]
 * 
 * Revision 1.1.3.3  1992/12/29  13:06:25  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:14  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  21:17:35  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:21:19  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:42:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  crypt_inline.c V=2 10/03/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
#ifdef __crypt_inline_included__
#else
#define __crypt_inline_included__

/* 
 *      Security Service (de/en)cryption API
 *
 *  As it is not permissible to provide public entry points for
 *  encryption routines in libraries, any security module that consumes these
 *  routines and will be part of a public library must have these routines
 *  inline and private.  This is accomplished by including crypt.h without
 *  defining the preprocessor variable SEC_CRYPT_PUBLIC_ENTRY.  BE CERTAIN
 *  that you do not  #define SEC_CRYPT_PUBLIC_ENTRY if you don't want public
 *  encryption entry points.
 *
 *  modules that "inline" this file should be sure to #include the 
 *  following .h files
 *  
 * #include <un_maloc.h>
 * #include <bstring.h>
 * #include <rgymacro.h>
 * #include <dce/passwd.h>
 * #include <dce/sec_base.h>
 * #include <krb5/krb5.h>  
 * #include <krb5/encryption.h>
 */

#define ALLOCATE(allocator, size) \
         (allocator ? (*(allocator))((size)) : (Pointer)malloc((size)))
#define DEALLOCATE(deallocator, ptr) \
         { if (deallocator) (*(deallocator))((ptr)); else free((ptr)); }
            

#ifdef  SEC_CRYPT_PUBLIC_ENTRY
#    define  SEC_CRYPT_SCOPE  PUBLIC
#else
#    define SEC_CRYPT_SCOPE   PRIVATE
#endif

typedef struct  _sec_crypt_context_t {
    krb5_keyblock       *ekey;
    krb5_enctype        etype;
    krb5_encrypt_block  eblock;
} sec_crypt_context_t;
    
SEC_CRYPT_SCOPE  void  sec_crypt_encrypt
#ifndef __STDC__
     ( context, allocator, deallocator, plen, ptext, clen, ctext, chksum,  stp )     
     sec_crypt_handle_t  context;
     Pointer             (*allocator)();
     void                (*deallocator)();
     unsigned32          plen;     /* [in] - length of plaintext */
     Pointer             ptext;    /* [in] - plaintext */
     unsigned32          *clen;    /* [out] - size of output buffer */
     Pointer             *ctext;   /* [out] - ciphertext */
     sec_chksum_t        *chksum;  /* [in/out] -  a checksum of plaintext */
     error_status_t      *stp;
#else
  (
     sec_crypt_handle_t  context,
     Pointer             (*allocator)(unsigned32  size),
     void                (*deallocator)(void * ptr),
     unsigned32          plen,     /* [in] - length of plaintext */
     Pointer             ptext,    /* [in] - plaintext */
     unsigned32          *clen,    /* [out] - size of output buffer */
     Pointer             *ctext,   /* [out] - ciphertext */
     sec_chksum_t        *chksum,  /* [in/out] -  a checksum of plaintext */
     error_status_t      *stp
  )
#endif
{
     Pointer             bytes;
     boolean32           free_bytes = false;

     sec_crypt_context_t *cc = (sec_crypt_context_t *) context;

     *clen = krb5_encrypt_size(plen, cc->eblock.crypto_entry);
     if (*clen != plen) {
         bytes = ALLOCATE(allocator, *clen);
         if (bytes == NULL) {
             SET_STATUS(stp, sec_rgy_cant_allocate_memory);
             return;
         } else {
             free_bytes = true;
             /* zero out any padding */
             memset((char *)bytes + plen, 0, *clen - plen);
             memcpy(bytes, ptext,  plen);
             *ctext = ALLOCATE(allocator, *clen);
             if (*ctext == NULL) {
                 SET_STATUS(stp, sec_rgy_cant_allocate_memory);
                 DEALLOCATE(deallocator, bytes);
                 return;
             }
         }
     } else {
         bytes = ptext;
     }

     CLEAR_STATUS(stp);

     /* 
      * compute a checksum before encrypting, if the caller requested one.
      * We need to compute this over the (possibly) expanded, padded 
      * plaintext because that's all we know about on the decryption side 
      */
     if (chksum != NULL) {
         sec_crypt_compute_checksum(context, *clen, (unsigned_char_p_t) bytes,
                                    chksum, stp);
     }

     if (BAD_STATUS(stp)) {
         DEALLOCATE(deallocator, *ctext);
         if (free_bytes) {
             DEALLOCATE(deallocator, bytes);
         }
         return;
     }
         
     /*
      * The encryption routine takes the unexpanded plaintext length and
      * the expanded plaintext buffer.  It recalculates the expanded size
      * internally and assumes that the plaintext buffer is the right size.
      */
     *stp = krb5_encrypt((krb5_pointer) bytes, (krb5_pointer) *ctext,
                         plen, &cc->eblock, 0);
     
    if (BAD_STATUS(stp)) {
         DEALLOCATE(deallocator, *ctext);
         *clen = 0;
         if (chksum != NULL) {
             sec_crypt_free_cksum_contents(chksum);
         }
     }

     if (free_bytes) {
         DEALLOCATE(deallocator, bytes);
     }
 }

SEC_CRYPT_SCOPE  void  sec_crypt_decrypt
#ifndef __STDC__
     ( context, allocator, deallocator, chksum, clen, ctext, ptext, stp )        
     sec_crypt_handle_t  context;
     Pointer             (*allocator)();
     void                (*deallocator)();
     sec_chksum_t        *chksum;  /* in-  a checksum of pre-encrypted ctext */
     unsigned32          clen;     /* in - size of input/output buffer */
     Pointer             ctext;    /* out - bytes to be decrypted */
     Pointer             *ptext;   /* out - decrypted plaintext  */
     error_status_t      *stp;
#else
  (
     sec_crypt_handle_t  context,
     Pointer             (*allocator)(unsigned32  size),
     void                (*deallocator)(Pointer  ptr),
     sec_chksum_t        *chksum,  /* in-  a checksum of pre-encrypted ctext */
     unsigned32          clen,     /* in - size of input/output buffer */
     Pointer             ctext,    /* out - bytes to be decrypted */
     Pointer             *ptext,   /* out - decrypted plaintext  */
     error_status_t      *stp
  )
#endif
{
     sec_chksum_t        sum;

     sec_crypt_context_t *cc = (sec_crypt_context_t *) context;

     *ptext = ALLOCATE(allocator, clen);
     if (*ptext == NULL) {
         SET_STATUS(stp, sec_rgy_cant_allocate_memory);
         return;
     }

     CLEAR_STATUS(stp);

     *stp = krb5_decrypt((krb5_pointer) ctext, (krb5_pointer) *ptext, clen, 
                         &cc->eblock, 0);

     /*
      * Translate kerberos error code to appropriate error status.
      * Note! : this is a temporary fix until sec_authn restructuring is done.
      */
     if (STATUS_EQUAL(stp, KRB5KRB_AP_ERR_BAD_INTEGRITY))
         SET_STATUS(stp, sec_rgy_bad_integrity);
     
     if (GOOD_STATUS(stp) && chksum != NULL) {
         sum.chksum_type = chksum->chksum_type;
         sec_crypt_compute_checksum(context, clen, (unsigned_char_t *) *ptext, &sum, stp);
         if (GOOD_STATUS(stp)) {
             if (!sec_crypt_checksum_equal(chksum, &sum)) {
                 SET_STATUS(stp, sec_rgy_bad_integrity); 
             }
             sec_crypt_free_cksum_contents(&sum);
         }
     }
 
     if (BAD_STATUS(stp)) {
         DEALLOCATE(deallocator, *ptext);
         *ptext = NULL;
     }
 }
#endif
