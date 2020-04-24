/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: smplt.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:45  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:12  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:38  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:55:51  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:23:32  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  13:49:42  rsalz
 * 	Added as part of rpc6 drop.
 * 	[1992/04/30  23:23:50  rsalz]
 * 
 * Revision 1.2  1992/01/19  22:14:17  devrcs
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
 * ncstest_simple client
 *
 * Establish a baseline of functionality...
 *  check basic arg passing
 *  check basic exception and non-[{comm,fault}_status] handling
 *  check basic [fault_status] exception handling
 *  check basic [comm_status] exception handling
 */


#include <ncstest.h>
#include <smpl.h>

#define VERBOSE(str) \
    { \
        globalref int ncstest_c_verbose; \
        if (ncstest_c_verbose) \
            printf("ncstest_simple_clt: %s() %s\n", which_call, str); \
    }

#define FAILURE(str) \
    { \
        failures++; \
        printf("ncstest_simple_clt: %s() - %s\n", which_call, str); \
    }

static cvt_case(i)
int i;
{
    unsigned char    o;

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

int ncstest_simple_clt(h)
rpc_binding_handle_t h;
{
    ncstest_simple_t1   t1_in, t1_out;
    int failures = 0;
    int i;
    error_status_t st;
    char *which_call;
    int give_up = false;

    /*
     * Perform a simple call with checking.  Since this call
     * isn't using comm_status, we setup a cleanup handler
     * to ensure that a RAISE isn't propagated any further.
     */

    t1_in.b = true;
    t1_in.i8 = 37;
    t1_in.i16 = 23095;
    t1_in.i32 = -59;
    for (i = 0; i < T1_C_LEN; i++)
        t1_in.c[i] = "JustTesting"[i];

    which_call = "ncstest_simple_op1";
    VERBOSE("happening now");
    TRY {
        if (ncstest_simple_op1(h, &t1_in, &t1_out) != 
            (t1_in.i8 + t1_in.i16 + t1_in.i32))
            FAILURE("incorrect function return value");

        if (t1_out.b != (t1_in.b ? false : true))
            FAILURE("incorrect boolean result");

        if (t1_out.i8 != -t1_in.i8)
            FAILURE("incorrect i8 result");

        if (t1_out.i16 != -t1_in.i16)
            FAILURE("incorrect i16 result");

        if (t1_out.i32 != -t1_in.i32)
            FAILURE("incorrect i32 result");

        for (i = 0; i < T1_C_LEN; i++)
            if (t1_out.c[i] != cvt_case(t1_in.c[i]))
                FAILURE("incorrect char result");
    } CATCH (rpc_x_comm_failure) {
        FAILURE("comm_failure exception raised - giving up");
        give_up = true;
    } CATCH_ALL {
        FAILURE("exception incorrectly raised - giving up");
        give_up = true;
    } ENDTRY
    if (give_up)
        goto DONE;

    /*
     * Perform some simple tests to check non-[{comm,fault}_status] 
     * exception handling.
     */

    which_call = "ncstest_simple_op2";
    VERBOSE("(no fault) happening now");
    TRY {
        /*
         * Don't generate an exception.
         */
        st = ncstest_simple_op2(h, false);
        if (st != error_status_ok)
            FAILURE("(no fault) - wrong function return");
    } CATCH (rpc_x_comm_failure) {
        FAILURE("comm_failure exception raised - giving up");
        give_up = true;
    } CATCH_ALL {
        FAILURE("(no fault) - exception incorrectly raised");
    } ENDTRY
    if (give_up)
        goto DONE;

    which_call = "ncstest_simple_op2";
    VERBOSE("(no fault) happening now");
    TRY {
        /*
         * Generate an exception.
         */
        st = ncstest_simple_op2(h, true);
        FAILURE("op2 (fault) - exception not raised");
    } CATCH(rpc_x_unknown_remote_fault) {
        /* this is what's suppose to happen */
    } CATCH (rpc_x_comm_failure) {
        FAILURE("comm_failure exception raised - giving up");
        give_up = true;
    } CATCH_ALL {
        FAILURE("op2 (fault) - wrong exception raised");
    } ENDTRY
    if (give_up)
        goto DONE;


    /*
     * Perform some simple tests to check [fault_status] 
     * exception handling.
     */

    which_call = "ncstest_simple_op3";
    VERBOSE("(no fault) happening now");
    TRY {
        /*
         * Don't generate an exception.
         */
        st = ncstest_simple_op3(h, false);
        if (st != error_status_ok)
            FAILURE("op3 (no fault) - wrong function return");
    } CATCH (rpc_x_comm_failure) {
        FAILURE("comm_failure exception raised - giving up");
        give_up = true;
    } CATCH_ALL {
        FAILURE("op3 (no fault) - exception incorrectly raised");
    } ENDTRY
    if (give_up)
        goto DONE;

    which_call = "ncstest_simple_op3";
    VERBOSE("(fault) happening now");
    TRY {
        /*
         * Generate an exception.
         */
        st = ncstest_simple_op3(h, true);
        if (st != rpc_s_fault_unspec)
            FAILURE("op3 (fault) - wrong function return");
    } CATCH (rpc_x_comm_failure) {
        FAILURE("comm_failure exception raised - giving up");
        give_up = true;
    } CATCH_ALL {
        FAILURE("op3 (fault) - exception incorrectly raised");
    } ENDTRY
    if (give_up)
        goto DONE;


    /*
     * Perform some simple tests to check [comm_status] 
     * exception handling (e.g. unknown interface)
     */

    which_call = "ncstest_unk_if_op1";
    VERBOSE("happening now");
    TRY {
        st = ncstest_unk_if_op1(h);
        if (st != rpc_s_unknown_if)
            FAILURE("unk_if_op1 - wrong function return");
    } CATCH (rpc_x_comm_failure) {
        FAILURE("comm_failure exception raised - giving up");
        give_up = true;
    } CATCH_ALL {
        FAILURE("unk_if_op1 - exception incorrectly raised");
    } ENDTRY

DONE:
    return failures;
}
