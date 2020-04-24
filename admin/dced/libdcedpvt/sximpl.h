/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sximpl.h,v $
 * Revision 1.1.2.2  1996/02/18  23:32:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:29  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:39  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/08  21:46 UTC  jss
 * 	Merge changes for CHFts14511, CHFts15038, CHFts14283.
 * 
 * 	HP revision /main/jss_mothra_fix3/1  1995/06/06  14:20 UTC  jss
 * 	Fix for OT12837. Fix lock usage problems.
 * 	[1995/12/08  20:49:05  root]
 * 
 * Revision 1.1.2.4  1994/08/25  16:32:37  rsalz
 * 	Add dced_waitpid and fix hostdata postprocessor reaping.
 * 	[1994/08/25  16:31:19  rsalz]
 * 
 * Revision 1.1.2.3  1994/05/05  20:07:47  rsalz
 * 	Initial server functionality drop.
 * 	[1994/05/05  20:01:08  rsalz]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:14  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:56:12  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:26:04  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:34  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#ifndef _SXIMPL_H
#define _SXIMPL_H
#include <scimpl.h>

typedef struct sx_pid_status_s_t {
    pid_t		pid;
    int			status;
    int                 done;
} sx_pid_status_t;

typedef struct sx_pid_status_list_s_t {
    unsigned32		count;
    sx_pid_status_t	*list;
} sx_pid_status_list_t;

typedef struct sx_pid_table_s_t {
    pthread_mutex_t		m;
    pthread_cond_t		cv;
    dce_lock_t			lock;
    sx_pid_status_list_t	pids;
} sx_pid_table_t;

extern sx_pid_table_t		sx_pids;
#endif
