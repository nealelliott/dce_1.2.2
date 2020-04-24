/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: svc_utils.c,v $
 * Revision 1.1.6.1  1996/07/08  18:32:30  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:26 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:35  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  00:06:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:51  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:42:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:17  root]
 * 
 * Revision 1.1.2.2  1994/06/17  18:41:20  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:05  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/11  22:48:46  luan
 * 	CR 9775 9776: Initial drop for supporting svc messages in libaudit.
 * 	[1994/03/11  22:28:57  luan]
 * 
 * $EndLog$
 */

/*
 * Initialize svc for audit component.
 */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudmac.h>
#include <dce/dceaudsvc.h>

dce_svc_handle_t aud_svc_handle;
static boolean32 svc_initialized = FALSE;

void dce_aud_init_svc()
{
    unsigned32 st;

    if (!svc_initialized) {
	aud_svc_handle = dce_svc_register(aud_svc_table, (idl_char *) "aud",
					  &st);
	dce_msg_define_msg_table(aud__table, 
		sizeof(aud_svc_table)/ sizeof(aud__table[0]), &st);
	svc_initialized = TRUE;
    }
    return;
}
