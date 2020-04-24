/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: arym.c,v $
 * Revision 1.1.6.2  1996/02/17  23:20:00  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:38:37  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:02:48  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/11/14  21:00 UTC  dat
 * 	Remove malloc prototype
 * 	[1995/12/11  19:42:44  root]
 * 
 * Revision 1.1.4.3  1993/01/25  18:03:31  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:34:32  bbelch]
 * 
 * Revision 1.1.4.2  1993/01/11  22:11:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:03:34  bbelch]
 * 
 * Revision 1.1.2.2  1992/07/14  21:40:21  cuti
 * 	Include scal.h to avoid short float mismatch problem.
 * 	[1992/07/13  21:20:12  cuti]
 * 
 * Revision 1.1  1992/01/19  03:34:43  devrcs
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



#include <stdio.h>
#include <ary.h>
#include <scal.h>
#include <nidltest.h> 

/*
 * a r r a y t e s t _ o p 1
 */
void arraytest_op1
#ifndef __STDC__
    (h, i, o)
handle_t h;
arraytest_t1 i;
arraytest_t1 o;
#else
  (
handle_t h,
arraytest_t1 i,
arraytest_t1 o
  )
#endif
{
    ndr_long_int indx;
    
    for (indx=0; indx<arraytest_c1; indx++)
        o[indx] = i[indx] * i[indx];
}


/*
 * a r r a y t e s t _ o p 2
 */
void arraytest_op2
#ifndef __STDC__
    (h, i, o, l_i, l_o)
handle_t h;
arraytest_t1 i;
arraytest_t1 o;
ndr_long_int l_i;
ndr_long_int *l_o;
#else
  (
handle_t h,
arraytest_t1 i, 
arraytest_t1 o,
ndr_long_int l_i,
ndr_long_int *l_o
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<=l_i; indx++)
        o[indx] = i[indx] * i[indx];
    *l_o = l_i;
}


/*
 * a r r a y t e s t _ o p 3
 */
void arraytest_op3
#ifndef __STDC__
    (h, i, o, l_i, m_i, l_o, m_o)
handle_t h;
arraytest_t2 i, o;
ndr_long_int l_i;
ndr_long_int m_i;
ndr_long_int *l_o;
ndr_long_int m_o;
#else
  (
handle_t h,
arraytest_t2 i,
arraytest_t2 o,
ndr_long_int l_i,
ndr_long_int m_i,
ndr_long_int *l_o,
ndr_long_int m_o
  )
#endif
{
    ndr_long_int indx;
    
    for (indx=0; indx<=m_o; indx++)
        o[indx] = i[indx] * i[indx];
    *l_o = l_i;
}


/*
 * a r r a y t e s t _ o p 4
 */
void arraytest_op4
#ifndef __STDC__
    (h, io)
handle_t h;
arraytest_t1 io;
#else
  (
handle_t h,
arraytest_t1 io
  )
#endif
{
    ndr_long_int indx;
    
    for (indx=0; indx<arraytest_c1; indx++)
        io[indx] = io[indx] * io[indx];
}


/*
 * a r r a y t e s t _ o p 5
 */
void arraytest_op5
#ifndef __STDC__
    (h, io, l_io)
handle_t h;
arraytest_t1 io;
ndr_long_int *l_io;
#else
  (
handle_t h,
arraytest_t1 io,
ndr_long_int *l_io
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<=*l_io; indx++)
        io[indx] = io[indx] * io[indx];
}


/*
 * a r r a y t e s t _ o p 6
 */
void arraytest_op6
#ifndef __STDC__
    (h, io, l_io, m_io)
handle_t h;
arraytest_t2 io;
ndr_long_int *l_io;
ndr_long_int m_io;
#else
  (
handle_t h,
arraytest_t2 io,
ndr_long_int *l_io,
ndr_long_int m_io
  )
#endif
{
    ndr_long_int indx;
    
    for (indx=0; indx<=*l_io; indx++)
        io[indx] = io[indx] * io[indx];
}


arraytest_t1 op8_outs;

/*
 * a r r a y t e s t _ o p 7
 */
void arraytest_op7
#ifndef __STDC__
    (h, i)
handle_t h;
arraytest_t1 i;
#else
  (
handle_t h,
arraytest_t1 i
  )
#endif
{
    ndr_long_int indx;
    
    for (indx=0; indx<arraytest_c1; indx++)
        op8_outs[indx] = i[indx] * i[indx];
}


/*
 * a r r a y t e s t _ o p 8
 */
void arraytest_op8
#ifndef __STDC__
    (h, o)
handle_t h;
arraytest_t1 o;
#else
  (
handle_t h,
arraytest_t1 o
  )
#endif
{
    ndr_long_int indx;
    
    for (indx=0; indx<arraytest_c1; indx++)
        o[indx] = op8_outs[indx];
}


struct {
    arraytest_t1 a;
    ndr_long_int l;
} op10_outs;


/*
 * a r r a y t e s t _ o p 9
 */
void arraytest_op9
#ifndef __STDC__
    (h, i, l_i)
handle_t h;
arraytest_t1 i;
ndr_long_int l_i;
#else
  (
handle_t h,
arraytest_t1 i,
ndr_long_int l_i
  )
#endif
{
    ndr_long_int indx;
    
    for (indx=0; indx<=l_i; indx++)
        op10_outs.a[indx] = i[indx] * i[indx];
    op10_outs.l = l_i;
}


/*
 * a r r a y t e s t _ o p 1 0
 */
void arraytest_op10
#ifndef __STDC__
    (h, o, l_o)
handle_t h;
arraytest_t1 o;
ndr_long_int *l_o;
#else
  (
handle_t h,
arraytest_t1 o,
ndr_long_int *l_o
  )
#endif
{
    ndr_long_int indx;
    
    for (indx=0; indx<=op10_outs.l; indx++)
        o[indx] = op10_outs.a[indx];
    *l_o = op10_outs.l;
}


struct {
    ndr_long_int *a;
    ndr_long_int l;
} op12_outs = {
    NULL,
    -1
};


/*
 * a r r a y t e s t _ o p 1 1
 */
void arraytest_op11
#ifndef __STDC__
    (h, l_i, m_o, i)
handle_t h;
ndr_long_int l_i;
ndr_long_int m_o;
arraytest_t2 i;
#else
  (
handle_t h,
ndr_long_int l_i,
ndr_long_int m_o,
arraytest_t2 i
  )
#endif
{
    ndr_long_int indx;
    
    if (op12_outs.l < l_i) {
        if (op12_outs.l > -1)
            free (op12_outs.a);
        op12_outs.a = (ndr_long_int *) malloc((l_i + 1) * sizeof(ndr_long_int));
    }
    for (indx=0; indx<=l_i; indx++)
        op12_outs.a[indx] = i[indx] * i[indx];
    op12_outs.l = l_i;
}


/*
 * a r r a y t e s t _ o p 1 2
 */
void arraytest_op12
#ifndef __STDC__
    (h, l_o, m_o, o)
handle_t h;
ndr_long_int *l_o;
ndr_long_int m_o;
arraytest_t2 o;
#else
  (
handle_t h,
ndr_long_int *l_o,
ndr_long_int m_o,
arraytest_t2 o
  )
#endif
{
    ndr_long_int indx;
    
    for (indx=0; indx<=m_o; indx++)
        o[indx] = op12_outs.a[indx];
    *l_o = op12_outs.l;
}


/*
 * a r r a y t e s t _ o p 1 3
 */
void arraytest_op13
#ifndef __STDC__
    (h, i, o)
handle_t h;
arraytest_t3 i;
arraytest_t5 o;
#else
  (
handle_t h,
arraytest_t3 i,
arraytest_t5 o
  )
#endif
{
    ndr_long_int indx1, indx2;
    
    for (indx1=0; indx1<arraytest_c2; indx1++)
        for (indx2=0; indx2<arraytest_c3; indx2++)
            o[indx1][indx2] = i[indx1][indx2] * i[indx1][indx2];
}


#ifdef BROKEN_CODE

/*
 * a r r a y t e s t _ o p 1 4
 */
void arraytest_op14
#ifndef __STDC__
    (h, i, o, l_i, l_o)
handle_t h;
arraytest_t5 i;
arraytest_t3 o;
ndr_long_int l_i;
ndr_long_int *l_o;
#else
  (
handle_t h,
arraytest_t5 i,
arraytest_t3 o,
ndr_long_int l_i,
ndr_long_int *l_o
  )
#endif
{
    ndr_long_int indx1, indx2;
    
    for (indx1=0; indx1<=l_i; indx1++)
        for (indx2=0; indx2<arraytest_c3; indx2++)
            o[indx1][indx2] = i[indx1][indx2] * i[indx1][indx2];
    *l_o = l_i;
}


/*
 * a r r a y t e s t _ o p 1 5
 */
void arraytest_op15
#ifndef __STDC__
    (h, i, o, l_i, m_i, l_o, m_o)
handle_t h;
arraytest_t6 i;
arraytest_t7 o;
ndr_long_int l_i;
ndr_long_int m_i; 
ndr_long_int *l_o;
ndr_long_int m_o;
#else
  (
handle_t h,
arraytest_t6 i,
arraytest_t7 o,
ndr_long_int l_i,
ndr_long_int m_i, 
ndr_long_int *l_o,
ndr_long_int m_o
  )
#endif
{
    ndr_long_int indx1, indx2;
    
    for (indx1=0; indx1<=m_o; indx1++)
        for (indx2=0; indx2<arraytest_c3; indx2++)
            o[indx1][indx2] = i[indx1][indx2] * i[indx1][indx2];
    *l_o = l_i;
}

#endif

/*
 * a r r a y t e s t _ o p 2 0
 */
void arraytest_op20
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_small_int i[arraytest_c1];
ndr_small_int o[arraytest_c1];
#else
  (
handle_t h,
ndr_small_int i[arraytest_c1],
ndr_small_int o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op1(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 2 1
 */
void arraytest_op21
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_usmall_int i[arraytest_c1];
ndr_usmall_int o[arraytest_c1];
#else
  (
handle_t h,
ndr_usmall_int i[arraytest_c1],
ndr_usmall_int o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op2(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 2 2
 */
void arraytest_op22
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_short_int i[arraytest_c1];
ndr_short_int o[arraytest_c1];
#else
  (
handle_t h,
ndr_short_int i[arraytest_c1],
ndr_short_int o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op3(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 2 3
 */
void arraytest_op23
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_ushort_int i[arraytest_c1];
ndr_ushort_int o[arraytest_c1];
#else
  (
handle_t h,
ndr_ushort_int i[arraytest_c1],
ndr_ushort_int o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op4(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 2 4
 */
void arraytest_op24
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_long_int i[arraytest_c1];
ndr_long_int o[arraytest_c1];
#else
  (
handle_t h,
ndr_long_int i[arraytest_c1],
ndr_long_int o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op5(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 2 5
 */
void arraytest_op25
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_ulong_int i[arraytest_c1];
ndr_ulong_int o[arraytest_c1];
#else
  (
handle_t h,
ndr_ulong_int i[arraytest_c1],
ndr_ulong_int o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op6(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 2 6
 */
void arraytest_op26
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_hyper_int i[arraytest_c1];
ndr_hyper_int o[arraytest_c1];
#else
  (
handle_t h,
ndr_hyper_int i[arraytest_c1],
ndr_hyper_int o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op7(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 2 7
 */
void arraytest_op27
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_uhyper_int i[arraytest_c1];
ndr_uhyper_int o[arraytest_c1];
#else
  (
handle_t h,
ndr_uhyper_int i[arraytest_c1],
ndr_uhyper_int o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op8(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 2 8
 */
void arraytest_op28
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_boolean i[arraytest_c1];
ndr_boolean o[arraytest_c1];
#else
  (
handle_t h,
ndr_boolean i[arraytest_c1],
ndr_boolean o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op9(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 2 9
 */
void arraytest_op29
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_byte i[arraytest_c1];
ndr_byte o[arraytest_c1];
#else
  (
handle_t h,
ndr_byte i[arraytest_c1],
ndr_byte o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op10(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 3 0
 */
void arraytest_op30
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_short_float i[arraytest_c1];
ndr_short_float o[arraytest_c1];
#else
  (
handle_t h,
ndr_short_float i[arraytest_c1],
ndr_short_float o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op11(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 3 1
 */
void arraytest_op31
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_long_float i[arraytest_c1];
ndr_long_float o[arraytest_c1];
#else
  (
handle_t h,
ndr_long_float i[arraytest_c1],
ndr_long_float o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op12(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 3 2
 */
void arraytest_op32
#ifndef __STDC__
    (h, i, o)
handle_t h;
ndr_char i[arraytest_c1];
ndr_char o[arraytest_c1];
#else
  (
handle_t h,
ndr_char i[arraytest_c1],
ndr_char o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++)
        scalartest_op13(h, i[indx], &o[indx]);
}    


/*
 * a r r a y t e s t _ o p 4 0
 */
void arraytest_op40
#ifndef __STDC__
    (h, i, o)
handle_t h;
arraytest_t8 i[arraytest_c1];
arraytest_t8 o[arraytest_c1];
#else
  (
handle_t h,
arraytest_t8 i[arraytest_c1],
arraytest_t8 o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++) {
        scalartest_op5(h, i[indx].a, &o[indx].a);
        scalartest_op3(h, i[indx].b, &o[indx].b);
    }
}    


/*
 * a r r a y t e s t _ o p 4 1
 */
void arraytest_op41
#ifndef __STDC__
    (h, i, o)
handle_t h;
arraytest_t9 i[arraytest_c1];
arraytest_t9 o[arraytest_c1];
#else
  (
handle_t h,
arraytest_t9 i[arraytest_c1],
arraytest_t9 o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx;

    for (indx=0; indx<arraytest_c1; indx++) {
        o[indx].flag = i[indx].flag;
        switch (i[indx].flag) {
            case false:
                scalartest_op5(
                    h, i[indx].tagged_union.a, &o[indx].tagged_union.a);
                break;
            case true:
                scalartest_op3 (
                    h, i[indx].tagged_union.b, &o[indx].tagged_union.b);
        }
    }
}    


/*
 * a r r a y t e s t _ o p 4 2
 */
void arraytest_op42
#ifndef __STDC__
    (h, i, o)
handle_t h;
arraytest_t10 i[arraytest_c1];
arraytest_t10 o[arraytest_c1];
#else
  (
handle_t h,
arraytest_t10 i[arraytest_c1],
arraytest_t10 o[arraytest_c1]
  )
#endif
{
    ndr_long_int indx, jndx;

    for (indx = 0; indx < arraytest_c1; indx++) {
        for (jndx = 0; i[indx][jndx] != '\000'; jndx++)
            scalartest_op13 (
                h, i[indx][jndx], &o[indx][jndx]);
        o[indx][jndx] = '\000';
    }
}    
