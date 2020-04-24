/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: encode_arq.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:02:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:24  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:00:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:16:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:49:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  encode_arq.c V=1 02/06/91 //littl/prgy/krb5/lib/mvr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/mvr/encode_arq.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * krb5_encode_as_req() function.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_encode_arq_c[] =
"$Id: encode_arq.c,v 1.1.4.2 1996/02/18 00:15:02 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/copyright.h>

#include <krb5/krb5.h>
#include <krb5/asn1.h>

/*
 Takes request
 and encodes for transmission, placing result in *packet.
 The string *packet will be allocated before formatting.

 returns errors from encryption routines, system errors
*/

krb5_error_code
encode_krb5_as_req(request, packet)
const krb5_kdc_req *request;
krb5_data **packet;
{
    krb5_error_code retval;

    if (retval = encode_krb5_tgs_req(request, packet))
	return(retval);
    /* Frob the ASN.1 application identifier on the front
       so we can use the same encoding/decoding routines for
       AS & TGS REQ/REP */
    /* XXX keep in sync with ASN.1 application identifiers */
    (*packet)->data[0] = ((*packet)->data[0] & 0xe0)|0x0a;
    return retval;
}
