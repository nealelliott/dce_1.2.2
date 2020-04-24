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
 * $Log: gciutil.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:49  root]
 * 
 * Revision 1.1.2.4  1994/09/19  08:39:25  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:32:37  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:25  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:16  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:47  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/05  06:41:33  marrek]
 * 
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:57:07  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:09  keutel
 * 	creation
 * 	[1994/03/21  12:55:06  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gciutil.c                                         */
/* Description : Utility functions for gdscp interpreter           */
/* Date        : Aug 02, 1993                                      */
/* Author      : A. Doerr, SNI BU BA KP13                          */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gciutil.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:37 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <gds.h>

#include <dce/tcl.h>

#include <gdscp.h>
#include <gctstruct.h>
#include <gctconv.h>
#include <gctargs.h>

#include <gcistruct.h>
#include <gciutil.h>


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_convert_result                             */
/* Description   : Converts the results received from the 	   */
/*		   translator after a show or a search operation   */
/*		   to a TCL list.				   */
/*		   The created TCL lists are different for the     */
/*		   show and search operations.			   */
/*		   The TCL interpreter result is set to the	   */
/*		   converted TCL list.	In case of errors, the     */
/*		   TCL interpreter result is set to the error      */
/*		   message.					   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : A. Doerr, SNI BU BA KP13                        */
/*                 M. Kronenburg, SNI BU BA KP13                   */
/*                 B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_i_convert_result(
    boolean       is_show_result, /* IN  - Result is from show operation */
    gc_t_vector  *vector,    	  /* IN  - Vector to convert 		 */
    Tcl_Interp   *interp)         /* OUT - The Tcl-interpreter           */
{
unsigned32  vect_idx;
unsigned32  i;
unsigned32  entry_info_count;
unsigned32  result_count;
unsigned32  temp_count;
signed16    retval;
char      **result_vector;
char      **entry_vector;
char       *result_list;
int         result_idx;
int         entry_idx;

    i = 0;
    vect_idx = result_idx = entry_idx = 0;
    temp_count = entry_info_count = result_count = 0;
    entry_vector = result_vector = NULL;
    retval = GC_SUCCESS;

    /* count vector items */
    while (vector[i].type != GC_T_EOL)
    {
	++result_count;
	++i;
	temp_count = 1;

	/* now count the attribute information vectors */
	while (!(vector[i].type == GC_T_EOL || vector[i].type == GC_T_DN))
	{
	    ++temp_count;
	    ++i;
	}

	/* this is done to compute the maximum no. of the vectors  */
	/* required for entry information.  Some results may have  */
	/* only 2 entry information vectors, some more.		   */
	if (entry_info_count < temp_count)
	{
	    entry_info_count = temp_count;
	}
    }

    entry_vector = (char **) malloc(sizeof(char *) * entry_info_count);

    if (entry_vector == NULL)
    {
	retval = GC_I_MEMORY_INSUFFICIENT;
    }
    else
    {
	result_vector = (char **) malloc(sizeof(char *) * result_count);

	if (result_vector == NULL)
	{
	    free(entry_vector);
	    retval = GC_I_MEMORY_INSUFFICIENT;
	}
    }

    if (retval == GC_SUCCESS)
    {
	while (vector[vect_idx].type != GC_T_EOL)
	{
	    while (vector[vect_idx].type == GC_T_DN)
	    {
		/* collect the distinguished name */
		entry_vector[entry_idx] = vector[vect_idx].value;
		++vect_idx;
		++entry_idx;

		/* collect all the attribute information strings */
		while (vector[vect_idx].type == GC_T_ATTRINFO ||
		       vector[vect_idx].type == GC_T_ATTRABBRV)
		{
		    entry_vector[entry_idx] = vector[vect_idx].value;
		    ++vect_idx;
		    ++entry_idx;
		}

		/* build sublist with DNs & attribute information */
		result_vector[result_idx] = Tcl_Merge(entry_idx, entry_vector);

		if (result_vector[result_idx] == NULL)
		{
		    retval = GC_I_MEMORY_INSUFFICIENT;
		    break;
		}

		++result_idx;

		/* reset entry vector */
		entry_idx = 0;
	    }

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    /* if it is any of these vectors, it means that the result */
	    /* is partial.  So include this message also as a part of  */
	    /* the TCL list.                                           */
	    if (vector[vect_idx].type == GC_T_REF_DSA ||
		vector[vect_idx].type == GC_T_REF_ADDR ||
		vector[vect_idx].type == GC_T_LIMIT_PROBLEM ||
		vector[vect_idx].type == GC_T_NO_CRIT_EXT)
	    {
		if (vector[vect_idx].type == GC_T_REF_DSA)
		{
		    /* get message string from catalog */
		    result_vector[result_idx] = (char *) dce_sprintf(
					     gc_t_incomplete_referral_returned,
					     vector[vect_idx].value);

		    if (result_vector[result_idx] == NULL)
		    {
			retval = GC_I_MEMORY_INSUFFICIENT;
			break;
		    }
		}
		else
		{
		    result_vector[result_idx] =  vector[vect_idx].value;
		}
		++result_idx;
		break;
	    }
	}

	/* now free the vector */
	free(entry_vector);

	if (retval != GC_SUCCESS)
	{
	    /* free the vectors */
	    for (i = 0; i < result_idx; ++i)
	    {
		free(result_vector[i]);
	    }
	}
	else
	{
	    /* build final result String only if it is not a show operation */
	    if (is_show_result != TRUE)
	    {
		result_list = Tcl_Merge(result_idx, result_vector);

		if (result_list == NULL)
		{
		    retval = GC_I_MEMORY_INSUFFICIENT;
		}
		else
		{
		    /* set the interpreter result */
		    Tcl_SetResult(interp, result_list, TCL_DYNAMIC);
		}

		/* now free the result vectors */
		for (i = 0; i < result_idx; ++i)
		{
		    free(result_vector[i]);
		}
	    }
	    else
	    {
		/* set the interpreter result */
		Tcl_SetResult(interp, *result_vector, TCL_DYNAMIC);

		if (vector[vect_idx].type == GC_T_REF_DSA)
		{
		    free(result_vector[result_idx - 1]);
		}
	    }
	}

	free(result_vector);
    }

    if (retval != GC_SUCCESS)
    {
	/* set the interpreter result to the appropriate error message */
	gc_i_set_interpreter_error(retval, NULL, interp);
    }

    return (retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_convert_list_result                        */
/* Description   : Converts the results received from the 	   */
/*		   translator after a list operation to a TCL list */
/*		   The TCL interpreter result is set to the	   */
/*		   converted TCL list.	In case of errors, the     */
/*		   TCL interpreter result is set to the error      */
/*		   message.					   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                 B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_i_convert_list_result(
    gc_t_vector  *vector,  /* IN  - Vector to convert   */
    Tcl_Interp   *interp)  /* OUT - The Tcl-interpreter */
{
unsigned32  vect_idx;
unsigned32  result_count;
signed16    retval;
char      **result_vector;
char       *result_list;
int         result_idx;


    result_count = 0;
    vect_idx = result_idx = result_count = 0;
    retval = GC_SUCCESS;

    /* count vector items */
    while (vector[result_count].type != GC_T_EOL)
    {
	 ++result_count;
    }

    result_vector = (char **) malloc(sizeof(char *) * result_count);

    if (result_vector == NULL)
    {
	retval = GC_I_MEMORY_INSUFFICIENT;
    }
    else
    {
	while (vector[vect_idx].type != GC_T_EOL)
	{
	    /* if it is any of these vectors, it means that the result */
	    /* is partial.  So include this message also as a part of  */
	    /* the TCL list.                                           */
	    if (vector[vect_idx].type == GC_T_REF_DSA ||
		vector[vect_idx].type == GC_T_REF_ADDR ||
		vector[vect_idx].type == GC_T_LIMIT_PROBLEM ||
		vector[vect_idx].type == GC_T_NO_CRIT_EXT)
	    {
		if (vector[vect_idx].type == GC_T_REF_DSA)
		{
		    /* get message string from catalog */
		    result_vector[result_idx] = (char *) dce_sprintf(
					     gc_t_incomplete_referral_returned,
					     vector[vect_idx].value);

		    if (result_vector[result_idx] == NULL)
		    {
			retval = GC_I_MEMORY_INSUFFICIENT;
			break;
		    }
		}
		else
		{
		    result_vector[result_idx] = vector[vect_idx].value;
		}
		++result_idx;
		break;
	    }

	    result_vector[result_idx] = vector[vect_idx].value;
	    ++vect_idx;
	    ++result_idx;
	}

	if (retval == GC_SUCCESS)
	{
	    /* build final result String */
	    result_list = Tcl_Merge(result_idx, result_vector);

	    if (vector[vect_idx].type == GC_T_REF_DSA)
	    {
		free(result_vector[result_idx - 1]);
	    }

	    /* free result-vectors */
	    free(result_vector);

	    if (result_list == NULL)
	    {
		retval = GC_I_MEMORY_INSUFFICIENT;
	    }
	    else
	    {
		/* set the interpreter result */
		Tcl_SetResult(interp, result_list, TCL_DYNAMIC);
	    }
	}
    }

    if (retval != GC_SUCCESS)
    {
	/* set the interpreter result to the appropriate error message */
	gc_i_set_interpreter_error(retval, NULL, interp);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_convert_showdsc_result                     */
/* Description   : Converts the results received from the 	   */
/*		   translator after a show service controls        */
/*		   operation to a TCL list 			   */
/*		   The TCL interpreter result is set to the	   */
/*		   converted TCL list.	In case of errors, the     */
/*		   TCL interpreter result is set to the error      */
/*		   message.					   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                 B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_i_convert_showdsc_result(
    gc_t_vector  *vector,  /* IN  - Vector to convert   */
    Tcl_Interp   *interp)  /* OUT - The Tcl-interpreter */
{
signed16   vect_idx;
signed16   result_count;
signed16   retval;
signed16   i;
char     **result_vector;
char      *service_vector[2];
char      *result_list;
int        result_idx;

    vect_idx = result_idx = result_count = 0;
    retval = GC_SUCCESS;

    /* count vector items */
    for (i = 0; vector[i].type != GC_T_EOL; ++i)
    {
	if (vector[i].type == GC_T_ATTRNAME)
	{
	    ++result_count;
	}
    }

    result_vector = (char **) malloc(sizeof(char *) * result_count);

    if (result_vector == NULL)
    {
	retval = GC_I_MEMORY_INSUFFICIENT;
    }
    else
    {
	/* scan all entries */
	while (vector[vect_idx].type == GC_T_ATTRNAME)
	{
	    service_vector[0] = vector[vect_idx].value;
	    ++vect_idx;

	    service_vector[1] = vector[vect_idx].value;
	    ++vect_idx;

	    result_vector[result_idx] = Tcl_Merge(2, service_vector);

	    if (result_vector[result_idx] == NULL)
	    {
		retval = GC_I_MEMORY_INSUFFICIENT;
		break;
	    }

	    ++result_idx;
	}

	if (retval == GC_SUCCESS)
	{
	    /* build final result String */
	    result_list = Tcl_Merge(result_idx, result_vector);

	    if (result_list == NULL)
	    {
		retval = GC_I_MEMORY_INSUFFICIENT;
	    }
	    else
	    {
		/* set the interpreter result */
		Tcl_SetResult(interp, result_list, TCL_DYNAMIC);
	    }
	}

	/* now free the intermediate vectors */
	for (i = 0; i < result_idx; ++i)
	{
	    free(result_vector[i]);
	}

	/* free vectors */
	free(result_vector);
    }

    if (retval != GC_SUCCESS)
    {
	/* set the interpreter result to the appropriate error message */
	gc_i_set_interpreter_error(retval, NULL, interp);
    }

    return (retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_convert_showabbrv_result                   */
/* Description   : Converts the results received from the 	   */
/*		   translator after a show abbreviations           */
/*		   operation to a TCL list 			   */
/*		   The TCL interpreter result is set to the	   */
/*		   converted TCL list.	In case of errors, the     */
/*		   TCL interpreter result is set to the error      */
/*		   message.					   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                 B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_i_convert_showabbrv_result(
    gc_t_vector  *vector,  /* IN  - Vector to convert   */
    Tcl_Interp   *interp)  /* OUT - The Tcl-interpreter */
{
unsigned32    vect_idx;
unsigned32    obj_count, attr_count, result_count;
unsigned32    all_stru_attr_count, temp_comp_count, comp_count;
unsigned32    i;
signed16      retval;
char  	    **all_obj_vect;
char  	    **all_attr_vect;
char  	    **all_stru_attr_vect;
char  	    **stru_attr_vect;
char  	    **result_vector;
char   	     *single_obj_vect[3];
char   	     *single_attr_vect[3];
char   	     *single_comp_vect[2];
char   	     *result_list;
int           stru_attr_idx;
int           all_obj_idx, all_attr_idx, all_stru_attr_idx, result_idx;

    i = 0;
    vect_idx = stru_attr_idx = 0;
    all_obj_idx = all_attr_idx = all_stru_attr_idx = result_idx = 0;
    obj_count = attr_count = result_count = 0;
    all_stru_attr_count = temp_comp_count = comp_count = 0;
    all_obj_vect = all_attr_vect = all_stru_attr_vect = NULL;
    stru_attr_vect = result_vector = NULL;
    retval = GC_SUCCESS;

    while (vector[i].type != GC_T_EOL)
    {
	switch(vector[i].type)
	{
	    case (GC_T_OCLABBRV):
		while (vector[i].type == GC_T_OCLABBRV)
		{
		    /* skip past the abbreviation, full name and */
		    /* object identifier vectors.		 */
		    i += 3;

		    ++obj_count;
		}
		++result_count;
		break;
	    case (GC_T_ATTRABBRV):
		while (vector[i].type == GC_T_ATTRABBRV)
		{
		    /* skip past the abbreviation, full name and */
		    /* object identifier vectors.		 */
		    i += 3;

		    ++attr_count;
		}
		++result_count;
		break;
	    case (GC_T_SATTRNAME):
		while (vector[i].type == GC_T_SATTRNAME)
		{
		    ++all_stru_attr_count;
		    ++i;
		    temp_comp_count = 1;

		    while (vector[i].type == GC_T_COMPABBRV)
		    {
			/* skip past the abbreviation and full name vectors */
			i += 2;

			++temp_comp_count;
		    }

		    /* this is done to compute the maximum no. of vectors   */
		    /* required for components.  Some structured attributes */
		    /* may have only one component vectors, some more.	    */
		    if (comp_count < temp_comp_count)
		    {
			comp_count = temp_comp_count;
		    }
		}
		++result_count;
		break;
	    default:
		break;
	}
    }

    /* allocate the area for the object class abbreviations vectors */
    all_obj_vect = (char **) malloc(sizeof(char *) * obj_count);

    if (all_obj_vect == NULL)
    {
	retval = GC_I_MEMORY_INSUFFICIENT;
    }
    else
    {
	/* allocate the area for the attribute abbreviations vectors */
	all_attr_vect = (char **) malloc(sizeof(char *) * attr_count);

	if (all_attr_vect == NULL)
	{
	    retval = GC_I_MEMORY_INSUFFICIENT;
	}
	else
	{
	    /* allocate area for structured attribute component abbreviations */
	    stru_attr_vect = (char **) malloc(sizeof(char *) * comp_count);

	    if (stru_attr_vect == NULL)
	    {
		retval = GC_I_MEMORY_INSUFFICIENT;
	    }
	    else
	    {
		/* allocate area for structured attribute abbreviations */
		all_stru_attr_vect = (char **) malloc(sizeof(char *) * 
						      all_stru_attr_count);

		if (all_stru_attr_vect == NULL)
		{
		    retval = GC_I_MEMORY_INSUFFICIENT;
		}
		else
		{
		   /* allocate area for result vector */
		    result_vector = (char **) 
				    malloc(sizeof(char *) * result_count);

		    if (result_vector == NULL)
		    {
			retval = GC_I_MEMORY_INSUFFICIENT;
		    }
		}
	    }
	}
    }

    if (retval != GC_SUCCESS)
    {
	if (all_obj_vect != NULL)
	    free(all_obj_vect);

	if (all_attr_vect != NULL)
	    free(all_attr_vect);

	if (stru_attr_vect != NULL)
	    free(stru_attr_vect);

	if (all_stru_attr_vect != NULL)
	    free(all_stru_attr_vect);

	if (result_vector != NULL)
	    free(result_vector);
    }
    else
    {
	while (vector[vect_idx].type != GC_T_EOL)
	{
	    switch(vector[vect_idx].type)
	    {
		case (GC_T_OCLABBRV):
		    /* scan all object class abbreviations */
		    while (vector[vect_idx].type == GC_T_OCLABBRV)
		    {
			single_obj_vect[0] = vector[vect_idx].value;
			++vect_idx;

			single_obj_vect[1] = vector[vect_idx].value;
			++vect_idx;

			single_obj_vect[2] = vector[vect_idx].value;
			++vect_idx;

			all_obj_vect[all_obj_idx] = 
						 Tcl_Merge(3, single_obj_vect);

			if (all_obj_vect[all_obj_idx] == NULL)
			{
			    retval = GC_I_MEMORY_INSUFFICIENT;
			    break;
			}

			++all_obj_idx;
		    }

		    if (retval != GC_SUCCESS)
		    {
			break;
		    }

		    result_vector[result_idx] = Tcl_Merge(all_obj_idx, 
							  all_obj_vect);

		    if (result_vector[result_idx] == NULL)
		    {
			retval = GC_I_MEMORY_INSUFFICIENT;
			break;
		    }

		    ++result_idx;

		    /* now free the intermediate vectors */
		    for (i = 0; i < all_obj_idx; ++i)
		    {
			free(all_obj_vect[i]);
		    }
		    free(all_obj_vect);
		    break;

		case (GC_T_ATTRABBRV) :
		    /* scan all attribute abbreviations */
		    while (vector[vect_idx].type == GC_T_ATTRABBRV)
		    {
			single_attr_vect[0] = vector[vect_idx].value;
			++vect_idx;

			single_attr_vect[1] = vector[vect_idx].value;
			++vect_idx;

			single_attr_vect[2] = vector[vect_idx].value;
			++vect_idx;

			all_attr_vect[all_attr_idx] = 
						Tcl_Merge(3, single_attr_vect);

			if (all_attr_vect[all_attr_idx] == NULL)
			{
			    retval = GC_I_MEMORY_INSUFFICIENT;
			    break;
			}

			++all_attr_idx;
		    }

		    if (retval != GC_SUCCESS)
		    {
			break;
		    }

		    result_vector[result_idx] = Tcl_Merge(all_attr_idx, 
							  all_attr_vect);

		    if (result_vector[result_idx] == NULL)
		    {
			retval = GC_I_MEMORY_INSUFFICIENT;
			break;
		    }

		    ++result_idx;

		    /* now free all the intermediate vectors */
		    for (i = 0; i < all_attr_idx; ++i)
		    {
			free(all_attr_vect[i]);
		    }
		    free(all_attr_vect);
		    break;

		case (GC_T_SATTRNAME) :
		    /* scan all component abbreviations */
		    while (vector[vect_idx].type == GC_T_SATTRNAME)
		    {
			stru_attr_vect[stru_attr_idx] = vector[vect_idx].value;
			++stru_attr_idx;
			++vect_idx;

			/* scan all component abbreviations for one component */
			while (vector[vect_idx].type == GC_T_COMPABBRV)
			{
			    single_comp_vect[0] = vector[vect_idx].value;
			    ++vect_idx;

			    single_comp_vect[1] = vector[vect_idx].value;
			    ++vect_idx;

			    stru_attr_vect[stru_attr_idx] = 
						Tcl_Merge(2, single_comp_vect);

			    if (stru_attr_vect[stru_attr_idx] == NULL)
			    {
				retval = GC_I_MEMORY_INSUFFICIENT;
				break;
			    }

			    ++stru_attr_idx;
			}

			if (retval != GC_SUCCESS)
			    break;

			all_stru_attr_vect[all_stru_attr_idx] = 
				      Tcl_Merge(stru_attr_idx, stru_attr_vect);

			if (all_stru_attr_vect[all_stru_attr_idx] == NULL)
			{
			    retval = GC_I_MEMORY_INSUFFICIENT;
			    break;
			}

			++all_stru_attr_idx;

			/* now free all the intermediate vectors */
			for (i = 1; i < stru_attr_idx; ++i)
			{
			    free(stru_attr_vect[i]);
			}
			stru_attr_idx = 0;
		    }

		    if (retval != GC_SUCCESS)
		    {
			break;
		    }

		    result_vector[result_idx] = Tcl_Merge(all_stru_attr_idx, 
							  all_stru_attr_vect);

		    if (result_vector[result_idx] == NULL)
		    {
			retval = GC_I_MEMORY_INSUFFICIENT;
			break;
		    }

		    ++result_idx;

		    /* now free all the intermediate vectors */
		    for (i = 0; i < all_stru_attr_idx; ++i)
		    {
			free(all_stru_attr_vect[i]);
		    }

		    free(stru_attr_vect);
		    free(all_stru_attr_vect);
		    break;

		default:
		    break;
	    }

	    if (retval != GC_SUCCESS)
	    {
		/* free all the intermediate vectors */
		for (i = 0; i < all_obj_idx; ++i)
		{
		    free(all_obj_vect[i]);
		}
		free(all_obj_vect);

		for (i = 0; i < all_attr_idx; ++i)
		{
		    free(all_attr_vect[i]);
		}
		free(all_attr_vect);

		for (i = 1; i < stru_attr_idx; ++i)
		{
		    free(stru_attr_vect[i]);
		}
		free(stru_attr_vect);

		for (i = 0; i < all_stru_attr_idx; ++i)
		{
		    free(all_stru_attr_vect[i]);
		}
		free(all_stru_attr_vect);

		for (i = 0; i < result_idx; ++i)
		{
		    free(result_vector[i]);
		}

		break;
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* build final result String */
	result_list = Tcl_Merge(result_idx, result_vector);

	if (result_list == NULL)
	{
	    retval = GC_I_MEMORY_INSUFFICIENT;
	}
	else
	{
	    /* set the interpreter result */
	    Tcl_SetResult(interp, result_list, TCL_DYNAMIC);
	}

	/* free the intermediate vectors */
	for (i = 0; i < result_idx; ++i)
	{
	    free(result_vector[i]);
	}

	free(result_vector);
    }

    if (retval != GC_SUCCESS)
    {
	/* set the interpreter result to the appropriate error message */
	gc_i_set_interpreter_error(retval, NULL, interp);
    }

    return (retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_repack_arguments                           */
/* Description   : This function creates a new character vector    */
/*		   array from the supplied input vector array.     */
/*		   The input character vector arguments are split  */
/*		   into elements by invoking the Tcl_Split_list    */
/*		   procedure and a new character vector array is   */
/*		   created.  This function also checks if the      */
/*		   GC_T_STRUCTURED_RESULT_SWITCH has been specified*/
/*		   If present, it appropriately sets the output    */
/*                 argument. The created array does not contain the*/
/*		   above switch.        			   */
/*		   In case of errors, the interpreter result is    */
/*		   set with the error message.			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.           */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_i_repack_arguments(
    char           **argv,              /* IN  - The argument vector          */
    int              argc,              /* IN  - Number of arguments          */
    gc_t_op_type     op_type, 	        /* IN  - Type of operation	      */
    char          ***new_argv,          /* OUT - The created vector           */
    signed16        *new_argc,          /* OUT - Count of created vectors     */
    boolean         *structured_result, /* OUT - struc.result switch specified*/
    Tcl_Interp      *interp)            /* OUT - The Tcl-interpreter          */
{
gc_t_vector  *error_vector;
signed16      retval;
signed16      vect_count;
signed16      total_count;
signed16      i, j;
boolean       is_present;
char 	    **vect_array;
char	    **split_args;
int           split_count;

    retval = GC_SUCCESS;
    vect_array = NULL;
    total_count = 0;

    /* initialize the output argument */
    if (structured_result != NULL)
    {
	*structured_result = FALSE;
    }

    /* count the number of vectors required */
    for (i = 0; i < argc; ++i)
    {
	/* if a switch has been specified */
	if (argv[i][0] == '-')
	{
	    ++total_count;
	    continue;
	}

	/* Argument specified. Break into elements */
	if (Tcl_SplitList(interp, argv[i], &split_count, &split_args) != TCL_OK)
	{
	    /* Invalid TCL list. The interpreter result will have */
	    /* been appropriately set by TCL_SplitList		  */
	    retval = GC_I_INVALID_ARGUMENT;
	    break;
	}

	/* check if the elements of the list are valid */
	for (j = 0; j < split_count; ++j)
	{
	    /* if switch is specified in the TCL list */
	    if (split_args[j][0] == '-')
	    {
		retval = GC_I_INVALID_ARGUMENT;

		/* set the interpreter to the error message */
		gc_i_set_interpreter_error(retval, argv[i], interp);

		break;
	    }
	}

	if (split_count > 0)
	{
	    /* now free the returned elements */
	    free ((char *) split_args);
	}

	if (retval != GC_SUCCESS)
	{
	    break;
	}

	total_count += split_count;
    }

    if (retval == GC_SUCCESS)
    {
	/* allocate the area for the new vector. One more is needed for NULL */
	vect_array = (char **) malloc(sizeof(char *) * (total_count + 1));

	if (vect_array == NULL)
	{
	    retval = GC_I_MEMORY_INSUFFICIENT;
	    /* set the interpreter result to the error message */
	    gc_i_set_interpreter_error(retval, NULL, interp);
	}
	else
	{
	    vect_count = 0;
	    for (i = 0; i < argc; ++i)
	    {
		/* if a switch is specified */
		if (argv[i][0] == '-')
		{
		    /* if GC_T_STRUCTURED_RESULT_SWITCH switch to be checked */
		    if (structured_result != NULL)
		    {
			/* invoke the translator function to check if  */
			/* structured result switch has been specified */
			retval = gc_t_check_switch(op_type, argv[i],
						  GC_T_STRUCTURED_RESULT_SWITCH,
						  &is_present, &error_vector);
			if (retval != GC_SUCCESS)
			{
			    /* set the interpreter result to error message */
			    gc_i_set_interp_to_trans_error(error_vector, retval,
							   interp);
			    /* free the error vector */
			    gc_t_free_result(error_vector);

			    break;
			}

			/* if present, ignore it and set the boolean argument */
			if (is_present == TRUE)
			{
			    *structured_result = TRUE;
			    continue;
			}
		    }

		    /* create the new vector */
		    vect_array[vect_count] = gc_i_strdup(argv[i]);

		    if (vect_array[vect_count] == NULL)
		    {
			retval = GC_I_MEMORY_INSUFFICIENT;

			/* set the interpreter result to error message */
			gc_i_set_interpreter_error(retval, NULL, interp);

			break;
		    }

		    ++vect_count;
		    continue;
		}

		/* not a switch, so break it into elements */
		if (Tcl_SplitList(interp, argv[i], &split_count, &split_args)
		    != TCL_OK)
		{
		    /* Invalid TCL list. The interpreter result will have */
		    /* been appropriately set by TCL_SplitList		  */
		    retval = GC_I_INVALID_ARGUMENT;
		    break;
		}

		/* run through the elements */
		for (j = 0; j < split_count; ++j)
		{
		    /* create the new vector */
		    vect_array[vect_count] = gc_i_strdup(split_args[j]);

		    if (vect_array[vect_count] == NULL)
		    {
			retval = GC_I_MEMORY_INSUFFICIENT;

			/* set the interpreter result to error message */
			gc_i_set_interpreter_error(retval, NULL, interp);

			break;
		    }
		    ++vect_count;
		}

		if (split_count > 0)
		{
		    free((char *) split_args);
		}

		if (retval != GC_SUCCESS)
		{
		    break;
		}
	    }
	}
    }

    if (retval != GC_SUCCESS)
    {
	if (vect_array != NULL)
	{
	    for (i = 0; i < vect_count; ++i)
	    {
		free(vect_array[i]);
	    }

	    free(vect_array);
	}
    }
    else
    {
	/* initialize the last vector to NULL */
	vect_array[vect_count] = NULL;

	*new_argc = vect_count;
	*new_argv = vect_array;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_strdup			   	   	   */
/* Description   : This function returns a copy of the input string*/
/*		   The area for the copied string is malloced by   */
/*		   this function.				   */
/*		   Note that the existing "strdup" function has    */
/*		   not been used because it is not part of POSIX   */
/*		   standard.   					   */
/* Return Value  : The copied string or NULL in case of malloc     */
/*		   failure					   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
char *gc_i_strdup(
    char *source)  /* IN - The input string to be copied */
{
char *copy;

    copy = (char *) malloc(sizeof(char) * (strlen(source) + 1));

    if (copy != NULL)
    {
	strcpy(copy, source);
    }

    return(copy);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_free_created_args	   	   	   */
/* Description   : This function is used to free the area allocated*/
/*		   for the character vector array by the 	   */
/*		   gc_i_repack_arguments function.		   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_i_free_created_args(
    char **vector)  /* IN - The error to be freed */
{
signed16 i;

    if (vector != NULL)
    {
	for (i = 0; vector[i] != NULL; ++i)
	{
	    free(vector[i]);
	}

	free(vector);
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_check_operation			   	   */
/* Description   : This function scans the operation table of an   */
/*		   object to determine if the user supplied        */
/*		   operation for a given object is valid.  It also */
/*		   checks if the operation is an unique abbreviated*/
/*		   form.    					   */
/*		   In case of errors, it sets the interpreter      */
/*		   result string with the error message.	   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_i_check_operation(
    gc_i_command_procedure *op_table,     /* IN  - The operation table	      */
    char	           *user_string,  /* IN  - The user supplied switch   */
    signed16		   *table_index,  /* OUT - Index into operation table */
    Tcl_Interp             *interp)       /* OUT - The Tcl-interpreter        */
{
signed16   retval;
signed16   match_count;
signed16   total_length;
signed16   reqd_index;
signed16   i;
char      *append_string;

    match_count = 0;
    total_length = 0;
    retval = GC_SUCCESS;

    if (user_string[0] != '\0')
    {
	/* locate the operation in the operation table */
	for (i = 0; op_table[i].name != NULL; ++i)
	{
	    /* if the user supplied string is an abbreviation or a */
	    /* full form of the operation defined in the table.    */
	    if (strstr(op_table[i].name, user_string) == op_table[i].name)
	    {
		reqd_index = i;

		++match_count;

		/* compute the length required.  Will be required later */
		/* in case of errors.  length is 2 more - one for end   */
		/* of string and another for a space between operations.*/
		total_length = total_length + 2 + strlen(op_table[i].name);
	    }
	}
    }

    if (match_count == 1)
    {
	/* user supplied operation string found in the table and is unique */
	*table_index = reqd_index;
    }
    else
    {
	if (match_count == 0)
	{
	    /* no match found in the operation table. */
	    /* must be an unknown switch or argument. */
	    if (*user_string == '-')
	    {
		retval = GC_I_MISSING_OPERATION;

		/* set the interpreter result */
		gc_i_set_interpreter_error(retval, NULL, interp);
	    }
	    else
	    {
		retval = GC_I_UNKNOWN_OPERATION;

		/* set the interpreter result */
		gc_i_set_interpreter_error(retval, user_string, interp);
	    }
	}
	else
	{
	    /* user supplied operation string is not unique */
	    retval = GC_I_AMBIGUOUS_OPERATION;

	    append_string = (char *) malloc(sizeof(char) *
					    (total_length + strlen(user_string)
					     + GC_I_MAX_ERR_STRING));
	    if (append_string == NULL)
	    {
		retval = GC_I_MEMORY_INSUFFICIENT;

		/* set the interpreter result */
		gc_i_set_interpreter_error(retval, NULL, interp);
	    }
	    else
	    {
		/* prepare the error string. The error string will */
		/* contain the user supplied string, followed by   */
		/* a list of valid "gdscp" operations.		   */
		sprintf(append_string, "\"%s\":", user_string);

		/* locate the operation in the operation table */
		for (i = 0; op_table[i].name != NULL; ++i)
		{
		    /* if the user supplied string is an abbreviation or a */
		    /* full form of the operation defined in the table.    */
		    if (strstr(op_table[i].name, user_string) == 
			op_table[i].name)
		    {
			strcat(append_string, " ");
			strcat(append_string, op_table[i].name);
		    }
		}

		/* set the interpreter result */
		gc_i_set_interpreter_error(retval, append_string, interp);

		/* now free the malloced string */
		free(append_string);
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_create_list_of_operations	   	   */
/* Description   : This function scans the operation table of an   */
/*		   object and creates a TCL list of the valid      */
/*		   operations encountered in the table.		   */
/*		   The TCL interpreter result is set to the	   */
/*		   created TCL list. In case of errors, the        */
/*		   TCL interpreter result is set to the error      */
/*		   message.					   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_i_create_list_of_operations(
    gc_i_command_procedure *op_table, /* IN  - The operation table */
    Tcl_Interp    	   *interp)   /* OUT - The Tcl-interpreter */
{
signed16  retval;
signed16  res_count;
char    **result_string;
char     *result_list;

    retval = GC_SUCCESS;
    res_count = 0;

    /* count the number of operations in the table */
    while (op_table[res_count].name != NULL)
    {
	++res_count;
    }

    result_string = (char **) malloc(sizeof(char *) * res_count);

    if (result_string == NULL)
    {
	retval = GC_I_MEMORY_INSUFFICIENT;
    }
    else
    {
	for (res_count = 0; op_table[res_count].name != NULL; ++res_count)
	{
	    result_string[res_count] = op_table[res_count].name;
	}

	/* build the result string */
	result_list = Tcl_Merge(res_count, result_string);

	if (result_list == NULL)
	{
	    retval = GC_I_MEMORY_INSUFFICIENT;
	}
	else
	{
	    /* set the interpreter result */
	    Tcl_SetResult(interp, result_list, TCL_DYNAMIC);
	}

	/* now free the malloced area */
	free(result_string);
    }

    if (retval != GC_SUCCESS)
    {
	/* set the interpreter result */
	gc_i_set_interpreter_error(retval, NULL, interp);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_set_interpreter_error   	   	   */
/* Description   : This function converts the error code received  */
/*		   from the gdscp interpreter to an error message  */
/*		   and sets the TCL interpreter result.            */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_i_set_interpreter_error(
    signed16	  retval,     /* IN  - The error code      	       */
    char         *append_str, /* IN  - String to be appended in result */
    Tcl_Interp   *interp)     /* OUT - The Tcl-interpreter   	       */
{
unsigned32  message_id;
char	   *message_string;

    switch(retval)
    {
	case (GC_I_MEMORY_INSUFFICIENT):
	   message_id = gc_t_memory_insufficient;
	   break;
	case (GC_I_UNKNOWN_OPTION):
	   message_id = gc_t_unknown_option;
	   break;
	case (GC_I_UNKNOWN_ARGUMENT):
	   message_id = gc_t_unknown_argument;
	   break;
	case (GC_I_INVALID_ARGUMENT):
	   message_id = gc_i_invalid_argument;
	   break;
	case (GC_I_INVALID_PARAMETER):
	   message_id = gc_t_invalid_parameter;
	   break;
	case (GC_I_UNKNOWN_OPERATION):
	   message_id = gc_i_unknown_operation;
	   break;
	case (GC_I_MISSING_OPERATION):
	   message_id = gc_i_missing_operation;
	   break;
	case (GC_I_TOO_MANY_ARGUMENTS):
	   message_id = gc_t_too_many_arguments;
	   break;
	case (GC_I_AMBIGUOUS_OPERATION):
	   message_id = gc_i_ambiguous_operation;
	   break;
	default:
	   break;
    }

    /* get the message string from the catalog */
    if (append_str != NULL)
    {
	message_string = (char *) dce_sprintf(message_id, append_str);
    }
    else
    {
	message_string = (char *) dce_sprintf(message_id);
    }

    if (message_string == NULL)
    {
	dce_fprintf(stderr, gc_t_memory_insufficient);
    }
    else
    {
	/* set the interpreter result */
	Tcl_SetResult(interp, message_string, TCL_DYNAMIC);
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_set_interp_to_trans_error   	   	   */
/* Description   : This function processes the error vector        */
/*		   received from the Translator and sets the       */
/*		   interpreter result to the error message         */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_i_set_interp_to_trans_error(
    gc_t_vector  *error_vector,  /* IN  - The Translator error vector      */
    signed16	  retval,        /* IN  - The return value from Translator */
    Tcl_Interp   *interp)        /* OUT - The Tcl-interpreter              */
{
    if (error_vector != NULL)
    {
	/* Set the interpreter result.				        */
	/* The free procedure is TCL_VOLATILE, because the error_vector */
	/* will be freed after this function returns.		        */
	Tcl_SetResult(interp, error_vector->value, TCL_VOLATILE);
    }
    else
    {
	/* must be a severe malloc failure, otherwise the error vector */
	/* returned by the Translator cannot be NULL. In case of       */
	/* serious malloc failures (in situations where there is no    */
	/* memory for the error message string) the gdscp Translator   */
	/* returns the error code with NULL error vector.	       */

	dce_fprintf(stderr, gc_t_memory_insufficient);
    }
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
