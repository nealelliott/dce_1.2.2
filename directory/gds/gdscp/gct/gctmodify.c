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
 * $Log: gctmodify.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:49  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:24  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:48  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:44  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:11  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:55:15  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:36:23  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:53  keutel
 * 	creation
 * 	[1994/03/21  12:56:41  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctaddattr.c					   */
/* Description : This file contains all the modules for the	   */
/*		 directory modify attribute operation.		   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctmodify.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:58 $";
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

#include <xomext.h>

#include <xoi.h>

#include <gdscp.h>
#include <gctstruct.h>
#include <gctconv.h>
#include <gcterr.h>
#include <gctutil.h>
#include <gctargs.h>
#include <gctbind.h>
#include <gctmoddsc.h>

/* import the necessary Object Identifier constants */
OM_IMPORT(DS_C_ENTRY_MOD_LIST)
OM_IMPORT(DS_C_ENTRY_MOD)
OM_IMPORT(DS_C_ATTRIBUTE)

/*-- Static Function(s) -------------------------------------------*/
static signed16 gc_t__modify_attributes(
    char         **in_vect,         /* IN  - Array of input vectors          */
    signed16       vect_count,      /* IN  - Count of input vectors          */
    signed16	   attr_info_count, /* IN  - Count of Attribute Info.strings */
    boolean        local_strings,   /* IN  - To specify local strings        */
    gc_t_vector  **out_vect);       /* OUT - NULL (success) or error message */

static signed16 gc_t__create_entry_mod(
    char 	   *attr_info,    /* IN  - Input attribute information string */
    OM_enumeration  ds_mod_type,  /* IN  - The Modification type              */
    boolean         local_strings,/* IN  - To specify local strings           */
    OM_object      *entry_mod,    /* OUT - The created object.		      */
    gc_t_vector   **error_vect);  /* OUT - Error message in case of errors    */

static signed16 gc_t__perform_modify(
    boolean            is_addattr_op, /* IN  - Indicates if add attr.operation*/
    OM_private_object  session,       /* IN  - The session object	      */
    OM_private_object  context,	      /* IN  - The context object	      */
    OM_private_object  name,	      /* IN  - The name object	      	      */
    OM_object	       entry_mod_list,/* IN  - DS_C_ENTRY_MOD_LIST object     */
    boolean            local_strings, /* IN  - To specify local strings       */
    gc_t_vector      **error_vect);   /* OUT - Error message in case of errors*/

static void gc_t__free_entry_mod_list(
    OM_object  entry_mod_list); /* IN - The object to be freed */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_modify					   */
/* Description   : This function is used to perform a directory	   */
/*		   modify attribute operation.			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_modify(
    char         **in_vect,       /* IN  - Array of input vectors          */
    signed16       vect_count,    /* IN  - Count of input vectors          */
    boolean        local_strings, /* IN  - To specify local strings        */
    gc_t_vector  **out_vect)      /* OUT - The result or the error message */
{
OM_private_object  session;
boolean	   	   is_addattr_op, is_modattr_op, is_delattr_op, is_modrdn_op;
boolean	   	   is_present;
signed16	   retval;
signed16	   attr_info_count;
signed16	   i;

    is_addattr_op = is_modattr_op = is_delattr_op = is_modrdn_op = FALSE;
    attr_info_count = 0;
    retval = GC_SUCCESS;
    i = 0;

    if (out_vect == NULL || vect_count < 0)
	return(GC_T_INVALID_PARAMETER);

    *out_vect = NULL;

    if (vect_count > 0 && in_vect == NULL)
    {
	retval = GC_T_INVALID_PARAMETER;
    }
    else
    {
	/* get the session object */
	if ((session = gc_t_get_bound_session()) == NULL)
	{
	    retval = GC_T_BIND_NOT_MADE;
	    /* create the error vector */
	    gc_t_create_gct_error_vector(retval, NULL, out_vect);
	}
	else
	{
	    /* check if object name is specified in the first input vector */
	    if (i < vect_count && in_vect[0] != NULL && in_vect[0][0] != '-')
	    {
		/* skip past it */
		++i;
	    }

	    for (; i < vect_count && in_vect[i] != NULL; ++i)
	    {
		/* if add attribute switch specified */
		retval = gc_t_check_switch(GC_T_MODIFY, in_vect[i], 
					   GC_T_ADD_ATTR_SWITCH,
					   &is_present, out_vect);

		if (retval != GC_SUCCESS)
		{
		    break;
		}

		if (is_present == TRUE)
		{
		    /* if this switch is repeated, or other switches are */
		    /* also specified, return an error.		         */
		    if (is_addattr_op == TRUE || is_modattr_op == TRUE ||
			is_delattr_op == TRUE || is_modrdn_op == TRUE)
		    {
			retval = GC_T_CONFLICTING_OPTION;
			break;
		    }

		    is_addattr_op = TRUE;

		    /* skip past the switch */
		    ++i;

		    /* check if it is followed by the attribute information */
		    if (i >= vect_count || in_vect[i] == NULL ||
			in_vect[i][0] == '-')
		    {
			retval = GC_T_MISSING_ATTRIBUTE_INFO;
			break;
		    }
		    ++attr_info_count;

		    /* skip past the attribute information */
		    ++i;

		    /* for this switch the attribute information should be */
		    /* specified only once.				   */ 
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

		/* if modify attribute switch specified */
		retval = gc_t_check_switch(GC_T_MODIFY, in_vect[i], 
					   GC_T_MOD_ATTR_SWITCH,
					   &is_present, out_vect);

		if (retval != GC_SUCCESS)
		{
		    break;
		}

		if (is_present == TRUE)
		{
		    /* if other switches are also specified, return an error. */
		    if (is_addattr_op == TRUE || is_delattr_op == TRUE ||
			is_modrdn_op == TRUE)
		    {
			retval = GC_T_CONFLICTING_OPTION;
			break;
		    }

		    is_modattr_op = TRUE;

		    /* skip past the switch */
		    ++i;

		    /* check if it is followed by the attribute information */
		    if (i >= vect_count || in_vect[i] == NULL ||
			in_vect[i][0] == '-')
		    {
			retval = GC_T_MISSING_ATTRIBUTE_INFO;
			break;
		    }

		    ++i;
		    ++attr_info_count;

		    /* check if it is followed by the new attribute info. */
		    if (i >= vect_count || in_vect[i] == NULL ||
			in_vect[i][0] == '-')
		    {
			retval = GC_T_MISSING_NEW_ATTRIBUTE_INFO;
			break;
		    }
		    ++attr_info_count;
		    continue;
		}

		/* if delete attribute switch specified */
		retval = gc_t_check_switch(GC_T_MODIFY, in_vect[i], 
					   GC_T_DEL_ATTR_SWITCH,
					   &is_present, out_vect);

		if (retval != GC_SUCCESS)
		{
		    break;
		}

		if (is_present == TRUE)
		{
		    /* if this switch is repeated, or other switches are */
		    /* also specified, return an error.		         */
		    if (is_addattr_op == TRUE || is_modattr_op == TRUE ||
			is_delattr_op == TRUE || is_modrdn_op == TRUE)
		    {
			retval = GC_T_CONFLICTING_OPTION;
			break;
		    }

		    is_delattr_op = TRUE;

		    /* skip past the switch */
		    ++i;

		    /* check if it is followed by the attribute information */
		    if (i >= vect_count || in_vect[i] == NULL ||
			in_vect[i][0] == '-')
		    {
			retval = GC_T_MISSING_ATTRIBUTE_INFO;
			break;
		    }

		    /* run through the attribute information arguments */
		    for (; i < vect_count && in_vect[i] != NULL; ++i)
		    {
			if (in_vect[i][0] == '-')
			{
			    --i;
			    break;
			}
			++attr_info_count;
		    }

		    continue;
		}

		/* if modify rdn switch specified */
		retval = gc_t_check_switch(GC_T_MODIFY, in_vect[i], 
					   GC_T_MOD_RDN_SWITCH,
					   &is_present, out_vect);

		if (retval != GC_SUCCESS)
		{
		    break;
		}

		if (is_present == TRUE)
		{
		    /* if this switch is repeated, or other switches are */
		    /* also specified, return an error.		         */
		    if (is_addattr_op == TRUE || is_modattr_op == TRUE ||
			is_delattr_op == TRUE || is_modrdn_op == TRUE)
		    {
			retval = GC_T_CONFLICTING_OPTION;
			break;
		    }

		    is_modrdn_op = TRUE;

		    /* skip past the switch */
		    ++i;

		    /* check if it is followed by the attribute information */
		    if (i >= vect_count || in_vect[i] == NULL ||
			in_vect[i][0] == '-')
		    {
			retval = GC_T_MISSING_ATTRIBUTE_INFO;
			break;
		    }
		    ++attr_info_count;

		    /* skip past the attribute information */
		    ++i;

		    /* for this switch the attribute information should be */
		    /* specified only once.				   */ 
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

		/* must be an unknown switch or argument */
		if (in_vect[i][0] == '-')
		{
		    retval = GC_T_UNKNOWN_OPTION;
		}
		else
		{
		    retval = GC_T_UNKNOWN_ARGUMENT;
		}
		break;
	    }
	}
    }

    if (retval == GC_SUCCESS && attr_info_count == 0)
    {
	retval = GC_T_MISSING_ATTRIBUTE_INFO;
    }

    if (retval == GC_SUCCESS)
    {
	/* if it is modify rdn operation */
	if (is_modrdn_op == TRUE)
	{
	    retval = gc_t_modrdn(in_vect, vect_count, local_strings, out_vect);
	}
	else
	{
	    retval = gc_t__modify_attributes(in_vect, vect_count, 
					     attr_info_count, local_strings, 
					     out_vect);
	}
    }
    else
    {
	if (*out_vect == NULL)
	{
	    /* create the error vector */
	    if (i >= vect_count)
	    {
		retval = gc_t_create_gct_arg_error_vector(retval, NULL, 
							  out_vect);
	    }
	    else
	    {
		retval = gc_t_create_gct_arg_error_vector(retval, in_vect[i], 
							  out_vect);
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__modify_attributes			   */
/* Description   : This function is used to perform a directory	   */
/*		   modify attribute operation.			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__modify_attributes(
    char         **in_vect,         /* IN  - Array of input vectors          */
    signed16       vect_count,      /* IN  - Count of input vectors          */
    signed16	   attr_info_count, /* IN  - Count of Attribute Info.strings */
    boolean        local_strings,   /* IN  - To specify local strings        */
    gc_t_vector  **out_vect)        /* OUT - NULL (success) or error message */
{
OM_private_object  session;
OM_private_object  context;
OM_private_object  name;
OM_object	   entry_mod_list;
OM_object	   entry_mod;
boolean	   	   is_addattr_op;
boolean	   	   is_present;
signed16	   desc_count;
signed16	   retval;
signed16	   i, k;
char		  *name_string;

    i = 0;
    name = NULL;
    entry_mod_list = NULL;
    name_string = NULL;
    *out_vect = NULL;
    is_addattr_op = FALSE;
    retval = GC_SUCCESS;

    /* get the session object */
    session = gc_t_get_bound_session();

    /* get the context object */
    context = gc_t_get_context();

    /* compute the total number of descriptors required. 2 more descriptors */
    /* are required - one each for the class identifier and NULL descriptor.*/
    desc_count = attr_info_count + 2;

    entry_mod_list = (OM_descriptor *) malloc(sizeof(OM_descriptor) * 
					      desc_count);
    if (entry_mod_list == NULL)
    {
	retval = GC_T_MEMORY_INSUFFICIENT;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, out_vect);
    }
    else
    {
	/* fill the first descriptor with DS_C_ENTRY_MOD_LIST */
	omX_fill_oid(OM_CLASS, DS_C_ENTRY_MOD_LIST, &entry_mod_list[0]);

	/* initialize the remaining to NULL descriptors */
	for (k = 1; k < desc_count; ++k)
	{
	    GC_T_FILL_OMD_NULL(&entry_mod_list[k]);
	}

	i = 0;

	/* check if the object name is specified in the first input vector. */
	if (in_vect[0][0] != '-')
	{
	    name_string = in_vect[0];
	    ++i;
	}

	for (k = 1; i < vect_count && in_vect[i] != NULL; ++i)
	{
	    /* if the modify attribute is specified */
	    gc_t_check_switch(GC_T_MODIFY, in_vect[i], GC_T_MOD_ATTR_SWITCH,
			      &is_present, out_vect);

	    if (is_present == TRUE)
	    {
		/* skip past the switch */
		++i;

		/* first create the entry for remove attribute */
		/* values operation.		               */
		retval = gc_t__create_entry_mod(in_vect[i], DS_REMOVE_VALUES,
					        local_strings, &entry_mod, 
						out_vect);
		if (retval == GC_SUCCESS)
		{
		    /* fill the descriptor */
		    omX_fill(DS_CHANGES, OM_S_OBJECT, 0, entry_mod, 
			     &entry_mod_list[k]);
		    ++k;
		    ++i;

		    /* now create the entry for add attribute */
		    /* value operation.			      */
		    retval = gc_t__create_entry_mod(in_vect[i], DS_ADD_VALUES,
						    local_strings, &entry_mod, 
						    out_vect);
		    if (retval == GC_SUCCESS)
		    {
			/* fill the descriptor */
			omX_fill(DS_CHANGES, OM_S_OBJECT, 0, entry_mod, 
				 &entry_mod_list[k]);
			++k;
		    }
		}

		if (retval != GC_SUCCESS)
		    break;

		continue;
	    }

	    /* if remove attribute switch specified */
	    gc_t_check_switch(GC_T_MODIFY, in_vect[i], GC_T_DEL_ATTR_SWITCH,
			      &is_present, out_vect);

	    if (is_present == TRUE)
	    {
		/* skip past the switch */
		++i;

		/* run through the attribute information arguments */
		for (; i < vect_count && in_vect[i] != NULL; ++i)
		{
		    if (in_vect[i][0] == '-')
		    {
			--i;
			break;
		    }

		    if (strchr(in_vect[i], GC_T_ATTR_EQUALITY_OP) == NULL)
		    {
			/* if there is no GC_T_ATTR_EQUALITY_OP character in */
			/* the attribute string, then only the attribute     */
			/* abbreviation has been specified.  So the complete */
			/* attribute is to be deleted.			     */

			/* create the DS_C_ENTRY_MOD object */
			retval = gc_t__create_entry_mod(in_vect[i], 
							DS_REMOVE_ATTRIBUTE,
							local_strings, 
							&entry_mod, out_vect);
		    }
		    else
		    {
			/* create the DS_C_ENTRY_MOD object */
			retval = gc_t__create_entry_mod(in_vect[i], 
							DS_REMOVE_VALUES,
							local_strings,
							&entry_mod, out_vect);
		    }

		    if (retval == GC_SUCCESS)
		    {
			/* fill the descriptor */
			omX_fill(DS_CHANGES, OM_S_OBJECT, 0, entry_mod, 
				 &entry_mod_list[k]);
			++k;
			continue;
		    }
		    else
		    {
			break;
		    }
		}

		if (retval != GC_SUCCESS)
		    break;

		continue;
	    }

	    /* if add attribute switch is specified */
	    gc_t_check_switch(GC_T_MODIFY, in_vect[i], GC_T_ADD_ATTR_SWITCH,
			      &is_present, out_vect);

	    if (is_present == TRUE)
	    {
		/* skip past the switch */
		++i;

		/* create the DS_C_ENTRY_MOD object */
		retval = gc_t__create_entry_mod(in_vect[i], DS_ADD_ATTRIBUTE,
						local_strings, &entry_mod, 
						out_vect);
		if (retval == GC_SUCCESS)
		{
		    /* fill the descriptor */
		    omX_fill(DS_CHANGES, OM_S_OBJECT, 0, entry_mod, 
			     &entry_mod_list[k]);
		    ++k;
		    is_addattr_op = TRUE;
		    continue;
		}
		else
		{
		   break;
		}
	    }
	}

	if (retval == GC_SUCCESS)
	{
	    /* convert the string to name */
	    retval = gc_t_string_to_name(name_string, local_strings, &name, 
					 out_vect);

	    if (retval == GC_SUCCESS)
	    {
		/* perform the modify operation */
		retval = gc_t__perform_modify(is_addattr_op, session, context, 
					      name, entry_mod_list, 
					      local_strings, out_vect);
	    }
	}
    }

    /* free the created objects */
    if (name != NULL)
	om_delete(name);

    if (entry_mod_list != NULL)
    {
	gc_t__free_entry_mod_list(entry_mod_list);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__create_entry_mod			   */
/* Description   : This function is used to create an object of	   */
/*		   type DS_C_ENTRY_MOD.			           */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__create_entry_mod(
    char 	   *attr_info,    /* IN  - Input attribute information string */
    OM_enumeration  ds_mod_type,  /* IN  - The Modification type              */
    boolean         local_strings,/* IN  - To specify local strings           */
    OM_object      *entry_mod,    /* OUT - The created object.		      */
    gc_t_vector   **error_vect)   /* OUT - Error message in case of errors    */
{
OM_private_object  attr_object;
OM_private_object  mod_object;
OM_descriptor	   attr_identifier[2] = {OM_NULL_DESCRIPTOR, 
					 OM_NULL_DESCRIPTOR};
OM_descriptor      mod_type[2] = {OM_NULL_DESCRIPTOR, OM_NULL_DESCRIPTOR};
OM_return_code	   om_ret_code;
OM_workspace	   workspace;
OM_integer  	   retval_xoi;
OM_string	   objid;
OM_string	   omstring;
signed16  	   retval;

    mod_object = NULL;
    attr_object = NULL;
    om_ret_code = OM_SUCCESS;
    objid.elements = NULL;
    retval = GC_SUCCESS;

    switch (ds_mod_type)
    {
	case (DS_ADD_ATTRIBUTE):
	case (DS_ADD_VALUES):
	case (DS_REMOVE_VALUES):
	    /* convert the attribute info string to DS_C_ATTRIBUTE object */
	    retval = gc_t_string_to_object(attr_info, DS_C_ATTRIBUTE, 
					   local_strings, &attr_object, 
					   error_vect);
	    break;
	case (DS_REMOVE_ATTRIBUTE):
	    /* get the object identifier from the abbreviation */
	    retval_xoi = xoi_get_attr_details(attr_info, NULL,
					      &(objid.elements),
					      &(objid.length), NULL, NULL);
	    if (retval_xoi != XOI_SUCCESS)
	    {
		omstring.elements = (void *) attr_info;
		omstring.length = strlen(attr_info);

		/* map the error to translator defined error and create the */
		/* error vector.					    */
		retval = gc_t_map_xoi_error(retval_xoi, attr_info, error_vect);
	    }
	    else
	    {
		/* fill the descriptor */
		omX_fill_oid(DS_ATTRIBUTE_TYPE, objid, &attr_identifier[0]);
	    }
	    break;
	default:
	    break;
    }

    if (retval == GC_SUCCESS)
    {
	/* fill the descriptor */
	omX_fill(DS_MOD_TYPE, OM_S_ENUMERATION, ds_mod_type, NULL, 
		 &mod_type[0]);

	/* get the workspace */
	workspace = gc_t_get_workspace();

	/* create the DS_C_ENTRY_MOD object */
	om_ret_code = om_create(DS_C_ENTRY_MOD, OM_FALSE, workspace,
				&mod_object);

	if (om_ret_code == OM_SUCCESS)
	{
	    /* now put the DS_MOD_TYPE attribute */
	    om_ret_code = om_put(mod_object, OM_REPLACE_ALL, &mod_type[0],
				 OM_NO_MORE_TYPES, 0, 0);

	    if (om_ret_code == OM_SUCCESS)
	    {
		if (ds_mod_type == DS_REMOVE_ATTRIBUTE)
		{
		    /* put the attribute identifier into the */
		    /* DS_C_ENTRY_MOD object.		     */
		    om_ret_code = om_put(mod_object, OM_REPLACE_ALL, 
					 &attr_identifier[0], OM_NO_MORE_TYPES, 
					 0, 0);
		}
		else
		{
		    /* put the created DS_C_ATTRIBUTE object into the */
		    /* DS_C_ENTRY_MOD object.			      */
		    om_ret_code = om_put(mod_object, OM_REPLACE_ALL, 
					 attr_object, OM_NO_MORE_TYPES, 0, 0);
		}

		if (om_ret_code == OM_SUCCESS)
		{
		    *entry_mod = mod_object;
		}
	    }
	}
    }

    /* now free the created objects */
    if (attr_object != NULL)
	om_delete(attr_object);

    if (objid.elements != NULL)
	free(objid.elements);

    if (om_ret_code != OM_SUCCESS)
    {
	/* map the XOM error to the translator defined */
	/* error and create the error string.          */
	retval = gc_t_map_xom_error(om_ret_code, error_vect);
    }

    if (retval == GC_SUCCESS)
    {
	*entry_mod = mod_object;
    }
    else
    {
	if (mod_object != NULL)
	{
	    om_delete(mod_object);
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__free_entry_mod_list			   */
/* Description   : This function is used to free the created   	   */
/*		   DS_C_ENTRY_MOD_LIST object.		           */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_t__free_entry_mod_list(
    OM_object  entry_mod_list)  /* IN - The object to be freed */
{
signed16 i;

    if (entry_mod_list == NULL)
	return;

    for (i = 0; entry_mod_list[i].type != OM_NO_MORE_TYPES; ++i)
    {
	if (entry_mod_list[i].type == DS_CHANGES)
	{
	    om_delete(entry_mod_list[i].value.object.object);
	}
    }

    free(entry_mod_list);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__perform_modify			    	   */
/* Description   : This function is used to perform the directory  */
/*		   modify operation. In the case of modify with    */
/*		   add attribute operation, the operation is first */
/*		   performed with the request DS_ADD_ATTRIBUTE. If */
/*		   this fails, then the operation is performed     */
/*		   with DS_ADD_VALUES.			           */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__perform_modify(
    boolean            is_addattr_op, /* IN  - Indicates if add attr.operation*/
    OM_private_object  session,       /* IN  - The session object	      */
    OM_private_object  context,	      /* IN  - The context object	      */
    OM_private_object  name,	      /* IN  - The name object	      	      */
    OM_object	       entry_mod_list,/* IN  - DS_C_ENTRY_MOD_LIST object     */
    boolean            local_strings, /* IN  - To specify local strings       */
    gc_t_vector      **error_vect)    /* OUT - Error message in case of errors*/
{
OM_private_object  entry_mod;
OM_return_code	   om_ret_code;
OM_descriptor      mod_type[2] = {OM_NULL_DESCRIPTOR, OM_NULL_DESCRIPTOR};
OM_sint	   	   invoke_id_return;
DS_status  	   ds_status;
signed16	   retval;
signed16	   i;

    retval = GC_SUCCESS;

    /* perform the modify operation */
    ds_status = ds_modify_entry(session, context, name, entry_mod_list,
			        &invoke_id_return);
    if (ds_status != DS_SUCCESS)
    {
        /* map the XDS error to translator defined error and */
        /* create the error vector.                          */
	retval = gc_t_map_xds_error(ds_status, local_strings, error_vect);

	/* free the error object */
	om_delete(ds_status);

	/* repeat the modify operation if the return value is       */
	/* GC_T_DS_E_ATTRIBUTE_OR_VALUE_EXISTS and the operation is */
	/* add attribute.					    */
	if (retval == GC_T_DS_E_ATTRIBUTE_OR_VALUE_EXISTS)
	{
	    /* now check if the operation was add attribute */
	    if (is_addattr_op == TRUE)
	    {
		/* free the error vector */
		gc_t_free_result(*error_vect);

		retval = GC_SUCCESS;

		/* If add attribute operation has failed, the operation */
		/* has to be repeated with DS_ADD_VALUES 		*/
		omX_fill(DS_MOD_TYPE, OM_S_ENUMERATION, DS_ADD_VALUES, 0, 
			 &mod_type[0]);

		/* locate the DS_C_ENTRY_INFO object */
		for (i = 0; entry_mod_list[i].type != OM_NO_MORE_TYPES; ++i)
		{
		    if (entry_mod_list[i].type == DS_CHANGES)
		    {
			entry_mod = entry_mod_list[i].value.object.object;
			break;
		    }
		}

		/* now replace the DS_MOD_TYPE with DS_ADD_VALUES */
		om_ret_code = om_put(entry_mod, OM_REPLACE_ALL, &mod_type[0],
				     OM_NO_MORE_TYPES, 0, 0);

		if (om_ret_code != OM_SUCCESS)
		{
		    /* map the XOM error to the translator defined */
		    /* error and create the error string.          */
		    retval = gc_t_map_xom_error(om_ret_code, error_vect);
		}
		else
		{
		    /* repeat the modify operation */
		    ds_status = ds_modify_entry(session, context, name, 
						entry_mod_list, 
						&invoke_id_return);
		    if (ds_status != DS_SUCCESS)
		    {
			/* map the XDS error to translator defined error and */
			/* create the error vector.                          */
			retval = gc_t_map_xds_error(ds_status, local_strings, 
						    error_vect);

			/* free the error object */
			om_delete(ds_status);
		    }
		}
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
