/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: des_int.h,v $
 * Revision 1.1.9.2  1996/02/18  23:02:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:43  marty]
 *
 * Revision 1.1.9.1  1995/12/08  17:41:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:58  root]
 * 
 * Revision 1.1.7.1  1993/10/14  17:45:16  mccann
 * 	CR8651 64bit port
 * 	[1993/10/14  17:42:33  mccann]
 * 
 * Revision 1.1.5.2  1992/12/29  13:58:34  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:52:06  zeliff]
 * 
 * Revision 1.1.3.2  1992/06/01  20:44:30  sekhar
 * 	Drop the changes made by Joe Pato for restructured des code.
 * 	[1992/06/01  19:58:17  sekhar]
 * 
 * Revision 1.1.1.2  1992/05/14  15:35:19  pato
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
*/

/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/des_int.h,v $
 * $Author: marty $
 * $Id: des_int.h,v 1.1.9.2 1996/02/18 23:02:18 marty Exp $ 
 *
 * Derived from:
 * Source: /afs/athena.mit.edu/astaff/project/krb5/src/lib/des/RCS/des_int.h,v
 * Author: jtkohl
 * Id: des_int.h,v 5.12 91/06/06 09:39:07 jtkohl Exp
 *
 * Copyright 1987, 1988, 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
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
 * Private include file for the Data Encryption Standard library.
 */

/* only do the whole thing once	 */
#ifndef DES_INTERNAL_DEFS
#define DES_INTERNAL_DEFS

#include <krb5/mit-des.h>

/*
 * number of iterations of the inner
 * loop of the DES algorithm.  The
 * standard is 16, but in case that is
 * too slow, we might do less.  Of
 * course, less also means less
 * security.
 */
#define	 AUTH_DES_ITER   16

#if defined(BITS32) || defined(BITS64)
/* these are for 32 bit machines  or 64 bit machines using 32bit structures */

typedef struct {
    unsigned b0:6;
    unsigned b1:6;
    unsigned b2:6;
    unsigned b3:6;
    unsigned b4:6;
    unsigned b5:2;
}       sbox_in_a;

typedef struct {
    unsigned b5:4;
    unsigned b6:6;
    unsigned b7:6;
}       sbox_in_b;

typedef struct {
    unsigned b0:4;
    unsigned b1:4;
    unsigned b2:4;
    unsigned b3:4;
    unsigned b4:4;
    unsigned b5:4;
    unsigned b6:4;
    unsigned b7:4;
}       sbox_out;

#else	/*BITS32*/
/* for sixteen bit machines */

typedef struct {
    unsigned b0:6;
    unsigned b1:6;
    unsigned b2:4;
}       sbox_in_16_a;

typedef struct {
    unsigned b2:2;
    unsigned b3:6;
    unsigned b4:6;
    unsigned b5:2;
}       sbox_in_16_b;

typedef struct {
    unsigned b5:4;
    unsigned b6:6;
    unsigned b7:6;
}       sbox_in_16_c;

typedef struct {
    unsigned b0:4;
    unsigned b1:4;
    unsigned b2:4;
    unsigned b3:4;
    unsigned b4:4;
    unsigned b5:4;
    unsigned b6:4;
    unsigned b7:4;
}       sbox_out;
#endif	/*BITS32 || BITS64*/


/* widen prototypes, if needed */
#include <krb5/widen.h>

/* cbc_cksum.c */
extern krb5_error_code mit_des_cbc_checksum
    PROTOTYPE((krb5_pointer, size_t,krb5_pointer,size_t, krb5_checksum * ));

/* cksum.c */
extern void mit_des_cbc_cksum
    PROTOTYPE((krb5_octet *, krb5_octet *, krb5_int32, mit_des_key_schedule ,
	       krb5_octet *));
/* des.c */
extern int mit_des_ecb_encrypt
    PROTOTYPE((krb5_octet *, krb5_octet *, mit_des_key_schedule , int ));

/* enc_dec.c */
extern krb5_error_code mit_des_encrypt_func
    PROTOTYPE(( krb5_const_pointer, krb5_pointer, const size_t,
	       krb5_encrypt_block *, krb5_pointer ));
extern krb5_error_code mit_des_decrypt_func
    PROTOTYPE(( krb5_const_pointer, krb5_pointer, const size_t,
	       krb5_encrypt_block *, krb5_pointer ));
extern krb5_error_code mit_des_cbc_encrypt
    PROTOTYPE((krb5_octet *, krb5_octet *, krb5_int32, mit_des_key_schedule,
	       krb5_octet *, int));

/* fin_rndkey.c */
extern krb5_error_code mit_des_finish_random_key
    PROTOTYPE(( krb5_pointer *));

/* finish_key.c */
extern krb5_error_code mit_des_finish_key
    PROTOTYPE(( krb5_encrypt_block *));

/* init_rkey.c */
extern krb5_error_code mit_des_init_random_key
    PROTOTYPE(( const krb5_keyblock *,  krb5_pointer *));

/* key_parity.c */
extern void mit_des_fixup_key_parity PROTOTYPE((mit_des_cblock ));
extern int mit_des_check_key_parity PROTOTYPE((mit_des_cblock ));

/* key_sched.c */
extern int mit_des_key_sched
    PROTOTYPE((mit_des_cblock , mit_des_key_schedule ));
extern int make_key_sched
    PROTOTYPE((mit_des_cblock , mit_des_key_schedule ));

/* new_rnd_key.c */
extern int mit_des_new_random_key
    PROTOTYPE((mit_des_cblock , mit_des_random_key_seed *));
extern void mit_des_init_random_number_generator
    PROTOTYPE((mit_des_cblock, mit_des_random_key_seed *));
extern void mit_des_set_random_generator_seed
    PROTOTYPE((mit_des_cblock , mit_des_random_key_seed *));
extern void mit_des_set_sequence_number
    PROTOTYPE((mit_des_cblock , mit_des_random_key_seed *));
extern void mit_des_generate_random_block
    PROTOTYPE((mit_des_cblock , mit_des_random_key_seed *));

/* process_ky.c */
extern krb5_error_code mit_des_process_key
    PROTOTYPE(( krb5_encrypt_block *,  const krb5_keyblock *));

/* random_key.c */
extern krb5_error_code mit_des_random_key
    PROTOTYPE(( krb5_pointer ,  krb5_keyblock **));

/* string2key.c */
extern krb5_error_code mit_des_string_to_key
    PROTOTYPE((const krb5_keytype, krb5_keyblock *, const krb5_data *,
	       const krb5_data *));

/* weak_key.c */
extern int mit_des_is_weak_key PROTOTYPE((mit_des_cblock ));

/* misc.c */
extern void swap_bits PROTOTYPE((char *));
extern krb5_ui_4 long_swap_bits PROTOTYPE((krb5_ui_4));
extern unsigned long swap_six_bits_to_ansi PROTOTYPE((unsigned long ));
extern unsigned long swap_four_bits_to_ansi PROTOTYPE((unsigned long ));
extern unsigned long swap_bit_pos_1 PROTOTYPE((unsigned long ));
extern unsigned long swap_bit_pos_0 PROTOTYPE((unsigned long ));
extern unsigned long swap_bit_pos_0_to_ansi PROTOTYPE((unsigned long ));
extern unsigned long rev_swap_bit_pos_0 PROTOTYPE((unsigned long ));
extern unsigned long swap_byte_bits PROTOTYPE((unsigned long ));
extern unsigned long swap_long_bytes_bit_number PROTOTYPE((unsigned long ));
#ifdef FILE
/* XXX */
extern void test_set PROTOTYPE((FILE *, const char *, int, const char *, int));
#endif

/* and back to normal... */
#include <krb5/narrow.h>

#endif	/*DES_INTERNAL_DEFS*/
