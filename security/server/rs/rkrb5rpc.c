/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rkrb5rpc.c,v $
 * Revision 1.1.6.1  1996/10/03  15:08:23  arvind
 * 	deal with change to `dispatch' signature.
 * 	[1996/09/16  22:57 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.4.2  1996/02/18  00:17:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:06  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:50:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:57  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:34:25  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:22:47  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:44:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rkrb5rpc.c V=3 12/10/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *      Registry Server - Kerberos V5 RPC interface
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rkrb5rpc.c	3 - 12/10/91";
#endif

#include <pthread.h>
#include <krb5rpc.h>
#include <dce/secsts.h>
#include <rgymacro.h>

#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <kdc_util.h>
#include <u_str.h>
#include <un_maloc.h>

PUBLIC void rsec_krb5rpc_sendto_kdc ( h, len, msg, out_len, resp_len, out, st )
    handle_t            h;
    unsigned32          len;
    unsigned char       *msg;
    unsigned32          out_len;
    unsigned32          *resp_len;
    unsigned char       *out;
    error_status_t      *st;
{
    krb5_fulladdr       from;
    krb5_address        addr;
    error_status_t      lst;
    krb5_data           pkt;
    krb5_data           *response;
    krb5_error_code     err;

    rpc_binding_to_string_binding(h, &addr.contents, &lst);
    if (STATUS_OK(&lst)) {
        addr.addrtype = ADDRTYPE_DCE;
        addr.length = u_strlen(addr.contents);
    } else {
        addr.addrtype = ADDRTYPE_UNSPECIFIED;
    }

    from.address = &addr;
    from.port = 0;

    pkt.length  = len;
    pkt.data    = (char *) msg;

    *resp_len = 0;

    /* port 135 is DCE portmapper.  close enough for what we want to do... */
    err = dispatch(&pkt, &from, 135, &response);

    if (err == 0) {
        if (response->length <= out_len) {
            *resp_len = response->length;
            memcpy((char *) out, response->data, response->length);
        } else {
            *st = sec_kdc_s_response_too_long;
        }
        krb5_free_data(response);
    }

    if (addr.addrtype == ADDRTYPE_DCE) {
        rpc_string_free(&addr.contents, &lst);
    }

    *st = err;
}    
