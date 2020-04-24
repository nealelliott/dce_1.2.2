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
 * Revision 1.1.4.2  1996/02/17  23:15:02  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:07  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:25:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:13:45  root]
 * 
 * Revision 1.1.2.2  1993/01/07  19:58:56  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:34:59  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:25:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <multiref.h>
#include <test_common.h>

 
#define SA 5
#define SB 6
#define SC 8

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    int failures = 0;
    error_status_t st;
    short int s3 = SA+SB;
    short int *p_s3 = &s3;
    short int **p_p_s3 = &p_s3;
    thing_t thing;
#define BINDING_LEN 256
    ndr_char string_binding[BINDING_LEN];
    static ndr_char nil_string[] = "";

    thing.s1 = SA;
    thing.rprprp = &p_p_s3;
    thing.s2 = SB;
    op0( h, (short int)SC, &thing );
    if ( ((thing.s1 + thing.s2) == ***thing.rprprp ) &&
          (thing.s1 == SA * SC) &&
	  (thing.s2 == SB * SC)) {
        printf("Test successful\n");
    } else {
        printf("Test failed\n");
        failures++;
    }

    return failures;
}
