/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavrosprt.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:22  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:09:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:15  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:41:00  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:05:17  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:13:13  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:09:15  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:12:23  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:55:12  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:22:58  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:34:27  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavrosprt.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:27 $";
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
#include "mavrospro.h"

extern struct tstrd * strd;
extern int compindex;
extern struct comp * complist;
extern char * filein;
extern char * package;
#ifndef XOS
/* char * sprintf();  */
#endif

extern int default_tag;
extern char ** exported;
extern int nb_exported;
extern char ** defined_proc_list;
extern int defined_proc_list_size;

tree_print()
{	int i, last;
	char fname[256];

	(void)sprintf(fname,"%s.pretty",package);
	if (freopen(fname,"w",stdout)==0){
		perror(fname);
		return;
	}
	(void)printf("%s DEFINITIONS %s TAGS ::=\nBEGIN\n",
	package, (default_tag)?"EXPLICIT":"IMPLICIT");

	for (i=0,last=0; i<compindex; i++){
		if( complist[i].c_type == T_IMPORT
		&&  complist[i].c_index != 0){
			last = i+1;
		}
	}
	if (last){
		(void)printf("IMPORTS\n");
		for (i=0; i<last; i++){
			if (complist[i].c_type != T_IMPORT)
				continue;
			(void)printf("%s ", complist[i].c_name);
			if (complist[i].c_first_son){
				(void)printf("(");
				(void) arg_print(complist[i].c_first_son);
				(void)printf(") ");
			}
			if (complist[i].c_index)
				(void) printf ("FROM %s\n",
				complist[i].c_index);
			else	(void) printf (",\n");
		}
		(void)printf("\n");
	}

	if (nb_exported){
		(void)printf("EXPORTS\n");
		for (i=0; i<nb_exported; i++){
			if (i) (void)printf(",\n");
			(void)printf("\t%s", exported[i]);
		}
		(void)printf(";\n\n");
	}

	if (defined_proc_list_size){
		(void)printf("DEFINED\n");
		for (i=0; i<defined_proc_list_size; i++){
			if (i) (void)printf(",\n");
			(void)printf("\t%s", defined_proc_list[i]);
		}
		(void)printf(";\n\n");
	}

	for (i=last;i<compindex;i++){
		if (complist[i].c_father != -1) continue;
		switch(complist[i].c_type){
		case T_IMPORT:	
		case T_TYPE:	type_print(i); break;
		default:	(void)printf("\n????????????\n");
				printcomp(i,0);
				(void)printf("\n\n");
		}
	}
	(void)printf("END\n");
}

type_print(i)
int i;
{	int j, flag = 0;

	(void)printf("%s (",complist[i].c_name, complist[i].c_index);
	j = arg_print(complist[i].c_first_son);
	(void)printf(") ::= ");
	if (complist[i].c_optional){
		mode_print(i);
		(void)printf(" IMPORTS\n");
		flag = 1;
	}else	(void)printf("\n");

	while (j != -1){
		if (flag)
			(void)printf("\n?????????????\n");
		else	flag = 1;
		printcomp(j,1);
		j = complist[j].c_brother;
	}
	(void)printf("\n\n");
}

arg_print(i)
int i;
{	int flag = 0;
	if (i != -1 
	&& complist[i].c_type == T_PARAMETER 
	&& complist[i].c_brother != -1){
		if (complist[complist[i].c_brother].c_type == T_PARAMETER)
			(void)printf("\n\t");
	}
	while (i != -1){
		if (complist[i].c_type != T_PARAMETER)	return(i);
		if (flag)	(void)printf(";\n\t");	else flag = 1;
		switch (complist[i].c_call_mode){
		case 1:	(void)printf("VAR "); break;
		case 2: (void)printf("VALUE "); break;
		default: break;
		}
		(void)printf("%s : %s",
		complist[i].c_name,complist[i].c_index);
		i = complist[i].c_brother;
	}
	return(-1);
}

printcomp (i,j)
int i,j;
{
	int k,k0,l,m, defaulted = 0;

	if (complist[i].c_type != T_SET_OF 
	&& complist[i].c_type != T_SEQUENCE_OF){
		mode_print(i);
	}
	switch (complist[i].c_type){
	case T_CHOICE:
		(void)printf("CHOICE\t"); 
		item_print(complist[i].c_first_son);
		(void)printf(" {\n");
		for (k=complist[complist[i].c_first_son].c_brother;
		k != -1;k=complist[k].c_brother){
			if (complist[k].c_type == T_ALLOC
			||  complist[k].c_type == T_DEFAULT
			||  complist[k].c_type == T_FINAL
			||  complist[k].c_type == T_INIT) break;
			for (l=0;l<j;l++) (void)printf("\t");
			if (complist[k].c_index)
				(void)printf("%s: ", complist[k].c_index);
			else	(void)printf("DEFAULT: ");
			printcomp(k,j+1);
			if (complist[k].c_brother != -1){
				(void)printf(",\n");
			}
		}
		(void)printf("}");
		break;
	case T_SEQUENCE:
		(void)printf("SEQUENCE {\n"); 
		for (k=0;k<j;k++) (void)printf("\t");
		goto moreseq;
	case T_SET:	
		(void)printf("SET {\n"); for (k=0;k<j;k++) (void)printf("\t");
	moreseq:
		for (k=complist[i].c_first_son;
		k != -1;k=complist[k].c_brother){
			if (complist[k].c_type == T_ALLOC
			||  complist[k].c_type == T_DEFAULT
			||  complist[k].c_type == T_FINAL
			||  complist[k].c_type == T_INIT) break;
			printcomp(k,j+1);
			if (complist[k].c_brother != -1){
				(void)printf(",\n");
			for (l=0	;l<j;l++) (void)printf("\t");
			}
		}
		(void)printf("}");
		break;
	case T_SEQUENCE_OF: (void)printf("SEQUENCE");
		goto sequence_of;
	case T_SET_OF:	(void)printf("SET");
	sequence_of:
		(void)printf(" OF ");
		printcomp(complist[i].c_first_son,j+1);
		(void)printf("\n");	for (k=0;k<j;k++) (void)printf("\t");
		(void)printf("INDEXED BY %s UP TO ", complist[i].c_name);
		item_print (k=complist[complist[i].c_first_son].c_brother);
		k=complist[k].c_brother;
		break;
	case T_IGNORE:
		(void)printf("IGNORE"); k=complist[i].c_first_son; break;
	case T_NULL:
		(void)printf("NULL");  k=complist[i].c_first_son; break;
	case T_COMP:
		(void)printf("COMPONENTS OF ");
	default:
		(void)printf("%s ",complist[i].c_name);
		for (k=k0=complist[i].c_first_son;
		k != -1;k=complist[k].c_brother){
			if (complist[k].c_type != T_VALUE) break;
			if (k!=k0)
				(void)printf(",\n");
			else	(void)printf("{\n");
			for (l=0;l<j;l++) (void)printf("\t");
			(void)printf("%s(%s)",
			complist[k].c_name,complist[k].c_index);
		}
		if (k!=k0){
			(void)printf("}\n");
			for (l=0;l<j;l++) (void)printf("\t");
		}
		(void)printf ("(");
		for (k0=k;k != -1;k=complist[k].c_brother){
			if (complist[k].c_type == T_ALLOC
			||  complist[k].c_type == T_DEFAULT
			||  complist[k].c_type == T_INIT) break;
			if(k != k0){
				(void)printf(",\n");
				for (l=0;l<j;l++) (void)printf("\t");
			}
			item_print(k);
		}
		(void)printf(")");
		break;
	}
	while (k != -1){
		(void)printf("\n");
		for (l=0;l<j-1;l++) (void)printf("\t");
		if (complist[k].c_type == T_ALLOC){
			(void)printf("ALLOCATE(");
			for (l=complist[k].c_first_son;l != -1;l=complist[l].c_brother){
				item_print(l);
				(void)printf(" : %s", complist[l].c_index);
				if (complist[l].c_brother != -1){
					(void)printf(",\n");
					for (m=0;m<j;m++) (void)printf("\t");
				}
			}
			(void)printf(")");
		}else if (  complist[k].c_type == T_INIT
			||  complist[k].c_type == T_FINAL
			||  complist[k].c_type == T_DEFAULT ){
			if (complist[k].c_type == T_DEFAULT){
				defaulted = 1;
				(void)printf("DEFAULT(");
			}else if (complist[k].c_type == T_FINAL){
				(void)printf("FINAL(");
			}else	(void)printf("INITIAL(");
			for (l=complist[k].c_first_son;
			l != -1;l=complist[l].c_brother){
			if (complist[l].c_type == T_INIT_STR){
				quoted_string_print(complist[l].c_name);
			}else{
				item_print(complist[l].c_first_son);
				(void)printf(" = ", complist[l].c_index);
				item_print(
				complist[complist[l].c_first_son].c_brother);}
			if (complist[l].c_brother != -1){
					(void)printf(";\n");
					for (m=0;m<j;m++) (void)printf("\t");
				}
			}
			(void)printf(")");
		}else	(void)printf("??????????");
		k = complist[k].c_brother;
	}
	if (complist[i].c_optional && !defaulted)	(void)printf(" OPTIONAL");
	return;
}

quoted_string_print(s)
char * s;
{	register x;

	putc('"',stdout);
	while (x = *s++){
		if (x == '"')	putc('"',stdout);
		putc(x,stdout);
	}
	putc('"',stdout);
}

item_print(i)
int i;
{	int j;

	(void)printf("%s",complist[i].c_name);
	for(j=complist[i].c_first_son; j!=-1; j=complist[j].c_brother){
		if (complist[j].c_type == T_ARGUMENT){
			(void)printf("[");
			item_print(j);
			(void)printf("]");
		}
		else 	(void)printf("%s%s",
			(complist[j].c_call_mode)?"->":".", complist[j].c_name);
	}
}

mode_print(i)
int i;
{
	if (complist[i].c_mode != -1){
		if (complist[i].c_id)	(void)printf("%s",complist[i].c_id);
		switch (complist[i].c_mode){
		case 0:	(void)printf("[UNIVERSAL %d] ",complist[i].c_number);
			break;
		case 1:	(void)printf("[APPLICATION %d] ",complist[i].c_number);
			break;
		case 2:	(void)printf("[%d] ",complist[i].c_number);
			break;
		case 3:	(void)printf("[PRIVATE %d] ",complist[i].c_number);
			break;
		}
		if (complist[i].c_implicit) (void)printf("IMPLICIT ");
	}
}
