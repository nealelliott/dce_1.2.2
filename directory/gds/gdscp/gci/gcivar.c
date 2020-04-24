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
 * $Log: gcivar.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:51  root]
 * 
 * Revision 1.1.2.4  1994/09/19  08:39:27  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:32:40  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:26  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:17  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:48  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:57:11  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:35:31  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:12  keutel
 * 	creation
 * 	[1994/03/21  12:55:09  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gcivar.c					   */
/* Description : This file contains modules for the manipulation of*/
/*		 TCL global variables.				   */
/* Date        : January, 1994                                     */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gcivar.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:39 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <gds.h>

#include <dce/tcl.h>

#include <gdscp.h>
#include <gctconv.h>
#include <gctstruct.h>

#include <gcistruct.h>
#include <gciutil.h>
#include <gcivar.h>

/*-- Local Defines ------------------------------------------------*/
#define  GC_I_CWO_VARIABLE  "gdscp_cwo"
#define  GC_I_T61_VARIABLE  "gdscp_t61"

/*-- Function Prototypes ------------------------------------------*/
char *gc_i__set_current_object(
    ClientData   client_data,/* IN - Not used                                */
    Tcl_Interp  *interp,     /* IN - Tcl-interpreter which processes the cmd */
    char	*name1,	     /* IN - Name of the variable which is traced    */
    char	*name2,	     /* IN - Not used.				     */
    int		 flags);     /* IN - Indicates the type of modification      */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_set_error_code                             */
/* Description   : This function is used to set the TCL variable   */
/*		   "errorCode" to the desired value.		   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_i_set_error_code(
    Tcl_Interp  *interp,    /* IN - Tcl-interpreter which processes the cmd */
    signed16  	 value)     /* IN - The value to be set 		    */
{
char value_string[16];

    sprintf(value_string, "%d", value);

    /* invoke the TCL set error code function */
    Tcl_SetErrorCode(interp, "GDSCP", value_string, (char *) NULL);

}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_set_cwo_value                              */
/* Description   : This function is used to set the current working*/
/*		   object variable to the provided value.	   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_i_set_cwo_value(
    Tcl_Interp  *interp,  /* IN - Tcl-interpreter which processes the cmd */
    char	*value)   /* IN - The value to be set.			  */
{
    Tcl_SetVar(interp, GC_I_CWO_VARIABLE, value, TCL_GLOBAL_ONLY);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_set_cwo_trace                              */
/* Description   : This function is used to set the variable trace */
/*		   on the global current working object variable   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_i_set_cwo_trace(
    Tcl_Interp  *interp)  /* IN - Tcl-interpreter which processes the cmd */
{
    Tcl_TraceVar(interp, GC_I_CWO_VARIABLE, 
		 TCL_GLOBAL_ONLY | TCL_TRACE_WRITES | TCL_TRACE_UNSETS,
		 gc_i__set_current_object, (ClientData) NULL);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__set_current_object                        */
/* Description   : This function is the trace procedure which is   */
/*		   invoked when the current working object variable*/
/*		   is modified. It in turn, calls the translator   */
/*		   gc_t_moveto function.			   */
/*		   In case of errors, the TCL interpreter result   */
/*		   is set to the error message.			   */
/* Return Value  : NULL or error message			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
char *gc_i__set_current_object(
    ClientData   client_data,/* IN - Not used                                */
    Tcl_Interp  *interp,     /* IN - Tcl-interpreter which processes the cmd */
    char	*name1,	     /* IN - Name of the variable which is traced    */
    char	*name2,	     /* IN - Not used.				     */
    int		 flags)      /* IN - Indicates the type of modification      */
{
gc_t_vector  *out_vector;
static char   error_msg[GC_T_MAX_CWO_ERR_STRING + 1];
signed16      status;
boolean	      local_strings;
char  	     *value;
char	     *str;

    /* clear result for interpreter */
    Tcl_ResetResult(interp);

    *error_msg = '\0';

    /* first remove the trace on the variable so that this function */
    /* is not invoked again, when the variable is reset.	    */
    Tcl_UntraceVar(interp, GC_I_CWO_VARIABLE, 
		   TCL_GLOBAL_ONLY | TCL_TRACE_WRITES | TCL_TRACE_UNSETS,
		   gc_i__set_current_object, (ClientData) NULL);

    /* if the current working object variable is being unset, return error */
    if (flags & TCL_TRACE_UNSETS)
    {
	/* get the error message from the catalog */
	str = (char *) dce_sprintf(gc_i_cannot_unset_var, GC_I_CWO_VARIABLE);

	strcpy(error_msg, str);

	/* set the interpreter result to the error message */
	Tcl_SetResult(interp, str, TCL_DYNAMIC);

	status = GC_I_CANNOT_UNSET_VAR;

	/* set the TCL errorCode variable */
	gc_i_set_error_code(interp, status);
    }
    else
    {
	/* read the value of the variable */
	value = Tcl_GetVar(interp, GC_I_CWO_VARIABLE, TCL_GLOBAL_ONLY);

	if (value != NULL)
	{
	    /* check if local strings specified or T61 string */ 
	    local_strings = gc_i_is_local_string(interp);

	    /* invoke the translator function */
	    status = gc_t_moveto(&value, 1, local_strings, &out_vector);

	    if (status != GC_SUCCESS)
	    {
		/* set the interpreter result to the error message */
		gc_i_set_interp_to_trans_error(out_vector, status, interp);

		strcpy(error_msg, interp->result);

		/* free the out vector */
		gc_t_free_result(out_vector);

		/* set the TCL errorCode variable */
		gc_i_set_error_code(interp, status);
	    }
	}
    }

    /* call the translator function to read the current object */
    if ((status = gc_t_pwo(&out_vector)) == GC_SUCCESS)
    {
	/* reset the current working object variable */
	gc_i_set_cwo_value(interp, out_vector[0].value);

	/* free the out vector */
	gc_t_free_result(out_vector);

	/* set the trace to the variable again */
	gc_i_set_cwo_trace(interp);
    }
    else
    {
	/* set the interpreter result to the error message */
	gc_i_set_interp_to_trans_error(out_vector, status, interp);

	strcpy(error_msg, interp->result);

	/* free the out vector */
	gc_t_free_result(out_vector);

	/* set the TCL errorCode variable */
	gc_i_set_error_code(interp, status);
    }

    /* if an error has occurred */
    if (*error_msg != '\0')
    {
	return(error_msg);
    }
    else
    {
	/* in case of success, the trace procedure should return NULL */
	return(NULL);
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_is_local_string                            */
/* Description   : This function reads the gdscp_t61 TCL variable  */
/*		   and accordingly returns a boolean value.        */
/* Return Value  : TRUE if gdscp_t61 undefined or gdscp_t61=FALSE. */
/*		   FALSE if gdscp_t61=TRUE.			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
boolean gc_i_is_local_string(
    Tcl_Interp  *interp)  /* IN - Tcl-interpreter which processes the cmd */
{
boolean  local_strings;
char    *value;

    local_strings = TRUE;

    /* read the value of the variable */
    value = Tcl_GetVar(interp, GC_I_T61_VARIABLE, TCL_GLOBAL_ONLY);

    if (value != NULL)
    {
	/* if set to TRUE */
	if (strcmp(value, "TRUE") == 0)
	{
	    local_strings = FALSE;
	}
    }

    return(local_strings);
}
