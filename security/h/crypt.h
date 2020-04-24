/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crypt.h,v $
 * Revision 1.1.4.2  1996/02/18  22:58:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:50  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:24:30  root
 * 	Submit
 * 	[1995/12/11  15:14:15  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:06:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:17  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  crypt.h V=1 07/31/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * 
 */

#ifdef __crypt_h_included__
#else
#define __crypt_h_included__

#include <dce/lbase.h>
#include <dce/sec_base.h>
#include <dce/passwd.h>
#include <Pointer.h>

typedef  Pointer  sec_crypt_handle_t;
                
/*
 * compute ciphertext key from plaintext input.
 */
void  sec_crypt_compute_key (
#ifdef __STDC__
   sec_passwd_rec_t    *plain,
   sec_passwd_type_t   cipher_type, /* type of ciphertext to generate */
   sec_passwd_rec_t    *cipher,
   error_status_t      *stp
#endif
);

/* 
 * create an encryption context 
 */
sec_crypt_handle_t  sec_crypt_create_context (
#ifdef __STDC__
   sec_passwd_rec_t    *cipher,  /* must be ciphertext */
   sec_etype_t         etype,    /* requested encryption type */
   error_status_t      *stp
#endif
);

void  sec_crypt_destroy_context (
#ifdef __STDC__
      sec_crypt_handle_t  *crypt_context
#endif
);


void  sec_crypt_compute_checksum (
#ifdef __STDC__
    sec_crypt_handle_t  context,
    unsigned32          len,
    unsigned_char_t     *data,
    sec_chksum_t        *checksum,
    error_status_t      *stp
#endif
);
                
boolean32  sec_crypt_checksum_equal (
#ifdef __STDC__
    sec_chksum_t        *checksum1,
    sec_chksum_t        *checksum2
#endif
);

/*
 * frees a pointer to a sec_chksum_t after freeing any allocated
 * field storage associated with the type.  To free just the fields, 
 * use sec_crypt_free_checksum_contents
 */
void  sec_crypt_free_checksum (
#ifdef __STDC__
    sec_chksum_t        **checksum
#endif
);
                
void  sec_crypt_free_cksum_contents (
#ifdef __STDC__
    sec_chksum_t        *checksum
#endif
);
   

/* s e c _ c r y p t _ f r e e _ k e y
 *
 * free any storage associated with a sec_passwd_rec_t.  Pointer values
 * are freed if they're non-null.  ciphertext arrays are zeroed out. 
 * Be careful about the prefix field: the routine  calls free() on
 * it if it is non-NULL.  The routine frees the storage for "key"
 * as well. If "key" is not pointer to heap storage, you need
 * to call sec_crypt_free_key_contents instead.
 */
void  sec_crypt_free_key (
#ifdef __STDC__
   sec_passwd_rec_t    **key
#endif
);

void  sec_crypt_free_key_contents (
#ifdef __STDC__
    sec_passwd_rec_t    *key
#endif
);

/* 
 * Public entry points exist for the following functions exists only if crypt.c has
 * been compiled with -DSEC_CRYPT_PUBLIC_ENTRY and bound into the application.
 * Public entry points for these en/de-cryption functions MUST NOT APPEAR in the 
 * security library.  Any client side module that requires access to these routines
 * should NOT define SEC_CRYPT_PUBLIC_ENTRY
 * 
 * both routines take allocator/deallocator function parameters.  If either
 * or both pointers are NULL, the standard system routines are used.
 */
#ifdef SEC_CRYPT_PUBLIC_ENTRY

void  sec_crypt_encrypt (                     
#ifdef __STDC__
    sec_crypt_handle_t  context,
    Pointer             (*allocator)(unsigned32 size),
    void                (*deallocator)(Pointer  ptr),
    unsigned32          plen,     /* [in] length of plaintext to be encrypted */
    Pointer             ptext,    /* [in] plaintext tobe encrypted */
    unsigned32          *elen,    /* [out] size of encrypted output buffer */
    Pointer             *ebytes,  /* [out] encrypted bytes */
    sec_chksum_t        *chksum,  /* [in,out] if non-NULL, a checksum of type
				   * requested in the chksum_type
				   * field is computed over the 
				   * input before encryption, 
				   * otherwise this parameter
				   * is ignored
				   */
    error_status_t      *stp
#endif
);

void  sec_crypt_decrypt (
#ifdef __STDC__
    sec_crypt_handle_t  context,
    Pointer             (*allocator)(unsigned32 size),
    void                (*deallocator)(Pointer  ptr),
    sec_chksum_t        *chksum,  /* [in] used for integrity: optional */
    unsigned32          elen,     /* [in] size of encrypted buffer. */
    Pointer             ebytes,   /* [in] bytes to be decrypted. */
    Pointer             *ptext,   /* [out] decrypted plaintext (length = elen) */
    error_status_t      *stp
#endif
);

#else 

/* If you need PRIVATE versions of the encryption and decryption routines your
 * module should contain the following includes  (the first four are lready 
 * included by crypt.h
 *
 * #include <dce/lbase.h>
 * #include <dce/sec_base.h>
 * #include <dce/passwd.h>
 * #include <Pointer.h>
 * #include <un_maloc.h>
 * #include <bstring.h>
 * #include <rgymacro.h>
 * #include <krb5/krb5.h>  
 * #include <krb5/encryption.h>
 * #include <crypt_inline.c>
 */
#endif /* ifdef SEC_CRYPT_PUBLIC_ENTRY */

#endif 

