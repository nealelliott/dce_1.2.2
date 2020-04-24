/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavrospro.c,v $
 * Revision 1.1.12.1  1996/07/08  18:22:38  arvind
 * 	Count error messages correctly.
 * 	Attempt to detect non-unique tags when OPTIONAL is in use.
 * 	[1996/06/07  17:48 UTC  sommerfeld  /main/sommerfeld_pk_kdc/1]
 *
 * Revision 1.1.10.2  1996/02/18  19:38:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:21  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  15:09:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:12  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:40:31  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:04:57  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:11:19  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:08:54  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  17:12:09  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:55:05  bbelch]
 * 
 * Revision 1.1.4.3  1992/11/30  11:11:54  marrek
 * 	November 1992 code drop
 * 	Replace malloc.h by stdlib.h
 * 	[1992/11/30  11:08:48  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  17:58:59  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  13:03:51  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:22:45  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:34:16  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavrospro.c,v $ $Revision: 1.1.12.1 $ $Date: 1996/07/08 18:22:38 $";
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
#include "mavrospro.h"

int incl_list_nb = 0, def_list_nb = 0;
char ** incl_list, ** def_list;

int current_brother = -1, current_father = -1;
int compindex = 0, compmax = 8;
int last_error = -1;
struct comp * complist;
extern int linenum;

int errcnt = 0, errmax = 20;
extern int debuglevel;
extern int compilation_mode;

char ** exported;
int nb_exported = 0;

extern char * filein;
extern char * package;

char ** defined_proc_list;
int defined_proc_list_size = 0;

parse_input()
{
	complist = (struct comp *) malloc(
	(unsigned)compmax * sizeof(struct comp));
	(void) yyparse();
	verify_tree();
}

setpackage(x)
char * x;
{
	if (package == (char *) 0) package = x;
}


memory_error(){
	(void)fprintf(stderr,"Out of memory\n");
	exit(-1);
}

defined_procedure(proc_name)
char * proc_name;
{
	int i;

	for (i=0; i<defined_proc_list_size; i++)
		if (strcmp(defined_proc_list[i], proc_name) == 0)
			return;
	if (defined_proc_list_size == 0)
		defined_proc_list =(char **)malloc((unsigned)sizeof(char *));
	else	defined_proc_list =(char **)realloc((char *)defined_proc_list,
		(unsigned)(defined_proc_list_size + 1) * sizeof(char *));
	defined_proc_list[defined_proc_list_size++] = proc_name;
}

get_desc(s,i,j)
char * s;
int *i, *j;
{	register k,l;
	for(k=0;k<compindex;k++){
		if (complist[k].c_type != T_TYPE
		&&  complist[k].c_type != T_IMPORT) continue;
		/* Protection against yet not lethal syntax errors */
		if (complist[k].c_name == 0) continue;
		/* Look whether type does match.	*/
		/* We should store that in the tree..	*/
		if (strcmp(complist[k].c_name,s) == 0){
			if (complist[k].c_type == T_IMPORT){
				*i = -1;
				*j = 0;
			}else{
				for (l=complist[k].c_first_son; 
				complist[l].c_type == T_PARAMETER;
				l = complist[l].c_brother){};
				*i = complist[l].c_mode;
				*j = complist[l].c_number;
			}
			return;
		}
	}
	stxerr("Unknown type",s);
	*i = -1; *j = 0;
	return;
}

/**************************\
* 			   *
* Error handling routines  *
* 			   *
\**************************/
stxerr(s1,s2)
char * s1, * s2;
{
	extern int yynerrs ;
#ifdef YY_ERR_MSG     /*AF*/

	yyerror(s1,s2);
	yyerror(0,0);
#else
	if (s2 == 0 || (strlen(s1) + strlen(s2)) > 252)
		yyerror(s1);
	else{	
	        ++ yynerrs;
		(void)(void)fprintf(stderr,"\"%s\", line %d: %s (%s).\n", 
		filein, last_error = linenum, s1, s2);
		last_error = linenum;

	}
#endif
	errcnt = yynerrs ;
	if (errcnt > errmax){
		(void)fprintf(stderr,"%s: too many errors: goodbye!\n",filein);
		exit(-1);
	}
}

#ifndef YY_ERR_MSG
yyerror(s)
char * s;
{
	extern int yynerrs ;

	++ yynerrs ;
	if (last_error != linenum)
		(void)fprintf(stderr,"\"%s\", line %d: %s.\n",
		filein, last_error = linenum, (s)?s:"syntax error");
	return;
}
#endif

warning(s)
char * s;
{
	(void)fprintf(stderr,"\"%s\", line %d: %s (Warning).\n",
	filein, linenum, s);
}

/**************************************************************************\
* 									   *
* Introduce a final component description in a structure.		   *
* 									   *
* The tree structure is reflected by the variables:			   *
* c_first_son:	pointer to the first child,				   *
* c_brother:	pointer to the next brother, or -1 if end of structure.	   *
* c_father:	pointer to the father.					   *
* 									   *
\**************************************************************************/
opencomp(id,n,m,i,t,ut,o,s1,s2,cm)
int n, m, i, t, o, cm,ut;
char * s1, * s2, *id;
{	register struct comp * c;
#ifdef DEBUGTREE
(void)fprintf(stderr,">(%d<%d(%d)) %d:%s\n",
	compindex,current_brother,current_father,t,s1);
#endif
	if (compindex >= compmax){
		compmax = 2 * compmax;
		if ((complist = (struct comp *)	realloc((char *)complist,
		(unsigned)(compmax * sizeof(struct comp)))) == 0)
			memory_error();
	}
	c = complist + compindex; c->c_line_num = linenum;
	c->c_number = n; c->c_mode = m; c->c_implicit = i; 
	c->c_type = t; c->c_optional = o; c->c_u_tag = ut;
	c->c_name = s1; c->c_index = s2; c->c_call_mode = cm; c->c_id = id;
	if (current_father != -1){
		if (complist[current_father].c_first_son == -1){
			complist[current_father].c_first_son = compindex;
		}
	}
	if (current_brother != -1)
		complist[current_brother].c_brother = compindex;
	c->c_first_son = -1;
	c->c_brother = -1;
	c->c_father = current_father;
	if (t != T_PARAMETER){
		current_brother = -1;
		current_father = compindex;
	}else{
		current_brother = compindex;
	}
	return(compindex++);
}

/******************************************\
* 					   *
* Finishes a component, gives it a value   *
* 					   *
\******************************************/
closecomp(id,n,m,i,t,ut,o,s1,s2,cm)
int n, m, i, t,o, cm, ut;
char * s1, * s2, *id;
{	register struct comp * c;


	c = complist + current_father;
	c->c_number = n; c->c_mode = m; c->c_implicit = i; 
	c->c_type = t; c->c_optional = o; c->c_u_tag = ut;
	if (c->c_u_tag != -1 && c->c_mode == -1){
		c->c_mode = 0; c->c_number = c->c_u_tag; c->c_implicit = 1;}
	c->c_name = s1; c->c_index = s2; c->c_call_mode = cm; c->c_id = id;
#ifdef DEBUGTREE
(void)fprintf(stderr,"<(%d) %d:%s (<%d(%d),>%d)\n",current_father,t,s1,c->c_father,c->c_brother,c->c_first_son);
#endif
	current_brother = current_father;
	current_father = c->c_father;
}

/*************************************************\
* 						  *
*  Finishes a component, without giving a value   *
* 						  *
\*************************************************/
cclose()
{	register struct comp * c;


	c = complist + current_father;

#ifdef DEBUGTREE
(void)fprintf(stderr,"<*(%d) %d:%s (<%d(%d),>%d)\n",
	current_father,c->c_type,c->c_name,c->c_father,c->c_brother,c->c_first_son);
#endif
	current_brother = current_father;
	current_father = c->c_father;
 	if (c->c_u_tag != -1 && c->c_mode == -1){
 		c->c_mode = 0; c->c_number = c->c_u_tag; c->c_implicit = 1;}
}
/*************************\
* 			  *
*  declares an argument   *
* 			  *
\*************************/
addarg (m,n,t)
int m,n;
char * t;
{	int i;

	for (i=n;i<compindex && i != -1;i=complist[i].c_brother){
		complist[i].c_call_mode = m;
		complist[i].c_index = t;
	}
}

addparameter(s)
char * s;
{
	return(opencomp((char *)0,0,0,0,T_PARAMETER,-1,0,s,(char *)0,0));
}

/*****************************\
* 			      *
* Export and Import control.  *
* 			      *
\*****************************/
export_all()
{	nb_exported = -1;
}

export(x)
char * x;
{
	if (nb_exported == 0)
		exported = (char **) malloc ((unsigned) 128*sizeof(char *));
	else if ((nb_exported&127) == 0)
		exported = (char **) realloc ((char *) exported,
		(unsigned) (nb_exported + 128)*sizeof(char *));
	exported[nb_exported++] = x;
}

open_module()
{}
/********************************************************************\
* 								     *
* When closing a module, we should take sufficient actions so that   *
* the list of imported modules can be pretty printed. This implies   *
* attaching the module ID to the last item declared in the list.     *
* 								     *
\********************************************************************/
close_module(x)
char * x;
{	char buf[256];
	int i, last;

	(void) strcpy(buf,x);
	for (i=0, last= -1; i<compindex; i++){
		if (complist[i].c_type == T_IMPORT)
			last = i;
	}
	if (last != -1 && complist[last].c_index == 0)
		complist[last].c_index = x;
	else	free(x);
	(void) strcat(buf,".h");
	one_more_include(buf);
}

one_more_include(buf)
char * buf;
{	int i;

	for (i=0; i<incl_list_nb; i++){
		if (strcmp(buf,incl_list[i]) == 0)
			return;
	}
	if (incl_list_nb == 0)
		incl_list = (char**) malloc((unsigned)sizeof(char *));
	else	incl_list = (char**) realloc((char *) incl_list,
		(unsigned)((incl_list_nb+1)*sizeof(char *)));
	incl_list[incl_list_nb] = malloc((unsigned) strlen(buf)+1);
	(void) strcpy(incl_list[incl_list_nb++],buf);
	return;
}


/***************************************************************************\
* 									    *
* Verification procedures						    *
* 									    *
* First, we walk down each type, in order to make sure that all components  *
* are correctly encoded.						    *
* 									    *
* Then, for each type, we compute a ``call mode'': it can be either a SET   *
* or a SEQUENCE generation, or a normal structured type, or a simple type.  *
* Simple types will be computed by means of macros. Those types are simple  *
* which have only one component, which is NOT a simple component defined    *
* later on, and which don't include allocations.			    *
* 									    *
* If the type includes some allocation, the decoding procedure will be	    *
* generated, but the coding procedures will be done by hand.		    *
* 									    *
\***************************************************************************/
verify_tree()
{
	int i_comp,i_first,k, i_exp;

	for (i_comp=0;i_comp<compindex;i_comp++){
		if (complist[i_comp].c_father == -1){
			if (complist[i_comp].c_type != T_TYPE
			&&  complist[i_comp].c_type != T_IMPORT){
				linenum = complist[i_comp].c_line_num;
				stxerr("Unknown module type",(char *)0);
			}
			verify_comp(&complist[i_comp],0,i_comp);
		}
	}

	/*
	 * [1996/06/06  10:51:48  sommerfeld] detect case where
	 * OPTIONAL field followed by something with exact same tag..
	 */
	for (i_comp=0; i_comp < compindex; i_comp++)
	{
	    struct comp *c = &complist[i_comp];
	    struct comp *d;
	    int next;
	    
	    if (c->c_optional == 1)
	    {
		for (next = c->c_brother;
		     next != -1;
		     next = d->c_brother)
		{
		    d = &complist[next];
		    if ((c->c_mode == d->c_mode) &&
			(c->c_number == d->c_number) &&
			(asnstruct(c) == asnstruct(d)))
		    {
			char errbuf[1024];
			linenum = c->c_line_num;
			sprintf(errbuf, "Type of `%s' indistinguishable from successor `%s'",
				c->c_id?c->c_id:c->c_name,
				d->c_id?d->c_id:d->c_name);
			
			stxerr("Ambiguous OPTIONAL", errbuf);
		    }
		    if (d->c_optional == 0)
			break;
		}
	    }
	}

#ifdef DEBUGTREE
(void)fprintf(stderr,"All types verified!\n");
#endif
	for (i_comp=compindex-1;i_comp>=0;i_comp--){
		if (complist[i_comp].c_type != T_TYPE) continue;
		for (i_first= complist[i_comp].c_first_son;
		i_first != -1;i_first = complist[i_first].c_brother)
			if (complist[i_first].c_type != T_PARAMETER) break;
		switch (complist[i_first].c_type){
		case T_SET:
			complist[i_comp].c_call_mode = SET_MODE;
			break;
		case T_SEQUENCE:
			complist[i_comp].c_call_mode = SEQUENCE_MODE;
			break;
		case T_ANYDEF:
		case T_CHOICE:
		case T_SET_OF:
		case T_SEQUENCE_OF:
		case T_COPY:
		case T_COPY_ALLOC:
			complist[i_comp].c_call_mode = COMPLEX_MODE;
			break;
		default:
			complist[i_comp].c_call_mode = COMPLEX_MODE;
			if (nb_exported == -1
			||  complist[i_first].c_implicit == 0 
			||  complist[i_first].c_mode != -1){
				break;
			}
			if (nb_exported){
				for (i_exp =0; i_exp < nb_exported; i_exp++)
					if (strcmp(complist[i_comp].c_name,
					exported[i_exp]) == 0) break;
				if (i_exp < nb_exported) break;
			}
			complist[i_comp].c_call_mode = DEFINE_MODE;
			for(k=complist[i_first].c_first_son; 
			k != -1; k = complist[k].c_brother){
				if (complist[k].c_type == T_ALLOC){
					complist[i_comp].c_call_mode = 
					HALF_MODE;
					continue;
				}
				if (complist[k].c_type == T_INIT){
					complist[i_comp].c_call_mode = 
					COMPLEX_MODE;
					break;
				}
			}
			if (complist[i_comp].c_call_mode != COMPLEX_MODE 
			&&  complist[i_comp].c_type == T_EXTERN){
			for(k=i_comp+1;k<compindex;k++){
				if (complist[k].c_type != T_TYPE &&
				strcmp(complist[k].c_name,
				complist[i_first].c_name)!=0)
					continue;
				if (complist[k].c_call_mode == DEFINE_MODE
				|| complist[k].c_call_mode == HALF_MODE)
					complist[i_comp].c_call_mode = 
					COMPLEX_MODE;
				else	k = compindex;
				break;
			}}
			break;
		}
#ifdef DEBUGTREE
(void)fprintf(stderr,"%s: mode %d\n",
complist[i_comp].c_name,complist[i_comp].c_call_mode);
#endif

	}
}

/********************************\
* 				 *
* Verify a particular component	 *
* 				 *
\********************************/
verify_comp(c,ft,up)
struct comp * c;
int ft,up;
{	int i;

	linenum = c->c_line_num;
	if (c->c_mode == -1){
	c->c_implicit = 0;
	switch (c->c_type) {
	case T_EXTERN: 
		get_desc(c->c_name,&c->c_mode,&c->c_number); 
		if (c->c_mode != -1) c->c_implicit = 1;
		break;
	case T_IGNORE:
	case T_COPY:
	case T_COPY_ALLOC:
		stxerr("IGNORE and COPY must be tagged",c->c_name);
		break;
	case T_CHOICE: break;
	case T_COMP:
		if (ft != T_SET && ft != T_SEQUENCE)
		stxerr(
		"COMPONENTS are only allowed within SET or SEQUENCE",
		(char *)0);
		break;
	default: 
		if (ft == T_SET || ft == T_CHOICE)
		stxerr("CHOICE and SET members should be tagged",
		(c->c_id)? c->c_id:c->c_name);
		break;
	}}
	if( c->c_type == T_DEFAULT && ( c->c_father == -1
	|| ((i=complist[c->c_father].c_father)==-1)||
	(complist[i].c_type != T_SET && complist[i].c_type != T_SEQUENCE))){
		i = c->c_father;
		stxerr("DEFAULT is only allowed within SET or SEQUENCE",
		(i== -1)?(char *)0:
		((complist[i].c_id)?complist[i].c_id:complist[i].c_name));
	}
	if (c->c_implicit == 1 && c->c_type == T_CHOICE){
		c->c_implicit = 0;
		warning("CHOICE cannot be IMPLICIT");}
	if (c->c_implicit == 1 && 
	(c->c_type == T_ANY || c->c_type == T_ANYDEF)){
		c->c_implicit = 0;
		warning("ANY cannot be IMPLICIT");}
	if (c->c_implicit == 0 && c->c_type == T_IGNORE)
		c->c_implicit = 1;
	if (c->c_type != 0 && c->c_type != T_PARAMETER){
		for (i=c->c_first_son; i != -1; i = complist[i].c_brother)
			verify_comp(&complist[i], c->c_type, up);
		if (c->c_type == T_COMP
		&&  complist[c->c_father].c_type != T_SET
		&&  complist[c->c_father].c_type != T_SEQUENCE) 
			stxerr(
			"COMPONENTS OF must be within SET or SEQUENCE",
			(char *)0);
		if (c->c_type == T_CHOICE || c->c_type == T_SET)
		for (i=c->c_first_son; complist[i].c_brother != -1; 
		i = complist[i].c_brother){
			if (complist[i].c_type == T_INIT
		    	||  complist[i].c_type == T_FINAL
			||  complist[i].c_type == T_DEFAULT
			||  complist[i].c_type == T_ALLOC
			||  complist[i].c_type == T_ARGUMENT) continue;
			if (complist[i].c_type == T_COMP){
				if (complist[c->c_father].c_type == T_SET)
				warning("Unicity of COMPONENTS not verified");
				continue;
			}
			if (complist[i].c_type == T_EXTERN
			&&  complist[i].c_mode == -1){
				warning("Unicity of tags not verified");
				continue;
			}
			if (complist[i].c_type == T_CHOICE
			&&  complist[i].c_mode == -1)
				match_in_tree(complist[i].c_first_son,
				complist[i].c_brother);
			else	match_in_list(complist[i].c_mode,
				complist[i].c_number,complist[i].c_brother);
		}
	}
	return;
}

match_in_tree(i0,i1)
{	int i;

	for (i=i0; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
	    	||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC
		||  complist[i].c_type == T_ARGUMENT) continue;
		if (complist[i].c_type == T_EXTERN
		&&  complist[i].c_mode == -1)
			continue;
		if (complist[i].c_type == T_CHOICE
		&&  complist[i].c_mode == -1)
			match_in_tree(complist[i].c_first_son,i1);
		else	match_in_list(complist[i].c_mode,complist[i].c_number,i1);
	}
}

match_in_list(m,n,j0)
{	int j;

	for (j=j0; j != -1; j = complist[j].c_brother){
		if (complist[j].c_type == T_INIT ||
	    	    complist[j].c_type == T_FINAL || 
	    	    complist[j].c_type == T_DEFAULT || 
	   	    complist[j].c_type == T_ALLOC ||
		    complist[j].c_type == T_ARGUMENT ||
		    complist[j].c_type == T_COMP) continue;
		if (complist[j].c_type == T_EXTERN
		&&  complist[j].c_mode == -1) continue;
		if (complist[j].c_type == T_CHOICE
		&&  complist[j].c_mode == -1){
			match_in_list(m,n,complist[j].c_first_son);
			continue;
		}
		if (complist[j].c_mode == m
		&&  complist[j].c_number == n){
			linenum = complist[j].c_line_num;
			stxerr(
			"Members of SET or CHOICE must have different tags",
			complist[j].c_name);
		}
	}
}

/**********************************************\
* 					       *
* This procedure is used from within ``lex''.  *
* 					       *
\**********************************************/

sanitize_quoted_string(result, original)
char * result, *original;
{	register char *x, *y;

	for (x=result,y = original+1;*y;){
		if (*y == '"'){
			if (*(y+1) == '"'){
				y+=2; 
				*x++ = '"'; 
			}else	break;
		}else	*x++ = *y++;
	}
	*x = 0;
}
