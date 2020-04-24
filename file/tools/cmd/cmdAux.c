/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmdAux.c,v $
 * Revision 1.1.11.1  1996/10/02  18:47:33  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:48:54  damon]
 *
 * Revision 1.1.6.2  1994/07/13  22:29:57  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:54:09  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:16  mbs]
 * 
 * Revision 1.1.6.1  1994/06/09  14:22:51  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:34:26  annie]
 * 
 * Revision 1.1.2.2  1993/01/21  15:54:57  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:38:18  cjd]
 * 
 * Revision 1.1  1992/01/19  02:50:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*
 *  cmdAux.c -- Implemenations of extra routines that are useful
 * when dealing with the AFS command parsing package
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <stdio.h>

#include <cmdAux.h>

char * cmdIter_NextData(cmdIter)
     cmd_dataIter_t *		cmdIter;
{
  char *	rtnVal = (char *)NULL;
  
  if (cmdIter->nextItem != (struct cmd_item *)NULL) {
    rtnVal = cmdIter->nextItem->data;
    cmdIter->nextItem = cmdIter->nextItem->next;
  }
  return (rtnVal);
}

char *  cmdIter_FirstData(cmdIter, paramIndex, aTs)
     cmd_dataIter_t *		cmdIter;
     int			paramIndex;
     struct cmd_syndesc *	aTs;
{
  cmdIter->theTs = aTs;
  cmdIter->nextItem = aTs->parms[paramIndex].items;

  return (cmdIter_NextData(cmdIter));
}

/* specific (often used) routines for setting up syntax descriptors */

void SetUpPathSwitch(aSyntax, type, flags)
     struct cmd_syndesc *	aSyntax;
     int			type;
     unsigned long		flags;
{
  (void)cmd_AddParm(aSyntax, "-path", type, flags,
		    "filename");
  (void)cmd_AddParamHelp(aSyntax, "-path", "directory_name", 0);
}


