/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_certify.c,v $
 * Revision 1.1.10.2  1996/02/18  00:07:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:33  marty]
 *
 * Revision 1.1.10.1  1995/12/13  16:23:50  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/3  1995/06/16  20:06 UTC  mdf
 * 	Memory leak cleanup.
 * 	[1995/05/23  17:47 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_6/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/03  22:03 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/28  16:48 UTC  aha
 * 	CHFts14138: create namecache file with secure mode; CHFts14693: init RPC binding handles to NULL
 * 
 * 	HP revision /main/aha_mothra4/2  1995/03/27  20:09 UTC  aha
 * 	Initialize RPC handles to null in declaration
 * 
 * 	HP revision /main/aha_mothra4/1  1995/03/24  21:13 UTC  aha
 * 	CHFts14693: init RPC binding handles to NULL
 * 	[1995/12/11  15:13:41  root]
 * 
 * Revision 1.1.6.1  1994/05/11  19:04:12  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  20:47:23  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * Revision 1.1.4.4  1993/03/16  15:38:32  burati
 * 	CR7503 Try all string bindings from config file, til one works
 * 	[1993/03/16  02:46:31  burati]
 * 
 * Revision 1.1.4.3  1992/12/29  13:04:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:41:58  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/08  21:48:16  burati
 * 	CR6365 Wasn't closing sec_clientd binding file after using it
 * 	[1992/12/08  18:30:24  burati]
 * 
 * Revision 1.1.2.2  1992/03/18  18:57:39  burati
 * 	CR759: Use dce_cf_prin_name_from_host to get correct host principal name.
 * 	Include string.h for prototypes.
 * 	[1992/03/18  15:41:16  burati]
 * 
 * Revision 1.1  1992/01/19  14:47:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  sec_certify.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
**  sec_cert(ify) client agent
*/

#include <stdio.h>
#include <string.h>

#include <dce/rpc.h>
#include <dce/sec_base.h>
#include <dce/sec_login.h>
#include <dce/sec_cert.h>
#include <dce/dce_cf.h>
#include <rsec_cert.h>

#include <macros.h>
#include <scd_loc.h>


/*
 * sec_login_validate_cert_auth
 *      Validate Certification Authority, by making sure the client can
 *      make an authenticated call.  Presently, the fact that the caller
 *      was able to make the authenticated call is enough, but there is
 *      a status parameter in the signature so that future implementations
 *      may make additional checks if necessary.
 */
void sec_login_validate_cert_auth
#ifndef __STDC__
    (login_context, st)
    sec_login_handle_t          login_context;
    error_status_t              *st;
#else
  (
    sec_login_handle_t          login_context,
    error_status_t              *st
  )
#endif
{
    unsigned_char_p_t       hostname = NULL;
    rpc_binding_handle_t    cert_h = NULL;
    error_status_t          lst;

    CLEAR_STATUS(st);
    /* Bind to sec_clientd if necessary */
    sec_login_util_bind_to_sec_clientd(&cert_h, st);

    /* Setup the auth info on the handle */
    if (GOOD_STATUS(st)) {
        dce_cf_prin_name_from_host(NULL, (char **)&(hostname), st);
        if (GOOD_STATUS(st)) {
            rpc_binding_set_auth_info(cert_h, hostname,
                                rpc_c_authn_level_pkt_integrity,
                                rpc_c_authn_dce_private,
                                (rpc_auth_identity_handle_t)login_context,
                                rpc_c_authz_dce, st);
        }
    }

    /* Call sec_clientd to validate certification authority */
    if (GOOD_STATUS(st)) {
        (*rsec_cert_v1_0_c_epv.rsec_login_validate_cert_auth)(cert_h, st);
    } else {
        /* Unable to bind to sec_clientd on this host (bad config file?) */
        SET_STATUS(st, sec_login_s_config);
    }

    /* clean up resources associated with handle */
    if (cert_h)
        rpc_binding_free(&cert_h, &lst);
    if (hostname)
	free(hostname);
}
