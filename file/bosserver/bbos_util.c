/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *  bosutil.c -- implementations of utility routines used in the bosserver
 *
 *
 *	Copyright (C) 1995, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <bbos_util.h>

#include <dcedfs/osi.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#include <dcedfs/osi_net.h>
#include <time.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbssmac.h>
#include <dfsbsssvc.h>
#include <dfsbssmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/bosserver/RCS/bbos_util.c,v 4.16 1995/07/26 16:07:43 blake Exp $")

/* make ctime easier to use */
/* EXPORT */
char * bosutil_DateOf(atime)
     long atime;
{
  static char		tbuffer[30];
  register char *	tp;
  char *		newLinePos;

  tp = ctime(&atime);
  if (tp != (char *)NULL) {
    (void)strcpy(tbuffer, tp);
    if ((newLinePos = (char *)index(tbuffer, '\n')) != (char *)NULL) {
      *newLinePos = '\0';    /* get rid of new line */
    }
  }
  else {
    (void)strcpy(tbuffer, "BAD TIME");
  }

  return tbuffer;
}

#define AFS_PARTITION_PREFIX	"vicep"

/* EXPORT */
long bosutil_GetPartitionID(name)
     char *	name;
{
  long		rtnVal = -1;
  int		placeIndex = 0;

  if (name != (char *)NULL) {
    if (isdigit(name[0])) {
      rtnVal = atol(name);
    }
    else {
      if (name[placeIndex] == '/') placeIndex++;	/* skip over leading slash */
      if (strncmp(&(name[placeIndex]), AFS_PARTITION_PREFIX,
		  strlen(AFS_PARTITION_PREFIX)) == 0) {
	placeIndex += strlen(AFS_PARTITION_PREFIX);
      }

      /* now compute the mapping */
      switch(strlen(&(name[placeIndex]))) {
        case 1:
	  if (islower(name[placeIndex])) {
	    rtnVal = name[placeIndex] - 'a';
	  }
	  else {
	    rtnVal = -1;
	  }

	  break;
        case 2:
	  if ((islower(name[placeIndex])) && (islower(name[placeIndex + 1]))) {
	    rtnVal = ((name[placeIndex] - 'a') * 26 +
		      (name[placeIndex + 1] - 'a') + 26);
	  }
	  else {
	    rtnVal = -1;
	  }

	  break;
        default:
	  rtnVal = -1;
	  break;
      }

    }
  }

  return rtnVal;
}

#define LONGEST_PART_NAME	2
/* EXPORT */
char * bosutil_GetPartitionName(partIndex)
     long	partIndex;
{
  static char	partName[LONGEST_PART_NAME + 1];
  int		i;
  int		decade;			/* not really the right term, but it will do */
  int		offsetWithinDecade;

  /*
   * the cheapest (time) way to do this, of course, would be a table lookup, but
   * this will do for now - typing in the 27 * 26 table entries isn't my idea
   * of a good time
   */
  for (i = 0; i <= LONGEST_PART_NAME; i++) {
    partName[i] = '\0';
  }

  decade = partIndex / 26;
  offsetWithinDecade = partIndex % 26;

  if (decade == 0) {
    partName[0] = 'a' + offsetWithinDecade;
  }
  else {
    partName[0] = 'a' + (decade - 1);
    partName[1] = 'a' + offsetWithinDecade;
  }

  return partName;
}


/* EXPORT */
char * HostErrorString(hostError)
     int	hostError;
{
  static char		rtnVal[BUFSIZ];

  switch (hostError) {
    case HOST_NOT_FOUND:
      {
      char *dcesptr;
      dcesptr = (char *)dce_sprintf(bss_s_host_not_found);
      (void) strcpy(rtnVal , dcesptr);
      free(dcesptr);
      }
      break;
    case TRY_AGAIN:
      {
      char *dcesptr;
      dcesptr = (char *)dce_sprintf(bss_s_try_again);
      (void) strcpy(rtnVal , dcesptr);
      free(dcesptr);
      }
      break;
    case NO_RECOVERY:
      {
      char *dcesptr;
      dcesptr = (char *)dce_sprintf(bss_s_no_recovery);
      (void) strcpy(rtnVal , dcesptr);
      free(dcesptr);
      }
      break;
    case NO_ADDRESS:
      {
      char *dcesptr;
      dcesptr = (char *)dce_sprintf(bss_s_valid_host_but_no_IPaddr);
      (void) strcpy(rtnVal , dcesptr);
      free(dcesptr);
      }
      break;
    default:
      {
      char *dcesptr;
      dcesptr = (char *)dce_sprintf(bss_s_unrecognized_host_error, hostError);
      (void) strcpy(rtnVal , dcesptr);
      free(dcesptr);
      }
      break;
  }

  return rtnVal;
}

/* EXPORT */
char * bosutil_em(acode)
     long	acode;
{
  if (acode == -1)
    return "communications failure (-1)";
  else if (acode == -3)
    return "communications timeout (-3)";
  else return dfs_dceErrTxt(acode);
}

/*
 * take a name (e.g. foo/bar), and a dir (e.g. /usr/afs/bin), and construct
 * /usr/afs/bin/bar...though it is not really clear why the head should be stripped
 * from the given file name...and even if it should be, it should be documented
 */
/* EXPORT */
void bosutil_ComputeDestDir(aname, adir, aresult, alen)
     char *aname, *adir, *aresult;
     long alen;
{
  register char *tp;

  (void)strncpy(aresult, adir, alen);
  tp = (char *)rindex(aname, '/');
  if (tp == (char *)NULL) {
    /* no '/' in name */
    (void)strncat(aresult, "/", (alen - strlen(adir)));
    (void)strncat(aresult, aname, (alen - strlen(adir)));
  }
  else {
    /* tp points at the / character */
    (void)strncat(aresult, tp, (alen - strlen(adir)));
  }
}
