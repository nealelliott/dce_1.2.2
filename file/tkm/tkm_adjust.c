/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 *      Copyright (C) 1992 1993 1994 Transarc Corporation
 *      All rights reserved.
 */

#include <stdio.h>
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/syscall.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>
#include <dce/dce_error.h>

#include <tkm_tokens.h>
#include <tkm_userint.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tkm/RCS/tkm_adjust.c,v 1.15 1994/11/07 16:58:35 cfe Exp $")

char programName[BUFSIZ];

int tkm_DoGetExpirationInterval (struct cmd_syndesc *	aSyntaxP,
				 char *			aRockP)
{
  int		rtnVal = 0;
  long		newInterval = -1;
  long		currentInterval;

  rtnVal = afs_syscall(AFSCALL_TKM, TKMOP_SET_EXP,
			(int)&newInterval, (int)&currentInterval, 0, 0);
  if (rtnVal == 0) {
    fprintf(stdout, "Current token expiration interval is %ld.\n", currentInterval);
  }
  else {
    perror("getting expiration interval");
  }

  return rtnVal;
}

int tkm_DoSetExpirationInterval (struct cmd_syndesc *	aSyntaxP,
				 char *			aRockP)
{
  int		rtnVal = 0;
  long		newInterval;
  long		currentInterval;

  newInterval = atol(cmd_FirstData(aSyntaxP, SET_EXP_NEW_EXP_INDEX));
  rtnVal = afs_syscall(AFSCALL_TKM, TKMOP_SET_EXP,
			(int)&newInterval, (int)&currentInterval, 0, 0);
  if (rtnVal == 0) {
    fprintf(stdout, "Token expiration interval set from %ld to %ld.\n",
	    currentInterval, newInterval);
  }
  else {
    perror("setting expiration interval");
  }

  return rtnVal;
}

int tkm_DoGetTokenQuota (struct cmd_syndesc *	aSyntaxP,
			 char *			aRockP)
{
  int		rtnVal = 0;
  long		newQuota = -1;
  long		currentQuota;

  rtnVal = afs_syscall(AFSCALL_TKM, TKMOP_SET_MAX_TOKENS, (int)&newQuota,
			(int)&currentQuota, 0, 0);
  if (rtnVal == 0) {
    fprintf(stdout, "Current token quota is %ld.\n", currentQuota);
  }
  else {
    perror("getting token quota");
  }

  return rtnVal;
}

int tkm_DoSetTokenQuota (struct cmd_syndesc *	aSyntaxP,
			 char *			aRockP)
{
  int		rtnVal = 0;
  long		newQuota;
  long		currentQuota;

  newQuota = atol(cmd_FirstData(aSyntaxP, SET_MAXTOKENS_INDEX));
  rtnVal = afs_syscall(AFSCALL_TKM, TKMOP_SET_MAX_TOKENS, (int)&newQuota,
			(int)&currentQuota, 0, 0);
  if (rtnVal == 0) {
      fprintf(stdout, "Token quota set from %ld to %ld.\n", currentQuota, newQuota);
  }
  else {
    perror("setting token quota");
  }

  return rtnVal;
}

int tkm_DoGetMinTokens (struct cmd_syndesc *	aSyntaxP,
			char *			aRockP)
{
  int		rtnVal = 0;
  long		newQuota = -1;
  long		currentQuota;

  rtnVal = afs_syscall(AFSCALL_TKM, TKMOP_SET_MIN_TOKENS, (int)&newQuota,
			(int)&currentQuota, 0, 0);
  if (rtnVal == 0) {
    fprintf(stdout, "Minimum number of free tokens is %ld.\n", currentQuota);
  }
  else {
    perror("getting minimum free tokens");
  }

  return rtnVal;
}

int tkm_DoSetMinTokens (struct cmd_syndesc *	aSyntaxP,
			char *			aRockP)
{
  int		rtnVal = 0;
  long		newQuota;
  long		currentQuota;

  newQuota = atol(cmd_FirstData(aSyntaxP, SET_MINTOKENS_INDEX));
  rtnVal = afs_syscall(AFSCALL_TKM, TKMOP_SET_MIN_TOKENS, (int)&newQuota,
			(int)&currentQuota, 0, 0);
  if (rtnVal == 0) {
      fprintf(stdout, "Minimum number of free tokens set from %ld to %ld.\n",
	      currentQuota, newQuota);
  }
  else {
    perror("setting minimum number of free tokens");
  }

  return rtnVal;
}

int tkm_DoGetMaxFiles (struct cmd_syndesc *	aSyntaxP,
		       char *			aRockP)
{
  int		rtnVal = 0;
  long		newQuota = -1;
  long		currentQuota;

  rtnVal = afs_syscall(AFSCALL_TKM, TKMOP_SET_MAX_FILES, (int)&newQuota,
			(int)&currentQuota, 0, 0);
  if (rtnVal == 0) {
    fprintf(stdout, "File structures quota is %ld.\n", currentQuota);
  }
  else {
    perror("getting file structures quota");
  }

  return rtnVal;
}

int tkm_DoSetMaxFiles (struct cmd_syndesc *	aSyntaxP,
		       char *			aRockP)
{
  int		rtnVal = 0;
  long		newQuota;
  long		currentQuota;

  newQuota = atol(cmd_FirstData(aSyntaxP, SET_MAXFILES_INDEX));
  rtnVal = afs_syscall(AFSCALL_TKM, TKMOP_SET_MAX_FILES, (int)&newQuota,
			(int)&currentQuota, 0, 0);
  if (rtnVal == 0) {
      fprintf(stdout, "File structures quota set from %ld to %ld.\n",
	      currentQuota, newQuota);
  }
  else {
    perror("setting file structures quota");
  }

  return rtnVal;
}

int tkm_DoGetMaxVols (struct cmd_syndesc *	aSyntaxP,
		      char *			aRockP)
{
  int		rtnVal = 0;
  long		newQuota = -1;
  long		currentQuota;

  rtnVal = afs_syscall(AFSCALL_TKM, TKMOP_SET_MAX_VOLS, (int)&newQuota,
			(int)&currentQuota, 0, 0);
  if (rtnVal == 0) {
    fprintf(stdout, "Fileset structures quota is %ld.\n", currentQuota);
  }
  else {
    perror("getting fileset structures quota");
  }

  return rtnVal;
}

int tkm_DoSetMaxVols (struct cmd_syndesc *	aSyntaxP,
		      char *			aRockP)
{
  int		rtnVal = 0;
  long		newQuota;
  long		currentQuota;

  newQuota = atol(cmd_FirstData(aSyntaxP, SET_MAXVOLS_INDEX));
  rtnVal = afs_syscall(AFSCALL_TKM, TKMOP_SET_MAX_VOLS, (int)&newQuota,
			(int)&currentQuota, 0, 0);
  if (rtnVal == 0) {
      fprintf(stdout, "Fileset structures quota set from %ld to %ld.\n",
	      currentQuota, newQuota);
  }
  else {
    perror("setting fileset structures quota");
  }

  return rtnVal;
}



int main(int	argc,
	 char *	argv[])
{
  int			rtnVal = 0;
  dce_error_string_t	errorString;
  int			conversionStatus;

  osi_setlocale(LC_ALL, "");
  (void)strncpy(programName, argv[0], sizeof(programName));

  tkm_SetUpSyntax();
  rtnVal = cmd_Dispatch(argc, argv);
  if (rtnVal != 0) {
    if ( cmd_IsCmdError(rtnVal) ) {
      dce_error_inq_text(rtnVal, errorString, &conversionStatus);
      if (conversionStatus == 0) {
	fprintf(stdout, "%s: %ld: %s\n", programName, rtnVal, errorString);
      }
      else {
	fprintf(stderr, "%s: unable to convert %ld to a dce error string\n",
		programName, rtnVal);
      }
    }
  }

  return rtnVal;
}
