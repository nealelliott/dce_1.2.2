/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ropen_main.c,v $
 * Revision 1.1.19.2  1996/02/17  23:29:23  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:10  marty]
 *
 * Revision 1.1.19.1  1995/12/13  21:56:26  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/09  20:47 UTC  bissen
 * 	merge HPDCE01
 * 	[1995/12/13  21:45:44  root]
 * 
 * Revision 1.1.17.1  1994/02/23  21:44:11  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:42:36  pellis]
 * 
 * 	HP revision /main/HPDCE01/1  1994/02/01  16:20  bissen
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.10.2  1993/07/09  16:29:04  root
 * 	Initial King Kong branch
 * 	[1993/07/09  16:28:28  root]
 * 
 * 	SVR4 porting changes - 8596
 * 	[1993/10/15  16:35:57  ohara]
 * 
 * Revision 1.1.15.1  1993/10/14  17:37:28  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:44  cmckeen]
 * 
 * Revision 1.1.12.2  1993/07/28  19:40:10  cmckeen
 * 	HP's TET'ized version of the system tests
 * 	[1993/07/27  13:55:40  cmckeen]
 * 
 * Revision 1.1.7.3  1993/05/27  19:02:24  eheller
 * 	*** empty log message ***
 * 
 * Revision 1.1.7.2  1993/05/27  14:21:28  eheller
 * 	*** empty log message ***
 * 	[1993/05/26  18:45:31  eheller]
 * 
 * Revision 1.1.8.2  1993/04/23  15:58:56  sekhar
 * 	Merged Hp_dce1_0_1 changes
 * 	[1993/04/16  15:02:15  sekhar]
 * 
 * Revision 1.1.1.7  1993/03/11  22:16:41  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.5.2  1993/02/05  15:29:20  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:50:03  cjd]
 * 
 * Revision 1.1.3.3  1992/11/09  13:12:24  mgm
 * 	Ported to HPUX
 * 	[1992/11/06  20:35:23  mgm]
 * 
 * Revision 1.1.2.4  1992/07/15  18:03:39  gmd
 * 	Changed msg_t to msg_struct to avoid conflict
 * 	with AIX /usr/include/sys/localedef31.h.
 * 	[1992/07/15  18:02:13  gmd]
 * 
 * Revision 1.1.2.3  1992/03/18  21:16:59  eperkins
 * 	Corrected includes
 * 	[1992/03/18  20:53:37  eperkins]
 * 
 * Revision 1.1.2.2  1992/02/13  21:52:33  eperkins
 * 	declared uid_t and gid_t as externs
 * 	[1992/02/13  21:52:13  eperkins]
 * 
 * Revision 1.1  1992/01/19  14:34:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   FILE_NAME: ropen_main.c
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS:	usage()
 *		initRPCClient()
 *		parseOptions()
 *		main()
 *
 *   ORIGINS: 27
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1991
 *   All Rights Reserved
 *
 *
 *   FOR_TEST: ropen
 *
 * DESCRIPTION
 *	   Implements the front-end to the ropen command.  It parses the users
 * 	   input and makes the RPC call with the given information.
 *
 * CHANGE HISTORY
 * 06/27/90		Wayne Mock		Initial design and coding
 * 10/21/91		Wayne Mock		Incorporate the inet_ntoa()
 *						into the code.
 */                                                                   

#if defined(SNI_SVR4_POSIX)
# include <sys/types.h>
#endif

#include <stdio.h>
#include <sys/file.h>
#include <sys/param.h>
#include <dce/rpc.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <systest.h>
#include <ropen_idl.h>
#include <dce/dce.h>

/*
 * These must always be last
 */
#include <dce/cma.h>
#include <dce/cma_ux.h>
#include <dce/cmalib_crtlx.h>
#include <dce/cma_px.h>


void exit();
#ifdef OSF_DCE 
extern uid_t getuid(void);
extern gid_t getgid(void);
#else
unsigned short getuid();
unsigned short getgid();
#endif


/*****************************************************************************
 * usage()
 *
 * DESCRIPTION
 *	prints the options that are necessary to use this command, and exits
 *	with a return code of 1 to notify the user something was not right.
 *
 * INPUT
 *	program		The name of the currently runnign program.
 *
 * OUTPUT
 *	None
 *
 * RETURN
 *	None; however, this function terminates the current process.
 ****************************************************************************/
void
usage()
{
	(void)fprintf( stderr,
"usage: ropen [-u <user> <password>]\n"
"             [-b | [-display name:number] [-i] [-n <string>] [-w]]\n"
"             <host> <command>\n"
"\n"
"  -u <user-name> <password>  Runs <command> as this authenticated user.\n"
"\n"
"  -b              Run <command> in the background.  Default runs <command>\n"
"                  in a window.\n"
"\n"
"  These are only valid when -b is not specified.\n"
"\n"
"  -display name:# Typical Xwindows -display option.  Only works for\n"
"                  Xwindow systems.\n"
"  -i              Bring the window up as an icon.  Default is a full size\n"
"                  window.\n"
"  -n <string>     Give the window the name of <string>.\n"
"  -w              Cause the <command> to wait for <enter> after it executes.\n"
"                  Default exits after <command>.\n"
	);
	exit( 2 );

} /* end usage() */


/*****************************************************************************
 * initRPCClient()
 *
 * DESCRIPTION
 *	Make all necessary function calls to make this program an
 *	RPC client.
 *
 * INPUT PARAMETERS
 *	none.
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	none.
 *
 ****************************************************************************/
void
initRPCClient( server, clientHandle )
char* server;
handle_t* clientHandle;
{	char *string_binding;	/* string representation of the binding */
	struct hostent* hp;	/* host information */
	char* inetaddr;		/* string representation of the RPC server's
				 * inet address.
				 */
	struct in_addr* addr;
	unsigned32 st;	/* DCE RPC error status */

/*
 * get all the RPC server host information offered by the system.
 */
	hp = gethostbyname( server );
	if( hp == NULL )
	{	perror( "initRPCClient() - gethostbyname()" );
		exit( 1 );
	}

	addr = (struct in_addr*)hp->h_addr_list[0];
	inetaddr = inet_ntoa( *addr );
	if( inetaddr == (char*) -1 )
	{	fprintf( stderr, 
		   "initRPCClient() - Unable to convert address to string\n" );
		exit( 1 );
	}


/*
 * create the string representation of the binding information.
 */
	rpc_string_binding_compose( (unsigned_char_p_t)"", 
				    (unsigned_char_p_t)"ncadg_ip_udp", 
				    (unsigned_char_p_t)inetaddr, 
				    (unsigned_char_p_t)PORT_NUMBER, 
				    (unsigned_char_p_t)"", 
				    (unsigned_char_p_t*)&string_binding, 
				    (unsigned32*)&st );
	if (st != rpc_s_ok)
	{	fprintf( stderr, 
			"initRPCClient() - rpc_string_binding_compose" );
	}

/*
 * do the binding.  This function makes this process an RPC client.
 */
	rpc_binding_from_string_binding((unsigned_char_p_t)string_binding, 
					(rpc_binding_handle_t*)clientHandle, 	
					(unsigned32*)&st);
	if (st != rpc_s_ok)
	{	fprintf( stderr, 
		    "initRPCClient() - rpc_binding_from_string_binding" );
	}

/* 
 * this is a special debug function.  Causes RPC to output more
 * information to stderr
 */
#ifdef DEBUG_RPC
	rpc__dbg_set_switches( "0-3.10", &st );
#endif

} /* end initRPCCLient() */



/*****************************************************************************
 * parseOptions()
 *
 * DESCRIPTION
 *	Determine if the user gave correct options.  If the options are 
 *	correct, fill in the appropriate data structures, if not, print the
 *	usage and die.
 *
 * INPUT
 *	argc	Number of arguments given by the user.
 *	argv    An array of pointers to the arugments given by the user.
 *
 * OUTPUT
 *	server	A pointer to the server name.  This must be specified by the
 *		user.
 *	cmdmsg	A pointer to the RPC structure.  Its members will be set 
 *		according to the given arguments.
 *
 * RETURN
 *	None.
 ****************************************************************************/
void
parseOptions( argc, argv, server, cmdmsg )
	int argc;		
	char *argv[];
	char **server;
	msg_struct *cmdmsg;
{
	int i;		/* loop control variable */
	int argc_limit;
/*
 * the user must specify at least 3 arguments and at most 12, this includes
 * the program name, argv[0].
 */
	if ( argc >= 3 && argc <= 12 )
	{
	/* 
	 * the server is ALWAYS the second to the last argument, and the 
	 * command is ALWAYS the last argument.  
	 */
		*server = argv[argc - 2];
		strcpy( cmdmsg->command, argv[argc-1] );

	/*
	 * argc_limit is all the arguments except argv[0], argv[argc-2],
	 * and argv[argc-1].
	 */
		argc_limit = argc - 3;

	/*
	 * LOOP:  This loop tries to determine the options specified by the
	 * user.  It begins at i=1 because we want to skip argv[0], and loops
	 * until i>argc-3 because there is no way to specify more options and 
	 * legally use this command.
	 */
		for( i = 1; i <= argc_limit; i++ )
		{
		/* 
		 * determine if argv[i] is a valid option, if so make the 
		 * necessary changes to cmdmsg
		 */
			if( strcmp( "-b", argv[i] ) == 0 )
			{	cmdmsg->ptype = ROPEN_BG;	
			}
			else 
			if( strcmp( "-i", argv[i] ) == 0 && 
				 cmdmsg->ptype != ROPEN_BG )
			{	cmdmsg->icon = TRUE;
			}
			else 
			if( strcmp( "-n", argv[i] ) == 0 && 
				 cmdmsg->ptype != ROPEN_BG )
			{	i++; if( i > argc_limit ) usage();
				strncpy( cmdmsg->name, argv[i], MAXNAMELEN );
			}
			else 
			if( strcmp( "-w", argv[i] ) == 0 && 
				cmdmsg->ptype != ROPEN_BG )
			{	cmdmsg->wait_flg = TRUE;
			}
			else 
			if( strcmp( "-display", argv[i] ) == 0 && 
				 cmdmsg->ptype != ROPEN_BG )
			{	i++; if( i > argc_limit ) usage();
				strcpy( cmdmsg->display, argv[i] );
			}
			else
			if( strcmp( "-u", argv[i] ) == 0 )
			{	i++; if( i > argc_limit ) usage(); 
				strcpy( cmdmsg->uname, argv[i] );
				i++; if( i > argc_limit ) usage(); 
				strcpy( cmdmsg->upasswd, argv[i] );
			}
		/*
		 * otherwise the user specified incorrect options, so print
		 * the correct options and exit.
		 */
			else
				usage();
		}
	}

/*
 * if not, show the correct syntax and exit.
 */
	else
		usage( argv[0] );

} /* end parseOptions() */


/*****************************************************************************
 * main()
 *
 * DESCRIPTION
 *	Initialize the cmdmsg structure
 *	Parse the users options
 *	Create the information to create the RPC client handle
 *	Create the RPC client handle
 *	Call the remote procedure
 *	Check the results and exit.
 *
 * INPUT
 *	argc	Number of arguments passed by the user.
 *	argv	An array of pointers to those arguments
 *
 * OUTPUT
 *	None.
 *
 * RETURN
 *	Any error codes generated by the remote procedure
 ****************************************************************************/
main( argc, argv )
int argc;
char *argv[];
{
	char *server;	/* name of the RPC server given by the user */
	msg_struct cmdmsg;	/* the argument passed to the remote procedure */
/*
 * These variables are for making the RPC.
 */
	unsigned32 result;	/* is the result returned by the remote procedure */
	handle_t clientHandle;

	cma_init();

/*
 * initialize the command message structure with default values
 */
	strcpy( cmdmsg.display, "unix:0" );	/* any valid X display name */
	strcpy( cmdmsg.name, "ropen shell" );
	cmdmsg.uname[0] = '\0';

	cmdmsg.uid = getuid();		/* uid of initiating user */
	cmdmsg.gid = getgid();		/* gid of initiating user */
	cmdmsg.ptype = ROPEN_WIN;	/* Type of process to create */
	cmdmsg.wait_flg = FALSE;		/* Do not wait by default */
	cmdmsg.icon = FALSE;		/* Open as a window not an icon */

/* 
 * determine what the user wants to do, and change server and cmdmsg to match
 */
	parseOptions( argc, argv, &server, &cmdmsg );

	initRPCClient( server, &clientHandle );

	remoteopen( (handle_t)clientHandle, (msg_struct*)&cmdmsg, (idl_long_int*)&result );


/*
 * Okay, we successfully called the remote procedure.  Now check the results.
 */
	if( result )
	{
	/*
	 * Server was unable to do our command.  Print error and die.
	 */
		(void)fprintf( stderr, 
			"%s: %s couldn't execute your command\n", 
				argv[0], server );
		exit( 1 );
	}
	return( 0 );

} /* end main() */
