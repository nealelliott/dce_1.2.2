/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: prottest.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:53  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:08:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:30  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:07:01  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:42:50  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:38:44  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:11:48  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:36:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#ifndef PROTTEST_H
#define PROTTEST_H

#define TEST_PROTOCOL

/*
 * the value of an invalid interface hint
 */
#define rpc_c_invalid_ihint     0xFFFF

/***********************************************************************/
/*
 * R P C _ P R O T S E Q _ I D _ T
 *
 * The RPC Protocol Sequence IDs.
 */
#define rpc_c_invalid_protseq_id        -1
#define rpc_c_protseq_id_ncacn_ip_tcp   0
#define rpc_c_protseq_id_ncacn_dnet_nsp 1
#define rpc_c_protseq_id_ncacn_osi_nsp  2
#define rpc_c_protseq_id_ncacn_osi_tp4  3
#define rpc_c_protseq_id_ncadg_ip_udp   4
#define rpc_c_protseq_id_ncadg_dds      5
#ifdef TEST_PROTOCOL
#define rpc_c_protseq_id_ncatp_ip_tcp   6
#define rpc_c_protseq_id_max            7
#else
#define rpc_c_protseq_id_max            6
#endif


/***********************************************************************/
/*
 * R P C _ P R O T S E Q _ T
 *
 * The RPC Protocol Sequence Strings.
 */

/*
 * An RPC protocol sequence including '\0'.
 */
#define rpc_c_protseq_max               32


#define rpc_protseq_ncacn_ip_tcp        "ncacn_ip_tcp"
#define rpc_protseq_ncacn_dnet_nsp      "ncacn_dnet_nsp"
#define rpc_protseq_ncacn_osi_nsp       "ncacn_osi_nsp"
#define rpc_protseq_ncacn_osi_tp4       "ncacn_osi_tp4"
#define rpc_protseq_ncadg_ip_udp        "ncadg_ip_udp"
#define rpc_protseq_ncadg_dds           "ncadg_dds"
#ifdef TEST_PROTOCOL
#define rpc_protseq_ncatp_ip_tcp        "ncatp_ip_tcp"
#endif

/***********************************************************************/
/*
 * R P C _ P R O T O C O L _ I D _ T
 *
 * The RPC Protocol IDs.
 */
#define rpc_c_protocol_id_ncacn         0
#define rpc_c_protocol_id_ncadg         1
#define rpc_c_protocol_id_ncatp         2
#define rpc_c_protocol_id_max           3


#endif /* PROTTEST_H */
