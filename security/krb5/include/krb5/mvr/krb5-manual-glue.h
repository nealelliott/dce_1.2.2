/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krb5-manual-glue.h,v $
 * Revision 1.1.6.1  1996/10/03  14:57:31  arvind
 * 	detect malloc failures and return an error.
 *
 * 	Add support for optional octet strings.
 *
 * 	code cleanup.
 * 	add `const' to a bunch of places.
 * 	[1996/09/16  21:59 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.4.2  1996/02/18  23:02:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:21  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:43:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:17  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:01:03  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:53:28  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  krb5-manual-glue.h V=3 02/21/91 //littl/prgy/krb5/include/krb5/mvr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* krb5-manual-glue.h
 * 
 * Copyright Hewlett-Packard Company 1990, 1991
 *  Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 *
 *
 * VersionID = "@(#)krb5-manual-glue.h	3 - 02/21/91";
 */

krb5_error_code asn1_field2krb5_flags (const asn1_field *m, krb5_flags *k);
krb5_error_code krb5_flags2asn1_field (const krb5_flags *k, asn1_field *m);

krb5_error_code asn1_field2krb5_data (const asn1_field *m, krb5_data *k);
krb5_error_code krb5_data2asn1_field (const krb5_data *k, asn1_field *m);

krb5_error_code asn1_field2krb5_octets (const asn1_field *m, int *l, krb5_octet **c);
krb5_error_code krb5_octets2asn1_field (const int *l, krb5_octet * const *c, asn1_field *m);

krb5_error_code opt_krb5_octets2asn1_field (int const *l, krb5_octet * const* c, asn1_field **m);
krb5_error_code opt_asn1_field2krb5_octets (asn1_field const* m, int *l, krb5_octet **c);

krb5_error_code PrincipalRealm2krb5_principal 
    (const PrincipalName *n, const Realm *r, krb5_principal *k);
krb5_error_code krb5_principal2PrincipalRealm
    (const krb5_principal *k, PrincipalName *n, Realm *r);


krb5_error_code OPrincipalRealm2krb5_principal 
    (PrincipalName * const *n, const Realm *r, krb5_principal *k);
krb5_error_code krb5_principal2OPrincipalRealm 
    (const krb5_principal *k, PrincipalName **n, Realm *r);

krb5_error_code OPrincipalORealm2krb5_principal 
    (PrincipalName * const* n, Realm * const* r, krb5_principal *k);
krb5_error_code krb5_principal2OPrincipalORealm 
    (const krb5_principal *k, PrincipalName **n, Realm **r);


krb5_error_code krb5_timestamp2asn1_field (const krb5_timestamp *k, asn1_field *m);
krb5_error_code asn1_field2krb5_timestamp (const asn1_field *m, krb5_timestamp *k);

krb5_error_code krb5_enctype_vec2INTSEQ
    (krb5_enctype * const* k, const int *n, INTSEQ *m);
krb5_error_code INTSEQ2krb5_enctype_vec (const INTSEQ *m, krb5_enctype **k, int *n);
