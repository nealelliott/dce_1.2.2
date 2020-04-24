/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: noauthcn.h,v $
 * Revision 1.1.514.2  1996/02/18  22:56:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:38  marty]
 *
 * Revision 1.1.514.1  1995/12/08  00:21:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:59:52  root]
 * 
 * Revision 1.1.512.1  1994/01/21  22:38:21  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:47  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  23:53:19  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:09:43  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  21:12:50  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:41:28  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  13:38:33  rsalz
 * 	 17-mar-1992 sp      Initial Version
 * 	[1992/04/30  23:20:32  rsalz]
 * 
 * $EndLog$
 */
#ifndef _NOAUTHCN_H
#define _NOAUTHCN_H 	1

/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  NAME
**
**      noauthcn.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  The noauth CN authentication module interface.
**
**
*/

#include <cn.h> 

typedef struct 
{
    rpc_cn_auth_info_t  cn_info;

    /*
     * Noauth specific fields here.
     */
} rpc_noauth_cn_info_t, *rpc_noauth_cn_info_p_t;

EXTERNAL rpc_cn_auth_epv_t rpc_g_noauth_cn_epv;

#endif /* _NOAUTHCN_H */
