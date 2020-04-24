/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dts_ntp_provider.c,v $
 * Revision 1.1.11.2  1996/02/17  23:35:24  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:37  marty]
 *
 * Revision 1.1.11.1  1995/12/08  18:13:21  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/04/10  17:55 UTC  pare
 * 	Merge fixes for CHFts14911
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/2  1995/04/10  17:53 UTC  pare
 * 	Check returned NTP packet for valid data -- fixes for CHFts14911.
 * 
 * 	HP revision /main/HPDCE02/4  1995/01/30  22:44 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/3  1994/12/13  21:31 UTC  pare
 * 
 * 	HP revision /main/HPDCE02/pare_mothra/1  1994/12/13  21:29 UTC  pare
 * 	Change second argument of daemon() from NOCLOSEFD to CLOSEFD
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/18  13:30 UTC  pare
 * 	Merge changes from Hpdce02
 * 	[1995/12/08  17:25:32  root]
 * 
 * Revision 1.1.6.3  1994/07/20  20:24:40  cbrooks
 * 	CR10945 fix Time Provider errors
 * 	[1994/07/20  18:25:53  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  13:26 UTC  pare
 * 	Merge to HODCE02
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  14:23  pare
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.7.7  1993/11/10  19:21:03  pare
 * 	Added call to PrintError routine when exiting thru diagnostic mode.
 * 	[1993/11/10  19:20:24  pare]
 * 
 * Revision 1.1.7.6  1993/11/08  20:17:22  pare
 * 	Added a command line switch (-z), which if true, sets the dts_ntp_provider in
 * 	diagnostic mode. This mode checks the error handling capability of the
 * 	ProcessRPCerror macro by passing it a bad status causing an error message to be
 * 	printed and an exit(1) to be invoked. The -z switch is not visible to the user thru
 * 	PrintHelp and InterActiveInput menu.
 * 	[1993/11/08  20:16:35  pare]
 * 
 * Revision 1.1.7.5  1993/09/18  12:52:59  pare
 * 	DCE1_0_3b03 Merge
 * 	[1993/09/18  12:52:22  pare]
 * 
 * Revision 1.1.8.3  1993/09/15  17:48:18  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.7.4  1993/08/10  14:22:16  pare
 * 	CHFts06450 -- dts_ntp_provider is started up in background in dce_config.
 * 	Changed dts_ntp_provider so that it can be started in the foreground like
 * 	the other DCE daemons -- it now forks and runs as a child dettached from
 * 	the parent.
 * 	[1993/08/10  14:19:51  pare]
 * 
 * Revision 1.1.6.2  1994/05/18  19:49:14  tom
 * 	Bug 10627 - Put ourselves into the background.
 * 	[1994/05/12  22:41:54  tom]
 * 
 * Revision 1.1.6.1  1994/04/28  21:08:31  cbrooks
 * 	Fix select parameters
 * 	[1994/04/27  19:40:56  cbrooks]
 * 
 * 	Code Cleanup; ANSI'fied; use termios
 * 	[1994/04/26  15:43:18  cbrooks]
 * 
 * 	DTS Code Cleanup
 * 	[1994/04/25  17:54:39  cbrooks]
 * 
 * Revision 1.1.4.2  1993/07/14  21:34:37  tom
 * 	Bug 8231 - Fix ReadTimes to really retry requests if they timeout.
 * 	[1993/07/14  20:44:34  tom]
 * 
 * Revision 1.1.7.3  1993/07/15  17:19:43  truitt
 * 	OT8231: Fix re-try requests to the ntp server.
 * 	[1993/07/15  17:19:24  truitt]
 * 
 * Revision 1.1.7.2  1993/07/08  20:09:53  root
 * 	Initial King Kong branch
 * 	[1993/07/08  20:09:18  root]
 * 
 * Revision 1.1.5.7  1993/05/28  13:12:08  truitt
 * 	Corrected bonehead mistake in version string.
 * 	[1993/05/28  13:11:13  truitt]
 * 
 * Revision 1.1.5.6  1993/05/25  12:52:47  truitt
 * 	Add new revision control string.
 * 	[1993/05/25  12:52:27  truitt]
 * 
 * Revision 1.1.5.5  1993/05/10  18:08:08  truitt
 * 	Add version control string.
 * 	[1993/05/10  18:07:44  truitt]
 * 
 * Revision 1.1.5.4  1993/04/23  12:32:17  truitt
 * 	Merge DCE1_0_2B23 with GAMERA.
 * 	[1993/04/22  17:52:02  truitt]
 * 
 * Revision 1.1.1.9  1993/04/21  20:52:17  root
 * 	    Loading drop DCE1_0_2B23
 * 
 * Revision 1.1.5.3  1993/04/01  19:18:23  truitt
 * 	OT 7537: Change name of struct 'sin' to 'sockin' to avoid gcc
 * 	compiler conflicts.
 * 	[1993/04/01  19:17:51  truitt]
 * 
 * Revision 1.1.2.7  1993/03/19  20:26:56  tom
 * 	Bug 7537 - Change name of struct 'sin' to 'sockin' to avoid
 * 	gcc compiler conflicts.
 * 	[1993/03/19  15:44:04  tom]
 * 
 * Revision 1.1.5.2  1993/03/24  14:35:59  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/24  14:35:21  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:34:26  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.2.6  1993/02/09  16:56:23  tom
 * 	Bug 7177 - In cvt_ntp_utc(), cast the tv_nsec to unsigned long to
 * 	avoid getting negative nsec's.
 * 	[1993/02/09  16:55:59  tom]
 * 
 * Revision 1.1.2.5  1993/01/18  22:15:16  jwade
 * 	Change comments to match the filename that should be compiled.
 * 	[1993/01/18  20:32:16  jwade]
 * 
 * Revision 1.1.2.4  1993/01/05  18:55:42  ko
 * 	Fix arithmetic error in provider.  It was not using the user specified
 * 	inaccuracy, just the systematic error.
 * 	[1993/01/05  15:27:58  ko]
 * 
 * Revision 1.1.2.3  1992/12/30  16:38:01  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:20:29  htf]
 * 
 * Revision 1.1.3.2  1992/12/22  00:23:02  smythe
 * 	added new OSF examples disclaimer comment for OSF OT 4998 fix
 * 	[1992/12/21  21:14:57  smythe]
 * 
 * Revision 1.1.2.2  1992/12/18  18:19:14  marcyw
 * 	Defect 4998: adding disclaimer comment
 * 	[1992/12/18  18:13:00  marcyw]
 * 
 * Revision 1.1.1.7  1992/09/21  18:56:21  osfrcs
 * 	    Loading drop Dce1_0_1fnl
 * 
 * Revision 1.1  1992/01/19  15:34:15  devrcs
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
 *	Module dts_ntp_provider.c
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
 * ABSTRACT:    Implements a DECdts Time Provider which obtains time
 *		from an NTP peer.
 *
 * ENVIRONMENT:	OSF Distributed Computing Environment
 *
 * AUTHORS:     Digital Distributed Time Service (DECdts).
 *		Distributed Processing Engineering (DPE).
 *                
 * DATE:	May 8, 1990
 *
 */
/*
 *                              DESCRIPTION OF THE
 *			     NETWORK TIME PROTOCOL (NTP)
 *
 *	The Network Time Protocol is an Internet recommended standard for
 *	distributing time. This provider assumes the user is familiar with
 *	the NTP protocol and has a NTP server available as a time source.
 * 
 * --------------------------------------------------------------------------
 */

/* --------------------------------------------------------------------------
 *	Compiling and Running the time provider.
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
 *       % cc dts_ntp_provider.c dtsprovider_sstub.c -ldce -o dtsprovider
 *
 *    To run the program interactively, simply type 'dtsprovider'
 *
 *	% dtsprovider
 *
 *	The provider will then ask you to enter the following parameters:
 *
 *	1. The Internet host name of the NTP server to query. This can be
 *	   specified as a name or an address (e.g. decwet.dec.com or 16.1.0.4).
 *	2. The poll period, which is the number of seconds between
 *	   NTP queries. Entering 300 for example would cause the provider
 *	   to query the NTP server every 5 minutes.
 *	3. The extra inaccuracy, which is the number of milliseconds of 
 *	   systematic inaccuracy in the timestamps delivered by NTP
 *	   The default value is 30 ms.  If the times returned by the
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
 *
 *	The provider can also be run in the background using the command
 *	line interface:
 *
 *		% dtsprovider -h decwet.dec.com -p 7200 &
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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <dce/utc.h>		/* utc library routines */
#include <dce/dtsprovider.h>	/* This file is generated by the NIDL */
				/* compiler from dtsprovider.idl */
#include <dce/dce_error.h>	/* rpc error messages */
#include <dce/dce_cf.h>
#include <dts_provider_util.h>	/* for daemon call */

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
#define K_NTP_TIMEOUT        (5)	   /* allow NTP server 5	*
					    * seconds to respond	*/
#define K_NTP_RETRY	     (3)	   /* Three tries per request	*/

#define K_NTP_JAN_1970	     (2208988800UL)  /* 1970 - 1900 in seconds	*/

#define K_DTS_NTP_STRATUM    (8)	   /* Stratum reported by DTS	*
					    * when it gives time to NTP	*/

/*
 *	NTP message parameters
 */
#ifdef __hpux /* fixes CHFts14911 */
#define K_NTP_VERSION1	     (0x0b)	   /* Status bits that encode	*/
#else					    		
#define K_NTP_VERSION1	     (0x08)	   /* Status bits that encode	*
                                            * version			*/  
#endif
#define K_NTP_UNSPECIFIED    (0)	   /* Stratum unspecified	*/

/*
 *  Maximum/Minimum parameters for TP
 */
#define K_MIN_INACCURACY     (0)	/* Allow for no systematic	*
					 * inaccuracy in each NTP resp.	*/
#define K_MAX_INACCURACY     (60 * K_MS_PER_SEC)
					/* Allow for at most 1 minute	*
					 * of inacc in each NTP resp.	*/
#define K_MIN_POLLRATE	     (60)	/* Minimum time between synchs.	*/
#define K_MAX_POLLRATE       (24*60*60)	/* Maximum time between synchs.	*/

#define K_MAX_HOSTNAME       (255)	/* largest hostname user can use*/


/*
 *	Default Control/Config Values
 */
#define K_DEF_INACCURACY     (30)          /* 30 ms extra inaccuracy in	*
					    * each NTP Time Response	*/
#define K_DEF_TMORATE	     (1 * 60)	   /* Let service wait 1 minute	*
					    * for NTP server to respond	*/
#define K_DEF_POLLRATE       (10 * 60)     /* query each 10 minutes	*
					    * to get a new time		*/
#define K_DEF_TIMESTAMPS     (4)	   /* Query the NTP server	*
					    * 4 times each synch.	*/
#define K_DEF_NOCLOCKSET     (0)	   /* Allow the clock to	*
					    * be set.			*/
#define K_DEF_TRACING	     (0)	   /* Time Provider tracing	*/

#ifdef __hpux
#define K_DEF_DEBUG          (0)           /* Debug switch              */
#define K_DEF_DIAG           (0)           /* Diag switch               */
#endif



/*
 * Structure definitions for NTP fixed point values
 *
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                         Integer Part                          |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                         Fraction Part                         |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |            Integer Part       |     Fraction Part             |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
struct l_fixedpt {
	u_long int_part;
	u_long fraction;
};

struct s_fixedpt {
	u_short sint_part;
	u_short sfraction;
};
/*
 *	NTP packet definitions
 *
 *    0                   1                   2                   3
 *    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |LI | VN  |0 0 0|    Stratum    |      Poll     |   Precision   |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                     Synchronizing Distance                    |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                     Estimated Drift Rate                      |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                  Reference Clock Identifier                   |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                 Reference Timestamp (64 bits)                 |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                 Originate Timestamp (64 bits)                 |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                  Receive Timestamp (64 bits)                  |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   |                                                               |
 *   |                  Transmit Timestamp (64 bits)                 |
 *   |                                                               |
 *   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
struct ntpdata {
	u_char status;		/* status of local clock and Mode info*/
	u_char stratum;		/* Stratum level */
	u_char poll;		/* poll value */
	int precision:8;
	struct s_fixedpt distance;
	u_long drift;
	u_long refid;
	struct l_fixedpt reftime;
	struct l_fixedpt org;
	struct l_fixedpt rec;
	struct l_fixedpt xmt;
};

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


int main(int , char	*[] );

void ContactProvider( handle_t , TPctlMsg *, error_status_t *);
 
void ServerRequestProviderTime( handle_t , TPtimeMsg   *, error_status_t *);

void CommandLineInput(int , char *argv[] );

void InterActiveInput(void);

void PrintError (char *);

void PrintHelp(void);

int ReadTimes( unsigned long , int , TPtimeMsg *);

int QueryProvider(int , utc_t *, utc_t *, utc_t *);

void cvt_utc_ntp( struct l_fixedpt *, utc_t *);
    
void cvt_ntp_utc( utc_t *, struct l_fixedpt *);
    
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

unsigned long  TIMESTAMP = K_DEF_TIMESTAMPS; /* time stamps at each sync*/

unsigned long  TMORATE   = K_DEF_TMORATE;    /* provider response timeout */

unsigned long  POLLRATE  = K_DEF_POLLRATE;   /* seconds between TP syncs*/

unsigned long  NOCLOCKSET= K_DEF_NOCLOCKSET; /* clock set		*/

unsigned long  TRACING   = K_DEF_TRACING;    /* output tracing		*/

#ifdef __hpux
unsigned long  DEBUG     = K_DEF_DEBUG;      /* debug switch            */
unsigned long  DIAG      = K_DEF_DIAG;       /* Diag switch            */
static int diag = 0;                         /* diag switch -z*/
#endif

unsigned long  INACCURACY= K_DEF_INACCURACY; /* provider inaccuracy	*/

timespec_t INACC;                        /* inacc of each time reading  */

char hostname[K_MAX_HOSTNAME+1];	 /* Host name			*/

static struct ntpdata ntp_data;		 /* NTP message structure	*/

struct ntpdata *pkt = &ntp_data;	 /* NTP message pointer		*/

struct sockaddr_in dst;			 /* Socket structure for NTP	*/

struct sockaddr_in sockin = {AF_INET};

int dstlen = sizeof(dst);		 /* Size of dst struct		*/

#ifdef __hpux /* Fix for CHFts14911 */
struct ntpdata *drainPkt = &ntp_data;	
int len = sizeof(dst);
#endif

int providerFD;			         /* provider file descriptor   */

#ifdef __hpux
static int debug = 0;                   /* debug -d */
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

    struct servent  *sp;		/* Service structure		 */
    struct hostent  *hp;	        /* Host structure                */
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
     * a timespec struct for use with API
     */	
    INACCURACY %= (60 * K_MS_PER_SEC);
    INACC.tv_sec = INACCURACY / K_MS_PER_SEC;
    INACCURACY %= K_MS_PER_SEC;
    INACC.tv_nsec = INACCURACY * K_NS_PER_MS;
    /*
     * Create a socket and bind it to ntp/udp
     */

    if ((providerFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	PrintError("dtsprovider: NTP socket");
	exit(1);
    }
    sp = getservbyname("ntp", "udp");
    if (sp == NULL) {
	(void) fprintf(stderr, "dtsprovider: UDP/NTP: service unknown\n");
	exit(1);
    }

    /*
     * Lookup host name and address
     */
    dst.sin_family = AF_INET;
    dst.sin_port = sp->s_port = sp->s_port;
    dst.sin_addr.s_addr = inet_addr(hostname);
    if (dst.sin_addr.s_addr == -1) {
	hp = gethostbyname(hostname);
	if (hp == NULL) {
	    (void) fprintf(stderr, "dtsprovider: No such host %s\n",
				   hostname);
	    exit(1);
	}
	(void) memcpy((char *)&dst.sin_addr, hp->h_addr, (size_t) hp->h_length);
    }
	
#ifdef __hpux /* Fix for CHFts14911 */
    if (fcntl(providerFD, F_SETFL, O_NONBLOCK) == -1 ) {
	PrintError("dtsprovider: NTP socket");
	exit(1);
    }
#endif

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
		     (unsigned_char_p_t)"NTP Time Provider",
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
     * At this point we listen for incoming calls from a DTS
     * daemon client program (running as a server) that wants
     * to synchronize with the Time Provider.  RPC dispatches
     * the requests to the appropriate procedures.
     */

    TraceIf (
	     fprintf(stderr, "Listening for connections ...\n");
	     );

    numThreads = K_MAX_NUM_THREADS;

   /*
    * create a signal catcher thread to kill kill use nicely 
    * after an external signal 
    */

    if(pthread_create(&signal_thread, pthread_attr_default, 
		   (pthread_startroutine_t) signal_catcher, 
		   (pthread_addr_t)NULL))
	perror("dts_ntp_provider: couldn't create signal thread:");

    rpc_server_listen (numThreads, &status);
/*    ProcessRPCerror (status, "RPC listen terminated abnormally", ABORT);*/

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

} /* end of ContactProvider */


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
     * Send the requests to the NTP server and get the
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
        PrintError("Exiting diagnostic mode.");
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

		case 'h': {
		    GETNEXTARG(argc);
		    (void)strncpy( hostname, argv[i], K_MAX_HOSTNAME );
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

    (void) printf("Enter the hostname[no default]: ");
    (void) fgets(inputLine, 512, stdin);
    (void) sscanf(inputLine, "%255s", hostname); 

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
		   "usage:\tdtsprovider -h hostname\n");
    (void) fprintf(stderr,
		   "\t\t[-p pollrate, range %d : %d, default %d]\n",
		   K_MIN_POLLRATE,K_MAX_POLLRATE,K_DEF_POLLRATE);
    (void) fprintf(stderr,
     "\t\t[-i extra milliseconds of inaccuracy, range %d : %d, default %d]\n",
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
 *			NTP request/response
 *
 *****************************************************************/
/*
 *++
 *  ReadTimes()
 *
 *  Functional Description:
 *
 *	Query the NTP server the requisite number of times. 
 *  
 *  Inputs:
 *
 *	timeStamps - the number of timestamps to read
 *	fd - the file descriptor of the NTP socket.
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

    retry = K_NTP_RETRY;

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
	    i++;
	} else {
	    if (status < 0 || (--retry == 0))
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
 *      Query an NTP server for the time. Generate the timestamp triplet.
 *  
 *  Inputs:
 *
 *	fd - file descriptor to read.
 *
 *  Outputs:
 *
 *	providerTime - return utc time, constructed from the NTP response.
 *	beforeTime - the utc just before the request is sent.
 *	afterTime - the utc just after the reply is received.
 * 
 *  Value Returned:
 *
 *	-1	Unequivocal failure - Stratum too far from TP.
 *	0	Success
 *	1	Timeout failure - retry warranted.
 *
 *--
 */

typedef void  fd_set_t  ;


int QueryProvider(int fd ,
                   utc_t *providerTime ,
                   utc_t *beforeTime ,
                   utc_t *afterTime )

{
    struct  timeval timeout;		/* time out value for select       */
    fd_set  readfds;			/* mask of file descriptors        */
    int	n;				/* file descriptor selected on     */
    utc_t ref, t1, t2, t3, t4;		/* NTP timestamps                  */
    timespec_t synchdist;		/* NTP synchronization distance    */
    timespec_t timesp, inaccsp;		/* Temps for time computation      */

#ifdef __hpux /* Fix for CHFts14911 */
    int length, send_length, recv_length;
    enum utc_cmptype relation;
    timespec_t tzero;
    reltimespec_t t_thousandSeconds, t_zeroSeconds;
    utc_t localTimeplus1k, thousandSeconds, zeroSeconds;

    /* construct a timestamp that represents 1000 seconds,
     *  ~15 minutes.
     */
    t_thousandSeconds.tv_sec = 1000;
    t_thousandSeconds.tv_nsec = 0;
    tzero.tv_sec = 0;
    tzero.tv_nsec = 0;
    if (utc_mkbinreltime(&thousandSeconds, &t_thousandSeconds, &tzero) < 0) {
	PrintError("dtsprovider: NTP utc_mkbinreltime");
	exit(1);
    }
    /* construct a timestamp that represents 0 seconds,
     */
    t_zeroSeconds.tv_sec = 0;
    t_zeroSeconds.tv_nsec = 0;
    tzero.tv_sec = 0;
    tzero.tv_nsec = 0;
    if (utc_mkbinreltime(&zeroSeconds, &t_zeroSeconds, &tzero) < 0) {
	PrintError("dtsprovider: NTP utc_mkbinreltime");
	exit(1);
    }

#endif    

    memset((char *)pkt, 0, sizeof(ntp_data));
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    timeout.tv_sec = K_NTP_TIMEOUT;
    timeout.tv_usec = 0;

    pkt->status = K_NTP_VERSION1;
    pkt->stratum = K_NTP_UNSPECIFIED;
    pkt->poll = 0;

#ifdef __hpux /* Fix for CHFts14911 */
    /* Drain the socket before attempting to send to it */
    for (;;) { 
        if ((length = recvfrom(fd, (char *)drainPkt, sizeof (ntp_data), 0,
                  (struct sockaddr *) &sockin, &len)) < 0) {
	    if (errno == EAGAIN)
                break;
        }
    }
#endif

    /*
     * Record 'before' timestamp
     */
    (void) utc_gettime(beforeTime);
    cvt_utc_ntp(&pkt->xmt, beforeTime);

#ifdef __hpux /* Fix for CHFts14911 */
    if ((send_length= sendto(fd, (char *)pkt, sizeof(ntp_data), 0,
		(struct sockaddr *) &dst, dstlen)) < 0) {
	PrintError("dtsprovider: NTP sendto");
	exit(1);
    }
#else
    if ((sendto(fd, (char *)pkt, sizeof(ntp_data), 0,
		(struct sockaddr *) &dst, dstlen)) < 0) {
	PrintError("dtsprovider: NTP sendto");
	exit(1);
    }
#endif
    /*
     * Wait for the reply by watching the file descriptor
     */
    if ((n = select(FD_SETSIZE, (fd_set_t *)&readfds, 0, 0, &timeout)) < 0) {
	PrintError("dtsprovider: NTP select");
	exit(1);
    }

    if (n == 0) {
	BEGIN_NONREENTRANT
	(void) fprintf(stderr, "\n\t* Timeout *\n");
	END_NONREENTRANT
	return(1);
    }
#ifdef __hpux /* Fix for CHFts14911 */
    if ((recv_length = recvfrom(fd, (char *)pkt, sizeof (ntp_data), 0,
	          (struct sockaddr *) &sockin, &dstlen)) < 0) {
	PrintError("dtsprovider: NTP recvfrom");
	exit(1);
    }
#else
    if ((recvfrom(fd, (char *)pkt, sizeof (ntp_data), 0,
	          (struct sockaddr *) &sockin, &dstlen)) < 0) {
	PrintError("dtsprovider: NTP recvfrom");
	exit(1);
    }
#endif

    /*
     * Record 'after' timestamp
     */
    (void) utc_gettime(afterTime);

#ifdef __hpux /* Fix for CHFts14911 */
    TraceIf(
	(void) printf("\nCheck ntp/udp receive packet\n");
	(void) printf("\tIP addr sent: [%s]\n", inet_ntoa(dst.sin_addr));
	(void) printf("\tIP addr recv: [%s]\n", inet_ntoa(sockin.sin_addr));
	(void) printf("\tStatus: %d\n", (int)(pkt->status & 0x03f)); 
	(void) printf("\tStratum: %d\n", pkt->stratum); 
	(void) printf("\tPacket size sent: [%d]\n", send_length);
	(void) printf("\tPacket size recv: [%d]\n",recv_length);
    );

    /* Make the following sanity checks on packet data received from
     * the ntp/udp socket.
     *
     * 1. Flush the ntp/udp socket before sending request.
     * 2. Check the send and receive packet length.
     * 3. Check stratum and status for NULL character.
     * 4. Check receive packet originate time with send packet xmit time.
     * 5. Check that reference time is not greater than or less than
     *    localtime plus 1000 seconds.
     * 6. Use the Xmit Timestamp as the NTP time instead of Recv Timestamp. 
     * 7. Check that the xmit time in the receive packet is NOT zero. 
     * 8. Check that the Leap indicator field is NOT 3. 
     */

    if (recv_length != send_length) {
	TraceIf(
		(void) printf("ERROR: Packet rejected, send/receive length missmatch.\n");
        );
	return(1);
    }

     /* Check that the Leap Indicator field is NOT 3 RFC1769 */
     if (((pkt->status&0xc0) == 0xc0) || (pkt->stratum == 0)) {
	TraceIf(
		(void) printf("ERROR: Packet rejected, status = %d\n",(int)(pkt->status & 0x03f));
        );
	return(1);
    }

#endif

    /*
     * Check stratum of timestamp
     */
    if (pkt->stratum >= K_DTS_NTP_STRATUM)
	return(-1);

    /*
     * Compute ntp time from information in the ntp message
     */

    cvt_ntp_utc(&ref, &pkt->reftime);
    cvt_ntp_utc(&t1, &pkt->org);
    cvt_ntp_utc(&t2, &pkt->rec);
    cvt_ntp_utc(&t3, &pkt->xmt);
    t4 = *afterTime;
    synchdist.tv_sec = ntohs((u_short) pkt->distance.sint_part);
    synchdist.tv_nsec =
	(u_long) ( (double)ntohs((u_short) pkt->distance.sfraction) *
		   (double)K_NS_PER_SEC / 65536.0 );

#ifdef __hpux /* Fixe for CHFts14911 */
    /* Check send packet xmit time with receive packet originate time, 
     * they should be the same.
     */
    if (utc_cmpmidtime(&relation, beforeTime, &t1) != 0) {
       PrintError("dtsprovider: NTP check originate time");
       return(-1);
    }
    switch (relation) {
        case utc_equalTo:
	    break;
        case utc_lessThan:
	    TraceIf(
		(void) printf("ERROR: Packet rejected, xmit time less than orig time.\n");
            );
	    return(1);
        case utc_greaterThan:
	    TraceIf(
		(void) printf("ERROR: Packet rejected, xmit time greater than orig time.\n");
            );
	    return(1);
        case utc_indeterminate:
	    TraceIf(
		(void) printf("ERROR: Packet rejected, can't compare xmit and orig time.\n");
            );
	    return(1);
    }  
    
    /* Check that the xmit time in the receive packet is greater than or equal to 
     * the receive time in the receive packet.  
     */
    if (utc_cmpmidtime(&relation, &t3, &t2) != 0) {
       PrintError("dtsprovider: NTP check originate time");
       return(-1);
    }
    switch (relation) {
        case utc_equalTo:
	    break;
        case utc_greaterThan:
	    break;
        case utc_lessThan:
	    TraceIf(
		(void) printf("ERROR: Packet rejected, xmit time less than receive time.\n");
            );
	    return(1);
        case utc_indeterminate:
	    TraceIf(
		(void) printf("ERROR: Packet rejected, can't compare xmit and receive time.\n");
            );
	    return(1);
    }  
    /* Check that the xmit time in the receive packet is NOT zero. 
     */
    if (utc_cmpmidtime(&relation, &t3, &zeroSeconds) != 0) {
       PrintError("dtsprovider: NTP check originate time");
       return(-1);
    }
    switch (relation) {
        case utc_equalTo:
	    TraceIf(
		(void) printf("ERROR: Packet rejected, xmit time equal to zero time.\n");
            );
	    return(1);
        case utc_greaterThan:
	    break;
        case utc_lessThan:
	    TraceIf(
		(void) printf("ERROR: Packet rejected, xmit time less than zero.\n");
            );
	    return(1);
        case utc_indeterminate:
	    TraceIf(
		(void) printf("ERROR: Packet rejected, can't compare xmit and zero time.\n");
            );
	    return(1);
    }  
    
    /* Check that the recv time (t2) in the receive packet is not greater than 
     * orig time (t1) plus 1000 seconds. 
     */

    if (utc_addtime(&localTimeplus1k, &t1, &thousandSeconds) < 0 ) {
	PrintError("dtsprovider: NTP utc_addtime");
	exit(1);
    }
    if (utc_cmpmidtime(&relation, &t2, &localTimeplus1k) != 0) {
       PrintError("dtsprovider: NTP check originate time");
       return(-1);
    }
    switch (relation) {
        case utc_equalTo:
	    break;
        case utc_lessThan:
	    break;
        case utc_greaterThan:
	    TraceIf(
	        (void) printf("ERROR: Packet rejected, reference time > local time + 1000 sec.\n");
            );
	    return(1);
        case utc_indeterminate:
	    TraceIf(
		(void) printf("ERROR: Packet rejected, can't compare ref and local time.\n");
            );
	    return(1);
    }  

    /* Check that the recv time (t2) in the receive packet is not less than 
     * orig time (t1) minus 1000 seconds. 
     */

    if (utc_subtime(&localTimeplus1k, &t1, &thousandSeconds) < 0 ) {
	PrintError("dtsprovider: NTP utc_addtime");
	exit(1);
    }
    if (utc_cmpmidtime(&relation, &t2, &localTimeplus1k) != 0) {
       PrintError("dtsprovider: NTP check originate time");
       return(-1);
    }
    switch (relation) {
        case utc_equalTo:
	    break;
        case utc_lessThan:
	    TraceIf(
	        (void) printf("ERROR: Packet rejected, reference time < local time - 1000 sec.\n");
            );
	    return(1);
        case utc_greaterThan:
	    break;
        case utc_indeterminate:
	    TraceIf(
		(void) printf("ERROR: Packet rejected, can't compare ref and local time.\n");
            );
	    return(1);
    }  




#endif

    TraceIf(
	(void) printf("\nRECEIVE PACKET");
	(void) printf("\n  NTP reference Timestamp [pkt->ref]:\t");
	PrintISO(&ref);
	(void) printf("\n  NTP originate Timestamp [pkt->org]:\t");
	PrintISO(&t1);
	(void) printf("\n  NTP receive Timestamp   [pkt->rec]:\t");
	PrintISO(&t2);
	(void) printf("\n  NTP transmit Timestamp  [pkt->xmt]:\t");
	PrintISO(&t3);
	(void) printf("\n  NTP synchronizing distance:\t%d.%09d\n",
		      synchdist.tv_sec, synchdist.tv_nsec);
    );
#ifdef __hpux
    (void) utc_bintime(&timesp, (timespec_t *)0, (long *)0, &t3);
#else
    (void) utc_bintime(&timesp, (timespec_t *)0, (long *)0, &t2);
#endif

    inaccsp = INACC;
    inaccsp.tv_sec += synchdist.tv_sec;
    inaccsp.tv_nsec += synchdist.tv_nsec;

    if (inaccsp.tv_nsec > K_NS_PER_SEC) {
	inaccsp.tv_sec++;
	inaccsp.tv_nsec -= K_NS_PER_SEC;
    };
    
    (void) utc_mkbintime(providerTime, &timesp, &inaccsp, 0L);

    return(0);
}

/*
 *++
 *  cvt_utc_ntp()
 *
 *  Functional Description:
 *
 *      Convert a utc timestamp into an NTP timestamp.
 *  
 *  Inputs:
 *
 *	utc - pointer to utc timestamp.
 *
 *  Outputs:
 *
 *	ntp - pointer to ntp timestamp. (See NTP spec for format).
 * 
 *  Value Returned:
 *
 *	None.
 *
 *--
 */
void cvt_utc_ntp( struct l_fixedpt *ntp ,
                  utc_t *utc )

{
    timespec_t timesp;

    (void) utc_bintime(&timesp, (timespec_t *)0, (long *)0, utc);
    ntp->int_part = ntohl((u_long) (K_NTP_JAN_1970 + timesp.tv_sec));
    ntp->fraction = ntohl((u_long) ((float) timesp.tv_nsec *
                                    4.294967295));
}    

/*
 *++
 *  cvt_ntp_utc()
 *
 *  Functional Description:
 *
 *      Convert an NTP timestamp into a utc timestamp.
 *  
 *  Inputs:
 *
 *	ntp - pointer to ntp timestamp. (See NTP spec for format).
 *
 *  Outputs:
 *
 *	utc - pointer to utc timestamp.
 * 
 *  Value Returned:
 *
 *	None.
 *
 *--
 */
void cvt_ntp_utc( utc_t *utc ,
                  struct l_fixedpt *ntp )

{
    timespec_t timesp;

    timesp.tv_sec = ntohl(ntp->int_part) - K_NTP_JAN_1970;
    timesp.tv_nsec = ((u_long)ntohl(ntp->fraction)) / 4.294967295;
    (void) utc_mkbintime(utc, &timesp, (timespec_t *)0, 0L);
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

