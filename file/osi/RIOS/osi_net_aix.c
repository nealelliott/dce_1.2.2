/*
 *      Copyright (C) 1994 Transarc Corporation
 *      All rights reserved.
 */

/*
 * HISTORY
 * $Log$
 */

#include <dcedfs/sysincludes.h>
#include <dcedfs/osi_net_mach.h>
#include <net/if.h>
#include <netinet/in_var.h>

#if	defined(KERNEL)
/***********************************************************************
 *
 * osi_SameHost(addrp):	Check if the address passed in is configured
 *			on any of our network interfaces.
 *
 ***********************************************************************/

int osi_SameHost(struct in_addr *addrp)
{
    extern struct in_ifaddr	*in_ifaddr;
    struct in_ifaddr 		*ifap;
    struct sockaddr_in 		*sinp;

    for (ifap=in_ifaddr; ifap; ifap=ifap->ia_next) {
	sinp = IA_SIN(ifap);
	if (addrp->s_addr == sinp->sin_addr.s_addr) {
	    return(1);
	}
    }
    return(0);
}


/***********************************************************************
 *
 * osi_SameSubNet(addrp):	Check if the address passed in belongs 
 *			to a subnet which is configured on any of
 *			our interfaces.
 *
 ***********************************************************************/

int osi_SameSubNet(struct in_addr *addrp)
{
    extern struct in_ifaddr	*in_ifaddr;
    struct in_ifaddr 		*ifap;

    for (ifap=in_ifaddr; ifap; ifap=ifap->ia_next) {
	if ((addrp->s_addr & ifap->ia_subnetmask) == ifap->ia_subnet) {
	    return(1);
	}
    }
    return(0);
}

/***********************************************************************
 *
 * osi_SameNet(addrp):	Check if the address passed in belongs to
 *			a network which is configured on any of
 *			our interfaces.
 *
 ***********************************************************************/

int osi_SameNet(struct in_addr *addrp)
{
    extern struct in_ifaddr	*in_ifaddr;
    struct in_ifaddr 		*ifap;

    for (ifap=in_ifaddr; ifap; ifap=ifap->ia_next) {
	if ((addrp->s_addr & ifap->ia_netmask) == ifap->ia_net)
	    return(1);
    }
    return(0);
}

#else /* KERNEL */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
/*
 * User space functions to help us along
 */
#define MAX_IFS 32
static struct sockaddr_in	myaddress = {0,0,0,0};
static struct sockaddr_in	mysubnetmask = {0.0,0,0};
static struct sockaddr_in	mynetmask = {0,0,0,0};
static char			myIfName[IFNAMSIZ];

/***********************************************************************
 *
 * osi_SetupIfInfo(): Get the interface information from the kernel
 * 	that we need for network adjacency computations.
 *
 ***********************************************************************/
/*
 * Some important assumptions:  This will only do AF_INET for now.
 * This will process only one address for now, the address of the primary
 * AF_INET interface that we get by trying to query each interface from
 * the list of configured interfaces that the kernel gives us.
 */
void osi_SetupIfInfo()
{
    int 		s, i;
    struct ifreq	*ifrp;
    struct ifconf	ifc;

    /* make a socket for calls */
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	printf("osi_SetupIFInfo: FAILED in socket setup, e=%d\n",errno);
	return;
    }

    /* allocate space for all the interfaces (names mostly) */
    ifrp = (struct ifreq *)osi_Alloc(MAX_IFS * sizeof(struct ifreq));
    ifc.ifc_buf = (caddr_t)ifrp;
    ifc.ifc_len = MAX_IFS * sizeof(struct ifreq);

    /* get all the configured interfaces */
    if (ioctl(s, SIOCGIFCONF, (caddr_t)&ifc) < 0) {
	printf("osi_SetupIFInfo: FAILED in SIOCGIFCONF, e=%d\n",errno);
	return;
    }

    /* figure out which interface supports AF_INET (other than loopback) */
    for (i=0; i<MAX_IFS; i++, ifrp++) {
	if ((ioctl(s, SIOCGIFADDR, (caddr_t)ifrp) == 0) &&
	    (bcmp(ifrp->ifr_name, "lo", 2) != 0)) {
	    break;
	}
    }
    bcopy(ifrp->ifr_name, myIfName, IFNAMSIZ);
    myaddress.sin_addr = ((struct sockaddr_in *)&ifrp->ifr_addr)->sin_addr;

    if (ioctl(s, SIOCGIFNETMASK, (caddr_t)ifrp) < 0) {
	printf("osi_SetupIFInfo: FAILED in SIOCGIFNETMASK e=%d\n",errno);
	return;
    }
    mysubnetmask.sin_addr = ((struct sockaddr_in *)&ifrp->ifr_addr)->sin_addr;
    osi_Free(ifc.ifc_buf, MAX_IFS * sizeof(struct ifreq));

    if (IN_CLASSA(myaddress.sin_addr.s_addr)) {
	mynetmask.sin_addr.s_addr = IN_CLASSA_NET;
    } else if (IN_CLASSB(myaddress.sin_addr.s_addr)) {
	mynetmask.sin_addr.s_addr = IN_CLASSB_NET;
    } else if (IN_CLASSC(myaddress.sin_addr.s_addr)) {
	mynetmask.sin_addr.s_addr = IN_CLASSC_NET;
    } else
	mynetmask.sin_addr.s_addr = 0;

    (void) close(s);
    return;
}


/***********************************************************************
 *
 * osi_SameHost(addrp):	Check if the address passed in is ours.
 *
 ***********************************************************************/

int osi_SameHost(struct in_addr *addrp)
{
    if (myaddress.sin_addr.s_addr == 0)
	osi_SetupIfInfo();

    if (myaddress.sin_addr.s_addr == addrp->s_addr)
	return(1);
    return(0);
}

/***********************************************************************
 *
 * osi_SameSubNet(addrp): Check if the address passed in belongs 
 *			to our subnet.
 *
 ***********************************************************************/

int osi_SameSubNet(struct in_addr *addrp)
{
    unsigned long	subnet;

    if (myaddress.sin_addr.s_addr == 0)
	osi_SetupIfInfo();

    subnet = myaddress.sin_addr.s_addr & mysubnetmask.sin_addr.s_addr;
    if ((addrp->s_addr & mysubnetmask.sin_addr.s_addr) == subnet)
	return(1);
    return(0);
}

/***********************************************************************
 *
 * osi_SameNet(addrp): Check if the address passed in belongs to
 *			our network.
 *
 ***********************************************************************/

int osi_SameNet(struct in_addr *addrp)
{
    unsigned long	net;
    
    if (myaddress.sin_addr.s_addr == 0)
	osi_SetupIfInfo();

    net = myaddress.sin_addr.s_addr & mynetmask.sin_addr.s_addr;
    if ((addrp->s_addr & mynetmask.sin_addr.s_addr) == net)
	return(1);
    return(0);
}

#endif /* KERNEL */
