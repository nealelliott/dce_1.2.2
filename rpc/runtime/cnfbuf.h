/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnfbuf.h,v $
 * Revision 1.1.62.2  1996/02/18  22:55:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:40  marty]
 *
 * Revision 1.1.62.1  1995/12/08  00:17:14  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:29 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/25  21:39 UTC  lmm
 * 	Merge allocation failure detection cleanup work
 * 
 * 	HP revision /main/lmm_alloc_fail_clnup/1  1995/05/25  21:00 UTC  lmm
 * 	allocation failure detection cleanup
 * 	[1995/12/07  23:57:33  root]
 * 
 * Revision 1.1.60.1  1994/01/21  22:34:15  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:53:50  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  22:58:39  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:59:24  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  20:15:13  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:31:09  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  18:38:15  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  18:26:41  rsalz]
 * 
 * 	30-mon-92 af         Use macro RPC_CN_ALIGN_PTR to align the fbp->data_p
 * 	                     pointer to an 8 byte aligned boundary.  In the
 * 	                     macro RPC_CN_FRAGBUF_SET_DATA_P.
 * 	[1992/05/01  18:07:10  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:08:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CNFBUF_H
#define _CNFBUF_H 1

/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnfbuf.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Definitions of types and interfaces to the fragment buffer
**  routines for connection based protocol services.
**
**
*/

/*
 * NOTE: rpc_c_cn_large_frag_size must always be at least
 * rpc_c_assoc_must_recv_frag_size as defined in cnassm.h. This is
 * an architectural requirement.
 */
#include <cnassm.h>

#if (RPC_C_CN_LARGE_FRAG_SIZE < RPC_C_ASSOC_MUST_RECV_FRAG_SIZE)
#error "large frag size < architecural minimum"
#endif

#define	RPC_C_CN_LG_FRAGBUF_ALLOC_SIZE (sizeof(rpc_cn_fragbuf_t)\
        + RPC_C_CN_LARGE_FRAG_SIZE - 1)

#define RPC_C_CN_SM_FRAGBUF_ALLOC_SIZE (sizeof(rpc_cn_fragbuf_t)\
        + RPC_C_CN_SMALL_FRAG_SIZE - 1 )

EXTERNAL unsigned32 rpc_g_cn_large_frag_size;


/***********************************************************************/
/*
 * R P C _ C N _ F R A G B U F _ P K T _ H D R
 *
 * The unpacked header for a received fragment starts at the used 
 * portion of the header overhead area.
 */

#define RPC_CN_FRAGBUF_PKT_HDR(fbuf) \
    (rpc_cn_packet_p_t) ((rpc_cn_fragbuf_p_t)(fbuf))->data_p

/***********************************************************************/
/*
 * R P C _ C N _ F R A G B U F _ A L L O C
 *
 */

#define RPC_CN_FRAGBUF_ALLOC(fragbuf, size, st)\
    if ((size) <= RPC_C_CN_SMALL_FRAG_SIZE)\
    {\
        (fragbuf) = rpc__cn_fragbuf_alloc (false);\
    }\
    else\
    {\
        (fragbuf) = rpc__cn_fragbuf_alloc (true);\
    }\
    if ((fragbuf) == NULL)\
        *(st) = rpc_s_no_memory;\
    else\
    {\
	(fragbuf)->data_size = (size);\
        *(st) = rpc_s_ok;\
    }

/***********************************************************************/
/*
 * R P C _ C N _ F R A G B U F _ S E T _ D A T A _ P
 *
 */

#define RPC_CN_FRAGBUF_SET_DATA_P(fbp)\
    (fbp)->data_p = (pointer_t) RPC_CN_ALIGN_PTR((fbp)->data_area, 8);

/***********************************************************************/
/*
 * R P C _ _ C N _ F R A G B U F _ F R E E
 *
 */

void rpc__cn_fragbuf_free _DCE_PROTOTYPE_ ((rpc_cn_fragbuf_p_t	/*fragbuf_p*/));


/***********************************************************************/
/*
 * R P C _ _ C N _ S M F R A G B U F _ F R E E
 *
 */

void rpc__cn_smfragbuf_free _DCE_PROTOTYPE_ ((rpc_cn_fragbuf_p_t /*fragbuf_p*/));


/***********************************************************************/
/*
 * R P C _ _ C N _ F R A G B U F _ A L L O C
 *
 */
#define RPC_C_CN_LARGE_FRAGBUF true
#define RPC_C_CN_SMALL_FRAGBUF false

rpc_cn_fragbuf_p_t rpc__cn_fragbuf_alloc _DCE_PROTOTYPE_ ((
    boolean32               /* alloc_large_buf */));


#endif /* _CNFBUF_H */
