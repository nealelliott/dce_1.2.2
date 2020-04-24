%{
/*
 *  @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: samslex.l,v $
 * Revision 1.1.6.2  1996/03/09  23:32:19  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:47:22  marty]
 *
 * Revision 1.1.6.1  1995/12/08  20:55:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:37  root]
 * 
 * Revision 1.1.4.6  1994/09/30  07:56:10  rsalz
 * 	Implement DCE RFC 24.2 (OT CR 11929)
 * 	[1994/09/30  07:44:22  rsalz]
 * 
 * Revision 1.1.4.5  1994/06/10  20:53:26  devsrc
 * 	cr10872- fix copyright
 * 	[1994/06/10  16:38:13  devsrc]
 * 
 * Revision 1.1.4.4  1994/05/06  19:45:06  rsalz
 * 	Increase message buffer size to NL_TEXTMAX (OT CR 10573)
 * 	[1994/05/06  19:40:57  rsalz]
 * 
 * Revision 1.1.4.3  1994/03/29  16:11:47  rsalz
 * 	Keep line numbers in sync (OT CR 10183).
 * 	[1994/03/29  16:08:24  rsalz]
 * 
 * Revision 1.1.4.2  1994/03/10  21:09:31  rsalz
 * 	Add collections and conditional text (OT CR 9697).
 * 	[1994/03/10  20:45:48  rsalz]
 * 
 * Revision 1.1.4.1  1993/12/28  16:56:35  rsalz
 * 	Allow octal and hex numbers, not just decimal.
 * 	[1993/12/28  16:56:26  rsalz]
 * 
 * Revision 1.1.2.2  1993/08/16  17:42:04  rsalz
 * 	Initial release
 * 	[1993/08/16  17:41:18  rsalz]
 * 
 * $EndLog$
 */
#include <sams.h>
#include <samsgram.h>


/*
**  I wish that all versions of lex had more buffer space by default.
*/
#if	!defined(NL_TEXTMAX)
#define NL_TEXTMAX	8192
#endif	/* defined(NL_TEXTMAX) */

#if	defined(YYLMAX)
#undef YYLMAX
#define YYLMAX	NL_TEXTMAX
#endif	/* defined(YYLMAX) */


static const char	*yyFilename;	/* Input file name	*/
static int		yyLine;		/* Input line counter	*/

static char	*FixString(char *);
%}
%%


action		{ return tACTION; /* NOTREACHED */ }
administrator	{ return tADMINISTRATOR; /* NOTREACHED */ }
attributes	{ return tATTRIBUTES; /* NOTREACHED */ }
code		{ return tCODE; /* NOTREACHED */ }
collection	{ return tCOLLECTION; /* NOTREACHED */ }
component	{ return tCOMPONENT; /* NOTREACHED */ }
default		{ return tDEFAULT; /* NOTREACHED */ }
end		{ return tEND; /* NOTREACHED */ }
engineer	{ return tENGINEER; /* NOTREACHED */ }
explanation	{ return tEXPLANATION; /* NOTREACHED */ }
handle		{ return tHANDLE; /* NOTREACHED */ }
not		{ return tNOT; /* NOTREACHED */ }
notes		{ return tNOTES; /* NOTREACHED */ }
operator	{ return tOPERATOR; /* NOTREACHED */ }
programmer	{ return tPROGRAMMER; /* NOTREACHED */ }
response	{ return tRESPONSE; /* NOTREACHED */ }
serviceability	{ return tSERVICEABILITY; /* NOTREACHED */ }
set		{ return tSET; /* NOTREACHED */ }
severity	{ return tSEVERITY; /* NOTREACHED */ }
size		{ return tSIZE; /* NOTREACHED */ }
start		{ return tSTART; /* NOTREACHED */ }
sub-component	{ return tSUBCOMP; /* NOTREACHED */ }
system		{ return tSYSTEM; /* NOTREACHED */ }
table		{ return tTABLE; /* NOTREACHED */ }
tables		{ return tTABLES; /* NOTREACHED */ }
technology	{ return tTECHNOLOGY; /* NOTREACHED */ }
text		{ return tTEXT; /* NOTREACHED */ }
user		{ return tUSER; /* NOTREACHED */ }
value		{ return tVALUE; /* NOTREACHED */ }
vendor		{ return tVENDOR; /* NOTREACHED */ }
"!"		{ return tNOT; /* NOTREACHED */ }

incatalog	{
		    yylval.Number = MTincatalog;
		    return tBIT;
		    /* NOTREACHED */
		}

intable		{
		    yylval.Number = MTintable;
		    return tBIT;
		    /* NOTREACHED */
		}

longtext	{
		    yylval.Number = MTlongtext;
		    return tBIT;
		    /* NOTREACHED */
		}

obsolete	{
		    yylval.Number = MThidden;
		    return tBIT;
		    /* NOTREACHED */
		}

reserved	{
		    yylval.Number = MThidden;
		    return tBIT;
		    /* NOTREACHED */
		}

undocumented	{
		    yylval.Number = MTnodoc;
		    return tBIT;
		    /* NOTREACHED */
		}

[0-9]+		{
		    char		*end;

		    yylval.Number = (int)strtol((char *)yytext, &end, 0);
		    if (*end)
			y2error("Illegal character %1.1s in number", end);
		    return tNUMBER;
		    /* NOTREACHED */
		}
0[xX][0-9a-fA-F]+	{
		    char		*end;

		    yylval.Number = (int)strtol((char *)yytext, &end, 0);
		    if (*end)
			y2error("Illegal character %1.1s in number", end);
		    return tNUMBER;
		    /* NOTREACHED */
		}

[-+0-9A-Za-z_]+ {
		    yylval.String = strdup((char *)yytext);
		    return tID;
		    /* NOTREACHED */
		}

\"[^"]*		{
		    /* Quoted string. */
		    int		c;

		    /* See the Lex paper in Volume 2A or PS1:16
		     * for details on this code. */
		    if (yytext[yyleng - 1] == '\\') {
			yymore();
		    }
		    else {
			c = input();
			if (c == '"') {
			    c = yytext[yyleng];
			    yytext[yyleng] = '\0';
			    yylval.String = FixString((char *)yytext + 1);
			    yylval.String = strdup(yylval.String);
			    yytext[yyleng] = c;
			    if (yyleng > NL_TEXTMAX - 2)
				yyerror("String too long");
			    return tID;
			}
			unput(c);
			yyerror("Bad string");
		    }
		}

#[^\n]*		{
		    /* Comment. */
		}

\n		{
		    /* Newline; just count it. */
		    yyLine++;
		}

[ \t]		{
		    /* Tasty whitespace. */
		}

.		{
		    /* Random character. */
		    return *yytext;
		    /* NOTREACHED */
		}

%%


#if	!defined(yywrap)
/*
**  Called by lex at end-of-stream.  Return one if no more input.
*/
int
yywrap(
    void
)
{
    return 1;
}
#endif	/* !defined(yywrap) */


/*
**  Fix up a quoted string by stripping leading and trailing newlines
**  and handling backslash escapes.
*/
static char *
FixString(
    char		*text
)
{
    char		*p;
    char		*q;

    for ( ; *text == '\n'; text++)
	yyLine++;
    for (q = text + strlen(text); q > text && q[-1] == '\n'; yyLine++)
	*--q = '\0';
    for (p = q = text; (*q = *p++) != '\0'; )
	if (*q++ == '\n')
	    for (yyLine++; *p && (*p == ' ' || *p == '\t'); p++)
		continue;

    return text;
}


/*
**  Write an error message.
*/
int
yyerror(
    char		*p
)
{
    char		buff[256];
    char		c;

    c = yytext[yyleng];
    yytext[yyleng] = '\0';
    (void)strncpy(buff, (char *)yytext, sizeof buff);
    buff[sizeof buff - 1] = '\0';
    yytext[yyleng] = c;
    (void)fprintf(stderr, "%s:%d:%s (near \"%s\")\n",
	    yyFilename, yyLine + 1, p, buff);
    Errors++;
}


/*
**  Write an error message that takes a "%s" argument.
*/
void
y2error(
    char		*fmt,
    char		*arg
)
{
    char		buff[256];

    (void)sprintf(buff, fmt, arg);
    yyerror(buff);
}


/*
**  Open the input, get ready to parse.
*/
void
yyopen(
    char		*name
)
{
    if ((yyin = fopen(name, "r")) == NULL) {
	(void)fprintf(stderr, "Can't open \"%s\" for input, %s\n",
		name, strerror(errno));
	exit(1);
    }
    yyLine = 0;
    yyFilename = name;
}
