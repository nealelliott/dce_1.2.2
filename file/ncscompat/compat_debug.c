/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: compat_debug.c,v $
 * Revision 1.1.51.1  1996/10/02  17:54:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:05  damon]
 *
 * Revision 1.1.46.3  1994/08/17  15:15:43  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/04  14:47:41  ruby]
 * 
 * Revision 1.1.46.2  1994/06/09  14:13:11  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:26:25  annie]
 * 
 * Revision 1.1.46.1  1994/02/04  20:22:39  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:53  devsrc]
 * 
 * Revision 1.1.44.1  1993/12/07  17:28:01  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:48:57  jaffe]
 * 
 * Revision 1.1.3.3  1993/01/19  16:08:02  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  15:11:31  cjd]
 * 
 * Revision 1.1.3.2  1992/11/24  17:54:50  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:09:42  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:47:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * compat_debug.c -- definition of the compat package debug flag
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/osi.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsncsmac.h>
#include <dfsncssvc.h>
#include <dfsncsmsg.h>

extern int  compat_svc_must_init;
extern void compat_svc_init();

#define INIT_SVC if (compat_svc_must_init) compat_svc_init()

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ncscompat/compat_debug.c,v 1.1.51.1 1996/10/02 17:54:18 damon Exp $")

unsigned long compat_debug_flag = 0x0;

EXPORT void compat_LogParamError(routineNameP, paramNameP, logCondition, filenameP, line)
     char *	routineNameP;
     char *	paramNameP;
     int	logCondition;
     char *	filenameP;
     int	line;
{
  if (logCondition) 
    {

    /* There are no servicability calls in the kernel. If you look in osi.h, */
    /*  you'll see that outside the kernel, osi_dp is replaced with a        */
    /* "printf".*/

#ifdef KERNEL
      osi_dp("%s: Error: required pointer parameter, %s, has NULL value",
          (routineNameP != (char *)NULL) ? routineNameP : "(unknown routine)",
	  (paramNameP != (char *)NULL) ? paramNameP : "(unknown parameter)");
      osi_dp("\t%s, %d\n", ((filenameP) ? filenameP : "(unknown file)"), line);
#else
      INIT_SVC;
      if (!routineNameP && !paramNameP)
	dce_svc_printf(NCS_S_CD_UNKNOWN_ALL_MSG);
      else if (!routineNameP && paramNameP)
	dce_svc_printf(NCS_S_CD_UNKNOWN_ROUTINE_MSG, paramNameP);
      else if (routineNameP && !paramNameP)
	dce_svc_printf(NCS_S_CD_UNKNOWN_PARAMETER_MSG, routineNameP);
      else if (routineNameP && paramNameP)
	dce_svc_printf(NCS_S_CD_KNOWN_ALL_MSG, routineNameP,
	       paramNameP);
      if (filenameP)
	dce_svc_printf(NCS_S_CD_FILENAME_MSG, filenameP, line);
      else
	dce_svc_printf(NCS_S_CD_UNKNOWN_FILENAME_MSG, line);
#endif /* KERNEL */
  }
}
