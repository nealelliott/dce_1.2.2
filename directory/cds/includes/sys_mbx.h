/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sys_mbx.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:34  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:18:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:50  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:02:44  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:09  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:39:36  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:17  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/29  15:50:22  peckham
 * 	Remove VMS conditionals.
 * 	Change the name of SYS_OPENMBX() back to sys_openmbx().
 * 	[1994/04/29  13:57:31  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:04:26  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:19:58  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:34:41  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:54:36  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _SYS_MBX_H
#define _SYS_MBX_H
/*
 * Module sys_mbx.h
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 *
 * MODULE DESCRIPTION:
 *
 * Networks & Communications Software Engineering
 *
 */

#include <threads_lib.h>
#include <server_diags.h>

typedef dthread_address_t             (*mbx_user_routine)(dthread_address_t, diag_request_t *);

typedef dthread_address_t             sys_mailbox;

int
sys_openmbx (
    unsigned char    *,
    int              ,
    mbx_user_routine ,
    int		     ,
    dthread_attr_t   *,
    sys_mailbox      *);

int
sys_closembx (
    sys_mailbox *);


int
sys_mbx_read (
    sys_mailbox   ,
    unsigned char *,
    int           *);

int
sys_mbx_write (
    sys_mailbox   ,
    unsigned char *,
    int           );

#endif  /* #ifndef _SYS_MBX_H */
