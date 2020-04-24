/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:13:52  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:32  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:42  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:55:53  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:33:11  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <epv.h>

static ndr_boolean verbose = false;

void
epv_op0
#ifdef NIDL_PROTOTYPES
(
    /*in*/     handle_t h,
    /*in,out*/ ndr_char name[]
)
#else
(h, name)
    handle_t h;
    ndr_char name[];
#endif
{
    if (verbose)
        printf("In epv_op0().\n");

    strcpy(name, epv_name_op0);
}

void
epv_op1
#ifdef NIDL_PROTOTYPES
(
    /*in*/     handle_t h,
    /*in,out*/ ndr_char name[]
)
#else
(h, name)
    handle_t h;
    ndr_char name[];
#endif
{
    if (verbose)
        printf("In epv_op1().\n");

    strcpy(name, epv_name_op1);
}
