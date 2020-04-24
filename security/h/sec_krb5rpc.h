/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_krb5rpc.h,v $
 * Revision 1.1.39.2  1996/02/18  23:01:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:26  marty]
 *
 * Revision 1.1.39.1  1995/12/08  17:29:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:17  root]
 * 
 * Revision 1.1.37.1  1993/10/05  22:30:39  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:11:23  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  13:27:31  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:16:05  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sec_krb5rpc.h V=1 10/10/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
**  Kerberos V5 RPC interface.
*/

#ifndef _sec_krb5_rpc_h_included
#define _sec_krb5_rpc_h_included

#include <krb5/base-defs.h>
#include <dce/nbase.h>

void sec_krb5rpc_sendto_kdc (
#ifdef __STDC__
    krb5_data       *realm,
    unsigned32      len,
    unsigned char   *msg,
    unsigned32      out_len,
    unsigned32      *resp_len,
    unsigned char   *out_buf,
    error_status_t  *st
#endif
);

#endif
