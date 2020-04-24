/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_afsFiles.c,v $
 * Revision 1.1.55.1  1996/10/02  17:04:20  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:52  damon]
 *
 * Revision 1.1.47.1  1994/10/20  19:29:39  sasala
 * 	Implement S12Y in bosserver subcomponent -UYHP: 12618
 * 	[1994/10/20  18:15:19  sasala]
 * 
 * Revision 1.1.45.3  1994/07/13  22:19:51  devsrc
 * 	merged with DCE1.1
 * 	[1994/07/11  15:17:33  devsrc]
 * 
 * 	merged with bl-10
 * 	[1994/06/28  19:58:27  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:53:53  mbs]
 * 
 * Revision 1.1.45.2  1994/06/09  13:52:14  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:55  annie]
 * 
 * Revision 1.1.45.2  1994/06/09  13:52:14  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:25:55  annie]
 * 
 * Revision 1.1.45.1  1994/02/04  20:06:19  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:06:40  devsrc]
 * 
 * Revision 1.1.43.1  1993/12/07  17:12:58  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  12:56:29  jaffe]
 * 
 * Revision 1.1.3.5  1993/01/18  19:55:47  cjd
 * 	Embedded copyright notice
 * 	[1993/01/18  19:31:54  cjd]
 * 
 * Revision 1.1.3.4  1993/01/13  15:53:11  shl
 * 	Transarc delta: comer-ot6332-open-log-first 1.1
 * 	  Selected comments:
 * 	    Moved code to check DFS directories after place where log is initialized.
 * 	    Used to log a message is there was a problem AND there wasn't a
 * 	    problem last time.  Now it does it whenever it detects a problem
 * 	    and when the state goes from bad to good.
 * 	[1993/01/12  18:35:30  shl]
 * 
 * Revision 1.1.3.3  1992/11/24  15:40:41  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:08:42  bolinger]
 * 
 * Revision 1.1.3.2  1992/08/31  18:32:16  jaffe
 * 	Replace missing RCS id 'Header'
 * 	[1992/08/31  13:56:39  jaffe]
 * 
 * 	Transarc delta: bab-ot4423-bosserver-remove-BOSSVR_LOCAL_DEBUG 1.1
 * 	  Selected comments:
 * 	    All uses of BOSSVR_LOCAL_DEBUG are obsolete (and some of them
 * 	    got in the way of debugging).  They were removed.
 * 	    ot 4423
 * 	    See above.
 * 	Transarc delta: jaffe-ot3211-fix-prototypes 1.1
 * 	  Selected comments:
 * 	    Remove ANSI-C declaration files which contained prototypes for standard
 * 	    functions.  We expect these prototypes to be defined by the system include
 * 	    files.
 * 	    use stdio instead of stdioDecls.h
 * 	[1992/08/28  20:48:46  jaffe]
 * 
 * Revision 1.1  1992/01/19  02:49:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  bossvr_afsFiles.c -- routines for dealing with a set of files and directories
 * set up by the bosserver
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>


RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bosserver/bossvr_afsFiles.c,v 1.1.55.1 1996/10/02 17:04:20 damon Exp $")

#include <dcedfs/osi.h>
#include <pwd.h>
#include <bossvr_afsFiles.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <bbos_pathnames.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbssmac.h>
#include <dfsbsssvc.h>
#include <dfsbssmsg.h>

struct bossvr_bosEntryStats {
    char *path;				/* pathname to check */
    int dir;				/* 1=>dir or 0=>file */
    int owner;				/* required owner */
    int reqPerm;			/* required permissions */
    int proPerm;			/* prohibited permissions */
    char ownerName[17];			/* name of required owner - "" means no owner required */
};

#ifdef AFS_SUNOS5_ENV
PRIVATE struct bossvr_bosEntryStats bossvr_bosEntryStats[] = 
{/*	 path			dir?	owner	reqP	proP 	ownerName*/
    {dce_base_dir,		1,	-1,	0755, 	02,	"bin"},
    {dce_subcomponents_dir,	1,	-1,	0755,	02,	"bin"},
    {afs_bin_dir,		1,	-1,	0755,	02,	"bin"},
    {afs_server_config_dir,	1,	-1,	0701,	02,	"bin"},
    {afs_server_admin_dir,	1,	-1,	0755,	02,	"bin"},
    {afs_bossvr_users_file,	0,	-1,	0600,	03,	"root"}
};

#else /* AFS_SUNOS5_ENV */

PRIVATE struct bossvr_bosEntryStats bossvr_bosEntryStats[] = 
{/*	 path			dir?	owner	reqP	proP 	ownerName*/
    {dce_base_dir,		1,	-1,	0755, 	02,	"root"},
    {dce_subcomponents_dir,	1,	-1,	0755,	02,	"root"},
    {afs_bin_dir,		1,	-1,	0755,	02,	"root"},
    {afs_server_config_dir,	1,	-1,	0701,	02,	"root"},
    {afs_server_admin_dir,	1,	-1,	0755,	02,	"root"},
    {afs_bossvr_users_file,	0,	-1,	0600,	03,	"root"}
};
#endif /* AFS_SUNOS5_ENV */

PRIVATE int bossvr_nbosEntryStats =
    sizeof(bossvr_bosEntryStats) / sizeof(bossvr_bosEntryStats[0]);


/*
 * CheckOwner - Enforces the owner of the entry as specified in the table.  If 
 * the entry is -1, it is filled in with the proper uid value.  This function
 * returns 1 if the ownership is proper w.r.t. 'realOwner'; 0 otherwise.
 *
 * No need to worry about multithreaded access to structure or getpwent() since
 * the bosserver is single-threaded.
 */
int CheckOwner(stats, realOwner)
  IN struct bossvr_bosEntryStats *stats;
  IN int realOwner;
{
    struct passwd *entryP;
    int rtnVal = 1;

    if (stats->owner == -1) {
	/* need to look up the ownerName */
	/* if ownerName is empty, there is no ownership requirement */
	if (stats->ownerName[0] != '\0') {
	    entryP = getpwnam(stats->ownerName);
	    if (entryP == (struct passwd *)0) {
		/* no entry in the password file */
		bossvr_SvcLog(bss_s_user_not_in_pw_file, stats->ownerName);
		rtnVal = 0;
	    }
	    else {
		/* got an owner */
		stats->owner = (int)entryP->pw_uid;
		rtnVal = (realOwner == stats->owner);
	    }
	}
    }
    else {
	rtnVal = (realOwner == stats->owner);
    }

    return rtnVal;
}


/*
 * StatEachEntry - If it's not there, it is okay.  If anything else goes wrong
 * complain.  Otherwise check permissions: shouldn't allow write or (usually)
 * read.
 */
PRIVATE int StatEachEntry (stats)
  IN struct bossvr_bosEntryStats *stats;
{
  struct stat	info;
  int		rights;
  int		rtnVal = 1;	/* assume success or no entry */
  
  if (stat (stats->path, &info)) {
    if (errno != ENOENT) {
      rtnVal = 0;
    }
  }
  else {
    rights = (info.st_mode & 0000777);

    if ((((info.st_mode & S_IFDIR) != 0) != stats->dir) ||	/* not expected type */
	!CheckOwner(stats, (int)info.st_uid) || 		/* incorrect owner */
			/* required permissions not present: */
	((rights & stats->reqPerm) != stats->reqPerm) ||
	((rights & stats->proPerm) != 0)) {	/* prohibited permissions present */
      rtnVal = 0;
    }
  }
  return rtnVal;
}

/*
 * DirAccessOK - checks the mode bits on the /usr/afs dir and decendents and
 * returns 0 if some are not OK and 1 otherwise.  For efficiency, it doesn't do
 * this check more often than every 5 seconds.
 */

EXPORT int DirAccessOK ()
{
    static unsigned long	lastTime = 0;
    static int			lastResult = -1;
    unsigned long		now = osi_Time();
    int				result;
    int				i;

    if ((now - lastTime) >= 5){
      lastTime = now;
      
      result = 1;
      for (i = 0; (i < bossvr_nbosEntryStats) && (result != 0); i++) {
	struct bossvr_bosEntryStats *e = &bossvr_bosEntryStats[i];
	if (!StatEachEntry (e)) {
	  result = 0;
	}
      }
      
      if ((result == 0) || (result != lastResult)) {		/* log changes */
       if (result != 0)  {
	bossvr_SvcLog(bss_s_server_dir_access_ok);
       }
       else {
	bossvr_SvcLog(bss_s_server_dir_access_not_ok);
       }
      }
      lastResult = result;
    }
    
    return lastResult;
}


EXPORT int GetRequiredDirPerm (path)
  IN char *path;
{
  int	i;
  int	rtnVal = -1;
  int	entryFound = 0;
  
  for (i = 0; (i < bossvr_nbosEntryStats) && (!entryFound); i++) {
    if (strcmp (path, bossvr_bosEntryStats[i].path) == 0) {
      rtnVal = bossvr_bosEntryStats[i].reqPerm;
      entryFound = 1;
    }
  }
  
  return rtnVal;
}


EXPORT int GetRequiredOwner (path)
  IN char *path;
{
  int	i;
  int	rtnVal = -1;
  int	entryFound = 0;
  
  for (i = 0; (i < bossvr_nbosEntryStats) && (!entryFound); i++) {
    if (strcmp (path, bossvr_bosEntryStats[i].path) == 0) {
      rtnVal = bossvr_bosEntryStats[i].owner;
      if (rtnVal < 0) {
	/* force the resolution of name to uid */
	(void)CheckOwner(&bossvr_bosEntryStats[i], 0);
	rtnVal = bossvr_bosEntryStats[i].owner;
      }
      entryFound = 1;
    }
  }
  
  return rtnVal;
}

