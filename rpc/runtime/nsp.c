/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsp.c,v $
 * Revision 1.1.629.2  1996/02/18  00:05:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:13  marty]
 *
 * Revision 1.1.629.1  1995/12/08  00:21:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:09  root]
 * 
 * Revision 1.1.627.3  1994/07/29  16:19:23  tom
 * 	Remove DNSPI_V2 ifdef.
 * 	[1994/07/28  22:34:20  tom]
 * 
 * Revision 1.1.627.2  1994/02/08  21:39:28  mori_m
 * 	CR 9701:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:39:18  mori_m]
 * 
 * Revision 1.1.627.1  1994/01/21  22:38:43  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:21  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  23:54:00  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:10:45  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  21:14:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:42:30  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  16:39:02  rsalz
 * 	 3-mar-92    wh      DCE 1.0.1 merge.
 * 	23-jan-92    ebm     Remove dependency on CDS' cts attribute.
 * 	[1992/05/01  16:30:39  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:12:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      NSP.C
**
**  FACILITY:
**
**     Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**     Defines all global variables needed by the RPC Name Service Component.
**
**
*/

#include <commonp.h>    /* Common internals for RPC Runtime system      */
#include <com.h>        /* Externals for Common Services component      */
#include <nsp.h>        /* Internals for Name Services component        */

/*
 * Global multi-dimensioned array to hold the architected Name Service
 * attributes string constants and their equivalent Name Service (opaque)
 * format.
 */

GLOBAL rpc_ns_attributes_t rpc_g_attr_table[RPC_C_NS_MAX][RPC_C_ATTR_MAX] = 
{
    {
        {RPC_C_NS_CLASS_VERSION},
        {RPC_C_NS_DNA_TOWERS},
        {RPC_C_NS_OBJECT_UUIDS},
        {RPC_C_NS_GROUP},
        {RPC_C_NS_PROFILE},
        {RPC_C_NS_CODESETS}
    }
};

/*
 * Authentication flag for DNS calls
 */
GLOBAL boolean32 rpc_g_ns_authn = true;

/*
 * NS class version number (DNS$CLASSVERSION)
 */
GLOBAL dnsVersion_t rpc_ns_class_version = {1,0};

/*
 * RPC_ClassVersion version number
 */
GLOBAL rpc_ns_version_t rpc_ns_rpc_class_version = {1,0};


/*
 * The global ns mutex lock.
 */
GLOBAL rpc_mutex_t ns_lock;
  
/*
 * Global expiration age (in seconds) for name service
 * local data refresh.
 */
GLOBAL unsigned32 rpc_g_ns_exp_age;

/*
 * Name Service component free-list descriptors
 */
GLOBAL rpc_list_desc_t rpc_g_free_if_list;
GLOBAL rpc_list_desc_t rpc_g_free_members;
GLOBAL rpc_list_desc_t rpc_g_free_lookup_nodes;

/*
 * The value that indicates whether or not the NS component has previously
 * been initialized (via a call to rpc__ns_init). Its declaration is
 * in nsp.h.
 */
GLOBAL boolean              rpc_ns_initialized = false;

