/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtsscp.c,v $
 * Revision 1.1.13.2  1996/02/17  23:34:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:57  marty]
 *
 * Revision 1.1.13.1  1995/12/08  18:09:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/8  1995/01/30  22:44 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/7  1994/10/10  14:38 UTC  pare
 * 	Merge bl21 changes.
 * 	[1995/12/08  17:24:20  root]
 * 
 * Revision 1.1.11.11  1994/09/27  20:56:20  rousseau
 * 	Fix test for last observers skew (CR 12371).
 * 	[1994/09/27  20:56:10  rousseau]
 * 
 * 	HP revision /main/HPDCE02/6  1994/09/26  13:51 UTC  pare
 * 	Merge bl18 changes
 * 
 * Revision 1.1.11.10  1994/09/21  22:01:39  tom
 * 	Call HYPER_TO_U64 (from dce64.h) to help 64 bit platforms.
 * 	[1994/09/21  21:14:20  tom]
 * 
 * Revision 1.1.11.9  1994/08/26  20:47:26  cbrooks
 * 	CR11900 - dtscp show sync complete fails
 * 	[1994/08/26  20:13:03  cbrooks]
 * 
 * Revision 1.1.11.8  1994/08/24  21:24:38  cbrooks
 * 	CR11867 - dtscp core dumps if dtsd bindings file not present
 * 	[1994/08/24  21:23:59  cbrooks]
 * 
 * Revision 1.1.11.7  1994/08/23  20:32:11  cbrooks
 * 	Code Cleanup - explicit assign to Bits64 from idl_hyper_int
 * 	[1994/08/23  20:19:24  cbrooks]
 * 
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:55  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:52:45  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:29:14  cbrooks]
 * 
 * Revision 1.1.11.6  1994/08/16  18:12:55  cbrooks
 * 	CR 11494 - 64bit changes
 * 	[1994/08/09  18:47:48  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/5  1994/08/23  18:01 UTC  pare
 * 	Add comment delimeter
 * 
 * 	HP revision /main/HPDCE02/4  1994/08/23  17:49 UTC  pare
 * 	Delete dts_read_bindings( &bvec_p)
 * 
 * 	HP revision /main/HPDCE02/3  1994/08/18  14:48 UTC  pare
 * 	Merge changes from Hpdce02
 * 
 * Revision 1.1.11.5  1994/07/29  18:27:14  cbrooks
 * 	CR11466 - new functions for read/write bindings file
 * 	[1994/07/29  18:25:12  cbrooks]
 * 
 * Revision 1.1.11.4  1994/06/29  17:57:11  cbrooks
 * 	CR10860 show command does not list clock resolution
 * 	[1994/06/29  16:52:26  cbrooks]
 * 
 * 	CR10802 - bad error message
 * 	[1994/06/29  14:08:36  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/07/19  12:53 UTC  pare
 * 	fix merge problems
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/30  12:26 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.7.5  1993/10/29  15:06:05  pare
 * 	Added #pragma covcc !instr around functions actParseSimple and actParseFull
 * 	since these functions are only touched via DECNET. The conditional compile
 * 	is inside the function so these functions are reported as uncovered by VISTA
 * 	coverage tools -- the above pragma statement turns off the coverage monitor.
 * 	[1993/10/29  15:05:28  pare]
 * 
 * Revision 1.1.7.4  1993/09/23  18:00:22  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/23  17:59:47  pare]
 * 
 * Revision 1.1.8.3  1993/09/15  17:47:04  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.3  1993/07/15  16:01:51  truitt
 * 	OT4769: The 'help show' command was showing the dts trace commands
 * 	        as valid commands.  They should only show up there during
 * 	        debug mode.
 * 	OT7973: Remove support for the 'Too Many Server Detected' command.
 * 	        Add support for the 'Time Provider Timeout Detected' command.
 * 	[1993/07/15  16:01:24  truitt]
 * 
 * Revision 1.1.11.3  1994/05/16  18:51:42  rhw
 * 	added #include <dts.h>
 * 	[1994/05/16  03:58:52  rhw]
 * 
 * 	fix for CR 10642
 * 
 * Revision 1.1.11.2  1994/05/12  15:17:42  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:40  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:57  rhw]
 * 
 * Revision 1.1.11.1  1994/04/21  22:05:33  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:52:21  cbrooks]
 * 
 * 	Checkpoint
 * 	[1994/04/01  22:35:03  cbrooks]
 * 
 * 	Checkpoint
 * 	[1994/03/28  20:22:52  cbrooks]
 * 
 * 	Intermediate checkin of dtsscp for remote control enhancement.
 * 	Enable command line parsing for "-h hostname"; otherwise,
 * 	default to the name returned by gethostname() [the local host]
 * 	[1994/03/10  22:48:19  cbrooks]
 * 
 * Revision 1.1.8.2  1993/07/14  21:26:12  tom
 * 	Change Too many servers counter to time provider timeouts counter.
 * 	[1993/07/14  20:41:09  tom]
 * 
 * Revision 1.1.6.8  1993/02/03  23:37:33  tom
 * 	Revision 1.1.7.2  1993/07/08  19:47:29  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:46:49  root]
 * 
 * Revision 1.1.5.5  1993/05/28  13:03:18  truitt
 * 	Corrected bonehead mistake in version string.
 * 	[1993/05/28  13:02:57  truitt]
 * 
 * Revision 1.1.5.4  1993/05/25  12:37:37  truitt
 * 	Add new version control string.
 * 	[1993/05/25  12:37:15  truitt]
 * 
 * Revision 1.1.5.3  1993/05/10  18:17:10  truitt
 * 	Add version control string.
 * 	[1993/05/10  18:16:41  truitt]
 * 
 * 	Bug 7121 - Change array subscript from K_SERV_NOT_TSG_MEMBER_ATTR
 * 	to K_SERV_NOT_TSG_MEMBER_CNTR in InitCounterDisplay.
 * 	[1993/02/03  21:47:54  tom]
 * 
 * Revision 1.1.6.7  1993/02/02  15:09:09  tom
 * 
 * 	Bug 6883 - Change help message:
 * 	 LOCAL FAULTS DETECTED  --> LOCAL TIMES NOT INTERSECTING
 * 	 FAULTY SERVERS DETECTED --> SERVER TIMES NOT INTERSECTING
 * 	[1993/02/01  22:32:21  tom]
 * 
 * 	Bug 4807 - show uid returned garbage.  Set the dataType to UUID
 * 	if we are going to pass a uuid through rpc.
 * 	[1993/01/28  22:37:18  tom]
 * 
 * 	The cma package messes with tty settings and doesn't always
 * 	get a chance to put them back. The program should have its
 * 	own file descriptor in this case so as not to do strange
 * 	things to the calling shell.
 * 	[1993/01/13  20:58:37  jd]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  22:11:50  htf]
 * 
 * 	Bug 6410 - Add setlocale for i18n.
 * 	[1992/12/15  15:41:22  tom]
 * 
 * 	Fix OT 5898 that produced incorrect SHOW output on a non-DECnet system.
 * 	[1992/11/05  15:27:54  yankes]
 * 
 * Revision 1.1.5.2  1993/03/18  14:16:37  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/18  14:15:27  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:32:25  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.2  1992/12/21  23:33:17  smythe
 * 	added OSF OT 5898 fix to disallow obsolete show commands
 * 	[1992/12/19  17:40:52  smythe]
 * 
 * Revision 1.1.1.7  1992/09/21  18:54:46  osfrcs
 * 	    Loading drop Dce1_0_1fnl
 * 
 * Revision 1.1.2.7  1992/06/30  15:28:36  ko
 * 	Fix Typo -> Synchronization NOT Synchronizaton
 * 	[1992/06/29  13:11:19  ko]
 * 
 * Revision 1.1.2.6  1992/06/09  20:30:02  ko
 * 	Fix OT 3916 - not displaying correct version number.  DTSSCP was hardcoding
 * 	the support level to '0' for some reason on all version typed attributes.
 * 	[1992/06/04  16:28:07  ko]
 * 
 * Revision 1.1.2.5  1992/06/02  14:31:57  ko
 * 	Fix ot 3921 & 3917 - displaying 'DTSS Version' instead of 'DTS Version' on a
 * 	show all command, printing counters as two discrete 32 bit longs instead of
 * 	printing as 64 bit quantities; now printing 64 bits & DTS Version.
 * 	[1992/06/01  14:57:41  ko]
 * 
 * Revision 1.1.2.4  1992/05/13  20:18:17  ko
 * 	fix ot 2825 - better help, remove DIGITAL only attributes
 * 	[1992/05/13  20:17:04  ko]
 * 
 * Revision 1.1.3.2  1992/05/07  15:38:17  ko
 * 	fix ot 2820, do not bugcheck on recoverable errors
 * 
 * Revision 1.1.2.2  1992/04/22  17:19:38  rsalz
 * 	<<<Fixed uninitialized variable freeStatus, for OT 2792>>>
 * 	[1992/04/22  15:38:22  rsalz]
 * 
 * Revision 1.1  1992/01/19  15:35:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *	Module:	DTSSCP.C
 */
/*
 ****************************************************************************
 *									    *
 *  COPYRIGHT (c) 1988, 1989,1990 BY					    *
 *  DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASSACHUSETTS.		    *
 *  ALL RIGHTS RESERVED.						    *
 * 									    *
 *  THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND COPIED   *
 *  ONLY IN  ACCORDANCE WITH  THE  TERMS  OF  SUCH  LICENSE  AND WITH THE   *
 *  INCLUSION OF THE ABOVE COPYRIGHT NOTICE. THIS SOFTWARE OR  ANY  OTHER   *
 *  COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY   *
 *  OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE IS  HEREBY   *
 *  TRANSFERRED.							    *
 * 									    *
 *  THE INFORMATION IN THIS SOFTWARE IS  SUBJECT TO CHANGE WITHOUT NOTICE   *
 *  AND  SHOULD  NOT  BE  CONSTRUED AS  A COMMITMENT BY DIGITAL EQUIPMENT   *
 *  CORPORATION.							    *
 * 									    *
 *  DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE  OR  RELIABILITY OF ITS   *
 *  SOFTWARE ON EQUIPMENT WHICH IS NOT SUPPLIED BY DIGITAL.		    *
 * 									    *
 *									    *
 ****************************************************************************
 */
/*
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:
 *
 *	This module contains the OS-independent peices of the control
 *	program for DTSS.
 *
 * ENVIRONMENT: 	all
 *
 * AUTHOR:	        K.P. Baryiames
 *
 * CREATION DATE: 	July, 1990
 *
 */

#include <dts.h>
#include <dtscp.h>		/* local prototypes  */
#include <dtss_includes.h>	/* top-level DTSS include file */
#include <dtssdef.h>
#include <ctype.h>		/* isspace() macro */
#include <stdio.h>		/* standard I/O library */
#include <dce/dce_cf.h>
#include <string.h>
#include <stdlib.h>		/* prototypes for some of the std lib funcs */
#include <locale.h>
#include <unistd.h>
#include <errno.h>

#include <nl_types.h>

#include <prsdef.h>		/* portable parser defintions */
#include <dtss_attr_ranges.h>   /* attribute ranges */
#include <dce/dce64.h>

#define UTC_TO_BIN_ABS(binTimePtr, UTCptr)\
*(binTimePtr) = *(UTCptr) 

/*
 *	Defines
 */

#define CP_ERROR		(0)	/* recoverable error */
#define CP_SUCCESS		(1)

#if '0' != '\060'		/* If not ASCII */
# define PTR "?"		/*  use "?" as pointer */
#else
# define PTR "^"		/*  else use "^" as pointer */
#endif

/*
 *	Global Variables
 */

char	*dce_msg_trailer = " (dce / dts)";

#define TRIM_MSG_TRAILER(msg) msg[strlen((char *)msg) - \
				  strlen(dce_msg_trailer)] = '\0'


#if CONFIG_SECURITY

/*
 * Security state variables
 */

sec_login_handle_t  loginContext_h;
unsigned_char_t     *dtsPrincipalName;

#endif /* CONFIG_SECURITY */

/*
 *	Static Variables required by Parser / Control Program
 */

static char prsBinFile [K_MAX_FILENAME+1];	/* parser file name */
static unsigned char allDone;			/* ready to exit CP? */
static unsigned char sendCtlMsg;		/* CTLmsg to send to the TS? */
static char cmdBuf[512];			/* user's command line buffer*/
static struct prs prsContext;			/* parser context block */
static char displayOutput;			/* flag - display the output */

#ifdef DCE_DTS_DEBUG
int debug = 0;					/* parser debug */
#endif

/*
 * These variables are used for directives sent to the Time Service
 * via RPC.
 */

CtlFunc            ctlFunc;        /* used to identify the directive */
idl_long_int       timeServType;   /* used with CREATE (sever or clerk */
idl_long_int       clockSet;       /* used with ENABLE and SYNCH   */
idl_long_int       serverCount;    /* number of servers returned in SHOW */
utc_t              timeVal;        /* used with UPDATE             */
Change_t           changeDir;      /* used sith CHANGE             */
CtlSet_t           setAttr;        /* used with SET DEFAULT        */
CtlSetAttr_t       setAttrVal;     /* used with SET                */
AttrVal_t          attribute;      /* used with SHOW               */
idl_long_int       shAttribute;    /* used with SHOW               */
CtlShowAllChar     showAllChar;    /* used with SHOW ALL CHARACTS  */
CtlShowAllStatus   showAllStatus;  /* used with SHOW ALL STATUS    */
CtlShowAllCntrs    showAllCntrs;   /* used with SHOW ALL COUNTERS  */
                                   /* used with SHOW LOCAL SERVERS */
ServerID           localServers[MAX_TSERVERS];
                                   /* used with SHOW GLOBAL SERVERS */
ServerID           globalServers[MAX_TSERVERS];


/*
 * user buffers
 */

CtlSetData userBuffer;          /* set attributes */
int userBufferSet = FALSE;
int userBufferSize = 0;

char IsClerkCounter[K_NUM_COUNTERS];   /* array of booleans, true if event  *
					* is a clerk event, false otherwise *
					* Add one, since events are numb'd  *
					* starting at 1.                    */



/*
 *	Parser's Action Routines and Routine Table
 */

int actHelp _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actComment _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actCreate _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actEnable _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actUpdate _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actSynchronize _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actChangeEpoch _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actAdvertise _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actUnadvertise _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actSet _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actShow _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actDisable _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actDelete _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actExit _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actParseDecimal _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actParseBool _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actParseCourier _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actParseString _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actParseSimple _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actParseFull _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actParseBRT _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actParseBAT _DCE_PROTOTYPE_ ((struct prs *pcbp));
int actSetHost  _DCE_PROTOTYPE_ ((struct prs * pcbp));

int pr_read _DCE_PROTOTYPE_ ((struct prs *, char *));
int pr_command _DCE_PROTOTYPE_ ((struct prs *));

struct action actRoutines[] =	/* action routine table */
{
    {actHelp, "actHelp"},
    {actComment, "actComment"},
    {actCreate, "actCreate"},
    {actEnable, "actEnable"},
    {actUpdate, "actUpdate"},
    {actSynchronize, "actSynchronize"},
    {actAdvertise, "actAdvertise"},
    {actUnadvertise, "actUnadvertise"},
    {actChangeEpoch, "actChangeEpoch"},
    {actSet, "actSet"},
    {actShow, "actShow"},
    {actDisable, "actDisable"},
    {actDelete, "actDelete"},
    {actExit, "actExit"},
    {actParseDecimal, "actParseDecimal"},
    {actParseBRT, "actParseBRT"},
    {actParseBAT, "actParseBAT"},
    {actParseString, "actParseString"},
    {actParseSimple, "actParseSimple"},
    {actParseFull, "actParseFull"},
    {actParseCourier, "actParseCourier"},
    {actParseBool, "actParseBool"},
    {actSetHost,   "actSetHost"}
};

/*
 *	Internal Prototypes
 */

void	FindPrsBinFile _DCE_PROTOTYPE_ ((char * ));

static int ParseCommand _DCE_PROTOTYPE_ ((void));

SetShow_t GetAttrType _DCE_PROTOTYPE_ ((long int ));

void DisplayCmdOutput _DCE_PROTOTYPE_ (( CtlFunc ));

void InitLoopVariables _DCE_PROTOTYPE_ ((void));

void InitCounterDisplay _DCE_PROTOTYPE_ ((void));

void PrintAttrText _DCE_PROTOTYPE_ ((int));

void PrintAttrValue _DCE_PROTOTYPE_ ((int, int , VOID *));

void PrintShowChar _DCE_PROTOTYPE_ ((CtlShowAllChar *));

void PrintShowServers _DCE_PROTOTYPE_ ((long , ServerID []));

static int InitClientRPC _DCE_PROTOTYPE_ ((rpc_binding_handle_t *, char *));

static int SendRPCrequest _DCE_PROTOTYPE_ ((rpc_binding_handle_t ));

static int ReadParseTable _DCE_PROTOTYPE_ ((void));

static void dtscp_init_svc _DCE_PROTOTYPE_(( char * ));

void dts_read_bindings _DCE_PROTOTYPE_(( rpc_binding_vector_t **, error_status_t *));

char * dce_error_text _DCE_PROTOTYPE_((error_status_t ));

/*
 * Externs & Defines
 */

int fd_stdin = 0 ;

rpc_binding_handle_t dts_g_dtscp_binding_h;

char * dtscp_g_local_hostname = 0 ;

char ** protseq_select_vector = 0 ;

static char * protseq_select_all [] = 
{
   "ncacn_ip_tcp",
   "ncadg_ip_udp",
   (char *)0
};


#define HOST_NAME_SIZE 128


/*
 *
 *    M A I N  
 *
 *
 */

int main ( int 	argc, char 	**argv)
{
    int			status = 0 ;
    error_status_t	rpcStatus = rpc_s_ok ;
    char * dtsd_hostname  = 0 ;
    char * requested_hostname = 0 ;

    /*
     * The cma package messes with tty settings and doesn't always
     * get a chance to put them back. The program should have its
     * own file descriptor in this case so as not to do strange 
     * things to the calling shell.
     */

    cma_init(); 
    fd_stdin = 0;
    if(isatty(fileno(stdin)))
       {
	 close(fileno(stdin));
	 fd_stdin = open("/dev/tty",O_RDWR);
       }

    if (FALSE)
	cma_debug();

    setlocale(LC_ALL, "");

    dtscp_init_svc( argv[0]);

    /*
     * Initialze the parser context structure.
     */
    prsContext.cmd = cmdBuf;		/* command line buffer */
    prsContext.cms = sizeof(cmdBuf);	/* size of command line buffer */
    prsContext.tbs = 0;			/* size of parse table */
    prsContext.act = actRoutines;	/* action routines */


    /*
     * Read in the parser file and set up to parse the user's commands.
     */

    if (!ReadParseTable())
	return (EXIT_FAILURE);


    dce_cf_get_host_name( &dtscp_g_local_hostname, &rpcStatus) ;
    if (rpcStatus != dce_cf_st_ok)
    {
	dts_dtscp_check_status( rpcStatus, "dce_cf_get_host_name:", 
			       dts_e_dtscp_resume);
	return ( EXIT_FAILURE) ;
    }
    else 
    {
	char * p = strrchr(dtscp_g_local_hostname, '/'); /* find the slash */

	if (p)
	    memmove(dtscp_g_local_hostname, p + 1, strlen(p));
    }
	    
    if (argc > 1) 
    {
	int nargc = argc ;
	char *** nargv = &argv ;
	
	if( dts_dtscp_parseargs( &nargc, nargv, &requested_hostname) )
	    return ( EXIT_FAILURE ) ;
	argc = nargc ;
	argv = *nargv ;
    }

    if (requested_hostname != 0 && /* none given */
	strcmp(dtscp_g_local_hostname, requested_hostname) != 0)  
	dtsd_hostname = requested_hostname ;
    else
	dtsd_hostname = dtscp_g_local_hostname ;
    
    /*
     * Initialize RPC client-code if communicating with the Time Service
     * via RPC.
     */

    if (!InitClientRPC (&dts_g_dtscp_binding_h, dtsd_hostname))
	return (EXIT_FAILURE);


    /*
     * Initalize counter display control.  Some counters are relevant 
     * only to servers, so Clerks do not display them.
     */

    InitCounterDisplay();


    /*
     * Global variable which is set when it's time to exit the CP.
     */
    allDone = FALSE;

    do
    {
	InitLoopVariables();

	/*
	 * If a command was specified on the OS command line, concatentate
	 * the parameters and 'prime' the command buffer.  Put a space between
	 * the arguments as we build the command line.
	 */
	if (argc > 1)
	{
	    char	*strPtr;
	    int		i;

	    for (strPtr = cmdBuf, i = 1; i < argc; i++)
            {
		(void) strcpy (strPtr, argv[i]);

		if (i < argc - 1)		/* if not the last argument */
		{
		    strPtr += strlen (strPtr);
		    *strPtr = ' ';
		    strPtr++;
		}
            }		

	    allDone = TRUE;
	}

	/*
	 * Now parse the command and send the resulting CtlMsg, if any, 
         * to the TS.
	 */
	status = ParseCommand();

	/*
	 * Send directive to the Time Service, via RPC
	 */

	if (status && sendCtlMsg)
	    status = SendRPCrequest( dts_g_dtscp_binding_h );

	/*
  	 * Determine if we need to output anything to the user.
	 * Do not check the alldone flag since we must service
	 * foreign command lines.  Check the function code instead.
 	 */
	if (status && displayOutput)
	    DisplayCmdOutput( ctlFunc );


    } while (!allDone);

    /*
     * Free the binding handle allocated by RPC
     */
    rpc_binding_free (&dts_g_dtscp_binding_h, &rpcStatus);


    /*
     * Free the parse table.
     */
    free (prsContext.tbl);

    /*
     * Exit conditionally based on the status of the last message
     * received from the time service.  This is only relevant to
     * commands entered via the command line interface.
     */

    if (argc > 1 && !status)
    {
	return (EXIT_FAILURE);
    }
    else
    {
	return (EXIT_SUCCESS);
    }
}

/*
 * D T S C _ I N I T _ S V C 
 * 
 * initialize serviceablity component of this program 
 */

static void dtscp_init_svc( char * progname) 
{
    char * p = strrchr( progname, '/') ;
    error_status_t st ;

    if (p) 
	p++;
    else
	p = progname ;

    dce_svc_set_progname( p , &st) ;
    dce_msg_define_msg_table( dts__table, 
			     sizeof dts__table /sizeof dts__table[0], &st) ;
    
    return ;
}


/*
 * R E A D _ P A R S E _ T A B L E
 *
 * Read the binary parse table.  Return TRUE if we succeed.  If we fail, display
 * an appropriate error message and return FALSE.
 *
 */
static int ReadParseTable(void)
{
    int			prStatus;
    error_status_t	dceStatus;
    int                 msgStatus;
    dce_error_string_t	dceErrorMsg;


    /*
     * Figure out what directory we should get the parse file from.
     */
    FindPrsBinFile (prsBinFile);

    /*
     * Determine the size of the parse table.  Pass in a buffer size of zero,
     * and let it complain and tell us how much it needs.  The allocate enough
     * memory to hold the parse table and read the parse table into memory.
     */
    prStatus = pr_read (&prsContext, prsBinFile);
    if (prStatus == RET_SIZE)
    {
	prsContext.tbl = (char *) malloc(prsContext.tbu);
	prsContext.tbs = prsContext.tbu;

	prStatus = pr_read (&prsContext, prsBinFile);
    }

    if (prStatus == RET_SUCCESS)
	return (TRUE);

    /*
     * We got an error.  Translate the PR error status into DCE error status
     * and display the appropriate message.
     */
    switch (prStatus)
    {
    case RET_OPENERROR:
	dceStatus = dts_s_prsfile_open;
	break;

    case RET_IOERROR:
	dceStatus = dts_s_prsfile_read;
	break;

    case RET_VERSION:
    case RET_FILE:
	dceStatus = dts_s_prsfile_version;
	break;

    case RET_SIZE:
    default:
	dceStatus = dts_s_prsfile_fatal;
	break;
    }

    dce_error_inq_text (dts_s_cp_init_prs, dceErrorMsg, &msgStatus);
    TRIM_MSG_TRAILER (dceErrorMsg);
    printf ("%s\n", (char *)dceErrorMsg);
    dce_error_inq_text (dceStatus, dceErrorMsg, &msgStatus);
    printf ((char *)dceErrorMsg, prsBinFile);
    printf ("\n");

    return (FALSE);
}


/*
 *  I N I T _ C L I E N T _ R P C
 *
 * Initialize the RPC client.
 * Obtain an explicit binding from a string.  This binding will allow
 * the client (the Control Program) to talk to the Time Service.
 *
 */

static int InitClientRPC (
    rpc_binding_handle_t  *binding_hp, 
    char * hostname
)
{
    unsigned32  	 rpcStatus = rpc_s_ok , secStatus = rpc_s_ok ;
    int			 msgStatus;
    rpc_if_handle_t      serverInterface;
    rpc_if_id_t          serverInterfaceID;
    rpc_ep_inq_handle_t  epContext;
    dce_error_string_t	 dceErrorMsg;


    /* if our requested host is our local host */
    if (strcmp(hostname, dtscp_g_local_hostname) == 0)
    {
	rpc_binding_vector_t * bvec_p = 0 ;
	int rv = 0 ;
	
	do 
	{
	    dts_read_bindings( &bvec_p, &rpcStatus);
	    if (rpcStatus != error_status_ok)
	    {
		rv = 1 ;
		fprintf(stderr, 
			"dtscp: can't read string bindings for local dtsd from bindings file: %s\n", 
			(rpcStatus == dts_s_binding_file_read_error) ? 
			strerror( errno ) : dce_error_text(rpcStatus));
	    }
	} while(0);

	if (rv)
	    return FALSE ;

	if (bvec_p -> count > 0) 
	    *binding_hp = bvec_p -> binding_h [ 0 ] ;
    }
#ifdef FIRST_TRY 
	error_status_t st = rpc_s_ok ;
	int i ;
	
	for (i = 0 ; protseq_select_all[i] != 0 ; i++) 
	{
	    rpc_string_binding_compose ((unsigned_char_p_t)NULL,       
					(unsigned_char_p_t)protseq_select_all[i],
					(unsigned_char_p_t)hostname,
					(unsigned_char_p_t)NULL,       
					(unsigned_char_p_t)NULL,       
					(&strBindName),                
					(&rpcStatus));                    
	    if (rpcStatus != rpc_s_ok) 
		continue;

	    if ( rpc_binding_from_string_binding( strBindName, 
						 binding_hp,
						 &rpcStatus), 
		rpcStatus != rpc_s_ok)
		continue;

	    rpc_string_free( &strBindName, &st) ;

	    if ( rpc_ep_resolve_binding( *binding_hp,
				      time_control_v2_0_c_ifspec,
				      &rpcStatus), 
		rpcStatus == rpc_s_ok) /* got one */
		break;
	} /* for */

    }
#endif 
    else 
    {
	protseq_select_vector = protseq_select_all ;
	dts_dtscp_select_nsi_binding( hostname, 
				     time_control_v2_0_c_ifspec, 
				     /* dts_dtscp_choose_dts_binding, */
				     dts_dtscp_choose_first_binding,
				     binding_hp,
				     &rpcStatus) ;
    }
    
    if (rpcStatus !=  rpc_s_ok)
    {
	dce_error_inq_text (dts_s_cp_init_rpc, dceErrorMsg, &msgStatus);
	TRIM_MSG_TRAILER (dceErrorMsg);
	printf ("%s\n", (char *)dceErrorMsg);
	dce_error_inq_text(rpcStatus, dceErrorMsg, &msgStatus);
	printf ("%s\n", (char *)dceErrorMsg);
	return (FALSE);
    }


#if CONFIG_SECURITY

    /*
     * Obtain the principal name of the time service daemon running on 
     * this node.  This is done by calling rpc_mgmt_inq_server_princ_name.
     * However, because this is a partially-bound handle and there is no
     * object UUID associated with it, we need to resolve the binding to 
     * be completed bounded.  We query the endpoint map database on the
     * node specified by the binding handle (which is this node, although
     * it could potentially be a remote node whose IP address is in the
     * string), to return the endpoint.  The search is based on the Control
     * Program Interface registered by the dts daemon.
     */

    serverInterface = time_control_v2_0_c_ifspec;
    rpc_if_inq_id (serverInterface, &serverInterfaceID, &rpcStatus);

    rpc_mgmt_ep_elt_inq_begin (*binding_hp,
			       rpc_c_ep_match_by_if,
			       &serverInterfaceID,
			       rpc_c_vers_all,
			       NULL,
			       &epContext,
			       &rpcStatus);
    if(rpcStatus == rpc_s_ok)
    {
	rpc_mgmt_ep_elt_inq_next (epContext, 
				  &serverInterfaceID, 
				  binding_hp,
				  NULL,
				  NULL,
				  &rpcStatus);
#if defined(DCE_DTS_DEBUG)
	if(rpcStatus == rpc_s_ok)
	    displayBinding(*binding_hp, "fully bound handle");
#endif /* DCE_DTS_DEBUG */

	rpc_mgmt_ep_elt_inq_done (&epContext, &rpcStatus);
    }

    /*
     * Get the time service principal by making an rpc management call to
     * the server.  This is done implicitely by RPC.
     */

    rpc_mgmt_inq_server_princ_name (*binding_hp,
			       	    rpc_c_authn_dce_private,
				    &dtsPrincipalName,
				    &rpcStatus);
    if (rpcStatus != rpc_s_ok)
    {
	dce_error_inq_text (dts_s_cp_init_rpc, dceErrorMsg, &msgStatus);
	TRIM_MSG_TRAILER (dceErrorMsg);
	printf ("%s\n", (char *)dceErrorMsg);
	dce_error_inq_text(rpcStatus, dceErrorMsg, &msgStatus);
	printf ("\t%s\n", (char *)dceErrorMsg);
	return (FALSE);
    }

#if defined(DCE_DTS_DEBUG)
    fprintf(stderr, "dts principal contacted: %s\n", dtsPrincipalName);
#endif



    /* Get the default context and authentication info into the binding
     * handle.  If we can't, keep going anyway, since an unauthenticated
     * user could be allowed to perform SHOW commands.
     */

    sec_login_get_current_context (&loginContext_h,
				   &secStatus);

    if (secStatus != error_status_ok)
    {
	dce_error_inq_text (dts_s_mgt_unauthn, dceErrorMsg, &msgStatus);
	TRIM_MSG_TRAILER (dceErrorMsg);
	printf ("%s\n", (char *)dceErrorMsg);
	dce_error_inq_text(secStatus, dceErrorMsg, &msgStatus);
	printf ("\t%s\n", (char *)dceErrorMsg);
	rpc_string_free (&dtsPrincipalName, &rpcStatus);
	return (TRUE);
    }

    rpc_binding_set_auth_info (*binding_hp,
			       dtsPrincipalName, 
			       rpc_c_authn_level_default,
			       rpc_c_authn_dce_private,
			       (rpc_auth_identity_handle_t)
			       loginContext_h,
			       rpc_c_authz_dce,
			       &rpcStatus);
    if (rpcStatus !=  rpc_s_ok)
    {
	dce_error_inq_text (dts_s_mgt_unauthn, dceErrorMsg, &msgStatus);
	TRIM_MSG_TRAILER (dceErrorMsg);
	printf ("%s\n", (char *)dceErrorMsg);
	dce_error_inq_text(rpcStatus, dceErrorMsg, &msgStatus);
	printf ("\t%s\n", (char *)dceErrorMsg);
	rpc_string_free (&dtsPrincipalName, &rpcStatus);
	return (TRUE);
    }


#endif /* CONFIG_SECURITY */
    
    return(TRUE);

} /* end InitClientRPC */


/*
 *
 * I N I T _ C O U N T E R _ D I S P L A Y
 *
 * Initialize the counters to display
 *
 */

void InitCounterDisplay(void)
{
    int i;

    for (i=0; i<K_NUM_COUNTERS; i++)
	IsClerkCounter[i] = 1;
    IsClerkCounter[K_FAULTY_SERVER_CNTR] = 0;
    IsClerkCounter[K_DIFFERENT_EPOCH_CNTR] = 0;
    IsClerkCounter[K_NO_GLOBAL_CNTR] = 0;
    IsClerkCounter[K_EPOCH_DECLARATION_CNTR] = 0;
    IsClerkCounter[K_UPDATE_COMPLETED_CNTR] = 0;
    IsClerkCounter[K_TIME_PROVIDER_CNTR] = 0;
    IsClerkCounter[K_NOT_RESPONDING_CNTR] = 0;
    IsClerkCounter[K_SERV_NOT_TSG_MEMBER_CNTR] = 0;
}


/*
 * P A R S E _ C O M M A N D
 *
 * Call the parser to parse a user command and interpret the return
 * value.
 *
 */
static int ParseCommand (void)
{
    int			prStatus;
    int                 msgStatus;
    dce_error_string_t	dceErrorMsg;
    int			pos;


    /*
     * Parse the command
     */
    prStatus = pr_command (&prsContext);


    /*
     * Check the command status.  If an error occurred, display an appropriate
     * error message.  If the error is fatal, or the user typed an EOF, set
     * allDone to TRUE so we exit.
     */

    switch (prStatus)
    {
    case RET_SUCCESS:
	return (TRUE);

    case RET_EOF:			/* CTRL-Z */
	allDone = TRUE;
	return (TRUE);
	
    case RET_ERROR:
	dce_error_inq_text (dts_s_bad_command, dceErrorMsg, &msgStatus);
	TRIM_MSG_TRAILER (dceErrorMsg);
	printf ("%s\n", (char *)dceErrorMsg);
	printf ("\n%s\n", cmdBuf);
	for (pos = prsContext.cur - cmdBuf; pos > 0; pos--)
	{
	    (void) printf (" ");
	}
	(void) printf (PTR);
	(void) printf ("\n");
	break;

    case RET_FILE:
	dce_error_inq_text (dts_s_prsfile_format, dceErrorMsg, &msgStatus);
	printf ((char *)dceErrorMsg, prsBinFile);
	printf ("\n");
	allDone = TRUE;
	break;
	
    case RET_QUIT:
	dce_error_inq_text (dts_s_prsact_fatal, dceErrorMsg, &msgStatus);
	TRIM_MSG_TRAILER (dceErrorMsg);
	printf ("%s\n", (char *)dceErrorMsg);
	printf ("\n%s\n", cmdBuf);
	allDone = TRUE;
	break;
	
    default:
	dce_error_inq_text (dts_s_prs_unknown, dceErrorMsg, &msgStatus);
	printf ("%s\n", (char *)dceErrorMsg);
	allDone = TRUE;
	break;
    }

    return (FALSE);

} /* end of routine ParseCommand */



/*
 * S E N D _ R P C _ R E Q U E S T
 *
 * Send a directive to the Time Service.
 * Each directive generates a separate RPC call to the server.
 *
 */

static int SendRPCrequest (rpc_binding_handle_t  bind_h)
{

    StatusBlk_t     statusBlk;
    error_status_t  comStatus, rpcStatus;
    int             status;
    idl_long_int    maxTservers = MAX_TSERVERS;
    dce_error_string_t   dceErrorMsg;
    CtlFunc 	    the_ctl_func ;

    /*
     * Clear statusBlk before sending it down to RPC.
     */
    statusBlk.status = 0;
    statusBlk.sysStatus = 0;

    /*
     * Depending on the directive, invoke the appropriate Remote
     * Procedure Call.
     */

    switch (ctlFunc)
    {
	case CtlCreate: CreateCmd (bind_h, &timeServType,
				   &statusBlk, &comStatus);
	                break;

	case CtlDelete: DeleteCmd (bind_h, 
				   &statusBlk, &comStatus);
	                break;


	case CtlEnable: EnableCmd (bind_h, &clockSet,
				   &statusBlk, &comStatus);
	                break;

	case CtlDisable: DisableCmd (bind_h,
				   &statusBlk, &comStatus);
	                 break;

	case CtlUpdate:  UpdateCmd (bind_h, &timeVal,
				   &statusBlk, &comStatus);
	                 break;

	case CtlChange:  ChangeCmd (bind_h, &changeDir,
				   &statusBlk, &comStatus);
	                 break;

	case CtlSynch:   SynchronizeCmd (bind_h, &clockSet,
					&statusBlk, &comStatus);
	                 break;

	case CtlAdver:   AdvertiseCmd (bind_h, &statusBlk, &comStatus);
	                 break;

	case CtlUnadver: UnadvertiseCmd (bind_h, &statusBlk, &comStatus);
	                 break;


	case CtlSetDef:  SetDefaultCmd (bind_h, &setAttr,
				       &statusBlk, &comStatus);
	                 break;

	case CtlSet:     setAttrVal.attrValue.Data.type = 
	                      GetAttrType (setAttrVal.setAttr.attribute);
	                 SetAttrCmd (bind_h, &setAttrVal,
				     &statusBlk, &comStatus);
	                 break;


	case CtlShow:
	                 attribute.Data.type = GetAttrType (shAttribute);
	                 ShowAttrCmd (bind_h, &shAttribute, &attribute,
				     &statusBlk, &comStatus);
	                 break;

	case CtlShowChar: ShowAllCharsCmd (bind_h, &showAllChar,
					   &statusBlk, &comStatus);
	                 break;

	case CtlShowStatus: ShowAllStatusCmd (bind_h, &showAllStatus,
					      &statusBlk, &comStatus);
	                 break;


	case CtlShowCntrs: ShowAllCntrsCmd (bind_h, &showAllCntrs,
					    &statusBlk, &comStatus);
	                 break;


	case CtlShowLservers: 
	    the_ctl_func = CtlShowLservers;
	    ShowLocServersCmd (bind_h,
			       &maxTservers,
			       localServers,
			       &serverCount,
			       the_ctl_func,
			       &statusBlk,
			       &comStatus);
	                 break;

	case CtlShowGservers: 
	    the_ctl_func = CtlShowGservers ;
	    ShowGblServersCmd (bind_h,
			       &maxTservers,
			       globalServers,
			       &serverCount,
			       the_ctl_func,
			       &statusBlk,
			       &comStatus);
	    break;

	default:
	    break;
    }



    if (comStatus != rpc_s_ok)
    {
	if (comStatus == rpc_s_comm_failure)
	    rpc_binding_reset (bind_h, &rpcStatus);
	
	dce_error_inq_text (dts_s_dtsd_comm, dceErrorMsg, &status);
	TRIM_MSG_TRAILER (dceErrorMsg);
	fprintf (stderr, "%s\n", (char *)dceErrorMsg);
	dce_error_inq_text (comStatus, dceErrorMsg, &status);
	fprintf (stderr, "%s\n", (char *)dceErrorMsg);
	return (CP_ERROR);
    }

    /*
     * Check the status block received back from the TS.
     */
    if (IsError (statusBlk))
    {
	dce_error_inq_text (dts_s_cp_error, dceErrorMsg, &status);
	TRIM_MSG_TRAILER (dceErrorMsg);
	fprintf (stderr, "%s\n", (char *)dceErrorMsg);
	dce_error_inq_text (statusBlk.sysStatus, dceErrorMsg, &status);
	fprintf(stderr, "\t%s\n", (char *)dceErrorMsg);
	return(CP_ERROR);
    }
   
    return(CP_SUCCESS);


}  /* end of SendRPCrequest */

/*
 *
 * G E T _A T T R _ T Y P E
 *
 * return the data type of the attribute.
 *
 */

SetShow_t GetAttrType (long int attrNum)
{

    SetShow_t dataType;

    dataType = VT_Decimal;   /* quiet compiler warnings */

    switch ((int)attrNum)
    {
	case K_CURRENT_TIME_ATTR:
	case K_LAST_SYNCH_ATTR:
	case K_CREATION_TIME_ATTR:
	case K_NEXT_TDF_ATTR:
	                        dataType = VT_Bintime;
				break;

	case K_CHECK_INTERVAL_ATTR :
	case K_ERROR_TOLLERANCE_ATTR :
	case K_TIME_DIFF_ATTR :
	case K_MAXIMUM_INACCURACY_ATTR :
	case K_LAN_TIMEOUT_ATTR :
	case K_WAN_TIMEOUT_ATTR :
	case K_SYNCH_ATTR :            
	                        dataType = VT_Reltime;
				break;

	case K_EPOCH_NUMBER_ATTR:
	case K_SERVERS_REQUIRED_ATTR:
	case K_QUERY_ATTEMPTS_ATTR:
	case K_CLOCK_ADJUSTMENT_ATTR:
	case K_CLOCK_DRIFT_ATTR:
	case K_STATE_ATTR:
	case K_VARIANT_ATTR:
	case K_CLOCK_RESOLUTION_ATTR:       
 	    dataType = VT_Decimal ;
	    break;
				
        case K_LOCAL_FAULT_ATTR: 	/* local times not intersect*/
	case K_FAULTY_SERVER_ATTR:      /* server time not intersect*/
	case K_DIFFERENT_EPOCH_ATTR:    /* difference epochs */
	case K_TOO_FEW_ATTR:            /* too few servers */
	case K_PROTOCOL_VERSION_ATTR:   /* Protocol mismatch */
	case K_TIME_REPS_ATTR:          /* Time Rep Mismatch */
	case K_NO_GLOBAL_ATTR:          /* No Global Servers Detected*/
	case K_NOT_RESPONDING_ATTR:     /* Servers not responding */
        case K_CLOCK_SET_ATTR:          /* Clock Settings */
	case K_EPOCH_DECLARATION_ATTR:  /* Epoch declaration */
        case K_TIME_DIFFS_ATTR:         /* Local Time differential */
        case K_SYSTEM_ERROR_ATTR:       /* System Error */
	case K_SYNCH_COMPLETED_ATTR:    /* Synch completed */
	case K_UPDATE_COMPLETED_ATTR:   /* Updates initiated */
	case K_STARTUP_COMPLETED_ATTR:  /* Enable Directives */
	case K_SHUTDOWN_COMPLETED_ATTR: /* Disable Directives Completed */
	case K_INSUFFICIENT_RESOURCE_ATTR: /* Insufficient Resources */
	case K_TIME_PROVIDER_ATTR :      /* Time Provider Failures */
	case K_TIME_PROVIDER_TIMEOUT_ATTR: /* Time Provider Timeout */
	case K_SERV_NOT_TSG_MEMBER_ATTR :  /* Servers not in groups */
	case K_LOC_SERV_NOT_TSG_MEMBER_ATTR : /* Local server not in group */
	    dataType = VT_Hyper ;
  	    break;
			
	case K_DECDTS_VERSION_ATTR:
	case K_TIME_REP_ATTR:

            dataType = VT_String;
            break;

	case K_SERVICE_TRACE_ATTR:
	case K_COMM_TRACE_ATTR:
	case K_SYNCH_TRACE_ATTR:
	case K_ARITH_TRACE_ATTR:
	case K_TP_PRESENT_ATTR:
	case K_AUTO_TDF_ATTR:
	                        dataType = VT_Boolean;
				break;

	case K_BACKUP_COURIER_ROLE_ATTR:
	case K_COURIER_ROLE_ATTR:
	                        dataType = VT_CourierRole;
				break;

	case K_SERVER_PRINCIPAL_NAME:
	case K_SERVER_ENTRY_NAME:
	case K_SERVER_GROUP_NAME:
	                        dataType = VT_String;
				break;

	case K_UID_ATTR:
				dataType = VT_UUID;
				break;
        default:
		 dataType = VT_String;
		 break;
			    
    }

    return (dataType);

}


/*
 * D I S P L A Y _ C M D _ O U T P U T
 *
 * interpret the response message sent by the server, and display its
 * contents based upon its function code.
 *
 */
void DisplayCmdOutput( CtlFunc func)
{
    int i, attr;

    switch ( func ) {
	case CtlShow:
	    PrintAttrText(shAttribute);
	    switch ((int)shAttribute) {
		case K_VARIANT_ATTR:
		PrintAttrValue(K_VARIANT_ATTR, K_TYPE_VARIANT, 
			       (VOID *)&attribute.Data.setData);
		break;

		case K_STATE_ATTR:
		PrintAttrValue(K_STATE_ATTR, K_TYPE_STATE, 
			       (VOID *)&attribute.Data.setData);
		break;

		case K_BACKUP_COURIER_ROLE_ATTR:
		case K_COURIER_ROLE_ATTR:
		PrintAttrValue(K_BACKUP_COURIER_ROLE_ATTR, K_TYPE_COURIER_ROLE,
			       (VOID *)&attribute.Data.setData);
		break;

		default:
	        PrintAttrValue(shAttribute, attribute.attributeType, 
			       (VOID *)&attribute.Data.setData);
		break;
	    }
	
	    break;

	case CtlShowChar:
	    PrintShowChar(&showAllChar);
	    break;

	case CtlShowStatus:

	    if (showAllStatus.variant == (long int)VTserver)
	    {
		PrintAttrText(K_UID_ATTR);
		PrintAttrValue(K_UID_ATTR,
			       K_TYPE_UID,
			       (VOID *)&showAllStatus.uid);
	    }

	    PrintAttrText(K_LAST_SYNCH_ATTR);
	    PrintAttrValue(K_LAST_SYNCH_ATTR,
			   K_TYPE_BINABSTIME, 
			   (VOID *)&showAllStatus.lastSynch);
	    PrintAttrText(K_STATE_ATTR);
	    PrintAttrValue(K_STATE_ATTR,
			   K_TYPE_STATE,
			   (VOID *)&showAllStatus.state);
	    break;

	case CtlShowCntrs:
	    {
		
		PrintAttrText(K_CREATION_TIME_ATTR);
		PrintAttrValue(K_CREATION_TIME_ATTR,
			       K_TYPE_BINABSTIME, 
			       (VOID *)&showAllCntrs.creationTime);
	    
		for (i = 0; i < K_NUM_COUNTERS; i++)
		{
		    if ((showAllCntrs.variant==(long int)VTserver)
			|| IsClerkCounter[i])
		    {
/* change type from Bits64 to idl_uhyper_int for aCounter	
 * so it matches with showAllCntrs.counters[].			
 * 
 * No. we must do an explicit conversion, since we can't guarantee 
 * that the fields of a Bit64 and an idl_hyper_int are defined 
 * the same across all platforms. We delay this, however, until 
 * the actual conversion routines.
 */

			idl_hyper_int aCounter ;
			attr = i+K_FIRST_COUNTER;
			
			if (attr != K_TIME_DIFFS_ATTR &&
			    attr != K_INVALID_MESSAGE_ATTR)
			{
			    PrintAttrText(i + K_FIRST_COUNTER );

			    aCounter = showAllCntrs.counters[i];
			    PrintAttrValue((i + K_FIRST_COUNTER),
					   K_TYPE_COUNTER64,
			    (VOID *)&aCounter);
			}
		    }
		} /* for */
	    } /* scope */
	    break;
	case CtlShowLservers:
	    PrintShowServers (serverCount, localServers);
	    break;
        case CtlShowGservers:
	    PrintShowServers (serverCount, globalServers);
            break;
        default:
	    break;
	}
	   
}


/*
 *
 * I N I T _ L O O P _ V A R I A B L E S
 *
 * Initialize the global state variables.  This routine is executed at the
 * top of the main loop.
 *
 */
void InitLoopVariables(void)
{
    /*
     * Initialize globals used for each command execution.  Zeroing
     * the param field implies that all params which are strings are
     * automatically null-terminated.
     */
    cmdBuf[0] = '\0';
    ctlFunc = (CtlFunc) 0;	/* there is no function 0. */
    (void) memset ((VOID *)&timeServType, 0, sizeof( long int ));
    (void) memset ((VOID *)&clockSet, 0, sizeof( long int ));
    (void) memset ((VOID *)&serverCount, 0, sizeof( long int ));
    (void) memset ((VOID *)&timeVal, 0, sizeof( utc_t ));
    (void) memset ((VOID *)&changeDir, 0, sizeof( Change_t ));
    (void) memset ((VOID *)&setAttr, 0, sizeof( CtlSet_t ));
    (void) memset ((VOID *)&setAttrVal, 0, sizeof( CtlSetAttr_t ));
    (void) memset ((VOID *)&attribute, 0, sizeof( AttrVal_t ));
    (void) memset ((VOID *)&shAttribute, 0, sizeof( long int ));
    (void) memset ((VOID *)&showAllChar, 0, sizeof( CtlShowAllChar ));
    (void) memset ((VOID *)&showAllStatus, 0, sizeof( CtlShowAllStatus ));
    (void) memset ((VOID *)&showAllCntrs, 0, sizeof( CtlShowAllCntrs));
    (void) memset ((VOID *)localServers, 0, sizeof( localServers ));
    (void) memset ((VOID *)globalServers, 0, sizeof( globalServers ));

    sendCtlMsg = FALSE;
    displayOutput = FALSE;
    userBufferSet = FALSE;
    userBufferSize= 0;
}

/*
 * A C T _ H E L P
 *
 * Provide help to a user
 *
 */
int actHelp(struct prs *pcbp)
{
    switch (prsContext.av1) {
	case 0: /* just help */
	(void)printf("\nHELP\n\n");
	(void)printf("\tThe HELP directive invokes the DTS HELP service to\n");
	(void)printf("\tdisplay information about a DTS directive.\n");
	(void)printf("\tIn response to a DTS prompt your can ask for help\n");
	(void)printf("\tabout a DTS directive.\n\n");
	(void)printf("\t\tdtscp> HELP CREATE\n");
	(void)printf("\t\tdtscp> HELP DELETE\n");
	(void)printf("\t\tdtscp> HELP ENABLE\n");
	(void)printf("\t\tdtscp> HELP DISABLE\n");
	(void)printf("\t\tdtscp> HELP UPDATE\n");
	(void)printf("\t\tdtscp> HELP SYNCHRONIZE\n");
	(void)printf("\t\tdtscp> HELP CHANGE\n");
	(void)printf("\t\tdtscp> HELP ADVERTISE\n");
	(void)printf("\t\tdtscp> HELP UNADVERTISE\n");
	(void)printf("\t\tdtscp> HELP SET\n");
	(void)printf("\t\tdtscp> HELP SHOW\n");
	(void)printf("\n\n");
	break;
	
	
	

	
	case 1:
	(void)printf("\nHELP CREATE\n\n");
	(void)printf("\tThe CREATE directive initializes the time service\n");
	(void)printf("\tprocess as a server or as a clerk.\n");
	(void)printf("\n\t\tdtscp>CREATE  [TYPE [SERVER|CLERK]]\n");
	(void)printf("\n\n");
	break;
	
	
	
	
	case 2:
	(void)printf("\nHELP DELETE\n\n");
	(void)printf("\tThe DELETE directive is issued to exit a disabled\n");
	(void)printf("\tDTS process.\n");
	(void)printf("\n\t\tdtscp>DELETE \n");
	(void)printf("\n\n");
	break;
	
	
	
	
	case 3:
	(void)printf("\nHELP ENABLE\n\n");
	(void)printf("\tThe ENABLE directive causes a disabled DTS process\n");
	(void)printf("\tto begin synchronizing.  A DTS process becomes disabled\n");
	(void)printf("\tas the result of a CREATE or a DISABLE directive.\n");
	(void)printf("\tThe SET CLOCK argument determines whether or not\n");
	(void)printf("\the result of the first synchronization is used to\n");
	(void)printf("\tnon-monotonically set the local clock.  The default\n");
	(void)printf("\tis for the clock not to be set.\n");
	(void)printf("\n\t\tdtscp>ENABLE  [SET [CLOCK] [TRUE|FALSE]]\n");
	(void)printf("\n\n");
	break;
	
	
	
	
	case 4:
	(void)printf("\nHELP DISABLE\n\n");
	(void)printf("\tThe DISABLE directive causes the time service to stop\n");
	(void)printf("\tsynchronize.  A disabled service can\n");
	(void)printf("\tbe enabled by using the ENABLE directive\n");
	(void)printf("\n\t\tdtscp>DISABLE \n\n\n");
	break;

	
	
	
	case 5:
	(void)printf("\nHELP UPDATE\n\n");
	(void)printf("\tThe UPDATE directive causes the time service to correct\n");
	(void)printf("\tthe local clock by drifting its value to the specified\n");
	(void)printf("\ttime.  UPDATE allows the user to perform a manual\n");
	(void)printf("\tsynchronization.  The specified time must be included\n");
	(void)printf("\tin the local clock's inaccuracy interval, and can not\n");
	(void)printf("\tspecify an inaccuracy of less than one second\n");
	(void)printf("\tThis directive is supported by servers only.\n");
	(void)printf("\n\t\tdtscp>UPDATE TIME [ABSOLUTE TIME]\n\n\n");
	break;
	
	
	
	
	case 6:
	(void)printf("\nHELP SYNCHRONIZE\n\n");
	(void)printf("\tThe SYNCHRONIZE directive requests an immediate synchronization.\n");
	(void)printf("\tThe time service can not initiate if the service is\n");
	(void)printf("\tcurrently synchronizing, disabled, or servicing an\n");
	(void)printf("\tupdate directive. The SET CLOCK argument determines\n");
	(void)printf("\tif the result of the synchronization will be used to\n");
	(void)printf("\tnon-monotonically set the local clock.  The default\n");
	(void)printf("\taction is to drift set the local clock\n");
	(void)printf("\n\t\tdtscp>SYNCHRONIZE  [SET [CLOCK] [TRUE|FALSE]]\n");
	(void)printf("\n\n");
	break;
	
	
	
	
	case 7:
	(void)printf("\nHELP CHANGE\n\n");
	(void)printf("\nThe CHANGE directive has two instances\n") ;
	(void)printf("\tThe CHANGE EPOCH directive changes the current epoch and\n");
	(void)printf("\toptionally sets the local clock.  The clock is set\n");
	(void)printf("\tnon-monotonically.\n");
	(void)printf("\n\t\tdtscp>CHANGE EPOCH {0..255} [TIME {ABSOLUTE TIME}]\n\n\n");
	(void)printf("\tThe CHANGE HOST directive resets the dtscp program\n");
	(void)printf("and opens a connection to the DTS Server on the remote host\n");
	(void)printf("\n\t\tdtscp>CHANGE HOST [TO] {hostname}\n");
	(void)printf("\n\n");	
	break;
	
	case 8:
	(void)printf("\nHELP ADVERTISE\n\n");
	(void)printf("\tThe ADVERTISE directive configures this DTS\n"); 
	(void)printf("\tTime Server as a global server.\n");

	break;
	
	case 9:
	(void)printf("\nHELP UNADVERTISE\n\n");
	(void)printf("\tThe UNADVERTISE directive removes this DTS Time\n");
	(void)printf("\tServer from the Cell Profile, thus making it\n");
	(void)printf("\tno longer be a global server\n");
	break;
	
	case 10:
	(void)printf("\nHELP SET\n\n");
	(void)printf("\tThe SET directive sets a single DTS characteristic\n");
	(void)printf("\tattribute to a specified value.\n\n");
	(void)printf("\t\tdtscp>SET  {ATTRIBUTE}\n");
	(void)printf("\t\t\tATTRIBUTES\n");
	(void)printf("\t\t\t\tCHECK INTERVAL\n");
	(void)printf("\t\t\t\tERROR TOLERANCE\n");
	(void)printf("\t\t\t\tMAXIMUM INACCURACY\n");
	(void)printf("\t\t\t\tSERVERS REQUIRED\n");
	(void)printf("\t\t\t\tQUERY ATTEMPTS\n");
	(void)printf("\t\t\t\tLOCAL SET TIMEOUT\n");
	(void)printf("\t\t\t\tGLOBAL SET TIMEOUT\n");
	(void)printf("\t\t\t\tSERVERS PRINCIPAL NAME\n");
	(void)printf("\t\t\t\tSERVERS ENTRY NAME\n");
	(void)printf("\t\t\t\tSERVERS GROUP NAME\n");
	(void)printf("\t\t\t\tSYNCHRONIZATION HOLD DOWN\n");
	(void)printf("\t\t\t\tCOURIER ROLE\n");
	(void)printf("\n\n");
	break;
	
	case 11:
	(void)printf("\nHELP SHOW\n\n");
	(void)printf("\tThe SHOW directive displays information about the\n");
	(void)printf("\ttime service\n\n");
	(void)printf("\t\tdtscp>SHOW  {SHOW ATTRIBUTE}\n");
	(void)printf("\t\t\tATTRIBUTES\n");
	(void)printf("\t\t\t\tALL STATUS\n");
	(void)printf("\t\t\t\tALL COUNTERS\n");
	(void)printf("\t\t\t\tALL CHARACTERISTICS\n");
	(void)printf("\t\t\t\tALL\n");
	(void)printf("\t\t\t\tCHECK INTERVAL\n");
	(void)printf("\t\t\t\tEPOCH NUMBER\n");
	(void)printf("\t\t\t\tERROR TOLERANCE\n");
	(void)printf("\t\t\t\tLOCAL TIME DIFFERENTIAL FACTOR\n");
	(void)printf("\t\t\t\tMAXIMUM INACCURACY\n");
	(void)printf("\t\t\t\tSERVERS REQUIRED\n");
	(void)printf("\t\t\t\tQUERY ATTEMPTS\n");
	(void)printf("\t\t\t\tLOCAL SET TIMEOUT\n");
	(VOID)printf("\t\t\t\tGLOBAL SET TIMEOUT\n");
	(void)printf("\t\t\t\tSYNCHRONIZATION HOLD DOWN\n");
	(void)printf("\t\t\t\tTYPE\n");
	(void)printf("\t\t\t\tCOURIER ROLE\n");
	(void)printf("\t\t\t\tACTING COURIER ROLE\n");
	(void)printf("\t\t\t\tCLOCK ADJUSTMENT RATE\n");
	(void)printf("\t\t\t\tCLOCK RESOLUTION\n");              
	(void)printf("\t\t\t\tMAXIMUM CLOCK DRIFT RATE\n");
	(void)printf("\t\t\t\tSERVERS PRINCIPAL NAME\n");
	(void)printf("\t\t\t\tSERVERS ENTRY NAME\n");
	(void)printf("\t\t\t\tSERVERS GROUP NAME\n");

	(void)printf("\t\t\t\tDTS VERSION\n");
	(void)printf("\t\t\t\tTIME REPRESENTATION VERSION\n");

#ifdef DCE_DTS_DEBUG
	(void)printf("\t\t\t\tSERVICE TRACE\n");
	(void)printf("\t\t\t\tCOMMUNICATION TRACE\n");
	(void)printf("\t\t\t\tSYNCHRONIZATION TRACE\n");
	(void)printf("\t\t\t\tARITHMETIC TRACE\n");
#endif
	(void)printf("\t\t\t\tCURRENT TIME\n");
	(void)printf("\t\t\t\tUID\n");
	(void)printf("\t\t\t\tLAST SYNCHRONIZATION\n");
	(void)printf("\t\t\t\tSTATE\n");
	(void)printf("\t\t\t\tCREATION TIME\n");
	(void)printf("\t\t\t\tLOCAL TIMES NOT INTERSECTING\n");
	(void)printf("\t\t\t\tSERVER TIMES NOT INTERSECTING\n");
	(void)printf("\t\t\t\tDIFFERENT EPOCHS DETECTED\n");
	(void)printf("\t\t\t\tTIME REPRESENTATION MISMATCHES DETECTED\n");
	(void)printf("\t\t\t\tTOO FEW SERVERS DETECTED\n");
	(void)printf("\t\t\t\tNO GLOBAL SERVERS DETECTED\n");
	(void)printf("\t\t\t\tSERVERS NOT RESPONDING\n");
	(void)printf("\t\t\t\tCLOCK SETTINGS\n");
	(void)printf("\t\t\t\tEPOCH CHANGES COMPLETED\n");
	(void)printf("\t\t\t\tSYSTEM ERRORS DETECTED\n");
	(void)printf("\t\t\t\tSYNCHRONIZATIONS COMPLETED\n");
	(void)printf("\t\t\t\tUPDATES INITIATED\n");
	(void)printf("\t\t\t\tENABLE DIRECTIVES COMPLETED\n");
	(void)printf("\t\t\t\tDISABLE DIRECTIVES COMPLETED\n");
	(void)printf("\t\t\t\tINSUFFICIENT RESOURCES DETECTED\n");
	(void)printf("\t\t\t\tTIME PROVIDER FAILURES DETECTED\n");
	(void)printf("\t\t\t\tTIME PROVIDER TIMEOUTS DETECTED\n");
	(void)printf("\t\t\t\tLOCAL SERVER NOT IN GROUP\n");
	(void)printf("\t\t\t\tSERVERS NOT IN GROUP\n");
	(void)printf("\t\t\t\tLOCAL SERVERS\n");
	(void)printf("\t\t\t\tGLOBAL SERVERS\n");
	(void)printf("\n\n");
	break;
	default: 
	(void)fprintf(stderr,"<Internal Error> %d\n",__LINE__);
	break;
    }
    return(ACT_SUCCESS);
}

/*
 *
 * A C T _ C O M M E N T
 *
 * parse off a comment line
 *
 */

int actComment (struct prs *pcbp)
{
    /*
     * The parse sends all the OS comment chars to this action routine.  Make
     * sure the comment character we have is the right one for this OS.
     */
    return (*prsContext.cur == K_OS_COMMENT_CHAR ? ACT_SUCCESS : ACT_ERROR);
}

/*
 * A C T _ C R E A T E
 *
 * Parse a create directive
 *
 */

int actCreate(struct prs *pcbp)
{
    timeServType = prsContext.av1;
    ctlFunc = CtlCreate;
    sendCtlMsg = TRUE;
    return ( ACT_SUCCESS );
}

/*
 * A C T _ E X I T
 *
 * Parse an exit or a quit directive
 *
 */
int actExit (struct prs *pcbp)
{
    allDone = TRUE;
    return (ACT_SUCCESS);
}


/*
 * A C T _ E N A B L E
 *
 * Parse an enable directive
 *
 */
int actEnable (struct prs *pcbp)
{
    clockSet = prsContext.av1;
    ctlFunc = CtlEnable;
    sendCtlMsg = TRUE;
    return (ACT_SUCCESS);
}

/* 
 * A C T _ U P D A T E
 *
 * Parse an update directive
 *
 */
int actUpdate (struct prs *pcbp)
{
    timeVal = userBuffer.setData.userBinTime;
    ctlFunc = CtlUpdate;
    sendCtlMsg = TRUE;
    return (ACT_SUCCESS);
}

/*
 * A C T _ S Y N C H R O N I Z E
 *
 * Parse a synchronize directive
 *
 */

int actSynchronize (struct prs *pcbp)
{
    clockSet = prsContext.av1;
    ctlFunc = CtlSynch;
    sendCtlMsg = TRUE;
    return (ACT_SUCCESS);
}

/*
 *
 * A C T _ C H A N G E_ E P O C H
 *
 * Parse the change epoch directive
 *
 * prsContext = 0 : parsing the epoch number
 * prsContext = 1 : using default time
 * prsContext = 2 : parsing both time and epoch
 *
 */

int actChangeEpoch (struct prs *pcbp)
{
    switch ( prsContext.av1 ) {
	
	case 0 : changeDir.newEpoch = prsContext.num;
	         ctlFunc = CtlChange;
                 sendCtlMsg = FALSE;
	         break;

	case 1 : changeDir.newTimePresent = FALSE;
                 sendCtlMsg = TRUE;
                 break;

        case 2 : changeDir.newTime = userBuffer.setData.userBinTime;
	         changeDir.newTimePresent = TRUE;
                 sendCtlMsg = TRUE;
	         break;
        default:
	         BugCheck();
    }

    return (ACT_SUCCESS);
}

/*
 *
 * A C T _ A D V E R T I S E
 *
 * Parse the advertise directive
 *
 */

int actAdvertise (struct prs *pcbp)
{
    ctlFunc = CtlAdver;
    sendCtlMsg = TRUE;
    return (ACT_SUCCESS);
}

/*
 *
 * A C T _ U N A D V E R T I S E
 *
 * Parse the Unadvertise directive
 *
 */

int actUnadvertise (struct prs *pcbp)
{
    ctlFunc = CtlUnadver;
    sendCtlMsg = TRUE;
    return (ACT_SUCCESS);
}



/*
 *
 * A C T _ S E T
 *
 * Parse the set directive
 *
 * If one of the user buffer global variables is not being used, then
 * use the default value of the specified attribute.
 *
 */

int actSet (struct prs *pcbp)
{

    if (!userBufferSet) {
	setAttr.attribute = prsContext.av1;
	ctlFunc = CtlSetDef;
	setAttr.useDefault = TRUE;
    }
    else {
	ctlFunc = CtlSet;
	setAttrVal.setAttr.useDefault = FALSE;
	setAttrVal.setAttr.attribute = prsContext.av1;
	setAttrVal.attrValue.Data.setData = userBuffer.setData;
	setAttrVal.attrValue.attrSize = userBufferSize;
    }

    sendCtlMsg = TRUE;
    return (ACT_SUCCESS);
}

/*
 *
 * A C T _S H O W
 *
 * parse the show directive
 *
 */

int actShow (pcbp) 
struct prs *pcbp;
{
    displayOutput = TRUE;

    /*
     * The 'ALL' functions are not proper attributes and are only
     * used in the CTL interface.  This is because we do not support
     * wildcarding in DTSSCP/CTL
     */
    switch (prsContext.av1) {
	case K_CTL_SHOW_ALL_COUNTERS:
        ctlFunc = CtlShowCntrs;
	break;
	
	case K_CTL_SHOW_ALL_CHAR:
	ctlFunc = CtlShowChar;
	break;
	
	case K_CTL_SHOW_ALL_STATUS:
	ctlFunc = CtlShowStatus;
	break;
	
	case K_CTL_SHOW_GLOBAL_SERVERS:
	ctlFunc = CtlShowGservers;
	break;
	
	case K_CTL_SHOW_LOCAL_SERVERS:
	ctlFunc = CtlShowLservers;
	break;



        /*
         * If this is being compiled in a non-DECnet, non-IEEE802.3
         * enviroment, make sure that these commands are illegal.
         */
        case K_ADVERTISEMENT_ATTR:
        case K_PROTOCOL_VERSION_ATTR:
        case K_INVALID_MESSAGE_ATTR:
        case K_GLOBAL_DIRECTORY_ATTR:
        case K_DNA_VERSION_ATTR:
        return (ACT_ERROR);

	/* 
	 * Otherwise it must be a real attribute
	 */
	default:
        if (prsContext.av1 < 0 ||    /* min-max attribute ids */
	    prsContext.av1 > K_MAX_ATTR)
	    return(ACT_ERROR);
	else {
	    ctlFunc = CtlShow;
	    shAttribute = prsContext.av1;
	}
	break;
    }

    sendCtlMsg = TRUE;
    return (ACT_SUCCESS);
}


/*
 *
 * A C T _ D I S A B L E
 *
 * Parse the Disable directive
 *
 */

int actDisable (struct prs *pcbp)
{
    ctlFunc = CtlDisable;
    sendCtlMsg = TRUE;
    return (ACT_SUCCESS);
}


/*
 *
 * A C T _ D E L E T E
 *
 * Parse the Delete directive
 *
 */

int actDelete (struct prs *pcbp)
{
    ctlFunc = CtlDelete;
    sendCtlMsg = TRUE;
    return (ACT_SUCCESS);
}

/*
 *
 * A C T _ H O S T 
 *
 * Parse the set host directive
 *
 */

int actSetHost (struct prs *pcbp)
{
    char newhostname[ HOST_NAME_SIZE ];
    rpc_binding_handle_t newbinding_h;
    int rv = ACT_SUCCESS ;
    unsigned32 rpcStatus = rpc_s_ok ;

    /* begin */

    sendCtlMsg = FALSE;


    if (userBufferSet) 
    {
	strncpy( newhostname, 
		(char *)userBuffer.setData.userString.buffer, HOST_NAME_SIZE);

	if (InitClientRPC( &newbinding_h, newhostname)) /* success! */
	{
	    rpc_binding_free( &dts_g_dtscp_binding_h, &rpcStatus) ;
	    dts_g_dtscp_binding_h = newbinding_h ;
	    rv = ACT_SUCCESS ;
	}
    }
    else 
    {
	rv = ACT_ERROR;
    }
    return (rv);
}


/*
 * A C T _ P A R S E _ S T R I N G
 *
 * store the parsed text sting
 *
 */
int actParseString(struct prs *pcbp)
{

    (void)memcpy((VOID *)userBuffer.setData.userString.buffer,
                 (VOID *)prsContext.cur,
		 (size_t)(prsContext.end - prsContext.cur));
    userBuffer.setData.userString.buffer[prsContext.end - prsContext.cur] = '\0';
    userBufferSize = strlen((char *)userBuffer.setData.userString.buffer) + 1;

    userBufferSet = TRUE;
    sendCtlMsg = FALSE;
    return (ACT_SUCCESS);

}

/*
 *
 * A C T _ P A R S E _F U L L
 *
 * Parse a full name
 *
 */
#pragma covcc !instr
int actParseFull(struct prs *pcbp)
{
    return(ACT_ERROR);
}
#pragma covcc instr

/*
 *
 * A C T _ P A R S E _ S I M P L E
 *
 * Parse a simple name
 *
 */
#pragma covcc !instr
int actParseSimple(struct prs *pcbp)
{
    return(ACT_ERROR);
}
#pragma covcc instr
/*
 *
 * A C T _ P A R S E _ D E C I M A L
 *
 * store the parsed decimal
 *
 */

int actParseDecimal(struct prs *pcbp)
{
    userBufferSize = sizeof(userBuffer.setData.userDecimal);
    userBuffer.setData.userDecimal = prsContext.num;
    userBufferSet = TRUE;
    sendCtlMsg = FALSE;
    return (ACT_SUCCESS);
}

/*
 * A C T _P A R S E _ B O O L
 *
 * store the parsed boolean value
 *
 */
int actParseBool (struct prs *pcbp)
{
    switch (prsContext.av1)
    {
	case 0 : 
	case 1 :
		userBuffer.setData.userBoolean = prsContext.av1;
		userBufferSet = TRUE;
		break;
	case 2 :
		userBufferSet = FALSE;
	        break;   /* Defaults */
	default:
		BugCheck();
     }
    
    userBufferSet = TRUE;
    userBufferSize = sizeof(userBuffer.setData.userBoolean);
    sendCtlMsg = FALSE;
    return (ACT_SUCCESS);
}

/*
 *
 * A C T _ P A R S E _ C O U R I E R
 *
 * Store the parsed courier value
 *
 */

int actParseCourier (struct prs *pcbp)
{
    switch (prsContext.av1)
    {
	case 0: 
	case 1:
	case 2: 
	{
	    userBuffer.setData.userCourierRole = prsContext.av1;
	    break;
	}
	default: 
	{
	    BugCheck();
	}
    }
    
    userBufferSet = TRUE;
    userBufferSize = sizeof(userBuffer.setData.userCourierRole);
    sendCtlMsg = FALSE;    
    return (ACT_SUCCESS);
}

/*
 *
 * A C T _ P A R S E _ B R T
 *
 * Convert the parsed ascii string into a binRelTime
 *
 */

int actParseBRT (struct prs *pcbp)
{
char userTimeStr[256];
int  status;

    (void)memcpy((VOID *)userTimeStr,
		 (VOID *)prsContext.cur,
		 (size_t)(prsContext.end - prsContext.cur));
    userTimeStr[prsContext.end - prsContext.cur] = '\0';

    status = utc_mkascreltime(
			   &userBuffer.setData.userRelTime,  /* OUT - new rel */
			   userTimeStr); /* IN  - NULL terminated string */
    if ( status != 0 ) {
	return(ACT_ERROR);
    }
	
			  
    userBufferSet = TRUE;
    userBufferSize = sizeof(utc_t);
    sendCtlMsg = FALSE;
    return (ACT_SUCCESS);

}


/*
 *
 * A C T _ P A R S E _ B A T
 *
 * Convert the parsed ascii string and store it as a utcTime
 *
 */

int actParseBAT (struct prs *pcbp)
{
char userTimeStr[256];
int status;

    (void)memcpy((VOID *)userTimeStr,
		 (VOID *)prsContext.cur,
		 (size_t)(prsContext.end - prsContext.cur));
    userTimeStr[prsContext.end - prsContext.cur] = '\0';

    status = utc_mkasctime(&userBuffer.setData.userBinTime,  /* OUT - new utc */
			  userTimeStr);  /* input - nul term user string */

    if ( status != 0 ) {
	return(ACT_ERROR);
    }
	
    userBufferSet = TRUE;
    userBufferSize = sizeof(utc_t);
    sendCtlMsg = FALSE;
    return (ACT_SUCCESS);
}

/*
 *
 * P R I N T _ A T T R _ T E X T
 *
 * Print out the Lexical form of the passed attribute number
 *
 */

void PrintAttrText( int attrNum)
{

    switch(attrNum) {

	case K_CHECK_INTERVAL_ATTR:
	(void)printf("%-25s","Check Interval" );
	break;

	case K_EPOCH_NUMBER_ATTR:
	(void)printf("%-25s","Epoch Number" );
	break;

	case K_ERROR_TOLLERANCE_ATTR:
	(void)printf("%-25s","Error Tolerance" );
	break;

	case K_TIME_DIFF_ATTR:
	(void)printf("%-25s","Local Time Differential Factor" );
	break;

	case K_MAXIMUM_INACCURACY_ATTR:
	(void)printf("%-25s","Maximum Inaccuracy" );
	break;

	case K_SERVERS_REQUIRED_ATTR:
	(void)printf("%-25s","Servers Required" );
	break;

	case K_QUERY_ATTEMPTS_ATTR:
	(void)printf("%-25s","Query Attempts" );
	break;

	case K_LAN_TIMEOUT_ATTR:
	(void)printf("%-25s","Local Set Timeout" );
	break;

	case K_WAN_TIMEOUT_ATTR:
	(void)printf("%-25s","Global Set Timeout" );
	break;

	case K_SYNCH_ATTR:
	(void)printf("%-25s","Synchronization Hold Down" );
	break;

	case K_VARIANT_ATTR:
	(void)printf("%-25s","Type" );
	break;

	case K_COURIER_ROLE_ATTR:
	(void)printf("%-25s","Courier Role" );
	break;

	case K_BACKUP_COURIER_ROLE_ATTR:
	(void)printf("%-25s","Acting Courier Role" );
	break;

	case K_CLOCK_ADJUSTMENT_ATTR:
	(void)printf("%-25s","Clock Adjustment Rate" );
	break;

	case K_CLOCK_DRIFT_ATTR:
	(void)printf("%-25s","Maximum Clock Drift Rate" );
	break;

	case K_DECDTS_VERSION_ATTR:
	(void)printf("%-25s","DTS Version" );
	break;

	case K_TIME_REP_ATTR:
	(void)printf("%-25s","Time Representation Version" );
	break;

	case K_SERVICE_TRACE_ATTR:
	(void)printf("%-25s","Service Trace" );
	break;

	case K_COMM_TRACE_ATTR:
	(void)printf("%-25s","Communication Trace" );
	break;

	case K_SYNCH_TRACE_ATTR:
	(void)printf("%-25s","Synchronization Trace" );
	break;

	case K_ARITH_TRACE_ATTR:
	(void)printf("%-25s","Arithmetic Trace" );
	break;

	case K_TP_PRESENT_ATTR:
	(void)printf("%-25s","Time Provider Present" );
	break; 

	case K_AUTO_TDF_ATTR:
	(void)printf("%-25s","Automatic TDF Change" );
	break;

	case K_NEXT_TDF_ATTR:
	(void)printf("%-25s","Next TDF Change" );
	break;

	case K_CLOCK_RESOLUTION_ATTR:
	(void)printf("%-25s","Clock Resolution" );
	break;

	case K_SERVER_PRINCIPAL_NAME:
	(void)printf("%-25s","Server Principal Name" );
	break;

	case K_SERVER_ENTRY_NAME:
	(void)printf("%-25s","Server Entry Name" );
	break;

	case K_SERVER_GROUP_NAME:
	(void)printf("%-25s","Server Group Name" );
	break;

	case K_CURRENT_TIME_ATTR:
	(void)printf("%-25s","Current Time" );
	break;

	case K_UID_ATTR:
	(void)printf("%-25s", "UID");
	break;

	case K_LAST_SYNCH_ATTR:
	(void)printf("%-25s","Last Synchronization" );
	break;

	case K_STATE_ATTR:
	(void)printf("%-25s","State" );
	break;

	case K_CREATION_TIME_ATTR:
	(void)printf("%-25s","Creation Time" );
	break;

	case K_LOCAL_FAULT_ATTR:
	(void)printf("%-25s","Local Times Not Intersecting" );
	break;

	case K_FAULTY_SERVER_ATTR:
	(void)printf("%-25s","Server Times Not Intersecting" );
	break;

	case K_DIFFERENT_EPOCH_ATTR:
	(void)printf("%-25s","Different Epochs Detected" );
	break;

	case K_TOO_FEW_ATTR:
	(void)printf("%-25s","Too Few Servers Detected" );
	break;

	case K_PROTOCOL_VERSION_ATTR:
	(void)printf("%-25s","Protocol Mismatches Detected" );
	break;

	case K_TIME_REPS_ATTR:
	(void)printf("%-25s","Time Representation Mismatches Detected" );
	break;

	case K_NO_GLOBAL_ATTR:
	(void)printf("%-25s","No Global Servers Detected" );
	break;

	case K_NOT_RESPONDING_ATTR:
	(void)printf("%-25s","Servers Not Responding" );
	break;

	case K_CLOCK_SET_ATTR :
	    (void)printf("%-25s","Clock Settings" );
	break;

	case K_EPOCH_DECLARATION_ATTR:
	(void)printf("%-25s","Epoch Changes Completed" );
	break;

	case K_SYSTEM_ERROR_ATTR:
	(void)printf("%-25s","System Errors Detected" );
	break;

	case K_SYNCH_COMPLETED_ATTR:
	(void)printf("%-25s","Synchronizations Completed" );
	break;

	case K_UPDATE_COMPLETED_ATTR:
	(void)printf("%-25s","Updates Initiated" );
	break;

	case K_STARTUP_COMPLETED_ATTR:
	(void)printf("%-25s","Enable Directives Completed" );
	break;

	case K_SHUTDOWN_COMPLETED_ATTR:
	(void)printf("%-25s","Disable Directives Completed" );
	break;

	case K_INSUFFICIENT_RESOURCE_ATTR:
	(void)printf("%-25s","Insufficient Resources Detected" );
	break;

	case K_TIME_PROVIDER_ATTR:
	(void)printf("%-25s","Time Provider Failures Detected" );
	break;

	case K_TIME_PROVIDER_TIMEOUT_ATTR:
	(void)printf("%-25s","Time Provider Timeouts Detected" );
	break;

	case K_LOC_SERV_NOT_TSG_MEMBER_ATTR:
	(void)printf("%-25s","Local server not in group" );
	break;

	case K_SERV_NOT_TSG_MEMBER_ATTR:
	(void)printf("%-25s","Servers not in group" );
	break;


	default:
	(void)printf("%-25s","<Unknown Attribute,internal error>");
    }
}

/*
 *
 * P R I N T _ A T T R _ V A L  U  E
 *
 * Print the attribute value.  Its type is given by 'attrType' and
 * its value is passed by the void pointer 'attrValue'
 *
 */

void PrintAttrValue( int attribute, int attrType, VOID *attrValue)
{
    char printStr[1024];
    size_t printStrLen = 1024;
    int letter,v_major,v_minor,v_level;
    String_t *string;
    
    switch ( attrType ) {
	case K_TYPE_BINRELTIME:
	if (utc_ascreltime( printStr, printStrLen, (utc_t *)attrValue))
	    sprintf(printStr,"<Internal Error> %d",__LINE__);
	break;
	
	case K_TYPE_UNSIGNED32:
	sprintf(printStr,"%u", *((unsigned *)attrValue));
	break;

	case K_TYPE_INTEGER32:
	sprintf(printStr,"%d", *((int *)attrValue));
	break;

	case K_TYPE_COURIER_ROLE:
	switch (*((int *)attrValue)) {
	    case 0: sprintf(printStr,"Courier");
	    break;
	    case 1:sprintf(printStr,"NonCourier");
	    break;
	    case 2:sprintf(printStr,"Backup Courier");
	    break;
	    default: sprintf(printStr,"<Internal Error> %d",__LINE__);
	    break;
	}
	break;
	     
	case K_TYPE_VERSION:
	letter= ((char *)attrValue)[K_VER_LET];
	v_major = ((char *)attrValue)[K_VER_MAJ];
	v_minor = ((char *)attrValue)[K_VER_MIN];
	v_level = ((char *)attrValue)[K_VER_SUB];
	sprintf(printStr,"%c%1d.%1d.%1d",
		letter,v_major,v_minor,v_level);
	break;

	case K_TYPE_STATE:
	switch ( *((int *)attrValue) ) {
	    case 0: sprintf(printStr,"%s","Off");
	    break;
	    case 1: sprintf(printStr,"%s","On");
            break;
            case 2: 
	    case 4: sprintf(printStr,"%s","Synchronizing");
            break;
            case 3: sprintf(printStr,"%s","Updating");
            break;
	    case 5: sprintf(printStr,"%s","Not Created");
	    break;
            default: sprintf(printStr,"<Internal Error> %d",__LINE__);
            break;
	}
	break;

	
	case K_TYPE_VARIANT:
	switch ( *((int *)attrValue) ) {
	    case 0: sprintf(printStr,"%s", "Server");
	    break;
	    case 1: sprintf(printStr,"%s", "Clerk");
	    break;
	    default:
	    sprintf(printStr,"<Internal Error> %d",__LINE__);
	    break;
	}
	break;

	case K_TYPE_BOOLEAN:
	switch( *((int *)attrValue) ) {
	    case 0: sprintf(printStr,"FALSE");
	    break;
	    case 1: sprintf(printStr,"TRUE");
	    break;
	    default: sprintf(printStr,"<Internal Error> %d",__LINE__);
	    break;
	}
	break;

	case K_TYPE_SIMPLENAME:
        {

	    break;
        }


	case K_TYPE_FULLNAME:
        {
	    break;
	}

	case K_TYPE_STRING:
	    string = (String_t *)attrValue;
	    strcpy(printStr, (char *)string->buffer );
	    break;

	case K_TYPE_BINABSTIME:
	    if (utc_asclocaltime(printStr, printStrLen, (utc_t *)attrValue))
	        sprintf(printStr,"<Internal Error>%d", __LINE__);
	    break;

	case K_TYPE_UID:
        {
	    uuid_t *uuid;
	    unsigned_char_p_t string_uuid;
	    unsigned32 status;

	    uuid = (uuid_t *)attrValue;
	    uuid_to_string (uuid, &string_uuid, &status);
	    sprintf(printStr, "%s", string_uuid);
	    rpc_string_free (&string_uuid, &status);

	    break;
	}

	case K_TYPE_COUNTER64:
	{
	  char *ptr,ch;
	  int  leadingZeros;
	  unsigned long int quot,remd,dividend;
	  unsigned long int divisor = 1000000000;  
	                          /* 10E9 equals the magnitude of 32 bit int. */
	                          /* Should be O.K. for larger size ints since*/
	                          /* counter type guaranteed to be 64 bits.   */

	  Bits64 t ;
	  
	  HYPER_TO_U64(t, *((idl_uhyper_int *)attrValue));

	    leadingZeros = TRUE;
	    printStr[0] = '\0';
	    uediv((Bits64*)&t,divisor,&quot,&remd);

	    /*
	     * Check for zero && for > size int
	     */
	    if (quot == 0 && remd == 0) {
	      printStr[0] = '0';
	      printStr[1] = '\0';
	    }
	    else if (quot != 0) {
	      leadingZeros = FALSE;
	      sprintf(printStr,"%u",quot);
	    }


	    /*
	     * if there are trailing zeros to print, or if
	     * there is a number less than 10E9, generate the output.
	     */
	    if (quot != 0 || remd != 0) {
	      ptr = &(printStr[strlen(printStr)]);
	      do {
	        divisor = divisor/10;  
	        dividend = remd;
	        quot = dividend/divisor;
		ch = quot + '0';
		if (ch != '0' || !leadingZeros) {
		  *ptr = ch;
		  ptr++;
		  leadingZeros = FALSE;
		}
	        remd    = dividend % divisor;
	      } while (divisor > 1);
	    
	      *ptr = '\0';
	    }
	      
	    break;
	}
	      

	case K_TYPE_ID802:
	case K_TYPE_TOWERSET:
	case K_TYPE_NSCTS:
	case K_TYPE_SET:
	case K_TYPE_ENUMERATION:
	    sprintf(printStr,"<Internal Error> %d",__LINE__);
	break;

	default:
	    sprintf(printStr,"<Internal Error> %d",__LINE__);
    }


    switch (attribute)
    {
	case K_CLOCK_ADJUSTMENT_ATTR:
	case K_CLOCK_DRIFT_ATTR:
	       (void)printf(" = %s  nsec/sec\n", printStr);
               break;
	       
        case K_CLOCK_RESOLUTION_ATTR:
	       (void)printf(" = %s  nsec\n", printStr);
               break;

	default:
	       (void)printf(" = %s\n", printStr);
    }
}

    
/*
 *
 * P R I N T _ S H O W _ C H A R
 *
 * Print out the values retuned by the show characteristics directive.
 *
 */

void PrintShowChar( CtlShowAllChar *charBuf)
{
    utc_t tempBinTime;
    int tempInteger32;
    
    if ( charBuf->variant == (long int)VTserver ) {
	
	PrintAttrText(K_CHECK_INTERVAL_ATTR);
	UTC_TO_BIN_ABS(&tempBinTime, &charBuf->checkInt);
	PrintAttrValue(K_CHECK_INTERVAL_ATTR,
		       K_TYPE_BINRELTIME,
		       (VOID *)&tempBinTime);

	PrintAttrText(K_EPOCH_NUMBER_ATTR);
	PrintAttrValue(K_EPOCH_NUMBER_ATTR,
		       K_TYPE_INTEGER32, 
		       (VOID *)&charBuf->epochNumber);
    }    

    PrintAttrText(K_ERROR_TOLLERANCE_ATTR);
    UTC_TO_BIN_ABS(&tempBinTime, &charBuf->errorTolerance);
    PrintAttrValue(K_ERROR_TOLLERANCE_ATTR,
		   K_TYPE_BINRELTIME,
		   (VOID *)&tempBinTime);
    
    PrintAttrText(K_TIME_DIFF_ATTR);
    {
	long tdfvalue = charBuf->serverTDF;
	reltimespec_t reltime;
	reltime.tv_sec = tdfvalue ;
	reltime.tv_nsec = 0L ;
	(void)utc_mkbinreltime(& tempBinTime, &reltime, (timespec_t*)0);
    }
    PrintAttrValue(K_TIME_DIFF_ATTR,
		   K_TYPE_BINRELTIME,
		   (VOID *)&tempBinTime);

    PrintAttrText(K_MAXIMUM_INACCURACY_ATTR);
    UTC_TO_BIN_ABS(&tempBinTime, &charBuf->maxInacc);
    PrintAttrValue(K_MAXIMUM_INACCURACY_ATTR,
		   K_TYPE_BINRELTIME,
		   (VOID *)&tempBinTime);
    
    PrintAttrText(K_SERVERS_REQUIRED_ATTR);
    PrintAttrValue(K_SERVERS_REQUIRED_ATTR,
		   K_TYPE_INTEGER32, 
		   (VOID *)&charBuf->minServers);
    
    PrintAttrText(K_QUERY_ATTEMPTS_ATTR);
    PrintAttrValue(K_QUERY_ATTEMPTS_ATTR,
		   K_TYPE_INTEGER32, 
		   (VOID *)&charBuf->queryAttempts);
    
    PrintAttrText(K_LAN_TIMEOUT_ATTR);
    UTC_TO_BIN_ABS(&tempBinTime, &charBuf->LANtimeOut);
    PrintAttrValue(K_LAN_TIMEOUT_ATTR,
		   K_TYPE_BINRELTIME,
		   (VOID *)&tempBinTime);
    
    PrintAttrText(K_WAN_TIMEOUT_ATTR);
    UTC_TO_BIN_ABS(&tempBinTime, &charBuf->WANtimeOut);
    PrintAttrValue(K_WAN_TIMEOUT_ATTR,
		   K_TYPE_BINRELTIME,
		   (VOID *)&tempBinTime);
    
    PrintAttrText(K_SYNCH_ATTR);
    UTC_TO_BIN_ABS(&tempBinTime, &charBuf->syncHold);
    PrintAttrValue(K_SYNCH_ATTR,
		   K_TYPE_BINRELTIME,
		   (VOID *)&tempBinTime);
    
    PrintAttrText(K_VARIANT_ATTR);
    tempInteger32 = (charBuf->variant==(long int)VTserver) ? 0 : 1;
    PrintAttrValue(K_VARIANT_ATTR,
		   K_TYPE_VARIANT,
		   (VOID *)&tempInteger32);
    
    if ( charBuf->variant == (long int)VTserver ) {
    
	PrintAttrText(K_COURIER_ROLE_ATTR);
	PrintAttrValue(K_COURIER_ROLE_ATTR,
		       K_TYPE_COURIER_ROLE, 
		       (VOID *)&charBuf->courierRole);

	PrintAttrText(K_BACKUP_COURIER_ROLE_ATTR);
	PrintAttrValue(K_BACKUP_COURIER_ROLE_ATTR,
		       K_TYPE_COURIER_ROLE, 
		       (VOID *)&charBuf->actingCourierRole);
    }
    
    PrintAttrText(K_CLOCK_ADJUSTMENT_ATTR);
    PrintAttrValue(K_CLOCK_ADJUSTMENT_ATTR,
		   K_TYPE_UNSIGNED32, 
		   (VOID *)&charBuf->clockAdjustment);
    
    PrintAttrText(K_CLOCK_DRIFT_ATTR);
    PrintAttrValue(K_CLOCK_DRIFT_ATTR,
		   K_TYPE_UNSIGNED32,
		   (VOID *)&charBuf->driftRate);

    PrintAttrText(K_CLOCK_RESOLUTION_ATTR);
    PrintAttrValue(K_CLOCK_RESOLUTION_ATTR,
		   K_TYPE_UNSIGNED32, 
		   (VOID *)&charBuf->clockResolution);


    PrintAttrText(K_DECDTS_VERSION_ATTR);
    PrintAttrValue(K_DECDTS_VERSION_ATTR,
		   K_TYPE_VERSION,
		   (VOID *)charBuf->DECdts);

    PrintAttrText(K_TIME_REP_ATTR);
    PrintAttrValue(K_TIME_REP_ATTR,
		   K_TYPE_VERSION,
		   (VOID *)charBuf->timeVersion);


    if ( charBuf->variant == (long int)VTserver ) {

	PrintAttrText(K_TP_PRESENT_ATTR);
	PrintAttrValue(K_TP_PRESENT_ATTR,
		       K_TYPE_BOOLEAN,
		       (VOID *)&charBuf->hasProvider);
    }

    PrintAttrText(K_AUTO_TDF_ATTR);
    PrintAttrValue(K_AUTO_TDF_ATTR,
		   K_TYPE_BOOLEAN,
		   (VOID *)&charBuf->autoTDF);

    PrintAttrText(K_NEXT_TDF_ATTR);
    PrintAttrValue(K_NEXT_TDF_ATTR,
		   K_TYPE_BINABSTIME,
		   (VOID *)&charBuf->nextTDFchange);


    if ( charBuf->variant == (long int)VTserver ) 
    {

	PrintAttrText(K_SERVER_PRINCIPAL_NAME);
	PrintAttrValue(K_SERVER_PRINCIPAL_NAME,
		       K_TYPE_STRING, 
		       (VOID *)&charBuf->serverPrincName);
	
	PrintAttrText(K_SERVER_ENTRY_NAME);
	PrintAttrValue(K_SERVER_ENTRY_NAME,
		       K_TYPE_STRING, 
		       (VOID *)&charBuf->serverEntryName);
	
	PrintAttrText(K_SERVER_GROUP_NAME);
	PrintAttrValue(K_SERVER_GROUP_NAME,
		       K_TYPE_STRING, 
		       (VOID *)&charBuf->TSgroupName);
    }

}


/*
 *
 * P R I N T _ S H O W _ S E R V E R S
 *
 * Print either the local or global server set contained in the 
 * service response message.
 *
 */

void PrintShowServers( long Count, ServerID  serverSet[])
{
    int i;
    ServerID   *serverID;
    utc_t utcDiff;
    utc_t SyncTime;
    utc_t ServTime;
    
    /* begin */

    (void) printf("\n\n");
    (void) printf("Known Servers\n");
    (void) printf("============================================================\n\n");

    for (i=0; i < Count; i++)        
    {
	UTCValue tvalue = { 0 }, inacc = { 0 };

	serverID = &serverSet[i];

	/* print the decnet address */
	
	
	(void) printf ("%s %s\n",
		       (serverID->isGlobal ? "Global" : "Local "),
		       serverID->nodeName.buffer
		       );
	
	/* 
	 * print the last use of server - if this server has been used before,
	 * convert the UTC to local time by calling the API and print the times
	 * in ISO format.  
	 */
	
	SyncTime = serverID->sync ;      
	(void) printf("\tLast Time Polled\t");
	
        utc_comptime(&tvalue, &inacc, (long *)0, &SyncTime);
	if ( U64FITSIN32(tvalue) ) 
	
	    (void) printf ("----------Not Used----------\n");
	else
	{
	    PrintAttrValue(0,
			   K_TYPE_BINABSTIME,
			   (VOID *)&SyncTime );
	}
	
	/* print the reported time */
        ServTime = serverID->serv ;
	(void) printf ("\tLast Observed Time\t");
	
        utc_comptime(&tvalue, &inacc, (long *)0, &ServTime);
	if ( U64FITSIN32(tvalue) )
	    (void) printf("----------Not Used----------\n");
	else
	{
	    PrintAttrValue (0,
			    K_TYPE_BINABSTIME,
			    (VOID *)&ServTime);
	}
	
	(void) printf("\tLast Observed Skew\t\t");
        utc_comptime( &tvalue, &inacc, (long *)0, &ServTime);
	if ( !U64FITSIN32(tvalue) )
	 {
	    (void) utc_subtime(&utcDiff, &ServTime, &SyncTime);
	    (void) utc_abstime(&utcDiff, &utcDiff);
	    PrintAttrValue(0,
			   K_TYPE_BINRELTIME,
			   (VOID *)&utcDiff);
	}
	else
	{
	    (void)printf("\n");
	}

	(void) printf("\tUsed in Last Synchronization\t ");

	if (serverID->lastUsed) {
	    (void) printf("%s\n","= TRUE");
	} else
	    (void) printf("%s\n","= FALSE");

	printf("\tTransport Type\t\t");
	switch((int)serverID->transport) {
	case K_PROT_IEEE802: printf("%s"," = IEEE 802.3");
	break;

	case K_PROT_DECNET:  printf("%s"," = DECnet");
	break;

	case K_PROT_UDP_IP:  printf("%s"," = UDP/IP");
	break;

	case K_PROT_TCP_IP:  printf("%s"," = TCP/IP");
	break;

	case K_PROT_RPC:  printf("%s","= RPC");
	break;

	default: printf("= Not Known???");
	break;
	}
	(void) printf("\n\n");
    }
}

char *
dce_error_text( error_status_t st)
{
    static u_char msg[ dce_c_error_string_len ];
    error_status_t lst ;

    dce_error_inq_text( st, msg, (int *)&lst);
    return (char *)msg;
}
