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
 * $Log: gctmoveto.c,v $
 * Revision 1.1.4.2  1996/02/18  18:19:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:51  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:27  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:49  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:47  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:13  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:55:24  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:36:30  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:57  keutel
 * 	creation
 * 	[1994/03/21  12:56:47  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctmoveto.c					   */
/* Description : This file contains all the modules for the 	   */
/*		 "moveto" and "pwo" commands of GDSCP.		   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctmoveto.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:19:01 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <gds.h>

#include <xom.h>
#include <xds.h>

#include <gdscp.h>
#include <gctstruct.h>
#include <gctconv.h>
#include <gctutil.h>
#include <gctargs.h>
#include <gcterr.h>
#include <gctbind.h>
#include <gctmoddsc.h>
#include <gctmoveto.h>

/*-- Local  Define(s)   -------------------------------------------*/

/* Import the necessary class identifiers */
OM_IMPORT(DS_C_ENTRY_INFO_SELECTION)

/*-- Static Variable(s) -------------------------------------------*/
/* To store the current object in the DIT */
static OM_string *gc_t_current_dit_position = NULL;

/*-- Static Function(s) -------------------------------------------*/
static signed16 gc_t__perform_moveto_operation(
    char         *dn_string,     /* IN  - The input DN string 		  */
    boolean       local_strings, /* IN  - To specify local strings        */
    boolean      *only_root,	 /* OUT - TRUE if input is root DN        */
    gc_t_vector **out_vect);     /* OUT - NULL (success) or error message */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_moveto					   */
/* Description   : This function is used to change the current 	   */
/*		   position in the DIT to the specified object name*/
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_moveto(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean       local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect)      /* OUT - NULL (success) or error message */
{
signed16  retval;
boolean   only_root;

    if (out_vect == NULL || vect_count < 0)
	return(GC_T_INVALID_PARAMETER);

    *out_vect = NULL;
    retval = GC_SUCCESS;

    if (gc_t_get_bound_session() == NULL)
    {
	retval = GC_T_BIND_NOT_MADE;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, out_vect);
    }
    else
    {
	/* validate the input arguments */
	retval = gc_t_check_args(GC_T_MOVETO, in_vect, vect_count, 
				 NULL, out_vect);
	if (retval == GC_SUCCESS)
	{
	    /* perform a read operation to check if the object exists */
	    retval = gc_t__perform_moveto_operation(in_vect[0], local_strings,
						    &only_root, out_vect);

	    /* if object exists */
	    if (retval == GC_SUCCESS)
	    {
		/* update the current DIT position */
		if (only_root == TRUE)
		{
		    retval = gc_t_update_current_dit(GC_T_ROOT_DN);
		}
		else
		{
		    retval = gc_t_update_current_dit(in_vect[0]);
		}

		if (retval != GC_SUCCESS)
		{
		    /* create the error vector */
		    gc_t_create_gct_error_vector(retval, NULL, out_vect);
		}
	    }
	}
    }

    if (retval == GC_SUCCESS)
	*out_vect = NULL;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__perform_moveto_operation		   */
/* Description   : This function is used to perform a directory    */
/*		   read operation and confirm if the object exists */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__perform_moveto_operation(
    char         *dn_string,     /* IN  - The input DN string 		  */
    boolean       local_strings, /* IN  - To specify local strings        */
    boolean      *only_root,	 /* OUT - TRUE if input is root DN        */
    gc_t_vector **out_vect)      /* OUT - NULL (success) or error message */
{
OM_private_object  session;	
OM_private_object  context;	
OM_private_object  result;	
OM_private_object  name;	
OM_sint		   invoke_id_return;
DS_status	   ds_status;
signed16  	   retval, i;
OM_descriptor      entry_selection[] = {
      OM_OID_DESC(OM_CLASS, DS_C_ENTRY_INFO_SELECTION) ,
    { DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, { { OM_FALSE, 0 } } },
    { DS_INFO_TYPE, OM_S_ENUMERATION, { { DS_TYPES_ONLY, 0 } } },
      OM_NULL_DESCRIPTOR };

    retval = GC_SUCCESS;
    *only_root = FALSE;
    name = NULL;

    /* get the session object */
    session = gc_t_get_bound_session();

    /* get the context object */
    context = gc_t_get_context();

    i = 0;
    /* skip past white spaces */
    while (isspace(dn_string[i]))
	++i;

    if (strncmp(&dn_string[i], GC_T_ROOT_DN, strlen(GC_T_ROOT_DN)) == 0)
    {
	i += strlen(GC_T_ROOT_DN);

	/* skip past white spaces */
	while (isspace(dn_string[i]))
	    ++i;

	/* if end of string */
	if (dn_string[i] == 0)
	{
	    /* since only root object specified, do not perform the */
	    /* read operation 				            */
	    *only_root = TRUE;
	}
    }

    if (*only_root == FALSE)
    {
	retval = gc_t_string_to_name(dn_string, local_strings, &name, out_vect);

	if (retval == GC_SUCCESS)
	{
	    /* perform the directory read operation */
	    ds_status = ds_read(session, context, name, entry_selection,
				&result, &invoke_id_return);

	    if (ds_status != DS_SUCCESS)
	    {
		/* map the XDS error to translator defined error and */
		/* create the error vector.                          */
		retval = gc_t_map_xds_error(ds_status, local_strings, out_vect);

		/* free the error object */
		om_delete(ds_status);
	    }
	    else
	    {
		/* delete the result object */
		om_delete(result);
	    }
	}
    }

    /* delete the created objects */
    if (name != NULL)
	om_delete(name);

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_update_current_dit			   */
/* Description   : This function is used to update the current 	   */
/*		   position variable to the specified object name  */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_update_current_dit(
    char  *dn_string)  /* IN  - The input DN string */
{
OM_string   full_dn;
signed16    dn_len, i;
signed16    retval;

    i = 0;
    retval = GC_SUCCESS;

    /* skip past white spaces */
    while (isspace(dn_string[i]))
	++i;

    /* if full DN Name not specified */
    if (dn_string[i] != GC_T_RDN_DELIMITER_OP)
    {
	retval = gc_t_create_full_dn_string(dn_string, &full_dn);
    }
    else
    {
	dn_len = strlen(dn_string);
	full_dn.elements = malloc(sizeof(char) * (dn_len + 1));
	if (full_dn.elements != NULL)
	{
	    memcpy(full_dn.elements, dn_string, dn_len + 1);
	    full_dn.length = dn_len;
	}
	else
	{
	    retval = GC_T_MEMORY_INSUFFICIENT;
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* if first time */
	if (gc_t_current_dit_position == NULL)
	{
	    gc_t_current_dit_position = (OM_string *) malloc(sizeof(OM_string));

	    if (gc_t_current_dit_position == NULL)
	    {
		retval = GC_T_MEMORY_INSUFFICIENT;
	    }
	    else
	    {
		gc_t_current_dit_position->elements = NULL;
	    }
	}

	if (retval == GC_SUCCESS)
	{
	    /* free the current area */
	    if (gc_t_current_dit_position->elements != NULL)
	    {
		free(gc_t_current_dit_position->elements);
	    }
	    gc_t_current_dit_position->elements = full_dn.elements;
	    gc_t_current_dit_position->length = full_dn.length;
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_pwo					   */
/* Description   : This function returns the name of the object	   */
/*		   at the current position in the DIT.		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_pwo(
    gc_t_vector **out_vect)  /* OUT - The result or the error message */
{
OM_string  *current;
signed16    retval;
signed16    vect_index;

    if (out_vect == NULL)
	return(GC_T_INVALID_PARAMETER);

    retval = GC_SUCCESS;
    vect_index = 0;

    current = gc_t_get_current_dit_position();

    /* allocate the size for the out vector. One more is required */
    /* to indicate end of vector.				  */
    *out_vect = (gc_t_vector *) malloc(sizeof(gc_t_vector) * 2);

    if (*out_vect != NULL)
    {
	/* fill the first vector type with GC_T_DN */
	retval = gc_t_create_result_vector_data(*out_vect, vect_index, 
						GC_T_DN, *current,
						OM_S_PRINTABLE_STRING);
	if (retval == GC_SUCCESS)
	{
	    ++vect_index;

	    /* initialize the second vector type to GC_T_EOL */
	    (*out_vect)[vect_index].type = GC_T_EOL;
	    (*out_vect)[vect_index].value = 0;
	}
	else
	{
	    free(*out_vect);
	}
    }
    else
    {
	retval = GC_T_MEMORY_INSUFFICIENT;
    }

    if (retval != GC_SUCCESS)
    {
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, out_vect);
    }
    
    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_get_current_dit_position		   */
/* Description   : This function is used to return the current 	   */
/*		   position in the DIT.				   */
/* Return Value  : The current position in the DIT		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_string *gc_t_get_current_dit_position(
    void)
{
    /* if gc_t_current_dit_position has not been initialized, set it to "/". */
    if (gc_t_current_dit_position == NULL)
    {
	gc_t_update_current_dit(GC_T_ROOT_DN);
    }

    return(gc_t_current_dit_position);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_free_current_dit_position		   */
/* Description   : This function is used to free the memory area   */
/*		   occupied by the current DIT position string     */ 
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_t_free_current_dit_position(
    void)
{
    if (gc_t_current_dit_position != NULL)
    {
	if (gc_t_current_dit_position->elements != NULL)
	{
	    free(gc_t_current_dit_position->elements);
	}

	free(gc_t_current_dit_position);
	gc_t_current_dit_position = NULL;
    }
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
