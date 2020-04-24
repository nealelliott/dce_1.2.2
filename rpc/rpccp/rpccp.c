/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: rpccp.c,v $
 * Revision 1.1.15.2  1996/02/18  00:00:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:59  marty]
 *
 * Revision 1.1.15.1  1995/12/08  00:15:50  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/30  22:40 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:33 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:33 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 	[1995/12/07  23:56:41  root]
 * 
 * Revision 1.1.13.2  1994/05/19  19:05:49  melman
 * 	added some (char *) casts
 * 	[1994/05/19  17:34:14  melman]
 * 
 * Revision 1.1.13.1  1994/01/21  22:32:39  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:22:17  cbrooks]
 * 
 * Revision 1.1.11.5  1993/10/18  19:32:13  tom
 * 	Bug 8313 - Help DEC out by including rpc and stubbase header files
 * 	before system includes.
 * 	[1993/10/18  19:31:47  tom]
 * 
 * Revision 1.1.11.4  1993/10/14  22:19:40  tom
 * 	Bug 8090 - Remove sigint_handler which was unused.
 * 	[1993/10/14  22:17:16  tom]
 * 
 * Revision 1.1.11.3  1993/09/07  20:45:12  tom
 * 	Bug 8560 - Use strncpy to make sure we don't overwrite static storage.
 * 	Don't use global variable target as local variable in usage.
 * 	[1993/09/07  20:45:00  tom]
 * 
 * Revision 1.1.11.1  1993/09/01  17:21:34  tom
 * 	Bug 7774 - Fix up help messsages for mapping commands.
 * 	If REMOTE_ENDPOINT_ACCESS is defined, show optional host address,
 * 	otherwise, show nothing.  Note reqired options in help.
 * 	[1993/09/01  16:12:06  tom]
 * 
 * Revision 1.1.9.3  1993/08/20  20:44:58  tom
 * 	Bug 7627 - print errors to stderr.
 * 	[1993/08/20  20:30:19  tom]
 * 
 * Revision 1.1.9.2  1993/08/16  18:50:57  ganni
 * 	CR 7523 fix: display correct -e usage for import command.
 * 	[1993/08/16  18:50:19  ganni]
 * 
 * Revision 1.1.5.9  1993/01/27  14:30:01  markar
 * 	     OT CR 6215 fix: fix -v usage for show/remove mapping commands.
 * 	[1993/01/22  16:07:25  markar]
 * 
 * Revision 1.1.5.8  1993/01/14  21:02:14  jd
 * 	sys/fcntl -> fcntl. RIOS couldn't find former.
 * 	[1993/01/14  18:55:27  jd]
 * 
 * Revision 1.1.5.7  1993/01/13  21:44:31  jd
 * 	The cma package messes with tty settings and doesn't always
 * 	get a chance to put them back. The program should have its
 * 	own file descriptor in this case so as not to do strange
 * 	things to the calling shell.
 * 	[1993/01/13  20:49:49  jd]
 * 
 * Revision 1.1.5.6  1993/01/07  16:58:37  tom
 * 	Bug 6412 - Add setlocale call for i18n.
 * 	[1993/01/05  23:22:56  tom]
 * 
 * Revision 1.1.5.5  1993/01/03  22:37:18  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:54:13  bbelch]
 * 
 * Revision 1.1.5.4  1992/12/23  19:41:09  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:55:02  zeliff]
 * 
 * Revision 1.1.5.3  1992/11/05  16:45:51  grace
 * 	      09-oct-92 dm        require -i on import
 * 	      07-oct-92 jm        using = instead of ==
 * 	[1992/11/05  15:29:26  grace]
 * 
 * Revision 1.1.5.2  1992/09/29  21:13:06  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:35:52  weisman]
 * 
 * Revision 1.1.2.3  1992/07/07  21:27:37  rsalz
 * 	 20-may-92 tw        add fix for rpccp show
 * 	[1992/07/07  21:23:14  rsalz]
 * 
 * Revision 1.1.2.2  1992/05/01  18:36:20  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  18:24:36  rsalz]
 * 
 * 	 02-apr-92 tw        remove sys/ from include errno.h
 * 	 30-mar-92 tw        add fix for "help add (show and remove)"
 * 	 10-feb-92 vj        In remove_element procedure, only compare
 * 	                     annotation strings an element was returned.
 * 	 05-feb-92 vj        Check for null member name in add_element.
 * 	 29-jan-92 mishkin   add add-mapping.
 * 	[1992/05/01  18:04:52  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:14:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      rpccp.c
**
**  FACILITY:
**
**      RPC
**
**  ABSTRACT:
**
**      Management utility for DCE RPC runtime functions.
**
**
*/

/*
 * Don't inherit stubbase.h's use of the exception based pthreads API.
 */
#ifndef NCK
#  define NCK
#endif
#include <dce/rpc.h>
#include <dce/stubbase.h>

#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>
#include <fcntl.h>

#include <dce/pthread_exc.h>
#include <dce/exc_handling.h>
#ifdef BROKEN_CMA_EXC_HANDLING
   extern EXCEPTION cma_e_alerted;
#  define pthread_cancel_e  cma_e_alerted
#endif

#include <dce/dce_error.h>

/***************************************************************************
 *
 * Local definitions
 *
 **************************************************************************/
#define PRIVATE     extern
#define INTERNAL    static

#define MYNAME                          "rpccp"
#define WHITE_SPACE                     " \t"

#define MAX_COMMAND_LENGTH              4096
#define MAX_COMMAND_NAME_LENGTH         32
#define MAX_COMMAND_ARGS                128
#define MAX_OPTIONS                     16
#define MAX_ANNOTATION_LENGTH           256
#define MAX_MEMBER_NAME_LENGTH          1024
#define MAX_BINDINGS                    32
#define MAX_OBJECTS                     32
#define MAX_IF_ID_STRING_LENGTH         36 + 9 + 9 + 1
#define MAX_UNSIGNED                    0xFFFFFFFFU

#define FIRST_OPERATION                 3   /* after "exit", "quit" & "help" */
#define EXIT_OPERATION                  0
#define QUIT_OPERATION                  1
#define HELP_OPERATION                  2
#define SHOW_ENTRY                      3
#define ADD_ENTRY                       4
#define REMOVE_ENTRY                    5
#define SHOW_GROUP                      6
#define REMOVE_GROUP                    7
#define ADD_MEMBER                      8
#define REMOVE_MEMBER                   9
#define SHOW_PROFILE                    10
#define REMOVE_PROFILE                  11
#define ADD_ELEMENT                     12
#define REMOVE_ELEMENT                  13
#define SHOW_SERVER                     14
#define EXPORT                          15
#define UNEXPORT                        16
#define IMPORT                          17
#define SHOW_MAPPING                    18
#define REMOVE_MAPPING                  19
#define ADD_MAPPING                     20
#define MAX_OPERATIONS                  21

#define CHECK_RPC_STATUS() \
{ \
    if (rpc_status != rpc_s_ok) \
    { \
        REPORT_DCE_ERROR(); \
        return (false); \
    } \
}

#define REPORT_DCE_ERROR() \
{ \
    dce_error_inq_text (rpc_status, error_message, NULL); \
    fprintf (stderr, "\n>>> rpc runtime error: %s\n\n", error_message); \
}

/***************************************************************************
 *
 * Forward Declarations
 *
 **************************************************************************/

INTERNAL boolean32 main_loop _DCE_PROTOTYPE_((int, char **));

INTERNAL boolean32 show_entry _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 add_entry _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 remove_entry _DCE_PROTOTYPE_((unsigned_char_p_t));

INTERNAL boolean32 show_group _DCE_PROTOTYPE_((unsigned_char_p_t, int, boolean32));
INTERNAL boolean32 remove_group _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 add_member _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 remove_member _DCE_PROTOTYPE_((unsigned_char_p_t));

INTERNAL boolean32 show_profile _DCE_PROTOTYPE_((unsigned_char_p_t, int, boolean32));
INTERNAL boolean32 remove_profile _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 add_element _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 remove_element _DCE_PROTOTYPE_((unsigned_char_p_t));

INTERNAL boolean32 show_mapping _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 remove_mapping _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 add_mapping _DCE_PROTOTYPE_((void));

INTERNAL boolean32 show_server _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 export _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 unexport _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 import _DCE_PROTOTYPE_((unsigned_char_p_t, rpc_if_id_p_t, boolean32));

INTERNAL boolean32 get_binding _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 get_interface_id _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 get_object _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 get_name_syntax _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 get_version_option _DCE_PROTOTYPE_((unsigned_char_p_t));
INTERNAL boolean32 expand_name _DCE_PROTOTYPE_((unsigned_char_p_t, unsigned_char_p_t *));

PRIVATE boolean32 rpccp_help _DCE_PROTOTYPE_((unsigned_char_p_t *));

/*
 * other utilities
 */
PRIVATE boolean32 usage _DCE_PROTOTYPE_((unsigned_char_p_t *));
INTERNAL int get_options _DCE_PROTOTYPE_((int, char **, char *));
PRIVATE char *get_string _DCE_PROTOTYPE_((char *, char *));

/***************************************************************************
 *
 * Data Declarations
 *
 **************************************************************************/

#include <errno.h>

char                *optarg;
int                 option_index;
boolean32           option_error;
int                 operation_number;
char                *target = NULL;

unsigned32          rpc_status;
dce_error_string_t  error_message;
 
unsigned_char_p_t   cmd_tokens[MAX_COMMAND_ARGS];
unsigned32          cmd_tokens_count;

unsigned_char_t     annotation[MAX_ANNOTATION_LENGTH];
boolean32           default_profile;
boolean32           binding_entry;
boolean32           all_interfaces;
boolean32           all_objects;
boolean32           update_data;
boolean32           no_replace;

rpc_if_id_t         interface_id;
unsigned_char_p_t   member_name = NULL;
unsigned32          name_syntax = rpc_c_ns_syntax_default;
unsigned32          number_count;
unsigned8           priority;
unsigned32          recursion_count;
unsigned32          version_option;

struct binding_vec
{
    unsigned32              count;
    rpc_binding_handle_t    binding_h[MAX_BINDINGS];
} binding_vector = {0};

struct uuid_vec
{
    unsigned32              count;
    uuid_p_t                uuid[MAX_OBJECTS];
} object_vector;

uuid_t          objects[MAX_OBJECTS];


/*
 * the NDR transfer syntax id
 */
rpc_syntax_id_t NIDL_ndr_id =
{
    {0x8a885d04U, 0x1ceb, 0x11c9, 0x9f, 0xe8,
    {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}}, 2
};


/*
 * for now, dummy up an if spec
 */
rpc_if_rep_t    interface_specification =
{
    0,                  /* ifspec rep version                           */
    0,                  /* operation count (not needed)                 */
    0,                  /* interface version (to be filled in later)    */
    { 0 },              /* interface uuid (to be filled in later)       */
    2,                  /* stub/runtime interface version number        */
    { 0, NULL },        /* endpoint vector (not needed)                 */
    { 1, &NIDL_ndr_id}, /* NDR transfer syntax (the only one for now)   */
    NULL,               /* server stub epv (not needed)                 */
    NULL                /* manager stub epv (not needed)                */
};

/*
 * Table of operations. To add an operation to this facility, create an
 * entry in this table AFTER the "help" entry, add new option 
 * characters to the parsing switch
 * statement in the main loop (if required), add an entry to the dispatching
 * switch statement in the main loop, and add a routine to perform the
 * operation. HELP_OPERATION is FIRST_OPERATION - 1. Don't forget to 
 * redefine MAX_OPERATIONS. Modify the 'opt_strs' table (below) as well.
 */
struct
{
    char    *display_name;
    char    *name;
    char    *options;
    char    *short_help;
    char    *long_help[MAX_OPTIONS];
} operation[MAX_OPERATIONS] =

    {
        { "exit", "exit", "", "", { "" } },
        { "quit", "quit", "", "", { "" } },
        { "help", "help", "", "", { "" } },

        /* 
         * This is indexed by FIRST_OPERATION:
         */
        { "show entry", "show_entry", "i:s:u", "isu",
            {
                "i    <if id>   interface identifier",
                "s    <syntax>  name syntax",
                "u              update local name-service data"
            }
        },
    
        { "add entry", "add_entry", "s:", "s",
            {
                "s    <syntax>  name syntax"
            }
        },
    
        { "remove entry", "remove_entry", "s:", "s",
            {
                "s    <syntax>  name syntax"
            }
        },
    
        { "show group", "show_group", "m:r;s:u", "mrsu",
            {
                "m    <name>    member name",
                "r    <count>   recursion level",
                "s    <syntax>  name syntax",
                "u              update local name-service data"
            }
        },
    
        { "remove group", "remove_group", "s:", "s",
            {
                "s    <syntax>  name syntax"
            }
        },
    
        { "add member", "add_member", "m:s:", "ms",
            {
                "m    <name>    member name	(required)",
                "s    <syntax>  name syntax"
            }
        },
    
        { "remove member", "remove_member", "m:s:", "ms",
            {
                "m    <name>    member name	(required)",
                "s    <syntax>  name syntax"
            }
        },
    
        { "show profile", "show_profile", "a:di:m:r;s:uv:", "adimrsuv",
            {
                "a    <string>  annotation field",
                "d              default profile element",
                "i    <if id>   interface identifier",
                "m    <name>    member name",
                "r    <count>   recursion level",
                "s    <syntax>  name syntax",
                "u              update local name-service data",
                "v    <option>  interface version option"
            }
        },
    
        { "remove profile", "remove_profile", "s:", "s",
            {
                "s    <syntax>  name syntax"
            }
        },
    
        { "add element", "add_element", "a:di:m:p:s:", "adimps",
            {
                "a    <string>  annotation field",
                "d              default profile element",
                "i    <if id>   interface identifier",
                "m    <name>    member name		(required)",
                "p    <0-7>     priority value",
                "s    <syntax>  name syntax"
            }
        },
    
       { "remove element", "remove_element", "a:di:m:s:", "adims",
            {
                "a    <string>  annotation field",
                "d              default profile element",
                "i    <if id>   interface identifier",
                "m    <name>    member name",
                "s    <syntax>  name syntax"
            }
        },
                                                                               
        { "show server", "show_server", "i;o;s:u", "iosu",
            {
                "i    <if id>   interface identifier",
                "o    <uuid>    object identifier",
                "s    <syntax>  name syntax",
                "u              update local name-service data"
            }
        },

        { "export", "export", "b:i:o:s:", "bios",
            {
                "b    <string>  string binding",
                "i    <if id>   interface identifier",
                "o    <uuid>    object identifier",
                "s    <syntax>  name syntax"
            }
        },
    
        { "unexport", "unexport", "i:o:s:v:", "iosv",
            {
                "i    <if id>   interface identifier",
                "o    <uuid>    object identifier",
                "s    <syntax>  name syntax",
                "v    <option>  interface version option"
            }
        },
    
        { "import", "import", "ei:n;o:s:uv:", "einosuv",
            {
                "e              display binding entry name",
                "i    <if id>   interface identifier",
                "n    <count>   return count",
                "o    <uuid>    object identifier",
                "s    <syntax>  name syntax",
                "u              update local name-service data",
                "v    <option>  interface version option"
            }
        },

        { "show mapping", "show_mapping", "i:o:v:", "iov",
            {
                "i    <if id>   interface identifier",
                "o    <uuid>    object identifier",
                "v    <option>  interface version option"
            }
        },

        { "remove mapping", "remove_mapping", "b:i:o:", "bio",
            {
                "b    <string>  string binding 		(required)",
                "i    <if id>   interface identifier 	(required)",
                "o    <uuid>    object identifier"
            }
        },

        { "add mapping", "add_mapping", "a:b:i:o:N", "abioN",
            {
                "a    <string>  annotation field",
                "b    <string>  string binding 		(required)",
                "i    <if id>   interface identifier 	(required)",
                "o    <uuid>    object identifier",
                "N              no replace"
            }
        }
    };

int fd_stdin;


/***************************************************************************
 *
 * Main procedure
 *
 **************************************************************************/

main (
  int         argc,
  char        **argv
)
{
    char        command[MAX_COMMAND_LENGTH];
    char        *char_ptr;
    int         arg_count;
    char        *arg_vector[MAX_COMMAND_ARGS];

    setlocale(LC_ALL, "");

#ifdef CMA_INCLUDE
    /*
     * for now, make sure threads are initialized
     */
    cma_init();
#endif
    /*
     * The cma package messes with tty settings and doesn't always
     * get a chance to put them back. The program should have its
     * own file descriptor in this case so as not to do strange 
     * things to the calling shell.
     */
    fd_stdin = 0;
    if(isatty(fileno(stdin)))
       {
	 close(fileno(stdin));
	 fd_stdin = open("/dev/tty",O_RDWR);
       }

    
    /*
     * see if we were invoked with operations
     */
    if (argc >= 2)
    {
        /*
         * pass the arguments along and execute the specified operation
         */
        TRY

        {
           if (main_loop (argc, argv))
            {
                exit(0);
            }
            else
            {
                exit(-1);
            }
        }

        CATCH (pthread_cancel_e)
        {
            fprintf (stderr, "\n>>> CANCEL -- Exception caught! Exit ...\n\n");
        }

        CATCH_ALL
        {
            fprintf (stderr, "\n>>> Exception! ...\n\n");
            RERAISE;
        }

        ENDTRY;

        /*
         * Only time we'll get here is on a cancel. Return an error
         * in case we were running in a script and were cancelled 
         * via kill. This allows the script to decide whether to
         * continue.
         */
        exit (-1);
    }


    /*
     * Enter the interactive loop (until forced out by the user).
     * Program never exits here, rather, it calls exit() from within
     * main_loop().
     */
    for (;;)
    {
        /*
         * initialize loop variables
         */
        char_ptr = command;
        arg_count = 1;
        
        /*
         * wait for a command from the user
         */
        if (get_string ("rpccp> ", command) == NULL)
        {
            exit(-1);
        }

        /*
         * set up an argument vector, using a dummy first argument
         */
        for (;;)
        {
            /*
             * locate the first non-whitespace character, and use it
             * as the start of an argument; be careful to handle trailing
             * whitspace
             */
            char_ptr += strspn (char_ptr, WHITE_SPACE);

            if (*char_ptr == '\0')
            {
                break;
            }

            /*
             * see if there is line continuation
             */
            if (*char_ptr == '\\')
            {
                /*
                 * add the next line to the current one and keep going
                 */
                if (get_string (">", char_ptr) == NULL)
                {
                    exit(-1);
                }
            }
            else
            {                    
                arg_vector[arg_count++] = char_ptr;

                /*
                 * locate the end of the argument
                 */
                char_ptr += strcspn (char_ptr, WHITE_SPACE);

                /*
                 * see if this is the end of the line
                 */
                if (*char_ptr == '\0')
                {
                    break;
                }
                else
                {
                    /*
                     * mark the end of the argument
                     */
                    *char_ptr++ = '\0';
                }
            }
        }

        TRY

        {
            main_loop (arg_count, arg_vector);
        }

        CATCH (pthread_cancel_e)
        {
            fprintf (stderr, "\n>>> CANCEL -- Exception caught! Continuing ...\n\n");
        }

        CATCH_ALL
        {
            fprintf (stderr, "\n>>> Exception! ...\n\n");
            RERAISE;
        }

        ENDTRY;
    }
}

INTERNAL boolean32 main_loop (
    int         argc,
    char        **argv
   )
{
    char        command_name[MAX_COMMAND_NAME_LENGTH];
    int 	c;
    boolean32   result;

    /*
     * initialize global variables
     */
    option_index = 2;
    operation_number = MAX_OPERATIONS;

    default_profile = false;
    binding_entry = false;
    all_interfaces = true;
    all_objects = true;
    update_data = false;
    no_replace = false;
    
    uuid_create_nil (&(interface_id.uuid), &rpc_status);

    CHECK_RPC_STATUS();
    
    annotation[0] = '\0';
    /*
     * The first time main_loop is called, member_name will be NULL.
     * Subsequent times it may be pointing to a string so we free 
     * the string and reset the pointer to NULL.
     */
    if (member_name != NULL)
    {
        rpc_string_free (&member_name, &rpc_status);
    }
    
    name_syntax = rpc_c_ns_syntax_default;
    number_count = 1;
    priority = 0;
    recursion_count = 0;
    version_option = rpc_c_vers_compatible;

    /*
     * The first time main_loop is called, binding_vector.count will be 0.
     * Subsequent times it may be > 0 and the bindings must be freed.
     */
    while (binding_vector.count > 0)
    {
        rpc_binding_free
            (&binding_vector.binding_h[--binding_vector.count], &rpc_status);

        CHECK_RPC_STATUS();
    }

    object_vector.count = 0;

    /*
     * see if this is one of the "two-tier" commands
     * check to see if argv[2] is less than three to
     * ensure that there is a "second tier" to these commands
     */
    if ((((strcmp (argv[1], "show")) == 0) ||
        ((strcmp (argv[1], "add")) == 0) ||
        ((strcmp (argv[1], "remove")) == 0)) &&
        (argc > 2 ))
    {
        /*
         * combine the first and second tokens
         */
        sprintf (command_name, "%s_%s", argv[1], argv[2]);

        /*
         * bump the option index past the second token
         */
        option_index++;
    }                        
    else
    {
        strncpy (command_name, argv[1], MAX_COMMAND_NAME_LENGTH);
        command_name[MAX_COMMAND_NAME_LENGTH-1] = '\0';
    }
    
    /*
     * find out the requested operation
     */
    for (operation_number = 0;
        operation_number < MAX_OPERATIONS;
        operation_number++)
    {
        if ((strcmp (command_name, operation[operation_number].name)) == 0) 
        {
            break;
        }
    }

    /*
     * Initialize the target pointer
     */
    target = NULL;

    /*
     * initialize the global command array
     */
    for (cmd_tokens_count = 0; 
         cmd_tokens_count < MAX_COMMAND_ARGS; 
         cmd_tokens_count++)
         cmd_tokens[cmd_tokens_count] = NULL;

    cmd_tokens_count = 0;

    /*
     * special case the exit command
     */
    if (operation_number == EXIT_OPERATION ||
        operation_number == QUIT_OPERATION)
    {
        exit (0);
    }

    /*
     * if not a valid operation, or no target name
     */
    if ((operation_number != HELP_OPERATION) && 
       (operation_number == MAX_OPERATIONS || argc < 3))
    {
        usage (cmd_tokens);
        return (false);
    }
    
    /*
     * parse the command line (past the operation name)
     */
    while ((c = get_options
        (argc, argv, operation[operation_number].options)) != EOF)
    {
        switch (c)
        {
            case '\0':  break;
            case 'a' :  strncpy ((char *) annotation, optarg, MAX_ANNOTATION_LENGTH); 
                        annotation[MAX_ANNOTATION_LENGTH-1] = '\0';
                        break;
            case 'b' :  if (!get_binding ((unsigned_char_p_t) optarg))
                            return (false); else break;
            case 'd' :  default_profile = true; break;
            case 'e' :  binding_entry = true; break;
            case 'i' :  if (!get_interface_id ((unsigned_char_p_t) optarg))
                            return (false); else break;
            case 'm' :  if (!expand_name (
                               (unsigned_char_p_t) optarg, &member_name))
                            return (false); else break;
            case 'n' :  if (optarg == NULL) number_count = MAX_UNSIGNED;
                            else number_count = atoi (optarg); break;
            case 'N' :  no_replace = true; break;
            case 'o' :  if (!get_object ((unsigned_char_p_t) optarg))
                            return (false); else break;
            case 'p' :  priority = atoi (optarg); break;
            case 'r' :  if (optarg == NULL) recursion_count = MAX_UNSIGNED;
                            else recursion_count = atoi (optarg); break;
            case 's' :  if (!get_name_syntax ((unsigned_char_p_t) optarg))
                            return (false); else break;
            case 'u' :  update_data = true; break;
            case 'v' :  if (!get_version_option ((unsigned_char_p_t) optarg))
                            return (false); else break;
            default  :  usage (cmd_tokens); return (false);
        }
    }

    if (option_error)
    {
        usage (cmd_tokens);
        option_error = false;
        return (false);
    }

    /*
     * Initialize these little devils.
     */
    result = false;
    rpc_status = rpc_s_ok;

    /*
     * do the requested operation on the target (last argument)
     */
    switch (operation_number)
    {
        case HELP_OPERATION:
            result = rpccp_help (cmd_tokens); break;

        case SHOW_ENTRY: 
            result = show_entry ((unsigned_char_p_t) target); break; 
        case ADD_ENTRY:
            result = add_entry ((unsigned_char_p_t) target); break;
        case REMOVE_ENTRY:
            result = remove_entry ((unsigned_char_p_t) target); break;

        case SHOW_GROUP:
            result = show_group ((unsigned_char_p_t) target, 0, false); break;
        case REMOVE_GROUP:
            result = remove_group ((unsigned_char_p_t) target); break;
        case ADD_MEMBER:
            result = add_member ((unsigned_char_p_t) target); break;
        case REMOVE_MEMBER:
            result = remove_member ((unsigned_char_p_t) target); break;

        case SHOW_PROFILE:
            result = show_profile ((unsigned_char_p_t) target, 0, false); break;
        case REMOVE_PROFILE:
            result = remove_profile ((unsigned_char_p_t) target); break;
        case ADD_ELEMENT:
            result = add_element ((unsigned_char_p_t) target); break;
        case REMOVE_ELEMENT:
            result = remove_element ((unsigned_char_p_t) target); break;

        case SHOW_SERVER:
            result = show_server ((unsigned_char_p_t) target); break;
        case EXPORT:
            result = export ((unsigned_char_p_t) target); break;
        case UNEXPORT:
            result = unexport ((unsigned_char_p_t) target); break;
        case IMPORT:
            result = import ((unsigned_char_p_t) target, &interface_id, false);
            break;

        case SHOW_MAPPING:
            result = show_mapping ((unsigned_char_p_t) target); break;
        case REMOVE_MAPPING:
            result = remove_mapping ((unsigned_char_p_t) target); break;
        case ADD_MAPPING:
            result = add_mapping (); break;
    }

    /*
     * If all was well with the command execution, return success.
     */
    if (result == true)
    {
        return (true);
    }
    else
    {
        /*
         * If the command was ok, but the arg list wasn't, display
         * the usage help.
         */
        if (rpc_status == rpc_s_ok)
        {
            usage (cmd_tokens);
        }

        /*
         * and then return a failure.
         */
        return (false);
    }
}

/***************************************************************************
 *
 * Subroutines
 *
 **************************************************************************/

/*
 * This routine parses the command line *after* the operation name
 * (that is, it ignores the first two words in the line, the program
 * name and the operation name), and returns:
 *
 *  - the first character of any command line switch, and
 *  - the value of the argument specified by the switch
 *
 * Note several diferences from "standard" Unix getopt fare:
 *
 *  -   semi-colon can be used in options strings to indicate optional
 *      arguments (in addition to colon for required arguments)
 *
 *  -   the switch character must always be directly after the '-' (that is,
 *      it is not permitted to concatenate multiple switches into one)
 *
 *  -   the routine will print an error message and exit if an illegal
 *      switch or syntax is detected
 *
 *  -   any token that doesn't start with a - will be saved in the
 *      variable 'token' and parsing will continue ('\0' will be returned)
 *
 *  -   a hyphen without a switch character is permitted ('\0' will be
 *      returned) so it can be used to allow a switch with optional arguments
 *      to precede some other non-switch token
 *
 *  -   as a side-effect, the global variable "target" is set to point to 
 *      argv[option_index][0] if there is no switch delimetter char.
 *
 *  -   likewise, all non-option tokens on the command line saved in the
 *      global array cmd_tokens[].
 *
 */
 
INTERNAL int get_options
(
    int         argc,
    char        **argv,
    char         *opts
)

{
    int         switch_ptr = 1;
    char        c,*option_ptr;

    optarg = NULL;
    option_error = false;

    /*
     * if the option index points past the last token, leave now
     */
    if (option_index >= argc)
    {
        return (EOF);
    }
    
    /*
     * see if it points to a switch
     */
    if (argv[option_index][0] != '-')
    {
        /*
         * Put pointer to current non-switch token in array. Keep 'target'
         * for backwards compatibility.
         */
        cmd_tokens[cmd_tokens_count++] = (unsigned_char_p_t)argv[option_index];
        target = argv[option_index++];
        return ('\0');
    }

    /*
     * see if there's a switch character
     */
    if (argv[option_index][1] == '\0')
    {
        option_index++;
        return ('\0');
    }

    /*
     * see if this is the end of parsing
     */
    if (strcmp (argv[option_index], "--") == 0)
    {
        /*
         * if so, just bump the option index for future reference
         */
        option_index++;
        return (EOF);
    }

    /*
     * get the first character of the option switch (past the '-')
     */
    c = argv[option_index][switch_ptr];

    /*
     * see if the first character is in the specified option set
     * (note: colon and semi-colon cannot be used as first characters,
     * because they are used as markers in the option set)
     */
    if (c == ':' || c == ';' ||
        (option_ptr = (char *) strchr (opts, (int) c)) == 0)
    {
        option_error = true;
        fprintf (stderr, "\n>>> command syntax error: illegal option -- %c\n", c);
        return (EOF);
    }

    /*
     * see what kind of option argument we have here -
     * colon for required, semi-colon for optional
     */
    option_ptr++;
    
    if (*option_ptr == ':' || *option_ptr == ';')
    {
        /*
         * see if the argument is appended to the switch (no space)
         */
        if (argv[option_index][switch_ptr+1] != '\0')
        {
            /*
             * if so, use the rest of the switch as the argument
             */
            optarg = &argv[option_index++][switch_ptr+1];
        }
        else
        {
            /*
             * if the argument is optional, see if next token is a switch
             * (if there is one)
             */
            if (*option_ptr == ';')
            {
                if (++option_index >= argc || argv[option_index][0] == '-')
                {
                    /*
                     * mark the option argument as omitted and leave
                     */
                    optarg = NULL;
                }
                else
                {
                    /*
                     * use the next token as the option argument
                     */
                    optarg = argv[option_index++];
                }
            }
            else
            {
                /*
                 * see if there are any more tokens to use for the argument
                 */
                if (++option_index >= argc)
                {
                    option_error = true;
                    fprintf
      (stderr, "\n>>> command syntax error: option requires an argument -- %c\n", c);
                    return (EOF);
                }
                else
                {
                    /*
                     * use the next token as the option argument
                     */
                    optarg = argv[option_index++];
                }
            }
        }
    }
    else
    {
        /*
         * if there is no option argument required, mark the argument as
         * empty, bump the option index, and return the switch character
         */
        optarg = (char *) NULL;
        option_index++;
    }

    return (c);
}

/*
 * get_string
 *
 * Get the next input string.  This is a thread-synchronous version of
 * what gets() does.
 *
 * Inputs:
 *	string = "char" pointer to string.
 *
 * Outputs:
 *      "char" pointer to non-blank character in string (control chars
 *	are considered blanks).
 */     
PRIVATE char *get_string (
  char        *prompt,
  char        *string
)
{
    char        c;
    char        *cs;
    int         cnt;

    do
    {
        cs = string;

 	if (isatty(fileno(stdin)))
            printf (prompt);
        fflush (stdout);
        fflush (stderr);

        do
        {
            cnt = read (fd_stdin, &c, 1);
            if (cnt == 1)
            {
                *cs++ = c;
            }
            else if (cnt == 0)
            {
                return (NULL);
            }
            else if (cnt < 0)
            {
                if (errno != EINTR) return (NULL);
                continue;
            }
        } while (c != '\n');
    
        *(cs - 1) = '\0';       /* Step on \n */

    } while (strlen(string) == 0);

    return (string);
}

/*
 * usage - display usage message
 */
PRIVATE boolean32 usage (
unsigned char    **cmd_tokens
)
{
    int         operation_number = MAX_OPERATIONS;
    char        command_name[MAX_COMMAND_NAME_LENGTH];
    int         i;
    char        *entry_usage;


    if (cmd_tokens[0] != NULL)
    {
        /*
         * see if this is one of the "two-tier" commands
         */
        if ((((strcmp ((char *)cmd_tokens[0], "show")) == 0) ||
            ((strcmp ((char *)cmd_tokens[0], "add")) == 0) ||
            ((strcmp ((char *)cmd_tokens[0], "remove")) == 0)) &&
            (cmd_tokens[1] != NULL))
        {
            /*
             * combine the first and second tokens
             */
            sprintf (command_name, "%s_%s",
                     (char *)cmd_tokens[0], cmd_tokens[1]);
        }                        
        else
        {
            strncpy (command_name, (char *)cmd_tokens[0],
                     MAX_COMMAND_NAME_LENGTH);
            command_name[MAX_COMMAND_NAME_LENGTH-1] = '\0';
        }
    
        /*
         * find out the requested operation
         */
        for (operation_number = 0;
            operation_number < MAX_OPERATIONS;
            operation_number++)
        {
            if ((strcmp (command_name, operation[operation_number].name)) == 0) 
            {
                break;
            }
        }
    }

    /*
     * no valid operation was selected - show them all
     */
    if (operation_number == MAX_OPERATIONS)
    {
        printf ("\nusage: %s operation [options[args]] entry_name\n\n", 
                MYNAME);
        printf ("operations:  quit (or exit)\n");
        printf ("             help\n");

        for (operation_number = FIRST_OPERATION;
            operation_number < MAX_OPERATIONS;
            operation_number++)
        {
            if ((strlen (operation[operation_number].short_help)) != 0)
            {
                printf ("             %s  -%s\n",
                    operation[operation_number].display_name,
                    operation[operation_number].short_help);
            }
            else
            {
                printf ("             %s  \n",
                    operation[operation_number].display_name);
            }
        }

        printf ("\n");
    }
    else
    {    
        /*
         * show the arguments for the selected operation
         */
        if ((strlen (operation[operation_number].short_help)) != 0)
        {
            switch(operation_number)
            {
                case ADD_MAPPING:
                    entry_usage = "";
                    break;

                case REMOVE_MAPPING:
#ifdef REMOTE_ENDPOINT_ACCESS
                    entry_usage = "[host-address]";
#else
                    entry_usage = "";
#endif /* REMOTE_ENDPOINT_ACCESS */
                    break;

                case SHOW_MAPPING:
                    entry_usage = "[host-address]";
                    break;

                default:
                    entry_usage = "<entry name>";
                    break;

            }
            printf ("\nusage: %s %s [<options>] %s\n\n",
                MYNAME, operation[operation_number].display_name, entry_usage);
  
            printf (
                 "options:     %s\n", operation[operation_number].long_help[0]);

            for (i = 1;
                i < strlen (operation[operation_number].short_help); i++)
            {
                printf ("             %s\n",
                    operation[operation_number].long_help[i]);
            }
        }
        else
        /*
         * short_help & long_help fields are used differently for two part 
         * commands like "show entry", etc.
         */
        {
            printf ("\nusage: %s %s\n",
                MYNAME, operation[operation_number].long_help[0]);
        }

        printf ("\n");
    }

    return (true);
}

INTERNAL boolean32 get_binding (
    unsigned_char_p_t   option_value
)
{
    char    *string_binding;


    /*
     * if there is a translation for the option value, use it
     */

    if (option_value != NULL)
    {
        if ((string_binding = (char *) (getenv ((char *) option_value))) ==
             NULL)
         {
             string_binding = (char *) option_value;
         }
    }
    else
    {
        string_binding = NULL;
    }

    /*
     * convert the string binding into a binding handle in the binding vector
     */
    rpc_binding_from_string_binding (
        (unsigned_char_p_t) string_binding,
        &(binding_vector.binding_h[binding_vector.count]),
        &rpc_status);

    /*
     * mark it as added to the vector
     */
    binding_vector.count++;

    CHECK_RPC_STATUS();

    return (true);
}

INTERNAL boolean32 get_interface_id (
   unsigned_char_p_t   option_value
)
{
    char    *string_if_id;
    char    interface_uuid[MAX_IF_ID_STRING_LENGTH];


    /*
     * if there is a translation for the option value, use it
     */
    if (option_value != NULL)
    {
        if ((string_if_id = (char *) (getenv ((char *) option_value))) ==
             NULL)
         {
             string_if_id = (char *) option_value;
         }
    }
    else
    {
        string_if_id = NULL;
    }

    /*
     * first check for interface wildcarding
     */
    if (string_if_id == NULL)
    {
        all_interfaces = true;
    }
    else
    {
        all_interfaces = false;

        /*
         * get the interface uuid string and the version numbers
         */
        sscanf (string_if_id, "%[^,],%hd.%hd",
            interface_uuid,
            &(interface_id.vers_major),
            &(interface_id.vers_minor));

        uuid_from_string ((unsigned_char_p_t) interface_uuid, 
                            &(interface_id.uuid), &rpc_status);

        CHECK_RPC_STATUS();
    }

    return (true);
}

INTERNAL boolean32 get_name_syntax (
    unsigned_char_p_t   option_value
)
{
    if ((strcmp ((char *) option_value, "dec_dns")) == 0)
        name_syntax = rpc_c_ns_syntax_dec_dns;
    else if ((strcmp ((char *) option_value, "dce")) == 0)
        name_syntax = rpc_c_ns_syntax_dce;
    else
    {
        fprintf (stderr, "\n>>> invalid name syntax: %s\n\n", (char *) option_value);
        return (false);
    }

    return (true);
}

INTERNAL boolean32 get_object (
    unsigned_char_p_t   option_value
)
{
    char    *string_object_uuid;
    

    /*
     * if there is a translation for the option value, use it
     */
    if (option_value != NULL)
    {
        if ((string_object_uuid = (char *) (getenv ((char *) option_value))) ==
             NULL)
         {
             string_object_uuid = (char *) option_value;
         }
    }
    else
    {
        string_object_uuid = NULL;
    }
      
    /*
     * first check for object wildcarding
     */
    if (string_object_uuid == NULL)
    {
        all_objects = true;
    }
    else
    {
        all_objects = false;
    
        /*
         * convert the string uuid to a real uuid and add to the array
         */
        uuid_from_string (
            (unsigned_char_p_t) string_object_uuid,
            &(objects[object_vector.count]),
            &rpc_status);

        /*
         * add a pointer to the array element to the vector and mark it as
         * added
         */
        object_vector.uuid[object_vector.count] =
            &(objects[object_vector.count]);

        object_vector.count++;

        CHECK_RPC_STATUS();
    }

    return (true);
}

INTERNAL boolean32 get_version_option (option_value)

unsigned_char_p_t   option_value;

{
    if ((strcmp ((char *) option_value, "all")) == 0)
        version_option = rpc_c_vers_all;
    else if ((strcmp ((char *) option_value, "compatible")) == 0)
        version_option = rpc_c_vers_compatible;
    else if ((strcmp ((char *) option_value, "exact")) == 0)
        version_option = rpc_c_vers_exact;
    else if ((strcmp ((char *) option_value, "major_only")) == 0)
        version_option = rpc_c_vers_major_only;
    else if ((strcmp ((char *) option_value, "upto")) == 0)
        version_option = rpc_c_vers_upto;
    else
    {
        fprintf (stderr, "\n>>> invalid version option: %s\n\n", (char *) option_value);
        return (false);
    }

    return (true);
}

/***************************************************************************
 *
 * Operations
 *
 **************************************************************************/

INTERNAL boolean32 show_entry (
    unsigned_char_p_t   entry_name
)
{
    if (entry_name == NULL) return (false);

    /*
     * show the server attributes of this entry
     */
    if (show_server (entry_name))
    {
        /*
         * show the group attributes of this entry
         */
        if (show_group (entry_name, 0, true))
        {
            /*
             * show the profile attributes of this entry
             */
            return (show_profile (entry_name, 0, true));
        }
    }

    return (false);
}

INTERNAL boolean32 add_entry (entry_name)

unsigned_char_p_t   entry_name;

{
    if (entry_name == NULL) return (false);

    rpc_ns_mgmt_entry_create (
        name_syntax,
        entry_name,
        &rpc_status);
        
    CHECK_RPC_STATUS();

    printf ("\n>>> entry added\n\n");

    return (true);
}

INTERNAL boolean32 remove_entry (
    unsigned_char_p_t   entry_name
)
{
    if (entry_name == NULL) return (false);

    rpc_ns_mgmt_entry_delete (
        name_syntax,
        entry_name,
        &rpc_status);
        
    CHECK_RPC_STATUS();

    printf ("\n>>> entry removed\n\n");

    return (true);
}

INTERNAL boolean32 show_group (
    unsigned_char_p_t   entry_name,
    int                 current_recursion_level,
    boolean32           embedded
)
{
    rpc_ns_inq_handle_t inquiry_context;
    unsigned_char_p_t   group_member_name;
    boolean32           members_found = false;
    boolean32           print_member = false;
    int                 i;
    unsigned32          temp_status;

    if (entry_name == NULL) return (false);

    rpc_ns_group_mbr_inq_begin (
        name_syntax,
        entry_name, 
        name_syntax,
        &inquiry_context,
        &rpc_status);

    CHECK_RPC_STATUS();
    
    /* 
     * Check if the caller wants us to update the local name-service
     * data. If so, set the handle expiration age so that we bypass
     * the cache.
     */
    if (update_data)
    {
        rpc_ns_mgmt_handle_set_exp_age (inquiry_context, 
            0, &rpc_status);
        CHECK_RPC_STATUS();
    }
        
    do
    {
        rpc_ns_group_mbr_inq_next (
            inquiry_context,
            &group_member_name,
            &rpc_status);

        if (rpc_status == rpc_s_entry_not_found)
        {
            /*
             * return and show error text if this is the top
             * (lowest number) level
             */
            if (current_recursion_level == 0)
            {
                CHECK_RPC_STATUS();
            }

            /*
             * otherwise, display message embedded in output
             */
            fprintf (stderr, "\n>>> entry not found: %s\n\n", entry_name);
            members_found = true;
            break;
        }

        if (rpc_status != rpc_s_no_more_members)
        {
            if (rpc_status != rpc_s_ok)
            {
                rpc_ns_group_mbr_inq_done (&inquiry_context, &temp_status);

                REPORT_DCE_ERROR();
                return (false);
            }

            print_member = false;
                
            if (member_name == NULL)
            {
                print_member = true;
            }
            else
            {
                if (strcmp
                    ((char *) group_member_name, (char *) member_name) == 0)
                {
                    print_member = true;
                }
            }

            if (print_member)
            {
                if (current_recursion_level == 0 && !members_found)
                {
                    printf ("\ngroup members:\n\n");
                }
    
                members_found = true;

                /*
                 * indent as appropriate for this recursion level
                 */
                for (i = 0; i < current_recursion_level; i++)
                {
                    printf ("  ");
                }

                printf ("  %s\n", group_member_name);
            
                /*
                 * recursively expand the next level of group nesting
                 */
                if (current_recursion_level < recursion_count)
                {
                    if (!show_group (group_member_name,
                        current_recursion_level + 1, embedded))
                    {
                        return (false);
                    }
                }
            }

            rpc_string_free (&group_member_name, &rpc_status);
                
            CHECK_RPC_STATUS();
        }
    } while (rpc_status != rpc_s_no_more_members);

    rpc_ns_group_mbr_inq_done (&inquiry_context, &rpc_status);

    CHECK_RPC_STATUS();

    if (current_recursion_level == 0)
    {
        if (members_found)
        {
            printf ("\n");
        }
        else
        {
            if (!embedded)
            {
                printf ("\n>>> no matching members found\n\n");
            }
        }
    }

    return (true);
}

INTERNAL boolean32 remove_group (
unsigned_char_p_t   entry_name
)
{
    if (entry_name == NULL) return (false);

    /*
     * delete the whole group
     */
    rpc_ns_group_delete (
        name_syntax,
        entry_name,
        &rpc_status);

    CHECK_RPC_STATUS();

    printf ("\n>>> group removed\n\n");

    return (true);
}

INTERNAL boolean32 add_member (
    unsigned_char_p_t   entry_name
)
{
    if (entry_name == NULL) return (false);

    if (member_name == NULL) return (false);

    rpc_ns_group_mbr_add (
        name_syntax,
        entry_name,
        name_syntax,
        member_name,
        &rpc_status);

    CHECK_RPC_STATUS();

    printf ("\n>>> group member added\n\n");

    return (true);
}

INTERNAL boolean32 remove_member (
    unsigned_char_p_t   entry_name
)
{
    if (entry_name == NULL) return (false);

    if (member_name == NULL) return (false);

    /*
     * remove the specified member from the group
     */
    rpc_ns_group_mbr_remove (
        name_syntax,
        entry_name,
        name_syntax,
        member_name,
        &rpc_status);

    CHECK_RPC_STATUS();

    printf ("\n>>> group member removed\n\n");

    return (true);
}

INTERNAL boolean32 show_profile (
    unsigned_char_p_t   entry_name,
    int                 current_recursion_level,
    boolean32           embedded
)
{
    rpc_ns_inq_handle_t inquiry_context;
    unsigned32          inquiry_type;
    rpc_if_id_t         element_if_id;
    unsigned_char_p_t   element_member_name;
    unsigned32          element_priority;
    unsigned_char_p_t   element_annotation;
    unsigned_char_p_t   uuid_string;
    boolean32           elements_found = false;
    int                 i;
    unsigned32          temp_status;


    if (entry_name == NULL) return (false);

    /*
     * see if they want the default profile element
     */
    if (default_profile)
    {
        inquiry_type = rpc_c_profile_default_elt;
    }
    else
    {
        /*
         * see if an interface was specified
         */
        if (!uuid_is_nil (&(interface_id.uuid), &rpc_status))
        {
            /*
             * see if a member name was also specified
             */
            if ((member_name != NULL) && (*member_name != '\0'))
            {
                inquiry_type = rpc_c_profile_match_by_both;
            }
            else
            {
                inquiry_type = rpc_c_profile_match_by_if;
            }
        }
        else
        {
            /*
             * see if only a member name was specified
             */
            if ((member_name != NULL) && (*member_name != '\0'))
            {
                inquiry_type = rpc_c_profile_match_by_mbr;
            }
            else
            {
                inquiry_type = rpc_c_profile_all_elts;
            }
        }
    }

    rpc_ns_profile_elt_inq_begin (
        name_syntax,
        entry_name, 
        inquiry_type,
        &interface_id,
        version_option,
        name_syntax,
        member_name,
        &inquiry_context,
        &rpc_status);

    CHECK_RPC_STATUS();
    
    /*
     * Check if the caller to update the local name-service
     * data. If so, set the handle expiration age so that 
     * we bypass the local cache.
     */
    if (update_data)
    {
        rpc_ns_mgmt_handle_set_exp_age (inquiry_context, 0, &rpc_status);

        CHECK_RPC_STATUS();                                              
    }

    do
    {
        rpc_ns_profile_elt_inq_next (
            inquiry_context,
            &element_if_id,
            &element_member_name,
            &element_priority,
            &element_annotation,
            &rpc_status);

        if (rpc_status == rpc_s_entry_not_found)
        {
            /*
             * Return and show error text if this is the top (lowest
             * number) level
             */
            if (current_recursion_level == 0)
            {
                CHECK_RPC_STATUS();
            }
            
            /*
             * otherwise, display message embedded in output
             */
            fprintf (stderr, "\n>>> entry not found: %s\n\n", entry_name);
            elements_found = true;
            break;
        }

        if (rpc_status != rpc_s_no_more_elements)
        {
            if (rpc_status != rpc_s_ok)
            {
                rpc_ns_profile_elt_inq_done (&inquiry_context, &temp_status);

                REPORT_DCE_ERROR();
                return (false);
            }
            
            /*
             * if an annotation was specified, see if it matches the
             * one in the profile element
             */
            if (annotation[0] == '\0' || (strcmp
                ((char *) annotation, (char *) element_annotation)) == 0)
            {
                if (current_recursion_level == 0 && !elements_found)
                {
                    printf ("\nprofile elements:\n\n");
                    elements_found = true;
                }
    
                /*
                 * display any compatible interfaces
                 */
                uuid_to_string
                     (&(element_if_id.uuid), &uuid_string, &rpc_status);

                if (rpc_status != rpc_s_ok)
                {
                    rpc_ns_profile_elt_inq_done
                        (&inquiry_context, &temp_status);

                    REPORT_DCE_ERROR();
                    return (false);
                }
                
                /*
                 * indent as appropriate for this recursion level
                 */
                for (i = 0; i < current_recursion_level; i++)
                {
                    printf ("  ");
                }

                printf ("  <interface id>   %s,%d.%d\n",
                    uuid_string,
                    element_if_id.vers_major,
                    element_if_id.vers_minor);

                rpc_string_free (&uuid_string, &rpc_status);

                CHECK_RPC_STATUS();

                for (i = 0; i < current_recursion_level; i++)
                {
                    printf ("  ");
                }

                printf ("  <member_name>    %s\n", element_member_name);
            
                for (i = 0; i < current_recursion_level; i++)
                {
                    printf ("  ");
                }
                
                printf ("  <priority>       %d\n", element_priority);

                for (i = 0; i < current_recursion_level; i++)
                {
                    printf ("  ");
                }
                
                printf ("  <annotation>     %s\n\n", element_annotation);

                /*
                 * recursively expand the next level of group nesting
                 */
                if (current_recursion_level < recursion_count)
                {
                    if (!show_profile (element_member_name,
                        current_recursion_level + 1, embedded))
                    {
                        return (false);
                    }
                }

                rpc_string_free (&element_member_name, &rpc_status);

                CHECK_RPC_STATUS();
                
                rpc_string_free (&element_annotation, &rpc_status);

                CHECK_RPC_STATUS();
            }
        }
    } while (rpc_status != rpc_s_no_more_elements);

    rpc_ns_profile_elt_inq_done (&inquiry_context, &rpc_status);
    
    CHECK_RPC_STATUS();

    if (!embedded && !elements_found && (current_recursion_level == 0))
    {
        printf ("\n>>> no matching elements found\n\n");
    }

    return (true);
}

INTERNAL boolean32 remove_profile (
    unsigned_char_p_t   entry_name
)
{
    if (entry_name == NULL) return (false);

    /*
     * delete the whole profile
     */
    rpc_ns_profile_delete (
        name_syntax,
        entry_name,
        &rpc_status);

    CHECK_RPC_STATUS();

    printf ("\n>>> profile removed\n\n");

    return (true);
}

INTERNAL boolean32 add_element (
    unsigned_char_p_t   entry_name
)
{
    if ((entry_name == NULL) ||
        (member_name == NULL))
    {
        return (false);
    }

    if (default_profile)
    {
        /*
         * add the default profile element
         */
        rpc_ns_profile_elt_add (
            name_syntax,
            entry_name,
            NULL,
            name_syntax,
            member_name,
            priority,
            annotation,
            &rpc_status);

        CHECK_RPC_STATUS();

        printf ("\n>>> default profile element added\n\n");
    }
    else
    {
        /*
         * add the specified element from the profile
         */
        rpc_ns_profile_elt_add (
            name_syntax,
            entry_name,
            &interface_id,
            name_syntax,
            member_name,
            priority,
            annotation,
            &rpc_status);

        if (rpc_status != rpc_s_profile_element_replaced)
        {
            CHECK_RPC_STATUS();
        }

        printf ("\n>>> profile element added\n\n");
    }

    return (true);
}

INTERNAL boolean32 remove_element (
    unsigned_char_p_t   entry_name
)
{
    rpc_ns_inq_handle_t inquiry_context;
    rpc_if_id_t         element_if_id;
    unsigned_char_p_t   element_member_name;
    unsigned32          element_priority;
    unsigned_char_p_t   element_annotation;
    boolean32           element_found = false;
    unsigned32          temp_status;


    if (entry_name == NULL) return (false);

    if (default_profile)
    {
        /*
         * remove the default profile element
         */
        rpc_ns_profile_elt_remove (
            name_syntax,
            entry_name,
            NULL,
            name_syntax,
            member_name,
            &rpc_status);

        CHECK_RPC_STATUS();

        printf ("\n>>> default profile element removed\n\n");
    }
    else
    {
        /*
         * see if an interface was specified
         */
        if (!uuid_is_nil (&(interface_id.uuid), &rpc_status))
        {
            /*
             * in this case a member name must be specified
             */
            if ((member_name == NULL) || (*member_name == '\0'))
            {
                fprintf (stderr, "\n>>> command syntax error: member name required\n\n");
                return (false);
            }

            /*
             * remove the specified element from the profile
             */
            rpc_ns_profile_elt_remove (
                name_syntax,
                entry_name,
                &interface_id,
                name_syntax,
                member_name,
                &rpc_status);

            CHECK_RPC_STATUS();

            printf ("\n>>> profile elements removed\n\n");
        }
        else
        {
            /*
             * if no interface id, how about an annotation?
             */
            if (annotation[0] == '\0')
            {
                /*
                 * an interface, member or annotation must be specified
                 */
                fprintf (stderr, "\n>>> command syntax error: interface or annotation required\n\n");
            }
            else
            {
                /*
                 * walk the whole profile looking for matching annotations
                 */
                rpc_ns_profile_elt_inq_begin (
                    name_syntax,
                    entry_name, 
                    rpc_c_profile_all_elts,
                    &interface_id,
                    version_option,
                    name_syntax,
                    member_name,
                    &inquiry_context,
                    &rpc_status);

                CHECK_RPC_STATUS();
    
                do
                {
                    rpc_ns_profile_elt_inq_next (
                        inquiry_context,
                        &element_if_id,
                        &element_member_name,
                        &element_priority,
                        &element_annotation,
                        &rpc_status);
                
                    if (rpc_status != rpc_s_no_more_elements)
                    {
                        if (rpc_status != rpc_s_ok)
                        {
                            rpc_ns_profile_elt_inq_done
                                (&inquiry_context, &temp_status);

                            REPORT_DCE_ERROR();
                            return (false);
                        }
                        else
                        {
                            /*
                             * does the annotation specified match
                             * the one in the profile element?
                             */
                            if ((strcmp ((char *) annotation,
                            (char *) element_annotation) == 0))
                            {
                                element_found = true;

                                /*
                                 * remove the element
                                 */
                                rpc_ns_profile_elt_remove (
                                   name_syntax,
                                   entry_name,
                                   &element_if_id,
                                   name_syntax,
                                   element_member_name,
                                   &rpc_status);

                                if (rpc_status != rpc_s_ok)
                                {
                                    rpc_ns_profile_elt_inq_done
                                        (&inquiry_context, &temp_status);
 
                                    REPORT_DCE_ERROR();
                                    return (false);
                                }  
                
                                printf ("\n>>> profile elements removed\n\n");
                            }

                        }
                     

                    }
                    /*
                     * Free member name if memory
                     * was allocated.
                     */
                    if (element_member_name != NULL)
                    {
                        rpc_string_free (&element_member_name, &rpc_status);
                        CHECK_RPC_STATUS();
                    }
    

                    /*
                     * Free annotation string memory was
                     * allocated.
                     */
                    if (element_annotation != NULL)
                    {
                        rpc_string_free (&element_annotation, &rpc_status);
                        CHECK_RPC_STATUS();
                    }

                } while (rpc_status != rpc_s_no_more_elements);
                
                rpc_ns_profile_elt_inq_done (&inquiry_context, &rpc_status);

                CHECK_RPC_STATUS();

                /*
                 * special-case this report, since the runtime won't return
                 * an error in this case
                 */
                if (!element_found)
                {
                    fprintf (stderr, "\n>>> rpc runtime error: profile element not found (dce / rpc)\n\n");
                }
            }
        }
    }

    return (true);
}

INTERNAL boolean32 show_server (
    unsigned_char_p_t   entry_name
)
{
    rpc_if_id_vector_p_t    if_id_vector = NULL;
    unsigned_char_p_t       uuid_string = NULL;
    rpc_ns_inq_handle_t     inquiry_context = NULL;
    uuid_t                  object_uuid;
    boolean32               binding_info_found = false;
    boolean32               objects_found = false;
    boolean32               display_object;
    int                     i;
    boolean32               result;
    unsigned32              status = (~0);
    

    if (entry_name == NULL) return (false);

    /*
     * see if objects are specified
     */
    if (all_objects || object_vector.count > 0)
    {
        /*
         * set up an inquiry context
         */
        rpc_ns_entry_object_inq_begin (
            name_syntax,
            entry_name,
            &inquiry_context,
            &rpc_status);
            
        if (rpc_status == rpc_s_ok)
        {        
            /*
             * Check to determine whether to update the local name-service
             * data. If so, set the handle expiration age so to bypass the 
             * local cache.
             */
            if (update_data)
            {
                rpc_ns_mgmt_handle_set_exp_age
                        (inquiry_context, 0, &rpc_status);

                if (rpc_status != rpc_s_ok)
                {
                    REPORT_DCE_ERROR();
                    goto EXIT_HANDLER;
                }
            }

            /*
             * inquire all of the objects
             */
            for (display_object = false; rpc_status != rpc_s_no_more_members;
                display_object = false)
            {
                rpc_ns_entry_object_inq_next (
                    inquiry_context,
                    &object_uuid,
                    &rpc_status);

                if (rpc_status != rpc_s_no_more_members)
                {
                    if (rpc_status != rpc_s_ok)
                    {
                        REPORT_DCE_ERROR();
                        goto EXIT_HANDLER;
                    }
                
                    /*
                     * check for a match against the ones specified
                     */
                    if (all_objects)
                    {
                        display_object = true;
                    }
                    else
                    {
                        for (i = 0; i < object_vector.count; i++)
                        {
                            if (uuid_equal
                                (&object_uuid, &(objects[i]), &rpc_status))
                            {
                                display_object = true;
                                break;
                            }
                        }
                    }
                
                    if (display_object)
                    {
                        if (!objects_found)
                        {
                            printf ("\nobjects:\n\n");
                            objects_found = true;
                        }

                        uuid_to_string (
                            &object_uuid,
                            &uuid_string,
                            &rpc_status);

                        if (rpc_status != rpc_s_ok)
                        {
                            REPORT_DCE_ERROR();
                            goto EXIT_HANDLER;
                        }

                        printf ("  %s\n", uuid_string);

                        rpc_string_free (&uuid_string, &rpc_status);

                        if (rpc_status != rpc_s_ok)
                        {
                            REPORT_DCE_ERROR();
                            goto EXIT_HANDLER;
                        }
                    }
                }
            }
                
            if (!objects_found)
            {
                printf ("\n>>> no matching objects found\n");
            }
        }
    }

    /*
     * see if interfaces are specified
     */
    if (all_interfaces || !uuid_is_nil (&(interface_id.uuid), &rpc_status))
    {
        /*
         * inquire the interfaces in this entry
         */
        rpc_ns_mgmt_entry_inq_if_ids (
            name_syntax,
            entry_name,
            &if_id_vector,
            &rpc_status);
            
        if (rpc_status != rpc_s_no_interfaces_exported)
        {
            if (rpc_status != rpc_s_ok)
            {
                REPORT_DCE_ERROR();
                goto EXIT_HANDLER;
            }
        
            /*
             * walk the vector looking for compatible interfaces
             */
            for (i = 0; i < if_id_vector->count; i++)
            {
                if (all_interfaces ||
                    (uuid_equal (
                        &(if_id_vector->if_id[i]->uuid),
                        &(interface_id.uuid),
                        &rpc_status) &&
                    if_id_vector->if_id[i]->vers_major ==
                        interface_id.vers_major &&
                    if_id_vector->if_id[i]->vers_minor ==
                        interface_id.vers_minor))
                {
                    if (rpc_status != rpc_s_ok)
                    {
                        REPORT_DCE_ERROR();
                        goto EXIT_HANDLER;
                    }
        
                    if (!binding_info_found)
                    {
                        printf ("\nbinding information:\n\n");
                        binding_info_found = true;
                    }
        
                    /*
                     * display any compatible interfaces
                     */
                    uuid_to_string (
                        &(if_id_vector->if_id[i]->uuid),
                        &uuid_string,
                        &rpc_status);

                    if (rpc_status != rpc_s_ok)
                    {
                        REPORT_DCE_ERROR();
                        goto EXIT_HANDLER;
                    }                

                    printf ("  <interface id>   %s,%d.%d\n",
                        uuid_string,
                        if_id_vector->if_id[i]->vers_major,
                        if_id_vector->if_id[i]->vers_minor);

                    /*
                     * show all the bindings for this interface
                     */
                    number_count = MAX_UNSIGNED;

                    result = import (entry_name, if_id_vector->if_id[i], true);

                    printf ("\n");

                    rpc_string_free (&uuid_string, &rpc_status);

                    if (rpc_status != rpc_s_ok)
                    {
                        REPORT_DCE_ERROR();
                        goto EXIT_HANDLER;
                    }
                }
            }
        }
    }

    if (!binding_info_found)
    {
        printf ("\n>>> no matching binding information found\n\n");
    }


EXIT_HANDLER:

    if (inquiry_context != NULL)
    {
        rpc_ns_entry_object_inq_done (&inquiry_context, &status);
    }

    if (if_id_vector != NULL)
    {
        rpc_if_id_vector_free (&if_id_vector, &status);
    }

    if (uuid_string != NULL)
    {
        rpc_string_free (&uuid_string, &status);
    }

    if (rpc_status != rpc_s_ok &&
        rpc_status != rpc_s_no_more_members &&
        rpc_status != rpc_s_no_interfaces_exported)
    {
        return (false);
    }
    
    return (true);
}

INTERNAL boolean32 export (
    unsigned_char_p_t   entry_name
)
{
    if (entry_name == NULL) return (false);

    /*
     * if no interface was specified, just export objects
     */
    if (uuid_is_nil (&(interface_id.uuid), &rpc_status))
    {
        /*
         * if there are no objects, report an error
         */
        if (object_vector.count == 0)
        {
            fprintf (stderr, "\n>>> command syntax error: at least one object or interface required\n\n");
        }
        else
        {
            rpc_ns_binding_export (
                name_syntax,
                entry_name,
                NULL,
                NULL,
                (uuid_vector_p_t) &object_vector,
                &rpc_status);

            CHECK_RPC_STATUS();

            printf ("\n>>> objects exported\n\n");
        }
    }
    else
    {
        /*
         * if there are no bindings, report an error
         */
        if (binding_vector.count == 0)
        {
            fprintf (stderr, "\n>>> command syntax error: bindings information required\n\n");
        }
        else
        {
            /*
             * fill in the if spec
             */
            interface_specification.vers =
                (interface_id.vers_minor << 16) | interface_id.vers_major;
            interface_specification.id = interface_id.uuid;
            
            /*
             * see if there are objects to export
             */
            if (object_vector.count == 0)
            {
                rpc_ns_binding_export (
                    name_syntax,
                    entry_name,
                    (rpc_if_handle_t) &interface_specification,
                    (rpc_binding_vector_p_t) &binding_vector,
                    (uuid_vector_p_t) NULL,
                    &rpc_status);

                CHECK_RPC_STATUS();

                printf ("\n>>> binding information exported\n\n");
            }
            else
            {
                /*
                 * export everything
                 */
                rpc_ns_binding_export (
                    name_syntax,
                    entry_name,
                    (rpc_if_handle_t) &interface_specification,
                    (rpc_binding_vector_p_t) &binding_vector,
                    (uuid_vector_p_t) &object_vector,
                    &rpc_status);

                CHECK_RPC_STATUS();

                printf ("\n>>> binding information and objects exported\n\n");
            }
        }
    }            

    return (true);
}

INTERNAL boolean32 unexport (
    unsigned_char_p_t   entry_name
)
{
    if (entry_name == NULL) return (false);

    /*
     * if no interface was specified, just export objects
     */
    if (uuid_is_nil (&(interface_id.uuid), &rpc_status))
    {
        /*
         * if there are no objects, report an error
         */
        if (object_vector.count == 0)
        {
            fprintf (stderr, "\n>>> command syntax error: at least one object or interface required\n\n");
        }
        else
        {
            /*
             * The version argument is meaningless here but still must
             * be specified correctly since it gets checked in the
             * called routine.
             */
            rpc_ns_mgmt_binding_unexport (
                name_syntax,
                entry_name,
                NULL,
                rpc_c_vers_all,
                (uuid_vector_p_t) &object_vector,
                &rpc_status);

            CHECK_RPC_STATUS();

            printf ("\n>>> objects unexported\n\n");
        }
    }
    else
    {
        /*
         * see if there are objects to export
         */
        if (object_vector.count == 0)
        {
            rpc_ns_mgmt_binding_unexport (
                name_syntax,
                entry_name,
                &interface_id,
                version_option,
                (uuid_vector_p_t) NULL,
                &rpc_status);

            CHECK_RPC_STATUS();

            printf ("\n>>> binding information unexported\n\n");
        }
        else
        {
            /*
             * unexport everything
             */
            rpc_ns_mgmt_binding_unexport (
                name_syntax,
                entry_name,
                &interface_id,
                version_option,
                (uuid_vector_p_t) &object_vector,
                &rpc_status);

            CHECK_RPC_STATUS();

            printf ("\n>>> binding information and objects unexported\n\n");
        }
    }            

    return (true);
}

INTERNAL boolean32 import (
    unsigned_char_p_t   entry_name,
    rpc_if_id_p_t       if_id_p,
    boolean32           embedded
)
{
    rpc_ns_import_handle_t  import_context;
    rpc_binding_handle_t    binding;
    unsigned_char_p_t       string_binding;
    unsigned_char_p_t       binding_entry_name;
    boolean32               binding_info_found = false;
    int                     n;
    

    if (entry_name == NULL) return (false);

    /*
     * fill in the if spec
     */
    interface_specification.id = if_id_p->uuid;
    interface_specification.vers =
        (if_id_p->vers_minor << 16) | if_id_p->vers_major;

    /*
     * if no interface was specified, it's an error
     */
    if (uuid_is_nil (&(interface_specification.id), &rpc_status))
    {
        fprintf (stderr, "\n>>> command syntax error: interface required\n\n");
        return (false);
    }

    /*
     * see if an object UUID was specified
     */
    if (object_vector.count == 0)
    {
        rpc_ns_binding_import_begin (
            name_syntax,
            entry_name,
            (rpc_if_handle_t) &interface_specification,
            NULL,
            &import_context,
            &rpc_status);
    }
    else
    {
        /*
         * pass along the specified object UUID
         */
        rpc_ns_binding_import_begin (
            name_syntax,
            entry_name,
            (rpc_if_handle_t) &interface_specification,
            &(objects[0]),
            &import_context,
            &rpc_status);
    }

    CHECK_RPC_STATUS();

    /*
     * Check to determine if  the caller wants to update the local
     * name-service data. If so, set the handle expiration age to
     * bypass the local cache.
     */
    if (update_data)
    {
        rpc_ns_mgmt_handle_set_exp_age (import_context, 0, &rpc_status);
        CHECK_RPC_STATUS();
    }

    /*
     * import until there are no more bindings available
     * (or the number specified is reached)
     */
    for (n = 0; rpc_status != rpc_s_no_more_bindings && n < number_count; n++)
    {
        rpc_ns_binding_import_next (
            import_context,
            &binding,
            &rpc_status);
            
        if (rpc_status != rpc_s_no_more_bindings)
        {
            CHECK_RPC_STATUS();
        
            if (!embedded && !binding_info_found)
            {
                printf ("\nbinding information:\n\n");
            }

            /*
             * Remove the object UUID, if any, that's automatically
             * included by import. We only want to display the
             * string equivalent of a tower - and it doesn't contain
             * object UUIDs.
             */
            rpc_binding_set_object (binding, NULL, &rpc_status);

            CHECK_RPC_STATUS();

            binding_info_found = true;
            
            /*
             * convert the binding to a string binding
             */
            rpc_binding_to_string_binding
                (binding, &string_binding, &rpc_status);
            
            CHECK_RPC_STATUS();
            
            /*
             * report the result
             */
            printf ("  <string binding> %s\n", (char *) string_binding);

            /*
             * see if they want the entry it was found in
             */
            if (binding_entry)
            {
                rpc_ns_binding_inq_entry_name (
                    binding,
                    name_syntax,
                    &binding_entry_name,
                    &rpc_status);
                    
                CHECK_RPC_STATUS();
                
                printf ("  <entry_name>     %s\n", (char *) binding_entry_name);

                /*
                 * free the binding entry name memory held by the runtime
                 */
                rpc_string_free (
                    &binding_entry_name,
                    &rpc_status);
                    
                CHECK_RPC_STATUS();
            }
                                
            if (!embedded)
            {
                printf ("\n");
            }

            /*
             * free the string binding memory held by the runtime
             */
            rpc_string_free (&string_binding, &rpc_status);            
            
            CHECK_RPC_STATUS();

            /*
             * free the binding memory held by the runtime
             */
            rpc_binding_free (&binding, &rpc_status);
                
            CHECK_RPC_STATUS();
        }
    }

    rpc_ns_binding_import_done (&import_context, &rpc_status);
        
    CHECK_RPC_STATUS();

    if (!binding_info_found)
    {
        printf ("\n>>> no matching binding information found\n\n");
    }

    return (true);
}

INTERNAL boolean32 show_mapping (
    unsigned_char_p_t   server_name
)
{
    rpc_binding_handle_t    ep_binding;
    rpc_binding_handle_t    binding;
    unsigned32              inq_type;
    rpc_ep_inq_handle_t     inq_context;
    uuid_t                  object_uuid;
    rpc_if_id_t             if_id;
    unsigned_char_p_t       elt_annotation;
    unsigned_char_p_t       uuid_string;
    unsigned_char_p_t       string_binding;
    boolean32               elements_found = false;


    if (server_name == NULL)
    {
        ep_binding = NULL;
    }
    else
    {
        rpc_binding_from_string_binding (
            (unsigned_char_p_t) server_name,
            &ep_binding,
            &rpc_status);
    
        CHECK_RPC_STATUS();
    }

    if (uuid_is_nil (&(interface_id.uuid), &rpc_status))
    {
        inq_type = (object_vector.count == 0) ? rpc_c_ep_all_elts : 
                                                rpc_c_ep_match_by_obj;
    }
    else
    {
        inq_type = (object_vector.count == 0) ? rpc_c_ep_match_by_if : 
                                                rpc_c_ep_match_by_both;
    }

    rpc_mgmt_ep_elt_inq_begin (
        ep_binding,
        inq_type,
        &interface_id,
        version_option,
        object_vector.uuid[0],
        &inq_context,
        &rpc_status);

    CHECK_RPC_STATUS();

    while (true)
    {
        rpc_mgmt_ep_elt_inq_next (
            inq_context,
            &if_id,
            &binding,
            &object_uuid,
            &elt_annotation,
            &rpc_status);

        if (rpc_status == rpc_s_no_more_elements)
        {
            break;
        }

        if (!elements_found)
        {
            printf ("\nmappings:\n");
            elements_found = true;
        }

        CHECK_RPC_STATUS();

        if (uuid_is_nil (&object_uuid, &rpc_status))
        {
            CHECK_RPC_STATUS();

            printf ("\n  <object>         nil\n");
        }
        else
        {
            CHECK_RPC_STATUS();

            uuid_to_string (&object_uuid, &uuid_string, &rpc_status);

            CHECK_RPC_STATUS();

            printf ("\n  <object>         %s\n", uuid_string);

            rpc_string_free (&uuid_string, &rpc_status);

            CHECK_RPC_STATUS();
        }

        uuid_to_string (&(if_id.uuid), &uuid_string, &rpc_status);

        CHECK_RPC_STATUS();
        
        printf ("  <interface id>   %s,%d.%d\n",
            uuid_string,
            if_id.vers_major,
            if_id.vers_minor);

        rpc_string_free (&uuid_string, &rpc_status);

        CHECK_RPC_STATUS();

        rpc_binding_to_string_binding (binding, &string_binding, &rpc_status);

        CHECK_RPC_STATUS();

        printf ("  <string binding> %s\n", string_binding);

        rpc_string_free (&string_binding, &rpc_status);

        CHECK_RPC_STATUS();

        rpc_binding_free (&binding, &rpc_status);

        CHECK_RPC_STATUS();

        printf ("  <annotation>     %s\n", elt_annotation);

        rpc_string_free (&elt_annotation, &rpc_status);

        CHECK_RPC_STATUS();
    }

    rpc_mgmt_ep_elt_inq_done (&inq_context, &rpc_status);

    CHECK_RPC_STATUS();

    if (ep_binding != NULL)
    {
        rpc_binding_free (&ep_binding, &rpc_status);

        CHECK_RPC_STATUS();
    }

    if (elements_found)
    {
        printf ("\n");
    }
    else
    {
        printf ("\n>>> no matching mappings found\n\n");
    }

    return (true);
}


INTERNAL boolean32 remove_mapping (
    unsigned_char_p_t   server_name
)
{
    rpc_binding_handle_t    ep_binding;
    unsigned16              i, j;
    boolean32               some_success = false, some_failure = false;
    unsigned32              temp_status;


    if (uuid_is_nil (&interface_id.uuid, &rpc_status)) 
    {
        fprintf (stderr, "\n>>> command syntax error: interface required\n\n");
        return (false);
    }

    if (binding_vector.count == 0)
    {
        fprintf (stderr, "\n>>> command syntax error: one or more bindings required\n\n");
        return (false);
    }

    if (server_name == NULL)
    {
        ep_binding = NULL;
    }
    else
    {
        rpc_binding_from_string_binding (
            (unsigned_char_p_t) server_name,
            &ep_binding,
            &rpc_status);
    
        CHECK_RPC_STATUS();
    }

    for (i = 0; i < binding_vector.count; i++)
    {
        if (object_vector.count == 0)
        {
            rpc_mgmt_ep_unregister (
                ep_binding,
                &interface_id,
                binding_vector.binding_h[i],
                NULL,
                &rpc_status);

            if (rpc_status == ept_s_not_registered)
            {
                char *string_binding;

                rpc_binding_to_string_binding (
                    binding_vector.binding_h[i], 
                    (unsigned_char_p_t *) &string_binding, 
                    &rpc_status);

                printf("\n>>> no matching mappings found for\n    binding %s\n",
                    string_binding);

                rpc_string_free
                    ((unsigned_char_p_t *) &string_binding, &rpc_status);

                some_failure = true;
            }
            else if (rpc_status == rpc_s_ok)
            {
                some_success = true;
            }
            else
            {
                if (ep_binding != NULL)
                {
                    rpc_binding_free (&ep_binding, &temp_status);
                }

                REPORT_DCE_ERROR();
                return (false);
            }
        }
        else
        {
            for (j = 0; j < object_vector.count; j++)
            {
                rpc_mgmt_ep_unregister (
                    ep_binding,
                    &interface_id,
                    binding_vector.binding_h[i],
                    object_vector.uuid[j],
                    &rpc_status);

                if (rpc_status == ept_s_not_registered)
                {
                    char *string_binding, *string_uuid;

                    rpc_binding_to_string_binding (
                        binding_vector.binding_h[i], 
                        (unsigned_char_p_t *) &string_binding, 
                        &rpc_status);

                    uuid_to_string (
                        object_vector.uuid[j], 
                        (unsigned_char_p_t *) &string_uuid, 
                        &rpc_status);

                    printf ("\n>>> no matching mappings found for\n    binding %s, object %s\n",
                        string_binding, string_uuid);

                    rpc_string_free
                        ((unsigned_char_p_t *) &string_binding, &rpc_status);
                    rpc_string_free
                        ((unsigned_char_p_t *) &string_uuid, &rpc_status);

                    some_failure = true;
                }
                else if (rpc_status == rpc_s_ok)
                {
                    some_success = true;
                }    		
                else
                {
                    if (ep_binding != NULL)
                    {
                        rpc_binding_free (&ep_binding, &temp_status);
                    }

                    REPORT_DCE_ERROR();
                    return (false);
                }
            }
        }
    }

    if (ep_binding != NULL)
    {
        rpc_binding_free (&ep_binding, &rpc_status);

        CHECK_RPC_STATUS();
    }

    if (some_success)
    {
        if (! some_failure)
        {
            printf ("\n>>> all matching mappings removed\n\n");
        }
        else
        {
            printf ("\n>>> some matching mappings removed\n\n");
        }
    }
    else
    {
        printf ("\n>>> no mappings removed\n\n");
    }

    return (true);
}

INTERNAL boolean32 add_mapping (void)
{
    rpc_binding_handle_t    ep_binding;


    if (uuid_is_nil (&interface_id.uuid, &rpc_status)) 
    {
        fprintf (stderr, "\n>>> command syntax error: interface required\n\n");
        return (false);
    }

    /*
     * fill in the if spec
     */
    interface_specification.vers =
        (interface_id.vers_minor << 16) | interface_id.vers_major;
    interface_specification.id = interface_id.uuid;
            
    if (binding_vector.count == 0)
    {
        fprintf (stderr, "\n>>> command syntax error: one or more bindings required\n\n");
        return (false);
    }

    if (no_replace)
    {
        rpc_ep_register_no_replace (
                (rpc_if_handle_t) &interface_specification,
                (rpc_binding_vector_p_t) &binding_vector,
                (uuid_vector_p_t) &object_vector,
                annotation,
                &rpc_status);
    }
    else
    {
        rpc_ep_register (
                (rpc_if_handle_t) &interface_specification,
                (rpc_binding_vector_p_t) &binding_vector,
                (uuid_vector_p_t) &object_vector,
                annotation,
                &rpc_status);
    }

    CHECK_RPC_STATUS();

    return (true);
}

/*
 * expand_name
 * 
 * A jacket around the call to rpc_ns_entry_expand_name(), which returns
 * the name in canonical form.
 */

INTERNAL boolean32 expand_name (
    unsigned_char_p_t   name_in,
    unsigned_char_p_t   *name_out
)
{
    /*
     * Allow for option with no args.
     */
    if (name_in == NULL) return (true);

    /*
     * else, expand the specified name:
     */
    rpc_ns_entry_expand_name (name_syntax, name_in, name_out, &rpc_status);

    CHECK_RPC_STATUS();

    return (true);
}
