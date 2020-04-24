/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bindcall.c,v $
 * Revision 1.1.6.2  1996/02/18  18:52:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:00  marty]
 *
 * Revision 1.1.6.1  1995/12/07  22:33:36  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:05 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:17:10  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:48:55  bfc
 * 	oct 95 idl drop
 * 	[1995/10/22  23:35:44  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/22  22:56:42  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE R1.1
 * 	[1995/10/21  18:25:12  bfc]
 * 
 * Revision 1.1.2.2  1994/06/10  20:53:32  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:43  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/02  22:43:36  tom
 * 	From DEC: Add binding callout function.
 * 	[1994/05/02  21:06:30  tom]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) Digital Equipment Corporation, 1994
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**  
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of 
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or 
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**  NAME:
**
**      bindcall.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Provides canned routines that may be used in conjunction with the
**      [binding_callout] ACF attribute.  These routines are called from a
**      client stub to possibly modify the binding handle, typically with
**      security information.
**
*/

#include <dce/rpc.h>
#include <dce/stubbase.h>

void rpc_ss_bind_authn_client
#ifdef IDL_PROTOTYPES
(
    rpc_binding_handle_t    *p_bh,      /* [io] Binding handle */
    rpc_if_handle_t         if_h,       /* [in] Interface handle */
    error_status_t          *p_st       /*[out] Return status */
)
#else
(p_bh, if_h, p_st)
    rpc_binding_handle_t    *p_bh;      /* [io] Binding handle */
    rpc_if_handle_t         if_h;       /* [in] Interface handle */
    error_status_t          *p_st;      /*[out] Return status */
#endif
{
    unsigned_char_t *princ_name;        /* Server principal name */

    /* Resolve binding handle if not fully bound */
    rpc_ep_resolve_binding(*p_bh, if_h, p_st);
    if (*p_st != rpc_s_ok)
        return;

    /* Get server principal name */
    rpc_mgmt_inq_server_princ_name(
        *p_bh,                              /* binding handle */
        (unsigned long)rpc_c_authn_default, /* default authentication service */
        &princ_name,                        /* server principal name */
        p_st);
    if (*p_st != rpc_s_ok)
        return;

    /* Set auth info in binding handle */
    rpc_binding_set_auth_info(
        *p_bh,                              /* binding handle */
        princ_name,                         /* server principal name */
        rpc_c_protect_level_default,        /* default protection level */
        (unsigned long)rpc_c_authn_default, /* default authentication service */
        NULL,                               /* def. auth cred (login ctx) */
        rpc_c_authz_name,                   /* authz based on cli princ name */
        p_st);
}
