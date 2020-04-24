/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.6.2  1996/02/17  23:14:18  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:45  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:06:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:38  root]
 * 
 * Revision 1.1.3.3  1993/01/07  19:57:27  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:34:05  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:21:30  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:44  weisman]
 * 
 * Revision 1.1  1992/01/19  03:24:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <gregbug1.h>
#if defined(VMS) || defined(SNI_SVR4_POSIX)
#include <string.h>
#else
#include <strings.h>
#endif
#include <gb1_macros.h>

print_info (
    bug_result_t *result,
    char *message
)
{
    message[0] = '\0';
    strncat(message,
	result->tagged_union.result.passwd.tagged_union.unix_encrypted.salt,
	sizeof(result->tagged_union.result.passwd.tagged_union.unix_encrypted)
    );
}

int client_aux(handle)
    handle_t handle;
{
    bug_result_t            result;
    char                    message[80];

    bug_test(handle, &result);

    print_info(&result,message);
    return(strncmp(message,"abcdefghijklmn",sizeof("abcdefghijklmn")));
}
