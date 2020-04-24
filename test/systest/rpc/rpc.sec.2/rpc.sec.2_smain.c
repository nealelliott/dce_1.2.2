/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc.sec.2_smain.c,v $
 * Revision 1.1.16.2  1996/02/17  23:28:14  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:19  marty]
 *
 * Revision 1.1.16.1  1995/12/11  22:44:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/10/26  19:46 UTC  bissen
 * 	merge from HPDCE01
 * 
 * 	fixed calculation of expiration time, pulled out key dumping.
 * 	[1993/01/07  19:06:49  mhickey]
 * 
 * Revision 1.1.4.2  1993/08/25  20:20:22  bissen
 * 	Changes for TET
 * 	[1993/08/25  20:19:45  bissen]
 * 
 * Revision 1.1.13.2  1994/02/22  18:39:06  ohara
 * 	check for no more members before status check
 * 	[1994/02/18  16:10:59  ohara]
 * 
 * Revision 1.1.13.1  1993/12/22  20:48:06  mhickey
 * 	no changes, different backing tree
 * 	[1993/12/22  20:44:31  mhickey]
 * 
 * 	Numerous changes in the course of fixing two defects:
 * 	General
 * 	-------
 * 	 - Made report interval come from the RPC_SRV_DEF_REP_INTVL
 * 	   config file variable.
 * 	 - Moved the start of the signal thread to just before the
 * 	   server_listen call so that the signal thread is not trying
 * 	   to remove CDS entries while the main thread is trying to
 * 	   create them (if a signal hits while they are getting
 * 	   created.)
 * 	 - Cleaned up the includes/defines and thread handling
 * 	 - streamline the value checking and conversion code.
 * 	 - Make sig_handler use the arg_p parameter as an idication of how to stop
 * 	   the main thread.
 * 	 - Fix/add comments and printfs
 * 	 - Define action for after server stops listening and make this the default
 * 	   way for the server to exit.
 * 	 - Fix thread type prints.
 * 
 * 	Defect 7993 - server doesn't clean up namespace
 * 	----------------------------------------------------
 * 	 - Change kill threads in server to cleanup and make it remove server entry
 * 	 - Cleans up objects and server entry from namespace if requested.
 * 
 * 	Defect 7743 - Doesn't work intercell
 * 	----------------------------------------------
 * 	 - Make the client name the simple name of the client so intercell works
 * 	 - Fix faulty logic in server authentication to make intercell work.
 * 	[1993/12/22  18:39:43  mhickey]
 * 
 * Revision 1.1.11.3  1993/10/20  19:37:02  ohara
 * 	svr4 ref port
 * 	[1993/10/20  19:36:21  ohara]
 * 
 * Revision 1.1.11.2  1993/10/01  16:40:48  mhickey
 * 	This submission fixes the follwoing defect:
 * 
 * 	 Defect 7724: rpc.cds.3 and rpc.sec.2 servers buffer output.
 * 
 * 	                       This is the rpc.cds.3 server part of the fix.
 * 	                       The server now does a 'setbuf(stdout,NULL)' to
 * 	                       avoid buffering messages.
 * 	[1993/10/01  16:33:12  mhickey]
 * 
 * Revision 1.1.11.1  1993/09/09  19:13:05  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/09  19:12:02  cmckeen]
 * 
 * Revision 1.1.9.2  1993/08/16  14:43:34  eheller
 * 	TIMEOFDAY fix.
 * 	[1993/07/23  18:31:15  eheller]
 * 
 * 	File tetified.
 * 	[1993/07/23  17:48:09  eheller]
 * 
 * Revision 1.1.2.8  1993/03/12  04:07:24  mhickey
 * 	Final checkin for comment fixes before final submit
 * 	[1993/03/12  04:00:21  mhickey]
 * 
 * 	Updating comments to agree with the current state of the world.
 * 	[1993/03/10  21:55:29  mhickey]
 * 
 * Revision 1.1.2.7  1993/02/25  20:16:45  mhickey
 * 	Added server reporting and moved setting of comm and fault status to
 * 	server manager.
 * 	[1993/02/19  21:23:58  mhickey]
 * 
 * Revision 1.1.2.5  1993/01/25  21:52:01  mhickey
 * 	Added an rpcsec2_rep_gen function to the server main to
 * 	satisfy the assumptions of the maintain_id and sig_handler
 * 	routines (the lack of this routine was causing
 * 	unresolved symbol errors that broke the build of 1/23/93).
 * 	Added the signal handler to get status from the server.
 * 
 * 	Defect 6993.
 * 	[1993/01/25  21:31:23  mhickey]
 * 
 * Revision 1.1.2.4  1993/01/22  20:33:51  mhickey
 * 	Added prototype for rpcsec3_kill_threads at the top
 * 	of the file.
 * 	[1993/01/20  19:30:20  mhickey]
 * 
 * 	Pulled the maintain_id routin out into a generic routine
 * 	in rpc.sec.2_maintain_id.c.  Added rpcsec2_kill_threads to
 * 	kill the threads we manage, and inserted a call to the
 * 	routine before every exit after the refresh thread is
 * 	created. Added an array to hold the pthread_t's of the
 * 	threads we manage, and defines for the semantics
 * 	of the indexes thereof
 * 	[1993/01/19  18:37:06  mhickey]
 * 
 * Revision 1.1.2.3  1993/01/12  17:17:04  mhickey
 * 	Trying to get around a bsubmit problem. No changes.
 * 	[1993/01/12  15:04:31  mhickey]
 * 
 * 	Check in for bsubmit of changes in notes below.
 * 	[1993/01/12  15:01:55  mhickey]
 * 
 * 	*** empty log message ***
 * 	[1993/01/05  21:24:42  mhickey]
 * 
 * 	Ported  to AIX.
 * 	[1993/01/02  20:29:56  mhickey]
 * 
 * Revision 1.1.2.2  1992/12/03  19:54:26  mhickey
 * 	Final check-in before initial submit.
 * 
 * 	Added automatic id refreshes via timer thread that refreshes
 * 	the id one minute before it is to expire.  Added a timedwait
 * 	to the rpcsec2_cnf_str routine to make the operations take
 * 	longer so that request buffering will take place.  Added
 * 	-l option to allow setting of protection level.
 * 
 * 	Defect 5887.
 * 	[1992/12/03  17:49:06  mhickey]
 * 
 * 	 Added routine tstrpc_server_check_auth() to extract the auth
 * 	 info from the client's binding handle, and verify it against
 * 	 what we expect.  Verification is done by server name, authz
 * 	 service, authn service, protection level and client name.
 * 	 Added calls to this routine to the two entry point routines.
 * 	 Made all printf's in the routines that are executed as a
 * 	 result of client calls conditional on debug to prevent
 * 	 a lot of useless noise during normal operation.
 * 
 * 	 We have authenticated RPC!!
 * 	[1992/10/16  13:33:07  mhickey]
 * 
 * 	Added tstrpc_cnf_str function and debug command line option.
 * 
 * 	Note that un/exporting and replacing obj UUIDS is still messed.
 * 	[1992/10/10  03:14:38  mhickey]
 * 
 * 	Initial checkin for base for rpc.security.2
 * 	[1992/10/06  17:30:31  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* rpc.sec.2_smain.c - server side of rpc.sec.2 test
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
 *---*    1. Assumes the server prinicipal identity specified in the config 
 *---*       file rpc.sec.2.conf or passed via the -f command line option.
 *---*    2. Looks in the CDS namespace for an existing entry for the rpc.sec.2
 *---*       interface (defined in the config file).  If a CDS namespace
 *---*       entry is found, the UUID for the entry is read and used when
 *---*       exporting binding handles to the CDS entry.  
 *---*    3. If an entry exists, but has no UUID, or too many UUIDS (>1),
 *---*       a UUID is created, or all but one UUID is unexported, 
 *---*       respectively.
 *---*    4. If no entry exists, a UUID is created, a CDS entry is created and
 *---*       the UUID is used when exporting binding handles to the CDS entry.
 *---*    5. The server services calls to rpcsec2_nop (adds one to passed value
 *---*       if it even, else returns an error), and rpcsec2_cnf_str (verifies
 *---*       checksums in a passed conformant struct containing a conformant
 *---*       array of ints).  All calls are checked to ensure authenticity or
 *---*       caller identity and protection level of 
 *---*       rpc_c_protect_level_pkt_privacy, or whatever level of privacy
 *---*       is specified on the command line.
 *---* 
 *---* CALLED BY
 *---*    rpc.sec.2_runtest.sh - (the driver for running the test program)
 *---* 
 *---* CALLS INTO 
 *---*    Reads rpc.sec.2.conf or file specified via the -f option for 
 *---*    definitions.
 *---*    rpc.sec.2_chk_stat.c  (rpcsec2_chk_stat() )
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
#include <stdio.h>
#include <sys/utsname.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/id_base.h>
#include <pthread.h>

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
 *---* Global variables
 *-------------------------------------------*/
/*---*
 *---* Entry point vector 
 *---*/
rpcsec2_v1_1_epv_t rpcsec2_v1_1_epv = {rpcsec2_nop, rpcsec2_cnf_str};

/*-------------------------------* 
 *---* Command line option flags
 *-------------------------------*/
int debug,            /*---* variable to turn on to get debug output     *---*/
    epv_def,          /*---* variable to turn on to let epv default      *---*/
    use_null,         /*---* variable to turn on to export no UUID       *---*/
    repl_uuid;        /*---* variable to replace uuid in namespace entry *---*/


/*----------------------*
 *---* Usage message
 *----------------------*/
char *usage = "usage: rpc.sec.2_srv [-n] [-r] [-d] [-e] [-f config_file] [-l prot_level";


/*---------------------------* 
 *---* call stats variables
 *---------------------------*/
int tot_calls,
    calls_passed,
    calls_failed;

/*---*
 *---* timer variables (currently unused )
 *---*/
struct timespec  min_time,      /*---* minimum RPC time          *---*/
                 max_time,      /*---* maximum RPC time          *---*/
                 start_time,    /*---* time we made 1rst RPC     *---*/
                 precall_time,  /*---* time we made this RPC     *---*/
                 postcall_time, /*---* time we got back from RPC *---*/
                 elpsd_time;    /*---* total elapsed RPC time    *---*/



/*----------------------------------------*
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
 *---* client principle name = cell root + name
 *---*/
char *client_pname;

/*---* 
 *---* variable that stores the expiration interval
 *---*/
long  id_lifetime;

/*---*
 *---* thread variables we need to syncronize id refreshes
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
#define RPCSEC2_MAIN_THD  0
#define RPCSEC2_REFR_THD  1
#define RPCSEC2_SIG_THD   2
#define RPCSEC2_REP_THD   3
#define RPCSEC2_SRV_NTHDS 4

/*---* array of threads we spawn *---*/
pthread_t thd_array[RPCSEC2_SRV_NTHDS];

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

#if defined(RPCSEC2_SRV_REPORTING)
  /*-----------------------------------*
   *---* thread variables we need to time reporting intervals 
   *-----------------------------------*/
  pthread_cond_t rep_timer_cond;
  pthread_mutex_t rep_timer_mutex;

#endif /*---* defined(RPCSEC2_SRV_REPORTING) *---*/

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


/*---------------------------------*
 *---* Namespace usage variables
 *---------------------------------*/
/*---*
 *---* flags that indicates to the cleanup routine whether the namespace server
 *---* entry has been created and should be cleaned up.
 *---*/
int server_entry_created;
int rem_srv_ent;


/*-----------------------------------------* 
 *---* Configuration variables/file management
 *-----------------------------------------*/
/*---* 
 *---* the array of parameter specs we need - passed to rd_conf()
 *---*/
param_spec_val_t params[] = {
                              {"RPCSEC2_SRV_KEYTAB_DIRPATH", 0},
                              {"RPCSEC2_SRV_KEYTAB_FN", 0},
                              {"RPCSEC2_SRV_PRINC_NAME", 0},
                              {"RPCSEC2_CLI_PRINC_NAME", 0},
                              {"RPCSEC2_SRV_CDS_NAME", 0},
                              {"RPCSEC2_SRV_MAX_CALLS", 0},
                              {"RPCSEC2_SRV_MAX_EXEC", 0},
                              {"RPCSEC2_SRV_REM_SRV_ENT", 0},
#if defined(RPCSEC2_SRV_REPORTING)
                              {"RPCSEC2_SRV_DEF_REP_INTVL", 0},
#endif /*---* defined(RPCSEC2_SRV_REPORTING) *---*/
                              {"RPCSEC2_PROT_LEVEL", 0}
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
       call_max,
       exec_max,
       rem_if_entry,
#if defined(RPCSEC2_SRV_REPORTING) 
       rep_int,
#endif /*---* defined(RPCSEC2_SRV_REPORTING) *---*/
       prot_lev
     } param_i;


/*------------------------------------------*
 *---* Function prototypes
 *------------------------------------------*/
void dump_uuid_v(uuid_vector_p_t, unsigned32 *);
void rpcsec2_srv_check_auth(rpc_binding_handle_t, unsigned32 *);
void maintain_id(int *);
void dump_key(sec_passwd_rec_t *);
void rpcsec2_cleanup(pthread_t *);
int signal_catcher (char *);

#if defined(RPCSEC2_SRV_REPORTING)
void rep_timer(int *);
#endif /*---* defined(RPCSEC2_SRV_REPORTING) *---*/

void rpcsec2_rep_gen(int);


/*-------------------------------------------------*
 *---* pointers to functions for external routines
 *-------------------------------------------------*/
/*---* report generation function *---*/
void (*rep_gen_func_p)() = rpcsec2_rep_gen;
/*---* thread cancel funtion      *---*/
void (*cleanup_func_p)() = rpcsec2_cleanup;

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
   int stat2,                         /*---* Generic status return param *---*/
       max_calls,                     /*---* max cuncurrent calls *---*/
       max_exec,                      /*---* max concurrent threads  *---*/
       pid=getpid(),
       rep_intvl=0,
       i;
   uuid_t obj_uuid;                   /*---* receives uuid if we are generating
                                       *---* one  
                                       *---*/
   uuid_vector_t obj_uuid_v;          /*---* uuuid vector for rpc register
                                       *---* interface call 
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
                                       *---* whether passwd has expired or not
                                       *---*/

   char                  *cp,
                         *sig_hndlr_arg = "server",
                         *conf_file=NULL; /*---* pointer to config file name 
                                           *---* if passed on the command line
                                           *---*/
   time_t           tbuf;             /*---* to get the current time *---*/
   signed32         exp_time;         /*---* when our identity expires *---*/
   pthread_t        timer_thread;     /*---* handle of timer thread *---*/

                    
extern char    *rd_conf_errs[];

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
                   

            case 'n': /*---* use a NULL uuid pointer *---*/
                      /*---* only one of -r or -n allowed *---*/
                      if (repl_uuid)
		      {
                         printf(" Server: only one of -n or -r allowed\n");
                         printf(" Server: %s\n", usage);
                         exit(1);
		      }
                      use_null = 1;
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
                                      (conf_file? conf_file: RPCSEC2_CONF_FN));
   rd_conf((conf_file? conf_file: RPCSEC2_CONF_FN), params, 
                   sizeof(params)/sizeof(param_spec_val_t), &status);
   /*---* if we did not succeed... *---*/
   if (status != rdconf_ok)
   {
      /*---* report error *---*/
      printf("\n\n Server: %s (%s).\n", rd_conf_errs[status],
                                   (conf_file? conf_file: RPCSEC2_CONF_FN));
      /*---* exit *---*/
      exit(1);
   }
   /*---* report success *---*/
   printf("\ndone.\n\n");
   
   /*---* max calls *---*/
   int_from_PVAL(&max_calls,params,call_max,"max calls", status);
   if (status)
      exit(2);

   /*---* max exec threads *---*/
   int_from_PVAL(&max_exec,params,exec_max,"max exec threads",status);
   if (status)
      exit(2);

   /*---* remove server entry flag *---*/
   int_from_PVAL(&rem_srv_ent,params,rem_if_entry,"remove server",status);
   if (status)
      exit(2);
   else
   {
      if (rem_srv_ent)
         printf(" Server: Server entry WILL BE REMOVED on exit...\n");
      else
         printf(" Server: Server entry WILL NOT BE REMOVED on exit...\n");
   }

   /*---* set the protection level *---*/
   if (protect_level_str)
      cp = protect_level_str;
   else
      cp = PVAL(params, prot_lev);

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

   /*---* register auth info *---*/
   printf(" Server: registering authorization info...");
   rpc_server_register_auth_info((unsigned_char_t *)PVAL(params,srv_princ), 
                                 rpc_c_authn_dce_secret, 
                                 (rpc_auth_key_retrieval_fn_t) NULL,
                                 (void *)keytab,
                                 &status);

   if (rpcsec2_check_stat(status, "registering auth info",
                              err_stats, &nerr_stats,  debug, 0))
      exit(1);

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
   sec_login_setup_identity((unsigned_char_t *)PVAL(params,srv_princ),
                            (sec_login_flags_t)sec_login_no_flags,
                            &login_ctxt, &status);
   
   if (rpcsec2_check_stat(status, "creating login context",
                              err_stats, &nerr_stats,  debug, 0))
      exit(1);

   /*---* Get our secret key from our keytab file *---*/
   printf(" Server: getting secret key from %s...", keytab);
   sec_key_mgmt_get_key(rpc_c_authn_dce_secret, (void *)keytab,
                        (char *)PVAL(params,srv_princ), (unsigned32)0, 
                        &sec_key_p, &status);

   if (rpcsec2_check_stat(status, "getting secret key",
                              err_stats, &nerr_stats,  debug, 0))
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

   /*---* finally, get our context *---*/
   printf(" Server: setting our own context...");
   sec_login_set_context(login_ctxt, &status);

   if (rpcsec2_check_stat(status, "setting our own context",
                              err_stats, &nerr_stats,  debug, 0))
      exit(1);

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
                                                      nerr_stats, debug, 1))   {
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
       int expdif;
       /*---*
        *---* set the alarm for exp_time - now - 60....
        *---*/
       exp_time -= time((time_t *)NULL);
       /*---* round up if necessary *---*/
       exp_time += ((expdif = exp_time%60)? 60 - expdif: 0);
       id_lifetime = exp_time - (exp_time > 60? 60: 0);
       printf(" Server: identity lifetime = %d mins, refresh interval is %d mins.\n", exp_time/60, id_lifetime/60);
       printf(" Server: Spawning refresh thread...");
       USE_PTHREAD(thread_tbl, thd_array, RPCSEC2_REFR_THD)
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

   /*---* store our thread id *---*/
   USE_PTHREAD(thread_tbl, thd_array, RPCSEC2_MAIN_THD);
   *(thread_tbl[RPCSEC2_MAIN_THD].thd_id) = pthread_self();

   

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
      rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(1);
   }
   printf("done.\n");


   /*-----------------------------------------------------*
    *---* check for presence/validity of current namespace
    *---* entry for this server
    *-----------------------------------------------------*
    *---*
    *---* to prevent clients from getting a bogus (stale) UUID in the 
    *---* imported binding handle, we try to find a name service entry for 
    *---* ourselves, and get the UUID (if there is one) out of it.  If 
    *---* repl_uuid is set, make a list of the UUIDS we find in the namespace 
    *---* entry, unexport it/them, generate a new uuid, and export it to the
    *---* namespace entry.  If repl_uuid is not set, we just use the old uuid.
    *---*/   
   printf(" Server: Looking for existing namespace entry for %s...", 
                                                        PVAL(params,cds_name));
   rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_dce, 
                                 (unsigned_char_t *)PVAL(params,cds_name),
                                 &inq_ctxt, &status);
   
   if (rpcsec2_check_stat(status, "looking for existing name space entry",
                              err_stats, &nerr_stats,  debug,0))
      printf("Bad name for object lookup?\n");
   else
   {
      /*---*
       *---* Declare a uuid_vector struct to hold UUIDS we find in the name
       *---* space
       *---*/
      struct {
         unsigned32 count;
         uuid_p_t uuid[RPCSEC2_SRV_UUID_BASE_ALLOC];
      } uuid_v = {0, {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
 
      int i;   /*---* generic pointer *---*/

      /*---* while there are UUIDS in the namespace entry *---*/
      while(status != rpc_s_no_more_members)
      {
         /*---* loop through available uuids, storing them in the vector *---*/
         printf(" Server: Allocating new uuid...");
         if (!(uuid_v.uuid[uuid_v.count]=(uuid_p_t)malloc(sizeof(uuid_t))))
	 {
            printf("couldn't get memory for uuid\n");
            rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
            exit(2);
         }
         printf(" Server: Getting next uuid...");
         rpc_ns_entry_object_inq_next(inq_ctxt, 
                                      uuid_v.uuid[uuid_v.count++],
                                      &status);
         if (status == rpc_s_entry_not_found)
         {
            printf("\n Server: No name space entry for %s\n",
                                                        PVAL(params,cds_name));
            break;
         }

	 if (status != rpc_s_no_more_members &&
	     rpcsec2_check_stat(status, "getting uuid",
				err_stats, &nerr_stats,  debug, 0))
         {
            /*---* error getting uuid, report it... *---*/
            printf("Can't get uuids to unexport!! \n");
            dump_uuid_v((uuid_vector_p_t)&uuid_v, &status);
            
            if (status)
	    {
               printf("error dumping uuids\n");
               rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
               exit(3);
	    }
            rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
            exit(2);
         }
      }  /*---* end of while() *---*/
   
      /*---*
       *---* we always push count one too far and allocate one too many uuids
       *---* so decrement the count and free the extra uuid slot.
       *---*/
      uuid_v.count--;
      free(uuid_v.uuid[uuid_v.count]);



      /*---*
       *---* we either exit the loop with rpc_s_entry_not_found or
       *---* rpc_s_no_more_members.  If we exit with rpc_s_no_more_members, 
       *---* indicating the loop exited after processing a namespace entry... 
       *---*/
      if (status != rpc_s_entry_not_found)
      {
         /*---* tell users we are done processing the old uuids *---*/
         printf(" Server: got all the uuids...\n");
         
         /*---* see if we got any uuids before printing/unexporting them *---*/
         if (uuid_v.count > 0 )
         {
            short uuid_cnt = uuid_v.count;

            /*---* we got some uuids, so print them *---*/
            dump_uuid_v((uuid_vector_p_t)&uuid_v, &status);
            if (status)
            {
                /*---* problems in printing, quit *---*/
                printf(" Server: error dumping uuids\n");
                rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
                exit(3);
            }
      
            /*---* free the context for the inquiry *---*/
            printf(" Server: freeing inquiry context...");
            rpc_ns_entry_object_inq_done(&inq_ctxt, &status);
   
            if (rpcsec2_check_stat(status, "freeing inquiry context",
                              err_stats, &nerr_stats, debug, 0))
	    {
               rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
               exit(1);
	    }

            /*---* if repl_uuid is not set... *---*/
            if (!repl_uuid && !use_null)
            {
               /*---* 
                *---* we are only supposed to have one uuid out in the name
                *---* space, so if we got more than one uuid back, we want to
                *---* unexport the others.  For the sake of efficiency, we 
                *---* will keep the last uuid we got back.  This eliminates
                *---* the need to shuffle the uuid vector to cover the hole
                *---*/

               /*---*
                *---* set the pointers in the obj_uuid vector we will pass   
                *---* since the pointers are going away when we exit this scope
                *---*/
               obj_uuid_v.count = 1;
               obj_uuid_v.uuid[0] = uuid_v.count > 1? uuid_v.uuid[uuid_v.count]
                                                    : uuid_v.uuid[0]; 
             
               /*---* if more than one uuid...  *---*/
               if (uuid_v.count > 1)
                  /*---* 
                   *---* adjust the count to account for the uuid we will 
                   *---* keep
                   *---*/
                  uuid_v.count--;
             
            } /*---* end of if (!repl_uuid && !use_null) *---*/
           
            /*---* 
             *---* if we are replacing, or not replacing, but there are too 
             *---* many uuids in the namespace entry...
             *---*/
            if (use_null || repl_uuid || uuid_cnt > 1)
            {
               /*---* unexport the uuids *---*/
               printf(" Server: unexporting the old uuids...");
               rpc_ns_binding_unexport(rpc_c_ns_syntax_dce, 
                                     (unsigned_char_t *)PVAL(params,cds_name), 
                                     (rpc_if_handle_t)NULL, 
                                     (uuid_vector_p_t)&uuid_v, &status);

               if (rpcsec2_check_stat(status, "unexporting uuids",
                              err_stats, &nerr_stats,  debug, 0))
	       {
                  rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
                  exit(1);          
	       }
            }

            /*---* free the space allocated for the existing uuids *---*/
            printf(" Server: Freeing allocated uuids...");  
            for (i=0; i< uuid_v.count; free(uuid_v.uuid[i++]));
            printf("done\n\n");
	  
         } /*---* end of if (uuid_v.count != 0) true block *---*/
         /*---* 
          *---* else if we found no uuid, and we want to export one.., 
          *---*/
         else if (uuid_v.count == 0 && !use_null)
            /*---* we have to  set repl_uuid to get a uuid created *---*/
            repl_uuid = 1;
         /*---* end of if (uuid_v.count != 0) false block *---*/

      } /*---* end of if (status != rpc_s_entry_not_found) true block *---*/
      /*---* 
       *---* else if we have no namespace entry...
       *---*/
      else if (!use_null)
         /*---* 
          *---* set repl_uuid so we will create a uuid if use_null is not set
          *---*/ 
         repl_uuid = 1;
      /*---* end of if (status != rpc_s_entry_not_found) false block *---*/

   }  /*---* end of if (rpcsec2_check_stat() )  *---*/

   /*---* if we are exporting a uuid *---*/
   if (!use_null)
   { 
      /*---* if we are replacing the uuid... *---*/
      if (repl_uuid)
      {
         /*---* create object uuids *---*/
         printf (" Server: creating uuid for obj...");
         uuid_create(&obj_uuid, &status);
 
         if (rpcsec2_check_stat(status, "creating obj uuid",
                              err_stats, &nerr_stats,  debug, 0))
	 {
            rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
            exit(1);
	 }
      }
  
      /*---* print the uuid for the user *---*/
      printf(" Server: converting to string uuid...");
      uuid_to_string(repl_uuid? &obj_uuid : obj_uuid_v.uuid[0],
                     &str_p, &status);

      if (status != uuid_s_ok)
         (void) rpcsec2_check_stat(status, "converting to string",
                              err_stats, &nerr_stats,  debug, 0);
      else
      {
         printf("done\n Server: exporting obj_uuid = %s\n\n", str_p);
      
         /*---* if we converted the uuid, free the space we put it in *---*/
         printf(" Server: freeing uuid string...");
         rpc_string_free(&str_p, &status);

         if (rpcsec2_check_stat(status, "freeing uuid string",
                              err_stats, &nerr_stats,  debug, 0))
            printf(" Server: Leaking memory :-{\n");
      }
   }
   else
      /*---* Using NULL for UUID spec *---*/
      printf(" Server: using null for uuid\n");

   /*---* create manager type uuid (we skip this) *---*/
   /*---* associate object uuids with manager type uuids (we skip this) *---*/
   /*---* 
    *---* register iface and epv with rpc - if epv_def is specified, then
    *---* pass a NULL pointer for the entry point vector arguement
    *---*/
   printf(" Server: Registering iface/epv...");
   rpc_server_register_if(rpcsec2_v1_1_s_ifspec, (uuid_t *)NULL,
     (epv_def? (rpc_mgr_epv_t)NULL: (rpc_mgr_epv_t)&rpcsec2_v1_1_epv),&status);

   if (rpcsec2_check_stat(status, "registering iface/epv",
                              err_stats, &nerr_stats,  debug, 0))
   {
      rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(1);
   }

   /*---* get binding handles allocated - use all protocol sequences *---*/
   printf(" Server: get binding handles allocated...");
   rpc_server_use_all_protseqs((unsigned32)max_calls, &status);

   if (rpcsec2_check_stat(status, "getting binding handles allocated",
                              err_stats, &nerr_stats,  debug, 0))
   {
      rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(1);
   }

   /*---* get the binding handles that were allocated *---*/
   printf(" Server: get the allocated binding handles...");
   rpc_server_inq_bindings(&bnd_vctr_p, &status);
   
   if (rpcsec2_check_stat(status, "getting the allocated binding handles",
                              err_stats, &nerr_stats,  debug,0))
   {
      rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
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
      rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(3);
   }

   printf(" Server: Principle name after the change is %s.\n",str_p);

   /*---* set the server_pname pointer to the name *---*/
   server_pname = (char *)str_p;

   client_pname = strrchr(server_pname, (int)'/');
   
   i = (client_pname - server_pname) + 1;
   
   /*---* set a pointer to the client name w/o cell stuff *---*/
   client_pname = PVAL(params, cli_princ);

   printf(" Server: Client principle name is %s\n", client_pname);

   /*---* if we are passing a UUID *---*/
   if (!use_null || repl_uuid)
   {
      /*---* 
       *---* load the uuid_vector; we use the new value if we are replacing 
       *---* the uuid, else we use the uuid we have from the namespace inquiry
       *---*/
      obj_uuid_v.count = 1;
      obj_uuid_v.uuid[0] = &obj_uuid;
   }

   /*---* register the endpoint with the endpoint mapper *---*/
   printf(" Server: registering with the endpoint mapper...");
   rpc_ep_register(rpcsec2_v1_1_s_ifspec, bnd_vctr_p, 
                   use_null? (uuid_vector_p_t)NULL : &obj_uuid_v, 
                   (unsigned_char_t *)"test rpc interface", &status);

   if (rpcsec2_check_stat(status, "registering endpoint mapper",
                              err_stats, &nerr_stats,  debug, 0))
   {
      rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(1);
   }
   
   /*---* export the bindings to the nameservice *---*/
   printf(" Server: export bindings to the namespace...");
   rpc_ns_binding_export(rpc_c_ns_syntax_dce, 
                         (unsigned_char_t *)PVAL(params,cds_name), 
                         rpcsec2_v1_1_s_ifspec,
                         bnd_vctr_p, 
                         use_null? (uuid_vector_p_t)NULL : &obj_uuid_v, 
                         &status);

   if (rpcsec2_check_stat(status, "exporting to namespace",
                              err_stats, &nerr_stats, debug, 0))
   {
      rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(1);
   }

   /*---* set the flag indicating that we exported to the namespace *---*/
   server_entry_created=1;

#if defined(RPCSEC2_SRV_REPORTING) 
   /*---* set reporting interval... *---*/
   int_from_PVAL(&rep_intvl,params,rep_int,"reporting interval",status);
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
      rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
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
      rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(1);
   }
   printf("done.\n");

   /*--- * create the reporting thread *---*/
   printf(" Server: Spawning reporting thread...");
   USE_PTHREAD(thread_tbl, thd_array, RPCSEC2_REP_THD)
   if (pthread_create(thread_tbl[RPCSEC2_REP_THD].thd_id,
                      pthread_attr_default, 
                      (pthread_startroutine_t)rep_timer, 
                      (pthread_addr_t)&rep_intvl) != 0 )
   {
      perror(" \n    Error creating reporting thread:");
      rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
      exit(1);
   }
   printf("done.\n");
#endif /*---* defined(RPCSEC2_SRV_REPORTING) *---*/

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
    *---* we do it here so that any received signals do not cause the 
    *---* the signal catcher to compete with the main thread.
    *---*/
   printf(" Server: Spawning signal catcher thread...");
   USE_PTHREAD(thread_tbl, thd_array, RPCSEC2_SIG_THD)
   if (pthread_create(thread_tbl[RPCSEC2_SIG_THD].thd_id, 
       pthread_attr_default, 
       (pthread_startroutine_t)signal_catcher, 
       (pthread_addr_t)sig_hndlr_arg) != 0 )
   {
      perror(" \n    Error creating signal catcher thread:");
      rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
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
       rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
       exit(1);
   }
   else
   {
       printf(" Server: Stopped from listening...clean up and exit.\n");
       rpcsec2_cleanup(thread_tbl[RPCSEC2_MAIN_THD].thd_id);
       printf(" Server: Exitting.\n");
       exit(0);
   }

}

void rpcsec2_nop(rpc_binding_handle_t bnd_hndle, idl_small_int *val, 
                idl_long_int *stat,
                error_status_t *fstat, 
                error_status_t *cstat)
{
  if (debug)
  {
     printf(" Server: Wakeup!!\n");
     printf(" Server: got a call; val = %d\n", *val);
  }
  /*---* check the authentication info in the binding handle *---*/
  rpcsec2_srv_check_auth(bnd_hndle, (unsigned32 *)stat);

  /*---* if the authentication routine rejects the access... *---*/
  if (*stat != rpcsec2_srv_auth_accept)
  {
     /*---* if debug set print a message *---*/
     if (debug)
        printf(" Server(nop): Authentication failed...returning.\n\n");
     return;
  }
  if (*val % 2)
  {
     if (debug)
        printf(" Server: Odd number passed, returning err_odd\n");
     *stat = rpcsec2_nop_err_odd;
  }
  else     
  {
     (*val)++;
     *stat = rpcsec2_nop_ok;
     if (debug)
        printf(" Server: returning %d\n", *val);
  }
  
  if (debug)
     printf(" Server: Returning...\n");
}



/*---*
 *---* define a macro to lock the stats update mutex, update the counter,
 *---* and unlock it
 *---*/
#define  RPCSEC2_LOCK_AND_UPD(counter) { \
                       if (debug) \
                          printf(" Server: updating stats..."); \
                       pthread_mutex_lock(&stats_update_mutex); \
                       counter++; \
                       pthread_mutex_unlock(&stats_update_mutex); \
                       if (debug) \
                          printf(" done\n"); \
				       }

/*---*
 *---* rpcsec2_cnf_str(type, struct, stat, fault_stat, comm_stat);
 *---*
 *---*  Accepts a struct containing a conformant array member, and verifies
 *---*  that the number stored in element of the array equals the following 
 *---*  checksum:
 *---*     id + (2*index) + (3*array_size)
 *---*/
void
rpcsec2_cnf_str(handle_t bnd_hndle, idl_short_int type, 
               long_conf_struct_t *conf_str_p, idl_long_int *stat, 
               error_status_t *fstat, 
               error_status_t *cstat)
{
   struct timespec now,
                   wakeup;

   /*---*
    *---* we currently have the idl and acf set up wrong, but until
    *---* we fix it, we set the comm_status here.
    *---*/
   *cstat = rpc_s_ok;
   *fstat = rpc_s_ok;
        
   if (debug)
      printf(" Server: Wakeup(in rpcsec2_cnf_str)\007\n");

   /*---* 
    *---* update total call counter.  We need to take the lock on the 
    *---* update mutex to serialize with other threads
    *---*/
   RPCSEC2_LOCK_AND_UPD(tot_calls)
  
   /*---* check the authentication info in the binding handle *---*/
   /*---* init the status before going in *---*/
   *stat=0;
   rpcsec2_srv_check_auth(bnd_hndle, (unsigned32 *)stat);

   /*---* if the authentication routine rejects the access... *---*/
   if (*stat != rpcsec2_srv_auth_accept)
   {
      /*---* if debug set print a message *---*/
      if (debug)
         printf(" Server(nop): Authentication failed...returning.\n\n");
      RPCSEC2_LOCK_AND_UPD(calls_failed)
      return;
   }
   /*---* initialize return value *---*/
   *stat = rpcsec2_cnf_ok;

   /*---* 
    *---* first check for problems that obviate processing the array: null
    *---* structure pointer, no size, or no id
    *---*
    *---* check pointer before deref; if we have a null struct pointer.. 
    *---*/
   if (!conf_str_p)
   {
      /*---* notify the user and set the status variable *---*/
      if (debug)
      {
         printf(" Server(cnf_str): NULL pointer passed for conf_str_p.\n");
         printf(" Server(cnf_str): returning rpcsec2_cnf_null_struc...\n");
      }
      *stat = rpcsec2_cnf_null_struc;
   }
   /*---* got a conf_str_p pointer, check that id is there *---*/
   else if (!conf_str_p->id)
   {
      /*---* notify the user and set the status variable *---*/
      if (debug)
      {
         printf(" Server(cnf_str): no id value in conf_str_p.\n");
         printf(" Server(cnf_str): returning rpcsec2_cnf_no_id...\n");
      }
      *stat = rpcsec2_cnf_no_id;
   }
   /*---* got a conf_str_p with an id, check that size is there *---*/
   else if (!conf_str_p->size)
   {
      /*---* notify the user and set the status variable *---*/
      if (debug)
      {
         printf(" Server(cnf_str): no size value in conf_str_p.\n");
         printf(" Server(cnf_str): returning rpcsec2_cnf_no_size...\n");
      }
      *stat = rpcsec2_cnf_no_size;
   }

   /*---* if we have no errors so far... *---*/
   if (*stat == rpcsec2_cnf_ok)
   {
      int i;    /*---* index for array access *---*/
      unsigned32 onow;
      unsigned32 wakeup_time;
      pthread_mutex_t call_delay_mutex;  /*---* used to lengthen calls *---*/

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
         *stat = rpcsec2_cnf_err_other;
      }

      /*---* get the current time... *---*/
      getclock(TIMEOFDAY, &now);
      onow=now.tv_sec;
      wakeup_time = (unsigned32)(now.tv_sec + (conf_str_p->size/3));
      wakeup.tv_sec = wakeup_time;
      wakeup.tv_nsec = 0;

      /*---*
       *---* we lock the mutex here solely because the timed wait call fails
       *---* if we don't have a lock when we call it.
       *---*/
      pthread_mutex_lock(&call_delay_mutex);
    
      if (debug)
         printf(" Server: sleeping for %d seconds..\n",
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
         printf(" Server: slept for %d seconds..\n", wakeup_time - onow);
      /*---* 
       *---* we have waited long enough, darn it! process the passed array
       *---*
       *---* if debug is set
       *---*/
      if (debug)
      {
         /*---* print what we got *---*/
         printf(" Server(cnf_str): Arguements are:\n");
         printf(" Server(cnf_str):   type  = %d,\n", type);
         printf(" Server(cnf_str):   struct long_conf_t {\n");
         printf(" Server(cnf_str):          id = %d; \n", conf_str_p->id);
         printf(" Server(cnf_str):        size = %d; \n", conf_str_p->size);
         printf(" Server(cnf_str):   },\n");
      }

      /*---* 
       *---* check the array members, comparing values against the checksum 
       *---* that we expect
       *---*/
      for (i=0; i < conf_str_p->size; i++)
      {
         /*---* 
          *---* check the value by subtracting (2*index + 3*size) from the 
          *---* value and then making sure that we end up with conf_str_p->id
          *---*/
         if (conf_str_p->array[i] - ((2*i) + (3 * (conf_str_p->size-i))) 
             != conf_str_p->id )
         {
            /*---* checksum didn't match, notify user, set return stat *---*/
            if (debug)
	    {
               printf(" Server(cnf_str): Bad checksum for array element %d\n",
                                                                            i);
               printf(" Server(cnf_str):status = rpcsec2_cnf_bad_chksum\n");
	    }
            *stat=rpcsec2_cnf_bad_chksum;
    
            /*---* if debug is set... *---*/
            if (debug)
            {
               /*---* print index, etc. *---*/               
               printf(" Server(cnf_str): index = %d\n", i);
               printf(" Server(cnf_str): conf_str_p->size = %d\n", 
                                                        conf_str_p->size);
               printf(" Server(cnf_str): conf_str_p->id = %d\n", 
                                                        conf_str_p->id);
               printf(" Server(cnf_str): conf_str_p->array[%d] = %d\n", i,
                                                        conf_str_p->array[i]);
            } /*---* end of if (debug) *---*/
         } /*--* end of if (struct..) *---*/
      } /*---* end of for (i=0...) *---*/
   } /*---* end of if (*stat...) *---*/
   if (*stat == rpcsec2_cnf_ok)
      RPCSEC2_LOCK_AND_UPD(calls_passed)
   else
      RPCSEC2_LOCK_AND_UPD(calls_failed)
}

void
dump_uuid_v(uuid_vector_p_t uuid_v, unsigned32 *status)
{
   int i;
   unsigned_char_t *str_p;
   unsigned32 loc_stat;

   printf("\tUUID vector at 0x%o contains %d UUIDS:\n",&uuid_v, uuid_v->count);
   *status = 0;

   /*---* print the list of the uuids we got *---*/
   for (i=0; i< uuid_v->count; i++)
   {
      /*---* convert it *---*/
      uuid_to_string(uuid_v->uuid[i], &str_p, &loc_stat);
      if (loc_stat != uuid_s_ok)
      {
         printf("error converting uuid\n");
         *status = 1;
      }
      
      printf("             %s\n",str_p);
     
      /*---* free the string *---*/
      rpc_string_free(&str_p, &loc_stat);
   
      if (loc_stat != rpc_s_ok)
      { 
         printf("error freeing uuid string\n");
      } 
   }
}


/*--------------------------------------------*
 *---* rpcsec2_srv_check_auth(binding_handle,  status)
 *---*
 *---*   check to see if the client requesting the service is 
 *---*   authorized to our satisfaction and using the authorization level we
 *---*   want.
 *--------------------------------------------*/
void
rpcsec2_srv_check_auth(rpc_binding_handle_t bnd_h, unsigned32 *status)
{
   rpc_authz_handle_t cli_privs;   /*---* cli name or a problem!      *---*/
   unsigned_char_p_t  srv_princ;   /*---* server princ spec'ed by cli *---*/
   unsigned32  protcn_lev,         /*---* level of authenticated RPC  *---*/
               authn_svc,           
               authz_svc,
               loc_status;
   char        *cp;                /*---* utility pointer             *---*/

   /*---* rip the auth info out of the binding handle *---*/
   if (debug)
      printf(" Server: getting client auth* info...");
   rpc_binding_inq_auth_client(bnd_h, &cli_privs, &srv_princ, &protcn_lev, 
                               &authn_svc, &authz_svc, &loc_status);

   /*---* if in debug mode, print the auth info from the client handle *---*/
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
      *status = rpcsec2_srv_auth_cant_get_info;

   /*---* 
    *---* check the server principle name. if names don't match, we have
    *---* a problem.  Note that we don't check this in the unauthenticated
    *---* case, since the server name is not passed in the binding.
    *---*/
   else if (protcn_lev != rpc_c_protect_level_none && 
                                       strcmp((char *)srv_princ, server_pname))
   {
      *status = rpcsec2_srv_auth_bad_srv;        /*---* set bad status *---*/

      /*---*  print the error (if debug on) *---*/
      if (debug)
         printf(" Server: Bad server principle name(%s)\n",srv_princ);
      
   }
   /*---* check the authorization service, if not rpc_c_authz_name *---*/
   else if (authz_svc != rpc_c_authz_name)
   {
      *status = rpcsec2_srv_auth_unsup_authz;  /*---* set bad status *---*/
   
      /*---* print the error (if debug on) *---*/
      if (debug)
         printf(" Server: Unsupported authorization service.\n");

   }
   /*---* check authentication service, if not rpc_c_authn_dce_secret... *---*/
   else if (authz_svc != rpc_c_authn_dce_secret)
   {
      *status = rpcsec2_srv_auth_unsup_authn;  /*---* set bad status *---*/
   
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
      *status = rpcsec2_srv_auth_unsup_lvl;  /*---* set bad status *---*/
   
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
          *status = rpcsec2_srv_auth_bad_cli;  /*---* set bad status *---*/
   
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
      *status = rpcsec2_srv_auth_accept;  /*--* set good status *---*/
       
      /*---* print a message (if debug on) *---*/
      if (debug)
         printf(" Server: Authenticated client.\n");
   }

}

void rpcsec2_cleanup(pthread_t *call_thd)
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
      char *srv_name = PVAL(params,cds_name);
			    
      /*---* report the time and remove the server entry *---*/
      (void)time(&tbuf);
      CTIME_NO_NL(t_ptr, &tbuf);
      printf(" Server: removing the server entry %s at %s...",srv_name, t_ptr);
      rpc_ns_mgmt_entry_delete(rpc_c_ns_syntax_default, 
                 (unsigned_char_t *)srv_name, &l_stat);
      rpcsec2_check_stat(l_stat, "Removing server namespace entry",
                                           err_stats, &nerr_stats, debug, 0);
   }

   /*---* report start time for thread cancels *---*/
   (void)time(&tbuf);
   printf(" Server: Starting thread cancels at %s\n", ctime(&tbuf));

   /*---* cancel all of the threads *---*/
   for (i=0; i < RPCSEC2_SRV_NTHDS; i++)
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

   printf(" Server: Starting thread cancels...\n");

}

void
rpcsec2_rep_gen(int call_ctxt)
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
      printf("Hi from rpcsec2_rep_gen: %s %d...\n", 
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
