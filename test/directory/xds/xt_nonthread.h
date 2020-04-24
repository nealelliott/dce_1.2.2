/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_nonthread.h,v $
 * Revision 1.1.6.2  1996/02/18  23:04:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:00  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:13:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:57  root]
 * 
 * Revision 1.1.4.1  1994/02/22  19:15:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:29:34  marrek]
 * 
 * Revision 1.1.2.2  1993/08/12  12:25:53  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:26:54  marrek]
 * 
 * $EndLog$
 */
#ifndef _XT_NONTHREAD_H
#define _XT_NONTHREAD_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_nonthread_rcsid[] = "@(#)$RCSfile: xt_nonthread.h,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 23:04:10 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

#include <xt_thread_struct.h>


/* declarations and (re)definitions for non-threaded version */

extern struct thread_data_struct xt_nonthread_block;

#define bt_ava xt_nonthread_block.bt_ava
#define bt_context xt_nonthread_block.bt_context
#define bt_last xt_nonthread_block.bt_last
#define bt_parms xt_nonthread_block.bt_parms
#define bt_rdn xt_nonthread_block.bt_rdn
#define bt_session xt_nonthread_block.bt_session
#define private_object_tbl xt_nonthread_block.private_object_tbl
#define public_object_tbl xt_nonthread_block.public_object_tbl
#define string_tbl xt_nonthread_block.string_tbl
#define rt_ava xt_nonthread_block.rt_ava
#define rt_context xt_nonthread_block.rt_context
#define rt_last xt_nonthread_block.rt_last
#define rt_parms xt_nonthread_block.rt_parms
#define rt_rdn xt_nonthread_block.rt_rdn
#define rt_session xt_nonthread_block.rt_session
#define session_tbl xt_nonthread_block.session_tbl
#define workspace_tbl xt_nonthread_block.workspace_tbl
#define xt_dump_objects xt_nonthread_block.xt_dump_objects
#define xt_dump_objects_private xt_nonthread_block.xt_dump_objects_private
#define xt_entry xt_nonthread_block.xt_entry
#define xt_testname xt_nonthread_block.xt_testname
#define xt_testcase_file xt_nonthread_block.xt_testcase_file
#define itoa_buf xt_nonthread_block.itoa_buf
#define ptoa_buf xt_nonthread_block.ptoa_buf


/* following definition must agree with that in dump.c */
#define XT_PRINTF printf


#define xt_this_thread_id_and_name() ""

#endif /* _XT_NONTHREAD_H */
