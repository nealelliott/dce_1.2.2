/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavrosbase.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:12  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:09:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:02  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:39:08  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:03:33  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:03:04  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:07:05  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:11:32  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:54:38  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:56:29  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  11:26:24  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:21:52  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:33:37  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavrosbase.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:17 $";
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
#define HEAD 128

extern struct tstrd * strd;
extern int procindex;
extern int compindex;
extern int linenum;
extern int debuglevel;
extern struct comp * complist;

extern char * filein;
extern char * package;
extern int incl_list_nb, srce_list_nb;
extern char ** incl_list, ** srce_list;
extern int def_list_nb;
extern char ** def_list;
extern int defined_proc_list_size;
extern char ** defined_proc_list;
extern int compilation_mode;
extern int session_protocol_mode;
extern int af_anydef;


/************************************************************************\
* 									 *
* Tree exploration:							 *
* 									 *
* Before encoding a routine, one must:					 *
* 									 *
* - list all the indexes of the embedded ``sequences of'' or ``set of''	 *
*   components,								 *
* 									 *
* - compute the ``maximum depth'' of the tree, in order to correctly	 *
*   dimension the size of the component stack,				 *
* 									 *
* - compute the maximum number of embedded set components, in order to	 *
*   dimension the size of the ``set component masks''.			 *
* 									 *
\************************************************************************/
char ** storelist;
int storemax = 0, current_list_head = 0, current_call_mode = 0;
int storeindex;
char * shortname();

store_in_list(s)
char * s;
{
	int i;

	for (i=0; i<storeindex; i++)
		if (strcmp(s,storelist[i])==0) return;
	if (storeindex >= storemax){
		if (storemax){
			storemax = 2*storemax;
			storelist = (char **) realloc((char *)storelist,
			(unsigned)(storemax*sizeof(char *)));
		}else{	storemax = 32;
			storelist = (char **) malloc((unsigned)(storemax*sizeof(char *)));
		}
		if (storelist == 0) memory_error();
	}
	storelist[storeindex++] = s;
}

get_depth (c,d,sc)
struct comp * c;
int d, *sc;
{	int i, j, k = 0, mset = 0, setc, comp=0;

	*sc = 0;

	if (c->c_type == T_PARAMETER) return(d);
	d += (c->c_mode != -1 && c->c_implicit == 0)? 2 : 1;
	if (c->c_type == T_COMP
	||  c->c_type == T_EXTERN) return(d);

	for (i=c->c_first_son;i!= -1; i=complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if ((j= get_depth(&complist[i],0,&setc)) > k) k = j;
		if (setc > mset) mset = setc;
		comp++;
	}
	if (c->c_type == T_SEQUENCE_OF || c->c_type == T_SET_OF)
		store_in_list(c->c_name);
	if (c->c_type == T_SET)
		mset += comp;
	*sc = mset;
	return(d+k);
}

/**************************************************************************\
* 									   *
* Line numbering							   *
* 									   *
* We produce in the generated source a line compatible with the output of  *
* the C preprocessor. The result is that error messages generated at 	   *
* routine compile time will be relative to the mavros input.		   *
* 									   *
* In debug mode, one will often want to debug the generated code instead.  *
* This is achieved by setting the debug level to a value larger than 5.	   *
* 									   *
* In some cases, one wants to suppress that line numbering without using   *
* the "debug" mode; this is done by a command line argument.               *
*                                                                          *
\**************************************************************************/
int dont_numberline = 0;

numberline(i)
int i;
{
        if (debuglevel <= 5 && dont_numberline == 0)
        (void)printf("\n# line %d \"%s\"\n",i,filein);
}


/**************************\
* 			   *
* Arguments to procedures  *
* 			   *
\**************************/

firstarg(c)
struct comp * c;
{
	int j;
	for (j=c->c_first_son;j != -1;j=complist[j].c_brother)
		if (complist[j].c_type == T_ARGUMENT) break;
	return(j);
}

argnumber(c,n)
struct comp * c;
int n;
{	int i,j;

	for (i=0, j=firstarg(c); i<n; i++, j=complist[j].c_brother){
		if (j != -1)
			if (complist[j].c_type == T_ARGUMENT) continue;
		linenum = c->c_line_num;
		stxerr("Not enough arguments",c->c_name);
		return(1);
	}
	if (j != -1)
		if (complist[j].c_type == T_ARGUMENT){
			linenum = c->c_line_num;
			stxerr("Too many arguments",c->c_name);
			return(1);
		}
	return(0);
}

arglst (c_first_son, l0, nocomma)
int c_first_son, l0, nocomma;
{	int i;

	for (i=c_first_son;i != -1;i = complist[i].c_brother){
		if (complist[i].c_type != T_PARAMETER) break;
		if (nocomma || i != c_first_son) (void)printf(",");
		(void)printf("%s",complist[i].c_name);
	}
	return(l0);
}

arglst_cpy (c_first_son, l0, nocomma)
int c_first_son, l0, nocomma;
{	int i;

	for (i=c_first_son;i != -1;i = complist[i].c_brother){
		if (complist[i].c_type != T_PARAMETER) break;
		if (nocomma || i != c_first_son) (void)printf(",");
		(void)printf("asn1_%s",complist[i].c_name);
	}
	return(l0);
}

argdcl (c_first_son, m, l0)
int 	c_first_son, m, l0;
{	int i;

	for (i=c_first_son;i != -1;i = complist[i].c_brother){
		if (complist[i].c_type != T_PARAMETER) break;
		if (complist[i].c_line_num != l0) numberline(l0 = complist[i].c_line_num);
		(void)printf("%s",complist[i].c_index);
		if((m == 0 && complist[i].c_call_mode == 1)
		|| (m == 1 && complist[i].c_call_mode != 2))	(void)printf (" *");
		(void)printf(" %s;\n",complist[i].c_name); l0++;
	}
	return(l0);
}

argdcl_cpy (c_first_son, m, l0)
int 	c_first_son, m, l0;
{	int i;

	for (i=c_first_son;i != -1;i = complist[i].c_brother){
		if (complist[i].c_type != T_PARAMETER) break;
		if (complist[i].c_line_num != l0) 
			numberline(l0 = complist[i].c_line_num);
		(void)printf("%s",complist[i].c_index);
		if((m == 0 && complist[i].c_call_mode == 1)
		|| (m == 1 && complist[i].c_call_mode != 2))	
			(void)printf (" *");
		(void)printf(" asn1_%s;\n",complist[i].c_name); l0++;
	}
	return(l0);
}


/*************************************************************************\
* 									  *
* Argprint:								  *
* 									  *
* Print out an argument to a procedure. The ``m'' flag indicates whether  *
* the argument should be passed by value (m=0) or by address (m=1).	  *
* 									  *
\*************************************************************************/
argprint(c,m,l0)
struct comp * c;
int m, l0;
{
	int i,k, local_mode=0;

	if (c->c_type == T_ARGUMENT){
	for (k=current_list_head;k != -1; k=complist[k].c_brother){
		if (complist[k].c_type != T_PARAMETER) break;
		if (strcmp(complist[k].c_name,c->c_name)) continue;
		switch(complist[k].c_call_mode){
			case 0:
				local_mode= current_call_mode;
				break;
			case 1: local_mode=1; break;
			case 2: local_mode=0; break;
		}
		break;
	}}
	if (c->c_first_son == -1) {
		if (m == local_mode)	(void)printf("%s",c->c_name);
		else if (m == 0)	(void)printf("*%s",c->c_name);
		else			(void)printf("&%s",c->c_name);
	}else{
		if (m == 1) (void)printf("&");
		(void)printf((local_mode)?"(*%s)":"%s",c->c_name);

		for (i = c->c_first_son; i != -1; i = complist[i].c_brother){
			if (complist[i].c_type == T_ARGUMENT){
				(void)printf("[");
				l0 = argprint(&complist[i],0,l0);
				(void)printf("]");
			}else	(void)printf("%s%s",(complist[i].c_call_mode)?"->":".",
				complist[i].c_name);
		}
	}
	return(l0);
}

cpy_argprint(c,m,l0)
struct comp * c;
int m, l0;
{
	int i,k, local_mode=0;
	char * found_in_list = "";

	if (c->c_type == T_ARGUMENT){
	for (k=current_list_head;k != -1; k=complist[k].c_brother){
		if (complist[k].c_type != T_PARAMETER) break;
		if (strcmp(complist[k].c_name,c->c_name)) continue;
		switch(complist[k].c_call_mode){
			case 0:
				local_mode=0; break;
			case 1: local_mode=1; break;
			case 2: local_mode=0; break;
		}
		found_in_list = "asn1_";
		break;
	}}
	if (c->c_first_son == -1) {
		if (m != local_mode)
			PUTC((m == 0)?'*':'&',stdout);
		(void)printf("%s%s",found_in_list, c->c_name);
	}else{
		if (m == 1) (void)printf("&");
		(void)printf((local_mode)?"(*%s%s)":"%s%s",
		found_in_list, c->c_name);

		for (i = c->c_first_son; i != -1; i = complist[i].c_brother){
			if (complist[i].c_type == T_ARGUMENT){
				(void)printf("[");
				l0 = argprint(&complist[i],0,l0);
				(void)printf("]");
			}else	(void)printf("%s%s",
				(complist[i].c_call_mode)?"->":".",
				complist[i].c_name);
		}
	}
	return(l0);
}

/*********************************************************************\
* 								      *
* exec_table:							      *
* 								      *
* This table stores the various parameters which dscribe how to call  *
* a particular type of coding or decoding function.		      *
* 								      *
\*********************************************************************/
exec_flag exec_table[] = {
 {"_dec","asn1z=","asn1z,asn1zm",1,ASN1CODER,"asn1,asn1",
	"register asn1 asn1z, asn1zm;\n", "asn1", "asn1z"},
 {"_cod","asn1z=","asn1z,asn1_m,asn1_n",0,ASN1CODER,"asn1,int,int",
	 "int asn1_m,asn1_n; register asn1 asn1z;\n", "asn1", "asn1z"},
 {"_free",(char *)0,(char *)0,0,ASN1CODER,(char *)0,
	(char *)0, "void", (char *)0},
 {"_len","asn1l += ","asn1_n",0,ASN1CODER,"int",
	"int asn1_n;\n", "int", "asn1l"},
 {"_ccod","asn1z=","asn1z, asn1_x509",0,ASN1CODER,"asn1, int",
	"register asn1 asn1z; int asn1_x509;\n", "asn1", "asn1z"},
 {"_dseq","asn1z=","asn1z,asn1zm",1,ASN1CODER,"asn1,asn1",
	"register asn1 asn1z, asn1zm;\n", "asn1", "asn1z"},
 {"_err",(char *)0,(char *)0,1,ASN1CODER,(char *)0,
	(char *)0, "void", (char *)0},
 {"_lwc","asn1z=","asn1z",0,LIGHTWEIGHT,"asn1",
	"register asn1 asn1z;\n", "asn1", "asn1z"},
 {"_lwd","asn1z=","asn1z,asn1zm",1,LIGHTWEIGHT,"asn1,asn1",
	"register asn1 asn1z, asn1zm;\n", "asn1", "asn1z"},
 {"_match",(char *)0,(char *)0,0,ASN1CODER,(char *)0,
	(char *)0, "void", (char *)0},
 {"_in","asn1z=","asn1z,asn1zm",1,INOUT,"char *, char *",
	"register char *asn1z, *asn1zm;\n", "char *", "asn1z"},
 {"_out","asn1z=","asn1z",0,INOUT,"char *",
	"register char * asn1z;\n", "char *", "asn1z"},
 {"_olen","asn1l += ",(char *)0,0,INOUT,(char *)0,
	(char *)0, "int", "asn1l"},
 {"_cin","asn1z=","asn1z,asn1zm",1,INOUT,"char *, char *",
	"register char *asn1z, *asn1zm;\n", "char *", "asn1z"},
 {"_cout","asn1z=","asn1z",0,INOUT,"char *",
	"register char * asn1z;\n", "char *", "asn1z"},
 {"_cpy",(char *)0,(char *)0,1,COPYING,(char *)0,
	(char *)0, "void", (char *)0}};

int sizeof_exec_table = (sizeof(exec_table) / sizeof(exec_flag));


/**************************************************************************\
* 									   *
* Coding the first parameters of either a basic call or an external call.  *
* 									   *
\**************************************************************************/

cd_extern_or_basic_call(callmode, m, n, element_name)
int callmode, m, n;
char * element_name;
{
	if (exec_table[callmode].value)
		(void)printf("%s",exec_table[callmode].value);
	(void)printf("%s%s(",element_name,exec_table[callmode].suffix);
	switch (callmode){
	case ENCODE:
		if (m != -1 && (m&HEAD))
			(void)printf("asn1z,asn1_m,asn1_n|asn1_x509");
		else if (n == 0)
			(void)printf("asn1z,%d,asn1_x509",m,n);
		else	(void)printf("asn1z,%d,%d|asn1_x509",m,n);
		break;
	case LENGTH:
		(void)printf("%d",n);
		break;
	default:
		if(exec_table[callmode].common_args)
		(void)printf("%s",exec_table[callmode].common_args);
	}

	return;		
}

/****************************************************************************\
* 									     *
* Coding the calls to the basic encoding routines.			     *
* 									     *
* This generates the call to the ``basic'' encoding and decoding types.	     *
* The call to each type is very much in line with that of the external	     *
* codings, e.g.:							     *
* 									     *
* 	<value> = <routine_name> (coding arguments,			     *
* 				parameters);				     *
* 									     *
* The coding routines for ASN-1 include a flag. By convention, this flag     *
* is encoded first, by a single instruction, or is similarly decoded --	     *
* if it is known, and if the particular type allows for that treatment.	     *
* The termination for this specific routines (or macros) will be "_vcod"     *
* or "_vdec", by opposition to the straight coding and decoding. Coding	     *
* macros will take then a coding argument (z), and decoding macros two (z,   *
* zmax).								     *
* 									     *
* The scalar decoding routines are treated by two steps:		     *
* value = decoded(area, &nextarea); area = nextarea;			     *
* this is embedded in a macro which assume the existence of a variable for   *
* "nextarea".								     *
* 									     *
* The number of parameters is a parameter of the routine type. In	     *
* principle, all parameters take the mode defined by the calling type, with  *
* the exception of the FLAGS were the second argument is a constant.	     *
* 									     *
\****************************************************************************/
int basic_one_parm[] = {0};
int basic_two_parms[] = {0,0};
int basic_flags_parms[] = {0,2};

struct basic_table_entry basic_table[] = {
{"asn1_null",		1,	1,	0,	0, (int *) 0},
{"asn1_any",		0,	0,	1,	2, basic_two_parms},
{"asn1_copy",		0,	0,	1,	2, basic_two_parms},
{"asn1_int",		1,	1,	0,	1, basic_one_parm},
{"asn1_boolean",	1,	1,	0,	1, basic_one_parm},
{"asn1_chars",		1,	0,	0,	1, basic_one_parm},
{"asn1_flags",		1,	0,	0,	2, basic_flags_parms},
{"asn1_bits",		1,	0,	1,	2, basic_two_parms},
{"asn1_octets",		1,	0,	1,	2, basic_two_parms},
{"asn1_oid",		1,	0,	1,	2, basic_two_parms},
{"asn1_real",		1,	1,	0,	1, basic_one_parm},
{"asn1_external",	0,	0,	0,	2, basic_two_parms},
{"asn1_ignore",		0,	0,	0,	0, (int *) 0}};

index_basic_op(i,allocate)
int i, *allocate;
{
	switch(i){
	case T_NULL:
		return(0);
	case T_ANY_ALLOC:
		*allocate = 1;
	case T_ANY:
		return(1);
	case T_COPY_ALLOC:
		*allocate = 1;
	case T_COPY:
		return(2);
	case T_INTEGER:
		return(3);
	case T_BOOLEAN:
		return(4);
	case T_CHARS:
		return(5);
	case T_FLAGS:
		return(6);
	case T_BITS_ALLOC:
		*allocate = 1;
	case T_BITS:
		return(7);
	case T_OCTETS_ALLOC:
		*allocate = 1;
	case T_OCTETS:
		return(8);
	case T_OID_ALLOC:
		*allocate = 1;
	case T_OID:
		return(9);
	case T_REAL:
		return(10);
	case T_EXTERNAL:
		return(11);
	case T_IGNORE:
		return(12);
	default:
		return(-1);
	}
}

print_basic_args(c,op_index, j, notfirstarg, arg_mode, l0)
struct comp * c;
int op_index;
int j, notfirstarg, arg_mode, l0;
{	int i, dist_mode;

	for (i = 0;  i < basic_table[op_index].bt_nb_parms 
	&& j != -1 && complist[j].c_type == T_ARGUMENT;
	i++, j = complist[j].c_brother){
		switch(basic_table[op_index].bt_parm_mode[i]){
			case 0:	dist_mode= arg_mode;
				break;
			case 1: dist_mode=1; break;
			case 2: dist_mode=0; break;
		}
		if (notfirstarg)
			PUTC(',',stdout);
		else	notfirstarg = 1;
		l0 = argprint(&complist[j],dist_mode,l0);
	}
	if (j != -1 && complist[j].c_type == T_ARGUMENT){
		linenum = c->c_line_num;
		stxerr("Too many arguments",c->c_name);
	}else if (i < basic_table[op_index].bt_nb_parms){
		linenum = c->c_line_num;
		stxerr("Not enough arguments",c->c_name);
	}
}


cdbasic(c,m,n,l0,callmode,allocmode)
struct comp * c;
int m,n,l0,callmode;
{       int i,j, op_index;
        int dist_mode = 0, notfirstarg, arg_mode, not_including;

	if ((op_index = index_basic_op(c->c_type, &allocmode)) < 0){
		stxerr("PANIC: unknown basic type", c->c_name);
		return(l0);
	}

	if ((not_including = ((callmode&INCLUDE_FLAG)==0)) == 0){
		callmode &= ~INCLUDE_FLAG;
	}else if (m != -1 && callmode == ENCODE && 
	basic_table[op_index].bt_vcod){
		l0 += openasncod(m,n,0);
		(void)printf("asn1z = %s_vcod(asn1z",
		basic_table[op_index].bt_name);
		notfirstarg = 1;
	}else if (m != -1 && callmode == DECODE && 
	basic_table[op_index].bt_vdec){
		l0 += asn1open(m,n,0,1); /* unsafe ! */
		(void)printf("asn1z = %s_vcod(asn1z,asn1zm", 
		basic_table[op_index].bt_name);
		notfirstarg = 1;
	}else{  cd_extern_or_basic_call(callmode, m, n, 
		basic_table[op_index].bt_name);
	}
	notfirstarg = (exec_table[callmode].common_args != 0);
	arg_mode = exec_table[callmode].arg_mode;

	l0 = print_basic_args(c,op_index, j = firstarg(c),
	notfirstarg, arg_mode, l0);

	if (callmode == C_CPY)
	for (i = 0;
	i < basic_table[op_index].bt_nb_parms && j != -1;
	i++, j = complist[j].c_brother){
		switch(basic_table[op_index].bt_parm_mode[i]){
			case 0:	dist_mode= 0;
				break;
			case 1: dist_mode=1; break;
			case 2: dist_mode=0; break;
		}
		PUTC(',',stdout);
		l0 = cpy_argprint(&complist[j],dist_mode,l0);
	}
	if (not_including){
		printf(");\n"); l0++;
	}
	return(l0);
}

/****************************************************************************\
* 									     *
* cdextern:								     *
* 									     *
* A single procedure, with a "callmode" flag, generates the call to a	     *
* subroutine that will perform the required function, e.g.:		     *
* 	DECODE: decode the element,					     *
* 	ENCODE:	code the element,					     *
* 	FREE:	free the allocated areas,				     *
* 	COMPCOD: code the components,					     *
* etc..(the flags are defined in "mavrospro.h", as external routine	     *
* generation flags).							     *
* 									     *
* The procedure will generate a call to a routine like:			     *
*  asn1z = element_dec(asn1z,asn1zm,element)				     *
* composed of the following segments:					     *
*  <value><name><suffix>(<common_args>,<argument_list>)			     *
* Where <name> is indeed the name of the element to be encoded or decoded,   *
* and where <value>, <suffix> and <common_args> depend from the "callmode".  *
* 									     *
* In order to generate properly the <argument_list>, one will search the     *
* declaration of the element in the "component list", and one will	     *
* determine for each argument its mode: by address or by value -- the	     *
* default mode depending of the "callmode".				     *
* 									     *
* Note that when generating the argument list we have to take care of those  *
* modes which dont require any "common args", so has not to generate any     *
* leading comma...							     *
* 									     *
* The routines can also be called in a "include" mode, in order to	     *
* generate the call list of "include" declarations. In that case, neither    *
* the <name> or <suffix> components are printed, nor the parenthesis; also,  *
* one should not generate any comments, even in debug mode.		     *
* 									     *
* The arguments corresponding to the various "callmode" are collected in     *
* the "exec_table".							     *
* 									     *
\****************************************************************************/
which_extern_type (name)
char * name;
{	int j;

	for (j=0;j<compindex;j++)
		if ((complist[j].c_type == T_TYPE
		||  complist[j].c_type == T_IMPORT)
		&&  strcmp(complist[j].c_name, name) == 0)
			return(j);
	return(-1);
}

print_extern_args(c, cref, l0, notfirstarg, arg_mode)
struct comp * c, * cref;
int l0, notfirstarg, arg_mode;
{	int i, j, dist_mode;

	for (j = cref->c_first_son, i=firstarg(c);
	i != -1; i = complist[i].c_brother, j = complist[j].c_brother){
		if (complist[i].c_type != T_ARGUMENT) break;
		if (j == -1 || complist[j].c_type != T_PARAMETER){
			linenum = complist[i].c_line_num;
			stxerr("Too many arguments",complist[i].c_name);
			break;
		}else switch(complist[j].c_call_mode){
			case 3:
			case 0:	dist_mode= arg_mode;
				break;
			case 1: dist_mode=1; break;
			case 2: dist_mode=0; break;
		}
		if (notfirstarg)
			PUTC(',',stdout);
		else	notfirstarg = 1;

		l0 = argprint(&complist[i],dist_mode,l0);
	}
	return(l0);
}

cdextern(c,m,n,l0,callmode)
struct comp * c;
int m,n,l0,callmode;
{	int i,j;
	int dist_mode = 0, notfirstarg, arg_mode, not_including;

#ifdef DEBUG
if ((debuglevel > 4) && ( (callmode&INCLUDE_FLAG) == 0)){
(void)printf("/* Calling %s%s */\n", c->c_name, exec_table[callmode].suffix); l0++;
(void)fflush(stdout);}
#endif
	if (l0 != c->c_line_num && l0 != -1) numberline(l0 = c->c_line_num);
	if (not_including = ((callmode&INCLUDE_FLAG) == 0)){
		cd_extern_or_basic_call(callmode, m, n, c->c_name);
	}else	callmode &= ~INCLUDE_FLAG;

	arg_mode = exec_table[callmode].arg_mode;

	if ((j = which_extern_type(c->c_name))<0){
		linenum = c->c_line_num;
		stxerr("Unknown type",c->c_name);
		return(l0);
	}
	notfirstarg = (exec_table[callmode].common_args != 0);
	l0 = print_extern_args(c, &complist[j], l0, 
	notfirstarg, arg_mode);

	if (callmode == C_CPY){
	for (j=complist[j].c_first_son, i=firstarg(c); i != -1;
	i = complist[i].c_brother, j = complist[j].c_brother){
		if (complist[i].c_type != T_ARGUMENT) break;
		if (j == -1 || complist[j].c_type != T_PARAMETER){
			linenum = complist[i].c_line_num;
			stxerr("Too many arguments",complist[i].c_name);
			break;
		}else switch(complist[j].c_call_mode){
			case 0:	dist_mode= 0; break;
			case 1: dist_mode=1; break;
			case 2: dist_mode=0; break;
		}
		PUTC(',',stdout);
		l0 = cpy_argprint(&complist[i],dist_mode,l0);
	}}
	if (not_including){
		(void)printf(");\n"); l0++;}

#ifdef DEBUG
if (debuglevel > 4 && not_including){
(void)printf("/* End of call to %s%s */\n", c->c_name, exec_table[callmode].suffix); 
l0++;
(void)fflush(stdout);}
#endif
	return(l0);
}

/***********************************************************************\
* 								        *
* Coding of the ANY DEFINED BY procedures.			        *
* 								        *
* These are defined by a declaration of the form:		        *
* 								        *
* type ::= ANY variable DEFINED BY procedure(arguments)		        *
* 								        *
* We have to generate, for each type of procedure, a call of the form:  *
* 								        *
* asn1z = (*procedure_dec(arguments))(asn1z,asn1zm,variable)	        *
* 								        *
* where the said variable is always passed by address.		        *
* 								        *
\***********************************************************************/
cd_anydef(c,l0,cmode)
struct comp     * c;
int             l0;
int             cmode;
{	register exec_flag * mode = &exec_table[cmode&~INCLUDE_FLAG];
	register the_arg = firstarg(c), i;

	if (cmode&INCLUDE_FLAG)
		cmode &= ~INCLUDE_FLAG;
	else if (mode->value)
		(void)printf("%s",mode->value);
	(void)printf("(*%s%s(",c->c_name, mode->suffix);
	for (i=complist[the_arg].c_brother; i != -1
	&& complist[i].c_type==T_ARGUMENT;
	i = complist[i].c_brother){
		if (i != complist[the_arg].c_brother)
			(void)printf(",");
			if (af_anydef)
				l0 = argprint(&complist[i],1,l0);
			else	l0 = argprint(&complist[i],0,l0);
	}
	if (af_anydef){
		(void)printf(",");
	        l0 = argprint(&complist[the_arg],1,l0);
	}

	(void)printf("))(");
	if (cmode == ENCODE)
		(void)printf("asn1z,-1,0,");
	else if (mode->common_args)
		(void)printf("%s,",mode->common_args);
	l0 = argprint(&complist[the_arg],1,l0);
	if (cmode == C_CPY){
		PUTC(',',stdout);
		if (af_anydef)
			l0 = cpy_argprint(&complist[the_arg],0,l0);
		else	l0 = cpy_argprint(&complist[the_arg],1,l0);
	}
	(void)printf(");\n"); l0++;
	return(l0);
}

/***********************************************************************\
* 								        *
* decl_proc_head:						        *
* Writes the first lines of a coding or decoding function declaration:  *
*   asn1 element_dec(asn1z, element)				        *
*   register asn1 asn1z;					        *
*   element_type * element;					        *
* 								        *
* proc_headline:						        *
* Writes the procedure name and its arguments:			        *
*   element_dec(asn1z, element)					        *
* 								        *
\***********************************************************************/
decl_proc_head(c,l0,cmode)
struct comp 	* c;
int 		l0;
int		cmode;
{	int callmode;

	if (exec_table[cmode].ret_type)
		printf("%s ", exec_table[cmode].ret_type);
	callmode = exec_table[cmode].arg_mode;
	l0 = proc_headline(c,l0,cmode);
	(void)printf("\n"); l0++;
	if (exec_table[cmode].arg_decl){
		(void)printf("%s",exec_table[cmode].arg_decl);
		l0++;
	}
	l0 = argdcl(c->c_first_son, callmode, l0);
	if (cmode == C_CPY){
		l0 = argdcl_cpy(c->c_first_son, 0,1);
	}
	return(l0);
}

proc_headline(c,l0,cmode)
struct comp * c;
int l0, cmode;
{
	(void)printf("%s%s(",c->c_name,exec_table[cmode].suffix);
	if (exec_table[cmode].common_args)
		(void)printf("%s",exec_table[cmode].common_args);
	l0 = arglst(c->c_first_son, l0,exec_table[cmode].common_args != 0);
	if (cmode == C_CPY){
		l0 = arglst_cpy(c->c_first_son, l0,1);
	}
	(void)printf(")");
	return(l0);
}

/**************************************************************\
* 							       *
*  This procedure prints the header, common to all routines.   *
* 							       *
\**************************************************************/

printhead()
{	int i, l0=0, lp = strlen(package), found = 0;

#if defined(nsc32000)
	(void)printf("#include \"asn1.h\"\n"); l0++;
#else
	(void)printf("#include <dce/asn1.h>\n"); l0++;
#endif

	for(i=0; i<incl_list_nb;i++){
#if defined(nsc32000)
		(void)printf("#include \"%s\"\n", incl_list[i]);
#else
		(void)printf("#include <%s>\n", incl_list[i]);
#endif
		l0++;
		if (found == 0
		&&  strncmp(incl_list[i],package,lp) == 0
		&&  strcmp(incl_list[i] + lp,".h") == 0)
			found = 1;
	}
	if (found == 0){
#if defined(nsc32000)
		(void)printf("#include \"%s.h\"\n",package); l0++;
#else
		(void)printf("#include <%s.h>\n",package); l0++;
#endif
	}
	return(l0);
}


/****************************************************\
* 						     *
* Generation of the makefile - quite brute force !   *
* 						     *
\****************************************************/

genermake()
{
	char fname[256];
	int i, j, k;

	(void) sprintf(fname,"%s.make",package);
	if (freopen(fname,"w",stdout)==0){
		perror(fname);
		return(-1);
	}
	(void)printf("\nRANLIB= ranlib\nCC= cc\n");
	(void)printf("\nCFILES= ");
	if (compilation_mode&ONE_FILE){
		(void)printf("%s.c ",package);
	}else for (i=0; i < compindex; i++){
		switch (complist[i].c_type){
		case T_TYPE:
			if (complist[i].c_optional != 0 
			|| complist[i].c_call_mode == DEFINE_MODE) break;
			(void)printf("\\\n\t");

			for(j=0,k=0;j<sizeof_exec_table;j++){
			if ((exec_table[j].comp_mode&compilation_mode) == 0)
	                          continue;
			if (is_head_of_file(complist[i].c_name,
			j,complist[i].c_call_mode)){
			if (k==0){
				(void)printf("\\\n\t");
				k=1;
			}
			(void)printf("%s%s.c ",
			shortname(complist[i].c_name),exec_table[j].suffix);
			}}
		default:
			break;
		}
	}
	(void)printf("\nSOURCES=");
	for (i=0;i < srce_list_nb; i++)
		(void)printf("\\\n\t%s.c ",srce_list[i]);

	(void)printf("\nOFILES=");
	for (i=0;i < srce_list_nb; i++)
		(void)printf("\\\n\t%s.o ",srce_list[i]);
	if (compilation_mode&ONE_FILE){
		(void)printf("\\\n\t%s.o ",package);
	}else for (i=0; i < compindex; i++){
		switch (complist[i].c_type){
		case T_TYPE:
			if (complist[i].c_optional != 0 
			|| complist[i].c_call_mode == DEFINE_MODE) break;
			for(j=0,k=0;j< sizeof_exec_table;j++){
			if ((exec_table[j].comp_mode&compilation_mode) == 0)
	                          continue;
			if (is_head_of_file(complist[i].c_name,
			j,complist[i].c_call_mode)){
			if (k==0){
				(void)printf("\\\n\t");
				k=1;
			}
			(void)printf("%s%s.o ",
			shortname(complist[i].c_name),exec_table[j].suffix);
			}}
		default:
			break;
		}
	}
	(void)printf("\nall: %s.a\n",package);
	(void)printf("\n%s.a: ${OFILES}\n",package);
	(void)printf("\tar cr %s.a ${OFILES}\n",package);
	(void)printf("\t${RANLIB} %s.a\n",package);
	(void)printf("\techo %s.a is up to date\n\n",package);

	(void)printf("\nclean: cleansrce cleanobj");
	(void)printf("\ncleansrce:\n\t-rm ${CFILES}\n");
	(void)printf("\ncleanobj:\n\t-rm ${OFILES}\n");
	(void)printf("\nlint: llib-l%s.ln",package);
	(void)printf("\nllib-l%s.ln: ${CFILES} ${SOURCES}\n",package);
	(void)printf("\tlint -o %s ${LINTFLAGS} ${CFLAGS} ${CFILES} ${SOURCES}\n",package);

	(void)printf("\n.c.o:\n\t${CC} -c ${CFLAGS} $*.c\n");
#ifdef DEBUG
(void)fflush(stdout);
#endif
	return(0);
}

#ifdef LONG_NAME_SIZE_OK
#define MAX_NAME_SIZE 128
#else
#define MAX_NAME_SIZE 7
#endif
char * shortname(x)
char * x;
{ static char shn[14]; unsigned short d = 0;

for (;*x;x++){
	d = d*1001;
	d += *x;}
(void) sprintf(shn,"MVR%x",d);
return(shn);
}

/********************\
* 		     *
* Opening of files.  *
* 		     *
\********************/

open_single_output_file()
{	char fname[32];

#ifdef DEBUG
	if (debuglevel > 7){
	(void)fprintf(stderr,"Opening %s.c\n", package);
	(void)fflush(stderr);}
#endif
	(void) sprintf(fname,"%s.c",package);
	if (freopen(fname,"w",stdout) == 0){
		perror(fname);
		return(-1);
	}
	return(printhead());
}

open_module_file(type_name,mode,l0)
char * type_name;
int mode, l0;
{	char fname[256];

	if (compilation_mode&ONE_FILE)
		return(l0);
	(void) sprintf(fname,"%s%s.c",
	shortname(type_name), exec_table[mode].suffix);
#ifdef DEBUG
	if (debuglevel > 7){
	(void)fprintf(stderr,"Opening %s\n",fname);
	(void)fflush(stderr);}
#endif
	if (freopen(fname,"w",stdout) == 0){
		perror(fname);
		return(-1);
	}
	l0 = printhead();
	return(l0);
}

/***************************************************************************\
* 									    *
* Is_head_of_file:							    *
* 									    *
* Determines whether the type of routine mentionned here is the first of a  *
* coding file.								    *
* 									    *
* It is used by the ``make file'' generator, and should be kept coherent    *
* with the type generation routines.					    *
* 									    *
\***************************************************************************/

is_head_of_file (c_name, c_mode, type_mode)
char * c_name;
int c_mode, type_mode;
{
	if (is_declared_procedure(c_name, c_mode)){
		if (!( c_mode == DECODE
		&& is_declared_procedure(c_name, FREE) == 0
		&& is_declared_procedure(c_name, ERRCOD) == 0))
			return(compilation_mode&ASN1CODER);
		else if (c_mode == ENCODE
		&& is_declared_procedure(c_name, LENGTH) == 0)
			return((compilation_mode&ASN1CODER)
			&&(type_mode != HALF_MODE));
		else	return(0);
	}

	switch(c_mode) {
	case DECODE:
		return(compilation_mode|ASN1CODER);
	case ENCODE:
		return((compilation_mode|ASN1CODER)&&(type_mode != HALF_MODE));
	case LWCOD:
		return(compilation_mode|LIGHTWEIGHT);
	case LWDEC:
		return((compilation_mode|LIGHTWEIGHT)&&(type_mode != HALF_MODE));
	case IN:
		return(compilation_mode|INOUT);
	case OUT:
		return((compilation_mode|INOUT)&&(type_mode != HALF_MODE));
	case C_CPY:
		return(compilation_mode|COPYING);
	default:
		return(0);
	}
}

/**********************************************************************\
* 								       *
* Look up in the list of ``declared'' modules, to determine whether a  *
* particular subroutine shall be encoded or not.		       *
* 								       *
\**********************************************************************/

is_declared_procedure(c_name, exec_mode)
char * c_name;
int exec_mode;
{
	int i;
	char procname[256];

	(void)sprintf(procname,"%s%s",c_name,exec_table[exec_mode].suffix);
	for (i=0; i < defined_proc_list_size; i++)
		if (strcmp(procname, defined_proc_list[i]) == 0)
			return(1);
	return(0);
}

/*************************************************\
* 						  *
* While decoding, affectation of initial values   *
* 						  *
\*************************************************/

dcinit(c,l0)
struct comp * c; int l0;
{	int i;
		
	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		if (complist[i].c_type == T_INIT_STR){
			(void)printf("%s\n",complist[i].c_name);
			l0++;
		}else{
			l0 =argprint(&complist[complist[i].c_first_son],0,l0);
			(void)printf(" = ");
			l0 = argprint(&complist[
			complist[complist[i].c_first_son].c_brother],0,l0);
			(void)printf(";\n"); l0++;
		}
	}
	return(l0);
}
