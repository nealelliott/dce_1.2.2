/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_pvt.h,v $
 * Revision 1.1.62.2  1996/03/11  13:25:55  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:01  marty]
 *
 * Revision 1.1.62.1  1995/12/08  17:54:12  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  20:31 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/13  20:25 UTC  tatsu_s
 * 	Added sec_acl_bind_local().
 * 	[1995/12/08  16:27:59  root]
 * 
 * Revision 1.1.60.1  1994/01/28  23:09:57  burati
 * 	Delegation/EPAC changes - add if_version (dlg_bl1)
 * 	[1994/01/18  22:11:44  burati]
 * 
 * Revision 1.1.58.1  1993/10/05  22:30:10  mccann
 * 	64bit porting changes
 * 	[1993/10/04  18:39:36  mccann]
 * 
 * Revision 1.1.4.4  1993/03/12  16:45:45  burati
 * 	CR7458 Add string binding to ACL handle for rebinding use
 * 	[1993/03/12  15:52:42  burati]
 * 
 * Revision 1.1.4.3  1992/12/29  12:36:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:32:17  zeliff]
 * 
 * Revision 1.1.4.2  1992/11/16  18:28:36  burati
 * 	CR4812 add auth_info to acl handle
 * 	[1992/11/14  00:42:38  burati]
 * 
 * Revision 1.1.2.2  1992/04/28  16:01:54  burati
 * 	CR755 New fields in acl context for rebinding/reimporting
 * 	[1992/04/28  02:04:09  burati]
 * 
 * Revision 1.1  1992/01/19  14:47:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  dacl_pvt.h
**
** Copyright (c) Hewlett-Packard Company 1991, 1993
** Unpublished work. All Rights Reserved.
**
*/
/*
**  Internal SEC_ACL client agent macros, types and globals
** 
*/

#ifndef _dacl_pvt_h__included_
#define _dacl_pvt_h__included_

#ifdef _dacl_ALLOCATE_GLOBALS_
#   define EXTERN
#   define INITVAL(X) X
#else
#   define EXTERN extern
#   define INITVAL(X)
#endif

#include <dce/rpc.h>
#include <dce/aclbase.h>
#include <dce/daclif.h>

#ifndef NULL
#   define NULL 0L
#endif

#include <macros.h>

/*
 * DEBUG tracing copied verbatim from RCA code for this release, to
 * make some serviceability possible until the real 1.1 cleanup happens
 */
#define SEC_ACL_DEBUG 1
#if SEC_ACL_DEBUG
#define SEC_ACL_DBG(n) (acl_binding_trace >= (n))
#define SEC_DBG_GPRINTF(x) do {if (SEC_ACL_DBG(1)) printf x; } while (0)
#define SEC_DBG_PRINTF(n,x) do {if (SEC_ACL_DBG(n)) printf x; } while (0)
#define SEC_HANDLE_PRINTF(handle, txt) \
if (SEC_ACL_DBG(3)) { \
    unsigned_char_p_t tnameP; \
    error_status_t    xst; \
    rpc_ns_binding_inq_entry_name(handle,rpc_c_ns_syntax_dce,&tnameP,&xst);\
    if (GOOD_STATUS(&xst)) { \
        printf("%s imported binding from %s\n", txt, tnameP); \
        free((char *)tnameP); \
    } \
}
#define SEC_OBJ_PRINTF(obj_uuid, txt) \
if (SEC_ACL_DBG(4)) { \
    unsigned_char_p_t touidP; \
    error_status_t    xst; \
    uuid_to_string(&obj_uuid, &touidP, &xst); \
    if (GOOD_STATUS(&xst)) { \
        printf("%s object uuid is %s\n", txt, touidP); \
        free((char *)touidP); \
    } \
}

#else
#define SEC_DBG_PRINTF(n,x)
#define SEC_DBG_GPRINTF(x)
#endif


/*
 * Client agent data types
 */

 /*
  * s e c _ a c l _ p v t _ h a n d l e _ b l k _ t 
  *
  * The internal data structure for representing binding contexts 
  */

typedef struct sec_acl_pvt_handle_blk {
    rpc_binding_handle_t        rpc_handle;
    rpc_ns_import_handle_t      import_handle;
    sec_acl_bind_auth_info_t    auth_info;
    boolean32                   is_dfs_acl;
    boolean32                   rpc_handle_valid;
    boolean32                   import_handle_valid;
    boolean32                   rebound;
    uuid_t                      obj_uuid;
    unsigned_char_p_t           resolved_name;
    sec_acl_component_name_t    component_name;
    unsigned_char_p_t           string_binding;
    error_status_t              status;
    unsigned32                  if_version;
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    union {
	void			*epv;
	rdaclif_v1_0_epv_t	*v1_0_epv;
	rdaclif_v0_0_epv_t	*v0_0_epv;
    }	lepv;
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
} sec_acl_pvt_handle_blk_t;

EXTERN  sec_acl_pvt_handle_blk_t
    *sec_acl_default_context_block_p INITVAL(= NULL);

#define HANDLE(CONTEXT) ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->rpc_handle

#define HANDLE_STR(CONTEXT) ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->string_binding

#define HANDLE_VALID(CONTEXT) \
 ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->rpc_handle_valid

#define IMPORT_HANDLE(CONTEXT) \
 ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->import_handle

#define AUTH_INFO(CONTEXT) \
 ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->auth_info

#define IMPORT_HANDLE_VALID(CONTEXT) \
 ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->import_handle_valid

#define COMPONENT_NAME(CONTEXT) \
 ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->component_name

#define RESOLVED_NAME(CONTEXT) \
 ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->resolved_name

#define LAST_ERROR(CONTEXT) ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->status

#define IS_DFS_ACL(CONTEXT) \
 ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->is_dfs_acl

#define REBOUND_YET(CONTEXT) \
 ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->rebound

#define OBJ_UUID(CONTEXT) \
 ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->obj_uuid

#define IF_VERSION(CONTEXT) \
 ((sec_acl_pvt_handle_blk_t *) (CONTEXT))->if_version

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
#define IS_LOCAL_ACL(CONTEXT) \
 (((sec_acl_pvt_handle_blk_t *) (CONTEXT))->lepv.epv != NULL)

#define V1_0_EPV(CONTEXT) \
 (*((sec_acl_pvt_handle_blk_t *) (CONTEXT))->lepv.v1_0_epv)

#define V0_0_EPV(CONTEXT) \
 (*((sec_acl_pvt_handle_blk_t *) (CONTEXT))->lepv.v0_0_epv)
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */

/*
 * dacl_rebind_update - Find, then rebind to, an update site.
 */
void sec__acl_rebind_update(
#ifdef __STDC__
    sec_acl_handle_t    context,
    uuid_t              *mgr_type,
    sec_acl_type_t      acl_type,
    error_status_t      *stp
#endif
);

/*
 * sec__acl_check_binding - validates binding and rebinds if necessary/possible
 */
boolean32 sec__acl_check_binding(
#ifdef __STDC__
    sec_acl_handle_t    context,
    error_status_t      *st_p
#endif
);

#endif /* _dacl_pvt_h__included_ */

