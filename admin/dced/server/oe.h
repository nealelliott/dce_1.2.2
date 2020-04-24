/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: oe.h,v $
 * Revision 1.1.4.2  1996/02/18  23:32:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:53:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:13  root]
 * 
 * Revision 1.1.2.7  1994/09/28  20:04:59  rsalz
 * 	Don't ping outselves; redo way local flag is set (OT CR 12384).
 * 	[1994/09/28  19:40:46  rsalz]
 * 
 * Revision 1.1.2.6  1994/08/25  16:32:26  rsalz
 * 	Some cleanups; purge #if 0'd server context.
 * 	[1994/08/25  16:30:44  rsalz]
 * 
 * Revision 1.1.2.5  1994/08/12  17:36:57  bowe
 * 	Added old_ept_make_local() prototype. [CR 11567]
 * 	[1994/08/12  13:31:20  bowe]
 * 
 * Revision 1.1.2.4  1994/06/09  18:15:33  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:17  annie]
 * 
 * Revision 1.1.2.3  1994/05/31  20:24:04  rsalz
 * 	Add -e flag; fix some EP bugs
 * 	[1994/05/31  20:22:53  rsalz]
 * 
 * Revision 1.1.2.2  1994/05/27  16:31:19  rsalz
 * 	Fix bootstrap phase; cleanup EP and LLB compat code
 * 	[1994/05/27  16:30:33  rsalz]
 * 
 * Revision 1.1.2.1  1994/05/26  19:42:23  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:54  rsalz]
 * 
 * $EndLog$
 */

/*
**  Declarations of functions for old EPT support.
*/

extern void old_ept_insert(unsigned32, ept_entry_t*, boolean32, boolean32, error_status_t*);
extern void old_ept_delete(unsigned32, ept_entry_t*, error_status_t*);
extern void old_ept_lookup(unsigned32, uuid_t*, rpc_if_id_t*, unsigned32, ept_lookup_handle_t*, unsigned32, unsigned32*, ept_entry_t*, error_status_t*
);
extern void old_ept_map(uuid_t*, twr_t*, ept_lookup_handle_t*, unsigned32, unsigned32*, twr_t**, error_status_t*);
extern void old_ept_lookup_handle_free(ept_lookup_handle_t*, error_status_t*);
extern void old_ept_inq_object(uuid_t*, error_status_t*);
extern void old_ept_mgmt_delete(boolean32, uuid_t*, twr_t*, error_status_t*);
extern void old_ept_lookup_handle_t_rundown(void *context_handle);
extern void old_ept_init(uuid_t*, error_status_t*);
extern void old_ept_fwd_map(uuid_t*, rpc_if_id_t*, boolean32, rpc_syntax_id_t*, rpc_protocol_id_t, unsigned32, unsigned32, rpc_addr_p_t, uuid_t*, unsigned32*, rpc_addr_p_t*, error_status_t*);
