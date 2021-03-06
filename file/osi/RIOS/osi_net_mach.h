/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_net_mach.h,v $
 * Revision 1.1.9.1  1996/10/02  17:59:23  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:31  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:16:26  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:40  annie]
 * 
 * Revision 1.1.2.4  1993/01/21  14:51:32  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:54:21  cjd]
 * 
 * Revision 1.1.2.3  1992/09/25  18:51:13  jaffe
 * 	Remove duplicate HEADER and LOG entries
 * 	[1992/09/25  12:28:46  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  20:33:49  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    new file for machine specific additions for the osi_net.h file.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  03:21:58  jaffe]
 * 
 * Revision 1.1.1.2  1992/08/30  03:21:58  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    new file for machine specific additions for the osi_net.h file.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 
 * $EndLog$
 */
/*
 *      Copyright (C) 1992 Transarc Corporation
 *      All rights reserved.
 */

#ifndef TRANSARC_OSI_NET_MACH_H
#define	TRANSARC_OSI_NET_MACH_H

#undef n_name /* protection from nlist.h */
#include <netdb.h>
#include <sys/socket.h>
#include <sys/domain.h>
#include <sys/protosw.h>
#include <netinet/in.h>

/*
 * routines exported by osi_net_aix.c:
 */
extern int osi_SameHost(struct in_addr *addrp);
extern int osi_SameSubNet(struct in_addr *addrp);
extern int osi_SameNet(struct in_addr *addrp);
#endif /* TRANSARC_OSI_NET_MACH_H */
