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
 * $Log: lb.h,v $
 * Revision 1.1.4.2  1996/02/18  23:32:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:53:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:10  root]
 * 
 * Revision 1.1.2.5  1994/07/15  17:22:20  rsalz
 * 	Update LLB support; remove sources from src/rpc (OT CR 7435).
 * 	[1994/07/15  17:20:24  rsalz]
 * 
 * Revision 1.1.2.4  1994/06/09  18:15:28  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:12  annie]
 * 
 * Revision 1.1.2.3  1994/05/31  20:24:00  rsalz
 * 	Add -e flag; fix some EP bugs
 * 	[1994/05/31  20:22:48  rsalz]
 * 
 * Revision 1.1.2.2  1994/05/27  16:31:14  rsalz
 * 	Fix bootstrap phase; cleanup EP and LLB compat code
 * 	[1994/05/27  16:30:23  rsalz]
 * 
 * Revision 1.1.2.1  1994/05/26  19:42:15  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:46  rsalz]
 * 
 * $EndLog$
 */
#include <llb.h>

/*
**  Public functions and datatypes in the LLB support module.
*/
void llb_init(error_status_t*);
void ncs_mgmt_if_register_epv(error_status_t*);

#if	defined(_COM_H)
void llb_fwd(uuid_t*, uuid_t*, rpc_addr_p_t, unsigned32*, rpc_addr_p_t*, error_status_t*);
#endif	/* defined(_COM_H) */
#if	defined(ept_v3_0_included)
void llb_map(uuid_t*, twr_t*, ept_lookup_handle_t*, unsigned32, unsigned32*, twr_t**, error_status_t*);
#endif	/* defined(ept_v3_0_included) */

extern llb__v4_0_epv_t llb_v4_0_mgr_epv;
