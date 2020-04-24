/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: svimpl.h,v $
 * Revision 1.1.6.1  1996/10/03  17:51:44  arvind
 * 	merge fixes for secval deadlock and [de]activate failures
 * 	[1996/08/16  20:23 UTC  lmm  /main/lmm_dced_secval_merge/1]
 *
 * 	merge fix for CHFts17901 from HP branch (secval threads)
 *
 * 	Submitted the fix for CHFts17901.
 *
 * 	Added TWO_MINUTES, sv_thd_ctlblk_t and selfcntx_ctlblk.
 * 	Removed sv_cond and sv_mutex.
 * 	sec_clientd() returns boolean.
 * 	[1996/03/19  15:08 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/1]
 *
 * Revision 1.1.4.2  1996/02/18  23:32:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:40  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:28:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/04  17:26 UTC  mdf
 * 	Merge for CHFts13120
 * 	[1995/12/08  18:00:08  root]
 * 
 * Revision 1.1.2.5  1994/10/17  19:31:06  sommerfeld
 * 	OT11548: fix hole in local preauth protocol
 * 	[1994/10/17  19:29:17  sommerfeld]
 * 
 * Revision 1.1.2.4  1994/06/13  20:05:41  rsalz
 * 	Code cleanup.
 * 	Server registration done in main now.
 * 	[1994/06/13  20:05:00  rsalz]
 * 
 * Revision 1.1.2.3  1994/05/10  20:15:19  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:50  rsalz]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:12  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:56:09  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:26:03  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:32  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#ifndef _SVIMPL_H
#define _SVIMPL_H

#include <pthread.h>
#include <oride_base.h>
#include <override.h>

#define ONE_MINUTE	60
#define TWO_MINUTES	(2 * ONE_MINUTE)
#define FIVE_MINUTES	(5 * ONE_MINUTE)
#define TEN_MINUTES	(10 * ONE_MINUTE)
#define ONE_HOUR	(60 * ONE_MINUTE)
#define PREAUTH_DIR	"/var/security/preauth"

/* ______________________________________________________________________ */

typedef struct sv_thd_ctlblk_s_t {
    pthread_mutex_t	m;
    pthread_cond_t	cv;
    volatile boolean	running;
    volatile boolean	stop;
    volatile boolean	suspend;
} sv_thd_ctlblk_t;

/* ______________________________________________________________________ */

extern char		*sv_override_file_name;
extern char		*sv_group_override_file_name;
extern char		*sv_clientd_binding;
extern char		*sv_preauth_dir;

extern sv_thd_ctlblk_t	selfcntx_ctlblk;

extern boolean32	sec_clientd(error_status_t *);
extern void		load_override_files(error_status_t *);
extern void		init_sv_preauth(error_status_t *);
#endif
