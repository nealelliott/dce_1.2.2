/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comtwr.h,v $
 * Revision 1.1.311.2  1996/02/18  22:55:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:06  marty]
 *
 * Revision 1.1.311.1  1995/12/08  00:18:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:58:26  root]
 * 
 * Revision 1.1.309.1  1994/01/21  22:36:01  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:55:54  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:22:59  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:03:15  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:45:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:34:59  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:05:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _COMTWR_H
#define _COMTWR_H 1
/*
**  Copyright (c) 1990 by
**  
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      comtwr.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**  
**  ABSTRACT:
**
**      Header file containing macros, definitions, typedefs and prototypes 
**      of exported routines from the comtwr.c module.
**  
**
**/

#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

PRIVATE void rpc__tower_free _DCE_PROTOTYPE_ (( 
    twr_p_t                 * /*tower*/,
    unsigned32              * /*status*/ ));

PRIVATE void rpc__tower_from_tower_ref _DCE_PROTOTYPE_ (( 
    rpc_tower_ref_p_t        /*tower_ref*/,
    twr_p_t                 * /*tower*/,
    unsigned32              * /*status*/ ));

PRIVATE void rpc__tower_to_tower_ref _DCE_PROTOTYPE_ (( 
   twr_p_t                  /*tower*/,
   rpc_tower_ref_p_t       * /*tower_ref*/,
   unsigned32              * /*status*/ ));

#endif /* _COMTRW_H */
