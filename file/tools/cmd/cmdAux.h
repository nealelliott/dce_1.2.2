/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmdAux.h,v $
 * Revision 1.1.9.1  1996/10/02  18:47:35  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:48:55  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:22:52  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:34:27  annie]
 * 
 * Revision 1.1.2.3  1993/01/21  15:55:00  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:38:24  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  19:47:54  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:32:09  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:50:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  cmdAux.h -- Extra macros data types and headers for routines that are useful
 * when dealing with the AFS command parsing package
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#ifndef TRANSARC_UI_CMDAUX_H
#define TRANSARC_UI_CMDAUX_H

/* allow CPP to get cmd.h from the same place it got cmdAux.h */
#include <dcedfs/cmd.h>

#define cmd_IsCmdError(error)	\
  ( ((error) >= CMD_EXCESSPARMS) && ((error) <= CMD_TOOBIG) )
#define cmd_IsParamPresent(ts, index)	\
  ( (ts)->parms[(index)].items != (struct cmd_item *)NULL )
#define cmd_FirstItem(ts, index)	( (ts)->parms[(index)].items  )
#define cmd_FirstData(ts, index)	( (ts)->parms[(index)].items->data )

#define cmd_IndexedAddParm(ts, name, type, flags, help, index)	\
  cmd_Seek(ts, index);	cmd_AddParm(ts, name, type, flags, help)

typedef struct cmd_dataIter {
  struct cmd_syndesc *	theTs;
  struct cmd_item *	nextItem;
} cmd_dataIter_t;

/* first, a few definitions we may need to make the declarations work */
#if !defined(_TAKES)
#define __CMDAUX_INCL_SET_TAKES__	1

#if defined(__STDC__) || defined(mips) || defined(_IBMR2)
#define _TAKES(x) x
#else /* __STDC__ */
#define _TAKES(x) ()
#endif /* __STDC__ */

#endif	/* _TAKES */

extern char * cmdIter_NextData _TAKES((
				       cmd_dataIter_t *		cmdIter
				     ));
extern char *  cmdIter_FirstData _TAKES((
					 cmd_dataIter_t *		cmdIter,
					 int			paramIndex,
					 struct cmd_syndesc *	aTs
				       ));

extern void SetUpPathSwitch _TAKES((
				    struct cmd_syndesc *	aSyntax,
				    int				type,
				    unsigned long		flags
				  ));

/* parameter checking routines that the user can use for validation */
extern int cmd_param_CheckStringNumeric _TAKES((
						char *		argStringP,
						unsigned long	flags
					       ));

extern int cmd_param_ValidateDecimalOrHexString _TAKES((
						char *		argStringP,
						unsigned long	flags
					       ));

/* try not to interfere with other declarations of the macro */
#if defined(__CMDAUX_INCL_SET_TAKES__)
#undef _TAKES
#endif

#endif /* TRANSARC_UI_CMDAUX_H */

