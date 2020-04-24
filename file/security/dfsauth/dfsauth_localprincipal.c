/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsauth_localprincipal.c,v $
 * Revision 1.1.609.1  1996/10/02  20:58:58  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:24  damon]
 *
 * Revision 1.1.604.3  1994/07/13  22:26:34  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  21:14:00  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:01:44  mbs]
 * 
 * Revision 1.1.604.2  1994/06/09  14:19:32  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:26  annie]
 * 
 * Revision 1.1.604.1  1994/02/04  20:29:39  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:17:52  devsrc]
 * 
 * Revision 1.1.602.1  1993/12/07  17:33:20  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:55:11  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/21  15:19:18  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:57:27  cjd]
 * 
 * Revision 1.1.5.3  1992/11/24  18:32:20  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:24:00  bolinger]
 * 
 * Revision 1.1.5.2  1992/10/27  21:28:34  jaffe
 * 	Transarc delta: bab-ot5476-dfsauth-force-noauth-notify 1.3
 * 	  Selected comments:
 * 	    When it finds no default creds, the dfsauth package will now
 * 	    complete initialization in unauthenticated mode.  This delta also
 * 	    includes code to allow the bos command to take advantage of this
 * 	    feature.
 * 	    ot 5476
 * 	    Include compat error code header file.
 * 	    The first implementation wasn't really clean.  The place for the forced
 * 	    noauth mode is not really in dfsauth package, but in the client of that
 * 	    package.  The dfsauth package was, essentially restored; one routine was
 * 	    added.  And the burden of retrying in noauth mode if no credentials were
 * 	    found is moved to the end client.
 * 	    Need to set noauth for the name space, even in some cases in which
 * 	    noauth is not specified to rpc_locate_dfs_server.
 * 	[1992/10/27  14:59:01  jaffe]
 * 
 * Revision 1.1.3.8  1992/04/14  03:36:02  mason
 * 	Transarc delta: bab-ot2423-dfsauth-client-make-prin-name 1.1
 * 	  Files modified:
 * 	    security/dfsauth: dfsauth.p.h, dfsauth_client.c
 * 	    security/dfsauth: dfsauth_localprincipal.c
 * 	  Selected comments:
 * 	    dfsauth_client_InitBindingAuth needed a way to ensure that the string
 * 	    it tried to pass to rpc_binding_set_auth_info was a reasonable
 * 	    principal name (there are cases in which we might like to pass
 * 	    it a binding location, for example).  This code was added and some
 * 	    related miscellaneous code cleanup was done.
 * 	    ot 2423
 * 	    Removed implementation of dfsauth routine that duplicated ncscompat
 * 	    routine functionality.  Alos did some code cleanup.
 * 	[1992/04/13  17:33:31  mason]
 * 
 * Revision 1.1.3.7  1992/03/05  16:29:26  mason
 * 	Substituted osi_GetPid() call for getpid() call.
 * 	[1992/02/27  20:13:41  mason]
 * 
 * Revision 1.1.3.6  1992/02/04  15:12:49  delgado
 * 	Fix warning by enclosing comment after #endif with comment delimiters
 * 	[1992/02/04  15:12:14  delgado]
 * 
 * Revision 1.1.3.5  1992/01/29  21:58:00  carl
 * 	removed notdef section of code
 * 	[1992/01/29  21:57:25  carl]
 * 
 * Revision 1.1.3.4  1992/01/29  17:24:23  cfe
 * 	Assure that we're using the use_new_principal_names code.
 * 	[1992/01/29  17:22:21  cfe]
 * 
 * Revision 1.1.3.3  1992/01/24  03:48:29  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:19:11  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  20:23:31  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:39:25  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:21:42  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
 */
/*
 *	dfsauth_localprincipal.c -- routines for accessing the local principal for 
 * dfsauth package
 *
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>

#include <dce/id_base.h>
#include <dce/secidmap.h>

#if !defined(AFS_OSF_ENV) && !defined(AFS_HPUX_ENV)
IMPORT char * rindex _TAKES((
			     char *	s,
			     int	c
			   ));
#endif /* !AFS_OSF_ENV && !AFS_HPUX_ENV */

#include <dce/dce_cf.h>

IMPORT char * RgySec_ErrorString _TAKES((error_status_t * secErrorP));

#include <dfsauth_uuidmap.h>
#include <dfsauth.h>
#include <dfsauth_debug.h>
#include <dfsauth_trace.h>

#include <../exception_print.h>	/* and pthread stuff */

#include <dcedfs/compat_errs.h>
#include <dcedfs/compat_dfsnames.h>
#include <dcedfs/icl.h>

#include <dfsauth_localprincipal.h>

struct uuid_suffix_map {
  char *	uuidStringP;
  char *	suffixP;
} uuid_suffix_map;

struct uuid_suffix_map uuidSuffixMap[] = {
  { UUIDMAP_BOSSERVER_UUID,		"_bosserver" },
  { UUIDMAP_AUTHTEST_SERVER_UUID,	"_authtestserver" }
};


IMPORT struct icl_set *dfsauth_iclSetp;

static char * dfsauth_PrinName_GetSuffix(uuidStringP)
     char *	uuidStringP;
{
  char *	rtnVal = (char *)NULL;
  int		numberSuffixMaps = (sizeof(uuidSuffixMap) / sizeof(uuidSuffixMap[0]));
  int		i;

  for (i = 0; (rtnVal == (char *)NULL) && (i < numberSuffixMaps); i++) {
    if (strcmp(uuidStringP, uuidSuffixMap[i].uuidStringP) == 0) {
      rtnVal = uuidSuffixMap[i].suffixP;
    }
  }
  
  return rtnVal;
}

EXPORT long dfsauth_PrinName_MakePrincipalName(nameBufferP, length)
     char *	nameBufferP;
     int	length;
{
  long			rtnVal = 0;
  int			exceptionRaised = 0;
  char *		hostStubP = (char *)NULL;
  char *		tailP = (char *)NULL;
  char			tempBuffer[BUFSIZ];
  sec_rgy_name_t	cellName;
  sec_rgy_name_t	prinName;
  error_status_t	pgoError = error_status_ok;

  static char		routineName[] = "dfsauth_PrinName_MakePrincipalName";

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered, base name is: %s", routineName, nameBufferP));
  icl_Trace1(dfsauth_iclSetp, DFSAUTH_ICL_LOCALPRINCIPAL_TRACE_0 , ICL_TYPE_STRING, nameBufferP);

  rtnVal = compat_MakeDfsName(nameBufferP, tempBuffer, sizeof(tempBuffer),
			      compat_dfs_name_principal);

  if (rtnVal == 0) {
    if (strlen(tempBuffer) < (unsigned)length) {
      strcpy(nameBufferP, tempBuffer);
    }
    else {
      dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_PRINCIPAL,
	       ("%s: buffer too small, need at least %d characters; have %d characters",
		routineName, strlen(tempBuffer) + 1, length));
      icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_LOCALPRINCIPAL_PRINCIPAL_1 , ICL_TYPE_LONG, strlen(tempBuffer)+1, ICL_TYPE_LONG, length);
      rtnVal = DAUT_ERROR_BUFFER_TOO_SMALL;	/* not enough room in buffer */
    }
  }

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning %d, constructed name is: %s",
	    routineName, rtnVal, nameBufferP));
  icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_LOCALPRINCIPAL_TRACE_2 , ICL_TYPE_LONG, rtnVal, ICL_TYPE_STRING, nameBufferP);
  
  return rtnVal;
}

EXPORT long dfsauth_PrinName_GetName(baseNameP, uuidStringP, nameBufferP, length)
     char *	baseNameP;
     char *	uuidStringP;
     char *	nameBufferP;
     int	length;
{
  long		rtnVal = 0;
  char *	suffixP = (char *)NULL;
  static char	routineName[] = "dfsauth_PrinName_GetName";

  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s entered", routineName));
  icl_Trace0(dfsauth_iclSetp, DFSAUTH_ICL_LOCALPRINCIPAL_TRACE_3 );
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s: basename is %s, adding suffix? %s", routineName,
	    (baseNameP != (char *)NULL) ? baseNameP : "(local)",
	    (uuidStringP != (char *)NULL) ? "yes" : "no"));
  icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_LOCALPRINCIPAL_TRACE_4,
	     ICL_TYPE_STRING, ((baseNameP != (char*)NULL) ? baseNameP : "(local)"),
	     ICL_TYPE_STRING, ((uuidStringP!=(char*)NULL) ? "yes" : "no"));
  
  if (baseNameP != (char *)NULL) {
    strncpy(nameBufferP, baseNameP, length - 1);
  }
  else {
    rtnVal = dfsauth_PrinName_GetBaseName((char *)NULL, nameBufferP, length);
  }

  if (rtnVal == 0) {
    rtnVal = dfsauth_PrinName_MakePrincipalName(nameBufferP, length);

    if ((rtnVal == 0) && (uuidStringP != (char *)NULL)) {
      suffixP = dfsauth_PrinName_GetSuffix(uuidStringP);
      if (suffixP != (char *)NULL) {
	if ((strlen(suffixP) + strlen(nameBufferP)) < (unsigned)length) {
	  strcat(nameBufferP, suffixP);
	}
	else {
	  rtnVal = DAUT_ERROR_BUFFER_TOO_SMALL;	/* not enough room */
	}
      }
    }
  }
    
  dmprintf(dfsauth_debug_flag, DFSAUTH_DEBUG_BIT_TRACE,
	   ("%s exiting, returning principal %s, value %#x", routineName,
	    nameBufferP, rtnVal));
  icl_Trace2(dfsauth_iclSetp, DFSAUTH_ICL_LOCALPRINCIPAL_TRACE_5 , ICL_TYPE_STRING, nameBufferP, ICL_TYPE_LONG, rtnVal);

  return rtnVal;
}

EXPORT long dfsauth_PrinName_GetBaseName(filenameP, nameBufferP, length)
     char *	filenameP;
     char *	nameBufferP;
     int	length;
{
  unsigned long 	rtnVal = 0;
  char *		theHostNameP;

  dce_cf_get_host_name(&theHostNameP, &rtnVal);
  if (rtnVal == dce_cf_st_ok) {
    if (strlen(theHostNameP) < (unsigned)length) {
      (void)strcpy(nameBufferP, theHostNameP);
    }
    else {
      rtnVal = DAUT_ERROR_BUFFER_TOO_SMALL;
    }
    free(theHostNameP);
  } else if (rtnVal == dce_cf_e_file_open) {
    rtnVal = DAUT_ERROR_FS_OPEN;
  } else if (rtnVal == dce_cf_e_no_mem) {
    rtnVal = DAUT_ERROR_BUFFER_ALLOCATION;
  } else if (rtnVal == dce_cf_e_no_match) {
    rtnVal = DAUT_ERROR_LOCAL_HOST_DATABASE;
  }
  
  return rtnVal;
}
