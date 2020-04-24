/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * @HP_DCE_VERSION@
 * @(#)Module: dts_acts_provider $Revision: 1.1.10.2 $ $Date: 1996/02/17 23:35:20 $
 */
/*
 * HISTORY
 * $Log: dts_acts_provider.c,v $
 * Revision 1.1.10.2  1996/02/17  23:35:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:34  marty]
 *
 * Revision 1.1.10.1  1995/12/08  18:13:17  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1994/12/13  21:25 UTC  pare
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1994/12/13  21:21 UTC  pare
 * 	Change second argument of daemon() from NOCLOSEFD to CLOSEFD.
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/26  19:15 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:25:29  root]
 * 
 * Revision 1.1.7.4  1994/08/23  20:32:22  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:59  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  13:22 UTC  pare
 * 	Merged in Hpdce02 changes
 * 
 * Revision 1.1.7.3  1994/07/20  20:24:36  cbrooks
 * 	CR10945 fix Time Provider errors
 * 	[1994/07/20  18:25:46  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  18:06 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.7.2  1994/05/18  19:49:11  tom
 * 	Bug 10627 - Put ourselves into the background.
 * 	[1994/05/12  22:41:50  tom]
 * 
 * Revision 1.1.7.1  1994/04/28  21:08:20  cbrooks
 * 	DTS Code Cleanup
 * 	[1994/04/28  17:33:05  cbrooks]
 * 
 * 	Fix select parameters
 * 	[1994/04/27  19:40:53  cbrooks]
 * 
 * 	Code Cleanup; ANSI'fied; use termios
 * 	[1994/04/26  15:43:13  cbrooks]
 * 
 * Revision 1.1.8.2  1993/07/08  20:06:56  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:06:20  root]
 * 
 * Revision 1.1.6.3  1993/05/28  13:15:04  truitt
 * 	Corrected bonehead mistake in version string.
 * 	[1993/05/28  13:13:57  truitt]
 * 
 * Revision 1.1.6.2  1993/03/24  14:24:36  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/24  14:23:56  truitt]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  22:20:16  htf]
 * 
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:12:45  marcyw]
 * 
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:06:07  sekhar]
 * 
 * Revision 1.1.1.8  1993/03/11  22:34:21  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.3.5  1993/03/02  19:53:19  truitt
 * 	Eliminate include of sgtty.h for hpux builds.
 * 	Set up proper cast types for the arguments to the
 * 	select command.  Used int instead of fd_set
 * 	[1993/03/02  19:52:49  truitt]
 * 
 * Revision 1.1.3.4  1992/12/22  00:22:44  smythe
 * 	added new OSF examples disclaimer comment for OSF OT 4998 fix
 * 	[1992/12/21  21:14:08  smythe]
 * 
 * Revision 1.1.3.3  1992/06/18  21:13:57  smythe
 * 	promoting previous changes to source tree
 * 	[1992/06/18  21:13:31  smythe]
 * 
 * Revision 1.1.2.2  1992/06/24  16:21:22  grober
 * 	HP/UX Porting Changes
 * 	[1992/06/22  15:55:19  grober]
 * 
 * Revision 1.1.3.2  1992/06/05  18:52:11  hopkins
 * 	include sys/file.h for FNDELAY if HPUX
 * 	[1992/06/05  15:50:29  hopkins]
 * 
 * Revision 1.1  1992/01/19  15:34:14  devrcs
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
 *	Module dts_acts_provider.c
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
 * ABSTRACT:    Implements a DECdts Time Provider (using RPC) which dials
 *		the National Institute of Standards and Technology using
 *		a Scholar Plus modem (DF242) or Hayes compatible modem
 *		at 1200 baud, and obtains the current UTC
 *
 * ENVIRONMENT:	OSF Distributed Computing Environment
 *
 * AUTHORS:     Digital Distributed Time Service (DECdts).
 *		Distributed Processing Engineering (DPE).
 *                
 * DATE:	May 2, 1990
 *
 */

/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */

/*
 *                              DESCRIPTION OF THE
 *                  AUTOMATED COMPUTER TELEPHONE SERVICE (ACTS)
 * 
 * 
 * The following is transmitted (at 1200 baud) after completion of the
 * telephone connection.
 * 
 *              ? = HELP
 *              National Bureau of Standards
 *              Telephone Time Service
 * 
 *                                      D  L D
 *               MJD  YR MO DA H  M  S  ST S UT1 msADV         OTM
 *              47222 89-03-02 21:39:15 83 0 +.3 045.0 UTC(NIST) *
 *              47222 89-03-02 21:39:16 83 0 +.3 045.0 UTC(NIST) *
 *              47222 89-03-02 21:39:17 83 0 +.3 045.0 UTC(NIST) *
 *              47222 89-03-02 21:39:18 83 0 +.3 045.0 UTC(NIST) *
 *              47222 89-03-02 21:39:19 83 0 +.3 037.6 UTC(NIST) #
 *              47222 89-03-02 21:39:20 83 0 +.3 037.6 UTC(NIST) #
 *              etc..etc...etc.......
 * 
 * UTC = Universal Time Coordinated, the official world time referred to the
 * zero meridian.
 * _________________________________________________________________________
 * 
 * DST = Daylight savings time characters, valid for most of the continental 
 * U.S., are set as follows:
 *   00 = We are on standard time (ST).    50 = We are on DST.
 *   99 to 51 = Now on ST, go to DST when your local time is 2:00 am and the
 *     count is 51.  The count is decremented daily at 00 (UTC).
 *   49 to 01 = Now on DST, go to ST when your local time is 2:00 am and the
 *     count is 01.  The count is decremented daily at 00 (UTC).
 * The two DST characters provide up to 48 days advance notice of a change in
 * time.  The count remains at 00 or 50 at other times.
 * _________________________________________________________________________
 * 
 * LS = Leap second flag is set to "1" to indicate that a leap second is to be
 * added as 23:59:60 (UTC) on the last day of the current UTC month.  The LS
 * flag will be reset to "0" starting with 23:59:60 (UTC).  The flag will
 * remain on for the entire month before the second is added.  Leap seconds
 * are added as needed at the end of any month.  Usually June and/or December
 * are chosen.
 * 
 *  The leap second flag will be set to a "2" to indicate that a leap second
 * is to be deleted at 23:59:58--00:00:00 on the last day of the current
 * month. (This latter provision is included per international recommendation
 * however it is not likely to be required in the near future.)
 * __________________________________________________________________________
 * 
 * DUT1 = Approximate difference between earth rotation time (UT1) and UTC, in
 * steps of 0.1 second.         DUT1 = UT1 - UTC
 * ___________________________________________________________________________
 * 
 * MJD = Modified Julian Date, often used to tag certain scientific data.
 * ___________________________________________________________________________
 * 
 * The full time format is sent at 1200 baud, 8 bit, 1 stop, no parity.
 * The HH:MM:SS msADV time format at 300 baud is also 8 bit, 1 stop, no parity. 
 * ___________________________________________________________________________
 * 
 * Maximum on line time will be 55 seconds.  If all lines are busy at any time,
 * the oldest call will be terminated if it has been on line more than 15
 * seconds, else, the call that first reaches 15 seconds will be terminated. 
 * ___________________________________________________________________________
 * 
 * Current time is valid at the "on-time" marker (OTM), either "*" or "#". 
 * The nominal on-time marker (*) will be transmitted 45 ms early to account
 * for the 8 ms required to send 1 character at 1200 baud, plus an additional
 * 7 ms for delay from NIST to the user, and approximately 30 ms "scrambler"
 * delay inherent in 1200 baud modems.  For the user who wants
 * the best possible accuracy at the OTM, NIST offers an alternate 300 baud
 * service with only HH:MM:SS MSADV and OTM. To use the alternate service,
 * simply call at 300 baud.  Because of the simple FSK modulation scheme used
 * at 300 baud, all modems tested had the same delay within about 1 ms.
 * ___________________________________________________________________________
 * 
 * For user comments write:
 * NIST-ACTS
 * Time and Frequency Division
 * Mail Stop 524
 * 325 Broadway
 * Boulder, CO  80303
 * 
 * --------------------------------------------------------------------------
 */

/* --------------------------------------------------------------------------
 *    Compiling and Running the time provider.
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
 *       % cc dts_acts_provider.c dtsprovider_sstub.c -ldce -o dtsprovider
 *
 *    To run the program interactively, simply type 'dtsprovider'
 *
 *	% dtsprovider
 *
 *	The provider will then ask you to enter the following parameters:
 *
 *	1. The name of the device to which the modem is connected,  
 *	   '/dev/tty02' for example.
 *	2. The poll period, which is the number of seconds between
 *	   dialups.  Entering 86400 for example would cause the provider
 *	   to dial-out every 24 hours.
 *	3. The Base inaccuracy, which is the number of milliseconds of 
 *	   systematic inaccuracy in the timestamps delivered by NIST.
 *	   The default value is 45 ms.  If the times returned by the
 *	   provider are systematically incorrect, then the base
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
 *	7. The default phone number is a valid telephone dial string
 *         preceded by either a T, for tone dialing or a P ro Pulse dialing.
 *
 *		T1-303-494-4774  (dashes are permitted).
 *              P1-303-494-4774
 *
 *	The provider can also be run in the background using the command
 *	line interface:
 *
 *		% dtsprovider -n T13034944774 -p 86400 &
 *
 *	To view the valid command line arguments start the provider
 *	interactively and then exit. The arguments will displayed.
 */

/* Include Files: */

#include <pthread.h>

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <termios.h>
#include <errno.h>
#include <dce/utc.h>		/* utc library routines */
#include <dce/dtsprovider.h>	/* This file is generated by the NIDL */
				/* compiler from dtsprovider.idl */
#include <dce/dce_error.h>	/* rpc error messages */
#include <dce/dce_cf.h>

#include <sys/file.h>         /* define FNDELAY */
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
 *	Conversion factors
 */

#define K_NS_PER_SEC	(1000000000)
#define K_MS_PER_SEC	(1000)
#define K_NS_PER_MS	(K_NS_PER_SEC / K_MS_PER_SEC)
#define K_100NS_PER_MS  (10000)

/*
 *	Literals
 */

#define K_DIAL_ATT_TMO   (120)  /* allow 120 seconds for modem to attach  *
				 * to phone call.			 */
#define K_ACTS_1200_SIZE (50)	/* size of the ACTS data string		 */
#define K_ACTS_1200_OTM_POS (K_ACTS_1200_SIZE - 1)
/* char position of the '*' or '#' on    */
/* time marker.				 */

/*
 *	Modem Dial Command
 */
#define HAYES_DIAL_COMMAND  "ATDT "
#define DF242_DIAL_COMMAND  "\001D "

/*
 *  Maximum/Minimum parameters for TP
 */
#define K_MIN_INACCURACY     (30)	/* Allow for at least 30 ms of	*
					 * systematic inaccuracy	*
					 * in each ACTS time response.	*/
#define K_MAX_INACCURACY     (60 * K_MS_PER_SEC)
					/* Allow for at most 1 minute	*
					 * of inacc in each ACTS resp.	*/
#define K_MIN_POLLRATE	     (60)	/* Minimum time between synchs.	*/
#define K_MAX_POLLRATE       (31*24*60*60)
					/* Maximum time between synchs.	*/

#define K_MAX_NUMBER         (26)	/* largest dial number modem    *
					 * can use.			*/
#define K_MAX_DEV_NAME	     (255)	/* largest device name string.	*/

/*
 *	Default Control/Config Values
 */
#define K_DEF_INACCURACY     (45)          /* 45 ms inacc. in each	*
					    * ACTS Time Response	*/
#define K_DEF_TMORATE	     (2 * 60)	   /* Let service wait 2 minutes*
					    * for dialup connect to	*
					    * succeed.			*/
#define K_DEF_POLLRATE	     (24*60*60)	   /* dial out each 24 hours	*
					    * to get a new time		*/
#define K_DEF_TIMESTAMPS     (4)	   /* Read the ACTS clock	*
					    * 4 times each synch.	*/
#define K_DEF_NOCLOCKSET     (0)	   /* Allow the clock to	*
					    * be set.			*/
#define K_DEF_TRACING	     (0)	   /* Time Provider tracing	*/

#define K_DEF_PHONE_NUM	    "T13034944774"  /* Acts dial out		*/

#define K_DEF_MODEM_TYPE    "Hayes"         /* Default Modem type */

#define K_DEF_DIAL_COMMAND   HAYES_DIAL_COMMAND

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
	   (void)fprintf(stderr, "%s - %s\n",           \
                         (message), _rpcmsg); \
           END_NONREENTRANT				\
           if ((code) == ABORT)                         \
	       exit(1);                                 \
	}                                               \
   }


/*
 *	Forward Definitions
 */

void ContactProvider( handle_t , TPctlMsg    * , error_status_t * );
 
void ServerRequestProviderTime( handle_t , TPtimeMsg   * , error_status_t * );

void CommandLineInput(int , char *[] );

int Delay (unsigned );
void PrintError (char * );
void PrintHelp(void);

int ReadTimes(unsigned long  , TPtimeMsg * );
int ReadNewLine(int  );
int ReadTimedLine(int  , utc_t * , utc_t * , utc_t * );

int ParseTime (utc_t * , char  * );

void PrintTimes(TPtimeMsg * );
void PrintISO(utc_t * );

void InterActiveInput(void) ;

int main( int, char *[]) ;

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
                                             /* max read failures from modem*/
unsigned long MAX_READ_FAILURES = 3 * K_DEF_TIMESTAMPS; 

unsigned long  TMORATE   = K_DEF_TMORATE;    /* provider response timeout */

unsigned long  POLLRATE  = K_DEF_POLLRATE;   /* seconds between TP syncs*/

unsigned long  NOCLOCKSET= K_DEF_NOCLOCKSET; /* clock set               */

unsigned long  TRACING   = K_DEF_TRACING;    /* output tracing          */

unsigned long  INACCURACY= K_DEF_INACCURACY; /* provider inaccuracy	*/

unsigned long  INACC;			 /* inacc in nanoseconds	*/

struct tm INACC_TM;			 /* inaccuracy of each time reading */

char ACTSnum[K_MAX_NUMBER+1];            /* ACTS dialup number		*/

char ModemCmd[K_MAX_NUMBER+1];		 /* Modem dial command		*/


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
    
    int           numThreads;       /* Max number of threads created */
    uuid_t        TP_uuid;          /* UUID for this interface */
    unsigned32    maxCalls;         /* Max # of concurrent calls this */
                                    /* server can receive.            */
    int i;
    unsigned32    status;
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
		     (unsigned_char_p_t)"ACTS Time Provider",
		     &status);
    ProcessRPCerror (status, "Can't register with endpoint mapper", ABORT);


    /*
     * Obtain the name of the machine's principal and register an 
     * authentication service to use for authenticated remote procedure
     * calls coming from the time service daemon.  HostName is NULL, so
     * this routine implicitely gets the host name and returns the
     * machine's principal name.
     */

    dce_cf_prin_name_from_host ((char *)NULL, 
				(char **)&machinePrincipalName, 
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
 *      DTS TP/TS REMOTE PROCEDURE CALL (RPC) ROUTINES
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
     * Read the times from ACTS.  Give the child some time
     * to complete the operation.
     */
    errStatus = ReadTimes(TIMESTAMP, timeResponse);

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

    /*
     * Copy in the default phone number and command
     */
    (void)strcpy(ACTSnum, K_DEF_PHONE_NUM);
    (void)strcpy(ModemCmd, K_DEF_DIAL_COMMAND);
    
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
		    MAX_READ_FAILURES = 3 * TIMESTAMP;
		    break;
		}

		case 'm': {
		    GETNEXTARG(argc);
		    if (!strcasecmp(argv[i], "hayes"))
			(void)strcpy(ModemCmd, HAYES_DIAL_COMMAND);
		    else if (!strcasecmp(argv[i], "scholar"))
			(void)strcpy(ModemCmd, DF242_DIAL_COMMAND);
		    else
		    {
			(void) fprintf(stderr, "-m:Bad modem name\n");
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

		case 'n': {
		    GETNEXTARG(argc);
		    (void)strncpy(ACTSnum, argv[i], K_MAX_NUMBER);
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
    char modemType[10];
    
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
     * Get the timestamp count and set the maximum modem read failures we 
     * area allowed to have.
     */
    (void) printf("Enter the Timestamp Count[%d]: ", TIMESTAMP);
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%d",&TIMESTAMP);
    if (TIMESTAMP < K_MIN_TIMESTAMPS || TIMESTAMP > K_MAX_TIMESTAMPS)
    {
	(void) fprintf(stderr, "-t:Time stamp count is out of legal range\n");
	exit(1);
    }
    else
	MAX_READ_FAILURES = 3 * TIMESTAMP;

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
    
    /* 
     * Get the phone number
     */
    (void) strcpy(ACTSnum, K_DEF_PHONE_NUM);
    (void) printf("Enter the telephone number[%s]: ", ACTSnum);
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%26s", ACTSnum);

    /* 
     * Get the modem type
     */

    (void) strcpy(modemType, K_DEF_MODEM_TYPE);
    (void) printf("Enter the modem type (Hayes or Scholar)[%s]: ", "Hayes");
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%26s", modemType );
           if (strncasecmp(modemType, "scholar", 5) == 0)
	       (void)strcpy(ModemCmd, DF242_DIAL_COMMAND);
	   else if (strncasecmp(modemType, "hayes", 5) == 0)
	       (void)strcpy(ModemCmd, K_DEF_DIAL_COMMAND);
           else
	   {
	       (void) fprintf(stderr, "-m: Modem type not supported\n");
	       exit(1);
	   }



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
		   "\t\t[-n Telephone Number, default : %s ]\n",
		   K_DEF_PHONE_NUM);
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
    (void) fprintf(stderr,
     "\t\t[-m modem_name, range Hayes : Scholar, default hayes]\n");

}
/* End of routine PrintHelp */

/*****************************************************************
 *
 *	Device Dependent code	- 
 *			Modem Control
 *			NIST/ACTS Parser
 *
 *****************************************************************/
/*
 *++
 *  ReadTimes()
 *
 *  Functional Description:
 *
 *	Parse out time stamps from the modem input.  The processing
 *	is done within the child sub-process.
 *  
 *  Inputs:
 *
 *	timeStamps - the number of timestamps to read
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
int ReadTimes(unsigned long timeStamps ,
               TPtimeMsg *tpTimeMsg )

{
    int fd;			    /* file descriptor to modem */
    int bufSize;                    /* temp, acts dial number buffer */
    struct termios termChar;        /* terminal characteristics */
    struct termios saveTermChar;    /* save terminal characteristics */
    int i, j;                       /* temp, index */
    int errStatus;                  /* success or failure status */
    char dialNumber[K_MAX_NUMBER + 1];  /* number to dial */
    
    /*
     * Build up the dial command
     */
    (void)strncpy(dialNumber, ModemCmd, K_MAX_NUMBER);
    (void)strncat(dialNumber, ACTSnum, K_MAX_NUMBER-4);
    bufSize = (int)strlen(dialNumber);
    dialNumber[bufSize] = '\r';
    bufSize = bufSize + 1;
    
    /*
     * open the port for read/write and do not block on the open
     */
    if ((fd = open(devicename, O_RDWR  | O_NDELAY, 0)) < 0)
    {
        char errorString[256];
	BEGIN_NONREENTRANT
	(void)sprintf(errorString, "%s: 200%s",
		      "dtsprovider: can not open device", devicename);
	END_NONREENTRANT
	PrintError(errorString);
	return(1);
    }
    
    /*
     * Save the current terminal characteristics
     */
    BEGIN_NONREENTRANT
    errStatus = tcgetattr(fd, &saveTermChar);
    END_NONREENTRANT
    if (errStatus < 0)
    {
	PrintError("dtsprovider: get attr ");
	return(1);
    }
    
    /*
     * Set the appropriate terminal characteristics 
     */
    termChar = saveTermChar;
    
    termChar.c_iflag |= IGNBRK | PARMRK;
    termChar.c_iflag &= ~(ISTRIP|BRKINT);

    termChar.c_oflag &= ~(OPOST);
    
    termChar.c_cflag &= ~(CSIZE | CSTOPB | PARENB);
    termChar.c_cflag |= (CS8 | CLOCAL | CREAD); 

    termChar.c_lflag &= ~(ISIG|ECHO|ICANON);

    termChar.c_cc[VMIN] = 0;
    termChar.c_cc[VTIME]= 100;
    
    (void)cfsetispeed(&termChar, B1200);
    (void)cfsetospeed(&termChar, B1200); 

    BEGIN_NONREENTRANT
    errStatus = tcsetattr(fd, TCSANOW, &termChar);
    END_NONREENTRANT
    if (errStatus < 0)
    {
	PrintError("dtsprovider: can not set the terminal characteristics");
	return(1);
    }

    /*
     * Wait for things to settle.
     */
    if (Delay (1))
    {
	PrintError("dtsprovider: cannot delay after setting term. chars.");
	return (1);
    }
    
    /*
     * dial the acts number
     *
     */
    if ( write(fd, dialNumber, bufSize) != bufSize )
    {
	PrintError("dtsprovider: error writing to device ");
	return(1);
    }

    /*
     * wait a second for the dial to go through
     */
    if (Delay (1))
    {
	PrintError("dtsprovider: cannot delay waiting for dial");
	return (1);
    }

    /*
     * Make the socket non-blocking
     */
    if (fcntl(fd, F_SETFL, FNDELAY) < 0) {
	PrintError("dtsprovider: error setting line to non-blocking");
        return(1);
    }

    
    /*
     * Read away all the header information sent by NIST
     * if an error has occurred in ReadNewLine(), return 1.
     * 
     */
    for (i=0; i < 5; i++){
	errStatus = ReadNewLine(fd);
	if (errStatus)
	    return(1);
    }

    /*
     * Now Read The Time Stamps.
     * Note that this loop will terminate on read failures and thus
     * is non-infinite.  A read failure will occur if the phone is
     * hung up.  NIST will hang up the phone after a maximum of
     * 55 seconds.
     */
   
    i = j = 0;
    while ( (i < timeStamps) && (j < MAX_READ_FAILURES)) {
	errStatus = ReadTimedLine(fd, &tpTimeMsg->timeStampList[i].TPtime,
			  &tpTimeMsg->timeStampList[i].beforeTime, 
			  &tpTimeMsg->timeStampList[i].afterTime);
	switch(errStatus){
	    case 0 :    i += 1;
	                break;

	    case -1:    if (j == MAX_READ_FAILURES)
		           return(1);
	                else 
			    j += 1;
	                break;

	    case 1 :    return(1);
	}
    }

   
    /*
     * Now Hang up the phone by setting the baud rate to 0.
     */
    termChar = saveTermChar;
    BEGIN_NONREENTRANT
    errStatus = cfsetispeed(&termChar, B0);
    END_NONREENTRANT
    if (errStatus < 0) {
	PrintError("dtsprovider: error setting input modem speed");
	return(1);
    }
    BEGIN_NONREENTRANT
    errStatus = cfsetospeed(&termChar, B0);
    END_NONREENTRANT
    if (errStatus < 0) {
	PrintError("dtsprovider: error setting output modem speed");
	return(1);
    }
    BEGIN_NONREENTRANT
    errStatus = tcsetattr(fd, TCSANOW, &termChar);
    END_NONREENTRANT
    if (errStatus < 0)
    {
	PrintError("dtsprovider: error disconnecting modem");
	return(1);
    }
    
    /*
     * close the socket to the modem
     */
    (void)close(fd);
    
    return(0);
}    

/*
 *++
 *  ReadNewLine()
 *
 *  Functional Description:
 *
 *	Keep reading until a new line character is reached.  The last
 *	character transmitted by NIST for each time stamp is an LF.
 *  
 *  Inputs:
 *
 *	fd - file descriptor to read.
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

typedef void fd_set_t ;

int ReadNewLine(int fd )
{
    char   buffer;
    int    bytes;
    int    num_found;
    fd_set rfds;
    struct timeval timeout;
    int    nfds;


/*
 * examine a maximum of fd+1 file descriptors in the select mask, since
 * only one descriptor at a time will be ready for read.
 */
    nfds = fd+1;

    timeout.tv_sec = 60;
    timeout.tv_usec = 0;

    for(;;)
    {
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);

#ifdef __hpux
	num_found = select(nfds, (int *)&rfds, 0, 0, &timeout);
#else
	num_found = select(nfds, (fd_set_t *)&rfds, (fd_set_t *)0, 
			   (fd_set_t *)0, &timeout);
#endif /* #ifdef __hpux */

	/*
	 * if select() timed-out because of a socket closing on the other side
	 * or no data available, return an error.
	 */
        if ( num_found == 0 ){
	    PrintError("dtsprovider: error reading data - select timed out\n");
	    return(1);
	}

	else if (num_found == -1){
	    if (errno != EINTR)
	    {
		PrintError("dtsprovider: error reading data - select error ");
		return(1);
	    }
	    else
		continue;
	}
	
	if ((bytes=read(fd, &buffer, 1))  > 0) {
	    if (buffer == '\n') {
		break;
	    }
	    else if (bytes==0)
		continue;
	    else if (bytes<0 && errno != EINTR ) {
		PrintError("dtsprovider: error reading data - line read ");
		return(1);
	    }
	}
    }
    
    return (0);    /* success */
}

/*
 *++
 *  ReadTimedLine()
 *
 *  Functional Description:
 *
 *	Read a single timestamp from ACTS. Generate the timestamp triplet
 *	waiting on the on time marker '*' or '#' (see description of
 *	on time marker in NIST comments on page 2).
 *  
 *  Inputs:
 *
 *	fd - file descriptor to read.
 *
 *  Outputs:
 *
 *	providerTime - return utc time, the time read from provider
 *	beforeTime - the utc just before the on time marker arrived
 *	afterTime - the utc just after the on time marker arrived.
 * 
 *  Value Returned:
 *
 *	0 - success
 *     -1 - failure
 *
 *--
 */


int ReadTimedLine(int fd ,
                   utc_t *providerTime ,
                   utc_t *beforeTime ,
                   utc_t *afterTime )

{
    char    dataLine[256*2];		/* bytes read                     */
    int	bytesRead[K_ACTS_1200_SIZE+2];	/* number of bytes each read      */
    utc_t utcTimes[K_ACTS_1200_SIZE+2]; /* time stamp each read           */
    int  num_found;
    struct  timeval timeout;		/* time out value for select      */
    fd_set rfds;			/* Read mask for select           */
    int	readIndex;			/* number of timestamps/reads     */
    int bytes;                          /* byte count of a single read    */
    int status;
    int  nfds;

/*
 * examine a maximum of fd+1 file descriptors in the select mask, since
 * only one descriptor at a time will be ready for read.
 */
    nfds = fd+1;
    
    /*
     *  The algorithm to read the timestamps is as follows:
     *
     *  1. flush away all old NIST data which may be sitting in the
     *	input buffer.
     *  2. Read away characters up until the new line line character
     *     has been seen.  This will position the stream at the start
     *     of the next NIST data string.
     *  3. Loop reading the data as it arrives.  Save the number of
     *     bytes in each iteration of the loop.  Read the clock
     *     after each read statement.
     *     When the number of bytes read is greater than or equal to
     *     the number of bytes in an NIST data record, break the loop.
     *  4. At this point the data is partitioned by the timestamps that
     *     where read during each iteration of the loop:
     *
     *     47222 88-03-02 21:39:19 83 0 +.3 037.6 UTC(NIST) #
     *    ^     ^        ^             ^             ^       ^
     *   T0     T1      .....	   TN-2		 TN-1	 TN<- times read
     *   B = 0  B = 6   .....          Bn-2=x        Bn-1=x1 BN=x2 
     *
     *  5. The before time is given by TN-2, the after time is given
     *     by TN and the NIST is determined by parsing the time string.
     */
    
    
    utc_gettime(&utcTimes[0]);
    bytesRead[0] = 0;
    /*
     * First flush any queued data
     */
    BEGIN_NONREENTRANT
    status = tcflush(fd, TCIOFLUSH);
    END_NONREENTRANT
    if (status < 0) {
	PrintError("provider: error flushing i/o queues");
	return(1);
    }
    
    /*
     * Now read off the remaining characters on this line of input
     * If an error occurred, return 1 as an error status.
     */
    if (ReadNewLine(fd))
	return(1);
    
    /*
     * Now look for the on-time marker
     */
    bytesRead[0] = 0;
    readIndex = 0;
    timeout.tv_sec = 60;
    timeout.tv_usec= 0;
    FD_ZERO(&rfds);
    
    do {
	FD_SET(fd, &rfds);
#ifdef __hpux
	num_found = select(nfds, (int *)&rfds, 0, 0, &timeout);
#else
	num_found = select(nfds, (fd_set_t *)&rfds, (fd_set_t *)0, 
			   (fd_set_t *)0, &timeout);
#endif /* #ifdef __hpux */

	/*
	 * if select() timed-out because of a socket closing on the other side
	 * or no data available, return an error.
	 */
        if ( num_found == 0 ){
	    PrintError("dtsprovider: error reading data - select timed out\n");
	    return(1);
	}

	else if (num_found == -1){
	    if (errno != EINTR)
	    {
		PrintError("dtsprovider: error reading data - select error ");
		return(1);
	    }
	    else
		continue;
	}
	
	readIndex += 1;
	bytes = read(fd, &dataLine[bytesRead[readIndex-1]], 256 );
        utc_gettime(&utcTimes[readIndex]);
        if (bytes <= 0) {
	    PrintError("dtsprovider: error reading timed line data ");
	    return(1);
	}
	bytesRead[readIndex] = bytesRead[readIndex-1] + bytes;
    }
    while (bytesRead[readIndex] < K_ACTS_1200_SIZE);
    
    /*
     * Figure out which time stamp to use
     */
    dataLine[bytesRead[readIndex]] = '\0';
    
    /*
     * First determine where the on-time marker is in the
     * data read.
     */
    if (dataLine[K_ACTS_1200_OTM_POS] != '*' &&
	dataLine[K_ACTS_1200_OTM_POS] != '#') {
	TraceIf((void)fprintf(stderr,"otm out of position:");
		(void)fprintf(stderr,"%s\n",dataLine););
	return(-1);
    }
    
    /*
     * Make sure the on time was read last (TN).
     */
    if (!(bytesRead[readIndex-1] <= K_ACTS_1200_OTM_POS && 
	  bytesRead[readIndex] >= K_ACTS_1200_OTM_POS)) {
	TraceIf((void)fprintf(stderr,"Missing Data or OTM out of position:");
		(void)fprintf(stderr,"%s\n",dataLine););
	return(-1);
    }	
    
    if (readIndex > 2) {
        *beforeTime = utcTimes[readIndex - 2];		/* Tn-2 */
        *afterTime  = utcTimes[readIndex];              /* Tn   */
    }
    else {
        *beforeTime = utcTimes[0];
        *afterTime  = utcTimes[1];
    }
    
    
    /*
     * Now parse the ACTS data line to determine
     * the actual time.
     */
    return(ParseTime(providerTime, dataLine));
	
    }

/*
 *++
 *  ParseTime()
 *
 *  Functional Description:
 *
 *	Convert the ACTS text data string into a numerical utc.
 *  
 *  Inputs:
 *
 *	buffer   - the NIST data string.
 *
 *  Outputs:
 *
 *	actsTime - utc returned by the parse
 * 
 *  Value Returned:
 *
 *	0 - Valid acts buffer, parse was successful
 *	-1- Failure.
 *
 *--
 */
int ParseTime (utc_t *actsTime ,
                char  *buffer )

{
    struct tm actsTM;
    char otm[K_ACTS_1200_SIZE],NIST[K_ACTS_1200_SIZE];
    double ut1,msAdv;
    int	 mjd,st,s;
    int  matched;
    
    BEGIN_NONREENTRANT
    matched = sscanf(buffer,
		     "%d%d-%d-%d%d:%d:%d%d%d%lf%lf%s%s",
		     &mjd,&actsTM.tm_year,&actsTM.tm_mon,
		     &actsTM.tm_mday,&actsTM.tm_hour,
		     &actsTM.tm_min,&actsTM.tm_sec,
		     &st,&s,&ut1,&msAdv,
		     NIST,otm);
    END_NONREENTRANT
    if ((matched != 13) || (otm[0] != '*' && otm[0] != '#'))
    {
	TraceIf((void)fprintf(stderr,"Failed to parse data string\n");
		(void)fprintf(stderr,"%s", buffer););
	return(-1);
    }
    
    /*
     * Convert the data returned by NIST into a utc_t.
     * Call the UTC API
     */
    actsTM.tm_mon--; 
    return(utc_mkgmtime(actsTime, &actsTM, 0, &INACC_TM, INACC));

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
