/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: cstubmts.c,v $
 * Revision 1.1.7.3  1996/02/18  18:51:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:00  marty]
 *
 * Revision 1.1.7.2  1996/02/07  17:48:05  parul
 * 	DCE 1.2.1 final drop from HP
 * 	[1996/02/07  16:13:53  parul]
 * 
 * 	DCE 1.2.1 final drop from HP
 * 
 * Revision 1.1.2.3  1993/08/27  14:49:11  hinxman
 * 	OT 8513 - Spell routine calls to support pickling of data containing pointers
 * 	[1993/08/27  14:48:34  hinxman]
 * 
 * Revision 1.1.2.2  1993/07/07  19:57:32  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:31:06  ganni]
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
**      cstubmts.c
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
#include <astp.h>
#include <command.h>
#include <cspell.h>
#include <cspeldcl.h>
#include <ddbe.h>
#include <ifspec.h>
#include <commstat.h>
#include <clihamts.h>
#include <cstubgen.h>
#include <mtsbacke.h>
#include <cstubmts.h>
#include <hdgen.h>
#include <user_exc.h>
#include <icharsup.h>


BE_handle_info_t BE_handle_info;

/******************************************************************************/
/*                                                                            */
/*    Spell test of status after runtime call                                 */
/*                                                                            */
/******************************************************************************/
void CSPELL_test_status
#ifdef PROTO
(
    FILE *fid
)
#else
    (fid)
    FILE *fid;
#endif
{
    fprintf(fid,
             "if (IDL_ms.IDL_status != error_status_ok) goto IDL_closedown;\n");
}

/******************************************************************************/
/*                                                                            */
/*    Spell test of status after rpc_call_transceive                          */
/*                                                                            */
/******************************************************************************/
void CSPELL_test_transceive_status
#ifdef PROTO
(
    FILE *fid
)
#else
    (fid)
    FILE *fid;
#endif
{
    fprintf(fid, "if (IDL_ms.IDL_status != error_status_ok)\n{\n");
    fprintf(fid, "IDL_ms.IDL_elt_p = NULL;\n");
    if ( ( BE_handle_info.handle_type == BE_auto_handle_k )
          && ( ! BE_handle_info.auto_handle_idempotent_op ) )
    {
        fprintf(fid,
"IDL_ms.IDL_restartable=IDL_ms.IDL_restartable&&(!(rpc_call_did_mgr_execute\n");
        fprintf(fid,"  ((rpc_call_handle_t)IDL_ms.IDL_call_h,&IDL_st2)));\n");
    }
    fprintf(fid, "goto IDL_closedown;\n}\n");
}

/******************************************************************************/
/*                                                                            */
/*    Spell client stub routine header                                        */
/*                                                                            */
/******************************************************************************/
void CSPELL_csr_header
#ifdef PROTO
(
    FILE *fid,
    char *p_interface_name,       /* Ptr to name of interface */
    AST_operation_n_t *p_operation, /* Ptr to operation node */
    boolean use_internal_name       /* use internal name if true */
)
#else
(fid, p_interface_name, p_operation, use_internal_name)
    FILE *fid;
    char *p_interface_name;
    AST_operation_n_t *p_operation;
    boolean use_internal_name;
#endif
{
    NAMETABLE_id_t emitted_name;
    AST_parameter_n_t *handle_param;
    handle_param = NULL;

    if (use_internal_name) {
        char op_internal_name[45];
        sprintf(op_internal_name, "op%ld_csr", p_operation->op_number);
        emitted_name = NAMETABLE_add_id(op_internal_name);
        fprintf(fid, "\nstatic ");
    }
    else if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
        char op_internal_name[3 * MAX_ID];
        sprintf(op_internal_name, "%s%s::%s",
		p_interface_name,
                (!IS_CONSTRUCTOR(p_operation) &&
		 !AST_STATIC_SET(p_operation)) ?
		     "Proxy" : "", BE_get_name(p_operation->name));
        emitted_name = NAMETABLE_add_id(op_internal_name);

	/*
	** Skip handle parameter for non-constructor & non-static
	** interface methods.
	** (Its the 'this' pointer)
	*/
        if (!IS_CONSTRUCTOR(p_operation) &&
	    !AST_STATIC_SET(p_operation)) {
            if (BE_is_handle_param(p_operation->parameters)) {
                handle_param = p_operation->parameters;
                p_operation->parameters = p_operation->parameters->next;
            }
	}
    }
    else
    {
        fprintf (fid, "\n");
            emitted_name = p_operation->name;
    }
        CSPELL_function_def_header (fid, p_operation, emitted_name);
        CSPELL_finish_synopsis (fid, p_operation->parameters);
        /* 
	** if we skipped over the handle parameter, restore the pointer to it
	*/
        if (handle_param != NULL) {
            p_operation->parameters = handle_param;
        }
}


/******************************************************************************/
/*                                                                            */
/*    Generation of a stub for a client operation call                        */
/*                                                                            */
/******************************************************************************/
static void CSPELL_client_stub_routine
#ifdef PROTO
(
    FILE *fid,                      /* Handle for emitted C text */
    AST_interface_n_t *p_interface, /* Ptr to AST interface node */
    language_k_t language,          /* Language stub is to interface to */
    AST_operation_n_t *p_operation, /* Ptr to AST operation node */
    char *p_interface_name,         /* Ptr to name of interface */
    unsigned long op_num,           /* Number of current operation */
    boolean use_internal_name,      /* use internal name if true */
    int num_declared_exceptions,    /* Count of user declared exceptions */
    int num_extern_exceptions       /* Count of user extern_exceptions */
)
#else
(fid, p_interface, language, p_operation, p_interface_name,
 op_num, use_internal_name, num_declared_exceptions, num_extern_exceptions)
    FILE *fid;
    AST_interface_n_t *p_interface;
    language_k_t language;
    AST_operation_n_t *p_operation;
    char *p_interface_name;
    unsigned long op_num;
    boolean use_internal_name;
    int num_declared_exceptions;
    int num_extern_exceptions;
#endif
{
    BE_stat_info_t comm_stat_info;
    BE_stat_info_t fault_stat_info;
    BE_cs_info_t cs_info;           /* I-char machinery description */
    AST_parameter_n_t * pp;
    int comma;

    /* What sort of status reporting? */
    BE_get_comm_stat_info( p_operation, &comm_stat_info );
    BE_get_fault_stat_info( p_operation, &fault_stat_info );

    /* Routine header */
    CSPELL_csr_header(fid, p_interface_name, p_operation,
        use_internal_name);

    fprintf (fid, "{\n");

    /*
    ** If we skipped over the handle parameter in the synopsis,
    ** add it as a local variable.
    */
    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
        if (!IS_CONSTRUCTOR(p_operation) &&
	    !AST_STATIC_SET(p_operation)) {
            CSPELL_var_decl (fid, p_operation->parameters->type,
                                  p_operation->parameters->name);
        }
    }


    /*
     * Analyze the association handle the call is being made on;
     * declare assoc_handle if necessary
     */
    BE_setup_client_handle (fid, p_interface, p_operation, &BE_handle_info);

    /*
     * Does operation use I-char machinery? If so, declare any needed variables
     */
    BE_cs_analyze_and_spell_vars(fid, p_operation, BE_client_side, &cs_info);

    fprintf(fid, "\n");



    /*
     * Standard local variables
     */
    fprintf(fid, "rpc_transfer_syntax_t IDL_transfer_syntax;\n");
    fprintf(fid, "rpc_iovector_elt_t IDL_outs;\n");
    fprintf(fid, "volatile ndr_ulong_int IDL_fault_code=error_status_ok;\n");
    fprintf(fid, "volatile ndr_ulong_int IDL_user_fault_id;\n");
    fprintf(fid,
           "volatile RPC_SS_THREADS_CANCEL_STATE_T IDL_async_cancel_state;\n");
    fprintf(fid, "IDL_ms_t IDL_ms;\n");
    fprintf(fid, "idl_byte IDL_stack_packet[IDL_STACK_PACKET_SIZE];\n");
    DDBE_spell_param_vec_def( fid, p_operation, BE_client_side,
                              BE_cmd_opt, BE_cmd_val );

    /* If there is a function result, we need somewhere to put it */
    if ( (p_operation->result->type->kind != AST_void_k)
       )
    {
        CSPELL_typed_name(fid, p_operation->result->type,
                            NAMETABLE_add_id("IDL_function_result"),
                            NULL, false, true, false);
        fprintf(fid, ";\n");
    }

    /*
     * Start of executable code
     */


    fprintf(fid, "RPC_SS_INIT_CLIENT\n");
    fprintf(fid, "RPC_SS_THREADS_DISABLE_ASYNC(IDL_async_cancel_state);\n");
    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
        if (!IS_CONSTRUCTOR(p_operation) && !AST_STATIC_SET(p_operation)) {
                fprintf(fid, "signed32 numAttempts = 1;\n");
                fprintf(fid, "signed32 retryCount = -1;\n");
        	fprintf( fid, "IDL_ms.IDL_restartable=idl_true;\n" );
                fprintf(fid,
     "RPC_SS_THREADS_ONCE(&IDL_interface_client_once,IDL_auto_handle_init);\n");
	}
    }

    if ( BE_handle_info.handle_type == BE_auto_handle_k )
    {
        fprintf( fid, "IDL_ms.IDL_restartable=idl_true;\n" );
        fprintf(fid,
     "RPC_SS_THREADS_ONCE(&IDL_interface_client_once,IDL_auto_handle_init);\n");
    }
    else
    {
	/* 
	 *  To support those platforms which do not allow both a CATCH and
	 *  FINALLY clause on the same TRY, we generate one TRY block with a
	 *  catch clause nested inside another TRY block with the FINALLY
	 *  clause.  We conditionally generate the extra TRY statement only for
	 *  non-auto handle case because for auto-handle we already have an
	 *  nest TRY block.
	 */
#ifdef NO_TRY_CATCH_FINALLY
	fprintf(fid, "TRY\n");
#endif
    }

    /*
    ** Add the code to get the handle parameter
    ** from the encapsulated object reference.
    */
    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
        if (!IS_CONSTRUCTOR(p_operation) &&
	    !AST_STATIC_SET(p_operation)) {
#if 0
		fprintf(fid, "IDL_handle = this->getHandle();\n");
#endif
        }
    }

    /* If there are user exceptions which are not external, initialize them */
    if (num_declared_exceptions != 0)
    {
        fprintf(fid,
             "RPC_SS_THREADS_ONCE(&IDL_exception_once,IDL_exceptions_init);\n");
    }

    fprintf(fid, "rpc_ss_init_marsh_state(IDL_type_vec, &IDL_ms);\n");
    fprintf(fid,
             "IDL_ms.IDL_stack_packet_status = IDL_stack_packet_unused_k;\n");
    fprintf(fid, "IDL_ms.IDL_stack_packet_addr = IDL_stack_packet;\n");
    fprintf(fid, "TRY\n");
    fprintf(fid, "IDL_ms.IDL_call_h = 0;\n");
    fprintf(fid, "IDL_ms.IDL_elt_p = NULL;\n");
    fprintf(fid, "IDL_ms.IDL_offset_vec = IDL_offset_vec;\n");
    fprintf(fid, "IDL_ms.IDL_rtn_vec = IDL_rtn_vec;\n");
    DDBE_spell_param_vec_init( fid, p_operation, BE_client_side,
                               BE_cmd_opt, BE_cmd_val );
    fprintf(fid, "IDL_ms.IDL_param_vec = IDL_param_vec;\n");
    fprintf(fid, "IDL_ms.IDL_side = IDL_client_side_k;\n");
    fprintf(fid, "IDL_ms.IDL_language = ");
    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx)
        fprintf(fid, "IDL_lang_cxx_k");
    else
        fprintf(fid, "IDL_lang_c_k");
    fprintf(fid, ";\n");

    if (AST_HAS_FULL_PTRS_SET(p_operation) &&
        (AST_HAS_IN_PTRS_SET(p_operation) || AST_HAS_OUT_PTRS_SET(p_operation)))
    {
        fprintf(fid,
"rpc_ss_init_node_table(&IDL_ms.IDL_node_table,&IDL_ms.IDL_mem_handle);\n");
    }

    if (AST_HAS_OUT_PTRS_SET(p_operation) )
    {
        /* The following call is done here to enable PRT testing */
        fprintf(fid, "rpc_ss_mts_client_estab_alloc(&IDL_ms);\n");
    }

    if (p_interface->malloc_callout_name != NAMETABLE_NIL_ID) {
        fprintf(fid, "IDL_ms.IDL_p_allocate = %s;\n", 
		BE_get_name(p_interface->malloc_callout_name));
    }
    if (p_interface->free_callout_name != NAMETABLE_NIL_ID) {
        fprintf(fid, "IDL_ms.IDL_p_free = %s;\n",
		BE_get_name(p_interface->free_callout_name));
    }

    /* Does operation use I-char machinery? If so, set up needed state */
    BE_spell_cs_state(fid, "IDL_ms.", BE_client_side, &cs_info);
    /*          And call the [cs_tag_rtn] if there is one */
    BE_spell_cs_tag_rtn_call(fid, "IDL_ms.", p_operation, BE_client_side,
                             &BE_handle_info, &cs_info, false);

    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
        if (!IS_CONSTRUCTOR(p_operation) && !AST_STATIC_SET(p_operation)) {
                fprintf(fid, "IDL_find_server:\n");
                fprintf(fid, "TRY\n");
                fprintf(fid, "IDL_handle = getHandle(&IDL_auto_handle_mutex, numAttempts, retryCount, (error_status_t *) &IDL_ms.IDL_status);\n");
                fprintf(fid, "if (IDL_ms.IDL_status!=error_status_ok) {\n");
                fprintf(fid, "IDL_ms.IDL_restartable=idl_false;\n");
                fprintf(fid, "goto IDL_closedown;\n");
                fprintf(fid, "}\n");
        }
    }
    CSPELL_call_start(fid, &BE_handle_info, p_interface, p_operation, op_num,
                        &comm_stat_info, &fault_stat_info);

    if (AST_HAS_IN_CTX_SET(p_operation)
        || AST_HAS_OUT_CTX_SET(p_operation)
        || cs_info.cs_machinery)
    {
        fprintf(fid, "IDL_ms.IDL_h=(handle_t)%c%s;\n",
                     BE_handle_info.deref_assoc, BE_handle_info.assoc_name);
    }


    /* Marshall the ins */
        DDBE_spell_marsh_or_unmar( fid, p_operation, "rpc_ss_ndr_marsh_interp",
                                "&IDL_ms", BE_client_side, BE_marshalling_k );

    fprintf(fid,"IDL_ms.IDL_elt_p = &IDL_outs;\n");
    fprintf(fid,
"rpc_call_transceive((rpc_call_handle_t)IDL_ms.IDL_call_h,(rpc_iovector_p_t)&IDL_ms.IDL_iovec,\n");
        fprintf(fid,
"  IDL_ms.IDL_elt_p,&IDL_ms.IDL_drep,(unsigned32*)&IDL_ms.IDL_status);\n");

    /* !!WORKAROUND!! for problem with buff_dealloc for [maybe] operation
                      with CN runtime */
    if (AST_MAYBE_SET(p_operation))
        fprintf(fid, "IDL_outs.buff_dealloc=NULL;\n");

    CSPELL_test_transceive_status(fid);


    /* Unmarshall the outs */
        DDBE_spell_marsh_or_unmar( fid, p_operation, "rpc_ss_ndr_unmar_interp",
                                "&IDL_ms", BE_client_side, BE_unmarshalling_k );

    fprintf(fid, "IDL_closedown: ;\n");

    /*
     * Catch the error that indicates that st has be set to a failing status
     * that should be reported, and then do normal cleanup processing.  If
     * for some reason the status is not set, then set it.
     */
    fprintf(fid, "CATCH(rpc_x_ss_pipe_comm_error)\n");
    if ( BE_handle_info.handle_type == BE_auto_handle_k )
    {
        DDBE_spell_restart_logic( fid, p_operation );

        /*
         * This label is used when no valid auto-handle binding can be
         * found.  The call to this is generated in CSPELL_bind_auto_handle
         * when no valid binding can be found.
         */
        fprintf(fid, "IDL_auto_binding_failure:;\n");
    }
    else {
    
    /*  On osf when we try a fully bound DG handle and the server is not
     *  listening, we get a rpc_s_comm_failure.  If we try a fully bound
     *  CN handle and the server is not listening, we get a 
     *  rpc_s_connect_reject.
     *
     *  However on NT we get an ept_s_server_unavailable in either case.
     *  This is an inconsistency, but after talking with a MS contact
     *  many customers are already coded to this return value, so they
     *  are reluctant to making this consistent with other platforms.
     *
     *  So for now we just spell into the stubs the ept_s_server_unavaialble
     *  exception.
     */

      if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
        if (!IS_CONSTRUCTOR(p_operation) && !AST_STATIC_SET(p_operation)) {
            fprintf(fid, "ENDTRY\n");
            fprintf(fid, "if (IDL_ms.IDL_status == rpc_s_comm_failure ||\n");
            fprintf(fid, "IDL_ms.IDL_status == rpc_s_connect_rejected ||\n");
            fprintf(fid, "IDL_ms.IDL_status == ept_s_not_registered ||\n");
            fprintf(fid, "IDL_ms.IDL_status == ept_s_server_unavailable)\n{\n");
            fprintf(fid, "if (IDL_ms.IDL_restartable)\n{\n");
            fprintf(fid, "rpc_ss_call_end_2(&IDL_ms.IDL_call_h,&IDL_fault_code,&IDL_user_fault_id,&IDL_ms.IDL_status);\n");
            fprintf(fid, "rpc_ss_ndr_clean_up(&IDL_ms);\n");
            fprintf(fid, "goto IDL_find_server;\n}\n");
            fprintf(fid, "}\n");
	}
      }
	/* 
	 *  Add the matching ENDTRY for the nested TRY/CATCH block, if
	 *  necessary, as decribed above.
	 */
#ifdef NO_TRY_CATCH_FINALLY
	fprintf(fid, "ENDTRY\n");
#endif
    }

    /*
     * Normal cleanup processing to free up resources and end the call and
     * and report any faults or failing statuses.
     */
    fprintf(fid, "FINALLY\n");

        fprintf(fid, "rpc_ss_ndr_clean_up(&IDL_ms);\n");

    /* End the call, but only if we have one to end for auto handle */
    if ( BE_handle_info.handle_type == BE_auto_handle_k )
        fprintf(fid, "if(IDL_ms.IDL_call_h!=NULL)");
    fprintf(fid,
"rpc_ss_call_end_2(&IDL_ms.IDL_call_h,&IDL_fault_code,&IDL_user_fault_id,&IDL_ms.IDL_status);\n");

    CSPELL_binding_free_if_needed( fid, &BE_handle_info );
    /* Must free user binding after ending the call */
    if ((BE_handle_info.handle_type == BE_parm_user_handle_k)
        || (BE_handle_info.handle_type == BE_impl_user_handle_k))
    {
        /* There is a user handle to unbind. As we are inside an exception
           handler, we don't want any exception the unbind causes */
        fprintf (fid, "TRY\n");
        fprintf (fid, "%s_unbind(%c%s%s, (handle_t)IDL_assoc_handle);\n",
            BE_handle_info.type_name, BE_handle_info.deref_generic,
            BE_handle_info.user_handle_name,
                                            "");
        fprintf (fid, "FINALLY\n");
    }

    /* If [represent_as] on handle_t parameter, release the handle_t */
    if ((BE_handle_info.handle_type == BE_rep_as_handle_t_k)
        || (BE_handle_info.handle_type == BE_rep_as_handle_t_p_k))
    {
        fprintf (fid, "TRY\n");
        fprintf( fid, "%s_free_inst((handle_t *)%s);\n",
                    BE_get_name(BE_handle_info.rep_as_type),
                    assoc_handle_ptr );
        fprintf (fid, "FINALLY\n");
    }

    /* Release memory allocated by stub code */
    fprintf(fid, "if (IDL_ms.IDL_mem_handle.memory)\n{\n");
    fprintf(fid, " rpc_ss_mem_free(&IDL_ms.IDL_mem_handle);\n}\n");

    /* Give status information to client, or raise the appropriate exception */
    CSPELL_return_status( fid, &comm_stat_info, &fault_stat_info,
        "IDL_ms.IDL_status",
        ( (comm_stat_info.type == BE_stat_result_k)
            || (fault_stat_info.type == BE_stat_result_k) )
                ? "IDL_function_result" : (char *)NULL,
        num_declared_exceptions + num_extern_exceptions, "&IDL_ms" );

    fprintf(fid, "RPC_SS_THREADS_RESTORE_ASYNC(IDL_async_cancel_state);\n");
    if ((BE_handle_info.handle_type == BE_parm_user_handle_k)
        || (BE_handle_info.handle_type == BE_impl_user_handle_k)
        || (BE_handle_info.handle_type == BE_rep_as_handle_t_k)
        || (BE_handle_info.handle_type == BE_rep_as_handle_t_p_k))
    {
        fprintf(fid, "ENDTRY\n");
    }
    fprintf(fid, "ENDTRY\n");


    /* Set the return value */
    if ( (p_operation->result->type->kind != AST_void_k)
       )
    fprintf(fid, "return IDL_function_result;\n");
    fprintf (fid, "}\n");

    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
	/*
	** only spell c-mapping functions for C++ static operations 
	** since they are called without the help of a V-table
	*/
        if (!IS_CONSTRUCTOR(p_operation) && !AST_STATIC_SET(p_operation))
		return;

        fprintf (fid, "extern \"C\" {\n");

	CSPELL_type_exp_simple( fid, p_operation->result->type);

        fprintf (fid, "%s_%s(\n",
	    BE_get_name(p_interface->name),
	    BE_get_name(p_operation->name));
	CSPELL_parameter_list(fid, p_operation->parameters,false);
	fprintf (fid, ")\n{\n");
	if (p_operation->result->type->kind != AST_void_k)
		fprintf (fid, "return ");
	fprintf (fid, "%s::%s(",
	    BE_get_name(p_interface->name),
	    BE_get_name(p_operation->name));

        /* Add in the parameters */
        comma = 0;
        for (pp = p_operation->parameters; pp != NULL; pp = pp->next) {
            if (AST_HIDDEN_SET(pp))
                continue;
	    /*
            if (pp->type->kind != AST_handle_k) {
	    */
                if (comma) fprintf(fid, ", \\\n");
                fprintf(fid, "    %s",BE_get_name(pp->name));
		comma = 1;
	    /*
	    }
	    */
	}
	fprintf( fid, ");\n}\n}\n"); 

    }
}

/******************************************************************************/
/*                                                                            */
/*    Stub for an operation with [encode] or [decode] attribute               */
/*                                                                            */
/******************************************************************************/
void DDBE_spell_pickling_stub
#ifdef PROTO
(
    FILE *fid,
    AST_interface_n_t *p_interface, /* Ptr to AST interface node */
    char *p_interface_name,       /* Ptr to name of interface */
    AST_operation_n_t *p_operation, /* Ptr to operation node */
    boolean use_internal_name       /* use internal name if true */
)
#else
(fid, p_interface, p_interface_name, p_operation, use_internal_name)
    FILE *fid;
    AST_interface_n_t *p_interface;
    char *p_interface_name;
    AST_operation_n_t *p_operation;
    boolean use_internal_name;
#endif
{
    boolean encode_decode;  /* True if operation has [encode] and [decode] */
    char *action_type;
    BE_stat_info_t comm_stat_info;
    BE_stat_info_t fault_stat_info;
    BE_cs_info_t cs_info;           /* I-char machinery description */

    /* What sort of status reporting? */
    BE_get_comm_stat_info( p_operation, &comm_stat_info );
    BE_get_fault_stat_info( p_operation, &fault_stat_info );


    BE_setup_client_handle (fid, p_interface, p_operation, &BE_handle_info);
    encode_decode = (AST_ENCODE_SET(p_operation) 
                                            && AST_DECODE_SET(p_operation));

    CSPELL_csr_header(fid, p_interface_name, p_operation, use_internal_name);

    fprintf(fid, "{\n");
    /*
     * Standard local variables
     */
    fprintf(fid, "volatile ndr_ulong_int IDL_fault_code=error_status_ok;\n");
    fprintf(fid, "volatile ndr_ulong_int IDL_user_fault_id;\n");
    fprintf(fid,
           "volatile RPC_SS_THREADS_CANCEL_STATE_T IDL_async_cancel_state;\n");
    fprintf(fid, "IDL_es_state_t *IDL_es_state_p;\n");
    fprintf(fid, "volatile IDL_ms_t *IDL_msp;\n");
    fprintf(fid, "idl_es_transfer_syntax_t IDL_es_transfer_syntax;\n");
    DDBE_spell_param_vec_def( fid, p_operation, BE_client_side,
                              BE_cmd_opt, BE_cmd_val );

    /* If there is a function result, we need somewhere to put it */
    if ( (p_operation->result->type->kind != AST_void_k)
       )
    {
        CSPELL_typed_name(fid, p_operation->result->type,
                            NAMETABLE_add_id("IDL_function_result"),
                            NULL, false, true, false);
        fprintf(fid, ";\n");
    }

    /*
     * Does operation use I-char machinery? If so, declare any needed variables
     */
    BE_cs_analyze_and_spell_vars(fid, p_operation, BE_client_side, &cs_info);


    /*
     * Start of executable code
     */
    fprintf(fid, "RPC_SS_INIT_CLIENT\n");
    fprintf(fid, "RPC_SS_THREADS_DISABLE_ASYNC(IDL_async_cancel_state);\n");
    fprintf(fid, "IDL_es_state_p = (IDL_es_state_t *)%c%s;\n",
                BE_handle_info.deref_assoc, BE_handle_info.assoc_name);
    fprintf(fid, "IDL_msp = (volatile IDL_ms_t *)IDL_es_state_p->IDL_msp;\n");
    fprintf(fid, "IDL_msp->IDL_offset_vec = IDL_offset_vec;\n");
    fprintf(fid, "IDL_msp->IDL_rtn_vec = IDL_rtn_vec;\n");
    fprintf(fid, "TRY\n");
    DDBE_spell_param_vec_init( fid, p_operation, BE_client_side,
                               BE_cmd_opt, BE_cmd_val );
    fprintf(fid, "IDL_msp->IDL_param_vec = IDL_param_vec;\n");
    fprintf(fid, "IDL_msp->IDL_side = IDL_client_side_k;\n");
    fprintf(fid, "IDL_msp->IDL_language = ");
    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx)
        fprintf(fid, "IDL_lang_cxx_k");
    else
        fprintf(fid, "IDL_lang_c_k");
    fprintf(fid, ";\n");

    if (AST_HAS_FULL_PTRS_SET(p_operation) &&
        (AST_HAS_IN_PTRS_SET(p_operation) || AST_HAS_OUT_PTRS_SET(p_operation)))
    {
        fprintf(fid,
"rpc_ss_init_node_table(&IDL_msp->IDL_node_table,&IDL_msp->IDL_mem_handle);\n");
    }

    if (AST_HAS_OUT_PTRS_SET(p_operation) )
    {
        fprintf(fid, "rpc_ss_mts_client_estab_alloc(IDL_msp);\n");
    }

    /* Does operation use I-char machinery? If so, set up needed state */
    BE_spell_cs_state(fid, "IDL_msp->", BE_client_side, &cs_info);
    if (cs_info.cs_machinery)
        fprintf(fid, "IDL_msp->IDL_h=NULL;\n");

    if (encode_decode)
        action_type = "IDL_both_k";
    else if (AST_ENCODE_SET(p_operation))
        action_type = "IDL_encoding_k";
    else
        action_type = "IDL_decoding_k";
    fprintf(fid, 
"idl_es_before_interp_call(%c%s,(rpc_if_handle_t)&IDL_ifspec,\n",
                BE_handle_info.deref_assoc, BE_handle_info.assoc_name);
    fprintf(fid, 
"  IDL_type_vec,%d,%s,&IDL_es_transfer_syntax,(IDL_msp_t)IDL_msp);\n",
                p_operation->op_number, action_type);

    /* If there is I-char machinery,  call the [cs_tag_rtn] if there is one */
    BE_spell_cs_tag_rtn_call(fid, "IDL_msp->", p_operation, BE_client_side,
                             &BE_handle_info, &cs_info, true);

        if (encode_decode)
            fprintf(fid, 
                    "if (IDL_es_state_p->IDL_action == IDL_encoding_k)\n{\n");
        if (AST_ENCODE_SET(p_operation))
            DDBE_spell_marsh_or_unmar( fid, p_operation,
                                       "rpc_ss_ndr_marsh_interp",
                                       "(IDL_msp_t)IDL_msp",
                                       BE_client_side, BE_marshalling_k );
        if (encode_decode)
            fprintf(fid, "}\nelse\n{\n");
        if (AST_DECODE_SET(p_operation))
            DDBE_spell_marsh_or_unmar( fid, p_operation,
                                       "rpc_ss_ndr_unmar_interp",
                                       "(IDL_msp_t)IDL_msp",
                                       BE_client_side, BE_unmarshalling_k );
        if (encode_decode)
            fprintf(fid, "}\n");
    fprintf(fid, "idl_es_after_interp_call((IDL_msp_t)IDL_msp);\n");
    fprintf(fid, "CATCH(rpc_x_ss_pipe_comm_error)\n");
    fprintf(fid, "FINALLY\n");

    /* Clean-up code */
    fprintf(fid, "idl_es_clean_up((IDL_msp_t)IDL_msp);\n");

    /* Give status information to client, or raise the appropriate exception */
    CSPELL_return_status( fid, &comm_stat_info, &fault_stat_info,
        "IDL_msp->IDL_status",
        ( (comm_stat_info.type == BE_stat_result_k)
            || (fault_stat_info.type == BE_stat_result_k) )
                ? "IDL_function_result" : (char *)NULL,
        0, "(IDL_msp_t)IDL_msp" );
    fprintf(fid, "RPC_SS_THREADS_RESTORE_ASYNC(IDL_async_cancel_state);\n");

    fprintf(fid, "ENDTRY\n");

    /* Set the return value */
    if ( (p_operation->result->type->kind != AST_void_k)
       )
    {
        fprintf(fid, "return IDL_function_result;\n");
    }
    fprintf (fid, "}\n");
}

/******************************************************************************/
/*                                                                            */
/*    Spells a proxy manager object for C++ support                           */
/*                                                                            */
/******************************************************************************/
void DDBE_spell_proxy_mgr_object
#ifdef PROTO
(
    FILE *fid,                      /* Handle for emitted C text */
    char *p_interface_name,
    AST_interface_n_t *p_interface  /* pointer to interface      */
)
#else
(fid, p_interface_name, p_interface)
FILE *fid;                         /* Handle for emitted C text */
char *p_interface_name;
AST_interface_n_t *p_interface;
#endif
{
    /*
     * Emit a proxy manager object
     */
    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
        fprintf (fid, INCLUDE_TEMPLATE, "proxymgr.hxx");
        fprintf (fid, "ProxyMgr %sProxyMgr (\n", p_interface_name);
        fprintf (fid, "    %sIID,\n", p_interface_name);
        fprintf (fid, "    (ProxyCreator) %sProxy::createProxy\n",
		p_interface_name);
        fprintf (fid, ");\n");
    }
}

/******************************************************************************/
/*                                                                            */
/*    Spells a client interface manager object for C++ support                */
/*                                                                            */
/******************************************************************************/

static void DDBE_spell_interface_mgr_object
#ifdef PROTO
(
    FILE *fid,                      /* Handle for emitted C text */
    char *p_interface_name,	    /* interface name		 */
    AST_interface_n_t *p_interface  /* pointer to interface      */
)
#else
(fid, p_interface_name, p_interface)
FILE *fid;                         /* Handle for emitted C text */
char *p_interface_name,	    	   /* interface name	        */
AST_interface_n_t *p_interface;
#endif
{
    /*
     * Emit an interface manager object
     */
    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
        fprintf (fid, INCLUDE_TEMPLATE, "ifacemgr.hxx");
        fprintf (fid, "CIfaceMgr %sCIfaceMgr (\n", p_interface_name);
	fprintf (fid, "    %sIID,\n", p_interface_name);
	fprintf(fid, "    %s_v%d_%d_c_ifspec\n",
		    p_interface_name,
		    (p_interface->version%65536), 
		    (p_interface->version/65536));
        fprintf (fid, ");\n");
    }
}

/******************************************************************************/
/*                                                                            */
/*    Spells a proxy DLL stub load function				      */
/*                                                                            */
/******************************************************************************/
void DDBE_spell_proxy_dll_load
#ifdef PROTO
(
    FILE *fid,                      /* Handle for emitted C text */
    char *p_interface_name,
    AST_interface_n_t *p_interface  /* pointer to interface      */
)
#else
(fid, p_interface_name, p_interface)
FILE *fid;                         /* Handle for emitted C text */
char *p_interface_name;
AST_interface_n_t *p_interface;
#endif
{
    char *s;
    char *t;

    /*
     * Emit a proxy DLL load function 
     */
    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {
	/*
	** copy the client stub file name to local memory and break
	** off the extension and any directory prefix to spell the 
	** function name.
	*/
	t = (char *) malloc(strlen((char *) BE_cmd_val[opt_cstub]) + 1);
	strcpy(t, (char *) BE_cmd_val[opt_cstub]);
	if (s = strrchr(t, '/'))
		s++;
	else
		s = t;
        fprintf (fid, "void %s_init(void) {\n", strtok(s, "."));
	free(t);
        fprintf (fid, "ProxyMgr %sProxyMgr (\n", p_interface_name);
        fprintf (fid, "    %sIID,\n", p_interface_name);
        fprintf (fid, "    (ProxyCreator) %sProxy::createProxy\n",
		p_interface_name);
        fprintf (fid, ");\n");
        fprintf (fid, "CIfaceMgr %sCIfaceMgr (\n", p_interface_name);
	fprintf (fid, "    %sIID,\n", p_interface_name);
	fprintf(fid, "    %s_v%d_%d_c_ifspec\n",
		    p_interface_name,
		    (p_interface->version%65536), 
		    (p_interface->version/65536));
        fprintf (fid, ");\n");
        fprintf (fid, "}\n");
    }
}


/******************************************************************************/
/*                                                                            */
/*    Main control flow for generating a client stub                          */
/*                                                                            */
/******************************************************************************/
void DDBE_gen_cstub
#ifdef PROTO
(
    FILE *fid,                      /* Handle for emitted C text */
    AST_interface_n_t *p_interface, /* Ptr to AST interface node */
    language_k_t language,          /* Language stub is to interface to */
    char header_name[],         /* Name of header file to be included in stub */
    boolean *cmd_opt,
    void **cmd_val,
    DDBE_vectors_t *dd_vip    /* Data driven BE vector information ptr */
)
#else
(fid, p_interface, language, header_name, cmd_opt, cmd_val, dd_vip)
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
    char *p_interface_name;
    boolean first;
    /* Exceptions may be declared or external. We need a count of both */
    int num_declared_exceptions;
    int num_extern_exceptions;
    AST_include_n_t	*incp;
    char		*fn_str;
    AST_export_n_t    *ep;
    AST_interface_n_t   *ifp = p_interface;
    char              c_map_name[500];
    NAMETABLE_id_t    sav_param_id;

    /*
     * Emit a #pragma nostandard to suppress warnings on non-standard C usage
     */
    fprintf(fid, "#ifdef VMS\n#pragma nostandard\n#endif\n");

    /*
     * Emit #defines and #includes
     */
    CSPELL_mts_includes(fid, header_name);

    NAMETABLE_id_to_string(p_interface->name, &p_interface_name);

    for (incp = p_interface->cincludes; incp; incp = incp->next) {
	    STRTAB_str_to_string (incp->simple_file_name, &fn_str);
	    fprintf (fid, USER_INCLUDE_H_TEMPLATE, fn_str);
    }

    /*
     * Emit if_spec definition
     */
    CSPELL_interface_def(fid, p_interface, BE_client_stub_k, false);

    /* If necessary, emit statics needed for [auto_handle] */
    if ( AST_AUTO_HANDLE_SET(p_interface) )
    {
        CSPELL_auto_handle_statics( fid );
    }
    else
    	if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx)
        	CSPELL_auto_handle_statics( fid );

    /* If there is an implicit handle, declare it */
    if (p_interface->implicit_handle_name != NAMETABLE_NIL_ID)
    {
            fprintf( fid, "globaldef " );
        if ( ! AST_IMPLICIT_HANDLE_G_SET(p_interface) )
        {
            fprintf(fid, "handle_t ");
        }
        else
        {
            spell_name( fid, p_interface->implicit_handle_type_name);
            fprintf( fid, " " );
        }
        spell_name( fid, p_interface->implicit_handle_name);
        fprintf( fid, ";\n" );
    }

    /* If there are any user exceptions, emit the necessary declarations */
    DDBE_user_exceptions(fid, p_interface,
                         &num_declared_exceptions, &num_extern_exceptions);
    /*
     *  Set up interpreter data structures
     */
    DDBE_spell_offset_vec( fid, dd_vip, cmd_opt, cmd_val );
    DDBE_spell_rtn_vec( fid, dd_vip, cmd_opt, cmd_val, TRUE );
    DDBE_spell_type_vec( fid, dd_vip, cmd_opt, cmd_val );

    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx)
        BE_gen_cxx_class(fid, p_interface, stub_class);
    DDBE_spell_proxy_mgr_object(fid, p_interface_name, p_interface);
    DDBE_spell_interface_mgr_object(fid, p_interface_name, p_interface);
    DDBE_spell_proxy_dll_load(fid, p_interface_name, p_interface);

    /*
     * Emit operation definitions
     */
    for (p_export = p_interface->exports; p_export; p_export = p_export->next)
    {
        if ((p_export->kind == AST_operation_k)
           )
        {
            BE_push_malloc_ctx();
            NAMETABLE_set_temp_name_mode();
            p_operation = p_export->thing_p.exported_operation;
            if (!AST_NO_CODE_SET(p_operation))
            {
                if (AST_ENCODE_SET(p_operation) || AST_DECODE_SET(p_operation))
                    DDBE_spell_pickling_stub(fid, p_interface, 
                        p_interface_name, p_operation, cmd_opt[opt_cepv]);
                else
                    CSPELL_client_stub_routine(fid, p_interface, language,
                        p_operation, p_interface_name, p_operation->op_number,
                        cmd_opt[opt_cepv], num_declared_exceptions,
                        num_extern_exceptions);
            }
            NAMETABLE_clear_temp_name_mode();
            BE_pop_malloc_ctx();
        }
    }



    if (cmd_opt[opt_cepv]) {
        /*
         * Emit EPV declarations
         */
        fprintf(fid, "/* global */ %s_v%d_%d_epv_t %s_v%d_%d_c_epv = {\n",
            p_interface_name,
            (p_interface->version%65536), (p_interface->version/65536),
            p_interface_name,
            (p_interface->version%65536), (p_interface->version/65536));

        first = true;

        for (p_export = p_interface->exports; p_export;
                p_export = p_export->next)
            if ((p_export->kind == AST_operation_k)
               )
        {
            if (first) first = false;
            else fprintf (fid, ",\n");
            p_operation = p_export->thing_p.exported_operation;
            if (!AST_NO_CODE_SET(p_operation))
            {
                fprintf(fid, " op%ld_csr", p_operation->op_number);
            }
            else fprintf(fid, " NULL");
        }
        fprintf (fid, "\n};\n");
    }

    if ((int)BE_cmd_val[opt_lang] == opt_lang_cxx) {

        fprintf(fid, "extern \"C\" {\n");
        for (ep = ifp->exports; ep; ep = ep->next)
	  if ((ep->kind == AST_operation_k))
          {
	    /* Setup a function type for the spellers */
	    AST_operation_n_t	*op = ep->thing_p.exported_operation;
	    AST_type_n_t       func_type_node;
	    char *		method_string;
    	    AST_parameter_n_t *	pp;
	    AST_parameter_n_t *handle_param;
	    AST_parameter_n_t	*new_param_p;   /* Ptr to new parameter node */
	    NAMETABLE_id_t      new_param_id;   /* Nametable id of new param */
	    AST_type_n_t	*interf_type_p; /* Node bound to inteface name*/
	    AST_pointer_n_t	*new_ptr_p;     /* Ptr to new pointer node */
	    AST_type_n_t	*new_type_p;    /* Ptr to new param type node */
    	    int comma;
    
	    func_type_node = *BE_function_p;
	    func_type_node.type_structure.function = op;
    
	    if (IS_CONSTRUCTOR(op) || IS_DESTRUCTOR(op)  || AST_STATIC_SET(op))
	    {
                continue;
	    }
	    sprintf(c_map_name, "%s_%s", 
	        BE_get_name(ifp->name),
	        BE_get_name(op->name));
	    sav_param_id = NAMETABLE_add_id(c_map_name);

	    handle_param = NULL;
	    if (BE_is_handle_param(op->parameters)) {
	        handle_param = op->parameters;
	        op->parameters = op->parameters->next;
	    }
	    /* Add the 'this' parameter */
	    new_param_id = NAMETABLE_add_id("obj");
	    new_param_p = AST_parameter_node(new_param_id);

	    interf_type_p = (AST_type_n_t *)NAMETABLE_lookup_binding(ifp->name);

	    new_ptr_p   = AST_pointer_node(interf_type_p);
	    new_type_p  = AST_type_node(AST_pointer_k);
	    new_type_p->type_structure.pointer = new_ptr_p;
	    new_type_p->kind = AST_pointer_k;
	    new_type_p->name = NAMETABLE_NIL_ID;

	    new_param_p->type = new_type_p;
	    new_param_p->name = new_param_id;
	    new_param_p->uplink = op;
	    AST_SET_REF(new_type_p);
	    AST_SET_IN(new_param_p);

	    /* put the 'this' pointer first on the parm list */
	    new_param_p->next = op->parameters;
    	    if (new_param_p->next != NULL) {
	        new_param_p->last = new_param_p->next->last;
	        new_param_p->next->last = NULL;
	    }
	    op->parameters = new_param_p;
	    CSPELL_typed_name (fid, &func_type_node, sav_param_id,
			BE_ulong_int_p, false, true, false);
	    op->parameters = new_param_p->next;
	    if (handle_param != NULL) {
	        op->parameters = handle_param;
	    }
            fprintf(fid, "{\n");
	    if (op->result->type->kind != AST_void_k) {
                fprintf(fid, "return ");
	    }
            fprintf(fid, "obj->%s(\n", BE_get_name(op->name));
            /* Add in the parameters */
            comma = 0;
            for (pp = op->parameters; pp != NULL; pp = pp->next) {
                if (AST_HIDDEN_SET(pp)) continue;
	        if (pp->type->kind == AST_handle_k) continue;
                if (comma) fprintf(fid, ",\n");
                fprintf(fid, "    %s",BE_get_name(pp->name));
	        comma = 1;
	    }
            fprintf(fid, ");\n");
            fprintf(fid, "}\n");
        }
        fprintf(fid, "}\n");
	/* generate wrappers to the bind functions */
	fprintf(fid, "extern \"C\" {\n");

	fprintf(fid, "\n%s *%s_bind_by_name(unsigned_char_t *name) {\n",
		p_interface_name, p_interface_name);
	fprintf(fid, "return %s::bind(name);\n",
		p_interface_name);
	fprintf(fid, "}\n");

	fprintf(fid, "\n%s *%s_bind_by_uuid(uuid_t * u) {\n",
		p_interface_name, p_interface_name);
	fprintf(fid, "return %s::bind(*u);\n",
		p_interface_name);
	fprintf(fid, "}\n");

	fprintf(fid, "\n%s *%s_bind_by_hndl(rpc_binding_handle_t bh) {\n",
		p_interface_name, p_interface_name);
	fprintf(fid, "return %s::bind(bh);\n",
		p_interface_name);
	fprintf(fid, "}\n");


	fprintf(fid, "\n%s *%s_bind_by_iface(void *iface) {\n",
		p_interface_name, p_interface_name);
	fprintf(fid, "return %s::bind((basecast *) iface);\n",
		p_interface_name);
	fprintf(fid, "}\n");

	fprintf(fid, "\nvoid %s_destroy(%s *obj) {\n",
		p_interface_name, p_interface_name);
	fprintf(fid, "delete obj;\n return;\n");
	fprintf(fid, "}\n");


	fprintf(fid, "\nvoid %s_secure(\n", p_interface_name);
	fprintf(fid, "    %s *obj,\n", p_interface_name);
	fprintf(fid, "    unsigned_char_t * principal,\n");
	fprintf(fid, "    unsigned32 protection,\n");
	fprintf(fid, "    unsigned32 authorization,\n");
	fprintf(fid, "    rpc_auth_identity_handle_t authid,\n");
	fprintf(fid, "    unsigned32 authorz_svc) {\n");

	fprintf(fid, "    obj->secure(\n");

	fprintf(fid, "        (unsigned char *) principal,\n");
	fprintf(fid, "        protection,\n");
	fprintf(fid, "        authorization,\n");
	fprintf(fid, "        authid,\n");
	fprintf(fid, "        authorz_svc\n");
	fprintf(fid, "    );\n}\n");

	fprintf(fid, "}\n");

    }

    /*
     * Emit a closing #pragma standard to match the nostandard pragma above
     */
    fprintf(fid, "#ifdef VMS\n#pragma standard\n#endif\n");
}
