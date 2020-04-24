/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_thread_struct.h,v $
 * Revision 1.1.6.2  1996/02/18  23:04:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:19  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:14:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:33  root]
 * 
 * Revision 1.1.4.1  1994/02/22  19:16:29  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:32:57  marrek]
 * 
 * Revision 1.1.2.2  1993/08/12  12:41:30  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:31:18  marrek]
 * 
 * $EndLog$
 */
#ifndef _XT_THREAD_STRUCT_H
#define _XT_THREAD_STRUCT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_thread_struct_rcsid[] = "@(#)$RCSfile: xt_thread_struct.h,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 23:04:22 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/* declarations and (re)definitions for threads */

#include <xt.h>
#include <xt_len.h>

#define MAX_NAME_LEN	64	/* thread name max length */
#define MAX_THREADS		255	/* max number of threads */

#define USE_GUARDS 0
#if USE_GUARDS
	#define GUARD_SIZE		16
#endif

struct thread_data_struct {
#if USE_GUARDS
OM_sint				guard0[GUARD_SIZE];		/* for overwrite check */
#endif
OM_object    		bt_ava[10];		/* array of ava's */
#if USE_GUARDS
OM_sint				guard1[GUARD_SIZE];		/* for overwrite check */
#endif
OM_private_object	bt_context;
#if USE_GUARDS
OM_sint				guard2[GUARD_SIZE];		/* for overwrite check */
#endif
OM_sint					bt_last;
#if USE_GUARDS
OM_sint				guard3[GUARD_SIZE];		/* for overwrite check */
#endif
struct tree_parm_struct	bt_parms[10];
#if USE_GUARDS
OM_sint				guard4[GUARD_SIZE];		/* for overwrite check */
#endif
OM_object    		bt_rdn[10];		/* array of rdn's */
#if USE_GUARDS
OM_sint				guard5[GUARD_SIZE];		/* for overwrite check */
#endif
OM_object			bt_session;
#if USE_GUARDS
OM_sint				guard6[GUARD_SIZE];		/* for overwrite check */
#endif
OM_private_object	private_object_tbl[XT_LEN_PRIVATE_OBJECT_TBL];
#if USE_GUARDS
OM_sint				guard7[GUARD_SIZE];		/* for overwrite check */
#endif
OM_public_object	public_object_tbl[XT_LEN_PUBLIC_OBJECT_TBL];
#if USE_GUARDS
OM_sint				guard8[GUARD_SIZE];		/* for overwrite check */
#endif
OM_string			string_tbl[XT_LEN_PUBLIC_OBJECT_TBL];
#if USE_GUARDS
OM_sint				guard9[GUARD_SIZE];		/* for overwrite check */
#endif
OM_object    		rt_ava[10];		/* array of ava's */
#if USE_GUARDS
OM_sint				guard10[GUARD_SIZE];		/* for overwrite check */
#endif
OM_private_object	rt_context;
#if USE_GUARDS
OM_sint				guard11[GUARD_SIZE];		/* for overwrite check */
#endif
OM_sint					rt_last;
#if USE_GUARDS
OM_sint				guard12[GUARD_SIZE];		/* for overwrite check */
#endif
struct tree_parm_struct	rt_parms[10];
#if USE_GUARDS
OM_sint				guard13[GUARD_SIZE];		/* for overwrite check */
#endif
OM_object    		rt_rdn[10];		/* array of rdn's */
#if USE_GUARDS
OM_sint				guard14[GUARD_SIZE];		/* for overwrite check */
#endif
OM_object			rt_session;
#if USE_GUARDS
OM_sint				guard15[GUARD_SIZE];		/* for overwrite check */
#endif
OM_descriptor		*session_tbl[XT_LEN_SESSION_TBL];
#if USE_GUARDS
OM_sint				guard16[GUARD_SIZE];		/* for overwrite check */
#endif
OM_workspace  		*workspace_tbl[XT_LEN_WORKSPACE_TBL];
#if USE_GUARDS
OM_sint				guard17[GUARD_SIZE];		/* for overwrite check */
#endif
OM_sint			 		xt_dump_objects;
#if USE_GUARDS
OM_sint				guard18[GUARD_SIZE];		/* for overwrite check */
#endif
OM_sint			 		xt_dump_objects_private;
#if USE_GUARDS
OM_sint				guard19[GUARD_SIZE];		/* for overwrite check */
#endif
char				xt_entry   [XT_FMT_FUNCMAX+1];
#if USE_GUARDS
OM_sint				guard20[GUARD_SIZE];		/* for overwrite check */
#endif
char            	xt_testname[XT_LEN_TESTNAME];
#if USE_GUARDS
OM_sint				guard21[GUARD_SIZE];		/* for overwrite check */
#endif
FILE				*xt_testcase_file; 		/* Testcase file pointer */
#if USE_GUARDS
OM_sint				guard22[GUARD_SIZE];		/* for overwrite check */
#endif
char				itoa_buf[81];
#if USE_GUARDS
OM_sint				guard23[GUARD_SIZE];		/* for overwrite check */
#endif
char				ptoa_buf[7];
#if USE_GUARDS
OM_sint				guard24[GUARD_SIZE];		/* for overwrite check */
#endif
}; /* end thread_data_struct */

#endif /* _XT_THREAD_STRUCT_H */
