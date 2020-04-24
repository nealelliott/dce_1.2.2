/*
 *      SCCS:  @(#)llib-linet.c	1.2 (92/09/30) 
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

/************************************************************************

SCCS:   	@(#)llib-linet.c	1.2 92/09/30
NAME:		llib-linet.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	inetlib lint library

MODIFICATIONS:

************************************************************************/


/* LINTLIBRARY */

#include <sys/types.h>
#include <netinet/in.h>
#include "dtmsg.h"
#include "ptab.h"
#include "inetlib_in.h"
#include "tsinfo_in.h"

int bs2tsinfo(from, fromlen, to, tolen) 
char *from; 
int fromlen; 
struct tsinfo **to; 
int *tolen; 
{ 
	return(0); 
}

struct in_addr *gethostaddr(host) 
char *host; 
{ 
	return((struct in_addr *) 0); 
}

struct in_addr *getlocalhostaddr() 
{ 
	return((struct in_addr *) 0); 
}

int gettccdaddr(pp) 
struct ptab *pp; 
{ 
	return(0); 
}

int gettccdport() 
{ 
	return(0); 
}

void ts_accept(lsd) 
int lsd; 
{ 
	;
}

void ts_connect(pp) 
struct ptab *pp; 
{ 
	; 
}

void ts_dead(pp) 
struct ptab *pp; 
{ 
	; 
}

void ts_listen(sd) 
int sd; 
{ 
	; 
}

int ts_nbio(pp) 
struct ptab *pp; 
{ 
	return(0); 
}

int ts_poll(pp, timeout) 
struct ptab *pp; 
int timeout; 
{ 
	return(0); 
}

int ts_ptalloc(pp) 
struct ptab *pp; 
{ 
	return(0); 
}

void ts_ptfree(pp) 
struct ptab *pp; 
{ 
	; 
}

int ts_rcvmsg(pp) 
struct ptab *pp; 
{ 
	return(0); 
}

int ts_sndmsg(pp) 
struct ptab *pp; 
{ 
	return(0); 
}

int ts_tcmputenv()
{
	return(0);
}

int tsinfo2bs(from, to) 
struct tsinfo *from; 
char *to; 
{ 
	return(0); 
}

