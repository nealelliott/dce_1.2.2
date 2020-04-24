/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kclient.c,v $
 * Revision 1.1.4.2  1996/02/17  23:23:26  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:56  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:14:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:05  root]
 * 
 * Revision 1.1.2.2  1993/01/13  19:53:38  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:21:14  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:42:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <ncstest.h>
#include <sys/errno.h>
#include <sys/user.h>

extern int doit();
static int argc;
static char *argv[NCSTEST_MAX_ARGC];


/*
 * I N A R G S _ T O _ A R G S
 *
 * Create argc,argv from the inargs.
 */
static int inargs_to_args(args)
ncstest_inargs_t *args;
{
    int i, nbytes;
    char *cp;

    argc = args->argc;
    if (argc > NCSTEST_MAX_ARGC) {
        printf("too many cmd line args: %d\n", argc);
        return -1;
    }

    cp = &args->argvchars[0];
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
int ncstest_c_doit(inargs, outargs)
ncstest_inargs_t    *inargs;
ncstest_outargs_t   *outargs;
{
    int r;
    ncstest_inargs_t    locinargs;
    ncstest_outargs_t   locoutargs;

    if ((copyin((caddr_t) inargs, (caddr_t) &locinargs, sizeof(ncstest_inargs_t))) != 0)
    {
       u.u_error = EFAULT;
       return -1;
    }
    if ((copyin((caddr_t) outargs, (caddr_t) &locoutargs, sizeof(ncstest_outargs_t))) != 0)
    {
       u.u_error = EFAULT;
       return -1;
    }

    r = inargs_to_args(&locinargs);
    if (r == -1) {
        locoutargs.st = -1;
        return;
    }

    if ((r = pincode(doit)))
       printf("pincode returned bad status\n");

    r = doit(argc, argv);
    locoutargs.st = (int) r;
    if ((copyout((caddr_t) &locoutargs, (caddr_t) outargs, sizeof(ncstest_outargs_t))) != 0)
    {
       u.u_error = EFAULT;
       return -1;
    }
}
