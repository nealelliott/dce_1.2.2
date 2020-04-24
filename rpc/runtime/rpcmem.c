/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpcmem.c,v $
 * Revision 1.1.648.2  1996/02/18  00:05:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:22  marty]
 *
 * Revision 1.1.648.1  1995/12/08  00:22:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:26  root]
 * 
 * Revision 1.1.646.2  1994/05/19  21:15:01  hopkins
 * 	Merge with DCE1_1.
 * 	[1994/05/04  19:44:00  hopkins]
 * 
 * 	Serviceability work.
 * 	[1994/05/03  20:28:16  hopkins]
 * 
 * Revision 1.1.646.1  1994/01/21  22:39:12  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:56  cbrooks]
 * 
 * Revision 1.1.5.4  1993/01/03  23:54:53  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:12:17  bbelch]
 * 
 * Revision 1.1.5.3  1992/12/23  21:16:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:43:59  zeliff]
 * 
 * Revision 1.1.5.2  1992/12/18  19:57:48  wei_hu
 * 	     15-oct-92 gh             Restore the missing DEC copyright.
 * 	[1992/12/18  19:47:24  wei_hu]
 * 
 * Revision 1.1.2.2  1992/05/01  16:05:20  rsalz
 * 	 05-feb-92 mishkin       fix realloc
 * 	[1992/05/01  00:18:37  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:06:52  devrcs
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
**  NAME:
**
**      rpcmem.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  non-macro version of runtime memory allocator (see rpcmem.h)
**
**
*/

#include <commonp.h>

PRIVATE pointer_t rpc__mem_alloc
#ifdef _DCE_PROTO_
(
    unsigned32 size,
    unsigned32 type,
    unsigned32 flags
)
#else
(size, type, flags)
unsigned32 size;
unsigned32 type;
unsigned32 flags;
#endif
{
    char *addr;

    RPC_MEM_ALLOC_IL(addr, char *, size, type, flags);

#ifdef DEBUG
    if ((type & 0xff) == rpc_g_dbg_switches[rpc_es_dbg_mem_type])
    {
        RPC_DBG_PRINTF(rpc_e_dbg_mem, 1,
	    ("(rpc__mem_alloc) type %x - %x @ %x\n",
	    type, size, addr));
    }
    else
    {
	RPC_DBG_PRINTF(rpc_e_dbg_mem, 10,
	    ("(rpc__mem_alloc) type %x - %x @ %x\n",
	    type, size, addr));
    }
#endif

    return addr;
}

PRIVATE pointer_t rpc__mem_realloc
#ifdef _DCE_PROTO_
(
    pointer_t addr,
    unsigned32 size,
    unsigned32 type,
    unsigned32 flags
)
#else
(addr, size, type, flags)
pointer_t addr;
unsigned32 size;
unsigned32 type;
unsigned32 flags;
#endif
{
    RPC_MEM_REALLOC_IL(addr, pointer_t, size, type, flags);

#ifdef DEBUG
    if ((type & 0xff) == rpc_g_dbg_switches[rpc_es_dbg_mem_type])
    {
        RPC_DBG_PRINTF(rpc_e_dbg_mem, 1,
	    ("(rpc__mem_realloc) type %x - %x @ %x\n",
	    type, size, addr));
    }
    else
    {
	RPC_DBG_PRINTF(rpc_e_dbg_mem, 10,
	    ("(rpc__mem_realloc) type %x - %x @ %x\n",
	    type, size, addr));
    }
#endif

    return addr;
}

PRIVATE void rpc__mem_free
#ifdef _DCE_PROTO_
(
    pointer_t addr,
    unsigned32 type
)
#else
(addr, type)
pointer_t addr;
unsigned32 type;
#endif
{

#ifdef DEBUG
    if ((type & 0xff) == rpc_g_dbg_switches[rpc_es_dbg_mem_type])
    {
        RPC_DBG_PRINTF(rpc_e_dbg_mem, 1,
	    ("(rpc__mem_free) type %x @ %x\n",
	    type, addr));
    }
    else
    {
	RPC_DBG_PRINTF(rpc_e_dbg_mem, 10,
	    ("(rpc__mem_free) type %x @ %x\n",
	    type, addr));
    }
#endif

    RPC_MEM_FREE_IL(addr, type);
}
