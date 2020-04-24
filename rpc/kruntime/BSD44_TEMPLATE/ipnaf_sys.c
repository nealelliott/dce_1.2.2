/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ipnaf_sys.c,v $
 * Revision 1.1.8.2  1996/02/17  23:59:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:20  marty]
 *
 * Revision 1.1.8.1  1995/12/08  00:13:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:33  root]
 * 
 * Revision 1.1.6.2  1994/05/27  15:35:35  tatsu_s
 * 	DG multi-buffer fragments.
 * 	Added rpc__ip_is_*().
 * 	[1994/05/03  18:35:21  tatsu_s]
 * 
 * Revision 1.1.6.1  1994/01/21  22:31:08  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:38:23  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  22:34:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:50:19  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:36:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:47:19  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  17:54:35  rsalz
 * 	 16-mar-92 nm    Strip old history.
 * 	[1992/05/01  17:49:47  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:15:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  NAME
**
**      ipnaf_osc
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This module contains routines specific to the Internet Protocol,
**  the Internet Network Address Family extension service, and the
**  Berkeley Unix system.
**
**
*/

#include <commonp.h>
#include <com.h>
#include <comnaf.h>
#include <ipnaf.h>

/*
**++
**
**  ROUTINE NAME:       rpc__ip_desc_inq_addr
**
**  SCOPE:              PRIVATE - declared in ipnaf.h
**
**  DESCRIPTION:
**      
**  Receive a socket descriptor which is queried to obtain family, endpoint
**  and network address.  If this information appears valid for an IP
**  address,  space is allocated for an RPC address which is initialized
**  with the information obtained from the socket.  The address indicating
**  the created RPC address is returned in rpc_addr.
**
**  INPUTS:
**
**      protseq_id      Protocol Sequence ID representing a particular
**                      Network Address Family, its Transport Protocol,
**                      and type.
**
**      desc            Descriptor, indicating a socket that has been
**                      created on the local operating platform.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      rpc_addr_vec
**
**      status          A value indicating the status of the routine.
**
**          rpc_s_ok               The call was successful.
**
**          rpc_s_no_memory         Call to malloc failed to allocate memory.
**
**          rpc_s_cant_inq_socket  Attempt to get info about socket failed.
**
**          Any of the RPC Protocol Service status codes.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__ip_desc_inq_addr (protseq_id, desc, rpc_addr_vec, status)

rpc_protseq_id_t        protseq_id;
rpc_socket_t            desc;
rpc_addr_vector_p_t     *rpc_addr_vec;
unsigned32              *status;

{
    rpc_ip_addr_p_t ip_addr;

    CODING_ERROR (status);
        
    /*
     * allocate memory for the new RPC address
     */
    RPC_MEM_ALLOC (
        ip_addr,
        rpc_ip_addr_p_t,
        sizeof(rpc_ip_addr_t),
        rpc_c_mem_rpc_addr,
        rpc_c_mem_waitok);
    
    /*
     * successful malloc
     */
    if (ip_addr == NULL)
    {
        *status = rpc_s_no_memory;
        return;
    }

    /*
     * insert individual parameters into RPC addr
     */
    (ip_addr)->rpc_protseq_id = protseq_id;
    (ip_addr)->len = sizeof (struct sockaddr_in);

    /*
     * get the socket info into the RPC addr
     */
    if (! RPC_SOCKET_IS_ERR( rpc__socket_inq_endpoint (desc, (rpc_addr_p_t) ip_addr) ))
    {
        *status = rpc_s_ok;
    }
    else
    {
        RPC_MEM_FREE (ip_addr, rpc_c_mem_rpc_addr);
        *status = rpc_s_cant_inq_socket;
        return;
    }

    RPC_MEM_ALLOC (
        *rpc_addr_vec,
        rpc_addr_vector_p_t,
        sizeof **rpc_addr_vec,
        rpc_c_mem_rpc_addr_vec,
        rpc_c_mem_waitok);
    
    if (*rpc_addr_vec == NULL)
    {
        RPC_MEM_FREE (ip_addr, rpc_c_mem_rpc_addr);
        *status = rpc_s_no_memory;
        return;
    }

    (*rpc_addr_vec)->len = 1;
    (*rpc_addr_vec)->addrs[0] = (rpc_addr_p_t) ip_addr;

    *status = rpc_s_ok;
    return;
}


/*
**++
**
**  ROUTINE NAME:       rpc__ip_get_broadcast
**
**  SCOPE:              PRIVATE - EPV declared in ipnaf.h
**
**  DESCRIPTION:
**      
**
**
**
**  INPUTS:
**
**      naf_id          Network Address Family ID serves
**                      as index into EPV for IP routines.
**
**      rpc_protseq_id
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:                        
**
**      rpc_addrs
**
**      status          A value indicating the status of the routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__ip_get_broadcast (naf_id, rpc_protseq_id, rpc_addrs, status)

rpc_naf_id_t        naf_id;
rpc_protseq_id_t    rpc_protseq_id;
rpc_addr_vector_p_t *rpc_addrs;
unsigned32          *status; 

{
    *rpc_addrs = NULL;
    *status = -1 /* !!! error_status_ok */;
}

/*
**++
**
**  ROUTINE NAME:       rpc__ip_init_local_addr_vec
**
**  SCOPE:              PRIVATE - declared in ipnaf.h
**
**  DESCRIPTION:
**      
**  Initialize the local address vectors.
**
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:                        
**
**      status          A value indicating the status of the routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:
**
**      Update local_ip_addr_vec
**
**--
**/

PRIVATE void rpc__ip_init_local_addr_vec
#ifdef _DCE_PROTO_
(
    unsigned32 *status
)
#else
(status)
unsigned32 *status; 
#endif
{
    *status = rpc_s_ok;
    return;
}

/*
**++
**
**  ROUTINE NAME:       rpc__ip_is_local_network
**
**  SCOPE:              PRIVATE - declared in ipnaf.h
**
**  DESCRIPTION:
**      
**  Return a boolean value to indicate if the given RPC address is on
**  the same IP subnet.
**
**
**  INPUTS:
**
**      rpc_addr        The address that forms the path of interest
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:                        
**
**      status          A value indicating the status of the routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      result          true => the address is on the same subnet.
**                      false => not.
**
**  SIDE EFFECTS:       none
**
**--
**/
PRIVATE boolean32 rpc__ip_is_local_network
#ifdef _DCE_PROTO_
(
    rpc_addr_p_t rpc_addr,
    unsigned32   *status
)
#else
(rpc_addr, status)
rpc_addr_p_t rpc_addr;
unsigned32   *status; 
#endif
{
    *status = rpc_s_ok;
    return false;
}

/*
**++
**
**  ROUTINE NAME:       rpc__ip_is_local_addr
**
**  SCOPE:              PRIVATE - declared in ipnaf.h
**
**  DESCRIPTION:
**      
**  Return a boolean value to indicate if the given RPC address is the
**  the local IP address.
**
**
**  INPUTS:
**
**      rpc_addr        The address that forms the path of interest
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:                        
**
**      status          A value indicating the status of the routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      result          true => the address is local.
**                      false => not.
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE boolean32 rpc__ip_is_local_addr
#ifdef _DCE_PROTO_
(
    rpc_addr_p_t rpc_addr,
    unsigned32   *status
)
#else
(rpc_addr, status)
rpc_addr_p_t rpc_addr;
unsigned32   *status; 
#endif
{
    *status = rpc_s_ok;
    return false;
}
