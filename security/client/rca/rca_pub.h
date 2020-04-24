/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rca_pub.h,v $
 * Revision 1.1.12.1  1996/10/15  20:48:56  arvind
 * 	Change prototype for rca_check_binding_resolved() to have a signed retry count.
 * 	[1996/03/29  16:07 UTC  jrr  /main/HPDCE02/jrr_davis_1/1]
 *
 * Revision 1.1.10.3  1996/02/18  22:57:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:32  marty]
 * 
 * Revision 1.1.10.2  1995/12/08  18:07:19  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/15  20:59 UTC  greg
 * 	Merge client compatibility fixes from greg_sec_migrate2.
 * 	[1995/05/03  23:50 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	rename rca_check_binding()  rca_check_binding_resolved().
 * 	[1995/05/02  20:29 UTC  greg  /main/HPDCE02/greg_sec_migrate2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/15  00:00 UTC  rps
 * 	copyright
 * 	[1995/04/05  17:03 UTC  rps  /main/MOTHRA_panic/2]
 * 
 * 	Add auth_info parameter to rca_get_cell_name, since we want the
 * 	*principal's* default cell name, not the machine's default cell name.
 * 	[1995/03/02  19:56 UTC  sommerfeld  /main/sommerfeld_krb5_alias_1/1]
 * 
 * Revision 1.1.6.1  1993/10/05  22:29:56  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:04:12  mccann]
 * 
 * Revision 1.1.4.2  1992/12/29  13:03:22  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:40:57  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/18  21:19:33  sommerfeld
 * 	Flush rca_get_site_name().
 * 	[1992/06/12  21:49:46  sommerfeld]
 * 
 * 	Flush rca_bind_to_host().
 * 	[1992/06/10  22:45:32  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:47:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rca_pub.h V=13 12/13/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/* 
**  Exported RCA macros, types and globals
*/

#ifndef rca_pub_h__included
#define rca_pub_h__included

#include <dce/rpc.h>
#include <dce/rgybase.h>
#include <rsbase.h>
#include <dce/binding.h>

typedef int rca_operation_t;
#define rca_op_first 0
#define rca_op_read 0
#define rca_op_write 1
#define rca_op_last 2

typedef long rca_nsi_options_t;
#define rca_NSI_name    0x1L
#define rca_NSI_cell    0x2L

/*
** Internal Routines
*/

boolean32 rca_check_binding(
    sec_rgy_handle_t  *context,
    rca_operation_t    which,
    error_status_t    *(status)
);

boolean32 rca_check_binding_resolved(
    sec_rgy_handle_t    *context,
    rca_operation_t      which,
    signed32            *retries,
    error_status_t      *(status)
);

void rca_handle_cache ( 
    rs_cache_data_t    *cache_info
);

void    rca_get_cell_name (
    sec_rgy_bind_auth_info_t *auth_info,
    sec_rgy_pname_t     cell_name
);

void rca_site_bind (
    unsigned_char_p_t           site_name,  /* [in] */
    sec_rgy_bind_auth_info_t    *auth_info, /* [in]  */
    long                        flags,
    rpc_if_handle_t             if_spec,
    char                        *princ,
    char                        *cell,      /* May be NULL */
    sec_rgy_handle_t            *context,   /* [out] */
    error_status_t              *status
);

#ifdef cache_handling_defined
void rca_cache_set_refresh_handler ( 
    rgy_cache_refresh_handler_t    handler
); 
#endif

#endif /* rca_pub_h__included */
