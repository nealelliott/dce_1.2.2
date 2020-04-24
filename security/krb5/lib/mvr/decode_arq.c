/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: decode_arq.c,v $
 * Revision 1.1.4.2  1996/02/18  00:14:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:02:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:22  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:59:55  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:16:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:49:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  decode_arq.c V=1 02/06/91 //littl/prgy/krb5/lib/mvr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/mvr/decode_arq.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * krb5_decode_as_req() function.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_decode_arq_c[] =
"$Id: decode_arq.c,v 1.1.4.2 1996/02/18 00:14:58 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/copyright.h>

#include <krb5/krb5.h>
#include <krb5/asn1.h>

/*
 Takes formatted AS_REQ packet, and decodes it, 
 filling in request with the results.

 contents of packet are all set to allocated storage which should be
 freed by the caller when finished with the request.

 returns system errors
*/

krb5_error_code
decode_krb5_as_req(packet, request)
krb5_data *packet;
krb5_kdc_req **request;
{
    krb5_error_code retval;
    krb5_octet save;
    
    /* Frob the ASN.1 application identifier on the front
       so we can use the same encoding/decoding routines for
       AS & TGS REQ/REP */
    /* XXX keep in sync with ASN.1 application identifiers */

    save = packet->data[0];
    packet->data[0] = (packet->data[0] & 0xe0)|0x0c;

    retval = decode_krb5_tgs_req(packet, request);
    packet->data[0] = save;
    return retval;
}
