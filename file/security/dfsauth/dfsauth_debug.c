/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_debug.c,v $
 * Revision 1.1.506.1  1996/10/02  20:58:48  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:15  damon]
 *
 * Revision 1.1.501.3  1994/07/13  22:26:45  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:14:10  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:40  mbs]
 * 
 * Revision 1.1.501.2  1994/06/09  14:19:24  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:20  annie]
 * 
 * Revision 1.1.501.1  1994/02/04  20:29:31  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:17:48  devsrc]
 * 
 * Revision 1.1.499.1  1993/12/07  17:33:14  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:54:08  jaffe]
 * 
 * Revision 1.1.3.3  1993/01/21  15:19:02  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:56:53  cjd]
 * 
 * Revision 1.1.3.2  1992/11/24  18:31:43  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:23:29  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:52:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	dfsauth_debug.c -- debugging flag and routines for dfsauth package
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/icl.h>

#include <dfsauth_trace.h>
#include <dfsauth_debug.h>

#ifndef KERNEL
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/security/dfsauth/dfsauth_debug.c,v 1.1.506.1 1996/10/02 20:58:48 damon Exp $")

EXPORT struct icl_set *dfsauth_iclSetp = (struct icl_set *)0;

long dfsauth_debug_flag = 0;

EXPORT int ParseDebugFlag(flagString, flagValP)
     char *	flagString;
     long *	flagValP;
{
  int		rtnVal = 1;	/* assume success */
  char		secondCharacter = *(flagString + 1);
  
  if (*flagString == '0') {
    if ((secondCharacter == 'x') || (secondCharacter == 'X')) {
      /* we have a hex number */
      rtnVal = sscanf(flagString, "%lx", flagValP);
    }
    else {
      /* we have an octal number */
      rtnVal = sscanf(flagString, "%lo", flagValP);
    }
  }
  else {
    /* we have a decimal value */
    *flagValP = atol(flagString);
  }	
  
  return (rtnVal);
}

EXPORT void dfsauth_LogParamError(routineNameP, parameterNameP)
     char *	routineNameP;
     char *	parameterNameP;
{
  dprintf(dfsauth_debug_flag,
	  ("%s: Error: required pointer parameter, %s, has NULL value",
	   (routineNameP != (char *)NULL) ? routineNameP : "(unknown routine)",
	   (parameterNameP != (char *)NULL) ? parameterNameP: "(unknown parameter)"));
  icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_DEBUG_NONE_0, ICL_TYPE_STRING, 
	     ((routineNameP != (char*)NULL) ? routineNameP : "(unknown routine)"),
	     ICL_TYPE_STRING,
	     ((parameterNameP!=(char*)NULL)?parameterNameP:"(unknown parameter)"));
}

EXPORT int dfsauth_InitTracingPackage()
{
    struct icl_log *logp;
    int code;
    static int initted = 0;

    if (initted)
	return 0;
    initted = 1;

    (void) dacl_InitTracingPackage();
    code = icl_CreateLog("common", 0, &logp);
    if (code == 0) {
	code = icl_CreateSetWithFlags("dfsauth", logp, (struct icl_log *) 0,
				      ICL_CRSET_FLAG_DEFAULT_OFF,
				      &dfsauth_iclSetp);
    }

    return code;
}
