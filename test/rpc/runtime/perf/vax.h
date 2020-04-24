/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: vax.h,v $
 * Revision 1.1.6.2  1996/02/18  23:07:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:01  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:15:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:48:17  root]
 * 
 * Revision 1.1.4.2  1993/01/12  00:03:08  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:25:50  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:41:58  rsalz
 * 	Part of rpc6 code drop.
 * 	[1992/05/01  01:23:00  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:41:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
static double _d, _g;
static long _h[4];

#define d_to_g(v) { \
    _d = v; \
    asm("	cvtdh	__d,__h"); \
    asm("	cvthg	__h,__g"); \
    v = _g; \
}

#define g_to_d(v) { \
    _g = v; \
    asm("	cvtgh	__g,__h"); \
    asm("	cvthd	__h,__d"); \
    v = _d; \
}

