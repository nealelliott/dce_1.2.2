/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xoi.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:09  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:23  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:51:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:36  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:22:41  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:43  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:50:29  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:49  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:19  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:20:11  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:58:11  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOI - XOM Object Info Module                      */
/* File Name   : xoi.c					   	   */
/* Description : This file contains all the source code for the	   */
/*		 public XOI convenience functions.		   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xoi.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:09 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <dce/dce.h>

#include <xom.h>

#include <xoistruct.h>
#include <xoiinit.h>
#include <xoisort.h>
#include <xoiutil.h>
#include <xoi.h>

/*-- Local Define(s) ----------------------------------------------*/
/* Define for the Schema file name				   */
#define XOI_DEF_SCHEMA_NAME	"xoischema"

/* Define for the directory where the default Schema file is stored */
#define XOI_INST_DIR		"/var/adm/directory/gds/adm/"

/* Define for the environment variable to read the full path name  */
/* of the user provided Schema file                                */
#define XOI_USR_SCHEMA_ENV      "XOI_SCHEMA_FILE"

/*-- Static Variable(s) -------------------------------------------*/
static	OM_boolean	xoi_initialized = OM_FALSE;

#ifdef THREADSAFE
static  OM_uint		xoi_line_no;
static  OM_integer	xoi_retval;
#endif

/*-- Static Function(s) -------------------------------------------*/
static void xoi_release_memory(
    void);

#ifdef THREADSAFE
static void xoi_init_once(
    void);
#endif


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_initialize				   */
/* Description   : This function is used to read the Schema file   */
/*                 and build up the internal data structures.      */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_initialize(
    OM_uint *line_no)  /* OUT - Contains erroneous line No.in case of errors */
{
#ifdef THREADSAFE
static pthread_once_t once_block = pthread_once_init;
#else
OM_integer  retval;
#endif

    if (line_no == NULL)
	return(XOI_INVALID_PARAMETER);

    if (xoi_initialized == OM_TRUE)
	return(XOI_SUCCESS);

#ifdef THREADSAFE
    pthread_once(&once_block, xoi_init_once);
    if (xoi_retval != XOI_SUCCESS)
    {
	*line_no = xoi_line_no;
	return(xoi_retval);
    }
#else
    if ((retval = xoi_init(line_no)) != XOI_SUCCESS)
	return(retval);
    xoi_initialized = OM_TRUE;
#endif

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_init_once			   	   */
/* Description   : This function invokes the xoi_init function.    */
/*                 This is used for making the code THREADSAFE     */
/*		   and ensuring that xoi_init is invoked only once */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
#ifdef THREADSAFE
static void xoi_init_once(
    void)
{
    xoi_retval = xoi_init(&xoi_line_no);

    if (xoi_retval == XOI_SUCCESS)
	xoi_initialized = OM_TRUE;
}
#endif


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_free				   	   */
/* Description   : This function is used to free all the private   */
/*		   data area used by XOI.	   		   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_free(
    void)
{
#ifdef THREADSAFE
static pthread_once_t once_block = pthread_once_init;
#endif

    if (xoi_initialized == OM_FALSE)
	return;

#ifdef THREADSAFE
    pthread_once(&once_block, xoi_release_memory);
#else
    xoi_release_memory();
#endif
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_release_memory			   	   */
/* Description   : This function is used to free all the private   */
/*		   data area used by XOI.	   		   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void xoi_release_memory(
    void)
{
    /* Free the Object Class and Attribute Information Structures */
    xoi_free_blockinfo();

    /* Free the sorted arrays used for binary search */
    xoi_free_sort_structures();
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_objclass_count			   */
/* Description   : This function is used to get the count of       */
/*		   object classes defined in the schema file.      */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_objclass_count(
    OM_uint *obj_count)  /* OUT - Count of number of object classes */
{
xoi_object_class  *obj_class_ptr;
OM_integer  	   retval;

    if (obj_count == NULL)
	return(XOI_INVALID_PARAMETER);

    retval = XOI_SUCCESS;

    if (xoi_initialized == OM_FALSE)
    {
	retval = XOI_SCHEMA_NOT_READ;
    }
    else
    {
	obj_class_ptr = xoi_get_objclass_ptr();
	if (obj_class_ptr == NULL)
	{
	    *obj_count = 0;
	}
	else
	{
	    *obj_count = obj_class_ptr->count;
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_obj_info_from_index			   */
/* Description   : This function is used to get the information    */
/*		   about the object class 			   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_obj_info_from_index(
    OM_uint  index,     /* IN  - Index into the object class array */
    char    *obj_id,    /* OUT - Object identifier                 */
    char    *abbrv,     /* OUT - Object Class abbreviation         */
    char    *full_name) /* OUT - Object Class full name            */
{
xoi_object_class  *obj_class_ptr;
xoi_oinfo	  *oinfo;
OM_integer	   retval;
OM_uint		   orig_index;

    retval = XOI_SUCCESS;

    if (xoi_initialized == OM_FALSE)
    {
	retval = XOI_SCHEMA_NOT_READ;
    }
    else
    {
	/* get the object class information pointer */
	obj_class_ptr = xoi_get_objclass_ptr();
	if (obj_class_ptr == NULL || index + 1 > obj_class_ptr->count)
	{
	    retval = XOI_UNKNOWN_OBJ_ID;
	}
	else
	{
	    /* convert the index to the original index */
	    orig_index = xoi_get_oinfo_system_index(index);

	    oinfo = &(obj_class_ptr->obj[orig_index]);

	    if (obj_id != NULL)
	    {
		/* converted the identifier to its dotted representation */
		xoi_ber_to_string(oinfo->e_objid, obj_id);
	    }

	    if (abbrv != NULL)
	    {
		/* convert and copy the abbreviation to upper case */
		xoi_strcpy_to_upper(abbrv, oinfo->abbrv);
	    }

	    if (full_name != NULL)
	    {
		strcpy(full_name, oinfo->name);
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_objclass_objid			   */
/* Description   : This function is used to get the BER Encoded    */
/*		   Object Identifier string for the Object Class.  */
/*		   The Object Class can be an abbreviation or a    */
/*		   string (Eg: 85.4.6)				   */
/*		   The memory space for the encoded identifier is  */
/*		   provided by this function.			   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_objclass_objid(
    char      *objclass,    /* IN  - Object Class Abbreviation or string */
    void     **e_objid,     /* OUT - The Encoded identifier		 */
    OM_uint32 *e_objid_len) /* OUT - Length of  the identifier		 */
{
OM_integer   retval;
OM_boolean   completed;
OM_string   *p;
xoi_oinfo   *obj;

    if (objclass == NULL || e_objid == NULL || e_objid_len == NULL)
	return(XOI_INVALID_PARAMETER);

    p = NULL;
    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	if (xoi_initialized == OM_FALSE)
	{
	    retval = XOI_SCHEMA_NOT_READ;
	    break;
	}

	/* If the Object Class is an abbreviation */
	if (isalpha(*objclass))
	{
	    /* Get the Object Class Information */
	    retval = xoi_get_oinfo_from_abbrv(objclass, &obj);
	    if (retval != XOI_SUCCESS)
		break;
	}
	else
	{  /* Object Class is in the form of identifier string (Eg: 85.4.6) */

	    /* Converted to BER Encoded form */
	    if ((retval = xoi_string_to_ber(objclass, &p)) != XOI_SUCCESS)
		break;

	    /* Get the Object Class Information */
	    if ((retval = xoi_get_oinfo_from_objid(p, &obj)) != XOI_SUCCESS)
		break;
	}

	/* Now create the string and set the user data pointer */
	*e_objid = (void *) malloc(sizeof(char) * obj->e_objid->length);
	if (*e_objid == NULL)
	{
	    retval  = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

	memcpy(*e_objid, obj->e_objid->elements, obj->e_objid->length);
	*e_objid_len = obj->e_objid->length;

	completed = OM_TRUE;
    }

    if (p != NULL)
	xoi_free_omstring(p);

    if (completed == OM_FALSE)
    {
	*e_objid = NULL;
	*e_objid_len = 0;
	return(retval);
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_objclass_name			   */
/* Description   : This function is used to get the Object Class   */
/*		   Abbreviation and full name. If any of the output*/
/*		   parameters is NULL, that particular information */
/*		   is not returned.				   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_objclass_name(
    OM_string *e_objid,   /* IN  - Encoded Object Identifier */
    char      *abbrv,     /* OUT - Object Class Abbreviation */
    char      *full_name) /* OUT - Object Class Full Name    */
{
OM_integer   retval;
OM_boolean   completed;
xoi_oinfo   *obj;

    if (e_objid == NULL)
	return(XOI_INVALID_PARAMETER);

    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	if (xoi_initialized == OM_FALSE)
	{
	    retval = XOI_SCHEMA_NOT_READ;
	    break;
	}

	/* Get the Object Class Information */
	if ((retval = xoi_get_oinfo_from_objid(e_objid, &obj)) != XOI_SUCCESS)
	    break;

	/* Now copy the data to the user supplied space */ 
	if (abbrv != NULL)
	{
	    /* convert and copy abbreviation to upper case */
	    xoi_strcpy_to_upper(abbrv, obj->abbrv);
	}

	if (full_name != NULL)
	{
	    strcpy(full_name, obj->name);
	}

	completed = OM_TRUE;
    }

    if (completed == OM_FALSE)
    {
	if (abbrv != NULL)
	    *abbrv = 0;
	if (full_name != NULL)
	    *full_name = 0;
	return(retval);
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_attribute_count			   */
/* Description   : This function is used to get the count of       */
/*		   attributes defined in the schema file.          */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_attribute_count(
    OM_uint *attr_count)  /* OUT - Count of number of attributes */
{
OM_integer  	retval;
xoi_attribute  *attr_ptr;

    if (attr_count == NULL)
	return(XOI_INVALID_PARAMETER);

    retval = XOI_SUCCESS;

    if (xoi_initialized == OM_FALSE)
    {
	retval = XOI_SCHEMA_NOT_READ;
    }
    else
    {
	attr_ptr = xoi_get_attribute_ptr();
	if (attr_ptr == NULL)
	{
	    *attr_count = 0;
	}
	else
	{
	    *attr_count = attr_ptr->count;
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_attr_info_from_index			   */
/* Description   : This function is used to get the information    */
/*		   about the attribute 				   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_attr_info_from_index(
    OM_uint  index,     /* IN  - Index into the attribute array */
    char    *obj_id,    /* OUT - Attribute Object identifier    */
    char    *abbrv,     /* OUT - Attribute abbreviation         */
    char    *full_name) /* OUT - Attribute full name            */
{
OM_integer  	retval;
OM_uint  	orig_index;
xoi_attribute  *attr_ptr;
xoi_ainfo      *ainfo;

    retval = XOI_SUCCESS;

    if (xoi_initialized == OM_FALSE)
    {
	retval = XOI_SCHEMA_NOT_READ;
    }
    else
    {
	/* get the attribute information pointer */
	attr_ptr = xoi_get_attribute_ptr();

	if (attr_ptr == NULL || index + 1 > attr_ptr->count)
	{
	    retval = XOI_UNKNOWN_OBJ_ID;
	}
	else
	{
	    /* convert the index to the original index */
	    orig_index = xoi_get_ainfo_system_index(index);

	    ainfo = &(attr_ptr->attr[orig_index]);

	    if (obj_id != NULL)
	    {
		/* convert the identifier to its dotted representation */
		xoi_ber_to_string(ainfo->e_objid, obj_id);
	    }

	    if (abbrv != NULL)
	    {
		/* convert and copy the abbreviation to upper case */
		xoi_strcpy_to_upper(abbrv, ainfo->abbrv);
	    }

	    if (full_name != NULL)
	    {
		strcpy(full_name, ainfo->name);
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_attr_details			   	   */
/* Description   : This function is used to get the details of an  */
/*		   Attribute.  The input parameter could be an     */
/*		   Abbreviation or an Object Id.string (Eg:85.4.6) */
/*		   The memory space for the Object Identifier and  */
/*		   the Class Identifier is provided by this 	   */
/*		   function. If any of the output parameters is    */
/*		   NULL, that particular information is not        */
/*		   returned.	   			   	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_attr_details(
    char       *attribute,   /* IN  - Attribute as an Abbreviation or a string*/
    OM_syntax  *omsyntax,    /* OUT - XOM syntax			      */
    void      **e_objid,     /* OUT - The Encoded identifier.		      */
    OM_uint32  *e_objid_len, /* OUT - Length of the identifier		      */
    void      **omclass,     /* OUT - XOM Class				      */
    OM_uint32  *omclass_len) /* OUT - Length of the Class identifier          */
{
OM_integer   retval;
OM_boolean   completed;
OM_string   *p;
xoi_ainfo   *attr;

    if (attribute == NULL)
	return(XOI_INVALID_PARAMETER);

    if (e_objid != NULL && e_objid_len == NULL)
	return(XOI_INVALID_PARAMETER);

    if (omclass != NULL && omclass_len == NULL)
	return(XOI_INVALID_PARAMETER);

    p = NULL;
    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	if (xoi_initialized == OM_FALSE)
	{
	    retval = XOI_SCHEMA_NOT_READ;
	    break;
	}

	/* If the Attribute is an abbreviation */
	if (isalpha(*attribute))
	{
	    /* Get the Attribute Information */
	    retval = xoi_get_ainfo_from_abbrv(attribute, &attr);
	    if (retval != XOI_SUCCESS)
		break;
	}
	else
	{  /* Attribute is in the form of an identifier string (Eg: 85.4.6) */

	    /* Converted to BER Encoded form */
	    if ((retval = xoi_string_to_ber(attribute, &p)) != XOI_SUCCESS)
		break;

	    /* Get the Attribute Information */
	    if ((retval = xoi_get_ainfo_from_objid(p, &attr)) != XOI_SUCCESS)
		break;
	}

	if (omsyntax != NULL)
	    *omsyntax = attr->omsyntax;

	if (e_objid != NULL)
	{
	    /* Create the string and set the user data pointer */
	    *e_objid = (void *) malloc(sizeof(char) * attr->e_objid->length);
	    if (*e_objid == NULL)
	    {
		retval  = XOI_MEMORY_INSUFFICIENT;
		break;
	    }

	    memcpy(*e_objid, attr->e_objid->elements, attr->e_objid->length);
	    *e_objid_len = attr->e_objid->length;
	}

	if (omclass != NULL)
	{
	    /* If attribute is structured */
	    if (attr->omsyntax == OM_S_OBJECT)
	    {
		/* Create the string and set the user data pointer */
		*omclass = (void *)
			   malloc(sizeof(char) * attr->class->omclass->length);
		if (*omclass == NULL)
		{
		    retval = XOI_MEMORY_INSUFFICIENT;
		    break;
		}

		memcpy(*omclass, attr->class->omclass->elements,
  		       attr->class->omclass->length);

		*omclass_len = attr->class->omclass->length;
	    }
	    else
	    {
		/* It is a normal attribute */
		*omclass = NULL;
		*omclass_len = 0;
	    }
	}
	completed = OM_TRUE;
    }

    if (p != NULL)
	xoi_free_omstring(p);

    if (completed == OM_FALSE)
    {
	if (omsyntax != NULL)
	    *omsyntax = 0;
	if (e_objid != NULL)
	{
	    *e_objid = NULL;
	    *e_objid_len = 0;
	}
	if (omclass != NULL)
	{
	    *omclass = NULL;
	    *omclass_len = 0;
	}
	return(retval);
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_attr_name	 			   */
/* Description   : This function is used to get the Attribute 	   */
/*		   Abbreviation and full name.  If any of the      */
/*		   output parameters is NULL, that particular      */
/*		   information is not returned.   		   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_attr_name(
    OM_string *e_objid,   /* IN  - Encoded Object Identifier */
    char      *abbrv,     /* OUT - Object Class Abbreviation */
    char      *full_name) /* OUT - Object Class Full Name    */
{
OM_integer   retval;
OM_boolean   completed;
xoi_ainfo   *attr;

    if (e_objid == NULL)
	return(XOI_INVALID_PARAMETER);

    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	if (xoi_initialized == OM_FALSE)
	{
	    retval = XOI_SCHEMA_NOT_READ;
	    break;
	}

	/* Get the Attribute Information */
	if ((retval = xoi_get_ainfo_from_objid(e_objid, &attr)) != XOI_SUCCESS)
	    break;

	/* Now copy the data to the user supplied space */ 
	if (abbrv != NULL)
	{
	    /* convert the abbreviation to upper case and copy */
	    xoi_strcpy_to_upper(abbrv, attr->abbrv);
	}

	if (full_name != NULL)
	    strcpy(full_name, attr->name);

	completed = OM_TRUE;
    }

    if (completed == OM_FALSE)
    {
	if (abbrv != NULL)
	    *abbrv = 0;
	if (full_name != NULL)
	    *full_name = 0;
	return(retval);
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_class_count			           */
/* Description   : This function is used to get the count of       */
/*		   classes (structured attributes) defined in the  */
/*		   schema file.          			   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_class_count(
    OM_uint *class_count)  /* OUT - Count of number of classes */
{
OM_integer 	     retval;
xoi_omclass_struct  *omclass_ptr;

    if (class_count == NULL)
	return(XOI_INVALID_PARAMETER);

    retval = XOI_SUCCESS;

    if (xoi_initialized == OM_FALSE)
    {
	retval = XOI_SCHEMA_NOT_READ;
    }
    else
    {
	omclass_ptr = xoi_get_omclass_ptr();
	if (omclass_ptr == NULL)
	{
	    *class_count = 0;
	}
	else
	{
	    *class_count = omclass_ptr->count;
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_class_info_from_index		   */
/* Description   : This function is used to get the information    */
/*		   about the class 				   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_class_info_from_index(
    OM_uint   index,       /* IN  - Index into the OM Class array      */
    char     *class_name,  /* OUT - Class Name                         */
    OM_uint  *comp_count)  /* OUT - Number of components of this class */
{
OM_integer           retval;
xoi_omclass_struct  *omclass_ptr;
xoi_class           *class;

    retval = XOI_SUCCESS;

    if (xoi_initialized == OM_FALSE)
    {
	retval = XOI_SCHEMA_NOT_READ;
    }
    else
    {
	omclass_ptr = xoi_get_omclass_ptr();
	if (omclass_ptr == NULL || index + 1 > omclass_ptr->count)
	{
	    retval = XOI_UNKNOWN_OBJ_ID;
	}
	else
	{
	    class = &(omclass_ptr->class[index]);

	    if (class_name != NULL)
	    {
		xoi_get_omclass_define(class->omclass, class_name);
	    }

	    if (comp_count != NULL)
	    {
		*comp_count = class->count;
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_comp_info_from_index		           */
/* Description   : This function is used to get the information    */
/*		   about the components of the class 		   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_comp_info_from_index(
    OM_uint  class_index, /* IN  - Index into the OM Class array      */
    OM_uint  comp_index,  /* IN  - Index into the component array     */
    char    *abbrv,       /* OUT - Component abbreviation             */
    char    *full_name)   /* OUT - Component full name                */
{
OM_integer  	     retval;
xoi_omclass_struct  *omclass_ptr;
xoi_class           *class;
xoi_cinfo	    *cinfo;

    retval = XOI_SUCCESS;

    if (xoi_initialized == OM_FALSE)
    {
	retval = XOI_SCHEMA_NOT_READ;
    }
    else
    {
	omclass_ptr = xoi_get_omclass_ptr();
	if (omclass_ptr == NULL || class_index + 1 > omclass_ptr->count)
	{
	    retval = XOI_UNKNOWN_OBJ_ID;
	}
	else
	{
	    class = &(omclass_ptr->class[class_index]);

	    if (comp_index + 1 > class->count)
	    {
		retval = XOI_UNKNOWN_OMTYPE;
	    }
	    else
	    {
		cinfo = &(class->comps[comp_index]);
		if (abbrv != NULL)
		{
		    /* convert and copy the abbreviation to upper case */
		    xoi_strcpy_to_upper(abbrv, cinfo->abbrv);
		}

		if (full_name != NULL)
		{
		    strcpy(full_name, cinfo->name);
		}
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_comp_details 			   */
/* Description   : This function is used to get the details of the */
/*		   component of an OM Class Name. If any of the    */
/*		   output parameters is NULL, that particular      */
/*		   information is not returned.			   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_comp_details(
    OM_string  *omclass,        /* IN  - XOM Class  		       */
    char       *abbrv,          /* IN  - Abbreviation of the component */
    OM_type    *omtype,         /* OUT - XOM Type		       */
    OM_syntax  *omsyntax,       /* OUT - XOM Syntax		       */
    void      **comp_class,     /* OUT - Class Name of the Component   */
    OM_uint32  *comp_class_len) /* OUT - Length of the Class Name      */
{
OM_integer   retval;
OM_boolean   completed;
xoi_cinfo   *comp;

    if (omclass == NULL || abbrv == NULL)
	return(XOI_INVALID_PARAMETER);

    if (comp_class != NULL && comp_class_len == NULL)
	return(XOI_INVALID_PARAMETER);

    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	if (xoi_initialized == OM_FALSE)
	{
	    retval = XOI_SCHEMA_NOT_READ;
	    break;
	}

	/* Get the class component information */
	retval = xoi_get_cinfo_from_abbrv(omclass, abbrv, &comp);
	if (retval != XOI_SUCCESS)
	    break;

	/* Now copy the data to the user supplied space */ 
	if (omtype != NULL)
	    *omtype = comp->omtype;

	if (omsyntax != NULL)
	    *omsyntax = comp->omsyntax;

	if (comp_class != NULL)
	{
	    /* Component is structured */
	    if (comp->omsyntax == OM_S_OBJECT)
	    {
		/* Create the string and set the user data pointer */
		*comp_class = (void *) 
			      malloc(sizeof(char) * 
				     comp->comp_class->omclass->length);
		if (*comp_class == NULL)
		{
		    retval = XOI_MEMORY_INSUFFICIENT;
		    break;
		}

		memcpy(*comp_class, comp->comp_class->omclass->elements,
		       comp->comp_class->omclass->length);

		*comp_class_len = comp->comp_class->omclass->length;
	    }
	    else
	    {
		/* It is a normal component */
		*comp_class = NULL;
		*comp_class_len = 0;
	    }
	}

	completed = OM_TRUE;
    }

    if (completed == OM_FALSE)
    {
	if (omtype != NULL)
	    *omtype = 0;
	if (omsyntax != NULL)
	    *omsyntax = 0;
	if (comp_class != NULL)
	{
	    *comp_class = NULL;
	    *comp_class_len = 0;
	}
	return(retval);
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_comp_name	 			   */
/* Description   : This function is used to get the Abbreviation   */
/*		   and full name of the components of an OM Class  */
/*		   Name. If any of the output parameters is NULL,  */
/*		   that particular information is not returned.	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_comp_name(
    OM_string *omclass,   /* IN  - XOM Class		  */
    OM_type    omtype,    /* IN  - XOM Type		  */
    char      *abbrv,     /* OUT - Component Abbreviation */
    char      *full_name) /* OUT - Component Full Name    */
{
OM_integer   retval;
OM_boolean   completed;
xoi_cinfo   *comp;

    if (omclass == NULL)
	return(XOI_INVALID_PARAMETER);

    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	if (xoi_initialized == OM_FALSE)
	{
	    retval = XOI_SCHEMA_NOT_READ;
	    break;
	}

	/* Get the class component information */
	retval = xoi_get_cinfo_from_omtype(omclass, omtype, &comp);
	if (retval !=  XOI_SUCCESS)
	    break;

	/* Now copy the data to the user supplied space */ 
	if (abbrv != NULL)
	{
	    /* convert the abbreviation to upper case and copy */
	    xoi_strcpy_to_upper(abbrv, comp->abbrv);
	}

	if (full_name != NULL)
	    strcpy(full_name, comp->name);

	completed = OM_TRUE;
    }

    if (completed == OM_FALSE)
    {
	if (abbrv != NULL)
	    *abbrv = 0;
	if (full_name != NULL)
	    *full_name = 0;
	return(retval);
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_schema_file_name		   	   */
/* Description   : This function is used to get the full name      */
/*		   of the schema file.  The calling function       */
/* 		   should provide the memory space for the input   */
/*		   parameter.					   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_schema_file_name(
    char  *file_name)  /* OUT - Name of the schema file */
{
char  *env_ptr;

    if (file_name == NULL)
    {
	return(XOI_INVALID_PARAMETER);
    }

    *file_name = '\0';

    /* Check environment variable for user defined schema file. */
    if ((env_ptr = getenv(XOI_USR_SCHEMA_ENV)) != NULL)
    {
	strcpy(file_name, env_ptr);
    }
    else
    {
	if (dcelocal_path != NULL)
	{
	    sprintf(file_name, "%s%s%s", dcelocal_path, XOI_INST_DIR, 
					 XOI_DEF_SCHEMA_NAME); 
	}
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_string_to_ber			   	   */
/* Description   : This function is used to convert the Object     */
/*		   Identifier String to BER Encoded form. The      */
/* 		   memory space for the Encoded string is provided */
/*		   by this function 	   			   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_string_to_ber(
    char       *objid,    /* IN  - Object Identifier String      */
    OM_string **e_objid)  /* OUT - BER Encoded Object Identifier */
{
OM_string  *p;
OM_uint32   len = 0;
OM_uint32   n, m;
OM_uint     octet_count;
OM_uint     i, t;
char	    enstr[XOI_MAX_OBJID + 1];
char  	    s[XOI_MAX_OBJID + 1];

    if (e_objid == NULL)
	return(XOI_INVALID_PARAMETER);

    *e_objid = NULL;

    if (objid == NULL)
	return(XOI_INVALID_OBJ_ID);

    /* Skip past the white space */
    while (isspace(*objid))
	++objid;

    /* First character should be a digit */
    if (!*objid || !isdigit(*objid))
	return(XOI_INVALID_OBJ_ID);

    while (*objid)
    {
	for (i = 0; *objid && *objid != '.'; ++i, ++objid)
	{
	    /* check for digits */
	    if (isdigit(*objid))
		s[i] = *objid;
	    else
		return(XOI_INVALID_OBJ_ID);
	}	

	if (i == 0)
	    return(XOI_INVALID_OBJ_ID);

	s[i] = 0;
	n = atol(s);

	/* String to be converted to multiple octets */
	if (n > 127)
 	{
	    m = n;
	    octet_count = 2;

	    /* Compute the total number of octets required */
	    while ((m = m/128) > 127)
		++octet_count;

	    t = octet_count;

	    /* Fill the last octet */
	    enstr[len+octet_count-1] = n % 128;

	    --octet_count;
	    n = n/128;	

	    for (; octet_count > 1; --octet_count, n = n/128)
	        enstr[len+octet_count-1] = (n % 128) | 128;

	    /* Fill the first octet */
	    enstr[len+octet_count-1] =  n | 128;

	    /* Skip past filled array */
	    len += t;
	}
	else
	{
	    enstr[len] = n;
	    ++len;
	}
	
	/* Skip past the '.' character */
	if (*objid == '.')
	    ++objid;
    }

    /* If last character is '.' then error */
    if (*--objid == '.')
	return(XOI_INVALID_OBJ_ID);

    if ((p = xoi_create_omstring(enstr, len)) == NULL)
	return(XOI_MEMORY_INSUFFICIENT);

    *e_objid = p;

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_ber_to_string		   	   	   */
/* Description   : This function is used to convert a BER Encoded  */
/*		   Object Identifier to the string form (Eg:85.4.6)*/
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_ber_to_string(
    OM_string *e_objid, /* IN  - Encoded Object Identifier */
    char      *objid)   /* OUT - Converted string	   */
{
OM_uint32  n, bitval;
OM_uint    len;
OM_uint    octet_count;
OM_uint	   i, k, a;
char  	   c[XOI_MAX_OBJID + 1];	

    *objid = 0;

    if (e_objid == NULL || e_objid->elements == NULL || 
	e_objid->length <= 0 || objid == NULL)
	return(XOI_INVALID_PARAMETER);

    len = (OM_uint) (e_objid->length);

    for (i = 0; i < len; ++i)
    {
	/* Compute the number of Octets in the current part */
	for (octet_count = 1; i < len; ++i)
	{
	    if (((unsigned char *) e_objid->elements)[i] < 128)
		break;
	    ++octet_count;
	}

	/* There should be a value with the signed bit 0 after  */
  	/* successive values with signed bit 1.			*/
	if (i >= len)
	{
	    *objid = 0;
	    return(XOI_INVALID_OBJ_ID);
	}

	for (bitval = 1, k = i, n = 0; octet_count > 0; --octet_count, --k)
	{
	    /* If the signed bit is 0 */
	    if (((unsigned char *) e_objid->elements)[k] < 128)
		a = ((unsigned char *) e_objid->elements)[k];
	    else
		a = ((unsigned char *) e_objid->elements)[k] - 128;
	    n = n + a * bitval;
	    bitval = bitval*128;
	}

	/* Convert to character string */
	sprintf(c, "%lu", n);
	strcat(objid, c);

	/* Add the "." separator */
	if (i + 1 < len)
	    strcat(objid, ".");
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
