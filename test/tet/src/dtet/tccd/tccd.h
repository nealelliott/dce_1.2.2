/*
 *      SCCS:  @(#)tccd.h	1.2 (92/09/30) 
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

SCCS:   	@(#)tccd.h	1.2 92/09/30
NAME:		tccd.h
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations of tccd-specific functions not declared in other
	header files

MODIFICATIONS:

************************************************************************/


#if __STDC__

extern void etdead(struct ptab *);
extern void logent(char *, char *);
extern void logerror(int, char *, int, char *, char *);
extern void loginit();
extern void op_access(struct ptab *);
extern void op_cfname(struct ptab *);
extern void op_chdir(struct ptab *);
extern void op_config(struct ptab *);
extern void op_exec(struct ptab *);
extern void op_kill(struct ptab *);
extern void op_lockfile(struct ptab *);
extern void op_mkdir(struct ptab *);
extern void op_mksdir(struct ptab *);
extern void op_mktmpdir(struct ptab *);
extern void op_putenv(struct ptab *);
extern void op_rcvconf(struct ptab *);
extern void op_rmdir(struct ptab *);
extern void op_rxfile(struct ptab *);
extern void op_sharelock(struct ptab *);
extern void op_sndconf(struct ptab *);
extern void op_tsfiles(struct ptab *);
extern void op_tsinfo(struct ptab *);
extern void op_unlink(struct ptab *);
extern void op_wait(struct ptab *);
extern int ss_tsargproc(char *, char *);
extern void ss_tsinitb4fork();
extern int ss_tslogon();
extern int tetrootset(char *);
extern int tetexecset(char *);
extern int tsrootset(char *);
extern void ts_forkdaemon();
extern void ts_logstart();

#else /* __STDC__ */

extern void etdead();
extern void logent();
extern void logerror();
extern void loginit();
extern void op_access();
extern void op_cfname();
extern void op_chdir();
extern void op_config();
extern void op_exec();
extern void op_kill();
extern void op_lockfile();
extern void op_mkdir();
extern void op_mksdir();
extern void op_mktmpdir();
extern void op_putenv();
extern void op_rcvconf();
extern void op_rmdir();
extern void op_rxfile();
extern void op_sharelock();
extern void op_sndconf();
extern void op_tsfiles();
extern void op_tsinfo();
extern void op_unlink();
extern void op_wait();
extern int ss_tsargproc();
extern void ss_tsinitb4fork();
extern int ss_tslogon();
extern int tetrootset();
extern int tetexecset();
extern int tsrootset();
extern void ts_forkdaemon();
extern void ts_logstart();

#endif /* __STDC__ */

#ifdef lint
void ti_lintstuff();
void ts_lintstuff();
#endif

/* work around for bug filed by DME team with XOpen (dtet_support 33) */ 
#ifndef MAXNAMLEN
#define MAXNAMLEN NAME_MAX
#endif
