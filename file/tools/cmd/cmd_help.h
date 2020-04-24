/*-
 * @TRANSARC_COPYRIGHT@
 *
 * $TALog: cmd_help.h,v $
 * Revision 1.1  1994/06/30  21:50:32  bab
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
 * The head file containing declarations for the help portion.
 * [added by delta bab-tools-cmd-add-interactive, r1.1]
 *
 * $EndTALog$
 */

#if !defined(__TRANSARC_CMD_HELP_H__)
#define __TRANSARC_CMD_HELP_H__ 1

#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/stds.h>

typedef void (*cmd_print_help_f_t) _TAKES((FILE * stream));
typedef struct cmd_help_entry {
  char *		subcmdP;
  char *		paramOptionP;
  char *		paramOptArgP;
  cmd_print_help_f_t	printHelpP;
} cmd_help_entry_t;

IMPORT void cmd_SetHelp _TAKES((
				cmd_help_entry_t * helpArrayP
			      ));

/*
 *  Unfortunately, we can't give the real prototype to the compiler here,
 * due to circular dependencies in the include files.
 *
 * IMPORT void cmd_ActivateInteraction _TAKES((struct cmd_syndesc * syntaxP));
 */

IMPORT void cmd_ActivateInteraction ();

#endif /* !defined(__TRANSARC_CMD_HELP_H__) */
