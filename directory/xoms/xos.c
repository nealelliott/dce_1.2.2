/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xos.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:18  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:51:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:49  root]
 * 
 * Revision 1.1.2.6  1994/08/24  19:53:48  keutel
 * 	OT 11844
 * 	[1994/08/24  15:43:57  keutel]
 * 
 * Revision 1.1.2.5  1994/06/21  14:49:06  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:49:41  marrek]
 * 
 * Revision 1.1.2.4  1994/06/10  21:22:56  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:51  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  16:04:30  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  06:41:11  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:47  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  15:50:34  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:39:18  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:26  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:26:15  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:59:20  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xos.c					   	   */
/* Description : This file contains all the routines for the       */
/*		 string to object and object to string functions   */
/* Date	       : July 1, 1993					   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xos.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:18 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <nl_types.h>

#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdsimport.h>

#include <xomext.h>
#include <xoi.h>

#include <xosstruct.h>
#include <xosutil.h>
#include <xosname.h>
#include <xosattr.h>
#include <xosclass.h>
#include <xosfilter.h>
#include <xos.h>

/*-- Local Define(s) ----------------------------------------------*/
#define XOS_NLS_FILE	"xoserr.cat"

/*-- Static Variable(s) -------------------------------------------*/
static  OM_integer 	     xos_init_retval;
static  nl_catd		     xos_nls_desc;

/* import the necessary object identifier constants */
OM_IMPORT(dsP_DS_C_DS_DN)
OM_IMPORT(dsP_DS_C_ATTRIBUTE)
OM_IMPORT(dsP_DS_C_FILTER)
OM_IMPORT(dsP_DS_C_ATTRIBUTE_ERROR)
OM_IMPORT(dsP_DS_C_ERROR)

/*-- Static Function(s) -------------------------------------------*/
static OM_integer xos__invoke_xoi_initialize(
    void);

static void xos__invoke_xoi_initialize_once(
    void);

static OM_return_code xos__string_to_name(
    OM_workspace       workspace,      /* IN  - The workspace		      */
    OM_string         *string,         /* IN  - The string to be converted    */
    OM_boolean         is_rdn,         /* IN  - Indicates RDN is to be created*/
    OM_boolean         local_strings,  /* IN  - To specify local strings      */
    OM_private_object *name,           /* OUT - The converted DN	      */
    OM_integer	      *error_position, /* OUT - Error Position in input string*/
    OM_integer	      *error_type);    /* OUT - Type of error 		      */

static OM_return_code xos__name_to_string(
    OM_object    name,	        /* IN  - The Dist.Name to be converted       */
    OM_boolean   is_rdn,        /* IN  - Indicates an RDN object             */
    OM_boolean	 local_strings, /* IN  - To indicate local string conversion */
    OM_string	*string);       /* OUT - The converted DN string 	     */

static OM_return_code xos__str_to_obj(
    OM_workspace          workspace,     /* IN  - The workspace		      */
    OM_string            *string,        /* IN  - The string to be converted  */
    OM_object_identifier  class,         /* IN  - The OM Class to be created  */
    OM_boolean            local_strings, /* IN  - To specify local strings    */
    OM_private_object    *object,        /* OUT - The converted Object	      */
    OM_integer	         *error_position,/* OUT - Error Position in I/P string*/
    OM_integer	         *error_type);   /* OUT - Type of error 	      */

static OM_return_code xos__obj_to_str(
    OM_object    object,        /* IN  - The Object to be converted          */
    OM_boolean	 local_strings, /* IN  - To indicate local string conversion */
    OM_string	*string);       /* OUT - The converted DN string 	     */

static OM_return_code xos__string_to_filter(
    OM_workspace       workspace,      /* IN  - The workspace		      */
    OM_string         *string,         /* IN  - The string to be converted    */
    OM_boolean         local_strings,  /* IN  - To specify local strings      */
    OM_private_object *filter,         /* OUT - The converted filter	      */
    OM_integer	      *error_position, /* OUT - Error Position in input string*/
    OM_integer	      *error_type);    /* OUT - Type of error 		      */

static OM_return_code xos__error_to_string(
    DS_status   error,         /* IN  - Object containing the error   	    */
    OM_boolean  is_attr_error, /* IN  - Indicates DS_C_ATTRIBUTE_ERROR      */
    OM_boolean	local_strings, /* IN  - To indicate local string conversion */
    OM_string  *string);       /* OUT - The converted error message   	    */

/*-- External Function(s) -----------------------------------------*/
extern void dsP_trace_msg(int, char *, ...);


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_string_to_object			   	   */
/* Description   : This function is used to convert an OM object   */
/*		   specified in string format to its OM descriptor */
/*		   format.					   */
/*                 The memory space for the returned OM_object is  */
/*                 provided by this function.                      */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_string_to_object(
    OM_workspace          workspace,     /* IN  - The workspace               */
    OM_string            *string,        /* IN  - The string to be converted  */
    OM_object_identifier  class,         /* IN  - The OM Class to be created  */
    OM_boolean            local_strings, /* IN  - To specify local strings    */
    OM_private_object    *object,        /* OUT - The converted Object        */
    OM_integer           *error_position,/* OUT - Error Position in I/P string*/
    OM_integer           *error_type)    /* OUT - Type of error               */
{
OM_return_code   retval;

    /* check if a DS_C_DS_DN object is required */
    if (class.length == dsP_DS_C_DS_DN.length &&
	memcmp(class.elements, dsP_DS_C_DS_DN.elements, 
	       dsP_DS_C_DS_DN.length) == 0)
    {
	/* convert the string to DS_C_DS_DN object */
	retval = xos__string_to_name(workspace, string, OM_FALSE, local_strings,
				     object, error_position, error_type);
    }
    else
    {
	/* check if a DS_C_DS_RDN object is required */
	if (class.length == dsP_DS_C_DS_RDN.length &&
	    memcmp(class.elements, dsP_DS_C_DS_RDN.elements,
		   dsP_DS_C_DS_RDN.length) == 0)
	{
	    /* convert the string to DS_C_DS_RDN object */
	    retval = xos__string_to_name(workspace, string, OM_TRUE, 
					 local_strings, object, 
					 error_position, error_type);
	}
	else
	{
	    /* check if a DS_C_FILTER object is required */
	    if (class.length == dsP_DS_C_FILTER.length &&
		memcmp(class.elements,
		       dsP_DS_C_FILTER.elements,
		       dsP_DS_C_FILTER.length) == 0)
	    {
		/* convert the string to DS_C_FILTER object */
		retval = xos__string_to_filter(workspace, string, local_strings,
					       object, error_position, 
					       error_type);
	    }
	    else
	    {
		/* convert the string to object */
		retval = xos__str_to_obj(workspace, string, class, 
					 local_strings, object, 
					 error_position, error_type);
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_object_to_string			   	   */
/* Description   : This function is used to convert an OM object   */
/*		   to its string format. The object can either be  */
/*		   a private object, a service generated public    */
/*                 object or a client generated object.            */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_object_to_string(
    OM_object    object,        /* The Object to be converted          */
    OM_boolean   local_strings, /* To indicate local string conversion */
    OM_string   *string)        /* The converted DN string             */
{
OM_return_code  retval;
OM_boolean	instance;

    /* if a service generated object */
    if (!XOS_CPUB(object))
    {
	/* check if DS_C_DS_DN has been passed */
	retval = om_instance(object, dsP_DS_C_DS_DN, &instance);

	/* om_instance returns an error code "OM_NO_SUCH_CLASS" if  */
	/* the object has been negotiated with a different package. */
	/* So set the retval to OM_SUCCESS and instance to OM_FALSE */
	if (retval == OM_NO_SUCH_CLASS)
	{
	    retval = OM_SUCCESS;
	    instance = OM_FALSE;
	}

	if (retval == OM_SUCCESS)
	{
	    if (instance == OM_TRUE)
	    {
		/* convert the name to string */
		retval = xos__name_to_string(object, OM_FALSE, local_strings, 
					     string);
	    }
	    else
	    {
		/* check if DS_C_DS_RDN has been passed */
		retval = om_instance(object, dsP_DS_C_DS_RDN, &instance);

		/* om_instance returns an error code "OM_NO_SUCH_CLASS" if  */
		/* the object has been negotiated with a different package. */
		/* So set the retval to OM_SUCCESS and instance to OM_FALSE */
		if (retval == OM_NO_SUCH_CLASS)
		{
		    retval = OM_SUCCESS;
		    instance = OM_FALSE;
		}

		if (retval == OM_SUCCESS)
		{
		    if (instance == OM_TRUE)
		    {
			/* convert the RDN object to string */
			retval = xos__name_to_string(object, OM_TRUE,
						     local_strings, string);
		    }
		    else
		    {
			/* check if DS_C_ERROR has been passed */
			retval = om_instance(object, dsP_DS_C_ERROR, &instance);

			/* om_instance returns error code "OM_NO_SUCH_CLASS"  */
			/* if the object has been negotiated with a different */
		        /* package.  So set the retval to OM_SUCCESS and      */
			/* instance to OM_FALSE 			      */
			if (retval == OM_NO_SUCH_CLASS)
			{
			    retval = OM_SUCCESS;
			    instance = OM_FALSE;
			}

			if (retval == OM_SUCCESS)
			{
			    if (instance == OM_TRUE)
			    {
				/* convert the error object to string */
				retval = xos__error_to_string(object, 
							      OM_FALSE,
							      local_strings,
							      string);
			    }
			    else
			    {
				/* check if DS_C_ATTRIBUTE_ERROR is passed */
				retval = om_instance(object, 
						     dsP_DS_C_ATTRIBUTE_ERROR, 
						     &instance);

				/* om_instance returns error OM_NO_SUCH_CLASS */
				/* if the object has been negotiated with a   */
				/* different package.  So set the retval to   */
				/* OM_SUCCESS and instance to OM_FALSE 	      */
				if (retval == OM_NO_SUCH_CLASS)
				{
				    retval = OM_SUCCESS;
				    instance = OM_FALSE;
				}

				if (retval == OM_SUCCESS)
				{
				    if (instance == OM_TRUE)
				    {
					/* convert the error object to string */
					retval = xos__error_to_string(
								 object,
								 OM_TRUE,
								 local_strings,
								 string);
				    }
				    else
				    {
					/* convert the object to string */
					retval = xos__obj_to_str(object, 
								 local_strings,
								 string);
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
    else
    {
	/* check if DS_C_DS_DN has been passed */
	if (object[0].value.string.length == dsP_DS_C_DS_DN.length &&
	    memcmp(object[0].value.string.elements, dsP_DS_C_DS_DN.elements,
		   dsP_DS_C_DS_DN.length) == 0)
	{
	    /* convert the name to string */
	    retval = xos__name_to_string(object, OM_FALSE, local_strings, 
					 string);
	}
	else
	{
	    /* check if DS_C_DS_RDN has been passed */
	    if (object[0].value.string.length == dsP_DS_C_DS_RDN.length &&
		memcmp(object[0].value.string.elements, 
		       dsP_DS_C_DS_RDN.elements, dsP_DS_C_DS_RDN.length) == 0)
	    {
		/* convert the name to string */
		retval = xos__name_to_string(object, OM_TRUE, local_strings, 
					     string);
	    }
	    else
	    {
		/* convert the object to string */
		retval = xos__obj_to_str(object, local_strings, string);
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__invoke_xoi_initialize			   */
/* Description   : This function is used to invoke the 		   */
/*		   xoi_initialize function, which reads the schema */
/*		   details from the schema file.		   */
/* Return Value  : OMX_SUCCESS or an error return code indicating  */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xos__invoke_xoi_initialize(
    void)
{
#ifdef THREADSAFE
static pthread_once_t once_block = pthread_once_init;
#else
static OM_boolean    xos_initialized = OM_FALSE;
#endif

#ifdef THREADSAFE
    pthread_once(&once_block, xos__invoke_xoi_initialize_once);
#else
    if (xos_initialized == OM_TRUE)
	return(OMX_SUCCESS);

    xos__invoke_xoi_initialize_once();

    if (xos_init_retval == OMX_SUCCESS)
	xos_initialized = OM_TRUE;
#endif

    return(xos_init_retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__invoke_xoi_initialize_once		   */
/* Description   : This function is used to invoke the 		   */
/*		   xoi_initialize function once, which reads the   */
/*		   schema details from the schema file.		   */
/* Return Value  : OMX_SUCCESS or an error return code indicating  */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void xos__invoke_xoi_initialize_once(
    void)
{
OM_integer  retval_xoi;
OM_uint     line_no;
char	    file_name[XOS_MAX_FILE_NAME + 1];

    if ((retval_xoi = xoi_initialize(&line_no)) != XOI_SUCCESS)
    {
	xos_init_retval = xos_map_xoi_error(retval_xoi);

	/* get the schema file name */
	xoi_get_schema_file_name(file_name);

	if (xos_init_retval == OMX_CANNOT_READ_SCHEMA)
	{
	    /* write the message into the log file */
	    dsP_trace_msg(1,
		   "   ERROR: Could not read XOM object info schema file <%s>\n", file_name);
	}
	else
	{
	    /* write the message into the log file */
	    dsP_trace_msg(1,
	      "   ERROR: Invalid XOM object info schema File <%s> - Error <%d>  Line No.<%d>\n",
	      file_name, xos_init_retval, line_no);
	}

	xos_init_retval = OMX_SCHEMA_NOT_READ;
    }
    else
    {
	xos_init_retval = OMX_SUCCESS;
    }

    if (xos_init_retval == OMX_SUCCESS)
    {
	/* open the NLS message file */
	xos_nls_desc = catopen(XOS_NLS_FILE, 0);
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__string_to_name				   */
/* Description   : This function is used to convert a Distinguished*/
/*		   name in the string format to an XOM object.	   */
/*		   The memory space for the returned OM_object is  */
/*		   provided by this function.			   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__string_to_name(
    OM_workspace       workspace,      /* IN  - The workspace		      */
    OM_string         *string,         /* IN  - The string to be converted    */
    OM_boolean         is_rdn,         /* IN  - Indicates RDN is to be created*/
    OM_boolean         local_strings,  /* IN  - To specify local strings      */
    OM_private_object *name,           /* OUT - The converted DN	      */
    OM_integer	      *error_position, /* OUT - Error Position in input string*/
    OM_integer	      *error_type)     /* OUT - Type of error 		      */
{
OM_return_code  retval;
OM_object	object;
char	       *val_mem_top;

    *error_position = 0;

    /* Read the schema file details */
    if ((*error_type = xos__invoke_xoi_initialize()) != OMX_SUCCESS)
    {
	*name = NULL;
	return(OM_TEMPORARY_ERROR);
    }

    /* malloc a memory heap for storing attribute values */
    val_mem_top = (char *) malloc(sizeof(char) * string->length);

    if (val_mem_top == NULL)
    {
	*error_type = OMX_MEMORY_INSUFFICIENT;
	*name = NULL;
	return(OM_MEMORY_INSUFFICIENT);
    }
    
    /* Convert it to public object */
    retval = xos_convert_str_to_public_name(string, val_mem_top, is_rdn,
					    local_strings, &object, 
					    error_position, error_type);
    if (retval == OM_SUCCESS)
    {
	*error_position = 0;
	*error_type = OMX_SUCCESS;

	/* if an RDN object has been created */
	if (is_rdn == OM_TRUE)
	{
	    /* convert the created RDN object to XOM private object */
	    retval = xos_convert_to_private(object, dsP_DS_C_DS_RDN, 
					    workspace, name);
	    /* free the public object */
	    xos_free_rdn_object(object);
	    free(object);
	}
	else
	{
	    /* convert the created DN object to XOM private object */
	    retval = xos_convert_to_private(object, dsP_DS_C_DS_DN, 
					    workspace, name);
	    /* free the public object */
	    xos_free_dn_object(object);
	}
    }

    /* free the memory heap */
    free(val_mem_top);

    if (retval != OM_SUCCESS)
    {
	*name = NULL;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__name_to_string				   */
/* Description   : This function is used to convert a Distinguished*/
/*		   name object (DS_C_DS_DN) or a Relative Disting. */
/*		   name object (DS_C_DS_RDN) to a string format.   */
/*		   The object can either be a private object,      */
/*		   a service generated public object or a client   */
/*		   generated object.      			   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__name_to_string(
    OM_object    name,	        /* IN  - The Dist.Name to be converted       */
    OM_boolean   is_rdn,        /* IN  - Indicates an RDN object             */
    OM_boolean	 local_strings, /* IN  - To indicate local string conversion */
    OM_string	*string)        /* OUT - The converted DN string 	     */
{
OM_value_position total_num;
OM_return_code    retval;
OM_object  	  object;
OM_boolean	  is_private;
OM_boolean	  is_service;

    /* Read the schema file details */
    if (xos__invoke_xoi_initialize() != OMX_SUCCESS)
	return(OM_TEMPORARY_ERROR);

    is_private = OM_FALSE;
    is_service = OM_FALSE;
    object = NULL;
    retval = OM_SUCCESS;

    /* if a service generated object */
    if (!XOS_CPUB(name))
    {
	/* if private object */
	if (XOS_PRIVATE(name))
	{
	    retval = om_get(name, OM_NO_EXCLUSIONS, OM_NO_MORE_TYPES,
			    local_strings, 0, 0, &object, &total_num);

	    if (retval == OM_SUCCESS)
	    {
		/* check if om_get has returned some object */
		if (total_num <= 0)
		{
		    retval = OM_NO_SUCH_OBJECT;
		}
	    }
	}
	else
	{
	    object = name;
	}
    }
    else
    {
	/* Client generated object */
	object = name;
    }

    if (retval == OM_SUCCESS)
    {
	/* if a DS_C_DS_DN object has been passed */
	if (is_rdn == OM_FALSE)
	{
	    /* convert the public object to string format */
	    retval = xos_convert_public_name_to_str(object, local_strings,
						    string);
	}
	else
	{
	    /* DS_C_DS_RDN object has been passed. Convert it to string */
	    retval = xos_convert_public_rdn_to_str(object, local_strings,
						   string);
	}
    }

    if (is_private == OM_TRUE)
    {
	if (object != NULL)
	{
	    /* free the object obtained from a call to om_get */
	    om_delete(object);
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__str_to_obj			   	   */
/* Description   : This function is used to convert the            */
/*		   attribute/type value specified in string 	   */
/*		   format to the XOM object (DS_C_ATTRIBUTE) or the*/
/*		   object corresponding to the OM Class specified  */
/*		   in the attribute value string.	           */
/*		   The input string could be a normal GDS attribute*/
/*		   like Common-Name, etc. or a structured attribute*/
/*		   like OR Address, ACL, etc.			   */
/*		   The memory space for the returned OM_object is  */
/*		   provided by this function.			   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__str_to_obj(
    OM_workspace          workspace,     /* IN  - The workspace		      */
    OM_string            *string,        /* IN  - The string to be converted  */
    OM_object_identifier  class,         /* IN  - The OM Class to be created  */
    OM_boolean            local_strings, /* IN  - To specify local strings    */
    OM_private_object    *object,        /* OUT - The converted Object	      */
    OM_integer	         *error_position,/* OUT - Error Position in I/P string*/
    OM_integer	         *error_type)    /* OUT - Type of error 	      */
{
OM_return_code  retval;
OM_object	pub_object;
OM_boolean	is_attribute;
char	       *val_mem_top;

    *error_position = 0;

    /* Read the schema file details */
    if ((*error_type = xos__invoke_xoi_initialize()) != OMX_SUCCESS)
    {
	*object = NULL;
	return(OM_TEMPORARY_ERROR);
    }

    /* malloc a memory heap for storing attribute values */
    val_mem_top = (char *) malloc(sizeof(char) * string->length);

    if (val_mem_top == NULL)
    {
	*error_type = OMX_MEMORY_INSUFFICIENT;
	*object = NULL;
	return(OM_MEMORY_INSUFFICIENT);
    }

    /* check if it is DS_C_ATTRIBUTE or DS_C_AVA object */
    if ((class.length == dsP_DS_C_ATTRIBUTE.length &&
	 memcmp(class.elements, dsP_DS_C_ATTRIBUTE.elements,
	        dsP_DS_C_ATTRIBUTE.length) == 0) ||
	(class.length == dsP_DS_C_AVA.length &&
	 memcmp(class.elements, dsP_DS_C_AVA.elements, 
		dsP_DS_C_AVA.length) == 0))
    {
	/* Convert it to public attribute object */
	retval = xos_to_public_attr_object(string, val_mem_top, local_strings,
					   &pub_object, error_position, 
					   error_type);
	is_attribute = OM_TRUE;
    }
    else
    {
	/* Convert it to the class object */
	retval = xos_str_to_obj_create_class(&class, string, val_mem_top,
					     local_strings, &pub_object, 
					     error_position, error_type);
	is_attribute = OM_FALSE;
    }

    if (retval == OM_SUCCESS)
    {
	*error_position = 0;
	*error_type = OMX_SUCCESS;

	/* Now convert the created public object to XOM private object */
	retval = xos_convert_to_private(pub_object, class, workspace, object);

	if (is_attribute == OM_TRUE)
	{
	    /* free the created public DS_C_ATTRIBUTE object */
	    xos_free_attr_object(pub_object);
	}
	else
	{
	    /* free the created public class object */
	    xos_free_class_object(pub_object);
	}
    }

    /* free the memory heap */
    free(val_mem_top);

    if (retval != OM_SUCCESS)
    {
	*object = NULL;
	return(retval);
    }

    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__obj_to_str			           */
/* Description   : This function is used to convert an XOM Object  */
/*		   of type DS_C_ATTRIBUTE or any other object with */
/*		   attribute values to a string format. The object */
/*		   can either be a private object, a service       */
/*		   generated public object or a client generated   */
/*		   object.					   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__obj_to_str(
    OM_object    object,        /* IN  - The Object to be converted          */
    OM_boolean	 local_strings, /* IN  - To indicate local string conversion */
    OM_string	*string)        /* OUT - The converted DN string 	     */
{
OM_value_position total_num;
OM_return_code    retval;
OM_uint32	  len_used;
OM_object  	  pub_object;
OM_boolean	  private;

    /* Read the schema file details */
    if (xos__invoke_xoi_initialize() != OMX_SUCCESS)
	return(OM_TEMPORARY_ERROR);

    retval = OM_SUCCESS;
    private = OM_FALSE;

    /* if not a client generated object */
    if (!XOS_CPUB(object))
    {
	/* if private object */
	if (XOS_PRIVATE(object))
	{
	    retval = om_get(object, OM_NO_EXCLUSIONS, OM_NO_MORE_TYPES,
			    local_strings, 0, 0, &pub_object, &total_num);

	    if (retval == OM_SUCCESS)
	    {
		/* check if om_get has returned some object */
		if (total_num <= 0)
		{
		    retval = OM_NO_SUCH_OBJECT;
		}
		else
		{
		    private = OM_TRUE;
		}
	    }
	}
	else
	{
	    pub_object = object;
	}
    }
    else
    {
	/* Client generated object */
	pub_object = object;
    }

    if (retval == OM_SUCCESS)
    {
	if (pub_object[0].type == OM_NO_MORE_TYPES)
	{
	    retval = OM_NO_SUCH_OBJECT;
	}
	else
	{
	    /* check if it is DS_C_ATTRIBUTE or DS_C_AVA object */
	    if ((pub_object[0].value.string.length==dsP_DS_C_ATTRIBUTE.length &&
		 memcmp(pub_object[0].value.string.elements, 
		        dsP_DS_C_ATTRIBUTE.elements, 
		        dsP_DS_C_ATTRIBUTE.length) == 0) ||
		(pub_object[0].value.string.length == dsP_DS_C_AVA.length &&
		 memcmp(pub_object[0].value.string.elements,
			dsP_DS_C_AVA.elements,
			dsP_DS_C_AVA.length) == 0))
	    {
		/* if an empty class object is supplied, convert it to */
		/* a string of 0 length.			       */
		if (pub_object[1].type == OM_NO_MORE_TYPES)
		{
		    string->length = 0;
		}
		else
		{
		    /* Convert the public attribute object to string */
		    retval = xos_convert_public_attr_to_str(pub_object, 
							    local_strings,
							    string);
		}
	    }
	    else
	    {
		if (pub_object[0].type == OM_CLASS &&
		    ((pub_object[0].syntax & OM_S_SYNTAX) == 
		     OM_S_OBJECT_IDENTIFIER_STRING))
		{
		    /* convert class object to string */
		    retval = xos_convert_class_to_string(
						   pub_object, local_strings,
						   string->length, OM_FALSE,
						   (char *)(string->elements),
						   &len_used); 
		    if (retval == OM_SUCCESS)
		    {
			/* Add the NULL character, if space is available */
			if (len_used + 1 < string->length)
			{
			    ((char *)(string->elements))[len_used] = 0;
			}

			string->length = len_used;
		    }
		}
		else
		{
		    retval = OM_NO_SUCH_OBJECT;
		}
	    }
	}
 
	if (private == OM_TRUE)
	{
	    om_delete(pub_object);
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__string_to_filter			   */
/* Description   : This function is used to convert a filter in    */
/*		   the string format to the corresponding XOM      */
/*		   private object (DS_C_FILTER).		   */
/*		   The memory space for the returned OM_object is  */
/*		   provided by this function.			   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__string_to_filter(
    OM_workspace       workspace,      /* IN  - The workspace		      */
    OM_string         *string,         /* IN  - The string to be converted    */
    OM_boolean         local_strings,  /* IN  - To specify local strings      */
    OM_private_object *filter,         /* OUT - The converted filter	      */
    OM_integer	      *error_position, /* OUT - Error Position in input string*/
    OM_integer	      *error_type)     /* OUT - Type of error 		      */
{
OM_return_code  retval;
OM_object	pub_object;
char	       *val_mem_top;

    *error_position = 0;

    /* Read the schema file details */
    if ((*error_type = xos__invoke_xoi_initialize()) != OMX_SUCCESS)
    {
	*filter = NULL;
	return(OM_TEMPORARY_ERROR);
    }

    /* malloc a memory heap for storing attribute values */
    val_mem_top = (char *) malloc(sizeof(char) * string->length);

    if (val_mem_top == NULL)
    {
	*error_type = OMX_MEMORY_INSUFFICIENT;
	*filter = NULL;
	return(OM_MEMORY_INSUFFICIENT);
    }

    /* convert it to public object */
    retval = xos_to_public_filter_object(string, val_mem_top, local_strings,
					 &pub_object, error_position, 
					 error_type);

    if (retval == OM_SUCCESS)
    {
	*error_position = 0;
	*error_type = OMX_SUCCESS;

	/* Now convert the created public object to XOM private object */
	retval = xos_convert_to_private(pub_object, dsP_DS_C_FILTER, workspace,
					filter);

	/* free the public object */
	xos_free_filter_object(pub_object);
    }

    /* free the memory heap */
    free(val_mem_top);

    if (retval != OM_SUCCESS)
    {
	*filter = NULL;
	return(retval);
    }

    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos___error_to_string			   */
/* Description   : This function is used to examine an XDS error   */
/*		   and return the DS_PROBLEM component in string   */
/*		   format. 					   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__error_to_string(
    DS_status   error,         /* IN  - Object containing the error         */
    OM_boolean  is_attr_error, /* IN  - Indicates DS_C_ATTRIBUTE_ERROR      */
    OM_boolean	local_strings, /* IN  - To indicate local string conversion */
    OM_string  *string)        /* OUT - The converted error message         */
{
OM_value_position total_num;
OM_public_object  attr_error;
OM_public_object  error_object;
OM_return_code    retval;
OM_type		  error_list[] = {OM_NO_MORE_TYPES, OM_NO_MORE_TYPES};

    /* Read the schema file details */
    if (xos__invoke_xoi_initialize() != OMX_SUCCESS)
    {
	return(OM_TEMPORARY_ERROR);
    }

    attr_error = NULL;
    error_object = NULL;
    retval = OM_SUCCESS;

    /* if DS_C_ATTRIBUTE_ERROR */
    if (is_attr_error == OM_TRUE)
    {
	/* extract the DS_PROBLEMS object */
	error_list[0] = DS_PROBLEMS;
	retval = om_get(error, 
			OM_EXCLUDE_ALL_BUT_THESE_TYPES +
			OM_EXCLUDE_SUBOBJECTS,
			error_list, local_strings, 0, 0, &attr_error, 
			&total_num);

	if (retval == OM_SUCCESS)
	{
	    /* check if om_get has returned some object */
	    if (total_num <= 0)
	    {
		retval = OM_NO_SUCH_OBJECT;
	    }
	    else
	    {
		/* Now extract the DS_PROBLEM object */
		error_list[0] = DS_PROBLEM;
		retval = om_get(attr_error->value.object.object, 
				OM_EXCLUDE_ALL_BUT_THESE_TYPES +
				OM_EXCLUDE_SUBOBJECTS,
				error_list, local_strings, 0, 0, 
				&error_object, &total_num);

		if (retval == OM_SUCCESS)
		{
		    /* check if om_get has returned some object */
		    if (total_num <= 0)
		    {
			retval = OM_NO_SUCH_OBJECT;
		    }
		}
	    }
	}
    }
    else
    {
	/* Extract the DS_PROBLEM object */
	error_list[0] = DS_PROBLEM;
	retval = om_get(error,
			OM_EXCLUDE_ALL_BUT_THESE_TYPES +
			OM_EXCLUDE_SUBOBJECTS,
			error_list, local_strings, 0, 0, &error_object, 
			&total_num);

	if (retval == OM_SUCCESS)
	{
	    /* check if om_get has returned some object */
	    if (total_num <= 0)
		retval = OM_NO_SUCH_OBJECT;
	}
    }

    if (retval == OM_SUCCESS)
    {
	/* convert the enumerated error type to string */
	if (attr_error == NULL)
	{
	    retval= xos_map_xds_error_to_string(xos_nls_desc,
					        error_object->value.enumeration,
						NULL, string);
	}
	else
	{
	    retval= xos_map_xds_error_to_string(xos_nls_desc,
					        error_object->value.enumeration,
						attr_error->value.object.object,
						string);
	}
    }

    /* now delete the extracted object */
    if (error_object != NULL)
    {
	om_delete(error_object);
    }

    if (attr_error != NULL)
    {
	om_delete(attr_error);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
