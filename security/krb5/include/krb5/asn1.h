/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: asn1.h,v $
 * Revision 1.1.6.1  1996/10/03  14:55:15  arvind
 * 	Simplify; remove direct dependancies on mavros from this file.
 * 	The interface to the asn.1 coder subsystem is now not dependant at
 * 	*all* on the output of mavros, which should reduce the churn when we
 * 	tweak the asn.1 defs..
 * 	[1996/09/16  21:25 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.4.2  1996/02/18  23:02:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:32  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:41:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:52  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:57:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:51:41  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  asn1.h V=3 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *  ASN.1 support (using MAVROS ASN1 package)
 */

#ifndef KRB5_ASN1__
#define KRB5_ASN1__

typedef krb5_authdata *krb5_authdata_vec;
typedef krb5_pa_data *krb5_pa_data_vec;
typedef krb5_etype_info_entry *krb5_etype_info_entry_vec;

#include <krb5/mvr/encode.h>

#endif
