/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20exec.c,v $
 * Revision 1.1.10.2  1996/02/18  19:41:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:30  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:19:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:30  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:47  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:12:45  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:25  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:35:01  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:40:44  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:37:03  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:09  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:06:22  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  14:57:10  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:10:35  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:59:16  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:42:48  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20exec.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:41:46 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***********************************************************************/
/*                                                                            */
/* NAME         : d20exec.c                                                   */
/*                                                                            */
/* AUTHOR       : R.Horn, D AP 11                                             */
/* DATE         : 12.11.90                                                    */
/*                                                                            */
/* DESCRIPTION  :                                                             */
/*                This program can be used to start up any other program      */
/*                which requires to run under the uid and gid of the          */
/*                directory system installation.                              */
/*                Calling syntax:                                             */
/*                      d20exec <program name> [<program calling parameter>]  */
/*                                                                            */
/*datoff **********************************************************************/

#include <gds.h>
#include <sys/types.h>
#include <stdio.h>
#include <pwd.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d20proto.h>


/* ----------------- DEFINITION OF CONSTANTS ------------------------------- */

#define	MAX_PARAM	20
#define AWK_CMD		"awk -F= '$1 == \"D2_USER\" { printf \"%%s\", substr($2,\
			0, index($2, \";\")-1); exit }' %s%s"
#define MAX_TMP_CMD	500

/* ------------------------------------------------------------------------- */

main (
  int argc,
  char * argv[])
{
	signed32	i ;
	FILE		*fd ;
	char            install_dir[D27_LFILE_NAME];
	char            *p = install_dir, tmpbuf[MAX_TMP_CMD] ;
	struct passwd   *pw;

	/* verify no. of calling arguments */
	if (argc > MAX_PARAM-1)
		exit (1) ;

	/* get installation path of the directory system */
	sprintf(p, "%s%s", dcelocal_path, D27_CLIENT_DIR);

	/* get user name of the directory system installation */
	sprintf (tmpbuf, AWK_CMD, p, "/.profile") ;
	if ((fd = popen (tmpbuf, "r")) == (FILE *) NULL)
		exit (3) ;

	fgets (tmpbuf, 25, fd) ;
	pclose (fd) ;

	/* get user id and group id */
	if ((pw = getpwnam (tmpbuf)) == (struct passwd *) NULL)
		exit (4) ;

	/* set user id and group id to directory system (installation user) level */
	setuid (pw->pw_uid) ;
	setgid (pw->pw_gid) ;

	/* copy calling parameter into new parameter block */
	for (i = 1, p = tmpbuf; i < argc; i++)
	{   strcpy(p, argv[i]);
	    p += strlen(p);
	    *p++ = ' ';
	}
	*p = D2_EOS;

	/* execute command */
	if (system(tmpbuf) != 0)
		exit (5) ;

	exit(0);
}
