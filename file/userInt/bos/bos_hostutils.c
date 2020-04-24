/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bos_hostutils.c,v $
 * Revision 1.1.431.1  1996/10/02  21:06:08  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:11  damon]
 *
 * Revision 1.1.426.4  1994/08/24  17:57:47  sasala
 * 	Implement partial S12Y for DFS - bos
 * 	[1994/08/24  17:43:02  sasala]
 * 
 * Revision 1.1.426.3  1994/07/13  22:23:02  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:53:45  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:54  mbs]
 * 
 * Revision 1.1.426.2  1994/06/09  14:23:53  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:35:48  annie]
 * 
 * Revision 1.1.426.1  1994/02/04  20:34:26  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:19:48  devsrc]
 * 
 * Revision 1.1.424.1  1993/12/07  17:36:54  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  17:47:16  jaffe]
 * 
 * Revision 1.1.3.4  1993/01/21  16:28:21  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:21:32  cjd]
 * 
 * Revision 1.1.3.3  1992/11/24  19:49:54  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:34:57  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/31  21:52:36  jaffe
 * 	Replace missing RCS ids
 * 	[1992/08/31  16:10:13  jaffe]
 * 
 * 	Transarc delta: jaffe-ot3211-fix-prototypes 1.1
 * 	  Selected comments:
 * 	    Remove ANSI-C declaration files which contained prototypes for standard
 * 	    functions.  We expect these prototypes to be defined by the system include
 * 	    files.
 * 	    use stdio, netinet/in.h and arpa/inet.h instead of Decls.h files
 * 	[1992/08/30  13:32:49  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:49:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bos_hostutils.c -- implementations of routines useful for dealing with host names
 * and addresses
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbosmac.h>
#include <dfsbossvc.h>
#include <dfsbosmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/userInt/bos/bos_hostutils.c,v 1.1.431.1 1996/10/02 21:06:08 damon Exp $")


#include <bos_hostutils.h>
#include <stdio.h>
#include <ctype.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
#include <dcedfs/osi.h>
*/

EXPORT char * hostutil_NetAddrStr(hostString)
     char *	hostString;
{
  char *		rtnVal = (char *)NULL;
  struct hostent *	tempHostEnt;
  struct in_addr	theAddr;
  char			hostname[BUFSIZ];
  
  if (hostString == (char *)NULL) {
    if (gethostname(hostname, BUFSIZ) == 0) {
      tempHostEnt = gethostbyname(hostname);
      if (tempHostEnt != (struct hostent *)NULL) {
	theAddr.s_addr = ntohl(*((long *)((tempHostEnt->h_addr_list)[0])));
	rtnVal = inet_ntoa(theAddr);
      }
    }
    else {
      dce_fprintf(stderr, bos_s_gethostename, strerror(errno));
    }
  }
  else {
    if (isdigit(hostString[0])) {
      rtnVal = hostString;
    }
    else {
      tempHostEnt = gethostbyname(hostString);
      if (tempHostEnt != (struct hostent *)NULL) {
	theAddr.s_addr = ntohl(*((long *)((tempHostEnt->h_addr_list)[0])));
	rtnVal = inet_ntoa(theAddr);
      }
    }
    
  }

  return rtnVal;
}
