/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavcod.c,v $
 * Revision 1.1.11.2  1996/02/18  19:38:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:07  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:08:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:46  root]
 * 
 * Revision 1.1.9.1  1994/02/22  15:36:47  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:01:50  marrek]
 * 
 * Revision 1.1.7.2  1993/08/11  13:52:10  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:05:16  marrek]
 * 
 * Revision 1.1.5.3  1992/12/31  17:10:54  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:58  bbelch]
 * 
 * Revision 1.1.5.2  1992/12/17  23:11:52  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:06:53  tom]
 * 
 * Revision 1.1.2.2  1992/06/01  20:20:52  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:22:06  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavcod.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 19:38:12 $";
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
#include "mavcod.h"

char * file_name = STD_INPUT_NAME;
char * pretty_name = 0;
char * header_name = 0;
char * mvr_name = 0;
char * indent = "\t";

extern int yynerrs; /*AF*/

main(argc,argv)
int argc;
char ** argv;
{	int *list, nb_in_list, i, auto_check = 0;
	FILE * f_out;

	setlocale(LC_ALL, "");

	for (i=1; i<argc;){
		if (strcmp("-check",argv[i]) == 0){
			auto_check = 1;
			i+=1;
			continue;
		}
		if (i+1 >= argc) goto usage;
		if (strcmp("-srce",argv[i]) == 0){
			if (freopen(argv[i+1],"r",stdin)==0){
				perror(argv[i+1]);
				goto usage;
			}
			file_name = argv[i+1];
		}else
		if (strcmp("-asn1",argv[i]) == 0){
			pretty_name = argv[i+1];
		}else
		if (strcmp("-pretty",argv[i]) == 0){
			pretty_name = argv[i+1];
		}else
		if (strcmp("-head",argv[i]) == 0){
			header_name = argv[i+1];
		}else
		if (strcmp("-mvr",argv[i]) == 0){
			mvr_name = argv[i+1];
		}else
		if (strcmp("-indent",argv[i]) == 0){
			indent = argv[i+1];
		}else goto usage;
		i+=2;
	}
	initialise_tree();
	(void) yyparse();
	if (yynerrs > 0){   /*AF*/
		(void)fprintf(stderr,
		"( MAVCOD )  -->  %d syntax error%s .\n",
		yynerrs,(yynerrs > 1)?"s":"");
		exit(1);
	}

	/* Pretty print the ASN-1 file, if requested */
	if (auto_check){
		print_tree("   ",stdout);
	}else if (pretty_name){
		if (f_out = fopen(pretty_name,"w")){
			print_tree(indent,f_out);
			(void) fclose(f_out);
		}else{	perror(pretty_name);
			goto usage;
		}
	}

	/* Look for optional components, that should be promoted */
	look_for_optional(0,0);
	if (auto_check){
		printf("\n\n----Same module, after promotions----\n\n");
		print_tree("   ",stdout);
	}

	/* Sort the components and edit the header file */
	nb_in_list = build_ordered_list(&list);
	if (auto_check){
		printf("\n\n----Build header file----\n\n");
		generate_types(stdout, "\t", list, nb_in_list);
	}else if (f_out = fopen(header_name,"w")){
		generate_types(f_out, indent, list, nb_in_list);
		(void) fclose(f_out);
	}else{	perror(header_name);
		goto usage;
	}
	/* Produce the MAVROS input */
	if (auto_check){
		printf("\n\n----Build mavros file----\n\n");
		mvr_package(stdout,"\t");
	}else if (f_out = fopen(mvr_name,"w")){
		mvr_package(f_out,indent);
		(void) fclose(f_out);
	}else{	perror(mvr_name);
		goto usage;
	}
	exit(0);
usage:
  fprintf(stderr,"Usage: %s [-srce file] [-pretty file]\n", argv[0]);
  fprintf(stderr,"[-head file] [-mvr file] [-indent string]\n");
  exit(1);
}
