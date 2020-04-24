/* 
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * HISTORY
 * $Log: xt_dsFuncs.c,v $
 * Revision 1.1.10.2  1996/02/18  00:24:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:13:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:42  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:28:43  marrek
 * 	June 1994 code submission.
 * 	[1994/06/21  11:48:42  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:33:09  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:22:23  marrek]
 * 
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:41:30  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:42:51  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  16:06:37  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  18:12:57  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:15:37  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:28:41  marrek]
 * 
 * Revision 1.1.6.3  1993/08/12  12:19:30  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:25:45  marrek]
 * 
 * Revision 1.1.6.2  1993/07/02  15:17:36  marrek
 * 	Changes for port to HP800.
 * 	[1993/07/02  14:35:52  marrek]
 * 
 * Revision 1.1.4.3  1993/01/05  20:34:07  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:48:20  htf]
 * 
 * Revision 1.1.4.2  1992/12/01  14:59:35  marrek
 * 	November 1992 code drop
 * 	[1992/12/01  13:18:25  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:54:16  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:24:20  melman]
 * 
 * Revision 1.1.2.2  1992/06/11  20:00:30  melman
 * 	 gds drop
 * 	[1992/06/11  19:23:13  melman]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: xt_dsFuncs.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 00:24:33 $";
#endif

/*************************************************************** 
*                                                              * 
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   * 
*                ALL RIGHTS RESERVED                           * 
*                                                              * 
***************************************************************/

/*daton *****************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : xt_dsFuncs.c                                        */
/*                                                                    */
/* AUTHOR       : Cahill             D AP11                           */
/* DATE         : 07.00.1987                                          */
/*                                                                    */
/* KOMPONENTE   : XDS/XOM automatic test program                      */
/*                                                                    */
/* DOK.-NR.     :                                                     */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEMABHAENGIGKEIT: -                                         */
/*                                                                    */
/* HISTORIE     :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  | Aenderungen                    | KZ | CR# FM# */
/* 1.0      | 13.07.90| original                       |br  |     */
/*          |     |                            |    |     */
/*datoff ****************************************************************/

/************************************************************************/
/*                                                                    */
/*          Module identifikation                                     */
/*                                                                    */
/************************************************************************/

/************************************************************************/
/*                                                                    */
/*            I N C L U D E S                                         */
/*                                                                    */
/************************************************************************/

#ifdef THREADSAFE
#	include <pthread.h>
#endif
#include <stdio.h>
#include <string.h>
#include <xt.h>
#ifdef THREADSAFE
#	include <xt_thread.h>
#else
#	include <xt_nonthread.h>
#endif
#include <xt_dsFuncs.h>
#include <dsXext.h> /* for dsX_extract_attr_values */

/***********************************************************************/
/*                                                                      */
/* jmptbl- when a function name is read from the testcase file, it is   */
/*            matched against the entname field of this structure, and  */
/*            test_func is invoked                                      */
/***********************************************************************/

extern struct jump_struct jmptbl[];

extern struct parm_tbl_struct parameter_tbl[];

extern OM_descriptor        *context_tbl[];
extern OM_descriptor        *name_tbl[];
extern OM_descriptor        *entry_tbl[];
extern OM_descriptor        *ava_tbl[];
extern OM_descriptor        *changes_tbl[];
extern OM_descriptor        *new_rdn_tbl[];
extern OM_descriptor        *filter_tbl[];
extern OM_descriptor        *selection_tbl[];
extern OM_descriptor        *read_result_tbl[];
extern OM_descriptor        *status_tbl[];
extern OM_descriptor        *compare_result_tbl[];
extern OM_descriptor        *list_result_tbl[];
extern OM_descriptor        *entry_modification_list_tbl[];
extern OM_descriptor        *entry_info_selection_tbl[];
extern OM_sint              subset_tbl[];
extern OM_uint32            boolean_tbl[];
extern OM_descriptor        *search_result_tbl[];
extern DS_feature           *feature_list_tbl[];
extern OM_object_identifier     *rules_tbl[];
extern OM_return_code       return_code_tbl[];
extern OM_object        *object_tbl[];
extern struct class_tbl_struct class_tbl[];
extern OM_object_identifier	*attr_type_tbl[];
extern OM_type          type_tbl[];
extern OM_type_list     type_list_tbl[];
extern OM_exclusions        exclusions_tbl[];
extern OM_modification      modification_tbl[];
extern OM_syntax        syntax_tbl[];
extern BOOLEAN xt_do_to_string_and_back;


OM_sint 
/***********************************************************************/
			xt_ds_abandon (void)
/***********************************************************************/
{
	static const char function_name[] = "xt_ds_abandon";
    OM_sint parm1, parm2;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;
    OM_private_object   session;
    OM_sint table_type;


    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
        XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsession = %s", parm);

    /* read invoke_id from the testcase file */
    if (xt_read_int (&parm2) != XT_OK)   {
        XT_LOG_ERROR( "Error reading invoke_id parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tinvoke_id = %s", xt_itoa(parm2));

    /* if session NULL - bind was not successful - don't call ds_aban
        don */
    if (session == NULL) {
        xt_log ("\tds_abandon() not called - NULL session parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    /* call the i/f function, ds_abandon(), with the parameters read
        */
    xt_log ("\tcall ds_abandon()");

    status_return = ds_abandon ( session, parm1 );

    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR( "Error reading status returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_ABANDN) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
		} /* endif  */

		if (status_return != DS_SUCCESS) {
			if ( om_delete (status_return) != OM_SUCCESS) {
				xt_log_both1(function_name, "delete of status_return failed");
				XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
				return(XT_FAILED);
			} /* endif */
		} /* endif */

    } /* endif  */
    xt_log ("\tds_abandon() returned OK\n");
    return (XT_OK);

} /* end xt_ds_abandon */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_add_entry()                                       */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_add_entry (void)
{
	static const char function_name[] = "xt_ds_add_entry";
    OM_sint parm1, parm2, parm3, parm4;
    OM_sint rv1;
    char    parm[64];
    OM_sint is_void;
    DS_status       status_return;
    OM_sint     invokeid_return;
    OM_return_code  om_return_code;
    OM_sint table_type,name_table_type,entry_table_type;
    OM_object       name;
    OM_object       entry;
    OM_private_object   session;
    OM_private_object   context;
    OM_object       r_context;


    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm4, parm, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
        XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsession = %s", parm);

    /* read context parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &r_context) != XT_OK) {
        XT_LOG_ERROR( "Error reading context parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_CONTEXT)  && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "context parameter not found in context_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    if (table_type == XT_CONTEXT) {
        if (parm1 == 0)  {          /* default context         */

            context = DS_DEFAULT_CONTEXT;
        } else {                    /* need a private object   */

            if (om_return_code = (om_put (context, OM_REPLACE_ALL, r_context, (OM_type_list)  NULL, ((OM_value_position) NULL),
                                                                                                                 ((OM_value_position) NULL))) !=
                OM_SUCCESS)  {
                XT_LOG_ERROR( "Error calling om_put for context");
                return(XT_NOT_CALLED);
            } /* endif  */
        } /* endif  */
    } else {
        context = r_context;
    } /* endif  */


    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&context, "=context");
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */


    /* read name parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &name_table_type, &parm2, parm, &is_void, &name) != XT_OK) {
        XT_LOG_ERROR( "Error reading name parameter");
        return(XT_NOT_CALLED);
    } /* endif */


    if ((name_table_type != XT_NAME) && (name_table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "name parameter not found in name_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */


    xt_log1 ("\tname = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&name, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */


    /* read entry parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &entry_table_type, &parm3, parm, &is_void, &entry) != XT_OK) {
        XT_LOG_ERROR( "Error reading entry parameter");
        return(XT_NOT_CALLED);
    } /* endif */


    if ((entry_table_type != XT_ENTRY) && (entry_table_type != XT_PRIVATE_OBJECT))  {
        XT_LOG_ERROR( "entry parm not found in entry_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */


    xt_log1 ("\tentry = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&entry, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */


    /* if session NULL- bind wasn't successful - don't call ds_add_entry */
    if (session == NULL) {
        xt_log ("\tds_add_entry() not called - NULL session parameter");
        return(XT_NOT_CALLED);
    } /* endif */



    /* call the i/f function, ds_add_entry(), with the parameters read */
    xt_log ("\tcall ds_add_entry()");

    status_return = ds_add_entry ( session, context, name, entry, &invokeid_return );

    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR( "Error reading status returned");
        return XT_ERROR ;
    } /* endif */


    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_ADDENT) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
		} /* endif */
    } /* endif */


	/* write back objects which may have been changed by xt_to_string_and_back*/
	if (xt_do_to_string_and_back) {
		switch ( entry_table_type) {
			case XT_ENTRY:
				entry_tbl[parm3] = entry;
				break;
			case XT_PRIVATE_OBJECT:
				private_object_tbl[parm3] = entry;
				break;
			default :
				assert(FALSE);
		} /* endswitch */

		switch ( name_table_type) {
			case XT_NAME:
				name_tbl[parm2] = name;
				break;
			case XT_PRIVATE_OBJECT:
				private_object_tbl[parm2] = name;
				break;
			default :
				assert(FALSE);
		} /* endswitch */
	} /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_add_entry() returned OK\n");
    return (XT_OK);

} /* end xt_ds_add_entry */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_bind()                                            */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_bind (void)
{
	static const char function_name[] = "xt_ds_bind";
    OM_sint parm1;
    OM_sint parm2;
    OM_sint rv1, rv2;
    OM_sint is_void;
    char    session_name[64];
    char    parm[64];
    OM_object           session;
    DS_status       status_return;
    OM_descriptor        * e_session;
    OM_private_object   bound_session_return;
    OM_return_code  om_return_code;
    OM_value_position   total_number;
    OM_sint table_type;
    OM_sint xt_return_code = XT_OK;

    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, session_name, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        xt_return_code = XT_NOT_CALLED;
    } /* endif */
    if (xt_return_code == XT_OK) {
        if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
            XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
            xt_return_code = XT_NOT_CALLED;
        } /* endif */
        xt_log1 ("\tsession = %s", session_name);
    } /* endif */

    /* read workspace parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_WORKSPACE, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading workspace parameter");
        xt_return_code = XT_NOT_CALLED;
    } /* endif */
    xt_log1 ("\tworkspace = %s", parm);

    if (xt_return_code == XT_OK) {
        xt_log ("\tcall ds_bind()");
        status_return = ds_bind ( session, workspace_tbl[parm2], &bound_session_return );
    } /* endif */

    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading DS_status");
        xt_return_code = XT_ERROR;
    } /* endif */

    if ((xt_return_code == XT_OK) && (!is_void)) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_BIND) != XT_OK) {
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			xt_return_code = XT_FAILED;
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
		} /* endif */
    } /* endif */

    if (xt_read_parm (XT_RETURN_VAL, XT_SESSION, &rv2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading DS_status");
        xt_return_code = XT_ERROR;
    } /* endif */

    /*
             * At last, we know where the return session should be store
        d in the
             * session table so the value of this can be initialized to
        NULL
             * whether the func is OK so far or not. Therefore, if the f
        unction
             * returns an error for any reason the session is NULL. If e
        verything is
             * still OK, all that is left is for the bound session to be
         compared with
             * the session parameter and if OK the return session will b
        e stored
             * in the session table.
             */
    if (!is_void)
        session_tbl[rv2] = NULL;

    if ((status_return == DS_SUCCESS) && (xt_return_code == XT_OK))  {
        if (!is_void)  {

            if (xt_do_to_string_and_back) {
                OM_return_code orc = xt_to_string_and_back(&bound_session_return, "=bound_session_return");
				if (orc != OM_SUCCESS) {
					return(XT_FAILED);
				} /* endif */
            } /* endif */

            /* compare session with bound_session_return */
            /* don't compare if session was the default session  */
            if (strcmp (session_name, XT_DEF_SESSION_TOKEN)) {

                if (session->syntax  & OM_S_PRIVATE) {
                    /* public object required */
                    if (om_return_code = (om_get (session, OM_NO_EXCLUSIONS, ((OM_type_list) 0), OM_FALSE,
                                                                        ((OM_value_position)    0), ((OM_value_position) 0),
                                                                         &e_session, &total_number)) != OM_SUCCESS)  {
                        xt_log1 ("\t(NOTICE) total_number from om_get for session = %s", xt_itoa(total_number));
                        xt_log1 ("\t(NOTICE) om_return_code from om_get for session = %s", xt_itoa(om_return_code));
                        xt_log ("\t(ERROR) om_return_code   from om_get for r_status not OM_SUCCESS");
                        XT_LOG_ERROR( "om_get for session failed");
                        xt_return_code = XT_ERROR;
                    } /* endif */
                } else {
                    e_session = session;
                } /* endif */

                if (xt_cmp_session (bound_session_return, e_session) != XT_OK) {
                    XT_LOG_ERROR( "expected and received bound_session differ");
                    xt_log ("\t(ERROR) expected and recd bound_session differ");
                    xt_return_code = XT_FAILED;
                } else {
                    xt_log ("\texpected and received bound_session same");
                } /* endif */
            } /* endif */
        } /* endif */
    } /* endif */

    /* if the func has executed OK, save the bound_session returned */
    if (xt_return_code == XT_OK) {
        session_tbl[rv2] = bound_session_return;
        xt_log ("\tds_bind() returned OK\n");
    } /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */


    return (xt_return_code);
} /* end xt_ds_bind */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_compare()                 */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_compare (void)
{
	static const char function_name[] = "xt_ds_compare";
    OM_sint parm1, parm2, parm3, parm4;
    OM_sint rv1, rv2;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;
    OM_private_object   result_return;
    OM_sint     invokeid_return;
    OM_return_code  om_return_code;
    OM_sint table_type;
    OM_object       name;
    OM_object       ava;
    OM_private_object   session;
    OM_private_object   context;
    OM_object       r_context;

    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm4, parm, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
        XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsession = %s", parm);


    /* read context parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &r_context) != XT_OK) {
        XT_LOG_ERROR( "Error reading context parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_CONTEXT)  && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "context parameter not found in context_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    if (table_type == XT_CONTEXT) {
        if (parm1 == 0)         /* default context         */

            context = DS_DEFAULT_CONTEXT;
        else if /* need a private object   */
        (om_return_code = (om_put (r_context, OM_REPLACE_ALL, r_context, (OM_type_list) NULL, ((OM_value_position) NULL),
                                                                         ((OM_value_position) NULL))) != OM_SUCCESS)    {
            XT_LOG_ERROR( "Error calling om_put for context");
            return(XT_NOT_CALLED);
        } /* endif */
    } else {
        context = r_context;
    } /* endif */

    /* read name parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm2, parm, &is_void, &name) != XT_OK) {
        XT_LOG_ERROR( "Error reading name parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_NAME) && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "name parameter not found in name_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tname = %s", parm);


    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&name, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* read ava parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm3, parm, &is_void, &ava) != XT_OK) {
        XT_LOG_ERROR( "Error reading ava parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_AVA)  && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "ava parameter not found in ava_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tava = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&ava, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */
	
    /* if session NULL - bind was not successful - don't call ds_compare */
    if (session == NULL) {
        xt_log ("\tds_compare() not called - NULL session parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    /* call the i/f function, ds_compare(), with the parameters read */
    xt_log ("\tcall ds_compare()");

    status_return = ds_compare ( session, context, name, ava, &result_return, &invokeid_return );

    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading status returned");
        return XT_ERROR ;
    } /* endif */

    /* compare expected and received status_return value */
    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_COMPAR) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
		} /* endif */
    } /* endif */

    if (status_return == DS_SUCCESS) {

        /* read expected compare_result value from the testcase file */
        /* and compare with received compare_result */
        if (xt_read_parm (XT_RETURN_VAL, XT_COMPARE_RESULT, &rv2, parm, &is_void) != XT_OK) {
            XT_LOG_ERROR( "Error reading result_return");
            return(XT_ERROR);
        } /* endif */

        if (!is_void) {
            if (xt_cmp_compare_result (result_return, compare_result_tbl[rv2]) != XT_OK) {
                /* test has failed, must write to log file */
                XT_LOG_ERROR( "expected and received result_return differ");
                xt_log("\t(ERROR) expected & received compare-result differ");
                return(XT_FAILED);
            } else {
                xt_log("\texpected & received compare-result same");
				if ( om_delete (result_return) != OM_SUCCESS) {
					xt_log_both1(function_name, "delete of result_return failed");
					XT_LOG_ERROR("\t(ERROR) delete of result_return failed");
					return(XT_FAILED);
				} /* endif */
            } /* endif */
        } /* endif */
    } /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_compare() returned OK\n");
    return (XT_OK);

} /* end xt_ds_compare */



/***********************************************************************/
/*                                                                      */
/*          xt_dsX_extract_attr_values()                    		*/
/*                                                                      */
/*  takes as parameters in testcase: 									*/
/* 		attribute_type 													*/
/*      expected OM return code											*/
/* 		expected number_of_values										*/
/* 		expected values													*/
/*                                                                      */
/* takes as object to extract from                                      */
/* 	private_object_tbl[PRIVATE_RESULT_INDEX]                            */
/*                                                                      */
/***********************************************************************/

OM_sint xt_dsX_extract_attr_values (void)
{
	static const char function_name[] = "xt_dsX_extract_attr_values";
    OM_sint parm2,parm3,parm4;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    OM_return_code  om_return_code;
	OM_public_object     values;
	OM_value_position    total_number;
	char *pchar;
    OM_sint xt_return_code = XT_OK;
    OM_type type_list[] = {DS_ENTRY,OM_NO_MORE_TYPES};
	OM_object attr_list, gotten_object;
	OM_value_position attr_count;
    OM_sint table_type;
	OM_object extracted_object;


    /* read attribute_type parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_ATTR_TYPE, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading attribute type parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tattribute_type = %s", parm);



	gotten_object = private_object_tbl[PRIVATE_RESULT_INDEX];

	/* get DS_C_ENTRY_INFO out of DS_C_READ_RESULT */
	om_return_code = om_get(
			gotten_object,
			OM_EXCLUDE_ALL_BUT_THESE_TYPES + OM_EXCLUDE_SUBOBJECTS,
			type_list, OM_FALSE, OM_ALL_VALUES, OM_ALL_VALUES,
									&attr_list, &attr_count);
	if( om_return_code != OM_SUCCESS || attr_count == 0) {
		xt_return_code = XT_FAILED;
		goto end;
	} /* endif */

    /* call the interface function, dsX_extract_attr_values(),
	with the parameters read */

    xt_log ("\tcall dsX_extract_attr_values()");

    om_return_code = dsX_extract_attr_values (
	/* OM_private_object object  IN  - The source object          */
		attr_list->value.object.object,
	/* OM_object_identifier  attribute_type, IN  - Attribute to be extracted  */
		*attr_type_tbl[parm2],
	/* OM_boolean local_strings */
		OM_TRUE,
	/* OM_public_object     *values, OUT - Extracted Attribute Values */
		&values,
	/* OM_value_position    *total_number)   OUT - Number of extracted values */
		&total_number);

    /* read expected om_return_code value from the testcase file    */
    /* and compare with received om_return_code                 */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading OM_return_code");
        xt_log_both1 ("xt_dsX_extract_attr_values: read %s", parm);
        return(XT_ERROR);
    } /* endif */


    if (!strcmp(parm, xt_om_error_to_string(om_return_code))) {
        /* expected and received same */
		xt_log1 ("\texpected and received DS_status from %s same", function_name);
        if (!strcmp (parm, "OM_SUCCESS")) {
            /* OM_SUCCESS expected and received */

			/* read expected number of extracted values from the testcase file */
			if (xt_read_int (&parm3) != XT_OK)   {
				XT_LOG_ERROR( "Error reading expected_number parameter");
				return(XT_FAILED);
			} /* endif */
			xt_log1 ("\texpected_number = %s", xt_itoa(parm3));

			/* read extracted object parameter from the testcase file */
			if (xt_read_object (XT_PARAMETER, &table_type, &parm4, parm, &is_void, &extracted_object) != XT_OK) {
				XT_LOG_ERROR( "Error reading extracted object parameter");
				return(XT_NOT_CALLED);
			} /* endif */
			if ((table_type != XT_AVA)  && (table_type != XT_PRIVATE_OBJECT)) {
				XT_LOG_ERROR( "extracted object parameter not found in ava_tbl or private_object table");
				return(XT_FAILED);
			} /* endif */
			xt_log1 ("\textracted object = %s", parm);

			/* check whether number of values as expected */
			if (parm3 ==  total_number) {
				xt_log ("\texpected and received number of values from dsX_extract_attr_values same");
				/* check whether the values were correct */
#if 0 /* commented out CC 17.1.94 */
				if (xt_cmp_object (*values, extracted_object) == XT_OK) {
					xt_log ("\texpected and received values from dsX_extract_attr_values same");
				} else {
					/* test has failed, must write to log file */
					XT_LOG_ERROR( "expected and received extracted objects differ");
					return(XT_FAILED);
				} /* endif */
#endif
			} else {
				XT_LOG_ERROR( "expected_number was not received number");
				xt_log_both1("dsX_extract_attr_values expected %s", xt_itoa(parm3));
				xt_log_both1("dsX_extract_attr_values returned %s", xt_itoa(total_number));
				return(XT_FAILED);
			} /* endif */

        } /* endif */
        xt_return_code = XT_OK;
    } else {
        xt_log_both ("\t(ERROR) expected and received OM_return_code from dsX_extract_attr_values differ");
        xt_log_both1("dsX_extract_attr_values expected %s", parm);
        xt_log_both1("dsX_extract_attr_values returned %s", pchar = xt_om_error_to_string(om_return_code));
        free(pchar);
        xt_return_code = XT_FAILED;
    } /* endif */


	if( xt_return_code == XT_OK && total_number != 0) {
		om_return_code = om_delete(values);
		if( om_return_code != OM_SUCCESS) {
			xt_log_both ("\t(ERROR) bad OM_return_code from om_delete in dsX_extract_attr_values");

			xt_log_both1("om_delete returned %s", pchar = xt_om_error_to_string(om_return_code));
			free(pchar);
			xt_return_code = XT_FAILED;
		} /* endif */
	} /* endif */

end:
	return (xt_return_code);

} /* end xt_dsX_extract_attr_values */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_initialize()              */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_initialize (void)
{
	static const char function_name[] = "xt_ds_initialize";
    OM_sint rv1;
    OM_workspace local_workspace;
    OM_sint is_void;
    char    parm[64];


    /* call the interface function, ds_initialize(), with the paramet
        ers read */

    xt_log ("\tcall ds_initialize ()");

    if ((local_workspace = ds_initialize()) == (OM_workspace) NULL) {
        xt_log ("\tds_initialize returned NULL");
    } else {
        xt_log ("\tds_initialize did not return NULL");
    } /* endif */


    /* compare expected and received results and return values */

    if (xt_read_parm (XT_RETURN_VAL, XT_WORKSPACE, &rv1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading DS_status");
        return(XT_ERROR);
    } /* endif */

    if (local_workspace == (OM_workspace) NULL) {
        if (is_void) {
            XT_LOG_ERROR( "ds_initialize returned NULL");
            xt_log ("\t(ERROR) ds_initialize returned NULL");
            return(XT_FAILED);
        } else {                        /*  not "void" parm   */
            if (strcmp (parm, "NULL")) {        /*  parm != "NULL"    */
                XT_LOG_ERROR("NULL not expected");
                xt_log ("\t(ERROR) ds_initialize returned NULL, NULL not expected");
                return(XT_FAILED);
            } else {
                xt_log ("\tds_initialize returned NULL, NULL expected");
            } /* endif */
        } /* endif */
    } else if (!is_void) {
        workspace_tbl[rv1] = local_workspace;   /* save workspace returned */
    } /* endif */

    xt_log ("\tds_initialize() returned OK\n");
    return (XT_OK);

} /* end xt_ds_initialize */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_list()                    */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_list (void)
{
	static const char function_name[] = "xt_ds_list";
    OM_sint parm1, parm2, parm3;
    OM_sint rv1, rv2;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;
    OM_private_object   result_return;
    OM_sint     invokeid_return;
    OM_return_code  om_return_code;
    OM_sint table_type;
    OM_object       name;
    OM_private_object   session;
    OM_private_object   context;
    OM_object       r_context;


    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm3, parm, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
        XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsession = %s", parm);


    /* read context parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &r_context) != XT_OK) {
        XT_LOG_ERROR( "Error reading context parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_CONTEXT)  && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "context parameter not found in context_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    if (table_type == XT_CONTEXT) {
        if (parm1 == 0)         /* default context         */

            context = DS_DEFAULT_CONTEXT;
        else if /* need a private object   */
        (om_return_code = (om_put (r_context, OM_REPLACE_ALL, r_context, (OM_type_list) NULL, ((OM_value_position) NULL),
                                                                         ((OM_value_position) NULL))) != OM_SUCCESS)    {
            XT_LOG_ERROR( "Error calling om_put for context");
            return(XT_NOT_CALLED);
        } /* endif */
    } else {
        context = r_context;
    } /* endif */

    /* read name parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm2, parm, &is_void, &name) != XT_OK) {
        XT_LOG_ERROR( "Error reading name parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_NAME) && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "name parameter not found in name_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tname = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&name, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */
	
    /* if session NULL - bind was not successful - don't call ds_list
         */
    if (session == NULL) {
        xt_log ("\tds_list() not called - NULL session parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    /* call the interface function, ds_list(), with the parameters re
        ad */
    xt_log ("\tcall ds_list()");

    status_return = ds_list ( session, context, name, &result_return, &invokeid_return );

    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR( "Error reading status returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_LIST) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			/* mac - causing problems
										xt_log ("\texpected and r
				eceived DS_status from ds_bind same");
							 */
		} /* endif */
    } /* endif */


    if (status_return == DS_SUCCESS) {

        /* read expected list_result value from the testcase file */
        /* and compare with received list_result */
        if (xt_read_parm (XT_RETURN_VAL, XT_LIST_RESULT, &rv2, parm, &is_void) != XT_OK) {
            XT_LOG_ERROR( "Error reading result_return");
            return(XT_ERROR);
        } /* endif */

        if (!is_void) {
            if (xt_cmp_list_result (result_return, list_result_tbl[rv2]) != XT_OK) {
                /* test has failed, must write to log file */
                XT_LOG_ERROR( "expected and received result_return differ");
                xt_log("\t(ERROR) expected & received list-result differ");
                return(XT_FAILED);
            } else {
                xt_log("\texpected & received list-result same");
				if ( om_delete (result_return) != OM_SUCCESS) {
					xt_log_both1(function_name, "delete of result_return failed");
					XT_LOG_ERROR("\t(ERROR) delete of result_return failed");
					return(XT_FAILED);
				} /* endif */
            } /* endif */
        } /* endif */
    } /* endif */


	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_list() returned OK\n");
    return (XT_OK);
} /* end xt_ds_list */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_modify_entry()                */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_modify_entry (void)
{
	static const char function_name[] = "xt_ds_modify_entry";
    OM_sint parm1, parm2, parm3, parm4;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;
    OM_sint     invokeid_return;
    OM_return_code  om_return_code;
    OM_sint table_type;
    OM_object       name;
    OM_object       changes;
    OM_private_object   session;
    OM_private_object   context;
    OM_object       r_context;


    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm4, parm, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
        XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsession = %s", parm);

    /* read context parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &r_context) != XT_OK) {
        XT_LOG_ERROR( "Error reading context parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_CONTEXT)  && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "context parameter not found in context_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    if (table_type == XT_CONTEXT) {
        if (parm1 == 0)         /* default context         */

            context = DS_DEFAULT_CONTEXT;
        else if /* need a private object   */
        (om_return_code = (om_put (r_context, OM_REPLACE_ALL, r_context, (OM_type_list) NULL, ((OM_value_position) NULL),
                                                                         ((OM_value_position) NULL))) != OM_SUCCESS)    {
            XT_LOG_ERROR( "Error calling om_put for context");
            return(XT_NOT_CALLED);
        } /* endif */
    } else {
        context = r_context;
    } /* endif */


    /* read name parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm2, parm, &is_void, &name) != XT_OK) {
        XT_LOG_ERROR( "Error reading name parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_NAME) && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "name parameter not found in name_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tname = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&name, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */
	
    /* read changes parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm3, parm, &is_void, &changes) != XT_OK) {
        XT_LOG_ERROR( "Error reading changes parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_CHANGES)  && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "changes parameter not found in entry_modification_list_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tchanges = %s", parm);

    /* if session NULL - bind was not successful - don't call ds_modi
        fy_entry */
    if (session == NULL) {
        xt_log ("\tds_modify_entry() not called - NULL session parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    /* call the i/f function, ds_modify_entry(), with parameters read
         */
    xt_log ("\tcall ds_modify_entry()");

    status_return = ds_modify_entry ( session, context, name, changes, &invokeid_return );


    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR( "Error reading DS_status");
        return(XT_ERROR);
    } /* endif */

    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_MODENT) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
		} /* endif */
    } /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_modify_entry() returned OK\n");
    return (XT_OK);

} /* end xt_ds_modify_entry */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_modify_rdn()              */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_modify_rdn (void)
{
	static const char function_name[] = "xt_ds_modify_rdn";
    OM_sint parm1, parm2, parm3, parm4, parm5;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;
    OM_sint     invokeid_return;
    OM_return_code  om_return_code;
    OM_sint table_type;
    OM_object       name;
    OM_object       new_rdn;
    OM_private_object   session;
    OM_private_object   context;
    OM_object       r_context;


    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm5, parm, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
        XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsession = %s", parm);

    /* read context parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &r_context) != XT_OK) {
        XT_LOG_ERROR( "Error reading context parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_CONTEXT)  && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "context parameter not found in context_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    if (table_type == XT_CONTEXT) {
        if (parm1 == 0)         /* default context         */

            context = DS_DEFAULT_CONTEXT;
        else if /* need a private object   */
        (om_return_code = (om_put (r_context, OM_REPLACE_ALL, r_context, (OM_type_list) NULL, ((OM_value_position) NULL),
                                                                         ((OM_value_position) NULL))) != OM_SUCCESS)    {
            XT_LOG_ERROR( "Error calling om_put for context");
            return(XT_NOT_CALLED);
        } /* endif */
    } else {
        context = r_context;
    } /* endif */

    /* read name parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm2, parm, &is_void, &name) != XT_OK) {
        XT_LOG_ERROR( "Error reading name parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_NAME) && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "name parameter not found in name_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tname = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&name, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */
	

    /* read new_rdn parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm3, parm, &is_void, &new_rdn) != XT_OK) {
        XT_LOG_ERROR( "Error reading new_rdn parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_RDN)  && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "new_rdn parm not found in name_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tnew_rdn = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&new_rdn, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */
	

    if (xt_read_parm (XT_PARAMETER, XT_BOOLEAN, &parm4, parm, &is_void)   != XT_OK) {
        XT_LOG_ERROR( "Error reading delete_old_rdn parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tdelete_old_rdn = %s", parm);

    /* if session NULL - bind not successful - don't call ds_modify_r
        dn */
    if (session == NULL) {
        xt_log ("\tds_modify_rdn() not called - NULL session parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    /* call the i/f function, ds_modify_rdn(), with the parameters re
        ad */
    xt_log ("\tcall ds_modify_rdn()");

    status_return = ds_modify_rdn ( session, context, name, new_rdn, boolean_tbl[parm4], &invokeid_return );


    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR( "Error reading DS_status");
        return(XT_ERROR);
    } /* endif */

    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_MODRDN) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
		} /* endif */
    } /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_modify_rdn() returned OK\n");
    return (XT_OK);

} /* end xt_ds_modify_rdn */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_read()                    */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_read (void)
{
	static const char function_name[] = "xt_ds_read";
    OM_sint parm2, parm3, parm4, parm5;
    OM_sint rv1, rv2;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;
    OM_sint     invokeid_return;
    OM_return_code  om_return_code;
    OM_sint table_type;
    OM_object       name;
    OM_object       selection;
    OM_private_object   session;
    OM_private_object   context;
    OM_object       r_context;


    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm5, parm, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
        XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsession = %s", parm);

    /* read context parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm2, parm, &is_void, &r_context) != XT_OK) {
        XT_LOG_ERROR( "Error reading context parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_CONTEXT)  && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "context parameter not found in context_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    if (table_type == XT_CONTEXT) {
        if (parm2 == 0)         /* default context         */

            context = DS_DEFAULT_CONTEXT;
        else if /* need a private object   */
        (om_return_code = (om_put (r_context, OM_REPLACE_ALL, r_context, (OM_type_list) NULL, ((OM_value_position) NULL),
                                                                         ((OM_value_position) NULL))) != OM_SUCCESS)    {
            XT_LOG_ERROR( "Error calling om_put for context");
            return(XT_NOT_CALLED);
        } /* endif */
    } else {
        context = r_context;
    } /* endif */

    /* read name parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm3, parm, &is_void, &name) != XT_OK) {
        XT_LOG_ERROR( "Error reading name parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_NAME) && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "name parameter not found in name_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tname = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&name, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */
	

    /* read  entry_info_selection parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm4, parm, &is_void, &selection) != XT_OK) {
        XT_LOG_ERROR( "Error reading entry_info_selection parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_SELECTION) && (table_type != XT_PRIVATE_OBJECT))  {
        XT_LOG_ERROR( "selection parameter not found in entry_info_selection_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tselection = %s", parm);

    /* if session NULL - bind was not successful - don't call ds_read
         */
    if (session == NULL) {
        xt_log ("\tds_read() not called - NULL session parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    /* call the interface function, ds_read(), with the parameters read */
    xt_log ("\tcall ds_read()");

    status_return = ds_read ( session, context, name, selection, &private_object_tbl[PRIVATE_RESULT_INDEX], &invokeid_return );

    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR( "Error reading DS_status");
        return(XT_ERROR);
    } /* endif */

    if (!is_void) {

		if (xt_cmp_status (status_return, status_tbl[rv1], XT_DSREAD) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
		} /* endif */
    } /* endif */

    if (status_return == DS_SUCCESS) {
        /* read expected read_result value from the testcase file */
        /* and compare with received read_result */
        if (xt_read_parm (XT_RETURN_VAL, XT_READ_RESULT, &rv2, parm, &is_void) == XT_ERROR)  {
            XT_LOG_ERROR( "Error reading private_result");
            return(XT_ERROR);
        } /* endif */

        if (!is_void) {
            if (xt_cmp_read_result (private_object_tbl[PRIVATE_RESULT_INDEX], read_result_tbl[rv2]) != XT_OK) {
                /* test has failed, must write to log file */
                XT_LOG_ERROR( "expected and received private_result differ");
                xt_log("\t(ERROR) expected & received read-result differ");
                return(XT_FAILED);
            } else {
                xt_log("\texpected & received read-result same");
            } /* endif */
        } /* endif */
    } /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_read() returned OK\n");
    return (XT_OK);

} /* end xt_ds_read */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_receive_result()              */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_receive_result (void)
{
	static const char function_name[] = "xt_ds_receive_result";
    OM_sint parm1;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;
    OM_uint     completion_flag_return;
    DS_status       operation_status_return;
    OM_private_object   result_return;
    OM_sint     invokeid_return;
    OM_sint table_type;
    OM_private_object   session;


    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
        XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsession = %s", parm);

    /* if session NULL - bind not successful - don't call ds_receive_
        result */
    if (session == NULL) {
        xt_log ("\tds_receive_result() not called - NULL session parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    /* call the i/f function, ds_receive_result(), with the parms rea
        d */
    xt_log ("\tcall ds_receive_result()");

    status_return = ds_receive_result ( session, &completion_flag_return, &operation_status_return, &result_return,
                        &invokeid_return    );


    /* compare expected and received results and return values */
    /* only have to check DS_status, because ds_receive_result is not */
    /* implemented fully, doesn't support asynchronous operation */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR( "Error reading DS_status");
        return(XT_ERROR);
    } /* endif */

    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_RECRES) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
#if 0 /* restore when receive_result really returns something */
			if ( om_delete (result_return) != OM_SUCCESS) {
				xt_log_both1(function_name, "delete of result_return failed");
				XT_LOG_ERROR("\t(ERROR) delete of result_return failed");
				return(XT_FAILED);
			} /* endif */
#endif /* 0 */
		} /* endif */
    } /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_receive_result() returned OK\n");
    return (XT_OK);

} /* end xt_ds_receive_result */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_remove_entry()                */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_remove_entry (void)
{
	static const char function_name[] = "xt_ds_remove_entry";
    OM_sint parm1, parm2, parm3;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;
    OM_sint     invokeid_return;
    OM_return_code  om_return_code;
    OM_sint table_type;
    OM_object       name;
    OM_private_object   session;
    OM_private_object   context;
    OM_object       r_context;


    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm3, parm, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
        XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsession = %s", parm);

    /* read context parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &r_context) != XT_OK) {
        XT_LOG_ERROR( "Error reading context parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_CONTEXT)  && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "context parameter not found in context_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    if (table_type == XT_CONTEXT) {
        if (parm1 == 0)         /* default context         */

            context = DS_DEFAULT_CONTEXT;
        else if /* need a private object   */
        (om_return_code = (om_put (r_context, OM_REPLACE_ALL, r_context, (OM_type_list) NULL, ((OM_value_position) NULL),
                                                                         ((OM_value_position) NULL))) != OM_SUCCESS)    {
            XT_LOG_ERROR( "Error calling om_put for context");
            return(XT_NOT_CALLED);
        } /* endif */
    } else {
        context = r_context;
    } /* endif */

    /* read name parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm2, parm, &is_void, &name) != XT_OK) {
        XT_LOG_ERROR( "Error reading name parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_NAME) && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "name parameter not found in name_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tname = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&name, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */
	

    /* if session NULL- bind not successful - don't call ds_remove_en
        rty */
    if (session == NULL) {
        xt_log ("\tds_remove_entry() not called - NULL session parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    /* call the i\f function, ds_remove_entry(), with parameters read
         */
    xt_log ("\tcall ds_remove_entry()");

    status_return = ds_remove_entry ( session, context, name, &invokeid_return );


    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading DS_status");
        return(XT_ERROR);
    } /* endif */

    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_RMENT) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
		} /* endif */
    } /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_remove_entry() returned OK\n");
    return (XT_OK);

} /* end xt_ds_remove_entry */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_search()                  */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_search (void)
{
	static const char function_name[] = "xt_ds_search";
    OM_sint parm1, parm2, parm3, parm4, parm5, parm6, parm7;
    OM_sint rv1, rv2;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;
    OM_private_object   result_return;
    OM_sint     invokeid_return;
    OM_return_code  om_return_code;
    OM_sint table_type;
    OM_object       name;
    OM_object       filter;
    OM_object       selection;
    OM_private_object   session;
    OM_private_object   context;
    OM_object       r_context;


    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm7, parm, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
        XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsession = %s", parm);

    /* read context parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &r_context) != XT_OK) {
        XT_LOG_ERROR( "Error reading context parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_CONTEXT)  && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "context parameter not found in context_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    if (table_type == XT_CONTEXT) {
        if (parm1 == 0)         /* default context         */

            context = DS_DEFAULT_CONTEXT;
        else if /* need a private object   */
        (om_return_code = (om_put (r_context, OM_REPLACE_ALL, r_context, (OM_type_list) NULL, ((OM_value_position) NULL),
                                                                         ((OM_value_position) NULL))) != OM_SUCCESS)    {
            XT_LOG_ERROR( "Error calling om_put for context");
            return(XT_NOT_CALLED);
        } /* endif */
    } else {
        context = r_context;
    } /* endif */

    /* read name parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm2, parm, &is_void, &name) != XT_OK) {
        XT_LOG_ERROR( "Error reading name parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_NAME) && (table_type != XT_PRIVATE_OBJECT)) {
        XT_LOG_ERROR( "name parameter not found in name_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tname = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&name, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */
	

    /* read subset parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_SUBSET, &parm3, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading subset parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsubset = %s", parm);

    /* read filter parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm4, parm, &is_void, &filter) != XT_OK)  {
        XT_LOG_ERROR( "Error reading filter parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_FILTER) && (table_type != XT_PRIVATE_OBJECT))  {
        XT_LOG_ERROR( "filter parm not found in filter_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tfilter = %s", parm);

    /* read search_aliases parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_BOOLEAN, &parm5, parm, &is_void)   != XT_OK) {
        XT_LOG_ERROR( "Error reading search_aliases parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsearch_aliases = %s", parm);

    /* read selection parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm6, parm, &is_void, &selection) != XT_OK) {
        XT_LOG_ERROR( "Error reading entry_info_selection parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_SELECTION) && (table_type != XT_PRIVATE_OBJECT))  {
        XT_LOG_ERROR( "search parameter not found in entry_info_selection_tbl or private_object table");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tselection = %s", parm);


    /* if session NULL - bind was not successful - don't call ds_sear
        ch */
    if (session == NULL) {
        xt_log ("\tds_search() not called - NULL session parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    /* call the interface function, ds_search(), with the parameters
        read */
    xt_log ("\tcall ds_search()");

    status_return = ds_search ( session, context, name, subset_tbl[parm3], filter, boolean_tbl[parm5],
                 selection, &result_return, &invokeid_return );

    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading DS_status");
        return(XT_ERROR);
    } /* endif */

    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_SEARCH) != XT_OK) {
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
/* mac - causing problem!
			xt_log ("\texpected and received DS_status from ds_search same");

*/
		} /* endif */

    } /* endif */

    if (status_return == DS_SUCCESS) {
        /* read expected search_result value from the testcase file */
        /* and compare with received search_result */
        if (xt_read_parm (XT_RETURN_VAL, XT_SEARCH_RESULT, &rv2, parm, &is_void) != XT_OK)  {
            XT_LOG_ERROR( "Error reading result_return");
            return(XT_ERROR);
        } /* endif */

        if (!is_void) {
            if (xt_cmp_search_result (result_return, search_result_tbl[rv2]) != XT_OK) {
                /* test has failed, must write to log file */
                XT_LOG_ERROR( "expected and received result_return differ");
                xt_log("\t(ERROR) expected & received search-result differ");
                return(XT_FAILED);
            } else {
                xt_log("\texpected & received search-result same");
				if ( om_delete (result_return) != OM_SUCCESS) {
					xt_log_both1(function_name, "delete of result_return failed");
					XT_LOG_ERROR("\t(ERROR) delete of result_return failed");
					return(XT_FAILED);
				} /* endif */
            } /* endif */
        } /* endif */
    } /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_search() returned OK\n");
    return (XT_OK);

} /* end xt_ds_search */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_shutdown()                */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_shutdown (void)
{
	static const char function_name[] = "xt_ds_shutdown";
    OM_sint parm1;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;

    /* read workspace parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_WORKSPACE, &parm1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading workspace parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tworkspace = %s", parm);

    /* call the interface function, ds_shutdown(), with the parameter
        s read */

    xt_log ("\tcall ds_shutdown()");

    status_return = ds_shutdown ( workspace_tbl[parm1] );

    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR( "Error reading DS_status");
        return(XT_ERROR);
    } /* endif */

    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_SHUTDN) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
		} /* endif */
    } /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_shutdown() returned OK\n");
    return (XT_OK);

} /* end xt_ds_shutdown */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_unbind()                  */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_unbind (void)
{
	static const char function_name[] = "xt_ds_unbind";
    OM_sint parm1;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;
    OM_sint table_type;
    OM_private_object   session;


    /* read session parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &session) != XT_OK) {
        XT_LOG_ERROR( "Error reading session parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    if ((table_type != XT_PRIVATE_OBJECT) && (table_type != XT_SESSION)) {
        XT_LOG_ERROR( "session parm not found in session_tbl or private_object_tbl");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsession = %s", parm);

    /* if session NULL - bind was not successful - don't call ds_unbi
        nd */
    if (session == NULL) {
        xt_log ("\tds_unbind() not called - NULL session parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    /* call the interface function, ds_unbind(), with the parameters
        read */
    xt_log ("\tcall ds_unbind()");

    status_return = ds_unbind ( session );


    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR( "Error reading DS_status");
        return(XT_ERROR);
    } /* endif */

    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_UNBIND) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
		} /* endif */
    } /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_unbind() returned OK\n");
    return (XT_OK);

} /* end xt_ds_unbind */


/***********************************************************************/
/*                                                                      */
/*          xt_ds_version()                 */
/*                                                                      */
/***********************************************************************/

OM_sint xt_ds_version (void)
{
	static const char function_name[] = "xt_ds_version";
    OM_sint parm1;
    OM_sint parm2;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    DS_status       status_return;


    /* read feature_list parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_FEATURE_LIST, &parm1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading feature_list parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tfeature_list = %s", parm);

    /* read workspace parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_WORKSPACE, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR( "Error reading workspace parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tworkspace = %s", parm);

    /* call the interface function, ds_version(), with the parameters
         read */

    xt_log ("\tcall ds_version()");

    status_return = ds_version (feature_list_tbl[parm1], workspace_tbl[parm2]);


    /* read expected status_return value from the testcase file */
    /* and compare with received status_return                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_STATUS, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR( "Error reading DS_status");
        return(XT_ERROR);
    } /* endif */

    if (!is_void) {
		if (xt_cmp_status (status_return, status_tbl[rv1], XT_VERSN) != XT_OK) {
			/* test has failed, must write to log file */
			XT_LOG_ERROR( "expected and received DS_status differ");
			xt_log1 ("\t(ERROR) expected and received DS_status from %s differ", function_name);
			return(XT_FAILED);
		} else {
			xt_log1 ("\texpected and received DS_status from %s same", function_name);
		} /* endif */
    } /* endif */

	if (status_return != DS_SUCCESS) {
		if ( om_delete (status_return) != OM_SUCCESS) {
			xt_log_both1(function_name, "delete of status_return failed");
			XT_LOG_ERROR("\t(ERROR) delete of status_return failed");
			return(XT_FAILED);
		} /* endif */
	} /* endif */

    xt_log ("\tds_version() returned OK\n");
    return (XT_OK);

} /* end xt_ds_version */
