%{
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavcod.lex,v $
 * Revision 1.1.10.2  1996/03/09  23:12:26  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:29:37  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:08:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:49  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:37:25  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:02:17  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:55:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:05:43  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:11:02  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:54:08  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  17:55:19  marrek
 * 	November 1992 code drop
 * 	[1992/11/26  11:22:27  marrek]
 * 
 * Revision 1.1.2.4  1992/06/30  20:52:48  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:30:54  melman]
 * 
 * Revision 1.1.2.3  1992/06/02  17:32:39  melman
 * 	Added %{ and %} around copyright and history.
 * 	[1992/06/02  17:32:07  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  20:21:05  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:32:10  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavcod.lex,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:12:26 $";
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
%}

%{
%}
%p 5000
%a 3000
%%

--.*$		;

SEQUENCE	{ return(SEQUENCE);}
ENUMERATED	{ return(ENUMERATED);}
ENCRYPTED	{ return(ENCRYPTED);}
ALLOCATE	{ return(ALLOCATE);}
EXTERNAL	{ return(EXTERNAL);}
INTEGER		{ return(INTEGER);}
IGNORE		{ return(IGNORE);}
OF		{ return(OF);}
VAR		{ return(VAR);}
VALUE		{ return(VALUE);}
INDEXED		{ return(INDEXED);}
BY		{ return(BY);}
UP		{ return(UP);}
TO		{ return(TO);}
ANY		{ return(ANY);}
BOOLEAN		{ return(BOOLEAN);}
OCTET		{ return(OCTET);}
STRING		{ return(STRING);}
NumericString	{ return(NString);}
PrintableString	{ return(PString);}
TeletexString	{ return(TString);}
T61String	{ return(TString);}
VideotexString	{ return(VTString);}
VisibleString	{ return(ViString);}
ISO646String	{ return(ViString);}
IA5String	{ return(IA5String);}
GraphicString	{ return(GrString);}
GeneralString	{ return(GeString);}
CHARS		{ return(CHARS);}
NumericChars	{ return(NChars);}
PrintableChars	{ return(PChars);}
TeletexChars	{ return(TChars);}
T61Chars	{ return(TChars);}
VideotexChars	{ return(VTChars);}
VisibleChars	{ return(ViChars);}
ISO646Chars	{ return(ViChars);}
IA5Chars	{ return(IA5Chars);}
GraphicChars	{ return(GrChars);}
GeneralChars	{ return(GeChars);}
GeneralizedTime	{ return(GeTime);}
UniversalTime	{ return(UnTime);}
UTCTime		{ return(UnTime);}
ObjectDescriptor { return(ObDesc);}
SET		{ return(SET);}
IMPLICIT	{ return(IMPLICIT);}
OPTIONAL	{ return(OPTIONAL);}
DEFAULT		{ return(DEFAULT);}
DEFINED		{ return(DEFINED);}
UNIVERSAL	{ return(UNIVERSAL);}
APPLICATION	{ return(APPLICATION);}
CHOICE		{ return(CHOICE);}
TYPE		{ return(TYPE);}
NULL		{ return(NULLITEM);}
COPY		{ return(COPY);}
COMPONENTS	{ return(COMPONENTS);}
FLAGS		{ return(FLAGS);}
OBJECT		{ return(OBJECT);}
ID		{ return(ID);}
OCTET		{ return(OCTET);}
BIT		{ return(BIT);}
DEFINITIONS	{ return(DEFINITIONS);}
EXPLICIT	{ return(EXPLICIT);}
TAGS		{ return(TAGS);}
IMPORTS		{ return(IMPORTS);}
EXPORTS		{ return(EXPORTS);}
END		{ return(END);}
BEGIN		{ return(xBEGIN);}
FROM		{ return(FROM);}
INITIAL		{ return(INITVAR);}
REAL		{ return(REAL);}

[-0-9][0-9]*	{
		if (yylval.var = malloc((strlen((char *)yytext)+1))){
			(void)strcpy(yylval.var,(char *)yytext);
			return(NUMERIC);
		}else{
			memory_error();
		}
	};

[A-Za-z][\-_0-9A-Za-z]*   {
			  size_t i;
		if (yylval.var = malloc((strlen((char *)yytext)+1))){
			for (i=(size_t)0;i<strlen((char *)yytext);++i)
			   if (yytext[i]=='-') yytext[i]=(int)'_';
			(void)strcpy(yylval.var,(char *)yytext);
			return(IDENT);
		}else{
			memory_error();
		}
	};
[ \t]		;
[\n]	{linenum++;}
.		{return( (int)yytext[0]);};
