/*
 *      SCCS:  @(#)tcmfuncs.h	1.4 (92/11/10) 
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

SCCS:   	@(#)tcmfuncs.h	1.4 92/11/10
NAME:		tcmfuncs.h
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations of extern tcm-specific functions not declared in
	other header files

MODIFICATIONS:
	Andrew Dingwall, UniSoft Ltd, October 1992
	added tet_callfuncname() declaration
	
	Added declarations of ictp.c and [ms]tcmdist.c functions.

************************************************************************/


#if __STDC__

extern void tcminit(int, char **);
extern int tcmptype();
extern char *tet_callfuncname();
extern void tet_docleanup();
extern void tet_opencom();
extern void tet_icstart(int, int);
extern void tet_icend(int, int);
extern void tet_tpstart(int, int);
extern void tet_tpend(int, int);
extern void tet_tcmstart(char *, int);
extern void tet_xdicstart(int, int);
extern void tet_xdicend(int, int);
extern void tet_xdtpstart(int, int);
extern void tet_xdtpend(int, int);
extern void ti_tcminit();
extern void ts_tcminit();
extern int ts_tsinfo2bs(char *, char *);
extern int ts_tsinfolen();

#else /* __STDC__ */

extern void tcminit();
extern int tcmptype();
extern char *tet_callfuncname();
extern void tet_docleanup();
extern void tet_opencom();
extern void tet_icstart();
extern void tet_icend();
extern void tet_tpstart();
extern void tet_tpend();
extern void tet_tcmstart();
extern void tet_xdicstart();
extern void tet_xdicend();
extern void tet_xdtpstart();
extern void tet_xdtpend();
extern void ti_tcminit();
extern void ts_tcminit();
extern int ts_tsinfo2bs();
extern int ts_tsinfolen();

#endif /* __STDC__ */


#ifdef lint
extern void ti_lintstuff();
extern void ts_lintstuff();
#endif /* lint */


