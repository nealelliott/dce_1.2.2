/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * cm.c -- routines that do the work required to execute a cm command
 *
 *
 *	Copyright (C) 1996, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/common_data.h>
#include <com_err.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>
#include <dcedfs/cm.h>
#include <dcedfs/compat.h>

#include <cm_userint.h>
#include <cm_sockaddr.h>

#include <dcedfs/ioctl.h>
#include <dcedfs/cm_serverpref.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfscmumac.h>
#include <dfscmusvc.h>
#include <dfscmumsg.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <langinfo.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/userInt/cm/RCS/cm.c,v 4.67 1996/08/19 19:16:25 andi Exp $")

#define LINEWIDTH 80

IMPORT int pioctl _TAKES((
			  char *		pathname,
			  unsigned int		ioctlNumber,
			  struct afs_ioctl *	ioctlData,
			  int			followSymlinkFlag
			));

#define CMUI_INIT_AFS_IOCTL(ioctlData)	\
  (ioctlData).in = (ioctlData).out = (caddr_t)NULL;	\
  (ioctlData).in_size = (ioctlData).out_size = 0

#define PIOCTL_UNUSED_STRING		((char *)NULL)
#define PIOCTL_UNUSED_INT		(0)
#define PIOCTL_NO_FOLLOW_SYMLINK	(0)
#define PIOCTL_FOLLOW_SYMLINK		(1)

/* BUFSIZ seems to come from stdio.h, and is about 4K on the RIOS,
 * at least.  I'm pretty puzzled by the use of this definition,
 * but it is at least far too large for CMUI_MAX_SERVERS (forces
 * various sizes not to fix in unsigned short length fields).
 */
#define CMUI_MAXSYSNAME			BUFSIZ
#define CMUI_MAX_CELL_NAME_LENGTH	BUFSIZ
#define CMUI_MAX_VOLUME_NAME_LENGTH	BUFSIZ
#define CMUI_MAX_SERVERS		1024	/* more reasonable size */

/* routines out of the AFS utility code */
IMPORT struct hostent * hostutil_GetHostByName _TAKES((
						       char * ahost
						     ));
IMPORT char * hostutil_GetNameByINet _TAKES((
					     struct in_addr	addr
					   ));

char		programName[BUFSIZ];

/* debugging */
PRIVATE long	cm_debug_flag = 0;
#define CM_DEBUG_BIT_CMD	1

dce_svc_handle_t cmu_svc_handle;


void initialize_svc()
{
  error_status_t st;

  cmu_svc_handle = dce_svc_register(cmu_svc_table, (idl_char *)"cmu", &st);
  if (st != svc_s_ok)
	fprintf(stderr, "Warning: Cannot register svc 0x%lx\n", st);

  dce_svc_set_progname(programName, &st);
  if (st != svc_s_ok)
	fprintf(stderr, "Warning: Cannot set program name 0x%lx\n", st);

  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);

  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
	fprintf(stderr, "Warning: Cannot route WARNING messages 0x%lx\n", st);

  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
	fprintf(stderr, "Warning: Cannot route ERROR messages 0x%lx\n", st);

  dfs_define_cmu_message_tables();

}


/* error message printing code lifted from AFS 3 version of fs command: */
void CMUI_PrintError(code, filename)
    int   code;
    char *filename;
{ /* CMUI_PrintError */
  char		prefixString[BUFSIZ];
  
    if (errno == EINVAL) {
	if (filename)
	    dce_svc_printf(CMU_S_INVALARG_MSG, programName, filename);
	else dce_svc_printf(CMU_S_INVALARG2_MSG, programName);
    }
    else if (errno == ENOENT) {
	if (filename) dce_svc_printf(CMU_S_ENOENT_MSG, programName, filename);
	else dce_svc_printf(CMU_S_ENOENT2_MSG, programName);
    }
    else if (errno == EROFS)  dce_svc_printf(CMU_S_EROFS_MSG, programName);
    else if (errno == EACCES) {
	if (filename) dce_svc_printf(CMU_S_EACCES_MSG, programName, filename);
	else dce_svc_printf(CMU_S_EACCES2_MSG, programName);
    }
    else {
	if (filename) 
 		dce_svc_printf(CMU_S_ERRORWITHFILENAME_MSG, programName, 
			filename, strerror(errno));
	else 
		dce_svc_printf(CMU_S_ERRORWOFILENAME_MSG, 
			programName, strerror(errno));
    }
} /* CMUI_PrintError */


/* routines that do the real work go here */

#define DEFAULT_PATH "."
/* if no parm specified in a particular slot, set parm to be "." instead */
static void SetDotDefault(anItemP)
     struct cmd_item **anItemP;
{
  register struct cmd_item *ti;
  
  if (*anItemP == (struct cmd_item *)NULL) {
    /* allocate an item representing "." */
    ti = (struct cmd_item *) malloc(sizeof(struct cmd_item));
    ti->next = (struct cmd_item *) 0;
    ti->data = (char *) malloc(sizeof(DEFAULT_PATH));
    strcpy(ti->data, DEFAULT_PATH);

    /* and push it into place */
    *anItemP = ti;
  }
    
}

char *	onOffStringTable[] = {
  "off",
  "on",
  ""
};


int StringToIndex(aSyntax, parmIndex, stringTable)
     struct cmd_syndesc *	aSyntax;
     int			parmIndex;
     char **			stringTable;
{
  char *	valStrP = cmd_FirstData(aSyntax, parmIndex);
  int		rtnVal = -1;
  int		index = 0;
  
  while ((strcmp(stringTable[index], "") != 0) && (rtnVal == -1)) {
    if (strcmp(stringTable[index], valStrP) == 0) {
      rtnVal = index;
    }
    index++;
  }
  
  return rtnVal;
}

/*
 * these routines determine the value returned by cmd_Dispatch (as long as
 * one of the routines is run - see comment in the init routine, above)
 */
int DoCheckFilesets(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  static char		routineName[] = "DoCheckFilesets";
  
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  CMUI_INIT_AFS_IOCTL(ioctlInfo);
  pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOCCKBACK, &ioctlInfo, PIOCTL_UNUSED_INT);
  if (pioctlCode == 0) {
    dce_svc_printf(CMU_S_CHECKBUFS_MSG);
  }
  else {
    rtnVal = 1;
    CMUI_PrintError(pioctlCode, (char *)NULL);
  }
  
  return rtnVal;
}

int DoFlush(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  struct cmd_item *	thisItemP;
  int			numFailures = 0;
  static char		routineName[] = "DoFlush";
  
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  numFailures = 0;
  SetDotDefault(&(aSyntax->parms[FLUSH_PATH_INDEX].items));
  CMUI_INIT_AFS_IOCTL(ioctlInfo);
  for (thisItemP = cmd_FirstItem(aSyntax, FLUSH_PATH_INDEX);
       thisItemP != (struct cmd_item *)NULL;
       thisItemP = thisItemP->next) {
    pioctlCode = pioctl(thisItemP->data, VIOCFLUSH, &ioctlInfo, PIOCTL_NO_FOLLOW_SYMLINK);
    if (pioctlCode != 0) {
      numFailures++;
      if (errno == EMFILE) {
	dce_svc_printf(CMU_S_CANTFLUSH_MSG, programName, thisItemP->data);
      }
      else {
	CMUI_PrintError(pioctlCode, thisItemP->data);
      }
    }
  }

  return numFailures;
}

int DoResetStores(as, arock)
struct cmd_syndesc *as;
char *arock; {
    struct afs_ioctl blob;
    register long code;

    CMUI_INIT_AFS_IOCTL(blob);
    code = pioctl((char *) 0, VIOCRESETSTORES, &blob, PIOCTL_FOLLOW_SYMLINK);
    if (code) {
	CMUI_PrintError(code, (char *)NULL);
    }
    return code;
}

int DoListStores(as, arock)
struct cmd_syndesc *as;
char *arock; {
    struct afs_ioctl blob;
    register long code;
    register long i;
    struct cm_liststores {
	long currentStores;
	long hyperCount;
	afs_hyper_t volIDs[50];
    } tbuffer;

    CMUI_INIT_AFS_IOCTL(blob);
    blob.out_size = sizeof(tbuffer);
    blob.out = (char *) &tbuffer;
    code = pioctl((char *) 0, VIOCLISTSTORES, &blob, PIOCTL_FOLLOW_SYMLINK);
    if (code) {
	CMUI_PrintError(code, (char *)NULL);
    }
    else {
	/* display the storing status.  The first word is the count of the total
	 * number of stores being retried at present, the second word is the count
	 * of the number of fileset IDs returned, and the remainder are the
	 * actual fileset IDs holding files being retried.
	 */
	if (tbuffer.currentStores == 0) {
	    dce_svc_printf(CMU_S_NORETRY_MSG);
	}
	else {
	    /* something is happening */
	    dce_svc_printf(CMU_S_RETRYFAILEDSTORES_MSG, tbuffer.currentStores);
	    for(i = 0; i < tbuffer.hyperCount; i++) {
		dce_svc_printf(CMU_S_FILESETNAME_MSG,
			       AFS_HGETBOTH(tbuffer.volIDs[i]));
	    }
	}
    }
    return code;
}

int DoFlushFileset(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  struct cmd_item *	thisItemP;
  int			numFailures = 0;
  static char		routineName[] = "DoFlushFileset";
  
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  numFailures = 0;
  SetDotDefault(&(aSyntax->parms[FLUSHVOL_PATH_INDEX].items));
  CMUI_INIT_AFS_IOCTL(ioctlInfo);
  for (thisItemP = cmd_FirstItem(aSyntax, FLUSHVOL_PATH_INDEX);
       thisItemP != (struct cmd_item *)NULL;
       thisItemP = thisItemP->next) {
    pioctlCode = pioctl(thisItemP->data, VIOC_FLUSHVOLUME, &ioctlInfo,
			PIOCTL_NO_FOLLOW_SYMLINK);
    if (pioctlCode != 0) {
      numFailures++;
      CMUI_PrintError(pioctlCode, thisItemP->data);
    }
  }

  return numFailures;
}

#define MAX_CACHE_PARMS_SIZE	16
int DoGetCacheInfo(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  long			cacheParms[MAX_CACHE_PARMS_SIZE];
  static char		routineName[] = "DoGetCacheInfo";
  
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  CMUI_INIT_AFS_IOCTL(ioctlInfo);

  ioctlInfo.out = (caddr_t)cacheParms;
  ioctlInfo.out_size = sizeof(cacheParms);
  
  pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOCGETCACHEPARMS,
		      &ioctlInfo, PIOCTL_UNUSED_INT);
  if (pioctlCode == 0) {
    if (cacheParms[2] == 0)
    	dce_svc_printf(CMU_S_DISKCACHEUSAGE_MSG, cacheParms[1], cacheParms[0]);
    else
    	dce_svc_printf(CMU_S_MEMCACHEUSAGE_MSG, cacheParms[1], cacheParms[0]);
    if (cacheParms[1] > cacheParms[0])
        dce_svc_printf(CMU_S_DELIBEXCEED_MSG);
  }
  else {
    rtnVal = 1;
    CMUI_PrintError(pioctlCode, (char *)NULL);    
  }

  return rtnVal;
}

int DoGetSetuid(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  struct cmd_item *	thisItemP;
  long			volStat;
  static char		routineName[] = "DoGetSetuid";
  
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  SetDotDefault(&(aSyntax->parms[GETSETUID_PATH_INDEX].items));

  CMUI_INIT_AFS_IOCTL(ioctlInfo);
  ioctlInfo.out = (caddr_t)(&volStat);
  ioctlInfo.out_size = sizeof(long);
  
  for (thisItemP = cmd_FirstItem(aSyntax, GETSETUID_PATH_INDEX);
       thisItemP != (struct cmd_item *)NULL;
       thisItemP = thisItemP->next) {
    pioctlCode = pioctl(thisItemP->data, VIOCGETVOLSTAT,
			&ioctlInfo, PIOCTL_FOLLOW_SYMLINK);

    if (pioctlCode == 0) {
      if (volStat & VIOC_DEF_SGIDOK) {
        dce_svc_printf(CMU_S_SETUIDALLOW_MSG, thisItemP->data);
      }
      else {
        dce_svc_printf(CMU_S_SETUIDNOTALLOW_MSG, thisItemP->data);
      }
    }
    else {
      if (errno == ENOENT) {
	dce_svc_printf(CMU_S_FSETNOEXIST_MSG, programName, thisItemP->data);
	rtnVal++;
      }
      else {
	rtnVal = 1;
	CMUI_PrintError(pioctlCode, thisItemP->data);
      }
    }
  }
  
  return rtnVal;
}

int DoGetDevOk(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  struct cmd_item *	thisItemP;
  long			volStat;
  static char		routineName[] = "DoGetDevOk";
  
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  SetDotDefault(&(aSyntax->parms[GETDEVOK_PATH_INDEX].items));

  CMUI_INIT_AFS_IOCTL(ioctlInfo);
  ioctlInfo.out = (caddr_t)(&volStat);
  ioctlInfo.out_size = sizeof(long);
  
  for (thisItemP = cmd_FirstItem(aSyntax, GETDEVOK_PATH_INDEX);
       thisItemP != (struct cmd_item *)NULL;
       thisItemP = thisItemP->next) {
    pioctlCode = pioctl(thisItemP->data, VIOCGETVOLSTAT,
			&ioctlInfo, PIOCTL_FOLLOW_SYMLINK);

    if (pioctlCode == 0) {
      if (volStat & VIOC_DEF_DEVOK) {
        dce_svc_printf(CMU_S_DEVICEALLOW_MSG, thisItemP->data);
      }
      else {
        dce_svc_printf(CMU_S_DEVICENOTALLOW_MSG, thisItemP->data);
      }
    }
    else {
      if (errno == ENOENT) {
	dce_svc_printf(CMU_S_FSETNOEXIST_MSG, programName, thisItemP->data);
	rtnVal++;
      }
      else {
	rtnVal = 1;
	CMUI_PrintError(pioctlCode, thisItemP->data);
      }
    }
  }
  return rtnVal;
}

int DoLsCells(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
    int			rtnVal = 0;
    struct afs_ioctl	ioctlInfo;
    long		pioctlCode;
    long		hostNumber = 0;
    char		outBuffer[osi_BUFFERSIZE];
    char *		hostNameP;
    int			i;
    int			endOfCellList = 0;
    char *		cellNameP;
    long		numbSockAddrs = 0;
    struct sockaddr *	hostSockaddrP;
  
    static char		routineName[] = "DoLsCells";
  
    dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	     ("%s entered."));

    CMUI_INIT_AFS_IOCTL(ioctlInfo);
    ioctlInfo.in = (caddr_t)(&hostNumber);
    ioctlInfo.in_size = sizeof(long);
    ioctlInfo.out = outBuffer;
    ioctlInfo.out_size = sizeof(outBuffer);

    while (!endOfCellList) {
	bzero(outBuffer, sizeof(outBuffer));

	pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOCGETCELL,
			    &ioctlInfo, PIOCTL_UNUSED_INT);
	if (pioctlCode == 0) {
	    bcopy(outBuffer, (char *)&numbSockAddrs, sizeof(long));
	    cellNameP = outBuffer + sizeof(long) +
		(numbSockAddrs * sizeof(struct sockaddr));
	    hostSockaddrP = (struct sockaddr *)(outBuffer + sizeof(long));

	    if (numbSockAddrs > 1) {
		dce_printf(cmu_s_lscells2, cellNameP);
	    } else {
		dce_printf(cmu_s_lscells1, cellNameP);
	    }

	    for (i = 0;  i < numbSockAddrs; i++) {
		if (hostSockaddrP[i].sa_family != AF_INET) {
		    dce_svc_printf(CMU_S_UNSUPADDR_MSG,
				   hostSockaddrP[i].sa_family);
		} else {
		    hostNameP = hostutil_GetNameByINet(((struct sockaddr_in *)(&hostSockaddrP[i]))->sin_addr);

		    if (((i+1) == numbSockAddrs)) {
			dce_printf(cmu_s_end, hostNameP);
		    } else {
			dce_printf(cmu_s_hostlist, hostNameP);
		    }
		}
	    }
	    dce_printf(cmu_s_newline);
	} else {
	    endOfCellList = 1;
	    if (errno != EDOM) {
		rtnVal = 1;
		CMUI_PrintError(pioctlCode, (char *)NULL);
	    }
	}
	hostNumber++;
    }
  
    return rtnVal;
}

int DoSetCacheSize(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  long			cacheParms[MAX_CACHE_PARMS_SIZE];
  long			newCacheSize = 0;
  int			resetToInit = 0;
  int			getLongCode = 0;
  char			newCacheSizeString[512];
  char *		s;
  unsigned long		maxLong = (unsigned)~0;
  static char		routineName[] = "DoSetCacheSize";
  
  maxLong = ((maxLong >> 1) -1);
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  CMUI_INIT_AFS_IOCTL(ioctlInfo);
  ioctlInfo.in = (caddr_t)(&newCacheSize);
  ioctlInfo.in_size = sizeof(long);
  ioctlInfo.out = (caddr_t)cacheParms;
  ioctlInfo.out_size = sizeof(cacheParms);

  /*
   * If the size parameter isn't present, the reset flag must be, so zero is
   * the right value for the new cache size, anyway.
   */
  if (cmd_IsParamPresent(aSyntax, SETCACHESIZE_SIZE_INDEX)) {
    getLongCode = 0;
    newCacheSize = strtol(cmd_FirstData(aSyntax, SETCACHESIZE_SIZE_INDEX),
			  &s, 0);
    if (newCacheSize != 0 ||
	s != cmd_FirstData(aSyntax, SETCACHESIZE_SIZE_INDEX)) {
      /*
       * Check for overflow, since the strtol routine doesn't do it and we
       * got a cm bug report on it.
       */
      sprintf(newCacheSizeString, "%u", newCacheSize);
      if (strcmp(newCacheSizeString,
		 cmd_FirstData(aSyntax, SETCACHESIZE_SIZE_INDEX)) != 0) {
	dce_svc_printf(CMU_S_CACHESIZE2BIG_MSG, programName, maxLong);
	getLongCode = 1;
      }
    }
    else {
      /*
       * String was not parsed as a number
       */
      dce_svc_printf(CMU_S_BADCACHESIZE_MSG, programName, 
		cmd_FirstData(aSyntax, SETCACHESIZE_SIZE_INDEX));
      getLongCode = 1;
    }
  }
  

  resetToInit = (newCacheSize == 0);
   
  if (getLongCode == 0) {
    pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOCSETCACHESIZE,
			&ioctlInfo, PIOCTL_UNUSED_INT);
    if (pioctlCode == 0) {
      if (resetToInit) {
	dce_svc_printf(CMU_S_RESETSIZE_MSG, programName);
      }
      else {
	/* Check if cache size was bumped up to a minimum usable size */
	/* If so, print message to user explaining difference         */
        pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOCGETCACHEPARMS,
			    &ioctlInfo, PIOCTL_UNUSED_INT);
	if (pioctlCode == 0) {
	  if (cacheParms[0] > newCacheSize) {
	    dce_svc_printf(CMU_S_ROUNDUPSIZE_MSG, programName, newCacheSize);
	  }
	  dce_svc_printf(CMU_S_NEWCACHESIZE_MSG, programName, cacheParms[0]);
	}
	else {
	  rtnVal = 1;
	  CMUI_PrintError(pioctlCode, (char *)NULL);
	}
      }
    }
    else {
      rtnVal = 1;
      CMUI_PrintError(pioctlCode, (char *)NULL);
    }
  }
  else {
    rtnVal = 1;
  }
  
  return rtnVal;
}

int DoSetDevOk(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  struct cmd_item *	thisItemP;
  long			currentVolStat = 0;
  long			newVolStat = 0;
  int			stateIndex = 1;
  static char		routineName[] = "DoSetDevOk";
  
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  if (cmd_IsParamPresent(aSyntax, SETDEVOK_STATE_INDEX)) {
    stateIndex = StringToIndex(aSyntax, SETDEVOK_STATE_INDEX, onOffStringTable);
    if (stateIndex == -1) {
      dce_svc_printf(CMU_S_BADDEVSTATE_MSG, programName, 
		cmd_FirstData(aSyntax, SETDEVOK_STATE_INDEX));
      rtnVal = 2;
    }
  }

  if (rtnVal == 0) {
    SetDotDefault(&(aSyntax->parms[SETDEVOK_PATH_INDEX].items));
    for (thisItemP = cmd_FirstItem(aSyntax, SETDEVOK_PATH_INDEX);
	 thisItemP != (struct cmd_item *)NULL;
	 thisItemP = thisItemP->next) {

      /* first, get the status, so we can avoid changing the setuid status */
      ioctlInfo.in = (caddr_t)NULL;
      ioctlInfo.in_size = 0;
      ioctlInfo.out = (caddr_t)(&currentVolStat);
      ioctlInfo.out_size = sizeof(long);
      pioctlCode = pioctl(thisItemP->data, VIOCGETVOLSTAT,
			  &ioctlInfo, PIOCTL_FOLLOW_SYMLINK);
      if (pioctlCode == 0) {
	newVolStat = ((stateIndex == 1) ?
		      (currentVolStat | VIOC_DEF_DEVOK) :
		      (currentVolStat & ~VIOC_DEF_DEVOK));

	/*  The second syscall may not really be necessary, but we have to do it
	 * to fix ot bug 4932.
	 */
	ioctlInfo.in = (caddr_t)(&newVolStat);
	ioctlInfo.in_size = sizeof(long);
	ioctlInfo.out = (caddr_t)NULL;
	ioctlInfo.out_size = 0;
	pioctlCode = pioctl(thisItemP->data, VIOCSETVOLSTAT,
			    &ioctlInfo, PIOCTL_FOLLOW_SYMLINK);
	
	if (pioctlCode != 0) {
	  rtnVal = 1;
	  CMUI_PrintError(pioctlCode, thisItemP->data);
	}
      }
      else {
	rtnVal = 1;
	CMUI_PrintError(pioctlCode, thisItemP->data);
      }
    }
  }
  
  return rtnVal;
}

int DoSetSetuid(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  struct cmd_item *	thisItemP;
  long			currentVolStat = 0;
  long			newVolStat = 0;
  int			stateIndex = 1;
  static char		routineName[] = "DoSetSetuid";
  
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  if (cmd_IsParamPresent(aSyntax, SETSETUID_STATE_INDEX)) {
    stateIndex = StringToIndex(aSyntax, SETSETUID_STATE_INDEX, onOffStringTable);
    if (stateIndex == -1) {
      dce_svc_printf(CMU_S_BADSETUIDSTATE_MSG,
	      programName, cmd_FirstData(aSyntax, SETSETUID_STATE_INDEX));
      rtnVal = 2;
    }
  }

  if (rtnVal == 0) {
    SetDotDefault(&(aSyntax->parms[SETSETUID_PATH_INDEX].items));
    for (thisItemP = cmd_FirstItem(aSyntax, SETSETUID_PATH_INDEX);
	 thisItemP != (struct cmd_item *)NULL;
	 thisItemP = thisItemP->next) {

      /* first, get the status, so we can avoid changing the devok status */
      ioctlInfo.in = (caddr_t)NULL;
      ioctlInfo.in_size = 0;
      ioctlInfo.out = (caddr_t)(&currentVolStat);
      ioctlInfo.out_size = sizeof(long);
      pioctlCode = pioctl(thisItemP->data, VIOCGETVOLSTAT,
			  &ioctlInfo, PIOCTL_FOLLOW_SYMLINK);
      if (pioctlCode == 0) {
	newVolStat = ((stateIndex == 1) ?
		      (currentVolStat | VIOC_DEF_SGIDOK) :
		      (currentVolStat & ~VIOC_DEF_SGIDOK));

	/*  The second syscall may not really be necessary, but we have to do it
	 * to fix ot bug 4932.
	 */
	ioctlInfo.in = (caddr_t)(&newVolStat);
	ioctlInfo.in_size = sizeof(long);
	ioctlInfo.out = (caddr_t)NULL;
	ioctlInfo.out_size = 0;
	pioctlCode = pioctl(thisItemP->data, VIOCSETVOLSTAT,
			    &ioctlInfo, PIOCTL_FOLLOW_SYMLINK);
	if (pioctlCode != 0) {
	  rtnVal = 1;
	  CMUI_PrintError(pioctlCode, thisItemP->data);
	}
      }
      else {
	rtnVal = 1;
	CMUI_PrintError(pioctlCode, thisItemP->data);
	
      }
    }
  }
  
  return rtnVal;
}

int DoStatServers(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  long			checkCodes = 0x2;	/* assume that check only local cell */
  char			inBuffer[CMUI_MAX_CELL_NAME_LENGTH + 1 + sizeof(long)];
  char *		nextInBuffer = inBuffer;
  int			i;
  int			j;
  char *		serverNameP;
  int			endOfHostList = 0;
  struct sockaddr	zerosockaddr;
  char			outBuffer[sizeof(long) +
				  (CMUI_MAX_SERVERS * sizeof(struct sockaddr))];
  struct in_addr	outAddrs[CMUI_MAX_SERVERS];
  int			outAddrCount;
  long			errorCode;
  struct sockaddr *	downServersP = (struct sockaddr *)(outBuffer + sizeof(long));
  struct sockaddr_in *	dip;	/* downservers internet pointer */
  char *		hostAddrP; /* char *, rather than long * because we have to
				      pull it out of a character buffer */
  
  static char		routineName[] = "DoStatServers";
  
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  /* initialize the input data for the pioctl */
  if (cmd_IsParamPresent(aSyntax, STATSERVERS_FAST_INDEX)) {
    checkCodes |= 0x1;
  }
  if (cmd_IsParamPresent(aSyntax, STATSERVERS_ALL_INDEX)) {
    checkCodes &= ~(0x2);	/* turn off local only checking */
  }
  bcopy((char *)(&checkCodes), nextInBuffer, sizeof(long));
  nextInBuffer += sizeof(long);
  
  /* now, if a cell name is given, copy it into the buffer */
  if (cmd_IsParamPresent(aSyntax, STATSERVERS_CELLNAME_INDEX)) {
    (void)strcpy(nextInBuffer, cmd_FirstData(aSyntax, STATSERVERS_CELLNAME_INDEX));
    nextInBuffer += (strlen(cmd_FirstData(aSyntax, STATSERVERS_CELLNAME_INDEX)) + 1);
  }
  
  /* init the output data for the pioctl */
  bzero((char *)outBuffer, sizeof(outBuffer));
  bzero((char *)&zerosockaddr, sizeof(struct sockaddr));

  CMUI_INIT_AFS_IOCTL(ioctlInfo);
  ioctlInfo.in = (caddr_t)inBuffer;
  ioctlInfo.in_size = nextInBuffer - inBuffer;
  ioctlInfo.out = (caddr_t)outBuffer;
  ioctlInfo.out_size = sizeof(outBuffer);

  pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOCCKSERV, &ioctlInfo, PIOCTL_UNUSED_INT);
  if (pioctlCode == 0) {
    bcopy(outBuffer, (char *)&errorCode, sizeof(long));
    if (errorCode == 0) {
      if (bcmp((char *)(&(downServersP[0])), (char *)&zerosockaddr,
	       sizeof(struct sockaddr)) == 0) {
	dce_svc_printf(CMU_S_ALLSVRRUN_MSG);
      }
      else {
	outAddrCount = 0;
	/* first scan the returned list, removing duplicates.  Duplicates
	 * can arise when two servers (e.g. file server and flserver)
	 * on the same machine are both down.
	 */
	for (i = 0; i < CMUI_MAX_SERVERS; i++) {
	  if (downServersP[i].sa_family != AF_UNSPEC) {
	    if (downServersP[i].sa_family == AF_INET) {
	      dip = (struct sockaddr_in *) (&downServersP[i]);
	      for(j=0; j<outAddrCount; j++) {
		/* check for duplicates */
		if (outAddrs[j].s_addr == dip->sin_addr.s_addr)
		  break;
	      }
	      if (j >= outAddrCount) {
		/* not found, add this dude in */
		outAddrs[outAddrCount++] = dip->sin_addr;
	      }
	    }
	    else {
	      dce_svc_printf(CMU_S_UNSUPADDR_MSG, downServersP[i].sa_family);
	    }
	  }
	  else break;
	}	/* for loop over down servers from kernel */
	for(i=0; i<outAddrCount; i++) {
	  serverNameP = hostutil_GetNameByINet(outAddrs[i]);
	  if ( i == 0)
		dce_svc_printf(CMU_S_SVRDOWN_MSG, serverNameP);
	  else if ( i == (outAddrCount-1))
	  	dce_svc_printf(CMU_S_END_MSG, serverNameP);
	  else
	  	dce_svc_printf(CMU_S_HOSTLIST_MSG, serverNameP);
	}
      }
    }
    else {
      if (errorCode == -1) {
	/* non-existent cell */
	dce_svc_printf(CMU_S_BADCELLNAME_MSG, 
		((cmd_IsParamPresent(aSyntax, STATSERVERS_CELLNAME_INDEX)) ?
		 cmd_FirstData(aSyntax, STATSERVERS_CELLNAME_INDEX): "(local cell)"));
	rtnVal = 1;
      }
      else {
	/* unknown error code */
	dce_svc_printf(CMU_S_BADKERNCODE_MSG, errorCode);
	rtnVal = 1;
      }
    }
  }
  else {
    rtnVal = 1;
    CMUI_PrintError(pioctlCode, (char *)NULL);
  }
  
  return rtnVal;
}

int DoSysname(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  int			rtnVal = 0;
  char			inSysnameCodeAndString[sizeof(long) + CMUI_MAXSYSNAME + 1];
  long			inSetSysnameCode =
    				cmd_IsParamPresent(aSyntax, SYSNAME_NEWSYS_INDEX);
  char *		inSetSysnameCodeBufferP;
  char *		inSysnameStringP;
  char			outSysnameCodeAndString[sizeof(long) + CMUI_MAXSYSNAME + 1];
  long			outSetSysnameCode = 0;
  char *		outSetSysnameCodeBufferP;
  char *		outSysnameStringP;
  static char		routineName[] = "DoSysname";
  
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  /* set up all the pointers into the buffers we will use */
  inSetSysnameCodeBufferP = inSysnameCodeAndString;
  inSysnameStringP = inSysnameCodeAndString + sizeof(long);
  outSetSysnameCodeBufferP = outSysnameCodeAndString;
  outSysnameStringP = outSysnameCodeAndString + sizeof(long);
  
  /* set up the ioctl structure */
  CMUI_INIT_AFS_IOCTL(ioctlInfo);

  ioctlInfo.in = (caddr_t)inSysnameCodeAndString;
  bcopy((char *)&inSetSysnameCode, inSetSysnameCodeBufferP, sizeof(long));

  if (inSetSysnameCode) {
    (void)strncpy(inSysnameStringP, cmd_FirstData(aSyntax, SYSNAME_NEWSYS_INDEX),
		  CMUI_MAXSYSNAME);
    ioctlInfo.in_size = (sizeof(long) +
			 strlen(cmd_FirstData(aSyntax, SYSNAME_NEWSYS_INDEX)) + 1);
  }
  else {
    ioctlInfo.in_size = sizeof(long);
    ioctlInfo.out = (caddr_t)outSysnameCodeAndString;
    ioctlInfo.out_size = sizeof(outSysnameCodeAndString);
  }

  /* call into the kernel to get/set the info we want */
  pioctlCode = pioctl(PIOCTL_UNUSED_STRING,
		      VIOC_AFS_SYSNAME, &ioctlInfo, PIOCTL_UNUSED_INT);
  if (pioctlCode == 0) {
    if (inSetSysnameCode) {
      dce_svc_printf(CMU_S_NEWSYSNAME_MSG, programName, inSysnameStringP);
    }
    else {
      bcopy(outSetSysnameCodeBufferP, (char *)&outSetSysnameCode, sizeof(long));
      if (outSetSysnameCode) {
	/* if the kernel put anything into the buffer */
	dce_svc_printf(CMU_S_CURSYSNAME_MSG, outSysnameStringP);
      }
      else {
	dce_svc_printf(CMU_S_NOSYSNAME_MSG);
      }
    }
  }
  else {
    CMUI_PrintError(pioctlCode, (char *)NULL);
    rtnVal = errno;
  }

  return rtnVal;
}

int DoWhereis(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
  int			rtnVal = 0;
  struct afs_ioctl	ioctlInfo;
  long			pioctlCode;
  struct cmd_item *	thisItemP;
  char			cellName[CMUI_MAX_CELL_NAME_LENGTH + 1];
  char *		serverName = (char *)NULL;
#ifdef oldcode
  struct afsFid		theFid;
#endif /* oldcode */
  int			i, j;
  int			cumlength;
  unsigned char		*printBuffer;
  unsigned char		*printPhrase;
  long			numbSockAddrs = 0;
  char			outBuffer[osi_BUFFERSIZE];
  struct sockaddr *	hostSockaddrP;
  char *		filesetNameP;
  
  static char		routineName[] = "DoWhereis";
  
  dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	   ("%s entered."));

  SetDotDefault(&(aSyntax->parms[WHEREIS_PATH_INDEX].items));
  for (thisItemP = cmd_FirstItem(aSyntax, WHEREIS_PATH_INDEX);
       thisItemP != (struct cmd_item *)NULL;
       thisItemP = thisItemP->next) {
    CMUI_INIT_AFS_IOCTL(ioctlInfo);

    /* first, get the cell on which the file resides */
    ioctlInfo.out = cellName;
    ioctlInfo.out_size = CMUI_MAX_CELL_NAME_LENGTH + 1;
    pioctlCode = pioctl(thisItemP->data, VIOC_FILE_CELL_NAME,
			&ioctlInfo, PIOCTL_FOLLOW_SYMLINK);
    if (pioctlCode != 0) {
      rtnVal++;
      CMUI_PrintError(pioctlCode, thisItemP->data);
      break;
    }

#ifdef oldcode    
    /* now, get the volume on which the file resides */
    /* HERE - we can't get the volume name for now, but we can get its id (yum yum!) */
    ioctlInfo.out = (caddr_t)(&theFid);
    ioctlInfo.out_size = sizeof(struct afsFid);
    pioctlCode = pioctl(thisItemP->data, VIOCGETFID,
			&ioctlInfo, PIOCTL_FOLLOW_SYMLINK);
    if (pioctlCode != 0) {
      rtnVal++;
      CMUI_PrintError(pioctlCode, thisItemP->data);
      break;
    }
#endif /* oldcode */

    /* now, get the list of hosts on which the file resides */
    bzero(outBuffer, sizeof(outBuffer));
    ioctlInfo.out = (caddr_t)outBuffer;
    ioctlInfo.out_size = sizeof(outBuffer);

    pioctlCode = pioctl(thisItemP->data, VIOCWHEREIS, &ioctlInfo, PIOCTL_FOLLOW_SYMLINK);
    if (pioctlCode != 0) {
      rtnVal++;
      CMUI_PrintError(pioctlCode, thisItemP->data);
      break;
    }

    bcopy(outBuffer, (char *)&numbSockAddrs, sizeof(long));
    hostSockaddrP = (struct sockaddr *)(outBuffer + sizeof(long));
    filesetNameP = outBuffer + sizeof(long) + (numbSockAddrs * sizeof(struct sockaddr));

    /* now, tell the user the results of the queries */
    /* All the gyrations below are done to ensure we don't 
     * wraparound. It implies message fragments in message
     * catalogs, but I don't see any way around it while still
     * ensuring the output is the same as before.  Also, the
     * fragments are duly documented, and contain at most a 
     * single new-line between and so should still be translatable (?).
     */
    printBuffer = dce_sprintf(cmu_s_fileresids, thisItemP->data);
    printPhrase = dce_sprintf(cmu_s_inthecell, cellName);
    dce_printf(cmu_s_fileresids, thisItemP->data);
    cumlength = strlen((char *)printBuffer) + strlen((char *)printPhrase);
    if (cumlength > LINEWIDTH) {
	dce_printf(cmu_s_newline);
	cumlength = strlen((char *)printPhrase);
    }
    dce_printf(cmu_s_inthecell, cellName);

    printBuffer = dce_sprintf(cmu_s_infileset, filesetNameP);
    cumlength += strlen((char *)printBuffer);
    if (cumlength > LINEWIDTH) {
	dce_printf(cmu_s_newline);
	cumlength = strlen((char *)printBuffer);
    }
    dce_printf(cmu_s_infileset, filesetNameP);

    for (i = 0; i < numbSockAddrs; i++) {
	if (hostSockaddrP[i].sa_family == AF_INET) {
	    serverName =
	      hostutil_GetNameByINet(((struct sockaddr_in *)hostSockaddrP)[i].sin_addr);
	    if (i == 0) {
		if (numbSockAddrs == 1) {
		    printBuffer = dce_sprintf(cmu_s_onhost, serverName);
		    cumlength += strlen((char *)printBuffer);
		    if (cumlength > LINEWIDTH) {
			dce_printf(cmu_s_newline);
			cumlength = strlen((char *)printBuffer);
		    }
		    dce_printf(cmu_s_onhost, serverName);
		    dce_printf(cmu_s_newline);
		} else {
		    printBuffer = dce_sprintf(cmu_s_onhosts, serverName);
		    cumlength += sizeof(printBuffer);
		    if (cumlength > LINEWIDTH) {
			dce_printf(cmu_s_newline);
			cumlength = strlen((char *)printBuffer);
		    }
		    dce_printf(cmu_s_onhosts, serverName);
		}
	    } else if (i == (numbSockAddrs-1)) {
		dce_printf(cmu_s_end, serverName);
		dce_printf(cmu_s_newline);
	    }
	    else
		dce_printf(cmu_s_hostlist, serverName);
	}
	else {
	    dce_svc_printf(CMU_S_UNSUPADDR_MSG, hostSockaddrP[i].sa_family);
	    rtnVal++;
	}
    }
  }
  
  return rtnVal;
}

int DoGetPrefs(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
    int			rtnVal = 0;
    struct afs_ioctl	ioctlInfo;
    long		pioctlCode, addr;
    int			resolve = 0, fldb = 0, i;
    FILE		*outfd;
    char		*fileName, *hostName, tbuffer[osi_BUFFERSIZE];
    struct sprefrequest	reqin;
    struct sprefinfo	*spout;
    static char		routineName[] = "DoGetPrefs";
  
    dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	     ("%s entered."));

     /* If output file given, make it stdout */
    if (cmd_IsParamPresent(aSyntax, GPREFS_PATH_INDEX) != 0) {
	fileName = cmd_FirstData(aSyntax, GPREFS_PATH_INDEX);
	dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD, ("%s Opening output file %s",
						   fileName));
	if (!(outfd = freopen(fileName, "w", stdout))) {
	    dce_svc_printf(CMU_S_CANT_OPEN_MSG,fileName);
	    return(1);
	}
    }

     /* If -numeric, do not resolve IP addr -> hostname */
    if (!cmd_IsParamPresent(aSyntax, GPREFS_NUMERIC_INDEX))
	resolve = 1;

    /* If -fldb, only report fldb servers */
    if (cmd_IsParamPresent(aSyntax, GPREFS_FLDB_INDEX))
	fldb = 1;

    /* set up the ioctl structure */
    CMUI_INIT_AFS_IOCTL(ioctlInfo);

    spout = (struct sprefinfo *)osi_Alloc(osi_BUFFERSIZE);
    reqin.offset = 0;

    do {
	 /* Subtract off fields of sprefinfo header and divide by spref size */
	reqin.num_servers =
	    (osi_BUFFERSIZE - (2 * sizeof(unsigned))) / sizeof(struct spref);

	ioctlInfo.in = (caddr_t)(&reqin);
	ioctlInfo.in_size = sizeof(struct sprefrequest);
	ioctlInfo.out = (char *)spout;
	ioctlInfo.out_size = osi_BUFFERSIZE;

	pioctlCode = pioctl(PIOCTL_UNUSED_STRING,
			    VIOC_GETSPREFS, &ioctlInfo,
			    PIOCTL_UNUSED_INT);
	if (pioctlCode != 0) {
	    rtnVal++;
	    CMUI_PrintError(pioctlCode, (char *)NULL);
	    break;
	}

	for (i=0; i<spout->num_servers; i++) {
	    u_int type = (spout->servers[i].flags & CM_PREF_MASK);
	    if (fldb) {
		if ((type != CM_PREF_FLDB) &&
		    (type != CM_PREF_FLDB + CM_PREF_QUEUED)) {
		    continue;  /* skip non-fldb entries if desired */
		}
	    } else {
		if ((type == CM_PREF_FLDB) ||
		    (type == CM_PREF_FLDB + CM_PREF_QUEUED)) {
		    continue; /* skip fldb entries if not asked for */
		}
	    }
	    if (resolve) {
		hostName =
		    hostutil_GetNameByINet(spout->servers[i].host.sin_addr);
	    } else {
		addr = ntohl(spout->servers[i].host.sin_addr.s_addr);
		sprintf(tbuffer, "%d.%d.%d.%d",
			(addr>>24) & 0xff,
			(addr>>16) & 0xff,
			(addr>>8) & 0xff,
			addr & 0xff);
		hostName = tbuffer;
	    }
	    dce_printf(cmu_s_host_rank,
	    	hostName, spout->servers[i].rank);
	    dce_printf(cmu_s_newline);
	}
	reqin.offset = spout->next_offset;
    } while (!pioctlCode && spout->next_offset > 0);

    osi_Free(spout, osi_BUFFERSIZE);
    return(rtnVal);
}

int DoSetPrefs(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
    int			rtnVal = 0;
    long		rank;
    FILE		*infd;
    char		*fileName, tbuffer[osi_BUFFERSIZE];
    struct cmd_item *	thisItemP;
    struct afs_ioctl	ioctlInfo;
    struct hostent	*thost;
    long		pioctlCode;
    struct spref	*sp;
    unsigned long	addr, incount;
    u_short		service = CM_PREF_FX;
    static char		routineName[] = "DoSetPrefs";
  
    dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD,
	     ("%s entered."));

    /* set up the ioctl structure */
    CMUI_INIT_AFS_IOCTL(ioctlInfo);

    ioctlInfo.in = (caddr_t)osi_Alloc(osi_BUFFERSIZE);
    sp = (struct spref *)ioctlInfo.in;
    incount = 0;

    /* If we got the -fldb switch, all ranks are for fldb servers */
    if (cmd_IsParamPresent(aSyntax, SPREFS_FLDB_INDEX) != 0) {
	service = CM_PREF_FLDB;
    }

     /* If input file given, get it and suck in all the file lines */
    if (cmd_IsParamPresent(aSyntax, SPREFS_PATH_INDEX) != 0) {
	fileName = cmd_FirstData(aSyntax, SPREFS_PATH_INDEX);
	dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD, ("%s Opening input file %s",
						   routineName, fileName));
	if (!(infd = fopen(fileName, "r"))) {
	    dce_svc_printf(CMU_S_CANT_OPEN_MSG,fileName);
	    return(EINVAL);
	}
	while (fscanf(infd, "%79s%ld", tbuffer, &rank) != EOF) {
	    thost = hostutil_GetHostByName(tbuffer);
	    if (!thost) {
		dce_svc_printf(CMU_S_CANT_RESOLVE_MSG,
			routineName, tbuffer);
		continue;
	    }
	    /* XXX Should look at thost->h_addrtype before assuming IN_ADDR */
	    sp->host.sin_addr = *((struct in_addr *) thost->h_addr);
	    sp->rank = rank;
	    sp->flags = service;

	    sp++;
	    incount++;
	    ioctlInfo.in_size += sizeof(struct spref);
	    if (ioctlInfo.in_size > osi_BUFFERSIZE - sizeof(struct spref)) {
		/*
		 * Time to cram this buffer into the kernel
		 */
		pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOC_SETSPREFS,
				    &ioctlInfo, PIOCTL_UNUSED_INT);
		if (pioctlCode != 0) {
		    rtnVal++;
		    CMUI_PrintError(pioctlCode, (char *)NULL);
		    /* Could bail out here, but might get more good data */
		}
		ioctlInfo.in_size = 0;
		sp = (struct spref *)ioctlInfo.in;
	    }
	}
	(void) fclose(infd);
    }
    
    /* If -stdin, suck in anything off the terminal we can get */
    if (cmd_IsParamPresent(aSyntax, SPREFS_STDIN_INDEX)) {
	while (scanf("%79s%ld", tbuffer, &rank) != EOF) {
	    thost = hostutil_GetHostByName(tbuffer);
	    if (!thost) {
		dce_svc_printf(CMU_S_CANT_RESOLVE_MSG,
			routineName, tbuffer);
		continue;
	    }
	    /* XXX Should look at thost->h_addrtype before assuming IN_ADDR */
	    sp->host.sin_addr = *((struct in_addr *) thost->h_addr);
	    sp->rank = rank;
	    sp->flags = service;

	    sp++;
	    incount++;
	    ioctlInfo.in_size += sizeof(struct spref);
	    if (ioctlInfo.in_size > osi_BUFFERSIZE - sizeof(struct spref)) {
		/*
		 * Time to cram this buffer into the kernel
		 */
		pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOC_SETSPREFS,
				    &ioctlInfo, PIOCTL_UNUSED_INT);
		if (pioctlCode != 0) {
		    rtnVal++;
		    CMUI_PrintError(pioctlCode, (char *)NULL);
		    /* Could bail out here, but might get more good data */
		}
		ioctlInfo.in_size = 0;
		sp = (struct spref *)ioctlInfo.in;
	    }

	}
    }

    /*
     * If any -server <machine/rank> args, get them now
     * Note that this loop is strange, it jumps up 2 at a time.
     */
    if (cmd_IsParamPresent(aSyntax, SPREFS_SERVER_INDEX)) {
	for (thisItemP = cmd_FirstItem(aSyntax, SPREFS_SERVER_INDEX);
	     ((thisItemP != (struct cmd_item *)NULL) &&
	      (thisItemP->next != (struct cmd_item *)NULL));
	     thisItemP = thisItemP->next->next) {

	    thost = hostutil_GetHostByName(thisItemP->data);
	    if (!thost) {
		dce_svc_printf(CMU_S_CANT_RESOLVE_MSG,
			routineName, thisItemP->data);
		continue;
	    }
	    /* XXX Should look at thost->h_addrtype before assuming IN_ADDR */
	    sp->host.sin_addr = *((struct in_addr *) thost->h_addr);
	    sp->rank = (unsigned short) atol(thisItemP->next->data);
	    sp->flags = service;

	    sp++;
	    incount++;
	    ioctlInfo.in_size += sizeof(struct spref);
	    if (ioctlInfo.in_size > osi_BUFFERSIZE - sizeof(struct spref)) {
		/*
		 * Time to cram this buffer into the kernel
		 */
		pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOC_SETSPREFS,
				    &ioctlInfo, PIOCTL_UNUSED_INT);
		if (pioctlCode != 0) {
		    rtnVal++;
		    CMUI_PrintError(pioctlCode, (char *)NULL);
		    /* Could bail out here, but might get more good data */
		}
		ioctlInfo.in_size = 0;
		sp = (struct spref *)ioctlInfo.in;
	    }

	}
    }
    /*
     * If there are some requests here, after processing all switches 
     * and inputs, push them into the kernel
     */
    if (ioctlInfo.in_size > 0) {
	/*
	 * Time to cram this buffer into the kernel
	 */
	pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOC_SETSPREFS,
			    &ioctlInfo, PIOCTL_UNUSED_INT);
	if (pioctlCode != 0) {
	    rtnVal++;
	    CMUI_PrintError(pioctlCode, (char *)NULL);
	}
    }
    dmprintf(cm_debug_flag, CM_DEBUG_BIT_CMD, ("%s: Accumulated %d errors\n",
					       routineName,rtnVal));
    return(rtnVal);
}

int DoGetProtectLevels(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
    int			rtnVal = 0;
    struct afs_ioctl	ioctlInfo;
    long			pioctlCode;
    struct cm_ProtBounds	result;
    int				anyPrinted;
    char			*authnDesc;
    char			authnNameBuff[50];
    static char		routineName[] = "DoGetProtectLevels";
 
    ioctlInfo.in = (caddr_t)NULL;
    ioctlInfo.in_size = 0;
    ioctlInfo.out = (caddr_t)&result;
    ioctlInfo.out_size = sizeof(result);
    bzero((char*)&result, sizeof(result));
    pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOC_GETPROTBNDS,
			&ioctlInfo, PIOCTL_UNUSED_INT);
    if (pioctlCode == 0) {
	if (result.formatTag == 1) {
	    anyPrinted = 0;
	    if (result.local.initialProtectLevel != rpc_c_protect_level_default) {
		dce_svc_printf(CMU_S_LCLINIAUTHN_MSG,
			       compat_authnToString(result.local.initialProtectLevel,
				authnNameBuff, sizeof(authnNameBuff)));
		anyPrinted = 1;
	    }
	    if (result.local.minProtectLevel != rpc_c_protect_level_default) {
		dce_svc_printf(CMU_S_LCLMINAUTHN_MSG,
			       compat_authnToString(result.local.minProtectLevel,
				authnNameBuff, sizeof(authnNameBuff)));
		anyPrinted = 1;
	    }
	    if (result.nonLocal.initialProtectLevel != rpc_c_protect_level_default) {
		dce_svc_printf(CMU_S_RMTINIAUTHN_MSG,
			       compat_authnToString(result.nonLocal.initialProtectLevel,
				authnNameBuff, sizeof(authnNameBuff)));
		anyPrinted = 1;
	    }
	    if (result.nonLocal.minProtectLevel != rpc_c_protect_level_default) {
		dce_svc_printf(CMU_S_RMTMINAUTHN_MSG,
			       compat_authnToString(result.nonLocal.minProtectLevel,
				authnNameBuff, sizeof(authnNameBuff)));
		anyPrinted = 1;
	    }
	    if (!anyPrinted) {
		dce_svc_printf(CMU_S_DEFAULTAUTHNS_MSG);
	    }
	} else {
	    pioctlCode = EINVAL;
	    rtnVal = 1;
	    CMUI_PrintError(pioctlCode, (char *)NULL);
	}
    }
    else {
	rtnVal = 1;
	CMUI_PrintError(pioctlCode, (char *) NULL);
    }
    return rtnVal;
}

int DoSetProtectLevels(aSyntax, aRock)
     struct cmd_syndesc *	aSyntax;
     char *			aRock;
{
    int			rtnVal = 0;
    struct afs_ioctl	ioctlInfo;
    long			pioctlCode;
    struct cm_ProtBounds	prot;
    int				gotAny;
    char			*authnDesc;
    static char		routineName[] = "DoSetProtectLevels";
 
    /* Get old values, edit in the requests, and then check consistency. */
    ioctlInfo.in = (caddr_t)NULL;
    ioctlInfo.in_size = 0;
    ioctlInfo.out = (caddr_t)&prot;
    ioctlInfo.out_size = sizeof(prot);
    bzero((char*)&prot, sizeof(prot));
    pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOC_GETPROTBNDS,
			&ioctlInfo, PIOCTL_UNUSED_INT);
    if (pioctlCode == 0) {
	if (prot.formatTag == 1) {
	    gotAny = 0;
	    if (aSyntax->parms[SETPROT_INITLOCAL_INDEX].items) {
		gotAny = 1;
		if (compat_nameToAuthnLevel(
			aSyntax->parms[SETPROT_INITLOCAL_INDEX].items->data,
			&prot.local.initialProtectLevel)) {
		    dce_svc_printf(CMU_S_BADINITLCLAUTHN_MSG,
			aSyntax->parms[SETPROT_INITLOCAL_INDEX].items->data);
		    rtnVal = 1;
		}
	    }
	    if (aSyntax->parms[SETPROT_MINLOCAL_INDEX].items) {
		gotAny = 1;
		if (compat_nameToAuthnLevel(
			aSyntax->parms[SETPROT_MINLOCAL_INDEX].items->data,
			&prot.local.minProtectLevel)) {
		    dce_svc_printf(CMU_S_BADMINLCLAUTHN_MSG,
			aSyntax->parms[SETPROT_MINLOCAL_INDEX].items->data);
		    rtnVal = 1;
		}
	    }
	    if (aSyntax->parms[SETPROT_INITREMOTE_INDEX].items) {
		gotAny = 1;
		if (compat_nameToAuthnLevel(
			aSyntax->parms[SETPROT_INITREMOTE_INDEX].items->data,
			&prot.nonLocal.initialProtectLevel)) {
		    dce_svc_printf(CMU_S_BADINITRMTAUTHN_MSG,
			aSyntax->parms[SETPROT_INITREMOTE_INDEX].items->data);
		    rtnVal = 1;
		}
	    }
	    if (aSyntax->parms[SETPROT_MINREMOTE_INDEX].items) {
		gotAny = 1;
		if (compat_nameToAuthnLevel(
			aSyntax->parms[SETPROT_MINREMOTE_INDEX].items->data,
			&prot.nonLocal.minProtectLevel)) {
		    dce_svc_printf(CMU_S_BADMINRMTAUTHN_MSG,
			aSyntax->parms[SETPROT_MINREMOTE_INDEX].items->data);
		    rtnVal = 1;
		}
	    }
	    if (gotAny) {
		if (prot.local.minProtectLevel != rpc_c_protect_level_default
		    && prot.local.initialProtectLevel != rpc_c_protect_level_default
		    && prot.local.minProtectLevel > prot.local.initialProtectLevel) {
		    dce_svc_printf(CMU_S_LCLPROTOVERLAP_MSG,
				   prot.local.minProtectLevel, prot.local.initialProtectLevel);
		    rtnVal = 1;
		}
		if (prot.nonLocal.minProtectLevel != rpc_c_protect_level_default
		    && prot.nonLocal.initialProtectLevel != rpc_c_protect_level_default
		    && prot.nonLocal.minProtectLevel > prot.nonLocal.initialProtectLevel) {
		    dce_svc_printf(CMU_S_RMTPROTOVERLAP_MSG);
		    rtnVal = 1;
		}
		if (rtnVal == 0) {
		    ioctlInfo.in = (caddr_t)&prot;
		    ioctlInfo.in_size = sizeof(prot);
		    ioctlInfo.out = (caddr_t)NULL;
		    ioctlInfo.out_size = 0;
		    pioctlCode = pioctl(PIOCTL_UNUSED_STRING, VIOC_SETPROTBNDS,
					&ioctlInfo, PIOCTL_UNUSED_INT);
		    if (pioctlCode != 0) {
			rtnVal = 1;
			CMUI_PrintError(pioctlCode, (char *) NULL);
		    }
		} else {
		    rtnVal = 1;
		    CMUI_PrintError(EINVAL, (char *) NULL);
		}
	    } else {
		/* no changes requested */
		dce_svc_printf(CMU_S_NONEWAUTHNS_MSG);
		rtnVal = 1;
	    }
	} else {
	    rtnVal = 1;
	    CMUI_PrintError(EINVAL, (char *) NULL);
	}
    } else {
	rtnVal = 1;
	CMUI_PrintError(pioctlCode, (char *) NULL);
    }
    return rtnVal;
}

int main(argc, argv)
     int	argc;
     char *	argv[];
{
  int		rtnVal = 0;
  long		cmdCode = 0;
  error_status_t st;
 
  osi_setlocale(LC_ALL, "");
 
  /* normal processing here */
  initialize_cmd_error_table();
  
  (void)strcpy(programName, argv[0]);

  initialize_svc();
  
  SetUpSyntax();
  
  /* now, let the cmd package figure out what needs to be done */
  cmdCode = cmd_Dispatch(argc, argv);
  if (cmdCode) {
    if ( cmd_IsCmdError(cmdCode) ) {
      rtnVal = 1;
    }
    else {
      /* other error handling code goes here */
      rtnVal = (int)cmdCode;
    }
  }

  return rtnVal;
}
