/*
 *      SCCS:  @(#)host.c	1.2 (92/09/30) 
 *
 *	UniSoft Ltd., London, England
 *
 * (C) Copyright 1992 X/Open Company Limited
 *
 * All rights reserved.  No part of this source code may be reproduced,
 * stored in a retrieval system, or transmitted, in any form or by any
 * means, electronic, mechanical, photocopying, recording or otherwise,
 * except as stated in the end-user licence agreement, without the prior
 * permission of the copyright owners.
 *
 * X/Open and the 'X' symbol are trademarks of X/Open Company Limited in
 * the UK and other countries.
 */

#ifndef lint
static char sccsid[] = "@(#)host.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)host.c	1.2 92/09/30
NAME:		host.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	INET host address lookup function

MODIFICATIONS:

************************************************************************/

#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include "dtmac.h"
#include "error.h"
#include "inetlib_in.h"

#ifdef NEEDsrcFile
static char srcFile[] = __FILE__;	/* file name for error reporting */
#endif


#define NHCACHE		2	/* no of host cache entries - one for localhost
				   and one for the master system */

/* host cache structure */
struct hcache {
	int hc_refcnt;
	struct in_addr hc_addr;
	char hc_host[SNAMELEN];
};

static struct hcache hcache[NHCACHE];	/* the cache itself */

/*
**	gethostaddr() - find the INET address of a host and return a pointer
**		thereto
**
**	return (struct in_addr *) 0 if the address cannot be found
*/

struct in_addr *gethostaddr(host)
char *host;
{
	register struct hcache *cp1, *cp2;
	register struct hostent *hp;
	register int max;

	/* see if host is already in the cache */
	for (cp1 = hcache; cp1 < &hcache[NHCACHE]; cp1++)
		if (!strcmp(cp1->hc_host, host)) {
			cp1->hc_refcnt++;
			return(&cp1->hc_addr);
		}

	/* look up the host's address in the hosts file */
	errno = 0;
	if ((hp = gethostbyname(host)) == (struct hostent *) 0) {
		error(errno != ENOTTY ? errno : 0,
			"can't find hosts entry for", host);
		return((struct in_addr *) 0);
	}

	/* store the entry in the least used cache slot */
	max = (int) ((unsigned) ~0 >> 1);
	for (cp2 = cp1 = hcache; cp1 < &hcache[NHCACHE]; cp1++)
		if (cp1->hc_refcnt <= 0) {
			cp2 = cp1;
			break;
		}
		else if (cp1->hc_refcnt <= max) {
			max = cp1->hc_refcnt;
			cp2 = cp1;
		}

	cp2->hc_addr = *((struct in_addr *) hp->h_addr);
	cp2->hc_refcnt = 1;
	(void) sprintf(cp2->hc_host, "%.*s", sizeof cp2->hc_host - 1, host);

	return(&cp2->hc_addr);
}

