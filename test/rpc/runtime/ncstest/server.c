/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: server.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:42  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:09  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:32  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:55:27  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:23:06  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  13:49:19  rsalz
 * 	 18-oct-91 sommerfeld    Don't clobber dbgstr when parsing switches.
 * 	 18-sep-91 labossiere    fix warnings of dce_error_inq_text() usage
 * 	[1992/04/30  23:23:22  rsalz]
 * 
 * Revision 1.2  1992/01/19  22:14:14  devrcs
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
 * ncstest server.
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
 * the ifinfo array and away we go...!
 *
 * KERNEL_TEST_DRIVER is #define'd when this module is being built for
 * context (2).  _KERNEL is #define'd for context (3).  Nothing in
 * particular is #define'd for context (1); though _KERNEL and
 * KERNEL_TEST_DRIVER will NOT be defined).
 */

#ifndef _KERNEL
#  include <stdio.h>
#endif

#include <ncstest.h>


#if defined(_KERNEL) || !defined(KERNEL_TEST_DRIVER)
/**************************************************************************/
/* 
 * Stuff used in the standard user mode code
 * and in the in-kernel test (contexts 1 and 3)
 */

static unsigned long MaxCalls;
static ndr_boolean use_reserved_threads;
static ndr_boolean no_ep_register;

/*
 * The set of interfaces supported by the server.
 */

#ifdef NCSTEST_SIMPLE
#include <smpl.h>
globalref ncstest_simple_v1_0_epv_t ncstest_simple_manager_epv;
#endif

#ifdef NCSTEST_AFS
#include <afs.h>
globalref AFS4Int_v3_0_epv_t ncstest_afs_manager_epv;
#endif

struct ifinfo_t {
    rpc_if_handle_t *if_spec_p;
    rpc_mgr_epv_t mepv;
    char *name;
} ifinfo[] = {
#ifdef NCSTEST_SIMPLE
    { &ncstest_simple_v1_0_s_ifspec, 
     (rpc_mgr_epv_t) &ncstest_simple_manager_epv, 
     ncstest_simple_name 
    },
#endif
#ifdef NCSTEST_AFS
    { &AFS4Int_v3_0_s_ifspec, 
     (rpc_mgr_epv_t) &ncstest_afs_manager_epv, 
     ncstest_afs_name 
    },
#endif
};

#define N_IFS (sizeof ifinfo / sizeof (struct ifinfo_t))

#ifndef _KERNEL
static rpc_binding_vector_p_t ncstest_s_bvec;
#endif

globaldef int ncstest_s_verbose = false;

static rpc_thread_pool_handle_t thread_poolh;

/*
 * U S A G E
 */
static void usage()
{
    printf(
        "Usage: %s_server [-vrd[level]] <max calls> <protseq> ... \n",
#ifdef _KERNEL
            "kernel"
#else
            "user"
#endif
    );
    printf("   -v: Turns on verbose mode\n");
    printf("   -r: Use reserved threads\n");
    printf("   -d[level]: Enables NCK debugging output\n");
}

/*
 * N C S T E S T _ M G M T _ A U T H
 *
 * Our rpc_mgmt_ operation authorization function.
 * Allow everything...
 */
static ncstest_mgmt_auth(binding_handle, requested_mgmt_operation, status)
rpc_binding_handle_t  binding_handle;
unsigned32            requested_mgmt_operation;
unsigned32            *status;
{
    *status = rpc_s_ok;
    return true;
}

/*
 * T H R E A D _ P O O L _ F N
 *
 * Thread pool lookup function.  Manage the thread pool(s)
 * based on a simple object uuid scheme:
 *      nil object id     - use default pool threads
 *      non-nil object id - use the reserved thread pool
 */
static void thread_pool_fn(obj_uuid, if_id, opnum, phandle, status)
uuid_p_t                obj_uuid;
rpc_if_id_p_t           if_id;
unsigned32              opnum;
rpc_thread_pool_handle_t *phandle;
unsigned32              *status;
{
    unsigned32 st;

    *phandle = NULL;
    if (obj_uuid != NULL && ! uuid_is_nil(obj_uuid, &st)) {
        *phandle = thread_poolh;
    }
    *status = 0;
}

/*
 * S E T U P _ T H R E A D _ P O O L S
 *
 * Create a threads pool and register a pool lookup function.
 * Create a thread pool for the specified interface.
 * return 0 - ok
 *       -1 - couldn't create pool
 *        1 - couldn't set thread pool fn
 */
static int setup_thread_pools()
{
    unsigned32 st, tst;
    unsigned char   emsg[64];

    if (! use_reserved_threads)
        return 0;

    rpc_server_create_thread_pool(2, &thread_poolh, &st);
    if (st != error_status_ok) {
        int tst;
        dce_error_inq_text(st, emsg, &tst);
        printf("Cannot create thread pool - %s\n", emsg);
        return -1;
    }

    rpc_server_set_thread_pool_fn(thread_pool_fn, &st);
    if (st != error_status_ok) {
        int tst;
        dce_error_inq_text(st, emsg, &tst);
        printf("Cannot set thread pool fn - %s\n", emsg);
        return 1;
    }

    return 0;
}

/*
 * T E A R D O W N _ T H R E A D _ P O O L S
 *
 * Free our thread pools and unregister our lookup fn.
 * return 0 - ok
 *       -1 - couldn't free pool
 *        1 - couldn't set thread pool fn NULL
 */
static int teardown_thread_pools()
{
    unsigned32 st, tst;
    unsigned char   emsg[64];

    if (! use_reserved_threads)
        return 0;

    rpc_server_free_thread_pool(&thread_poolh, false /* wait */, &st);
    if (st != error_status_ok) {
        int tst;
        dce_error_inq_text(st, emsg, &tst);
        printf("Cannot free thread pool - %s\n", emsg);
        return -1;
    }

    rpc_server_set_thread_pool_fn(NULL, &st);
    if (st != error_status_ok) {
        int tst;
        dce_error_inq_text(st, emsg, &tst);
        printf("Cannot set thread pool fn NULL - %s\n", emsg);
        return 1;
    }

    return 0;
}

/*
 * R E G I S T E R _ I F S
 *
 * Register the interfaces we export.
 */
static error_status_t register_ifs()
{
    error_status_t  st, st1, tst;
    unsigned long   interface;
    unsigned char   emsg[64];

    st = error_status_ok;

    for (interface = 0; interface < N_IFS; interface++) {
        rpc_server_register_if(
            *ifinfo[interface].if_spec_p, NULL,
            ifinfo[interface].mepv, &st
        );
        if (st != error_status_ok) {
        int tst;
            dce_error_inq_text(st, emsg, &tst);
            printf(
                "Cannot register %s test manager - %s\n",
                ifinfo[interface].name, emsg
            );
            break;
        }
#ifndef _KERNEL
        if (! no_ep_register) {
            printf("Registering Endpoints...\n");
            rpc_ep_register(*ifinfo[interface].if_spec_p, ncstest_s_bvec, 
                0, (unsigned_char_p_t) ifinfo[interface].name, &st1);
            if (st1 != error_status_ok) {
                int tst;
                dce_error_inq_text(st1, emsg, &tst);
                printf(
                    "Cannot register %s endpoints - %s\n",
                    ifinfo[interface].name, emsg
                );
           }
       }
#endif
    }
    return st;
}


/*
 * U N R E G I S T E R _ I F S
 *
 * unregister the interfaces we exported.
 */
static error_status_t unregister_ifs()
{
    error_status_t  st, tst;
    unsigned long   interface;
    unsigned char   emsg[64];

    st = error_status_ok;

    for (interface = 0; interface < N_IFS; interface++) {
        rpc_server_unregister_if(
            *ifinfo[interface].if_spec_p, NULL, &st
        );
        if (st != error_status_ok) {
            int tst;
            dce_error_inq_text(st, emsg, &tst);
            printf(
                "Cannot unregister interface %s - %s\n",
                ifinfo[interface].name, emsg
            );
            break;
        }
#ifndef _KERNEL
        if (! no_ep_register) {
            printf("Unregistering Endpoints...\n");
            rpc_ep_unregister(*ifinfo[interface].if_spec_p, ncstest_s_bvec, 
                0, &st);
            if (st != error_status_ok) {
                int tst;
                dce_error_inq_text(st, emsg, &tst);
                printf(
                    "Cannot unregister %s endpoints - %s\n",
                    ifinfo[interface].name, emsg
                );
           }
       }
#endif
    }
    return st;
}


/*
 * G E T _ E N D P O I N T S
 *
 * Get ports on which to listen
 */
static error_status_t get_endpoints(argc, argv)
int argc;
char *argv[];
{

    error_status_t          st, tst;
    ndr_char                *string_binding;
    rpc_binding_vector_p_t  bvec;
    rpc_binding_vector_t    nil_bvec;
    ndr_boolean             validprotseq;
    unsigned long           i;
    unsigned char           emsg[64];

    st = error_status_ok;
    /*
     * Validate the specified protocols.
     */
    for (i = 0; i < argc; i++) {
        validprotseq = rpc_network_is_protseq_valid(
            (ndr_char *)argv[i], &st
        );
        if (st != error_status_ok) {
            int tst;
            dce_error_inq_text(st, emsg, &tst);
            printf(
                "Cannot check protocol sequence - %s\n", emsg
            );
            return st;
        }
        if (!validprotseq) {
            printf("Invalid protocol sequence %s\n", argv[i]);
            return st;
        }
    }

    /*
     * Since we may be in the kernel, determine what protseqs the server is 
     * already listening on and only add in new ones.
     */

    rpc_server_inq_bindings(&bvec, &st);
    if (st != error_status_ok && st != rpc_s_no_bindings) {
        int tst;
        dce_error_inq_text(st, emsg, &tst);
        printf("Cannot inquire bindings - %s\n", emsg);
        return st;
    }

    if (st == rpc_s_no_bindings) {
        nil_bvec.count = 0;
        nil_bvec.binding_h[0] = NULL;
        bvec = &nil_bvec;
    }

    for (i = 0; i < argc; i++) {
        ndr_boolean duplicate;
        int b;

        duplicate = false;
        for (b = 0; b < bvec->count; b++) {
            rpc_binding_to_string_binding(
                bvec->binding_h[b], &string_binding, &st
            );
            if (st != error_status_ok) {
                int tst;
                dce_error_inq_text(st, emsg, &tst);
                printf("Cannot get string binding - %s\n", emsg);
                return st;
            }

            /*
             * If the protseq is already on listening list, skip it.
             */
            if (strcmp(argv[i], string_binding) == 0)
                duplicate = true;

            rpc_string_free(&string_binding, &tst);
            if (duplicate)
                break;
        }
        if (duplicate)
            continue;

        rpc_server_use_protseq((ndr_char *)argv[i], MaxCalls, &st);
        if (st != error_status_ok) {
            int tst;
            dce_error_inq_text(st, emsg, &tst);
            printf(
                "Cannot use protocol sequence %s - %s\n",
                argv[i], emsg
            );
            return st;
        }
    }
    if (bvec->count != 0)
        rpc_binding_vector_free(&bvec, &tst);

    /*
     * Just inform the user of all the protseq's/endpoints available.
     */

    rpc_server_inq_bindings(&bvec, &st);
    if (st != error_status_ok) {
        int tst;
        dce_error_inq_text(st, emsg, &tst);
        printf("Cannot inquire bindings - %s\n", emsg);
        return st;
    }

    printf("Bindings:\n");
    for (i = 0; i < bvec->count; i++) {
        rpc_binding_to_string_binding(
            bvec->binding_h[i], &string_binding, &st
        );
        if (st != error_status_ok) {
            int tst;
            dce_error_inq_text(st, emsg, &tst);
            printf("Cannot get string binding - %s\n", emsg);
            return st;
        }
        printf("%s\n", string_binding);
        rpc_string_free(&string_binding, &tst);
    }


#ifndef _KERNEL
    ncstest_s_bvec = bvec;
#else
    rpc_binding_vector_free(&bvec, &st);
#endif

    return st;
}

/*
 * D O I T
 *
 * Actually do the server side of the test.
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
    ndr_boolean     debug;
    char            *dbgstr;
    char            *server;
    int             passes;
    int             failures, failure_total;
    extern          char *strrchr();
    int             auth_proto = rpc_c_authn_none;
    unsigned char   *auth_name;
    unsigned char   emsg[64];
    int             i;
    
    st = error_status_ok;
    ncstest_s_verbose = debug = use_reserved_threads = no_ep_register = ndr_false;

    argc--; argv++;

    while (argc && argv[0][0] == '-') {

        if ((strcmp(argv[0], "-p") == 0) &&
            (argc > 3))
        {
            auth_proto = atoi (argv[1]);
            auth_name = (unsigned char *)argv[2];
printf("auth_proto=%d, auth_name=%s.\n", auth_proto, auth_name);
            argv += 3;
            argc -= 3;
            continue;
        }
            
        ncstest_s_verbose = (strrchr(argv[0], 'v') != NULL);
        use_reserved_threads = (strrchr(argv[0], 'r') != NULL);
        debug = ((dbgstr = strrchr(argv[0], 'd')) != NULL);
        no_ep_register =  ((strrchr(argv[0], 'e')) != NULL);

        if (debug) {
            dbgstr++;
            if (*dbgstr == '\0')
                dbgstr = "0.1";
        }

        argv++, argc--;
    }

    if (debug) {
        rpc__dbg_set_switches((ndr_char *)dbgstr, &st);
        if (st != error_status_ok) {
            int tst;
            dce_error_inq_text(st, emsg, &tst);
            printf(
                "Error setting debug level %s - %s\n",
                dbgstr, emsg
            );
            return st;
        }
    }

    if (argc < 2) {
        usage();
        return -1;
    }

    MaxCalls = atoi(argv[0]);

    st = get_endpoints(argc-1, &argv[1]);
    if (st != error_status_ok)
        return st;

    st = register_ifs();
    if (st != error_status_ok)
        return st;

    if (auth_proto != rpc_c_authn_none) {
        printf("Registering auth info...\n");
        rpc_server_register_auth_info (auth_name, auth_proto, NULL, NULL, &st);
printf("server_register_auth_info return st=%d\n", st);
        if (st != error_status_ok)
            return st;
    }

    rpc_mgmt_set_authorization_fn(
                (rpc_mgmt_authorization_fn_t)ncstest_mgmt_auth, &st);
/*
 * printf("mgmt_set_authorization_fn returns st=%d\n", st);
 */
    if (st != error_status_ok)
        return st;

    setup_thread_pools();
    
    printf("Listening...\n");
    rpc_server_listen(MaxCalls, &st);
#ifdef _KERNEL
    /*
     * Someone may have already started up a listener...
     * It would be nice if we could wait around till we were
     * told to shutdown and then unregister our interfaces
     * (maybe someday)...
     */
    if (st == rpc_s_already_listening) {
        printf("listener already running (leaving interfaces registered)\n");
        return st;
    }
#endif
    printf("Shutdown; listen returned 0x%x\n", st);

/*
 *   if ((((int) st) = teardown_thread_pools()) < 0)
 */
    if (( st = teardown_thread_pools()) < 0)
        return st;

    st = unregister_ifs();

    return st;
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
        nbytes += (strlen(argv[i]) + 1);
        cp += nbytes;
    }

    return 0;
}

/*
 * N C S T E S T _ S _ D O I T
 *
 * Get the test server to run (called from the NCS test jig device driver).
 */

void ncstest_s_doit(inargs, outargs)
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
        nbytes += (strlen(argv[i]) + 1);
        if (nbytes > NCSTEST_MAX_ARGVBYTES) {
            printf("too many cmd line argv bytes: > %d\n", NCSTEST_MAX_ARGVBYTES);
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
 * Get the kernel to start the test server.
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

    if (ioctl(fd, NIOCNTS, &args) == -1)
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


