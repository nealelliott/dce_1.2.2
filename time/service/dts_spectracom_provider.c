/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: dts_spectracom_provider $Revision: 1.1.10.2 $ $Date: 1996/02/17 23:35:29 $
 */
/*
 * HISTORY
 * $Log: dts_spectracom_provider.c,v $
 * Revision 1.1.10.2  1996/02/17  23:35:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:41  marty]
 *
 * Revision 1.1.10.1  1995/12/08  18:13:29  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/07  23:06 UTC  dat
 * 	Confine references to debug to "__hpux" defined blocks only.
 * 
 * 	HP revision /main/HPDCE02/5  1995/01/30  22:44 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/4  1994/12/13  21:46 UTC  pare
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1994/12/13  21:44 UTC  pare
 * 	Change second argument of daemon() from NOCLOSEFD to CLOSEFD.
 * 
 * 	HP revision /main/HPDCE02/3  1994/08/18  13:36 UTC  pare
 * 	Merge changes from Hpdce02
 * 	[1995/12/08  17:25:38  root]
 * 
 * Revision 1.1.7.3  1994/07/20  20:24:44  cbrooks
 * 	CR10945 fix Time Provider errors
 * 	[1994/07/20  18:26:07  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/07/19  12:33 UTC  pare
 * 	fix merge problems
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  15:15 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 be
 * 	ta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.7.2  1994/05/18  19:49:19  tom
 * 	Bug 10627 - Put ourselves into the background.
 * 	[1994/05/12  22:42:01  tom]
 * 
 * Revision 1.1.7.1  1994/04/28  21:08:37  cbrooks
 * 	Fix select parameters
 * 	[1994/04/27  19:40:59  cbrooks]
 * 
 * 	Code Cleanup; ANSI'fied; use termios
 * 	[1994/04/26  15:43:23  cbrooks]
 * 
 * 	DTS Code Cleanup
 * 	[1994/04/25  18:55:40  cbrooks]
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  14:31  pare
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.8.6  1993/12/06  22:01:28  pare
 * 	Add DCE revision control string.
 * 	[1993/12/06  22:00:53  pare]
 * 
 * Revision 1.1.8.5  1993/11/10  19:07:39  pare
 * 	Added a call to the PrintError routine when exiting through diagnostic mode.
 * 	[1993/11/10  19:07:02  pare]
 * 
 * Revision 1.1.8.4  1993/11/08  21:05:20  pare
 * 	Added a command line switch (-z), which if true, sets the dts_ntp_provider in
 * 	diagnostic mode. This mode checks the error handling capability of the
 * 	ProcessRPCerror macro by passing it a bad status causing an error message to be
 * 	printed and an exit(1) to be invoked. The -z switch is not visible to the user thru
 * 	PrintHelp and InterActiveInput menu.
 * 	[1993/11/08  21:04:42  pare]
 * 
 * Revision 1.1.8.3  1993/08/24  13:22:34  pare
 * 	Changed dts_spectracom_provider so that it can be started in the foreground like
 * 	the other DCE daemons -- it now forks and runs as a child detached from the parent.
 * 	Modified termio structure from:
 * 	   ttyb.c_cflag &= ~CSIZE;
 * 	   ttyb.c_cflag |= CS8;
 * 	   ttyb.c_cflag &= ~(PARENB | PARODD);
 * 	to:
 * 	   ttyb.c_cflag = 0x00000170
 * 	This change to the tremio structure was required to prevent the "select" call
 * 	in the routine QueryProvider from timing out.
 * 	[1993/08/24  13:21:36  pare]
 * 
 * Revision 1.1.8.2  1993/07/08  20:12:45  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:12:11  root]
 * 
 * Revision 1.1.6.4  1993/05/28  13:18:14  truitt
 * 	Corrected bonehead mistake in version string.
 * 	[1993/05/28  13:16:56  truitt]
 * 
 * Revision 1.1.6.3  1993/04/01  18:35:38  truitt
 * 	Replace sgtty structure with termios structure.
 * 	Replace sgtty.h include file with termios.h include file.
 * 	[1993/04/01  18:35:00  truitt]
 * 
 * Revision 1.1.6.2  1993/03/24  14:45:05  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/24  14:44:13  truitt]
 * 
 * 	Change comments to match the filename that should be compiled.
 * 	[1993/01/18  20:34:38  jwade]
 * 
 * 	Additional check for null in function ParseTime.  We were getting
 * 	radio clock fault without this check.
 * 	[1993/01/18  16:53:00  jwade]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  22:20:40  htf]
 * 
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:13:14  marcyw]
 * 
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:06:26  sekhar]
 * 
 * Revision 1.1.1.8  1993/03/11  22:34:31  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.5  1993/03/03  15:50:33  truitt
 * 	Remove include of termios.h and add define of TCIOFLUSH.
 * 	Set up proper cast types for the arguments in the select
 * 	command.  Used int instead of fd_set.
 * 	[1993/03/03  15:50:08  truitt]
 * 
 * Revision 1.1.3.4  1992/12/22  00:23:17  smythe
 * 	added new OSF examples disclaimer comment for OSF OT 4998 fix
 * 	[1992/12/21  21:15:24  smythe]
 * 
 * Revision 1.1.2.2  1992/06/24  16:24:36  grober
 * 	HP/UX Porting Changes
 * 	[1992/06/22  15:56:26  grober]
 * 
 * Revision 1.1.3.3  1992/06/19  19:37:59  smythe
 * 	added include of termios.h to define TCIOFLUSH
 * 	[1992/06/19  19:36:23  smythe]
 * 
 * Revision 1.1.3.2  1992/06/19  00:01:13  smythe
 * 	added hpux changes for ioctl TIOCEXCL and ioctl TIOCFLUSH calls
 * 	[1992/06/19  00:00:30  smythe]
 * 
 * Revision 1.1  1992/01/19  15:34:40  devrcs
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
 *	Module dts_spectracom_provider.c
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
 * ABSTRACT:    Implements a DECdts Time Provider which obtains the
 *		time from a Spectracom radio clock connected to a
 *		terminal line.
 *
 * ENVIRONMENT:	OSF Distributed Computing Environment
 *
 * AUTHORS:     Digital Distributed Time Service (DECdts).
 *		Distributed Processing Engineering (DPE).
 *                
 * DATE:	June 20, 1990
 *
 */

/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */

/*
 *                           DESCRIPTION OF THE
 *			   SPECTRACOM RADIO CLOCK
 * 
 *	The SPECTRACOM-8170 and NetClock-2 are WWVB receivers suitable for
 *	connection to a serial port. These function within the majority of
 *	North America. They are manufactured by:
 *
 *		SPECTRACOM CORPORATION
 *		101 Despatch Drive
 *		East Rochester, New York, 14445
 *		(716) 381-4827
 *
 *	Contact SPECTRACOM about availability of products suitable for use
 *	in Europe.
 *
 * --------------------------------------------------------------------------
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
 *       % cc dts_spectracom_provider.c dtsprovider_sstub.c -ldce -o dtsprovider
 *
 *
 *    To run the program interactively, simply type 'dtsprovider'
 *
 *	% dtsprovider
 *
 *	The provider will then ask you to enter the following parameters:
 *
 *	1. The name of the device to which the radio clock is connected,  
 *	   '/dev/tty02' for example.
 *	2. The poll period, which is the number of seconds between
 *	   queries.  Entering 60 for example would cause the provider
 *	   to be queried once a minute. 
 *	3. The Base inaccuracy, which is the number of milliseconds of 
 *	   systematic inaccuracy in the timestamps delivered by the
 *	   radio clock. The default value is 100 ms. If the times returned
 *	   by the provider are systematically incorrect, then the base
 *	   inaccuracy may need to be increased.
 *	4. The number of timestamps read at each synchronization.
 *         The range is 1 to 6 readings and the default is 4.
 *	5. The 'disallow clock set' flag which causes the service
 *         to enable or disable clock adjustments which DTS would
 *         otherwise cause. This is useful during TP development.
 *         Setting 'disallow clock set' to TRUE disables clock
 *         adjustments.
 *      6. The 'output trace' flag enables and disables the logging
 *         of TP tracing information to standard output.  Setting
 *         the output trace flag to true causes information to be
 *         logged.
 *
 *	The provider can also be run in the background using the command
 *	line interface:
 *
 *		% dtsprovider -d /dev/tty02 -p 60 &
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


#include <sys/types.h>
#include <termios.h>

#include <sys/time.h>
#include <dce/utc.h>		/* utc library routines */
#include <dce/dtsprovider.h>	/* This file is generated by the NIDL */
				/* compiler from dtsprovider */
#include <dce/dce_error.h>	/* rpc error messages */
#include <dce/dce_cf.h>
#include <dts_provider_util.h>
#include <unistd.h>

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
 * FD macros
 */
#ifndef FD_SET
#define	NFDBITS		32
#define	FD_SETSIZE	32
#define	FD_SET(n, p)	((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
#define	FD_CLR(n, p)	((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
#define	FD_ISSET(n, p)	((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
#define FD_ZERO(p)	memset((char *)(p), 0, sizeof(*(p)))
#endif
/*
 * Spectracom character conversion macros
 */
#define CVT_ASCII_TO_INT(chr) ((chr) - '0')
#define CVT_ASCII_TO_INT_AND_ADD(chrhi, chrlo) \
	((CVT_ASCII_TO_INT(chrhi) << 6) + CVT_ASCII_TO_INT(chrlo))


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

#define SPECTRACOM_PROMPT		"T"

#define MAX_RESPONSE_LENGTH	(100)	/* Length of maximum response	*/
#define MIN_RESPONSE_LENGTH	(26)	/* Length of minimum valid resp.*/

#define K_SPECTRACOM_TIMEOUT	(5)	/* Allow 5 seconds for radio	*
					 * clock to respond		*/
#define K_SPECTRACOM_RETRY	(2)	/* Retry twice			*/

#define SPECTRACOM_BAUD		(B9600)	/* Baud rate of Spectracom term.*/

/*
 *  Maximum/Minimum parameters for TP
 */
#define K_MIN_INACCURACY     (10)	/* Allow for at least 10 ms of	*
					 * systematic inaccuracy	*
					 * in each radio clock response.*/
#define K_MAX_INACCURACY     (10 * K_MS_PER_SEC)
					/* Allow for at most 10 sec of	*
					 * inacc in each radio clock	*
					 * response			*/
#define K_MIN_POLLRATE	     (30)	/* Minimum time between synchs.	*/
#define K_MAX_POLLRATE       (60 * 60)	/* Maximum time between synchs.	*/

#define K_MAX_DEV_NAME	     (255)	/* largest device name string.	*/

/*
 *	Default Control/Config Values
 */
#define K_DEF_INACCURACY     (30)          /* 30 ms inacc. in each	*
					    * radio clock response	*/
#define K_DEF_TMORATE	     (1 * 60)	   /* Let service wait 1 minute	*
					    * for provider to		*
					    * succeed.			*/
#define K_DEF_POLLRATE       (2 * 60)	   /* Query the radio clock     *
					    * every 2 minutes to get    *
					    * the time			*/
#define K_DEF_TIMESTAMPS     (4)	   /* Read the radio clock	*
					    * 4 times each synch.	*/
#define K_DEF_NOCLOCKSET     (0)	   /* Allow the clock to	*
					    * be set.			*/
#define K_DEF_TRACING	     (0)	   /* Time Provider tracing	*/
#define K_DEF_DIAG           (0)           /* Diag switch               */


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
 * prototypes
 */

int main(int, char*argv[] );

void ContactProvider( handle_t , TPctlMsg *, error_status_t *);

void ServerRequestProviderTime( handle_t , TPtimeMsg *, error_status_t *);
void CommandLineInput(int , char *argv[] );
void InterActiveInput(void);

int Delay (unsigned );
void PrintError (char *);

void PrintHelp(void);

int ReadTimes( unsigned long , int , TPtimeMsg *);
int QueryProvider(int , utc_t *, utc_t *, utc_t *);
int ParseTime (utc_t *, char *);
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

char devicename[K_MAX_DEV_NAME+1];	     /* name of TP device	*/

unsigned long  TIMESTAMP = K_DEF_TIMESTAMPS; /* time stamps at each sync*/

unsigned long  TMORATE   = K_DEF_TMORATE;    /* provider response timeout */

unsigned long  POLLRATE  = K_DEF_POLLRATE;   /* seconds between TP syncs*/

unsigned long  NOCLOCKSET= K_DEF_NOCLOCKSET; /* clock set               */

unsigned long  TRACING   = K_DEF_TRACING;    /* output tracing          */

unsigned long  INACCURACY= K_DEF_INACCURACY; /* provider inaccuracy	*/

unsigned long  INACC;			 /* inacc in nanoseconds	*/

struct tm INACC_TM;			 /* inaccuracy of each time reading */

int providerFD;				 /* provider file descriptor   */

#ifdef __hpux
unsigned long  DIAG      = K_DEF_DIAG;       /* Diag switch            */
static int diag = 0;                         /* diag switch -z*/
static int debug = 0;                   
#endif


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

    struct termios ttyb ;
    int             numThreads;         /* Max number of threads created */
    uuid_t          TP_uuid;            /* UUID for this interface       */
    unsigned32	    status;             /* status return values          */
    int i, sts;
    unsigned32    maxCalls;         /* Max # of concurrent calls this */
                                    /* server can receive.            */
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
     * a TM struct for use with API
     */	
    INACC_TM.tm_year = INACC_TM.tm_mon = INACC_TM.tm_mday = 0;
    INACC_TM.tm_hour = 0;
    INACC_TM.tm_min = INACCURACY / (60 * K_MS_PER_SEC);
    INACCURACY %= 60 * K_MS_PER_SEC;
    INACC_TM.tm_sec = INACCURACY / K_MS_PER_SEC;
    INACCURACY %= K_MS_PER_SEC;
    INACC = INACCURACY * K_NS_PER_MS;

    /*
     * Open the radio clock file descriptor for read and write.
     */

    /*
     * AIX does not support IOCTL with TIOCEXCL for a tty device.
     * so we have to use the O_EXCL flag in the open call.
     */


    if ((providerFD = open(devicename, O_EXCL | O_RDWR, 0)) < 0) {
	PrintError("dtsprovider: open radio clock");
	exit(1);
    }
  
    /*
     * Set to cooked mode
     */

     cfsetospeed(&ttyb, SPECTRACOM_BAUD);    /* set output speed */
     cfsetispeed(&ttyb, SPECTRACOM_BAUD);    /* set input speed */
     ttyb.c_iflag &= (IXOFF | IXANY);
     ttyb.c_oflag &= ~OPOST;
     ttyb.c_lflag &= ~(XCASE | ISIG | ICANON | ECHO); /* raw mode */
     ttyb.c_cflag = 0x00000170; 
     ttyb.c_cc[VTIME] = 0;                /* wait forever */
     ttyb.c_cc[VMIN] = 1;                 /* return on each character */
     sts = tcsetattr(providerFD, TCSANOW, &ttyb); /* make change now */ 

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
	    );

    /*
     * Add address information for this Time Provider to the endpoint
     * mapper database. The endpoint map is managed by the RPC daemon,
     * which creates and deletes elements on behalf of RPC servers.
     */

    rpc_ep_register (time_provider_v1_0_s_ifspec,
		     bind_vector,
		     NULL,
		     (unsigned_char_p_t)"Spectracom Time Provider",
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

    numThreads = K_MAX_NUM_THREADS;

    TraceIf (
	     fprintf(stderr, "Listening for connections ...\n");
	     );

    rpc_server_listen (numThreads, &status);
/*    ProcessRPCerror (status, "RPC listen terminated abnormally", ABORT); */

    /* unregister our endpoints like a good citizen */
     rpc_ep_unregister (time_provider_v1_0_s_ifspec,
                    bind_vector,
                    NULL,
                    &status);

    exit(0);
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

    timeResponse->timeStampCount = TIMESTAMP;
    
    /*
     * Send the requests to the provider and get the
     * results.
     */
    errStatus = ReadTimes(TIMESTAMP, providerFD, timeResponse);

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
	     "Continue from ProcessRPCerror macro.",
	      CONTINUE);
	PrintError("Exiting diagnostic mode");
	(void) close(providerFD);
	exit(1);
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
#ifdef __hpux
		    debug = 1;
#endif
		    break;
		}

		case 'c': {
		    GETNEXTARG(argc);
		    if (1 != sscanf(argv[i],"%d",&NOCLOCKSET))
		    {
			(void) fprintf(stderr, 
				"-c:Clockset value is out of legal range\n");
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

		case 't': {
		    GETNEXTARG(argc);
		    if (1 != sscanf(argv[i], "%d", &TIMESTAMP) ||
			TIMESTAMP < K_MIN_TIMESTAMPS || 
			TIMESTAMP > K_MAX_TIMESTAMPS)
		    {
			(void) fprintf(stderr,
				"-t:Time stamp count is out of legal range\n");
			PrintHelp();
			exit(1);
		    }
		    break;
		}

		case 'd': {
		    GETNEXTARG(argc);
		    (void)strcpy(devicename, argv[i]);
		    break;
		}
#ifdef __hpux
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
    
    (void) printf("\n\n\nEnter the Device Name: ");
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%255s", devicename);
    
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
     * Get the timestamp count
     */
    (void) printf("Enter the Timestamp Count[%d]: ", TIMESTAMP);
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%d",&TIMESTAMP);
    if (TIMESTAMP < K_MIN_TIMESTAMPS || TIMESTAMP > K_MAX_TIMESTAMPS)
    {
	(void) fprintf(stderr, "-t:Time stamp count is out of legal range\n");
	exit(1);
    }

    /*
     * Get the disallow clock set flag
     */
    (void) printf("Disallow clock set[%d(Y:1,N:0)] : ", NOCLOCKSET);
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%d",&NOCLOCKSET);

    /*
     * Get the Output Trace flag
     */
    (void) printf("Output Trace [%d(Y:1,N:0)] : ", TRACING);
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%d",&TRACING);
#ifdef __hpux
    if (TRACING)
        debug = 1;
#endif

}

/*
 *++
 *  Delay()
 *
 *  Functional Description:
 *
 *	Print out error text to the user.
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
int Delay (unsigned secs )

{
#if _DECTHREADS_
    timespec_t interval;

    interval.tv_sec = secs;
    interval.tv_nsec = 0;
    if (pthread_delay_np (&interval) < 0)
    {
	PrintError("dtsprovider: cannot delay");
	return (1);
    }
#else
    unsigned sleep();

    sleep(secs);
#endif

    return (0);

} /* End of routine Delay */

/*
 *++
 *  PrintError()
 *
 *  Functional Description:
 *
 *	Print out error text to the user.
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
void PrintError (char *errorString )

{
    BEGIN_NONREENTRANT

    perror (errorString);

    END_NONREENTRANT
}
/* End of routine PrintError */

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
		   "usage:\tdtsprovider -d device-name\n");
    (void) fprintf(stderr,
		   "\t\t[-p pollrate, range %d : %d, default %d]\n",
		   K_MIN_POLLRATE,K_MAX_POLLRATE,K_DEF_POLLRATE);
    (void) fprintf(stderr,
     "\t\t[-i milliseconds of inaccuracy, range %d : %d, default %d]\n",
		   K_MIN_INACCURACY, K_MAX_INACCURACY, K_DEF_INACCURACY);
    (void) fprintf(stderr,
		   "\t\t[-t timestamps, range %d : %d, default %d]\n",
		   K_MIN_TIMESTAMPS,K_MAX_TIMESTAMPS,K_DEF_TIMESTAMPS);
    (void) fprintf(stderr,
     "\t\t[-c disallow clock set, range 0(false) : 1(true), default %d]\n",
		   K_DEF_NOCLOCKSET);
    (void) fprintf(stderr,
     "\t\t[-o output trace, range 0(false) : 1(true), default %d]\n",
		   K_DEF_TRACING);
}
/* End of routine PrintHelp */

/*****************************************************************
 *
 *	Device Dependent code	- 
 *			Spectracom radio clock control
 *
 *****************************************************************/
/*
 *++
 *  ReadTimes()
 *
 *  Functional Description:
 *
 *	Parse out time stamps from the radio clock input.
 *  
 *  Inputs:
 *
 *	timeStamps - the number of timestamps to read
 *	fd - radio clock file descriptor
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
int ReadTimes( unsigned long timeStamps ,
               int fd ,
               TPtimeMsg *tpTimeMsg )

{
    int i;                          /* temp, index */
    int retry;
    int status;

    retry = K_SPECTRACOM_RETRY;

    /*
     * Now Read The Time Stamps.  
     * Note that this loop will terminate having read timeStamps timestamps
     * or exhausting retry retries. and thus is non-infinite.
     */

    i = 0;
    while ( i < timeStamps ) {
	status = QueryProvider( fd, &tpTimeMsg->timeStampList[i].TPtime,  
				&tpTimeMsg->timeStampList[i].beforeTime, 
				&tpTimeMsg->timeStampList[i].afterTime );
	if (status == 0) {
	    i += 1;
	} else {
	    if (status < 0 || retry--)
		return(-1);
	}
    }

    return(0);
}

/*
 *++
 *  QueryProvider()
 *
 *  Functional Description:
 *
 *      Query the provider for the time. Generate the timestamp triplet.
 *  
 *  Inputs:
 *
 *	fd - file descriptor to read.
 *
 *  Outputs:
 *
 *	providerTime - return utc time, the time read from provider
 *	beforeTime - the utc just before the command is sent
 *	afterTime - the utc just after the on time marker arrived.
 * 
 *  Value Returned:
 *
 *	-1	Unequivocal failure - Radio clock fault
 *	0	Success
 *	1	Timeout failure - retry warranted.
 *
 *--
 */

typedef void fd_set_t ;

int QueryProvider(int fd ,
                   utc_t *providerTime ,
                   utc_t *beforeTime ,
                   utc_t *afterTime )

{
    struct timeval timeout;		/* time out value for select       */
    char dataLine[MAX_RESPONSE_LENGTH];	/* Response buffer		   */
    fd_set readfds;			/* mask of file descriptors        */
    int	n;				/* file descriptor selected on     */
    int nfds = fd+1;                    /* number of file descripts to select */
    int ioctl_arg;
    int status;

    /*
     * Flush input and output queues
     */
    ioctl_arg = 0;

    BEGIN_NONREENTRANT 

    status = tcflush(fd, TCIOFLUSH);

    END_NONREENTRANT
    if (status < 0) {
	PrintError("dtsprovider: can't flush radio clock input");
	exit (1);
    }

    /*
     * Prepare for select
     */
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    timeout.tv_sec = K_SPECTRACOM_TIMEOUT;
    timeout.tv_usec = 0;

    /*
     * Record 'before' timestamp
     */
    (void) utc_gettime(beforeTime);

    /*
     * Write 'Query' command to radio clock
     */
    if (write(fd, SPECTRACOM_PROMPT, (sizeof(SPECTRACOM_PROMPT)-1)) !=
	(sizeof(SPECTRACOM_PROMPT)-1)) {
	PrintError("dtsprovider: can't send command to radio clock");
	exit(1);
    }

    /*
     * Wait for the reply by watching the file descriptor
     */
#ifdef __hpux
    if ((n = select(nfds, (int *)&readfds, 0, 0, &timeout)) < 0) {
#else
    if ((n = select(nfds, (fd_set_t *)&readfds, (fd_set_t *)0, (fd_set_t *)0,
                    &timeout)) < 0) {
#endif /* #ifdef __hpux */
	PrintError("dtsprovider: radio clock select");
	exit(1);
    }

    /*
     * Record 'after' timestamp
     */
    (void) utc_gettime(afterTime);

    /*
     * Check for timeout in select
     */
    if (n == 0) {
	BEGIN_NONREENTRANT
	(void) fprintf(stderr, "dtsprovider:\t* Timeout *\n");
	END_NONREENTRANT
	return(1);
    }

    /*
     * Wait so that all the characters are in and read the line
     */
    Delay(2); 
    n = read(fd, dataLine, MAX_RESPONSE_LENGTH);
    if (n < MIN_RESPONSE_LENGTH) {
	if (n < 0) {
            PrintError("dtsprovider: error reading radio clock");
	} else {
	    BEGIN_NONREENTRANT
	    (void) fprintf(stderr,
                      "dtsprovider: radio clock returned only %d bytes\n", n);
	    END_NONREENTRANT
	    dataLine[n] = '\0';
	    (void) fprintf(stderr, "time returned - %s\n", dataLine);
        }
	return(1);
    }    
    dataLine[n] = '\0';

    /*
     * Now parse the radio clock data to determine the actual time.
     */
    return(ParseTime(providerTime, dataLine));
}

/*
 *++
 *  ParseTime()
 *
 *  Functional Description:
 *
 *	Convert the radio clock text data string into a numerical utc.
 *  
 *  Inputs:
 *
 *	buffer - the radio clock data string.
 *
 *  Outputs:
 *
 *	utcTime - utc returned by the parse
 * 
 *  Value Returned:
 *
 *	0 - Buffer, parse was successful
 *	-1 - hard Failure.
 *	1 - Temporary failure - Retry later.
 *
 *--
 */
int ParseTime (utc_t *utcTime ,
                char  *buffer )

{
    static char daytab[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}};
    
    struct tm timetm;
    unsigned long ins;
    unsigned long tns;
    char status;
    int leapyear;
    int i;

    /*
     * Copy base inaccuracy
     */
    ins = INACC;

    /*
     * Convert and parse status bytes
     */
    status = buffer[2];

    if (status == '*') {
	BEGIN_NONREENTRANT
	(void) fprintf(stderr,
                  "dtsprovider: Spectracom manually set, ignoring time\n");
	END_NONREENTRANT
	return(1);
    }

    if (status == '?') {
	BEGIN_NONREENTRANT
	(void) fprintf(stderr, "dtsprovider: Spectracom has lost signal\n");
	END_NONREENTRANT
    }

    /*
     * Check clock qualtity indicator
     */

    if (buffer[3] == 'A')
	ins += 9 * K_NS_PER_MS;		/* Add additional 9 ms. */
    else if (buffer[3] == 'B')
	ins += 99 * K_NS_PER_MS;	/* Add additional 99 ms. */
    else if (buffer[3] == 'C')
	ins += 499 * K_NS_PER_MS;	/* Add additional 499 ms. */
    else if (buffer[3] == 'D')
	return(1);			/* Clock too far out */

    if (buffer[3] == '$') {
	BEGIN_NONREENTRANT
	(void) fprintf(stderr,
		       "dtsprovider: Spectracom year incorrect in switches\n");
	END_NONREENTRANT
    }

#ifdef _AIX
    if (((buffer[0] != '\r' && buffer[0]!= '\n') || buffer[1] != '\n') ||
#else
    if ((buffer[0] != '\r' || buffer[1] != '\n') ||
#endif
        (buffer[13] != ':') || (buffer[16] != ':') || (buffer[19] != '.')) {
	BEGIN_NONREENTRANT
	(void) fprintf(stderr, "dtsprovider: radio clock fault\n");
	(void) fprintf(stderr, "time returned - %26s\n", buffer);
	END_NONREENTRANT
	return(1);
    }

    /*
     * Convert bytes 4 and 5 into year. If the result is before 1990, the
     * century most have rolled over.
     */

    timetm.tm_year = CVT_ASCII_TO_INT(buffer[4]) * 10 +
		     CVT_ASCII_TO_INT(buffer[5]);
    if (timetm.tm_year < 90)
	timetm.tm_year += 100;

    /*
     * Convert bytes 7, 8 and 9 into day of year
     */
    timetm.tm_yday = CVT_ASCII_TO_INT(buffer[7]) * 100 +
		     CVT_ASCII_TO_INT(buffer[8]) * 10 +
		     CVT_ASCII_TO_INT(buffer[9]);

    /*
     * Convert bytes 11 and 12 into hours
     */
    timetm.tm_hour = CVT_ASCII_TO_INT(buffer[11]) * 10 +
		     CVT_ASCII_TO_INT(buffer[12]);

    /*
     * Convert bytes 14 and 15 into minutes
     */
    timetm.tm_min = CVT_ASCII_TO_INT(buffer[14]) * 10 +
		    CVT_ASCII_TO_INT(buffer[15]);

    /*
     * Convert bytes 15 and 16 into seconds
     */
    timetm.tm_sec = CVT_ASCII_TO_INT(buffer[17]) * 10 +
		    CVT_ASCII_TO_INT(buffer[18]);

    /*
     * Convert bytes 20, 21 and 22 into nanoseconds
     */
    tns = (CVT_ASCII_TO_INT(buffer[20]) * 100 +
	   CVT_ASCII_TO_INT(buffer[21]) * 10 +
	   CVT_ASCII_TO_INT(buffer[22])) * K_NS_PER_MS;

    /*
     * Compute month and day of month
     */
    leapyear = ((timetm.tm_year % 4 == 0) && (timetm.tm_year % 100 != 0)) ||
               (timetm.tm_year % 400 == 0);
    timetm.tm_mday = timetm.tm_yday;
    for (i=1; timetm.tm_mday > daytab[leapyear][i]; i++)
	timetm.tm_mday -= daytab[leapyear][i];
    timetm.tm_mon = i-1;

    /*
     * Check values
     */
    if (timetm.tm_sec > 60 ||			/* Leap seconds */
        timetm.tm_min > 59 ||
	timetm.tm_hour > 23 ||
	timetm.tm_yday > 366) {
	BEGIN_NONREENTRANT
	(void) fprintf(stderr, "dtsprovider: radio clock fault\n");
	(void) fprintf(stderr, "time returned - %26s\n", buffer);
	END_NONREENTRANT
	return(1);
    }

    /*
     * Convert the data returned by radio clock into a utc_t.
     * Call the UTC API
     */
    return(utc_mkgmtime(utcTime, &timetm, tns, &INACC_TM, ins));

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
	(void) printf("K_TPI_FAILURE\n");
        (void) printf("******************************************\n");
	return;
    }

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
