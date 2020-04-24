/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnnet.c,v $
 * Revision 1.1.98.2  1996/02/18  00:01:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:38  marty]
 *
 * Revision 1.1.98.1  1995/12/08  00:17:22  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:29 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/9  1995/06/30  21:06 UTC  tatsu_s
 * 	Submitted the fix for OT12904/CHFts15508.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.fix_ot12885.b1/1  1995/06/20  20:39 UTC  tatsu_s
 * 	Set keepalive on a new connection.
 * 	Don't set pkt_nodelay for local rpc.
 * 
 * 	HP revision /main/HPDCE02/8  1995/05/05  20:50 UTC  tatsu_s
 * 	Submitted the fix for CHFts14944.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.scale_fix.b0/1  1995/04/21  20:37 UTC  tatsu_s
 * 	Fixed max_call_requests to work in rpc_server_use_*protseq*().
 * 
 * 	HP revision /main/HPDCE02/7  1995/04/02  01:13 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/HPDCE02/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:17 UTC  lmm
 * 	add memory allocation failure detection
 * 
 * 	HP revision /main/HPDCE02/6  1995/03/06  17:19 UTC  tatsu_s
 * 	Submitted the local rpc security bypass.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.local_rpc.b1/1  1995/02/22  22:31 UTC  tatsu_s
 * 	Local RPC Security Bypass.
 * 
 * 	HP revision /main/HPDCE02/5  1995/01/31  21:15 UTC  tatsu_s
 * 	Submitted the local rpc cleanup.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.vtalarm.b0/1  1995/01/26  20:34 UTC  tatsu_s
 * 	Call rpc__local_socket_close() instead of rpc__socket_close()
 * 	except for accept()'ed sockets.
 * 
 * 	HP revision /main/HPDCE02/4  1995/01/16  19:14 UTC  markar
 * 	Merging Local RPC mods
 * 
 * 	HP revision /main/HPDCE02/markar_local_rpc/2  1995/01/16  14:30 UTC  markar
 * 	init rpc_addr arg to desc_inq_peer_addr
 * 
 * 	HP revision /main/HPDCE02/markar_local_rpc/1  1995/01/05  19:11 UTC  markar
 * 	Implementing Local RPC bypass mechanism
 * 
 * 	HP revision /main/HPDCE02/3  1994/09/21  14:50 UTC  tatsu_s
 * 	Merged mothra up to dce1_1_b18.
 * 	[1995/12/07  23:57:38  root]
 * 
 * Revision 1.1.96.5  1994/08/22  20:38:37  tom
 * 	For port restrictions, make the client side follow the
 * 	restrictions too.  Call socket_bind before socket_connect. (OT 8238)
 * 	[1994/08/22  20:20:06  tom]
 * 
 * 	HP revision /main/HPDCE02/2  1994/08/17  20:41 UTC  tatsu_s
 * 	Merged from Hpdce02_01
 * 
 * Revision 1.1.96.4  1994/08/12  16:22:26  tom
 * 	When setting socket buffers, use globals
 * 	rpc_g_cn_socket_{read,write}_buffer (OT 11440).
 * 	[1994/08/11  20:20:22  tom]
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:34 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * Revision 1.1.8.3  1993/12/20  22:12:38  sommerfeld
 * 	add ifdef around prev. fix.
 * 	[1993/12/20  22:11:53  sommerfeld]
 * 
 * 	Add yield on accept error path to tone down annoying buzz loop.
 * 	[1993/12/20  22:10:59  sommerfeld]
 * 
 * Revision 1.1.8.2  1993/11/18  16:45:39  tatsu_s
 * 	Added the debugging output.
 * 	[1993/11/16  19:16:57  tatsu_s]
 * 
 * 	Fix OT8238.
 * 	Call rpc__socket_bind() in rpc__cn_network_req_connect().
 * 	[1993/11/15  20:39:00  tatsu_s]
 * 
 * Revision 1.1.5.3  1993/09/15  15:28:38  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.96.3  1994/06/21  21:53:38  hopkins
 * 	Serviceability
 * 	[1994/06/08  21:29:10  hopkins]
 * 
 * Revision 1.1.96.2  1994/05/19  21:14:26  hopkins
 * 	Merge with DCE1_1.
 * 	[1994/05/04  19:37:02  hopkins]
 * 
 * 	Serviceability work.
 * 	[1994/05/03  20:18:01  hopkins]
 * 
 * Revision 1.1.96.1  1994/01/21  22:34:27  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:54:13  cbrooks]
 * 
 * Revision 1.1.8.3  1993/08/26  19:15:55  tatsu_s
 * 	Bug 1709 - If rpc_addr_vec is empty, return rpc_s_no_addrs status
 * 	in rpc__cn_network_desc_inq_ep().
 * 	[1993/08/26  19:10:11  tatsu_s]
 * 
 * Revision 1.1.8.2  1993/08/26  16:43:58  tom
 * 	Bug 8099 - Don't DIE if keep alive option can not be set in
 * 	rpc__cn_network_req_connect().  Ignore the error.
 * 	[1993/08/25  20:13:46  tom]
 * 
 * Revision 1.1.4.10  1993/03/05  19:19:46  weisman
 * 	         11-feb-93     burton    add pthread_testcancel
 * 	                                 after pthread_cancel
 * 	[1993/03/05  19:07:36  weisman]
 * 
 * Revision 1.1.4.9  1993/01/03  22:58:51  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:59:47  bbelch]
 * 
 * Revision 1.1.4.8  1992/12/23  20:15:57  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:31:31  zeliff]
 * 
 * Revision 1.1.4.7  1992/12/18  19:54:22  wei_hu
 * 	      3-nov-92    wh    Enable async cancels across connect.
 * 	      3-nov-92    gh    Pass correct status in rpc__cn_network_req_connect().
 * 	[1992/12/18  19:44:20  wei_hu]
 * 
 * Revision 1.1.4.6  1992/11/19  20:38:56  marcyw
 * 	     Backing out questionable changes
 * 	     [1992/11/19  20:07:07  marcyw]
 * 
 * Revision 1.1.4.4  1992/11/13  21:07:35  zeliff
 * 	     Backing out questionable changes
 * 	     [1992/11/13  21:00:55  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/22  20:55:04  rsalz
 * 	Close 3855; unlock CN mutex only if locked.
 * 	[1992/06/22  19:48:25  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:09:27  devrcs
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
**      cnnet.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  The NCA Connection Protocol Service's Network Service.
**
**
 */

#include <commonp.h>    /* Common declarations for all RPC runtime */
#include <com.h>        /* Common communications services */
#include <comprot.h>    /* Common protocol services */
#include <cnp.h>        /* NCA Connection private declarations */
#include <cnpkt.h>      /* NCA Connection packet encoding */
#include <cnid.h>       /* NCA Connection local ID service */
#include <cnassoc.h>    /* NCA Connection association service */
#include <cnassm.h>     /* NCA Connection association state machine */
#include <cnfbuf.h>     /* NCA Connection fragment buffer service */
#include <cncall.h>     /* NCA Connection call service */
#include <cnnet.h>


/***********************************************************************/
/*
 * Global variables
 */
static unsigned32	rpc_g_cn_socket_read_buffer=0;
static unsigned32	rpc_g_cn_socket_write_buffer=0;


/***********************************************************************/
/*
 * Local routines
 */
INTERNAL void rpc__cn_network_desc_inq_ep _DCE_PROTOTYPE_ ((
    rpc_socket_t             /*desc*/,
    rpc_protseq_id_t         /*pseq_id*/,
    unsigned_char_p_t       * /*endpoint*/,
    unsigned32              *status));

INTERNAL void rpc__cn_network_serr_to_status _DCE_PROTOTYPE_ ((
    rpc_socket_error_t       /*serr*/,
    unsigned32              *st));

INTERNAL pointer_t rpc__cn_network_init_desc _DCE_PROTOTYPE_ ((
    rpc_socket_t                * /*desc*/,
    boolean32                    /*spawned*/,
    rpc_protseq_id_t             /*pseq_id*/,
    unsigned32                   /*backlog*/,
    unsigned32                  *st));


/***********************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_desc_inq_ep
**
**  SCOPE:              INTERNAL - declared locally.
**
**  DESCRIPTION:
**
**  This routine will return an endpoint for the given socket and protocol
**  sequence id.
**
**  INPUTS:
**
**      desc            The network descriptor (socket) being queried.
**
**      pseq_id         The protocol sequence id of the network descriptor.
**
**  INPUTS/OUTPUTS:
**
**      endpoint        A pointer to the endpoint determined by this routine.
**
**  OUTPUTS:            none
**
**      status          Status returned.
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

INTERNAL void rpc__cn_network_desc_inq_ep 
#ifdef _DCE_PROTO_
(
  rpc_socket_t            desc,
  rpc_protseq_id_t        pseq_id,
  unsigned_char_p_t       *endpoint,
  unsigned32              *status
)
#else
(desc, pseq_id, endpoint, status)
rpc_socket_t            desc;
rpc_protseq_id_t        pseq_id;
unsigned_char_p_t       *endpoint;
unsigned32              *status;
#endif
{
    rpc_addr_vector_p_t rpc_addr_vec;
    unsigned32          temp_status;
    
    RPC_CN_DBG_RTN_PRINTF (rpc__cn_network_desc_inq_ep);
    CODING_ERROR (status);

    /*
     * Create an RPC address from the socket descriptor.
     */
    rpc__naf_desc_inq_addr (pseq_id, desc, &rpc_addr_vec, status);
    if (*status != rpc_s_ok) return;
    
    if (rpc_addr_vec->len == 0)
    {
        rpc__naf_addr_vector_free (&rpc_addr_vec, &temp_status);
        *status = rpc_s_no_addrs;
        return;
    }

    /*
     * Inquire the endpoint from one of the RPC addresses returned.
     */
    rpc__naf_addr_inq_endpoint (rpc_addr_vec->addrs[0], endpoint, status);
    if (*status != rpc_s_ok)
    {
        rpc__naf_addr_vector_free (&rpc_addr_vec, &temp_status);
        return;
    }

    /*
     * Free the vector of RPC addresses.
     */
    rpc__naf_addr_vector_free (&rpc_addr_vec, status);
}

/***********************************************************************/


/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_use_protseq
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**
**  This routine will create the appropriate number of sockets based
**  on the max number of concurrent calls requested and set them up to
**  receive connect requests.
**
**  INPUTS:
**
**      pseq_id         The protocol sequence id over which to
**                      create the socket.
**      max_calls       The min number of concurrent call requests
**                      the server wishes to be able to handle.
**      rpc_addr        The rpc_addr containing the endpoint and use
**                      to bind to the socket. 
**      endpoint        The endpoint referred to above.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      status          status returned
**                              rpc_s_ok
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

PRIVATE void rpc__cn_network_use_protseq 
#ifdef _DCE_PROTO_
(
  rpc_protseq_id_t        pseq_id,
  unsigned32              max_calls,
  rpc_addr_p_t            rpc_addr,
  unsigned_char_p_t       endpoint,
  unsigned32              *status
)
#else
(pseq_id, max_calls, rpc_addr, endpoint, status)
rpc_protseq_id_t        pseq_id;
unsigned32              max_calls;
rpc_addr_p_t            rpc_addr;
unsigned_char_p_t       endpoint;
unsigned32              *status;
#endif
{
    unsigned32          i;
    unsigned32          num_socks;
    unsigned32          created_sock_index;
    rpc_socket_t        sock;
    rpc_socket_error_t  serr;
    pointer_t           priv_info;
    rpc_socket_t        *sock_list;
    unsigned32          backlog;
    unsigned32          temp_status;
    static boolean      spawned              = false;

    /*
     * Local/Static var's for use in auto-start handling.
     */
#ifdef AUTOSTART
    static boolean              spawned_same_address = false;
    static boolean              spawned_checked      = false;
           rpc_naf_id_t         auto_naf_id;
           rpc_network_if_id_t  auto_network_if_id;
           rpc_protocol_id_t    auto_prot_id;
           rpc_addr_vector_p_t  auto_rpc_addr_vec    = NULL;
    static unsigned_char_p_t    auto_endpoint        = NULL;
           unsigned32           auto_status          = rpc_s_coding_error;
#endif

    CODING_ERROR (status);
    
    RPC_CN_DBG_RTN_PRINTF (rpc__cn_network_use_protseq);

    /*
     * If the caller specified the default number of max calls use
     * the maximum listen backlog for a single socket. This is because
     * for a well-known endpoint we can only create one socket.
     */
    if (max_calls == rpc_c_protseq_max_calls_default)
    {
        max_calls = RPC_C_LISTEN_BACKLOG;
    }
    else
    {
        /*
         * If the callers has specified something other than the max
         * calls default. If it exceeds the system max of queued
         * connections on a socket and is using a well-known
         * endpoint then return an error. Mutiple sockets using the
         * same endpoint cannot be created.
         */
        if ((endpoint != NULL) && (max_calls > RPC_C_LISTEN_BACKLOG))
        {
           *status = rpc_s_calls_too_large_for_wk_ep; 
           return;
       }
    }

#ifdef AUTOSTART
    /*
     *************************************************************************
     * Autostart processing.                                                 *
     *                                                                       *
     * IF we have been spawned AND the socket associated with stdin has the  *
     * same address family, socket type, protocol family and endpoint as the *
     * protocol sequence being registered AND we have not already processed  *
     * the spawned case, THEN don't create a socket but use stdin instead.   *
     *************************************************************************
     */

    /*
     * See if we've already checked for auto-spawning.
     * If we have, don't do it again.
     */
    if (!spawned_checked)
    {
        /*
         * Determine the network address family id from the
         * RPC address given.
         */
        auto_naf_id = rpc_addr->sa.family;
        
        /*
         * If this call succeeds, we can safely assume stdin is a socket and
         * we will also have all the information about the socket that we need.
         */
        rpc__naf_desc_inq_network (0, 
                                   &auto_naf_id, 
                                   &auto_network_if_id, 
                                   &auto_prot_id, 
                                   &auto_status);
        spawned_checked = true;
        spawned = (auto_status == rpc_s_ok);

        /*
         * If we are spawned then determine the address associated with the
         * socket on stdin.  Then get the endpoint from the address for later
         * comparison with the endpoint associated with the protocol sequence
         * being registered (if it's well known).
         */
        if (spawned)
        {
            rpc__naf_desc_inq_addr (pseq_id, 0, &auto_rpc_addr_vec, status);
            if (*status != rpc_s_ok)
            {
                return;
            }
            if (auto_rpc_addr_vec->len == 0)
            {
                rpc__naf_addr_vector_free (&auto_rpc_addr_vec, &auto_status);
                *status = rpc_s_no_addrs;
                return;
            }
            rpc__naf_addr_inq_endpoint (auto_rpc_addr_vec->addrs[0],
                                        &auto_endpoint,
                                        status);
            if (*status != rpc_s_ok)
            {
                rpc__naf_addr_vector_free (&auto_rpc_addr_vec, &auto_status);
                return;
            }
            rpc__naf_addr_vector_free (&auto_rpc_addr_vec, &auto_status);
        }
    }

    /*
     * If we have been given an endpoint, then check the endpoint against
     * the endpoint of the socket on stdin.
     */
    if (endpoint != NULL)
    {
        if (spawned)
        {
            spawned_same_address = !strcmp ((char *) endpoint, 
                                            (char *) auto_endpoint);
        }
    }

#endif

    /*
     * Create as many sockets as are required to fufill the server's
     * request to be able to handle "max_calls" concurrent call
     * requests. For the CN protocol this means being able to
     * concurrently be able to queue and subsequently accept
     * "max_calls" connect requests. Since each socket can queue
     * "RPC_C_LISTEN_BACKLOG" connect requests the number of sockets
     * required are "max_calls"/"RPC_C_LISTEN_BACKLOG". Note that we
     * will not add the sockets to the listener thread's pool until all
     * of them have been created. This will avoid the problem of not
     * having a rpc__network_remove_desc function.
     */
    num_socks = (max_calls + RPC_C_LISTEN_BACKLOG - 1)/RPC_C_LISTEN_BACKLOG;
    RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                    ("(rpc__cn_network_use_protseq) Creating %d sockets\n", 
                     num_socks));
    RPC_MEM_ALLOC (sock_list, 
                   rpc_socket_t *, 
                   ((num_socks) * sizeof (rpc_socket_t)),
                   RPC_C_MEM_SOCKET_LIST,
                   RPC_C_MEM_WAITOK);
    if (sock_list == NULL){
	*status = rpc_s_no_memory;
	return;
    }
    for (i = 0; i < num_socks; i++)
        sock_list[i] = -1;
    for (i = 0, backlog = max_calls; 
         i < num_socks; 
         i++, backlog -= RPC_C_LISTEN_BACKLOG)
    {
        /*
         * First, create a socket for this RPC Protocol Sequence.
         */
        RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                        ("(rpc__cn_network_use_protseq) Created socket #%d\n", 
                         (i + 1)));
#ifdef AUTOSTART
        /*
         * See if we should use stdin as a socket or create a new one.
         */
        if ((i == 0) && (spawned_same_address))
        {
            /*
             * Just set the socket descriptor to stdin (0).
             */
            sock = 0;
        }
        else
#endif
        {
            serr = rpc__socket_open(pseq_id, &sock);
            if (RPC_SOCKET_IS_ERR (serr))
            {
                *status = rpc_s_cant_create_sock;
                break;
            }
        }

        /*
         * Set the socket to close itself if the process execs.
         */
        rpc__socket_set_close_on_exec(sock);

        /*
         * Next, bind the socket to the RPC address.
         */
        rpc__naf_addr_set_endpoint (endpoint, &rpc_addr, status);
        if (*status != rpc_s_ok)
        {
            rpc__socket_close (sock);
            break;
        }
        
        serr = rpc__socket_bind (sock, rpc_addr);
        if (RPC_SOCKET_IS_ERR (serr))
        {
            *status = rpc_s_cant_bind_sock;
            rpc__socket_close (sock);
            break;
        }

        /*
         * Initialize the socket.
         */
        priv_info = rpc__cn_network_init_desc (&sock,
                                               spawned,
                                               pseq_id,
                                               MIN (RPC_C_LISTEN_BACKLOG, backlog),
                                               status);
        if (*status != rpc_s_ok)
        {
            rpc__socket_close (sock);
            break;
        }

        /*
         * Finally, add the socket to the listener thread's select
         * pool of sockets.
         */
        rpc__network_add_desc (sock, 
                               true, 
                               (endpoint == NULL), 
                               pseq_id, 
                               priv_info, 
                               status);
        
        if (*status != rpc_s_ok)
        {
            rpc__socket_close (sock);
            break;
        }

        /*
         * Add the socket to our list we're keeping.
         */
        sock_list[i] = sock;
    }

    /*
     * Check whether we were able to create all the sockets we
     * needed to. If not, remove all sockets created in this call from
     * the listener thread's select pool and close the socket.
     * Ignore status codes being returned from remove_desc.
     */
    if (i != num_socks)
    {
        for (created_sock_index = 0; 
             created_sock_index < i; 
             created_sock_index++)
        {
	    if (sock_list[created_sock_index] != -1)
	    {
            rpc__network_remove_desc (sock_list[created_sock_index], &temp_status);
            rpc__socket_close (sock_list[created_sock_index]);
	    }
        }
    }
    else
    {
        *status = rpc_s_ok;
    }
    RPC_MEM_FREE (sock_list, RPC_C_MEM_SOCKET_LIST);
}


/***********************************************************************/


/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_init_desc
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**
**  This routine will do a socket listen on the network descriptor
**  given.
**
**  INPUTS:
**
**      spawned         A boolean indicating that the network descriptor
**                      being added is the product of an automatic
**                      spawning sequence and is to be replaced with
**                      another descriptor.
**
**      pseq_id         The protocol sequence id of the network
**                      descriptor being added.
**
**      backlog         The socket connection queue length.

**  INPUTS/OUTPUTS:
**
**      desc            The network descriptor being added to the
**                      listen thread's pool.
**
**  OUTPUTS:
**
**      status          status returned
**                              rpc_s_ok
**                              rpc_s_cant_create_sock
**                              rpc_s_cant_bind_sock
**                              rpc_s_cant_listen_sock
**                              rpc_s_no_addrs
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      return          The private information block to be
**                      registered with the network descriptor.
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL pointer_t rpc__cn_network_init_desc 
#ifdef _DCE_PROTO_
(
  rpc_socket_t            *desc,
  boolean32               spawned,
  rpc_protseq_id_t        pseq_id,
  unsigned32              backlog,
  unsigned32              *status
)
#else
(desc, spawned, pseq_id, backlog, status)
rpc_socket_t            *desc;
boolean32               spawned;
rpc_protseq_id_t        pseq_id;
unsigned32              backlog;
unsigned32              *status;
#endif
{
    rpc_socket_t        connected_desc;
    rpc_addr_vector_p_t rpc_addr_vec;
    rpc_addr_p_t        rpc_addr;
    unsigned_char_t     *endpoint;
    rpc_socket_error_t  serr;
    rpc_cn_assoc_t      *assoc;
    unsigned32          temp_status;
    unsigned32          ssize, rsize;
    
    CODING_ERROR (status);
    
    RPC_CN_DBG_RTN_PRINTF (rpc__cn_network_init_desc);

    /*
     * If we are spawned then we have to do a number of things differently.
     *
     * Get another socket and bind it to the same address as
     * the one for the socket descriptor passed to us except we need a unique
     * endpoint.  We get the unique endpoint by calling the set_endpoint
     * routine with a NULL endpoint.  It will dynamically assign us a unique
     * endpoint.
     */
    if (spawned)
    {
        /*
         * Determine the address(es) the descriptor passed in is
         * associated with.
         */
        connected_desc = *desc;
        rpc__naf_desc_inq_addr (pseq_id, connected_desc, &rpc_addr_vec, status);
        if (*status != rpc_s_ok)
        {
            return (NULL);
        }
        if (rpc_addr_vec->len == 0)
        {
            rpc__naf_addr_vector_free (&rpc_addr_vec, status);
            *status = rpc_s_no_addrs;
            return (NULL);
        }
        rpc_addr = rpc_addr_vec->addrs[0];

        /*
         * Get a new (and unique) endpoint address by providing a
         * NULL endpoint to the addr_set_endpoint routine.
         */
        rpc__naf_addr_set_endpoint (NULL, &rpc_addr, status);
        if (*status != rpc_s_ok)
        {
            rpc__naf_addr_vector_free (&rpc_addr_vec, &temp_status);
            return (NULL);
        }

        /*
         * Get another socket. Note that we need to invalidate the descriptor
         * passed to us if we return with an error since the calling routine,
         * register_protseq(), will close the descriptor and we don't want the
         * original descriptor passed to us to be closed since it's being
         * listened to due to the rpc__cn_assoc_listen() call below.
         */
        serr = rpc__socket_open (pseq_id, desc);
        if (RPC_SOCKET_IS_ERR(serr))
        {
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                            ("(rpc__cn_network_init_desc) rpc__socket_open failed, error = %d\n",
                             RPC_SOCKET_ETOI(serr)));
            rpc__naf_addr_vector_free (&rpc_addr_vec, status);
            *desc = -1;
            *status = rpc_s_cant_create_sock;
            return (NULL);
        }

        /*
         * Set the socket to close itself if the process execs.
         */
        rpc__socket_set_close_on_exec (*desc);

        /*
         * Bind the descriptor to the address created above.
         */
        serr = rpc__socket_bind (*desc, rpc_addr);
        if (RPC_SOCKET_IS_ERR(serr))
        {
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                            ("(rpc__cn_network_init_desc) rpc__socket_bind failed, error = %d\n",
                             RPC_SOCKET_ETOI(serr)));
            rpc__naf_addr_vector_free (&rpc_addr_vec, status);
            *status = rpc_s_cant_bind_sock;
            return (NULL);
        }
        rpc__naf_addr_vector_free (&rpc_addr_vec, &temp_status);
    }

    /*
     * Setup the socket's send and receive buffering
     */
    serr = rpc__socket_set_bufs (*desc, 
                                 rpc_g_cn_socket_read_buffer,
                                 rpc_g_cn_socket_write_buffer,
                                 &ssize,
                                 &rsize);
    if (RPC_SOCKET_IS_ERR (serr))
    {
        RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                        ("(rpc__cn_network_init_desc) desc->%x Can't set socket bufs, error=%d\n",
                         *desc,
                         RPC_SOCKET_ETOI (serr)));
    }

    RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_BUFFS,
                    ("(rpc__cn_network_init_desc) desc->%x desired_sndbuf %u, desired_rcvbuf %u\n",
                     *desc, rpc_g_cn_socket_read_buffer, rpc_g_cn_socket_write_buffer));
    RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_BUFFS,
                    ("(rpc__cn_network_init_desc) desc->%x actual sndbuf %lu, actual rcvbuf %lu\n",
                     *desc, ssize, rsize));

    if (rsize < RPC_C_ASSOC_MUST_RECV_FRAG_SIZE)
    {
        /*
         * rpc_m_recvbuf_toosmall
         * "(%s) Socket's maximum receive buffering is less than
         * NCA Connection Protocol minimum requirement"
         */
        RPC_DCE_SVC_PRINTF ((
	    DCE_SVC(RPC__SVC_HANDLE, "%s"),
	    rpc_svc_cn_errors,
	    svc_c_sev_fatal | svc_c_action_abort,
	    rpc_m_recvbuf_toosmall,
	    "rpc__cn_network_init_desc" ));
    }

    /*
     * Determine the endpoint of the socket.
     */
    rpc__cn_network_desc_inq_ep (*desc, pseq_id, &endpoint, status);
    if (*status != rpc_s_ok)
    {
        return (NULL);
    }


    if (spawned)
    {
        /*
         * Allocate an association control block. The association control
         * block which comes back will have all mutexes and condition
         * variables created. Also the receiver thread will be created.
         */
        assoc = rpc__cn_assoc_listen (connected_desc, endpoint, status);
        if (*status != rpc_s_ok)
        {
            rpc_string_free (&endpoint, &temp_status);
            return (NULL);
        }
    }

    /*
     * Now do the listen on the bound socket.
     */
    serr = rpc__socket_listen (*desc, backlog);
    if (RPC_SOCKET_IS_ERR(serr))
    {
        RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                        ("(rpc__cn_network_init_desc) desc->%x rpc__socket_listen failed, error = %d\n", 
                         *desc, RPC_SOCKET_ETOI(serr)));
        rpc_string_free (&endpoint, &temp_status);
        *status = rpc_s_cant_listen_sock;
        return (NULL);
    }

    *status = rpc_s_ok;
    return ((pointer_t) endpoint);
}


/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_inq_prot_vers
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**      
**  Return the version number of the NCA CN protocol currently in use.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      prot_id         The NCA protocol identifier.
**	version_major	The NCA CN major version number.
**	version_minor	The NCA CN minor version number.
**      status          The result of the operation. One of:
**                          rpc_s_ok
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE void rpc__cn_network_inq_prot_vers 
#ifdef _DCE_PROTO_
(
  unsigned8               *prot_id,
  unsigned32		*version_major,
  unsigned32		*version_minor,
  unsigned32              *status
)
#else
(prot_id, version_major, version_minor, status)
unsigned8               *prot_id;
unsigned32		*version_major;
unsigned32		*version_minor;
unsigned32              *status;
#endif
{
    CODING_ERROR (status);

    *prot_id = RPC_C_CN_PROTO_ID;
    *version_major = RPC_C_CN_PROTO_VERS;
    *version_minor = RPC_C_CN_PROTO_VERS_MINOR;
    *status = rpc_s_ok;
}


/***********************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_select_dispatch
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**
**  This routine will accept the connection request which has
**  arrived on the network descriptor provided as input. An
**  association control block will then be allocated (and as a result a
**  receiver thread to go along with it). Once the association
**  control block has been set up with the new connection information
**  the receiver thread will be started.
**
**  INPUTS:
**
**      desc            The network descriptor on which the network
**                      event arrived.
**      priv_info       The private information block given by the
**                      protocol service when the network descriptor
**                      was intialized.
**      is_active       true indicates the application is waiting
**                      for network events on this socket.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**                      rpc_s_ok
**                      rpc_s_cannot_accept
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


PRIVATE void rpc__cn_network_select_dispatch 
#ifdef _DCE_PROTO_
(
  rpc_socket_t            desc,
  pointer_t               priv_info,
  boolean32               is_active,
  unsigned32              *st
)
#else
(desc, priv_info, is_active, st)
rpc_socket_t            desc;
pointer_t               priv_info;
boolean32               is_active;
unsigned32              *st;
#endif
{
    rpc_socket_t        newdesc;
    rpc_cn_assoc_t      *assoc;
    rpc_socket_error_t  serr;
    
    RPC_CN_DBG_RTN_PRINTF (rpc__cn_network_select_dispatch);
    CODING_ERROR(st);
    
    RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                    ("CN: desc->%x connection request received\n", desc));
    
    /*
     * We have been called by the network listener thread because a
     * network event occured on the descriptor passed in. Since the
     * only descriptors the network listener thread is listening on for us
     * are listening sockets we should just be able to accept the
     * connection. 
     */
    serr = rpc__socket_accept (desc, NULL, &newdesc);
    if (RPC_SOCKET_IS_ERR(serr))
    {
        RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                        ("(rpc__cn_network_select_dispatch) desc->%x rpc__socket_accept failed, error = %d\n",
                         desc, RPC_SOCKET_ETOI(serr)));
        
        *st = rpc_s_cannot_accept;
	pthread_yield();
    }
    else
    {
#ifdef	DEBUG
        if (RPC_DBG2 (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL))
        {
            rpc_protseq_id_t    protseq_id;
            rpc_addr_p_t        rpc_addr;
            unsigned_char_t     *netaddr, *endpoint;
            unsigned32          dbg_status;

            rpc__naf_desc_inq_protseq_id (newdesc,
                                          RPC_C_PROTOCOL_ID_NCACN,
                                          &protseq_id,
                                          &dbg_status); 
            rpc__naf_desc_inq_peer_addr (newdesc, 
                                         protseq_id,
                                         &rpc_addr,
                                         &dbg_status);
	    if (dbg_status == rpc_s_no_memory){
		*st = rpc_s_no_memory;
		return;
	    }
            rpc__naf_addr_inq_netaddr (rpc_addr,
                                       &netaddr,
                                       &dbg_status);
	    if (dbg_status == rpc_s_no_memory){
		*st = rpc_s_no_memory;
		return;
	    }
            rpc__naf_addr_inq_endpoint (rpc_addr,
                                        &endpoint,
                                        &dbg_status);
	    if (dbg_status == rpc_s_no_memory){
		*st = rpc_s_no_memory;
		return;
	    }
            rpc__naf_addr_free (&rpc_addr, &dbg_status);
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                ("CN: desc->%x connection request accepted/new connection on desc->%x from %s[%s]\n",
                 desc,
                 newdesc,
                 netaddr,
                 endpoint));
            rpc_string_free (&netaddr, &dbg_status);
            rpc_string_free (&endpoint, &dbg_status);
        }
#endif
        if (!is_active)
        {
            /*
             * If the application is not currently listening for calls
             * then promptly close this socket and therefore the
             * connection.
             */
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_GENERAL,
                            ("CN: desc->%x socket not active ... being closed\n", newdesc));
            serr = rpc__socket_close (newdesc);
        }
        else
        {
            /*
             * Set the socket to close itself if the process execs.
             */
            rpc__socket_set_close_on_exec (newdesc);
            
#ifndef NODELAY_BROKEN
            /*
             * The application is listening for new calls.
             * Set the packet no delay option on the connection.
             */
            rpc__naf_set_pkt_nodelay (newdesc, NULL, st);
            if (*st != rpc_s_ok)
            {
                /* 
                 * The set option failed. We'll continue anyway.
                 */
                RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                                ("(rpc__cn_network_select_dispatch) desc->%x rpc__naf_set_pkt_nodelay failed, error = %d\n",
                                 newdesc, *st));
            }
#endif
            /*
             * Set the keepalive socket option for this connection.
             */
            serr = rpc__socket_set_keepalive (newdesc);
            if (RPC_SOCKET_IS_ERR(serr))
            {
                /* 
                 * The set option failed. We'll continue anyway.
                 */
                RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                                ("(rpc__cn_network_select_dispatch) desc->%x rpc__socket_set_keepalive failed, error = %d\n",
                                 newdesc, serr));
            }
            /*
             * Acquire the CN global mutex to prevent other threads from
             * running.
             */
            RPC_CN_LOCK ();
            
            /*
             * Allocate an association control block. The association
             * control block which comes back will have all mutexes and
             * condition variables created. Also the receiver thread
             * will be created.
             */
            assoc = rpc__cn_assoc_listen (newdesc, 
                                          (unsigned_char_t *) priv_info, 
                                          st);
            if (*st != rpc_s_ok)
            {
                /* 
                 * The association listen failed. Close the connection.
                 */
                serr = rpc__socket_close (newdesc);
            }

	    /*
	     * Release the CN global mutex to allow other threads to
	     * run.
	     */
	    RPC_CN_UNLOCK ();
	}
    }
}


/***********************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_req_connect
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**
**  This routine will create a connection to the address given as an
**  argument.
**
**  INPUTS:
**
**      assoc           The association control block allocated for
**                      this connection.
**      rpc_addr        The RPC address to which the connection is
**                      to be made.
**      call_r          The call rep data structure to be linked to
**                      the association control block. This may be
**                      NULL if this association is being allocated
**                      on the server side. 
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**                      rpc_s_ok
**                      rpc_s_cant_create_sock
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

PRIVATE void rpc__cn_network_req_connect 
#ifdef _DCE_PROTO_
(
  rpc_addr_p_t            rpc_addr,
  rpc_cn_assoc_p_t        assoc,
  unsigned32              *st
)
#else
(rpc_addr, assoc, st)
rpc_addr_p_t            rpc_addr;
rpc_cn_assoc_p_t        assoc;
unsigned32              *st;
#endif
{
    rpc_socket_error_t  serr;
    volatile boolean32  retry_op;
    boolean32           connect_completed;
    rpc_naf_id_t        naf_id;
    rpc_addr_p_t        temp_rpc_addr;
    unsigned32          temp_status;
    unsigned32          ssize, rsize;

    RPC_CN_DBG_RTN_PRINTF (rpc__cn_network_req_connect);
    CODING_ERROR(st);
    
    /*
     * Now we need to create a connection to the server address
     * contained in the RPC address given. First create a socket to
     * do the connect on.
     */
    serr = rpc__socket_open (rpc_addr->rpc_protseq_id, &assoc->cn_ctlblk.cn_sock);
    if (RPC_SOCKET_IS_ERR(serr))
    {
        RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                        ("(rpc__cn_network_req_connect) call_rep->%x assoc->%x desc->%x rpc__socket_open failed, error = %d\n",
                         assoc->call_rep,
                         assoc,
                         assoc->cn_ctlblk.cn_sock,
                         RPC_SOCKET_ETOI(serr)));
        
        *st = rpc_s_cant_create_sock;
    }
    else
    {
        /*
         * Setup the socket's send and receive buffering
         */
        serr = rpc__socket_set_bufs (assoc->cn_ctlblk.cn_sock,
                                     rpc_g_cn_socket_read_buffer,
                                     rpc_g_cn_socket_write_buffer,
                                     &ssize,
                                     &rsize);
        if (RPC_SOCKET_IS_ERR (serr))
        {
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                            ("(rpc__cn_network_req_connect) call_rep->%x assoc->%x desc->%x Can't set socket bufs, error=%d\n",
                             assoc->call_rep,
                             assoc,
                             assoc->cn_ctlblk.cn_sock, 
                             RPC_SOCKET_ETOI (serr)));
        }
        
        RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_BUFFS,
                        ("(rpc__cn_network_req_connect) desc->%x desired_sndbuf %u, desired_rcvbuf %u\n",
                         assoc->cn_ctlblk.cn_sock, rpc_g_cn_socket_read_buffer, rpc_g_cn_socket_write_buffer));
        RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_BUFFS,
                       ("(rpc__cn_network_req_connect) desc->%x actual sndbuf %lu, actual rcvbuf %lu\n",
                        assoc->cn_ctlblk.cn_sock, ssize, rsize));
        
        if (rsize < RPC_C_ASSOC_MUST_RECV_FRAG_SIZE)
        {
	    /*
	     * rpc_m_recvbuf_toosmall
	     * "(%s) Socket's maximum receive buffering is less than
	     * NCA Connection Protocol minimum requirement"
	     */
	    RPC_DCE_SVC_PRINTF ((
	    DCE_SVC(RPC__SVC_HANDLE, "%s"),
	        rpc_svc_cn_errors,
	        svc_c_sev_fatal | svc_c_action_abort,
	        rpc_m_recvbuf_toosmall,
	        "rpc__cn_network_req_connect" ));
        }

        /*
         * Set the socket to close itself if the process execs.
         */
        rpc__socket_set_close_on_exec(assoc->cn_ctlblk.cn_sock);

        /*
         * Allocate the temporarry rpc_addr to be passed to
         * rpc__socket_bind().
         */
        naf_id = rpc_g_protseq_id[rpc_addr->rpc_protseq_id].naf_id;
        rpc__naf_addr_alloc (rpc_addr->rpc_protseq_id,
                             naf_id,
                             (unsigned_char_p_t) NULL,  /* endpoint */
                             (unsigned_char_p_t) NULL,  /* network address */
                             (unsigned_char_p_t) NULL,  /* network option */
                             &temp_rpc_addr,
                             st);
        if (*st != rpc_s_ok)
        {
            serr = rpc__socket_close (assoc->cn_ctlblk.cn_sock);
            if (RPC_SOCKET_IS_ERR(serr))
            {
                /*
                 * The socket close failed.
                 */
                RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                                ("(rpc__cn_network_req_connect) desc->%x rpc__socket_close failed, error = %d\n", 
                                 assoc->cn_ctlblk.cn_sock, 
                                 RPC_SOCKET_ETOI(serr)));
            }
            return;
        }

        /*
         * Next, bind the socket to the RPC address.
         */
        serr = rpc__socket_bind (assoc->cn_ctlblk.cn_sock, temp_rpc_addr);
        rpc__naf_addr_free(&temp_rpc_addr, &temp_status);
        if (RPC_SOCKET_IS_ERR (serr))
        {
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                            ("(rpc__cn_network_req_connect) desc->%x rpc__socket_bind failed, error = %d\n",
                             assoc->cn_ctlblk.cn_sock, 
                             RPC_SOCKET_ETOI(serr)));
            *st = rpc_s_cant_bind_sock;
            
            /*
             * The bind request failed. Close the socket just created
             * and free the association control block.
             */
            serr = rpc__socket_close (assoc->cn_ctlblk.cn_sock);
            if (RPC_SOCKET_IS_ERR(serr))
            {
                /*
                 * The socket close failed.
                 */
                RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                                ("(rpc__cn_network_req_connect) desc->%x rpc__socket_close failed, error = %d\n", 
                                 assoc->cn_ctlblk.cn_sock, 
                                 RPC_SOCKET_ETOI(serr)));
            }
            return;
        }

        /*
         * Indicate that the connection is being attempted.
         */
        assoc->cn_ctlblk.cn_state = RPC_C_CN_CONNECTING;
        
        /*
         * Since the connect call will block we will release the CN
         * global mutex before the call and reqaquire it after the call.
         */
        RPC_CN_UNLOCK ();
        
        /*
         * Now actually do the connect to the server.
         */
        retry_op = true;

        /*
         * Poll for cancels while the connect is in progress.
         * Only exit the while loop on success, failure, or
         * when you've received a cancel and the cancel timer
         * has expired.
         * If it is just a cancel, set cancel_pending and 
         * start the cancel timer.
         */
        connect_completed = false;
        while (! connect_completed)
        {
            TRY
            {

#ifdef NON_CANCELLABLE_IO
	    /*
             * By posix definition pthread_setasynccancel is not a "cancel
             * point" because it must return an error status and an errno.
             * pthread_setasynccancel(CANCEL_ON) will not deliver
             * a pending cancel nor will the cancel be delivered asynchronously,
             * thus the need for pthread_testcancel.
             * 
	     */
                pthread_setasynccancel(CANCEL_ON);
	        pthread_testcancel();
#endif
                serr = rpc__socket_connect (assoc->cn_ctlblk.cn_sock, rpc_addr);

#ifdef NON_CANCELLABLE_IO
                pthread_setasynccancel(CANCEL_OFF);
#endif   
                /*
                 * If we got here, then the connect was not cancelled;
                 * it has therefore completed either successfully or
                 * with serr set.
                 */
                connect_completed = true;
            }
            CATCH (pthread_cancel_e)
            {
#ifdef NON_CANCELLABLE_IO
                pthread_setasynccancel(CANCEL_OFF);
#endif   

                RPC_CN_LOCK ();
                /*
                 * Record the fact that a cancel has been
                 * detected. Also, start a timer if this is
                 * the first cancel detected.
                 */
                rpc__cn_call_local_cancel (assoc->call_rep,
                                           &retry_op,
                                           st);
                RPC_DBG_PRINTF (rpc_e_dbg_cancel, RPC_C_CN_DBG_CANCEL,
                                ("(rpc__cn_network_req_connect) call_rep->%x assoc->%x desc->%x cancel caught before association setup\n", 
                                 assoc->call_rep,
                                 assoc,
                                 assoc->cn_ctlblk.cn_sock));
                RPC_CN_UNLOCK ();
            }
            ENDTRY
            if (!retry_op)
            {
                RPC_CN_LOCK ();
                rpc__socket_close (assoc->cn_ctlblk.cn_sock);
                return;
            }

        }

        /* 
         * The connect completed; see if it completed successfully.
         */
        RPC_CN_LOCK ();

        /*
         * This is not an error.   We are already connected.
         * This happens because the underlying threads package
         * uses non-blocking I/O.
         */
	if (serr == RPC_C_SOCKET_EISCONN)
	{
	    serr = RPC_C_SOCKET_OK;
	}

        if (RPC_SOCKET_IS_ERR(serr))
        {
            RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                            ("(rpc__cn_network_req_connect) desc->%x rpc__socket_connect failed, error = %d\n",
                             assoc->cn_ctlblk.cn_sock, 
                             RPC_SOCKET_ETOI(serr)));
            
            rpc__cn_network_serr_to_status (serr, st);
            
            /*
             * The connect request failed. Close the socket just created
             * and free the association control block.
             */
            serr = rpc__socket_close (assoc->cn_ctlblk.cn_sock);
            if (RPC_SOCKET_IS_ERR(serr))
            {
                /*
                 * The socket close failed.
                 */
                RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                                ("(rpc__cn_network_req_connect) desc->%x rpc__socket_close failed, error = %d\n", 
                                 assoc->cn_ctlblk.cn_sock, 
                                 RPC_SOCKET_ETOI(serr)));
            }
        }
        else
        {
#ifndef NODELAY_BROKEN
            /*
             * Set the packet no delay option on the connection.
             */
            rpc__naf_set_pkt_nodelay (assoc->cn_ctlblk.cn_sock,
                                      rpc_addr, 
                                      st);
            if (*st != rpc_s_ok)
            {
                /* 
                 * The set option failed. We'll continue anyway.
                 */
                RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                                ("(rpc__cn_network_req_connect) desc->%x rpc__naf_set_pkt_nodelay failed, error = %d\n",
                                 assoc->cn_ctlblk.cn_sock, 
                                 *st));
            }
#endif
            
            /*
             * Indicate that there is a valid connection.
             */
            assoc->cn_ctlblk.cn_state = RPC_C_CN_OPEN;
            
            /*
             * A connection is now set up. Tell the receiver thread to begin
             * receiving on the connection.
             */
            if (assoc->cn_ctlblk.cn_rcvr_waiters)
            {
                RPC_COND_SIGNAL (assoc->cn_ctlblk.cn_rcvr_cond, 
                                 rpc_g_global_mutex);
            }
            
            /*
             * Set the keepalive socket option for this connection.
             */
            serr = rpc__socket_set_keepalive (assoc->cn_ctlblk.cn_sock);
            if (RPC_SOCKET_IS_ERR(serr))
            {
                /*
                 * The socket set keepalive option failed.
                 * The connection on the other side may have gone away.
                 * This is not a big deal because the client will
                 * find this out in the normal manner.
                 */
                RPC_DBG_PRINTF (rpc_e_dbg_general, RPC_C_CN_DBG_ERRORS,
                                ("(rpc__cn_network_req_connect) desc->%x rpc__naf_set_pkt_keepalive failed failed, error = %d\n",
                                 assoc->cn_ctlblk.cn_sock, serr));
            }

            /*
             * Everything went OK. Return a successful status code. 
             */
            *st = rpc_s_ok;
        }
    }
}


/***********************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_close_connect
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**
**  This routine will close an open connection.
**
**  INPUTS:
**
**      assoc           The association control block to which this connection
**                      is attached. 
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      st              The return status of this routine.
**                      rpc_s_ok
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

PRIVATE void rpc__cn_network_close_connect 
#ifdef _DCE_PROTO_
(
  rpc_cn_assoc_p_t        assoc,
  unsigned32              *st
)
#else
(assoc, st)
rpc_cn_assoc_p_t        assoc;
unsigned32              *st;
#endif
{

    RPC_CN_DBG_RTN_PRINTF (rpc__cn_network_close_connect);
    CODING_ERROR (st);
    
    /*
     * Closing the connection will cause the receiver thread to stop
     * trying to receive on and and return to waiting on a condition
     * variable for a new connection. We don't want to close the
     * socket here since the a thread other than the receiver thread
     * for this connection may open a new socket, get the same
     * descriptor, and establish a connection. The receiver thread
     * would then never detect that the connection went away and two
     * receiver threads would be receiving off the same connection.
     */
    if (assoc->cn_ctlblk.cn_state == RPC_C_CN_OPEN)
    {
        pthread_cancel (assoc->cn_ctlblk.cn_rcvr_thread_id);
    }
    *st = rpc_s_ok;
}


/***********************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_mon
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**
**  This routine notifies the connection protocol that at least one
**  connection to the address space identified in the binding rep
**  provided as input should be kept open.
**
**  INPUTS:
**
**      binding_r       The binding rep identifying the client
**                      process to be monitored.
**      client_h        The unique identifier of the client process
**                      which is really the id of an association group.
**      rundown         The routine to call if the communications
**                      link to the client process fails.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**                      rpc_s_ok
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

PRIVATE void rpc__cn_network_mon 
#ifdef _DCE_PROTO_
(
  rpc_binding_rep_p_t     binding_r,
  rpc_client_handle_t     client_h,
  rpc_network_rundown_fn_t rundown,
  unsigned32              *st
)
#else
(binding_r, client_h, rundown, st)
rpc_binding_rep_p_t     binding_r;
rpc_client_handle_t     client_h;
rpc_network_rundown_fn_t rundown;
unsigned32              *st;
#endif
{
    rpc_cn_assoc_grp_t  *assoc_grp;
    rpc_cn_local_id_t   grp_id;

    RPC_CN_DBG_RTN_PRINTF (rpc__cn_network_mon);
    CODING_ERROR(st);
    
    /*
     * Get the association group using the group id provided as a
     * client handle. 
     */
    grp_id.all = (unsigned32) client_h;
    grp_id = rpc__cn_assoc_grp_lkup_by_id (grp_id,
                                           RPC_C_CN_ASSOC_GRP_SERVER,
                                           st);

    /*
     * If the association group control block can't be found
     * return an error.
     */
    if (RPC_CN_LOCAL_ID_VALID (grp_id))
    {
        /*
         * Now we have the association group. Place the rundown function
         * in it and bump the reference count.
         */
        assoc_grp = RPC_CN_ASSOC_GRP (grp_id);
        assoc_grp->grp_liveness_mntr = rundown;
        assoc_grp->grp_refcnt++;
        *st = rpc_s_ok;
    }
}


/***********************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_stop_mon
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**
**  This routine is called when it is no longer necessary for the
**  runtime to keep a connection open for the caller to the address
**  space identified by the provided client handle.
**
**  INPUTS:
**
**      binding_r       The binding rep identifying the client
**                      process to be monitored.
**      client_h        The unique identifier of the client process
**                      which is really an association group control block.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**                      rpc_s_ok
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

PRIVATE void rpc__cn_network_stop_mon 
#ifdef _DCE_PROTO_
(
  rpc_binding_rep_p_t     binding_r,
  rpc_client_handle_t     client_h,
  unsigned32              *st
)
#else
(binding_r, client_h, st)
rpc_binding_rep_p_t     binding_r;
rpc_client_handle_t     client_h;
unsigned32              *st;
#endif
{
    rpc_cn_assoc_grp_t  *assoc_grp;
    rpc_cn_local_id_t   grp_id;
    
    CODING_ERROR(st);
    RPC_CN_DBG_RTN_PRINTF (rpc__cn_network_stop_mon);
    
    /*
     * Get the association group using the group id provided as a
     * client handle. 
     */
    grp_id.all = (unsigned32) client_h;
    grp_id = rpc__cn_assoc_grp_lkup_by_id (grp_id,
                                           RPC_C_CN_ASSOC_GRP_SERVER,
                                           st);

    /*
     * If the association group control block can't be found
     * return an error.
     */
    if (RPC_CN_LOCAL_ID_VALID (grp_id))
    {
        /*
         * Now we have the association group. Decrement the reference count.
         */
        assoc_grp = RPC_CN_ASSOC_GRP (grp_id);
        assoc_grp->grp_refcnt--;
        *st = rpc_s_ok;
    }
}


/***********************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_maint
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**
**  This routine is called when a connection to the address space
**  represented by the binding rep should be kept alive. Since we are
**  assuming all our connections have the KEEPALIVE feature there is
**  nothing for us to do here except make sure we keep at least one
**  connection open.
**
**  INPUTS:
**
**      binding_r       The binding rep identifying the server
**                      process to which a communications link
**                      should be maintained.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**                      rpc_s_ok
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

PRIVATE void rpc__cn_network_maint 
#ifdef _DCE_PROTO_
(
  rpc_binding_rep_p_t     binding_r,
  unsigned32              *st
)
#else
(binding_r, st)
rpc_binding_rep_p_t     binding_r;
unsigned32              *st;
#endif
{
    rpc_cn_assoc_grp_t          *assoc_grp;
    rpc_cn_local_id_t           grp_id;

    CODING_ERROR(st);
    RPC_CN_DBG_RTN_PRINTF (rpc__cn_network_maint);
    
    /*
     * Get the association group using the group id contained in the
     * binding handle.
     */
    grp_id = rpc__cn_assoc_grp_lkup_by_id (((rpc_cn_binding_rep_t *)
                                            (binding_r))->grp_id,
                                           RPC_C_CN_ASSOC_GRP_CLIENT,
                                           st);  
    
    /*
     * If the association group control block can't be found
     * return an error.
     */
    if (RPC_CN_LOCAL_ID_VALID (grp_id))
    {
        /*
         * We now have the association group control block we've been
         * looking for. Increment the reference count.
         */
        assoc_grp = RPC_CN_ASSOC_GRP (grp_id);
        assoc_grp->grp_refcnt++;
        *st = rpc_s_ok;
    }
}


/***********************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_stop_maint
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**
**  This routine is called when a connection to the address space
**  represented by the binding rep need no longer be kept alive.
**
**  INPUTS:
**
**      binding_r       The binding rep identifying the server
**                      process to which a communications link
**                      was being maintained.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      st              The return status of this routine.
**                      rpc_s_ok
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

PRIVATE void rpc__cn_network_stop_maint 
#ifdef _DCE_PROTO_
(
  rpc_binding_rep_p_t     binding_r,
  unsigned32              *st
)
#else
(binding_r, st)
rpc_binding_rep_p_t     binding_r;
unsigned32              *st;
#endif
{
    rpc_cn_assoc_grp_t  *assoc_grp;
    rpc_cn_local_id_t   grp_id;

    CODING_ERROR(st);
    RPC_CN_DBG_RTN_PRINTF (rpc__cn_network_stop_maint);
    
    /*
     * Get the association group using the group id contained in the
     * binding handle.
     */
    grp_id = rpc__cn_assoc_grp_lkup_by_id (((rpc_cn_binding_rep_t *)
                                            (binding_r))->grp_id,
                                           RPC_C_CN_ASSOC_GRP_CLIENT,
                                           st); 
    
    /*
     * If the association group control block can't be found
     * return an error.
     */
    if (RPC_CN_LOCAL_ID_VALID (grp_id))
    {
        /*
         * We now have the association group control block we've been
         * looking for. Decrement the reference count.
         */
        assoc_grp = RPC_CN_ASSOC_GRP (grp_id);
        assoc_grp->grp_refcnt--;
        *st = rpc_s_ok;
    }
}

/***********************************************************************/


/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_connect_fail
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**
**  This routine determine whether a given status code indicated a connect
**  request failed.
**
**  INPUTS:
**
**      st              The status code.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     boolean32
**
**      true            The status code is from a failed connection
**                      request false otherwise
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE boolean32 rpc__cn_network_connect_fail 
#ifdef _DCE_PROTO_
(
unsigned32              st
)
#else
(st)
unsigned32              st;
#endif
{
    switch ((int)st)
    {
        case rpc_s_cancel_timeout:
        case rpc_s_connect_timed_out:
        case rpc_s_connect_rejected:
        case rpc_s_network_unreachable:
        case rpc_s_connect_no_resources:
        case rpc_s_rem_network_shutdown:
        case rpc_s_too_many_rem_connects:
        case rpc_s_no_rem_endpoint:
        case rpc_s_rem_host_down:
        case rpc_s_host_unreachable:
        case rpc_s_access_control_info_inv:
        case rpc_s_loc_connect_aborted:
        case rpc_s_connect_closed_by_rem:
        case rpc_s_rem_host_crashed:
        case rpc_s_invalid_endpoint_format:
        case rpc_s_cannot_connect:
        {
            return (true);
        }
        
        default:
        {
            return (false);
        }
    }
}

/***********************************************************************/


/*
**++
**
**  ROUTINE NAME:       rpc__cn_network_serr_to_status
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**
**  This routine converts a socket interface error into an RPC
**  status code.
**
**  INPUTS:
**
**      serr            The socket error.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      st              The status code.
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

INTERNAL void rpc__cn_network_serr_to_status 
#ifdef _DCE_PROTO_
(
  rpc_socket_error_t      serr,
  unsigned32              *st
)
#else
(serr, st)
rpc_socket_error_t      serr;
unsigned32              *st;
#endif 
{
    switch (serr)
    {
        case RPC_C_SOCKET_ETIMEDOUT:
        *st = rpc_s_connect_timed_out;
        break;
        
        case RPC_C_SOCKET_ECONNREFUSED:
        *st = rpc_s_connect_rejected;
        break;
        
        case RPC_C_SOCKET_ENETUNREACH:
        *st = rpc_s_network_unreachable;
        break;
        
        case RPC_C_SOCKET_ENOSPC:
        *st = rpc_s_connect_no_resources;
        break;
        
        case RPC_C_SOCKET_ENETDOWN:
        *st = rpc_s_rem_network_shutdown;
        break;
        
        case RPC_C_SOCKET_ETOOMANYREFS:
        *st = rpc_s_too_many_rem_connects;
        break;
        
        case RPC_C_SOCKET_ESRCH:
        *st = rpc_s_no_rem_endpoint;
        break;
        
        case RPC_C_SOCKET_EHOSTDOWN:
        *st = rpc_s_rem_host_down;
        break;
        
        case RPC_C_SOCKET_EHOSTUNREACH:
        *st = rpc_s_host_unreachable;
        break;
        
        case RPC_C_SOCKET_EACCESS:
        *st = rpc_s_access_control_info_inv;
        break;
        
        case RPC_C_SOCKET_ECONNABORTED:
        *st = rpc_s_loc_connect_aborted;
        break;
        
        case RPC_C_SOCKET_ECONNRESET:
        *st = rpc_s_connect_closed_by_rem;
        break;
        
        case RPC_C_SOCKET_ENETRESET:
        *st = rpc_s_rem_host_crashed;
        break;
        
        case RPC_C_SOCKET_ENOEXEC:
        *st = rpc_s_invalid_endpoint_format;
        break;
        
        default:
        *st = rpc_s_cannot_connect;
        break;
    }
}    


/*
**++
**
**  ROUTINE NAME:       rpc__cn_inq_sock_buffsize
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**
**  This routine returns the CN global socket buffer sizes.
**  A zero value means the operating system default.
**
**  INPUTS:		none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      rsize           The receive buffer size (rpc_g_cn_socket_read_buffer)
**
**      ssize           The send buffer size (rpc_g_cn_socket_read_buffer)
**
**      st              The status code.
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

PRIVATE void
rpc__cn_inq_sock_buffsize(
	unsigned32	*rsize,
	unsigned32	*ssize,
	error_status_t	*st)
{
    *rsize = rpc_g_cn_socket_read_buffer;
    *ssize = rpc_g_cn_socket_write_buffer;
    *st = rpc_s_ok;
}


/*
**++
**
**  ROUTINE NAME:       rpc__cn_set_sock_buffsize
**
**  SCOPE:              PRIVATE - declared in cnnet.h
**
**  DESCRIPTION:
**
**  This routine sets the CN global socket buffer sizes.
**  A zero value for either buffer will use the OS default buffering.
**
**  INPUTS:
**
**       rsize          The socket receive buffer size
**
**       ssize          The socket send buffer size
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      st              The status code.
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

PRIVATE void
rpc__cn_set_sock_buffsize(
	unsigned32	rsize,
	unsigned32	ssize,
	error_status_t	*st)
{
    rpc_g_cn_socket_read_buffer = rsize;
    rpc_g_cn_socket_write_buffer = ssize;
    *st = rpc_s_ok;
}
