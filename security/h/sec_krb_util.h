/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_krb_util.h,v $
 * Revision 1.1.12.1  1996/10/03  14:51:49  arvind
 * 	two cleanups:
 * 	1) expunge sec_krb_name_parsed_cmp in favor of separate functions
 * 	for each type of compare
 *
 * 	we leave it defined as a bogon to catch use of it at compile time.
 *
 * 	2) correct type of pwd_keyproc and its argument to match reality.
 * 	[1996/09/16  20:36 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.10.3  1996/02/18  23:01:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:28  marty]
 * 
 * Revision 1.1.10.2  1995/12/08  17:29:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:18  root]
 * 
 * Revision 1.1.6.1  1994/10/25  17:18:35  mob
 * 	Fix for OT 12545
 * 	[1994/10/24  20:38:22  mob]
 * 
 * Revision 1.1.4.1  1994/07/26  18:03:56  rsalz
 * 	Add declaration of sec_krb_tgtprinc_from_dce_cell_pair (OT CR 11446).
 * 	[1994/07/26  17:36:57  rsalz]
 * 
 * Revision 1.1.2.2  1992/12/29  13:27:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:16:09  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sec_krb_util.h V=7 12/10/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 */

#ifndef __sec_krb_util_h_included__
#define __sec_krb_util_h_included__

#include <krb5/base-defs.h>
#include <krb5/krb5.h>
#include <krb5/encryption.h>
#include <dce/nbase.h>
#include <dce/sec_base.h>
#include <dce/passwd.h>

/* a handy reference to the architectural priv server principal name */
extern char  *ptgs_princ_name;

/*
 * sec_krb_parse_name and sec_krb_unparse_name are declared in sec_authn.idl
 */

void sec_krb_string_from_dce_pair (
#ifdef __STDC__
    char            *cell_name,
    char            *princ_name,
    char            **krb_princ,
    error_status_t  *stp
#endif
);

void sec_krb_tgtprinc_from_dce_cell_pair (
#ifdef __STDC__
    char            *cell_name,        /* [in] */
    char            *princ_name,       /* [in] */
    char            **krb_princ,       /* [out] */
    error_status_t  *stp               /* [out] */
#endif
);

void sec_krb_princ_from_dce_pair ( 
#ifdef __STDC__
    char            *cell_name,        /* [in] */
    char            *princ_name,       /* [in] */
    krb5_principal  *krb_princ,        /* [out] */
    error_status_t  *stp               /* [out] */
#endif
);

void sec_dce_pair_from_krb_princ ( 
#ifdef __STDC__
    krb5_principal  krb_princ,        /* [in] */
    char            **cell_name,      /* [out] */
    char            **princ_name,     /* [out] */
    error_status_t  *stp              /* [out] */
#endif
);


/* sec_krb_tgs_parsed_name and sec_krb_ptgs_parsed_name return 
 * krb5 principal names for the ticket-granting-ticket server and
 * DCE privilege server, respectively, in internal krb5_principal
 * format.
 */

void sec_krb_tgs_parsed_name ( 
#ifdef __STDC__
    char            *cell_name,       /* [in] */
    krb5_principal  *tgs_parsed_name, /* [out] */
    error_status_t  *stp              /* [out] */
#endif
);

#define sec_krb_ptgs_parsed_name(cell_name, ptgs_princ, stp) \
    sec_krb_princ_from_dce_pair((cell_name), ptgs_princ_name, (ptgs_princ), (stp)) 

void sec_krb_tgs_parsed_name_from_dce_pair (
#ifdef __STDC__
    char            *client_cell_name,/* [in] */
    char            *server_cell_name,/* [in] */
    krb5_principal  *tgs_parsed_name, /* [out] */
    error_status_t  *stp              /* [out] */
#endif
);


/* 
 * convert a DCE cell name (in global format) to a kerberos realm name,
 * and return the result.  The result is a pointer to dynamic storage
 * and must be freed by the caller when no longer needed.  The input
 * cell_name may not be NULL.  If the input is not a valid DCE global
 * name, the function returns its input.  This function performs
 * a purely local, syntactic evaluation of the cell_name. It does not
 * verify that the cell actually exists.
 *
 * sec_krb_realm_to_cell converts a realm name into the corresponding
 * DCE global cell name.  Storage considerations are the same as for
 * sec_krb_realm_from_dce_cell.
 */

char  * sec_krb_realm_from_dce_cell ( 
#ifdef __STDC__
    char            *cell_name       /* [in] */
#endif
);

char  * sec_krb_realm_to_cell_len ( 
#ifdef __STDC__
    unsigned32      len,
    char            *realm           /* [in] */
#endif
);

char  * sec_krb_realm_to_cell ( 
#ifdef __STDC__
    char            *realm           /* [in] */
#endif
);

/* 
 * function to compare principal, realm, or both component(s) 
 * of two krb5 principals
 */
#define sec_krb_name_cmp_princ @ERROR_BOGON@
#define sec_krb_name_cmp_realm @ERROR_BOGON@
#define sec_krb_name_cmp_whole   @ERROR_BOGON@

#define sec_krb_name_parsed_cmp(p1,p2,p3) @ERROR_BOGON@

boolean32 sec_krb_name_parsed_cmp_princ (
    krb5_principal  princ1,
    krb5_principal  princ2
);

boolean32 sec_krb_name_parsed_cmp_realm (
    krb5_principal  princ1,
    krb5_principal  princ2
);

#define sec_krb_name_parsed_cmp_whole(p1,p2) krb5_principal_compare(p1,p2)

krb5_keytype  sec_to_krb_keytype (
#ifdef __STDC__
    sec_passwd_type_t sec_keytype,
    error_status_t    *stp
#endif
);

sec_passwd_type_t  krb_to_sec_keytype (
#ifdef __STDC__
    long int /* krb5_keytype */     krb_keytype,
    error_status_t                  *stp
#endif
);

krb5_keyblock *  sec_to_krb_key (
#ifdef __STDC__
    sec_passwd_rec_t  *sec_key,
    error_status_t    *stp
#endif
);

sec_passwd_rec_t *  krb_to_sec_key (
#ifdef __STDC__
    krb5_keyblock     *krb_key,
    error_status_t    *stp
#endif
);

krb5_enctype  sec_to_krb_etype (
#ifdef __STDC__
    sec_etype_t     sec_etype,
    error_status_t  *stp
#endif
);

krb5_enctype  sec_to_krb_chksum_type (
#ifdef __STDC__
    sec_chksum_type_t  sec_chksum_type,
    error_status_t     *stp
#endif
);


/* 
 * declarations needed to make use of the DCE version of the kerberos
 * TGT acquisition API
 */ 
      
#if 0
struct pwd_keyproc_arg {
    krb5_principal who;
    krb5_data password;
};
#else
#define pwd_keyproc_arg @@OBSOLETE@@
#endif
  
/*  declaration of string password keyproc defined in in_tkt_pwd.c */
extern krb5_error_code pwd_keyproc(krb5_context ctxt,
				   krb5_const krb5_enctype type,
				   krb5_data *salt,
				   krb5_const_pointer keyseed,
				   krb5_keyblock **key);

/* Definition of sec_id_globalize_name implemented in sec_authn.c */
error_status_t sec_id_globalize_name (
#ifdef __STDC__
    unsigned char   *name,
    unsigned char   **global_name
#endif
);

#endif
