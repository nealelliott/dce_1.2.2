/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnid.h,v $
 * Revision 1.1.34.2  1996/02/18  22:55:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:41  marty]
 *
 * Revision 1.1.34.1  1995/12/08  00:17:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:34  root]
 * 
 * Revision 1.1.32.1  1994/01/21  22:34:18  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:54:08  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:58:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:59:32  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:15:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:31:16  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:05:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CNID_H
#define _CNID_H	1
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnid.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Interface to the Local Identifier Service.
**
**
*/

/*
 * R P C _ C N _ L O C A L _ I D _ E Q U A L
 */
#define RPC_CN_LOCAL_ID_EQUAL(id1, id2)\
    ((id1.parts.id_seqnum == id2.parts.id_seqnum) &&\
     (id1.parts.id_index == id2.parts.id_index)) 

/*
 * R P C _ C N _ L O C A L _ I D _ V A L I D
 */

#define RPC_CN_LOCAL_ID_VALID(id) (id.parts.id_seqnum != 0)

/*
 * R P C _ C N _ L O C A L _ I D _ C L E A R
 */
#define RPC_CN_LOCAL_ID_CLEAR(id)\
{\
    id.parts.id_seqnum = 0;\
    id.parts.id_index = 0;\
}

/*
 * R P C _ _ C N _ I N I T _ S E Q N U M
 *
 * This routine initializes the global sequence number cell and
 * corresponding mutex.
 */

void rpc__cn_init_seqnum _DCE_PROTOTYPE_ ((void));

/*
 * R P C _ _ C N _ G E N _ L O C A L _ I D
 *
 * This routine creates a new local identifier.
 */

void rpc__cn_gen_local_id _DCE_PROTOTYPE_ ((
    unsigned32          /* index */,
    rpc_cn_local_id_t   * /* lcl_id */));

#endif /* _CNID_H */
