/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20util.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:16  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:22:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:04  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:07  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:20  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:04:19  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:39:17  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  11:08:31  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:42:55  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:11:19  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:08:07  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  23:12:22  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:07:44  tom]
 * 
 * Revision 1.1.4.2  1992/11/27  15:41:52  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:47:44  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:02:31  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:46:24  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20util.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:28 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*****************************************************************************/
/*                                                                           */
/*   Program: d20util                                                        */
/*                                                                           */
/*   Short description:                                                      */
/*       The functionality of this utility depends from the first argument.  */
/*                                                                           */
/*       If the first argument is equal to -k then the program initiates     */
/*       the termination of the directory system processes (by sending the   */
/*       specified signal to them). For doing this, the program must always  */
/*       run under the user-id (UID) of root, because some directory system  */
/*       processes are running under the user-id of root, too.               */
/*                                                                           */
/*       If the first argument is equal to -s then the program interpretes   */
/*       the further arguments as file names and writes information about    */
/*       this files (file size and file name) to the standard output.        */
/*                                                                           */
/*   Calling parameter:                                                      */
/*                                                                           */
/*          if argv[1] = -k                                                  */
/*                      argv[2] = signal no.                                 */
/*                      argv[3] = 1. process-id                              */
/*                        ...                                                */
/*                      argv[n] = n. process-id                              */
/*                                                                           */
/*          if argv[1] = -s                                                  */
/*                      argv[2] = name of 1. file                            */
/*                        ...                                                */
/*                      argv[n] = name of n. file                            */
/*                                                                           */
/* ------------------------------------------------------------------------  */
/*   AUTHOR: R.Horn                                      DATE: 22.05.89      */
/*****************************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <locale.h>

#include <d20proto.h>


 int main (
   int	argc ,
   char	*argv[] )
{
	int		i, sig_no ;
	struct stat	info ;

	setlocale(LC_ALL, "");

	if (argc < 2 || argv[1][0] != '-') {
		fprintf (stderr, "%s: Usage: %s -<k|s> <additional arguments>\n",
							argv[0], argv[0]) ;
		exit (1) ;
	}

	switch ((int) argv[1][1]) {
	case 'k':
		/* send specified signal to all given processes */
		if (argc < 3) {
			printf ("Usage: %s <signal no> <pid1> [<pid2> ... <pidn>]\n", argv[0]) ;
			exit (1) ;
		}
		for (i = 3, sig_no = atoi (argv[2]); i < argc; i++) {
			if (kill (atoi (argv[i]), sig_no) < 0) {
				perror (argv[i]) ;
				exit (1) ;
			}
		}
		break ;
	case 's':
		/* get size of all given files */
		for (i = 2; i < argc; i++) {
			if (stat (argv[i], &info) < 0) {
				perror (argv[i]) ;
				exit (1) ;
			}
			fprintf (stdout, "%12.1ld %s\n", info.st_size, argv[i]) ;
		}
		break ;
	default:
		fprintf (stderr, "%s: Usage: %s -<k|s> <additional arguments>\n",
							argv[0], argv[0]) ;
		exit (1) ;
		break ;
	}
	exit (0) ;
}
