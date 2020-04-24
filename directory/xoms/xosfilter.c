/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosfilter.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:24  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:52:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:27:01  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:23:05  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:59  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:36  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:17:22  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:35  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:32:07  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  11:00:19  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosfilter.c					   */
/* Description : This file contains all the routines for the       */
/*		 string to filter conversion.			   */
/* Date	       : July 1, 1993					   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xosfilter.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:24 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <xom.h>
#include <xds.h>

#include <xoi.h>
#include <xomext.h>

#include <xosstruct.h>
#include <xosutil.h>
#include <xosattr.h>
#include <xosclass.h>
#include <xosfilter.h>

#ifdef XOS_DEBUG
#include <xosdebug.h>
#endif

/*-- Local Defines(s) ---------------------------------------------*/
#define STACK_SIZE_INITIAL   10  /* Initial size of the stack 		 */
#define STACK_SIZE_INCREMENT  5  /* Incremental size of stack, when full */

/* Import the necessary Object Identifier constants */
OM_IMPORT(dsP_DS_C_FILTER)
OM_IMPORT(dsP_DS_C_FILTER_ITEM)
OM_IMPORT(dsP_DS_C_DS_DN)

/*-- Static Function(s) -------------------------------------------*/
static OM_return_code xos__get_token(
    char       *str,	    /* IN  - The input string to be scanned 	      */
    OM_uint32   len,  	    /* IN  - Length of the input string		      */
    xos_token  *token_type, /* OUT - Type of token encountered		      */
    char       *abbrv,	    /* OUT - Abbreviation str. if token is XOS_F_ABBRV*/
    OM_uint32  *len_read,   /* OUT - Number of characters read in input string*/
    OM_integer *error_pos,  /* OUT - Error position in case of errors	      */
    OM_integer *error_type);/* OUT - Type of error in case of errors	      */

static OM_return_code xos__perform_sanity_checks(
    xos_token	     input_token,  /* IN  - The read input token      	    */
    xos_stack_token  stack_token,  /* IN  - The token on top of stack 	    */
    OM_integer	    *error_type);  /* OUT - Type of error in case of errors */

static OM_return_code xos__create_filter_item(
    char        *abbrv,	       /* IN  - The attribute abbreviation string     */
    char        *str,	       /* IN  - The input string 		      */
    OM_uint32    len,	       /* IN  - Length of the input string	      */
    char        *val_mem_top,  /* IN  - Memory heap to store attribute values */
    OM_boolean   local_strings,/* IN  - To specify local strings      	      */
    OM_object   *filter_item,  /* OUT - The created filter item		      */
    OM_boolean  *negate_item,  /* OUT - Indicates if item is to be negated    */
    OM_uint32   *len_read,     /* OUT - Number of characters read in input str*/
    OM_integer  *error_pos,    /* OUT - Error position in case of errors      */
    OM_integer	*error_type);  /* OUT - Type of errors in case of errors      */

static OM_return_code xos__get_filter_item_length(
    char       *str,	     /* IN  - The input string			*/
    OM_uint32   len,	     /* IN  - Length of the input string	*/
    OM_uint    *wild_count,  /* OUT - Count of wild characters in input */
    OM_uint32  *item_len,    /* OUT - Length of attribute value		*/
    OM_integer *error_pos,   /* OUT - Error position in case of errors	*/
    OM_integer *error_type); /* OUT - Type of error in case of errors	*/

static OM_return_code xos__create_item_ds_present(
    OM_string  *objid,	     /* IN  - Object Identifier of the attribute */
    OM_object  *filter_item, /* OUT - The created filter object		 */
    OM_integer *error_type); /* OUT - Type of error in case of errors    */

static OM_return_code xos__create_substrings_obj(
    OM_string  *objid,	       /* IN  - Attribute Object Identifier           */
    OM_syntax   omsyntax,      /* IN  - Syntax of the Attribute		      */
    char       *str,	       /* IN  - Attribute Value string		      */
    OM_uint32   len,	       /* IN  - Length of Value string		      */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_uint     wild_count,    /* IN  - Count of wild characters in input     */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *filter_item,   /* OUT - The created filter item		      */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type);   /* OUT - Type of error in case of errors	      */

static OM_return_code xos__get_substring_length(
    char       *str,	     /* IN  - The input string			  */
    OM_uint32   len,   	     /* IN  - Length of the input string	  */
    OM_uint32  *substr_len,  /* OUT - Length of the encountered substring */
    OM_integer *error_pos,   /* OUT - Error position in case of errors	  */
    OM_integer *error_type); /* OUT - Type of errors in case of errors	  */

static OM_return_code xos__get_substring_value(
    char 	*in_str,      /* IN  - The input string		        */
    OM_uint32 	 in_len,      /* IN  - Length of the input string       */
    char 	*out_str,     /* OUT - The converted string	        */
    OM_uint32 	*out_len,     /* OUT - Length of the output string      */
    OM_integer 	*error_pos,   /* OUT - Error position in case of errors */
    OM_integer 	*error_type); /* OUT - Type of error in case of errors  */

static OM_return_code xos__create_single_filter(
    OM_type         object_type, /* IN  - Object type (Filter item or Filter) */
    OM_enumeration  filter_enum, /* IN  - DS_FILTER_TYPE		      */
    OM_object  	    object,      /* IN  - DS_C_FILTER or DS_C_FILTER_ITEM     */
    OM_object      *filter); 	 /* OUT - The created filter object   	      */

static OM_return_code xos__rearrange_stack(
    xos_token	    read_token,   /* IN  - The token read	   	   */
    xos_stack_item *stack,        /* I/O - The stack			   */
    OM_sint	   *stack_index,  /* I/O - Index to top of stack	   */
    OM_integer	   *error_type);  /* OUT - Type of error in case of errors */

static OM_sint xos__compare_priority(
    xos_token       read_token,   /* IN - The token read  */
    xos_stack_token stack_token); /* IN - The stack token */

static void xos__make_values_adjacent(
    OM_object filter); /* I/O - The filter to be rearranged */

static int xos__sort_filter(
    const void *desc1,  /* IN - The first descriptor to be compared  */
    const void *desc2); /* IN - The second descriptor to be compared */

static void xos__free_filter_item_object(
    OM_object filter_item);  /* IN - The filter item object to be freed */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_to_public_filter_object			   */
/* Description   : This function is used to convert a filter in the*/
/*		   string format to a public object.		   */
/*		   The input string is of the form		   */
/*	      (CN~=schmid) && ((OCL=ORP) || (OCL=REP)) && !(SN=ron)*/ 
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_return_code xos_to_public_filter_object(
    OM_string  *string,        /* IN  - The string to be converted 	      */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *filter,        /* OUT - The converted filter object 	      */
    OM_integer *error_pos,     /* OUT - Error Position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors	      */
{
OM_return_code	retval;
OM_integer	retval_xoi;
OM_boolean	negate_item;
OM_uint32       len, len_read, curr_index;
OM_object	filter_obj;
OM_object	filter_item;
OM_syntax	omsyntax;
OM_uint		stack_size;
OM_sint	        stack_index;
xos_stack_item *stack;
xos_stack_item *p, completed_item;
xos_token	token_type;
char            abbrv[XOS_MAX_OBJID + 1];
char	       *str;

#ifdef XOS_DEBUG
    xos_debug_string("xos_to_public_filter_object : ENTRY", 0, 1);
    xos_debug_string("Input String:", 0, 0);
    xos_debug_string((char *)(string->elements), string->length, 0);
#endif

    len = string->length;
    str = (char *) (string->elements);

    curr_index = 0;
    /* skip past white spaces */
    while (curr_index < len && isspace(str[curr_index]))
	++curr_index;

    if (curr_index == len)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_ATTR_INFO;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    len -= curr_index;

    /* allocate the memory for stack */
    stack_size = STACK_SIZE_INITIAL;
    stack = (xos_stack_item *) malloc(sizeof(xos_stack_item) * stack_size);

    if (stack == NULL)
    {
	*error_type = OMX_MEMORY_INSUFFICIENT;
	return(OM_MEMORY_INSUFFICIENT);
    }

    stack_index = 0;

    stack[stack_index].token = XOS_S_START;
    ++stack_index;

    retval = OM_SUCCESS;
    while (retval == OM_SUCCESS)
    {
#ifdef XOS_DEBUG
    xos_debug_stack(stack, stack_index);
#endif

	retval = xos__get_token(&str[curr_index], len, &token_type, abbrv, 
				&len_read, error_pos, error_type);
	if (retval != OM_SUCCESS)
	{
	    *error_pos += curr_index;
	    break;
	}

	retval = xos__perform_sanity_checks(token_type, 
					    stack[stack_index - 1].token, 
					    error_type);
        if (retval != OM_SUCCESS)
	{
	    /* error position would have been properly set during the call */
	    /* to xos__get_token.				 	   */
	    *error_pos += curr_index;
	    break;
	}

	if (token_type == XOS_F_ABBRV)
	{
	    /* validate the abbreviation */
	    retval_xoi = xoi_get_attr_details(abbrv, &omsyntax, NULL, NULL,
					      NULL, NULL);
	    if (retval_xoi != XOI_SUCCESS)
	    {
		/* error position would have been properly set during the */
		/* call to xos__get_token.				  */
		*error_pos += curr_index;
		*error_type = xos_map_xoi_error(retval_xoi);
		retval = OM_WRONG_VALUE_MAKEUP;
		break;
	    }
	}

	/* skip past the read length */
	len -= len_read;
	curr_index += len_read;

	switch(token_type)
	{
	    case (XOS_F_ABBRV):
		retval = xos__create_filter_item(abbrv, &str[curr_index], len, 
						 val_mem_top, local_strings,
						 &filter_item, &negate_item, 
						 &len_read, error_pos, 
						 error_type);
		if (retval != OM_SUCCESS)
		{
		    *error_pos += curr_index;
		    break;
		}

		/* skip past the read length */
		len -= len_read;
		curr_index += len_read;

		/* move to the next available memory block */
		val_mem_top += len_read;

		/* if there is a NOT operator on stack or if the filter */
		/* item is to be negated, negate the filter item and    */
		/* create a filter.		   			*/
		if (stack[stack_index - 1].token == XOS_S_NOT ||
		    negate_item == OM_TRUE)
		{
		    retval = xos__create_single_filter(DS_FILTER_ITEMS, DS_NOT,
						       filter_item,&filter_obj);
		    if (retval != OM_SUCCESS)
		    {
			*error_pos = curr_index;
			break;
		    }

		    /* pop the NOT from the stack */
		    if (stack[stack_index - 1].token == XOS_S_NOT)
			--stack_index;

		    /* push the filter on the stack */
		    stack[stack_index].token = XOS_S_FILTER;
		    stack[stack_index].object = filter_obj;
		    ++stack_index;
		}
		else
		{
		    /* push the filter item on the stack */
		    stack[stack_index].token = XOS_S_FILTER_ITEM;
		    stack[stack_index].object = filter_item;
		    ++stack_index;
		}

		break;

	    case (XOS_F_BRACKET_CLOSE):
	    case (XOS_F_AND):
	    case (XOS_F_OR):
	    case (XOS_F_EOS):
#ifdef XOS_DEBUG
    xos_debug_string("Rearranging Stack", 0, 0);
#endif
		/* rearrange the stack based on the priorities of the token */
		retval = xos__rearrange_stack(token_type, stack, &stack_index,
					      error_type);
		if (retval != OM_SUCCESS)
		    *error_pos = curr_index;

		/* after rearranging the stack, check if end of input */
		if (token_type == XOS_F_EOS)
		{
		    completed_item.token = stack[stack_index - 1].token;
		    completed_item.object = stack[stack_index - 1].object;

		    /* pop the filter/filter item and XOS_S_START from stack */
		    stack_index -= 2;
		}
		break;

	    case (XOS_F_BRACKET_OPEN):
		/* just push it on the stack */
		stack[stack_index].token = XOS_S_BRACKET_OPEN;
		++stack_index;
		break;

	    case (XOS_F_NOT):
		/* if there is already a NOT operator on stack, remove both */
		if (stack[stack_index - 1].token == XOS_S_NOT)
		{
		    --stack_index;
		}
		else
		{
		    stack[stack_index].token = XOS_S_NOT;
		    ++stack_index;
		}
		break;
	    default:
		retval = OM_WRONG_VALUE_MAKEUP;
		*error_pos = curr_index + 1;
		*error_type = OMX_UNKNOWN_KEYWORD;
		break;
	}

	if (retval != OM_SUCCESS || stack_index <= 0)
	    break;

	/* if stack is full, allocate more area */
	if (stack_index + 1 >= stack_size) 
	{
	    stack_size += STACK_SIZE_INCREMENT;
	    p = (xos_stack_item *) realloc(stack, sizeof(xos_stack_item) * 
						  stack_size);
	    if (p == NULL)
	    {
		*error_type = OMX_MEMORY_INSUFFICIENT;
		retval = OM_MEMORY_INSUFFICIENT;
		break;
	    }
	    stack = p;
	}
    }

    if (retval == OM_SUCCESS)
    {
	if (completed_item.token == XOS_S_FILTER_ITEM)
	{
	    retval = xos__create_single_filter(DS_FILTER_ITEMS, DS_AND,
					       completed_item.object, 
					       &filter_obj);
	}
	else
	{
	    filter_obj = completed_item.object;
	}
    }

    if (retval != OM_SUCCESS)
    {
	while (stack_index > 0)
	{
	    if (stack[stack_index - 1].token == XOS_S_FILTER)
		xos_free_filter_object(stack[stack_index - 1].object);

	    if (stack[stack_index - 1].token == XOS_S_FILTER_ITEM)
		xos__free_filter_item_object(stack[stack_index - 1].object);

	    --stack_index;
	}
    }

    free(stack);

    if (retval == OM_SUCCESS)
	*filter = filter_obj;

#ifdef XOS_DEBUG
    xos_debug_string("xos_to_public_filter_object : EXIT", 0, -1);
#endif

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__get_token			   	   */
/* Description   : This function is used to scan the input string  */
/*		   and return the type of token encountered.	   */
/*		   Additionally, if the token is an abbreviation   */
/*		   it converts it to its object identifier and     */
/*		   if it is a structured attribute, it also returns*/
/*		   its class identifier.			   */
/*		   The input string is of the form		   */
/*	      (CN~=schmid) && ((OCL=ORP) || (OCL=REP)) && !(SN=ron)*/ 
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__get_token(
    char       *str,	    /* IN  - The input string to be scanned 	      */
    OM_uint32   len,  	    /* IN  - Length of the input string		      */
    xos_token  *token_type, /* OUT - Type of token encountered		      */
    char       *abbrv,	    /* OUT - Abbreviation str. if token is XOS_F_ABBRV*/
    OM_uint32  *len_read,   /* OUT - Number of characters read in input string*/
    OM_integer *error_pos,  /* OUT - Error position in case of errors	      */
    OM_integer *error_type) /* OUT - Type of error in case of errors	      */
{
OM_return_code	retval;
OM_uint32  	curr_index, i;

#ifdef XOS_DEBUG
char		debug_msg[256];
#endif

#ifdef XOS_DEBUG
    xos_debug_string("xos__get_token : ENTRY", 0, 1);
    xos_debug_string("Input String:", 0, 0);
    xos_debug_string(str, len, 0);
#endif

    curr_index = 0;
    i = 0;

    /* skip past white spaces */
    while (curr_index < len && isspace(str[curr_index]))
	++curr_index;

    /* set the error position here so that in case of errors much later */
    /* like unknown abbreviation, etc. it can be used.			*/
    *error_pos = curr_index + 1;
    retval = OM_SUCCESS;

    if (curr_index == len)
    {
	*len_read = curr_index;
	*token_type = XOS_F_EOS;
    }
    else
    {
	switch (str[curr_index])
	{
	    case (XOS_BRACKET_OPEN_OP):
		*token_type = XOS_F_BRACKET_OPEN;
		break;
	    case (XOS_BRACKET_CLOSE_OP):
		*token_type = XOS_F_BRACKET_CLOSE;
		break;
	    case (XOS_NOT_OP):
		*token_type = XOS_F_NOT;
		break;
	    case (XOS_AND_OP):
		/* check if it is followed by the XOS_AND_OP character */
		if (curr_index + 1 >= len ||
		    str[curr_index + 1] != XOS_AND_OP)
		{
		    *error_pos = curr_index + 1;
		    *error_type = OMX_UNKNOWN_KEYWORD;
		    retval = OM_WRONG_VALUE_MAKEUP;
		    break;
		}
		++curr_index;
		*token_type = XOS_F_AND;
		break;
	    case (XOS_OR_OP):
		/* check if it is followed by the XOS_OR_OP character */
		if (curr_index + 1 >= len ||
		    str[curr_index + 1] != XOS_OR_OP)
		{
		    *error_pos = curr_index + 1;
		    *error_type = OMX_UNKNOWN_KEYWORD;
		    retval = OM_WRONG_VALUE_MAKEUP;
		    break;
		}
		++curr_index;
		*token_type = XOS_F_OR;
		break;
	    default:
		/* must be attribute abbreviation or object identifier in */
		/* dotted representation.				  */
		while (curr_index < len && i < XOS_MAX_OBJID)
		{
		    if (!(isalnum(str[curr_index]) || str[curr_index] == '.'))
			break;

		    abbrv[i] = str[curr_index];
		    ++i;
		    ++curr_index;
		}
		if (i == 0 || i == XOS_MAX_OBJID)
		{
		    *error_pos = curr_index - i + 1;
		    *error_type = OMX_UNKNOWN_KEYWORD;
		    retval = OM_WRONG_VALUE_MAKEUP;
		    break;
		}

		/* Append the End of string character to the abbreviation */
		abbrv[i] = 0;

		*token_type = XOS_F_ABBRV;
		--curr_index;
		break;
	}
	*len_read = curr_index + 1;
    }

#ifdef XOS_DEBUG
    xos_debug_read_token(*token_type);
    if (*token_type == XOS_F_ABBRV)
    {
	sprintf(debug_msg, "Abbreviation = <%s>", abbrv);
	xos_debug_string(debug_msg, 0, 0);
    }
    sprintf(debug_msg, "Length Read = <%d>", *len_read);
    xos_debug_string(debug_msg, 0, 0);
    xos_debug_string("xos__get_token : EXIT", 0, -1);
#endif

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__perform_sanity_checks		   	   */
/* Description   : This function is used to perform sanity checks  */
/*		   on the read token by comparing it with the      */
/*		   current stack token.				   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__perform_sanity_checks(
    xos_token	     input_token,  /* IN  - The read input token      	    */
    xos_stack_token  stack_token,  /* IN  - The token on top of stack 	    */
    OM_integer	    *error_type)   /* OUT - Type of error in case of errors */
{

    *error_type = OMX_SUCCESS;

    switch (input_token)
    {
	case (XOS_F_ABBRV):
	case (XOS_F_BRACKET_OPEN):
	case (XOS_F_NOT):
	    if (stack_token == XOS_S_FILTER_ITEM || 
		stack_token == XOS_S_FILTER)
	    {
		*error_type = OMX_MISSING_COMPOUND_OP;
	    }
	    break;
	case (XOS_F_AND):
	case (XOS_F_OR):
	    if (!(stack_token == XOS_S_FILTER_ITEM || 
		  stack_token == XOS_S_FILTER))
	    {
		*error_type = OMX_MISSING_ABBRV;
	    }
	    break;
	case (XOS_F_BRACKET_CLOSE):
	    if (stack_token == XOS_S_START)
	    {
		*error_type = OMX_MISMATCHED_BRACKETS;
		break;
	    }
	    if (stack_token == XOS_S_BRACKET_OPEN)
	    {
		*error_type = OMX_MISSING_ATTR_INFO;
		break;
	    }
	    if (!(stack_token == XOS_S_FILTER_ITEM || 
		  stack_token == XOS_S_FILTER))
	    {
		*error_type = OMX_MISSING_ABBRV;
		break;
	    }
	    break;
	case (XOS_F_EOS):
	    if (stack_token == XOS_S_START)
	    {
		*error_type = OMX_MISSING_ATTR_INFO;
		break;
	    }
	    if (stack_token == XOS_S_BRACKET_OPEN)
	    {
		*error_type = OMX_MISMATCHED_BRACKETS;
		break;
	    }
	    if (!(stack_token == XOS_S_FILTER_ITEM || 
		  stack_token == XOS_S_FILTER))
	    {
		*error_type = OMX_MISSING_ABBRV;
		break;
	    }
	    break;
	default:
	    *error_type = OMX_UNKNOWN_KEYWORD;
	    break;
    }

    if (*error_type != OMX_SUCCESS)
	return(OM_WRONG_VALUE_MAKEUP);

    return(OM_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_filter_item	   	   	   */
/* Description   : This function scans the input string and creates*/
/*		   a DS_C_FILTER_ITEM object for the first value   */
/*		   encountered in the input string.		   */
/*		   The input string is of the form		   */
/*	       ~=schmid) && ((OCL=ORP) || (OCL=REP)) && !(SN=ron)  */ 
/*		   In this example the filter item is made for the */
/*		   string "~=schmid".				   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_filter_item(
    char        *abbrv,	       /* IN  - The attribute abbreviation string     */
    char        *str,	       /* IN  - The input string 		      */
    OM_uint32    len,	       /* IN  - Length of the input string	      */
    char        *val_mem_top,  /* IN  - Memory heap to store attribute values */
    OM_boolean   local_strings,/* IN  - To specify local strings      	      */
    OM_object   *filter_item,  /* OUT - The created filter item		      */
    OM_boolean  *negate_item,  /* OUT - Indicates if item is to be negated    */
    OM_uint32   *len_read,     /* OUT - Number of characters read in input str*/
    OM_integer  *error_pos,    /* OUT - Error position in case of errors      */
    OM_integer	*error_type)   /* OUT - Type of errors in case of errors      */
{
OM_return_code  retval;
OM_integer	retval_xoi;
OM_object	object;
OM_uint32	item_len, curr_index;
OM_syntax	omsyntax;
OM_string	objid;
OM_string	omclass;
OM_enumeration	item_type;
OM_uint		wild_count;
OM_uint		i = 0;

#ifdef XOS_DEBUG
char  		debug_msg[256];
#endif

#ifdef XOS_DEBUG
    xos_debug_string("xos__create_filter_item : ENTRY", 0, 1);
    xos_debug_string("Input String :", 0, 0);
    xos_debug_string(str, len, 0);
#endif

    *negate_item = OM_FALSE;

    /* get the attribute details from the abbreviation */
    retval_xoi = xoi_get_attr_details(abbrv, &omsyntax, 
				      &(objid.elements), &(objid.length), 
				      &(omclass.elements), &(omclass.length));

    if (retval_xoi != XOI_SUCCESS)
    {
	*error_pos = 0;
	*error_type = xos_map_xoi_error(retval_xoi);
	return(OM_WRONG_VALUE_MAKEUP);
    }

    curr_index = 0;
    /* skip past white spaces */
    while (curr_index < len && isspace(str[curr_index]))
	++curr_index;

    retval = OM_SUCCESS;

    /* if no attribute value is specified then request is only for DS_PRESENT */
    if (curr_index >= len)
    {
	item_type = DS_PRESENT;
    }
    else
    {
	switch (str[curr_index])
	{
	    case (XOS_APPROX_OP):
		/* check if it is followed by XOS_ATTR_EQUALITY_OP */
		if (curr_index + 1 >= len || 
		    str[curr_index + 1] != XOS_ATTR_EQUALITY_OP) 
		{
		    *error_pos = curr_index + 2;
		    *error_type = OMX_MISSING_OPERATOR;
		    retval = OM_WRONG_VALUE_MAKEUP;
		    break;
		}
		curr_index += 2;
		item_type = DS_APPROXIMATE_MATCH;
		break;
	    case (XOS_ATTR_EQUALITY_OP):
		item_type = DS_EQUALITY;
		++curr_index;
		break;
	    case (XOS_GREATER_OP):
		/* check if it is followed by XOS_ATTR_EQUALITY_OP */
		if (curr_index + 1 < len &&
		    str[curr_index + 1] == XOS_ATTR_EQUALITY_OP)
		{
		    item_type = DS_GREATER_OR_EQUAL;
		    ++curr_index;
		}
		else
		{
		    /* only GREATER THAN specified. This is similar to */
		    /* to NOT (LESS OR EQUAL TO)		       */
		    *negate_item = OM_TRUE;
		    item_type = DS_LESS_OR_EQUAL;
		}
		++curr_index;
		break;
	    case (XOS_LESSER_OP):
		/* check if it is followed by XOS_ATTR_EQUALITY_OP */
		if (curr_index + 1 < len &&
		    str[curr_index + 1] == XOS_ATTR_EQUALITY_OP)
		{
		    item_type = DS_LESS_OR_EQUAL;
		    ++curr_index;
		}
		else
		{
		    /* only LESSER THAN specified. This is similar to */
		    /* to NOT (GREATER OR EQUAL TO)	              */
		    *negate_item = OM_TRUE;
		    item_type = DS_GREATER_OR_EQUAL;
		}
		++curr_index;
		break;
	    case (XOS_AND_OP):
	    case (XOS_OR_OP):
	    case (XOS_BRACKET_CLOSE_OP):
		/* No attribute value specified. Hence request only for */
		/* the presence of the attribute.			*/
		item_type = DS_PRESENT;
		break;
	    default:
		*error_pos = curr_index + 1;
		*error_type = OMX_MISSING_OPERATOR;
		retval = OM_WRONG_VALUE_MAKEUP;
		break;
	}
    }

    if (retval == OM_SUCCESS)
    {
#ifdef XOS_DEBUG
    sprintf(debug_msg, "DS_FILTER_ITEM_TYPE = <%d>\n", item_type);
    xos_debug_string(debug_msg, 0, 0);
#endif

	*len_read = curr_index;
	len -= curr_index;

	/* if the request is only for presence of attribute */
	if (item_type == DS_PRESENT)
	{
	    /* create filter item with DS_PRESENT */
	    retval = xos__create_item_ds_present(&objid, &object, error_type);
	}
	else
	{
	    if (omsyntax == OM_S_OBJECT)
	    {
		/* get the length of the structured attribute value */
		retval = xos_get_class_value_data(&str[curr_index], len, 
						  OM_TRUE, &item_len, NULL, 
						  error_pos, error_type);
		if (retval == OM_SUCCESS)
		{
		    /* create the structured attribute object */
		    retval = xos_create_struct_attr_obj(&objid, &omclass, 
							&str[curr_index], 
							item_len, val_mem_top, 
							OM_TRUE, local_strings,
							&object, error_pos, 
							error_type);
		}
	    }
	    else
	    {
		/* get the length of the filter item */
		retval = xos__get_filter_item_length(&str[curr_index], len, 
						     &wild_count, &item_len, 
						     error_pos, error_type);
		if (retval == OM_SUCCESS)
		{
		    if (wild_count == 0)
		    {
			/* create the attribute object */
			retval = xos_create_normal_attr_obj(&objid, omsyntax, 
							    &str[curr_index], 
							    item_len, 	
							    val_mem_top, 
							    OM_TRUE, 
							    local_strings, 
							    &object, error_pos, 
							    error_type);
		    }
		    else
		    {
			/* create filter item object with substrings */
			retval = xos__create_substrings_obj(&objid, omsyntax,
							    &str[curr_index], 
							    item_len,
							    val_mem_top,
							    wild_count, 
							    local_strings,
							    &object, error_pos,
							    error_type);
		    }
		}
            }

	    if (retval != OM_SUCCESS)
	    {
		*error_pos += curr_index;
	    }
	    else
	    {
		if (omsyntax == OM_S_OBJECT || wild_count == 0)
		{
		    /* locate DS_FILTER_ITEM_TYPE and replace its value */
		    while (object[i].type != DS_FILTER_ITEM_TYPE)
			++i; 

		    object[i].value.enumeration = item_type;
		}

		*len_read += item_len;
	    }
	}
    }

    if (retval != OM_SUCCESS)
    {
	free(objid.elements);
	if (omsyntax == OM_S_OBJECT)
	    free(omclass.elements);
    }
    else
    {
	/* If the object is of type DS_C_DS_DN, free the omclass since the  */
	/* memory space would be provided by xos_convert_str_to_public_name */
	if (omsyntax == OM_S_OBJECT)
	{
	    if (omclass.length == dsP_DS_C_DS_DN.length &&
		memcmp(omclass.elements, dsP_DS_C_DS_DN.elements,
		       dsP_DS_C_DS_DN.length) == 0)
	    {
		free(omclass.elements);
	    }
	}

	*filter_item = object;
    }

#ifdef XOS_DEBUG
    sprintf(debug_msg, "Length Read = <%d>", *len_read);
    xos_debug_string(debug_msg, 0, 0);
    xos_debug_string("xos__create_filter_item : EXIT", 0, -1);
#endif

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__get_filter_item_length	   	   	   */
/* Description   : This function scans the input string and return */
/*		   the length of the first attribute value         */
/*		   encountered in the input string. Additionally,  */
/*		   the function also returns a count of the number */
/*		   of wild cards encountered in the input string.  */
/*		   This function is only used for normal attributes*/
/*		   The input string is of the form		   */
/*		   schmid) && ((OCL=ORP) || (OCL=REP)) && !(SN=ron)*/ 
/*		   In this example the length returned is 6.       */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__get_filter_item_length(
    char       *str,	    /* IN  - The input string			*/
    OM_uint32   len,	    /* IN  - Length of the input string		*/
    OM_uint    *wild_count, /* OUT - Count of wild characters in input  */
    OM_uint32  *item_len,   /* OUT - Length of attribute value		*/
    OM_integer *error_pos,  /* OUT - Error position in case of errors	*/
    OM_integer *error_type) /* OUT - Type of error in case of errors	*/
{
OM_return_code  retval;
OM_uint32   	curr_index;
OM_boolean	end_of_value;
OM_boolean 	open_quote_found, close_quote_found;

    *wild_count = 0;

    curr_index = 0;
    /* skip past white spaces */
    while (curr_index < len && isspace(str[curr_index]))
	++curr_index;

    if (curr_index == len)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_ATTR_VALUE;
	return(OM_WRONG_VALUE_MAKEUP);
    }

    open_quote_found = close_quote_found = OM_FALSE;
    end_of_value = OM_FALSE;
    retval = OM_SUCCESS;

    for (; curr_index < len; ++curr_index)
    {
	/* if escape operator */
	if (str[curr_index] == XOS_ESCAPE_OP)
	{
	    /* if the escape character is the last data character */
	    if (curr_index == len - 1)
	    {
		retval = OM_WRONG_VALUE_MAKEUP;
		break;
	    }

	    /* skip through escape character */
	    ++curr_index;

	    /* if hexadecimal values specified ("\xnn") */
	    if (tolower(str[curr_index]) == XOS_HEX_OP)
	    {
		if (curr_index + 2 > len - 1)
		{
		    retval = OM_WRONG_VALUE_MAKEUP;
		    break;
		}

		/* skip past the hexadecimal characters */
		curr_index += 2;
	    }
	    continue;
	}

	/* if quote character */
	if (str[curr_index] == XOS_ATTR_QUOTE_OP)
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
		continue;
	    }
	}

        if (str[curr_index] == XOS_WILD_CARD_OP)
	{
	    ++*wild_count;
	    continue;
	}

	/* The reserved characters are not interpreted within quotes */
	if (open_quote_found == OM_FALSE || close_quote_found == OM_TRUE)
	{
	    switch(str[curr_index])
	    {
		case(XOS_AND_OP):
		case(XOS_OR_OP):
		case(XOS_BRACKET_CLOSE_OP):
		case(XOS_BRACKET_OPEN_OP):
		case(XOS_F_NOT):
		    end_of_value = OM_TRUE;
		    break;
	    }

	    if (end_of_value == OM_TRUE || isspace(str[curr_index]))
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
    }

    if (retval == OM_SUCCESS)
	*item_len = curr_index;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_item_ds_present	   	   	   */
/* Description   : This function is used to create the             */
/*		   DS_C_FILTER_ITEM object with DS_FILTER_ITEM_TYPE*/
/*		   as DS_PRESENT.				   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_item_ds_present(
    OM_string  *objid,	     /* IN  - Object Identifier of the attribute */
    OM_object  *filter_item, /* OUT - The created filter object		 */
    OM_integer *error_type)  /* OUT - Type of error in case of errors    */
{
OM_return_code  retval;
OM_object	object;

    /* Allocate the memory for the OM_descriptors. One each is */
    /* required for the DS_C_FILTER_ITEM, DS_ATTRIBUTE_TYPE,   */
    /* DS_FILTER_ITEM_TYPE and the OM_NULL_DESCRIPTOR.	       */
    object = (OM_descriptor *) malloc(sizeof(OM_descriptor) * 4);

    if (object != NULL)
    {
	/* Initialize the first OM_descriptor to DS_C_FILTER_ITEM */
	omX_fill_oid(OM_CLASS, dsP_DS_C_FILTER_ITEM, &object[0]);

	/* Initialize the second OM_descriptor to DS_ATTRIBUTE_TYPE */
	omX_fill_oid(DS_ATTRIBUTE_TYPE, *objid, &object[1]);

	/* Initialize the third OM_descriptor to DS_FILTER_ITEM_TYPE */
	omX_fill(DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, DS_PRESENT, NULL, 
		 &object[2]);

	/* Initialize the fourth OM_descriptor to NULL */
	XOS_FILL_OMD_NULL(&object[3]);

	*filter_item = object;

	retval = OM_SUCCESS;
   }
   else
   {
	*error_type = OMX_MEMORY_INSUFFICIENT;
	retval = OM_MEMORY_INSUFFICIENT;
   }
   
   return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_substrings_obj	   	   	   */
/* Description   : This function is used to scan the input string  */
/*		   and create a DS_C_FILTER_ITEM with DS_SUBSTRINGS*/
/*		   The input string is of the form "aaaa*bb*cc"	   */ 
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_substrings_obj(
    OM_string  *objid,	       /* IN  - Attribute Object Identifier           */
    OM_syntax   omsyntax,      /* IN  - Syntax of the Attribute		      */
    char       *str,	       /* IN  - Attribute Value string		      */
    OM_uint32   len,	       /* IN  - Length of Value string		      */
    char       *val_mem_top,   /* IN  - Memory heap to store attribute values */
    OM_uint     wild_count,    /* IN  - Count of wild characters in input     */
    OM_boolean  local_strings, /* IN  - To specify local strings      	      */
    OM_object  *filter_item,   /* OUT - The created filter item		      */
    OM_integer *error_pos,     /* OUT - Error position in case of errors      */
    OM_integer *error_type)    /* OUT - Type of error in case of errors	      */
{
OM_return_code	retval;
OM_object  	object;
OM_uint32  	curr_index, i;
OM_uint32	substr_len, val_len, str_len;
OM_uint		tot_desc, j;

    curr_index = 0;
    /* skip past white spaces */
    while (curr_index < len && isspace(str[curr_index]))
	++curr_index;

    retval = OM_SUCCESS;

    if (curr_index == len)
    {
	retval = OM_WRONG_VALUE_MAKEUP;
    }
    else
    {
	/* if quote character */
	if (str[curr_index] == XOS_ATTR_QUOTE_OP)
	{
	    /* skip past the opening quote character */
	    ++curr_index;

	    /* Remove the trailing spaces */
	    i = len - 1;
	    while (i > 0 && isspace(str[i]))
		--i;

	    /* Remove the closing quote character */
	    --i;

	    if (i == 0)
		retval = OM_WRONG_VALUE_MAKEUP;
	    else
		len = i + 1;
	}
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_MISSING_ATTR_VALUE;
	return(retval);
    }

    i = curr_index;
    if (str[curr_index] == XOS_WILD_CARD_OP)
    {
	++curr_index;

	/* skip past white spaces */
	while (curr_index < len && isspace(str[curr_index]))
	    ++curr_index;

	/* only "*" specified */
	if (curr_index == len)
	{
	    /* create filter item with DS_PRESENT */
	    return(xos__create_item_ds_present(objid, filter_item, 
					       error_type));
	}
    }

    curr_index = i;

    /* Compute the total number of required OM_descriptor structures 	   */
    /* One each is required for the DS_C_FILTER_ITEM, DS_ATTRIBUTE_TYPE,   */
    /* DS_FILTER_ITEM_TYPE and OM_NULL_DESCRIPTOR.          		   */
    /* The DS_ATTRIBUTE_VALUES require (wild_count + 1).		   */
    tot_desc = 4 + (wild_count + 1);
    object = (OM_descriptor *) malloc(sizeof(OM_descriptor) * (tot_desc));

    /* Initialize the first descriptor to DS_C_FILTER_ITEM */
    omX_fill_oid(OM_CLASS, dsP_DS_C_FILTER_ITEM, &object[0]);

    /* Initialize the second OM_descriptor to DS_ATTRIBUTE_TYPE */
    omX_fill_oid(DS_ATTRIBUTE_TYPE, *objid, &object[1]);

    /* Initialize the third OM_descriptor to DS_FILTER_ITEM_TYPE */
    omX_fill(DS_FILTER_ITEM_TYPE, OM_S_ENUMERATION, DS_SUBSTRINGS, NULL, 
	     &object[2]);

    /* Initialize all the others to NULL descriptors */
    for (j = 3; j < tot_desc; ++j)
	XOS_FILL_OMD_NULL(&object[j]);

    j = 3;
    str_len = len - curr_index;

    /* if first character is not XOS_WILD_CARD_OP, then DS_INITIAL_SUBSTRING */
    if (str[curr_index] != XOS_WILD_CARD_OP)
    {
	/* get the length of the substring */
	retval = xos__get_substring_length(&str[curr_index], str_len, 
					   &substr_len, error_pos, error_type);

	if (retval != OM_SUCCESS)
	{
	    *error_pos += curr_index;
	}
	else
	{
	    /* process reserved characters and create a normalized string */
	    retval = xos__get_substring_value(&str[curr_index], substr_len, 
					      val_mem_top, &val_len, error_pos,
					      error_type);
	    if (retval != OM_SUCCESS)
	    {
		*error_pos += curr_index;
	    }
	    else
	    {
		retval = xos_fill_descriptor(DS_INITIAL_SUBSTRING, omsyntax, 
					     val_len, val_mem_top, 
					     local_strings, &object[j]);

		if (retval != OM_SUCCESS)
		{
		    *error_pos = curr_index + 1;
		    *error_type = OMX_WRONG_VALUE;
		}
	    }
	}

	if (retval == OM_SUCCESS)
	{
	    /* skip past the read value */
	    curr_index += substr_len;
	    str_len -= substr_len;
	    ++j;

	    /* move to the next available memory */
	    val_mem_top += val_len;
	}
    }

    if (retval == OM_SUCCESS)
    {
	/* skip past XOS_WILD_CARD_OP character */
	++curr_index;
	--str_len;
	--wild_count;

	while (curr_index < len)
	{
	    retval = xos__get_substring_length(&str[curr_index], str_len, 
					      &substr_len,error_pos,error_type);
	    if (retval != OM_SUCCESS)
	    {
		*error_pos += curr_index;
		break;
	    }

	    retval = xos__get_substring_value(&str[curr_index], substr_len, 
					      val_mem_top, &val_len, error_pos,
					      error_type);
	    if (retval != OM_SUCCESS)
	    {
		*error_pos += curr_index;
		break;
	    }

	    if (wild_count == 0)
	    {
		/* request is for DS_FINAL_SUBSTRING */
		retval = xos_fill_descriptor(DS_FINAL_SUBSTRING, omsyntax, 
					     val_len, val_mem_top, 
					     local_strings, &object[j]);
	    }
	    else
	    {
		retval = xos_fill_descriptor(DS_ATTRIBUTE_VALUES, omsyntax, 
					     val_len, val_mem_top, 
					     local_strings, &object[j]);
	    }

	    if (retval != OM_SUCCESS)
	    {
		*error_pos = curr_index + 1;
		*error_type = OMX_WRONG_VALUE;
		break;
	    }

	    ++j;

	    /* skip past the read length and the XOS_WILD_CARD_OP character */
	    curr_index = curr_index + substr_len + 1;
	    str_len = str_len - substr_len - 1;

	    --wild_count;

	    /* move to the next available memory */
	    val_mem_top += val_len;
	}
    }

    if (retval != OM_SUCCESS)
    {
	free(object);
    }
    else
    {
	*filter_item = object;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__get_substring_length	   	   	   */
/* Description   : This function is used to scan the input string  */
/*		   and return the length of the first encountered  */
/*		   substring. The input string is of the form      */
/*		   "aaaa*bb*cc". In this example the length 	   */ 
/*		    returned is 4.				   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__get_substring_length(
    char       *str,	    /* IN  - The input string			 */
    OM_uint32   len,   	    /* IN  - Length of the input string		 */
    OM_uint32  *substr_len, /* OUT - Length of the encountered substring */
    OM_integer *error_pos,  /* OUT - Error position in case of errors	 */
    OM_integer *error_type) /* OUT - Type of errors in case of errors	 */
{
OM_return_code  retval;
OM_uint32  	curr_index;

    for (curr_index = 0; curr_index < len; ++curr_index)
    {
	/* if escape operator */
	if (str[curr_index] == XOS_ESCAPE_OP)
	{
	    /* skip through the escape character */
	    ++curr_index;

	    /* if hexadecimal values specified ("\xnn") */
	    if (tolower(str[curr_index]) == XOS_HEX_OP)
	    {
		/* skip past the hexadecimal characters */
		curr_index += 2;
	    }
	    continue;
	}

	if (str[curr_index] == XOS_WILD_CARD_OP)
	    break;
    }

    if (curr_index == 0)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_FORMAT_ERROR;
	retval = OM_WRONG_VALUE_MAKEUP;
    }
    else
    {
	*substr_len = curr_index;
	retval = OM_SUCCESS;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__get_substring_value	   	   	   */
/* Description   : This function converts the input substring to   */
/*		   a normalized string. All the special characters */
/*		   and escape sequences in the input string are    */
/*		   processed.					   */ 
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__get_substring_value(
    char 	*in_str,     /* IN  - The input string		       */
    OM_uint32 	 in_len,     /* IN  - Length of the input string       */
    char 	*out_str,    /* OUT - The converted string	       */
    OM_uint32 	*out_len,    /* OUT - Length of the output string      */
    OM_integer 	*error_pos,  /* OUT - Error position in case of errors */
    OM_integer 	*error_type) /* OUT - Type of error in case of errors  */
{
OM_return_code	retval;
OM_uint32 	curr_index;
OM_uint32 	i;
OM_sint		d;

    curr_index = 0;
    retval = OM_SUCCESS;
    for (i = 0; curr_index < in_len; ++curr_index)
    {
	/* if escape character */
	if (in_str[curr_index] == XOS_ESCAPE_OP)
	{
	    /* skip past XOS_ESCAPE_OP character */
	    ++curr_index;

	    /* If hexadecimal values specified ("\xnn") */
	    if (tolower(in_str[curr_index]) == XOS_HEX_OP)
	    {
		/* skip past XOS_HEX_OP character */
		++curr_index;

		/* validate the hex characters */
		if (!(isxdigit(in_str[curr_index]) &&
		      isxdigit(in_str[curr_index + 1])))
		{
		    retval =  OM_WRONG_VALUE_MAKEUP;
		    break;
		}

		/* fetch the hexadecimal value */
		sscanf(&in_str[curr_index], "%2x", &d);
		out_str[i] = d;
		++curr_index;
	    }
	    else
	    {
		out_str[i] = in_str[curr_index];
	    }
	    ++i;
	    continue;
	}

	out_str[i] = in_str[curr_index];
	++i;
    }

    if (retval != OM_SUCCESS)
    {
	*error_pos = curr_index + 1;
	*error_type = OMX_FORMAT_ERROR;
    }
    else
    {
	if (i == 0)
	{
	    *error_pos = curr_index + 1;
	    *error_type = OMX_MISSING_ATTR_VALUE;
	    retval = OM_WRONG_VALUE_MAKEUP;
	}
    }

    if (retval == OM_SUCCESS)
	*out_len = i;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__create_single_filter			   */
/* Description   : This function is used to create a DS_C_FILTER   */
/*		   object from the input object. The input object  */
/*		   is either DS_C_FILTER or DS_C_FILTER_ITEM.      */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__create_single_filter(
    OM_type         object_type, /* IN  - Object type (Filter item or Filter) */
    OM_enumeration  filter_enum, /* IN  - DS_FILTER_TYPE		      */
    OM_object  	    object,      /* IN  - DS_C_FILTER or DS_C_FILTER_ITEM     */
    OM_object      *filter)  	 /* OUT - The created filter object   	      */
{
OM_return_code  retval;
OM_object	filter_obj;

    /* Allocate the memory for the class descriptor, DS_FILTER_ITEMS, */
    /* DS_FILTER_TYPE and NULL descriptor.		    	      */
    filter_obj = (OM_descriptor *) malloc(sizeof(OM_descriptor) * 4);

    if (filter_obj == NULL)
    {
	retval = OM_MEMORY_INSUFFICIENT;
    }
    else
    {
	/* Initialize the first OM_descriptor to DS_C_FILTER */
	omX_fill_oid(OM_CLASS, dsP_DS_C_FILTER, &filter_obj[0]);

	/* Initialize the second OM_descriptor to DS_FILTER_TYPE */
	omX_fill(DS_FILTER_TYPE, OM_S_ENUMERATION, filter_enum, NULL, 
		 &filter_obj[1]);

	/* Initialize the third OM_descriptor to DS_FILTER_ITEMS */
	omX_fill(object_type, OM_S_OBJECT, 0, object, &filter_obj[2]);

	/* Initialize the fourth OM_descriptor to NULL descriptor */
	XOS_FILL_OMD_NULL(&filter_obj[3]);

	*filter = filter_obj;

	retval = OM_SUCCESS;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__rearrange_stack		   	   	   */
/* Description   : This function checks the prority of the read    */
/*		   token axoinst the tokens in the stack and       */
/*		   accordingly rearranges the stack.		   */
/* Return Value  : OM_SUCCESS or an error return code indicating   */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_return_code xos__rearrange_stack(
    xos_token	    read_token,   /* IN  - The token read	   	   */
    xos_stack_item *stack,        /* I/O - The stack			   */
    OM_sint	   *stack_index,  /* I/O - Index to top of stack	   */
    OM_integer	   *error_type)   /* OUT - Type of error in case of errors */
{
OM_return_code	retval;
OM_object	filter_obj;
OM_uint		desc_index;
OM_uint		filter_count;
xos_stack_token stack_token;
xos_stack_item  stack_item;

    retval = OM_SUCCESS;
    while (retval == OM_SUCCESS)
    {
	/* compare the priorities of the read token with those on stack. If */
	/* priority of read token is lower, pop all the filter/filter items */
	/* from stack and recreate a new filter with AND/OR condition.	    */
	while (xos__compare_priority(read_token, stack[*stack_index - 2].token)
	       < 0)
	{
	    stack_token = stack[*stack_index - 2].token;
	    filter_count = 2;

	    /* count similar AND/OR tokens on stack */
	    while (stack[*stack_index - 2 * filter_count].token == stack_token)
		++filter_count;

	    /* Allocate the memory for the OM_descriptors. One each is     */
	    /* required for the DS_C_FILTER, DS_FILTER_TYPE and NULL desc. */
	    filter_obj = (OM_descriptor *) 
		          malloc(sizeof(OM_descriptor) * (3 + filter_count));

	    if (filter_obj == NULL)
	    {
		*error_type =  OMX_MEMORY_INSUFFICIENT;
		retval = OM_MEMORY_INSUFFICIENT;
		break;
	    }

	    /* Initialize the first OM_descriptor to DS_C_FILTER */
	    omX_fill_oid(OM_CLASS, dsP_DS_C_FILTER, &filter_obj[0]);

	    /* Initialize the second OM_descriptor to DS_FILTER_TYPE */
	    if (stack_token == XOS_S_AND)
	    {
		omX_fill(DS_FILTER_TYPE, OM_S_ENUMERATION, DS_AND, NULL, 
			 &filter_obj[1]);
	    }
	    else
	    {
		omX_fill(DS_FILTER_TYPE, OM_S_ENUMERATION, DS_OR, NULL, 
			 &filter_obj[1]);
	    }

	    /* Initialize the last OM_descriptor to NULL descriptor */
	    XOS_FILL_OMD_NULL(&filter_obj[3 + filter_count - 1]);

	    desc_index = 2;

	    while (filter_count > 0)
	    {
		/* Initialize descriptor with filter/filter item */
		if (stack[*stack_index - 1].token == XOS_S_FILTER_ITEM)
	 	{
		    omX_fill(DS_FILTER_ITEMS, OM_S_OBJECT, 0, 
			     stack[*stack_index - 1].object, 
			     &filter_obj[desc_index]);
		}
		else
	 	{
		    omX_fill(DS_FILTERS, OM_S_OBJECT, 0, 
			     stack[*stack_index - 1].object, 
			     &filter_obj[desc_index]);
		}
		++desc_index;

		/* pop the filter/filter items from stack */
		--*stack_index;
		--filter_count;

		/* pop operator */
		if (filter_count > 0)
		    --*stack_index;
	    }

	    /* rearrange the DS_C_FILTER object such that all the DS_FILTERS */
	    /* are together and all the DS_FILTER_ITEMS are together.	     */
	    xos__make_values_adjacent(filter_obj);

	    /* push the newly constructed DS_C_FILTER on stack */
	    stack[*stack_index].token = XOS_S_FILTER;
	    stack[*stack_index].object = filter_obj;
	    ++*stack_index;
	}

	if (retval != OM_SUCCESS)
	    break;

	/* compare the priorities of the read token with those on stack.  */
	/* If priority of read token is greater, push read token on stack.*/
	if (xos__compare_priority(read_token, stack[*stack_index - 2].token) 
	    > 0)
	{
	    stack[*stack_index].token = 
		(read_token == XOS_F_OR) ? XOS_S_OR : XOS_S_AND;
	    ++*stack_index;
	    break;
	}

	/* The priorities of both tokens are now the same */
	if ((read_token == XOS_F_OR && 
	     stack[*stack_index - 2].token == XOS_S_OR) ||
	    (read_token == XOS_F_AND && 
	     stack[*stack_index - 2].token == XOS_S_AND))
	{
	    stack[*stack_index].token =
		(read_token == XOS_F_OR) ? XOS_S_OR : XOS_S_AND;
	    ++*stack_index;
	    break;
	}

	if (read_token == XOS_F_BRACKET_CLOSE &&
	    stack[*stack_index - 2].token == XOS_S_BRACKET_OPEN)
	{
	    stack_item = stack[*stack_index - 1];

	    /* check if there is a NOT operator before the filter */
	    if (stack[*stack_index - 3].token == XOS_S_NOT)
	    {
		if (stack_item.token == XOS_S_FILTER_ITEM)
		{
		    retval = xos__create_single_filter(DS_FILTER_ITEMS, DS_NOT,
						       stack_item.object, 
						       &filter_obj);
		}
		else
		{
		    retval = xos__create_single_filter(DS_FILTERS, DS_NOT,
						       stack_item.object, 
						       &filter_obj);
		}
		if (retval != OM_SUCCESS)
		{
		    *error_type =  OMX_MEMORY_INSUFFICIENT;
		    break;
		}

		/* pop the NOT operator from stack */
		*stack_index -= 3;

		/* push the created filter on the stack */
		stack[*stack_index].token = XOS_S_FILTER;
		stack[*stack_index].object = filter_obj;
		++*stack_index;
	    }
	    else
	    {
		/* pop the filter/filter item and bracket from stack */
		*stack_index -= 2;

		/* push the popped filter/filter item onto the stack */
		stack[*stack_index].token = stack_item.token;
		stack[*stack_index].object = stack_item.object;
		++*stack_index;
	    }
	    break;
	}

	/* End of input string reached */
	if (read_token == XOS_F_EOS &&
	    stack[*stack_index - 2].token == XOS_S_START)
	{
	    break;
	}

	/* must be mismatched brackets */
	*error_type = OMX_MISMATCHED_BRACKETS;
	retval = OM_WRONG_VALUE_MAKEUP;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__compare_priority	   	   	   */
/* Description   : This function compares the priority of the token*/
/*		   read axoinst the stack token and returns an     */
/*		   appropriate value.				   */
/* Return Value  : A positive integer if the token priority is     */
/*		   greater than the stack priority, a negative     */
/*		   integer if the token priority is less than stack*/
/*		   priority, 0 otherwise.			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_sint xos__compare_priority(
    xos_token       read_token,  /* IN - The token read  */
    xos_stack_token stack_token) /* IN - The stack token */
{
OM_sint token_priority, stack_priority;

    switch(read_token)
    {
	case XOS_F_EOS:
        case XOS_F_BRACKET_CLOSE:
	    token_priority = 0;
	    break;
	case XOS_F_OR:
	    token_priority = 1;
	    break;
	case XOS_F_AND:
	    token_priority = 2;
	    break;
	default:
	    token_priority = -1;
	    break;
    }

    switch(stack_token)
    {
	case (XOS_S_START):
	case (XOS_S_BRACKET_OPEN):
	    stack_priority = 0;
	    break;
	case (XOS_S_OR):
	    stack_priority = 1;
	    break;
	case (XOS_S_AND):
	    stack_priority = 2;
	    break;
	default:
	    stack_priority = -1;
	    break;
    }

    if (token_priority < stack_priority)
	return(-1);

    if (token_priority > stack_priority)
	return(1);

    return(0);
	
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__make_values_adjacent			   */
/* Description   : This function is used to scan the DS_C_FILTER   */
/*		   object and rearrange the object descriptors	   */
/*		   so that all DS_FILTERS are together and all the */
/*		   DS_FILTER_ITEMS are together.		   */
/*		   Note that if this not done, XOM returns with a  */
/*		   message OM_VALUES_NOT_ADJACENT.		   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void xos__make_values_adjacent(
    OM_object filter)  /* I/O - The filter to be rearranged */
{
OM_object object;
OM_type	  omtype;
OM_uint	  i;
OM_uint   desc_count = 0;

    object = filter;
    omtype = object[2].type;

    i = 3;
    /* skip through similar OM_types */
    while (object[i].type == omtype)
	++i;

    /* now skip through non-similar OM_types */
    while (object[i].type != OM_NO_MORE_TYPES && object[i].type != omtype)
	++i;

    /* if end of descriptor list reached, then values are adjacent */
    if (object[i].type == OM_NO_MORE_TYPES)
	return;

    /* count the total number of descriptors of type DS_FILTERS and   */
    /* DS_FILTER_ITEMS.	 Skip the class and DS_FILTER_TYPE descriptor */
    for (i = 2; object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (object[i].type == DS_FILTERS || object[i].type == DS_FILTER_ITEMS)
	    ++desc_count;
    }

    /* now sort the array of DS_FILTERS and DS_FILTER_ITEMS */
    qsort((void *) &object[2], (size_t) desc_count, 
	  (size_t) sizeof(OM_descriptor), xos__sort_filter);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__sort_filter			   	   */
/* Description   : This function is used by the qsort function to  */
/*		   sort the array of OM_descriptors used to build  */
/*		   a DS_C_FILTER object. The sorting is such that  */
/*		   all the DS_FILTER_ITEMS are grouped together    */
/*		   followed by all the DS_FILTERS.		   */
/* Return Value  : -1 if DS_FILTER_ITEMS, 1 if DS_FILTERS.	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static int xos__sort_filter(
    const void *desc1,  /* IN - The first descriptor to be compared  */
    const void *desc2)  /* IN - The second descriptor to be compared */
{
    if (((OM_descriptor *) desc1)->type == DS_FILTER_ITEMS)
	return(-1);
    else
	return(1);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_free_filter_object			   */
/* Description   : This function is used to free the memory space  */
/*		   occupied by the filter object.		   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xos_free_filter_object(
    OM_object filter)  /* IN - The filter object to be freed */
{
OM_uint i;

    for (i = 0; filter[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (filter[i].type == DS_FILTERS)
	{
	    /* call this function recursively */
	    xos_free_filter_object(filter[i].value.object.object);
	    continue;
	}
	if (filter[i].type == DS_FILTER_ITEMS)
	{
	    /* free the filter items */
	    xos__free_filter_item_object(filter[i].value.object.object);
	}
    }
    free(filter);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos__free_filter_item_object			   */
/* Description   : This function is used to free the memory space  */
/*		   occupied by the filter item object.		   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void xos__free_filter_item_object(
    OM_object filter_item)  /* IN - The filter item object to be freed */
{
OM_uint i;

    for (i = 0; filter_item[i].type != OM_NO_MORE_TYPES; ++i)
    {
	/* if structured attribute */
	if (filter_item[i].type == DS_ATTRIBUTE_VALUES &&
	    filter_item[i].syntax == OM_S_OBJECT)
	{
	    break;
	}
    }

    if (filter_item[i].type != OM_NO_MORE_TYPES)
    {
	/* free the structured attribute object */
	xos_free_attr_object(filter_item);
    }
    else
    {
	for (i = 0; filter_item[i].type != OM_NO_MORE_TYPES; ++i)
	{
	    /* free the object identifier string */
	    if (filter_item[i].type == DS_ATTRIBUTE_TYPE)
	    {
		free(filter_item[i].value.string.elements);
		continue;
	    }

	    if (filter_item[i].type == DS_ATTRIBUTE_VALUES &&
		filter_item[i].syntax == OM_S_OBJECT_IDENTIFIER_STRING)
	    {
		/* The value is freed only if the attribute syntax is */
		/* OM_S_OBJECT_IDENTIFIER_STRING.                     */
		free(filter_item[i].value.string.elements);
		continue;
	    }
	}
	free(filter_item);
    }
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
