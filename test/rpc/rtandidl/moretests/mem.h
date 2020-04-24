/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mem.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:36  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:05:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:43:28  root]
 * 
 * Revision 1.1.2.3  1993/01/25  18:04:58  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:37:01  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  22:15:02  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:07:09  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:35:20  devrcs
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



#ifndef RPCMEM_H
#define RPCMEM_H

#ifdef LOGPTS
#define MODULE_MASK             0xff00
#define EVENT_MASK              0x00ff
#define RPC_LOG_INITIALIZE      { if (rpc__log_ptr_init()) {rpc_g_log_ptr = rpc__log_ptr_init();}}
#define LOG_RPC(code)           TIMESTAMP (0x0300 | (EVENT_MASK & code))
#define TIMESTAMP(code)         {*rpc_g_log_ptr = (code);}

typedef unsigned short rpc_logpt_t;
typedef rpc_logpt_t  *rpc_logpt_ptr_t;
extern  rpc_logpt_ptr_t rpc__log_ptr_init ();
extern  rpc_logpt_ptr_t rpc_g_log_ptr;
#else
#define RPC_LOG_INITIALIZE
#define LOG_RPC(code)
#endif

#define	RPC_LOG_CLIENT_STUB_NTR         LOG_RPC (0x00)
#define	RPC_LOG_CLIENT_STUB_XIT         LOG_RPC (0x01)
#define	RPC_LOG_SERVER_STUB_PRE         LOG_RPC (0x02)
#define	RPC_LOG_SERVER_STUB_POST        LOG_RPC (0x03)

#define	RPC_LOG_CALL_START_NTR          LOG_RPC (0x04)
#define	RPC_LOG_CALL_START_XIT          LOG_RPC (0x05)
#define	RPC_LOG_CALL_TRANSMIT_NTR       LOG_RPC (0x06)
#define	RPC_LOG_CALL_TRANSMIT_XIT       LOG_RPC (0x07)
#define	RPC_LOG_CALL_TRANSCEIVE_NTR     LOG_RPC (0x08)
#define	RPC_LOG_CALL_TRANSCEIVE_XIT     LOG_RPC (0x09)
#define	RPC_LOG_CALL_RECEIVE_NTR        LOG_RPC (0x0a)
#define	RPC_LOG_CALL_RECEIVE_XIT        LOG_RPC (0x0b)
#define	RPC_LOG_CALL_END_NTR            LOG_RPC (0x0c)
#define	RPC_LOG_CALL_END_XIT            LOG_RPC (0x0d)

#define RPC_LOG_CN_CALL_START_NTR       LOG_RPC (0x0e)
#define RPC_LOG_CN_CALL_START_XIT       LOG_RPC (0x0f)
#define RPC_LOG_CN_CALL_TRANSMIT_NTR    LOG_RPC (0x10)
#define RPC_LOG_CN_CALL_TRANSMIT_XIT    LOG_RPC (0x11)
#define RPC_LOG_CN_CALL_TRANSCEIVE_NTR  LOG_RPC (0x12)
#define RPC_LOG_CN_CALL_TRANSCEIVE_XIT  LOG_RPC (0x13)
#define RPC_LOG_CN_CALL_RECEIVE_NTR     LOG_RPC (0x14)
#define RPC_LOG_CN_CALL_RECEIVE_XIT     LOG_RPC (0x15)
#define RPC_LOG_CN_CALL_END_NTR         LOG_RPC (0x16)
#define RPC_LOG_CN_CALL_END_XIT         LOG_RPC (0x17)

#define RPC_LOG_CN_CTHD_NTR             LOG_RPC (0x18)
#define RPC_LOG_CN_CTHD_XIT             LOG_RPC (0x19)

#define RPC_LOG_CN_ASSOC_REQ_NTR        LOG_RPC (0x1a)
#define RPC_LOG_CN_ASSOC_REQ_XIT        LOG_RPC (0x1b)
#define RPC_LOG_CN_ASSOC_LIS_NTR        LOG_RPC (0x1c)
#define RPC_LOG_CN_ASSOC_LIS_XIT        LOG_RPC (0x1d)
#define RPC_LOG_CN_ASSOC_ALLOC_NTR      LOG_RPC (0x1e)
#define RPC_LOG_CN_ASSOC_ALLOC_XIT      LOG_RPC (0x1f)
#define RPC_LOG_CN_ASSOC_DEALLOC_NTR    LOG_RPC (0x20)
#define RPC_LOG_CN_ASSOC_DEALLOC_XIT    LOG_RPC (0x21)
#define RPC_LOG_CN_ASSOC_POP_CALL_NTR   LOG_RPC (0x22)
#define RPC_LOG_CN_ASSOC_POP_CALL_XIT   LOG_RPC (0x23)
#define RPC_LOG_CN_ASSOC_PUSH_CALL_NTR  LOG_RPC (0x24)
#define RPC_LOG_CN_ASSOC_PUSH_CALL_XIT  LOG_RPC (0x25)
#define RPC_LOG_CN_ASSOC_Q_FRAG_NTR     LOG_RPC (0x26)
#define RPC_LOG_CN_ASSOC_Q_FRAG_XIT     LOG_RPC (0x27)
#define RPC_LOG_CN_ASSOC_RECV_FRAG_NTR  LOG_RPC (0x28)
#define RPC_LOG_CN_ASSOC_RECV_FRAG_XIT  LOG_RPC (0x29)
#define RPC_LOG_CN_ASSOC_SEND_FRAG_NTR  LOG_RPC (0x2a)
#define RPC_LOG_CN_ASSOC_SEND_FRAG_XIT  LOG_RPC (0x2b)
#define RPC_LOG_CN_ASSOC_SYN_NEG_NTR    LOG_RPC (0x2c)
#define RPC_LOG_CN_ASSOC_SYN_NEG_XIT    LOG_RPC (0x2d)
#define RPC_LOG_CN_ASSOC_SYN_LKUP_NTR   LOG_RPC (0x2e)
#define RPC_LOG_CN_ASSOC_SYN_LKUP_XIT   LOG_RPC (0x2f)
#define RPC_LOG_CN_ASSOC_ACB_CR_NTR     LOG_RPC (0x30)
#define RPC_LOG_CN_ASSOC_ACB_CR_XIT     LOG_RPC (0x31)
#define RPC_LOG_CN_ASSOC_ACB_FR_NTR     LOG_RPC (0x32)
#define RPC_LOG_CN_ASSOC_ACB_FR_XIT     LOG_RPC (0x33)
#define RPC_LOG_CN_ASSOC_ACB_DEAL_NTR   LOG_RPC (0x34)
#define RPC_LOG_CN_ASSOC_ACB_DEAL_XIT   LOG_RPC (0x35)

#define RPC_LOG_CN_GRP_ADDR_LKUP_NTR    LOG_RPC (0x36)
#define RPC_LOG_CN_GRP_ADDR_LKUP_XIT    LOG_RPC (0x37)
#define RPC_LOG_CN_GRP_ID_LKUP_NTR      LOG_RPC (0x38)
#define RPC_LOG_CN_GRP_ID_LKUP_XIT      LOG_RPC (0x39)
#define RPC_LOG_03A                     LOG_RPC (0x3a)
#define RPC_LOG_03B                     LOG_RPC (0x3b)
#define RPC_LOG_03C                     LOG_RPC (0x3c)
#define RPC_LOG_03D                     LOG_RPC (0x3d)
#define RPC_LOG_03E                     LOG_RPC (0x3e)
#define RPC_LOG_03F                     LOG_RPC (0x3f)
#define RPC_LOG_CN_GRP_REMID_LKUP_NTR   LOG_RPC (0x40)
#define RPC_LOG_CN_GRP_REMID_LKUP_XIT   LOG_RPC (0x41)

#define RPC_LOG_SOCKET_OPEN_NTR         LOG_RPC (0x42)
#define RPC_LOG_SOCKET_OPEN_XIT         LOG_RPC (0x43)
#define RPC_LOG_SOCKET_CLOSE_NTR        LOG_RPC (0x44)
#define RPC_LOG_SOCKET_CLOSE_XIT        LOG_RPC (0x45)
#define RPC_LOG_SOCKET_BIND_NTR         LOG_RPC (0x46)
#define RPC_LOG_SOCKET_BIND_XIT         LOG_RPC (0x47)
#define RPC_LOG_SOCKET_CONNECT_NTR      LOG_RPC (0x48)
#define RPC_LOG_SOCKET_CONNECT_XIT      LOG_RPC (0x49)
#define RPC_LOG_SOCKET_ACCEPT_NTR       LOG_RPC (0x4a)
#define RPC_LOG_SOCKET_ACCEPT_XIT       LOG_RPC (0x4b)
#define RPC_LOG_SOCKET_LISTEN_NTR       LOG_RPC (0x4c)
#define RPC_LOG_SOCKET_LISTEN_XIT       LOG_RPC (0x4d)
#define RPC_LOG_SOCKET_SENDMSG_NTR      LOG_RPC (0x4e)
#define RPC_LOG_SOCKET_SENDMSG_XIT      LOG_RPC (0x4f)
#define RPC_LOG_SOCKET_RECVFROM_NTR     LOG_RPC (0x50)
#define RPC_LOG_SOCKET_RECVFROM_XIT     LOG_RPC (0x51)
#define RPC_LOG_SOCKET_RECVMSG_NTR      LOG_RPC (0x52)
#define RPC_LOG_SOCKET_RECVMSG_XIT      LOG_RPC (0x53)
#define RPC_LOG_SOCKET_INQ_EP_NTR       LOG_RPC (0x54)
#define RPC_LOG_SOCKET_INQ_EP_XIT       LOG_RPC (0x55)

#define	RPC_LOG_SELECT_PRE              LOG_RPC (0x56)
#define	RPC_LOG_SELECT_POST             LOG_RPC (0x57)

#define RPC_LOG_MUTEX_INIT_NTR          LOG_RPC (0x58)
#define RPC_LOG_MUTEX_INIT_XIT          LOG_RPC (0x59)
#define RPC_LOG_MUTEX_DELETE_NTR        LOG_RPC (0x5a)
#define RPC_LOG_MUTEX_DELETE_XIT        LOG_RPC (0x5b)
#define RPC_LOG_MUTEX_LOCK_NTR          LOG_RPC (0x5c)
#define RPC_LOG_MUTEX_LOCK_XIT          LOG_RPC (0x5d)
#define RPC_LOG_MUTEX_TRY_LOCK_NTR      LOG_RPC (0x5e)
#define RPC_LOG_MUTEX_TRY_LOCK_XIT      LOG_RPC (0x5f)
#define RPC_LOG_MUTEX_UNLOCK_NTR        LOG_RPC (0x60)
#define RPC_LOG_MUTEX_UNLOCK_XIT        LOG_RPC (0x61)
#define RPC_LOG_MUTEX_LOCK_ASRT_NTR     LOG_RPC (0x62)
#define RPC_LOG_MUTEX_LOCK_ASRT_XIT     LOG_RPC (0x63)
#define RPC_LOG_MUTEX_UNLOCK_ASRT_NTR   LOG_RPC (0x64)
#define RPC_LOG_MUTEX_UNLOCK_ASRT_XIT   LOG_RPC (0x65)
#define RPC_LOG_COND_INIT_NTR           LOG_RPC (0x66)
#define RPC_LOG_COND_INIT_XIT           LOG_RPC (0x67)
#define RPC_LOG_COND_DELETE_NTR         LOG_RPC (0x68)
#define RPC_LOG_COND_DELETE_XIT         LOG_RPC (0x69)
#define RPC_LOG_COND_WAIT_NTR           LOG_RPC (0x6a)
#define RPC_LOG_COND_WAIT_XIT           LOG_RPC (0x6b)
#define RPC_LOG_COND_TIMED_WAIT_NTR     LOG_RPC (0x6c)
#define RPC_LOG_COND_TIMED_WAIT_XIT     LOG_RPC (0x6d)
#define RPC_LOG_COND_SIGNAL_NTR         LOG_RPC (0x6e)
#define RPC_LOG_COND_SIGNAL_XIT         LOG_RPC (0x6f)
#define RPC_LOG_COND_BROADCAST_NTR      LOG_RPC (0x70)
#define RPC_LOG_COND_BROADCAST_XIT      LOG_RPC (0x71)

#define RPC_LOG_MEM_ALLOC_NTR           LOG_RPC (0x72)
#define RPC_LOG_MEM_ALLOC_XIT           LOG_RPC (0x73)
#define RPC_LOG_MEM_REALLOC_NTR         LOG_RPC (0x74)
#define RPC_LOG_MEM_REALLOC_XIT         LOG_RPC (0x75)
#define RPC_LOG_MEM_FREE_NTR            LOG_RPC (0x76)
#define RPC_LOG_MEM_FREE_XIT            LOG_RPC (0x77)

#define RPC_LOG_LIST_ELT_ALLOC_NTR      LOG_RPC (0x78)
#define RPC_LOG_LIST_ELT_ALLOC_XIT      LOG_RPC (0x79)
#define RPC_LOG_LIST_ELT_FREE_NTR       LOG_RPC (0x7a)
#define RPC_LOG_LIST_ELT_FREE_XIT       LOG_RPC (0x7b)

#define RPC_LOG_IF_LOOKUP_NTR           LOG_RPC (0x7c)
#define RPC_LOG_IF_LOOKUP_XIT           LOG_RPC (0x7d)
#define RPC_LOG_07E                     LOG_RPC (0x7e)
#define RPC_LOG_07F                     LOG_RPC (0x7f)

#define RPC_LOG_NAF_ADDR_ALLOC_NTR      LOG_RPC (0x80)
#define RPC_LOG_NAF_ADDR_ALLOC_XIT      LOG_RPC (0x81)
#define RPC_LOG_NAF_ADDR_COPY_NTR       LOG_RPC (0x82)
#define RPC_LOG_NAF_ADDR_COPY_XIT       LOG_RPC (0x83)
#define RPC_LOG_NAF_ADDR_FREE_NTR       LOG_RPC (0x84)
#define RPC_LOG_NAF_ADDR_FREE_XIT       LOG_RPC (0x85)

#define RPC_LOG_UUID_EQUAL_NTR          LOG_RPC (0x86)
#define RPC_LOG_UUID_EQUAL_XIT          LOG_RPC (0x87)
#define RPC_LOG_088                     LOG_RPC (0x88)
#define RPC_LOG_089                     LOG_RPC (0x89)
#define RPC_LOG_UUID_HASH_NTR           LOG_RPC (0x8a)
#define RPC_LOG_UUID_HASH_XIT           LOG_RPC (0x8b)
#define RPC_LOG_UUID_CREATE_NTR         LOG_RPC (0x8c)
#define RPC_LOG_UUID_CREATE_XIT         LOG_RPC (0x8d)

#define RPC_LOG_CN_RCV_PKT_NTR          LOG_RPC (0x8e)
#define RPC_LOG_CN_RCV_PKT_XIT          LOG_RPC (0x8f)
#define RPC_LOG_090                     LOG_RPC (0x90)
#define RPC_LOG_091                     LOG_RPC (0x91)
#define RPC_LOG_CN_PROCESS_PKT_NTR      LOG_RPC (0x92)
#define RPC_LOG_CN_PROCESS_PKT_XIT      LOG_RPC (0x93)

#define RPC_LOG_SERVER_COND_WAIT_PRE    LOG_RPC (0x94)
#define RPC_LOG_SERVER_COND_WAIT_POST   LOG_RPC (0x95)
#define RPC_LOG_SERVER_RECVMSG_PRE      LOG_RPC (0x96)
#define RPC_LOG_SERVER_RECVMSG_POST     LOG_RPC (0x97)
#define RPC_LOG_SERVER_SENDMSG_PRE      LOG_RPC (0x98)
#define RPC_LOG_SERVER_SENDMSG_POST     LOG_RPC (0x99)
#define RPC_LOG_SERVER_YIELD_PRE        LOG_RPC (0x9a)
#define RPC_LOG_SERVER_YIELD_POST       LOG_RPC (0x9b)
#define RPC_LOG_SERVER_COND_SIG_PRE     LOG_RPC (0x9c)
#define RPC_LOG_SERVER_COND_SIG_POST    LOG_RPC (0x9d)

#define RPC_LOG_CLIENT_COND_WAIT_PRE    LOG_RPC (0x9e)
#define RPC_LOG_CLIENT_COND_WAIT_POST   LOG_RPC (0x9f)
#define RPC_LOG_CLIENT_RECVMSG_PRE      LOG_RPC (0xa0)
#define RPC_LOG_CLIENT_RECVMSG_POST     LOG_RPC (0xa1)
#define RPC_LOG_CLIENT_SENDMSG_PRE      LOG_RPC (0xa2)
#define RPC_LOG_CLIENT_SENDMSG_POST     LOG_RPC (0xa3)
#define RPC_LOG_CLIENT_YIELD_PRE        LOG_RPC (0xa4)
#define RPC_LOG_CLIENT_YIELD_POST       LOG_RPC (0xa5)
#define RPC_LOG_CLIENT_COND_SIG_PRE     LOG_RPC (0xa6)
#define RPC_LOG_CLIENT_COND_SIG_POST    LOG_RPC (0xa7)

#define RPC_LOG_SERVER_LOOP_NTR         LOG_RPC (0xa8)
#define RPC_LOG_SERVER_LOOP_XIT         LOG_RPC (0xa9)
#define RPC_LOG_CLIENT_LOOP_NTR         LOG_RPC (0xaa)
#define RPC_LOG_CLIENT_LOOP_XIT         LOG_RPC (0xab)

#define rpc_c_mem_avail             0
#define rpc_c_mem_dg_chand          1       /* rpc_dg_handle_client_t       */
#define rpc_c_mem_dg_shand          2       /* rpc_dg_handle_server_t       */
#define rpc_c_mem_dg_ccall          3       /* rpc_dg_ccall_t               */
#define rpc_c_mem_dg_scall          4       /* rpc_dg_scall_t               */
#define rpc_c_mem_dg_rqe            5       /* rpc_dg_recvq_elt_t           */
#define rpc_c_mem_dg_xqe            6       /* rpc_dg_xmitq_elt_t           */
#define rpc_c_mem_dg_rawpkt         7       /* rpc_dg_raw_pkt_t             */
#define rpc_c_mem_dg_pktbody        8       /* rpc_dg_pkt_body_t            */
#define rpc_c_mem_dg_pktlog         9       /* from dg pktlog()             */
#define rpc_c_mem_dg_ccte          10       /* rpc_dg_cct_elt_t             */
#define rpc_c_mem_dg_scte          11       /* rpc_dg_sct_elt_t             */
#define rpc_c_mem_if_rgy_entry     12       /* rpc_if_rgy_entry_t           */
#define rpc_c_mem_util             13       /* from rpc_util_malloc()       */
#define rpc_c_mem_sock_info        14       /* rpc_sock_info_t              */
#define rpc_c_mem_iove_list        15       /* rpc_iove_list_elt_t          */
#define rpc_c_mem_dg_lossy         16       /* misc stuff for dglossy.c     */
#define rpc_c_mem_v1_pktbody       17
                                   /* pkt buff rpc__pre_v2_iface_server_call */
#define rpc_c_mem_v1_stub          18       /* rpc1_{alloc,free}_pkt()      */
#define rpc_c_mem_if_type_info     19       /* rpc_if_type_info_t           */
#define rpc_c_mem_lookaside_list   20       /* generic cn lookaside list elt */
#define rpc_c_mem_rpc_addr         21       /* rpc_addr + naf data          */
#define rpc_c_mem_dg_client_rep    22       /* rpc_dg_client_rep_t          */
#define rpc_c_mem_dg_maint         23       /* maintain liveness structure  */
#define rpc_c_mem_uuid             24       /* uuid_t                       */
#define rpc_c_mem_nsentry          25       /* rpc_nsentry_t                */
#define rpc_c_mem_nsattr           26       /* rpc_nsattr_t                 */
#define rpc_c_mem_nsuuid           27       /* rpc_nsuuid_t                 */
#define rpc_c_mem_nstower          28       /* rpc_nstower_t                */
#define rpc_c_mem_tower            29       /* rpc_tower_t                  */
#define rpc_c_mem_syntax_id        30       /* rpc_syntax_id_t              */
#define rpc_c_mem_binding_vec      31       /* rpc_binding_vector_t         */
#define rpc_c_mem_cn_assoc_grp_blk 32       
                        /* rpc_assoc_group_t[rpc_c_assoc_grp_tbl_alloc_size] */
#define rpc_c_mem_cn_assoc         33       /* rpc_assoc_t                  */
#define rpc_c_mem_cn_call_rep      34       /* rpc_cn_call_rep_t            */
#define rpc_c_mem_cn_lg_fragbuf    35       /* lg_fragbuf_alloc_size        */
#define rpc_c_mem_cn_sm_fragbuf    36       /* sm_fragbuf_alloc_size        */
#define rpc_c_mem_cn_binding_rep   37       /* rpc_cn_binding_rep_t         */
#define rpc_c_mem_import           38       /* rpc_import_rep_t             */
#define rpc_c_mem_if_rep           39       /* rpc_if_rep_t                 */
#define rpc_c_mem_lkup_rep         40       /* rpc_lkup_rep_t               */
#define rpc_c_mem_lkup_node        41       /* rpc_lkup_node_t              */
#define rpc_c_mem_lkup_mbr         42       /* rpc_lkup_mbr_t               */
#define rpc_c_mem_inq_rep          43       /* rpc_inq_rep_t                */
#define rpc_c_mem_nsprofile_elt    44       /* rpc_nsprofile_elt_t          */
#define rpc_c_mem_nsprofile_octet_str 45    /* rpc_profile_elt_octet_string_t */
#define rpc_c_mem_binding_vector   46       /* rpc_binding_vector_t         */
#define rpc_c_mem_cn_syntax        47       /* rpc_syntax_t                 */
#define rpc_c_mem_rpc_addr_vec     48       /* rpc_addr + naf data          */
#define rpc_c_mem_if_id            49       /* rpc_if_id_t                  */
#define rpc_c_mem_if_id_vector     50       /* rpc_if_id_vector_t           */
#define rpc_c_mem_protseq_vector   51       /* rpc_protseq_vector_t         */
#define rpc_c_mem_string           52       /* various string types         */
#define rpc_c_mem_mgr_epv          53       /* manager epv                  */
#define rpc_c_mem_ep_entry         54       /* endpoint mapper struct       */
#define rpc_c_mem_uuid_vector      55       /* uuid_vector_t                */
#define rpc_c_mem_obj_rgy_entry    56       /* rpc_obj_rgy_entry_t          */
#define rpc_c_mem_thread_context   57       /* rpc_thread_context_t         */
#define rpc_c_mem_comnl_nit_elt    58       /* rpc_comnl_nit_elt_t          */
#define rpc_c_mem_nsresolution     59       /* portion of resolved name     */
#define rpc_c_mem_nsresidual       60       /* portion of unresolved name   */
    /* can only use up to "rpc_c_mem_maxtypes - 1" without upping it */
#define rpc_c_mem_max_types        61       /* i.e. 0 : (max_types - 1)     */



/*
 * RPC memory use statistics database and database mutex.
 * This is a LEVEL 3 mutex.
 *
 * See rpcglob.[ch] for the actual database and lock.
 */

typedef struct 
{
    unsigned32 inuse;         /* number currently allocated */
    unsigned32 calls;         /* total ever allocated */
    unsigned32 fails;         /* denied alloc requests */
    unsigned32 maxsize;       /* max size allocated for this type */
} rpc_mem_stats_elt_t, *rpc_mem_stats_elt_p_t;

extern rpc_mem_stats_elt_t rpc_g_mem_stats[];

/*
 * Values for the 'flags' argument to RPC_MEM_ALLOC
 */

#define rpc_c_mem_waitok    0
#define rpc_c_mem_nowait    1


/*
 * Concurrency control for the mem statistics database.
 * We avoid the locking for the moment... after all, this is
 * only statistics stuff.
 */

#ifdef NOTDEF
#define RPC_MEM_LOCK_INIT(junk)         RPC_MUTEX_INIT(rpc_g_global_mutex)
#define RPC_MEM_LOCK(junk)              RPC_MUTEX_LOCK(rpc_g_global_mutex)
#define RPC_MEM_UNLOCK(junk)            RPC_MUTEX_UNLOCK(rpc_g_global_mutex)
#define RPC_MEM_TRY_LOCK(bp)            RPC_MUTEX_TRY_LOCK(rpc_g_global_mutex,(bp))
#define RPC_MEM_LOCK_DELETE(junk)       RPC_MUTEX_DELETE(rpc_g_global_mutex)
#define RPC_MEM_LOCK_ASSERT(junk)       RPC_MUTEX_LOCK_ASSERT(rpc_g_global_mutex)
#define RPC_MEM_UNLOCK_ASSERT(junk)     RPC_MUTEX_UNLOCKED_ASSERT(rpc_g_global_mutex)
#else
#define RPC_MEM_LOCK_INIT(junk)
#define RPC_MEM_LOCK(junk)
#define RPC_MEM_UNLOCK(junk)
#define RPC_MEM_TRY_LOCK(bp)
#define RPC_MEM_LOCK_DELETE(junk)
#define RPC_MEM_LOCK_ASSERT(junk)
#define RPC_MEM_UNLOCK_ASSERT(junk)
#endif


/*
 * R P C _ M E M _ A L L O C
 * 
 * (addr) == NULL iff "no memory available"
 *
 * Sample usage:
 *      rpc_dg_ccall_p_t ccall;
 *      RPC_MEM_ALLOC(ccall, rpc_dg_ccall_p_t, sizeof *rpc_dg_ccall_p_t, 
 *              rpc_c_mem_dg_ccall, rpc_c_mem_nowait);
 *      if (ccall == NULL)
 *          alloc failed
 *
 * Note that we just raise an exception if the malloc fails (since most
 * callers don't yet check the return value).  In any case, this will
 * probably always be the correct thing to do in a user space NCK
 * implementation).
 */

#define RPC_MEM_ALLOC(addr, cast, size, type, flags) \
{ \
    RPC_LOG_MEM_ALLOC_NTR; \
    (addr) = (cast) malloc(size); \
    RPC_MEM_LOCK (0); \
    rpc_g_mem_stats[type].calls++; \
    if ((addr) == NULL) { \
        rpc_g_mem_stats[type].fails++; \
        exc_raise_status(rpc_s_no_memory); \
    } else \
        rpc_g_mem_stats[type].inuse++; \
    if ((size) > rpc_g_mem_stats[type].maxsize) \
        rpc_g_mem_stats[type].maxsize = (size); \
    RPC_MEM_UNLOCK (0); \
    RPC_LOG_MEM_ALLOC_XIT; \
}


/*
 * R P C _ M E M _ R E A L L O C
 * 
 * (addr) == NULL iff "no memory available"
 *
 * Sample usage:
 *      rpc_dg_ccall_p_t ccall;
 *      RPC_MEM_REALLOC(ccall, rpc_dg_ccall_p_t, sizeof *rpc_dg_ccall_p_t, 
 *              rpc_c_mem_dg_ccall, rpc_c_mem_nowait);
 *      if (ccall == NULL)
 *          alloc failed
 *
 * Note that we just raise an exception if the realloc fails (since most
 * callers don't yet check the return value).  In any case, this will
 * probably always be the correct thing to do in a user space NCK
 * implementation).
 */

#define RPC_MEM_REALLOC(addr, cast, size, type, flags) \
{ \
    RPC_LOG_MEM_REALLOC_NTR; \
    (addr) = (cast) realloc(addr, size); \
    RPC_MEM_LOCK (0); \
    rpc_g_mem_stats[type].calls++; \
    if ((addr) == NULL) { \
        rpc_g_mem_stats[type].fails++; \
        exc_raise_status(rpc_s_no_memory); \
    } else \
        rpc_g_mem_stats[type].inuse++; \
    if ((size) > rpc_g_mem_stats[type].maxsize) \
        rpc_g_mem_stats[type].maxsize = (size); \
    RPC_MEM_UNLOCK (0); \
    RPC_LOG_MEM_REALLOC_XIT; \
}


/*
 * R P C _ M E M _ F R E E
 *
 * Sample useage:
 *      ...
 *      RPC_MEM_FREE(ccall, rpc_c_mem_dg_ccall);
 */

#define RPC_MEM_FREE(addr, type) \
{ \
    RPC_LOG_MEM_FREE_NTR; \
    free(((cma_t_address) (addr))); \
    RPC_MEM_LOCK (0); \
    --rpc_g_mem_stats[type].inuse; \
    RPC_MEM_UNLOCK (0); \
    RPC_LOG_MEM_FREE_XIT; \
}

#endif
