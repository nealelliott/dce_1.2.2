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
 * Revision 1.1.4.2  1996/02/17  23:17:30  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:37  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:53  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:37:44  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:44:31  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <string1.h>
#include <test_common.h>


int client_aux(h,passes)
      handle_t h;
      int passes;
{
    char *value;
    i_h = h;
      
    value = remote_strcat("hello ","there");
    if (strcmp(value,"hello there") != 0)
    {
        printf("***FAILURE***: remote_strcat\n");
        printf("Inputs: '%s' '%s' Result: '%s'\n");
        return 1;
    }
      
    return 0;
}
