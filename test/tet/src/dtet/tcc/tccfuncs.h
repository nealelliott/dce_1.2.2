/*
 *      SCCS:  @(#)tccfuncs.h	1.2 (92/09/30) 
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

SCCS:   	@(#)tccfuncs.h	1.2 92/09/30
NAME:		tccfuncs.h
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations of tcc-specific client-related functions not
	declared in other header files


MODIFICATIONS:

************************************************************************/


#if __STDC__ 

extern int sd_start();
extern int tccinit(int, char **);
extern int ts_stserver(struct ptab *, char **);
extern int ts_tccinit();
extern int ts_tsinfo2bs(char *, char *);
extern int ts_tsinfolen();
extern char **ts_xdargs(char *);
extern int xd_start(char *);

#else /* __STDC__ */

extern int sd_start();
extern int tccinit();
extern int ts_stserver();
extern int ts_tccinit();
extern int ts_tsinfo2bs();
extern int ts_tsinfolen();
extern char **ts_xdargs();
extern int xd_start();

#endif /* __STDC__ */


#ifdef lint

extern void ti_lintstuff();
extern void ts_lintstuff();

#endif /* lint */

