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
 * $Log: gctbind.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:40  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:14  root]
 * 
 * Revision 1.1.2.5  1994/09/19  08:39:32  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:33:15  marrek]
 * 
 * Revision 1.1.2.4  1994/07/19  14:48:57  marrek
 * 	Included negotiation of SAP package for the ds_version call.
 * 	[1994/07/19  12:26:36  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:40  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:34  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:03  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:54:26  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:35:49  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:39  keutel
 * 	March 1994 code drop
 * 	[1994/03/22  11:32:45  keutel]
 * 
 * 	creation
 * 	[1994/03/21  12:56:07  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctbind.c					   */
/* Description : This file contains all the modules for the bind   */
/*		 and unbind operations				   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctbind.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:48 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gds.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xom.h>
#include <xds.h>
#include <xdsgds.h>
#include <xdsbdcp.h>
#include <xdsmdup.h>
#include <xdssap.h>

#include <xomext.h>
#include <xoi.h>

#include <gdscp.h>
#include <gctstruct.h>
#include <gctconv.h>
#include <gctutil.h>
#include <gctargs.h>
#include <gcterr.h>
#include <gctmoveto.h>
#include <gctmoddsc.h>
#include <gctbind.h>

/* Export the necessary Object Identifier constants */
OM_EXPORT(DS_C_DS_DN)
OM_EXPORT(DS_C_DS_RDN)
OM_EXPORT(DS_C_AVA)
OM_EXPORT(DS_C_ATTRIBUTE_LIST)
OM_EXPORT(DS_C_ATTRIBUTE)
OM_EXPORT(DS_C_ENTRY_MOD_LIST)
OM_EXPORT(DS_C_FILTER)
OM_EXPORT(DS_C_PRESENTATION_ADDRESS)
OM_EXPORT(DS_C_ENTRY_MOD)
OM_EXPORT(DS_C_ENTRY_INFO_SELECTION)
OM_EXPORT(DSX_C_GDS_SESSION)
OM_EXPORT(DSX_C_GDS_CONTEXT)
OM_EXPORT(DS_C_ATTRIBUTE_ERROR)
OM_EXPORT(DS_C_NAME_ERROR)
OM_EXPORT(DS_C_REFERRAL)

/*-- Static Variable(s) -------------------------------------------*/
/* To store the bound session returned after a successful bind operation */
static 	OM_private_object	gc_t_bound_session = NULL;

/* To store Workspace created during the call to ds_initialize */
static  OM_workspace		gc_t_workspace = NULL;

/*-- Static Function(s) -------------------------------------------*/
static signed16 gc_t__initialize_workspace(
    gc_t_vector **out_vect);  /* OUT - Error message in case of errors */

static signed16 gc_t__unbind_from_server(
    gc_t_vector **out_vect); /* OUT - Error message in case of errors */

static signed16 gc_t__perform_bind(
    signed16 	   directory_id,  /* IN  - Directory Identifier		   */
    char 	  *dsa_name,      /* IN  - DSA Name		   	   */
    char 	  *psap_address,  /* IN  - PSAP Address		   	   */
    char 	  *user_name,     /* IN  - User Name		   	   */
    char 	  *user_password, /* IN  - User Password		   */
    char	  *auth_type,     /* IN  - Type of Authentication mechanism*/
    boolean        use_cache,     /* IN  - Use Cache		   	   */
    boolean        local_strings, /* IN  - To specify local strings        */
    gc_t_vector  **out_vect);     /* OUT - Error message in case of errors */



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_bind					   */
/* Description   : This function scans the input vector strings    */
/*		   and accordingly performs a bind operation.	   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_bind(
    char        **in_vect,       /* IN  - Array of input vectors 	  */
    signed16	  vect_count,    /* IN  - Count of input vectors 	  */
    boolean       local_strings, /* IN  - To specify local strings        */
    gc_t_vector **out_vect)      /* OUT - NULL (success) or error message */
{
signed16  directory_id;
signed16  retval;
boolean   use_cache;
boolean   is_present;
char	 *dsa_name, *psap_address, *user_name, *user_password, *auth_type;

    if (out_vect == NULL || vect_count < 0)
    {
	return(GC_T_INVALID_PARAMETER);
    }

    directory_id = 0;
    *out_vect = NULL;
    dsa_name = psap_address = user_name = user_password = auth_type = NULL;
    use_cache = FALSE;
    retval = GC_SUCCESS;

    if (vect_count > 0 && in_vect == NULL)
    {
	retval = GC_T_INVALID_PARAMETER;
	/* create the error vector */
	gc_t_create_gct_arg_error_vector(retval, NULL, out_vect);
    }
    else
    {
	for (; vect_count > 0 && *in_vect != NULL; ++in_vect, --vect_count)
	{
	    /* if directory identifier switch specified */
	    retval = gc_t_check_switch(GC_T_BIND, *in_vect, GC_T_DIRID_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if specified more than once */
		if (directory_id != 0)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		--vect_count;
		++in_vect;

		/* check if the switch is followed by the directory id */
		if (vect_count <= 0 || *in_vect == NULL || **in_vect == '-')
		{
		    retval = GC_T_MISSING_DIRECTORY_ID;
		    break;
		}

		/* validate the directory identifier */
		if ((directory_id = atoi(*in_vect)) == 0)
		{
		    retval = GC_T_INVALID_DIRECTORY_ID;
		    break;
		}
		continue;
	    }

	    /* If DSA name switch is specified */
	    retval = gc_t_check_switch(GC_T_BIND, *in_vect, GC_T_DSA_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if specified more than once or PSAP Address or */
		/* use cache switch also specified.  	  	  */
		if (dsa_name != NULL || psap_address != NULL || 
		    use_cache == TRUE)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		--vect_count;
		++in_vect;

		/* check if the switch is followed by the DSA name */
		if (vect_count <= 0 || *in_vect == NULL || **in_vect == '-')
		{
		    retval = GC_T_MISSING_DSA_NAME;
		    break;
		}
		dsa_name = *in_vect;
		continue;
	    }

	    /* If PSAP Address switch is specified */
	    retval = gc_t_check_switch(GC_T_BIND, *in_vect, GC_T_PSAP_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if specified more than once or DSA name or */
		/* use cache switch also specified. 	      */
		if (psap_address != NULL || dsa_name != NULL ||
		    use_cache == TRUE)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		--vect_count;
		++in_vect;

		/* check if the switch is followed by the address */
		if (vect_count <= 0 || *in_vect == NULL || **in_vect == '-')
		{
		    retval = GC_T_MISSING_PSAP_ADDRESS;
		    break;
		}
		psap_address = *in_vect;
		continue;
	    }

	    /* If Authentication type switch is specified */
	    retval = gc_t_check_switch(GC_T_BIND, *in_vect, 
				       GC_T_AUTHENTICATION_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if specified more than once or use cache also specified */
		if (auth_type != NULL || use_cache == TRUE)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		--vect_count;
		++in_vect;

		/* check if the switch is followed by the authentication type */
		if (vect_count <= 0 || *in_vect == NULL || **in_vect == '-')
		{
		    retval = GC_T_MISSING_AUTHENTICATION_TYPE;
		    break;
		}
		auth_type = *in_vect;
		continue;
	    }

	    /* If User Name switch is specified */
	    retval = gc_t_check_switch(GC_T_BIND, *in_vect, 
				       GC_T_USER_NAME_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if specified more than once or use cache also specified */
		if (user_name != NULL || use_cache == TRUE)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		--vect_count;
		++in_vect;

		/* check if the switch is followed by the user name */
		if (vect_count <= 0 || *in_vect == NULL || **in_vect == '-')
		{
		    retval = GC_T_MISSING_USER_NAME;
		    break;
		}
		user_name = *in_vect;
		continue;
	    }

	    /* If user password switch is specified */
	    retval = gc_t_check_switch(GC_T_BIND, *in_vect, GC_T_PASSWD_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if specified more than once or use cache also specified */
		if (user_password != NULL || use_cache == TRUE)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		--vect_count;
		++in_vect;

		/* check if the switch is followed by the password */
		if (vect_count <= 0 || *in_vect == NULL || **in_vect == '-')
		{
		    retval = GC_T_MISSING_USER_PASSWORD;
		    break;
		}
		user_password = *in_vect;
		continue;
	    }

	    /* if cache switch specified */
	    retval = gc_t_check_switch(GC_T_BIND, *in_vect, GC_T_CACHE_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* if DSA Name or PSAP Address or User Name or User Password */
		/* also specified 				       	     */
		if (dsa_name != NULL || psap_address != NULL ||
		    user_name != NULL || user_password != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		use_cache = TRUE;
		continue;
	    }

	    /* must be an unknown switch or parameter */
	    if (**in_vect == '-')
		retval = GC_T_UNKNOWN_OPTION;
	    else
		retval = GC_T_UNKNOWN_ARGUMENT;

	    break;
	}
    }

    if (retval != GC_SUCCESS)
    {
	if (*out_vect == NULL)
	{
	    /* create the error vector */
	    if (in_vect == NULL)
	    {
		gc_t_create_gct_arg_error_vector(retval, NULL, out_vect);
	    }
	    else
	    {
		gc_t_create_gct_arg_error_vector(retval, *in_vect, out_vect);
	    }
	}
    }
    else
    {
	/* perform the bind operation with the user supplied parameters */
	retval = gc_t__perform_bind(directory_id, dsa_name, psap_address,
				    user_name, user_password, auth_type,
				    use_cache, local_strings, out_vect);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__initialize_workspace			   */
/* Description   : This function invokes the ds_initialize and     */
/*		   creates a workspace.		 	   	   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__initialize_workspace(
    gc_t_vector **out_vect)   /* OUT - Error message in case of errors */
{
DS_feature packages[] = {
		{ OM_STRING(OMP_O_DS_BASIC_DIR_CONTENTS_PKG), OM_TRUE },
		{ OM_STRING(OMP_O_DS_MHS_DIR_USER_PKG), OM_TRUE },
		{ OM_STRING(OMP_O_DSX_GDS_PKG), OM_TRUE },
		{ OM_STRING(OMP_O_DS_STRONG_AUTHENT_PKG), OM_TRUE },
		{ {(OM_uint32) 0, (void *) 0}, OM_FALSE } };
DS_status  ds_status;
signed16   retval;

    retval = GC_SUCCESS;

    gc_t_workspace = ds_initialize();
    if (gc_t_workspace == NULL)
    {
	retval = GC_T_CANNOT_INITIALIZE_WORKSPACE;
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, out_vect);
    }
    else
    {
	ds_status = ds_version(packages, gc_t_workspace);
	if (ds_status != DS_SUCCESS)
	{
	    /* map the XDS error to translator defined error and */
	    /* create the error vector.				 */
	    retval = gc_t_map_xds_error(ds_status, TRUE, out_vect);

	    /* free the error object */
	    om_delete(ds_status);
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_unbind					   */
/* Description   : This function is used to perform the unbind     */
/*		   and shutdown operations.		 	   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_unbind(
    gc_t_vector **out_vect) /* OUT - NULL (success) or error message */
{
signed16  retval;

    if (out_vect == NULL)
	return(GC_T_INVALID_PARAMETER);

    /* free the context object */
    gc_t_free_context();

    retval = gc_t__unbind_from_server(out_vect);

    if (gc_t_workspace != NULL)
    {
	ds_shutdown(gc_t_workspace);
	gc_t_workspace = NULL;
    }

    if (retval == GC_SUCCESS)
	*out_vect = NULL;

    /* free the current DIT position string */
    gc_t_free_current_dit_position();

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__unbind_from_server			   */
/* Description   : This function is used to perform the unbind     */
/*		   operation.		 	   		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__unbind_from_server(
    gc_t_vector **out_vect) /* OUT - Error message in case of errors */
{
DS_status  ds_status;
signed16   retval;

    retval = GC_SUCCESS;

    if (gc_t_bound_session != NULL)
    {
	ds_status = ds_unbind(gc_t_bound_session);
	if (ds_status == DS_SUCCESS)
	{
	    om_delete(gc_t_bound_session);
	    gc_t_bound_session = NULL;
	}
	else
	{
            /* map the XDS error to translator defined error and */
            /* create the error vector.                          */
	    retval = gc_t_map_xds_error(ds_status, TRUE, out_vect);

	    /* free the error object */
	    om_delete(ds_status);
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__perform_bind				   */
/* Description   : This function is used to create the 		   */
/*		   DSX_C_GDS_SESSION object.	   		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__perform_bind(
    signed16 	   directory_id,  /* IN  - Directory Identifier		   */
    char 	  *dsa_name,      /* IN  - DSA Name		   	   */
    char 	  *psap_address,  /* IN  - PSAP Address		   	   */
    char 	  *user_name,     /* IN  - User Name		   	   */
    char 	  *user_password, /* IN  - User Password		   */
    char	  *auth_type,     /* IN  - Type of Authentication mechanism*/
    boolean        use_cache,     /* IN  - Use Cache		   	   */
    boolean        local_strings, /* IN  - To specify local strings        */
    gc_t_vector  **out_vect)      /* OUT - Error message in case of errors */
{
OM_private_object  user_name_object, dsa_name_object, psap_object;
OM_enumeration     auth_mechanism;
OM_integer	   retval_xoi;
OM_object	   in_session;
OM_string	   norm_passwd;
OM_uint		   line_no;
DS_status      	   ds_status;
signed32	   error_position;
signed16	   desc_count, i;
signed16 	   retval;
char		  *append_str;
char		  *msg;

    desc_count = 0;
    user_name_object = dsa_name_object = psap_object = NULL;
    append_str = NULL;
    norm_passwd.elements = NULL;
    in_session = NULL;
    retval = GC_SUCCESS;

    /* if bind to cache specified, and dir.id not specified set dir-id to 1 */
    if (use_cache == TRUE && directory_id == 0)
    {
	directory_id = 1;
    }

    /* check the authentication mechanism */
    if (auth_type == NULL)
    {
	/* authentication mechanism is not specified. User name or */
	/* password should also not be specified.		   */
	if (user_password != NULL || user_name != NULL)
	{
	    retval = GC_T_MISSING_AUTHENTICATION_TYPE;
	}
    }
    else
    {
	/* for simple authentication, both user name and password are required*/
	if (strcmp(auth_type, GC_T_SIMPLE_STR) == 0)
	{
	    if (user_password == NULL)
	    {
		retval = GC_T_MISSING_USER_PASSWORD;
	    }
	    else
	    {
		if (user_name == NULL)
		{
		    retval = GC_T_MISSING_USER_NAME;
		}
		else
		{
		    auth_mechanism = DSX_SIMPLE;
		}
	    }
	}
	else
	{
	    /* for strong authentication, only user name is required */
	    if (strcmp(auth_type, GC_T_STRONG_STR) == 0)
	    {
		if (user_name == NULL)
		{
		    retval = GC_T_MISSING_USER_NAME;
		}
		else
		{
		    auth_mechanism = DSX_STRONG;
		}
	    }
	    else
	    {
		/* for DCE authentication, both the user name and */
		/* password are not required.			  */
		if (strcmp(auth_type, GC_T_DCE_STR) == 0)
		{
		    auth_mechanism = DSX_DCE_AUTH;
		}
		else
		{
		    /* must be an unknown argument */
		    retval = GC_T_UNKNOWN_ARGUMENT;
		    append_str = auth_type;
		}
	    }
	}
    }
    
    if (retval != GC_SUCCESS)
    {
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, append_str, out_vect);
    }
    else
    {
	/* initialize workspace only once */
	if (gc_t_workspace == NULL)
	{
	    retval = gc_t__initialize_workspace(out_vect);

	    /* now initialize the XOM object info module */
	    if (retval == GC_SUCCESS)
	    {
		retval_xoi = xoi_initialize(&line_no);

		if (retval_xoi != XOI_SUCCESS)
		{
		    retval = gc_t_map_xoi_schema_file_error(retval_xoi, line_no,
							    out_vect);
		}
	    }
	}

	if (retval == GC_SUCCESS && dsa_name != NULL)
	{
	    ++desc_count;

	    /* convert the string to DS_C_NAME object */
	    retval = gc_t_string_to_name(dsa_name, local_strings, 
					 &dsa_name_object, out_vect);
	}

	if (retval == GC_SUCCESS && psap_address != NULL)
	{
	    ++desc_count;

	    /* convert the string to object */
	    retval = gc_t_string_to_object(psap_address, 
					   DS_C_PRESENTATION_ADDRESS, 
					   TRUE, &psap_object, 
					   out_vect);
	}

	if (retval == GC_SUCCESS && auth_type != NULL)
	{
	    ++desc_count;
	}

	if (retval == GC_SUCCESS && user_name != NULL)
        {
	    ++desc_count;

	    /* convert the string to DS_C_NAME object */
	    retval = gc_t_string_to_name(user_name, local_strings,
					 &user_name_object, out_vect);
        }

	if (retval == GC_SUCCESS && user_password != NULL)
	{
	    ++desc_count;
	    
	    /* convert the string to normalized form */
	    retval = gc_t_convert_to_normalized_string(user_password,
						       strlen(user_password),
						       &norm_passwd,
						       &error_position);
	    if (retval != GC_SUCCESS)
	    {
		if (retval == GC_T_MISSING_VALUE)
		{
		    /* create the error vector */
		    gc_t_create_gct_error_vector(retval, NULL, out_vect);
		}
		else
		{
		    /* get the intermediate message string from catalog */
		    msg = (char *) dce_sprintf(gc_t_error_position_format,
					       user_password, error_position);

		    /* create the error vector */
		    gc_t_create_gct_error_vector(retval, msg, out_vect);

		    /* now free the intermediate message string */
		    free(msg);
		}
	    }
	}

	if (retval == GC_SUCCESS && directory_id != 0)
	{
	    ++desc_count;
	}

	if (retval == GC_SUCCESS && use_cache == FALSE)
	{
	    if (dsa_name == NULL && psap_address == NULL)
	    {
		/* if the dsa name or the psap address is not specified,   */
		/* and other details are specified, then return an error   */
		/* This is because, if DSX_C_GDS_SESSION object is created */
		/* withoud dsa name and psap address, XDS binds to cache.  */
		if (desc_count != 0)
		{
		    retval = GC_T_MISSING_DSA_NAME;

		    /* create the error vector */
		    gc_t_create_gct_error_vector(retval, NULL, out_vect);
		}
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* since nothing has been specified bind with default session */
	if (desc_count == 0)
	{
	    in_session = DS_DEFAULT_SESSION;
	}
	else
	{
	    /* Allocate the area for the DSX_C_GDS_SESSION object   */
	    /* 3 more descriptors are required - one each for the   */
	    /* OM_class, DS_FILE_DESCRIPTOR and the NULL descriptor */
	    desc_count += 3;
	    in_session = (OM_descriptor *) 
			 malloc(sizeof(OM_descriptor) * desc_count);

	    if (in_session == NULL)
	    {
		retval = GC_T_MEMORY_INSUFFICIENT;
		/* create the error vector */
		gc_t_create_gct_error_vector(retval, NULL, out_vect);
	    }
	    else
	    {
		/* Initialize the first descriptor DSX_C_GDS_SESSION class */
		omX_fill_oid(OM_CLASS, DSX_C_GDS_SESSION, &in_session[0]);

		/* Initialize the second descriptor to DS_FILE_DESCRIPTOR */
		omX_fill(DS_FILE_DESCRIPTOR, OM_S_INTEGER,
			 DS_NO_VALID_FILE_DESCRIPTOR, 0, &in_session[1]);

		i = 2;
		if (directory_id != 0)
		{
		    omX_fill(DSX_DIR_ID, OM_S_INTEGER, directory_id, 0, 
			     &in_session[i]);
		    ++i;
		}

		if (dsa_name != NULL)
		{
		    omX_fill(DS_DSA_NAME, OM_S_OBJECT, 0, dsa_name_object, 
			     &in_session[i]);
		    ++i;
		}

		if (psap_address != NULL)
		{
		    omX_fill(DS_DSA_ADDRESS, OM_S_OBJECT, 0, psap_object, 
			     &in_session[i]);
		    ++i;
		}

		if (auth_type != NULL)
		{
		    omX_fill(DSX_AUTH_MECHANISM, OM_S_ENUMERATION, 
			     auth_mechanism, 0, &in_session[i]);
		    ++i;
		}

		if (user_name != NULL)
		{
		    omX_fill(DS_REQUESTOR, OM_S_OBJECT, 0, user_name_object, 
			     &in_session[i]);
		    ++i;
		}

		if (user_password != NULL)
		{
		    omX_fill(DSX_PASSWORD, OM_S_OCTET_STRING, 
			     norm_passwd.length, norm_passwd.elements,
			     &in_session[i]);
		    ++i;
		}

		/* fill the last descriptor with NULL */
		GC_T_FILL_OMD_NULL(&in_session[i]);
	    }
	}

        if (retval == GC_SUCCESS)
	{
	    /* if a bind operation has already been performed previously, */
	    /* unbind from the previous session and then bind.	          */
	    if ((retval = gc_t__unbind_from_server(out_vect)) == GC_SUCCESS)
	    {
		ds_status = ds_bind(in_session, gc_t_workspace, 
				    &gc_t_bound_session);

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

    /* free the created objects */
    if (user_name_object != NULL)
	om_delete(user_name_object);

    if (dsa_name_object != NULL)
	om_delete(dsa_name_object);

    if (psap_object != NULL)
	om_delete(psap_object);

    if (in_session != NULL)
	free(in_session);

    if (norm_passwd.elements != NULL)
	free(norm_passwd.elements);

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_get_bound_session			   */
/* Description   : This function is returns the bound session      */
/*		   object.		   	   		   */
/* Return Value  : The bound session				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_private_object gc_t_get_bound_session(
    void)
{
    return(gc_t_bound_session);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_get_workspace			   	   */
/* Description   : This function is returns the workspace object   */
/* Return Value  : The workspace object	   			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_workspace gc_t_get_workspace(
    void)
{
    return(gc_t_workspace);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
