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
 * Revision 1.1.16.2  1996/02/18  00:00:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:33  marty]
 *
 * Revision 1.1.16.1  1995/12/08  00:14:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:54  root]
 * 
 * Revision 1.1.14.2  1994/05/27  15:35:43  tatsu_s
 * 	Merged up with DCE1_1.
 * 	[1994/05/20  20:53:07  tatsu_s]
 * 
 * 	DG multi-buffer fragments.
 * 	Added rpc__ip_is_*().
 * 	[1994/05/03  18:32:52  tatsu_s]
 * 
 * Revision 1.1.14.1  1994/01/21  22:31:39  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:41:19  cbrooks]
 * 
 * Revision 1.1.12.1  1993/09/01  14:38:14  delgado
 * 	DFS 486 port - fix code to better support variable length sockaddrs
 * 	[1993/09/01  14:31:53  delgado]
 * 
 * Revision 1.1.7.4  1993/01/03  22:35:25  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:51:01  bbelch]
 * 
 * Revision 1.1.7.3  1992/12/23  19:37:25  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:49:21  zeliff]
 * 
 * Revision 1.1.7.2  1992/10/13  17:24:27  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:14:04  marty]
 * 
 * Revision 1.1.5.2  1992/09/29  22:46:41  marty
 * 	OSC1.1 upgrade
 * 	[1992/09/29  22:38:48  marty]
 * 
 * Revision 1.1.3.2  1992/09/29  20:10:08  garyf
 * 	fixes for port to OSF/1 R1.1
 * 
 * Revision 1.1.2.3  1992/05/20  18:31:03  garyf
 * 	change to use correct OSF1 compilation flag
 * 	[1992/05/20  18:24:31  garyf]
 * 
 * Revision 1.1.2.2  1992/04/27  18:13:24  toml
 * 	Merge PMAX user and RIOS kernel space versions.
 * 	Add kernel support for rpc__ip_desc_inq_addr() to enumerate interfaces.
 * 	[1992/04/27  18:09:22  toml]
 * 
 * Revision 1.1  1992/01/19  03:15:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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


#if	defined(_KERNEL) && defined(__OSF__)
#include <sys/mbuf.h>
#include <sys/socket.h>
#include <sys/socketvar.h>
#include <sys/domain.h>
#include <sys/protosw.h>
#define _SOCKADDR_LEN  /* OSF1 kernel deals with bsd4.4-style, variable length sockaddrs */
#define NO_SIOCGIFADDR /* OSF kernel does have one, but using it introduces a bug */
#endif

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

typedef boolean (*enumerate_fn_p_t)
    PROTOTYPE ((
        int                     /* in  */ desc,
        struct ifreq            /* in  */ *ifr,
        unsigned32              /* in  */ if_flags,
        struct sockaddr         /* in  */ *if_addr,
        rpc_ip_addr_p_t         /* out */ ip_addr,
        rpc_ip_addr_p_t         /* out */ netmask_addr
    ));

INTERNAL void enumerate_interfaces 
    PROTOTYPE ((
        rpc_protseq_id_t        protseq_id,
        rpc_socket_t            desc,
        enumerate_fn_p_t        efun,
        rpc_addr_vector_p_t     *rpc_addr_vec,
        rpc_addr_vector_p_t     *netmask_addr_vec,
        unsigned32              *st
    ));

INTERNAL boolean get_addr
    PROTOTYPE ((
        int                     desc,
        struct ifreq            *ifr,
        unsigned32              if_flags,
        struct sockaddr         *if_addr,
        rpc_ip_addr_p_t         ip_addr,
        rpc_ip_addr_p_t         netmask_addr
    ));                            

INTERNAL boolean get_broadcast_addr
    PROTOTYPE ((
        int                     desc,
        struct ifreq            *ifr,
        unsigned32              if_flags,
        struct sockaddr         *if_addr,
        rpc_ip_addr_p_t         ip_addr,
        rpc_ip_addr_p_t         netmask_addr
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


/*
 * Sockaddrs are varying length and at most 256 bytes.
 * a struct ifreq may not be large enough to hold the
 * new sockaddrs so that's what this union is for.
 */
typedef union{
      char buf[256+IFNAMSIZ];
      struct ifreq u_ifreq;
} ifr_buf;

#define ifrb_name  u_ifreq.ifr_name  
#define ifrb_addr  u_ifreq.ifr_addr
#define ifrb_flags u_ifreq.ifr_flags

typedef union{
      char buf[256];
      struct sockaddr u_sa;
}sockaddr_u;

#define su_len  u_sa.sa_len
#define su_addr u_sa.sa_addr
#define su_family u_sa.sa_family

#ifdef	_KERNEL
/*
 * We are supposed to return to the caller a buffer filled with information
 * for all the interfaces and all the addresses associated with each interface.
 *
 * [ifname1 (16 bytes)] [addr1]
 * [ifname1 (16 bytes)] [addr n]
 * [ifname2 (16 bytes)] [addr1]
 * [ifname2 (16 bytes)] [addr n]
 * ...
 * The [addr] are actually variable length sockaddrs, which can actually
 * be larger than a struct sockaddr, hence we have had to declare the
 * above unions to accommodate this.
 */

PRIVATE int get_if(ifc, ifcount)
struct ifconf *ifc;
unsigned long *ifcount;
{
   register struct ifnet *ifp;
   register struct ifaddr *ifa;
   register char *cp, *ep;
   ifr_buf ifr; 
   struct ifreq *ifrp;
   int ifrlen;
   int space = ifc->ifc_len;
   int error = 0;

   (*ifcount) = 0;   
   ifrp = ifc->ifc_req;
   ep = ifr.ifrb_name + sizeof (ifr.ifrb_name) - 2;

   (*ifcount)=0;
   for(ifp = ifnet; space > sizeof (ifr) && ifp; ifp = ifp->if_next)
   {
      /*b_copy(ifp->if_name, ifr.ifrb_name, sizeof (ifr.ifrb_name) - 2);*/
      memmove( ifr.ifrb_name, ifp->if_name, sizeof (ifr.ifrb_name) - 2);

      for (cp = ifr.ifrb_name; cp < ep && *cp; cp++)
         ;
      *cp++ = '0' + ifp->if_unit;
      *cp = '\0';
      if ((ifa = ifp->if_addrlist) == 0)
      {
#ifdef	_SOCKADDR_LEN
	 continue;
#else
	 /*+I don't understand how this will help much anyway. XXX */

         /*b_zero(caddr_t)&ifr.ifrb_addr, sizeof(ifr.ifrb_addr));*/
         memset((caddr_t)&ifr.ifrb_addr, 0, sizeof(ifr.ifrb_addr));

         /*b_copy((caddr_t)&ifr, (caddr_t)ifrp, sizeof (ifr));*/
         memmove((caddr_t)ifrp, (caddr_t)&ifr, sizeof (ifr));

         space -= sizeof (ifr), ifrp++;
#endif
      } else
         for ( ; space > sizeof (ifr) && ifa; ifa = ifa->ifa_next)
         {
#ifdef	_SOCKADDR_LEN
	    ifrlen = sizeof(struct ifreq) - sizeof(struct sockaddr) + ifa->ifa_addr->sa_len;
	    /*b_copy((caddr_t)ifa->ifa_addr, (caddr_t)&(ifr.ifrb_addr), ifa->ifa_addr->sa_len);*/
	    memmove((caddr_t)&(ifr.ifrb_addr), (caddr_t)ifa->ifa_addr, ifa->ifa_addr->sa_len);*/
#else
	    ifrlen = sizeof(ifr);
            ifr.ifr_addr = ifa->ifa_addr;
#endif
            /*b_copy((caddr_t)&ifr, (caddr_t)ifrp, ifrlen);*/

            memmove( (caddr_t)ifrp, (caddr_t)&ifr, ifrlen);

            space -= ifrlen;
	    ifrp = (struct ifreq *)((caddr_t)ifrp + ifrlen);
/*	    (caddr_t) ifrp += ifrlen; */
            (*ifcount)++;
         }
   }
   ifc->ifc_len -= space;
   return (error);
}


#ifdef	__OSF__
int
osf_ifioctl(so, cmd, data)
struct socket *so;
int           cmd;
caddr_t      data;
{
	int err;
	DOMAIN_FUNNEL_DECL(f);

	DOMAIN_FUNNEL(sodomain(so), f);
	SOCKET_LOCK(so);

	err = ifioctl(so, cmd, data);

	SOCKET_UNLOCK(so);
	DOMAIN_UNFUNNEL(f);
	return (err);
}
#endif	/* __OSF__ */

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
    unsigned char           *buf;
    struct ifconf           ifc;
    struct ifreq            *ifr, *last_ifr;
    ifr_buf                 ifreqb;
    short                   if_flags;
    long                    if_count;
    sockaddr_u              if_addr;
    rpc_ip_addr_p_t         netmask_addr = NULL;
#ifdef	_KERNEL
    int                     errno;
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
    printf("enumerate_interfaces\n");
    buf=0;
    RPC_MEM_ALLOC(buf, unsigned char *, 8192, rpc_c_mem_string, rpc_c_mem_waitok); 
    if (buf == NULL){
        *status = rpc_s_no_memory;
         return;
    } 
    ifc.ifc_len = 8192 ;
    ifc.ifc_buf = (caddr_t) buf;
    
ifconf_again:
#ifdef	_KERNEL
    /*
     * for OSF, would be great to do
     * if (errno = osf_ifioctl(desc, OSIOCGIFCONF, (caddr_t) &ifc)),
     * but that would do copyout()'s to user space, so ...
     */
    if (errno = get_if(&ifc, &if_count))
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
    printf("%d interfaces returned\n",if_count);
    RPC_DBG_PRINTF(rpc_e_dbg_general, 10,
		   ("%d bytes of ifreqs, ifreq is %d bytes\n", ifc.ifc_len, sizeof(ifreqb)));

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
		   (if_count * (sizeof (rpc_addr_p_t))),
        rpc_c_mem_rpc_addr_vec,
        rpc_c_mem_waitok);
    
    if (*rpc_addr_vec == NULL)
    {
        if (buf)
            RPC_MEM_FREE(buf, rpc_c_mem_string);
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
#ifdef SOCKADDR_LEN
        memcpy(&ifreqb, ifr, ifrlen);
#else
        memcpy(&ifreqb, ifr, sizeof(struct ifreq));
#endif
ifflags_again:        
#ifdef _KERNEL
        if (errno = osf_ifioctl(desc, SIOCGIFFLAGS, (caddr_t)&ifreqb))
#else
	if (ioctl(desc, SIOCGIFFLAGS, &ifreqb) < 0)
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
        if_flags = ifreqb.ifrb_flags;     /* Copy out the flags */
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
        printf("enumerate_interfaces memmove %lx, %lx, %ld\n", ifr->ifr_addr,
                &if_addr, sizeof(if_addr.buf));
        /*b_copy(&(ifr->ifr_addr), (caddr_t)&if_addr, sizeof(if_addr.buf));*/
        memmove((caddr_t)&if_addr, &(ifr->ifr_addr), sizeof(if_addr.buf));

        printf("return from memmove");
#else
        /*
         * Do the SIOCGIFADDR on a copy of the ifr.  See above.
         */
#ifdef SOCKADDR_LEN
        memcpy(&ifreqb, ifr, sizeof(ifreq)-sizeof(sockaddr)+ifr->ifr_addr.sa_len);
#else
        memcpy(&ifreqb, ifr, sizeof(ifreq));
#endif
ifaddr_again:
#ifdef _KERNEL
        if (errno = osf_ifioctl(desc, SIOCGIFADDR, (caddr_t)&ifreqb))
#else
	if (ioctl(desc, SIOCGIFADDR, &ifreqb) < 0)
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
#ifdef SOCKADDR_LEN
        memcpy (&if_addr, &ifreq.ifr_addr, sizeof(struct sockaddr));
#else
        memcpy (&if_addr, &ifreqb.ifrb_addr, if_addr.su_len);
#endif

#endif	/* NO_SIOCGIFADDR */
 
        /*
         * If this isn't an Internet-family address, ignore it.
         */
        if (if_addr.su_family != AF_INET)
        {
            RPC_DBG_PRINTF(rpc_e_dbg_general, 10, ("AF %d not INET\n", if_addr.su_family));
            continue;
        }

        /*
         * Allocate and fill in an IP RPC address for this interface.
         */
        RPC_MEM_ALLOC (
            ip_addr,
            rpc_ip_addr_p_t,
            sizeof (rpc_ip_addr_t),
            rpc_c_mem_rpc_addr,
            rpc_c_mem_waitok);

        if (ip_addr == NULL)
        {
            *status = rpc_s_no_memory;
            goto FREE_IT;
        }

        ip_addr->rpc_protseq_id = protseq_id;
        /* This is okay since we're only handling AF_INET */
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
        if (!(*efun) ((int)desc, ifr, if_flags, (struct sockaddr *)&if_addr, ip_addr, netmask_addr))
        {
            RPC_MEM_FREE (ip_addr, rpc_c_mem_rpc_addr);
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
    if (buf != NULL)
       RPC_MEM_FREE(buf, rpc_c_mem_string);

    for (i = 0; i < (*rpc_addr_vec)->len; i++)
    {
        RPC_MEM_FREE ((*rpc_addr_vec)->addrs[i], rpc_c_mem_rpc_addr);
    }

    RPC_MEM_FREE (*rpc_addr_vec, rpc_c_mem_rpc_addr_vec);
    if (netmask_addr_vec != NULL)
    {
        for (i = 0; i < (*netmask_addr_vec)->len; i++)
        {
            RPC_MEM_FREE ((*netmask_addr_vec)->addrs[i], RPC_C_MEM_RPC_ADDR);
        }
        RPC_MEM_FREE (*netmask_addr_vec, RPC_C_MEM_RPC_ADDR_VEC);
    }
   printf("return from enumerate_interfaces");
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

int                     desc;
struct ifreq            *ifr;
unsigned32              if_flags;
struct sockaddr         *if_addr;
rpc_ip_addr_p_t         ip_addr;
rpc_ip_addr_p_t         netmask_addr;

{
    ifr_buf                 ifreqb;
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
#ifdef SOCKADDR_LEN
        memcpy(&ifreqb, ifr, sizeof(ifreq)-sizeof(sockaddr)+ifr->ifr_addr.sa_len);
#else
        memcpy(&ifreqb, ifr, sizeof(ifreq));
#endif
    ifnetaddr_again:
#ifdef _KERNEL
        if (errno = osf_ifioctl(desc, SIOCGIFNETMASK, (caddr_t)&ifreqb))
#else
	if (ioctl(desc, SIOCGIFNETMASK, &ifreqb) < 0)
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

#ifdef SOCKADDR_LEN
        memcpy (&netmask_addr->sa, &ifreq.ifr_addr, sizeof(struct sockaddr));
#else
        memcpy (&netmask_addr->sa, &ifreqb.ifrb_addr, if_addr.su_len);
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
	
PRIVATE void rpc__ip_desc_inq_addr (protseq_id, desc, rpc_addr_vec, status)

rpc_protseq_id_t        protseq_id;
rpc_socket_t            desc;
rpc_addr_vector_p_t     *rpc_addr_vec;
unsigned32              *status;

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
            rpc_c_mem_rpc_addr,
            rpc_c_mem_waitok);

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
            RPC_MEM_FREE (ip_addr, rpc_c_mem_rpc_addr);
            *status = rpc_s_cant_inq_socket;
            return;
        }
#endif

	RPC_MEM_ALLOC (
            *rpc_addr_vec,
            rpc_addr_vector_p_t,
            sizeof **rpc_addr_vec,
            rpc_c_mem_rpc_addr_vec,
            rpc_c_mem_waitok);
    
        if (*rpc_addr_vec == NULL)
        {
            RPC_MEM_FREE (ip_addr, rpc_c_mem_rpc_addr);
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

PRIVATE void rpc__ip_get_broadcast (naf_id, rpc_protseq_id, rpc_addrs, status)

rpc_naf_id_t        naf_id;
rpc_protseq_id_t    rpc_protseq_id;
rpc_addr_vector_p_t *rpc_addrs;
unsigned32          *status; 

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

INTERNAL boolean get_broadcast_addr (desc, ifr, if_flags, if_addr, ip_addr, netmask_addr)

int                     desc;
struct ifreq            *ifr;
unsigned32              if_flags;
struct sockaddr        *if_addr;
rpc_ip_addr_p_t         ip_addr;
rpc_ip_addr_p_t         netmask_addr;

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

PRIVATE void rpc__ip_get_broadcast (naf_id, protseq_id, rpc_addr_vec, status)

rpc_naf_id_t            naf_id;
rpc_protseq_id_t        protseq_id;
rpc_addr_vector_p_t     *rpc_addr_vec;
unsigned32              *status; 

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
