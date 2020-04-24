/*
 *      SCCS:  @(#)xresd.h	1.2 (92/09/30) 
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

SCCS:   	@(#)xresd.h	1.2 92/09/30
NAME:		xresd.h
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations of extern xresd-specific functions not declared in
	other header files

MODIFICATIONS:

************************************************************************/


#if __STDC__

extern void op_cfname(struct ptab *);
extern void op_rcfname(struct ptab *);
extern void op_codesf(struct ptab *);
extern char * rcodesf();
extern void op_tfclose(struct ptab *);
extern void op_tfopen(struct ptab *);
extern void op_tfwrite(struct ptab *);
extern void op_icend(struct ptab *);
extern void op_icstart(struct ptab *);
extern void op_result(struct ptab *);
extern void op_tpend(struct ptab *);
extern void op_tpstart(struct ptab *);
extern void op_xres(struct ptab *);
extern void op_xropen(struct ptab *);
extern void op_xrsys(struct ptab *);
extern void ss_tsinitb4fork();
extern void tfdead(struct ptab *);
extern int ts_xresdinit();
extern void xtdead(struct ptab *);
extern void xtloop();
extern char *get_code(int, int *);

#else /* __STDC__ */

extern void op_cfname();
extern void op_rcfname();
extern void op_codesf();
extern char * rcodesf();
extern void op_tfclose();
extern void op_tfopen();
extern void op_tfwrite();
extern void op_icend();
extern void op_icstart();
extern void op_result();
extern void op_tpend();
extern void op_tpstart();
extern void op_xres();
extern void op_xropen();
extern void op_xrsys();
extern void ss_tsinitb4fork();
extern void tfdead();
extern int ts_xresdinit();
extern void xtdead();
extern void xtloop();
extern char *get_code();

#endif /* __STDC__ */

#ifdef lint
extern void ti_lintstuff();
extern void ts_lintstuff();
#endif /* lint */

