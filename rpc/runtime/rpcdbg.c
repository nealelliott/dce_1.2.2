/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpcdbg.c,v $
 * Revision 1.1.941.2  1996/02/18  00:05:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:19  marty]
 *
 * Revision 1.1.941.1  1995/12/08  00:21:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:20  root]
 * 
 * Revision 1.1.939.3  1994/09/15  13:36:22  rsarbo
 * 	pull in rpc__print_source() if building for kernel && debug
 * 	[1994/09/15  13:24:55  rsarbo]
 * 
 * Revision 1.1.939.2  1994/05/19  21:14:59  hopkins
 * 	More serviceability.
 * 	[1994/05/18  21:25:13  hopkins]
 * 
 * 	Merge with DCE1_1.
 * 
 * 	Serviceability work.
 * 	[1994/05/03  20:27:41  hopkins]
 * 
 * Revision 1.1.939.1  1994/01/21  22:38:58  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:35  cbrooks]
 * 
 * Revision 1.1.8.2  1993/08/26  19:20:30  tatsu_s
 * 	Bug 8255 - Disable the current thread's general cancelability
 * 	while calling write().
 * 	Added support for rpc_e_dbg_pid for printing the process id.
 * 	[1993/08/24  17:01:38  tatsu_s]
 * 
 * Revision 1.1.4.3  1993/01/03  23:54:38  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:11:50  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  21:15:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:43:33  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  16:05:14  rsalz
 * 	 09-apr-92 nm    Workaround AIX 3.2 compiler bug
 * 	[1992/05/01  00:18:26  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:11:27  devrcs
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
**      rpcdbg.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Various data and functions for the debug component.
**
**
*/

#include <ctype.h>
#include <commonp.h>
#include <string.h>

/*
 * Debug table
 *
 * A vector of "debug levels", one level per "debug switch".
 */

GLOBAL unsigned8 rpc_g_dbg_switches[RPC_C_DBG_SWITCHES];
      
/*
 * string buffer used by uuid_string()
 */
#ifdef DEBUG
INTERNAL char         uuid_string_buff[40];
#endif

/*
 * Make more allowances for (kernel) portability.
 */
#ifndef RPC_DBG_PRINTF_STDERR
#  define RPC_DBG_PRINTF_STDERR     fprintf(stderr,
#endif

/*
 * R P C _ _ D B G _ S E T _ S W I T C H E S
 *
 * Set debug switches from string.  The format of the string is:
 *
 *      SwitchRange.level,SwitchRange.level,...
 *
 * where a "SwitchRange" is either a single integer (e.g., "5") or a
 * range of integers of the form "integer-integer" (e.g., "1-5").  "level"
 * is the integer debug level to be applied to all the switches in the
 * range.  Putting it all together, an sample string that this function
 * can process is: "1-5.7,6.3,9.5". *
 *
 * This code largely cribbed from sendmail's "tTflag" function, which is...
 * 
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that this notice is preserved and that due credit is given
 * to the University of California at Berkeley. The name of the University
 * may not be used to endorse or promote products derived from this
 * software without specific prior written permission. This software
 * is provided ``as is'' without express or implied warranty.
 *
 *  Sendmail
 *  Copyright (c) 1983  Eric P. Allman
 *  Berkeley, California
 */

PUBLIC void rpc__dbg_set_switches 
#ifdef _DCE_PROTO_
(
    char            *s,
    unsigned32      *status
)
#else
(s, status)
char            *s;
unsigned32      *status;
#endif
{
#ifndef DEBUG

    *status = rpc_s_ok;

#else

    int         first, last;
    register int i;

    *status = rpc_s_ok; 

    for (;;)
    {
        /*
         * find first flag to set
         */
        i = 0;
        while (isdigit (*s))
            i = i * 10 + (*s++ - '0');
        first = i;

        /*
         * find last flag to set
         */
        if (*s == '-')
        {
            i = 0;
            while (isdigit (*++s))
                i = i * 10 + (*s - '0');
        }
        last = i;

        /*
         * find the level to set it to
         */
        i = 1;
        if (*s == '.')
        {
            i = 0;
            while (isdigit (*++s))
                i = i * 10 + (*s - '0');
        }

        /*
         * clean up args
         */
        if (first >= RPC_DBG_N_SWITCHES)
            first = RPC_DBG_N_SWITCHES - 1;
        if (last >= RPC_DBG_N_SWITCHES)
            last = RPC_DBG_N_SWITCHES - 1;

        /*
         * set the flags
         */
        while (first <= last)
            rpc_g_dbg_switches[first++] = i;

        /*
         * more arguments?
         */
        if (*s++ == '\0')
            return;
    }
#endif
}


/* ======================================================================= */

#ifndef DCE_RPC_SVC
#ifndef NO_RPC_PRINTF

/*
 * R P C _ _ P R I N T F
 *
 * Note: This function uses a variable-length argument list. The "right"
 * way to handle this is using the ANSI C notation (listed below under
 * #ifdef STDARG_PRINTF). However, not all of the compilers support this,
 * so it's here just for future reference purposes.
 *
 * An alternative is to use the "varargs" convention (listed below under
 * #ifndef NO_VARARGS_PRINTF). Most compilers support this convention,
 * however you can't use prototypes with this.
 *
 * The last choice is to use the "old" notation. In this case also you
 * can't use prototypes.
 *
 * 12/31/93 CLBrooks - only support the stdargs form 
 */
PRIVATE int rpc__printf (char *format, ...)
{
    char            buff[300];
    char            *s = buff;

    if (RPC_DBG (rpc_e_dbg_pid, 1))
    {
        sprintf (s, "[pid: %06lu] ", getpid());
        s = &buff[strlen(buff)];
    }

    if (RPC_DBG (rpc_e_dbg_timestamp, 1))
    {
        sprintf (s, "[time: %06lu] ", rpc__clock_stamp());
        s = &buff[strlen(buff)];
    }

    if (RPC_DBG (rpc_e_dbg_thread_id, 1))
    {
        pthread_t self;

        self = pthread_self ();
#ifdef CMA_INCLUDE
        sprintf (s, "[thread: %08x.%08x] ", self.field1, self.field2);
#else
        sprintf (s, "[thread: %08x] ", self);
#endif
        s = &buff[strlen (buff)];
    }

    {
	va_list         arg_ptr;
	
	va_start (arg_ptr, format);
	vsprintf (s, format, arg_ptr);
	va_end (arg_ptr);
    }

    {
        int prev;
        prev = pthread_setcancel (CANCEL_OFF);
        write (2, buff, strlen (buff));
        (void) pthread_setcancel (prev);
    }
}

#endif /* NO_RPC_PRINTF */
#endif /* DCE_RPC_SVC */

/*
 * R P C _ _ D I E
 *
 * Try to report what happened and get out.
 *
 */

PRIVATE void rpc__die 
#ifdef _DCE_PROTO_
(
    char            *text,
    char            *file,
    int             line
)
#else
(text, file, line)
char            *text;
char            *file;
int             line;
#endif
{
#ifndef FILE_SEPARATOR_CHAR 
#define FILE_SEPARATOR_CHAR '/'
/*#error  "FILE_SEPARATOR_CHAR not defined!"*/
#endif

#if 0
#ifdef MSDOS
#  define SEPCHAR   '\\'
#else
#ifdef vms
#  define SEPCHAR   ']'
#else
#  define SEPCHAR   '/'
#endif
#endif
#endif /* 0 */

    char        *p = strrchr (file, FILE_SEPARATOR_CHAR);

    EPRINTF("(rpc) *** FATAL ERROR \"%s\" at %s\\%d ***\n",
            text, p == NULL ? file : p + 1, line);
    abort ();
}


/*
 * R P C _ _ U U I D _ S T R I N G
 *
 * Return a pointer to a printed UUID.
 */

PRIVATE char *rpc__uuid_string 
#ifdef _DCE_PROTO_
(
    uuid_t          *uuid
)
#else
(uuid)
uuid_t          *uuid;
#endif
{
#ifndef DEBUG

    return ("");

#else

    unsigned_char_p_t   uuid_string_p;
    unsigned32          status;


    uuid_to_string (uuid, &uuid_string_p, &status);
    if (status != uuid_s_ok)
    {
        return (NULL);
    }

    strncpy (uuid_string_buff, (char *) uuid_string_p, sizeof uuid_string_buff);
    rpc_string_free (&uuid_string_p, &status);

    return (uuid_string_buff);

#endif
}


#if !defined(DCE_RPC_SVC) || (defined(_KERNEL) && defined(DEBUG))
/*
 * R P C _ _ P R I N T _ S O U R C E
 *
 * Auxiliary function to print source file name and line number.  Used by
 * RPC_DBG_PRINT macro.
 */

PRIVATE void rpc__print_source (file, line)

char            *file;
int             line;

{
#ifdef DEBUG
    if (RPC_DBG(rpc_e_dbg_source, 1))
    {
#ifndef _KERNEL
        EPRINTF("    [file: %s, line: %d]\n", file, line);
#else
        printf("    [file: %s, line: %d]\n", file, line);
#endif
    }
#endif
}
#endif /* !DCE_RPC_SVC || (KERNEL && DEBUG) */
