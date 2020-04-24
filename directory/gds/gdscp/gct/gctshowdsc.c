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
 * $Log: gctshowdsc.c,v $
 * Revision 1.1.4.2  1996/02/18  18:19:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:57  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:36  root]
 * 
 * Revision 1.1.2.4  1994/09/19  08:39:35  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:33:30  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:54  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:53  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:19  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:55:46  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:06  keutel
 * 	creation
 * 	[1994/03/21  12:57:06  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctshowdsc.c					   */
/* Description : This file contains all the modules for the        */
/*		 showdsc operation.				   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctshowdsc.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:19:09 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <gds.h>

#include <xom.h>
#include <xds.h>
#include <xdsgds.h>

#include <gdscp.h>
#include <gctstruct.h>
#include <gctconv.h>
#include <gcterr.h>
#include <gctutil.h>
#include <gctmoddsc.h>

/*-- Local Define(s) ----------------------------------------------*/
#define GC_T_MAX_SERVICE_CONTROLS  15

/*-- Static Function(s) -------------------------------------------*/
static signed16 gc_t__create_sc_vector_data(
    OM_public_object   public_context, /* IN  - The context object       */
    signed16	       sc_index,       /* IN  - Service Control index    */
    unsigned32	       vect_index,     /* IN  - Index into result vector */
    boolean            abbrv_result,   /* IN  - To specify abbrev.result */
    gc_t_vector	     **result_vector,  /* IN  - The result vector        */
    gc_t_vector	     **error_vector);  /* IN  - The error vector         */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_showdsc					   */
/* Description   : This function performs the showdsc operation.   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_showdsc(
    boolean       abbrv_result, /* IN  - To specify abbreviated result   */
    gc_t_vector **out_vect)     /* OUT - The result or the error message */
{
OM_private_object  context;
OM_value_position  total_number;
OM_public_object   public_context; 
OM_return_code     om_ret_code;
gc_t_vector	  *result_vector;
gc_t_vector	  *error_vector;
unsigned32  	   i;
signed16  	   retval;
signed16	   vect_count;
signed16	   sc_index;

    if (out_vect == NULL)
	return(GC_T_INVALID_PARAMETER);

    public_context = NULL;
    retval = GC_SUCCESS;

    context = gc_t_get_context();

    if (context == DS_DEFAULT_CONTEXT)
    {
	public_context = NULL;
    }
    else
    {
	/* extract the object */
	om_ret_code = om_get(context, OM_NO_EXCLUSIONS, OM_NO_MORE_TYPES,
			     OM_TRUE, 0, 0, &public_context, &total_number);

	if (om_ret_code == OM_SUCCESS)
	{
	    /* check if om_get has returned some object */
	    if (total_number <= 0)
	    {
		om_ret_code = OM_NO_SUCH_OBJECT;
	    }
	}

	if (om_ret_code != OM_SUCCESS)
	{
	    /* map the XOM error to the translator defined */
	    /* error and create the error string.          */
	    retval = gc_t_map_xom_error(om_ret_code, &error_vector);
	}
    }

    if  (retval == GC_SUCCESS)
    {
	/* Allocate the area for the result vector.			   */
	/* Each service control requires two vectors - one for the title   */
	/* and one for the value. Additionally one more vector is required */
	/* for GC_T_EOL.			   			   */
	vect_count = (GC_T_MAX_SERVICE_CONTROLS * 2) + 1;
	result_vector = (gc_t_vector *) malloc(sizeof(gc_t_vector) * 
					       vect_count);
	if (result_vector == NULL)
	{
	    retval = GC_T_MEMORY_INSUFFICIENT;
	    /* create the error vector */
	    gc_t_create_gct_error_vector(retval, NULL, &error_vector);
	}
	else
	{
	    /* initialize all the vectors to GC_T_EOL */
	    for (i = 0; i < vect_count; ++i)
	    {
		result_vector[i].type = GC_T_EOL;
	    }

	    i = 0;

	    /* now fill the result vector with the values */
	    for (sc_index = 0; sc_index < GC_T_MAX_SERVICE_CONTROLS; ++sc_index)
	    {
		retval = gc_t__create_sc_vector_data(public_context, sc_index, 
						     i, abbrv_result, 
						     &result_vector, 
						     &error_vector);
		if (retval != GC_SUCCESS)
		{
		    break;
		}

		i += 2;
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	*out_vect = result_vector;
    }
    else
    {
	/* free the result vector */
	if (result_vector != NULL)
	    gc_t_free_result(result_vector);

	*out_vect = error_vector;
    }

    /* delete the extracted object */
    if (public_context != NULL)
	om_delete(public_context);

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__create_sc_vector_data			   */
/* Description   : This function is used to fill the result vector */
/*		   with the created data.			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__create_sc_vector_data(
    OM_public_object   public_context, /* IN  - The context object       */
    signed16	       sc_index,       /* IN  - Service Control index    */
    unsigned32	       vect_index,     /* IN  - Index into result vector */
    boolean            abbrv_result,   /* IN  - To specify abbrev.result */
    gc_t_vector	     **result_vector,  /* IN  - The result vector        */
    gc_t_vector	     **error_vector)   /* IN  - The error vector         */
{
unsigned32  message_id;
OM_string   omstring;
OM_type     sc_omtype;
signed16    retval;
signed16    i;
char       *descript;
char       *default_value;
char	    c[GC_T_MAX_VAR + 1];

    retval = GC_SUCCESS;

    switch (sc_index)
    {
	case (0):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_automatic_continuation;
	    }
	    else
	    {
		descript = GC_T_AUTOMATIC_CONTINUATION_SWITCH + 1;
	    }
	    default_value = GC_T_TRUE_STR;
	    sc_omtype = DS_AUTOMATIC_CONTINUATION;
	    break;
	case (1):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_cache_class;
	    }
	    else
	    {
		descript = GC_T_CACHE_CLASS_SWITCH + 1;
	    }
	    default_value = GC_T_NONE_STR;
	    sc_omtype = DSX_NORMAL_CLASS;
	    break;
	case (2):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_chaining_prohib;
	    }
	    else
	    {
		descript = GC_T_CHAINING_PROHIBITED_SWITCH + 1;
	    }
	    default_value = GC_T_TRUE_STR;
	    sc_omtype = DS_CHAINING_PROHIB;
	    break;
	case (3):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_dont_deref_alias;
	    }
	    else
	    {
		descript = GC_T_DONT_DEREF_ALIAS_SWITCH + 1;
	    }
	    default_value = GC_T_FALSE_STR;
	    sc_omtype = DS_DONT_DEREFERENCE_ALIASES;
	    break;
	case (4):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_dont_use_copy;
	    }
	    else
	    {
		descript = GC_T_DONT_USE_COPY_SWITCH + 1;
	    }
	    default_value = GC_T_TRUE_STR;
	    sc_omtype = DS_DONT_USE_COPY;
	    break;
	case (5):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_use_dua_cache;
	    }
	    else
	    {
		descript = GC_T_USE_CACHE_SWITCH + 1;
	    }
	    default_value = GC_T_FALSE_STR;
	    sc_omtype = DSX_DUA_CACHE;
	    break;
	case (6):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_cache_first;
	    }
	    else
	    {
		descript = GC_T_CACHE_FIRST_SWITCH + 1;
	    }
	    default_value = GC_T_FALSE_STR;
	    sc_omtype = DSX_DUAFIRST;
	    break;
	case (7):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_local_scope;
	    }
	    else
	    {
		descript = GC_T_LOCAL_SCOPE_SWITCH + 1;
	    }
	    default_value = GC_T_FALSE_STR;
	    sc_omtype = DS_LOCAL_SCOPE;
	    break;
	case (8):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_prefer_adm_funcs;
	    }
	    else
	    {
		descript = GC_T_PREFER_ADMIN_FUNCTIONS_SWITCH + 1;
	    }
	    default_value = GC_T_FALSE_STR;
	    sc_omtype = DSX_PREFER_ADM_FUNCS;
	    break;
	case (9):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_prefer_chaining;
	    }
	    else
	    {
		descript = GC_T_PREF_CHAIN_SWITCH + 1;
	    }
	    default_value = GC_T_FALSE_STR;
	    sc_omtype = DS_PREFER_CHAINING;
	    break;
	case (10):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_priority;
	    }
	    else
	    {
		descript = GC_T_PRIORITY_SWITCH + 1;
	    }
	    default_value = GC_T_MEDIUM_STR;
	    sc_omtype = DS_PRIORITY;
	    break;
	case (11):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_scope_of_referral;
	    }
	    else
	    {
		descript = GC_T_SCOPE_OF_REFERRAL_SWITCH + 1;
	    }
	    default_value = GC_T_UNLIMITED_STR;
	    sc_omtype = DS_SCOPE_OF_REFERRAL;
	    break;
	case (12):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_size_limit;
	    }
	    else
	    {
		descript = GC_T_SIZE_LIMIT_SWITCH + 1;
	    }
	    default_value = GC_T_INFINITE_STR;
	    sc_omtype = DS_SIZE_LIMIT;
	    break;
	case (13):
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_time_limit;
	    }
	    else
	    {
		descript = GC_T_TIME_LIMIT_SWITCH + 1;
	    }
	    default_value = GC_T_INFINITE_STR;
	    sc_omtype = DS_TIME_LIMIT;
	    break;
	default:
	    if (abbrv_result == FALSE)
	    {
		message_id = gc_t_showdsc_use_dsa;
	    }
	    else
	    {
		descript = GC_T_USE_DSA_SWITCH + 1;
	    }
	    default_value = GC_T_TRUE_STR;
	    sc_omtype = DSX_USEDSA;
	    break;
    }

    if (public_context != NULL)
    {
	/* locate the descriptor */
	for (i = 0; public_context[i].type != OM_NO_MORE_TYPES; ++i)
	{
	    if (sc_omtype == DSX_NORMAL_CLASS)
	    {
		if (public_context[i].type == DSX_NORMAL_CLASS ||
		    public_context[i].type == DSX_PRIV_CLASS ||
		    public_context[i].type == DSX_RESIDENT_CLASS)
		{
		    if (public_context[i].value.boolean == OM_TRUE)
		    {
			break;
		    }
		}
	    }
	    else
	    {
		if (public_context[i].type == sc_omtype)
		{
		    break;
		}
	    }
	}

	if (public_context[i].type != OM_NO_MORE_TYPES)
	{
	    switch (public_context[i].syntax)
	    {
		case (OM_S_BOOLEAN):
		    if (public_context[i].type == DSX_NORMAL_CLASS)
		    {
			default_value = GC_T_NORMAL_STR;
			break;
		    }

		    if (public_context[i].type == DSX_PRIV_CLASS)
		    {
			default_value = GC_T_PRIVILEGE_STR;
			break;
		    }

		    if (public_context[i].type == DSX_RESIDENT_CLASS)
		    {
			default_value = GC_T_RESIDENT_STR;
			break;
		    }

		    if (public_context[i].value.boolean == OM_TRUE)
		    {
			default_value = GC_T_TRUE_STR;
		    }
		    else
		    {
			default_value = GC_T_FALSE_STR;
		    }
		    break;
		case (OM_S_ENUMERATION):
		    if (sc_omtype == DS_SCOPE_OF_REFERRAL)
		    {
			switch(public_context[i].value.enumeration)
			{
			    case (DS_DMD):
				default_value = GC_T_DMD_STR;
				break;
			    case (DS_COUNTRY):
				default_value = GC_T_COUNTRY_STR;
				break;
			    default:
				break;
			}
		    }
		    else
		    {
			switch(public_context[i].value.enumeration)
			{
			    case (DS_LOW):
				default_value = GC_T_LOW_STR;
				break;
			    case (DS_MEDIUM):
				default_value = GC_T_MEDIUM_STR;
				break;
			    case (DS_HIGH):
				default_value = GC_T_HIGH_STR;
				break;
			    default:
				break;
			}
		    }
		    break;
		case (OM_S_INTEGER):
		    sprintf(c, "%d", public_context[i].value.integer);
		    default_value = c;
		    break;
		default:
		    break;
	    }
	}
    }

    /* set the first result vector to GC_T_ATTRNAME */
    if (abbrv_result == FALSE)
    {
	/* get the service control name from the catalog */
	descript = (char *) dce_sprintf(message_id);

	if (descript == NULL)
	{
	    retval = GC_T_MEMORY_INSUFFICIENT;
	}
	else
	{
	    (*result_vector)[vect_index].type = GC_T_ATTRNAME;
	    (*result_vector)[vect_index].value = descript;
	}
    }
    else
    {
	omstring.elements = (void *) descript;
	omstring.length = strlen(descript);

	retval = gc_t_create_result_vector_data(*result_vector, vect_index,
						GC_T_ATTRNAME, omstring,
						OM_S_PRINTABLE_STRING);
    }

    if (retval == GC_SUCCESS)
    {
	omstring.elements = (void *) default_value;
	omstring.length = strlen(default_value);

	/* set the second result vector to GC_T_ATTRVAL */
	retval = gc_t_create_result_vector_data(*result_vector, vect_index + 1,
						GC_T_ATTRVAL, omstring,
						OM_S_PRINTABLE_STRING);
    }

    if (retval != GC_SUCCESS)
    {
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, error_vector);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
