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
 * $Log: gctread.c,v $
 * Revision 1.1.4.2  1996/02/18  18:19:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:53  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:29  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:50  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:48  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:14  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:55:29  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:36:34  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:59  keutel
 * 	creation
 * 	[1994/03/21  12:56:50  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctread.c					   */
/* Description : This file contains all the modules for the 	   */
/*		 directory read operation.			   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctread.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:19:03 $";
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
#include <gctmoveto.h>

/* Import the necessary class identifiers */
OM_IMPORT(DS_C_ENTRY_INFO_SELECTION)

/*-- Static Function(s) -------------------------------------------*/
static void gc_t__free_read_entry_selection(
    OM_object selection); /* IN - The object to be freed */

static signed16 gc_t__perform_read(
    OM_private_object  session,        /* IN  - The session object           */
    OM_private_object  context,        /* IN  - The context object           */
    OM_private_object  name,           /* IN  - The base DN                  */
    OM_object          entry_selection,/* IN  - Requested Info               */
    boolean            abbrv_result,   /* IN  - Indicates abbreviated result */
    boolean            local_strings,  /* IN  - To specify local strings     */
    gc_t_vector      **out_vect);      /* OUT - Result or the error message  */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_read					   */
/* Description   : This function is used to perform a directory	   */
/*		   read operation.				   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_read(
    char         **in_vect,       /* IN  - Array of input vectors          */
    signed16       vect_count,    /* IN  - Count of input vectors          */
    boolean        abbrv_result,  /* IN  - To indicated abbreviated result */
    boolean        local_strings, /* IN  - To specify local strings        */
    gc_t_vector  **out_vect)      /* OUT - The result or the error message */
{
OM_private_object  session;
OM_private_object  context;
OM_private_object  name;
OM_object	   entry_selection;
signed16	   attr_info_count, attr_type_count;
signed16  	   retval, i, k;
boolean	   	   is_present;
char		  *name_string;

    if (out_vect == NULL || vect_count < 0)
	return(GC_T_INVALID_PARAMETER);

    i = 0;
    *out_vect = NULL;
    attr_type_count = 0;
    entry_selection = NULL;
    name_string = NULL;
    name = NULL;

    /* get the session object */
    if ((session = gc_t_get_bound_session()) == NULL)
    {
	retval = GC_T_BIND_NOT_MADE;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, out_vect);
    }
    else
    {
	/* validate the input arguments and get the count of the */
	/* attribute information arguments.			 */
	retval = gc_t_check_args(GC_T_READ, in_vect, vect_count, 
				 &attr_info_count, out_vect);
	if (retval == GC_SUCCESS)
	{
	    /* get the context object */
	    context = gc_t_get_context();

	    /* Allocate the memory for the descriptors.                */
	    /* 4 more are required - one each for the class identifier,*/
	    /* DS_ALL_ATTRIBUTES, DS_INFO_TYPE and the NULL descriptor */
	    entry_selection = (OM_descriptor *)
			      malloc(sizeof(OM_descriptor) *
				     (attr_info_count + 4));
	    if (entry_selection == NULL)
	    {
		retval = GC_T_MEMORY_INSUFFICIENT;
		/* create the error vector */
		gc_t_create_gct_error_vector(retval, NULL, out_vect);
	    }
	    else
	    {
		/* fill the first descriptor with DS_C_ENTRY_INFO_SELECTION */
		omX_fill_oid(OM_CLASS, DS_C_ENTRY_INFO_SELECTION, 
			     &entry_selection[0]);

		/* initialize the remaining to NULL descriptors */
		for (k = 1; k < attr_info_count + 4; ++k)
		{
		    GC_T_FILL_OMD_NULL(&entry_selection[k]);
		}

		i = 0;

		/* check if the object name is specified in the first */
		/* input vector.				      */
		if (i < vect_count && in_vect[i] != NULL && 
		    in_vect[0][0] != '-')
		{
		    name_string = in_vect[0];
		    ++i;
		}

		k = 1;

		for (; i < vect_count && in_vect[i] != NULL; ++i)
		{
		    /* if attribute info switch specified */
		    gc_t_check_switch(GC_T_READ, in_vect[i], 
				      GC_T_ATTR_INFO_SWITCH,
				      &is_present, out_vect);

		    if (is_present == TRUE)
		    {
			/* skip past the switch */
			++i;

			/* collect all the attribute information */
			for (; i < vect_count && in_vect[i] != NULL; ++i, ++k)
			{
			    if (in_vect[i][0] == '-')
			    {
				--i;
				break;
			    }

			    /* convert abbreviation and fill it in descriptor */
			    retval = gc_t_fill_with_objid(in_vect[i],
							 DS_ATTRIBUTES_SELECTED,
							  &entry_selection[k], 
							  out_vect);
			    if (retval != GC_SUCCESS)
				break;
			}

			if (retval != GC_SUCCESS)
			    break;

			continue;
		    }

		    /* if attribute type only switch specified */
		    gc_t_check_switch(GC_T_READ, in_vect[i], 
				      GC_T_ATTR_TYPE_SWITCH,
				      &is_present, out_vect);

		    if (is_present == TRUE)
		    {
			++attr_type_count;
			continue;
		    }
		}
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* fill the descriptor with DS_ALL_ATTRIBUTES */
	if (attr_info_count == 0 || attr_type_count > 0)
	{
	    omX_fill(DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_TRUE, 0, 
		     &entry_selection[k]);
	}
	else
	{
	    omX_fill(DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE, 0, 
		     &entry_selection[k]);
	}
	++k;

	/* fill the descriptor with DS_INFO_TYPE */
	if (attr_type_count > 0)
	{
	    omX_fill(DS_INFO_TYPE, OM_S_ENUMERATION, DS_TYPES_ONLY, 0, 
		     &entry_selection[k]);
	}
	else
	{
	    omX_fill(DS_INFO_TYPE, OM_S_ENUMERATION, DS_TYPES_AND_VALUES, 0, 
		     &entry_selection[k]);
	}
	++k;

	/* convert the string to name */
	retval = gc_t_string_to_name(name_string, local_strings, &name, 
				     out_vect);

	if (retval == GC_SUCCESS)
	{
	    /* perform the read operation and process the result */
	    retval = gc_t__perform_read(session, context, name, entry_selection,
					abbrv_result, local_strings, out_vect);
	}
    }

    /* now free the created objects */
    gc_t__free_read_entry_selection(entry_selection);

    if (name != NULL)
	om_delete(name);

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__perform_read				   */
/* Description   : This function is used to perform a directory	   */
/*		   read operation and process the results.	   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__perform_read(
    OM_private_object  session,        /* IN  - The session object           */
    OM_private_object  context,        /* IN  - The context object           */
    OM_private_object  name,           /* IN  - The base DN                  */
    OM_object          entry_selection,/* IN  - Requested Info               */
    boolean            abbrv_result,   /* IN  - Indicates abbreviated result */
    boolean            local_strings,  /* IN  - To specify local strings     */
    gc_t_vector      **out_vect)       /* OUT - Result or the error message  */
{
OM_private_object  result;
OM_value_position  total_number;
OM_return_code     om_ret_code;
OM_public_object   entry_info;
OM_boolean	   request_local_strings;
OM_type            included_types[] =  { DS_ENTRY, OM_NO_MORE_TYPES };
OM_sint            invoke_id_return;
DS_status          ds_status;
gc_t_vector       *result_vector;
gc_t_vector       *err_vector;
unsigned32	   vect_count, i;
signed16	   retval;

    retval = GC_SUCCESS;
    entry_info = NULL;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    /* perform read operation */
    ds_status = ds_read(session, context, name, entry_selection, &result,
			&invoke_id_return);

    if (ds_status != DS_SUCCESS)
    {
        /* map the XDS error to translator defined error and */
        /* create the error vector.                          */
	retval = gc_t_map_xds_error(ds_status, local_strings, &err_vector);

	/* free the error object */
	om_delete(ds_status);
    }
    else
    {
	om_ret_code = om_get(result, OM_EXCLUDE_ALL_BUT_THESE_TYPES,
			     included_types, request_local_strings, 0, 0,
			     &entry_info, &total_number);

	/* free the result */
	om_delete(result);

	/* check if om_get has returned some object */
	if (om_ret_code == OM_SUCCESS && total_number <= 0)
        {
	    om_ret_code = OM_NO_SUCH_OBJECT;
	}

	if (om_ret_code != OM_SUCCESS)
	{
	    /* map the XOM error to the translator defined */
	    /* error and create the error string.          */
	    retval = gc_t_map_xom_error(om_ret_code, &err_vector);
	}
	else
	{
	    /* count the number of DNs and attribute type/values returned */
	    gc_t_get_entry_info_vect_count(entry_info, abbrv_result,
					   &vect_count);

	    result_vector = (gc_t_vector *) malloc(sizeof(gc_t_vector) *
						   vect_count);
	    if (result_vector == NULL)
	    {
		retval = GC_T_MEMORY_INSUFFICIENT;
		/* create the error vector */
		gc_t_create_gct_error_vector(retval, NULL, &err_vector);
	    }
	    else
	    {
		/* initialize the complete result vector with GC_T_EOL */
		for (i = 0; i < vect_count; ++i)
		    result_vector[i].type = GC_T_EOL;

		/* process the read result */
		retval = gc_t_process_entry_info(entry_info, abbrv_result,
					         local_strings, NULL, 
						 &result_vector, &err_vector);

		if (retval != GC_SUCCESS)
		{
		    /* free the allocated areas */
		    gc_t_free_result(result_vector);
		}
	    }
	}
    }

    /* delete the extracted entry information */
    if (entry_info != NULL)
    {
	om_delete(entry_info);
    }

    if (retval == GC_SUCCESS)
    {
	*out_vect = result_vector;
    }
    else
    {
	*out_vect = err_vector;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__free_read_entry_selection		   */
/* Description   : This function is used to free the entry	   */
/*		   selection object.				   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_t__free_read_entry_selection(
    OM_object selection)  /* IN - The object to be freed */
{
signed16  i;

    if (selection == NULL)
	return;

    /* skip throught the first class descriptor */
    for (i = 1; selection[i].type != OM_NO_MORE_TYPES; ++i)
    {
	/* free the attribute object identifiers */
	if (selection[i].type == DS_ATTRIBUTES_SELECTED)
	{
	    free(selection[i].value.string.elements);
	}
    }

    free(selection);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
