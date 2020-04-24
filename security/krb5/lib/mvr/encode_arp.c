/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: encode_arp.c,v $
 * Revision 1.1.6.1  1996/10/03  15:05:26  arvind
 * 	add `const' in the right place..
 * 	[1996/09/16  21:29 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.4.2  1996/02/18  00:15:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:26  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  18:02:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:23  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:00:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:16:39  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:49:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  encode_arp.c V=3 11/11/91 //littl/prgy/krb5/lib/mvr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/mvr/encode_arp.c,v $
 * $Author: arvind $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 *
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.
 *
 * krb5_encode_as_rep() function.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_encode_arp_c[] =
"$Id: encode_arp.c,v 1.1.6.1 1996/10/03 15:05:26 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/copyright.h>

#include <krb5/krb5.h>
#include <krb5/asn1.h>

/*
 Takes reply
 and encodes for transmission, placing result in *packet.
 The string *packet will be allocated before formatting.

 returns errors from encryption routines, system errors
*/

krb5_error_code
encode_krb5_as_rep(reply, packet)
const krb5_kdc_rep *reply;
krb5_data **packet;
{
    krb5_error_code retval;


    if (retval = encode_krb5_tgs_rep(reply, packet))
	return(retval);
    /* Frob the ASN.1 application identifier on the front
       so we can use the same encoding/decoding routines for
       AS & TGS REQ/REP */
    /* XXX keep in sync with ASN.1 application identifiers */
    (*packet)->data[0] = ((*packet)->data[0] & 0xe0)|0x0b;
    return retval;
}
