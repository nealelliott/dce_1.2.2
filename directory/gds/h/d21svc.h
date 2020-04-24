/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21svc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:37  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:59:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:13:32  root]
 * 
 * Revision 1.1.2.4  1994/07/06  15:07:28  marrek
 * 	July 1994 code drop.
 * 	[1994/07/06  10:03:05  marrek]
 * 
 * Revision 1.1.2.3  1994/06/21  14:47:07  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:40:48  marrek]
 * 
 * Revision 1.1.2.2  1994/06/09  18:46:12  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:16  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:14:50  marrek
 * 	Bug fixes.
 * 	[1994/05/02  13:05:55  marrek]
 * 
 * 	Changes for svc bug.
 * 	[1994/04/27  11:14:15  marrek]
 * 
 * 	Created for April Submission.
 * 	[1994/04/26  12:49:55  marrek]
 * 
 * $EndLog$
 */
#ifndef _D21SVC_H
#define _D21SVC_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d21svc_rcsid[] = "@(#)$RCSfile: d21svc.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:34 $";
#endif

/* macros for logging */

#define DUA_TRACE_IAPL_ENTRY \
			DCE_SVC_LOG((GDS_S_DUA_IAPL_ENTRY_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_OPMODE_CACHE_ONLY \
			DCE_SVC_LOG((GDS_S_DUA_OPMODE_CACHE_ONLY_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_OPMODE_DSA_ONLY \
			DCE_SVC_LOG((GDS_S_DUA_OPMODE_DSA_ONLY_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_OPMODE_CACHE_FIRST \
			DCE_SVC_LOG((GDS_S_DUA_OPMODE_CACHE_FIRST_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_OPMODE_DSA_FIRST \
			DCE_SVC_LOG((GDS_S_DUA_OPMODE_DSA_FIRST_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_OPMODE_CACHE_MODIFY \
			DCE_SVC_LOG((GDS_S_DUA_OPMODE_CACHE_MODIFY_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_OPMODE_ALIAS_MODIFY \
			DCE_SVC_LOG((GDS_S_DUA_OPMODE_ALIAS_MODIFY_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_OPMODE_SHADOW_MODIFY \
			DCE_SVC_LOG((GDS_S_DUA_OPMODE_SHADOW_MODIFY_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_OPMODE_CHAINED_MODIFY \
			DCE_SVC_LOG((GDS_S_DUA_OPMODE_CHAINED_MODIFY_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_OPMODE_STD_MODIFY \
			DCE_SVC_LOG((GDS_S_DUA_OPMODE_STD_MODIFY_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_OPMODE_ONE_MODIFY \
			DCE_SVC_LOG((GDS_S_DUA_OPMODE_ONE_MODIFY_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_OPMODE_TWO_MODIFY \
			DCE_SVC_LOG((GDS_S_DUA_OPMODE_TWO_MODIFY_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_UNKNOWN_OPMODE \
			DCE_SVC_LOG((GDS_S_DUA_UNKNOWN_OPMODE_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_DUMMY_BIND_TO_CDS \
			DCE_SVC_LOG((GDS_S_DUA_DUMMY_BIND_TO_CDS_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_BIND_TO_DEFAULT_DSA \
			DCE_SVC_LOG((GDS_S_DUA_BIND_TO_DEFAULT_DSA_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_BIND_TO_NAMED_DSA_1 \
			DCE_SVC_LOG((GDS_S_DUA_BIND_TO_NAMED_DSA_1_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_BIND_TO_ADDRESSED_DSA \
			DCE_SVC_LOG((GDS_S_DUA_BIND_TO_ADDRESSED_DSA_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_BIND_TO_NAMED_DSA_2 \
			DCE_SVC_LOG((GDS_S_DUA_BIND_TO_NAMED_DSA_2_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_BIND_TO_LOCAL_DSA(x) \
			DCE_SVC_LOG((GDS_S_DUA_BIND_TO_LOCAL_DSA_MSG, \
				d27_thread_id(), x, function_name));

#define DUA_TRACE_BIND_TO_REMOTE_DSA \
			DCE_SVC_LOG((GDS_S_DUA_BIND_TO_REMOTE_DSA_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_BIND_TAB_INITIALIZED \
			DCE_SVC_LOG((GDS_S_DUA_BIND_TAB_INITIALIZED_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_IPC_TAB_INITIALIZED \
			DCE_SVC_LOG((GDS_S_DUA_IPC_TAB_INITIALIZED_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_USING_EXISTING_BIND_ID(x) \
			DCE_SVC_LOG((GDS_S_DUA_USING_EXISTING_BIND_ID_MSG, \
				d27_thread_id(), function_name, x));

#define DUA_TRACE_USING_NEW_BIND_ID(x) \
			DCE_SVC_LOG((GDS_S_DUA_USING_NEW_BIND_ID_MSG, \
				d27_thread_id(), function_name, x));

#define DUA_TRACE_TRY_BIND_AS_ANONYMOUS \
			DCE_SVC_LOG((GDS_S_DUA_TRY_BIND_AS_ANONYMOUS_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_BIND_TABLE_OVERFLOW \
			DCE_SVC_LOG((GDS_S_DUA_BIND_TABLE_OVERFLOW_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_IPC_TABLE_OVERFLOW \
			DCE_SVC_LOG((GDS_S_DUA_IPC_TABLE_OVERFLOW_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_REFERRALS_FOUND \
			DCE_SVC_LOG((GDS_S_DUA_REFERRALS_FOUND_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_LOOP_DETECTED \
			DCE_SVC_LOG((GDS_S_DUA_LOOP_DETECTED_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_POQ_EMPTY \
			DCE_SVC_LOG((GDS_S_DUA_POQ_EMPTY_MSG, \
				d27_thread_id(),function_name));

#define DUA_TRACE_WRONG_IAPL_INPUT(x) \
			DCE_SVC_LOG((GDS_S_DUA_WRONG_INPUT_MSG, \
				d27_thread_id(), function_name, x));

#define DUA_TRACE_ATTRIBUTE_PROBLEM_AT_IAPL(x) \
			DCE_SVC_LOG((GDS_S_DUA_ATTRIBUTE_PROBLEM_AT_IAPL_MSG, \
				d27_thread_id(), x));

#define DUA_TRACE_CDS_FUNCTION_FAILED(x) \
			DCE_SVC_LOG((GDS_S_DUA_CDS_FUNCTION_FAILED_MSG, \
				d27_thread_id(), x, function_name));

#define DUA_TRACE_PROBLEM_IN_CDS(x) \
			DCE_SVC_LOG((GDS_S_DUA_PROBLEM_IN_CDS_MSG, \
				d27_thread_id(), x));

#define DUA_TRACE_ATTRIBUTE_PROBLEM_IN_APDU(x) \
			DCE_SVC_LOG((GDS_S_DUA_ATTRIBUTE_PROBLEM_IN_APDU_MSG, \
				d27_thread_id(), x));

#define DUA_TRACE_IPC_BIND_FAILED \
			DCE_SVC_LOG((GDS_S_DUA_IPC_BIND_FAILED_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_ERROR_PDU_RECEIVED \
			DCE_SVC_LOG((GDS_S_DUA_ERROR_PDU_RECEIVED_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_INTERRUPTED \
			DCE_SVC_LOG((GDS_S_DUA_INTERRUPTED_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_WRONG_CREDENTIAL_KIND(x) \
			DCE_SVC_LOG((GDS_S_DUA_WRONG_CREDENTIAL_KIND_MSG, \
				d27_thread_id(), x, function_name));

#define DUA_TRACE_NO_BIND(x) \
			DCE_SVC_LOG((GDS_S_DUA_NO_BIND_MSG, \
				d27_thread_id(), function_name, x));

#define DUA_TRACE_CREATE_EVENT_DISPATCHER_FAILED(x) \
			DCE_SVC_LOG((GDS_S_DUA_CREATE_EVENT_DISP_FAILED_MSG, \
				d27_thread_id(), function_name, x));

#define DUA_TRACE_CREATED_EVENT_DISPATCHER \
			DCE_SVC_LOG((GDS_S_DUA_CREATED_EVENT_DISPATCHER_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_ENTRY_EVENT_DISPATCHER \
			DCE_SVC_LOG((GDS_S_DUA_ENTRY_EVENT_DISPATCHER_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_SELECT_LEFT \
			DCE_SVC_LOG((GDS_S_DUA_SELECT_LEFT_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_SELECT_CALL_FAILED(x) \
			DCE_SVC_LOG((GDS_S_DUA_SELECT_CALL_FAILED_MSG, \
				d27_thread_id(), function_name, x));

#define DUA_TRACE_CANCEL_EXCEPTION_DETECTED(x) \
			DCE_SVC_LOG((GDS_S_DUA_CANCEL_EXCEPTION_DETECTED_MSG, \
				d27_thread_id(), function_name, x));

#define DUA_TRACE_PROTECT_INIT_FAILED(x) \
			DCE_SVC_LOG((GDS_S_DUA_PROTECT_INIT_FAILED_MSG, \
				d27_thread_id(), function_name, x));

#define DUA_TRACE_D27_301_FAILED \
			DCE_SVC_LOG((GDS_S_DUA_D27_301_FAILED_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_D27_302_FAILED \
			DCE_SVC_LOG((GDS_S_DUA_D27_302_FAILED_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_X500_TO_CDS_NAME_FAILED \
			DCE_SVC_LOG((GDS_S_DUA_X500_TO_CDS_NAME_FAILED_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_CDS_TO_X500_NAME_FAILED \
			DCE_SVC_LOG((GDS_S_DUA_CDS_TO_X500_NAME_FAILED_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_IAPL_EXIT(x) \
			DCE_SVC_LOG((GDS_S_DUA_IAPL_EXIT_MSG, \
				d27_thread_id(), function_name, x));

#define DUA_TRACE_CDS_NAME \
			DCE_SVC_LOG((GDS_S_DUA_CDS_NAME_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_X500_NAME \
			DCE_SVC_LOG((GDS_S_DUA_X500_NAME_MSG, \
				d27_thread_id(), function_name));

#define DUA_TRACE_D_N(x) \
			DCE_SVC_LOG((GDS_S_DUA_D_N_MSG, \
				d27_thread_id(), x));

#define DUA_TRACE_C_ARG(x) \
			DCE_SVC_LOG((GDS_S_DUA_SERVICE_CONTROLS_MSG, \
				x->d2_serv_cntrls)); \
			DCE_SVC_LOG((GDS_S_DUA_TIME_LIMIT_MSG, \
				x->d2_time_limit)); \
			DCE_SVC_LOG((GDS_S_DUA_SIZE_LIMIT_MSG, \
				x->d2_size_limit));

#define DUA_TRACE_SEND_ABANDON(x) \
			DCE_SVC_LOG((GDS_S_DUA_SEND_ABANDON_MSG, \
				d27_thread_id(), x));

#define DUA_TRACE_SUCC_ABANDONED(x) \
			DCE_SVC_LOG((GDS_S_DUA_SUCC_ABANDONED_MSG, \
				d27_thread_id(), x));

#define DUA_TRACE_NOT_ABANDONED(x) \
			DCE_SVC_LOG((GDS_S_DUA_NOT_ABANDONED_MSG, \
				d27_thread_id(), x));

#define DUA_TRACE_PTHREAD_COND_SIGNAL_FAILED(x) \
			DCE_SVC_LOG((GDS_S_PTHREAD_COND_SIGNAL_FAILED_MSG, \
				x,function_name));

#define DUA_TRACE_MUTEX_LOCK_FAILED(x) \
			DCE_SVC_LOG((GDS_S_MUTEX_LOCK_FAILED_MSG, \
				x,function_name));

#define DUA_TRACE_PTHREAD_CREATE_FAILED(x) \
			DCE_SVC_LOG((GDS_S_PTHREAD_CREATE_FAILED_MSG, \
				x));

#define DUA_TRACE_SIGPROCMASK_FAILED \
			DCE_SVC_LOG((GDS_S_DUA_SIGPROCMASK_FAILED_MSG, \
				d27_thread_id,function_name,errno));

#endif /* _D21SVC_H */
