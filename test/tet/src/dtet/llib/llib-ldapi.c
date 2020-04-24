/*
 *      SCCS:  @(#)llib-ldapi.c	1.2 (92/09/30) 
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

SCCS:   	@(#)llib-ldapi.c	1.2 92/09/30
NAME:		llib-ldapi.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	May 1992

DESCRIPTION:
	dapi lint library

MODIFICATIONS:

************************************************************************/

/* LINTLIBRARY */

#include "dapi.h"

void tet_exit(status)
int status;
{
	;
}

void tet_logoff()
{
	;
}

int tet_remgetlist(sysnames)
int **sysnames;
{
	return(0);
}

int tet_remgetsys()
{
	return(0);
}

int tet_remexec(sysname, file, argv)
int sysname;
char *file;
char **argv;
{
	return(0);
}

int tet_remkill(remoteid)
int remoteid;
{
	return(0);
}

int tet_remwait(remoteid, waittime, statloc)
int remoteid, waittime, *statloc;
{
	return(0);
}

int tet_sync(syncptno, syncnames, waittime)
int syncptno, *syncnames, waittime;
{
	return(0);
}

