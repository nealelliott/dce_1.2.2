/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: scal_mgr.c,v $
 * Revision 1.1.10.2  1996/03/09  20:51:33  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:19  marty]
 *
 * Revision 1.1.10.1  1995/12/13  21:55:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:23  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:18:22  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:17:54  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:36:56  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.3  1992/11/04  13:04:13  mgm
 * 	Had to ifdef IDL_PROTOTYPES in ops 1 2 3 4 9 10 11 13
 * 	[1992/11/04  13:03:16  mgm]
 * 
 * Revision 1.1.3.2  1992/06/11  19:58:17  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:05:17  mcfarland]
 * 
 * 	Fix idl parms
 * 	[1992/03/18  00:12:57  rps]
 * 
 * Revision 1.1.1.2  1992/05/28  15:14:39  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:36:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 
 * Copyright 1989 by Apollo Computer, Inc.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 * 
 */
/* @(#)scalm.c	1.1
 *  NIDL TEST 
 *	Extracted	1/31/91 16:33:00
 *	from delta on	1/31/91 16:30:59
 */
#ifndef LINT
static char sccs_id[] = "@(#)scalm.c	1.1";
#endif

#include <scal.h>


/*
 * O P 1
 */
void scalartest_op1
#ifdef IDL_PROTOTYPES
(
	handle_t h,
	idl_small_int i, 
	idl_small_int *o
)
#else
(h, i, o)
#endif

#ifndef IDL_PROTOTYPES
handle_t h;
idl_small_int i;
idl_small_int *o;
#endif
{
    *o = i + 1;
}


/*
 * O P 2
 */
void scalartest_op2
#ifdef IDL_PROTOTYPES
(
	handle_t h,
	idl_usmall_int i, 
	idl_usmall_int *o
)
#else
(h, i, o)
#endif

#ifndef IDL_PROTOTYPES
handle_t h;
idl_usmall_int i;
idl_usmall_int *o;
#endif
{
    *o = i + 1;
}


/*
 * O P 3
 */
void scalartest_op3
#ifdef IDL_PROTOTYPES
(
        handle_t h,
        idl_short_int i,
        idl_short_int *o
)
#else
(h, i, o)
#endif

#ifndef IDL_PROTOTYPES
handle_t h;
idl_short_int i;
idl_short_int *o;
#endif
{
    *o = i + 1;
}


/*
 * O P 4
 */
void scalartest_op4
#ifdef IDL_PROTOTYPES
(
        handle_t h,
        idl_ushort_int i,
        idl_ushort_int *o
)
#else
(h, i, o)
#endif

#ifndef IDL_PROTOTYPES
handle_t h;
idl_ushort_int i;
idl_ushort_int *o;
#endif
{
    *o = i + 1;
}


/*
 * O P 5
 */
void scalartest_op5(
handle_t h,
idl_long_int i, idl_long_int *o)
{
    *o = i + 1;
}


/*
 * O P 6
 */
void scalartest_op6(
handle_t h,
idl_ulong_int i, idl_ulong_int *o)
{
    *o = i + 1;
}


/*
 * O P 7
 */
void scalartest_op7(
handle_t h,
idl_hyper_int i, idl_hyper_int *o)
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
void scalartest_op8(
handle_t h,
idl_uhyper_int i, idl_uhyper_int *o)
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
#ifdef IDL_PROTOTYPES
(
        handle_t h,
        idl_boolean i,
        idl_boolean *o
)
#else
(h, i, o)
#endif

#ifndef IDL_PROTOTYPES
handle_t h;
idl_boolean i;
idl_boolean *o;
#endif
{
    if (i) 
        *o = idl_false;
    else
        *o = idl_true;
}


/*
 * O P 1 0
 */
void scalartest_op10
#ifdef IDL_PROTOTYPES
(
        handle_t h,
        idl_byte i,
        idl_byte *o
)
#else
(h, i, o)
#endif

#ifndef IDL_PROTOTYPES
handle_t h;
idl_byte i;
idl_byte *o;
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
#ifdef IDL_PROTOTYPES
(
        handle_t h,
        idl_short_float i,
        idl_short_float *o
)
#else
(h, i, o)
#endif

#ifndef IDL_PROTOTYPES
handle_t h;
idl_short_float i;
idl_short_float *o;
#endif
{
    *o = i * i;
}


/*
 * O P 1 2
 */
void scalartest_op12(
handle_t h,
idl_long_float i, idl_long_float *o)
{
    *o = i * i;
}


/*
 * O P 1 3
 */
void scalartest_op13
#ifdef IDL_PROTOTYPES
(
        handle_t h,
        idl_char i,
        idl_char *o
)
#else
(h, i, o)
#endif

#ifndef IDL_PROTOTYPES
handle_t h;
idl_char i;
idl_char *o;
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
