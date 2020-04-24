/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavroscpy.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:14  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:09:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:04  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:39:21  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:03:43  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:04:17  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:07:20  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:11:36  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:54:42  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:56:51  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  13:02:44  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:22:00  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:33:42  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavroscpy.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:19 $";
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

#ifndef lint
#define PUTC	putc
#else
#define PUTC (void)fputc
#endif

#include <stdio.h>
#include "mavrospro.h"

extern struct tstrd 	* strd;
extern struct comp 	* complist;

extern int 		procindex,
	 		compindex,
			linenum,
	 		debuglevel,
	 		compilation_mode,
	 		incl_list_nb, 
			srce_list_nb;
extern char 		* filein,
	 		* package,
	 		** incl_list, 
			** srce_list;

extern int		storeindex,
			current_list_head,
			current_call_mode;
extern char		** storelist;
char *shortname();

/**********************\
* 		       *
* Browsing the tree !  *
* 		       *
\**********************/
genercpy() 
{       int i, l0=0;

        for (i=0; i < compindex; i++)
 	{       switch (complist[i].c_type){
                case T_TYPE:
                        if (complist[i].c_optional == 0 )
				if ((l0=cpy_onetype(&complist[i],l0))==-1)
					return(-1);
                        break;
                default:
                        break;
		}
	}
	return(0);
}

/****************************************************************************\
* 									     *
* Generation of the copying routines					     *
* 									     *
* The big problem here is the necessity to manipulate twice the same number  *
* of arguments, which will be passed once in ``coding'' mode and the other   *
* time in ``decoding'' fashion. Basically, a copying routine is the	     *
* concatenation of ``encode'' and ``decode''.				     *
* 									     *
\****************************************************************************/
cpy_onetype(c, l0)
struct comp	*c;
int l0;
{	int	first_comp, setnb = 0;

        if (c->c_call_mode == DEFINE_MODE) 
		return(l0);
	if (is_declared_procedure(c->c_name, C_CPY))
		return(l0);
#ifdef DEBUG
        if (debuglevel > 7) (void)fprintf(stderr,"Copying %s\n",c->c_name);
#endif
	storeindex = 0; 
	setnb = 0;
        (void) get_depth(c, 0, &setnb);
        current_list_head = c->c_first_son;
        for (first_comp= c->c_first_son; first_comp != -1; 
	     first_comp = complist[first_comp].c_brother)
                if (complist[first_comp].c_type != T_PARAMETER) break;

	/* Opening a file for the copying routines */
/*AF CPY-routinen nicht in ENCODE-Mod., wenn -u fehlt 
	if ((l0 = open_module_file(c->c_name,ENCODE,l0)) < 0)
*/
	if ((l0 = open_module_file(c->c_name,C_CPY,l0)) < 0)
		return(-1);

        current_call_mode = 1;
	l0 = decl_proc_head(c, l0, C_CPY);
	l0 += decl_var_cpy(storeindex,storelist);
	l0 = cpy_comp(&complist[first_comp], l0);
	(void)printf("}\n"); l0++;
	return(l0);
}

/*****************************************************\
* 						      *
* Declaration of the parameters of copying routines.  *
* 						      *
\*****************************************************/

decl_var_cpy(stindex,stlist)
int stindex;
char ** stlist;
{	int i, l0 = 0;

	(void)printf("{\n"); l0++;
	if (stindex){
		(void)printf("int");
		for (i=0; i<stindex; i++) 
			(void)printf("%c%s",(i==0)?' ':',',stlist[i]);
		(void)printf(";\n");l0++;
	}
	return(l0);
}

/****************************************************************************\
* 									     *
* The main switch for get : ``_cpy.c''				     	     *
* 									     *
\****************************************************************************/

cpy_comp(c, l0)
struct comp 	* c;
int 		l0;
{	int i;

	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		if (complist[i].c_type == T_ALLOC)
			l0 = cpy_alloc(&complist[i],c->c_type,l0);
		if (complist[i].c_type == T_INIT)
			l0 = dcinit(&complist[i],l0);
	}

	switch(c->c_type){
	case T_EXTERN:
		l0 = cdextern(c,-1,0,l0,C_CPY);
		break;
	case T_ANYDEF:
		l0 = cd_anydef(c,l0,C_CPY); break;
	case T_SEQUENCE:
	case T_SET:
		l0 = seq_cpy(c,l0); break;
	case T_CHOICE:
		l0 = choice_cpy(c,l0); break;
	case T_IGNORE:
		break;
	case T_SET_OF:
	case T_SEQUENCE_OF:
		l0 = array_cpy(c,l0); break;
	case T_OCTETS_ALLOC:
	case T_OCTETS:
	case T_ANY:
	case T_ANY_ALLOC:
	case T_COPY:
	case T_COPY_ALLOC:
	case T_OID:
	case T_OID_ALLOC:
	case T_CHARS:
	case T_NULL:
	case T_BITS:
	case T_BITS_ALLOC:
	case T_FLAGS:
	case T_INTEGER:
	case T_BOOLEAN:
	case T_REAL:                
                l0 = cdbasic(c,0,0,l0,C_CPY,0);
		break;
	default:
		(void)fprintf(stderr,
		"Unexpected component type: %d.\n",c->c_type);
		break;
	}
	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		if (complist[i].c_type == T_FINAL)
			l0 = dcinit(&complist[i],l0);
	}
	return(l0);
}

/*****************************************************************\
* 								  *
* Allocation of memory during the copying:			  *
* 								  *
* The allocation of memory for fields and strings is done by the  *
* dewcoding routines themselves!!!				  *
* 								  *
\*****************************************************************/

cpy_alloc(c,t,l0)
struct comp * c; int t,l0;
{	int i;
	struct comp * father;

	switch (t){
	case T_BITS:
	case T_OCTETS:
	case T_OID:
	case T_ANY:
	case T_COPY:
	case T_BITS_ALLOC:
	case T_OCTETS_ALLOC:
	case T_OID_ALLOC:
	case T_ANY_ALLOC:
	case T_COPY_ALLOC:
	case T_CHARS:
		return(l0);
	default:
		break;
	}
	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		l0 = argprint(&complist[i],0,l0);
		(void)printf("= (%s *) asn1_malloc ((unsigned)(",complist[i].c_index);
		switch (t){
		case T_SEQUENCE_OF:
		case T_SET_OF:
			(void)printf("sizeof(%s)*", complist[i].c_index);
			father = &complist[c->c_father];
			l0 = cpy_argprint(&complist[
			complist[father->c_first_son].c_brother],0,l0);
			(void)printf("));\n");
			break;
		default:
			(void)printf("sizeof(%s)));\n",complist[i].c_index); break;
		}
		l0++;
	}
	return(l0);
}

/***************************************************************************\
* 									    *
* SEQUENCE and SETs							    *
* 									    *
* The copying of sequences and sets is equal to that of all the components  *
* which are actually present, plus indeed the copying of the selection	    *
* fields for the missing components.					    *
* 									    *
* There is no specific component handling routine.			    *
* 									    *
\***************************************************************************/

seq_cpy(c,l0)
struct comp * c;
int l0;
{	int i,j,k,l;

	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP){
			l0 = cdextern(&complist[i],-1,0,l0,C_CPY);
			continue;
		}
		for (j = complist[i].c_first_son; j != -1; 
		j = complist[j].c_brother)
			if (complist[j].c_type == T_DEFAULT) break;
		if (j != -1){
			(void)printf("if(");
			for (k=j; k!=-1; k=complist[k].c_brother){
				l = complist[j].c_first_son;
				l0 = cpy_argprint(
				&complist[complist[l].c_first_son],0,l0);
				(void)printf(" != ");
				l0 = cpy_argprint(
				&complist[complist[complist[l].c_first_son].c_brother],0,l0);
				if (complist[k].c_brother != -1)
					(void)printf(" ||\n");
				l0++;
			}
			(void)printf("){\n");
		}
		l0 = cpy_comp(&complist[i],l0);
		if(j != -1){
			(void)printf("}else{\n"); l0++;
			for (k=j; k!=-1; k=complist[k].c_brother){
				l = complist[j].c_first_son;
				l0 = argprint
				(&complist[complist[l].c_first_son],0,l0);
				(void)printf(" = ");
				l0 = cpy_argprint(&complist[complist[
				complist[l].c_first_son].c_brother],0,l0);
				(void)printf(";\n");
				l0++;
			}
			(void)printf("}\n"); l0++;
		}
	}
	return(l0);
}

/*********************************************************************\
* 								      *
* SEQUENCE OF, SET OF						      *
* 								      *
* This components are encoded as a sequence of values, without tags.  *
* 								      *
\*********************************************************************/
int array_cpy(c, l0)
struct comp 	* c;
int 		l0;
{	struct comp     * nbr_comp;

	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	(void)printf("for (%s=0; %s <", c->c_name, c->c_name);
	l0 = cpy_argprint(nbr_comp = 
	&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf("; %s++){\n", c->c_name);
	l0 = cpy_comp(&complist[c->c_first_son],l0);
	(void)printf("}\n"); 
	l0++;
	l0 = argprint(nbr_comp,0,l0);
	(void)printf(" = ");
	l0 = cpy_argprint(nbr_comp,0,l0);
	(void)printf(";\n"); l0++;
	return(l0);	
}

/******************************************\
* 					   *
* CHOICES				   *
* 					   *
* Are encoded as the selected component.   *
* 					   *
\******************************************/
int choice_cpy(c, l0)
struct comp 	* c;
int 		l0;
{	int 	i, openchoice = 1;

	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	(void)printf("switch (CAST_FOR_SWITCH(");
	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf(" = ");
	l0 = cpy_argprint(&complist[c->c_first_son],0,l0);
	(void)printf(")){\n");
	l0++;
	for (i=complist[c->c_first_son].c_brother; 
	i != -1; i = complist[i].c_brother)
	{	if(complist[i].c_type == T_INIT
		|| complist[i].c_type == T_FINAL
		|| complist[i].c_type == T_DEFAULT
		|| complist[i].c_type == T_ALLOC) break;
		if (l0 != complist[i].c_line_num)
			numberline(l0 = complist[i].c_line_num);
		if (complist[i].c_index == 0)
			(void)printf("default: ");
		else	(void)printf("case %s: ",complist[i].c_index);
		l0 = cpy_comp(&complist[i],l0);
		if (complist[i].c_index == 0)
		{	openchoice = 0;
			(void)printf("break;}\n"); 
			l0++; 
			return(l0);
		}
		else
		{	(void)printf("break;\n"); 
			l0++;
		}
	}
	if (openchoice)
	{	(void)printf("default:break;}\n"); 
		l0++;
	}
	if (debuglevel > 4)
	{	(void)printf("/* End of CHOICE */\n"); 
		l0++;
		(void)fflush(stdout);
	}
	return(l0);
}
