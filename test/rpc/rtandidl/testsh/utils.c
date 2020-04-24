/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */

/*
 * HISTORY 
 * $Log: utils.c,v $
 * Revision 1.1.4.2  1996/02/17  23:23:04  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:42  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:13:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:33  root]
 * 
 * Revision 1.1.2.4  1993/03/11  16:26:00  cuti
 * 	Change #ifndef SYSV to #ifndef PATH_MAX, so PATH_MAX won't redefine.
 * 	[1993/03/11  15:21:38  cuti]
 * 
 * Revision 1.1.2.3  1993/03/10  22:26:21  cuti
 * 	Add #ifndef, #ifdef to define TREU and FALSE, and redefine PATH_MAX to MAXPATHLEN for non-SYSV.
 * 	[1993/03/10  21:23:52  cuti]
 * 
 * Revision 1.1.2.2  1993/02/26  22:39:17  weisman
 * 	very major cleanup
 * 	[1993/02/26  22:22:34  weisman]
 * 
 * $EndLog$
 */

#include "tsh.h"
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#ifndef PATH_MAX
#define PATH_MAX  MAXPATHLEN
#endif

#ifndef TRUE
#define TRUE 1
#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0
#endif

extern int yydebug;
extern int lex_debug_helper;
extern unsigned char yytext [];
extern int yylineno;
extern FILE  * yyin;

static char current_pathname [PATH_MAX];

void delete_env_variable (char * name_p)
{
    /* 
     * PORTING NOTE: 
     * 
     * Not all implementations have an unsetenv() function, so
     * here is (hopefully) generic logic.  If you have an
     * unsetenv(), you should use it instead of the following
     * logic.
     *
     * We loop over the environment strings and try to match the 
     * name.  When found, move the subsequent env vars up. 
     * 
     * NB: This routine does not free the old environment string.
     * We cannot assume that this particular environment variable
     * was malloced.
     */
    
    extern char ** environ;
    int	chars_in_name;
    char **	envp;
                      
    if (verbose)
    vlog ("unset %s\n", name_p);
                      
    chars_in_name = strlen (name_p);

    for (envp = environ; *envp; envp++)
    {
        if ((strncmp (name_p, *envp, chars_in_name) == 0)  && 
            (* (*envp + chars_in_name) == '=' ))     
        break;                  /* found it */     
    }

    if (*envp)                  /* env found */
    {
        /* 
         * Free the environment string.  See portability note above in 
         * set_handler.  
         */
        
        free (* envp);

        while (*envp) 
        {
            *envp = * (envp + 1);
            envp ++;
        }
    }
}                               /* delete_env_variable */


void yyerror (char * text_p)
{

    fprintf (stderr, "\"%s\", around line %d: %s\n",
             TshFileName, yylineno, text_p);

}


void _doinclude (char * pathname, int mode)
{
    FILE	*fd;

    if (verbose)
        vlog ("including: %s\n", pathname);

    push_include_stack (mode);

    /* 
     * Remember the current pathname
     */

    strcpy (current_pathname, pathname);
    TshFileName = current_pathname;

    if (fd = fopen (TshFileName, "r"))
    {
        yyin = fd;
        yylineno = 1;

    }
}                               /* _doinclude */

void doinclude (char * pathname, int mode)
{
    int	error = FALSE;		/* assume success */
    searchlist_p_t	slist;
    extern searchlist_t	searcher;       

    /* 
     * See if file is there.
     */

    if (!access (pathname, R_OK)) 
    {
        _doinclude (pathname, mode);
    }

    /* 
     * Otherwise, try user-supplied searchlist.
     */

    else 
    {
        error = TRUE;
        slist = &searcher;

        while (slist = slist->searchnext) 
        {
            int	t;
            char tmpbuf [PATH_MAX];

            strcpy (tmpbuf, slist->searchdir);

            if (tmpbuf [t = strlen (slist->searchdir)] != '/') 
            {
                tmpbuf [t++] = '/';
                tmpbuf [t] = '\0';
            }

            strcat (tmpbuf, pathname);
            if (!access (tmpbuf, R_OK))  
            {
                error = FALSE;
                _doinclude (tmpbuf, mode);
            }
        }                       /* while slist */
    }                           /* try search paths */

    if (error)
        log ("testsh: %s: %s\n", 
             pathname, strerror (errno));

}                               /* doinclude */


/* 
 * Following defines the stack for nested include files
 */

typedef struct struct_include_stack 
{
    struct struct_include_stack * next_p;

    char                saved_pathname [PATH_MAX];
    FILE                *yyin;
    int                 yylineno;
    int                 mode;
    int                 errorflag;
 
} INCLUDE_STACK, * INCLUDE_STACK_P;

INCLUDE_STACK_P include_stack_head_p;

void initialize_include_stack (void)
{
    include_stack_head_p = NULL;
}                               /* initialize_include_stack */


void push_include_stack (int mode)
{
    INCLUDE_STACK_P save_state_p;

    save_state_p = malloc (sizeof (INCLUDE_STACK));
        
    if (save_state_p == NULL)
    {
        fprintf (stderr, "Could not malloc include stack frame.\n");
        exit (2);
    }

    /* 
     * Fill in the new stack frame and thread onto head of list.
     */

    save_state_p -> next_p = include_stack_head_p;

    strcpy (save_state_p -> saved_pathname, TshFileName);

    save_state_p -> yyin = yyin;
    save_state_p -> yylineno = yylineno;
    save_state_p -> mode = mode;

    /* 
     * If we're in EXECUTE_MODE, then save errorflag on the state 
     * stack. 
     */

    if (mode == EXECUTE_MODE)
    {
        save_state_p -> errorflag = errorflag;
    }

    include_stack_head_p = save_state_p;

}                               /* push_include_stack */

int pop_include_stack (void)
{

    /* 
     * Pop the top frame off of the include stack.  Return 1 
     * to the caller if we are at the end of the stack, else 
     * return 0.
     */  

    INCLUDE_STACK_P save_state_p;

    if (include_stack_head_p == NULL)
        return 1;               /* end of include stack */

    /* 
     * Restore old values to caller.
     */

    save_state_p = include_stack_head_p;

    yyin = save_state_p -> yyin;
    yylineno = save_state_p->yylineno;

    strcpy (current_pathname, save_state_p -> saved_pathname);
    TshFileName = current_pathname;

    /* 
     * If this stack frame was EXECUTE_MODE, then restore errorflag
     */

    if (save_state_p -> mode == EXECUTE_MODE)
        errorflag = save_state_p -> errorflag;

    if (yydebug)
    {
        printf ("pop_include_stack %s\n", current_pathname);
    }

    /* 
     * Unthread this frame and deallocate.
     */

    include_stack_head_p = save_state_p -> next_p;
    free (save_state_p);

    /* 
     * Return 0, indicating remaining stack entries.
     */

    return 0;
}                               /* pop_include_stack */

/* 
 * Define replacements for default lex input and unput.  These 
 * versions manage include files.
 */

char input_buffer [YYLMAX];
char * input_pointer = input_buffer;
char unput_buffer [YYLMAX];
char * unput_pointer = unput_buffer; /* initialize to empty */

unsigned char input (void)
{

#define INCLUDE_STRING "include"
#define INCLUDE_STRING_LEN 7

#define EXECUTE_STRING "execute"
#define EXECUTE_STRING_LEN 7

    static int empty_input_buffer = TRUE;
    char return_char;

    /* 
     * If there is anything in the unput buffer, return it.
     */

    if (unput_pointer != unput_buffer)
        return (* (-- unput_pointer));

    /* 
     * Loop until we have something available for the caller. 
     */

    while (empty_input_buffer)
    {
        if (fgets (input_buffer, YYLMAX, yyin) != NULL)
        {
            yylineno ++;

            /* 
             * Throw away comment lines here.
             */

            if (input_buffer [0] == '#')
            {
                if (lex_debug_helper)
                {
                    printf ("comment (%d): \"%s\"\n", yylineno, input_buffer); 
                }

            }                   /* comment */

            /* 
             * Check for an include directive.  If so, 
             * extract filename and push current state.
             */
                
            else if (strncmp (INCLUDE_STRING, input_buffer, 
                              INCLUDE_STRING_LEN) == 0)
            {
                char * filename;
                    
                filename = strtok (input_buffer + INCLUDE_STRING_LEN, 
                                   " \t\n");

                if (filename == NULL)
                {
                    yyerror ("No include file name given");
                    continue;   /* skip to next line */

                }               /* strtok failed */
                    
		if(verbose)
                    vlog("include %s\n", filename);

                doinclude (filename, INCLUDE_MODE);

            }                   /* it's an include */


            /* 
             * Check for an execute directive.  If so, 
             * extract pathname and push current state.
             */
                
            else if (strncmp (EXECUTE_STRING, input_buffer, 
                              EXECUTE_STRING_LEN) == 0)
            {
                char * filename;
                    
                filename = strtok (input_buffer + EXECUTE_STRING_LEN, 
                                   " \t\n");

                if (filename == NULL)
                {
                    yyerror ("No execute file name given");
                    continue;   /* skip to next line */

                }               /* strtok failed */
                    
		if(verbose)
                    vlog("execute %s\n", filename);

                doinclude (filename, EXECUTE_MODE);

            }                   /* it's an execute */

            /* 
             * Otherwise it's a normal text character.  Prepare 
             * to return to caller.
             */

            else
            {
                input_pointer = input_buffer; /* reset to beginning */
                empty_input_buffer = FALSE; 
            }                   /* not an include */
        }                       /* not eof */

        else                    /* eof from fgets */
        {

            /* 
             * Close the exhausted input file and pop the include 
             * stack.
             */

            fclose (yyin);
            if (pop_include_stack () == 1)
            {
                /* 
                 * Hit EOF on top-level file.  Return EOF 
                 * to caller.
                 */

                return 0;       /* as in lex specification */
            }
        }                       /* eof from fgets */
    }                           /* empty_input_buffer */
    
    /* 
     * We have data for caller.
     */

    return_char = *input_pointer ++;

    if ((return_char == '\n') || (return_char == '\0'))
        empty_input_buffer = TRUE;

    return return_char;
}                               /* input */

void unput (char input_char)
{
    /* 
     * Check for unput buffer overflow.
     */

    if (unput_pointer - sizeof (unput_buffer) == unput_buffer)
    {
        yyerror ("lex unput buffer exhausted.");
        exit (1);
    }

    *unput_pointer++ = input_char;
}                               /* unput */

