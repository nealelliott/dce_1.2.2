/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client.c,v $
 * Revision 1.1.11.2  1996/02/17  23:23:40  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:07  marty]
 *
 * Revision 1.1.11.1  1995/12/11  20:14:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:28  root]
 * 
 * Revision 1.1.9.1  1993/10/15  17:00:27  tom
 * 	Bug 8303 - cast argument to rpc_binding_set_auth_info.
 * 	[1993/10/14  22:48:54  tom]
 * 
 * Revision 1.1.7.2  1993/08/12  16:07:22  kissel
 * 	OT 8341 - Include param.h for HPUX kernel builds.
 * 	[1993/08/11  19:21:31  kissel]
 * 
 * Revision 1.1.4.2  1993/01/13  19:55:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:22:55  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  13:49:04  rsalz
 * 	 04-nov-91 sommerfeld    - Add -pag option (only makes sense in kernel).
 * 	                         - Clean up passing of argv/argc to avoid overrunning buffer.
 * 	 05-aug-91 labosseriere  mods for -e and -t switches
 * 	[1992/04/30  23:23:07  rsalz]
 * 
 * Revision 1.2  1992/01/19  22:14:13  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
 */
/*
 *  OSF DCE Version 1.0 
 */
/*
 * ncstest client.
 *
 * This is another ncs test suite.  This code is designed to work
 * in both user and kernel RPC environments.  This somewhat limits
 * what can be done in the code and most definitely induces strange
 * looking code.
 *
 * Parts of the code in this module get used in 3 different contexts:
 * (1) Standard user-space ncstest client.
 * (2) User space driver for in-kernel ncstest client.
 * (3) In-kernel ncstest client.
 *
 * With lots of luck, this is one of the last client / server templates
 * that we'll have to write for such purposes.  Just snap new tests into
 * the tinfo array and away we go...!
 *
 * KERNEL_TEST_DRIVER is #define'd when this module is being built for
 * context (2).  _KERNEL is #define'd for context (3).  Nothing in
 * particular is #define'd for context (1); though _KERNEL and
 * KERNEL_TEST_DRIVER will NOT be defined).
 */

#ifndef _KERNEL
#  include <stdio.h>
#else
#ifdef __hpux
#  include <sys/param.h>
#endif
#  include <sys/user.h>
#endif

#include <ncstest.h>

#ifdef _KERNEL
#define GETUID() u.u_uid
#define GETPAG() u.u_uid
#define malloc idl_malloc_krpc
#define free idl_free_krpc
#else
#define GETPAG() getuid()
#define GETUID() getuid()
#endif

#if defined(_KERNEL) || !defined(KERNEL_TEST_DRIVER)
/**************************************************************************/
/* 
 * Stuff used in the standard user mode code (context[1])
 * and in the in-kernel test (context[3])
 */

/*
 * The set of tests supported by the client.
 */

#define NO_NCSTEST \
    { 0, " ", " " }

#ifdef NCSTEST_SIMPLE
#include <smpl.h>
extern int ncstest_simple_clt();
#endif

#ifdef NCSTEST_AFS
#include <afs.h>
extern int ncstest_afs_clt();
#endif

int ncstest_shutdown_clt();

static struct tinfo_t {
    int (*cproc)();
    char *name;
    char *usage;
} tinfo[] = {
#ifdef NCSTEST_SIMPLE
    /*  0 */ { ncstest_simple_clt, ncstest_simple_name, " " },
#else
    /*  0 */ NO_NCSTEST,
#endif
#ifdef NCSTEST_AFS
    /*  1 */ { ncstest_afs_clt, ncstest_afs_name, " " },
#else
    /*  1 */ NO_NCSTEST,
#endif
    /*  2 */ { ncstest_shutdown_clt, "server shutdown test", " " },
};

#define N_TESTS (sizeof tinfo / sizeof (struct tinfo_t))

globaldef int ncstest_c_verbose = false;
globaldef int ncstest_c_eflag = false;
static int ncstest_c_local_shutdown = false;
static int ncstest_c_use_call_timeout = false;
static int nil_obj = true;

/*
 * U S A G E
 *
 * Print how to use this program
 */
static void usage(test)
int test;
{
    int i;

    if (test != -1)
        printf(
            "usage: %s_client [-vScd[level]] %d <server> <passes> %s\n",
#ifdef _KERNEL
            "kernel",
#else
            "user",
#endif
            test, tinfo[test].usage
        );
    else {
        printf("usage: %s_client [-vScoed[level]] [-t<level>] [-p <p> <n> <l>] <test#> ",
#ifdef _KERNEL
            "kernel"
#else
            "user"
#endif
        );
        printf("<server> <passes>\n");
        printf("   -v: Selects verbose mode\n");
        printf("   -S: shutdown test via local call\n");
        printf("   -c: use 15 sec call timeout\n");
        printf("   -o: use non-nil object id\n");
        printf("   -e: exit (stop iterations) when error encountered\n");
        printf("   -d[level]: Enables NCK debugging output\n");
        printf("   -t<level>: set com_timeout level\n");
        printf("   -p <auth_proto> <auth_name> <authn_level>: Use authenticated RPC\n");

        printf("\n");

        for (i = 0; i < N_TESTS; i++)
            printf(
                "        test %d: %s-- usage: %s_client %d <server> <passes> %s\n",
                i, tinfo[i].name,
#ifdef _KERNEL
            "kernel",
#else
            "user",
#endif
            i, tinfo[i].usage
            );
    }
}

/*
 * N C S T E S T _ S H U T D O W N _ C L T
 *
 * Client processing for server shutdown test.
 */
int ncstest_shutdown_clt(h)
rpc_binding_handle_t h;
{
    unsigned32 st, tst;
    int failures = 0;
    unsigned char   emsg[64];

    if (ncstest_c_local_shutdown)
    {
        rpc_mgmt_stop_server_listening(NULL, &st);
    }                       
    else                    
    {                       
        rpc_mgmt_stop_server_listening(h, &st);
    }

    if (st != error_status_ok)
    {
        int tst;
        dce_error_inq_text(st, emsg, &tst);
        printf("(ncstest_clt) shutdown failed - %s\n", emsg);
        failures++;
    }

    return failures;
}

/*
 * D O I T
 *
 * Actually do the client side of the test.
 */
#if !defined (_AIX)
static error_status_t doit(argc, argv)
#else
error_status_t doit(argc, argv)
#endif
int argc;
char *argv[];
{
    error_status_t  st, tst;
    int             test, i;
    ndr_boolean     debug;
    char            *dbgstr;
    char            *server;
    int             auth_proto = rpc_c_authn_none;
    int             authn_level = rpc_c_authn_level_default;
    unsigned char  *auth_name;
    int             passes;
    int             failures, failure_total;
    rpc_binding_handle_t    h;
    unsigned char   emsg[64];
    unsigned32      com_timeout = rpc_c_binding_default_timeout;
    char            *com_timeout_str;
    extern          char *strrchr();
    unsigned32      *pag_uid = NULL;
    unsigned32      pag = GETPAG();

    st = error_status_ok;
    failures = failure_total = 0;
    ncstest_c_verbose = debug = ncstest_c_local_shutdown = ndr_false;
    ncstest_c_use_call_timeout = false;

    argc--; argv++;

    while (argc && argv[0][0] == '-') {

        if ((strcmp(argv[0], "-p") == 0) &&
            (argc > 4))
        {
            auth_proto = atoi (argv[1]);
            auth_name = (unsigned char *)argv[2];
            authn_level = atoi (argv[3]);
printf("auth_proto=%d, auth_name=%s., auth_level=%d\n", auth_proto, auth_name, authn_level);
            argv += 4;
            argc -= 4;
            continue;
        }

        if ((strcmp(argv[0], "-pag") == 0) &&
            (argc > 2))
        {
            pag = atoi (argv[1]);
            argv += 2;
            argc -= 2;
            continue;
        }
        ncstest_c_verbose = (strrchr(argv[0], 'v') != NULL);
        ncstest_c_local_shutdown = (strrchr(argv[0], 'S') != NULL);
        ncstest_c_use_call_timeout = (strrchr(argv[0], 'c') != NULL);
        debug = ((dbgstr = strrchr(argv[0], 'd')) != NULL);
        nil_obj = (strrchr(argv[0], 'o') == NULL);
        ncstest_c_eflag = (strrchr(argv[0], 'e') != NULL);

        if (debug) {
            dbgstr++;
            if (*dbgstr == '\0')
                dbgstr = "0.1";
        }

        if ((com_timeout_str = strrchr(argv[0], 't')) != NULL)
        {
            com_timeout = atoi (&com_timeout_str[1]);
        }

        argv++, argc--;
    }
    if (argc < 3) {
        usage(-1);
        st = -1;
        goto DONE;
    }

    test = atoi(argv[0]);
    server = argv[1];
    passes = atoi(argv[2]);

    if (test < 0 || test >= N_TESTS) { 
        printf("Bad test number: %d\n", test);
        st = -2;
        goto DONE;
    }

    if (tinfo[test].cproc == NULL) {
        printf("Test not included: %d\n", test);
        st = -3;
        goto DONE;
    }

    if (debug) {
        rpc__dbg_set_switches((ndr_char *)dbgstr, &st);
        if (st != error_status_ok) {
            int tst;
            dce_error_inq_text(st, emsg, &tst);
            printf("Error setting debug level %s - %s\n", dbgstr, emsg);
            goto DONE;
        }
    }

    rpc_binding_from_string_binding((ndr_char *)server, &h, &st);
    if (st != error_status_ok) {
        int tst;
        dce_error_inq_text(st, emsg, &tst);
        printf(
            "Cannot get binding from string binding %s - %s\n",
            server, emsg
        );
        goto DONE;
    }

    rpc_mgmt_set_com_timeout(h, com_timeout, &st);
    if (st != error_status_ok) {
        int tst;
        dce_error_inq_text(st, emsg, &tst);
        printf(
            "Cannot set com timeout '%d' - %s\n",
            com_timeout, emsg
        );
        goto DONE;
    }

    if (! nil_obj) {
        uuid_t  obj_id;

        uuid_create(&obj_id, &st);
        rpc_binding_set_object(h, &obj_id, &st);
        if (st != error_status_ok) {
            int tst;
            dce_error_inq_text(st, emsg, &tst);
            printf("Cannot set binding object - %s\n", emsg);
            goto DONE;
        }
    }

    if (ncstest_c_use_call_timeout) {
        rpc_mgmt_set_call_timeout(h, 15 /* sec */, &st);
        if (st != error_status_ok) {
            int tst;
            dce_error_inq_text(st, emsg, &tst);
            printf("Cannot set call timeout - %s\n", emsg); 
        }
    }

    if (auth_proto != rpc_c_authn_none) {
#ifdef _KERNEL
        /*
         * In the kernel, we use an array of two unsigned32's; the
         * first one is the process authentication group, while the
         * second is the current UNIX user id.
         */
        pag_uid = malloc (2 * sizeof(unsigned32));
        
        pag_uid[0] = pag;
        pag_uid[1] = GETUID();
#endif
printf("calling binding_set_auth_info next\n");
        rpc_binding_set_auth_info (h, auth_name,
            authn_level, auth_proto, (rpc_auth_identity_handle_t)pag_uid,
            rpc_c_authz_dce, &st);
printf("binding_set_auth_info returns st=%d\n", st);
        if (st != error_status_ok) {
            int tst;
            dce_error_inq_text(st, emsg, &tst);
            printf(
                "Cannot set auth info on handle to authn proto %d, name %s - %s\n",
                auth_proto, auth_name, emsg
            );
            goto DONE;
        }
    }

    /*
     * The following is meaningless (but harmless) for the kernel.
     */
    rpc_mgmt_set_cancel_timeout(5 /* sec */, &st);
    if (st != error_status_ok) {
        int tst;
        dce_error_inq_text(st, emsg, &tst);
        printf("Cannot set cancel timeout - %s\n", emsg);
    }

    printf("Bound to %s\n", server);

#ifndef _KERNEL
    TRY {
#endif
    for (i = 0; i < passes; i++) {
        failure_total += failures = (*tinfo[test].cproc)(h);
        if (failures != 0) {
            printf(
                "pass %d encountered %d failure%s.\n",
                i, failures, failures == 1 ? "" : "s"
            );
            if (ncstest_c_eflag) {
                printf("terminating loop\n");
                break;
            }
        } else
           if (ncstest_c_verbose)
               printf("pass %d passed\n", i);
#ifndef _KERNEL
        pthread_testcancel();
#endif
    }
#ifndef _KERNEL
    } CATCH_ALL {
        printf("cancel received...\n");
        RERAISE;
    } ENDTRY
#endif
    if (failure_total == 0)
            printf ("test passed.\n");
        else
            printf(
                "encountered %d total failure%s.\n",
                failure_total, failures == 1 ? "" : "s"
            );

    rpc_binding_free(&h, &st);

    if (pag_uid) {
        free(pag_uid);
        pag_uid = 0;
    }

    if (st != error_status_ok) {
        int tst;
        dce_error_inq_text(st, emsg, &tst);
        printf("Cannot free binding - %s\n", emsg);
        goto DONE;
    }

DONE:
    if (st != error_status_ok)
        return st;
    return failure_total;
}

#endif /* defined(_KERNEL) || !defined(KERNEL_TEST_DRIVER) */



#ifdef _KERNEL
/**************************************************************************/
/*
 * The following code is used only in kernel space (context 3).
 */

#if !defined (_AIX)
static int argc;
static char *argv[NCSTEST_MAX_ARGC];

/*
 * I N A R G S _ T O _ A R G S
 *
 * Create argc,argv from the inargs.
 */
static int inargs_to_args(args)
ncstest_args_t *args;
{
    int i, nbytes;
    char *cp;

    argc = args->inargs.argc;
    if (argc > NCSTEST_MAX_ARGC) {
        printf("too many cmd line args: %d\n", argc);
        return -1;
    }

    cp = &args->inargs.argvchars[0];
    nbytes = 0; 
    for (i = 0; i < argc; i++) {
        argv[i] = cp;
        nbytes = (strlen(argv[i]) + 1);
        cp += nbytes;
    }
    return 0;
}

/*
 * N C S T E S T _ C _ D O I T
 *
 * Get the client test to run (called from the NCS test jig device driver).
 */
void ncstest_c_doit(inargs, outargs)
ncstest_inargs_t    *inargs;
ncstest_outargs_t   *outargs;
{
    int r;

    r = inargs_to_args(inargs);
    if (r == -1) {
        outargs->st = -1;
        return;
    }

    r = doit(argc, argv);
    outargs->st = (int) r;
}

#endif /* !_AIX */
#endif /* _KERNEL */


#ifdef KERNEL_TEST_DRIVER
/**************************************************************************/
/*
 * The following code is used only in the user space test driver (context 2).
 */

/*
 * A R G S _ T O _ I N A R G S
 *
 * Copy the cmd line args to the kernel test input arg buffer.
 */
static int args_to_inargs(argc, argv, inargs)
int argc;
char *argv[];
ncstest_inargs_t *inargs;
{
    int i, nbytes;
    char *cp;

    if (argc > NCSTEST_MAX_ARGC) {
        printf("too many cmd line args: %d\n", argc);
        return -1;
    }
    inargs->argc = argc;

    cp = &inargs->argvchars[0];
    nbytes = 0; 
    for (i = 0; i < argc; i++) {
        nbytes = (strlen(argv[i]) + 1);
        if ((cp-&inargs->argvchars[0]) > NCSTEST_MAX_ARGVBYTES) {
            printf("too many cmd line argv bytes: > %d\n",
                NCSTEST_MAX_ARGVBYTES);
            return -1;
        }
        strcpy(cp, argv[i]);
        cp += nbytes;
    }

    return 0;
}


/*
 * K E R N E L _ D O I T
 *
 * Get the kernel to run the test client.
 */
static error_status_t kernel_doit(argc, argv)
int argc;
char *argv[];
{
    ncstest_args_t args;
    int fd;

    if ((fd = open(NCSDEVICE, O_RDWR)) == -1)
    {
        printf("can't open '%s'\n", NCSDEVICE);
        perror("");
        exit(-1);
    }

    if (args_to_inargs(argc, argv, &args.inargs) == -1)
        exit(-1);

    if (ioctl(fd, NIOCNTC, &args) == -1)
    {
        perror("ioctl");
        exit(-1);
    }
    return args.outargs.st;
}

#endif /* KERNEL_TEST_DRIVER */


#ifndef _KERNEL
/**************************************************************************/
/*
 * The following code is used only in user space (both contexts 1 and 2).
 */

/* 
 * M A I N
 */
main(argc, argv)
int argc;
char *argv[];
{
    error_status_t st;

#ifdef KERNEL_TEST_DRIVER
    st = kernel_doit(argc, argv);
#else
    st = doit(argc, argv);
#endif

    exit((int) st);
}

#endif /* _KERNEL */
