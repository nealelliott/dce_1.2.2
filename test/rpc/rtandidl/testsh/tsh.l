/*
*/
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
 * Revision 1.1.7.2  1996/03/11  02:28:06  marty
 * 	Update OSF copyright years
 * 	[1996/03/10  20:00:01  marty]
 *
 * Revision 1.1.7.1  1995/12/11  20:13:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:29  root]
 * 
 * Revision 1.1.5.4  1993/03/10  22:25:49  cuti
 * 	Add #ifndef, #ifdef to define TRUE and FALSE.
 * 	[1993/03/10  22:23:06  cuti]
 * 
 * Revision 1.1.5.3  1993/02/26  22:39:06  weisman
 * 	very major cleanup
 * 	[1993/02/26  22:16:06  weisman]
 * 
 * Revision 1.1.5.2  1993/01/13  19:51:20  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:18:43  bbelch]
 * 
 * $EndLog$
 */


#include	"tsh.h"
#include	"y.tab.h"
#include        <errno.h>
#include        <limits.h>
#include        <unistd.h>
#include        <stdio.h>

/* 
 * Debugging
 */

#ifndef TRUE
#define TRUE 1
#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0
#endif
int lex_debug_helper = 0;

extern searchlist_t	searcher;

static int lookup_reserved_word (char * arg_text);
static char * parse_string_token (char first_char);
static disable_keyword_recognition = FALSE;

/* 
 * Notes of STRING memory management.
 * 
 * The macro RETURN_STRING_TOKEN mallocs its result and does 
 * a strcpy into it.   This macro always returns a STRING 
 * token.  
 * 
 * The yacc actions are are responsible for deallocating 
 * these STRINGS when appropriate. 
 * 
 */

#define RETURN_PLAIN_TOKEN(token)  \
    return (token)

#define RETURN_STRING_TOKEN(token)  \
    {yylval.string_token_p = strdup (token); return (STRING); }

/* 
 * Some versions of lex define input and unput as macros, delete 
 * those here. 
 */

#if defined (input)
# undef input
#endif

#if defined (unput)
# undef unput
#endif

%}

%%

[ \t]+                  { /* Ignore unclaimed horizontal whitespace */ }

\\\n		        {
                          /* 
		           * If we're at a continuation and there 
		           * is nothing else on the line, discard it.
                           * here.
                           */
                        }

[^ \$\t\n\="]		{ 
                          /* 
                           * An unquoted string.  It may be an 
                           * unquoted string or keyword.  Lookup 
                           * the token in the symbol table and 
                           * return the corresponding token type. 
                           * Otherwise if not a keyword, return 
                           * an allocated string.     
                           */
    
                          int token_value;
			  char * token_string;

			  token_string = parse_string_token (yytext [0]);
                          token_value = lookup_reserved_word (token_string);

                          if (token_value != -1)
                          {
                              RETURN_PLAIN_TOKEN (token_value);
                          }
                          
                          else
                          {
                              RETURN_STRING_TOKEN (token_string); 
                          }
                        }

"\""                    { 
                          /* 
                           * Quoted string.
                           */
    
                           RETURN_STRING_TOKEN 
                               (parse_string_token (yytext [0]));
                        }

"\$"			{ RETURN_PLAIN_TOKEN (DOLLAR_SIGN); }

"\="			{ RETURN_PLAIN_TOKEN (EQUAL); }

"\n"			{ 
                          disable_keyword_recognition = FALSE;
                          RETURN_PLAIN_TOKEN (CR); 
                        }

%%

static char * parse_string_token (char first_char)
{
    /* 
     * Parse an unquoted or a quoted string.   Arg contains the 
     * first char which indicates quotedness or not.  
     * 
     * This routine maintains its own static text buffer and 
     * returns a pointer to that buffer.  The caller is 
     * responsible for copying data out of this buffer.  
     * (analogous to yytext). 
     */
    
#define STRING_BUFFER_BYTES YYLMAX
#define INSERT_CHAR(c) \
    {if (p == (string_token_buffer + sizeof (string_token_buffer))) \
      {yyerror ("string_token_buffer exhausted."); \
       exit (1); } \
     else *p++ = c;}
    
    static char string_token_buffer [STRING_BUFFER_BYTES];
    int quoted_string_mode;
    char *p;
    int done;
    char c;

    p = string_token_buffer;

    /* 
     * Special case incoming quoted string.
     */
    
    if (first_char == '\"')
    {
        quoted_string_mode = TRUE;
    }
    else
    {
        quoted_string_mode = FALSE;
        unput (first_char);             /* pickup in loop below */
    }

    /* 
     * Read input characters and place into buffer.  We stop when one of 
     * the following is true:
     * 
     * - end of line for unquoted strings
     * - whitespace for unquoted strings
     * - closing double quote for quoted strings
     * - end of file.
     * 
     * Errors:
     * - end of line in a quoted string
     * - invalid escape sequence
     * 
     * Special characters:  This routine recognizes the following escaped 
     * sequences:
     * \\  backslash
     * \$  dollar sign
     * \"  double quote
     * \=  equals sign
     * ..\ backslash on end of line = continuation line
     *
     */

    done = FALSE;

    while (! done)
    {

        /* 
         * Pickup next char.
         */

        c = input (c);

        /* 
         * Check for eof.
         */
        
        if (c == 0)
        {
            break;
        }
        
        /* 
         * Check for special chars.
         */
        
        switch (c)
        {
        case '\n':

            /* 
             * Newline seen.  Push it back into the input stream 
             * so the yacc grammar can see a <CR> token.
             */
            
            unput (c);
            done = TRUE;

            if (quoted_string_mode)
            {
                yyerror ("Unterminated quoted string");
            }

            break;

            /* 
             * Handle simple delimitors
             */

        case ' ':
        case '\t':
        case '$':
        case '=':

            if (quoted_string_mode)
            {
                INSERT_CHAR (c);
            }
            else                /* unquoted string mode */
            {
                unput (c);
                done = TRUE;
            }
            break;
            
        case '\"':

            if (quoted_string_mode)
            {
                done = TRUE;
            }
            else
            {
                /* 
                 * We are seeing an opening quote within an unquoted 
                 * string.  We'll terminate the unquoted string and push 
                 * the quote back.
                 */

                done = TRUE;
                unput (c);
            }
            break;
   
        /* 
         * Process escaped chars.
         */

        case '\\':
            {
                char escaped_char;

                escaped_char = input ();
            
                /* 
                 * examine the escaped char.
                 */

                if ((escaped_char == '\\') ||
                    (escaped_char == '$')  ||
                    (escaped_char == '\"') ||
                    (escaped_char == '='))
                {
                    INSERT_CHAR (escaped_char);
                }

                else
                if (escaped_char == '\n')
                {
                    done = TRUE;    /* force a logical break */
                }        

                else            /* unknown escape sequence */
                {
                    yyerror ("Unknown escape sequence");
                    done = TRUE;
                }
            }                   /* escaped char */
            break;
            
        /* 
         * Process any normal char.
         */

        default:
            INSERT_CHAR (c);
            break;
        }                       /* switch (c) */

    }                           /* while not done */
    
    /* 
     * Put in terminator and return.
     */

    INSERT_CHAR ('\0');

    if (lex_debug_helper)
    {
        printf ("Parsed string token: \"%s\"\n", string_token_buffer);
    }

    return string_token_buffer;
}                               /* parse_string_token */


static int lookup_reserved_word (char * arg_text)
{
    /* 
     * lookup_reserved_word examines a string which may be a 
     * keyword.  If so, it returns the keyword value, otherwise 
     * returns (-1) if not a keyword.  
     */

    typedef struct struct_tokens
    {
        char * token_string;
        int token_value;
        int disable_subsequent_keywords; 
    } TOKENS;
    
    int i;
    int lo;
    int hi;
    int test;
    
    /* 
     * Tokens must be ordered by token_string.
     */

    TOKENS tokens [] =          
    {   { "clear", CLEAR, FALSE },
        { "continue", CONTINUE, FALSE },
        { "default", DEFAULT, FALSE },
        { "echo", ECHO_STR, TRUE },
        { "onerror", ONERROR, FALSE },
        { "pause", PAUSE, FALSE },
        { "read", READ, TRUE },
        { "remote", REMOTE, TRUE },
        { "run", RUN, TRUE },
        { "set", SETCMD, TRUE },
        { "stop", STOP, FALSE },
        { "subtotal", SUBTOTAL, FALSE },
        { "summary", SUMMARY, FALSE },
        { "test", TEST, FALSE },
        { "unset", UNSET, TRUE }
    };
    
    /* 
     * Notes regarding keyword recognition:
     * 
     * testsh allows keywords to be used as strings outside of 
     * their keyword context.  When we recognize particular 
     * keywords, we disable keyword recognition until the next 
     * newline.  This behavior is triggered on a per-keyword 
     * basis.  Keyword recognition is turned back on at the end 
     * of a logical statement.
     * 
     * Sounds like fortran.
     * 
     */
    
    if (disable_keyword_recognition)
        return -1;              /* it's a string. */

    /* 
     * Lookup reserved word token.  Return yacc token value to 
     * caller, otherwise if not found, return -1.  
     */
    
    for (lo = 0, hi = (sizeof (tokens) / sizeof (TOKENS)) - 1;
         lo <= hi; )
    {
        i = (lo + hi) / 2;
        test = strcmp (arg_text, tokens[i].token_string);

        if (test < 0)
            hi = i - 1;
        else
        if (test > 0)
            lo = i + 1;
        else                            /* found */
        {
            disable_keyword_recognition = 
                tokens[i].disable_subsequent_keywords; 

            return tokens [i].token_value;
        }                               /* found */

    }                           /* for () */
    return -1;                  /* not found, it's a string */
}                               /* lookup_reserved_word */


