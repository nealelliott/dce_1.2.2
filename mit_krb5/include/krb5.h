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
 * $Log: krb5.h,v $
 * Revision 1.1.2.2  1996/07/24  19:17:14  arvind
 * 	OSF DCE 1.2.2 - Intl. build changes
 * 	[1996/07/24  19:16:27  arvind]
 *
 * Revision 1.1.2.1  1996/06/05  20:29:13  arvind
 * 	     DCE1.2.2 Drop 2 to OSF
 * 	     [1996/06/05  19:28:31  arvind]
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * include/krb5.h
 *
 * Copyright 1989,1990,1995 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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
 * General definitions for Kerberos version 5.
 */

#ifndef KRB5_GENERAL__
#define KRB5_GENERAL__

#ifndef KRB5_AUTOCONF__
#define KRB5_AUTOCONF__
#include "autoconf.h"
#endif

#ifdef _MSDOS

#define SIZEOF_INT      2
#define SIZEOF_LONG     4

#ifndef KRB5_CONFIG__
#define KRB5_PROVIDE_PROTOTYPES
#define HAS_VOID_TYPE
#define HAVE_STDARG_H
#ifndef INTERFACE
#define INTERFACE   __far __export __pascal
#define INTERFACE_C __far __export __cdecl
#endif /* !INTERFACE */
#define FAR     _far
#define NEAR    _near
#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#define _SIZE_T_DEFINED
#endif /* !_SIZE_T_DEFINED */
#ifndef KRB5_SYSTYPES__
#define KRB5_SYSTYPES__
#include <sys/types.h>
typedef unsigned long u_long;      /* Not part of sys/types.h on the pc */
typedef unsigned int	u_int;
typedef unsigned short	u_short;
typedef unsigned char	u_char;
#endif /* KRB5_SYSTYPES__ */

#endif /* !KRB5_CONFIG */
#else /* !_MSDOS */
#ifndef KRB5_CONFIG__
#define INTERFACE
#define INTERFACE_C
#define FAR
#define NEAR
#endif /* !KRB5_CONFIG__ */
#include <sys/types.h>
#endif /* _MSDOS */

/*
 * begin wordsize.h
 */

/*
 * Word-size related definition.
 */

typedef	unsigned char	krb5_octet;
typedef	unsigned char	krb5_ui_1;

#if (SIZEOF_INT == 2)
typedef	int	krb5_int16;
typedef	unsigned int	krb5_ui_2;
#define VALID_INT_BITS    0x7fff
#define VALID_UINT_BITS   0xffff
#elif (SIZEOF_SHORT == 2)
typedef	short	krb5_int16;
typedef	unsigned short	krb5_ui_2;
#else
  ?==error: undefined 16 bit type
#endif

#if (SIZEOF_INT == 4)
typedef	int	krb5_int32;
typedef	unsigned int	krb5_ui_4;
#define VALID_INT_BITS    0x7fffffff
#define VALID_UINT_BITS   0xffffffff
#elif (SIZEOF_LONG == 4)
typedef	long	krb5_int32;
typedef	unsigned long	krb5_ui_4;
#elif (SIZEOF_SHORT == 4)
typedef	short	krb5_int32;
typedef	unsigned short	krb5_ui_4;
#else
 ?== error: undefined 32 bit type
#endif

#define KRB5_INT32_MAX	2147483647
/* this strange form is necessary since - is a unary operator, not a sign
   indicator */
#define KRB5_INT32_MIN	(-KRB5_INT32_MAX-1)

/*
 * end wordsize.h
 */

/*
 * begin "base-defs.h"
 */

/*
 * Basic definitions for Kerberos V5 library
 */

#ifndef FALSE
#define	FALSE	0
#endif
#ifndef TRUE
#define	TRUE	1
#endif

typedef	unsigned int krb5_boolean;
typedef	unsigned int krb5_msgtype;	
typedef	unsigned int krb5_kvno;	

typedef	unsigned int krb5_addrtype;
typedef unsigned int krb5_keytype;
typedef unsigned int krb5_enctype;
typedef unsigned int krb5_cksumtype;
typedef unsigned int krb5_authdatatype;

typedef krb5_int32	krb5_preauthtype; /* This may change, later on */
typedef	krb5_int32	krb5_flags;
typedef krb5_int32	krb5_timestamp;
typedef	krb5_int32	krb5_error_code;
typedef krb5_int32	krb5_deltat;

typedef krb5_error_code	krb5_magic;

typedef struct _krb5_data {
    krb5_magic magic;
    int length;
    char FAR *data;
} krb5_data;

/* make const & volatile available without effect */

#if !defined(__STDC__) && !defined(HAS_ANSI_CONST) && !defined(_WINDOWS)
#define const
#endif

#if defined(__STDC__) || defined(HAS_VOID_TYPE) || defined(_WINDOWS)
typedef	void FAR * krb5_pointer;
typedef void const FAR * krb5_const_pointer;
#else
typedef char FAR * krb5_pointer;
typedef char const FAR * krb5_const_pointer;
#endif

#if defined(__STDC__) || defined(KRB5_PROVIDE_PROTOTYPES) || defined(_WINDOWS)
#define PROTOTYPE(x) x
#ifdef OSF_DCE
#define DECLARG(type, val) type val
#define OLDDECLARG(type, val)
#endif /* OSF_DCE */
#if defined(__STDC__) || defined(HAVE_STDARG_H) || defined(_WINDOWS)
#define	STDARG_P(x) x
#else
#define STDARG_P(x) ()
#endif /* defined(__STDC__) || defined(HAVE_STDARG_H) */
#else
#define PROTOTYPE(x) ()
#ifdef OSF_DCE
#define DECLARG(type, val) val
#define OLDDECLARG(type, val) type val;
#endif /* OSF_DCE */
#define STDARG_P(x) ()
#ifdef OSF_DCE
#define DECLARG(type, val) val
#define OLDDECLARG(type, val) type val;
#endif /* OSF_DCE */
#endif /* STDC or PROTOTYPES */

#ifdef NO_NESTED_PROTOTYPES
#define	NPROTOTYPE(x) ()
#else
#define	NPROTOTYPE(x) PROTOTYPE(x)
#endif

typedef struct krb5_principal_data {
    krb5_magic magic;
    krb5_data realm;
    krb5_data FAR *data;		/* An array of strings */
    krb5_int32 length;
    krb5_int32 type;
} krb5_principal_data;

typedef	krb5_principal_data FAR * krb5_principal;

/*
 * Per V5 spec on definition of principal types
 */

/* Name type not known */
#define KRB5_NT_UNKNOWN		0
/* Just the name of the principal as in DCE, or for users */
#define KRB5_NT_PRINCIPAL	1
/* Service and other unique instance (krbtgt) */
#define KRB5_NT_SRV_INST	2
/* Service with host name as instance (telnet, rcommands) */
#define KRB5_NT_SRV_HST		3
/* Service with host as remaining components */
#define KRB5_NT_SRV_XHST	4
/* Unique ID */
#define KRB5_NT_UID		5

/* constant version thereof: */
typedef const krb5_principal_data FAR *krb5_const_principal;

#define krb5_princ_realm(context, princ) (&(princ)->realm)
#define krb5_princ_set_realm(context, princ,value) ((princ)->realm = *(value))
#define krb5_princ_set_realm_length(context, princ,value) (princ)->realm.length = (value)
#define krb5_princ_set_realm_data(context, princ,value) (princ)->realm.data = (value)
#define	krb5_princ_size(context, princ) (princ)->length
#define	krb5_princ_type(context, princ) (princ)->type
#define	krb5_princ_name(context, princ) (princ)->data
#define	krb5_princ_component(context, princ,i) ((princ)->data + i)

/*
 * end "base-defs.h"
 */

/*
 * begin "hostaddr.h"
 */

/* structure for address */
typedef struct _krb5_address {
    krb5_magic magic;
    krb5_addrtype addrtype;
    int length;
    krb5_octet FAR *contents;
} krb5_address;

/* per Kerberos v5 protocol spec */
#define	ADDRTYPE_INET		0x0002
#define	ADDRTYPE_CHAOS		0x0005
#define	ADDRTYPE_XNS		0x0006
#define	ADDRTYPE_ISO		0x0007
#define ADDRTYPE_DDP		0x0010
/* not yet in the spec... */
#define ADDRTYPE_ADDRPORT	0x0100
#define ADDRTYPE_IPPORT		0x0101

/* macros to determine if a type is a local type */
#define ADDRTYPE_IS_LOCAL(addrtype) (addrtype & 0x8000)

/* implementation-specific stuff: */
typedef struct _krb5_fulladdr {
    krb5_address FAR * address;
    krb5_address FAR * port;
} krb5_fulladdr;

/*
 * end "hostaddr.h"
 */


struct _krb5_context;
typedef struct _krb5_context FAR * krb5_context;

struct _krb5_auth_context;
typedef struct _krb5_auth_context krb5_auth_context;

/*
 * begin "encryption.h"
 */

typedef struct _krb5_keyblock {
    krb5_magic magic;
    krb5_keytype keytype;
    krb5_enctype etype;	/* hint of what encryption type to use */
    int length;
    krb5_octet FAR *contents;
} krb5_keyblock;

typedef struct _krb5_checksum {
    krb5_magic magic;
    krb5_cksumtype checksum_type;	/* checksum type */
    int length;
    krb5_octet FAR *contents;
} krb5_checksum;

typedef struct _krb5_encrypt_block {
    krb5_magic magic;
    struct _krb5_cryptosystem_entry FAR *crypto_entry;
    krb5_keyblock FAR *key;
    krb5_pointer priv;			/* for private use, e.g. DES
					   key schedules */
} krb5_encrypt_block;

typedef struct _krb5_enc_data {
    krb5_magic magic;
    krb5_enctype etype;
    krb5_kvno kvno;
    krb5_data ciphertext;
} krb5_enc_data;

/* could be used in a table to find an etype and initialize a block */
typedef struct _krb5_cryptosystem_entry {
    krb5_magic magic;
    krb5_error_code (*encrypt_func) NPROTOTYPE(( krb5_const_pointer /* in */,
					       krb5_pointer /* out */,
					       const size_t,
					       krb5_encrypt_block FAR *,
					       krb5_pointer));
    krb5_error_code (*decrypt_func) NPROTOTYPE(( krb5_const_pointer /* in */,
					       krb5_pointer /* out */,
					       const size_t,
					       krb5_encrypt_block FAR *,
					       krb5_pointer));
    krb5_error_code (*process_key) NPROTOTYPE(( krb5_encrypt_block FAR *,
					      const krb5_keyblock FAR *));
    krb5_error_code (*finish_key) NPROTOTYPE(( krb5_encrypt_block FAR *));
    krb5_error_code (*string_to_key) NPROTOTYPE((const krb5_encrypt_block FAR *,
						 const krb5_keytype,
						krb5_keyblock FAR *,
						const krb5_data FAR *,
 	                                        const krb5_data FAR *));
    krb5_error_code  (*init_random_key) NPROTOTYPE((const krb5_keyblock FAR *,
						   krb5_pointer FAR *));
    krb5_error_code  (*finish_random_key) NPROTOTYPE(( krb5_pointer FAR *));
    krb5_error_code (*random_key) NPROTOTYPE(( const krb5_encrypt_block FAR *,
					      krb5_pointer,
					      krb5_keyblock FAR * FAR *));
    int block_length;
    int pad_minimum;			/* needed for cksum size computation */
    int keysize;
    krb5_enctype proto_enctype;		/* encryption type,
					   (assigned protocol number AND
					    table index) */
    krb5_keytype proto_keytype;		/* key type,
					   (assigned protocol number AND
					    table index) */
} krb5_cryptosystem_entry;

typedef struct _krb5_cs_table_entry {
    krb5_magic magic;
    krb5_cryptosystem_entry FAR *system;
    krb5_pointer random_sequence;	/* from init_random_key() */
} krb5_cs_table_entry;

/* could be used in a table to find a sumtype */
typedef krb5_error_code  (*SUM_FUNC) NPROTOTYPE (
			(krb5_pointer /* in */,
			size_t /* in_length */,
			krb5_pointer /* key/seed */,
			size_t /* key/seed size */,
			krb5_checksum FAR * /* out_cksum */));

typedef struct _krb5_checksum_entry {
    krb5_magic magic;
    SUM_FUNC sum_func;
    int checksum_length;		/* length of stuff returned by
					   sum_func */
    unsigned int is_collision_proof:1;
    unsigned int uses_key:1;
} krb5_checksum_entry;

/* per Kerberos v5 protocol spec */
#define	KEYTYPE_NULL		0x0000
#define KEYTYPE_DES		0x0001	/* Data Encryption Standard,
					   FIPS 46,81 */

#define	ETYPE_NULL		0x0000
#define	ETYPE_DES_CBC_CRC	0x0001	/* DES cbc mode with CRC-32 */
#define	ETYPE_DES_CBC_MD4	0x0002	/* DES cbc mode with RSA-MD4 */
#define	ETYPE_DES_CBC_MD5	0x0003	/* DES cbc mode with RSA-MD5 */
#define	ETYPE_RAW_DES_CBC       0x0004  /* Raw DES cbc mode */

#define ETYPE_UNKNOWN		0x1FF 	/* Reserved local value */

#define	CKSUMTYPE_CRC32		0x0001
#define	CKSUMTYPE_RSA_MD4	0x0002
#define	CKSUMTYPE_RSA_MD4_DES	0x0003
#define	CKSUMTYPE_DESCBC	0x0004
/* des-mac-k */
/* rsa-md4-des-k */
#define	CKSUMTYPE_RSA_MD5	0x0007
#define	CKSUMTYPE_RSA_MD5_DES	0x0008

/* macros to determine if a type is a local type */
#define KEYTYPE_IS_LOCAL(keytype) (keytype & 0x8000)
#define ETYPE_IS_LOCAL(etype) (etype & 0x8000)
#define CKSUMTYPE_IS_LOCAL(cksumtype) (cksumtype & 0x8000)

#ifndef krb5_roundup
/* round x up to nearest multiple of y */
#define krb5_roundup(x, y) ((((x) + (y) - 1)/(y))*(y))
#endif /* roundup */

/* macro function definitions to help clean up code */
#define	krb5_encrypt_size(length, crypto) \
     krb5_roundup((length)+(crypto)->pad_minimum, (crypto)->block_length)

/* This array is indexed by encryption type */
extern krb5_cs_table_entry * NEAR krb5_csarray[];
extern int krb5_max_cryptosystem;		/* max entry in array */

/* This array is indexed by key type, and has (should have) pointers to
   the same entries as krb5_csarray */
/* XXX what if a given keytype works for several etypes? */
extern krb5_cs_table_entry * NEAR krb5_keytype_array[];
extern int krb5_max_keytype;		/* max entry in array */

/* This array is indexed by checksum type */
extern krb5_checksum_entry * NEAR krb5_cksumarray[];
extern int krb5_max_cksum;		/* max entry in array */

#define valid_etype(etype)     ((((int) (etype)) <= krb5_max_cryptosystem) && ((etype) > 0) && krb5_csarray[etype])

#define valid_keytype(ktype)     ((((int) (ktype)) <= krb5_max_keytype) && ((ktype) > 0) && krb5_keytype_array[ktype])

#define valid_cksumtype(cktype)     ((((int) (cktype)) <= krb5_max_cksum) && ((cktype) > 0) && krb5_cksumarray[cktype])

#define is_coll_proof_cksum(cktype) (krb5_cksumarray[cktype]->is_collision_proof)
#define is_keyed_cksum(cktype) (krb5_cksumarray[cktype]->uses_key)

/* set up *eblockp to use etype */
#define krb5_use_cstype(context, eblockp, etype) (eblockp)->crypto_entry = krb5_csarray[(etype)]->system
/* ...or keytype */
#define krb5_use_keytype(context, eblockp, keytype) (eblockp)->crypto_entry = krb5_keytype_array[(keytype)]->system

#define krb5_encrypt(context, inptr, outptr, size, eblock, ivec) (*(eblock)->crypto_entry->encrypt_func)(inptr, outptr, size, eblock, ivec)
#define krb5_decrypt(context, inptr, outptr, size, eblock, ivec) (*(eblock)->crypto_entry->decrypt_func)(inptr, outptr, size, eblock, ivec)
#define krb5_process_key(context, eblock, key) (*(eblock)->crypto_entry->process_key)(eblock, key)
#define krb5_finish_key(context, eblock) (*(eblock)->crypto_entry->finish_key)(eblock)
#define krb5_string_to_key(context, eblock, keytype, keyblock, data, princ) (*(eblock)->crypto_entry->string_to_key)(eblock, keytype, keyblock, data, princ)
#define krb5_init_random_key(context, eblock, keyblock, ptr) (*(eblock)->crypto_entry->init_random_key)(keyblock, ptr)
#define krb5_finish_random_key(context, eblock, ptr) (*(eblock)->crypto_entry->finish_random_key)(ptr)
#define krb5_random_key(context, eblock, ptr, keyblock) (*(eblock)->crypto_entry->random_key)(eblock, ptr, keyblock)

#define krb5_eblock_keytype(context, eblockp) ((eblockp)->crypto_entry->proto_keytype)
#define krb5_eblock_enctype(context, eblockp) ((eblockp)->crypto_entry->proto_enctype)

/*
 * Here's the stuff for the checksum switch:
 */
#define krb5_checksum_size(context, ctype)  (krb5_cksumarray[ctype]->checksum_length)
#define krb5_calculate_checksum(context, ctype, in, in_length, seed, seed_length, outcksum) ((*krb5_cksumarray[ctype]->sum_func)(in, in_length, seed, seed_length, outcksum))

/*
 * end "encryption.h"
 */

/*
 * begin "fieldbits.h"
 */

/* kdc_options for kdc_request */
/* options is 32 bits; each host is responsible to put the 4 bytes
   representing these bits into net order before transmission */
/* #define	KDC_OPT_RESERVED	0x80000000 */
#define	KDC_OPT_FORWARDABLE		0x40000000
#define	KDC_OPT_FORWARDED		0x20000000
#define	KDC_OPT_PROXIABLE		0x10000000
#define	KDC_OPT_PROXY			0x08000000
#define	KDC_OPT_ALLOW_POSTDATE		0x04000000
#define	KDC_OPT_POSTDATED		0x02000000
/* #define	KDC_OPT_UNUSED		0x01000000 */
#define	KDC_OPT_RENEWABLE		0x00800000
/* #define	KDC_OPT_UNUSED		0x00400000 */
/* #define	KDC_OPT_RESERVED	0x00200000 */
/* #define	KDC_OPT_RESERVED	0x00100000 */
/* #define	KDC_OPT_RESERVED	0x00080000 */
/* #define	KDC_OPT_RESERVED	0x00040000 */
/* #define	KDC_OPT_RESERVED	0x00020000 */
/* #define	KDC_OPT_RESERVED	0x00010000 */
/* #define	KDC_OPT_RESERVED	0x00008000 */
/* #define	KDC_OPT_RESERVED	0x00004000 */
/* #define	KDC_OPT_RESERVED	0x00002000 */
/* #define	KDC_OPT_RESERVED	0x00001000 */
/* #define	KDC_OPT_RESERVED	0x00000800 */
/* #define	KDC_OPT_RESERVED	0x00000400 */
/* #define	KDC_OPT_RESERVED	0x00000200 */
/* #define	KDC_OPT_RESERVED	0x00000100 */
/* #define	KDC_OPT_RESERVED	0x00000080 */
/* #define	KDC_OPT_RESERVED	0x00000040 */
/* #define	KDC_OPT_RESERVED	0x00000020 */
#define	KDC_OPT_RENEWABLE_OK		0x00000010
#define	KDC_OPT_ENC_TKT_IN_SKEY		0x00000008
/* #define	KDC_OPT_UNUSED		0x00000004 */
#define	KDC_OPT_RENEW			0x00000002
#define	KDC_OPT_VALIDATE		0x00000001

/*
 * Mask of ticket flags in the TGT which should be converted into KDC
 * options when using the TGT to get derivitive tickets.
 * 
 *  New mask = KDC_OPT_FORWARDABLE | KDC_OPT_PROXIABLE |
 *             KDC_OPT_ALLOW_POSTDATE | KDC_OPT_RENEWABLE
 */
#define KDC_TKT_COMMON_MASK		0x54800000

/* definitions for ap_options fields */
/* ap_options are 32 bits; each host is responsible to put the 4 bytes
   representing these bits into net order before transmission */
#define	AP_OPTS_RESERVED		0x80000000
#define	AP_OPTS_USE_SESSION_KEY		0x40000000
#define	AP_OPTS_MUTUAL_REQUIRED		0x20000000
/* #define	AP_OPTS_RESERVED	0x10000000 */
/* #define	AP_OPTS_RESERVED	0x08000000 */
/* #define	AP_OPTS_RESERVED	0x04000000 */
/* #define	AP_OPTS_RESERVED	0x02000000 */
/* #define	AP_OPTS_RESERVED	0x01000000 */
/* #define	AP_OPTS_RESERVED	0x00800000 */
/* #define	AP_OPTS_RESERVED	0x00400000 */
/* #define	AP_OPTS_RESERVED	0x00200000 */
/* #define	AP_OPTS_RESERVED	0x00100000 */
/* #define	AP_OPTS_RESERVED	0x00080000 */
/* #define	AP_OPTS_RESERVED	0x00040000 */
/* #define	AP_OPTS_RESERVED	0x00020000 */
/* #define	AP_OPTS_RESERVED	0x00010000 */
/* #define	AP_OPTS_RESERVED	0x00008000 */
/* #define	AP_OPTS_RESERVED	0x00004000 */
/* #define	AP_OPTS_RESERVED	0x00002000 */
/* #define	AP_OPTS_RESERVED	0x00001000 */
/* #define	AP_OPTS_RESERVED	0x00000800 */
/* #define	AP_OPTS_RESERVED	0x00000400 */
/* #define	AP_OPTS_RESERVED	0x00000200 */
/* #define	AP_OPTS_RESERVED	0x00000100 */
/* #define	AP_OPTS_RESERVED	0x00000080 */
/* #define	AP_OPTS_RESERVED	0x00000040 */
/* #define	AP_OPTS_RESERVED	0x00000020 */
/* #define	AP_OPTS_RESERVED	0x00000010 */
/* #define	AP_OPTS_RESERVED	0x00000008 */
/* #define	AP_OPTS_RESERVED	0x00000004 */
/* #define	AP_OPTS_RESERVED	0x00000002 */
#define AP_OPTS_USE_SUBKEY      0x00000001

#define AP_OPTS_WIRE_MASK	0xfffffff0

/* definitions for ad_type fields. */
#define	AD_TYPE_RESERVED	0x8000
#define	AD_TYPE_EXTERNAL	0x4000
#define	AD_TYPE_REGISTERED	0x2000

#define AD_TYPE_FIELD_TYPE_MASK	0x1fff

/* Ticket flags */
/* flags are 32 bits; each host is responsible to put the 4 bytes
   representing these bits into net order before transmission */
/* #define	TKT_FLG_RESERVED	0x80000000 */
#define	TKT_FLG_FORWARDABLE		0x40000000
#define	TKT_FLG_FORWARDED		0x20000000
#define	TKT_FLG_PROXIABLE		0x10000000
#define	TKT_FLG_PROXY			0x08000000
#define	TKT_FLG_MAY_POSTDATE		0x04000000
#define	TKT_FLG_POSTDATED		0x02000000
#define	TKT_FLG_INVALID			0x01000000
#define	TKT_FLG_RENEWABLE		0x00800000
#define	TKT_FLG_INITIAL			0x00400000
#define	TKT_FLG_PRE_AUTH		0x00200000
#define	TKT_FLG_HW_AUTH			0x00100000
/* #define	TKT_FLG_RESERVED	0x00080000 */
/* #define	TKT_FLG_RESERVED	0x00040000 */
/* #define	TKT_FLG_RESERVED	0x00020000 */
/* #define	TKT_FLG_RESERVED	0x00010000 */
/* #define	TKT_FLG_RESERVED	0x00008000 */
/* #define	TKT_FLG_RESERVED	0x00004000 */
/* #define	TKT_FLG_RESERVED	0x00002000 */
/* #define	TKT_FLG_RESERVED	0x00001000 */
/* #define	TKT_FLG_RESERVED	0x00000800 */
/* #define	TKT_FLG_RESERVED	0x00000400 */
/* #define	TKT_FLG_RESERVED	0x00000200 */
/* #define	TKT_FLG_RESERVED	0x00000100 */
/* #define	TKT_FLG_RESERVED	0x00000080 */
/* #define	TKT_FLG_RESERVED	0x00000040 */
/* #define	TKT_FLG_RESERVED	0x00000020 */
/* #define	TKT_FLG_RESERVED	0x00000010 */
/* #define	TKT_FLG_RESERVED	0x00000008 */
/* #define	TKT_FLG_RESERVED	0x00000004 */
/* #define	TKT_FLG_RESERVED	0x00000002 */
/* #define	TKT_FLG_RESERVED	0x00000001 */

/* definitions for lr_type fields. */
#define	LR_TYPE_THIS_SERVER_ONLY	0x8000

#define LR_TYPE_INTERPRETATION_MASK	0x7fff

/* definitions for ad_type fields. */
#define	AD_TYPE_EXTERNAL	0x4000
#define	AD_TYPE_REGISTERED	0x2000

#define AD_TYPE_FIELD_TYPE_MASK	0x1fff
#define AD_TYPE_INTERNAL_MASK	0x3fff

/* definitions for msec direction bit for KRB_SAFE, KRB_PRIV */
#define	MSEC_DIRBIT		0x8000
#define	MSEC_VAL_MASK		0x7fff

/*
 * end "fieldbits.h"
 */

/*
 * begin "proto.h"
 */

/* Protocol version number */
#define	KRB5_PVNO	5

/* Message types */

#define	KRB5_AS_REQ	((krb5_msgtype)10) /* Req for initial authentication */
#define	KRB5_AS_REP	((krb5_msgtype)11) /* Response to KRB_AS_REQ request */
#define	KRB5_TGS_REQ	((krb5_msgtype)12) /* TGS request to server */
#define	KRB5_TGS_REP	((krb5_msgtype)13) /* Response to KRB_TGS_REQ req */
#define	KRB5_AP_REQ	((krb5_msgtype)14) /* application request to server */
#define	KRB5_AP_REP	((krb5_msgtype)15) /* Response to KRB_AP_REQ_MUTUAL */
#define	KRB5_SAFE	((krb5_msgtype)20) /* Safe application message */
#define	KRB5_PRIV	((krb5_msgtype)21) /* Private application message */
#define	KRB5_CRED	((krb5_msgtype)22) /* Credential forwarding message */
#define	KRB5_ERROR	((krb5_msgtype)30) /* Error response */

/* LastReq types */
#define KRB5_LRQ_NONE			0
#define KRB5_LRQ_ALL_LAST_TGT		1
#define KRB5_LRQ_ONE_LAST_TGT		(-1)
#define KRB5_LRQ_ALL_LAST_INITIAL	2
#define KRB5_LRQ_ONE_LAST_INITIAL	(-2)
#define KRB5_LRQ_ALL_LAST_TGT_ISSUED	3
#define KRB5_LRQ_ONE_LAST_TGT_ISSUED	(-3)
#define KRB5_LRQ_ALL_LAST_RENEWAL	4
#define KRB5_LRQ_ONE_LAST_RENEWAL	(-4)
#define KRB5_LRQ_ALL_LAST_REQ		5
#define KRB5_LRQ_ONE_LAST_REQ		(-5)

/* PADATA types */
#define KRB5_PADATA_NONE                0
#define	KRB5_PADATA_AP_REQ		1
#define	KRB5_PADATA_TGS_REQ		KRB5_PADATA_AP_REQ
#define KRB5_PADATA_ENC_TIMESTAMP	2
#define	KRB5_PADATA_PW_SALT		3
#if 0				/* Not used */
#define KRB5_PADATA_ENC_ENCKEY          4  /* Key encrypted within itself */
#endif
#define KRB5_PADATA_ENC_UNIX_TIME       5  /* timestamp encrypted in key */
#define KRB5_PADATA_ENC_SANDIA_SECURID  6  /* SecurId passcode */
#define KRB5_PADATA_SESAME		7  /* Sesame project */
#define KRB5_PADATA_OSF_DCE		8  /* OSF DCE */
    
/* Reserved for SPX pre-authentication. */
#define KRB5_PADATA_DASS		16

/* Transited encoding types */
#define	KRB5_DOMAIN_X500_COMPRESS	1

/* alternate authentication types */
#define	KRB5_ALTAUTH_ATT_CHALLENGE_RESPONSE	64

/* authorization data types */
#define	KRB5_AUTHDATA_OSF_DCE	64
#define KRB5_AUTHDATA_SESAME	65

/*
 * end "proto.h"
 */

/*
 * begin "error_def.h"
 */

#include "krb5_err.h"
#include "kdb5_err.h"
#include "kv5m_err.h"
#include "asn1_err.h"
#include <errno.h>

/*
 * end "error_def.h"
 */

/* Time set */
typedef struct _krb5_ticket_times {
    krb5_timestamp authtime; /* XXX ? should ktime in KDC_REP == authtime
				in ticket? otherwise client can't get this */ 
    krb5_timestamp starttime;		/* optional in ticket, if not present,
					   use authtime */
    krb5_timestamp endtime;
    krb5_timestamp renew_till;
} krb5_ticket_times;

/* structure for auth data */
typedef struct _krb5_authdata {
    krb5_magic magic;
    krb5_authdatatype ad_type;
    int length;
    krb5_octet FAR *contents;
} krb5_authdata;

/* structure for transited encoding */
typedef struct _krb5_transited {
    krb5_magic magic;
    krb5_octet tr_type;
    krb5_data tr_contents;
} krb5_transited;

typedef struct _krb5_enc_tkt_part {
    krb5_magic magic;
    /* to-be-encrypted portion */
    krb5_flags flags;			/* flags */
    krb5_keyblock FAR *session;		/* session key: includes keytype */
    krb5_principal client;		/* client name/realm */
    krb5_transited transited;		/* list of transited realms */
    krb5_ticket_times times;		/* auth, start, end, renew_till */
    krb5_address FAR * FAR *caddrs;	/* array of ptrs to addresses */
    krb5_authdata FAR * FAR *authorization_data; /* auth data */
} krb5_enc_tkt_part;

typedef struct _krb5_ticket {
    krb5_magic magic;
    /* cleartext portion */
    krb5_principal server;		/* server name/realm */
    krb5_enc_data enc_part;		/* encryption type, kvno, encrypted
					   encoding */
    krb5_enc_tkt_part FAR *enc_part2;	/* ptr to decrypted version, if
					   available */
} krb5_ticket;

/* the unencrypted version */
typedef struct _krb5_authenticator {
    krb5_magic magic;
    krb5_principal client;		/* client name/realm */
    krb5_checksum FAR *checksum;	/* checksum, includes type, optional */
    krb5_int32 cusec;			/* client usec portion */
    krb5_timestamp ctime;		/* client sec portion */
    krb5_keyblock FAR *subkey;		/* true session key, optional */
    krb5_int32 seq_number;		/* sequence #, optional */
    krb5_authdata FAR * FAR *authorization_data; /* New add by Ari, auth data */
} krb5_authenticator;

typedef struct _krb5_tkt_authent {
    krb5_magic magic;
    krb5_ticket FAR *ticket;
    krb5_authenticator FAR *authenticator;
    krb5_flags ap_options;
} krb5_tkt_authent;

/* credentials:  Ticket, session key, etc. */
typedef struct _krb5_creds {
    krb5_magic magic;
    krb5_principal client;		/* client's principal identifier */
    krb5_principal server;		/* server's principal identifier */
    krb5_keyblock keyblock;		/* session encryption key info */
    krb5_ticket_times times;		/* lifetime info */
    krb5_boolean is_skey;		/* true if ticket is encrypted in
					   another ticket's skey */
    krb5_flags ticket_flags;		/* flags in ticket */
    krb5_address FAR * FAR *addresses;	/* addrs in ticket */
    krb5_data ticket;			/* ticket string itself */
    krb5_data second_ticket;		/* second ticket, if related to
					   ticket (via DUPLICATE-SKEY or
					   ENC-TKT-IN-SKEY) */
    krb5_authdata FAR * FAR *authdata;	/* authorization data */
} krb5_creds;

/* Last request fields */
typedef struct _krb5_last_req_entry {
    krb5_magic magic;
    krb5_octet lr_type;
    krb5_timestamp value;
} krb5_last_req_entry;

/* pre-authentication data */
typedef struct _krb5_pa_data {
    krb5_magic magic;
    krb5_ui_2  pa_type;
    int length;
    krb5_octet FAR *contents;
} krb5_pa_data;

typedef struct _krb5_kdc_req {
    krb5_magic magic;
    krb5_msgtype msg_type;		/* AS_REQ or TGS_REQ? */
    krb5_pa_data FAR * FAR *padata;	/* e.g. encoded AP_REQ */
    /* real body */
    krb5_flags kdc_options;		/* requested options */
    krb5_principal client;		/* includes realm; optional */
    krb5_principal server;		/* includes realm (only used if no
					   client) */
    krb5_timestamp from;		/* requested starttime */
    krb5_timestamp till;		/* requested endtime */
    krb5_timestamp rtime;		/* (optional) requested renew_till */
    krb5_int32 nonce;			/* nonce to match request/response */
    int netypes;			/* # of etypes, must be positive */
    krb5_enctype FAR *etype;		/* requested encryption type(s) */
    krb5_address FAR * FAR *addresses;	/* requested addresses, optional */
    krb5_enc_data authorization_data;	/* encrypted auth data; OPTIONAL */
    krb5_authdata FAR * FAR *unenc_authdata; /* unencrypted auth data,
					   if available */
    krb5_ticket FAR * FAR *second_ticket;/* second ticket array; OPTIONAL */
} krb5_kdc_req;

typedef struct _krb5_enc_kdc_rep_part {
    krb5_magic magic;
    /* encrypted part: */
    krb5_msgtype msg_type;		/* krb5 message type */
    krb5_keyblock FAR *session;		/* session key */
    krb5_last_req_entry FAR * FAR *last_req; /* array of ptrs to entries */
    krb5_int32 nonce;			/* nonce from request */
    krb5_timestamp key_exp;		/* expiration date */
    krb5_flags flags;			/* ticket flags */
    krb5_ticket_times times;		/* lifetime info */
    krb5_principal server;		/* server's principal identifier */
    krb5_address FAR * FAR *caddrs;	/* array of ptrs to addresses,
					   optional */
} krb5_enc_kdc_rep_part;

typedef struct _krb5_kdc_rep {
    krb5_magic magic;
    /* cleartext part: */
    krb5_msgtype msg_type;		/* AS_REP or KDC_REP? */
    krb5_pa_data FAR * FAR *padata;	/* preauthentication data from KDC */
    krb5_principal client;		/* client's principal identifier */
    krb5_ticket FAR *ticket;		/* ticket */
    krb5_enc_data enc_part;		/* encryption type, kvno, encrypted
					   encoding */
    krb5_enc_kdc_rep_part FAR *enc_part2;/* unencrypted version, if available */
} krb5_kdc_rep;

/* error message structure */
typedef struct _krb5_error {
    krb5_magic magic;
    /* some of these may be meaningless in certain contexts */
    krb5_timestamp ctime;		/* client sec portion; optional */
    krb5_int32 cusec;			/* client usec portion; optional */
    krb5_int32 susec;			/* server usec portion */
    krb5_timestamp stime;		/* server sec portion */
    krb5_ui_4 error;			/* error code (protocol error #'s) */
    krb5_principal client;		/* client's principal identifier;
					   optional */
    krb5_principal server;		/* server's principal identifier */
    krb5_data text;			/* descriptive text */
    krb5_data e_data;			/* additional error-describing data */
} krb5_error;

typedef struct _krb5_ap_req {
    krb5_magic magic;
    krb5_flags ap_options;		/* requested options */
    krb5_ticket FAR *ticket;		/* ticket */
    krb5_enc_data authenticator;	/* authenticator (already encrypted) */
} krb5_ap_req;

typedef struct _krb5_ap_rep {
    krb5_magic magic;
    krb5_enc_data enc_part;
} krb5_ap_rep;

typedef struct _krb5_ap_rep_enc_part {
    krb5_magic magic;
    krb5_timestamp ctime;		/* client time, seconds portion */
    krb5_int32 cusec;			/* client time, microseconds portion */
    krb5_keyblock FAR *subkey;		/* true session key, optional */
    krb5_int32 seq_number;		/* sequence #, optional */
} krb5_ap_rep_enc_part;

typedef struct _krb5_response {
    krb5_magic magic;
    krb5_octet message_type;
    krb5_data response;
} krb5_response;

typedef struct _krb5_safe {
    krb5_magic magic;
    krb5_data user_data;		/* user data */
    krb5_timestamp timestamp;		/* client time, optional */
    krb5_int32 usec;			/* microsecond portion of time,
					   optional */
    krb5_int32 seq_number;		/* sequence #, optional */
    krb5_address FAR *s_address;	/* sender address */
    krb5_address FAR *r_address;	/* recipient address, optional */
    krb5_checksum FAR *checksum;	/* data integrity checksum */
} krb5_safe;

typedef struct _krb5_priv {
    krb5_magic magic;
    krb5_enc_data enc_part;		/* encrypted part */
} krb5_priv;

typedef struct _krb5_priv_enc_part {
    krb5_magic magic;
    krb5_data user_data;		/* user data */
    krb5_timestamp timestamp;		/* client time, optional */
    krb5_int32 usec;			/* microsecond portion of time, opt. */
    krb5_int32 seq_number;		/* sequence #, optional */
    krb5_address FAR *s_address;	/* sender address */
    krb5_address FAR *r_address;	/* recipient address, optional */
} krb5_priv_enc_part;

typedef struct _krb5_cred_info {
    krb5_magic magic;
    krb5_keyblock FAR *session;         /* session key used to encrypt */
					/* ticket */
    krb5_principal client;              /* client name/realm, optional */
    krb5_principal server;              /* server name/realm, optional */
    krb5_flags flags;			/* ticket flags, optional */
    krb5_ticket_times times;		/* auth, start, end, renew_till, */
                                        /* optional */
    krb5_address FAR * FAR *caddrs;	/* array of ptrs to addresses */
} krb5_cred_info;

typedef struct _krb5_cred_enc_part {
    krb5_magic magic;
    krb5_int32 nonce;                   /* nonce, optional */
    krb5_timestamp timestamp;           /* client time */
    krb5_int32 usec;                    /* microsecond portion of time */
    krb5_address FAR *s_address;        /* sender address, optional */
    krb5_address FAR *r_address;        /* recipient address, optional */
    krb5_cred_info FAR * FAR *ticket_info;
} krb5_cred_enc_part;    

typedef struct _krb5_cred {
    krb5_magic magic;
    krb5_ticket FAR * FAR *tickets;	/* tickets */
    krb5_enc_data enc_part;		/* encrypted part */
    krb5_cred_enc_part FAR *enc_part2; 	/* unencrypted version, if available*/
} krb5_cred;

/* Sandia password generation structures */
typedef struct _passwd_phrase_element {
    krb5_magic magic;
    krb5_data FAR *passwd;
    krb5_data FAR *phrase;
} passwd_phrase_element;

typedef struct _krb5_pwd_data {
    krb5_magic magic;
    int sequence_count;
    passwd_phrase_element FAR * FAR *element;
} krb5_pwd_data;

/* these need to be here so the typedefs are available for the prototypes */

/*
 * begin "safepriv.h"
 */

#define KRB5_AUTH_CONTEXT_DO_TIME       0x00000001
#define KRB5_AUTH_CONTEXT_RET_TIME      0x00000002
#define KRB5_AUTH_CONTEXT_DO_SEQUENCE   0x00000004
#define KRB5_AUTH_CONTEXT_RET_SEQUENCE  0x00000008
 
typedef struct krb5_replay_data { 
    krb5_timestamp      timestamp; 
    krb5_int32          usec;
    krb5_int32          seq; 
} krb5_replay_data;

/* flags for krb5_auth_con_genaddrs() */
#define KRB5_AUTH_CONTEXT_GENERATE_LOCAL_ADDR           0x00000001
#define KRB5_AUTH_CONTEXT_GENERATE_REMOTE_ADDR          0x00000002
#define KRB5_AUTH_CONTEXT_GENERATE_LOCAL_FULL_ADDR      0x00000004
#define KRB5_AUTH_CONTEXT_GENERATE_REMOTE_FULL_ADDR     0x00000008

/*
 * end "safepriv.h"
 */


/*
 * begin "ccache.h"
 */

typedef	krb5_pointer	krb5_cc_cursor;	/* cursor for sequential lookup */

typedef struct _krb5_ccache {
    krb5_magic magic;
    struct _krb5_cc_ops FAR *ops;
    krb5_pointer data;
} FAR *krb5_ccache;

typedef struct _krb5_cc_ops {
    krb5_magic magic;
    char FAR *prefix;
    char FAR * (INTERFACE *get_name) NPROTOTYPE((krb5_context, krb5_ccache));
    krb5_error_code (INTERFACE *resolve) NPROTOTYPE((krb5_context, krb5_ccache FAR *,
					    char FAR *));
    krb5_error_code (INTERFACE *gen_new) NPROTOTYPE((krb5_context, krb5_ccache FAR *));
    krb5_error_code (INTERFACE *init) NPROTOTYPE((krb5_context, krb5_ccache,
					    krb5_principal));
    krb5_error_code (INTERFACE *destroy) NPROTOTYPE((krb5_context, krb5_ccache));
    krb5_error_code (INTERFACE *close) NPROTOTYPE((krb5_context, krb5_ccache));
    krb5_error_code (INTERFACE *store) NPROTOTYPE((krb5_context, krb5_ccache,
					    krb5_creds FAR *));
    krb5_error_code (INTERFACE *retrieve) NPROTOTYPE((krb5_context, krb5_ccache,
					    krb5_flags, krb5_creds FAR *,
					    krb5_creds FAR *));
    krb5_error_code (INTERFACE *get_princ) NPROTOTYPE((krb5_context, krb5_ccache,
					    krb5_principal FAR *));
    krb5_error_code (INTERFACE *get_first) NPROTOTYPE((krb5_context, krb5_ccache,
					    krb5_cc_cursor FAR *));
    krb5_error_code (INTERFACE *get_next) NPROTOTYPE((krb5_context, krb5_ccache,
					    krb5_cc_cursor FAR *, krb5_creds FAR *));
    krb5_error_code (INTERFACE *end_get) NPROTOTYPE((krb5_context, krb5_ccache,
					    krb5_cc_cursor FAR *));
    krb5_error_code (INTERFACE *remove_cred) NPROTOTYPE((krb5_context, krb5_ccache,
					    krb5_flags, krb5_creds FAR *));
    krb5_error_code (INTERFACE *set_flags) NPROTOTYPE((krb5_context, krb5_ccache,
					    krb5_flags));
} krb5_cc_ops;

/* for retrieve_cred */
#define	KRB5_TC_MATCH_TIMES		0x00000001
#define	KRB5_TC_MATCH_IS_SKEY		0x00000002
#define	KRB5_TC_MATCH_FLAGS		0x00000004
#define	KRB5_TC_MATCH_TIMES_EXACT	0x00000008
#define	KRB5_TC_MATCH_FLAGS_EXACT	0x00000010
#define	KRB5_TC_MATCH_AUTHDATA		0x00000020
#define	KRB5_TC_MATCH_SRV_NAMEONLY	0x00000040
#define	KRB5_TC_MATCH_2ND_TKT		0x00000080

/* for set_flags and other functions */
#define KRB5_TC_OPENCLOSE		0x00000001

#define krb5_cc_initialize(context, cache, principal) (*(cache)->ops->init)(context, cache, principal)
#define krb5_cc_gen_new(context, cache) (*(cache)->ops->gen_new)(context, cache)
#define krb5_cc_destroy(context, cache) (*(cache)->ops->destroy)(context, cache)
#define krb5_cc_close(context, cache) (*(cache)->ops->close)(context, cache)
#define krb5_cc_store_cred(context, cache, creds) (*(cache)->ops->store)(context, cache, creds)
#define krb5_cc_retrieve_cred(context, cache, flags, mcreds, creds) (*(cache)->ops->retrieve)(context, cache, flags, mcreds, creds)
#define krb5_cc_get_principal(context, cache, principal) (*(cache)->ops->get_princ)(context, cache, principal)
#define krb5_cc_start_seq_get(context, cache, cursor) (*(cache)->ops->get_first)(context, cache, cursor)
#define krb5_cc_next_cred(context, cache, cursor, creds) (*(cache)->ops->get_next)(context, cache, cursor, creds)
#define krb5_cc_end_seq_get(context, cache, cursor) (*(cache)->ops->end_get)(context, cache, cursor)
#define krb5_cc_remove_cred(context, cache, flags, creds) (*(cache)->ops->remove_cred)(context, cache,flags, creds)
#define krb5_cc_set_flags(context, cache, flags) (*(cache)->ops->set_flags)(context, cache, flags)
#define krb5_cc_get_name(context, cache) (*(cache)->ops->get_name)(context, cache)

extern krb5_cc_ops FAR *krb5_cc_dfl_ops;

/*
 * end "ccache.h"
 */

/*
 * begin "rcache.h"
 */

typedef struct krb5_rc_st {
    krb5_magic magic;
    struct _krb5_rc_ops FAR *ops;
    krb5_pointer data;
} FAR *krb5_rcache;

typedef struct _krb5_donot_replay {
    krb5_magic magic;
    char FAR *server;			/* null-terminated */
    char FAR *client;			/* null-terminated */
    krb5_int32 cusec;
    krb5_timestamp ctime;
} krb5_donot_replay;

typedef struct _krb5_rc_ops {
    krb5_magic magic;
    char FAR *type;
    krb5_error_code (*init)NPROTOTYPE((krb5_context, krb5_rcache,krb5_deltat)); /* create */
    krb5_error_code (*recover)NPROTOTYPE((krb5_context, krb5_rcache)); /* open */
    krb5_error_code (*destroy)NPROTOTYPE((krb5_context, krb5_rcache));
    krb5_error_code (*close)NPROTOTYPE((krb5_context, krb5_rcache));
    krb5_error_code (*store)NPROTOTYPE((krb5_context, krb5_rcache,krb5_donot_replay FAR *));
    krb5_error_code (*expunge)NPROTOTYPE((krb5_context, krb5_rcache));
    krb5_error_code (*get_span)NPROTOTYPE((krb5_context, krb5_rcache,krb5_deltat FAR *));
    char FAR *(*get_name)NPROTOTYPE((krb5_context, krb5_rcache));
    krb5_error_code (*resolve)NPROTOTYPE((krb5_context, krb5_rcache, char FAR *));
} krb5_rc_ops;

krb5_error_code krb5_rc_default 
	PROTOTYPE((krb5_context,
		   krb5_rcache FAR *));
krb5_error_code krb5_rc_register_type 
	PROTOTYPE((krb5_context,
		   krb5_rc_ops FAR *));
krb5_error_code krb5_rc_resolve_type 
	PROTOTYPE((krb5_context,
		   krb5_rcache FAR *,char FAR *));
krb5_error_code krb5_rc_resolve_full 
	PROTOTYPE((krb5_context,
		   krb5_rcache FAR *,char FAR *));
char FAR * krb5_rc_get_type 
	PROTOTYPE((krb5_context,
		   krb5_rcache));
char FAR * krb5_rc_default_type 
	PROTOTYPE((krb5_context));
char FAR * krb5_rc_default_name 
	PROTOTYPE((krb5_context));
krb5_error_code krb5_auth_to_rep 
	PROTOTYPE((krb5_context,
		   krb5_tkt_authent FAR *,
		   krb5_donot_replay FAR *));

#define krb5_rc_initialize(context, id, span) (*(id)->ops->init)(context, id, span)
#define krb5_rc_recover(context, id) (*(id)->ops->recover)(context, id)
#define krb5_rc_destroy(context, id) (*(id)->ops->destroy)(context, id)
#define krb5_rc_close(context, id) (*(id)->ops->close)(context, id)
#define krb5_rc_store(context, id, dontreplay) (*(id)->ops->store)(context, id, dontreplay)
#define krb5_rc_expunge(context, id) (*(id)->ops->expunge)(context, id)
#define krb5_rc_get_lifespan(context, id, spanp) (*(id)->ops->get_span)(context, id, spanp)
#define krb5_rc_get_name(context, id) (*(id)->ops->get_name)(context, id)
#define krb5_rc_resolve(context, id, name) (*(id)->ops->resolve)(context, id, name)

extern krb5_rc_ops krb5_rc_dfl_ops;

/*
 * end "rcache.h"
 */

/*
 * begin "keytab.h"
 */


/* XXX */
#define MAX_KEYTAB_NAME_LEN 1100 /* Long enough for MAXPATHLEN + some extra */

typedef krb5_pointer krb5_kt_cursor;	/* XXX */

typedef struct krb5_keytab_entry_st {
    krb5_magic magic;
    krb5_principal principal;	/* principal of this key */
    krb5_timestamp timestamp;   /* time entry written to keytable */
    krb5_kvno vno;		/* key version number */
    krb5_keyblock key;		/* the secret key */
} krb5_keytab_entry;


typedef struct _krb5_kt {
    krb5_magic magic;
    struct _krb5_kt_ops FAR *ops;
    krb5_pointer data;
} FAR *krb5_keytab;


typedef struct _krb5_kt_ops {
    krb5_magic magic;
    char FAR *prefix;
    /* routines always present */
    krb5_error_code (*resolve) 
	NPROTOTYPE((krb5_context,
		    const char FAR *,
		    krb5_keytab FAR *));
    krb5_error_code (*get_name) 
	NPROTOTYPE((krb5_context,
		    krb5_keytab,
		    char FAR *,
		    int));
    krb5_error_code (*close) 
	NPROTOTYPE((krb5_context,
		    krb5_keytab));
    krb5_error_code (*get) 
	NPROTOTYPE((krb5_context,
		    krb5_keytab,
		    krb5_principal,
		    krb5_kvno,
		    krb5_keytype,
		    krb5_keytab_entry FAR *));
    krb5_error_code (*start_seq_get) 
	NPROTOTYPE((krb5_context,
		    krb5_keytab,
		    krb5_kt_cursor FAR *));	
    krb5_error_code (*get_next) 
	NPROTOTYPE((krb5_context,
		    krb5_keytab,
		    krb5_keytab_entry FAR *,
		    krb5_kt_cursor FAR *));
    krb5_error_code (*end_get) 
	NPROTOTYPE((krb5_context,
		    krb5_keytab,
		    krb5_kt_cursor FAR *));
    /* routines to be included on extended version (write routines) */
    krb5_error_code (*add) 
	NPROTOTYPE((krb5_context,
		    krb5_keytab,
		    krb5_keytab_entry FAR *));
    krb5_error_code (*remove) 
	NPROTOTYPE((krb5_context,
		    krb5_keytab,
		  krb5_keytab_entry FAR *));
} krb5_kt_ops;

#define krb5_kt_get_name(context, keytab, name, namelen) (*(keytab)->ops->get_name)(context, keytab,name,namelen)
#define krb5_kt_close(context, keytab) (*(keytab)->ops->close)(context, keytab)
#define krb5_kt_get_entry(context, keytab, principal, vno, keytype, entry) (*(keytab)->ops->get)(context, keytab, principal, vno, keytype, entry)
#define krb5_kt_start_seq_get(context, keytab, cursor) (*(keytab)->ops->start_seq_get)(context, keytab, cursor)
#define krb5_kt_next_entry(context, keytab, entry, cursor) (*(keytab)->ops->get_next)(context, keytab, entry, cursor)
#define krb5_kt_end_seq_get(context, keytab, cursor) (*(keytab)->ops->end_get)(context, keytab, cursor)
/* remove and add are functions, so that they can return NOWRITE
   if not a writable keytab */


extern krb5_kt_ops krb5_kt_dfl_ops;

/*
 * end "keytab.h"
 */

/*
 * begin "func-proto.h"
 */

krb5_error_code INTERFACE krb5_init_context
	PROTOTYPE((krb5_context FAR *));
void krb5_free_context
	PROTOTYPE((krb5_context));

krb5_error_code krb5_set_default_in_tkt_etypes
	PROTOTYPE((krb5_context,
		   const krb5_enctype *));
krb5_error_code krb5_get_default_in_tkt_etypes
	PROTOTYPE((krb5_context,
		   krb5_enctype **));

/* libkrb.spec */
krb5_error_code krb5_kdc_rep_decrypt_proc
	PROTOTYPE((krb5_context,
		   const krb5_keyblock *,
		   krb5_const_pointer,
		   krb5_kdc_rep * ));
krb5_error_code krb5_encode_ticket
	PROTOTYPE((krb5_context,
		   const krb5_ticket *,
		   krb5_data ** ));
krb5_error_code krb5_encrypt_tkt_part
	PROTOTYPE((krb5_context,
		   krb5_encrypt_block *,
		   const krb5_keyblock *,
		   krb5_ticket * ));
krb5_error_code krb5_decrypt_tkt_part
	PROTOTYPE((krb5_context,
		   const krb5_keyblock *,
		   krb5_ticket * ));
krb5_error_code krb5_get_cred_from_kdc
	PROTOTYPE((krb5_context,
		   krb5_ccache,		/* not const, as reading may save
					   state */
		   krb5_creds *,
		   krb5_creds **,
		   krb5_creds *** ));
void krb5_free_tgt_creds
	PROTOTYPE((krb5_context,
		   krb5_creds ** ));	/* XXX too hard to do with const */

#define	KRB5_GC_USER_USER	1	/* want user-user ticket */
#define	KRB5_GC_CACHED		2	/* want cached ticket only */

krb5_error_code INTERFACE krb5_get_credentials
	PROTOTYPE((krb5_context,
		   const krb5_flags,
		   krb5_ccache,
		   krb5_creds FAR *,
		   krb5_creds FAR * FAR *));
krb5_error_code INTERFACE krb5_get_cred_via_tkt
        PROTOTYPE((krb5_context,
                   krb5_creds *,
                   const krb5_flags,
                   krb5_address * const *,
                   krb5_creds *,
                   krb5_creds **));
krb5_error_code krb5_mk_req
	PROTOTYPE((krb5_context,
		   krb5_auth_context **,
		   const krb5_flags,
		   char *,
		   char *,
		   krb5_data *,
		   krb5_ccache,
		   krb5_data * ));
krb5_error_code INTERFACE krb5_mk_req_extended
	PROTOTYPE((krb5_context,
		   krb5_auth_context FAR * FAR *,
		   const krb5_flags,
		   krb5_data FAR *,
		   krb5_creds FAR *,
		   krb5_data FAR * ));
krb5_error_code krb5_mk_rep
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_data *));
krb5_error_code INTERFACE krb5_rd_rep
	PROTOTYPE((krb5_context,
		   krb5_auth_context FAR *,
		   const krb5_data FAR *,
		   krb5_ap_rep_enc_part FAR * FAR *));
krb5_error_code krb5_mk_error
	PROTOTYPE((krb5_context,
		   const krb5_error *,
		   krb5_data * ));
krb5_error_code krb5_rd_error
	PROTOTYPE((krb5_context,
		   const krb5_data *,
		   krb5_error ** ));
krb5_error_code krb5_rd_safe
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   const krb5_data *,
		   krb5_data *,
		   krb5_replay_data *));
krb5_error_code krb5_rd_priv
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   const krb5_data *,
		   krb5_data *,
		   krb5_replay_data *));
krb5_error_code INTERFACE krb5_parse_name
	PROTOTYPE((krb5_context,
		   const char FAR *,
		   krb5_principal FAR * ));
krb5_error_code INTERFACE krb5_unparse_name
	PROTOTYPE((krb5_context,
		   krb5_const_principal,
		   char FAR * FAR * ));
krb5_error_code krb5_unparse_name_ext
	PROTOTYPE((krb5_context,
		   krb5_const_principal,
		   char **,
		   int *));
krb5_boolean krb5_address_search
	PROTOTYPE((krb5_context,
		   const krb5_address *,
		   krb5_address * const *));
krb5_boolean krb5_address_compare
	PROTOTYPE((krb5_context,
		   const krb5_address *,
		   const krb5_address *));
int krb5_address_order
	PROTOTYPE((krb5_context,
		   const krb5_address *,
		   const krb5_address *));
krb5_boolean krb5_realm_compare
	PROTOTYPE((krb5_context,
		   krb5_const_principal,
		   krb5_const_principal));
krb5_boolean krb5_principal_compare
	PROTOTYPE((krb5_context,
		   krb5_const_principal,
		   krb5_const_principal));
int krb5_fulladdr_order
	PROTOTYPE((krb5_context,
		   const krb5_fulladdr *,
		   const krb5_fulladdr *));
krb5_error_code krb5_copy_keyblock
    	PROTOTYPE((krb5_context,
		   const krb5_keyblock *,
	       krb5_keyblock **));
krb5_error_code krb5_copy_keyblock_contents
    	PROTOTYPE((krb5_context,
		   const krb5_keyblock *,
	       krb5_keyblock *));
krb5_error_code krb5_copy_creds
    	PROTOTYPE((krb5_context,
		   const krb5_creds *,
	       krb5_creds **));
krb5_error_code krb5_copy_data
    	PROTOTYPE((krb5_context,
		   const krb5_data *,
	       krb5_data **));
krb5_error_code krb5_copy_principal
    	PROTOTYPE((krb5_context,
		   krb5_const_principal,
	       krb5_principal *));
krb5_error_code krb5_copy_addresses
    	PROTOTYPE((krb5_context,
		   krb5_address * const *,
	       krb5_address ***));
krb5_error_code krb5_copy_ticket
    	PROTOTYPE((krb5_context,
		   const krb5_ticket *, krb5_ticket **));
krb5_error_code krb5_copy_authdata
    	PROTOTYPE((krb5_context,
		   krb5_authdata * const *,
	       krb5_authdata ***));
krb5_error_code krb5_copy_authenticator
    	PROTOTYPE((krb5_context,
		   const krb5_authenticator *,
	       krb5_authenticator **));
krb5_error_code krb5_copy_checksum
    	PROTOTYPE((krb5_context,
		   const krb5_checksum *,
	       krb5_checksum **));
void INTERFACE krb5_init_ets PROTOTYPE((krb5_context));
krb5_error_code krb5_generate_subkey
    	PROTOTYPE((krb5_context,
		   const krb5_keyblock *, krb5_keyblock **));
krb5_error_code krb5_generate_seq_number
    	PROTOTYPE((krb5_context,
		   const krb5_keyblock *, krb5_int32 *));
krb5_error_code krb5_get_server_rcache
    	PROTOTYPE((krb5_context,
		   const krb5_data *, krb5_rcache *));
krb5_error_code INTERFACE_C krb5_build_principal_ext
    	STDARG_P((krb5_context, krb5_principal FAR *, int, const char FAR *, ...));
krb5_error_code krb5_build_principal
    	STDARG_P((krb5_context, krb5_principal *, int, const char *, ...));
#ifdef va_start
/* XXX depending on varargs include file defining va_start... */
krb5_error_code krb5_build_principal_va
    	PROTOTYPE((krb5_context,
		   krb5_principal *, int, const char *, va_list));
#endif

krb5_error_code krb5_425_conv_principal
	PROTOTYPE((krb5_context,
		   const char *name, const char *instance, const char *realm,
		   krb5_principal *princ));

krb5_error_code krb5_obtain_padata
    	PROTOTYPE((krb5_context,
		   int type, krb5_principal client, krb5_address **src_addr,
	           krb5_keyblock *encrypt_key, krb5_pa_data **data));

krb5_error_code krb5_verify_padata
    	PROTOTYPE((krb5_context,
		   krb5_pa_data * data, krb5_principal client,
	       krb5_address **src_addr, krb5_keyblock *decrypt_key,
	       int *req_id, int *flags));

/* libkt.spec */
krb5_error_code krb5_kt_register
	PROTOTYPE((krb5_context,
		   krb5_kt_ops * ));
krb5_error_code krb5_kt_resolve
	PROTOTYPE((krb5_context,
		   const char *,
		   krb5_keytab * ));
krb5_error_code krb5_kt_default_name
	PROTOTYPE((krb5_context,
		   char *,
		   int ));
krb5_error_code krb5_kt_default
	PROTOTYPE((krb5_context,
		   krb5_keytab * ));
krb5_error_code krb5_kt_free_entry
	PROTOTYPE((krb5_context,
		   krb5_keytab_entry * ));
/* remove and add are functions, so that they can return NOWRITE
   if not a writable keytab */
krb5_error_code krb5_kt_remove_entry
	PROTOTYPE((krb5_context,
		   krb5_keytab,
		   krb5_keytab_entry * ));
krb5_error_code krb5_kt_add_entry
	PROTOTYPE((krb5_context,
		   krb5_keytab,
		   krb5_keytab_entry * ));
krb5_error_code krb5_principal2salt
	PROTOTYPE((krb5_context,
		   krb5_const_principal, krb5_data *));
krb5_error_code krb5_principal2salt_norealm
	PROTOTYPE((krb5_context,
		   krb5_const_principal, krb5_data *));

/* librc.spec--see rcache.h */

/* libcc.spec */
krb5_error_code krb5_cc_resolve
	PROTOTYPE((krb5_context,
		   char *,
		   krb5_ccache * ));
krb5_error_code krb5_cc_generate_new
	PROTOTYPE((krb5_context,
		   krb5_cc_ops *,
		   krb5_ccache * ));
char * krb5_cc_default_name
	PROTOTYPE((krb5_context));
krb5_error_code INTERFACE krb5_cc_default
	PROTOTYPE((krb5_context,
		   krb5_ccache FAR *));
unsigned int INTERFACE krb5_get_notification_message
   PROTOTYPE((void));

/* chk_trans.c */
krb5_error_code krb5_check_transited_list
    PROTOTYPE((krb5_context,
		   krb5_data *trans, krb5_data *realm1, krb5_data *realm2));

/* free_rtree.c */
void krb5_free_realm_tree
	PROTOTYPE((krb5_context,
		   krb5_principal *));

/* krb5_free.c */
void INTERFACE krb5_free_principal
	PROTOTYPE((krb5_context,
		   krb5_principal ));
void krb5_free_authenticator
	PROTOTYPE((krb5_context,
		   krb5_authenticator * ));
void krb5_free_authenticator_contents
	PROTOTYPE((krb5_context,
		   krb5_authenticator * ));
void INTERFACE krb5_free_addresses
	PROTOTYPE((krb5_context,
		   krb5_address FAR * FAR * ));
void krb5_free_address
	PROTOTYPE((krb5_context,
		   krb5_address * ));
void krb5_free_authdata
	PROTOTYPE((krb5_context,
		   krb5_authdata ** ));
void krb5_free_enc_tkt_part
	PROTOTYPE((krb5_context,
		   krb5_enc_tkt_part * ));
void krb5_free_ticket
	PROTOTYPE((krb5_context,
		   krb5_ticket * ));
void krb5_free_tickets
	PROTOTYPE((krb5_context,
		   krb5_ticket ** ));
void krb5_free_kdc_req
	PROTOTYPE((krb5_context,
		   krb5_kdc_req * ));
void krb5_free_kdc_rep
	PROTOTYPE((krb5_context,
		   krb5_kdc_rep * ));
void krb5_free_last_req
	PROTOTYPE((krb5_context,
		   krb5_last_req_entry ** ));
void krb5_free_enc_kdc_rep_part
	PROTOTYPE((krb5_context,
		   krb5_enc_kdc_rep_part * ));
void krb5_free_error
	PROTOTYPE((krb5_context,
		   krb5_error * ));
void krb5_free_ap_req
	PROTOTYPE((krb5_context,
		   krb5_ap_req * ));
void krb5_free_ap_rep
	PROTOTYPE((krb5_context,
		   krb5_ap_rep * ));
void krb5_free_safe
	PROTOTYPE((krb5_context,
		   krb5_safe * ));
void krb5_free_priv
	PROTOTYPE((krb5_context,
		   krb5_priv * ));
void krb5_free_priv_enc_part
	PROTOTYPE((krb5_context,
		   krb5_priv_enc_part * ));
void krb5_free_cred
   PROTOTYPE((krb5_context, 
         krb5_cred *));
void INTERFACE krb5_free_creds
	PROTOTYPE((krb5_context,
		   krb5_creds FAR *));
void INTERFACE krb5_free_cred_contents
	PROTOTYPE((krb5_context,
		   krb5_creds FAR *));
void krb5_free_cred_enc_part
   PROTOTYPE((krb5_context,
         krb5_cred_enc_part *));
void krb5_free_checksum
	PROTOTYPE((krb5_context,
		   krb5_checksum *));
void krb5_free_keyblock
	PROTOTYPE((krb5_context,
		   krb5_keyblock *));
void krb5_free_pa_data
	PROTOTYPE((krb5_context,
		   krb5_pa_data **));
void INTERFACE krb5_free_ap_rep_enc_part
	PROTOTYPE((krb5_context,
		   krb5_ap_rep_enc_part FAR *));
void krb5_free_tkt_authent
	PROTOTYPE((krb5_context,
		   krb5_tkt_authent *));
void krb5_free_pwd_data
   PROTOTYPE((krb5_context,
         krb5_pwd_data *));
void krb5_free_pwd_sequences
   PROTOTYPE((krb5_context,
         passwd_phrase_element **));

/* From krb5/os but needed but by the outside world */
krb5_error_code INTERFACE krb5_us_timeofday
	PROTOTYPE((krb5_context,
		   krb5_int32 FAR *,
		   krb5_int32 FAR * ));
krb5_error_code INTERFACE krb5_timeofday
	PROTOTYPE((krb5_context,
		   krb5_int32 FAR * ));
		 /* get all the addresses of this host */
krb5_error_code INTERFACE krb5_os_localaddr
	PROTOTYPE((krb5_address FAR * FAR * FAR *));
krb5_error_code INTERFACE krb5_get_default_realm
	PROTOTYPE((krb5_context,
		    char FAR * FAR * ));
krb5_error_code INTERFACE krb5_set_default_realm
	PROTOTYPE((krb5_context,
                   const char FAR * ));
krb5_error_code INTERFACE krb5_sname_to_principal
        PROTOTYPE((krb5_context,
		   const char FAR *,
                   const char FAR *,
                   krb5_int32,
                   krb5_principal FAR *));

krb5_error_code INTERFACE krb5_set_config_files
	PROTOTYPE ((krb5_context, const char **));

krb5_error_code krb5_encode_kdc_rep
	PROTOTYPE((krb5_context,
		   const krb5_msgtype,
		   const krb5_enc_kdc_rep_part *,
		   krb5_encrypt_block *,
		   const krb5_keyblock *,
		   krb5_kdc_rep *,
		   krb5_data ** ));

krb5_error_code krb5_send_tgs
	PROTOTYPE((krb5_context,
		   const krb5_flags,
		   const krb5_ticket_times *,
		   const krb5_enctype *,
		   krb5_const_principal,
		   krb5_address * const *,
		   krb5_authdata * const *,
		   krb5_pa_data * const *,
		   const krb5_data *,
		   krb5_creds *,
		   krb5_response * ));

krb5_error_code krb5_get_in_tkt
	PROTOTYPE((krb5_context,
		   const krb5_flags,
		   krb5_address * const *,
		   krb5_enctype *,
		   krb5_preauthtype *,
		   krb5_error_code ( * )(krb5_context,
					const krb5_keytype,
                                        krb5_data *,
                                        krb5_const_pointer,
                                        krb5_keyblock **),
		   krb5_const_pointer,
		   krb5_error_code ( * )(krb5_context,
					const krb5_keyblock *,
					krb5_const_pointer,
					krb5_kdc_rep * ),
		   krb5_const_pointer,
		   krb5_creds *,
		   krb5_ccache,
		   krb5_kdc_rep ** ));

krb5_error_code INTERFACE krb5_get_in_tkt_with_password
	PROTOTYPE((krb5_context,
		   const krb5_flags,
		   krb5_address FAR * const FAR *,
		   krb5_enctype FAR *,
		   krb5_preauthtype FAR *,
		   const char FAR *,
		   krb5_ccache,
		   krb5_creds FAR *,
		   krb5_kdc_rep FAR * FAR * ));

krb5_error_code krb5_get_in_tkt_with_skey
	PROTOTYPE((krb5_context,
		   const krb5_flags,
		   krb5_address * const *,
		   krb5_enctype *,
		   krb5_preauthtype *,
		   const krb5_keyblock *,
		   krb5_ccache,
		   krb5_creds *,
		   krb5_kdc_rep ** ));

krb5_error_code krb5_get_in_tkt_with_keytab
	PROTOTYPE((krb5_context,
		   const krb5_flags,
		   krb5_address * const *,
		   krb5_enctype *,
		   krb5_preauthtype *,
		   const krb5_keytab,
		   krb5_ccache,
		   krb5_creds *,
		   krb5_kdc_rep ** ));


krb5_error_code krb5_decode_kdc_rep
	PROTOTYPE((krb5_context,
		   krb5_data *,
		   const krb5_keyblock *,
		   const krb5_enctype,
		   krb5_kdc_rep ** ));

krb5_error_code krb5_rd_req
	PROTOTYPE((krb5_context,
		   krb5_auth_context **,
		   const krb5_data *,
		   krb5_const_principal,
		   krb5_keytab,
		   krb5_flags *,
		   krb5_ticket **));

krb5_error_code krb5_rd_req_decoded
	PROTOTYPE((krb5_context,
		   krb5_auth_context **,
		   const krb5_ap_req *,
		   krb5_const_principal,
		   krb5_keytab,
		   krb5_flags *,
		   krb5_ticket **));

krb5_error_code krb5_kt_read_service_key
	PROTOTYPE((krb5_context,
		   krb5_pointer,
		   krb5_principal,
		   krb5_kvno,
		   krb5_keytype,
		   krb5_keyblock **));
krb5_error_code krb5_mk_safe
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   const krb5_data *,
		   krb5_data *,
		   krb5_replay_data *));
krb5_error_code krb5_mk_priv
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   const krb5_data *,
		   krb5_data *,
		   krb5_replay_data *));
krb5_error_code krb5_cc_register
	PROTOTYPE((krb5_context,
		   krb5_cc_ops *,
		   krb5_boolean ));

krb5_error_code krb5_sendauth 
	PROTOTYPE((krb5_context,
		   krb5_auth_context **,
		   krb5_pointer,
		   char *,
		   krb5_principal,
		   krb5_principal,
		   krb5_flags,
		   krb5_data *,
		   krb5_creds *,
		   krb5_ccache,
		   krb5_error **,
		   krb5_ap_rep_enc_part **,
		   krb5_creds **));
	
krb5_error_code krb5_recvauth PROTOTYPE((krb5_context,
		   krb5_auth_context **,
		   krb5_pointer,
		   char *,
		   krb5_principal,
		   krb5_int32, 
		   krb5_keytab,
		   krb5_ticket **));

krb5_error_code krb5_walk_realm_tree
    	PROTOTYPE((krb5_context,
		   const krb5_data *,
	       const krb5_data *,
	       krb5_principal **,
	       int));

krb5_error_code INTERFACE krb5_mk_ncred
    	PROTOTYPE((krb5_context,
                   krb5_auth_context *,
                   krb5_creds **,
                   krb5_data **,
                   krb5_replay_data *));

krb5_error_code INTERFACE krb5_mk_1cred
        PROTOTYPE((krb5_context,
                   krb5_auth_context *,
                   krb5_creds *,
                   krb5_data **,
                   krb5_replay_data *));

krb5_error_code krb5_auth_con_init
	PROTOTYPE((krb5_context,
		   krb5_auth_context **));

krb5_error_code krb5_auth_con_free
	PROTOTYPE((krb5_context,
		   krb5_auth_context *));

krb5_error_code krb5_auth_con_setflags
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_int32));

krb5_error_code krb5_auth_con_getflags
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_int32 *));

krb5_error_code krb5_auth_con_setaddrs
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_address *,
		   krb5_address *));

krb5_error_code krb5_auth_con_getaddrs
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_address **,
		   krb5_address **));

krb5_error_code krb5_auth_con_setports
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_address *,
		   krb5_address *));

krb5_error_code krb5_auth_con_setuseruserkey
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_keyblock *));

krb5_error_code krb5_auth_con_getkey
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_keyblock **));

krb5_error_code krb5_auth_con_getlocalsubkey
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_keyblock **));

krb5_error_code krb5_auth_con_setcksumtype
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_cksumtype));

krb5_error_code krb5_auth_con_getcksumtype
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_cksumtype *));

krb5_error_code krb5_auth_con_getlocalseqnumber
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_int32 *));

krb5_error_code krb5_auth_con_getremoteseqnumber
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_int32 *));

krb5_error_code krb5_auth_con_setivector
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_pointer));

krb5_error_code krb5_auth_con_getivector
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_pointer *));

krb5_error_code krb5_auth_con_setrcache
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_rcache));

krb5_error_code krb5_auth_con_getrcache
	PROTOTYPE((krb5_context,
		   krb5_auth_context *,
		   krb5_rcache *));

krb5_error_code krb5_auth_con_getauthenticator
   PROTOTYPE((krb5_context,
         krb5_auth_context *,
         krb5_authenticator **));

krb5_error_code krb5_auth_con_getremotesubkey
   PROTOTYPE((krb5_context,
         krb5_auth_context *,
         krb5_keyblock **));

#define KRB5_REALM_BRANCH_CHAR '.'

/*
 * end "func-proto.h"
 */

/*
 * begin "k5-free.h"
 */

/* to keep lint happy */
#define krb5_xfree(val) free((char FAR *)(val))
#ifdef OSF_DCE
#define xfree(val) free((char *)(val))
#endif

#define krb5_free_data(context, val) { krb5_xfree((val)->data); krb5_xfree(val);}

/*
 * end "k5-free.h"
 */

/* The name of the Kerberos ticket granting service... and its size */
#define	KRB5_TGS_NAME		"krbtgt"
#define KRB5_TGS_NAME_SIZE	6

/* flags for recvauth */
#define KRB5_RECVAUTH_SKIP_VERSION	0x0001
#define KRB5_RECVAUTH_BADAUTHVERS	0x0002

#endif /* KRB5_GENERAL__ */
