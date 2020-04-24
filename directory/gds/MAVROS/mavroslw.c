/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavroslw.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:19  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:09:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:10  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:40:19  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:04:45  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:10:05  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:08:40  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:12:05  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:55:01  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:22:37  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:34:11  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavroslw.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:24 $";
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
extern int debuglevel;
extern int compindex;
extern struct comp * complist;

/************************************\
* 				     *
*  Procedures for the INTEGER type   *
* 				     *
\************************************/

integercodlw(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,1)) return(l0);
	(void)printf("ASN1_INTLW(asn1z,");
	l0 = argprint(&complist[firstarg(c)],0,l0);
	(void)printf ("); asn1z += WORD;\n"); l0++;
	return(l0);	
}

integerdeclw(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,1)) return(l0);

	l0 = argprint(&complist[firstarg(c)],0,l0);
	(void)printf(" = ASN1_LWINT(asn1z); asn1z += WORD;\n"); l0++;
	return(l0);	
}

/*********************************\
* 				  *
*  Procedures for the REAL type   *
* 				  *
\*********************************/

realcodlw(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,1)) return(l0);
	(void)printf("ASN1_REALLW(asn1z,");
	l0 = argprint(&complist[firstarg(c)],0,l0);
	(void)printf ("); asn1z += REALWORD;\n"); l0++;
	return(l0);	
}

realdeclw(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,1)) return(l0);

	l0 = argprint(&complist[firstarg(c)],0,l0);
	(void)printf(" = ASN1_LWREAL(asn1z); asn1z += REALWORD;\n"); l0++;
	return(l0);	
}

/******************************************\
* 					   *
* Procedures for coding the EXTERNAL type  *
* 					   *
\******************************************/

externlwc(c,l0)
struct comp * c;
int l0;
{	int i;

	if (argnumber(c,2)) return(l0);
	(void)printf("asn1z = asn1_external_lwc(asn1z,");
	l0 = argprint(&complist[i=firstarg(c)],0,l0);
	PUTC(',',stdout);
	l0 = argprint(&complist[complist[i].c_brother],0,l0);
	(void)printf(");\n"); l0++;
	return(l0);	
}

externlwd(c,l0)
struct comp * c;
int l0;
{	int i;

	if (argnumber(c,2)) return(l0);
	(void)printf("asn1z = asn1_external_lwd(asn1z,asn1zm,");
	l0 = argprint(&complist[i=firstarg(c)],0,l0);
	PUTC(',',stdout);
	l0 = argprint(&complist[complist[i].c_brother],0,l0);
	(void)printf(");\n"); l0++;
	return(l0);
}

/*****************************************************\
* 						      *
*  Procedures for coding and decoding octet strings   *
* 						      *
\*****************************************************/

octetscodlw(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,2)) return(l0);
        (void)printf("asn1l =");
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
        (void)printf(";ASN1_INTLW(asn1z,asn1l); asn1z += WORD;\n");
	(void)printf ("ASN1_MOVE (");
	l0 = argprint(&complist[c->c_first_son],0,l0);	
	(void)printf(", asn1z, asn1l); asn1z += asn1l;\n"); l0++;
#ifdef ALIGNMENT
	(void)printf (" if (asn1l = (int) asn1z & (WORD -1)) asn1z += WORD - asn1l;\n");
	l0++;
#endif
	return(l0);	
}

octetsdeclw(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,2)) return(l0);
        l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
        (void)printf(" = asn1l = ASN1_LWINT(asn1z); asn1z += WORD;\n"); l0++;
	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf (" = asn1z; asn1z += asn1l;\n"); l0++;
#ifdef ALIGNMENT
        (void)printf (" if (asn1l = (int) asn1z & (WORD -1)) asn1z += WORD - asn1l;\n");
        l0++;
#endif
	return(l0);
}

octetsalldeclw(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,2)) return(l0);

        l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
        (void)printf(" = asn1l = ASN1_LWINT(asn1z); asn1z += WORD;\n"); l0++;
        (void)printf ("ASN1_MOVE (asn1z, ");
        l0 = argprint(&complist[c->c_first_son],0,l0);
        (void)printf(", asn1l); asn1z += asn1l;\n");l0++;
#ifdef ALIGNMENT
        (void)printf (" if (asn1l = (int) asn1z & (WORD -1)) asn1z += WORD - asn1l;\n");
        l0++;
#endif
	return(l0);
}

/***************************************************************\
* 							        *
*  Procedures for coding and decoding null terminated strings   *
* 							        *
\***************************************************************/
charscodlw(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,1)) return(l0);
        (void)printf("asn1l = strlen (");
        l0 = argprint(&complist[c->c_first_son],0,l0);
        (void)printf(");ASN1_INTLW(asn1z,asn1l); asn1z += WORD;\n"); l0++;
        (void)printf ("ASN1_MOVE (");
        l0 = argprint(&complist[c->c_first_son],0,l0);
        (void)printf (", asn1z, asn1l); asn1z += asn1l;\n"); l0++;
#ifdef ALIGNMENT
        (void)printf (" if (asn1l = (int) asn1z & (WORD -1)) asn1z += WORD - asn1l;\n");
        l0++;
#endif
	return(l0);
}

charsdeclw(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,1)) return(l0);
        (void)printf("asn1l = ASN1_LWINT(asn1z); asn1z += WORD;\n");l0++;
        (void)printf ("ASN1_MOVE (asn1z, ");
	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf(", asn1l);\n"); l0++;
	(void)printf("(");
        l0 = argprint(&complist[c->c_first_son],0,l0);
        (void)printf (")[asn1l] = 0;");
	(void)printf("asn1z += asn1l;\n"); l0++;
#ifdef ALIGNMENT
        (void)printf (" if (asn1l = (int) asn1z & (WORD -1)) asn1z += WORD - asn1l;\n");
	l0++;
#endif
	return(l0);
}

/***************************************************\
* 						    *
*  Procedures for coding and decoding bit strings   *
* 						    *
\***************************************************/

bitcodlw(c,l0)
struct comp * c;
int l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);

        (void)printf("asn1l = ");
        l0 = argprint(&complist[complist[i].c_brother],0,l0);
        (void)printf(";ASN1_INTLW(asn1z,asn1l); asn1z += WORD;");
	(void)printf ("asn1l = ((asn1l + 7) >> 3);\n"); l0++;
        (void)printf ("ASN1_MOVE (");
        l0 = argprint(&complist[i],0,l0);
        (void)printf(", asn1z, asn1l); asn1z += asn1l;\n");l0++;
#ifdef ALIGNMENT
        (void)printf (" if (asn1l = (int) asn1z & (WORD -1)) asn1z += WORD - asn1l;\n");
	l0++;
#endif
	return(l0);	
}

bitdeclw(c,l0)
struct comp * c;
int l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);

        l0 = argprint(&complist[complist[i].c_brother],0,l0);
        (void)printf(" = asn1l = ASN1_LWINT(asn1z); asn1z += WORD;\n"); l0++;
        l0 = argprint(&complist[i],0,l0);
        (void)printf ("= asn1z; asn1z += ((asn1l + 7) >> 3);\n"); l0++;
#ifdef ALIGNMENT
        (void)printf (" if (asn1l = (int) asn1z & (WORD -1)) asn1z += WORD - asn1l;\n");
	l0++;
#endif
	return(l0);
}

bitalldeclw(c,l0)
struct comp * c;
int l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);

        l0 = argprint(&complist[complist[i].c_brother],0,l0);
        (void)printf(" = asn1l = ASN1_LWINT(asn1z); asn1z += WORD;\n"); l0++;
	(void)printf ("asn1l = ((asn1l +7) >> 3);\n"); l0++;
	(void)printf ("ASN1_MOVE (asn1z, ");
        l0 = argprint(&complist[i],0,l0);
        (void)printf(", asn1l); asn1z += asn1l;\n"); l0++;
#ifdef ALIGNMENT
        (void)printf (" if (asn1l = (int) asn1z & (WORD -1)) asn1z += WORD - asn1l;\n");
        l0++;
#endif
	return(l0);
}

/*******************************************\
* 					    *
* Procedures for coding and decoding flags  *
* 					    *
\*******************************************/

flagcodlw(c,l0)
struct comp * c;
int l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);

        (void)printf("asn1z = asn1_flags_lwc(asn1z,");
        l0 = argprint(&complist[i],0,l0);
	(void)printf(",");
        l0 = argprint(&complist[complist[i].c_brother],0,l0);
	(void)printf(");\n");
#ifdef ALIGNMENT
        (void)printf (" if (asn1l = (int) asn1z & (WORD -1)) asn1z += WORD - asn1l;\n");
	l0++;
#endif
	return(l0);	
}

flagdeclw(c,l0)
struct comp * c;
int l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);
        (void)printf("asn1l = ASN1_LWINT(asn1z); asn1z += WORD;\n"); l0++;
	(void)printf ("asn1l = ((asn1l +7) >> 3);\n"); l0++;
	(void)printf ("ASN1_MOVE (asn1z,");
        l0 = argprint(&complist[i],1,l0);
        (void)printf(",(asn1l>WORD)?WORD:asn1l); asn1z += asn1l;\n"); l0++;
#ifdef ALIGNMENT
        (void)printf ("if(asn1l=(int) asn1z & (WORD -1)) asn1z += WORD - asn1l;\n");
        l0++;
#endif
	return(l0);
}

/**************************************\
* 				       *
*  Procedures for SEQUENCEs and SETs   *
* 				       *
\**************************************/

seqcodlw(c,l0)
struct comp * c;
int l0;
{	int i,t;

	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP){
			l0 = cdextern(&complist[i],-1,0,l0,LWCOD);
			continue;
		}
		if (complist[i].c_type == T_IGNORE){
			(void)printf("ASN1_LW0(asn1z);");l0++;
			continue;
		}
		l0 = defaulttest(&complist[i],l0,&t,0);
		if (t || complist[i].c_optional){
			(void)printf(
			"{ asn1 asn1z2=asn1z; asn1z += WORD;\n");
			l0++;
		}
		l0 = cdcomplw(&complist[i],l0);
		if (t || complist[i].c_optional){
			(void)printf("asn1l = (asn1z - asn1z2) - WORD;");
			(void)printf(
			"ASN1_INTLW(asn1z2, asn1l);}\n"); 
			l0++;
		}
		if (t == 1){
			(void)printf ("} else { ASN1_LW0(asn1z);}");l0++;
		}
	}
	return(l0);
}

seqdeclw(c,l0)
struct comp * c;
int l0;
{	int i, t;

	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP){
			l0 = cdextern(&complist[i],-1,0,l0,LWDEC);
			continue;
		}
		if (complist[i].c_type == T_IGNORE){
			(void)printf("asn1l = ASN1_LWINT(asn1z);");
			(void)printf("asn1z += WORD + asn1l;\n"); l0++;
			continue;
		}
		/* "complist[i].o" is true either because 
		 *	OPTIONAL or DEFAULT has been used in the 
		 * definition.
		 */
		if (complist[i].c_optional){
			(void)printf("asn1l =ASN1_LWINT(asn1z);asn1z += WORD;\n"); 
			l0++;
			(void)printf("if (asn1l) {\n"); l0++;
		}

		l0 = dccomplw(&complist[i],l0);

		if (complist[i].c_optional)
		{	l0 = defaulttest(&complist[i],l0,&t,1);
			(void)printf("}\n"); l0++;
		}
	}

	return(l0);
}

/**************************\
* 			   *
*  Procedures for CHOICE   *
* 			   *
\**************************/

choicedeclw(c,l0)
struct comp * c;
int l0;
{	int i;
	int positiontag = 0;

        (void)printf("asn1l = ASN1_LWINT(asn1z); asn1z += WORD;\n"); l0++;

	(void)printf ("switch (asn1l) {\n");

	for (i=complist[c->c_first_son].c_brother; 
	i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;

		(void)printf ("case %d:\n", positiontag++);
		l0 = dccomplw(&complist[i],l0);
		if (complist[i].c_index != 0){
			l0 = argprint(&complist[c->c_first_son],0,l0);
			(void)printf(" = %s;",complist[i].c_index);
		}
		(void)printf ("break;\n");
	}
	(void)printf("}\n");l0++;
	return(l0);
}

choicecodlw(c,l0)
struct comp * c;
int l0;
{	int i, openchoice = 1;
	int positiontag = 0;

	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	(void)printf("switch (CAST_FOR_SWITCH(");
	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf(")){\n"); l0++;
	for (i=complist[c->c_first_son].c_brother; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (l0 != complist[i].c_line_num)
			numberline(l0 = complist[i].c_line_num);
		if (complist[i].c_index == 0)
			(void)printf("default: ");
		else	(void)printf("case %s: ",complist[i].c_index);
		(void)printf ("ASN1_INTLW(asn1z,%d); asn1z += WORD;\n",
		positiontag++); l0++;
		l0 = cdcomplw(&complist[i],l0);
		if (complist[i].c_index == 0){
			openchoice = 0;
			(void)printf("break;}\n"); l0++; return(l0);
		}else{	(void)printf("break;\n"); l0++;}
	}
	if (openchoice){	(void)printf("default:break;}\n"); l0++;}

	return(l0);
}

/******************************************\
* 					   *
*  procedures for SET OF and SEQUENCE of   *
* 					   *
\******************************************/

arraycodlw(c,l0)
struct comp * c;
int l0;
{
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);

	(void)printf (" ASN1_INTLW(asn1z,");
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf(");asn1z += WORD;\n");
		
	(void)printf("for (%s=0; %s <",c->c_name,c->c_name);
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf("; %s++){\n",c->c_name); l0++;
	l0 = cdcomplw(&complist[c->c_first_son],l0);
	(void)printf("}\n"); l0++;

	return(l0);	
}

arraydeclw(c,l0)
struct comp * c;
int l0;
{
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);

        l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
        (void)printf(" = ASN1_LWINT(asn1z); asn1z += WORD;\n");
	(void)printf ("for (%s=0; %s<", c->c_name, c->c_name);
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf (";%s++){\n",c->c_name);
	l0 = dccomplw(&complist[c->c_first_son],l0);
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	(void)printf("}\n");
	return(l0);
}

/************************************\
* 				     *
*  Procedures for the BOOLEAN type   *
* 				     *
\************************************/

booleancodlw(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,1)) return(l0);
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);

	(void)printf("*asn1z++ = (");
	l0 = argprint(&complist[firstarg(c)],0,l0);
	(void)printf(")?1:0;\n");l0++;
#ifdef ALIGNMENT
	(void)printf ("asn1z += WORD -1;"); l0++;
#endif
	return(l0);	
}

booleandeclw(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,1)) return(l0);
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);

	l0 = argprint(&complist[firstarg(c)],0,l0);
#ifdef ALIGNMENT
	(void)printf(" = *asn1z++; asn1z += WORD -1;\n"); l0++;
#else
	(void)printf(" = *asn1z++;\n"); l0++;
#endif
	return(l0);
}

/***************************************************\
* 						    *
*  The main switch for the encoding of ``_cod.c''   *
* 						    *
\***************************************************/

cdcomplw(c,l0)
struct comp * c;
int l0;
{
	switch(c->c_type){
	case T_OCTETS_ALLOC:
	case T_OCTETS:
	case T_OID_ALLOC:
	case T_OID:
	case T_ANY:
	case T_ANY_ALLOC:
	case T_COPY:
        case T_COPY_ALLOC:
		l0 = octetscodlw(c,l0); break;
	case T_ANYDEF:
		l0 = cd_anydef(c,l0,LWCOD); break;
	case T_FLAGS:
		l0 = flagcodlw(c,l0); break;
	case T_BITS:
	case T_BITS_ALLOC:
		l0 = bitcodlw(c,l0); break;
	case T_CHARS:
		l0 = charscodlw(c,l0); break;
	case T_INTEGER:
		l0 = integercodlw(c,l0); break;
	case T_EXTERN:
		l0 = cdextern(c,-1,0,l0,LWCOD); break;
	case T_SEQUENCE:
	case T_SET:
		l0 = seqcodlw(c,l0); break;
	case T_CHOICE:
		l0 = choicecodlw(c,l0); break;
	case T_IGNORE:
		(void)printf("ASN1_LW0(asn1z);\n"); l0++;
	case T_SEQUENCE_OF:
	case T_SET_OF:
		l0 = arraycodlw(c,l0); break;
	case T_BOOLEAN:
		l0 = booleancodlw(c,l0); break;
	case T_REAL:
		l0 = realcodlw(c,l0); break;
	case T_EXTERNAL:
		l0 = externlwc(c,l0); break;
	case T_NULL:
		break;
	}
	return(l0);
}

/**************************************************************************\
* 									   *
* Allocation of memory during the decoding:				   *
* 									   *
* C: node created on the "allocation" in the "mavros.gr" file. Contains    *
* the information about the arguments list for which memory allocation is  *
* needed.								   *
* 									   *
* T: type of the father of the "c" node. 				   *
* 									   *
\**************************************************************************/
dcalloclw(c,t,l0)
struct comp * c;
int t;			
int l0;		
{	int i;

	for (i=c->c_first_son; i!= -1; i=complist[i].c_brother){
		l0 = argprint(&complist[i],0,l0);
		(void)printf("= (%s *) asn1_malloc ((unsigned)(",complist[i].c_index);

		switch (t){
		case T_BITS:
			/* Horrible patch for MAVCOD */
			if (strcmp(complist[i].c_index,"asn1_field") == 0)
				goto standard;
			complist[c->c_father].c_type = T_BITS_ALLOC;
		case T_BITS_ALLOC:
			(void)printf("((ASN1_LWINT(asn1z))>>3)+1));\n");
			break;
		case T_OCTETS:
			/* Horrible patch for MAVCOD */
			if (strcmp(complist[i].c_index,"asn1_field") == 0)
				goto standard;
			complist[c->c_father].c_type = T_OCTETS_ALLOC;
		case T_OCTETS_ALLOC:
			(void)printf ("ASN1_LWINT(asn1z)+1));\n");
			break;
		case T_CHARS:
			(void)printf ("ASN1_LWINT(asn1z)+1));\n");
			break;
		case T_SEQUENCE_OF:
		case T_SET_OF:
			(void)printf("(ASN1_LWINT(asn1z)+1)*sizeof(%s)));\n",
			complist[i].c_index); 
			break;
		default:
		standard:
			(void)printf("sizeof(%s)));\n",complist[i].c_index); 
			break;
		}
		l0++;
	}
	return(l0);
}
/*****************************\
* 			      *
*  The main decoding switch   *
* 			      *
\*****************************/

dccomplw (c,l0)
struct comp * c; int l0;
{	int i;

#ifdef DEBUG
if (debuglevel > 6){
(void)printf ("/* decoding %d (%d:%s) */\n",(c - complist),c->c_type,c->c_name);
l0++;}
#endif
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);

	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		if (complist[i].c_type == T_ALLOC)
			l0 = dcalloclw(&complist[i],c->c_type,l0);
		if (complist[i].c_type == T_INIT)  
		l0 = dcinit(&complist[i],l0);
	}
	switch(c->c_type){
	case T_ANYDEF:
		l0 = cd_anydef(c,l0,LWDEC); break;
	case T_ANY:
	case T_COPY:
	case T_OID:
	case T_OCTETS:
		l0 = octetsdeclw(c,l0); break;
	case T_OID_ALLOC:
	case T_OCTETS_ALLOC:
	case T_ANY_ALLOC:
        case T_COPY_ALLOC:
		l0 = octetsalldeclw(c,l0); break;
	case T_FLAGS:
		l0 = flagdeclw(c,l0); break;
	case T_BITS:
		l0 = bitdeclw(c,l0); break;
	case T_BITS_ALLOC:
		l0 = bitalldeclw(c,l0); break;
	case T_CHARS:
		l0 = charsdeclw(c,l0); break;
	case T_INTEGER:
		l0 = integerdeclw(c,l0); break;
	case T_SEQUENCE: 
	case T_SET:
		l0 = seqdeclw(c,l0); break;
	case T_CHOICE: 
		l0 = choicedeclw(c,l0); break;
	case T_IGNORE:	
		(void)printf("asn1z = asn1_skiplw(asn1z);\n"); l0++; break;
	case T_SEQUENCE_OF: 
	case T_SET_OF:
		l0 = arraydeclw(c,l0); break;
	case T_BOOLEAN:
		l0 = booleandeclw(c,l0); break;
	case T_REAL:
		l0 = realdeclw(c,l0); break;
	case T_EXTERNAL:
		l0 = externlwd(c,l0); break;
	case T_NULL:
		break;
	case T_EXTERN:	
	default:
		l0 = cdextern(c,-1,0,l0,LWDEC); break;
	}
	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		if (complist[i].c_type == T_FINAL)
			l0 = dcinit(&complist[i],l0);
	}
	return(l0);
}

/***********************************************************************\
* 								        *
* Generation of the procedure for one given type.		        *
* 								        *
* The logic follows closely that of the ASN1 encoding and decoding      *
* routines; the "_len" and "_free" routines are not generated, as one   *
* can reuse those defined for ASN1.				        *
* 								        *
\***********************************************************************/
one_lw(
c,l0)
struct comp * c;
int l0;
{	int i, first_comp, sc=0;

	if (c->c_call_mode == DEFINE_MODE) return(l0);
#ifdef DEBUG
	if (debuglevel > 7)
	(void)fprintf(stderr,"Coding %s\n",c->c_name);
#endif
	storeindex = 0; sc = 0;
	(void)get_depth(c,0,&sc);
	current_list_head = c->c_first_son;
	for (first_comp= c->c_first_son;first_comp != -1;
	first_comp = complist[first_comp].c_brother)
		if (complist[first_comp].c_type != T_PARAMETER) break;

	/* Decoding file */
	if ((l0 = open_module_file(c->c_name,LWDEC,l0)) < 0)
		return(-1);

	/* generation of the light weight decoding routine */
	current_call_mode = 1;
	l0 = decl_proc_head(c,l0,LWDEC);
	(void)printf("{ int asn1l ");
	for (i=0; i<storeindex; i++) (void)printf(",%s",storelist[i]);
	(void)printf(";\n");l0++;
	l0 = dccomplw(&complist[first_comp],l0);
	(void)printf("return(asn1z);}\n"); l0++;

	if (c->c_call_mode == HALF_MODE) return(l0);

	/* Generation of the light weight coding routines */
	if ((l0 = open_module_file(c->c_name,LWCOD,l0)) < 0)
		return(-1);

	current_call_mode = 0;
	l0 = decl_proc_head(c,l0,LWCOD);
	(void)printf("{ int asn1l;\n");
	l0 += decl_var_cod(storeindex,storelist);
	l0 = cdcomplw(&complist[first_comp],l0);
	(void)printf("return(asn1z);}\n"); l0++;
	return(l0);
}







/************************************\
* 				     *
* Generation of all the procedures.  *
* 				     *
\************************************/

generlw()
{	int i, l0=0;

	for (i=0; i < compindex; i++){
		switch (complist[i].c_type){
		case T_TYPE:
			if (complist[i].c_optional == 0 )
				if ((l0 = one_lw(&complist[i],l0)) == -1)
					return(-1);
			break;
		default:
			break;
		}
	}
	return(0);
}
