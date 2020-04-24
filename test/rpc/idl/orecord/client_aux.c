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
 * Revision 1.1.4.2  1996/02/17  23:15:39  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:35:20  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:26:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:14:16  root]
 * 
 * Revision 1.1.2.3  1993/01/13  21:49:25  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:36:53  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:29:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <orecord.h>
#include <test_common.h>

int client_aux(h)
handle_t h;
{
    int i, failures = 0;
    ndr_long_int out, len, d, checksum = 0;
    ndr_char c;
    orecord_t *o;

    len = random()%1000;
    o = (orecord_t *)malloc(sizeof(orecord_t)+(len-1)*sizeof(ndr_char));

    o->namelen = len;
    for (i = 0; i < len; i++) 
        o->name[i] = 'A' + random()%('z' - 'A');
    o->name[len-1] = '\0';

    for (i = 0; i < len; i++) checksum += o->name[i];
    checksum += c = 'A' + random()%('z'-'A');
    checksum += d = random();

    if ((out = op1(h, c, o, d)) != checksum)
    {
        printf("Checksums don't match (%d != %d)!\n", checksum, out);
        failures++;
    }

    return failures;
}
