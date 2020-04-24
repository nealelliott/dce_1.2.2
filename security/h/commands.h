/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: commands.h,v $
 * Revision 1.2.6.2  1996/02/18  22:58:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:49  marty]
 *
 * Revision 1.2.6.1  1995/12/13  16:24:29  root
 * 	Submit
 * 	[1995/12/11  15:14:14  root]
 * 
 * Revision 1.2.4.3  1992/12/29  13:06:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:11  zeliff]
 * 
 * Revision 1.2.4.2  1992/10/12  19:45:21  burati
 * 	CR2828 rgy_edit coredumps deleting names > 1020 chars
 * 	[1992/10/09  21:06:03  burati]
 * 
 * Revision 1.2.2.2  1992/05/08  17:37:19  burati
 * 	Fix buffer length
 * 	[1992/05/07  23:26:46  burati]
 * 
 * Revision 1.2  1992/01/19  22:13:45  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */

/*
 *  OSF DCE Version 1.0 
 */

/*  commands.h V=4 04/29/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      command line interpreter - public header file
*/

#ifndef commands__h_included
#define commands__h_included

#include <dce/nbase.h>

/*
 * This is needed to resolve the circular reference to struct command_buf_s
 * In the next typedef, struct command_buf_s is used in the prototype's 
 * arglist, in ANSI C it's scope is only the arg list.  This line 
 * reserves the tag name so that all references to struct command_buf_s
 * refer to the same thing.  See ANSI C section 3.5.2.3
 */
#ifdef __STDC__
struct command_buf_s;
#endif      

typedef error_status_t (*function)(
#ifdef __STDC__
    struct command_buf_s *
#endif
);

#define CMD_LINE_LENGTH 1153 /* room for 1 name + 128 char command */

typedef struct command_buf_s {
        boolean32 done;
        char command[CMD_LINE_LENGTH];
        function op;
        int num_args;
        char **args;
} command_buf;

typedef struct {
        char    *name;
        function code;
        function help;
        int     min;
} command_table;

/*
** Avoid having to repeat the same signature over and over by
** having macros that define commands and help functions.
*/

#ifdef __STDC__
#    define COMMAND(NAME) extern error_status_t NAME ( command_buf *c )
#else
#    define COMMAND(NAME) extern error_status_t NAME ()
#endif

#ifdef __STDC__
#    define HELP(NAME) extern error_status_t NAME ( command_buf *c )
#else
#    define HELP(NAME) extern error_status_t NAME ()
#endif

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

void set_commands(
#ifdef __STDC__ 
    command_table  *c
#endif
);

/* 
 * procs_cmds
 * prompts user for command, parses command line and dispatches 
 * command with arguments
 */

void procs_cmds(
#ifdef __STDC__
    boolean32 must_prompt,
    char *prompt_string
#endif
);

#endif
