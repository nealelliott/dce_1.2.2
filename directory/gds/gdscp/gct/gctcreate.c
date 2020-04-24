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
 * $Log: gctcreate.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:17  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:42  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:37  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:05  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:54:37  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:35:59  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:44  keutel
 * 	creation
 * 	[1994/03/21  12:56:14  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctcreate.c					   */
/* Description : This file contains all the modules for the	   */
/*		 directory create entry operation.		   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctcreate.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:51 $";
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
#include <gctconv.h>
#include <gctstruct.h>
#include <gctargs.h>
#include <gcterr.h>
#include <gctutil.h>
#include <gctbind.h>
#include <gctmoddsc.h>

/* Import the necessary Object Identifier constants */
OM_IMPORT(DS_C_ATTRIBUTE_LIST)
OM_IMPORT(DS_C_ATTRIBUTE)

/*-- Static Function(s) -------------------------------------------*/
static void gc_t__free_create_attribute_list(
    OM_object attribute_list); /* IN - The object to be freed */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_create					   */
/* Description   : This function is used to create an object entry */
/*		   in the DIT.					   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_create(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean       local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect)      /* OUT - NULL (success) or error message */
{
OM_private_object  session;
OM_private_object  context;
OM_private_object  name;
OM_object	   attribute, attribute_list;
OM_sint    	   invoke_id_return;
DS_status  	   ds_status;
signed16	   attr_info_count;
signed16  	   retval;
signed16  	   i, k;

    if (out_vect == NULL || vect_count < 0)
	return(GC_T_INVALID_PARAMETER);

    i = 0;
    name = NULL;
    attribute_list = NULL;
    *out_vect = NULL;
    retval = GC_SUCCESS;

    /* get the session object */
    if ((session = gc_t_get_bound_session()) == NULL)
    {
	retval = GC_T_BIND_NOT_MADE;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, out_vect);
    }
    else
    {
	/* get the context object */
	context = gc_t_get_context();

	/* validate the input arguments and get a count of the attribute */
	/* information arguments.					 */
	retval = gc_t_check_args(GC_T_CREATE, in_vect, vect_count, 
				 &attr_info_count, out_vect);
	if (retval == GC_SUCCESS)
	{
	    /* Allocate the memory for the descriptors.		       */
	    /* 2 more are required - one each for the class identifier */
	    /* and the NULL descriptor.				       */
	    attribute_list = (OM_descriptor *) malloc(sizeof(OM_descriptor) * 
						      (attr_info_count + 2));
	    if (attribute_list == NULL)
	    {
		retval = GC_T_MEMORY_INSUFFICIENT;
		/* create the error vector */
		gc_t_create_gct_error_vector(retval, NULL, out_vect);
	    }
	    else
	    {
		/* fill the first descriptor with DS_C_ATTRIBUTE_LIST */
		omX_fill_oid(OM_CLASS, DS_C_ATTRIBUTE_LIST, 
			     &attribute_list[0]);

		/* initialize the remaining to NULL descriptors */
		for (k = 1; k < attr_info_count + 2; ++k)
		{
		    GC_T_FILL_OMD_NULL(&attribute_list[k]);
		}

		/* extract the object name from the first argument and */
		/* convert it to DS_C_DS_DN object.		       */
		retval = gc_t_string_to_name(in_vect[0], local_strings, &name, 
					     out_vect);

		/* skip past the object name and the attribute info switch */
		i = 2;

		if (retval == GC_SUCCESS)
		{
		    for (k = 1; i < vect_count && in_vect[i] != NULL; ++i, ++k)
		    {
			/* convert the attribute information to object */
			retval = gc_t_string_to_object(in_vect[i], 
						       DS_C_ATTRIBUTE, 
						       local_strings,
						       &attribute, out_vect);
			if (retval != GC_SUCCESS)
			    break;

			omX_fill(DS_ATTRIBUTES, OM_S_OBJECT, 0, attribute, 
				 &attribute_list[k]);
		    }
		}
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* perform the add entry operation */
	ds_status = ds_add_entry(session, context, name, attribute_list,
				 &invoke_id_return);
	if (ds_status != DS_SUCCESS)
	{
            /* map the XDS error to translator defined error and */
            /* create the error vector.                          */
	    retval = gc_t_map_xds_error(ds_status, local_strings, out_vect);

	    /* free the error object */
	    om_delete(ds_status);
	}
    }

    /* now free the created objects */
    gc_t__free_create_attribute_list(attribute_list);

    if (name != NULL)
	om_delete(name);

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__free_create_attribute_list		   */
/* Description   : This function is used to free the attribute list*/
/*		   object.					   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_t__free_create_attribute_list(
    OM_object attribute_list)  /* IN - The object to be freed */
{
signed16 i;

    if (attribute_list == NULL)
	return;

    for (i = 0; attribute_list[i].type != OM_NO_MORE_TYPES; ++i)
    {
	/* free the DS_C_ATTRIBUTE object */
	if (attribute_list[i].type == DS_ATTRIBUTES)
	{
	    om_delete(attribute_list[i].value.object.object);
	}
    }

    free(attribute_list);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
