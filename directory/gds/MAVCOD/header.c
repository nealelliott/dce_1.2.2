/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: header.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:05  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:08:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:43  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:36:23  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:01:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:50:14  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:04:57  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:10:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:51  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:54:08  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  13:01:24  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:20:43  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:21:58  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: header.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:10 $";
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


/***************************************************************************\
* 									    *
* Generation of the header file.					    *
* 									    *
* One of the big problems here is to maintain consistency with the	    *
* MAVROS input. This is achievd by filling in a special token for each	    *
* ``extern'' or ``basic'' element, i.e. the name of the parameter by which  *
* the element will be called. In principle, this name is derived from	    *
* the ``id'' assigned to the element; if this ``id'' is absent, a somewhat  *
* random name will be assigned.						    *
* 									    *
\***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "mavcod.h"

generate_types(f_out, tabulation, list, nb_in_list)
FILE * f_out;
int *list, nb_in_list;
char * tabulation;
{	int i,j;

        /*AF erg. wg. COPY*/
	/* Removed by C. Huitema, 11 feb 1991
	fprintf(f_out,"typedef unsigned char UC;");
	fprintf (f_out,"\n");
	*/

	for (i=0; i<nb_in_list; i++){
		fprintf(f_out,"typedef ");
		j = tree_element[list[i]].a_first_son;
		tree_element[j].a_ext_id = tree_element[list[i]].a_name;
		generate_structure(f_out,j,0,tabulation,
		&list[i], nb_in_list - i,0); /* aend.14.01.91 H.T. iA v.Huitema */
		fprintf (f_out,"\n");
#ifdef DEBUG
		fflush(f_out);
#endif
	}
}

generate_struct_head(f_out, element)
FILE * f_out;
int element;
{
	fprintf(f_out,"struct");
	if (tree_element[tree_element[element].a_father].a_type == T_TYPE)
		fprintf(f_out," %s",
		tree_element[tree_element[element].a_father].a_name);
}

generate_structure(f_out,
	element, 
	depth, tabulation, 
	list, nb_in_list, 
	need_pointer)
FILE * f_out;
int element, depth, *list, nb_in_list, need_pointer;
char * tabulation;
{	int i, rank;
	char * s;
	struct asn1_tree * x = &tree_element[element];

	switch (x->a_type){
	case T_SET_OF:
	case T_SEQUENCE_OF:
		if ((s = tree_element[x->a_first_son].a_id) == 0)
			s = "v";
		tree_element[x->a_first_son].a_ext_id = s;
		break;
	default:
		for (i=x->a_first_son, rank = 1; i != -1;
		i = tree_element[i].a_brother, rank++){
			if ((s = tree_element[i].a_id) == 0){
				s = malloc((unsigned)16);
				(void)sprintf(s,"x_%d",rank);
			}
			tree_element[i].a_ext_id = s;
		}
	}

	if (x->a_option == O_OPTION || x->a_type== T_ANYDEF )
                                    /*AF notwendig fuer ANY DEF...*/
		need_pointer = 1;
	switch(x->a_type){
	case T_EXTERN:
	case T_COMP:
		if (found_in_list(x->a_name,list,nb_in_list) != -1)
			fprintf(f_out,"struct %s",x->a_name);
		else	fprintf(f_out,"%s", x->a_name);
		break;
	case T_SET:
	case T_SEQUENCE:
		generate_struct_head(f_out,element);
		generate_list(f_out,element,depth+1,
		tabulation,list,nb_in_list,0);
		break;
	case T_CHOICE:
		generate_struct_head(f_out,element);
		fprintf(f_out," {\n");
		do_indent(f_out, depth+1, tabulation);
		fprintf(f_out,"int x;\n");
		do_indent(f_out, depth+1, tabulation);
		fprintf(f_out,"union");
		generate_list(f_out,element,depth+2,
		tabulation,list,nb_in_list,1);
		fprintf(f_out," v;\n");
		do_indent(f_out, depth, tabulation);
		fprintf(f_out,"}");
		break;
	case T_SEQUENCE_OF:
	case T_SET_OF:
		generate_struct_head(f_out,element);
		fprintf(f_out," {\n");
		do_indent(f_out, depth+1, tabulation);
		fprintf(f_out,"int n;\n");
		do_indent(f_out, depth+1, tabulation);
		generate_structure(f_out,x->a_first_son,
		depth+1, tabulation,list,nb_in_list,1);
		do_indent(f_out, depth, tabulation);
		fprintf(f_out,"}");
		break;
	case T_NULL:
		fputs("int",f_out); break;  /*AF void\int */
	case T_IGNORE:  /*AF ergaenzt */
		fputs("int",f_out); break;
	case T_INTEGER:
	case T_BOOLEAN:
	case T_ENUMERATED:
		fputs("int",f_out); break;
	case T_REAL:
		fputs("double",f_out); break;
	case T_OCTETS:
		fputs("asn1_field",f_out); break;
	case T_OID:
		fputs("asn1_field",f_out); break;
	case T_EXTERNAL:
		fputs("asn1_external",f_out); break;
	case T_BITS:
		fputs("asn1_field",f_out); break;
	case T_FLAGS: /*AF FLAGS ergaenzt */
		fputs("int",f_out); break;
	case T_ANYDEF:
		fputs("asn1_opaque",f_out);break;
		/* 14.01.91 H.T. i.A von Huitema */
	case T_ANY:
        case T_COPY:  /*AF COPY ergaenzt*/
		fputs("asn1_field",f_out); break;
	default:
		fputs("PANIC",f_out); break;
	}
	if (need_pointer) fputs(" *",f_out);
	fprintf(f_out," %s;\n",x->a_ext_id);
}

generate_list(f_out,
	element, 
	depth, tabulation, 
	list, nb_in_list, 
	need_pointer)
FILE * f_out;
int element, depth, *list, nb_in_list, need_pointer;
char * tabulation;
{	int i, tot = 0;

	fprintf(f_out," {\n");
	for (i=tree_element[element].a_first_son; i != -1;
	i = tree_element[i].a_brother){
		if (tree_element[i].a_type == T_NULL
		&&  tree_element[i].a_option == O_ALWAYS)
			continue;
		tot++;
		do_indent(f_out, depth, tabulation);
		generate_structure(f_out,i,depth,tabulation,
		list,nb_in_list,need_pointer);
	}
	do_indent(f_out, depth-1, tabulation);
	/* Struct and Unions must have at least one component */
	if (tot == 0){
		fprintf(f_out,"int filler;");
	}
	fprintf(f_out,"}");
}
