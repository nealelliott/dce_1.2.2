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
 * $Log: gctmoddsc.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:47  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:57:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:22  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:46  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:42  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:55:10  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:55:06  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:51  keutel
 * 	creation
 * 	[1994/03/21  12:56:35  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gctmoddsc.c					   */
/* Description : This file contains all the modules for modifying  */
/*		 the default service control settings		   */
/* Date	       : September 1, 1993				   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gctmoddsc.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:56 $";
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

#include <xomext.h>

#include <gdscp.h>
#include <gctstruct.h>
#include <gctconv.h>
#include <gctutil.h>
#include <gctargs.h>
#include <gcterr.h>
#include <gctbind.h>
#include <gctmoddsc.h>

/* Import the necessary Object Identifier constants */
OM_IMPORT(DSX_C_GDS_CONTEXT)

/*-- Static Variable(s) -------------------------------------------*/
static OM_private_object gc_t_context = NULL;

/*-- Static Function(s) -------------------------------------------*/
static signed16 gc_t__create_context(
    OM_workspace  workspace,      /* IN  - The workspace 		   */
    OM_object	  public_context, /* IN  - The public context object	   */
    gc_t_vector **out_vect);      /* OUT - Error message in case of errors */

static void gc_t__fill_context_descriptor(
    OM_object  public_context,  /* IN - The descriptor to be filled */
    OM_type    omtype,	        /* IN - OM_Type of the attribute    */
    OM_syntax  omsyntax,	/* IN - OM_syntax of the attribute  */
    void      *value);	        /* IN - Value of the attribute      */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_moddsc					   */
/* Description   : This function is used to modify the default	   */
/*		   service control settings.			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_t_moddsc(
    char        **in_vect,    /* IN  - Array of input vectors          */
    signed16      vect_count, /* IN  - Count of input vectors          */
    gc_t_vector **out_vect)   /* OUT - NULL (success) or error message */
{
OM_return_code  om_ret_code;
OM_enumeration	enum_value;
OM_boolean	bool_value;
OM_workspace    workspace;
OM_integer	time_limit_value, size_limit_value;
OM_object       public_context;
OM_type		type_value;
boolean		default_switch;
boolean		is_present;
boolean		infinite_time_limit, infinite_size_limit, unlimited_scope;
boolean    	no_cache_class;
signed16	desc_count;
signed16  	retval, i, k;
char	       *time_limit, *size_limit, *prefer_adm_funcs, *local_scope;
char	       *use_cache, *use_dsa, *dont_use_copy, *dont_deref_alias;
char	       *cache_priority, *prefer_chain, *chaining_prohibited;
char	       *automatic_continuation, *cache_first, *cache_class;
char	       *scope_of_referral; 
char	       *append_string;

    if (out_vect == NULL || vect_count < 0)
    {
	return(GC_T_INVALID_PARAMETER);
    }

    i = 0;
    desc_count = 0;
    public_context = NULL;
    default_switch = FALSE;
    infinite_time_limit = infinite_size_limit = unlimited_scope = FALSE;
    no_cache_class = FALSE;
    time_limit = size_limit = prefer_adm_funcs = local_scope = NULL;
    use_cache = use_dsa = dont_use_copy = dont_deref_alias = NULL;
    cache_priority = prefer_chain = chaining_prohibited = NULL;
    automatic_continuation = cache_first = cache_class = NULL;
    scope_of_referral = NULL;
    append_string = NULL;
    *out_vect = NULL;
    retval = GC_SUCCESS;

    if (vect_count > 0 && in_vect == NULL)
    {
	retval = GC_T_INVALID_PARAMETER;
    }
    else
    {
	if ((workspace = gc_t_get_workspace()) == NULL)
	{
	    retval = GC_T_BIND_NOT_MADE;
	}
	else
	{
	    if (vect_count == 0 || in_vect == NULL)
	    {
		retval = GC_T_MISSING_ARGUMENTS;
            }
	}
    }

    if (retval != GC_SUCCESS)
    {
	/* create the error vector */
	gc_t_create_gct_error_vector(retval, NULL, out_vect);
    }
    else
    {
	for (; i < vect_count && in_vect[i] != NULL; ++i)
	{
	    append_string = in_vect[i];

	    /* if default switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_DEFAULT_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* with this switch, no other argument should be specified */
		if (vect_count > 1)
		{
		    retval = GC_T_TOO_MANY_ARGUMENTS;
		}
		else
		{
		    default_switch = TRUE;
		}
		break;
	    }

	    /* if Size Limit switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_SIZE_LIMIT_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (size_limit != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by a size limit value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_SIZE_LIMIT;
		    break;
		}

		size_limit = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if Time Limit switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_TIME_LIMIT_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (time_limit != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by a time limit value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_TIME_LIMIT;
		    break;
		}

		time_limit = in_vect[i];
		++desc_count;

		continue;
	    }

	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_PREFER_ADMIN_FUNCTIONS_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (prefer_adm_funcs != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		prefer_adm_funcs = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if LocalScope switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_LOCAL_SCOPE_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (local_scope != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		local_scope = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if UseCache switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_USE_CACHE_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (use_cache != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		use_cache = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if UseDSA switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_USE_DSA_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (use_dsa != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		use_dsa = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if DontUseCopy switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_DONT_USE_COPY_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (dont_use_copy != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		dont_use_copy = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if DontDerefAlias switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_DONT_DEREF_ALIAS_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (dont_deref_alias != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		dont_deref_alias = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if Cache Priority switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_PRIORITY_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (cache_priority != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		cache_priority = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if Prefer chaining switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_PREF_CHAIN_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (prefer_chain != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		prefer_chain = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if Chaining switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_CHAINING_PROHIBITED_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (chaining_prohibited != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		chaining_prohibited = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if automatic continuation switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_AUTOMATIC_CONTINUATION_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (automatic_continuation != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		automatic_continuation = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if CacheFirst switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_CACHE_FIRST_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (cache_first != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		cache_first = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if cache class switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_CACHE_CLASS_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (cache_class != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		cache_class = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* if scope of referral switch specified */
	    retval = gc_t_check_switch(GC_T_MOD_DSC, in_vect[i], 
				       GC_T_SCOPE_OF_REFERRAL_SWITCH,
				       &is_present, out_vect);

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    if (is_present == TRUE)
	    {
		/* check if it is specified more than once */
		if (scope_of_referral != NULL)
		{
		    retval = GC_T_CONFLICTING_OPTION;
		    break;
		}

		++i;

		/* check if it is followed by its value */
		if (i >= vect_count || in_vect[i] == NULL ||
		    in_vect[i][0] == '-')
		{
		    retval = GC_T_MISSING_VALUE;
		    break;
		}

		scope_of_referral = in_vect[i];
		++desc_count;

		continue;
	    }

	    /* must be an unknown switch or parameter */
	    if (in_vect[i][0] == '-')
		retval = GC_T_UNKNOWN_OPTION;
	    else
		retval = GC_T_UNKNOWN_ARGUMENT;

	    break;
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* if default switch is specified */
	if (default_switch == TRUE)
	{
	    /* free the context so that only default values are      */
	    /* returned during a subsequent call to gc_t_get_context */
	    gc_t_free_context();
	}
	else
	{
	    /* Allocate the area for the descriptors of context object */ 
	    /* Two more are required for the DSX_C_GDS_CONTEXT and the */
	    /* NULL descriptor.			    	           */
	    public_context = (OM_descriptor *) 
			      malloc(sizeof(OM_descriptor) * (desc_count + 2));

	    if (public_context != NULL)
	    {
		/* initialize the first descriptor with the class */
		omX_fill_oid(OM_CLASS, DSX_C_GDS_CONTEXT, &public_context[0]);

		/* initialize the others to NULL descriptors */
		for (k = 1; k < desc_count + 2; ++k)
		{
		    GC_T_FILL_OMD_NULL(&public_context[k]);
		}

		k = 1;
		while (retval == GC_SUCCESS)
		{
		    /* if size limit has been specified */
		    if (size_limit != NULL)
		    {
			/* Check if size limit = INFINITE has been specified */
			if (strcmp(size_limit, GC_T_INFINITE_STR) == 0)
			{
			    infinite_size_limit = TRUE;
			}
			else
			{
			    size_limit_value = (OM_integer) atoi(size_limit);
			    if (size_limit_value == 0)
			    {
				append_string = size_limit;
				retval = GC_T_INVALID_SIZE_LIMIT;
				break;
			    }

			    gc_t__fill_context_descriptor(&public_context[k], 
							  DS_SIZE_LIMIT, 
							  OM_S_INTEGER,
						    (void *) &size_limit_value);
			    ++k;
			}
		    }

		    /* if Time Limit switch specified */
		    if (time_limit != NULL)
		    {
			/* Check if time limit = INFINITE has been specified */
			if (strcmp(time_limit, GC_T_INFINITE_STR) == 0)
			{
			    infinite_time_limit = TRUE;
			}
			else
			{
			    time_limit_value = (OM_integer) atoi(time_limit);
			    if (time_limit_value == 0)
			    {
				append_string = time_limit;
				retval = GC_T_INVALID_TIME_LIMIT;
				break;
			    }
			    else
			    {
				gc_t__fill_context_descriptor(
							     &public_context[k],
							     DS_TIME_LIMIT, 
							     OM_S_INTEGER,
						    (void *) &time_limit_value);
				++k;
			    }
			}
		    }

		    /* if Prefer Admin Functions switch specified */
		    if (prefer_adm_funcs != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(prefer_adm_funcs, GC_T_TRUE_STR) == 0)
			{
			    bool_value = OM_TRUE;
			}
			else
			{
			    if (strcmp(prefer_adm_funcs, GC_T_FALSE_STR) == 0)
			    {
				bool_value = OM_FALSE;
			    }
			    else
			    {
				append_string = prefer_adm_funcs;
				retval = GC_T_UNKNOWN_ARGUMENT;
				break;
			    }
			}
			gc_t__fill_context_descriptor(&public_context[k],
						      DSX_PREFER_ADM_FUNCS,
						      OM_S_BOOLEAN, 
						      (void *) &bool_value);
			++k;
		    }

		    /* if LocalScope switch specified */
		    if (local_scope != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(local_scope, GC_T_TRUE_STR) == 0)
			{
			    bool_value = OM_TRUE;
			}
			else
			{
			    if (strcmp(local_scope, GC_T_FALSE_STR) == 0)
			    {
				bool_value = OM_FALSE;
			    }
			    else
			    {
				append_string = local_scope;
				retval = GC_T_UNKNOWN_ARGUMENT;
				break;
			    }
			}
			gc_t__fill_context_descriptor(&public_context[k],
						      DS_LOCAL_SCOPE,
						      OM_S_BOOLEAN, 
						      (void *) &bool_value);
			++k;
		    }


		    /* if UseCache switch specified */
		    if (use_cache != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(use_cache, GC_T_TRUE_STR) == 0)
			{
			    bool_value = OM_TRUE;
			}
			else
			{
			    if (strcmp(use_cache, GC_T_FALSE_STR) == 0)
			    {
				bool_value = OM_FALSE;
			    }
			    else
			    {
				append_string = use_cache;
				retval = GC_T_UNKNOWN_ARGUMENT;
				break;
			    }
			}
			gc_t__fill_context_descriptor(&public_context[k],
						      DSX_DUA_CACHE,
						      OM_S_BOOLEAN, 
						      (void *) &bool_value);
			++k;
		    }

		    /* if UseDSA switch specified */
		    if (use_dsa != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(use_dsa, GC_T_TRUE_STR) == 0)
			{
			    bool_value = OM_TRUE;
			}
			else
			{
			    if (strcmp(use_dsa, GC_T_FALSE_STR) == 0)
			    {
				bool_value = OM_FALSE;
			    }
			    else
			    {
				append_string = use_dsa;
				retval = GC_T_UNKNOWN_ARGUMENT;
				break;
			    }
			}
			gc_t__fill_context_descriptor(&public_context[k],
						      DSX_USEDSA, OM_S_BOOLEAN, 
						      (void *) &bool_value);
			++k;
		    }

		    /* if DontUseCopy switch specified */
		    if (dont_use_copy != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(dont_use_copy, GC_T_TRUE_STR) == 0)
			{
			    bool_value = OM_TRUE;
			}
			else
			{
			    if (strcmp(dont_use_copy, GC_T_FALSE_STR) == 0)
			    {
				bool_value = OM_FALSE;
			    }
			    else
			    {
				append_string = dont_use_copy;
				retval = GC_T_UNKNOWN_ARGUMENT;
				break;
			    }
			}
			gc_t__fill_context_descriptor(&public_context[k],
						      DS_DONT_USE_COPY,
						      OM_S_BOOLEAN, 
						      (void *) &bool_value);
			++k;
		    }

		    /* if DontDerefAlias switch specified */
		    if (dont_deref_alias != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(dont_deref_alias, GC_T_TRUE_STR) == 0)
			{
			    bool_value = OM_TRUE;
			}
			else
			{
			    if (strcmp(dont_deref_alias, GC_T_FALSE_STR) == 0)
			    {
				bool_value = OM_FALSE;
			    }
			    else
			    {
				append_string = dont_deref_alias;
				retval = GC_T_UNKNOWN_ARGUMENT;
				break;
			    }
			}
			gc_t__fill_context_descriptor(&public_context[k],
						    DS_DONT_DEREFERENCE_ALIASES,
						    OM_S_BOOLEAN, 
						    (void *) &bool_value);
			++k;
		    }

		    /* if Cache Priority switch specified */
		    if (cache_priority != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(cache_priority, GC_T_LOW_STR) == 0)
			{
			    enum_value = DS_LOW;
			}
			else
			{
			    if (strcmp(cache_priority, GC_T_MEDIUM_STR) == 0)
			    {
				enum_value = DS_MEDIUM;
			    }
			    else
			    {
				if (strcmp(cache_priority, GC_T_HIGH_STR) == 0)
				{
				    enum_value = DS_HIGH;
				}
				else
				{
				    append_string = cache_priority;
				    retval = GC_T_UNKNOWN_ARGUMENT;
				    break;
				}
			    }
			}
			gc_t__fill_context_descriptor(&public_context[k],
						      DS_PRIORITY,
						      OM_S_ENUMERATION, 
						      (void *) &enum_value);
			++k;
		    }

		    /* if Prefer chaining switch specified */
		    if (prefer_chain != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(prefer_chain, GC_T_TRUE_STR) == 0)
			{
			    bool_value = OM_TRUE;
			}
			else
			{
			    if (strcmp(prefer_chain, GC_T_FALSE_STR) == 0)
			    {
				bool_value = OM_FALSE;
			    }
			    else
			    {
				append_string = prefer_chain;
				retval = GC_T_UNKNOWN_ARGUMENT;
				break;
			    }
			}
			gc_t__fill_context_descriptor(&public_context[k],
						      DS_PREFER_CHAINING,
						      OM_S_BOOLEAN, 
						      (void *) &bool_value);
			++k;
		    }

		    /* if Chaining Prohibited switch specified */
		    if (chaining_prohibited != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(chaining_prohibited, GC_T_TRUE_STR) == 0)
			{
			    bool_value = OM_TRUE;
			}
			else
			{
			    if (strcmp(chaining_prohibited,GC_T_FALSE_STR) == 0)
			    {
				bool_value = OM_FALSE;
			    }
			    else
			    {
				append_string = chaining_prohibited;
				retval = GC_T_UNKNOWN_ARGUMENT;
				break;
			    }
			}
			gc_t__fill_context_descriptor(&public_context[k],
						      DS_CHAINING_PROHIB,
						      OM_S_BOOLEAN, 
						      (void *) &bool_value);
			++k;
		    }

		    /* if automatic continuation switch specified */
		    if (automatic_continuation != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(automatic_continuation, GC_T_TRUE_STR) == 0)
			{
			    bool_value = OM_TRUE;
			}
			else
			{
			    if (strcmp(automatic_continuation, GC_T_FALSE_STR) 
				== 0)
			    {
				bool_value = OM_FALSE;
			    }
			    else
			    {
				append_string = automatic_continuation;
				retval = GC_T_UNKNOWN_ARGUMENT;
				break;
			    }
			}
			gc_t__fill_context_descriptor(&public_context[k],
						      DS_AUTOMATIC_CONTINUATION,
						      OM_S_BOOLEAN, 
						      (void *) &bool_value);
			++k;
		    }

		    /* if CacheFirst switch specified */
		    if (cache_first != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(cache_first, GC_T_TRUE_STR) == 0)
			{
			    bool_value = OM_TRUE;
			}
			else
			{
			    if (strcmp(cache_first, GC_T_FALSE_STR) == 0)
			    {
				bool_value = OM_FALSE;
			    }
			    else
			    {
				append_string = cache_first;
				retval = GC_T_UNKNOWN_ARGUMENT;
				break;
			    }
			}
			gc_t__fill_context_descriptor(&public_context[k],
						      DSX_DUAFIRST,
						      OM_S_BOOLEAN, 
						      (void *) &bool_value);
			++k;
		    }

		    /* if cache class switch specified */
		    if (cache_class != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(cache_class, GC_T_NONE_STR) == 0)
			{
			    no_cache_class = TRUE;
			}
			else
			{
			    if (strcmp(cache_class, GC_T_RESIDENT_STR) == 0)
			    {
				type_value = DSX_RESIDENT_CLASS;
			    }
			    else
			    {
				if (strcmp(cache_class, GC_T_PRIVILEGE_STR) 
				    == 0)
				{
				    type_value = DSX_PRIV_CLASS;
				}
				else
				{
				    if (strcmp(cache_class, GC_T_NORMAL_STR) 
					== 0)
				    {
					type_value = DSX_NORMAL_CLASS;
				    }
				    else
				    {
					append_string = cache_class;
					retval = GC_T_UNKNOWN_ARGUMENT;
					break;
				    }
				}
			    }
			    bool_value = OM_TRUE;
			    gc_t__fill_context_descriptor(&public_context[k],
							  type_value, 
							  OM_S_BOOLEAN,
							  (void *) &bool_value);
			    ++k;
			}
		    }

		    /* if scope of referral switch specified */
		    if (scope_of_referral != NULL)
		    {
			/* now check if it is a valid value */
			if (strcmp(scope_of_referral, GC_T_UNLIMITED_STR) == 0)
			{
			    unlimited_scope = OM_TRUE;
			}
			else
			{
			    if (strcmp(scope_of_referral,GC_T_COUNTRY_STR) == 0)
			    {
				enum_value = DS_COUNTRY;
			    }
			    else
			    {
				if (strcmp(scope_of_referral,GC_T_DMD_STR) == 0)
				{
				    enum_value = DS_DMD;
				}
				else
				{
				    append_string = scope_of_referral;
				    retval = GC_T_UNKNOWN_ARGUMENT;
				    break;
				}
			    }
			    gc_t__fill_context_descriptor(&public_context[k],
						          DS_SCOPE_OF_REFERRAL,
						          OM_S_ENUMERATION, 
						          (void *) &enum_value);
			    ++k;
			}
		    }
		    break;
		}
	    }
	    else
	    {
		retval = GC_T_MEMORY_INSUFFICIENT;
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	/* convert to private object only if there is something in */
	/* the public object.				           */
	if (public_context != NULL && 
	    public_context[1].type != OM_NO_MORE_TYPES)
	{
	    retval = gc_t__create_context(workspace, public_context, out_vect);
	}
    }
    else
    {
	if (*out_vect == NULL)
	{
	    /* create the error vector */
	    gc_t_create_gct_arg_error_vector(retval, append_string, out_vect);
	}
    }

    if (public_context != NULL)
	free(public_context);

    if (retval == GC_SUCCESS)
    {
	om_ret_code = OM_SUCCESS;
	/* if infinite time limit specified, remove DS_TIME_LIMIT */
	/* from context object					  */
	if (infinite_time_limit == TRUE)
	{
	    if (gc_t_context != NULL)
	    {
		/* remove the existing DS_TIME_LIMIT from the context */
		om_ret_code = om_remove(gc_t_context, DS_TIME_LIMIT, 0, 
					OM_ALL_VALUES);

		/* if it was not existing earlier, it is okay */
		if (om_ret_code == OM_NOT_PRESENT)
		{
		    om_ret_code = OM_SUCCESS;
		}
	    }
	}

	if (om_ret_code == OM_SUCCESS)
	{
	    /* if infinite size limit specified, remove DS_SIZE_LIMIT */
	    /* from context object				      */
	    if (infinite_size_limit == TRUE)
	    {
		if (gc_t_context != NULL)
		{
		    /* remove the existing DS_SIZE_LIMIT from the context */
		    om_ret_code = om_remove(gc_t_context, DS_SIZE_LIMIT, 
					    0, OM_ALL_VALUES);

		    /* if it was not existing earlier, it is okay */
		    if (om_ret_code == OM_NOT_PRESENT)
		    {
			om_ret_code = OM_SUCCESS;
		    }
		}
	    }
	}

	if (om_ret_code == OM_SUCCESS)
	{
	    /* if unlimited scope of referral specified, remove */
	    /* DS_SCOPE_OF_REFERRAL from context object		*/
	    if (unlimited_scope == TRUE)
	    {
		if (gc_t_context != NULL)
		{
		    /* remove the existing DS_SCOPE_OF_REFERRAL from context */
		    om_ret_code = om_remove(gc_t_context, DS_SCOPE_OF_REFERRAL, 
					    0, OM_ALL_VALUES);

		    /* if it was not existing earlier, it is okay */
		    if (om_ret_code == OM_NOT_PRESENT)
		    {
			om_ret_code = OM_SUCCESS;
		    }
		}
	    }
	}

	if (om_ret_code == OM_SUCCESS)
	{
	    /* if cache class NONE is specified, remove the existing   */
	    /* DSX_NORMAL_CLASS, DSX_PRIV_CLASS and DSX_RESIDENT_CLASS */
	    /* from context.					       */
	    if (no_cache_class == TRUE)
	    {
		if (gc_t_context != NULL)
		{
		    /* first remove DSX_NORMAL_CLASS from context */
		    om_ret_code = om_remove(gc_t_context, DSX_NORMAL_CLASS, 
					    0, OM_ALL_VALUES);

		    /* if it was not existing earlier, it is okay */
		    if (om_ret_code == OM_SUCCESS || 
			om_ret_code == OM_NOT_PRESENT)
		    {
			/* now remove DSX_PRIV_CLASS from context */
			om_ret_code = om_remove(gc_t_context, DSX_PRIV_CLASS, 
						0, OM_ALL_VALUES);

			/* if it was not existing earlier, it is okay */
			if (om_ret_code == OM_SUCCESS ||
			    om_ret_code == OM_NOT_PRESENT)
			{
			    /* now remove DSX_RESIDENT_CLASS from context */
			    om_ret_code = om_remove(gc_t_context, 
						    DSX_RESIDENT_CLASS, 
						    0, OM_ALL_VALUES);

			    /* if it was not existing earlier, it is okay */
			    if (om_ret_code == OM_NOT_PRESENT)
			    {
				om_ret_code = OM_SUCCESS;
			    }
			}
		    }
		}
	    }
	}

	if (om_ret_code != OM_SUCCESS)
	{
	    /* map the XOM error to the translator defined */
	    /* error and create the error string.          */
	    retval = gc_t_map_xom_error(om_ret_code, out_vect);
	}
    }

    if (retval == GC_SUCCESS)
    {
	*out_vect = NULL;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__create_context				   */
/* Description   : This function creates a private 		   */
/*		   DSX_C_GDS_CONTEXT object from a public object   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_t__create_context(
    OM_workspace  workspace,      /* IN  - The workspace 		   */
    OM_object	  public_context, /* IN  - The public context object	   */
    gc_t_vector **out_vect)       /* OUT - Error message in case of errors */
{
OM_return_code  om_ret_code;
signed16  	retval;

    retval = GC_SUCCESS;

    if (gc_t_context == NULL)
    {
	om_ret_code = om_create(DSX_C_GDS_CONTEXT, OM_TRUE, workspace,
				&gc_t_context);
	if (om_ret_code != OM_SUCCESS)
	{
	    retval = gc_t_map_xom_error(om_ret_code, out_vect);
	}
    }

    if (retval == GC_SUCCESS)
    {
	om_ret_code = om_put(gc_t_context, OM_REPLACE_ALL, public_context, 
			     OM_NO_MORE_TYPES, 0, 0);
	if (om_ret_code == OM_SUCCESS)
	{
	    *out_vect = NULL;
	}
	else
	{
	    gc_t_free_context();
	    retval = gc_t_map_xom_error(om_ret_code, out_vect);
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t__fill_context_descriptor		   */
/* Description   : This function is used to fill the public context*/
/*		   object with user supplied values.		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*		   the type of error.				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_t__fill_context_descriptor(
    OM_object  public_context,  /* IN - The descriptor to be filled */
    OM_type    omtype,	        /* IN - OM_Type of the attribute    */
    OM_syntax  omsyntax,	/* IN - OM_syntax of the attribute  */
    void      *value) 	        /* IN - Value of the attribute      */
{
OM_enumeration  enum_value;
OM_integer 	int_value;
OM_boolean 	bool_value;

    switch(omsyntax)
    {
	case (OM_S_INTEGER):
	    int_value = *((OM_integer *) value);
	    omX_fill(omtype, omsyntax, int_value, 0, public_context);
	    break;
	case (OM_S_BOOLEAN):
	    bool_value = *((OM_boolean *) value);
	    omX_fill(omtype, omsyntax, bool_value, 0, public_context);
	    break;
	case (OM_S_ENUMERATION):
	    enum_value = *((OM_enumeration *) value);
	    omX_fill(omtype, omsyntax, enum_value, 0, public_context);
	    break;
	default:
	    break;
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_get_context				   */
/* Description   : This function returns the context object	   */
/* Return Value  : The context object				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_private_object gc_t_get_context(
    void)
{
    if (gc_t_context != NULL)
	return(gc_t_context);
    else
	return(DS_DEFAULT_CONTEXT);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_t_free_context				   */
/* Description   : This function is used to free the context object*/
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_t_free_context(
    void)
{
    if (gc_t_context != NULL)
	om_delete(gc_t_context);

    gc_t_context = NULL;
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
