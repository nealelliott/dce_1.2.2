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
 * $Log: utc_ops.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:18  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:24:49  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/10  19:09 UTC  truitt
 * 	Merge _e work.
 * 
 * 	HP revision /main/truitt_dcecp_chfts11/1  1995/02/10  19:07 UTC  truitt
 * 	CHFts14285: Use macros to check for extra args
 * 	in the commands.  This allows the error code to
 * 	be set easily, as well as the error message.
 * 	[1995/12/08  17:58:08  root]
 * 
 * Revision 1.1.2.5  1994/09/02  19:31:46  rousseau
 * 	Fix handling of result (CR 12035).
 * 	[1994/09/02  19:31:31  rousseau]
 * 
 * Revision 1.1.2.4  1994/08/10  19:21:24  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:15:42  annie]
 * 
 * 	expand copyright with OSF copyright text
 * 
 * 	expand copyright with OSF copyright text
 * 
 * Revision 1.1.2.3  1994/07/28  20:34:51  rousseau
 * 	64 bit changes (CR 11451).
 * 	[1994/07/28  20:33:27  rousseau]
 * 
 * Revision 1.1.2.2  1994/07/06  15:32:20  rousseau
 * 	Added code to do correct utc_* call for correct output (CR 11187).
 * 	[1994/07/06  15:31:56  rousseau]
 * 
 * Revision 1.1.2.1  1994/07/01  14:25:30  rousseau
 * 	Implemented utc subcomponent.
 * 	[1994/07/01  14:24:36  rousseau]
 * 
 * $EndLog$
 */

/*
 * MODULE: utc_ops.c
 *
 * DESCRIPTION:
 *   This module contains all of the functions that implement the
 * dcecp utc commands. 
 *
 */

/* INCLUDE FILES  */

#include <stdlib.h>
#include <dcp_utc.h>

/* DECLARATIONS */

/* INTERNAL VARIABLES */

/* PROTOTYPES */


/*
 * FUNCTION:    utc_add
 *
 * OVERVIEW:    This function adds two relative timestamps or a relative
 *              and an absolut timestamp.
 * 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
utc_add(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    utc_t                       time1, time2, sum;
    char                        result[UTC_MAX_STR_LEN];
    int                         rel_count = 0;
    boolean32                   abs_flag = FALSE;
    
    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS_TWO_ARG();

    /* Did the user not specify the arguments? */
    if (argc < 3) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_wrong_num_args, 2);
        return TCL_ERROR;
    } 
    
    /* Convert the first string into a timestamp */
    if (utc_mkascreltime(&time1, argv[1]) == 0) {
        /* We have a relative timestamp */
        rel_count++;
    } 
    else if (utc_mkasctime(&time1, argv[1]) == 0) {
        /* We have an absolute timestamp */
        abs_flag = TRUE;
    }
    else {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_utc_bad_time, argv[1]);
        return TCL_ERROR;
    }

    /* Convert the second string into a timestamp */
    if (utc_mkascreltime(&time2, argv[2]) == 0) {
        /* We have a relative timestamp */
        rel_count++;
    } 
    else if (utc_mkasctime(&time2, argv[2]) == 0) {
        /* If this is our second absolute timestamp, then complain */
        if (abs_flag) {
            DCP_SET_RESULT_CODE(dcp_s_utc_add_two_abs);
            return TCL_ERROR;
        }
    }
    else {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_utc_bad_time, argv[2]);
        return TCL_ERROR;
    }
       
    /* Add the times */
    if (utc_addtime(&sum, &time1, &time2) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_utc_add_failed);
        return TCL_ERROR;
    }

    /* Convert the result back into a string */
    if (rel_count == 2) {
        if (utc_ascreltime(result, UTC_MAX_STR_LEN, &sum) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_utc_conv_fail);
            return TCL_ERROR;
        }
    }
    else {
        if (utc_ascanytime(result, UTC_MAX_STR_LEN, &sum) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_utc_conv_fail);
            return TCL_ERROR;
        }
    }

    Tcl_SetResult(interp, result, TCL_VOLATILE);
    return TCL_OK;
}


/*
 * FUNCTION:    utc_compare
 *
 * OVERVIEW:    This function compares two absolute timestamps. Returns
 *              -1 if the first is earlier, 1 if the second is earlier
 *              or 0 if it is indeterminte. Accepts a -noinaccuracy to
 *              ignore inaccuracies in comparisons. In this case a 
 * 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
utc_compare(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    utc_t                       time1, time2;
    enum utc_cmptype            relation;
    boolean32                   abs_flag = FALSE, rel_flag = FALSE;
    boolean32                   noinacc_flag = FALSE;
    
    dcp_ArgvInfo arg_table[] = {
        {"-noinaccuracy", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_utc_compare_noinaccuracy_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&noinacc_flag;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS_TWO_ARG();

    /* Did the user not specify the arguments? */
    if (argc < 3) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_wrong_num_args, 2);
        return TCL_ERROR;
    } 
    
    /* Convert the first string into a timestamp */
    if (utc_mkascreltime(&time1, argv[1]) == 0) {
        /* We have a relative timestamp */
        rel_flag = TRUE;
    } 
    else if (utc_mkasctime(&time1, argv[1]) == 0) {
        /* We have an absolute timestamp */
        abs_flag = TRUE;
    }
    else {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_utc_bad_time, argv[1]);
        return TCL_ERROR;
    }

    /* Convert the second string into a timestamp */
    if (utc_mkascreltime(&time2, argv[2]) == 0) {
        /* We have a relative timestamp */
        if (abs_flag) {
            DCP_SET_RESULT_CODE(dcp_s_utc_compare_diff_ts);
            return TCL_ERROR;
        }
    } 
    else if (utc_mkasctime(&time2, argv[2]) == 0) {
        /* We have an absolute timestamp */
        if (rel_flag) {
            DCP_SET_RESULT_CODE(dcp_s_utc_compare_diff_ts);
            return TCL_ERROR;
        }
    }
    else {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_utc_bad_time, argv[2]);
        return TCL_ERROR;
    }
       
    if (noinacc_flag) {
        /* Compare the two times */
        if (utc_cmpmidtime(&relation, &time1, &time2) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_utc_compare_failed);
            return TCL_ERROR;
        }
    }
    else {
        /* Compare the two times */
        if (utc_cmpintervaltime(&relation, &time1, &time2) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_utc_compare_failed);
            return TCL_ERROR;
        }
    }

    switch (relation) {
      case utc_equalTo:
        Tcl_SetResult(interp, "0", TCL_STATIC);
        break;
      case utc_lessThan:
        Tcl_SetResult(interp, "-1", TCL_STATIC);
        break;
      case utc_greaterThan:
        Tcl_SetResult(interp, "1", TCL_STATIC);
        break;
      case utc_indeterminate:
        Tcl_SetResult(interp, "0", TCL_STATIC);
        break;
    }

    return TCL_OK;
}


/*
 * FUNCTION:    utc_convert
 *
 * OVERVIEW:    Convert the supplied timestamp to the local timezone or
 *              GMT if -gmt is specified.
 * 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
utc_convert(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    utc_t                       time;
    char                        result[UTC_MAX_STR_LEN];
    boolean32                   gmt_flag = FALSE;
    
    dcp_ArgvInfo arg_table[] = {
        {"-gmt", DCP_ARGV_CONSTANT, (char *)TRUE, NULL,
             dcp_t_utc_convert_gmt_help},
        {NULL, DCP_ARGV_END, NULL, NULL, 0} 
    };
    
    arg_table[0].dst = (char *)&gmt_flag;
    
    if (dcp_ParseArgv(interp, &argc, argv, arg_table, 0) != TCL_OK)
        return TCL_ERROR;
    
    /* Check to see if the user just wants help */
    DCP_CHECK_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS();

    /* Did the user not specify an argument? */
    if (argc < 2) {
        DCP_SET_RESULT_CODE(dcp_s_need_arg);
        return TCL_ERROR;
    } 
    
    if (utc_mkasctime(&time, argv[1]) != 0) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_utc_bad_time, argv[1]);
        return TCL_ERROR;
    }

    /* Convert the timestamp */
    if (gmt_flag) {
        if (utc_ascgmtime(result, UTC_MAX_STR_LEN, &time) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_utc_convert_failed);
            return TCL_ERROR;
        }
    }
    else {
        if (utc_asclocaltime(result, UTC_MAX_STR_LEN, &time) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_utc_convert_failed);
            return TCL_ERROR;
        }
    }

    Tcl_SetResult(interp, result, TCL_VOLATILE);
    return TCL_OK;
}


/*
 * FUNCTION:    utc_multiply
 *
 * OVERVIEW:    Multiply a timestamp by an integer or floating point number.
 * 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
utc_multiply(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    utc_t                       time, mult;
    double                      factor;
    char                        result[UTC_MAX_STR_LEN];
    char                        *tmp;
    
    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS_TWO_ARG();

    /* Did the user not specify the arguments? */
    if (argc < 3) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_wrong_num_args, 2);
        return TCL_ERROR;
    } 
    
    /* Convert the first string into an relative timestamp */
    if (utc_mkascreltime(&time, argv[1]) != 0) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_utc_bad_rel_time, argv[1]);
        return TCL_ERROR;
    }

    /* Convert the second string into a multiplication factor */
    factor = strtod(argv[2], &tmp);
    if (*tmp != '\0') {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_utc_bad_mult, argv[2]);
        return TCL_ERROR;
    }

    /* Multiply the timestamp */
    if (utc_mulftime(&mult, &time, factor) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_utc_multiply_failed);
        return TCL_ERROR;
    }

    /* Convert the result back into a string */
    if (utc_ascreltime(result, UTC_MAX_STR_LEN, &mult) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_utc_conv_fail);
        return TCL_ERROR;
    }

    Tcl_SetResult(interp, result, TCL_VOLATILE);
    return TCL_OK;
}


/*
 * FUNCTION:    utc_subtract
 *
 * OVERVIEW:    Returns the difference between two timestamps.
 * 
 *
 * INPUTS:
 *  argc
 *    Count of arguments in argv.
 *
 *  argv
 *    Pointer to a list of strings that contain the user supplied arguments.
 *
 * INPUT/OUTPUTS:
 *  interp
 *    A pointer to the current TCL interpreter. 
 *
 * OUTPUTS:
 *  None
 *
 * RETURNS:
 *  TCL_OK on success, TCL_ERROR on failure.
 */
int
utc_subtract(ClientData clientData, Tcl_Interp *interp, int argc, char **argv)
{
    utc_t                       time1, time2, diff;
    char                        result[UTC_MAX_STR_LEN];
    boolean32                   rel_flag = FALSE;
    int                         abs_count = 0;
    
    /* Check to see if the user just wants help */
    DCP_NO_COMMAND_HELP();

    /* Check for extraneous arguments */
    DCP_CHECK_EXTRA_ARGS_TWO_ARG();

    /* Did the user not specify the arguments? */
    if (argc < 3) {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_wrong_num_args, 2);
        return TCL_ERROR;
    } 
    
    /* Convert the first string into a timestamp */
    if (utc_mkascreltime(&time1, argv[1]) == 0) {
        /* We have a relative timestamp */
        rel_flag = TRUE;
    } 
    else if (utc_mkasctime(&time1, argv[1]) == 0) {
        /* We have an absolute timestamp */
        abs_count++;
    }
    else {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_utc_bad_time, argv[1]);
        return TCL_ERROR;
    }

    /* Convert the second string into a timestamp */
    if (utc_mkascreltime(&time2, argv[2]) == 0) {
        /* We have a relative timestamp */
    } 
    else if (utc_mkasctime(&time2, argv[2]) == 0) {
        /* 
         * We have an absolute timestamp. Subtracting an absolute timestamp 
         * from a relative one generates garbage results, so complain if this
         * is the case.
         */
        if (rel_flag) {
            DCP_SET_RESULT_CODE(dcp_s_utc_sub_abs_from_rel);
            return TCL_ERROR;
        }
        abs_count++;
    }
    else {
        DCP_SET_RESULT_CODE_ONE_ARG(dcp_s_utc_bad_time, argv[2]);
        return TCL_ERROR;
    }
       
    /* Subtract the times */
    if (utc_subtime(&diff, &time1, &time2) != 0) {
        DCP_SET_RESULT_CODE(dcp_s_utc_sub_failed);
        return TCL_ERROR;
    }

    /* Convert the result back into a string */
    if (abs_count == 1) {
        if (utc_ascanytime(result, UTC_MAX_STR_LEN, &diff) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_utc_conv_fail);
            return TCL_ERROR;
        }
    }
    else {
        if (utc_ascreltime(result, UTC_MAX_STR_LEN, &diff) != 0) {
            DCP_SET_RESULT_CODE(dcp_s_utc_conv_fail);
            return TCL_ERROR;
        }
    }

    Tcl_SetResult(interp, result, TCL_VOLATILE);
    return TCL_OK;
}
