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
 * $Log: asn1_encode.h,v $
 * Revision 1.1.2.1  1996/06/05  20:33:34  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:37:17  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * src/lib/krb5/asn.1/asn1_encode.h
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

#ifndef __ASN1_ENCODE_H__
#define __ASN1_ENCODE_H__

#include "k5-int.h"
#include <time.h>
#include "krbasn1.h"
#include "asn1buf.h"

/*
   Overview

     Each of these procedures inserts the encoding of an ASN.1
     primitive in a coding buffer.

   Operations

     asn1_encode_integer
     asn1_encode_octetstring
     asn1_encode_null
     asn1_encode_printablestring
     asn1_encode_ia5string
     asn1_encode_generaltime
     asn1_encode_generalstring
*/

asn1_error_code asn1_encode_integer
	PROTOTYPE((asn1buf *buf, const long val, int *retlen));
/* requires  *buf is allocated
   modifies  *buf, *retlen
   effects   Inserts the encoding of val into *buf and returns 
              the length of the encoding in *retlen.
             Returns ENOMEM to signal an unsuccesful attempt
              to expand the buffer. */

asn1_error_code asn1_encode_unsigned_integer
	PROTOTYPE((asn1buf *buf, const unsigned long val, int *retlen));
/* requires  *buf is allocated
   modifies  *buf, *retlen
   effects   Inserts the encoding of val into *buf and returns 
              the length of the encoding in *retlen.
             Returns ENOMEM to signal an unsuccesful attempt
              to expand the buffer. */

asn1_error_code asn1_encode_octetstring
	PROTOTYPE((asn1buf *buf,
		   const int len, const asn1_octet *val,
		   int *retlen));
/* requires  *buf is allocated
   modifies  *buf, *retlen
   effects   Inserts the encoding of val into *buf and returns 
              the length of the encoding in *retlen.
             Returns ENOMEM to signal an unsuccesful attempt
              to expand the buffer. */

asn1_error_code asn1_encode_charstring
	PROTOTYPE((asn1buf *buf,
		   const int len, const char *val,
		   int *retlen));
/* requires  *buf is allocated
   modifies  *buf, *retlen
   effects   Inserts the encoding of val into *buf and returns 
              the length of the encoding in *retlen.
             Returns ENOMEM to signal an unsuccesful attempt
              to expand the buffer. */

asn1_error_code asn1_encode_null
	PROTOTYPE((asn1buf *buf, int *retlen));
/* requires  *buf is allocated
   modifies  *buf, *retlen
   effects   Inserts the encoding of NULL into *buf and returns 
              the length of the encoding in *retlen.
             Returns ENOMEM to signal an unsuccesful attempt
              to expand the buffer. */

asn1_error_code asn1_encode_printablestring
	PROTOTYPE((asn1buf *buf,
		   const int len, const char *val,
		   int *retlen));
/* requires  *buf is allocated
   modifies  *buf, *retlen
   effects   Inserts the encoding of val into *buf and returns 
              the length of the encoding in *retlen.
             Returns ENOMEM to signal an unsuccesful attempt
              to expand the buffer. */

asn1_error_code asn1_encode_ia5string
	PROTOTYPE((asn1buf *buf,
		   const int len, const char *val,
		   int *retlen));
/* requires  *buf is allocated
   modifies  *buf, *retlen
   effects   Inserts the encoding of val into *buf and returns 
              the length of the encoding in *retlen.
             Returns ENOMEM to signal an unsuccesful attempt
              to expand the buffer. */

asn1_error_code asn1_encode_generaltime
	PROTOTYPE((asn1buf *buf, const time_t val, int *retlen));
/* requires  *buf is allocated
   modifies  *buf, *retlen
   effects   Inserts the encoding of val into *buf and returns
              the length of the encoding in *retlen.
             Returns ENOMEM to signal an unsuccesful attempt
              to expand the buffer.
   Note: The encoding of GeneralizedTime is YYYYMMDDhhmmZ */

asn1_error_code asn1_encode_generalstring
	PROTOTYPE((asn1buf *buf,
		   const int len, const char *val,
		   int *retlen));
/* requires  *buf is allocated,  val has a length of len characters
   modifies  *buf, *retlen
   effects   Inserts the encoding of val into *buf and returns 
              the length of the encoding in *retlen.
             Returns ENOMEM to signal an unsuccesful attempt
              to expand the buffer. */

#endif
