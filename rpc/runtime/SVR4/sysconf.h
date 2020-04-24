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
 * $Log: sysconf.h,v $
 * Revision 1.1.73.2  1996/02/18  22:55:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:30  marty]
 *
 * Revision 1.1.73.1  1995/12/08  00:16:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:11  root]
 * 
 * Revision 1.1.71.1  1994/01/21  22:33:40  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:59:53  cbrooks]
 * 
 * Revision 1.1.6.2  1993/06/24  20:17:04  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:38:43  hinman]
 * 
 * Revision 1.1.3.6  1993/02/01  20:35:48  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  16:14:02  hinman]
 * 
 * Revision 1.1.3.5  1993/01/03  22:57:37  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:57:52  bbelch]
 * 	Revision 1.1.4.2  1993/01/11  16:11:27  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.3.4  1992/12/23  20:11:34  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:29:24  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/10  15:14:16  sommerfeld
 * 	Add RPC_NLS_FORMAT and RPC_DEFAULT_NLSPATH.
 * 	[1992/10/26  20:58:48  sommerfeld]
 * 
 * Revision 9.6.4.3  93/01/05  10:17:54  greg
 * 	Fold in new OSF copyright.
 * 
 * Revision 9.6.4.2  92/12/22  14:04:26  greg
 * 	Fold in changes from OSF version 1.1.3.3.  This file will be auto-updateable
 * 	after the 1.0.2 SVR merge.
 * 
 * Revision 9.6.1.5  92/12/17  13:31:26  blurie
 * 	Reentrant library changes.
 * 
 * 	SVR4 needs to include stdio.h because it is not being
 * 	implicitly included by cma anymore.  Some rpc files
 * 	depend on the prototypes in stdio.h.
 * 	[92/12/17  11:37:26  blurie]
 * 
 * Revision 9.6.1.4  92/09/30  16:47:50  root
 * 	Acceptance of OSF rev 1.1.3.2
 * 
 * Revision 9.12.1.2  92/09/30  10:02:00  hinman
 * 	Auto checkin of OSF rev 1.1.3.2
 * 
 * Revision 1.1.3.2  1992/09/29  20:42:28  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:45:16  weisman]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  NAME:
**
**      sysconf.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This file contains all definitions specific to the SINIX SVR4 platform
**
*/

#ifndef _SYSCONF_H 
#define _SYSCONF_H

#include <stdio.h>

#ifdef SNI_SVR4_POSIX
#include <sys/types.h>
#include <sys/socket.h>
#else
#include <sys/file.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <time.h>
#endif /* SNI_SVR4_POSIX */

#include <dce/cma.h>
#include <dce/cma_ux.h>
#include <dce/cmalib_crtlx.h>
#include <dce/cma_px.h>
#include <dce/cma_errno.h>
#include <dce/pthread_exc.h>
#include <assert.h>

#ifndef SNI_SVR4_POSIX
#include <fcntl.h>
#include <string.h>
#endif /* ! SNI_SVR4_POSIX */

#define NO_VOID_STAR

/*
 * To use prototyping on the SVR4 platform the declaration of
 * rpc__printf cannot be used .Turn
 * the declaration and definition of rpc__printf off by definining
 * NO_RPC_PRINTF and define rpc__printf to be plain old printf.
 */
#define USE_PROTOTYPES
#define NO_RPC_PRINTF
#define rpc__printf printf 

/*
 * Define the protocol keywords needed
 */
#ifndef NAF_IP
#define NAF_IP
#endif

#ifndef PROT_NCADG
#define PROT_NCADG
#endif

#ifndef PROT_NCACN
#define PROT_NCACN
#endif

#if defined(SNI_SVR4V3)
# define RPC_DEFAULT_NLSPATH "/opt/dcelocal/lib/nls/msg/En/%s.cat"
# define RPC_NLS_FORMAT "%s.cat"
#else
# define RPC_DEFAULT_NLSPATH "/usr/lib/nls/msg/en_US.88591/%s"
# define RPC_NLS_FORMAT "%s"
#endif /* defined(SNI_SVR4V3) */

#endif /* _SYSCONF_H */
