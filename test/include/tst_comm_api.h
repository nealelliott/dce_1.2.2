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
 * $Log: tst_comm_api.h,v $
 * Revision 1.1.4.2  1996/02/18  23:05:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:48  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:54:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:37  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:44:45  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:25  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/10  22:40:58  bhaim
 * 	Common TET functions
 * 	[1994/03/10  22:40:03  bhaim]
 * 
 * $EndLog$
 */
#ifndef __TST_COMM_API_H__
#define __TST_COMM_API_H__

#include <sys/types.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>

extern int tst_chk_command(char *command, pid_t *pid);

extern int tst_chk_process(pid_t *pid);

extern void tst_dce_login(char *, char *, error_status_t *status);

extern int tst_tet_printf(const char *,...);

extern int tst_tet_syncall(int syncpoint ,int timeout);


#endif /* __TST_COMM_API_H__  */

