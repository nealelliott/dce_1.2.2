/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: getip.c,v $
 * Revision 1.1.7.2  1996/02/18  19:19:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:20  marty]
 *
 * Revision 1.1.7.1  1995/12/07  21:40:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:57:15  root]
 * 
 * Revision 1.1.5.1  1994/06/09  16:05:35  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:14  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/18  21:28:41  tom
 * 	New program to replace getip function in dce_config. (Bug #6424)
 * 	[1992/12/18  21:27:33  tom]
 * 
 * $EndLog$
 */
#if !defined(lint) && !defined(_NOIDENT)
static char rcsid[] = "@(#)$RCSfile: getip.c,v $ $Revision: 1.1.7.2 $ (OSF) $Date: 1996/02/18 19:19:53 $";
#endif

#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

main(argc, argv)
	int argc;
	char **argv;
{
	struct hostent *hp;
	struct sockaddr_in sin;


	if (argc != 2) {
		fprintf(stderr, "Usage: getip host\n");
		exit(1);
	}

	if (!(hp = gethostbyname(argv[1]))) {
		exit(1);
	}
	memcpy(&sin.sin_addr, hp->h_addr, sizeof(sin.sin_addr));
	printf("%s\n", inet_ntoa(sin.sin_addr));
	exit (0);
}
