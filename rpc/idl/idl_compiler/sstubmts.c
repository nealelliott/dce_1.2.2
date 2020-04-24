/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sstubmts.c,v $
 * Revision 1.1.8.2  1996/02/18  18:52:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:50  marty]
 *
 * Revision 1.1.8.1  1995/12/07  22:22:03  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/2  1995/11/21  20:21 UTC  psn
 * 	Fix parameter value for  rpc_ss_send_server_exception_2() called in stub for RIOS build.
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/20  21:01 UTC  jrr
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/11/17  17:05 UTC  dat  /main/dat_xidl2/1]
 * 
 * 	oct 95 idl drop
 * 	[1995/10/23  00:44:30  bfc]
 * 	 *
 * 	may 95 idl drop
 * 	[1995/10/21  22:58:52  bfc]
 * 	 *
 * 	DCE for DEC OSF/1: populate from OSF DCE 1.1
 * 	[1995/10/21  17:25:15  bfc]
 * 	 *
 * 
 * 	HP revision /main/HPDCE02/7  1995/06/30  19:42 UTC  lmm
 * 	merge stack reduction fix
 * 
 * 	HP revision /main/HPDCE02/lmm_reduce_rpc_stack/3  1995/06/27  21:18 UTC  lmm
 * 	don't just RAISE exception if stub variable malloc fails
 * 
 * 	HP revision /main/HPDCE02/lmm_reduce_rpc_stack/2  1995/06/21  17:21 UTC  lmm
 * 	combine malloc of IDL_msp and IDL_stack_packet when saving stack space
 * 
 * 	HP revision /main/HPDCE02/lmm_reduce_rpc_stack/1  1995/06/21  16:24 UTC  lmm
 * 	conditionally allocate large stub variables to save stack space
 * 
 * 	HP revision /main/HPDCE02/6  1995/03/08  15:00 UTC  gaz
 * 	Fix spelling of DMS_names when interface has encoding operations
 * 
 * 	HP revision /main/HPDCE02/lmm_mothra_instr/4  1995/03/07  19:58 UTC  lmm
 * 	Fix spelling of DMS_names when
 * 	interface has encoding operations
 * 
 * 	HP revision /main/HPDCE02/5  1995/03/05  14:01 UTC  gaz
 * 	Merge DMS changes to mainline
 * 
 * 	HP revision /main/HPDCE02/lmm_mothra_instr/3  1995/02/22  19:14 UTC  gaz
 * 	Merge changes out to branch
 * 
 * 	HP revision /main/HPDCE02/4  1995/01/05  20:11 UTC  lmm
 * 	Merge user-defined exception support fixes into Mothra
 * 
 * 	HP revision /main/HPDCE02/lmm_idlxx_user_excs/1  1995/01/05  19:42 UTC  lmm
 * 	no longer generate OODCE-specific faults
 * 
 * 	HP revision /main/HPDCE02/3  1994/11/01  23:56 UTC  lmm
 * 	Merging idl++ mods into Mothra
 * 
 * 	HP revision /main/HPDCE02/lmm_merge_idlxx/1  1994/11/01  23:52 UTC  lmm
 * 	Merging idl++ mods into Mothra
 * 
 * 	HP revision /main/HPDCE02/lmm_mothra_instr/2  1995/02/01  23:56 UTC  lmm
 * 	add static boolean for interface registration;
 * 	change internal #ifdef macro to _HP_DCE_INSTR
 * 
 * 	HP revision /main/HPDCE02/lmm_mothra_instr/1  1994/11/01  23:32 UTC  lmm
 * 	adding DMS support
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/21  14:49 UTC  tatsu_s
 * 	Merged mothra up to dce1_1_b18.
 * 	[1995/12/07  21:13:06  root]
 * 
 * Revision 1.1.4.2  1994/09/14  20:27:07  tom
 * 	Bug 7990 - Restore general cancellability state when an exception
 * 	is raised in the server manager.
 * 	[1994/09/14  20:24:13  tom]
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:32 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/24  19:41  lmm
 * 	merge kk and hpdce01
 * 
 * Revision 1.1.5.5  1993/11/11  21:55:57  sanders
 * 	Modified setting of HP_IDL_ii struct so that
 * 	return value goes in array slot 0
 * 	[1993/11/11  21:55:35  sanders]
 * 
 * Revision 1.1.5.4  1993/11/03  17:34:18  lmm
 * 	DTS CHFts09415: No marshall routine with -DINSTRUMENTED
 * 	added "if (instrument)" around HP initializations
 * 	[1993/11/03  17:33:56  lmm]
 * 
 * Revision 1.1.5.3  1993/10/25  14:55:46  lmm
 * 	DTS CHFts09308: Size zero param name and type array declarations
 * 	Added check for no input parameters in CSPELL_server_stub_routine
 * 	[1993/10/25  14:54:43  lmm]
 * 
 * Revision 1.1.5.2  1993/10/21  20:40:54  lmm
 * 	Merged tracing functionality into new 1.0.3 backend
 * 	[1993/10/21  20:40:03  lmm]
 * 
 * Revision 1.1.4.1  1994/06/23  18:25:23  tom
 * 	From DEC: add opaque context handle support.
 * 	[1994/06/23  18:24:10  tom]
 * 
 * Revision 1.1.2.2  1993/07/07  20:03:55  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:34:50  ganni]
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1991, 1992, 1993 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**  Digital Equipment Corporation, Maynard, Mass.
**  All Rights Reserved.  Unpublished rights reserved
**  under the copyright laws of the United States.
**
**  The software contained on this media is proprietary
**  to and embodies the confidential technology of
**  Digital Equipment Corporation.  Possession, use,
**  duplication or dissemination of the software and
**  media is authorized only pursuant to a valid written
**  license from Digital Equipment Corporation.
**
**  RESTRICTED RIGHTS LEGEND   Use, duplication, or
**  disclosure by the U.S. Government is subject to
**  restrictions as set forth in Subparagraph (c)(1)(ii)
**  of DFARS 252.227-7013, or in FAR 52.227-19, as
**  applicable.
**
**  NAME:
**
**      sstubmts.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**  Generation of server stub file for MTS compiler
**
*/

#include <nidl.h>
#include <ast.h>
#include <bedeck.h>
#include <command.h>
#include <ddbe.h>
#include <ifspec.h>
#include <cspeldcl.h>
#include <cspell.h>
#include <mtsbacke.h>
#include <mtspipes.h>
#include <sstubmts.h>
#include <user_exc.h>
#include <clihandl.h>
#include <icharsup.h>
#include <cstubmts.h>


typedef struct param_node_link_t {
    AST_parameter_n_t *p_param;
    struct param_node_link_t *p_next;
} param_node_link_t;
/* Used to build a list of "interesting" parameters of an operation */

typedef enum {
                                /* handle_t parameter with [represent_as] */
    BE_no_rep_as_handle_t_k,  /* not present */
    BE_val_rep_as_handle_t_k, /* passed by value */
    BE_ref_rep_as_handle_t_k  /* passed by reference */
} BE_rep_as_handle_t_k_t;

static char rep_as_handle_name[] = "IDL_handle_rep_as";

/*
 * BE_server_binding_analyze
 *
 * Find whether operation can be called by [auto_handle] client, and
 * whether there is a first handle_t parameter with a [represent_as] attribute
 *
 */
static void BE_server_binding_analyze
#ifdef PROTO
(
    AST_operation_n_t *p_operation,
    boolean *server_binding_explicit, /* TRUE if no client can use [auto_handle]
                                         binding with this operation */
    BE_rep_as_handle_t_k_t *p_rep_as_handle_param,
    NAMETABLE_id_t *p_rep_as_type_name,  /* type of handle param */
    NAMETABLE_id_t *p_binding_handle_name
)
#else
( p_operation, server_binding_explicit, p_rep_as_handle_param,
  p_rep_as_type_name, p_binding_handle_name )
    AST_operation_n_t *p_operation;
    boolean *server_binding_explicit;
    BE_rep_as_handle_t_k_t *p_rep_as_handle_param;
    NAMETABLE_id_t *p_rep_as_type_name;
    NAMETABLE_id_t *p_binding_handle_name;
#endif
{
    AST_parameter_n_t *p_first_parameter;
    AST_type_n_t *p_type, *p_pointee_type;

    *p_rep_as_handle_param = BE_no_rep_as_handle_t_k;
    *p_binding_handle_name = NAMETABLE_add_id("IDL_binding_handle");
    p_first_parameter = p_operation->parameters;
    if ( p_first_parameter == NULL )
    {
        /* No parameters */
        *server_binding_explicit = false;
        return;
    }
    p_type = p_first_parameter->type;
    if ( p_type->kind == AST_handle_k )
    {
        /* handle_t by value */
        *server_binding_explicit = true;
        *p_binding_handle_name = p_first_parameter->name;
        if ( p_type->rep_as_type != NULL )
        {
            *p_rep_as_handle_param = BE_val_rep_as_handle_t_k;
            *p_rep_as_type_name = p_type->name;
        }
        return;
    }
    if ( AST_HANDLE_SET(p_type) )
    {
        /* Customized handle by value */
        *server_binding_explicit = true;
        return;
    }
    if ( p_type->kind == AST_pointer_k )
    {
        p_pointee_type = p_type->type_structure.pointer->pointee_type;
        if ( p_pointee_type->kind == AST_handle_k )
        {
            /* handle_t by reference */
            *server_binding_explicit = true;
            *p_binding_handle_name = p_first_parameter->name;
            if ( p_pointee_type->rep_as_type != NULL )
            {
                *p_rep_as_handle_param = BE_ref_rep_as_handle_t_k;
                *p_rep_as_type_name = p_pointee_type->name;
            }
            return;
        }
        if ( AST_HANDLE_SET(p_pointee_type) )
        {
            /* Customized handle by reference */
            *server_binding_explicit = true;
            return;
        }
    }
    if ( AST_HAS_IN_CTX_SET(p_operation) )
    {
        *server_binding_explicit = true;
        return;
    }
    if ( AST_EXPLICIT_HANDLE_SET(p_operation) )
    {
        /* [explicit_handle] in ACF */
        *server_binding_explicit = false;
        return;
    }
    *server_binding_explicit = false;
    return;
}


/*
 * DDBE_spell_stack_surrogates
 *
 * Spell server surrogates as stack variables
 */
static void DDBE_spell_stack_surrogates
#ifdef PROTO
(
    FILE *fid,
    param_node_link_t **p_fixed_char_array_list,
                /* Pointer to list of fixed size character array parameters */
    AST_operation_n_t *p_operation
)
#else
( fid, p_fixed_char_array_list, p_operation )
    FILE *fid;
    param_node_link_t **p_fixed_char_array_list;
    AST_operation_n_t *p_operation;
#endif
{
    unsigned long param_index;
    AST_parameter_n_t *pp;  /* Pointer down list of parameters */
    AST_type_n_t array_elt_ptr_type_node;
    AST_pointer_n_t array_elt_ptr_pointer_node;

    param_index = 0;
    for (pp = p_operation->parameters; pp != NULL; pp = pp->next) 
    {
        param_index++;
        if (param_index == 1)
        {
            if (pp->type->kind == AST_handle_k)
            {
                continue;
            }
            else if ( (pp->type->kind == AST_pointer_k)
                      && (pp->type->type_structure.pointer->pointee_type->kind
                                                     == AST_handle_k) )
            {
                continue;
            }
        }
        if (AST_REF_SET(pp) && (pp->type->rep_as_type == NULL)
            && (AST_CONFORMANT_SET(pp->type)
                      || (DDBE_ARRAYIFIED(pp)
                          && AST_CONFORMANT_SET(pp->type->type_structure.
                             pointer->pointee_type->array_rep_type))))
        {
            /* [ref] arrayified pointer or array without [ptr] or [unique]
                 Storage allocated by interpreter */
            continue;
        }

        if ((pp->type->kind == AST_pointer_k) && AST_REF_SET(pp)
                 && AST_CONFORMANT_SET(pp->type->type_structure.pointer
                                        ->pointee_type)
                 && (pp->type->type_structure.pointer->pointee_type
                        ->rep_as_type == NULL))
        {
            /* [ref] pointer to conformant object
                 Storage allocated by interpreter */
            continue;
        }
        /*
         * No surrogate is needed for a [heap] parameter unless it has a
         * non-[ref] pointer attribute in which case a surrogate for the
         * pointer is needed and the pointee is allocated in the Interpreter.
         */
        if (AST_HEAP_SET(pp) && !AST_PTR_SET(pp) && !AST_UNIQUE_SET(pp))
        {
            /* No stack surrogate. Storage allocated by interpreter */
            continue;
        }
        if (AST_CONTEXT_SET(pp))
        {
            /* Special data structure for context handle */
            fprintf(fid,"IDL_ee_context_t %s;\n", BE_get_name(pp->name));
            continue;
        }
        if (AST_CONTEXT_SET(pp))
        {
            /* Special data structure for context handle */
            fprintf(fid,"IDL_ee_context_t %s;\n", BE_get_name(pp->name));
            continue;
        }
        if (pp->type->kind == AST_pointer_k)
        {
            CSPELL_typed_name(fid,
                              ((AST_REF_SET(pp) || AST_AMP_SET(pp->type)) && pp->type->type_structure.
                                  pointer->pointee_type->kind != AST_interface_k
                               ? pp->type->type_structure.pointer->pointee_type
                               : pp->type),
                              pp->name, NULL, false, true, false);
        }
        else if ( (pp->type->kind == AST_array_k)
                    && (AST_UNIQUE_SET(pp) || AST_PTR_SET(pp)) )
        {
            array_elt_ptr_pointer_node.pointee_type
                = pp->type->type_structure.array->element_type;
            array_elt_ptr_type_node.name = NAMETABLE_NIL_ID;
            array_elt_ptr_type_node.kind = AST_pointer_k;
            array_elt_ptr_type_node.flags = 0;
            array_elt_ptr_type_node.xmit_as_type = NULL;
            array_elt_ptr_type_node.rep_as_type = NULL;
            array_elt_ptr_type_node.cs_char_type = NULL;
            array_elt_ptr_type_node.array_rep_type = NULL;
            array_elt_ptr_type_node.type_structure.pointer
                = &array_elt_ptr_pointer_node;
            CSPELL_typed_name(fid, &array_elt_ptr_type_node,
                              pp->name, NULL, false, true, false);
        }
        else
            CSPELL_typed_name(fid, pp->type, pp->name, NULL, false, true, 
                                false);
        fprintf(fid, ";\n");
    }
}

/*
 * CSPELL_manager_call
 *
 * Emit a call to a manager operation
 */
static void CSPELL_manager_call
#ifdef PROTO
(
    FILE *fid,
    AST_interface_n_t *p_interface,
    AST_operation_n_t *p_operation,
    BE_rep_as_handle_t_k_t rep_as_handle_param,
    NAMETABLE_id_t rep_as_type_name,
    NAMETABLE_id_t binding_handle_name
)
#else
(fid, p_interface, p_operation, rep_as_handle_param, rep_as_type_name,
 binding_handle_name)
    FILE *fid;
    AST_interface_n_t *p_interface;
    AST_operation_n_t *p_operation;
    BE_rep_as_handle_t_k_t rep_as_handle_param;
    NAMETABLE_id_t rep_as_type_name;
    NAMETABLE_id_t binding_handle_name;
#endif
{
    AST_parameter_n_t *pp;  /* Pointer down list of parameters */
    int param_index;        /* Index of parameter in param list */
    int visible_param_count;    /* Number of parameters spelt into call */

    if ( rep_as_handle_param != BE_no_rep_as_handle_t_k )
    {
        fprintf( fid, "%s_to_local(&%s,&%s);\n",
                BE_get_name(rep_as_type_name),
                BE_get_name(binding_handle_name),
                rep_as_handle_name );
    }

    fprintf (fid, "\n/* manager call */\n");
    fprintf( fid, "IDL_manager_entered = ndr_true;\n" );


    fprintf( fid, "RPC_SS_THREADS_DISABLE_ASYNC(IDL_async_cancel_state);\n");
    fprintf( fid,
              "RPC_SS_THREADS_ENABLE_GENERAL(IDL_general_cancel_state);\n" );

{
    int has_handle = FALSE;

    if ((int)CMD_vals[opt_lang] == opt_lang_cxx)
    {
	fprintf(fid, "\n#ifdef __cplusplus\n");

	/* If not a constructor nor a static, find the object */
	if (!IS_CONSTRUCTOR(p_operation) &&
	    !AST_STATIC_SET(p_operation)) 
	{
    	    fprintf(fid, "IDL_this = (%s *)", BE_get_name(p_interface->name));
	    fprintf(fid, "rpc_object_reference::_getThis(IDL_handle,\n");
	    fprintf(fid, "((rpc_if_rep_t *) %s_v%d_%d_s_ifspec)->id);\n",
		    BE_get_name(p_interface->name),
		    (p_interface->version%65536), 
		    (p_interface->version/65536));

            if (p_interface->lookup_callout_name != NAMETABLE_NIL_ID) {
                fprintf(fid, "if (!IDL_this) {\n");
	        fprintf(fid, "uuid_t u;\n");
	        fprintf(fid, "rpc_binding_inq_object(IDL_handle,&u,(unsigned32*)&IDL_ms.IDL_status);\n");
	        fprintf(fid, "if (IDL_ms.IDL_status != error_status_ok)\n");
	        fprintf(fid, "  RAISE(rpc_x_ss_remote_comm_failure);\n");

                fprintf(fid, "IDL_this = %s(&u);\n",
                    BE_get_name(p_interface->lookup_callout_name));
	        fprintf(fid, "if (!IDL_this) RAISE(rpc_x_object_not_found);\n");
	        fprintf(fid, "}\n");
            } else {
	        fprintf(fid, "if (!IDL_this) RAISE(rpc_x_object_not_found);\n");
	    }

	}
	if (IS_DESTRUCTOR(p_operation))
	{
	    fprintf(fid, "delete IDL_this;\n");
	    fprintf(fid, "#else\n");
	}
	else
	    fprintf(fid, "\n#endif\n");
    }

    if (p_operation->result->type->kind != AST_void_k)
    {
        if (AST_CONTEXT_SET(p_operation->result))
            fprintf(fid, "IDL_function_result.local = ");
        else
            fprintf(fid, "IDL_function_result = ");
    }
    if ((int)CMD_vals[opt_lang] == opt_lang_cxx) {
	fprintf(fid, "\n#ifdef __cplusplus\n");
	if (IS_CONSTRUCTOR(p_operation)) {
	    fprintf(fid, "new %s(",BE_get_name(p_operation->cxx_new_name));
	}
	else if (AST_STATIC_SET(p_operation))
	{
            if (p_operation->cxx_static_name)
	    {
	        fprintf(fid, "%s(",
	            BE_get_name(p_operation->cxx_static_name));
	    }
	    else
	    {
	        fprintf(fid, "%s::%s(",
		    BE_get_name(p_interface->name),
	            BE_get_name(p_operation->name));
	    }
	}
	else {
	    fprintf(fid, "IDL_this->%s(",BE_get_name(p_operation->name));
	}
	fprintf(fid, "\n#else\n");
    }
    fprintf(fid, "(*((%s_v%d_%d_epv_t *)IDL_mgr_epv)->",
            BE_get_name(p_interface->name), (p_interface->version%65536),
            (p_interface->version/65536));
            spell_name(fid, p_operation->name);

    fprintf(fid,")(");
    if ((int)CMD_vals[opt_lang] == opt_lang_cxx)
	fprintf(fid, "\n#endif\n");
    visible_param_count = 0;
    param_index = 1;
    for (pp = p_operation->parameters; pp != NULL; pp = pp->next)
    {
        if (AST_HIDDEN_SET(pp))
        {
            /* Parameter does not appear in signature delivered to user */
            /* Note that hidden parameter cannot appear before binding handle */
            param_index++;
            continue;
        }
        else
        {
            visible_param_count++;
            if (visible_param_count > 1)
                fprintf (fid, ",\n ");
        }
        if (param_index == 1)
        {
            if ( rep_as_handle_param != BE_no_rep_as_handle_t_k )
            {
                fprintf( fid, "%c%s",
                  ((rep_as_handle_param == BE_ref_rep_as_handle_t_k)
                                                                        )
                                                                    ? '&' : ' ',
                  rep_as_handle_name );
                param_index++;
                continue;
            }
            else if (pp->type->kind == AST_handle_k)
            {
		if ((int)CMD_vals[opt_lang] == opt_lang_cxx)
		{
		    if (!IS_CONSTRUCTOR(p_operation) &&
	    		!AST_STATIC_SET(p_operation)) {
			    fprintf(fid, "\n#ifndef __cplusplus\n");
			    has_handle = TRUE;
			}
		}
		if ((int)CMD_vals[opt_lang] == opt_lang_cxx)
		    if (IS_CONSTRUCTOR(p_operation)) {
            		visible_param_count--;
                	param_index++;
			continue;
		    }
                spell_name(fid, binding_handle_name);
                param_index++;
		if ((int)CMD_vals[opt_lang] == opt_lang_cxx)
		    if (!IS_CONSTRUCTOR(p_operation) && 
	    		!AST_STATIC_SET(p_operation) &&
		        has_handle && 
			pp->next == NULL)
			    fprintf(fid, "\n#endif\n");
                continue;
            }
            else if ( (pp->type->kind == AST_pointer_k)
                      && (pp->type->type_structure.pointer->pointee_type->kind
                                                     == AST_handle_k) )
            {
		if ((int)CMD_vals[opt_lang] == opt_lang_cxx)
		    if (!IS_CONSTRUCTOR(p_operation)) {
			fprintf(fid, "\n#ifndef __cplusplus\n");
			has_handle = TRUE;
			}
                fprintf(fid, " &%s", BE_get_name(binding_handle_name));
                param_index++;
		if ((int)CMD_vals[opt_lang] == opt_lang_cxx)
		    if (!IS_CONSTRUCTOR(p_operation) &&
		    has_handle && pp->next == NULL) fprintf(fid, "\n#endif\n");
                continue;
            }
        }
        else
	{
	    if ((int)CMD_vals[opt_lang] == opt_lang_cxx) 
	    {
		if (has_handle) fprintf(fid, "\n#endif\n");
		has_handle = FALSE;
	    }
	}

        if ((AST_HEAP_SET(pp)
            || (AST_CONFORMANT_SET(pp->type)
                 && !AST_UNIQUE_SET(pp) && !AST_PTR_SET(pp))
            || ((pp->type->kind == AST_pointer_k) && AST_REF_SET(pp)
                    && (BE_Is_Arrayified(pp,pp->type)
                        || AST_CONFORMANT_SET(pp->type->type_structure.pointer
                                                ->pointee_type))))
           )
        {
            /*
             * Cast needed on (void *) pointer to dynamically allocated store.
             * A different cast is needed if we need to dereference the ptr.
             */
            boolean deref;
            if ((pp->type->kind == AST_array_k)
                && (AST_UNIQUE_SET(pp) || AST_PTR_SET(pp)))
            {
                DDBE_spell_manager_param_cast( fid, pp->type );
                fprintf(fid, "*(rpc_void_p_t *)");
            }
            else
            {
                deref = ((pp->type->kind != AST_pointer_k)
                     && (pp->type->kind != AST_array_k)
                    );
                if (!deref)
                    DDBE_spell_manager_param_cast( fid, pp->type );
                fprintf( fid, " %c", ((deref) ? '*' : ' ') );
                if (deref)
                    CSPELL_ptr_cast_exp( fid, pp->type );
            }
            fprintf( fid, "(IDL_param_vec[%d])", param_index );
        }
        else if (AST_CONTEXT_SET(pp))
        {
            /* Opaque context handle type requires cast */
            if (pp->type->type_structure.pointer->pointee_type->kind
                == AST_pointer_k
                &&  pp->type->type_structure.pointer->pointee_type->
                    type_structure.pointer->pointee_type->kind
                    == AST_structure_k)
                CSPELL_cast_exp(fid, pp->type);
            /* Context handle by value is pointer to void,
                              by reference is pointer to pointer to void */ 
            fprintf( fid, "%c%s.local", 
                     ((pp->type->type_structure.pointer->pointee_type->kind
                                                            == AST_pointer_k)
                                                                            )
                                                                    ? '&' : ' ',
                     BE_get_name(pp->name) );
        }
        else
        {
            if (
                   ((pp->type->kind == AST_pointer_k) && AST_REF_SET(pp)
                    && !(BE_Is_Arrayified(pp,pp->type))) )
            {
                /* non-arrayified parameter passed by reference */
		/* if its an interface pointer, don't pass address of */
		if (!(pp->type->kind == AST_pointer_k
                      && pp->type->type_structure.pointer->pointee_type->kind
                         == AST_interface_k))
                	fprintf(fid, "&");
            }
            spell_name(fid, pp->name);
        }
        
        param_index++;
    }

    fprintf(fid, ");\n");

    if ((int)CMD_vals[opt_lang] == opt_lang_cxx) 
    {
	if (IS_DESTRUCTOR(p_operation))
	    fprintf(fid, "\n#endif\n");
    }
}

    fprintf( fid,
              "RPC_SS_THREADS_RESTORE_GENERAL(IDL_general_cancel_state);\n" );
    fprintf( fid, "RPC_SS_THREADS_RESTORE_ASYNC(IDL_async_cancel_state);\n");

    if ( rep_as_handle_param != BE_no_rep_as_handle_t_k )
    {
        fprintf( fid, "%s_free_local(&%s);\n",
                BE_get_name(rep_as_type_name),
                rep_as_handle_name );
    }
}

/*
 * DDBE_convert_out_contexts
 *
 * Convert user [out] context handles from local format to wire format
 * and change the param vector entries to point at the wire formats
 *
 */
static void DDBE_convert_out_contexts
#ifdef PROTO
(
    FILE *fid,
    AST_operation_n_t *p_operation,
    NAMETABLE_id_t binding_handle_name
)
#else
(fid, p_operation,binding_handle_name)
    FILE *fid;
    AST_operation_n_t *p_operation;
    NAMETABLE_id_t binding_handle_name;
#endif
{
    AST_parameter_n_t *pp;

    if ( AST_CONTEXT_SET(p_operation->result) )
    {
        fprintf(fid,
"rpc_ss_ee_ctx_to_wire(IDL_function_result.local,&IDL_function_result.wire, %s,",
        BE_get_name(binding_handle_name));
        if ( AST_CONTEXT_RD_SET(p_operation->result->type) )
        {
            spell_name(fid, p_operation->result->type->name);
            fprintf(fid, "_rundown, ");
        }
        else
            fprintf(fid, "(void (*)())NULL, ");
        fprintf(fid, "idl_false, &IDL_ms.IDL_status);\n");
        CSPELL_test_status(fid);
    }

    for (pp = p_operation->parameters; pp != NULL; pp = pp->next) 
    {
        if ( AST_OUT_SET(pp) && AST_CONTEXT_SET(pp) )
        {
            fprintf(fid, "rpc_ss_ee_ctx_to_wire(%s.local, &%s.wire, %s,",
                    BE_get_name(pp->name), BE_get_name(pp->name),
                    BE_get_name(binding_handle_name));
            /* [out] context must be passed by reference.
                    Does it require rundown? */
            if ( AST_CONTEXT_RD_SET(pp->type->type_structure.pointer
                                                            ->pointee_type) )
            {
                spell_name(fid, pp->type->type_structure.pointer
                                                          ->pointee_type->name);
                fprintf(fid, "_rundown, ");
            }
            else
                fprintf(fid, "(void (*)())NULL, ");
            fprintf(fid, "%s, &IDL_ms.IDL_status);\n",
                         AST_IN_SET(pp) ? "idl_true" : "idl_false");
            CSPELL_test_status(fid);
        }
    }
}

/*
 *  CSPELL_server_stub_routine
 *
 *  Generate a server stub routine for an operation
 */
static void CSPELL_server_stub_routine
#ifdef PROTO
(
    FILE *fid,
    language_k_t language,
    AST_interface_n_t *p_interface,
    AST_operation_n_t *p_operation,
    int num_declared_exceptions,    /* Count of user declared exceptions */
    int num_extern_exceptions       /* Count of user extern_exceptions */
)
#else
(fid, language, p_interface, p_operation, num_declared_exceptions,
 num_extern_exceptions)
    FILE *fid;
    language_k_t language;
    AST_interface_n_t *p_interface;
    AST_operation_n_t *p_operation;
    int num_declared_exceptions;
    int num_extern_exceptions;
#endif
{
    long first_pipe;        /* Index of first pipe to be processed */
    boolean explicit_binding;
    BE_rep_as_handle_t_k_t rep_as_handle_param;
    NAMETABLE_id_t rep_as_type_name;
    NAMETABLE_id_t binding_handle_name;
    param_node_link_t *fixed_char_array_list = NULL;
                /* List of fixed size character array parameters */
    BE_cs_info_t cs_info;           /* I-char machinery description */
    BE_handle_info_t handle_info;


    BE_server_binding_analyze(p_operation, &explicit_binding,
                 &rep_as_handle_param, &rep_as_type_name, &binding_handle_name);
    handle_info.deref_assoc = ' ';
    NAMETABLE_id_to_string(binding_handle_name, &handle_info.assoc_name);

    fprintf (fid, "\nstatic void op%ld_ssr", p_operation->op_number);
    fprintf (fid, "\n#ifdef IDL_PROTOTYPES\n");

    fprintf (fid, "(\n");
    fprintf (fid, " handle_t %s,\n", handle_info.assoc_name);
    fprintf (fid, " rpc_call_handle_t IDL_call_h,\n");
    fprintf (fid, " rpc_iovector_elt_p_t IDL_elt_p,\n");
    fprintf (fid, " ndr_format_p_t IDL_drep_p,\n");
    fprintf (fid, " rpc_transfer_syntax_p_t IDL_transfer_syntax_p,\n");
    fprintf (fid, " rpc_mgr_epv_t IDL_mgr_epv,\n");
    fprintf (fid, " error_status_t *IDL_status_p\n)\n");

    fprintf (fid, "#else\n");

    fprintf (fid,
         "(%s, IDL_call_h, IDL_elt_p, IDL_drep_p, IDL_transfer_syntax_p, IDL_mgr_epv, IDL_status_p)\n",
             handle_info.assoc_name);
    fprintf (fid, " handle_t %s;\n",  handle_info.assoc_name);
    fprintf (fid, " rpc_call_handle_t IDL_call_h;\n");
    fprintf (fid, " rpc_iovector_elt_p_t IDL_elt_p;\n");
    fprintf (fid, " ndr_format_p_t IDL_drep_p;\n");
    fprintf (fid, " rpc_transfer_syntax_p_t IDL_transfer_syntax_p;\n");
    fprintf (fid, " rpc_mgr_epv_t IDL_mgr_epv;\n");
    fprintf (fid, " error_status_t *IDL_status_p;\n");

    fprintf (fid, "#endif\n");

    fprintf (fid, "{\n");
    fprintf(fid, "IDL_ms_t IDL_ms;\n");
    fprintf(fid, "idl_byte IDL_stack_packet[IDL_STACK_PACKET_SIZE];\n");
    fprintf(fid, "volatile ndr_boolean IDL_manager_entered = ndr_false;\n");
    fprintf(fid,
         "volatile RPC_SS_THREADS_CANCEL_STATE_T IDL_async_cancel_state;\n");
    fprintf(fid,
         "volatile RPC_SS_THREADS_CANCEL_STATE_T IDL_general_cancel_state;\n");

    if (AST_HAS_IN_PIPES_SET(p_operation)
    || AST_HAS_OUT_PIPES_SET(p_operation))
    {
        fprintf(fid, "long IDL_current_pipe = 0;\n");
    }

    DDBE_spell_param_vec_def( fid, p_operation, BE_server_side,
                              BE_cmd_opt, BE_cmd_val );
    DDBE_spell_stack_surrogates( fid,
                                 &fixed_char_array_list,
                                 p_operation );

    /* Does operation use I-char machinery? If so, declare needed variables */
    BE_cs_analyze_and_spell_vars(fid, p_operation, BE_server_side, &cs_info);

    if (AST_HAS_IN_PTRS_SET(p_operation)
    || AST_HAS_OUT_PTRS_SET(p_operation)
    || AST_ENABLE_ALLOCATE_SET(p_operation))
    {
        fprintf(fid, "rpc_ss_thread_support_ptrs_t IDL_support_ptrs;\n");
    }

    /* If there is a function result, we need somewhere to put it */
    if (p_operation->result->type->kind != AST_void_k)
    {
        if ( AST_CONTEXT_SET(p_operation->result) )
        {
            /* Declare stack workspace for the wire form of context handle
                    function result */
            fprintf(fid, "IDL_ee_context_t IDL_function_result;\n");
        }
        else
        {
            CSPELL_typed_name(fid, p_operation->result->type,
                            NAMETABLE_add_id("IDL_function_result"),
                            NULL, false, true, false);
            fprintf(fid, ";\n");
        }
    }

    if ( rep_as_handle_param != BE_no_rep_as_handle_t_k )
    {
        fprintf( fid, "%s %s;\n",
                      BE_get_name(rep_as_type_name), rep_as_handle_name );
    }

    if (AST_HAS_IN_CTX_SET(p_operation)
        || AST_HAS_OUT_CTX_SET(p_operation))
    {
        fprintf(fid, "rpc_client_handle_t IDL_client_id=NULL;\n");
    }


    /*
     *  Start of executable code
     */


    fprintf(fid, "RPC_SS_INIT_SERVER\n");
    fprintf(fid, "rpc_ss_init_marsh_state(IDL_type_vec, &IDL_ms);\n");
    fprintf(fid,
             "IDL_ms.IDL_stack_packet_status = IDL_stack_packet_unused_k;\n");
    fprintf(fid, "IDL_ms.IDL_stack_packet_addr = IDL_stack_packet;\n");
    fprintf(fid, "TRY\n");
    fprintf(fid, "IDL_ms.IDL_offset_vec = IDL_offset_vec;\n");
    fprintf(fid, "IDL_ms.IDL_rtn_vec = IDL_rtn_vec;\n");
    fprintf(fid, "IDL_ms.IDL_call_h = (volatile rpc_call_handle_t)IDL_call_h;\n");
    fprintf(fid, "IDL_ms.IDL_drep = *IDL_drep_p;\n");

    fprintf(fid, "IDL_ms.IDL_elt_p = IDL_elt_p;\n");
    DDBE_spell_param_vec_init( fid, p_operation, BE_server_side,
                               BE_cmd_opt, BE_cmd_val );
    fprintf(fid, "IDL_ms.IDL_param_vec = IDL_param_vec;\n");
    fprintf(fid, "IDL_ms.IDL_side = IDL_server_side_k;\n");
    fprintf(fid, "IDL_ms.IDL_language = ");
    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx)
        fprintf(fid, "IDL_lang_cxx_k");
    else
        fprintf(fid, "IDL_lang_c_k");
    fprintf(fid, ";\n");


    /* Does operation use I-char machinery? If so, set up needed state */
    BE_spell_cs_state(fid, "IDL_ms.", BE_server_side, &cs_info);
    if (cs_info.cs_machinery)
        fprintf(fid, "IDL_ms.IDL_h=%s;\n", handle_info.assoc_name);


    /* If there are user exceptions which are not external, initialize them */
    if (num_declared_exceptions != 0)
    {
        fprintf(fid,
             "RPC_SS_THREADS_ONCE(&IDL_exception_once,IDL_exceptions_init);\n");
    }

    /*
     *  Is there a reference from this client to a context?
     */
    if (AST_HAS_IN_CTX_SET(p_operation)
        || AST_HAS_OUT_CTX_SET(p_operation))
    {
        fprintf(fid,
"rpc_ss_ctx_client_ref_count_i_2(%s,&IDL_client_id,(error_status_t*)&IDL_ms.IDL_status);\n",
              handle_info.assoc_name);
        CSPELL_test_status(fid);
    }

    /*
     * Node initializations
     */
    if (AST_HAS_IN_PTRS_SET(p_operation)
    || AST_HAS_OUT_PTRS_SET(p_operation)
    || AST_ENABLE_ALLOCATE_SET(p_operation))
    {
        fprintf( fid,
"rpc_ss_create_support_ptrs( &IDL_support_ptrs,&IDL_ms.IDL_mem_handle);\n" );
        if (AST_HAS_FULL_PTRS_SET(p_operation))
        {
            fprintf(fid,
"rpc_ss_init_node_table(&IDL_ms.IDL_node_table,&IDL_ms.IDL_mem_handle);\n");
            if (AST_REFLECT_DELETIONS_SET(p_operation))
            {
                fprintf(fid,
                     "rpc_ss_enable_reflect_deletes(IDL_ms.IDL_node_table);\n");
            }
        }
    }

    /*
     * Pipe initializations
     */
    if (AST_HAS_IN_PIPES_SET(p_operation) || AST_HAS_OUT_PIPES_SET(p_operation))
    {
        DDBE_init_server_pipes( fid, p_operation, &first_pipe );
        fprintf( fid, "IDL_current_pipe=(%d);\n", first_pipe );
    }


    /* Unmarshall the ins */

    if ((int)CMD_vals[opt_lang] == opt_lang_cxx) {
    	/* Get a handle on the object */
    	fprintf(fid, "%s *IDL_this;\n",BE_get_name(p_interface->name));
    }

        DDBE_spell_marsh_or_unmar( fid, p_operation, "rpc_ss_ndr_unmar_interp",
                                "&IDL_ms", BE_server_side, BE_unmarshalling_k );

    /* If there is I-char machinery,  call the [cs_tag_rtn] if there is one */
    BE_spell_cs_tag_rtn_call(fid, "IDL_ms.", p_operation, BE_server_side,
                             &handle_info, &cs_info, false);



    CSPELL_manager_call(fid, p_interface, p_operation,
                    rep_as_handle_param, rep_as_type_name, binding_handle_name);


    DDBE_convert_out_contexts(fid, p_operation, binding_handle_name);

    if (AST_HAS_IN_PIPES_SET(p_operation) || AST_HAS_OUT_PIPES_SET(p_operation))
    {
        fprintf( fid, "if (IDL_current_pipe != %d)\n", BE_FINISHED_WITH_PIPES );
        fprintf( fid, "{\n" );
        fprintf( fid, "RAISE(rpc_x_ss_pipe_discipline_error);\n" );
        fprintf( fid, "}\n" );
    }


    /* Marshall the outs */
    /* Debugging output, required by COM object protocol */

        fprintf(fid, "{\n");
        DDBE_spell_marsh_or_unmar( fid, p_operation, "rpc_ss_ndr_marsh_interp",
                                "&IDL_ms", BE_server_side, BE_marshalling_k );
        fprintf(fid, "if (IDL_ms.IDL_iovec.num_elt != 0)\n");


        fprintf(fid,
"  rpc_call_transmit((rpc_call_handle_t)IDL_ms.IDL_call_h,(rpc_iovector_p_t)&IDL_ms.IDL_iovec,\n");
        fprintf(fid,
            "  (unsigned32*)&IDL_ms.IDL_status);  /* Send remaining outs */\n");
    
    
        fprintf(fid, "\n}");

    /* Since goto IDL_closedown is done conditionally, there may never
     * be a reference to the label and the compilers on NT will report
     * a warning a every generated stub.  So just in case we check the 
     * status again just to at least reference the label.
     */ 

    CSPELL_test_status(fid);

    fprintf(fid, "\nIDL_closedown: ;\n");
    fprintf(fid, "CATCH_ALL\n");

    fprintf(fid, "if ( IDL_manager_entered )\n{\n");
    fprintf(fid,
              "RPC_SS_THREADS_RESTORE_GENERAL(IDL_general_cancel_state);\n");
    fprintf(fid, "RPC_SS_THREADS_RESTORE_ASYNC(IDL_async_cancel_state);\n");
    fprintf(fid, "}\n");


    /*
     * For all exceptions other than report status, send the exception to the
     * client.  For the report status exception, just fall through and
     * perform the normal failing status reporting.
     */
        fprintf(fid, "rpc_ss_ndr_clean_up(&IDL_ms);\n"); 
    fprintf(fid,
         "if (!RPC_SS_EXC_MATCHES(THIS_CATCH,&rpc_x_ss_pipe_comm_error))\n{\n");
    fprintf(fid, "if ( ! IDL_manager_entered )\n{\n");
    if ( ! explicit_binding )
    {
        fprintf(fid, "IDL_ms.IDL_status = rpc_s_manager_not_entered;\n");
    }
    fprintf(fid, "}\n");
    if ( ! explicit_binding )
    {
        fprintf(fid, "else\n");
    }
    fprintf(fid,"{\n");

    fprintf(fid,"rpc_ss_send_server_exception_2(IDL_call_h,THIS_CATCH,%d,%s,&IDL_ms);\n",
            num_declared_exceptions + num_extern_exceptions,
            (num_declared_exceptions + num_extern_exceptions) ? "IDL_exception_addresses" : "0"
	    );
    fprintf(fid, "IDL_ms.IDL_status = error_status_ok;\n}\n");
    fprintf(fid, "}\n");
    fprintf(fid, "ENDTRY\n");


    if (AST_HAS_IN_CTX_SET(p_operation)
        || AST_HAS_OUT_CTX_SET(p_operation))
    {
        fprintf(fid,
             "rpc_ss_ctx_client_ref_count_d_2(%s, IDL_client_id);\n",
              handle_info.assoc_name);
    }

    if (AST_HAS_IN_PTRS_SET(p_operation)
    || AST_HAS_OUT_PTRS_SET(p_operation)
    || AST_ENABLE_ALLOCATE_SET(p_operation))
    {
        fprintf( fid, "rpc_ss_destroy_support_ptrs();\n" );
    }

    fprintf(fid, "if (IDL_ms.IDL_mem_handle.memory)\n{\n");
    fprintf(fid, " rpc_ss_mem_free(&IDL_ms.IDL_mem_handle);\n}\n");
    fprintf(fid, "if (IDL_ms.IDL_status != error_status_ok)\n{\n");
    fprintf(fid, "if (IDL_ms.IDL_status == rpc_s_call_cancelled)\n{\n");
    fprintf(fid,   "rpc_ss_send_server_exception(");
    fprintf(fid,      "IDL_call_h,&RPC_SS_THREADS_X_CANCELLED);\n");
    fprintf(fid,   "IDL_ms.IDL_status = error_status_ok;\n");
    fprintf(fid,  "}\nelse\n{\n");
    if ( ! explicit_binding )
    {
        fprintf(fid, "if (IDL_manager_entered)\n");
    }
    fprintf(fid,   "{\nrpc_ss_send_server_exception(");
    fprintf(fid,      "IDL_call_h,&rpc_x_ss_remote_comm_failure);\n");
    fprintf(fid,   "IDL_ms.IDL_status = error_status_ok;\n");
    fprintf(fid, "}\n}\n}\n");

    /* When reached here IDL_ms.IDL_status is either error_status_ok
     * or rpc_s_manager_not_entered
     */
    fprintf(fid, "*IDL_status_p = IDL_ms.IDL_status;\n");




    fprintf(fid, "}\n");
}


/******************************************************************************/
/*                                                                            */
/*    Spells a interface manager object for C++ support                       */
/*                                                                            */
/******************************************************************************/

void DDBE_spell_interface_mgr_object
#ifdef PROTO
(
    FILE *fid,                      /* Handle for emitted C text */
    AST_interface_n_t *p_interface  /* pointer to interface      */
)
#else
(fid, p_interface)
FILE *fid;                         /* Handle for emitted C text */
AST_interface_n_t *p_interface;
#endif
{
    /*
     * Emit an interface manager object
     */
    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
        fprintf (fid, INCLUDE_TEMPLATE, "ifacemgr.hxx");
        fprintf (fid, "IfaceMgr %sIfaceMgr (\n",
		BE_get_name(p_interface->name));
	fprintf (fid, "    %sIID,\n", BE_get_name(p_interface->name));
	fprintf(fid, "    %s_v%d_%d_s_ifspec,\n",
		    BE_get_name(p_interface->name),
		    (p_interface->version%65536), 
		    (p_interface->version/65536));
        fprintf (fid, "    1\n");
        fprintf (fid, ");\n");
    }

}


/******************************************************************************/
/*                                                                            */
/*    Spells a stub dll load function                                         */
/*                                                                            */
/******************************************************************************/

void DDBE_spell_stub_dll_load
#ifdef PROTO
(
    FILE *fid,                      /* Handle for emitted C text */
    AST_interface_n_t *p_interface  /* pointer to interface      */
)
#else
(fid, p_interface)
FILE *fid;                         /* Handle for emitted C text */
AST_interface_n_t *p_interface;
#endif
{
    char *s;
    /*
     * Emit an interface manager object
     */
    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
	/*
        ** copy the server stub file name to local memory and break
        ** off the extension to spell the function name
        */
        s = (char *) malloc(strlen((char *) BE_cmd_val[opt_sstub]) + 1);
        strcpy(s, (char *) BE_cmd_val[opt_sstub]);
        fprintf (fid, "void %s_init(void) {\n", strtok(s, "."));
        free(s);
        fprintf (fid, "IfaceMgr %sIfaceMgr (\n",
		BE_get_name(p_interface->name));
	fprintf (fid, "    %sIID,\n", BE_get_name(p_interface->name));
	fprintf(fid, "    %s_v%d_%d_s_ifspec,\n",
		    BE_get_name(p_interface->name),
		    (p_interface->version%65536), 
		    (p_interface->version/65536));
        fprintf (fid, "    1\n");
        fprintf (fid, ");\n");
        fprintf (fid, "}\n");
    }

}

/*
 * BE_gen_sstub
 *
 * Public entry point for server stub file generation
 */
void BE_gen_sstub
#ifdef PROTO
(
    FILE *fid,              /* Handle for emitted C text */
    AST_interface_n_t *p_interface,     /* Ptr to AST interface node */
    language_k_t language,  /* Language stub is to interface to */
    char header_name[],     /* Name of header file to be included in stub */
    boolean *cmd_opt,
    void **cmd_val,
    DDBE_vectors_t *dd_vip    /* Data driven BE vector information ptr */
)
#else
(fid, p_interface, language, header_name,  cmd_opt, cmd_val, dd_vip)
    FILE *fid;
    AST_interface_n_t *p_interface;
    language_k_t language;
    char header_name[];
    boolean *cmd_opt;
    void **cmd_val;
    DDBE_vectors_t *dd_vip;
#endif
{
    AST_export_n_t *p_export;
    AST_operation_n_t *p_operation;
    boolean first;
    /* Exceptions may be declared or external. We need a count of both */
    int num_declared_exceptions;
    int num_extern_exceptions;
    AST_include_n_t	*incp;
    char		*fn_str;



    /*
     * Emit a #pragma nostandard to suppress warnings on non-standard C usage
     */
    fprintf(fid, "#ifdef VMS\n#pragma nostandard\n#endif\n");

    /*
     * Emit #defines and #includes
     */
    CSPELL_mts_includes(fid, header_name);

    for (incp = p_interface->sincludes; incp; incp = incp->next) {
	    STRTAB_str_to_string (incp->simple_file_name, &fn_str);
	    fprintf (fid, USER_INCLUDE_H_TEMPLATE, fn_str);
    }

    /*
     *  Set up interpreter data structures
     */
    DDBE_spell_offset_vec( fid, dd_vip, cmd_opt, cmd_val );
    DDBE_spell_rtn_vec( fid, dd_vip, cmd_opt, cmd_val, FALSE );
    DDBE_spell_type_vec( fid, dd_vip, cmd_opt, cmd_val );


    /* If there are any user exceptions, emit the necessary declarations */
    DDBE_user_exceptions(fid, p_interface,
                         &num_declared_exceptions, &num_extern_exceptions);

    /*
     *  Emit manager entry point vector, if requested
     */
    if (cmd_opt[opt_mepv])
	CSPELL_manager_epv(fid, p_interface);


    /*
     * Emit operation definitions
     */
    for (p_export = p_interface->exports; p_export; p_export = p_export->next)
        if ((p_export->kind == AST_operation_k)
                && ( ! AST_ENCODE_SET(p_export->thing_p.exported_operation))
                && ( ! AST_DECODE_SET(p_export->thing_p.exported_operation))
                   )
        {
            BE_push_malloc_ctx();
            NAMETABLE_set_temp_name_mode();
            p_operation = p_export->thing_p.exported_operation;
            CSPELL_server_stub_routine(fid, language, p_interface, p_operation,
                                num_declared_exceptions, num_extern_exceptions);
            NAMETABLE_clear_temp_name_mode();
            BE_pop_malloc_ctx();
        }

    /*
     * Emit server epv
     */
    fprintf (fid, "\nstatic rpc_v2_server_stub_proc_t IDL_epva[] = \n{\n");
    first = true;
    for (p_export = p_interface->exports; p_export; p_export = p_export->next)
        if ((p_export->kind == AST_operation_k) 
           )
        {
            if (first)
                first = false;
            else
                fprintf(fid, ",\n");
            if (AST_ENCODE_SET(p_export->thing_p.exported_operation)
                || AST_DECODE_SET(p_export->thing_p.exported_operation))
            {
                fprintf(fid, "NULL");
            }
            else
                fprintf(
                    fid, " (rpc_v2_server_stub_proc_t)op%ld_ssr",
                    p_export->thing_p.exported_operation->op_number
                    );
        }
    fprintf (fid, "\n};\n");

    /*
     * Emit static if_spec definition and global exported pointer
     */
    CSPELL_interface_def(fid, p_interface, BE_server_stub_k, cmd_opt[opt_mepv]);

    DDBE_spell_interface_mgr_object( fid, p_interface);
    DDBE_spell_stub_dll_load( fid, p_interface);

    /*
     * Emit a closing #pragma standard to match the nostandard pragma above
     */
    fprintf(fid, "#ifdef VMS\n#pragma standard\n#endif\n");
}
