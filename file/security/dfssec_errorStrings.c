/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfssec_errorStrings.c,v $
 * Revision 1.1.616.1  1996/10/02  20:59:19  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:37  damon]
 *
 * Revision 1.1.611.2  1994/06/09  14:19:54  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:38  annie]
 * 
 * Revision 1.1.611.1  1994/02/04  20:29:54  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:17:59  devsrc]
 * 
 * Revision 1.1.609.1  1993/12/07  17:33:35  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:56:37  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/21  15:19:54  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:58:32  cjd]
 * 
 * Revision 1.1.5.3  1992/11/24  19:40:47  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:24:51  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/31  21:14:11  jaffe
 * 	Transarc delta: jess-ot2869-com_err-usage-cleanup 1.5
 * 	  Selected comments:
 * 	    The use of com_err in dfs is inconsistent and in some cases
 * 	    incorrect.
 * 	    1) there are many sources which include <com_err.h> but don't
 * 	    use any definitions from it.
 * 	    2) there are sources which use com_err, but don't include the
 * 	    <com_err.h>
 * 	    3) there are subdirectories which do not need libcom_err.a for
 * 	    any reason that link with it.
 * 	    This delta is created to clean up the usage of com_err in dfs.
 * 	    Delete line "#include <com_err.h>
 * 	[1992/08/30  12:24:02  jaffe]
 * 
 * Revision 1.1.3.6  1992/07/07  21:57:41  mason
 * 	Transarc delta: bab-ot4457-security-dacl2acl-more-files 1.1
 * 	  Selected comments:
 * 	    More routines need to be moved into the dacl2acl library to
 * 	    support the new acl_edit work.
 * 	    ot 4457
 * 	    See above.
 * 	[1992/07/07  21:36:41  mason]
 * 
 * Revision 1.1.3.5  1992/04/06  02:22:12  garyf
 * 	fix compilation warnings
 * 	[1992/04/06  02:18:30  garyf]
 * 
 * Revision 1.1.3.4  1992/01/25  20:49:35  zeliff
 * 	dfs6.3 merge, part2
 * 	[1992/01/25  20:08:32  zeliff]
 * 
 * $EndLog$
 */
/*
 * dfssec_errorStrings.c - code to return string corresponding to error status
 */

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dce/rpcbase.h>
#include <dce/rpcsts.h>
#include <dce/dce_error.h>

#if !defined(KERNEL) && !defined(TRANSARC_DFSAUTH_EXCEPTION_PRINT_H)
#include <pthread.h>
#endif /* !KERNEL */



EXPORT char * RgySec_ErrorString(secErrorP)
     error_status_t *	secErrorP;
{
  char *		rtnValP = (char *)NULL;
  int			decodedString = 0;
  dce_error_string_t	defaultString;
  int			status = 0;
  
#if defined(DACL2ACL)
  rtnValP = (char *)malloc(sizeof(dce_error_string_t));
#else /* defined(DACL2ACL) */
  rtnValP = (char *)osi_Alloc(sizeof(dce_error_string_t));
#endif /* defined(DACL2ACL) */

  dce_error_inq_text(*secErrorP, (unsigned char *)rtnValP, &status);
  if (status == 0) {
    decodedString = 1;
  }
  else {
#if !defined(KERNEL)
    sprintf(rtnValP, "%s", strerror((int)(*secErrorP)));
    if (strncmp(rtnValP, "Unknown code", strlen("Unknown code")) != 0) {
      decodedString = 1;
    }
#endif /* !defined(KERNEL) */
  }

  return rtnValP;
}

EXPORT void Print_DCE_Error(stream, programName, routineName, routineErrorStatusP)
     FILE *		stream;
     char *		programName;
     char *		routineName;
     error_status_t *	routineErrorStatusP;
{
  char *	errorStringP = RgySec_ErrorString(routineErrorStatusP);
  
  fprintf(((stream) ? stream : stderr), "%s: %s error: %s.\n",
	  ((programName) ? programName : "(unknown program name)"),
	  ((routineName) ? routineName : "(unknown routine name)"),
	  RgySec_ErrorString(routineErrorStatusP));
  
#if defined(DACL2ACL)
  free(errorStringP);
#else /* defined(DACL2ACL) */
  osi_Free(errorStringP, sizeof(dce_error_string_t));
#endif /* defined(DACL2ACL) */
}

EXPORT char * SysErrString(errNumber)
     int	errNumber;
{
  return strerror (errNumber);
}

EXPORT char * Rpc_Authn_Service_Strings(authnService)
     long	authnService;
{
  static char	defaultString[BUFSIZ];
  
  if (authnService == rpc_c_authn_none) {
    return "rpc_c_authn_none";
  }
  else if (authnService == rpc_c_authn_dce_secret) {
    return "rpc_c_authn_dce_secret";	
  }
  else if (authnService == rpc_c_authn_dce_public) {
    return "rpc_c_authn_dce_public";
  }
  else {
    sprintf(defaultString, "unknown authn service: %#x", authnService);
    return defaultString;
  }
}

EXPORT char * Rpc_Authz_Service_Strings(authzService)
     long	authzService;
{
  static char	defaultString[BUFSIZ];
  
  if (authzService == rpc_c_authz_none) {
    return "rpc_c_authz_none";
  }
  else if (authzService == rpc_c_authz_name) {
    return "rpc_c_authz_name";	
  }
  else if (authzService == rpc_c_authz_dce) {
    return "rpc_c_authz_dce";
  }
  else {
    sprintf(defaultString, "unknown authz service: %#x", authzService);
    return defaultString;
  }
}


     
