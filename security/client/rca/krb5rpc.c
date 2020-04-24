/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krb5rpc.c,v $
 * Revision 1.1.8.2  1996/03/11  13:28:04  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:51  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:06:42  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/10/19  17:03 UTC  burati
 * 	Merge fix for CHFts16610 from mb_mothra7
 * 	[1995/12/08  16:32:11  root]
 * 
 * Revision 1.1.4.2  1992/12/29  13:02:52  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:40:33  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/06  22:46:00  sommerfeld
 * 	[CR3275] Close site after call to avoid memory leak.
 * 	[1992/05/06  20:14:36  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:47:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  krb5rpc.c V=3 10/10/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
**  Kerberos V5 RPC interface.
*/

#if !defined(LINT) && !defined(apollo)
static char *VersionID = "@(#)krb5rpc.c	3 - 10/10/91";
#endif

#include <sec_krb5rpc.h>
#include <krb5rpc.h>
#include <rgymacro.h>
#include <rca_pub.h>
#include <rca_pvt.h>
#include <sec_krb_util.h>

/* Global bind_to_master flag set in sec_login_pvt.c */
extern unsigned32 btm_flag;

/* s e c _ k r b 5 r p c _ s e n d t o _ k d c
 *
 * Communicate with KDC via rpc.  The input and output arguments are the
 * byte stream messages defined by the Kerberos V5 protocol specification.
 * This function replaces the direct use of UDP by the MIT code.
 */
void sec_krb5rpc_sendto_kdc
  (
    krb5_data       *realm,
    unsigned32      len,
    unsigned char   *msg,
    unsigned32      out_len,
    unsigned32      *resp_len,
    unsigned char   *out_buf,
    error_status_t  *st
  )
{
    char                        *target_cell;
    sec_rgy_bind_auth_info_t    bind_auth_info;
    sec_rgy_handle_t            tmp_context = NULL, context = NULL;
    error_status_t		lst;

    *resp_len = 0;
    target_cell = sec_krb_realm_to_cell_len((unsigned int) realm->length, 
                                                realm->data);
    if (target_cell == NULL) {
        SET_STATUS(st, sec_rgy_cant_allocate_memory);
        return;
    }

    /* bind to a KDC server in the appropriate cell - need to use an
     * appropriate internal binding function and bind by KDC
     * interface uuid.  Don't need to use authenticated rpc because the
     * KDC itself implements the KRB5_AP_REQ/REP client server
     * authentication protocol.
     */
    bind_auth_info.info_type = sec_rgy_bind_auth_none;
    rca_site_bind((unsigned char *) target_cell, 
                    &bind_auth_info,
                    rca_NSI_cell,
                    krb5rpc_v1_0_c_ifspec,
                    NULL,
                    target_cell,
                    &tmp_context,
                    st);
    if (STATUS_OK(st)) {

        /* Get a binding to the master if necessary */
        if (btm_flag > 0) {
            rca_site_resolve_update(&bind_auth_info, tmp_context, &context,st);
	} else {
	    context = tmp_context;
	}

        if (STATUS_OK(st)) {

            SETUP_RETRY(rca_op_read, st) {
                (*krb5rpc_v1_0_c_epv.rsec_krb5rpc_sendto_kdc)
                  (HANDLE(context), len, msg, out_len, resp_len, out_buf, st);

                NORMAL_CASE;
                RETRY_CASE;
            }
            RETRY_END;
	    if (context != tmp_context) {
		sec_rgy_site_close (context, &lst);
	    }
        }
	sec_rgy_site_close(tmp_context, &lst);
    }
    free(target_cell);
}
