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
 * $Log: gctlist.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:21  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:45  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:41  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:08  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:54:56  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:36:15  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:48  keutel
 * 	creation
 * 	[1994/03/21  12:56:28  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctlist.c					   */
/* Description : This file contains all the modules for the        */
/*		 directory list operation.			   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctlist.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:55 $";
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
#include <gctutil.h>
#include <gctargs.h>
#include <gcterr.h>
#include <gctbind.h>
#include <gctmoddsc.h>

/*-- Static Function(s) -------------------------------------------*/
static signed16 gc_t__perform_list(
    OM_private_object   session,       /* IN  - The session object 	    */
    OM_private_object   context,       /* IN  - The context object 	    */
    OM_private_object   name,	       /* IN  - The DN object      	    */
    boolean             local_strings, /* IN  - To specify local strings    */
    gc_t_vector       **out_vect);     /* OUT - Result or the error message */

static signed16 gc_t__process_list_rdns(
    OM_object      name,          /* IN  - The Parent DN 		*/
    OM_object      rdn,	          /* IN  - The Child RDN		*/
    unsigned32     vect_index,    /* IN  - Index into the result vector */
    boolean        local_strings, /* IN  - To specify local strings     */
    gc_t_vector  **result_vect,   /* OUT - Contains the result	        */
    gc_t_vector  **error_vect);   /* OUT - Contains Errors    	        */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_list					   */
/* Description   : This function is used to perform a directory	   */
/*		   list operation				   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.			 	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_list(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean       local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect)      /* OUT - The result or the error message */
{
OM_private_object  session;
OM_private_object  context;
OM_private_object  name;
signed16  	   retval;
char		  *name_string;

    if (out_vect == NULL || vect_count < 0)
	return(GC_T_INVALID_PARAMETER);

    *out_vect = NULL;
    retval = GC_SUCCESS;
    name = NULL;
    name_string = NULL;

    /* get the session object */
    if ((session = gc_t_get_bound_session()) == NULL)
    {
	retval = GC_T_BIND_NOT_MADE;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, out_vect);
    }
    else
    {
	/* validate the input arguments. */
	retval = gc_t_check_args(GC_T_LIST, in_vect, vect_count, NULL, 
				 out_vect);
	if (retval  == GC_SUCCESS)
	{
	    /* get the context object */
	    context = gc_t_get_context();

	    if (vect_count > 0)
	    {
		/* extract the name string from the first argument vector */
		name_string = in_vect[0];
	    }

	    /* convert the object name string to DS_C_DS_DN object */
	    retval = gc_t_string_to_name(name_string, local_strings, &name, 
					 out_vect);

	    if (retval == GC_SUCCESS)
	    {
		/* perform the list operation */
		retval = gc_t__perform_list(session, context, name, 
					    local_strings, out_vect);
	    }
	}
    }

    /* now free the created object */
    if (name != NULL)
	om_delete(name);

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__perform_list				   */
/* Description   : This function performs the directory	list       */
/*		   operation and creates the result vector.	   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.			 	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__perform_list(
    OM_private_object   session,       /* IN  - The session object 	    */
    OM_private_object   context,       /* IN  - The context object 	    */
    OM_private_object   name,	       /* IN  - The DN object      	    */
    boolean             local_strings, /* IN  - To specify local strings    */
    gc_t_vector       **out_vect)      /* OUT - Result or the error message */
{
OM_return_code     om_ret_code;
OM_private_object  result;
OM_public_object   list_info;
OM_public_object   list_info_det;
OM_public_object   returned_name;
OM_public_object   list_info_item;
OM_public_object   partial_qual_obj;
OM_public_object   rdn;
OM_value_position  total_number;
OM_boolean	   request_local_strings;
OM_type		   included_types[] = {DS_LIST_INFO, OM_NO_MORE_TYPES};
OM_sint		   invoke_id_return;
DS_status	   ds_status;
gc_t_vector	  *result_vector;
unsigned32         vect_index;
unsigned32	   vect_count;
unsigned32	   subord_count;
unsigned32	   i, j;
unsigned16         partial_vect_count;
signed16  	   retval;

    list_info = NULL;
    subord_count = 0;
    returned_name = NULL;
    partial_qual_obj = NULL;
    result_vector = NULL;
    vect_index = 0;
    retval = GC_SUCCESS;

    /* perform the list operation */
    ds_status = ds_list(session, context, name, &result, &invoke_id_return);

    if (ds_status != DS_SUCCESS)
    {
        /* map the XDS error to translator defined error and */
        /* create the error vector.                          */
	retval = gc_t_map_xds_error(ds_status, local_strings, out_vect);

	/* free the error object */
	om_delete(ds_status);
    }
    else
    {
	if (local_strings == TRUE)
	{
	    request_local_strings = OM_TRUE;
	}
	else
	{
	    request_local_strings = OM_FALSE;
	}
	/* get the list info */
	om_ret_code = om_get(result, 
			     OM_EXCLUDE_ALL_BUT_THESE_TYPES,
			     included_types, request_local_strings, 0, 0, 
			     &list_info, &total_number);
	/* free the result */
	om_delete(result);

	if (om_ret_code != OM_SUCCESS)
	{
	    /* map the XOM error to the translator defined */
	    /* error and create the error string.          */
	    retval = gc_t_map_xom_error(om_ret_code, out_vect);
	}
	else
	{
	    /* check if om_get has returned some object */
	    if (total_number <= 0)
	    {
		retval = GC_T_SUBORDINATES_NOT_FOUND;
		/* create the error vector */
		gc_t_create_gct_error_vector(retval, NULL, out_vect);
	    }
	    else
	    {
		list_info_det = list_info->value.object.object;

		/* scan the descriptor list */
		for (i = 0; list_info_det[i].type != OM_NO_MORE_TYPES; ++i)
		{
		    if (list_info_det[i].type == DS_OBJECT_NAME)
		    {
			returned_name = list_info_det[i].value.object.object;
			continue;
		    }

		    if (list_info_det[i].type == DS_SUBORDINATES)
		    {
			++subord_count;
			continue;
		    }

		    if (list_info_det[i].type == DS_PARTIAL_OUTCOME_QUAL)
		    {
			partial_qual_obj = list_info_det[i].value.object.object;
			continue;
		    }
		}

		partial_vect_count = 0;

		/* if there is a partial outcome qualifier */
		if (partial_qual_obj != NULL)
		{
		    /* count the number of vectors required for the partial */
		    /* outcome qualifier.                                   */
		    gc_t_get_partial_vect_count(partial_qual_obj, 
					        &partial_vect_count);
		}

		/* if no children found */
		if (subord_count == 0)
		{
		    /* if no partial outcome qualifier present */
		    if (partial_vect_count == 0)
		    {
			retval = GC_T_SUBORDINATES_NOT_FOUND;
			/* create the error vector */
			gc_t_create_gct_error_vector(retval, NULL, out_vect);
		    }
		    else
		    {
			/* create an error vector indicating the reason for */
			/* the partial outcome qualifier.		    */
			retval = gc_t_create_partial_qual_error(
							       partial_qual_obj,
							       local_strings,
							       out_vect);
			if (retval == GC_SUCCESS)
			{
			    /* return the translator defined error */
			    retval = GC_T_OPERATION_INCOMPLETE;
			}
		    }
		}
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {    
	/* allocate the area for the result vector. An additional */
	/* vector is required for GC_T_EOL.			  */

	vect_count = subord_count + partial_vect_count + 1;

	result_vector = (gc_t_vector *) malloc(sizeof(gc_t_vector) * 
					       vect_count);
	if (result_vector == NULL)
	{
	    retval = GC_T_MEMORY_INSUFFICIENT;
	    /* create the error vector */
	    gc_t_create_gct_error_vector(retval, NULL, out_vect);
	}
	else
	{
	    /* initialize the result vector with GC_T_EOL */
	    for (i = 0; i < vect_count; ++i)
		result_vector[i].type = GC_T_EOL;

	    for (i = 0; list_info_det[i].type != OM_NO_MORE_TYPES; ++i)
	    {
		/* if children RDNs */
		if (list_info_det[i].type == DS_SUBORDINATES)
		{
		    list_info_item = list_info_det[i].value.object.object;

		    /* locate the RDNs */
		    for (j = 0; list_info_item[j].type != OM_NO_MORE_TYPES; ++j)
		    {
			if (list_info_item[j].type == DS_RDN)
			{
			    rdn = list_info_item[j].value.object.object;
			    break;
			}
		    }

		    if (returned_name != NULL)
		    {
			/* create the result vector */
			retval = gc_t__process_list_rdns(returned_name, rdn, 
							 vect_index,
							 local_strings,
							 &result_vector,
							 out_vect);
		    }
		    else
		    {
			/* create the result vector */
			retval = gc_t__process_list_rdns(name, rdn, vect_index,
							 local_strings,
							 &result_vector,
							 out_vect);
		    }

		    if (retval != GC_SUCCESS)
		    {
			/* create the error vector */
			gc_t_create_gct_error_vector(retval, NULL, out_vect);
			break;
		    }
		    ++vect_index;
		}
	    }

	    if (retval == GC_SUCCESS)
	    {
		/* if a partial outcome qualifier is present */
		if (partial_vect_count != 0)
		{
		    /* process the partial outcome qualifier object */
		    retval = gc_t_process_partial_qual_object(partial_qual_obj,
						    local_strings,
						    &result_vector[vect_index],
						    out_vect);
					
		}
	    }
	}
    }

    if (retval != GC_SUCCESS)
    {
	/* free the result vector */
	if (result_vector != NULL)
	    gc_t_free_result(result_vector);
    }
    else
    {
	*out_vect = result_vector;
    }

    /* free the extracted object */
    if (list_info != NULL)
    {
	om_delete(list_info);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__process_list_rdns			   */
/* Description   : This function is used to convert the RDNs       */
/*		   from a list result to strings and update the    */
/*		   result vector.			   	   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.			 	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__process_list_rdns(
    OM_object      name,          /* IN  - The Parent DN 		*/
    OM_object      rdn,	          /* IN  - The Child RDN		*/
    unsigned32     vect_index,    /* IN  - Index into the result vector */
    boolean        local_strings, /* IN  - To specify local strings     */
    gc_t_vector  **result_vect,   /* OUT - Contains the result	        */
    gc_t_vector  **error_vect)    /* OUT - Contains Errors    	        */
{
OM_return_code  om_ret_code;
OM_boolean	request_local_strings;
OM_string       dn_omstring;
signed16	dn_len;
signed16        retval;
char            dn_string[GC_T_MAX_DN_STRING + 1];

    retval = GC_SUCCESS;
    dn_omstring.elements = (void *) &dn_string;
    dn_omstring.length = GC_T_MAX_DN_STRING + 1;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    /* convert the object to string */
    om_ret_code = omX_object_to_string(name, request_local_strings, 
				       &dn_omstring);

    if (om_ret_code == OM_SUCCESS)
    {
	dn_len = dn_omstring.length;

	/* if it is not root DN string, add the GC_T_RDN_DELIMITER_OP char.*/
	if (strlen(GC_T_RDN_DELIMITER_STR) != dn_omstring.length ||
	    memcmp(GC_T_RDN_DELIMITER_STR, dn_omstring.elements, 
		   dn_omstring.length) != 0)
	{
	    /* concatenate the GC_T_RDN_DELIMITER_OP character */
	    sprintf(&dn_string[dn_omstring.length], "%c", 
		    GC_T_RDN_DELIMITER_OP);
	    ++dn_len;
	}

	dn_omstring.elements = &dn_string[dn_len];
	dn_omstring.length = GC_T_MAX_DN_STRING + 1 - dn_len;

	/* now convert the RDNs to strings */
	om_ret_code = omX_object_to_string(rdn, request_local_strings, 
					   &dn_omstring);

	if (om_ret_code == OM_SUCCESS)
	{
	    dn_omstring.elements = dn_string;
	    dn_omstring.length = dn_omstring.length + dn_len;

	    /* fill the full dn string into the result vector */
	    retval = gc_t_create_result_vector_data(*result_vect, vect_index, 
						    GC_T_DN, dn_omstring,
						    OM_S_PRINTABLE_STRING);
	    if (retval != GC_SUCCESS)
	    {
		/* create the error vector */
		gc_t_create_gct_error_vector(retval, NULL, error_vect);
	    }
	}
	else
	{
	    /* map the error to the translator defined error and create */
	    /* error vector 						*/
	    retval = gc_t_map_xom_error(om_ret_code, error_vect);
	}
    }
    else
    {
	/* map the error to the translator defined error and create */
	/* error vector 					    */
	retval = gc_t_map_xom_error(om_ret_code, error_vect);
    }
    
    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
