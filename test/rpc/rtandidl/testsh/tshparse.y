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
 * $Log: tshparse.y,v $
 * Revision 1.1.8.2  1996/03/11  02:28:09  marty
 * 	Update OSF copyright years
 * 	[1996/03/10  20:00:03  marty]
 *
 * Revision 1.1.8.1  1995/12/11  20:13:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:31  root]
 * 
 * Revision 1.1.4.8  1993/03/11  16:25:48  cuti
 * 	Change #ifndef SYSV to #ifndef PATH_MAX, so PATH_MAX won't redefine.
 * 	[1993/03/11  15:21:15  cuti]
 * 
 * Revision 1.1.4.7  1993/03/10  22:26:02  cuti
 * 	Redefine PATH_MAX to MAXPATHLEN for non-SYSV.
 * 	[1993/03/10  21:23:27  cuti]
 * 
 * Revision 1.1.4.6  1993/02/26  22:39:10  weisman
 * 	very major cleanup
 * 	[1993/02/26  22:16:12  weisman]
 * 
 * Revision 1.1.4.5  1993/01/13  19:51:24  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:18:52  bbelch]
 * 
 * Revision 1.1.4.4  1992/11/04  14:23:26  cuti
 * 	Change "fopen() < 0" to "fopen() != )"
 * 	[1992/11/03  22:00:18  cuti]
 * 
 * 	5912: Unable to compile under HP-UX ANSI_c compiler. Add #ifdef clause.
 * 	[1992/11/03  20:15:47  cuti]
 * 
 * Revision 1.1.4.2  1992/09/29  21:24:53  devsrc
 * 	[OT 5373]  SNI/SVR4 Merge UPDATE: Use execve() instead of execvp() for all
 * 	           systems not just SVR4
 * 	[1992/09/24  14:25:07  sekhar]
 * 
 * 	[OT 5373]   SNI/SVR4 Merge.
 * 	            Also added OSF copyright marker and history log within
 * 	[1992/09/15  15:26:51  sekhar]
 * 
 * $EndLog$
 */

/*
 * tshparse.y - grammer for tsh
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
 * @(#)tshparse.y	1.14
 *  
 *	Extracted	1/22/91 11:04:36 
 * 	from delta on	1/17/91 16:03:45
 *
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
#ifndef LINT
static char sccs_id[] = "@(#)tshparse.y	1.14";
#endif

#include "tsh.h"
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>

#if !defined (YYDEBUG)
#  define YYDEBUG 1
#endif

    int failcnt = 0;
    int subfailcnt = 0;
    int passcnt = 0;
    int subpasscnt = 0;
    extern searchlist_t	searcher;
    void yyerror (char * text_p);

    /* 
     * Following defines the exec command argument list.
     */

#ifndef PATH_MAX
#define PATH_MAX MAXPATHLEN
#endif
#define ARGLIST_ELEMENTS 200
    char * (arglist [ARGLIST_ELEMENTS]);
    int arglist_count;

%}

/* 
 * Tokens.
 */

%token CLEAR
%token CONTINUE
%token CR
%token DEFAULT
%token DOLLAR_SIGN
%token ECHO_STR
%token EQUAL
%token EXECUTE
%token INCLUDE
%token ONERROR
%token PAUSE
%token READ
%token REMOTE
%token RUN
%token SETCMD
%token STOP
%token STRING
%token SUBTOTAL
%token SUMMARY
%token TEST
%token UNSET

/* 
 * yyval types
 */

%union
{
    char * string_token_p;              /* pointer to a malloced string */
    unsigned long integer_token;
}

/* 
 * token and nonterminal types
 */

%type <string_token_p> STRING text 
%type <integer_token> onerror_option 

%%

program         : program_items
                | null
                ;

program_items   : command
                | program_items command
                ;

command         : individual_command
                | error CR        /* Error recovery point */
                  {
                      yyerrok;
                  }
                ;

individual_command: 
                  CR                    /* null command */
                |
                  ECHO_STR echo_handler CR
                | 
                  ONERROR onerror_option CR
                  {
                      errorflag = $2;
                  }
                | 
                  PAUSE pause_handler CR
                | 
                  READ read_handler CR
                | 
                  REMOTE remote_handler CR
                | 
                  RUN run_handler CR
                | 
                  SETCMD set_handler CR
                | 
                  SUBTOTAL subtotal_handler CR
                | 
                  SUMMARY summary_handler CR
                | 
                  TEST test_handler CR
                | 
                  UNSET unset_handler CR
                ;

echo_handler    : init_arglist
                  arglist
                  {     
                      int i;

                      for (i = 0; i < arglist_count; i++)
                      {     
                          vlog ("%s ", arglist [i]);
                      }

                      vlog ("%s\n", "");
                      dealloc_arglist_members();
                  }
                ;


text            :               /* caller must free() */
                  STRING
                  {
                      $$ = $1;
                  }
                |
                  DOLLAR_SIGN
                  STRING
                  {
                      char * env_str_p;
                      
                      env_str_p = getenv ($2);
                      
                      if (env_str_p == NULL)
                      {
                          char buff [200];

                          strcpy (buff, "Environment variable not set: $"); 
                          strncat  (buff, $2, sizeof (buff));
                          buff [sizeof buff - 1] = '\0'; 
                          free ($2);
                          yyerror (buff);
                          YYERROR;
                      }
                      else
                      {
                          $$ = strdup (env_str_p);
                      }
                      free ($2); /* free the name of the env var */
                  }                      

init_arglist    :
                  {
                      init_arglist ();
                  }
                ;

arglist         :               /* caller must dealloc_arglist_members() */
                  null
                |
                  arglist_entries
                ;

arglist_entries :
                  text
                  {
                      append_arglist ($1);
                  }
                |
                  arglist_entries text
                  {
                      append_arglist ($2);
                  }
                ;


onerror_option  : 
		  STOP
                  {
			if (verbose)
				vlog ("onerror stop\n");
			$$ = 1;
                  }
                | CONTINUE
                  {
			if (verbose)
				vlog ("onerror continue\n");
			$$ = 0;
		  }
          	| DEFAULT
		  {
			if (verbose)
				vlog ("onerror default\n");
			$$ = errdefault;
		  }
                ;

test_handler    : init_arglist 
                  text          /* name of command */
                  {
                      append_arglist ($2); /* command name */
                          
                      if (scroutput)
                          append_arglist (strdup ("-s"));
                          
                      if (outputmask != MASKDEFAULT)
                      {
                          char buf [50];
                              
                          append_arglist (strdup ("-d"));

                          sprintf (buf, "%d", outputmask);
                          append_arglist (strdup (buf));
                      }
                  }

                  arglist
                  {
                      if (runprog())
                      {
                          failcnt ++;
                          subfailcnt ++;

                          if (errorflag) 
                          {
                              subtotal (0);
                              summary ();
                              exit (1);
                          }
                      }
                      else      /* runprog passed */
                      {
                          passcnt++;
                          subpasscnt++;
                      }
                      dealloc_arglist_members();
                  }
              ;

run_handler     : init_arglist
                  arglist
                  {
                      if (verbose)
                          vlog ("run");

                      runprog ();
                      dealloc_arglist_members();
                  }


remote_handler  : init_arglist
                  text          /* name of command */
                  {
                      append_arglist (strdup ("rsh"));
                      append_arglist ($2); /* command name */
                  }                  
                  arglist
                  {
                      if (verbose)
                          vlog ("remote");

                      runprog ();
                      dealloc_arglist_members ();
                  }

set_handler     : STRING
                  EQUAL
                  text
                  {

                      int string_len;
                      char * text;

                      string_len = 
                          strlen ($1) + /* lhs */
                          1  +          /* EQUAL */
                          strlen ($3) + /* rhs */
                          1;            /* null byte */

                      text = (char *) malloc (string_len);

                      sprintf (text, "%s=%s", $1, $3);

                      free ($1);
                      free ($3);

                      if (verbose)
                          vlog ("set %s\n", text);

                      putenv (text);

                      /* 
                       * Porting note:
                       * 
                       * Some versions of putenv require the 
                       * argument storage remain through the life 
                       * of the process.  To be generic, we are 
                       * not going to free it and suffer the 
                       * small memory leak.  If your platform 
                       * can free it, do so here.
                       * 
                       * We also can't free the object at UNSET 
                       * time; we don't know if the particular 
                       * environment variable was set through 
                       * some mechanism other than SET (and 
                       * therefore not malloced.)  
                       * 
                       * A better implementation would be to keep 
                       * our own local database of the SET 
                       * environment variables and free the 
                       * storage when appropriate.  
                       */
                      
                      /*** free (text);  ***/

                  }
                ;

unset_handler   : STRING
                  {

                      if (verbose)
                          vlog ("unset %s\n", $1);

                      delete_env_variable ($1);
                      free ($1); /* free STRING */
                  }
                ;


subtotal_handler: CLEAR
                {       

                    if (verbose)
			vlog ("subtotal clear\n");

                    subtotal (1);
                }

                |

                  null
                {
                    if (verbose)
			vlog ("subtotal\n");

                    subtotal (0);
                }

summary_handler : null
                {
                    if (verbose)
			vlog ("summary\n");

                    summary();
                }                    


pause_handler   :
                {
                    /* 
                     * Semi-lifted from original tshparse.y
                     */

                    FILE *f;
                    char c;

                    if (verbose)
			vlog ("pause\n");

                    if ((f = fopen ("/dev/tty", "r+")) == NULL)
                    {
			log ("testsh: unable to open /dev/tty  %s\n",
                             strerror (errno));

			exit (1);
                    }

                    fprintf (f, "Press RETURN to continue Q to quit\n");
                    fflush (f);

                    for (;;) 
                    {
                        c = fgetc (f);

			if (c == 'q' | c == 'Q') 
                        {
                            log ("testsh: Pause recieved user abort\n");
                            exit (0);
			}

			if (c == '\n')
                            break;
                    }

                    fclose (f);
                }
                ;

read_handler    : STRING STRING

                {
                    /* 
                     * Taken from original tshparse.y
                     */

                    FILE * f;
                    char input_buf [YYLMAX]; 
                    char putenv_text [YYLMAX];

                    if ((f = fopen ("/dev/tty", "r+")) == NULL)
                    {
			log ("testsh: unable to open /dev/tty  %s\n",
                             strerror (errno));

			exit (1);
                    }

                    /* 
                     * Put out prompt.
                     */
                    
                    fprintf (f, "%s\n", $1);
                    fflush (f);

                    /* 
                     * Get response and build up string for putenv.
                     */

                    fscanf (f, "%s", input_buf);
                    sprintf (putenv_text, "%s=%s", $2,input_buf);

                    putenv (putenv_text);

                    fclose (f);
                }
                ;

null            :               /*  */
                ;

    
%%

void init_arglist (void)
{
    arglist_count = 0;
}                               /* init_arglist */

int append_arglist (char * s)
{

    /* 
     * append_arglist places a new arglist entry onto the end of 
     * the arglist vector.  It also places a null entry one 
     * beyond as required by execvp.
     */

    if ((arglist_count + 1) == ARGLIST_ELEMENTS)
    {
        yyerror ("Argument list buffer exhausted."); 
        exit (1);
    }
    else 
    {
        arglist [arglist_count++] = s;
        arglist [arglist_count] = NULL;
    }
}                               /* append_arglist */

void dealloc_arglist_members (void)
{
    int i;
    
    for (i = 0; i < arglist_count; i++)
    {
        free (arglist [i]);
    }
}                               /* dealloc_arglist_members */

void subtotal (int clrflag)
{
    log ("Subtotal: %d passed, %d failed\n", subpasscnt, subfailcnt);

    if(clrflag)
        subpasscnt = subfailcnt = 0;

}                                       /* subtotal */

void summary(void)
{
    log("Summary: %d passed, %d failed\n",passcnt,failcnt);
}                               /* summary */



int runprog (void)
{
    int	pid;
    int	stat_loc = -1;                  /* assume failure */
    searchlist_p_t	slist;

    if (verbose) 
    {
        int i;

        printf ("test ");

        /* 
         * Print out the args.  If an arg has embedded 
         * spaces, put it in quotes.
         */

        for (i = 0; i < arglist_count; i++)
        {
            if (strchr (arglist[i], ' ') == NULL)
                printf ("%s ", arglist [i]);
            else
                printf ("\"%s\" ", arglist [i]);
        }
        printf ("\n");
    }

    fflush(stdout);

    switch (pid = fork ())  
    {

    case -1:
        log ("fork failed %d\n", errno);
        break;

    case 0:
        /* 
         * Child process.
         */

        execvp (arglist[0], &(arglist[0]));

        /* 
         * Normal PATH search failed.  If we're using the 
         * user-supplied search list, then try that now.
         */

        slist = &searcher;

        while (slist = slist->searchnext) 
        {
            int	t;

            char tmpbuf [PATH_MAX];

            /* 
             * Copy in directory from search list.
             */
 
            strcpy (tmpbuf, slist -> searchdir);

            /* 
             * Append the user's program name.
             */

            if (tmpbuf [t = strlen (slist -> searchdir)] 
                != '/') 
            {
                tmpbuf [t++] = '/';
                tmpbuf [t] = '\0';
            }

            strcat (tmpbuf, arglist[0]);

            /* 
             * Try it.
             */

            execvp (tmpbuf, &arglist [0]);
        }                               /* while slist */

        log ("testsh: Unable to exec %s (%s)\n", 
             arglist [0],
             strerror (errno));
                
        /* 
         * Call _exit instead of exit.  We need to ensure 
         * that this child process does not change any 
         * stdio of the parent testsh.
         */

        _exit (1);

        break;

    default:                            /* parent process */
        waitpid (pid,
                 & stat_loc,
                 0);                    /* options = 0 */

        break;
    }                                   /* switch */

    return (stat_loc);
}
