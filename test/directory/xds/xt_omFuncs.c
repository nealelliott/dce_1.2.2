/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_omFuncs.c,v $
 * Revision 1.1.12.2  1996/02/18  00:24:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:12:01  marty]
 *
 * Revision 1.1.12.1  1995/12/08  22:13:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:59  root]
 * 
 * Revision 1.1.10.5  1994/07/06  15:09:37  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:26:47  marrek]
 * 
 * Revision 1.1.10.4  1994/06/21  14:28:48  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:47:50  marrek]
 * 
 * Revision 1.1.10.3  1994/05/10  15:33:14  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:22:51  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:43:08  marrek]
 * 
 * Revision 1.1.10.2  1994/03/23  16:06:42  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  18:13:26  keutel]
 * 
 * Revision 1.1.10.1  1994/02/22  19:15:58  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:30:01  marrek]
 * 
 * Revision 1.1.8.1  1993/10/14  19:28:47  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:27:16  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  12:54:29  keutel]
 * 
 * Revision 1.1.6.2  1993/08/12  12:27:10  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:27:13  marrek]
 * 
 * Revision 1.1.4.2  1993/01/05  20:34:26  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:48:41  htf]
 * 
 * Revision 1.1.2.3  1992/07/01  16:54:21  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:24:27  melman]
 * 
 * Revision 1.1.2.2  1992/06/11  20:01:10  melman
 * 	 gds drop
 * 	[1992/06/11  19:23:45  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xt_omFuncs.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 00:24:42 $";
#endif

/***************************************************************
*                                                              * 
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   * 
*                ALL RIGHTS RESERVED                           * 
*                                                              * 
***************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : xt_omFuncs.c                                        */
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
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*          Module identifikation                                     */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*            I N C L U D E S                                         */
/*                                                                    */
/**********************************************************************/

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
#include <omXext.h>	/* for omX_Extract() */
#include <xt_omFuncs.h>
#include <xt_rValue.h>



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
extern OM_object_identifier *rules_tbl[];
extern OM_return_code       return_code_tbl[];
extern OM_object            object_tbl[];
extern struct class_tbl_struct class_tbl[];
extern OM_type              type_tbl[];
extern OM_type_list         type_list_tbl[];
extern OM_type_list         nav_path_tbl[];
extern OM_exclusions        exclusions_tbl[];
extern OM_modification      modification_tbl[];
extern OM_syntax            syntax_tbl[];

extern BOOLEAN xt_do_to_string_and_back;

OM_workspace convenience_workspace;

char *OM_to_normal_string (OM_string string);

OM_IMPORT(DS_C_FILTER)
OM_IMPORT(DS_C_DS_DN)
OM_IMPORT(DS_C_DS_RDN)
OM_IMPORT(DS_C_AVA)
OM_IMPORT(DS_C_ATTRIBUTE)
OM_IMPORT(DS_C_DS_DN)
OM_IMPORT(DS_C_FACSIMILE_PHONE_NBR)
OM_IMPORT(DS_C_POSTAL_ADDRESS)
OM_IMPORT(DS_C_TELEX_NBR)
OM_IMPORT(DS_C_PRESENTATION_ADDRESS)
OM_IMPORT(DS_C_ATTRIBUTE)
OM_IMPORT(DSX_C_GDS_ACL)
OM_IMPORT(DSX_C_GDS_ACL_ITEM)
OM_IMPORT(MH_C_OR_ADDRESS)
OM_IMPORT(MH_C_OR_NAME)

/****************************************************************************/
/*                                                                          */
/*          xt_om_copy()                    */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_copy (void)
{
	static const char function_name[] = "xt_om_copy";
    OM_sint parm1, parm2;
    OM_sint rv1, rv2;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_private_object   copy_return;
    OM_private_object   original;
    OM_descriptor        * e_object;
    OM_descriptor        * r_object;
    OM_value_position   total_number;


    /* read original parameter from the testcase file */
    if (xt_read_private_object (XT_PARAMETER, XT_PRIVATE_OBJECT, &parm1, parm, &is_void, &original) != XT_OK) {
        XT_LOG_ERROR ( "Error reading original parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\toriginal = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&original, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* read workspace parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_WORKSPACE, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ( "Error reading workspace parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tworkspace = %s", parm);


    /* call the interface function, om_copy(), with the parameters read */
    xt_log ("\tcall om_copy()");

    return_code = om_copy (original, workspace_tbl[parm2], &copy_return );

    xt_log1 ("\tom_copy return code = %s", xt_om_error_to_string(return_code));

    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ( "Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK)  {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ( "expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_copy differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_copy same");
        } /* endif */
	} /* endif */

    if (return_code == OM_SUCCESS) {
        /* save copy_return in the private_object_tbl */
        if (xt_read_parm (XT_RETURN_VAL, XT_PRIVATE_OBJECT, &rv2, parm, &is_void) == XT_ERROR) {
            XT_LOG_ERROR ( "Error reading copy_return returned");
            return XT_ERROR ;
        } /* endif */
        if (xt_do_to_string_and_back) {
            OM_return_code orc = xt_to_string_and_back(&copy_return, parm);
			if (orc != OM_SUCCESS) {
				return(XT_FAILED);
			} /* endif */
        } /* endif */
        private_object_tbl[rv2] = copy_return;

        /* get public object of returned object and original */
        if ((om_get (original, OM_NO_EXCLUSIONS, ((OM_type_list) 0), OM_TRUE, ((OM_value_position) 0), ((OM_value_position) 0), &e_object, &total_number)) !=
            OM_SUCCESS)  {
            XT_LOG_ERROR ("om_get for copy_return failed");
            return XT_ERROR ;
        } /* endif */

        if (xt_do_to_string_and_back) {
            OM_return_code orc = xt_to_string_and_back(&e_object, "=e_object");
			if (orc != OM_SUCCESS) {
				return(XT_FAILED);
			} /* endif */
        } /* endif */

        if ((om_get (copy_return, OM_NO_EXCLUSIONS, ((OM_type_list) 0), OM_TRUE, ((OM_value_position) 0),
                          ((OM_value_position) 0), &r_object, &total_number)) != OM_SUCCESS)  {
            XT_LOG_ERROR ("om_get for copy_return failed");
            return XT_ERROR ;
        } /* endif */

        if (xt_do_to_string_and_back) {
            OM_return_code orc = xt_to_string_and_back(&r_object, "=r_object");
			if (orc != OM_SUCCESS) {
				return(XT_FAILED);
			} /* endif */
        } /* endif */

        /* compare the original and copy */
        if (xt_cmp_object (r_object, e_object) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received copy_return differ");
            xt_log ("\t(ERROR) expected and received copy_return from om_copy differ");
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received copy_return from om_copy same");
        } /* endif */
    } /* endif */

    xt_log ("\tom_copy() returned OK\n");
    return (XT_OK);

} /* end xt_om_copy */


/****************************************************************************/
/*                                                                          */
/*          xt_om_copy_value()              */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_copy_value (void)
{
	static const char function_name[] = "xt_om_copy_value";
    OM_sint parm1, parm2, parm3, parm5, parm6;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_object       source;
    OM_object       destination;


    /* read source parameter from the testcase file */
    if (xt_read_private_object (XT_PARAMETER, XT_PRIVATE_OBJECT, &parm1, parm, &is_void, &source) != XT_OK) {
        XT_LOG_ERROR ("Error reading source parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsource = %s", parm);


    /* read source_type parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_TYPE, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading source_type parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsource_type = %s", parm);


    /* read source_value_position parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading source_value_position parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES */
        parm3 = OM_ALL_VALUES;
    } else {
        parm3 = atoi(parm);
    } /* endif */
    xt_log2 ("\tsource_value_position = %s (integer value = %s)", parm, xt_itoa(parm3));


    /* read destination parameter from the testcase file */
    if (xt_read_private_object (XT_PARAMETER, XT_PRIVATE_OBJECT, &parm1, parm, &is_void, &destination) != XT_OK) {
        XT_LOG_ERROR ("Error reading destination parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tdestination = %s", parm);


    /* read destination_type parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_TYPE, &parm5, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading destination_type parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tdestination_type = %s", parm);


    /* read destination_value_position parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading destination_value_position parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES  */
        parm6 = OM_ALL_VALUES;
    } else {
        parm6 = atoi(parm);
    } /* endif */
    xt_log2 ("\tdestination_value_position = %s (integer value = %s)", parm, xt_itoa(parm6));


    /* call the interface function, om_copy_value(), with the parameters read */
    xt_log ("\tcall om_copy_value()");

    return_code = om_copy_value    ( source, type_tbl[parm2], parm3, destination, type_tbl[parm5], parm6);

    xt_log1 ("\tom_copy_value return code = %s", xt_om_error_to_string(return_code));

    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_copy_value differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_copy_value same");
        } /* endif */
	} /* endif */

    xt_log ("\tom_copy_value() returned OK\n");
    return (XT_OK);

} /* end xt_om_copy_value */


/****************************************************************************/
/*                                                                          */
/*          xt_om_create()                  */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_create (void)
{
	static const char function_name[] = "xt_om_create";
    OM_sint parm1, parm2, parm3;
    OM_sint rv1, rv2;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_private_object   object_return;


    /* read class parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_CLASS, &parm1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading class parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tclass = %s", parm);


    /* read initialize parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_BOOLEAN, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading initialize parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tinitialize = %s", parm);


    /* read workspace parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_WORKSPACE, &parm3, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading workspace parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tworkspace = %s", parm);


    /* call the interface function, om_create(), with the parameters read */
    xt_log ("\tcall om_create()");

    return_code = om_create ( *class_tbl[parm1].class_type, boolean_tbl[parm2], workspace_tbl[parm3], &object_return );


    xt_log1 ("\tom_create return code = %s", xt_om_error_to_string(return_code));

    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_create differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_create same");
        } /* endif */
	} /* endif */

    if (return_code == OM_SUCCESS) {
        if (xt_read_parm (XT_RETURN_VAL, XT_PRIVATE_OBJECT, &rv2, parm, &is_void) == XT_ERROR) {
            XT_LOG_ERROR ("Error reading object returned");
            return XT_ERROR ;
        } /* endif */

        if (!is_void) {
            if (!(object_return->syntax  & OM_S_PRIVATE)) {
                XT_LOG_ERROR ("object_return is not PRIVATE");
                return XT_ERROR ;
            } /* endif */
			if (xt_do_to_string_and_back) {
				OM_return_code orc = xt_to_string_and_back(&object_return, parm);
				if (orc != OM_SUCCESS) {
					return(XT_FAILED);
				} /* endif */
			} /* endif */
            private_object_tbl[rv2] = object_return;
        } /* endif */
    } /* endif */

    xt_log ("\tom_create() returned OK\n");
    return (XT_OK);

} /* end xt_om_create */


/****************************************************************************/
/*                                                                          */
/*          xt_om_decode()                  */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_decode (void)
{
	static const char function_name[] = "xt_om_decode";
    OM_sint parm1;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_private_object   encoding;
    OM_private_object   original;


    /* read source parameter from the testcase file */
    if (xt_read_private_object (XT_PARAMETER, XT_PRIVATE_OBJECT, &parm1, parm, &is_void, &encoding) != XT_OK) {
        XT_LOG_ERROR ( "Error reading encoding parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsource = %s", parm);


    /* call the interface function, om_decode(), with the parameters read */
    xt_log ("\tcall om_decode()");


    return_code = om_decode    ( encoding, &original);

    xt_log1 ("\tom_decode return code = %s", xt_om_error_to_string(return_code));

    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_decode differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_decode same");
        } /* endif */
	} /* endif */

    xt_log ("\tom_decode() returned OK\n");
    return (XT_OK);

} /* end xt_om_decode */


/****************************************************************************/
/*                                                                          */
/*          xt_om_delete()                  */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_delete (void)
{
	static const char function_name[] = "xt_om_delete";
    OM_sint parm1;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_object       r_object;
    OM_sint table_type;


    /* read subject parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &r_object) != XT_OK) {
        XT_LOG_ERROR ("Error reading object parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tobject = %s", parm);


    /* call the interface function, om_delete(), with the parameters read */
    xt_log ("\tcall om_delete()");

    return_code = om_delete    ( r_object );

    xt_log1 ("\tom_delete return code = %s", xt_om_error_to_string(return_code));

    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_delete differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_delete same");
        } /* endif */
	} /* endif */

    xt_log ("\tom_delete() returned OK\n");
    return (XT_OK);

} /* end xt_om_delete */


/****************************************************************************/
/*                                                                          */
/*          xt_om_encode()                  */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_encode (void)
{
	static const char function_name[] = "xt_om_encode";
    OM_sint parm1;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_private_object       encoding;
    OM_private_object       original;
    OM_object_identifier    rules;


    /* read original parameter from the testcase file */
    if (xt_read_private_object (XT_PARAMETER, XT_PRIVATE_OBJECT, &parm1, parm, &is_void, &original) != XT_OK) {
        XT_LOG_ERROR ("Error reading original parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\toriginal = %s", parm);

    /* read rules parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_RULES, &parm1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading rules parameter");
        return XT_NOT_CALLED ;
    } /* endif */
    xt_log1 ("\trules = %s", parm);
    rules = *rules_tbl[parm1];


    /* call the interface function, om_encode(), with the parameters read */
    xt_log ("\tcall om_encode()");

    return_code = om_encode    ( original, rules, &encoding);

    xt_log1 ("\tom_encode return code = %s", xt_om_error_to_string(return_code));


    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_encode differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_encode same");
        } /* endif */
	} /* endif */

    xt_log ("\tom_encode() returned OK\n");
    return (XT_OK);

} /* end xt_om_encode */


/****************************************************************************/
/*                                                                          */
/*          xt_om_get()                 */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_get (void)
{
	static const char function_name[] = "xt_om_get";
    OM_sint parm1, parm2, parm3, parm4, parm5, parm6;
    OM_sint rv1, rv2, rv4;
    char    rv3[64];
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_object   original;
    OM_value_position   total_number_return;
    OM_public_object    copy_return = 0;
    OM_public_object    r_object;
    OM_sint table_type;


    /* read original parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &original) != XT_OK) {
        XT_LOG_ERROR ("Error reading original parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\toriginal = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&original, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* read exclusions parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_EXCLUSIONS, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error exclusions source_type parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\texclusions = %s", parm);


    /* read included_types parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_TYPE_LIST, &parm3, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading included_types parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tincluded_types = %s", parm);


    /* read local_strings parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_BOOLEAN, &parm4, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading local_strings parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tlocal_strings = %s", parm);


    /* read initial_value parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading initial_value parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES  */
        parm5 = OM_ALL_VALUES;
    } else {
        parm5 = atoi(parm);
    } /* endif */
    xt_log2 ("\tinitial_value = %s (integer value = %s)", parm, xt_itoa(parm5));


    /* read limiting_value parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading limiting parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES  */
        parm6 = OM_ALL_VALUES;
    } else {
        parm6 = atoi(parm);
    } /* endif */
    xt_log2 ("\tlimiting_value = %s (integer value = %s)", parm, xt_itoa(parm6));


    /* call the interface function, om_get(), with the parameters read */
    xt_log ("\tcall om_get()");

    return_code = om_get ( original, exclusions_tbl[parm2],
	type_list_tbl[parm3], boolean_tbl[parm4], parm5, parm6,
	&copy_return, &total_number_return );


    xt_log1 ("\tom_get return code = %s", xt_om_error_to_string(return_code));

    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_get differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_get same");
        } /* endif */
	} /* endif */

    if (return_code == OM_SUCCESS) {
        if (xt_read_public_object (XT_RETURN_VAL, XT_PUBLIC_OBJECT, &rv2, parm, &is_void, &r_object) == XT_ERROR)  {
            XT_LOG_ERROR ("Error reading copy returned");
            return XT_ERROR ;
        } /* endif */

        /*
        r_object = copy_return;
		 */
        if (copy_return) {
			if (xt_do_to_string_and_back) {
				/* call xt_to_string_and_back only if the exclusion is not */
				/* OM_EXCLUDE_MULTIPLES, otherwise xt_to_string_and_back   */
				/* will fail.											   */
				if (exclusions_tbl[parm2] != OM_EXCLUDE_MULTIPLES) {
				    OM_return_code orc = xt_to_string_and_back(&copy_return, parm);
					if (orc != OM_SUCCESS) {
						return(XT_FAILED);
					} /* endif */
				} /* endif */
			} /* endif */
            public_object_tbl[rv2] = copy_return;
		} /* endif */

        if (copy_return && ((copy_return->syntax) & OM_S_PRIVATE)) {
            XT_LOG_ERROR ("copy_return is not PUBLIC");
            xt_log ("\t(ERROR) copy_return from om_get is not a PUBLIC object");
            return XT_ERROR ;
        } /* endif */


#if 0
        if (!is_void) {
            if (xt_cmp_object (copy_return, public_object_tbl[rv2]) != XT_OK)  {
                XT_LOG_ERROR ("expected and received copy_return differ");
                xt_log ("\t(ERROR) expected and received copy_return from om_get differ");
                return(XT_FAILED);
            } else {
                xt_log("\texpected and received copy_return from om_get same");
            } /* endif */
		} /* endif */
#endif /* 0 */

        xt_log1 ("\tom_get total_number returned = %s",
		xt_itoa(total_number_return));

        if (xt_read_string (rv3) != XT_OK) {
            XT_LOG_ERROR ("Error reading total_number_return parameter");
            return(XT_ERROR);
        } /* endif */

        if (strcmp(rv3, "void")) {      /* if expected total_number not void */
            rv4 = atoi(rv3);
            if (xt_cmp_integer (rv4, total_number_return) != XT_OK) {
                /* test has failed, must write to log file */
                XT_LOG_ERROR ("expected and received total_number_return differ");
                xt_log ("\t(ERROR) expected and received total_number_return from om_get differ");
				xt_log_both1("number expected was %s",xt_itoa(rv4));
				xt_log_both1("number returned was %s",
				xt_itoa(total_number_return));
                return(XT_FAILED);
            } else {
                xt_log ("\texpected and received total_number_return from om_get same");
            } /* endif */
        } /* endif */
    } /* endif */

    xt_log ("\tom_get() returned OK\n");
    return (XT_OK);

} /* end xt_om_get */


/****************************************************************************/
/*                                                                          */
/*          xt_om_instance()                */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_instance (void)
{
	static const char function_name[] = "xt_om_instance";
    OM_sint parm1, parm2;
    OM_sint rv1, rv2;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_boolean      instance_return;
    OM_object       r_object;
    OM_sint table_type;


    /* read subject parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &r_object) != XT_OK) {
        XT_LOG_ERROR ("Error reading subject parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsubject = %s", parm);

    /* read class parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_CLASS, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading class parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tclass = %s", parm);

    /* call the interface function, om_instance(), with the parameters read */
    xt_log ("\tcall om_instance()");

    return_code = om_instance (r_object, *class_tbl[parm2].class_type, &instance_return );

    xt_log1 ("\tom_instance return code = %s", xt_om_error_to_string(return_code));

    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_instance differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_instance same");
        } /* endif */
	} /* endif */

    if (return_code == OM_SUCCESS) {
        if (xt_read_parm (XT_RETURN_VAL, XT_BOOLEAN, &rv2, parm, &is_void) == XT_ERROR)  {
            XT_LOG_ERROR ("Error reading instance_return returned");
            return XT_ERROR ;
        } /* endif */

        xt_log1 ("\tom_instance instance returned = %s", xt_itoa(instance_return));

        if (!is_void) {
            if (xt_cmp_boolean (instance_return, boolean_tbl[rv2]) != XT_OK) {
                /* test has failed, must write to log file */
                XT_LOG_ERROR ("expected and received copy_return differ");
                xt_log ("\t(ERROR) expected and received instance_return from om_instance differ");
			xt_log_both1("expected boolean %s", xt_itoa(boolean_tbl[rv2]));
			xt_log_both1("returned boolean %s", xt_itoa(instance_return));
                return(XT_FAILED);
            } else {
                xt_log ("\texpected and received instance_return from om_instance same");
            } /* endif */
		} /* endif */
    } /* endif */

    xt_log ("\tom_instance() returned OK\n");
    return (XT_OK);

} /* end xt_om_instance */


/****************************************************************************/
/*                                                                          */
/*          xt_om_put()                 */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_put (void)
{
	static const char function_name[] = "xt_om_put";
    OM_sint parm1, parm2, parm3, parm4, parm5, parm6;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_object       r_object;
    OM_private_object   destination;
    OM_sint table_type;


    /* read destination parameter from the testcase file */
    if (xt_read_private_object (XT_PARAMETER, XT_PRIVATE_OBJECT, &parm1, parm, &is_void, &destination) != XT_OK) {
        XT_LOG_ERROR ("Error reading destination parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tdestination = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&destination, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* read modification parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_MODIFICATION, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading modification parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tmodification = %s", parm);


    /* read source parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm3, parm, &is_void, &r_object) != XT_OK) {
        XT_LOG_ERROR ("Error reading source parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsource = %s", parm);


    /* read included_types parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_TYPE_LIST, &parm4, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading included_types parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tincluded_types = %s", parm);


    /* read initial_value parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading initial_value parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES */
        parm5 = OM_ALL_VALUES;
    } else {
        parm5 = atoi(parm);
    } /* endif */
    xt_log2 ("\tinitial_value = %s (integer value = %s)", parm, xt_itoa(parm5));


    /* read limiting_value parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading limiting_value parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES  */
        parm6 = OM_ALL_VALUES;
    } else {
        parm6 = atoi(parm);
    } /* endif */
    xt_log2 ("\tlimiting_value = %s (integer value = %s)", parm, xt_itoa(parm6));


    /* call the interface function, om_put(), with the parameters read */
    xt_log ("\tcall om_put()");

    return_code = om_put ( destination, modification_tbl[parm2], r_object, type_list_tbl[parm4], parm5, parm6 );


    xt_log1 ("\tom_put return code = %s", xt_om_error_to_string(return_code));

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&destination, "=destination");
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_put differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_put same");
        } /* endif */
	} /* endif */

	/* destination must be written back, since it could have
		been changed by xt_to_string_and_back */
    if (xt_do_to_string_and_back) {
		private_object_tbl[parm1] = destination;
	} /* endif */

    xt_log ("\tom_put() returned OK\n");
    return (XT_OK);

} /* end xt_om_put */


/****************************************************************************/
/*                                                                          */
/*          xt_om_read()                    */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_read (void)
{
	static const char function_name[] = "xt_om_read";
    OM_sint parm1, parm2, parm3, parm4, parm5, parm6;
    OM_sint rv1, rv2, rv3;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_string_length    string_length;
    OM_private_object   subject;
    OM_string       elements;


    /* read subject parameter from the testcase file */
    if (xt_read_private_object (XT_PARAMETER, XT_PRIVATE_OBJECT, &parm1, parm, &is_void, &subject) != XT_OK) {
        XT_LOG_ERROR ("Error reading subject parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsubject = %s", parm);

    /* read type parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_TYPE, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading type parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\ttype = %s", parm);


    /* read value_position parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading value_position parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES  */
        parm3 = OM_ALL_VALUES;
    } else {
        parm3 = atoi(parm);
    } /* endif */
    xt_log2 ("\tvalue_position = %s (integer value = %s)", parm, xt_itoa(parm3));


    /* read local_string parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_BOOLEAN, &parm4, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading local_string parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tlocal_string = %s", parm);


    /* read string offset parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading string offset parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES  */
        parm5 = OM_ALL_VALUES;
    } else {
        parm5 = atoi(parm);
    } /* endif */

    string_length = parm5;
    xt_log2 ("\tstring offset = %s (integer value = %s)", parm, xt_itoa(parm5));


    /* read elements parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_STRING, &parm6, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading elements parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\telements = %s", parm);
    elements = string_tbl[parm6];

    /* read expected return_code value from the testcase file */
    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

	if(!is_void && return_code_tbl[rv1] == OM_SUCCESS) {
		if (xt_do_to_string_and_back) {
			OM_return_code orc = xt_to_string_and_back(&subject, parm);
			if (orc != OM_SUCCESS) {
				return(XT_FAILED);
			} /* endif */
		} /* endif */
	} /* endif */

    /* call the interface function, om_read(), with the parameters read */
    xt_log ("\tcall om_read()");

    return_code = om_read (subject, type_tbl[parm2], parm3, boolean_tbl[parm4], &string_length, &elements );

    xt_log1 ("\tom_read return code = %s", xt_om_error_to_string(return_code));

    /* compare expected return code with received return_code */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_read differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_read same");
        } /* endif */
	} /* endif */

    if (return_code == OM_SUCCESS) {
        xt_log1 ("\tstring length returned = %s", xt_itoa(string_length));

        /* read string_length parameter from the testcase file */
        if (xt_read_string (parm) != XT_OK)  {
            XT_LOG_ERROR ("Error reading string_length parameter");
            return(XT_ERROR);
        } /* endif */

        if (strcmp(parm, "void")) {
            if (!strcmp(parm, "OM_ALL_VALUES"))  {
                rv2 = OM_ALL_VALUES;
            } else {
                rv2 = atoi(parm);
            } /* endif */
            xt_log2 ("\tvalue_position = %s (integer value = %s)", parm, xt_itoa(rv2));

            if (xt_cmp_integer (rv2, string_length) != XT_OK) {
                /* test has failed, must write to log file */
                XT_LOG_ERROR ("expected and received string_length returned differ");
                xt_log ("\t(ERROR) expected and received string_length returned from om_read differ");
			xt_log_both1 ("\treturned string length = %s", xt_itoa(string_length));
			xt_log_both1 ("\texpected string length = %s", xt_itoa(rv2));
                return(XT_FAILED);
            } else {
                xt_log ("\texpected and received string_length returned from om_read same");
            } /* endif */
        } /* endif */

        xt_log1 ("\telements returned (string length) = %s", xt_itoa(elements.length));
        xt_log1 ("\telements returned (string )       = %s", elements.elements);

        /* read expected elements parameter from the testcase file */
        if (xt_read_parm (XT_RETURN_VAL, XT_STRING, &rv3, parm, &is_void) != XT_OK) {
            XT_LOG_ERROR ("Error reading expected elements parameter");
            return(XT_ERROR);
        } /* endif */

        if (strcmp(parm, "void")) {
            if (xt_cmp_string (string_tbl[rv3], elements) != XT_OK) {
                /* test has failed, must write to log file */
                XT_LOG_ERROR ("expected and received elements returned differ");
                xt_log ("\t(ERROR) expected and received elements returned from om_read differ");
				xt_log_both1("expected string %s", string_tbl[rv3].elements);
				xt_log_both1("returned string %s", elements.elements);
                return(XT_FAILED);
            } else {
                xt_log ("\texpected and received elements returned from om_read same");
            } /* endif */

        } /* endif */

        string_tbl[parm6] = elements;
    } /* endif */

    xt_log ("\tom_read() returned OK\n");
    return (XT_OK);

} /* end xt_om_read */


/****************************************************************************/
/*                                                                          */
/*          xt_om_remove()                  */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_remove (void)
{
	static const char function_name[] = "xt_om_remove";
    OM_sint parm1, parm2, parm3, parm4;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_private_object   subject;


    /* read subject parameter from the testcase file */
    if (xt_read_private_object (XT_PARAMETER, XT_PRIVATE_OBJECT, &parm1, parm, &is_void, &subject) != XT_OK) {
        XT_LOG_ERROR ("Error reading subject parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsubject = %s", parm);

    if (xt_do_to_string_and_back) {
        OM_return_code orc = xt_to_string_and_back(&subject, parm);
		if (orc != OM_SUCCESS) {
			return(XT_FAILED);
		} /* endif */
    } /* endif */

    /* read type parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_TYPE, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading type parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\ttype = %s", parm);



    /* read initial_value parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading initial_value parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES  */
        parm3 = OM_ALL_VALUES;
    } else {
        parm3 = atoi(parm);
    } /* endif */
    xt_log2 ("\tinitial_value = %s (integer value = %s)", parm, xt_itoa(parm3));


    /* read limiting_value parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading limiting_value parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES  */
        parm4 = OM_ALL_VALUES;
    } else {
        parm4 = atoi(parm);
    } /* endif */
    xt_log2 ("\tlimiting_value = %s (integer value = %s)", parm, xt_itoa(parm4));

    /* call the interface function, om_remove(), with the parameters read */
    xt_log ("\tcall om_remove()");

    return_code = om_remove    ( subject, type_tbl[parm2], parm3, parm4 );

    xt_log1 ("\tom_remove return code = %s", xt_om_error_to_string(return_code));

    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_remove differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_remove same");
        } /* endif */
    } /* endif */


	/* subject must be written back, since it could have
		been changed by xt_to_string_and_back */
    if (xt_do_to_string_and_back) {
		private_object_tbl[parm1] = subject;
	} /* endif */

    xt_log ("\tom_remove() returned OK\n");
    return (XT_OK);

} /* end xt_om_remove */


/****************************************************************************/
/*                                                                          */
/*          xt_om_write()                   */
/*                                                                          */
/****************************************************************************/

OM_sint xt_om_write (void)
{
	static const char function_name[] = "xt_om_write";
    OM_sint parm1, parm2, parm3, parm4, parm5, parm6;
    OM_sint rv1, rv2;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_string_length    string_length;
    OM_private_object   subject;


    /* read subject parameter from the testcase file */
    if (xt_read_private_object (XT_PARAMETER, XT_PRIVATE_OBJECT, &parm1, parm, &is_void, &subject) != XT_OK) {
        XT_LOG_ERROR ("Error reading subject parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsubject = %s", parm);

    /* read type parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_TYPE, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading type parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\ttype = %s", parm);

    /* read value_position parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading value_position parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES  */
        parm3 = OM_ALL_VALUES;
    } else {
        parm3 = atoi(parm);
    } /* endif */
    xt_log2 ("\tvalue_position = %s (integer value = %s)", parm, xt_itoa(parm3));

    /* read syntax parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_SYNTAX, &parm4, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading syntax parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsyntax = %s", parm);

    /* read string_length parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading string_length parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES  */
        parm5 = OM_ALL_VALUES;
    } else {
        parm5 = atoi(parm);
    } /* endif */
    xt_log2 ("\tstring_length = %s (integer value = %s)", parm, xt_itoa(parm5));
    string_length = parm5;

    /* read elements parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_STRING, &parm6, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading elements parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\telements = %s", parm);

    /* call the interface function, om_write(), with the parameters read */
    xt_log ("\tcall om_write()");

    return_code = om_write ( subject, type_tbl[parm2], parm3, syntax_tbl[parm4], &string_length, string_tbl[parm6] );

    xt_log1 ("\tom_write return code = %s", xt_om_error_to_string(return_code));

    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */
    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK) {
            /* test has failed, must write to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_write differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_write same");
        } /* endif */
	} /* endif */


    if (return_code == OM_SUCCESS) {
        /* read return string_length return parm from the testcase file */
        if (xt_read_string (parm) != XT_OK)  {
            XT_LOG_ERROR ("Error reading return string_length return parameter");
            return(XT_ERROR);
        } /* endif */

        if (strcmp(parm, "void")) {
            if (!strcmp(parm, "OM_ALL_VALUES"))  {
                rv2 = OM_ALL_VALUES;
            } else {
                rv2 = atoi(parm);
            } /* endif */
            xt_log2 ("\tstring_length (expected) = %s (integer value = %s)", parm, xt_itoa(rv2));

            if (xt_cmp_integer (rv2, string_length) != XT_OK) {
                /* test has failed, must write to log file */
                XT_LOG_ERROR ("expected and received string_length returned differ");
                xt_log ("\t(ERROR) expected and received string_length returned from om_write differ");
				xt_log_both1("expected string %s", string_tbl[rv2].elements);
				xt_log_both1("returned string %s", xt_itoa(string_length));
                return(XT_FAILED);
            } else {
                xt_log ("\texpected and received string_length returned from om_write same");
            } /* endif */
        } /* endif */
    } /* endif */

    xt_log ("\tom_write() returned OK\n");
    return (XT_OK);

} /* end xt_om_write */


/****************************************************************************/
/*                                                                          */
/*          xt_omX_fill()                   */
/*                                                                          */
/****************************************************************************/

OM_sint xt_omX_fill (void)
{
	const char function_name[] = "xt_omX_fill";

#if 0 /* not required yet, if ever CC 26.1.94 */

    OM_sint parm2, parm3, parm4, parm5, parm6;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      return_code;
    OM_string_length    string_length;
    OM_private_object   subject;
    OM_object           destination;
    OM_sint table_type;


    /* read destination parameter from the testcase file */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm3, parm, &is_void, &destination) != XT_OK) {
        XT_LOG_ERROR ("Error reading destination parameter");
        return XT_ERROR ;
    } /* endif */
    xt_log1 ("\tdestination = %s", parm);


    /* read type parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_TYPE, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading type parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\ttype = %s", parm);

    /* read syntax parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_SYNTAX, &parm4, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading syntax parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tsyntax = %s", parm);

    /* read length parameter from the testcase file */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading length parameter");
        return(XT_NOT_CALLED);
    } /* endif */


    /* read elements parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_STRING, &parm6, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading elements parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\telements = %s", parm);


    if (!strcmp(parm, "OM_ALL_VALUES"))  {  /* if parameter = OM_ALL_VALUES  */
        parm5 = OM_ALL_VALUES;
    } else {
        parm5 = atoi(parm);
    } /* endif */
    xt_log2 ("\tstring_length = %s (integer value = %s)", parm, xt_itoa(parm5));
    string_length = parm5;
    /* call the interface function, om_fill(), with the parameters read */
    xt_log ("\tcall om_fill()");

    return_code = omX_fill (
			type_tbl[parm2], 
			parm3,
			syntax_tbl[parm4], 
			&string_length,
			string_tbl[parm6] );

    xt_log1 ("\tom_fill return code = %s", xt_om_error_to_string(return_code));

    /* read expected return_code value from the testcase file */
    /* and compare with received return_code                  */
    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) == XT_ERROR) {
        XT_LOG_ERROR ("Error reading return_code returned");
        return XT_ERROR ;
    } /* endif */

    if (!is_void) {
        if (xt_cmp_return_code (return_code, return_code_tbl[rv1]) != XT_OK)  {
            /* test has failed, must fill to log file */
            XT_LOG_ERROR ("expected and received OM_return_code differ");
            xt_log ("\t(ERROR) expected and received OM_return_code from om_fill differ");
			xt_log_both1("expected OM_return_code  %s", xt_om_error_to_string(return_code_tbl[rv1]));
			xt_log_both1("returned OM_return_code  %s", xt_om_error_to_string(return_code));
            return(XT_FAILED);
        } else {
            xt_log ("\texpected and received OM_return_code from om_fill same");
        } /* endif */
	} /* endif */

    xt_log ("\tom_fill() returned OK\n");


    return (XT_OK);
#endif /* 0 */
	return (XT_FAILED); /* temporary */

} /* end xt_omX_fill */


/****************************************************************************/
/*                                                                          */
/*          xt_omX_fill_oid()                   */
/*                                                                          */
/****************************************************************************/

OM_sint xt_omX_fill_oid (void)
{
/*	static const char function_name[] = "xt_omX_fill_oid"; */
    return (XT_FAILED);
} /* end */



/****************************************************************************/
/*                                                                          */
/*          xt_omX_extract()                   							*/
/*                                                                          */
/*  takes as parameters in testcase: 										*/
/* 		navigation_path														*/
/* 		exclusions 															*/
/* 		included_types 														*/
/* 		local_strings 														*/
/* 		initial_value 														*/
/* 		limiting_value 														*/
/*      expected OM return code												*/
/*      expected values														*/
/*      (if OM_success expected) expected number returned                   */
/*      (if OM_success expected) expected object returned                   */
/*                                                                          */
/* takes as object to extract from                                           */
/* 	private_object_tbl[PRIVATE_RESULT_INDEX]                                */
/*                                                                          */
/****************************************************************************/

OM_sint xt_omX_extract (void)
{
	static const char function_name[] = "xt_omX_extract";

	OM_value_position initial_value;
	OM_value_position limiting_value;
	OM_public_object values;
	OM_value_position total_number;
    OM_return_code om_return_code;
    OM_sint xt_return_code = XT_FAILED;
    OM_sint is_void;
    OM_sint rv1,rv2;
    char    parm[64];
	OM_sint parm2,parm3,parm4,parm5;
	OM_sint expected_number_returned;


    /* read navigation_path */
    if (xt_read_parm (XT_PARAMETER, XT_NAV_PATH, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading navigation_path parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tnavigation_path = %s", parm);

    /* read exclusions */
    if (xt_read_parm (XT_PARAMETER, XT_EXCLUSIONS, &parm3, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading exclusions source_type parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\texclusions = %s", parm);


    /* read included_types */
    if (xt_read_parm (XT_PARAMETER, XT_TYPE_LIST, &parm4, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading included_types parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tincluded_types = %s", parm);


    /* read local_strings */
    if (xt_read_parm (XT_PARAMETER, XT_BOOLEAN, &parm5, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading local_strings parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tlocal_strings = %s", parm);



    /* read initial_value */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading initial_value parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  {
        initial_value = OM_ALL_VALUES;
    } else {
        initial_value = atoi(parm);
    } /* endif */
    xt_log1 ("\tinitial_value = %s", xt_itoa(initial_value));


    /* read limiting_value */
    if (xt_read_string (parm) != XT_OK)  {
        XT_LOG_ERROR ("Error reading limiting_value parameter");
        return(XT_NOT_CALLED);
    } /* endif */

    if (!strcmp(parm, "OM_ALL_VALUES"))  { 
        limiting_value = OM_ALL_VALUES;
    } else {
        limiting_value = atoi(parm);
    } /* endif */
    xt_log1 ("\tlimiting_value = %s", xt_itoa(limiting_value));

    om_return_code = omX_extract(
	/* OM_private_object  object IN  - Extract from this object    */
		private_object_tbl[PRIVATE_RESULT_INDEX],
 	/* OM_type_list navigation_path IN  - Leads to the target object  */
		nav_path_tbl[parm2],
    /* OM_exclusions      exclusions IN  - Scope of extraction         */
		exclusions_tbl[parm3],
    /* OM_type_list       included_types IN  - Objects to be extracted     */
		type_list_tbl[parm4],
    /* OM_boolean         local_strings IN  - Not yet supported       */
		boolean_tbl[parm5],
    /* OM_value_position  initial_value IN  - First value to be extracted */
		initial_value,
    /* OM_value_position  limiting_value IN  - Last value to be extracted  */
		limiting_value,
    /* OM_public_object  *values OUT - Array of extracted objects  */
		&values,
    /* OM_value_position *total_number OUT - Count of extracted objects  */
		&total_number);

    /* read expected om_return_code value from the testcase file    */
    /* and compare with received om_return_code                 */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading OM_return_code");
        xt_log_both1 ("\tread %s", parm);
		xt_return_code = XT_ERROR;
		goto end;
    } /* endif */

	xt_log1("\texpected OM_return_code  %s", parm);
    if (!strcmp(parm, xt_om_error_to_string(om_return_code))) {
        /* expected and received same */
        xt_log ("\texpected and received OM_return_code from omX_extract same");
        if (!strcmp (parm, "OM_SUCCESS")) {
            /* OM_SUCCESS expected and received */
			xt_log1("\tnumber extracted was %s\n", xt_itoa(total_number));

			/* read expected number returned */
			if (xt_read_string (parm) != XT_OK) {
				XT_LOG_ERROR ("Error reading expected number returned");
				xt_return_code = XT_ERROR;
				goto end;
			} /* endif */
			expected_number_returned = atoi(parm);
			xt_log1("\texpected number returned was %s\n", xt_itoa(expected_number_returned));


			if( total_number != expected_number_returned) {
				xt_log_both ("\t(ERROR) expected and received number returned from omX_extract differ");
				xt_log_both1("\texpected %s", xt_itoa(expected_number_returned));
				xt_log_both1("\treturned %s", xt_itoa(total_number));
				goto end;
			} else {
				/* see if expected values returned  */
				if (xt_read_public_object (XT_RETURN_VAL, XT_PUBLIC_OBJECT, &rv2, parm, &is_void, &values) == XT_ERROR)  {
					XT_LOG_ERROR ("Error reading copy returned");
					xt_return_code = XT_ERROR ;
					goto end;
				} else {
					if (xt_cmp_object ( public_object_tbl[rv2], values) != XT_OK) {
						/* test has failed, must write to log file */
						XT_LOG_ERROR ("expected and received values differ");
						xt_log ("\t(ERROR) expected and received values from omX_extract differ");
						xt_return_code = XT_FAILED;
						goto end;
					} else {
						xt_log ("\texpected and received copy_return from omX_extract same");
					} /* endif */
				} /* endif */
			} /* endif */
        } /* endif */
        xt_return_code = XT_OK;
    } else {
        xt_log_both ("\t(ERROR) expected and received OM_return_code from omX_extract differ");
        xt_log_both1("\texpected %s", parm);
        xt_log_both1("\treturned %s", xt_om_error_to_string(om_return_code));
        xt_return_code = XT_FAILED;
    } /* endif */

#if 0 /* commented out 8.1.94 CC */
	if( xt_return_code == XT_OK && total_number != 0) {
		om_return_code = om_delete(values);
		if( om_return_code != OM_SUCCESS) {
			xt_log_both ("\t(ERROR) bad OM_return_code from om_delete in omX_extract");

			xt_log_both1("\tom_delete returned %s", xt_om_error_to_string(om_return_code));
			xt_return_code = XT_FAILED;
		} /* endif */
	} /* endif */
#endif

end:
	return (xt_return_code);
} /* end xt_omX_extract */




/***********************************************************************/
/*                                                                      */
/*          xt_omX_string_to_object()                 				*/
/*                                                                      */
/* takes as parameters in testcase:										*/
/*	  class		 														*/
/*    string															*/
/*   expected OM return code											*/
/*  [if expected OM return code not OM_SUCCESS:							*/
/*     error_type  -- XT_XRETURN_CODE extended XOM return code			*/
/*    error_position]													*/
/***********************************************************************/

OM_sint xt_omX_string_to_object (void)
{
	static const char function_name[] = "xt_omX_string_to_object";
    OM_sint parm1;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      om_return_code;
    OM_string string;
    char    string_elements[XT_MAX_XOMS_STRING];
    OM_sint xt_return_code = XT_FAILED;
    OM_private_object object;
    OM_integer error_position; /* type determined by function */
    OM_integer error_type; /* type determined by function */
	OM_sint		int_read;

    string.elements = string_elements;

	/* read class */
    if (xt_read_parm (XT_PARAMETER, XT_CLASS, &parm1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading class parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tobject class = %s", parm);

    /* read string parameter from the testcase file */
    if (xt_read_quoted_string (string_elements) != XT_OK) {
        XT_LOG_ERROR ("Error reading string parameter");
        return(XT_NOT_CALLED);
    } /* endif */
    string.length = strlen(string_elements);
    xt_log1 ("\tstring = \"%s\"", string_elements);

    /* read expected om_return_code value from the testcase file    */
    /* and compare with received om_return_code                 */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading OM_return_code");
		xt_log_both1("\tread %s",parm);
        return(XT_ERROR);
    } /* endif */

    om_return_code = omX_string_to_object(
		/* OM_workspace workspace  */  convenience_workspace,
		/* OM_string *string, */  &string,
		/* OM_object_identifier class */ *class_tbl[parm1].class_type,
		/* OM_boolean local_strings */ OM_TRUE,
		/* OM_private_object *object  */  &object,
		/* OM_integer   *error_position     */  &error_position,
		/* OM_integer *error_type)    */  &error_type);


    if (!strcmp(parm, xt_om_error_to_string(om_return_code))) {
        /* expected and received same */
        xt_log ("\texpected and received OM_return_code from omX_string_to_object same");
        if (strcmp (parm, "OM_SUCCESS")) {
			/* read expected extended OM return code */
            if (xt_read_parm (XT_RETURN_VAL, XT_XRETURN_CODE, &rv1, parm, &is_void) != XT_OK) {
                XT_LOG_ERROR ("Error reading extended OM_return_code");
				xt_log1("\tread %s",parm);
                return(XT_ERROR);
            } else {
				if (rv1 != error_type)  {
                    XT_LOG_ERROR ("wrong error type");
                    xt_log_both2("\tomX_string_to_object expected %s, or %s", parm, xt_itoa(rv1));
                    xt_log_both1("\tomX_string_to_object returned %s", xt_itoa(error_type));
                } /* endif */

                /* read expected error position and check if same returned */
                if (xt_read_int (&int_read) != XT_OK) {
                    XT_LOG_ERROR ("Error reading expected error position");
					xt_log_both1("\tread %s",xt_itoa(int_read));
                    xt_return_code = XT_ERROR;
                    goto end;
                } /* endif */

                if (int_read != error_position) {
                    XT_LOG_ERROR ("wrong error position");
                    xt_log_both1("\tomX_string_to_object expected %s", parm);
                    xt_log_both1("\tomX_string_to_object returned %s", xt_itoa(error_position));
                } /* endif */
			} /* endif */
        } /* endif */
        xt_return_code = XT_OK;
    } else {
        xt_log_both ("\t(ERROR) expected and received OM_return_code from omX_string_to_object differ");
        xt_log_both1("\tomX_string_to_object expected %s", parm);
        xt_log_both1("\tomX_string_to_object returned %s", xt_om_error_to_string(om_return_code));
        xt_return_code = XT_FAILED;
    } /* endif */

end:
    return (xt_return_code);

} /* end xt_omX_string_to_object */

/***********************************************************************/
/*                                                                      */
/*          xt_omX_object_to_string()				                    */
/*                                                                      */
/* takes as parameters in testcase:
        object
		local_strings flag
        expected OM return code
        [expected string if expected return code was success]
*/
/***********************************************************************/

OM_sint xt_omX_object_to_string (void)
{
	static const char function_name[] = "xt_omX_object_to_string";
    OM_sint parm1,parm2;
    OM_sint rv1;
    OM_sint is_void;
    char    parm[64];
    OM_return_code      om_return_code;
    OM_string string;
    char    string_elements[XT_MAX_XOMS_STRING];
    char    expected_string[XT_MAX_XOMS_STRING];
    OM_sint xt_return_code = XT_FAILED;
    OM_object       input_object;
    OM_sint table_type;
    char    *pchar;

    /* initialize string */
    string.elements = string_elements;
    string.length = XT_MAX_XOMS_STRING;

	/* read object */
    if (xt_read_object (XT_PARAMETER, &table_type, &parm1, parm, &is_void, &input_object) != XT_OK) {
        XT_LOG_ERROR ("Error reading object parameter");
		xt_log_both1("\tread %s",parm);
        return(XT_NOT_CALLED);
    } /* endif */

    xt_log1 ("\tobject = %s", parm);

    /* read local strings flag parameter from the testcase file */
    if (xt_read_parm (XT_PARAMETER, XT_BOOLEAN, &parm2, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading local strings flag parameter");
		xt_log_both1("\tread %s",parm);
        return(XT_NOT_CALLED);
    } /* endif */
    xt_log1 ("\tlocal_strings = %s", parm);

    /* read expected om_return_code value from the testcase file    */
    /* and compare with received om_return_code                 */

    if (xt_read_parm (XT_RETURN_VAL, XT_RETURN_CODE, &rv1, parm, &is_void) != XT_OK) {
        XT_LOG_ERROR ("Error reading expected OM_return_code");
		xt_log_both1("\tread %s",parm);
        return(XT_ERROR);
    } /* endif */
    xt_log1 ("\texpected OM_return_code = %s", parm);

    om_return_code = omX_object_to_string(
	/* OM_object    object           */ input_object,
	/* OM_boolean   local_strings  */ boolean_tbl[parm2],
    /* OM_string   *string          */ &string	);

    if (!strcmp(parm, xt_om_error_to_string(om_return_code))) {
        /* expected and received same */
        xt_log ("\texpected and received OM_return_code from omX_object_to_string same");
        if (!strcmp (parm, "OM_SUCCESS")) {
            /* OM_SUCCESS expected and received */
            if (xt_read_quoted_string(expected_string) != XT_OK) {
                XT_LOG_ERROR ("Error reading expected string parameter");
				xt_log_both1("\tread %s",parm);
                xt_return_code = XT_FAILED;
            } else {
				xt_log ("\txt_omX_object_to_string: omX_object to string: OM_SUCCESS expected and received");
                /* compare received with expected string */
                if (!strcmp(expected_string, pchar = OM_to_normal_string(string))) {
					free(pchar);
					xt_log ("\txt_omX_object_to__string: omX_object to string: expected string was received");
                    xt_return_code = XT_OK;
                } else {
					free(pchar);
					xt_log_both ("\txt_omX_object_to_string: omX_object to string: expected string NOT received");
					xt_log_both1("\texpected string <<<%s>>>",expected_string);
					xt_log_both1("\treceived string <<<%s>>>",OM_to_normal_string(string));
					free(pchar);
                    xt_return_code = XT_FAILED;
                } /* endif */
            } /* endif */
		} else{
			/* expected unsuccessful outcome */
			xt_return_code = XT_OK;
        } /* endif */
    } else {
        xt_log_both ("\t(ERROR) expected and received OM_return_code from omX_object_to_string differ");
        xt_log_both1("\tomX_object_to_string expected %s", parm);
        xt_log_both1("\tomX_object_to_string returned %s", xt_om_error_to_string(om_return_code));
        xt_return_code = XT_FAILED;
    } /* endif */

    return (xt_return_code);

} /* end xt_omX_object_to_string */


/*********************************************************************/
/* utility for readable OM_errors 									*/
/*********************************************************************/
/* must be kept aligned with xom.h: */
const struct {
    char    *string;
    OM_sint value;
} om_error[] = {
    { "OM_SUCCESS",				0 },
    { "OM_ENCODING_INVALID",	1 },
    { "OM_FUNCTION_DECLINED",	2 },
    { "OM_FUNCTION_INTERRUPTED",3 },
    { "OM_MEMORY_INSUFFICIENT",	4 },
    { "OM_NETWORK_ERROR",		5 },
    { "OM_NO_SUCH_CLASS",		6 },
    { "OM_NO_SUCH_EXCLUSION",	7 },
    { "OM_NO_SUCH_MODIFICATION",8 },
    { "OM_NO_SUCH_OBJECT",		9 },
    { "OM_NO_SUCH_RULES",		10 },
    { "OM_NO_SUCH_SYNTAX",		11 },
    { "OM_NO_SUCH_TYPE",		12 },
    { "OM_NO_SUCH_WORKSPACE",	13 },
    { "OM_NOT_AN_ENCODING",		14 },
    { "OM_NOT_CONCRETE",		15 },
    { "OM_NOT_PRESENT",			16 },
    { "OM_NOT_PRIVATE",			17 },
    { "OM_NOT_THE_SERVICES",	18 },
    { "OM_PERMANENT_ERROR",		19 },
    { "OM_POINTER_INVALID",		20 },
    { "OM_SYSTEM_ERROR",    21 },
    { "OM_TEMPORARY_ERROR",		22 },
    { "OM_TOO_MANY_VALUES",		23 },
    { "OM_VALUES_NOT_ADJACENT",	24 },
    { "OM_WRONG_VALUE_LENGTH",	25 },
    { "OM_WRONG_VALUE_MAKEUP", 	26 },
    { "OM_WRONG_VALUE_NUMBER",	27 },
    { "OM_WRONG_VALUE_POSITION",28 },
    { "OM_WRONG_VALUE_SYNTAX",	29 },
    { "OM_WRONG_VALUE_TYPE",	30 },
    { "OMX_CLASS_NOT_FOUND_IN_SCHEMA_FILE",	31 }
};






/***********************************************************************/

char    *xt_om_error_to_string(OM_sint i)
{
	if (xt_do_to_string_and_back) {
        assert (i >= 0  && i <= 31);
	} else {
        assert (i >= 0  && i <= 30);
	} /* endif */

	return(om_error[i].string);
} /* end xt_om_error_to_string */


/***********************************************************************/
/*                                                                     */
/*                    xt_to_string_and_back()                      */
/*                                                                     */
/***********************************************************************/

/* calls convenience library function to convert object
parameter to string, prints string in log file,
calls convenience library function to convert string
back to object, then calls om_get to convert private
to public object. Depending on whether the original
object was private or public, the new private or public
object replaces it, and the other is deleted */

OM_return_code
xt_to_string_and_back(
			OM_object *pobject,
			const char *object_name)
{
	static const char function_name[] = "xt_to_string_and_back";
    OM_public_object new_public_object;
    OM_private_object new_private_object;
    OM_string string;
    char    string_elements[XT_MAX_XOMS_STRING];
    OM_return_code om_return_code = OM_SUCCESS;
    OM_integer error_position; /* type determined by function */
    OM_integer error_type, int_read; /* type determined by function */
    OM_value_position total_number;
    OM_object_identifier  class;
    char    *pchar;
	BOOLEAN was_private;
	OM_workspace effective_workspace;
	#define OBJECT_IN_CLASS(object, class) \
		( OMP_CLASS_ID_STR(object).length ==  class.length && \
		 !memcmp(OMP_CLASS_ID_STR(object).elements,class.elements,class.length))

		  
    if ( *pobject == NULL) {
        om_return_code == OM_SUCCESS;
        goto end;
    } /* endif */


	/* filter out unconvertible classes */
	if (!( OBJECT_IN_CLASS(*pobject,DS_C_DS_DN) ||
		   OBJECT_IN_CLASS(*pobject,DS_C_DS_RDN) ||
		   OBJECT_IN_CLASS(*pobject,DS_C_AVA) ||
		   OBJECT_IN_CLASS(*pobject,DS_C_FACSIMILE_PHONE_NBR) ||
		   OBJECT_IN_CLASS(*pobject,DS_C_POSTAL_ADDRESS) ||
		   OBJECT_IN_CLASS(*pobject,DS_C_TELEX_NBR) ||
		   OBJECT_IN_CLASS(*pobject,DS_C_PRESENTATION_ADDRESS) ||
		   OBJECT_IN_CLASS(*pobject,DS_C_ATTRIBUTE) ||
		   OBJECT_IN_CLASS(*pobject,DSX_C_GDS_ACL) ||
		   OBJECT_IN_CLASS(*pobject,DSX_C_GDS_ACL_ITEM) ||
		   OBJECT_IN_CLASS(*pobject,MH_C_OR_ADDRESS) ||
		   OBJECT_IN_CLASS(*pobject,MH_C_OR_NAME))) {
		om_return_code = OM_SUCCESS;
		goto end;
	} /* endif */

	/* don't try to convert objects beginning with 'bad_' */
	/* which will cause problems */
	if(!strncmp(object_name,"bad_",4)) {
		om_return_code = OM_SUCCESS;
		goto end;
	} /* endif */

	/* see if input object was public or private */
	was_private = OMP_PRIVATE(*pobject);
    xt_log_both1("\tobject was%sprivate",was_private?" ":" not ");

	/* read class from input *pobject */
	class = (*pobject)->value.string;


    /* initialize string */
    string.elements = string_elements;
    string.length = XT_MAX_XOMS_STRING;

    om_return_code = omX_object_to_string(
	/* OM_object object */				*pobject,
	/* OM_boolean local_strings */		OM_TRUE,
    /* OM_string *string */				&string);

    if (om_return_code != OM_SUCCESS) {
        xt_log_both1("\tomX_object_to_string failed: %s", xt_om_error_to_string(om_return_code));
        xt_log_both1("\txt_to_string_and_back: object was: %s", object_name);
        goto end;
    } /* endif */

    xt_log_both("\tobject_to_string returned <<<<");
    xt_log_both(pchar = OM_to_normal_string(string));
    free(pchar);
    xt_log_both_cat(">>>>");
    xt_log_both(object_name);

	/* get appropriate workspace for object */
	if (OMP_CPUB(*pobject)) {
		effective_workspace = convenience_workspace;
	} else {
		effective_workspace = OMP_WORKSPACE(*pobject);
	} /* endif */


    om_return_code = omX_string_to_object(
	/* OM_workspace workspace   */		effective_workspace,
	/* OM_string *string        */		&string,
	/* OM_object_identifier  class */	class,
	/* OM_boolean local_strings */ OM_TRUE,
    /* OM_private_object *object*/		&new_private_object,
	/* OM_integer *error_position      */		&error_position,
	/* OM_integer *error_type         */		&error_type);

    if (om_return_code != OM_SUCCESS) {
        xt_log_both1("\txt_to_string_and_back: omX_string_to_object failed: %s", xt_om_error_to_string(om_return_code));
        xt_log_both("\tfailing string was <<<<");
        xt_log_both(pchar = OM_to_normal_string(string));
        free (pchar);
        xt_log_both(">>>>");
        xt_log_both1("\terror position was %s",xt_itoa(error_position));
        xt_log_both1("\terror type was %s",xt_itoa(error_type));
        goto end;
	} else {
        xt_log_both("\txt_to_string_and_back: omX_string_to_object succeeded");
    } /* endif */


	/* call om_get as check of structure of new object */
    om_return_code = om_get(
	/* OM_private_object   original			*/ 	new_private_object,
	/* OM_exclusions       exclusions		*/	OM_NO_EXCLUSIONS,
    /* OM_type_list        included_types	*/	(OM_type_list) 0,
	/* OM_boolean      local_strings		*/	OM_TRUE,
    /* OM_value_position   initial_value	*/	(OM_value_position) 0,
	/* OM_value_position   limiting_value	*/	(OM_value_position) 0,
    /* OM_public_object        *copy		*/	&new_public_object,
	/* OM_value_position       *total_number*/	&total_number);

	xt_log_both1("\ttotal number was %s", xt_itoa(total_number));

    if (om_return_code == OM_SUCCESS) {
		OM_object object_to_be_deleted, object_to_be_returned;
		xt_log_both("\txt_to_string_and_back: om_get of new object succeeded");
		if( was_private) {
			object_to_be_deleted = new_public_object;
			object_to_be_returned = new_private_object;
		} else {
			object_to_be_deleted = new_private_object;
			object_to_be_returned = new_public_object;
		} /* endif */

		/* delete original object if private or service-generated public*/
#if 0
/******** Commented this code --- B T NAIK */
		if (OMP_PRIVATE(*pobject) ||  OMP_SPUB(*pobject)) {
			om_return_code = om_delete(*pobject);
			if (om_return_code != OM_SUCCESS) {
				xt_log_both1("\txt_to_string_and_back: om_delete of original object failed: %s", xt_om_error_to_string(om_return_code));
				xt_log_both1("\txt_to_string_and_back: object was: %s", object_name);
				goto end;
			} else {
				xt_log_both("\txt_to_string_and_back: om_delete of original object succeeded");
			} /* endif */
		} /* endif */
#endif /* 0 */
/*********** End of Commented Code --- B T NAIK ***/

/*** Included this new code --- B T NAIK *****/

	    if (was_private) {
			om_put(*pobject, OM_REPLACE_ALL, object_to_be_returned, 0, 0, 0);

/* moved the next line here CC 8.1.94 */
		} else {
			*pobject = object_to_be_returned;
		} /* endif */
/*** End of included new code --- B T NAIK ***/


		om_return_code = om_delete(object_to_be_deleted);
		if (om_return_code != OM_SUCCESS) {
			xt_log_both2("\txt_to_string_and_back: om_delete of %s failed: %s",
			was_private?"new_public_object":"new_private_object", xt_om_error_to_string(om_return_code));
		} /* endif */
	} else {
		xt_log_both1("\txt_to_string_and_back: om_get of new object failed: %s",xt_om_error_to_string(om_return_code));
    } /* endif */

	if(om_return_code == OM_SUCCESS ) {
		xt_log_both1("\txt_to_string_and_back of %s succeeded", object_name);
	} else {
		xt_err1(function_name,"object was %s", object_name);
	} /* endif */

end:
	if (!xt_strict_to_string_and_back) {
		om_return_code = OM_SUCCESS;
	} /* endif */

	return (om_return_code);
} /* end xt_to_string_and_back */



/***********************************************************************/
/*                                                                      */
/*          OM_to_normal_string()                                   */
/*                                                                      */
/*  NOTE: user must free allocated space returned !!!                   */
/***********************************************************************/
char *OM_to_normal_string(
	OM_string string)
{
    char    *pchar = malloc(string.length + 1);
    strncpy(pchar, string.elements, string.length);
    pchar[string.length] = '\0';
    return (pchar);
} /* end OM_to_normal_string */

