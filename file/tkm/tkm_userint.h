/*
 *	Copyright (C) 1992 1993 1994 Transarc Corporation
 *	All Rights Reserved.
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *	tkm_userint.h -- header file containing constants and routine
 *	prototypes shared between tkm.c and tkm_userint.c
 *
 */

#if !defined(TRANSARC_TKM_USERINT_H)
#define TRANSARC_TKM_USERINT_H 1

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/cmd.h>

/**** the operation arguments ****/

#define SET_EXP_NEW_EXP_INDEX		0
#define SET_MAXTOKENS_INDEX	0
#define SET_MINTOKENS_INDEX	0
#define SET_MAXFILES_INDEX	0
#define SET_MAXVOLS_INDEX	0

/* the operation that main needs to call to get things set up */
extern void tkm_SetUpSyntax (void);

/* the operations to which the setup routines need to refer */
extern int tkm_DoGetExpirationInterval (struct cmd_syndesc *	aSyntaxP,
					char *			aRockP);
extern int tkm_DoSetExpirationInterval (struct cmd_syndesc *	aSyntaxP,
					char *			aRockP);
extern int tkm_DoGetTokenQuota (struct cmd_syndesc *	aSyntaxP,
				char *			aRockP);
extern int tkm_DoSetTokenQuota (struct cmd_syndesc *	aSyntaxP,
				char *			aRockP);
extern int tkm_DoGetMinTokens (struct cmd_syndesc *	aSyntaxP,
			       char *			aRockP);
extern int tkm_DoSetMinTokens (struct cmd_syndesc *	aSyntaxP,
			       char *			aRockP);
extern int tkm_DoGetMaxFiles (struct cmd_syndesc *	aSyntaxP,
			       char *			aRockP);
extern int tkm_DoSetMaxFiles (struct cmd_syndesc *	aSyntaxP,
			       char *			aRockP);
extern int tkm_DoGetMaxVols (struct cmd_syndesc *	aSyntaxP,
			       char *			aRockP);
extern int tkm_DoSetMaxVols (struct cmd_syndesc *	aSyntaxP,
			       char *			aRockP);
#endif /* !defined(TRANSARC_TKM_USERINT_H) */
