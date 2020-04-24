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
 * Revision 1.1.4.2  1996/02/17  23:17:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:36:38  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:30:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:15:54  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:37:48  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:44:34  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:28:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <string1.h>
#include <test_common.h>

static char *_remote_strcat(s1, s2)
    /* [in] */  char	*s1;
    /* [in] */  char	*s2;
{
      char *result;
      result = (char *)rpc_ss_allocate(strlen(s1)+strlen(s2)+1);
      *result = '\0';
      strcat(result,s1);      
      strcat(result,s2);      
      return result;
}

globaldef string1_v0_0_epv_t string1_v0_0_m_epv = {_remote_strcat};
