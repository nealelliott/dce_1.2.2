/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnmgmt.c,v $
 * Revision 1.1.36.2  1996/02/18  00:01:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:36  marty]
 *
 * Revision 1.1.36.1  1995/12/08  00:17:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:36  root]
 * 
 * Revision 1.1.34.1  1994/01/21  22:34:23  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:54:10  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:58:47  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:59:39  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:15:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:31:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:05:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnmgmt.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  The NCA Connection Protocol Service's Management Service.
**
**
*/

#include <commonp.h>    /* Common declarations for all RPC runtime */
#include <com.h>        /* Common communications services */
#include <comprot.h>    /* Common protocol services */
#include <cnp.h>        /* NCA Connection private declarations */
#include <cnmgmt.h>


/*
**++
**
**  ROUTINE NAME:       rpc__cn_mgmt_init
**
**  SCOPE:              PRIVATE - declared in cnmgmt.h,
**                                called from cninit.
**
**  DESCRIPTION:
**
**  Initialize the Connection management data collection
**  registers.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   Management counters cleared. 
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_mgmt_init (void)
{
    memset (&rpc_g_cn_mgmt, 0, sizeof (rpc_g_cn_mgmt));
}

/*
**++
**
**  Routine NAME:       rpc__cn_mgmt_inq_calls_sent
**
**  SCOPE:              PRIVATE - declared in cnmgmt.h
**
**  DESCRIPTION:
**
**  Report the total number of RPC that have been sent by
**  the NCA Connection Protocol.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          The number of RPCs sent through the NCA
**                      Connection Protocol Service.
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE unsigned32 rpc__cn_mgmt_inq_calls_sent (void)

{
    return (rpc_g_cn_mgmt.calls_sent);
}



/*
**++
**
**  Routine NAME:       rpc__cn_mgmt_inq_calls_rcvd
**
**  SCOPE:              PRIVATE - declared in cnmgmt.h
**
**  DESCRIPTION:
**
**  Report the total number of RPCs that have been received by
**  the NCA Connection Protocol.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          The number of RPCs received through the NCA
**                      Connection Protocol Service.
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE unsigned32 rpc__cn_mgmt_inq_calls_rcvd (void)

{
    return (rpc_g_cn_mgmt.calls_rcvd);
}


/*
**++
**
**  ROUTINE NAME:       rpc__cn_mgmt_inq_pkts_sent
**
**  SCOPE:              PRIVATE - declared in cnmgmt.h
**
**  DESCRIPTION:
**
**  Report the total number of packets that have been sent by
**  the NCA Connection Protocol.
**
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          The number of RPC packets sent by the NCA
**                      Connection Protocol Service.
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE unsigned32 rpc__cn_mgmt_inq_pkts_sent (void)

{

    return (rpc_g_cn_mgmt.pkts_sent);
}


/*
**++
**
**  ROUTINE NAME:       rpc__cn_mgmt_inq_pkts_rcvd
**
**  SCOPE:              PRIVATE - declared in cnmgmt.h
**
**  DESCRIPTION:
**
**  Report the total number of packets that have been received by
**  the NCA Connection Protocol.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          The number of RPC packets received by
**                      the NCA Connection Protocol Service.
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE unsigned32 rpc__cn_mgmt_inq_pkts_rcvd (void)
{
    return (rpc_g_cn_mgmt.pkts_rcvd);
}
