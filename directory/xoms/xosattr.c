/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosattr.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:19  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:32  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:51:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:51  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:22:58  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:52  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:32  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:56  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:29  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:27:19  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:59:33  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosattr.c					   */
/* Description : This file contains all the routines for string    */
/*		 to object and object to string conversions	   */
/*	         Additionally some more routines handling 	   */
/*		 structured attributes are contained in xosclass.c */
/* Date	       : July 1, 1993					   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xosattr.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:19 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <xom.h>
#include <xds.h>

#include <xoi.h>
#include <xomext.h>

#include <xosstruct.h>
#include <xosutil.h>
#include <xosname.h>
#include <xosclass.h>
#include <xosattr.h>

#ifdef XOS_DEBUG
#include <xosdebug.h>
#endif


/* Import the necessary Object Identifier constants */ 
OM_IMPORT(dsP_DS_C_ATTRIBUTE)
OM_IMPORT(dsP_DS_C_DS_DN)
OM_IMPORT(dsP_DS_C_FILTER_ITEM)

/*-- Static Function(s) -------------------------------------------*/
static OM_return_code xos__get_normal_attr_value_len(
    char	*value,	      /* IN  - Attribute Info string	        */
    OM_uint32    value_len,   /* IN  - Length of Attribute Info string  */
    OM_uint32	*len,         /* OUT - Length of the 1st attribute value*/
    OM_integer	*error_pos,   /* OUT - Error position in case of errors */
    OM_integer	*error_type); /* OUT - Type of error in case of errors  */

static OM_return_code xos__get_normal_attr_value_count(
    char	*value,	      /* IN  - Attribute Value string	        */
    OM_uint32    value_len,   /* IN  - Length of Value string	        */
    OM_uint 	*count,       /* OUT - Count of total number of values  */
    OM_integer	*error_pos,   /* OUT - Error position in case of errors */
    OM_integer	*error_type); /* OUT - Type of error in case of errors  */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_to_public_attr_object		   	   */
/* Description   : This function is used to convert the Attribute  */
/*		   type/value information in the string format to  */
/*		   a public object (DS_C_ATTRIBUTE). 		   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_to_public_attr_object(
    OM_string  *attr_string,   /* IN  - The string to be converted            */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *attr_obj,      /* OUT - The converted public object           */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors       */
{
OM_return_code   retval;
OM_integer	 retval_xoi;
OM_syntax	 omsyntax;
OM_string	 objid;
OM_string	 omclass;
OM_uint32	 len, curr_index;
OM_uint		 j;
char		 attr[XOS_MAX_OBJID + 1];
char		*str;

#ifdef XOS_DEBUG
    xos_debug_string("xos_to_public_attr_object : ENTRY", 0, 1);
    xos_debug_string("Input Attr. String:", 0, 0);
    xos_debug_string((char *)(attr_string->elements), attr_string->length, 0);
#endif

    omclass.elements = NULL;

    len = attr_string->length;
    str = (char *) (attr_string->elements);

    curr_index = 0;
    /* skip past white spaces */
    while (curr_index < len && isspace(str[curr_index]))
	++curr_index;

    if (curr_index == len)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_ATTR_INFO;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    *error_pos = curr_index + 1;

    /* Get the object identifier string or the abbreviation */
    for (j = 0; curr_index < len; ++curr_index, ++j)
    {
	if (j > XOS_MAX_OBJID)
	{
	    *error_type = OMX_UNKNOWN_ABBRV;
	    return(OM_WRONG_VALUE_MAKEUP);
	}

	/* Abbreviation string is an alpha-numeric string or an */
	/* object identifier string in dotted representation.   */
	if (!(isalnum(str[curr_index]) || str[curr_index] == '.'))
	    break;

	attr[j] = str[curr_index];
    }

    if (j == 0)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_ABBRV;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    /* Append the NULL character */
    attr[j] = 0;

    /* skip past white spaces */
    while (curr_index < len && isspace(str[curr_index]))
	++curr_index;

    /* check if XOS_ATTR_EQUALITY_OP is present */
    if (str[curr_index] != XOS_ATTR_EQUALITY_OP)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_EQUAL_OPERATOR;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    /* skip past XOS_ATTR_EQUALITY_OP character */
    ++curr_index;

    retval_xoi = xoi_get_attr_details(attr, &omsyntax, 
				      &(objid.elements), &(objid.length), 
				      &(omclass.elements), &(omclass.length));

    if (retval_xoi != XOI_SUCCESS)
    {
	/* error_pos is not initialized here, because it would have */
	/* been set properly.					    */
	*error_type = xos_map_xoi_error(retval_xoi);
	return(OM_WRONG_VALUE_MAKEUP);
    }

    len -= curr_index;

    /* if structured attribute */
    if (omsyntax == OM_S_OBJECT)
    {
	/* Create the structured attribute public object */
	retval = xos_create_struct_attr_obj(&objid, &omclass, &str[curr_index],
					    len, val_mem_top, OM_FALSE, 
					    local_strings, attr_obj, 
					    error_pos, error_type);

	/* If the object is of type DS_C_DS_DN, free the omclass since the  */
	/* memory space would be provided by xos_convert_str_to_public_name */
	/* function.   							    */
	if ((retval != OM_SUCCESS) || 
	    (omclass.length == dsP_DS_C_DS_DN.length &&
	     memcmp(omclass.elements, dsP_DS_C_DS_DN.elements, 
		   dsP_DS_C_DS_DN.length) == 0))
	{
	    free(omclass.elements);
	}
    }
    else
    {
	/* Create the public object for the attribute */
	retval = xos_create_normal_attr_obj(&objid, omsyntax, &str[curr_index],
					    len, val_mem_top, OM_FALSE, 
					    local_strings, attr_obj, 
					    error_pos, error_type);
    }

    if (retval != OM_SUCCESS)
    {
	free(objid.elements);
	*error_pos += curr_index;
    }

#ifdef XOS_DEBUG
    xos_debug_string("xos_to_public_attr_object : EXIT", 0, -1);
#endif

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_create_normal_attr_obj			   */
/* Description   : This function is used to create the XOM object  */
/*		   for a normal attribute.  The input is a string  */
/*		   with attribute value information. 		   */
/*		   Eg: "783929;794848".    			   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_create_normal_attr_obj(
    OM_string  *objid,         /* IN  - Attribute Object Identifier           */
    OM_syntax   omsyntax,      /* IN  - Syntax of the Attribute	              */
    char       *value,	       /* IN  - Attribute Value string	              */
    OM_uint32   value_len,     /* IN  - Length of Value string	              */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  is_filter,     /* IN  - Boolean flag to indicate filter item  */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *attr_obj,      /* OUT - The created object		      */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors       */
{
OM_return_code  retval;
OM_object	object;
OM_uint32       len;
OM_uint32       curr_index;
OM_uint32	a_val_len;
OM_uint		tot_desc;
OM_uint         val_count;
OM_uint		i;

#ifdef XOS_DEBUG
char		debug_msg[256];
#endif

#ifdef XOS_DEBUG
    xos_debug_string("xos_create_normal_attr_obj : ENTRY", 0, 1);
    xos_debug_string("Input Attr.String:", 0, 0);
    xos_debug_string(value, value_len, 0);
#endif

    /* Get the total count of attribute values */
    retval = xos__get_normal_attr_value_count(value, value_len, &val_count, 
				              error_pos, error_type);

    if (retval != OM_SUCCESS)
	return(retval);

#ifdef XOS_DEBUG
    sprintf(debug_msg, "Total number of values = <%d>", val_count);
    xos_debug_string(debug_msg, 0, 0);
#endif

    /* If the attribute object is going to be used as a DS_C_FILTER_ITEM */
    if (is_filter == OM_TRUE)
    {
	/* Compute the total number of OM_descriptor structures. */
	/* 4 more descriptors are required for DS_C_FILTER_ITEM, */
	/* DS_ATTRIBUTE_TYPE, DS_FILTER_ITEM_TYPE,  and the      */
	/* OM_NULL_DESCRIPTOR.                                   */
	tot_desc = 4 + val_count;
    }
    else
    {
	/* Compute the total number of OM_descriptor structures. */
	/* 3 more descriptors are required for DS_C_ATTRIBUTE,   */
	/* DS_ATTRIBUTE_TYPE and the OM_NULL_DESCRIPTOR.	 */ 
	tot_desc = 3 + val_count;
    }

    /* Allocate the memory for the OM_descriptor structures */
    object = (OM_descriptor *) malloc(sizeof(OM_descriptor) * tot_desc);

    if (object == NULL)
    {
	*error_type = OMX_MEMORY_INSUFFICIENT;
	return(OM_MEMORY_INSUFFICIENT);
    }

    /* If the attribute object is going to be used as a DS_C_FILTER_ITEM */
    if (is_filter == OM_TRUE)
    {
	/* Initialize the first OM_descriptor to DS_C_FILTER_ITEM */
	omX_fill_oid(OM_CLASS, dsP_DS_C_FILTER_ITEM, &object[0]);
    }
    else
    {
	/* Initialize the first OM_descriptor to DS_C_ATTRIBUTE */
	omX_fill_oid(OM_CLASS, dsP_DS_C_ATTRIBUTE, &object[0]);
    }

    /* Initialize the second OM_descriptor to DS_ATTRIBUTE_TYPE */
    omX_fill_oid(DS_ATTRIBUTE_TYPE, *objid, &object[1]);

    /* Initialize all the others to NULL descriptors */
    for (i = 2; i < tot_desc; ++i)
    {
	/* If the attribute object is going to be used as DS_C_FILTER_ITEM */
	/* Initialize the penultimate descriptor to DS_FILTER_ITEM_TYPE.   */
	if (is_filter == OM_TRUE && i == tot_desc - 2)
	{
	    omX_fill(DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, DS_EQUALITY, 
		     NULL, &object[i]);
	}
	else
	{
	    XOS_FILL_OMD_NULL(&object[i]);
	}
    }
    
    curr_index = 0;

    for (i = 0; i < val_count; ++i)
    {
	/* get the length of the attribute value */
	retval = xos__get_normal_attr_value_len(&value[curr_index], value_len, 
						&len, error_pos, error_type); 

	if (retval != OM_SUCCESS)
	    break;

#ifdef XOS_DEBUG
    sprintf(debug_msg, "Length of read attribute value = <%d>", len);
    xos_debug_string(debug_msg, 0, 0);
#endif

	/* Process the reserved characters in the attribute value and */
	/* create a normalized string.				      */
	retval = xos_convert_to_normalized_string(&value[curr_index], len, 
						  XOS_ATTR_QUOTE_OP,
						  val_mem_top, &a_val_len, 
						  error_pos, error_type);
	if (retval != OM_SUCCESS)
	    break;

#ifdef XOS_DEBUG
    xos_debug_string("Normalized attribute value:", 0, 0);
    xos_debug_string(val_mem_top, a_val_len, 0);
#endif

	/* fill the OM_descriptor with the attribute value */
	retval = xos_fill_descriptor(DS_ATTRIBUTE_VALUES, omsyntax, a_val_len, 
				     val_mem_top, local_strings, &object[i+2]);

	if (retval != OM_SUCCESS)
	{
	    *error_type = OMX_WRONG_VALUE;
	    break;
	}

	/* skip past the current attribute value and the */
	/* XOS_RECUR_OP character 			 */
	curr_index = curr_index + len + 1;
	value_len = value_len - len - 1;

	/* move to the next available memory location */
	val_mem_top += a_val_len;
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos += curr_index;
	free(object);
	return(retval);
    }

    *attr_obj = object;

#ifdef XOS_DEBUG
    xos_debug_string("xos_create_normal_attr_obj : EXIT", 0, -1);
#endif

    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__get_normal_attr_value_count		   */
/* Description   : This function is used to scan a normal Attribute*/
/*		   value string and return the count of the number */
/*		   of attribute values.	The input is a string with */
/*		   attribute information. Eg: "783929;794848"  	   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__get_normal_attr_value_count(
    char	*value,	      /* IN  - Attribute Value string	        */
    OM_uint32    value_len,   /* IN  - Length of Value string	        */
    OM_uint 	*count,       /* OUT - Count of total number of values  */
    OM_integer	*error_pos,   /* OUT - Error position in case of errors */
    OM_integer	*error_type)  /* OUT - Type of error in case of errors  */
{
OM_return_code  retval;
OM_uint32       attr_len, len;
OM_uint32 	curr_index;
OM_uint   	tot_count;

    curr_index = 0;
    tot_count = 0;
    len = value_len;

    /* Compute the total number of attribute values */
    while (curr_index < value_len)
    {
	/* get the length of the current attribute value */
	retval = xos__get_normal_attr_value_len(&value[curr_index], len, 
						&attr_len, error_pos, 
						error_type); 
	if (retval != OM_SUCCESS)
	{
	    *error_pos += curr_index;
	    return(retval);
	}

	/* skip past the current attribute value */
	curr_index += attr_len;
	len -= attr_len;

	/* skip past XOS_RECUR_OP character */
        if (len > 0)
	{
	    ++curr_index;
	    --len;
	}

	++tot_count;
    }

    if (tot_count == 0)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_ATTR_VALUE;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    *count = tot_count;
    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__get_normal_attr_value_len		   */
/* Description   : This function is used to scan the Attribute     */
/*		   value string for a normal Attribute and return  */
/*		   the length of the first encountered attribute   */
/*		   value. The input is a string with attribute     */
/*		   information. Eg: "783929;794848"	   	   */
/*		   The end of the attribute value would be the     */
/*		   character just before the XOS_RECUR_OP or       */
/*		   the end of string.				   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__get_normal_attr_value_len(
    char	*value,	     /* IN  - Attribute Info string	       */
    OM_uint32    value_len,  /* IN  - Length of Attribute Info string  */
    OM_uint32	*len,        /* OUT - Length of the 1st attribute value*/
    OM_integer	*error_pos,  /* OUT - Error position in case of errors */
    OM_integer	*error_type) /* OUT - Type of error in case of errors  */
{
OM_return_code  retval;
OM_uint32   	curr_index;
OM_boolean 	open_quote_found, close_quote_found;

    curr_index = 0;
    /* skip past white spaces */
    while (curr_index < value_len && isspace(value[curr_index]))
	++curr_index;

    if (curr_index == value_len || value[curr_index] == XOS_RECUR_OP)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_ATTR_VALUE;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    open_quote_found = close_quote_found = OM_FALSE;
    retval = OM_SUCCESS;
    for (; curr_index < value_len; ++curr_index)
    {
	/* if escape operator */
	if (value[curr_index] == XOS_ESCAPE_OP)
	{
	    /* if the escape character is the last data character */
	    if (curr_index == value_len - 1)
	    {
		retval = OM_WRONG_VALUE_MAKEUP;
		break;
	    }

	    /* skip through escape character */
	    ++curr_index;

	    /* If hexadecimal values specified ("\xnn") */
	    if (tolower(value[curr_index]) == XOS_HEX_OP)
	    {
		if (curr_index + 2 > value_len - 1)
		{
		    retval = OM_WRONG_VALUE_MAKEUP;
		    break;
		}

		/* Skip past the hexadecimal characters */
		curr_index += 2;
	    }
	    continue;
	}

	/* If quote character */
	if (value[curr_index] == XOS_ATTR_QUOTE_OP)
	{
	    /* if additional quote character found */
	    if (close_quote_found == OM_TRUE)
	    {
		retval = OM_WRONG_VALUE_MAKEUP;
		break;
	    }
	    else
	    {
		if (open_quote_found == OM_FALSE)
		    open_quote_found = OM_TRUE;
		else
		    close_quote_found = OM_TRUE;
	    }
	    continue;
	}

	/* The XOS_RECUR_OP is not interpreted within quotes */
	if (open_quote_found == OM_FALSE || close_quote_found == OM_TRUE)
	{
	    if (value[curr_index] == XOS_RECUR_OP)
		break;
	}

	/* if there is some more non space data after closed quotes */
	if (close_quote_found == OM_TRUE && (!isspace(value[curr_index])))
	{
	    retval = OM_WRONG_VALUE_MAKEUP;
	    break;
	}
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_FORMAT_ERROR;
    }
    else
    {
	if (open_quote_found == OM_TRUE && close_quote_found == OM_FALSE)
	{
	    *error_pos  = curr_index + 1;
	    *error_type = OMX_MISMATCHED_QUOTES;
	    retval = OM_WRONG_VALUE_MAKEUP;
	}
	else
	{
	    /* if there is no more data after XOS_RECUR_OP character */
	    if (value[curr_index] == XOS_RECUR_OP && 
		curr_index + 1 == value_len)
	    {
		*error_pos = curr_index + 2;
		*error_type = OMX_MISSING_ATTR_VALUE;
		retval = OM_WRONG_VALUE_MAKEUP;
	    }
	}
    }

    if (retval == OM_SUCCESS)
	*len = curr_index;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_free_attr_object			   	   */
/* Description   : This function is used to free the memory space  */
/*		   ocupied by the DS_C_ATTRIBUTE object created    */
/*		   for normal attributes. 	   		   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xos_free_attr_object(
    OM_object attr_object)  /* IN - The object to be freed */
{
OM_boolean   class_deleted = OM_FALSE;
OM_object    p;
OM_uint      i;

    /* skip the first descriptor which is DS_C_ATTRIBUTE */
    for (i = 1; attr_object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	/* free the object identifier */
	if (attr_object[i].type == DS_ATTRIBUTE_TYPE)
	{
	    free(attr_object[i].value.string.elements);
	    continue;
	}

	if (attr_object[i].type == DS_ATTRIBUTE_VALUES &&
	    attr_object[i].syntax == OM_S_OBJECT_IDENTIFIER_STRING)
	{
	    /* The value is freed only if the attribute syntax is */
	    /* OM_S_OBJECT_IDENTIFIER_STRING.			  */
	    free(attr_object[i].value.string.elements);
	    continue;
	}

	if (attr_object[i].syntax == OM_S_OBJECT)
	{
	    p = attr_object[i].value.object.object;

	    if (p[0].type == OM_NO_MORE_TYPES)
		continue;

	    /* If object is of type DS_C_DS_DN */
	    if (p[0].value.string.length == dsP_DS_C_DS_DN.length &&
		memcmp(p[0].value.string.elements, dsP_DS_C_DS_DN.elements, 
		       dsP_DS_C_DS_DN.length) == 0)
	    {
		xos_free_dn_object(p);
		continue;
	    }
	    else
	    {
		/* The class identifier memory space is shared by */
		/* all the recurring values of the class. So it   */
		/* must be deleted only once.			  */
		if (class_deleted == OM_FALSE)
		{
		    /* free the class identifier */
		    free(p[0].value.string.elements);
		    class_deleted = OM_TRUE;
		}
		xos_free_class_object(p);
	    }
	}
    }

    /* Now free the object */
    free(attr_object);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_convert_public_attr_to_str		   */
/* Description   : This function is used to convert an XOM public  */
/*		   object (DS_C_ATTRIBUTE) to a string format. 	   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_convert_public_attr_to_str(
    OM_object   object, 	/* IN  - The Object to be converted          */
    OM_boolean  local_strings,  /* IN  - To indicate local string conversion */
    OM_string  *string) 	/* OUT - The converted string       	     */
{
OM_value_position  total_num;
OM_return_code     retval;
OM_integer	   retval_xoi;
OM_boolean	   delete_sub_object;
OM_object	   p, sub_object;
OM_syntax	   syntax;
OM_string	   dn_string;
OM_uint32          index, len_used, k;
OM_uint	           val_count;
OM_uint	           i, len;
char	           abbrv[XOS_MAX_OBJID + 1];
char		   c[XOS_MAX_VAR + 1];
char              *str;

    str = (char *) (string->elements);
    index = 0;

    /* get the attribute object identifier */
    for (i = 1; object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (object[i].type == DS_ATTRIBUTE_TYPE)
	    break;
    }

    if (object[i].type != DS_ATTRIBUTE_TYPE)
	return(OM_WRONG_VALUE_TYPE);

    /* Get the attribute abbreviation from the attribute object identifier */
    retval_xoi = xoi_get_attr_name(&(object[i].value.string), abbrv, NULL);

    if (retval_xoi != XOI_SUCCESS)
    {
	/* Since unable to get the abbreviation, convert the */
	/* Object Identifier to its dotted representation    */
	retval_xoi = xoi_ber_to_string(&(object[i].value.string), abbrv);

	if (retval_xoi != XOI_SUCCESS)
	    return(OM_ENCODING_INVALID);
    }

    len = strlen(abbrv);

    /* Check memory */
    if (len > string->length)
	return(OM_MEMORY_INSUFFICIENT);

    /* Copy the Attribute Abbreviation and XOS_ATTR_EQUALITY_OP character */
    strncpy(str, abbrv, len);
    index += len;
    str[index] = XOS_ATTR_EQUALITY_OP;
    ++index;
 
    val_count = 0;
    retval = OM_SUCCESS;

    /* process the attribute values */
    for (i = 1; object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (object[i].type != DS_ATTRIBUTE_VALUES)
	{
	    /* only DS_ATTRIBUTE_TYPE and DS_ATTRIBUTE_VALUES are allowed */
	    if (object[i].type != DS_ATTRIBUTE_TYPE)
	    {
		retval = OM_WRONG_VALUE_TYPE;
		break;
	    }
	    continue;
	}

	/* If multiple values */
	if (val_count > 0)
	{
	    /* check memory */
	    if (index + 1 > string->length)
	    {
		retval = OM_MEMORY_INSUFFICIENT;
		break;
	    }

	    /* Append the XOS_RECUR_OP operator */
	    str[index] = XOS_RECUR_OP;
	    ++index;
	}

	/* if it is an object */
	if ((object[i].syntax & OM_S_SYNTAX) == OM_S_OBJECT)
	{
	    sub_object = object[i].value.object.object;

	    if (sub_object == NULL || sub_object[0].type == OM_NO_MORE_TYPES)
	    {
		retval = OM_WRONG_VALUE_MAKEUP;
		break;
	    }

	    delete_sub_object = OM_FALSE;

	    /* if this sub-object is a private object, convert it to a */
	    /* public object through a call to om_get.		       */
	    if (XOS_PRIVATE(sub_object))
	    {
		retval = om_get(sub_object, OM_NO_EXCLUSIONS, OM_NO_MORE_TYPES,
				local_strings, 0, 0, &p, &total_num);

		if (retval == OM_SUCCESS)
		{
		    /* check if om_get has returned some object */
		    if (total_num <= 0)
		    {
			retval = OM_NO_SUCH_OBJECT;
			break;
		    }
		    else
		    {
			sub_object = p;
			delete_sub_object = OM_TRUE;
		    }
		}
		else
		{
		    break;
		}
	    }

	    /* if object is of type DS_C_DS_DN */
	    if (sub_object[0].value.string.length == dsP_DS_C_DS_DN.length &&
		memcmp(sub_object[0].value.string.elements, 
		       dsP_DS_C_DS_DN.elements, dsP_DS_C_DS_DN.length) == 0)
	    {
		/* Check memory */
		if (index + 1 > string->length)
		{
		    retval = OM_MEMORY_INSUFFICIENT;
		    break;
		}

		/* Append the class start character */
		str[index] = XOS_SATTR_START_OP;
		++index;

		dn_string.elements = &str[index];
		dn_string.length = string->length - index;

		/* convert the dn object to string */
		retval = xos_convert_public_name_to_str(sub_object, 
							local_strings,
						        &dn_string);

		if (retval != OM_SUCCESS)
		    break;

		/* skip past the length for the object */
		index += dn_string.length;

		if (index + 1 > string->length)
		{
		    retval = OM_MEMORY_INSUFFICIENT;
		    break;
		}

		/* Append the class start character */
		str[index] = XOS_SATTR_END_OP;
		++index;
	    }
	    else
	    {
		/* check if it is a blank object with only class descriptor */
		if (sub_object[1].type == OM_NO_MORE_TYPES)
		{
		    retval = OM_WRONG_VALUE_MAKEUP;
		    break;
		}

		/* convert the object to string */
		retval =  xos_convert_class_to_string(sub_object, local_strings,
						      string->length - index,
						      OM_TRUE, &str[index], 
						      &len_used);
		/* Skip past the length for the object */
		index += len_used;
	    }

	    if (retval != OM_SUCCESS)
		break;

	    if (delete_sub_object == OM_TRUE)
	    {
		om_delete(sub_object);
		delete_sub_object = OM_FALSE;
	    }
	}
	else
	{
	    /* if the local bit is string, force the syntax to be printable */
	    if (object[i].syntax & OM_S_LOCAL_STRING)
	    {
		syntax = OM_S_PRINTABLE_STRING;
	    }
	    else
	    {
		/* force the high bits to 0 */
		syntax = object[i].syntax & OM_S_SYNTAX;
	    }

	    switch(syntax)
	    {
		/* if the syntax is any of these, then copy the value from */
		/* length member of the OM_string.		           */
		case (OM_S_INTEGER):
		case (OM_S_BOOLEAN):
		case (OM_S_ENUMERATION):
		    sprintf(c, "%ld", object[i].value.string.length);
		    len = strlen(c);

		    /* check memory */
		    if (index + len > string->length)
		    {
			retval = OM_MEMORY_INSUFFICIENT;
			break;
		    }

		    strncpy(&str[index], c, len);
		    index += len;
		    break;
		case (OM_S_OCTET_STRING):
		case (OM_S_OBJECT_IDENTIFIER_STRING):
		    if (syntax == OM_S_OBJECT_IDENTIFIER_STRING)
		    {
			/* try to get object class abbreviation */
			retval_xoi = xoi_get_objclass_name(
				       &(object[i].value.string), abbrv, NULL);
			if (retval_xoi == XOI_SUCCESS)
			{
			    len = strlen(abbrv);

			    /* check memory */
			    if (index + len > string->length)
			    {
				retval = OM_MEMORY_INSUFFICIENT;
				break;
			    }
			    memcpy(&str[index], abbrv, len);
			    index += len;
			    break;
			}
		    }

		    /* check memory */
		    if (index + 1 > string->length)
		    {
			retval = OM_MEMORY_INSUFFICIENT;
			break;
		    }

		    /* Append the XOS_ATTR_QUOTE_OP character */
		    str[index] = XOS_ATTR_QUOTE_OP;
		    ++index;

		    for (k = 0; k < object[i].value.string.length; ++k)
		    {
			/* Check memory */
			if (index + 5 > string->length)
			{
			    retval = OM_MEMORY_INSUFFICIENT;
			    break;
			}

			str[index] = XOS_ESCAPE_OP;
			++index;
			str[index] = XOS_HEX_OP;
			++index;
			sprintf(&str[index], "%02x", 
				((char *)(object[i].value.string.elements))[k]);
			index += 2;
		    }

		    if (retval != OM_SUCCESS)
			break;

		    /* check memory */
		    if (index + 1 > string->length)
		    {
			retval = OM_MEMORY_INSUFFICIENT;
			break;
		    }

		    /* Append the XOS_ATTR_QUOTE_OP character */
		    str[index] = XOS_ATTR_QUOTE_OP;
		    ++index;

		    break;
		default:
		    /* Append the value and interpret reserved characters */ 
		    for (k = 0; k < object[i].value.string.length; ++k)
		    {
			switch(((char *)(object[i].value.string.elements))[k])
			{
			    /* If reserved character, escape it */
			    case (XOS_ATTR_EQUALITY_OP):
			    case (XOS_RECUR_OP):
			    case (XOS_ATTR_QUOTE_OP):
			    case (XOS_ESCAPE_OP):
				/* Check memory */
				if (index + 1 > string->length)
				{
				    retval = OM_MEMORY_INSUFFICIENT;
				    break;
				}

				str[index] = XOS_ESCAPE_OP;
				++index;
				break;
			    default:
				break;
			}

		        if (retval != OM_SUCCESS)
			    break;

			/* Check memory */
			if (index + 1 > string->length)
			{
			    retval = OM_MEMORY_INSUFFICIENT;
			    break;
			}

			str[index] = ((char *)
				      (object[i].value.string.elements))[k];
			++index;
		    }
		    break;
	    }
	    if (retval != OM_SUCCESS)
		break;
	}
	++val_count;
    }

    if (retval == OM_SUCCESS)
    {
	if (val_count == 0)
	{
	    retval = OM_WRONG_VALUE_MAKEUP;
	}
	else
	{
	    /* Add the NULL character, if space is available */
	    if (index + 1 < string->length)
		str[index] = 0;

	    string->length = index;
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
