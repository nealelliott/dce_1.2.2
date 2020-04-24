/*
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
 * $Log: error_table.y,v $
 * Revision 1.1.9.2  1996/03/09  23:29:33  marty
 * 	Update OSF copyright year
 * 	[1996/03/09  22:44:54  marty]
 *
 * Revision 1.1.9.1  1995/12/08  17:39:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:38  root]
 * 
 * Revision 1.1.7.3  1992/12/29  13:56:01  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:50:49  zeliff]
 * 
 * Revision 1.1.7.2  1992/12/11  21:01:15  sommerfeld
 * 	[OT6336] use isdigit rather than hardcoded compares against chars..
 * 	[1992/12/09  21:59:38  sommerfeld]
 * 
 * Revision 1.1.5.3  1992/11/23  00:07:07  sommerfeld
 * 	Remove unnecessary function gensym().
 * 	[1992/11/20  22:30:48  sommerfeld]
 * 
 * 	Clean up logic in set_table_fun to use normal C idiom for string
 * 	walking and avoid double increment of string pointer.
 * 	[1992/11/20  22:17:23  sommerfeld]
 * 
 * Revision 1.1.5.2  1992/09/29  21:18:28  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:22:10  sekhar]
 * 
 * Revision 1.1.2.2  1992/04/09  20:08:47  jim
 * 	Changes for AIX 3.2.  Do not define malloc or string functions.
 * 	[1992/04/09  19:53:31  jim]
 * 
 * $EndLog$
 */
 

/*  error_table.y V=3 11/20/91 //littl/prgy/krb5/comerr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
%{
#include <stdlib.h>
#include <stdio.h>
    
char *str_concat(), *ds(), *quote();
char *current_token = (char *)NULL;
extern char *table_name;
%}
%union {
        char *dynstr;
}

%token ERROR_TABLE ERROR_CODE_ENTRY END
%token <dynstr> STRING QUOTED_STRING
%type <dynstr> ec_name description table_id table_fun header
%{
%}
%start error_table
%%

error_table     :       ERROR_TABLE header error_codes END
                        { table_name = ds($2);
                          current_token = table_name;
                          put_ecs(); }
                ;

header          :       table_fun table_id
                        { current_token = $1;
                          $$ = $2; }
                |       table_id
                        { current_token = $1;
                          set_table_fun(ds("1"));
                          $$ = $1;
                        }
                ;

table_fun       :       STRING
                        { current_token = $1;
                          set_table_fun($1);
                          $$ = $1; }
                ;

table_id        :       STRING
                        { current_token = $1;
                          set_table_num($1);
                          $$ = $1; }
                ;

error_codes     :       error_codes ec_entry
                |       ec_entry
                ;

ec_entry        :       ERROR_CODE_ENTRY ec_name ',' description
                        { add_ec($2, $4);
                          free($2);
                          free($4); }
                |       ERROR_CODE_ENTRY ec_name '=' STRING ',' description
                        { add_ec_val($2, $4, $6);
                          free($2);
                          free($4);
                          free($6);
                        }
                ;

ec_name         :       STRING
                        { $$ = ds($1);
                          current_token = $$; }
                ;

description     :       QUOTED_STRING
                        { $$ = ds($1);
                          current_token = $$; }
                ;

%%
/*
 *
 * Copyright 1986, 1987 by the MIT Student Information Processing Board
 *
 * For copyright info, see mit-sipb-cr.h.
 */

#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/time.h>
#include "internal.h"
#include "error_table.h"
#include "mit-sipb-cr.h"

#ifndef lint
static char const rcsid_error_table_y[] =
    "$Header: /u0/rcs_trees/dce/rcs/security/krb5/comerr/error_table.y,v 1.1.9.2 1996/03/09 23:29:33 marty Exp $";
#endif

extern FILE *hfile, *msfile;

char *
ds(string)
        char const *string;
{
        char *rv;
        rv = malloc(strlen(string)+1);
        strcpy(rv, string);
        return(rv);
}

char *
quote(string)
        char const *string;
{
        char *rv;
        rv = malloc(strlen(string)+3);
        strcpy(rv, "\"");
        strcat(rv, string);
        strcat(rv, "\"");
        return(rv);
}

long table_number = 0;
int current = 0;
char **error_codes = (char **)NULL;

add_ec(name, description)
        char const *name, *description;
{
        if (msfile) {
            if (current > 0)
                fprintf(msfile, "%d\t%s\n", current, description);
        }
        if (error_codes == (char **)NULL) {
                error_codes = (char **)malloc(sizeof(char *));
                *error_codes = (char *)NULL;
        }
        error_codes = (char **)realloc((char *)error_codes,
                                       (current + 2)*sizeof(char *));
        error_codes[current++] = ds(name);
        error_codes[current] = (char *)NULL;
}

add_ec_val(name, val, description)
        char const *name, *val, *description;
{
        const int ncurrent = atoi(val);
        if (ncurrent < current) {
                printf("Error code %s (%d) out of order", name,
                       current);
                return;
        }
      

        while (ncurrent > current) 
            current++;
        
        if (msfile) {
            if (current > 0)
                fprintf(msfile, "%d\t%s\n", current, description);
        }
        if (error_codes == (char **)NULL) {
                error_codes = (char **)malloc(sizeof(char *));
                *error_codes = (char *)NULL;
        }
        error_codes = (char **)realloc((char *)error_codes,
                                       (current + 2)*sizeof(char *));
        error_codes[current++] = ds(name);
        error_codes[current] = (char *)NULL;
} 

put_ecs()
{
        int i;
        for (i = 0; i < current; i++) {
             if (error_codes[i] != (char *)NULL)
                  fprintf(hfile, "#define %-40s (%ldL)\n",
                          error_codes[i], table_number + i);
        }
}

/*
 * char_to_num -- maps letters and numbers into very small space
 *      0-9        -> 0-9
 *      mixed case -> 10-35
 *      _          -> 36
 *      others are reserved
 */

static const char char_set[] =
        "abcdefghijklmnopqrstuvwxyz_0123456789";

int char_to_num(c)
        char c;
{
        const char *where;
        int diff;

        if (isupper(c)) c = tolower(c);

        where = strchr (char_set, c);
        if (where) {
                /* start at 1 so we can decode */
                diff = where - char_set;
                assert (diff < 050);    /* it is radix 50, after all */
                return diff;
        }
        else if (isprint (c))
                fprintf (stderr,
                         "Illegal character `%c' in error table name\n",
                         c);
        else
                fprintf (stderr,
                         "Illegal character %03o in error table name\n",
                         c);
        exit (1);
}

set_table_fun(astring)
char *astring; {
    register char *tp;
    unsigned int tc;

    for(tp=astring; (tc = *tp) != 0; tp++) {
        if (!isdigit(tc)) {
            fprintf(stderr, "Table function '%s' must be a decimal integer.\n",
                    astring);
            exit(1);
        }
    }
    table_number += (atoi(astring)) << 28;
}

/* for compatibility with old comerr's, we truncate package name to 4
 * characters, but only store first 3 in the error code.  Note that this
 * function, as a side effect, truncates the table name down to 4 chars.
 */
set_table_num(string)
  char *string;
{
        long temp;
        int ctr;

        if ((temp = strlen(string)) > 4) {
                fprintf(stderr, "Table name %s too long, truncated ",
                        string);
                string[4] = '\0';
                fprintf(stderr, "to %s\n", string);
        }
        if (temp == 4) {
            fprintf(stderr, "Table name %s too long, only 3 characters fit in error code.\n",
                    string);
        }
        if (char_to_num (string[0]) > char_to_num ('z')) {
                fprintf (stderr, "%s%s%s%s",
                         "First character of error table name must be ",
                         "a letter; name ``",
                         string, "'' rejected\n");
                exit (1);
        }
        temp = 0;
        for(ctr=0; ctr < 3; ctr++) {            /* copy at most 3 chars to integer */
            if (*string == '\0') break;         /* and watch for early end */
            temp = (temp * 050)                 /* "radix fifty" is base 050 = 40 */
                + char_to_num(*string);
            string++;
        }
        table_number += temp << 12;
}

#ifndef DSEE
#include "et_lex.lex.c"
#else
#include "$(et_lex.lex.l).c"
#endif

