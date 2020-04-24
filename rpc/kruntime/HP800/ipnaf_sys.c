/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: ipnaf_sys.c,v $
 * Revision 1.1.57.2  1996/02/17  23:59:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:25  marty]
 *
 * Revision 1.1.57.1  1995/12/08  00:13:57  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/08/21  20:26 UTC  maunsell_c
 * 	walnut creek port
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/1  1995/07/13  22:20 UTC  maunsell_c
 * 	walnut creek port
 * 
 * 	HP revision /main/HPDCE02/maunsell_WC_port/1  1995/07/13  22:20 UTC  maunsell_c
 * 	aslco
 * 
 * 	HP revision /main/HPDCE02/3  1994/10/28  15:20 UTC  kissel
 * 	Fix MP locking for 9.x.  Of course it is different from 10.0.
 * 
 * 	HP revision /main/HPDCE02/2  1994/10/26  17:55 UTC  kissel
 * 	Merge in MP-safe changes from kissel_mpsafe_bug2 (but due to a
 * 	mistake, the changes really come from .../HPDCE01/2).
 * 
 * 	HP revision /main/HPDCE01/2  1994/10/20  20:56 UTC  kissel
 * 	Add locking around the call to ifioctl() since it expects it for MP-safe operation.
 * 
 * 	HP revision /main/HPDCE02/1  1994/09/19  13:25 UTC  mort
 * 	change get_if from PRIVATE (which isn't) to INTERNAL
 * 	[1995/12/07  23:55:39  root]
 * 
 * Revision 1.1.55.4  1994/06/10  20:54:06  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:59:52  devsrc]
 * 
 * Revision 1.1.55.3  1994/05/27  15:35:38  tatsu_s
 * 	Merged up with DCE1_1.
 * 	[1994/05/20  20:52:54  tatsu_s]
 * 
 * 	DG multi-buffer fragments.
 * 	Added rpc__ip_is_*().
 * 	[1994/04/29  18:54:26  tatsu_s]
 * 
 * Revision 1.1.55.2  1994/02/02  21:48:53  cbrooks
 * 	OT9855 code cleanup breaks KRPC
 * 	[1994/02/02  20:59:57  cbrooks]
 * 
 * Revision 1.1.55.1  1994/01/21  22:31:20  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:32:59  cbrooks]
 * 
 * Revision 1.1.4.2  1993/06/10  19:21:58  sommerfeld
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:03:33  kissel]
 * 
 * 	Add kernel support for rpc__ip_desc_inq_addr() to enumerate interfaces.
 * 	[1992/10/16  22:11:43  toml]
 * 
 * 	06/16/92  tmm  Created from COSL drop
 * 	[1992/06/18  18:30:41  tmm]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  NAME
**
**      ipnaf_osc
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
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
**
**  NAME
**
**      ipnaf_sys
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
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**  27-nov-90 nacey     rpc__socket_inq_addr() -> rpc__socket_inq_endpoint()
**  30-jul-90 nacey     cribbed from ip/ipnaf_bsd.c
**                      and old ip/ipnaf.c
**  17-jul-90 mishkin   initial code
**
**  %a%private_end  
**
*/

#include <commonp.h>
#include <com.h>
#include <comnaf.h>
#include <ipnaf.h>


#include <net/if.h>
#include <sys/ioctl.h>

/*
** This module is in the HP800 directory, so we don't need to test for __OSF__.
** However, due to the new locking code below, we need these header files to
** define SOCKET_{UN}LOCK.
*/
#if	defined(_KERNEL)
#include <sys/mbuf.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/domain.h>
#include <sys/protosw.h>
#if __hpux1010p
#include <sys/sem_alpha.h>
#endif
#if ! __hpux1000p
#include <net/netmp.h>
#endif /* __hpux1000p */
#endif
#define NO_SIOCGIFADDR /* OSF kernel does have one, but using it introduces a bug */

#ifdef _SOCKADDR_LEN
/*
 * Note that in the world of BSD 4.4, the struct ifreq's returned
 * from SIOCGIFCONF are *varying length*, but a minimum of 32 bytes.
 *
 * This has some interesting implications on how to parse the result
 * from SIOCGIFCONF.
 */
#endif


/***********************************************************************
 *
 *  Internal prototypes and typedefs.
 */

typedef boolean (*enumerate_fn_p_t) _DCE_PROTOTYPE_ ((
        rpc_socket_t            /* in  */  /*desc*/,
        struct ifreq            /* in  */ * /*ifr*/,
        unsigned32              /* in  */  /*if_flags*/,
        struct sockaddr         /* in  */ * /*if_addr*/,
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
        rpc_socket_t             /*desc*/,
        struct ifreq            * /*ifr*/,
        unsigned32               /*if_flags*/,
        struct sockaddr         * /*if_addr*/,
        rpc_ip_addr_p_t          /*ip_addr*/,
        rpc_ip_addr_p_t          /*netmask_addr*/
    ));                            

INTERNAL boolean get_broadcast_addr _DCE_PROTOTYPE_ ((
        int                      /*desc*/,
        struct ifreq            * /*ifr*/,
        unsigned32               /*if_flags*/,
        struct sockaddr         * /*if_addr*/,
        rpc_ip_addr_p_t          /*ip_addr*/,
        rpc_ip_addr_p_t          /*netmask_addr*/
    ));

/*
 * Since we are in kernel, probably we should lookup them everytime
 * called instead of caching?
 */
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


#ifdef	_KERNEL

/*
   This is basically a copy of /afs/ch/project/hpux/build/hpux8.x/base/kern_src/net/if.c
   (ifconf) with the copyout's changed to b_c_o_p_y's.
*/
INTERNAL int get_if
#ifdef _DCE_PROTO_
(
    struct ifconf *ifc
)
#else
(ifc)
struct ifconf *ifc;
#endif
{
   register struct ifnet *ifp;
   register struct ifaddr *ifa;
   register char *cp, *ep;
   struct ifreq ifr, *ifrp;
   int ifrlen;
   int space = ifc->ifc_len;
   int error = 0;
   
   ifrp = ifc->ifc_req;
   ep = ifr.ifr_name + sizeof (ifr.ifr_name) - 2;

   for(ifp = ifnet; space > sizeof (ifr) && ifp; ifp = ifp->if_next)
   {
      bcopy(ifp->if_name, ifr.ifr_name, sizeof (ifr.ifr_name) - 2);

      for (cp = ifr.ifr_name; cp < ep && *cp; cp++)
         ;
      *cp++ = '0' + ifp->if_unit;
      *cp = '\0';
      if ((ifa = ifp->if_addrlist) == 0)
      {
#ifdef	_SOCKADDR_LEN
	 continue;
#else
	 /* I don't understand how this will help much anyway. XXX */
         /*b_z_e_r_o((caddr_t)&ifr.ifr_addr, sizeof(ifr.ifr_addr));*/
         memset((caddr_t)&ifr.ifr_addr, 0, sizeof(ifr.ifr_addr));

         bcopy((caddr_t)&ifr, (caddr_t)ifrp, sizeof (ifr));


         space -= sizeof (ifr), ifrp++;
#endif
      } else
         for ( ; space > sizeof (ifr) && ifa; ifa = ifa->ifa_next)
         {
#ifdef	_SOCKADDR_LEN
	    ifrlen = sizeof(struct ifreq) - sizeof(struct sockaddr) + ifa->ifa_addr->sa_len;
	    bcopy((caddr_t)ifa->ifa_addr, (caddr_t)&ifr.ifr_addr, ifa->ifa_addr->sa_len);

#else
	    ifrlen = sizeof(ifr);
            ifr.ifr_addr = ifa->ifa_addr;
#endif
            bcopy((caddr_t)&ifr, (caddr_t)ifrp, ifrlen);

            space -= ifrlen;
	    ifrp = (struct ifreq *)((caddr_t) ifrp + ifrlen);
         }
   }
   ifc->ifc_len -= space;
   return (error);
}
#endif	/* _KERNEL */

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

INTERNAL void enumerate_interfaces
    (protseq_id, desc, efun, rpc_addr_vec, netmask_addr_vec, status)

rpc_protseq_id_t        protseq_id;
rpc_socket_t            desc;
enumerate_fn_p_t        efun;
rpc_addr_vector_p_t     *rpc_addr_vec;
rpc_addr_vector_p_t     *netmask_addr_vec;
unsigned32              *status;

{
    rpc_ip_addr_p_t         ip_addr;
    int                     i;
    unsigned char           buf[1024]; /* XXX risky for _KERNEL */
    struct ifconf           ifc;
    struct ifreq            *ifr, *last_ifr;
    struct ifreq            ifreq;
    short                   if_flags;
    struct sockaddr         if_addr;
    rpc_ip_addr_p_t         netmask_addr = NULL;
#ifdef	_KERNEL
    int                     errno;
    int			    oldlevel;
    sv_sema_t		    savestate;
#endif	/* _KERNEL */

#ifdef _SOCKADDR_LEN
    int                     ifrlen;
#else
#define ifrlen sizeof(struct ifreq)
#endif	/* _SOCKADDR_LEN */

    CODING_ERROR (status);

    /*
     * Get the list of network interfaces.
     */
    ifc.ifc_len = sizeof (buf);
    ifc.ifc_buf = (caddr_t) buf;
    
ifconf_again:
#ifdef	_KERNEL
    /*
     * for OSF, would be great to do
     * if (errno = osf_ifioctl(desc, OSIOCGIFCONF, (caddr_t) &ifc)),
     * but that would do copyout()'s to user space, so ...
     */
    if (errno = get_if(&ifc))
#else
    if (ioctl (desc, (int) SIOCGIFCONF, (caddr_t) &ifc) < 0)
#endif	/* _KERNEL */
    {
        if (errno == EINTR)
        {
            goto ifconf_again;
        }
        *status = -2;   /* !!! */
        return;
    }

    RPC_DBG_PRINTF(rpc_e_dbg_general, 10,
		   ("%d bytes of ifreqs, ifreq is %d bytes\n", ifc.ifc_len, sizeof(struct ifreq)));

#ifdef MAX_DEBUG
    if (RPC_DBG2(rpc_e_dbg_general, 15))
    {
        int i;
        for (i=0; i<ifc.ifc_len; i++) {
            if ((i % 32) == 0)
                printf("\n%4x: ", i);
            printf("%02x ", buf[i]);
        }
        printf("\n");
    }
#endif

    /*
     * Figure out how many interfaces there must be and allocate an  
     * RPC address vector with the appropriate number of elements.
     * (We may ask for a few too many in case some of the interfaces
     * are uninteresting.)
     */
    RPC_MEM_ALLOC (
        *rpc_addr_vec,
        rpc_addr_vector_p_t,
        (sizeof **rpc_addr_vec) +
		   (((ifc.ifc_len/sizeof (struct ifreq)) - 1) * (sizeof (rpc_addr_p_t))),
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
            (sizeof **netmask_addr_vec) +
        (((ifc.ifc_len/sizeof (struct ifreq)) - 1) * (sizeof (rpc_addr_p_t))),
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
    last_ifr = (struct ifreq *) (ifc.ifc_buf + ifc.ifc_len);

    for (ifr = ifc.ifc_req; ifr < last_ifr; ifr = (struct ifreq *)(((caddr_t)ifr) + ifrlen))
    {
#ifdef _SOCKADDR_LEN
        ifrlen = sizeof (struct ifreq) - sizeof(struct sockaddr) + ifr->ifr_addr.sa_len ;
#endif
        RPC_DBG_PRINTF(rpc_e_dbg_general, 10, ("interface %s\n", ifr->ifr_name));
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
#ifdef _KERNEL
	/*
	** I have not found any other callers of ifioctl() in kern_bld/net/if.c,
	** with this operation in the kernel, so I can't copy anything.
	** However, it contains a LOCK_ASSERT macro that requires the socket to
	** be locked by its caller, so I had better do it.  The sad things is,
	** it does not look like this particular operation even uses the socket;
	** it uses the other args to do its job, so it does not really need the
	** socket to be locked.  This is the same kind of code that was added to
	** the rpc__socket_inq_endpoint() entry in comsoc_sys.c in this
	** directory.  The locking is different on 9.x and 10.0, so we need to
	** make a distinction here.  In comsoc_sys.c we hid this in macros, but
	** this is the only place we do locking in this file, so we'll just do
	** it here.
	*/
#if __hpux1000p
	NET_SPLNET(oldlevel);	/* This only applies to uniprocessors. */
	SOCKET_LOCK(desc);	/* This only applies to multiprocessors. */
#else
	NETMP_GO_EXCLUSIVE(oldlevel, savestate);
#endif /* __hpux1000p */
	errno = ifioctl(desc, SIOCGIFFLAGS, (caddr_t)&ifreq);
#if __hpux1000p
	SOCKET_UNLOCK(desc);
	NET_SPLX(oldlevel);
#else
	NETMP_GO_UNEXCLUSIVE(oldlevel, savestate);
#endif /* __hpux1000p */

        if (errno)
#else
	if (ioctl(desc, SIOCGIFFLAGS, &ifreq) < 0)
#endif	/* _KERNEL */
        {
            RPC_DBG_PRINTF(rpc_e_dbg_general, 10,
                ("SIOCGIFFLAGS returned errno %d\n", errno));
            if (errno == EINTR)
            {
                goto ifflags_again;
            }
            continue;
        }
        if_flags = ifreq.ifr_flags;     /* Copy out the flags */
        RPC_DBG_PRINTF(rpc_e_dbg_general, 10, ("flags are %x\n", if_flags));

	/* Ignore interfaces which are not 'up'. */
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
	if_addr = ifr->ifr_addr;
#else
        /*
         * Do the SIOCGIFADDR on a copy of the ifr.  See above.
         */
        memcpy(&ifreq, ifr, sizeof(ifreq));
ifaddr_again:

#ifdef _KERNEL
#ifdef	__OSF__
        if (errno = osf_ifioctl(desc, SIOCGIFADDR, (caddr_t)&ifreq))
#else
#error	"Requires porting to your OS"
#endif /* _KERNEL */
#else
	if (ioctl(desc, SIOCGIFADDR, &ifreq) < 0)
#endif	/* _KERNEL */
        {
            RPC_DBG_PRINTF(rpc_e_dbg_general, 10,
                ("SIOCGIFADDR returned errno %d\n", errno));
            if (errno == EINTR)
            {
                goto ifaddr_again;
            }

            /* XXX probably could just continue here XXX */
	    *status = -4;
            goto FREE_IT;
        }

        memcpy (&if_addr, &ifreq.ifr_addr, sizeof(struct sockaddr));

#endif	/* NO_SIOCGIFADDR */
 
        /*
         * If this isn't an Internet-family address, ignore it.
         */
        if (if_addr.sa_family != AF_INET)
        {
            RPC_DBG_PRINTF(rpc_e_dbg_general, 10, ("AF %d not INET\n", if_addr.sa_family));
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
        if (!(*efun) (desc, ifr, if_flags, &if_addr, ip_addr, netmask_addr))
        {
            RPC_MEM_FREE (ip_addr, RPC_C_MEM_RPC_ADDR);
            if (netmask_addr != NULL)
                RPC_MEM_FREE (netmask_addr, RPC_C_MEM_RPC_ADDR);
            continue;
        }

        (*rpc_addr_vec)->addrs[(*rpc_addr_vec)->len++] = (rpc_addr_p_t) ip_addr;
        if (netmask_addr_vec != NULL && netmask_addr != NULL)
            (*netmask_addr_vec)->addrs[(*netmask_addr_vec)->len++]
                = (rpc_addr_p_t) netmask_addr;
    }

    if ((*rpc_addr_vec)->len == 0) 
    {
        *status = -5;   /* !!! */
        goto FREE_IT;
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

INTERNAL boolean get_addr (desc, ifr, if_flags, if_addr, ip_addr, netmask_addr)

rpc_socket_t            desc;
struct ifreq            *ifr;
unsigned32              if_flags;
struct sockaddr         *if_addr;
rpc_ip_addr_p_t         ip_addr;
rpc_ip_addr_p_t         netmask_addr;

{
    struct ifreq            ifreq;
#ifdef	_KERNEL
    int                     errno;
#endif	/* _KERNEL */

    if (netmask_addr == NULL)
    {
	/* Ignore loopback. */
	if ((if_flags & IFF_LOOPBACK) != 0)
            return(false);
#ifdef	_SOCKADDR_LEN
        /* If we have obtained a bsd4.4-style sockaddr, convert it     *
         * now to a bsd4.3-style sockaddr, which is what RPC deals in. */
        ((struct osockaddr *)if_addr)->sa_family = if_addr->sa_family;
#endif
        memcpy (&ip_addr->sa, if_addr, sizeof(struct sockaddr_in));
        return (true);
    }
    else
    {
#ifdef	_SOCKADDR_LEN
        ((struct osockaddr *)if_addr)->sa_family = if_addr->sa_family;
#endif
        memcpy (&ip_addr->sa, if_addr, sizeof(struct sockaddr_in));

        /*
         * Inquire the interface's netmask address.
         */
        memcpy(&ifreq, ifr, sizeof(ifreq));
    ifnetaddr_again:
#ifdef _KERNEL
        if (errno = ifioctl(desc, SIOCGIFNETMASK, (caddr_t)&ifreq))
#else
        if (ioctl(desc, SIOCGIFNETMASK, &ifreq) < 0)
#endif	/* _KERNEL */
        {
            RPC_DBG_PRINTF(rpc_e_dbg_general, 10,
                ("SIOCGIFNETMASK returned errno %d\n", errno));
            if (errno == EINTR)
            {
                goto ifnetaddr_again;
            }
            return (false);
        }

#ifdef	_SOCKADDR_LEN
        ((struct osockaddr *)&ifreq.ifr_addr)->sa_family = ifreq.ifr_addr.sa_family;
#endif
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
    loc_ip_addr.len = sizeof (rpc_ip_addr_t);

#ifdef _KERNEL
    if (rpc__socket_inq_endpoint (desc, (rpc_addr_p_t)&loc_ip_addr) < 0)
#else
    if (getsockname (desc, &loc_ip_addr.sa, &loc_ip_addr.len) < 0)
#endif /* _KERNEL */
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
        /*
         * allocate memory for the new RPC address
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
            return;
        }

#ifndef	IPNAF_BSD
        /*
         * insert individual parameters into RPC addr
         */
        (ip_addr)->rpc_protseq_id = protseq_id;
        (ip_addr)->len = sizeof (struct sockaddr_in);

        /*
         * get the socket info into the RPC addr
         */
        if (!RPC_SOCKET_IS_ERR(rpc__socket_inq_endpoint (desc, (rpc_addr_p_t) ip_addr)))
        {
            *status = rpc_s_ok;
        }
        else
        {
            RPC_MEM_FREE (ip_addr, RPC_C_MEM_RPC_ADDR);
            *status = rpc_s_cant_inq_socket;
            return;
        }
#endif

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

#ifdef	IPNAF_BSD
        ip_addr->rpc_protseq_id = protseq_id;
        ip_addr->len            = sizeof (struct sockaddr_in);
        ip_addr->sa             = loc_ip_addr.sa;
#endif
	
        (*rpc_addr_vec)->len = 1;
        (*rpc_addr_vec)->addrs[0] = (rpc_addr_p_t) ip_addr;

        *status = rpc_s_ok;
        return;
    }
}


#ifdef _KERNEL
/*
**++
**
**  ROUTINE NAME:       rpc__ip_get_broadcast
**
**  SCOPE:              PRIVATE - EPV declared in ipnaf.h
**
**  DESCRIPTION:
**      
**
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
**      rpc_addrs
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
    rpc_naf_id_t        naf_id,
    rpc_protseq_id_t    rpc_protseq_id,
    rpc_addr_vector_p_t *rpc_addrs,
    unsigned32          *status 
)
#else
(naf_id, rpc_protseq_id, rpc_addrs, status)
rpc_naf_id_t        naf_id;
rpc_protseq_id_t    rpc_protseq_id;
rpc_addr_vector_p_t *rpc_addrs;
unsigned32          *status; 
#endif
{
    *rpc_addrs = NULL; /* Kernel version doesn't do a lot currently */
    *status = -1 /* !!! error_status_ok */;
}
#else	/* _KERNEL */

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
    struct sockaddr        *if_addr,
    rpc_ip_addr_p_t         ip_addr
    rpc_ip_addr_p_t         netmask_addr;
)
#else
(desc, ifr, if_flags, if_addr, ip_addr, netmask_addr)
int                     desc;
struct ifreq            *ifr;
unsigned32              if_flags;
struct sockaddr        *if_addr;
rpc_ip_addr_p_t         ip_addr;
rpc_ip_addr_p_t         netmask_addr;
#endif
{
    struct ifreq            ifreq;

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
    if (ioctl(desc, (int) SIOCGIFBRDADDR, &ifreq) < 0) 
    {
        if (errno == EINTR)
        {
            goto ifbrdaddr_again;
        }

        return (false);
    }

    memcpy (&ip_addr->sa, &ifreq.ifr_broadaddr, sizeof(struct sockaddr_in));
#ifdef _SOCKADDR_LEN
    (struct osockaddr *)(&ip_addr->sa)->sa_family = ip_addr->sa.sa_family;
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
}
#endif /* _KERNEL */

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
    rpc_socket_t            desc;
    unsigned32              lstatus;
    unsigned32              i;
#ifdef	_KERNEL
    int                     errno;
#endif	/* _KERNEL */
    rpc_addr_vector_p_t     rpc_addr_vec = NULL;
    rpc_addr_vector_p_t     netmask_addr_vec = NULL;

    CODING_ERROR (status);

    /*
     * Open a socket to pass to "enumerate_interface".
     */
    errno = socreate(AF_INET, &desc, SOCK_DGRAM, 0
#ifndef	HPUX80
                     ,0 /* flags */      
#endif
                     );

    if (errno) 
    {
        *status = rpc_s_cant_create_socket;   /* !!! */
        return;
    }

    enumerate_interfaces
        (RPC_C_PROTSEQ_ID_NCADG_IP_UDP, desc, get_addr,
         &rpc_addr_vec, &netmask_addr_vec, status);
    errno = soclose(desc);

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
            RPC_DBG_PRINTF(rpc_e_dbg_general, 10,
("(rpc__ip_init_local_addr_vec) local network [%d.%d.%d.%d] netmask [%d.%d.%d.%d]\n",
                            UC(p[0]), UC(p[1]), UC(p[2]), UC(p[3]),
                            UC(mp[0]), UC(mp[1]), UC(mp[2]), UC(mp[3])));
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
