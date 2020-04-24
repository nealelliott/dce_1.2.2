/*
 *      SCCS:  @(#)dapi.h	1.2 (92/09/30) 
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

SCCS:   	@(#)dapi.h	1.2 92/09/30 DTET release 1.0b
NAME:		dapi.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations of public DTET API functions

	these are the only published functions in the DTET API
	all other interfaces are for DTET internal use and may go away one day

MODIFICATIONS:

************************************************************************/


#if __STDC__
extern void tet_exit(int);
extern void tet_logoff();
extern int tet_remgetlist(int **);
extern int tet_remgetsys();
extern int tet_remexec(int, char *, char **);
extern int tet_remkill(int);
extern int tet_remwait(int, int, int *);
extern int tet_sync(int, int *, int);
#else /* __STDC__ */
extern void tet_exit();
extern void tet_logoff();
extern int tet_remgetlist();
extern int tet_remgetsys();
extern int tet_remexec();
extern int tet_remkill();
extern int tet_remwait();
extern int tet_sync();
#endif /* __STDC__ */

