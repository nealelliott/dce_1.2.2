/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rep_addr.c,v $
 * Revision 1.1.683.1  1996/10/02  18:14:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:45:57  damon]
 *
 * $EndLog$
 */
/*
 * (C) Copyright 1990, 1994 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */

/* rep_addr.c: reading the local network addresses. */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/basicincludes.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/rep_data.h>
#include <dcedfs/icl.h>

#include <repser.h>
#include <rep_trace.h>

#include <pthread.h>

#include <net/if.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/rep/rep_addr.c,v 1.1.683.1 1996/10/02 18:14:01 damon Exp $")

struct afsNetAddr myNetAddrs[ADDRSPERSITE];
int numMyNetAddrs = -1;
int goodMyNetAddrs = -1;

int getMyNetAddrs()
{/* Get our internet address as afsNetAddrs in ``myNetAddrs'' with a count in numMyNetAddrs. */
    register int i;
    register struct hostent *hp;
    char namebuf[500];
    struct sockaddr_in sockbuff;
    register char *thisAddr;
    extern int errno;

    numMyNetAddrs = 0;
    bzero(myNetAddrs, sizeof(myNetAddrs));
    bzero(namebuf, sizeof(namebuf));
    if (gethostname(namebuf, sizeof(namebuf)) != 0) {
	Log("getMyNetAddrs: Cannot get name of this machine: code %d",
	    errno);
	return 0;
    }
    if (namebuf[sizeof(namebuf)-1] != '\0') {
	Log("getMyNetAddrs: Host name size overflows %d-character buffer",
	    sizeof(namebuf));
	return 0;
    }
    hp = (struct hostent *)hostutil_GetHostByName(namebuf);
    if (!hp) {
	Log("getMyNetAddrs: Cannot find address of this host (%s)",
	    namebuf);
	return 0;
    }
#ifdef h_addr
    /* OK, h_addr is a backward-compatibility macro */
    for (i = 0; numMyNetAddrs < ADDRSPERSITE; ++i) {
	thisAddr = hp->h_addr_list[i];
	if (thisAddr == 0) break;
	bzero((char *)&sockbuff, sizeof(sockbuff));
	bcopy(thisAddr, (char *)&sockbuff.sin_addr, hp->h_length);
	myNetAddrs[numMyNetAddrs] = *((afsNetAddr *) &sockbuff);
	myNetAddrs[numMyNetAddrs].type = hp->h_addrtype;
	++numMyNetAddrs;
    }
#else /* h_addr */
    /* OK, h_addr is a structure element */
    thisAddr = hp->h_addr;
    bzero((char *)&sockbuff, sizeof(sockbuff));
    bcopy(thisAddr, (char *)&sockbuff.sin_addr, hp->h_length);
    myNetAddrs[0] = *((afsNetAddr *) &sockbuff);
    myNetAddrs[0].type = hp->h_addrtype;
    ++numMyNetAddrs;
#endif /* h_addr */
    goodMyNetAddrs = 0;
    for (i = 0; i < numMyNetAddrs; ++i) {
	if (myNetAddrs[i].type == AF_INET) {
	    goodMyNetAddrs = i;
	    break;
	}
    }
    icl_Trace1(rep_iclSetp, REP_TR_2ADDR,
		ICL_TYPE_LONG, numMyNetAddrs);
    return numMyNetAddrs;
}


int OnThisMachine(WhatP, howMany)
register struct afsNetAddr *WhatP; int32 howMany;
{/* Return TRUE iff the given *WhatP is us. */
    register int Ix, NIx;

    if (numMyNetAddrs == -1) getMyNetAddrs();
    if (numMyNetAddrs == 0) return 0;
    for (Ix = 0; Ix < howMany; ++Ix) {
	for (NIx = 0; NIx < numMyNetAddrs; ++NIx) {
	    if (WhatP[Ix].type == myNetAddrs[NIx].type) {
		switch (WhatP[Ix].type) {
		    case AF_INET:
			if (bcmp((char *)&(((struct sockaddr_in *) &WhatP[Ix])->sin_addr.s_addr),
				 (char *)&(((struct sockaddr_in *) &myNetAddrs[NIx])->sin_addr.s_addr),
				 4) == 0) return 1;
			break;
		    default:
			if (bcmp((char *)WhatP[Ix].data,
				 (char *)myNetAddrs[NIx].data,
				 sizeof(WhatP[Ix].data)) == 0) return 1;
			break;
		}
	    }
	}
    }
    return 0;
}
