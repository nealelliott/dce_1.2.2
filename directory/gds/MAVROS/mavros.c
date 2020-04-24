/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavros.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:11  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:09:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:58  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:38:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:03:00  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:59:58  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:06:28  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  17:11:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:54:25  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  23:11:55  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:06:58  tom]
 * 
 * Revision 1.1.4.2  1992/11/27  17:55:35  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  13:02:01  marrek]
 * 
 * Revision 1.1.2.3  1992/06/01  20:21:31  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:33:09  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/09  20:05:58  jim
 * 	change rindex function to strrchr for AIX 3.2.
 * 	[1992/04/09  19:29:37  jim]
 * 
 * Revision 1.1  1992/01/19  15:11:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavros.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:16 $";
#endif

 
/* ****************************************************************** */
/*                                                                    */
/*   COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONSSYSTEME AG   */
/*                           AND INRIA                                */
/*   All rights reserved                                              */
/*                                                                    */
/*   This software is furnished under licence and may be used only    */
/*   in accordance with the terms of that licence and with the        */
/*   inclusion of the above copyright notice.                         */
/*   This software may not be provided or otherwise made available    */
/*   to, or used by, any other person. No title to or ownership of    */
/*   the software is hereby  transferred.                             */
/*                                                                    */
/* ****************************************************************** */

#include <stdio.h>
#include <locale.h>
#include <string.h>
#include "mavrospro.h"

char * filein = (char *) 0, * package = (char *) 0;
extern int errcnt;
extern int def_list_nb;
extern char ** def_list;
int  srce_list_nb = 0;
char ** srce_list;
int compilation_mode = 0;
int session_protocol_mode = 0;
int debuglevel = 0;
extern int nb_exported;
extern int dont_numberline;
int af_anydef = 0;

main(argc,argv)
int argc;
char ** argv;
{	int i;
	char *x;


	setlocale(LC_ALL, "");

	if (argc < 2) goto usage;
	filein = argv[1];
	for (i=2;i<argc;i++){
		x = argv[i];
		if (strcmp(x,"-x")==0){
			if (++i < argc)
				debuglevel = atoi(argv[i]);
			continue;
		}
		if (strcmp(x,"-p")==0){
			compilation_mode |= PRETTY;
			continue;
		}
		if (strcmp(x,"-m")==0){
			if ((i+1) < argc && *argv[i+1] != '-')
				package = argv[++i];
			compilation_mode |= ASN1CODER | INCLUDE | MAKEFILE;
			continue;
		}
                if (strcmp(x,"-a")==0){
                        compilation_mode |= INOUT;
                        continue;
		}
                if (strcmp(x,"-c")==0){
                        compilation_mode |= COPYING;
                        continue;
		}
		if (strcmp(x,"-w")==0){
			compilation_mode |= LIGHTWEIGHT;
			continue;
		}
		if (strcmp(x,"-u")==0){
			compilation_mode |= ONE_FILE;
			continue;
		}
		if (strcmp(x,"-session")==0){
			session_protocol_mode = 1;
			continue;
		}
		if (strcmp(x,"-d")==0){
			if (++i < argc)
				i += (def_list_nb = file_list(
				def_list = &argv[i],argc - i)) - 1;
			continue;
		}
		if (strcmp(x,"-s")==0){
			if (++i < argc){
				i += (srce_list_nb = srce_names(
				srce_list = &argv[i],argc - i)) - 1;
			}
			continue;
		}
		if (strcmp(x,"-i")==0){
			i++;
			while (i < argc && (*(argv[i]) != '-'))
				one_more_include(argv[i++]);
			continue;
		}
		if (strcmp(x,"-nolinenum")==0){
			dont_numberline = 1;
			continue;
		}
		if (strcmp(x,"-af_anydef")==0){
			af_anydef = 1;
			continue;
		}

		(void)fprintf(stderr,"Unknown argument: %s\n",argv[i]);
		goto usage;
	}
	if (strcmp(filein,"-")){
		(void) freopen(filein,"r",stdin);
	}

	errcnt = 0;
	parse_input();


	if (compilation_mode&PRETTY) 
		tree_print();
	if (errcnt){
		(void)fprintf(stderr,
		"(( MAVROS )) --> %d syntax error%s; no generation of code.\n",
		errcnt,(errcnt > 1)?"s":"");
		exit(1);
	}

	if (compilation_mode&INCLUDE) 
		if (generincl() < 0) goto usage;

	if (compilation_mode&ONE_FILE
	&&  compilation_mode&(ASN1CODER|INOUT|LIGHTWEIGHT|COPYING))
		if (open_single_output_file() < 0)
			goto usage;

	if (compilation_mode&ASN1CODER) 
		if (genercode() < 0) goto usage;

	if (compilation_mode&LIGHTWEIGHT) 
		if (generlw() < 0) goto usage;

	if (compilation_mode&INOUT) 
		if (generinout() < 0)
			goto usage;

	if (compilation_mode&COPYING)
		if (genercpy() < 0)
			goto usage;

	if (compilation_mode&MAKEFILE) 
		if (genermake() < 0)
			goto usage;

	exit(0);
usage:
	(void)fprintf(stderr,
	"Usage: mavros input [-m [package]] [-p] [-x debug_level]\\\n");
	(void)fprintf(stderr,
	"[-d definition_files] [-s source_files] [-i header_files]\n");
	(void)fprintf(stderr,
	"[-a] [-w] [-u]\n");
	exit(1);
}


file_list(argv,m)
char ** argv;
int m;
{	int i=0;


	do {
		if (*argv[i] == '-') break;
		i++;
	}while (i < m);
	return(i);
}

srce_names(argv,m)
char ** argv;
int m;
{	int i=0;
	char * x;

	do {
		if (*argv[i] == '-') break;
		if (((x = strrchr(argv[i],'.')) != NULL) && strcmp(x,".c") == 0)
			*x = 0;
		i++;
	}while (i < m);
	return(i);
}
