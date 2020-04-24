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
 * $Log: tsh.l,v $
 * Revision 1.1.11.2  1996/03/11  02:35:46  marty
 * 	Update OSF copyright years
 * 	[1996/03/10  20:04:09  marty]
 *
 * Revision 1.1.11.1  1995/12/11  21:50:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:20  root]
 * 
 * Revision 1.1.9.2  1992/12/31  17:35:12  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:59:29  htf]
 * 
 * Revision 1.1.2.3  1992/07/16  19:31:23  sekhar
 * 	OT 4727 : Enclose the copyright marker and the history within a pair of
 * 	          %{ , %} as required by lex syntax.
 * 	[1992/07/16  19:22:05  sekhar]
 * 
 * Revision 1.1.2.2  1992/07/14  21:14:59  sekhar
 * 	[CR4616] Always pick up the definition of malloc  from malloc.h
 * 	[1992/07/14  21:13:31  sekhar]
 * 
 * Revision 1.1  1992/01/19  04:07:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
%}
%{
/* tsh.l - lexical analysis for the tsh
 * 
 * Copyright 1989 by Apollo Computer, Inc.
 * Copyright 1990 by Locus Computing Corp.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 * 
 * @(#)tsh.l	1.13
 *  
 *	Extracted	1/7/91 10:19:07 
 * 	from delta on	11/8/90 16:17:15
 *
 */
#ifndef LINT
static char sccs_id[] = "@(#)tsh.l	1.13";
#endif
#include <tsh.h>
#include <malloc.h>
#include <y.tab.h>
extern char * 	getenv();
#ifdef LEXDEBUG
int	dbg=1;
char * token();
# define		RET(X) 		{if(dbg)fprintf(stderr,"lex %s [%s]\n",token(X),yytext);return(X);}
#else
# define		RET(X)		{return(X);}
#endif
extern char *	TshFileName;
void	retstring();
static char mybuf[YYLMAX];	/* work buffer 				*/
static char * myptr = mybuf;	/* work buffer ptr for echo and args 	*/
static char * litptr;		/* work buffer ptr for literals		*/
static char tshnull = '\0';
%}
%start KEY ARGS cmt literal echo str
%{
static int	oldstate=KEY;
%}
%%
	if(yybgin-yysvec-1 == 0)	/* voodoo */
		BEGIN KEY;

<KEY>"$"	{ oldstate = KEY; litptr = mybuf; BEGIN literal; }
<KEY>"#"	{ BEGIN cmt; }

<KEY>echo	{ 
			int	c;
			while((c = yyinput()) == ' ' || c == '\t');
			myptr = mybuf;
			unput(c);
			BEGIN echo; 
			RET(ECHOCMD);
		}

<KEY>run	{
			myptr = mybuf;
			BEGIN ARGS;
			RET(RUN);
		}
<KEY>remote	{
			myptr = mybuf;
			BEGIN ARGS;
			RET(REMOTE);
		}
<KEY>test	{
			myptr = mybuf;
			BEGIN ARGS;
			RET(TEST);
		}
<KEY>include	{
			myptr = mybuf;
			BEGIN ARGS;
			RET(INCLUDE);
		}
<KEY>execute	{
			myptr = mybuf;
			BEGIN ARGS;
			RET(EXECUTE);
		}
<KEY>set	{
			myptr = mybuf;
			BEGIN	ARGS;
			RET(SETCMD);
		}
<KEY>unset	{
			myptr = mybuf;
			BEGIN	ARGS;
			RET(UNSET);
		}
<KEY>read	{
			myptr = mybuf;
			BEGIN	ARGS;
			RET(READ);
		}
<KEY>pause	RET(PAUSE);
<KEY>summary	RET(SUMMARY);
<KEY>onerror	RET(ONERROR);
<KEY>stop	RET(STOP);
<KEY>subtotal	RET(SUBTOTAL);
<KEY>clear	RET(CLEAR);
<KEY>continue	RET(CONTINUE);
<KEY>default	RET(DEFAULT);
<KEY>"\n"	RET(CR);
<KEY>[ \t]+	;
<KEY>.		{
			int c;
			log("testsh:%s Unknown command on line %d\n",TshFileName,
			    yylineno);
			log(">>>%c",yytext[0]);
			while((c = yyinput()) != '\n')
				log("%c",c);
			log("<<<\n");
		}
<ARGS>"$"	{
			oldstate = ARGS;
			litptr = myptr;
			BEGIN literal;
		}
<ARGS>\\	{
			int	c;

			switch(c = yyinput()) {
			case '\n':	/* eat the newline */
				break;
			case '$':
			case '"':
			case '\\':
				*myptr++ = yytext[0];  /* add a backslash */
				break;
			default:
				unput(c);	/* send it back */
				break;
			}
		}
<ARGS>\"	{
			if(myptr != mybuf) {
				*myptr = '\0';
				retstring();
				myptr = mybuf;
				BEGIN str;
				RET(STRING);
			}
			BEGIN str;
		}
<ARGS>\n	{
			if(myptr != mybuf) {
				*myptr = '\0';
				retstring();
				BEGIN KEY;
				yyunput('\n');
				RET(STRING);
			}
			BEGIN KEY;
			RET(CR);
		}
<ARGS>[ \t]+	{
			if(myptr != mybuf) {
				*myptr = '\0';
				retstring();
				RET(STRING);
			}
		}
<ARGS>.		{ 
			*myptr++ = yytext[0]; 
		}
<cmt>.		;
<cmt>\n		{BEGIN KEY;}
<echo>\$	{ 
			oldstate = echo;
			litptr = myptr;
			BEGIN literal; 
		}
<echo>\\	{
			char	c;

			switch(c = yyinput()) {
			case '\\':
			case '$':	/* falls through */
				*myptr++ = c;
				break;
			default:
				unput(c);	/* send it back */
				break;
			}
		}

<echo>\n	{
			*myptr = '\0';
			BEGIN KEY;
			yylval.cp = mybuf;
			RET(ECHOSTR);
		}
<echo>.		{
			*myptr++ = yytext[0];
		}
<literal>[ \t\n]	{
			register char *ncp, *cp;
			*litptr = '\0';
			yyunput(yytext[0]);
			if(cp = getenv(myptr)) {
				ncp = cp + strlen(cp);
				while(ncp > cp) {
					yyunput(*(--ncp));
				}
			}
			else {
				log("testsh: Unable to translate literal '%s'\n",myptr);
			}
			BEGIN oldstate;
		}
<literal>.	{
			*litptr++ = yytext[0];
		}
<str>\"		{
			if(myptr != mybuf) {
				*myptr = '\0';
				retstring();
				myptr = mybuf;
				BEGIN ARGS;
				RET(STRING);
			}
			else {
				yylval.cp = &tshnull;
				BEGIN ARGS;
				RET(STRING);
			}
		}
<str>\n		{
			log("testsh:%s line %d newline in quoted string",
				TshFileName,yylineno);
			BEGIN KEY;
		}
<str>.		{
			*myptr++ = yytext[0];
		}

%%
#ifdef LEXDEBUG
extern struct tok { char * token; int	value; } yytoks[];

char *
token(val)
{
	register struct tok *tok;
	for (tok = yytoks;tok->value;tok++)
		if(tok->value == val)
			return(tok->token);
	return("Unknown");
}
#endif
void
retstring()
{
	if(yylval.cp = malloc((myptr-mybuf)+1)) {
		strcpy(yylval.cp,mybuf);
		myptr = mybuf;
	}
	else {
		log("testsh: Unable to allocate more memory goodbye\n");
		exit(-3);
	}
}
