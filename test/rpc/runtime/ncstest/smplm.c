/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: smplm.c,v $
 * Revision 1.1.738.2  1996/02/17  23:23:43  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:11  marty]
 *
 * Revision 1.1.738.1  1995/12/11  20:14:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:36  root]
 * 
 * Revision 1.1.736.1  1994/01/21  22:40:29  cbrooks
 * 	RPC Code Cleanup-Initial Submission
 * 	[1994/01/21  20:17:17  cbrooks]
 * 
 * Revision 1.1.6.1  1993/10/15  17:00:29  tom
 * 	Bug 8303 - remove semi-colons after brace.
 * 	Remove static from routine declarations.
 * 	[1993/10/14  22:48:57  tom]
 * 
 * Revision 1.1.4.2  1993/01/13  19:55:46  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:23:26  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  13:49:36  rsalz
 * 	Added as part of rpc6 drop.
 * 	[1992/04/30  23:23:45  rsalz]
 * 
 * Revision 1.2  1992/01/19  22:14:16  devrcs
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
 * ncstest_simple manager
 */

#include <dce/dce.h>
#include <ncstest.h>
#include <smpl.h>

#ifdef _KERNEL
/*
 * This hackery is necessary to use the kernel EXCEPTION_INIT macro.
 */
#  define lcl_exc_id  -1
#endif

#define VERBOSE(str) \
    { \
        globalref int ncstest_s_verbose; \
        if (ncstest_s_verbose) \
            printf("ncstest_simple_mgr: %s() %s\n", which_call, str); \
    }

#define FAILURE(str) \
    { \
        failures++; \
        printf("ncstest_simple_mgr: %s() - %s\n", which_call, str); \
    }

static cvt_case(i)
int i;
{
    char    o;

    /*
     * may as well do the right thing for all char sets
     * (and not depend on clib macros/routines).
     */
    switch (i) {
        case 'a': o = 'A'; break;
        case 'b': o = 'B'; break;
        case 'c': o = 'C'; break;
        case 'd': o = 'D'; break;
        case 'e': o = 'E'; break;
        case 'f': o = 'F'; break;
        case 'g': o = 'G'; break;
        case 'h': o = 'H'; break;
        case 'i': o = 'I'; break;
        case 'j': o = 'J'; break;
        case 'k': o = 'K'; break;
        case 'l': o = 'L'; break;
        case 'm': o = 'M'; break;
        case 'n': o = 'N'; break;
        case 'o': o = 'O'; break;
        case 'p': o = 'P'; break;
        case 'q': o = 'Q'; break;
        case 'r': o = 'R'; break;
        case 's': o = 'S'; break;
        case 't': o = 'T'; break;
        case 'u': o = 'U'; break;
        case 'v': o = 'V'; break;
        case 'w': o = 'W'; break;
        case 'x': o = 'X'; break;
        case 'y': o = 'Y'; break;
        case 'z': o = 'Z'; break;
        case 'A': o = 'a'; break;
        case 'B': o = 'b'; break;
        case 'C': o = 'c'; break;
        case 'D': o = 'd'; break;
        case 'E': o = 'e'; break;
        case 'F': o = 'f'; break;
        case 'G': o = 'g'; break;
        case 'H': o = 'h'; break;
        case 'I': o = 'i'; break;
        case 'J': o = 'j'; break;
        case 'K': o = 'k'; break;
        case 'L': o = 'l'; break;
        case 'M': o = 'm'; break;
        case 'N': o = 'n'; break;
        case 'O': o = 'o'; break;
        case 'P': o = 'p'; break;
        case 'Q': o = 'q'; break;
        case 'R': o = 'r'; break;
        case 'S': o = 's'; break;
        case 'T': o = 't'; break;
        case 'U': o = 'u'; break;
        case 'V': o = 'v'; break;
        case 'W': o = 'w'; break;
        case 'X': o = 'x'; break;
        case 'Y': o = 'y'; break;
        case 'Z': o = 'z'; break;
        default: o = i; break;
    }

    return o;
}

/*
 *  n c s t e s t _ s i m p l e _ o p 1
 *
 *  return a mutation of input arg:
 *      invert the boolean
 *      negate the various size ints
 *      case convert the char array
 *  the function return value is the sum of the integers in the input.
 */
long ncstest_simple_op1
#ifdef _DCE_PROTOTYPE_
        (handle_t h, ncstest_simple_t1 *t1_in, ncstest_simple_t1 *t1_out)
#else
        (h, t1_in, t1_out)
handle_t h;
ncstest_simple_t1  *t1_in;
ncstest_simple_t1  *t1_out;
#endif
{
    long res;
    int i;
    char *which_call = "ncstest_simple_op1";

    VERBOSE("");

    res = t1_in->i8 + t1_in->i16 + t1_in->i32;

    t1_out->b = t1_in->b ? false : true;
    t1_out->i8 = -t1_in->i8;
    t1_out->i16 = -t1_in->i16;
    t1_out->i32 = -t1_in->i32;
    for (i=0; i<T1_C_LEN; i++)
        t1_out->c[i] = cvt_case(t1_in->c[i]);

    return res;
}

/*
 *  n c s t e s t _ s i m p l e _ o p 2
 *
 * Raise an exception as indicated by the flag.
 * There is no ACF spec for this operation so a remote exception
 * should be reraised locally.
 */
error_status_t ncstest_simple_op2
#ifdef _DCE_PROTOTYPE_
        ( handle_t h, idl_boolean raise_exception)
#else
        (h, raise_exception)
handle_t h;
ndr_boolean raise_exception;
#endif
{
    EXCEPTION    lcl_exc;
    char *which_call = "ncstest_simple_op2";

    VERBOSE("");

    EXCEPTION_INIT(lcl_exc);

    if (raise_exception) {
        RAISE(lcl_exc);
        printf("ncstest_simple_op2: RAISE failed\n");
        return 0x0000dead;
    }

    return error_status_ok;
}

/*
 *  n c s t e s t _ s i m p l e _ o p 3
 *
 * Raise an exception as indicated by the flag.
 * The ACF declares the function return to have [comm_status, fault_status].
 */
error_status_t ncstest_simple_op3
#ifdef _DCE_PROTOTYPE_
        (handle_t h, idl_boolean raise_exception)
#else
        (h, raise_exception)
handle_t h;
ndr_boolean raise_exception;
#endif
{
    EXCEPTION    lcl_exc;
    char *which_call = "ncstest_simple_op3";

    VERBOSE("");

    EXCEPTION_INIT(lcl_exc);

    if (raise_exception) {
        RAISE(lcl_exc);
        printf("ncstest_simple_op3: RAISE failed\n");
        return 0x0000dead;
    }

    return error_status_ok;
}

globaldef ncstest_simple_v1_0_epv_t ncstest_simple_manager_epv = {
    ncstest_simple_op1,
    ncstest_simple_op2,
    ncstest_simple_op3
};
