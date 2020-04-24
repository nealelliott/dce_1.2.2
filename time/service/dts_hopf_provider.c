/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: dts_hopf_provider $Revision: 1.1.10.2 $ $Date: 1996/02/17 23:35:22 $
 */
/*
 * HISTORY
 * $Log: dts_hopf_provider.c,v $
 * Revision 1.1.10.2  1996/02/17  23:35:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:36  marty]
 *
 * Revision 1.1.10.1  1995/12/08  18:13:19  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  13:28 UTC  pare
 * 	Merge from Hpdceo2
 * 	[1995/12/08  17:25:31  root]
 * 
 * Revision 1.1.7.3  1994/07/20  20:24:38  cbrooks
 * 	CR10945 fix Time Provider errors
 * 	[1994/07/20  18:25:50  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  12:59 UTC  pare
 * 	Merge to HPDCE02
 * 
 * Revision 1.1.7.2  1994/05/18  19:49:13  tom
 * 	Bug 10627 - Put ourselves into the background.
 * 	[1994/05/12  22:41:53  tom]
 * 
 * Revision 1.1.7.1  1994/04/28  21:08:24  cbrooks
 * 	Fix select parameters
 * 	[1994/04/27  19:40:55  cbrooks]
 * 
 * 	Code Cleanup; ANSI'fied; use termios
 * 	[1994/04/26  15:43:16  cbrooks]
 * 
 * 	DTS Code Cleanup
 * 	[1994/04/25  18:40:55  cbrooks]
 * 
 * 	DTS Code Cleanup
 * 
 * Revision 1.1.8.2  1993/07/08  20:08:30  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:07:58  root]
 * 
 * Revision 1.1.6.4  1993/05/28  13:15:07  truitt
 * 	Corrected bonehead mistake in version string.
 * 	[1993/05/28  13:14:32  truitt]
 * 
 * Revision 1.1.6.3  1993/04/01  18:31:56  truitt
 * 	Replace sgtty structure with termios structure.
 * 	Replace sgtty.h include file with termios.h include file.
 * 	[1993/04/01  18:31:23  truitt]
 * 
 * Revision 1.1.6.2  1993/03/24  14:31:00  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/24  14:30:11  truitt]
 * 
 * 	Change comments to match the filename that should be compiled.
 * 	[1993/01/18  20:21:49  jwade]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  22:20:23  htf]
 * 
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:12:52  marcyw]
 * 
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:06:17  sekhar]
 * 
 * Revision 1.1.1.8  1993/03/11  22:34:23  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.5  1993/03/03  15:47:45  truitt
 * 	Remove include os termios.h and add define of TCIOFLUSH.
 * 	Set up proper cast types for the arguments in the select
 * 	command.  Used int instead of fd_set.
 * 	[1993/03/03  15:47:20  truitt]
 * 
 * Revision 1.1.3.4  1992/12/22  00:22:49  smythe
 * 	added new OSF examples disclaimer comment for OSF OT 4998 fix
 * 	[1992/12/21  21:17:33  smythe]
 * 
 * Revision 1.1.2.2  1992/06/24  16:23:02  grober
 * 	HP/UX Porting Changes
 * 	[1992/06/22  15:56:08  grober]
 * 
 * Revision 1.1.3.3  1992/06/19  19:37:14  smythe
 * 	added include of termios.h to define TCIOFLUSH
 * 	[1992/06/19  19:35:58  smythe]
 * 
 * Revision 1.1.3.2  1992/06/18  23:58:17  smythe
 * 	added hpux changes for ioctl TIOCEXCL and ioctl TIOCFLUSH calls
 * 	[1992/06/18  23:56:18  smythe]
 * 
 * Revision 1.1  1992/01/19  15:34:39  devrcs
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
 *	Module dts_hopf_provider.c
 *	Version V1.0
 */
/*
 * Copyright (c) 1991 by
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
 *		time from a Hopf radio clock connected to a
 *		terminal line.
 *
 * ENVIRONMENT:	OSF Distributed Computing Environment
 *
 * AUTHORS:	CEC Karlsruhe
 *		Digital Distributed Time Service (DECdts).
 *
 * DATE:	March 12, 1991
 *
 */

/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */

/*
 *                              DESCRIPTION OF THE
 *				 HOPF RADIO CLOCK
 * 
 *	The HOPF Model 6020 receiver is a radio clock which receives
 *	DCF77 (near Frankfurt, Germany).
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
 *       % cc dts_hopf_provider.c dtsprovider_sstub.c -ldce -o dtsprovider
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
				/* compiler from dtsprovider.idl */
#include <dce/dce_error.h>	/* rpc error messages */
#include <dce/dce_cf.h>
#include <dts_provider_util.h>
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
 * Hopf character conversion macros
 */
#define CVT_ASCII_TO_INT(chr) (isdigit(chr) ? \
	((chr) - '0') : (((chr) - 'A') + 10))
#define CVT_ASCII_TO_INT_AND_ADD(chrhi, chrlo) \
	((CVT_ASCII_TO_INT(chrhi) * 10) + CVT_ASCII_TO_INT(chrlo))


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

#define M_HOPF_FLAG_STX		'\002'	/* 'STX' header flag		*/
#define M_HOPF_FLAG_ETX		'\003'	/* 'ETX' trailer flag		*/

#define M_HOPF_TIME_RADIO	0x08	/* Time availability		*/
#define M_HOPF_TIME_CLOCK	0x04	/* Internal clock mode		*/
#define M_HOPF_DST_IND		0x02	/* DST indicator		*/
#define M_HOPF_DST_ANN		0x01	/* DST announcement		*/

#define M_HOPF_UTC_IND		0x08	/* UTC indicator		*/
#define M_HOPF_WEEK_DAY		0x07	/* day of week			*/

#define HOPF_PROMPT		"g02"

#define MAX_RESPONSE_LENGTH	(100)	/* Length of maximum response	*/
#define MIN_RESPONSE_LENGTH	(18)	/* Length of minimum valid resp.*/

#define K_HOPF_TIMEOUT		(5)	/* Allow 5 seconds for radio	*
					 * clock to respond		*/
#define K_HOPF_RETRY		(2)	/* Retry 2 times		*/

#define HOPF_BAUD		(B9600)	/* Baud rate of Hopf term.	*/

/*
 *  Maximum/Minimum parameters for TP
 */
#define K_MIN_INACCURACY     (30 + 1 * K_MS_PER_SEC)
					/* Allow for at least 1 sec of	*
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
#define K_DEF_INACCURACY     (100 + 1 * K_MS_PER_SEC)
					   /* 100 ms inacc. in each	*
					    * radio clock response plus	*
					    * 1 sec inacc in interface  */
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

int main(int , char	*argv[] );

void ContactProvider( handle_t , TPctlMsg *, error_status_t *);
 
void ServerRequestProviderTime( handle_t , TPtimeMsg *, error_status_t *);

void CommandLineInput(int , char *argv[] );

void InterActiveInput(void);

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
    int i;
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

     cfsetospeed(&ttyb, HOPF_BAUD);    /* set output speed */
     cfsetispeed(&ttyb, HOPF_BAUD);    /* set input speed */
     ttyb.c_iflag &= (IXOFF | IXANY);  
     ttyb.c_oflag &= ~OPOST;           /* no output processing */
     ttyb.c_lflag &= ~(XCASE | ISIG | ICANON | ECHO); /* raw mode */
     ttyb.c_cflag &= ~CSIZE;
     ttyb.c_cflag |= CS8;                 /* 8 bits */
     ttyb.c_cflag &= ~(PARENB | PARODD);  /* no parity */
     ttyb.c_cc[VTIME] = 0;                /* wait forever */
     ttyb.c_cc[VMIN] = 1;                 /* return on each character */
     tcsetattr(providerFD, TCSANOW, &ttyb); /* make change now */ 

    if (!TRACING)
    {
        daemon(CHDIR, NOCLOSEFD);	/* detach program from tty */
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
		     (unsigned_char_p_t)"Hopf Time Provider",
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

}

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
 *			Hopf radio clock control
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

    retry = K_HOPF_RETRY;

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
    int status;
    struct timeval timeout;		/* time out value for select       */
    char dataLine[MAX_RESPONSE_LENGTH];	/* Response buffer		   */
    fd_set readfds;			/* mask of file descriptors        */
    int	n;				/* file descriptor selected on     */
    int nfds = fd+1;                    /* number of file descripts to select */

    /*
     * Flush input and output queues
     */
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
    timeout.tv_sec = K_HOPF_TIMEOUT;
    timeout.tv_usec = 0;

    /*
     * Record 'before' timestamp
     */
    (void) utc_gettime(beforeTime);

    /*
     * Write 'Query' command to radio clock
     */
    if (write(fd, HOPF_PROMPT, (sizeof(HOPF_PROMPT)-1)) !=
	(sizeof(HOPF_PROMPT)-1)) {
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

    if (n == 0) {
	BEGIN_NONREENTRANT
	(void) fprintf(stderr, "\n\t* Timeout *\n");
	END_NONREENTRANT
	return(1);
    }

    if (read(fd, dataLine, sizeof(dataLine)) < MIN_RESPONSE_LENGTH) {
	PrintError("dtsprovider: radio clock returned bad data");
	exit(1);
    }

    /*
     * Record 'after' timestamp
     */
    (void) utc_gettime(afterTime);

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
    struct tm timetm;
    int status1, status2;

    /*
     * Check message header
     */
    if (buffer[0] != M_HOPF_FLAG_STX) {
	TraceIf( (void) printf("dtsprovider: Hopf returned bad header\n"); );
	return(1);
    }

    /*
     * Check message trailer
     */
    if ((buffer[15] != '\n') ||
	(buffer[16] != '\r') ||
	(buffer[17] != M_HOPF_FLAG_ETX)) {
	TraceIf( (void) printf("dtsprovider: Hopf returned bad trailer\n"); );
	return(1);
    }

    /*
     * Convert and parse status bytes
     */
    status1 = CVT_ASCII_TO_INT(buffer[1]);
    status2 = CVT_ASCII_TO_INT(buffer[2]);

    /*
     * Perform various sanity checks
     */

    if ( !(status1 & M_HOPF_TIME_RADIO) ) {
	TraceIf( (void) printf("dtsprovider: Hopf radio time not available\n"); );
	return(1);
    }

    if ( !(status1 & M_HOPF_TIME_CLOCK) ) {
	TraceIf( (void) printf("dtsprovider: Hopf internal clock time not available\n"); );
	return(1);
    }

    if ( !(status2 & M_HOPF_UTC_IND) ) {
	TraceIf( (void) printf("dtsprovider: Hopf no UTC time\n"); );
	return(1);
    }

    /*
     * Convert bytes 3 and 4 into hours
     */
    timetm.tm_hour = CVT_ASCII_TO_INT_AND_ADD(buffer[3], buffer[4]);

    /*
     * Convert bytes 5 and 6 into minutes
     */
    timetm.tm_min = CVT_ASCII_TO_INT_AND_ADD(buffer[5], buffer[6]);

    /*
     * Convert bytes 7 and 8 into seconds
     */
    timetm.tm_sec = CVT_ASCII_TO_INT_AND_ADD(buffer[7], buffer[8]);

    /*
     * Convert bytes 9 and 10 into day of month
     */
    timetm.tm_mday = CVT_ASCII_TO_INT_AND_ADD(buffer[9], buffer[10]);

    /*
     * Convert bytes 11 and 12 into month
     */
    timetm.tm_mon = CVT_ASCII_TO_INT_AND_ADD(buffer[11], buffer[12]) - 1;

    /*
     * Convert bytes 13 and 14 into year - 1900
     */
    timetm.tm_year = CVT_ASCII_TO_INT_AND_ADD(buffer[13], buffer[14]);

    /*
     * Check values
     */
    if (timetm.tm_sec > 60 ||			/* Leap seconds */
        timetm.tm_min > 59 ||
	timetm.tm_hour > 23 ||
	timetm.tm_mday > 31 ||
	timetm.tm_mon > 11) {
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
    return(utc_mkgmtime(utcTime, &timetm, 0, &INACC_TM, INACC));

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
