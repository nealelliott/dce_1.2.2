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
 * $Log: gctutil.c,v $
 * Revision 1.1.4.2  1996/02/18  18:19:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:59  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:58:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:38  root]
 * 
 * Revision 1.1.2.4  1994/09/19  08:39:36  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:33:45  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:55  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:55  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:21  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:56:00  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:36:54  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:17:07  keutel
 * 	creation
 * 	[1994/03/21  12:57:18  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctutil.c					   */
/* Description : This file contains all the utility functions used */
/*		 by the GDSCP Translator.	   		   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctutil.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:19:11 $";
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

#include <xomext.h>

#include <xoi.h>

#include <gdscp.h>
#include <gctstruct.h>
#include <gctconv.h>
#include <gcterr.h>
#include <gctbind.h>
#include <gctmoveto.h>
#include <gctutil.h>

/* import the necessary Object Identifier constants */
OM_IMPORT(DS_C_DS_DN)
OM_IMPORT(DS_C_FILTER)

/*-- Static Function(s) -------------------------------------------*/
static signed16 gc_t__process_result_dn(
    OM_object        dn_object,     /* IN  - The Distinguished Name object  */
    gc_t_vect_type   vect_type,	    /* IN  - Type of vector		    */
    boolean          local_strings, /* IN  - To specify local strings       */
    unsigned32      *vect_index,    /* I/O - Index into result vector	    */
    gc_t_vector    **result_vector, /* OUT - Result vector		    */
    gc_t_vector    **err_vector);   /* OUT - Error vector in case of errors */

static signed16 gc_t__process_result_attr(
    OM_object      attr_object,   /* IN  - The DS_C_ATTRIBUTE object      */
    boolean        local_strings, /* IN  - To specify local strings       */
    unsigned32    *vect_index,    /* I/O - Index into result vector	  */
    gc_t_vector  **result_vector, /* OUT - Result vector		  */
    gc_t_vector  **err_vector);   /* OUT - Error vector in case of errors */

static signed16 gc_t__create_attr_info(
    OM_object      attr_object,   /* IN  - The DS_C_ATTRIBUTE object      */
    boolean        local_strings, /* IN  - To specify local strings       */
    unsigned32    *vect_index,    /* I/O - Index into result vector	  */
    gc_t_vector  **result_vector, /* OUT - Result vector		  */
    gc_t_vector  **err_vector);   /* OUT - Error vector in case of errors */

static signed16 gc_t__process_result_struct_attr(
    OM_object      class_object,  /* IN  - The class object               */
    boolean        local_strings, /* IN  - To specify local strings       */
    unsigned32    *vect_index,    /* I/O - Index into result vector	  */
    gc_t_vector  **result_vector, /* OUT - Result vector		  */
    gc_t_vector  **err_vector);   /* OUT - Error vector in case of errors */

static void gc_t__get_class_comp_count(
    OM_object   class,		   /* IN  - The class object           */
    unsigned32 *comp_type_count,   /* OUT - Number of Component types  */
    unsigned32 *comp_val_count,    /* OUT - Number of Component values */
    unsigned32 *scomp_type_count); /* OUT - No.of struct.comp.types    */

static signed16 gc_t__process_cont_ref_obj(
    OM_object      ref_object,    /* IN  - The continuation reference object */
    boolean        local_strings, /* IN  - To specify local strings          */
    gc_t_vector   *result_vect,   /* I/O - The result vector         	     */
    gc_t_vector  **error_vect);   /* OUT - Error message in case of errors   */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_create_full_dn_string		   	   */
/* Description   : This function create a DN string with the full  */
/*		   path name. The input string which is the 	   */
/*		   relative name is appended to the current DIT	   */
/*		   position.					   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_create_full_dn_string(
    char      *rdn_string,  /* IN  - The RDN string           */
    OM_string *full_dn)     /* OUT - The created full DN      */
{
OM_string *current_dn;
char	  *value;
signed16   rdn_len;
signed16   delim_len;
signed16   retval;

    retval = GC_SUCCESS;

    if (rdn_string == NULL)
    {
	rdn_len = 0;
    }
    else
    {
	rdn_len = strlen(rdn_string);
    }

    current_dn = gc_t_get_current_dit_position();

    delim_len = strlen(GC_T_RDN_DELIMITER_STR);
    full_dn->elements = malloc(sizeof(char) * 
			       (current_dn->length + delim_len + rdn_len + 1));
    if (full_dn->elements != NULL)
    {
	memcpy(full_dn->elements, current_dn->elements, current_dn->length);
	full_dn->length = current_dn->length;
	value = ((char *) (full_dn->elements)) + current_dn->length;

	/* Append the delimiter only if it is not root */
	if (strlen(GC_T_RDN_DELIMITER_STR) != current_dn->length ||
	    memcmp(GC_T_RDN_DELIMITER_STR, current_dn->elements,
		   current_dn->length) != 0)
	{
	    memcpy(value, GC_T_RDN_DELIMITER_STR, delim_len);
	    full_dn->length += delim_len;
	    value += delim_len;
	}

	memcpy(value, rdn_string, rdn_len);
	full_dn->length += rdn_len;

	/* append the EOS character */
	((char *) (full_dn->elements))[full_dn->length] = '\0';
    }
    else
    {
	retval = GC_T_MEMORY_INSUFFICIENT;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_string_to_name		   	           */
/* Description   : This function is used to invoke the convenience */
/*	   	   library omX_string_to_object routine to convert */
/*		   a DN string to XDS private object		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_string_to_name(
    char               *dn_string,    /* IN  - The string to be converted     */
    boolean             local_strings,/* IN  - To specify local strings       */
    OM_private_object  *name, 	      /* OUT - The converted object	      */
    gc_t_vector       **error_vect)   /* OUT - Error message in case of errors*/
{
OM_return_code  om_ret_code;
OM_workspace    workspace;
OM_integer	error_position, error_type;
OM_boolean      request_local_strings;
OM_string	full_dn;
OM_string      *current_dn;
boolean         relative_name;
signed16	dn_len;
signed16        retval;
signed16	i;

    i = 0;
    relative_name = FALSE;
    *error_vect = NULL;
    retval = GC_SUCCESS;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    if (dn_string == NULL)
    {
	dn_len = 0;
    }
    else
    {
	dn_len = strlen(dn_string);
    }

    if ((workspace = gc_t_get_workspace()) == NULL)
    {
	retval = GC_T_BIND_NOT_MADE;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, error_vect);
    }
    else
    {
	/* if string is not specified, use the current DIT position */
	if (dn_string == NULL)
	{
	    current_dn = gc_t_get_current_dit_position();
	    full_dn.elements = current_dn->elements;
	    full_dn.length = current_dn->length;
	}
	else
	{
	    /* skip past white spaces */
	    while (i < dn_len && isspace(dn_string[i]))
		++i;

	    if (i == dn_len)
	    {
		retval = GC_T_MISSING_OBJECT_NAME;
	    }
	    else
	    {
		/* if full DN Name not specified */
		if (dn_string[i] != GC_T_RDN_DELIMITER_OP)
		{
		    retval = gc_t_create_full_dn_string(dn_string, &full_dn);
		    if (retval == GC_SUCCESS)
		    {
			relative_name = TRUE;
		    }
		}
		else
		{
		    full_dn.elements = (void *) dn_string;
		    full_dn.length = dn_len;
		}
	    }
	}

	if (retval != GC_SUCCESS)
	{
	    /* create the error vector */
	    gc_t_create_gct_error_vector(retval, NULL, error_vect);
	}
	else
	{
	    /* convert the string to DS_C_NAME object */
	    om_ret_code = omX_string_to_object(workspace, &full_dn, DS_C_DS_DN,
					       request_local_strings, name, 
					       &error_position, &error_type);
	    if (om_ret_code != OM_SUCCESS)
	    {
		/* if the error is in the input string */
		if (om_ret_code == OM_WRONG_VALUE_MAKEUP && 
		    error_type != OMX_SUCCESS)
		{
		    /* map the error to the translator defined error and */
	    	    /* create error vector 				 */
		    retval = gc_t_map_xos_error(error_type, 
					        (char *) (full_dn.elements),
						error_position, error_vect);
		}
		else
		{
		    /* map the XOM error to the translator defined */
		    /* error and create the error string.          */
		    retval = gc_t_map_xom_error(om_ret_code, error_vect);
		}
	    }
	}
    }

    /* if relative name specified, free the created full DN string */
    if (relative_name == TRUE)
	free(full_dn.elements);

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_string_to_object		   	   */
/* Description   : This function is used to invoke the convenience */
/*	   	   library omX_string_to_object routine to convert */
/*		   an attribute type/value string to XDS private   */
/*		   object		   			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_string_to_object(
    char                 *string,        /* IN  - The string to be converted */
    OM_object_identifier  class,         /* IN  - The OM Class to be created */
    boolean               local_strings, /* IN  - To specify local strings   */
    OM_private_object    *object,        /* OUT - The converted object	     */
    gc_t_vector         **error_vect)    /* OUT - Error messages, if errors  */
{
OM_return_code  om_ret_code;
OM_workspace    workspace;
OM_boolean      request_local_strings;
OM_integer	error_position, error_type;
OM_string  	attr_string;	
unsigned32	str_len, i;
signed16        retval;

    retval = GC_SUCCESS;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    if (string == NULL)
    {
	str_len = 0;
    }
    else
    {
	str_len = strlen(string);
    }

    if ((workspace = gc_t_get_workspace()) == NULL)
    {
	retval = GC_T_BIND_NOT_MADE;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, error_vect);
    }
    else
    {
	i = 0;
	/* skip past white spaces */
	while (i < str_len && isspace(string[i]))
	    ++i;

	if (i == str_len)
	{
	    retval = GC_T_MISSING_VALUE;
	    /* create the error vector */
	    gc_t_create_gct_error_vector(retval, NULL, error_vect);
	}
	else
	{
	    attr_string.elements = (void *) string;
	    attr_string.length = str_len;

	    /* convert the string to object */
	    om_ret_code = omX_string_to_object(workspace, &attr_string, 
					       class, request_local_strings,
					       object, &error_position, 
					       &error_type);

	    if (om_ret_code != OM_SUCCESS)
	    {
		/* if the error is in the input string */
		if (om_ret_code == OM_WRONG_VALUE_MAKEUP && 
		    error_type != OMX_SUCCESS)
		{
		    /* map error to the translator defined error and create */
	    	    /* error vector 					    */
		    retval = gc_t_map_xos_error(error_type, string,
						error_position, error_vect);
		}
		else
		{
		    /* map the XOM error to the translator defined */
		    /* error and create the error string.          */
		    retval = gc_t_map_xom_error(om_ret_code, error_vect);
		}
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_string_to_filter		   	   */
/* Description   : This function is used to invoke the convenience */
/*	   	   library omX_string_to_filter routine to convert */
/*		   the expression string to XDS DS_C_FILTER object */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_string_to_filter(
    char               *string,       /* IN  - The string to be converted     */
    boolean             local_strings,/* IN  - To specify local strings       */
    OM_private_object  *filter,       /* OUT - The converted filter object    */
    gc_t_vector       **error_vect)   /* OUT - Error message in case of errors*/
{
OM_return_code  om_ret_code;
OM_workspace    workspace;
OM_boolean	request_local_strings;
OM_integer	error_position, error_type;
OM_string       filter_string;
signed16	str_len;
signed16	retval, i;

    retval = GC_SUCCESS;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    if (string == NULL)
    {
	str_len = 0;
    }
    else
    {
	str_len = strlen(string);
    }

    if ((workspace = gc_t_get_workspace()) == NULL)
    {
	retval = GC_T_BIND_NOT_MADE;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, error_vect);
    }
    else
    {
	i = 0;
	/* skip past white spaces */
	while (i < str_len && isspace(string[i]))
	    ++i;

	if (i == str_len)
	{
	    retval = GC_T_MISSING_FILTER;
	    /* create the error vector */
	    gc_t_create_gct_error_vector(retval, NULL, error_vect);
	}
	else
	{
	    filter_string.elements = (void *) string;
	    filter_string.length = str_len;

	    /* convert the string to filter object */
	    om_ret_code = omX_string_to_object(workspace, &filter_string, 
					       DS_C_FILTER, 
					       request_local_strings, filter, 
					       &error_position, &error_type);
	    if (om_ret_code != OM_SUCCESS)
	    {
		/* if the error is in the input string */
		if (om_ret_code == OM_WRONG_VALUE_MAKEUP && 
		    error_type != OMX_SUCCESS)
		{
		    /* map error to the translator defined error and create */
		    /* error vector 					    */
		    retval = gc_t_map_xos_error(error_type, string,
						error_position, error_vect);
		}
		else
		{
		    /* map the XOM error to the translator defined */
		    /* error and create the error string.          */
		    retval = gc_t_map_xom_error(om_ret_code, error_vect);
		}
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_convert_to_normalized_string	   	   */
/* Description   : This function converts the input string to a    */
/*	   	   normalized string. All the special characters   */
/*		   and escape sequences in the input string are    */
/*		   processed.		   			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_convert_to_normalized_string(
    char       *in_str,	     /* IN  - The input string 		       */
    signed32    in_len,	     /* IN  - Length of the input string       */
    OM_string  *out_str,     /* OUT - The converted string 	       */
    signed32   *error_pos)   /* OUT - Error position in case of errors */
{
signed32  curr_index, d, i, k;
signed16  retval;
boolean   open_quote_found, close_quote_found;

    retval = GC_SUCCESS;
    open_quote_found = close_quote_found = FALSE;

    curr_index = 0;
    /* skip past white spaces */
    while (curr_index < in_len && isspace(in_str[curr_index]))
	++curr_index;

    if (curr_index == in_len)
    {
	retval = GC_T_MISSING_VALUE;
    }
    else
    {
	out_str->elements = malloc(sizeof(char) * in_len);
	if (out_str->elements == NULL)
	{
	    retval = GC_T_MEMORY_INSUFFICIENT;
	}
    }

    if (retval == GC_SUCCESS)
    {
	if (in_str[curr_index] == GC_T_QUOTE_OP)
	{
	    open_quote_found = TRUE;
	    ++curr_index;
	}

	for (i = 0; curr_index < in_len; ++curr_index)
	{
	    /* if escape character */
	    if (in_str[curr_index] == GC_T_ESCAPE_OP)
	    {
		/* skip past GC_T_ESCAPE_OP character */
		++curr_index;

		/* check if something specified after escape character */
		if (curr_index == in_len)
		{
		    retval = GC_T_FORMAT_ERROR;
		    break;
		}

		/* if hexadecimal values specified */
		if (tolower(in_str[curr_index]) == GC_T_HEX_OP)
		{
		    /* skip past the GC_T_HEX_OP character */
		    ++curr_index;

		    /* check if something specified after the character */
		    if (curr_index == in_len || 
			curr_index + 1 == in_len)
		    {
			retval = GC_T_FORMAT_ERROR;
			break;
		    }

		    /* validate the hex characters */
		    if (!(isxdigit(in_str[curr_index]) &&
			  isxdigit(in_str[curr_index + 1])))
		    {
			retval = GC_T_FORMAT_ERROR;
			break;
		    }

		    /* fetch the hexadecimal value */
		    sscanf(&in_str[curr_index], "%2x", &d);
		    ((char *)(out_str->elements))[i] = d;
		    ++curr_index;
		}
		else
		{
		    ((char *)(out_str->elements))[i] = in_str[curr_index];
		}
		++i;
		continue;
	    }

	    /* if quote character */
	    if (in_str[curr_index] == GC_T_QUOTE_OP)
	    {
		/* if additional quote character found */
		if  (close_quote_found == TRUE)
		{
		    retval = GC_T_FORMAT_ERROR;
		    break;
		}
		else
		{
		    close_quote_found = TRUE;
		    continue;
		}
	    }

	    if (close_quote_found == TRUE && (!isspace(in_str[curr_index])))
	    {
		retval = GC_T_FORMAT_ERROR;
		break;
	    }

	    /* ignore spaces if they are not within quotes */
	    if (open_quote_found == FALSE || close_quote_found == TRUE)
	    {
		if (isspace(in_str[curr_index]))
		{
		    for (k = curr_index + 1; k < in_len; ++k)
		    {
			if (!isspace(in_str[k]))
			    break;
		    }

		    /* if spaces are not part of data like in "wolf schmid" */
		    if (k == in_len)
			break;
		}
	    }

	    ((char *)(out_str->elements))[i] = in_str[curr_index];
	    ++i;
	}
    }

    if (retval == GC_SUCCESS)
    {
	if (i == 0)
	{
	    retval = GC_T_MISSING_VALUE;
	}
	else
	{
	    if (open_quote_found == TRUE && close_quote_found == FALSE)
	    {
		retval = GC_T_MISMATCHED_QUOTES;
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	out_str->length = i;
    }
    else
    {
	*error_pos = curr_index + 1;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_fill_with_objid	   	   		   */
/* Description   : This function is used to fill the OM_descriptor */
/*	   	   with the Object Identifier string. The input    */
/*		   string is an attribute abbreviation.		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_fill_with_objid(
    char	   *abbrv,     /* IN  - The attribute abbreviation       */
    OM_type	    omtype,    /* IN  - OM type to be filled in		 */ 
    OM_descriptor  *desc,      /* I/O - The descriptor to be filled      */
    gc_t_vector   **out_vect)  /* OUT - Error messages in case of errors */
{
OM_integer  retval_xoi;
OM_string   objid, str;
signed16    retval;

    retval = GC_SUCCESS;

    /* get the object identifier from the abbreviation */
    retval_xoi = xoi_get_attr_details(abbrv, NULL, &(objid.elements),
				      &(objid.length), NULL, NULL);

    if (retval_xoi != XOI_SUCCESS)
    {
	str.elements = (void *) abbrv;
	str.length = strlen(abbrv);

	/* map the error to translator defined error and create the */
	/* error vector.					    */
	retval = gc_t_map_xoi_error(retval_xoi, abbrv, out_vect);
    }
    else
    {
	/* fill the descriptor */
	omX_fill_oid(omtype, objid, desc);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_create_result_vector_data		   */
/* Description   : This function is used to the fill the result    */
/*		   vector with the required data. The input data   */
/*		   string is malloced and copied.		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error                               */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_create_result_vector_data(
    gc_t_vector    *result_vector, /* IN - The result vector            */
    unsigned32      vector_index,  /* IN - Index into the result vector */
    gc_t_vect_type  vector_type,   /* IN - Type of vector	        */
    OM_string	    data,	   /* IN - The input data	        */
    OM_syntax       syntax)        /* IN - OM_syntax of the data        */
{
OM_integer    retval_xoi;
OM_syntax     omsyntax;
unsigned32    conv_data_len, i, k;
signed16      retval;
boolean       hex_string;
char   	     *value;
char	      full_name[GC_T_MAX_ATTR_NAME + 1];
char          str[GC_T_MAX_VAR + 1];
char  	     *conv_data;

    hex_string = FALSE;
    retval = GC_SUCCESS;

    /* if the local bit is string, force the syntax to be printable */
    if (syntax & OM_S_LOCAL_STRING)
    {
	omsyntax = OM_S_PRINTABLE_STRING;
    }
    else
    {
	/* force the high bits to 0 */
	omsyntax = syntax & OM_S_SYNTAX;
    }

    switch (omsyntax)
    {
	/* if the syntax is any of these, then copy the values from */
	/* the length member of the OM_string.			    */
	case (OM_S_INTEGER):
	case (OM_S_BOOLEAN):
	case (OM_S_ENUMERATION):
	    sprintf(str, "%ld", data.length);
	    conv_data = str;
	    conv_data_len = strlen(str);
	    break;
	case (OM_S_OCTET_STRING):
	    /* These values are printed as hex strings. Each character */
	    /* occupies four bytes - "\xnn".			       */
	    conv_data_len = data.length * 4;
	    hex_string = TRUE;
	    break;
	case (OM_S_OBJECT_IDENTIFIER_STRING):
	    /* try to get the full name */
	    retval_xoi = xoi_get_objclass_name(&data, NULL, full_name);

	    if (retval_xoi == XOI_SUCCESS)
	    {
		conv_data = full_name;
		conv_data_len = strlen(full_name);
	    }
	    else
	    {
		/* These values are printed as hex strings. Each character */
		/* occupies four bytes - "\xnn".		           */
		conv_data_len = data.length * 4;
		hex_string = TRUE;
	    }
	    break;
	default:
	    conv_data = (char *) (data.elements);
	    conv_data_len = data.length;
	    break;
    }

    /* allocate the area, including the area for end of string */
    value = malloc(sizeof(char) * (conv_data_len + 1));

    if (value != NULL)
    {
	if (hex_string == FALSE)
	{
	    memcpy(value, conv_data, conv_data_len);
	}
	else
	{
	    k = 0;
	    for (i = 0; i < data.length; ++i)
	    {
		value[k] = GC_T_ESCAPE_OP;
		++k;
		value[k] = GC_T_HEX_OP;
		++k;
		sprintf(&value[k], "%02x", ((char *) (data.elements))[i]);
		k += 2;
	    }
	}

	/* put the EOS character */
	value[conv_data_len] = 0;

	result_vector[vector_index].type = vector_type;
	result_vector[vector_index].value = value;
    }
    else
    {
	retval = GC_T_MEMORY_INSUFFICIENT;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_process_entry_info			   */
/* Description   : This function is used to process the result     */
/*		   contained in the entry information and create   */
/*		   the result vector.		   		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_process_entry_info(
    OM_public_object   entry_info,    /* IN  - The entry information object   */
    boolean            abbrv_result,  /* IN  - To indicates abbreviated result*/
    boolean            local_strings, /* IN  - To specify local strings       */
    unsigned32        *vectors_used,  /* OUT - No. of vectors used for result */
    gc_t_vector      **result_vector, /* OUT - Result vector		      */
    gc_t_vector      **err_vector)    /* OUT - Error vector in case of errors */
{
OM_object   entry_obj, dn_obj, attr_obj;
unsigned32  vect_index;
unsigned32  i, j;
signed16    retval;

    retval = GC_SUCCESS;
    vect_index = 0;

    for (i = 0; entry_info[i].type != OM_NO_MORE_TYPES; ++i)
    {
	switch(entry_info[i].type)
	{
	    case (DS_ENTRIES):
	    case (DS_ENTRY):
		entry_obj = entry_info[i].value.object.object;
		for (j = 0; entry_obj[j].type != OM_NO_MORE_TYPES; ++j)
		{
		    switch(entry_obj[j].type)
		    {
			case (DS_OBJECT_NAME):
			    dn_obj = entry_obj[j].value.object.object;
			    /* convert the dn object to strings */
			    retval = gc_t__process_result_dn(dn_obj, 
							     GC_T_DN,
							     local_strings,
							     &vect_index,
							     result_vector,
							     err_vector);
			    break;
			case (DS_ATTRIBUTES):
			    attr_obj = entry_obj[j].value.object.object;

			    /* if abbreviated result is not desired */
			    if (abbrv_result != TRUE)
			    {
				/* convert the attribute object to strings */
				retval = gc_t__process_result_attr(attr_obj,
								  local_strings,
								   &vect_index,
							          result_vector,
								   err_vector);
			    }
			    else
			    {
				/* convert attribute objects to attr.info */
				retval = gc_t__create_attr_info(attr_obj,
								local_strings,
								&vect_index,
								result_vector,
								err_vector);
			    }
			    break;
			default:
			    break;
		    }

		    if (retval != GC_SUCCESS)
		    {
			break;
		    }
		}
		break;
	    default:
		break;
	}

	if (retval != GC_SUCCESS)
	{
	    break;
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* fill the last vector with GC_T_EOL */
	(*result_vector)[vect_index].type = GC_T_EOL;
	(*result_vector)[vect_index].value = 0;

	if (vectors_used != NULL)
	{
	    *vectors_used = vect_index + 1;
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__process_result_dn			   */
/* Description   : This function is used to process the DN object  */
/*		   in the search/read/list result.  	 	   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__process_result_dn(
    OM_object        dn_object,     /* IN  - The Distinguished Name object  */
    gc_t_vect_type   vect_type,     /* IN  - Type of vector                 */
    boolean          local_strings, /* IN  - To specify local strings       */
    unsigned32      *vect_index,    /* I/O - Index into result vector	    */
    gc_t_vector    **result_vector, /* OUT - Result vector		    */
    gc_t_vector    **err_vector)    /* OUT - Error vector in case of errors */
{
OM_return_code  om_ret_code;
OM_boolean	request_local_strings;
OM_string	dn_omstring;
char		dn_string[GC_T_MAX_DN_STRING + 1];
signed16  	retval;

    retval = GC_SUCCESS;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    dn_omstring.elements = (void *) &dn_string;
    dn_omstring.length = GC_T_MAX_DN_STRING + 1;

    /* convert the object to string */
    om_ret_code = omX_object_to_string(dn_object, request_local_strings, 
				       &dn_omstring);

    if (om_ret_code == OM_SUCCESS)
    {
	/* put the EOS character */
	((char *) (dn_omstring.elements))[dn_omstring.length] = 0;

	/* fill the dn string into the result vector */
	retval = gc_t_create_result_vector_data(*result_vector, *vect_index, 
						vect_type, dn_omstring,
						OM_S_PRINTABLE_STRING);

	if (retval == GC_SUCCESS)
	{
	    ++*vect_index;
	}
	else
	{
	    /* create the error vector */
	    gc_t_create_gct_error_vector(retval, NULL, err_vector);
	}
    }
    else
    {
	/* map the error to the translator defined error and create */
	/* error vector 					    */
	retval = gc_t_map_xom_error(om_ret_code, err_vector);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__process_result_attr			   */
/* Description   : This function is used to process the attribute  */
/*		   object in read/search result.	           */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__process_result_attr(
    OM_object      attr_object,   /* IN  - The DS_C_ATTRIBUTE object      */
    boolean        local_strings, /* IN  - To specify local strings       */
    unsigned32    *vect_index,    /* I/O - Index into result vector	  */
    gc_t_vector  **result_vector, /* OUT - Result vector		  */
    gc_t_vector  **err_vector)    /* OUT - Error vector in case of errors */
{
OM_integer      retval_xoi;
OM_object	class_object;
OM_syntax	attr_syntax;
OM_string      *attr_objid;
OM_string       attr_name_string;
gc_t_vect_type  vector_type;
unsigned32   	i;
signed16   	retval; 
boolean		object_is_dn;
char  	 	attr_name[GC_T_MAX_ATTR_NAME + 1];

    object_is_dn = FALSE;
    retval = GC_SUCCESS;

    /* locate the attribute object identifier */
    for (i = 0; attr_object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (attr_object[i].type == DS_ATTRIBUTE_TYPE)
	    break;
    }

    if (attr_object[i].type != DS_ATTRIBUTE_TYPE)
    {
	/* map the XOM error to the translator defined */
	/* error and create the error string.          */
	retval = gc_t_map_xom_error(OM_NO_SUCH_OBJECT, err_vector);
    }
    else
    {
	attr_objid = &(attr_object[i].value.string);

	/* locate the attribute values */
	for (i = 0; attr_object[i].type != OM_NO_MORE_TYPES; ++i)
	{
	    if (attr_object[i].type == DS_ATTRIBUTE_VALUES)
		break;
	}

	/* if no attribute values are present then request is for attribute */
	/* types only.							    */
	if (attr_object[i].type != DS_ATTRIBUTE_VALUES)
	{
	    vector_type = GC_T_ATTRNAME;
	}
	else
	{
	    if ((attr_object[i].syntax & OM_S_SYNTAX) == OM_S_OBJECT)
	    {
		class_object = attr_object[i].value.object.object;

		/* if object is of type DS_C_DS_DN, treat it as a normal */
		/* attribute object.					 */
		if (class_object[0].value.string.length == DS_C_DS_DN.length &&
		    memcmp(class_object[0].value.string.elements, 
			   DS_C_DS_DN.elements, DS_C_DS_DN.length) == 0)
		{
		    object_is_dn = TRUE;
		    vector_type = GC_T_ATTRNAME;
		}
		else
		{
		    vector_type = GC_T_SATTRNAME;
		}
	    }
	    else
	    {
		vector_type = GC_T_ATTRNAME;
	    }
	}

	/* get the attribute full name */
	retval_xoi = xoi_get_attr_name(attr_objid, NULL, attr_name);

	if (retval_xoi != XOI_SUCCESS)
	{
	    /* since unable to get full name, convert the object identifier */
	    /* to its dotted representation.			            */
	    retval_xoi = xoi_ber_to_string(attr_objid, attr_name);
	}

	if (retval_xoi == XOI_SUCCESS)
	{
	    attr_name_string.elements = (void *) attr_name;
	    attr_name_string.length = strlen(attr_name);
	    /* fill the attribute name string into the result vector */
	    retval = gc_t_create_result_vector_data(*result_vector, *vect_index,
						    vector_type, 
						    attr_name_string, 
						    OM_S_PRINTABLE_STRING);
	    if (retval == GC_SUCCESS)
	    {
		++*vect_index;
	    }
	    else
	    {
		/* create the error vector */
		gc_t_create_gct_error_vector(retval, NULL, err_vector);
	    }
	}
	else
	{
	    /* map the error to translator defined error and create the */
	    /* error vector.					        */
	    retval = gc_t_map_xoi_error(retval_xoi, NULL, err_vector);
	}
    }
    
    if (retval == GC_SUCCESS)
    {
	for (i = 0; attr_object[i].type != OM_NO_MORE_TYPES; ++i)
	{
	    if (attr_object[i].type == DS_ATTRIBUTE_VALUES)
	    {
		if ((attr_object[i].syntax & OM_S_SYNTAX) == OM_S_OBJECT)
		{
		    class_object = attr_object[i].value.object.object;

		    /* if the object is of type DS_C_DS_DN */
		    if (object_is_dn == TRUE)
		    {
			/* Treat it as a normal component and convert the */
			/* dn object to string.				  */
			retval = gc_t__process_result_dn(class_object,
							 GC_T_ATTRVAL,
							 local_strings,
							 vect_index,
							 result_vector,
							 err_vector);
		    }
		    else
		    {
			retval = gc_t__process_result_struct_attr(class_object,
								  local_strings,
								  vect_index,
								  result_vector,
								  err_vector);
		    }
		    if (retval != GC_SUCCESS)
		    {
			break;
		    }
		}
		else
		{
		    attr_syntax = attr_object[i].syntax;

		    /* fill the attribute name string into the result vector */
		    retval = gc_t_create_result_vector_data(*result_vector,
							    *vect_index, 
							    GC_T_ATTRVAL,
						    attr_object[i].value.string,
							    attr_syntax);
		    if (retval == GC_SUCCESS)
		    {
			++*vect_index;
		    }
		    else
		    {
			/* create the error vector */
			gc_t_create_gct_error_vector(retval, NULL, err_vector);
			break;
		    }
		}
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__process_result_struct_attr		   */
/* Description   : This function is used to process the structured */
/*		   attribute object in the read/search result.     */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__process_result_struct_attr(
    OM_object      class_object,  /* IN  - The class object               */
    boolean        local_strings, /* IN  - To specify local strings       */
    unsigned32    *vect_index,    /* I/O - Index into result vector	  */
    gc_t_vector  **result_vector, /* OUT - Result vector		  */
    gc_t_vector  **err_vector)    /* OUT - Error vector in case of errors */
{
OM_integer      retval_xoi;
OM_object	comp_object;
OM_string       class_id;
OM_syntax       comp_syntax;
OM_string       comp_name_string;
gc_t_vect_type  vector_type;
signed16        retval;
signed16  	i;
boolean         object_is_dn;
char  	        comp_name[GC_T_MAX_ATTR_NAME + 1];

    object_is_dn = FALSE;
    retval = GC_SUCCESS;

    /* get the class identifier which is in the 0th descriptor */
    class_id.elements = class_object[0].value.string.elements;
    class_id.length = class_object[0].value.string.length;

    /* skip through the first class identifier */
    for (i = 1; class_object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if ((class_object[i].syntax & OM_S_SYNTAX) == OM_S_OBJECT)
	{
	    comp_object = class_object[i].value.object.object;

	    /* if the object is of type DS_C_DS_DN, treat it as a normal */
	    /* component.					         */
	    if (comp_object[0].value.string.length == DS_C_DS_DN.length &&
		memcmp(comp_object[0].value.string.elements,
		       DS_C_DS_DN.elements, DS_C_DS_DN.length) == 0)
	    {
		object_is_dn = TRUE;
		vector_type = GC_T_COMPNAME;
	    }
	    else
	    {
		vector_type = GC_T_SCOMPNAME;
	    }
	}
	else
	{
	    vector_type = GC_T_COMPNAME;
	}

	/* get the full name of the component */
	retval_xoi = xoi_get_comp_name(&class_id, class_object[i].type,
				       NULL, comp_name);
	if (retval_xoi == GC_SUCCESS)
	{
	    comp_name_string.elements = (void *) comp_name;
	    comp_name_string.length = strlen(comp_name);

	    /* fill the component name string into the result vector */
	    retval = gc_t_create_result_vector_data(*result_vector, 
						    *vect_index,
						    vector_type,
						    comp_name_string,
						    OM_S_PRINTABLE_STRING);
	    if (retval == GC_SUCCESS)
	    {
		++*vect_index;

		/* if the component is further structured */
		if ((class_object[i].syntax & OM_S_SYNTAX) == OM_S_OBJECT)
		{
		    comp_object = class_object[i].value.object.object;
		    if (object_is_dn == TRUE)
		    {
			/* Treat it as a normal component and convert the */
			/* dn object to string.				  */
			gc_t__process_result_dn(comp_object, GC_T_COMPVAL,
						local_strings, vect_index, 
						result_vector, err_vector);
		    }
		    else
		    {
			/* component is further structured */
			/* call this function recursively  */
			retval = gc_t__process_result_struct_attr(comp_object,
								  local_strings,
								  vect_index,
								  result_vector,
								  err_vector);

			if (retval == GC_SUCCESS)
			{
			    /* initialize vector to structured component end */
			    (*result_vector)[*vect_index].type = GC_T_SCOMPEND;
			    (*result_vector)[*vect_index].value = 0;
			    ++*vect_index;
			}
			else
			{
			    break;
			}
		    }
		}
		else
		{
		    /* for P-Selector, S-Selector and T-Selector     */
		    /* set the syntax to local string so that values */
		    /* are not printed in the octet form.	     */
		    if (class_object[i].type == DS_P_SELECTOR ||
			class_object[i].type == DS_S_SELECTOR ||
			class_object[i].type == DS_T_SELECTOR)
		    {
		       comp_syntax = class_object[i].syntax | OM_S_LOCAL_STRING;
		    }
		    else
		    {
			comp_syntax = class_object[i].syntax;
		    }

		    /* fill the component value string into the result vector */
		    retval = gc_t_create_result_vector_data(*result_vector, 
							    *vect_index,
							    GC_T_COMPVAL, 
						   class_object[i].value.string,
							    comp_syntax);
		    if (retval == GC_SUCCESS)
		    {
			++*vect_index;
		    }
		    else
		    {
			/* create the error vector */
			gc_t_create_gct_error_vector(retval, NULL, err_vector);
			break;
		    }
		}
	    }
	    else
	    {
		/* create the error vector */
		gc_t_create_gct_error_vector(retval, NULL, err_vector);
		break;
	    }
	}
	else
	{
	    /* map the error to translator defined error and create the */
	    /* error vector.					        */
	    retval = gc_t_map_xoi_error(retval_xoi, NULL, err_vector);
	    break;
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__create_attr_info			   */
/* Description   : This function is used to process the attribute  */
/*		   object in read/search result and create the     */
/*		   result vector in an abbreviated form (like c=de)*/
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__create_attr_info(
    OM_object      attr_object,   /* IN  - The DS_C_ATTRIBUTE object      */
    boolean        local_strings, /* IN  - To specify local strings       */
    unsigned32    *vect_index,    /* I/O - Index into result vector	  */
    gc_t_vector  **result_vector, /* OUT - Result vector		  */
    gc_t_vector  **err_vector)    /* OUT - Error vector in case of errors */
{
OM_return_code  om_ret_code;
OM_integer	retval_xoi;
OM_boolean	request_local_strings;
OM_string  	attr_info_string;
unsigned32      i;
signed16        retval;
char       	attr_info[GC_T_MAX_AINFO_STRING + 1];
char       	abbrv[GC_T_MAX_OBJID + 1]; /* provide for GC_T_MAX_OBJID */

    retval = GC_SUCCESS;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    /* locate the attribute values */
    for (i = 0; attr_object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (attr_object[i].type == DS_ATTRIBUTE_VALUES)
	    break;
    }

    if (attr_object[i].type == DS_ATTRIBUTE_VALUES)
    {
	attr_info_string.elements = (void *) attr_info;
	attr_info_string.length = GC_T_MAX_AINFO_STRING + 1;

	/* convert the DS_C_ATTRIBUTE object to string */
	om_ret_code = omX_object_to_string(attr_object, request_local_strings, 
					   &attr_info_string);
	if (om_ret_code == OM_SUCCESS)
	{
	    /* put the EOS character */
	    ((char *) (attr_info_string.elements))[attr_info_string.length] = 0;

	    retval = gc_t_create_result_vector_data(*result_vector, *vect_index,
						    GC_T_ATTRINFO, 
						    attr_info_string,
						    OM_S_PRINTABLE_STRING);
	    if (retval == GC_SUCCESS)
	    {
		++*vect_index;
	    }
	    else
	    {
		/* create the error vector */
		gc_t_create_gct_error_vector(retval, NULL, err_vector);
	    }
	}
	else
	{
	    /* map the XOM error to the translator defined */
	    /* error and create the error string.          */
	    retval = gc_t_map_xom_error(om_ret_code, err_vector);
	}
    }
    else
    {
	/* since no attribute values found, request must be only for */
	/* attribute types.					     */
	/* Locate the attribute type.				     */
	for (i = 0; attr_object[i].type != OM_NO_MORE_TYPES; ++i)
	{
	    if (attr_object[i].type == DS_ATTRIBUTE_TYPE)
		break;
	}

	if (attr_object[i].type != DS_ATTRIBUTE_TYPE)
	{
	    /* map the XOM error to the translator defined */
	    /* error and create the error string.          */
	    retval = gc_t_map_xom_error(OM_NO_SUCH_OBJECT, err_vector);
	}
	else
	{
	    /* convert the object identifier to abbreviation */
	    retval_xoi = xoi_get_attr_name(&(attr_object[i].value.string), 
					   abbrv, NULL);
	    if (retval_xoi != XOI_SUCCESS)
	    {
		/* since unable to get abbreviation, convert the object */
		/* identifier to its dotted representation.		*/
		retval_xoi = xoi_ber_to_string(&(attr_object[i].value.string),
					       abbrv);
	    }

	    if (retval_xoi == XOI_SUCCESS)
	    {
		attr_info_string.elements = (void *) abbrv;
		attr_info_string.length = strlen(abbrv);
		/* fill the attribute abbreviation string into result vector */
		retval = gc_t_create_result_vector_data(*result_vector,
							*vect_index,
							GC_T_ATTRABBRV, 
							attr_info_string,
							OM_S_PRINTABLE_STRING);
		if (retval == GC_SUCCESS)
		{
		    ++*vect_index;
		}
		else
		{
		    /* create the error vector */
		    gc_t_create_gct_error_vector(retval, NULL, err_vector);
		}
	    }
	    else
	    {
		/* map the error to translator defined error and create the */
		/* error vector.					    */
		retval = gc_t_map_xoi_error(retval_xoi, NULL, err_vector);
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_get_entry_info_vect_count		   */
/* Description   : This function is used to scan the entry info.   */
/*		   object and determine the number of vectors      */
/*		   required to fill the result.			   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_t_get_entry_info_vect_count(
    OM_public_object   entry_info,   /* IN  - The entry information object   */
    boolean            abbrv_result, /* IN  - To indicates abbreviated result*/
    unsigned32        *vect_count)   /* OUT - Count of required vectors	     */
{
OM_object   attr_obj, entry_obj;
unsigned32  dn_count, attr_type_count, attr_val_count;
unsigned32  comp_type_count, comp_val_count;
unsigned32  scomp_type_count;
unsigned32  i, j, k;

    dn_count = attr_type_count = attr_val_count = 0;
    comp_type_count = comp_val_count = scomp_type_count = 0;

    for (i = 0; entry_info[i].type != OM_NO_MORE_TYPES; ++i)
    {
	switch(entry_info[i].type)
	{
	    case (DS_ENTRIES):
	    case (DS_ENTRY):
		entry_obj = entry_info[i].value.object.object;
		for (j = 0; entry_obj[j].type != OM_NO_MORE_TYPES; ++j)
		{
		    switch(entry_obj[j].type)
		    {
			case (DS_OBJECT_NAME):
			    ++dn_count;
			    break;
			case (DS_ATTRIBUTES):
			    attr_obj = entry_obj[j].value.object.object;
			    for (k = 0; attr_obj[k].type != OM_NO_MORE_TYPES; 
				 ++k)
			    {
				switch(attr_obj[k].type)
				{
				    case (DS_ATTRIBUTE_TYPE):
					++attr_type_count;
					break;
				    case (DS_ATTRIBUTE_VALUES):
					/* if abbreviated result is required, */
					/* only the DS_ATTRIBUTE_TYPE should  */
					/* be counted.			      */
					if (abbrv_result != TRUE)
					{
					    /* if structured attribute, get */
					    /* the component count.	    */
					    if ((attr_obj[k].syntax 
						 & OM_S_SYNTAX) == OM_S_OBJECT)
					    {
						gc_t__get_class_comp_count(
						attr_obj[k].value.object.object,
					        &comp_type_count, 
						&comp_val_count, 
						&scomp_type_count);
					    }
					    else
					    {
						++attr_val_count;
					    }
					}
					break;
				    default:
					break;
				}
			    }
			    break;
			default:
			    break;
		    }
		}
		break;
	    default:
		break;
	}
    }

    /* Update the total count of vectors required.			      */
    /* For every occurance of structured component type i.e. GC_T_SCOMPNAME   */
    /* there should be one more vector to indicate GC_T_SCOMPEND. Hence,      */
    /* 2 * scomp_type_count. One more vector is required to indicate GC_T_EOL */
    *vect_count = dn_count + attr_type_count + attr_val_count +
		  comp_type_count + comp_val_count + 2 * scomp_type_count + 1;
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__get_class_comp_count			   */
/* Description   : This function is used to scan the class object  */
/*		   containing the attribute value and determine    */
/*		   the total number of components.		   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_t__get_class_comp_count(
    OM_object   class,		   /* IN  - The class object           */
    unsigned32 *comp_type_count,   /* OUT - Number of Component types  */
    unsigned32 *comp_val_count,    /* OUT - Number of Component values */
    unsigned32 *scomp_type_count)  /* OUT - No.of struct.comp.types    */
{
unsigned32 i;

    for (i = 0; class[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if ((class[i].syntax & OM_S_SYNTAX) == OM_S_OBJECT)
	{
	    /* if component is further structured call function recursively */
	    gc_t__get_class_comp_count(class[i].value.object.object,
				       comp_type_count, comp_val_count,
				       scomp_type_count);
	    ++*scomp_type_count;
	    continue;
	}

	switch(class[i].type)
	{
	    case (OM_CLASS):
		break;
	    default:
		/* Both the component type and value are specified in the */
		/* same descriptor.					  */
		++*comp_type_count;
		++*comp_val_count;
		break;
	}
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_get_partial_vect_count	   	           */
/* Description   : This function is used to return a count of the  */
/*		   number of vectors required to process the       */
/*		   partial outcome qualifier returned after a      */
/*		   search operation 				   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_t_get_partial_vect_count(
    OM_object    partial_obj,  /* IN  - The partial outcome qualifier object*/
    unsigned16  *vect_count)   /* OUT - Count of the number of vectors      */
{
OM_object   referral;
unsigned16  i, m;

    *vect_count = 0;

    for (i = 0; partial_obj[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (partial_obj[i].type == DS_LIMIT_PROBLEM &&
	    partial_obj[i].value.enumeration != DS_NO_LIMIT_EXCEEDED)
	{
	    ++*vect_count;
	    continue;
	}

	if (partial_obj[i].type == DS_UNAVAILABLE_CRIT_EXT &&
	    partial_obj[i].value.boolean == OM_TRUE)
	{
	    ++*vect_count;
	    continue;
	}

	if (partial_obj[i].type == DS_UNEXPLORED)
	{
	    referral = partial_obj[i].value.object.object;

	    for (m = 0; referral[m].type != OM_NO_MORE_TYPES; ++m)
	    {
		if (referral[m].type == DS_ACCESS_POINTS)
		{
		    /* 2 vectors are required for this - one for the */
		    /* referral DSA name and one for PSAP address.   */
		    *vect_count += 2;
		}
	    }
	}
    }

    if (*vect_count > 0)
    {
	/* one more vector is required for GC_T_EOL */
	++*vect_count;
    }
}

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_process_partial_qual_object	   	   */
/* Description   : This function is used to process the 	   */
/*		   DS_C_PARTIAL_OUTCOME_QUAL object and create     */
/*		   the appropriate result vectors.		   */
/*		   This function is invoked when there is a        */
/*		   partial outcome qualifier accompanying the 	   */
/*		   result in case of the search and list operations*/
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_process_partial_qual_object(
    OM_object      partial_obj,  /* IN  - The partial outcome qualifier object*/
    boolean        local_strings,/* IN  - To specify local strings            */
    gc_t_vector   *result_vect,  /* I/O - The result vector	      	      */
    gc_t_vector  **error_vect)   /* OUT - Error message in case of errors     */
{
gc_t_vect_type  vector_type;
signed16	retval;
signed16	i, n;
char	       *msg;

    n = 0;
    retval = GC_SUCCESS;

    for (i = 0; partial_obj[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (partial_obj[i].type == DS_LIMIT_PROBLEM ||
	    partial_obj[i].type == DS_UNAVAILABLE_CRIT_EXT)
	{
	    msg = NULL;

	    /* if there is a limit problem */
	    if (partial_obj[i].type == DS_LIMIT_PROBLEM)
	    {
		switch (partial_obj[i].value.enumeration)
		{
		    case (DS_ADMIN_LIMIT_EXCEEDED):
			vector_type = GC_T_LIMIT_PROBLEM;
			msg = (char *) dce_sprintf(gc_t_admin_limit_exceeded);
			break;
		    case (DS_SIZE_LIMIT_EXCEEDED):
			vector_type = GC_T_LIMIT_PROBLEM;
			msg = (char *) dce_sprintf(gc_t_size_limit_exceeded);
			break;
		    case (DS_TIME_LIMIT_EXCEEDED):
			vector_type = GC_T_LIMIT_PROBLEM;
			msg = (char *) dce_sprintf(gc_t_time_limit_exceeded);
			break;
		    default:
			break;
		}
	    }
	    else
	    {
		if (partial_obj[i].type == DS_UNAVAILABLE_CRIT_EXT &&
		    partial_obj[i].value.boolean == OM_TRUE)
		{
		    vector_type = GC_T_NO_CRIT_EXT;
		    msg = (char *) dce_sprintf(gc_t_unavailable_crit_ext);
		}
	    }

	    if (msg != NULL)
	    {
		result_vect[n].type = vector_type;
		result_vect[n].value = msg;

		++n;
	    }

	    continue;
	}

	/* if there is a continuation referral */
	if (partial_obj[i].type == DS_UNEXPLORED)
	{
	    /* process the continuation reference object */
	    retval = gc_t__process_cont_ref_obj(
					     partial_obj[i].value.object.object,
					     local_strings, result_vect, 
					     error_vect);
	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    /* update the vector count. Two vectors would have been used */
	    /* one for the DSA Name and the other for the PSAP address.  */
	    n += 2;
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* initialize the vector with GC_T_EOL */
	result_vect[n].type = GC_T_EOL;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__process_cont_ref_obj	   	           */
/* Description   : This function is used to process the 	   */
/*		   DS_ACCESS_POINTS attribute of a 		   */
/*		   DS_C_CONTINUATION_REF object and create         */
/*		   appropriate result vectors.			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__process_cont_ref_obj(
    OM_object      ref_object,   /* IN  - The continuation reference object */
    boolean        local_strings,/* IN  - To specify local strings          */
    gc_t_vector   *result_vect,  /* I/O - The result vector         	    */
    gc_t_vector  **error_vect)   /* OUT - Error message in case of errors   */
{
OM_public_object  access_object;
signed16  	  retval;
signed16	  i, n;

    retval = GC_SUCCESS;;
    n = 0;

    for (i = 0; ref_object[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (ref_object[i].type == DS_ACCESS_POINTS)
	{
	    access_object = ref_object[i].value.object.object;

	    /* process the referral DSA name and the PSAP address in the */
	    /* access point object.                                      */
	    retval = gc_t_process_access_points(access_object, local_strings,
						&result_vect[n], error_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    /* update the vector index. Two vectors have been used - one for */
	    /* the Referral DSA Name and the other for the PSAP address.     */
	    n += 2;
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_process_access_points		   	   */
/* Description   : This function processes the DS_C_ACCESS_POINT   */
/*		   object and creates the vectors containing the   */
/*		   Referral DSA name and PSAP Address.		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.                              */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_process_access_points(
    OM_object     access_object,  /* IN  - The access point object	  */
    boolean       local_strings,  /* IN  - To specify local strings       */
    gc_t_vector  *result_vect,    /* I/O - The result vector  	  	  */
    gc_t_vector **error_vect)     /* OUT - Error vector in case of errors */
{
OM_return_code  om_ret_code;
OM_boolean      request_local_strings;
OM_string       om_string;
signed16        retval;
signed16        k;
char	        value_string[GC_T_MAX_AINFO_STRING + 1];

    retval = GC_SUCCESS;
    om_ret_code = OM_SUCCESS;

    if (local_strings == TRUE)
    {
	request_local_strings = OM_TRUE;
    }
    else
    {
	request_local_strings = OM_FALSE;
    }

    /* first process the referral dsa name */
    for (k = 0; access_object[k].type != OM_NO_MORE_TYPES; ++k)
    {
	if (access_object[k].type == DS_AE_TITLE)
	{
	    om_string.elements = value_string;
	    om_string.length = GC_T_MAX_AINFO_STRING + 1;

	    /* convert the name to string format */
	    om_ret_code = omX_object_to_string(
					   access_object[k].value.object.object,
					   request_local_strings, &om_string);

	    if (om_ret_code != OM_SUCCESS)
	    {
		/* map the XOM error to the translator defined */
		/* error and create the error string.          */
		retval = gc_t_map_xom_error(om_ret_code, error_vect);
		break;
	    }

	    /* put the EOS character */
	    ((char *) (om_string.elements))[om_string.length] = 0;

	    /* fill the string into the vector */
	    retval = gc_t_create_result_vector_data(result_vect, 0, 
						    GC_T_REF_DSA, om_string,
						    OM_S_PRINTABLE_STRING);
	    if (retval != GC_SUCCESS)
	    {
		/* create the error vector */
		retval = gc_t_create_gct_error_vector(retval, NULL, error_vect);
		break;
	    }

	    break;
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* now process the referral dsa psap address */
	for (k = 0; access_object[k].type != OM_NO_MORE_TYPES; ++k)
	{
	    if (access_object[k].type == DS_ADDRESS)
	    {
		om_string.elements = value_string;
		om_string.length = GC_T_MAX_AINFO_STRING + 1;

		/* convert the psap address to string format */
		om_ret_code = omX_object_to_string(
					   access_object[k].value.object.object,
					   OM_TRUE, &om_string);

		if (om_ret_code != OM_SUCCESS)
		{
		    /* map the XOM error to the translator defined */
		    /* error and create the error string.          */
		    retval = gc_t_map_xom_error(om_ret_code, error_vect);
		    break;
		}

		/* put the EOS character */
		((char *) (om_string.elements))[om_string.length] = 0;

		/* fill the string into the vector */
		retval = gc_t_create_result_vector_data(result_vect, 1, 
						 	GC_T_REF_ADDR, 
							om_string,
						        OM_S_PRINTABLE_STRING);
		if (retval != GC_SUCCESS)
		{
		    /* create the error vector */
		    retval = gc_t_create_gct_error_vector(retval, NULL,
							  error_vect);
		    break;
		}

		break;
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_free_result			   	   */
/* Description   : This function is used to free the result vector */
/*		   created by the GDSCP Translator.		   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_t_free_result(
    gc_t_vector  *res_vector)  /* IN - The result vector to be freed */
{
unsigned32 i;

    if (res_vector != NULL)
    {
	for (i = 0; res_vector[i].type != GC_T_EOL; ++i)
	{
	    if (res_vector[i].type != GC_T_SCOMPEND)
	    {
		free(res_vector[i].value);
	    }
        }

        free(res_vector);
    }
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
