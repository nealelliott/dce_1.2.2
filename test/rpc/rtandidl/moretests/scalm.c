/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: scalm.c,v $
 * Revision 1.1.537.2  1996/02/17  23:22:03  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:03  marty]
 *
 * Revision 1.1.537.1  1995/12/11  20:09:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:51  root]
 * 
 * Revision 1.1.535.1  1994/01/21  22:40:26  cbrooks
 * 	RPC Code Cleanup-Initial Submission
 * 	[1994/01/21  20:16:52  cbrooks]
 * 
 * Revision 1.1.4.2  1993/08/27  20:38:02  tom
 * 	Bug 8517 - fix RIOS compile error on op11 decl.
 * 	[1993/08/27  20:19:19  tom]
 * 
 * Revision 1.1.2.3  1993/01/25  19:27:53  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:44:09  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:39:48  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:13:15  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:36:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#include <nidltest.h>
#include <scal.h>


/*
 * O P 1
 */
void scalartest_op1
#ifdef _DCE_PROTO_
(
handle_t h,
ndr_small_int i,
ndr_small_int *o
)
#else
(h, i, o)
handle_t h;
ndr_small_int i, *o; 
#endif
{
    *o = i + 1;
}


/*
 * O P 2
 */
void scalartest_op2
#ifdef _DCE_PROTO_
(
handle_t h,
ndr_usmall_int i,
ndr_usmall_int *o
)
#else
(h, i, o)
handle_t h;
ndr_usmall_int i, *o;
#endif
{
    *o = i + 1;
}


/*
 * O P 3
 */
void scalartest_op3
#ifdef _DCE_PROTO_
(
handle_t h,
ndr_short_int i,
ndr_short_int *o
)
#else
(h, i, o)
handle_t h;
ndr_short_int i, *o;
#endif
{
    *o = i + 1;
}


/*
 * O P 4
 */
void scalartest_op4
#ifdef _DCE_PROTO_
(
handle_t h,
ndr_ushort_int i,
ndr_ushort_int *o
)
#else
(h, i, o)
handle_t h;
ndr_ushort_int i, *o;
#endif
{
    *o = i + 1;
}


/*
 * O P 5
 */
void scalartest_op5
#ifdef __STDC__ 
(
handle_t h,
ndr_long_int i,
ndr_long_int *o
)
#else
(h, i, o)
handle_t h;
ndr_long_int i, *o;
#endif
{
    *o = i + 1;
}


/*
 * O P 6
 */
void scalartest_op6
#ifdef __STDC__ 
(
handle_t h,
ndr_ulong_int i,
ndr_ulong_int *o
)
#else
(h, i, o)
handle_t h;
ndr_ulong_int i, *o;
#endif
{
    *o = i + 1;
}


/*
 * O P 7
 */
void scalartest_op7
#ifdef __STDC__ 
(
handle_t h,
ndr_hyper_int i,
ndr_hyper_int *o
)
#else
(h, i, o)
handle_t h;
ndr_hyper_int i, *o;
#endif
{
#ifdef cray
    *o = i + 1;
#else
    if (i.low == 0xFFFFFFFF) {
        o->low = 0;
        o->high = i.high + 1;
    } else {
        o->low = i.low + 1;
        o->high = i.high;
    }
#endif
}


/*
 * O P 8
 */
void scalartest_op8
#ifdef __STDC__ 
(
handle_t h,
ndr_uhyper_int i,
ndr_uhyper_int *o
)
#else
(h, i, o)
handle_t h;
ndr_uhyper_int i, *o;
#endif
{
#ifdef cray
    *o = i + 1;
#else
    if (i.low == 0xFFFFFFFF) {
        o->low = 0;
        o->high = i.high + 1;
    } else {
        o->low = i.low + 1;
        o->high = i.high;
    }
#endif
}


/*
 * O P 9
 */
void scalartest_op9
#ifdef _DCE_PROTO_
(
handle_t h,
ndr_boolean i,
ndr_boolean *o
)
#else
(h, i, o)
handle_t h;
ndr_boolean i, *o;
#endif
{
    if (i) 
        *o = ndr_false;
    else
        *o = ndr_true;
}


/*
 * O P 1 0
 */
void scalartest_op10
#ifdef _DCE_PROTO_
(
handle_t h,
ndr_byte i,
ndr_byte *o
)
#else
(h, i, o)
handle_t h;
ndr_byte i, *o;
#endif
{
    *o = ~i;
}


/*
 * O P 1 1
 *
 * Note on the declaration of scalartest_op11:
 * 
 * Apparently, in some C implementations, a synopsis style header overrides
 * a prototype causing the compiled code to expect that floats have been
 * widened to doubles by the caller.  When the caller, in this case a NIDL
 * generated server stub routine, has been compiled with a prototype it
 * will not have widened the float causing conflict.  Ideally, all of
 * the manager routines would be cleanly declared with both a prototype
 * and a synopsis under the control of __STDC__ but life is short and
 * this is the only case in which it matters here.
 */
void scalartest_op11
#ifdef _DCE_PROTO_
(
handle_t h, 
ndr_short_float i, 
ndr_short_float *o
)
#else
(h, i, o)
handle_t h;
ndr_short_float i, *o;
#endif
{
    *o = i * i;
}


/*
 * O P 1 2
 */
void scalartest_op12
#ifdef _DCE_PROTO_
(
handle_t h, 
ndr_long_float i, 
ndr_long_float *o
)
#else
(h, i, o)
handle_t h;
ndr_long_float i, *o;
#endif
{
    *o = i * i;
}


/*
 * O P 1 3
 */
void scalartest_op13

#ifdef _DCE_PROTO_
(
handle_t h,
ndr_char i,
ndr_char *o
)
#else
(h, i, o)
handle_t h;
ndr_char i, *o;
#endif
{
    switch (i) {
        case 'a':
            *o = 'A';
            break;
        case 'b':
            *o = 'B';
            break;
        case 'c':
            *o = 'C';
            break;
        case 'd':
            *o = 'D';
            break;
        case 'e':
            *o = 'E';
            break;
        case 'f':
            *o = 'F';
            break;
        case 'g':
            *o = 'G';
            break;
        case 'h':
            *o = 'H';
            break;
        case 'i':
            *o = 'I';
            break;
        case 'j':
            *o = 'J';
            break;
        case 'k':
            *o = 'K';
            break;
        case 'l':
            *o = 'L';
            break;
        case 'm':
            *o = 'M';
            break;
        case 'n':
            *o = 'N';
            break;
        case 'o':
            *o = 'O';
            break;
        case 'p':
            *o = 'P';
            break;
        case 'q':
            *o = 'Q';
            break;
        case 'r':
            *o = 'R';
            break;
        case 's':
            *o = 'S';
            break;
        case 't':
            *o = 'T';
            break;
        case 'u':
            *o = 'U';
            break;
        case 'v':
            *o = 'V';
            break;
        case 'w':
            *o = 'W';
            break;
        case 'x':
            *o = 'X';
            break;
        case 'y':
            *o = 'Y';
            break;
        case 'z':
            *o = 'Z';
            break;
        case 'A':
            *o = 'a';
            break;
        case 'B':
            *o = 'b';
            break;
        case 'C':
            *o = 'c';
            break;
        case 'D':
            *o = 'd';
            break;
        case 'E':
            *o = 'e';
            break;
        case 'F':
            *o = 'f';
            break;
        case 'G':
            *o = 'g';
            break;
        case 'H':
            *o = 'h';
            break;
        case 'I':
            *o = 'i';
            break;
        case 'J':
            *o = 'j';
            break;
        case 'K':
            *o = 'k';
            break;
        case 'L':
            *o = 'l';
            break;
        case 'M':
            *o = 'm';
            break;
        case 'N':
            *o = 'n';
            break;
        case 'O':
            *o = 'o';
            break;
        case 'P':
            *o = 'p';
            break;
        case 'Q':
            *o = 'q';
            break;
        case 'R':
            *o = 'r';
            break;
        case 'S':
            *o = 's';
            break;
        case 'T':
            *o = 't';
            break;
        case 'U':
            *o = 'u';
            break;
        case 'V':
            *o = 'v';
            break;
        case 'W':
            *o = 'w';
            break;
        case 'X':
            *o = 'x';
            break;
        case 'Y':
            *o = 'y';
            break;
        case 'Z':
            *o = 'z';
            break;
        default:
            *o = i;
    }
}
