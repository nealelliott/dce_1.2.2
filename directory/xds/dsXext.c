/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsXext.c,v $
 * Revision 1.1.4.2  1996/02/18  18:25:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:47  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:33:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:44  root]
 * 
 * Revision 1.1.2.4  1994/06/10  21:18:55  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:23  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  16:04:02  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:25  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  15:48:59  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:04  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:22:37  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:13:22  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:55:25  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XDS Convenience Functions                         */
/* File Name   : dsXext.c					   */
/* Description : This file contains all the routines for the       */
/*		 public XDS convenience functions	   	   */
/* Date	       : July 1, 1993					   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsXext.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:25:19 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>

#include <xom.h>
#include <xds.h>

#include <xdsext.h>
#include <dsXext.h>


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : dsX_extract_attr_values			   		       */
/* Description   : This function is used to extract Attribute      */
/*		   		   values corresponding to the specified Attribute */
/*		   		   Type from an object.				   			   */
/*		   		   The memory space for the returned objects is    */
/*		   		   provided by this function.			   		   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   		   the type of error				   			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	       */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code dsX_extract_attr_values(
    OM_private_object     object,	      /* IN  - The source object 	      */
    OM_object_identifier  attribute_type, /* IN  - Attribute to be extracted  */
    OM_boolean			  local_strings,  /* IN  - Local strings required     */
    OM_public_object     *values,	      /* OUT - Extracted Attribute Values */
    OM_value_position    *total_number)   /* OUT - Number of extracted values */
{
OM_value_position  attr_count;
OM_value_position  count;
OM_return_code     retval;
OM_public_object   attr_list;
OM_public_object   attr_type;
OM_public_object   attr_value;
OM_type		       type_list[] = {OM_NO_MORE_TYPES, OM_NO_MORE_TYPES};
OM_sint			   i;

    /* Validate the parameters. The OM_private_object is validated */
    /* by the om_get function.					   				   */
    if (attribute_type.elements == NULL || attribute_type.length == 0 ||
		values == NULL || total_number == NULL) {
		retval = OM_POINTER_INVALID;
		goto end;
    } /* endif */

    *values = NULL;
    *total_number = 0;

    type_list[0] = DS_ATTRIBUTES;
    retval = om_get(object, 
		    		OM_EXCLUDE_ALL_BUT_THESE_TYPES + OM_EXCLUDE_SUBOBJECTS,
		    		type_list, OM_FALSE, OM_ALL_VALUES, OM_ALL_VALUES, 
		    		&attr_list, &attr_count);

    if (retval != OM_SUCCESS) {
		goto end;
	} /* endif */

    /* check if om_get has returned some object */
    if (attr_count <= 0) {
		retval = OM_NO_SUCH_OBJECT;
		goto end;
	} /* endif */
	
    type_list[0] = DS_ATTRIBUTE_TYPE;

    /* Search the list for the required attribute type */
    for (i = 0; i < attr_count; ++i) {
		retval = om_get(attr_list[i].value.object.object,
						OM_EXCLUDE_ALL_BUT_THESE_TYPES + OM_EXCLUDE_SUBOBJECTS,
						type_list, OM_FALSE, OM_ALL_VALUES, OM_ALL_VALUES, 
						&attr_type, &count);

		if (retval != OM_SUCCESS) {
			break;
		} /* endif */

		/* check if om_get has returned something */
		if (count <= 0) {
			retval = OM_NO_SUCH_OBJECT;
			break;
		} /* endif */

		/* Attribute Type found */
		if (attr_type->value.string.length == attribute_type.length &&
			memcmp(attr_type->value.string.elements, attribute_type.elements,
			   	   attribute_type.length) == 0) {
			om_delete(attr_type);
			retval = OM_SUCCESS;
			break;
		} /* endif */

		om_delete(attr_type);
		retval = OM_NOT_PRESENT;
    } /* endfor */

    if (retval == OM_SUCCESS) {
		type_list[0] = DS_ATTRIBUTE_VALUES;

		/* Get the Attribute Value from the corresponding item in the */
		/* attribute list.					      					  */
	    retval = om_get(attr_list[i].value.object.object,
						OM_EXCLUDE_ALL_BUT_THESE_TYPES,
						type_list, local_strings, OM_ALL_VALUES, OM_ALL_VALUES, 
						&attr_value, &count);
    } /* endif */

    om_delete(attr_list);

    if (retval != OM_SUCCESS) {
		goto end;
	} /* endif */

    /* check if om_get has returned something */
    if (count <= 0) {
		retval = OM_NO_SUCH_OBJECT;
		goto end;
	} /* endif */

    *values = attr_value;
    *total_number = count;
	retval = OM_SUCCESS;

end:
    return(retval);
} /* end dsX_extract_attr_values */
