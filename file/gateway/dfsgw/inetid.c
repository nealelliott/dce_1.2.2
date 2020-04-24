/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: inetid.c,v $
 * Revision 1.1.6.1  1996/10/02  17:49:57  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:21  damon]
 *
 * Revision /main/HPDCE02/2  1995/06/22  20:13 UTC  maunsell_c
 * 	mothra C&S fixes
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/1  1995/05/26  17:26 UTC  brezak
 * 	remove extra deref in store_info() and make store_info() static.
 * 
 * Revision /main/HPDCE02/1  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
 * 	add @sys @host support for dfs_login/dfsgwd/dfsgw
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:08  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:47  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:04:05  mckeen]
 * 
 * $EndLog$
 */
/*
 * inetid.c - Implementation of internet network ID processing.
 */
/*
 * Internet addresses are one form of network ID, this module
 * specifies the interface of a network ID  that is an
 * internet address.
 */
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netid.h>
#include <inetid.h> 

#include <dcedfs/stds.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgw/inetid.c,v 1.1.6.1 1996/10/02 17:49:57 damon Exp $")

extern char *progname;
static int valid;
static struct sockaddr addr;
static char fullname[MAXHOSTNAMELEN];
static netid_t net_addr;
static netid_p netaddr;

/*
 * inetid_init - initializer.
 *
 * This little routine just sets some variables up.
 */
netid_p
inetid_init()
{
   extern int inetid_assign_id_str();
   extern int inetid_assign_id_addr();
   extern int inetid_get_sockaddr();
   extern int inetid_get_str();


   net_addr.assign_id_str = inetid_assign_id_str;
   net_addr.assign_id_addr = inetid_assign_id_addr;
   net_addr.get_sockaddr = inetid_get_sockaddr;
   net_addr.get_str = inetid_get_str;
   netaddr = &net_addr;
   valid = 0;
   addr.sa_family = AF_INET;
   return(netaddr);
}

/*
 * store_info - Store network information in object.
 *
 * This is a little internal function that stores away relevant
 * network information given a struct hostent.
 */
static void
store_info(struct hostent *h)
{ 
   (void) strcpy((char *)&fullname, h->h_name);
   
   /*
    * Place numerical address in sockaddr structure.
    */ 
   (void) memmove((char *)&addr.sa_data, *h->h_addr_list, h->h_length);
}

/*
 * inetid_assign_id_addr - Assign address to inet object via sockaddr.
 *
 * This routine is passed a struct sockaddr which holds in network-independent
 * form the network identifier of a node.  It initializes the inet object
 * using this information.
 * 
 * This routine returns 0 on success, -1 otherwise.
 */
int
inetid_assign_id_addr(struct sockaddr *a)
{
   struct hostent *h;                                 /* Hostent info */
   unsigned long x;

   /*
    * This is garbarage.  There's no automatic alignment
    * fault handlers on UX, a.sa_data starts on a 2 byte
    * boundary, and the gethostbyaddr routine uses structure
    * copies that do ldws and cause alignment faults.  So
    * do a little copy here just to get around it.  Sheesh.
    */
    (void) memcpy(&x, a->sa_data, 4);
    if ((h = gethostbyaddr(((char *)&x), 4, AF_INET)) != 0) {
       valid++;
       store_info(h);
       return(0);
    } else {
       return(-1);
    }
}

/*
 * inetid_assign_id_str - Assign address to inet object via ascii string.
 *
 * This routine is passed a string, which is either an internet address 
 * in dot notation, or a string that represents the name of the node.
 * Given either, this routine will initialize the object with an address.
 * 
 * This routine returns 0 on success, -1 otherwise.
 */
int
inetid_assign_id_str(char *str_address)
{
   unsigned long iaddr;                      /* Internet adresses are 32 bits */
   struct hostent *h;                        /* hostent info */



   /*
    * Assume we're being passed the "dot" notation, in which case
    * the call to inet_addr will succeed. Otherwise use gethostbyname.
    */
   if ((iaddr = inet_addr(str_address)) == (unsigned long)-1) {
      if ((h = gethostbyname(str_address)) != 0) {
         (void) memmove((char *)&iaddr, *h->h_addr_list, 4); 
         valid++;
      }
   } else {
      if ((h = gethostbyaddr((char *)&iaddr, 4, AF_INET)) == 0) {
         return(-1);
      } else {
         valid++;
      }
   }
   /*
    * If we were able to obtain full information about the host, save
    * the name for later reporting.
    */
    if (valid) {
       store_info(h);
       return(0);
    } else {
       return(-1);
    }
}


/*
 * inetid_get_sockaddr - Return address in sockaddr form. 
 */
int
inetid_get_sockaddr(struct sockaddr *a)
{
   if (valid) {
      *a = addr;
      return(0); 
   } else {
      return(1);
   }
      
}

/*
 * inetid_get_str - Return address in ascii form.
 */
int
inetid_get_str(char *str_address, size_t len)
{
   if (valid) {
      strncpy(str_address, fullname, len - 1);
      return(1);
   } else {
      return(0);
   }
}
