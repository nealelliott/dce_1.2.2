/*
 *      SCCS:  @(#)tslib.h	1.2 (92/09/30) 
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

SCCS:   	@(#)tslib.h	1.2 92/09/30 DTET release 1.0b
NAME:		tslib.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations for function interfaces to the transport-specific
	function library

	all transport-specific libraries should implement all of these
	interfaces

MODIFICATIONS:

************************************************************************/


#if __STDC__

extern void ts_connect(struct ptab *);
extern void ts_dead(struct ptab *);
extern int ts_poll(struct ptab *, int);
extern int ts_ptalloc(struct ptab *);
extern void ts_ptfree(struct ptab *);
extern int ts_rcvmsg(struct ptab *);
extern int ts_sndmsg(struct ptab *);
extern int ts_tcmputenv();

#else /* __STDC__ */

extern void ts_connect();
extern void ts_dead();
extern int ts_poll();
extern int ts_ptalloc();
extern void ts_ptfree();
extern int ts_rcvmsg();
extern int ts_sndmsg();
extern int ts_tcmputenv();

#endif /* __STDC__ */

