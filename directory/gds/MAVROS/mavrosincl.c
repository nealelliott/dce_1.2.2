/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavrosincl.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:16  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:09:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:07  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:39:49  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:04:16  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:07:20  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:08:03  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:11:49  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:54:51  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:22:20  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:33:56  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavrosincl.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:22 $";
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

/****************************************************************************\
* 									     *
* Generation of the "include" file.					     *
* 									     *
* The include file, for an ASN1 module, contains for some "obvious" type     *
* coding procedures MACROS to replace the normal coding and decoding	     *
* routines.								     *
* 									     *
* For the "half-obvious" procedures, which require memory allocation, it     *
* contains the type declaration of the decoding procedures + the MACROS for  *
* the coding procedures.						     *
* 									     *
* For the less obvious procedures, it contains the type declaration of the   *
* coding and decoding procedures, plus also the type declaration of the	     *
* COMPONENTS coding and decoding procedure -- if they are defined.	     *
* 									     *
* The include file also contain value definitions, for INTEGER constants     *
* -- when "enumerated" data types are encountered -- and for NAMED BITS in   *
* the "flags" variables. The declaration of this named bits can vary	     *
* according to the "big endian" or "little endian" nature of the machine,    *
* as the flags are encoded as integer variables.			     *
* 									     *
\****************************************************************************/
#include <stdio.h>
#include "mavrospro.h"

extern struct tstrd * strd;
extern int procindex;
extern int compindex;
extern int linenum;
extern struct comp * complist;

extern char * filein;
extern char * package;
extern int def_list_nb;
extern char ** def_list;
extern int debuglevel;
extern int current_list_head;	/* passed to ``argprint()'' */
extern int current_call_mode;	/* passed to ``argprint()'' */
extern int compilation_mode;

char * extern_title(), * ctime();
#ifndef XOS
/* char * sprintf(); */
#endif
long time();

generincl()
{	int i,j,k;
	long lt;
	char fname[256], buf[256];
	FILE * FF;
	int this_bit, which_byte;

	(void) sprintf(fname,"%s.h",package);
	if (freopen(fname,"w",stdout) == 0){
		perror(fname);
		return(-1);
	}

	lt = time((long *)0);
	(void)printf("/*\n%s.h,\n%s*/\n\n",package,ctime(&lt));

	for (i=0;i<def_list_nb;i++){
		if ((FF=fopen(def_list[i],"r"))==0){
			perror(def_list[i]); 
			return(-1);}
#ifdef DEBUG
if(debuglevel > 2)
	(void)printf("/* %s */\n",def_list[i]);
#endif
		while (fgets(buf,256,FF)) fputs(buf,stdout);
		(void)fclose(FF);
		(void)printf("\n");
	}

	for (i=0,k=0; i < compindex; i++){
		if (complist[i].c_type != T_VALUE
		||  complist[complist[i].c_father].c_type == T_FLAGS){
			if (k == 1) k = 2;
			continue;
		}
		if (!k)
			(void)printf("\n/* CONSTANT DEFINITIONS */\n\n");
		else	if (k == 2) (void)printf("\n");
		k = 1;
		(void)printf("#define %s %s\n", 
		complist[i].c_name, complist[i].c_index);
	}

	for (i=0; i < compindex; i++){
		if (complist[i].c_type != T_VALUE
		||  complist[complist[i].c_father].c_type != T_FLAGS){
			if (k == 3) k = 4;
			continue;
		}
		switch(k){
		case 0:
			(void)printf("\n/* CONSTANT DEFINITIONS */\n\n");
			(void)printf("#ifdef BIGENDIAN\n");
			break;
		case 1:
			(void)printf("\n");
		case 2:	(void)printf("#ifdef BIGENDIAN\n");
			break;
		case 3: break;
		case 4: (void)printf("\n");
		}
		k = 3;
		this_bit = atoi(complist[i].c_index);
		(void)printf("#define %s (1<<((sizeof(int)*8)-%d))\n",
		complist[i].c_name, this_bit +1);
	}

	if (k > 2){
		(void)printf("#else\n"); k=2;
		for (i=0; i < compindex; i++){
			if (complist[i].c_type != T_VALUE
			||  complist[complist[i].c_father].c_type != T_FLAGS){
				if (k == 3) k = 4;
				continue;
			}
			if (k == 4) (void)printf("\n");
			k = 3;
			this_bit = atoi(complist[i].c_index);
			which_byte = this_bit/8;
			this_bit = 7 - (this_bit%8) + which_byte*8;
			(void)printf("#define %s (1<<(%d))\n",
			complist[i].c_name, this_bit);
		}
		(void)printf("#endif\n");
	}

	for (i=0,k=0; i < compindex; i++){
		if (complist[i].c_type != T_TYPE
		|| (complist[i].c_call_mode != DEFINE_MODE
		 && complist[i].c_call_mode != HALF_MODE)) continue;
		if (!k)
			(void)printf("\n/* CODING AND DECODING MACROS */\n\n");
		k = 1;
		for (j= complist[i].c_first_son;j != -1;
		j = complist[j].c_brother)
			if (complist[j].c_type != T_PARAMETER) break;
		current_list_head = complist[i].c_first_son;
		macrogen(&complist[i],&complist[j],ENCODE);
		macrogen(&complist[i],&complist[j],LENGTH);
		if (compilation_mode&LIGHTWEIGHT)
			macrogen(&complist[i],&complist[j],LWCOD);
		if (compilation_mode&INOUT){
			macrogen(&complist[i],&complist[j],OUT);
			macrogen(&complist[i],&complist[j],OUTLEN);
		}
		if (complist[i].c_call_mode == DEFINE_MODE){
			macrogen(&complist[i],&complist[j],DECODE);
			macrogen(&complist[i],&complist[j],FREE);
			if (compilation_mode&LIGHTWEIGHT)
				macrogen(&complist[i],&complist[j],LWDEC);
			if (compilation_mode&INOUT)
				macrogen(&complist[i],&complist[j],IN);
			macrogen(&complist[i],&complist[j],ERRCOD);
			if (compilation_mode&COPYING)
				macrogen(&complist[i],&complist[j],C_CPY);
		}
	}
	(void)printf("\n/* TYPE MATCHING MACROS */\n");
	for (i=0; i < compindex; i++){
		if (complist[i].c_type != T_TYPE) continue;
		inclmatch(&complist[i]);
	}

        for (i=0,k=0; i < compindex; i++){
		if (complist[i].c_type != T_TYPE
		|| complist[i].c_call_mode == DEFINE_MODE) continue;
		if (!k)
		(void)printf("\n/* CODING AND DECODING PROCEDURES */\n\n");
		k = 1;
		incltype(&complist[i]);
	}
	return(0);
}

/****************************************************************************\
* 									     *
* For each data type, the list of procedures which have to be generated	     *
* depend of the ``generation mode'': only the decoding for ``half obvious''  *
* types, ``sequence'' components coding and decoding and possibly also	     *
* ``set'' components decoding in some cases.				     *
* 									     *
\****************************************************************************/
incltype(c)
struct comp * c;
{
	decl_routine(c,DECODE);
	decl_routine(c,FREE);
	decl_routine(c,ERRCOD);

	if (compilation_mode&LIGHTWEIGHT)
		decl_routine(c,LWDEC);

	if (compilation_mode&INOUT)
		decl_routine(c,IN);
	if (compilation_mode&COPYING)
		decl_routine(c,C_CPY);

	if (c->c_call_mode == HALF_MODE) return;

	decl_routine(c,ENCODE);
	decl_routine(c,LENGTH);
	if (compilation_mode&LIGHTWEIGHT)
		decl_routine(c,LWCOD);

	if (compilation_mode&INOUT){
		decl_routine(c,OUT);
		decl_routine(c,OUTLEN);

	}

	if (c->c_call_mode != SEQUENCE_MODE && c->c_call_mode != SET_MODE) 
		return;
	decl_routine(c,COMPCOD);

	if (compilation_mode&INOUT)
		decl_routine(c,C_OUT);
	if (c->c_call_mode != SEQUENCE_MODE) 
		return;
	decl_routine(c,C_SEQ_D);
	if (compilation_mode&INOUT){
		decl_routine(c,C_IN);
	}
	(void) printf("\n");
	return;
}

decl_routine(c,cmode)
struct comp * c;
int cmode;
{	int callmode, i, j, need_comma;
	if (is_declared_procedure(c->c_name, cmode))
		return;
	(void) printf("%s %s%s (", exec_table[cmode].ret_type,
	c->c_name, exec_table[cmode].suffix);

	if (exec_table[cmode].arg_proto){
		printf("%s", exec_table[cmode].arg_proto);
		need_comma = 1;
	}else	need_comma = 0;

	callmode = exec_table[cmode].arg_mode;
	for (j=0; j<2; j++){
		for (i=c->c_first_son;i != -1;i = complist[i].c_brother){
			if (complist[i].c_type != T_PARAMETER) break;
			if (need_comma) 
				(void)printf(",");
			else	need_comma = 1;
			if (complist[i].c_call_mode == 3){
				(void)printf("void *");
				if (callmode) 
					(void)printf("*");
				continue;
			}
			printf("%s",complist[i].c_index);
			if (callmode == 0 && complist[i].c_call_mode == 1
			||  callmode == 1 && complist[i].c_call_mode != 2)
				(void)printf("*");
		}
		if (cmode != C_CPY) break;
		callmode = 0;
	}
	printf(");\n");
}

/***************************************************************************\
* 									    *
* Coding or decoding by macro: ``foo'' is only a reference to ``bar''.	    *
* 									    *
* 	#define foo_cod(z,m,n,a,b,c,d) (\				    *
* 		bar_cod(z,m,n,a.x,b,c.y,d))				    *
* 	#define foo_dec(z,m,n,a,b,c,d) \				    *
* 		bar_dec(z,m,n,a.x,b,&c.y,d)				    *
* 									    *
* The "bar_cod" and "bar_dec" procedure may be particular for the build in  *
* types: we refer to some specific procedures within the ASN1 library.	    *
* 									    *
\***************************************************************************/
macrogen(foo,bar,callmode)
struct comp * foo, * bar;
int callmode;
{	char * title;
	int extcall;

	if (is_declared_procedure(foo->c_name,callmode))
		return;

	current_call_mode = exec_table[callmode].arg_mode;
	extcall = callmode|INCLUDE_FLAG;
	title = (bar->c_type == T_EXTERN)?bar->c_name:
	extern_title(bar->c_type);

	(void)printf("#define ");

	(void) proc_headline(foo,0,callmode);

	(void)printf(" \\\n\t%s%s(",title,exec_table[callmode].suffix);
	if (exec_table[callmode].common_args)
		(void)printf("%s",exec_table[callmode].common_args);

	if (bar->c_type == T_EXTERN)
		(void) cdextern(bar,0,0,-1,extcall);
	else	(void) cdbasic(bar,0,0,-1,extcall,0);
	(void)printf(")\n");
	return;
}


char * extern_title(i)
int i;
{
	int j, allocate=0;

	if ((j = index_basic_op(i, &allocate))<0)
		return("PANIC");
	else	return(basic_table[j].bt_name);
}

/***************************************************************************\
* 									    *
* Type matching macros							    *
* 									    *
* When decoding CHOICES, SETs, or optional components in SEQUENCES, 	    *
* one may have to decide whether the next item is or is not an element of   *
* type ``X''. MAVROS generate here a ``X_match(z)'' macro, which returns a  *
* boolean value, TRUE if the tags present in the encoding match, false	    *
* otherwise.								    *
* 									    *
\***************************************************************************/
inclmatch(c)
struct comp * c;
{	int first_comp;

	(void)printf("#define %s%s(z) (",c->c_name,exec_table[MATCH].suffix);
	for (first_comp= c->c_first_son;first_comp != -1;
	first_comp = complist[first_comp].c_brother)
		if (complist[first_comp].c_type != T_PARAMETER)
			break;
	if (complist[first_comp].c_mode == -1
	&& (complist[first_comp].c_type == T_ANY
	||  complist[first_comp].c_type == T_ANY_ALLOC
	||  complist[first_comp].c_type == T_ANYDEF))
	       (void)printf("1");
	else if (complist[first_comp].c_type == T_CHOICE
	&&  complist[first_comp].c_mode == -1){
		match_incl_choice(&complist[first_comp]);
	}else 	if (complist[first_comp].c_type == T_EXTERN
	&&  complist[first_comp].c_mode == -1){
		match_incl_extern(&complist[first_comp]);
	}else	match_incl_asn1(complist[first_comp].c_mode, 
		complist[first_comp].c_number, asnstruct(&complist[first_comp]));
	(void)printf(")\n");
}

match_incl_extern(c)
struct comp * c;
{
	(void)printf("%s%s(z)",c->c_name,exec_table[MATCH].suffix);
}

match_incl_choice(c)
struct comp * c;
{	int i, first = 0;

	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (first++)
			(void)printf("\\\n||\t");
		if (complist[i].c_mode == -1
		&&  complist[i].c_type == T_CHOICE)
			match_incl_choice(&complist[i]);
		else if (complist[i].c_mode == -1
		&&  complist[i].c_type == T_EXTERN)
			match_incl_extern(&complist[i]);
		else if (complist[i].c_mode == -1
		&& ( complist[i].c_type == T_ANY
		||   complist[i].c_type == T_ANY_ALLOC
		||   complist[i].c_type == T_ANYDEF))
		 	(void)printf("(1)");
		else{	(void)printf("(");
			match_incl_asn1(complist[i].c_mode, complist[i].c_number, 
			asnstruct(&complist[i]));
			(void)printf(")");
		}
	}
}

match_incl_asn1(m,n,t)
int m,n,t;
{
	unsigned char x; int l=0,k,i=0;

	if (n < 31){
		switch (t){
		case 0:
			x = (m<<6)|n;
			(void)printf("(z)[0] == %d",x);
			break;
		case 1:
			x = (m<<6)|(1<<5)|n;
			(void)printf("(z)[0] == %d",x);
			break;
		case 2:
			x = (m<<6)|(1<<5)|n;
			(void)printf("((z)[0]|32) == %d",x);
			break;
		}
	}else{	(void)printf("\\\n\t");
		switch (t){
		case 0:
			x = (m<<6)|31;
			(void)printf("(z)[0] == %d",x);
			break;
		case 1:
			x = (m<<6)|(1<<5)|31;
			(void)printf("(z)[0] == %d",x);
			break;
		case 2:
			x = (m<<6)|(1<<5)|31;
			(void)printf("((z)[0]|32) == %d",x);
			break;
		}
		k = n; while(k){ l++; k = k>>7;}
		while (l--){
			i++;
			if (l)
				x = 128 | ((n>>(7*l))&127);
			else	x = n&127;
			(void)printf(" && (z)[%d] == %d",i,x);
		}
	}
}

