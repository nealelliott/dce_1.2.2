/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: noauthdg.h,v $
 * Revision 1.1.520.3  1996/02/18  22:56:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:39  marty]
 *
 * Revision 1.1.520.2  1995/12/08  00:21:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:59:54  root]
 * 
 * Revision 1.1.516.1  1994/10/18  18:04:17  tatsu_s
 * 	Fix OT12540: Added first_frag argument to rpc__noauth_dg_recv_ck().
 * 	[1994/10/06  20:07:08  tatsu_s]
 * 
 * Revision 1.1.514.1  1994/01/21  22:38:23  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:49  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  23:53:23  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:09:50  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  21:13:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:41:36  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  16:38:31  rsalz
 * 	 10-mar-92 wh              DCE 1.0.1 merge.
 * 	[1992/05/01  16:30:13  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:11:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NOAUTHDG_H
#define _NOAUTHDG_H	1
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      noauthdgp.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Definition of types private to the noauth-datagram glue module.
**
**
*/

#define NCK_NEED_MARSHALLING

#include <dg.h>
#include <noauth.h>
#include <dce/conv.h>

#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif


/*
 * For various reasons, it's painful to get at the NDR tag of the
 * underlying data, so we cheat and just encode it in big-endian order.
 */

#define rpc_marshall_be_long_int(mp, bei) \
{       long temp = htonl(bei);            \
        rpc_marshall_long_int (mp, temp);  \
}
      
#define rpc_convert_be_long_int(mp, bei) \
{                                       \
    rpc_unmarshall_long_int(mp, bei);   \
    bei = ntohl(bei);                   \
}


/*
 * DG EPV routines.
 */


#ifdef __cplusplus
extern "C" {
#endif


void rpc__noauth_dg_pre_call _DCE_PROTOTYPE_((                         
        rpc_auth_info_p_t               ,
        handle_t                        ,
        unsigned32                      *
    ));

rpc_auth_info_p_t rpc__noauth_dg_create _DCE_PROTOTYPE_((
        unsigned32                      * /*st*/
    ));

void rpc__noauth_dg_encrypt _DCE_PROTOTYPE_((
        rpc_auth_info_p_t                /*info*/,
        rpc_dg_xmitq_elt_p_t            ,
        unsigned32                      * /*st*/
    ));

void rpc__noauth_dg_pre_send _DCE_PROTOTYPE_((                         
        rpc_auth_info_p_t                /*info*/,
        rpc_dg_xmitq_elt_p_t             /*pkt*/,
        rpc_dg_pkt_hdr_p_t               /*hdrp*/,
        rpc_socket_iovec_p_t             /*iov*/,
        int                              /*iovlen*/,
        pointer_t                        /*cksum*/,
        unsigned32                      * /*st*/
    ));

void rpc__noauth_dg_recv_ck _DCE_PROTOTYPE_((                         
        rpc_auth_info_p_t                /*info*/,
        rpc_dg_recvq_elt_p_t             /*pkt*/,
        pointer_t                        /*cksum*/,
        boolean32                        /*first_frag*/,
        error_status_t                  * /*st*/
    ));

void rpc__noauth_dg_who_are_you _DCE_PROTOTYPE_((                         
        rpc_auth_info_p_t                /*info*/,
        handle_t                        ,
        uuid_t                          *,
        unsigned32                      ,
        unsigned32                      *,
        uuid_t                          *,
        unsigned32                      *
    ));

void rpc__noauth_dg_way_handler _DCE_PROTOTYPE_((
        rpc_auth_info_p_t                /*info*/,
        ndr_byte                        * /*in_data*/,
        signed32                         /*in_len*/,
        signed32                         /*out_max_len*/,
        ndr_byte                        * /*out_data*/,
        signed32                        * /*out_len*/,
        unsigned32                      * /*st*/
    ));

#ifdef __cplusplus
}
#endif

#endif /* _NOAUTHDG_H */
