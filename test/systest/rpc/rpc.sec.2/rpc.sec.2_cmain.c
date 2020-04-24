/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc.sec.2_cmain.c,v $
 * Revision 1.1.16.2  1996/02/17  23:28:06  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:15  marty]
 *
 * Revision 1.1.16.1  1995/12/11  22:43:46  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/10/26  19:46 UTC  bissen
 * 	merge from HPDCE01
 * 	[1995/12/11  22:20:34  root]
 * 
 * Revision 1.1.13.1  1993/12/22  20:47:59  mhickey
 * 	no changes, different backing tree
 * 	[1993/12/22  20:44:28  mhickey]
 * 
 * 	no
 * 
 * 	A number of fixes while fixing 7743
 * 	 - Cleaned up the includes/defines and thread handling
 * 	 - Passes the "client" arg to sighandler
 * 	 - Streamlined checking and conversion of command config file numeric values
 * 	 - Added call to get the server principal name to make intercell work
 * 	 - Fixed some printfs and comments
 * 	 - Added calls to check_stat to get application errors tracked.
 * 
 * 	defect 7743
 * 	[1993/12/22  18:32:39  mhickey]
 * 
 * Revision 1.1.11.3  1993/10/20  19:36:58  ohara
 * 	svr4 ref port
 * 	[1993/10/20  19:36:08  ohara]
 * 
 * Revision 1.1.11.2  1993/09/28  13:16:55  cmckeen
 * 	Fixed syntax errors.
 * 	[1993/09/28  13:16:33  cmckeen]
 * 
 * Revision 1.1.4.2  1993/08/25  20:09:51  bissen
 * 	Changes for TET
 * 	[1993/08/25  20:09:02  bissen]
 * 
 * Revision 1.1.2.9  1993/03/12  04:06:57  mhickey
 * 	Final checkin for comment fixes before final submit
 * 	[1993/03/12  04:00:02  mhickey]
 * 
 * 	Updating comments to agree with current state of the world.
 * 	[1993/03/10  21:53:23  mhickey]
 * 
 * Revision 1.1.2.8  1993/02/25  20:16:02  mhickey
 * 	Removed ifdef for RPCSEC2_STAT_DEBUG since this has benn fixed
 * 	elsewhere.
 * 	[1993/02/25  17:19:57  mhickey]
 * 
 * 	Added confirmation of id expiration and setting of comm status variable.
 * 	[1993/02/19  21:42:28  mhickey]
 * 
 * 	Fix for validate prob?
 * 	[1993/01/23  18:42:17  mhickey]
 * 
 * Revision 1.1.2.6  1993/01/22  20:54:37  mhickey
 * 	Fixed a typo in rpcsec2_kill_threads output that was
 * 	screwing up the report generator.
 * 	[1993/01/20  18:49:43  mhickey]
 * 
 * 	Fixed overloading of start_time variable that was
 * 	causing miscalculation of end time, abortively
 * 	terminating CHO runs.
 * 	[1993/01/20  17:08:54  mhickey]
 * 
 * 	fixed some logic errors that were causing too many reports
 * 	to be generated, and attempts to kill the report thread when
 * 	it was not active.
 * 	[1993/01/19  20:36:58  mhickey]
 * 
 * 	Found out that refresh of an expired id is not allowed,
 * 	and since it is possible for a client ticket to time
 * 	out while waiting for the creation of the sync file,
 * 	it was necessary to switch to the use of a thread
 * 	to maintain the id.
 * 
 * 	Switching to the threaded model for id refreshes meant
 * 	that I could no longer use a signal handler catching
 * 	SIGVTALRM to time the reports since this is caught
 * 	internally for CMA scheduling.  So I added a thread to
 * 	generate the reports at the specified intervals.
 * 
 * 	However, I still wanted a way to get a final report if
 * 	the client was killed via cntl-c, so I added another
 * 	thread (thanks to Tom Jordahl for the basic
 * 	sigprocmask/sigwait code) to catch signals like
 * 	SIGINT, SIGQUIT, SIGHUP, etc, and print a report
 * 	before exiting.
 * 
 * 	Having all these threads around with the possibility
 * 	of an exit becoming necessary from one of three threads
 * 	necessitated the addition of a routine rpcsec2_kill_threads
 * 	that kills all of the threads but the caller (which is
 * 	assumed to exit on it's own after the call returns). If
 * 	this is not done, the uncancelled threads run on and on
 * 	after the initial thread exits.
 * 
 * 	Removed the include of timers.h, will use the timespec
 * 	def in cma_px.h on a RIOS.
 * 
 * 	Removed all use of signals for timing reports.  This was
 * 	necessary
 * 	[1993/01/19  18:25:43  mhickey]
 * 
 * Revision 1.1.2.5  1993/01/14  20:09:30  mhickey
 * 	Fixed a type mismatch
 * 	[1993/01/14  20:08:42  mhickey]
 * 
 * Revision 1.1.2.4  1993/01/13  18:18:09  mhickey
 * 	Fixed the setting of start_time to get accurate run duration
 * 	report.
 * 
 * 	Changed all sleeps to pthread_delay_nps because the
 * 	sleeps, not being wrapped on RIOS or PMAX (or anything
 * 	else except for SVR4 for that matter) were causing all
 * 	the threads in the process to block for between 3 and
 * 	6 minutes.  This had the symptom of causing clients
 * 	that were syncing to fail if they were using a connection
 * 	binding because the rpc threads that ping-pong "I'm OK
 * 	You're okay" messages with the various servers were being
 * 	blocked long enough to time out.  So by the time I got
 * 	around to using the binding, it was useless. (Thanks
 * 	to Herr Doktor Weisman for this one :-)).
 * 	[1993/01/13  18:15:39  mhickey]
 * 
 * 	Removed garbage from a comment.
 * 	[1993/01/06  20:40:35  mhickey]
 * 
 * 	A few fixup things:
 * 	  Reports now print RUNNING for regular reports, as well
 * 	  as the STARTING, FINISHED, STOPPED, DIED messages previously
 * 	  printed.
 * 
 * 	  Fixed some typo's in the report output.
 * 
 * 	  Added a call to time before we exit the call loop due
 * 	  to making the specified number of calls.  This makes the
 * 	  client correctly report the runtime.
 * 
 * 	  added a setbuf 0 call to get output out sooner.
 * 
 * 	  verified builds with gcc -pedantic
 * 	[1993/01/06  20:27:53  mhickey]
 * 
 * 	Fixed signal setup for RIOS and fixed a warning in assignment.
 * 	[1992/12/08  18:14:50  mhickey]
 * 
 * Revision 1.1.4.2  1993/01/07  15:41:11  mhickey
 * 	added security key dumping in debug mode
 * 
 * Revision 1.1.2.3  1992/12/03  19:53:53  mhickey
 * 	 To clarify the rest of the history messages:
 * 	    This program started as a 'get-your-feet-wet'
 * 	    task called testrpc.  It matured into the rpc.sec.2
 * 	    system test, and was thusly renamed.  Major amounts
 * 	    of functionality were added between 1.1.1.4 and 1.1.1.5.
 * 	    These were (because I was in a rush) glossed over in the
 * 	    comment associated with 1.1.1.5.  Basically the following
 * 	    were added:
 * 	       1) support for using the configuration file reading
 * 	          routine rpcsec2_rd_conf() was added.
 * 	       2) support for using the error tracking and reporting
 * 	          functions of check_stat were added.
 * 	       3) all non-DCE return value description strings were
 * 	          abstracted out to the rpc.sec.2_err_str.c file
 * 	       4) fault based id refreshes were added (ie: "darn I failed,
 * 	          oh, look, it was because my id is old, I'll refresh semantics)
 * 	       5) the ablility to run for X time or X passes.
 * 	       6) reporting routine
 * 	       7) signal (timer or interrupt signal) based reporting.
 * 	       8) switchable timing of RPC's and tracking of min, max,
 * 	          total elapsed times.
 * 	       9) the ablility to assume it's own identity
 * 	      10) ability to select a specified binding type and
 * 	          verify connectivity.
 * 	      11) syncronized RPC call start based on filesystem semaphore
 * 	          (read: start RPC's only after this file exists :-) )
 * 	      12) boundary mode operation - one call and out
 * 	      13) -l option to set protection level for RPC's
 * 
 * 	  This file was then moved from src/test/systest/rpc to
 * 	  src/test/systest/rpc/rpc.sec.2.
 * 
 * 	 Defect 5887.
 * 	[1992/12/03  18:22:47  mhickey]
 * 
 * 	Completed coding including boundary mode, id refreshes, reporting, etc.
 * 	[1992/11/19  21:48:28  mhickey]
 * 
 * 	Added the call to rpc_binding_set_auth_info to indicate authenticated
 * 	RPC, and added code to handle authentication errors from the server.
 * 	[1992/10/16  13:29:05  mhickey]
 * 
 * 	Added code to allocate and initialize with checksums a
 * 	conformant struct for a call to tstrpc_cnf_str().  Added
 * 	the calls to tstrpc_cnf_str().
 * 
 * 	Also abstracted return value checking into a
 * 	routine tstrpc_check_ret().
 * 	[1992/10/10  03:11:27  mhickey]
 * 
 * 	Initial checkin for base for rpc.security.2
 * 	[1992/10/06  17:29:20  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* rpc.sec.2_cmain.c - client side of rpc.sec.2 test
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the rpc.sec.2 DCE System Test
 *---*
 *---*    The rpc.sec.2 system test corresponds to test rpc.sec.2 in the 
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the rpc.sec.2 system test, see the test 
 *---*    plan, or the README file in this directory.
 *---* 
 *---* FUNCTION
 *---*    Performs fully authenticated RPC's to a server, passing a 
 *---*    struct containing a conformant array.  Operates in two modes:
 *---*      1. Boundary mode (specified by the -b command line option)
 *---*           In this mode the client is part of a group of clients that
 *---*           are to simultaneously access the server in an effort to 
 *---*           determine the maximum numeber of concurrent calls the server
 *---*           can handle.  This being the case, the client attempts only 
 *---*           one RPC, the execution of which it attempts to syncronize 
 *---*           with the other clients in the test.  In this mode, the test 
 *---*           does all of the setup for the RPC, then polls the filesystem 
 *---*           at 5 second intervals for the existence of a file (specified 
 *---*           on the command line via the -S option).  This file 
 *---*           (referred to as the sync file) is assumed to have been 
 *---*           created by the last client started as part of the boundary
 *---*           test.  When the file becomes available the client goes to 
 *---*           sleep for at least 5 minutes (actually 10 minutes - the 
 *---*           number of seconds since the last 5 minute boundary).  This
 *---*           wait is intended to allow all the (possibly remote) clients
 *---*           involved in the test to notice the existence of the file and 
 *---*           schedule their RPC for the same time.  When the client 
 *---*           awakens, the RPC is immediately executed, a report line is 
 *---*           printed, and the client exits with one of the following
 *---*           exit statuses:
 *---*              0 - no errors
 *---*              1 - comm error on call and protocol was UPD, or
 *---*                  rpc_s_server_too_busy and protocol was CN.
 *---*              2 - some other error.
 *---* 
 *---*           Note that running in this mode assumes that one of the 
 *---*           following is true:
 *---*              1. either the sync file is written to a directory mounted
 *---*                 on the same place on all the machines where clients 
 *---*                 involved in the test are running; or
 *---*              2. the process that calls this script will notice the 
 *---*                 existence of the file and propogate it to the same
 *---*                 place on all of the systems where clients are running.
 *---* 
 *---*       2. Stress mode (default mode of operation)
 *---*           In this mode the client makes RPC's to the server either until
 *---*           the interval specified (= number of hours specified by the -h
 *---*           option + number of minutes specified by the -m opion, or the
 *---*           interval RPCSEC2_CLI_DEF_RUN_INTVL specified in the config
 *---*           file) has elapsed, or until the number of RPC's specified by 
 *---*           the -r option have been made.  The -i option specifies the 
 *---*           interval at which one line report lines are to be printed on
 *---*           standard output.  If the -p option is specified, the value 
 *---*           specified by the -i option is taken to be the number of calls
 *---*           to make between each report generation.  If the -h and/or -m
 *---*           options are specified, then the value specified by the -i
 *---*           option is taken to be the interval in minutes between each 
 *---*           report generation.  If the -h or -m option is specified and 
 *---*           the -i option is not specified, the report interval is the 
 *---*           value assigned in the config file to the variable 
 *---*           RPCSEC2_CLI_DEF_REP_INTVL.
 *---*
 *---*    Note that the defaults for the program are to run for
 *---*    RPCSEC2_DEF_RUN_INTVL hours, generating report lines every 
 *---*    RPCSEC2_DEF_REP_INTVL minutes.
 *---*
 *---*    The following is a list of the steps in the program:
 *---*
 *---*    1. Gets values for operational parameters by reading the config 
 *---*       file (rpc.sec.2.conf or passed via the -f command line option).
 *---*    2. Assumes the client principal identity specified in the config 
 *---*       file (rpc.sec.2.conf or passed via the -f command line option).
 *---*    3. Sets various strings for reporting, such as hostname and operating
 *---*       system.
 *---*    4. Looks in the CDS namespace for an existing entry for the rpc.sec.2
 *---*       interface (defined in the config file).  If a CDS namespace
 *---*       entry is found, then a binding handle is obtained for the 
 *---*       interface, and an rpc_mgmt_is_server_listening call is made to
 *---*       verify that the binding is usable. 
 *---*    5. The binding handle is converted to a string binding and then
 *---*       parsed to find the protocal type.
 *---*    6. The handle is annotated to indicate the protection level specified
 *---*       by the RPCSEC2_PROT_LEVEL variable in the config file, or by the
 *---*       -l command line option.
 *---*    7. The process looks for the sync file and sleeps after finding it
 *---*       to syncronize startup of it's RPC's.
 *---*    8. The client makes the RPC.  If the -b flag is specified, then
 *---*       the client checks the return status from the call and exits with
 *---*       the appropriate value as described above.  If the client is 
 *---*       running in stress mode, then the status is checked, counts of 
 *---*       successes, failures, total calls and, if the -t option is 
 *---*       specified, call times are updated, and the next RPC is made.
 *---* 
 *---* CALLED BY
 *---*    rpc.sec.2_runtest.sh - (the driver for running the test program)
 *---* 
 *---* CALLS INTO 
 *---*    Reads rpc.sec.2.conf or file specified via the -f option for 
 *---*    definitions.
 *---*    rpc.sec.2_chk_stat.c  (rpcsec2_check_stat() )
 *---*    rpc.sec.2_defs.h      (return values and data structures)
 *---*    DCE APIS
 *---*
 *---* PREREQUISITES
 *---*    Assumes a running DCE cell on the system that has been setup with 
 *---*    the script rpc.sec.2_setup.sh.
 *---*/

/*-------------------------------------------*
 *---*  Includes
 *-------------------------------------------*/
/*---*
 *---* Global include files
 *---*/
#include <errno.h>
#include <stdio.h>
#include <sys/utsname.h>
#include <sys/time.h>

#include <sys/stat.h>

/*---* allow conditional compilation of threads stuff for bug invest *---*/
#if !defined(RPCSEC2_NO_CLI_THDS)
#   include <pthread.h>
#else /*---* defined(RPCSEC2_NO_CLI_THDS) *---*/
#   include <sys/signal.h>
#endif /*---* !defined(RPCSEC2_NO_CLI_THDS) *---*/

#include <dce/dce_error.h>
#include <dce/sec_login.h>
#include <dce/id_base.h>
#include <ctype.h>
#include <stdlib.h>
#include <fcntl.h>
#if defined(__hpux)  /* added as hpux has no mode.h - ewh/hp */
#  include <sys/inode.h>
#else
#  include <sys/mode.h>
#endif  /* hpux source machine true */

/*---------------------------*
 *---* Private include files
 *---------------------------*/
#include <rpc_sec_2.h>
#include <rpc.sec.2_defs.h>
#include <rpctest.h>         /* added for TIMEOFDAY req - ewh/hp */

#ifdef SNI_SVR4
#define getclock(command, address)  gettimeofday(address)
#endif /* SNI_SVR4 */


/*-------------------------------------------*
 *---* Global Declarations
 *-------------------------------------------*/
/*--------------------------------*
 *---* Command line option flags 
 *--------------------------------*/
short nflag,   /*---* use nil uuid                                     *---*/
      bflag,   /*---* boundary mode                                    *---*/
      Nflag,   /*---* use NULL for UUID                                *---*/
      nflag,   /*---* make null calls (for rpc performace testing      *---*/
      aflag,   /*---* automatically retry other bindings if call fails *---*/
      debug,   /*---* take a guess!                                    *---*/
      Lflag,   /*---* indicates we are the last client started         *---*/
      sflag,   /*---* did we get a sequence number?                    *---*/
      tflag,   /*---* track and report on time of calls                *---*/
      uflag;   /*---* used passed string UUID                          *---*/



/*--------------------------*
 *---* Usage Message
 *--------------------------*/
char *usage = "usage: rpc.sec.2_cli [-S sync_file] -f config_file [-P datagram | connection] [-a] [-b] [-d] [-n] [-N | -u obj_uuid] [-s seq_string] [-v] [-i report_interval] [-L] [-l prot_lev] [[-p passes] | [-h hours_to_run] [-m minutes_to_run]] [-t]";



/*----------------------------------------------* 
 *---* Call statistics variables.  These are global since we can't pass 
 *---* stuff to the signal handler that needs them.
 *----------------------------------------------*/
/*---* Counters *---*/
int  calls,         /*---* total calls             *---*/
     id_refr_cnt,   /*---* number of times id refreshed       *---*/
     passed,        /*---* successes                          *---*/
     failed;        /*---* failures                           *---*/

/*---* timer variables *---*/
struct timespec  min_time,      /*---* minimum RPC time        *---*/
                 max_time,      /*---* maximum RPC time        *---*/
                 start_time,    /*---* time we made 1rst RPC     *---*/
                 precall_time,  /*---* time we made this RPC     *---*/
                 postcall_time, /*---* time we got back from RPC *---*/
                 elpsd_time;    /*---* total elapsed RPC time  *---*/


/*--------------------------------------*
 *---* error tracking array and counter 
 *---------------------------------------*/
err_stat_p_t err_stats[CHK_STAT_MAX_ERR];
int nerr_stats;



/*--------------------------------------*
 *---* Authorization/authentication variables - generally global for use of 
 *---* the identity maintenance function.
 *--------------------------------------*/
sec_login_handle_t login_ctxt;     /*---* dce login ctxt for auth rpc *---*/
void               *sec_key_p;     /*---* receives ptr to keydata from 
                                    *---* sec_key_mgmt_get_key 
                                    *---*/
char des_key[9];                   /*---* this is a gross hack to work around 
                                    *---* having to get the key each time we
                                    *---* want to refresh the identity (it is
                                    *---* destroyed by _validate_ to prevent
                                    *---* theft from VM).
                                    *---*/
long  id_lifetime;                 /*---* expiration interval      *---*/
time_t last_id_refr;               /*---* time of last id refresh  *---*/

/*---*
 *---* thread variables we need to syncronize name refreshes
 *---*/
pthread_cond_t refr_id_cond;
pthread_mutex_t refr_id_mutex;



/*----------------------------------------*
 *---* Thread management structs/defines 
 *----------------------------------------*/
/*---* defines for threads we spawn *---*/
#define RPCSEC2_MAIN_THD  0
#define RPCSEC2_REFR_THD  1
#define RPCSEC2_SIG_THD   2
#define RPCSEC2_REP_THD   3
#define RPCSEC2_CLI_NTHDS 4

/*---* array of threads we spawn *---*/
pthread_t thd_array[RPCSEC2_CLI_NTHDS];

/*---*
 *---* table of pointers to the threads and labels for them
 *---* we use the two global arrays to avoid calls to malloc in the 
 *---* code, and to allow compile time initialization and use of 
 *---* thread_tbl[i] == 0 meaning no thread.
 *---*/
thd_tbl_entry_t thread_tbl[] = { 
                                 {0, "Main thread"},
 			         {0, "Id Refresh Thread"},
    			         {0, "Signal Handler Thread"},
				 {0, "Reporting Thread"}
			       };

/*-----------------------------------------------*
 *---* thread variables we need to time reporting intervals 
 *-----------------------------------------------*/
pthread_cond_t rep_timer_cond;
pthread_mutex_t rep_timer_mutex;


/*---------------------------------------------------*
 *---* other variables used outside of main()
 *---------------------------------------------------*/
char *protocol;             /*---* what protocol is our binding       *---*/
time_t last_id_refr;        /*---* time of last id refresh            *---*/
int    start_delay,         /*---* time to wait after sync file found *---*/
       seq_numb;            /*---* identifies this client to driver   *---*/
struct utsname uname_vals;  /*---* OS type, hostname; etc.            *---*/




/*-----------------------------------------* 
 *---* Configuration variables/file management
 *-----------------------------------------*/
/*---* 
 *---* the array of parameter specs we need - passed to rd_conf()
 *---*/
param_spec_val_t params[] = {
                              {"RPCSEC2_CLI_PRINC_NAME", 0},
                              {"RPCSEC2_SRV_PRINC_NAME", 0},
                              {"RPCSEC2_SRV_CDS_NAME", 0},
                              {"RPCSEC2_CLI_KEYTAB_DIRPATH", 0},
                              {"RPCSEC2_CLI_KEYTAB_FN", 0},
                              {"RPCSEC2_CLI_DEF_RUN_INTVL", 0},
                              {"RPCSEC2_CLI_DEF_REP_INTVL", 0},
                              {"RPCSEC2_CLI_SYNC_DELAY", 0},
                              {"RPCSEC2_PROT_LEVEL", 0}
                            };

/*---*
 *---* enum for accessing the params array using macros in rpc.sec.2_defs.h
 *---* (actually the only one we use is PVAL()).
 *---*/
enum {
       cli_princ,
       srv_princ,
       cds_name,
       keytab_dir,
       keytab_fn,
       run_int,
       rep_int,
       delay,
       prot_lev
} param_i;


/*-------------------------------------------*
 *---* Function Prototypes 
 *-------------------------------------------*/
int rpcsec2_cli_check_ret(unsigned32 ret_stat, error_status_t comm_stat,
                  error_status_t fault_stat, short op_def);
void maintain_id(int *);
void rpcsec2_rep_gen(int);
void rep_timer(int *);
void rpcsec2_cli_timed_cnf_str_call(rpc_binding_handle_t bnd_hndle, 
               idl_short_int type, 
               long_conf_struct_t *conf_str_p, idl_long_int *stat_p, 
               error_status_t *fstat_p, 
               error_status_t *cstat_p);
void dump_key(sec_passwd_rec_t *);
struct timespec *rpcsec2_cli_syncstart(char *, int, struct timespec *);
int signal_catcher (char *);
void rpcsec2_kill_threads(pthread_t *);

/*-------------------------------------------------*
 *---* pointers to functions for external routines
 *-------------------------------------------------*/
/*---* report generation function *---*/
void (*rep_gen_func_p)() = rpcsec2_rep_gen;
/*---* thread cancel funtion      *---*/
void (*cleanup_func_p)() = rpcsec2_kill_threads;


/*-------------------------------------------*
 *---* Start of main program 
 *-------------------------------------------*/
main(int argc, char **argv)
{
   /*------------------------------------------------------*
    *---* declarations for everything but the kitchen sink
    *------------------------------------------------------*/
   unsigned32           protect_level,         /*---* protection level   *---*/
                        ret_stat,              /*---* generic status var *---*/
                        fault_stat = rpc_s_ok, /*---* server fault status *--*/
                        comm_stat = rpc_s_ok;  /*---* comm status var    *---*/
   signed32             exp_time;
   unsigned_char_p_t    srv_pname;             /*---* srvr principal name *--*/
   uuid_t               uuid;                  /*---* storage for uuid   *---*/
   rpc_ns_handle_t      imprt_ctxt;            /*---* import context     *---*/
   rpc_binding_handle_t bndng_hndle;           /*---* server bind handle *---*/
   idl_small_int        val = 0;               /*---* value for rem. call *--*/
   unsigned_char_p_t    str_p;                 /*---* generic pointer    *---*/
   int                  pid=getpid(),          /*---* pid for id         *---*/
                        passes=0,              /*---* passes to run      *---*/
                        rep_intvl=0,           /*---* reporting interval *---*/
                        run_intvl,             /*---* time to run        *---*/
                        hours = 0,             /*---* hours to run       *---*/
                        mins = 0,              /*---* minutes to run     *---*/
                        end_time=0,            /*---* End time - seconds *---*/
                        done,                  /*---* flag for loop exit *---*/
                        i;                     /*---* generic counter    *---*/
   char                 *gcp,                  /*---* generic char ptr   *---*/
                        *protect_level_str,    /*---* string rep. of same *--*/
                        ubuf[15],
                        *sig_hndlr_arg="client",
                        *sync_fn;              /*---* synchronization    
                                                *---* file name          *---*/
   short                comm_fault_prob = 0;   /*---* general fault flag *---*/
   long_conf_struct_t   cnf_str;               
   long_conf_struct_p_t cnf_str_p;             /*---* 
                                                *---* major param for call to
                                                *---* rpcsec2_cnf_str()
                                                *---*/

   sec_login_auth_src_t auth_src;       /*---* receives auth src info from call
                                         *---* to sec_login_validate_identity
                                         *---*/
   boolean32            reset_passwd;   /*---* receives flag value indicating
                                         *---* whether passwd has expired.
                                         *---*/
   char                 *keytab,        /*---* pointer to keytab filename *--*/
                        *conf_file=NULL; /*---* pointer to config file name 
                                          *---* if passed on the command line
                                          *---*/
extern char             *rd_conf_errs[]; /*---*  rd conf error strings
                                                  *---*/

#if defined(RPCSEC2_NO_CLI_THDS)
   struct sigaction     signal_action;  /*---* sigaction struct for handling
                                         *---* SIGINT and SIGALRM
                                         *---*/
   sigset_t             sigmask;        /*---* defines the signals to block 
                                         *---* while in the signal handler
                                         *---*/
#endif /*---* defined(RPCSEC2_NO_CLI_THDS) *---*/

   time_t               tbuf;

   /*---* 
    *---* the rather bogus nature of this assignment is necessitated by 
    *---* apparent lack of intelligence on the part of cc...it does not figure
    *---* out that the address of a function declared extern void is 
    *---* equivilent to a void (*)() :-(
    *---*/
   void (*rpcsec2_cli_call_p)() =
                      (void (*)())rpcsec2_cnf_str; /*---* the func called *-*/

   /*---* 
    *---* in an effort to get output if run via rsh woth out put redirected
    *---* back to caller, run unbuffered
    *---*/
   setbuf(stdout, (char *)NULL);

   /*---* show our start time *---*/
   time(&tbuf);
   printf("Client(%d): Starting at %s...\n", pid, ctime(&tbuf));

   /*-------------------------------------------*
    *---*         Parse command line        *---* 
    *-------------------------------------------*/
   ++argv;
 
   /*---* while we have stuff on the command line *---*/
   while(*argv)  
   {
      /*---* get the flag *---*/
      if (**argv == '-')
         switch(*((*argv)+1)) 
         {
            case 'N': /*---* use NULL UUID *---*/
                      /*---* allow only one of -N or -u *---*/
                      if (uflag)
                      {
                         printf (
                             "Client(%d): Only one of -n, -N or -u allowed\n",
                             seq_numb);
                         printf("Client(%d): %s\n",seq_numb, usage);
                         exit(3);
                      }

                      Nflag = 1;
                      printf("Client(%d): Using NULL for UUID\n",seq_numb);
                      break;
      
	    case 'P': /*---* protocol to use *---*/
                      /*---* FALLTHROUGH     *---*/
                      
            case 'S': /*---* sync_file name  *---*/
                      /*---* if we have a good arg... *---*/
                      if (*(argv+1))
                      {
                         /*---* check the protocol if applicable *---*/
                         if (*((*argv)+1) == 'P')
                            if (!strcmp(*(argv+1), "datagram"))
			    { 
                               ++argv;
                               protocol = "ncadg_ip_udp";
                               /*---* report using dg *---*/
                               printf("Client(%d): Using protocol %s...\n",
                                                          seq_numb, protocol); 
			    }
                            else if (!strcmp(*(argv+1), "connection"))
			    { 
                               ++argv;
                               protocol = "ncacn_ip_tcp";
                               /*---* report using cn *---*/
                               printf("Client(%d): Using protocol %s...\n",
                                                          seq_numb, protocol); 
			    }
                            else
                            {
                               printf(
                                  "Client(%d): Bad value for -P option(%s).\n",
                                                          seq_numb, *(argv+1));
                               printf("Client(%d): %s\n", seq_numb, usage);
                               exit(3);
                            }
                         else
                            printf("Client(%d): Looking for sync file %s...\n",
                                                seq_numb, (sync_fn = *++argv));
                      
		      }
                      else
		      {
                         printf("Client(%d): Bad value for -P option(%s).\n",
                                                          seq_numb, *(argv+1));
                         printf("Client(%d): %s\n", seq_numb, usage);
                         exit(3);
                      }
                      break;
 
 
	    case 'b': /*---* boundary mode *---*/
                      /*---* 
                       *---* not allowed to do boundary mode with 
                       *---* -h -i, -p or -m options which imply stress mode
                       *---*/
                      if ( rep_intvl || hours || mins || passes )
                      {
                         printf("Client(%d): -b not allowed with -m -h -i or -p\n", seq_numb);
                         printf("Client(%d): %s\n", seq_numb, usage);
                         exit(3);
                      }
                      bflag = 1;
                      printf("Client(%d): Running in boundary mode...\n",
                                                                     seq_numb);
                      break;
 
	    case 'd': /*---* debug mode *---*/
                      debug = 1;
                      printf("Client(%d): Debug mode on.\n",seq_numb);
                      break;

	    case 'f': /*---* configuration file name *---*/
                      /*---* if no value for option... *---*/
                      if (!*(argv+1))
		      {
                         /*---* error and usage *---*/
                         printf("Client(%d): no value for config file name.\n",
                                                                     seq_numb);
                         printf("Server: %s\n", seq_numb, usage);
		      }
                      /*---* else set pointer *---*/
                      conf_file = *++argv;
                      break;

	    case 'L': /*---* last client flag *---*/
                      Lflag = 1;
                      printf("Client(%d): I am the last client...\n",seq_numb);
                      break;
                 
            case 'l': /*---* the protection level *---*/
                      /*---* if no value specified *---*/
                      if (! *(argv+1))
                      {
                         printf(" Client(%d): no value for -l flag (prot level).\n", seq_numb);
                         printf(" Client(%d): %s\n", seq_numb, usage);
                         exit(1);
                      }
                      /*---* else we got a value; is it any good? *---*/
                      else if (strcmp(*++argv, "none") && 
                               strcmp(*argv, "conn") &&
                               strcmp(*argv, "call") &&
                               strcmp(*argv, "pkt") &&
                               strcmp(*argv, "integ") &&
                               strcmp(*argv, "priv") )
                      {
                         printf("Client(%d): bad value for -l flag (prot level).\n", seq_numb);
                         printf("Client(%d): must be one of none, conn, call, pkt, integ, priv.\n", seq_numb);
                         printf("Client(%d): %s\n", seq_numb, usage);
                         exit(1);
                      }
                      else
                         protect_level_str = *argv;
                
                      break;

	    case 'p': /*---* number of passes *---*/
                      /*---*
                       *---* Can't use with time interval or boundary mode.
                       *---* if either of these has been specified...
                       *---*/
                      if ( bflag || hours || mins )
	              {
                         printf(
                             "Client(%d): Can't use -%c with -b, -h, or -m.\n",
                                                      seq_numb, *((*argv)+1) );
                         printf("Client(%d): %s\n", seq_numb, usage);
                         exit(3);
                      }
                      /*---* FALLTHROUGH *---*/

	    case 'h': /*---* hours to run *---*/
                      /*---* FALLTHROUGH *---*/

            case 'm': /*---* minutes to run *---*/
                      /*---* 
                       *---* not allowed with boundary mode(-b) or passes
                       *---* if either have been specified...
                       *---*/
                      if ( bflag || passes )
                      {
                         printf("Client(%d): Can't use -%c with -b or -p.\n",
                                                      seq_numb, *((*argv)+1) );
                         printf("Client(%d): %s\n", seq_numb, usage);
                         exit(3);
                      }
                      /*---* FALLTHROUGH *---*/
            
	    case 'i': /*---* report interval *---*/
                      /*---* 
                       *---* not allowed with boundary mode(-b)
                       *---* if boundary mode has been specified...
                       *---*/
                      if ( bflag )
                      {
                         printf("Client(%d): Can't use -i with -b option.\n",
                                                                     seq_numb);
                         printf("Client(%d): %s\n", seq_numb, usage);
                         exit(3);
                      }
                      /*---* FALLTHROUGH *---*/

	    case 's': /*---* sequence number *---*/
                      /*---* if we got a good arg... *---*/
                      if (*(argv+1) && isdigit(**(argv+1)))
		      {
                         /*---*
                          *---* set a pointer to the variable indicated by
                          *---* **argv
                          *---*/
                         int *i_p = *((*argv)+1) == 'h'? &hours: 
                                   (*((*argv)+1) == 'm'? &mins: 
                                   (*((*argv)+1) == 's'? &seq_numb: 
                                   (*((*argv)+1) == 'p'?&passes: &rep_intvl)));

                         /*---* 
                          *---* convert value to decimal and store in the 
                          *---* right variable
                          *---*/
                         sscanf(*++argv, "%d", i_p);
                         if (*((*(argv-1))+1) == 's')
			 {
                            printf("Client(%d): Sequence number is %d...\n",
                                                          seq_numb, seq_numb);
                            sflag++;
			 }
                         else if (*((*(argv-1))+1) == 'p')
                            printf("Client(%d): Executing for %d passes...\n",
                                                          seq_numb, passes);
                      }
                      /*---* else we have no arg or a bad arg... *---*/
                      else                      
	              {
                         /*---* print appropriate message... *---*/
                         printf("Client(%d): %s for -%c option.\n", seq_numb,
                                           *(argv+1)? "bad value": "no value",
                                           *((*argv)+1));
                         printf("Client(%d): %s\n", seq_numb, usage);
                         exit(3);
                      }
                      break;

	    case 't': /*---* test all bindings *---*/
                      tflag = 1;
                      printf("Client(%d): Timing rpc calls\n",seq_numb);
                      rpcsec2_cli_call_p = 
                                   (void(*)())rpcsec2_cli_timed_cnf_str_call;
                      break;

            case 'u': /*---* use passed (we hope) UUID *---*/
                      /*---* allow only one of -n, -N, or -u *---*/
                      if (nflag || Nflag)
                      {
                         printf (
                             "Client(%d): Only one of -n, -N, or -u allowed\n",
                             seq_numb);
                         printf("Client(%d): %s\n",seq_numb, usage);
                         exit(3);
                      }

                      uflag = 1;

                      if (*(argv+1))
                      {
                         /*---* convert string to uuid *---*/
                         printf("Client(%d): converting %s to uuid...", 
                                                                     seq_numb,
                                                                     *++argv);
                         uuid_from_string((unsigned_char_t *) *argv, &uuid,
                                          &ret_stat);

                         if (ret_stat != uuid_s_ok)
                            rpcsec2_check_stat(ret_stat, "converting to uuid",
                                              err_stats, &nerr_stats, debug,0);
                         else
                            printf("done.\n\n");
                      }
                      else
                      {
                         printf("Client(%d): no value for -u option\n",
                                                                     seq_numb);
                         printf("Client(%d): %s\n",seq_numb, usage);
                         exit(3);
                      }

                      break;

 
            case 'v': /*---* value for procedure call *---*/
                      if (!*(argv+1) || !isdigit(**(argv+1)))
                      { 
                         printf("%s value for -v option. %s\n", 
                                (!*(argv+1)? "No": "Bad"), 
                                (!*(argv+1)? "": *(argv+1))); 

                         printf("Client(%d): %s\n",seq_numb,usage);
                         exit(3);
                      }
                      
                      val = atoi(*++argv);
                      break;

	    case 'n': /*---* try nil uuid *---*/
                      nflag = 1;
                      printf("Client(%d): Using nil UUID\n", seq_numb);

                      /*---* FALLTHROUGH *---*/

            default :
                      if (*((*argv)+1) != 'n')
	              {
                         printf("Client(%d): bad option %s\n",seq_numb, *argv); 
                         printf("Client(%d): %s\n", seq_numb, usage);
                         exit(3);
                      }                         
 
                      printf ("Client(%d): Creating NIL uuid for import...",
                                                                     seq_numb);
                      uuid_create_nil(&uuid,&ret_stat);
  
                      if (rpcsec2_check_stat(ret_stat,  "creating nil uuid", 
                                              err_stats, &nerr_stats, debug,0))
                         exit(3);
                      
                      break;
         }
      else
      {
         printf("Client(%d): bad command line input\n",seq_numb);
         printf("Client(%d): %s\n", seq_numb, usage);
         exit(3);
      }

      argv++;
   }

   /*----------------------------------------------------------*
    *---* Get our operational parameters from the config file
    *----------------------------------------------------------*/
   printf("Client(%d): Reading config file (%s)...",  seq_numb,
                                      (conf_file? conf_file: RPCSEC2_CONF_FN));
   rd_conf((conf_file? conf_file: RPCSEC2_CONF_FN), params, 
                   sizeof(params)/sizeof(param_spec_val_t), &ret_stat);
   /*---* if we did not succeed... *---*/
   if (ret_stat != rdconf_ok)
   {
      /*---* report error *---*/
      printf("\n\nClient(%d): %s (%s).\n", seq_numb, 
                                           rd_conf_errs[ret_stat],
                                      (conf_file? conf_file: RPCSEC2_CONF_FN));
      /*---* exit *---*/
      exit(2);
   }
   /*---* report success *---*/
   printf("\ndone.\n\n");

   /*---* convert the start_delay param *---*/
   int_from_PVAL(&start_delay,params,delay,"start delay",ret_stat);
   if (ret_stat)
       exit(2);

   /*---* set the protection level *---*/
   if (protect_level_str)
      gcp = protect_level_str;
   else
      gcp = PVAL(params, prot_lev);

   /*---* transform the string to a real value with an ugly big if *---*/
   sprintf(ubuf,"Client(%d)", seq_numb);
   SET_PROT_LEVEL(gcp, protect_level, ubuf);
   
   printf(" Client(%d): Using protection level %s...\n", seq_numb, gcp);

   /*---* if no arg passed, just use NULL *---*/
   if ( !(nflag || uflag))
   {
      printf("Client(%d): Using NULL for object UUID\n", seq_numb);
      Nflag = 1;
   }

   /*---* if no seq_numb, use pid... *---*/
   if (!seq_numb) 
   {
      printf("Client(%d): No sequence number specified, using %d...\n", 
                                                                seq_numb, pid);
      seq_numb = pid;
   }

   /*---* if not in boundary mode... *---*/
   if (!bflag)
   {
      /*---* set the report interval if not set on the command line *---*/
      if (!rep_intvl)
      {
         /*---* set reporting interval... *---*/
         int_from_PVAL(&rep_intvl,params,rep_int,"reporting interval",ret_stat);
         if (ret_stat)
            exit(2);
      }
      /*---* if executing for some number of passes... *---*/
      if (passes)
      {
         /*---* tell the user when we will report *---*/
         printf("Client(%d): printing status after every %d passes...\n",
                                                 seq_numb, rep_intvl);
      }
      /*---* else this is a timed run... *---*/
      else
      {
         /*---* get hours and minutes set *---*/
         if (!(hours || mins))
	 {
            int_from_PVAL(&hours,params,run_int,"run duration",ret_stat);
            if (ret_stat)
               exit(2);
	 }
         /*---* report to the user *---*/
         printf("Client(%d): Executing for %d hours and %d mins...\n",seq_numb,
                                                                  hours, mins);
         /*---* set the run interval in seconds *---*/
         run_intvl = (hours * 3600) + (mins * 60);

         /*---* tell the user when we will report *---*/
         printf("Client(%d): Printing status every %d minutes...\n",seq_numb,
                                                                  rep_intvl);
         /*---* set the rep_intvl in seconds *---*/
         rep_intvl *= 60;
         
         /*---* init the cond variable for the reporting thread *---*/
         printf("Client(%d): Initializing reporting condition variable...",
                                                                     seq_numb);
         if (pthread_cond_init(&rep_timer_cond,pthread_condattr_default) != 0)
         {
            perror(" \n    Error initializing reporting condition variable:");
            exit(1);
         }
         printf("done.\n");

         /*---*
          *---* initialize the reporting mutex
          *---*/
         printf("Client(%d): Initializing the reporting mutex...", seq_numb);
         if (pthread_mutex_init(&rep_timer_mutex,pthread_mutexattr_default)!=0)
         {
            perror(" \n    Error initializing reporting mutex:");
            exit(1);
         }
         printf("done.\n");
      }
   }


   /*---* get the machine name and os_type *---*/
   uname(&uname_vals);
   printf("Client(%d): PID: %d System: %s; O/S: %s...\n", seq_numb, pid,
                                     uname_vals.nodename, uname_vals.sysname);
   

   /*------------------------------------------------------*
    *---* Register auth info, get our identity, and "login"
    *------------------------------------------------------*
    *---* construct the keytab filename. First get space for the string. We 
    *---* will need length(pathname) + length(filename) + '/' + '\0'
    *---*/
   keytab = (char *)malloc(strlen(PVAL(params, keytab_dir)) +
                           strlen(PVAL(params, keytab_fn))  + 2 );
   /*---* fill in the string *---*/
   if (debug)
   {
      printf("keytab dir = %s\n", PVAL(params, keytab_dir));
      printf("keytab file = %s\n", PVAL(params, keytab_fn));
      printf("keytab path = %s", PVAL(params, keytab_dir));
      printf("/%s\n", PVAL(params, keytab_fn));
   }
   sprintf(keytab, "%s/", PVAL(params, keytab_dir));
   strcat(keytab, PVAL(params, keytab_fn));

   /*---* for the efficiency freak in me ...*---*/
   free(PVAL(params, keytab_dir));
   free(PVAL(params, keytab_fn));

   /*---* Create a context and get the login contex loaded *---*/
   printf("Client(%d): create login context...(%s)", seq_numb,
                                            PVAL(params, cli_princ));
   sec_login_setup_identity((unsigned_char_t *)PVAL(params,cli_princ),
                            (sec_login_flags_t)sec_login_no_flags,
                            &login_ctxt, &ret_stat);
   
   if (rpcsec2_check_stat(ret_stat, "creating login context",
                                       err_stats, &nerr_stats, debug,0))
      exit(1);

   /*---* Get our secret key from our keytab file *---*/
   printf("Client(%d): getting secret key from %s...", seq_numb, keytab);
   sec_key_mgmt_get_key(rpc_c_authn_dce_secret, (void *)keytab,
                        (char *)PVAL(params,cli_princ), (unsigned32)0, 
                        &sec_key_p, &ret_stat);

   if (rpcsec2_check_stat(ret_stat, "getting secret key",
                                       err_stats, &nerr_stats, debug,0))
      exit(1);

   if (debug)
      dump_key((sec_passwd_rec_t *)sec_key_p);

   /*---* 
    *---* sec_login_validate destroys the key as part of attempt to prevent
    *---* reuse of the key.   Since we want the timer thread to process 
    *---* refresh cycles as fast as possible, we don't want to have to get the
    *---* key all the time, so this vanishing data is vexing.  To get around
    *---* this behavior, we store the data in a global array, and fix the 
    *---* struct in the timer thread.
    *---*/
   memcpy(des_key, 
      (char *)(((sec_passwd_rec_t *)sec_key_p)->key.tagged_union.des_key),8);
   *(des_key+8) = '\0';

   /*---* validate our login context *---*/
   printf("Client(%d): validating login context...",seq_numb);
   if (debug)
      dump_key((sec_passwd_rec_t *)sec_key_p);
   sec_login_validate_identity(login_ctxt, sec_key_p, &reset_passwd,
                                                        &auth_src, &ret_stat);
   /*---*
    *---* ???? need to do something about expired passwd here ????
    *---*/
   if (rpcsec2_check_stat(ret_stat, "validating login context",
                                       err_stats, &nerr_stats, debug, 0))
      exit(1);
   

   /*---* if we did not get validated for network ops... *---*/
   if (auth_src != sec_login_auth_src_network)
   {
      /*---* error and out *---*/
      printf("Client(%d): Not authorized for network operations!\n", seq_numb);
      exit(2);
   }

   /*---* certify identity information *---*/
   printf("Client(%d): certifying identity info...", seq_numb);
   sec_login_certify_identity(login_ctxt, &ret_stat);
 
   if (rpcsec2_check_stat(ret_stat, "certifying identity info",
                                       err_stats, &nerr_stats, debug, 0))
      exit(1);

   /*---* finally, get our context *---*/
   printf("Client(%d): setting our own context...", seq_numb);
   sec_login_set_context(login_ctxt, &ret_stat);

   if (rpcsec2_check_stat(ret_stat, "setting our own context",
                                       err_stats, &nerr_stats, debug, 0))
      exit(1);
   /*---* 
    *---* we will manage our own identity updates.  We get the expiration 
    *---* time, subtract the current time, and then set up a thread to 
    *---* refresh our id 60 seconds before it expires.
    *---*
    *---* initialize the condition variable
    *---*/
   printf("Client(%d): Initializing condition variable...",seq_numb);
   if ( pthread_cond_init(&refr_id_cond, pthread_condattr_default) != 0 )
   {
      perror(" \n    Error initializing condition variable:");
      exit(1);
   }
   printf("done.\n");

   /*---*
    *---* initialize the mutex
    *---*/
   printf("Client(%d): Initializing the mutex...", seq_numb);
   if ( pthread_mutex_init(&refr_id_mutex, pthread_mutexattr_default) != 0 )
   {
      perror(" \n    Error initializing mutex:");
      exit(1);
   }
   printf("done.\n");

   /*---*
    *---* get the expiration time...
    *---*/
   if (debug)
      printf("Client(%d): Getting our expiration date...", seq_numb);
   sec_login_get_expiration(login_ctxt, &exp_time, &ret_stat);
   if (rpcsec2_check_stat(ret_stat, "getting expiration time", err_stats, 
                                                      nerr_stats, debug, 1))  
   {
       /*---*  
        *---* we can't verify if it is the identity problem. 
        *---* since we may in fact be okay, we just go on. 
        *---*/
       printf("Client(%d): Can't get identity information\n", seq_numb);
   }
   /*---*
    *---* we got the info, so check it against the current time.
    *---*/
   else
   {
       int expdif;
       char *_cp1, *_cp2;
       time_t _tbuf;
    
       /*---*
        *---* set the alarm for exp_time - now - 60....
        *---* Empirical evidence suggests that a 1 minute buffer may not be
        *---* long enough.  In order to try to avoid this 
        *---* problem, we use half the id lifetime.
        *---*/
       CTIME_NO_NL(_cp1, &exp_time)
       exp_time -= time(&_tbuf);
       CTIME_NO_NL(_cp2, &_tbuf)
       /*---* round up if necessary *---*/
       exp_time += ((expdif = exp_time%60)? 60 - expdif: 0);
       id_lifetime = exp_time / 2;
       printf("Client(%d): identity lifetime = %d mins, refresh interval is %d mins.\n", seq_numb, exp_time/60, id_lifetime/60);
       printf("Client(%d): Spawning refresh thread...", seq_numb);
       USE_PTHREAD(thread_tbl, thd_array, RPCSEC2_REFR_THD);
       if (pthread_create(thread_tbl[RPCSEC2_REFR_THD].thd_id, 
           pthread_attr_default, 
           (pthread_startroutine_t)maintain_id, 
           (pthread_addr_t)&id_lifetime) != 0 )
       {
          perror(" \n    Error creating refresh thread:");
          exit(1);
       }
       printf("done.\n");

   }
   USE_PTHREAD(thread_tbl, thd_array, RPCSEC2_MAIN_THD);
   *(thread_tbl[RPCSEC2_MAIN_THD].thd_id) = pthread_self();

   /*---* 
    *---* create the thread that will handle signals
    *---*/
   printf("Client(%d): Spawning signal catcher thread...", seq_numb);
   USE_PTHREAD(thread_tbl, thd_array, RPCSEC2_SIG_THD);
   if (pthread_create(thread_tbl[RPCSEC2_SIG_THD].thd_id, 
       pthread_attr_default, 
       (pthread_startroutine_t)signal_catcher, 
       (pthread_addr_t)sig_hndlr_arg) != 0 )
   {
      perror(" \n    Error creating signal catcher thread:");
      rpcsec2_kill_threads(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(1);
   }
   printf("done.\n");
   

   /*---* 
    *---* setup for call to rpcsec2_cnf_str()
    *---* 
    *---* We need to allocate space for the struct, and then for the array
    *---* defined with in it.  RPC mandates that the array must be the 
    *---* last thing in the struct.  This (I bet 10 bucks) is so that we
    *---* can play pointer games to handle runtime sizing of the array element.
    *---*
    *---* allocate a struct, plus the value arguement we got passed on the
    *---* command line with -v (if none passed, use 10) - 1.  We allocate
    *---* val - 1 since the struct definition that idl generates for array
    *---* members allocates 1 for us.
    *---*/
   printf("Client(%d): allocating space for RPC args...", seq_numb);
   if (!(cnf_str_p =
     (long_conf_struct_p_t)malloc(sizeof(long_conf_struct_t)+ ((val?val-1:9) * sizeof(long))) ) )
   {
      /*---* no space *---*/
      printf("\n     Error: no space for RPC args\n\n");
      rpcsec2_kill_threads(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(2);
   }
   printf("\ndone.\n");

   /*---* 
    *---* load test struct 
    *---*/
   cnf_str.id = pid;
   cnf_str.size = 1;
   cnf_str.array[0] = pid + (2*0) + (3*(1-0));

   /*---* 
    *---* Load the struct members id, and size
    *---*/
   cnf_str_p->id = pid;
   cnf_str_p->size = val? val : 10; 
   
   /*---* 
    *---* loop through the array, setting members to the following value which 
    *---* is used as a checksum at the server:
    *---*   id + (2*index) + (3*size)
    *---*/
   for (i=0; i< cnf_str_p->size; i++)
      cnf_str_p->array[i] = pid + (2*i) + (3*(cnf_str_p->size - i));

   /*---* if debug mode set... *---*/
   if (debug)
   {
      /*---* dump the struct *---*/
      printf("Client(%d): struct is :\n",seq_numb);
      printf("Client(%d):    id = %d\n", seq_numb,pid);  
      printf("Client(%d):  size = %d\n", seq_numb,cnf_str_p->size);  
      printf("Client(%d): array =\n", seq_numb);  
      for (i=0; i < cnf_str_p->size; i++)
         printf("Client(%d):          [%d] = %d\n", seq_numb,i,
                                                          cnf_str_p->array[i]);
   }

   /*---* 
    *---* we take any binding unless protocol is specified 
    *---*/
   /*-------------------------------------------------------------*
    *---* look for bindings for the rpcsec2 interface 
    *-------------------------------------------------------------*/
   printf("Client(%d): Trying to import binding handles for %s...", seq_numb,
                                                        PVAL(params,cds_name));
   rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce, 
                               (unsigned_char_p_t)PVAL(params,cds_name), 
                               rpcsec2_v1_1_c_ifspec, 
                               Nflag? (uuid_p_t)NULL : &uuid, &imprt_ctxt,
                               &ret_stat);

   if (rpcsec2_check_stat(ret_stat, "importing binding handles",
                                       err_stats, &nerr_stats, debug, 0))
   {
      rpcsec2_kill_threads(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(1);
   }

   /*---* initialize exit flag *---*/
   done = 0;   
   while(ret_stat != rpc_s_no_more_bindings && !done )
   {
      /*---* get a (the) binding handle associated with the interface *---*/
      printf("Client(%d): get next binding handle...", seq_numb);
      rpc_ns_binding_import_next(imprt_ctxt, &bndng_hndle, &ret_stat);   

      if (rpcsec2_check_stat(ret_stat, "getting next binding handle",
                                       err_stats, &nerr_stats, debug, 0))
      {
         rpcsec2_kill_threads(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
         exit(1);
      }
     
      /*---* then we must convert the binding to a string binding, parse 
       *---* the string binding, and compare the protocol in the binding to
       *---* the specified protocol.
       *---*/
      printf("Client(%d): converting binding handle...", seq_numb);
      /*---* convert to a string *---*/
      rpc_binding_to_string_binding(bndng_hndle, &str_p, &ret_stat);
      if (rpcsec2_check_stat(ret_stat, "converting binding to string",
                                 err_stats, &nerr_stats, debug, 0) == 0 )
      {
          unsigned_char_t *locstr_p;
          /*---* if we are in debug mode... *---*/
          if (debug)
             /*---* print it *--*/
             printf("Client(%d): got %s\n", seq_numb,str_p);
          /*---* parse string binding to see if it of the right type *---*/
          printf("Client(%d): Parsing string binding...", seq_numb);
          rpc_string_binding_parse(str_p, (unsigned_char_t **)NULL,
                                      &locstr_p, (unsigned_char_t **)NULL,
                                      (unsigned_char_t **)NULL,
                                      (unsigned_char_t **)NULL, &ret_stat);
          if (rpcsec2_check_stat(ret_stat, "parsing string binding",
                                  err_stats, &nerr_stats, debug, 0) == 0)
          {
             if ( !protocol || !strcmp(locstr_p, protocol))
	     {
                protocol= protocol? protocol: (char *)locstr_p;

                /*---* 
                 *---* check to make sure this is a usable binding.  We first
                 *---* need to convert the partially bound handle to a fully
                 *---* bound handle for the rpc_mgmt_is_server_listening
                 *---* call.
                 *---*/
                printf("Client(%d): converting partial to full binding...", 
                                                                     seq_numb);
                rpc_ep_resolve_binding(bndng_hndle, rpcsec2_v1_1_c_ifspec, 
                                                                 &ret_stat);
                if (!rpcsec2_check_stat(ret_stat,"converting to full binding",
                                       err_stats, &nerr_stats,debug,0))
		{
                   printf("Client(%d): Checking binding conectivity...",
                                                                   seq_numb);
                   rpc_mgmt_is_server_listening(bndng_hndle, &ret_stat);
                   if (rpcsec2_check_stat(ret_stat, "checking binding",
                                  err_stats, &nerr_stats,debug,0) == 0)
		   {
                      printf("Client(%d): Got a binding...\n", seq_numb);
                      /*---* set loop exit flag *---*/
                      done = 1;
	           }   
		}
                else
		   printf("Client(%d): could not check connectivity...\n", 
                                                                     seq_numb);

                if (!done)
                {
                   printf("Client(%d): freeing protocol string...", seq_numb);
                   rpc_string_free(&locstr_p, &ret_stat);
                   if (rpcsec2_check_stat(ret_stat, "freeing protocol string",
                                       err_stats, &nerr_stats,debug,0))
                      printf("Client(%d): leaking %d bytes of memory...\n", 
                                                   seq_numb, strlen(locstr_p));
		}
	    }
            /*---* free the memory *---*/            
            printf("Client(%d): freeing binding handle string...", seq_numb);
            rpc_string_free(&str_p, &ret_stat);
         
            if (rpcsec2_check_stat(ret_stat, "freeing binding string",
                                       err_stats, &nerr_stats,debug,0))
               printf("Client(%d): Leaking memory :-{\n", seq_numb);
	 }
      }
   }

   /*---* if we didn't get a binding... *---*/
   if (!done)
   {
      /*---* error and out *---*/
      printf("Client(%d): couldn't get a binding!\n");
      rpcsec2_kill_threads(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(2);
   }

   /*---* need to get the server name to put in the set auth info call *---*/
   printf("Client(%d): Getting server name...", seq_numb);
   rpc_mgmt_inq_server_princ_name(bndng_hndle,
                                  (unsigned32)rpc_c_authn_dce_secret,
                                  &srv_pname,
                                  &ret_stat);
   if (rpcsec2_check_stat(ret_stat, "getting server name",
                                       err_stats, &nerr_stats, debug,0))
   {
      rpcsec2_kill_threads(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(2);
   }
   /*---* report server name, *---*/
   printf("Client(%d): Server principal name is %s...\n",seq_numb, 
                                                        (char *)srv_pname);

   /*---* specify authz and authn information, use default login ctxt *---*/
   printf("Client(%d): setting authz and authn info...",seq_numb);
      
   rpc_binding_set_auth_info(bndng_hndle,  srv_pname,  protect_level,
                             (unsigned32)rpc_c_authn_dce_secret, 
                             (rpc_auth_identity_handle_t)login_ctxt,
                             (unsigned32)rpc_c_authz_name,
                             &ret_stat);

   if (rpcsec2_check_stat(ret_stat, "setting auth info",
                                       err_stats, &nerr_stats, debug,0))
   {
      rpcsec2_kill_threads(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(2);
   }

   /*---* show our ready time *---*/
   time(&tbuf);
   CTIME_NO_NL(gcp, &tbuf)
 
   /*---* print the READY message that the driver script looks for  *---*/
   printf("Client( %d ): READY to start making calls Host: %s OS: %s Protocol: %s at %s...\n", seq_numb, uname_vals.nodename, uname_vals.sysname, protocol, 
                                                                          gcp);
   fflush(stdout);

   /*---* 
    *---* if we are running for a duration, start the report generation
    *---* thread.  Starting it here means that the reports might not be 
    *---* generated at exactly 5 minutes after the STARTING report, but 
    *---* given that adding this in after the syncstart call adds an 
    *---* undesirable chunk of overhead before the first call, we'll accept 
    *---* the skew in report time.
    *---*/
   if (!passes && !bflag)
   {
      printf("Client(%d): Spawning reporting thread...", seq_numb);
      USE_PTHREAD(thread_tbl, thd_array, RPCSEC2_REP_THD);
      if (pthread_create(thread_tbl[RPCSEC2_REP_THD].thd_id, 
          pthread_attr_default, 
          (pthread_startroutine_t)rep_timer, 
          (pthread_addr_t)&rep_intvl) != 0 )
      {
         perror(" \n    Error creating reporting thread:");
         rpcsec2_kill_threads(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
         exit(1);
      }
      printf("done.\n");
   }

   if (sync_fn)
   {
      struct timespec sync_delay_ts = {0,0};
      /*---* the pthread call is a wrapped sleep *---*/
      pthread_delay_np(rpcsec2_cli_syncstart(sync_fn, Lflag, &sync_delay_ts));
   }

  
   /*---* if we are in boundary mode... *---*/
   if (bflag)
   {
      char *msg_p;
      int ex_val=0;

      /*---* get the time... *---*/
      getclock(TIMEOFDAY, &precall_time);

      /*---* and call rpcsec2_cnf_str() *---*/
      rpcsec2_cnf_str(bndng_hndle, (idl_short_int)bflag, cnf_str_p, 
                                (idl_long_int *)&ret_stat, &fault_stat, 
                                &comm_stat);

      /*---* get return time *---*/
      getclock(TIMEOFDAY, &postcall_time);
    
      /*---* save time for print *---*/
      min_time = postcall_time;

      /*---* get elapsed time *---*/
      TVAL_SUB(postcall_time, precall_time);
   

      /*---* check comm status to see if server rejected us because it was too
       *---* busy.
       *---* if datagram and rpc_s_comm_failure or 
       *---* tcp and rpc_s_server_too_busy...
       *---*/
      if (
      (!strcmp(protocol,"ncadg_ip_udp") && comm_stat == rpc_s_comm_failure)||
    (!strcmp(protocol,"ncacn_ip_tcp") && comm_stat == rpc_s_server_too_busy))
      { 
         ex_val=1;
         /*---* set message pointer *---*/
         msg_p = "FAILED - SERVER IS TOO BUSY";
      }
      else if ((comm_fault_prob = rpcsec2_cli_check_ret(ret_stat, comm_stat,
                                  fault_stat, 
                                  (short)rpcsec2_op_is_cnf_str)) == 2)
      { 
         ex_val = 2;
         msg_p = "FAILED - comm or server error";
      }
      else if (comm_fault_prob == 1)
      {
         ex_val = 2;
         msg_p = "FAILED - server returned an error";
      }
      else
         msg_p = "SUCCEEDED - call to on rpcsec2_cnf_str";

      /*---* report.  Need to strip the #@$ newline off the time first *---*/
      CTIME_NO_NL(gcp, &(precall_time.tv_sec))
      printf(
       "Client( %d ): Prtcl: %s %s (start: %s  elapsed: %d sec %d nsec.\n",
       seq_numb, protocol, msg_p, gcp, postcall_time.tv_sec,
       postcall_time.tv_nsec);
      /*---* 
       *---* Cancel all our threads, otherwise they will continue after 
       *---* this main thread exits
       *---*/
      rpcsec2_kill_threads(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(ex_val);
   }
   else
   {
      /*---* generate initial report line *---*/
      rpcsec2_rep_gen(1);


      /*---* while we are not done *---*/
      done = 0;
      while (!done)
      {
         /*---*
          *---* make the call
          *---*/
         if (!tflag && !start_time.tv_sec)
             getclock(TIMEOFDAY, &start_time );
 
         (*rpcsec2_cli_call_p)(bndng_hndle, (idl_short_int)bflag, cnf_str_p, 
                            &ret_stat, &fault_stat, &comm_stat);

         /*---*
          *---* if start_time is empty here, then we are timing calls, and 
          *---* can use the precall time from the first call
          *---*/
         if (! start_time.tv_sec)
            start_time = precall_time;

         /*---* update call count *---*/
         calls++;

         /*---* check on success or failure of call *---*/
         comm_fault_prob = rpcsec2_cli_check_ret(ret_stat, comm_stat, 
                                   fault_stat, (short)rpcsec2_op_is_cnf_str);

         /*---* if we failed... *---*/
         if (comm_fault_prob)
	 {
            /*---* increment failed counter *---*/
            failed++;
            /*---* 
             *---* we are interested in auth failures.  if we failed due to
             *---* ticket expiration, find out if it makes sense given our
             *---* expiration time.
             *---*/
            if (comm_stat == rpc_s_auth_tkt_expired)
            {
               /*---*
                *---* get the expiration time...
                *---*/
               if (debug)
                  printf("Client(%d): Getting our expiration date...",
                                                                   seq_numb);
               sec_login_get_expiration(login_ctxt, &exp_time, &ret_stat);
               if (rpcsec2_check_stat(ret_stat, "getting expiration time", 
                                            err_stats, &nerr_stats, debug, 1))
               {
                  /*---*  
                   *---* we can't verify if it is the identity problem. 
                   *---* since we may in fact be okay, we just go on. 
                   *---*/
                  printf("Client(%d): Can't get identity information\n", 
                                                                   seq_numb);
               }
               printf("Client(%d): Expiration date from context: %s...\n",
                                       seq_numb, ctime((time_t *)&exp_time));
               printf("Client(%d): Last id refresh was at: %s...\n", 
                                  seq_numb, ctime((time_t *)&last_id_refr));
               printf("Client(%d): Refresh interval: %d seconds (%d min)...\n",
                                    seq_numb, id_lifetime, id_lifetime/60);
	    }
	 }
         else
            passed++;
        
         /*---* figure out if we should stop *---*/
         /*---* if we are running for an interval *---*/
         if (run_intvl)
	 {
            (void)time(&tbuf);

            /*---* if end time not set... *---*/
            if (!end_time)
	    {
               /*---* set it *---*/
               end_time = start_time.tv_sec + run_intvl;
               if (debug)
	       {
                 /*---* strip newline from time string *---*/
                  CTIME_NO_NL(gcp, &end_time)
                  printf("Client(%d): estimated end time is %s...\n",seq_numb, 
                                                                         gcp);
	       }
	    }
            /*---* else if we have passed end time *---*/
            else if (tbuf >= end_time )
	    {
	       if (debug)
	       {
                  CTIME_NO_NL(gcp, &tbuf)
                  printf("Client(%d): Exiting at %s ", seq_numb, gcp);
                  CTIME_NO_NL(gcp, &end_time)
                  printf("(end time was %s).\n", gcp);
	       }
                  /*---* set exit flag *---*/
               done = 1;
	    }
	 } 
         /*---* else if we are running for x passes... *---*/
         else
	 {
            /*---* if it is time for a report *---*/
            if (calls % rep_intvl == 0 && calls != passes)
               /*---* print one *---*/
               rpcsec2_rep_gen(0);
            if (calls == passes)
            {
               /*---* end processing expects the current time in tbuf *---*/
               (void)time(&tbuf);
               done = 1;
            }
         }
      }
   }
   
   /*---* generate final report line *---*/
   rpcsec2_rep_gen(done?-1:-2);

   run_intvl = tbuf - start_time.tv_sec;
   if (debug)
   {
      printf("Start time : %s\n", ctime((time_t *)&start_time.tv_sec));
      printf("End time   : %s\n", ctime(&tbuf));
   }

   printf("Client(%d): RUNTIME: %d hours, %d min, %d secs.\n", seq_numb,
               run_intvl/3600, (run_intvl%3600)/60, (run_intvl%3600)%60);
   
   /*---* free the resources *---*/
   printf ("Client(%d): freeing import context...", seq_numb);
   rpc_ns_binding_import_done(&imprt_ctxt, &ret_stat);
   if (rpcsec2_check_stat(ret_stat, "freeing import context",
                                       err_stats, &nerr_stats, debug, 0))
     printf("Client(%d): leaking memory (sizeof(import context))...\n", 
                                                                     seq_numb);

   /*---* 
    *---* Cancel all our threads, otherwise they will continue after 
    *---* this main thread exits
    *---*/
   rpcsec2_kill_threads(thread_tbl[RPCSEC2_MAIN_THD].thd_id);

#if defined(RPCSEC2_DRVR_HNDLS_SIGCHLD)
   /*---* 
    *---* We use a signal handler in the driver to catch child exit.  In order
    *---* to prevent the handler from being bombarded with exits as all the 
    *---* run intervals expire, we sleep for seq_numb * 2 minutes to stagger
    *---* the arrival of the interrupts
    *---*/
   start_time.tv_sec = sflag?seq_numb * 120: 0;
   printf("Client(%d): exiting in %d seconds...\n",seq_numb,start_time.tv_sec);
   pthread_delay_np(&start_time);
#endif /*---* defined(RPCSEC2_DRVR_HNDLS_SIGCHLD) *---*/

   exit(1);
   
}


/*---*
 *---* rpcsec2_cli_check_ret(ret_stat, comm_stat, fault_stat, op_def)
 *---*
 *---*    takes the return status, comm status, and fault status values
 *---*    resulting from a remote call and checks for error returns.  The 
 *---*    check of the return status is done based on the op_def passed. 
 *---* 
 *---*    Returns 0 for success, 1 for server error return and 2 for comm or
 *---*    fault status.
 *---*/
int
rpcsec2_cli_check_ret(unsigned32 ret_stat, error_status_t comm_stat,
                  error_status_t fault_stat, short op_def)
{
      int ret_val;
      extern char *rpcsec2_nop_errs[],
                  *rpcsec2_cnf_str_errs[],
                  *rpcsec2_srv_check_auth_errs[];

      /*---* See if we succeeded.  First check comm_status... *---*/
      if (rpcsec2_check_stat(comm_stat, "comm error making remote call",
                                       err_stats, &nerr_stats,debug, 1))
      {
         /*---* comm prob, reiterate bad news of failure *---*/
         if (debug)
            printf("Client(%d): Remote call failed due to comm problem.\n",
                                                                     seq_numb);
         
         /*---* set return status *---*/
         ret_val = 2;
        
         /*---* lets just see what is in the return status variable *---*/
         if (debug)
            printf("Client(%d): checking return status...\n", seq_numb);
         rpcsec2_check_stat(ret_stat, "return status with comm error",
                                       err_stats, &nerr_stats, debug, 1);
      } 
      /*---* comm status is okay; next check fault status... *---*/
      else if (rpcsec2_check_stat(fault_stat,"server fault making remote call",
                                       err_stats, &nerr_stats,debug, 1))      {
         /*---* server fault of some sort. Reiterate bad news of failure *---*/
         if (debug)
            printf("Client(%d): Remote call failed due to server fault.\n",
                                                                     seq_numb);
   
         /*---* set return status *---*/
         ret_val = 2;
      } 
      /*---* 
       *---* comm status and fault status okay, check procedure status 
       *---*
       *---* See if we failed due to authorization problems
       *---* if ret_stat >= rpcsec2_srv_auth_bad_srv....
       *---*/
      else if (ret_stat > rpcsec2_srv_auth_bad_srv)
      {
         /*---* switch on the error return *---*/
         switch(ret_stat)
	 {
	    case rpcsec2_srv_auth_bad_srv: /*---*
                                            *---* bad server principle name 
                                            *---* passed in RPC
                                            *---*/
                   if (debug)
                      printf("Client(%d): %s (%s).\n", seq_numb, 
                                         rpcsec2_srv_check_auth_errs[ret_stat],
                                              PVAL(params,srv_princ));
                   break;

	    case rpcsec2_srv_auth_bad_cli: /*---* 
                                            *---* passed bad client principal 
                                            *---* name in RPC
                                            *---*/
                   if (debug)
                      printf("Client(%d): %s(%s).\n", seq_numb,
                                         rpcsec2_srv_check_auth_errs[ret_stat],
                                             PVAL(params, cli_princ));
                   break;

		                 default: /*---* print the error string *---*/
                   if (debug)
                      printf("Client(%d): %s.\n", seq_numb,
                                        rpcsec2_srv_check_auth_errs[ret_stat]);
	 }
         (void)rpcsec2_check_stat(ret_stat,
                                         rpcsec2_srv_check_auth_errs[ret_stat],
                                       err_stats, &nerr_stats,debug, 1);
      }
    
      /*---*   
       *---* else if we are checking for rpcsec2_nop()...
       *---*/
      else if (op_def == rpcsec2_op_is_nop)
      {
         /*---* check for success *---*/
         if (ret_stat == rpcsec2_nop_ok)    
         {
            /*---* Success! Give good news to user *---*/
            if (debug)
               printf("Client(%d): Call to rpcsec2_nop() succeeded\n",
                                                                     seq_numb);

            /*---* Set return value *---*/
             ret_val = 0;
	 }
         else
         {
            /*---* remote call returns error, notify user *---*/
            if (debug)
	    {
               printf(
                    "Client(%d): Remote call failed - server returned error\n",
                                                                    seq_numb);
               printf("Client(%d): %s.\n", seq_numb, 
                                                  rpcsec2_nop_errs[ret_stat]); 
	    }
            (void)rpcsec2_check_stat(ret_stat,
                                         rpcsec2_nop_errs[ret_stat],
                                       err_stats, &nerr_stats,debug, 1);

	 } /*---* end of if (ret_stat == rpcsec2_ok) *---*/
      } /*---* end of if (op_def == rpcsec2_op_is_nop) true block *---*/
      /*---* we are checking for rpcsec2_cnf_str()... *---*/
      else
      {
         /*---* check for success *---*/
         if (ret_stat == rpcsec2_cnf_ok)    
         {
             /*---* Success! Give good news to user *---*/ 
             if (debug)
                printf("Client(%d): Call to rpcsec2_cnf_str() succeeded\n",
                                                                     seq_numb);

             /*---* Set return value *---*/
             ret_val = 0;
	 }
         else
         {

            /*---* remote call returns error, notify user *---*/
            if (debug)
	    {
               printf(
                    "Client(%d): Remote call failed - server returned error\n",
                                                                     seq_numb);
               printf("Client(%d): %s.\n", seq_numb, 
                                               rpcsec2_cnf_str_errs[ret_stat]);
	    }
            (void)rpcsec2_check_stat(ret_stat,
                                         rpcsec2_cnf_str_errs[ret_stat],
                                       err_stats, &nerr_stats,debug, 1);

	 } /*---* end of if (ret_stat == rpcsec2_ok) *---*/
      } /*---* end of if (op_def == rpcsec2_op_is_nop) false block *---*/
      
      return(ret_val);
} /*---* end of rpcsec2_cli_check_ret() *---*/


/*---*
 *---* rpcsec2_cli_timed_cnf_str_call(bnd_hndle, type, struct, stat, fstat, 
 *---*                                cstat)
 *---*   
 *---*   Makes a timed call to rpcsec2_cnf_str_()
 *---*/
void
rpcsec2_cli_timed_cnf_str_call(handle_t bnd_hndle, idl_short_int type, 
               long_conf_struct_t *conf_str_p, idl_long_int *stat_p, 
               error_status_t *fstat_p, 
               error_status_t *cstat_p)
{

   /*---* do timing stuff then make the call *---*/

   /*---* get precall time *---*/
   getclock(TIMEOFDAY, &precall_time);
     
   /*---* and call rpcsec2_cnf_str() *---*/
   rpcsec2_cnf_str(bnd_hndle, type, conf_str_p, stat_p, fstat_p, cstat_p);

   /*---* get postcall time *---*/
   getclock(TIMEOFDAY, &postcall_time);
         
   /*---* get elapsed time *---*/
   TVAL_SUB(postcall_time, precall_time)
     
   /*---* set min time *---*/
   TVAL_SETMIN(min_time, postcall_time);

   /*---* set max time *---*/ 
   TVAL_SETMAX(max_time, postcall_time);
   
   /*---* update  elapsed time *---*/        
   TVAL_ADD(elpsd_time, postcall_time);
}

/*---*
 *---* rpcsec2_rep_gen(int)
 *---*    
 *---*   Called from the code if running for x passes, or as a signal 
 *---*   handler for SIGALARM if running for interval x.  Prints a line
 *---*   containing the client id (sequence number passed via the -s option),
 *---*   the machine name, the O/S type, the number of calls, number of passes,
 *---*   number of fails, the protocol, the min, max, and average times for 
 *---*   executing the RPC's, and the date.  Makes use of the global variables
 *---*   for hostname, protocol, client id, os_type, and call_stats.  Note that
 *---*   the prinout is one long line by design to facilitate parsing by an
 *---*   automated caller.
 *---*/
void rpcsec2_rep_gen(int sig)
{
   time_t tbuf;
   double avg;
   char toutbuf1[30], toutbuf2[30], *gcp, *gcp2;
   
   /*---*
    *---* if we are in debug mode...
    *---*/
   if (debug)
      /*---* print hi message *---*/
      printf("Hi from rpcsec2_rep_gen: %s %d...\n", 
                     (sig? "handling signal: ": "called from code: "), sig);
   
   /*---*
    *---* print the info
    *---*/
   (void)time(&tbuf);
   /*---* strip the newline from the time string *---*/
   CTIME_NO_NL(gcp, &tbuf);
   strcpy(toutbuf1, gcp); 
   gcp=toutbuf1;
   CTIME_NO_NL(gcp2, &last_id_refr);

   printf("Cli: %d %s Host: %s O/S: %s Time: %s Protcl: %s Calls: %d Pass: %d Fail: %d Min: %u sec %d nsec Max: %u sec %d nsec Elapsed: %u sec %d nsec Avg: %.9f sec id_refr: %d last: %s\n",
   seq_numb, 
   (calls == 0 && sig == 1? "STARTING":
    (sig == -1? "FINISHED":
     (sig == -2?"DIED":
      (sig == 2? "STOPPED":"RUNNING")))), 
   uname_vals.nodename, uname_vals.sysname, gcp, protocol, calls, passed, 
   failed, min_time.tv_sec, min_time.tv_nsec, max_time.tv_sec, 
   max_time.tv_nsec, elpsd_time.tv_sec, elpsd_time.tv_nsec, 
   (calls?(double)elpsd_time.tv_sec/(double)calls: (double)0), id_refr_cnt,
   gcp2);
   
   /*---* print any errors encountered so far *---*/
   dump_errs(seq_numb, "Client", err_stats, nerr_stats);

   fflush(stdout);

}

void rpcsec2_kill_threads(pthread_t *call_thd)
{
   int i;
   char buf[256];
   time_t tbuf;
   char *t_ptr;

   /*---* report start time for thread cancels *---*/
   (void)time(&tbuf);
   printf("Client(%d): Starting thread cancels at %s\n", seq_numb,
                                                                ctime(&tbuf));

   /*---* cancel all of the threads *---*/
   for (i=0; i < RPCSEC2_CLI_NTHDS; i++)
   {
      printf("Client(%d):    %s...", seq_numb, thread_tbl[i].thd_label);


      /*---* try the thread cancel, but not for the calling thread *---*/
      if (thread_tbl[i].thd_id == (pthread_t *)0)
      {
         printf("Client(%d): %s not created...skipping...\n", seq_numb,
                                          thread_tbl[i].thd_label);
         continue;
      }    

      if (pthread_equal(*call_thd, *(thread_tbl[i].thd_id)))
      {
         printf("Client(%d): calling thread...skipping...\n", seq_numb);
         continue;
      } 
      
      /*---* 
       *---* if pthread_cancel returns non-zero, then it is an error.  
       *---* Create an appropriate message and print it.
       *---*/
      if (pthread_cancel(*(thread_tbl[i].thd_id)))
      {
          sprintf(buf, "\nClient(%d): trying to kill %s thread", seq_numb,
                                                      thread_tbl[i].thd_label);
          perror(buf);
      }
      else
          printf("done.\n");
   }
  
   /*---* report the time we finished the thread cancels *---*/
   (void)time(&tbuf);
   printf("Client(%d): Thread cancels complete at %s\n\n", seq_numb, 
                                                                 ctime(&tbuf));

}



/*---*
 *---* rpcsec2_cli_syncstart(fn, last_cli)
 *---*
 *---*     Looks for a specified file to base time of first RPC on, or 
 *---*     creates file if last_cli is true.  Loads a timespec struct whose
 *---*     addressed is passed in with the number of seconds to sleep before
 *---*     the first RPC.  Returns the address of the struct.
 *---*/
struct timespec *
rpcsec2_cli_syncstart(char *fn, int last_cli, struct timespec *tspec_p)
{
   int fd = 0;
   long tbuf;
   char *gcp;

   (void)time(&tbuf);
   /*---* if we are the last client... *---*/
   if (last_cli)
   {
      /*---* create the file *---*/
      /*---* report, but need to strip the newline from the time string *---*/
      CTIME_NO_NL(gcp, &tbuf)
      printf("Client(%d): CREATing sync file %s at %s...\n", seq_numb,fn,gcp);
      fd = creat(fn, O_RDWR);
      /*---* change mode to read for everybody *---*/
      fchmod(fd, S_IRWXU | S_IRGRP | S_IROTH);
   }
   /*---* else not last client... *---*/
   else
   {
      int did_notice=0;
      /*---* while we can't open the file... *---*/
      while (fd <= 0)
      {
         /*---*
          *---* it should not matter if we use the passed struct for 
          *---* the delay since we are expected to fill it with a value 
          *---* anyway.
          *---*/
         tspec_p->tv_sec = 5;

/*---* allow conditional compilation for use of threads calls *---*/
#if !defined(RPCSEC2_NO_CLI_THDS)
         pthread_delay_np(tspec_p);
#else /*---* !defined(RPCSEC2_NO_CLI_THDS) *---*/
         sleep(tspec_p->tv_sec);
#endif /*---* !defined(RPCSEC2_NO_CLI_THDS) *---*/

         (void)time(&tbuf);
         /*---* if the user wants the blow-by-blow... *---*/
         if (debug || !did_notice)
	 {
            /*---* report *---*/
            /*---*need to strip the newline from the time string *---*/
            CTIME_NO_NL(gcp, &tbuf)
            printf("Client(%d): Looking for sync file %s at %s...\n", 
                                                                  seq_numb, fn,
                                                                          gcp);
            did_notice=1;
	 }
         /*---* try to open the file *---*/
         fd = open(fn, O_RDONLY);
      }
      printf("Client(%d): Found it!...\n",seq_numb);
   }
   /*---* close the file *---*/
   close(fd);
   /*---* return seconds to second five minute boundary from now *---*/
   tspec_p->tv_sec = (start_delay * 2) - (time(NULL) % start_delay);
   return(tspec_p);

}
