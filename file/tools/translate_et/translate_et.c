/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/*
 * HISTORY
 * $Log: translate_et.c,v $
 * Revision 1.1.550.1  1996/10/02  18:48:44  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:18  damon]
 *
 * Revision 1.1.545.2  1994/06/09  14:23:02  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:34:37  annie]
 * 
 * Revision 1.1.545.1  1994/02/04  20:33:07  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:19:13  devsrc]
 * 
 * Revision 1.1.543.1  1993/12/07  17:35:57  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  17:19:19  jaffe]
 * 
 * Revision 1.1.4.3  1993/01/21  15:55:49  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:39:13  cjd]
 * 
 * Revision 1.1.4.2  1992/11/24  19:48:01  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:32:15  bolinger]
 * 
 * Revision 1.1.2.3  1992/06/08  22:46:40  mason
 * 	Transarc delta: bab-translate_et 1.2
 * 	  Selected comments:
 * 	    It turns out that dce_error_inq_text actually puts some useful information
 * 	    into the error string, even when it returns an error code.  We now make
 * 	    use of that information.
 * 	    Print out the partially converted string, even if dce_error_inq_text
 * 	    claimed an error occurred in the conversion.
 * 	[1992/06/08  21:56:05  mason]
 * 
 * Revision 1.1.2.2  1992/04/21  16:15:25  mason
 * 	A new version of translate_et which works with DCE error tables.
 * 
 * 	$TALog: translate_et.c,v $
 * 	Revision 1.9  1994/11/07  17:57:10  cfe
 * 	[merge of changes from 1.6 to 1.8 into 1.7]
 * 
 * 	[*** log entries omitted ***]
 * 
 * Revision 1.7  1994/06/29  22:51:20  bab
 * 	Started the process of bringing deltas made against dfs files in the alpine
 * 	tree into a dfs configuration.  Changed translate_et to use the command
 * 	parser and added a hex-or-decimal validation string to the list of validation
 * 	routines that the command parser maintains in its library.
 * 
 * 	Added code to allow compilation on all system types and added the use
 * 	of the command parser to parse and validate input strings.
 * 	[from r1.6 by delta bab-tools-translate_et-make-user-cmd-parser, r1.1]
 * 
 * Revision 1.6  1993/06/07  13:32:46  ota
 * 	As a convenience translate_et should interpret standard system errnos.
 * 
 * 	Call strerror to translate error codes less than 256 that dce_inq_text
 * 	  barfs on.
 * 	[from r1.5 by delta ota-db3649-translate_et-print-errnos-too, r1.1]
 * 
 * Revision 1.5  1993/01/29  15:06:17  jaffe
 * 	Pick up files from the OSF up to the 2.4 submission.
 * 	[from r1.4 by delta osf-revdrop-01-27-93, r1.1]
 * 
 * 	[1992/04/20  20:40:36  mason]
 * 
 * $EndLog$
 */

/*
 * COPYRIGHT 1993, 1992 Transarc Corporation, All Rights Reserved
 */

/*
 * This is the basic translate_et facility ported to use the newer DCE
 * error tables.
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

#include <dce/dce_error.h>

#define CODE_INDEX	0

char	programName[BUFSIZ];

#if defined(SUNOS4)
/*
 *   We have to use this one on SunOS4...it's not quite right, but we'll
 * have to live with it.
 */
IMPORT long strtol _TAKES((
			   char *	string,
			   char **	ptr,
			   int	base
			 ));

/* this one is in the library, but can't seem to find it in a header file */
IMPORT char * strerror _TAKES((int errnum));
#endif /* defined(SUNOS4) */

PRIVATE void translate_et_ui_SetUpSyntax _TAKES(( void ));

PRIVATE int translate_et_Main (aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int				rtnVal = 0;
  register struct cmd_item *	thisItemP;
  unsigned long			statusToConvert;
  dce_error_string_t		errorString;
  int				conversionStatus;
  char *			sysMsg;
  
  for (thisItemP = cmd_FirstItem(aSyntaxP, CODE_INDEX);
       thisItemP != (struct cmd_item *)NULL;
       thisItemP = thisItemP->next) {
    errorString[0] = '\0';
    conversionStatus = 0;

#if defined(SUNOS4)
    statusToConvert = (unsigned long)strtol(thisItemP->data, (char **)NULL, 0);
#else /* defined(SUNOS4) */
    statusToConvert = strtoul(thisItemP->data, (char **)NULL, 0);
#endif /* defined(SUNOS4) */
    dce_error_inq_text(statusToConvert, errorString, &conversionStatus);
    if (conversionStatus == 0) {
      fprintf(stdout, "%s: %s: %s\n", programName, thisItemP->data, errorString);
    }
    else {
      if (statusToConvert < 256) {
	sysMsg = strerror (statusToConvert);
	fprintf (stdout, "%s: %s: Standard system errno table gives: %s\n",
		 programName, thisItemP->data, sysMsg);
      }
      else {
	fprintf(stdout, "%s: unable to fully convert %s (%ld):\n\t%s\n",
		programName, thisItemP->data, statusToConvert, errorString);
      }
    }
  }

  return rtnVal;
}

PRIVATE void translate_et_ui_SetUpSyntax()
{
  struct cmd_syndesc *	mainSyntaxP;

  mainSyntaxP = cmd_CreateSyntax("initcmd", translate_et_Main, (char *)NULL,
				 "translate DCE and system error codes into strings");
  (void)cmd_IndexedAddParm(mainSyntaxP, "-code", CMD_LIST, CMD_REQUIRED,
			   "decimal_or_0x-prefixed_hex_number", CODE_INDEX);
  cmd_AddValidationRoutine(mainSyntaxP, "-code",
			   cmd_param_ValidateDecimalOrHexString);
}

int main(argc, argv)
     int	argc;
     char *	argv[];
{
  int			rtnVal = 0;
  int			cmdCode = 0;
  
  (void)strncpy(programName, argv[0], sizeof(programName));
  translate_et_ui_SetUpSyntax();
  cmdCode = cmd_Dispatch(argc, argv);
  if (cmdCode) {
    if ( cmd_IsCmdError(cmdCode) ) {
      /*
	com_err(argv[0], cmdCode, "command dispatch failed");
	*/
    }
    rtnVal = 1;
  }
  
  return rtnVal;
}
