/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.4.2  1996/02/17  23:13:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:31  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:03:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:37  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:55:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:33:04  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdlib.h>
#include <stdio.h>
#include <epv.h>

ndr_boolean verbose = false;

int client_aux
#ifdef NIDL_PROTOTYPES
(handle_t h, int passes)
#else
(h, passes)
handle_t h;
int passes;
#endif
{
    long failures = 0;
    ndr_char name[epv_length];

    name[0] = '\0';

    epv_v0_0_c_epv.epv_op0(h, name);
    if (strcmp(name, epv_name_op0)) {
        ++failures;
    }

    epv_v0_0_c_epv.epv_op1(h, name);
    if (strcmp(name, epv_name_op1)) {
        ++failures;
    }

    return failures;
}
