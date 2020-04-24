/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: alt_common_krpc.h,v $
 * Revision 1.1.6.2  1996/02/18  23:46:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:03  marty]
 *
 * Revision 1.1.6.1  1995/12/08  00:13:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:28  root]
 * 
 * Revision 1.1.4.1  1994/01/21  22:31:02  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:32:05  cbrooks]
 * 
 * Revision 1.1.2.1  1993/09/01  14:29:34  delgado
 * 	DFS 486 port
 * 	[1993/09/01  14:29:10  delgado]
 * 
 * 	DFS 486 port
 * 
 * $EndLog$
 */
#ifndef _ALT_COMMON_KRPC_H
#define _ALT_COMMON_KRPC_H

/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**
**
**  NAME:
**
**      alt_common_krpc.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Alternate common include files for kernel rpc.
**
**
*/

#include <rpcfork.h>
#include <rpcdbg.h>
#include <rpcclock.h>
#include <rpcmem_krpc.h>
#include <rpcmutex.h>
#include <rpctimer.h>
#include <rpclist.h>
#include <rpcrand_krpc.h>

/*
 * get the runtime's exception handling ids.
 */
#include <dce/ker/exc_handling_ids_krpc.h>

/*
 * If sysconf.h doesn't define uiomove, assume it's the "traditional
 * UNIX" definition.
 */
    
#ifndef rpc__uiomove
#define rpc__uiomove(buf,len,dir,uio) uiomove(buf,len,dir,uio)
#endif

#endif /* _ALT_COMMON_KRPC_H */
