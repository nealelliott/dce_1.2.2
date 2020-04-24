/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_file.c,v $
 * Revision 1.1.11.2  1996/02/18  00:24:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:57  marty]
 *
 * Revision 1.1.11.1  1995/12/08  22:13:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:49  root]
 * 
 * Revision 1.1.9.4  1994/07/06  15:09:35  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:26:29  marrek]
 * 
 * Revision 1.1.9.3  1994/05/10  15:33:12  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:22:32  marrek]
 * 
 * 	Use correct rcs id string.
 * 	[1994/05/02  15:03:31  marrek]
 * 
 * Revision 1.1.9.2  1994/03/23  16:06:40  keutel
 * 	March 1994 code drop
 * 	[1994/03/22  11:27:43  keutel]
 * 
 * 	March 1994 code drop
 * 	[1994/03/21  18:13:07  keutel]
 * 
 * Revision 1.1.9.1  1994/02/22  19:15:50  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:29:09  marrek]
 * 
 * Revision 1.1.7.2  1993/08/12  12:22:35  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:26:25  marrek]
 * 
 * Revision 1.1.5.3  1993/01/05  20:34:17  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:48:35  htf]
 * 
 * Revision 1.1.5.2  1992/12/01  15:02:21  marrek
 * 	November 1992 code drop
 * 	[1992/12/01  13:18:57  marrek]
 * 
 * Revision 1.1.3.3  1992/07/06  18:14:08  melman
 * 	CR 3975: xds test core dump client to client/server
 * 	[1992/07/06  18:11:58  melman]
 * 
 * Revision 1.1.3.2  1992/06/11  20:00:57  melman
 * 	 gds drop
 * 	[1992/06/11  19:23:33  melman]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: xt_file.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 00:24:37 $";
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
/* NAME         : xt_file.c                                           */
/*                                                                    */
/* AUTHOR       : Cahill              D AP11                          */
/* DATE         : 10.00.1990                                          */
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
/*          |     |                            |    |     */
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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <xt.h>
#include <xt_export.h>
#include <xt_parms.h>
#ifdef THREADSAFE
#	include <xt_thread.h>
#else
#	include <xt_nonthread.h>
#endif
#include <dsUtils.h>
#include <xt_file.h>

/**********************************************************************/
/*                                                                    */
/*                         xt_file.c                                  */
/*                                                                    */
/**********************************************************************/

char    xt_ofname[80] = "xt_test.log"; /* log file name */

/* This file contains a list of the names of all testcases */
/* to be run for -a (serial) and -t (threads, parallel) cases*/
char    xt_afname[40];
FILE    *xt_testcase_list_file = (FILE *) NULL; /* Testcases file */
FILE    *xt_log_file = (FILE *) NULL; /* log file */


static OM_sint xt_find_parm (OM_sint table_type, OM_sint *r_table_index, char *r_value);
static OM_sint xt_find_object (OM_sint *table_type, OM_sint *r_table_index,
			char *r_value, OM_object *p_object);
static OM_sint xt_find_private_object (OM_sint table_type, OM_sint *r_table_index,
			char *r_value, OM_private_object *p_object);
static OM_sint xt_find_public_object (OM_sint table_type, OM_sint *r_table_index,
			char *r_value, OM_public_object *p_object);
/****************************************************************************/
/*                                                                          */
/*   Open Auto Test Logfile for output.                                     */
/*                                                                          */
/****************************************************************************/

OM_sint xt_openo(void)
{
    char    *xt_path;
    char    xt_logname[D27_LFILE_NAME];

    if ((xt_path = getenv("D2_LOG_DIR")) == (char *) NULL)
        xt_path = getenv("HOME");

    sprintf(xt_logname, "%s/%s", xt_path, xt_ofname);
    if ((xt_log_file = fopen(xt_logname, "w")) == (FILE  * )    0 )  {
        xt_err1("xt_openo", "error creating output Log File - %s", xt_logname);
        return(XT_ERROR);
    } /* endif */

    return (XT_OK);
} /* end xt_openo */


/****************************************************************************/
/*   Open file containing a list of all testcases to be run                 */
/*                                                                          */
/****************************************************************************/

OM_sint xt_opena(void)
{
    OM_sint retc = XT_ERROR;

    if ((xt_testcase_list_file = fopen(xt_afname, "r")) == (FILE *  ) 0 ) {
        xt_err1("xt_opena", "error opening input file of Test Case names - %s", xt_afname);
        retc = XT_ERROR;
    } else {
        retc = XT_OK;
    } /* endif */

    return retc;
} /* end xt_opena */


/****************************************************************************/
/*   Close file containing a list of all testcases to be run                */
/*                                                                          */
/****************************************************************************/

OM_sint xt_closea(void)
{
    OM_sint retc = XT_ERROR;

    if (xt_testcase_list_file != (FILE * ) NULL) {
        fclose(xt_testcase_list_file);
        xt_testcase_list_file = (FILE *)NULL;
        retc = XT_OK;
    } else {
        retc = XT_ERROR;
    } /* endif */

    return retc;
} /* end xt_closea */


/****************************************************************************/
/*                                                                          */
/*   Open existing Test Case File for input.                        */
/*   Will read the functions and from this file and execute them        */
/*   automatically.                                                         */
/*                                                                          */
/****************************************************************************/

OM_sint xt_opent(void)
{
    OM_sint retc = XT_ERROR;

    if (xt_runtype != XT_RUNTYPE_THREAD) {
        XT_PRINTF("Testcase = %s\n\n", xt_testname);
    } /* endif */

    if ((xt_testcase_file = fopen(xt_testname, "r")) == (FILE * ) 0 ) {
        xt_err1("xt_opent", "error opening input Test Case file - %s", xt_testname);
        retc = XT_ERROR;
    } else {
        retc = XT_OK;
    } /* endif */

    return retc;
} /* end xt_opent */


/****************************************************************************/
/*                                                                          */
/*   Close existing Test Case File.                                     */
/*                                                                          */
/****************************************************************************/

OM_sint xt_closet(void)
{
    OM_sint retc = XT_ERROR;

    if (xt_testcase_file != (FILE *) NULL)  {
        fclose(xt_testcase_file);
        xt_testcase_file = (FILE * )NULL;
        retc = XT_OK;
    } else {
        retc = XT_ERROR;
    } /* endif */

    return retc;
} /* end xt_closet */


/****************************************************************************/
/*                                                                          */
/*   Open Include file                              */
/*                                                                          */
/****************************************************************************/

OM_sint xt_openp(const char *filename)
{
    OM_sint retc = XT_ERROR;

    if ((xt_testcase_file = fopen(filename, "r")) == (FILE * ) 0 ) {
        xt_err1("xt_openp", "error opening input Include file - %s", filename);
        retc = XT_ERROR;
    } else {
        retc = XT_OK;
    } /* endif */

    return retc;
} /* end xt_openp */


/****************************************************************************/
/*                                                                          */
/*   Close Include file                             */
/*                                                                          */
/****************************************************************************/

OM_sint xt_closep(void)
{
    OM_sint retc = XT_ERROR;

    if (xt_testcase_file != (FILE *) NULL)  {
        fclose(xt_testcase_file);
        xt_testcase_file = (FILE * )NULL;
        retc = XT_OK;
    } else {
        retc = XT_ERROR;
    } /* endif */

    return retc;
} /* end closep */


/****************************************************************************/
/*                                                                          */
/*   Read the file name of the next testcase to be run and open the         */
/*   test case file. Return EOF if have read last case                      */
/*                                                                          */
/****************************************************************************/

OM_sint xt_rtest(void)
{
    OM_sint retc = XT_ERROR;
    char    linebuf[XT_MAXLINE+1];

    do {
        if ((fgets (linebuf, XT_MAXLINE, xt_testcase_list_file)) ==  NULL) {
            xt_err1("xt_rtest", "EOF input file of testcases - %s\n", xt_afname);
            return  XT_EOF;
        } /* endif */
        sscanf (linebuf , "%s", xt_testname);
    } while (!strncmp(xt_testname, "--", 2));

    if (xt_opent() != XT_OK) {
        xt_err1("xt_rtest", "error opening input Test Case file - %s", xt_testname);
        retc = XT_ERROR;
    } else {
        retc = XT_OK;
    } /* endif */

    return retc;
} /* end xt_rtest */



/****************************************************************************/
/*                                                                          */
/*   Read the file name of the next testcase to be run and open the         */
/*   test case file. Return EOF if on last case.                            */
/*                                                                          */
/****************************************************************************/

OM_sint xt_rtest_till_last(void)
{
    OM_sint retc = XT_ERROR, buf[8];

    retc = xt_rtest();
    if (retc == XT_OK) {
        buf[0] = fgetc(xt_testcase_list_file);
        buf[1] = fgetc(xt_testcase_list_file);
        if (buf[0] == EOF || buf[1] == EOF) {
            retc = XT_EOF;
        } else {
            ungetc(buf[1], xt_testcase_list_file);
            ungetc(buf[0], xt_testcase_list_file);
        } /* endif */
    } /* endif */

    return retc;
} /* end xt_rtest_till_last */


/*****************************************************************************/
/*                                                                           */
/*   Functions to write and read the header and trailers for the parameter   */
/*   blocks.                                                                 */
/*                                                                           */
/*****************************************************************************/

void xt_log_head(const char *entry)    /* Write testcase header to log file */
{
    long    t;

    (void) fprintf (xt_log_file, "\n\n%s ************************************\
********************************************\n",
                                                 xt_this_thread_id_and_name());

#if 1
    time(&t);
    (void) fprintf (xt_log_file, "%s: TESTCASE: %s    %.20s    PID: %d\
     USER-ID: %-8s\n\n", xt_this_thread_id_and_name(),
                        entry, ctime(&t), getpid(), cuserid((char
    *)0));
#endif
} /* end xt_log_head */


OM_sint xt_log_func(const char *entry) /* Write function name to log file   */
{
    fprintf(xt_log_file, "%s: Function %s : \n", xt_this_thread_id_and_name(), entry);
    (void) fflush  (NULL);
	return (XT_OK);
}


void xt_log_pass(const char *entry)    /* Write "PASSED" to log file        */
{
    fprintf(xt_log_file, "%s: TESTCASE %s PASSED\n\n ", xt_this_thread_id_and_name(), entry);
    (void) fflush  (NULL);
} /* end xt_log_pass */


void xt_log_fail(const char *entry)       /* Write "FAILED" to log file        */
{
    fprintf(xt_log_file, "%s: TESTCASE %s FAILED\n\n ", xt_this_thread_id_and_name(), entry);
    xt_num_failures++;
    (void) fflush  (NULL);
} /* end xt_log_pass */


/****************************************************************************/
/*                                                                          */
/*   xt_log_default_context -                                              */
/*   print the default context to the log file                  */
/*                                                                          */
/*   xt_log_default_session -                                              */
/*   print the default session to the log file                  */
/*                                                                          */
/****************************************************************************/
void xt_log_default_context (void)
{
    xt_log ("Default context -");
}


void xt_log_default_session (void)
{
    xt_log ("Default session -");
} /* end xt_log_default_session */


/****************************************************************************/
/*                                                                          */
/*   xt_read_parm -                                                         */
/*   function to read a parameter or return value string from the testcase  */
/*   file, check if it is "void" (only legal if a return value), if not     */
/*   validate that it corresponds to a valid parameter and returns the      */
/*   index within the subtable of the parameter table                       */
/*                                                                          */
/****************************************************************************/

OM_sint xt_read_parm ( OM_sint parm_type,
OM_sint table_type,
OM_sint *r_table_index,
char *r_value,
OM_sint *r_void )
{
    char    str[64];

    /* loop past blank lines and comments, ie lines begining with "--" */

    if (xt_read_string (str) == XT_EOF)  {
        xt_err ("xt_read_parm", "premature end of testcase file");
        return XT_ERROR;
    } /* endif */

    if (!strcmp (str, "void")) {    /* void parameter, don't compare    */
        /* with expected results            */
        if (parm_type == XT_PARAMETER) {
            xt_err ("xt_read_parm", "void parameter not allowed");
            return XT_ERROR;
        } /* endif */
        *r_void = TRUE;
    } else {
        *r_void = FALSE;
        if (xt_find_parm (table_type, r_table_index, str) != XT_OK)  {
            xt_err2("xt_read_parm", "can not locate parameter %s in table %s", str, xt_itoa(table_type));
            return XT_ERROR;
        } /* endif */
    } /* endif */
    strcpy(r_value, str);
    return XT_OK;
} /* end xt_read_parm */


static OM_sint xt_find_parm (OM_sint table_type, OM_sint *r_table_index, char *r_value)
{
    OM_sint i;

    for (i = 0; i < XT_LEN_PARAMETER_TBL; i++) {
        if (!strcmp (r_value, parameter_tbl[i].parm)) {
            if (parameter_tbl[i].type == table_type) {
                *r_table_index = parameter_tbl[i].index;
                return  XT_OK;
            } else {
                xt_err ("xt_find_parm", "wrong type of parameter");
                return  XT_ERROR;
            } /* endif */
        } /* endif */
    } /* end for */
    xt_err ("xt_find_parm", "parameter not in table of parameters");
    return  XT_ERROR;
} /* end xt_find_parm */


/****************************************************************************/
/*                                                                          */
/*   xt_read_object -                                                       */
/*   function to read a parameter or return value string from the testcase  */
/*   file, and return an object                                 */
/*                                                                          */
/****************************************************************************/

OM_sint xt_read_object ( OM_sint parm_type,
OM_sint *table_type,
OM_sint *r_table_index,
char *r_value,
OM_sint *r_void,
OM_object *object )
{
    char    str[64];

    /* loop past blank lines and comments, ie lines begining with "--" */

    if (xt_read_string (str) == XT_EOF)  {
        xt_err ("xt_read_object", "premature end of testcase file");
        return XT_ERROR;
    } /* endif */

    if (!strcmp (str, "void")) {    /* void parameter, don't compare    */
        /* with expected results            */
        if (parm_type == XT_PARAMETER) {
            xt_err ("xt_read_object", "void parameter not allowed");
            return XT_ERROR;
        } /* endif */
        *r_void = TRUE;
    } else {
        *r_void = FALSE;
        if (xt_find_object (table_type, r_table_index, str, object) !=  XT_OK) {
            xt_err2("xt_read_object", "can not locate parameter %s in table %s", str, xt_itoa(*table_type));
            return XT_ERROR;
        } /* endif */
        strcpy(r_value, str);
    } /* endif */
    return XT_OK;
} /* end xt_read_object */


static OM_sint xt_find_object (OM_sint *table_type, OM_sint *r_table_index, char *r_value,
OM_object *p_object)
{
    OM_object   object;
    OM_descriptor *     *table;
    BOOLEAN is_found = FALSE;
	OM_sint i;

    for (i = 0; i < XT_LEN_PARAMETER_TBL; i++) {
        if (!strcmp (r_value, parameter_tbl[i].parm)) {
            *r_table_index = parameter_tbl[i].index;
            *table_type = parameter_tbl[i].type;
            is_found = TRUE;
            break;
        } /* endif */
    } /* end for */

    if (is_found) {
        if (parameter_tbl[i].table == NULL)  {
            switch (*table_type) {
            case XT_SESSION:
                object = (OM_object)session_tbl[*r_table_index];
                break;
            case XT_PRIVATE_OBJECT:
                object = (OM_private_object)private_object_tbl[*r_table_index];
                break;
            case XT_PUBLIC_OBJECT:
                object = (OM_public_object)public_object_tbl[*r_table_index];
                break;
            default:
                xt_err ("xt_find_object", "parameter with NULL tbl not in table of parameters");
                return  XT_ERROR;
            } /* end switch */
        } else {
            table = parameter_tbl[i].table;
            object = table[*r_table_index];
        } /* endif */
        *p_object = object;
        return  XT_OK;
    } else {
        xt_err ("xt_find_object", "parameter not in table of parameters");
        return  XT_ERROR;
    } /* endif */

} /* end xt_find_object */


/****************************************************************************/
/*                                                                          */
/*   xt_read_private_object -                                               */
/*   function to read a parameter or return value string from the testcase  */
/*   file, and return a private object                              */
/*                                                                          */
/****************************************************************************/

OM_sint xt_read_private_object ( OM_sint parm_type,
OM_sint table_type,
OM_sint *r_table_index,
char *r_value,
OM_sint *r_void,
OM_private_object *private_object )
{
    char    str[64];

    /* loop past blank lines and comments, ie lines begining with "--" */

    if (xt_read_string (str) == XT_EOF)  {
        xt_err ("xt_read_private_object", "premature end of testcase file");
        return XT_ERROR;
    } /* endif */

    if (!strcmp (str, "void")) {    /* void parameter, don't compare    */
        /* with expected results            */
        if (parm_type == XT_PARAMETER) {
            xt_err ("xt_read_private_object", "void parameter not allowed");
            return XT_ERROR;
        } /* endif */
        *r_void = TRUE;
    } else {
        *r_void = FALSE;
        if (xt_find_private_object (table_type, r_table_index, str, private_object) != XT_OK) {
            xt_err2("xt_read_private_object", "can not locate parameter %s in table %s", str, xt_itoa(table_type));
            return XT_ERROR;
        }
        if (!(*private_object)->syntax & OM_S_PRIVATE) {
            xt_err ("xt_read_private_object", "object found is not PRIVATE");
            return XT_ERROR;
        }
        strcpy(r_value, str);
    }
    return XT_OK;
} /* end xt_read_private_object */



static OM_sint xt_find_private_object (OM_sint table_type, OM_sint *r_table_index, char *r_value,

OM_private_object *p_object)
{
    OM_private_object object;
    OM_sint i;

    for (i = 0; i < XT_LEN_PARAMETER_TBL; i++) {
        if (!strcmp (r_value, parameter_tbl[i].parm)) {
            *r_table_index = parameter_tbl[i].index;
            if (parameter_tbl[i].type == XT_PRIVATE_OBJECT)  {
                object = private_object_tbl[*r_table_index];
            } else {
                if (parameter_tbl[i].type == XT_BOUND_SESSION) {
                    object = (OM_object)session_tbl[*r_table_index];
                } else {


                    if (parameter_tbl[i].table == NULL)  {
                        switch (table_type)  {
                        case XT_SESSION:
                            object = (OM_object)session_tbl[*r_table_index];
                            break;
                        case XT_PRIVATE_OBJECT:
                            object = (OM_private_object)private_object_tbl[*r_table_index];
                            break;
                        case XT_PUBLIC_OBJECT:
                            object = (OM_public_object)public_object_tbl[*r_table_index];
                            break;
                        default:
                            xt_err ("xt_find_object", "parameter with NULL tbl not in table of parameters");
                            return  XT_ERROR;
                        } /* end switch */
                    } else {
                        object = parameter_tbl[i].table[*r_table_index];
                    } /* endif */


                } /* endif */
            } /* endif */
            *p_object = object;
            return  XT_OK;
        } /* endif */
    } /* end for */

    xt_err ("xt_find_private_object", "parameter not in table of parameters");
    return  XT_ERROR;
} /* end xt_find_private_object */


/****************************************************************************/
/*                                                                          */
/*   xt_read_public_object -                                                */
/*   function to read a parameter or return value string from the testcase  */
/*   file, and return a public object                               */
/*                                                                          */
/****************************************************************************/

OM_sint xt_read_public_object ( OM_sint parm_type,
OM_sint table_type,
OM_sint *r_table_index,
char *r_value,
OM_sint *r_void,
OM_public_object *public_object )
{
    char    str[64];

    /* loop past blank lines and comments, ie lines begining with "--" */

    if (xt_read_string (str) == XT_EOF)  {
        xt_err ("xt_read_public_object", "premature end of testcase file");
        return XT_ERROR;
    } /* endif */

    if (!strcmp (str, "void")) {    /* void parameter, don't compare    */
        /* with expected results            */
        if (parm_type == XT_PARAMETER) {
            xt_err ("xt_read_public_object", "void parameter not allowed");
            return XT_ERROR;
        } /* endif */
        *r_void = TRUE;
    } else {
        *r_void = FALSE;
        if (xt_find_public_object (table_type, r_table_index, str, public_object) != XT_OK) {
            xt_err2("xt_read_public_object", "can not locate parameter %s in table %s", str, xt_itoa(table_type));
            return XT_ERROR;
        } /* endif */
        if ((*public_object)->syntax & OM_S_PRIVATE) {
            xt_err ("xt_read_public_object", "object found is not PUBLIC");
            return XT_ERROR;
        }
        strcpy(r_value, str);
    } /* endif */
    return XT_OK;
} /* end xt_read_public_object */


static OM_sint xt_find_public_object (OM_sint table_type, OM_sint *r_table_index, char *r_value,

OM_public_object *p_object)
{
    OM_public_object object;
    OM_sint i;

    for (i = 0; i < XT_LEN_PARAMETER_TBL; i++)
        if (!strcmp (r_value, parameter_tbl[i].parm)) {
            *r_table_index = parameter_tbl[i].index;
            if (parameter_tbl[i].type == XT_PUBLIC_OBJECT)
                object = public_object_tbl[*r_table_index];
            else
                object = parameter_tbl[i].table[*r_table_index];
            *p_object = object;
            return  XT_OK;
        } /* end for */

    xt_err ("xt_find_public_object", "parameter not in table of parameters");
    return  XT_ERROR;
} /* end xt_find_public_object */


/****************************************************************************/
/*                                                                          */
/*   xt_read_string -                                                          */
/*   function to read a string from the testcase file                       */
/****************************************************************************/

OM_sint xt_read_string (char *r_value)
{
    BOOLEAN is_found = FALSE;
    char    t_str[64];
    char    c;
    OM_sint j;

    while (!is_found) {

        while (((c = fgetc(xt_testcase_file)) == ' ') || (c ==  '\n') ||  (c == '\t'))
            ;

        if (c == EOF)
            return XT_EOF;

        for (j = 0; (c != EOF) && (c != '\n') && (c != ' ') &&  (c !=  '\t'); c = fgetc(xt_testcase_file),
                          j++) {
            t_str[j] = c;
        } /* endfor */
        t_str[j] = '\0';

        if (strcmp (t_str, "--")) {     /* t_str != "--" */
            strcpy(r_value, t_str);
            is_found = TRUE;
        } /* endif */
        while ((c != '\n') && (c != EOF))
            c = fgetc(xt_testcase_file);
    } /* end while */
    return XT_OK;
} /* end xt_read_string */


/****************************************************************************/
/*   xt_read_int -                                                          */
/*   function to read an integer value from the testcase file               */
/*                                                                          */
/****************************************************************************/
OM_sint xt_read_int (OM_sint *value)
{
    if (fscanf (xt_testcase_file, "%d", value) != 1) {
        xt_err ("xt_read_int", "did not read parameter from file correctly");
        return  XT_ERROR;
    } /* endif */
    return  XT_OK;
} /* end xt_read_int */


/****************************************************************************/
/*   xt_read_equals -                                                       */
/*   function to read a string and integer value, format "%s = %s", */
/*   from the testcase file                                 */
/*                                                                          */
/****************************************************************************/
OM_sint xt_read_equals (char **r_str, OM_sint *r_val)
{
    BOOLEAN is_found = FALSE;
    char    linebuf[80];
    char    *str;
    OM_sint val;
    OM_sint retval;

    str = (char *)malloc(80);
    while (!is_found) {
        if (fgets(linebuf, XT_MAXLINE, xt_testcase_file) == (char
        *) NULL)
        return XT_EOF;

        retval = sscanf(linebuf, "%s = %d", str, &val);

        if (retval < 2)
            return XT_END_INPUT;

        if ((retval == 2) && strcmp    (str, "--")) {
            *r_str = str;
            *r_val = val;
            is_found = TRUE;
        } /* endif */
    } /* end while */
    return XT_OK;
} /* end xt_read_equals */


/****************************************************************************/
/*   xt_read_quoted_string -                                            */
/*   function to read a quoted string , format ".*"			                 */
/*   from the testcase file                                                 */
/*                                                                          */
/*   assumes return_string has XT_MAX_XOMS_STRING space                      */
/****************************************************************************/
OM_sint xt_read_quoted_string (char *return_string)
{
    BOOLEAN is_found = FALSE;
	OM_sint	return_code = XT_ERROR;
    char    linebuf[XT_MAX_XOMS_STRING];
    char    *first_quote, *last_quote;


    while (!is_found) {
        if (fgets(linebuf, XT_MAX_XOMS_STRING, xt_testcase_file) ==  (char *)NULL) {
            return_code = XT_EOF;
            break;
        } /* endif */

        if (!strcmp (linebuf, "--")) {
            continue;
        } else {
            first_quote = strchr(linebuf, '\"');
            last_quote = strrchr(linebuf, '\"');

            if (first_quote == NULL     || last_quote == first_quote) {
                return_code = XT_ERROR;
                break;
            } else {
                *last_quote = '\0';
                strcpy(return_string, first_quote + 1);
                is_found = TRUE;
                return_code = XT_OK;
            } /* endif */
        } /* endif */
    } /* end while */
    return (return_code);
} /* end xt_read_quoted_string */


/*****************************************************************************/
/*                                                                           */
/*   Conversion Routines.                                                    */
/*                                                                           */
/*****************************************************************************/

char    *xt_itoa (OM_sint i)
{
    (void) sprintf(itoa_buf, "%d", i);
    return itoa_buf;
} /* end xt_itoa */


char    *xt_ptoa (char *p)  /* Pointer to Ascii.                 */
{
    (void) sprintf(ptoa_buf, "%x", p);
    return ptoa_buf;
} /* end xt_ptoa */


/****************************************************************************/
/*                                                                          */
/*   Find the start of the next entry in the Auto Test File starting from   */
/*   the current position.                                                  */
/*   Returns XT_OK if found one, or XT_ERROR if hit EOF.                    */
/*                                                                          */
/*   xt_entry contains the name of the function or event found.             */
/*   On exit, the file pointer is repositioned at the start of the header   */
/*   line, ready to be read by xt_rhead().                                  */
/*                                                                          */
/****************************************************************************/



OM_sint xt_fnext(void)
{
    OM_sint retc = XT_ERROR;
    char    token[81],
    entry[81],
    linebuf [XT_MAXLINE+1];

    while (retc == XT_ERROR) {

        assert (xt_testcase_file != (FILE *)NULL);

        if (fgets(linebuf, XT_MAXLINE, xt_testcase_file) == (char *) NULL) {
            retc = XT_ERROR;
            break;
        } /* endif */


        if (sscanf(linebuf, XT_FMT_FUNC, token, entry) != 2)
            continue;

        /* Should be "Function","Include or "Failure" */
        if (strcmp(token, XT_FUNCTION_TOKEN) &&
		strcmp(token, XT_INCLUDE_TOKEN) )
            continue;

        /* It's a valid entry header.             */
        (void) strcpy(xt_entry, entry);

        if (!strcmp(token, XT_FUNCTION_TOKEN))
            retc = XT_FUNCTION;
        if (!strcmp(token, XT_INCLUDE_TOKEN))
            retc = XT_INCLUDE;
    } /* end while */

    return retc;
} /* end xt_fnext */


/****************************************************************************/
/*                                                                          */
/*   Error handling and reporting functions.                                */
/*   (Write to stderr for the moment - may change later to a real file.)    */
/*                                                                          */
/****************************************************************************/

void    xt_err(const char *func, const char *msg)
{
    XT_PRINTF(" Func %s : %s\n", func, msg);
} /* end xt_err */


void    xt_err1(const char *func, const char *msg, const char *p1)
{
    char    msgp1[256];

    sprintf(msgp1, msg, p1);
    XT_PRINTF(" Func %s : %s\n", func, msgp1);
} /* end xt_err1 */


void    xt_err2(const char *func, const char *msg, const char *p1, const char *p2)
{
    char    msgp12[256];

    sprintf(msgp12, msg, p1, p2);
    XT_PRINTF(" Func %s : %s\n", func, msgp12);
} /* end xt_err2 */


void    xt_errx(const char *func, const char *msg)
{


    XT_PRINTF(" Func %s : %s\n", func, msg);
    XT_PRINTF(" Exiting program\n");
    xt_exit(XT_ERROR);
} /* end xt_errx */


void    xt_errx1(const char *func, const char *msg, const char *p1)
{
    char    msgp1[256];

    sprintf(msgp1, msg, p1);
    XT_PRINTF(" Func %s : %s\n", func, msgp1);
    XT_PRINTF(" Exiting program\n");
    xt_exit(XT_ERROR);
} /* end xt_errx1 */


void    xt_errx2(const char *func, const char *msg, const char *p1, const char *p2)
{
    char    msgp12[256];

    sprintf(msgp12, msg, p1, p2);
    XT_PRINTF(" Func %s : %s\n", func, msgp12);
    XT_PRINTF(" Exiting program\n" );
    xt_exit(XT_ERROR);
} /* end xt_errx2 */


void    xt_perrx(const char *func, const char *msg)
{
    char    funcmsg[256];

    sprintf(funcmsg, "Func %s : %s", func, msg);
    perror(funcmsg);
    XT_PRINTF(" Exiting program\n");
    xt_exit(XT_ERROR);
} /* end xt_perrx */


void    xt_exit(OM_sint exitc)
{
    xt_dinit();             /* Close files.                      */
	fflush(0);
#ifdef THREADSAFE
	xt_thread_deinit();
#endif /* THREADSAFE */
    exit(exitc);
} /* end xt_exit */


/****************************************************************************/
/*                                                                          */
/*   Functions to record results of an automatic run in the Log File.       */
/*                                                                          */
/****************************************************************************/

void    xt_log_nocrlf(const char *msg)
{
    fprintf(xt_log_file, "%s: %s", xt_this_thread_id_and_name(), msg);
} /* end xt_log_nocrlf */


void    xt_log_cat(const char *msg)
{
    fprintf(xt_log_file, "%s", msg);
} /* end xt_log_cat */


void    xt_log_newline(void)
{
    fprintf(xt_log_file, "\n");
} /* end xt_log_newline*/


void    xt_log(const char *msg)
{
    fprintf(xt_log_file, "%s: %s\n", xt_this_thread_id_and_name(), msg);
} /* end xt_log */


void    xt_log1(const char *msg, const char *p1)
{
    char    msgp1[1024];

    sprintf(msgp1, msg, p1);
    fprintf(xt_log_file, "%s: %s\n", xt_this_thread_id_and_name(), msgp1);
} /* end xt_log1 */


void    xt_log2(const char *msg, const char *p1, const char *p2)
{
    char    msgp1[1024];

    sprintf(msgp1, msg, p1, p2);
    fprintf(xt_log_file, "%s: %s\n", xt_this_thread_id_and_name(), msgp1);
} /* end xt_log2 */


/* xt_log_both functions send output to stdout as well as file */

void    xt_log_both_nocrlf(const char *msg)
{
    fprintf(xt_log_file, "%s: %s", xt_this_thread_id_and_name(), msg);
    XT_PRINTF("%s", msg);
} /* end xt_log_both_nocrlf */


void    xt_log_both_cat(const char *msg)
{
    fprintf(xt_log_file, "%s", msg);
    XT_PRINTF("%s", msg);
} /* end xt_log_both_cat */


void    xt_log_both_newline(void)
{
    fprintf(xt_log_file, "\n");
    XT_PRINTF("\n");
} /* end xt_log_both_newline*/


void    xt_log_both(const char *msg)
{
    fprintf(xt_log_file, "%s: %s\n", xt_this_thread_id_and_name(), msg);
    XT_PRINTF("%s\n", msg);
} /* end xt_log_both */


void    xt_log_both1(const char *msg, const char *p1)
{
    char    msgp1[1024];

    sprintf(msgp1, msg, p1);
    fprintf(xt_log_file, "%s: %s\n", xt_this_thread_id_and_name(), msgp1);
    XT_PRINTF("%s\n", msgp1);
} /* end xt_log_both1 */


void    xt_log_both2(const char *msg, const char *p1, const char *p2)
{
    char    msgp1[1024];

    sprintf(msgp1, msg, p1, p2);
    fprintf(xt_log_file, "%s: %s\n", xt_this_thread_id_and_name(), msgp1);
    XT_PRINTF("%s\n", msgp1);
} /* end xt_log_both2 */
