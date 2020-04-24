/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: remoteopen.c,v $
 * Revision 1.1.22.2  1996/02/17  23:29:22  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:46:09  marty]
 *
 * Revision 1.1.22.1  1995/12/13  21:56:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:43  root]
 * 
 * Revision 1.1.18.1  1993/10/15  21:16:06  ohara
 * 	SVR4 porting changes - 8596
 * 	[1993/10/15  16:35:54  ohara]
 * 
 * Revision 1.1.16.2  1993/07/28  19:39:57  cmckeen
 * 	HP's TET'ized version of the system tests
 * 	[1993/07/27  13:55:31  cmckeen]
 * 
 * Revision 1.1.11.3  1993/05/27  19:01:54  eheller
 * 	*** empty log message ***
 * 
 * Revision 1.1.11.2  1993/05/27  14:19:39  eheller
 * 	*** empty log message ***
 * 	[1993/05/26  18:44:08  eheller]
 * 
 * Revision 1.1.7.2  1993/04/23  15:58:43  sekhar
 * 	Merged Hp_dce1_0_1 changes
 * 	[1993/04/16  15:01:52  sekhar]
 * 
 * Revision 1.1.1.7  1993/03/11  22:16:35  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.9.2  1993/02/05  15:29:08  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:49:53  cjd]
 * 
 * Revision 1.1.3.4  1992/12/09  15:36:27  mgm
 * 	Brought over fixes from dce1.0.2 area.
 * 	[1992/12/09  15:34:26  mgm]
 * 
 * Revision 1.1.3.3  1992/11/09  13:12:24  mgm
 * 	Ported to HPUX
 * 	[1992/11/06  20:35:23  mgm]
 * 
 * Revision 1.1.2.8  1992/07/20  20:34:58  gmd
 * 	Changed msg_t to msg_struct to avoid conflict
 * 	with AIX /usr/include/sys/localedef31.h.
 * 	[1992/07/20  15:43:45  gmd]
 * 
 * Revision 1.1.2.7  1992/07/17  21:59:38  root
 * 	  Changed to create the xterm with a scrolled window of 2000 lines.
 * 	[1992/07/17  21:12:54  root]
 * 
 * Revision 1.1.2.5  1992/06/16  12:54:19  ramb
 * 	Modified program to detach from forked processes which could execute for an
 * 	indeterminate period of time, by creating an intermediary child process which
 * 	orphans the final child process. The orphaned process becomes attached to the
 * 	init process, which periodically clean them up when they finish executing and
 * 	reach the zombie state.
 * 	[1992/06/16  12:46:53  ramb]
 * 
 * Revision 1.1.2.4  1992/06/08  18:09:57  ramb
 * 	  Modified code to exit if rpc calls fail.
 * 	       Modified to specifiy xterm with absolute pathname
 * 	       Modified number of concurrent calls to 10 from 1 for the server.
 * 
 * 	Modified code to exit if RPC calls fail.
 * 	Modified to specify 'xterm' with absolute path name.
 * 	Modified number of concurrent calls to 10 from 1 for server.
 * 	[1992/06/08  13:25:43  ramb]
 * 
 * Revision 1.1.2.3  1992/03/18  20:55:48  eperkins
 * 	corrected includes
 * 	[1992/03/18  20:55:25  eperkins]
 * 
 * Revision 1.1.2.2  1992/02/13  21:47:46  eperkins
 * 	declared uid_t and gid_t as externs
 * 	[1992/02/13  21:47:26  eperkins]
 * 
 * Revision 1.1  1992/01/19  14:34:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 *
 *   FILE_NAME: remoteopen.c
 *
 *   COMPONENT_NAME: dce.systest
 *
 *   FUNCTIONS:	allocateMemory()
 *		runUsersCommand()
 *		remoteopen()
 *		main()
 *
 *   ORIGINS: 27
 *
 *   (C) COPYRIGHT International Business Machines Corp. 1991
 *   All Rights Reserved
 *
 *   FOR_TEST: Any
 *
 * SUMMARY
 * 	This is an RPC client function that opens a window and executes a 
 * 	command in that window, returning the results to the calling process.
 *
 * CHANGE HISTORY
 *	06/27/90	Wayne Mock		Initial design and coding
 *	09/30/91	Wayne Mock		Ported to OSF/1
 *      06/15/92        Ram Balaraman           Modified to work correctly on 
 *                                              both AIX and OSF/1 platforms.
 */                                                                   

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <ropen_idl.h>

/*
 * These must always be last
 */
#include <dce/cma.h>
#include <dce/cma_ux.h>
#include <dce/cmalib_crtlx.h>
#include <dce/cma_px.h>
#include <dce/dce_error.h>

void exit();
#ifdef OSF_DCE  
extern uid_t getuid(void);
extern gid_t getgid(void);
#else
unsigned short getuid();
unsigned short getgid();
#endif
void perror();
char *getenv();


/*
 * these are needed for the RPC stuff
 */
globalref ropen_idl_v0_0_epv_t t_manager_vec;
globaldef ropen_idl_v0_0_epv_t t_manager_vec = {remoteopen};


char* waitSuffix = "; echo Hit enter to continue...; read ans";
char* noWaitSuffix = "";

#define MAXARGS		20

/*****************************************************************************
 * allocateMemory()
 *
 * DESCRIPTION
 *	Allocate buflen bytes of memory and return a pointer to it.
 *	If the allocation fails, determine the errno, print an error
 * 	message then exit the process.
 *
 * INPUT
 *	buflen		Specifies the length of the buffer to allocate
 *
 * OUTPUT
 *	none
 *
 * RETURN
 *	Returns a pointer to the new block of memory if successful; otherwise,
 *	it returns NULL.
 *
 ****************************************************************************/
char *
allocateMemory( buflen )
unsigned buflen;
{
	char *buf;		/* pointer to the newly allocate buffer */
/*
 * allocate the memory for the buffer.  report any errors and terminate if 
 * necesary.
 */
	buf = (char *)malloc( buflen );
	if( buf == NULL )
	{
		perror( "ropen()" );
		exit( errno );
	}

	return( buf );

} /* end allocateMemory() */


/*****************************************************************************
 * runUsersCommand()
 *
 * DESCRIPTION
 *	Forks a child process, changes the UID and GID of the child, does a
 *	system() call to execute the command.  Returns the error codes of 
 *	that command to its parent process.
 *
 * INPUT
 *	command		String containing the command to execute.
 *	cmdmsg		A structure that contains the UID and GID of the
 *			initiating user.
 *
 * OUTPUT
 *	None
 *
 * RETURN
 *	No function return value; however, the child process will exit with
 *	an error value.
 ****************************************************************************/
void
runUsersCommand( cmdmsg, args )
msg_struct *cmdmsg;
char* args[];
{
	int result;		/* result from the system() */
	int rc, pid;
	int child_pid, grandchild_pid, returned_pid;
	int status;

	child_pid = fork();
	if ( child_pid != 0 ) {
          /* iam the father and I will wait for my child to finish before
             returning */
	  returned_pid = 0;
          while (returned_pid != child_pid) {
	    returned_pid = wait(&status);
	  }
          return;
        }

        /* iam the child and my father is waiting for me to finish
           i will create another child and make the child do the work.
           I will exit without waiting for the child orphaning my child.
           The god (init process) will look after my child.
        */
        grandchild_pid = fork();
        if (grandchild_pid != 0) {
          /* iam the child, i have a son now and iam going to die, leaving
             my son in the hands of the god (init process)*/
          exit(0);
        }

        /* iam the grandchild. I have to do the real job. When I finish
           and become a zombie, the init process will clean me up and take
           me to heaven... */

		rc = getpid();
		status = setpgrp(rc, rc);
		
		if (status == -1) {             /* detach from parent */
                    (void)perror("setpgrp" );
		    exit(errno);
		  }

        if ( cmdmsg->uname == '\0' ){
          rc = setgid( cmdmsg->gid );
          if( rc == -1 ) {	
            perror( "setgid" );
            exit(errno);
          }
          rc = setuid( cmdmsg->uid );
          if( rc == -1 )	{
            perror( "setuid" );
            exit(errno);
          }
        }

#ifdef DEBUG_ROPEND
		{		
		char** a;
		for( a = args; *a != NULL; a++ ) {	
		    printf( "%s ", *a );
		}
		printf("\n" );
	        }
#endif /* DEBUG_ROPEND */

	/* 
	 * make the command call with the new list of args.
	 */
        execvp( *args, args );
		
#ifdef DEBUG_ROPEND
{		printf( "*** execvp failed with errno %d\n", errno );
}
#endif /* DEBUG_ROPEND */
	exit( result );
} /* end runUsersCommand() */


/*****************************************************************************
 * remoteopen()
 *
 * DESCRIPTION
 *	This opens a window an runs the command given by the initiating user.
 *	It determines which type of window to open, builds the appropriate
 *	command string.  Forks a new process whose UID and GID are changed to
 *	match the initiating users.  This new process opens a window
 *	and executes the command.  The parent process waits until its child
 *	is complete, then determines if the child failed or not.  It returns
 *	the result to the calling process.
 *
 * INPUT
 *	cmdmsg	A structure that contains all the necessary information
 *		to determine the correct window to open and command to
 *		run.
 * OUTPUT
 *	None.
 *
 * RETURN
 *	Returns a pointer to an integer.  The interger contains any error
 *	codes that may have been generated.
 *
 ****************************************************************************/
void
remoteopen( clientHandle, cmdmsg, result )
handle_t clientHandle;
msg_struct *cmdmsg;
long* result;
{	char *buf;		/* holds the command string */
	unsigned buflen;	/* lenght of buf in bytes */
	char *suffix;		/* last part of the command to execute */
	char* args[MAXARGS];
	char* execmd;
	int argcnt;

	execmd = ( cmdmsg->uname[0] == '\0' ) ? execmd = "sh" : "su.dce";

/*
 * determine the correct args[] and suffix to use based on the command line 
 * arguments passed from the ropen call made by the user.
 */
	switch( cmdmsg->ptype )
	{
	case ROPEN_BG:
		args[0] = execmd;
		argcnt = 1;
		if( cmdmsg->uname[0] != '\0' )
		{	args[argcnt++] = (char*)cmdmsg->uname;
			args[argcnt++] = "-p";
			args[argcnt++] = (char*)cmdmsg->upasswd;
		}
		args[argcnt++] = "-c";
		suffix = noWaitSuffix;
		break;
	case ROPEN_WIN: /* ROPEN_WIN is the default */
	default:
#if defined(SYSTYPE_sunos)
		args[0] = "shelltool";
		args[1] = "-I";
		argcnt=2;
#else /* any AIX or OSF/1 */
		args[0] = "/usr/bin/X11/xterm";
		args[1] = "-d";
		args[2] = (char*)cmdmsg->display;
		args[3] = "-T";
		args[4] = (char*)cmdmsg->name;
		args[5] = "-n";
		args[6] = (char*)cmdmsg->name;
		args[7] = "-sb";
		args[8] = "-sl";
		args[9] = "2000";     /* save 2000 lines of display */
		argcnt=10;
		if( cmdmsg->icon )
		{	args[argcnt++] = "-i";
		}
		args[argcnt++] = "-e";
		args[argcnt++] = execmd;
		if( cmdmsg->uname[0] != '\0' )
		{	args[argcnt++] = (char*)cmdmsg->uname;
			args[argcnt++] = "-p";
			args[argcnt++] = (char*)cmdmsg->upasswd;
		}
		args[argcnt++] = "-c";
		
#endif
		suffix = (cmdmsg->wait_flg) ? waitSuffix : noWaitSuffix;
		break;
	}
		
/*
 * compute the length of the entire command string.  That is equal to length
 * of the of the given command plus the length of the suffix plus 1 for the 
 * EOS character.
 */
	buflen = strlen(cmdmsg->command) + strlen(suffix) + 1;
	buf = allocateMemory( buflen );

/*
 * make the command buffer with the open command and the given string
 */
	sprintf( buf, "%s%s", cmdmsg->command, suffix );

	args[argcnt++] = buf;
	args[argcnt++] = NULL;

	if( argcnt > MAXARGS )
	{	fprintf( stderr, "Too many args! - Memory leak possible!\n" );
	}

/*
 * run the users command in a window
 */
	runUsersCommand( cmdmsg, args );

/* 
 * free the buffers.  They are no longer needed.
 */
	free( buf );
	*result = 0;

} /* end remoteopen() */


/*****************************************************************************
 * main()
 *
 * DESCRIPTION
 *	Parse the command line arguements and initialize global data.
 *	Initialize the message queue to establish two-way communication
 *	with the Path Generator.  Establish this programs as an rpc
 *	server and begin serving.
 *
 * INPUT PARAMETERS
 *	int argc	Number of command line arguments
 *	char* argv[]	All the command line arguments specified
 *
 * OUTPUT PARAMETERS
 *	none.
 *
 * RETURN VALUE
 *	int		Indicates whether the program had problems.
 *
 ****************************************************************************/
int 
main( argc, argv )
int argc;
char* argv[];
{	unsigned32 st;
	int status;
	rpc_binding_vector_t* bindings;
	char* string_binding;
	char error_string[dce_c_error_string_len];
	int errst;
	int MyPid;



        /* detach from control terminal */
	close(stdin);
	close(stdout);
	close(stderr);

#ifdef SNI_SVR4
        if( setpgrp() == -1 ) {
#else
        if( setpgrp( 0, getpid() ) == -1 ) {
#endif
	  (void)perror( "setpgrp" );
	  exit(0);
	}


	rpc_server_use_protseq_if((unsigned_char_p_t)"ip", 
				  (unsigned32)10, 
				  (rpc_if_handle_t)ropen_idl_v0_0_s_ifspec, 
				  (unsigned32*)&st);
	if (st != rpc_s_ok)
	{	
	        dce_error_inq_text( st, error_string, &errst );
		fprintf( stderr, "%s\n", error_string );
		exit(errno);
	}

	rpc_server_register_if(ropen_idl_v0_0_s_ifspec,
				NULL,
				(rpc_mgr_epv_t)&t_manager_vec,
				&st);
	if (st != rpc_s_ok)
	{	fprintf( stderr, "Cannot register interface" );
		exit(errno);
	}

	rpc_server_inq_bindings(&bindings, &st);
	if (st != rpc_s_ok)
	{	fprintf( stderr, "Cannot inquire bindings" );
		exit(errno);
	}

	rpc_binding_to_string_binding((rpc_binding_handle_t)bindings->binding_h[0], 
				      (unsigned_char_p_t*)&string_binding,
				      (unsigned32*)&st);
	if (st != rpc_s_ok)
	{	fprintf( stderr, 
			    "Cannot convert binding to string binding" );
		exit(errno);
	}

#ifdef DEBUG_RPC
        rpc__dbg_set_switches( "0-3.10", &st );
#endif

	rpc_server_listen(1, &st);

	return 0;

} /* end main() */

/*****************************************************************************
 *                    		  END OF FILE
 ****************************************************************************/




