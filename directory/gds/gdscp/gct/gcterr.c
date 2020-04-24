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
 * $Log: gcterr.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:19  root]
 * 
 * Revision 1.1.2.4  1994/09/19  08:39:34  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:33:26  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:43  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:39  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:06  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:54:50  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:36:10  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:46  keutel
 * 	creation
 * 	[1994/03/21  12:56:23  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gcterr.c					   */
/* Description : This file contains all the error handling 	   */
/*	         functions used by the GDSCP Translator.	   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gcterr.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:53 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gds.h>

#include <xom.h>
#include <xds.h>

#include <xomext.h>

#include <xoi.h>

#include <gdscp.h>
#include <gctstruct.h>
#include <gctconv.h>
#include <gcterr.h>
#include <gctutil.h>

/* Import the necessary Object Identifier constants */
OM_IMPORT(DS_C_ATTRIBUTE_ERROR)
OM_IMPORT(DS_C_NAME_ERROR)
OM_IMPORT(DS_C_REFERRAL)

/*-- Static Function(s) -------------------------------------------*/
static signed16 gc_t__process_cont_ref_error(
    DS_status     ref_object,    /* IN  - Contains DS_ACCESS_POINTS attribute */
    boolean       local_strings, /* IN  - To specify local strings            */
    gc_t_vector **error_vect);   /* OUT - The converted error string vector   */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_map_xds_error			           */
/* Description   : This function maps the XDS errors to the errors */
/*		   defined by the GDSCP Translator and creates the */
/*		   appropriate error message string.		   */
/* Return Value  : The error value defined by the Translator	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_map_xds_error(
    DS_status     error_xds,     /* IN - The XDS error to be converted      */
    boolean       local_strings, /* IN  - To specify local strings          */
    gc_t_vector **error_vect)    /* OUT - The converted error string vector */
{
OM_value_position  total_num;
OM_public_object   attr_error;
OM_public_object   error_object;
OM_public_object   resolved_dn;
OM_return_code     om_ret_code;
OM_boolean	   instance;
OM_boolean	   request_local_strings;
OM_string	   om_string;
OM_type		   error_list[] = {OM_NO_MORE_TYPES, OM_NO_MORE_TYPES};
signed16	   vect_count;
signed16 	   retval, i;
boolean	   	   error_xom, is_referral;
char		   dn_string[GC_T_MAX_DN_STRING + 1];
char 		  *string;

    error_xom = FALSE;
    resolved_dn = NULL;
    error_object = NULL;
    is_referral = FALSE;
    om_ret_code = OM_SUCCESS;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    while (om_ret_code == OM_SUCCESS)
    {
	/* check if a referral has been returned */
	if ((om_ret_code = om_instance(error_xds, DS_C_REFERRAL, 
				       &instance)) != OM_SUCCESS)
	{
	    error_xom = TRUE;
	    break;
	}

	if (instance == OM_TRUE)
	{
	    is_referral = TRUE;
	    break;
	}

	/* check if DS_C_ATTRIBUTE_ERROR has been passed */
	if ((om_ret_code = om_instance(error_xds, DS_C_ATTRIBUTE_ERROR, 
				       &instance)) != OM_SUCCESS)
	{
	    error_xom = TRUE;
	    break;
	}

	if (instance == OM_TRUE)
	{
	    /* extract the DS_PROBLEMS object */
	    error_list[0] = DS_PROBLEMS;
	    om_ret_code = om_get(error_xds,
				 OM_EXCLUDE_ALL_BUT_THESE_TYPES +
				 OM_EXCLUDE_SUBOBJECTS,
				 error_list, request_local_strings, 0, 0, 
				 &attr_error, &total_num);

	    if (om_ret_code != OM_SUCCESS)
	    {
		error_xom = TRUE;
		break;
	    }

	    /* check if om_get has returned some object */
	    if (total_num <= 0)
	    {
		om_ret_code = OM_NO_SUCH_OBJECT;
		error_xom = TRUE;
		break;
	    }

	    /* now extract the DS_PROBLEM object */
	    error_list[0] = DS_PROBLEM;
	    om_ret_code = om_get(attr_error->value.object.object,
				 OM_EXCLUDE_ALL_BUT_THESE_TYPES +
				 OM_EXCLUDE_SUBOBJECTS,
				 error_list, request_local_strings, 0, 0, 
				 &error_object, &total_num);

	    /* now delete the extracted attribute error object */
	    om_delete(attr_error);

	    if (om_ret_code != OM_SUCCESS)
	    {
		error_xom = TRUE;
		break;
	    }

	    /* check if om_get has returned some object */
	    if (total_num <= 0)
	    {
		om_ret_code = OM_NO_SUCH_OBJECT;
		error_xom = TRUE;
		break;
	    }
	}
	else
	{
	    /* check if DS_C_NAME_ERROR has been passed */
	    if ((om_ret_code = om_instance(error_xds, DS_C_NAME_ERROR, 
					   &instance)) != OM_SUCCESS)
	    {
		error_xom = TRUE;
		break;
	    }

	    if (instance == OM_TRUE)
	    {
		/* extract the DS_MATCHED object */
		error_list[0] = DS_MATCHED;
		om_ret_code = om_get(error_xds,
				     OM_EXCLUDE_ALL_BUT_THESE_TYPES +
				     OM_EXCLUDE_SUBOBJECTS,
				     error_list, request_local_strings, 0, 0, 
				     &resolved_dn, &total_num);

		if (om_ret_code != OM_SUCCESS)
		{
		    error_xom = TRUE;
		    break;
		}

		/* check if om_get has returned some object */
		if (total_num <= 0)
		{
		    om_ret_code = OM_NO_SUCH_OBJECT;
		    error_xom = TRUE;
		    break;
		}
	    }

	    /* Extract the DS_PROBLEM object */
	    error_list[0] = DS_PROBLEM;
	    om_ret_code = om_get(error_xds,
				 OM_EXCLUDE_ALL_BUT_THESE_TYPES +
				 OM_EXCLUDE_SUBOBJECTS,
				 error_list, request_local_strings, 0, 0, 
				 &error_object, &total_num);

	    if (om_ret_code != OM_SUCCESS)
	    {
		error_xom = TRUE;
		break;
	    }

	    /* check if om_get has returned some object */
	    if (total_num <= 0)
	    {
		om_ret_code = OM_NO_SUCH_OBJECT;
		error_xom = TRUE;
		break;
	    }
	}
	break;
    }
	
    if (error_xom != TRUE)
    {
	if (is_referral == TRUE)
	{
	    retval = gc_t__process_cont_ref_error(error_xds, local_strings, 
						  error_vect);

	    if (retval == GC_SUCCESS)
	    {
		/* return the translator defined error */
		retval = GC_T_REFERRAL_RETURNED;
	    }
	}
	else
	{
	    switch(error_object->value.enumeration)
	    {
		case (DS_E_ADMIN_LIMIT_EXCEEDED):
		    retval = GC_T_DS_E_ADMIN_LIMIT_EXCEEDED;
		    break;
		case (DS_E_AFFECTS_MULTIPLE_DSAS):
		    retval = GC_T_DS_E_AFFECTS_MULTIPLE_DSAS;
		    break;
		case (DS_E_ALIAS_DEREFERENCING_PROBLEM):
		    retval = GC_T_DS_E_ALIAS_DEREFERENCING_PROBLEM;
		    break;
		case (DS_E_ALIAS_PROBLEM):
		    retval = GC_T_DS_E_ALIAS_PROBLEM;
		    break;
		case (DS_E_ATTRIBUTE_OR_VALUE_EXISTS):
		    retval = GC_T_DS_E_ATTRIBUTE_OR_VALUE_EXISTS;
		    break;
		case (DS_E_BAD_ARGUMENT):
		    retval = GC_T_DS_E_BAD_ARGUMENT;
		    break;
		case (DS_E_BAD_CLASS):
		    retval = GC_T_DS_E_BAD_CLASS;
		    break;
		case (DS_E_BAD_CONTEXT):
		    retval = GC_T_DS_E_BAD_CONTEXT;
		    break;
		case (DS_E_BAD_NAME):
		    retval = GC_T_DS_E_BAD_NAME;
		    break;
		case (DS_E_BAD_SESSION):
		    retval = GC_T_DS_E_BAD_SESSION;
		    break;
		case (DS_E_BAD_WORKSPACE):
		    retval = GC_T_DS_E_BAD_WORKSPACE;
		    break;
		case (DS_E_BUSY):
		    retval = GC_T_DS_E_BUSY;
		    break;
		case (DS_E_CANNOT_ABANDON):
		    retval = GC_T_DS_E_CANNOT_ABANDON;
		    break;
		case (DS_E_CHAINING_REQUIRED):
		    retval = GC_T_DS_E_CHAINING_REQUIRED;
		    break;
		case (DS_E_COMMUNICATIONS_PROBLEM):
		    retval = GC_T_DS_E_COMMUNICATIONS_PROBLEM;
		    break;
		case (DS_E_CONSTRAINT_VIOLATION):
		    retval = GC_T_DS_E_CONSTRAINT_VIOLATION;
		    break;
		case (DS_E_DIT_ERROR):
		    retval = GC_T_DS_E_DIT_ERROR;
		    break;
		case (DS_E_ENTRY_EXISTS):
		    retval = GC_T_DS_E_ENTRY_EXISTS;
		    break;
		case (DS_E_INAPPROP_AUTHENTICATION):
		    retval = GC_T_DS_E_INAPPROP_AUTHENTICATION;
		    break;
		case (DS_E_INAPPROP_MATCHING):
		    retval = GC_T_DS_E_INAPPROP_MATCHING;
		    break;
		case (DS_E_INSUFFICIENT_ACCESS_RIGHTS):
		    retval = GC_T_DS_E_INSUFFICIENT_ACCESS_RIGHTS;
		    break;
		case (DS_E_INVALID_ATTRIBUTE_SYNTAX):
		    retval = GC_T_DS_E_INVALID_ATTRIBUTE_SYNTAX;
		    break;
		case (DS_E_INVALID_ATTRIBUTE_VALUE):
		    retval = GC_T_DS_E_INVALID_ATTRIBUTE_VALUE;
		    break;
		case (DS_E_INVALID_CREDENTIALS):
		    retval = GC_T_DS_E_INVALID_CREDENTIALS;
		    break;
		case (DS_E_INVALID_REF):
		    retval = GC_T_DS_E_INVALID_REF;
		    break;
		case (DS_E_INVALID_SIGNATURE):
		    retval = GC_T_DS_E_INVALID_SIGNATURE;
		    break;
		case (DS_E_LOOP_DETECTED):
		    retval = GC_T_DS_E_LOOP_DETECTED;
		    break;
		case (DS_E_MISCELLANEOUS):
		    retval = GC_T_DS_E_MISCELLANEOUS;
		    break;
		case (DS_E_MISSING_TYPE):
		    retval = GC_T_DS_E_MISSING_TYPE;
		    break;
		case (DS_E_MIXED_SYNCHRONOUS):
		    retval = GC_T_DS_E_MIXED_SYNCHRONOUS;
		    break;
		case (DS_E_NAMING_VIOLATION):
		    retval = GC_T_DS_E_NAMING_VIOLATION;
		    break;
		case (DS_E_NO_INFO):
		    retval = GC_T_DS_E_NO_INFO;
		    break;
		case (DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE):
		    retval = GC_T_DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE;
		    break;
		case (DS_E_NO_SUCH_OBJECT):
		    retval = GC_T_DS_E_NO_SUCH_OBJECT;
		    break;
		case (DS_E_NO_SUCH_OPERATION):
		    retval = GC_T_DS_E_NO_SUCH_OPERATION;
		    break;
		case (DS_E_NOT_ALLOWED_ON_NON_LEAF):
		    retval = GC_T_DS_E_NOT_ALLOWED_ON_NON_LEAF;
		    break;
		case (DS_E_NOT_ALLOWED_ON_RDN):
		    retval = GC_T_DS_E_NOT_ALLOWED_ON_RDN;
		    break;
		case (DS_E_NOT_SUPPORTED):
		    retval = GC_T_DS_E_NOT_SUPPORTED;
		    break;
		case (DS_E_OBJECT_CLASS_MOD_PROHIB):
		    retval = GC_T_DS_E_OBJECT_CLASS_MOD_PROHIB;
		    break;
		case (DS_E_OBJECT_CLASS_VIOLATION):
		    retval = GC_T_DS_E_OBJECT_CLASS_VIOLATION;
		    break;
		case (DS_E_OUT_OF_SCOPE):
		    retval = GC_T_DS_E_OUT_OF_SCOPE;
		    break;
		case (DS_E_PROTECTION_REQUIRED):
		    retval = GC_T_DS_E_PROTECTION_REQUIRED;
		    break;
		case (DS_E_TIME_LIMIT_EXCEEDED):
		    retval = GC_T_DS_E_TIME_LIMIT_EXCEEDED;
		    break;
		case (DS_E_TOO_LATE):
		    retval = GC_T_DS_E_TOO_LATE;
		    break;
		case (DS_E_TOO_MANY_OPERATIONS):
		    retval = GC_T_DS_E_TOO_MANY_OPERATIONS;
		    break;
		case (DS_E_TOO_MANY_SESSIONS):
		    retval = GC_T_DS_E_TOO_MANY_SESSIONS;
		    break;
		case (DS_E_UNABLE_TO_PROCEED):
		    retval = GC_T_DS_E_UNABLE_TO_PROCEED;
		    break;
		case (DS_E_UNAVAILABLE):
		    retval = GC_T_DS_E_UNAVAILABLE;
		    break;
		case (DS_E_UNAVAILABLE_CRIT_EXT):
		    retval = GC_T_DS_E_UNAVAILABLE_CRIT_EXT;
		    break;
		case (DS_E_UNDEFINED_ATTRIBUTE_TYPE):
		    retval = GC_T_DS_E_UNDEFINED_ATTRIBUTE_TYPE;
		    break;
		case (DS_E_UNWILLING_TO_PERFORM):
		    retval = GC_T_DS_E_UNWILLING_TO_PERFORM;
		    break;
		default:
		    retval = GC_T_UNKNOWN_XDS_ERROR;
		    break;
	    }

	    /* allocate the size for the error vector. One more is required */
	    /* to indicate end of vector.			 	    */
	    if (resolved_dn == NULL)
	    {
		vect_count = 2;
	    }
	    else
	    {
		/* one more additional vector is required to the resolved DN */
		vect_count = 3;
	    }

	    *error_vect = (gc_t_vector *) malloc(sizeof(gc_t_vector) * 
						 vect_count);
	    if (*error_vect != NULL)
	    {
		/* allocate the size for the message string */
		string = (char *) malloc(sizeof(char) * 
					 (GC_T_MAX_ERR_STRING + 1));
		if (string != NULL)
		{
		    om_string.elements = (void *) string;
		    om_string.length = GC_T_MAX_ERR_STRING;

		    /* convert the XDS error to string */
		    om_ret_code = omX_object_to_string(error_xds, 
						       request_local_strings, 
						       &om_string);
		    if (om_ret_code == OM_SUCCESS)
		    {
			/* put the EOS character */
			((char *) (om_string.elements))[om_string.length] = 0;

			i = 0;
			/* initialize the first vector type to GC_T_ERR */
			(*error_vect)[i].type = GC_T_ERR;
			(*error_vect)[i].value = string;

			++i;

			if (resolved_dn != NULL)
			{
			    *dn_string = 0;
			    om_string.elements = (void *) dn_string;
			    om_string.length = GC_T_MAX_DN_STRING + 1;

			    /* convert the name to string */
			    omX_object_to_string(
					     resolved_dn->value.object.object, 
					     request_local_strings, &om_string);

			    /* put the EOS character */
			  ((char *) (om_string.elements))[om_string.length] = 0;

			   /* fill the dn string into the result vector */
			   if (gc_t_create_result_vector_data(*error_vect, i,
							  GC_T_RESOLVED_DN, 
							  om_string,
							  OM_S_PRINTABLE_STRING)
			       == GC_SUCCESS)
			   {
				++i;
			   }
			}

			/* initialize the last vector type to GC_T_EOL */
			(*error_vect)[i].type = GC_T_EOL;
			(*error_vect)[i].value = 0;
		    }
		    else
		    {
			error_xom = TRUE;
			free(string);
			free(*error_vect);
		    }
		}
		else
		{
		    free(*error_vect);
		    retval = GC_T_MEMORY_INSUFFICIENT;
		    gc_t_create_gct_error_vector(retval, NULL, error_vect);
		}
	    }
	    else
	    {
		retval = GC_T_MEMORY_INSUFFICIENT;
		gc_t_create_gct_error_vector(retval, NULL, error_vect);
	    }
	}
    }

    if (error_xom == TRUE)
    {
	/* map the XOM error to the translator defined */
	/* error and create the error string.          */
	retval = gc_t_map_xom_error(om_ret_code, error_vect);
    }

    /* now delete the extracted error object */
    if (error_object != NULL)
    {
	om_delete(error_object);
    }

    if (resolved_dn != NULL)
    {
	om_delete(resolved_dn);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__process_cont_ref_error		   	   */
/* Description   : This function processes the DS_ACCESS_POINTS    */
/*		   attribute of DS_C_CONTINUATION_REF object and   */
/*		   creates the appropriate message string and error*/
/*		   vectors.  The first error vector contains the   */
/*		   error message string and the subsequent vectors */
/*		   contain the referral DSA name and PSAP address. */ 
/*		   This is used in cases where a directory 	   */
/*		   operation returns a referral error.		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.                              */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__process_cont_ref_error(
    DS_status     ref_object,    /* IN  - Contains DS_ACCESS_POINTS attribute */
    boolean       local_strings, /* IN  - To specify local strings            */
    gc_t_vector **error_vect)    /* OUT - The converted error string vector   */
{
OM_value_position  total_num;
OM_public_object   access_point_value;
OM_public_object   access_object;
OM_return_code     om_ret_code;
OM_boolean	   request_local_strings;
OM_string	   om_string;
OM_type		   type_list[] = {DS_ACCESS_POINTS, OM_NO_MORE_TYPES};
gc_t_vector	  *out_vect;
signed16	   vect_count;
signed16  	   retval;
signed16	   i, k, n;
char		   ref_name[GC_T_MAX_DN_STRING + 1];

    retval = GC_SUCCESS;;
    om_ret_code = OM_SUCCESS;
    access_point_value = NULL;
    out_vect = NULL;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    /* extract the DS_ACCESS_POINTS object */
    om_ret_code = om_get(ref_object, OM_EXCLUDE_ALL_BUT_THESE_TYPES,
			 type_list, request_local_strings, 0, 0, 
			 &access_point_value, &total_num);

	/* check if om_get has returned some object */
    if (om_ret_code == OM_SUCCESS && total_num <= 0)
    {
	om_ret_code = OM_NO_SUCH_OBJECT;
    }

    if (om_ret_code != OM_SUCCESS)
    {
	/* map the XOM error to the translator defined */
	/* error and create the error string.          */
	retval = gc_t_map_xom_error(om_ret_code, error_vect);
    }
    else
    {
	/* allocate the area for the error vectors.  One vector     */
	/* is required for the error message string.  Two vectors   */
	/* each (one for presentation address and one for dsa name) */
	/* are required for the access points and one is required   */
	/* to indicated end of list (GC_T_EOL).		            */
	vect_count =  1 + (2 * total_num) + 1;

	out_vect = (gc_t_vector *) malloc(sizeof(gc_t_vector) * vect_count);

	if (out_vect == NULL)
	{
	    retval = GC_T_MEMORY_INSUFFICIENT;
	    gc_t_create_gct_error_vector(retval, NULL, error_vect); 
	}	    
	else
	{
	    /* initialize the vectors to GC_T_EOL */
	    for (n = 0; n < vect_count; ++n)
	    {
		out_vect[n].type = GC_T_EOL;
	    }

	    om_string.elements = (void *) ref_name;
	    om_string.length = GC_T_MAX_DN_STRING;

	    access_object = access_point_value->value.object.object;

	    /* now append the referral dsa name to the error message */
	    for (k = 0; access_object[k].type != OM_NO_MORE_TYPES; ++k)
	    {
		if (access_object[k].type == DS_AE_TITLE)
		{
		    /* convert the referral name object to string */
		    om_ret_code = omX_object_to_string(
					  access_object[k].value.object.object,
					  request_local_strings, &om_string);

		    if (om_ret_code != OM_SUCCESS)
		    {
			/* map the XOM error to the translator defined */
			/* error and create the error string.          */
			retval = gc_t_map_xom_error(om_ret_code,
						    error_vect);
		    }
		    else
		    {
			/* put the EOS character */
			((char *) (om_string.elements))[om_string.length] = 0;
		    }
		    break;
		}
	    }

	    n = 0;

	    /* initialize the first vector with the error message string */
	    out_vect[n].type = GC_T_ERR;
	    out_vect[n].value = (char *) dce_sprintf(gc_t_referral_returned,
						     ref_name);
	    if (out_vect[n].value == NULL)
	    {
                free(out_vect);
                retval = GC_T_MEMORY_INSUFFICIENT;
                gc_t_create_gct_error_vector(retval, NULL, error_vect);
	    }
	    ++n;
	}
    }

    if (retval == GC_SUCCESS)
    {
	for (i = 0; i < total_num; ++i)
	{
	    access_object = access_point_value[i].value.object.object;

	    /* process the referral DSA name and the PSAP address in the */
	    /* access point object.					 */
	    retval = gc_t_process_access_points(access_object, local_strings,
						&out_vect[n], error_vect);
	    if (retval != GC_SUCCESS)
		break;

	    /* Append the vector index.  Two vectors would have been     */
	    /* created - one for DSA Name and the other for PSAP address */
	    n += 2;
	}
    }

    if (retval == GC_SUCCESS)
    {
	*error_vect = out_vect;
    }
    else
    {
	/* free the allocated memory */
	if (out_vect != NULL)
	{
	    gc_t_free_result(out_vect);
	}
    }

    /* free the extracted object */
    if (access_point_value != NULL)
    {
	om_delete(access_point_value);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_create_partial_qual_error	   	   */
/* Description   : This function is used to create an error vector */
/*		   containing details of the partial outcome       */
/*		   qualifier returned in the result after a search */
/*		   operation. The first vector contains the message*/
/*		   string and the subsequent vectors contain the   */
/*		   details of the partial outcome qualifier.	   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.                              */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_create_partial_qual_error(
    OM_object      partial_obj,   /* IN  - Partial outcome qualifier object */
    boolean        local_strings, /* IN  - To specify local strings         */
    gc_t_vector  **error_vect)    /* OUT - Error message in case of errors  */
{
OM_return_code  om_ret_code;
OM_boolean      request_local_strings;
OM_object       referral;
OM_object       access_point;
OM_string       om_string;
gc_t_vector    *vector;
unsigned16      vect_count, i;
signed16	len;
signed16	retval;
signed16	k, m, n;
char	        ref_name[GC_T_MAX_DN_STRING + 1];
char	       *msg;

    msg = NULL;
    vector = NULL;
    om_ret_code = OM_SUCCESS;
    retval = GC_SUCCESS;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    /* count the number of vectors required for the partial */
    /* outcome qualifier.                                   */
    gc_t_get_partial_vect_count(partial_obj, &vect_count);

    /* allocate the memory for the vector. One more vector is */
    /* required for the initial message string.		      */
    vector = (gc_t_vector *) malloc(sizeof(gc_t_vector) * (vect_count + 1));

    if (vector == NULL)
    {
	retval = GC_T_MEMORY_INSUFFICIENT;
	gc_t_create_gct_error_vector(retval, NULL, error_vect);
    }
    else
    {
	/* initialize the vectors */
	for (i = 0; i < vect_count + 1; ++i)
	{
	    vector[i].type = GC_T_EOL;
	}

	/* set the first error message string vector */
	for (k =  0; partial_obj[k].type != OM_NO_MORE_TYPES; ++k)
	{
	    if (partial_obj[k].type == DS_LIMIT_PROBLEM)
	    {
		switch (partial_obj[k].value.enumeration)
		{
		    case (DS_ADMIN_LIMIT_EXCEEDED):
			msg = (char *) dce_sprintf(gc_t_admin_limit_exceeded);
			break;
		    case (DS_SIZE_LIMIT_EXCEEDED):
			msg = (char *) dce_sprintf(gc_t_size_limit_exceeded);
			break;
		    case (DS_TIME_LIMIT_EXCEEDED):
			msg = (char *) dce_sprintf(gc_t_time_limit_exceeded);
			break;
		    default:
			break;
		}

		if (msg != NULL)
		{
		    break;
		}
	    }

	    if (partial_obj[k].type == DS_UNAVAILABLE_CRIT_EXT &&
		partial_obj[k].value.boolean == OM_TRUE)
	    {
		/* get the error message string from the catalog */
		msg = (char *) dce_sprintf(gc_t_unavailable_crit_ext);
		break;
	    }

	    if (partial_obj[k].type == DS_UNEXPLORED)
	    {
		break;
	    }
	}

	i = 0;

	if (partial_obj[k].type == DS_UNEXPLORED)
	{
	    referral = partial_obj[k].value.object.object;

	    for (m = 0; referral[m].type != OM_NO_MORE_TYPES; ++m)
	    {
		if (referral[m].type == DS_ACCESS_POINTS)
		{
		    access_point = referral[m].value.object.object;

		    for (n = 0; access_point[n].type != OM_NO_MORE_TYPES; ++n)
		    {
			if (access_point[n].type == DS_AE_TITLE)
			{
			    /* Note that this is treated as a GDSCP error */
			    /* since this is not an XDS error.  XDS has   */
			    /* returned success with no results but a     */
			    /* referral in the result.		          */

			    om_string.elements = (void *) ref_name;
			    om_string.length = GC_T_MAX_DN_STRING;

			    /* convert the name to string */
			    om_ret_code = omX_object_to_string(
					    access_point[n].value.object.object,
					    request_local_strings, &om_string);

			    if (om_ret_code == OM_SUCCESS)
			    {
				len = om_string.length;

				/* put the EOS character */
			 	((char *) (om_string.elements))[len] = 0;

				/* fill the string in the vector */
				vector[i].type = GC_T_ERR;
				vector[i].value = (char *) dce_sprintf(
				              gc_t_incomplete_referral_returned,
					      ref_name);

				if (vector[i].value == NULL)
				{
				    retval = GC_T_MEMORY_INSUFFICIENT;

				    /* create the error vector */
				    gc_t_create_gct_error_vector(retval, NULL,
								 error_vect);
				}
			    }
			    else
			    {
				/* map the XOM error to translator defined */
				/* error and create the error string.      */
				retval = gc_t_map_xom_error(om_ret_code,
							    error_vect);
			    }
			    break;
			}
		    }
		    break;
		}
	    }
	}
	else
	{
	    /* initialize vector with error string from catalog */
	    vector[i].type = GC_T_ERR;
	    vector[i].value = (char *) dce_sprintf(gc_t_incomplete_operation, 
						   msg);

	    /* now free the intermediate message */
	    free(msg);

	    if (vector[i].value == NULL)
	    {
		retval = GC_T_MEMORY_INSUFFICIENT;
		gc_t_create_gct_error_vector(retval, NULL, error_vect);
	    }
	}

	++i;

	if (retval == GC_SUCCESS)
	{
	    /* now process the partial outcome qualifier object and */
	    /* create the remaining vectors.			    */
	    retval = gc_t_process_partial_qual_object(partial_obj, 
						      local_strings, &vector[i],
						      error_vect);
	}
    }

    if (retval == GC_SUCCESS)
    {
	*error_vect = vector;
    }
    else
    {
	/* free the allocated areas */
	if (vector != NULL)
	{
	    gc_t_free_result(vector);
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_map_xom_error			   	   */
/* Description   : This function maps the XOM errors to the errors */
/*		   defined by the GDSCP Translator and creates the */
/*		   appropriate error message string.		   */
/* Return Value  : The error value defined by the Translator	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_map_xom_error(
    OM_return_code   om_ret_code,  /* IN  - The XOM error to be converted     */
    gc_t_vector    **error_vect)   /* OUT - The converted error string vector */
{
unsigned32  message_id;
signed16    retval;

    switch(om_ret_code)
    {
	case (OM_ENCODING_INVALID):
	    retval = GC_T_OM_ENCODING_INVALID;
	    message_id = gc_t_om_encoding_invalid;
	    break;
	case (OM_FUNCTION_DECLINED):
	    message_id = gc_t_om_function_declined;
	    retval = GC_T_OM_FUNCTION_DECLINED;
	    break;
	case (OM_FUNCTION_INTERRUPTED):
	    message_id = gc_t_om_function_interrupted;
	    retval = GC_T_OM_FUNCTION_INTERRUPTED;
	    break;
	case (OM_MEMORY_INSUFFICIENT):
	    message_id = gc_t_om_memory_insufficient;
	    retval = GC_T_OM_MEMORY_INSUFFICIENT;
	    break;
	case (OM_NETWORK_ERROR):
	    message_id = gc_t_om_network_error;
	    retval = GC_T_OM_NETWORK_ERROR;
	    break;
	case (OM_NO_SUCH_CLASS):
	    message_id = gc_t_om_no_such_class;
	    retval = GC_T_OM_NO_SUCH_CLASS;
	    break;
	case (OM_NO_SUCH_EXCLUSION):
	    message_id = gc_t_om_no_such_exclusion;
	    retval = GC_T_OM_NO_SUCH_EXCLUSION;
	    break;
	case (OM_NO_SUCH_MODIFICATION):
	    message_id = gc_t_om_no_such_modification;
	    retval = GC_T_OM_NO_SUCH_MODIFICATION;
	    break;
	case (OM_NO_SUCH_OBJECT):
	    message_id = gc_t_om_no_such_object;
	    retval = GC_T_OM_NO_SUCH_OBJECT;
	    break;
	case (OM_NO_SUCH_RULES):
	    message_id = gc_t_om_no_such_rules;
	    retval = GC_T_OM_NO_SUCH_RULES;
	    break;
	case (OM_NO_SUCH_SYNTAX):
	    message_id = gc_t_om_no_such_syntax;
	    retval = GC_T_OM_NO_SUCH_SYNTAX;
	    break;
	case (OM_NO_SUCH_TYPE):
	    message_id = gc_t_om_no_such_type;
	    retval = GC_T_OM_NO_SUCH_TYPE;
	    break;
	case (OM_NO_SUCH_WORKSPACE):
	    message_id = gc_t_om_no_such_workspace;
	    retval = GC_T_OM_NO_SUCH_WORKSPACE;
	    break;
	case (OM_NOT_AN_ENCODING):
	    message_id = gc_t_om_not_an_encoding;
	    retval = GC_T_OM_NOT_AN_ENCODING;
	    break;
	case (OM_NOT_CONCRETE):
	    message_id = gc_t_om_not_concrete;
	    retval = GC_T_OM_NOT_CONCRETE;
	    break;
	case (OM_NOT_PRESENT):
	    message_id = gc_t_om_not_present;
	    retval = GC_T_OM_NOT_PRESENT;
	    break;
	case (OM_NOT_PRIVATE):
	    message_id = gc_t_om_not_private;
	    retval = GC_T_OM_NOT_PRIVATE;
	    break;
	case (OM_NOT_THE_SERVICES):
	    message_id = gc_t_om_not_the_services;
	    retval = GC_T_OM_NOT_THE_SERVICES;
	    break;
	case (OM_PERMANENT_ERROR):
	    message_id = gc_t_om_permanent_error;
	    retval = GC_T_OM_PERMANENT_ERROR;
	    break;
	case (OM_POINTER_INVALID):
	    message_id = gc_t_om_pointer_invalid;
	    retval = GC_T_OM_POINTER_INVALID;
	    break;
	case (OM_SYSTEM_ERROR):
	    message_id = gc_t_om_system_error;
	    retval = GC_T_OM_SYSTEM_ERROR;
	    break;
	case (OM_TEMPORARY_ERROR):
	    message_id = gc_t_om_temporary_error;
	    retval = GC_T_OM_TEMPORARY_ERROR;
	    break;
	case (OM_TOO_MANY_VALUES):
	    message_id = gc_t_om_too_many_values;
	    retval = GC_T_OM_TOO_MANY_VALUES;
	    break;
	case (OM_VALUES_NOT_ADJACENT):
	    message_id = gc_t_om_values_not_adjacent;
	    retval = GC_T_OM_VALUES_NOT_ADJACENT;
	    break;
	case (OM_WRONG_VALUE_LENGTH):
	    message_id = gc_t_om_wrong_value_length;
	    retval = GC_T_OM_WRONG_VALUE_LENGTH;
	    break;
	case (OM_WRONG_VALUE_MAKEUP):
	    message_id = gc_t_om_wrong_value_makeup;
	    retval = GC_T_OM_WRONG_VALUE_MAKEUP;
	    break;
	case (OM_WRONG_VALUE_NUMBER):
	    message_id = gc_t_om_wrong_value_number;
	    retval = GC_T_OM_WRONG_VALUE_NUMBER;
	    break;
	case (OM_WRONG_VALUE_POSITION):
	    message_id = gc_t_om_wrong_value_position;
	    retval = GC_T_OM_WRONG_VALUE_POSITION;
	    break;
	case (OM_WRONG_VALUE_SYNTAX):
	    message_id = gc_t_om_wrong_value_syntax;
	    retval = GC_T_OM_WRONG_VALUE_SYNTAX;
	    break;
	case (OM_WRONG_VALUE_TYPE):
	    message_id = gc_t_om_wrong_value_type;
	    retval = GC_T_OM_WRONG_VALUE_TYPE;
	    break;
	case (OMX_CLASS_NOT_FOUND_IN_SCHEMA_FILE):
	    message_id = gc_t_omx_class_not_found_in_schema_file;
	    retval = GC_T_OMX_CLASS_NOT_FOUND_IN_SCHEMA_FILE;
	    break;
	default:
	    message_id = gc_t_unknown_xom_error;
	    retval = GC_T_UNKNOWN_XOM_ERROR;
	    break;
    }

    /* allocate the size for the error vector. One more is required */
    /* to indicate end of vector.				    */
    *error_vect = (gc_t_vector *) malloc(sizeof(gc_t_vector) * 2);

    if (*error_vect != NULL)
    {
	/* initialize the first vector type to GC_T_ERR */
	(*error_vect)[0].type = GC_T_ERR;

	/* get the error message from the catalog */
	(*error_vect)[0].value = (char *) dce_sprintf(message_id);

	if ((*error_vect)[0].value == NULL)
	{
	    free(*error_vect);
	    retval = GC_T_MEMORY_INSUFFICIENT;
	    gc_t_create_gct_error_vector(retval, NULL, error_vect);
	}
	else
	{
	    /* initialize the second vector type to GC_T_EOL */
	    (*error_vect)[1].type = GC_T_EOL;
	    (*error_vect)[1].value = 0;
	}
    }
    else
    {
	retval = GC_T_MEMORY_INSUFFICIENT;
	gc_t_create_gct_error_vector(retval, NULL, error_vect);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_map_xos_error			   	   */
/* Description   : This function maps the XOM convenience          */
/*		   library errors to the errors defined by the     */
/*		   GDSCP Translator and creates the appropriate    */
/*		   error message string.		   	   */
/* Return Value  : The error value defined by the Translator	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_map_xos_error(
    OM_integer    error,        /* IN  - The error to be converted         */
    char	 *err_string,   /* IN  - Additional string needed in output*/
    OM_integer	  error_pos,    /* OUT - 0 or error position		   */
    gc_t_vector **error_vect)   /* OUT - The converted error string vector */
{
unsigned32  message_id;
signed16    retval;
char 	   *str;

    switch(error)
    {
	case (OMX_CANNOT_READ_SCHEMA):
	    message_id = gc_t_omx_cannot_read_schema;
	    retval = GC_T_OMX_CANNOT_READ_SCHEMA;
	    break;
	case (OMX_SCHEMA_NOT_READ):
	    message_id = gc_t_omx_schema_not_read;
	    retval = GC_T_OMX_SCHEMA_NOT_READ;
	    break;
	case (OMX_NO_START_OBJ_BLOCK):
	    message_id = gc_t_omx_no_start_obj_block;
	    retval = GC_T_OMX_NO_START_OBJ_BLOCK;
	    break;
	case (OMX_NO_END_OBJ_BLOCK):
	    message_id = gc_t_omx_no_end_obj_block;
	    retval = GC_T_OMX_NO_END_OBJ_BLOCK;
	    break;
	case (OMX_EMPTY_OBJ_BLOCK):
	    message_id = gc_t_omx_empty_obj_block;
	    retval = GC_T_OMX_EMPTY_OBJ_BLOCK;
	    break;
	case (OMX_OBJ_FORMAT_ERROR):
	    message_id = gc_t_omx_obj_format_error;
	    retval = GC_T_OMX_OBJ_FORMAT_ERROR;
	    break;
	case (OMX_DUPLICATE_OBJ_ABBRV):
	    message_id = gc_t_omx_duplicate_obj_abbrv;
	    retval = GC_T_OMX_DUPLICATE_OBJ_ABBRV;
	    break;
	case (OMX_DUPLICATE_OBJ_OBJ_ID):
	    message_id = gc_t_omx_duplicate_obj_obj_id;
	    retval = GC_T_OMX_DUPLICATE_OBJ_OBJ_ID;
	    break;
	case (OMX_NO_START_ATTR_BLOCK):
	    message_id = gc_t_omx_no_start_attr_block;
	    retval = GC_T_OMX_NO_START_ATTR_BLOCK;
	    break;
	case (OMX_NO_END_ATTR_BLOCK):
	    message_id = gc_t_omx_no_end_attr_block;
	    retval = GC_T_OMX_NO_END_ATTR_BLOCK;
	    break;
	case (OMX_EMPTY_ATTR_BLOCK):
	    message_id = gc_t_omx_empty_attr_block;
	    retval = GC_T_OMX_EMPTY_ATTR_BLOCK;
	    break;
	case (OMX_ATTR_FORMAT_ERROR):
	    message_id = gc_t_omx_attr_format_error;
	    retval = GC_T_OMX_ATTR_FORMAT_ERROR;
	    break;
	case (OMX_DUPLICATE_ATTR_ABBRV):
	    message_id = gc_t_omx_duplicate_attr_abbrv;
	    retval = GC_T_OMX_DUPLICATE_ATTR_ABBRV;
	    break;
	case (OMX_DUPLICATE_ATTR_OBJ_ID):
	    message_id = gc_t_omx_duplicate_attr_obj_id;
	    retval = GC_T_OMX_DUPLICATE_ATTR_OBJ_ID;
	    break;
	case (OMX_NO_START_CLASS_BLOCK):
	    message_id = gc_t_omx_no_start_class_block;
	    retval = GC_T_OMX_NO_START_CLASS_BLOCK;
	    break;
	case (OMX_NO_END_CLASS_BLOCK):
	    message_id = gc_t_omx_no_end_class_block;
	    retval = GC_T_OMX_NO_END_CLASS_BLOCK;
	    break;
	case (OMX_EMPTY_CLASS_BLOCK):
	    message_id = gc_t_omx_empty_class_block;
	    retval = GC_T_OMX_EMPTY_CLASS_BLOCK;
	    break;
	case (OMX_CLASS_FORMAT_ERROR):
	    message_id = gc_t_omx_class_format_error;
	    retval = GC_T_OMX_CLASS_FORMAT_ERROR;
	    break;
	case (OMX_NO_CLASS_NAME):
	    message_id = gc_t_omx_no_class_name;
	    retval = GC_T_OMX_NO_CLASS_NAME;
	    break;
	case (OMX_DUPLICATE_CLASS_BLOCK):
	    message_id = gc_t_omx_duplicate_class_block;
	    retval = GC_T_OMX_DUPLICATE_CLASS_BLOCK;
	    break;
	case (OMX_CLASS_BLOCK_UNDEFINED):
	    message_id = gc_t_omx_class_block_undefined;
	    retval = GC_T_OMX_CLASS_BLOCK_UNDEFINED;
	    break;
	case (OMX_INVALID_ABBRV):
	    message_id = gc_t_omx_invalid_abbrv;
	    retval = GC_T_OMX_INVALID_ABBRV;
	    break;
	case (OMX_INVALID_OBJ_ID):
	    message_id = gc_t_omx_invalid_obj_id;
	    retval = GC_T_OMX_INVALID_OBJ_ID;
	    break;
	case (OMX_INVALID_CLASS_NAME):
	    message_id = gc_t_omx_invalid_class_name;
	    retval = GC_T_OMX_INVALID_CLASS_NAME;
	    break;
	case (OMX_INVALID_SYNTAX):
	    message_id = gc_t_omx_invalid_syntax;
	    retval = GC_T_OMX_INVALID_SYNTAX;
	    break;
	case (OMX_MEMORY_INSUFFICIENT):
	    message_id = gc_t_omx_memory_insufficient;
	    retval = GC_T_OMX_MEMORY_INSUFFICIENT;
	    break;
	case (OMX_INVALID_PARAMETER):
	    message_id = gc_t_omx_invalid_parameter;
	    retval = GC_T_OMX_INVALID_PARAMETER;
	    break;
	case (OMX_UNKNOWN_ABBRV):
	    message_id = gc_t_omx_unknown_abbrv;
	    retval = GC_T_OMX_UNKNOWN_ABBRV;
	    break;
	case (OMX_UNKNOWN_OBJ_ID):
	    message_id = gc_t_omx_unknown_obj_id;
	    retval = GC_T_OMX_UNKNOWN_OBJ_ID;
	    break;
	case (OMX_UNKNOWN_OMTYPE):
	    message_id = gc_t_omx_unknown_omtype;
	    retval = GC_T_OMX_UNKNOWN_OMTYPE;
	    break;
	case (OMX_MISSING_AVA):
	    message_id = gc_t_omx_missing_ava;
	    retval = GC_T_OMX_MISSING_AVA;
	    break;
	case (OMX_MISSING_ABBRV):
	    message_id = gc_t_omx_missing_abbrv;
	    retval = GC_T_OMX_MISSING_ABBRV;
	    break;
	case (OMX_FORMAT_ERROR):
	    message_id = gc_t_omx_format_error;
	    retval = GC_T_OMX_FORMAT_ERROR;
	    break;
	case (OMX_UNKNOWN_ERROR):
	    message_id = gc_t_omx_unknown_error;
	    retval = GC_T_OMX_UNKNOWN_ERROR;
	    break;
	case (OMX_MISSING_RDN_DELIMITER):
	    message_id = gc_t_omx_missing_rdn_delimiter;
	    retval = GC_T_OMX_MISSING_RDN_DELIMITER;
	    break;
	case (OMX_MISMATCHED_QUOTES):
	    message_id = gc_t_omx_mismatched_quotes;
	    retval = GC_T_OMX_MISMATCHED_QUOTES;
	    break;
	case (OMX_MISSING_EQUAL_OPERATOR):
	    message_id = gc_t_omx_missing_equal_operator;
	    retval = GC_T_OMX_MISSING_EQUAL_OPERATOR;
	    break;
	case (OMX_MISSING_ATTR_VALUE):
	    message_id = gc_t_omx_missing_attr_value;
	    retval = GC_T_OMX_MISSING_ATTR_VALUE;
	    break;
	case (OMX_MISSING_ATTR_INFO):
	    message_id = gc_t_omx_missing_attr_info;
	    retval = GC_T_OMX_MISSING_ATTR_INFO;
	    break;
	case (OMX_MISSING_CLASS_START_OP):
	    message_id = gc_t_omx_missing_class_start_op;
	    retval = GC_T_OMX_MISSING_CLASS_START_OP;
	    break;
	case (OMX_MISSING_CLASS_END_OP):
	    message_id = gc_t_omx_missing_class_end_op;
	    retval = GC_T_OMX_MISSING_CLASS_END_OP;
	    break;
	case (OMX_MISSING_CLASS_VALUE):
	    message_id = gc_t_omx_missing_class_value;
	    retval = GC_T_OMX_MISSING_CLASS_VALUE;
	    break;
	case (OMX_MISSING_COMP_VALUE):
	    message_id = gc_t_omx_missing_comp_value;
	    retval = GC_T_OMX_MISSING_COMP_VALUE;
	    break;
	case (OMX_MISMATCHED_BRACKETS):
	    message_id = gc_t_omx_mismatched_brackets;
	    retval = GC_T_OMX_MISMATCHED_BRACKETS;
	    break;
	case (OMX_UNEXPECTED_OPERATOR):
	    message_id = gc_t_omx_unexpected_operator;
	    retval = GC_T_OMX_UNEXPECTED_OPERATOR;
	    break;
	case (OMX_WRONG_VALUE):
	    message_id = gc_t_omx_wrong_value;
	    retval = GC_T_OMX_WRONG_VALUE;
	    break;
	case (OMX_UNKNOWN_KEYWORD):
	    message_id = gc_t_omx_unknown_keyword;
	    retval = GC_T_OMX_UNKNOWN_KEYWORD;
	    break;
	case (OMX_MISSING_OPERATOR):
	    message_id = gc_t_omx_missing_operator;
	    retval = GC_T_OMX_MISSING_OPERATOR;
	    break;
	case (OMX_MISSING_COMPOUND_OP):
	    message_id = gc_t_omx_missing_compound_op;
	    retval = GC_T_OMX_MISSING_COMPOUND_OP;
	    break;
	default:
	    message_id = gc_t_unknown_conv_lib_error;
	    retval = GC_T_UNKNOWN_CONV_LIB_ERROR;
	    break;
    }

    /* allocate the size for the error vector. One more is required */
    /* to indicate end of vector.				    */
    *error_vect = (gc_t_vector *) malloc(sizeof(gc_t_vector) * 2);

    if (*error_vect != NULL)
    {
	/* initialize the first vector type to GC_T_ERR */
	(*error_vect)[0].type = GC_T_ERR;

	if (error_pos != 0)
	{
	    /* get the intermediate message string from catalog */
	    str = (char *) dce_sprintf(gc_t_error_position_format, 
				       err_string, error_pos);
	    if (str == NULL)
	    {
	        (*error_vect)[0].value = NULL;
	    }
	    else
	    {
		/* now create the full message string from catalog */
		(*error_vect)[0].value = (char *) dce_sprintf(message_id, str);

		/* free the intermediate string */
		free(str);
	    }
	}
	else
	{
	    if (err_string != NULL)
	    {
		/* get the message string from the catalog */
		(*error_vect)[0].value = (char *) dce_sprintf(message_id, 
							      err_string);
	    }
	    else
	    {
		/* get the message string from the catalog */
		(*error_vect)[0].value = (char *) dce_sprintf(message_id);
	    }
	}

	if ((*error_vect)[0].value == NULL)
	{
	    free(*error_vect);
	    retval = GC_T_MEMORY_INSUFFICIENT;
	    gc_t_create_gct_error_vector(retval, NULL, error_vect);
	}
	else
	{
	    /* initialize the second vector type to GC_T_EOL */
	    (*error_vect)[1].type = GC_T_EOL;
	    (*error_vect)[1].value = 0;
	}
    }
    else
    {
	retval = GC_T_MEMORY_INSUFFICIENT;
	gc_t_create_gct_error_vector(retval, NULL, error_vect);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_create_gct_error_vector			   */
/* Description   : This function converts the GDSCP Translator     */
/*		   errors to the appropriate error message string. */
/* Return Value  : The error value defined by the Translator	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_create_gct_error_vector(
    signed16      error,        /* IN  - The error to be converted            */
    char	 *append_str,   /* IN  - String to be appended to error string*/
    gc_t_vector **error_vect)   /* OUT - The converted error string vector    */
{
unsigned32  message_id;
signed16    retval;

    retval = error;

    switch(error)
    {
	case (GC_T_CONFLICTING_OPTION):
	    message_id = gc_t_conflicting_option;
	    break;
	case (GC_T_UNKNOWN_OPTION):
	    message_id = gc_t_unknown_option;
	    break;
	case (GC_T_MISSING_DIRECTORY_ID):
	    message_id = gc_t_missing_directory_id;
	    break;
	case (GC_T_MISSING_OBJECT_NAME):
	    message_id = gc_t_missing_object_name;
	    break;
	case (GC_T_MISSING_ATTRIBUTE_INFO):
	    message_id = gc_t_missing_attribute_info;
	    break;
	case (GC_T_MISSING_NEW_ATTRIBUTE_INFO):
	    message_id = gc_t_missing_new_attribute_info;
	    break;
	case (GC_T_MISSING_DSA_NAME):
	    message_id = gc_t_missing_dsa_name;
	    break;
	case (GC_T_MISSING_AUTHENTICATION_TYPE):
	    message_id = gc_t_missing_authentication_type;
	    break;
	case (GC_T_MISSING_PSAP_ADDRESS):
	    message_id = gc_t_missing_psap_address;
	    break;
	case (GC_T_MISSING_USER_NAME):
	    message_id = gc_t_missing_user_name;
	    break;
	case (GC_T_MISSING_USER_PASSWORD):
	    message_id = gc_t_missing_user_password;
	    break;
	case (GC_T_MISSING_FILTER):
	    message_id = gc_t_missing_filter;
	    break;
	case (GC_T_MISSING_SCOPE_OF_SEARCH):
	    message_id = gc_t_missing_scope_of_search;
	    break;
	case (GC_T_MISSING_TIME_LIMIT):
	    message_id = gc_t_missing_time_limit;
	    break;
	case (GC_T_MISSING_SIZE_LIMIT):
	    message_id = gc_t_missing_size_limit;
	    break;
	case (GC_T_MISSING_ARGUMENTS):
	    message_id = gc_t_missing_arguments;
	    break;
	case (GC_T_INVALID_DIRECTORY_ID):
	    message_id = gc_t_invalid_directory_id;
	    break;
	case (GC_T_INVALID_TIME_LIMIT):
	    message_id = gc_t_invalid_time_limit;
	    break;
	case (GC_T_INVALID_SIZE_LIMIT):
	    message_id = gc_t_invalid_size_limit;
	    break;
	case (GC_T_UNKNOWN_ARGUMENT):
	    message_id = gc_t_unknown_argument;
	    break;
	case (GC_T_CANNOT_INITIALIZE_WORKSPACE):
	    message_id = gc_t_cannot_initialize_workspace;
	    break;
	case (GC_T_BIND_NOT_MADE):
	    message_id = gc_t_bind_not_made;
	    break;
	case (GC_T_MISSING_VALUE):
	    message_id = gc_t_missing_value;
	    break;
	case (GC_T_FORMAT_ERROR):
	    message_id = gc_t_format_error;
	    break;
	case (GC_T_MISMATCHED_QUOTES):
	    message_id = gc_t_mismatched_quotes;
	    break;
	case (GC_T_INVALID_PARAMETER):
	    message_id = gc_t_invalid_parameter;
	    break;
	case (GC_T_TOO_MANY_ARGUMENTS):
	    message_id = gc_t_too_many_arguments;
	    break;
	case (GC_T_ENTRY_NOT_FOUND):
	    message_id = gc_t_entry_not_found;
	    break;
	case (GC_T_SUBORDINATES_NOT_FOUND):
	    message_id = gc_t_subordinates_not_found;
	    break;
	case (GC_T_MEMORY_INSUFFICIENT):
	    message_id = gc_t_memory_insufficient;
	    break;
	case (GC_T_AMBIGUOUS_OPTION):
	    message_id = gc_t_ambiguous_option;
	    break;
	case (GC_T_TOO_MANY_VALUES):
	    message_id = gc_t_too_many_values;
	    break;
	default:
	    message_id = gc_t_unknown_gcp_error;
	    retval = GC_T_UNKNOWN_GCP_ERROR;
	    break;
    }

    /* allocate the size for the error vector. One more is required */
    /* to indicate end of vector.				    */
    *error_vect = (gc_t_vector *) malloc(sizeof(gc_t_vector) * 2);

    if (*error_vect != NULL)
    {
	/* initialize the first vector type to GC_T_ERR */
	(*error_vect)[0].type = GC_T_ERR;

	/* get the message string from the catalog */
	if (append_str != NULL)
	{
	    (*error_vect)[0].value = (char *) dce_sprintf(message_id, 
							  append_str);
	}
	else
	{
	    (*error_vect)[0].value = (char *) dce_sprintf(message_id);
	}

	if ((*error_vect)[0].value == NULL)
	{
	    free(*error_vect);
	    retval = GC_T_MEMORY_INSUFFICIENT;
	    *error_vect = NULL;
	}
	else
	{
	    /* initialize the second vector type to GC_T_EOL */
	    (*error_vect)[1].type = GC_T_EOL;
	    (*error_vect)[1].value = 0;
	}
    }
    else
    {
	retval = GC_T_MEMORY_INSUFFICIENT;
	*error_vect = NULL;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_map_xoi_error			   	   */
/* Description   : This function converts the XOI Module errors    */
/*		   to the appropriate error message string. 	   */
/*		   Note that this function does not handle 	   */
/*		   XOI module errors related to errors in the      */
/*		   XOM object info schema file.			   */ 
/* Return Value  : The error value defined by the Translator	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_map_xoi_error(
    OM_integer    error,        /* IN  - The error to be converted            */
    char	 *append_str,   /* IN  - String to be appended to error string*/
    gc_t_vector **error_vect)   /* OUT - The converted error string vector    */
{
OM_integer  error_xos;
signed16    retval;

    /* The XOI Module error is first converted to the equivalent     */
    /* XOM Convenience library error and is treated as a convenience */
    /* library error.						     */
    switch(error)
    {
	case (XOI_SCHEMA_NOT_READ):
	    error_xos = OMX_SCHEMA_NOT_READ;
	    break;
	case (XOI_INVALID_ABBRV):
	    error_xos = OMX_INVALID_ABBRV;
	    break;
	case (XOI_INVALID_OBJ_ID):
	    error_xos = OMX_INVALID_OBJ_ID;
	    break;
	case (XOI_INVALID_CLASS_NAME):
	    error_xos = OMX_INVALID_CLASS_NAME;
	    break;
	case (XOI_MEMORY_INSUFFICIENT):
	    error_xos = OMX_MEMORY_INSUFFICIENT;
	    break;
	case (XOI_INVALID_PARAMETER):
	    error_xos = OMX_INVALID_PARAMETER;
	    break;
	case (XOI_UNKNOWN_ABBRV):
	    error_xos = OMX_UNKNOWN_ABBRV;
	    break;
	case (XOI_UNKNOWN_OBJ_ID):
	    error_xos = OMX_UNKNOWN_OBJ_ID;
	    break;
	case (XOI_UNKNOWN_OMTYPE):
	    error_xos = OMX_UNKNOWN_OMTYPE;
	    break;
	default:
	    error_xos = OMX_UNKNOWN_ERROR;
	    break;
    }

    /* map the error to the translator defined error and create error string */
    retval = gc_t_map_xos_error(error_xos, append_str, 0, error_vect);

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_map_xoi_schema_file_error	   	   */
/* Description   : This function converts the XOI module errors    */
/*		   related to errors in the XOI object info schema */
/*		   file. It converts the errors to the translator  */
/*		   defined errors.				   */
/* Return Value  : The error value defined by the Translator	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_map_xoi_schema_file_error(
    OM_integer    error,        /* IN  - The error to be converted            */
    OM_uint	  line_no,	/* IN  - The error line number in schema file */
    gc_t_vector **error_vect)   /* OUT - The converted error string vector    */
{
unsigned32  message_id;
char	    file_name[GC_T_MAX_FILE_NAME + 1];
char	   *str;
signed16    retval;

    /* get the XOM object information schema file name */
    xoi_get_schema_file_name(file_name);

    /* The XOI Module error is converted to the equivalent */
    /* XOM Convenience library error and is treated as a   */
    /* convenience library error.			   */
    switch(error)
    {
	case (XOI_CANNOT_READ_SCHEMA):
	    retval = GC_T_OMX_CANNOT_READ_SCHEMA;
	    message_id = gc_t_omx_cannot_read_schema;
	    break;
	case (XOI_NO_START_OBJ_BLOCK):
	    retval = GC_T_OMX_NO_START_OBJ_BLOCK;
	    message_id = gc_t_omx_no_start_obj_block;
	    break;
	case (XOI_NO_END_OBJ_BLOCK):
	    retval = GC_T_OMX_NO_END_OBJ_BLOCK;
	    message_id = gc_t_omx_no_end_obj_block;
	    break;
	case (XOI_EMPTY_OBJ_BLOCK):
	    retval = GC_T_OMX_EMPTY_OBJ_BLOCK;
	    message_id = gc_t_omx_empty_obj_block;
	    break;
	case (XOI_OBJ_FORMAT_ERROR):
	    retval = GC_T_OMX_OBJ_FORMAT_ERROR;
	    message_id = gc_t_omx_obj_format_error;
	    break;
	case (XOI_DUPLICATE_OBJ_ABBRV):
	    retval = GC_T_OMX_DUPLICATE_OBJ_ABBRV;
	    message_id = gc_t_omx_duplicate_obj_abbrv;
	    break;
	case (XOI_DUPLICATE_OBJ_OBJ_ID):
	    retval = GC_T_OMX_DUPLICATE_OBJ_OBJ_ID;
	    message_id = gc_t_omx_duplicate_obj_obj_id;
	    break;
	case (XOI_NO_START_ATTR_BLOCK):
	    retval = GC_T_OMX_NO_START_ATTR_BLOCK;
	    message_id = gc_t_omx_no_start_attr_block;
	    break;
	case (XOI_NO_END_ATTR_BLOCK):
	    retval = GC_T_OMX_NO_END_ATTR_BLOCK;
	    message_id = gc_t_omx_no_end_attr_block;
	    break;
	case (XOI_EMPTY_ATTR_BLOCK):
	    retval = GC_T_OMX_EMPTY_ATTR_BLOCK;
	    message_id = gc_t_omx_empty_attr_block;
	    break;
	case (XOI_ATTR_FORMAT_ERROR):
	    retval = GC_T_OMX_ATTR_FORMAT_ERROR;
	    message_id = gc_t_omx_attr_format_error;
	    break;
	case (XOI_DUPLICATE_ATTR_ABBRV):
	    retval = GC_T_OMX_DUPLICATE_ATTR_ABBRV;
	    message_id = gc_t_omx_duplicate_attr_abbrv;
	    break;
	case (XOI_DUPLICATE_ATTR_OBJ_ID):
	    retval = GC_T_OMX_DUPLICATE_ATTR_OBJ_ID;
	    message_id = gc_t_omx_duplicate_attr_obj_id;
	    break;
	case (XOI_NO_START_CLASS_BLOCK):
	    retval = GC_T_OMX_NO_START_CLASS_BLOCK;
	    message_id = gc_t_omx_no_start_class_block;
	    break;
	case (XOI_NO_END_CLASS_BLOCK):
	    retval = GC_T_OMX_NO_END_CLASS_BLOCK;
	    message_id = gc_t_omx_no_end_class_block;
	    break;
	case (XOI_EMPTY_CLASS_BLOCK):
	    retval = GC_T_OMX_EMPTY_CLASS_BLOCK;
	    message_id = gc_t_omx_empty_class_block;
	    break;
	case (XOI_CLASS_FORMAT_ERROR):
	    retval = GC_T_OMX_CLASS_FORMAT_ERROR;
	    message_id = gc_t_omx_class_format_error;
	    break;
	case (XOI_NO_CLASS_NAME):
	    retval = GC_T_OMX_NO_CLASS_NAME;
	    message_id = gc_t_omx_no_class_name;
	    break;
	case (XOI_DUPLICATE_CLASS_BLOCK):
	    retval = GC_T_OMX_DUPLICATE_CLASS_BLOCK;
	    message_id = gc_t_omx_duplicate_class_block;
	    break;
	case (XOI_CLASS_BLOCK_UNDEFINED):
	    retval = GC_T_OMX_CLASS_BLOCK_UNDEFINED;
	    message_id = gc_t_omx_class_block_undefined;
	    break;
	case (XOI_INVALID_ABBRV):
	    retval = GC_T_OMX_INVALID_ABBRV;
	    message_id = gc_t_omx_invalid_abbrv;
	    break;
	case (XOI_INVALID_OBJ_ID):
	    retval = GC_T_OMX_INVALID_OBJ_ID;
	    message_id = gc_t_omx_invalid_obj_id;
	    break;
	case (XOI_INVALID_CLASS_NAME):
	    retval = GC_T_OMX_INVALID_CLASS_NAME;
	    message_id = gc_t_omx_invalid_class_name;
	    break;
	case (XOI_INVALID_SYNTAX):
	    retval = GC_T_OMX_INVALID_SYNTAX;
	    message_id = gc_t_omx_invalid_syntax;
	    break;
	case (XOI_INVALID_PARAMETER):
	    retval = GC_T_OMX_INVALID_PARAMETER;
	    message_id = gc_t_omx_invalid_parameter;
	    break;
	case (XOI_MEMORY_INSUFFICIENT):
	    retval = GC_T_OMX_MEMORY_INSUFFICIENT;
	    message_id = gc_t_omx_memory_insufficient;
	    break;
	default:
	    retval = GC_T_OMX_UNKNOWN_ERROR;
	    message_id = gc_t_omx_unknown_error;
	    break;
    }

    /* allocate the size for the error vector. One more is required */
    /* to indicate end of vector.				    */
    *error_vect = (gc_t_vector *) malloc(sizeof(gc_t_vector) * 2);

    if (*error_vect != NULL)
    {
	/* initialize the first vector type to GC_T_ERR */
	(*error_vect)[0].type = GC_T_ERR;

	/* get the message string from the catalog */
	if (retval == GC_T_OMX_CANNOT_READ_SCHEMA)
	{
	    (*error_vect)[0].value = (char *) dce_sprintf(message_id, 
							  file_name);
	}
	else
	{
	    /* get the intermediate message string from catalog */
	    str = (char *) dce_sprintf(message_id);

	    if (str == NULL)
	    {
		(*error_vect)[0].value = NULL;
	    }
	    else
	    {
		(*error_vect)[0].value = (char *) dce_sprintf(
						 gc_t_omx_schema_file_error,
						 str, line_no, file_name);
		/* now free the intermediate string */
		free(str);
	    }
	}

	if ((*error_vect)[0].value == NULL)
	{
	    free(*error_vect);
	    retval = GC_T_MEMORY_INSUFFICIENT;
	    gc_t_create_gct_error_vector(retval, NULL, error_vect);
	}
	else
	{
	    /* initialize the second vector type to GC_T_EOL */
	    (*error_vect)[1].type = GC_T_EOL;
	    (*error_vect)[1].value = 0;
	}
    }
    else
    {
	retval = GC_T_MEMORY_INSUFFICIENT;
	gc_t_create_gct_error_vector(retval, NULL, error_vect);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_create_gct_arg_error_vector		   */
/* Description   : This function converts the GDSCP Translator     */
/*		   errors related to faulty arguments (parameters) */
/*		   to the appropriate error message string. 	   */
/* Return Value  : The error value defined by the Translator	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_create_gct_arg_error_vector(
    signed16      error,        /* IN  - The error to be converted            */
    char	 *append_str,   /* IN  - String to be appended to error string*/
    gc_t_vector **error_vect)   /* OUT - The converted error string vector    */
{
signed16 retval;

    switch(error)
    {
	case (GC_T_UNKNOWN_OPTION):
	case (GC_T_UNKNOWN_ARGUMENT):
	case (GC_T_CONFLICTING_OPTION):
	case (GC_T_INVALID_DIRECTORY_ID):
	case (GC_T_INVALID_TIME_LIMIT):
	case (GC_T_INVALID_SIZE_LIMIT):
	    /* create the error vector */
	    retval = gc_t_create_gct_error_vector(error, append_str, 
						  error_vect);
	    break;
	default:
	    /* create the error vector */
	    retval = gc_t_create_gct_error_vector(error, NULL, error_vect);
	    break;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
