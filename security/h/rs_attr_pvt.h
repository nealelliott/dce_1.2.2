/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_attr_pvt.h,v $
 * Revision 1.1.11.1  1996/08/09  12:04:38  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/06/19  20:33 UTC  aha  /main/aha_pk6/3]
 *
 * 	Changes for Public Key Login; fix rs_attr_update_verify_under_RIW prototype
 * 	[1996/06/18  20:56 UTC  aha  /main/aha_pk6/2]
 *
 * 	Changes for Public Key Login; change rs_attr_update_verify_under_RIW "sch_list"
 * 	parameter from pointer to pointer to pointer.
 * 	[1996/06/07  19:06 UTC  aha  /main/aha_pk6/1]
 *
 * 	Changes for Public Key Login; support Registry storage of KDC private key;
 * 	define function allowing rs_acct_replace() to utilize same logic as
 * 	done under db lock in rs_attr_update().
 * 	[1994/10/17  18:18:09  cuti  1.1.5.1]
 *
 * Revision 1.1.9.3  1996/02/18  22:58:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:21  marty]
 * 
 * Revision 1.1.9.2  1995/12/08  17:27:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:25  root]
 * 
 * Revision 1.1.5.1  1994/10/17  18:18:09  cuti
 * 	delete unused macros, add schema entry field length constants
 * 	[1994/10/14  16:15:03  cuti]
 * 
 * Revision 1.1.2.3  1994/07/15  14:59:09  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:15:57  mdf]
 * 
 * Revision 1.1.2.2  1994/06/17  18:42:01  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:38  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/02  17:54:52  hanfei
 * 	Initial ERA drop
 * 	[1994/02/18  19:08:49  hanfei]
 * 
 * $EndLog$
 */

#ifndef rs_attr_pvt_h__included
#define rs_attr_pvt_h__included

#include <rsdb_util.h>
#include <rsdb_attr_pvt.h>

/* rs_attr_operation_t: used to determine which permset
 * is required to check authorization for a given operation
 */
typedef enum rs_attr_operation_t {
    rs_attr_op_query,
    rs_attr_op_update,
    rs_attr_op_test,
    rs_attr_op_delete,
    rs_attr_op_last
} rs_attr_operation_t;

/* Schema entry field length constants
 */
#define RS_SCH_ATTR_NAME_MAX_LEN	1024
#define RS_SCH_STRING_MAX_LEN		1024
#define RS_SCH_SCOPE_MAX_LEN		RS_SCH_ATTR_NAME_MAX_LEN
#define RS_SCH_COMMENT_MAX_LEN		RS_SCH_STRING_MAX_LEN

/*
 * r s _ a t t r _ u p d a t e _ v e r i f y _ u n d e r _ R I W
 *
 * For use only by rs_attr_update() and rs_acct_replace()
 * Must be called under db RIW lock
 */
PUBLIC void rs_attr_update_verify_under_RIW(
    handle_t			h,
    sec_attr_component_name_t	component_name,	/* [in] */
    unsigned32			num_to_write,	/* [in] */
    sec_attr_t			in_attrs[],	/* [in] */
    rsdb_util_rgy_obj_info_t	*rgy_obj_info_p,/* [in] */
    signed32			*failure_index,	/* [out] */
    rsdb_attr_sch_list_t        **sch_list_p_p, /* [out] */
    signed32                    **xref_p_p,     /* [out] */
    error_status_t		*st_p
);

#endif /* rs_attr_pvt_h__included */
