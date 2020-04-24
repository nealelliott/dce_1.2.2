/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: analyse.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:04  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:08:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:39  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:35:59  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:01:17  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:48:32  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:04:40  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:10:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:44  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:53:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  11:22:07  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:20:33  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:21:49  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: analyse.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:09 $";
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
* Analyse of the tree structure:					    *
* 									    *
* 1- Look for optional or defaulted components which are not of a simple    *
* type. Those components should be described separately, so that one could  *
* easily generate data types, and also copying and comparison routines.	    *
* 									    *
* 2- Look for the cross references between components. If a component	    *
* cross references another ``by value'', then the corresponding type	    *
* should be generated first in the ``include'' file.			    *
* 									    *
\***************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mavcod.h"

extern int current_father;

char * build_element_name(element)
int element;
{	char * x, * new_name;

	if (element == 0){
		new_name = malloc((unsigned)1024);
		new_name[0] = 0;
		return(new_name);
	}
	new_name = build_element_name(tree_element[element].a_father);
	if ((x = tree_element[element].a_id)==0)
		x = tree_element[element].a_name;
	new_name =
	realloc(new_name,(unsigned)strlen(new_name)+strlen(x)+2);
	if (new_name[0])
		(void) strcat(new_name,"_");
	(void) strcat(new_name,x);
	for (x=new_name; *x; x++)
		if (!isalnum(*x)) *x = '_';
	return(new_name);
}

promote_structure(element)
int element;
{	char * new_name;
	struct asn1_tree *x, *y;
	int i;

	new_name = build_element_name(element);

	current_father = 0;
	open_stack(new_name);
	add_element();

	x = &tree_element[element];
	y = &tree_element[tree_element_nb];

	y->a_first_son = x->a_first_son;
	x->a_first_son = -1;
	y->a_last_son = x->a_last_son;
	x->a_last_son = -1;

	for (i = y->a_first_son; i != -1; i = tree_element[i].a_brother)
		tree_element[i].a_father = tree_element_nb;

	y->a_id = (char *)0;
	y->a_mode = C_UNSPEC;
	y->a_num = 0;
	y->a_implicit = F_UNDEF;

	y->a_type = x->a_type;
	x->a_type = T_EXTERN;
	y->a_name = x->a_name;
	x->a_name = new_name;

	y->a_option = O_ALWAYS;
	y->a_default = (char*)0;

	close_type(new_name);
}

look_for_optional(element, father_needs_pointer)
int element, father_needs_pointer;
{	int i;

	if (father_needs_pointer ||
	tree_element[element].a_option != O_ALWAYS)
	switch (tree_element[element].a_type){
	case T_SET:
	case T_SEQUENCE:
	case T_CHOICE:
	case T_SEQUENCE_OF:
	case T_SET_OF:
		promote_structure(element);
		return;
	default:
		break;
	}

	switch (tree_element[element].a_type){
	case T_CHOICE:
	case T_SEQUENCE_OF:
	case T_SET_OF:
		father_needs_pointer = 1;
		break;
	default:
		father_needs_pointer = 0;
	}

	for (i=tree_element[element].a_first_son;
	i != -1; i = tree_element[i].a_brother)
		look_for_optional(i, father_needs_pointer);
}

/*************************************************\
* 						  *
* Compute a relative ordering of the data types.  *
* 						  *
\*************************************************/

	
sort_data_types(list,nb_in_list)
int * list,nb_in_list;
{	int i, one_more_loop, references, new_head, trial;

	for (trial =0, one_more_loop = 1; 
	trial < nb_in_list && one_more_loop; trial++){
		one_more_loop = 0;
		for (i=1; i<nb_in_list; i++){
			if ((references = 
			found_reference(list[i-1],list+i,nb_in_list-i))!=-1){
				new_head = list[i+references];
				list[i+references] = list[i-1];
				list[i-1] = new_head;
				one_more_loop = 1;
			}
		}
	}
	if (one_more_loop){
		stxerr("Probable loop in definitions",
		tree_element[new_head].a_name);
	}
}

found_reference(el, list, nb_in_list)
int el, * list, nb_in_list;
{	int i;

	for (el = tree_element[el].a_first_son; el != -1;
	el = tree_element[el].a_brother){
		if((tree_element[el].a_type == T_EXTERN
		||  tree_element[el].a_type == T_COMP)
		&& (i = found_in_list(tree_element[el].a_name,
		list, nb_in_list)) != -1
		&& !(data_is_structured(list[i])&&type_need_pointer(el))){
			return(i);
		}
		if ((i = found_reference(el, list, nb_in_list))!= -1)
			return(i);
	}
	return(-1);
}

type_need_pointer(el)
int el;
{
	if (tree_element[el].a_option == O_OPTION)
		return(1);
	switch (tree_element[tree_element[el].a_father].a_type){
	case T_CHOICE:
	case T_SEQUENCE_OF:
	case T_SET_OF:
		return(1);
	default:
		return(0);
	}
}

data_is_structured(el)
int el;
{
	switch(tree_element[tree_element[el].a_first_son].a_type){
	case T_SET:
	case T_SEQUENCE:
	case T_CHOICE:
	case T_SEQUENCE_OF:
	case T_SET_OF:
		return(1);
	default:
		return(0);
	}
}

found_in_list(s, list, nb_in_list)
char * s;
int *list, nb_in_list;
{	int i;

	for (i = 0; i<nb_in_list; i++)
		if (strcmp(s,tree_element[list[i]].a_name)==0)
			return(i);
	return(-1);
}

build_ordered_list(list)
int ** list;
{	int nb_in_list=0, i;

	for (i=tree_element[0].a_first_son; i != -1;
	i = tree_element[i].a_brother){
		if ((nb_in_list&0x7F) == 0)
			*list = (int *)((nb_in_list)?
			realloc((char *)*list,(unsigned)(128+i)*sizeof(int)):
					/*AF*/
			malloc((unsigned)128*sizeof(int)));
		(*list)[nb_in_list++] = i;
	}
	/*AF*/
	if ( *list != (int *) 0 && nb_in_list > 0 )
	  sort_data_types(*list,nb_in_list);
	return(nb_in_list);
}


