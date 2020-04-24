%{
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: csrclex.l,v $
 * Revision 1.1.4.2  1996/03/09  23:25:41  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:41:22  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:11:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:55  root]
 * 
 * Revision 1.1.2.3  1994/07/27  21:37:24  mori_m
 * 	CR: 11341 Use dce_strdup.
 * 	[1994/07/27  21:32:56  mori_m]
 * 
 * Revision 1.1.2.2  1994/06/10  20:53:31  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:42  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/08  21:47:44  mori_m
 * 	CR 9702:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:47:34  mori_m]
 * 
 * $EndLog$
 */

#include <csrc.h>
#include <csrcgram.h>
#include <errno.h>
#include <dcecsrmsg.h>

#if	defined(YYLMAX)
#undef YYLMAX
#define YYLMAX  1024
#endif	/* defined(YYLMAX) */


static const char	*yyFilename;	/* Input file name      */
static int		yyLine;		/* Input line counter   */
static char		*temp;

static char	*FixString(char *);
%}
%%

rgy_value	{return tCodeSet; }
char_values	{return tCharSets; }
loc_name	{return tCodeName; }
max_bytes	{return tMaxBytes; }
start		{return tSTART; }
end		{return tEND; }
"{"		{return tSTART; }
"}"		{return tEND; }

0[Xx][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]	{
			int	c;
			sscanf((char *)yytext, "%x", &yylval.Number);
			c = input();
			if (c == ':')
			{
				unput(c);
				return tIntArray;
			}
			else
			{
				unput(c);
				return tNumber;
			}
		}

0[Xx][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]+	{
			sscanf((char *)yytext, "%x", &yylval.LongNumber);
			return tLongNumber;
		}

:0[Xx][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]	{
			temp = (char *)yytext;
			sscanf((char *)++temp, "%x", &yylval.Number);
			return tIntArray;
		}

[0-9]*		{
			yylval.Number = atoi((char *)yytext);
			return tNumber;
		}

[-+0-9A-Za-z_]+	{
			yylval.String = (char *)dce_strdup((char *)yytext);
			return tName;
		}

\"[^"]*		{
			/* Quoted string. */
			int	c;

			/* See the Lex paper in Volume 2A or PS1:16 for
			** the details on this code
			*/

			if (yytext[yyleng - 1] == '\\')
			{
				yymore();
			}
			else
			{
				c = input();
				if (c == '"')
				{
					c = yytext[yyleng];
					yytext[yyleng] = '\0';
					yylval.String = FixString((char *)yytext + 1);
					yylval.String = (char *)dce_strdup(yylval.String);
					yytext[yyleng] = c;
					return tName;
				}
				unput(c);
				yyerror("Bad String");
			}
		}

#[^\n]*		{
			/* Comment */
		}

description[^\n]* {
			if (yytext[yyleng - 1] == '\\')
			{
				yymore();
			}
		}

\n		{
			/* Newline; just count it. */
			yyLine++;
		}

[ \t]		{
			/* Spaces */
		}

.		{
			/* Random character */
			return *yytext;
		}

%%


#if     !defined(yywrap)
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
#endif  /* !defined(yywrap) */

/*
**  Fix up a quoted string by stripping leading and trailing newlines
**  and handling backslash escapes.
*/
static char *
FixString(
	char	*text
)
{
#if 0
	static char	D8[] = "01234567";
	char		*op;
	int		i;
#endif	/* 0 */
	char		*p;
	char		*q;

	while (*text == '\n')
		text++;
	for (q = text + strlen(text); q > text && q[-1] == '\n'; )
		*--q = '\0';
	for (p = q = text; (*q = *p++) != '\0'; )
		if (*q++ == '\n')
	for (/*yyLine++*/; *p && (*p == ' ' || *p == '\t'); p++)
		continue;

#if 0
	for (p = q = text; *q; q++, p++) {
		if (*q != '\\') {
			*p = *q;
			continue;
		}
		switch (*++q) {
		default:			/* Undefined; ignore it */
		case '\'': case '\\': case '"': case '?':
			*p = *q;
			break;
		case 'a':	*p = '\007';	break;	/* Alert	*/
		case 'b':	*p = '\b';	break;	/* Backspace	*/
		case 'f':	*p = '\f';	break;	/* Form feed	*/
		case 'n':	*p = '\n';	break;	/* New line	*/
		case 'r':	*p = '\r';	break;	/* Carriage return */
		case 't':	*p = '\t';	break;	/* Horizontal tab  */
		case 'v':	*p = '\n';	break;	/* Vertical tab	*/
		case '0': case '1': case '2': case '3':
		case '4': case '5': case '6': case '7':
			*p = 0;
			for (i = 0; (op = strchr(D8, *q)) != NULL && i < 3; i++, q++)
				*p = (*p << 3) + op - D8;
			q--;
			break;
		}
	}
	*p = '\0';
#endif  /* 0 */
	return text;
}

/*
**  Write an error message.
*/
int
yyerror(
	char	*p
)
{
	char	buff[256];
	char	c;

	c = yytext[yyleng];
	yytext[yyleng] = '\0';
	(void)strncpy(buff, (char *)yytext, sizeof buff);
	buff[sizeof buff - 1] = '\0';
	yytext[yyleng] = c;
	(void)dce_fprintf(stderr, csrc_s_lex_error,
		yyFilename, yyLine + 1, p, buff);
	csrc_g_errors++;
}

/*
**  Write an error message that takes a "%s" argument.
*/
void
y2error(
	char	*fmt,
	char	*arg
)
{
	char	buff[256];

	(void)sprintf(buff, fmt, arg);
	yyerror(buff);
}

/*
**  Open the input, get ready to parse.
*/
void
yyopen(
	char	*name
)
{
	if ((yyin = fopen(name, "r")) == NULL) {
		(void)dce_fprintf(stderr, csrc_s_cannot_open_input,
			name, strerror(errno));
		exit(1);
	}
	yyLine = 0;
	yyFilename = name;
}
