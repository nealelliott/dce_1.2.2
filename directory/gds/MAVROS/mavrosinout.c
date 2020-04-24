/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavrosinout.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:18  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:09:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:08  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:40:05  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:04:32  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:08:38  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:08:21  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:11:54  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:54:56  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:58:29  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  13:03:33  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:22:30  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:34:03  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavrosinout.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:23 $";
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
char * rcsinout_id = "$Header:";
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
generinout()
{       int i, l0=0;

        for (i=0; i < compindex; i++)
 	{       switch (complist[i].c_type){
                case T_TYPE:
                        if (complist[i].c_optional == 0 )
				if ((l0=inout_onetype(&complist[i],l0))==-1)
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
* Generation of input output decoding routines:				     *
* 									     *
* The generation follows basically the same mode than the ASN1 routines.     *
* The following routines are generated:					     *
* 									     *
* 1- Parsing routine:							     *
* 	<comp>_in, for all types.					     *
* 	<comp>_c_in, for decoding "components of sequences",		     *
* 	but not for decoding components of sets: we use the same decoding    *
* 	routine in both cases there!					     *
* 									     *
* 2- Length of output prediction routine:				     *
* 	<comp>_olen, for all types.					     *
* 									     *
* 3- Printing routines:							     *
* 	<comp>_out, for all types.					     *
* 	<comp>_c_out, for components of sequences or sets.		     *
* 									     *
\****************************************************************************/
inout_onetype(c, l0)
struct comp	*c;
int l0;
{	int first_comp, set_comp = 0, setnb = 0;
	int dont_code_out, dont_code_outlen;
	int i;

        if (c->c_call_mode == DEFINE_MODE) return(l0);
#ifdef DEBUG
        if (debuglevel > 7) (void)fprintf(stderr,"Parsing %s\n",c->c_name);
#endif

	storeindex = 0; 
	setnb = 0;
        (void) get_depth(c, 0, &setnb);
        current_list_head = c->c_first_son;
        for (first_comp= c->c_first_son; first_comp != -1; 
	     first_comp = complist[first_comp].c_brother)
                if (complist[first_comp].c_type != T_PARAMETER) break;
        set_comp = look_for_set_comp(first_comp);

	if (is_declared_procedure(c->c_name,IN))
		goto coding_out_proc;

	/* Opening a file for the parsing routines */
	if ((l0 = open_module_file(c->c_name,IN,l0)) < 0)
		return(-1);
        current_call_mode = 1;
        if (c->c_call_mode == SEQUENCE_MODE)
	{	/* Parsing of sequences	*/
		l0 = decl_proc_head(c,l0,C_IN);
		l0 += decl_var_inout(set_comp,storeindex,storelist,setnb,IN);
		for (i= complist[first_comp].c_first_son;
		i!=-1; i=complist[i].c_brother){
			if (complist[i].c_type == T_ALLOC)
				l0 = dcalloc(&complist[i],
				complist[first_comp].c_type,l0);
			if (complist[i].c_type == T_INIT) 
				l0 = dcinit(&complist[i],l0);
		}
		l0 = seqbody_in(&complist[first_comp],l0);
		for (i= complist[first_comp].c_first_son;
		i!=-1; i=complist[i].c_brother){
			if (complist[i].c_type == T_FINAL)
				l0 = dcinit(&complist[i],l0);
		}
		(void)printf("return(asn1z);}\n"); l0++;

		/* parsing the sequence by calling the ad-hoc routine */
		l0 = decl_proc_head(c,l0,IN);
		(void)printf("{\n"); l0++;
		l0 += asn1_open_input();
		l0 = proc_headline(c,l0,C_IN);(void)printf(";\n");
		(void)printf ("while(asn1_inside_bracket(asn1z)){\n");
		(void)printf("asn1z = asn1_skip_input(asn1z,asn1zm);\n");
		(void)printf(
		"asn1z = asn1_skip_input_separator(asn1z,asn1zm);}\n");
		l0 += 4;
		l0 += asn1_close_input();
		(void)printf("return(asn1z);}\n"); l0++;
	}
	else
	{	/* Standard parsing */
		l0 = decl_proc_head(c, l0, IN);
		l0 += decl_var_inout(set_comp,storeindex,storelist,setnb,IN);
		l0 = in_compbis(&complist[first_comp], l0);
		(void)printf("return(asn1z);}\n"); l0++;
	}

coding_out_proc:
	if (c->c_call_mode == HALF_MODE) return(l0);
        current_call_mode = 0;
	dont_code_out = is_declared_procedure(c->c_name,OUT);
	dont_code_outlen = is_declared_procedure(c->c_name,OUTLEN);

	if (dont_code_out && dont_code_outlen)
		return(l0);

	/* opening a file for print out routines */
	if ((l0 = open_module_file(c->c_name,OUT,l0)) < 0)
		return(-1);

	if (dont_code_out)
		goto coding_outlen;

        numberline(l0 = c->c_line_num);
        if (c->c_call_mode == SET_MODE || c->c_call_mode == SEQUENCE_MODE)
	{	/* generating the component print out routine */
		l0 = decl_proc_head(c, l0, C_OUT);
		l0 += decl_var_inout(set_comp,storeindex,storelist,setnb,OUT);
		l0 = seq_out_comp(&complist[first_comp],l0);
		(void)printf("return(asn1z);}\n");
		l0 = decl_proc_head(c, l0, OUT);
		(void)printf("{\n"); l0++;
		(void)printf("ASN1_OPEN_BRACKET(asn1z);\n"); l0++;
		(void)printf("asn1z = ");
		l0 = proc_headline(c,l0,C_OUT); (void)printf(";\n"); l0++;
		(void)printf("ASN1_CLOSE_BRACKET(asn1z);\n"); l0++;
		(void)printf("return(asn1z);}\n");
		l0++;
		l0++;
	}
	else
	{	/* generating the print out routine */
		l0 = decl_proc_head(c, l0, OUT);
		l0 += decl_var_inout(set_comp,storeindex,storelist,setnb,OUT);
		l0 = out_compbis(&complist[first_comp], l0);
		(void)printf("return(asn1z);}\n");
		l0++;
	}
coding_outlen:
        current_call_mode = 0;
	l0 = decl_proc_head(c, l0, OUTLEN);
	l0 += decl_var_inout(set_comp,storeindex,storelist,setnb,OUTLEN);
	l0 = olen_comp(&complist[first_comp], l0, 0);
	(void)printf("return(asn1l);}\n");
	l0++;
	return(l0);
}

/******************************************************************\
* 								   *
* Declaration of the parameters of parsing and printing routines.  *
* 								   *
\******************************************************************/

decl_var_inout(set_comp,stindex,stlist,setnb,type_proc)
int set_comp, stindex, setnb,type_proc;
char ** stlist;
{	int i, l0 = 0;

	(void)printf("{\n"); l0++;
	if (type_proc == IN){
		(void)printf("char * asn1z2;\n"); l0++;
		if (set_comp){
			(void)printf ("char * asn1z_0;\n"); l0++;}
		if (setnb){
			(void)printf("int asn1setc=0;\nchar asn1set[%d];\n",setnb); 
			l0+= 2;}
	}else{	(void)printf("int asn1l = 0;\n"); l0++;}
	if (stindex){
		(void)printf("int");
		for (i=0; i<stindex; i++) 
			(void)printf("%c%s",(i==0)?' ':',',stlist[i]);
		(void)printf(";\n");l0++;
	}
	return(l0);
}

/****************************************************\
*                                                    *
* The main switch for out_length : ``_olen.c''	     *
*                                                    *
\****************************************************/
olen_comp(c, l0, id_needed)
struct comp * c;
int l0, id_needed;
{
	char	* name_of_type;

#ifdef DEBUG
	if (debuglevel > 6)
	{	(void)printf ("/* length of %d (%d:%s) */\n", 
			(c - complist), c->c_type, c->c_name);
		l0++;
		(void)fflush(stdout);
	}
#endif

	if ((name_of_type = c->c_id)==0 && id_needed)
		name_of_type = c->c_name;
	if (name_of_type)
	{	(void)printf("asn1l += %d;\n", 
		strlen(name_of_type) + 4);
		l0++;
	}
	l0 = olen_compbis(c, l0);
#ifdef DEBUG
	if (debuglevel > 6)
		(void)printf ("/* end of length of %d (%d:%s) */\n",
			(c - complist), c->c_type, c->c_name);
#endif
	return(l0);
}

olen_compbis(c, l0)
struct comp 	* c;
int 		l0;
{
	switch(c->c_type){
	case T_EXTERN:
		l0 = cdextern(c,-1,0,l0,OUTLEN);
		break;
	case T_SEQUENCE:
	case T_SET:
		l0 = seq_olen(c,l0); break;
	case T_CHOICE:
		l0 = choice_olen(c,l0); break;
	case T_IGNORE:
		break;
	case T_SET_OF:
	case T_SEQUENCE_OF:
		l0 = array_olen(c,l0); break;
	case T_ANYDEF:
		l0 = cd_anydef(c,l0,OUTLEN); break;
	case T_INTEGER:
	case T_FLAGS:
	case T_OCTETS_ALLOC:
	case T_OCTETS:
	case T_ANY:
	case T_ANY_ALLOC:
	case T_COPY:
	case T_COPY_ALLOC:
	case T_CHARS:
	case T_NULL:
	case T_BITS:
	case T_BITS_ALLOC:
	case T_BOOLEAN:
	case T_REAL:                
	case T_OID:
	case T_OID_ALLOC:
                l0 = cdbasic(c,0,0,l0,OUTLEN,0);
		break;
	default:

		(void)fprintf(stderr,"Unexpected component type: %d.\n"); break;
	}
	return(l0);
}

/****************************************************************************\
* 									     *
* The main switch for get : ``_in.c''				     	     *
* 									     *
\****************************************************************************/

in_comp(c,l0,needed)
struct comp 	* c;
int 		l0;
{	char * name;

	if (needed ||  c->c_id){
		if ((name = c->c_id) == 0) name = c->c_name;
		if (name == 0){
			if (c->c_type != T_CHOICE){
			linenum = c->c_line_num;
			stxerr("Identifier missing for I/O",(char *) 0);}
		}else if (needed) {
			(void)printf (
			"asn1z = asn1_skip_equal_sign(asn1z+%d,asn1zm);\n",
			strlen(name));
		}else{
			(void)printf (
			"asn1z = asn1_skip_if_match(asn1z,asn1zm,\"%s\");\n",
			name);
		}
		l0++;
	}
	l0 = in_compbis(c, l0);
	return(l0);
}

in_compbis(c, l0)
struct comp 	* c;
int 		l0;
{	int i;

	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		if (complist[i].c_type == T_ALLOC)
			l0 = in_alloc(&complist[i],c->c_type,l0);
		if (complist[i].c_type == T_INIT)
			l0 = dcinit(&complist[i],l0);
	}

	switch(c->c_type){
	case T_EXTERN:
		l0 = cdextern(c,-1,0,l0,IN);
		break;
	case T_ANYDEF:
		l0 = cd_anydef(c,l0,IN); break;
	case T_SEQUENCE:
		l0 = seq_in(c,l0); break;
	case T_SET:
		l0 = set_in(c,l0); break;
	case T_CHOICE:
		l0 = choice_in(c,l0); break;
	case T_IGNORE:
		break;
	case T_SET_OF:
	case T_SEQUENCE_OF:
		l0 = array_in(c,l0); break;
	case T_FLAGS:
		l0 = flags_in(c, l0);	 break;
	case T_BITS:
	case T_BITS_ALLOC:
/*		l0 = bits_in(c, l0);	 break; */
	case T_OCTETS_ALLOC:
	case T_OCTETS:
	case T_ANY:
	case T_ANY_ALLOC:
	case T_COPY:
	case T_COPY_ALLOC:
	case T_CHARS:
	case T_OID:
	case T_OID_ALLOC:
                l0 = cdbasic(c,0,0,l0,IN,0); break;
	case T_NULL:
		l0 = null_in(l0); break;
	case T_INTEGER:
                l0 = integer_in(c, l0);     break;
	case T_BOOLEAN:
		l0 = boolean_in(c, l0); break;
	case T_REAL:                
                l0 = real_in(c, l0);     break;
	default:
		(void)fprintf(stderr,"Unexpected component type: %d.\n"); break;
	}
	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		if (complist[i].c_type == T_FINAL)
			l0 = dcinit(&complist[i],l0);
	}
	return(l0);
}

/***************************************************************************\
* 									    *
* Allocation of memory during the parsing:				    *
* 									    *
* The allocation follows the same pattern as during the decoding. However,  *
* the computation of string length or that of element occurences is not	    *
* identical.								    *
* 									    *
* There is no specific routine for the initialisation of variables, as the  *
* treatment is exactly the same as for the ASN-1 decoding.		    *
* 									    *
\***************************************************************************/

in_alloc(c,t,l0)
struct comp * c; int t,l0;
{	int i;

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
		(void)printf("= (%s *) asn1_malloc ((unsigned)(",
		complist[i].c_index);
		switch (t){
		case T_SEQUENCE_OF:
		case T_SET_OF:
			(void)printf(
			"sizeof(%s)*asn1_input_number(asn1z,asn1zm)));\n",
			complist[i].c_index); break;
		default:
			(void)printf("sizeof(%s)));\n",complist[i].c_index); break;
		}
		l0++;
	}
	return(l0);
}

/****************************************************************************\
* 									     *
* The main switch for print : ``_out.c''				     *
* 									     *
* The routine ``outcomp'' will print the type identification, and then call  *
* ``outcompbis'', which will switch to the correct printing routine.	     *
* 									     *
\****************************************************************************/
out_comp(c, l0, id_needed)
struct comp 	* c;
int 		l0;
int		id_needed;
{
	char	* name_of_type;

	if ((name_of_type = c->c_id)==0 && c->c_type != T_CHOICE && id_needed)
		name_of_type = c->c_name;
	if (name_of_type)
	{	(void)printf("strcpy(asn1z,\"%s\"); asn1z += %d;\n",
			name_of_type, strlen(name_of_type)); 
		l0++;
		(void)printf("ASN1_EQUAL_SIGN(asn1z);\n"); l0++;
	}
	l0 = out_compbis(c, l0);
	return(l0);
}

out_compbis(c, l0)
struct comp 	* c;
int 		l0;
{
	switch(c->c_type){
	case T_EXTERN:
		l0 = cdextern(c,-1,0,l0,OUT);
		break;
	case T_ANYDEF:
		l0 = cd_anydef(c,l0,OUT); break;
	case T_SEQUENCE:
	case T_SET:
		l0 = seq_out(c,l0); break;
	case T_CHOICE:
		l0 = choice_out(c,l0); break;
	case T_IGNORE:
		break;
	case T_SET_OF:
	case T_SEQUENCE_OF:
		l0 = array_out(c,l0); break;
	case T_INTEGER:
		l0 = integer_out(c,l0); break;
	case T_FLAGS:
		l0 = flags_out(c,l0); break;
	case T_OCTETS_ALLOC:
	case T_OCTETS:
	case T_ANY:
	case T_ANY_ALLOC:
	case T_COPY:
	case T_COPY_ALLOC:
	case T_NULL:
	case T_BITS:
	case T_BITS_ALLOC:
	case T_CHARS:
	case T_BOOLEAN:
	case T_REAL:                
	case T_OID:
	case T_OID_ALLOC:
                l0 = cdbasic(c,0,0,l0,OUT,0); break;
	default:
		(void)fprintf(stderr,"Unexpected component type: %d.\n"); break;
	}
	return(l0);
}


/****************************************************************************\
* 									     *
* Procedures for the BOOLEAN type:					     *
* 									     *
* The coding and decoding is written in such a way that the BOOLEAN 	     *
* variable can be arbitrarily declared as CHAR, SHORT or any other form of   *
* INT in the application level structures.				     *
* 									     *
* There is no need of an ``output length'' routine, as we can always return  *
* the constant 5 (FALSE is 5 characters).				     *
* 									     *
\****************************************************************************/
boolean_in(c, l0)
struct comp     * c;
int             l0;
{	struct comp * d = &complist[c->c_first_son];

	if (argnumber(c, 1)) return(l0);
	l0 = argprint(d, 0, l0);
	(void)printf(
	"= asn1_boolean_v_in(asn1z,asn1zm,&asn1z2); asn1z = asn1z2;\n");
        l0++;
        return(l0);
}

/****************************************************************************\
* 									     *
* Procedures for the NULL type:						     *
* 									     *
* There is no variable associated to NULL types. We output "NULL", and 	     *
* check that the input value is indeed equal to "NULL".			     *
* 									     *
* There is no need of an ``output length'' routine, as we can always return  *
* the constant 4 (NULL is 4 characters).				     *
* 									     *
\****************************************************************************/
null_in(l0)
int             l0;
{
	(void)printf("asn1z = asn1_null_in(asn1z,asn1zm);\n");
        l0++;
        return(l0);
}

/****************************************************************************\
* 									     *
* Procedures for the INTEGER and ENUMERATED types			     *
* 									     *
* Integers can have arbitrary types: int, short, char, etc.		     *
* They can optionally take their values in ``definition lists''.	     *
* We assume that the definition lists have been stored in tables, named by   *
* the name of the module + the sequence number of the component; the tables  *
* hold tuples of the form : int, value.					     *
* 									     *
\****************************************************************************/
integer_in(c, l0)
struct comp	* c;
int		l0;
{	int i, first = firstarg(c), at_least_one_value = 0;

	if (argnumber(c, 1)) return(l0);
	for (i=c->c_first_son; i != -1; i=complist[i].c_brother){
		if (complist[i].c_type != T_VALUE) continue;
		at_least_one_value = 1;
		printf ("if (asn1_compare_name(asn1z,\"%s\")){\n",
		complist[i].c_name);
		l0 = argprint(&complist[first], 0, l0+1);
		printf (" = %s; asn1z += %d;}\nelse ", 
		complist[i].c_index, strlen(complist[i].c_name));
		l0++;
	}
	if (at_least_one_value) (void) PUTC('{',stdout);
	l0 = argprint(&complist[first], 0, l0);
	(void)printf("= strtol(asn1z,&asn1z2,0); asn1z = asn1z2;%s\n",
	(at_least_one_value)?"}":"");
	l0++;
	return(l0);
}

integer_out(c, l0)
struct comp	* c;
int		l0;
{	int i, first = firstarg(c);

	if (argnumber(c, 1)) return(l0);
	for (i=c->c_first_son; i != -1; i=complist[i].c_brother){
		if (complist[i].c_type != T_VALUE) continue;
		printf ("if (");
		l0 = argprint(&complist[first], 0, l0);
		printf (
		" == %s){\nstrcpy(asn1z,\"%s\"); asn1z += %d;}\nelse ",
		complist[i].c_index, complist[i].c_name,
		strlen(complist[i].c_name));
		l0 += 2;
	}
	l0 = cdbasic(c,0,0,l0,OUT,0);
	return(l0);
}

/***********************************************************\
* 							    *
* Procedures for the FLAGS:				    *
* 							    *
* These short arrays of bits should be printed as lists of  *
* flags.						    *
* 							    *
\***********************************************************/
flags_in(c, l0)
struct comp	* c;
int		l0;
{	int i, found_a_value = 0;

	if (argnumber(c, 2)) return(l0);
	for (i=c->c_first_son; i != -1; i=complist[i].c_brother){
		if (complist[i].c_type != T_VALUE) continue;
		if (found_a_value == 0){
			printf ("if (*asn1z == ASN1_OPENING_BRACKET){\n");
			l0 = argprint(&complist[firstarg(c)], 0, l0+1);
			printf ("=0;\n"); l0++;
			l0 += asn1_open_input();
			printf("while(asn1_inside_bracket(asn1z)){\n");
			l0 += 2;
			found_a_value = 1;
		}
		printf ("if (asn1_compare_name(asn1z,\"%s\")){\n",
		complist[i].c_name);
		l0 = argprint(&complist[firstarg(c)], 0, l0);
		printf (" |= %s; asn1z+= %d;}\n else ",
		complist[i].c_name, strlen(complist[i].c_name));
		l0 ++;
	}
	if (found_a_value){
		printf (
		"{ asn1z = asn1_err_in(asn1z,ASN1_ERR_FLAGS);break;}\n");
		printf (
		"asn1z = asn1_skip_input_separator(asn1z,asn1zm);}\n");
		l0 += 2 + asn1_close_input();
		printf("}\nelse "); l0++;
	}
	l0 = cdbasic(c,0,0,l0,IN,0);
	return(l0);
}

flags_out(c, l0)
struct comp	* c;
int		l0;
{	int i, found_a_value = 0;

	if (argnumber(c, 2)) return(l0);
	for (i=c->c_first_son; i != -1; i=complist[i].c_brother){
		if (complist[i].c_type != T_VALUE) continue;
		if (found_a_value){
			printf ("*asn1z++ = ASN1_SEPARATOR;}\n");
		}else{	printf ("*asn1z++ = ASN1_OPENING_BRACKET;\n");
			found_a_value = 1;
		}
		l0++;
		printf ("if (");
		l0 = argprint(&complist[firstarg(c)], 0, l0);
		printf (
		" & %s){\nstrcpy(asn1z,\"%s\"); asn1z += %d;\n",
		complist[i].c_name, complist[i].c_name,
		strlen(complist[i].c_name));
		l0 += 2;
	}
	if (found_a_value){
		printf ("}\n*asn1z++ = ASN1_CLOSING_BRACKET;\n");
		l0 += 2;
	}else	l0 = cdbasic(c,0,0,l0,OUT,0);
	return(l0);
}

/*******\
*       *
* REAL  *
*       *
\*******/
real_in(c, l0)
struct comp	* c;
int		l0;
{	
	if (argnumber(c, 1)) return(l0);
	l0 = argprint(&complist[firstarg(c)], 0, l0);
	(void)printf("= asn1_real_v_in(asn1z,asn1zm,&asn1z2); asn1z = asn1z2;\n");
	l0++;
	return(l0);
}

/***************************************************************************\
* 									    *
* SEQUENCE								    *
* 									    *
* The handling of sequences includes a ``parenthesing'' routine and a	    *
* ``component handler''. The printing is common to SEQUENCES and SETs.	    *
* 									    *
* The decoding is less straightforward. After opening the parenthesis, we   *
* will call the component decoding routine. This routine will:		    *
* 									    *
* 1- Search for a ``type <equal>'' component; try to match the type	    *
* indication with the current ``looked for'' type. If it does not match,    *
* then if the type is mandatory an error will be returned (component	    *
* missing).								    *
* 									    *
* 2- If no type was indicated, then assume that the element is the ``next   *
* in sequence''.							    *
* 									    *
* 3- Decode the value. The decoding may return a null pointer if an error   *
* was detected -- take care!						    *
* 									    *
* 4- Look for either a separator between values or a closing parenthesis.   *
* 									    *
* Attention should be paid to the handling of the ``components of''	    *
* structures.								    *
* 									    *
\***************************************************************************/
seq_out(c,l0)
struct comp * c;
int l0;
{
	(void)printf("ASN1_OPEN_BRACKET(asn1z);\n"); l0++;
	l0 = seq_out_comp(c,l0);
	(void)printf("ASN1_CLOSE_BRACKET(asn1z);\n"); l0++;
	return(l0);
}

seq_out_comp(c,l0)
struct comp * c;
int l0;
{	int i, t = 0, not_first = 0;

	for (i=c->c_first_son; i != -1; i=complist[i].c_brother){
		if (complist[i].c_type == T_IGNORE)
			continue;
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;

		if (not_first){
			(void)printf("ASN1_SEPARATE(asn1z);\n");
			l0++;
			if (t){
				(void)printf("}\n"); l0++;}
		}else	not_first = 1;

		if (complist[i].c_type == T_COMP){
			l0 = cdextern(&complist[i],-1,0,l0,C_OUT);
			t = 0;
		}else{
			l0 = defaulttest(&complist[i],l0,&t,0);
			l0 = out_comp(&complist[i],l0,
			complist[i].c_optional || c->c_type == T_SET);
		}
	}
	if (not_first && t){
		(void)printf("}\n"); l0++;}
	return(l0);
}

seq_olen(c,l0)
struct comp * c;
int l0;
{	int i,t;

#ifdef DEBUG
	if (debuglevel > 4)
	{	(void)printf("/* Start of STRUCT_olen */\n"); 
		l0++;
		(void)fflush (stdout);
	}
#endif
	(void)printf("asn1l += ASN1_BRACKET_OP_LEN;\n");
	l0++;
	for (i=c->c_first_son; i != -1; i = complist[i].c_brother)
	{	if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (i != c->c_first_son){
			(void)printf("asn1l += ASN1_SEPARATE_LEN;\n"); 
			l0++;
		}
		if (complist[i].c_type == T_COMP){
			l0 = cdextern(&complist[i],-1,0,l0,OUTLEN);
			continue;
		}
		l0 = defaulttest(&complist[i],l0,&t,0);
		l0 = olen_comp(&complist[i],l0, c->c_optional || (c->c_type == T_SET));
		if (t)
		{	(void)printf("};\n"); 
			l0++;
		}
	}
	(void)printf("asn1l += ASN1_BRACKET_CL_LEN;\n"); 
#ifdef DEBUG
	if (debuglevel > 4)
	{	(void)printf("/* End of STRUCT_olen */\n"); 
		l0++;
		(void)fflush (stdout);
	}
#endif
	return(l0);
}


seqbody_in(c,l0)
struct comp * c;
int l0;
{	int i,j,needed;

	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP){
			l0 = cdextern(&complist[i],-1,0,l0,C_IN);
			continue;
		}
		(void)printf("if (asn1_inside_bracket(asn1z)");
		if (needed = (complist[i].c_type == T_IGNORE
		|| complist[i].c_optional)){
			(void)printf(" &&\n"); l0++;
			l0 = comp_input_test(&complist[i],l0);
		}
		(void)printf("){\n"); l0++;
		l0 = in_comp(&complist[i],l0,needed);
		(void)printf("asn1z = asn1_skip_input_separator(asn1z,asn1zm);\n"); 
		l0++;
		(void)printf("}\n"); l0++;
		if (complist[i].c_type == T_IGNORE) continue;
		if (complist[i].c_optional){
			for (j = complist[i].c_first_son; j != -1; 
			j = complist[j].c_brother)
				if (complist[j].c_type == T_DEFAULT) break;
			if (j == -1) continue;
			(void)printf("else {");
			for (;j!=-1;j=complist[j].c_brother)
				l0 = affectprint(&complist[j],l0,0);
			(void)printf("}\n"); l0++;
		}else{	(void)printf(
			"else { asn1z = asn1_err_in(asn1z,ASN1_ERR_MISS);\n");
			l0++;
			l0 = errcomp(&complist[i],l0);
			(void)printf("}\n"); l0++;
		}
	}
	return(l0);
}

seq_in(c,l0)
struct comp * c;
int l0;
{
	l0 += asn1_open_input();

	l0 = seqbody_in(c,l0);

	(void)printf ("while(asn1_inside_bracket(asn1z)){\n"); l0++;
	(void)printf("asn1z = asn1_skip_input(asn1z,asn1zm);\n");l0++;
	(void)printf("asn1z = asn1_skip_input_separator(asn1z,asn1zm);}\n"); l0++;
	l0 += asn1_close_input();
	return(l0);
}

/***************************************************************************\
* 									    *
* SETS									    *
* 									    *
* The coding is the same as that of SEQUENCEs.				    *
* 									    *
* The decoding is more difficult, as components may appear in any order +   *
* can be referred to in ``component of'' structures. One must indeed test   *
* that components dont appear twice, and that mandatory components are all  *
* presents...								    *
* 									    *
\***************************************************************************/
set_in(c, l0)
struct comp 	* c;
int 		l0;
{	int i, j, first=1, comp = 0, icomp = 0;

	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP){
			(void)printf("if(asn1z){ asn1z_0 = asn1z;\n"); l0++;
			l0 = cdextern(&complist[i],-1,0,l0,IN);
			(void)printf("if (asn1z) asn1z = asn1z_0;} else\n"); l0++;
			l0 = cdextern(&complist[i],-1,0,l0,ERRCOD);
			(void)printf(";\n"); l0++;
			continue;
		}else	comp++;
	}
	(void)printf("ASN1_ZERO(asn1set+asn1setc,%d); asn1setc += %d;\n", 
	comp, comp);
	l0 += asn1_open_input();
	(void)printf ("while(asn1_inside_bracket(asn1z)){\n"); l0++;
	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP) continue;
		if (first)
			first = 0;
		else	(void)printf(" else ");
		(void)printf("if((asn1set[asn1setc - %d] == 0)&&(\n", comp - icomp);
		l0 = comp_input_test(&complist[i],l0);
			(void)printf(")){\n"); l0++;
		(void)printf("asn1set[asn1setc - %d] = 1;\n", comp - icomp);
		l0 = in_comp(&complist[i],l0,1);
		(void)printf("}");
		icomp++;
	}
	(void)printf("else asn1z = asn1_skip_input(asn1z,asn1zm);\n");l0++;
	(void)printf("asn1z = asn1_skip_input_separator(asn1z,asn1zm);}\n"); l0++;
	(void)printf("asn1setc -= %d;\n",comp);
	for (i=c->c_first_son, icomp= -1; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP) continue;
		icomp++;
		if (complist[i].c_type == T_IGNORE) continue;
		if (complist[i].c_optional){
			for (j = complist[i].c_first_son; j != -1; j = complist[j].c_brother)
				if (complist[j].c_type == T_DEFAULT) break;
			if (j == -1) continue;
		}
		(void)printf("if (asn1set[asn1setc + %d] == 0){\n", icomp);
		if (complist[i].c_optional)
			for (;j != -1;j=complist[j].c_brother)
				l0 = affectprint(&complist[j],l0,0);
		else{	(void)printf(
			"asn1z=asn1_err_in(asn1z,ASN1_ERR_MISS);\n");
			l0++;
			l0 = errcomp(&complist[i],l0);
		}
		(void)printf("}\n"); l0++;
	}
	l0 += asn1_close_input();
	return(l0);
}

/*********************************************************************\
* 								      *
* SEQUENCE OF, SET OF						      *
* 								      *
* This components are encoded as a sequence of values, without tags.  *
* 								      *
\*********************************************************************/
int array_out(c, l0)
struct comp 	* c;
int 		l0;
{
	(void)printf("ASN1_OPEN_BRACKET(asn1z);\n"); 
	l0++;
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	(void)printf("for (%s=0; %s <", c->c_name, c->c_name);
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf("; %s++){\n", c->c_name);
	(void)printf("if (%s) ASN1_SEPARATE(asn1z);\n",c->c_name); l0++;
	l0++;
	l0 = out_compbis(&complist[c->c_first_son],l0);
	(void)printf("}\n"); 
	l0++;
	(void)printf("ASN1_CLOSE_BRACKET(asn1z);\n"); 
	l0++;
	return(l0);	
}

int array_olen(c, l0)
struct comp 	* c;
int 		l0;
{
	if (debuglevel > 4)
	{ 	(void)printf("/* Start of ARRAY_olen */\n"); 
		l0++;
		(void)fflush(stdout);
	}

	(void)printf("asn1l += ASN1_BRACKET_OP_LEN;\n");
	(void)printf("asn1l += ASN1_SEPARATE_LEN *");
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	PUTC(';',stdout);
	(void)printf("for (%s=0; %s <",c->c_name,c->c_name);
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf("; %s++){\n",c->c_name); 
	l0++;
	l0 = olen_comp(&complist[c->c_first_son],l0,0);
	(void)printf("}\n"); 
	l0++;
	(void)printf("asn1l += ASN1_BRACKET_CL_LEN;\n");

	if (debuglevel > 4)
	{ 	(void)printf("/* End of ARRAY_olen */\n"); 
		l0++;
		(void)fflush(stdout);
	}

	return(l0);
}

int array_in(c, l0)
struct comp 	* c;
int 		l0;
{
	l0 += asn1_open_input();
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	(void)printf ("%s=0; while(asn1_inside_bracket(asn1z)){\n",
	c->c_name);
	l0++;
	l0 = in_comp(&complist[c->c_first_son],l0,0);
	(void)printf("asn1z = asn1_skip_input_separator(asn1z,asn1zm);\n"); 
	l0++;
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	(void)printf("%s++;}",c->c_name);
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf("= %s;\n",c->c_name); 
	l0++;
	l0 += asn1_close_input();
	return(l0);
}

/********************************************************\
* 							 *
* CHOICES						 *
* 							 *
* Are encoded as the selected component, between braces	 *
* 							 *
\********************************************************/
int choice_out(c, l0)
struct comp 	* c;
int 		l0;
{	int 	i, openchoice = 1;

	if (c->c_id != 0 || complist[c->c_father].c_type == T_TYPE)
		(void)printf("ASN1_OPEN_BRACKET_CHOICE(asn1z);\n"); l0++;
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	(void)printf("switch (CAST_FOR_SWITCH(");
	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf(")){\n");
	l0++;
	for (i=complist[c->c_first_son].c_brother; 
	i != -1; i = complist[i].c_brother)
	{	if( complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (l0 != complist[i].c_line_num)
			numberline(l0 = complist[i].c_line_num);
		if (complist[i].c_index == 0)
			(void)printf("default: ");
		else	(void)printf("case %s: ",complist[i].c_index);
		l0 = out_comp(&complist[i],l0,1);
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
	if (c->c_id != 0 || complist[c->c_father].c_type == T_TYPE)
		(void)printf("ASN1_CLOSE_BRACKET(asn1z);\n"); l0++;
	if (debuglevel > 4)
	{	(void)printf("/* End of CHOICE */\n"); 
		l0++;
		(void)fflush(stdout);
	}
	return(l0);
}

int choice_olen(c, l0)
struct comp 	* c;
int 		l0;
{	int i, openchoice = 1;

	(void)printf("asn1l += ASN1_BRACKET_LEN;\n");
	l0++;

	(void)printf("switch (CAST_FOR_SWITCH(");
	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf(")){\n"); 
	l0++;
	for (i=complist[c->c_first_son].c_brother;
	i != -1; i = complist[i].c_brother)
	{	if( complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (l0 != complist[i].c_line_num)
			numberline(l0 = complist[i].c_line_num);
		if (complist[i].c_index == 0)
			(void)printf("default: ");
		else	(void)printf("case %s: ",complist[i].c_index);
		l0 = olen_comp(&complist[i],l0,1);
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

int choice_in(c, l0)
struct comp 	* c;
int 		l0;
{	int 	i;

	if (c->c_id != 0 || complist[c->c_father].c_type == T_TYPE)
		l0 += asn1_open_input();
	for (i=complist[c->c_first_son].c_brother; 
	i != -1; i = complist[i].c_brother)
	{	if( complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_ARGUMENT) continue;

		(void)printf("if(");
		l0 = comp_input_test(&complist[i],l0);
		(void)printf("){\n"); l0++;
		l0 = in_comp(&complist[i],l0,1);
		if (complist[i].c_index != 0){
			l0 = argprint(&complist[c->c_first_son],0,l0);
			(void)printf(" = %s;",complist[i].c_index);
		}
		(void)printf("}else \n"); 
		l0++;
	}
	(void)printf("{ asn1z = asn1_err_in(asn1z,ASN1_ERR_CHOICE);\n"); 
	l0++;
	l0 = errcomp(c,l0);
	(void)printf("}\n"); 
	l0++;
	if (c->c_id != 0 || complist[c->c_father].c_type == T_TYPE)
		l0 += asn1_close_input();
	return(l0);
}

/****************************************************************************\
* 									     *
* Input of optional components:						     *
* 									     *
* Optional components within sequences, as well as components of sets or     *
* alternative within choices, must be recognized. The general structure is:  *
* 									     *
* If component is present, decode it and set a variable (char *) to some     *
* identifying value. Which yelds:					     *
* 									     *
* 	1- For single components:					     *
* 		Test the ID; 						     *
* 		if it matches:						     *
* 			open the component, decode it.			     *
* 	2- For <unqualified choices>:					     *
* 		For each component,					     *
* 			test the ID;					     *
* 			if it matches, open and decode.			     *
* 			if not, pass to the next one.			     *
* 									     *
\****************************************************************************/
int choice_input_test(c, l0)
struct comp 	* c;
int 		l0;
{	int 	i, first = 1;

	for (i=complist[c->c_first_son].c_brother; i != -1; i = complist[i].c_brother)
	{	if( complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_ARGUMENT) continue;

		if (first){
			(void)printf("("); first = 0;}
		else{	(void)printf("\n ||"); l0++;}
		l0=comp_input_test(&complist[i],l0);
	}
	(void)printf(")\n"); 
	l0++;
	return(l0);
}

int comp_input_test(c, l0)
struct comp 	* c;
int 		l0;
{	char 	* name;


	if ((name = c->c_id) == 0)
	{	if (c->c_type == T_CHOICE)
			return (choice_input_test(c,l0));
		else	name = c->c_name;
	}
	(void)printf("asn1_compare_name(asn1z,\"%s\")\n",name);
	return(l0);
}

/***************************************\
* 				        *
* Opening and closing of coding blocks  *
* 				        *
\***************************************/
asn1_open_input()
{
	(void)printf("asn1z = asn1_open_input(asn1z,asn1zm);\n");
	return(1);
}

asn1_close_input()
{
	(void)printf("asn1z = asn1_close_input(asn1z,asn1zm);\n");
	return(1);
}
