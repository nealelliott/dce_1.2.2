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
 * Revision 1.1.4.2  1996/02/17  23:17:04  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:19  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:29:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:29  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:34:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:42:24  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:27:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <represent.h>
#include <test_common.h>

void s_reverse
( h, in_string, out_string )
    /* [in] */handle_t h;
    JIS_KANJI in_string[];
    IBM_KANJI out_string[];
{
    int i, length;
                  
#ifdef DEBUG
    printf("Server received %s\n", (char *)in_string);
#endif
    length = strlen((char *)in_string) / 2;
    for (i = 0; i<length; i++)
    {
        out_string[i].high_byte = in_string[length - i - 1].low_byte;
        out_string[i].low_byte = in_string[length - i - 1].high_byte;
    }
    out_string[length].high_byte = 0;
    out_string[length].low_byte = 0;

}

globaldef represent_v0_0_epv_t represent_v0_0_m_epv = {s_reverse};
