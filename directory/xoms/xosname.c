/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosname.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:26  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:37  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:52:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:27:05  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:23:08  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:57:01  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:37  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:17:34  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:38  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:33:01  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  11:00:36  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosname.c					   */
/* Description : This file contains all the routines for the       */
/*	         string to name and name to string conversions 	   */
/* Date	       : July 1, 1993					   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xosname.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:26 $";
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
#include <xdscds.h>

#include <xoi.h>
#include <xomext.h>

#include <xosstruct.h>
#include <xosutil.h>
#include <xosname.h>

#ifdef XOS_DEBUG
#include <xosdebug.h>
#endif

/* Import the necessary Object Identifier constants */
OM_IMPORT(dsP_DS_C_DS_DN)
OM_IMPORT(dsP_DS_C_DS_RDN)
OM_IMPORT(dsP_DS_C_AVA)

/*-- Local Define(s) ----------------------------------------------*/

/* define for the abbreviation for typeless attribute used in CDS */
#define	XOS_TYPELESS_ABBRV  "TL"

/* define for the DCE Global root context */
#define XOS_DCE_GLOBAL_ROOT "/..."

/*-- Static Function(s) -------------------------------------------*/
static OM_return_code xos__create_root_dn(
    OM_object  *name,        /* OUT - The created object    	      */
    OM_integer *error_type); /* OUT - Type of error in case of errors */

static OM_return_code xos__create_full_dn(
    char       *dn_str,        /* IN  - The DN string 	    	              */
    OM_integer  dn_len,        /* IN  - The length of the DN string           */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *name,          /* OUT - The converted DN                      */
    OM_integer *error_pos,     /* OUT - Error Position in case of errors      */
    OM_integer *error_type);   /* OUT - Type of error in case of errors       */

static OM_return_code xos__create_ds_c_ds_rdn(
    char       *rdn_str,       /* IN  - The RDN string 	    	              */
    OM_integer  rdn_len,       /* IN  - The length of the RDN string          */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *rel_name,      /* OUT - The converted RDN                     */
    OM_integer *error_pos,     /* OUT - Error Position in case of errors      */
    OM_integer *error_type);   /* OUT - Type of error in case of errors       */

static OM_return_code xos__create_rdn(
    char          *dn_str,        /* IN  - The DN string 	    	      */
    OM_integer     dn_len,        /* IN  - The length of the DN	string        */
    char	  *val_mem_top,   /* IN  - Memory heap to store attr.values   */
    OM_descriptor *rdn_object,    /* IN  - Pointer to RDN object to be created*/
    OM_descriptor *ava_object,    /* IN  - Pointer to AVA object to be created*/
    OM_boolean     local_strings, /* IN  - To specify local strings           */
    OM_uint	  *rdn_desc_used, /* OUT - Number of descriptors used for RDN */
    OM_uint	  *ava_desc_used, /* OUT - Number of descriptors used for AVA */
    OM_integer	  *rdn_len,	  /* OUT - Length of the RDN in input string  */
    OM_integer    *error_pos,     /* OUT - Error position in case of errors   */
    OM_integer    *error_type);   /* OUT - Type of error in case of errors    */

static OM_return_code xos__create_ava(
    char 	   *dn_str,         /* IN  - The DN string		      */
    OM_integer      dn_len,         /* IN  - The length of the DN string      */
    char	   *val_mem_top,    /* IN  - Memory heap to store attr. values*/
    OM_descriptor  *ava_object,     /* IN  - Pointer to AVA obj. to be created*/
    OM_boolean      only_attr_value,/* IN  - Indicates only attr.value present*/
    OM_boolean      local_strings,  /* IN  - To specify local strings         */
    OM_integer	   *ava_len,        /* OUT - Length of AVA in input string    */
    OM_integer	   *error_pos,      /* OUT - Error position in case of errors */
    OM_integer	   *error_type);    /* OUT - Type of error in case of errors  */

static OM_return_code xos__get_ava_len(
    char        *str,            /* IN  - The input string 	     	      */
    OM_integer   len,            /* IN  - The length of the input string      */
    OM_boolean   only_attr_value,/* IN  - Indicates only attr.value is present*/
    OM_integer 	*ava_len,        /* OUT - Length of the AVA		      */
    OM_integer  *error_pos,      /* OUT - Error position in case of errors    */
    OM_integer  *error_type);    /* OUT - Type of error in case of errors     */

static OM_return_code xos__get_total_obj_count(
    char       *dn_str,      /* IN  - The DN string			 */
    OM_integer  dn_len,      /* IN  - The length of the DN string	 */
    OM_uint    *total_rdn,   /* OUT - Total number of RDNs 	         */
    OM_uint    *total_ava,   /* OUT - Total number of AVAs 	         */
    OM_integer *error_pos,   /* OUT - Error position in case of errors   */
    OM_integer *error_type); /* OUT - Type of error in case of errors    */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_convert_str_to_public_name		   */
/* Description   : This function is used to convert a Distinguished*/
/*                 name in the string format to a public object.   */
/*		   The input string is of the form		   */
/*		   /c=de/o=sni/ou=ap11/cn=naik, sn=bhavesh ....	   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_convert_str_to_public_name(
    OM_string  *dn_string,     /* IN  - The string to be converted            */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  is_rdn,        /* IN  - Indicates that an RDN is to be created*/
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *name,          /* OUT - The converted DN                      */
    OM_integer *error_pos,     /* OUT - Error Position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors       */
{
OM_return_code 	 retval;
OM_integer	 dn_len, curr_index;
OM_integer	 i, n;
char		*dn_str;

#ifdef XOS_DEBUG
    xos_debug_string("xos_convert_str_to_public_name : ENTRY", 0, 1);
    xos_debug_string("Input DN String:", 0, 0);
    xos_debug_string((char *)(dn_string->elements), dn_string->length, 0);
#endif

    dn_len = (OM_integer) (dn_string->length);
    dn_str = (char *) (dn_string->elements);
    retval = OM_SUCCESS;

    /* Skip past white spaces */
    for (curr_index = 0; dn_len > 0 && isspace(dn_str[curr_index]);++curr_index)
	--dn_len;

    if (dn_len == 0)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_RDN_DELIMITER;
	retval = OM_WRONG_VALUE_MAKEUP;
    }
    else
    {
	/* check if XOS_DCE_GLOBAL_ROOT string is specified and skip past it */
	i = strlen(XOS_DCE_GLOBAL_ROOT);
	if (dn_len >= i && 
	    strncmp(XOS_DCE_GLOBAL_ROOT, &dn_str[curr_index], i) == 0)
	{
	    /* skip past the XOS_DCE_GLOBAL_ROOT string */
	    curr_index += i;
	    dn_len -= i;

	    /* skip past white spaces */
	    while(dn_len > 0 && isspace(dn_str[curr_index]))
	    {
		++curr_index;
		--dn_len;
	    }

	    if (dn_len == 0)
	    {
		/* only root specified, so create root DN */
		retval = xos__create_root_dn(name, error_type);
	    }
	    else
	    {
		retval = xos__create_full_dn(&dn_str[curr_index], dn_len, 
					     val_mem_top, local_strings, name, 
					     error_pos, error_type);
		if (retval != OM_SUCCESS)
		{
		    *error_pos += curr_index;
		}
	    }
	}
	else
	{
	    /* if only RDN is to be created */
	    if (is_rdn == OM_TRUE)
	    {
		/* check for XOS_RDN_DELIMITER_OP character */
		if (dn_str[curr_index] == XOS_RDN_DELIMITER_OP)
		{
		    *error_pos = curr_index + 1;
		    *error_type = OMX_FORMAT_ERROR;
		    retval = OM_WRONG_VALUE_MAKEUP;
		}
		else
		{
		    /* create the RDN object */
		    retval = xos__create_ds_c_ds_rdn(&dn_str[curr_index], 
						     dn_len, val_mem_top, 
						     local_strings, name,
						     error_pos, error_type);
		    if (retval != OM_SUCCESS)
		    {
			*error_pos += curr_index;
		    }
		}
	    }
	    else
	    {
		i = curr_index;

		/* skip past XOS_RDN_DELIMITER_OP */
		++i;
		n = 1;

		/* skip past white spaces */
		while (isspace(dn_str[i]))
		{
		    ++i;
		    ++n;
		}

		/* only root specified, so create root DN */
		if (n == dn_len)
		{
		    retval = xos__create_root_dn(name, error_type);
		}
		else
		{
		    /* create the DS_C_DS_DN object */
		    retval = xos__create_full_dn(&dn_str[curr_index], dn_len,
						 val_mem_top, local_strings, 
					         name, error_pos, error_type);
		    if (retval != OM_SUCCESS)
		    {
			*error_pos += curr_index;
		    }
		}
	    }
	}
    }

#ifdef XOS_DEBUG
    xos_debug_string("xos_convert_str_to_public_name : EXIT", 0, -1);
#endif

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_root_dn				   */
/* Description   : This function is used to create the DN Object   */
/*		   for root "/".				   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_root_dn(
    OM_object  *name,        /* OUT - The created object    	      */
    OM_integer *error_type)  /* OUT - Type of error in case of errors */
{
OM_return_code  retval;
OM_object  	dn_object;

    retval = OM_SUCCESS;

    dn_object = (OM_descriptor *) malloc(sizeof(OM_descriptor) * 2);

    if (dn_object == NULL)
    {
	*error_type = OMX_MEMORY_INSUFFICIENT;
	retval = OM_MEMORY_INSUFFICIENT;
    }
    else
    {
	/* Initialize the first OM_descriptor to DS_C_DS_DN object */
	omX_fill_oid(OM_CLASS, dsP_DS_C_DS_DN, &dn_object[0]);

	/* Initialize the second OM_descriptor to NULL descriptor */
	XOS_FILL_OMD_NULL(&dn_object[1]);

	*name = dn_object;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_full_dn				   */
/* Description   : This function is used to create the DN Object   */
/*		   The input string is of the form		   */
/*		   /c=de/o=sni/ou=ap11/cn=naik, sn=bhavesh ....    */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_full_dn(
    char       *dn_str,        /* IN  - The DN string 	    	              */
    OM_integer  dn_len,        /* IN  - The length of the DN string           */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *name,          /* OUT - The converted DN                      */
    OM_integer *error_pos,     /* OUT - Error Position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors       */
{
OM_return_code   retval;
OM_descriptor   *dn_desc_mem_top;
OM_descriptor   *rdn_desc_mem_top;
OM_descriptor   *ava_desc_mem_top;
OM_integer	 rdn_len;
OM_integer	 curr_index, i;
OM_object 	 dn_object;
OM_uint		 rdn_desc_used, ava_desc_used;
OM_uint		 total_desc, total_rdn, total_ava;

#ifdef XOS_DEBUG
char		 debug_msg[256];
#endif

#ifdef XOS_DEBUG
    xos_debug_string("xos__create_full_dn : ENTRY", 0, 1);
    xos_debug_string("Input DN String:", 0, 0);
    xos_debug_string(dn_str, dn_len, 0);
#endif

    curr_index = 0;
    dn_object = NULL;
    retval = OM_SUCCESS;

    retval = xos__get_total_obj_count(dn_str, dn_len, &total_rdn, &total_ava, 
				      error_pos, error_type);
#ifdef XOS_DEBUG
    sprintf(debug_msg, "Total RDN = <%d>, Total AVA = <%d>", 
	    total_rdn, total_ava);
    xos_debug_string(debug_msg, 0, 0);
#endif

    if (retval == OM_SUCCESS)
    {
	/* Compute the total number of descriptors required.	       */
	/* For the DS_C_DS_DN descriptor, "total_rdn + 2" are required */
	/* For DS_C_DS_RDN, "(total_rdn * 2) + total_ava" are required */
	/* For DS_C_AVA, "total_ava * 4" are required.		       */
	total_desc = (total_rdn + 2) + ((total_rdn * 2) + total_ava) + 
		     (total_ava * 4);

	dn_desc_mem_top  = (OM_descriptor *) 
			   malloc(sizeof(OM_descriptor) * total_desc);

	if (dn_desc_mem_top == NULL)
	{
	    *error_type = OMX_MEMORY_INSUFFICIENT;
	    retval = OM_MEMORY_INSUFFICIENT;
	}
	else
	{
	    /* Initialize the start block of rdn and ava descriptors */
	    rdn_desc_mem_top = dn_desc_mem_top + total_rdn + 2;
	    ava_desc_mem_top = rdn_desc_mem_top + (total_rdn * 2) + total_ava;

	    dn_object = dn_desc_mem_top;

	    /* Initialize the first OM_descriptor to DS_C_DS_DN object */
	    omX_fill_oid(OM_CLASS, dsP_DS_C_DS_DN, &dn_object[0]);

	    /* Initialize all the others to NULL descriptors */
	    for (i = 1; i < total_desc; ++i)
		XOS_FILL_OMD_NULL(&dn_object[i]);

	    for (i = 1; i < total_rdn + 1; ++i)
	    {
		/* Skip past the XOS_RDN_DELIMITER_OP character */
		++curr_index;
		--dn_len;

		retval = xos__create_rdn(&dn_str[curr_index], dn_len, 
					 val_mem_top, rdn_desc_mem_top, 
					 ava_desc_mem_top, local_strings,
					 &rdn_desc_used, &ava_desc_used, 
					 &rdn_len, error_pos, error_type);

		if (retval != OM_SUCCESS)
		{
		    *error_pos += curr_index;
		    break;
		}

		/* set the DN object descriptor to the created RDN object */
		omX_fill(DS_RDNS, OM_S_OBJECT, 0, rdn_desc_mem_top, 
			 &dn_object[i]);

		rdn_desc_mem_top += rdn_desc_used;
		ava_desc_mem_top += ava_desc_used;

		/* skip past the current RDN */
		dn_len -= rdn_len;
		curr_index += rdn_len;

		/* move to the next available memory */
		val_mem_top += rdn_len;
	    }
	}
    }

    if (retval != OM_SUCCESS)
    {
	if (dn_object != NULL)
	{
	    xos_free_dn_object(dn_object);
	}
    }
    else
    {
	*name = dn_object;
    }

#ifdef XOS_DEBUG
    xos_debug_string("xos__create_full_dn : EXIT", 0, -1);
#endif

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_ds_c_ds_rdn			   */
/* Description   : This function is used to create the DS_C_DS_RDN */
/*		   object. The input string is of the form	   */
/*		   "cn=naik, sn=bhavesh"			   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_ds_c_ds_rdn(
    char       *rdn_str,       /* IN  - The RDN string 	    	              */
    OM_integer  rdn_len,       /* IN  - The length of the RDN string          */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *rel_name,      /* OUT - The converted RDN                     */
    OM_integer *error_pos,     /* OUT - Error Position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors       */
{
OM_return_code  retval;
OM_descriptor  *ava_desc_mem_top;
OM_object	rdn_object;
OM_integer	dummy_len;
OM_integer	ava_len;
OM_integer  	curr_index, i;
OM_integer  	len;
OM_uint		dummy_rdn_used, dummy_ava_used;
OM_uint		total_desc;
OM_uint		ava_count;

    len = rdn_len;
    curr_index = 0;
    ava_count = 1;
    rdn_object = NULL;

    while (len > 0)
    {
	retval = xos__get_ava_len(&rdn_str[curr_index], len, OM_FALSE,
				  &ava_len, error_pos, error_type);
	if (retval != OM_SUCCESS)
	{
	    *error_pos += curr_index;
	    break;
	}

	/* skip past the current AVA */
	curr_index += ava_len;
	len -= ava_len;

	if (len > 0)
	{
	    if (rdn_str[curr_index] == XOS_AVA_SEPARATOR_OP)
	    {
		/* skip past the XOS_AVA_SEPARATOR_OP character */
		++curr_index;
		--len;
		++ava_count;
	    }
	    else
	    {
		/* must be format error because for the DS_C_DS_RDN object  */
		/* only single RDN is allowed with or without multiple AVAs */
		*error_pos = curr_index + 1;
		*error_type = OMX_FORMAT_ERROR;
		retval = OM_WRONG_VALUE_MAKEUP;
		break;
	    }
	}
    }

    if (retval == OM_SUCCESS)
    {
	/* Compute the total number of descriptors required.	    */
	/* For the DS_C_DS_RDN object, "ava_count + 2" are required */
	/* For the DS_C_AVA object, "ava_count * 4" are required    */
	total_desc = (ava_count + 2) + (ava_count * 4);

	rdn_object = (OM_descriptor *)
		      malloc(sizeof(OM_descriptor) * total_desc);
	if (rdn_object == NULL)
	{
	    *error_type = OMX_MEMORY_INSUFFICIENT;
	    retval = OM_MEMORY_INSUFFICIENT;
	}
	else
	{
	    /* initialize the start block of ava descriptors */
	    ava_desc_mem_top = rdn_object + ava_count + 2;

	    /* initialize all descriptors to NULL */
	    for (i = 0; i < total_desc; ++i)
	    {
		XOS_FILL_OMD_NULL(&rdn_object[i]);
	    }

	    /* create the RDN object 					*/
	    /* The variable with prefixes dummy are used as dummies and */
	    /* are of no use here, but are used because the create_rdn  */
	    /* function requires it.				        */
	    retval = xos__create_rdn(rdn_str, rdn_len, val_mem_top,
				     rdn_object, ava_desc_mem_top,
				     local_strings, &dummy_rdn_used, 
				     &dummy_ava_used, &dummy_len, 
				     error_pos, error_type);
	    if (retval == OM_SUCCESS)
	    {
		*rel_name = rdn_object;
	    }
	}
    }

    if (retval != OM_SUCCESS)
    {
	if (rdn_object != NULL)
	{
	    free(rdn_object);
	}
    }
    
    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_rdn				   */
/* Description   : This function is used to create the RDN Object  */
/*		   The input string is of the form		   */
/*		   c=de/o=sni/ou=ap11/cn=naik, sn=bhavesh .... 	   */
/*		   The input string is scanned and the first  	   */
/*		   encountered RDN is created.			   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_rdn(
    char          *dn_str,        /* IN  - The DN string 	    	      */
    OM_integer     dn_len,        /* IN  - The length of the DN	string        */
    char	  *val_mem_top,   /* IN  - Memory heap to store attr.values   */
    OM_descriptor *rdn_object,    /* IN  - Pointer to RDN object to be created*/
    OM_descriptor *ava_object,    /* IN  - Pointer to AVA object to be created*/
    OM_boolean     local_strings, /* IN  - To specify local strings           */
    OM_uint	  *rdn_desc_used, /* OUT - Number of descriptors used for RDN */
    OM_uint	  *ava_desc_used, /* OUT - Number of descriptors used for AVA */
    OM_integer	  *rdn_len,	  /* OUT - Length of the RDN in input string  */
    OM_integer    *error_pos,     /* OUT - Error position in case of errors   */
    OM_integer    *error_type)    /* OUT - Type of error in case of errors    */
{
OM_return_code   retval;
OM_boolean	 only_attr_value;
OM_integer	 ava_len;
OM_integer	 curr_index, i;

#ifdef XOS_DEBUG
char		 debug_msg[256];
#endif

#ifdef XOS_DEBUG
    xos_debug_string("xos__create_rdn : ENTRY", 0, 1);
#endif

    /* Initialize the first OM_descriptor as a DS_C_DS_RDN object */
    omX_fill_oid(OM_CLASS, dsP_DS_C_DS_RDN, &rdn_object[0]);

    only_attr_value = OM_TRUE;

    /* check if XOS_AVA_EQUALITY_OP is present */
    for (curr_index = 0; curr_index < dn_len; ++curr_index)
    {
	/* if escape operator */
	if (dn_str[curr_index] == XOS_ESCAPE_OP)
	{
	    /* Skip past escape character */
	    ++curr_index;
	    if (tolower(dn_str[curr_index]) == XOS_HEX_OP)
	    {
		/* skip past hexadecimal characters */
		curr_index += 2;
	    }
	    continue;
	}
	if (dn_str[curr_index] == XOS_AVA_QUOTE_OP || 
	    dn_str[curr_index] == XOS_AVA_SEPARATOR_OP ||
	    dn_str[curr_index] == XOS_RDN_DELIMITER_OP)
	{
	    break;
	}
	if (dn_str[curr_index] == XOS_AVA_EQUALITY_OP)
	{
	    /* both attribute type and value specified */
	    only_attr_value = OM_FALSE;
	    break;
	}
    } 

    curr_index = 0;
    *ava_desc_used = 0;

    i = 0;

    *rdn_len = 0;

    while (dn_len > 0)
    {
	retval = xos__create_ava(&dn_str[curr_index], dn_len, val_mem_top, 
				 ava_object, only_attr_value, local_strings,
				 &ava_len, error_pos, error_type);

	if (retval != OM_SUCCESS)
	{
	    xos_free_rdn_object(rdn_object);
	   *error_pos += curr_index;
	    return(retval);
	}

	++i;
	/* set the RDN object descriptor to the created AVA object */
	omX_fill(DS_AVAS, OM_S_OBJECT, 0, ava_object, &rdn_object[i]);

 	/* 4 descriptors are used up to create the AVA object */
	*ava_desc_used += 4;
	ava_object += 4;

	++*rdn_desc_used;

	/* skip past current AVA */
	curr_index += ava_len;
	dn_len -= ava_len;

	/* move to the next available memory */
	val_mem_top += ava_len;

	if (dn_len > 0)
	{
	    if (dn_str[curr_index] == XOS_RDN_DELIMITER_OP)
		break;

	    /* skip past XOS_AVA_SEPARATOR_OP character */
	    ++curr_index;
	    --dn_len;
	}
    }

    ++i;
    /* Fill the last descriptor with NULL */
    XOS_FILL_OMD_NULL(&rdn_object[i]);

    *rdn_desc_used = i + 1;
    *rdn_len = curr_index;

#ifdef XOS_DEBUG
    sprintf(debug_msg, "RDN length read = <%d>", *rdn_len);
    xos_debug_string(debug_msg, 0, 0);
    xos_debug_string("xos__create_rdn : EXIT", 0, -1);
#endif

    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_ava				   */
/* Description   : This function is used to create an AVA Object   */
/*		   The input string is of the form		   */
/*		   c=de/o=sni/ou=ap11/cn=naik, sn=bhavesh .... 	   */
/*		   The input string is scanned and the first  	   */
/*		   encountered AVA is created. The end of the AVA  */
/*		   would be the character just before the	   */
/*		   XOS_RDN_DELIMITER_OP or XOS_AVA_SEPARATOR_OP    */
/*		   character.			   		   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_ava(
    char 	   *dn_str,         /* IN  - The DN string		      */
    OM_integer	    dn_len,         /* IN  - The length of the DN string      */
    char	   *val_mem_top,    /* IN  - Memory heap to store attr. values*/
    OM_descriptor  *ava_object,     /* IN  - Pointer to AVA obj. to be created*/
    OM_boolean      only_attr_value,/* IN  - Indicates only attr.value present*/
    OM_boolean      local_strings,  /* IN  - To specify local strings         */
    OM_integer	   *ava_len,        /* OUT - Length of AVA in input string    */
    OM_integer	   *error_pos,      /* OUT - Error position in case of errors */
    OM_integer	   *error_type)     /* OUT - Type of error in case of errors  */
{
OM_return_code  retval;
OM_integer	retval_xoi;
OM_integer      curr_index;
OM_integer	len, j;
OM_string       objid;
OM_syntax	omsyntax;
OM_uint32	val_len;
OM_uint32       str_len;
char	        attr[XOS_MAX_OBJID + 1];

#ifdef XOS_DEBUG
char		debug_msg[256];
#endif

#ifdef XOS_DEBUG
    xos_debug_string("xos__create_ava : ENTRY", 0, 1);
    xos_debug_string("Input String:", 0, 0);
    xos_debug_string(dn_str, (OM_uint32) dn_len, 0);
#endif

    /* get the length of the first AVA in the input string */
    retval = xos__get_ava_len(dn_str, dn_len, only_attr_value, &len,
			      error_pos, error_type);

    if (retval != OM_SUCCESS)
	return(retval);

    curr_index = 0;

    /* Skip past white spaces */
    while (curr_index < len && isspace(dn_str[curr_index]))
	++curr_index;

    /* if only attribute values are specified and attribute type is not    */
    /* specified, then it is typeless. So set the abbreviation accordingly */
    if (only_attr_value == OM_TRUE)
    {
	strcpy(attr, XOS_TYPELESS_ABBRV);
    }
    else
    {
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
	    if (!(isalnum(dn_str[curr_index]) || dn_str[curr_index] == '.'))
		break;

	    attr[j] = dn_str[curr_index];
	}

	if (j == 0)
	{
	    *error_pos = curr_index + 1;
	    *error_type = OMX_MISSING_ABBRV;
	    return(OM_WRONG_VALUE_MAKEUP);
	}

	/* Append the NULL character */
	attr[j] = 0;

	/* Skip past white spaces */ 
	while (curr_index < len && isspace(dn_str[curr_index])) 
	    ++curr_index;

	if (dn_str[curr_index] != XOS_AVA_EQUALITY_OP)
	{
	    *error_pos = curr_index + 1;
	    *error_type = OMX_MISSING_EQUAL_OPERATOR;
	    return(OM_WRONG_VALUE_MAKEUP);
	}

	/* Skip past the XOS_AVA_EQUALITY_OP character */
	++curr_index;
    }

    str_len = len - curr_index; 
    
    /* Process the reserved characters in the attribute value and create */
    /* a normalized string.						 */
    retval = xos_convert_to_normalized_string(&dn_str[curr_index], str_len, 
					      XOS_AVA_QUOTE_OP, 
					      val_mem_top, &val_len, 
					      error_pos, error_type);

    if (retval != OM_SUCCESS)
    {
	*error_pos += curr_index;
	return(retval);
    }

    retval_xoi = xoi_get_attr_details(attr, &omsyntax, &(objid.elements),
			              &(objid.length), NULL, NULL);

    if (retval_xoi != XOI_SUCCESS)
    {
	/* error_pos is not initialized here, because it would have */
	/* been set properly.					    */
	*error_type = xos_map_xoi_error(retval_xoi);
	return(OM_WRONG_VALUE_MAKEUP);
    }

    /* Initialize the first OM_descriptor as a DS_C_AVA object */
    omX_fill_oid(OM_CLASS, dsP_DS_C_AVA, &ava_object[0]);

    omX_fill_oid(DS_ATTRIBUTE_TYPE, objid, &ava_object[1]);
    
    xos_fill_descriptor(DS_ATTRIBUTE_VALUES, omsyntax, val_len, val_mem_top, 
			local_strings, &ava_object[2]);

    /* Initialize the last one with NULL descriptor */
    XOS_FILL_OMD_NULL(&ava_object[3]);

    *ava_len = len;

#ifdef XOS_DEBUG
    sprintf(debug_msg, "AVA length read = <%d>", *ava_len);
    xos_debug_string(debug_msg, 0, 0);
    xos_debug_string("Normalized attribute value:", 0, 0);
    xos_debug_string(val_mem_top, val_len, 0);
    xos_debug_string("xos__create_ava : EXIT", 0, -1);
#endif

    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__get_total_obj_count			   */
/* Description   : This function is used to read the DN string and */
/*		   return the total number of RDNs and AVAs in the */
/*		   string.					   */
/*		   The input string is of the form		   */
/*		   /c=de/o=sni/ou=ap11/cn=naik, sn=bhavesh ....	   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__get_total_obj_count(
    char       *dn_str,      /* IN  - The DN string			 */
    OM_integer  dn_len,      /* IN  - The length of the DN string	 */
    OM_uint    *total_rdn,   /* OUT - Total number of RDNs 	         */
    OM_uint    *total_ava,   /* OUT - Total number of AVAs 	         */
    OM_integer *error_pos,   /* OUT - Error position in case of errors   */
    OM_integer *error_type)  /* OUT - Type of error in case of errors    */
{
OM_return_code  retval;
OM_integer      curr_index;
OM_integer      ava_len;
OM_integer      i;
OM_uint         rdn_count, ava_count;

    curr_index = 0;
    /* Skip past white spaces */
    while (curr_index < dn_len && isspace(dn_str[curr_index]))
	++curr_index;

    /* if XOS_RDN_DELIMITER_OP is missing */
    if (curr_index == dn_len || dn_str[curr_index] != XOS_RDN_DELIMITER_OP)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_RDN_DELIMITER;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    /* Skip past XOS_RDN_DELIMITER_OP character */
    ++curr_index;

    i = curr_index;

    /* skip past white spaces */
    while (curr_index < dn_len && isspace(dn_str[curr_index]))
	++curr_index;

    /* Only root ("/") specified */
    if (curr_index == dn_len)
    {
	*total_rdn = 0;
	*total_ava = 0;
	return(OM_SUCCESS);
    }

    curr_index = i;
    dn_len -= i;

    rdn_count = 1;
    ava_count = 1;
    retval = OM_SUCCESS;

    /* Compute the total number of RDNs */
    while (dn_len > 0)
    {
	retval = xos__get_ava_len(&dn_str[curr_index], dn_len, OM_FALSE, 
				  &ava_len, error_pos, error_type);
	if (retval != OM_SUCCESS)
	{
	    *error_pos += curr_index;
	    break;
	}

	/* skip past the current AVA */
	curr_index += ava_len;
	dn_len -= ava_len;

	if (dn_len > 0)
	{
	    if (dn_str[curr_index] == XOS_RDN_DELIMITER_OP)
	    {
		++rdn_count;
		++ava_count;
	    }

	    if (dn_str[curr_index] == XOS_AVA_SEPARATOR_OP)
		++ava_count;
	}

	/* Skip past XOS_RDN_DELIMITER_OP or XOS_AVA_SEPARATOR_OP */
	--dn_len;
	++curr_index;
    }

    if (retval == OM_SUCCESS)
    {
	*total_rdn = rdn_count;
	*total_ava = ava_count;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__get_ava_len				   */
/* Description   : This function scans the input string and returns*/
/*		   the length of the first encountered AVA.        */
/*		   The input string is of the form		   */
/*		   c=de/o=sni/ou=ap11/cn=naik, sn=bhavesh .... 	   */
/*		   The end of the AVA would be the character just  */
/*		   before the XOS_RDN_DELIMITER_OP or 		   */
/*		   XOS_AVA_SEPARATOR_OP character.		   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__get_ava_len(
    char        *str,            /* IN  - The input string 	     	      */
    OM_integer	 len,            /* IN  - The length of the input string      */
    OM_boolean   only_attr_value,/* IN  - Indicates only attr.value is present*/
    OM_integer 	*ava_len,        /* OUT - Length of the AVA		      */
    OM_integer  *error_pos,      /* OUT - Error position in case of errors    */
    OM_integer  *error_type)     /* OUT - Type of error in case of errors     */
{
OM_return_code  retval;
OM_boolean 	open_quote_found, close_quote_found;
OM_integer     	curr_index;

    curr_index = 0;

    /* Skip past white spaces */
    while (curr_index < len && isspace(str[curr_index]))
	++curr_index;

    if (curr_index == len || str[curr_index] == XOS_AVA_SEPARATOR_OP || 
	str[curr_index] == XOS_RDN_DELIMITER_OP)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_AVA;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    open_quote_found = close_quote_found = OM_FALSE;
    retval = OM_SUCCESS;
    for (; curr_index < len; ++curr_index)
    {
	/* If escape operator */
	if (str[curr_index] == XOS_ESCAPE_OP)
	{
	    /* If the escape character is the last data character */
	    if (curr_index == len - 1)
	    {
		retval = OM_WRONG_VALUE_MAKEUP;
		break;
	    }

	    /* Skip through escape character */
	    ++curr_index;

	    /* If hexadecimal values specified ("\xnn") */
	    if (tolower(str[curr_index]) == XOS_HEX_OP)
	    {
		if (curr_index + 2 > len - 1)
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
        if (str[curr_index] == XOS_AVA_QUOTE_OP)
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

	/* The XOS_AVA_SEPARATOR_OP and XOS_RDN_DELIMITER_OP characters */
	/* are not interpreted within quotes			        */
	if (open_quote_found == OM_FALSE || close_quote_found == OM_TRUE)
	{
	    if (str[curr_index] == XOS_AVA_SEPARATOR_OP ||
		str[curr_index] == XOS_RDN_DELIMITER_OP)
		break;
	}

	/* if there is some more non space data after closed quotes */
	if (close_quote_found == OM_TRUE && (!isspace(str[curr_index])))
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
	    *error_pos = curr_index + 1;
	    *error_type = OMX_MISMATCHED_QUOTES;
	    retval = OM_WRONG_VALUE_MAKEUP;
	}
	else
	{
	    /* If there is no more data after XOS_AVA_SEPARATOR_OP or */
	    /* XOS_RDN_DELIMITER_OP characters 			      */
	    if (((str[curr_index] == XOS_AVA_SEPARATOR_OP) || 
		 (str[curr_index] == XOS_RDN_DELIMITER_OP)) && 
		(curr_index + 1 == len))
	    {
		*error_pos = curr_index + 2;
		*error_type = OMX_MISSING_AVA;
		retval = OM_WRONG_VALUE_MAKEUP;
	    }
	    else
	    {
		/* if only attribute values are specified and attribute */
		/* type is not specified, multiple AVAs are not allowed */
		if (only_attr_value == OM_TRUE && 
		    str[curr_index] == XOS_AVA_SEPARATOR_OP)
		{
		    *error_pos = curr_index + 1;
		    *error_type = OMX_FORMAT_ERROR;
		    retval = OM_WRONG_VALUE_MAKEUP;
		}
	    }
	}
    }

    if (retval == OM_SUCCESS)
	*ava_len = curr_index;

    return(retval);
}

    
/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_free_dn_object				   */
/* Description   : This function is used to free the memory space  */
/*		   occupied by the DN object.              	   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xos_free_dn_object(
    OM_object dn_object)   /* IN - The DN Object to be freed */
{
OM_uint i;

    for (i = 0; dn_object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	/* free the RDNs */
	if (dn_object[i].type == DS_RDNS)
	    xos_free_rdn_object(dn_object[i].value.object.object);
    }

    free(dn_object);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_free_rdn_object				   */
/* Description   : This function is used to free the memory space  */
/*		   occupied by the RDN object.              	   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xos_free_rdn_object(
    OM_object rdn_object)   /* IN - The RDN Object to be freed */
{
OM_object ava;
OM_uint   i, j;

    for (i = 0; rdn_object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (rdn_object[i].type == DS_AVAS)
	{
	    ava = rdn_object[i].value.object.object;

	    /* free the object identifier */
	    for (j = 0; ava[j].type != OM_NO_MORE_TYPES; ++j)
	    {
		if (ava[j].type == DS_ATTRIBUTE_TYPE)
		    free(ava[j].value.string.elements);
	    }
	}
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_convert_public_name_to_str		   */
/* Description   : This function is used to convert a Distinguished*/
/*                 name public object (DS_C_DS_DN) to a string	   */
/*		   format.					   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_convert_public_name_to_str(
    OM_object   dn_object,     /* IN  - The Dist.Name to be converted       */
    OM_boolean  local_strings, /* IN  - To indicate local string conversion */
    OM_string  *string)        /* OUT - The converted DN string             */
{
OM_value_position  total_num;
OM_return_code     retval;
OM_boolean	   delete_rdn_object;
OM_object  	   rdn_object, p;
OM_string	   rdn_string;
OM_uint	   	   index;
OM_uint        	   i;
char              *str;

    str = (char *) (string->elements);
    index = 0;

    /* Only root '/' object */
    if (dn_object[1].type == OM_NO_MORE_TYPES)
    {
	str[0] = XOS_RDN_DELIMITER_OP;

	/* Add the EOS if space is available */
	if (string->length > 1)
	    str[1] = 0;

	/* Note : The length does not include the NULL character */
	string->length = 1;

	return(OM_SUCCESS);
    }

    retval = OM_SUCCESS;

    for (i = 1; dn_object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (dn_object[i].type != DS_RDNS)
	{
	    retval = OM_WRONG_VALUE_TYPE;
	    break;
	}

	/* Check memory */
	if (index + 1 > string->length)
	{
	    retval = OM_MEMORY_INSUFFICIENT;
	    break;
	}
 
	/* Append the XOS_RDN_DELIMITER_OP character */
	str[index] = XOS_RDN_DELIMITER_OP;
	++index;

	rdn_object = dn_object[i].value.object.object;

	if (rdn_object == NULL || rdn_object[0].type == OM_NO_MORE_TYPES)
	{
	    retval = OM_WRONG_VALUE_MAKEUP;
	    break;
	}

	delete_rdn_object = OM_FALSE;

	/* if the rdn object is a private object, convert it to a */
	/* public object through a call to om_get.		  */
	if (XOS_PRIVATE(rdn_object))
	{
	    retval = om_get(rdn_object, OM_NO_EXCLUSIONS, OM_NO_MORE_TYPES,
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
		    rdn_object = p;
		    delete_rdn_object = OM_TRUE;
		}
	    }
	    else
	    {
		break;
	    }
	}

	if (rdn_object[1].type == OM_NO_MORE_TYPES)
	{
	    retval = OM_WRONG_VALUE_MAKEUP;
	    break;
	}

	rdn_string.elements = (void *) (&str[index]);
	rdn_string.length = string->length - index;

	/* convert the RDN to string */
	retval = xos_convert_public_rdn_to_str(rdn_object, local_strings,
					       &rdn_string);
	if (retval == OM_SUCCESS)
	{
	    /* skip past the converted rdn string */
	    index += rdn_string.length;
	}
	else
        {
	    break;
        }

	if (delete_rdn_object == OM_TRUE)
	{
	    om_delete(rdn_object);
	    delete_rdn_object = OM_FALSE;
	}
    }

    if (retval == OM_SUCCESS)
    {
	/* Add the NULL character if space is available */
	if (index + 1 < string->length)
	    str[index] = 0;

	string->length = index;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_convert_public_rdn_to_str		   */
/* Description   : This function is used to convert a Relative     */
/*		   Distinguished name public object (DS_C_DS_RDN)  */
/*		   to a string format.				   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_convert_public_rdn_to_str(
    OM_object   rdn_object,    /* IN  - The Rel.Dist.Name to be converted   */
    OM_boolean  local_strings, /* IN  - To indicate local string conversion */
    OM_string  *string)        /* OUT - The converted RDN string            */
{
OM_value_position  total_num;
OM_return_code     retval;
OM_integer         retval_xoi;
OM_boolean	   delete_ava_object;
OM_object  	   ava_object, p;
char	          *str;
char	   	   abbrv[XOS_MAX_OBJID + 1];/* MAX_OBJID since could be string*/
char	   	   c[XOS_MAX_VAR + 1];
OM_uint		   index;
OM_uint	   	   ava_count;
OM_uint        	   len, j, k, m;

    index = 0;
    ava_count = 0;
    str = (char *) (string->elements);
    retval = OM_SUCCESS;

    for (j = 1; rdn_object[j].type != OM_NO_MORE_TYPES; ++j)
    {
	if (rdn_object[j].type != DS_AVAS)
	{
	    retval = OM_WRONG_VALUE_TYPE;
	    break;
	}

	ava_object = rdn_object[j].value.object.object;

	if (ava_object == NULL || ava_object[0].type == OM_NO_MORE_TYPES)
	{
	    retval = OM_WRONG_VALUE_MAKEUP;
	    break;
	}

	delete_ava_object = OM_FALSE;

	/* if the ava object is a private object, convert it to a */
	/* public object through a call to om_get.		  */
	if (XOS_PRIVATE(ava_object))
	{
	    retval = om_get(ava_object, OM_NO_EXCLUSIONS, OM_NO_MORE_TYPES,
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
		    ava_object = p;
		    delete_ava_object = OM_TRUE;
		}
	    }
	    else
	    {
		break;
	    }
	}

	/* get the attribute object identifier */
	for (m = 1; ava_object[m].type != OM_NO_MORE_TYPES; ++m)
	{
	    if (ava_object[m].type == DS_ATTRIBUTE_TYPE)
		break;
	}

	if (ava_object[m].type != DS_ATTRIBUTE_TYPE)
	{
	    retval = OM_WRONG_VALUE_TYPE;
	    break;
	}
	
	/* check if attribute type is typeless */
	if (ava_object[m].value.string.length == 
	    (sizeof(OMP_O_DSX_TYPELESS_RDN) - 1) &&
	    memcmp(ava_object[m].value.string.elements, OMP_O_DSX_TYPELESS_RDN, 
		   ava_object[m].value.string.length) == 0)
	{
	    /* multiple AVAs not allowed for typeless attribute */
	    if (ava_count > 0)
	    {
		retval = OM_WRONG_VALUE_MAKEUP;
		break;
	    }
	}
	else
	{
	    /* Get the attribute abbreviation */
	    retval_xoi = xoi_get_attr_name(&(ava_object[m].value.string), 
					   abbrv, NULL);
	    if (retval_xoi != XOI_SUCCESS)
	    {
		/* Since unable to get the abbreviation, convert the */
		/* Object Identifier to its dotted representation    */
		retval_xoi = xoi_ber_to_string(&(ava_object[m].value.string),
					       abbrv);
		if (retval_xoi != XOI_SUCCESS)
		{
		    retval = OM_ENCODING_INVALID;
		    break;
		}
	    }

	    /* If multiple AVAs */
	    if (ava_count > 0)
	    {
		/* Check memory */
		if (index + 1 > string->length)
		{
		    retval = OM_MEMORY_INSUFFICIENT;
		    break;
		}

		str[index] = XOS_AVA_SEPARATOR_OP;
		++index;
	    }

	    len = strlen(abbrv);

	    /* Check memory */
	    if (index + len + 1 > string->length)
	    {
		retval = OM_MEMORY_INSUFFICIENT;
		break;
	    }

	    /* Append the Attribute Abbreviation and XOS_AVA_EQUALITY_OP */
	    strncpy(&str[index], abbrv, len);
	    index += len;
	    str[index] = XOS_AVA_EQUALITY_OP;
	    ++index;
	}

	/* get the attribute value */
	for (m = 1; ava_object[m].type != OM_NO_MORE_TYPES; ++m)
	{
	    if (ava_object[m].type == DS_ATTRIBUTE_VALUES)
		break;
	}
	if (ava_object[m].type != DS_ATTRIBUTE_VALUES)
	{
	    retval = OM_WRONG_VALUE_TYPE;
	    break;
	}

	switch(ava_object[m].syntax & OM_S_SYNTAX)
	{
	    /* if the syntax is any of these, then copy the value from */
	    /* length member of the OM_string.			       */
	    case (OM_S_INTEGER):
	    case (OM_S_BOOLEAN):
	    case (OM_S_ENUMERATION):
		sprintf(c, "%ld", ava_object[m].value.string.length);
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
	    default:
		/* Append the Attribute Value and interpret reserved chars*/
		for (k = 0; k < ava_object[m].value.string.length; ++k)
		{
		    switch(((char *) (ava_object[m].value.string.elements))[k])
		    {
			/* If reserved character, escape it */
			case (XOS_RDN_DELIMITER_OP):
			case (XOS_ESCAPE_OP):
			case (XOS_AVA_SEPARATOR_OP):
			case (XOS_AVA_EQUALITY_OP):
			case (XOS_AVA_QUOTE_OP):
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
				  (ava_object[m].value.string.elements))[k];
		    ++index;
		}
		break;
	}

	if (retval != OM_SUCCESS)
	    break;

	++ava_count;

	if (delete_ava_object == OM_TRUE)
	{
	    om_delete(ava_object);
	    delete_ava_object = OM_FALSE;
	}
    } 

    if (retval == OM_SUCCESS)
    {
	/* Add the NULL character if space is available */
	if (index + 1 < string->length)
	    str[index] = 0;

	string->length = index;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
