/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_main.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:19  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:53  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:29:32  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:46:23  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:49:02  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:15:52  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:47  devrcs
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



#ifndef TSH_MAIN_INCLUDED
#define TSH_MAIN_INCLUDED

#include <dce/rpc.h>


/*  Here is the context block that holds various bits of info relative to each 
 *  test program that gets executed from the TSH.  These are in effect global vars
 *  because of the way the context block is used.  It is typoically the first
 *  parameter to any function that needs the context info that it contains.
 */
typedef struct ctx
    {
    char_p_t       *ProgramName;        /* program name is argv[0] off command line */
    ndr_long_int   stderrSet;           /* set if -s encountered on command line */
    ndr_long_int   PerformanceSet;      /* set if -p encountered on command line */
    ndr_long_int   OptionsMask;         /* set to value specified by -d on cmd line */
    ndr_long_int   InfoUsed;            /* set when info block is printed */
    ndr_long_int   ErrorSet;            /* set when error is found during test */
    char_p_t       *InfoString;         /* info string about test from .tsh file */
    } CtxBlock;

#endif
