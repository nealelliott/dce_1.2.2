/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: tstdriver.c,v $
 * Revision 1.1.6.2  1996/02/17  23:22:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:26  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:12:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:04  root]
 * 
 * Revision 1.1.3.3  1993/01/13  19:49:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:16:37  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:23:27  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:48:25  weisman]
 * 
 * Revision 1.1  1992/01/19  03:38:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
**
**  NAME
**
**      tstdriver.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  command driver module for test program
**  
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**   14-aug-91 bmartin  Remove RPC_VERIFY_INIT call - there are no private
**                      calls here.
**   19-jul-91 bmartin  Move forward declaration of get_cmd and external
**                      reference of cmdtab to cover scope of the module;
**                      use ansi-c remove () not UNIX-specific unlink().
**   8-may-91 woodbury  Add lock file to force synchronous running of tests.
**                      Allow asynchronous testing with -a flag.
**   7-may-91 woodbury  Add code for incremental numbering of tests - this
**                      is useful when running this program from a tsh script.
**                      Add '-i' flag to initialize environment variable.
**                      Optioned out some dead code.
**                      Fixed [potential] typecast bug for user-supplied status 
**                      codes which are > 2^31.
**  14-mar-91 woodbury  access violation problem with *margv[0]
**  14-nov-90 woodbury  add "in_status" as IN under "-c" flag
**  07-nov-90 woodbury  implemented use of verbose flag
**                      fixed bug so INSF_ARGS gets returned when no cmd input
**  01-nov-90 woodbury  change -s flag to -f to avoid conflict with tsh flag
**                      de-TABBED
**  26-oct-90 burton    add - close test script file and exit
**  26-oct-90 woodbury  add call to RPC_VERIFY_INIT
**  25-oct-90 woodbury  de-TABBED and brought indentation in line with 
**                      coding conventions. Made it mostly 'lint'-free.
**  25-oct-90 woodbury  began to clean up. changed default action to getting
**                      arguments from the command line. Added '-s' flag to
**                      signify use of script file.
**  15-oct-90           original was public domain software
**
**  %a%private_end  
**
*/

/*#include    <sysconf.h> */    /* System definitions for RPC runtime */
#ifdef SNI_SVR4
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#include 	<sys/errno.h>
#include    <dce/rpc.h>     /* Public definitions for RPC runtime */
#include    <tstdriver.h>   /* Local definitions and testdriver protocol */
#include    <comtest.h>     /* macros for status checks, etc. */

/* 
 * forward function declarations
 */

struct cmd      *getcmd();

/*
 * external data 
 */
extern struct cmd   cmdtab[];

/*
 * global data for this module
 */
#if defined(SNI_SVR4_POSIX)
sigjmp_buf toplevel;           /* non-local goto stuff for cmd scanner */
#else
jmp_buf toplevel;           /* non-local goto stuff for cmd scanner */
#endif
int     verbose = 0;        /* print messages returned by tests */
#if 0
int     fromatty;           /* input is from a terminal */
#endif
int     use_script=0;       /* use an external script for commands */
int     use_stderr=0;       /* flag for duplicating diagnostics */
int     async_testing=0;    /* flag to allow asynchronous running of tests */

unsigned32  in_status=0;    /* IN argument specifying a status code value */

char    line[LINESZ];       /* input line buffer */
int     margc;              /* count of arguments on input line */

char    *margv[20] = {NULL};/* args parsed from input line */
char    *stringbase;        /* current place in buffer */
char    *argbase;           /* current storage point in arg buffer */
char    argbuf[200];        /* argument storage buffer */
char    *altarg;            /* */
FILE    *tscpt;             /* Test Script file descriptor */
FILE    *testno_fptr;       /* FILE descriptor for test number data file */

char    *slurpstring();
int     intr(); 

extern int errno;



main(argc, argv)
int     argc;
char    *argv[];
{
    char    *cp;
    int     top;
    int     exstat = COMTEST_FAILED;
    char    out_text[80];
    extern  char *tstscript;
    long    local_status;

    
    in_status = rpc_s_ok;
         
    argc--, argv++;
    while (argc > 0 && **argv == '-') 
    {
        for (cp = *argv + 1; *cp; cp++)
            switch (*cp) 
            {

                /*
                 * Initialize data file containing test number.
                 */ 
                case 'i':
                if (( testno_fptr = fopen (COMTEST_TESTNO, "w+")) != NULL ) 
                {
                    int fstat;
                    fstat = fprintf (testno_fptr, "%d", 1);
                    if (fstat < 0)
                    {
                        sprintf (out_text, "Could not write data to file %s",
                                 COMTEST_TESTNO);
                        perror (out_text);
                        exit (1);
                    }
                    if ((fclose (testno_fptr)) != 0)
                    {
                        sprintf (out_text, "Could not close file %s", COMTEST_TESTNO);
                        perror (out_text);
                        exit (1);
                    }
                }
                else
                {
                    sprintf (out_text, "Can't open file %s", COMTEST_TESTNO );
                    perror (out_text);
                    exit (1);
                }
                break;

                case 'v':
                verbose = 1;
                break;  
    
                /*
                 * Note that any flags that follow a "-f filename" are lost
                 */
                case 'f':
                if (in_status != rpc_s_ok)
                {
                    fprintf (stdout,
                        "use of '-f' and '-c' flags is mutally exclusive.\n");
                    if (use_stderr) fprintf (stderr,
                        "use of '-f' and '-c' flags is mutally exclusive.\n");
                    exit(1);
                }
                use_script = 1;
                break;

                case 'a':
                async_testing = 1;
                break;

                case 's':
                use_stderr = 1;
                break;

                case 'd':
                argc--;
                argv++;
                break;

                case 'w':
                argc--; argv++;
                argc--; argv++;
                break;

                case 'c':
                argc--;
                argv++;
                if ((isanum (*argv, &local_status)) == 0)
                {
                    fprintf (stdout, 
                        "unexpected value for status code INPUT: %s.\n", *argv);
                    if (use_stderr) fprintf (stderr, 
                        "unexpected value for status code INPUT: %s.\n", *argv);
                    exit(1);
                }
                in_status = (unsigned32) local_status;
                break;
                default:
                fprintf(stdout,
                    "ftp: %c: unknown option\n", *cp);
                if (use_stderr) fprintf(stderr,
                    "ftp: %c: unknown option\n", *cp);
                exit(1);
            }
        argc--, argv++;
    }
#if 0
    fromatty = isatty(fileno(stdin));
#endif

    /*
     * If there is any argument left on the command line, 
     * it must be the name of an alternative testscript file. 
     */
    if (use_script == 1)
    {
        if (argc > 0 )
        {
            tstscript = *argv;
            argc--, argv++;
        }
        if (( tscpt = fopen (tstscript, "r")) == NULL )
        {
            sprintf (out_text, "Can't open testscript file %s", tstscript );
            perror(out_text);
            exit(1);
        }
#if defined(SNI_SVR4_POSIX)
        top = sigsetjmp(toplevel, 1) == 0;
#else
        top = setjmp(toplevel) == 0;
#endif

        if (top) 
        {
#if defined(SNI_SVR4_POSIX)
			{
			struct sigaction	act;

			act.sa_handler = intr;
			if ( sigemptyset( &act.sa_mask ) != 0 )
				{
				perror("sigemptyset");
				exit(1);
				}
			act.sa_flags = 0;

			if ( sigaction( SIGINT, &act, (struct sigaction *)NULL ) != 0 )
				{
				perror("sigaction");
				exit(1);
				}
			}
#else
            (void) signal(SIGINT, (void *) intr);
#endif
        }
        for (;;) 
        {
            cmdscanner();
            top = 1;

            if((fclose (tscpt)) == EOF)
            {
                perror("Failed to close");
            }
            break;
        }
    }
    else if (argc > 0)
    {
        exstat = cmdexecute (argc, argv);
    }
    else
    {
        fprintf (stderr, 
  "usage: tstdriver [[-v][-d][-c][-i][-a][-s][-f[<file>]][-w[<sec>]] <arg1, ... argn>\n");
    }

    if (async_testing == 0)
    {
        /*
         * Remove the file that insures synchronous testing.
        */
        remove (COMTEST_SYNC_FILE);
    }
    exit ((exstat != COMTEST_OK) ? 1 : 0);    
}

int intr()
{

#if defined(SNI_SVR4_POSIX)
    siglongjmp(toplevel, 1);
#else
    longjmp(toplevel, 1);
#endif
}

/*
 * Command parser.
 */
cmdscanner()

{
    register struct cmd *c;
    size_t              siz;
    unsigned32          status;
    static int          testno = 1;
    int                 ret;
    char                *marg;

    for (;;) 
    {

        if (fgets(line, LINESZ, tscpt) == 0)
        {
            break;
        }
        siz = strlen(line);
        line[(siz - 1)] = '\0';
        if (line[0] == 0)
            break;

        makeargv();
        if (margc == 0) 
        {
            continue;
        }

        marg = margv[0];

        c = getcmd (marg);
        if (c == (struct cmd *)(-1)) 
        {
            fprintf (stdout, "?Ambiguous command\n");
            if (use_stderr) fprintf (stderr, "?Ambiguous command\n");
            continue;
        }
        if (c == 0) 
        {
            fprintf (stdout, "invalid command: %s\n", margv[0]);
            if (use_stderr) 
                fprintf (stderr, "invalid command: %s\n", margv[0]);
            continue;
        }

        ret = (*c->c_handler)(margc, margv, &status);

        PRINT_STATUS(ret, testno++, marg, "com test", status);
    }
#if defined(SNI_SVR4_POSIX)
			{
			struct sigaction	act;

			act.sa_handler = intr;
			if ( sigemptyset( &act.sa_mask ) != 0 )
				{
				perror("sigemptyset");
				exit(1);
				}
			act.sa_flags = 0;

			if ( sigaction( SIGINT, &act, (struct sigaction *)NULL ) != 0 )
				{
				perror("sigaction");
				exit(1);
				}
			}
#else
    (void) signal(SIGINT, (void *) intr);
#endif
}

/*
 * execute single command 
 */
cmdexecute(argc,argv)

int     argc;
char    *argv[];

{
    register struct cmd *c;
    unsigned32          status;
    int                 testno;
    int                 ret;
    char                *arg;
    char                out_text[128];
    int                 temp;

    /*
     * Insure synchronous execution of several tests
     */
    if (async_testing == 0)
    {
        temp = create_sync_lock();
        if (temp == (-1))
        {
            perror ("Could not open lock file");
            exit (1);
        }
    }

    /*
     * Open the file that contains the current test number.
     */
    testno_fptr = NULL;
    if (( testno_fptr = fopen (COMTEST_TESTNO, "r+")) == NULL ) 
    {
        testno_fptr = fopen (COMTEST_TESTNO, "w+");
    }

    if (testno_fptr != NULL)
    {
        /*
         * Get any data that may exist in file.
         */
        if ((fscanf (testno_fptr, "%d", &testno)) == EOF)
        {
            /*
             * No data (new file), set testno.
             */
            testno = 1;
        }

        if ((fseek (testno_fptr, 0, 0)) != 0)
        {
            perror ("Could not fseek to beginning of file");
            exit (1);
        }

        /*
         * Write the next test number to file.
         */
        if ((fprintf (testno_fptr, "%d", (testno + 1))) < 0)
        {
            sprintf (out_text, "Could not write data to file %s", 
                     COMTEST_TESTNO);
            perror (out_text);
            exit (1);
        }

        if ((fclose (testno_fptr)) != 0)
        {
            sprintf (out_text, "Could not close file %s", COMTEST_TESTNO);
            perror (out_text);
            exit (1);
        }
    }
    else
    {
        sprintf (out_text, "Could not open file %s for writing",
                 COMTEST_TESTNO);
        perror (out_text);
        exit (1);
    }


    arg = argv[0];

    c = getcmd (arg);

    ret = INSF_ARGS;

    if (c != (struct cmd *)0)
    {
        ret = (*c->c_handler)(argc, argv, &status);
		if (ret == COMTEST_OK)
			status = rpc_s_ok;
        PRINT_STATUS(ret, testno, arg, "com test", status);
        return (ret);
    }

    fprintf (stdout, "invalid command \n");
    if (use_stderr) fprintf (stderr, "invalid command \n");
              
    return (INVALID_ARG);
}


/*
 * Slice a string up into argc/argv.
 */

int slrflag;

makeargv()
{
    char **argp;
    char *slurpstring();

    margc = 0;
    argp = margv;
    stringbase = line;          /* scan from first of buffer */
    argbase = argbuf;           /* store from first of buffer */
    slrflag = 0;
    while (*argp++ = slurpstring())
         margc++;
}

/*
 * Parse string into argbuf;
 * handle quoting and strings
 */
char *
slurpstring()
{
    int got_one = 0;
    register char *sb = stringbase;
    register char *ap = argbase;
    char *tmp = argbase;        /* will return this if token found */



S0:
    switch (*sb) {

    case '\0':
         goto OUT;

    case ' ':
    case '\t':
         sb++; goto S0;

    default:
         switch (slrflag) {
            case 0:
                 slrflag++;
                 break;
            case 1:
                 slrflag++;
                 altarg = sb;
                 break;
            default:
                 break;
         }
         goto S1;
    }

S1:
    switch (*sb) {

    case ' ':
    case '\t':
    case '\0':
         goto OUT;    /* end of token */

    case '\\':
         sb++; goto S2;    /* slurp next character */

#ifdef  SLURP_QUOTED
    case '"':
         sb++; goto S3;    /* slurp quoted string */
#endif
    default:
         *ap++ = *sb++;    /* add character to token */
         got_one = 1;
         goto S1;
    }

S2:
    switch (*sb) {

    case '\0':
         goto OUT;

    default:
         *ap++ = *sb++;
         got_one = 1;
         goto S1;
    }

S3:
    switch (*sb) {

    case '\0':
         goto OUT;

    case '"':
         sb++; goto S1;

    default:
         *ap++ = *sb++;
         got_one = 1;
         goto S3;
    }

OUT:
    if (got_one)
         *ap++ = '\0';
    argbase = ap;            /* update storage pointer */
    stringbase = sb;        /* update scan pointer */
    if (got_one) {
         return(tmp);
    }
    switch (slrflag) {
         case 0:
            slrflag++;
            break;
         case 1:
            slrflag++;
            altarg = (char *) 0;
            break;
         default:
            break;
    }
    return((char *)0);
}

struct cmd *
getcmd(name)
    register char *name;
{
    register char *p, *q;
    register struct cmd *c, *found;
    register int nmatches, longest;

    longest = 0;
    nmatches = 0;
    found = 0;
    for (c = cmdtab; (c != NULL) && ((p = c->c_name) != NULL); c++) 
    {
        for (q = name; *q == *p++; q++)
            if (*q == 0)
                return (c);
        if (!*q) 
        {
            if (q - name > longest) 
            {
                 longest = q - name;
                 nmatches = 1;
                 found = c;
            } 
            else if (q - name == longest)
                 nmatches++;
         }
    }
    if (nmatches > 1)
         return ((struct cmd *)-1);
    return (found);
}


int isanum( str, num )

char    *str;
long    *num;
{
    char    *cptr;
    int     res;

    cptr = str;
    res = 0;
    while (*cptr != '\0') 
    {
        if (isdigit(*cptr)) 
        {
            res = 1;
        } 
        else if (*cptr != '-') 
        {
            if (verbose != 0)
            {   
                printf("offending char is \"%c\" (%o, octal)\n", *cptr, *cptr);
            }
            res = 0;
            break;
        }
        cptr++;
    }

    if (res == 0)
    {
        return( 0 );
    } 
    else 
    {
        *num = atoi( str );
    }
    return( 1 );
}


/*
**++
**
**  Routine NAME:       create_sync_lock
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**  Create a lockfile which ensures synchronous execution of a number of 
**  tests. This may occur when this program is run many times in succession.
**  This routine blocks until the file can be opened.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:            none
**
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     integer
**
**                      Returns 0 on success, (-1) on failure.
**
**  SIDE EFFECTS:       Creates a lock file.
**
**--
**/

create_sync_lock ()

{
    int     fd;  
    int     max;

    max = 0;
    do 
    {
        errno = 0;
        fd = open (COMTEST_SYNC_FILE, (O_CREAT|O_EXCL|O_RDWR), 0666);
        if (fd >= 0)
        {
            close (fd);
        }
        else
        {
            if (max > 500)
            {
                return (-1);
            }
            max++;
        }
    } while ((fd < 0) && (errno == EEXIST));

    if (fd < 0)
    {
        return (-1);
    }
    return (0);
}
