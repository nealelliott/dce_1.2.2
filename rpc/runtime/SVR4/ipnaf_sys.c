/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: ipnaf_sys.c,v $
 * Revision 1.1.62.2  1996/02/18  00:01:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:12  marty]
 *
 * Revision 1.1.62.1  1995/12/08  00:16:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:09  root]
 * 
 * Revision 1.1.60.3  1994/06/29  20:55:26  ohara
 * 	add the rpc__ip* routines
 * 	[1994/06/09  16:50:23  ohara]
 * 
 * Revision 1.1.60.2  1994/02/02  22:06:00  ohara
 * 	code cleanup
 * 	[1994/02/02  21:02:23  ohara]
 * 
 * Revision 1.1.60.1  1994/01/21  22:33:37  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:59:51  cbrooks]
 * 
 * Revision 1.1.3.5  1993/02/01  20:35:40  hinman
 * 	[hinman@sni] - Final merge before bsubmit
 * 	[1993/01/31  16:13:53  hinman]
 * 
 * Revision 1.1.3.4  1993/01/03  22:57:33  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:57:45  bbelch]
 * 	Revision 1.1.4.2  1993/01/11  16:11:15  hinman
 * 	[hinman] - Check in merged SNI version
 * 
 * Revision 1.1.3.3  1992/12/23  20:11:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:29:17  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:42:19  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:45:08  weisman]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  NAME
**
**      ipnaf_bsd
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This module contains routines specific to the Internet Protocol,
**  the Internet Network Address Family extension service, and the
**  Berkeley Unix system.
**
**
*/


#ifdef SNI_SVR4
#include <sys/types.h>
#include <sys/fcntl.h>
#include <netconfig.h>
#include <sys/stropts.h>
#include <sys/sockio.h>
#endif /* SNI_SVR4 */

#include <commonp.h>
#include <com.h>
#include <comnaf.h>
#include <ipnaf.h>

#include <net/if.h>
#include <sys/ioctl.h>




/***********************************************************************
 *
 *  Internal prototypes and typedefs.
 */

typedef boolean (*enumerate_fn_p_t)    _DCE_PROTOTYPE_ ((
        int                     /* in  */  /*desc*/,
        struct ifreq            /* in  */ * /*ifr*/,
        unsigned32              /* in  */  /*if_flags*/,
        struct sockaddr_in      /* in  */ * /*if_addr*/,
        rpc_ip_addr_p_t         /* out */  /*ip_addr*/,
	rpc_ip_addr_p_t         /* out */  /*netmask_addr*/
    ));

INTERNAL void enumerate_interfaces _DCE_PROTOTYPE_ ((
        rpc_protseq_id_t         /*protseq_id*/,
        rpc_socket_t             /*desc*/,
        enumerate_fn_p_t         /*efun*/,
        rpc_addr_vector_p_t     * /*rpc_addr_vec*/,
	 rpc_addr_vector_p_t     * /*netmask_addr_vec*/,
        unsigned32              * /*st*/
    ));

INTERNAL boolean get_addr _DCE_PROTOTYPE_ ((
        int                      /*desc*/,
        struct ifreq            * /*ifr*/,
        unsigned32               /*if_flags*/,
        struct sockaddr_in      * /*if_addr*/,
        rpc_ip_addr_p_t          /*ip_addr*/,
	rpc_ip_addr_p_t          /*netmask_addr*/
    ));                            

INTERNAL boolean get_broadcast_addr _DCE_PROTOTYPE_ ((
        int                      /*desc*/,
        struct ifreq            * /*ifr*/,
        unsigned32               /*if_flags*/,
        struct sockaddr_in      * /*if_addr*/,
        rpc_ip_addr_p_t          /*ip_addr*/,
	rpc_ip_addr_p_t          /*netmask_addr*/
    ));

INTERNAL int do_ioctl _DCE_PROTOTYPE_ ((
		int		 /*s*/,
		int		 /*cmd*/,
		char		* /*data*/,
		int		 /*len*/
 ));


#ifndef NO_SPRINTF
#  define RPC__IP_NETWORK_SPRINTF   sprintf
#else
#  define RPC__IP_NETWORK_SPRINTF   rpc__ip_network_sprintf
#endif

typedef struct
{
    unsigned32  num_elt;
    struct
    {
        unsigned32  addr;
        unsigned32  netmask;
    } elt[1];
} rpc_ip_s_addr_vector_t, *rpc_ip_s_addr_vector_p_t;

INTERNAL rpc_ip_s_addr_vector_p_t local_ip_addr_vec = NULL;

/*
**++
**
**  ROUTINE NAME:       enumerate_interfaces 
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  Return a vector of IP RPC addresses.  Note that this function is
**  shared by both "rpc__ip_desc_inq_addr" and "rpc__ip_get_broadcast"
**  so that we have to have only one copy of all the gore (ioctl's)
**  associated with inquiring about network interfaces.  This routine
**  filters out all network interface information that doesn't correspond
**  to up, non-loopback, IP-addressed network interfaces.  The supplied
**  procedure pointer (efun) does the rest of the work.
**  
**
**  INPUTS:
**
**      protseq_id      Protocol Sequence ID representing a particular
**                      Network Address Family, its Transport Protocol,
**                      and type.
**
**      desc            Descriptor, indicating a socket that has been
**                      created on the local operating platform.
**
**      efun            Procedure pointer supplied to "do the rest of the work".
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      rpc_addr_vec    Returned vector of RPC addresses.
**
**
**      status          A value indicating the status of the routine.
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


 

INTERNAL void enumerate_interfaces 
#ifdef _DCE_PROTO_
(
    rpc_protseq_id_t        protseq_id,
    rpc_socket_t            desc,
    enumerate_fn_p_t        efun,
    rpc_addr_vector_p_t     *rpc_addr_vec,
    rpc_addr_vector_p_t     *netmask_addr_vec,
    unsigned32              *status
)
#else
(protseq_id, desc, efun, rpc_addr_vec, netmask_addr_vec, status)
rpc_protseq_id_t        protseq_id;
rpc_socket_t            desc;
enumerate_fn_p_t        efun;
rpc_addr_vector_p_t     *rpc_addr_vec;
rpc_addr_vector_p_t     *netmask_addr_vec;
unsigned32              *status;
#endif
{
    rpc_ip_addr_p_t         ip_addr;
    int                     n_ifs;
    unsigned char           buf[1024];
    struct ifconf           ifc;
    struct ifreq            *ifr, *last_ifr;
    struct ifreq            ifreq;
    short                   if_flags;
    struct sockaddr_in      if_addr;
    int                     i;
#ifdef _SOCKADDR_LEN
    int                     prev_size;
#else
const size_t prev_size = sizeof(struct ifreq);
#endif
    rpc_ip_addr_p_t         netmask_addr = NULL;


#ifdef SNI_SVR4
	int		ll;
#endif

    CODING_ERROR (status);

#ifndef SNI_SVR4

    /*
     * Get the list of network interfaces.
     */


    ifc.ifc_len = sizeof (buf);
    ifc.ifc_buf = (caddr_t) buf;
#endif

ifconf_again:
#ifdef SNI_SVR4
	if ((ll = do_ioctl(desc,SIOCGIFCONF,(char *) buf, sizeof(buf))) <0)
#else
    if (ioctl (desc, (int) SIOCGIFCONF, (caddr_t) &ifc) < 0) 
#endif
    {
        if (errno == EINTR)
        {
            goto ifconf_again;
        }
        *status = -2;   /* !!! */
        return;
    }

    /*
     * Figure out how many interfaces there must be and allocate an  
     * RPC address vector with the appropriate number of elements.
     * (We may ask for a few too many in case some of the interfaces
     * are uninteresting.)
     */
    n_ifs = ll/ sizeof (struct ifreq);

    RPC_DBG_PRINTF(rpc_e_dbg_general, 10,
        ("%d bytes of ifreqs, ifreq is %d bytes\n", ll, sizeof(struct ifreq)));

#ifdef MAX_DEBUG
    if (RPC_DBG2(rpc_e_dbg_general, 15))
    {
        int i;
	char msgbuf[128];

        for (i=0; i<ifc.ifc_len; i++) {
            if ((i % 32) == 0) {
                if (i != 0)
                    RPC_DBG_PRINTF(rpc_e_dbg_general, 15, ("%s\n",msgbuf));
                sprintf(msgbuf, "%4x: ", i);
             }
        sprintf(msgbuf, "%s%02x ", msgbuf, buf[i]);
        }
        if (i != 0)
            RPC_DBG_PRINTF(rpc_e_dbg_general, 15, ("%s\n",msgbuf));

    }
#endif

    RPC_MEM_ALLOC (
        *rpc_addr_vec,
        rpc_addr_vector_p_t,
        (sizeof **rpc_addr_vec) + ((n_ifs - 1) * (sizeof (rpc_addr_p_t))),
        RPC_C_MEM_RPC_ADDR_VEC,
        RPC_C_MEM_WAITOK);
    
    if (*rpc_addr_vec == NULL)
    {
        *status = rpc_s_no_memory;
        return;
    }
    if (netmask_addr_vec != NULL)
    {
        RPC_MEM_ALLOC (
            *netmask_addr_vec,
            rpc_addr_vector_p_t,
         (sizeof **netmask_addr_vec) + ((n_ifs - 1) * (sizeof (rpc_addr_p_t))),
            RPC_C_MEM_RPC_ADDR_VEC,
            RPC_C_MEM_WAITOK);

        if (*netmask_addr_vec == NULL)
        {
            *status = rpc_s_no_memory;
            RPC_MEM_FREE (*rpc_addr_vec, RPC_C_MEM_RPC_ADDR_VEC);
            return;
        }

        (*netmask_addr_vec)->len = 0;
    }


    /*
     * Go through the interfaces and get the info associated with them.
     */
    (*rpc_addr_vec)->len = 0;
#ifdef SNI_SVR4
	for (ifr = (struct ifreq *)buf; --n_ifs >=0; ifr++)
#else
    last_ifr = (struct ifreq *) (ifc.ifc_buf + ifc.ifc_len);

    for (i=0, ifr = ifc.ifc_req; ifr < last_ifr ;
         i++, ifr = (struct ifreq *)(( (char *) ifr ) + prev_size))
#endif
    {
        RPC_DBG_PRINTF(rpc_e_dbg_general, 10, ("interface %d: %s\n",
            i, ifr->ifr_name));

        /*
         * Get the interface's flags.  If the flags say that the interface
         * is not up or is the loopback interface, skip it.  Do the
         * SIOCGIFFLAGS on a copy of the ifr so we don't lose the original
         * contents of the ifr.  (ifr's are unions that hold only one
         * of the interesting interface attributes [address, flags, etc.]
         * at a time.)
         */
        memcpy(&ifreq, ifr, sizeof(ifreq));
ifflags_again:        
#ifdef SNI_SVR4
		if (do_ioctl(desc,SIOCGIFFLAGS, (char *)&ifreq,
			 sizeof(struct ifreq)) <0)

#else
        if (ioctl(desc, SIOCGIFFLAGS, &ifreq) < 0)
#endif
        {
            RPC_DBG_PRINTF(rpc_e_dbg_general, 10,
                ("SIOCGIFFLAGS returned errno %d\n", errno));
            if (errno == EINTR)
            {
                goto ifflags_again;
            }
            *status = -3;
            continue;
        }
        if_flags = ifreq.ifr_flags;     /* Copy out the flags */
        RPC_DBG_PRINTF(rpc_e_dbg_general, 10, ("flags are %x\n", if_flags));

        /*
         * Ignore interfaces which are not 'up'.
         */
        if ((if_flags & IFF_UP) == 0)
            continue;

        /*
         * Get the addressing stuff for this interface.
         */
#ifdef NO_SIOCGIFADDR
        /*
         * Note that some systems do not return the address for the
         * interface given.  However the ifr array elts contained in
         * the ifc block returned from the SIOCGIFCONF ioctl above already
         * contains the correct addresses. So these systems should define
         * NO_SIOCGIFADDR in their platform specific include file.
         */
#else
        /*
         * Do the SIOCGIFADDR on a copy of the ifr.  See above.
         */
        memcpy(&ifreq, ifr, sizeof(ifreq));
ifaddr_again:
#ifdef SNI_SVR4
		if (do_ioctl(desc,SIOCGIFADDR, (char *)&ifreq,
			 sizeof(struct ifreq)) <0)
#else
        if (ioctl(desc, SIOCGIFADDR, &ifreq) < 0)
#endif
        {
            RPC_DBG_PRINTF(rpc_e_dbg_general, 10,
                ("SIOCGIFADDR returned errno %d\n", errno));
            if (errno == EINTR)
            {
                goto ifaddr_again;
            }

            *status = -4;
            goto FREE_IT;
        }
#endif
        memcpy (&if_addr, (struct sockaddr_in *) &ifr->ifr_addr, sizeof(struct sockaddr));

        /*
         * If this isn't an Internet-family address, ignore it.
         */
        if (if_addr.sin_family != AF_INET)
        {
            RPC_DBG_PRINTF(rpc_e_dbg_general, 10, ("AF %d not INET\n",
                if_addr.sin_family));
            continue;
        }


        /*
         * Allocate and fill in an IP RPC address for this interface.
         */
        RPC_MEM_ALLOC (
            ip_addr,
            rpc_ip_addr_p_t,
            sizeof (rpc_ip_addr_t),
            RPC_C_MEM_RPC_ADDR,
            RPC_C_MEM_WAITOK);

        if (ip_addr == NULL)
        {
            *status = rpc_s_no_memory;
            goto FREE_IT;
        }

        ip_addr->rpc_protseq_id = protseq_id;
        ip_addr->len            = sizeof (struct sockaddr_in);
        if (netmask_addr_vec != NULL)
        {
            RPC_MEM_ALLOC (
                netmask_addr,
                rpc_ip_addr_p_t,
                sizeof (rpc_ip_addr_t),
                RPC_C_MEM_RPC_ADDR,
                RPC_C_MEM_WAITOK);
            
            if (netmask_addr == NULL)
            {
                *status = rpc_s_no_memory;
                RPC_MEM_FREE (ip_addr, RPC_C_MEM_RPC_ADDR);
                goto FREE_IT;
            }
            
            netmask_addr->rpc_protseq_id = protseq_id;
            netmask_addr->len            = sizeof (struct sockaddr_in);
        }

        /*
         * Call out to do any final filtering and get the desired IP address
         * for this interface.  If the callout function returns false, we
         * forget about this interface.
         */
       if ((*efun) (desc, ifr, if_flags, &if_addr, ip_addr, netmask_addr) == false) 
        {
            RPC_MEM_FREE (ip_addr, RPC_C_MEM_RPC_ADDR);
            if (netmask_addr != NULL)
                RPC_MEM_FREE (netmask_addr, RPC_C_MEM_RPC_ADDR);
            continue;
        }

        RPC_SOCKET_FIX_ADDRLEN(ip_addr);
        (*rpc_addr_vec)->addrs[(*rpc_addr_vec)->len++] = (rpc_addr_p_t) ip_addr;
        if (netmask_addr_vec != NULL && netmask_addr != NULL)
            (*netmask_addr_vec)->addrs[(*netmask_addr_vec)->len++]
                = (rpc_addr_p_t) netmask_addr;
    }

    *status = rpc_s_ok;
    return;

FREE_IT:

    for (i = 0; i < (*rpc_addr_vec)->len; i++)
    {
        RPC_MEM_FREE ((*rpc_addr_vec)->addrs[i], RPC_C_MEM_RPC_ADDR);
    }

    RPC_MEM_FREE (*rpc_addr_vec, RPC_C_MEM_RPC_ADDR_VEC);
    if (netmask_addr_vec != NULL)
    {
        for (i = 0; i < (*netmask_addr_vec)->len; i++)
        {
            RPC_MEM_FREE ((*netmask_addr_vec)->addrs[i], RPC_C_MEM_RPC_ADDR);
        }
        RPC_MEM_FREE (*netmask_addr_vec, RPC_C_MEM_RPC_ADDR_VEC);
    }
}

/*
**++
**
**  ROUTINE NAME:       get_addr
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  This function is called from "rpc__ip_desc_inq_addr" via
**  "enumerate_interfaces".  See comments in "enumerate_interfaces" for
**  details.
**
**
**  INPUTS:             none
**
**      desc            Socket being used for ioctl's.
**
**      ifr             Structure describing the interface.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      ip_addr
**
**      netmask_addr    netmask address
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     
**
**      result          true => we generated up an address for this interface
**                      false => we didn't.
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL boolean get_addr 
#ifdef _DCE_PROTO_
(
    int                     desc,
    struct ifreq            *ifr,
    unsigned32              if_flags,
    struct sockaddr_in      *if_addr,
    rpc_ip_addr_p_t         ip_addr,
    rpc_ip_addr_p_t         netmask_addr
)
#else
(desc, ifr, if_flags, if_addr, ip_addr, netmask_addr)
int                     desc;
struct ifreq            *ifr;
unsigned32              if_flags;
struct sockaddr_in      *if_addr;
rpc_ip_addr_p_t         ip_addr;
rpc_ip_addr_p_t         netmask_addr;
#endif
{
    struct ifreq            ifreq;

    if (netmask_addr == NULL)
    {
        if ((if_flags & IFF_LOOPBACK) != 0)
        {
            return (false);
        }

        memcpy (&ip_addr->sa, if_addr, sizeof(struct sockaddr_in));
        return (true);
    }
    else
    {
        memcpy (&ip_addr->sa, if_addr, sizeof(struct sockaddr_in));

        /*
         * Inquire the interface's netmask address.
         */
        ifreq = *ifr;
    ifnetaddr_again:
        if (ioctl(desc, (int) SIOCGIFNETMASK, &ifreq) == -1)
        {
            if (errno == EINTR)
            {
                goto ifnetaddr_again;
            }

            return (false);
        }

        memcpy (&netmask_addr->sa, &ifreq.ifr_addr, sizeof(struct sockaddr_in));
        return (true);
    }
}

/*
**++
**
**  ROUTINE NAME:       rpc__ip_desc_inq_addr
**
**  SCOPE:              PRIVATE - declared in ipnaf.h
**
**  DESCRIPTION:
**      
**  Receive a socket descriptor which is queried to obtain family, endpoint
**  and network address.  If this information appears valid for an IP
**  address,  space is allocated for an RPC address which is initialized
**  with the information obtained from the socket.  The address indicating
**  the created RPC address is returned in rpc_addr.
**
**  INPUTS:
**
**      protseq_id      Protocol Sequence ID representing a particular
**                      Network Address Family, its Transport Protocol,
**                      and type.
**
**      desc            Descriptor, indicating a socket that has been
**                      created on the local operating platform.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      rpc_addr_vec
**
**      status          A value indicating the status of the routine.
**
**          rpc_s_ok               The call was successful.
**
**          rpc_s_no_memory         Call to malloc failed to allocate memory.
**
**          rpc_s_cant_inq_socket  Attempt to get info about socket failed.
**
**          Any of the RPC Protocol Service status codes.
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

PRIVATE void rpc__ip_desc_inq_addr 
#ifdef _DCE_PROTO_
(
    rpc_protseq_id_t        protseq_id,
    rpc_socket_t            desc,
    rpc_addr_vector_p_t     *rpc_addr_vec,
    unsigned32              *status
)
#else
(protseq_id, desc, rpc_addr_vec, status)
rpc_protseq_id_t        protseq_id;
rpc_socket_t            desc;
rpc_addr_vector_p_t     *rpc_addr_vec;
unsigned32              *status;
#endif
{
    rpc_ip_addr_p_t         ip_addr;
    rpc_ip_addr_t           loc_ip_addr;
    unsigned16              i;


    CODING_ERROR (status);

    /*
     * Do a "getsockname" into a local IP RPC address.  If the network
     * address part of the result is non-zero, then the socket must be
     * bound to a particular IP address and we can just return a RPC
     * address vector with that one address (and endpoint) in it.
     * Otherwise, we have to enumerate over all the local network
     * interfaces the local host has and construct an RPC address for
     * each one of them.
     */
#ifdef SNI_SVR4
    loc_ip_addr.len = sizeof (struct sockaddr_in);
#else
    loc_ip_addr.len = sizeof (rpc_ip_addr_t);
#endif

    if (getsockname (desc, &loc_ip_addr.sa, &loc_ip_addr.len) < 0)
    {
        *status = -1;   /* !!! */
        return;
    }

    if (loc_ip_addr.sa.sin_addr.s_addr == 0)
    {
        enumerate_interfaces
            (protseq_id, desc, get_addr, rpc_addr_vec, NULL, status);

        if (*status != rpc_s_ok)
        {
            return; 
        }
        for (i = 0; i < (*rpc_addr_vec)->len; i++)
        {
            ((rpc_ip_addr_p_t) (*rpc_addr_vec)->addrs[i])->sa.sin_port = loc_ip_addr.sa.sin_port;
        }
    }
    else
    {
        RPC_MEM_ALLOC (
            ip_addr,
            rpc_ip_addr_p_t,
            sizeof (rpc_ip_addr_t),
            RPC_C_MEM_RPC_ADDR,
            RPC_C_MEM_WAITOK);

        if (ip_addr == NULL)
        {
            *status = rpc_s_no_memory;
            return;
        }

        RPC_MEM_ALLOC (
            *rpc_addr_vec,
            rpc_addr_vector_p_t,
            sizeof **rpc_addr_vec,
            RPC_C_MEM_RPC_ADDR_VEC,
            RPC_C_MEM_WAITOK);
    
        if (*rpc_addr_vec == NULL)
        {
            RPC_MEM_FREE (ip_addr, RPC_C_MEM_RPC_ADDR);
            *status = rpc_s_no_memory;
            return;
        }

        ip_addr->rpc_protseq_id = protseq_id;
        ip_addr->len            = sizeof (struct sockaddr_in);
        ip_addr->sa             = loc_ip_addr.sa;

        (*rpc_addr_vec)->len = 1;
        (*rpc_addr_vec)->addrs[0] = (rpc_addr_p_t) ip_addr;

        *status = rpc_s_ok;
        return;
    }
}

/*
**++
**
**  ROUTINE NAME:       get_broadcast_addr
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  This function is called from "rpc__ip_get_broadcast" via
**  "enumerate_interfaces".  See comments in "enumerate_interfaces" for
**  details.
**
**
**  INPUTS:             none
**
**      desc            Socket being used for ioctl's.
**
**      ifr             Structure describing the interface.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      ip_addr
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**      result          true => we generated up an address for this interface
**                      false => we didn't.
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL boolean get_broadcast_addr 
#ifdef _DCE_PROTO_
(
    int                     desc,
    struct ifreq            *ifr,
    unsigned32              if_flags,
    struct sockaddr_in      *if_addr,
    rpc_ip_addr_p_t         ip_addr,
    rpc_ip_addr_p_t         netmask_addr
)
#else
(desc, ifr, if_flags, if_addr, ip_addr, netmask_addr)
int                     desc;
struct ifreq            *ifr;
unsigned32              if_flags;
struct sockaddr_in      *if_addr;
rpc_ip_addr_p_t         ip_addr;
rpc_ip_addr_p_t         netmask_addr;
#endif
{
    struct ifreq            ifreq;
	int					err;	

    /*
     * If the interface's flags say this isn't a broadcast interface,
     * or isn't up, ignore it.
     */
    if ((if_flags & IFF_BROADCAST) == 0 || (if_flags & IFF_UP) == 0)
    {
        return (false);
    }

#ifndef BROADCAST_NEEDS_LOOPBACK
    /*
     * #define BROADCAST_NEEDS_LOOPBACK in case you need to broadcast
     * over the loopback interface to see your own broadcasts.
     */
    if ((if_flags & IFF_LOOPBACK) != 0)
    {
        return (false);
    }
#endif

    /*
     * Inquire the interface's broadcast address.
     */
    ifreq = *ifr;
ifbrdaddr_again:
#ifdef SNI_SVR4
    if ((err = do_ioctl(desc, (int) SIOCGIFBRDADDR, (char *) &ifreq, sizeof (struct ifreq))) < 0) 
#else
    if (ioctl(desc, (int) SIOCGIFBRDADDR, &ifreq) < 0) 
#endif
    {
        if (errno == EINTR)
        {
            goto ifbrdaddr_again;
        }

        return (false);
    }

    memcpy (&ip_addr->sa, &ifreq.ifr_broadaddr, sizeof(struct sockaddr_in));
#ifdef _SOCKADDR_LEN
    ip_addr->sa.sin_len = 0;
#endif
    return (true);
}

/*
**++
**
**  ROUTINE NAME:       rpc__ip_get_broadcast
**
**  SCOPE:              PRIVATE - EPV declared in ipnaf.h
**
**  DESCRIPTION:
**      
**  Return a vector of RPC addresses that represent all the address
**  required so that sending on all of them results in broadcasting on
**  all the local network interfaces.
**
**
**  INPUTS:
**
**      naf_id          Network Address Family ID serves
**                      as index into EPV for IP routines.
**
**      rpc_protseq_id
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:                        
**
**      rpc_addr_vec
**
**      status          A value indicating the status of the routine.
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

PRIVATE void rpc__ip_get_broadcast 
#ifdef _DCE_PROTO_
(
    rpc_naf_id_t            naf_id,
    rpc_protseq_id_t        protseq_id,
    rpc_addr_vector_p_t     *rpc_addr_vec,
    unsigned32              *status 
)
#else
(naf_id, protseq_id, rpc_addr_vec, status)
rpc_naf_id_t            naf_id;
rpc_protseq_id_t        protseq_id;
rpc_addr_vector_p_t     *rpc_addr_vec;
unsigned32              *status; 
#endif
{
    int                     desc;


    CODING_ERROR (status);

    /*
     * Open a socket to pass to "enumerate_interface".
     */
    desc = socket(AF_INET, SOCK_DGRAM, 0);

    if (desc < 0) 
    {
        *status = -7;   /* !!! */
        return;
    }

    enumerate_interfaces
        (protseq_id, desc, get_broadcast_addr, rpc_addr_vec, NULL, status);
    close(desc);
}


/*
 * this routine isolates the SVR4 ioctl
 * See SVR4 Programmer's Guide: Streams for more info.
 * 
 * INPUT:
 * s		file descriptor
 * cmd		the IOCTL needed
 * data	        ptr to data passed
 * len		length of data passed
 *
 * OUTPUT:
 *    0, -1 or length
 */
	

int do_ioctl 
#ifdef _DCE_PROTO_
(
    int	s,		/* file descriptor */
    int	cmd,
    char	*data,
    int	len
)
#else
(s, cmd, data, len)
int	s;		/* file descriptor */
int	cmd;
char	*data;
int	len;
#endif
{
	struct strioctl	ic;
	
	ic.ic_cmd = cmd;
	ic.ic_len = len;

	/* use the default value for timeout; it doing so
           becomes a problem, pass it as an arg  
         */
	ic.ic_timout = 0;

	ic.ic_dp = data;
	return (ioctl(s,I_STR,(char *) &ic) < 0 ? -1 : ic.ic_len);
}
/*
**++
**
**  ROUTINE NAME:       rpc__ip_init_local_addr_vec
**
**  SCOPE:              PRIVATE - declared in ipnaf.h
**
**  DESCRIPTION:
**      
**  Initialize the local address vectors.
**
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:                        
**
**      status          A value indicating the status of the routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:
**
**      Update local_ip_addr_vec
**
**--
**/

PRIVATE void rpc__ip_init_local_addr_vec
#ifdef _DCE_PROTO_
(
    unsigned32 *status
)
#else
(status)
unsigned32 *status; 
#endif
{
    int                     desc;
    unsigned32              lstatus;
    unsigned32              i;
    rpc_addr_vector_p_t     rpc_addr_vec = NULL;
    rpc_addr_vector_p_t     netmask_addr_vec = NULL;

    CODING_ERROR (status);

    /*
     * Open a socket to pass to "enumerate_interface".
     */
    desc = socket(AF_INET, SOCK_DGRAM, 0);

    if (desc < 0) 
    {
        *status = rpc_s_cant_create_socket;   /* !!! */
        return;
    }

    enumerate_interfaces
        (RPC_C_PROTSEQ_ID_NCADG_IP_UDP, desc, get_addr,
         &rpc_addr_vec, &netmask_addr_vec, status);
    close(desc);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Do some sanity check.
     */

    if (rpc_addr_vec == NULL
        || netmask_addr_vec == NULL
        || rpc_addr_vec->len != netmask_addr_vec->len
        || rpc_addr_vec->len == 0)
    {
        RPC_DBG_GPRINTF(("(rpc__ip_init_local_addr_vec) no local address\n"));
        *status = rpc_s_no_addrs;
        goto free_rpc_addrs;
    }

    RPC_MEM_ALLOC (
        local_ip_addr_vec,
        rpc_ip_s_addr_vector_p_t,
        (sizeof *local_ip_addr_vec)
            + ((rpc_addr_vec->len - 1) * (sizeof (local_ip_addr_vec->elt[0]))),
        RPC_C_MEM_UTIL,
        RPC_C_MEM_WAITOK);
    if (local_ip_addr_vec == NULL)
    {
        *status = rpc_s_no_memory;
        goto free_rpc_addrs;
    }

    local_ip_addr_vec->num_elt = rpc_addr_vec->len;

    for (i = 0; i < rpc_addr_vec->len; i++)
    {
        local_ip_addr_vec->elt[i].addr =
            ((rpc_ip_addr_p_t) rpc_addr_vec->addrs[i])->sa.sin_addr.s_addr;
        local_ip_addr_vec->elt[i].netmask =
            ((rpc_ip_addr_p_t) netmask_addr_vec->addrs[i])->sa.sin_addr.s_addr;
#ifdef DEBUG
        if (RPC_DBG2(rpc_e_dbg_general, 10))
        {
            char         buff[16], mbuff[16];
            unsigned8    *p, *mp;

            p = (unsigned8 *) &(local_ip_addr_vec->elt[i].addr);
            mp = (unsigned8 *) &(local_ip_addr_vec->elt[i].netmask);
            RPC__IP_NETWORK_SPRINTF(buff, "%d.%d.%d.%d",
                                    UC(p[0]), UC(p[1]), UC(p[2]), UC(p[3]));
            RPC__IP_NETWORK_SPRINTF(mbuff, "%d.%d.%d.%d",
                                    UC(mp[0]), UC(mp[1]), UC(mp[2]), UC(mp[3]));
            RPC_DBG_PRINTF(rpc_e_dbg_general, 10,
            ("(rpc__ip_init_local_addr_vec) local network [%s] netmask [%s]\n",
                            buff, mbuff));
        }
#endif
    }

free_rpc_addrs:
    if (rpc_addr_vec != NULL)
    {
        for (i = 0; i < rpc_addr_vec->len; i++)
        {
            RPC_MEM_FREE (rpc_addr_vec->addrs[i], RPC_C_MEM_RPC_ADDR);
        }
        RPC_MEM_FREE (rpc_addr_vec, RPC_C_MEM_RPC_ADDR_VEC);
    }
    if (netmask_addr_vec != NULL)
    {
        for (i = 0; i < netmask_addr_vec->len; i++)
        {
            RPC_MEM_FREE (netmask_addr_vec->addrs[i], RPC_C_MEM_RPC_ADDR);
        }
        RPC_MEM_FREE (netmask_addr_vec, RPC_C_MEM_RPC_ADDR_VEC);
    }
    return;
}

/*
**++
**
**  ROUTINE NAME:       rpc__ip_is_local_network
**
**  SCOPE:              PRIVATE - declared in ipnaf.h
**
**  DESCRIPTION:
**      
**  Return a boolean value to indicate if the given RPC address is on
**  the same IP subnet.
**
**
**  INPUTS:
**
**      rpc_addr        The address that forms the path of interest
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:                        
**
**      status          A value indicating the status of the routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      result          true => the address is on the same subnet.
**                      false => not.
**
**  SIDE EFFECTS:       none
**
**--
**/
PRIVATE boolean32 rpc__ip_is_local_network
#ifdef _DCE_PROTO_
(
    rpc_addr_p_t rpc_addr,
    unsigned32   *status
)
#else
(rpc_addr, status)
rpc_addr_p_t rpc_addr;
unsigned32   *status; 
#endif
{
    rpc_ip_addr_p_t         ip_addr = (rpc_ip_addr_p_t) rpc_addr;
    unsigned32              addr1;
    unsigned32              addr2;
    unsigned32              i;

    CODING_ERROR (status);

    if (rpc_addr == NULL)
    {
        *status = rpc_s_invalid_arg;
        return false;
    }

    *status = rpc_s_ok;

    if (local_ip_addr_vec == NULL)
    {
        /*
         * We should call rpc__ip_init_local_addr_vec() here. But, it
         * requires the mutex lock for local_ip_addr_vec. For now just return
         * false.
         */
        return false;
    }

    /*
     * Compare addresses.
     */
    for (i = 0; i < local_ip_addr_vec->num_elt; i++)
    {
        if (ip_addr->sa.sin_family != AF_INET)
        {
            continue;
        }

        addr1 = ip_addr->sa.sin_addr.s_addr & local_ip_addr_vec->elt[i].netmask;
        addr2 = local_ip_addr_vec->elt[i].addr & local_ip_addr_vec->elt[i].netmask;

        if (addr1 == addr2)
        {
            return true;
        }
    }

    return false;
}

/*
**++
**
**  ROUTINE NAME:       rpc__ip_is_local_addr
**
**  SCOPE:              PRIVATE - declared in ipnaf.h
**
**  DESCRIPTION:
**      
**  Return a boolean value to indicate if the given RPC address is the
**  the local IP address.
**
**
**  INPUTS:
**
**      rpc_addr        The address that forms the path of interest
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:                        
**
**      status          A value indicating the status of the routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      result          true => the address is local.
**                      false => not.
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE boolean32 rpc__ip_is_local_addr
#ifdef _DCE_PROTO_
(
    rpc_addr_p_t rpc_addr,
    unsigned32   *status
)
#else
(rpc_addr, status)
rpc_addr_p_t rpc_addr;
unsigned32   *status; 
#endif
{
    rpc_ip_addr_p_t         ip_addr = (rpc_ip_addr_p_t) rpc_addr;
    unsigned32              i;

    CODING_ERROR (status);

    if (rpc_addr == NULL)
    {
        *status = rpc_s_invalid_arg;
        return false;
    }

    *status = rpc_s_ok;

    if (local_ip_addr_vec == NULL)
    {
        /*
         * We should call rpc__ip_init_local_addr_vec() here. But, it
         * requires the mutex lock for local_ip_addr_vec. For now just return
         * false.
         */
        return false;
    }

    /*
     * Compare addresses.
     */
    for (i = 0; i < local_ip_addr_vec->num_elt; i++)
    {
        if (ip_addr->sa.sin_family != AF_INET)
        {
            continue;
        }

        if (ip_addr->sa.sin_addr.s_addr == local_ip_addr_vec->elt[i].addr)
        {
            return true;
        }
    }

    return false;
}
