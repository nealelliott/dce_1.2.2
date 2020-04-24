/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ipnaf_sys.c,v $
 * Revision 1.1.76.2  1996/02/18  00:01:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:10  marty]
 *
 * Revision 1.1.76.1  1995/12/08  00:16:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:05  root]
 * 
 * Revision 1.1.74.3  1994/05/27  15:35:50  tatsu_s
 * 	Merged up with DCE1_1.
 * 	[1994/05/20  20:53:56  tatsu_s]
 * 
 * 	DG multi-buffer fragments.
 * 	Added rpc__ip_is_*().
 * 	[1994/05/03  16:15:09  tatsu_s]
 * 
 * Revision 1.1.74.2  1994/01/22  22:49:50  cbrooks
 * 	Code Cleanup - CR 9797
 * 	[1994/01/22  21:22:27  cbrooks]
 * 
 * Revision 1.1.74.1  1994/01/21  22:33:27  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:59:45  cbrooks]
 * 
 * Revision 1.1.6.1  1993/09/29  15:09:57  tom
 * 	Bug 8369 - Cast argument to getsockname().
 * 	[1993/09/29  15:09:30  tom]
 * 
 * Revision 1.1.4.4  1993/02/03  16:11:04  paul
 * 	Modified enumerate_interfaces() to add point to point entries to end of
 * 	interface list.
 * 	[1993/02/03  16:10:16  paul]
 * 
 * Revision 1.1.4.3  1993/01/03  22:57:17  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:57:18  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  20:10:55  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:28:43  zeliff]
 * 
 * Revision 1.1.2.3  1992/07/07  21:27:41  rsalz
 * 	Close OT3718:  rpc__ip_get_broadcast didn't close socket when done.
 * 	[1992/07/01  15:21:29  rsalz]
 * 
 * Revision 1.1.2.2  1992/06/26  05:57:30  jim
 * 	Change to insure AIX 3.1.5 compatibility.
 * 	[1992/06/05  22:56:14  jim]
 * 
 * 	Changes for AIX 3.2 and BSD 4.4 sockets.
 * 	[1992/05/29  18:27:20  jim]
 * 
 * Revision 1.1  1992/01/19  03:05:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
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

typedef boolean (*enumerate_fn_p_t)
    _DCE_PROTOTYPE_ ((
        int                     /* in  */ desc,
        struct ifreq            /* in  */ *ifr,
        unsigned32              /* in  */ if_flags,
        struct sockaddr_in      /* in  */ *if_addr,
        rpc_ip_addr_p_t         /* out */ ip_addr,
        rpc_ip_addr_p_t         /* out */ netmask_addr
    ));

INTERNAL void enumerate_interfaces 
    _DCE_PROTOTYPE_ ((
        rpc_protseq_id_t        protseq_id,
        rpc_socket_t            desc,
        enumerate_fn_p_t        efun,
        rpc_addr_vector_p_t     *rpc_addr_vec,
        rpc_addr_vector_p_t     *netmask_addr_vec,
        unsigned32              *st
    ));

INTERNAL boolean get_addr
    _DCE_PROTOTYPE_ ((
        int                     desc,
        struct ifreq            *ifr,
        unsigned32              if_flags,
        struct sockaddr_in      *if_addr,
        rpc_ip_addr_p_t         ip_addr,
        rpc_ip_addr_p_t         netmask_addr
    ));                            

INTERNAL boolean get_broadcast_addr
    _DCE_PROTOTYPE_ ((
        int                     desc,
        struct ifreq            *ifr,
        unsigned32              if_flags,
        struct sockaddr_in      *if_addr,
        rpc_ip_addr_p_t         ip_addr,
        rpc_ip_addr_p_t         netmask_addr
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
**      netmask_addr_vec Returned vector of netmask RPC addresses.
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

#if _BSD == 44 
/*
 * Note that in the world of BSD 4.4, the struct ifreq's returned
 * from SIOCGIFCONF are *varying length*, but a minimum of 32 bytes.
 *
 * This has some interesting implications on how to parse the result
 * from SIOCGIFCONF.
 */
#endif

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
    char                    buf[1024], *cp, *cplim;
    struct ifconf           ifc;
    struct ifreq            *ifr;
    struct ifreq            ifreq;
    short                   if_flags;
    struct sockaddr_in      if_addr;
    int                     i, cnt, nptp=0, stop;
    int                     err;
    rpc_ip_addr_p_t         netmask_addr = NULL;
    CODING_ERROR (status);

    /*
     * Get the list of network interfaces.
     */
    ifc.ifc_len = sizeof (buf);
    ifc.ifc_buf = buf;

ifconf_again:
    if ((err = ioctl (desc, (int) SIOCGIFCONF, (caddr_t) &ifc)) < 0) 
    {
        if (err == EINTR)
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

#if _BSD == 44
    /*
     * This will be the maximum number of interfaces 
     */
#endif

    n_ifs = ifc.ifc_len / sizeof (struct ifreq);

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

    cplim = buf + ifc.ifc_len;

#define max(x,y) ((x) > (y) ? (x) : (y))
#ifdef AIX32
#define size(p) max((p).sa_len, sizeof(p))
#else
#define size(p) sizeof(p)
#endif

    for (cp = buf; 
         cp < cplim; 
         cp += sizeof(ifr->ifr_name) + size(ifr->ifr_addr))
    {
        /*
         * Get the interface's flags.  If the flags say that the interface
         * is not up or is the loopback interface, skip it.  Do the
         * SIOCGIFFLAGS on a copy of the ifr so we don't lose the original
         * contents of the ifr.  (ifr's are unions that hold only one
         * of the interesting interface attributes [address, flags, etc.]
         * at a time.)
         */
        ifr = (struct ifreq *) cp;
        ifreq = *ifr;
ifflags_again:        
        if ((err = ioctl(desc, SIOCGIFFLAGS, &ifreq)) < 0)
        {
            if (err == EINTR)
            {
                goto ifflags_again;
            }

            *status = -3;
            goto FREE_IT;
        }

        if_flags = ifreq.ifr_flags;     /* Copy out the flags */

        /*
         * Get the addressing stuff for this interface.
         */
#ifdef NO_SIOCGIFADDR
        /*
         * Note that some systems do not return the address for the
         * interface given.  However the ifr array elts contained in
         * the ifc block returned from the SIOCGIFCONF ioctl above already
         * contains the correct addresses. So these system should define
         * NO_SIOCGIFADDR in their platform specific include file.
         */
        if_addr = * (struct sockaddr_in *) &ifr->ifr_addr;
#else
        /*
         * Do the SIOCGIFADDR on a copy of the ifr.  See above.
         */
        ifreq = *ifr;
ifaddr_again:
        if ((err = ioctl(desc, SIOCGIFADDR, &ifreq)) < 0)
        {
            if (err == EINTR)
            {
                goto ifaddr_again;
            }

            *status = -4;
            goto FREE_IT;
        }
        if_addr = * (struct sockaddr_in *) &ifreq.ifr_addr;
#endif

        /*
         * If this isn't an Internet-family address, ignore it.
         */
        if (if_addr.sin_family != AF_INET)
        {
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

/* CMVC 3687 Start */

	/*
	** As interfaces on the machine are found they are put into an array
	** which has n_ifs-1 elements (n_ifs defined above).  As a result of
	** this defect we want to put the point to point (p2p) interfaces at
	** the end of the array.  While we are looping through the interfaces
	** we will just put the p2p's at the end of the array and later (after
	** we're done looping) we'll shuffle the p2p interfaces into empty
	** slots in the middle of the array.  Each time we encounter a p2p
	** interface we increment the np2p variable which (when subtracted
	** from n_ifs-1) marks the last available slot in the array.
	*/

        if (if_flags & IFF_POINTOPOINT) {
          (*rpc_addr_vec)->addrs[n_ifs-1-nptp++] = (rpc_addr_p_t) ip_addr;
          if (netmask_addr_vec != NULL && netmask_addr != NULL)
              (*netmask_addr_vec)->addrs[n_ifs-1-nptp++]
                  = (rpc_addr_p_t) netmask_addr;
        } else {
          (*rpc_addr_vec)->addrs[(*rpc_addr_vec)->len++]=(rpc_addr_p_t) ip_addr;
          if (netmask_addr_vec != NULL && netmask_addr != NULL)
              (*netmask_addr_vec)->addrs[(*netmask_addr_vec)->len++]
                  = (rpc_addr_p_t) netmask_addr;
        }
    }

    /*
    ** Shuffle p2p interfaces at the end of the array into empty slots in the
    ** middle, if there are any.
    */

    cnt = (*rpc_addr_vec)->len;
    if ((n_ifs - (cnt + nptp)) < nptp)
      stop = n_ifs - (cnt + nptp);
    else
      stop = nptp;
    stop += cnt;

    for (i = cnt, nptp = n_ifs - 1; i < stop; i++, nptp--) {
      (*rpc_addr_vec)->addrs[i] = (*rpc_addr_vec)->addrs[nptp];
      (*rpc_addr_vec)->addrs[nptp] = NULL;
      if (netmask_addr_vec != NULL)
      {
          (*netmask_addr_vec)->addrs[i] = (*netmask_addr_vec)->addrs[nptp];
          (*netmask_addr_vec)->addrs[nptp] = NULL;
      }
    }

/* CMVC 3687 End */

/*
** Prior to CMVC 3687
**      (*rpc_addr_vec)->addrs[(*rpc_addr_vec)->len++] = (rpc_addr_p_t) ip_addr;
**  }
*/

#undef max
#undef size

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
        /*
         * If the interface's flags say the this is a loopback interface
         * or that it isn't up, ignore it.
         */
        if ((if_flags & IFF_LOOPBACK) != 0 || (if_flags & IFF_UP) == 0)
        {
            return (false);
        }

        ip_addr->sa = *if_addr;
#if _BSD == 44
        ip_addr->sa.sin_len = 0;
#endif
        return (true);
    }
    else
    {
        /*
         * If the interface's flags say that this isn't up, ignore it.
         */

        if ((if_flags & IFF_UP) == 0)
        {
            return (false);
        }
        ip_addr->sa = *if_addr;
#if _BSD == 44
        ip_addr->sa.sin_len = 0;
#endif

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

        netmask_addr->sa = * (struct sockaddr_in *) &ifreq.ifr_addr;
#if _BSD == 44
        netmask_addr->sa.sin_len = 0;
#endif
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
    loc_ip_addr.len = sizeof (rpc_ip_addr_t);

    if (getsockname (desc, (struct sockaddr *)&loc_ip_addr.sa, (int *)&loc_ip_addr.len) < 0)
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
    int                     err;

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
    if ((err = ioctl(desc, (int) SIOCGIFBRDADDR, &ifreq)) == -1) 
    {
        if (err == EINTR)
        {
            goto ifbrdaddr_again;
        }

        return (false);
    }

    ip_addr->sa = * (struct sockaddr_in *) &ifreq.ifr_broadaddr;
#if _BSD == 44
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
