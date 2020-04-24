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
 * $Log: gctcompare.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:16  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:41  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:35  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:04  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:54:32  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:35:54  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:42  keutel
 * 	creation
 * 	[1994/03/21  12:56:11  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctcompare.c					   */
/* Description : This file contains all the modules for the        */
/*		 directory compare operation.			   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctcompare.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:50 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <gds.h>

#include <xom.h>
#include <xds.h>

#include <gdscp.h>
#include <gctstruct.h>
#include <gctconv.h>
#include <gctutil.h>
#include <gctargs.h>
#include <gcterr.h>
#include <gctbind.h>
#include <gctmoddsc.h>

/* import the necessary Object Identifier constants */
OM_IMPORT(DS_C_AVA)
OM_IMPORT(DS_C_ATTRIBUTE)

/*-- Static Function(s) -------------------------------------------*/
static signed16 gc_t__perform_compare(
    OM_private_object   session,       /* IN  - The session object	    */
    OM_private_object   context,       /* IN  - The context object	    */
    OM_private_object   name,          /* IN  - The name object		    */
    OM_private_object   ava,           /* IN  - The ava object		    */
    boolean             local_strings, /* IN  - To specify local strings    */
    gc_t_vector       **out_vect);     /* OUT - Result or the error message */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_compare					   */
/* Description   : This function is used to perform a directory    */
/*		   compare operation.				   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_compare(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean       local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect)      /* OUT - The result or the error message */
{
OM_private_object  session;
OM_private_object  context;
OM_private_object  name;
OM_private_object  attr_object;
OM_private_object  ava_object;
OM_return_code	   om_ret_code;
OM_workspace	   workspace;
char		  *name_string;
signed16  	   retval;
signed16	   i;

    if (out_vect == NULL || vect_count < 0)
	return(GC_T_INVALID_PARAMETER);

    i = 0;
    name = attr_object = ava_object = NULL;
    name_string = NULL;
    *out_vect = NULL;
    retval = GC_SUCCESS;

    /* get the session object */
    if ((session = gc_t_get_bound_session()) == NULL)
    {
	retval = GC_T_BIND_NOT_MADE;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, out_vect);
    }
    else
    {
	/* validate the input switches */
	retval = gc_t_check_args(GC_T_COMPARE, in_vect, vect_count, NULL, 
				 out_vect);
	if (retval == GC_SUCCESS)
	{
	    /* get the context object */
	    context = gc_t_get_context();

	    /* get the workspace */
	    workspace = gc_t_get_workspace();

	    i = 0;

	    /* check if object name is specified in the first argument */
	    if (in_vect[0][0] != '-')
	    {
		name_string = in_vect[0];
		++i;
	    }

	    /* convert the string to name */
	    retval = gc_t_string_to_name(name_string, local_strings, &name, 
					 out_vect);

	    if (retval == GC_SUCCESS)
	    {
		/* now skip past the attribute info switch */
		++i;

		/* convert the attribute info string to DS_C_ATTRIBUTE object */
		retval = gc_t_string_to_object(in_vect[i], DS_C_ATTRIBUTE, 
					       local_strings, &attr_object, 
					       out_vect);
		if (retval == GC_SUCCESS)
		{
		    /* create a DS_C_AVA object */
		    om_ret_code = om_create(DS_C_AVA, OM_FALSE, workspace,
					    &ava_object);
		    if (om_ret_code == OM_SUCCESS)
		    {
			/* for the compare operation, a DS_C_AVA object  */
			/* is required. So put the DS_C_ATTRIBUTE object */
			/* in the DS_C_AVA object.		  	 */
			om_ret_code = om_put(ava_object, OM_REPLACE_ALL, 
					     attr_object, OM_NO_MORE_TYPES, 
					     0, 0);

			if (om_ret_code == OM_SUCCESS)
			{
			    /* now perform the compare operation */
			    retval = gc_t__perform_compare(session, context,
							   name, ava_object,
							   local_strings,
							   out_vect);
			}
			else
			{
			    /* map the XOM error to the translator defined */
			    /* error and create the error string.	   */
			    retval = gc_t_map_xom_error(om_ret_code, out_vect);
			}
		    }
		    else
		    {
			/* map the XOM error to the translator defined */
			/* error and create the error string.          */
			retval = gc_t_map_xom_error(om_ret_code, out_vect);
		    }
		}
	    }
	}
    }

    /* now delete the created objects */
    if (name != NULL)
	om_delete(name);

    if (ava_object != NULL)
	om_delete(ava_object);

    if  (attr_object != NULL)
	om_delete(attr_object);


    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__perform_compare			   */
/* Description   : This function is used to perform the directory  */
/*		   compare operation and create the result vector. */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__perform_compare(
    OM_private_object   session,       /* IN  - The session object	    */
    OM_private_object   context,       /* IN  - The context object	    */
    OM_private_object   name,          /* IN  - The name object		    */
    OM_private_object   ava,           /* IN  - The ava object		    */
    boolean             local_strings, /* IN  - To specify local strings    */
    gc_t_vector       **out_vect)      /* OUT - Result or the error message */
{
OM_private_object  result;
OM_value_position  total_number;
OM_return_code	   om_ret_code;
OM_object	   ds_matched;
OM_string	   result_string;
OM_sint	   	   invoke_id_return;
OM_type		   included_types[] = {DS_MATCHED, OM_NO_MORE_TYPES};
DS_status  	   ds_status;
gc_t_vector	  *error_vector;
gc_t_vector	  *result_vector;
signed16	   retval;

    ds_matched = NULL;
    retval = GC_SUCCESS;

    /* perform the compare operation */
    ds_status = ds_compare(session, context, name, ava, &result, 
			   &invoke_id_return);

    if (ds_status != DS_SUCCESS)
    {
        /* map the XDS error to translator defined error and */
        /* create the error vector.                          */
	retval = gc_t_map_xds_error(ds_status, local_strings, &error_vector);

	/* free the error object */
	om_delete(ds_status);
    }
    else
    {
	om_ret_code = om_get(result, OM_EXCLUDE_ALL_BUT_THESE_TYPES,
			     included_types, OM_TRUE, 0, 0, &ds_matched,
			     &total_number);
	/* free the result */
	om_delete(result);

	if (om_ret_code != OM_SUCCESS)
	{
	    /* map the XOM error to the translator defined */
	    /* error and create the error string.          */
	    retval = gc_t_map_xom_error(om_ret_code, &error_vector);
	}
	else
	{
	    /* check if om_get has returned some object */
	    if (total_number <= 0)
	    {
		retval = OM_NO_SUCH_OBJECT;
		/* create the error vector */
		gc_t_create_gct_error_vector(retval, NULL, &error_vector);
	    }
	    else
	    {
		/* Allocate the area for the result vector. An additional */
		/* vector is required for GC_T_EOL.			  */
		result_vector = (gc_t_vector *) malloc(sizeof(gc_t_vector) * 2);

		if (result_vector == NULL)
		{
		    retval = GC_T_MEMORY_INSUFFICIENT;
		    /* create the error vector */
		    gc_t_create_gct_error_vector(retval, NULL, &error_vector);
		}
		else
		{
		    if (ds_matched->value.boolean == OM_TRUE)
		    {
			result_string.elements = (void *) GC_T_TRUE_STR;
			result_string.length = strlen(GC_T_TRUE_STR);
		    }
		    else
		    {
			result_string.elements = (void *) GC_T_FALSE_STR;
			result_string.length = strlen(GC_T_FALSE_STR);
		    }

		    /* set the first vector to the result */
		    retval = gc_t_create_result_vector_data(result_vector, 0, 
							    GC_T_BOOLEAN,
							    result_string,
							 OM_S_PRINTABLE_STRING);
		    if (retval != GC_SUCCESS)
		    {
			free(result_vector);
			/* create the error vector */
			gc_t_create_gct_error_vector(retval, NULL, 
						     &error_vector);
		    }
		    else
		    {
			/* set the second vector to GC_T_EOL */
			result_vector[1].type = GC_T_EOL;
		    }
		}
	    }
	}
    }

    /* free the extracted object */
    if (ds_matched != NULL)
	om_delete(ds_matched);

    if (retval == GC_SUCCESS)
    {
	*out_vect = result_vector;
    }
    else
    {
	*out_vect = error_vector;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
