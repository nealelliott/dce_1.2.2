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
 * $Log: gctmodrdn.c,v $
 * Revision 1.1.4.2  1996/02/18  18:19:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:26  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:49  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:46  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:12  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:55:20  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:36:26  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:55  keutel
 * 	creation
 * 	[1994/03/21  12:56:43  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctmodrdn.c					   */
/* Description : This file contains all the modules for the        */
/*		 directory modify RDN operation.		   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctmodrdn.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:19:00 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <gds.h>

#include <xom.h>
#include <xds.h>

#include <xomext.h>

#include <gdscp.h>
#include <gctstruct.h>
#include <gctconv.h>
#include <gcterr.h>
#include <gctutil.h>
#include <gctargs.h>
#include <gctbind.h>
#include <gctmoddsc.h>

/* import the necessary Object Identifier constants */ 
OM_IMPORT(DS_C_DS_RDN)

/*-- Static Function(s) -------------------------------------------*/


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_modrdn					   */
/* Description   : This function is used to perform a directory	   */
/*		   modify RDN operation.			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_modrdn(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean       local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect)      /* OUT - NULL (success) or error message */
{
OM_private_object  session;
OM_private_object  context;
OM_private_object  name;
OM_private_object  rdn_object;
OM_return_code	   om_ret_code;
OM_workspace	   workspace;
OM_boolean	   request_local_strings;
OM_integer	   error_position, error_type;
OM_string	   rdn_omstring;
OM_sint		   invoke_id_return;
DS_status	   ds_status;
signed16  	   retval;
signed16  	   i;
boolean	   	   is_present;
char		  *name_string;
char		  *rdn_string;

    if (out_vect == NULL || vect_count < 0)
	return(GC_T_INVALID_PARAMETER);

    i = 0;
    *out_vect = NULL;
    name_string = rdn_string = NULL;
    name = NULL;
    rdn_object = NULL;
    retval =  GC_SUCCESS;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    if (vect_count > 0 && in_vect == NULL)
    {
	retval = GC_T_INVALID_PARAMETER;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, out_vect);
    }
    else
    {
	/* get the session object */
	if ((session = gc_t_get_bound_session()) == NULL)
	{
	    retval = GC_T_BIND_NOT_MADE;
	    /* create the error vector */
	    gc_t_create_gct_error_vector(retval, NULL, out_vect);
	}
	else
	{
	    i = 0;

	    /* check if object name is specified in the first input vector */
	    if (i < vect_count && in_vect[0] != NULL && in_vect[0][0] != '-')
	    {
		name_string = in_vect[0];
		++i;
	    }

	    for (; i < vect_count && in_vect[0] != NULL; ++i)
	    {
		/* if modify rdn switch specified */
		retval = gc_t_check_switch(GC_T_MODIFY, in_vect[i], 
					   GC_T_MOD_RDN_SWITCH,
					   &is_present, out_vect);

		if (retval != GC_SUCCESS)
		{
		    break;
		}

		if (is_present == TRUE)
		{
		    /* check if it is specified more than once */
		    if (rdn_string != NULL)
		    {
			retval = GC_T_CONFLICTING_OPTION;
			break;
		    }

		    /* skip past the switch */
		    ++i;

		    /* check if it is followed by the new rdn string */
		    if (i >= vect_count || in_vect[i] == NULL ||
			in_vect[i][0] == '-')
		    {
			retval = GC_T_MISSING_ATTRIBUTE_INFO;
			break;
		    }

		    rdn_string = in_vect[i];
		    continue;
		}

		/* must be an unknown switch or argument */
		if (in_vect[i][0] == '-')
		{
		    retval = GC_T_UNKNOWN_OPTION;
		}
		else
		{
		    retval = GC_T_UNKNOWN_ARGUMENT;
		}
		break;
	    }
	}
    }

    if (retval == GC_SUCCESS && rdn_string == NULL)
    {
	retval = GC_T_MISSING_ATTRIBUTE_INFO;
    }

    if (retval == GC_SUCCESS)
    {
	/* get the context object */
	context = gc_t_get_context();

	/* get the workspace */
	workspace = gc_t_get_workspace();

	/* convert the string to name */
	retval = gc_t_string_to_name(name_string, local_strings, &name, 
				     out_vect);

	if (retval == GC_SUCCESS)
	{
	    rdn_omstring.elements = (void *) rdn_string;
	    rdn_omstring.length = strlen(rdn_string);

	    /* convert the attribute info string to RDN object */
	    om_ret_code = omX_string_to_object(workspace, &rdn_omstring, 
					       DS_C_DS_RDN, 
					       request_local_strings, 
					       &rdn_object, &error_position, 
					       &error_type);
	    if (om_ret_code != OM_SUCCESS)
	    {
		if (om_ret_code == OM_WRONG_VALUE_MAKEUP && 
		    error_type != OMX_SUCCESS)
		{
		    /* map the error to the translator defined error and */
		    /* create error vector 				 */
		    retval = gc_t_map_xos_error(error_type, rdn_string,
						error_position, out_vect);
		}
		else
		{
		    /* map the XOM error to the translator defined */
		    /* error and create the error string.          */
		    retval = gc_t_map_xom_error(om_ret_code, out_vect);
		}
	    }
	    else
	    {
		/* perform the modify RDN operation */
		ds_status = ds_modify_rdn(session, context, name, rdn_object, 
					  OM_TRUE, &invoke_id_return);
		if (ds_status != DS_SUCCESS)
		{
        	    /* map the XDS error to translator defined error and */
        	    /* create the error vector.                          */
		    retval = gc_t_map_xds_error(ds_status, local_strings,
						out_vect);

		    /* free the error object */
		    om_delete(ds_status);
		}
	    }
	}
    }
    else
    {
	if (*out_vect == NULL)
	{
	    /* create the error vector */
	    if (i >= vect_count)
	    {
		gc_t_create_gct_arg_error_vector(retval, NULL, out_vect);
	    }
	    else
	    {
		gc_t_create_gct_arg_error_vector(retval, in_vect[i], out_vect);
	    }
	}
    }

    /* now delete the created objects */
    if (name != NULL)
	om_delete(name);

    if (rdn_object != NULL)
	om_delete(rdn_object);

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
