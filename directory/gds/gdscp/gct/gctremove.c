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
 * $Log: gctremove.c,v $
 * Revision 1.1.4.2  1996/02/18  18:19:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:54  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:31  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:51  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:49  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:15  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:55:32  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:36:37  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:01  keutel
 * 	creation
 * 	[1994/03/21  12:56:52  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctremove.c					   */
/* Description : This file contains all the modules for the        */
/*		 directory remove entry operation.		   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctremove.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:19:05 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include <gds.h>

#include <xom.h>
#include <xds.h>

#include <gdscp.h>
#include <gctconv.h>
#include <gctstruct.h>
#include <gcterr.h>
#include <gctutil.h>
#include <gctargs.h>
#include <gctbind.h>
#include <gctmoddsc.h>

/*-- Static Function(s) -------------------------------------------*/


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_remove					   */
/* Description   : This function is used to remove an object entry */
/*		   from the DIT. 				   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_remove(
    char        **in_vect,       /* IN  - Array of input vectors          */
    signed16      vect_count,    /* IN  - Count of input vectors          */
    boolean       local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect)      /* OUT - NULL (success) or error message */
{
OM_private_object  session;
OM_private_object  context;
OM_private_object  name;
OM_sint		   invoke_id_return;
DS_status	   ds_status;
signed16  	   retval;
signed16  	   i;

    if (out_vect == NULL || vect_count < 0)
	return(GC_T_INVALID_PARAMETER);

    i = 0;
    name = NULL;
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
	/* validate the input arguments */
	retval = gc_t_check_args(GC_T_REMOVE, in_vect, vect_count, NULL, 
				 out_vect);
	if (retval ==  GC_SUCCESS)
	{
	    /* get the context object */
	    context = gc_t_get_context();

	    /* extract the name string from the first input vector and */
	    /* convert it to DS_C_DS_DN object.			       */
	    retval = gc_t_string_to_name(in_vect[0], local_strings, &name, 
					 out_vect);

	    if (retval == GC_SUCCESS)
	    {
		/* perform the remove entry operation */
		ds_status = ds_remove_entry(session, context, name, 
					    &invoke_id_return);

		if (ds_status != DS_SUCCESS)
		{
		    /* map the XDS error to translator defined error and */
		    /* create the error vector.                          */
		    retval = gc_t_map_xds_error(ds_status, local_strings, 
						out_vect);

		    /* free the error object */
		    om_delete(ds_status);
		}
	    }
	}
    }

    /* free the create object */
    if (name != NULL)
	om_delete(name);

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
