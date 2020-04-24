/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acf.h,v $
 * Revision 1.1.15.2  1996/02/18  23:44:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:43:48  marty]
 *
 * Revision 1.1.15.1  1995/12/07  22:12:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/dat_xidl2/1  1995/11/17  17:01 UTC  dat
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/12/07  21:09:25  root]
 * 
 * Revision 1.1.2.1  1995/10/23  01:47:05  bfc
 * 	oct 95 idl drop
 * 	[1995/10/23  01:46:18  bfc]
 * 
 * 	may 95 idl drop
 * 	[1995/10/21  21:11:15  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:22:35  bfc]
 * 
 * Revision 1.1.4.3  1993/01/03  21:37:24  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  14:31:40  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  18:42:06  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  00:58:39  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/14  12:46:23  mishkin
 * 	     Added yymaxdepth (for HP-UX).
 * 	[1992/04/10  19:36:35  mishkin]
 * 
 * Revision 1.1  1992/01/19  03:01:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**
**  NAME:
**
**      acf.h
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Include file for ACF parser.
**
**  VERSION: DCE 1.0
**
*/


#ifndef ACF_H
#define ACF_H


/*
 *  The macros below redefine global names that are referenced by lex and/or
 *  yacc.  This allows multiple lex-and-yacc-based parsers to be present
 *  within the same program without multiply defined global names.
 *
 *  **NOTE**:  For portability, the list below must contain ALL lex/yacc
 *  function and variable names that are global in ONE OR MORE of the
 *  implementations of lex/yacc that are supported to build the IDL compiler.
 */
#define yyact       acf_yyact
#define yyback      acf_yyback
#define yybgin      acf_yybgin
#define yychar      acf_yychar
#define yychk       acf_yychk
#define yycrank     acf_yycrank
#define yydebug     acf_yydebug
#define yydef       acf_yydef
#define yyerrflag   acf_yyerrflag
#define yyestate    acf_yyestate
#define yyexca      acf_yyexca
#define yyextra     acf_yyextra
#define yyfnd       acf_yyfnd
#define yyin        acf_yyin
#define yyinput     acf_yyinput
#define yyleng      acf_yyleng
#define yylex       acf_yylex
#define yylineno    acf_yylineno
#define yylook      acf_yylook
#define yylsp       acf_yylsp
#define yylstate    acf_yylstate
#define yylval      acf_yylval
#define yymatch     acf_yymatch
#define yymorfg     acf_yymorfg
#define yynerrs     acf_yynerrs
#define yyolsp      acf_yyolsp
#define yyout       acf_yyout
#define yyoutput    acf_yyoutput
#define yypact      acf_yypact
#define yyparse     acf_yyparse
#define yypgo       acf_yypgo
#define yyprevious  acf_yyprevious
#define yyps        acf_yyps
#define yypv        acf_yypv
#define yypvt       acf_yypvt
#define yyr1        acf_yyr1
#define yyr2        acf_yyr2
#define yyreds      acf_yyreds
#define yys         acf_yys
#define yysbuf      acf_yysbuf
#define yysptr      acf_yysptr
#define yystate     acf_yystate
#define yysvec      acf_yysvec
#define yytchar     acf_yytchar
#define yytext      acf_yytext
#define yytmp       acf_yytmp
#define yytoks      acf_yytoks
#define yytop       acf_yytop
#define yyunput     acf_yyunput
#define yyv         acf_yyv
#define yyval       acf_yyval
#define yyvstop     acf_yyvstop
#define yywrap      acf_yywrap
#define yymaxdepth  act_yymaxdepth
#define yyposix_point	acf_yyposix_point
#define yynls16		acf_yynls16
#define yynls_wchar	acf_yynls_wchar
#define yylocale	acf_yylocale

#endif /* ACF_H */
