/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavrosgen.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:15  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:09:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:05  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:39:35  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:04:05  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:05:49  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:07:43  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:11:40  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:54:46  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:57:40  marrek
 * 	November 1992 code drop
 * 	[1992/11/23  13:03:05  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:22:11  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:33:50  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavrosgen.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:20 $";
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
extern int compilation_mode;
extern int session_protocol_mode;


/***********************\
* 		        *
* Generating ASN1 tags  *
* 		        *
\***********************/
asn1codlen(n)
int n;
{	int l = 1;
	if (n < 31)	return(1);
	while (n){
		l++; n = n>>7;
	}
	return(l);
}

openasncod(m,n,s)
int m,n,s;
{	unsigned char x; int l=0,k;
	if (m != -1 && (m & HEAD))
		(void)printf("if (asn1_m == -1) {");
	if (session_protocol_mode){
		(void)printf("*asn1z++ = %d;",n);}
	else if (n < 31){
		x = (m<<6)|(s<<5)|n;
		(void)printf("*asn1z++ = %d;",x);
	}else{
		x = (m<<6)|(s<<5)|31;
		(void)printf("*asn1z++ = %d;",x);
		k = n; while(k){ l++; k = k>>7;}
		while (l--){
			if (l)
				x = 128 | ((n>>(7*l))&127);
			else	x = n&127;
			(void)printf("*asn1z++ = %d;",x);
		}
	}
	if (m != -1 &&(m&HEAD)){
	if (session_protocol_mode) 
		(void)printf("} else *asn1z++ = asn1_n;");
	else 	(void)printf(
		"} else asn1z = asn1_type_cod(asn1z,asn1_m,asn1_n,%d);",s);}

	if (session_protocol_mode && s){
 		(void)printf("\n{asn1 asn1z0=asn1z;\nasn1z += 3;\n");
 		return(3);
	}else if (s){
 		(void)printf("\n{asn1 asn1z0=asn1z++;\n");
 		return(2);
	}else	return(0);
}

closeasncod()
{
	if (session_protocol_mode){
		(void)printf(
 		"asn1z = session_protocol_end(asn1z, asn1z0);}\n");
 	}else{	(void)printf("asn1z =((asn1_x509)?asn1_defined_end:");
 		(void)printf("asn1_regular_end) (asn1z, asn1z0);}\n");
 	}
	return(1);
}

closeasnset()
{
	if (session_protocol_mode){
		(void)printf(
 		"asn1z = session_protocol_end(asn1z, asn1z0);}\n");
 	}else{	(void)printf("asn1z =((asn1_x509)?asn1_unique_set:");
 		(void)printf("asn1_regular_end) (asn1z, asn1z0);}\n");
 	}
	return(1);
}

asnstruct(c)
struct comp * c;
{
	if (c->c_implicit == 0 && c->c_mode != -1)	return(1);
	else switch (c->c_type){
	case T_EXTERNAL:
	case T_SEQUENCE:
	case T_SET:
	case T_SET_OF:
	case T_SEQUENCE_OF:	return(1);
	case T_INTEGER:
	case T_NULL:
	case T_BOOLEAN: return(0);
	default: return(2);
	}
}

matchasn1(m,n,t)
int m,n,t;
{
	unsigned char x; int l=0,k,i=0;

        if (session_protocol_mode){
                (void)printf(" *asn1z==%d ",n);
	}else if (n < 31){
		switch (t){
		case 0:
			x = (m<<6)|n;
			(void)printf(" *asn1z==%d ",x);
			break;
		case 1:
			x = (m<<6)|(1<<5)|n;
			(void)printf(" *asn1z==%d ",x);
			break;
		case 2:
			x = (m<<6)|(1<<5)|n;
			(void)printf("((*asn1z)|32)==%d ",x);
			break;
		}
	}else{
		switch (t){
		case 0:
			x = (m<<6)|31;
			(void)printf(" *asn1z==%d ",x);
			break;
		case 1:
			x = (m<<6)|(1<<5)|31;
			(void)printf(" *asn1z==%d ",x);
			break;
		case 2:
			x = (m<<6)|(1<<5)|31;
			(void)printf("((*asn1z)|32)==%d ",x);
			break;
		}
		k = n; while(k){ l++; k = k>>7;}
		i = 0;
		while (l--){
			i++;
			if (l)
				x = 128 | ((n>>(7*l))&127);
			else	x = n&127;
			(void)printf("&& *(asn1z+%d)==%d ",i,x);
		}
	}
}

/***************************************************************************\
* 									    *
* A set of functions to enable the secure decoding of T-L-V components.	    *
* 									    *
* Open:									    *
* Decode the tag field. This is done either by skipping a fixed number of   *
* bytes, when the expected tag is well known, or by calling a decoding	    *
* routine.								    *
* Then, if the component is structured - a tag, sequence or set -, decode   *
* the length field.							    *
* Open updates the pointer to the next byte ``asn1z'', which will contain   *
* zero if an error was encountered. It also places in the stack a pointer   *
* to the last byte of the value, ``asn1z2'', save the value of ``asn1zm''   *
* in ``asn1zms'', and updates ``asn1zm''.				    *
* 									    *
* Inside:								    *
* Test whether the next component is still inside the current structure.    *
* 									    *
* Close:								    *
* Call the ``asn1_close'' procedure in order to check the decoded length    *
* of the component; update ``asn1z'', set it to zero if an error is	    *
* encountered.  Then, restore the previous value of ``asn1zm''.		    *
* 									    *
\***************************************************************************/
asn1open(m,n,s,unsafe)
{	int l=0, k;

	if (unsafe)
 		(void)printf("if (asn1z) ");
	if (session_protocol_mode){
		(void)printf("asn1z++;");
	}else if (m == -1)
		(void)printf("asn1z = asn1_skiptype(asn1z,asn1zm);");
	else if (n < 32)
		(void)printf("asn1z++;");
	else{	k = n; while(k){ l++; k = k>>7;}
		(void)printf("asn1z += %d;",l+1);
	}
	if (s){
		(void)printf("\n{ asn1 asn1zms=asn1zm, asn1z2;\n");
		(void)printf("asn1z= %s(asn1z,asn1zm,&asn1z2);\n",
		(session_protocol_mode)?"session_length":"asn1_length");
                (void)printf("if(asn1z2) asn1zm=asn1z2;\n");
		return(4);
	}else	return(0);
}

asn1inside()
{
	if (session_protocol_mode)
		(void)printf("(asn1z && asn1z + 1 < asn1zm)");
	else	(void)printf("(asn1z && asn1z + 1 < asn1zm && *asn1z)");
}

asn1close()
{
	(void)printf("asn1z = %s(asn1z,asn1zm,asn1z2);\n",
	(session_protocol_mode)?"session_close_parm":"asn1_close");
	(void)printf("asn1zm = asn1zms;}\n");
	return(2);
}

/************************************\
* 				     *
*  Procedures for the INTEGER type   *
* 				     *
\************************************/

integercod(c,m,n,l0)
struct comp * c;
int m,n,l0;
{
	if (argnumber(c,1)) return(l0);
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	l0 += openasncod(m,n,0);
	(void)printf("asn1z = asn1_intcod(asn1z,");
	l0 = argprint(&complist[firstarg(c)],0,l0);
	(void)printf(");\n"); l0++;
	return(l0);	
}

integerlen(n,l0)
int n,l0;
{
	/* WE ONLY WANT A MAJORANT */
	(void)printf("asn1l += %d + 5;",asn1codlen(n)); l0++;
	return(l0);	
}

integerdec(c,m,n,l0,unsafe)
struct comp * c;
int m,n,l0,unsafe;
{
	if (argnumber(c,1)) return(l0);
	if (m == -1){
		(void) printf("if (asn1z = asn1_skiptype(asn1z,asn1zm))");
		(void) printf("asn1z = asn1_intdec(asn1z,asn1zm,");
		l0 = argprint(&complist[firstarg(c)],1,l0);
		(void)printf(");\n"); l0++;
	}else{	if (unsafe)
			(void) printf("if (asn1z) {");
		l0 += asn1open(m,n,0,0);
		(void)printf("asn1z = asn1_intdec(asn1z,asn1zm,");
		l0 = argprint(&complist[firstarg(c)],1,l0);
		if (unsafe)
			(void) printf(");}\n");
		else	(void)printf(");\n");
		l0++;
	}
	return(l0);	
}

/********************************\
* 				 *
*  Procedures for the REAL type	 *
* 				 *
\********************************/

realcod(c,m,n,l0)
struct comp * c;
int m,n,l0;
{
	if (argnumber(c,1)) return(l0);
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	l0 += openasncod(m,n,0);
	(void)printf("asn1z = asn1_realcod(asn1z,(double)(");
	l0 = argprint(&complist[firstarg(c)],0,l0);
	(void)printf("));\n"); l0++;
	return(l0);	
}

reallen(n,l0)
int n,l0;
{
	/* WE ONLY WANT A MAJORANT */
	(void)printf("asn1l += %d + 9;",asn1codlen(n)); l0++;
	return(l0);	
}

realdec(c,m,n,l0,unsafe)
struct comp * c;
int m,n,l0,unsafe;
{
	if (argnumber(c,1)) return(l0);
	if (m == -1){
		(void) printf("if (asn1z = asn1_skiptype(asn1z,asn1zm)){\n");
		l0++;
	}else{	if (unsafe){
			(void) printf("if (asn1z) {\n");
			l0++;
		}
		l0 += asn1open(m,n,0,unsafe);
	}
	l0 = argprint(&complist[firstarg(c)],0,l0);
	(void)printf(" = asn1_realval(asn1z,asn1zm); asn1z += (*asn1z) +1;");
	if (m == -1 || unsafe)
		(void) printf("}\n");
	else	(void) printf("\n");
	l0++;
	return(l0);	
}

/******************************************\
* 					   *
* Procedures for coding the EXTERNAL type  *
* 					   *
\******************************************/

externlen(c,n,l0)
struct comp * c;
int l0;
{	int i;

	if (argnumber(c,2)) return(l0);
	(void)printf("asn1l += asn1_external_len(%d,",n);
	l0 = argprint(&complist[i=firstarg(c)],0,l0);
	PUTC(',',stdout);
	l0 = argprint(&complist[complist[i].c_brother],0,l0);
	(void)printf(");\n"); l0++;
	return(l0);	
}

externcod(c,m,n,l0)
struct comp * c;
int l0;
{	int i;

	if (argnumber(c,2)) return(l0);
	(void)printf("asn1z = asn1_external_cod(asn1z,");
	if (m != -1 && (m&HEAD))
		(void)printf("asn1_m,asn1_n,");
	else	(void)printf("%d,%d,",m,n);
	l0 = argprint(&complist[i=firstarg(c)],0,l0);
	PUTC(',',stdout);
	l0 = argprint(&complist[complist[i].c_brother],0,l0);
	(void)printf(");\n"); l0++;
	return(l0);
}

externdec(c,l0)
struct comp * c;
int l0;
{	int i;

	if (argnumber(c,2)) return(l0);
	(void)printf("asn1z = asn1_external_dec(asn1z,asn1zm,");
	l0 = argprint(&complist[i=firstarg(c)],1,l0);
	PUTC(',',stdout);
	l0 = argprint(&complist[complist[i].c_brother],1,l0);
	(void)printf(");\n"); l0++;
	return(l0);	
}

/*************************************\
* 				      *
* Procedures for coding the ANY type  *
* 				      *
\*************************************/

anylen(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,2)) return(l0);
	(void)printf("asn1l += ");
	l0 = argprint(&complist[complist[firstarg(c)].c_brother],0,l0);
	(void)printf(" + WORD;\n"); l0++;
	return(l0);	
}

anydec(c,l0)
struct comp * c;
int l0;
{	int i;

	if (argnumber(c,2)) return(l0);
	l0 = argprint(&complist[i=firstarg(c)],0,l0);
	(void)printf("= asn1z; if(asn1z = asn1_skip(asn1z,asn1zm))\n"); l0++;
	l0 = argprint(&complist[complist[i].c_brother],0,l0); 
	(void)printf("= asn1z - ");
	l0 = argprint(&complist[i=firstarg(c)],0,l0); (void)printf(";\n"); l0++;
	return(l0);	
}

anyalldec(c,l0)
struct comp * c;
int l0;
{	int i;

	if (argnumber(c,2)) return(l0);
	(void)printf("if (asn1z = asn1_move(asn1z_0 = asn1z,asn1zm,");
	l0 = argprint(&complist[i=firstarg(c)],0,l0);
	(void)printf("))\n"); l0++;
	l0 = argprint(&complist[complist[i].c_brother],0,l0); 
	(void)printf("= asn1z - asn1z_0;\n"); l0++;
	return(l0);	
}

anycod(c,l0)
struct comp * c;
int l0;
{	int i;

	if (argnumber(c,2)) return(l0);
	(void)printf("ASN1_MOVE(");
	l0 = argprint(&complist[i = firstarg(c)],0,l0);
	(void)printf(",asn1z,");
	l0 = argprint(&complist[complist[i].c_brother],0,l0);
	(void)printf(");\n"); l0++;
	(void)printf("asn1z += ");
	l0 = argprint(&complist[complist[i].c_brother],0,l0);
	(void)printf(";\n"); l0++;
	return(l0);
}

/****************************************************************************\
* 									     *
* The copy type is treated as any, except when coding + changing the flag.   *
* 									     *
\****************************************************************************/
copycod(c,m,n,l0)
struct comp * c;
int l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);
	if (m != -1 && (m&HEAD)){
		(void)printf("asn1z = asn1_copy(asn1z,asn1_m,asn1_n,");
	}else	(void)printf("asn1z = asn1_copy(asn1z,%d,%d,",m,n);
	l0 = argprint(&complist[i],0,l0);
	(void)printf(",");
	l0 = argprint(&complist[complist[i].c_brother],0,l0);
	(void)printf(");\n"); l0++;
	return(l0);	
}

/****************************************************\
* 						     *
* Procedures for coding and decoding octet strings   *
* 						     *
\****************************************************/

octetslen(c,n,l0)
struct comp * c;
int n,l0;
{
	if (argnumber(c,2)) return(l0);
	/* MAX STRING LENGTH ON 3 BYTES... */
	(void)printf("asn1l += %d + WORD + ",asn1codlen(n));
#ifdef ALIGNMENT
	(void)printf("WORD -1 + ");
#endif
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf(";\n"); l0++;
	return(l0);	
}

octetscod(c,m,n,l0)
struct comp * c;
int m,n,l0;
{
	if (argnumber(c,2)) return(l0);
	l0 += openasncod(m,n,0);
	if (session_protocol_mode)
		(void)printf("asn1z = session_lencod(asn1z,");
	else	(void)printf("asn1z = asn1_lencod(asn1z,");
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf(");\n"); l0++;
	(void)printf("ASN1_MOVE(");
	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf(",asn1z,");
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf("); asn1z += ");
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf(";\n"); l0++;
	return(l0);	
}

octetsdec(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,2)) return(l0);
	if (session_protocol_mode)
		(void)printf("asn1z = session_octets_dec(asn1z,asn1zm,");
	else	(void)printf("asn1z = asn1_octets_dec(asn1z,asn1zm,");
	l0 = argprint(&complist[c->c_first_son],1,l0);
	(void)printf(",");
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],1,l0);
	(void)printf(");\n"); l0++;
	return(l0);
}

octetsalldec(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,2)) return(l0);
	if (session_protocol_mode)
		(void)printf ("asn1z = session_octmov(asn1z,asn1zm,");
	else	(void)printf ("asn1z = asn1_octmov(asn1z,asn1zm,");
	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf(",");
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],1,l0);
	(void)printf(");\n"); l0++;
	return(l0);
}

/**************************************************************\
* 							       *
* Procedures for coding and decoding null terminated strings   *
* 							       *
\**************************************************************/
charslen(c,n,l0)
struct comp * c;
int n,l0;
{
	if (argnumber(c,1)) return(l0);
	/* MAX STRING LENGTH ON 3 BYTES... */
	(void)printf("asn1l += %d + WORD + strlen(",asn1codlen(n));
	l0 = argprint(&complist[c->c_first_son],0,l0);
#ifdef ALIGNMENT
	(void)printf(") + WORD -1;\n");
#else
	(void)printf(");\n"); 
#endif
	l0++;
	return(l0);	
}

charscod(c,m,n,l0)
struct comp * c;
int m,n,l0;
{
	if (argnumber(c,1)) return(l0);
	l0 += openasncod(m,n,0);
	(void) printf("{ register int asn1l;\n"); l0++;
	if (session_protocol_mode)
		(void)printf("asn1z = session_lencod(asn1z,asn1l=strlen(");
	else	(void)printf("asn1z = asn1_lencod(asn1z,asn1l=strlen(");

	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf("));\n"); l0++;
	(void)printf("ASN1_MOVE(");
	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf(",asn1z,asn1l); asn1z += asn1l;}\n"); l0++;
	return(l0);	
}

charsdec(c,l0)
struct comp * c;
int l0;
{
	if (argnumber(c,1)) return(l0);
	if (session_protocol_mode)
		(void)printf ("asn1z = session_strmov(asn1z,asn1zm,");
	else	(void)printf ("asn1z = asn1_strmov(asn1z,asn1zm,");
	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf(");\n"); l0++;
	return(l0);
}

/**************************************************\
* 						   *
* Procedures for coding and decoding bit strings   *
* 						   *
\**************************************************/

bitlen(c,n,l0)
struct comp * c;
int n,l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);
	/* MAX STRING LENGTH ON 3 BYTES... */
	(void)printf("asn1l += %d + WORD + ((",asn1codlen(n));
	l0 = argprint(&complist[complist[i].c_brother],0,l0);
#ifdef ALIGNMENT
	(void)printf(")>>3) + WORD;\n"); l0++;
#else
	(void)printf(")>>3) + 1;\n"); l0++;
#endif
	return(l0);
}

bitcod(c,m,n,l0)
struct comp * c;
int m,n,l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);
	l0 += openasncod(m,n,0);
	if (session_protocol_mode)
		(void)printf("asn1z = session_bitcod(asn1z,");
	else	(void)printf("asn1z = asn1_bitcod(asn1z,");
	l0 = argprint(&complist[i],0,l0);
	(void)printf(",");
	l0 = argprint(&complist[complist[i].c_brother],0,l0);
	(void)printf(");\n"); l0++;
	return(l0);	
}

bitdec(c,l0)
struct comp * c;
int l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);
	if (session_protocol_mode)
		(void)printf("asn1z = session_bitdec(asn1z,asn1zm,");
	else	(void)printf ("asn1z = asn1_bits_dec(asn1z,asn1zm,");
	l0 = argprint(&complist[i],1,l0);
	(void)printf(",");
	l0 = argprint(&complist[complist[i].c_brother],1,l0);
	(void)printf(");\n");l0++;
	return(l0);
}

bitalldec(c,l0)
struct comp * c;
int l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);
	if (session_protocol_mode)
		(void)printf("asn1z = session_binmov(asn1z,asn1zm,");
	else	(void)printf ("asn1z = asn1_binmov(asn1z,asn1zm,");
	l0 = argprint(&complist[i],0,l0);
	(void)printf(",");
	l0 = argprint(&complist[complist[i].c_brother],1,l0);
	(void)printf(");\n"); l0++;
	return(l0);
}

/*************************************************************************\
* 									  *
* Procedures for the handling of flags, i.e. bit strings coded in words.  *
* 									  *
\*************************************************************************/

flagcod(c,m,n,l0)
struct comp * c;
int m,n,l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);
	l0 += openasncod(m,n,0);
	if (session_protocol_mode)
		(void)printf("asn1z = session_flagcod(asn1z,");
	else	(void)printf("asn1z = asn1_flagcod(asn1z,");
	l0 = argprint(&complist[i],0,l0);
	(void)printf(",");
	l0 = argprint(&complist[complist[i].c_brother],0,l0);
	(void)printf(");\n"); l0++;
	return(l0);	
}

flagdec(c,l0)
struct comp * c;
int l0;
{	int i = firstarg(c);

	if (argnumber(c,2)) return(l0);
	if (session_protocol_mode)
		(void)printf("asn1z = session_flags_dec(asn1z,asn1zm,");
	else	(void)printf ("asn1z = asn1_flags_dec(asn1z,asn1zm,");
	l0 = argprint(&complist[i],1,l0);
	(void)printf(",");
	l0 = argprint(&complist[complist[i].c_brother],0,l0);
	(void)printf(");\n"); l0++;
	return(l0);
}

/***********************************\
* 				    *
* Procedures for the BOOLEAN type   *
* 				    *
\***********************************/

booleanlen(n,l0)
int n,l0;
{
#ifdef ALIGNMENT
	(void)printf("asn1l += %d + WORD - 1;\n",asn1codlen(n)); l0++;
#else
	(void)printf("asn1l += %d + 2;\n",asn1codlen(n)); l0++;
#endif
	return(l0);	
}

booleancod(c,m,n,l0)
struct comp * c;
int m,n,l0;
{	struct comp * d = &complist[c->c_first_son];

	if (argnumber(c,1)) return(l0);
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	l0 += openasncod(m,n,0);
	(void)printf("*asn1z++ = 1; *asn1z++ = (");
	l0 = argprint(d,0,l0);
	(void)printf(")?0xFF:0;\n"); l0++;
	return(l0);	
}

booleandec(c,m,n,l0,unsafe)
struct comp * c;
int m,n,l0,unsafe;
{	struct comp * d = &complist[c->c_first_son];

	if (argnumber(c,1)) return(l0);
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	l0 += asn1open(m,n,0,unsafe);
	(void)printf("if (asn1z == 0 || asn1z +2 > asn1zm || *asn1z++ !=1)\n");
	(void)printf("asn1z = asn1_errdec(asn1z,ASN1_ERR_BOOL);\n");l0+= 2;
	(void)printf("else {");
	l0 = argprint(d,0,l0);
	(void)printf(" = (*asn1z++ != 0);}\n"); l0++;
	return(l0);	
}

/********************************\
* 				 *
* Procedures for the NULL type 	 *
* 				 *
\********************************/

nulllen(n,l0)
int n,l0;
{
	(void)printf("asn1l += %d +1;\n",asn1codlen(n)); l0++;
	return(l0);	
}

nullcod(c,m,n,l0)
struct comp * c;
int m,n,l0;
{
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	l0 += openasncod(m,n,0);
	(void)printf("*asn1z++ = 0;\n"); l0++;
	return(l0);	
}

nulldec(m,n,l0,unsafe)
int m,n,l0,unsafe;
{
	l0 += asn1open(m,n,0,unsafe);
	(void)printf(	"if (asn1z == 0 || asn1z >= asn1zm || *asn1z++)\n"); 
	(void)printf("asn1z = asn1_errdec(asn1z,ASN1_ERR_NULL);\n");
	l0+=2;
	return(l0);	
}

/*********************************\
* 				  *
* Procedures for default values.  *
* 				  *
\*********************************/

affectprint(c,l0,m)
struct comp * c;
int l0, m;
{	int i;

	if (m)	(void)printf("if(");
	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		l0 = argprint(&complist[complist[i].c_first_son],0,l0);
		(void)printf(" %s ",(m)?"!=":"=");
		l0 = argprint(&complist[complist[complist[i].c_first_son].c_brother],0,l0);
		if (m){
			if(complist[i].c_brother != -1)
				(void)printf(" ||\n");
			else	(void)printf("){\n");
		}else	(void)printf(";\n");
		l0++;
	}
	return(l0);
}

defaulttest(c,l0,t,m)
struct comp * c;
int l0, *t, m;
{	int i;
#ifdef DEBUG
if (debuglevel > 4)
(void)printf ("/* default %d (%d:%s) */\n",(c - complist),c->c_type,c->c_name);
#endif   

	*t = 0;
	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_DEFAULT){
			if (m==1){ (void)printf("}else {\n"); l0++;};
			*t = 1;
			l0 = affectprint(&complist[i],l0,m==0);
			break;
		}
	}
	return(l0);
}

/**************************************************************************\
* 									   *
* Procedures for SEQUENCEs and also SETs: the only difference is in the	   *
* decoding phase.							   *
* 									   *
* Two specific problems are encountered within the handling of SEQUENCES   *
* (and SETs): Choice components and ``COMPONENTS OF''. Untagged choice	   *
* components are hard to decode, as their tags can take a set of values;   *
* for this reason, the special ``match-in-choice'' procedure was write a   *
* test that recognizes one of the elements.				   *
* 									   *
* 									   *
* The insertion of COMPONENTS OF is done via externally generated	   *
* subroutines:								   *
* 									   *
* Components coding:							   *
* 	A simple code to "z = <IDENT>_ccod(z,arglist);"			   *
* 	results in the coding of all components.			   *
* 									   *
* Components decoding within sequence:					   *
* 	A simple call to "z = <IDENT>_dseq(z,zm,arglist);"		   *
* 	where "zm" is address of the last byte of the embedding		   *
* 	sequence, or 0 if and EOC is expected. The call will		   *
* 	include the decoding of the components plus the setting of	   *
* 	default values.							   *
* 									   *
* Components decoding within a SET:					   *
* 	The same procedure is used for decoding components and decoding	   *
* 	the set itself.							   *
* 									   *
\**************************************************************************/

seqlen(c,n,l0)
struct comp * c;
int n,l0;
{	int i,t;

#ifdef DEBUG
if (debuglevel > 4){
(void)printf("/* Start of STRUCT_len */\n"); l0++;
(void)fflush (stdout);}
#endif
	(void)printf("asn1l += %d + 3;\n",asn1codlen(n)); l0++;
	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP){
			l0 = cdextern(&complist[i],-1,0,l0,LENGTH);
			continue;
		}
		l0 = defaulttest(&complist[i],l0,&t,0);
		l0 = lncomp(&complist[i],l0);
		if (t){	(void)printf("} asn1l += WORD;\n"); l0++;}
	}
#ifdef DEBUG
if (debuglevel > 4){
(void)printf("/* End of STRUCT_len */\n"); l0++;
(void)fflush (stdout);}
#endif
	return(l0);
}

seqbodycod(c,l0)
struct comp * c;
int l0;
{	int i,t;

	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP){
			l0 = cdextern(&complist[i],-1,0,l0,COMPCOD);
			continue;
		}
		l0 = defaulttest(&complist[i],l0,&t,0);
		l0 = cdcomp(&complist[i],l0,0);
		if(t)	(void)printf("}\n");
	}
	return(l0);
}

seqcod(c,m,n,l0)
struct comp * c;
int m,n,l0;
{
	l0 += openasncod(m,n,1);
	l0 = seqbodycod(c,l0);
	l0 += (c->c_type == T_SET) ? closeasnset(): closeasncod();
	return(l0);	
}

seqbodydec(c,l0)
struct comp * c;
int l0;
{	int i,j;

	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP){
			l0 = cdextern(&complist[i],-1,0,l0,C_SEQ_D);
			continue;
		}
		(void)printf("if(");
		asn1inside();
		if (complist[i].c_type == T_CHOICE && complist[i].c_mode == -1){
			(void)printf("&& ");
			l0 = matchchoice(&complist[i],l0);
		}else if (complist[i].c_type == T_EXTERN 
		&& complist[i].c_mode == -1){
			(void)printf("&& ");
			l0 = matchextern(&complist[i],l0);
		}else if ((complist[i].c_type != T_ANY 
		&& complist[i].c_type != T_ANY_ALLOC
		&& complist[i].c_type != T_ANYDEF)
		|| (complist[i].c_implicit == 0 && complist[i].c_mode != -1)){
			(void)printf("&&");
			matchasn1(complist[i].c_mode, complist[i].c_number, 
			asnstruct(&complist[i]));
		}
		(void)printf("){\n"); l0++;
		l0 = dccomp(&complist[i],l0,0);
		(void)printf("}\n"); l0++;
		if (complist[i].c_type == T_IGNORE) continue;
		if (complist[i].c_optional){
			for (j = complist[i].c_first_son; 
			j != -1; j = complist[j].c_brother)
				if (complist[j].c_type == T_DEFAULT) break;
			if (j == -1) continue;
			(void)printf("else {");
			for (;j!=-1;j=complist[j].c_brother)
				l0 = affectprint(&complist[j],l0,0);
			(void)printf("}\n"); l0++;
		}else{	(void)printf(
			"else { asn1z = asn1_errdec(asn1z,ASN1_ERR_MISS);\n");
			l0++;
			l0 = errcomp(&complist[i],l0);
			(void)printf("}\n"); l0++;
		}
	}
	return(l0);
}

seqdec(c,m,n,l0,unsafe)
struct comp * c;
int m,n,l0,unsafe;
{
	l0 += asn1open(m,n,1,unsafe);
	l0 = seqbodydec(c,l0);

	(void)printf("while("); asn1inside(); (void)printf(")");
	(void)printf("asn1z = asn1_skip(asn1z,asn1zm);\n");l0++;
	l0 += asn1close();
	return(l0);
}

seqerr(c,l0)
struct comp * c;
int l0;
{	int i, j;

	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP){
			l0 = cdextern(&complist[i],-1,0,l0,ERRCOD);
			continue;
		}
		if (complist[i].c_type == T_IGNORE) continue;
		if (complist[i].c_optional){
			for (j = complist[i].c_first_son; j != -1; j = complist[j].c_brother)
				if (complist[j].c_type == T_DEFAULT) break;
			if (j == -1) continue;
			for (;j!=-1;j=complist[j].c_brother)
				l0 = affectprint(&complist[j],l0,0);
		}else{	l0 = errcomp(&complist[i],l0);
		}
	}
	return(l0);
}

/****************************************************************************\
* 									     *
* Decoding of sets:							     *
* 									     *
* First, decode all components, using the ``set'' procedure. This could be   *
* a bit more optimized, as we will decode the header several times, but to   *
* the expense of a longer code.						     *
* 									     *
* Then, set to zero ``comp'' components of the ``set components stack'',     *
* before attempting to decode the set.					     *
* 									     *
* The set is decoded by a loop: if the flag of a component is recognized,    *
* the component is noted as ``present'', and the corresponding decoding is   *
* performed. We guarantee that each component will be decoded at most once;  *
* the occurence of ``unrecognized components'' or that of ``duplicate	     *
* components'' will not be treated as an error.				     *
* 									     *
* Then, for all components which were not ``present'', either the default    *
* values are set or, if the component is mandatory, an error is signalled.   *
* 									     *
\****************************************************************************/
setdec(c,m,n,l0,unsafe)
struct comp * c;
int m,n,l0,unsafe;
{	int i, j, first=1, comp = 0, icomp = 0;

	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP){
			(void)printf("if(asn1z){ asn1z_0 = asn1z;\n"); l0++;
			l0 = cdextern(&complist[i],-1,0,l0,DECODE);
			(void)printf("if (asn1z) asn1z = asn1z_0;} else\n"); l0++;
			l0 = cdextern(&complist[i],-1,0,l0,ERRCOD);
			(void)printf(";\n"); l0++;
			continue;
		}else	comp++;
	}
	(void)printf("ASN1_ZERO(asn1set+asn1setc,%d); asn1setc += %d;\n", 
	comp, comp);
	if (unsafe){
		(void) printf("if (asn1z){");
	}
	l0 += asn1open(m,n,1,0);
	(void)printf ("while(");
	asn1inside();
	(void)printf("){\n"); l0++;
	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_COMP) continue;
		if (first)
			first = 0;
		else	(void)printf(" else ");
		(void)printf("if(");
		if ( complist[i].c_type == T_CHOICE && complist[i].c_mode == -1)
			l0 = matchchoice(&complist[i],l0);
		else if (complist[i].c_type == T_EXTERN && complist[i].c_mode == -1)
			l0 = matchextern(&complist[i],l0);
		else	matchasn1(complist[i].c_mode, complist[i].c_number, 
			asnstruct(&complist[i]));
		(void)printf("){\n"); l0++;
		(void)printf("asn1set[asn1setc - %d] = 1;\n", comp - icomp);
		l0 = dccomp(&complist[i],l0,0);
		(void)printf("}");
		icomp++;
	}
	(void)printf("else asn1z = asn1_skip(asn1z,asn1zm);}\n");l0++;
	l0 += asn1close();
	if (unsafe){
		(void)printf("}\n"); l0++;
	}
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
		if (complist[i].c_optional){
			for (;j != -1;j=complist[j].c_brother)
				l0 = affectprint(&complist[j],l0,0);
		}else{	(void)printf("asn1z = asn1_errdec(asn1z,ASN1_ERR_MISS);\n");
			l0++;
			l0 = errcomp(&complist[i],l0);
		}
		(void)printf("}\n"); l0++;
	}
	return(l0);
}

/***************************************************************************\
* 									    *
* Procedures for CHOICE							    *
* 									    *
* The ``matchchoice'' routine provides a test for the presence of a	    *
* component of type CHOICE within a set, a sequence or another choice.	    *
* Although the test can look somewhat redundant (each component will be	    *
* tested in turn), it cannot be easily bypassed, as we have either to test  *
* for default values (sequence), or to mark the presence of a component	    *
* (set), or set a particular choice alternative (choice).		    *
* 									    *
* Moreover, including CHOICE components withion other structures without    *
* tagging them should not be too encouraged...				    *
* 									    *
\***************************************************************************/

matchchoice(c,l0)
struct comp * c;
int l0;
{	int i, first = 0;

	(void)printf (" ( ");
	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_ARGUMENT) continue;
		if (first++){
			(void)printf("\n|| ");l0++;}
		if (complist[i].c_mode == -1
		&&  complist[i].c_type == T_CHOICE)
			l0 = matchchoice(&complist[i],l0);
		else if (complist[i].c_mode == -1
		&&  complist[i].c_type == T_EXTERN)
			l0 = matchextern(&complist[i],l0);
		else	matchasn1(complist[i].c_mode, complist[i].c_number, 
			asnstruct(&complist[i]));

	}
	(void)printf(" ) ");
	return(l0);
}

matchextern(c,l0)
struct comp * c;
int l0;
{
	(void)printf("%s%s(asn1z)", c->c_name, exec_table[MATCH].suffix);
	return(l0);
}

choicedec(c,l0,unsafe)
struct comp * c;
int l0,unsafe;
{	int i;

	if (unsafe){
		printf("{ int asn1_choice_ok=1;\n");
		printf("if (asn1z == 0) asn1_choice_ok = 0; else {\n");
		l0 += 2;
	}

	for (i=complist[c->c_first_son].c_brother; i != -1; 
	i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (complist[i].c_type == T_ARGUMENT) continue;
		(void)printf("if(");
		if (complist[i].c_mode == -1
		&&  complist[i].c_type == T_CHOICE){
			l0 = matchchoice(&complist[i],l0);
		} else if (complist[i].c_mode == -1
		&&  complist[i].c_type == T_EXTERN)
			l0 = matchextern(&complist[i],l0);
		else	matchasn1(complist[i].c_mode, complist[i].c_number,
			asnstruct(&complist[i]));
		(void)printf("){\n"); l0++;
		l0 = dccomp(&complist[i],l0,0);
		if (complist[i].c_index != 0){
			l0 = argprint(&complist[c->c_first_son],0,l0);
			(void)printf(" = %s;",complist[i].c_index);
		}
		(void)printf("}else\n"); l0++;
	}

	if (unsafe){
	(void) printf("asn1_choice_ok = 0;} if (asn1_choice_ok == 0){\n"); 
	l0++;
	}else	(void) printf("{");
	(void)printf("asn1z = asn1_errdec(asn1z,ASN1_ERR_CHOICE);\n"); l0++;
	l0 = errcomp(c,l0);
	if (unsafe){
		(void)printf("}}\n");
	}else	(void)printf("}\n");
	l0++;
	return(l0);
}

errchoice(c,l0)
struct comp * c;
int l0;
{	int i=complist[c->c_first_son].c_brother;

	if (i != -1
	&& !(	complist[i].c_type == T_INIT
	||	complist[i].c_type == T_FINAL
	||	complist[i].c_type == T_DEFAULT
	||	complist[i].c_type == T_ALLOC)){
		l0 = errcomp(&complist[i],l0);
		if (complist[i].c_index != 0){
			l0 = argprint(&complist[c->c_first_son],0,l0);
			(void)printf(" = %s;\n",complist[i].c_index); l0++;
		}
	}
	return(l0);
}

choicelen(c,l0)
struct comp * c;
int l0;
{	int i, openchoice = 1;

	(void)printf("asn1l += WORD; switch (CAST_FOR_SWITCH(");
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
		l0 = lncomp(&complist[i],l0);
		if (complist[i].c_index == 0){
			openchoice = 0;
			(void)printf("break;}\n"); l0++; return(l0);
		}else{	(void)printf("break;\n"); l0++;}
	}
	if (openchoice){	(void)printf("default:break;}\n"); l0++;}
#ifdef DEBUG
if (debuglevel > 4){
(void)printf("/* End of CHOICE */\n"); l0++;
(void)fflush(stdout);}
#endif
	return(l0);
}

choicecod(c,l0)
struct comp * c;
int l0;
{	int i, openchoice = 1;

	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	(void)printf("switch (CAST_FOR_SWITCH(");
	l0 = argprint(&complist[c->c_first_son],0,l0);
	(void)printf(")){\n"); l0++;
	for (i=complist[c->c_first_son].c_brother; 
	i != -1; i = complist[i].c_brother){
		if (complist[i].c_type == T_INIT
		||  complist[i].c_type == T_FINAL
		||  complist[i].c_type == T_DEFAULT
		||  complist[i].c_type == T_ALLOC) break;
		if (l0 != complist[i].c_line_num)
			numberline(l0 = complist[i].c_line_num);
		if (complist[i].c_index == 0)
			(void)printf("default: ");
		else	(void)printf("case %s: ",complist[i].c_index);
		l0 = cdcomp(&complist[i],l0,0);
		if (complist[i].c_index == 0){
			openchoice = 0;
			(void)printf("break;}\n"); l0++; return(l0);
		}else{	(void)printf("break;\n"); l0++;}
	}
	if (openchoice){	(void)printf("default:break;}\n"); l0++;}
#ifdef DEBUG
if (debuglevel > 4){
(void)printf("/* End of CHOICE */\n"); l0++;
(void)fflush(stdout);}
#endif
	return(l0);
}

/*****************************************\
* 					  *
* Procedures for SET OF and SEQUENCE OF   *
* 					  *
\*****************************************/

arraylen(c,n,l0)
struct comp * c;
int n,l0;
{
#ifdef DEBUG
if (debuglevel > 4){
(void)printf("/* End of ARRAY_len */\n"); l0++;
(void)fflush(stdout);}
#endif
	(void)printf("asn1l += %d + 3;\n",asn1codlen(n)); l0++;
	(void)printf("for (%s=0; %s <",c->c_name,c->c_name);
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf("; %s++){\n",c->c_name); l0++;
	l0 = lncomp(&complist[c->c_first_son],l0);
	(void)printf("}\n"); l0++;
#ifdef DEBUG
if (debuglevel > 4){
(void)printf("/* End of ARRAY_len */\n"); l0++;
(void)fflush(stdout);}
#endif
	return(l0);
}

arraycod(c,m,n,l0)
struct comp * c;
int m,n,l0;
{
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	l0 += openasncod(m,n,1);
	(void)printf("for (%s=0; %s <",c->c_name,c->c_name);
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf("; %s++){\n",c->c_name); l0++;
	l0 = cdcomp(&complist[c->c_first_son],l0,0);
	(void)printf("}\n"); l0++;
	l0 += (c->c_type == T_SET_OF) ? closeasnset(): closeasncod();
	return(l0);	
}

arraydec(c,m,n,l0,unsafe)
struct comp * c;
int m,n,l0,unsafe;
{
	l0 += asn1open(m,n,1,unsafe);
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	(void)printf ("%s=0; while(",c->c_name);
	asn1inside();
	(void)printf("){\n"); l0++;
	l0 = dccomp(&complist[c->c_first_son],l0,0);
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	(void)printf("%s++;}",c->c_name);
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf("= %s;\n",c->c_name); l0++;
	l0 += asn1close();
	return(l0);
}

errarray(c,l0)
struct comp * c;
int l0;
{
	l0 = argprint(&complist[complist[c->c_first_son].c_brother],0,l0);
	(void)printf("= 0;\n"); l0++;
	return(l0);
}

/****************************************************\
* 						     *
* The general switch for the encoding of ``_len.c''  *
* 						     *
\****************************************************/
lncompbis(c,m,n,l0)
struct comp * c;
int m,n,l0;
{
	switch(c->c_type){
	case T_OCTETS:
	case T_OCTETS_ALLOC:
	case T_OID:
	case T_OID_ALLOC:
		if (m == -1) {n = 4;}
		l0 = octetslen(c,n,l0); break;
	case T_BITS:
	case T_FLAGS:
	case T_BITS_ALLOC:
		if (m == -1) {n = 3;}
		l0 = bitlen(c,n,l0); break;
	case T_CHARS:
		if (m == -1) {n = 19;}
		l0 = charslen(c,n,l0); break;
	case T_ANY:
	case T_COPY:
	case T_ANY_ALLOC:
	case T_COPY_ALLOC:
		l0 = anylen(c,l0); break;
	case T_INTEGER:
		if (m == -1) {m = 0; n = 2;}
		l0 = integerlen(n,l0); break;
	case T_EXTERN:
		l0 = cdextern(c,m,n,l0,LENGTH); break;
	case T_SEQUENCE: if (m == -1) {n = 16;}
		l0 = seqlen(c,n,l0); break;
	case T_SET: if (m == -1) {n = 17;}
		l0 = seqlen(c,n,l0); break;
	case T_CHOICE: l0 = choicelen(c,l0); break;
	case T_IGNORE:
		(void)printf("asn1l += WORD;\n"); l0++;
		break;
	case T_SEQUENCE_OF: if (m == -1) {n = 16;}
		l0 = arraylen(c,n,l0); break;
	case T_SET_OF: if (m == -1) {n = 17;}
		l0 = arraylen(c,n,l0); break;
	case T_BOOLEAN:if (m == -1) {n = 1;}
		l0 = booleanlen(n,l0); break;
	case T_EXTERNAL:
		l0 = externlen(c,n,l0); break;
	case T_REAL:
		l0 = reallen(n,l0); break;
	case T_NULL:if (m == -1) {n = 5;}
		l0 = nulllen(n,l0); break;
	case T_ANYDEF:
		l0 = cd_anydef(c,l0,LENGTH); break;
	default:
		(void)printf("\n/* PANIC (default) %s */",c->c_name); break;
	}
	return(l0);
}


lncomp(c,l0)
struct comp * c;
int l0;
{
#ifdef DEBUG
if (debuglevel > 6){
(void)printf ("/* length of %d (%d:%s) */\n",(c - complist),c->c_type,c->c_name); l0++;
(void)fflush(stdout);
}
#endif
	if (c->c_implicit == 0 && c->c_mode != -1){
		(void)printf("asn1l += %d +3;\n",asn1codlen(c->c_number));
		l0 = lncompbis(c,-1,0,l0);
	}else	l0 = lncompbis(c,c->c_mode,c->c_number,l0);
#ifdef DEBUG
if (debuglevel > 6)
(void)printf ("/* end of length of %d (%d:%s) */\n",(c - complist),c->c_type,c->c_name);
#endif
	return(l0);
}

/*************************************************\
* 						  *
* The main switch for the encoding of ``_cod.c''  *
* 						  *
\*************************************************/

cdcompbis(c,m,n,l0)
struct comp * c;
int m,n,l0;
{
	switch(c->c_type){
	case T_OCTETS_ALLOC:
	case T_OCTETS:
	case T_OID_ALLOC:
	case T_OID:
		l0 = octetscod(c,m,n,l0); break;
	case T_FLAGS:
		l0 = flagcod(c,m,n,l0); break;
	case T_BITS:
	case T_BITS_ALLOC:
		l0 = bitcod(c,m,n,l0); break;
	case T_CHARS:
		l0 = charscod(c,m,n,l0); break;
	case T_ANY:
	case T_ANY_ALLOC:
		l0 = anycod(c,l0); break;
	case T_ANYDEF:
		l0 = cd_anydef(c,l0,ENCODE); break;
	case T_COPY:
	case T_COPY_ALLOC:
		if (m == -1)
			l0 = anycod(c,l0);
		else	l0 = copycod(c,m,n,l0); break;
	case T_INTEGER:
		l0 = integercod(c,m,n,l0); break;
	case T_EXTERN:
		l0 = cdextern(c,m,n,l0,ENCODE); break;
	case T_SEQUENCE:
	case T_SET:
		l0 = seqcod(c,m,n,l0); break;
	case T_CHOICE:
		l0 = choicecod(c,l0); break;
	case T_IGNORE:	break;
	case T_SEQUENCE_OF:
	case T_SET_OF:
		l0 = arraycod(c,m,n,l0); break;
	case T_BOOLEAN:
		l0 = booleancod(c,m,n,l0); break;
	case T_EXTERNAL:
		l0 = externcod(c,m,n,l0); break;
	case T_REAL:
		l0 = realcod(c,m,n,l0); break;
	case T_NULL:
		l0 = nullcod(c,m,n,l0); break;
	}
	return(l0);
}

/*****************************************************************\
* 								  *
* Starts the coding of a new node.				  *
* "c->c_implicit" is true only in the case the key word IMPLICIT  *
* 	was used in association to the def of the node.		  *
* 	eg: 	type (...) ::=					  *
* 			[0] IMPLICIT SEQUENCE ...		  *
* 								  *
* "c->c_mode" differs from -1 only in the case there is a	  *
* 	specific tag associated to this node			  *
* 	eg: 	type (...) ::=					  *
* 			[UNIVERSAL 6] ...			  *
* 	eg: 	type (...) ::=					  *
* 			[0] IMPLICIT SEQUENCE ...		  *
* 								  *
* if no tag is used, "c->c_mode" = -1 and "c->c_number" = 0	  *
* 	eg: 	type (...) ::=					  *
* 			SEQUENCE {...}				  *
* 								  *
\*****************************************************************/
cdcomp(c,l0,head)
struct comp * c;
int l0,head;
{	int m, n;
#ifdef DEBUG
if (debuglevel > 6)
(void)printf ("/* coding %d (%d:%s) %s*/\n",(c - complist),c->c_type,c->c_name,
(head)?"HEAD ":"");
#endif
	if (c->c_implicit == 0 && c->c_mode != -1){
		/* not IMPLICIT and a specific tag is associated;
		 * coding this tag, the "S" bit will be set to 1,
		 * since the tag introduces a new level of
		 * embedment.	*/
		l0 += openasncod((head)?(c->c_mode)|HEAD:c->c_mode,
		c->c_number,1);
		/* then coding the type following the specific tag. */
		if (c->c_u_tag == -1){
			m = -1; n = 0;}
		else{	m = 0; n = c->c_u_tag;}
		l0 = cdcompbis(c,m,n,l0);
		l0 += closeasncod();

	}else
		/* . either an IMPLICIT coding is requested
		 *   (associated with a specific class ("m")
		 *   and tag ("n").
		 * . or no tag at all was used ("c->m" = -1,
		 *   "c->n" = 0)
		 */
		l0 = cdcompbis(c,(head)?(c->c_mode)|HEAD:c->c_mode,
		c->c_number,l0);
	return(l0);
}

/*********************************************\
* 					      *
*  Allocation of memory during the decoding   *
* 					      *
\*********************************************/

dcalloc(c,t,l0)
struct comp * c; int t,l0;
{	int i;

	for (i=c->c_first_son; i!= -1; i=complist[i].c_brother){
		l0 = argprint(&complist[i],0,l0);
		(void)printf("= (%s *) asn1_malloc ((unsigned)(",
		complist[i].c_index);
		switch (t){
		case T_BITS:
			/* Horrible patch for MAVCOD */
			if (strcmp(complist[i].c_index,"asn1_field") == 0)
				goto standard;
			complist[c->c_father].c_type = T_BITS_ALLOC;
			goto field;
		case T_OCTETS:
			/* Horrible patch for MAVCOD */
			if (strcmp(complist[i].c_index,"asn1_field") == 0)
				goto standard;
			complist[c->c_father].c_type = T_OCTETS_ALLOC;
			goto field;
		case T_OID:
			/* Horrible patch for MAVCOD */
			if (strcmp(complist[i].c_index,"asn1_field") == 0)
				goto standard;
			complist[c->c_father].c_type = T_OID_ALLOC;
			goto field;
		case T_ANY:
			/* Horrible patch for MAVCOD */
			if (strcmp(complist[i].c_index,"asn1_field") == 0)
				goto standard;
			complist[c->c_father].c_type = T_ANY_ALLOC;
			goto field;
		case T_COPY:
			/* Horrible patch for MAVCOD */
			if (strcmp(complist[i].c_index,"asn1_field") == 0)
				goto standard;
			complist[c->c_father].c_type = T_COPY_ALLOC;
			goto field;
		case T_BITS_ALLOC:
		case T_OCTETS_ALLOC:
		case T_OID_ALLOC:
		case T_ANY_ALLOC:
		case T_COPY_ALLOC:
		case T_CHARS:
		field:
			(void)printf("asn1_size(asn1z,asn1zm)));\n"); 
			break;
		case T_SEQUENCE_OF:
		case T_SET_OF:
			(void)printf(
	"sizeof(%s)*(asn1_number(asn1_skiptype(asn1z,asn1zm),asn1zm)+1)));\n",
			complist[i].c_index); break;
		default:
		/* Note that we add a label for the MAVCOD related patch */
		standard:
			(void)printf("sizeof(%s)));\n",complist[i].c_index); break;
		}
		l0++;
	}
	return(l0);
}

/****************************\
* 			     *
* The main decoding switch   *
* 			     *
\****************************/

dccompbis(c,m,n,l0,unsafe)
struct comp * c;
int m,n,l0,unsafe;
{	int i;
	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		if (complist[i].c_type == T_ALLOC)
			l0 = dcalloc(&complist[i],c->c_type,l0);
		if (complist[i].c_type == T_INIT)  
			l0 = dcinit(&complist[i],l0);
	}
	switch(c->c_type){
	case T_OCTETS:
		l0 = octetsdec(c,l0); break;
	case T_OCTETS_ALLOC:
		l0 = octetsalldec(c,l0); break;
	case T_OID:
		l0 = octetsdec(c,l0); break;
	case T_OID_ALLOC:
		l0 = octetsalldec(c,l0); break;
	case T_FLAGS:
		l0 = flagdec(c,l0); break;
	case T_BITS:
		l0 = bitdec(c,l0); break;
	case T_BITS_ALLOC:
		l0 = bitalldec(c,l0); break;
	case T_CHARS:
		l0 = charsdec(c,l0); break;
	case T_ANY:
	case T_COPY:
		l0 = anydec(c,l0); break;
	case T_ANYDEF:
		l0 = cd_anydef(c,l0,DECODE); break;
	case T_ANY_ALLOC:
	case T_COPY_ALLOC:
		l0 = anyalldec(c,l0); break;
	case T_INTEGER:
		l0 = integerdec(c,m,n,l0,unsafe); break;
	case T_SEQUENCE:
		l0 = seqdec(c,m,n,l0,unsafe); break;
	case T_SET:
		l0 = setdec(c,m,n,l0,unsafe); break;
	case T_CHOICE: l0 = choicedec(c,l0,unsafe); break;
	case T_IGNORE:	
		(void)printf("asn1z = asn1_skip(asn1z,asn1zm);\n");l0++; 
		break;
	case T_SEQUENCE_OF:
		l0 = arraydec(c,m,n,l0,unsafe); break;
	case T_SET_OF:
		l0 = arraydec(c,m,n,l0,unsafe); break;
	case T_BOOLEAN:
		l0 = booleandec(c,m,n,l0,unsafe); break;
	case T_EXTERNAL:
		l0 = externdec(c,l0); break;
	case T_REAL:
		l0 = realdec(c,m,n,l0,unsafe); break;
	case T_NULL:
		l0 = nulldec(m,n,l0,unsafe); break;
	case T_EXTERN:	
	default:
		l0 = cdextern(c,m,n,l0,DECODE); break;
	}
	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		if (complist[i].c_type == T_FINAL)
			l0 = dcinit(&complist[i],l0);
	}
	return(l0);
}

dccomp(c, l0, unsafe)
struct comp * c; 
int l0,unsafe;
{	int m, n;
#ifdef DEBUG
if (debuglevel > 6)
(void)printf ("/* decoding %d (%d:%s) */\n",(c - complist),c->c_type,c->c_name);
#endif
	if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
	if (c->c_implicit == 0 && c->c_mode != -1){
		l0 += asn1open(c->c_mode,c->c_number,1,unsafe);
		if (c->c_u_tag == -1){
			m = -1; n = 0;
		}else{	m = 0; n = c->c_u_tag;}
		l0 = dccompbis(c,m,n,l0,1);
		l0 += asn1close();
	}else	l0 = dccompbis(c,c->c_mode,c->c_number,l0,unsafe);
	return(l0);
}

/****************************************************************************\
* 									     *
* In case of decoding errors:						     *
* 									     *
* The decoding can indeed not continue, and a null pointer will be 	     *
* returned.  But some memory allocation was already performed. The tough     *
* fact is that, in C, there is no system independant ``garbage collector''.  *
* Hence, there is a risk to fill up the memory with partially decoded	     *
* messages. The proposed solution is to enable the user to invoke the	     *
* standard ``XXX_free'' procedure. It would fail -- and probably trigger     *
* some lethal memory error -- if we had left the message ``partially	     *
* allocated''; thus the ``decoding in error mode'' will perform all	     *
* initialisations and memory allocations which are required, keeping them    *
* indeed to a minimum.							     *
* 									     *
\****************************************************************************/
erralloc(c,t,l0)
struct comp * c; int t,l0;
{	int i;

	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		l0 = argprint(&complist[i],0,l0);
		(void)printf("= (%s *) asn1_malloc (",complist[i].c_index);
		switch (t){
		case T_BITS:
		case T_BITS_ALLOC:
		case T_OCTETS:
		case T_OCTETS_ALLOC:
		case T_OID:
		case T_OID_ALLOC:
		case T_ANY:
		case T_ANY_ALLOC:
		case T_COPY:
		case T_COPY_ALLOC:
		case T_CHARS:
			(void)printf("1);\n"); break;
		case T_SEQUENCE_OF:
		case T_SET_OF:
		default:
			(void)printf("sizeof(%s));\n",complist[i].c_index); break;
		}
		l0++;
	}
	return(l0);
}

errcomp(c,l0)
struct comp * c;
int l0;
{	int i;
	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		if (complist[i].c_type == T_ALLOC)
			l0 = erralloc(&complist[i],c->c_type,l0);
		if (complist[i].c_type == T_INIT)  
			l0 = dcinit(&complist[i],l0);
	}
	switch(c->c_type){
	case T_OCTETS:
	case T_OCTETS_ALLOC:
	case T_OID:
	case T_OID_ALLOC:
	case T_FLAGS:
	case T_BITS:
	case T_BITS_ALLOC:
	case T_CHARS:
	case T_ANY:
	case T_COPY:
	case T_COPY_ALLOC:
	case T_ANY_ALLOC:
	case T_INTEGER:
	case T_IGNORE:	
	case T_BOOLEAN:
	case T_NULL:
	case T_REAL:
		l0 = cdbasic(c,-1,0,l0,ERRCOD,0);
		break;
	case T_ANYDEF:
		l0 = cd_anydef(c,l0,ERRCOD); break;
	case T_EXTERNAL:
		l0 = cdbasic(c,-1,0,l0,ERRCOD,0);
		break;
	case T_SEQUENCE:
	case T_SET:
		l0 = seqerr(c,l0); break;
	case T_CHOICE: 
		l0 = errchoice(c,l0); break;
	case T_SEQUENCE_OF:
	case T_SET_OF:
		l0 = errarray(c,l0); break;
	case T_EXTERN:	
		l0 = cdextern(c,-1,0,l0,ERRCOD); break;
	default:
		break;
	}
	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		if (complist[i].c_type == T_FINAL)
			l0 = dcinit(&complist[i],l0);
	}
	return(l0);
}

/**********************************************\
* 					       *
* Procedures for the generation of ``_free''   *
* 					       *
\**********************************************/

fralloc(c,l0)
struct comp * c; 
int l0;
{	int i;

	for (i=c->c_first_son; i!=-1; i=complist[i].c_brother){
		(void)printf("asn1_free(");
		l0 = argprint(&complist[i],0,l0);
		(void)printf(");\n"); l0++;}
	return(l0);
}

fr_something_to_free(c)
struct comp * c;
{       int i;

	switch(c->c_type){
	case T_EXTERN:	
	case T_COMP:
	case T_EXTERNAL:
	case T_OID:
	case T_COPY:
	case T_ANY:
	case T_BITS:
	case T_OCTETS:
	case T_CHARS:
	case T_ALLOC:
		return(-1);
	default:
		break;
	}
        for (i=c->c_first_son; i!= -1; i=complist[i].c_brother){
		if (fr_something_to_free(&complist[i]))
			return(-1);
	}
	return(0);
}

frcomp(c,l0)
struct comp * c;
int l0;
{
	int i, t, openchoice = 1;

#ifdef DEBUG
if (debuglevel > 6){
(void)printf("/* c at %x */\n", c);
(void)fflush(stdout);
(void)printf ("/* free of %d (%d:%s) */\n",(c - complist),c->c_type,c->c_name);
(void)fflush(stdout);
}
#endif
	switch(c->c_type){
	case T_SET:
	case T_SEQUENCE:
		if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
		for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
			if (complist[i].c_type == T_INIT
			||  complist[i].c_type == T_FINAL
			||  complist[i].c_type == T_DEFAULT
			||  complist[i].c_type == T_ALLOC) break;
			if (fr_something_to_free(&complist[i])==0)
				continue;
			l0 = defaulttest(&complist[i],l0,&t,0);
			l0 = frcomp(&complist[i],l0);
			if(t)	(void)printf("}\n");
		}
		break;
	case T_CHOICE:
		if (fr_something_to_free(c) == 0)
			break;
		if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
		(void)printf("switch (CAST_FOR_SWITCH(");
		l0 = argprint(&complist[c->c_first_son],0,l0);
		(void)printf(")){\n"); l0++;
		for (i=complist[c->c_first_son].c_brother; 
		i != -1; i = complist[i].c_brother){
			if (complist[i].c_type == T_INIT
			||  complist[i].c_type == T_FINAL
			||  complist[i].c_type == T_DEFAULT
			||  complist[i].c_type == T_ALLOC) break;
			if (l0 != complist[i].c_line_num)
				numberline(l0 = complist[i].c_line_num);
			if (complist[i].c_index == 0)
				(void)printf("default: ");
			else	(void)printf("case %s: ",complist[i].c_index);
			l0 = frcomp(&complist[i],l0);
			if (complist[i].c_index == 0){
				openchoice = 0;
				(void)printf("break;}\n"); l0++; return(l0);
			}else{	(void)printf("break;\n"); l0++;}
		}
		if (openchoice){	(void)printf("default:break;}\n"); l0++;}
		break;
	case T_SET_OF:
	case T_SEQUENCE_OF:
		if (fr_something_to_free(c) == 0)
			break;
		if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
		(void)printf("for (%s=0; %s <",c->c_name,c->c_name);
		l0 = argprint(
		&complist[complist[c->c_first_son].c_brother],0,l0);
		(void)printf("; %s++){\n",c->c_name); l0++;
		l0 = frcomp(&complist[c->c_first_son],l0);
		(void)printf("}\n"); l0++;
		break;
	case T_ANYDEF:
		break;
	case T_EXTERN:	
	case T_COMP:
		l0 = cdextern(c,0,0,l0,FREE); break;
	case T_EXTERNAL:
	case T_OID:
	case T_COPY:
	case T_ANY:
	case T_BITS:
	case T_OCTETS:
	case T_CHARS:
		for (i=c->c_first_son; i!= -1; i=complist[i].c_brother)
			if (complist[i].c_type == T_ALLOC) break;
		if (i == -1)
	                l0 = cdbasic(c,0,0,l0,FREE,0);
	default:
		break;
	}
	for (i=c->c_first_son; i!= -1; i=complist[i].c_brother){
		if (complist[i].c_type == T_ALLOC)
			l0 = fralloc(&complist[i],l0);
	}
#ifdef DEBUG
if (debuglevel > 6)
(void)printf ("/* end of free of %d (%d:%s) */\n",(c - complist),c->c_type,c->c_name);
#endif
	return(l0);
}

anydef_found(c)
struct comp * c;
{	int i;

	if (c->c_type == T_ANYDEF)
		return(-1);
	for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
		if (anydef_found(&complist[i]))
			return(-1);
	}
	return(0);
}

free_anydef(c,l0)
struct comp * c;
int l0;
{	int i, t, openchoice = 1;

	if (c->c_type == T_ANYDEF){
	}
	switch(c->c_type){
	case T_SET:
	case T_SEQUENCE:
		if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
		for (i=c->c_first_son; i != -1; i = complist[i].c_brother){
			if (complist[i].c_type == T_INIT
			||  complist[i].c_type == T_FINAL
			||  complist[i].c_type == T_DEFAULT
			||  complist[i].c_type == T_ALLOC) break;
			if (anydef_found(&complist[i]) == 0)
				continue;
			l0 = defaulttest(&complist[i],l0,&t,0);
			l0 = free_anydef(&complist[i],l0);
			if(t)	(void)printf("}\n");
		}
		break;
	case T_CHOICE:
		if (anydef_found(c) == 0)
			break;
		if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
		(void)printf("switch (CAST_FOR_SWITCH(");
		l0 = argprint(&complist[c->c_first_son],0,l0);
		(void)printf(")){\n"); l0++;
		for (i=complist[c->c_first_son].c_brother; 
		i != -1; i = complist[i].c_brother){
			if (complist[i].c_type == T_INIT
			||  complist[i].c_type == T_FINAL
			||  complist[i].c_type == T_DEFAULT
			||  complist[i].c_type == T_ALLOC) break;
			if (l0 != complist[i].c_line_num)
				numberline(l0 = complist[i].c_line_num);
			if (complist[i].c_index == 0)
				(void)printf("default: ");
			else	(void)printf("case %s: ",complist[i].c_index);
			l0 = free_anydef(&complist[i],l0);
			if (complist[i].c_index == 0){
				openchoice = 0;
				(void)printf("break;}\n"); l0++; return(l0);
			}else{	(void)printf("break;\n"); l0++;}
		}
		if (openchoice){	(void)printf("default:break;}\n"); l0++;}
		break;
	case T_SET_OF:
	case T_SEQUENCE_OF:
		if (anydef_found(c) == 0)
			break;
		if (l0 != c->c_line_num) numberline(l0 = c->c_line_num);
		(void)printf("for (%s=0; %s <",c->c_name,c->c_name);
		l0 = argprint(
		&complist[complist[c->c_first_son].c_brother],0,l0);
		(void)printf("; %s++){\n",c->c_name); l0++;
		l0 = free_anydef(&complist[c->c_first_son],l0);
		(void)printf("}\n"); l0++;
		break;
	case T_ANYDEF:
		l0 = cd_anydef(c,l0,FREE);
		break;
	default:
		break;
	}
	return(l0);
}

/***************************************************************************\
* 									    *
* New generation of the ``one type'' routine -- when a routine		    *
* can be used as ``components''.					    *
* 									    *
* If the tagged type is either "SEQUENCE" or "SET", we will generate	    *
* the ``components coder'', and call it from the inside. For the other	    *
* types, we will simply generate the coding routine.			    *
* 									    *
* If the tagged type is "SEQUENCE", we will test that all components	    *
* have different tags. If this is the case, we will generate the same	    *
* routines as for "sets". Otherwise, we will simply generate the "sequence  *
* component" routine, and a "sequence decoder" that calls it.		    *
* 									    *
* If the tagged type is "SET", we will generate the "set component"	    *
* defaulting and decoding routine, a simple "sequence component" decoder    *
* that is derived from the SET decoder, and a decoding routine.		    *
* 									    *
* For the other types, we will implement a simple decoding routine.	    *
* 									    *
* For all types, we have to implement a "length" routine, and also a	    *
* "free" routine. These routines can be either implemented as macros or	    *
* as regular routines, based on a test: if the routine can be determined    *
* by a single instruction, without referring to a yet unknown type, then    *
* it is a macro.  This "macroing" is noted in the tree (CM variable of	    *
* the T_EXTERN component) so that the makefile is generated correctly.	    *
* 									    *
* Note that for the components which are not of type "CHOICE", "SET" or	    *
* "SEQUENCE", "SET" or "SEQUENCE" of, which are ``IMPLICIT'' by nature,	    *
* and which don't include an ALLOCATE or DEFAULT clause, we could also	    *
* implement coding and decoding by means of macros.			    *
* 									    *
* In the preceeding versions, the ``length'' and ``free'' routines	    *
* were put in separate files. It is more reasonable to encode ``length''    *
* with the ``coding'' and ``free'' with the decoding. This minimize the	    *
* number of files... an important aspect of performance optimization.	    *
* 									    *
\***************************************************************************/
onetype(c,l0)
struct comp * c;
int l0;
{	int i, first_comp, set_comp = 0, sc=0;
	int dont_decode, dont_free, dont_errcod, dont_encode, dont_length;

	if (c->c_call_mode == DEFINE_MODE) return(l0);

	storeindex = 0; sc = 0;
	(void) get_depth(c,0,&sc);
	current_list_head = c->c_first_son;
	for (first_comp= c->c_first_son;first_comp != -1;
	first_comp = complist[first_comp].c_brother)
		if (complist[first_comp].c_type != T_PARAMETER) break;
	set_comp = look_for_set_comp(first_comp);

	dont_decode = is_declared_procedure(c->c_name,DECODE);
	dont_free   = is_declared_procedure(c->c_name,FREE);
	dont_errcod = is_declared_procedure(c->c_name,ERRCOD);

	if (dont_decode && dont_free && dont_errcod)
		goto coding_encode_proc;
#ifdef DEBUG
	if (debuglevel > 7)
	(void)fprintf(stderr,"Coding %s\n",c->c_name);
#endif

	if (dont_decode)
		goto coding_free_proc;

	/* Decoding file */
	if ((l0 = open_module_file(c->c_name,DECODE,l0)) < 0)
		return(-1);

	current_call_mode = 1;

	numberline(l0 = c->c_line_num);
	if (c->c_call_mode == SEQUENCE_MODE){
		/* Sequence components decoding */
		l0 = decl_proc_head(c,l0,C_SEQ_D);
		(void) printf("{\n");
		l0++;
		l0 += decl_var_dec(set_comp, storeindex, storelist,sc);
		for (i= complist[first_comp].c_first_son;
		i!= -1; i=complist[i].c_brother){
			if (complist[i].c_type == T_ALLOC)
				l0 = dcalloc(&complist[i],
				complist[first_comp].c_type,l0);
			if (complist[i].c_type == T_INIT) 
				l0 = dcinit(&complist[i],l0);
		}
		l0 = seqbodydec(&complist[first_comp],l0);
		for (i= complist[first_comp].c_first_son;
		i!= -1; i=complist[i].c_brother){
			if (complist[i].c_type == T_FINAL)
				l0 = dcinit(&complist[i],l0);
		}
		(void)printf("return(asn1z);}\n"); l0++;
		/* Sequence decoding */
		l0 = decl_proc_head(c,l0,DECODE);
		(void)printf("{\n");
		l0 += asn1open(-1,0,1,1);
		if (complist[first_comp].c_implicit == 0){
			l0 += asn1open(0,16,1,1);}
		/* Calling the procedure */
		(void)printf("asn1z = ");
		l0 = proc_headline(c,l0,C_SEQ_D);(void)printf(";\n"); l0++;
		(void)printf("while("); asn1inside(); (void)printf(")");
		(void)printf("asn1z = asn1_skip(asn1z,asn1zm);\n"); l0++;
		l0 += asn1close();
		if (complist[first_comp].c_implicit == 0){
			l0 += asn1close();}
		(void)printf("return(asn1z);}\n"); l0++;
	}else{
		/* Standard decoding */
		l0 = decl_proc_head(c,l0,DECODE);
		(void)printf("{\n"); l0++;
		l0 += decl_var_dec(set_comp, storeindex, storelist,sc);
		l0 = dccomp(&complist[first_comp],l0,1);
		(void)printf("return(asn1z);}\n"); l0++;
	}

	/* Free */
coding_free_proc:
	if (dont_free)
		goto coding_err_proc;
	numberline(l0 = c->c_line_num);
	current_call_mode = 0;
	l0 = decl_proc_head(c,l0,FREE);
	if (storeindex > 0){
		(void)printf("{ int");
		for (i=0; i<storeindex; i++) 
			(void)printf("%c%s",(i)?',':' ',storelist[i]);
		(void)printf(";\n");
	}else (void)printf("{\n");
	l0++;
	l0 = free_anydef(&complist[first_comp],l0);
	l0 = frcomp(&complist[first_comp],l0);
	(void)printf("return;}\n"); l0++;

	/* Error decoding */
coding_err_proc:
	if (dont_errcod)
		goto coding_length_proc;
	numberline(l0 = c->c_line_num);
	current_call_mode = 1;
	l0 = decl_proc_head(c,l0,ERRCOD);
	(void)printf("{\n"); l0++;
	l0 = errcomp(&complist[first_comp],l0);
	(void)printf("return;}\n"); l0++;

coding_encode_proc:
	dont_encode = is_declared_procedure(c->c_name,ENCODE);
	dont_length = is_declared_procedure(c->c_name,LENGTH);

	if (dont_encode && dont_length)
		return(l0);
	/* The coding file */
	if (c->c_call_mode == HALF_MODE) return(l0);
	if ((l0 = open_module_file(c->c_name,ENCODE,l0)) < 0)
		return(-1);
	current_call_mode = 0;
	if (dont_encode)
		goto coding_length_proc;
	numberline(l0 = c->c_line_num);
	if (c->c_call_mode == SET_MODE || c->c_call_mode == SEQUENCE_MODE){
		/* component coding routine */
		l0 = decl_proc_head(c,l0,COMPCOD);
		(void)printf("{\n");l0++;
		l0 += decl_var_cod(storeindex,storelist);
		l0 = seqbodycod(&complist[first_comp],l0);
		(void)printf("return(asn1z);}\n"); l0++;
		/* type coding calls the component coding */
		l0 = decl_proc_head(c,l0,ENCODE);
		(void) printf("{\n");
		(void)printf("int asn1_x509;\n");
		(void)printf("if (asn1_x509 = (asn1_n & ASN1_X509)){\n");
		(void)printf("\tasn1_n &= ~ASN1_X509;}\n");
		l0 += 4;
		l0 += openasncod(complist[first_comp].c_mode|HEAD,
		complist[first_comp].c_number,1);
		if (complist[first_comp].c_implicit == 0){
			l0 += openasncod(0,
			(complist[first_comp].c_type == T_SET)?17:16,1);
		}
		/* Calling the procedure */
		(void)printf("asn1z = ");
		l0 = proc_headline(c,l0,COMPCOD); (void)printf(";\n"); l0++;
		if (c->c_call_mode == SET_MODE)
			l0 += closeasnset();
		else	l0 += closeasncod();
		if (complist[first_comp].c_implicit == 0)
			l0 += closeasncod();
		(void)printf("return(asn1z);}\n");
	}else{
		/* type coding is straight */
		l0 = decl_proc_head(c,l0,ENCODE);
		(void) printf("{\n");
		(void)printf("int asn1_x509;\n");
		l0 += 2;
		l0 += decl_var_cod(storeindex,storelist);
		(void)printf("if (asn1_x509 = (asn1_n & ASN1_X509)){\n");
		(void)printf("\tasn1_n &= ~ASN1_X509;}\n");
		l0 += 2;
		l0 = cdcomp(&complist[first_comp],l0,1);
		(void)printf("return(asn1z);}\n"); l0++;
	}
coding_length_proc:
	if (dont_length)
		return(l0);
	/* Length */
	numberline(l0 = c->c_line_num);
	current_call_mode = 0;
	l0 = decl_proc_head(c,l0,LENGTH);
	(void)printf("{ int asn1l = 0");
	for (i=0; i<storeindex; i++) (void)printf(",%s",storelist[i]);
	(void)printf(";\n");l0++;
	l0 = lncomp(&complist[first_comp],l0);
	(void)printf("return(asn1l);}\n");
	l0++;
	return(l0);
}

look_for_set_comp(i0)
int i0;
{	int i, t = (complist[i0].c_type == T_SET);
	int tany = (complist[i0].c_type == T_ANY
		||  complist[i0].c_type == T_ANY_ALLOC
		||  complist[i0].c_type == T_ANYDEF
                ||  complist[i0].c_type == T_COPY
                ||  complist[i0].c_type == T_COPY_ALLOC/*AF + COPY, COPY_ALLOC*/
                   );

	for(i = complist[i0].c_first_son; i != -1; i = complist[i].c_brother){
		if (tany && complist[i].c_type == T_ALLOC) return(1);
		if (t && complist[i].c_type == T_COMP) return(1);
		if (look_for_set_comp(i)) return(1);
	}
	return(0);
}

decl_var_dec(set_comp,stindex,stlist,setnb)
int set_comp, stindex, setnb;
char ** stlist;
{	int i, l0 = 0;

	if (set_comp)
		(void)printf("asn1 asn1z_0;\n");
	if (setnb){
		(void)printf("char asn1set[%d];\n", setnb);
		(void)printf("int asn1setc=0;\n");
		l0+=2;
	}
	if (stindex){
		for (i=0; i<stindex; i++) 
			(void)printf("%s%s",(i)?" ,":"int ",stlist[i]);
		(void)printf(";\n");
		l0++;
	}
	return(l0);
}

decl_var_cod(stindex,stlist)
int stindex;
char ** stlist;
{	int i, l0=0;

	/* Header of the ``coding'' routine */
	if (stindex){
		for (i=0; i<stindex; i++) 
			(void)printf("%s%s",(i)?" ,":"int ",stlist[i]);
		(void)printf(";\n");l0++;
	}
	return(l0);
}

/***********************\
* 		        *
* Browsing the tree !   *
* 		        *
\***********************/

genercode()
{	int i, l0=0;

	for (i=0; i < compindex; i++){
		switch (complist[i].c_type){
		case T_TYPE:
			if (complist[i].c_optional == 0 )
				if ((l0 = onetype(&complist[i],l0)) == -1)
					return(-1);
			break;
		default:
			break;
		}
	}
	return(0);
}
