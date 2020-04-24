/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dts_null_provider.c,v $
 * Revision 1.1.6.2  1996/02/17  23:35:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:39  marty]
 *
 * 	Added a command line switch (-z), which if true, sets the dts_null_provider in
 * 	diagnostic mode. This mode checks the error handling capability of the
 * 	ProcessRPCerror macro by passing it a bad status causing an error message to be
 * 	printed and an exit(1) to be invoked. The -z switch is not visible to the user thru
 * 	PrintHelp and InterActiveInput menu.
 * 	[1993/11/08  19:40:38  pare]
 *
 * 	CHFts06450 -- dts_null_provider is started up in background in dce_config.
 * 	Changed dts_null_provider so that it can be started in the foreground like
 * 	the other DCE daemons -- it now forks and runs as a child dettached from
 * 	the parent.
 * 	[1993/08/10  14:20:04  pare]
 *
 * 	Initial King Kong branch
 * 	[1993/07/08  20:10:48  root]
 *
 * Revision 1.1.6.1  1995/12/08  18:13:23  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/01/30  22:44 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/4  1994/12/13  21:43 UTC  pare
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1994/12/13  21:40 UTC  pare
 * 	Change second argument of daemon() from NOCLOSEFD to CLOSEFD
 * 
 * 	HP revision /main/HPDCE02/3  1994/08/18  13:33 UTC  pare
 * 	Merge changes from Hpdce02
 * 	[1995/12/08  17:25:34  root]
 * 
 * 	Corrected bonehead mistake in version string.
 * 	[1993/05/28  13:11:31  truitt]
 * 
 * 	Add new revision control string.
 * 	[1993/05/25  12:53:41  truitt]
 * 
 * 	Add version control string.
 * 	[1993/05/10  18:09:11  truitt]
 * 
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/24  14:38:40  truitt]
 * 
 * Revision 1.1.4.3  1994/07/20  20:24:41  cbrooks
 * 	CR10945 fix Time Provider errors
 * 	[1994/07/20  18:25:57  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/07/19  12:18 UTC  pare
 * 	fix merge problem
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  13:43 UTC  pare
 * 	Merge to HPDCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  14:29  pare
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.1.8  1993/03/11  22:34:28  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.4.2  1994/05/18  19:49:15  tom
 * 	Bug 10627 - Put ourselves into the background.
 * 	[1994/05/12  22:41:57  tom]
 * 
 * Revision 1.1.4.1  1994/04/28  21:08:34  cbrooks
 * 	Fix select parameters
 * 	[1994/04/27  19:40:58  cbrooks]
 * 
 * 	Code Cleanup; ANSI'fied; use termios
 * 
 * 	DTS Code Cleanup
 * 	[1994/04/25  18:41:14  cbrooks]
 * 
 * Revision 1.1.2.5  1993/01/18  22:15:31  jwade
 * 	Change comments to match the filename that should be compiled.
 * 	[1993/01/18  20:34:14  jwade]
 * 
 * Revision 1.1.2.4  1993/01/05  18:55:57  ko
 * 	Fix arithmetic error in inaccuracy computations of provider.
 * 	[1993/01/05  15:30:06  ko]
 * 
 * Revision 1.1.2.3  1992/12/30  16:38:07  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:20:34  htf]
 * 
 * Revision 1.1.3.2  1992/12/22  00:23:14  smythe
 * 	added new OSF examples disclaimer comment for OSF OT 4998 fix
 * 	[1992/12/21  21:16:51  smythe]
 * 
 * Revision 1.1.2.2  1992/12/18  18:19:22  marcyw
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:13:07  marcyw]
 * 
 * Revision 1.1.1.7  1992/09/21  18:56:23  osfrcs
 * 	    Loading drop Dce1_0_1fnl
 * 
 * Revision 1.1  1992/01/19  15:34:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * The Example code is provided to you as "Pass-through Code".
 * Pass-through Code and its related Documentation are not warrented,
 * supported, or indemnified by OSF, and are provided "AS IS".
 * You can use, sublicense, or disribute the same at your own risk.
 */
/*
 *	Module dts_null_provider.c
 *	Version V1.0
 */
/*
 * Copyright (c) 1990, 1991 by
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
 * and  should  not be  construed  as  a commitment by Digital Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliability  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DECdts)
 *
 * ABSTRACT:    Implements a DECdts Time Provider which assumes the host's
 *		time is already synchronized in some manner, either NTP or
 *		specialized hardware.
 *
 * ENVIRONMENT:	OSF Distributed Computing Environment
 *
 * AUTHORS:     Digital Distributed Time Service (DECdts).
 *		Distributed Processing Engineering (DPE).
 *                
 * DATE:	May 8, 1990
 *
 */


/* --------------------------------------------------------------------------
 *	Compiling and Running the time provider.
 *
 *    Two steps are required to build the time provider program:
 *
 *    1. Generate the interface file dtsprovider.h and the server
 *       stub code dtsprovider_sstub.c by compiling the IDL file
 *       /usr/include/dce/dtsprovider.idl with the IDL compiler.
 *       Note that only the server stub needs to be generated.
 *           
 *       % idl -client none -no_mepv -keep c_source dtsprovider.idl
 *
 *    2. Compile the C source code and link with the libdce.a library
 *
 *       % cc dts_null_provider.c dtsprovider_sstub.c -ldce -o dtsprovider
 *
 *
 *    To run the program interactively, simply type 'dtsprovider'
 *
 *	% dtsprovider
 *
 *	The provider will then ask you to enter the following parameters:
 *
 *	1. The poll period, which is the number of seconds between
 *	   time service queries. Entering 300 for example would cause the
 *	   provider to supply the inaccuracy to the time service every
 *	   5 minutes.
 *	2. The inaccuracy, which is the number of milliseconds of 
 *	   systematic inaccuracy in the timestamps delivered by the
 *	   host. The default value is 100 ms.
 *      3. The 'output trace' flag enables and disables the logging
 *         of TP tracing information to standard output.  Setting
 *         the output trace flag to true causes information to be
 *         logged.
 *
 *	The provider can also be run in the background using the command
 *	line interface:
 *
 *		% dtsprovider -p 600 -i 120 &
 *
 *	To view the valid command line arguments start the provider
 *	interactively and then exit. The arguments will displayed.
 */

/* Include Files: */

#include <pthread.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>
#include <dce/utc.h>		/* utc library routines */
#include <dce/dtsprovider.h>	/* This file is generated by the NIDL */
				/* compiler from dtsprovider.idl */
#include <dce/dce_error.h>	/* rpc error messages */
#include <dce/dce_cf.h>
#include <dts_provider_util.h>

#ifdef __hpux
#include <unistd.h>
#endif

/* 
 *	MACROS
 */

/*
 * Threadsafe macros
 */
#if _DECTHREADS_

#define BEGIN_NONREENTRANT cma_lock_global();
#define END_NONREENTRANT cma_unlock_global();

#else

#define BEGIN_NONREENTRANT
#define END_NONREENTRANT

#endif /* _DECTHREADS_ */

/*
 * Tracing Macro, Execute the code only if
 * the TRACING flag is set.
 */
#define TraceIf(Code)	if (TRACING) {BEGIN_NONREENTRANT Code END_NONREENTRANT}
/*
 * Get the next argument
 * from the command line interface.
 */
#define GETNEXTARG(argcount) if (++i >= argcount) { PrintHelp (); exit(1); }


/*
 *	Conversion factors
 */

#define K_NS_PER_SEC	(1000000000)
#define K_MS_PER_SEC	(1000)
#define K_NS_PER_MS	(K_NS_PER_SEC / K_MS_PER_SEC)
#define K_100NS_PER_MS  (10000)

/*
 *	Literals
 */

/*
 *  Maximum/Minimum parameters for TP
 */
#define K_MIN_INACCURACY     (10)	/* Allow for 10 ms systematic   *
					* inaccuracy                    */
#define K_MAX_INACCURACY     (60 * K_MS_PER_SEC)
					/* Allow for at most 1 minute   *
					 * of inaccuracy		*/
#define K_MIN_POLLRATE	     (2)	/* Minimum time between synchs.	*/
#define K_MAX_POLLRATE       (24*60*60)	/* Maximum time between synchs.	*/

/*
 *	Default Control/Config Values
 */
#define K_DEF_INACCURACY     (100)	   /* Assume 100 ms of inacc.	*/
#define K_DEF_TMORATE	     (1)	   /* Let service wait 1 secs.  *
					    * for provider to respond   */
#define K_DEF_POLLRATE       (60)          /* query each 60 seconds     *
					    * to get a new time		*/
#define K_DEF_TRACING	     (0)	   /* Time Provider tracing	*/

#ifdef __hpux
#define K_DEF_DEBUG          (0)           /* Debug switch              */
#define K_DEF_DIAG           (0)           /* Diag switch               */
#endif

/*
 *     RPC definitions
 */

#define ABORT                (1)    /* abort the TP if error in RPC call */
#define CONTINUE             (0)    /* continue after an RPC error */
#define K_MAX_NUM_THREADS    (1)    /* Maximum number of threads we allow */

/*
 * This macro checks if an error occurred in an RPC 
 * call. Print the message if this is the case. Exit the TPP
 * process if requested by the caller of the macro.
 */

#define ProcessRPCerror(st, message, code)              \
   {                                                    \
	if ((st) != rpc_s_ok)                           \
	{                                               \
	   int _locstat;				\
	   char _rpcmsg[dce_c_error_string_len];	\
							\
	   dce_error_inq_text((st),                     \
                              (unsigned char *)_rpcmsg, \
                              &_locstat);               \
	   BEGIN_NONREENTRANT				\
	   (void)fprintf(stderr, "%s - %s\n", (message), _rpcmsg); \
           END_NONREENTRANT				\
           if ((code) == ABORT)                         \
	       exit(1);                                 \
	}                                               \
   }


/*
 *	Forward Definitions
 */


int main(int	,  char	*argv[] );


void ContactProvider( handle_t , TPctlMsg *, error_status_t *);
 
void ServerRequestProviderTime( handle_t , TPtimeMsg *, error_status_t *);
void CommandLineInput(int , char *argv[] );

void InterActiveInput(void);

void PrintHelp(void);

int ReadTimes( TPtimeMsg *);

void PrintTimes(TPtimeMsg *);
void PrintISO(utc_t *);

/*
 * RPC global definitions
 * 
 * Entry point vector for routines called from the DTS
 * deamon (RPC client) to the TPP (RPC server).
 */

globaldef time_provider_v1_0_epv_t time_provider_epv =
{
    ContactProvider,
    ServerRequestProviderTime
};

/* 
 * Global Variables
 */

unsigned long  TMORATE   = K_DEF_TMORATE;    /* provider response timeout */

unsigned long  POLLRATE  = K_DEF_POLLRATE;   /* seconds between TP syncs*/

unsigned long  NOCLOCKSET= 1;		     /* clock set               */

unsigned long  TRACING   = K_DEF_TRACING;    /* output tracing          */

#ifdef __hpux
unsigned long  DEBUG     = K_DEF_DEBUG;      /* debug switch            */
unsigned long  DIAG      = K_DEF_DIAG;       /* Diag switch            */
static int debug = 0;          		     /* debug -d */
static int diag = 0;          		     /* diag switch -z*/
#endif   

unsigned long  INACCURACY= K_DEF_INACCURACY; /* provider inaccuracy	*/

timespec_t INACC_timespec;		 /* inaccuracy of each time reading */


/*
 *++
 *  main()
 *
 *  Functional Description:
 *
 *	Main Loop for the TP.
 *
 *  Inputs: 
 *
 *	argc, argv - standard command line input parameters.
 *
 *  Outputs: 
 *
 *	None.
 * 
 *  Value Returned:  
 *
 *	None.
 *
 *--
 */
int main(int	argc ,
          char	*argv[] )

{

    int             numThreads;		/* Max number of threads created */
    uuid_t          TP_uuid;            /* UUID for this interface       */
    unsigned32	    maxCalls;         	/* Max # of concurrent calls this */
                                        /* server can receive.            */
    int i;
    unsigned32	    status;             /* status return values          */
    rpc_binding_vector_p_t bind_vector;  /* vector of binding handles */
                                         /* over which server can receive */
                                         /* remote procedure calls.       */
    unsigned_char_t *machinePrincipalName;
    pthread_t signal_thread ;

    /* 
     * Get the arguments
     */
    if (argc < 2)
    {
	PrintHelp ();
        InterActiveInput();
    }
    else
	CommandLineInput(argc, argv);
    /*
     * Convert the user specified inaccuracy into
     * a timespec struct for use with API.
     */	
    INACC_timespec.tv_sec = INACCURACY / K_MS_PER_SEC;
    INACC_timespec.tv_nsec = (INACCURACY % K_MS_PER_SEC) * K_NS_PER_MS;

    if (!TRACING)
    {
        daemon(CHDIR, CLOSEFD);	/* detach program from tty */
    }

    /* 
     * The rest of this code (in main) is relevant to RPC. Initialize the 
     * server by registering the interface and the protocol sequence with the 
     * RPC run-time library. Then, listen for calls coming from a client.  
     * There is only one Time Provider RPC server per node and only 
     * one client invokes it (the DTS daemon).
     *
     * Note:  Once the first RPC library call is made, then the process will
     * be multi-threaded.  All code called after this point must be threadsafe.
     */

    /*
     * Create a null type UUID for registering the TPP interface
     */
    uuid_create_nil (&TP_uuid, &status);

    /*
     * Register the TPP interface with the RPC run-time library
     */
    rpc_server_register_if (time_provider_v1_0_s_ifspec,
			    &TP_uuid,
			    (rpc_mgr_epv_t) &time_provider_epv,
			    &status);
    /*
     * If not a successful interface registration, exit TPP
     */
    ProcessRPCerror (status,
		     "Unable to register interface for TPP", 
		     ABORT);

    /*
     * Tell the RPC run-time to listen for remote procedure calls using
     * all supported protocol sequences.  Since there is only one 
     * client sending calls (The DTS time service), the maximum number
     * of concurrent calls should be only one.
     */
    maxCalls = 1;
    rpc_server_use_all_protseqs (maxCalls,
			         &status);
    ProcessRPCerror (status, 
		     "TPP unable to register protocol sequences ", 
		     ABORT);

    /* 
     * Get the vector of bindings over which this server can 
     * receive remote procedure calls.
     */

    rpc_server_inq_bindings (&bind_vector, &status);
    ProcessRPCerror (status, "Can't inquire bindings", ABORT);

    /*
     * If trace has been enabled, check the binding vector to 
     * confirm that the TPP server is there.
     */

    TraceIf(
	    rpc_server_inq_bindings (&bind_vector, &status);
            ProcessRPCerror (status, 
			     "Can't inquire bindings", ABORT);
	    for (i=0; i< bind_vector->count; i++)
	    {
		unsigned_char_p_t str_binding;
		
		rpc_binding_to_string_binding (bind_vector->binding_h[i],
					       &str_binding,
					       &status);
		ProcessRPCerror (status, "Can't bind to string", CONTINUE);
		if (status == rpc_s_ok)
		   fprintf (stderr, "Got binding: %s\n", str_binding);
		rpc_string_free (&str_binding, &status);
	    }
    )

    /*
     * Add address information for this Time Provider to the endpoint
     * mapper database. The endpoint map is managed by the RPC daemon,
     * which creates and deletes elements on behalf of RPC servers.
     */

    rpc_ep_register (time_provider_v1_0_s_ifspec,
		     bind_vector,
		     NULL,
		     (unsigned_char_p_t)"NULL Time Provider",
		     &status);
    ProcessRPCerror (status, "Can't register with endpoint mapper", ABORT);


    /*
     * Obtain the name of the machine's principal and register an 
     * authentication service to use for authenticated remote procedure
     * calls coming from the time service daemon.  HostName is NULL, so
     * this routine implicitely gets the host name and returns the 
     * machine's principal name.
     */

    dce_cf_prin_name_from_host ((char *)NULL, (char **)&machinePrincipalName, 
				&status);
    ProcessRPCerror (status, "Can't get machine's host name", ABORT);



    rpc_server_register_auth_info (machinePrincipalName,
				  rpc_c_authn_dce_private,
				  NULL,
				  NULL,
				  &status);

    ProcessRPCerror (status, "Can't register authentication", ABORT);


    /*
     * create a signal catcher thread to kill kill use nicely 
     * after an external signal 
     */
 
     if(pthread_create(&signal_thread, pthread_attr_default, 
                  (pthread_startroutine_t) signal_catcher, 
                  (pthread_addr_t)NULL))
       perror("dts_ntp_provider: couldn't create signal thread:");


    /*
     * At this point we listen for incoming calls from a DTS
     * daemon client program (running as a server) that wants
     * to synchronize with the Time Provider.  RPC dispatches
     * the requests to the appropriate procedures.
     */

    TraceIf (
	     fprintf(stderr, "Listening for connections ...\n");
	     );


    numThreads = K_MAX_NUM_THREADS;
    rpc_server_listen (numThreads, &status);

/*    ProcessRPCerror (status, "RPC listen terminated abnormally", ABORT); */

     /* unregister our endpoints like a good citizen */
     rpc_ep_unregister (time_provider_v1_0_s_ifspec,
                    bind_vector,
                    NULL,
                    &status);


}
/* End of main program*/


/**********************************************************************
 *
 *	DTS TP/TS REMOTE PROCEDURE CALL (RPC) ROUTINES
 *
 **********************************************************************/

/*
 *++
 *  ContactProvider()
 *
 *  Functional Description:
 *
 *     This procedure is invoked by a remote client to initiate contact
 *     with the Time Provider Program.  The task of this routine is to 
 *     send an initial control response message to the client.
 *     
 *  
 *  Inputs:
 *       bind_handle         - pointer to the rpc_binding_handle
 *                             passed by the client
 *       TimeServiceRequest  - pointer to a structure with control
 *                             information. 
 *
 *
 *  Implicit Inputs:
 *
 *	None.
 *
 *  Outputs:
 *       TMOresponse   - pointer to time response message returned to
 *                       the client (the DTS server) which will identify
 *                       this TPP server as active.
 *
 *       ComStatus     - This status is used by RPC to detect any 
 *                       communication errors between the client and 
 *                       the server.  We return the value rpc_s_ok if
 *                       the call was successfully invoked.
 *
 *  Implicit Outputs:
 *
 *	None.
 *
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */

void ContactProvider( handle_t    bind_handle ,
                      TPctlMsg    *TMOresponse ,
                      error_status_t *ComStatus )

{

    TraceIf(
	    fprintf(stderr, "Executing ContactProvider ...\n");
	    );

    /*
     * Prepare a time out response message to issue to the service
     * which will identify this process as active, and 
     * tell the service how long it must wait for the time response.
     */
    TMOresponse->status = (unsigned short)K_TPI_SUCCESS;
    TMOresponse->nextPoll = POLLRATE;
    TMOresponse->timeout  = TMORATE;
    TMOresponse->noClockSet = NOCLOCKSET;
    *ComStatus = rpc_s_ok;

} /* End of ContactProvider */


/*
 *++
 *  ServerRequestProviderTime()
 *
 *  Functional Description:
 *
 *     
 *  
 *  Inputs:
 *       bind_handle   - pointer to the rpc_binding_handle
 *                       passed by the client
 *
 *
 *  Implicit Inputs:
 *
 *	None.
 *
 *  Outputs:
 *       TimeResponse   - This structure contains the time stamps and 
 *                        version info returned to the client.
 *
 *       ComStatus      - This status is used by RPC to detect any 
 *                        communication errors between the client and 
 *                        the server.  We return the value rpc_s_ok if
 *                        the call was successfully invoked.
 *      
 *
 *  Implicit Outputs:
 *
 *	None.
 *
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */
 
void ServerRequestProviderTime( handle_t    bind_handle ,
                                 TPtimeMsg   *timeResponse ,
                                 error_status_t *ComStatus )

{
    int errStatus;

    TraceIf(
	   fprintf(stderr, "Executing ServerRequestProviderTime ...\n");
	    );

    /*
     * Return the number of time stamps in the message.
     */

    timeResponse->timeStampCount = 1;
    
    /*
     * Send the requests to the provider and get the
     * results.
     */

    errStatus = ReadTimes(timeResponse);
    
    /*
     * Check to see if the times were read by the child properly.
     * Return success if the times are there, failure otherwise.
     */

    timeResponse->status = errStatus ? K_TPI_FAILURE : K_TPI_SUCCESS;
    
    TraceIf(
	    (void)printf("Reply written :\n");
	    PrintTimes(timeResponse);
	    );

    /*
     * return RPC status to the caller.  This status is used for 
     * signaling communication errors to the client, in case it 
     * cannot bind to the server, or the server crashes while handling
     * an RPC call.  Return rpc_s_ok, since Time Provider errors are
     * returned in the timeResponse structure.
     */

    *ComStatus = rpc_s_ok;

#ifdef __hpux
    if (diag)  {
	errStatus = 1;
        ProcessRPCerror (errStatus,
		     "Exiting Diagnostic Mode", 
		     ABORT);
    }
#endif


} /* End of ServerRequestProviderTime */

/*******************************************************************
 *
 *		User Interface Routines
 *
 *******************************************************************/

/*
 *++
 *  CommandLineInput()
 *
 *  Functional Description:
 *
 *	Set the Time provider control parameters using the 
 *	user's command line parameters.
 *  
 *  Inputs:
 *
 *	argc - argc from main.
 *	argv - argv from main.
 *
 *  Outputs:
 *
 *	None.
 * 
 *  Value Returned:
 *
 *	None.
 *
 *--
 */
void CommandLineInput(int argc ,
                       char *argv[] )

{
    int i;


    for (i=1 ; i<argc ; i++)
    {
	if (argv[i][0] != '-')
	{
	    PrintHelp ();
	    exit(1);
	} 
        else 
        {
	    /* 
	     * Accept upper and lower case.
	     */
	    switch (tolower(argv[i][1]))
	    {
		case 'o': {
		    GETNEXTARG(argc);
		    if (1 != sscanf(argv[i],"%d",&TRACING))
		    {
			(void) fprintf(stderr, 
				"-o:Output value is out of legal range\n");
			PrintHelp ();
			exit(1);
		    }
		    break;
		}

		case 'p': {
		    GETNEXTARG(argc);
		    if (1 != sscanf(argv[i],"%d",&POLLRATE) ||
			POLLRATE < K_MIN_POLLRATE || 
			POLLRATE > K_MAX_POLLRATE)
		    {
			(void) fprintf(stderr, 
				"-p:Pollrate value is out of legal range\n");
			PrintHelp ();
			exit(1);
		    }
		    break;
		}

		case 'i': {
		    GETNEXTARG(argc);
		    if (1 != sscanf(argv[i], "%d", &INACCURACY) ||
			INACCURACY < K_MIN_INACCURACY || 
			INACCURACY > K_MAX_INACCURACY)
		    {
			(void) fprintf(stderr,
				"-i:Inaccuracy value is out of legal range\n");
			PrintHelp();
			exit(1);
		    }
		    break;
		}
#ifdef __hpux
		case 'd': {
	            GETNEXTARG(argc);
	            if (1 != sscanf(argv[i],"%d",&DEBUG))
		    {
		        (void) fprintf(stderr,
		              "-d:Debug value is out of legal range\n");
			PrintHelp ();
			exit(1);
                    }
		    debug = 1;
		    break;
		}
		case 'z': {
	            GETNEXTARG(argc);
	            if (1 != sscanf(argv[i],"%d",&DIAG))
		    {
		        (void) fprintf(stderr,
		              "-z:Diag value is out of legal range\n");
			PrintHelp ();
			exit(1);
                    }
		    diag = 1;
		    break;
		}
#endif

		default: {
		    PrintHelp();
		    exit(1);
		}
	    }	/* switch */
	}	/* else */
    }		/* for */

}

/*
 *++
 *  InteractiveInput()
 *
 *  Functional Description:
 *
 *	Set the Time provider control parameters using the 
 *	user's interactive responses.
 *  
 *  Inputs:
 *
 *	None.
 *
 *  Outputs:
 *
 *	None.
 * 
 *  Value Returned:
 *
 *	None.
 *
 *--
 */
void InterActiveInput(void)

{
    char inputLine[512];

    /*
     * Get the POLLRATE
     */
    (void) printf("Enter the Poll Rate in Seconds[%d]: ", POLLRATE);
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%d", &POLLRATE); 
    if (POLLRATE < K_MIN_POLLRATE || POLLRATE > K_MAX_POLLRATE)
    {
	(void) fprintf(stderr, "-p:Pollrate value is out of legal range\n");
	exit(1);
    }

    /*
     * Get the Inaccuracy
     */
    (void) printf("Enter the Inaccuracy[%d(ms)]: ", INACCURACY);
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%d", &INACCURACY);
    if (INACCURACY < K_MIN_INACCURACY || INACCURACY > K_MAX_INACCURACY)
    {
	(void) fprintf(stderr, "-i:Inaccuracy value is out of legal range\n");
	exit(1);
    }

    /*
     * Get the Output Trace flag
     */
    (void) printf("Output Trace [%d(Y:1,N:0)] : ", TRACING);
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%d",&TRACING);

#ifdef __hpux
    /*
    * Get the Debug flag
    */
    (void) printf("Debug [%d(Y:1,N:0)] : ", DEBUG);
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%d",&DEBUG);
    debug = DEBUG;
#endif

}

/*
 *++
 *  PrintHelp()
 *
 *  Functional Description:
 *
 *	Print out help text to the user.
 *  
 *  Inputs:
 *
 *	None.
 *
 *  Outputs:
 *
 *	None.
 * 
 *  Value Returned:
 *
 *	None.
 *
 *--
 */
void PrintHelp(void)

{
    (void) fprintf(stderr,
		   "usage:\tdtsprovider\n");
    (void) fprintf(stderr,
		   "\t\t[-p pollrate, range %d : %d, default %d]\n",
		   K_MIN_POLLRATE,K_MAX_POLLRATE,K_DEF_POLLRATE);
    (void) fprintf(stderr,
     "\t\t[-i milliseconds of inaccuracy, range %d : %d, default %d]\n",
		   K_MIN_INACCURACY, K_MAX_INACCURACY, K_DEF_INACCURACY);
    (void) fprintf(stderr,
     "\t\t[-o output trace, range 0(false) : 1(true), default %d]\n",
		   K_DEF_TRACING);

#ifdef __hpux
    (void) fprintf(stderr,
	     "\t\t[-d debug, usage 0(false) : 1(true), default %d]\n",
					 K_DEF_DEBUG);
#endif

}
/* End of routine PrintHelp */

/*****************************************************************
 *
 *	Device Dependent code	- 
 *			NULL request/response
 *
 *****************************************************************/
/*
 *++
 *  ReadTimes()
 *
 *  Functional Description:
 *
 *	Query the system's clock the requisite number of times. 
 *  
 *  Inputs:
 *
 *	None
 *
 *  Outputs:
 *
 *	tpTimeMsg - buffer to return the timestamp data.
 * 
 *  Value Returned:
 *
 *	0 - Success
 *	-1 - Failure
 *
 *--
 */
int ReadTimes( TPtimeMsg *tpTimeMsg )

{
    timespec_t	ttime;

    /* Read the timestamps */
    if (utc_gettime(&tpTimeMsg->timeStampList[0].beforeTime) != 0)
	return(-1);
    if (utc_gettime(&tpTimeMsg->timeStampList[0].TPtime) != 0)
	return(-1);
    if (utc_gettime(&tpTimeMsg->timeStampList[0].afterTime) != 0)
	return(-1);

    /* Set inaccuracy of current time */
    if (utc_bintime(&ttime, (timespec_t *)0, (long *)0,
		       &tpTimeMsg->timeStampList[0].TPtime) != 0)
	return(-1);
    return(utc_mkbintime(&tpTimeMsg->timeStampList[0].TPtime,
			 &ttime, &INACC_timespec, 0L));
}

/*
 **********************************************************
 *		Diagnostic  Routines.
 **********************************************************
 */
/*
 *++
 *  PrintTimes()
 *
 *  Functional Description:
 *
 *	Print out the contents of a time response message
 *  
 *  Inputs:
 *
 *	timeResponse - the time response message to print out.
 *
 *  Outputs:
 *
 *	None.
 * 
 *  Value Returned:
 *
 *	None.
 *
 *--
 */
void PrintTimes(TPtimeMsg *timeResponse )

{
    int  i;

    if (timeResponse->status == K_TPI_FAILURE)
    {
	BEGIN_NONREENTRANT
	(void) printf("K_TPI_FAILURE\n");
        (void) printf("******************************************\n");
	END_NONREENTRANT
	return;
    }

    BEGIN_NONREENTRANT 
    (void) printf("K_TPI_SUCCESS\n");
    (void) printf("Time Stamps   : %d\n",
		  timeResponse->timeStampCount);
    for (i = 0; i < timeResponse->timeStampCount; i++)
    {
	(void) printf("Before Time\t:");
	PrintISO(&timeResponse->timeStampList[i].beforeTime);
	(void) printf("\nTP Time\t\t:");
	PrintISO(&timeResponse->timeStampList[i].TPtime);
	(void) printf("\nAfter Time\t:");
	PrintISO(&timeResponse->timeStampList[i].afterTime);
	(void) printf("\n\n");
    }
    (void) printf("******************************************\n");
    END_NONREENTRANT
}

/*
 *++
 *  PrintISO()
 *
 *  Functional Description:
 *
 *	Print out a binary absolute time as an LOCAL ISO time
 *  
 *  Inputs:
 *
 *	utcTime - the address of a utc_t.
 *
 *  Outputs:
 *
 *	None.
 * 
 *  Value Returned:
 *
 *	None.
 *
 *--
 */
void PrintISO(utc_t *utcTime )

{
    char	gmTimeStr[50];

    (void) utc_ascgmtime (gmTimeStr, sizeof(gmTimeStr), utcTime);
    printf ("%s\t", gmTimeStr);
}
