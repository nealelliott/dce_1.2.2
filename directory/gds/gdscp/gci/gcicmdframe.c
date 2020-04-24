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
 * $Log: gcicmdframe.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:23  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:44  root]
 * 
 * Revision 1.1.2.5  1994/09/19  08:39:23  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:32:14  marrek]
 * 
 * Revision 1.1.2.4  1994/08/19  10:21:11  keutel
 * 	OT 11475: tcl incore message tables
 * 	[1994/08/18  15:04:23  keutel]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:21  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:11  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:43  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:56:39  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:04  keutel
 * 	creation
 * 	[1994/03/21  12:54:44  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gcicmdframe.c                                     */
/* Description : This file contains the `main()' function of gdscp.*/
/*               It also contains all the necessary procedures     */
/*		 for the interactive and non-interactive mode.     */
/* Date        : July 26, 1993                                     */
/* Author      : A. Doerr, SNI BU BA KP13                          */
/*               M. Kronenburg, SNI BU BA KP13                     */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gcicmdframe.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:31 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>                                                     
#include <sys/stat.h>                                                      
#include <fcntl.h>
#include <locale.h>

#include <gds.h>

#include <dce/dcesvcmsg.h>
#include <dce/tcl.h>
#include <dce/dcetclmsg.h>

#include <gdscp.h>
#include <gctconv.h>
#include <gctstruct.h>

#include <gcistruct.h>
#include <gciutil.h>
#include <gciprint.h>
#include <gcivar.h>
#include <gcimisc.h>
#include <gcix500obj.h>
#include <gcix500abbr.h>
#include <gcix500svc.h>

/*-- Local Defines -------------------------------------------*/
#define GC_I_MAX_PATH  2048       /* maximum string length in path variable   */
#define GC_I_PATH_DIV  ':'        /* separating character for multiple paths  */
#define GC_I_TCL_SUF   ".tcl"     /* suffix for tcl-scripts 		      */
#define GC_I_RC_FILE   ".gdscprc" /* Initialization file to be read 	      */

/* define for the TCL source command to source the TCL init file */
#define GC_I_TCL_SOURCE_COMMAND    "source [info library]/init.tcl"

/* define for the relative installation path name of gdscp */
#define GC_I_GDSCP_REL_INST_PATH   "gdscp"

/* define for the init script file name of gdscp */
#define GC_I_GDSCP_INIT_FILE   	   "init.gdscp"

/* define for the TCL environment variable to specify gdscp installation path */
#define GC_I_GDSCP_LIB_VARIABLE	   "gdscp_library"

/* define for the user specified environment variable */
#define GC_I_USER_DIR_ENV      	   "TCL_PATH"

/* define to specify command line switch */
#define GC_I_COMMAND_SWITCH  	   "-c"

/* constants for prompt strings */
#define GC_I_PRIMARY_PROMPT    	   "gdscp> "
#define GC_I_SECONDARY_PROMPT      ">> "

/* possible exit values of gdscp */
#define GC_I_EXIT_SUCCESS        0 /* no error 				      */
#define GC_I_INCORRECT_CMD_LINE  1 /* incorrect commandline 		      */
#define GC_I_FILE_DOES_NOT_EXIST 2 /* file given in commandline does not exist*/
#define GC_I_TCL_EVALFILE_FAILED 3 /* Executing Tcl_EvalFile failed 	      */
#define GC_I_TCL_EVAL_FAILED     4 /* Executing Tcl_VarEval failed 	      */

/*-- Global variables ----------------------------------------*/
boolean gc_i_interactive_mode;

/*- External functions ---------------------------------------*/
extern  char *readline(char *);
extern  void  add_history(char *);

/*-- Private Prototypes --------------------------------------*/
int main( 
    int     argc,  /* IN - number of arguments */
    char  **argv); /* IN - arguments           */

static void gc_i__read_and_eval_command( 
    Tcl_Interp  *interp); /* IN - TCL interpreter which processes the command */

static void gc_i__register_commands(
    Tcl_Interp  *interp); /* IN - Interpreter where commands are registered */

static void gc_i__evaluate_init_files(
    Tcl_Interp  *interp); /* IN - The TCL interpreter */

static void gc_i__check_interactive_mode( 
    Tcl_Interp  *interp, /* IN - The TCL interpreter */
    signed16 	 argc,   /* IN - Number of arguments */
    char       **argv);  /* IN - Arguments	     */

static void gc_i__split_paths( 
    char       *path_string, /* IN  - user provided string                   */
    char        div_char ,   /* IN  - character dividing several directories */
    char     ***path_array,  /* OUT - the several parts of the paths         */
    signed16   *dir_count);  /* OUT - number of directories in input string  */

static void gc_i__source_user_scripts( 
    Tcl_Interp  *interp,     /* IN  - The TCL interpreter       */ 
    DIR         *source_dir, /* IN  - pointer to the directory  */
    char        *dir_name);  /* IN  - name of this directory 	*/


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : main                                            */
/* Description   : Creates a TCL interpreter, runs the 		   */
/*		   initialization script files, registers gdscp    */
/*		   commands.  In the interactive mode, it then	   */
/*		   performs a read-and-eval loop.  In the 	   */
/*		   non-interactive mode, the commands are executed */
/*		   and an exit is made.				   */
/* Author        : A. Doerr, SNI BU BA KP13                        */
/*                                                                 */
/*-----------------------------------------------------------------*/
int main( 
    int    argc,  /* IN - number of arguments */
    char **argv)  /* IN - arguments           */
{
unsigned32   status;
Tcl_Interp  *interp;

    /* create TCL interpreter */
    interp = Tcl_CreateInterp();

    /* set the locale */
    setlocale(LC_ALL, "");

    /* load the TCL message table */
    dce_msg_define_msg_table(dce_tcl_g_table, 
			   sizeof(dce_tcl_g_table) / sizeof(dce_tcl_g_table[0]),
			   &status);

    if (status != msg_s_ok)
    {
	fprintf(stderr, "Cannot register TCL message table, error 0x%lx\n",
			status);
	exit(1);
    }

    /* load the gds message table */
    dce_msg_define_msg_table(gds_msg_table, 
			     sizeof(gds_msg_table) / sizeof(gds_msg_table[0]),
			     &status);

    if (status != msg_s_ok)
    {
	fprintf(stderr, "Cannot register gdscp message table, error 0x%lx\n",
			status);
	exit(1);
    }

    /* initialize the value of the current working object variable to root */
    gc_i_set_cwo_value(interp, "/");

    /* set the variable trace on the current working object variable */
    gc_i_set_cwo_trace(interp);

    /* register the C-parts of the gdscp commands */
    gc_i__register_commands(interp);

    /* evaluate the initialization script files */
    gc_i__evaluate_init_files(interp);

    /* check if a filename or command is given to gdscp */
    /* if yes then evaluate and exit 			*/
    gc_i__check_interactive_mode(interp, (signed16) argc, argv);

    /* read-and-evaluate-loop */
    while (TRUE)
    {
        /* read and execute a complete command */
        gc_i__read_and_eval_command(interp);
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__read_and_eval_command                     */
/* Description   : Outputs a prompt and reads input from stdin     */
/*		   until we have a complete tcl command. The       */
/*		   command is then executed.			   */
/* Return Value  : NONE                               		   */
/* Author        : A. Doerr, SNI BU BA KP13                        */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__read_and_eval_command( 
    Tcl_Interp  *interp) /* IN - TCL-interpreter which processes the command */
{
Tcl_DString   dyn_str_buf;
char         *input_line;
char         *command;
int           retval_tcl;

    /* create dynamic string buffer */
    Tcl_DStringInit(&dyn_str_buf);

    /* output primary prompt and get user input */
    input_line = readline(GC_I_PRIMARY_PROMPT);

    /* while command not complete */
    while (TRUE)
    {
	/* check if the user has hit the interrupt key, or other  */
	/* special keys.  If so, interrupt the reading and return */
	if (input_line == NULL)
	{
	    /* free the dynamic string buffer */
	    Tcl_DStringFree(&dyn_str_buf);
	    return;
	}

	/* add the command to the history list.  Note that this is  */
	/* not the TCL history list but the history list maintained */
	/* by the editline utility.				    */
	add_history(input_line);

        /* assemble input lines to a complete command */
        command = Tcl_DStringAppend(&dyn_str_buf, input_line, -1);

	free(input_line);

        /* if command is complete, stop fetching */
        if (command[0] == 0 || Tcl_CommandComplete(command))
        {
	    break;
        }
	else
	{
            /* output secondary prompt and get user input */
            input_line = readline(GC_I_SECONDARY_PROMPT);
	}
    }

    /* evaluate the command and push it onto the TCL history */
    retval_tcl = Tcl_RecordAndEval(interp, command, 0);

    /* free the dynamic string buffer */
    Tcl_DStringFree(&dyn_str_buf);

    /* command evaluated without error */
    if (retval_tcl == TCL_OK)
    {
	/* if the command has a result, print it */
	if (*interp->result != '\0')
	{
	    printf("%s\n", interp->result);
	}
    }
    else
    {
        /* if we have a tcl-interpreter error */
        if (retval_tcl == TCL_ERROR)
        {
	    if (*interp->result != '\0')
	    {
		dce_fprintf(stderr, gc_i_error_string, interp->result);
	    }
        }
        else
        {
            dce_fprintf(stderr, gc_i_tcl_error_string, retval_tcl);

	    if (*interp->result != '\0')
	    {
		fprintf(stderr, "%s\n", interp->result);
	    }
        }
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__register_commands                         */
/* Description   : Register gdscp commands in the Tcl-interpreter. */
/* Return Value  : NONE                                            */
/* Author        : A. Doerr, SNI BU BA KP13                        */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__register_commands(
    Tcl_Interp  *interp)  /* IN - Interpreter where commands are registered */
{
static gc_i_command_procedure  command_info[] = 
{
    {GC_I_X500OBJ_COMMAND_STRING,   gc_i_x500obj},
    {GC_I_X500SVC_COMMAND_STRING,   gc_i_x500svc},
    {GC_I_X500ABBR_COMMAND_STRING,  gc_i_x500abbr},
    {GC_I_EXIT_COMMAND_STRING,      gc_i_exit},
    {GC_I_QUIT_COMMAND_STRING,      gc_i_exit},
    { NULL,       	 	    (Tcl_CmdProc *) NULL}
};

gc_i_command_procedure  *command;

    /* process all commands */
    for (command = command_info; command->name != NULL; ++command)
    {
        /* register command */
        Tcl_CreateCommand(interp, command->name, command->proc,
                          (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__evaluate_init_files                       */
/* Description   : This function evaluates all the intitalization  */
/*		   files of "gdscp". The files include the init    */
/*		   file of TCL, the init file of gdscp, 	   */
/*		   $HOME/.gdscprc (if available) and all the other */
/*		   files if specified by user in the environment   */
/*		   variable. 					   */
/* Return Value  : NONE                                            */
/* Author        : M. Kronenburg SNI BU BA KP13                    */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__evaluate_init_files(
    Tcl_Interp  *interp)  /* IN - The TCL interpreter */
{
DIR        *source_dir;
signed16   part_count;
signed16   i;
char       *path_variable = NULL;
char      **dir_array = NULL;
char       *home_dir;
char        full_file_name[GC_I_MAX_PATH + 1];
char        inst_dir[GC_I_MAX_PATH + 1];
int	    retval_tcl;

    /* source in the TCL "init.tcl" file */
    if (Tcl_Eval(interp, GC_I_TCL_SOURCE_COMMAND) != TCL_OK)
    {
	/* if there is an error, just print the message on stderr */
	fprintf(stderr, "%s", interp->result);

	/* clear the interpreter result */
	Tcl_ResetResult(interp);
    }

    if (dcelocal_path != NULL)
    {
	/* build the full directory name */
	sprintf(inst_dir, "%s/%s", dcelocal_path, GC_I_GDSCP_REL_INST_PATH);
    }
    else
    {
	sprintf(inst_dir, "%s", GC_I_GDSCP_REL_INST_PATH); 
    }

    /* set the TCL environment variable to the gdscp installation path */
    Tcl_SetVar(interp, GC_I_GDSCP_LIB_VARIABLE, inst_dir, TCL_GLOBAL_ONLY);

    sprintf(full_file_name, "%s/%s", inst_dir, GC_I_GDSCP_INIT_FILE);

    /* evaluate the gdscp initialization file     */
    retval_tcl = Tcl_EvalFile(interp, full_file_name);

    if (retval_tcl != TCL_OK)
    {
	/* if there is an error, just print the message on stdout */
	fprintf(stderr, "%s", interp->result);

	/* clear the interpreter result */
	Tcl_ResetResult(interp);
    }

    /* read the user customized init file */
    if ((home_dir = getenv("HOME")) != NULL)
    {
	sprintf(full_file_name, "%s/%s", home_dir, GC_I_RC_FILE);

	/* check if file exists */
	if (access(full_file_name, F_OK) == 0)
	{
	    /* evaluate the file */
	    retval_tcl = Tcl_EvalFile(interp, full_file_name);
    
	    if (retval_tcl != TCL_OK)
	    {
		/* if there is an error, just print warning message to user */
	        dce_fprintf(stderr, gc_i_warning_file_eval, full_file_name, 
			    interp->result);

		/* clear the interpreter result */
		Tcl_ResetResult(interp);
	    }
	}
    }

    /* if the user has specified directories in the environment variable, */
    /* evaluate all the files in that directory.			  */
    if ((path_variable = getenv(GC_I_USER_DIR_ENV)) != NULL)
    {
	/* if the user has supplied multiple directory paths, split them */
	/* into several directories 					 */
	gc_i__split_paths(path_variable, GC_I_PATH_DIV, &dir_array, 
			  &part_count);

	for (i = 0; i < part_count; ++i)
	{
	    source_dir = opendir(dir_array[i]);
	    if (source_dir == NULL)
	    {
		dce_fprintf(stderr, gc_i_cannot_open_dir, dir_array[i]);
	    }
	    else
	    {
		/* source in and evaluate script files in the directory */
		gc_i__source_user_scripts(interp, source_dir, dir_array[i]);
		closedir(source_dir);
	    }
	}
    }
}
    

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__check_interactive_mode                    */
/* Description   : This function checks if a file argument was     */
/*		   specified to gdscp.  If so, the file is 	   */
/*		   evaluated.  It also checks if the user has      */
/*		   specified the command line option. If so, 	   */
/*		   the user specified commands are executed.	   */
/*                 Additionally this function checks all arguments */
/*		   given to gdscp and terminates gdscp if there is */
/*		   a wrong commandline.				   */
/* Return Value  : NONE                                            */
/* Author        : M. Kronenburg SNI BU BA KP13                    */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__check_interactive_mode( 
    Tcl_Interp  *interp, /* IN - The TCL interpreter */
    signed16 	 argc,   /* IN - Number of arguments */
    char       **argv)   /* IN - Arguments	     */
{
gc_t_vector  *out_vect;
boolean       command_specified, file_specified;
boolean       print_usage_message;
signed16      i = 0;
char	     *command_string;
int           retval_tcl;

    command_specified = FALSE;
    file_specified = FALSE;
    gc_i_interactive_mode = TRUE;
    print_usage_message = FALSE;

    if (argc > 1)
    {
	gc_i_interactive_mode = FALSE;

	++i;

	/* if the GC_I_COMMAND_SWITCH is specified */
	if (strcmp(argv[i], GC_I_COMMAND_SWITCH) == 0)
	{
	    ++i;

	    /* if nothing is specified after the switch */
	    if (argc == i)
	    {
		print_usage_message = TRUE;
	    }
	    else
	    {
		command_specified = TRUE;
	    }
	}
	else
	{
	    /* only file option possible now 				  */
	    /* check number of arguments and also check if it is a switch */
	    if (argc > 2 || argv[i][0] == '-')
	    {
		print_usage_message = TRUE;
	    }
	    else
	    {
		/* check if file exists */
		if (access(argv[i], F_OK) != 0)
		{
		    dce_fprintf(stderr, gc_i_file_does_not_exist, argv[i]);

		    /* call the translator clean up routine */
		    gc_t_unbind(&out_vect);

		    exit(GC_I_FILE_DOES_NOT_EXIST);
		}

		file_specified = TRUE;
	    }
	}
    }

    if (print_usage_message == TRUE)
    {
	dce_fprintf(stderr, gc_i_incorrect_usage);
	dce_fprintf(stderr, gc_i_usage_msg);

	/* call the translator clean up routine */
	gc_t_unbind(&out_vect);

	exit (GC_I_INCORRECT_CMD_LINE);
    }

    /* set the "tcl_interactive" variable */
    Tcl_SetVar(interp, "tcl_interactive",
 	       ((gc_i_interactive_mode == TRUE) && isatty(0)) ? "1" : "0",
	       TCL_GLOBAL_ONLY);

    if (file_specified == TRUE)
    {
	/* evaluate the given file */
	retval_tcl = Tcl_EvalFile(interp, argv[i]);

	if (retval_tcl != TCL_OK)
	{
	    dce_fprintf(stderr, gc_i_error_string, interp->result);

	    /* unbind from the server and clean up */
	    gc_t_unbind(&out_vect);

	    exit(GC_I_TCL_EVALFILE_FAILED);
	}
	    
	/* unbind from the server and clean up */
	gc_t_unbind(&out_vect);

	exit(GC_I_EXIT_SUCCESS);
    }

    if (command_specified == TRUE)
    {
	argc -= i;
	argv += i;

	/* create the complete string consisting of commands */
	command_string = Tcl_Concat(argc, argv);	

	/* evaluate the commands */
	retval_tcl = Tcl_VarEval(interp, command_string, NULL);

	free(command_string);

	if (retval_tcl == TCL_OK)
	{
	    /* unbind from the server and clean up */
	    gc_t_unbind(&out_vect);

	    exit(GC_I_EXIT_SUCCESS);
	}
	else
	{
	    if (retval_tcl == TCL_ERROR)
	    {
		if (*interp->result != '\0')
		{
		    dce_fprintf(stderr, gc_i_error_string, interp->result);
		}
	    }
	    else
	    {
		dce_fprintf(stderr, gc_i_tcl_error_string, retval_tcl);

		if (*interp->result != '\0')
		{
		    fprintf(stderr, "%s\n", interp->result);
		}
	    }

	    /* unbind from the server and clean up */
	    gc_t_unbind(&out_vect);

	    exit (GC_I_TCL_EVAL_FAILED);
	}
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__split_paths                               */
/* Description   : This function splits the user provided string   */
/*		   which consists of multiples paths into          */
/*		   individual parts.				   */
/* Return Value  : NONE						   */
/* Author        : M. Kronenburg SNI BU BA KP13                    */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__split_paths( 
    char       *path_string, /* IN  - user provided string                   */
    char        div_char ,   /* IN  - character dividing several directories */
    char     ***path_array,  /* OUT - the several parts of the paths         */
    signed16   *dir_count)   /* OUT - number of directories in input string  */
{
signed16   part_count = 0;
char      *index_ptr;
char      *start_ptr;

    start_ptr = path_string;

    /* skipping all dividing characters at the beginning of the string */
    while (*start_ptr == div_char)
    {
	++start_ptr;
    }

    /* count the number of different parts in user string */
    while ((index_ptr = strchr(start_ptr, (int) div_char)) != NULL)
    {
	++part_count;
	start_ptr = index_ptr + 1;

	/* skip all dividing characters */
	while (*start_ptr == div_char)
	{
	    ++start_ptr;
	}
    }

    /* check if there is a directory at the end */
    if (*start_ptr != '\0')
    {
	++part_count;
    }

    *dir_count = part_count;

    *path_array = (char **) malloc(sizeof(char *) * part_count);

    if (*path_array == NULL)
    {
	/* don't expect malloc failure so early, since this is the */
	/* initialization phase of gdscp. But if malloc does fail, */
	/* do not process the user provided path string.	   */
	*dir_count = 0;
	return;
    }

    start_ptr = path_string;
    part_count = 0;

    /* skipping all dividing characters at the beginning of the string */
    while (*start_ptr == div_char)
    {
	++start_ptr;
    }

    /* search for all parts */
    while ((index_ptr = strchr(start_ptr, (int) div_char)) != NULL)
    {
	(*path_array)[part_count] = (char *) malloc(sizeof(char) *
						   (index_ptr - start_ptr + 1));

	if ((*path_array)[part_count] == NULL)
	{
	    /* don't expect malloc failure so early, since this is the */
	    /* initialization phase of gdscp. But if malloc does fail, */
	    /* do not process the user provided path string.	       */
	    *dir_count = 0;
	    return;
	}

	strncpy((*path_array)[part_count], start_ptr, index_ptr - start_ptr);
	((*path_array)[part_count])[index_ptr-start_ptr] = '\0';

	start_ptr = index_ptr + 1;

	/* skipping all dividing characters */
	while (*start_ptr == div_char)
	{
	    ++start_ptr;
	}

	++part_count;
    }

    /* check if there is a directory at the end */
    if (*start_ptr != '\0')
    {
	(*path_array)[part_count] = (char *) malloc(sizeof(char) *
						    (strlen(start_ptr) + 1));

	if ((*path_array)[part_count] == NULL)
	{
	    /* don't expect malloc failure so early, since this is the */
	    /* initialization phase of gdscp. But if malloc does fail, */
	    /* do not process the user provided path string.	       */
	    *dir_count = 0;
	    return;
	}

	strncpy((*path_array)[part_count], start_ptr, strlen(start_ptr));
	((*path_array)[part_count])[strlen(start_ptr)] = '\0';
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__source_user_scripts                       */
/* Description   : This function is used to source in all the TCL  */
/*		   scripts (files ending with ".tcl" from a user   */
/*		   specified directory.				   */
/* Return Value  : NONE						   */
/* Author        : M. Kronenburg SNI BU BA KP13                    */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__source_user_scripts( 
    Tcl_Interp  *interp,     /* IN  - The TCL interpreter       */ 
    DIR         *source_dir, /* IN  - pointer to the directory  */
    char        *dir_name)   /* IN  - name of this directory 	*/
{
struct dirent  *buf;
char 	        full_file_name[GC_I_MAX_PATH + 1];
int             len;
int             retval_tcl;

    /* All files in the directory ending with ".tcl" are sourced. */
    while ((buf = readdir(source_dir)) != NULL)
    {
	/* check if suffix is ".tcl".  if yes then source the file */
	if ((len = strlen(buf->d_name)) >= strlen(GC_I_TCL_SUF))
	{
	    if (strcmp(buf->d_name + len - strlen(GC_I_TCL_SUF), GC_I_TCL_SUF) 
		== 0)
	    {
		sprintf(full_file_name, "%s/%s", dir_name, buf->d_name);

		/* evaluate the file */
		retval_tcl = Tcl_EvalFile(interp, full_file_name);
    
		if (retval_tcl != TCL_OK)
		{
		    dce_fprintf(stderr, gc_i_warning_file_eval, full_file_name,
				interp->result);
		}
	    }
	}
    }
}
