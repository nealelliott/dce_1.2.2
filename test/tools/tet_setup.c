/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: tet_setup.c,v $
 * Revision 1.1.4.2  1996/02/17  23:32:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:52  marty]
 *
 * Revision 1.1.4.1  1995/12/11  23:06:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:37:23  root]
 * 
 * Revision 1.1.2.3  1994/06/10  20:51:28  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:20:15  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/26  18:06:20  pwang
 * 	Added optional [-x] switch to ignore the returned error status
 * 	[1994/05/26  18:01:17  pwang]
 * 
 * Revision 1.1.2.1  1994/04/29  21:03:04  pwang
 * 	Initial Implementation
 * 	[1994/04/29  21:01:37  pwang]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dce/dce.h>
#include <dce/sec_login.h>

extern char **environ;

void
main(int argc, char **argv) 
{
	sec_login_handle_t	login_context;
	error_status_t		status;
	int			ignore;
	int			i;

	ignore = 0;
	while ((i = getopt(argc, argv, "x")) != -1)
		switch (i) {
		default:
			exit(1);
			/* NOTREACHED */
		case 'x':
			ignore = 1;
			break;
		}
	argc -= optind;
	argv += optind;

	/*
	** If nothing to setup, just return
	*/
	if (argc < 1) {
		fprintf(stderr, 
			"Usage: tet_setup [-x] setup_program args ...\n");
		exit(1);
	}
	/*
	** Release the user's login context if one is
	** dced_login'd
	*/
	sec_login_get_current_context(&login_context, &status);
	if (status == error_status_ok) {
		sec_login_release_context(&login_context, &status);	
		if (status != error_status_ok) {
			dce_fprintf(stderr, status);
			exit(1);
		}
	}
	/*
	** Setup root
	*/
	if (setuid(0) < 0) {
		perror("setuid");
		exit(1);
	}
	sec_login_get_current_context(&login_context, &status);
	if (status != error_status_ok) {
		dce_fprintf(stderr, status);
		if (ignore) {
			fprintf(stderr, "Continuing anyway...\n");
		} else {
			exit(1);
		}
	}
	/*
	** Exec the setup program
	*/
	execvp(argv[0], &argv[0]);
	/*
	** If we get here, the setup program failed
	*/
	perror(argv[0]);
	exit(1);
}
