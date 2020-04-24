/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc.cds.3_smain.c,v $
 * Revision 1.1.90.2  1996/02/17  23:27:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:02  marty]
 *
 * Revision 1.1.90.1  1995/12/11  22:43:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/08  19:54 UTC  bissen
 * 	merge from HPDCE01
 * 	[1995/12/11  22:20:10  root]
 * 
 * Revision 1.1.87.2  1994/02/23  21:43:56  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:39:57  pellis]
 * 
 * Revision 1.1.87.1  1993/12/22  20:51:23  mhickey
 * 	no_changes
 * 	[1993/12/22  20:50:34  mhickey]
 * 
 * 	fixed a parameter transformation and made the
 * 	report interval come from the RPC_SRV_DEF_REP_INTVL
 * 	config file variable.
 * 	[1993/12/17  21:02:46  mhickey]
 * 
 * 	Moved the start of the signal thread to just before the
 * 	server_listen call so that the signal thread is not trying
 * 	to remove CDS entries while the main thread is trying to
 * 	create them (if a signal hits while they are getting
 * 	created.)
 * 	[1993/12/14  16:40:45  mhickey]
 * 
 * 	Numerous changes in the course of fixing two defects:
 * 	General
 * 	-------
 * 	- Cleaned up the includes/defines and thread handling
 * 	- streamline the value checking and conversion code.
 * 	- Make sig_handler use the arg_p parameter as an idication of how to stop
 * 	  the main thread.
 * 	- Fix/add comments and printfs
 * 	- Define action for after server stops listening and make this the default
 * 	- way for the server to exit.
 * 	- Fix thread type prints.
 * 
 * 	Defect 7993 - server doesn't clean up namespace
 * 	----------------------------------------------------
 * 	- Change kill threads in server to cleanup and make it remove server entry
 * 	- Cleans up objects and server entry from server
 * 
 * 	Defect 7743 - Doesn't work intercell
 * 	----------------------------------------------
 * 	Make the client name the simple name of the client so intercell works
 * 	Fix faulty logic in server authentication to make intercell work.
 * 	[1993/12/11  04:59:12  mhickey]
 * 
 * Revision 1.1.85.4  1993/11/02  22:27:49  ohara
 * 	svr4 porting changes
 * 	[1993/11/02  21:21:30  ohara]
 * 
 * Revision 1.1.85.3  1993/10/14  17:37:11  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:26  cmckeen]
 * 
 * Revision 1.1.85.2  1993/10/01  16:40:46  mhickey
 * 	This submission contains the following fixes:
 * 	  Defect 7747: rpc.cds.3 server and client don't renew
 * 	               tickets early enough
 * 
 * 	               This is the server side of the fix.  The
 * 	               server now refreshes it's ticket halfway
 * 	               through the ticket lifetime.  The change
 * 	               is in the setting of the variable
 * 	               'exp_time'.
 * 
 * 	  Defect 7744: rpc.cds.3 server and client fail if some
 * 	               command line options are not specified.
 * 
 * 	               This is the server side of the fix. The
 * 	               server now reads the defaults for the
 * 	               starting calendar number, and number of calendars
 * 	               from the config file, and defaults the data
 * 	               directory to '.'  If the values are not specified
 * 	               on the command line, the defaults are used.
 * 	               The changes are the setting of 'cal_d_dir to '.',
 * 	               the checks to see if 'cdscalseq' and 'ncdscals'
 * 	               are set, and the conversion and assignment of
 * 	               the defaults if they are not.
 * 
 * 	  Defect 7724: rpc.cds.3 and rpc.sec.2 servers buffer output.
 * 
 * 	               This is the rpc.cds.3 server part of the fix.
 * 	               The server now does a 'setbuf(stdout,NULL)' to
 * 	               avoid buffering messages.
 * 
 * 	  Also fixed the printf noting the startup of the refresh thread:
 * 	  was "Spawning timer thread", changed to "Spawning refresh thread"
 * 	[1993/10/01  15:13:31  mhickey]
 * 
 * Revision 1.1.85.1  1993/09/14  15:01:15  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/14  15:00:35  cmckeen]
 * 
 * Revision 1.1.7.2  1993/08/19  15:46:29  bissen
 * 	Fixed for hp-ux
 * 	[1993/08/19  15:45:45  bissen]
 * 
 * Revision 1.1.2.4  1993/03/12  18:56:36  mhickey
 * 	Fixing bsubmit mystery meat
 * 	[1993/03/12  18:52:19  mhickey]
 * 
 * Revision 1.1.1.3  1993/01/29  20:28:45  mhickey
 * 	Initial check -in for the rpc.cds.3 system test.
 * 
 * Revision 1.1.1.2  1992/12/22  04:44:10  mhickey
 * 	Initial checkin of rpc.cds.3 server module.
 * 
 * Revision 1.1.1.2  1992/12/03  17:49:06  mhickey
 * 	Final check-in before initial submit.
 * 
 * 	Added automatic id refreshes via timer thread that refreshes
 * 	the id one minute before it is to expire.  Added a timedwait
 * 	to the rpcsec2_cnf_str routine to make the operations take
 * 	longer so that request buffering will take place.  Added
 * 	-l option to allow setting of protection level.
 * 
 * 	Defect 5887.
 * 
 * Revision 1.1.1.4  1992/10/16  13:33:07  mhickey
 * 	 Added routine tstrpc_server_check_auth() to extract the auth
 * 	 info from the client's binding handle, and verify it against
 * 	 what we expect.  Verification is done by server name, authz
 * 	 service, authn service, protection level and client name.
 * 	 Added calls to this routine to the two entry point routines.
 * 	 Made all printf's in the routines that are executed as a
 * 	 result of client calls conditional on debug_sw to prevent
 * 	 a lot of useless noise during normal operation.
 * 
 * 	 We have authenticated RPC!!
 * 
 * Revision 1.1.1.3  1992/10/10  03:14:38  mhickey
 * 	Added tstrpc_cnf_str function and debug command line option.
 * 
 * 	Note that un/exporting and replacing obj UUIDS is still messed.
 * 
 * Revision 1.1.1.2  1992/10/06  17:30:31  mhickey
 * 	Initial checkin for base for rpc.security.2
 * 
 * $EndLog$
 */

/*---*
 *---* rpc.cds.3_smain.c - server side of rpc.cds.3 test
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
 *---*    1. Assumes the server prinicipal identity specified in the config 
 *---*       file (rpc.cds.3.conf or passed via the -f command line option).
 *---*       This is only true if the server was compiled with the 
 *---*       -DRPCCDS3_DO_LOGIN compiler switch, otherwise, the server
 *---*       runs with the identity of the invoker.
 *---*    2. Spawns a thread to maintain id's identity; only done if 
 *---*       compiled with the -DRPCCDS3_AUTO_REFRESH compiler switch.
 *---*    3. Reads the calendar data files and builds an internal array
 *---*       of the calendars and their data.
 *---*    4. For each calendar to be exported, generates a string of the 
 *---*       form rpccds3_cal<n> where n is between the number specified
 *---*       for the starting calendar with the RPCCDS3_SRV_CAL_SEQ variable
 *---*       in the config file, or with the -I option on the command line,
 *---*       and the starting number plus the number of calandars to export;
 *---*       specified by the RPCCDS3_SRV_NCALS variable in the config file,
 *---*       or by the -n command line option.
 *---*    5. For each calendar, a uuid is assigned from the existing CDS 
 *---*       entry, or is created anew.
 *---*    6. If not already present in the namespace, the calendar entry
 *---*       is exported to the namespace, and it's group attribute filled
 *---*       in with the name of the server CDS entry.
 *---*    7. Exports the server entry.
 *---*    8. Services calls to rpccds3_send_cal, by picking the uuid out of
 *---*       the binding handle and using it to determine which calendar to
 *---*       return.
 *---*
 *---* CALLED BY
 *---*    rpc.cds.3_runtest.sh - (the driver for running the test program)
 *---* 
 *---* CALLS INTO 
 *---*    Reads rpc.cds.3.conf or file specified via the -f option for 
 *---*    definitions.
 *---*    chk_stat.c  (rpcsec2_chk_stat() )
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
#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/id_base.h>
#include <dce/dce.h>
#include <pthread.h>
#include "../../tools/systest.h"

#include <sys/stat.h>

/*----------------------------*
 *---* Private include files
 *----------------------------*/
#include <rpc_cds_3.h>
#include <rpc.cds.3_defs.h>
#include "../rpc.sec.2/rpctest.h"



/*-------------------------------------------*
 *---* Global variables
 *-------------------------------------------*/
/*---------------------------*
 *---* Entry point vector 
 *---------------------------*/
rpccds3_v1_0_epv_t rpccds3_v1_0_epv = {rpccds3_nop, rpccds3_send_cal};

/*---------------------------------* 
 *---* Command line option flags
 *---------------------------------*/
int debug,  /*---* variable to turn on to get debug output *---*/
    epv_def,   /*---* variable to turn on to let epv default  *---*/
    use_null,  /*---* variable to turn on to export no UUID   *---*/
    repl_uuid; /*---* variable to replace uuid in namespace entry *---*/

/*---------------------*
 *---* Usage message
 *---------------------*/
char *usage = "usage: rpc.cds.3_srv [-n] [-r] [-d] [-e] [-f config_file] [-l prot_level";

/*----------------------------------* 
 *---* call stats variables
 *-----------------------------------*/
int tot_calls,
    calls_passed,
    calls_failed;

/*---*
 *---* timer variables (currently unused )
 *---*/
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
unsigned32 protect_level;          /*---* protection level value        *---*/
char      *protect_level_str;      /*---* string representation of same *---*/
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
sec_login_auth_src_t auth_src;     /*---* receives auth src info from call
                                    *---* to sec_login_validate_identity
                                    *---*/
char               *keytab;        /*---* pointer to keytab filename *---*/

/*---* 
 *---* server principle name = cell root + name
 *---*/
char *server_pname;

/*---* 
 *---* client principle name = name
 *---*/
char *client_pname;
/*---* 
 *---* variable that stores the expiration interval minus one minute
 *---*/
long  id_lifetime;

/*---*
 *---* thread variables we need to syncronize name refreshes
 *---*/
pthread_cond_t refr_id_cond;
pthread_mutex_t refr_id_mutex;

/*---* other variables for id refresh routine *---*/
time_t last_id_refr;
int id_refr_cnt;




/*----------------------------------------*
 *---* Thread management structs/defines 
 *----------------------------------------*/
/*---* defines for threads we spawn *---*/
#define RPCCDS3_MAIN_THD  0
#define RPCCDS3_REFR_THD  1
#define RPCCDS3_SIG_THD   2
#define RPCCDS3_REP_THD   3
#define RPCCDS3_SRV_NTHDS 4

/*---* array of threads we spawn *---*/
pthread_t thd_array[RPCCDS3_SRV_NTHDS];

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

#if defined(RPCCDS3_SRV_REPORTING)
  /*-----------------------------------------------*
   *---* thread variables we need to time reporting intervals 
   *-----------------------------------------------*/
  pthread_cond_t rep_timer_cond;
  pthread_mutex_t rep_timer_mutex;

#endif /*---* defined(RPCCDS3_SRV_REPORTING) *---*/

struct utsname uname_vals;  /*---* OS type, hostname; etc. *---*/


/*----------------------------------------* 
 *---* Management code control variables
 *----------------------------------------*/
/*---*
 *---* thread variables we need to enforce buffering of incoming calls
 *---* (ie: these are used to make the server take relatively large amounts of
 *---* time to process a call - in the order of seconds
 *---*/
pthread_cond_t call_delay_cond;

pthread_mutex_t stats_update_mutex;  /*---* protects passed/failed counters **/

/*---*
 *---* minimum duration of manager calls
 *---*/
int srv_call_delay;



/*------------------------------------------*
 *---* Calendar data management
 *------------------------------------------*/
/*---*
 *---* base of the calendars array and an indexing pointer
 *---*/
cal_entry_p_t calendars, calendars_p;

/*---*
 *---* number of calendars we manage
 *---*/
int ncalendars;

/*---* 
 *---* number of the last calendar we exported.  This is global so that 
 *---* if something goes wrong during the calendar create, and we are to 
 *---* remove the calendar namespace entries, we know how many to try to 
 *---* remove.
 *---*/
int curr_seq,
    cdscalseq;                   /*---* starting sequence number for 
                                   *---* calendars *---*/
/*---*
 *---* pointer to space that can hold a calendar name
 *---*/
 char *cal_cds_name;      /*---* points to name in cds *---*/



/*---------------------------------*
 *---* Namespace usage variables
 *---------------------------------*/
/*---*
 *---* flags that indicates to the cleanup routine whether the namespace server
 *---* entry has been created and should be cleaned up.
 *---*/
int server_entry_created;
int rem_srv_ent;

/*---*
 *---* flags that indicates to the cleanup routine whether the namespace server
 *---* entry has been created and should be cleaned up.
 *---*/
int server_objects_created;
int rem_srv_objs;


/*-----------------------------------------* 
 *---* Configuration variables/file management
 *-----------------------------------------*/
/*---* 
 *---* the array of parameter specs we need - passed to rd_conf()
 *---*/
param_spec_val_t params[] = {
                              {"RPCCDS3_SRV_KEYTAB_DIRPATH", 0},
                              {"RPCCDS3_SRV_KEYTAB_FN", 0},
                              {"RPCCDS3_SRV_PRINC_NAME", 0},
                              {"RPCCDS3_CLI_PRINC_NAME", 0},
                              {"RPCCDS3_SRV_CDS_NAME", 0},
                              {"RPCCDS3_SRV_OBJ_DIR", 0},
                              {"RPCCDS3_SRV_CAL_DATA", 0},
                              {"RPCCDS3_SRV_NCALS", 0},
                              {"RPCCDS3_SRV_CALSEQ", 0},
                              {"RPCCDS3_SRV_MAX_CALLS", 0},
                              {"RPCCDS3_SRV_MAX_EXEC", 0},
                              {"RPCCDS3_SRV_CALL_DELAY", 0},
                              {"RPCCDS3_SRV_REM_SRV_OBJS", 0},
                              {"RPCCDS3_SRV_REM_SRV_ENT", 0}, 
#if defined(RPCCDS3_SRV_REPORTING)
                              {"RPCCDS3_SRV_DEF_REP_INTVL", 0},
#endif /*---* defined(RPCCDS3_SRV_REPORTING) *---*/
                              {"RPCCDS3_PROT_LEVEL", 0}
                            };

/*---*
 *---* enum for accessing the params array using macros in rpc.sec.2_defs.h
 *---*/
enum {
       keytab_dir, 
       keytab_fn, 
       srv_princ, 
       cli_princ, 
       cds_name,
       obj_dir,
       cal_data,
       cdscals,
       calseq,
       call_max,
       exec_max,
       srv_delay,
       rem_objs,
       rem_if_entry,
#if defined(RPCCDS3_SRV_REPORTING) 
       rep_int,
#endif /*---* defined(RPCCDS3_SRV_REPORTING) *---*/
       prot_lev
     } param_i;


/*------------------------------------------*
 *---* Function prototypes
 *------------------------------------------*/
void rpccds3_cleanup(pthread_t *);
void maintain_id(int *);
void dump_key(sec_passwd_rec_t *);
int signal_catcher (char *);

#if defined(RPCCDS3_SRV_REPORTING)
void rep_timer(int *);
#endif /*---* defined(RPCCDS3_SRV_REPORTING) *---*/

void rpccds3_rep_gen(int);
int rpccds3_cal_comp(void *, void *);
void rpccds3_srv_check_auth(rpc_binding_handle_t, unsigned32 *);

/*--------------------------------------------*
 *---* pointers to functions to external routines
 *---------------------------------------------*/
/*---* report generation function *---*/
void (*rep_gen_func_p)() = rpccds3_rep_gen;
void (*cleanup_func_p)() = rpccds3_cleanup;


/*------------------------------------------*
 *---* Start of main program
 *------------------------------------------*/
main(int argc, char **argv)
{

   /*-----------------------------------*
    *---* declare local variables 
    *-----------------------------------*/
   unsigned32 status;                 /*---* Generic status return param *---*/
   rpc_binding_vector_t *bnd_vctr_p;  /*---* param to receive bindings   *---*/
   unsigned char msgbuff[dce_c_error_string_len];  /*---* buff for error 
                                                    *---* messages
                                                    *---*/
   int stat2,                         /*---* Generic status return param *-*/
       max_calls,                     /*---* max cuncurrent calls *---*/
       max_exec,                      /*---* max concurrent threads  *---*/
#if defined(RPCCDS3_SRV_REPORTING)
       rep_intvl,
#endif /*---* defined(RPCCDS3_SRV_REPORTING) *---*/
       pid=getpid(),
       uuid_from= 0,                  /*---* indicates if uuid is new or
                                       *---* needs unexporting *---*/
       ncdscals=0,                    /*---* number of cals to export *---*/
       uuidsdone=0,     
       i;

   uuid_t obj_uuid,                   /*---* receives uuid if we are 
                                       *---* generating one  
                                       *---*/
        **uuid_pp;                    /*---* index pointer for cal uuids *-*/

   uuid_vector_t obj_uuid_v,          /*---* uuid vector for rpc register
                                       *---* interface call 
                                       *---*/
               *uuid_v_p,
               *uuid_vec_p;           /*---* pointer to the calendar uuid vec
                                       *---*/
   unsigned_char_t *str_p;            /*---* Generic pointer for convert to 
                                       *---* string calls
                                       *---*/
   rpc_ns_handle_t inq_ctxt;          /*---* receives inquiry context info
                                       *---* when looking for bindings
                                       *---*/
   sec_passwd_rec_t   srv_passwd;     /*---* password rec that we fill in for
                                       *---* sec_login_validate_identity call
                                       *---*/
   boolean32        reset_passwd;     /*---* receives flag value indicating
                                       *---* whether passwd has expired or 
                                       *---* not.
                                       *---*/

   char           *cp,
                  *srv_cds_name,      /*---* guess!? *---*/
                  *sig_hndlr_arg = "server",
                  *cal_data_f=NULL,
                  *cal_d_dir = ".",   /*---* directory where calendar data
                                       *---* files are located.  Default is 
                                       *---* current directory.    *---*/
                     *conf_file=NULL; /*---* pointer to config file name 
                                       *---* if passed on the command line
                                       *---*/
   time_t           tbuf;             /*---* to get the current time *---*/
   signed32         exp_time;         /*---* when our identity expires *---*/
   pthread_t        timer_thread;     /*---* handle of timer thread *---*/

   extern char    *rd_conf_errs[],
                  *rpccds3_parse_cal_spec_errs[],
                  *rpccds3_set_cal_uuid_errs[];

   /*---* 
    *---* in an effort to get output as it happens, run unbuffered.
    *---*/
   setbuf(stdout, (char *)NULL);

   /*-------------------------------------*
    *---* process the command line
    *-------------------------------------*/
   ++argv;        /*---* skip the program name *---*/    

   /*---* while we have args left on the command line... *---*/
   while(*argv)  
   {
      /*---* if this is a flag value... *---*/
      if (**argv == '-')
         /*---* 'switch' on the flag value *---*/
         switch(*((*argv)+1)) 
         {
	    case 'D': /*---* directory for calendar data files *---*/
                      if ( ! *(argv+1))
		      {
                         /*---* error and usage *---*/
                         printf(" Server: no value for calendar directory name.\n");
                         printf(" Server: %s\n", usage);
                         exit(1);
		      }
                      cal_d_dir = *++argv;
                      break;
                      
            case 'c': /*---* calendar data file list *---*/
                      cal_data_f = *++argv;
                      break;

	    case 'd': /*---* debug on *---*/
                      debug = 1;
                      break;

	    case 'e': /*---* let epv default *---*/
                      printf(" Server: Letting epv default.\n");
                      epv_def = 1;
                      break;
 
	    case 'f': /*---* configuration file name *---*/
                      /*---* if no value for option... *---*/
                      if (!*(argv+1))
		      {
                         /*---* error and usage *---*/
                         printf(" Server: no value for config file name.\n");
                         printf(" Server: %s\n", usage);
                         exit(1);
		      }
                      /*---* else set pointer *---*/
                      conf_file = *++argv;
                      break;

            case 'l': /*---* the protection level *---*/
                      /*---* if no value specified *---*/
                      if (! *(argv+1))
                      {
                         printf("Server: no value for -l flag (prot level).\n");
                         printf("Server: %s\n", usage);
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
                         printf(" Server: bad value for -l flag (prot level).\n");
                         printf(" Server; must be one of none, conn, call, pkt, integ, priv.\n");
                         printf(" Server: %s\n", usage);
                         exit(1);
                      }
                      else
                         protect_level_str = *argv;
                
                      break;
                   

            case 'n': /*---* number of calendars to export to cds *---*/
                      /*---* FALLTHROUGH *---*/
	    case 'I': /*---* initial sequence number of cds cals *---*/
                     
                      /*---* if we got a good arg... *---*/
                      if (*(argv+1) && isdigit(**(argv+1)))
		      {
                         char c = *((*argv)+1);
                         sscanf(*++argv, "%d",c == 'n'? &ncdscals: &cdscalseq);
		      }
                      else
		      {
                         /*---* print appropriate message... *---*/
                         printf(" Server: %s for -%c option.\n", 
                                         (*(argv+1)?"bad value": "no value"),
                                         *((*(argv-1))+1));
                         printf(" Server: %s\n", usage);
                         exit(3);
                      }                         
                      break;
 
	    case 'r': /*---* replace any existing uuid *---*/
                      /*---* only one of -r or -n allowed *---*/
                      if (use_null)
		      {
                         printf(" Server: only one of -n or -r allowed\n");
                         printf(" Server: %s\n", usage);
                         exit(1);
		      }
                      repl_uuid = 1;
                      break;
            
            default :
                      printf(" Server: bad option %s\n", *argv); 
                      printf(" Server: %s\n", usage);
                      exit(1);

                      break;
         }
      /*---* else this is not a flag value *---*/
      else
      {
         /*---* error: report and exit *---*/
         printf(" Server: bad command line input\n");
         printf(" Server: %s\n",usage);
         exit(1);
      } /*---* end of if (**argv...) *---*/

      argv++;        /*---* get next command line arg *---*/
   } /*---* end of while(*argv) *---*/

   /*----------------------------------------------------------*
    *---* Get our operational parameters from the config file
    *----------------------------------------------------------*/
   printf(" Server: Reading config file (%s)...", 
                                     (conf_file? conf_file:RPCCDS3_CONF_FN));
   rd_conf((conf_file? conf_file: RPCCDS3_CONF_FN), params, 
                   sizeof(params)/sizeof(param_spec_val_t), &status);
   /*---* if we did not succeed... *---*/
   if (status != rdconf_ok)
   {
      /*---* report error *---*/
      printf("\n\n Server: %s (%s).\n", rd_conf_errs[status],
                                   (conf_file? conf_file: RPCCDS3_CONF_FN));
      /*---* exit *---*/
      exit(1);
   }
   /*---* report success *---*/
   printf("\ndone.\n\n");
   
   /*---* convert our numberic params *---*/
   /*---* max cals *---*/
   int_from_PVAL(&max_calls,params, param_i,call_max,"max calls", status);
   if (status)
      exit(2);

   /*---* max exec threads *---*/
   int_from_PVAL(&max_exec,params,param_i,exec_max,"max exec threads",status);
   if (status)
      exit(2);

   /*---* server call delay *---*/
   int_from_PVAL(&srv_call_delay,params,param_i,srv_delay,"call delay",status);
   if (status)
      exit(2);

   /*---* remove server entry flag *---*/
   int_from_PVAL(&rem_srv_ent,params,param_i,rem_if_entry,"remove server",status);
   if (status)
      exit(2);
   else
   {
      if (rem_srv_ent)
         printf(" Server: Server entry WILL BE REMOVED on exit...\n");
      else
         printf(" Server: Server entry WILL NOT BE REMOVED on exit...\n");
   }

   /*---* remove server objects flag *---*/
   int_from_PVAL(&rem_srv_objs,params,param_i,rem_objs,"remove objects",status);
   if (status)
      exit(2);
   else
   {
      if (rem_srv_objs)
         printf(" Server: Server objects WILL BE REMOVED on exit...\n");
      else
         printf(" Server: Server objects WILL NOT BE REMOVED on exit...\n");
   }
      
   /*---*
    *---* set the initial cds number from the config file value if not set
    *---* on command line 
    *---*/
   if (!cdscalseq)
   {
      /*---* try to convert the param from the config file *---*/
      int_from_PVAL(&cdscalseq,params,param_i,calseq,"cal sequence",status);
      if (status)
         exit(2);
   }

   /*---*
    *---* set the max cds number from the config file value if not set
    *---* on command line 
    *---*/
   if (!ncdscals)
   {
      /*---* try to convert the param from the config file *---*/
      int_from_PVAL(&ncdscals,params,param_i,cdscals,"cal range",status);
      if (status)
         exit(2);
   }

   printf(" Server: Starting cal sequence: %d...\n", cdscalseq);
   printf(" Server: Exporting %d cds calendars...\n", ncdscals);

   /*---* set the protection level *---*/
   if (protect_level_str)
      cp = protect_level_str;
   else
      cp = PVAL(params, param_i, prot_lev);

   /*---* transform the string to a real value with an ugly big if *---*/
   SET_PROT_LEVEL(cp, protect_level, "Server")
   /*---* if not already set, set the protection level string *---*/
   protect_level_str = cp;
   
   printf(" Server: Using protection level %s...\n", cp);

   /*---* get the machine name and os_type *---*/
   uname(&uname_vals);
   printf(" Server: PID: %d System: %s; O/S: %s...\n", pid,
                                     uname_vals.nodename, uname_vals.sysname);

   /*------------------------------------------------------*
    *---* Register auth info, get our identity, and "login"
    *------------------------------------------------------*
    *---*
    *---* set auth info for full authenticated RPC. Use shared secret authn,
    *---* default key retrieval func, and our own keytab file.
    *---*
    *---* construct the keytab filename. First get space for the string. We 
    *---* will need length(pathname) + length(filename) + '/' + '\0'
    *---*/
   keytab = (char *)malloc(strlen(PVAL(params, param_i, keytab_dir)) +
                           strlen(PVAL(params, param_i, keytab_fn))  + 2 );
   /*---* fill in the string *---*/
   sprintf(keytab, "%s/", PVAL(params, param_i, keytab_dir));
   strcat(keytab,  PVAL(params, param_i, keytab_fn));

   /*---* for the efficiency freak in me ...*---*/
   free(PVAL(params, param_i, keytab_dir));
   free(PVAL(params, param_i, keytab_fn));

   /*---* register auth info *---*/
   printf(" Server: registering authorization info...");
   rpc_server_register_auth_info(
                          (unsigned_char_t *)PVAL(params,param_i,srv_princ), 
                                 rpc_c_authn_dce_secret, 
                                 (rpc_auth_key_retrieval_fn_t) NULL,
                                 (void *)keytab,
                                 &status);

   if (rpcsec2_check_stat(status, "registering auth info",
                              err_stats, &nerr_stats,  debug, 0))
      exit(1);

#if defined(RPCCDS3_DO_LOGIN)
   if (debug)
   {
      /*---* get the current server principle *---*/
      rpc_mgmt_inq_server_princ_name((rpc_binding_handle_t)NULL, 
                                rpc_c_authn_dce_secret, &str_p,  &status);
      printf(" Server: Principle name before the change is %s.\n",str_p);
      rpc_string_free(&str_p, &status);
   }

   /*---* Create a context and get the login contex loaded *---*/
   printf(" Server: create login context...");
   sec_login_setup_identity((unsigned_char_t *)PVAL(params,param_i,srv_princ),
                            (sec_login_flags_t)sec_login_no_flags,
                            &login_ctxt, &status);
   
   if (rpcsec2_check_stat(status, "creating login context",
                              err_stats, &nerr_stats,  debug, 0))
      exit(1);

   /*---* Get our secret key from our keytab file *---*/
   printf(" Server: getting secret key from %s...", keytab);
   sec_key_mgmt_get_key(rpc_c_authn_dce_secret, (void *)keytab,
                        (char *)PVAL(params,param_i,srv_princ), (unsigned32)0, 
                        &sec_key_p, &status);

   if (rpcsec2_check_stat(status, "getting secret key",
                              err_stats, &nerr_stats,  debug, 0))
      exit(1);

   if (debug)
      dump_key((sec_passwd_rec_t *)sec_key_p);

   /*---* 
    *---* We have found that if we use the key pointer, or assign the key 
    *---* struct to a global key struct, that the key data disappears after
    *---* we spawn our thread.  Since we want the timer thread to process 
    *---* refresh cycles as fast as possible, we don't want to have to get the
    *---* key all the time, so this vanishing data is vexing.  To workaround
    *---* the problem, we store the data in a global array, and fix the 
    *---* struct in the timer thread.
    *---*/
   memcpy(des_key, 
      (char *)(((sec_passwd_rec_t *)sec_key_p)->key.tagged_union.des_key),8);
   *(des_key+8) = '\0';

   /*---* validate our login context *---*/
   printf(" Server: validating login context...");
   sec_login_validate_identity(login_ctxt, sec_key_p, &reset_passwd,
                                                           &auth_src, &status);
   /*---*
    *---* ???? need to do something about expired passwd here ????
    *---*/
  
   if (rpcsec2_check_stat(status, "validating login context",
                              err_stats, &nerr_stats,  debug, 0))
      exit(1);
   
   /*---* if we did not get validated for network ops... *---*/
   if (auth_src != sec_login_auth_src_network)
   {
      /*---* error and out *---*/
      printf(" Server: Not authoriized for network operations!\n");
      exit(2);
   }

   /*---* certify identity information *---*/
   printf(" Server: certifying identity info...");
   sec_login_certify_identity(login_ctxt, &status);
 
   if (rpcsec2_check_stat(status, "certifying identity info",
                              err_stats, &nerr_stats,  debug, 0))
      exit(1);

   /*---* finally, set our context *---*/
   printf(" Server: setting our own context...");
   sec_login_set_context(login_ctxt, &status);

   if (rpcsec2_check_stat(status, "setting our own context",
                              err_stats, &nerr_stats,  debug, 0))
      exit(1);

#else /*---* ! defined RPCCDS3_DO_LOGIN *---*/
   /*---*
    *---* if not establishing our own identity, just get the 
    *---* identity under which we were invoked.
    *---*/
   printf(" Server: Using default context...getting current context...");
   sec_login_get_current_context(&login_ctxt, &status);
   if (rpcsec2_check_stat(status, "getting login context",
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
   printf(" Server: Initializing condition variable...");
   if ( pthread_cond_init(&refr_id_cond, pthread_condattr_default) != 0 )
   {
      perror(" \n    Error initializing condition variable:");
      exit(1);
   }
   printf("done.\n");

   /*---*
    *---* initialize the mutex
    *---*/
   printf(" Server: Initializing the mutex...");
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
      printf(" Server: Getting our expiration date...");
   sec_login_get_expiration(login_ctxt, &exp_time, &status);
   if (rpcsec2_check_stat(status, "getting expiration time", err_stats, 
                                                     &nerr_stats, debug, 1))   {
       /*---*  
        *---* we can't verify if it is the identity problem. 
        *---* since we may in fact be okay, we just go on. 
        *---*/
       printf(" Server: Can't get identity information\n");
   }
   /*---*
    *---* we got the info, so check it against the current time.
    *---*/
   else
   {
       int expdif=0;

       /*---*
        *---* set the alarm for half the time through the ticket lifetime
        *---*/
       exp_time -= time((time_t *)NULL);
       /*---* round up exp_time if necessary *---*/
       exp_time  += ((expdif = exp_time%60)? 60 - expdif: 0);
       id_lifetime = exp_time / 2; 
       printf(" Server: identity lifetime = %d mins, refresh interval is %d mins.\n", exp_time/60, id_lifetime/60);
       printf(" Server: Spawning refresh thread...");
       USE_PTHREAD(thread_tbl, thd_array, RPCCDS3_REFR_THD)
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

   /*---* store our thread id *---*/
   USE_PTHREAD(thread_tbl, thd_array, RPCCDS3_MAIN_THD);
   *(thread_tbl[RPCCDS3_MAIN_THD].thd_id) = pthread_self();



   /*---* 
    *---* We want to make sure that the server calls take a certain amount of 
    *---* time to execute, thereby ensuring (we hope) that the calls will stack
    *---* up in the runtime call buffer.  We do this by making each call
    *---* "sleep" via timed wait on a condition variable for whatever time is 
    *---* specified.  For the algorithm used to determine the time to sleep, 
    *---* see the comments in rpcsec2_cnf_str() below.
    *---*
    *---* initialize the condition variable
    *---*/
   printf(" Server: Initializing duration condition variable...");
   if ( pthread_cond_init(&call_delay_cond, pthread_condattr_default) != 0 )
   {
      perror(" \n    Error initializing duration condition variable:");
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }
   printf("done.\n");

   /*---* read the calendars *---*/
   printf(" Server: Reading calendar data files...");
   rpccds3_parse_cal_spec(cal_d_dir, 
               cal_data_f? cal_data_f: PVAL(params, param_i, cal_data),
               &calendars, &ncalendars, debug, &status);
   if (status != rpccds3_parse_cal_spec_ok)
   {
      printf(" Server: error reading calendar data: %s.\n", 
                                        rpccds3_parse_cal_spec_errs[status]);
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }
  
   for (calendars_p = calendars; calendars_p - calendars < ncalendars; 
                                                               calendars_p++)
   if (debug)
   {
      printf(" Server: Data for calendar %s is:\n", 
                                         (calendars_p->cal_data)->cal_name);
           printf("     lines: %d\n", (calendars_p->cal_data)->cal_nlines);
           for (i=0; i < (calendars_p->cal_data)->cal_nlines;  
            printf("     line %d: %s\n", i, 
                                  ((calendars_p->cal_data)->cal_lines)[i++]));
   }
  
   /*---* 
    *---* now get a uuid for each calendar.  This is either newly created, or
    *---* is the uuid from an existing namespace entry for the calendar.  If
    *---* it is an existing uuid, and we already have a uuid set for the 
    *---* calendar, then on return from rpccds3_set_cal_uuid, the old_uuid
    *---* pointer should point to uuids to unexport before exporting our
    *---* one.
    *---*/
   /*---* 
    *---* get space for a vector of uuids to export to the server entry
    *---*/
   if (! (uuid_vec_p = (uuid_vector_t *)malloc(sizeof(uuid_vector_t)+ ((ncalendars - 1) * sizeof(uuid_p_t)))))
   {   
      printf(" Server: Can't get space for uuid vector.\n");
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }
   uuid_pp = uuid_vec_p->uuid;
   uuid_vec_p->count=0;

   /*---* 
    *---* now get uuids for each of the calendars.  If the uuid is new, then
    *---* do the export of the uuid and the ns_group_mmbr_add. 
    *---*/
   if (! (cal_cds_name = (char *)malloc(strlen(PVAL(params, param_i, obj_dir)) + 50)))
   {
      printf(" Server: no space for calendar cds name\n");
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }
   for (calendars_p=calendars, uuidsdone=0, curr_seq=cdscalseq; 
            curr_seq - cdscalseq < ncdscals; curr_seq++)
   { 
      /*---* construct the calendar name *---*/
      sprintf(cal_cds_name, "%s/rpccds3_cal%d", PVAL(params, param_i, obj_dir),
                                          curr_seq);
      /*---* first get the uuid set for the calendar *---*/
      printf(" Server: Getting uuid for calendar %s...",
                                         (calendars_p->cal_data)->cal_name);
      uuid_v_p = (uuid_vector_t *)NULL;

      rpccds3_set_cal_uuid(cal_cds_name, 
                     &(calendars_p->cal_uuid), &uuid_from, &uuid_v_p,
                     err_stats, &nerr_stats, debug,
                     &status);
      if (status != rpccds3_set_cal_uuid_ok)
      {
         printf("Error: %s\n", rpccds3_set_cal_uuid_errs[status]);
         rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
         exit(1);
      }
      else
         printf("done.\n");

      if (debug)
      {
         unsigned_char_t *str_p;
         unsigned32 locstatus;
    
         printf(" Server: uuid for %s is from %s.\n", cal_cds_name,
                    (uuid_from == uuid_from_uuidgen? "uuid_create":
                 (uuid_from == uuid_from_cds?"cds entry":
                 (uuid_from == uuid_from_both?"both":"calendar entry"))));

         /*---* print the uuid for the user *---*/
         printf(" Server: converting to string uuid...");
         uuid_to_string(&(calendars_p->cal_uuid),&str_p, &locstatus);
         if (locstatus != uuid_s_ok)
            (void) rpcsec2_check_stat(locstatus, "converting to string",
                                       err_stats, &nerr_stats,  debug, 0);
         else
         {
            printf("done\n Server: uuid for calendar %s is %s...\n\n",
                                                     cal_cds_name, str_p);
    
            /*---* 
             *---* if we converted the uuid, free the space we put it
             *---* in 
             *---*/
            printf(" Server: freeing uuid string...");
            rpc_string_free(&str_p, &locstatus);
            if (rpcsec2_check_stat(locstatus, "freeing uuid string",
                                  err_stats, &nerr_stats,  debug, 0))
               printf(" Server: Leaking memory :-{\n");
         }
      }
 
      /*---* check the returns to see what the uuid story is *---*/
      if (uuid_v_p)
      {
         printf(" Server: unexporting old uuid from entry for %s...",
                                                          cal_cds_name);
         rpc_ns_binding_unexport(rpc_c_ns_syntax_default, 
                         (unsigned_char_t *)cal_cds_name,
                         (rpc_if_handle_t)NULL, uuid_v_p, &status);

         if (rpcsec2_check_stat(status, "unexporting calendar entry", 
                                         err_stats, &nerr_stats, debug, 0))
         {
            rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
            exit(1);
         }
      }
      if (uuid_from == uuid_from_uuidgen || uuid_from == uuid_from_cds || 
                                             uuid_from == uuid_from_cal)
      {
         if (uuid_from == uuid_from_uuidgen || uuid_from == uuid_from_cal)
         {
            /*---*
             *---* if this is a uuid or new object, do the export 
             *---*/
            obj_uuid_v.uuid[0] = &(calendars_p->cal_uuid);
            obj_uuid_v.count =  1;
            printf(" Server: exporting entry for calendar %s to %s...", 
                         (calendars_p->cal_data)->cal_name, cal_cds_name);
            rpc_ns_binding_export(rpc_c_ns_syntax_default, 
                            (unsigned_char_t *)cal_cds_name,
                            (rpc_if_handle_t)NULL,
                            (rpc_binding_vector_t *)NULL, &obj_uuid_v, 
                            &status);
            if (rpcsec2_check_stat(status, "exporting calendar entry", 
                                         err_stats, &nerr_stats, debug, 0))
            {
               rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
               exit(1);
            }
         }
         if (uuid_from == uuid_from_uuidgen || uuid_from == uuid_from_cds)
         {
            /*---* 
             *---* add the uuid to the uuid vector we will export to the 
             *---* server entry.
             *---*/
            *uuid_pp++ = &(calendars_p->cal_uuid);
            uuid_vec_p->count++;
         }
      }

      /*---* 
       *---* at this point we have an object entry in the namespace that needs
       *---* removing if we are cleaning up later.  Set the "created objects"
       *---* flag if it is not already set. 
       *---*/
      server_objects_created=1;


      /*---* for efficiency's sake *---*/
      srv_cds_name = PVAL(params, param_i, cds_name);
 
      /*---* 
       *---* now add the server's cds entry to the group attribute of the 
       *---* object entry.
       *---*/
      printf(" Server: Adding %s to the group attribute in %s...",
                                                 srv_cds_name, cal_cds_name);
      rpc_ns_group_mbr_add(rpc_c_ns_syntax_default, 
                          (unsigned_char_t *)cal_cds_name, 
                          rpc_c_ns_syntax_default,
                          (unsigned_char_t *)srv_cds_name, &status);
      if (rpcsec2_check_stat(status, "adding server to calendar group",
                                       err_stats, &nerr_stats, debug, 0))
      {
         rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
         exit(1);
      }

      /*---* 
       *---* if we have used all the calendars, then start over with the 
       *---* first, else use the next.
       *---*/
      calendars_p = calendars_p - calendars == ncalendars - 1? calendars:
                                                               calendars_p+1;
   }
   /*---* 
    *---* if we successfully created all of the calendar entries, then
    *---* decrement the curr_seq number so that the cleanup routine won't try
    *---* to remove what is not there.
    *---*/
   curr_seq--;
   if (debug)
   {
      printf(" Server: Dumping server object uuid vector before sort...\n");
      dump_uuid_v(uuid_vec_p, &status);
      if (status)
         printf(" Server: dump of uuid vector failed.\n");
   }

   /*---* sort the calendar entries to make searches faster *---*/
   qsort((void *)calendars, ncalendars, sizeof(cal_entry_t), 
         (int (*)())rpccds3_cal_comp);

   if (debug)
   {
      printf(" Server: Dumping server object uuid vector after sort...\n");
      dump_uuid_v(uuid_vec_p, &status);
      if (status)
         printf(" Server: dump of uuid vector failed.\n");
   }
   /*---* 
    *---* register iface and epv with rpc - if epv_def is specified, then
    *---* pass a NULL pointer for the entry point vector arguement
    *---*/
   printf(" Server: Registering iface/epv...");
   rpc_server_register_if(rpccds3_v1_0_s_ifspec, (uuid_t *)NULL,
     (epv_def? (rpc_mgr_epv_t)NULL: (rpc_mgr_epv_t)&rpccds3_v1_0_epv),
     &status);

   if (rpcsec2_check_stat(status, "registering iface/epv",
                              err_stats, &nerr_stats,  debug, 0))
   {
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }

   /*---* get binding handles allocated - use all protocol sequences *---*/
   printf(" Server: get binding handles allocated...");
   rpc_server_use_all_protseqs((unsigned32)max_calls, &status);

   if (rpcsec2_check_stat(status, "getting binding handles allocated",
                              err_stats, &nerr_stats,  debug, 0))
   {
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }

   /*---* get the binding handles that were allocated *---*/
   printf(" Server: get the allocated binding handles...");
   rpc_server_inq_bindings(&bnd_vctr_p, &status);
   
   if (rpcsec2_check_stat(status, "getting the allocated binding handles",
                              err_stats, &nerr_stats,  debug,0))
   {
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }

   /*---* if debug is set, print the binding handles returned. *---*/
   if (debug != 0 )
   {
      int i;
      
      printf(" Server: Bindings from rpc_server_inq_bindings():\n");

      for (i=0; i< bnd_vctr_p->count; i++)
      {
         rpc_binding_to_string_binding(bnd_vctr_p->binding_h[i], &str_p, 
                                                                 &status);
         if (!rpcsec2_check_stat(status, "converting bindings to strings",
                              err_stats, &nerr_stats,  debug,1))
         {
            printf(" Server:       %d) %s\n", i, str_p);
            rpc_string_free(&str_p, &status);
      
            if (rpcsec2_check_stat(status, "freeing binding string",
                              err_stats, &nerr_stats,  debug,1))
               printf(" Server: Leaking %d bytes of memory :-{\n",
                                                        strlen((char *)str_p));
         }
      }
   }


   /*---* get the current server principle *---*/
   printf(" Server: Get the server principle name...");
   rpc_mgmt_inq_server_princ_name((rpc_binding_handle_t)NULL, 
                                rpc_c_authn_dce_secret, &str_p,  &status);
  
   if (rpcsec2_check_stat(status, "getting server principle name",
                              err_stats, &nerr_stats,  debug, 0))
   {
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(3);
   }

   printf(" Server: Principle name is %s.\n",str_p);

   /*---* set the server_pname pointer to the name *---*/
   server_pname = (char *)str_p;

   /*---* set a pointer to the client name w/o cell stuff *---*/
   client_pname = PVAL(params, param_i, cli_princ);

   printf(" Server: Client principle name is %s\n", client_pname);

   /*---* register the endpoint with the endpoint mapper *---*/
   printf(" Server: register the endpoint...");
   rpc_ep_register(rpccds3_v1_0_s_ifspec, bnd_vctr_p, 
                   uuid_vec_p, 
                   (unsigned_char_t *)"RPC/CDS Systest (rpc.cds.3) interface",
                   &status);
   if (rpcsec2_check_stat(status, "registering with endpoint mapper",
                              err_stats, &nerr_stats,  debug, 0))
   {
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }
   
   /*---* 
    *---* now export the server entry. Note that we don't worry about old
    *---* uuids in the server entry.  This means that the correct and 
    *---* reliable way to access the server is via the calendar objects 
    *---* exported.  If a client acces the server directly, and the server
    *---* contains old objects, then the client could get a bad binding
    *---* back.
    *---*/
   printf(" Server: exporting server entry to %s...", srv_cds_name);
   if (debug)
   {
      printf(" Server: Dumping server object uuid vector pre-export...\n");
      dump_uuid_v(uuid_vec_p, &status);
      if (status)
         printf(" Server: dump of uuid vector failed.\n");
   }

   rpc_ns_binding_export(rpc_c_ns_syntax_default, 
                   (unsigned_char_t *)srv_cds_name,rpccds3_v1_0_s_ifspec,
                    bnd_vctr_p, uuid_vec_p, &status);

   if (rpcsec2_check_stat(status, "exporting server entry",
                              err_stats, &nerr_stats, debug, 0))
   {
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }

   /*---* set the "server exported" flag *---*/
   server_entry_created=1;

#if defined(RPCCDS3_SRV_REPORTING) 
   /*---* set reporting interval... *---*/
   int_from_PVAL(&rep_intvl,params,param_i,rep_int,"reporting interval",status);
   if (status)
      exit(2);
   /*---* tell the user... *---*/
   printf(" Server: Printing status every %d minutes...\n", rep_intvl);
   /*---* set the rep_intvl in seconds *---*/
   rep_intvl *= 60;
         
   /*---* init the cond variable for the reporting thread *---*/
   printf(" Server: Initializing reporting condition variable...");
   if (pthread_cond_init(&rep_timer_cond,pthread_condattr_default) != 0)
   {
      perror(" \n    Error initializing reporting condition variable:");
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }
   printf("done.\n");

   /*---*
    *---* initialize the reporting mutex
    *---*/
   printf(" Server: Initializing the reporting mutex...");
   if (pthread_mutex_init(&rep_timer_mutex,pthread_mutexattr_default) != 0)
   {
      perror(" \n    Error initializing reporting mutex:");
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }
   printf("done.\n");

   /*--- * create the reporting thread *---*/
   printf(" Server: Spawning reporting thread...");
   USE_PTHREAD(thread_tbl, thd_array, RPCCDS3_REP_THD);
   if (pthread_create(thread_tbl[RPCCDS3_REP_THD].thd_id,
                      pthread_attr_default, 
                      (pthread_startroutine_t)rep_timer, 
                      (pthread_addr_t)&rep_intvl) != 0 )
   {
      perror(" \n    Error creating reporting thread:");
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }
   printf("done.\n");
#endif /*---* defined(RPCCDS3_SRV_REPORTING) *---*/

   /*---*
    *---* initialize the mutex that serializes access to the global
    *---* call stats counters.
    *---*/
   printf(" Server: Initializing the call stats mutex...");
   if ( pthread_mutex_init(&stats_update_mutex, pthread_mutexattr_default)!=0)
   {
      perror(" \n    Error initializing call stats mutex:");
      exit(1);
   }
   printf("done.\n");

   /*---* 
    *---* create the thread that will handle signals
    *---* We do this right before the server listen call so that if a signal
    *---* is received before the server starts listening, the signal thread
    *---* does not compete trying to cleanup while the main thread is creating.
    *---*/
   printf(" Server: Spawning signal catcher thread...");
   USE_PTHREAD(thread_tbl, thd_array, RPCCDS3_SIG_THD);
   if (pthread_create(thread_tbl[RPCCDS3_SIG_THD].thd_id, 
       pthread_attr_default, 
       (pthread_startroutine_t)signal_catcher, 
       (pthread_addr_t)sig_hndlr_arg) != 0 )
   {
      perror(" \n    Error creating signal catcher thread:");
      rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
      exit(1);
   }
   printf("done.\n");
   


   /*---* listen for requests *---*/
   printf(" Server: Ready for requests\n");
   fflush(stdout);
   rpc_server_listen((unsigned32)max_exec, &status);
  
   if (rpcsec2_check_stat(status, "starting to listen",
                              err_stats, &nerr_stats,  debug, 0))
   { 
       printf(" Server: Error starting to listen.\n");
       rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
       exit(1);
   }
   else
   {
       printf(" Server: Stopped from listening...clean up and exit.\n");
       rpccds3_cleanup(thread_tbl[RPCCDS3_MAIN_THD].thd_id);
       printf(" Server: Exitting.\n");
       exit(0);
   }   

} /*---* end of main() *---*/



/*---*
 *---* define a macro to lock the stats update mutex, update the counter,
 *---* and unlock it
 *---*/
#define  RPCCDS3_LOCK_AND_UPD(counter) { \
                       if (debug) \
                          printf(" Server: updating stats..."); \
                       pthread_mutex_lock(&stats_update_mutex); \
                       counter++; \
                       pthread_mutex_unlock(&stats_update_mutex); \
                       if (debug) \
                          printf(" done\n"); \
				       }

calndr_data_p_t
rpccds3_send_cal(rpc_binding_handle_t bind_h, idl_long_int *stat,
                error_status_t *fstat,  error_status_t *cstat)
{
   cal_entry_p_t cal_e_p;
   uuid_t bind_h_uuid;
   struct timespec now,
                   wakeup;
   unsigned32 status, onow, wakeup_time;
   pthread_mutex_t call_delay_mutex;
   int i;
   signed32 res;
   extern char *rpccds3_send_cal_errs[];   

   /*---*
    *---* until we set up seperate client and server idl files, 
    *---* we need to set the fault and comm status variables.
    *---*/
   *cstat = *fstat = rpc_s_ok;
  
   if (debug)
   {
      printf(" Server: Wakeup in rpccds3_send_cal!!\007\n");
   }

   /*---* 
    *---* update total call counter.  We need to take the lock on the 
    *---* update mutex to serialize with other threads
    *---*/
   RPCCDS3_LOCK_AND_UPD(tot_calls)

   /*---* check the authentication info in the binding handle *---*/
   *stat=0;
   rpccds3_srv_check_auth(bind_h, (unsigned32 *)stat);

   /*---* if the authentication routine rejects the access... *---*/
   if (*stat != (idl_long_int)rpccds3_srv_auth_accept)
   {
      /*---* if debug set print a message *---*/
      if (debug)
         printf(" Server(send_cal): Authentication failed...returning.\n\n");
      RPCCDS3_LOCK_AND_UPD(calls_failed)
      return((calndr_data_p_t) NULL);
   }
   if (debug)
   {
      unsigned_char_t *str_b, *uuid, *prot, *addr, *endpt, *opts;

      printf(" Server(send_cal): converting binding...\n");
      rpc_binding_to_string_binding(bind_h, &str_b, &status);

      printf(" Server(send_cal): Decomposing binding handle...\n");
      rpc_string_binding_parse(str_b, &uuid, &prot, &addr, &endpt, &opts,
                               &status);
      printf(" Server(send_cal): binding contains:\n");
      printf("    uuid: %s\n    prot: %s\n    addr: %s\n    endpt: %s\n    opts: %s\n",
              uuid, prot, addr, endpt, opts);
      rpc_string_free(&str_b, &status); rpc_string_free(&uuid, &status); 
      rpc_string_free(&prot, &status);  rpc_string_free(&addr, &status); 
      rpc_string_free(&endpt, &status); rpc_string_free(&opts, &status);
   }
      
      
   /*---* get the object uuid from the handle *---*/
   if (debug)
      printf(" Server: getting object uuid from binding handle...");
   rpc_binding_inq_object(bind_h, &bind_h_uuid, &status);
   if (rpcsec2_check_stat(status, "getting uuid from binding handle", 
                                  err_stats, &nerr_stats, debug, 1))
   {
      if (debug)
         printf(" Server(send_cal): bad binding handle\n");
      *stat = (idl_long_int)rpccds3_send_cal_bad_h;
   }
   else if (uuid_is_nil(&bind_h_uuid, &status) == TRUE)
   {
      if (debug) 
      {
         unsigned_char_t *uuid_str;
         uuid_to_string(&bind_h_uuid, &uuid_str, &status);
         printf(" Server: uuid is : %s\n", uuid_str);
         printf(" Server(send_cal): no uuid in handle\n");
         rpc_string_free(&uuid_str, &status);
      }
      *stat = (idl_long_int)rpccds3_send_cal_no_uuid;
   }
   else
   {
      /*---* 
       *---* the first thing we want to do is make sure we don't return too 
       *---* fast.  We take the size of the array and divide by 3, and wait 
       *---* for the number seconds we get back; ie: if the array is 10 
       *---* elements long, we wait for 10/3 = 3 seconds before processing 
       *---* the array.  This should be enough to make calls get buffered.
       *---*
       *---* Note the extreme bogusness of this section in the multi thread
       *---* environment.  I can't use the system timer functions very well
       *---* because cma uses the timer signals for thread scheduling; ie:
       *---* alarm(n) is out.  So I turn to the cma paradigm for timing, which
       *---* is to do a pthread_cond_timedwait() for the amount of time I want
       *---* to wait.  Now I don't care about the mutex or the condition it 
       *---* guards; all I am interested in is the time going by.  I would like
       *---* to say "declare some global junk mutex and cond variable, and then
       *---* just do timedwaits on them in each thread", BUT NOOOOOO!  The 
       *---* sticking point here is that there is no way in the pthreads 
       *---* package to figure out who has a particular mutex locked; ie: the
       *---* equivilent of a call like
       *---*     pthread_t pthread_mutex_lock_owner(pthread_mutex_t *mutex).
       *---* Since I can't figure out, after the cond_timedwait, whether or
       *---* or not I have the lock, I can't figure out whether or not to do 
       *---* the pthread_mutex_unlock.  So I am hosed.  If I try to unlock a 
       *---* mutex which I do not have locked, then I am in error.  If I am in
       *---* the thread that was lucky enough to have locked the thread, and
       *---* I do not unlock the mutex before returning, then subsequent RPC's
       *---* will block trying to lock a locked mutex.
       *---* This means that either I serialize the threads via a hierarchical
       *---* locking scheme, so that I know which thread gets the lock, or I 
       *---* have to create a seperate junk mutex for each thread, just so that
       *---* I can honor the semantics of cond_timedwait.  So in the interests
       *---* of concurrency, I create the stinking junk mutex in each thread.
       *---*/
      /*---*  init the stinking mutex *---*/
      if (pthread_mutex_init(&call_delay_mutex, pthread_mutexattr_default)!= 0)
      {
         perror(" \n    Error initializing duration mutex:");
         *stat = (idl_long_int)rpccds3_send_cal_unknown;
         RPCCDS3_LOCK_AND_UPD(calls_failed)
         return ((calndr_data_p_t) NULL);
      }

      /*---* get the current time... *---*/
      getclock(TIMEOFDAY, &now);
      onow=now.tv_sec;
      wakeup_time = (unsigned32)(now.tv_sec + srv_call_delay);
      wakeup.tv_sec = wakeup_time;
      wakeup.tv_nsec = 0;

      /*---*
       *---* we lock the mutex here solely because the timed wait call fails
       *---* if we don't have a lock when we call it.
       *---*/
      pthread_mutex_lock(&call_delay_mutex);
    
      if (debug)
         printf(" Server(send_cal): sleeping for %d seconds..\n",
                                                   wakeup_time - now.tv_sec);

      /*---* while condition guards against spurious wakeup *---*/
      while ( now.tv_sec < wakeup_time )
      {
         /*---* timedwait waits till we should start processing *---*/
         pthread_cond_timedwait(&call_delay_cond, &call_delay_mutex, &wakeup);

         /*---* and find out what time it is for the while condition *---*/
         getclock(TIMEOFDAY, &now);
      }
    
      /*---* unlock the mutex *---*/
      pthread_mutex_unlock(&call_delay_mutex);
      /*---* destroy the mutex since it is no longer needed *---*/
      pthread_mutex_destroy(&call_delay_mutex);

      if (debug)
         printf(" Server(send_cal: slept for %d seconds..\n", 
                                                        wakeup_time - onow);
      /*---* 
       *---* we have a good uuid, so search for a calendar entry with the same
       *---* uuid.  We use linear search since at the time of creation, we 
       *---* expect only in the vicinity of 20 - 200 elements in the array,
       *---* and we don't fee that this necessitates the impementation of 
       *---* hashing, etc.
       *---*/
      if (debug)
         printf(" Server(send_cal): looking for match for uuid...\n");
      for (*stat = rpccds3_send_cal_no_cal, cal_e_p = calendars; 
           cal_e_p - calendars < ncalendars ; cal_e_p++)
      {
         if (debug)
            printf(" Server(send_cal): comparing uuids...");
         res = uuid_compare(&(cal_e_p->cal_uuid), &bind_h_uuid, &status);
         if (rpcsec2_check_stat(status, "comparing uuids", 
                                err_stats, &nerr_stats, debug, 1))
         {
            if (debug)
               printf(" Server(send_cal): bad uuid in binding handle\n");
            *stat = (idl_long_int)rpccds3_send_cal_bad_uuid;
            break;
         }
         else if (!res)
	 {
            if (debug)
               printf(" Server(send_cal): got the calendar(%s)\n", 
                                               cal_e_p->cal_data->cal_name);
            *stat = (idl_long_int)rpccds3_send_cal_ok;
            break;
	 }
         else if (res == 1)
	 {
            if (debug)
               printf(" Server(send_cal): no match for uuid\n");
            *stat = (idl_long_int)rpccds3_send_cal_no_cal; 
            break;
	 }
         
      }
   }
   if (cal_e_p - calendars > ncalendars)
   {
      if (debug)
         printf(" Server(send_cal): no match for uuid\n");
      *stat = (idl_long_int)rpccds3_send_cal_no_cal;
   }
   if (*stat == (idl_long_int)rpccds3_send_cal_ok)
   {
      RPCCDS3_LOCK_AND_UPD(calls_passed)
      return(cal_e_p->cal_data);  
   }
   else
   {
      if (debug)
         printf(" Server(send_cal): %s...\n",rpccds3_send_cal_errs[*stat]); 
      RPCCDS3_LOCK_AND_UPD(calls_failed)
      return((calndr_data_p_t)NULL);
   }
}

void rpccds3_nop(rpc_binding_handle_t bind_h, error_status_t *fstat,
                                                       error_status_t *cstat)
{
  if (debug)
  {
     printf(" Server: Wakeup in rpccds3_nop!!\n");
  }
}

int
rpccds3_cal_comp(void *cal_e_p1, void *cal_e_p2)
{
   unsigned32 stat;

   return(uuid_compare(&(((cal_entry_p_t)cal_e_p1)->cal_uuid),
                       &(((cal_entry_p_t)cal_e_p2)->cal_uuid),
                       &stat));
}



/*--------------------------------------------*
 *---* rpccds3_srv_check_auth(binding_handle,  status)
 *---*
 *---*   check to see if the client requesting the service is 
 *---*   authorized to our satisfaction and using the authorization level we
 *---*   want.
 *--------------------------------------------*/
void
rpccds3_srv_check_auth(rpc_binding_handle_t bnd_h, unsigned32 *status)
{
   rpc_authz_handle_t cli_privs;   /*---* cli name or a problem! *---*/
   unsigned_char_p_t  srv_princ;   /*---* server princ spec'ed by cli *---*/
   unsigned32  protcn_lev,         /*---* level of authenticated RPC *---*/
               authn_svc,           
               authz_svc,
               loc_status;
   char        *cp;                /*---* utility pointer *---*/

   /*---* rip the auth info out of the binding handle *---*/
   if (debug)
      printf(" Server: getting client auth* info...");
   rpc_binding_inq_auth_client(bnd_h, &cli_privs, &srv_princ, &protcn_lev, 
                               &authn_svc, &authz_svc, &loc_status);

   if (debug && loc_status == rpc_s_ok)
   {
      printf(" Server:  Binding handle contained:\n");
      printf("   server: %s   expect: %s\n", (char *)srv_princ,server_pname);
      SET_AUTHZ_SVC_STR(cp, authz_svc, " Server")
      printf("   authzn: %s(%u)   expect: %s(%u)\n", cp, authz_svc, 
                                               "name based", rpc_c_authz_name);
      SET_AUTHN_SVC_STR(cp, authn_svc, " Server")
      printf("   authnn: %s(%u)   expect: %s(%u)\n", cp, authn_svc,
                                        "dce secret", rpc_c_authn_dce_secret);
      printf("   client: %s   expect: %s\n", (char *)cli_privs, client_pname);
      SET_PROT_LEVEL_STR(cp, protcn_lev, " Server")
      printf("   protcn: %s(%u)   expect: %s(%u)\n", cp, protcn_lev, 
                                            protect_level_str, protect_level);
   }
   if (rpcsec2_check_stat(loc_status, "getting client auth info",
                              err_stats, &nerr_stats, debug, 1))
      *status = rpccds3_srv_auth_cant_get_info;

   /*---* 
    *---* check the server principle name. if names don't match, we have
    *---* a problem.  Note that we don't check this in the unauthenticated
    *---* case, since the server name is not passed in the binding.
    *---*/
   else if (protcn_lev != rpc_c_protect_level_none && 
                                       strcmp((char *)srv_princ, server_pname))
   {
      *status = rpccds3_srv_auth_bad_srv;        /*---* set bad status *---*/

      /*---*  print the error (if debug on) *---*/
      if (debug)
         printf(" Server: Bad server principle name(%s)\n",srv_princ);
      
   }
   /*---* check the authorization service, if not rpc_c_authz_name *---*/
   else if (authz_svc != rpc_c_authz_name)
   {
      *status = rpccds3_srv_auth_unsup_authz;  /*---* set bad status *---*/
   
      /*---* print the error (if debug on) *---*/
      if (debug)
         printf(" Server: Unsupported authorization service.\n");

   }
   /*---* check authentication service, if not rpc_c_authn_dce_secret... *---*/
   else if (authz_svc != rpc_c_authn_dce_secret)
   {
      *status = rpccds3_srv_auth_unsup_authn;  /*---* set bad status *---*/
   
      /*---* print the error (if debug on) *---*/
      if (debug)
         printf(" Server: Unsupported authentication service.\n");

   }
   /*---* 
    *---* check the protection level.  We require it to be equal to or 
    *---* higher than what we expect.
    *---*/
   else if (protcn_lev < protect_level) 
   {
      *status = rpccds3_srv_auth_unsup_lvl;  /*---* set bad status *---*/
   
      /*---* print the error (if debug on) *---*/
      if (debug)
         printf(" Server: Unsupported protection level.\n");
   }
   /*---*
    *---*  check the client name. we don't bother if no authentication is
    *---*  specified, since the parameter will contain nothing useful.
    *---*/
   else if (protcn_lev != rpc_c_protect_level_none)
   {
       /*---* get a pointer to what should be the name w/o cell stuff *---*/
       unsigned_char_p_t ucp = 
               (unsigned_char_p_t)( (char *)cli_privs + (strlen(cli_privs) - strlen(client_pname)));

       /*---* if the names don't match *---*/
       if (strcmp(client_pname, (char *)ucp))
       {
          *status = rpccds3_srv_auth_bad_cli;  /*---* set bad status *---*/
   
          /*---* print the error (if debug on) *---*/
          if (debug)
             printf(" Server: Invalid client principle name(%s).\n",
                                           (unsigned_char_p_t)cli_privs);
       }
   }

   /*---* 
    *---* if status is clear here, then we made it past all of the checks
    *---*/
   if (*status == 0 )
   {
      *status = rpccds3_srv_auth_accept;  /*--* set good status *---*/
       
      /*---* print a message (if debug on) *---*/
      if (debug)
         printf(" Server: Authenticated client.\n");
   }

}

/*---*
 *---* rpccds3_cleanup(thread_id) - cleansup the namespace, if requested,
 *---*                              and kills spawned threads.
 *---*/
void rpccds3_cleanup(pthread_t *call_thd)
{
   int i;
   char buf[256];
   unsigned32 l_stat;
   time_t tbuf;
   char *t_ptr;

   /*---* report cleanup start time *---*/
   (void)time(&tbuf);
   printf(" Server: STARTing server cleanup at %s...\n\n", ctime(&tbuf));

   /*---*
    *---* if we are to remove the server entry and the server entry was 
    *---* exported to the namespace...
    *---*/
   if (rem_srv_ent && server_entry_created) 
   { 
      char *srv_name = PVAL(params, param_i, cds_name);
			    
      /*---* report the time and remove the server entry *---*/
      (void)time(&tbuf);
      CTIME_NO_NL(t_ptr, &tbuf);
      printf(" Server: removing the server entry %s at %s...",srv_name, t_ptr);
      rpc_ns_mgmt_entry_delete(rpc_c_ns_syntax_default, 
                 (unsigned_char_t *)srv_name, &l_stat);
      rpcsec2_check_stat(l_stat, "Removing server namespace entry",
                                           err_stats, &nerr_stats, debug, 0);
   }
   /*---*
    *---* if we are to remove the server objects  and server objects were
    *---* exported to the namespace...
    *---*/
   if (rem_srv_objs && server_objects_created)
   {
      int i;
      
      /*---* report start time for removing the objects *---*/
      (void)time(&tbuf);
      CTIME_NO_NL(t_ptr, &tbuf);
      printf(" Server: Removing cds calendar objects from %s at %s.\n",
                             PVAL(params, param_i, obj_dir), t_ptr);
      for (i = cdscalseq; i <= curr_seq; i++)
      {
         /*---* construct the calendar name *---*/
         sprintf(cal_cds_name, "%s/rpccds3_cal%d", 
                                            PVAL(params, param_i, obj_dir), i);
         printf("          removing %s...",cal_cds_name);

         /*---* remove it *---*/
         rpc_ns_mgmt_entry_delete(rpc_c_ns_syntax_default, 
                                  (unsigned_char_t *)cal_cds_name, &l_stat);
         rpcsec2_check_stat(l_stat, "Removing server object entry",
                                           err_stats, &nerr_stats, debug, 0);
      }

      /*---* report end time for removing the objects *---*/
      (void)time(&tbuf);
      CTIME_NO_NL(t_ptr, &tbuf);
      printf(" Server: Done removing cds calendar objects from %s at %s.\n",
                             PVAL(params, param_i, obj_dir), t_ptr);
   } 

   /*---* report start time for thread cancels *---*/
   (void)time(&tbuf);
   printf(" Server: Starting thread cancels at %s\n", ctime(&tbuf));

   /*---* cancel all of the threads *---*/
   for (i=0; i < RPCCDS3_SRV_NTHDS; i++)
   {
      printf(" Server:    %s...", thread_tbl[i].thd_label);


      /*---* try the thread cancel, but not for the calling thread *---*/
      if (thread_tbl[i].thd_id == (pthread_t *)0)
      {
         printf(" Server: %s not created...skipping...\n",
                                          thread_tbl[i].thd_label);
         continue;
      }    

      if (pthread_equal(*call_thd, *(thread_tbl[i].thd_id)))
      {
         printf(" Server: calling thread...skipping...\n");
         continue;
      } 
      
      /*---* 
       *---* if pthread_cancel returns non-zero, then it is an error.  
       *---* Create an appropriate message and print it.
       *---*/
      if (pthread_cancel(*(thread_tbl[i].thd_id)))
      {
          sprintf(buf, "\n Server: trying to kill %s thread", 
                                                      thread_tbl[i].thd_label);
          perror(buf);
      }
      else
          printf("done.\n");
   }
  
   /*---* report the time we finished the thread cancels *---*/
   (void)time(&tbuf);
   printf(" Server: Thread cancels complete at %s\n\n", ctime(&tbuf));

   /*---* report cleanup end time *---*/
   (void)time(&tbuf);
   printf(" Server: COMPLETED server cleanup at %s\n\n", ctime(&tbuf));
}

void
rpccds3_rep_gen(int call_ctxt)
{
   time_t tbuf;
   double avg;
   int pid=getpid();
   char toutbuf1[30], toutbuf2[30], *gcp, *gcp2;
   
   /*---*
    *---* if we are in debug mode...
    *---*/
   if (debug)
      /*---* print hi message *---*/
      printf("Hi from rpccds3_rep_gen: %s %d...\n", 
                     (call_ctxt? "handling signal: ": "called from code: "), 
                     call_ctxt);
   
   /*---*
    *---* print the info
    *---*/
   (void)time(&tbuf);
   /*---* strip the newline from the time string *---*/
   CTIME_NO_NL(gcp, &tbuf);
   strcpy(toutbuf1, gcp); 
   gcp=toutbuf1;
   CTIME_NO_NL(gcp2, &last_id_refr);

   printf(" Server %s Host: %s O/S: %s Time: %s Calls: %d Pass: %d Fail: %d Min: %u sec %d nsec Max: %u sec %d nsec Elapsed: %u sec %d nsec Avg: %.9f sec id_refr: %d last: %s\n",
   (tot_calls == 0 && call_ctxt == 1? "STARTING":
    (call_ctxt == -1? "FINISHED":
     (call_ctxt == -2?"DIED":
      (call_ctxt == 2? "STOPPED":"RUNNING")))), 
   uname_vals.nodename, uname_vals.sysname, gcp, tot_calls, 
   calls_passed, calls_failed, min_time.tv_sec, min_time.tv_nsec, 
   max_time.tv_sec, max_time.tv_nsec, elpsd_time.tv_sec, elpsd_time.tv_nsec,
   (tot_calls?(double)elpsd_time.tv_sec/(double)tot_calls: (double)0), 
   id_refr_cnt, gcp2);
   
   /*---* print any errors encountered so far *---*/
   dump_errs(pid, "Server", err_stats, nerr_stats);

   fflush(stdout);
}
