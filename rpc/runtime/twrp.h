/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: twrp.h,v $
 * Revision 1.1.320.2  1996/02/18  22:56:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:59  marty]
 *
 * Revision 1.1.320.1  1995/12/08  00:22:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:39  root]
 * 
 * Revision 1.1.318.1  1994/01/21  22:39:27  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  22:00:10  cbrooks]
 * 
 * Revision 1.1.2.4  1993/01/03  23:55:44  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:13:45  bbelch]
 * 
 * Revision 1.1.2.3  1992/12/23  21:18:05  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:45:26  zeliff]
 * 
 * Revision 1.1.2.2  1992/12/21  19:21:35  wei_hu
 * 	      27-oct-92   ebm     Add ip family port and address size constants.
 * 	[1992/12/18  20:28:34  wei_hu]
 * 
 * Revision 1.1  1992/01/19  03:07:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _TWRP_H
#define _TWRP_H	1
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      twrp.h
**
**  FACILITY:
**
**      Protocol Tower Services
**
**  ABSTRACT:
**
**      Private protocol tower service typedefs, constant definitions, etc.
**
**
*/

/*
 * Protocol identifiers for each lower tower floor.
 */
#define TWR_C_FLR_PROT_ID_DNA           0x02 /* DNA     */
#define TWR_C_FLR_PROT_ID_OSI           0x03 /* OSI     */
#define TWR_C_FLR_PROT_ID_NSP           0x04 /* NSP     */
#define TWR_C_FLR_PROT_ID_TP4           0x05 /* TP4     */
#define TWR_C_FLR_PROT_ID_ROUTING       0x06 /* Routing */
#define TWR_C_FLR_PROT_ID_TCP           0x07 /* TCP     */
#define TWR_C_FLR_PROT_ID_UDP           0x08 /* UDP     */
#define TWR_C_FLR_PROT_ID_IP            0x09 /* IP      */

/*
 * Number of lower floors in each address family.
 */
#define TWR_C_NUM_IP_LOWER_FLRS       2  /* Number lower flrs in ip tower  */
#define TWR_C_NUM_DNA_LOWER_FLRS      3  /* Number lower flrs in dna tower */
#define TWR_C_NUM_OSI_LOWER_FLRS      3  /* Number lower flrs in osi tower */
#define TWR_C_NUM_DDS_LOWER_FLRS      2  /* Number lower flrs in dds tower  */

/*
 * Number of bytes overhead per floor = protocol identifier (lhs) count 
 * unsigned (2) + protocol identifier (1) + address data (rhs) count unsigned (2)
 */
#define TWR_C_FLR_OVERHEAD  5  

/*
 * If (and when) the twr facility is separated from rpc, modify 
 * these twr_c_* to replace the rpc_c_* with the underlying constant.
 */

/*
 * Number of bytes in the tower floor count field 
 */

#define  TWR_C_TOWER_FLR_COUNT_SIZE  RPC_C_TOWER_FLR_COUNT_SIZE

/*
 * Number of bytes in the lhs count field of a floor.
 */
#define   TWR_C_TOWER_FLR_LHS_COUNT_SIZE  RPC_C_TOWER_FLR_LHS_COUNT_SIZE

/*
 * Number of bytes in the rhs count field of a floor.
 */
#define   TWR_C_TOWER_FLR_RHS_COUNT_SIZE  RPC_C_TOWER_FLR_RHS_COUNT_SIZE

/*
 * Number of bytes for storing a major or minor version.
 */
#define  TWR_C_TOWER_VERSION_SIZE  RPC_C_TOWER_VERSION_SIZE  

/*
 * Number of bytes for storing a tower floor protocol id or protocol id prefix.
 */

#define  TWR_C_TOWER_PROT_ID_SIZE  RPC_C_TOWER_PROT_ID_SIZE

/*
 * Number of bytes for storing a uuid.
 */
#define  TWR_C_TOWER_UUID_SIZE  RPC_C_TOWER_UUID_SIZE  

/*
 * IP family port and address sizes
 */
#define  TWR_C_IP_PORT_SIZE  2
#define  TWR_C_IP_ADDR_SIZE  4

#endif /* _TWRP_H */
