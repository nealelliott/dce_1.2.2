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
 * $Log: gcimisc.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:45  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:22  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:12  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:44  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  11:46:23  marrek]
 * 
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:56:42  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:06  keutel
 * 	creation
 * 	[1994/03/21  12:54:47  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gcimisc.c					   */
/* Description : This file contains modules for the miscellaneous  */
/*		 commands of "gdscp".			   	   */
/* Date        : January, 1994                                     */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gcimisc.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:33 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <gds.h>

#include <dce/tcl.h>

#include <gdscp.h>
#include <gctconv.h>
#include <gctstruct.h>

#include <gcistruct.h>
#include <gciutil.h>
#include <gcivar.h>
#include <gcimisc.h>


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_exit                                       */
/* Description   : Command procedure for gdscp "exit/quit" command.*/
/*                 Function substitutes functionality of built in  */
/*		   "exit" command. Additionally, it also terminates*/
/*                 the directory session.             		   */
/*		   In the case of errors, the TCL interpreter      */
/*		   result is set to the error message and the      */
/*		   TCL error variable "errorCode" is set to the    */
/*		   error code.					   */
/* Return Value  : TCL_OK if exit was successful                   */
/*                 TCL_ERROR otherwise                             */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
int gc_i_exit(
    ClientData    client_data,/* IN - Not used 				      */
    Tcl_Interp   *interp,     /* IN - Tcl-interpreter which processes the cmd */
    int           argc,       /* IN - Number of arguments 		      */
    char        **argv)       /* IN - Arguments themselves 		      */
{
gc_t_vector  *out_vect;
signed16      exit_status;
signed16      i;
char	     *append_string;
int	      return_code;
int           exit_code;

    exit_status = GC_SUCCESS;
    exit_code = 0;
    return_code = TCL_OK;
    append_string = NULL;

    /* clear result for interpreter */
    Tcl_ResetResult(interp);

    if (argc == 0 || argv == NULL || *argv == NULL)
    {
	exit_status = GC_I_INVALID_PARAMETER;
    }
    else
    {
	/* skip past the "exit" command, which is in first argument vector */
	++argv;
	--argc;

	if (argc > 0)
	{
	    if (argc > 1)
	    {
		exit_status = GC_I_TOO_MANY_ARGUMENTS;
	    }
	    else
	    {
		/* check if a valid number is supplied */
		for (i = 0; argv[0][i] != '\0'; ++i)
		{
		    if (!isdigit(argv[0][i]))
			break;
		}

		if (argv[0][i] == '\0')
		{
		    exit_code = atoi(argv[0]);
		}
		else
		{
		    if (argv[0][0] == '-')
		    {
			exit_status = GC_I_UNKNOWN_OPTION;
		    }
		    else
		    {
			exit_status = GC_I_INVALID_ARGUMENT;
		    }
		    append_string = argv[0];
		}
	    }
	}
    }

    if (exit_status != GC_SUCCESS)
    {
	/* set the interpreter result to the appropriate error message */
	gc_i_set_interpreter_error(exit_status, append_string, interp);

	return_code = TCL_ERROR;
    }
    else
    {
	exit_status = gc_t_unbind(&out_vect);

	if (exit_status != GC_SUCCESS)
	{
	    /* set the interpreter result to the appropriate error message */
	    gc_i_set_interp_to_trans_error(out_vect, exit_status, interp);

	    /* free the result vector */
	    gc_t_free_result(out_vect);

	    return_code = TCL_ERROR;
	}
    }

    if (return_code != TCL_OK)
    {
	/* set the TCL errorCode variable */
	gc_i_set_error_code(interp, exit_status);

	return(return_code);
    }

    exit (exit_code);
}
