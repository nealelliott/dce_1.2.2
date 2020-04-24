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
 * $Log: asn1_decode.h,v $
 * Revision 1.1.2.1  1996/06/05  20:33:12  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:36:58  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * src/lib/krb5/asn.1/asn1_decode.h
 * 
 * Copyright 1994 by the Massachusetts Institute of Technology.
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
 */

#ifndef __ASN1_DECODE_H__
#define __ASN1_DECODE_H__

#include "k5-int.h"
#include "krbasn1.h"
#include "asn1buf.h"

/*
   Overview

     These procedures take an asn1buf whose current position points
     to the beginning of an ASN.1 primitive (<id><length><contents>).
     The primitive is removed from the buffer and decoded.

   Operations

    asn1_decode_integer
    asn1_decode_unsigned_integer
    asn1_decode_octetstring
    asn1_decode_charstring
    asn1_decode_generalstring
    asn1_decode_null
    asn1_decode_printablestring
    asn1_decode_ia5string
    asn1_decode_generaltime
*/

/* asn1_error_code asn1_decode_type(asn1buf *buf, ctype *val); */
/* requires  *buf is allocated
   modifies  *buf, *len
   effects   Decodes the octet string in *buf into *val.
             Returns ENOMEM if memory is exhausted.
	     Returns asn1 errors. */

asn1_error_code asn1_decode_integer
	PROTOTYPE((asn1buf *buf, long *val));
asn1_error_code asn1_decode_unsigned_integer
	PROTOTYPE((asn1buf *buf, unsigned long *val));
asn1_error_code asn1_decode_null
	PROTOTYPE((asn1buf *buf));

asn1_error_code asn1_decode_octetstring
	PROTOTYPE((asn1buf *buf, int *retlen, asn1_octet **val));
asn1_error_code asn1_decode_generalstring
	PROTOTYPE((asn1buf *buf, int *retlen, char **val));
asn1_error_code asn1_decode_charstring
	PROTOTYPE((asn1buf *buf, int *retlen, char **val));
/* Note: A charstring is a special hack to account for the fact that
         krb5 structures store some OCTET STRING values in krb5_octet
	 arrays and others in krb5_data structures 
	PROTOTYPE((which use char arrays).
	 From the ASN.1 point of view, the two string types are the same,
	 only the receptacles differ. */
asn1_error_code asn1_decode_printablestring
	PROTOTYPE((asn1buf *buf, int *retlen, char **val));
asn1_error_code asn1_decode_ia5string
	PROTOTYPE((asn1buf *buf, int *retlen, char **val));

asn1_error_code asn1_decode_generaltime
	PROTOTYPE((asn1buf *buf, time_t *val));

#endif
