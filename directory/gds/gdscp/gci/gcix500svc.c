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
 * $Log: gcix500svc.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:57  root]
 * 
 * Revision 1.1.2.4  1994/09/19  08:39:30  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:33:05  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:30  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:22  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:52  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  11:46:42  marrek]
 * 
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:57:37  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:20  keutel
 * 	creation
 * 	[1994/03/21  12:55:30  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gcix500svc.c					   */
/* Description : This file contains modules for the manipulation   */
/*		 of x500svc object.				   */
/* Date        : January, 1994                                     */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gcix500svc.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:45 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gds.h>

#include <dce/tcl.h>

#include <gdscp.h>
#include <gctstruct.h>
#include <gctconv.h>

#include <gcistruct.h>
#include <gciutil.h>
#include <gciprint.h>
#include <gcivar.h>
#include <gcix500svc.h>


/*-- Static Function(s) -------------------------------------------*/
int gc_i__x500svc_modify(
    ClientData    client_data,/* IN - Not used 				      */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments 		      */
    char        **argv);      /* IN - Arguments themselves 		      */

int gc_i__x500svc_help(
    ClientData    client_data,/* IN - Not used 				      */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments 		      */
    char        **argv);      /* IN - Arguments themselves 		      */

int gc_i__x500svc_show(
    ClientData    client_data,/* IN - Not used 				      */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments 		      */
    char        **argv);      /* IN - Arguments themselves 		      */

int gc_i__x500svc_operations(
    ClientData    client_data,/* IN - Not used 				      */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments 		      */
    char        **argv);      /* IN - Arguments themselves 		      */

static void gc_i__display_x500svc_help(
    void);

static void gc_i__display_x500svc_verbose(
   void);

static void gc_i__display_x500svc_oper_help(
    char  *operation); /* IN - The operation about which help is sought */


/*-- Static Variable(s) ------------------------------------------*/
/* To store the valid operations and their callbacks */
/* Do not change the order in the table, because the "operations" */
/* command returns this order.					  */
static gc_i_command_procedure  gc_i_x500svc_cmd_info[] = 
{ 
    {GC_I_MODIFY_OPERATION_STRING,     gc_i__x500svc_modify},
    {GC_I_SHOW_OPERATION_STRING,       gc_i__x500svc_show},
    {GC_I_HELP_OPERATION_STRING,       gc_i__x500svc_help},
    {GC_I_OPERATIONS_OPERATION_STRING, gc_i__x500svc_operations},
    {NULL,                             (Tcl_CmdProc *) NULL}
};


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_x500svc                             	   */
/* Description   : This function invokes the relevant function to  */
/*		   perform the desired operation on the x500svc    */
/*		   object.  		   			   */
/*		   In the case of errors, the TCL interpreter      */
/*		   result is set to the error message and the      */
/*		   TCL error variable "errorCode" is set to the    */
/*		   error code.					   */
/* Return Value  : TCL_OK or TCL_ERROR				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
int gc_i_x500svc(
    ClientData    client_data,/* IN - Not used                                */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments                     */
    char        **argv)       /* IN - Arguments themselves                    */
{
signed16  op_index;
signed16  status;
int       return_code;


    status = GC_SUCCESS;
    return_code = TCL_OK;

    if (argc == 0 || argv == NULL || argv[0] == NULL)
    {
         status = GC_I_INVALID_PARAMETER;
    }
    else
    {
	if (argc <= 1 || argv[1] == NULL)
	{
	    status = GC_I_MISSING_OPERATION;
	}
    }

    if (status != GC_SUCCESS)
    {
	/* set the interpreter result to the appropriate error message */
	gc_i_set_interpreter_error(status, NULL, interp);

	return_code = TCL_ERROR;
    }
    else
    {
	/* skip past first argument vector which contains "x500svc" string */
	++argv;
	--argc;

	/* check if the operation is registered in the procedure table */
	status = gc_i_check_operation(gc_i_x500svc_cmd_info, *argv,
				      &op_index, interp);

	if (status != GC_SUCCESS)
	{
	    return_code = TCL_ERROR;
	}
    }

    if (return_code != TCL_OK)
    {
	/* set the TCL errorCode variable */
	gc_i_set_error_code(interp, status);
    }
    else
    {
	/* skip through the operation argument */
	++argv;
	--argc;

	/* call the relevant procedure */
	return_code = (gc_i_x500svc_cmd_info[op_index].proc)(client_data,interp,
							     argc, argv);
    }

    return(return_code);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__x500svc_modify                            */
/* Description   : This function invokes the Translator moddsc     */
/*		   function.                         		   */
/*		   In the case of errors, the TCL interpreter      */
/*		   result is set to the error message and the      */
/*		   TCL error variable "errorCode" is set to the    */
/*		   error code.					   */
/* Return Value  : TCL_OK if successful, TCL_ERROR otherwise       */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
int gc_i__x500svc_modify(
    ClientData    client_data,/* IN - Not used 				      */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments 		      */
    char        **argv)       /* IN - Arguments themselves 		      */
{
gc_t_vector  *out_vect;
signed16      new_argc;
signed16      modify_status;
char	    **new_argv;
int	      return_code;

    return_code = TCL_OK;

    /* clear result for interpreter */
    Tcl_ResetResult(interp);

    /* rebuild the argument list */
    modify_status = gc_i_repack_arguments(argv, argc, GC_T_MOD_DSC, &new_argv,
					  &new_argc, NULL, interp);

    if (modify_status == GC_SUCCESS)
    {
	/* invoke the translator function */
	modify_status = gc_t_moddsc(new_argv, new_argc, &out_vect);

	/* free the created arguments */
	gc_i_free_created_args(new_argv);

	if (modify_status != GC_SUCCESS)
	{
	    /* set the interpreter result to the error message */
	    gc_i_set_interp_to_trans_error(out_vect, modify_status, interp);

	    /* finished with result vector */
	    gc_t_free_result(out_vect);
	}
    }

    if (modify_status != GC_SUCCESS)
    {
	/* set the TCL errorCode variable */
	gc_i_set_error_code(interp, modify_status);

	return_code = TCL_ERROR;
    }

    return(return_code);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__x500svc_show                              */
/* Description   : This function invokes the Translator showdsc    */
/*		   function and processes the result.		   */
/*		   If the request is for a TCL list, the TCL	   */
/*		   interpreter result is set to the returned list. */
/*		   In the case of errors, the TCL interpreter      */
/*		   result is set to the error message and the      */
/*		   TCL error variable "errorCode" is set to the    */
/*		   error code.					   */
/* Return Value  : TCL_OK if successful, TCL_ERROR otherwise       */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
int gc_i__x500svc_show(
    ClientData    client_data,/* IN - Not used 				      */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments 		      */
    char        **argv)       /* IN - Arguments themselves 		      */
{
gc_t_vector  *out_vect;
signed16      new_argc;
signed16      show_status;
boolean       structured_result;
char	    **new_argv;
int	      return_code;

    return_code = TCL_OK;
    new_argv = NULL;
    out_vect = NULL;

    /* clear result for interpreter */
    Tcl_ResetResult(interp);

    /* check if GC_T_STRUCTURED_RESULT_SWITCH has been specified */
    /* and create a new argument vector. The new created vector  */
    /* does not contain this switch.			         */
    show_status = gc_i_repack_arguments(argv, argc, GC_T_SHOW_DSC,
					&new_argv, &new_argc, 
					&structured_result, interp);
    if (show_status != GC_SUCCESS)
    {
	return_code = TCL_ERROR;
    }
    else
    {
	/* if there are some more arguments */
	if (new_argc > 0)
	{
	    if (new_argv[0][0] == '-')
	    {
		show_status = GC_I_UNKNOWN_OPTION;
	    }
	    else
	    {
		show_status = GC_I_UNKNOWN_ARGUMENT;
	    }

	    /* set the interpreter result to the appropriate error message */
	    gc_i_set_interpreter_error(show_status, new_argv[0], interp);

	    return_code = TCL_ERROR;
	}
    }

    /* free the created new argument array */
    if (new_argv != NULL)
    {
	gc_i_free_created_args(new_argv);
    }

    if (return_code == TCL_OK)
    {
	if (structured_result == TRUE)
	{
	    /* invoke the translator function requesting for full names */
	    show_status = gc_t_showdsc(FALSE, &out_vect);
	}
	else
	{
	    /* invoke translator function requesting for abbreviated results*/
	    show_status = gc_t_showdsc(TRUE, &out_vect);
	}

	if (show_status != GC_SUCCESS)
	{
	    /* set the interpreter result to the appropriate error message */
	    gc_i_set_interp_to_trans_error(out_vect, show_status, interp);

	    return_code = TCL_ERROR;
	}
    }

    if (return_code == TCL_OK)
    {
	/* if results are to be printed in a structured format */
	if (structured_result == TRUE)
	{
	    /* convert the result so that it can be printed in a */
	    /* structured format.				 */
	    show_status = gc_i_prepare_showdsc_print_result(out_vect, interp);

	    if (show_status == GC_SUCCESS)
	    {
		/* print the result */
		gc_i_print_result();
	    }
	}
	else
	{
	    /* convert the result into a TCL list */
	    show_status = gc_i_convert_showdsc_result(out_vect, interp);
	}

	if (show_status != GC_SUCCESS)
	{
	    return_code = TCL_ERROR;
	}
    }

    if (return_code != TCL_OK)
    {
	/* set the TCL errorCode variable */
	gc_i_set_error_code(interp, show_status);
    }

    if (out_vect != NULL)
    {
	/* finished with result vector */
	gc_t_free_result(out_vect);
    }

    return(return_code);
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__x500svc_help                              */
/* Description   : This function prints the help text about the    */
/*		   x500svc object and its operations.		   */
/*		   In the case of errors, the TCL interpreter      */
/*		   result is set to the error message and the      */
/*		   TCL error variable "errorCode" is set to the    */
/*		   error code.					   */
/* Return Value  : TCL_OK if successful, TCL_ERROR otherwise       */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
int gc_i__x500svc_help(
    ClientData    client_data,/* IN - Not used 				      */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments 		      */
    char        **argv)       /* IN - Arguments themselves 		      */
{
signed16  status;
signed16  i;
char     *append_string;
char     *verbose_string = GC_I_VERBOSE_SWITCH;
int       return_code;

    status = GC_SUCCESS;
    return_code = TCL_OK;
    append_string = NULL;

    /* clear result for interpreter */
    Tcl_ResetResult(interp);

    if (argc > 1)
    {
	status = GC_I_TOO_MANY_ARGUMENTS;
    }
    else
    {
	if (argc == 0)
	{
	    /* display help about the x500svc object and its operations */
	    gc_i__display_x500svc_help();
	}
	else
	{
	    append_string = argv[0];

	    if (argv[0][0] == '-')
	    {
		/* if the user supplied argument is an abbreviated or a */
		/* full form of the verbose swith.		        */
		if (strstr(verbose_string, argv[0]) == verbose_string)
		{
		   /* display help about x500svc object (like in man page) */
		    gc_i__display_x500svc_verbose();
		}
		else
		{
		    status = GC_I_UNKNOWN_OPTION;
		}
	    }
	    else
	    {
		/* user has requested help about the operation */
		/* locate the operation in the operation table */
		for (i = 0; gc_i_x500svc_cmd_info[i].name != NULL; ++i)
		{
		    if (strcmp(gc_i_x500svc_cmd_info[i].name, argv[0]) == 0)
		    {
			break;
		    }
		}

		/* operation found */
		if (gc_i_x500svc_cmd_info[i].name != NULL)
		{
		    /* display help about the oeration and its switches */
		    gc_i__display_x500svc_oper_help(argv[0]);
		}
		else
		{
		    status = GC_I_UNKNOWN_OPERATION;
		}
	    }
	}
    }

    if (status != GC_SUCCESS)
    {
	gc_i_set_interpreter_error(status, append_string, interp);

	/* set the TCL errorCode variable */
	gc_i_set_error_code(interp, status);

	return_code = TCL_ERROR;
    }

    return(return_code);
}

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__x500svc_operations                        */
/* Description   : This function returns the list of valid 	   */
/*		   operations for the x500svc object.		   */
/*		   The TCL interpreter result is set to the	   */
/*		   returned result list.			   */
/*		   In the case of errors, the TCL interpreter      */
/*		   result is set to the error message and the      */
/*		   TCL error variable "errorCode" is set to the    */
/*		   error code.					   */
/* Return Value  : TCL_OK if successful, TCL_ERROR otherwise       */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
int gc_i__x500svc_operations(
    ClientData    client_data,/* IN - Not used 				      */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments 		      */
    char        **argv)       /* IN - Arguments themselves 		      */
{
signed16  status;
int       return_code;

    status = GC_SUCCESS;
    return_code = TCL_OK;

    /* clear result for interpreter */
    Tcl_ResetResult(interp);

    /* if there are more arguments */
    if (argc > 0)
    {
	if (argv[0][0] == '-')
	{
	    status = GC_I_UNKNOWN_OPTION;
	}
	else
	{
	    status = GC_I_UNKNOWN_ARGUMENT;
	}

	gc_i_set_interpreter_error(status, argv[0], interp);
    }
    else
    {
	status = gc_i_create_list_of_operations(gc_i_x500svc_cmd_info, interp);
    }

    if (status != GC_SUCCESS)
    {
	/* set the TCL errorCode variable */
	gc_i_set_error_code(interp, status);

	return_code = TCL_ERROR;
    }

    return(return_code);  
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__display_x500svc_help                      */
/* Description   : This function prints the help text about the    */
/*		   x500svc object and its operations on stdout.    */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__display_x500svc_help(
    void)
{
signed16  max_len;
signed16  len;
signed16  i;
 
    max_len = 0;

    /* compute the maximum length of the operation */
    for (i = 0; gc_i_x500svc_cmd_info[i].name != NULL; ++i)
    {
	len = strlen(gc_i_x500svc_cmd_info[i].name);

	if (len > max_len)
	{
	    max_len = len;
	}
    }

    for (i = 0; gc_i_x500svc_cmd_info[i].name != NULL; ++i)
    {
	/* print the operation name */
	printf("%s", gc_i_x500svc_cmd_info[i].name);

	len = strlen(gc_i_x500svc_cmd_info[i].name);

	/* format the output */
	while (len <= max_len)
	{
	    putchar(' ');
	    ++len;
	}

	putchar(' ');

	if (strcmp(GC_I_MODIFY_OPERATION_STRING, 
		   gc_i_x500svc_cmd_info[i].name) == 0)
	{
	    /* display text from the catalog */
	    dce_fprintf(stdout, gc_i_help_x500svc_modify);
	    continue;
	}

	if (strcmp(GC_I_SHOW_OPERATION_STRING, 
		   gc_i_x500svc_cmd_info[i].name) == 0)
	{
	    /* display text from the catalog */
	    dce_fprintf(stdout, gc_i_help_x500svc_show);
	    continue;
	}

	if (strcmp(GC_I_HELP_OPERATION_STRING, 
		   gc_i_x500svc_cmd_info[i].name) == 0)
	{
	    /* display text from the catalog */
	    dce_fprintf(stdout, gc_i_help_x500svc_help);
	    continue;
	}

	if (strcmp(GC_I_OPERATIONS_OPERATION_STRING, 
		   gc_i_x500svc_cmd_info[i].name) == 0)
	{
	    /* display text from the catalog */
	    dce_fprintf(stdout, gc_i_help_x500svc_operations);
	    continue;
	}
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__display_x500svc_verbose                   */
/* Description   : This function prints the help text explaining   */
/*		   the x500svc object and what it represents.      */
/*	           The text is similar to the man page entry.      */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__display_x500svc_verbose(
   void)
{
    /* display text from the catalog */
    dce_fprintf(stdout, gc_i_help_x500svc_verbose);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__display_x500svc_oper_help                 */
/* Description   : This function prints the help text about an     */
/*		   operation on the x500svc object and the valid   */
/*		   switches for the operation.			   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__display_x500svc_oper_help(
    char  *operation)  /* IN - The operation about which help is sought */
{
    if (strcmp(operation, GC_I_MODIFY_OPERATION_STRING) == 0)
    {
	/* display text from the catalog */
	dce_fprintf(stdout, gc_i_help_x500svc_modify_autocon_option);
	dce_fprintf(stdout, gc_i_help_x500svc_modify_cachecls_option);
	dce_fprintf(stdout, gc_i_help_x500svc_modify_chprohib_option);
	dce_fprintf(stdout, gc_i_help_x500svc_modify_default_option);
	dce_fprintf(stdout, gc_i_help_x500svc_modify_noderef_option);
	dce_fprintf(stdout, gc_i_help_x500svc_modify_nocopy_option);
	dce_fprintf(stdout, gc_i_help_x500svc_modify_duacache_option);
	dce_fprintf(stdout, gc_i_help_x500svc_modify_duafirst_option);
        dce_fprintf(stdout, gc_i_help_x500svc_modify_local_option);
        dce_fprintf(stdout, gc_i_help_x500svc_modify_prefadm_option);
        dce_fprintf(stdout, gc_i_help_x500svc_modify_prefchain_option);
        dce_fprintf(stdout, gc_i_help_x500svc_modify_priority_option);
        dce_fprintf(stdout, gc_i_help_x500svc_modify_scope_option);
        dce_fprintf(stdout, gc_i_help_x500svc_modify_sizelimit_option);
        dce_fprintf(stdout, gc_i_help_x500svc_modify_timelimit_option);
	dce_fprintf(stdout, gc_i_help_x500svc_modify_usedsa_option);
	return;
    }

    if (strcmp(operation, GC_I_SHOW_OPERATION_STRING) == 0)
    {
	/* display text from the catalog */
	dce_fprintf(stdout, gc_i_help_x500svc_show_struct_option);
	return;
    }

    if (strcmp(operation, GC_I_HELP_OPERATION_STRING) == 0)
    {
	/* display text from the catalog */
	dce_fprintf(stdout, gc_i_help_x500svc_help_verbose_option);
	return;
    }

    if (strcmp(operation, GC_I_OPERATIONS_OPERATION_STRING) == 0)
    {
	/* there are no switches for this operation. so simply return */
	return;
    }
}
