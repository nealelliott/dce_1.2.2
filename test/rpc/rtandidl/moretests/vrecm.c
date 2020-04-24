/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: vrecm.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:40  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:13:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:24  root]
 * 
 * Revision 1.1.4.2  1993/08/27  20:38:08  tom
 * 	Bug 8518 - Rios doesn't define STDC, so we use IDL_PROTOTYPES.
 * 	[1993/08/27  20:19:23  tom]
 * 
 * Revision 1.1.2.3  1993/01/25  19:30:47  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:48:02  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:51:09  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:18:18  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:14  devrcs
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
# Revision 1.1  91/09/11  16:16:04  marco
# Initial revision
# 
*/



#include <vrec.h>

#ifdef vaxc
/*
 * s c a n _ a _ f l o a t
 *
 * routine is used in place of sscanf because sscanf on VMS 4.7 didn't
 * seem to work on float values
 */
ndr_char *scan_a_float
#ifndef __STDC__
    (s, f) 
ndr_char *s;
ndr_short_float *f;
#else
  (
ndr_char *s,
ndr_short_float *f
  )
#endif
{
    ndr_short_float mag, c, sign;
    ndr_boolean point_seen;

    while (*s == ' ')
        s++;

    switch (*s) {
        case '-':
            sign = -1.0;
            s++;
            break;
        case '+':
            s++;
        default:
            sign = 1.0;
    }
    mag = 0.0;
    point_seen = false;
    c = 1.0;
    while (true) {
        if (*s == '.') {
            point_seen = true;
            s++;
        }

        if (!point_seen)
            mag *= 10.0;
        else
            c = c / 10.0;

        switch (*s) {
            case '0':
                break;
            case '1':
                mag = mag + c;
                break;
            case '2':
                mag = mag + 2.0 * c;
                break;
            case '3':
                mag = mag + 3.0 * c;
                break;
            case '4':
                mag = mag + 4.0 * c;
                break;
            case '5':
                mag = mag + 5.0 * c;
                break;
            case '6':
                mag = mag + 6.0 * c;
                break;
            case '7':
                mag = mag + 7.0 * c;
                break;
            case '8':
                mag = mag + 8.0 * c;
                break;
            case '9':
                mag = mag + 9.0 * c;
                break;

            default:
                *f = mag * sign;
                return s;
        }
        s++;
    }
}
#endif


/*
 * v a r r e c t e s t _ o p 1
 */
void varrectest_op1
/*
#ifndef __STDC__
*/
#ifndef IDL_PROTOTYPES
    (h, i, o)
handle_t h;
varrectest_t1 i;
varrectest_t1 *o;
#else
  (
handle_t h,
varrectest_t1 i, 
varrectest_t1 *o
  )
#endif
{
    ndr_short_float x, y;
    ndr_char *cursor;


    switch (i.tag) {
        case 0:
        case 2:
            o->tag = 1;
            o->tagged_union.case_1.x = i.tagged_union.case_0.a;
            o->tagged_union.case_1.y = i.tagged_union.case_0.b;
            break;
        case 1:
            o->tag = 3;
            sprintf (o->tagged_union.s, "%f %f",
                     i.tagged_union.case_1.x,
                     i.tagged_union.case_1.y);
            break;
        case 3:
            o->tag = 0;
#ifdef vaxc
            scan_a_float (scan_a_float (i.tagged_union.s, &x), &y);
#else
            sscanf (i.tagged_union.s, "%f %f", &x, &y);
#endif
            o->tagged_union.case_0.a = x;
            o->tagged_union.case_0.b = y;
            break;
    }
}


/*
 * v a r r e c t e s t _ o p 2
 */
void varrectest_op2
/*
#ifndef __STDC__
*/
#ifndef IDL_PROTOTYPES
    (h, i, o)
handle_t h;
varrectest_t2 i;
varrectest_t2 *o;
#else
  (
handle_t h,
varrectest_t2 i,
varrectest_t2 *o
  )
#endif
{
    ndr_short_float x, y;


    switch (i.tag) {
        case 0:
        case 2:
            o->tag = 1;
            o->tu.c1.x = i.tu.c0.a;
            o->tu.c1.y = i.tu.c0.b;
            break;
        case 1:
            o->tag = 3;
            sprintf (o->tu.s, "%f %f",
                     i.tu.c1.x,
                     i.tu.c1.y);
            break;
        case 3:
            o->tag = 0;
#ifdef vaxc
            scan_a_float (scan_a_float (i.tu.s, &x), &y);
#else
            sscanf (i.tu.s, "%f %f", &x, &y);
#endif
            o->tu.c0.a = x;
            o->tu.c0.b = y;
            break;
    }
}


/*
 * v a r r e c t e s t _ o p 3
 */
void varrectest_op3
/*
#ifndef __STDC__
*/
#ifndef IDL_PROTOTYPES
    (h, i)
handle_t h;
varrectest_t1 i;
#else
  (
handle_t h,
varrectest_t1 i
  )
#endif
{
    ndr_short_float x, y;

}


/*
 * v a r r e c t e s t _ o p 4
 */
void varrectest_op4
/*
#ifndef __STDC__
*/
#ifndef IDL_PROTOTYPES
    (h, tag_to_use, o)
handle_t h;
idl_short_int tag_to_use;
varrectest_t1 *o;
#else
  (
handle_t h,
idl_short_int tag_to_use,
varrectest_t1 *o
  )
#endif


{
    ndr_short_float x, y;

    o->tag = tag_to_use;
}
