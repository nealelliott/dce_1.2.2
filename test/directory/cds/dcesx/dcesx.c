/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */

/*
 * HISTORY
 * $Log: dcesx.c,v $
 * Revision 1.1.10.2  1996/02/18  00:24:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:25  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:09:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:44:17  root]
 * 
 * Revision 1.1.8.3  1993/08/20  20:49:17  pwang
 * 	Removed the usages of rpc_server_use_protseq and use
 * 	rpc_server_use_all_protseqs only. The environment
 * 	variable RPC_SUPPORTED_PROTSEQS could be used to test
 * 	a particular protseq anyway. [OT#7383]
 * 	[1993/08/20  19:47:14  pwang]
 * 
 * Revision 1.1.8.2  1993/06/17  21:05:24  sommerfeld
 * 	Flush incorrect duplicate prototype for gethostname (should be in unistd.h?
 * 	It is on HP-UX)
 * 	added static before listener/skulker code to quiet compiler warnings
 * 	[1993/05/11  17:17:58  smythe]
 * 
 * Revision 1.1.4.3  1993/03/29  20:32:25  jd
 * 	Fixed get_options returning char. It returns int now.
 * 	[1993/03/08  14:28:48  jd]
 * 
 * 	Fix up usage argument pasing conflict.
 * 	[1993/03/08  13:34:54  jd]
 * 
 * Revision 1.1.4.2  1993/01/12  18:29:26  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/07  01:13:16  bbelch]
 * 
 * Revision 1.1.2.3  1992/06/26  20:10:07  mfox
 * 	Replaced varargs stuff with stdargs.h macros.
 * 	[1992/06/26  20:09:17  mfox]
 * 
 * Revision 1.1.2.2  1992/06/08  19:58:04  weisman
 * 		Original submission.
 * 	[1992/06/08  19:24:18  weisman]
 * 
 * $EndLog$
 */
/*
** COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1992. ALL RIGHTS RESERVED.
**
** THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
** ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
** INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
** COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
** OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
** TRANSFERRED.
**
** THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
** AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
** CORPORATION.
**
** DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
** SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
**
**  NAME
**
**      dcesx.c
**
**  FACILITY:
**
**      DCE System Exerciser
**
**  ABSTRACT:
**
**      TBS
**
**  MODIFICATION HISTORY:
**
******************************************************************************
**
**  25-oct-91 sudama    fix command options; add pass count;
**                      fix bug in dcesx_client to terminate thread;
**                      add dcesx_usage() and remove junk
**  24-oct-91 sudama    change varargs.h to stdarg.h and move pthread.h
**                      to top of includes
**  01-oct-91 ouellette Add pthread_detach.
**  25-sep-91 ouellette Return errors from dcesx_server.
**  16-sep-91 ouellette Add delay command line arg.
**  12-sep-91 ouellette ep_unregister after unexport.
**  27-aug-91 ouellette Cleanup and fill in routines.
**  27-aug-91 sudama    original creation
**
******************************************************************************
**
**  07-feb-92 mbs "char" -=> "unsigned char"
**                added some #includes
**                added "-i num" interfaces option
**                make listener single-threaded
**  12-feb-92 mbs added i/f registration mutex
**                display "pass #"
**                embellish options/usage display
**  13-feb-92 mbs added skulking and delay when import binding fails
**  14-feb-92 mbs added some error checking
**                added condition variable for skulks
**  17-feb-92 mbs pthread_cond_timedwait() uses absolute time
**  19-feb-92 mbs allow multiple server/client threads
**                incorporate a "graceful-shutdown"
**  20-feb-92 mbs added retries for skulks
**  21-feb-92 mbs added i/f registry active array
**                added sanity checking of command line inputs
**  06-mar-92 mbs added dcesx_listener() and dcesx_skulker() threads
**  09-mar-92 mbs remove if_mutex locks during server register and
**                unregister except when requested with the 'L'
**                (LOCK) command line option
**  10-mar-92 mbs allow -p0 to run forever
**  17-mar-92 mbs added do_login() for security
**  19-mar-92 mbs bugfix: broke 'passes' with -p0 addition
**  24-mar-92 mbs dress-up logging output
**  25-mar-92 mbs add more info to logging output
**                add skulking when adding/removing group members
******************************************************************************
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include <dce/dce_error.h>
#include <dce/cdsclerk.h>

#ifdef DCE_SEC
#include <dce/sec_login.h>
#endif

#include "dcesx0.h"
#include "dcesx1.h"
#include "dcesx2.h"
#include "dcesx3.h"
#include "dcesx4.h"
#include "dcesx5.h"
#include "dcesx6.h"
#include "dcesx7.h"
#include "dcesx8.h"
#include "dcesx9.h"

/*
 * All external routine declarations have been flushed, as they should only
 * appear in system header files..
 */

/*
 * Internal routine declarations.
 */
#ifdef DCE_SEC
static void dcesx_do_login();
#endif
static void dcesx_server_register();
static void dcesx_server_unregister();
static void dcesx_client();
static int get_options();
static void dcesx_usage();
static void dcesx_help();
static void dcesx_listener();
static void dcesx_skulker();
static void dcesx_skulk_root();
static void dcesx_skulk_request();
static void dcesx_add_group();
static void dcesx_remove_group();

/*
 * Error handling related variables and macros.
 */
enum
{
    dcesx_s_ok = 0,

    dcesx_s_usage,
    dcesx_s_system,
    dcesx_s_ifrdy,

    dcesx_s_cds_skulk_dir,

    dcesx_s_rpc_binding_free,
    dcesx_s_rpc_binding_to_string_binding,
    dcesx_s_rpc_binding_vector_free,
    dcesx_s_rpc_ep_register,
    dcesx_s_rpc_ep_unregister,
    dcesx_s_rpc_if_inq_id,
    dcesx_s_rpc_mgmt_stop_server_listening,
    dcesx_s_rpc_ns_binding_export,
    dcesx_s_rpc_ns_binding_unexport,
    dcesx_s_rpc_ns_binding_import_begin,
    dcesx_s_rpc_ns_binding_import_next,
    dcesx_s_rpc_ns_binding_import_done,
    dcesx_s_rpc_ns_group_mbr_add,
    dcesx_s_rpc_ns_group_mbr_remove,
    dcesx_s_rpc_ns_mgmt_set_exp_age,
    dcesx_s_rpc_ns_profile_elt_add,
    dcesx_s_rpc_ns_profile_elt_remove,
    dcesx_s_rpc_server_inq_bindings,
    dcesx_s_rpc_server_listen,
    dcesx_s_rpc_server_register,
    dcesx_s_rpc_server_register_if,
    dcesx_s_rpc_server_unregister_if,
    dcesx_s_rpc_string_free,
    dcesx_s_rpc_use_all_protseqs,
    dcesx_s_rpc_use_protseq,

    dcesx_s_pthread_create_failed,
    dcesx_s_pthread_detach_failed
};

#define REPORT_DCE_ERROR(status) \
{ \
    dce_error_inq_text (status, error_text, &error_status); \
\
    if (error_status != -1) \
    { \
        dcesx_log ( "%s\n", error_text); \
    } \
    else \
    { \
        dcesx_log ("*** no error text translation 0x%x\n", status); \
    } \
}

unsigned char   error_text[dce_c_error_string_len];
int             error_status;

/*
 * Use a single manager routine for all the interfaces in the
 * interface table.
 */
dcesx_if0_v1_0_epv_t    dcesx_g_mgr_epv =
{
    dcesx_server
};

/*
 * This is the interface table. Each interface will use a single
 * manager routine but will have its own .idl and .acf files.
 */
#define dcesx_c_if_max  10

rpc_if_handle_t     *dcesx_g_if_table[dcesx_c_if_max] =
{
    &dcesx_if0_v1_0_s_ifspec,
    &dcesx_if1_v1_0_s_ifspec,
    &dcesx_if2_v1_0_s_ifspec,
    &dcesx_if3_v1_0_s_ifspec,
    &dcesx_if4_v1_0_s_ifspec,
    &dcesx_if5_v1_0_s_ifspec,
    &dcesx_if6_v1_0_s_ifspec,
    &dcesx_if7_v1_0_s_ifspec,
    &dcesx_if8_v1_0_s_ifspec,
    &dcesx_if9_v1_0_s_ifspec
};

/*
 * The client EPV table. This table should parallel the interface
 * table.
 */
dcesx_if0_v1_0_epv_t    *dcesx_g_c_epv_table[dcesx_c_if_max] =
{
    (dcesx_if0_v1_0_epv_t *)&dcesx_if0_v1_0_c_epv,
    (dcesx_if0_v1_0_epv_t *)&dcesx_if1_v1_0_c_epv,
    (dcesx_if0_v1_0_epv_t *)&dcesx_if2_v1_0_c_epv,
    (dcesx_if0_v1_0_epv_t *)&dcesx_if3_v1_0_c_epv,
    (dcesx_if0_v1_0_epv_t *)&dcesx_if4_v1_0_c_epv,
    (dcesx_if0_v1_0_epv_t *)&dcesx_if5_v1_0_c_epv,
    (dcesx_if0_v1_0_epv_t *)&dcesx_if6_v1_0_c_epv,
    (dcesx_if0_v1_0_epv_t *)&dcesx_if7_v1_0_c_epv,
    (dcesx_if0_v1_0_epv_t *)&dcesx_if8_v1_0_c_epv,
    (dcesx_if0_v1_0_epv_t *)&dcesx_if9_v1_0_c_epv
};

/*
 * Definitions and variables for the directory entries.
 */
#define dcesx_c_entry_name              0
#define dcesx_c_group_name              1
#define dcesx_c_profile_name            2
#define dcesx_c_directory_names_max     3

char *dcesx_g_directory_names[dcesx_c_directory_names_max] =
{
    "",
    "/.:/dcesx_group",
    "/.:/dcesx_profile"
};

#define dcesx_c_entry_name_max          4096
#define dcesx_c_hostname_max            31

char    entry_name[dcesx_c_entry_name_max];
char    group_name[dcesx_c_entry_name_max];
char    profile_name[dcesx_c_entry_name_max];
char    hostname[dcesx_c_hostname_max];

/*
 * Miscellaneous variable definitions.
 */

int                 multi_client = false;
int                 single       = false;
int                 re_random    = false;
int                 debug        = false;
int                 do_lock      = false;
int                 do_skulks    = false;
int                 quiet        = false;
int                 verbose      = false;

int                 delay_time = 10;
int                 if_num     = dcesx_c_if_max;
int                 max_calls  = dcesx_c_if_max;
int                 retry_max  = 1;

int                 listener_running = false;

int                 passes = 1;
int                 pass_count[dcesx_c_if_max] = {0};

char                *options = "dDhHKLMqQRSvVi:m:p:r:t:";
char                *optarg;
int                 option_index = 1;
boolean32           option_error;

#ifdef DCE_SEC
int                 Authentication = 0;
#endif

/*
 ********************************************************************
 * Mutexes, condition variables and data items that they protect
 *
 *  Note: the rest of our 'global' booleans are declared volatile to
 *       ensure that the compiler doesn't try to optimize us into
 *       trouble with flags and other important items
 ********************************************************************
 */

/*
 * dcesx_group_mutex
 *
 * This mutex protects the dcesx_groups counter.
 *
 * Anyone modifying dcesx_groups must acquire this mutex.
 */

pthread_mutex_t         dcesx_group_mutex;
volatile int            dcesx_groups = 0;

/*
 * dcesx_ifrdy_mutex
 *
 * This mutex protects the dcesx_ifrdy_flag[] array.
 *
 * Anyone modifying any flag in the array must acquire this mutex.
 */

pthread_mutex_t         dcesx_ifrdy_mutex;
volatile boolean32      dcesx_ifrdy_flag[dcesx_c_if_max] = {0};

/*
 * dcesx_ifreg_mutex
 *
 * This mutex protects the dcesx_server_register() and
 * dcesx_server_unregister() operations.  It is used to ensure that
 * the registration and de-registration operations are atomic.
 *
 * Acquire this mutex during the entire registration and
 * de-registration processes.
 */

pthread_mutex_t         dcesx_ifreg_mutex;

/*
 * dcesx_skulk_rqst_mutex
 *
 * This mutex protects the dcesx_skulk_rqst_cond condition variable
 * and the dcesx_skulker_waiting boolean.
 */

pthread_mutex_t         dcesx_skulk_rqst_mutex;
pthread_cond_t          dcesx_skulk_rqst_cond;

volatile boolean32      dcesx_skulker_waiting = false;

volatile boolean32      dcesx_skulker_needed = true;
volatile int            dcesx_skulk_id = 0;

/*
 * if_skulk[]
 *
 * This array structure contains an entry for each interface that we
 * support.  Each interface may have a skulk request pending.  The
 * if_skulk_t structure contains all the elements of control required
 * for the background skulker to handle the requests.
 */

struct if_skulk_t
{
    volatile boolean32  rqst_flag;
    volatile boolean32  done_flag;
    volatile int        skulk_id;
    pthread_mutex_t     rqst_mutex;
    pthread_cond_t      rqst_cond;
} if_skulk[dcesx_c_if_max];


/*
**++
**
**  ROUTINE NAME:       dcesx_log
**
**  SCOPE:              INTERNAL - declared locally.
**
**  DESCRIPTION:
**
**  This routine will print a log entry in the format:
**      >>> host thread-id:pid --- date --- <log entry> 
**
**  for example:
**      >>> ko.nac.dec.com 1234:5123 --- Tue Sep  3 12:21:58 EDT 1991 --- protocols registered
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

#ifdef __STDC__
static int dcesx_log (char *format, ...)
#else
static int dcesx_log (format)
char *format;
#endif

{
    va_list	    arg_ptr;
    char            buff[200];
    char            *s = buff;


    sprintf (s, ">>> ");

    s = &buff[strlen (buff)];

    if (verbose)
    {
        /*
         * Get the host name.
         */
        sprintf (s, "%s ", hostname);
        s = &buff[strlen (buff)];
        
        /*
         * Get the thread id.
         */
        {
            pthread_t self;
            
            self = pthread_self ();
            sprintf (s, "%08x:", self);
            s = &buff[strlen (buff)];
        }
        
        /*
         * Get the process id.
         */
        {
            pid_t   pid;

            
            pid = getpid();
            sprintf (s, "%08x", pid);
            s = &buff[strlen (buff)];
        }
        
        /*
         * Get the time
         */
        {
            time_t  cur_time;
            char    *cur_ctime;


            cur_time = time (NULL);
            cur_ctime = ctime (&cur_time);
            cur_ctime [strlen (cur_ctime) - 1] = '\0';
            sprintf (s, " - %s - ", cur_ctime);
            s = &buff[strlen (buff)];
        }
    }

    va_start (arg_ptr, format);
    vsprintf (s, format, arg_ptr);
    va_end (arg_ptr);

    fputs (buff, stderr);
}

/*
**++
**
**  ROUTINE NAME:       main
**
**  SCOPE:              INTERNAL - declared locally.
**
**  DESCRIPTION:
**
**  Entry point for DCE exerciser process.
**
**  INPUTS:     
**
**      argc            Command line arg count
**      argv            Command line arg vector
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

main (argc, argv)

int         argc;
char        *argv[];

{
    unsigned32  status;
    int         c;
    int         i, if_max;
    pthread_t   listener_thread_id;
    unsigned32  listener_thread_status;
    pthread_t   skulker_thread_id;
    unsigned32  skulker_thread_status;

    /*
     * see if any options were specified on the command line
     */
    while ((c = get_options (argc, argv, options)) != EOF)
    {
        switch (c)
        {
            case '\0': break;
            case 'K' : do_skulks    = true; break;
            case 'L' : do_lock      = true; break;
            case 'M' : multi_client = true; break;
            case 'R' : re_random    = true; break;
            case 'S' : single       = true; break;

            case 'd': case 'D' : debug   = true; break;
            case 'q': case 'Q' : quiet   = true; break;
            case 'v': case 'V' : verbose = true; break;

            case 'i' : if_num     = atoi (optarg); break;
            case 'm' : max_calls  = atoi (optarg); break;
            case 'p' : passes     = atoi (optarg); break;
            case 'r' : retry_max  = atoi (optarg); break;
            case 't' : delay_time = atoi (optarg); break;

            case 'H' :
            case 'h' : dcesx_help(); exit (dcesx_s_usage);
            default  : dcesx_usage(); exit (dcesx_s_usage);
        }
    }

    /*
     * sanity check our inputs
     */
    if (if_num > dcesx_c_if_max)
    {
        dcesx_log ("too many interfaces specified ... max is %d\n",
                   dcesx_c_if_max);
        exit (dcesx_s_usage);
    }

#ifdef DCE_SEC
    /*
     * set up a secure login
     */
    dcesx_do_login ();
#endif

    /*
     * initialize/create our exercisor mutex(es)
     */
    pthread_mutex_init (&dcesx_ifreg_mutex, pthread_mutexattr_default);
    pthread_mutex_init (&dcesx_ifrdy_mutex, pthread_mutexattr_default);
    pthread_mutex_init (&dcesx_group_mutex, pthread_mutexattr_default);

    pthread_mutex_init (&dcesx_skulk_rqst_mutex, pthread_mutexattr_default);
    pthread_cond_init  (&dcesx_skulk_rqst_cond,  pthread_condattr_default);

    /*
     * initialize our interface skulk request structure
     */
    for (i=0; i<dcesx_c_if_max; i++)
    {
        if_skulk[i].rqst_flag = false;
        if_skulk[i].done_flag = false;
        if_skulk[i].skulk_id  = 0;
        pthread_mutex_init (&if_skulk[i].rqst_mutex, pthread_mutexattr_default);
        pthread_cond_init  (&if_skulk[i].rqst_cond,  pthread_condattr_default);
    }

    /*
     * get the host name and initialize the entry name in the table
     */
    gethostname (hostname, dcesx_c_hostname_max);
    sprintf (entry_name, "/.:/dcesx_entry_%s", hostname);
    dcesx_g_directory_names[dcesx_c_entry_name] = entry_name;

    /*
     * construct the profile and group entry names if the test is
     * not supposed to interact with other hosts
     */
    if (single)
    {
        sprintf (group_name, "/.:/dcesx_group_%s", hostname);
        dcesx_g_directory_names[dcesx_c_group_name] = group_name;
        sprintf (profile_name, "/.:/dcesx_profile_%s", hostname);
        dcesx_g_directory_names[dcesx_c_profile_name] = profile_name;
    }

    /*
     * set up the runtime to use all supported protocols
     */
    rpc_server_use_all_protseqs (rpc_c_protseq_max_reqs_default, &status);
    if (status != rpc_s_ok)
    {
        dcesx_log ("*** Can't set up server protocols\n");
        exit (dcesx_s_rpc_use_all_protseqs);
    }

    if (!quiet) dcesx_log ("protocols registered\n");

    /*
     * start the listener thread
     */
    if ((pthread_create (&listener_thread_id,
                         pthread_attr_default,
                         (pthread_startroutine_t) dcesx_listener,
                         (pthread_addr_t) 0)) == -1)
    {
        dcesx_log ("*** Can't start listener thread\n");
        exit (dcesx_s_pthread_create_failed);
    }

    /*
     * start the skulker thread
     */
    if ((pthread_create (&skulker_thread_id,
                         pthread_attr_default,
                         (pthread_startroutine_t) dcesx_skulker,
                         (pthread_addr_t) 0)) == -1)
    {
        dcesx_log ("*** Can't start skulker thread\n");
        exit (dcesx_s_pthread_create_failed);
    }

    /*
     * skulk the root directory so that we are ensured of starting
     * with a clean slate
     */
    dcesx_skulk_root();

    /*
     * none of our server i/f's are ready
     */
    for (i=0; i<dcesx_c_if_max; i++)
    {
        dcesx_ifrdy_flag[i] = false;
    }

    /*
     * register our interfaces
     *
     * note: if "multi_client" then we register all of the
     *       interfaces at once, otherwise, they are registered in a
     *       round-robin fashion
     */
    if_max = multi_client ? if_num : 1;

    for (i=0; i<if_max; i++)
    {
        pass_count[i] = 0;
        dcesx_server_register (i, &status);
        if (status != dcesx_s_ok)
        {
            dcesx_log ( "*** Unable to register interface #%d\n", i);
            REPORT_DCE_ERROR (status);
            exit (dcesx_s_rpc_server_register);
        }
    }

    /*
     * join to the listener thread
     */
    pthread_join (listener_thread_id,
                  (pthread_addr_t *) &listener_thread_status);
    if (listener_thread_status != dcesx_s_ok)
    {
        dcesx_log ( "*** listener returned bad status\n");
        exit (listener_thread_status);
    }

    /*
     * shutdown the skulker and join to it
     */
    dcesx_skulker_needed = false;
    pthread_join (skulker_thread_id,
                  (pthread_addr_t *) &skulker_thread_status);
    if (skulker_thread_status != dcesx_s_ok)
    {
        dcesx_log ( "*** skulker returned bad status\n");
        exit (skulker_thread_status);
    }

    /*
     * exit without error
     */
    exit (dcesx_s_ok);
}

/*
**++
**
**  ROUTINE NAME:       dcesx_server_register
**
**  SCOPE:              INTERNAL - declared locally.
**
**  DESCRIPTION:
**
**  This routine will register the interface specified with the RPC
**  runtime, register all server bindings in the RPC runtime with the
**  endpoint mapper, export all server bindings to a directory
**  service server entry and add the server entry to a well known
**  profile and group. A client thread is then created to make and
**  RPC on the interface just registered.
**
**  INPUTS:     
**
**      if_index        index of the interface to register/export
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      status
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

static void dcesx_server_register (if_index, status)

int             if_index;
error_status_t  *status;

{
    rpc_binding_vector_p_t  binding_vector;
    rpc_if_id_t             if_id;
    int                     i;
    unsigned_char_t         annotation[16];
    static boolean32        expiration_set = false;

    /*
     * gain access to the exercisor i/f registration mutex
     */
    if (do_lock)
    {
        pthread_mutex_lock (&dcesx_ifreg_mutex);
    }

    /*
     * register the specified interface
     */
    rpc_server_register_if (
        *dcesx_g_if_table[if_index],
        NULL,
        (rpc_mgr_epv_t) &dcesx_g_mgr_epv, 
        status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't register server interface\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_server_register_if);
    }

    if (!quiet)
    {
        dcesx_log ("%02d: server interface registered\n", if_index);
    }
    
    /*
     * get back the bindings actually supported for this server
     */
    rpc_server_inq_bindings (&binding_vector, status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't inquire server bindings\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_server_inq_bindings);
    }

    if (!quiet)
    {
        for (i = 0; i < binding_vector->count; i++)
        {
            unsigned_char_p_t string_binding;
            
            rpc_binding_to_string_binding
                (binding_vector->binding_h[i], &string_binding, status);

            if (*status != rpc_s_ok)
            {
                dcesx_log ("%02d: *** can't get string binding\n", if_index);
                exit (dcesx_s_rpc_binding_to_string_binding);
            }
            
            dcesx_log ("%02d: got binding: %s\n", if_index, string_binding);

            rpc_string_free (&string_binding, status);

            if (*status != rpc_s_ok)
            {
                dcesx_log ("%02d: *** rpc_string_free() failed\n", if_index);
                REPORT_DCE_ERROR (*status);
                exit (dcesx_s_rpc_string_free);
            }
        }
    }

    /*
     * register the dynamic endpoints that were created
     */
    sprintf ((char *) annotation, "dcesx-if#%d", if_index);

    rpc_ep_register (
        *dcesx_g_if_table[if_index], 
        binding_vector, 
        NULL, 
        annotation,
        status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't register server endpoints\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_ep_register);
    }

    if (!quiet)
    {
        dcesx_log ("%02d: server endpoints registered with endpoint mapper\n",
                   if_index);
    }

    /*
     * export the server bindings to the directory service
     */
    rpc_ns_binding_export (
        rpc_c_ns_syntax_default,
        (unsigned_char_t *) dcesx_g_directory_names[dcesx_c_entry_name],
        *dcesx_g_if_table[if_index],
        binding_vector,
        NULL,
        status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't export server binding information\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_ns_binding_export);
    }

    if (!quiet)
    {
        dcesx_log ("%02d: exported server binding information to entry %s\n", 
                   if_index, dcesx_g_directory_names[dcesx_c_entry_name]);
    }

    rpc_binding_vector_free (&binding_vector, status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't free binding vector\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_binding_vector_free);
    }
    
    /*
     * add this server entry to the dcesx group
     */
    dcesx_add_group (if_index);

    /*
     * add this server entry to the dcesx profile
     */
    rpc_if_inq_id (*dcesx_g_if_table[if_index], &if_id, status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't inquire interface id\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_if_inq_id);
    }

    rpc_ns_profile_elt_add (
        rpc_c_ns_syntax_default,
        (unsigned_char_t *) dcesx_g_directory_names[dcesx_c_profile_name],
        &if_id,
        rpc_c_ns_syntax_default,
        (unsigned_char_t *) dcesx_g_directory_names[dcesx_c_entry_name],
        0,
        annotation,
        status);        

    if (*status != rpc_s_ok)
    {
        /*
         * Special case the informational status code returned when
         * a profile element has been overwritten.
         */
        if (*status != rpc_s_profile_element_replaced)
        {
            dcesx_log ("%02d: *** Can't add server to profile\n", if_index);
            REPORT_DCE_ERROR (*status);
            exit (dcesx_s_rpc_ns_profile_elt_add);
        }

        *status = rpc_s_ok;
    }

    if (!quiet)
    {
        dcesx_log ("%02d: added server entry %s to profile %s\n", 
                   if_index,
                   dcesx_g_directory_names[dcesx_c_entry_name],
                   dcesx_g_directory_names[dcesx_c_profile_name]);
    }

    /*
     * now that the i/f has been registered and exported we can mark
     * it as being "ready"
     */
    pthread_mutex_lock (&dcesx_ifrdy_mutex);

    dcesx_ifrdy_flag[if_index] = true;

    pthread_mutex_unlock (&dcesx_ifrdy_mutex);

    /*
     * skulk the root and then allow others access to the execisor i/f
     * registration mutex
     */
    dcesx_skulk_request (if_index);
    if (do_lock)
    {
        pthread_mutex_unlock (&dcesx_ifreg_mutex);
    }

    /*
     * create a client of this server
     */
    {
        pthread_t       client_thread_id;

        if ((pthread_create (
            &client_thread_id,
            pthread_attr_default,
            (pthread_startroutine_t) dcesx_client,
            (pthread_addr_t) if_index)) == -1)
        {
            dcesx_log ("%02d: *** Can't create client thread\n", if_index);
            *status = dcesx_s_pthread_create_failed;
        }
        else
        {
            char        thread_id[9];

            sprintf (thread_id, "%08x", client_thread_id);

            if (!quiet)
            {
                dcesx_log ("%02d: created client thread %s to call if #%d\n", 
                           if_index, thread_id, if_index);
            }

            if ((pthread_detach (&client_thread_id)) == -1)
            {
                dcesx_log ("%02d: *** Can't detach client thread\n", if_index);
                *status = dcesx_s_pthread_detach_failed;
            }
        }
    }

    /*
     * set the expiration age to 0 to force nsi updates on every
     * 'next' operation, note that we only do this once
     */
    if (!expiration_set)
    {
        error_status_t exp_status;

        rpc_ns_mgmt_set_exp_age(0, &exp_status);
        if (exp_status != rpc_s_ok)
        {
            dcesx_log ( "%02d: *** Can't set expiration age to 0\n", if_index);
            REPORT_DCE_ERROR (exp_status);
            exit (dcesx_s_rpc_ns_mgmt_set_exp_age);
        }
        expiration_set = true;

        if (!quiet) dcesx_log ("%02d: *** expiration age set to 0\n", if_index);
    }
}

/*
**++
**
**  ROUTINE NAME:       dcesx_server_unregister
**
**  SCOPE:              INTERNAL - declared locally.
**
**  DESCRIPTION:
**
**  This routine will unregister the interface specified with the RPC
**  runtime, unregister all server bindings in the RPC runtime from the
**  endpoint mapper, unexport all server bindings from the directory
**  service server entry and remove the server entry from the well known
**  profile and group.
**
**  INPUTS:     
**
**      if_index        index of the interface to unregister/unexport
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      status
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

static void dcesx_server_unregister (if_index, status)

int             if_index;
error_status_t  *status;

{
    rpc_binding_vector_p_t  binding_vector;
    rpc_if_id_t             if_id;

    /*
     * gain access to the exercisor i/f registration mutex
     */
    if (do_lock)
    {
        pthread_mutex_lock (&dcesx_ifreg_mutex);
    }

    /*
     * unregister the specified interface
     */
    rpc_server_unregister_if (*dcesx_g_if_table[if_index], NULL, status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't unregister server interface\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_server_unregister_if);
    }

    if (!quiet)
    {
        dcesx_log ("%02d: server interface unregistered\n", if_index);
    }

    /*
     * unexport the server bindings from the directory service
     */
    rpc_ns_binding_unexport (
        rpc_c_ns_syntax_default,
        (unsigned_char_t *) dcesx_g_directory_names[dcesx_c_entry_name],
        *dcesx_g_if_table[if_index],
        NULL,
        status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't unexport server binding information\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_ns_binding_unexport);
    }

    if (!quiet)
    {
        dcesx_log ("%02d: unexported server binding information from entry %s\n", 
                   if_index, dcesx_g_directory_names[dcesx_c_entry_name]);
    }

    /*
     * remove this server entry from the dcesx group
     */
    dcesx_remove_group (if_index);

    /*
     * remove this server entry from the dcesx profile
     */
    rpc_if_inq_id (*dcesx_g_if_table[if_index], &if_id, status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't inquire interface id\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_if_inq_id);
    }

    rpc_ns_profile_elt_remove (
        rpc_c_ns_syntax_default,
        (unsigned_char_t *) dcesx_g_directory_names[dcesx_c_profile_name],
        &if_id,
        rpc_c_ns_syntax_default,
        (unsigned_char_t *) dcesx_g_directory_names[dcesx_c_entry_name],
        status);        

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't remove server from profile\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_ns_profile_elt_remove);
    }

    if (!quiet)
    {
        dcesx_log ("%02d: removed server entry %s from profile %s\n",
                   if_index,
                   dcesx_g_directory_names[dcesx_c_entry_name],
                   dcesx_g_directory_names[dcesx_c_profile_name]);
    }

    /*
     * get back the bindings actually supported for this server
     */
    rpc_server_inq_bindings (&binding_vector, status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't inquire server bindings\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_server_inq_bindings);
    }

    /*
     * unregister the dynamic endpoints that were created
     */
    rpc_ep_unregister (
        *dcesx_g_if_table[if_index], 
        binding_vector, 
        NULL, 
        status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't unregister server endpoints\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_ep_unregister);
    }

    if (!quiet)
    {
        dcesx_log ("%02d: server endpoints unregistered from endpoint mapper\n",
                   if_index);
    }

    rpc_binding_vector_free (&binding_vector, status);

    if (*status != rpc_s_ok)
    {
        dcesx_log ("%02d: *** Can't free binding vector\n", if_index);
        REPORT_DCE_ERROR (*status);
        exit (dcesx_s_rpc_binding_vector_free);
    }

    /*
     * skulk the root and then allow others access to the execisor
     * i/f registration mutex
     */
    dcesx_skulk_request (if_index);
    if (do_lock)
    {
        pthread_mutex_unlock (&dcesx_ifreg_mutex);
    }
}

/*
**++
**
**  ROUTINE NAME:       dcesx_server
**
**  SCOPE:              INTERNAL - declared locally.
**
**  DESCRIPTION:
**
**  This routine will be the manager routine for all RPCs made over
**  any of the dcesx interfaces. It will unregister the interface the
**  RPC was made over and register the next one, wrapping around if
**  necessary. 
**
**  INPUTS:     
**
**      binding_handle The client binding handle.
**      if_index        index of the interface to unregister/unexport
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

void dcesx_server (binding_handle, if_index, status)

handle_t        binding_handle;
unsigned32      if_index;
error_status_t  *status;

{
    unsigned_char_p_t string_binding;
    int               next_if_index;
    int               done;
    int               i;
    int               ifrdy;

    /*
     * See if this interface is "ready", if it is then we can
     * continue, if it is not, then we should just exit with
     * appropriate status
     *
     * note that in either case, it is safe for us to mark the i/f
     * as "not ready"
     */
    pthread_mutex_lock (&dcesx_ifrdy_mutex);

    ifrdy = dcesx_ifrdy_flag[if_index]; /* get the ready status */
    dcesx_ifrdy_flag[if_index] = false; /* mark this i/f not ready */

    pthread_mutex_unlock (&dcesx_ifrdy_mutex);

    if (!ifrdy)
    {
        *status = dcesx_s_ifrdy;
	return;
    }

    /*
     * display some progress information if we are not operating in
     * "quiet" mode
     */
    if (!quiet)
    {
        rpc_binding_to_string_binding (binding_handle,
                                       &string_binding,
                                       status);

        if (*status != rpc_s_ok)
        {
            dcesx_log ("%02d: *** rpc_binding_to_string_binding() failed\n", if_index);
            REPORT_DCE_ERROR (*status);
            exit (dcesx_s_rpc_binding_to_string_binding);
        }

        dcesx_log ("%02d: RPC: call from %s to unregister\n",
                   if_index, string_binding);

        rpc_string_free (&string_binding, status);

        if (*status != rpc_s_ok)
        {
            dcesx_log ("%02d: *** rpc_string_free() failed\n", if_index);
            REPORT_DCE_ERROR (*status);
            exit (dcesx_s_rpc_string_free);
        }
    }

    /*
     * unregister the current interface
     */
    dcesx_server_unregister (if_index, status);

    if (*status == dcesx_s_ok)
    {
        /*
         * print out something if running in quiet mode to indicate
         * that we're not dead
         */
        if (quiet)
        {
            fprintf (stderr, ".");
        }

        /*
         * keep track of the number of passes we've made
         */
        pass_count[if_index] += 1;
        if (!quiet)
        {
            int i;
            char buf[256], tmpbuf[16];

            buf[0] = '\0';
            for (i=0; i<if_num; i++)
            {
                sprintf (tmpbuf, "%d/%d ", i, pass_count[i]);
                strcat (buf, tmpbuf);
            }

            dcesx_log ("%02d: passes %s\n", if_index, buf);

            if (pass_count[if_index] >= passes)
            {
                dcesx_log ("%02d: interface complete\n", if_index);
            }
        }

        /*
         * see if we have been given a specific number of passes to
         * cycle through, if not, then we are not done
         */
        done = false;
        if (passes > 0)
        {
            /*
             * we are not done until all requested interfaces have
             * been exercised the requested number of times
             */
            done = true;
            for (i=0; i<if_num; i++)
            {
                if (pass_count[i] < passes)
                {
                    done = false;
                }
            }
        }

        /*
         * if we're done then shutdown the listener thread
         */
        if (done)
        {
            if (!quiet)
            {
                dcesx_log ("%02d: *** shutting down listener thread\n", if_index);
            }

            rpc_mgmt_stop_server_listening (NULL, status);
            if (*status != rpc_s_ok)
            {
                dcesx_log ("%02d: *** rpc_mgmt_server_stop_listening() failed\n", if_index);
                REPORT_DCE_ERROR (*status);
                exit (dcesx_s_rpc_mgmt_stop_server_listening);
            }

            if (!quiet)
            {
                dcesx_log ("%02d: *** successful completion\n", if_index);
            }
            else
            {
                fprintf (stderr, "\n");
            }
            return;
        }

        /*
         * if we're running in "multi_client" mode then we just
         * re-register the original interface, otherwise we register
         * the next interface in sequence
         */
        if (multi_client)
        {
            next_if_index = if_index;
        }
        else
        {
            next_if_index = if_index + 1;
            if (next_if_index == if_num)
            {
                next_if_index = 0;
            }
        }

        /*
         * register the interface if it has not been exercised the
         * requested number of times
         */
        if ((pass_count[next_if_index] < passes) || (passes == 0))
        {
            dcesx_server_register (next_if_index, status);
        }
    }
}

/*
**  ROUTINE NAME:       dcesx_client
**
**  SCOPE:              INTERNAL - declared locally.
**
**  DESCRIPTION:
**
**  This routine will locate a server for the interface specified in
**  the directory service. The starting point for this search will
**  randomly be either the group, profile or local host's server entry.
**
**  INPUTS:     
**
**      if_index        index of the interface to look up and make
**                      an RPC to
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

static void dcesx_client (if_index)

int         if_index;

{
    rpc_binding_handle_t        binding_handle;
    rpc_ns_handle_t             import_context;
    unsigned32                  status;
    unsigned32                  directory_search;
    unsigned_char_p_t           string_binding;
    boolean32                   call_complete;
    boolean32                   more_bindings;

    /*
     * yield until there is a listener thread running
     */
    while (!listener_running)
    {
        pthread_yield();
    }

    /*
     * randomly pick a search context to start with and
     * keep searching until a usable binding handle is found
     */
    directory_search = rand() % dcesx_c_directory_names_max;
    call_complete = false;

    while (!call_complete)
    {
        /*
         * set up a search context in order to import a binding handle
         * for a dcesx server from the directory service -
         * pick a server entry, group or profile at random to import from
         */
        if (re_random)
        {
            directory_search = rand() % dcesx_c_directory_names_max;
        }
        if (do_lock) pthread_mutex_lock (&dcesx_ifreg_mutex);
        rpc_ns_binding_import_begin (
            rpc_c_ns_syntax_default,
            (unsigned_char_t *) dcesx_g_directory_names[directory_search],
            *dcesx_g_if_table[if_index],
            NULL,
            &import_context,
            &status);
        if (do_lock) pthread_mutex_unlock (&dcesx_ifreg_mutex);
        
        if (status != rpc_s_ok)
        {
            dcesx_log ("%02d: *** Can't begin import\n", if_index);
            REPORT_DCE_ERROR (status);
            exit (dcesx_s_rpc_ns_binding_import_begin);
        }
        
        if (!quiet)
        {
            dcesx_log ("%02d: importing from %s\n", if_index,
                       dcesx_g_directory_names[directory_search]);
        }
        
        more_bindings = true;

        while (!call_complete && more_bindings)
        {
            /*
             * import a single binding, then try it in a real RPC
             */
            if (do_lock) pthread_mutex_lock (&dcesx_ifreg_mutex);
            rpc_ns_binding_import_next (import_context, &binding_handle, &status);
            if (do_lock) pthread_mutex_unlock (&dcesx_ifreg_mutex);

            if (status != rpc_s_ok)
            {
                dcesx_log ("%02d: *** Can't import next binding\n", if_index);
                REPORT_DCE_ERROR (status);

                if (status == rpc_s_no_more_bindings)
                {
                    /*
                     * request a skulk
                     */
                    dcesx_skulk_request (if_index);

                    /*
                     * make note of the fact that there are no more
                     * bindings and continue looping
                     */
                    more_bindings = false;
                    continue;
                }
                else
                {
                    exit (dcesx_s_rpc_ns_binding_import_next);
                }
            }
            
            /*
             * Make an RPC to unregister this interface and register a
             * new one.
             */
            if (!quiet)
            {
                rpc_binding_to_string_binding
                    (binding_handle, &string_binding, &status);

                if (status != rpc_s_ok)
                {
                    dcesx_log ("%02d: *** rpc_binding_to_string_binding() failed\n", if_index);
                    REPORT_DCE_ERROR (status);
                    exit (dcesx_s_rpc_binding_to_string_binding);
                }

                dcesx_log ("%02d: RPC: calling %s to unregister\n",
                           if_index, string_binding);
            }

            dcesx_g_c_epv_table[if_index]->dcesx_server (binding_handle, 
                                                         if_index,
                                                         &status);

            /*
             * if the call doesn't get through, try to import another
             * server binding 
             */
            if (status != rpc_s_ok)
            {
                dcesx_log ("%02d: *** call to %s failed\n",
                           if_index, string_binding);
                if (status == dcesx_s_ifrdy)
                {
                    dcesx_log ("%02d: *** server i/f was not available\n",
                               if_index);
                }
                else
                {
                    REPORT_DCE_ERROR (status);
                }
            }
            else
            {
                call_complete = true;
            }
            
            rpc_binding_free (&binding_handle, &status);

            if (status != rpc_s_ok)
            {
                dcesx_log ("%02d: *** Can't free binding\n", if_index);
                REPORT_DCE_ERROR (status);
                exit (dcesx_s_rpc_binding_free);
            }

            if (!quiet)
            {
                rpc_string_free (&string_binding, &status);

                if (status != rpc_s_ok)
                {
                    dcesx_log ("%02d: *** rpc_string_free() failed\n", if_index);
                    REPORT_DCE_ERROR (status);
                    exit (dcesx_s_rpc_string_free);
                }
            }
        }
        
        /*
         * free the search context
         */
        rpc_ns_binding_import_done (&import_context, &status);

        if (status != rpc_s_ok)
        {
            dcesx_log ("%02d: *** Can't free search context\n", if_index);
            REPORT_DCE_ERROR (status);
            exit (dcesx_s_rpc_ns_binding_import_done);
        }
    }

    /*
     * terminate this thread
     */
    if (!quiet)
    {
        dcesx_log ("%02d: exiting thread\n", if_index);
    }

    pthread_exit (dcesx_s_ok);
}

/***************************************************************************
 *
 * Subroutines
 *
 **************************************************************************/

/*
 * This routine parses the command line *after* the operation name
 * (that is, it ignores the first two words in the line, the program
 * name and the operation name), and returns:
 *
 *  - the first character of any command line switch, and
 *  - the value of the argument specified by the switch
 *
 * Note several diferences from "standard" Unix getopt fare:
 *
 *  -   semi-colon can be used in options strings to indicate optional
 *      arguments (in addition to colon for required arguments)
 *
 *  -   the switch character must always be directly after the '-' (that is,
 *      it is not permitted to concatenate multiple switches into one)
 *
 *  -   the routine will print an error message and exit if an illegal
 *      switch or syntax is detected
 *
 *  -   a hyphen without a switch character is permitted ('\0' will be
 *      returned) so it can be used to allow a switch with optional arguments
 *      to precede some other non-switch token
 */
 
static int get_options (argc, argv, opts)

int         argc;
char        **argv, *opts;

{
    int         switch_ptr = 1;
    char        c, *option_ptr;


    optarg = NULL;
    option_error = false;

    /*
     * if the option index points past the last token, leave now
     */
    if (option_index >= argc)
    {
        return (EOF);
    }
    
    /*
     * see if it points to a switch
     */
    if (argv[option_index][0] != '-')
    {
        return ('\0');
    }

    /*
     * see if there's a switch character
     */
    if (argv[option_index][1] == '\0')
    {
        option_index++;
        return ('\0');
    }

    /*
     * see if this is the end of parsing
     */
    if (strcmp (argv[option_index], "--") == 0)
    {
        /*
         * if so, just bump the option index for future reference
         */
        option_index++;
        return (EOF);
    }

    /*
     * get the first character of the option switch (past the '-')
     */
    c = argv[option_index][switch_ptr];

    /*
     * see if the first character is in the specified option set
     * (note: colon and semi-colon cannot be used as first characters,
     * because they are used as markers in the option set)
     */
    if (c == ':' || c == ';' ||
        (option_ptr = (char *) strchr (opts, (int) c)) == 0)
    {
        option_error = true;
        printf (">>> command syntax error : illegal option -- %c\n", c);
        dcesx_usage();
        exit (dcesx_s_usage);
    }

    /*
     * see what kind of option argument we have here -
     * colon for required, semi-colon for optional
     */
    option_ptr++;
    
    if (*option_ptr == ':' || *option_ptr == ';')
    {
        /*
         * see if the argument is appended to the switch (no space)
         */
        if (argv[option_index][switch_ptr+1] != '\0')
        {
            /*
             * if so, use the rest of the switch as the argument
             */
            optarg = &argv[option_index++][switch_ptr+1];
        }
        else
        {
            /*
             * if the argument is optional, see if next token is a switch
             * (if there is one)
             */
            if (*option_ptr == ';')
            {
                if (++option_index >= argc || argv[option_index][0] == '-')
                {
                    /*
                     * mark the option argument as omitted and leave
                     */
                    optarg = NULL;
                }
                else
                {
                    /*
                     * use the next token as the option argument
                     */
                    optarg = argv[option_index++];
                }
            }
            else
            {
                /*
                 * see if there are any more tokens to use for the argument
                 */
                if (++option_index >= argc)
                {
                    option_error = true;
                    printf
( ">>> command syntax error : option requires an argument -- %c\n", c);
                    dcesx_usage();
                    exit (dcesx_s_usage);
                }
                else
                {
                    /*
                     * use the next token as the option argument
                     */
                    optarg = argv[option_index++];
                }
            }
        }
    }
    else
    {
        /*
         * if there is no option argument required, mark the argument as
         * empty, bump the option index, and return the switch character
         */
        optarg = (char *) NULL;
        option_index++;
    }

    return (c);
}

/*
 * dcesx_add_group
 */
static void dcesx_add_group (if_index)
int if_index;
{
    error_status_t status;

    pthread_mutex_lock (&dcesx_group_mutex);

    if (dcesx_groups == 0)
    {
        rpc_ns_group_mbr_add (
            rpc_c_ns_syntax_default,
            (unsigned_char_t *) dcesx_g_directory_names[dcesx_c_group_name],
            rpc_c_ns_syntax_default,
            (unsigned_char_t *) dcesx_g_directory_names[dcesx_c_entry_name],
            &status);        

        if (status != rpc_s_ok)
        {
            dcesx_log ("*** Can't add server to group\n");
            REPORT_DCE_ERROR (status);
            exit (dcesx_s_rpc_ns_group_mbr_add);
        }

        if (!quiet)
        {
            dcesx_log ("added server entry %s to group %s\n", 
                       dcesx_g_directory_names[dcesx_c_entry_name],
                       dcesx_g_directory_names[dcesx_c_group_name]);
        }

        dcesx_skulk_request (if_index);
    }

    dcesx_groups++;

    pthread_mutex_unlock (&dcesx_group_mutex);
}

/*
 * dcesx_remove_group
 */
static void dcesx_remove_group (if_index)
int if_index;
{
    error_status_t status;

    pthread_mutex_lock (&dcesx_group_mutex);

    dcesx_groups--;

    if (dcesx_groups == 0)
    {
        rpc_ns_group_mbr_remove (
            rpc_c_ns_syntax_default,
            (unsigned_char_t *) dcesx_g_directory_names[dcesx_c_group_name],
            rpc_c_ns_syntax_default,
            (unsigned_char_t *) dcesx_g_directory_names[dcesx_c_entry_name],
            &status);        

        if (status != rpc_s_ok)
        {
            dcesx_log ("*** Can't remove server from group\n");
            REPORT_DCE_ERROR (status);
            exit (dcesx_s_rpc_ns_group_mbr_remove);
        }

        if (!quiet)
        {
            dcesx_log ("removed server entry %s from group %s\n", 
                       dcesx_g_directory_names[dcesx_c_entry_name],
                       dcesx_g_directory_names[dcesx_c_group_name]);
        }

        dcesx_skulk_request (if_index);
    }

    pthread_mutex_unlock (&dcesx_group_mutex);
}

/*
 * dcesx_usage - a simple routine to display usage
 */
static void dcesx_usage()
{
printf("usage:dcesx [-help] [-DKLQRSVMH] [-i ifnum] [-p passes] [-r retries] [-t delay] [-m max calls]\n");
}

/*
 * dcesx_help - a routine to display a help screen
 */
static void dcesx_help()
{
    dcesx_usage();

    printf("\n    valid switches:\n\n");
    printf("        -D   enable debug information\n");
    printf("        -K   perform skulks\n");
    printf("        -L   lock around imports\n");
    printf("        -M   enable multiple client threads\n");
    printf("        -Q   quiet mode\n");
    printf("        -R   re-randomize search order\n");
    printf("        -S   single system\n");
    printf("        -V   verbose\n");
    printf("\n");
    printf("        -H     help (display this information)\n");
    printf("        -help  help (display this information)\n");
    printf("\n");
    printf("        -i ifnum     specify # of interfaces\n");
    printf("        -m max_calls specify # of call threads\n");
    printf("        -p passes    specify # of passes\n");
    printf("        -r retries   specify # of passes\n");
    printf("        -t delay     specify delay time\n");
}

#ifdef DCE_SEC
/* 
 * Determine if authentication is required.
 */
static void dcesx_do_login ()
{

  error_status_t sec_status;
  sec_login_handle_t login_context_h;

  /* Get current context and save it */
  sec_login_get_current_context(&login_context_h, &sec_status);
  if (sec_status == error_status_ok)
    Authentication = 1;
  else {
    printf("sec_login_get_current_context %d\n", sec_status);
    Authentication = 0;
 }
}

#endif  /* DCE_SEC */

/*
 * dcesx_listener
 */
static void dcesx_listener()
{
    unsigned32 status;

    if (!quiet)
    {
        dcesx_log ("listener: listening for calls ...\n");
    }

    listener_running = true;
    rpc_server_listen (max_calls, &status);
    if (status != rpc_s_ok)
    {
        dcesx_log ( "*** Abnormal termination of server\n");
        REPORT_DCE_ERROR (status);
        exit (dcesx_s_rpc_server_listen);
    }

    /*
     * terminate this thread
     */
    if (!quiet)
    {
        dcesx_log ("listener: exiting thread\n");
    }

    pthread_exit (dcesx_s_ok);
}

/*
 * dcesx_skulk_root - a routine to skulk the route directory
 */
static void dcesx_skulk_root()
{
    int status;
    int retry;
    cds_full_name_t *root_dir_p = (cds_full_name_t *)"/.:";
    cdsFlagStat_t cdsFlags;

    if (!quiet)
    {
        dcesx_log ("SK: skulking /.:\n");
    }

    /*
     * skulk the root directory
     */
    for (retry=0; retry<retry_max; retry++)
    {
        if (retry != 0)
        {
            dcesx_log ("*** retry #%d - skulk root directory\n",
                       retry);
        }

        /*
         * initialize the flags for the skulk
         */
        memset (&cdsFlags, '\0', sizeof(cdsFlags));
        cdsFlags.fsConf     = cdsConfMed;
        cdsFlags.fsNameType = cdsStrDCE;
#ifdef DCE_SEC
        cdsFlags.fsUnAuth   = !Authentication;
        cdsFlags.fsTrustAll = !Authentication;
#endif
        
        /*
         * call the nameservice
         */
        status = cdsSkulkDir (root_dir_p, &cdsFlags);

        if (status == CDS_SUCCESS) break;

        dcesx_log ("*** Unable to skulk root directory\n");
        REPORT_DCE_ERROR (cdsFlags.fsLocStat);
    }
    if (status != CDS_SUCCESS) exit (dcesx_s_cds_skulk_dir);

    if (!quiet)
    {
        dcesx_log ("SK: skulk complete\n");
    }
}

/*
 * dcesx_skulk_request
 */
static void dcesx_skulk_request (ifnum)
int ifnum;
{
    if (debug)
    {
        dcesx_log ("%02d: skulk request\n", ifnum);
    }

    /*
     * wait until any previous requests are satisfied
     */
    while (if_skulk[ifnum].rqst_flag)
    {
        pthread_yield();
    }

    pthread_mutex_lock (&if_skulk[ifnum].rqst_mutex);

    if_skulk[ifnum].rqst_flag = true;
    if_skulk[ifnum].done_flag = false;
    if_skulk[ifnum].skulk_id  = 0;

    while (!if_skulk[ifnum].done_flag)
    {
        pthread_mutex_lock (&dcesx_skulk_rqst_mutex);
        if (dcesx_skulker_waiting)
        {
            pthread_cond_signal (&dcesx_skulk_rqst_cond);
        }
        pthread_mutex_unlock (&dcesx_skulk_rqst_mutex);
        pthread_cond_wait (&if_skulk[ifnum].rqst_cond,
                           &if_skulk[ifnum].rqst_mutex);
    }

    if_skulk[ifnum].rqst_flag = false;

    pthread_mutex_unlock (&if_skulk[ifnum].rqst_mutex);

    if (debug)
    {
        dcesx_log ("%02d: skulk request satisfied\n", ifnum);
    }
}

/*
 * dcesx_skulker
 */
static void dcesx_skulker ()
{
    static int dcesx_skulk_rqst_num = 0;
    static int dcesx_skulk_rqst_max = 0;

    if (!quiet)
    {
        dcesx_log ("SK: waiting for skulk requests ...\n");
    }

    while (dcesx_skulker_needed)
    {
        boolean32 skulk_request;
        int i;

        /*
         * wait until there is a skulk request pending
         */
        skulk_request=false;
        while (!skulk_request && dcesx_skulker_needed)
        {
            for (i=0; i<dcesx_c_if_max; i++)
            {
                if (if_skulk[i].rqst_flag)
                {
                    skulk_request = true;
                }
            }
            if (!skulk_request)
            {
                struct timespec delay;
                struct timeval  now;
                int rc;

                pthread_mutex_lock (&dcesx_skulk_rqst_mutex);
                dcesx_skulker_waiting = true;

                if (debug)
                {
                    dcesx_log ("SK: waiting ... (%d seconds)\n",
                               delay_time);
                }
                
                /*
                 * delay and give the nameservice a chance to settle
                 */
                if (gettimeofday (&now, NULL))
                {
                    dcesx_log ("SK: can't get current time\n");
                    exit (dcesx_s_system);
                }
                delay.tv_sec  = now.tv_sec + delay_time;
                delay.tv_nsec = now.tv_usec * 1000;
                
                rc = pthread_cond_timedwait (&dcesx_skulk_rqst_cond,
                                             &dcesx_skulk_rqst_mutex,
                                             &delay);
                if (debug && (rc != 0))
                {
                    dcesx_log ("SK: timedwait timeout ... continuing\n");
                }

                dcesx_skulker_waiting = false;
                pthread_mutex_unlock (&dcesx_skulk_rqst_mutex);
            }
        }
        
        /*
         * A skulk request is pending, set the skulk_id of all
         * current requesters and keep track of the number of
         * requestors
         */
        dcesx_skulk_id++;
        dcesx_skulk_rqst_num = 0;
        for (i=0; i<dcesx_c_if_max; i++)
        {
            pthread_mutex_lock (&if_skulk[i].rqst_mutex);
            if (if_skulk[i].rqst_flag)
            {
                dcesx_skulk_rqst_num++;
                if_skulk[i].done_flag = false;
                if_skulk[i].skulk_id = dcesx_skulk_id;
            }
            pthread_mutex_unlock (&if_skulk[i].rqst_mutex);
        }

        /*
         * keep track of the maximum number of requesters
         */
        if (dcesx_skulk_rqst_max < dcesx_skulk_rqst_num)
        {
            dcesx_skulk_rqst_max = dcesx_skulk_rqst_num;
        }

        if (debug)
        {
            dcesx_log ("SK: skulk requests - %d pending (%d max)\n",
                       dcesx_skulk_rqst_num, dcesx_skulk_rqst_max);
        }

        /*
         * skulk the root
         */
        dcesx_skulk_root();

        /*
         * signal all the current requesters who match the current
         * skulk id
         */
        for (i=0; i<dcesx_c_if_max; i++)
        {
            pthread_mutex_lock (&if_skulk[i].rqst_mutex);
            if (if_skulk[i].rqst_flag && (if_skulk[i].skulk_id == dcesx_skulk_id))
            {
                if (if_skulk[i].done_flag)
                {
                    dcesx_log ("SK: *** if #%d was 'stuck'\n", i);
                }
                else
                {
                    if_skulk[i].done_flag = true;
                }
                if (debug)
                {
                    dcesx_log ("SK: signaling if #%d\n", i);
                }
                pthread_cond_broadcast (&if_skulk[i].rqst_cond);
            }
            pthread_mutex_unlock (&if_skulk[i].rqst_mutex);
        }
        pthread_yield();
    }

    /*
     * terminate this thread
     */
    if (!quiet)
    {
        dcesx_log ("SK: maximum number of skulk requesters was %d\n", 
                   dcesx_skulk_rqst_max);

        dcesx_log ("SK: exiting thread\n");
    }

    pthread_exit (dcesx_s_ok);
}
