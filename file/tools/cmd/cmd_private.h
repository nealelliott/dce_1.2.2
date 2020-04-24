/*-
 * @TRANSARC_COPYRIGHT@
 *
 * $TALog: cmd_private.h,v $
 * Revision 1.1  1994/07/12  19:17:17  bab
 * Added special treatment for ambiguous option names: if the user types
 * an ambiguous string, the program now tells the user all the possible
 * disambiguations and asks the user to type more characters next time.  Also
 * made it possible for a user to go directly back into argument specification
 * from the "Execute this command..." message.
 *
 * Wanted to be able to specify prototypes for routines that are
 * only supposed to be exported to other files within the cmd package.
 * [added by delta bab-tools-cmd-add-interactive, r1.5]
 *
 * $EndTALog$
 */

/*
 *    cmd_private.h:  header file containing declarations and definitions
 *  shared among the different parts of the cmd parsing pacakge, but that
 *  are not exported to clients of the cmd parsing package.
 */

#if !defined(__TRANSARC_CMD_PRIVATE_H__)
#define __TRANSARC_CMD_PRIVATE_H__ 1

#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include "cmd.h"
#include "cmdAux.h"
#include "cmd_help.h"

/* a couple of useful macros */
#define CMD_BIT(i)	(1L << i)
#define CMD_IS_BIT_SET(bit, set)	((CMD_BIT((bit)) & (set)) != 0)

#define INTERACTIVE_KEY_CHAR	'+'
#define TTY_CONNECTED (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))

IMPORT struct cmd_syndesc * cmd_FindSyntax _TAKES((
						   char *aname,
						   int *aambig
						 ));

IMPORT int cmd_FindParam _TAKES((
				 struct cmd_syndesc *	syntaxDescr,
				 char *			paramName
			       ));

IMPORT int cmd_FindAllParams _TAKES((
				     struct cmd_syndesc *	syntaxDescrP,
				     char *			paramNameP,
				     unsigned long *		altMatchesP
				   ));

#endif /* !defined(__TRANSARC_CMD_PRIVATE_H__) */
