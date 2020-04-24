/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_bind_util.h,v $
 * Revision 1.1.6.3  1996/02/18  23:00:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:19  marty]
 *
 * Revision 1.1.6.2  1995/12/08  17:29:00  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:14 UTC  greg
 * 	Merge client compatibility fixes from greg_sec_migrate2.
 * 	[1995/05/03  23:51 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Add  prototype for sec_bind_util_binding_resolve().
 * 	[1995/05/02  20:30 UTC  greg  /main/greg_sec_migrate2/1]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:48  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:16:05  annie]
 * 
 * Revision 1.1.2.1  1994/07/15  14:59:58  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:59:41  mdf]
 * 
 * $EndLog$
 */

#ifndef sec_bind_util_h__included
#define sec_bind_util_h__included

/*
 * s e c _ b i n d _ r e t r y _ s t a t u s
 *
 * Return true if it is appropriate to retry an operation that
 * received status "op_st".
 * Return false if it is not appropriate to retry the operation.
 */
boolean32 sec_bind_retry_status (
    error_status_t *op_stp
);

/* sec_bind_util_resolve_binding 
 *
 * If the specifed rpc binding handle is not
 * fully resolved (has no endpoint) resolve it
 * using the specified if_handle.  If the which_protseq
 * parameter is NULL, the binding handle is resolved
 * regardless of the protocol sequence.  If the
 * which_protseq parameter is non-null, the
 * binding handle is resolved only if it is a
 * binding handle over the specifed protocol sequence.
 * Returns false if the rpc_handle requires resolution
 * and could not be resolved, true otherwise.
 */
boolean32 sec_bind_util_resolve_binding ( 
    rpc_binding_handle_t  rpc_handle,    /* [in/out] */
    rpc_if_handle_t       if_handle,     /* [in]     */
    unsigned_char_p_t     which_protseq, /* [in]     */
    error_status_t        *status        /* [out]    */
);

#endif
