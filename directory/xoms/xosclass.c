/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosclass.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:21  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:51:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:53  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:23:01  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:56  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:34  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:52:50  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:17:08  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:30  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:28:56  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:59:51  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosclass.c					   */
/* Description : This file contains all the routines for string    */
/*		 to structured attribute and structured attribute  */
/*		 to string conversions  			   */
/* Date	       : July 1, 1993					   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xosclass.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:21 $";
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

#ifdef XOS_DEBUG
#include <xosdebug.h>
#endif


/* Import the necessary Object Identifier constants */
OM_IMPORT(dsP_DS_C_ATTRIBUTE)
OM_IMPORT(dsP_DS_C_DS_DN)
OM_IMPORT(dsP_DS_C_FILTER_ITEM)

/*-- Static Function(s) -------------------------------------------*/
static OM_return_code xos__get_class_value_count(
    char       *value,       /* IN  - The input component value string */
    OM_uint32   value_len,   /* IN  - The length of the input string   */
    OM_uint    *class_count, /* OUT - Count of the class values        */
    OM_integer *error_pos,   /* OUT - Error position in case of errors */
    OM_integer *error_type); /* OUT - Type of error in case of errors  */

static OM_return_code xos__create_class_obj(
    OM_string  *omclass,       /* IN  - Class Identifier		      */
    char       *value,	       /* IN  - Attribute Value string	              */
    OM_uint32   value_len,     /* IN  - Length of Value string	              */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *class_obj,     /* OUT - The created object		      */
    OM_uint32  *class_len,     /* OUT - Length of Class value in input string */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type);   /* OUT - Type of error in case of errors       */

static OM_return_code xos__create_struct_class_obj(
    OM_string  *omclass,       /* IN  - Class Identifier		      */
    char       *value,	       /* IN  - Attribute Value string	              */
    OM_uint32   value_len,     /* IN  - Length of Value string	              */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *class_obj,     /* OUT - The created object		      */
    OM_uint32  *class_len,     /* OUT - Length of Class value in input string */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type);   /* OUT - Type of error in case of errors       */

static OM_return_code xos__create_dn_class_obj(
    char       *value,	       /* IN  - Attribute Value string	              */
    OM_uint32   value_len,     /* IN  - Length of Value string	              */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *class_obj,     /* OUT - The created object		      */
    OM_uint32  *class_len,     /* OUT - Length of Class value in input string */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type);   /* OUT - Type of error in case of errors       */

static OM_return_code xos__load_class_comp(
    OM_string  *omclass,       /* IN  - Class Identifier		      */
    char       *value,	       /* IN  - Attribute Value string	              */
    OM_uint32   value_len,     /* IN  - Length of Value string	              */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_object   class_obj,     /* IN  - Class Object structure                */
    OM_uint	desc_index,    /* IN  - Index into the Class Object Structure */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_uint32  *len_used,      /* OUT - No. of characters read in input string*/
    OM_uint    *desc_used,     /* OUT - Count of descriptors used 	      */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type);   /* OUT - Type of error in case of errors       */

static OM_return_code xos__get_comp_value_len(
    char       *value,       /* IN  - The input component value string */
    OM_uint32   value_len,   /* IN  - The length of the input string   */
    OM_uint32  *comp_len,    /* OUT - Length of the component value    */
    OM_integer *error_pos,   /* OUT - Error position in case of errors */
    OM_integer *error_type); /* OUT - Type of error in case of errors  */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_create_struct_attr_obj			   */
/* Description   : This function is used to create the XOM object  */
/*		   for a structured attribute.  The input is a 	   */
/*		   string with component value information. 	   */
/*		   Eg: "{xx=2;5, yy={aa=1;2, bb=3};{bb=5}};{xx=50}"*/
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_create_struct_attr_obj(
    OM_string  *objid,         /* IN  - Attribute Object Identifier           */
    OM_string  *omclass,       /* IN  - Class Identifier                      */
    char       *value,	       /* IN  - Attribute Value string	              */
    OM_uint32   value_len,     /* IN  - Length of Value string	              */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  is_filter,     /* IN  - Boolean flag to indicate filter item  */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *object,        /* OUT - The created object	              */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors       */
{
OM_return_code   retval;
OM_object 	 attr_obj, class_obj;
OM_uint32        curr_index, class_len;
OM_uint		 tot_desc;
OM_uint		 class_val_count;
OM_sint		 i;

#ifdef XOS_DEBUG
char		 debug_msg[256];
#endif

#ifdef XOS_DEBUG
    xos_debug_string("xos_create_struct_attr_obj : ENTRY", 0, 1);
    xos_debug_string("Input string :", 0, 0);
    xos_debug_string(value, value_len, 0);
#endif

    /* Get the total count of structured attribute values */
    retval = xos__get_class_value_count(value, value_len, &class_val_count,
					error_pos, error_type);

    if (retval != OM_SUCCESS)
	return(retval);

#ifdef XOS_DEBUG
    sprintf(debug_msg, "Total numer of class values = <%d>", class_val_count);
    xos_debug_string(debug_msg, 0, 0);
#endif

    /* If the attribute object is going to be used as a DS_C_FILTER_ITEM */
    if (is_filter == OM_TRUE)
    {
	/* Compute the total number of OM_descriptor structures. */
	/* 4 more descriptors are required for DS_C_FILTER_ITEM, */ 
	/* DS_ATTRIBUTE_TYPE, DS_FILTER_ITEM_TYPE,  and the      */
	/* OM_NULL_DESCRIPTOR.					 */
	tot_desc = 4 + class_val_count;
    }
    else
    {
	/* Compute the total number of OM_descriptor structures. */
	/* 3 more descriptors are required for DS_C_ATTRIBUTE,   */ 
	/* DS_ATTRIBUTE_TYPE and the OM_NULL_DESCRIPTOR.	 */
	tot_desc = 3 + class_val_count;
    }
  
    /* Allocate the memory for the OM_descriptor structures */
    attr_obj = (OM_descriptor *) malloc(sizeof(OM_descriptor) * tot_desc);

    if (attr_obj == NULL)
    {
	*error_type = OMX_MEMORY_INSUFFICIENT;
	return(OM_MEMORY_INSUFFICIENT);
    }

    /* If the attribute object is going to be used as a DS_C_FILTER_ITEM */
    if (is_filter == OM_TRUE)
    {
	/* Initialize the first OM_descriptor to DS_C_FILTER_ITEM */
	omX_fill_oid(OM_CLASS, dsP_DS_C_FILTER_ITEM, &attr_obj[0]);
    }
    else
    {
	/* Initialize the first OM_descriptor to DS_C_ATTRIBUTE */
	omX_fill_oid(OM_CLASS, dsP_DS_C_ATTRIBUTE, &attr_obj[0]);
    }

    /* Initialize the second OM_descriptor to DS_ATTRIBUTE_TYPE */
    omX_fill_oid(DS_ATTRIBUTE_TYPE, *objid, &attr_obj[1]);

    /* Initialize all the others to NULL descriptors */
    for (i = 2; i < tot_desc; ++i)
    {
	/* If the attribute object is going to be used as DS_C_FILTER_ITEM */
	/* Initialize the penultimate descriptor to DS_FILTER_TYPE.        */
	if (is_filter == OM_TRUE && i == tot_desc - 2)
	{
	    omX_fill(DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, DS_EQUALITY, 
		     NULL, &attr_obj[i]);
	}
	else
	{
	    XOS_FILL_OMD_NULL(&attr_obj[i]);
	}
    }

    curr_index = 0;

    for (i = 0; i < class_val_count; ++i)
    {
	/* Create the XOM object for the class */
	retval = xos__create_class_obj(omclass, &value[curr_index], value_len, 
				       val_mem_top, local_strings, &class_obj, 
				       &class_len, error_pos, error_type);

	if (retval != OM_SUCCESS)
	    break;

	/* Now fill the OM_descriptor */
	omX_fill(DS_ATTRIBUTE_VALUES, OM_S_OBJECT, 0, class_obj, 
		 &attr_obj[i+2]);

	/* skip past the current class value */
	curr_index = curr_index + class_len;
	value_len = value_len - class_len;

	/* move to the next available memory */
	val_mem_top += class_len;

	/* skip past XOS_RECUR_OP character */
	if (class_val_count > 1)
	{
	    /* skip past white spaces */
	    while (isspace(value[curr_index]))
	    {
		++curr_index;
		--value_len;
	    }

	    /* now skip past XOS_RECUR_OP character */
	    ++curr_index;
	    --value_len;
	}
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos += curr_index;

	--i;
	/* free all the allocated areas */
	for (; i >= 0; --i)
	{
	    xos_free_class_object(attr_obj[i+2].value.object.object);
	}
	free(attr_obj);
	return(retval);
    }

    *object = attr_obj;

#ifdef XOS_DEBUG
    xos_debug_string("xos_create_struct_attr_obj : EXIT", 0, -1);
#endif

    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_class_obj			   */
/* Description   : This function is used to create the XOM object  */
/*		   for the first encountered class value block in  */
/*		   the input string.  This function is invoked     */
/*		   recursively if any of the components are further*/
/*		   structured. The input is a string with component*/
/*		   value information.   		   	   */
/*		   Eg: "{xx=2;5, yy={aa=1;2, bb=3};{bb=5}};{xx=50}"*/
/*		   The end of the class value would be the first   */
/*		   corresponding XOS_SATTR_END_OP character.	   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_class_obj(
    OM_string  *omclass,       /* IN  - Class Identifier		      */
    char       *value,	       /* IN  - Attribute Value string	              */
    OM_uint32   value_len,     /* IN  - Length of Value string	              */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *class_obj,     /* OUT - The created object		      */
    OM_uint32  *class_len,     /* OUT - Length of Class value in input string */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors       */
{
OM_return_code  retval;
OM_boolean	dn_syntax = OM_FALSE;
OM_uint32	len;
OM_uint32	curr_index;

    curr_index = 0;

    /* skip past white spaces */
    while (curr_index < value_len && isspace(value[curr_index]))
	++curr_index;

    value_len -= curr_index;

    /* Check if it an object of DN Syntax */
    if (omclass->length == dsP_DS_C_DS_DN.length &&
	 memcmp(omclass->elements, dsP_DS_C_DS_DN.elements, 
		dsP_DS_C_DS_DN.length) == 0)
    {
	retval = xos__create_dn_class_obj(&value[curr_index], value_len, 
					  val_mem_top, local_strings, class_obj,
					  &len, error_pos, error_type);
	dn_syntax = OM_TRUE;
    }
    else
    {
	retval = xos__create_struct_class_obj(omclass, &value[curr_index], 
					      value_len, val_mem_top, 
					      local_strings, class_obj, &len, 
					      error_pos, error_type);
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos += curr_index;
    }
    else
    {
	*class_len = len + curr_index;

	/* now check if there is any irrelevant data after class string */
	value = value + curr_index + len;
	value_len = value_len - len;
	curr_index = 0;

	/* skip past white spaces */
	while (curr_index < value_len && isspace(value[curr_index]))
	    ++curr_index;

	if (curr_index < value_len && 
	    (!(value[curr_index] == XOS_SATTR_END_OP || 
	       value[curr_index] == XOS_RECUR_OP ||
	       value[curr_index] == XOS_COMP_DELIMITER_OP)))
	{
	    *error_pos = *class_len + curr_index + 1;
	    *error_type = OMX_FORMAT_ERROR;
	    retval = OM_WRONG_VALUE_MAKEUP;
	    if (dn_syntax == OM_TRUE)
		xos_free_dn_object(*class_obj);
	    else
		xos_free_class_object(*class_obj);
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_struct_class_obj			   */
/* Description   : This function is used to create the XOM object  */
/*		   for the first encountered class value block in  */
/*		   the input string.  This function is used for    */
/*		   objects whose class name is other than          */
/*		   DS_C_DS_DN. The input is a string with component*/
/*		   value information.   		   	   */
/*		   Eg: "{xx=2;5, yy={aa=1;2, bb=3};{bb=5}};{xx=50}"*/
/*		   The end of the class value would be the first   */
/*		   corresponding XOS_SATTR_END_OP character.	   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_struct_class_obj(
    OM_string  *omclass,       /* IN  - Class Identifier		      */
    char       *value,	       /* IN  - Attribute Value string	              */
    OM_uint32   value_len,     /* IN  - Length of Value string	              */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *class_obj,     /* OUT - The created object		      */
    OM_uint32  *class_len,     /* OUT - Length of Class value in input string */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors       */
{
OM_return_code  retval;
OM_object       obj;
OM_uint32	curr_index, len_used;
OM_uint	        comp_count;
OM_uint	        desc_used, i;

#ifdef XOS_DEBUG
char		debug_msg[256];
#endif

#ifdef XOS_DEBUG
    xos_debug_string("xos__create_struct_class_obj : ENTRY", 0, 1);
    xos_debug_string("Input String :", 0, 0);
    xos_debug_string(value, value_len, 0);
#endif

    /* Get the total count of components for the class */
    retval = xos_get_class_value_data(value, value_len, OM_TRUE, class_len, 
				      &comp_count, error_pos, error_type);

    if (retval != OM_SUCCESS)
	return(retval);

#ifdef XOS_DEBUG
    sprintf(debug_msg, "Total number of component values = <%d>", comp_count);
    xos_debug_string(debug_msg, 0, 0);
#endif

    /* skip past XOS_SATTR_START_OP */
    --value_len;
    curr_index = 1;

    /* Allocate the memory for the OM_descriptor structures.    */
    /* 2 more descriptors are required for the Class Identifier */
    /* and the OM_NULL_DESCRIPTOR.			        */
    obj = (OM_descriptor *) malloc(sizeof(OM_descriptor) * (2 + comp_count));

    if (obj == NULL)
    {
	*error_type = OMX_MEMORY_INSUFFICIENT;
	return(OM_MEMORY_INSUFFICIENT);
    }

    /* Initialize the first OM_descriptor to the Class Identifier */
    omX_fill_oid(OM_CLASS, *omclass, &obj[0]);

    /* Initialize all the others to NULL descriptors */
    for (i = 1; i < comp_count + 2; ++i)
	XOS_FILL_OMD_NULL(&obj[i]);

    i = 0;
    while (i < comp_count)
    {
	/* Load the component details into the descriptor structure */
	retval = xos__load_class_comp(omclass, &value[curr_index], value_len, 
				      val_mem_top, obj, i+1, local_strings,
				      &len_used, &desc_used, error_pos, 
				      error_type);

	if (retval != OM_SUCCESS)
	    break;

	/* skip past current component value and XOS_COMP_DELIMITER_OP */
	curr_index = curr_index + len_used + 1;
	value_len = value_len - len_used - 1;

	/* move to the next available memory */
	val_mem_top += len_used;

	/* update the number of used descriptor structures */
	i += desc_used;
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos += curr_index;
	xos_free_class_object(obj);
	return(retval);
    }

    *class_obj = obj;

#ifdef XOS_DEBUG
    sprintf(debug_msg, "Length of read class value = <%d>", *class_len);
    xos_debug_string(debug_msg, 0, 0);
    xos_debug_string("xos__create_struct_class_obj : EXIT", 0, -1);
#endif

    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_dn_class_obj			   */
/* Description   : This function is used to create the XOM object  */
/*		   for the first encountered class value block in  */
/*		   the input string.  This function is used for    */
/*		   objects whose class name is DS_C_DS_DN. The     */
/*		   input is a string with component value 	   */
/*		   information.   		   	   	   */
/*		   Eg: "{/c=de/o=sni};{/c=de/o=dap}"		   */
/*		   The end of the class value would be the first   */
/*		   corresponding XOS_SATTR_END_OP character.	   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_dn_class_obj(
    char       *value,	       /* IN  - Attribute Value string	              */
    OM_uint32   value_len,     /* IN  - Length of Value string	              */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *class_obj,     /* OUT - The created object		      */
    OM_uint32  *class_len,     /* OUT - Length of Class value in input string */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors       */
{
OM_return_code  retval;
OM_string	string;

#ifdef XOS_DEBUG
char		debug_msg[256];
#endif

#ifdef XOS_DEBUG
    xos_debug_string("xos__create_dn_class_obj : ENTRY", 0, 1);
    xos_debug_string("Input String :", 0, 0);
    xos_debug_string(value, value_len, 0);
#endif

    retval = xos_get_class_value_data(value, value_len, OM_TRUE, class_len, 
				      NULL, error_pos, error_type);

    if (retval != OM_SUCCESS)
	return(retval);

    /* skip past XOS_SATTR_START_OP character */
    string.elements = value + 1;

    /* reduce the length for XOS_SATTR_START_OP and XOS_SATTR_END_OP */
    string.length = *class_len - 2;

    /* now create the DN object */
    retval = xos_convert_str_to_public_name(&string, val_mem_top, OM_FALSE,
					    local_strings, class_obj, 
					    error_pos, error_type);
    if (retval != OM_SUCCESS)
	++*error_pos;

#ifdef XOS_DEBUG
    sprintf(debug_msg, "Length of read class value = <%d>", *class_len);
    xos_debug_string(debug_msg, 0, 0);
    xos_debug_string("xos__create_dn_class_obj : EXIT", 0, -1);
#endif

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__load_class_comp		   	   	   */
/* Description   : This function is used to load details of the    */
/*		   component value in the descriptor structure.    */
/*		   The input is a string with component value      */
/*		   information. 		   		   */
/*		   Eg: "xx=2;5, yy={aa=1;2, bb=3};{bb=5}};{xx=50}" */
/*		   In the above example "xx=2;5" is interpreted    */
/*		   and loaded into the descriptor structure.       */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__load_class_comp(
    OM_string  *omclass,       /* IN  - Class Identifier		      */
    char       *value,	       /* IN  - Attribute Value string	              */
    OM_uint32   value_len,     /* IN  - Length of Value string	              */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_object   class_obj,     /* IN  - Class Object structure                */
    OM_uint	desc_index,    /* IN  - Index into the Class Object Structure */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_uint32  *len_used,      /* OUT - No. of characters read in input string*/
    OM_uint    *desc_used,     /* OUT - Count of descriptors used 	      */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors       */
{
OM_return_code  retval;
OM_integer	retval_xoi;
OM_object       comp_obj;
OM_uint32       curr_index, len, comp_len, c_val_len;
OM_string       comp_omclass;
OM_syntax       omsyntax;
OM_type         omtype;
OM_uint		j;
char	        abbrv[XOS_MAX_ABBRV + 1];

#ifdef XOS_DEBUG
char		debug_msg[256];
#endif

#ifdef XOS_DEBUG
    xos_debug_string("xos__load_class_comp : ENTRY", 0, 1);
    xos_debug_string("Input String :", 0, 0);
    xos_debug_string(value, value_len, 0);
#endif

    curr_index = 0;
    *desc_used = 0;

    /* skip past white spaces */
    while (curr_index < value_len && isspace(value[curr_index]))
	++curr_index;

    *error_pos = curr_index + 1;

    /* get the abbreviation */
    for (j = 0; curr_index < value_len; ++curr_index, ++j)
    {
	if (j > XOS_MAX_ABBRV)
	{
	    *error_type = OMX_UNKNOWN_ABBRV;
	    return(OM_WRONG_VALUE_MAKEUP);
	}

	/* Abbreviation string is an alpha numeric string */
	if (!isalnum(value[curr_index]))
	    break;

	abbrv[j] = value[curr_index];
    }

    if (j == 0)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_ABBRV;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    /* Append the NULL character */
    abbrv[j] = 0;

    /* skip past white spaces */
    while (curr_index < value_len && isspace(value[curr_index]))
	++curr_index;

    /* check if XOS_ATTR_EQUALITY_OP character is present */
    if (value[curr_index] != XOS_ATTR_EQUALITY_OP)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_EQUAL_OPERATOR;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    /* skip past the XOS_ATTR_EQUALITY_OP character */
    ++curr_index;

    /* get the details of the component */ 
    retval_xoi = xoi_get_comp_details(omclass, abbrv, &omtype, &omsyntax,
				      &(comp_omclass.elements), 
				      &(comp_omclass.length));

    if (retval_xoi != XOI_SUCCESS)
    {
	/* error_pos is not initialized here because it would have */
	/* been properly set.					   */
	*error_type = xos_map_xoi_error(retval_xoi);
	return(OM_WRONG_VALUE_MAKEUP);
    }

    len = value_len - curr_index;

    while (OM_TRUE)
    {
	/* if the component is further structured */
	if (omsyntax == OM_S_OBJECT)
	{
	    /* invoke the function recursively */
	    retval = xos__create_class_obj(&comp_omclass, &value[curr_index], 
					   len, val_mem_top, local_strings,
					   &comp_obj, &comp_len, error_pos, 
					   error_type);
	    if (retval != OM_SUCCESS)
	    {
		*error_pos += curr_index;
		break;
	    }

	    /* Now fill up the descriptor */
	    omX_fill(omtype, OM_S_OBJECT, 0, comp_obj, 
		     &(class_obj[desc_index]));
	}
	else
	{
	    /* get the length of the component value */
	    retval = xos__get_comp_value_len(&value[curr_index], len, &comp_len,
					     error_pos, error_type);
	    if (retval != OM_SUCCESS)
	    {
		*error_pos += curr_index;
		break;
	    }

	    /* process the reserved characters in the component value */
	    /* and create a normalized string.			      */
	    retval = xos_convert_to_normalized_string(&value[curr_index], 
						      comp_len, 
						      XOS_ATTR_QUOTE_OP,
						      val_mem_top, &c_val_len,
						      error_pos, error_type);
	    if (retval != OM_SUCCESS)
	    {
		*error_pos += curr_index;
		break;
	    }

#ifdef XOS_DEBUG
    sprintf(debug_msg, "Length of the component value read = <%d>", comp_len);
    xos_debug_string(debug_msg, 0, 0);
    xos_debug_string("Normalized value:", 0, 0);
    xos_debug_string(val_mem_top, c_val_len, 0);
#endif

	    /* Now fill the descriptor */
	    retval = xos_fill_descriptor(omtype, omsyntax, c_val_len, 
					 val_mem_top, local_strings, 
					 &class_obj[desc_index]);

	    if (retval != OM_SUCCESS)
	    {
		*error_type = OMX_WRONG_VALUE;
		*error_pos = curr_index;
		break;
	    }
	}

	++desc_index;
	++*desc_used;

	/* skip past the current component_value */
	curr_index += comp_len;
	len -= comp_len;

	/* move to the next available memory location */
	val_mem_top += comp_len;

	if (curr_index < value_len)
	{
	    /* skip past white spaces */
	    while (curr_index < value_len && isspace(value[curr_index]))
	    {
		++curr_index;
		--len;
	    }
	}

	/* check if more than one component value is specified */
	if (curr_index < value_len && value[curr_index] == XOS_RECUR_OP)
	{
	    /* skip past the XOS_RECUR_OP character */
	    ++curr_index;
	    --len;
	}
	else
	{
	    break;
	}
    }

    if (retval != OM_SUCCESS)
    {
	if (omsyntax == OM_S_OBJECT)
	{
	    free(comp_omclass.elements);

	    /* free the created class objects */
	    --desc_index;
	    for (; *desc_used > 0; --*desc_used, --desc_index)
	    {
	       xos_free_class_object(class_obj[desc_index].value.object.object);

	       /* Now initialize to NULL, so that it is not freed axoin */
	       /* by the calling function.			   	*/
	       XOS_FILL_OMD_NULL(&class_obj[desc_index]);
	    }
	}

	return(retval);
    }

    /* If the object is of type DS_C_DS_DN, free the omclass since the  */
    /* memory space would be provided by xos_convert_str_to_public_name */
    if (omsyntax == OM_S_OBJECT)
    {
	if (comp_omclass.length == dsP_DS_C_DS_DN.length &&
	    memcmp(comp_omclass.elements, dsP_DS_C_DS_DN.elements, 
		   dsP_DS_C_DS_DN.length) == 0)
	{
	    free(comp_omclass.elements);
	}
    }

    *len_used = curr_index;

#ifdef XOS_DEBUG
    sprintf(debug_msg,"Length of component value string read = <%d>",*len_used);
    xos_debug_string(debug_msg, 0, 0);
    xos_debug_string("xos__load_class_comp : EXIT", 0, -1);
#endif

    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__get_comp_value_len		   	   */
/* Description   : This function reads the input string and returns*/
/*		   the length of the first encountered component   */
/*		   value.					   */
/*		   The input is a string with component value      */
/*		   information. 		   		   */
/*		   Eg: "2;5, yy={aa=1;2, bb=3};{bb=5}};{xx=50}"    */
/*		   In the above example the length of the first    */
/*		   component value is 1.			   */ 
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__get_comp_value_len(
    char       *value,      /* IN  - The input component value string */
    OM_uint32   value_len,  /* IN  - The length of the input string   */
    OM_uint32  *comp_len,   /* OUT - Length of the component value    */
    OM_integer *error_pos,  /* OUT - Error position in case of errors */
    OM_integer *error_type) /* OUT - Type of error in case of errors  */
{
OM_return_code  retval;
OM_boolean  	open_quote_found, close_quote_found;
OM_uint32    	curr_index;

    retval = OM_SUCCESS;

    curr_index = 0;
    /* skip past white spaces */
    while (curr_index < value_len && isspace(value[curr_index]))
	++curr_index;

    if (curr_index == value_len || value[curr_index] == XOS_RECUR_OP ||
	value[curr_index] == XOS_COMP_DELIMITER_OP || 
	value[curr_index] == XOS_SATTR_END_OP)
    {
	*error_type = OMX_MISSING_COMP_VALUE;
	retval = OM_WRONG_VALUE_MAKEUP;
    }
    else
    {
	if (value[curr_index] == XOS_SATTR_START_OP)
	{
	    *error_type = OMX_UNEXPECTED_OPERATOR;
	    retval = OM_WRONG_VALUE_MAKEUP;
	}
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos = curr_index + 1;
	return(retval);
    }

    open_quote_found = close_quote_found = OM_FALSE;

    for (; curr_index < value_len; ++curr_index)
    {
	/* if escape operator */
	if (value[curr_index] == XOS_ESCAPE_OP)
	{
	    /* skip past escape character */
	    ++curr_index;

	    /* If hexadecimal values specified ("\xnn") */
	    if (tolower(value[curr_index]) == XOS_HEX_OP)
	    {
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
		*error_type = OMX_FORMAT_ERROR;
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

	/* The reserved characters are not interpreted within quotes */
	if (open_quote_found == OM_FALSE || close_quote_found == OM_TRUE)
	{
	    if (value[curr_index] == XOS_SATTR_START_OP || 
		value[curr_index] == XOS_ATTR_EQUALITY_OP)
	    {
		*error_type = OMX_UNEXPECTED_OPERATOR;
		retval = OM_WRONG_VALUE_MAKEUP;
		break;
	    }
		
	    if (value[curr_index] == XOS_COMP_DELIMITER_OP ||
		value[curr_index] == XOS_SATTR_END_OP || 
		value[curr_index] == XOS_RECUR_OP)
	    {
		break;
	    }
	}

	/* if there is some more non space data after closed quotes */
	if (close_quote_found == OM_TRUE && (!isspace(value[curr_index])))
	{
	    *error_type = OMX_FORMAT_ERROR;
	    retval = OM_WRONG_VALUE_MAKEUP;
	    break;
	}
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos = curr_index + 1;
    }
    else
    {
	if (open_quote_found == OM_TRUE && close_quote_found == OM_FALSE)
	{
	    *error_pos = curr_index + 1;
	    *error_type = OMX_MISMATCHED_QUOTES;
	    retval = OM_WRONG_VALUE_MAKEUP;
	}
	else
	{
	    /* if no more data after XOS_RECUR_OP or XOS_COMP_DELIMITER_OP */
	    if (curr_index + 1 == value_len && 
		(value[curr_index] == XOS_RECUR_OP || 
		 value[curr_index] == XOS_COMP_DELIMITER_OP))
	    {
		*error_pos = curr_index + 2;
		*error_type = OMX_MISSING_COMP_VALUE;
		retval = OM_WRONG_VALUE_MAKEUP;
	    }
	}
    }

    if (retval == OM_SUCCESS)
	*comp_len = curr_index;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__get_class_value_count		   	   */
/* Description   : This function is used to count the number of    */
/*		   class values in the input string.		   */
/*		   The input is a string with component value      */
/*		   information. 		   		   */
/*		   Eg: "{xx=2;5, yy={aa=1;2, bb=3};{bb=5}};{xx=50}"*/
/*		   In the above example the count of the class     */
/*		   value is 2.					   */ 
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__get_class_value_count(
    char       *value,       /* IN  - The input component value string */
    OM_uint32   value_len,   /* IN  - The length of the input string   */
    OM_uint    *class_count, /* OUT - Count of the class values        */
    OM_integer *error_pos,   /* OUT - Error position in case of errors */
    OM_integer *error_type)  /* OUT - Type of error in case of errors  */
{
OM_return_code  retval;
OM_uint32       len, class_len;
OM_uint32       curr_index;
OM_uint		tot_count;

    curr_index = 0;
   
    /* skip past white spaces */
    while (curr_index < value_len && isspace(value[curr_index]))
	++curr_index;

    if (curr_index == value_len)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_CLASS_VALUE;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    len = value_len - curr_index;
    tot_count = 0;

    /* Compute the total number of class values */
    while (curr_index < value_len)
    {
	/* get the length of the current class value */
	retval = xos_get_class_value_data(&value[curr_index], len, OM_TRUE,
					  &class_len, NULL, error_pos, 
					  error_type);
	if (retval != OM_SUCCESS)
	{
	    *error_pos += curr_index;
	    return(retval);
	}

	++tot_count;

	/* skip past the current class value */
	curr_index = curr_index + class_len;
	len = len - class_len;

	/* skip past XOS_RECUR_OP character */
	if (curr_index < value_len)
	{
	    /* skip past white spaces */
	    while (curr_index < value_len && isspace(value[curr_index]))
	    {
		++curr_index;
		--len;
	    }

	    /* now skip past XOS_RECUR_OP character */
	    if (curr_index < value_len)
	    {
		if (value[curr_index] == XOS_RECUR_OP)
		{
		    ++curr_index;
		    --len;
		    if (curr_index == value_len)
		    {
			*error_pos = curr_index + 1;
			*error_type = OMX_MISSING_CLASS_VALUE;
			return(OM_WRONG_VALUE_MAKEUP);
		    }
		}
		else
		{
		    *error_pos = curr_index + 1;
		    *error_type = OMX_FORMAT_ERROR;
		    return(OM_WRONG_VALUE_MAKEUP);
		}
	    }
	}
    }

    *class_count = tot_count;
    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_get_class_value_data		   	   */
/* Description   : This function reads the input string and returns*/
/*		   the first encountered class value and the number*/
/*		   of components in this class value. The end of   */
/*		   the class value would be the first corresponding*/
/*		   XOS_SATTR_END_OP character.  		   */
/*		   The input is a string with component value      */
/*		   information. 		   		   */
/*		   Eg: "{xx=2;5, yy={aa=1;2, bb=3};{bb=5}};{xx=50}"*/
/*		   In the above example the length of the first    */
/*		   class value is 34.				   */ 
/*		   If the input parameter delim_reqd is set to     */
/*		   OM_FALSE, the input string does not have the    */
/*		   class delimiters.				   */
/*		   Eg: "xx=2;5, yy={aa=1;2, bb=3}".		   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_get_class_value_data(
    char       *value,      /* IN  - The input component value string 	     */
    OM_uint32   value_len,  /* IN  - The length of the input string   	     */
    OM_boolean  delim_reqd, /* IN  - Whether class delimiters ("{","}") reqd.*/
    OM_uint32  *class_len,  /* OUT - Length of the class value        	     */
    OM_uint    *comp_count, /* OUT - Total number of components in the class */
    OM_integer *error_pos,  /* OUT - Error position in case of errors 	     */
    OM_integer *error_type) /* OUT - Type of error in case of errors  	     */
{
OM_return_code  retval;
OM_boolean 	quote_found;
OM_uint32   	curr_index;
OM_uint	    	tot_count;
OM_uint	    	open_brace_count, close_brace_count;

    tot_count = 0;

    retval = OM_SUCCESS;
    curr_index = 0;
    /* skip past white spaces */
    while (curr_index < value_len && isspace(value[curr_index]))
	++curr_index;

    if (curr_index == value_len || value[curr_index] == XOS_RECUR_OP)
    {
	*error_type = OMX_MISSING_CLASS_VALUE;
	retval = OM_WRONG_VALUE_MAKEUP;
    }
    else
    {
	if (delim_reqd == OM_TRUE)
	{
	    if (value[curr_index] != XOS_SATTR_START_OP)
	    {
		*error_type = OMX_MISSING_CLASS_START_OP;
		retval = OM_WRONG_VALUE_MAKEUP;
	    }
	}
	else
	{
	    if (value[curr_index] == XOS_SATTR_START_OP)
	    {
		*error_type = OMX_FORMAT_ERROR;
		retval = OM_WRONG_VALUE_MAKEUP;
	    }
	}
    }

    if (retval == OM_SUCCESS)
    {
	/* skip past XOS_SATTR_START_OP character */
	if (delim_reqd == OM_TRUE)
	{
	    ++curr_index;
	    open_brace_count = 1;
	}
	else
	{
	    open_brace_count = 0;
	}

	/* skip past white spaces */
	while (curr_index < value_len && isspace(value[curr_index]))
	    ++curr_index;

	if (curr_index == value_len || value[curr_index] == XOS_RECUR_OP || 
	    value[curr_index] == XOS_SATTR_END_OP || 
	    value[curr_index] == XOS_COMP_DELIMITER_OP)
	{
	    *error_type = OMX_MISSING_COMP_VALUE;
	    retval = OM_WRONG_VALUE_MAKEUP;
	}
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos = curr_index + 1;
	return(retval);
    }

    close_brace_count = 0;
    quote_found = OM_FALSE;
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

	    /* skip past escape character */
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
	    if (quote_found == OM_TRUE)
		quote_found = OM_FALSE;
	    else
		quote_found = OM_TRUE;
	    continue;
	}

	/* The reserved characters are not interpreted within quotes */
	if (quote_found ==  OM_FALSE)
	{
	    if (value[curr_index] == XOS_SATTR_END_OP)
	    {
		++close_brace_count;

		/* The XOS_SATTR_END_OP character should correspond to the */
		/* structured class block and not any other structured     */
		/* component block within the class block.    	           */
		if (open_brace_count == close_brace_count)
		{
		    ++tot_count;

		    if (delim_reqd == OM_TRUE)
			break;
		}
	    }

	    /* Encountered Start of another structured component block */
	    if (value[curr_index] == XOS_SATTR_START_OP)
		++open_brace_count;

	    if (value[curr_index] == XOS_COMP_DELIMITER_OP || 
		value[curr_index] == XOS_RECUR_OP)
	    {
		/* The operators should correspond to the structured class   */
		/* block and not any other structured component block within */
		/* the class block.					     */
		if (open_brace_count == close_brace_count + 1)
		    ++tot_count;
	    }

	    if (delim_reqd == OM_FALSE && value[curr_index] == XOS_RECUR_OP)
	    {
		/* if class delimiters are not specified, end of the 	  */
		/* class value string would be the XOS_RECUR_OP character */ 
		if (open_brace_count == close_brace_count)
		    break;
	    }
	}
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_FORMAT_ERROR;
	return(retval);
    }

    if (retval == OM_SUCCESS)
    {
	if (quote_found == OM_TRUE)
	{
	    *error_type = OMX_MISMATCHED_QUOTES;
	    retval = OM_WRONG_VALUE_MAKEUP;
	}
	else
	{
	    if (open_brace_count != close_brace_count)
	    {
		*error_type = OMX_MISMATCHED_BRACKETS;
		retval = OM_WRONG_VALUE_MAKEUP;
	    }
	    else
	    {
		/* if end of string reached */
		if (curr_index == value_len && delim_reqd == OM_TRUE)
		{
		    *error_type = OMX_MISSING_CLASS_END_OP;
		    retval = OM_WRONG_VALUE_MAKEUP;
		}
	    }
	}
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos = curr_index + 1;
    }
    else
    {
	if (class_len != NULL)
	{
	    *class_len = curr_index + 1;

	    if (delim_reqd == OM_FALSE)
	    {
		if (value[curr_index] != XOS_RECUR_OP)
		    --*class_len;
	    }
	}

	if (comp_count != NULL)
	    *comp_count = tot_count;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_str_to_obj_create_class			   */
/* Description   : This function is called by the xos__str_to_obj  */
/*		   function to create an object corresponding      */
/*		   to the OM Class specified in the input string.  */
/*		   The input is a string with component value      */
/*		   information.   		   	   	   */
/*		   Eg: "xx=2;5, yy={aa=1;2, bb=3}"		   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_str_to_obj_create_class(
    OM_string  *omclass,       /* IN  - Class Identifier		      */
    OM_string  *string,	       /* IN  - The string to be converted	      */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *class_obj,     /* OUT - The created object		      */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors       */
{
OM_return_code   retval;
OM_uint32	 class_len;
OM_uint32	 dst_len;
OM_uint32	 src_idx, dst_idx;
char		*src, *dst;

    dst = NULL;

    /* get the length of the class string */
    retval = xos_get_class_value_data((char *) (string->elements), 
				      string->length, OM_FALSE, &class_len, 
				      NULL, error_pos, error_type);
    if (retval ==  OM_SUCCESS)
    {
	/* now check for additional characters after the class string */
	if (class_len != string->length)
	{
	    *error_pos = class_len;
	    *error_type = OMX_FORMAT_ERROR;
	    retval = OM_WRONG_VALUE_MAKEUP;
	}
	else
	{
	    /* To use the existing functions, the input string has to be   */
	    /* formatted i.e. the XOS_SATTR_START_OP and XOS_SATTR_END_OP  */
	    /* characters have to be placed at the start and end of the    */
	    /* string respectively.	   				   */

	    src = (char *) (string->elements);

	    /* allocate the size. Two more bytes are required for the */
	    /* XOS_SATTR_START_OP and XOS_SATTR_END_OP characters     */
	    dst = (char *) malloc(sizeof(char) * (string->length + 2));

	    if (dst == NULL)
	    {
		*error_type = OMX_MEMORY_INSUFFICIENT;
		retval = OM_MEMORY_INSUFFICIENT;
	    }
	    else
	    {
		/* copy the XOS_SATTR_START_OP character */
		dst[0] = XOS_SATTR_START_OP;

		/* copy the string */
		for (src_idx = 0, dst_idx = 1; src_idx < string->length; 
						          ++src_idx, ++dst_idx)
		{
		    dst[dst_idx] = src[src_idx];
		}

		/* append the XOS_SATTR_END_OP character */
		dst[dst_idx] = XOS_SATTR_END_OP;

		dst_len = dst_idx + 1;

		/* now convert the string to class */
		retval = xos__create_class_obj(omclass, dst, dst_len, 
					       val_mem_top, local_strings,
					       class_obj, &class_len, 
					       error_pos, error_type);
		if (retval != OM_SUCCESS)
		{
		    /* reduce the length of the error position by 1, to */
		    /* account for the XOS_SATTR_START_OP character     */
		    /* introduced at the start of the string 	        */
		    if (*error_pos> 1)
		    {
			--*error_pos;
		    }
		}
	    }
	}
    }

    /* now free the allocated area */
    if (dst != NULL)
    {
	free(dst);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_free_class_object		   	   */
/* Description   : This function is used to free the memory space  */
/*		   occupied by the class object.		   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xos_free_class_object(
    OM_object object)  /* IN - The object to be freed */
{
OM_object  p;
OM_type    deleted_omtype = OM_NO_MORE_TYPES;
OM_uint    i;

    /* skip the first class descriptor */
    for (i = 1; object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (object[i].syntax == OM_S_OBJECT)
	{
	    p = object[i].value.object.object;

	    if (p[0].type == OM_NO_MORE_TYPES)
	        continue;

	    if (p[0].value.string.length == dsP_DS_C_DS_DN.length &&
		memcmp(p[0].value.string.elements, dsP_DS_C_DS_DN.elements,
		       dsP_DS_C_DS_DN.length) == 0)
	    {
		xos_free_dn_object(p);
	    }
	    else
	    {
		/* The class identifier memory space is shared by */
		/* all the recurring values of the class. So it   */
		/* must be deleted only once.			  */
		if (deleted_omtype != object[i].type)
		{
		     /* free the class identifier */
		    free(p[0].value.string.elements);
		    deleted_omtype = object[i].type;
		}
	        /* call this function recursively */
		xos_free_class_object(p);
	    }
	}
	else
	{
	    if (object[i].syntax == OM_S_OBJECT_IDENTIFIER_STRING)
	    {
		free(object[i].value.string.elements);
	    }
	}
    }

    /* Now free the class object */
    free(object);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_convert_class_to_string		   	   */
/* Description   : This function is used to convert a structured   */
/*		   attribute object to string form.		   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_convert_class_to_string(
    OM_object    object,       /* IN  - The object to be converted	      */
    OM_boolean   local_strings,/* IN  - To indicate local string conversion   */
    OM_uint32    max_len,      /* IN  - Maximum length of the converted string*/
    OM_boolean   delim_reqd,   /* IN  - Whether Class Delimiters "{","}" reqd.*/
    char        *string,       /* OUT - The converted string 		      */
    OM_uint32   *len_used)     /* OUT - Number of chars. written to O/P string*/
{
OM_value_position  total_num;
OM_return_code     retval;
OM_integer	   retval_xoi;
OM_boolean	   is_local_string_syntax;
OM_boolean	   delete_sub_object;
OM_syntax	   syntax;
OM_object	   sub_object, p;
OM_uint32          index, l, k;
OM_uint	           comp_count, comp_val_count, len, i;
OM_string	   dn_string;
OM_type            prev_omtype = OM_NO_MORE_TYPES;
char	           abbrv[XOS_MAX_OBJID + 1];
char		   c[XOS_MAX_VAR + 1];

    if (object[0].type == OM_NO_MORE_TYPES)
	return(OM_NO_SUCH_OBJECT);

    index = 0;

    if (index + 1 > max_len)
	return(OM_MEMORY_INSUFFICIENT);

    /* Append the class start character */
    if (delim_reqd == OM_TRUE)
    {
	string[index] = XOS_SATTR_START_OP;
	++index;
    }

    comp_count = 0;
    retval = OM_SUCCESS;

    /* skip past the class identifier */
    for (i = 1; object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	/* if component is recurring */
	if (object[i].type == prev_omtype)
	{
	    ++comp_val_count;
	}
	else
	{
	    prev_omtype = object[i].type;
	    comp_val_count = 0;
	}

	/* Check if more than one component */
	if (comp_count > 0 && comp_val_count == 0)
	{
	    /* check memory */
	    if (index + 1 > max_len)
	    {
		retval = OM_MEMORY_INSUFFICIENT;
		break;
	    }

	    /* Append the XOS_COMP_DELIMITER_OP character */
	    string[index] = XOS_COMP_DELIMITER_OP;
	    ++index;
	}

	/* Append the component abbreviation only for the first */
	/* component value.					*/
	if (comp_val_count == 0)
	{
	    /* get the component abbreviation */
	    if (xoi_get_comp_name(&(object[0].value.string), object[i].type,
				  abbrv, NULL) != XOI_SUCCESS)
	    {
		retval = OMX_CLASS_NOT_FOUND_IN_SCHEMA_FILE;
		break;
	    }

	    len = strlen(abbrv);

	    /* check memory */
	    if (index + len + 1> max_len)
	    {
		retval = OM_MEMORY_INSUFFICIENT;
		break;
	    }

	    /* Append the component abbreviation */
	    strncpy(&string[index], abbrv, len);
	    index += len;

	    /* Append the XOS_ATTR_EQUALITY_OP character */
	    string[index] = XOS_ATTR_EQUALITY_OP;
	    ++index;
	}
	else
	{
	    /* check memory */
	    if (index + 1 > max_len)
	    {
		retval = OM_MEMORY_INSUFFICIENT;
		break;
	    }

	    /* Append the XOS_RECUR_OP character */
	    string[index] = XOS_RECUR_OP;
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
	    /* public object through a call to om_get.                 */
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
		/* check memory */
		if (index + 1 > max_len)
		{
		    retval = OM_MEMORY_INSUFFICIENT;
		    break;
		}

		/* Append the XOS_SATTR_START_OP character */
		string[index] = XOS_SATTR_START_OP;
		++index;

		dn_string.elements = &string[index];
		dn_string.length = max_len - index;

		/* convert the DN object to string */
		retval = xos_convert_public_name_to_str(sub_object, 
							local_strings, 
							&dn_string);
		if (retval != OM_SUCCESS)
		    break;

		/* skip past the DN string */
		index += dn_string.length;

		/* check memory */
		if (index + 1 > max_len)
		{
		    retval = OM_MEMORY_INSUFFICIENT;
		    break;
		}

		/* Append the XOS_SATTR_END_OP character */
		string[index] = XOS_SATTR_END_OP;
		++index;
	    }
	    else
	    {
		/* if object with only class descriptor */
		if (sub_object[1].type == OM_NO_MORE_TYPES)
		{
		    retval = OM_WRONG_VALUE_MAKEUP;
		    break;
		}

		/* call this function recursively */
		retval = xos_convert_class_to_string(sub_object, local_strings,
						     max_len - index, 
						     OM_TRUE, &string[index], 
						     &l);
		if (retval != OM_SUCCESS)
		    break;

		/* skip past the length */
		index += l;
	    }

	    if (delete_sub_object == OM_TRUE)
	    {
		om_delete(sub_object);
		delete_sub_object = OM_FALSE;
	    }
	}
	else
	{
	    /* force the high bits of the syntax to 0 */
	    syntax = object[i].syntax & OM_S_SYNTAX;

	    switch(syntax)
	    {
		/* if the syntax is any of these, then copy the value from  */
		/* length member of the OM_string.			    */
		case (OM_S_INTEGER):
		case (OM_S_BOOLEAN):
		case (OM_S_ENUMERATION):
		    sprintf(c, "%ld", object[i].value.string.length);
		    len = strlen(c);

		    /* check memory */
		    if (index + len > max_len)
		    {
			retval = OM_MEMORY_INSUFFICIENT;
			break;
		    }

		    strncpy(&string[index], c, len);
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
			    if (index + len > max_len)
			    {
				retval = OM_MEMORY_INSUFFICIENT;
				break;
			    }
			    memcpy(&string[index], abbrv, len);
			    index += len;
			    break;
			}
		    }

		    /* check memory */
		    if (index + 1 > max_len)
		    {
			retval = OM_MEMORY_INSUFFICIENT;
			break;
		    }

		    /* Append the XOS_ATTR_QUOTE_OP character */
		    string[index] = XOS_ATTR_QUOTE_OP;
		    ++index;

		    is_local_string_syntax = OM_FALSE;

		    /* check if the local string bit is set */
		    if (object[i].syntax & OM_S_LOCAL_STRING)
		    {
			/* for the network addresses, do not interpret */
			/* the octet value.			       */
			if (object[i].type == DS_N_ADDRESSES)
			{
			    if (local_strings == OM_TRUE)
			    {
				is_local_string_syntax = OM_TRUE;
			    }
			}
		    }
		    else
		    {
			/* for P-Selector, S-Selector and T-Selector */
			/* do not interpret the octet value.	     */
			if (object[i].type == DS_P_SELECTOR ||
			    object[i].type == DS_S_SELECTOR ||
			    object[i].type == DS_T_SELECTOR)
			{
			    if (local_strings == OM_TRUE)
			    {
				is_local_string_syntax = OM_TRUE;
			    }
			}
		    }

		    if (is_local_string_syntax == OM_TRUE)
		    {
			for (k = 0; k < object[i].value.string.length; ++k)
			{
			    /* Check memory */
			    if (index + 1 > max_len)
			    {
				retval = OM_MEMORY_INSUFFICIENT;
				break;
			    }
			    string[index] = 
				 ((char *)(object[i].value.string.elements))[k];
			    ++index;
			}
		    }
		    else
		    {
			for (k = 0; k < object[i].value.string.length; ++k)
			{
			    /* Check memory */
			    if (index + 5 > max_len)
			    {
				retval = OM_MEMORY_INSUFFICIENT;
				break;
			    }

			    string[index] = XOS_ESCAPE_OP;
			    ++index;
			    string[index] = XOS_HEX_OP;
			    ++index;
			    sprintf(&string[index], "%02x",
				((char *)(object[i].value.string.elements))[k]);
			    index += 2;
			}
		    }

		    if (retval != OM_SUCCESS)
			break;

		    /* check memory */
		    if (index + 1 > max_len)
		    {
			retval = OM_MEMORY_INSUFFICIENT;
			break;
		    }

		    /* Append the XOS_ATTR_QUOTE_OP character */
		    string[index] = XOS_ATTR_QUOTE_OP;
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
			    case (XOS_SATTR_START_OP):
			    case (XOS_SATTR_END_OP):
			    case (XOS_COMP_DELIMITER_OP):
				/* check memory */
				if (index + 1 > max_len)
				{
				    retval = OM_MEMORY_INSUFFICIENT;
				    break;
				}

				string[index] = XOS_ESCAPE_OP;
				++index;
				break;
			    default:
				break;
			}

			if (retval != OM_SUCCESS)
			    break;

			/* check memory */
			if (index + 1 > max_len)
			{
			    retval = OM_MEMORY_INSUFFICIENT;
			    break;
			}

			string[index] = ((char *)
					 (object[i].value.string.elements))[k];
			++index;
		    }
		    break;
	    }
	    if (retval != OM_SUCCESS)
		break;
	}
	++comp_count;
    }

    if (retval == OM_SUCCESS)
    {
	if (delim_reqd == OM_TRUE)
	{
	    if (index + 1 > max_len)
	    {
		retval = OM_MEMORY_INSUFFICIENT;
	    }
	    else
	    {
		string[index] = XOS_SATTR_END_OP;
		*len_used = index + 1;
	    }
	}
	else
	{
	    *len_used = index;
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
