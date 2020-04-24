/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: print.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:10  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:08:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:52  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:37:49  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:02:36  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:57:36  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:06:04  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:11:12  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:54:16  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:21:16  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:32:19  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: print.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:14 $";
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
#include "mavcod.h"

extern char * package_name ;
extern int tag_mode;

extern char ** export_list;
extern unsigned int nb_exported;

extern char ** import_list;
extern unsigned int nb_imported;

extern int * import_limit;
extern char ** import_module;
extern unsigned int nb_import_module;

char * mode_id[] = {
"UNIVERSAL ",
"APPLICATION ",
"",
"PRIVATE "};

char * implicit_id[] = {
"IMPLICIT ",
"EXPLICIT "};

print_element(x,depth,indent,f_out)
struct asn1_tree * x;
int depth;
char * indent;
FILE * f_out;
{	int brother;

	switch (x->a_type){
	case T_TYPE:
		fprintf(f_out, "%s ::= ", x->a_name);
		if (x->a_first_son == -1)
			fputs("{}",f_out);
		else 	print_element(&tree_element[x->a_first_son],
			depth,indent,f_out);
		break;
	case T_INT_VALUE:
		fprintf(f_out,"%s(%s)",x->a_id, x->a_name);
		break;
	case T_COMP:
		fprintf(f_out,"COMPONENTS OF %s", x->a_name);
		break;
	default:
		if (x->a_id) fprintf(f_out,"%s ",x->a_id);
		if (x->a_mode != C_UNSPEC){
			fprintf(f_out,"[%s%d] ",
			mode_id[x->a_mode], x->a_num);
		}
		if (x->a_implicit != F_UNDEF)
			fputs(implicit_id[x->a_implicit],f_out);

	      /*  fputs(x->a_name,f_out);       */
	      /*  ersetzt durch */
	       if (x->a_type == T_ANYDEF)
			fprintf(f_out,"ANY DEFINED BY ");
	       if (x->a_name)
			fputs(x->a_name,f_out);
	    /* 14.01.91 H.T. i.A. Huitema */
		if ((brother = x->a_first_son) != -1){
			if (x->a_type != T_SET_OF
			&&  x->a_type != T_SEQUENCE_OF)
				putc('{',f_out);
			for(;;) {
				putc('\n',f_out);
				do_indent(f_out, depth, indent);
				print_element(&tree_element[brother],
				depth + 1, indent, f_out);
				brother = tree_element[brother].a_brother;
				if (brother != -1)
					putc(',',f_out);
				else	break;
			}
			if (x->a_type != T_SET_OF
			&&  x->a_type != T_SEQUENCE_OF)
				putc('}',f_out);
		}
		switch(x->a_option){
		case O_DEFAULT:	
			fprintf(f_out," DEFAULT(%s)",x->a_default);
			break;
		case O_OPTION:
			fputs(" OPTIONAL",f_out);
		default:
			break;
		}
	}
}

do_indent(f_out, depth, indent)
FILE * f_out;
int depth;
char * indent;
{	int i;

	for (i=0; i<depth; i++) fputs(indent,f_out);
}

print_tree(indent,f_out)
char * indent;
FILE * f_out;
{	int i, m;

	fprintf(f_out,"%s DEFINITIONS ", package_name);
	if (tag_mode != F_UNDEF) fputs(implicit_id[tag_mode],f_out);
	fputs("::= BEGIN\n",f_out);

	if (nb_exported){
		fputs("EXPORTS\n",f_out);
		for (i=0;;){
			do_indent(f_out,1,indent);
			fputs(export_list[i],f_out);
			i++;
			if (i < nb_exported)
				fputs(",\n",f_out);
			else{	fputs(";\n",f_out);
				break;
			}
		}
		putc('\n',f_out);
	}

	if (nb_imported){
		fputs("IMPORTS\n",f_out);
		for (i=0, m=0;;){
			do_indent(f_out,1,indent);
			fputs(import_list[i],f_out);
			i++;
			if (i == import_limit[m]){
				fprintf(f_out," FROM %s",import_module[m++]);
				if (i == nb_imported){
					fputs(";\n",f_out);
					break;
				}else	putc('\n',f_out);
			}else	fputs(",\n",f_out);

		}
		putc('\n',f_out);
	}

	for(i = tree_element[0].a_first_son;
	i != -1;
	i = tree_element[i].a_brother){
		print_element(&tree_element[i],1,indent, f_out);
		fputs("\n\n",f_out);
#ifdef DEBUG
		fflush(f_out);
#endif
	}
	fputs("END\n",f_out);
}

#ifndef lint
dump_tree()
{
	print_tree("\t",stdout);
}
#endif
