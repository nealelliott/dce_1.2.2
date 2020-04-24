/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_run.c,v $
 * Revision 1.1.10.2  1996/02/18  00:24:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:12:13  marty]
 *
 * Revision 1.1.10.1  1995/12/08  22:14:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:21  root]
 * 
 * Revision 1.1.8.4  1994/07/06  15:09:49  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:28:18  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:33:27  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:24:00  marrek]
 * 
 * 	Use correct rcs id string.
 * 	[1994/05/02  15:03:56  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  16:06:48  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  18:13:54  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:16:17  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:32:12  marrek]
 * 
 * Revision 1.1.6.2  1993/08/12  12:37:26  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:30:21  marrek]
 * 
 * Revision 1.1.4.3  1993/01/05  20:34:50  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:49:29  htf]
 * 
 * Revision 1.1.4.2  1992/12/01  15:07:13  marrek
 * 	November 1992 code drop
 * 	[1992/12/01  13:22:04  marrek]
 * 
 * Revision 1.1.2.3  1992/07/01  16:54:26  melman
 * 	GDS drop of 6/26/92
 * 	[1992/07/01  16:24:34  melman]
 * 
 * Revision 1.1.2.2  1992/06/11  20:02:31  melman
 * 	 gds drop
 * 	[1992/06/11  19:24:05  melman]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: xt_run.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 00:24:52 $";
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
/* NAME         : xt_run.c                                            */
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
#include <string.h>
#include <errno.h>
#include <xt.h>
#ifdef THREADSAFE
#	include <xt_thread.h>
#else
#	include <xt_nonthread.h>
#endif
#include <sys/time.h>
#include  <xt_run.h>
#include  <xt_test.h> /* for xt_interrupt_* */


OM_IMPORT (DS_C_AVA)
OM_IMPORT (DS_A_COMMON_NAME)
OM_IMPORT (DS_A_COUNTRY_NAME)
OM_IMPORT (DS_A_LOCALITY_NAME)
OM_IMPORT (DS_A_ORG_NAME)
OM_IMPORT (DS_A_ORG_UNIT_NAME)
OM_IMPORT (DS_A_STATE_OR_PROV_NAME)
OM_IMPORT (DS_A_STREET_ADDRESS)
OM_IMPORT (DS_A_SURNAME)
OM_IMPORT (DS_C_DS_RDN)
OM_IMPORT (DS_C_DS_DN)
OM_IMPORT (DS_C_ATTRIBUTE_LIST)

extern char xt_iterations[];

#ifdef THREADSAFE
extern char xt_timeslice[];
extern void xt_thread_fail(void);
extern void xt_thread_pass(void);
extern void xt_initialize_thread_data(void);
extern void xt_print_thread_results(void);
extern void xt_reset_thread_results(void);
#else
extern void xt_initialize_global_data(void);
#endif /* THREADSAFE */
#define ONE_MILLION 1000000

/* static function declarations */
static OM_sint initialize_convenience_workspace(void);
static void xt_setup_interrupt_operation(OM_uint32 delay_us);
static OM_uint32 xt_time_for_one_run_us(void);
static void xt_run_threads(OM_uint32 interrupt_delay_us);
static void xt_run_threads_repeatable(OM_uint32 interrupt_delay_us);

/* static data declarations */
static OM_sint32 xt_run_time_us;

/****************************************************************************/
/*                                                                          */
/*   Entry point to the XDS/XOM tester                      */
/*   Command line parameters determine the kind of run.                     */
/*                                                                          */
/****************************************************************************/

int xt_main(int argc, char *argv[])
{
    OM_sint retc,  times ;
    BOOLEAN failed = FALSE;
    OM_sint xt_run(void);
	extern BOOLEAN xt_do_to_string_and_back;

    xt_runtype = xt_init(argc, argv);

#ifdef THREADSAFE
    if (xt_runtype == XT_RUNTYPE_THREAD ) {
        xt_thread_name_set( "main thread", 0);
    } /* endif */
#endif /* THREADSAFE */

    if (xt_openo() != XT_OK) {
        xt_errx("xt_main", "error opening log file");
    } /* endif */

	if (xt_do_to_string_and_back) {
		if (initialize_convenience_workspace() != XT_OK) {
			xt_errx("xt_main", "error initializing convenience workspace");
		} /* endif */
	} /* endif */

    switch (xt_runtype)  {
    case XT_RUNTYPE_ONE:
        times =  atoi(xt_iterations);
        while ((times > 0) && !failed) {
            if (xt_opent() != XT_OK) {
                xt_errx("xt_main", "error opening testcase file");
            } /* endif */
			if ( xt_interrupt_enabled) {
				xt_setup_interrupt_operation(atol(xt_interrupt_delay_string));
			} /* endif */
            if (xt_run() !=  XT_OK) {
                xt_log_fail (xt_testname);
                XT_PRINTF ("Testcase %s failed\n\n", xt_testname);
                xt_err("xt_main", "test FAILED");
            } else {
                xt_log_pass (xt_testname);
                XT_PRINTF ("Testcase %s passed\n\n", xt_testname);
            } /* endif */
            times--;
        } /* endwhile */
        if (xt_dinit() == XT_ERROR)  {
            xt_errx("xt_main", "error closing files");
        } /* endif */
        break;
    case XT_RUNTYPE_ALL:
        times =  atoi(xt_iterations);
        while ((times > 0) && !failed) {
            if (xt_opena() != XT_OK) {
                xt_errx("xt_main", "error opening file of testcase names");
            } /* endif */
#if THREADSAFE
			xt_initialize_thread_data();
#else /* THREADSAFE */
			xt_initialize_global_data();
#endif /* THREADSAFE */
            while ((retc = xt_rtest()) != XT_EOF) {
                if (retc !=  XT_OK) {
                    xt_errx("xt_main", "error reading testcase");
                } /* endif */
				if ( xt_interrupt_enabled) {
					xt_setup_interrupt_operation(atol(xt_interrupt_delay_string));
				} /* endif */
                if (xt_run() !=  XT_OK) {
                    xt_log_fail (xt_testname);
                    XT_PRINTF ("Testcase %s failed\n\n", xt_testname);
                    xt_err1("xt_main", "Testcase %s FAILED", xt_testname);
                } else {
                    xt_log_pass (xt_testname);
                    XT_PRINTF ("Testcase %s passed\n\n", xt_testname);
                } /* endif */
                if (xt_closet() != XT_OK) {
                    xt_errx("xt_main", "error closing testcase file");
                } /* endif */
                if ( xt_verbose) {
                    xt_check_string_tbl();
                } /* endif */
            } /* endwhile */
            times--;
            if (xt_closea() != XT_OK) {
                xt_errx("xt_main", "error closing file of testcase names");
            } /* endif */
        } /* endwhile */
        if (xt_dinit() == XT_ERROR)  {
            xt_errx("xt_main", "error closing files");
        } /* endif */
        break;
#ifdef THREADSAFE
    case XT_RUNTYPE_THREAD:
        times =  atoi(xt_iterations);
        if (strcmp(xt_timeslice, XT_DEF_TIMESLICE))  {
            OM_sint status;
            struct itimerval desired_new_interval, real_new_interval,
            old_interval;
            long    timeslice =  atol(xt_timeslice);


            if (timeslice < atol(XT_MIN_TIMESLICE))  {
                xt_err("timeslice must be at least (us)", XT_MIN_TIMESLICE);
                xt_err("timeslice set to minimum", XT_MIN_TIMESLICE);
            } else {
                if (timeslice > atol(XT_MAX_TIMESLICE))  {
                    xt_err("cannot exceed timeslice of (us) ", XT_MAX_TIMESLICE);
                    xt_err("timeslice set to maximum", XT_MAX_TIMESLICE);
                } else {

                    /* set up timeslice */
                    desired_new_interval.it_interval.tv_sec =  0;
                    desired_new_interval.it_interval.tv_usec =  timeslice;
                    desired_new_interval.it_value.tv_sec =  0;
                    desired_new_interval.it_value.tv_usec =  timeslice;
                    status = setitimer(ITIMER_VIRTUAL, &desired_new_interval, &old_interval);
                    if (status != 0) {
                        XT_PRINTF("in xt_main, setitimer bad status\n");
                        XT_PRINTF("COULD NOT SET TIMESLICE !!\n");
                    } else {
                        XT_PRINTF("old interval.it_interval.tv_sec was %i\n", old_interval.it_interval.tv_sec);
                        XT_PRINTF("old interval.it_interval.tv_usec was %i\n", old_interval.it_interval.tv_usec);
                        status = getitimer(ITIMER_VIRTUAL, &real_new_interval);
                        if (status != 0) {
                            XT_PRINTF("in xt_main," "getitimer bad status\n");
                            XT_PRINTF("COULD NOT SET TIMESLICE !!\n");
                        } else {
                            XT_PRINTF("new interval.it_interval.tv_sec was %i\n", real_new_interval.it_interval.tv_sec);
                            XT_PRINTF("new interval.it_interval.tv_usec was %i\n", real_new_interval.it_interval.tv_usec);
                            if (desired_new_interval.it_interval.tv_sec !=  real_new_interval.it_interval.tv_sec ||
                                desired_new_interval.it_interval.tv_usec !=  real_new_interval.it_interval.tv_usec) {
                                XT_PRINTF("NOTE: desired timeslice setting " "not achieved !!!\n");
                            } /* endif */
                        } /* endif */
                    } /* endif */
                } /* endif */
            } /* endif */
        } /* endif */
        while ((times > 0) && !failed) {
            OM_sint thread_number;
#if THREADSAFE
            xt_initialize_thread_data();
#else /* THREADSAFE */
            xt_initialize_global_data();
#endif /* THREADSAFE */
            if (xt_opena() != XT_OK) {
                xt_errx("xt_main", "error opening file of testcase names");
            } /* endif */

            /* run startup thread in context of main thread */
            if (xt_rtest_till_last() ==  XT_OK) {
                strcat(xt_testname, "(startup)");
                xt_thread_name_set( xt_testname, 0);
                if (xt_run() ==  XT_OK) {
                    xt_thread_pass();
                } else {
                    xt_thread_fail();
                } /* endif */
                xt_thread_name_set( "main thread", 0);
            } else {
                xt_errx("xt_main", "could not read enough testcases");
            } /* endif */

			if (xt_interrupt_automatic) {

				/* run with immediate cancel */
				xt_run_threads_repeatable(0);

				/* run with cancel in the middle */
				xt_run_threads_repeatable(xt_run_time_us*50/100);

			} /* endif */

			/* run with cancel at end - cancel will be produced only */
			/* if xt_interrupt_enabled is TRUE */
			xt_run_threads(xt_run_time_us);
			xt_print_thread_results();

            /* run shutdown thread in context of main thread */
            strcat(xt_testname, "(shutdown)");
            xt_thread_name_set( xt_testname, 0);
            if (xt_run() ==  XT_OK) {
                xt_thread_pass();
            } else {
                xt_thread_fail();
            } /* endif */
            xt_thread_name_set( "main thread", 0);

            times--;
            if (xt_closea() != XT_OK) {
                xt_errx("xt_main", "error closing file of testcase names");
            } /* endif */
            if ( xt_verbose) {
                xt_check_string_tbl();
            } /* endif */
        } /* endwhile */
        if (xt_dinit() == XT_ERROR)  {
            xt_errx("xt_main", "error closing files");
        } /* endwhile */
        break;
#endif /* THREADSAFE */
    default:
        xt_errx("xt_main", "bad retcode from xt_init");
        break;
    } /* endswitch */

	if (xt_do_to_string_and_back) {
		xoi_free();
	} /* endif */
#ifdef THREADSAFE
	xt_thread_deinit();
#endif /* THREADSAFE */
    return(XT_OK);
} /* end xt_main */


/****************************************************************************/
/*                                                                          */
/*   Main control loop for the automatic run.                               */
/*                                                                          */
/****************************************************************************/

OM_sint xt_run(void)
{
	static const char function_name[] = "xt_run";
    struct jump_struct *ptr;
    OM_sint retc1;
    OM_sint retc = XT_OK;
    OM_sint f_retc;
    char    include[64];
    BOOLEAN test_failed = FALSE;

    /* while more entries exist  */
    while ((retc1 =  xt_fnext()) != XT_ERROR) {  /* in the testcase file */

        if (retc1 == XT_FUNCTION) {
            if ((ptr = xt_sbyname(xt_entry)) ==  (struct jump_struct *)NULL)  {
                xt_err1 ("xt_run", "unknown entry, %s", xt_entry);
                xt_log1("Unknown Entry %s\n", xt_entry);
                test_failed = TRUE;
                retc = XT_ERROR;
                break;
            } /* endif */
            if (xt_log_func (ptr->entname) != XT_OK) {
                xt_err ("xt_run", "Error writing to log file");
                xt_log ("Error writing to log file");
                test_failed = TRUE;
                retc = XT_ERROR;
                break;
            } /* endif */
            if ((f_retc = (*ptr->test_func)()) == XT_OK) {
                XT_PRINTF ("%s returned OK\n", ptr->entname);
            } else {
                if (f_retc == XT_NOT_CALLED) {
                    XT_PRINTF ("%s not called\n", ptr->entname);
                    xt_log1("\t%s not called\n", ptr->entname);
                } else {
                    xt_err("xt_run", "Function call FAILED");
                    XT_PRINTF ("%s returned ERROR\n", ptr->entname);
                    xt_log1("\t%s returned ERROR\n", ptr->entname);
                } /* endif */
                test_failed = TRUE;
                retc = XT_ERROR;
#if FALSE /* CC 13.4.94 to allow delete tree to execute */
                break;
#endif /* FALSE */
            } /* endif */
        } else if (retc1 == XT_INCLUDE) {
			FILE * save_fp = xt_testcase_file;
			if (xt_openp(xt_entry) != XT_OK) {
				xt_err("xt_run", "error opening include file");
				xt_log1("Error opening include file %s\n", include);
				test_failed = TRUE;
				retc = XT_ERROR;
				break;
			} /* endif */
			strcpy(include, xt_entry);
			xt_log1("BEGIN Include %s\n", include);
			XT_PRINTF ("\n");
			XT_PRINTF ("Opening Include file %s\n", include);
			if (xt_run() !=  XT_OK) {
				xt_log1("Error occured in Include %s\n", include);
#if FALSE /* include can fail without test failing */
				xt_log_fail (xt_testname);
				XT_PRINTF("Testcase %s failed\n\n", xt_testname);
				xt_err("xt_run", "test failed");
				test_failed = TRUE;
				retc = XT_ERROR;
				break;
#endif /* FALSE */ 
			} /* endif */
			if (xt_closep() != XT_OK) {
				xt_err("xt_run", "error closing include file");
				xt_log1("Error closing include file %s\n", include);
				test_failed = TRUE;
				retc = XT_ERROR;
				break;
			} /* endif */
			xt_log1("END Include %s\n\n", include);
			XT_PRINTF ("Exiting Include file %s\n\n", include);
			xt_testcase_file = save_fp;
        } /* endif */
    } /* endwhile */
    return retc;
} /* end xt_run */

static OM_sint 
/****************************************************************************/
		xt_run_repeatable(void)		/* saves file state for repeating */
/****************************************************************************/
{
	int retc;
	fpos_t position, position_list;
	extern FILE *xt_testcase_list_file;

	retc = fgetpos(xt_testcase_file,&position);
#if FALSE /* CC 18.4.94 */
	if ( retc == 0) {
		retc = fgetpos(xt_testcase_list_file,&position_list);
	} else {
		goto end;
	} /* endif */
#endif /* FALSE */
	xt_run();
	if ( retc == 0) {
		retc = fsetpos(xt_testcase_file,&position);
	} else {
		goto end;
	} /* endif */
#if FALSE /* CC 18.4.94 */
	if ( retc == 0) {
		retc = fsetpos(xt_testcase_list_file,&position_list);
	} else {
		goto end;
	} /* endif */
#endif /* FALSE */
end:
	return (retc);
} /* end xt_run_repeatable */

/****************************************************************************/
/*   jmptbl - when a function name is read from the testcase file, it is    */
/*            matched against the entname field of this structure, and      */
/*            test_func is invoked                                          */
/****************************************************************************/

struct jump_struct jmptbl[] = {
    { "COPY",              XT_COPY,    xt_om_copy           },
    { "COPY_VALUE",        XT_CPYVAL,  xt_om_copy_value     },
    { "CREATE",            XT_CREATE,  xt_om_create         },
    { "DECODE",            XT_DECODE,  xt_om_decode         },
    { "DELETE",            XT_DELETE,  xt_om_delete         },
    { "ENCODE",            XT_ENCODE,  xt_om_encode         },
    { "GET",               XT_GET,     xt_om_get            },
    { "INSTANCE",          XT_INSTAN,  xt_om_instance       },
    { "PUT",               XT_PUT,     xt_om_put            },
    { "OM_READ",           XT_OMREAD,  xt_om_read           },
    { "REMOVE",            XT_REMOVE,  xt_om_remove         },
    { "WRITE",             XT_WRITE,   xt_om_write          },
    { "ABANDON",           XT_ABANDN,  xt_ds_abandon        },
    { "ADD_ENTRY",         XT_ADDENT,  xt_ds_add_entry      },
    { "BIND",              XT_BIND,    xt_ds_bind           },
    { "COMPARE",           XT_COMPAR,  xt_ds_compare        },
    { "INITIALIZE",        XT_INIT,    xt_ds_initialize     },
    { "LIST",              XT_LIST,    xt_ds_list           },
    { "MODIFY_ENTRY",      XT_MODENT,  xt_ds_modify_entry   },
    { "MODIFY_RDN",        XT_MODRDN,  xt_ds_modify_rdn     },
    { "READ",              XT_DSREAD,  xt_ds_read           },
    { "RECEIVE_RESULT",    XT_RECRES,  xt_ds_receive_result },
    { "REMOVE_ENTRY",      XT_RMENT,   xt_ds_remove_entry   },
    { "SEARCH",            XT_SEARCH,  xt_ds_search         },
    { "SHUTDOWN",          XT_SHUTDN,  xt_ds_shutdown       },
    { "UNBIND",            XT_UNBIND,  xt_ds_unbind         },
    { "VERSION",           XT_VERSN,   xt_ds_version        },
    { "BULK_LOAD",         XT_BLDTREE, xt_build_tree        },
    { "BULK_UNLOAD",       XT_RMTREE,  xt_remove_tree       },
    { "FILL",              XT_FILL,    xt_omX_fill          },
    { "FILL_OID",          XT_FILLO,   xt_omX_fill_oid      },
    { "EXTRACT",           XT_EXTRACT, xt_omX_extract       },
    { "EXTRACT_ATTR_VALUES",XT_EXTRACTA, xt_dsX_extract_attr_values },
    { "STRING_TO_OBJECT",  XT_S_TO_O,  xt_omX_string_to_object  },
    { "OBJECT_TO_STRING",  XT_O_TO_S,  xt_omX_object_to_string  },
    { "" }
};









/****************************************************************************/
/*                                                                          */
/*   Search by entry name in the function table                             */
/*                                                                          */
/****************************************************************************/


struct jump_struct *xt_sbyname(char *name)  /* Name of entry to find.*/
{
    OM_sint i =  0,
    gotit =  0;

    while (strcmp(jmptbl[i].entname, "")) {
        if (strcmp(name, jmptbl[i].entname) == 0) {
            gotit++;
            break;
        } /* endif */
        i++;
    } /* endwhile */

    if (gotit) {
        return(&jmptbl[i]);
    } else {
        return((struct jump_struct *) NULL);
    } /* endif */
} /* end xt_sbyname */


/****************************************************************************/
/*                                                                          */
/*          xt_build_ava()                  */
/*                                                                          */
/****************************************************************************/

OM_sint xt_build_ava(OM_object ava, char value[], OM_sint num)
{

    OM_sint retc = XT_ERROR;

    /* build an ava */

    ava[0].type   =  OM_CLASS;
    ava[0].syntax =  OM_S_OBJECT_IDENTIFIER_STRING;
    ava[0].value.string.length  =  OMP_LENGTH(DS_C_AVA);
    ava[0].value.string.elements =  OMP_D_DS_C_AVA;

    ava[1].type   =  DS_ATTRIBUTE_TYPE;
    ava[1].syntax =  OM_S_OBJECT_IDENTIFIER_STRING;

    ava[2].type   =  DS_ATTRIBUTE_VALUES;
    ava[2].value.string.length       =  strlen (value);

    if (ava[2].value.string.elements) {
        free (ava[2].value.string.elements);
    } /* endif */
    if ((ava[2].value.string.elements =   (char *)malloc(strlen(value) +  1)) == NULL) {
        xt_err("xt_build_ava", "Error allocating ava[2].value.string.elements");
        return XT_ERROR;
    } /* endif */
    strcpy (ava[2].value.string.elements, value);

    ava[3].type   =  OM_NO_MORE_TYPES;
    ava[3].syntax =  OM_S_NO_MORE_SYNTAXES;
    ava[3].value.string.length  =  0;
    ava[3].value.string.elements =  0;

    if (num == OMP_O_DS_A_COMMON_NAME[2]) {
#ifdef DEBUG
        XT_PRINTF ("OMP_O_DS_A_COMMON_NAME\n");
#endif /* DEBUG */
        ava[1].value.string.length  = OMP_LENGTH(DS_A_COMMON_NAME);
        ava[1].value.string.elements = OMP_D_DS_A_COMMON_NAME;
        ava[2].syntax               = OM_S_TELETEX_STRING;
        retc =  XT_OK;
    } /* endif */
    if (num == OMP_O_DS_A_COUNTRY_NAME[2]) {
#ifdef DEBUG
        XT_PRINTF ("OMP_O_DS_A_COUNTRY_NAME\n");
#endif /* DEBUG */
        ava[1].value.string.length  = OMP_LENGTH(DS_A_COUNTRY_NAME);
        ava[1].value.string.elements = OMP_D_DS_A_COUNTRY_NAME;
        ava[2].syntax               = OM_S_PRINTABLE_STRING;
        retc =  XT_OK;
    } /* endif */
    if (num == OMP_O_DS_A_LOCALITY_NAME[2]) {
#ifdef DEBUG
        XT_PRINTF ("OMP_O_DS_A_LOCALITY_NAME\n");
#endif /* DEBUG */
        ava[1].value.string.length  = OMP_LENGTH(DS_A_LOCALITY_NAME);
        ava[1].value.string.elements = OMP_D_DS_A_LOCALITY_NAME;
        ava[2].syntax               = OM_S_TELETEX_STRING;
        retc =  XT_OK;
    } /* endif */
    if (num == OMP_O_DS_A_ORG_NAME[2]) {
#ifdef DEBUG
        XT_PRINTF ("OMP_O_DS_A_ORG_NAME\n");
#endif /* DEBUG */
        ava[1].value.string.length  = OMP_LENGTH(DS_A_ORG_NAME);
        ava[1].value.string.elements = OMP_D_DS_A_ORG_NAME;
        ava[2].syntax               = OM_S_TELETEX_STRING;
        retc =  XT_OK;
    } /* endif */
    if (num == OMP_O_DS_A_ORG_UNIT_NAME[2]) {
#ifdef DEBUG
        XT_PRINTF ("OMP_O_DS_A_ORG_UNIT_NAME\n");
#endif /* DEBUG */
        ava[1].value.string.length  = OMP_LENGTH(DS_A_ORG_UNIT_NAME);
        ava[1].value.string.elements = OMP_D_DS_A_ORG_UNIT_NAME;
        ava[2].syntax               = OM_S_TELETEX_STRING;
        retc =  XT_OK;
    } /* endif */
    if (num == OMP_O_DS_A_STATE_OR_PROV_NAME[2]) {
#ifdef DEBUG
        XT_PRINTF ("OMP_O_DS_A_STATE_OR_PROV_NAME\n");
#endif /* DEBUG */
        ava[1].value.string.length  = OMP_LENGTH(DS_A_STATE_OR_PROV_NAME);
        ava[1].value.string.elements = OMP_D_DS_A_STATE_OR_PROV_NAME;
        ava[2].syntax               = OM_S_TELETEX_STRING;
        retc =  XT_OK;
    } /* endif */
    if (num == OMP_O_DS_A_STREET_ADDRESS[2]) {
#ifdef DEBUG
        XT_PRINTF ("OMP_O_DS_A_STREET_ADDRESS\n");
#endif /* DEBUG */
        ava[1].value.string.length  = OMP_LENGTH(DS_A_STREET_ADDRESS);
        ava[1].value.string.elements = OMP_D_DS_A_STREET_ADDRESS;
        ava[2].syntax               = OM_S_TELETEX_STRING;
        retc =  XT_OK;
    } /* endif */
    if (num == OMP_O_DS_A_SURNAME[2]) {
#ifdef DEBUG
        XT_PRINTF ("OMP_O_DS_A_SURNAME\n");
#endif /* DEBUG */
        ava[1].value.string.length  = OMP_LENGTH(DS_A_SURNAME);
        ava[1].value.string.elements = OMP_D_DS_A_SURNAME;
        ava[2].syntax               = OM_S_TELETEX_STRING;
        retc =  XT_OK;
    } /* endif */

    if (retc !=  XT_OK) {
        xt_err("xt_build_ava", "Descriptor not recognised");
    } /* endif */

    return retc;
} /* end xt_build_ava */

OM_sint 
/****************************************************************************/
xt_build_rdn(OM_object rdn, OM_object ava)
/****************************************************************************/
{

    /* build an rdn */

    rdn[0].type         = OM_CLASS;
    rdn[0].syntax       = OM_S_OBJECT_IDENTIFIER_STRING;
    rdn[0].value.string.length  = OMP_LENGTH(DS_C_DS_RDN);
    rdn[0].value.string.elements = OMP_D_DS_C_DS_RDN;

    rdn[1].type         = DS_AVAS;
    rdn[1].syntax       = OM_S_OBJECT;
    rdn[1].value.object.padding = 0;
    rdn[1].value.object.object  = ava;

    rdn[2].type         = OM_NO_MORE_TYPES;
    rdn[2].syntax       = OM_S_NO_MORE_SYNTAXES;
    rdn[2].value.string.length  = 0;
    rdn[2].value.string.elements = 0;

    return XT_OK;
} /* end xt_build_rdn */


OM_sint
/****************************************************************************/
xt_build_name(OM_descriptor name[], OM_object rdns[], OM_sint count, OM_sint first)
/****************************************************************************/
{
    OM_sint j;

    /* build name */
    name[0].type                 =   OM_CLASS;
    name[0].syntax               =   OM_S_OBJECT_IDENTIFIER_STRING;
    name[0].value.string.length      =   OMP_LENGTH(DS_C_DS_DN);
    name[0].value.string.elements    =   OMP_D_DS_C_DS_DN;

    for (j = 1; j <= (count  - first); j++) {
        if (j > (XT_DSANAME_MAX  - 2)) {
            xt_err("xt_build_name", "DSA name too long");
            return XT_ERROR;
        } /* endif */
        name[j].type                 =   DS_RDNS;
        name[j].syntax               =   OM_S_OBJECT;
        name[j].value.object.padding =   0;
        name[j].value.object.object  =   rdns[first+j-1];
    } /* endfor */

    name[j].type                     =   OM_NO_MORE_TYPES;
    name[j].syntax                   =   OM_S_NO_MORE_SYNTAXES;
    name[j].value.string.length      =   0;
    name[j].value.string.elements    =   0;

    return XT_OK;
} /* end xt_build_name */



/****************************************************************************/
	OM_sint xt_build_attribute_list(OM_object att_list, char *type)
/****************************************************************************/
{
    /* build att_list */
    att_list[0].type                 =   OM_CLASS;
    att_list[0].syntax               =   OM_S_OBJECT_IDENTIFIER_STRING;
    att_list[0].value.string.length  =   OMP_LENGTH(DS_C_ATTRIBUTE_LIST);
    att_list[0].value.string.elements =   OMP_D_DS_C_ATTRIBUTE_LIST;

    att_list[1].type                 =   OM_NO_MORE_TYPES;
    att_list[1].syntax               =   OM_S_NO_MORE_SYNTAXES;
    att_list[1].value.string.length  =   0;
    att_list[1].value.string.elements =   0;

    return XT_OK;
} /* end xt_build_attribute_list */

static OM_sint
/****************************************************************************/
		initialize_convenience_workspace(void)
/****************************************************************************/
{
	extern OM_workspace convenience_workspace;

    if ((convenience_workspace = ds_initialize()) == (OM_workspace) NULL) {
        xt_errx("xt_init", "ds_initialize of convenience workspace failed");
	} else {
		/* const  should be here, causes warnings until I/F changed */
			DS_feature packages[] = {
			{ OM_STRING(OMP_O_DS_BASIC_DIR_CONTENTS_PKG), OM_TRUE },
			{ OM_STRING(OMP_O_DS_MHS_DIR_USER_PKG), OM_TRUE },
			{ OM_STRING(OMP_O_DSX_GDS_PKG), OM_TRUE },
			{ {(OM_uint32) 0, NULL}, OM_FALSE } };


		if ( ds_version(packages, convenience_workspace) != DS_SUCCESS) {
			xt_errx("xt_init", "ds_version of convenience workspace failed");
		} /* endif */
    } /* endif */

	return (XT_OK);
} /* end initialize_convenience_workspace */

static void
/****************************************************************************/
				xt_set_alarm(
/****************************************************************************/
	long	delay)
{
	OM_sint status;
	struct itimerval desired_new_delay, real_new_delay, old_delay;
	if (delay < atol(XT_MIN_DELAY))  {
		xt_err("delay must be at least (us)", XT_MIN_DELAY);
		xt_err("delay set to minimum", XT_MIN_DELAY);
	} else if (delay > atol(XT_MAX_DELAY))  {
			xt_err("cannot exceed delay of (us) ", XT_MAX_DELAY);
			xt_err("delay set to maximum", XT_MAX_DELAY);
	} else {

		/* set up delay */
		desired_new_delay.it_interval.tv_sec =  0;
		desired_new_delay.it_interval.tv_usec =  delay;
		desired_new_delay.it_value.tv_sec =  0;
		desired_new_delay.it_value.tv_usec =  delay;
		status = setitimer(ITIMER_REAL, &desired_new_delay, &old_delay);
		if (status != 0) {
			XT_PRINTF("in xt_set_alarm, setitimer bad status\n");
			XT_PRINTF("COULD NOT SET DELAY !!\n");
		} else {
			XT_PRINTF("old delay.it_interval.tv_sec was %i\n", old_delay.it_interval.tv_sec);
			XT_PRINTF("old delay.it_interval.tv_usec was %i\n", old_delay.it_interval.tv_usec);
			status = getitimer(ITIMER_VIRTUAL, &real_new_delay);
			if (status != 0) {
				XT_PRINTF("in xt_set_alarm," "getitimer bad status\n");
				XT_PRINTF("COULD NOT SET DELAY !!\n");
			} else {
				XT_PRINTF("new delay.it_interval.tv_sec was %i\n", real_new_delay.it_interval.tv_sec);
				XT_PRINTF("new delay.it_interval.tv_usec was %i\n", real_new_delay.it_interval.tv_usec);
				if (desired_new_delay.it_interval.tv_sec !=  real_new_delay.it_interval.tv_sec ||
					desired_new_delay.it_interval.tv_usec !=  real_new_delay.it_interval.tv_usec) {
					XT_PRINTF("NOTE: desired delay setting " "not achieved !!!\n");
				} /* endif */
			} /* endif */
		} /* endif */
	} /* endif */
} /* end xt_set_alarm */

static void
/*************************************************************/
xt_setup_interrupt_operation(OM_uint32 delay_us)
/*************************************************************/
{
    static const char function_name[] = "xt_setup_interrupt_operation";

#ifdef THREADSAFE
	unsigned long seconds = delay_us/ONE_MILLION;
	long nanoseconds = (delay_us-ONE_MILLION*seconds)*ONE_MILLION;
	struct timespec delay_struct;

	delay_struct.tv_sec =  seconds;
	delay_struct.tv_nsec = nanoseconds;
	if ( xt_verbose) {
		XT_PRINTF("%s,about to call pthread_delay_np(seconds=>%ld, nanosecconds=>%ld\n",function_name,seconds,nanoseconds);
	} /* endif */
	pthread_delay_np(&delay_struct);
	if ( xt_verbose) {
		XT_PRINTF("%s, about to call xt_threads_cancel\n",function_name);
	} /* endif */
	xt_threads_cancel();
#else /* THREADSAFE */
	xt_set_alarm(atol(xt_interrupt_delay_string));
#endif /* THREADSAFE */
} /* end xt_setup_interrupt_operation */


static OM_uint32
/*************************************************************/
			xt_time_for_one_run_us(void)
/*************************************************************/
{
	time_t elapsed_time = 0;
	OM_uint32 iterations, kept_iterations;
	#define MIN_DURATION_SECONDS 10

	for (iterations = 1;elapsed_time < MIN_DURATION_SECONDS; iterations *= 5) {
		time_t the_time = time(0);
		int i;
		for (i = kept_iterations = iterations; i > 0; --i) {
			if (xt_run_repeatable() != 0) {
				xt_errx("xt_time_for_one_run_us", "xt_run_repeatable failed");
			} /* endif */
		} /* endfor */
		elapsed_time = time(0)-the_time;
	} /* endfor */

	return ((elapsed_time * ONE_MILLION)/kept_iterations); /* microseconds */
} /* end xt_time_for_one_run_us */


static void
/*************************************************************/
			xt_run_threads(OM_uint32 interrupt_delay_us)
/*************************************************************/
{
    static const char function_name[] = "xt_run_threads";
	static OM_boolean timing_done = FALSE;
	OM_sint thread_number, retc;

	if (xt_verbose) {
		printf("%s, interrupt_delay_us => %ld\n",function_name,
		interrupt_delay_us);
	} /* endif */

	/* create other threads */
	for (thread_number = 1; (retc =  xt_rtest_till_last()) !=  XT_EOF; thread_number++) {
		if (retc !=  XT_OK) {
			xt_errx(function_name, "error reading testcase");
		} /* endif */
		/* */
		if ( xt_interrupt_automatic && !timing_done) {
			xt_run_time_us = xt_time_for_one_run_us();
			XT_PRINTF("********************************\n");
			XT_PRINTF("run time was %ld microseconds\n",xt_run_time_us);
			XT_PRINTF("********************************\n");
			timing_done = TRUE;
			/* clear spurious results from timing tests */
			xt_reset_thread_results();
		} /* endif */

		if (xt_thread_create(thread_number, xt_run, xt_testname) != XT_OK)  {
			xt_log_fail (xt_testname);
			XT_PRINTF ("thread create for Testcase %s FAILED\n\n", xt_testname);
			xt_err1("%s, thread create for Testcase %s FAILED",function_name, xt_testname);
			XT_PRINTF("this would have been thread number %d\n", xt_threads_created());
			xt_errx(function_name,"thread create FAILURE");
		} /* endif */
	} /* endfor */

	/* release threads from wait on condition variable  */
	if ( xt_verbose) {
		XT_PRINTF("%s, about to call xt_threads_start()\n",function_name);
	} /* endif */
	xt_threads_start();

	if ( xt_interrupt_enabled) {
		if ( xt_verbose) {
			XT_PRINTF("%s,about to call xt_setup_interrupt_operation( interrupt_delay_us => %ld)\n",function_name,interrupt_delay_us);
		} /* endif */
		xt_setup_interrupt_operation(interrupt_delay_us);
	} /* endif */

	/* wait for threads to terminate */
	if ( xt_verbose) {
		XT_PRINTF("%s, about to call xt_threads_join\n",function_name);
	} /* endif */
	xt_threads_join();
} /* end xt_run_threads */

static void
/*************************************************************/
			xt_run_threads_repeatable(OM_uint32 interrupt_delay_us)
/*************************************************************/
{
	fpos_t position, position_list;
	extern FILE *xt_testcase_list_file;
	int retc;

	retc = fgetpos(xt_testcase_file,&position);
	if ( retc == 0) {
		retc = fgetpos(xt_testcase_list_file,&position_list);
	} else {
		goto end;
	} /* endif */
	xt_run_threads(interrupt_delay_us);
	if ( retc == 0) {
		retc = fsetpos(xt_testcase_file,&position);
	} else {
		goto end;
	} /* endif */
	if ( retc == 0) {
		retc = fsetpos(xt_testcase_list_file,&position_list);
	} else {
		goto end;
	} /* endif */
end:
	;
} /* end xt_run_threads_repeatable */
