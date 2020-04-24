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
 * Revision 1.1.4.2  1996/02/17  23:18:40  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:25  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:32:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:57  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:58:36  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:47:40  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <xmitas_nested.h>

int client_aux(h, passes)
    handle_t h;
    int passes;
{
    small_t smt;

    /*
     * small_t is transmitted as short_t which is transmitted as long_t.
     * At each translation point a value is added in to verify execution:
     *  small_t -> short_t adds  2
     *  short_t -> long_t  adds  7
     *  long_t  -> short_t adds 11
     *  short_t -> small_t adds  3
     */
    smt.sm = 0;
    op0(h, &smt);
    if (smt.sm != 2*(2+7+11+3))
        fprintf(stderr, "client_aux op0: Expected: %d, Actual: %d\n",
                2*(2+7+11+3), smt.sm);

    return 0;
}
