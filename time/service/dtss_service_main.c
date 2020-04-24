/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_service_main.c,v $
 * Revision 1.1.21.2  1996/02/17  23:35:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:47  marty]
 *
 * Revision 1.1.21.1  1995/12/08  18:13:49  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/12  1995/06/19  13:18 UTC  pare
 * 	Merge fix for CHFts15503
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/3  1995/06/16  18:37 UTC  pare
 * 	FIX for CHFts15503
 * 
 * 	HP revision /main/HPDCE02/11  1995/01/30  22:45 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/10  1994/11/30  21:15 UTC  pare
 * 	Change return from utc_getTCPtime() to exit(1)
 * 
 * 	HP revision /main/HPDCE02/9  1994/11/23  19:52 UTC  pare
 * 	Check return value from utc_getTCPtime()
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/2  1994/11/17  21:16 UTC  pare
 * 	Check return from utc_getTCPtime
 * 
 * 	HP revision /main/HPDCE02/8  1994/11/16  14:59 UTC  pare
 * 	Merged changes.
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1994/11/16  14:53 UTC  pare
 * 	Removed tcp soket code -- replaced with library call utc_getTCPtime().
 * 
 * 	HP revision /main/HPDCE02/7  1994/10/31  20:42 UTC  pare
 * 	Add tcp/inetd time service (-t) to dtsdate command.
 * 
 * 	HP revision /main/HPDCE02/6  1994/10/10  17:44 UTC  pare
 * 	Merge bl21 changes.
 * 	[1995/12/08  17:25:51  root]
 * 
 * Revision 1.1.18.19  1994/10/05  16:09:47  tom
 * 	Bug 12498 - Move audit init ever later in the process, right
 * 	before we do rpc listen, since it requires interfaces to be registered.
 * 	[1994/10/05  16:06:33  tom]
 * 
 * Revision 1.1.18.18  1994/09/30  20:14:01  tom
 * 	Bug 12438 - Move audit init to after unix init so we will
 * 	put out audit file in var/adm/time.
 * 	[1994/09/30  20:10:10  tom]
 * 
 * 	HP revision /main/HPDCE02/5  1994/10/03  12:49 UTC  pare
 * 	Move dts_audit_init() call to after InitUnix(). InitUnix() does a chdir()
 * 	to the appropriate directory for the audit file.
 * 
 * 	HP revision /main/HPDCE02/4  1994/09/26  19:32 UTC  pare
 * 	Merge bl18 changes
 * 
 * Revision 1.1.18.17  1994/09/23  20:00:14  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:26:58  tom]
 * 
 * Revision 1.1.18.15  1994/08/31  20:30:54  cbrooks
 * 	CR11964 - auditing calls cause dtsd to dump core
 * 	[1994/08/31  20:21:27  cbrooks]
 * 
 * Revision 1.1.18.14  1994/08/25  21:47:48  cbrooks
 * 	CR11240 - retrieve config info from dced
 * 	[1994/08/25  20:17:13  cbrooks]
 * 
 * Revision 1.1.18.13  1994/08/24  20:44:23  cbrooks
 * 	CR 11856 - dtsdate -q needs different error status
 * 	[1994/08/24  20:11:14  cbrooks]
 * 
 * Revision 1.1.18.12  1994/08/23  20:32:29  cbrooks
 * 	Code Cleanup - ANSI switches
 * 	[1994/08/23  15:26:26  cbrooks]
 * 
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:57:04  cbrooks]
 * 
 * 	CR11495 - remove fprintfs in code; use serviceability
 * 	[1994/08/19  19:44:37  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:53:02  cbrooks]
 * 
 * Revision 1.1.18.11  1994/08/18  20:37:30  cbrooks
 * 	CR11760 - global mutex not inited
 * 	[1994/08/18  20:36:55  cbrooks]
 * 
 * Revision 1.1.18.10  1994/08/16  18:13:03  cbrooks
 * 
 * 	[1994/08/15  18:52:07  cbrooks]
 * 
 * 	CR 11494 - 64 bit changes
 * 	[1994/08/09  19:11:17  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/3  1994/08/18  13:49 UTC  pare
 * 	Merge changes from Hpdce02
 * 
 * 	HP revision /main/HPDCE02/2  1994/07/19  12:28 UTC  pare
 * 	fix merge problems
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  17:48 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.18.9  1994/08/05  20:47:41  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:41  cbrooks]
 * 
 * 	CR11240 dced config enhancements
 * 	[1994/08/03  14:58:29  cbrooks]
 * 
 * Revision 1.1.18.8  1994/06/30  19:45:25  cbrooks
 * 	CR11112 - new SAMS messages
 * 	[1994/06/28  20:13:34  cbrooks]
 * 
 * Revision 1.1.18.7  1994/06/24  19:34:02  cbrooks
 * 	CR10792 - change dtstime to dtsdate
 * 	[1994/06/24  19:33:01  cbrooks]
 * 
 * 	CR10972 - add prototocol sequence to error message
 * 	[1994/06/23  19:02:36  cbrooks]
 * 
 * 	CR 10792 - checkpoint
 * 	[1994/06/23  14:59:29  cbrooks]
 * 
 * Revision 1.1.18.4  1994/04/28  21:08:49  cbrooks
 * 	DTS Code Cleanup
 * 	[1994/04/28  21:06:46  cbrooks]
 * 
 * Revision 1.1.18.3  1994/04/21  22:06:04  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:59:49  cbrooks]
 * 
 * 	Checkpoint:4.15.94
 * 	[1994/04/15  21:14:48  cbrooks]
 * 
 * 	Checkpoint
 * 	[1994/03/28  20:28:44  cbrooks]
 * 
 * Revision 1.1.18.2  1994/03/11  18:29:54  rsalz
 * 	Fix dce_msg_define_msg_table; had to re-order #include's (OT CR 10123).
 * 	[1994/03/11  16:34:53  rsalz]
 * 
 * Revision 1.1.18.1  1994/01/25  17:16:40  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/25  17:15:40  rsalz]
 * 
 * 	HP revision /main/HPDCE01/2  1994/05/03  18:09 UTC  pare
 * 	Fix for CHFts11838
 * 
 * 	HP revision /main/HPDCE01/pare_rodan/1  1994/05/03  18:06 UTC  pare
 * 	Add wait loop for DCE services
 * 
 * Revision 1.1.11.5  1993/11/24  19:53:20  pare
 * 	Fix OT6076.
 * 	Redirect stdin, stdout & stderr to /dev/null when becoming a
 * 	daemon so that starting rpcd via remsh will work.
 * 	[1993/11/24  19:52:27  pare]
 * 
 * Revision 1.1.11.4  1993/09/28  13:29:52  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/18  13:44:49  pare]
 * 
 * Revision 1.1.13.3  1993/09/15  17:48:37  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.14.2  1993/09/18  13:29:20  pare
 * 	DCE1_0_3b03 Merge
 * 
 * Revision 1.1.16.2  1993/07/14  21:29:09  tom
 * 	Bug 7794 - Init random number generator in main for RandomizeInterval()
 * 	[1993/07/14  20:44:45  tom]
 * 
 * Revision 1.1.13.3  1993/09/15  17:48:37  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.11.3  1993/07/16  12:35:30  truitt
 * 	OT7794: Initialize random number generator (srand48) in main for the
 * 	        RandomizeInterval() function.
 * 	[1993/07/16  12:34:54  truitt]
 * 
 * Revision 1.1.16.2  1993/07/14  21:29:09  tom
 * 	Bug 7794 - Init random number generator in main for RandomizeInterval()
 * 	[1993/07/14  20:44:45  tom]
 * 
 * Revision 1.1.11.2  1993/07/08  20:18:31  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:17:54  root]
 * 
 * Revision 1.1.9.10  1993/06/08  18:32:55  truitt
 * 	CHFts07767: Add a ReturnIfError() function call after the
 * 	InitGlobalSet() function call to trap any global init errors.
 * 	[1993/06/08  18:32:30  truitt]
 * 
 * Revision 1.1.9.9  1993/06/02  18:36:37  truitt
 * 	CHFts07497: Added call to utc_semcleanup() to get rid of the
 * 	allocated semaphore identifiers when dtsd cleans itself up.
 * 	[1993/06/02  18:36:14  truitt]
 * 
 * Revision 1.1.9.8  1993/05/28  13:10:08  truitt
 * 	Corrected bonehead mistake in version string.
 * 	[1993/05/28  13:09:42  truitt]
 * 
 * Revision 1.1.9.7  1993/05/25  12:56:28  truitt
 * 	Add new revision control string.
 * 	[1993/05/25  12:56:06  truitt]
 * 
 * Revision 1.1.9.6  1993/05/10  18:11:50  truitt
 * 	Add version control string.
 * 	[1993/05/10  18:11:15  truitt]
 * 
 * Revision 1.1.9.5  1993/04/23  12:33:56  truitt
 * 	Merge DCE1_0_2B23 with GAMERA.
 * 	[1993/04/22  17:55:07  truitt]
 * 
 * Revision 1.1.1.9  1993/04/21  20:52:48  root
 * 	    Loading drop DCE1_0_2B23
 * 
 * Revision 1.1.9.4  1993/04/06  18:13:43  truitt
 * 	Remerge with dce1.0.2 due to failure of RCS tree.
 * 	[1993/04/06  18:13:14  truitt]
 * 
 * Revision 1.1.9.3  1993/04/01  19:35:32  truitt
 * 	OT7514: Don't call BugCheckOSstatus when child returns status,
 * 	just exit with that status.  Actual diff provided did not match
 * 	our version of this file.
 * 	[1993/04/01  19:34:40  truitt]
 * 
 * Revision 1.1.9.2  1993/04/01  16:37:50  truitt
 * 	Removed covinit function.  No longer needed for VISTA instrumentation.
 * 	[1993/04/01  16:35:13  truitt]
 * 
 * Revision 1.1.8.11  1993/03/17  16:37:18  tom
 * 	Bug 7514 - Don't call BugCheckOSstatus when child returns status,
 * 	just exit with that status.
 * 	[1993/03/17  16:02:06  tom]
 * 
 * Revision 1.1.3.6  1993/03/10  15:21:48  truitt
 * 	Corrected a misleading comment concerning unix debug_mode flag.
 * 	[1993/03/10  15:30:58  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:34:62  osfrcs
 * 	Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.8.10  1993/01/29  15:09:48  tom
 * 	Bug 7052 - Check error status in InitializeService() before telling
 * 	parent to detach.
 * 	[1993/01/28  22:56:24  tom]
 * 
 * Revision 1.1.8.9  1993/01/22  23:51:40  jwade
 * 	Fix compile errors with last drop.
 * 	[1993/01/22  23:21:52  jwade]
 * 
 * Revision 1.1.8.8  1993/01/22  15:47:47  jwade
 * 	Do not call BugCheckOSstatus if the statusBlk.sysStatus is
 * 	sec_login_s_no_current_context.  This prevents a core dump
 * 	if the machine principal is destroyed but will print a message
 * 	to the console.
 * 	[1993/01/19  21:39:30  jwade]
 * 
 * Revision 1.1.8.7  1992/12/30  16:38:45  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:21:06  htf]
 * 
 * Revision 1.1.3.5  1992/12/21  23:57:09  smythe
 * 	added home directory for core dumps from OSF OT 3912
 * 	[1992/12/21  20:08:36  smythe]
 * 
 * Revision 1.1.8.6  1992/12/18  17:08:08  tom
 * 	Bug 3912 - Change to a known directory in case of core dumps.
 * 	[1992/12/18  17:06:52  tom]
 * 
 * Revision 1.1.8.5  1992/12/15  16:06:55  tom
 * 	Bug 6410 - Add setlocale for i18n.
 * 	[1992/12/15  15:41:29  tom]
 * 
 * Revision 1.1.8.4  1992/10/27  22:47:16  bolinger
 * 	Fix OT defect 5650:  put dtsd into its own session correctly on OSF/1.
 * 	The previous code, though needlessly complex (and non POSIX-compliant),
 * 	worked until the SVR4 merge, at which point a change in the symbols
 * 	used to select what code was used here caused the wrong code to be
 * 	chosen on OSF/1.
 * 
 * 	Recast and simplify the whole #ifdef chain for session initialization.
 * 	Rather than distinguishing SVR4 versus BSD versus other, distinguish
 * 	only POSIX from non-POSIX.  (Both SVR4 and the reference platforms are
 * 	POSIX-compliant.)
 * 	[1992/10/27  17:35:36  bolinger]
 * 
 * Revision 1.1.8.3  1992/10/13  18:13:16  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:15:17  marty]
 * 
 * Revision 1.1.9.2  1992/09/30  20:22:10  garyf
 * 	change to conditional for setpgroup call
 * 	[1992/09/29  20:29:04  garyf]
 * 
 * Revision 1.1.8.2  1992/09/29  20:43:56  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:06:57  sekhar]
 * 
 * Revision 1.1.3.4  1992/08/26  18:31:18  pare
 * 	Added covinit function for instrumenting with Vista
 * 	[1992/08/26  18:30:49  pare]
 * 
 * Revision 1.1.3.5  1992/06/24  16:52:03  grober
 * 	HP/UX Porting Changes
 * 	[1992/06/22  15:58:14  grober]
 * 
 * Revision 1.1.3.3  1992/06/18  23:46:26  smythe
 * 	merged hpux changes with b19 changes
 * 	[1992/06/18  22:10:03  smythe]
 * 
 * Revision 1.1.1.4  1992/06/16  07:07:03  root
 * 	Loading drop Dce1_0_1b19
 * 
 * Revision 1.1.3.4  1992/06/10  19:22:10  ko
 * 	Fix OT. 4121, 4098 - not properly detatching from parent, not building
 * 	on AIX systems.
 * 	[1992/06/10  15:40:01  ko]
 * 
 * Revision 1.1.3.3  1992/06/04  18:05:36  ko
 * 	Fix OT 3549 - dtssd parent exits before management thread initialized.
 * 	Created a shared pipe between the parent and child.  The parent blocks
 * 	on a read on the pipe after forking off the child.  The child writes to
 * 	pipe when it initializes its management, causing the parent to exit.
 * 	Abnormal exits by the child are reported by the parent.
 * 	Also some code movement out of InitializeService to try and isolate
 * 	the new routines.
 * 	[1992/06/04  15:56:57  ko]
 * 
 * Revision 1.1.3.2  1992/05/11  14:46:58  ko
 * 	fix ot 2840 & 2862, use LogeDCEevent, BugCheck on ReadHWclock error
 * 	[1992/05/07  15:42:43  ko]
 * 
 * Revision 1.1  1992/01/19  15:34:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1989,1990,1991 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 */

/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */

#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <locale.h>
#include <stdarg.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dce_priv_proc.h>
#include <dts.h>
#include <dtss_includes.h>
#include <dtss_service.h>
#include <dcedtssvc.h>
#include <dcedtsmac.h>
#include <dce/dcesvcmsg.h>
#include <dce/dcedtsmsg.h>
#include <dce/dced.h>

#include <sys/errno.h>


#if	defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <aud_evtsp.h>
#endif	/* defined(DCE_AUD_POINTS) */

/*
 * forward declarations of procedures in this module
 */

typedef struct dts_dtsd_cmdargs_s_t 
{
    boolean server;
    boolean client;
    boolean global ;
    boolean clockset ;
    int couriertype;
}dts_dtsd_cmdargs_t;

/*
 * Prototypes
 */

static StatusBlk InitializeService (int, char *[], dts_dtsd_cmdargs_t *); /* init this module, other inits */
static StatusBlk Cleanup (void);           /* cleanup before exiting */
static int ProcessEvent (EventBlk *);            /* Process event */
static VOID dts_dtsd__usage_error(void);

static VOID InitUnix(int _1, char * _2[], int _3[2], dts_dtsd_cmdargs_t * _4);
static VOID DetachUnix(int [2]);
static VOID SetIDs(void);

static StatusBlk dts_dtsd__initialize_from_args( const dts_dtsd_cmdargs_t *
				const) ;

static int dts_dtsd_dtsdate( int, char *[]) ;

static void dts_dtsdate_usage_error( void ) ;

static void dts_dtsd_get_remote_time( char *, utc_t *, utc_t *, 
				     error_status_t *) ;

void dts_svc_init( char * ) ;

void get_security_config_info ( sec_id_t *, sec_id_t *, error_status_t	*);

void dts_dtsd_init_mutex( void ) ;

void dts_dtsd_update_attributes(uuid_t *, boolean32, error_status_t *);

#if	defined(DCE_AUD_POINTS)
void dts_aud_init(void);
#endif	/* defined(DCE_AUD_POINTS) */

/*
 * Global Variables
 */

ShmBuf shmBuf;				/* Shared info between modules */
					/* all modules have shmPtr defined */

ShmBuf *shmPtr;				/* pointer to shared memory */
 
static utc_t utc;


typedef struct dts_dtsd_global_data_s_t
{
    pthread_mutex_t dtsd_global_mutex ;
    pthread_mutexattr_t dtsd_global_mutexattr ;
    boolean32 started_from_dced;
    boolean32 running_as_server;
    boolean32 read_attrs_from_file ;
    int debug_mode;                        /* debug flag on? */
    uuid_t conf_id;		/* srvrconf object UUID */
    uuid_t exec_id;		/* srvrexec object UUID */
} dts_dtsd_global_data_t ;


/* allow 0 fill for the uuid fields */
dts_dtsd_global_data_t dts_dtsd_global_data = 
{
    0 
};

#define DDGD dts_dtsd_global_data

#define DTS_LOCK_GLOBAL pthread_mutex_lock( &DDGD.dtsd_global_mutex) 

#define DTS_UNLOCK_GLOBAL pthread_mutex_unlock( &DDGD.dtsd_global_mutex)

/*
 * Our Serviceability handle 
 */

dce_svc_handle_t dts_svc_handle;


RpcBuf  rpcBuf;	  /*Shared RPC and security relevant info between modules */
RpcBuf  *rpcBufPtr;  /* This pointer is externally defined where needed */

#if	defined(DCE_AUD_POINTS)
dce_aud_trail_t dts_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

/*
 *   The following vaiable is used to increase the stack size of all threads
 *   created by DTS
 */
pthread_attr_t thread_attribute;

/*
 * The following two dimentional array defines the legal state
 * transitions for the DTSS service.  Access to the array is
 * by use of the IsState macro and the StateChange routine.
 * The array should not be accessed directly.
 * Rows indicate the current state and cols indicate the desired
 * next state.
 *
 *
 * CURR    NEXT    ISSUED BY/FROM		COMMENTS/RESTRICTIONS/
 *						  ROUTINE REFERENCES
 * -------------------------------------------------------------------
 * NCR     OFF     Directive Create		Created once
 * OFF	   ON	   Directive Start Enable     See Rtn.    ReInitState,
 *							  DirectiveEnable,
 *							  StartCmd
 *
 * ON	   OFF	   Directive Disable	 	See Rtn. DirectiveDisable.
 * ON	SYNC_SET   Directive Synch 		See Rtn. SynchCmd,
 *							 DirectiveSynchronize
 *
 * ON   SYNC_ADJ   Directive Synch, SYNCH	See SynchCmd, Synchronize
 *		   Timer, Update Complete           DirectiveSynchronize
 *						    DirectiveUpdate
 *
 * ON   UPDATE	   Directive Update		See DirectiveUpdate.
 *						Will cancel a synchronization,
 *						See Synchronize(abort).
 *						Will set
 *						the next synch timer to go
 *						off and cancel any current
 *						updates and adjustment,
 *						see DirectiveUpdate.
 *
 * UPDATE OFF	Directive Disable	Cancels the current update.  See
 *					DirectiveDisable.
 *
 * UPDATE UPDATE Directive Update Time  Cancel the current update, spec. a
 *					new update, See DirectiveUpdate.
 *
 * UPDATE SYNC_ADJ Synch Timer Exp.   Update is complete, resume synchs.
 *				      See DirectiveUpdate for timer setting,
 *				      Synchronize for state change.
 *
 * UPDATE ON	Set TDF Directive     Canceled update due to TDF change,
 *				      See SetTDF.
 *
 * S_SET OFF	Disable Directive
 * S_ADJ OFF	Set TDF Directive     See DirectiveDisable for disable,
 *				      Synchronize, SetTDF for synch cancel
 *				      and reschedule.
 *
 * S_SET S_SET  Directive Synchron    See DirectiveSynchronize, Synchronize.
 * S_ADJ S_ADJ
 *
 * S_SET ON	Sync Completed	      See Synchronize.
 * S_ADJ
 */
static char DTSSserviceState[K_STATE_COUNT][K_STATE_COUNT] =
{
    /* off 	on	synch_a	update synch_set not_cr<--- next state */
	0,	1,	0,	0,	0,	   1,    /*  off      */
	1,	0,	1,	1,  	1,         0,    /*  on       */
	1,	1,	1,	0,      0,         0,    /*  sync_adj */
	1,	1,	1,	1,      0,         0,    /*  update   */
	1,      1,      0,      0,      1,         0,    /*  sync_set */
        1,      0,      0,      0,      0,         0     /*  not created */
};

/*
 * local constants
 */

#define K_PROC_EVENT_EXIT     0        /* values returned by ProcessEvent */
#define K_PROC_EVENT_CONTINUE 1        /* to the main loop */


const int WRITE_SIDE_OF_PIPE = 1 ;
const int READ_SIDE_OF_PIPE  = 0 ;

const char * dtsdate = "dtsdate" ;
const char * dtstime = "dtstime" ; 
	    

/*
 *++
 *  main ()
 *  Functional Description:
 *	Calls all the initialization routines and then
 *	goes into the top level loop of the DTSS Service.
 *	When the control program tells us to exit, this
 *	routines calls the cleanup and then exits.
 *  Implicit Outputs:
 *	status returned to DCL or other caller
 *  Side Effects:
 *	runs the DTSS Service
 *--
 */

int main (int argc, char **argv)
{
    StatusBlk statusBlk;
    EventBlk *eblkPtr;
    int exitFlag;
    dts_dtsd_cmdargs_t cmdargs = { FALSE, FALSE, 
			FALSE, FALSE, K_ROLE_BACKUPCOURIER};
    boolean role_assigned = FALSE  ;
    char * progname = 0 ;
    /*
     * Initialization and startup.
     */
    (void)setlocale(LC_ALL, "");
    srand48(time(NULL));

    progname = strrchr(argv[0], '/') ;
    if (progname) 
	progname++;
    else
	progname = argv[0];

    dts_svc_init( progname ) ;

    if (strcmp(progname, dtsdate) == 0) 
    {
	int status = dts_dtsd_dtsdate( argc, argv) ;
	exit(status);
    }
	
    dts_dtsd_init_mutex() ; 


    /*
     * NOTE:  All this state stuff has to change because of the
     *	      inclusion of the DNS advertiser.  Consequently, state
     *	      is not handled correctly, nor is it currently reported.
     */


    statusBlk = InitializeService (argc, argv, &cmdargs);
    if (IsError (statusBlk) &&
       (statusBlk.sysStatus == sec_login_s_no_current_context)) {
	exit(1);
    }

    if (IsError (statusBlk))
        BugCheckOSstatus (statusBlk.sysStatus);

    role_assigned = (cmdargs.client || cmdargs.server) ;

    if (role_assigned) 
    {
	/* queue up some dummy events based on data in the cmdargs */
	/* and then just enter the main loop */

	statusBlk = dts_dtsd__initialize_from_args( &cmdargs);
	if (IsError(statusBlk))
	    goto DTSD_EXIT;
    }
    
    /*
     * The main loop. DTSS service spends most of its time in this loop
     * processing events. Event blocks are created in interrupt routines
     * in response to a message arriving or a timer going off. The routine
     * GetNextEvent finds the next event in priority order and returns it
     * to this loop. This loop then dispatches to the appropriate processing
     * routine.
     */
    dce_svc_printf(DCE_SVC(dts_svc_handle, ""), dts_s_general,
	svc_c_sev_notice, dts_s_starting);
    do
    {
        eblkPtr = GetNextEvent ();
        exitFlag = ProcessEvent (eblkPtr);
    } while (exitFlag != K_PROC_EVENT_EXIT);

    /*
     * Tell CP that we're turning off.
     * Note that TSstate has nothing to do with the internal state machine.
     * Its states are TSoff, TSinitializing and TSon.
     */
    shmPtr->mgtParams.TSstate = K_STATE_OFF;

 DTSD_EXIT:			/* announce exit and clean up ... */
    ;
    
    dce_svc_printf(DCE_SVC(dts_svc_handle, ""), dts_s_general,
	svc_c_sev_notice, dts_s_exiting);
    Cleanup ();

    /*NOTREACHED*/
    return 0;
}


/*
 *++
 *  ProcessEvent (eblkPtr)
 *	Called only from main loop.
 *	Processes the event that GetNextEventreturned.
 *  Inputs:
 *	Pointer to EventBlk
 *  Implicit Inputs:
 *	shared memory mapped and locked
 *  Value Returned:
 *	K_PROC_EVENT_CONTINUE unless control program has told us to exit
 *	and K_PROC_EVENT_EXIT in that case.
 *--
 */
static
int ProcessEvent (EventBlk *eblkPtr)
{
    StatusBlk statusBlk;

    switch (eblkPtr->event)
    {
        /*
         * Clean up and exit.
         */
        case ETterminate:
            return (K_PROC_EVENT_EXIT);

        /*
         * Process a received provider packet (from the time provider).
         */
        case ETprovider:
            /*
             * Process the received provider packet.
             * Synchronize with new TP stimulus
             */
            Synchronize(INrcvdTPresponse, eblkPtr);

	    WakeTPthreadWithResp (eblkPtr);

	    break;

        /*
	 * Process a provider failure event block (from the TP interface)
	 */
	case ETprvdrFail:
	    Synchronize (INrcvdTPresponse, eblkPtr);
	    WakeTPthreadWithResp (eblkPtr);
	    break;

        /*
         * Process a received control packet (from the control program).
         */
        case ETcontrol:
            /*
             * Process the received control packet.
             */
            statusBlk = ProcessControlMsg (eblkPtr);
            if (IsError (statusBlk))
                BugCheckOSstatus (statusBlk.sysStatus);

            break;

        /*
	 * Process a response to an RPC time request
	 */
	case ETtimeRespRPC:
	    Synchronize (INrcvdTimeResponse, eblkPtr);
	    FreeTimeReqEvent (eblkPtr);
	    break;

        /*
         * The following are timer events.
         */

        case ETsynchronize:

            Synchronize (INsynchTimer, eblkPtr);
            /*
             * state machine will start timer again when this
             * synchronization is complete.
             */
            break;

        case ETqueryTimeout:
            Synchronize (INqueryTimeout, eblkPtr);
            break;

        default:
            BugCheck ();

    }
    return (K_PROC_EVENT_CONTINUE);
}


void dts_svc_init(char * progname )
{
    error_status_t st;

    /* begin */

    dce_svc_set_progname(progname, &st);
    dts_svc_handle = dce_svc_register(dts_svc_table, (idl_char*)"dts", &st);
    dce_msg_define_msg_table(dts__table,
	sizeof dts__table / sizeof dts__table[0], &st);
    return;
}

#if	defined(DCE_AUD_POINTS)
/*
 * Open the audit trail file
 *
 * This must be called after we register at least one interface,
 * such as the management interface.
 */
void dts_aud_init(void)
{
    error_status_t st = error_status_ok ;

    dce_aud_open(aud_c_trl_open_write, "dts_aud_trail", 
	DTS_AUD_FIRST_EVT_NUMBER, DTS_AUD_NUM_OF_EVTS, &dts_aud_trail, &st);
    if (st != error_status_ok) {
        dce_svc_printf(DCE_SVC(dts_svc_handle, ""), dts_s_general,
       	        svc_c_sev_error, dts_s_aud_open_file_failed, "dts_aud_trail");
	exit(1);
    }
    return;
}
#endif	/* defined(DCE_AUD_POINTS) */

/*
 *++
 *  InitializeService ()
 *	Initializes the Time Service.
 *	The initialization starts at execution, after
 *	DTSS$CONTROL creates the DTSS$SERVICE process.
 *  Implicit Inputs:
 *	assumes that shared memory is mapped and locked
 *  Value Returned:
 *	StatusBlk
 *  Side Effects:
 *	changes to shared memory, buffers allocated, reads posted, etc.
 *--
 */
static
StatusBlk InitializeService (
    int argc, 
    char **argv, 
    dts_dtsd_cmdargs_t * argptr
    )
{
    StatusBlk statusBlk;
    int       i;
    int count;
#define FIVE 5
#define ONE_MINUTE 60
    unsigned32     rpcStatus;
    int parent_child_pipe[2];
    unsigned char  tempString[512];
    boolean32 dced_started_me ;
    int locstat;

    /* begin */

    /*
     * Almost all modules use the global shmPtr to access the shared
     * variables.
     */
    shmPtr = &shmBuf;

    InitUnix(argc,argv,parent_child_pipe, argptr);

    /* N.B. at this point, we are running in the child process */

#ifdef K_DTSS_STACKSIZE 
    /*
     * Initialize the pthread attribute with a stack size
     * of K_DTSS_STACKSIZE
     */
    if (pthread_attr_create(&thread_attribute) < 0)
    {
        BugCheckOSstatus(errno);
    }

    if (pthread_attr_setstacksize(&thread_attribute, (long)K_DTSS_STACKSIZE) < 0)
    {
        BugCheckOSstatus(errno);
    }
#else /* K_DTSS_STACKSIZE */	
    /* If it is not a 64-bit machine, set thread_attr = to the default *
     * vaule.			*/
    thread_attribute = pthread_attr_default;
#endif  /* K_DTSS_STACKSIZE */


    /*
     * Initialize RPC buffer fields.
     */
    rpcBufPtr = &rpcBuf;

    uuid_create_nil (&rpcBuf.TServicePrincUUID, &rpcStatus);
    strcpy( (char *)rpcBuf.cellProfile, CELLPROFILE_NAME);
    rpcBuf.lanProfile  = NULL;

    /*
     * Initialize Object UUID from the Time Service's interface UUID.
     * Use the interface UUID as its object UUID when we register it
     * with the endpointmapper and the namespace.  This object UUID is
     * also used with the rdacl interface.
     */
#if CONFIG_SECURITY
    uuid_from_string ((unsigned_char_t *)TIME_SERVICE_UUID,
		      &rpcBuf.TServiceObjUUID,
		      &rpcStatus);
#else
    uuid_create (&rpcBuf.TServiceObjUUID, &rpcStatus);
#endif /* CONFIG_SECURITY */

    rpcBuf.objUUIDvector.count = 1;
    rpcBuf.objUUIDvector.uuid[0] = &rpcBuf.TServiceObjUUID;

    /*
     * Get the host entry name.
     */
    dce_cf_get_host_name ((char **)&rpcBuf.hostName, &rpcStatus);
    if (rpcStatus != dce_cf_st_ok)
    {
	dce_error_string_t tempstring ;
	dce_svc_printf( DTS_S_CF_HOSTNAME_MSG,
		       dce_dts_error_text( rpcStatus, tempstring));
	statusBlk.status = K_DCE_ERROR;
	statusBlk.sysStatus = rpcStatus;
	return (statusBlk);
    }

#if CONFIG_SECURITY
    /*
     * Get the security-relevant configuration info
     */

#ifdef __hpux
/* Add security re-try loop -- try every minute for five minutes
 * during initialization. This is necessary for the case where the
 * DCE client machine comes up before the server.
*/
    count = 0;
    do
    {
        get_security_config_info (&rpcBuf.host_principal,
			      &rpcBuf.my_cell,
			      &rpcStatus);
        if (rpcStatus != error_status_ok) {
            ++count;
            dce_error_inq_text(rpcStatus, tempString, &locstat);
	    fprintf(stderr, "\tError in initialization: %s \n",tempString);
	    fprintf(stderr, "\tWaiting one minute for DCE security services -- retries remaining: %d\n", FIVE-count);
	    sleep(ONE_MINUTE);
        }
	else {
	    fprintf(stderr, "\tSecurity initialization successful.\n");
	    break;
        }
     }
     while( count < FIVE);
#else
	get_security_config_info (&rpcBuf.host_principal,
				      &rpcBuf.my_cell, &rpcStatus);
#endif /* __hpux */

    if (rpcStatus != error_status_ok)
    {
	dce_svc_printf( DTS_S_SECURITY_INIT_MSG, 
		       dce_dts_error_text( rpcStatus, tempString));
	fprintf(stderr, "\tError in initialization: %s \n",tempString); 
	statusBlk.status = K_DCE_ERROR;
	statusBlk.sysStatus = rpcStatus;
	return (statusBlk);
    }
#endif /* CONFIG_SECURITY */

    /*
     * Set up the default time service principal name
     */
    strcpy ((char *)rpcBuf.DefServPrincName, (char *)rpcBufPtr->hostName);
    strcat ((char *)rpcBuf.DefServPrincName, DEF_TSERV_PRINC_NAME);

    /*
     * Initialize with the default management parameters.
     */

    UTCassign (&shmBuf.mgtParams.advertiseInt, K_ADVERTISE_INT_LO,
		K_ADVERTISE_INT_HI);
    UTCassign (&shmBuf.mgtParams.checkInt,K_CHECK_INT_LO,K_CHECK_INT_HI);

    shmBuf.mgtParams.courierRole 	      = K_ROLE_BACKUPCOURIER;
    shmBuf.mgtParams.myCourierRole            = K_BACKUP_ROLE_NONCOURIER;

    shmBuf.mgtParams.epochNumber 	      = K_EPOCH_NUMBER;

    UTCassign (&shmBuf.mgtParams.errorTolerance,K_ERROR_TOLERANCE_LO,
		K_ERROR_TOLERANCE_HI);
    UTCassign (&shmBuf.mgtParams.maxInacc,K_MAX_INACC_LO,K_MAX_INACC_HI);


    shmBuf.mgtParams.minServers 	      = K_MIN_SERVERS_CLERK;

    shmBuf.mgtParams.repetitions 	      = K_REPETITIONS;

    shmBuf.mgtParams.LANtimeOut.lo 	      = K_LAN_TIMEOUT_LO;
    shmBuf.mgtParams.LANtimeOut.hi 	      = K_LAN_TIMEOUT_HI;

    shmBuf.mgtParams.WANtimeOut.lo 	      = K_WAN_TIMEOUT_LO;
    shmBuf.mgtParams.WANtimeOut.hi 	      = K_WAN_TIMEOUT_HI;

    shmBuf.mgtParams.syncHold.lo 	      = K_SYNC_HOLD_CLERK_LO;
    shmBuf.mgtParams.syncHold.hi 	      = K_SYNC_HOLD_CLERK_HI;

#ifdef BIT64_CHANGES
    UTCassign (&shmBuf.mgtParams.LANtimeOut,K_LAN_TIMEOUT_LO,K_LAN_TIMEOUT_HI);
    UTCassign (&shmBuf.mgtParams.WANtimeOut,K_WAN_TIMEOUT_LO,K_WAN_TIMEOUT_HI);
    UTCassign (&shmBuf.mgtParams.syncHold,K_SYNC_HOLD_CLERK_LO,
		K_SYNC_HOLD_CLERK_HI);
#endif

    shmBuf.mgtParams.serverTrace 	      = FALSE;
    shmBuf.mgtParams.commTrace 		      = FALSE;
    shmBuf.mgtParams.syncTrace 		      = FALSE;

    shmBuf.mgtParams.DNAversion.dtss_support  = K_DNA_VERSION_SUPPORT;
    shmBuf.mgtParams.DNAversion.dtss_minor    = K_DNA_VERSION_MINOR;
    shmBuf.mgtParams.DNAversion.dtss_major    = K_DNA_VERSION_MAJOR;
    shmBuf.mgtParams.DNAversion.dtss_level    = K_DNA_VERSION_LEVEL;

    shmBuf.mgtParams.DECdts.dtss_support      = K_DECDTS_VERSION_SUPPORT;
    shmBuf.mgtParams.DECdts.dtss_minor 	      = K_DECDTS_VERSION_MINOR;
    shmBuf.mgtParams.DECdts.dtss_major 	      = K_DECDTS_VERSION_MAJOR;
    shmBuf.mgtParams.DECdts.dtss_level	      = K_DECDTS_VERSION_LEVEL;

    shmBuf.mgtParams.timeVersion.dtss_support = K_TIME_VERSION_SUPPORT;
    shmBuf.mgtParams.timeVersion.dtss_minor   = K_TIME_VERSION_MINOR;
    shmBuf.mgtParams.timeVersion.dtss_major   = K_TIME_VERSION_MAJOR;
    shmBuf.mgtParams.timeVersion.dtss_level   = K_TIME_VERSION_LEVEL;

    shmBuf.mgtParams.variant 		      = K_VARIANT;
    shmBuf.mgtParams.TSstate 		      = K_STATE_NOT_CREATED;
    shmBuf.mgtParams.hasProvider 	      = 0;
    shmBuf.mgtParams.traceFlags 	      = 0;

 
    /* XXX set Time Server Entry Name */
    memset((VOID *)shmBuf.mgtParams.servPrincName, 0, K_MAX_ENTRY_LENGTH);
    strcpy((char *)shmBuf.mgtParams.servEntryName,
	   (char *)rpcBufPtr->hostName);
    strcat((char *)shmBuf.mgtParams.servEntryName, DEF_SERVER_ENTRY_NAME);
    strcpy((char *)shmBuf.mgtParams.groupName, DEF_TSERV_GROUP_NAME);

    /*
     * 7-13-94 CBrooks 
     * add call to retrieve any configuration data that may have been 
     * stored in the dtsd
     *
     * Set the variant parameter now, so we can use is IsServer() macro 
     * later on
     */

    if (argptr -> server) 
	shmBuf.mgtParams.variant = VTserver ;
    else if (argptr -> client)
	shmBuf.mgtParams.variant = VTclerk ;
    
    if (argptr -> server ) 
	shmBuf.mgtParams.minServers = K_MIN_SERVERS_SERVER ;

    /*
     * only look for config attributes if we have a personality -
     * i.e. we're either a server or a clerk 
     * If we're started with no identity, then we don't even look 
     * for configuration attributes
     */

    if (IsServer() || IsClerk())
    {
	boolean32 use_file, fetch_attributes ;
	DTS_LOCK_GLOBAL ;
	fetch_attributes = (DDGD.started_from_dced || 
			    (DDGD.debug_mode && DDGD.read_attrs_from_file));
	use_file = DDGD.read_attrs_from_file ;
	DTS_UNLOCK_GLOBAL;
	if (fetch_attributes) 
	{

	    dce_error_string_t tempstring;

	    dts_dtsd_update_attributes( &DDGD.exec_id, use_file,
				       &rpcStatus) ;

	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_config, svc_c_debug1,
			   "Read configuration attributes from dced: %s\n",
			    (rpcStatus == error_status_ok) ? "Success" : 
			   dce_dts_error_text( rpcStatus, tempstring)));
	}

    }
    
    memset((VOID *)&shmBuf.myNodeId, 0, sizeof(shmBuf.myNodeId));

    for ( i = 0; i < K_NUM_COUNTERS; i++ )
    {
	UTCzero (&(shmBuf.counters[i]));
    }

    statusBlk = ReadHWclkParams(&shmPtr->clockParams.clockResolution,
				&shmPtr->clockParams.clockDrift);

    if (statusBlk.status != DTSS_SUCCESS)
    {
	BugCheckOSstatus(statusBlk.sysStatus);
    }

    /*
     * Initialize other fields
     */
    utc_gettime( &shmBuf.mgtParams.creationTime );

    /*
     * Initialize the sub-systems.
     */
    statusBlk = InitState ();
    ReturnIfError (statusBlk);
    statusBlk = InitEvent ();
    ReturnIfError (statusBlk);

    /*
     * Notify our parent that we are initialized.
     * only if *not* started from dtsd. 
     */
    DTS_LOCK_GLOBAL;
    dced_started_me = DDGD.started_from_dced;
    DTS_UNLOCK_GLOBAL;
    
    if (! dced_started_me) 
    {
	if (parent_child_pipe[ WRITE_SIDE_OF_PIPE] > 0)
	    DetachUnix(parent_child_pipe);
    }

    ReturnDTSSstatus (DTSS_SUCCESS, statusBlk);
}

/*
 *++
 *  Cleanup ()
 *	Cleans up before exiting.  Note that the DTSS active flag in VMS, is
 *	cleared in the image rundown routine so that it is always cleared on
 *	exit, regardless of the way we exit.
 *  Implicit Inputs:
 *	Assumes that shared memory is mapped and locked.
 *  Value Returned:
 *	StatusBlk
 *--
 */
static
StatusBlk Cleanup (void)
{
    StatusBlk statusBlk;
    /*
     * Stop clock adjustment so that base times remains consistent.
     */
    AdjustClkEnd();

    /*
     * Close the channel to the control program.
     */
    ShutdownCPinterface ();

    /* close other channels */
    ShutdownTPinterface ();

    /*
     * Clean up RPC interfaces.
     */
    ShutDownRPC ();

    ReturnDTSSstatus (DTSS_SUCCESS, statusBlk);
}


/*
 *++
 *  StateChange()
 *  This routine moderates an internal DTSS service state change.  The
 *  state of the service becomes equal to the value of the formal parameter
 *  if the state transition is legal.
 *  Inputs:
 *    nextState : The state we would like to transit to.
 *  Implicit Inputs:
 *   shmPtr->mgtParams.state - current state/context of service.
 *   DTSSserviceState - state array.
 *  Value Returned:
 *   True - there is a valid state change to the desired next state.
 *   False- an illegal state change was prescribed.
 *  Side Effects:
 *   shmPtr->mgtParams.TSstate may be modified.
 *--
 */
unsigned long StateChange(int nextState)
{
/*
 *   Valid state changes are defined by the global state
 *   array : DTSSserviceState;
 *
 */

    if (DTSSserviceState[shmPtr->mgtParams.TSstate][nextState] )
    {
	DCE_SVC_DEBUG((dts_svc_handle, dts_s_states, svc_c_debug1,
	    "StateChange %d to %d\n", shmPtr->mgtParams.TSstate, nextState));
        shmPtr->mgtParams.TSstate = nextState;
 	return( TRUE );
    }
    return( FALSE );
}

/*
 *++
 *  InitUnix ()
 *      Forks a child process which becomes the dtss deamon.  Sets the
 *      effective, real uid to root, disassociates terminal, sets new
 *      group.
 *  Inputs:
 *	argv,argc - command line args to process.
 *      parent_child_pipe - a file descriptor set for use with pipe,
 *                          sets a pipe between the parent and child
 *                          so that child will notify parent when
 *                          it has initialized.
 *  Side Effects:
 *	BugChecks on Errors.
 *--
 */
static VOID InitUnix(
 int argc, 
 char *argv[], 
 int  parent_child_pipe[],
 dts_dtsd_cmdargs_t * argptr
 )
{
    int i;
    int  pid, fd;
#ifdef _BSD 
    union wait e_status ;
#else
    int  e_status;              /* status of child */
#endif

    char dummy_buffer;
    error_status_t st;
    const char * const courier_role_p = "courier";
    const char * const noncourier_role_p = "noncourier";
    int local_debug_mode = FALSE;
    char * option_flags = "fgdsk:cow:" ;
    boolean32 read_attr_file = FALSE ;

    /* begin */
char debug_mode;            /* unix debug flag on? */

    parent_child_pipe[0] = parent_child_pipe[1] = -1;


    /* look for debug switch, and don't background if it is there */

    while ((i = getopt(argc, argv, option_flags)) != EOF)
	switch (i) {

	default:
	    dts_dtsd__usage_error() ;
	    exit(1);
	    /* NOTREACHED */
	    break;

	case 'd':		/* debugging  */
	    local_debug_mode = TRUE;
	    break;

	case 'g':		/* global server  */
	    argptr->global = TRUE ;
	    break ;
	    
	case 's':		/* server  */
	    shmPtr->mgtParams.variant = VTserver;
	    argptr->server = TRUE ;
	    if (argptr->client)
		dts_dtsd__usage_error() ;
	    break;

	case 'w':		/* serviceability  */
	    dce_svc_routing((unsigned char *)optarg, &st);
	    if (st != svc_s_ok)
	    {
		dce_error_string_t tempString ;
		dce_svc_printf( DTS_S_ROUTING_SWITCH_MSG, 
			       dce_dts_error_text( st, tempString));
	    }
	    break;

	case 'c':		/* client */
	    argptr->client = TRUE ;
	    if (argptr->server)
		dts_dtsd__usage_error();
	    break;

	case 'k':		/* courier */
	    if (argptr->client)	/* can't be a client and a courier */
		dts_dtsd__usage_error() ;

	    if (!strcmp(optarg, courier_role_p))
		argptr->couriertype = K_ROLE_COURIER;
	    else if (!strcmp(optarg, noncourier_role_p))
		argptr->couriertype = K_ROLE_NONCOURIER ;
	    else
		dts_dtsd__usage_error(); /* wrong argument  */
	    break;

        case 'o':
	    argptr->clockset = TRUE ;
	    break ;

	case 'f':
	    read_attr_file = TRUE;
	    break;
	}

    /*
     * if global asserted, then must be a server and not a client 
     */

    if (argptr->global && ( (! argptr->server) || argptr->client))
	dts_dtsd__usage_error() ; 

    DTS_LOCK_GLOBAL ;
    if (argptr->server) 
	DDGD.running_as_server = TRUE ;
	
    
    if (dce_server_inq_uuids( &DDGD.conf_id, 
			      &DDGD.exec_id,
			     &st), st != rpc_s_ok)
	DDGD.started_from_dced = FALSE ;
    else
	DDGD.started_from_dced = TRUE ;

    if (read_attr_file)
	DDGD.read_attrs_from_file = TRUE ;

    if (local_debug_mode)
	DDGD.debug_mode = TRUE ;
    DTS_UNLOCK_GLOBAL ;

    if (local_debug_mode != TRUE) 
    {
	
	/* Change to a known directory so if/when we dump core... */
	char homedir[K_MAX_FILENAME+1];
	boolean32 dced_started_me = FALSE ;

	strcpy(homedir, dcelocal_path);
	strcat(homedir, K_DTS_HOME_DIR);
	(void)chdir(homedir);

	DTS_LOCK_GLOBAL ;
	dced_started_me = DDGD.started_from_dced;
	DTS_UNLOCK_GLOBAL;

	if (! dced_started_me) 
	{
	     
	    if ((pipe(parent_child_pipe)) != 0) {
		perror("dtsd unable to establish parent/child pipe");
		BugCheckOSstatus(errno);
	    }

	    if ((pid = fork()) < 0) {
		perror("dtsd unable to fork child process");
		BugCheckOSstatus(errno);
	    }

	    if (pid != 0)		/* parent */
	    {
		/* We are in the parent.  Close the write fd of the pipe
		 * and wait for the child to notify us it has detatched.
		 * N.B. the read will block until the child either writes 
		 * to the pipe or closes it.
		 */
		(void)close(parent_child_pipe[ WRITE_SIDE_OF_PIPE ]);
		read(parent_child_pipe[ READ_SIDE_OF_PIPE], 
		     &dummy_buffer, sizeof(dummy_buffer));


		/*
		 * Will Child report back any status to us?
		 */
		if (waitpid(pid, (int *)&e_status, WNOHANG) != 0)
		{
		    /*
		     * Child process returned status.
		     * Child is responsible for printing its own error message
		     */
#ifdef _POSIX_SOURCE
		    _exit(WEXITSTATUS(e_status));
#else
		    _exit(e_status);
#endif
		}
		else		
		{
		    /* no status in child - presume all is well */
		    _exit(0);
		}
	    } /* parent process */
	    
	    /* child closes read fd */

	    (void)close(parent_child_pipe[ READ_SIDE_OF_PIPE ]); 

	} /* dont_fork */
	
	#ifndef HPDCE_FIX_OT6076
	    if ((fd = open("/dev/null", O_RDWR,
		   S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)) != -1)
	    {
	       (void)dup2(fd,0);
	       (void)dup2(fd,1);
	       (void)dup2(fd,2);
	       (void)close(fd);
	    }
	#endif  /* HPDCE_FIX_OT6076 */

	SetIDs();		/* child sets ids        */
	
    }
    return;
}


/*
 *++
 *  SetIDs()
 *     Set the group, user id of the process, detach from terminal.
 *  Side Effects:
 *      sets process actual and effective id.
 *	BugChecks on Errors.
 *--
 */
static VOID SetIDs(void)
{
#if defined (SNI_SVR4_POSIX) || defined(_POSIX_SOURCE) || defined(HPUX)

	/*
	 * Note:  in a POSIX environment, all uid's (real,
	 * effective, saved) have to be changed as a unit,
	 * using setuid(2).  We assume that this daemon is
	 * being run as root already, and that setuid() was
	 * used to change all uid's to 0 in parallel.  No
	 * need, therefore, to use setreuid(2) or other
	 * non-POSIX functions.
	 */

#else	/* defined (SNI_SVR4_POSIX) || defined(_POSIX_SOURCE) */

	/*
	 * Try to ensure both real and effective uid's are zero
	 */
	if (setreuid(0, 0) < 0) {
	    perror("dtsd: setreuid");
	}


# ifdef	TIOCNOTTY
	{
	    int  c;

	    if ( (c = open("/dev/tty", O_RDWR, 0)) >= 0) {
		    ioctl(c, TIOCNOTTY, (char *)NULL);
		    close(c);
	    }
	}
# endif

#endif	/* defined (SNI_SVR4_POSIX) || defined(_POSIX_SOURCE) */

	if (setsid() == -1)
	    perror("dtsd: setsid");
	return ;
}


/*
 *++
 *  DetachUnix()
 *     Causes the parent process to exit by closing the pipe which
 *     the child and parent are maintaining.
 *  Side Effects:
 *      Causes the parent process to exit with success.
 *--
 */
static VOID DetachUnix(int parent_child_pipe[2])
{

        /*
	 * Close the pipe, notify our parent that we are
	 * done.
	 */
        (void)close(parent_child_pipe[ WRITE_SIDE_OF_PIPE]);
	return;
}

static VOID dts_dtsd__usage_error(void) 
{
    /* begin */
    dce_svc_printf( DTS_S_USAGE_ERROR_MSG);
    exit(1);
}

/*
 * dts_dtsd__initialize_from_args( dts_dtsd_cmdargs_t * )
 * 
 * issue the appropriate set of startup commands based on 
 * command line arguments.
 *
 */
static StatusBlk dts_dtsd__initialize_from_args( 
    const dts_dtsd_cmdargs_t * const cmdargp
) 
{
    unsigned32 clock_set = 0 ;
    StatusBlk status_Blk = { DTSS_SUCCESS, rpc_s_ok };
    unsigned long retval = 0 ;
    unsigned32 value = 0 ;
    extern StatusBlk NCLtoDCEerror( unsigned long) ;
    struct TransBuf the_value ;

    /* begin */

    if (cmdargp -> server)
        value = K_VARIANT_SERVER ;
    else
        value = K_VARIANT_CLERK ;

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
	   "initialize_from_args: Creating Server\n"));

    retval = DirectiveCreate( 0UL, value, &status_Blk);
    if (retval != K_NCL_RSP_SYSTEM_ERROR)
	status_Blk = NCLtoDCEerror(retval);

    if (IsError( status_Blk))
	goto ErrorReturn ;

    if (cmdargp -> server) 
    {
	/*
	 * set courier role 
	 */

	if (cmdargp -> couriertype != K_ROLE_BACKUPCOURIER) 
	{
	    
	    long attribute_type = K_COURIER_ROLE_ATTR;
	    the_value.transbuf_w_size = 4 ;
	    the_value.transbuf_w_type = K_TYPE_INTEGER32 ;

	    (void)memcpy((void *)&the_value.transbuf_t_data, 
		   (void *)&cmdargp->couriertype, 
		   sizeof (cmdargp -> couriertype));

	    
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
			   "initialize_from_args: Set courier"));

	    retval = DirectiveSet( 0UL, 
				  attribute_type,
				  &the_value,
				  (int)0,
				  &status_Blk);
	    if (retval != K_NCL_RSP_SYSTEM_ERROR)
		status_Blk = NCLtoDCEerror(retval);
	    if (IsError( status_Blk ))
		goto ErrorReturn ;
	}

	/*
	 * if we're a global server, 
	 * then it "pays to advertise"
	 */

	if ( cmdargp -> global)	/* global or local server */
	{
	    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
			   "initialize_from_args: Advertise\n"));

	    retval = DirectiveAdvertise( 0UL, (void *)0, 0UL, &status_Blk);
	    if (retval != K_NCL_RSP_SYSTEM_ERROR)
		status_Blk = NCLtoDCEerror(retval);
	    if (IsError( status_Blk ))
		goto ErrorReturn ;

	}

    } /* if we should run as a server  */

    /* 
     * always enable ourselves 
     */

    DCE_SVC_DEBUG((dts_svc_handle, dts_s_threads, svc_c_debug1,
		   "initialize_from_args: Enable\n"));

    if (cmdargp -> clockset) 
	clock_set = 1 ;
    retval = DirectiveEnable( 0UL, clock_set, &status_Blk);
    if (retval != K_NCL_RSP_SYSTEM_ERROR)
	status_Blk = NCLtoDCEerror(retval);
    if (IsError( status_Blk ))
	goto ErrorReturn ;

 ErrorReturn:
    ;
    return status_Blk;
}

/*
 * dts_dtsd_dtsdate - retrieve remote time from server, 
 * set local clock if its isn't within range 
 */

static int dts_dtsd_dtsdate( 
  int ac, 
  char * av[]
)
{
    
    char * remhostname = 0 ;	/* remote host name or addr  */
    unsigned long windowsecs  = 0UL; /* seconds difference we'll tolerate */

    utc_t remote_time, time_now, result ;
    reltimespec_t rtime_spec;

    int utcflag = 0;
    int tcpflag = 0 ;
    int queryflag = 0 ;
    int silentflag = 0 ;
    int localflag = 1 ;
    int ch ;
    error_status_t status ;
    char tbuff[ 128 ] ;
    
    /* begin */

    if (ac == 1) 
    {
	if(utc_gettime( &time_now) < 0) 
	   exit(1);
	if(utc_asclocaltime(tbuff, 128, &time_now) < 0)
	   exit(1);
	printf("%s\n", tbuff);
	exit(0);
    }
	
    while((ch = getopt( ac, av, ":uqst")) != -1) 
    {
	switch(ch)
	{
	case 'u':
	    utcflag++;
	    localflag = 0;
	    break;
	    
	case 's':
	    silentflag++;
	    break;

	case 't':
	    tcpflag++;
	    localflag = 0;
	    break;

	case 'q':
	    queryflag++;
	    break;

	default:
	    dts_dtsdate_usage_error();
	    break;
	}
    }
    

    if (optind == ac) 
	dts_dtsdate_usage_error() ;

    remhostname = av[optind++] ;
    if (optind < ac ) 
    {
#ifndef __hpux_CHFts15503
        char * str = 0;
	char * p;
#else
	char * str, * p ;
#endif
	
	windowsecs = strtoul( av[optind], &p, 10) ;
	if ( windowsecs == 0 && p == str) 
	    dts_dtsdate_usage_error();	/* not a number */
	else if (*p != 0)	/* bad character */
	    dts_dtsdate_usage_error() ;
    }

    if ( ! tcpflag ) {	
        dts_dtsd_get_remote_time( remhostname, &remote_time, &time_now, &status) ;
        if (status != rpc_s_ok) 
	    return 1 ;
    }
    else {
        if ( utc_gettime( &time_now) < 0 )
            exit (1); 	
    }

    if (tcpflag)
    {
       if ((utc_getTCPtime(remhostname, &utc)) < 0 )
	   exit (1);
       remote_time = utc;
    }

    if ( !silentflag )  
    {
	static char timebuffer[ UTC_MAX_STR_LEN + 5];
	
	if (utcflag) 
	{
	    utc_ascgmtime(timebuffer, UTC_MAX_STR_LEN - 1, &remote_time) ;
	}
	if (tcpflag)
	{
           utc_ascgmtime(timebuffer, UTC_MAX_STR_LEN, &utc);
	}
        if (localflag)
	{
            utc_asclocaltime(timebuffer, UTC_MAX_STR_LEN - 1, &remote_time) ;
        }	
	printf("%s\n", timebuffer) ;
    }	


    /*
     * compute absolute value of the timestamps. If they differ 
     * by more than the interval given, slam the value of the 
     * local host to the value retrieved from the remote host 
     */
    if (windowsecs != 0) 
    {
	enum utc_cmptype relation ;
	long ldiff = 0 ;

	/*
	 * compute time difference between this machine and 
	 * the remote system
	 */
	
	/* result = remote_time - time_now */
	/* result will be a _relative_ binary timestamp,
	   since both of the other times are absolute */

	if (utc_cmpintervaltime( &relation, &remote_time, &time_now))
	    return 0 ;

	if(utc_subtime( &result, &remote_time, &time_now))
	    return 0 ;

	if (utc_binreltime( &rtime_spec, (timespec_t *)0, &result))
	    return 0 ;
	
	if ((ldiff = labs(rtime_spec.tv_sec)) > windowsecs) 
	{
	    time_t theRemoteTime = 0L ;
	    timespec_t time_spec; 

	    if (queryflag) 
		exit(2) ;	
	    
	    if ( ! dce_priv_proc_is_appropriate())
	    {

		dce_svc_printf(DTS_S_SUPERUSER_MSG) ;
		return 1 ;
	    }

	    /*
	     * convert remote time to time_spec, and 
	     * use this value to set the clock 
	     */
	    
	    if (utc_bintime( &time_spec, (timespec_t *)0, (long *)0, 
			    &remote_time))
		return 1 ;
	    theRemoteTime = time_spec.tv_sec ;
	    (void)stime( &theRemoteTime ) ;
	    return 0;
	}
    }

    return 0;
}

static void dts_dtsdate_usage_error(void)
{
    dce_svc_printf(DTS_S_DTSDATE_USAGE_MSG) ;
    exit(1);
}


/*
 * dts_dtsd_get_remote_time 
 * given a remote host name or IP address, attempt to 
 * contact the dtsd at that host, and retrieve the current time from 
 * that system
 *
 * Algorithm: build a string binding for each allowed protocol sequence,
 * and try the call. Cycle thru each possible protocol sequence (our host 
 * may only understand a subset of the possible protocol sequences 
 */

static void dts_dtsd_get_remote_time( 
 char * remhostname,
 utc_t *remote_time_p, 
 utc_t *time_now_p,
 error_status_t * statusp
)
{
    unsigned char * the_string_binding_p = 0 ;
    rpc_binding_handle_t binding_handle ;
    rpc_protseq_vector_t * protseq_vector_p = 0 ;
    int i = 0 ;
    rpc_if_handle_t       reqInterface;
    rpc_if_id_t           reqInterfaceID;
    unsigned_char_t       * string_uuid = 0 ;
    error_status_t localstatus = rpc_s_ok ;
  
    /* begin */

    *statusp = rpc_s_ok ;
    
    if(rpc_network_inq_protseqs( &protseq_vector_p, statusp),
       *statusp != rpc_s_ok)
    {
	goto Cleanup_Vector ;
    }
    
    reqInterface = time_service_v1_0_c_ifspec ;
    rpc_if_inq_id (reqInterface, &reqInterfaceID, &localstatus);
    if (localstatus != rpc_s_ok)
    {

	dce_error_string_t tempstring ;
	
	dce_svc_printf(DTS_S_INTERFACE_ID_MSG, 
		       dce_dts_error_text( localstatus, tempstring));
	*statusp = localstatus ;
	return ;
    }

   if(uuid_to_string( &reqInterfaceID.uuid, &string_uuid, &localstatus), 
      localstatus != rpc_s_ok) 
   {
       dce_error_string_t tempstring ;
       dce_svc_printf(DTS_S_UUID_TO_STRING_MSG, 
		      dce_dts_error_text( localstatus, tempstring));
       *statusp = localstatus ;
       return;
   }
    
    for(i = 0 ; i < protseq_vector_p->count; i++)
    {
	
	rpc_string_binding_compose( 
		   (unsigned_char_t *)string_uuid, /* object ID */
		   protseq_vector_p->protseq[i], /* prot seq */
		   (unsigned_char_t *)remhostname, /* host */
		   (unsigned_char_t *)0, /* endpoint */
		   (unsigned_char_t *)0, /* no options */
		   &the_string_binding_p, /* our string binding */
		   statusp);
	if (*statusp != rpc_s_ok) 
	    break;
	
	if(rpc_binding_from_string_binding( the_string_binding_p,
					&binding_handle,
					statusp), *statusp != rpc_s_ok)
	    break;

	if (rpc_ep_resolve_binding( binding_handle, 
				   time_service_v1_0_c_ifspec, 
				   statusp), *statusp != rpc_s_ok)
	{
	    dce_error_string_t tempstring;

	    dce_svc_printf(DTS_S_EP_RESOLVE_BINDING_MSG, 
			   remhostname, 
			   protseq_vector_p->protseq[i], 
			   dce_dts_error_text(*statusp, tempstring)); 
	    continue;
	}
	
	/*
	 * make the DTS Management call here to get the current time 
	 */
	
	{			/* scope */

	    idl_ulong_int processing_delay = 0 ; /* ignored */

	    if(utc_gettime( time_now_p))
	    {
		*statusp = rpc_s_invalid_arg ; /* say it's anything */
		break ;
	    }
	    ClerkRequestTime( binding_handle, remote_time_p, 
			     &processing_delay, statusp) ;

	    if (*statusp == rpc_s_ok)
		break;
	    else 
	    {
		dce_error_string_t tempstring;
		dce_svc_printf(DTS_S_CLERK_REQUEST_MSG, 
			       dce_dts_error_text(*statusp, tempstring));
	    }
	} /* scope */
	
    }
    
    if (the_string_binding_p)
	rpc_string_free( &the_string_binding_p, &localstatus) ;

    if (string_uuid) 
	rpc_string_free( &string_uuid, &localstatus);

 Cleanup_Vector:
    ;
    if (protseq_vector_p) 
	rpc_protseq_vector_free( &protseq_vector_p, &localstatus) ;
    return ;
}				
    
char * dce_dts_error_text( error_status_t st, dce_error_string_t msgtext)
{
    error_status_t lst = error_status_ok ;
    
    sprintf((char *)msgtext, "(%lu)", st) ;
    dce_error_inq_text(st, msgtext, (int *)&lst);
    return (char *)msgtext;
}

    
void dts_dtsd_init_mutex( void )
{
    /* N.B. Any failure here is a fatal error */

    if(pthread_mutexattr_create( 
				&DDGD.dtsd_global_mutexattr))
    {
	dce_svc_printf( DTS_S_MUTEXATTR_CREATE_MSG) ;
	exit(1);
    }

    if(pthread_mutexattr_setkind_np( 
         &DDGD.dtsd_global_mutexattr, MUTEX_FAST_NP))
    {
	dce_svc_printf( DTS_S_MUTEXATTR_SETKIND_MSG ) ;
	exit(1);
    }

    if(pthread_mutex_init( &DDGD.dtsd_global_mutex, 
			  DDGD.dtsd_global_mutexattr))
    {
	dce_svc_printf( DTS_S_MUTEX_INIT_MSG ) ;
	exit(1);
    }
}
