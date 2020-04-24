/*-
 * @TRANSARC_COPYRIGHT@
 *
 * $TALog: interactive_test.c,v $
 * Revision 1.5  1995/03/03  21:48:26  bab
 * Added interface to testing command to test that required arguments are
 * collected before the opportunity to execute the command is offered.
 *
 * See above.
 * [from r1.4 by delta bab-tools-cmd-add-interactive, r1.9]
 *
 * Revision 1.4  1994/07/12  19:17:17  bab
 * Added special treatment for ambiguous option names: if the user types
 * an ambiguous string, the program now tells the user all the possible
 * disambiguations and asks the user to type more characters next time.  Also
 * made it possible for a user to go directly back into argument specification
 * from the "Execute this command..." message.
 *
 * Added a set of tests for option name disambiguation.
 * [from r1.3 by delta bab-tools-cmd-add-interactive, r1.5]
 *
 * Revision 1.3  1994/07/11  22:54:25  bab
 * Obtained a copy of the scrip[t that Jeff Kaminski wrote to process
 * the man pages and produce C structures used for printing interactive
 * help.  Also received an example file, produced by the script.  In checking
 * out the new example, found a bug to be corrected in cmd.c.
 *
 * Changed the initialization to use the array name generated
 * by the real script that produces the help structures.
 * [from r1.2 by delta bab-tools-cmd-add-interactive, r1.4]
 *
 * Revision 1.2  1994/07/11  21:23:21  bab
 * Added simple examples of interactive help, cribbed from an older version
 * of the dceback man page.  The "help" text isn't really accurate here,
 * but this was just for debugging purposes. More up-to-date versions, along
 * with the script to generate them from man pages will be coming along from
 * Jeff in the future.
 *
 * Added initialization of available interactive help to the initialization of
 * the cmd structures early in the program's execution.
 * [from r1.1 by delta bab-tools-cmd-add-interactive, r1.3]
 *
 * Revision 1.1  1994/07/08  21:53:57  bab
 * Adding program to test the interactive interface, based on dceback.  This
 * is a check-point version of this delta during merge checking/debugging.
 *
 * Skeletal command suite that does nothing except demonstrate/test the
 * interactive mode (but does not, as yet, include a demo of interactive help).
 * This code is based on the dceback interface.
 * [added by delta bab-tools-cmd-add-interactive, r1.2]
 *
 * $EndTALog$
 */

#if !defined(_NOIDENT) && !defined(lint)
static char *TARCSId = "$Id: interactive_test.c,v 1.5 1995/03/03 21:48:26 bab Exp $";
#endif

/*
 *  interactive_test.c -- routines to test out the interactive functionality
 * of the DFS command tool - based on the dceback interface
 *
 *
 *	Copyright (C) 1994 Transarc Corporation
 *	All rights reserved.
 *
 */

#include <stdio.h>

#include <dce/dce_cf_const.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>
#include <dcedfs/cmd_help.h>

#include <interactive_test.h>

#define DCEBACK_EXPOSE_SERVER_BASED_PARAMS 1

char programName[BUFSIZ];

typedef enum dceback_action 
{
  dceback_backup,
  dceback_restore
} dceback_action_t;

/* forward declarations for prototypes */
EXPORT int interactive_test_DoAmbiguous _TAKES((
					struct cmd_syndesc *	aSyntaxP,
					char *			aRockP
					      ));
PRIVATE void interactive_test_ui_SetUpAmbiguous _TAKES(( void ));

EXPORT int interactive_test_DoRequired _TAKES((
					struct cmd_syndesc *	aSyntaxP,
					char *			aRockP
					      ));
PRIVATE void interactive_test_ui_SetUpRequired _TAKES(( void ));

PRIVATE int ValidateAbsolutePath _TAKES((
					 char *		theStringP,
					 struct cmd_parmdesc *	theParamDescP
				       ));

PRIVATE void dceback_ui_SetUpCommonArgs _TAKES((
  struct cmd_syndesc *	aSyntaxP,
  dceback_action_t	action
  ));

PRIVATE void dceback_ui_SetUpCommonArgs(aSyntaxP, action)
  struct cmd_syndesc *	aSyntaxP;
  dceback_action_t	action;
{
  (void)cmd_IndexedAddParm(aSyntaxP, "-host", CMD_SINGLE, CMD_REQUIRED,
			   "machine_name", HOST_INDEX);

#if defined(DCEBACK_EXPOSE_SERVER_BASED_PARAMS)
  (void)cmd_AddParamHelp(aSyntaxP, "-host", "local", CMD_PARM_LITERAL);

  (void)cmd_IndexedAddParm(aSyntaxP, "-allownoencrypt", CMD_FLAG,
			   CMD_OPTIONAL, "", ALLOWNOENCRYPT_INDEX);
#endif /* defined(DCEBACK_EXPOSE_SERVER_BASED_PARAMS) */

  if (action == dceback_backup) {
    /* add the backup arguments */
    (void)cmd_IndexedAddParm(aSyntaxP, "-destfile", CMD_SINGLE,
			     CMD_REQUIRED, "local_pathname", DESTFILE_INDEX);
    (void)cmd_IndexedAddParm(aSyntaxP, "-stdout", CMD_FLAG,
			     CMD_REQUIRED, "", STDOUT_INDEX);
    (void)cmd_MakeAlternate(aSyntaxP, "-destfile", "-stdout");
  }
  else {
    /* add the restore arguments */
    (void)cmd_IndexedAddParm(aSyntaxP, "-sourcefile", CMD_SINGLE,
			     CMD_REQUIRED, "local_pathname", SOURCEFILE_INDEX);
    (void)cmd_IndexedAddParm(aSyntaxP, "-stdin", CMD_FLAG,
			     CMD_REQUIRED, "", STDIN_INDEX);
    (void)cmd_MakeAlternate(aSyntaxP, "-sourcefile", "-stdin");
  
    (void)cmd_IndexedAddParm(aSyntaxP, "-destdir", CMD_SINGLE,
			     CMD_OPTIONAL, "remote_pathname", DESTDIR_INDEX);
    cmd_AddValidationRoutine(aSyntaxP, "-destdir",
			     ValidateAbsolutePath);
  }
}

PRIVATE int ValidateAbsolutePath(theStringP, theParamDescP)
     char *			theStringP;
     struct cmd_parmdesc *	theParamDescP;
{
  int		rtnVal = 0;	/* assume success */

  if (*theStringP == PATH_COMPONENT_SEPARATOR) {
    rtnVal = 0;
  }
  else {
    rtnVal = 1;
  }
  
  return rtnVal;
}

PRIVATE void dceback_ui_SetUpBackupCDS()
{
  struct cmd_syndesc *	backupCDSSyntaxP;

  backupCDSSyntaxP = cmd_CreateSyntax("dumpcds", dceback_DoBackupCDS,
				      (char *)NULL,
				      "back up CDS data files from specified \
machine");

  dceback_ui_SetUpCommonArgs(backupCDSSyntaxP, dceback_backup);

  cmd_ActivateInteraction(backupCDSSyntaxP);
}

PRIVATE void dceback_ui_SetUpBackupDFS()
{
  struct cmd_syndesc *	backupDFSSyntaxP;

  backupDFSSyntaxP = cmd_CreateSyntax("dumpdfs", dceback_DoBackupDFS,
				      (char *)NULL,
				      "back up DFS data files from specified \
machine");

  (void)cmd_IndexedAddParm(backupDFSSyntaxP, "-component", CMD_LIST,
			   (CMD_OPTIONAL | CMD_PARM_LITERAL),
			   DFSTYPE_ADMIN_STRING, COMP_INDEX);
  (void)cmd_AddParamHelp(backupDFSSyntaxP, "-component", DFSTYPE_BAKDB_STRING,
			 CMD_PARM_LITERAL);
  (void)cmd_AddParamHelp(backupDFSSyntaxP, "-component", DFSTYPE_CONFIG_STRING,
			 CMD_PARM_LITERAL);
  (void)cmd_AddParamHelp(backupDFSSyntaxP, "-component", DFSTYPE_DFSTAB_STRING,
			 CMD_PARM_LITERAL);
  (void)cmd_AddParamHelp(backupDFSSyntaxP, "-component", DFSTYPE_FLDB_STRING,
			 CMD_PARM_LITERAL);

  dceback_ui_SetUpCommonArgs(backupDFSSyntaxP, dceback_backup);
  cmd_ActivateInteraction(backupDFSSyntaxP);
}

PRIVATE void dceback_ui_SetUpBackupMisc()
{
  struct cmd_syndesc *	backupMiscSyntaxP;

  backupMiscSyntaxP = cmd_CreateSyntax("dumpmisc", dceback_DoBackupMisc,
				       (char *)NULL,
			      "back up miscellaneous data files from specified \
machine");

  dceback_ui_SetUpCommonArgs(backupMiscSyntaxP, dceback_backup);
  cmd_ActivateInteraction(backupMiscSyntaxP);
}

PRIVATE void dceback_ui_SetUpBackupSecurity()
{
  struct cmd_syndesc *	backupSecuritySyntaxP;

  backupSecuritySyntaxP = cmd_CreateSyntax("dumpsecurity",
					   dceback_DoBackupSecurity,
					   (char *)NULL,
			      "back up security data files from specified \
machine");

  (void)cmd_IndexedAddParm(backupSecuritySyntaxP, "-component", CMD_LIST,
			   (CMD_OPTIONAL | CMD_PARM_LITERAL),
			   SECTYPE_COMMON_STRING, COMP_INDEX);
  (void)cmd_AddParamHelp(backupSecuritySyntaxP, "-component",
			 SECTYPE_MASTER_STRING, CMD_PARM_LITERAL);

  dceback_ui_SetUpCommonArgs(backupSecuritySyntaxP, dceback_backup);
  cmd_ActivateInteraction(backupSecuritySyntaxP);
}

PRIVATE void dceback_ui_SetUpRestoreCDS()
{
  struct cmd_syndesc *	restoreCDSSyntaxP;

  restoreCDSSyntaxP = cmd_CreateSyntax("restorecds", dceback_DoRestoreCDS,
				       (char *)NULL,
				      "restore CDS data files to specified \
machine");

  dceback_ui_SetUpCommonArgs(restoreCDSSyntaxP, dceback_restore);
  cmd_ActivateInteraction(restoreCDSSyntaxP);
}

PRIVATE void dceback_ui_SetUpRestoreDFS()
{
  struct cmd_syndesc *	restoreDFSSyntaxP;

  restoreDFSSyntaxP = cmd_CreateSyntax("restoredfs", dceback_DoRestoreDFS,
				       (char *)NULL,
				      "restore DFS data files to specified \
machine");

  (void)cmd_IndexedAddParm(restoreDFSSyntaxP, "-component", CMD_LIST,
			   (CMD_OPTIONAL | CMD_PARM_LITERAL),
			   DFSTYPE_ADMIN_STRING, COMP_INDEX);
  (void)cmd_AddParamHelp(restoreDFSSyntaxP, "-component", DFSTYPE_BAKDB_STRING,
			 CMD_PARM_LITERAL);
  (void)cmd_AddParamHelp(restoreDFSSyntaxP, "-component", DFSTYPE_CONFIG_STRING,
			 CMD_PARM_LITERAL);
  (void)cmd_AddParamHelp(restoreDFSSyntaxP, "-component", DFSTYPE_DFSTAB_STRING,
			 CMD_PARM_LITERAL);
  (void)cmd_AddParamHelp(restoreDFSSyntaxP, "-component", DFSTYPE_FLDB_STRING,
			 CMD_PARM_LITERAL);

  dceback_ui_SetUpCommonArgs(restoreDFSSyntaxP, dceback_restore);
  cmd_ActivateInteraction(restoreDFSSyntaxP);
}

PRIVATE void dceback_ui_SetUpRestoreMisc()
{
  struct cmd_syndesc *	restoreMiscSyntaxP;

  restoreMiscSyntaxP = cmd_CreateSyntax("restoremisc", dceback_DoRestoreMisc,
					(char *)NULL,
			      "restore miscellaneous data files to specified machine");

  dceback_ui_SetUpCommonArgs(restoreMiscSyntaxP, dceback_restore);
  cmd_ActivateInteraction(restoreMiscSyntaxP);
}

PRIVATE void dceback_ui_SetUpRestoreSecurity()
{
  struct cmd_syndesc *	restoreSecuritySyntaxP;

  restoreSecuritySyntaxP = cmd_CreateSyntax("restoresecurity",
					    dceback_DoRestoreSecurity,
					   (char *)NULL,
				      "restore security data files to \
specified machine");

  (void)cmd_IndexedAddParm(restoreSecuritySyntaxP, "-component", CMD_LIST,
			   (CMD_OPTIONAL | CMD_PARM_LITERAL),
			   SECTYPE_COMMON_STRING, COMP_INDEX);
  (void)cmd_AddParamHelp(restoreSecuritySyntaxP, "-component",
			 SECTYPE_MASTER_STRING,
			 CMD_PARM_LITERAL);

  dceback_ui_SetUpCommonArgs(restoreSecuritySyntaxP, dceback_restore);
  cmd_ActivateInteraction(restoreSecuritySyntaxP);
}

PRIVATE void interactive_test_ui_SetUpAmbiguous()
{
  struct cmd_syndesc *	ambiguousSyntaxP;

  ambiguousSyntaxP = cmd_CreateSyntax("ambiguous",
				      interactive_test_DoAmbiguous,
				      (char *)NULL,
				      "test ambiguous option resolution");
  cmd_IndexedAddParm(ambiguousSyntaxP, "-a", CMD_FLAG, CMD_OPTIONAL,
		     "ambiguous a option", 0);
  cmd_IndexedAddParm(ambiguousSyntaxP, "-aa", CMD_FLAG, CMD_OPTIONAL,
		     "ambiguous a option", 1);
  cmd_IndexedAddParm(ambiguousSyntaxP, "-aabbc", CMD_FLAG, CMD_OPTIONAL,
		     "ambiguous option", 2);
  cmd_IndexedAddParm(ambiguousSyntaxP, "-aabbd", CMD_FLAG, CMD_OPTIONAL,
		     "ambiguous option", 3);
  cmd_IndexedAddParm(ambiguousSyntaxP, "-aabbe", CMD_FLAG, CMD_OPTIONAL,
		     "ambiguous option", 4);

  cmd_ActivateInteraction(ambiguousSyntaxP);
}

PRIVATE void interactive_test_ui_SetUpRequired()
{
  struct cmd_syndesc *	ambiguousSyntaxP;

  ambiguousSyntaxP = cmd_CreateSyntax("required_test",
				      interactive_test_DoRequired,
				      (char *)NULL,
				      "test required argument checking");
  cmd_IndexedAddParm(ambiguousSyntaxP, "-rs0", CMD_SINGLE, CMD_REQUIRED,
		     "required single 0", 0);
  cmd_IndexedAddParm(ambiguousSyntaxP, "-os1", CMD_SINGLE, CMD_OPTIONAL,
		     "optional single 1", 1);
  cmd_IndexedAddParm(ambiguousSyntaxP, "-ol2", CMD_LIST, CMD_OPTIONAL,
		     "optional list 2", 2);
  cmd_IndexedAddParm(ambiguousSyntaxP, "-rl3", CMD_LIST, CMD_REQUIRED,
		     "required list 3", 3);
  cmd_IndexedAddParm(ambiguousSyntaxP, "-rs4", CMD_SINGLE, CMD_REQUIRED,
		     "required single 4", 4);

  cmd_ActivateInteraction(ambiguousSyntaxP);
}

EXPORT void dceback_ui_SetUpSyntax()
{
  IMPORT cmd_help_entry_t	help_array[];
  
  dceback_ui_SetUpBackupCDS();
  dceback_ui_SetUpBackupDFS();
  dceback_ui_SetUpBackupMisc();
  dceback_ui_SetUpBackupSecurity();

  dceback_ui_SetUpRestoreCDS();
  dceback_ui_SetUpRestoreDFS();
  dceback_ui_SetUpRestoreMisc();
  dceback_ui_SetUpRestoreSecurity();

  interactive_test_ui_SetUpAmbiguous();
  interactive_test_ui_SetUpRequired();
  
  cmd_SetHelp(help_array);
}

/*
 *  The stubs for the routines that would do the work.
 */
EXPORT int dceback_DoBackupCDS (aSyntaxP, aRockP)
  struct cmd_syndesc *	aSyntaxP;
  char *		aRockP;
{
  int		rtnVal = 0;	/* assume success */
  static char	routineName[] = "dceback_DoBackupCDS";
  
  fprintf(stdout, "%s: %s called.\n", programName, routineName);

  fprintf(stdout, "%s: %s exiting; returning %d.\n",
	  programName, routineName, rtnVal);
  return rtnVal;
}

EXPORT int dceback_DoBackupDFS (aSyntaxP, aRockP)
  struct cmd_syndesc *	aSyntaxP;
  char *		aRockP;
{
  int		rtnVal = 0;	/* assume success */
  static char	routineName[] = "dceback_DoBackupDFS";
  
  fprintf(stdout, "%s: %s called.\n", programName, routineName);

  fprintf(stdout, "%s: %s exiting; returning %d.\n",
	  programName, routineName, rtnVal);
  return rtnVal;
}

EXPORT int dceback_DoBackupMisc (aSyntaxP, aRockP)
  struct cmd_syndesc *	aSyntaxP;
  char *		aRockP;
{
  int		rtnVal = 0;	/* assume success */
  static char	routineName[] = "dceback_DoBackupMisc";
  
  fprintf(stdout, "%s: %s called.\n", programName, routineName);

  fprintf(stdout, "%s: %s exiting; returning %d.\n",
	  programName, routineName, rtnVal);
  return rtnVal;
}

EXPORT int dceback_DoBackupSecurity (aSyntaxP, aRockP)
  struct cmd_syndesc *	aSyntaxP;
  char *		aRockP;
{
  int		rtnVal = 0;	/* assume success */
  static char	routineName[] = "dceback_DoBackupSecurity";
  
  fprintf(stdout, "%s: %s called.\n", programName, routineName);

  fprintf(stdout, "%s: %s exiting; returning %d.\n",
	  programName, routineName, rtnVal);
  return rtnVal;
}

EXPORT int dceback_DoRestoreCDS (aSyntaxP, aRockP)
  struct cmd_syndesc *	aSyntaxP;
  char *		aRockP;
{
  int		rtnVal = 0;	/* assume success */
  static char	routineName[] = "dceback_DoRestoreCDS";
  
  fprintf(stdout, "%s: %s called.\n", programName, routineName);

  fprintf(stdout, "%s: %s exiting; returning %d.\n",
	  programName, routineName, rtnVal);
  return rtnVal;
}

EXPORT int dceback_DoRestoreDFS (aSyntaxP, aRockP)
  struct cmd_syndesc *	aSyntaxP;
  char *		aRockP;
{
  int		rtnVal = 0;	/* assume success */
  static char	routineName[] = "dceback_DoRestoreDFS";
  
  fprintf(stdout, "%s: %s called.\n", programName, routineName);

  fprintf(stdout, "%s: %s exiting; returning %d.\n",
	  programName, routineName, rtnVal);
  return rtnVal;
}

EXPORT int dceback_DoRestoreMisc (aSyntaxP, aRockP)
  struct cmd_syndesc *	aSyntaxP;
  char *		aRockP;
{
  int		rtnVal = 0;	/* assume success */
  static char	routineName[] = "dceback_DoRestoreMisc";
  
  fprintf(stdout, "%s: %s called.\n", programName, routineName);

  fprintf(stdout, "%s: %s exiting; returning %d.\n",
	  programName, routineName, rtnVal);
  return rtnVal;
}

EXPORT int dceback_DoRestoreSecurity (aSyntaxP, aRockP)
  struct cmd_syndesc *	aSyntaxP;
  char *		aRockP;
{
  int		rtnVal = 0;	/* assume success */
  static char	routineName[] = "dceback_DoRestoreSecurity";
  
  fprintf(stdout, "%s: %s called.\n", programName, routineName);

  fprintf(stdout, "%s: %s exiting; returning %d.\n",
	  programName, routineName, rtnVal);
  return rtnVal;
}

EXPORT int interactive_test_DoAmbiguous (aSyntaxP, aRockP)
  struct cmd_syndesc *	aSyntaxP;
  char *		aRockP;
{
  int		rtnVal = 0;	/* assume success */
  static char	routineName[] = "interactive_test_DoAmbiguous";
  
  fprintf(stdout, "%s: %s called.\n", programName, routineName);

  fprintf(stdout, "%s: %s exiting; returning %d.\n",
	  programName, routineName, rtnVal);
  return rtnVal;
}

EXPORT int interactive_test_DoRequired (aSyntaxP, aRockP)
  struct cmd_syndesc *	aSyntaxP;
  char *		aRockP;
{
  int		rtnVal = 0;	/* assume success */
  static char	routineName[] = "interactive_test_DoRequired";
  
  fprintf(stdout, "%s: %s called.\n", programName, routineName);

  fprintf(stdout, "%s: %s exiting; returning %d.\n",
	  programName, routineName, rtnVal);
  return rtnVal;
}

EXPORT int main (argc, argv)
  int		argc;
  char *	argv[];
{
  int		rtnVal = 0;
  long		cmdCode = 0;
  
  /* intialization and clean up code */
#if defined(notdef)
  (void)cmd_SetBeforeProc(InitProc, (char *)NULL);
  (void)cmd_SetAfterProc(TermProc, (char *)NULL);
#endif /* defined(notdef) */
  
  (void)strcpy(programName, argv[0]);
  
  dceback_ui_SetUpSyntax();
  
  /* now, let the cmd package figure out what needs to be done */
  cmdCode = cmd_Dispatch(argc, argv);
  if (cmdCode) {
    rtnVal = 1;
  }

  return rtnVal;
}
