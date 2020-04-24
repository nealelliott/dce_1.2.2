/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ncadg_plog.h,v $
 * Revision 1.1.6.2  1996/02/18  23:07:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:48  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:59  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:53:18  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:20:50  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:51:27  rsalz
 * 	Edited as part of rpc6 drop.
 * 	[1992/05/01  00:57:57  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:43:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <dce/rpc.h>

typedef unsigned char u_char;
typedef unsigned char byte_t;
typedef unsigned long unsigned32;
typedef unsigned short unsigned16;
typedef unsigned long rpc_clock_t;

#define rpc_c_dg_pt_request           0 /* client -> server */
#define rpc_c_dg_pt_ping              1 /* client -> server */
#define rpc_c_dg_pt_response          2 /* server -> client */
#define rpc_c_dg_pt_fault             3 /* server -> client */
#define rpc_c_dg_pt_working           4 /* server -> client */
#define rpc_c_dg_pt_nocall            5 /* server -> client */
#define rpc_c_dg_pt_reject            6 /* server -> client */
#define rpc_c_dg_pt_ack               7 /* client -> server */
#define rpc_c_dg_pt_quit              8 /* client -> server */
#define rpc_c_dg_pt_fack              9 /* both directions  */
#define rpc_c_dg_pt_quack            10 /* server -> client */

#define rpc_c_dg_pt_max_type rpc_c_dg_pt_quack


typedef struct {
    u_char _rpc_vers;       /* 00:01 RPC version (see RPC_DG_HDR_*_VERS below) */
    u_char _ptype;          /* 01:01 packet type (see RPC_HDR_HDR_*_PTYPE below) */
    u_char flags;           /* 02:01 flags (see rpc_c_dg_pf_... below) */
    u_char flags2;          /* 03:01 more flag (see rpc_c_dg_pf2__... below) */
    u_char drep[3];         /* 04:03 data type format of sender (see below) */
    u_char serial_hi;       /* 07:01 high byte of fragment serial number */
    uuid_t object;          /* 08:16 object UID */
    uuid_t if_id;           /* 24:16 interface UID */
    uuid_t actuid;          /* 40:16 activity UID of caller */
    unsigned32 server_boot; /* 56:04 time server booted */
    unsigned32 if_vers;     /* 60:04 version of interface */
    unsigned32 seq;         /* 64:04 sequence # -- monotonically increasing */
    unsigned16 opnum;       /* 68:02 operation # within the trait */
    unsigned16 ihint;       /* 70:02 interface hint (which interface w/in server) */
    unsigned16 ahint;       /* 72:02 activity hint */
    unsigned16 len;         /* 74:02 length of body */
    unsigned16 fragnum;     /* 76:02 fragment # */
    u_char auth_proto;      /* 78:01 authentication protocol */
    u_char serial_lo;       /* 79:01 low byte of fragment serial number */
} rpc_dg_pkt_hdr_t, *rpc_dg_pkt_hdr_p_t;

#define rpc_c_dg_ptype_mask 0x1f    /* 5 bits wide */

#define RPC_DG_HDR_INQ_PTYPE(hdrp) \
    ((hdrp)->_ptype & rpc_c_dg_ptype_mask)


/*
 * Stuff from dgutl.c (because it's private and it really shouldn't be).
 */

#ifndef rpc_c_dg_pkt_log_size
#  define rpc_c_dg_pkt_log_size 256
#endif

#define MAX_LOGGED_PKT_BODY_LEN 16
#define MAX_LOGGED_PKT_LEN (MAX_LOGGED_PKT_BODY_LEN + sizeof(rpc_dg_pkt_hdr_t))

typedef struct {
    rpc_clock_t         timestamp;
    rpc_dg_pkt_hdr_t    hdr;
    byte_t              body[MAX_LOGGED_PKT_BODY_LEN];
} pktlog_elt_t, *pktlog_elt_p_t;

