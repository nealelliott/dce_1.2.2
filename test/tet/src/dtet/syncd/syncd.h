/*
 *      SCCS:  @(#)syncd.h	1.2 (92/09/30) 
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

SCCS:   	@(#)syncd.h	1.2 92/09/30
NAME:		syncd.h
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations of syncd-specific functions not declared in other
	header files

MODIFICATIONS:

************************************************************************/


#if __STDC__

extern void op_async(struct ptab *);
extern void op_snget(struct ptab *);
extern void op_snsys(struct ptab *);
extern void op_usync(struct ptab *);
extern void stdead(struct ptab *);
extern void stloop();
extern void ss_tsinitb4fork();

#ifndef NOTRACE
extern char *stflags(int);
#endif /* !NOTRACE */

#else /* __STDC__ */

extern void op_async();
extern void op_snget();
extern void op_snsys();
extern void op_usync();
extern void stdead();
extern void stloop();
extern void ss_tsinitb4fork();

#ifndef NOTRACE
extern char *stflags();
#endif /* !NOTRACE */

#endif /* __STDC__ */

#ifdef lint
extern void ti_lintstuff();
extern void ts_lintstuff();
#endif /* lint */

