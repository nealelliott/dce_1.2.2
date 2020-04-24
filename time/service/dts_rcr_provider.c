/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: dts_rcr_provider.c,v $
 * Revision 1.1.2.2  1996/03/09  20:53:01  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:30:24  marty]
 *
 * Revision 1.1.2.1  1995/12/08  18:13:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/07  22:56 UTC  dat
 * 	Confine references to debug and debug_fd to if __hpux defined only.
 * 
 * 	HP revision /main/HPDCE02/3  1995/10/05  20:12 UTC  pare
 * 	Merge changes
 * 
 * 	HP revision /main/pare_MOTHRA/2  1995/10/04  18:47 UTC  pare
 * 	Remove HP JAPAN login/passwd -- CHFts16559
 * 
 * 	HP revision /main/HPDCE02/2  1995/08/10  12:41 UTC  mgm
 * 	fix so stripping will strip HISTORY
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/28  12:37 UTC  pare
 * 	Create HPDCE02 branch
 * 
 * 	HP revision /main/pare_MOTHRA/1  1995/07/27  20:35 UTC  pare
 * 	Create initial version.
 * 	[1995/12/08  17:25:37  root]
 * 
 * $EndLog$
 */

#include <pthread.h>
#ifdef OSF1
	#define _USE_OLD_TTY
	#include <sys/ttydev.h>
#endif
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <termios.h>
#if !defined(__hpux)
	#include <sgtty.h>
#endif
#include <errno.h>
#include <dce/utc.h>
#include <dce/dtsprovider.h>
#include <dce/dce_error.h>
#include <dts_provider_util.h>

#if defined( __hpux) || defined (SNI_SVR4)
	#include <sys/file.h>
#endif

#if _DECTHREADS_
	#define BEGIN_NONREENTRANT pthread_lock_global_np();
	#define END_NONREENTRANT pthread_unlock_global_np();
#else
	#define BEGIN_NONREENTRANT
	#define END_NONREENTRANT
#endif

#define TraceIf(Code) if(TRACING) { BEGIN_NONREENTRANT Code END_NONREENTRANT }
#define GETNEXTARG(argcount) if(++i >= argcount) { PrintHelp(); exit(1); }

#ifndef FD_SET
	#define	NFDBITS		32
	#define	FD_SETSIZE	32
	#define	FD_SET(n, p)   ((p)->fds_bits[(n)/NFDBITS] |= (1 << ((n) % NFDBITS)))
	#define	FD_CLR(n, p)   ((p)->fds_bits[(n)/NFDBITS] &= ~(1 << ((n) % NFDBITS)))
	#define	FD_ISSET(n, p) ((p)->fds_bits[(n)/NFDBITS] & (1 << ((n) % NFDBITS)))
	#define FD_ZERO(p)     memset((char *)(p), 0, sizeof(*(p)))
#endif

#define K_NS_PER_SEC   (1000000000)
#define K_MS_PER_SEC   (1000)
#define K_NS_PER_MS    (K_NS_PER_SEC / K_MS_PER_SEC)
#define K_100NS_PER_MS (10000)

#define K_DIAL_ATT_TMO      (120)
#define K_ACTS_1200_SIZE    (50)
#define K_ACTS_1200_OTM_POS (K_ACTS_1200_SIZE - 1)

#define HAYES_DIAL_COMMAND  "ATD"
#define DF242_DIAL_COMMAND  "\001D"
#define AT_INIT_COMMAND     "-atinit"
#define USER_ID_COMMAND     "-user"
#define USER_PASSWD_COMMAND "-passwd"
#define DEBUG_COMMAND       "-debug"

#define K_MIN_INACCURACY (30)
#define K_MAX_INACCURACY (60 * K_MS_PER_SEC)
#define K_MIN_POLLRATE	 (60)
#define K_MAX_POLLRATE   (31*24*60*60)
#define K_MAX_NUMBER     (26)
#define K_MAX_DEV_NAME	 (255)

#define K_DEF_INACCURACY   (45)
#define K_DEF_TMORATE	   (2 * 60)
#define K_DEF_POLLRATE	   (24*60*60)
#define K_DEF_TIMESTAMPS   (4)
#define K_DEF_NOCLOCKSET   (0)
#define K_DEF_TRACING	   (0)

#ifdef __hpux
	#define K_DEF_DEBUG (0)
	#define K_DEF_DIAG  (0)
#endif

#define K_DEF_INIT_STRING  "ATZ"
#define K_DEF_PHONE_NUM	   "T0423277592"
#define K_DEF_USER_NAME	   "TELJJY"
#define K_DEF_USER_PASSWD  "CRL"
#define K_DEF_MODEM_TYPE   "Hayes"
#define K_DEF_DIAL_COMMAND HAYES_DIAL_COMMAND

#define ABORT             (1)
#define CONTINUE          (0)
#define K_MAX_NUM_THREADS (1)

#define ProcessRPCerror(st, message, code)				\
{									\
	if(( st ) != rpc_s_ok )						\
	{								\
		int _locstat;						\
		char _rpcmsg[ dce_c_error_string_len ];			\
									\
		dce_error_inq_text(( st ) ,				\
					( unsigned char * )_rpcmsg ,	\
					&_locstat );			\
		BEGIN_NONREENTRANT					\
		( void )fprintf( stderr , "%s - %s\n" ,			\
				( message ) , _rpcmsg );		\
		END_NONREENTRANT					\
		if(( code ) == ABORT )					\
			exit( 1 );					\
	}								\
}

int  Delay();
void PrintError();
void PrintISO();
void PrintTimes();
void PrintHelp();
void InterActiveInput();
void CommandLineInput();
int  ReadTimes();
int  ReadNewLine();
int  ReadTimedLine();
int  ParseTime();
void DisconnectLine();

int SI_open();
int SI_close();
int SI_write();
int SI_read();

globaldef time_provider_v1_0_epv_t time_provider_epv = {
						ContactProvider,
						ServerRequestProviderTime };

char          devicename[ K_MAX_DEV_NAME +1 ];
unsigned long TIMESTAMP = K_DEF_TIMESTAMPS;
unsigned long MAX_READ_FAILURES = 3 * K_DEF_TIMESTAMPS;
unsigned long TMORATE   = K_DEF_TMORATE;
unsigned long POLLRATE  = K_DEF_POLLRATE;
unsigned long NOCLOCKSET= K_DEF_NOCLOCKSET;
unsigned long TRACING   = K_DEF_TRACING;

#ifdef __hpux
	unsigned long  DEBUG = K_DEF_DEBUG;
	unsigned long  DIAG  = K_DEF_DIAG;
	static int     debug = 0;
	static int     diag  = 0;
#endif

unsigned long INACCURACY= K_DEF_INACCURACY;
unsigned long INACC;
struct tm     INACC_TM;
char          ACTSnum[ K_MAX_NUMBER +1 ];
char          UserName[ K_MAX_NUMBER +1 ];
char          UserPasswd[ K_MAX_NUMBER +1 ];
char          ModemCmd[ K_MAX_NUMBER +1 ];
char          InitCmd[ K_MAX_NUMBER +1 ];

#ifdef __hpux
	int debug_fd;
#endif

int main( argc , argv )
int  argc;
char *argv[];
{
	int                    numThreads , i;
	uuid_t                 TP_uuid;
	unsigned32             maxCalls;
	unsigned32             status;
	rpc_binding_vector_p_t bind_vector;
	unsigned_char_t        *machinePrincipalName;

	if( argc <2 ){
		PrintHelp();
		InterActiveInput();
	} else
	CommandLineInput(argc, argv);

#ifdef __hpux
	if(( debug >0 ) || (TRACING)) {
		setvbuf( stdout , NULL , _IONBF , NULL );
		setvbuf( stderr , NULL , _IONBF , NULL );
	} else { 
                daemon(CHDIR, CLOSEFD); /* detach program from tty */
	}
#endif

	INACC_TM.tm_year = INACC_TM.tm_mon = INACC_TM.tm_mday = 0;
	INACC_TM.tm_hour = 0;
	INACC_TM.tm_min = INACCURACY / ( 60 * K_MS_PER_SEC );
	INACCURACY %= 60 * K_MS_PER_SEC;
	INACC_TM.tm_sec = INACCURACY / K_MS_PER_SEC;
	INACCURACY %= K_MS_PER_SEC;
	INACC = INACCURACY * K_NS_PER_MS;

	uuid_create_nil( &TP_uuid , &status );

/* RPC-routine
*/
	rpc_server_register_if( time_provider_v1_0_s_ifspec , &TP_uuid ,
				( rpc_mgr_epv_t )&time_provider_epv ,
				&status );
	ProcessRPCerror(status, "Unable to register interface for TPP", 
			ABORT);

	maxCalls = 1;

/* RPC-routine
*/
	rpc_server_use_all_protseqs( maxCalls , &status );
	ProcessRPCerror(status, "TPP unable to register protocol sequences ",
			ABORT);

	rpc_server_inq_bindings( &bind_vector , &status );
	ProcessRPCerror(status, "Can't inquire bindings", ABORT);

	TraceIf(
		rpc_server_inq_bindings( &bind_vector , &status );
		ProcessRPCerror(status, "Can't inquire bindings" ,ABORT);

		for( i = 0; i < bind_vector->count; i++ ){
			unsigned_char_p_t str_binding;

			rpc_binding_to_string_binding( bind_vector->binding_h[ i ] ,
							&str_binding ,
							&status );
			ProcessRPCerror(status,
					"Can't bind to string", CONTINUE);

			if( status == rpc_s_ok )
				fprintf( stderr , "Got binding: %s\n" ,
					str_binding );

			rpc_string_free( &str_binding , &status );
		}
	)

	rpc_ep_register( time_provider_v1_0_s_ifspec , bind_vector ,
			NULL , ( unsigned_char_p_t )"ACTS Time Provider" ,
			&status );
	ProcessRPCerror(status, "Can't register with endpoint mapper",
			ABORT);

	rpc_binding_vector_free( &bind_vector , &status );

	dce_cf_prin_name_from_host(( char *)NULL , &machinePrincipalName ,
					&status );
	ProcessRPCerror(status, "Can't get machine's host name", ABORT);

	rpc_server_register_auth_info( machinePrincipalName ,
					rpc_c_authn_dce_private ,
					NULL , NULL , &status );
	ProcessRPCerror(status, "Can't register authentication", ABORT);

	TraceIf(
		fprintf( stderr , "Listening for connections ...\n" );
	);

	numThreads = K_MAX_NUM_THREADS;

/* RPC-routine
*/
	rpc_server_listen( numThreads , &status );
	ProcessRPCerror(status, "RPC listen terminated abnormally", ABORT);

/* MODEM-test routine
	{
		TPtimeMsg *timeResponse;
		int       errStatus;

		timeResponse=malloc( sizeof(TPtimeMsg)
					+ sizeof( utc_t ) *3 * TIMESTAMP );
		timeResponse->timeStampCount = TIMESTAMP;

		errStatus = ReadTimes(TIMESTAMP , timeResponse );
		timeResponse->status = errStatus ? K_TPI_FAILURE : K_TPI_SUCCESS;
		PrintTimes( timeResponse );
		free( timeResponse );
	}
*/
}

void ContactProvider( bind_handle , TMOresponse , ComStatus )
handle_t       bind_handle;
TPctlMsg       *TMOresponse;
error_status_t *ComStatus;
{
	TraceIf(
		fprintf( stderr , "Executing ContactProvider ...\n" );
	);

	TMOresponse->status     = ( unsigned short )K_TPI_SUCCESS;
	TMOresponse->nextPoll   = POLLRATE;
	TMOresponse->timeout    = TMORATE;
	TMOresponse->noClockSet = NOCLOCKSET;

	*ComStatus = rpc_s_ok;
}
 
void  ServerRequestProviderTime( bind_handle, timeResponse, ComStatus)
handle_t       bind_handle;
TPtimeMsg      *timeResponse;
error_status_t *ComStatus;
{
	int errStatus;

	TraceIf(
		fprintf( stderr , "Executing ServerRequestProviderTime ...\n" );
	);

	timeResponse->timeStampCount = TIMESTAMP;

	errStatus = ReadTimes( TIMESTAMP , timeResponse );
	timeResponse->status = errStatus ? K_TPI_FAILURE : K_TPI_SUCCESS;

	TraceIf(
		( void )printf( "Reply written :\n" );
		PrintTimes( timeResponse );
	);

	*ComStatus = rpc_s_ok;

#ifdef __hpux
	if( diag ){
		errStatus = 1;
		ProcessRPCerror(errStatus,
				"Continue from ProcessRPCerror macro.",
				CONTINUE);
		PrintError( "Exiting diagnostic mode." );
		exit( 1 );
	}
#endif
}

void CommandLineInput( argc , argv )
int  argc;
char *argv[];
{
	int i;

	( void )strcpy( ACTSnum , K_DEF_PHONE_NUM );
	( void )strcpy( UserName , K_DEF_USER_NAME );
	( void )strcpy( UserPasswd , K_DEF_USER_PASSWD );
	( void )strcpy( ModemCmd , K_DEF_DIAL_COMMAND );
	( void )strcpy( InitCmd , K_DEF_INIT_STRING );

	for( i = 1; i < argc; i++ ){
		if( argv[ i ][ 0 ] != '-' ){
			PrintHelp();
			exit( 1 );
		} else {
			if( strncmp( argv[ i ] , AT_INIT_COMMAND , sizeof( AT_INIT_COMMAND ) -1 ) == 0 ){
				GETNEXTARG( argc );
				( void )strncpy( InitCmd , argv[ i ] ,
							K_MAX_NUMBER );
				continue;
			}

			if( strncmp( argv[ i ] , USER_ID_COMMAND , sizeof( USER_ID_COMMAND ) -1 ) == 0 ){
				GETNEXTARG( argc );
				( void )strncpy( UserName , argv[ i ] ,
							K_MAX_NUMBER );
				continue;
			}
			if( strncmp( argv[ i ] , USER_PASSWD_COMMAND , sizeof( USER_PASSWD_COMMAND ) -1 ) == 0 ){
				GETNEXTARG( argc );
				( void )strncpy( UserPasswd , argv[ i ] ,
							K_MAX_NUMBER );
				continue;
			}

#ifdef __hpux
			if( strncmp( argv[ i ] , DEBUG_COMMAND , sizeof( DEBUG_COMMAND ) -1 ) == 0 ){
				GETNEXTARG( argc );
				sscanf( argv[ i ] , "%d" , &debug );
				if(( debug >2 )&&( debug <0 ))
					debug = 1;
				continue;
			}
#endif
			switch( tolower( argv[ i ][ 1 ])){
				case 'o': {
					GETNEXTARG( argc );
					if( 1 != sscanf( argv[ i ] , "%d" , &TRACING )){
						( void )fprintf( stderr ,
							"-o:Output value is out of legal range\n" );
						PrintHelp();
						exit( 1 );
					}
					break;
				}
				case 'c': {
					GETNEXTARG( argc );
					if( 1 != sscanf( argv[ i ] , "%d" , &NOCLOCKSET )){
						( void )fprintf( stderr ,
							"-c:Clockset value is out of legal range\n" );
						PrintHelp();
						exit( 1 );
					}
					break;
				}
				case 'p': {
					GETNEXTARG( argc );
					if( 1 != sscanf( argv[ i ] , "%d" , &POLLRATE) ||
							POLLRATE < K_MIN_POLLRATE ||
							POLLRATE > K_MAX_POLLRATE ){
						( void )fprintf( stderr ,
							"-p:Pollrate value is out of legal range\n" );
						PrintHelp();
						exit( 1 );
					}
					break;
				}
				case 'i': {
					GETNEXTARG( argc );
					if( 1 != sscanf( argv[ i ] , "%d" , &INACCURACY) ||
							INACCURACY < K_MIN_INACCURACY || 
							INACCURACY > K_MAX_INACCURACY ){
						( void )fprintf( stderr ,
							"-i:Inaccuracy value is out of legal range\n" );
						PrintHelp();
						exit( 1 );
					}
					break;
				}
				case 't': {
					GETNEXTARG( argc );
					if( 1 != sscanf( argv[ i ] , "%d" , &TIMESTAMP) ||
							TIMESTAMP < K_MIN_TIMESTAMPS || 
							TIMESTAMP > K_MAX_TIMESTAMPS ){
						( void )fprintf( stderr ,
							"-t:Time stamp count is out of legal range\n" );
						PrintHelp();
						exit( 1 );
					}
					MAX_READ_FAILURES = 3 * TIMESTAMP;
					break;
				}
				case 'm': {
					GETNEXTARG( argc );
					if( !strcasecmp( argv[ i ] , "hayes" ))
						( void )strcpy( ModemCmd ,
								HAYES_DIAL_COMMAND );
					else
						if( !strcasecmp( argv[ i ] , "scholar" ))
							( void )strcpy( ModemCmd ,
								DF242_DIAL_COMMAND );
						else {
							( void )fprintf( stderr ,
								"-m:Bad modem name\n" );
							PrintHelp();
							exit( 1 );
						}
					break;
				}
				case 'd': {
					GETNEXTARG( argc );
					( void )strcpy( devicename , argv[ i ]);
					break;
				}
				case 'n': {
					GETNEXTARG( argc );
					( void )strncpy( ACTSnum , argv[ i ] ,
							K_MAX_NUMBER );
					break;
				}
				default: {
					PrintHelp();
					exit( 1 );
				}
			}
		}
	}
}

void InterActiveInput()
{
	char inputLine[ 512 ];
	char modemType[ 10 ];

	( void )printf( "\n\n\nEnter the Device Name: " );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%255s" , devicename );

	( void )printf( "Enter the Poll Rate in Seconds[%d]: " , POLLRATE );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%d" , &POLLRATE ); 
	if( POLLRATE < K_MIN_POLLRATE || POLLRATE > K_MAX_POLLRATE ){
		( void )fprintf( stderr ,
				"-p:Pollrate value is out of legal range\n" );
		exit( 1 );
	}

	( void )printf( "Enter the Inaccuracy[%d(ms)]: " , INACCURACY );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%d" , &INACCURACY );
	if( INACCURACY < K_MIN_INACCURACY || INACCURACY > K_MAX_INACCURACY ){
		( void )fprintf( stderr ,
				"-i:Inaccuracy value is out of legal range\n" );
		exit( 1 );
	}

	( void )printf( "Enter the Timestamp Count[%d]: " , TIMESTAMP );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%d" , &TIMESTAMP );
	if( TIMESTAMP < K_MIN_TIMESTAMPS || TIMESTAMP > K_MAX_TIMESTAMPS ){
		( void )fprintf( stderr ,
				"-t:Time stamp count is out of legal range\n" );
		exit( 1 );
	} else
		MAX_READ_FAILURES = 3 * TIMESTAMP;

	( void )printf( "Disallow clock set[%d(Y:1,N:0)] : " , NOCLOCKSET );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%d" , &NOCLOCKSET );

	( void )printf( "Output Trace [%d(Y:1,N:0)] : " , TRACING );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%d" , &TRACING );

#ifdef __hpux
	( void )printf( "Debug On [%d(always:2,only error:1,none:0)] : " , debug );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%d" , &debug );
#endif
	( void )strcpy( ACTSnum , K_DEF_PHONE_NUM );
	( void )printf( "Enter the telephone number[%s]: " , ACTSnum );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%26s" , ACTSnum );

	( void )strcpy( UserName , K_DEF_USER_NAME );
	( void )printf( "Enter the user ID of telephone JJY[%s]: " , UserName );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%26s" , UserName );

	( void )strcpy( UserPasswd , K_DEF_USER_PASSWD );
	( void )printf( "Enter the user password of telephone JJY[%s]: " , UserPasswd );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%26s" , UserPasswd );

	( void )strcpy( InitCmd , K_DEF_INIT_STRING );
	( void )printf( "Enter the modem initialize string [%s]: " , InitCmd );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%26s" , InitCmd );

	( void )strcpy( modemType , K_DEF_MODEM_TYPE );
	( void )printf( "Enter the modem type (Hayes or Scholar)[%s]: " ,
			"Hayes" );
	( void )fgets( inputLine , 512 , stdin );
	( void )sscanf( inputLine , "%26s" , modemType );
	if( strncasecmp( modemType , "scholar" , 5) ==0 )
		( void )strcpy( ModemCmd , DF242_DIAL_COMMAND );
	else
		if( strncasecmp( modemType , "hayes" , 5 ) ==0 )
			( void )strcpy( ModemCmd , K_DEF_DIAL_COMMAND );
		else {
			( void )fprintf( stderr ,
					"-m: Modem type not supported\n" );
			exit( 1 );
		}
}

int Delay( secs )
unsigned secs;
{
	#if _DECTHREADS_
		timespec_t interval;

		interval.tv_sec = secs;
		interval.tv_nsec = 0;

		if( pthread_delay_np( &interval ) <0 ){
			PrintError( "dtsprovider: cannot delay" );
			return( 1 );
		}
	#else
		unsigned sleep();

		sleep( secs );
	#endif

	return(0);
}

void PrintError( errorString )
char *errorString;
{
	BEGIN_NONREENTRANT
	perror( errorString );
	END_NONREENTRANT
}

void PrintHelp()
{
	( void )fprintf( stderr ,
			"usage:\tdtsprovider -d device-name\n" );
	( void )fprintf( stderr ,
			"\t\t[-n Telephone Number, default : %s ]\n" ,
			K_DEF_PHONE_NUM );
	( void )fprintf( stderr ,
			"\t\t[-user User ID of Telephone JJY, default : %s ]\n" ,
			K_DEF_USER_NAME );
	( void )fprintf( stderr ,
			"\t\t[-passwd User Passwd of Telephone JJY, default : %s ]\n" ,
			K_DEF_USER_PASSWD );
	( void )fprintf( stderr ,
			"\t\t[-atinit Modem initialize strings, default : %s ]\n" ,
			K_DEF_INIT_STRING );
	( void )fprintf( stderr ,
			"\t\t[-p pollrate, range %d : %d, default %d]\n" ,
			K_MIN_POLLRATE , K_MAX_POLLRATE , K_DEF_POLLRATE );
	( void )fprintf( stderr ,
			"\t\t[-i milliseconds of inaccuracy, range %d : %d, default %d]\n" ,
			K_MIN_INACCURACY , K_MAX_INACCURACY , K_DEF_INACCURACY );
	( void )fprintf( stderr ,
			"\t\t[-t timestamps, range %d : %d, default %d]\n" ,
			K_MIN_TIMESTAMPS , K_MAX_TIMESTAMPS , K_DEF_TIMESTAMPS );
	( void )fprintf( stderr ,
			"\t\t[-c disallow clock set, range 0(false) : 1(true), default %d]\n" ,
			K_DEF_NOCLOCKSET );
	( void )fprintf( stderr ,
			"\t\t[-o output trace, range 0(false) : 1(true), default %d]\n" ,
			K_DEF_TRACING );
#ifdef __hpux
	( void )fprintf( stderr ,
			"\t\t[-debug debug on, range 0(none) : 1(only error) : 2(always), default %d]\n" ,
			debug );
#endif
	( void )fprintf( stderr ,
			"\t\t[-m modem_name, range Hayes : Scholar, default hayes]\n" );
	( void )fprintf( stderr ,
			"Example : dts_rcr_provider -d /dev/tty00 -n 'T0423276127' -atinit 'ATZ'\n" );
}

int SI_open( path , oflag , fmode )
char *path;
int oflag;
int fmode;
{
	char *log_str = "0644";
	int  log_mode;

#ifdef __hpux
	if( debug >0 ){
		sscanf( log_str , "%o" , &log_mode );
		if(( debug_fd = open( "/tmp/dts_provider.log" , O_CREAT | O_WRONLY | O_APPEND , log_mode )) <0 ){
			char errorString[ 256 ];

			BEGIN_NONREENTRANT
			( void )sprintf( errorString , "%s: 200%s" ,
					"dtsprovider: can not open log file" );
			END_NONREENTRANT
			PrintError( errorString );
			return( 1 );
		}
	}
#endif
	return( open( path , oflag , fmode ));
}

int SI_close( cl_fd )
int cl_fd;
{
#ifdef __hpux
	if( debug >0 ){
			close( debug_fd );
	}
#endif
	return( close( cl_fd ));
}

int SI_read( r_fd , r_buf , r_bytes )
int  r_fd;
void *r_buf;
int  r_bytes;
{
	int ac_r_bytes;

	ac_r_bytes = read( r_fd , r_buf , r_bytes );
#ifdef __hpux
	if( debug >0 ){
		if( write( debug_fd , r_buf , ac_r_bytes ) != ac_r_bytes ){
			PrintError( "dtsprovider: error writing log file" );
			return( 1 );
		}
	}
#endif
	return( ac_r_bytes );
}

int SI_write( w_fd , w_buf , w_bytes )
int  w_fd;
void *w_buf;
int  w_bytes;
{
	char out_data[ 1000 ];
	int  out_length;

#ifdef __hpux
	if( debug >0 ){
		sprintf( out_data , "\nWRITE(%s:%d)\n" , w_buf , w_bytes );
		out_length=strlen( out_data );
		if( write( debug_fd , out_data , out_length ) != out_length ){
			PrintError( "dtsprovider: error writing log file" );
			return( 1 );
		}
	}
#endif
	return( write( w_fd , w_buf , w_bytes ));
}

void DisconnectLine( fd , log_msg )
int fd;
char *log_msg;
{
#ifdef __hpux
	if( debug >0 ){
		write( debug_fd , log_msg , strlen( log_msg ));
		write( debug_fd , "\n" , 1 );
		close( debug_fd );
	}
#endif
	write( fd , "+++\r" , 4 );
	Delay( 1 );
	write( fd , "ATH0\r" , 5 );
	close( fd );
}

int ReadTimes( timeStamps , tpTimeMsg )
unsigned long timeStamps;
TPtimeMsg     *tpTimeMsg;
{
	int            fd;
	int            bufSize , bufSize2 , bufSize3 , bufSize4;
	struct termios termChar;
	struct termios saveTermChar;
	int            i , j , bytes , status;
	int            errStatus;
	char           dialNumber[ K_MAX_NUMBER +1 ];
	char           buffer;

	( void )strncpy( dialNumber , ModemCmd , K_MAX_NUMBER );
	( void )strncat( dialNumber , ACTSnum , K_MAX_NUMBER -4 );
	bufSize = ( int )strlen( dialNumber );
	dialNumber[ bufSize ] = '\r';
	bufSize = bufSize + 1;

	bufSize2 = ( int )strlen( InitCmd );
	InitCmd[ bufSize2 ] = '\r';
	bufSize2 = bufSize2 + 1;

	bufSize3 = ( int )strlen( UserName );
	UserName[ bufSize3 ] = '\r';
	bufSize3 = bufSize3 + 1;

	bufSize4 = ( int )strlen( UserPasswd );
	UserPasswd[ bufSize4 ] = '\r';
	bufSize4 = bufSize4 + 1;

	if(( fd = SI_open( devicename , O_RDWR | O_NDELAY , 0 )) <0 ){
		char errorString[ 256 ];

		BEGIN_NONREENTRANT
		( void )sprintf( errorString , "%s: 200%s" ,
				"dtsprovider: can not open device" ,
				devicename );
		END_NONREENTRANT
		PrintError( errorString );
		DisconnectLine( fd , "dtsprovider: can not open device" );
		return( 1 );
	}

	BEGIN_NONREENTRANT
	errStatus = tcgetattr( fd , &saveTermChar );
	END_NONREENTRANT
	if( errStatus <0 ){
		PrintError( "dtsprovider: get attr " );
		DisconnectLine( fd , "dtsprovider: get attr" );
		return( 1 );
	}

	termChar = saveTermChar;

	termChar.c_iflag      |= IGNBRK | PARMRK;
	termChar.c_iflag      &= ~( ISTRIP | BRKINT );
	termChar.c_oflag      &= ~( OPOST );
	termChar.c_cflag      &= ~( CSTOP | CBAUD | CSIZE | CSTOPB | PARENB );
	termChar.c_cflag      |= ( CS8 | CLOCAL | B1200 | CREAD );
	termChar.c_lflag      &= ~( ISIG | ECHO | ICANON );
	termChar.c_cc[ VMIN ]  = 0;
	termChar.c_cc[ VTIME ] = 100;

	BEGIN_NONREENTRANT
	errStatus = tcsetattr( fd , TCSANOW , &termChar );
	END_NONREENTRANT
	if( errStatus < 0 ){
		PrintError( "dtsprovider: can not set the terminal characteristics" );
		DisconnectLine( fd , "dtsprovider: can not set the terminal characteristics" );
		return( 1 );
	}

	if( Delay( 1 )){
		PrintError( "dtsprovider: cannot delay after setting term. chars." );
		DisconnectLine( fd , "dtsprovider: cannot delay after setting term. chars." );
		return( 1 );
	}

	if( SI_write( fd , InitCmd , bufSize2 ) != bufSize2 ){
		PrintError( "dtsprovider: error writing to device " );
		DisconnectLine( fd , "dtsprovider: error writing data" );
		return( 1 );
	}

	if( Delay( 1 )){
		PrintError( "dtsprovider: cannot delay after setting term. chars." );
		DisconnectLine( fd , "dtsprovider: cannot delay after setting term. chars." );
		return( 1 );
	}

	if( SI_write( fd , dialNumber , bufSize ) != bufSize ){
		PrintError( "dtsprovider: error writing to device " );
		DisconnectLine( fd , "dtsprovider: error writing to device " );
		return( 1 );
	}

	status = 1;
	while( status ){
		if(( bytes = SI_read( fd , &buffer , 1 )) >0 ){
			if( buffer == '?' ){
				status = 0;
			}
		}
	}

	if( SI_write( fd , UserName , bufSize3 ) != bufSize3 ){
		PrintError( "dtsprovider: error writing to device " );
		DisconnectLine( fd , "dtsprovider: error writing to device " );
		return( 1 );
	}

	status = 1;
	while( status ){
		if(( bytes = SI_read( fd , &buffer , 1 )) >0 ){
			if( buffer == '?' ){
				status = 0;
			}
		}
	}

	if( SI_write( fd , UserPasswd , bufSize4 ) != bufSize4 ){
		PrintError( "dtsprovider: error writing to device " );
		DisconnectLine( fd , "dtsprovider: error writing to device " );
		return( 1 );
	}

	status = 1;
	while( status ){
		if(( bytes = SI_read( fd , &buffer , 1 )) >0 ){
			if( buffer == '>' ){
				status = 0;
			}
		}
	}

	if( SI_write( fd , "LOOP\r" , 5 ) !=5 ){
		PrintError( "dtsprovider: error writing to device " );
		DisconnectLine( fd , "dtsprovider: error writing to device " );
		return( 1 );
	}

	if( Delay( 1 )){
		PrintError( "dtsprovider: cannot delay waiting for command" );
		DisconnectLine( fd , "dtsprovider: cannot delay waiting for command" );
		return( 1 );
	}

	if( SI_write( fd , "C\r" , 2 ) !=2 ){
		PrintError( "dtsprovider: error writing to device " );
		DisconnectLine( fd , "dtsprovider: error writing to device " );
		return( 1 );
	}

	if( Delay( 2 )){
		PrintError( "dtsprovider: cannot delay waiting for command" );
		DisconnectLine( fd , "dtsprovider: cannot delay waiting for command" );
		return( 1 );
	}

	if( SI_write( fd , "COM\r" , 4 ) !=4 ){
		PrintError( "dtsprovider: error writing to device " );
		DisconnectLine( fd , "dtsprovider: error writing to device " );
		return( 1 );
	}

	if( Delay( 1 )){
		PrintError( "dtsprovider: cannot delay waiting for command" );
		DisconnectLine( fd , "dtsprovider: cannot delay waiting for command" );
		return( 1 );
	}

	status = 1;
	while( status ){
		if(( bytes = SI_read( fd , &buffer , 1 )) >0 ){
			if( buffer == '>' ){
				status = 0;
			}
		}
	}

	BEGIN_NONREENTRANT
	status = tcflush( fd , TCIOFLUSH );
	END_NONREENTRANT
	if( status <0 ){
		PrintError( "provider: error flushing i/o queues" );
		DisconnectLine( fd , "provider: error flushing i/o queues" );
		return( 1 );
	}

	if( fcntl( fd , F_SETFL , FNDELAY ) <0 ){
		PrintError( "dtsprovider: error setting line to non-blocking" );
		DisconnectLine( fd , "dtsprovider: error setting line to non-blocking" );
		return( 1 );
	}

	i = j = 0;
	while(( i < timeStamps )&&( j < MAX_READ_FAILURES )){
		errStatus = ReadTimedLine( fd ,
					&tpTimeMsg->timeStampList[ i ].TPtime ,
					&tpTimeMsg->timeStampList[ i ].beforeTime , 
					&tpTimeMsg->timeStampList[ i ].afterTime );

		if( Delay( 1 )){
			PrintError( "dtsprovider: cannot delay waiting for command" );
			DisconnectLine( fd , "dtsprovider: cannot delay waiting for command" );
			return( 1 );
		}
		BEGIN_NONREENTRANT
		status = tcflush( fd , TCIOFLUSH );
		END_NONREENTRANT
		if( status <0 ){
			PrintError( "provider: error flushing i/o queues" );
			DisconnectLine( fd , "provider: error flushing i/o queues" );
			return( 1 );
		}

		switch( errStatus ){
			case  0 : {
				i += 1;
				break;
			}
			case -1 : {
				if( j == MAX_READ_FAILURES ){
					DisconnectLine( fd );
					return( 1 );
				} else 
					j += 1;
				break;
			}
			case 1 : {
				DisconnectLine( fd );
				return( 1 );
			}
		}
	}

	if( Delay( 1 )){
		PrintError( "dtsprovider: cannot delay waiting for command" );
		DisconnectLine( fd , "dtsprovider: cannot delay waiting for command" );
		return( 1 );
	}

	if( SI_write( fd , "BYE\r" , 4 ) !=4 ){
		PrintError( "dtsprovider: error writing to device " );
		DisconnectLine( fd , "dtsprovider: error writing to device " );
		return( 1 );
	}

/* send "ATH" command to modem
	status = 1;
	while( status ){
		if(( bytes = SI_read( fd , &buffer , 1 )) > 0 ){
			if( buffer == 'R' ){
				status = 0;
			}
		}
	}

	if( SI_write( fd , "ATH\r" , 4 ) !=4 ){
		PrintError( "dtsprovider: error writing to device " );
		DisconnectLine( fd , "dtsprovider: error writing to device " );
		return( 1 );
	}
*/

	termChar = saveTermChar;
	BEGIN_NONREENTRANT
	errStatus = cfsetispeed( &termChar , B0 );
	END_NONREENTRANT
	if( errStatus <0 ){
		PrintError( "dtsprovider: error setting input modem speed" );
		DisconnectLine( fd , "dtsprovider: error setting input modem speed" );
		return( 1 );
	}

	BEGIN_NONREENTRANT
	errStatus = cfsetospeed( &termChar , B0 );
	END_NONREENTRANT
	if( errStatus <0 ){
		PrintError( "dtsprovider: error setting output modem speed" );
		DisconnectLine( fd , "dtsprovider: error setting output modem speed" );
		return( 1 );
	}

	BEGIN_NONREENTRANT
	errStatus = tcsetattr( fd , TCSANOW , &termChar );
	END_NONREENTRANT
	if( errStatus <0 ){
		PrintError( "dtsprovider: error disconnecting modem" );
		DisconnectLine( fd , "dtsprovider: error disconnecting modem" );
		return( 1 );
	}

	SI_close( fd );
	return( 0 );
}

int ReadTimedLine( fd , providerTime , beforeTime , afterTime )
int   fd;
utc_t *providerTime;
utc_t *beforeTime;
utc_t *afterTime;
{
	char  timeLine[ 256 ];
	int   timeIndex = 0;
	char  dateLine[ 256 ];
	int   dateIndex = 0;
	int   tl_bytes = 0;
	int   test_ch , bytes , status;
	utc_t utcTimes[ K_ACTS_1200_SIZE +2 ];

	int   bytesRead[ K_ACTS_1200_SIZE +2 ];
	int   num_found;
	 
	fd_set rfds;
	int    nfds;

	nfds = fd +1;

	BEGIN_NONREENTRANT
	status = tcflush( fd , TCIOFLUSH );
	END_NONREENTRANT
	if( status <0 ){
		PrintError("provider: error flushing i/o queues");
		return( 1 );
	}

	if( SI_write( fd , "DUSA\r" , 5 ) !=5 ){
		PrintError( "dtsprovider: error writing to device " );
		return( 1 );
	}

	status = 1;
	while( status ){
		if(( bytes = SI_read( fd , &dateLine[ dateIndex ] , 1 )) > 0 ){
			test_ch = ( int )dateLine[ dateIndex ];

			if(((( int )dateLine[ dateIndex ] >= 48 )&&
					(( int )dateLine[ dateIndex ] <= 57 ))||
					( dateLine[ dateIndex ] == '/' )){
				tl_bytes = tl_bytes + bytes;
				if( tl_bytes ==8 ){
					status = 0;
					dateLine[ dateIndex +1 ] = '\0';
				} else {
					dateIndex++;
				}
			}
		}
	}

	BEGIN_NONREENTRANT
	status = tcflush( fd , TCIOFLUSH );
	END_NONREENTRANT
	if( status <0 ){
		PrintError("provider: error flushing i/o queues");
		return( 1 );
	}

	utc_gettime(&utcTimes[0]);

	if( SI_write( fd , "UTC\r" , 4 ) !=4 ){
		PrintError( "dtsprovider: error writing to device " );
		return( 1 );
	}

	tl_bytes = 0;
	status = 1;
	while( status ){
		if(( bytes = SI_read( fd , &timeLine[ timeIndex ] , 1 )) > 0 ){
			test_ch = ( int )timeLine[ timeIndex ];

			if(((( int )timeLine[ timeIndex ] >= 48 )&&
					(( int )timeLine[ timeIndex ] <= 57 ))||
					( timeLine[ timeIndex ] == ':' )){
				tl_bytes = tl_bytes + bytes;
				if( tl_bytes ==8 ){
					status = 0;
					timeLine[ timeIndex +1 ] = '\0';
				} else {
					timeIndex++;
				}
			}
		}
	}

        utc_gettime(&utcTimes[1]);

	if( SI_write( fd , "\r" , 1 ) !=1 ){
		PrintError( "dtsprovider: error writing to device " );
		return( 1 );
	}

	*beforeTime = utcTimes[0];
	*afterTime  = utcTimes[1];

	return( ParseTime( providerTime , dateLine , timeLine ));
}

int ParseTime( actsTime , buffer1 , buffer2 )
utc_t *actsTime;
char   *buffer1;
char   *buffer2;
{
	struct tm actsTM;
	char      otm[ K_ACTS_1200_SIZE ] , NIST[ K_ACTS_1200_SIZE ];
	double    ut1 , msAdv;
	int       mjd , st , s , matched;
    
	BEGIN_NONREENTRANT
	matched = sscanf( buffer1 , "%2d/%2d/%2d" ,
			&actsTM.tm_mon , &actsTM.tm_mday , &actsTM.tm_year );
	END_NONREENTRANT
	if( matched != 3 ){
		TraceIf(( void )fprintf( stderr ,
					"Failed to parse data string\n" );
			( void )fprintf( stderr ,"%s" , buffer1 );
		);
		return( -1 );
	}

	BEGIN_NONREENTRANT
	matched = sscanf( buffer2 , "%d:%d:%d" ,
			&actsTM.tm_hour , &actsTM.tm_min , &actsTM.tm_sec );
	END_NONREENTRANT
	if( matched != 3 ){
		TraceIf(( void )fprintf( stderr ,
					"Failed to parse data string\n" );
			( void )fprintf( stderr , "%s" , buffer2 );
		);
		return( -1 );
	}
	actsTM.tm_mon--; 

	return( utc_mkgmtime( actsTime , &actsTM , 0 , &INACC_TM , INACC ));
}

void PrintTimes( timeResponse )
TPtimeMsg *timeResponse;
{
	int i;

	if( timeResponse->status == K_TPI_FAILURE ){
		BEGIN_NONREENTRANT
		( void )printf( "K_TPI_FAILURE\n" );
		( void )printf( "******************************************\n" );
		END_NONREENTRANT
		return;
	}

	BEGIN_NONREENTRANT
	( void )printf( "K_TPI_SUCCESS\n" );
	( void )printf( "Time Stamps   : %d\n" , timeResponse->timeStampCount );

	for( i = 0; i < timeResponse->timeStampCount; i++ ){
		( void )printf( "Before Time\t:" );
		PrintISO( &timeResponse->timeStampList[ i ].beforeTime );

		( void )printf( "\nTP Time\t\t:" );
		PrintISO( &timeResponse->timeStampList[ i ].TPtime );

		( void )printf( "\nAfter Time\t:" );
		PrintISO( &timeResponse->timeStampList[ i ].afterTime );

		(void)printf( "\n\n" );
	}

	( void )printf( "******************************************\n" );
	END_NONREENTRANT
}

void PrintISO( utcTime )
utc_t *utcTime;
{
	char gmTimeStr[ 50 ];

	( void )utc_ascgmtime( gmTimeStr , sizeof( gmTimeStr ) , utcTime );
	printf( "%s\t" , gmTimeStr );
}

int ReadNewLine( fd )
int fd;
{
	char           buffer;
	int            bytes;
	int            num_found;
	fd_set         rfds;
	struct timeval timeout;
	int            nfds;

	nfds = fd +1;

	timeout.tv_sec  = 60;
	timeout.tv_usec = 0;

	while( 1 ){
		FD_ZERO( &rfds );
		FD_SET( fd , &rfds );

		#ifdef __hpux
			num_found = select( nfds , ( int *)&rfds ,
						0 , 0 , &timeout );
		#else
			num_found = select( nfds , &rfds ,
						( fd_set *)0 , ( fd_set *)0 ,
				   		&timeout );
		#endif

 	       if( num_found ==0 ){
		    PrintError( "dtsprovider: error reading data - select timed out\n" );
		    return( 1 );
		} else
			if( num_found == -1 ){
				if( errno != EINTR ){
					PrintError( "dtsprovider: error reading data - select error" );
					return( 1 );
				} else
					continue;
			}

		if(( bytes = SI_read( fd , &buffer , 1 )) >0 ){
			if( buffer == '\n' ){
				break;
			} else
				if( bytes==0 )
					continue;
				else
					if( bytes <0 && errno != EINTR ){
						PrintError( "dtsprovider: error reading data - line read" );
						return( 1 );
					}
		}
	}
	return(0);
}
