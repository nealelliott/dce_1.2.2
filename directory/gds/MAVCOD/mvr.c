/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mvr.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:08  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:08:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:51  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:37:37  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:02:27  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:56:38  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:05:53  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:11:08  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:54:12  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:21:11  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:32:15  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mvr.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:13 $";
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

/***************************************\
* 				        *
* Generation of a "package.mvr" file.   *
* 				        *
\***************************************/
#include <stdio.h>
#include "mavcod.h"

extern char * mode_id[];
extern char * implicit_id[];

extern char * package_name ;
extern int tag_mode;

extern char ** export_list;
extern unsigned int nb_exported;

extern char ** import_list;
extern unsigned int nb_imported;

extern int * import_limit;
extern char ** import_module;
extern unsigned int nb_import_module;

extern char * build_element_name () ;/*AF*/

/*AF nb_of_flags eingefuehrt
*/
int nb_of_flags;

mvr_package(f_out,indent)
FILE * f_out;
char * indent;
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
	}
	if (nb_imported){
		fputs("IMPORTS\n",f_out);
		for (i=0, m=0;;){
			do_indent(f_out,1,indent);
			fprintf(f_out,"%s(VAR x:%s)",
			import_list[i],import_list[i]);
			i++;
			if (i == import_limit[m]){
				fprintf(f_out," FROM %s",import_module[m++]);
				if (i == nb_imported){
					fputs(";\n",f_out);
					break;
				}else	putc('\n',f_out);
			}else	fputs(",\n",f_out);

		}
	}
	if (nb_exported || nb_imported)
		putc('\n',f_out);

	mvr_type_list(f_out,indent);
	fputs("END\n",f_out);
}


mvr_type_list(f_out,indent)
FILE * f_out;
char * indent;
{	int i;

	for( i = tree_element[0].a_first_son; i != -1;
	i = tree_element[i].a_brother){
		mvr_element(&tree_element[i],0,indent,f_out);
		fputs("\n\n",f_out);
#ifdef DEBUG
		fflush(f_out);
#endif
	}
}

mvr_element(x,depth,indent,f_out)
struct asn1_tree * x;
int depth;
char * indent;
FILE * f_out;
{
	switch (x->a_type){
	case T_TYPE:
		fprintf(f_out, "%s(VAR x: %s) ::= ", x->a_name, x->a_name);
		if (x->a_first_son == -1)
			fputs("{}",f_out);
		else 	mvr_element(&tree_element[x->a_first_son],
			depth,indent,f_out);
		break;
	case T_COMP:
		fprintf(f_out,"COMPONENTS OF %s", x->a_name);
		mvr_single_argument(x,f_out);
		break;
	default:
		if (x->a_id) fprintf(f_out,"%s ",x->a_id);
		if (x->a_mode != C_UNSPEC){
			fprintf(f_out,"[%s%d] ",
			mode_id[x->a_mode], x->a_num);
		}
		if (x->a_implicit != F_UNDEF)
			fputs(implicit_id[x->a_implicit],f_out);

		mvr_arguments(x, depth, indent, f_out);
	}
}

mvr_arguments(x, depth, indent, f_out)
struct asn1_tree * x;
int depth;
char * indent;
FILE * f_out;
{	int brother, i;
	char * new_name;
	switch(x->a_type){
	case T_EXTERN:
		fputs(x->a_name,f_out);
		mvr_single_argument(x,f_out);
		break;
	case T_COMP:
		fprintf(f_out,"COMPONENTS OF %s", x->a_name);
		mvr_single_argument(x,f_out);
		break;
	case T_SET:
	case T_SEQUENCE:
		fprintf(f_out,"%s {",x->a_name);
		for(brother = x->a_first_son;;) {
			putc('\n',f_out);
			do_indent(f_out, depth+1, indent);
			mvr_element(&tree_element[brother],
			depth + 1, indent, f_out);
/*
			if (tree_element[brother].a_option == O_OPTION
			&& tree_element[brother].a_type != T_NULL)
AF stattdessen...(Sonderrolle v. ANYDEF) */
			if (tree_element[brother].a_option == O_OPTION
			&&  (   tree_element[brother].a_type != T_NULL
                             && tree_element[brother].a_type != T_ANYDEF
                            )
                           )
			mvr_allocate(brother,depth+1,indent,f_out,A_OPTION);
			if (tree_element[brother].a_option != O_ALWAYS){
			putc('\n',f_out);
			do_indent(f_out, depth+1, indent);
			if (tree_element[brother].a_type == T_NULL){
				fputs ("OPTIONAL", f_out);
			}else{
			fputs("DEFAULT (",f_out);
			mvr_argument(&tree_element[brother],f_out,A_OPTION);
			fprintf(f_out," = %s)",
			(tree_element[brother].a_default)?
			tree_element[brother].a_default:"0");
			}
			}
			brother = tree_element[brother].a_brother;
			if (brother != -1)
				putc(',',f_out);
			else	break;
		}
		fprintf(f_out,"}");
		return;
	case T_CHOICE:
		fputs("CHOICE ",f_out);
		mvr_argument(x,f_out,A_CHOICE_INDEX);
		fputs(" {",f_out);
		for(brother = x->a_first_son,i=1;;i++){
			putc('\n',f_out);
			do_indent(f_out, depth+1, indent);
			fprintf(f_out,"%d: ",i);
			mvr_element(&tree_element[brother],
			depth + 1, indent, f_out);
/*
			if (tree_element[brother].a_type != T_NULL)
AF stattdessen...(Sonderrolle v. ANYDEF) */
			if (   tree_element[brother].a_type != T_NULL
                            && tree_element[brother].a_type != T_ANYDEF
                           )
			mvr_allocate(brother,depth+1,indent,f_out,A_CHOICE);
			brother = tree_element[brother].a_brother;
			if (brother != -1)
				putc(',',f_out);
			else	break;
		}
		putc('}',f_out);
		return;
	case T_SEQUENCE_OF:
	case T_SET_OF:
		fprintf(f_out,"%s \n",x->a_name);
		do_indent(f_out, depth+1, indent);
		mvr_element(&tree_element[x->a_first_son],
		depth + 1, indent, f_out);
		putc('\n',f_out);
		do_indent(f_out, depth, indent);
		fprintf(f_out,"INDEXED BY I_%d UP TO ",depth+1);
		(void)mvr_argument(x,f_out,A_ARRAY_INDEX);
		do_indent(f_out, depth, indent);
		mvr_allocate(x->a_first_son,depth,indent,f_out,A_ARRAY);
		return;
	case T_NULL:
		fputs("NULL",f_out);
		break;
	case T_IGNORE:  /*AF*/
		fputs("IGNORE",f_out);
		break;
	case T_INTEGER:
		fputs("INTEGER",f_out);
		mvr_int_values(x,depth,indent,f_out);
		mvr_single_argument(x,f_out);
		break;
	case T_ENUMERATED:
		fputs("ENUMERATED",f_out);
		mvr_int_values(x,depth,indent,f_out);
		mvr_single_argument(x,f_out);
		break;
	case T_EXTERNAL:
		fputs(x->a_name,f_out);
		mvr_double_argument(x,f_out,"stx_id","data");
		break;
		/* H.T. i.A. von Huitema */
	case T_BOOLEAN:
	case T_REAL:
		fputs(x->a_name,f_out);
		mvr_single_argument(x,f_out);
		break;
	case T_ANYDEF:
		fputs("ANY ",f_out);
		mvr_argument(x,f_out,A_STD);
		putc('\n',f_out);
		do_indent(f_out, depth, indent);
		fputs("DEFINED BY ",f_out);
		fputs(new_name = build_element_name(x - tree_element),f_out);
		free(new_name);
		look_for_name_up_tree(x - tree_element,x->a_name,f_out);
		break;
	case T_ANY:
        case T_COPY:  /*AF COPY ergaenzt*/
	case T_OCTETS:
	case T_OID:
		fputs(x->a_name,f_out);
		mvr_double_argument(x,f_out,"v","l");
		break;
	case T_BITS:
		fputs(x->a_name,f_out);
		mvr_int_values(x,depth,indent,f_out);
		mvr_double_argument(x,f_out,"v","l");
		break;
	case T_FLAGS: /*AF ergaenzt */
		fputs(x->a_name,f_out);
		mvr_int_values(x,depth,indent,f_out);
                /*AF** */
                {
                  char xxx_tmp[5];
                  /*nb_of_flags*/ 
                  sprintf(xxx_tmp,"%d",nb_of_flags+1);
                  mvrFLarg(x,f_out,xxx_tmp);
                }
		break;
	default:
		fprintf(f_out,"PANIC"); break;
	}
}

mvr_int_values(x,depth,indent,f_out)
struct asn1_tree * x;
int depth;
char * indent;
FILE * f_out;
{	int brother;

	if (x->a_first_son == -1)
		return;
	putc('{',f_out);
/*iAF
	for(brother = x->a_first_son;;) {
*/
	for(brother = x->a_first_son, nb_of_flags=0;;++nb_of_flags) {
		putc('\n',f_out);
		do_indent(f_out, depth+1, indent);
		fprintf(f_out,"%s(%s)",
		tree_element[brother].a_id, tree_element[brother].a_name);
		brother = tree_element[brother].a_brother;
		if (brother != -1)
			putc(',',f_out);
		else	break;
	}
	putc('}',f_out);
}

/*****************************************\
* 					  *
* mvr_argument:				  *
* 					  *
* Print the argument name of a variable.  *
* 					  *
\*****************************************/
mvr_argument(x,f_out,a_mode)
struct asn1_tree * x;
FILE * f_out;
int a_mode;
{
	int i = x->a_father, depth;

	if (tree_element[i].a_type != T_TYPE){
		depth = mvr_argument(&tree_element[i],f_out,A_STD);
		if (tree_element[i].a_type == T_CHOICE)
			fputs(".v",f_out);
		fprintf(f_out,".%s",x->a_ext_id);
		switch (a_mode){
		case A_ARRAY_INDEX:
			fputs(".n",f_out); break;
		case A_ARRAY:
		case A_CHOICE:
		case A_OPTION:
			break;
		case A_CHOICE_INDEX:
			fputs(".x",f_out); break;
		default:
			switch(tree_element[i].a_type){
			case T_CHOICE:
				fputs("[0]",f_out);
				break;
			case T_SET_OF:
			case T_SEQUENCE_OF:
				fprintf(f_out,"[I_%d]",depth);
				break;
			default:
				if (   x->a_option == O_OPTION
                                    && x->a_type != T_ANYDEF   /*AF*/
                                   )
					fputs("[0]",f_out);
				break;
			}
			switch(x->a_type){
			case T_CHOICE:
				/*fputs(".v",f_out);  AF TEST*/
				break;
			default:
				break;
			}
		}
	}else{
		fputs("x",f_out);
		switch(a_mode){
		case A_ARRAY_INDEX:
			fputs(".n",f_out); break;
		case A_CHOICE_INDEX:
			fputs(".x",f_out); break;
		default:
			break;
		}
		depth = 0;
	}
	return(depth+1);
}

mvr_single_argument(x,f_out)
struct asn1_tree * x;
FILE * f_out;
{
	putc('(',f_out);
	mvr_argument(x,f_out,A_STD);
	putc(')',f_out);
}

mvrFLarg(x,f_out,s1)
struct asn1_tree * x;
FILE * f_out;
char * s1;
{
	putc('(',f_out);
	mvr_argument(x,f_out,A_STD);
	fprintf(f_out,",%s",s1);
	putc(')',f_out);
}


mvr_double_argument(x,f_out,s1,s2)
struct asn1_tree * x;
FILE * f_out;
char * s1, * s2;
{
	putc('(',f_out);
	mvr_argument(x,f_out,A_STD);
	fprintf(f_out,".%s,",s1);
	mvr_argument(x,f_out,A_STD);
	fprintf(f_out,".%s)",s2);
}



/***************************************\
* 				        *
* Generation of the allocation request  *
* 				        *
\***************************************/
mvr_allocate(el,depth, indent, f_out, a_mode)
int el;
int depth;
char * indent;
FILE * f_out;
int a_mode;
{
	putc('\n',f_out);
	do_indent(f_out, depth, indent);
	fputs("ALLOCATE (",f_out);
	mvr_argument(&tree_element[el],f_out,a_mode);
	putc(':',f_out);
	switch (tree_element[el].a_type){
	case T_NULL:
		fputs("int",f_out); break; /*AF void\int */
	case T_IGNORE:break; /*AF*/
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
		/* H.T. i.A. von Huitema */
	case T_ANY:
        case T_COPY:  /*AF COPY ergaenzt*/
		fputs("asn1_field",f_out); 
		/* Removed by C. Huitema, 11 feb 1991
		** putc(';',f_out);
		** putc('\n',f_out);
		** do_indent(f_out, depth, indent);
		** mvr_argument(&tree_element[el],f_out,a_mode);
		** fputs("^.v",f_out);
		** putc(':',f_out);
		** fputs("UC",f_out);
		*/
		break;
	default:
		fputs(tree_element[el].a_name,f_out); break;
	}
	putc(')',f_out);
}

/**********************************************************************\
* 								       *
* look_for_name_up_tree:					       *
* 								       *
* In constructs of the form "ANY DEFINED BY name", the "name" should   *
* be matched to the identification of a variable in the same block --  *
* or in the same scope.						       *
* 								       *
\**********************************************************************/
look_for_name_up_tree(x,name,f_out)
int x;
char * name;
FILE * f_out;
{
	while (tree_element[x].a_father > 0){
		if (look_for_name_at_level(
		tree_element[x].a_father,name,f_out,x) == 0)
			return;
		x = tree_element[x].a_father;
	}
	fprintf(stderr,"Error: %s unknown in the scope of %s.\n",
	name, tree_element[x].a_name);
	fprintf(f_out,"(%s)",name);
	return;
}

look_for_name_at_level(father,name,f_out,tried)
int father;
char * name;
FILE * f_out;
int tried;
{	int i;

	/* Look first on the same level */
	for (i = tree_element[father].a_first_son;
	i != -1 && i != tried; i = tree_element[i].a_brother){
		if (tree_element[i].a_id != 0
		&&  strcmp(name,tree_element[i].a_id) == 0){
			mvr_single_argument(&tree_element[i],f_out);
			return(0);
		}
	}
	/* Look next to the children */
	for (i = tree_element[father].a_first_son;
	i != -1 && i != tried; i = tree_element[i].a_brother){
		if (look_for_name_at_level(i,name,f_out,-1) == 0)
			return(0);
	}
	return(-1);
}
