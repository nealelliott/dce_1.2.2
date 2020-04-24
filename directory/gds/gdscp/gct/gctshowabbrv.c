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
 * $Log: gctshowabbrv.c,v $
 * Revision 1.1.4.2  1996/02/18  18:19:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:56  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:34  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:53  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:52  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:18  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:55:42  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:04  keutel
 * 	creation
 * 	[1994/03/21  12:57:02  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctshowabbrv.c				   	   */
/* Description : This file contains all the modules for the	   */
/*		 showabbrv operation				   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctshowabbrv.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:19:08 $";
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

#include <xoi.h>

#include <gdscp.h>
#include <gctconv.h>
#include <gctstruct.h>
#include <gcterr.h>
#include <gctutil.h>

/*-- Static Function(s) -------------------------------------------*/
static signed16 gc_t__showabbrv_get_count(
    OM_uint	 *obj_count,   /* OUT - The count of Object Classes     */
    OM_uint	 *attr_count,  /* OUT - The count of attributes         */
    OM_uint	 *class_count, /* OUT - The count of OM Classes         */
    OM_uint	 *comp_count,  /* OUT - The count of Components         */
    gc_t_vector **out_vect);   /* OUT - The result or the error message */

static signed16 gc_t__showabbrv_load_vector(
    OM_uint	  obj_count,   /* OUT - The count of Object Classes     */
    OM_uint	  attr_count,  /* OUT - The count of attributes         */
    OM_uint	  class_count, /* OUT - The count of OM Classes         */
    gc_t_vector **result_vector, /* OUT - The result vector		  */
    gc_t_vector **error_vector); /* OUT - Contains error messages if any  */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_showabbrv				   */
/* Description   : This function is used to return the complete    */
/*		   list of valid object class and attribute        */
/*		   abbreviations along with their full names and   */
/*		   object identifiers.				   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_showabbrv(
    gc_t_vector **out_vect)   /* OUT - The result or the error message */
{
OM_integer    retval_xoi;
OM_uint	      line_no;
OM_uint       obj_count, attr_count, class_count, comp_count;
OM_uint       vect_count, i;
gc_t_vector  *result_vector;
signed16      retval;

    if (out_vect == NULL)
	return(GC_T_INVALID_PARAMETER);

    retval = GC_SUCCESS;

    /* Initialize the XOI module */
    if ((retval_xoi = xoi_initialize(&line_no)) != XOI_SUCCESS)
    {
	/* map the error to translator defined error and create the */
	/* error vector.					    */
	retval = gc_t_map_xoi_schema_file_error(retval_xoi, line_no, out_vect);
    }
    else
    {
	/* get the count of attributes and object classes */
	retval = gc_t__showabbrv_get_count(&obj_count, &attr_count,
					   &class_count, &comp_count, out_vect);
	if (retval == GC_SUCCESS)
	{
	    /* Allocate the memory for the result vector 		   */
	    /* 3 vectors are required for each object class - one each for */
	    /* the abbreviation, full name and the object identifier.      */
	    /* 3 vectors are required for each attribute - one each for    */
	    /* the abbreviation, full name and the object identifier.      */
	    /* 1 vector is required for each OM class name		   */
	    /* 2 vectors is required for each component - one each for     */
	    /* the abbreviation and the full name.			   */
	    /* Finally 1 vector is required to specify GC_T_EOL.	   */

	    vect_count = (obj_count * 3) + (attr_count * 3) + class_count +
			 (comp_count * 2) + 1;

	    result_vector = (gc_t_vector *) malloc(sizeof(gc_t_vector) * 
						   vect_count);
	    if (result_vector == NULL)
	    {
		retval = GC_T_MEMORY_INSUFFICIENT;
		/* create the error vector */
		gc_t_create_gct_error_vector(retval, NULL, out_vect);
	    }
	    else
	    {
		/* initialize the complete result vector with GC_T_EOL */
		for (i = 0; i < vect_count; ++i)
		    result_vector[i].type = GC_T_EOL;

		retval = gc_t__showabbrv_load_vector(obj_count, attr_count,
						     class_count, 
						     &result_vector, out_vect);
		if (retval != GC_SUCCESS)
		{
		    /* free the result vector */
		    gc_t_free_result(result_vector);
		}
		else
		{
		    *out_vect = result_vector;
		}
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__showabbrv_get_count		           */
/* Description   : This function is used to compute the number of  */
/*		   object classes, attributes, OM Classes and      */
/*		   components of the various OM classes defined.   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__showabbrv_get_count(
    OM_uint	 *obj_count,   /* OUT - The count of Object Classes     */
    OM_uint	 *attr_count,  /* OUT - The count of attributes         */
    OM_uint	 *class_count, /* OUT - The count of OM Classes         */
    OM_uint	 *comp_count,  /* OUT - The count of Components         */
    gc_t_vector **out_vect)    /* OUT - The result or the error message */
{
OM_integer  retval_xoi;
OM_uint     count;
OM_uint     i;

    /* get the count of number of object classes */
    if ((retval_xoi = xoi_get_objclass_count(obj_count)) == XOI_SUCCESS)
    {
	/* get the count of number of attributes */
	if ((retval_xoi = xoi_get_attribute_count(attr_count)) == XOI_SUCCESS)
	{
	    /* get the count of number of OM classes */
	    if ((retval_xoi = xoi_get_class_count(class_count)) == XOI_SUCCESS)
	    {
		*comp_count = 0;

		/* get the number of components in each OM Class */
		for (i = 0; i < *class_count; ++i)
		{
		    retval_xoi = xoi_get_class_info_from_index(i, NULL, &count);

		    if (retval_xoi != XOI_SUCCESS)
		    {
			break;
		    }

		    *comp_count += count;
		}
	    }
	}
    }

    if (retval_xoi != XOI_SUCCESS)
    {
	/* map the error to translator defined error and create the */
	/* error vector.					    */
	return(gc_t_map_xoi_error(retval_xoi, NULL, out_vect));
    }
    else
    {
	return(GC_SUCCESS);
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__showabbrv_load_vector		           */
/* Description   : This function is used to update the result      */
/*		   vector with the relevant data about the         */
/*		   object classes, attributes, OM Classes and      */
/*		   components of the various OM classes defined.   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__showabbrv_load_vector(
    OM_uint	  obj_count,     /* IN  - The count of Object Classes     */
    OM_uint	  attr_count,    /* IN  - The count of attributes         */
    OM_uint	  class_count,   /* IN  - The count of OM Classes         */
    gc_t_vector **result_vector, /* OUT - The result vector		  */
    gc_t_vector **error_vector)  /* OUT - Contains error messages if any  */
{
OM_integer retval_xoi;
OM_string  abbrv_string, full_name_string, obj_id_string;
OM_uint	   comp_count, i, j;
unsigned32 vect_index;
signed16   retval;
char       abbrv[GC_T_MAX_ABBRV + 1];
char       full_name[GC_T_MAX_ATTR_NAME + 1];
char       obj_id[GC_T_MAX_OBJID + 1];

    vect_index = 0;
    retval = GC_SUCCESS;
    *error_vector = NULL;

    /* first load the object class details */
    for (i = 0; i < obj_count; ++i)
    {
	retval_xoi = xoi_get_obj_info_from_index(i, obj_id, abbrv, full_name);

	if (retval_xoi != XOI_SUCCESS)
	{
	    /* map the error to translator defined error and create the */
	    /* error vector.					        */
	    retval = gc_t_map_xoi_error(retval_xoi, NULL, error_vector);
	    break;
	}
	else
	{
	    abbrv_string.elements = (void *) abbrv;
	    abbrv_string.length = strlen(abbrv);

	    /* fill the object class abbreviation string into result vector */
	    retval = gc_t_create_result_vector_data(*result_vector, vect_index,
						    GC_T_OCLABBRV, 
						    abbrv_string, 
						    OM_S_PRINTABLE_STRING);
	    if (retval == GC_SUCCESS)
	    {
		++vect_index;
	    }
	    else
	    {
		break;
	    }

	    full_name_string.elements = (void *) full_name;
	    full_name_string.length = strlen(full_name);

	    /* fill the object class name string into the result vector */
	    retval = gc_t_create_result_vector_data(*result_vector, vect_index,
						    GC_T_OCLNAME, 
						    full_name_string, 
						    OM_S_PRINTABLE_STRING);
	    if (retval == GC_SUCCESS)
	    {
		++vect_index;
	    }
	    else
	    {
		break;
	    }

	    obj_id_string.elements = (void *) obj_id;
	    obj_id_string.length = strlen(obj_id);

	    /* fill the object class identifier string into result vector */
	    retval = gc_t_create_result_vector_data(*result_vector, vect_index,
						    GC_T_OBJID, 
						    obj_id_string, 
						    OM_S_PRINTABLE_STRING);
	    if (retval == GC_SUCCESS)
	    {
		++vect_index;
	    }
	    else
	    {
		break;
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* now fill the attribute details into the result vector */
	for (i = 0; i < attr_count; ++i)
	{
	    retval_xoi = xoi_get_attr_info_from_index(i, obj_id, abbrv, 
						      full_name);
	    if (retval_xoi != XOI_SUCCESS)
	    {
		/* map the error to translator defined error and create the */
		/* error vector.					    */
		retval = gc_t_map_xoi_error(retval_xoi, NULL, error_vector);
		break;
	    }
	    else
	    {
		abbrv_string.elements = (void *) abbrv;
		abbrv_string.length = strlen(abbrv);

		/* fill attribute abbreviation string into result vector */
		retval = gc_t_create_result_vector_data(*result_vector, 
						        vect_index,
							GC_T_ATTRABBRV, 
							abbrv_string, 
							OM_S_PRINTABLE_STRING);
		if (retval == GC_SUCCESS)
		{
		    ++vect_index;
		}
		else
		{
		    break;
		}

		full_name_string.elements = (void *) full_name;
		full_name_string.length = strlen(full_name);

		/* fill the attribute name string into the result vector */
		retval = gc_t_create_result_vector_data(*result_vector, 
							vect_index,
							GC_T_ATTRNAME, 
							full_name_string, 
							OM_S_PRINTABLE_STRING);
		if (retval == GC_SUCCESS)
		{
		    ++vect_index;
		}
		else
		{
		    break;
		}

		obj_id_string.elements = (void *) obj_id;
		obj_id_string.length = strlen(obj_id);

	        /* fill the attribute identifier string into result vector */
		retval = gc_t_create_result_vector_data(*result_vector, 
							vect_index,
							GC_T_OBJID, 
							obj_id_string, 
							OM_S_PRINTABLE_STRING);
		if (retval == GC_SUCCESS)
		{
		    ++vect_index;
		}
		else
		{
		    break;
		}
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* now fill the class details into the result vector */
	for (i = 0; i < class_count; ++i)
	{
	    xoi_get_class_info_from_index(i, full_name, &comp_count);

	    full_name_string.elements = (void *) full_name;
	    full_name_string.length = strlen(full_name);

	    /* fill the attribute name string into the result vector */
	    retval = gc_t_create_result_vector_data(*result_vector, 
						    vect_index, GC_T_SATTRNAME, 
						    full_name_string, 
						    OM_S_PRINTABLE_STRING);
	    if (retval == GC_SUCCESS)
	    {
		++vect_index;

		/* now fill the component details into the result vector */
		for (j = 0; j < comp_count; ++j)
		{
		    retval_xoi = xoi_get_comp_info_from_index(i, j, abbrv, 
							      full_name);
		    if (retval_xoi != XOI_SUCCESS)
		    {
			/* map the error to translator defined error and */
			/* create the error vector.		         */
			retval = gc_t_map_xoi_error(retval_xoi, NULL, 
						    error_vector);
			break;
		    }
		    else
		    {
			abbrv_string.elements = (void *) abbrv;
			abbrv_string.length = strlen(abbrv);

			/* fill the component abbreviation into result vector */
			retval = gc_t_create_result_vector_data(*result_vector,
							        vect_index,
								GC_T_COMPABBRV, 
								abbrv_string, 
							 OM_S_PRINTABLE_STRING);
			if (retval == GC_SUCCESS)
			{
			    ++vect_index;
			}
			else
			{
			    break;
			}

			full_name_string.elements = (void *) full_name;
			full_name_string.length = strlen(full_name);

			/* fill the component name string into result vector */
			retval = gc_t_create_result_vector_data(*result_vector,
							        vect_index,
								GC_T_COMPNAME, 
							       full_name_string,
							 OM_S_PRINTABLE_STRING);
			if (retval == GC_SUCCESS)
			{
			    ++vect_index;
			}
			else
			{
			    break;
			}
		    }
		}

		if (retval != GC_SUCCESS)
		{
		    break;
		}
	    }
	    else
	    {
		break;
	    }
	}
    }

    if (retval != GC_SUCCESS)
    {
	if (*error_vector == NULL)
	{
	    /* create the error vector */
	    gc_t_create_gct_error_vector(retval, NULL, error_vector);
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
