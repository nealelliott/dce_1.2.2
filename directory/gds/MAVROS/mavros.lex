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
 * $Log: mavros.lex,v $
 * Revision 1.1.10.2  1996/03/09  23:12:29  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:29:40  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:09:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:01  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:38:54  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:03:18  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:01:52  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:06:49  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:11:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:54:33  bbelch]
 * 
 * Revision 1.1.2.4  1992/06/30  20:52:50  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:31:00  melman]
 * 
 * Revision 1.1.2.3  1992/06/02  17:32:41  melman
 * 	Added %{ and %} around copyright and history.
 * 	[1992/06/02  17:30:50  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  20:21:42  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:39:29  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: mavros.lex,v $ $Revision: 1.1.10.2 $ $Date: 1996/03/09 23:12:29 $";
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
DEFINED		{ return(DEFINED);}
BY		{ return(BY);}
UP		{ return(UP);}
TO		{ return(TO);}
ANY		{ return(ANY);}
BOOLEAN		{ return(BOOLEAN);}
OCTETS		{ return(OCTETS);}
BITS		{ return(X_BITS);}
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
STRING		{ return(STRING);}
IMPLICIT	{ return(IMPLICIT);}
OPTIONAL	{ return(OPTIONAL);}
DEFAULT		{ return(DEFAULT);}
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
FINAL		{ return(FINALVAR);}

\'[0-9A-Fa-f]*\'H	{
		yylval.var = malloc((unsigned)strlen(yytext)+1);
		(void)strcpy(yylval.var,yytext);
		return(HEXA_STRING);
		};

\'[01]*\'B	{
		yylval.var = malloc((unsigned)strlen(yytext)+1);
		(void)strcpy(yylval.var,yytext);
		return(BINARY_STRING);
		};

["]([^"]|["]["])*["]	{
		yylval.var = malloc((unsigned)strlen(yytext));
		sanitize_quoted_string(yylval.var,yytext);
		return(QUOTED_STRING);
		};

[-0-9][0-9]*	{
		if (yylval.var = malloc((unsigned)(strlen(yytext)+1))){
			(void)strcpy(yylval.var,yytext);
			return(NUMERIC);
		}else{
			memory_error();
		}
	};

[A-Za-z][_0-9A-Za-z]*	{
		if (yylval.var = malloc((unsigned)(strlen(yytext)+1))){
			(void)strcpy(yylval.var,yytext);
			return(IDENT);
		}else{
			memory_error();
		}
	};
[ \t]		;
[\n]	{linenum++;}
.		{return( (int)yytext[0]);};
