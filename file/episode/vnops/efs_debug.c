/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_debug.c,v $
 * Revision 1.1.104.1  1996/10/02  17:43:42  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:45  damon]
 *
 * Revision 1.1.99.2  1994/06/09  14:05:18  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:20:52  annie]
 * 
 * Revision 1.1.99.1  1994/02/04  20:16:29  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:59  devsrc]
 * 
 * Revision 1.1.97.1  1993/12/07  17:22:25  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/06  13:37:22  jaffe]
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1994 Transarc Corporation
 * All rights reserved.
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/debug.h>
#include <dcedfs/episode/efs_debug.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_debug.c,v 1.1.104.1 1996/10/02 17:43:42 damon Exp $")

/*
 * efs_debug - debugging interface to allow tweaking internal code.
 *
 * XXX: this should probably be turned off for production code.
 */

/* EXPORT */
long efs_debug(package, op, parm1, parm2)
     long package, op, parm1, parm2;
{
      long code;

#if AFSL_DEBUG_LEVEL < 3
      /* debugging disabled */
      return EDOM;
#else
      if (!osi_suser(osi_getucred())) {
	  return EPERM;
      } else switch(package) {
	case EFS_DEBUG_ASYNC:
	  code = asevent_efs_debug(op, parm1, parm2);
	  break;
	case EFS_DEBUG_VNOPS:
	  code = efs_vnops_debug(op, parm1, parm2);
	  break;
	default:
	case EFS_DEBUG_LOGBUF:
	case EFS_DEBUG_ANODE:
	case EFS_DEBUG_DIR:
	case EFS_DEBUG_ACL:
	  code = EINVAL;
	  break;
      }
      return code;
#endif /* AFSL_DEBUG_LEVEL */
}
