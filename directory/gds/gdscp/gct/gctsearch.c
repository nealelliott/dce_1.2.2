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
 * $Log: gctsearch.c,v $
 * Revision 1.1.4.2  1996/02/18  18:19:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:55  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:32  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:52  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:50  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:17  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:55:38  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:36:42  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:03  keutel
 * 	creation
 * 	[1994/03/21  12:56:58  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctsearch.c					   */
/* Description : This file contains all the modules for the 	   */
/*		 directory search operation.			   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctsearch.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:19:06 $";
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

#include <xoi.h>

#include <gdscp.h>
#include <gctconv.h>
#include <gctstruct.h>
#include <gcterr.h>
#include <gctutil.h>
#include <gctargs.h>
#include <gctbind.h>
#include <gctmoddsc.h>
#include <gctmoveto.h>

/* Import the necessary class identifiers */
OM_IMPORT(DS_C_ENTRY_INFO_SELECTION)

/*-- Local  Define(s)   -------------------------------------------*/

/*-- Static Variable(s) -------------------------------------------*/

/*-- Static Function(s) -------------------------------------------*/
static signed16 gc_t__perform_search(
    OM_private_object  session,	       /* IN  - The session object	    */
    OM_private_object  context,	       /* IN  - The context object	    */
    OM_private_object  name,	       /* IN  - The base DN		    */
    OM_sint	       subset,	       /* IN  - Scope of search   	    */
    OM_private_object  filter,	       /* IN  - The filter object	    */
    OM_boolean	       search_aliases, /* IN  - Search Aliases		    */
    OM_object	       entry_selection,/* IN  - Requested Info		    */
    boolean            abbrv_result,   /* IN  - Indicates abbreviated result*/
    boolean            local_strings,  /* IN  - To specify local strings    */
    gc_t_vector      **out_vect);      /* OUT - Result or the error message */

static void gc_t__free_search_entry_selection(
    OM_object selection); /* IN - The object to be freed */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_search					   */
/* Description   : This function is used to perform a directory	   */
/*		   search operation.				   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_search(
    char         **in_vect,       /* IN  - Array of input vectors          */
    signed16       vect_count,    /* IN  - Count of input vectors          */
    boolean        abbrv_result,  /* IN  - To indicated abbreviated result */
    boolean        local_strings, /* IN  - To specify local strings        */
    gc_t_vector  **out_vect)      /* OUT - The result or the error message */
{
OM_private_object  session;
OM_private_object  context;
OM_private_object  name, filter;
OM_boolean	   dereference_alias;
OM_object	   entry_selection;
OM_sint		   subset;
signed16	   attribute_info_count;
signed16   	   retval;
signed16   	   i, k;
boolean  	   type_only;
boolean  	   value_also;
boolean  	   request_all;
boolean  	   is_present;
char		  *name_string;

    if (out_vect == NULL || vect_count < 0)
	return(GC_T_INVALID_PARAMETER);

    i = 0;
    *out_vect = NULL;
    name_string = NULL;
    type_only = value_also = request_all = FALSE;
    dereference_alias = OM_TRUE;
    entry_selection = NULL;
    name = filter = NULL;
    subset = -1;
    retval = GC_SUCCESS;

    if (vect_count > 0 && in_vect == NULL)
    {
	retval = GC_T_INVALID_PARAMETER;
    }
    else
    {
	/* get the session object */
	if ((session = gc_t_get_bound_session()) == NULL)
	{
	    retval = GC_T_BIND_NOT_MADE;
	}
	else
	{
	    /* get the context object */
	    context = gc_t_get_context();

	    attribute_info_count = 0;

	    /* count the number of attribute information arguments supplied */
	    for (; i < vect_count && in_vect[i] != NULL; ++i)
	    {
		retval = gc_t_check_switch(GC_T_SEARCH, in_vect[i], 
					   GC_T_ATTR_INFO_SWITCH,
					   &is_present, out_vect);

		if (retval != GC_SUCCESS)
		{
		    break;
		}

		if (is_present == TRUE)
		{
		    /* skip past the attribute information switch */
		    ++i;

		    /* now run through the attribute information arguments */
		    for (; i < vect_count && in_vect[i] != NULL; ++i)
		    {
			if (in_vect[i][0] == '-')
			    break;

			++attribute_info_count;
		    }
		}
	    }

	    if (retval == GC_SUCCESS)
	    {
		/* Allocate the memory for the descriptors.                */
		/* 4 more are required - one each for the class identifier,*/
		/* DS_ALL_ATTRIBUTES, DS_INFO_TYPE and the NULL descriptor */ 
		entry_selection = (OM_descriptor *) 
				  malloc(sizeof(OM_descriptor) * 
					 (attribute_info_count + 4));
		if (entry_selection == NULL)
		{
		    retval = GC_T_MEMORY_INSUFFICIENT;
		}
		else
		{
		    /* fill first descriptor with DS_C_ENTRY_INFO_SELECTION */
		    omX_fill_oid(OM_CLASS, DS_C_ENTRY_INFO_SELECTION, 
				 &entry_selection[0]);

		    /* initialize the remaining to NULL descriptors */
		    for (k = 1; k < attribute_info_count + 4; ++k)
		    {
			GC_T_FILL_OMD_NULL(&entry_selection[k]);
		    } 
		}
	    }
	}
    }

    if (retval != GC_SUCCESS)
    {
	/* create the error vector */
	if (*out_vect == NULL)
	{
	    gc_t_create_gct_error_vector(retval, NULL, out_vect);
	}
    }
    else
    {
	i = 0;

	/* check if the object name is specified in the first input vector */
	if (i < vect_count && in_vect[0] != NULL && in_vect[0][0] != '-')
	{
	    name_string = in_vect[0];
	    ++i;
	}

	k = 1;

	for (; i < vect_count && in_vect[i] != NULL; ++i)
	{
	    /* if attribute info switch specified */
	    retval = gc_t_check_switch(GC_T_SEARCH, in_vect[i], 
				       GC_T_ATTR_INFO_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if attribute type only or all attribute values switch */
		/* or attribute info switch also specified 		 */
		if (type_only == TRUE || request_all == TRUE || 
		    value_also == TRUE)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		/* skip past the attribute info switch */
		++i;

		/* check if it is followed by the attribute abbreviation */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_ATTRIBUTE_INFO;
		    break;
		}

		value_also = TRUE;

		/* now run through the attribute information arguments */
		for (; i < vect_count && in_vect[i] != NULL; ++i, ++k)
		{
		    if (in_vect[i][0] == '-')
		    {
			--i;
			break;
		    }

		    /* convert the abbreviation and fill it in the descriptor */
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

	    /* if base object subset switch specified */
	    retval = gc_t_check_switch(GC_T_SEARCH, in_vect[i], 
				       GC_T_BASE_OBJ_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if any other subset switch also specified */
		if (subset != -1)
		{
		    if (subset != DS_BASE_OBJECT)
		    {
			retval = GC_T_CONFLICTING_OPTION;
			break;
		    }
		}
		subset = DS_BASE_OBJECT;
		continue;
	    }

	    /* if single level subset switch specified */
	    retval = gc_t_check_switch(GC_T_SEARCH, in_vect[i], 
				       GC_T_SING_LEVEL_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if any other subset switch also specified */
		if (subset != -1)
		{
		    if (subset != DS_ONE_LEVEL)
		    {
			retval = GC_T_CONFLICTING_OPTION;
			break;
		    }
		}
		subset = DS_ONE_LEVEL;
		continue;
	    }

	    /* if whole subtree subset switch specified */
	    retval = gc_t_check_switch(GC_T_SEARCH, in_vect[i], 
				       GC_T_SUBTREE_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if any other subset switch also specified */
		if (subset != -1)
		{
		    if (subset != DS_WHOLE_SUBTREE)
		    {
			retval = GC_T_CONFLICTING_OPTION;
			break;
		    }
		}
		subset = DS_WHOLE_SUBTREE;
		continue;
	    }

	    /* if filter switch specified */
	    retval = gc_t_check_switch(GC_T_SEARCH, in_vect[i], 
				       GC_T_FILTER_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if specified more than once */
		if (filter != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by the filter */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_FILTER;
		    break;
		}

		retval = gc_t_string_to_filter(in_vect[i], local_strings, 
					       &filter, out_vect);
		if (retval != GC_SUCCESS)
		    break;

		continue;
	    }

	    /* if request all attribute switch specified */
	    retval = gc_t_check_switch(GC_T_SEARCH, in_vect[i], 
				       GC_T_REQUEST_ALL_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if attribute info switch or attribute type only switch */
		/* also specified 					  */
		if (value_also == TRUE || type_only == TRUE)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}
		/* set the flag. The descriptor will be filled later */
		request_all = TRUE;
		continue;
	    }

	    /* if attribute type only switch specified */
	    retval = gc_t_check_switch(GC_T_SEARCH, in_vect[i], 
				       GC_T_ATTR_TYPE_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if attribute value switch or all attribute values switch */
		/* also specified 					    */
		if (value_also == TRUE || request_all == TRUE)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}
		/* set the flag. The descriptor will be filled later */
		type_only = TRUE;
		continue;
	    }

	    /* if no aliases switch specified */
	    retval = gc_t_check_switch(GC_T_SEARCH, in_vect[i], 
				       GC_T_NOALIASES_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		dereference_alias = OM_FALSE;
		continue;
	    }

	    /* must be an unknown switch or parameter */
	    if (in_vect[i][0] == '-')
		retval = GC_T_UNKNOWN_OPTION;
	    else
		retval = GC_T_UNKNOWN_ARGUMENT;

	    break;
	}
    }

    if (retval == GC_SUCCESS && subset == -1)
    {
	retval = GC_T_MISSING_SCOPE_OF_SEARCH;
    }

    if (retval == GC_SUCCESS)
    {
	/* fill the descriptor with DS_ALL_ATTRIBUTES */
	if (request_all == TRUE || type_only == TRUE)
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
	if (type_only == TRUE)
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
	    /* perform the search operation and process the result */
	    retval = gc_t__perform_search(session, context, name, subset, 
					  filter, dereference_alias, 
					  entry_selection, abbrv_result, 
					  local_strings, out_vect);
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

    /* now free the created objects */
    gc_t__free_search_entry_selection(entry_selection);

    if (name != NULL)
	om_delete(name);

    if (filter != NULL)
	om_delete(filter);

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__perform_search				   */
/* Description   : This function is used to perform a directory	   */
/*		   search operation and process the results.	   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__perform_search(
    OM_private_object  session,	       /* IN  - The session object	    */
    OM_private_object  context,	       /* IN  - The context object	    */
    OM_private_object  name,	       /* IN  - The base DN		    */
    OM_sint	       subset,	       /* IN  - Scope of search   	    */
    OM_private_object  filter,	       /* IN  - The filter object	    */
    OM_boolean	       search_aliases, /* IN  - Search Aliases		    */
    OM_object	       entry_selection,/* IN  - Requested Info		    */
    boolean            abbrv_result,   /* IN  - Indicates abbreviated result*/
    boolean            local_strings,  /* IN  - To specify local strings    */
    gc_t_vector      **out_vect)       /* OUT - Result or the error message */
{
OM_private_object  result;
OM_value_position  total_number;
OM_return_code	   om_ret_code;
OM_public_object   entry_info;
OM_public_object   partial_qual;
OM_boolean	   request_local_strings;
OM_type		   navigation_path[] = { DS_SEARCH_INFO, OM_NO_MORE_TYPES };
OM_type		   included_types[] =  { OM_NO_MORE_TYPES, OM_NO_MORE_TYPES };
OM_sint		   invoke_id_return;
DS_status  	   ds_status;
gc_t_vector	  *result_vector;
gc_t_vector	  *err_vector;
unsigned32         result_vect_count;
unsigned32  	   vect_count, i;
unsigned32  	   vectors_used;
unsigned16	   partial_vect_count;
signed16           retval;

    retval = GC_SUCCESS;
    entry_info = NULL;
    partial_qual = NULL;
    result_vector = NULL;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    if (filter != NULL)
    {
	/* initiate search */
	ds_status = ds_search(session, context, name, subset, filter,
			      search_aliases, entry_selection, &result,
			      &invoke_id_return);
    }
    else
    {
	/* initiate search with DS_NO_FILTER */
	ds_status = ds_search(session, context, name, subset, DS_NO_FILTER,
			      search_aliases, entry_selection, &result,
			      &invoke_id_return);
    }

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
	partial_vect_count = 0;

	/* check and extract DS_C_PARTIAL_OUTCOME_QUAL, if present.    */
	included_types[0] = DS_PARTIAL_OUTCOME_QUAL;

	om_ret_code = omX_extract(result, navigation_path,
				  OM_EXCLUDE_ALL_BUT_THESE_TYPES,
				  included_types, request_local_strings, 0, 0,
				  &partial_qual, &total_number);

	/* Note that if the return code is not OM_SUCCESS, it is not  */
	/* processed, since it concerns the partial outcome qualifier.*/
	/* Anyway, the object will be tested again during a second    */
	/* call to omX_extract.					      */
	if (om_ret_code == OM_SUCCESS)
	{
	    if (total_number > 0)
	    {
		/* count the number of vectors required for the partial */
		/* outcome qualifier.				        */
		gc_t_get_partial_vect_count(partial_qual->value.object.object,
					    &partial_vect_count);
	    }
	}

	/* now extract the DS_ENTRIES object */
	included_types[0] = DS_ENTRIES;
	om_ret_code = omX_extract(result, navigation_path,
				  OM_EXCLUDE_ALL_BUT_THESE_TYPES,
				  included_types, request_local_strings, 0, 0,
				  &entry_info, &total_number);

	if (om_ret_code != OM_SUCCESS)
	{
	    /* if no entries are present */
	    if (om_ret_code == OM_NOT_PRESENT)
	    {
		/* if no partial outcome qualifier present */
		if (partial_vect_count == 0)
		{
		    retval = GC_T_ENTRY_NOT_FOUND;
		    /* create the error vector */
		    gc_t_create_gct_error_vector(retval, NULL, &err_vector);
		}
		else
		{
		    /* create an error vector indicating the reason for */
		    /* the partial outcome qualifier.	                */
		    retval = gc_t_create_partial_qual_error(
					      partial_qual->value.object.object,
					      local_strings, &err_vector);
		    if (retval == GC_SUCCESS)
		    {
			/* return the translator defined error */
			retval = GC_T_OPERATION_INCOMPLETE;
		    }
		}
	    }
	    else
	    {
		/* map the XOM error to the translator defined */
		/* error and create the error string.          */
		retval = gc_t_map_xom_error(om_ret_code, &err_vector);
	    }
	}

	/* free the result */
	om_delete(result);
    }

    if (retval == GC_SUCCESS)
    {
	/* count the number of DNs and attribute type/values returned */
	gc_t_get_entry_info_vect_count(entry_info, abbrv_result, 
				       &result_vect_count);

	vect_count = result_vect_count + partial_vect_count;

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

	    /* process the search result */
	    retval = gc_t_process_entry_info(entry_info, abbrv_result,
					     local_strings, &vectors_used, 
					     &result_vector, &err_vector);
	    if (retval == GC_SUCCESS)
	    {
		/* if a partial outcome qualifier is present */
		if (partial_vect_count != 0)
		{
		    /* process the partial outcome qualifier object */
		    retval = gc_t_process_partial_qual_object(
					      partial_qual->value.object.object,
					      local_strings,
					      &result_vector[vectors_used - 1],
					      &err_vector);
		}
	    }
	}
    }

    /* delete the extracted objects */
    if (entry_info != NULL)
    {
	om_delete(entry_info);
    }

    if (partial_qual != NULL)
    {
	om_delete(partial_qual);
    }

    if (retval == GC_SUCCESS)
    {
	*out_vect = result_vector;
    }
    else
    {
	*out_vect = err_vector;

	/* free the allocated areas */
	if (result_vector != NULL)
	{
	    gc_t_free_result(result_vector);
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__free_search_entry_selection		   */
/* Description   : This function is used to free the entry	   */
/*		   selection object.				   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_t__free_search_entry_selection(
    OM_object selection)  /* IN - The object to be freed */
{
unsigned16  i;

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
