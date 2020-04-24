/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnrcvr.c,v $
 * Revision 1.1.118.1  1996/10/15  20:47:04  arvind
 * 	Initialize call rep packet count to squelch a purify warning.
 * 	[1996/10/10  23:57 UTC  sommerfeld  /main/sommerfeld_pk_kdc_2/1]
 *
 * Revision 1.1.114.2  1996/02/18  00:02:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:44  marty]
 * 
 * Revision 1.1.114.1  1995/12/08  00:17:31  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/gaz_dce_instr/jrr_1.2_mothra/1  1995/11/16  21:30 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/gaz_dce_instr/4  1995/08/17  04:18 UTC  gaz
 * 	Ensure that DMS is NOT built in the Kernel RPC
 * 
 * 	HP revision /main/HPDCE02/gaz_dce_instr/3  1995/08/04  20:41 UTC  gaz
 * 	DMS code cleanup
 * 
 * 	HP revision /main/HPDCE02/gaz_dce_instr/2  1995/07/25  14:51 UTC  gaz
 * 	Timestamp call arrival when calltag is allocated
 * 	add conditional compilation directives
 * 
 * 	HP revision /main/HPDCE02/gaz_dce_instr/1  1995/07/15  20:24 UTC  gaz
 * 	attach DMS calltag to call_rep
 * 
 * 	HP revision /main/HPDCE02/3  1995/04/02  01:13 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/HPDCE02/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:17 UTC  lmm
 * 	add memory allocation failure detection
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/31  21:15 UTC  tatsu_s
 * 	Submitted the local rpc cleanup.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.vtalarm.b0/1  1995/01/26  20:35 UTC  tatsu_s
 * 	Call rpc__local_socket_close() for the client sockets.
 * 
 * 	HP revision /main/HPDCE02/1  1995/01/04  18:25 UTC  tatsu_s
 * 	Submitted CN orpahned assoc fix.
 * 
 * 	HP revision /main/tatsu_s.mothra_cn_fix.b0/1  1995/01/04  02:07 UTC  tatsu_s
 * 	Fixed an orphaned assoc.
 * 	[1995/12/07  23:57:44  root]
 * 
 * Revision 1.1.111.9  1994/09/01  16:12:54  ganni
 * 	Use RPC_CN_AUTH_REQUIRED macro, instead of just checking the
 * 	NULL pointer.
 * 	[1994/09/01  16:12:22  ganni]
 * 
 * Revision 1.1.111.8  1994/08/25  19:31:14  ganni
 * 	If unauthenticated response is received for an authenticated
 * 	request, exit from receive_dispatch() with st set to
 * 	rpc_s_authn_level_mismatch.
 * 	[1994/08/25  19:27:40  ganni]
 * 
 * Revision 1.1.111.7  1994/08/05  19:49:42  tom
 * 	Bug 11358 - Translate status to message on call_failed errors.
 * 	[1994/08/05  19:06:35  tom]
 * 
 * Revision 1.1.111.6  1994/07/14  21:31:10  tom
 * 	  27-apr-93	    us  Removed the predicate table as last
 * 		        step to the performance enhancements
 * 		        to rpc__cn_sm_event_eval.  This amounts
 * 		        to changing call parameters to
 * 	       	        rpc__cn_sm_init(). For an explanation
 * 		        of the full set of performance changes,
 * 		        see cnsm.c comments.
 * 	[1994/07/11  16:51:20  tom]
 * 
 * Revision 1.1.111.5  1994/06/21  21:53:41  hopkins
 * 	Merge up to DCE1_1.
 * 	[1994/06/08  21:29:30  hopkins]
 * 
 * 	More serviceability
 * 	[1994/05/31  19:51:47  hopkins]
 * 
 * 	Serviceability:
 * 	  Use RPC_DCE_SVC_PRINTF instead of EPRINTF
 * 	  and fprintf.
 * 	[1994/05/24  22:11:42  hopkins]
 * 
 * Revision 1.1.111.4  1994/05/19  21:14:29  hopkins
 * 	Merge with DCE1_1.
 * 	[1994/05/04  19:37:56  hopkins]
 * 
 * 	Serviceability work.
 * 	[1994/05/03  20:18:27  hopkins]
 * 
 * Revision 1.1.111.3  1994/03/17  23:06:38  tom
 * 	Big PAC -
 * 	  Don't check any packet of type bind or bind_ack for version mismatch.
 * 	  Check packet minor version number against the one stored in the
 * 	  association, we may have negotiated a lower one.
 * 	[1994/03/17  23:04:15  tom]
 * 
 * Revision 1.1.111.2  1994/01/21  22:34:39  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:54:21  cbrooks]
 * 
 * Revision 1.1.111.1  1994/01/14  20:31:20  tom
 * 	Big Pac - Check packet minor version number against the one
 * 	stored in the association, we may have negotiated a lower one.
 * 	Don't check any packet of type bind for version numbers.
 * 	[1994/01/13  22:51:13  tom]
 * 
 * Revision 1.1.6.3  1993/10/25  21:39:44  tom
 * 	Bug 9240 - Explicitly catch cancels during receive_packet.
 * 	We set the status to connection_closed so clanup will be
 * 	done correctly (assoc removed from group).
 * 	This doesn't seem to completely fix this problem, but should help.
 * 	[1993/10/25  21:39:22  tom]
 * 
 * Revision 1.1.6.2  1993/10/18  19:26:29  tom
 * 	Bug 9090 - In rpc__cn_network_receiver, dont use
 * 	assoc (in while condition) after you have dealloc'd it.
 * 	Catch the cancel that rpc__cn_assoc_acb_free() will post to us
 * 	and set a flag to exit while.
 * 	[1993/10/18  15:17:05  tom]
 * 
 * Revision 1.1.6.1  1993/10/06  19:58:46  ganni
 * 	Bug 8068 - Initialize the Iovectors in the callrep from 1.
 * 	[1993/10/06  19:58:16  ganni]
 * 
 * Revision 1.1.4.11  1993/04/09  14:54:57  wei_hu
 * 	           8-apr-93    wh      Tighten the check for invalid packets:
 * 	                               add more sanity check on ptype and also
 * 	                               check the protocol versions on every
 * 	                               packet after the first (presumably a
 * 	                               BIND).
 * 	[1993/04/09  14:50:06  wei_hu]
 * 
 * Revision 1.1.4.10  1993/03/05  19:20:02  weisman
 * 	      1-mar-93    wh      Initialize all the iovector elements in
 * 	                          the call rep.
 * 	     25-feb-93    dm      In rpc__cn_assoc_acb_dealloc(), call
 * 	                          pthread_testcancel() to be certain there
 * 	                          is no pending cancel on the receiver thread.
 * 	     30-jan-93    bb      add pthread_testcancel after
 * 	                          pthread_setasynccancel
 * 	18-dec-92    ko      Check the assoc_status and assoc_local_status
 * 			     after reaquiring the CN lock after the calls
 * 			     to recvmsg() and cond_wait(). This will handle
 * 			     the case of other threads detecting errors on the
 * 			     association, cleaning up and then telling the
 * 			     receiver thread to close the connection.
 * 	[1993/03/05  19:07:53  weisman]
 * 
 * Revision 1.1.4.9  1993/02/05  16:04:11  raizen
 * 	 27-jan-93    dr      Init call_r->prot_tlr to NULL in receive_packet
 * 	[1993/02/04  23:18:39  raizen]
 * 
 * Revision 1.1.4.8  1993/01/03  22:59:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:00:18  bbelch]
 * 
 * Revision 1.1.4.7  1992/12/23  20:17:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:31:59  zeliff]
 * 
 * Revision 1.1.4.6  1992/12/18  19:54:41  wei_hu
 * 	      3-dec-92    ko      No longer copy transfer syntax
 * 	                          UUID into presentation context
 * 	                          element. The stub does not require it.
 * 	      3-nov-92    wh      Enable async cancels across recvmsg.
 * 	08-oct-92    dr      In receive_packet, move up the wait for an
 * 	                          outstanding send to complete to cover the error
 * 	                          path as well as the normal path, so socket won't
 * 	                          be closed while send is outstanding.
 * 	[1992/12/18  19:44:44  wei_hu]
 * 
 * Revision 1.1.4.5  1992/11/19  20:39:03  marcyw
 * 	     Backing out questionable changes
 * 	     [1992/11/19  20:09:38  marcyw]
 * 
 * Revision 1.1.4.3  1992/11/13  21:07:39  zeliff
 * 	     Backing out questionable changes
 * 	     [1992/11/13  21:01:01  zeliff]
 * 	Revision 1.1.2.4  1992/07/07  21:28:11  rsalz
 * 	     25-jun-92    wh      Do not special-case handling of protocol
 * 	                          versions when DEBUG turned on.
 * 	     25-jun-92    ko      Use RPC_CN_ASSOC_WAKEUP macro to notify
 * 	                          waiting threads of results.
 * 	     19-jun-92    ko      Unpack the header on authenticated call
 * 	                          request packets before adjusting the
 * 	                          frag length for stub padding.
 * 	[1992/07/07  21:24:17  rsalz]
 * 
 * Revision 1.1.2.3  1992/05/22  18:57:48  wei_hu
 * 	19-may-92    ko      Use bit fields in assoc cn_ctlblk to
 * 	                     synchronize access to state machines
 * 	                     between sender and receiver threads now that
 * 	                     global mutex is being released in sender thread.
 * 	12-apr-92    sp      Fix RPC_CN_AUTH_CVT_ID* invocations.
 * 	30-mar-92    af      Fix pointer arithmetic for systems with 64 bit
 * 	                     pointers.  Change cast from (unsigned32) to
 * 	                     (unsigned8 *).
 * 	 16-mar-92   sp      Copy raw packet when processing bind or
 * 	                     alter-context PDUs so that security processing
 * 	                     will work correctly.
 * 	                     Various changes for interoperability.
 * 	[1992/05/22  18:43:25  wei_hu]
 * 
 * Revision 1.1  1992/01/19  03:12:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnrcvr.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  The NCA Connection Protocol Service's Receiver Service.
**
**
*/

#include <commonp.h>    /* Common declarations for all RPC runtime */
#include <com.h>        /* Common communications services */
#include <ndrp.h>       /* System (machine architecture) dependent definitions */
#include <cnp.h>        /* NCA Connection private declarations */
#include <cnnet.h>      /* NCA Connection network service */
#include <cnsm.h>       /* NCA Connection state machine service */
#include <cnassm.h>     /* NCA Connection association state machine */
#include <cnclsm.h>     /* NCA Connection call state machine */
#include <cnpkt.h>      /* NCA Connection packet encoding */
#include <cnfbuf.h>     /* NCA Connection fragment buffer service */
#include <cnassoc.h>    /* NCA Connection association service */
#include <cnrcvr.h>     /* NCA Connection receiver service */
#include <comauth.h>    /* Externals for Auth. Services sub-component */
#include <cncall.h>     /* NCA connection call service */
#include <comcthd.h>    /* Externals for call thread services component */
#include <cncthd.h>     /* NCA Connection call executor service */



/******************************************************************************/
/*
 * Internal variables
 */
/******************************************************************************/
/*
 * P A C K E T _ I N F O _ T A B L E
 *
 * Call and Association packet event information table.
 * This table is indexed by packet type.
 */

typedef struct
{
    unsigned8 class;
    unsigned8 event;
} rpc_cn_pkt_info_t, *rpc_cn_pkt_info_p_t;

#define CALL_CLASS_PKT  0       /* packet is a call related packet */
#define ASSOC_CLASS_PKT 1       /* packet is an association related packet */
#define DGRAM_CLASS_PKT 2       /* packet is a datagram related packet (illegal) */


#ifndef RPC_C_DGRAM_TYPE_PKT
#define RPC_C_DGRAM_TYPE_PKT 0xff     /* this is an arbitrary value */
#endif

INTERNAL rpc_cn_pkt_info_t packet_info_table[] =
{
    { CALL_CLASS_PKT,  RPC_C_CALL_RPC_IND },            /* 00 - request */
    { DGRAM_CLASS_PKT, RPC_C_DGRAM_TYPE_PKT },          /* 01 - ping */
    { CALL_CLASS_PKT,  RPC_C_CALL_RPC_CONF },           /* 02 - response */
    { CALL_CLASS_PKT,  RPC_C_CALL_FAULT },              /* 03 - fault */
    { DGRAM_CLASS_PKT, RPC_C_DGRAM_TYPE_PKT },          /* 04 - working */
    { DGRAM_CLASS_PKT, RPC_C_DGRAM_TYPE_PKT },          /* 05 - nocall */
    { DGRAM_CLASS_PKT, RPC_C_DGRAM_TYPE_PKT },          /* 06 - reject */
    { DGRAM_CLASS_PKT, RPC_C_DGRAM_TYPE_PKT },          /* 07 - ack */
    { DGRAM_CLASS_PKT, RPC_C_DGRAM_TYPE_PKT },          /* 08 - quit */
    { DGRAM_CLASS_PKT, RPC_C_DGRAM_TYPE_PKT },          /* 09 - fack */
    { DGRAM_CLASS_PKT, RPC_C_DGRAM_TYPE_PKT },          /* 10 - quack */
    { ASSOC_CLASS_PKT, RPC_C_ASSOC_IND },               /* 11 - bind */
    { ASSOC_CLASS_PKT, RPC_C_ASSOC_ACCEPT_CONF },       /* 12 - bind ack */
    { ASSOC_CLASS_PKT, RPC_C_ASSOC_REJECT_CONF },       /* 13 - bind nak */
    { ASSOC_CLASS_PKT, RPC_C_ASSOC_ALTER_CONTEXT_IND }, /* 14 - alter context */
    { ASSOC_CLASS_PKT, RPC_C_ASSOC_ALTER_CONTEXT_CONF },/* 15 - alter context response */
    { ASSOC_CLASS_PKT, RPC_C_ASSOC_AUTH3_IND },         /* 16 - auth3 */
    { ASSOC_CLASS_PKT, RPC_C_ASSOC_SHUTDOWN_IND },      /* 17 - shutdown */
    { CALL_CLASS_PKT,  RPC_C_CALL_REMOTE_ALERT_IND },   /* 18 - remote alert */
    { CALL_CLASS_PKT,  RPC_C_CALL_ORPHANED }            /* 19 - orphaned */
};


/******************************************************************************/
/*
 * Internal routine declarations
 */
/******************************************************************************/
/*
 * R E C E I V E _ D I S P A T C H
 */
INTERNAL void receive_dispatch _DCE_PROTOTYPE_ ((
        rpc_cn_assoc_p_t        /*assoc*/
    ));

/*
 * R E C E I V E _ P A C K E T
 */
INTERNAL void receive_packet _DCE_PROTOTYPE_ ((
        rpc_cn_assoc_p_t        /*assoc*/,
        rpc_cn_fragbuf_p_t      * /*fragbuf_p*/,
        rpc_cn_fragbuf_p_t      * /*ovf_fragbuf_p*/,
        unsigned32              * /*st*/
    ));

/*
 * R P C _ C N _ S E N D _ F A U L T 
 *
 * This macro will cause a fault PDU to be sent back to the client
 * and will terminate the RPC.
 */
#define RPC_CN_SEND_FAULT(call_r, st) \
{\
    rpc_binding_rep_t   *binding_r; \
\
    rpc__cn_call_reject ((rpc_call_rep_p_t) call_r, st);\
    binding_r = (rpc_binding_rep_t *) (call_r)->binding_rep; \
    RPC_CN_UNLOCK (); \
    rpc__cn_call_end ((rpc_call_rep_p_t *) &(call_r), &st); \
    RPC_CN_LOCK (); \
    RPC_BINDING_RELEASE (&binding_r, \
                         &st); \
}


/*
**++
**  ROUTINE NAME:       rpc__cn_network_receiver
**
**  SCOPE:              PRIVATE - declared in cnrcvr.h
**
**  DESCRIPTION:        
**
**  This routine constitutes the top-level receiver thread (both client and
**  server) and is invoked by "thread create" in "association
**  lookaside alloc" routine to process incoming packets. 
**
**  It receives packets on an association until terminated by a
**  cancel, the connection breaks or a resource exhaustion problem
**  is hit.
**
**  INPUTS:
**
**      assoc             pointer to an association control block
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       Posts events to the association and call state machines.
**
**--
*/

PRIVATE void rpc__cn_network_receiver 
#ifdef _DCE_PROTO_
(
  rpc_cn_assoc_p_t        assoc
)
#else
(assoc)
rpc_cn_assoc_p_t        assoc;
#endif
{
    rpc_socket_error_t  serr;
    boolean             done=false;

    RPC_CN_DBG_RTN_PRINTF (rpc__cn_network_receiver);

    RPC_DBG_PRINTF (rpc_e_dbg_threads, RPC_C_CN_DBG_THREADS,
        ("####### assoc->%x Entered receiver thread \n", assoc));

    RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                    ("CN: assoc->%x call_rep->none Receiver thread starting...\n",
                     assoc));

    /*
     * Loop until a cancel is sent to this thread.
     */
    while (!done && !assoc->cn_ctlblk.exit_rcvr)
    {
        RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                        ("CN: assoc->%x call_rep->none Entering receive loop...\n",
                         assoc));

        /*
         * Lock the global connection mutex to prevent other threads
         * from running while the receiver thread is. This mutex will be
         * released when we block (either explicitly or implicitly by a
         * condition variable wait).
         */
        RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                        ("CN: Attemping to lock global mutex\n"));
        RPC_CN_LOCK ();
        RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                        ("CN: Global mutex locked\n"));
        
        /*
         * Wait for a session/transport connection to be established.
         */
        TRY
        {
            while (assoc->cn_ctlblk.cn_state != RPC_C_CN_OPEN)
            {
                assoc->cn_ctlblk.cn_rcvr_waiters++;
                RPC_DBG_PRINTF (rpc_e_dbg_threads, RPC_C_CN_DBG_THREADS,
                    ("####### assoc->%x Waiting for new connection \n", assoc));
                TRY 
                {
                    RPC_COND_WAIT (assoc->cn_ctlblk.cn_rcvr_cond,
                                   rpc_g_global_mutex);
                }
                CATCH(pthread_cancel_e)
                {
                    RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
				    ("CN: assoc->%x rcvr free'ed by acb_free\n",
				     assoc));
                    done = true;
                }
		CATCH_ALL
		{
		    /*
		     * rpc_m_unexpected_exc
		     * "(%s) Unexpected exception was raised"
		     */
		    RPC_DCE_SVC_PRINTF ((
			DCE_SVC(RPC__SVC_HANDLE, "%s"),
		        rpc_svc_recv,
		        svc_c_sev_fatal | svc_c_action_abort,
			rpc_m_unexpected_exc,
			"rpc__cn_network_receiver: cond_wait" ));
                }
                ENDTRY

		assoc->cn_ctlblk.cn_rcvr_waiters--;

                if (done == true)
		    break;
                RPC_DBG_PRINTF (rpc_e_dbg_threads, RPC_C_CN_DBG_THREADS,
                    ("####### assoc->%x Got a new connection \n", assoc));
            }
            
            if (done)
	    {
		RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
		("CN: assoc->%x call_rep->none Receiver awake ... free'ed\n",
		                assoc));
	    }
	    else
	    {
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                            ("CN: assoc->%x call_rep->none Receiver awake ... Connection established\n",
                             assoc));

            /*
             * Increment theassociation control block's reference count since we
             * are now using it and receive packets as long as the
             * connection is open. 
             */
            RPC_CN_ASSOC_ACB_INC_REF (assoc);
            
            /*
             * A connection has been established.
             */
            RPC_CN_STATS_INCR (connections);
            TRY
            {
                receive_dispatch (assoc);
            }
            CATCH(pthread_cancel_e)
            {
                RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
("CN: call_rep->%x assoc->%x desc->%x receiver canceled, caught in rpc__cn_network_receiver()\n",
                                assoc->call_rep,
                                assoc,
                                assoc->cn_ctlblk.cn_sock));
            }
            CATCH_ALL
            {
		/*
		 * rpc_m_unexpected_exc
		 * "(%s) Unexpected exception was raised"
		 */
		RPC_DCE_SVC_PRINTF ((
		    DCE_SVC(RPC__SVC_HANDLE, "%s"),
		    rpc_svc_recv,
		    svc_c_sev_fatal | svc_c_action_abort,
		    rpc_m_unexpected_exc,
		    "rpc__cn_network_receiver" ));
            }
            ENDTRY
                
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                                ("CN: assoc->%x call_rep->none No longer receiving...Close socket\n",
                                 assoc));
                /*
                 * Either the connection was broken or another
                 * thread has sent us a cancel indicating the
                 * connection should be broken. In either case
                 * close the socket and set the connection state
                 * to closed.
                 */
                RPC_CN_STATS_INCR (closed_connections);
                serr = rpc__socket_close (assoc->cn_ctlblk.cn_sock);
                if (RPC_SOCKET_IS_ERR(serr))
                {
                    /*
                     * The socket close failed.
                     */
                    RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
("(rpc__cn_network_receiver) assoc->%x desc->%x rpc__socket_close failed, error = %d\n", 
                                     assoc,
                                     assoc->cn_ctlblk.cn_sock,                                  
                                     RPC_SOCKET_ETOI(serr)));
                }
                
                assoc->cn_ctlblk.cn_state = RPC_C_CN_CLOSED;
                
                /*
                 * Remove any pending cancel on this assoc. Otherwise, it's
                 * possible that the receiver thread will see this cancel after
                 * the next call begins.
                 */
                TRY
                {
                    pthread_testcancel();
                }
                CATCH_ALL
                {
                    RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                        ("CN: assoc->%x rcvr cancel found at acb_dealloc\n",
                        assoc));
                }
                ENDTRY

                /*
                 * Deallocate the association control block.
                 */
                rpc__cn_assoc_acb_dealloc (assoc);

                /*
                 * Check if rpc__cn_assoc_acb_free() posted the cancel.
                 */
                TRY
                {
                    pthread_testcancel();
                }
                CATCH(pthread_cancel_e)
                {    
                    RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                        ("CN: assoc->%x rcvr free'ed by acb_dealloc\n",
                        assoc));
                    done = true;
                }
                CATCH_ALL
                {
		    /*
		     * rpc_m_unexpected_exc
		     * "(%s) Unexpected exception was raised"
		     */
		    RPC_DCE_SVC_PRINTF ((
		        DCE_SVC(RPC__SVC_HANDLE, "%s"),
		        rpc_svc_recv,
		        svc_c_sev_fatal | svc_c_action_abort,
		        rpc_m_unexpected_exc,
		        "rpc__cn_network_receiver" ));
                }
                ENDTRY
	    }
        }
        CATCH_ALL
        {
        }
        ENDTRY
        
        /*
         * Unlock the global connection mutex.
         */
        RPC_CN_UNLOCK ();
    } /* end while (!assoc->cn_ctlblk.exit_rcvr) */

    RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                    ("CN: assoc->%x call_rep->none Receiver thread exiting...\n",
                     assoc));
}


/******************************************************************************/
/*
**++
**
**  ROUTINE NAME:       receive_dispatch
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**
**  This is the low-level routine for receiving and dispatching packets.
**
**  This routine is called once per "connection" and will continue to
**  receive and dispatch packets until some kind of error is encountered.
**
**  INPUTS:
**
**      assoc           pointer to an association control block
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
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

INTERNAL void receive_dispatch 
#ifdef _DCE_PROTO_
(
  rpc_cn_assoc_p_t        assoc
)
#else
(assoc)
rpc_cn_assoc_p_t        assoc;
#endif
{
    rpc_cn_fragbuf_p_t          fragbuf_p;
    rpc_cn_fragbuf_p_t          ovf_fragbuf_p;
    rpc_cn_call_rep_p_t         call_r;
    unsigned32                  st;
    rpc_cn_packet_p_t           pktp;
    unsigned8                   ptype;
    boolean                     unpack_ints;
    unsigned32                  i;
    rpc_cn_syntax_t             *pres_context;
    unsigned32                  auth_st;
    rpc_cn_sec_context_t        *sec_context;
    boolean                     already_unpacked;

    /*
     * Onetime (auto) initialization.
     */
    st = rpc_s_ok;
    fragbuf_p = NULL;
    ovf_fragbuf_p = NULL;
    call_r = NULL;

    /*
     * Main receive processing.
     *
     * We loop, receiving and processing packets until some kind of error
     * is encountered.
     */
    for (i = 0;; i++)
    {
        RPC_LOG_CN_PROCESS_PKT_NTR;

        /*
         * Increment the per-association security context next receive
         * sequence number.
         */
        assoc->security.assoc_next_rcv_seq = i;

        /*
         * Receive a packet from the network.
         */
        TRY
        {
            receive_packet (assoc, &fragbuf_p, &ovf_fragbuf_p, &st);
        }
        CATCH(pthread_cancel_e)
        {
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
("CN: call_rep->%x assoc->%x desc->%x receiver canceled, caught in receive_dispatch()\n",
                            assoc->call_rep, 
                            assoc,
                            assoc->cn_ctlblk.cn_sock));
           st = rpc_s_connection_closed;
        }
        CATCH_ALL
        {
	    /*
	     * rpc_m_unexpected_exc
	     * "(%s) Unexpected exception was raised"
	     */
	    RPC_DCE_SVC_PRINTF ((
	        DCE_SVC(RPC__SVC_HANDLE, "%s"),
	        rpc_svc_recv,
	        svc_c_sev_fatal | svc_c_action_abort,
	        rpc_m_unexpected_exc,
	        "receive_dispatch" ));
        }
        ENDTRY

        if (st != rpc_s_ok)
        {
            break;
        }

        already_unpacked = false;

        /*
         * Point to the packet header.
         */
        pktp = (rpc_cn_packet_p_t) fragbuf_p->data_p;

        /*
         * Trace the incoming packet.
         */
        RPC_CN_PKT_TRC (pktp);
        RPC_CN_PKT_DUMP (pktp, fragbuf_p->data_size);

        /*
         * Keep some stats on the packets received.
         */
        RPC_CN_STATS_INCR (pstats[RPC_CN_PKT_PTYPE (pktp)].rcvd);
        RPC_CN_STATS_INCR (pkts_rcvd);

        /*
         * Setup some local variables.
         */
        ptype = RPC_CN_PKT_PTYPE (pktp);

	/*
	 * Make sure that we have a valid packet type.
	 * If not, we return an error, and the caller will close
	 * the connection.
	 */        
        if (/* (ptype < 0) ||*/ (ptype > RPC_C_CN_PKT_MAX_TYPE) ||
            (packet_info_table[ptype].class == DGRAM_CLASS_PKT))
        {
            st = rpc_s_protocol_error;
            break;
        }

        /*
         * Do some first packet only processing...
         */
        if (i == 0)
        {
            /*
             * Stash the remote NDR format away. Also create boolean
             * to determine whether we have to bother unpacking the
             * packet header.
             */
            NDR_UNPACK_DREP (&(RPC_CN_ASSOC_NDR_FORMAT (assoc)),
                             RPC_CN_PKT_DREP (pktp));
            if ((NDR_DREP_INT_REP (RPC_CN_PKT_DREP (pktp)) != 
                 NDR_LOCAL_INT_REP))
            {
                unpack_ints = true;
            }
            else
            {
                unpack_ints = false;
            }
        }
        else
        {
            /*
             * Sanity check the major and minor version numbers.
             * We let the association state machine do this check
             * in the case BIND packets because the
             * protocol calls for a call reject if the versions 
             * do not match at that point.
             * For subsequent packets, we do the check here.
             */
            if (!(ptype == RPC_C_CN_PKT_BIND) &&
                ((RPC_CN_PKT_VERS (pktp) != RPC_C_CN_PROTO_VERS) ||
                (RPC_CN_PKT_VERS_MINOR (pktp) > RPC_C_CN_PROTO_VERS_MINOR)))
            {
                st = rpc_s_rpc_prot_version_mismatch;
                break;
            }
        }

        auth_st = rpc_s_ok;
        sec_context = NULL;

        /*
         * Determine whether the received PDU contains an
         * authentication trailer.  We don't care about byte
         * ordering in the following macro invocation because
         * the trailer length is compared with zero to determine
         * whether or not the trailer is present.
         */
        if (RPC_CN_PKT_AUTH_TLR_PRESENT (pktp))
        {
            rpc_cn_auth_tlr_t               *auth_tlr;
            unsigned16                      auth_len;
            unsigned32                      key_id;
            unsigned16                      frag_len;

            /*
             * If the pdu is a bind or alter-context pdu and we need to
             * unpack the packet, save the raw form of the pdu so that
             * the checksum can be computed correctly later.  Do this by 
             * allocating a fragbuf and chaining it to the association
             * control block, then copying the packet to it.  The fragbuf
             * will be deallocated in the state machine after invoking
             * recv_check.
             */
            if ((unpack_ints) &&
                ((ptype == RPC_C_CN_PKT_BIND) ||
                 (ptype == RPC_C_CN_PKT_BIND_ACK) ||
                 (ptype == RPC_C_CN_PKT_ALTER_CONTEXT) ||
                 (ptype == RPC_C_CN_PKT_ALTER_CONTEXT_RESP) ||
                 (ptype == RPC_C_CN_PKT_BIND_NAK) ||
                 (ptype == RPC_C_CN_PKT_AUTH3)))
            {
                assoc->raw_packet_p = rpc__cn_fragbuf_alloc (true);
		if (assoc->raw_packet_p == NULL){
		    st = rpc_s_no_memory;
		    break;
		}
                assoc->raw_packet_p->data_size = fragbuf_p->data_size;
                memcpy (assoc->raw_packet_p->data_p,
                        fragbuf_p->data_p,
                        fragbuf_p->data_size);
            }

            /*
             * Locate the authentication trailer in the PDU.
             */
            auth_len = RPC_CN_PKT_AUTH_LEN (pktp);
            if (unpack_ints)
            {
                SWAB_INPLACE_16 (auth_len);
            }
            
	    auth_tlr = (rpc_cn_auth_tlr_t *) ((unsigned8 *)(pktp) +
                fragbuf_p->data_size - 
                (auth_len + RPC_CN_PKT_SIZEOF_COM_AUTH_TLR));

            /*
             * Find the appropriate security context element using the key ID
             * contained in the auth_value part of the trailer.  Also obtain
             * the size of the credentials in the proper format.
             */
            key_id = auth_tlr->key_id;
            if (unpack_ints)
            {
                SWAB_INPLACE_32 (key_id);
            }

            if ((ptype != RPC_C_CN_PKT_BIND) && 
                (ptype != RPC_C_CN_PKT_ALTER_CONTEXT) &&
                (ptype != RPC_C_CN_PKT_BIND_ACK) && 
                (ptype != RPC_C_CN_PKT_ALTER_CONTEXT_RESP))
            {
                rpc__cn_assoc_sec_lkup_by_id (assoc,
                                              key_id,
                                              &sec_context,
                                              &auth_st);
                
                /*
                 * If a security context was located apply the
                 * per-packet security check. Any errors found in either
                 * locating the security context or during the check will
                 * be handled below according to the type of PDU received.
                 */
                if (auth_st == rpc_s_ok)
                {
                    /*
                     * Note that cred_len is zero for all per-message
                     * packets.
                     */
                    RPC_CN_AUTH_RECV_CHECK (RPC_CN_AUTH_CVT_ID_WIRE_TO_API (auth_tlr->auth_type, &st),
                                            &assoc->security,
                                            sec_context,
                                            (rpc_cn_common_hdr_t *)pktp,
                                            fragbuf_p->data_size,
                                            0, /* cred_len */
                                            auth_tlr,
                                            unpack_ints,
                                            &auth_st);
                    if (auth_st == rpc_s_ok)
                    {
                        /*
                         * Unpack the header part of the packet.
                         * This will make it easier to remove from the frag
                         * length any padding that was required to
                         * get the auth trailer 4-byte aligned at
                         * the sender.
                         *
                         * Since recv_check may have moved the auth_tlr,
                         * we get the pointer again.
                         */
                        if (unpack_ints)
                        {
                            rpc__cn_unpack_hdr (pktp);
                            already_unpacked = true;
                        }
                        auth_len = RPC_CN_PKT_AUTH_LEN (pktp);
                        auth_tlr = (rpc_cn_auth_tlr_t *) ((unsigned8 *)(pktp) +
                                                          fragbuf_p->data_size - 
                                                          (auth_len
                                                           + 
                                                           RPC_CN_PKT_SIZEOF_COM_AUTH_TLR));
                        frag_len = RPC_CN_PKT_FRAG_LEN (pktp);
                        frag_len -= auth_tlr->stub_pad_length;
                        RPC_CN_PKT_FRAG_LEN (pktp) = frag_len;
                        fragbuf_p->data_size -= auth_tlr->stub_pad_length;
                    }
                    else
                    {
                        /*
                         * Handle any error which occured while performing
                         * either the recv check or key ID lookup. Errors which
                         * occur on the server for a call class PDU will be
                         * handled later by sending a fault PDU back.
                         *
                         * On the client side this error should just be
                         * handed back to the client thread waiting, if
                         * any.
                         * 
                         * On the server side the error should be reflected
                         * back to the client on either a FAULT
                         * PDU if the recv_check failed on a call class
                         * PDU. If it failed on an assoc class PDU then the
                         * best we can probably do is close the association
                         * (it may be possible to respond with a BIND_NAK
                         * if the recv_check failed on a BIND).
                         */
                        if (assoc->assoc_flags & RPC_C_CN_ASSOC_CLIENT)
                        {
                            (*fragbuf_p->fragbuf_dealloc)(fragbuf_p);
                            sec_context->sec_status = auth_st;
                            RPC_CN_ASSOC_WAKEUP (assoc);
                            continue;
                        }
                        else
                        {
                            dce_error_string_t error_text;
                            int temp_status;

                            dce_error_inq_text(auth_st, error_text, &temp_status);
                            /*
			     * rpc_m_call_failed_s
			     * "%s on server failed: %s"
			     */
			    RPC_DCE_SVC_PRINTF ((
				DCE_SVC(RPC__SVC_HANDLE, "%s%x"),
				rpc_svc_recv,
				svc_c_sev_error,
				rpc_m_call_failed_s,
				"RPC_CN_AUTH_RECV_CHECK",
				error_text ));

                            if (packet_info_table[ptype].class == ASSOC_CLASS_PKT)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            if ((assoc->assoc_flags & RPC_C_CN_ASSOC_CLIENT) &&
               (ptype == RPC_C_CN_PKT_RESPONSE) &&
               (RPC_CN_AUTH_REQUIRED(assoc->call_rep->binding_rep->auth_info)))
            {
                RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                        ("CN: auth_info %x\n", assoc->call_rep->binding_rep->auth_info));
                RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                        ("CN: should not continue further with this PDU\n"));
                (*fragbuf_p->fragbuf_dealloc)(fragbuf_p);
                st = rpc_s_authn_level_mismatch;
                RPC_CN_ASSOC_WAKEUP (assoc);
                break;
            }
        }

        /*
         * Unpack the packet header, if necessary, and check to see
         * if the packet type is within the legal range of values.
         */
        if (unpack_ints && !already_unpacked)
        {
            rpc__cn_unpack_hdr (pktp);
        }

        /*
         * Finally, post the event to the appropriate state machine
         */
        if (packet_info_table[ptype].class == CALL_CLASS_PKT)
        {
            if ((ptype == RPC_C_CN_PKT_REQUEST) 
                && 
                (RPC_CN_PKT_FLAGS (pktp) & RPC_C_CN_FLAGS_FIRST_FRAG))
            {
                /*
                 * This is the first fragment of a call request packet.
                 * Allocate a call rep and mark it as being a server
                 * call rep.
                 */
                call_r = (rpc_cn_call_rep_t *) 
                    rpc__list_element_alloc (&rpc_g_cn_call_lookaside_list, 
                                             true); 
		if (call_r == NULL){
		    st = rpc_s_no_memory;
		    break;
		}		    
                call_r->common.is_server = true;
                
                /*
                 * Place the new call rep in the association for
                 * use in cancel processing.
                 */
                rpc__cn_assoc_push_call (assoc, call_r); 
                
                /*
                 * Initialize the server call state machine.
                 */
                rpc__cn_sm_init (rpc_g_cn_server_call_sm,
                                 rpc_g_cn_server_call_action_tbl,
                                 &(call_r->call_state),
				 rpc_c_cn_svr_call);

		call_r->num_pkts = 0;
                call_r->sec = sec_context;
                call_r->cn_call_status = rpc_s_ok;
                call_r->last_frag_received = false;
                call_r->call_executed = false;
                call_r->common.u.server.cthread.is_queued = false;
                call_r->prot_tlr = NULL;

                {
                int i;
                for( i=1; i<RPC_C_MAX_IOVEC_LEN; i++ ) {
                    call_r->buffered_output.iov.elt[i].buff_addr = NULL;
                    call_r->buffered_output.iov.elt[i].buff_dealloc = NULL;
                }
                }

                /*
                 * Initialize some cancel state information. 
                 */
                call_r->common.u.server.cancel.accepting = true;
                call_r->common.u.server.cancel.queuing = true;
                call_r->common.u.server.cancel.had_pending = false;
                call_r->common.u.server.cancel.count = 0;
                call_r->u.server.cancel.local_count = 0;

                /*
                 * Allocate a binding rep and put either a nil UUID
                 * or the object UUID in the request packet header in
                 * it, if present.
                 */
                if (RPC_CN_PKT_FLAGS (pktp) & RPC_C_CN_FLAGS_OBJECT_UUID) 
                { 
                    call_r->binding_rep = 
                    rpc__binding_alloc (true, 
                                        &RPC_CN_PKT_OBJECT (pktp), 
                                        RPC_C_PROTOCOL_ID_NCACN, 
                                        NULL, 
                                        &st); 
                } 
                else 
                { 
                    call_r->binding_rep = 
                    rpc__binding_alloc (true, 
                                        &uuid_g_nil_uuid, 
                                        RPC_C_PROTOCOL_ID_NCACN, 
                                        NULL, 
                                        &st); 
                } 

                /*
                 * If the binding_alloc failed, simply break out of
                 * the loop to close the connection.
                 */
                if (st != rpc_s_ok)
                {
                    break;
                }

                /*
                 * Put the association group id in the binding rep.
                 */
                ((rpc_cn_binding_rep_t *)call_r->binding_rep)->grp_id 
                    = assoc->assoc_grp_id; 
                call_r->assoc = assoc; 

                /*
                 * Attach the auth info, if any, to the new binding
                 * rep. Make sure to add a reference to it.
                 */
                if (sec_context)
                {
                    call_r->binding_rep->auth_info = sec_context->sec_info;
                    RPC_CN_AUTH_ADD_REFERENCE (sec_context->sec_info);
                }


                /*
                 * Post the event to the call state machine.
                 */
                RPC_CN_POST_FIRST_CALL_SM_EVENT (call_r,
                                                 assoc, 
                                                 packet_info_table[ptype].event, 
                                                 fragbuf_p, 
                                                 st);

                /*
                 * Now that we have a call rep set up and are in the
                 * call_request state see whether the security PDU
                 * receive check or key ID lookup performed earlier in this routine,
                 * if required, passed. If it failed a fault PDU
                 * will be sent back to the client. 
                 */
                if (st != rpc_s_ok)
                {
                    RPC_CN_SEND_FAULT (call_r, st);
                    fragbuf_p = NULL;
                    continue;
                }
                if (auth_st != rpc_s_ok)
                {
                    RPC_CN_SEND_FAULT (call_r, auth_st);
                    fragbuf_p = NULL;
                    continue;
                }

                /*
                 * Get the i/f id and version using the presentation
                 * context id in the header.
                 */
                rpc__cn_assoc_syntax_lkup_by_id (assoc,
                                                 RPC_CN_PKT_PRES_CONT_ID (pktp),
                                                 &pres_context,
                                                 &st);

                call_r->u.server.if_id = &pres_context->syntax_abstract_id.id;
                call_r->u.server.if_vers = pres_context->syntax_abstract_id.version;
                call_r->transfer_syntax.index = pres_context->syntax_vector_index;
                call_r->transfer_syntax.convert_epv = pres_context->syntax_epv;
                call_r->u.server.ihint = pres_context->syntax_ihint;
                call_r->context_id = pres_context->syntax_pres_id;

                if (st != rpc_s_ok)
                {
                    RPC_CN_SEND_FAULT (call_r, st);
                    fragbuf_p = NULL;
                    continue;
                }
                
                /*
                 * Invoke a call thread.
                 */
                rpc__cthread_invoke_null ((rpc_call_rep_p_t) call_r,
                                          &call_r->binding_rep->obj,
                                          call_r->u.server.if_id,
                                          call_r->u.server.if_vers,
                                          (unsigned32) call_r->opnum,
                                          rpc__cn_call_executor,
                                          (pointer_t) call_r,
                                          &st);
                
                /*
                 * Check whether the call was queued. If it was, just
                 * set the status code to "ok".
                 */
                if (st == rpc_s_call_queued)
                {
                    RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                                    ("CN: call_rep->%x assoc->%x desc->%x call queued\n",
                                     call_r,
                                     assoc,
                                     assoc->cn_ctlblk.cn_sock));
                    st = rpc_s_ok;
                }
                else
                {
                    /*
                     * If we get an error back, then we were unable to
                     * create the call thread.  Send a fault PDU.
                     */
                    if (st != rpc_s_ok)
                    {
                        RPC_CN_SEND_FAULT (call_r, st);
                        fragbuf_p = NULL;
                        continue;
                    }
                }

                /*
                 * Keep some stats on the number of calls received.
                 */
                RPC_CN_STATS_INCR (calls_rcvd);
            }
            else
            {
                RPC_CN_POST_CALL_SM_EVENT (assoc, 
                                           packet_info_table[ptype].event, 
                                           fragbuf_p,
                                           st);
            }
        }
        else 
        {
            RPC_CN_ASSOC_EVAL_NETWORK_EVENT (assoc, 
                                             packet_info_table[ptype].event, 
                                             fragbuf_p,
                                             st);
        }

        /*
         * Unlock the CN global mutex, yield the processor to allow
         * another thread to run and re-aquire the CN global mutex.
         */
        RPC_CN_UNLOCK ();
        pthread_yield ();
        RPC_CN_LOCK ();
        
        /*
         * NULL our pointer to the fragment buffer so we'll be forced to get
         * a new one.
         */
        fragbuf_p = NULL;
        RPC_LOG_CN_PROCESS_PKT_XIT;
    } /* end-for (i = 0;; i++) */

    /*
     * Some failure occured while receiving packets. Indicate this
     * failure to the association state machine.
     */
    rpc__cn_assoc_post_error (assoc, st);

    /*
     * If we still have a fragbufs, then deallocate them.
     */
    if (fragbuf_p)
    {
        (*fragbuf_p->fragbuf_dealloc)(fragbuf_p);
    }
    if (ovf_fragbuf_p)
    {
        (*ovf_fragbuf_p->fragbuf_dealloc)(ovf_fragbuf_p);
    }
}


/******************************************************************************/
/*
**++
**
**  ROUTINE NAME:       receive_packet
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**
**  This is a (large) wrapper around the socket recvmsg() routine.
**
**  We do this since stream sockets don't guarantee the preservation of RPC
**  packet boundaries.  If we receive partial packets, we have to piece them
**  back together again.  Also, if we receive more than one packet during a
**  read operation, we preserve the excess bytes read and return them the next
**  time we are called.
**
**  INPUTS:
**
**      assoc           pointer to an association control block
**
**  INPUTS/OUTPUTS:
**
**      fragbuf_p       pointer to a fragbuf pointer
**      ovf_fragbuf_p   pointer to a overflow fragbuf pointer
**
**  OUTPUTS:            
**
**      st              the return status of this routine
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

INTERNAL void receive_packet 
#ifdef _DCE_PROTO_
(
  rpc_cn_assoc_p_t        assoc,
  rpc_cn_fragbuf_p_t      *fragbuf_p,
  rpc_cn_fragbuf_p_t      *ovf_fragbuf_p,
  unsigned32              *st
)
#else
(assoc, fragbuf_p, ovf_fragbuf_p, st)
rpc_cn_assoc_p_t        assoc;
rpc_cn_fragbuf_p_t      *fragbuf_p;
rpc_cn_fragbuf_p_t      *ovf_fragbuf_p;
unsigned32              *st;
#endif
{
    rpc_cn_fragbuf_t    *fbp;
    unsigned16          frag_length;
    int                 bytes_rcvd;
    rpc_socket_iovec_t  iov;
    rpc_socket_error_t  serr;
    signed32            need_bytes;
    static rpc_addr_p_t addr = NULL;

    RPC_LOG_CN_RCV_PKT_NTR;
    CODING_ERROR (st);
    
    /*
     * One time (auto) initialization.
     */
    fbp = NULL;

    /*
     * Assume the worst is going to happen and zap our return value.
     */
    *fragbuf_p = NULL;

    /*
     * If we have a left over fragment buffer (overflow), then use it.
     */
    if (*ovf_fragbuf_p != NULL)
    {
        fbp = *ovf_fragbuf_p;
        *ovf_fragbuf_p = NULL;
    }

    /*
     * If we need a fragment buffer, then allocate one.
     */
    if (fbp == NULL)
    {
        fbp = rpc__cn_fragbuf_alloc (true);
    }
    if (fbp == NULL){
	*st = rpc_s_no_memory;
	return;
    }

    /*
     ******************************************************************
     * At this point we must determine which of the following 3
     * situations we are faced with:
     *
     *  1) The fragbuf is empty.
     *  2) The fragbuf contains a partial RPC packet.
     *  3) The fragbuf contains a complete RPC packet.
     *
     * The first thing we do is to check whether or not we have
     * enough data in the current fragbuf to determine the complete
     * RPC packet length.  Note that in the 3rd case, the fragbuf may
     * actually contain more than one RPC packet.
     *
     ******************************************************************
     */

    /*
     * Do we have enough data in the fragbuf to determine the RPC packet
     * length?
     *
     * If we do, we can figure out exactly how many bytes to request on our
     * next read. If we don't, we just ask for the maximum number of bytes
     * that our fragbuf will hold.
     */
    if (fbp->data_size >= RPC_C_CN_FRAGLEN_HEADER_BYTES)
    {
        /*
         * Okay, we have enough of the header to figure out how big
         * this fragment is.
         */
        frag_length = RPC_CN_PKT_FRAG_LEN ((rpc_cn_packet_p_t)(fbp->data_p));

        /*
         * Swap bytes if our integer formats are different.
         */
        if (NDR_DREP_INT_REP(RPC_CN_PKT_DREP((rpc_cn_packet_p_t)fbp->data_p)) 
            != NDR_LOCAL_INT_REP)
        {
            SWAB_INPLACE_16 (frag_length);
        }

        /*
         * Figure out how many bytes we need.
         */
        need_bytes = frag_length - fbp->data_size;
    }
    else
    {
        /*
         * We don't have enough data to compute the fragment length.
         */
        frag_length = 0;

        /*
         * Ask for as many bytes as our fragbuf will hold.
         */
        need_bytes = fbp->max_data_size - fbp->data_size;
    }

    /*
     * Read from the socket until we've read at least one entire packet.
     */
    while (need_bytes > 0)
    {

        /*
         * Initialize our iovector.
         */
        iov.base = (byte_p_t)((unsigned8 *)(fbp->data_p) + fbp->data_size);
        iov.len = need_bytes;

        /*
         * Release the CN global mutex before reading from the
         * socket. This will allow other threads to run if we have to
         * block here.
         */
        RPC_CN_UNLOCK ();
        TRY
        {
#ifdef NON_CANCELLABLE_IO
       /*
        * By posix definition pthread_setasynccancel is not a "cancel
        * point" because it must return an error status and an errno.
        * pthread_setasynccancel(CANCEL_ON) will not deliver
        * a pending cancel nor will the cancel be delivered asynchronously,
        * thus the need for pthread_testcancel.
	*/
	    pthread_setasynccancel(CANCEL_ON);
	    pthread_testcancel();

#endif
            RPC_SOCKET_RECVMSG (assoc->cn_ctlblk.cn_sock,
                                &iov,
                                1,
                                addr,
                                &bytes_rcvd,
                                &serr);

#ifdef NON_CANCELLABLE_IO
	    pthread_setasynccancel(CANCEL_OFF);
#endif            
        }
        CATCH (pthread_cancel_e)
        {
#ifdef NON_CANCELLABLE_IO
	    pthread_setasynccancel(CANCEL_OFF);
#endif            
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
("CN: call_rep->%x assoc->%x desc->%x receiver canceled, caught in receive_packet()\n",
                             assoc->call_rep, 
                             assoc,
                             assoc->cn_ctlblk.cn_sock));

            /*
             * Re-acquire the CN global mutex and free any fragment
             * buffers we have outstanding.
             */
            RPC_CN_LOCK ();
            if (fbp)
            {
                (*(fbp)->fragbuf_dealloc)(fbp);
            }
            RERAISE;
        }
        CATCH_ALL
        {
	    /*
	     * rpc_m_unexpected_exc
	     * "(%s) Unexpected exception was raised"
	     */
	    RPC_DCE_SVC_PRINTF ((
	        DCE_SVC(RPC__SVC_HANDLE, "%s"),
	        rpc_svc_recv,
	        svc_c_sev_fatal | svc_c_action_abort,
	        rpc_m_unexpected_exc,
	        "receive_packet" ));
        }
        ENDTRY

        /*
         * Re-acquire the CN global mutex.
         */
        RPC_CN_LOCK ();

        /*
         * Hold off on processing the packet if the sending thread for this
         * connection is currently in a sendmsg.
         */
        while (assoc->cn_ctlblk.in_sendmsg)
        {
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                            ("CN: call_rep->%x assoc->%x desc->%x waiting for sendmsg to complete...\n", 
                             assoc->call_rep, 
                             assoc,
                             assoc->cn_ctlblk.cn_sock,
                             bytes_rcvd));
            assoc->cn_ctlblk.waiting_for_sendmsg_complete = true;
            RPC_COND_WAIT (assoc->cn_ctlblk.cn_rcvr_cond,
                           rpc_g_global_mutex);
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                            ("CN: call_rep->%x assoc->%x desc->%x sendmsg complete\n", 
                             assoc->call_rep, 
                             assoc,
                             assoc->cn_ctlblk.cn_sock,
                             bytes_rcvd));
            assoc->cn_ctlblk.waiting_for_sendmsg_complete = false;
        }
        
        /*
         * Process any errors reading the socket or any errors detected by 
         * other threads using this association. These other threads will have
         * cleaned up the assoc state and the receiver thread just has to close
         * the connection now.
         */
        if ((RPC_SOCKET_IS_ERR (serr))
            ||
            (bytes_rcvd == 0)
            ||
            (assoc->assoc_local_status != rpc_s_ok)
            ||
            (assoc->assoc_status != rpc_s_ok))
        {
            /*
             * Make sure the connection is really closed. It could
             * be a zero length sequenced packet socket packet.
             */
            if (rpc__naf_is_connect_closed (assoc->cn_ctlblk.cn_sock, st))
            {
                RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                                ("CN: call_rep->%x assoc->%x desc->%x connection closed recvmsg failed serr = %x, bytes_rcvd = %d\n",
                                 assoc->call_rep,
                                 assoc,
                                 assoc->cn_ctlblk.cn_sock, 
                                 serr,
                                 bytes_rcvd));
                
                /*
                 * Deallocate the fragbuf which we were using.
                 */
                (*fbp->fragbuf_dealloc)(fbp);
                
                *st = rpc_s_connection_closed;
                return;
            }
        }

        RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                        ("CN: call_rep->%x assoc->%x desc->%x received %d bytes\n", 
                         assoc->call_rep, 
                         assoc,
                         assoc->cn_ctlblk.cn_sock,
                         bytes_rcvd));

        /*
         * Update the number of bytes received.
         */
        fbp->data_size += bytes_rcvd;

        /*
         * If we don't already know the fragment length, then we now have to go
         * through the same gymnastics that we did before vis a vis determining
         * whether or not we've read enough of this fragment to determine the
         * complete fragment length.
         */
        if ((frag_length == 0) && (fbp->data_size >= RPC_C_CN_FRAGLEN_HEADER_BYTES))
        {
            /*
             * We don't know the fragment length and we have enough bytes to
             * determine it so compute the fragment length.
             */
            frag_length = RPC_CN_PKT_FRAG_LEN ((rpc_cn_packet_p_t)(fbp->data_p));

            /*
             * Swap bytes if our integer formats are different.
             */
            if (NDR_DREP_INT_REP( RPC_CN_PKT_DREP((rpc_cn_packet_p_t)fbp->data_p) ) != NDR_LOCAL_INT_REP)
            {
                SWAB_INPLACE_16 (frag_length);
            }

#ifdef DEBUG
            /*
             * Sanity check the protocol versions in the header.
             * Except for BIND and BIND_NAK packets.
             */
            {
                unsigned32 ptype;
                ptype = RPC_CN_PKT_PTYPE((rpc_cn_packet_p_t)fbp->data_p);

                if ((ptype != RPC_C_CN_PKT_BIND) &&
                    (ptype != RPC_C_CN_PKT_BIND_NAK) &&
                    ((RPC_CN_PKT_VERS ((rpc_cn_packet_p_t)fbp->data_p) != RPC_C_CN_PROTO_VERS) ||
                     (RPC_CN_PKT_VERS_MINOR ((rpc_cn_packet_p_t)fbp->data_p) != assoc->assoc_vers_minor)))
                {
                    /*
                     * We have incompatible protocol versions.
		     */
		    /*
		     * "(receive_packet) assoc->%x %s: Protocol version mismatch -
		     *            major->%x minor->%x"
                     */
		    RPC_DCE_SVC_PRINTF ((
			DCE_SVC(RPC__SVC_HANDLE, "%x%s%x%x"),
			rpc_svc_cn_pkt,
			svc_c_sev_warning,
			rpc_m_prot_mismatch,
			assoc,
			rpc__cn_pkt_name(ptype),
			RPC_CN_PKT_VERS ((rpc_cn_packet_p_t)fbp->data_p),
			RPC_CN_PKT_VERS_MINOR ((rpc_cn_packet_p_t)fbp->data_p) ));
                }
            }
            
            /*
             * Sanity check the fragment size.
             * Signal an error if this fragment is bigger than a fragbuf.
             */
            if (frag_length > rpc_g_cn_large_frag_size)
            {
                /*
		 * "(receive_packet) assoc->%x frag_length %d in header >
		 *                fragbuf data size %d"
		 */
                RPC_DCE_SVC_PRINTF ((
		    DCE_SVC(RPC__SVC_HANDLE, "%x%d%d"),
		    rpc_svc_cn_pkt,
		    svc_c_sev_warning,
		    rpc_m_frag_toobig,
                    assoc,
                    frag_length, 
                    rpc_g_cn_large_frag_size ));
                *st = rpc_s_protocol_error;

	        /*
	         * Deallocate the fragbuf which we were using.
	         */
	        (*fbp->fragbuf_dealloc)(fbp);
                return;
            }
#endif
        }

        /*
         * Recalculate how many bytes we need to complete this fragment.
         */
        if (frag_length == 0)
        {
            need_bytes = fbp->max_data_size - fbp->data_size;
        }
        else
        {
            need_bytes = frag_length - fbp->data_size;
        }
    } /* end of while(need_bytes > 0) */

    /*
     * Handle the situation where we have read too much data.
     * This means that we have read into the next packet.
     * So, get an "overflow" fragment buffer and copy the excess data into it.
     */

    /*
     * There is room for some optimization here.  If we have enough bytes to
     * determine the length of the next packet and the whole thing will fit into
     * a small fragbuf, then we should probably use a small one.  For now, we're
     * going to apply the KISS principle.
     */
    if (need_bytes < 0)
    {
        /*
         * Get an overflow fragment buffer.
         */
        *ovf_fragbuf_p = rpc__cn_fragbuf_alloc (true);
	if (*ovf_fragbuf_p == NULL){
	    *st = rpc_s_no_memory;
	    (*fbp->fragbuf_dealloc)(fbp);
	    return;
	}
        (*ovf_fragbuf_p)->data_size = abs(need_bytes);

        /*
         * Set the fragbuf data size to the fragment length and copy the
         * excess data to the overflow fragment buffer.
         */
        fbp->data_size = frag_length;
        memcpy ((*ovf_fragbuf_p)->data_p,
                (pointer_t)((unsigned8 *)(fbp->data_p) + fbp->data_size),
                (*ovf_fragbuf_p)->data_size);
    }

    /*
     * Return a pointer to the just-received packet along with okay status.
     */
    *fragbuf_p = fbp;
    *st = rpc_s_ok;
    RPC_LOG_CN_RCV_PKT_XIT;
}

