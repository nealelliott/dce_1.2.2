/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_thread.h,v $
 * Revision 1.1.6.2  1996/02/18  23:04:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:18  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:14:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:31  root]
 * 
 * Revision 1.1.4.2  1994/03/23  16:06:53  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  18:14:10  keutel]
 * 
 * Revision 1.1.4.1  1994/02/22  19:16:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:32:49  marrek]
 * 
 * Revision 1.1.2.2  1993/08/12  12:40:42  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:31:09  marrek]
 * 
 * $EndLog$
 */
#ifndef _XT_THREAD_H
#define _XT_THREAD_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_thread_rcsid[] = "@(#)$RCSfile: xt_thread.h,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 23:04:21 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

#include <xt_thread_struct.h>
#include <assert.h>
#include <xdsp.h> /* for dsP_thread_id */


/* declarations and (re)definitions for threads */


#define bt_ava xt_this_thread_data()->bt_ava
#define bt_context xt_this_thread_data()->bt_context
#define bt_last xt_this_thread_data()->bt_last
#define bt_parms xt_this_thread_data()->bt_parms
#define bt_rdn xt_this_thread_data()->bt_rdn
#define bt_session xt_this_thread_data()->bt_session
#define private_object_tbl xt_this_thread_data()->private_object_tbl
#define public_object_tbl xt_this_thread_data()->public_object_tbl
#define string_tbl xt_this_thread_data()->string_tbl
#define rt_ava xt_this_thread_data()->rt_ava
#define rt_context xt_this_thread_data()->rt_context
#define rt_last xt_this_thread_data()->rt_last
#define rt_parms xt_this_thread_data()->rt_parms
#define rt_rdn xt_this_thread_data()->rt_rdn
#define rt_session xt_this_thread_data()->rt_session
#define session_tbl xt_this_thread_data()->session_tbl
#define workspace_tbl xt_this_thread_data()->workspace_tbl
#define xt_dump_objects xt_this_thread_data()->xt_dump_objects
#define xt_dump_objects_private xt_this_thread_data()->xt_dump_objects_private
#define xt_entry xt_this_thread_data()->xt_entry
#define xt_testname xt_this_thread_data()->xt_testname
#define xt_testcase_file xt_this_thread_data()->xt_testcase_file
#define itoa_buf xt_this_thread_data()->itoa_buf
#define ptoa_buf xt_this_thread_data()->ptoa_buf


/* following definition must agree with that in dump.c */
#define XT_PRINTF printf("[%d]%s: ",dsP_thread_id(),xt_this_thread_name()), printf

#endif /* _XT_THREAD_H */
