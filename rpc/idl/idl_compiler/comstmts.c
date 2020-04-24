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
 * $Log: comstmts.c,v $
 * Revision 1.1.9.2  1996/02/18  18:51:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:04:54  marty]
 *
 * Revision 1.1.9.1  1995/12/07  22:15:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/20  19:12 UTC  jrr
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/11/17  17:02 UTC  dat  /main/dat_xidl2/1]
 * 
 * 	may 5 version from nightly
 * 	[1995/10/31  18:38:47  bfc]
 * 	 *
 * 	DCE for DEC OSF/1: populate from OSF DCE R1.1
 * 	[1995/10/31  18:37:26  bfc]
 * 	 *
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/22  15:02 UTC  mgm
 * 	merge
 * 
 * 	HP revision /main/lmm_fix_idl_umrs/1  1995/07/21  21:50 UTC  lmm
 * 	don't spell IDL_user_fault_id when no user exceptions
 * 	[1995/12/07  21:10:33  root]
 * 
 * Revision 1.1.5.1  1994/05/04  20:29:36  tom
 * 	Bug 10057 - fix non PROTO declarations.
 * 	[1994/05/04  20:26:26  tom]
 * 
 * Revision 1.1.2.2  1993/07/07  19:56:59  ganni
 * 	reduced stub idl sources
 * 	[1993/07/07  19:30:42  ganni]
 * 
 * $EndLog$
 */
/*
**  NAME:
**
**      comstmts.c
**
**  FACILITY:
**
**      Interface Definition Language (IDL) Compiler
**
**  ABSTRACT:
**
**      Deliver comm_status or raise exception in client stub
**      Deliver fault_status or raise exception in client stub
**      For MTS compiler
**
*/

#include <nidl.h>
#include <ast.h>
#include <bedeck.h>
#include <commstat.h>
#include <cspell.h>
#include <nametbl.h>

/*******************************************************************************/
/*                                                                             */
/*  Determine how comm_status is to be returned                                */
/*                                                                             */
/*******************************************************************************/
void BE_get_comm_stat_info
#ifdef PROTO
(
    AST_operation_n_t *p_operation,
    BE_stat_info_t *p_comm_stat_info
)
#else
( p_operation, p_comm_stat_info )
    AST_operation_n_t *p_operation;
    BE_stat_info_t *p_comm_stat_info;
#endif
{
    AST_parameter_n_t *p_parameter;

    if ( AST_COMM_STATUS_SET(p_operation->result) )
    {
        p_comm_stat_info->type = BE_stat_result_k;
        return;
    }
    for ( p_parameter = p_operation->parameters;
          p_parameter != NULL;
          p_parameter = p_parameter->next )
    {
        if ( AST_COMM_STATUS_SET(p_parameter) )
        {
            p_comm_stat_info->type = BE_stat_param_k;
            p_comm_stat_info->name = p_parameter->name;
            return;
        }
        else if ( AST_ADD_COMM_STATUS_SET(p_parameter) )
        {
            p_comm_stat_info->type = BE_stat_addl_k;
            p_comm_stat_info->name = p_parameter->name;
            return;
        }
    }
    p_comm_stat_info->type = BE_stat_except_k;
}

/*******************************************************************************/
/*                                                                             */
/*  Determine how fault_status is to be returned                               */
/*                                                                             */
/*******************************************************************************/
void BE_get_fault_stat_info
#ifdef PROTO
(
    AST_operation_n_t *p_operation,
    BE_stat_info_t *p_fault_stat_info
)
#else
( p_operation, p_fault_stat_info )
    AST_operation_n_t *p_operation;
    BE_stat_info_t *p_fault_stat_info;
#endif
{
    AST_parameter_n_t *p_parameter;

    if ( AST_FAULT_STATUS_SET(p_operation->result) )
    {
        p_fault_stat_info->type = BE_stat_result_k;
        return;
    }
    for ( p_parameter = p_operation->parameters;
          p_parameter != NULL;
          p_parameter = p_parameter->next )
    {
        if ( AST_FAULT_STATUS_SET(p_parameter) )
        {
            p_fault_stat_info->type = BE_stat_param_k;
            p_fault_stat_info->name = p_parameter->name;
            return;
        }
        else if ( AST_ADD_FAULT_STATUS_SET(p_parameter) )
        {
            p_fault_stat_info->type = BE_stat_addl_k;
            p_fault_stat_info->name = p_parameter->name;
            return;
        }
    }
    p_fault_stat_info->type = BE_stat_except_k;
}

/*******************************************************************************/
/*                                                                             */
/*    Spell code that returns status to client                                 */
/*                                                                             */
/*******************************************************************************/
void CSPELL_return_status
#ifdef PROTO
(
    FILE *fid,
    BE_stat_info_t *p_comm_stat_info,
    BE_stat_info_t *p_fault_stat_info,
    char *status_var_name,
    char *result_param_name,
    int num_user_exceptions,
    char *IDL_msp_name     /* Lexical form of pointer to IDL_ms_t state block */
)
#else
( fid, p_comm_stat_info, p_fault_stat_info, status_var_name, result_param_name,
  num_user_exceptions, IDL_msp_name )
    FILE *fid;
    BE_stat_info_t *p_comm_stat_info;
    BE_stat_info_t *p_fault_stat_info;
    char *status_var_name;
    char *result_param_name;
    int num_user_exceptions;
    char *IDL_msp_name     /* Lexical form of pointer to IDL_ms_t state block */
#endif
{
#define MAX_STATUS_STRING 72+MAX_ID+MAX_ID
    char *str_p_comm_status;    /* String used as parameter describing how
                                    comm status is to be returned */
    char *str_p_fault_status;    /* String used as parameter describing how
                                    fault status is to be returned */
    char *name_work;
    char comm_status_work[MAX_STATUS_STRING];
    char fault_status_work[MAX_STATUS_STRING];

    /* Handle [comm_status] parameters */
    switch( p_comm_stat_info->type )
    {
        case BE_stat_addl_k:
            /* 
             *  If an added comm_status parameter, always pass it to
             *  rpc_ss_report_error so that it will be set to either
             *  error_status_ok, or the correct status value.
             */
            NAMETABLE_id_to_string( p_comm_stat_info->name,
                                                       &str_p_comm_status );
            break;
        case BE_stat_result_k:
            /*
             * For a function result, pass the address of the surrogate, if
             * we had a comm_status-related error.  Otherwise pass NULL so
             * that rpc_ss_report_error will not overwrite the user's value
             */
            sprintf( comm_status_work,"(%s!=error_status_ok) ? &%s : NULL",
                        status_var_name, result_param_name );
            str_p_comm_status = comm_status_work;
            break;
        case BE_stat_param_k:
            /*
             *  For a user parameter, pass the parameter, if we had a
             *  comm_status-related error.  Otherwise pass NULL so that
             *  rpc_ss_report_error will not overwrite the user's value
             */
            NAMETABLE_id_to_string( p_comm_stat_info->name, &name_work );
            sprintf( comm_status_work,"(%s!=error_status_ok) ? %s : NULL",
                        status_var_name, name_work );
            str_p_comm_status = comm_status_work;
            break;
        default:
            str_p_comm_status = "NULL";
            break;
    }
    ASSERTION(strlen(str_p_comm_status) < MAX_STATUS_STRING);

    /* Handle [fault_status] parameters */
    switch( p_fault_stat_info->type )
    {
        case BE_stat_addl_k:
            /* 
             *  If an added fault_status parameter, always pass it to
             *  rpc_ss_report_error so that it will be set to either
             *  error_status_ok, or the correct status value.
             */
            NAMETABLE_id_to_string( p_fault_stat_info->name,
                                                       &str_p_fault_status );
            break;
        case BE_stat_result_k:
            /*
             *  For a function result, pass the address of the surrogate, if we
             *  had an error.  Otherwise pass NULL so that rpc_ss_report_error
             *  will not overwrite the user's value.  We need not distiguish
             *  between comm/fault errors because either will mean that the
             *  user's values has not been unmarshalled anyway and thus we
             *  should set it to error_status_ok.
             */
            sprintf( fault_status_work,"(%s!=error_status_ok) ? &%s : NULL",
                        status_var_name, result_param_name );
            str_p_fault_status = fault_status_work;
            break;
        case BE_stat_param_k:
            /*
             *  For a function result, pass the address of the surrogate, if we
             *  had an error.  Otherwise pass NULL so that rpc_ss_report_error
             *  will not overwrite the user's value.  We need not distiguish
             *  between comm/fault errors because either will mean that the
             *  user's values has not been unmarshalled anyway and thus we
             *  should set it to error_status_ok.
             */
            NAMETABLE_id_to_string( p_fault_stat_info->name, &name_work );
            sprintf( fault_status_work,"(%s!=error_status_ok) ? %s : NULL",
                        status_var_name, name_work );
            str_p_fault_status = fault_status_work;
            break;
        default:
            str_p_fault_status = "NULL";
            break;
    }
    ASSERTION(strlen(str_p_fault_status) < MAX_STATUS_STRING);

    fprintf( fid,
	     "rpc_ss_report_error_2(IDL_fault_code,%s,%s,\n",
	     num_user_exceptions ? "IDL_user_fault_id" : "0",
	     status_var_name );
    fprintf( fid, 
" (RPC_SS_THREADS_CANCEL_STATE_T *)&IDL_async_cancel_state, %s, %s, %s, %s);\n",
             str_p_comm_status, str_p_fault_status,
             num_user_exceptions ? "IDL_exception_addresses" : "NULL",
             IDL_msp_name
             );

}
