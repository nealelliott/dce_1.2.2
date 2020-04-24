/*
 *      SCCS:  @(#)sigmap.c	1.2 (92/09/30) 
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
static char sccsid[] = "@(#)sigmap.c	1.2 (92/09/30) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)sigmap.c	1.2 92/09/30
NAME:		sigmap.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	map of local signal numbers to DTET signal numbers

	the signal names in the map were collected from <signal.h> files
	on several machines; however, your mileage may vary

	the common signals (0 -> 15) have the same value on most systems;
	the local and DTET signal numbers are the same for these signals and
	the code in mapsignal() and unmapsignal() is most efficient when
	this is the case

MODIFICATIONS:

************************************************************************/

#include <signal.h>
#include "sigmap.h"


struct sigmap Sigmap[] = {

	{ 0,		0 },

#ifdef SIGHUP
	{ SIGHUP,	1 },
#endif

#ifdef SIGINT
	{ SIGINT,	2 },
#endif

#ifdef SIGQUIT
	{ SIGQUIT,	3 },
#endif

#ifdef SIGILL
	{ SIGILL,	4 },
#endif

#ifdef SIGTRAP
	{ SIGTRAP,	5 },
#endif

#ifdef SIGIOT
	{ SIGIOT,	6 },
#endif

#ifdef SIGEMT
	{ SIGEMT,	7 },
#endif

#ifdef SIGFPE
	{ SIGFPE,	8 },
#endif

#ifdef SIGKILL
	{ SIGKILL,	9 },
#endif

#ifdef SIGBUS
	{ SIGBUS,	10 },
#endif

#ifdef SIGSEGV
	{ SIGSEGV,	11 },
#endif

#ifdef SIGSYS
	{ SIGSYS,	12 },
#endif

#ifdef SIGPIPE
	{ SIGPIPE,	13 },
#endif

#ifdef SIGALRM
	{ SIGALRM,	14 },
#endif

#ifdef SIGTERM
	{ SIGTERM,	15 },
#endif

#ifdef SIGURG
	{ SIGURG,	16 },
#endif

#ifdef SIGSTOP
	{ SIGSTOP,	17 },
#endif

#ifdef SIGTSTP
	{ SIGTSTP,	18 },
#endif

#ifdef SIGCONT
	{ SIGCONT,	19 },
#endif

#ifdef SIGCHLD
	{ SIGCHLD,	20 },
#endif

#ifdef SIGTTIN
	{ SIGTTIN,	21 },
#endif

#ifdef SIGTTOU
	{ SIGTTOU,	22 },
#endif

#ifdef SIGIO
	{ SIGIO,	23 },
#endif

#ifdef SIGXCPU
	{ SIGXCPU,	24 },
#endif

#ifdef SIGXFSZ
	{ SIGXFSZ,	25 },
#endif

#ifdef SIGVTALRM
	{ SIGVTALRM,	26 },
#endif

#ifdef SIGPROF
	{ SIGPROF,	27 },
#endif

#ifdef SIGWINCH
	{ SIGWINCH,	28 },
#endif

#ifdef SIGUSR1
	{ SIGUSR1,	30 },
#endif

#ifdef SIGUSR2
	{ SIGUSR2,	31 },
#endif

#ifdef SIGABRT
	{ SIGABRT,	1001 },
#endif

#ifdef SIGCLD
	{ SIGCLD,	1002 },
#endif

#ifdef SIGLOST
	{ SIGLOST,	1003 },
#endif

#ifdef SIGPOLL
	{ SIGPOLL,	1004 },
#endif

#ifdef SIGPWR
	{ SIGPWR,	1005 }
#endif

};

int Nsigmap = sizeof Sigmap / sizeof Sigmap[0];

