/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: commands.c,v $
 * Revision 1.1.12.2  1996/02/18  00:22:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:11  marty]
 *
 * Revision 1.1.12.1  1995/12/08  18:01:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:52  root]
 * 
 * Revision 1.1.7.2  1994/09/15  21:04:55  cuti
 * 	ot12181: Change third argument of mbstowcs() from 0 to strlen(buf).
 * 	[1994/09/15  21:04:28  cuti]
 * 
 * Revision 1.1.7.1  1994/09/09  20:41:11  kline_s
 * 	OT 12028: I18N code cleanup for Security.
 * 	[1994/09/09  20:36:49  kline_s]
 * 
 * Revision 1.1.4.5  1992/12/29  16:40:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:31:46  zeliff]
 * 
 * Revision 1.1.4.4  1992/12/24  14:51:48  burati
 * 	CR6629 Put TRY/CATCH in command processing loop to catch ctrl-c
 * 	[1992/12/23  22:29:28  burati]
 * 
 * Revision 1.1.4.3  1992/12/09  21:03:21  burati
 * 	CR6393 procs_cmds was incorrectly writing prompt string terminator
 * 	[1992/12/09  19:36:12  burati]
 * 
 * Revision 1.1.4.2  1992/10/12  19:46:25  burati
 * 	CR2828 rgy_edit coredumps deleting names > 1020 chars
 * 	[1992/10/09  21:06:49  burati]
 * 
 * Revision 1.1.2.2  1992/05/08  17:26:39  burati
 * 	Fix internal buffer length
 * 	[1992/05/07  23:29:24  burati]
 * 
 * Revision 1.1  1992/01/19  14:41:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  commands.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
**
*/
/*
**      command line interpreter routines
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)commands.c	5 - 10/29/91";
#endif

#include <stdio.h>
#include <ctype.h>
#include <wchar.h>
#include <sys/types.h>
#include <sys/time.h>
#include <un_io.h>
#include <un_strng.h>
#include <un_maloc.h>
#include <stdlib.h>
#include <macros.h>
#include <commands.h>

#include <dce/nbase.h>

#include <args.h>

#define CMD_OP 1
#define HELP_OP 2

PRIVATE command_table *commands;

PRIVATE make_lower_case
#ifndef __STDC__
    (buf)
   char *buf;
#else
  (
   char *buf
  )
#endif
{
    wchar_t     *wcs_buf, *wcs_ptr;
    int         wcs_size;

    wcs_buf = NULL;

    /* Determine the size of wchar_t buffer needed to be allocated,
     * then malloc storage and convert to wchar_t for processing.
     */

    wcs_size = mbstowcs((wchar_t *) 0, buf, strlen(buf));
    wcs_buf = malloc((wcs_size + 1) * sizeof(wchar_t));
    mbstowcs(wcs_buf, buf, (wcs_size+1));

    /* Convert to lower case */
    wcs_ptr = wcs_buf;
    while (*wcs_ptr != (wchar_t) 0) {
         if (iswupper(*wcs_ptr)) 
             *wcs_ptr = towlower(*wcs_ptr);
         wcs_ptr++;
    }

    /* Convert back to multibyte string format & cleanup */
    wcstombs(buf, wcs_buf, (wcs_size+1));
    free(wcs_buf);
}
 

PUBLIC error_status_t tty_do_nil
#ifndef __STDC__
    (c)
   command_buf *c;
#else
  (
   command_buf *c
  )
#endif
{
    error_status_t  st;
    SET_STATUS(&st, error_status_ok);
    return st;
}


PUBLIC error_status_t tty_do_nogood
#ifndef __STDC__
    (c)
   command_buf *c;
#else
  (
   command_buf *c
  )
#endif
{
    error_status_t  st;
    SET_STATUS(&st, error_status_ok);
    printf("Unknown command: %s\n",c->args[0]);
    return st;
}

PRIVATE function get_op
#ifndef __STDC__
    (s, op)
   char *s;
   int op;
#else
  (
   char *s,
   int op
  )
#endif
{
    char ignore_case[CMD_LINE_LENGTH];
    int i;

    strncpy(ignore_case, s, sizeof(ignore_case));
    make_lower_case(ignore_case);

    for (i=0; commands[i].name != NULL; i++) {
        if (match_command(commands[i].name, ignore_case, commands[i].min))
            if (op == CMD_OP) {
                return commands[i].code;
            } else if (op == HELP_OP) {
                return commands[i].help;
            } else {
                break;
            }
    }
    return tty_do_nogood;
}


PUBLIC error_status_t tty_do_help
#ifndef __STDC__
    (c)
   command_buf  *c;
#else
  (
   command_buf  *c
  )
#endif
{
    int             i, j;
    char            buf[32];
    char            *p;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    if (c->num_args == 1) {
        printf("Known commands are:");
        for (i = 0; *commands[i].name != '\0'; i++) {
            if ((i) % 4 == 0) {
                printf("\n\t");
            }
            p = commands[i].name;
            for (j = 0; (j < sizeof(buf)-1) && *p; j++) {
                buf[j] = *p++;
                if (j == commands[i].min) {
                    buf[j+1] = buf[j];
                    buf[j] = '[';
                    j++;
                }
            }
            if (j != commands[i].min) {
                buf[j++] = ']';
            }
            buf[j] = '\0';
            printf("%-15.15s ", buf);
        }
        printf("\n");
    } else {
        for (i = 1; i < c->num_args; i++) {
            c->args[0] = c->args[i];
            c->op = get_op(c->args[i], HELP_OP);
            (*c->op)(c);
        }
    }

    return st;
}

PUBLIC error_status_t tty_help_all
#ifndef __STDC__
    (c)
   command_buf *c;
#else
  (
   command_buf *c
  )
#endif
{
    int i;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);


    for (i = 0; *commands[i].name != '\0'; i++) {
        (*(commands[i].help))(c);
    }

    return st;
}

PUBLIC error_status_t tty_help_help
#ifndef __STDC__
    ( c ) 
   command_buf *c;
#else
  (
   command_buf *c
  )
#endif
{
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    printf("\
help                            -- yields list of known commands\n\
help command1 { command2 ... }  -- yields information on specified commands\n");

    return st;
}


PRIVATE get_command
#ifndef __STDC__
    (c)
   command_buf    *c;
#else
  (
   command_buf    *c
  )
#endif
{
    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 250000;

    do {
        c->done = (gets(c->command) == NULL);
    } while (c->done && !feof(stdin));

    if (!c->done) {
        args_get(c->command, &c->num_args, &c->args);
        if (c->num_args > 0)
            c->op = get_op(c->args[0], CMD_OP);
        else
            c->op = tty_do_nil;
    }
    select(0, 0, 0, 0, &timeout);
}


/* 
 * set_commands
 * initializes the command table; must be of the form
 *
 *   command_table commands[] = 
 *   {      
 *       { "add",   tty_do_add,    tty_help_add,  1 },
 *       { "drop",  tty_do_drop,   tty_help_drop, 1 }
 *   };
 *  
 * 
 *   where entries are: 
 *   command_name, operation, help function, min length of command
 */

PUBLIC  void set_commands
#ifndef __STDC__
    (c)
command_table    *c;
#else
  (
command_table    *c
  )
#endif
{
    commands = c;
}


/* 
 * procs_cmds
 * prompts user for command, parses command line and dispatches 
 * command with arguments
 */

PUBLIC  void procs_cmds
#ifndef __STDC__
    (must_prompt, prompt_string)
    boolean32  must_prompt;
    char    *prompt_string;
#else
  (
    boolean32  must_prompt,
    char    *prompt_string
  )
#endif
{
    command_buf       c;

    while (TRUE) {
        TRY {
            if (must_prompt) {
                fputs(prompt_string, stdout);
            }
            get_command(&c);

            if (c.done) {
                return;
            }
            (*c.op)(&c);
        } CATCH_ALL {
        } ENDTRY        
    }
}
