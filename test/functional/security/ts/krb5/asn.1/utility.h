/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 * Copyright (c) Hewlett-Packard Company 1996
 * Unpublished work. All Rights Reserved.
 */
/*
 * HISTORY
 * $Log: utility.h,v $
 * Revision 1.1.2.1  1996/09/25  20:56:17  arvind
 * 	OSF DCE 1.2.2 - missing files
 * 	[1996/09/25  20:55:45  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/06/20  18:18 UTC  mullan_s
 * 	Merge to DCE_1.2.2
 * 
 * Revision /main/mullan_dce_krb5_der_work/1  1996/06/10  19:40 UTC  mullan_s
 * 	Port to DCE.
 * 
 * $EndLog$
 */
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "krb5.h"
#ifndef OSF_DCE
#include "krbasn1.h"
#include "asn1buf.h"
#endif

#ifdef OSF_DCE
typedef krb5_error_code asn1_error_code;
typedef struct code_buffer_rep {
  char *base, *bound, *next;
} asn1buf;
#endif

asn1_error_code asn1_krb5_data_unparse
	PROTOTYPE((const krb5_data *code, char **s));
/* modifies  *s;
   effects   Instantiates *s with a string representation of the series
	      of hex octets in *code.  (e.g. "02 02 00 7F")  If code==NULL,
	      the string rep is "<NULL>".  If code is empty (it contains no
	      data or has length <= 0), the string rep is "<EMPTY>".
	     If *s is non-NULL, then its currently-allocated storage
	      will be freed prior to the instantiation.
	     Returns ENOMEM or the string rep cannot be created. */

krb5_error_code krb5_data_parse
	PROTOTYPE((krb5_data *d, const char *s));
/* effects  Parses character string *s into krb5_data *d. */

krb5_error_code krb5_data_hex_parse
	PROTOTYPE((krb5_data *d, const char *s));
/* requires  *s is the string representation of a sequence of
              hexadecimal octets.  (e.g. "02 01 00")
   effects  Parses *s into krb5_data *d. */

void asn1buf_print
	PROTOTYPE((const asn1buf *buf));

#endif
