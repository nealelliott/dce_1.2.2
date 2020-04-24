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
 * $Log: gctargs.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:38  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:13  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:39  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:32  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:00  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:54:19  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:37  keutel
 * 	creation
 * 	[1994/03/21  12:56:02  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctargs.c					   */
/* Description : This file contains all the utility functions for  */
/*		 processing the input argument vectors to the      */
/*		 translator functions.				   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctargs.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:46 $";
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
#include <gcterr.h>
#include <gctargs.h>


/*-- Static Variable(s) -------------------------------------------*/
/* To store the list of valid switches for the "gdscp" operations  */
/* IMPORTANT NOTE: Make sure in this table that all the switches   */
/*		   for an operation are listed together.	   */
static struct { gc_t_op_type  operation;
		char	     *defined_switch;
	      } gc_t_switch_table[] =
{
    { GC_T_BIND,      GC_T_AUTHENTICATION_SWITCH },
    { GC_T_BIND,      GC_T_CACHE_SWITCH },
    { GC_T_BIND,      GC_T_DIRID_SWITCH },
    { GC_T_BIND,      GC_T_DSA_SWITCH },
    { GC_T_BIND,      GC_T_USER_NAME_SWITCH },
    { GC_T_BIND,      GC_T_PASSWD_SWITCH },
    { GC_T_BIND,      GC_T_PSAP_SWITCH },
    { GC_T_COMPARE,   GC_T_ATTR_INFO_SWITCH },
    { GC_T_CREATE,    GC_T_ATTR_INFO_SWITCH },
    { GC_T_LIST,      GC_T_STRUCTURED_RESULT_SWITCH },
    { GC_T_MODIFY,    GC_T_ADD_ATTR_SWITCH },
    { GC_T_MODIFY,    GC_T_MOD_ATTR_SWITCH },
    { GC_T_MODIFY,    GC_T_DEL_ATTR_SWITCH },
    { GC_T_MODIFY,    GC_T_MOD_RDN_SWITCH },
    { GC_T_READ,      GC_T_ATTR_INFO_SWITCH },
    { GC_T_READ,      GC_T_STRUCTURED_RESULT_SWITCH },
    { GC_T_READ,      GC_T_ATTR_TYPE_SWITCH },
    { GC_T_SEARCH,    GC_T_REQUEST_ALL_SWITCH },
    { GC_T_SEARCH,    GC_T_ATTR_INFO_SWITCH },
    { GC_T_SEARCH,    GC_T_BASE_OBJ_SWITCH },
    { GC_T_SEARCH,    GC_T_FILTER_SWITCH },
    { GC_T_SEARCH,    GC_T_NOALIASES_SWITCH },
    { GC_T_SEARCH,    GC_T_SING_LEVEL_SWITCH },
    { GC_T_SEARCH,    GC_T_STRUCTURED_RESULT_SWITCH },
    { GC_T_SEARCH,    GC_T_SUBTREE_SWITCH },
    { GC_T_SEARCH,    GC_T_ATTR_TYPE_SWITCH },
    { GC_T_MOD_DSC,   GC_T_AUTOMATIC_CONTINUATION_SWITCH },
    { GC_T_MOD_DSC,   GC_T_CACHE_CLASS_SWITCH },
    { GC_T_MOD_DSC,   GC_T_CHAINING_PROHIBITED_SWITCH },
    { GC_T_MOD_DSC,   GC_T_DEFAULT_SWITCH },
    { GC_T_MOD_DSC,   GC_T_DONT_DEREF_ALIAS_SWITCH },
    { GC_T_MOD_DSC,   GC_T_DONT_USE_COPY_SWITCH },
    { GC_T_MOD_DSC,   GC_T_USE_CACHE_SWITCH },
    { GC_T_MOD_DSC,   GC_T_CACHE_FIRST_SWITCH },
    { GC_T_MOD_DSC,   GC_T_LOCAL_SCOPE_SWITCH },
    { GC_T_MOD_DSC,   GC_T_PREFER_ADMIN_FUNCTIONS_SWITCH },
    { GC_T_MOD_DSC,   GC_T_PREF_CHAIN_SWITCH },
    { GC_T_MOD_DSC,   GC_T_PRIORITY_SWITCH },
    { GC_T_MOD_DSC,   GC_T_SCOPE_OF_REFERRAL_SWITCH },
    { GC_T_MOD_DSC,   GC_T_SIZE_LIMIT_SWITCH },
    { GC_T_MOD_DSC,   GC_T_TIME_LIMIT_SWITCH },
    { GC_T_MOD_DSC,   GC_T_USE_DSA_SWITCH },
    { GC_T_SHOW_DSC,  GC_T_STRUCTURED_RESULT_SWITCH },
    { GC_T_SHOW_ABBR, GC_T_STRUCTURED_RESULT_SWITCH }
};


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_check_args			   	   */
/* Description   : This function is used to scan the input vector  */
/*		   and validate the arguments based on the type of */
/*		   operation being performed. It also returns the  */
/*		   count of attribute information arguments in the */
/*		   input vector.		   		   */
/*		   Note that this function is only used for the    */
/*		   "compare", "create", "list", "moveto", "read"   */
/*		   and "remove" operations.			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_check_args(
    gc_t_op_type   op_type,         /* IN  - Type of operation               */
    char         **in_vect,         /* IN  - Array of input vectors          */
    signed16       vect_count,      /* IN  - Count of input vectors          */
    signed16	  *attr_info_count, /* OUT - Count of attr.info arguments    */
    gc_t_vector  **error_vect)      /* OUT - Error message in case of errors */
{
signed16  retval;
signed16  a_info_count, a_type_count;
signed16  i;
boolean   is_present;

    i = 0;
    a_info_count = a_type_count = 0;
    *error_vect = NULL;
    retval = GC_SUCCESS;

    if (vect_count > 0 && in_vect == NULL)
    {
	retval = GC_T_INVALID_PARAMETER;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, error_vect);
    }
    else
    {
	i = 0;

	/* check if the object name is specified in the first input vector */
	if (i < vect_count && in_vect[0] != NULL && in_vect[0][0] != '-')
	{
	    /* skip past the object name */
	    ++i;
	}
	else
	{
	    /* for these operations, object name is mandatory */
	    switch (op_type)
	    {
		case (GC_T_CREATE):
		case (GC_T_MOVETO):
		case (GC_T_REMOVE):
		    retval = GC_T_MISSING_OBJECT_NAME;
		    break;
		default:
		    break;
	    }
	}

	if (retval == GC_SUCCESS)
	{
	    for (; i < vect_count && in_vect[i] != NULL; ++i)
	    {
		/* if attribute info switch specified */
		retval = gc_t_check_switch(op_type, in_vect[i], 
					   GC_T_ATTR_INFO_SWITCH,
					   &is_present, error_vect);

		if (retval != GC_SUCCESS)
		{
		    break;
		}

		if (is_present == TRUE)
		{
		    /* if attribute type only switch also specified */
		    if (a_type_count != 0)
		    {
			retval = GC_T_CONFLICTING_OPTION;
			break;
		    }

		    /* The attribute information switch is valid only for the */
		    /* following operations.				      */
		    switch (op_type)
		    {
			case (GC_T_CREATE):
			case (GC_T_READ):
			case (GC_T_COMPARE):
			    break;
			default:
			    retval = GC_T_UNKNOWN_OPTION;
			    break;
		    }

		    if (retval != GC_SUCCESS)
			break;

		    /* check if this switch is repeated */
		    if (a_info_count != 0)
		    {
			retval = GC_T_CONFLICTING_OPTION;
			break;
		    }

		    /* skip past the switch */
		    ++i;

		    /* check if it is followed by attribute information */
		    if (i >= vect_count || in_vect[i] == NULL ||
			in_vect[i][0] == '-')
		    {
			retval = GC_T_MISSING_ATTRIBUTE_INFO;
			break;
		    }

		    /* for this operation only one attribute information */
		    /* argument can be specified.			 */
		    if (op_type == GC_T_COMPARE)
		    {
			++a_info_count;

			/* skip past the attribute information */
			++i;

			if (i < vect_count && in_vect[i] != NULL)
			{
			    if (in_vect[i][0] != '-')
			    {
				retval = GC_T_TOO_MANY_VALUES;
				break;
			    }
			}

			--i;
			continue;
		    }

		    /* run through the attribute information arguments */
		    for (; i < vect_count && in_vect[i] != NULL; ++i)
		    {
			if (in_vect[i][0] == '-')
			{
			    --i;
			    break;
			}
			++a_info_count;
		    }

		    continue;
		}

		/* if attribute type only switch specified */
		retval = gc_t_check_switch(op_type, in_vect[i], 
					   GC_T_ATTR_TYPE_SWITCH,
					   &is_present, error_vect);

		if (retval != GC_SUCCESS)
		{
		    break;
		}

		if (is_present == TRUE)
		{
		    /* The attribute type only switch is valid only for */
		    /* these operations.   			    	*/
		    switch (op_type)
		    {
			case (GC_T_READ):
			    break;
			default:
			    retval = GC_T_UNKNOWN_OPTION;
			    break;
		    }

		    if (retval != GC_SUCCESS)
			break;

		    /* if attribute info switch also specified */
		    if (a_info_count != 0)
		    {
			retval = GC_T_CONFLICTING_OPTION;
			break;
		    }

		    ++a_type_count;
		    continue;
		}

		/* must be an unknown switch or argument */
		if (in_vect[i][0] == '-')
		{
		    retval = GC_T_UNKNOWN_OPTION;
		}
		else
		{
		    if (op_type == GC_T_REMOVE || op_type == GC_T_LIST ||
			op_type == GC_T_MOVETO)
		    {
			retval = GC_T_TOO_MANY_VALUES;
		    }
		    else
		    {
			retval = GC_T_UNKNOWN_ARGUMENT;
		    }
		}
		break;
	    }
	}
    }

    if (retval != GC_SUCCESS)
    {
	if (*error_vect == NULL)
	{
	    /* create the error vector */
	    if (i >= vect_count)
	    {
		gc_t_create_gct_arg_error_vector(retval, NULL, error_vect);
	    }
	    else
	    {
		gc_t_create_gct_arg_error_vector(retval, in_vect[i], 
						 error_vect);
	    }
	}
    }
    else
    {
	if (a_info_count == 0)
	{
	    switch (op_type)
	    {
		/* for this operation, attribute info is mandatory */
		case (GC_T_COMPARE):
		    retval = GC_T_MISSING_ATTRIBUTE_INFO;

		    /* create the error vector */
		    gc_t_create_gct_arg_error_vector(retval, NULL, error_vect);
		    break;
		default:
		    break;
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	if (attr_info_count != NULL)
	    *attr_info_count = a_info_count;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_check_switch			   	   */
/* Description   : This function is used to scan the switch table  */
/*		   to determine if the user supplied switch is     */
/*		   valid for a given operation.  It also checks    */
/*		   if the switch is an unique abbreviated form.    */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_check_switch(
    gc_t_vect_type  operation,     /* IN  - The type of gdscp operation       */
    char	   *user_switch,   /* IN  - The user supplied switch          */
    char	   *defined_switch,/* IN  - The Translator defined switch     */
    boolean        *is_present,    /* OUT - Indicates user switch is OK or not*/
    gc_t_vector   **error_vect)    /* OUT - Error vector in case of errors    */
{
signed16  switch_count;
signed16  table_size;
signed16  retval;
signed16  total_length;
signed16  i;
char     *append_string;

    switch_count = 0;
    total_length = 0;
    retval = GC_SUCCESS;

    *is_present = FALSE;

    /* if the user supplied switch is an abbreviation or a full */
    /* form of the translator defined switch.		        */
    if (user_switch[0] == '-' && user_switch[1] != '\0' &&
	strstr(defined_switch, user_switch) == defined_switch)
    {
	table_size = (signed16) (sizeof(gc_t_switch_table) /
				 sizeof(gc_t_switch_table[0]));

	/* locate the operation in the switch table */
	for (i = 0; i < table_size; ++i)
	{
	    if (gc_t_switch_table[i].operation == operation)
	    {
		break;
	    }
	}

	/* operation found in the table */
	if (i < table_size)
	{
	    while (i < table_size && 
		   gc_t_switch_table[i].operation == operation)
	    {
		/* if the user supplied switch is an abbreviation or a */
		/* full form of the switch defined in the table.       */
		if (strstr(gc_t_switch_table[i].defined_switch, user_switch)
		    == gc_t_switch_table[i].defined_switch)
		{
		    ++switch_count;

		    /* compute the length required.  Will be required later */
		    /* in case of errors.  length is 2 more - one for end   */
		    /* of string and another for a space between switches.  */
		    total_length = total_length + 2 +
				   strlen(gc_t_switch_table[i].defined_switch);
		}

		++i;
	    }
	}
    }

    if (switch_count != 0)
    {
	if (switch_count == 1)
	{
	    /* user supplied switch is unique for this operation */
	    *is_present = TRUE;
	}
	else
	{
	    /* user supplied switch is not unique for this operation */
	    retval = GC_T_AMBIGUOUS_OPTION;

	    append_string = (char *) malloc(sizeof(char) *
					    (total_length + strlen(user_switch)
					     + GC_T_MAX_VAR));
	    if (append_string == NULL)
	    {
		retval = GC_T_MEMORY_INSUFFICIENT;
		gc_t_create_gct_error_vector(retval, NULL, error_vect);
	    }
	    else
	    {
		/* prepare the error string. The error string will */
		/* contain the user supplied switch, followed by   */
		/* a list of valid "gdscp" switches.		   */
		sprintf(append_string, "\"%s\":", user_switch);

		/* locate the operation in the switch table */
		for (i = 0; i < table_size; ++i)
		{
		    if (gc_t_switch_table[i].operation == operation)
		    {
			break;
		    }
		}

		while (i < table_size && 
		       gc_t_switch_table[i].operation == operation)
		{
		    /* if user supplied switch is an abbreviation or a */
		    /* full form of the switch defined in the table.   */
		    if (strstr(gc_t_switch_table[i].defined_switch, 
			       user_switch) == 
					gc_t_switch_table[i].defined_switch)
		    {
			strcat(append_string, " ");
			strcat(append_string, 
			       gc_t_switch_table[i].defined_switch);
		    }

		    ++i;
		}

		/* create the error vector */
		gc_t_create_gct_error_vector(retval, append_string, error_vect);

		/* now free the area */
		free(append_string);
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
