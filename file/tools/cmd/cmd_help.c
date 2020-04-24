/*-
 * @TRANSARC_COPYRIGHT@
 *
 * $TALog: cmd_help.c,v $
 * Revision 1.2  1994/07/12  19:17:16  bab
 * Added special treatment for ambiguous option names: if the user types
 * an ambiguous string, the program now tells the user all the possible
 * disambiguations and asks the user to type more characters next time.  Also
 * made it possible for a user to go directly back into argument specification
 * from the "Execute this command..." message.
 *
 * Use cmd_private.h for its prototypes.
 * [from r1.1 by delta bab-tools-cmd-add-interactive, r1.5]
 *
 * Revision 1.1  1994/06/30  21:50:31  bab
 * Imported the changes that had been made in an alpine configuration
 * to DFS files into a dfs configuration, so that they won't be lost this
 * time.
 *
 * This one implements a backwardly-compatible version of an interactive
 * version of the command parser.  This implementation has no effect on
 * packages that don't know about it or make use, so it requires
 * no changes to any other packages.  The interactive dialogue is EXTREMEMLY
 * similar to that used by the interactive mode of chpass (when no EDITOR
 * environment variable is set).
 *
 * The bulk of the implementation.
 * [added by delta bab-tools-cmd-add-interactive, r1.1]
 *
 * $EndTALog$
 */

#if !defined(_NOIDENT) && !defined(lint)
static char *TARCSId = "$Id: cmd_help.c,v 1.2 1994/07/12 19:17:16 bab Exp $";
#endif

#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <cmd.h>
#include <cmd_help.h>
#include <cmd_private.h>

PRIVATE struct cmd_parmdesc * cmd_FindParamForHelp _TAKES((
						   cmd_help_entry_t * helpEntryP
							 ));
IMPORT struct cmd_syndesc * cmd_FindSyntax _TAKES((
						   char *aname,
						   int *aambig
						 ));

/*
 *  This FindParam differs from the one in cmd.c in that it is going to return
 * a pointer to the appropriate param descriptor.
 */
PRIVATE struct cmd_parmdesc * cmd_FindParamForHelp (helpEntryP)
     cmd_help_entry_t *	helpEntryP;
{
  struct cmd_parmdesc *		rtnValP = (struct cmd_parmdesc *)NULL; /* assume failed */
  struct cmd_syndesc *		syntaxP = (struct cmd_syndesc *)NULL;
  int				paramIndex = -1;
  
  syntaxP = cmd_FindSyntax(helpEntryP->subcmdP, (int *)NULL);
  if (syntaxP != (struct cmd_syndesc *)NULL) {
    if ((paramIndex = cmd_FindParam(syntaxP, helpEntryP->paramOptionP)) != -1) {
      rtnValP = &(syntaxP->parms[paramIndex]);
    }
  }
  
  return rtnValP;
}

/*
 *  returned value is number of array entries for which no match could be found
 */
EXPORT void cmd_SetHelp (helpArrayP)
     cmd_help_entry_t * helpArrayP;
{
  int			i;
  struct cmd_parmdesc *	paramP;
  
  for (i = 0; helpArrayP[i].subcmdP != (char *)NULL; i++) {
    if ((paramP = cmd_FindParamForHelp(&(helpArrayP[i]))) != (struct cmd_parmdesc *)NULL) {
      paramP->helpEntryP = &(helpArrayP[i]);
    }
    /*
     *   The common args appear in the help array, too, so there isn't much point
     * in complaining about problems.
     */
  }
}

EXPORT void cmd_ActivateInteraction (syntaxP)
     struct cmd_syndesc * syntaxP;
{
  syntaxP->allowInteractive = 1;
}


