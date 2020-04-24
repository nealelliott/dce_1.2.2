/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omXext.c,v $
 * Revision 1.1.4.2  1996/02/18  18:25:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:41:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:14  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:21:45  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:26  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:21  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:14  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:54  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:13:38  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:55:44  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : omXext.c					   					   */
/* Description : This file contains all the public XOM convenience */
/*		 		 library functions	   			   				   */
/* Date	       : July 1, 1993					   				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omXext.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:25:52 $";
#endif

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <stdio.h>

#include <xom.h>

#include <xosstruct.h>
#include <xos.h>

#include <xomext.h>
#include <omXext.h>


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : omX_fill					   					   */
/* Description   : This function is used to initialize an	   	   */
/*		   		   OM_descriptor structure with the user supplied  */
/*		   		   values for OM_type, OM_syntax and OM_value	   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   		   the type of error				   			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code omX_fill(
    OM_type	      	  type,        /* IN  - Type of Object	     	 */
    OM_syntax	      syntax,      /* IN  - Syntax of the object     */
    OM_uint32	      length,      /* IN  - Data length		     	 */
    void	     	 *elements,    /* IN  - Data Value 		     	 */
    OM_descriptor    *destination) /* OUT - The filled up descriptor */
{
    if (destination == NULL) {
	    return(OM_POINTER_INVALID);
    }

    destination->type = type;
    destination->syntax = syntax;
    destination->value.string.length = length;
    destination->value.string.elements = elements;

    return(OM_SUCCESS);

} /* end omX_fill */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : omX_fill_oid					   				   */
/* Description   : This function is used to initialize an	   	   */
/*		   		   OM_descriptor structure with the user supplied  */
/*		   		   values for OM_type and OM_value	   	   		   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   		   the type of error				   			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	       */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code omX_fill_oid(
    OM_type	          	  type,        /* IN  - Type of Object	         */
    OM_object_identifier  object_id,   /* IN  - Value of the object      */
    OM_descriptor        *destination) /* OUT - The filled up descriptor */
{
    if (destination == NULL) {
		return(OM_POINTER_INVALID);
    }

    destination->type = type;
    destination->syntax = OM_S_OBJECT_IDENTIFIER_STRING;
    destination->value.string.length = object_id.length;
    destination->value.string.elements = object_id.elements;

    return(OM_SUCCESS);
} /* end omX_fill_oid */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : omX_extract					   				   */
/* Description   : This function is used to extract the first      */
/*		   		   occurrence of the requested OM_types from the   */
/*		   		   specified Object or any of its subobjects.	   */
/*		   		   The memory space for the returned OM_object is  */
/*		   		   provided by this function.			   		   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   		   the type of error				   			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code omX_extract(
    OM_private_object  object,          /* IN  - Extract from this object    */
    OM_type_list       navigation_path, /* IN  - Leads to the target object  */
    OM_exclusions      exclusions,      /* IN  - Scope of extraction         */
    OM_type_list       included_types,  /* IN  - Objects to be extracted     */
    OM_boolean         local_strings,   /* IN  - Local strings required      */
    OM_value_position  initial_value,   /* IN  - First value to be extracted */
    OM_value_position  limiting_value,  /* IN  - Last value to be extracted  */
    OM_public_object  *values,	 		/* OUT - Array of extracted objects  */
    OM_value_position *total_number)	/* OUT - Count of extracted objects  */
{
OM_value_position count;
OM_return_code    retval;
OM_object	  	  obj_in, obj_out;
OM_type		  	  type_list[] = {OM_NO_MORE_TYPES, OM_NO_MORE_TYPES};

    /* Validate the parameters. The remaining parameters are validated */
    /* by the om_get function.					       				   */
    if (navigation_path == NULL || values == NULL  || total_number == NULL) {
		return(OM_POINTER_INVALID);
    } /* endif */

    retval = OM_SUCCESS;
    *values = NULL; 
    *total_number = 0;

    /* No navigation path specified */
    if (navigation_path[0] == OM_NO_MORE_TYPES) {
		retval = OM_WRONG_VALUE_TYPE;
	} else {
		/* Initialize the type_list */
		type_list[0] = navigation_path[0];

		retval = om_get(object, 
						OM_EXCLUDE_ALL_BUT_THESE_TYPES + OM_EXCLUDE_SUBOBJECTS,
						type_list, local_strings, OM_ALL_VALUES, OM_ALL_VALUES, 
						&obj_in, &count);

		if (retval == OM_SUCCESS) {
			/* check if om_get has returned some object */
			if (count <= 0) {
				retval = OM_NOT_PRESENT;
			} else {
				OM_sint		  i;
				/* Navigate to the required object */
				for (i = 1; navigation_path[i] != OM_NO_MORE_TYPES; ++i) {
					type_list[0] = navigation_path[i];

					/* Check the syntax. It should be of type OM_S_OBJECT */
					if (!XOS_OBJECT(obj_in)) {
						om_delete(obj_in);
						retval = OM_NOT_PRESENT;
						break;
					} /* endif */

					retval = om_get(obj_in->value.object.object, 
								    OM_EXCLUDE_ALL_BUT_THESE_TYPES + 
								    OM_EXCLUDE_SUBOBJECTS, 
								    type_list, local_strings, OM_ALL_VALUES, 
								    OM_ALL_VALUES, &obj_out, &count);

					/* Free the object now */
					om_delete(obj_in);

					if (retval == OM_SUCCESS) {
						/* check if om_get has returned some object */
						if (count <= 0) {
							retval = OM_NOT_PRESENT;
							break;
						} /* endif */
					} else {
						break;
					} /* endif */

					obj_in = obj_out;
				} /* endfor */
			} /* endif */
		} /* endif */
	} /* endif */

	if (retval == OM_SUCCESS) {
	    /* Check the syntax. It should be of type OM_S_OBJECT */
		if (!XOS_OBJECT(obj_in)) {
			retval = OM_NOT_PRESENT;
		} else {
			/* Now extract the required data */
			retval = om_get(obj_in->value.object.object, exclusions, 
					included_types, local_strings, initial_value, 
					limiting_value, values, total_number);

			if (retval == OM_SUCCESS) {
				/* check if om_get has returned something */
				if (*total_number <= 0) {
					retval  = OM_NOT_PRESENT;
				} /* endif */
			} /* endif */
		} /* endif */

		om_delete(obj_in);
    } /* endif */

    return(retval);
} /* end omX_extract */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : omX_string_to_object				   			   */
/* Description   : This function is used to convert an OM object   */
/*		   		   specified in string format to its OM descriptor */
/*		   		   format.					   					   */
/*		   		   The memory space for the returned OM_object is  */
/*		   		   provided by this function.			   		   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   		   the type of error				   			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	       */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code omX_string_to_object(
    OM_workspace          workspace,     /* IN  - The workspace		          */
    OM_string            *string,        /* IN  - The string to be converted  */
    OM_object_identifier  class,         /* IN  - The OM Class to be created  */
    OM_boolean            local_strings, /* IN  - Local strings specified     */
    OM_private_object    *object,        /* OUT - The converted Object	      */
    OM_integer	         *error_position,/* OUT - Error Position in I/P string*/
    OM_integer	         *error_type)    /* OUT - Type of error 	          */
{
    /* Validate the parameters*/
    if (string == NULL || object == NULL || error_position == NULL || 
	    error_type == NULL || workspace == NULL) {
		return(OM_POINTER_INVALID);
    } /* endif */

    if (string->length == 0) {
	    /* create an object with only the class descriptor */
		return(om_create(class, OM_FALSE, workspace, object));
    } else {
		if (string->elements != NULL) {
			return(xos_string_to_object(workspace, string, class, local_strings,
									    object, error_position, error_type));
		} else {
			return(OM_POINTER_INVALID);
		} /* endif */
    } /* endif */
} /* end omX_string_to_object */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : omX_object_to_string				   			   */
/* Description   : This function is used to convert an OM Object   */
/*		   		   to its string format. The object can either be  */
/*		   		   a private object, a service generated public    */
/*		   		   object or a client generated object.		       */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   		   the type of error				   			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code omX_object_to_string(
    OM_object    object,        /* The Object to be converted          */
    OM_boolean	 local_strings, /* To indicate local string conversion */
    OM_string	*string)        /* The converted DN string 	           */
{
    /* Validate the parameters */
    if (object == NULL || object[0].type == OM_NO_MORE_TYPES || 
		string == NULL || string->elements == NULL || string->length == 0) {
		return(OM_POINTER_INVALID);
    } /* endif */

    return(xos_object_to_string(object, local_strings, string));
} /* end omX_object_to_string */
