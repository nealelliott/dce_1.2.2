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
 * Revision 1.1.4.2  1996/02/17  23:17:03  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:17  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:29:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:28  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:34:39  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:42:20  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:27:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h> 
#include <represent.h>
#include <test_common.h>

int client_aux(h,passes)
      handle_t h;
      int passes;
{
    error_status_t st;
    JIS_KANJI in_string[SIZE];
    IBM_KANJI out_string[SIZE];
    int i;

    for (i=0; i<26; i++)
    {
        in_string[i].high_byte = 'A' + i;
        in_string[i].low_byte = 'a' + i;
    }

    in_string[i].high_byte = '\0';
    in_string[i].low_byte = '\0';

    reverse( h, in_string, out_string );

    printf("Client received %s\n", (char *)out_string);
    return 0;
}
