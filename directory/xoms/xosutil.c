/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosutil.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:28  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:52:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:27:09  root]
 * 
 * Revision 1.1.2.4  1994/06/21  14:49:07  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:49:47  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:23:11  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:57:02  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:17:40  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:41  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:34:11  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  11:00:51  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosutil.c					   */
/* Description : This file contains all the utility routines used  */
/*		 by the convenience functions.			   */
/* Date	       : July 1, 1993					   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xosutil.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:28 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <nl_types.h>

#include <xom.h>
#include <xds.h>

#include <xoi.h>
#include <xomext.h>

#include <xosstruct.h>
#include <xosutil.h>


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_map_xoi_error				   */
/* Description   : This function maps the XOI error values to      */
/*		   the errors defined by the XOM convenience 	   */
/*		   functions.					   */
/* Return Value  : The mapped error value			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xos_map_xoi_error(
    OM_integer xoi_error)  /* IN - The XOI error to be mapped */
{
OM_integer retval;

    switch (xoi_error)
    {
	case (XOI_CANNOT_READ_SCHEMA):
	    retval = OMX_CANNOT_READ_SCHEMA;
	    break;
	case (XOI_SCHEMA_NOT_READ):
	    retval = OMX_SCHEMA_NOT_READ;
	    break;
	case (XOI_NO_START_OBJ_BLOCK):
	    retval = OMX_NO_START_OBJ_BLOCK;
	    break;
	case (XOI_NO_END_OBJ_BLOCK):
	    retval = OMX_NO_END_OBJ_BLOCK;
	    break;
	case (XOI_EMPTY_OBJ_BLOCK):
	    retval = OMX_EMPTY_OBJ_BLOCK;
	    break;
	case (XOI_OBJ_FORMAT_ERROR):
	    retval = OMX_OBJ_FORMAT_ERROR;
	    break;
	case (XOI_DUPLICATE_OBJ_ABBRV):
	    retval = OMX_DUPLICATE_OBJ_ABBRV;
	    break;
	case (XOI_DUPLICATE_OBJ_OBJ_ID):
	    retval = OMX_DUPLICATE_OBJ_OBJ_ID;
	    break;
	case (XOI_NO_START_ATTR_BLOCK):
	    retval = OMX_NO_START_ATTR_BLOCK;
	    break;
	case (XOI_NO_END_ATTR_BLOCK):
	    retval = OMX_NO_END_ATTR_BLOCK;
	    break;
	case (XOI_EMPTY_ATTR_BLOCK):
	    retval = OMX_EMPTY_ATTR_BLOCK;
	    break;
	case (XOI_ATTR_FORMAT_ERROR):
	    retval = OMX_ATTR_FORMAT_ERROR;
	    break;
	case (XOI_DUPLICATE_ATTR_ABBRV):
	    retval = OMX_DUPLICATE_ATTR_ABBRV;
	    break;
	case (XOI_DUPLICATE_ATTR_OBJ_ID):
	    retval = OMX_DUPLICATE_ATTR_OBJ_ID;
	    break;
	case (XOI_NO_START_CLASS_BLOCK):
	    retval = OMX_NO_START_CLASS_BLOCK;
	    break;
	case (XOI_NO_END_CLASS_BLOCK):
	    retval = OMX_NO_END_CLASS_BLOCK;
	    break;
	case (XOI_EMPTY_CLASS_BLOCK):
	    retval = OMX_EMPTY_CLASS_BLOCK;
	    break;
	case (XOI_CLASS_FORMAT_ERROR):
	    retval = OMX_CLASS_FORMAT_ERROR;
	    break;
	case (XOI_NO_CLASS_NAME):
	    retval = OMX_NO_CLASS_NAME;
	    break;
	case (XOI_DUPLICATE_CLASS_BLOCK):
	    retval = OMX_DUPLICATE_CLASS_BLOCK;
	    break;
	case (XOI_CLASS_BLOCK_UNDEFINED):
	    retval = OMX_CLASS_BLOCK_UNDEFINED;
	    break;
	case (XOI_INVALID_ABBRV):
	    retval = OMX_INVALID_ABBRV;
	    break;
	case (XOI_INVALID_OBJ_ID):
	    retval = OMX_INVALID_OBJ_ID;
	    break;
	case (XOI_INVALID_CLASS_NAME):
	    retval = OMX_INVALID_CLASS_NAME;
	    break;
	case (XOI_INVALID_SYNTAX):
	    retval = OMX_INVALID_SYNTAX;
	    break;
	case (XOI_MEMORY_INSUFFICIENT):
	    retval = OMX_MEMORY_INSUFFICIENT;
	    break;
	case (XOI_INVALID_PARAMETER):
	    retval = OMX_INVALID_PARAMETER;
	    break;
	case (XOI_UNKNOWN_ABBRV):
	    retval = OMX_UNKNOWN_ABBRV;
	    break;
	case (XOI_UNKNOWN_OBJ_ID):
	    retval = OMX_UNKNOWN_OBJ_ID;
	    break;
	case (XOI_UNKNOWN_OMTYPE):
	    retval = OMX_UNKNOWN_OMTYPE;
	    break;
	default:
	    retval = OMX_UNKNOWN_ERROR;
	    break;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_convert_to_normalized_string		   */
/* Description   : This function converts the input string to a    */
/*		   normalized string. All the special characters   */
/*		   and escape sequences in the input string are    */
/*		   processed.					   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_convert_to_normalized_string(
    char  	*in_str,     /* IN  - The input string		             */
    OM_uint32    in_len,     /* IN  - Length of the input string	     */
    char   	 quote_char, /* IN  - Quote character used to delimit values */
    char        *out_str,    /* OUT - Output converted string		     */
    OM_uint32   *out_len,    /* OUT - Length of the output string	     */
    OM_integer  *error_pos,  /* OUT - Error position in case of errors       */
    OM_integer  *error_type) /* OUT - Type of error in case of errors	     */
{
OM_return_code  retval;
OM_uint32	curr_index;
OM_uint32    	i, k;
OM_boolean  	quote_found;
OM_sint	     	d;

    curr_index = 0;
    /* Skip past white spaces */
    while (curr_index < in_len && isspace(in_str[curr_index]))
	++curr_index;

    if (curr_index == in_len)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_ATTR_VALUE;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    if (in_str[curr_index] == quote_char)
    {
	quote_found = OM_TRUE;
	++curr_index;
    }
    else
    {
	quote_found = OM_FALSE;
    }

    retval = OM_SUCCESS;
    for (i = 0; curr_index < in_len; ++curr_index)
    {
	/* If escape character */
	if (in_str[curr_index] == XOS_ESCAPE_OP)
	{
	    /* skip past XOS_ESCAPE_OP character */
	    ++curr_index;

	    /* If hexadecimal values specified ("\xnn") */
	    if (tolower(in_str[curr_index]) == XOS_HEX_OP)
	    {
		/* skip past XOS_HEX_OP character */
		++curr_index;

		/* validate the hex characters */
		if (!(isxdigit(in_str[curr_index]) && 
		    isxdigit(in_str[curr_index + 1])))
		{
		    retval = OM_WRONG_VALUE_MAKEUP;
		    break;
		}

		/* fetch the hexadecimal value */
		sscanf(&in_str[curr_index], "%2x", &d);
		out_str[i] = d;
		++curr_index;
	    }
	    else
	    {
		out_str[i] = in_str[curr_index];
	    }
	    ++i;
	    continue;
	}

        /* If quote character */
        if (in_str[curr_index] == quote_char)
	{
	    /* if open quote character is somewhere within the value */
	    if (quote_found == OM_FALSE)
	    {
		retval = OM_WRONG_VALUE_MAKEUP;
		break;
	    }
	    else
	    {
		break;
	    }
        }

	/* Ignore spaces if they are not within quotes */
	if (quote_found == OM_FALSE)
	{
	    if (isspace(in_str[curr_index]))
	    {
		for (k = curr_index + 1; k < in_len; ++k)
		{
		    if (!isspace(in_str[k]))
			break;
		}

		/* if spaces are not part of the data like in "henry mueller" */
		if (k == in_len)
		    break;
	    }
	}
	out_str[i] = in_str[curr_index];
	++i;
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_FORMAT_ERROR;
    }
    else
    {
	if (i == 0)
	{
	    *error_pos = curr_index + 1;
	    *error_type = OMX_MISSING_ATTR_VALUE;
	    retval = OM_WRONG_VALUE_MAKEUP;
	}
    }

    if (retval == OM_SUCCESS)
	*out_len = i; 

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_fill_descriptor   		   	   */
/* Description   : This function is used to initialize an          */
/*		   OM_descriptor structure with the user supplied  */
/*		   values for OM_type, OM_syntax and OM_value.     */
/*		   Additionally if the OM_syntax is of type	   */
/*		   OM_S_INTEGER or OM_S_BOOLEAN or OM_S_ENUMERATION*/
/*		   the passed elements pointer is also freed.      */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_fill_descriptor(
    OM_type     type,          /* IN  - Type of Object           */
    OM_syntax   syntax,	       /* IN  - Syntax of the object     */
    OM_uint32   length,	       /* IN  - Data length              */
    void       *elements,      /* IN  - Data Value               */
    OM_boolean  local_strings, /* IN  - To specify local strings */
    OM_object   destination)   /* OUT - The filled up descriptor */
{
OM_integer retval_xoi;
OM_string  obj_class_id;
OM_integer k;
OM_uint32  i;
char	   abbrv[XOS_MAX_OBJID + 1];
char	   s[XOS_MAX_VAR + 1];

    switch(syntax)
    {
	case (OM_S_INTEGER):
	case (OM_S_BOOLEAN):
	case (OM_S_ENUMERATION):
	    if (length > XOS_MAX_VAR)
		return(OM_WRONG_VALUE_MAKEUP);

	    strncpy(s, elements, length);
	    s[length] = 0;

	    i = atoi(s);

	    omX_fill(type, syntax, i, 0, destination);
	    break;

	case (OM_S_OBJECT_IDENTIFIER_STRING):
	    /* check if the attribute value is an abbreviation */
	    for (k = 0; k < length; ++k)
	    {
	    	if (!isalnum(((char *) elements)[k]))
		    break;
	    }

	    retval_xoi = XOI_UNKNOWN_ABBRV;

	    /* if attribute value is an abbreviation */
	    if (k == length)
	    {
		if (length > XOS_MAX_OBJID)
		    return(OM_WRONG_VALUE_MAKEUP);

		strncpy(abbrv, elements, length);

		/* Now append the End of String character */
		abbrv[length] = 0;

		/* Get the identifier for the object class from abbreviation */
		retval_xoi = xoi_get_objclass_objid(abbrv, 
						    &(obj_class_id.elements),
						    &(obj_class_id.length));

	    }

	    if (retval_xoi != XOI_SUCCESS)
	    {
		/* Attribute value is not a valid abbreviation. So fill */
		/* the OM_descriptor with user supplied attribute value.*/

		/* Malloc is done here because for attributes with */
		/* OM_S_OBJECT_IDENTIFIER syntax, the value is     */
		/* freed in the xos_free_attr_object function.     */
		obj_class_id.elements = malloc(sizeof(char) * length);
		if (obj_class_id.elements == NULL)
		    return(OM_MEMORY_INSUFFICIENT);

		memcpy(obj_class_id.elements, elements, length);
		obj_class_id.length = length;
	    }

	    /* Now fill the OM_descriptor */
	    omX_fill(type, syntax, obj_class_id.length, obj_class_id.elements, 
		     destination); 
	    break;

	default:
	    /* if local strings specified, set the high bit of the syntax */
	    if (local_strings == OM_TRUE)
	    {
		syntax |= OM_S_LOCAL_STRING;
	    }

	    /* Now fill the OM_descriptor */
	    omX_fill(type, syntax, length, elements, destination);
	    break;
    }

    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_convert_to_private			   */
/* Description   : This function converts a public object to an    */
/*		   XOM private object			   	   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_convert_to_private(
    OM_object		  public_obj,  /* IN  - Object to be converted */
    OM_object_identifier  class,       /* IN  - Class of the object    */
    OM_workspace          workspace,   /* IN  - The workspace          */
    OM_private_object    *private_obj) /* OUT - The converted object   */
{
OM_private_object  p_obj;
OM_return_code     retval;

    if ((retval = om_create(class, OM_FALSE, workspace, &p_obj)) == OM_SUCCESS)
    {
        retval = om_put(p_obj, OM_REPLACE_ALL, public_obj, 
		 	OM_NO_MORE_TYPES, 0, 0);
	if (retval == OM_SUCCESS)
	    *private_obj = p_obj;
    }
    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_map_xds_error_to_string			   */
/* Description   : This function converts the enumerated XDS error */
/*		   value to a string.			   	   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_map_xds_error_to_string(
    nl_catd         nls_desc,    /* IN  - Pointer to the message catalog */
    OM_enumeration  enum_error,  /* IN  - The enumerated error value     */
    OM_object       object,      /* IN  - DS_C_ATTRIBUTE_ERROR or NULL   */
    OM_string	   *string)      /* OUT - The converted string	         */
{
OM_value_position total_num;
OM_public_object  attr_type;
OM_return_code    retval;
OM_integer    	  retval_xoi;
OM_type		  type_list[] = {DS_ATTRIBUTE_TYPE, OM_NO_MORE_TYPES};
OM_uint    	  len;
char   		  str[XOS_MAX_VAR + 1];
char   		  full_name[XOI_MAX_NAME + 1];
char		 *error_string;
char  	         *def_msg;
int		  msg_no;

    str[0] = 0;
    full_name[0] = 0;

    switch (enum_error)
    {
	case (DS_E_ADMIN_LIMIT_EXCEEDED):
	    def_msg = "Operation cannot be performed within the administrative constraints.";
	    msg_no = 1;
	    break;
	case (DS_E_AFFECTS_MULTIPLE_DSAS):
	    def_msg = "Modification affects several DSAs and is prohibited.";
	    msg_no = 2;
	    break;
	case (DS_E_ALIAS_DEREFERENCING_PROBLEM):
	    def_msg = "An alias is encountered where an alias is not permitted.";
	    msg_no = 3;
	    break;
	case (DS_E_ALIAS_PROBLEM):
	    def_msg = "An alias is dereferenced that names an object that does not exist.";
	    msg_no = 4;
	    break;
	case (DS_E_ATTRIBUTE_OR_VALUE_EXISTS):
	    def_msg = "An attempt is made to add an attribute or value that already exists.";
	    msg_no = 5;
	    break;
	case (DS_E_BAD_ARGUMENT):
	    def_msg = "A bad argument was supplied.";
	    msg_no = 6;
	    break;
	case (DS_E_BAD_CLASS):
	    def_msg = "The OM Class of an argument is not supported for this operation.";
	    msg_no = 7;
	    break;
	case (DS_E_BAD_CONTEXT):
	    def_msg = "An invalid context argument was supplied.";
	    msg_no = 8;
	    break;
	case (DS_E_BAD_NAME):
	    def_msg = "An invalid name argument was supplied.";
	    msg_no = 9;
	    break;
	case (DS_E_BAD_SESSION):
	    def_msg = "An invalid session argument was supplied.";
	    msg_no = 10;
	    break;
	case (DS_E_BAD_WORKSPACE):
	    def_msg = "An invalid workspace argument was supplied.";
	    msg_no = 11;
	    break;
	case (DS_E_BUSY):
	    def_msg = "The Directory Service is temporarily too busy to perform the operation.";
	    msg_no = 12;
	    break;
	case (DS_E_CANNOT_ABANDON):
	    def_msg = "An attempt to abandon an operation has failed.";
	    msg_no = 13;
	    break;
	case (DS_E_CHAINING_REQUIRED):
	    def_msg = "Chaining is required to perform the operation, but is prohibited by the DS_CHAINING_PROHIBITED service control.";
	    msg_no = 14;
	    break;
	case (DS_E_COMMUNICATIONS_PROBLEM):
	    def_msg = "A communication error has occurred.";
	    msg_no = 15;
	    break;
	case (DS_E_CONSTRAINT_VIOLATION):
	    def_msg = "The attribute or attribute value does not conform to the imposed constraints.";
	    msg_no = 16;
	    break;
	case (DS_E_DIT_ERROR):
	    def_msg = "An inconsistency is detected in the DIT.";
	    msg_no = 17;
	    break;
	case (DS_E_ENTRY_EXISTS):
	    def_msg = "The name passed to the add entry operation already exists.";
	    msg_no = 18;
	    break;
	case (DS_E_INAPPROP_AUTHENTICATION):
	    def_msg = "The level of security attached to the requestor's credential is inconsistent.";
	    msg_no = 19;
	    break;
	case (DS_E_INAPPROP_MATCHING):
	    def_msg = "An attempt was made to use an undefined matching rule for the attribute type.";
	    msg_no = 20;
	    break;
	case (DS_E_INSUFFICIENT_ACCESS_RIGHTS):
	    def_msg = "The requestor does not have permission to perform the operation.";
	    msg_no = 21;
	    break;
	case (DS_E_INVALID_ATTRIBUTE_SYNTAX):
	    def_msg = "The attribute value does not conform to the attribute syntax of the attribute type.";
	    msg_no = 22;
	    break;
	case (DS_E_INVALID_ATTRIBUTE_VALUE):
	    def_msg = "The attribute value in the AVA does not conform to the attribute syntax.";
	    msg_no = 23;
	    break;
	case (DS_E_INVALID_CREDENTIALS):
	    def_msg = "The requester's credentials are invalid.";
	    msg_no = 24;
	    break;
	case (DS_E_INVALID_REF):
	    def_msg = "The DSA is unable to perform the request as directed.";
	    msg_no = 25;
	    break;
	case (DS_E_INVALID_SIGNATURE):
	    def_msg = "The signature affixed to the request is invalid.";
	    msg_no = 26;
	    break;
	case (DS_E_LOOP_DETECTED):
	    def_msg = "A DSA detected a loop within the directory.";
	    msg_no = 27;
	    break;
	case (DS_E_MISCELLANEOUS):
	    def_msg = "A miscellaneous error occurred while interacting with the Directory Service.";
	    msg_no = 28;
	    break;
	case (DS_E_MISSING_TYPE):
	    def_msg = "The attribute type is not included in the AVA.";
	    msg_no = 29;
	    break;
	case (DS_E_MIXED_SYNCHRONOUS):
	    def_msg = "An attempt was made to start a synchronous operation with outstanding asynchronous operations.";
	    msg_no = 30;
	    break;
	case (DS_E_NAMING_VIOLATION):
	    def_msg = "The modification leaves the DIT structured incorrectly.";
	    msg_no = 31;
	    break;
	case (DS_E_NO_INFO):
	    def_msg = "The request produced a security error for which no other information is available.";
	    msg_no = 32;
	    break;
	case (DS_E_NO_SUCH_ATTRIBUTE_OR_VALUE):
	    def_msg = "The specified attribute or value was not found in the directory entry.";
	    msg_no = 33;
	    break;
	case (DS_E_NO_SUCH_OBJECT):
	    def_msg = "The specified name does not match the name of any object in the directory.";
	    msg_no = 34;
	    break;
	case (DS_E_NO_SUCH_OPERATION):
	    def_msg = "The Directory Service has no knowledge of the operation that is to be abandoned.";
	    msg_no = 35;
	    break;
	case (DS_E_NOT_ALLOWED_ON_NON_LEAF):
	    def_msg = "Modification to an interior node of the DIT is prohibited.";
	    msg_no = 36;
	    break;
	case (DS_E_NOT_ALLOWED_ON_RDN):
	    def_msg = "The modification alters an object's RDN.";
	    msg_no = 37;
	    break;
	case (DS_E_NOT_SUPPORTED):
	    def_msg = "An attempt is made to use optional functionality, which is not available in this implementation.";
	    msg_no = 38;
	    break;
	case (DS_E_OBJECT_CLASS_MOD_PROHIB):
	    def_msg = "The modification alters an entry's object class attribute.";
	    msg_no = 39;
	    break;
	case (DS_E_OBJECT_CLASS_VIOLATION):
	    def_msg = "The modification leaves a directory entry inconsistent with its object class definition.";
	    msg_no = 40;
	    break;
	case (DS_E_OUT_OF_SCOPE):
	    def_msg = "The Directory cannot provide a referral or partial outcome qualifier within the required scope.";
	    msg_no = 41;
	    break;
	case (DS_E_PROTECTION_REQUIRED):
	    def_msg = "The Directory Service is unwilling to perform the operation because it is unsigned.";
	    msg_no = 42;
	    break;
	case (DS_E_TIME_LIMIT_EXCEEDED):
	    def_msg = "The operation could not be performed within the specified time limit.";
	    msg_no = 43;
	    break;
	case (DS_E_TOO_LATE):
	    def_msg = "The operation has already completed, either successfully or erroneously.";
	    msg_no = 44;
	    break;
	case (DS_E_TOO_MANY_OPERATIONS):
	    def_msg = "No more Directory Service operations can be performed.";
	    msg_no = 45;
	    break;
	case (DS_E_TOO_MANY_SESSIONS):
	    def_msg = "No more Directory Service sessions can be started.";
	    msg_no = 46;
	    break;
	case (DS_E_UNABLE_TO_PROCEED):
	    def_msg = "The DSA does not have the administrative authority over the particular naming context.";
	    msg_no = 47;
	    break;
	case (DS_E_UNAVAILABLE):
	    def_msg = "Some part of the directory is currently not available.";
	    msg_no = 48;
	    break;
	case (DS_E_UNAVAILABLE_CRIT_EXT):
	    def_msg = "The requested critical extensions are not available.";
	    msg_no = 49;
	    break;
	case (DS_E_UNDEFINED_ATTRIBUTE_TYPE):
	    def_msg = "The attribute type is undefined.";
	    msg_no = 50;
	    break;
	case (DS_E_UNWILLING_TO_PERFORM):
	    def_msg = "Some part of the Directory Service is not willing to perform the operation.";
	    msg_no = 51;
	    break;
	default:
	    def_msg = "Unknown XDS Error - ";
	    msg_no = 52;
	    sprintf(str, "<%d>.", enum_error);
	    break;
    }

    /* get the error message string from the NLS catalog */
    if (nls_desc != (nl_catd) -1)
    {
	error_string = catgets(nls_desc, 1, msg_no, def_msg);
    }
    else
    {
	error_string = def_msg;
    }

    if (strlen(error_string) + strlen(str) + 1 > string->length)
	return(OM_MEMORY_INSUFFICIENT);

    /* copy the message string */
    sprintf((char *) (string->elements), "%s%s", error_string, str);

    len = strlen(error_string) + strlen(str);

    /* if there is a DS_PROBLEMS object, try to append the full name */
    /* of the attribute that is violating.			     */
    if (object != NULL)
    {
	/* extract the DS_ATTRIBUTE_TYPE from the object */
	retval = om_get(object, 
			OM_EXCLUDE_ALL_BUT_THESE_TYPES + 
			OM_EXCLUDE_SUBOBJECTS,
			type_list, OM_TRUE, 0, 0, &attr_type, &total_num);

	if (retval == OM_SUCCESS)
	{
	    /* check if om_get has returned some object */
	    if (total_num > 0)
	    {
		/* get the full name of the attribute from obj.identifier */
		retval_xoi = xoi_get_attr_name(&(attr_type->value.string), NULL,
					       full_name);

		/* now delete the extracted object */
		om_delete(attr_type);

		if (retval_xoi == XOI_SUCCESS)
		{
		    def_msg = "Violating attribute -";

		    if (nls_desc != (nl_catd) -1)
		    {
			/* get the error message string from the NLS catalog */
			error_string = catgets(nls_desc, 1, 53, def_msg);
		    }
		    else
		    {
			error_string = def_msg;
		    }

		    if (len + strlen(full_name) + strlen(error_string) + 3 
			<= string->length)
		    {
			sprintf(&(((char *) (string->elements))[len]),
				"%s %s.", error_string, full_name);
		    }
		    len = len + strlen(error_string) + strlen(full_name) + 2;
		}
	    }
	}
    }

    string->length = len;

    return(OM_SUCCESS);
}

/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
