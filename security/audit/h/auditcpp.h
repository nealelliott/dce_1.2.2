/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: auditcpp.h,v $
 * Revision 1.1.6.1  1996/07/08  18:26:59  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:24 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  22:57:33  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  22:57:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:26  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:39:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:17:49  root]
 * 
 * Revision 1.1.2.5  1994/05/10  15:14:34  weisz
 * 	CR 10147: added constant RPC_RETRIES
 * 	[1994/05/10  15:11:16  weisz]
 * 
 * Revision 1.1.2.4  1994/03/11  22:48:31  luan
 * 	CR 9775 9776: Added inclusion of <dce/dceaudmac.h> and <dce/dceaudsvc.h>.
 * 	[1994/03/11  22:03:16  luan]
 * 
 * Revision 1.1.2.3  1994/03/03  18:58:20  luan
 * 	CR 9773: Make consistent prior-inclusion checks in header files.
 * 	[1994/03/03  18:57:02  luan]
 * 
 * 	CR 9773: Make consistent prior-inclusion checks in header files.
 * 
 * Revision 1.1.2.2  1994/02/18  19:46:59  luan
 * 	CR 9777: Changed to use <dce/audit.h> instead of <audit.h>.
 * 	[1994/02/11  03:02:36  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:04  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:17  zeliff]
 * 
 * $EndLog$
 */

/*
 * auditcpp.h: audit control program include file
 */

#ifndef _AUDITCPP_H
#define _AUDITCPP_H

#include <string.h>
#include <stdio.h>
#include <sys/time.h>

#include <dce/dce.h>
#include <dce/pgo.h>
#include <dce/sec_base.h>
#include <dce/dce_cf.h>
#include <dce/dce_msg.h>

#include <dce/audit.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudmac.h>
#include <dce/dceaudsvc.h>

#include <error_statusp.h>
#include <eslp.h>
#include <pathsp.h>

#define MAX_ARGUMENTS 128
#define MAX_STRING_LENGTH 1024
#define MAX_EVENT_CLASSES 1024
#define MAX_CONDITIONS 4
#define MAX_ACTIONS 4
#define RPC_RETRIES 5

/* Internal state */
extern boolean32            bound_to_auditd;
extern boolean32            batch;
extern boolean32            debug_mode;
extern int                  true_count, counter;
extern int                  evt_class_cnt;
extern utc_t                start_time, end_time, total_time;

extern rpc_binding_handle_t auditd_handle;

extern char    *Type[]; 

struct aud_cp_index_entry_s_t {
    char           *name;
    unsigned32      number;
};

extern struct aud_cp_index_entry_s_t aud_g_cp_condition[];

extern struct aud_cp_index_entry_s_t aud_g_cp_action[];

extern struct aud_cp_index_entry_s_t aud_g_cp_evt_class[];

extern boolean32 audfilter_show();
extern boolean32 audfilter_remove();
extern boolean32 audfilter_add();
extern boolean32 audfilter_delete();
extern boolean32 audfilter_list();
extern boolean32 aud_modify();
extern boolean32 audtrail_show();
extern boolean32 aud_stop();
extern boolean32 aud_cp_input_subject(aud_esl_type_t *, sec_rgy_name_t);
extern void      aud_cp_init_ec_list();

#endif /* _AUDITCPP_H */
