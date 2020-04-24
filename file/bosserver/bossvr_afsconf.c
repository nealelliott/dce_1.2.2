/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_afsconf.c,v $
 * Revision 1.1.19.1  1996/10/02  17:04:23  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:54  damon]
 *
 * Revision 1.1.11.1  1994/10/20  19:29:40  sasala
 * 	Implement S12Y in bosserver subcomponent -UYHP: 12618
 * 	[1994/10/20  18:15:21  sasala]
 * 
 * Revision 1.1.9.2  1994/07/13  22:19:52  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:34  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  19:58:28  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:54  mbs]
 * 
 * Revision 1.1.9.1  1994/06/09  13:52:16  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:59  annie]
 * 
 * Revision 1.1.9.1  1994/06/09  13:52:16  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:59  annie]
 * 
 * Revision 1.1.5.3  1993/01/18  19:55:55  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:32:09  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  15:40:54  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:08:52  bolinger]
 * 
 * 	Merging dfs5.11 to dfs6.3
 * 
 * Revision 1.1.2.2  1992/01/24  01:48:42  devsrc
 * 	Bring in dfs6.3 sources
 * 	[1992/01/22  19:30:29  devsrc]
 * 
 * Revision 1.1.3.2  91/10/31  15:58:38  devsrc
 * 	Merging dfs5.5 to dfs5.11
 * 
 * Revision 1.1.2.2  91/08/21  13:21:34  garyf
 * 	fix bad function prototype
 * 	[91/08/21  13:18:57  garyf]
 * 
 * $EndLog$
*/
/*
 *  bossvr_afsconf.c -- routines to very roughly approximate some of the afsconf routines
 * for use in bosserver testing (until the afsconf layer works for AFS 4)
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>


RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bosserver/bossvr_afsconf.c,v 1.1.19.1 1996/10/02 17:04:23 damon Exp $")

#define MAX_USERNAME_LENGTH	60

#include <bbos_util.h>
#include <bossvr_afsconf.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbssmac.h>
#include <dfsbsssvc.h>
#include <dfsbssmsg.h>

/* bosserver super-user info */
EXPORT int afsconf_AddUser(bossvr_confdir, name)
     struct afsconf_dir *	bossvr_confdir;
     char *			name;
{
  return BZNOTIMPL;
}

EXPORT int afsconf_DeleteUser(bossvr_confdir, name)
     struct afsconf_dir *	bossvr_confdir;
     char *			name;
{
  return BZNOTIMPL;
}

EXPORT int afsconf_GetNthUser(bossvr_confdir, number, nameP, length)
     struct afsconf_dir *	bossvr_confdir;
     int			number;
     char *			nameP;
     int			length;
{
#ifdef BOSSVR_TESTING
  char		userName[MAX_USERNAME_LENGTH];
  int		rtnVal = 0;

  if (length >= MAX_USERNAME_LENGTH) {
    if (number <= 10) {
       
      {
      char *dcesptr;
      dcesptr = dce_sprintf(bss_s_User_number_is, number);
      strcpy(userName , dcesptr);
      free(dcesptr);
      }
      strcpy(nameP, userName);
    }
    else {
      rtnVal = 1;
    }
  }
  else {
    rtnVal = 1;
  }
  
  return rtnVal;
#endif /* BOSSVR_TESTING */

  return BZNOTIMPL;
}

EXPORT int afsconf_SuperUser(bossvr_confdir, bosserverBindingP)
     struct afsconf_dir *	bossvr_confdir;
     rpc_binding_handle_t *	bosserverBindingP;
{
  return 1;	/* everybody wins for now */
}


/* key info */
EXPORT int afsconf_AddKey (bossvr_confdir, kvno, key)
     struct afsconf_dir *	bossvr_confdir;
     long			kvno;
     char			key[8];
{
  return BZNOTIMPL;
}

EXPORT int afsconf_DeleteKey (bossvr_confdir, kvno)
     struct afsconf_dir *	bossvr_confdir;
     long			kvno;
{
  return BZNOTIMPL;
}

EXPORT int afsconf_GetKeys (bossvr_confdir, keys)
     struct afsconf_dir *	bossvr_confdir;
     struct afsconf_keys *	keys;
{
  return BZNOTIMPL;
}

/* cell info */
EXPORT int afsconf_GetCellInfo (bossvr_confdir, cellName, service, cellInfoP)
     struct afsconf_dir *	bossvr_confdir;
     char *			cellName;
     char *			service;
     struct afsconf_cell *	cellInfoP;
{
  return BZNOTIMPL;
}

EXPORT int afsconf_GetLocalCell(bossvr_confdir, cellNameP, length)
     struct afsconf_dir *	bossvr_confdir;
     char *			cellNameP;
     int			length;
{
#ifdef BOSSVR_TESTING
  int	rtnVal = 0;

  if (length >= strlen("transarc.com")) {
    strcpy(cellNameP, "transarc.com");
  }
  else {
    rtnVal = 1;
  }

  return rtnVal;
#endif /* BOSSVR_TESTING */

  return BZNOTIMPL;
}

EXPORT int afsconf_GetNoAuthFlag (bossvr_confdir)
     struct afsconf_dir *	bossvr_confdir;
{
  return BZNOTIMPL;
}

EXPORT int afsconf_SetCellInfo (path, cellInfoP)
     char *			path;
     struct afsconf_cell *	cellInfoP;
{
  return BZNOTIMPL;
}

EXPORT int afsconf_SetNoAuthFlag(bossvr_confdir, flag)
     struct afsconf_dir *	bossvr_confdir;
     long			flag;
{
  return BZNOTIMPL;
}

/* and an extra one just for kicks */
EXPORT int ka_KeyCheckSum(theKey, theCheckSum)
     char *		theKey;
     unsigned long *	theCheckSum;
{
  return BZNOTIMPL;
}





