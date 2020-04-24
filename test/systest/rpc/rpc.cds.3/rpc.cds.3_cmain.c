/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc.cds.3_cmain.c,v $
 * Revision 1.1.89.2  1996/02/17  23:27:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:55  marty]
 *
 * Revision 1.1.89.1  1995/12/11  22:42:39  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/10  15:59 UTC  jss
 * 	Fix "getting next binding" problem.
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/09  19:27 UTC  bissen
 * 	merge from HPDCE01
 * 	[1995/12/11  22:19:53  root]
 * 
 * Revision 1.1.85.2  1994/05/05  16:21:43  pellis
 * 	Changed debug from short to int, defect 10136.
 * 	[1994/05/05  16:21:21  pellis]
 * 
 * Revision 1.1.85.1  1993/12/22  20:51:17  mhickey
 * 	no_changes
 * 	[1993/12/22  20:50:21  mhickey]
 * 
 * 	fixed an arg type mismatch and a variable to
 * 	digit transformation.
 * 	[1993/12/17  21:03:35  mhickey]
 * 
 * 	fix uninitialized pointer.
 * 	[1993/12/14  19:15:27  mhickey]
 * 
 * 	A number of fixes while fixing 7743
 * 	 - Cleaned up the includes/defines and thread handling
 * 	 - Passes the client arg to sighandler
 * 	 - Added code to set defaults for the cal seq and number from the config file
 * 	   if not specified on the command line
 * 	 - Streamlined checking and conversion of command config file numeric values
 * 	 - Added call to get the server principal name to make intercell work
 * 	 - Fixed bad logic that tried to free calendar data even if the call failed.
 * 	 - Fixed some printfs and comments
 * 	 - Added calls to check_stat to get application errors tracked.
 * 
 * Revision 1.1.83.4  1993/10/20  19:36:49  ohara
 * 	svr4 ref port
 * 	[1993/10/20  19:35:47  ohara]
 * 
 * Revision 1.1.83.3  1993/10/05  19:43:47  cmckeen
 * 	Merged with changes from 1.1.83.2
 * 	[1993/10/05  19:43:39  cmckeen]
 * 
 * 	Fix syntax error
 * 	[1993/10/05  19:03:36  cmckeen]
 * 
 * Revision 1.1.83.2  1993/10/01  16:40:42  mhickey
 * 	This submission contains fixes for the following defects:
 * 
 * 	          Defect 7747: rpc.cds.3 server and client don't renew
 * 	                       tickets early enough
 * 
 * 	                       This is the client side of the fix.  The
 * 	                       client now refreshes it's ticket halfway
 * 	                       through the ticket lifetime.  The change
 * 	                       is in the setting of the variable
 * 	                       'exp_time'.
 * 
 * 	          Defect 7744: rpc.cds.3 server and client fail if some
 * 	                       command line options are not specified.
 * 
 * 	                       This is the client side of the fix. The
 * 	                       client now reads the defaults for the
 * 	                       starting calendar number, and number of calendars
 * 	                       from the config file, and defaults the data
 * 	                       directory to '.'  If the values are not specified
 * 	                       on the command line, the defaults are used.
 * 	                       The changes are the setting of 'cal_d_dir to '.',
 * 	                       the checks to see if 'cdscalseq' and 'ncdscals'
 * 	                       are set, and the conversion and assignment of
 * 	                       the defaults if they are not.
 * 	[1993/10/01  15:21:15  mhickey]
 * 
 * Revision 1.1.83.1  1993/09/14  15:01:09  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/14  15:00:31  cmckeen]
 * 
 * Revision 1.1.7.2  1993/08/19  15:44:31  bissen
 * 	Fixed for hp-ux
 * 	[1993/08/19  15:43:08  bissen]
 * 
 * Revision 1.1.2.3  1993/03/12  18:55:36  mhickey
 * 	Fixing bsubmit mystery meat
 * 	[1993/03/12  18:50:57  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* rpc.cds.3_cmain.c - client side of rpc.cds.3 test
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the rpc.cds.3 DCE System Test
 *---*
 *---*    The rpc.cds.3 system test corresponds to test rpc.cds.3 in the 
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the rpc.cds.3 system test, see the test 
 *---*    plan, or the README file in this directory.
 *---* 
 *---* FUNCTION
 *---*    Performs namespace lookups for bindings, and then makes RPC's to a 
 *---*    server, receiving a conformant struct of strings describing a 
 *---*    primitive calendar application's data.  Each RPC is done with a new
 *---*    binding, obtained by NSI lookup using an object UUID referring to a
 *---*    specific calendar object exported to the namespace by the server.  In
 *---*    other words, the server internally associates specific UUIDs with 
 *---*    the calendars it manages.  It then creates CDS namespace entries for
 *---*    the calendars, and exports the UUID that it internally associates 
 *---*    with each calendar to the corresponding CDS entry (these entries are
 *---*    the "calendar objects").  The client, in order to import a certain
 *---*    calendar, uses the name to find the CDS object; from the CDS object
 *---*    it obtains the UUID that the server associates with one of it's
 *---*    calendars.  Then the client uses the object UUID to get a binding
 *---*    to the server.  When the client uses the binding containing the UUID
 *---*    to make a call to the server, the server manager code sees the UUID,
 *---*    recognizes it as one that it associates with a calendar, and returns
 *---*    the coresponding calendar's data to the client.
 *---*
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
 *---*           the -p option have been made.  The -i option specifies the 
 *---*           interval at which one line report lines are to be printed on
 *---*           standard output.  If the -r option is specified, the value 
 *---*           specified by the -i option is taken to be the number of calls
 *---*           to make between each report generation.  If the -h and/or -m
 *---*           options are specified, then the value specified by the -i
 *---*           option is taken to be the interval in minutes between each 
 *---*           report generation.  If the -h or -m option is specified and 
 *---*           the -i option is not specified, the report interval is the 
 *---*           value assigned in the config file to the variable 
 *---*           RPCCDS3_CLI_DEF_REP_INTVL.
 *---*
 *---*    Note that the defaults for the program are to run for
 *---*    RPCCDS3_DEF_RUN_INTVL hours, generating report lines every 
 *---*    RPCCDS3_DEF_REP_INTVL minutes.
 *---*
 *---*    The following is a list of the steps in the program:
 *---*
 *---*    1. Gets values for operational parameters by reading the config 
 *---*       file (rpc.cds.3.conf or passed via the -f command line option).
 *---*    2. Assumes the client principal identity specified in the config 
 *---*       file.
 *---*    3. Sets various strings for reporting, such as hostname and operating
 *---*       system.
 *---*    4. Looks in the CDS namespace for an existing entry for the rpc.cds.3
 *---*       interface (defined in the config file).  If a CDS namespace
 *---*       entry is found, then all the binding handles that 
 *---*       rpc_binding_import_next will return are sequentially imported,
 *---*       and an rpc_mgmt_is_server_listening call is made to
 *---*       verify that the binding is usable.  This step ensures that the 
 *---*       later steps will find some usable bindings at the server, and 
 *---*       that the server is alive.
 *---*    5. The first of rIf a protocol is specified, then the binding handle is converted 
 *---*       to a string binding and then parsed to find the protocol type.
 *---*    6. The process looks for the sync file and sleeps after finding it
 *---*       to syncronize startup of it's RPC's.
 *---*    7. The client makes the RPC.  If the -b flag is specified, then
 *---*       the client checks the return status from the call and exits with
 *---*       the appropriate value as described above.  If the client is 
 *---*       running in stress mode, then the status is checked, counts of 
 *---*       successes, failures, total calls and call times are updated, and
 *---*       the next RPC is made.
 *---* 
 *---* CALLED BY
 *---*    rpc.cds.3_runtest.sh - (the driver for running the test program)
 *---* 
 *---* CALLS INTO 
 *---*    Reads rpc.cds.3.conf or file specified via the -f option for 
 *---*    definitions.
 *---*    rpc.cds.3_chk_stat.c  (rpcsec2_check_stat() )
 *---*    rpc.cds.3_defs.h      (return values and data structures)
 *---*    DCE APIS
 *---*
 *---* PREREQUISITES
 *---*    Assumes a running DCE cell on the system that has been setup with 
 *---*    the script rpc.cds.3_setup.sh.
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
#include <pthread.h>
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

#include <sys/stat.h>

/*---------------------------*
 *---* Private include files
 *---------------------------*/
#include <rpc_cds_3.h>
#include <rpc.cds.3_defs.h>
#include "../rpc.sec.2/rpctest.h"



/*-------------------------------------------*
 *---* Global Declarations
 *-------------------------------------------*/
/*--------------------------------*
 *---* Command line option flags 
 *--------------------------------*/
short nflag,   /*---* use nil uuid          *---*/
      bflag,   /*---* boundary mode         *---*/
      Nflag,   /*---* use NULL for UUID     *---*/
      nflag,   /*---* make null calls (for rpc performace testing *---*/
      aflag,   /*---* automatically retry other bindings if call fails *---*/
      Lflag,   /*---* indicates we are the last client started         *---*/
      sflag,   /*---* did we get a sequence number?                    *---*/
      tflag,   /*---* track and report on time of calls                *---*/
      uflag;   /*---* used passed string UUID                          *---*/
int   debug;   /*---* take a guess!                                    *---*/

/*---------------------------*
 *---* Usage Message
 *---------------------------*/
char *usage = "usage: rpc.cds.3_cli [-S sync_file] -f config_file [-P datagram | connection] [-a age] [-b] [-d] [-n ncals] [-s seq_num] [-v size] [-i report_interval] [-L] [-l prot_lev] [[-p passes] | [-h hours_to_run] [-m minutes_to_run]] [-t]";

/*----------------------------------------------* 
 *---* Call statistics variables.  These are global since we can't pass 
 *---* stuff to the signal handler that needs them.
 *----------------------------------------------*/
/*---* Counters *---*/
int  calls,         /*---* total calls                        *---*/
     id_refr_cnt,   /*---* number of times id refreshed       *---*/
     passed,        /*---* successes                          *---*/
     failed;        /*---* failures                           *---*/

/*---* Timer variables *---*/
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
#define RPCCDS3_MAIN_THD  0
#define RPCCDS3_REFR_THD  1
#define RPCCDS3_SIG_THD   2
#define RPCCDS3_REP_THD   3
#define RPCCDS3_CLI_NTHDS 4

/*---* array of threads we spawn *---*/
pthread_t thd_array[RPCCDS3_CLI_NTHDS];

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



/*------------------------------------------*
 *---* Calendar data management
 *------------------------------------------*/
/*---*
 *---* base of the calendars array and an indexing pointer
 *---*/
cal_entry_p_t calendars, calendars_p;

/*---*
 *---* number of calendars the server exports
 *---*/
int ncalendars;

/*---* pointer to returned value is public to avoid changing interface to 
 *---* rpccds3_chk_cli_stat
 *---*/
calndr_data_p_t ret_cal_data_p;



/*---------------------------------------------------*
 *---* other variables used outside of main()
 *---------------------------------------------------*/
char *protocol;             /*---* what protocol is our binding       *---*/
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
                              {"RPCCDS3_CLI_PRINC_NAME", 0},
                              {"RPCCDS3_SRV_PRINC_NAME", 0},
                              {"RPCCDS3_SRV_CDS_NAME", 0},
                              {"RPCCDS3_SRV_OBJ_DIR", 0},
                              {"RPCCDS3_SRV_CAL_DATA", 0},
                              {"RPCCDS3_SRV_NCALS", 0},
                              {"RPCCDS3_SRV_CALSEQ", 0},
                              {"RPCCDS3_CLI_KEYTAB_DIRPATH", 0},
                              {"RPCCDS3_CLI_KEYTAB_FN",0},
                              {"RPCCDS3_CLI_DEF_RUN_INTVL", 0},
                              {"RPCCDS3_CLI_DEF_REP_INTVL", 0},
                              {"RPCCDS3_CLI_SYNC_DELAY", 0},
                              {"RPCCDS3_CLI_CDS_CACHE_AGE", 0},
                              {"RPCCDS3_PROT_LEVEL", 0}
                            };

/*---*
 *---* enum for accessing the params array using macros in rpc.cds.3_defs.h
 *---* (actually the only one we use is PVAL()).
 *---*/
enum {
       cli_princ,
       srv_princ,
       cds_name,
       obj_dir,
       cal_data,
       cdscals,
       calseq,
       keytab_dir,
       keytab_fn,
       run_int,
       rep_int,
       delay,
       cds_cache_age,
       prot_lev
} param_i;


/*-------------------------------------------*
 *---* Function Prototypes 
 *-------------------------------------------*/
int rpccds3_cli_check_ret(unsigned32, error_status_t, error_status_t, short);
void maintain_id(int *);
void rep_timer(int *);
void dump_key(sec_passwd_rec_t *);
struct timespec *rpccds3_cli_syncstart(char *, int, struct timespec *);
int signal_catcher (char *);
void rpccds3_cli_kill_threads(pthread_t *);
void rpccds3_cli_rep_gen(int);
calndr_data_p_t rpccds3_cli_timed_send_cal_call(rpc_binding_handle_t, 
                                                idl_long_int *, 
                                                error_status_t *,
                                                error_status_t *);
int rpccds3_cal_comp(void *, void *);

void rpccds3_get_binding_by_uuid(char *, char *, rpc_binding_handle_t *,  
                                                                 unsigned32 *);
void dump_binding(rpc_binding_handle_t);


/*-------------------------------------------------*
 *---* pointers to functions for external routines
 *-------------------------------------------------*/
/*---* report generation function *---*/
void (*rep_gen_func_p)() = rpccds3_cli_rep_gen;
/*---* thread cancel funtion      *---*/
void (*cleanup_func_p)() = rpccds3_cli_kill_threads;


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
                        comm_stat = rpc_s_ok,  /*---* comm status var    *---*/
                        cache_age=0;           /*---* max age of cds cache 
                                                *---* data               *---*/
   unsigned_char_p_t    srv_pname;           /*---* srvr principal name *---*/
   signed32             exp_time;
   uuid_t               uuid;                /*---* storage for uuid   *---*/
   rpc_ns_handle_t      imprt_ctxt;          /*---* import context     *---*/
   rpc_binding_handle_t bndng_hndle;         /*---* server bind handle *---*/
   idl_small_int        val = 0;             /*---* value for rem. call *--*/
   unsigned_char_p_t    str_p;               /*---* generic pointer    *---*/
   int                  pid=getpid(),        /*---* pid for id         *---*/
                        rep_intvl=0,         /*---* reporting interval *---*/
                        run_intvl,           /*---* time to run        *---*/
                        hours = 0,           /*---* hours to run       *---*/
                        mins = 0,            /*---* minutes to run     *---*/
                        done = 0,
                        end_time=0,          /*---* End time - seconds *---*/
                        ncdscals=0,          /*---* number of cals available
                                              *---* to try to   import *---*/
                        cdscalseq=0,         /*---* starting sequence number 
                                              *---* for calendars *---*/
                        uuidsdone=0,     
                        currseq,
                        curr_cal,
                        passes = 0,          /*---* passes to run      *---*/
                        onegood,             /*---* got one good binding *---*/
                        i;                   /*---* generic counter    *---*/
   char                 *gcp,                /*---* generic char ptr   *---*/
                        *protect_level_str,  /*---* string rep. of same *--*/
                        ubuf[15],            /*---* utility buffer     *---*/
                        resp[2],             /*---* buff for response  *---*/
                        *keytab,             /*---* pointer to keytab name *-*/
                        *cal_cds_name,       /*---* points to name in cds *--*/
                        *srv_cds_name,       /*---* guess!? *---*/
                        *sig_hndlr_arg="client",
                        *sync_fn=NULL,          /*---* synchronization    
                                                *---* file name          *---*/
                        *cal_data_f=NULL,
                        *cal_d_dir = ".",    /*---* directory where calendar 
                                              *---* data files are located.
                                              *---* Default is the current
                                              *---* directory. 
                                              *---*/
                        *conf_file=NULL;     /*---* pointer to config filename 
                                              *---* if passed on command line
                                              *---*/
   short                comm_fault_prob=0;   /*---* general fault flag *---*/
   sec_login_auth_src_t auth_src;     /*---* receives auth src info from call
                                       *---* to sec_login_validate_identity
                                       *---*/
   boolean32            reset_passwd;   /*---* receives flag value indicating
                                         *---* whether passwd has expired.
                                         *---*/

extern char     *rd_conf_errs[], /*---*  rd conf error strings
                                          *---*/
               *rpccds3_send_cal_errs[], /*---* error strings from 
                                          *---* the RPC
                                          *---*/
         *rpccds3_parse_cal_spec_errs[]; /*---* error strings from the 
                                          *---* calendar data loader.
                                          *---*/

   time_t               tbuf;
   calndr_data_p_t (*rpccds3_cli_call_p)() = 
           (calndr_data_p_t (*)())rpccds3_send_cal; /*---* the func called *-*/


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
	    case 'D': /*---* directory for calendar data files *---*/
                      if ( ! *(argv+1))
		      {
                         /*---* error and usage *---*/
                         printf("Client(%d): no value for calendar directory name.\n", seq_numb);
                         printf("Client(%d): %s\n", seq_numb, usage);
                         exit(1);
		      }
                      cal_d_dir = *++argv;
                      break;
                      
            case 'c': /*---* calendar data file list *---*/
                      cal_data_f = *++argv;
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
            case 'n': /*---* number of calendars to export to cds *---*/
                      /*---* FALLTHROUGH *---*/
            case 'a': /*---* age in minutes for cds cache data *---*/
                      /*---* FALLTHROUGH *---*/
	    case 'I': /*---* initial sequence number of cds cals *---*/
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
                                   (*((*argv)+1) == 'n'? &ncdscals: 
                                   (*((*argv)+1) == 'I'? &cdscalseq: 
                                   (*((*argv)+1) == 'a'? (int *)&cache_age: 
                                   (*((*argv)+1) == 'p'?&passes: 
                                                        &rep_intvl))))));

                         /*---* 
                          *---* convert value to decimal and store in the 
                          *---* right variable
                          *---*/
                         sscanf(*++argv, "%d", i_p);
                         /*---* if appropriate, print a message *---*/
                         if (*((*(argv-1))+1) == 's')
                            printf("Client(%d): Sequence number is %d...\n",
                                                          seq_numb, seq_numb);
                         else if (*((*(argv-1))+1) == 'p')
                            printf("Client(%d): Executing for %d passes...\n",
                                                          seq_numb, passes);
                         else if (*((*(argv-1))+1) == 'I')
                            printf("Client(%d): Initial cds calendar: %d...\n",
                                                        seq_numb, cdscalseq);
                         else if (*((*(argv-1))+1) == 'n')
                            printf(
                                 "Client(%d): Importing %d cds calendars...\n",
                                                        seq_numb, ncdscals);
 
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
                      rpccds3_cli_call_p = 
                        (calndr_data_p_t (*)())rpccds3_cli_timed_send_cal_call;
                      break;

 
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
                                    (conf_file? conf_file: RPCCDS3_CONF_FN));
   rd_conf((conf_file? conf_file: RPCCDS3_CONF_FN), params, 
                   sizeof(params)/sizeof(param_spec_val_t), &ret_stat);
   /*---* if we did not succeed... *---*/
   if (ret_stat != rdconf_ok)
   {
      /*---* report error *---*/
      printf("\n\nClient(%d): %s (%s).\n", seq_numb, 
                                           rd_conf_errs[ret_stat],
                                     (conf_file? conf_file: RPCCDS3_CONF_FN));
      /*---* exit *---*/
      exit(2);
   }
   /*---* report success *---*/
   printf("\ndone.\n\n");

   /*---* convert the start_delay param *---*/
   int_from_PVAL(&start_delay,params,param_i,delay,"start delay",ret_stat);
   if (ret_stat)
       exit(2);

   /*---*
    *---* set the initial cds number from the config file value if not set
    *---* on command line 
    *---*/
   if (!cdscalseq)
   {
      /*---* try to convert the param from the config file *---*/
      int_from_PVAL(&cdscalseq,params,param_i,calseq,"cal sequence",ret_stat);
      if (ret_stat)
         exit(2);
   }

   /*---*
    *---* set the max cds number from the config file value if not set
    *---* on command line 
    *---*/
   if (!ncdscals)
   {
      /*---* try to convert the param from the config file *---*/
      int_from_PVAL(&ncdscals,params,param_i,cdscals,"cal range",ret_stat);
      if (ret_stat)
         exit(2);
   }

   printf("Client(%d): Starting cal sequence: %d...\n", seq_numb,cdscalseq);
   printf("Client(%d): Exporting %d cds calendars...\n", seq_numb,ncdscals);

   /*---* set the protection level *---*/
   if (protect_level_str)
      gcp = protect_level_str;
   else
      gcp = PVAL(params, param_i, prot_lev);

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

   /*---* Calculate and report on cache aging parameter *---*/
   if (cache_age == 0)
   {
      /*---* 
       *---* not specified by the user, so use the default from the config file
       *---*/
      int_from_PVAL((int *)&cache_age,params,param_i,cds_cache_age,"cache age",ret_stat);
      if (ret_stat)
         exit(2);
   }
   /*---* 
    *---* at this point, we should have a value for minutes that CDS cache data
    *---* is considered valid for.  Print, then convert to seconds for the 
    *---* actual call.
    *---*/
   printf("Client(%d): Cache data will be invalidated after %u minutes...\n",
                                                          seq_numb, cache_age);
   cache_age *= 60;

   /*---* if not in boundary mode... *---*/
   if (!bflag)
   {
      /*---* set the reporting interval (if not set on command line) *---*/
      if (!rep_intvl)
      {
         int_from_PVAL(&rep_intvl,params,param_i,rep_int,"reporting interval",ret_stat);
         if (ret_stat)
            exit(2);
      }

      /*---* if executing for some number of passes... *---*/
      if (passes)
      {
         /*---* tell the user when we will report... *---*/
         printf("Client(%d): printing status after every %d passes...\n",
                                                 seq_numb, rep_intvl);
      }
      /*---* else this is a timed run... *---*/
      else
      {
         /*---* get hours and minutes set *---*/
         if (!(hours || mins))
	 {
            int_from_PVAL(&hours,params,param_i,run_int,"run duration",ret_stat);
            if (ret_stat)
               exit(2);
	 }
         /*---* report to the user *---*/
         printf("Client(%d): Executing for %d hours and %d mins...\n",
                                                       seq_numb, hours, mins);
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
         if (pthread_cond_init(&rep_timer_cond,pthread_condattr_default)!=0)
         {
            perror(" \n  Error initializing reporting condition variable:");
            exit(1);
         }
         printf("done.\n");

         /*---*
          *---* initialize the reporting mutex
          *---*/
         printf("Client(%d): Initializing the reporting mutex...",seq_numb);
         if (pthread_mutex_init(&rep_timer_mutex,
                                              pthread_mutexattr_default)!=0)
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
   keytab = (char *)malloc(strlen(PVAL(params, param_i, keytab_dir)) +
                           strlen(PVAL(params, param_i, keytab_fn))  + 2 );
   /*---* fill in the string *---*/
   if (debug)
   {
      printf("keytab dir = %s\n", PVAL(params, param_i, keytab_dir));
      printf("keytab file = %s\n", PVAL(params, param_i, keytab_fn));
      printf("keytab path = %s", PVAL(params, param_i, keytab_dir));
      printf("/%s\n", PVAL(params, param_i, keytab_fn));
   }
   sprintf(keytab, "%s/", PVAL(params, param_i, keytab_dir));
   strcat(keytab, PVAL(params, param_i, keytab_fn));

   /*---* for the efficiency freak in me ...*---*/
   free(PVAL(params, param_i, keytab_dir));
   free(PVAL(params, param_i, keytab_fn));

#if defined(RPCCDS3_DO_LOGIN)
   /*---* Create a context and get the login contex loaded *---*/
   printf("Client(%d): create login context...(%s)", seq_numb,
                              PVAL(params, param_i,cli_princ));
   sec_login_setup_identity((unsigned_char_t *)PVAL(params, param_i,cli_princ),
                            (sec_login_flags_t)sec_login_no_flags,
                            &login_ctxt, &ret_stat);
   
   if (rpcsec2_check_stat(ret_stat, "creating login context",
                                       err_stats, &nerr_stats, debug,0))
      exit(1);

   /*---* Get our secret key from our keytab file *---*/
   printf("Client(%d): getting secret key from %s...", seq_numb, keytab);
   sec_key_mgmt_get_key(rpc_c_authn_dce_secret, (void *)keytab,
                        (char *)PVAL(params, param_i,cli_princ), (unsigned32)0, 
                        &sec_key_p, &ret_stat);

   if (rpcsec2_check_stat(ret_stat, "getting secret key",
                                       err_stats, &nerr_stats, debug,0))
      exit(1);

   if (debug)
      dump_key((sec_passwd_rec_t *)sec_key_p);

   /*---* 
    *---* since sec_login validate nulls the key data to prevent resuse of the
    *---* key structure, we store the data in a global array, and fix the 
    *---* struct in the refresh thread.
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

   /*---* finally, set our context *---*/
   printf("Client(%d): setting our own context...", seq_numb);
   sec_login_set_context(login_ctxt, &ret_stat);

   if (rpcsec2_check_stat(ret_stat, "setting our own context",
                                       err_stats, &nerr_stats, debug, 0))
      exit(1);

#else /*---* ! defined(RPCCSD3_DO_LOGIN) *---*/
   /*---*
    *---* if not establishing our own identity, just get the 
    *---* identity under which we were invoked.
    *---*/
   printf("Client(%d): Using default context...getting current context...",
                                                                    seq_numb);
   sec_login_get_current_context(&login_ctxt, &ret_stat);
   if (rpcsec2_check_stat(ret_stat, "getting login context",
                              err_stats, &nerr_stats,  debug, 0))
      exit(1);
#endif /*---* if defined(RPCCDS3_DO_LOGIN) *---*/

#if defined(RPCCDS3_AUTO_REFRESH) 
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
                                                     &nerr_stats, debug, 1))
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
        *---* Set the interval to wait before refreshing the indentity.  We
        *---* used to allow a one minute buffer for the refresh to happen. Now
        *---* (based on experience) we use half the ticket lifetime.
        *---*/
       CTIME_NO_NL(_cp1, &exp_time)
       exp_time -= time(&_tbuf);
       CTIME_NO_NL(_cp2, &_tbuf)
       /*---* round up if necessary *---*/
       exp_time += ((expdif = exp_time%60)? 60 - expdif: 0);
       id_lifetime = exp_time / 2;
       printf("Client(%d): identity lifetime = %d mins, refresh interval is %d mins.\n", seq_numb, exp_time/60, id_lifetime/60);
       printf("Client(%d): Spawning refresh thread...", seq_numb);
       USE_PTHREAD(thread_tbl, thd_array, RPCCDS3_REFR_THD);
       if (pthread_create(thread_tbl[RPCCDS3_REFR_THD].thd_id, 
           pthread_attr_default, 
           (pthread_startroutine_t)maintain_id, 
           (pthread_addr_t)&id_lifetime) != 0 )
       {
          perror(" \n    Error creating refresh thread:");
          exit(1);
       }
       printf("done.\n");

   }
#endif /*---* defined(RPCCDS3_AUTO_REFRESH) *---*/

   /*---* store our main thread id *---*/
   USE_PTHREAD(thread_tbl, thd_array, RPCCDS3_MAIN_THD);
   *(thread_tbl[RPCCDS3_MAIN_THD].thd_id) = pthread_self();

   /*---*
    *---* create the thread that will handle signals
    *---*/
   printf("Client(%d): Spawning signal catcher thread...", seq_numb);
   USE_PTHREAD(thread_tbl, thd_array, RPCCDS3_SIG_THD);
   if (pthread_create(thread_tbl[RPCCDS3_SIG_THD].thd_id, 
       pthread_attr_default, 
       (pthread_startroutine_t)signal_catcher, 
       (pthread_addr_t)sig_hndlr_arg) != 0 )
   {
      perror(" \n    Error creating signal catcher thread:");
      rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }
   printf("done.\n");
   

   /*---* 
    *---* setup for call to rpccds3_send_cal
    *---* 
    *---* we call rpccds3_parse_cal_spec to parse the spec we got from the
    *---* configuration file or the command line (actually just a space 
    *---* separated list of calendr data files).
    *---*
    *---* After the calendars are loaded, we need to select one,  then
    *---* look for a binding for it.
    *---*/
   printf("Client(%d): Loading calendar data...", seq_numb);
   
   rpccds3_parse_cal_spec(cal_d_dir, 
               cal_data_f? cal_data_f: PVAL(params, param_i, cal_data),
               &calendars, &ncalendars, debug, &ret_stat);
   if (ret_stat != rpccds3_parse_cal_spec_ok)
   {
      printf("Client: error reading calendar data: %s.\n", 
                                        rpccds3_parse_cal_spec_errs[ret_stat]);
      exit(1);
   }
  
   for (calendars_p = calendars; calendars_p - calendars < ncalendars; 
                                                               calendars_p++)
   if (debug)
   {
      printf("Client: Data for calendar %s is:\n", 
                                         (calendars_p->cal_data)->cal_name);
           printf("     lines: %d\n", (calendars_p->cal_data)->cal_nlines);
           for (i=0; i < (calendars_p->cal_data)->cal_nlines;  
            printf("     line %d: %s\n", i, 
                                  ((calendars_p->cal_data)->cal_lines)[i++]));
   }

   /*---* sort the calendars to make searches faster *---*/
   qsort((void *)calendars, (size_t)ncalendars, (size_t)sizeof(cal_entry_t),
         (int(*)())rpccds3_cal_comp); 


   /*---* set the CDS cache expiration time *---*/
   printf("Client(%d): Setting the CDS cache expiration interval...",seq_numb);
   rpc_ns_mgmt_set_exp_age(cache_age, &ret_stat);
   if (rpcsec2_check_stat(ret_stat, "Setting CDS cache expiration age",
                                       err_stats, &nerr_stats, debug, 0))
   {
      rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }

   if (debug)
   {
      unsigned32 check_age;

      /*---* if debugging, check the expiration age to verify it is set *---*/
      rpc_ns_mgmt_inq_exp_age(&check_age, &ret_stat);
      if (rpcsec2_check_stat(ret_stat, "Getting CDS cache expiration age",
                                       err_stats, &nerr_stats, debug, 0))
      {
         rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
         exit(1);
      }
      printf("Client(%d): CDS cache expiration age is %d...\n", seq_numb,
                                                                check_age/60);
   }
      
   /*-------------------------------------------------------------*
    *---* look for bindings for the rpccds3 interface 
    *-------------------------------------------------------------*/
   /*---* 
    *---* Test the bindings the server exports
    *---*/
   printf("Client(%d): Trying to import binding handles for %s...", seq_numb,
                                             PVAL(params, param_i,cds_name));
   rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce, 
                          (unsigned_char_p_t)PVAL(params, param_i,cds_name), 
                               rpccds3_v1_0_c_ifspec, 
                               (uuid_p_t)NULL, &imprt_ctxt,
                               &ret_stat);

   if (rpcsec2_check_stat(ret_stat, "importing binding handles",
                                       err_stats, &nerr_stats, debug, 0))
   {
      rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }

   /*---* initialize indicator of one good binding *---*/
   onegood=0;
   while(ret_stat == rpc_s_ok )
   {
      int do_check = 1;
      unsigned32 stat;

      /*---* get a (the) binding handle associated with the interface *---*/
      printf("Client(%d): get next binding handle...", seq_numb);
      rpc_ns_binding_import_next(imprt_ctxt, &bndng_hndle, &ret_stat);   

/***
      rpcsec2_check_stat(ret_stat, "getting next binding handle",
                                            err_stats, &nerr_stats, debug, 0);
***/
      if (ret_stat == rpc_s_no_more_bindings)
         continue;
      else if (ret_stat != rpc_s_ok)
      {
	 rpcsec2_check_stat(ret_stat, "getting next binding handle",
			    err_stats, &nerr_stats, debug, 0);
         rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
         exit(1);
      }
     
      /*---* Convert the binding to a string binding since we may need the
       *---* string binding later to check the protocol, and it will be handy
       *---* for printing messages.
       *---*/
      printf("Client(%d): converting binding handle...", seq_numb);
      rpc_binding_to_string_binding(bndng_hndle, &str_p, &stat);
      if (rpcsec2_check_stat(stat, "converting binding to string",
                                           err_stats, &nerr_stats, debug, 0) )
         str_p = (unsigned_char_t *) "Could not convert binding";

      else if (protocol || debug || bflag)
      {
         unsigned_char_t *locstr_p;

         /*---* if we are in debug mode... *---*/
         if (debug)
            /*---* print it *--*/
            printf("Client(%d): got %s\n", seq_numb,str_p);

         if (protocol || bflag)
         {
            /*---* parse binding to see if it's of the right type *---*/
            printf("Client(%d): Parsing string binding...", seq_numb);
            rpc_string_binding_parse(str_p, (unsigned_char_t **)NULL,
                                      &locstr_p, (unsigned_char_t **)NULL,
                                      (unsigned_char_t **)NULL,
                                      (unsigned_char_t **)NULL, &stat);
            if (rpcsec2_check_stat(stat, "parsing string binding",
                                  err_stats, &nerr_stats, debug, 0) == 0)
            {
               if (!bflag && strcmp(locstr_p, protocol)) 
                  /*---* don't check if not right protocol binding *---*/
                  do_check = 0;
               else
                  protocol = protocol? protocol: 
                            (strcmp(locstr_p,"ncacn_ip_tcp")?"ncadg_ip_udp":
                                                             "ncacn_ip_tcp");
                                        
               /*---* free the protocol string *---*/
               printf("Client(%d): freeing protocol string...", seq_numb);
               rpc_string_free(&locstr_p, &stat);
               if (rpcsec2_check_stat(stat, "freeing protocol string",
                                            err_stats, &nerr_stats,debug,0))
                  printf("Client(%d): leaking %d bytes of memory...\n", 
                                                seq_numb, strlen(locstr_p));
            }  /*---* if we parsed binding *---*/
         } /*---* if we are checking protocol *---*/

      }  /*---* if we converted the binding to a string *---*/

      /*---* 
       *---* check to make sure this is a usable binding.  We 
       *---* first need to convert the partially bound handle to a fully
       *---* bound handle for the rpc_mgmt_is_server_listening call.
       *---*/
      if (do_check == 1)
      {
         printf("Client(%d): converting partial to full binding...", seq_numb);
         rpc_ep_resolve_binding(bndng_hndle, rpccds3_v1_0_c_ifspec, &stat);
         if (!rpcsec2_check_stat(stat,"converting to full binding",err_stats,
                                                          &nerr_stats,debug,0))
         {
            printf("Client(%d): Checking binding conectivity...", seq_numb);
            rpc_mgmt_is_server_listening(bndng_hndle, &stat);
            if (rpcsec2_check_stat(stat, "checking binding", err_stats, 
                                                     &nerr_stats,debug,0) == 0)
            {
               onegood++;
               printf("Client(%d): verified binding %s...\n", seq_numb, str_p);
            }
            else
               printf("Client(%d): dead binding %s...\n", seq_numb, str_p);
         }
         else
            printf("Client(%d): could not check connectivity...\n", seq_numb);
      }
      if (strcmp(str_p, "Could not convert binding"))
      {
         /*---* free the memory *---*/            
         printf("Client(%d): freeing binding handle string...",seq_numb);
         rpc_string_free(&str_p, &stat);
     
         if (rpcsec2_check_stat(stat, "freeing binding string",
                                           err_stats, &nerr_stats,debug,0))
            printf("Client(%d): Leaking memory :-{\n", seq_numb);
      }
      
      /*---* free the binding *---*/
      printf("Client(%d): freeing binding...", seq_numb);
      rpc_binding_free(&bndng_hndle, &stat);
      if (rpcsec2_check_stat(stat, "freeing binding string", err_stats, 
                                                          &nerr_stats,debug,0))
         printf("Client(%d): Leaking memory :-{ (sizeof(binding))\n",seq_numb);
      
   } /*---* while (ret_stat == rpc_s_ok ) *---*/

   /*---* free the resources *---*/
   printf("Client(%d): Freeing import context...", seq_numb);
   rpc_ns_binding_import_done(&imprt_ctxt, &ret_stat);
   if (rpcsec2_check_stat(ret_stat, "freeing inquiry context",
                                            err_stats, &nerr_stats, debug, 0))
      /*---* 
       *---* if we can't free the inquiry context, we warn the user, 
       *---* but we don't consider it a fatal problem.
       *---*/
      printf("Client(%d): leaking memory :-( (inq_ctxt)\n", seq_numb);

   /*---* if we didn't get a binding... *---*/
   if (!onegood)
   {
      /*---* error and out *---*/
      printf("Client(%d): couldn't find a live binding!\n");
      rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(2);
   }
   else
      printf("Client(%d): found %d good bindings at the server.\n", seq_numb,
                                                                      onegood);

   /*---*
    *---*  get a binding through the first calendar we are to import 
    *---*  First we have to construct the name of the calendar 
    *---*/
   currseq = cdscalseq;
   if (!(cal_cds_name = (char *)malloc(strlen(PVAL(params, param_i, obj_dir)) +
                                       strlen("rpccds3_cal") + 10)))
   { 
      printf("Client(%d): couldn't get space for calendar cds names.\n",
                                                                    seq_numb);
      rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(2);
   }
  
   sprintf(cal_cds_name, "%s/rpccds3_cal%d",PVAL(params, param_i, obj_dir),
                                                                   currseq++);
  
   /*---* now we can call rpccds3_get_handle_by_uuid *---*/
   printf("Client(%d): Getting inital binding through %s...", seq_numb, 
                                                                 cal_cds_name);
   rpccds3_get_binding_by_uuid(cal_cds_name, protocol, &bndng_hndle,&ret_stat);

   if (rpcsec2_check_stat(ret_stat, "getting initial binding by uuid",
                                             err_stats, &nerr_stats, debug,0))
   {
      rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(2);
   }

   if (debug)
      dump_binding(bndng_hndle);

   /*---* 
    *---* check to make sure this is a usable binding.  We 
    *---* first need to convert the partially bound handle to a fully
    *---* bound handle for the rpc_mgmt_is_server_listening call.
    *---*/
   printf("Client(%d): converting partial to full binding...", seq_numb);
   rpc_ep_resolve_binding(bndng_hndle, rpccds3_v1_0_c_ifspec, &ret_stat);
   if (!rpcsec2_check_stat(ret_stat,"converting to full binding",err_stats,
                                                       &nerr_stats,debug,0))
   {
      printf("Client(%d): Checking binding conectivity...", seq_numb);
      rpc_mgmt_is_server_listening(bndng_hndle, &ret_stat);
      if (rpcsec2_check_stat(ret_stat, "checking binding", err_stats, 
                                                  &nerr_stats,debug,0) == 0)
         printf("Client(%d): verified inital binding...\n",seq_numb);
      else
         printf("Client(%d): dead inital binding...\n", seq_numb);
   }
   else
      printf("Client(%d): could not check connectivity...\n", seq_numb);

   /*---* need to get the server name to put in the set auth info call *---*/
   printf("Client(%d): Getting server name...", seq_numb);
   rpc_mgmt_inq_server_princ_name(bndng_hndle,
                                  (unsigned32)rpc_c_authn_dce_secret,
                                  &srv_pname,
                                  &ret_stat);
   if (rpcsec2_check_stat(ret_stat, "getting server name",
                                       err_stats, &nerr_stats, debug,0))
   {
      rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(2);
   }
   /*---* report server name, *---*/
   printf("Client(%d): Server principal name is %s...\n",seq_numb, 
                                                        (char *)srv_pname);

   /*---* specify authz and authn information *---*/
   printf("Client(%d): setting authz and authn info...",seq_numb);
      
   rpc_binding_set_auth_info(bndng_hndle, srv_pname, protect_level,
                             (unsigned32)rpc_c_authn_dce_secret, 
                             (rpc_auth_identity_handle_t)login_ctxt,
                             (unsigned32)rpc_c_authz_name,
                             &ret_stat);

   if (rpcsec2_check_stat(ret_stat, "setting auth info",
                                       err_stats, &nerr_stats, debug,0))
   {
      rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(2);
   }

   /*---* show our ready time *---*/
   time(&tbuf);
   CTIME_NO_NL(gcp, &tbuf)
 
   protocol = protocol? protocol: "any";

   /*---* print the READY message that the driver script looks for  *---*/
   printf("Client( %d ): READY to start making calls Host: %s OS: %s Protocol: %s at %s...\n", seq_numb, uname_vals.nodename, uname_vals.sysname, 
                                                                protocol, gcp);
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
      USE_PTHREAD(thread_tbl, thd_array, RPCCDS3_REP_THD);
      if (pthread_create(thread_tbl[RPCCDS3_REP_THD].thd_id, 
          pthread_attr_default, 
          (pthread_startroutine_t)rep_timer, 
          (pthread_addr_t)&rep_intvl) != 0 )
      {
         perror(" \n    Error creating reporting thread:");
         rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
         exit(1);
      }
      printf("done.\n");
   }

   if (sync_fn)
   {
      struct timespec sync_delay_ts = {0,0};
      /*---* the pthread call is a wrapped sleep *---*/
      pthread_delay_np(rpccds3_cli_syncstart(sync_fn,Lflag, &sync_delay_ts));
   }

  
   /*---* if we are in boundary mode... *---*/
   if (bflag)
   {
      char *msg_p;
      int ex_val=0;

      /*---* get the time... *---*/
      getclock(TIMEOFDAY, &precall_time);

      /*---* and call rpcsec2_cnf_str() *---*/
      ret_cal_data_p = rpccds3_send_cal(bndng_hndle,  
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
      if ((!strcmp(protocol,"ncadg_ip_udp") && comm_stat==rpc_s_comm_failure)||
    (!strcmp(protocol,"ncacn_ip_tcp") && comm_stat == rpc_s_server_too_busy))
      { 
         ex_val=1;
         /*---* set message pointer *---*/
         msg_p = "FAILED - SERVER IS TOO BUSY";
      }
      else if ((comm_fault_prob = rpccds3_cli_check_ret(ret_stat, comm_stat,
                                  fault_stat, 
                                  (short)rpccds3_op_is_send_cal)) == 2)
      { 
         ex_val = 2;
         msg_p = "FAILED - comm or server error";
      }
      else if (comm_fault_prob == 1)
      {
         ex_val = 2;
         msg_p = "FAILED - server returned an error";
      }
      else if (comm_fault_prob == 3)
      {
         ex_val = 2;
         msg_p = "FAILED - unmatched or corrupted calendar";
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
      rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(ex_val);
   }
   else
   {
      int trys;

      /*---* generate initial report line *---*/
      rpccds3_cli_rep_gen(1);

      /*---* while we are not done *---*/
      done = 0;
      while (!done)
      {
         /*---*
          *---* make the call
          *---*/
         if (!tflag && !start_time.tv_sec)
             getclock(TIMEOFDAY, &start_time );
 
#if defined(RPCCDS3_COMM_STAT_WKARND)
         comm_stat = rpc_s_ok;
#endif  /*---* defined(RPCCDS3_COMM_STAT_WKARND) *---*/

         ret_cal_data_p = (*rpccds3_cli_call_p)(bndng_hndle,&ret_stat,
                                                      &fault_stat,&comm_stat);

         /*---*
          *---* if start_time is empty here, then we are timing calls, and 
          *---* can use the precall time from the first call
          *---*/
         if (! start_time.tv_sec)
            start_time = precall_time;

         /*---* update call count *---*/
         calls++;

         /*---* check on success or failure of call *---*/
         comm_fault_prob = rpccds3_cli_check_ret(ret_stat, comm_stat, 
                                   fault_stat,(short)rpccds3_op_is_send_cal);

         /*---*
          *---* we now know if the call was successful or not, so we 
          *---* can free the memory allocated by the runtime to hold the 
          *---* returned calendar data (if the call succeeded)...
          *---*/
         if (!comm_fault_prob)
	 {
            if (debug)
            printf("Client(%d): freeing calendar data at 0x%x...\n", seq_numb,
                                                               ret_cal_data_p);
            rpc_ss_client_free((idl_void_p_t) ret_cal_data_p);
  	 }

         /*---* and we can free the binding we just used *---*/
         if (debug)
            printf("Client(%d): freeing binding...", seq_numb);
         rpc_binding_free(&bndng_hndle, &ret_stat);
         if (rpcsec2_check_stat(ret_stat, "freeing binding string", err_stats, 
                                                          &nerr_stats,debug,1))
            printf("Client(%d): Leaking memory :-{ (sizeof(binding))\n",
                                                                     seq_numb);
         
         /*---* Now do backend handling of call status. If we failed... *---*/
         if (comm_fault_prob)
            /*---* increment failed counter *---*/
            failed++;
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
               rpccds3_cli_rep_gen(0);
            if (calls == passes)
            {
               /*---* end processing expects the current time in tbuf *---*/
               (void)time(&tbuf);
               done = 1;
            }
         }

         
         /*---* 
          *---* we need to make sure that we have a handle to the next 
          *---* calendar before we iterate the loop, so we loop here 
          *---* looking for bindings and incrementing the currseq until we get
          *---* a binding or try all the calendars we are supposed to try.
          *---*/
         for (trys = 0; trys < ncdscals;  
             currseq = 
                 ((currseq >= (cdscalseq + (ncdscals-1)))?cdscalseq:currseq+1),
             trys++)
         {
            sprintf(cal_cds_name, "%s/rpccds3_cal%d",
                                      PVAL(params, param_i, obj_dir), currseq);
  
            /*---* now we can call rpccds3_get_handle_by_uuid *---*/
            if (debug)
               printf("Client(%d): Getting next binding through %s...", 
                                                      seq_numb, cal_cds_name);
            rpccds3_get_binding_by_uuid(cal_cds_name, protocol, &bndng_hndle,
                                                                   &ret_stat);

            if (rpcsec2_check_stat(ret_stat, "getting next binding by uuid",
                                             err_stats, &nerr_stats, debug,1))
               continue;
            else
            {
               if (debug)
               {
                 printf("Client(%d): got the following new handle for %s...\n",
                                                     seq_numb, cal_cds_name);
                 dump_binding(bndng_hndle);
               }

               /*---* specify authz and authn information *---*/
               if (debug)
                  printf("Client(%d): setting authz and authn info...",
                                                                  seq_numb);
               rpc_binding_set_auth_info(bndng_hndle, srv_pname, 
                             protect_level,
                             (unsigned32)rpc_c_authn_dce_secret, 
                             (rpc_auth_identity_handle_t)login_ctxt,
                             (unsigned32)rpc_c_authz_name,
                             &ret_stat);

               if (rpcsec2_check_stat(ret_stat, "setting auth info",
                                              err_stats, &nerr_stats, debug,1))
               {
                  printf("Client(%d): couldn't set auth info...\n", seq_numb);

                  /*---* free the binding we just tried *---*/
                  if (debug)
                     printf("Client(%d): freeing binding...", seq_numb);
                  rpc_binding_free(&bndng_hndle, &ret_stat);
                  if (rpcsec2_check_stat(ret_stat, "freeing binding string", 
                                              err_stats,  &nerr_stats,debug,1))
                     printf(
                          "Client(%d): Leaking memory :-{ (sizeof(binding))\n",
                                                                     seq_numb);
                  continue;
               }
               currseq = 
                 ((currseq >= (cdscalseq + (ncdscals-1)))?cdscalseq:currseq+1);
               break;  /*---* from the 'find a handle' loop *--*/
            }
         }  /*---* for () *---*/

         /*---* test to see if we were unable to get a binding *---*/
         if (trys == ncdscals)
         {
            printf( 
                "Client(%d): Can't get binding through assigned cds entries.\n",
                                                                     seq_numb);
            /*---* set done flag to get a good runtime print *---*/
            done = -1;
            break;
	 }
      }  /*---* while (!done) *---*/         
   } /*---* if ( bflag ) *---*/

   /*---* generate final report line *---*/
   rpccds3_cli_rep_gen(done?-1:-2);

   run_intvl = tbuf - start_time.tv_sec;
   if (debug)
   {
      printf("Start time : %s\n", ctime((time_t *)&start_time.tv_sec));
      printf("End time   : %s\n", ctime(&tbuf));
   }

   printf("Client(%d): RUNTIME: %d hours, %d min, %d secs.\n", seq_numb,
               run_intvl/3600, (run_intvl%3600)/60, (run_intvl%3600)%60);
   
   /*---* 
    *---* Cancel all our threads, otherwise they will continue after 
    *---* this main thread exits
    *---*/
   rpccds3_cli_kill_threads(thread_tbl[RPCCDS3_MAIN_THD].thd_id);

#if defined(RPCCDS3_DRVR_HNDLS_SIGCHLD)
   /*---* 
    *---* We use a signal handler in the driver to catch child exit.  In order
    *---* to prevent the handler from being bombarded with exits as all the 
    *---* run intervals expire, we sleep for seq_numb * 2 minutes to stagger
    *---* the arrival of the interrupts
    *---*/
   start_time.tv_sec = sflag?seq_numb * 120: 0;
   printf("Client(%d): exiting in %d seconds...\n",seq_numb,start_time.tv_sec);
   pthread_delay_np(&start_time);
#endif /*---* defined(RPCCDS3_DRVR_HNDLS_SIGCHLD) *---*/

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
rpccds3_cli_check_ret(unsigned32 ret_stat, error_status_t comm_stat,
                  error_status_t fault_stat, short op_def)
{
      int ret_val;
      extern char *rpccds3_nop_errs[],
                  *rpccds3_send_cal_errs[],
                  *rpccds3_srv_check_auth_errs[];

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
                                       err_stats, &nerr_stats,debug, 1))      
      {
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
      else if (ret_stat > rpccds3_srv_auth_bad_srv)
      {
         /*---* switch on the error return *---*/
         switch(ret_stat)
	 {
	    case rpccds3_srv_auth_bad_srv: /*---*
                                            *---* bad server principle name 
                                            *---* passed in RPC
                                            *---*/
                   if (debug)
                      printf("Client(%d): %s (%s).\n", seq_numb, 
                                         rpccds3_srv_check_auth_errs[ret_stat],
                                              PVAL(params, param_i,srv_princ));
                   break;

	    case rpccds3_srv_auth_bad_cli: /*---* 
                                            *---* passed bad client principal 
                                            *---* name in RPC
                                            *---*/
                   if (debug)
                      printf("Client(%d): %s(%s).\n", seq_numb,
                                       rpccds3_srv_check_auth_errs[ret_stat],
                                           PVAL(params, param_i, cli_princ));
                   break;

		                default: /*---* print the error string *---*/
                   if (debug)
                      printf("Client(%d): %s.\n", seq_numb,
                                      rpccds3_srv_check_auth_errs[ret_stat]);
	 }
         /*---* get the message in the stats table *---*/
         (void)rpcsec2_check_stat(ret_stat,
                                  rpccds3_srv_check_auth_errs[ret_stat],
                                       err_stats, &nerr_stats,debug, 1);

         ret_stat = 1;
      }
    
      /*---*   
       *---* else if we are checking for rpccds3_nop()...
       *---*/
      else if (op_def == rpccds3_op_is_nop)
      {
         /*---* check for success *---*/
         if (ret_stat == rpccds3_nop_ok)    
         {
            /*---* Success! Give good news to user *---*/
            if (debug)
               printf("Client(%d): Call to rpccds3_nop() succeeded\n",
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
                                               rpccds3_nop_errs[ret_stat]); 
	    }
            
            /*---* get the message in the stats table *---*/
            (void)rpcsec2_check_stat(ret_stat,
                                  rpccds3_nop_errs[ret_stat],
                                       err_stats, &nerr_stats,debug, 1);
            ret_val = 1;

	 } /*---* end of if (ret_stat == rpcsec2_ok) *---*/
      } /*---* end of if (op_def == rpcsec2_op_is_nop) true block *---*/
      /*---* we are checking for rpccds3_send_cal()... *---*/
      else
      {
         /*---* check for success *---*/
         if (ret_stat == rpccds3_send_cal_ok)    
         {
             /*---* Success! Give good news to user *---*/ 
             if (debug)
                printf("Client(%d): Call to rpccds3_send_cal() succeeded\n",
                                                                     seq_numb);

             if (rpccds3_compare_cal(ret_cal_data_p) == 0)
	     {
                printf("Client(%d): Returned data inconsistency\n", seq_numb);
                ret_val = 3;
	     }
             else
                /*---* Set return value *---*/
                ret_val = 0;
	 }
         else
         {

            /*---* remote call returns error, notify user *---*/
            if (debug)
	    {
               printf(
                    "Client(%d): Remote call failed, server returns error\n",
                                                                   seq_numb);
               printf("Client(%d): %s.\n", seq_numb, 
                                            rpccds3_send_cal_errs[ret_stat]);
               /*---* get the message in the stats table *---*/
               (void)rpcsec2_check_stat(ret_stat,
                                     rpccds3_send_cal_errs[ret_stat],
                                          err_stats, &nerr_stats,debug, 1);
	    }
            ret_val = 1;

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
calndr_data_p_t
rpccds3_cli_timed_send_cal_call(handle_t bnd_hndle,  idl_long_int *stat_p, 
                            error_status_t *fstat_p, error_status_t *cstat_p)
{
   calndr_data_p_t ret_val;

   /*---* do timing stuff then make the call *---*/

   /*---* get precall time *---*/
   getclock(TIMEOFDAY, &precall_time);
     
   /*---* and call rpcsec2_cnf_str() *---*/
   ret_val = rpccds3_send_cal(bnd_hndle, stat_p, fstat_p, cstat_p);

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
   
   return(ret_val);
}

/*---*
 *---* rpccds3_cli_rep_gen(int)
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
void rpccds3_cli_rep_gen(int sig)
{
   time_t tbuf;
   double avg;
   char toutbuf1[30], toutbuf2[30], statbuf[30], *gcp, *gcp2;
   
   /*---*
    *---* if we are in debug mode...
    *---*/
   if (debug)
      /*---* print hi message *---*/
      printf("Hi from rpccds3_cli_rep_gen: %s %d...\n", 
                     (sig? "handling signal: ": "called from code: "), sig);
   
   /*---*
    *---* if we get something funny, then put it in the message
    *---*/
   switch (sig)  
   {
      case -2: /*---*      died       *---*/
               /*---* FALLTHROUGH *---*/
      case -1: /*---* successful exit *---*/
               /*---* FALLTHROUGH *---*/
      case 1:  /*---*     Starting    *---*/
               /*---* FALLTHROUGH *---*/
      case 0:  /*---*     Report      *---*/
               /*---* FALLTHROUGH *---*/
      case 2:  /*---*     stopped     *---*/
              break;
      default: sprintf(statbuf, "ARG_IS_%d", sig);
               break;
   }

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
    (sig == 0? "RUNNING":
     (sig == -1? "FINISHED":
      (sig == -2?"DIED":
       (sig == 2? "STOPPED":statbuf))))), 
   uname_vals.nodename, uname_vals.sysname, gcp, protocol?protocol:"any",
   calls, passed, 
   failed, min_time.tv_sec, min_time.tv_nsec, max_time.tv_sec, 
   max_time.tv_nsec, elpsd_time.tv_sec, elpsd_time.tv_nsec, 
   (calls?(double)elpsd_time.tv_sec/(double)calls: (double)0), id_refr_cnt,
   gcp2);
   
   /*---* print any errors encountered so far *---*/
   dump_errs(seq_numb, "Client", err_stats, nerr_stats);

   fflush(stdout);

}

void 
rpccds3_cli_kill_threads(pthread_t *call_thd)
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
   for (i=0; i < RPCCDS3_CLI_NTHDS; i++)
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
 *---* rpccds3_cli_syncstart(fn, last_cli)
 *---*
 *---*     Looks for a specified file to base time of first RPC on, or 
 *---*     creates file if last_cli is true.  Loads a timespec struct whose
 *---*     addressed is passed in with the number of seconds to sleep before
 *---*     the first RPC.  Returns the address of the struct.
 *---*/
struct timespec *
rpccds3_cli_syncstart(char *fn, int last_cli, struct timespec *tspec_p)
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


int
rpccds3_cal_comp(void *cal_e_p1, void *cal_e_p2)
{
   return(strcmp(((cal_entry_p_t)cal_e_p1)->cal_data->cal_name,
                 ((cal_entry_p_t)cal_e_p2)->cal_data->cal_name));
}

int
rpccds3_compare_cal(calndr_data_p_t cal_data_p)
{
   int res, gotit=0;

   if (debug)
      printf("Client(%d): trying to match and verify %s...\n", seq_numb,
                                                       cal_data_p->cal_name);

   for (calendars_p = calendars; calendars_p - calendars < ncalendars; calendars_p++)
   {
      if (debug)
         printf("Client(%d): Comparing to %s...\n", seq_numb,
                                             calendars_p->cal_data->cal_name);

      res = strcmp(cal_data_p->cal_name, calendars_p->cal_data->cal_name);
      if (res == -1)
         /*---* it's not here, so break without setting the gotit flag *---*/
         break;
      else if (!res)
      {
         gotit=1;
         
         if (debug)
            printf("Client(%d): Matched %s...\n", seq_numb,
                                             calendars_p->cal_data->cal_name);
         break;
      }
   }
   
   if (gotit)
   {
      int i;

      if (debug)
        printf("Client(%d): Comparing calendar data...\n", seq_numb);
       
      for (i = 0; i < calendars_p->cal_data->cal_nlines; i++)
      {
         if (strcmp(calendars_p->cal_data->cal_lines[i], 
                                                     cal_data_p->cal_lines[i]))
	 {
            if (debug)
               printf("Client(%d): data mismatch in calendar(%s != %s)\n", 
                                seq_numb,calendars_p->cal_data->cal_lines[i],
                                cal_data_p->cal_lines[i]);
            gotit=0;
            break;
         }
      }
   } 
   if (!gotit)
   {
      if (debug)
         printf("Client(%d): No match or bad data for %s\n", seq_numb,
                                                     cal_data_p->cal_name);
   }
   return(gotit);
}

/*---*
 *---* rpccds3_get_binding_by_uuid(name, protocol, handle, status)
 *---*
 *---*    Gets the uuid from name, then does a binding_import from name
 *---*    specifying the uuid.  If protocol is specified, then the binding
 *---*    is checked to see if the protocol matches what was requested.
 *---*/
void
rpccds3_get_binding_by_uuid(char *cds_name, char *protocol, 
                           rpc_binding_handle_t *handle_p, unsigned32 *status)
{
  rpc_ns_handle_t inq_ctxt;   /*---* receives the context for CDS queries *--*/
  uuid_t          obj_uuid;   /*---* receives the uuid from the CDS entry *--*/
  unsigned32      locstat;  
  int done=0;

  /*---* look for name in the namespace *---*/
  rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_dce, 
                                (unsigned_char_t *)cds_name,
                                &inq_ctxt, status);
   
  if (*status == rpc_s_ok)
  {
    int got_error = 0;

    /*---* got a lookup context.  Now get the uuid *---*/
    if (debug)
       printf("Client(%d): Getting uuid from %s...\n", seq_numb, cds_name);
    rpc_ns_entry_object_inq_next(inq_ctxt, &obj_uuid, status);

    if (*status != rpc_s_ok)
    {
      /*---* no object uuid found *---*/
      printf("Client(%d): Can't get uuid from %s\n", seq_numb, cds_name);
      got_error = 1;
    }
    else
      /*---* got a uuid from name. *---*/
      if (debug)
         printf("Client(%d): Got a uuid from %s...\n", seq_numb, cds_name);
         
    /*---* free the inqiry context *---*/
    if (debug)
       printf("Client(%d): freeing obj inquiry context...", seq_numb);
    rpc_ns_entry_object_inq_done(&inq_ctxt, &locstat);
    if (rpcsec2_check_stat(locstat, "freeing inquiry context",
                                            err_stats, &nerr_stats, debug, 1))
      /*---* 
       *---* if we can't free the inquiry context, we warn the user, 
       *---* but we don't consider it a fatal problem.
       *---*/
      if (debug)
         printf("Client(%d): leaking memory :-( (inq_ctxt)\n", seq_numb);

    if (!got_error)
    {
      /*---* now lets look for the bindings *---*/
      rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce, 
                                    (unsigned_char_p_t)cds_name, 
                                     rpccds3_v1_0_c_ifspec, &obj_uuid,
                                     &inq_ctxt, status);
      if (*status == rpc_s_ok)
      {
        /*---* got a context...try for a binding *---*/
        while (*status == rpc_s_ok && !done)
        {
          rpc_ns_binding_import_next(inq_ctxt, handle_p, status);   

          if (*status == rpc_s_ok)
          {
            /*---* got a binding !
             *---*
             *---* if protocol is specified, then we do the caller the
             *---* curtesy of converting the binding to a string, parsing
             *---* it, and checking to see if it matches.  If it doesn't
             *---* match, look for anouther one.
             *---*/
            if (debug)
               printf("Client(%d): Got a binding...\n", seq_numb);

            if (!protocol || !strcmp(protocol, "any") )
               /*---* we are done! *---*/
               break;
            else              
            {
              unsigned_char_t *str_p = (unsigned_char_t *)NULL;
 
              /*---*
               *---* we need to try to see if this binding is for the 
               *---* requested protocol
               *---*
               *---* convert the binding to a string binding 
               *---*/
              if (debug)
                printf("Client(%d): converting binding handle...", seq_numb);

              rpc_binding_to_string_binding(*handle_p, &str_p, &locstat);
              if (locstat != rpc_s_ok)
              {
                /*---*
                 *---* we can't convert the binding to check the protocol.
                 *---* Our primary purpose is to return a binding, and we 
                 *---* already have one, so just warn the user.
                 *---*/
                printf("Client(%d): Can't convert binding...\n", seq_numb);

                /*---*                 
                 *---* break out of the loop here since we can't do any
                 *---* of the checking below.  
                 *---*/
                break;                
              }
              else
              {
                unsigned_char_t *locstr_p = (unsigned_char_t *)NULL;

                /*---* if we are in debug mode... *---*/
                if (debug)
                  /*---* print it *--*/
                  printf("Client(%d): got %s\n", seq_numb,str_p);
        
                /*---* parse string binding to see if protocol matches *---*/
                if (debug)
                  printf("Client(%d): Parsing string binding...", seq_numb);
                rpc_string_binding_parse(str_p, (unsigned_char_t **)NULL,
                                    &locstr_p, (unsigned_char_t **)NULL,
                                    (unsigned_char_t **)NULL,
                                    (unsigned_char_t **)NULL, &locstat);
                if (locstat != rpc_s_ok)
                {
                  /*---*
                   *---* we can't parse the binding to check the protocol.
                   *---* Our primary purpose is to return a binding, and we 
                   *---* already have one, so just warn the user.
                   *---*/
                  printf("Client(%d): Can't parse binding (%s)...\n",
                                                            seq_numb, str_p);

                }
                else
                {
                  /*---* 
                   *---* if the binding protocol matches what was requested,
                   *---*/
                  if (!strcmp(locstr_p, protocol))
                  { 
                    if (debug)
                       printf("Client(%d): found %s binding for %s.\n",
                                                    seq_numb, protocol, 
                                                                    cds_name);
                    done = 1;
                  }
                }  /*---* if (locstat != rpc_s_ok (string parse) *---*/
                
                /*---* if the binding got parsed then free the string *---*/
                if (locstr_p)
                {
                  if (debug)
                    printf("Client(%d): freeing protocol string...", seq_numb);
                  rpc_string_free(&locstr_p, &locstat);
                  if (rpcsec2_check_stat(locstat, "freeing protocol string",
                                           err_stats, &nerr_stats,debug,1))
                    printf("Client(%d): Leaking memory :-{\n", seq_numb);
                }
              }  /*---* if (locstat != rpc_s_ok (binding to string) *---*/

              /*---* if the binding got converted then free the string *---*/
              if (str_p)
              {
                if (debug)
                   printf("Client(%d): freeing binding string...", seq_numb);
                rpc_string_free(&str_p, &locstat);
                if (rpcsec2_check_stat(locstat, "freeing binding string",
                                           err_stats, &nerr_stats,debug,1))
                   printf("Client(%d): Leaking memory :-{\n", seq_numb);
              }
            }  /*---* if (!protocol)  *---*/
            
            /*---* 
             *---* if we got a binding, but it was not what we wanted, then
             *---* free the resources.
             *---*/
            if (!done)
            {
               if (debug)
                  printf("Client(%d)(get_bind): freeing binding...", seq_numb);

               rpc_binding_free(handle_p, &locstat);
               if (rpcsec2_check_stat(locstat, "freeing binding string",
                                              err_stats, &nerr_stats,debug,1))
                  printf("Client(%d): Leaking memory :-{ (sizeof(binding))\n",
                                                                     seq_numb);
	    }
          }  /*---* if (*status == rpc_s_ok) (getting a binding) *---*/
        }  /*---* while (*status == rpc_s_ok ) *---*/            
        
        /*---* free the import context *---*/
        if (debug)
           printf("Client(%d): freeing import context...", seq_numb);
        rpc_ns_binding_import_done(&inq_ctxt, &locstat);
        if (rpcsec2_check_stat(locstat, "freeing inquiry context",
                                            err_stats, &nerr_stats, debug, 1))
        /*---* 
         *---* if we can't free the inquiry context, we warn the user, 
         *---* but we don't consider it a fatal problem.
         *---*/
        if (debug)
           printf("Client(%d): leaking memory :-( (inq_ctxt)\n", seq_numb);

      }  /*---* if (*status == rpc_s_ok ) (import_begin) *---*/
    }  /*---* if (*status == rpc_s_ok ) (inquire_next) *---*/
  }  /*---* if (*status == rpc_s_ok )  (inquire_begin) *---*/
} 
              

