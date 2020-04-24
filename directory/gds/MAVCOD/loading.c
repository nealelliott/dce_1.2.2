/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: loading.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:06  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:08:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:44  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:36:35  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:01:42  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:51:17  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:05:06  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:10:53  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:53:55  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:54:30  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  13:01:33  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:20:48  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:22:02  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: loading.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:11 $";
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
#include <stdlib.h>
#include "mavcod.h"
/**********************************\
* 				   *
* Load in memory the ASN.1 syntax  *
* 				   *
\**********************************/
struct asn1_tree * tree_element = 0;

int tree_element_nb = -1;
int current_father = -1;

extern char * file_name;
extern char * header_name;
extern char * mvr_name;
extern int linenum;

struct tags tags_unspec = { C_UNSPEC, 0, F_UNDEF};
struct options option_always = { O_ALWAYS, (char *)0};
struct element element_error = { T_EXTERN, "ERROR"};

char * package_name ;
int tag_mode;

char ** export_list = 0;
unsigned int nb_exported = 0;

char ** import_list = 0;
unsigned int nb_imported = 0;

int * import_limit = 0;
char ** import_module = 0;
unsigned int nb_import_module = 0;

module_start(name,tags)
char * name;
int tags;
{
	package_name = name;
	if (strcmp(file_name,STD_INPUT_NAME) == 0){
		file_name = malloc((unsigned)(strlen(name)+6));
		(void)sprintf(file_name,"%s.asn1",name);
	}
	if (header_name == 0){
		header_name = malloc((unsigned)(strlen(name)+7));
		(void)sprintf(header_name,"%s_mvr.h",name);
	}
	if (mvr_name == 0){
		mvr_name = malloc((unsigned)(strlen(name)+5));
		(void)sprintf(mvr_name,"%s.mvr",name);
	}
	tag_mode = tags;
}

export_item(exported)
char * exported;
{
	if (nb_exported == 0)
		export_list=(char**)malloc(ALLOCATION_STEP*sizeof(char*));
	else if (nb_exported%ALLOCATION_STEP == 0)
		export_list=(char**)realloc((char *)export_list,
		(nb_exported+ALLOCATION_STEP)*sizeof(char*));
	if (export_list == 0)
		memory_error();

	export_list[nb_exported++] = exported;
}

import_item(imported)
char * imported;
{
	if (nb_imported == 0)
		import_list=(char**)malloc(ALLOCATION_STEP*sizeof(char*));
	else if (nb_imported%ALLOCATION_STEP == 0)
		import_list=(char**)realloc((char *)import_list,
		(nb_imported+ALLOCATION_STEP)*sizeof(char*));
	if (import_list == 0)
		memory_error();

	import_list[nb_imported++] = imported;
}

close_import_module (module)
char * module;
{
	if (nb_import_module == 0){
		import_module=(char**)malloc(ALLOCATION_STEP*sizeof(char*));
		import_limit =(int*)malloc(ALLOCATION_STEP*sizeof(int));
	}else if (nb_import_module%ALLOCATION_STEP == 0){
		import_module=(char**)realloc((char *)import_module,
		(nb_import_module+ALLOCATION_STEP)*sizeof(char*));
		import_limit=(int *)realloc((char *)import_limit,
		(nb_import_module+ALLOCATION_STEP)*sizeof(int));
	}
	if (import_module == 0
	||  import_limit  == 0) memory_error();

	import_module[nb_import_module] = module;
	import_limit[nb_import_module++] = nb_imported;
}

/*******************************************\
* 					    *
* Declaration of one of the tree elements.  *
* 					    *
\*******************************************/

tree_alloc()
{
	if (tree_element_nb == 0){
		tree_element = (struct asn1_tree *)malloc(
		ALLOCATION_STEP * sizeof(struct asn1_tree));
	}else if ((tree_element_nb % ALLOCATION_STEP)==0){
		tree_element = (struct asn1_tree *)realloc(
		(char *)tree_element,
		(ALLOCATION_STEP+tree_element_nb)* sizeof(struct asn1_tree));
	}
	if (tree_element == 0)
		memory_error();	
}

add_element()
{	struct asn1_tree * x;
	int current_brother;

	tree_element_nb++;
	tree_alloc();
	x = & tree_element[tree_element_nb];

	/* manages the chaining in the tree */

	x->a_father = current_father;
	x->a_brother = -1;
	x->a_first_son = -1;
	x->a_last_son = -1;

	x->a_ext_id = 0;

	if (current_father != -1){
		if(tree_element[current_father].a_first_son == -1)
		tree_element[current_father].a_first_son = tree_element_nb;
		current_brother = tree_element[current_father].a_last_son;
		if (current_brother != -1)
		tree_element[current_brother].a_brother = tree_element_nb;
		tree_element[current_father].a_last_son = tree_element_nb;
	}
}

initialise_tree()
{
	current_father = -1;
	add_element();
	current_father = 0;
	tree_element[0].a_id = (char *)0;
	tree_element[0].a_mode = C_UNSPEC;
	tree_element[0].a_num = 0;
	tree_element[0].a_implicit = F_UNDEF;
	tree_element[0].a_type = T_UNDEF;
	tree_element[0].a_name = "Root";
	tree_element[0].a_option = O_ALWAYS;
	tree_element[0].a_default = (char *)0;
}

declare_element(id, tag, el, op)
char * id;
struct tags tag;
struct element el;
struct options op;
{
	struct asn1_tree * x;
	int current_element;

	current_element = tree_element[current_father].a_last_son;
	x = & tree_element[current_element];

	/* fills in the values */
	x->a_id		= id;
	x->a_mode	= tag.m;
	x->a_num	= tag.n;
	x->a_implicit	= tag.i;
	x->a_type	= el.t;
	x->a_name	= el.st;
	x->a_option	= op.t;
	x->a_default	= op.n;
}

open_stack(name)
char * name;
{
	add_element();
	current_father = tree_element_nb;
	tree_element[current_father].a_name = name;
}

close_stack()
{
	if (current_father == -1)
		stxerr("Stack error",(char *)0);
	else	current_father = tree_element[current_father].a_father;
}

declare_item_value(s,v)
char *s, *v;
{
	struct element e;
	e.t = T_INT_VALUE;
	e.st = v;
	add_element();
	declare_element(s,tags_unspec,e,option_always);
}

close_type(s)
char * s;
{
	struct element e;

	close_stack();

	e.t = T_TYPE;
	e.st = s;

	declare_element((char *)0,tags_unspec,e,option_always);
}

/*********************\
* 		      *
* Error declarations  *
* 		      *
\*********************/

memory_error()
{
	stxerr("Out of memory!!!\n",(char *)0);
	exit(-1);
}

stxerr(s1, s2)
char *s1, *s2;
{
#ifdef YY_ERR_MSG  /*AF*/
	yyerror(s1,s2);
	yyerror(0,0);
#else
	fprintf(stderr,"\"%s\",line %d: %s", file_name, linenum, s1);
	if (s2) fprintf(stderr," (%s)",s2);
	putc('\n',stderr);
#endif
}

#ifndef  YY_ERR_MSG
yyerror(s1)
char * s1;
{
	if (strcmp(s1,"syntax error") == 0) return;
	stxerr(s1,(char *)0);
}
#endif
