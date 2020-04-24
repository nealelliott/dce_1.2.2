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
 * $Log: manager.c,v $
 * Revision 1.1.6.2  1996/02/17  23:14:19  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:46  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:06:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:44  root]
 * 
 * Revision 1.1.3.3  1993/01/07  19:57:42  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:34:16  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:21:36  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:48  weisman]
 * 
 * Revision 1.1  1992/01/19  03:24:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#if defined(VMS) || defined(SNI_SVR4_POSIX)
#include <string.h>
#else
#include <strings.h>
#endif
#include <gregbug1.h>
#include <gb1_macros.h>

void his_bug_test ( h, result )
    handle_t     h;
    bug_result_t *result;
{
    result->status = error_status_ok;
    result->tagged_union.result.passwd.ptype = passwd_unix;
    strncpy( (char *)&result->tagged_union.result.passwd.tagged_union.unix_encrypted,
             "abcdefghijklmn", 14);

}


globaldef gregbug1_v0_0_epv_t gregbug1_v0_0_m_epv = {his_bug_test};

        
