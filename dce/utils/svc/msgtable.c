/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * Data for in-core message tables.
 */

/*
 * HISTORY
 * $Log: msgtable.c,v $
 * Revision 1.1.6.2  1996/02/18  19:22:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:15  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:37:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:09:42  root]
 * 
 * Revision 1.1.4.9  1994/09/30  19:44:54  rsalz
 * 	Merged with changes from 1.1.4.8
 * 	[1994/09/30  19:44:45  rsalz]
 * 
 * 	Implement DCE RFC 24.2 (OT CR 11929).
 * 	[1994/09/27  04:45:46  rsalz]
 * 
 * Revision 1.1.4.8  1994/09/28  20:43:18  bowe
 * 	Add messags for cfg, dts, gss to table. [CR 12366]
 * 	[1994/09/28  20:42:35  bowe]
 * 
 * Revision 1.1.4.7  1994/09/26  14:12:05  rsalz
 * 	Tweak include order to get lib message table (OT CR 12327).
 * 	[1994/09/26  13:58:47  rsalz]
 * 
 * Revision 1.1.4.6  1994/09/08  19:42:06  jd
 * 	Added the cds table so things other than cds know  how to map
 * 	cds statuses.
 * 	[1994/09/08  17:50:47  jd]
 * 
 * Revision 1.1.4.5  1994/06/22  20:13:34  pwang
 * 	Added dced default message table [OT#10948].
 * 	[1994/06/22  20:09:37  pwang]
 * 
 * Revision 1.1.4.4  1994/06/21  17:28:48  tom
 * 	Add rpc default message table (OT 10310).
 * 	[1994/06/14  18:37:16  tom]
 * 
 * Revision 1.1.4.3  1994/06/09  16:06:05  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:40  devsrc]
 * 
 * Revision 1.1.4.2  1994/03/25  20:07:58  sekhar
 * 	[ OT 10164 ] Security SVC code drop ( sec_svc_bl1 )
 * 	             Compile in default message text table sec__table
 * 	[1994/03/25  19:48:02  sekhar]
 * 
 * Revision 1.1.4.1  1993/12/20  20:50:46  rsalz
 * 	Merge msg.sams into svc.sams
 * 	[1993/12/20  20:50:21  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  18:08:54  rsalz
 * 	Initial release
 * 	[1993/08/16  18:03:47  rsalz]
 * 
 * $EndLog$
 */

#include <msgimpl.h>


/*
**  Declarations of tables that are compiled in.
*/
#include <dce/dcesvcmsg.h>
#include <dce/dcesecmsg.h>
#include <dce/dcerpcmsg.h>
#include <dce/dcedhdmsg.h>
#include <dce/dcecdsmsg.h>
#include <dce/dcelibmsg.h>
#include <dce/dcecfgmsg.h>
#include <dce/dcedtsmsg.h>
#include <dce/dcegssmsg.h>


/*
**  The compiled table of tables.
*/
dce_msg_table_list_t	dce_msg_g_compiled_table[] = {
    { dce_svc_g_table, sizeof dce_svc_g_table / sizeof dce_svc_g_table[0],
    smallest_svc_message_id, biggest_svc_message_id },

    { sec__table, sizeof sec__table / sizeof sec__table[0],
    smallest_sec_message_id, biggest_sec_message_id },

    { rpc_g_svc_msg_table, sizeof rpc_g_svc_msg_table / sizeof rpc_g_svc_msg_table[0],
    smallest_rpc_message_id, biggest_rpc_message_id },

    { dced_svc_g_table, sizeof dced_svc_g_table / sizeof dced_svc_g_table[0],
    smallest_dhd_message_id, biggest_dhd_message_id },

    { cds_msg_table, sizeof cds_msg_table / sizeof cds_msg_table[0],
    smallest_cds_message_id, biggest_cds_message_id },

    { dce_lib_g_table, sizeof dce_lib_g_table / sizeof dce_lib_g_table[0],
    smallest_lib_message_id, biggest_lib_message_id },

    { dts__table, sizeof dts__table / sizeof dts__table[0],
    smallest_dts_message_id, biggest_dts_message_id },

    { dce_cfg_g_table, sizeof dce_cfg_g_table / sizeof dce_cfg_g_table[0],
    smallest_cfg_message_id, biggest_cfg_message_id },

    { gss__table, sizeof gss__table / sizeof gss__table[0],
    smallest_gss_message_id, biggest_gss_message_id }
};


/*
**  Internal pointers and counts; don't edit.
*/
int			dce_msg_g_table_size =
	sizeof dce_msg_g_compiled_table / sizeof dce_msg_g_compiled_table[0];
dce_msg_table_list_t	*dce_msg_g_curr_table = dce_msg_g_compiled_table;
