/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dds.c,v $
 * Revision 1.1.518.2  1996/02/18  00:03:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:55:47  marty]
 *
 * Revision 1.1.518.1  1995/12/08  00:19:11  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:32 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:14 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:18 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/07  23:58:39  root]
 * 
 * Revision 1.1.516.3  1994/06/21  21:53:50  hopkins
 * 	Serviceability
 * 	[1994/06/08  21:30:34  hopkins]
 * 
 * Revision 1.1.516.2  1994/05/19  21:14:36  hopkins
 * 	Merge with DCE1_1.
 * 	[1994/05/04  19:38:58  hopkins]
 * 
 * 	Serviceability work.
 * 	[1994/05/03  20:19:18  hopkins]
 * 
 * Revision 1.1.516.1  1994/01/21  22:36:14  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:56:18  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  23:23:19  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:03:49  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  20:46:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:35:34  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  17:19:46  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  17:15:33  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:07:53  devrcs
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
**  NAME:
**
**      dds.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Implements operations on DDS (Apollo DOMAIN domain sockets) NAF addresses.
**
**
*/

#include <commonp.h>
#include <com.h>
#include <comnaf.h>

#include <netinet/in.h>         /* for ntohs, etc. */

/* =============================================================================== */

struct dds_addr {
    long        network;
    long        node;
};

struct sockaddr_dds {
    short           sdds_family;
    short           sdds_port;
    struct dds_addr sdds_addr;
};

typedef struct rpc_dds_addr_t {
    rpc_protseq_id_t        rpc_protseq_id;
    unsigned32              len;
    struct sockaddr_dds     sa;
} rpc_dds_addr_t, *rpc_dds_addr_p_t;

#define sock_dds_broadcast_node 0xefffffff

/* =============================================================================== */

/*
 * Define some TPDU/TSDU constants, if they haven't already been defined
 * (typically in a system header file).
 */

/*
 * The max # of data bytes that can go into a DDS packet body and be
 * accepted through the transport service interface (i.e., sockets).
 */
#ifndef rpc_c_dds_max_tsdu
#  define rpc_c_dds_max_tsdu 1280
#endif

/*
 * The max # of data bytes that can go into a UDP packet body such that
 * the resulting packet can fit through any of the local network
 * interfaces without being fragmented.
 */
#ifndef rpc_c_dds_max_loc_unfrg_tpdu
#  define rpc_c_dds_max_loc_unfrg_tpdu 1280
#endif

/*
 * The max # of data bytes that can go into a UDP packet body such that
 * the resulting packet can be sent to some host without being
 * fragmented along the path.
 */ 
#ifndef rpc_c_dds_max_pth_unfrg_tpdu     
#  define rpc_c_dds_max_pth_unfrg_tpdu 1280
#endif

/* =============================================================================== */

INTERNAL void addr_alloc 
    PROTOTYPE((
        rpc_protseq_id_t        rpc_protseq_id,
        rpc_naf_id_t            naf_id,
        unsigned_char_p_t       endpoint,
        unsigned_char_p_t       netaddr,
        unsigned_char_p_t       network_options,
        rpc_addr_p_t            *rpc_addr,
        unsigned32              *st
    ));

INTERNAL void addr_copy  
    PROTOTYPE((
        rpc_addr_p_t            src_rpc_addr,
        rpc_addr_p_t            *dst_rpc_addr,
        unsigned32              *st
    ));

INTERNAL void addr_free 
    PROTOTYPE((
        rpc_addr_p_t            *rpc_addr,
        unsigned32              *st
    ));

INTERNAL void addr_set_endpoint 
    PROTOTYPE((
        unsigned_char_p_t       endpoint,
        rpc_addr_p_t            *rpc_addr,
        unsigned32              *st
    ));

INTERNAL void addr_inq_endpoint 
    PROTOTYPE((
        rpc_addr_p_t            rpc_addr,
        unsigned_char_p_t       *endpoint,
        unsigned32              *st
    ));

INTERNAL void addr_set_netaddr 
    PROTOTYPE((
        unsigned_char_p_t       netaddr,
        rpc_addr_p_t            *rpc_addr,
        unsigned32              *st
    ));

INTERNAL void addr_inq_netaddr 
    PROTOTYPE((
        rpc_addr_p_t            rpc_addr,
        unsigned_char_p_t       *netaddr,
        unsigned32              *st
    ));

INTERNAL void addr_set_options 
    PROTOTYPE((
        unsigned_char_p_t       network_options,
        rpc_addr_p_t            *rpc_addr,
        unsigned32              *st
    ));

INTERNAL void addr_inq_options 
    PROTOTYPE((
        rpc_addr_p_t            rpc_addr,
        unsigned_char_p_t       *network_options,
        unsigned32              *st
    ));

INTERNAL void desc_inq_network
    PROTOTYPE ((
        rpc_socket_t                desc,
        rpc_network_if_id_t         *socket_type,
        rpc_network_protocol_id_t   *protocol_id,
        unsigned32                  *status
    ));

INTERNAL void desc_inq_addr 
    PROTOTYPE((
        rpc_protseq_id_t        protseq_id,
        rpc_socket_t            desc,
        rpc_addr_vector_p_t     *rpc_addrs,
        unsigned32              *st
    ));

INTERNAL void inq_max_tsdu
    PROTOTYPE((
        rpc_naf_id_t            naf_id,
        rpc_network_if_id_t     iftype,
        rpc_network_protocol_id_t  protocol,
        unsigned32              *max_tsdu,
        unsigned32              *status
    ));

INTERNAL void get_broadcast
    PROTOTYPE((
        rpc_naf_id_t            naf_id,
        rpc_protseq_id_t        rpc_protseq_id,
        rpc_addr_vector_p_t     *rpc_addrs,
        unsigned32              *status
    ));
              
INTERNAL boolean addr_compare
    PROTOTYPE((
        rpc_addr_p_t            addr1,
        rpc_addr_p_t            addr2,
        unsigned32              *st
    ));

INTERNAL void inq_max_pth_unfrag_tpdu
    PROTOTYPE((
        rpc_addr_p_t            rpc_addr,
        rpc_network_if_id_t     iftype,
        rpc_network_protocol_id_t protocol,
        unsigned32              *max_tpdu,
        unsigned32              *status
    ));

INTERNAL void inq_max_loc_unfrag_tpdu
    PROTOTYPE((
        rpc_naf_id_t            naf_id,
        rpc_network_if_id_t     iftype,
        rpc_network_protocol_id_t protocol,
        unsigned32              *max_tpdu,
        unsigned32              *status
    ));

INTERNAL void tower_flrs_from_addr
    PROTOTYPE ((
        rpc_addr_p_t                rpc_addr,
        twr_p_t                     *lower_flrs,
        unsigned32                  *status
    ));

INTERNAL void tower_flrs_to_addr
    PROTOTYPE ((
        byte_p_t                    tower_octet_string,
        rpc_addr_p_t                *rpc_addr,
        unsigned32                  *status
    ));
/* =============================================================================== */

/*
 * R P C _ _ D D S _ I N I T
 *
 */

void rpc__dds_init(epv, st)

rpc_naf_epv_t    **epv;
unsigned32       *st;

{
    static rpc_naf_epv_t naf_epv =
    {
        addr_alloc,
        addr_copy,
        addr_free,
        addr_set_endpoint,
        addr_inq_endpoint,
        addr_set_netaddr,
        addr_inq_netaddr,
        addr_set_options,
        addr_inq_options,
        desc_inq_addr,
        desc_inq_network,
        inq_max_tsdu,
        get_broadcast,
        addr_compare,
        inq_max_pth_unfrag_tpdu,
        inq_max_loc_unfrag_tpdu,
        NULL,
        NULL,
        tower_flrs_from_addr,
        tower_flrs_to_addr,
        NULL
    };

    *epv = &naf_epv;
    *st = error_status_ok;
}


/*
 * A D D R _ A L L O C
 *
 */

INTERNAL void addr_alloc(rpc_protseq_id, naf_id, endpoint, netaddr, network_options, rpc_addr, st)
rpc_protseq_id_t        rpc_protseq_id;
rpc_naf_id_t            naf_id;
unsigned_char_p_t       endpoint;
unsigned_char_p_t       netaddr;
unsigned_char_p_t       network_options;
rpc_addr_p_t            *rpc_addr;
unsigned32              *st;
{
    rpc_dds_addr_p_t dds_addr;

    assert(naf_id == rpc_c_naf_id_dds);

    RPC_MEM_ALLOC (
        dds_addr,
        rpc_dds_addr_p_t,
        sizeof (rpc_dds_addr_t),
        rpc_c_mem_rpc_addr,
        rpc_c_mem_waitok);

    if (dds_addr == NULL)
    {
        *st = rpc_s_no_memory;
        return;
    }

    *rpc_addr = (rpc_addr_p_t) dds_addr;

    dds_addr->rpc_protseq_id = rpc_protseq_id;
    dds_addr->len = sizeof dds_addr->sa;
    dds_addr->sa.sdds_family = rpc_c_naf_id_dds;

    dds_addr->sa.sdds_port = 0;
    dds_addr->sa.sdds_addr.network = 0;
    dds_addr->sa.sdds_addr.node = 0;
    
    addr_set_endpoint(endpoint, rpc_addr, st);
    addr_set_netaddr(netaddr, rpc_addr, st);
    addr_set_options(network_options, rpc_addr, st);
    
    *st = rpc_s_ok;
} 


/*
 * A D D R _ C O P Y  
 *
 */

INTERNAL void addr_copy(src_rpc_addr, dst_rpc_addr, st)
rpc_addr_p_t            src_rpc_addr;
rpc_addr_p_t            *dst_rpc_addr;
unsigned32              *st;
{ 
    rpc_dds_addr_p_t dds_addr;

    assert(src_rpc_addr->sa.family == rpc_c_naf_id_dds);

    RPC_MEM_ALLOC (
        dds_addr,
        rpc_dds_addr_p_t,
        sizeof (rpc_dds_addr_t),
        rpc_c_mem_rpc_addr,
        rpc_c_mem_waitok);

    if (dds_addr == NULL)
    {
        *st = rpc_s_no_memory;
        return;
    }

    *dst_rpc_addr = (rpc_addr_p_t) dds_addr;

    bcopy(src_rpc_addr, *dst_rpc_addr, sizeof *dds_addr);

    *st = rpc_s_ok;
} 


/*
 * A D D R _ F R E E
 *
 */

INTERNAL void addr_free(rpc_addr, st)
rpc_addr_p_t            *rpc_addr;
unsigned32              *st;
{ 
    assert((*rpc_addr)->sa.family == rpc_c_naf_id_dds);

    RPC_MEM_FREE(*rpc_addr, rpc_c_mem_rpc_addr);
    *rpc_addr = NULL;
    
    *st = rpc_s_ok;
} 


/*
 * A D D R _ S E T _ E N D P O I N T
 *
 */

INTERNAL void addr_set_endpoint(endpoint, rpc_addr, st)
unsigned_char_p_t       endpoint;
rpc_addr_p_t            *rpc_addr;
unsigned32              *st;
{ 
    rpc_dds_addr_p_t dds_addr = (rpc_dds_addr_p_t) *rpc_addr;

    assert(dds_addr->sa.sdds_family == rpc_c_naf_id_dds);

    if (endpoint == NULL)
        dds_addr->sa.sdds_port = 0;
    else
        dds_addr->sa.sdds_port = htons(atoi((char *) endpoint));
    
    *st = rpc_s_ok;
} 


/*
 * A D D R _ I N Q _ E N D P O I N T
 *
 */

INTERNAL void addr_inq_endpoint(rpc_addr, endpoint, st)
rpc_addr_p_t            rpc_addr;
unsigned_char_p_t       *endpoint;
unsigned32              *st;
{ 
#define     rpc_c_endpoint_dds_max   6   /* 5 ascii digits + nul */
    rpc_dds_addr_p_t dds_addr = (rpc_dds_addr_p_t) rpc_addr;

    assert(dds_addr->sa.sdds_family == rpc_c_naf_id_dds);

    if (dds_addr->sa.sdds_port == 0)
    {
        RPC_MEM_ALLOC(
            *endpoint,
            unsigned_char_p_t,
            sizeof(unsigned32),     /* can't stand to get just 1 byte */
            rpc_c_mem_string,
            rpc_c_mem_waitok);
	if (*endpoint == NULL){
	    *st = rpc_s_no_memory;
	    return;
	}
        *endpoint[0] = 0;
    }
    else
    {
        RPC_MEM_ALLOC(
            *endpoint,
            unsigned_char_p_t,
            rpc_c_endpoint_dds_max,
            rpc_c_mem_string,
            rpc_c_mem_waitok);
	if (*endpoint == NULL){
	    *st = rpc_s_no_memory;
	    return;
	}
        sprintf((char *) *endpoint, "%d", ntohs(dds_addr->sa.sdds_port));
    }
    
    *st = rpc_s_ok;
} 


/*
 * A D D R _ S E T _ N E T A D D R
 *
 */

INTERNAL void addr_set_netaddr(netaddr, rpc_addr, st)
unsigned_char_p_t       netaddr;
rpc_addr_p_t            *rpc_addr;
unsigned32              *st;
{ 
    rpc_dds_addr_p_t dds_addr = (rpc_dds_addr_p_t) *rpc_addr;
    long network, node;
    int i;
    boolean numeric;
    std_$call void name_$resolve(), file_$locatei();
    std_$call msg_$get_my_net();
    struct {
        long high;
        long low;
    } uid;

    assert(dds_addr->sa.sdds_family == rpc_c_naf_id_dds);

    /*
     * check to see if this is a request to remove the netaddr
     */
    if (netaddr == NULL || strlen ((char *) netaddr) == 0)
    {
        dds_addr->sa.sdds_addr.network = dds_addr->sa.sdds_addr.node = 0;
        *st = rpc_s_ok;
        return;
    }

    /*
     * See if there's a leading "#" -- means numeric address must follow.
     * Note we accept numeric addresses withOUT the "#" too.
     */
    numeric = (netaddr[0] == '#');
    if (numeric)
        netaddr++;

    i = sscanf((char *) netaddr, "%lx.%lx", &network, &node);
    if (i == 2)
    {
        dds_addr->sa.sdds_addr.node = node;
        dds_addr->sa.sdds_addr.network = network;
        *st = rpc_s_ok;
        return;
    }

    if (numeric)
    {
        *st = rpc_s_inval_net_addr;
        return;
    }

    name_$resolve(*netaddr, (short) strlen((char *) netaddr), uid, *st);
    if (*st != 0)
        return;

    file_$locatei(uid, dds_addr->sa.sdds_addr, *st);
    if (*st != 0)
        return;

    if (dds_addr->sa.sdds_addr.network == 0) 
        msg_$get_my_net(dds_addr->sa.sdds_addr.network);
    
    *st = rpc_s_ok;
} 


/*
 * A D D R _ I N Q _ N E T A D D R
 *
 */

INTERNAL void addr_inq_netaddr(rpc_addr, netaddr, st)
rpc_addr_p_t            rpc_addr;
unsigned_char_p_t       *netaddr;
unsigned32              *st;
{ 
#define NA_SIZE 18      /* !!! big enough for FFFFFFFF.FFFFFFFF plus a '\0' */

    rpc_dds_addr_p_t dds_addr = (rpc_dds_addr_p_t) rpc_addr;

    assert(dds_addr->sa.sdds_family == rpc_c_naf_id_dds);

    RPC_MEM_ALLOC(
        *netaddr,
        unsigned_char_p_t,
        NA_SIZE,
        rpc_c_mem_string,
        rpc_c_mem_waitok);
	if (*netaddr == NULL){
	    *st = rpc_s_no_memory;
	    return;
	}

    sprintf((char *) *netaddr, "%lx.%lx",
            ntohl(dds_addr->sa.sdds_addr.network), 
            ntohl(dds_addr->sa.sdds_addr.node));
    
    *st = rpc_s_ok;
} 


/*
 * A D D R _ S E T _ O P T I O N S
 *
 */

INTERNAL void addr_set_options(network_options, rpc_addr, st)
unsigned_char_p_t       network_options;
rpc_addr_p_t            *rpc_addr;
unsigned32              *st;
{ 
    /* a nil call for this naf */
    
    *st = rpc_s_ok;
} 


/*
 * A D D R _ I N Q _ O P T I O N S
 *
 */

INTERNAL void addr_inq_options(rpc_addr, network_options, st)
rpc_addr_p_t            rpc_addr;
unsigned_char_p_t       *network_options;
unsigned32              *st;
{ 
    RPC_MEM_ALLOC(
        *network_options,
        unsigned_char_p_t,
        sizeof(unsigned32),     /* only really need 1 byte */
        rpc_c_mem_string,
        rpc_c_mem_waitok);
	if (*network_options == NULL){
	    *st = rpc_s_no_memory;
	    return;
	}

    *network_options[0] = 0;
    *st = rpc_s_ok;
} 

/*
 * D E S C _ I N Q _ N E T W O R K
 *
 */

INTERNAL void desc_inq_network (desc, socket_type, protocol_id, status)
rpc_socket_t              desc;
rpc_network_if_id_t       *socket_type;
rpc_network_protocol_id_t *protocol_id;
unsigned32                *status;
{
    rpc_socket_error_t serr;
    
    CODING_ERROR (status);

    /*
     * Get the socket type.
     */
     
    serr = rpc__socket_get_if_id (desc, socket_type);
    if (RPC_SOCKET_IS_ERR (serr))
    {
        RPC_DBG_GPRINTF (("rpc__socket_get_if_id: serr=%d\n",serr));
        *status = -1;
        return;
    }

    /*
     * For now, there is a one to one relationship between the protocol family
     * and the socket type.
     */

    switch (*socket_type)
    {
        case SOCK_STREAM:       *protocol_id = 6;       /* TCP */
                                break;

        case SOCK_DGRAM:        *protocol_id = 17;      /* UDP */
                                break;

        default:
				/*
				 * rpc_m_unk_sock_type
				 * "(%s) Unknown socket type"
				 */
				RPC_DCE_SVC_PRINTF ((
				    DCE_SVC(RPC__SVC_HANDLE, "%s"),
				    rpc_svc_general
				    svc_c_sev_fatal | svc_c_action_abort,
				    rpc_m_unk_sock_type,
				    "desc_inq_network" ));
				break;
    }

    *status = rpc_s_ok;
}

/*
 * D E S C _ I N Q _ A D D R
 *
 */

INTERNAL void desc_inq_addr (protseq_id, desc, rpc_addrs, st)
rpc_protseq_id_t        protseq_id;
rpc_socket_t            desc;
rpc_addr_vector_p_t     *rpc_addrs;
unsigned32              *st;
{ 
    int len;
    rpc_dds_addr_p_t dds_addr;
    rpc_addr_vector_p_t vector;

    RPC_MEM_ALLOC (
        vector,
        rpc_addr_vector_p_t,
        sizeof (*vector),
        rpc_c_mem_rpc_addr_vec,
        rpc_c_mem_waitok);

    if (vector == NULL)
    {
        *st = rpc_s_no_memory;
        return;
    }

    RPC_MEM_ALLOC (
        dds_addr,
        rpc_dds_addr_p_t,
        sizeof (rpc_dds_addr_t),
        rpc_c_mem_rpc_addr,
        rpc_c_mem_waitok);

    if (dds_addr == NULL)
    {
        RPC_MEM_FREE(vector, rpc_c_mem_rpc_addr_vec);
        *st = rpc_s_no_memory;
        return;
    }

    len = sizeof(struct sockaddr_dds);

    if (getsockname((int) desc, &dds_addr->sa, &len) < 0)
    {
        RPC_MEM_FREE(dds_addr, rpc_c_mem_rpc_addr);
        RPC_MEM_FREE(vector, rpc_c_mem_rpc_addr_vec);
        *st = rpc_s_no_memory;
        return;
    }

    vector->len = 1;
    vector->addrs[0] = (rpc_addr_p_t) dds_addr;

    dds_addr->rpc_protseq_id = protseq_id;
    dds_addr->len = sizeof dds_addr->sa;

    *rpc_addrs = vector;
    *st = rpc_s_ok;
} 


/*
 * I N Q _ M A X _ T S D U
 *
 */

INTERNAL void inq_max_tsdu (naf_id, iftype, protocol, max_tsdu, st)
rpc_naf_id_t naf_id;
rpc_network_if_id_t iftype;
rpc_network_protocol_id_t protocol;
unsigned32 *max_tsdu;
unsigned32     *st;
{
    *max_tsdu = rpc_c_dds_max_tsdu;
    *st = rpc_s_ok;
}


/*
 * G E T _ B R O A D C A S T
 *
 * !!! Shouldn't we worry about node's with two network cards?
 *
 */
INTERNAL void get_broadcast (naf_id, rpc_protseq_id, rpc_addrs, st)
rpc_naf_id_t naf_id;
rpc_protseq_id_t rpc_protseq_id;
rpc_addr_vector_p_t *rpc_addrs;
unsigned32     *st; 
{
    rpc_dds_addr_p_t dds_addr;
    rpc_addr_vector_p_t vector;

    RPC_MEM_ALLOC (
        vector,
        rpc_addr_vector_p_t,
        sizeof (*vector),
        rpc_c_mem_rpc_addr_vec,
        rpc_c_mem_waitok);

    if (vector == NULL)
    {
        *st = rpc_s_no_memory;
        return;
    }

    RPC_MEM_ALLOC (
        dds_addr,
        rpc_dds_addr_p_t,
        sizeof (rpc_dds_addr_t),
        rpc_c_mem_rpc_addr,
        rpc_c_mem_waitok);

    if (dds_addr == NULL)
    {
        RPC_MEM_FREE(vector, rpc_c_mem_rpc_addr_vec);
        *st = rpc_s_no_memory;
        return;
    }

    vector->len = 1;
    vector->addrs[0] = (rpc_addr_p_t) dds_addr;

    dds_addr->rpc_protseq_id = rpc_protseq_id;
    dds_addr->len = sizeof dds_addr->sa;

    dds_addr->sa.sdds_family = rpc_c_naf_id_dds;
    dds_addr->sa.sdds_port = 0;
    dds_addr->sa.sdds_addr.network = 0;
    dds_addr->sa.sdds_addr.node = htonl(sock_dds_broadcast_node);
    
    *rpc_addrs = vector;
    *st = rpc_s_ok;
}

                                                      
/*
 * A D D R _ C O M P A R E 
 *
 * Determine if two address are equal.
 */
INTERNAL boolean addr_compare (addr1, addr2, st)
rpc_addr_p_t    addr1, addr2;
unsigned32      *st;
{
    rpc_dds_addr_p_t dds_addr1 = (rpc_dds_addr_p_t) addr1;
    rpc_dds_addr_p_t dds_addr2 = (rpc_dds_addr_p_t) addr2;

    if (dds_addr1->rpc_protseq_id == dds_addr2->rpc_protseq_id &&
        dds_addr1->len == dds_addr2->len &&       
        dds_addr1->sa.sdds_family == dds_addr2->sa.sdds_family &&       
        dds_addr1->sa.sdds_port == dds_addr2->sa.sdds_port &&       
        dds_addr1->sa.sdds_addr.network == dds_addr2->sa.sdds_addr.network &&       
        dds_addr1->sa.sdds_addr.node == dds_addr2->sa.sdds_addr.node )
    {
        return (true);
    }
    else
    {
        return (false);
    }
}


/*
 * I N Q _ M A X _ P T H _ U N F R A G _ T P D U
 *
 */

INTERNAL void inq_max_pth_unfrag_tpdu (rpc_addr, iftype, protocol, max_tpdu, st)
rpc_addr_p_t rpc_addr;
rpc_network_if_id_t iftype;
rpc_network_protocol_id_t protocol;
unsigned32 *max_tpdu;
unsigned32     *st;
{
    *max_tpdu = rpc_c_dds_max_pth_unfrg_tpdu;

#ifdef DEBUG
    if (rpc_g_dbg_switches[rpc_es_dbg_dds_max_pth_unfrag_tpdu] > 0)  
        *max_tpdu = rpc_g_dbg_switches[rpc_es_dbg_dds_max_pth_unfrag_tpdu] * 32;
#endif

    *st = rpc_s_ok;
}


/*
 * I N Q _ M A X _ L O C _ U N F R A G _ T P D U
 *
 */

INTERNAL void inq_max_loc_unfrag_tpdu (naf_id, iftype, protocol, max_tpdu, st)
rpc_naf_id_t naf_id;
rpc_network_if_id_t iftype;
rpc_network_protocol_id_t protocol;
unsigned32 *max_tpdu;
unsigned32     *st;
{
    *max_tpdu = rpc_c_dds_max_loc_unfrg_tpdu;

#ifdef DEBUG
    if (rpc_g_dbg_switches[rpc_es_dbg_dds_max_loc_unfrag_tpdu] > 0)  
        *max_tpdu = rpc_g_dbg_switches[rpc_es_dbg_dds_max_loc_unfrag_tpdu] * 32;
#endif

    *st = rpc_s_ok;
}

/*
 * T O W E R _ F L R S _ F R O M _ A D D R
 *
 */

INTERNAL void tower_flrs_from_addr (rpc_addr, lower_flrs, status)
rpc_addr_p_t       rpc_addr;
twr_p_t            *lower_flrs;
unsigned32         *status;
{
    unsigned32    net_prot_id;

    CODING_ERROR (status);

#ifdef RPC_NO_TOWER_SUPPORT

    *status = rpc_s_coding_error;

#else

#if 0
    /*
     * The use of a temporary lower floors twr_t is in anticipation
     * of the twr_* routines belonging to a separate library with
     * their own memory allocation. In that case, twr_* allocates
     * memory for returning the lower towers, and we must copy from
     * twr_* memory into our (rpc) memory. After the copy, we free
     * the twr_* allocated memory. 
     *
     * For now, twr_* routines also use RPC_MEM_ALLOC and RPC_MEM_FREE,
     * so we'll skip the extra copy.
     */
    twr_p_t     temp_lower_flrs;
#endif

    /*
     * Get the network protocol id (aka transport layer protocol)
     * for this RPC addr.
     */
    net_prot_id = RPC_PROTSEQ_INQ_NET_PROT_ID(rpc_addr->rpc_protseq_id);

    /*
     * Convert sockaddr to lower tower floors.
     */
    twr_dds_lower_flrs_from_sa (net_prot_id, (sockaddr_t *) &(rpc_addr->sa), 
#if 0
        &temp_lower_flrs, 
#else
        lower_flrs, 
#endif
        status);

    if (*status != twr_s_ok)
    {
        return;
    }

#if 0
    /*
     * Allocate a tower structure to hold the wire (and nameservice)
     * representation of the lower tower floors returned from twr_*().
     *
     * The size includes the sizof twr_t + length of the tower floors 
     * returned from twr_dds_lower_flrs_from_sa - 1 (for tower_octet_string[0].
     */
    RPC_MEM_ALLOC (
        *lower_flrs, 
        twr_p_t, 
        sizeof(twr_t) + temp_lower_flrs->tower_length - 1,
        rpc_c_mem_tower, 
        rpc_c_mem_waitok );
    if (*lower_flrs == NULL){
	*status = rpc_s_no_memory;
	return;
    }

    /*
     * Set the tower length to the length of the tower flrs returnd from 
     * twr_ip_lower_flrs_from_sa.
     */
    (*lower_flrs)->tower_length = temp_lower_flrs->tower_length;

    /*
     * Copy the lower tower floors to the tower octet string.
     */
    memcpy ((*lower_flrs)->tower_octet_string, 
        temp_lower_flrs->tower_octet_string,
        temp_lower_flrs->tower_length);

    /*
     * Free the twr_ip_lower_flrs_from_sa allocated memory.
     */
    RPC_MEM_FREE (temp_lower_flrs, rpc_c_mem_tower);
#endif

#endif

    return;
}

/*
 * T O W E R _ F L R S _ T O _ A D D R
 *
 */

INTERNAL void tower_flrs_to_addr (tower_octet_string, rpc_addr, status)
byte_p_t           tower_octet_string;
rpc_addr_p_t       *rpc_addr;
unsigned32         *status;
{
    sockaddr_t    *sa;
    unsigned32    sa_len;

    CODING_ERROR (status);

#ifdef RPC_NO_TOWER_SUPPORT

    *status = rpc_s_coding_error;

#else

    /*
     * Convert the lower floors of a tower to a sockaddr.
     */
    twr_dds_lower_flrs_to_sa (
        tower_octet_string,        /* tower octet string (has flr count). */
        &sa,                       /* returned sockaddr     */
        &sa_len,                   /* returned sockaddr len */
        status);

    if (*status != twr_s_ok)
    {
        return;
    }

    /*
     * Call the common NAF routine to create an RPC addr from a sockaddr.
     * (rpc__naf_addr_from_sa doesn't dispatch to a naf-specific routine.)
     */
    rpc__naf_addr_from_sa (sa, sa_len, rpc_addr, status);

    /*
     * Always free the twr_ip_lower_flrs_to_sa allocated memory - regardless 
     * of the status from rpc__naf_addr_from_sa.
     */
    RPC_MEM_FREE (sa, rpc_c_mem_rpc_addr);

#endif

    /*
     * Return whatever status we had.
     */
    return;
}
