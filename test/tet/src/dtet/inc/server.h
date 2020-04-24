/*
 *      SCCS:  @(#)server.h	1.2 (92/09/30) 
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

SCCS:   	@(#)server.h	1.2 92/09/30 DTET release 1.0b
NAME:		server.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations of server-specific functions that may be called by
	library routines

MODIFICATIONS:

************************************************************************/


#if __STDC__

extern int ss_argproc(char *, char *);
extern void ss_cleanup();
extern void ss_connect(struct ptab *);
extern void ss_dead(struct ptab *);
extern void ss_initdaemon();
extern void ss_logoff(struct ptab *);
extern int ss_logon(struct ptab *);
extern void ss_newptab(struct ptab *);
extern void ss_process(struct ptab *);
extern void ss_procrun();
extern int ss_ptalloc(struct ptab *);
extern void ss_ptfree(struct ptab *);
extern int ss_serverloop();
extern void ss_serverproc(struct ptab *);
extern void ss_timeout(struct ptab *);
extern int ss_tsinfo(struct ptab *, int);

#else /* __STDC__ */

extern int ss_argproc();
extern void ss_cleanup();
extern void ss_connect();
extern void ss_dead();
extern void ss_initdaemon();
extern void ss_logoff();
extern int ss_logon();
extern void ss_newptab();
extern void ss_process();
extern void ss_procrun();
extern int ss_ptalloc();
extern void ss_ptfree();
extern int ss_serverloop();
extern void ss_serverproc();
extern void ss_timeout();
extern int ss_tsinfo();

#endif /* __STDC__ */

