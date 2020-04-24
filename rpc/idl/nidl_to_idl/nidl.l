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
 * $Log: nidl.l,v $
 * Revision 1.1.7.2  1996/03/09  23:25:56  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:41:35  marty]
 *
 * Revision 1.1.7.1  1995/12/07  22:36:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:25  root]
 * 
 * Revision 1.1.4.4  1993/03/23  22:02:23  weisman
 * 	Fix for OT 7525: Copy changes for new AIX lex (ncform)
 * 	from idl_compiler/nidl.l into nidl_to_idl/nidl.l
 * 	[1993/03/23  21:55:32  weisman]
 * 
 * Revision 1.1.4.3  1993/01/03  22:15:09  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:48:21  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:16:11  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:11:30  zeliff]
 * 
 * Revision 1.1.2.2  1992/01/22  22:33:41  jim
 * 	RCS header and copyright statement incorrectly placed.
 * 	This breaks lexx and yacc because it does not see this as a comment.
 * 	[1992/01/22  21:30:14  jim]
 * 
 * Revision 1.1  1992/01/19  19:29:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * ==========================================================================
 * Confidential and Proprietary.  Copyright 1987 by Apollo Computer Inc.,
 * Chelmsford, Massachusetts.  Unpublished -- All Rights Reserved Under
 * Copyright Laws Of The United States.
 *
 * Apollo Computer Inc. reserves all rights, title and interest with respect
 * to copying, modification or the distribution of such software programs and
 * associated documentation, except those rights specifically granted by Apollo
 * in a Product Software Program License, Source Code License or Commercial
 * License Agreement (APOLLO NETWORK COMPUTING SYSTEM) between Apollo and
 * Licensee.  Without such license agreements, such software programs may not
 * be used, copied, modified or distributed in source or object code form.
 * Further, the copyright notice must appear on the media, the supporting
 * documentation and packaging as set forth in such agreements.  Such License
 * Agreements do not grant any rights to use Apollo Computer's name or trademarks
 * in advertising or publicity, with respect to the distribution of the software
 * programs without the specific prior written permission of Apollo.  Trademark
 * agreements may be obtained in a separate Trademark License Agreement.
 * ==========================================================================
 */

    /*  Get definitions for token values    */
#include <ctype.h>

#if defined(MSDOS) || defined(SYS5) || defined(vaxc)
#  include <string.h>
#else
#  include <strings.h>
#endif

#include <idl_base.h>
#include <utils.h>
#include <errors.h>
#include <nametbl.h>
#include <ast.h>
#include <astp.h>
#include <y_tab.h>
#include <sysdep.h>

boolean search_attributes_table = false ;
boolean search_pas_keywords     = true  ;
boolean search_c_keywords       = false ;
extern boolean interface_is_imported;

#ifndef MAX_INCLUSION_DEPTH
#define MAX_INCLUSION_DEPTH 10
#endif

static  FILE *inclusion_stack[MAX_INCLUSION_DEPTH] ;
static  int  stack_depth = 0 ;

static  char    *source ;

comment_t       *comment_list = NULL;
comment_t       *comment_last = NULL;


void yymark __PROTOTYPE((void));
int  yyinclude __PROTOTYPE((void)) ;
void yyuninclude __PROTOTYPE((void));
void read_c_comment __PROTOTYPE((void));
void read_pas_comment __PROTOTYPE((void));
int yylex __PROTOTYPE((void));
int yylook __PROTOTYPE((void));
int yywrap __PROTOTYPE((void));
int yyuuid __PROTOTYPE((char *s, uuid_$t *uuid));
void commenteof __PROTOTYPE((void));
int screen __PROTOTYPE ((char *token));
int yyinput __PROTOTYPE((void));


#ifndef _AIX
# if defined(SNI_SVR4)
    void yyoutput (
# else
    int yyoutput (
# endif
# ifdef PROTO
    int c
# endif
  );

# if defined(SNI_SVR4)
    void yyunput (
# else
    int yyunput (
# endif
# ifdef PROTO
    int c
# endif
  );
#endif	/* _AIX */

int yyback __PROTOTYPE ((int *p, int m));
void push_comment ();
extern void exit __PROTOTYPE((int));
%}

%p 2000
%a 2100

/* regular definitions  */

delim           [ \t\n]
ws              {delim}+
letter          [A-Za-z_$]
digit           [0-9]
hex_digit       [0-9A-Fa-f]
id              {letter}({letter}|{digit})*
integer         -?{digit}+
pas_hex_integer (16#{hex_digit}*)
c_hex_integer   (0(x|X){hex_digit}*)
float           {digit}+(\.{digit}+)?
octet           {hex_digit}{hex_digit}
octet_dot       {octet}\.
uuid            \({octet}{octet}{octet}{octet}{octet}{octet_dot}{octet_dot}{octet_dot}{octet_dot}{octet_dot}{octet_dot}{octet_dot}{octet_dot}{octet}\)

other       [ \t]

%%
{ws}        {   /* No action, and no return */ }

":"         {return(COLON);}
","         {return(COMMA);}
".."        {return(DOTDOT);}
"="         {return(EQUAL);}
"^"         {return(HAT);}
\[          {return(LBRACKET);}
"("         {return(LPAREN);}
\]          {return(RBRACKET);}
")"         {return(RPAREN);}
";"         {return(SEMI);}
"*"         {return(STAR);}
"}"         {return(RBRACE);}
"&"         {return(AMPER);}
^"#"{ws}*{digit}*({ws}+.*)?\n  { yymark() ;}
^"%include" { return yyinclude() ;}
^"%INCLUDE" { return yyinclude() ;}
^"%pascal"  {language_style = pascal_style;}
^"%PASCAL"  {language_style = pascal_style;}
^"%c"       {language_style = c_style;}
^"%C"       {language_style = c_style;}

'[^'\n]*'   {
                char    stripped_string[max_string_len] ;

                strcpy(stripped_string, &yytext[1]);
                stripped_string[strlen(yytext)-2] = 0 ;
                yylval.y_string = STRTAB_add_string(stripped_string);
                return(STRING);
            }
\"[^\"\n]*\"  {
                char    stripped_string[max_string_len] ;

                strcpy(stripped_string, &yytext[1]);
                stripped_string[strlen(yytext)-2] = 0 ;
                yylval.y_string = STRTAB_add_string(stripped_string);
                return(STRING);
            }
"{"         { if (language_style == pascal_style)
                read_pas_comment() ;
              else
                return (LBRACE);
            }

"/*"        { read_c_comment(); }

{id}        {
             return screen(yytext) ;
            }
{integer}   {
             sscanf(yytext,"%ld", &yylval.y_ival) ;
             return(INTEGER_NUMERIC);
            }
{pas_hex_integer}   {
                 sscanf(&yytext[3],"%lx", &yylval.y_ival) ;
                 return(INTEGER_NUMERIC);
                }

{c_hex_integer}   {
                 sscanf(&yytext[2],"%lx", &yylval.y_ival) ;
                 return(INTEGER_NUMERIC);
                }
{uuid}      {   return (yyuuid(&yytext[1], &yylval.y_uuid)); }

{other}     {return (int) yytext[0];}
%%

void commenteof()
{
        fprintf (stderr, "unexpected EOF inside comment at line %d\n",
             yylineno);
        terminate();
}

int yywrap ()
{
    int    c ;
    yyuninclude() ;
    if ((c = input()) != 0)
    {
        unput(c) ;
        return (0) ;
    }
    yylineno = 1 ;
        return (1);
}

int screen(token)
    char *token ;
{
    typedef struct
    {
        char    *keyword;
        int     token_value;
    } keyword_entry ;

    static keyword_entry pas_keywords[] =
    {
        /*  NIDL/Pascal keywords */

        {"ARRAY",       ARRAY_KW},
        {"array",       ARRAY_KW},
        {"BOOLEAN",     BOOLEAN_KW},
        {"boolean",     BOOLEAN_KW},
        {"BYTE",        BYTE_KW},
        {"byte",        BYTE_KW},
        {"CASE",        CASE_KW},
        {"case",        CASE_KW},
        {"CHAR",        CHAR_KW},
        {"char",        CHAR_KW},
        {"CONST",       CONST_KW},
        {"const",       CONST_KW},
        {"DOUBLE",      DOUBLE_KW},
        {"double",      DOUBLE_KW},
        {"END",         END_KW},
        {"end",         END_KW},
        {"FALSE",       FALSE_KW},
        {"false",       FALSE_KW},
        {"FUNCTION",    FUNCTION_KW},
        {"function",    FUNCTION_KW},
        {"HANDLE_T",    HANDLE_T_KW},
        {"handle_t",    HANDLE_T_KW},
        {"IN",          IN_KW},
        {"in",          IN_KW},
        {"IMPORT",      IMPORT_KW},
        {"import",      IMPORT_KW},
        {"INTEGER",     INTEGER_KW},
        {"integer",     INTEGER_KW},
        {"INTEGER8",    INTEGER8_KW},
        {"integer8",    INTEGER8_KW},
        {"INTEGER32",   INTEGER32_KW},
        {"integer32",   INTEGER32_KW},
        {"INTEGER64",   INTEGER64_KW},
        {"integer64",   INTEGER64_KW},
        {"INTERFACE",   INTERFACE_KW},
        {"interface",   INTERFACE_KW},
        {"NIL",         NIL_KW},
        {"nil",         NIL_KW},
        {"OF",          OF_KW},
        {"of",          OF_KW},
        {"OPTIONS",     OPTION_KW},
        {"options",     OPTION_KW},
        {"OTHERWISE",   OTHERWISE_KW},
        {"otherwise",   OTHERWISE_KW},
        {"OUT",         OUT_KW},
        {"out",         OUT_KW},
        {"PROCEDURE",   PROCEDURE_KW},
        {"procedure",   PROCEDURE_KW},
        {"REAL",        REAL_KW},
        {"real",        REAL_KW},
        {"RECORD",      RECORD_KW},
        {"record",      RECORD_KW},
        {"REF",         REF_KW},
        {"ref",         REF_KW},
        {"rpc_$drep_t", DREP_T_KW},
        {"RPC_$DREP_T", DREP_T_KW},
        {"SET",         SET_KW},
        {"set",         SET_KW},
        {"STRING0",     STRING0_KW},
        {"string0",     STRING0_KW},
        {"TRUE",        TRUE_KW},
        {"true",        TRUE_KW},
        {"TYPE",        TYPE_KW},
        {"type",        TYPE_KW},
        {"UNIV_PTR",    UNIV_PTR_KW},
        {"univ_ptr",    UNIV_PTR_KW},
        {"UNSIGNED",    UNSIGNED_KW},
        {"unsigned",    UNSIGNED_KW},
        {"UNSIGNED8",   UNSIGNED8_KW},
        {"unsigned8",   UNSIGNED8_KW},
        {"UNSIGNED32",  UNSIGNED32_KW},
        {"unsigned32",  UNSIGNED32_KW},
        {"UNSIGNED64",  UNSIGNED64_KW},
        {"unsigned64",  UNSIGNED64_KW},
        {"VAL",         VAL_KW},
        {"val",         VAL_KW},
        {0,             0}
    };

        /*  NIDL/C Keywords  */
    static keyword_entry c_keywords[] =
    {
        {"interface",        INTERFACE_KW},
        {"import",           IMPORT_KW},
        {"typedef",          TYPEDEF_KW},
        {"const",            CONST_KW},
        {"struct",           STRUCT_KW},
        {"union",            UNION_KW},
        {"bitset",           BITSET_KW},
        {"default",          DEFAULT_KW},
        {"enum",             ENUM_KW},
        {"set",              SET_KW},
        {"int",              INT_KW},
        {"byte",             BYTE_KW},
        {"char",             CHAR_KW},
        {"unsigned",         UNSIGNED_KW},
        {"hyper",            HYPER_KW},
        {"long",             LONG_KW},
        {"small",            SMALL_KW},
        {"float",            FLOAT_KW},
        {"short",            SHORT_KW},
        {"void",             VOID_KW},
        {"double",           DOUBLE_KW},
        {"boolean",          BOOLEAN_KW},
        {"case",             CASE_KW},
        {"switch",           SWITCH_KW},
        {"string0",          STRING0_KW},
        {"handle_t",         HANDLE_T_KW},
        {"rpc_$drep_t",      DREP_T_KW},
        {"true",             TRUE_KW},
        {"false",            FALSE_KW},
        {"NULL",             NIL_KW},
        {0,                  0}
    } ;

    static keyword_entry attributes[] =
    {
        {"BROADCAST",       BROADCAST_KW},
        {"broadcast",       BROADCAST_KW},
        {"COMM_STATUS",     COMM_STATUS_KW},
        {"comm_status",     COMM_STATUS_KW},
        {"IDEMPOTENT",      IDEMPOTENT_KW},
        {"idempotent",      IDEMPOTENT_KW},
        {"HANDLE",          HANDLE_KW},
        {"handle",          HANDLE_KW},
        {"IMPLICIT_HANDLE", IMPLICIT_HANDLE_KW},
        {"implicit_handle", IMPLICIT_HANDLE_KW},
        {"in",              IN_KW},
        {"LAST_IS",         LAST_IS_KW},
        {"last_is",         LAST_IS_KW},
        {"LOCAL",           LOCAL_KW},
        {"local",           LOCAL_KW},
        {"MAYBE",           MAYBE_KW},
        {"maybe",           MAYBE_KW},
        {"MAX_IS",          MAX_IS_KW},
        {"max_is",          MAX_IS_KW},
        {"out",             OUT_KW},
        {"PORT",            PORT_KW},
        {"port",            PORT_KW},
        {"REMOTE",          REMOTE_KW},
        {"remote",          REMOTE_KW},
        {"SECURE",          SECURE_KW},
        {"secure",          SECURE_KW},
        {"TRANSMIT_AS",     TRANSMIT_AS_KW},
        {"transmit_as",     TRANSMIT_AS_KW},
        {"UUID",            UUID_KW},
        {"uuid",            UUID_KW},
        {"VERSION",         VERSION_KW},
        {"version",         VERSION_KW},
        {0,                 0}
    };

    int i;

    /*
     *  Scan the reserved word table.
     */
    if (search_pas_keywords)
    {
        for (i = 0; pas_keywords[i].keyword != 0; i++)
            if (strcmp(token, pas_keywords[i].keyword) == 0)
                return pas_keywords[i].token_value ;
    }
    else
    {
        for (i = 0; c_keywords[i].keyword != 0; i++)
            if (strcmp(token, c_keywords[i].keyword) == 0)
                return c_keywords[i].token_value ;
    } ;

    /*
     *  Not a reserved word.
     *      See if attribute, if not add to nametable.
     */

    if (search_attributes_table)
        for (i = 0; attributes[i].keyword != 0; i++)
            if (strcmp(token, attributes[i].keyword) == 0)
                return attributes[i].token_value ;

    if (language_style == pascal_style)
        yylval.y_id = NAMETABLE_add_id(token, true);
    else
        yylval.y_id = NAMETABLE_add_id(token, true);
    return IDENTIFIER ;
}


void yymark()
{
    if (source)
        free(source) ;
    source = (char *) alloc(yyleng) ;
    if (source)
        sscanf(yytext, "# %d %s", &yylineno, source) ;
}

int  yyinclude()
{
    int         start, end, len ;
    int         c ;
    char        included_file_name[max_string_len] ;
    char        error_message[max_string_len] ;

    if (stack_depth == MAX_INCLUSION_DEPTH)
        error("Too many %%includes\n") ;

    len = 0 ;
    while (c= input())
    {
        if (c == '\'' || c == '"')
        {
            while (c = input())
            {
                if (c == '\'' || c == '"')
                {
                    included_file_name[len] = '\0' ;

                    inclusion_stack[stack_depth++] = yyin ;
                    while (c = input())
                        if (c == ';') break ;
                    if ( (yyin = fopen(included_file_name, "r")) == 0)
                    {
                        sprintf(error_message, "Couldn't open include file %s\n", included_file_name) ;
                        error(error_message) ;
                    }
                    return yylex() ;
                }
                included_file_name[len++] = c ;
            }
        }
    }
    /* XXX is this right? */
    return c;
}

void yyuninclude()
{
    if (stack_depth > 0)
    {
        --stack_depth ;
        yyin = inclusion_stack[stack_depth] ;
    }
}

void push_comment(text,text_size)
char *text;
int  text_size;
{
    comment_t *new;

    if (interface_is_imported) return;

    new = (comment_t *)alloc(sizeof(comment_t));
    new->comment_text = (char *)alloc(text_size + 1);
    *new->comment_text = '\0';
    strncat(new->comment_text, text, text_size);

    /* link onto the comment list */
    if (comment_list == NULL) {
        comment_list = new;
        comment_last = new;
        }
    else {
        comment_last->next = new;
        comment_last = new;
        }
}


#define MAXBUF  256

void read_c_comment()
{
    register int i;
    char        buf[MAXBUF];
    comment_t   a_comment;

    i = 0;
    for (buf[i]=input(); buf[i] != 0; buf[++i]=input())
    {
        if (buf[i] == '*')
        {
            if ((buf[++i] = input()) == '/')
                break ;
            else
                unput(buf[i--]);
        }

        if (i >= (MAXBUF - 2))
        {
            push_comment(buf,i+1);
            i = -1;
        }
    }

    buf[i-1] = '\n';
    push_comment(buf, i);
    if (buf[i] == 0) commenteof();
}

void read_pas_comment()
{
    register int i;
    char        buf[MAXBUF];
    comment_t   a_comment;

    i = 0;
    for (buf[i]=input(); buf[i] != 0; buf[++i]=input())
    {
        if (buf[i] == '}') break;
        if (i >= (MAXBUF - 2))
        {
            push_comment(buf,i);
            i = -1;
        }
    }

    buf[i] = '\n';
    push_comment(buf,i+1);
    if (buf[i] == 0) commenteof();
}


int yyuuid(s, uuid)
char *s;
uuid_$t *uuid;
{
    unsigned short host[7];
    unsigned short family;

    sscanf(s, "%8lx%4hx.%2hx.%2hx.%2hx.%2hx.%2hx.%2hx.%2hx.%2hx",
              &uuid->time_high, &uuid->time_low, &family,
              &host[0], &host[1], &host[2], &host[3], &host[4], &host[5], &host[6]);

    uuid->family = family;

    uuid->host[0] = host[0];
    uuid->host[1] = host[1];
    uuid->host[2] = host[2];
    uuid->host[3] = host[3];
    uuid->host[4] = host[4];
    uuid->host[5] = host[5];
    uuid->host[6] = host[6];
    return (UUID_REP) ;
}
