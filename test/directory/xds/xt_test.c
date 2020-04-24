/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * HISTORY
 * $Log: xt_test.c,v $
 * Revision 1.1.11.2  1996/02/18  00:24:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:12:16  marty]
 *
 * Revision 1.1.11.1  1995/12/08  22:14:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:46:25  root]
 * 
 * Revision 1.1.9.5  1994/08/10  08:26:07  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:18:22  marrek]
 * 
 * Revision 1.1.9.4  1994/06/21  14:29:03  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:49:20  marrek]
 * 
 * Revision 1.1.9.3  1994/05/10  15:33:29  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/06  10:24:06  marrek]
 * 
 * 	Use correct rcs id string.
 * 	[1994/05/02  15:04:00  marrek]
 * 
 * Revision 1.1.9.2  1994/03/23  16:06:49  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  18:13:58  keutel]
 * 
 * Revision 1.1.9.1  1994/02/22  19:16:21  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:32:23  marrek]
 * 
 * Revision 1.1.6.2  1993/08/12  12:38:29  marrek
 * 	July 1993 code drop.
 * 	[1993/08/03  12:30:37  marrek]
 * 
 * Revision 1.1.4.2  1993/01/05  20:35:00  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:49:36  htf]
 * 
 * Revision 1.1.2.2  1992/06/11  20:02:41  melman
 * 	 gds drop
 * 	[1992/06/11  19:24:11  melman]
 * 
 * $EndLog$
 */
#if !defined lint && defined GDS_RCS_ID
static char rcsid[] = "@(#)$RCSfile: xt_test.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 00:24:55 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

#ifdef THREADSAFE
#	include <pthread.h>
#	include <xt_thread.h>
#else
#	include <xt_nonthread.h>
#endif
#include <locale.h>
#include <xt_test.h>
#include <dce/dce.h>	/* for dcelocal_path */

extern char xt_ofname[];
extern char xt_afname[];
extern FILE *xt_log_file;

char    xt_iterations[4];   /* no of times tests are run */
char    xt_timeslice[64];   /* timeslice in microsec for threads scheduler */
char    xt_interrupt_delay_string[64];/* delay before cancel/signal in us */
OM_boolean	xt_interrupt_enabled = FALSE;
OM_boolean	xt_interrupt_automatic = FALSE;


/* run type, i.e. single testcase, multiple serial or multiple parallel */
OM_sint xt_runtype = XT_RUNTYPE_NONE;

/* command line flag for debugging information */
BOOLEAN xt_verbose = FALSE;

/* command line flag for testing convenience library: if on, all
objects are converted to strings and back to objects */
BOOLEAN xt_do_to_string_and_back = FALSE;

/* flag to indicate whether failure of xt_to_string_and_back
should cause failure of test. When it is TRUE, then the test
will fail on failure of xt_to_string_and_back */
BOOLEAN xt_strict_to_string_and_back = TRUE;

OM_sint xt_num_failures = 0;

int
main ( int argc, char *argv[])
{

	setlocale(LC_ALL, "");

#ifdef THREADSAFE
    xt_thread_init();
    printf("Directory tester: threads version %s [%s  %s]\n", XT_VERSION, __DATE__, __TIME__);
#else
    xt_nonthread_init();
    printf("Directory tester: version %s [%s  %s]\n", XT_VERSION, __DATE__, __TIME__);
#endif /* THREADSAFE */

	if (!(argc == 2 && argv[1][1] == '0')) {
		OM_sint counter;
		printf("       arguments: ");
		for ( counter = 1; counter < argc; counter++) {
			printf(" %s ", argv[counter]);
		} /* endfor */
		printf("\n");
	} /* endif */

#ifdef THREADSAFE
	TRY
#endif /*   THREADSAFE */

    xt_main(argc, argv);


#if USE_GUARDS
    {
        extern void check_guards(void);
        check_guards();
    }
#endif


#ifdef THREADSAFE
	CATCH (exc_uninitexc_e)
		XT_PRINTF("in main, exception \'exc_uninitexc_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_illaddr_e)
		XT_PRINTF("in main, exception \'exc_illaddr_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_exquota_e)
		XT_PRINTF("in main, exception \'exc_exquota_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_insfmem_e)
		XT_PRINTF("in main, exception \'exc_insfmem_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_nopriv_e)
		XT_PRINTF("in main, exception \'exc_nopriv_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_illinstr_e)
		XT_PRINTF("in main, exception \'exc_illinstr_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_resaddr_e)
		XT_PRINTF("in main, exception \'exc_resaddr_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_privinst_e)
		XT_PRINTF("in main, exception \'exc_privinst_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_resoper_e)
		XT_PRINTF("in main, exception \'exc_resoper_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_SIGTRAP_e)
		XT_PRINTF("in main, exception \'exc_SIGTRAP_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_SIGIOT_e)
		XT_PRINTF("in main, exception \'exc_SIGIOT_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_SIGEMT_e)
		XT_PRINTF("in main, exception \'exc_SIGEMT_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_aritherr_e)
		XT_PRINTF("in main, exception \'exc_aritherr_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_SIGSYS_e)
		XT_PRINTF("in main, exception \'exc_SIGSYS_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_SIGPIPE_e)
		XT_PRINTF("in main, exception \'exc_SIGPIPE_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_excpu_e)
		XT_PRINTF("in main, exception \'exc_excpu_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_exfilsiz_e)
		XT_PRINTF("in main, exception \'exc_exfilsiz_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_intovf_e)
		XT_PRINTF("in main, exception \'exc_intovf_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_intdiv_e)
		XT_PRINTF("in main, exception \'exc_intdiv_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_fltovf_e)
		XT_PRINTF("in main, exception \'exc_fltovf_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_fltdiv_e)
		XT_PRINTF("in main, exception \'exc_fltdiv_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_fltund_e)
		XT_PRINTF("in main, exception \'exc_fltund_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_decovf_e)
		XT_PRINTF("in main, exception \'exc_decovf_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_subrng_e)
		XT_PRINTF("in main, exception \'exc_subrng_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_cancel_e)
		XT_PRINTF("in main, exception \'pthread_cancel_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_assertion_e)
		XT_PRINTF("in main, exception \'pthread_assertion_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_badparam_e)
		XT_PRINTF("in main, exception \'pthread_badparam_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_bugcheck_e)
		XT_PRINTF("in main, exception \'pthread_bugcheck_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_exit_thread_e)
		XT_PRINTF("in main, exception \'pthread_exit_thread_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_existence_e)
		XT_PRINTF("in main, exception \'pthread_existence_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_in_use_e)
		XT_PRINTF("in main, exception \'pthread_in_use_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_use_error_e)
		XT_PRINTF("in main, exception \'pthread_use_error_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_wrongmutex_e)
		XT_PRINTF("in main, exception \'pthread_wrongmutex_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_stackovf_e)
		XT_PRINTF("in main, exception \'pthread_stackovf_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_nostackmem_e)
		XT_PRINTF("in main, exception \'pthread_nostackmem_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_notstack_e)
		XT_PRINTF("in main, exception \'pthread_notstack_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_unimp_e)
		XT_PRINTF("in main, exception \'pthread_unimp_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_inialrpro_e)
		XT_PRINTF("in main, exception \'pthread_inialrpro_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_defer_q_full_e)
		XT_PRINTF("in main, exception \'pthread_defer_q_full_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (pthread_signal_q_full_e)
		XT_PRINTF("in main, exception \'pthread_signal_q_full_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_accvio_e)
		XT_PRINTF("in main, exception \'exc_accvio_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_SIGILL_e)
		XT_PRINTF("in main, exception \'exc_SIGILL_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_SIGFPE_e)
		XT_PRINTF("in main, exception \'exc_SIGFPE_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_SIGBUS_e)
		XT_PRINTF("in main, exception \'exc_SIGBUS_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_SIGSEGV_e)
		XT_PRINTF("in main, exception \'exc_SIGSEGV_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_SIGXCPU_e)
		XT_PRINTF("in main, exception \'exc_SIGXCPU_e\' raised\n");
		fflush(NULL);
		RERAISE;
	CATCH (exc_SIGXFSZ_e)
		XT_PRINTF(" in main, exception \'exc_SIGXFSZ_e\' raised\n");
		fflush(NULL);
		RERAISE;
    ENDTRY
		fflush (NULL);

#endif /* THREADSAFE */

    return (xt_num_failures);

} /* end main */


/****************************************************************************/
/*                                                                          */
/*   Decode the command line to see if an auto run and to get filenames:    */
/*  -a <file_of_testnames>  :   run through all the tests serially     */
/*                  if <file_of_testnames> is provided then */
/*                  the tests in that file are executed     */
/*                  if not, the default is "Testcases"      */
/*  -t <file_of_testnames>  :   run through all the tests in parallel*/
/*                  if <file_of_testnames> is provided then */
/*                  the tests in that file are executed     */
/*                  if not, the default is "Testcases"      */
/*  -i testcase_fname   :   testcase file               */
/*  -o fname        :   output logfile name which overrides     */
/*                  the default             */
/*                                                                          */
/****************************************************************************/

OM_sint xt_init(OM_sint argc, char *argv[])
{
    OM_sint retc = XT_OK, str_size, i;

    /* Initializations */
    xt_dump_objects = FALSE;
    xt_dump_objects_private = FALSE;
    (void) strcpy(xt_ofname, XT_DEF_LOG_FNAME);
    (void) strcpy(xt_iterations, XT_DEF_NO_ITERATIONS);
    (void) strcpy(xt_timeslice, XT_DEF_TIMESLICE);
    (void) strcpy(xt_afname, XT_DEF_ALL_FNAME);

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            switch (argv[i][1])  {
            case 'a':                   /* Can be -a only    */
                if (xt_runtype != XT_RUNTYPE_ALL &&      xt_runtype !=  XT_RUNTYPE_NONE)
                    xt_usage(argv[0]);
                else
                    xt_runtype = XT_RUNTYPE_ALL;
                if (i + 1 < argc)
                    if (*argv[i+1] != '-') {
                        str_size = strlen(argv[i+1]);
                        (void)strncpy(xt_afname, argv[i+1], str_size);
                        xt_afname[str_size] = '\0';
                    } /* endif */
                break;
            case 'c':
                xt_do_to_string_and_back = TRUE;
				xt_strict_to_string_and_back = TRUE;
                break;
            case 'i':                   /* Must be -i fname  */
                if (xt_runtype != XT_RUNTYPE_ONE &&      xt_runtype !=  XT_RUNTYPE_NONE) {
                    xt_usage(argv[0]);
                } else {
                    xt_runtype = XT_RUNTYPE_ONE;
                    if (i + 1 < argc)
                        if (*argv[i+1] != '-') {
                            str_size = strlen(argv[i+1]);
                            if (str_size > XT_LEN_TESTNAME)  {
                                xt_err("xt_init", "testname too long");
                                exit(XT_ERROR);
                            } else {
                                (void)strncpy(xt_testname, argv[i+1], str_size);
                                xt_testname[str_size] =  '\0';
                            } /* endif */
                        } /* endif */
                } /* endif */
                break;
#ifdef THREADSAFE
            case 't':
                if (xt_runtype != XT_RUNTYPE_THREAD  &&  xt_runtype !=  XT_RUNTYPE_NONE)
                    xt_usage(argv[0]);
                else
                    xt_runtype = XT_RUNTYPE_THREAD;
                if (i + 1 < argc) {
                    if (*argv[i+1] != '-') {
                        str_size = strlen(argv[i+1]);
                        (void)strncpy(xt_afname, argv[i+1], str_size);
                        xt_afname[str_size] = '\0';
                    } /* endif */
                } /* endif */
                break;
#endif /* THREADSAFE */
            case 'o':                   /* Must be -o fname  */
                (void) strcpy(xt_ofname, argv[i+1]);
                break;
            case 'n':
                (void) strcpy(xt_iterations, argv[i+1]);
                break;
            case 'd':
                if ((argv[i][2]) == 'p')
                    xt_dump_objects_private = TRUE;
                else
                    xt_dump_objects = TRUE;
                break;
            case 's':
                (void) strcpy(xt_timeslice, argv[i+1]);
                break;
            case 'v':
                xt_verbose = TRUE;
                break;
            case 'x':
				xt_interrupt_enabled = TRUE;
				if (argv[i][2] == 'a')  {
					xt_interrupt_automatic = TRUE;
				} else {
					long delay;
					char *endptr;
					if ( argv[i][2] == '\0') {
						(void) strcpy(xt_interrupt_delay_string, argv[i+1]);
					} else {
						(void) strcpy(xt_interrupt_delay_string, &argv[i][2]);
					} /* endif */
					if ( xt_interrupt_delay_string == NULL) {
						xt_err("xt_init", "bad delay value for interrupt (-x)");
						exit(XT_ERROR);
					} /* endif */
					delay = strtol(xt_interrupt_delay_string, &endptr, 0 );

					if (delay == 0 && (errno != 0 || xt_interrupt_delay_string == endptr)) {
						xt_err("xt_init", "bad delay value for interrupt (-x)");
						exit(XT_ERROR);
					 } else if (errno !=0 && 
								(delay == LONG_MAX || delay == LONG_MIN)) {
						xt_err("xt_init", "bad delay value for interrupt (-x)");
						exit(XT_ERROR);
					} /* endif */
				} /* endif */
                break;
            case '0':
                fflush(NULL);
                exit(0);
                break;
            default:
                xt_usage(argv[0]);
                break;
            } /* end switch */
        } /* endif */
    } /* end for */

    if (xt_runtype == XT_RUNTYPE_NONE) {
        xt_usage(argv[0]);
    } /* endif */

    if (retc == XT_OK) {
        retc = xt_runtype;         /* Return the type of run.           */
    } /* endif */
    return retc;
} /* end xt_init */


OM_sint xt_usage(char *pgmname)
{
#ifdef THREADSAFE
    printf("usage: %s { -i <testcase> | -a <testcases> | -t <testcases>} [-o <logfile>] [-n <number>] [-s <time in us>] [-c]  [-v] [-x <delay in us>] [-0]\n",
                                                                       pgmname);
#else
    printf("usage: %s { -i <testcase> | -a <testcases> } [-o <logfile>] [-n <number>] [-v] [-x <delay in us> | -xa] [-0]\n", pgmname);
#endif /* THREADSAFE */
    printf(" -i for single testcase\n");
    printf(" -a for multiple sequential testcases\n");
#ifdef THREADSAFE
    printf(" -t for multiple threaded testcases\n");
#endif /* THREADSAFE */
    printf(" -o to override logfile name default (%s)\n", XT_DEF_LOG_FNAME);
    printf(" -n to override number of iterations default (%s)\n", XT_DEF_NO_ITERATIONS);
#ifdef THREADSAFE
    printf(" -s to override default time-slice (in us), range from %s to %s\n", XT_MIN_TIMESLICE, XT_MAX_TIMESLICE);
#endif /* THREADSAFE */
    printf(" -c for conversion of objects to string and back(convenience library)\n");
    printf(" -v for verbose output\n");

#ifdef THREADSAFE
    printf(" -x to cause interrupt (cancel) after given delay (in us)\n");
    printf(" -xa to cause interrupt (cancel) after automatic delay\n");
#else
    printf(" -x to cause interrupt (signal) after given delay (in us)\n");
    printf(" -xa to cause interrupt (signal) after automatic delay\n");
#endif /* THREADSAFE */
    printf(" -0 to print version info and exit\n");
    exit (XT_ERROR);
} /* end xt_usage */


/****************************************************************************/
/*                                                                          */
/*   Close the output and input (if any) files.                             */
/*                                                                          */
/****************************************************************************/

OM_sint xt_dinit(void)
{

    if (xt_testcase_file != (FILE *) NULL)  {
        fclose(xt_testcase_file);
        xt_testcase_file = (FILE * )NULL;
    } /* endif */

    if (xt_log_file != (FILE * ) NULL) {
        fclose(xt_log_file);
        xt_log_file = (FILE * )NULL;
    } /* endif */

    return (XT_OK);

} /* end xt_dinit */
