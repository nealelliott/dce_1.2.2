/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: compat_dfsnames.c,v $
 * Revision 1.1.72.1  1996/10/02  17:54:22  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:07  damon]
 *
 * Revision 1.1.67.3  1994/07/13  22:28:36  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:41:32  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:51  mbs]
 * 
 * Revision 1.1.67.2  1994/06/09  14:13:15  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:26:27  annie]
 * 
 * Revision 1.1.67.1  1994/02/04  20:22:40  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:54  devsrc]
 * 
 * Revision 1.1.65.1  1993/12/07  17:28:03  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:49:13  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/19  16:08:13  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  15:11:40  cjd]
 * 
 * Revision 1.1.5.3  1992/11/24  17:55:00  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:09:53  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/27  21:15:23  jaffe
 * 	Transarc delta: bab-ot5476-dfsauth-force-noauth-notify 1.3
 * 	  Selected comments:
 * 	    When it finds no default creds, the dfsauth package will now
 * 	    complete initialization in unauthenticated mode.  This delta also
 * 	    includes code to allow the bos command to take advantage of this
 * 	    feature.
 * 	    ot 5476
 * 	    Include the error code header file.
 * 	    The first implementation wasn't really clean.  The place for the forced
 * 	    noauth mode is not really in dfsauth package, but in the client of that
 * 	    package.  The dfsauth package was, essentially restored; one routine was
 * 	    added.  And the burden of retrying in noauth mode if no credentials were
 * 	    found is moved to the end client.
 * 	    Need to set noauth for the name space, even in some cases in which
 * 	    noauth is not specified to rpc_locate_dfs_server.
 * 	[1992/10/27  14:36:07  jaffe]
 * 
 * Revision 1.1.3.4  1992/01/29  19:23:29  cfe
 * 	ensure that dfs-server principal names are used properly
 * 	[1992/01/29  19:22:50  cfe]
 * 
 * Revision 1.1.3.3  1992/01/24  03:46:07  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:14:30  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:51:16  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:35:35  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:18:12  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
 */
/*
 *	compat_dfsnames.c -- routines for interconverting DFS pathnames
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/debug.h>
#include <dcedfs/osi.h>

#include <compat_errs.h>
#include <compat_dfsnames.h>
#include <compat_debug.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#define	DFS_PRINCIPAL_SUFFIX	"/dfs-server"
#define DFS_BINDING_SUFFIX	"/self"

#if	!defined(AFS_HPUX_ENV)
IMPORT char * rindex _TAKES((
			     char *	s,
			     int	c
			   ));
#endif	/* !AFS_HPUX_ENV */
/*
 *  compat_MakeDfsName - takes a string and produces another string corresponding
 * to the input string and the type of DFS name required (binding or principal).
 * If the input string is zero-length or composed of only slashes ('/'), the string
 * is considered to be malformed and an error is returned.  If there is not enough
 * room in the output buffer to compose a name of the desired type, an error code
 * is returned.
 *
 * Parameters:
 *	baseNameP :	pointer to NULL-terminated name that is to be transformed
 * into a principal or binding name
 *	bufferP :	pointer to array of characters in which the constructed name
 * will be left
 *	bufferSize:	number of characters available in the output buffer (bufferP)
 *	desiredNameType: the type of name to be constructed in bufferP, dfs_name_binding
 * names end with the suffix "/self" and dfs_name_principal names end with the
 * suffix "/dfs-server"
 */
EXPORT long compat_MakeDfsName(baseNameP, bufferP, bufferSize, desiredNameType)
     char *			baseNameP;
     char *			bufferP;
     unsigned int		bufferSize;
     compat_dfs_name_t		desiredNameType;
{
  long		rtnVal = 0;
  char *	lastSlashP = (char *)NULL;
  char *	firstSuffixCharP = (char *)NULL;
  char *	desiredSuffixP = ((desiredNameType == compat_dfs_name_binding) ?
				  DFS_BINDING_SUFFIX :
				  DFS_PRINCIPAL_SUFFIX);
  char *	otherSuffixP = ((desiredNameType == compat_dfs_name_binding) ?
				DFS_PRINCIPAL_SUFFIX :
				DFS_BINDING_SUFFIX);
  static char	routineName[] = "compat_MakeDfsName";

  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_TRACE,
	   ("%s entered, base name is: %s", routineName, ((baseNameP) ? baseNameP : "")));

  if ((baseNameP != (char *)NULL) && (bufferP != (char *)NULL)) {
    if (strlen(baseNameP) < bufferSize) {
      strcpy(bufferP, baseNameP);
      
      /* back up over trailing slashes */
      lastSlashP = rindex(bufferP, '/');
      while ((strlen(lastSlashP) == 1) && (lastSlashP != (char *)NULL)) {
	*lastSlashP = '\0';
	lastSlashP = rindex(bufferP, '/');
      }
      
      if (lastSlashP != (char *)NULL) {
	if (strcmp(lastSlashP, desiredSuffixP) == 0) {
	  /* we already have what we want */
	  firstSuffixCharP = (char *)NULL;
	}
	else if (strcmp(lastSlashP, otherSuffixP) == 0) {
	  /* the string has the opposite suffix */
	  firstSuffixCharP = lastSlashP;
	}
	else {
	  /* start the suffix writing over the \0 */
	  firstSuffixCharP = &(bufferP[strlen(bufferP)]);
	}
      }
      else {
	if (strlen(bufferP)) {
	  /* no slashes, so start writing at the end, over the \0 */
	  firstSuffixCharP = &(bufferP[strlen(bufferP)]);
	}
	else {
	  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_PRINCIPAL,
		   ("%s: no characters left in path after removing trailing slashes",
		    routineName));
	  rtnVal = COMPAT_ERROR_MALFORMED_HOSTNAME;	/* malformed name */
	}
      }
      
      if (firstSuffixCharP != (char *)NULL) {
	*firstSuffixCharP = '\0';
	if ((strlen(bufferP) + strlen(desiredSuffixP)) < bufferSize) {
	  strcat(bufferP, desiredSuffixP);
	}
	else {
	  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_PRINCIPAL,
		   ("%s: buffer too small, need at least %d characters; have %d characters",
		    routineName, strlen(bufferP) + strlen(desiredSuffixP) + 1, bufferSize));
	  rtnVal = COMPAT_ERROR_BUFFER_TOO_SMALL;	/* not enough room in buffer */
	}
      }
    }
    else {
      dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_PRINCIPAL,
	       ("%s: buffer too small, need at least %d characters; have %d characters",
		routineName, strlen(baseNameP) + 1, bufferSize));
      *bufferP = '\0';		/* make it possible to print on exit */
      rtnVal = COMPAT_ERROR_BUFFER_TOO_SMALL;	/* not enough room in buffer */
    }
  }
  else {
    if (baseNameP == (char *)NULL) {
      compat_LogParamError(routineName, "baseNameP", 1, __FILE__, __LINE__);
    }
    if (bufferP == (char *)NULL) {
      compat_LogParamError(routineName, "bufferP", 1, __FILE__, __LINE__);
    }
    
    rtnVal = COMPAT_ERROR_PARAMETER_ERROR;
  }
  
  dmprintf(compat_debug_flag, COMPAT_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %d, constructed name is: %s",
	    routineName, rtnVal, ((bufferP) ? bufferP : "")));

  return rtnVal;
}
