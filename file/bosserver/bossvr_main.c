/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bossvr_main.c,v $
 * Revision 1.1.31.1  1996/10/02  17:04:32  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:59  damon]
 *
 * $EndLog$
 */
/*
 *  bossvr_main.c -- main routines for bosserver execution
 *
 *
 *	Copyright (C) 1995, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>


#define ROOT_ID 0
#define ROOT_NAME "root"
/*
#define BOSSVR_CHECK_LICENSE	1
 */

#include <dcedfs/sysincludes.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/resource.h>

#include <dcedfs/ktime.h>
#include <dcedfs/common_data.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

#ifdef use_cds_compat
/*
#include <dcedfs/compat_cds.h>
 */
#else /* use_cds_compat */
#include <dcedfs/dfsauth.h>
#endif /* use_cds_compat */

#if defined(AFS_HPUX_ENV)
#include <fcntl.h>
#endif /* defined(AFS_HPUX_ENV) */

#include <dce/rpc.h>
#include <dcedfs/icl.h>

#include <pthread.h>
#include <bossvr_debug.h>
#include <bbos_ncs_interface.h>
#include <bbos_bnode.h>
#include <bbos_util.h>
#include <bossvr_afsFiles.h>
#include <bbos_pathnames.h>
#include <bossvr_trace.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbssmac.h>
#include <dfsbsssvc.h>
#include <dfsbssmsg.h>

dce_svc_handle_t   bss_svc_handle;

void initialize_svc()
{
  error_status_t st;

  bss_svc_handle = dce_svc_register(bss_svc_table,  (idl_char *)"bss",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dfs_define_bss_message_tables();
}

/* use the pthread default stack size for now */
#define BOSSVR_DEFAULT_STACK_SIZE	(60 * 512)
#define BOSSVR_MAX_KEY_MGMT_SLEEP        600

/* define BosConfig end record string */
#define BOSSVR_CONFIG_ENDRECORD		"END RECORD"

#ifdef AFS_OSF_ENV
/* The default stack size used by rpc threads on OSF is not  */
/* sufficient for the bosserver BOSSVR_AddSUser function */
/* Therefore we select a stack which we believe is "large enough" */
/* (at least over 100k)		*/ 
#define BOSSVR_RPC_STACK_SIZE           (250000)
#endif

#ifdef	AFS_AIX_ENV
IMPORT int atoi _TAKES((
			char * nptr
		      ));
IMPORT int fprintf _TAKES((
			   FILE *	stream,
			   char *	format,
			   ...
			 ));
#endif /* AFS_AIX_ENV */

/* the thread routines */
#ifdef notdef
IMPORT pthread_addr_t * ChildWatchThread _TAKES((pthread_addr_t * ignoredArgP));
IMPORT pthread_addr_t * ReBossvrWatchThread _TAKES((pthread_addr_t * ignoredArgP));
IMPORT pthread_addr_t * BnodeTimeoutWatchThread _TAKES((pthread_addr_t * ignoredArgP));

IMPORT pthread_startroutine_t ChildWatchThread;
IMPORT pthread_startroutine_t ReBossvrWatchThread;
IMPORT pthread_startroutine_t BnodeTimeoutWatchThread;
#endif /* notdef */

IMPORT void ChildWatchThread _TAKES((void));
IMPORT void ReBossvrWatchThread _TAKES((void));
IMPORT void BnodeTimeoutWatchThread _TAKES((void));

IMPORT struct bnode_ops ezbnode_ops, cronbnode_ops;

struct bztemp {
    FILE *file;
    unsigned long cksum;	/* check sum */
};

char		programName[BUFSIZ];
char *		bossvr_fileName = afs_bossvr_config_file;
char *		bossvr_licenseName = afs_bossvr_license_file;
FILE *		bossvr_logFile;

int		restartTimesValid = 0;
struct ktime	bossvr_nextRestartKT;
struct ktime	bossvr_nextDayKT;

struct afsconf_dir *	bossvr_confdir = (struct afsconf_dir *)NULL;
     
/* all the basic (non-RPC server) threads that the bosserver uses */
pthread_t	childWatchTid;
pthread_t	reBossvrWatchTid;
pthread_t	bnodeTimeoutWatchTid;
pthread_t      	keyManagerTid;

/* synchronization objects */
pthread_mutex_t	bosserverMutex;
pthread_cond_t	newRestartTimeCond;
pthread_cond_t	bnodeTimeoutChangeCond;

pthread_cond_t	reBossvrThreadReadyCond;
pthread_cond_t	timeoutThreadReadyCond;
pthread_cond_t	childWatchThreadReadyCond;

int		reBossvrThreadReady;
int		timeoutThreadReady;
int		childWatchThreadReady;

long		execOnRebossvr;

/* this has to be global because it's needed on a restart */
PRIVATE  char *	listFilenameP;		/* the name of the admin file to use */

/* define ICL event set pointer */
EXPORT struct icl_set *bossvr_iclSetp = (struct icl_set *)0;

EXPORT void bossvr_SvcLog(catm, b,c,d,e,f)
const unsigned32 catm;
char *b, *c, *d, *e, *f; 
{
  char 		tdate[26];
  long 		myTime;
  unsigned char *expanded_message;
  error_status_t st;

  myTime = time(0);
  strcpy(tdate, ctime(&myTime));	/* copy out of static area asap */
  tdate[24] = ':';
  expanded_message = dce_msg_get_msg(catm, &st);
  if (bossvr_logFile) {
    fprintf(bossvr_logFile, "%s ", tdate);
    if (expanded_message) {
	fprintf(bossvr_logFile, expanded_message, b, c, d, e, f);
    } else {
	fprintf(bossvr_logFile, "NO MEMORY for msg %ld\n", catm);
    }
    fflush(bossvr_logFile);
    fsync(fileno(bossvr_logFile));
  }
  else {
    printf("%s ", tdate);
    if (expanded_message) {
	printf(expanded_message, b, c, d, e, f);
    } else {
	printf("NO MEMORY for msg %ld\n", catm);
    }
    fflush(stdout);
  }
  if (expanded_message != NULL)
      free(expanded_message);

  sync();	/*  incredibly inefficient, but seems to be the only way to really
		 *  force the changes to disk (& avoid the truncation problem on AIX 3.2)
		 */
}

void BossvrPthreadFinish();	/* forward declaration */
void bossvr_ReBossvr()
{
  struct bnode_token	t1, t2, t3, t4;
  long			code;
  int			noAuthStatus;
  int			savedErrno;
  static char		routineName[] = "bossvr_ReBossvr";
  
  /* the RPC threads are already quiesced at this point */
  if (pthread_mutex_lock(&bosserverMutex) != 0) {
    /* log the error and go on; we are about to shutdown anyway */
    savedErrno = errno;
    bossvr_SvcLog(bss_s_global_unlok_err, routineName, savedErrno);
  }
  
  /* shutdown all our child processes - ignore errors, we're about to shutdown, anyway */
  (void)bossvr_ShutdownAll();

  BossvrPthreadFinish();

  /* 
   * If there is a core file now, it hasn't been processed so it must be
   * from something else.  Rename it to core.unknnown so we don't think it 
   * was a bosserver core dump when we restart.
   */
  (void) rename("core", "core.unknown");

  if (execOnRebossvr) {
    /* the first arg is the name of the binary to be executed */
    t1.key = afs_bosserver_binary;
    t1.next = &t2;
    
    /* use the same admin list as we are currently using */
    t2.key = "-adminlist";
    t2.next = &t3;
    
    t3.key = listFilenameP;
    t3.next = (struct bnode_token *)NULL;
    
    code = dfsauth_server_GetNoAuthStatus(&noAuthStatus);
    if (code == 0) {
      if (noAuthStatus == 1) {
	/* no auth flag is set, pass "-noauth" to new bosserver */
	t3.next = &t4;
	t4.key = "-noauth";
	t4.next = (struct bnode_token *)NULL;
      }
    }
    else {
	bossvr_SvcLog(bss_s_get_noauth_status_err, programName, code);
    }
    
    bnode_DoExec(&t1);
  }

  _exit(1);	/* terminate the entire application */
}

long NCS_Init(maxCalls, listFilenameP)
     int	maxCalls;
     char *	listFilenameP;
{
  long				rtnVal = error_status_ok;
  error_status_t		ncs_error_status;
  char				calleeName[BUFSIZ];
  static char			routineName[] = "NCS_Init";
  uuid_t			nil_uuid;

  uuid_create_nil(&nil_uuid, (unsigned32 *)&ncs_error_status);
  if (ncs_error_status == uuid_s_ok) {
    dfs_register_rpc_server(BOSSVR_v0_0_s_ifspec,
			    (rpc_mgr_epv_t)NULL,
			    (uuid_t *)&nil_uuid, 
			    (uuid_t *)&nil_uuid,
			    maxCalls, listFilenameP, "DFS bosserver", 
			    &ncs_error_status);
    if (ncs_error_status == error_status_ok) {
      dfs_installInterfaceDescription(BOSSVR_v0_0_s_ifspec,
				      BOSSVR_v0_0_s_ifspec,
				      1 /* provider_version number */,
		   (unsigned_char_t *)"Transarc BOS Server Interface",
				      &ncs_error_status);
      if (ncs_error_status != error_status_ok) {
	bossvr_SvcLog(bss_s_intfc_desc_install_failed, programName,
		      dfs_dceErrTxt(ncs_error_status));
	rtnVal = ncs_error_status;
      }
    }
    else {
      bossvr_SvcLog(bss_s_register_dfs_svr_err, programName,
		    dfs_dceErrTxt(ncs_error_status));
      rtnVal = ncs_error_status;
    }
  }
  else {
    bossvr_SvcLog(bss_s_cant_create_nil_UUID, programName,
		  dfs_dceErrTxt(ncs_error_status));
    rtnVal = ncs_error_status;
  }

  execOnRebossvr = 0;
  if (rtnVal == error_status_ok) {
    
#if defined(AFS_OSF_ENV)
    {
      unsigned32 status;
      
      rpc_mgmt_set_server_stack_size(BOSSVR_RPC_STACK_SIZE, &status);
      if (status != rpc_s_ok){
	bossvr_SvcLog(bss_s_cant_set_rpc_stack_size,
		      BOSSVR_RPC_STACK_SIZE, status);
	rtnVal = status;
      }
    }
#endif /* AFS_OSF_ENV */
    
    if (rtnVal == error_status_ok) {	/* may have changed on OSF/1 */
      /* now, listen to the network for incoming calls */
      bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_BINDING,
		      ("%s: about to listen for network requests.", programName));
      icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_MAIN_BINDING_0);
      (void)strcpy(calleeName, "rpc_server_listen");
      bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_DCE_TRACE,
		      ("%s: %s about to call %s", programName, routineName, calleeName));
      icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_MAIN_TRACE_1);
      rpc_mgmt_set_server_com_timeout(rpc_c_binding_default_timeout+2,
				      &ncs_error_status);
      rpc_server_listen(maxCalls, &ncs_error_status);
      bossvr_dmprintf(bossvr_debug_flag, BOSSVR_DEBUG_BIT_DCE_TRACE,
		      ("%s: %s returned from %s", programName, routineName, calleeName));
      icl_Trace0(bossvr_iclSetp, BOSSVR_ICL_MAIN_TRACE_2);
      
      if (ncs_error_status == error_status_ok) {
	/* normal shutdown, probably by another bosserver */
	if (ncs_error_status =
	    compat_UnregisterServer(BOSSVR_v0_0_s_ifspec, (uuid_t *)NULL)) {
	    bossvr_SvcLog(bss_s_unregister_self_failed,
			  programName, dfs_dceErrTxt(ncs_error_status));
	    rtnVal = 1;
	}
      }
      else {
	bossvr_SvcLog(bss_s_bossvr_not_listening,
		      programName, dfs_dceErrTxt(ncs_error_status));
	rtnVal = 1;
      }
    }
  }

  bossvr_ReBossvr();	/* shutdown everything */
  return rtnVal;	/* it is an error to get to here */
}

EXPORT void bossvr_ThreadExit(argP)
     void *	argP;
{
  char *	thrNameP = ((cleanupArg_t *)argP)->threadName;
  char *	msgP = ((cleanupArg_t *)argP)->msg;
  int		rlseLock = ((cleanupArg_t *)argP)->releaseLockFlag;
  
  IMPORT pthread_mutex_t	bosserverMutex;
  
  bossvr_SvcLog(bss_s_thrdname_and_msg, thrNameP, msgP);
  
  if (rlseLock == BOSSVR_RELEASE_GLOBAL_LOCK) {
    if (pthread_mutex_unlock(&bosserverMutex) != 0) {
      bossvr_SvcLog(bss_s_exit_error, thrNameP, errno);
    }
  }
}


/*
 * This function is the bosy of the thread that manages the dfs-server 
 * key.  All it does is figure out the name of the principal then call
 * the sec_key_mgmt_manage_key() function, which should never return.
 */
PRIVATE pthread_addr_t *DfsServerKeyManagerThread(ignoredArgP)
     pthread_addr_t *	ignoredArgP;
{
  char *			principalName;
  int 			bufferSize = 1024;
  unsigned long 		code = 0;
  int 			done = 0;
  char			routineName[] = "DfsServerKeyManagerThread";
  struct timespec       interval={10,0};
  
  /* allocate some space to hold principal name */
  principalName = malloc(bufferSize);
  while(!done && (principalName != NULL) && (code == 0)) {
    code = dfsauth_PrinName_GetName(NULL, NULL, principalName, bufferSize);
    if (code == DAUT_ERROR_BUFFER_TOO_SMALL) {
      bufferSize *= 2;
      principalName = realloc(principalName, bufferSize);
      code = 0;
    }
    else {
      done = 1;
    }
  }
  if (code != 0) {
    bossvr_SvcLog(bss_s_cant_get_princ_name,
		  programName, routineName, dfs_dceErrTxt(code));
  }
  else if (principalName == NULL) {
    bossvr_SvcLog(bss_s_cant_alloc_princ_svr_memory, programName, routineName);
  }
  else {
    /* we have a server principal name now, free unneeded memory */
    principalName = realloc(principalName, strlen(principalName)+1);
    
    while(1) {
      /* call key management function */
      sec_key_mgmt_manage_key(rpc_c_authn_dce_secret, NULL, principalName, &code);
      bossvr_SvcLog(bss_s_sec_key_returned, programName, routineName,
               (code == error_status_ok) ? "success" : dfs_dceErrTxt(code));
      if (code != error_status_ok) {
	struct timespec delay;

	/* give the admin time to fix things */
	delay.tv_sec = 600;		/* ten minutes */
	delay.tv_nsec = 0;
	pthread_delay_np(&delay);
      }
    }
  }
  
  free(principalName);
  pthread_exit((pthread_addr_t)NULL);
}

/* make sure a dir exists */
static int MakeDir(adir)
     register char *adir;
{
  int		rtnVal = 0;
  int		reqOwner;
  struct stat	tstat;
  char		routineName[] = "MakeDir";
  
  if ((stat(adir, &tstat) < 0) || (tstat.st_mode & S_IFMT) != S_IFDIR) {
    /* if there is trouble finding the entry, or it is not a directory */
    int reqPerm;
    unlink(adir);
    reqPerm = GetRequiredDirPerm (adir);
    if (reqPerm == -1) reqPerm = 0777;
    rtnVal = mkdir(adir, reqPerm);
    if (rtnVal == 0) {
      reqOwner = GetRequiredOwner (adir);
      if (reqOwner != -1) {
	rtnVal = chown(adir, reqOwner, /* XXX don't have a group - use 0 */0);
      }
      else {
	bossvr_SvcLog(bss_s_unknown_owner, programName, routineName, adir);
      }
    }
  }
  return rtnVal;
}

/* create all the bossvr dirs */
static int CreateDirs()
{
  int		rtnVal = 0;
  
  /* rtnVal will count the number of failures */
  rtnVal += ((MakeDir(dce_base_dir) != 0) ? 1 : 0);
  rtnVal += ((MakeDir(dce_subcomponents_dir) != 0) ? 1 : 0);
  rtnVal += ((MakeDir(afs_server_config_dir) != 0) ? 1 : 0);
  rtnVal += ((MakeDir(afs_server_admin_dir) != 0) ? 1 : 0);
  rtnVal += ((MakeDir(afs_bin_dir) != 0) ? 1 : 0);
  
  return rtnVal;
}


/* strip the \\n from the end of the line, if it is present */
static StripLine(abuffer)
register char *abuffer; {
    register char *tp;
    
    tp = abuffer + strlen(abuffer); /* starts off pointing at the null  */
    if(tp == abuffer) return 0;	    /* null string, no last character to check */
    tp--;	/* aim at last character */
    if (*tp == '\n') *tp = 0;
    return 0;
}


/* do a simple check sum oin the data written out */
static unsigned long bzcksum(str)
char *str;
{
    register char *p;
    register unsigned long cksum = 0;

    /* if not debugging, turn off end record printing */
    if ((bossvr_debug_flag & BOSSVR_DEBUG_MASK(BOSSVR_DEBUG_BIT_END_RECORD)) == 0)
	return 0;

    for(p = str; *p; p++)
	cksum += (unsigned long)*p;

    return cksum;
}

#define	MAXPARMS    20
ReadBossvrFile(aname)
     char *aname;
{
  register FILE *	tfile;
  char			tbuffer[BOSSVR_BSSIZE];
  register char *	tp;
  char *		instp, *typep;
  register long 	code;
  long 			ktmask, ktday, kthour, ktmin, ktsec;
  long 			i, goal;
  struct bnode *	tb;
  char *		parms[MAXPARMS];
  int			fd = -1;
  
#ifdef NOLONGERUSED
    /* rename BossvrInit to BosServer for the user */
    if (!aname) {
	/* if BossvrInit exists and BosConfig doesn't, try a rename */
	if (access("/usr/afs/local/BossvrInit", 0) == 0
	    && access("/usr/afs/local/BosConfig", 0) != 0) {
	    code = rename("/usr/afs/local/BossvrInit", "/usr/afs/local/BosConfig");
	    if (code < 0)
		dce_fprintf(stderr, bss_s_bosconfig_rename, strerror(errno));
	}
    }
#endif /* NOLONGERUSED */

  /* setup default times we want to do restarts */
#if 0
  bossvr_nextRestartKT.mask = KTIME_HOUR | KTIME_MIN | KTIME_DAY;
  bossvr_nextRestartKT.hour = 4; /* 4 am */
  bossvr_nextRestartKT.min = 0;
  bossvr_nextRestartKT.day =	0;  /* Sunday */
#else /* 0 */
  /* By default, no automatic restarts. */
  bossvr_nextRestartKT.mask = KTIME_NEVER;
#endif /* 0 */
  bossvr_nextDayKT.mask = KTIME_HOUR | KTIME_MIN;
  bossvr_nextDayKT.hour = 5;
  bossvr_nextDayKT.min = 0;
  
  for(code=0;code<MAXPARMS;code++)
    parms[code] = (char *) 0;
  instp = typep = (char *) 0;
  tfile = (FILE *) 0;
  if (!aname) aname = bossvr_fileName;
  tfile = fopen(aname, "r");
  if (!tfile) {
    fd = open(aname, (O_RDWR | O_CREAT), 0644);
    if (fd >= 0) {
      tfile = fdopen(fd, "r");
      if (!tfile) {
	bossvr_SvcLog(bss_s_cant_produce_fileptr_from_filedesc, programName, errno);
	return -1;
      }
    }
    else {
      bossvr_SvcLog(bss_s_open_BosConfig_file_failed, programName, errno);
      return -1;
    }

    /* write out the defaults here */
    code = WriteBossvrFile(aname);
  }
  else {
    /* we have a real file that we now need to read */

    int endRecordFound = 0;
    unsigned long cksum = 0;
    unsigned long cksumFromFile;

    instp = (char *) osi_Alloc(BOSSVR_BSSIZE);
    typep = (char *) osi_Alloc(BOSSVR_BSSIZE);
    while (1) {
      /* ok, read lines giving parms and such from the file */
      tp = fgets(tbuffer, sizeof(tbuffer), tfile);
      if (tp == (char	*) 0) {
	  if (!endRecordFound &&
	      ((bossvr_debug_flag & BOSSVR_DEBUG_MASK(BOSSVR_DEBUG_BIT_END_RECORD)) != 0))
          {
	      bossvr_SvcLog(bss_s_corrupted_file_no_end_record);
          }

	  break;	/* all done */
      }
      if (tbuffer[0] == '#') {
	  if ((bossvr_debug_flag & BOSSVR_DEBUG_MASK(BOSSVR_DEBUG_BIT_END_RECORD)) != 0) {
	      /* check for end record if we're debugging */
	      if (strncmp(tbuffer+2, BOSSVR_CONFIG_ENDRECORD, 
			  strlen(BOSSVR_CONFIG_ENDRECORD)) == 0) {
		  register char *p;

		  endRecordFound++;

		  /* get check sum, if any */
		  p = tbuffer + 2 + strlen(BOSSVR_CONFIG_ENDRECORD) + 1;
		  if (*p == '(') {
		      /* check sum in file */
		      cksumFromFile = atoi(++p);
		      if (cksumFromFile != cksum) {
			  bossvr_SvcLog(bss_s_checksum_mismatch, cksumFromFile, cksum);
		      }

		  }
	      }
	  }
	  continue;	/* skip the comment */
      }
      cksum += bzcksum(tbuffer);

      if (strncmp(tbuffer, "restarttime", 11) == 0) {
	code = sscanf(tbuffer, "restarttime %d %d %d %d %d",
		      &ktmask, &ktday, &kthour, &ktmin, &ktsec);
	if (code != 5) {
	  code = -1;
	  goto fail;
	}
	/* otherwise we've read in the proper ktime structure; now assign
	   it and continue processing */
	bossvr_nextRestartKT.mask = ktmask;
	bossvr_nextRestartKT.day = ktday;
	bossvr_nextRestartKT.hour = kthour;
	bossvr_nextRestartKT.min = ktmin;
	bossvr_nextRestartKT.sec = ktsec;
	continue;
      }
      
      if (strncmp(tbuffer, "checkbintime", 12) == 0) {
	code = sscanf(tbuffer, "checkbintime %d %d %d %d %d",
		      &ktmask, &ktday, &kthour, &ktmin, &ktsec);
	if (code != 5) {
	  code = -1;
	  goto fail;
	}
	/* otherwise we've read in the proper ktime structure; now assign
	   it and continue processing */
	bossvr_nextDayKT.mask = ktmask;	/* time to restart the system */
	bossvr_nextDayKT.day = ktday;
	bossvr_nextDayKT.hour = kthour;
	bossvr_nextDayKT.min = ktmin;
	bossvr_nextDayKT.sec = ktsec;
	continue;
      }
      
      if (strncmp("bnode", tbuffer, 5) != 0) {
	code = -1;
	goto fail;
      }
      code = sscanf(tbuffer, "bnode %s %s %d", typep, instp, &goal);
      if (code != 3) {
	code = -1;
	goto fail;
      }
      
      for(i=0;i<MAXPARMS;i++) {
	/* now read the parms, until we see an "end" line */
	tp = fgets(tbuffer, sizeof(tbuffer), tfile);
	if (!tp) {
	  code = -1;
	  goto fail;
	}
	cksum += bzcksum(tbuffer);
	StripLine(tbuffer);
	if (tbuffer[0] != '#') {	/* lines beginning with '#' are ignored */
	  if (!strncmp(tbuffer, "end", 3)) break;
	  if (strncmp(tbuffer, "parm ", 5)) {
	    code = -1;
	    goto fail;    /* no "parm " either */
	  }
	  if (!parms[i])  /* make sure there's space */
	    parms[i] = (char *) osi_Alloc(BOSSVR_BSSIZE);
	  strcpy(parms[i], tbuffer+5);    /* remember the parameter for later */
	}
      }
      
      /* ok, we have the type and parms, now create the object 
       * but don't update BosConfig */
      code = bnode_Create(typep, instp, &tb, parms[0], parms[1], parms[2],
			  parms[3], parms[4], parms[5], /*updateConfig?*/0);
      if (code) goto fail;
      
      /* bnode created in 'temporarily shutdown' state;
	 check to see if we are supposed to run this guy,
	 and if so, start the process up */
      if (goal) {
	bnode_InitFileGoal(tb, BSTAT_NORMAL); /* Init... doesn't rewrite BossvrInit */
	bnode_SetStat(tb, BSTAT_NORMAL);	/* set goal, taking effect immediately */
      }
      else {
	bnode_InitFileGoal(tb, BSTAT_SHUTDOWN);	/* doesn't rewrite file */
	bnode_SetStat(tb, BSTAT_SHUTDOWN);
      }
    }

    /* all done */
    code = 0;
  }
  
  return(code);

fail:
  if (instp) osi_Free(instp, BOSSVR_BSSIZE);
  if (typep) osi_Free(typep, BOSSVR_BSSIZE);
  for(i=0;i<MAXPARMS;i++) if (parms[i]) osi_Free(parms[i], BOSSVR_BSSIZE);
  if (tfile) fclose(tfile);
  return -1;
}

/* read license file */
static ReadLicenseFile(aname, aservers, anfss)
char *aname;
long *aservers;
long *anfss; {
    char tbuffer[40];
    register FILE *tfile;
    char *tp;
    register long code;
    long key;

    tfile = fopen(aname, "r");
    if (!tfile) return -1;
    tp = fgets(tbuffer, sizeof(tbuffer), tfile);
    fclose(tfile);
    if (tp == (char *) 0) return -1;
    code = sscanf(tbuffer, "%d", &key);
    *aservers = (key/10000) % 100;
    *anfss = key % 2;
    if (code != 1) {
	dce_svc_printf(BSS_S_LICENSE_FILE_SYNTAX_ERR_MSG, aname);
	return -1;
    }
    return 0;
}

/* write one bnode's worth of entry into the file */
static bzwrite(abnode, at)
register struct bnode *abnode;
register struct bztemp *at; 
{
    register int i;
    char tbuffer[BOSSVR_BSSIZE+64];
    register long code;

     
    sprintf(tbuffer, "bnode %s %s %d\n", abnode->type->name, abnode->name, abnode->fileGoal);
    at->cksum += bzcksum(tbuffer);
    fputs(tbuffer, at->file);
    strcpy(tbuffer, "parm ");
    for(i=0;;i++) {
	code = bnode_GetParm(abnode, i, tbuffer + 5 /* "parm " */, BOSSVR_BSSIZE);
	if (code) {
	    if (code != BZDOM) return code;
	    break;
	}
	strcat(tbuffer, "\n");
	at->cksum += bzcksum(tbuffer);
	fputs(tbuffer, at->file);
    }
    at->cksum += bzcksum("end\n");
    fprintf(at->file, "end\n");
    fflush(at->file);
    fsync(fileno(at->file));

    return 0;
}

/* write a new bossvr file */
int WriteBossvrFile(aname)
char *aname; 
{
  register FILE *tfile;
  char databuf[256];
  char tbuffer[256];
  register long code;
  struct bztemp btemp;

  if (!aname) aname = bossvr_fileName;
  strcpy(tbuffer, aname);
  strcat(tbuffer, ".NBZ");
  tfile = fopen(tbuffer, "w");
  if (!tfile) return -1;
  btemp.file = tfile;
  btemp.cksum = 0;	/* zero out check sum */
  sprintf(databuf, "restarttime %d %d %d %d %d\n", bossvr_nextRestartKT.mask,
	  bossvr_nextRestartKT.day, bossvr_nextRestartKT.hour, bossvr_nextRestartKT.min,
	  bossvr_nextRestartKT.sec);
  btemp.cksum += bzcksum(databuf);
  fputs(databuf, tfile);
  sprintf(databuf, "checkbintime %d %d %d %d %d\n", bossvr_nextDayKT.mask,
	  bossvr_nextDayKT.day, bossvr_nextDayKT.hour, bossvr_nextDayKT.min,
	  bossvr_nextDayKT.sec);
  btemp.cksum += bzcksum(databuf);
  fputs(databuf, tfile);
  fflush(tfile);
  fsync(fileno(tfile));

  code = bnode_ApplyInstance(bzwrite, &btemp);
  if (code || (code = ferror(tfile))) {	/* something went wrong */
    fclose(tfile);
    unlink(tbuffer);
    return code;
  }

  /* if debugging, write out end record with check sum */
  if ((bossvr_debug_flag & BOSSVR_DEBUG_MASK(BOSSVR_DEBUG_BIT_END_RECORD)) != 0)
      fprintf(tfile, "# %s (%u)\n", BOSSVR_CONFIG_ENDRECORD, btemp.cksum);

  /* close the file, check for errors and snap new file into place */
  if (fclose(tfile) == EOF) {
    unlink(tbuffer);
    return -1;
  }
  code = rename(tbuffer, aname);
  if (code) {
    unlink(tbuffer);
    return -1;
  }

  sync();	/*  incredibly inefficient, but seems to be the only way to really
		 *  force the changes to disk (& avoid the truncation problem on AIX 3.2)
		 */
  return 0;
}


/*
 * The following routine initializes AND GRABS the global bosserver mutex and returns
 * with that mutex still held.
 */
void BossvrPthreadInit()
{
  pthread_mutexattr_t	mutexAttr;
  pthread_attr_t	threadAttr;
  int			condCode;

  /* make sure cma is ready to go */
  /* cma_init(); */

  /* first, get everybody's attribute values set up */

  /* create & init attr values needed by the mutex */
  if (pthread_mutexattr_create(&mutexAttr) != 0) {
    bossvr_SvcLog(bss_s_mutex_attr_create_err, programName, errno);
    exit(2);
  }

  /* create & init attr values needed by the threads */
  if (pthread_attr_create(&threadAttr) != 0) {
    bossvr_SvcLog(bss_s_thread_attr_create_err, programName, errno);
    exit(2);
  }
  if (pthread_attr_setstacksize(&threadAttr, BOSSVR_DEFAULT_STACK_SIZE) != 0) {
    bossvr_SvcLog(bss_s_set_stacksize_in_thread_attr_error,
		  programName, BOSSVR_DEFAULT_STACK_SIZE, errno);
    exit(2);
  }
  /* now, actually create the synchronization & pthread objects */
  if (pthread_mutex_init(&bosserverMutex, mutexAttr) != 0) {
    bossvr_SvcLog(bss_s_bossvr_global_mutex_init_err, programName, errno);
    exit(2);
  }

  /* initialize the condition vars the threads use for communication w/ RPC threads */
  if (pthread_cond_init(&newRestartTimeCond, pthread_condattr_default) != 0) {
    bossvr_SvcLog(bss_s_init_new_restart_time_cond_err, programName, errno);
    exit(2);
  }
  if (pthread_cond_init(&bnodeTimeoutChangeCond, pthread_condattr_default) != 0) {
    bossvr_SvcLog(bss_s_init_bnode_timeout_change_cond_err, programName, errno);
    exit(2);
  }
  
  /* initialize the condition vars the threads use for communication w/ the main thread */
  if (pthread_cond_init(&reBossvrThreadReadyCond, pthread_condattr_default) != 0) {
    bossvr_SvcLog(bss_s_error_initing_new_restarttime_cond, programName, errno);
    exit(2);
  }

  if (pthread_cond_init(&timeoutThreadReadyCond, pthread_condattr_default) != 0) {
    bossvr_SvcLog(bss_s_init_thrd_rdy_timeout_cond_err, programName, errno);
    exit(2);
  }
  if (pthread_cond_init(&childWatchThreadReadyCond, pthread_condattr_default) != 0) {
    bossvr_SvcLog(bss_s_init_childwatch_thrd_rdy_timeout_err, programName, errno);
    exit(2);
  }
  
  /* hold the global bosserver mutex */
  if (pthread_mutex_lock(&bosserverMutex) != 0) {
    bossvr_SvcLog(bss_s_lock_global_mutex_init_err, programName, errno);
    exit(2);
  }

  /* init bossvr sleeping package */
  if ((condCode = bossvr_InitSleepers()) != 0) {
    bossvr_SvcLog(bss_s_init_sleeping_pkg_error, condCode);
    exit(2);
  }
  
  /* spawn all the non-RPC bosserver threads */
  reBossvrThreadReady = 0;
  timeoutThreadReady = 0;
  childWatchThreadReady = 0;
#ifdef AFS_OSF_ENV
  if (pthread_create(&childWatchTid, threadAttr, 
		      (pthread_startroutine_t)ChildWatchThread, (void *) NULL)
#else
  if (pthread_create(&childWatchTid, threadAttr, 
		 (pthread_startroutine_t)ChildWatchThread, (pthread_addr_t)NULL)
#endif /* AFS_OSF_ENV */
      != 0) {
    bossvr_SvcLog(bss_s_create_SIGCHLD_watchthread_err, programName, errno);
    exit(2);
  }

#ifdef AFS_OSF_ENV
  if (pthread_create(&reBossvrWatchTid, threadAttr, (pthread_startroutine_t)ReBossvrWatchThread, (void *) NULL)
#else
  if (pthread_create(&reBossvrWatchTid, threadAttr, (pthread_startroutine_t)ReBossvrWatchThread, (pthread_addr_t)NULL)
#endif /* AFS_OSF_ENV */
      != 0) {
    bossvr_SvcLog(bss_s_create_rebossvr_watchthread_err, programName, errno);
    exit(2);
  }

  if (pthread_create(&bnodeTimeoutWatchTid, threadAttr, (pthread_startroutine_t)BnodeTimeoutWatchThread,
#ifdef AFS_OSF_ENV
		     (void *) NULL) != 0) {
#else
		     (pthread_addr_t) NULL) != 0) {
#endif /* AFS_OSF_ENV */
    bossvr_SvcLog(bss_s_bnode_timeout_watchthread_create_err, programName, errno);
    exit(2);
  }

#ifdef AFS_OSF_ENV
  if (pthread_create(&keyManagerTid, pthread_attr_default, 
		     (pthread_startroutine_t)DfsServerKeyManagerThread, (void *) NULL)
#else
  if (pthread_create(&keyManagerTid, pthread_attr_default, 
		     (pthread_startroutine_t)DfsServerKeyManagerThread, (pthread_addr_t) NULL)
#endif /* AFS_OSF_ENV */
      != 0) {
    bossvr_SvcLog(bss_s_dfssvr_key_mgmt_thread_create_err, programName, errno);
    exit(2);
  }

  /* now reclaim the space used by the prototype attributes objects */
  if (pthread_mutexattr_delete(&mutexAttr) != 0) {
    bossvr_SvcLog(bss_s_del_mutex_attr_init_err, programName, errno);
    exit(2);
  }
  if (pthread_attr_delete(&threadAttr) != 0) {
    bossvr_SvcLog(bss_s_del_thread_attr_init_err, programName, errno);
    exit(2);
  }

}

/*
 * The following routine waits until all the helper threads have signalled that they
 * are ready to run.
 */
void BossvrWaitForHelperThreads()
{
  int	pthreadRtnCode;
  int	savedErrno;
  
  while (reBossvrThreadReady == 0) {
    pthreadRtnCode = pthread_cond_wait(&reBossvrThreadReadyCond, &bosserverMutex);
    savedErrno = errno;
    if (pthreadRtnCode != 0) {
      bossvr_SvcLog(bss_s_reBossvr_thrd_cond_sig_err, programName);
      exit(2);
    }
  }

  while (timeoutThreadReady == 0) {
    pthreadRtnCode = pthread_cond_wait(&timeoutThreadReadyCond, &bosserverMutex);
    savedErrno = errno;
    if (pthreadRtnCode != 0) {
      bossvr_SvcLog(bss_s_timeout_thrd_cond_sig_err, programName);
      exit(2);
    }
  }

  while (childWatchThreadReady == 0) {
    pthreadRtnCode = pthread_cond_wait(&childWatchThreadReadyCond, &bosserverMutex);
    savedErrno = errno;
    if (pthreadRtnCode != 0) {
      bossvr_SvcLog(bss_s_childwatch_thrd_cond_sig_err, programName);
      exit(2);
    }
  }
}


/*
 * The following routine assumes that the global bosserver mutex is held on entry
 * (i.e., that no other operations are in progress).
 */
void BossvrPthreadFinish()
{
  /* kill all the non-RPC threads */
  if (pthread_cancel(childWatchTid) != 0) {
    bossvr_SvcLog(bss_s_kill_SIGCHLD_watchthrd_err, programName, errno);
  }

  /* if execOnRebossvr is set, it was set by the watch thread, which then exited */
  if (!execOnRebossvr) {
    /* in this case, someone else has pushed us out of the listen */
    if (pthread_cancel(reBossvrWatchTid) != 0) {
      bossvr_SvcLog(bss_s_kill_reBossvr_watchthrd_err, programName, errno);
    }
  }
  
  if (pthread_cancel(bnodeTimeoutWatchTid) != 0) {
    bossvr_SvcLog(bss_s_kill_bnode_timeout_wathcthrd_err, programName, errno);
  }
  
  /* destroy all the synchronization objects */
  if (pthread_cond_destroy(&newRestartTimeCond) != 0) {
    bossvr_SvcLog(bss_s_destroy_new_restart_cond_err, programName, errno);
  }
  
  if (pthread_cond_destroy(&bnodeTimeoutChangeCond) != 0) {
    bossvr_SvcLog(bss_s_destroy_bnode_timeout_cond_err, programName, errno);
  }
  

  if (pthread_cond_destroy(&reBossvrThreadReadyCond) != 0) {
    bossvr_SvcLog(bss_s_destroy_reBossvr_thrd_rdycond_err, programName, errno);
  }
  if (pthread_cond_destroy(&timeoutThreadReadyCond) != 0) {
    bossvr_SvcLog(bss_s_destroy_timeout_thrd_rdycond_err, programName, errno);
  }
  if (pthread_cond_destroy(&childWatchThreadReadyCond) != 0) {
    bossvr_SvcLog(bss_s_destroy_childwatch_thrdrdy_timeout_cond_err,
		  programName, errno);
  }
  

#if 0
  /*
   * There could be pending requests waiting on the bosserver lock.  These
   * will try to use this lock and could get errors.  The best thing is to
   * leave it locked and go ahead and die or exec.
   */
  if (pthread_mutex_unlock(&bosserverMutex) != 0) {
    bossvr_SvcLog(bss_s_unlock_global_mutex_err2, programName, errno);
  }
  
  if (pthread_mutex_destroy(&bosserverMutex) != 0) {
    bossvr_SvcLog(bss_s_desroy_global_mutex_err2, programName, errno);
  }
#endif  
  bossvr_SvcLog(bss_s_appl_exiting, programName);
}

static int bossvr_SetUpBnodes()
{
  int	rtnVal = 0;
  
  rtnVal = bnode_Init();

  if (!rtnVal) {
    bnode_Register("simple", &ezbnode_ops, 1);
    bnode_Register("cron", &cronbnode_ops, 2);
  }
  
  return rtnVal;
}


static int bossvr_CheckLicense()
{
  int			rtnVal = 0;
  long			nServerLicenses, nNFSLicenses;
  FILE *		consoleFile;
#if defined(AFS_HPUX_ENV)
  int			consoleFid;
#endif /* defined(AFS_HPUX_ENV) */

  /* don't let /dev/console become the controlling tty here */
#if defined(AFS_HPUX_ENV)
  consoleFid = open("/dev/console", O_WRONLY | O_APPEND, O_NOCTTY);
  if (consoleFid != -1) {
    consoleFile = fdopen(consoleFid, "a");
  }
#else
  consoleFile = fopen("/dev/console", "a");
#endif

  if (consoleFile) {
    rtnVal = ReadLicenseFile(bossvr_licenseName, &nServerLicenses, &nNFSLicenses);
    if (rtnVal == 0){
      if (nServerLicenses == 99)
	dce_fprintf(consoleFile, bss_s_AFS31_site_license);
      else
	dce_fprintf(consoleFile, bss_s_amt_of_file_server_licenses, nServerLicenses);
      if (nNFSLicenses == 0)
	dce_fprintf(consoleFile, bss_s_no_NFSorAFS_xlator_license ); 
      else
	dce_fprintf(consoleFile, bss_s_have_NFSorAFS_xlator_license ); 
    }
    else {
      dce_fprintf(consoleFile, bss_s_No_license_file_contact_Transarc, bossvr_licenseName);
    }
    fclose(consoleFile);
  }
  
  return rtnVal;
}

static int bossvr_BeginLogging()
{
  int		rtnVal = 0;
  char		savedLogFileName[BUFSIZ];
  int		savedErrno;
  static char	routineName[] = "bossvr_BeginLogging";
  
  /* switch to logging information to the BosLog file */
  {
  char *dcesptr;
  dcesptr = dce_sprintf(bss_s_saved_LogFileName, afs_bosserver_log_file);
  strcpy(savedLogFileName , dcesptr);
  free(dcesptr);
  }
  rename(afs_bosserver_log_file, savedLogFileName);	/* try rename first */
#ifdef notdef
  bossvr_logFile = fopen(afs_bosserver_log_file, "a");
#endif /* notdef */

  /*
   * Connect stdout and stderr to the log file, too, so that sub-packages that use the
   * afs debugging (dprintf, dmprintf) macros will write to the log file.
   */
  /* first, connect stdout */
  fflush(stdout);
  bossvr_logFile = freopen(afs_bosserver_log_file, "a", stdout);
  if (!bossvr_logFile) {
    rtnVal = 1;
  }
  else {
    bossvr_SvcLog(bss_s_logging_begins, programName);

    /* now, connect stderr */
    fflush(stderr);
    if (dup2(fileno(stdout), fileno(stderr)) == -1) {
      savedErrno = errno;
      bossvr_SvcLog(bss_s_error_routing_stderr_to_logfile, programName, savedErrno);
    }
  }
  
  return rtnVal;
}

/*
 * define functions/variables that will let the bosserver become a
 * daemon.  The basic problem is that we don't want the parent to 
 * return before we know that the child is really going to run; but, 
 * we don't want to start threads in the child before the fork.  So, 
 * to get around this, we open a pipe between the parent and child.
 * The parent doesn't exit until it gets a status code or EOF from the 
 * child.
 */
static int umbilicus[2];
static int pipeSetup = 0;

static void bossvr_PullThePlug(code)
int code;
{
  /* pass the code back to the parent and let it die */
  if (pipeSetup) {
    (void) write(umbilicus[1], (char *)&code, sizeof(code));
    (void) close(umbilicus[1]);
    pipeSetup = 0;
  }
}

static void bossvr_GetRidOfTty()
{
  int		forkPid;
  int		code;
  int		retVal;

  /* create communications pipe between child and parent */
  if (pipe(umbilicus) < 0)
  {
      (void) dce_fprintf(stderr, bss_s_cant_create_child_parent_pipe, errno);
      exit(1);
  }
  pipeSetup = 1;

/* The CMA fork() wrapper doesn't work, but other wrappers do. */
#if defined(CMA_UX) && defined(fork)
#undef fork /* Turn off CMA wrapper for fork */
#endif /* defined(CMA_UX) && defined(fork) */
  forkPid = fork();
  if (forkPid > 0) {
    (void) close(umbilicus[1]);	/* close writer side of the pipe */

    /* read a status code from the child */
    if ((code = read(umbilicus[0], (char *)&retVal, sizeof(retVal))) <= 0) {
	(void) dce_fprintf(stderr, bss_s_bossvr_not_started);
	exit(1);	/* child failed */
    }
    if (retVal != 0) {
      (void) dce_fprintf(stderr, bss_s_bossvr_not_started2);
    }
    exit(retVal);	/* parent exits; child continues */
  }
  else if (forkPid < 0) {
    dce_fprintf(stderr, bss_s_could_not_fork, programName, forkPid, errno);
    exit(1);
  }
  
  /* we are in the child here */
  
  /*
   * get rid of our controlling terminal and get into a new
   * process group
   */
  if (setsid() == -1) {
    dce_fprintf(stderr, bss_s_setsid_error_in_child, programName, errno);
  }
}

#define BOSSVR_CMD_ADMINLIST_OFFSET	0
#define BOSSVR_CMD_NOAUTH_OFFSET	1
int BossvrMain(aSyntaxP, aRockP)
     struct cmd_syndesc *	aSyntaxP;
     char *			aRockP;
{
  int			rtnVal = 0;	/* as a good value as any for now */
  int			maxCalls = 4;
  int			noAuth = 0;
  int			code;
  int			bnodeCode;
  int			savedErrno;
  int			pthreadRtnCode;
  char			defaultListFilename[] = "admin.bos";
  int			listFilenameNext = 0;
  error_status_t	ncs_error_status;
  struct icl_log *	logp;
  sigset_t		signalBlockSet;

  if (geteuid() != ROOT_ID) {
    dce_svc_printf(BSS_S_PROG_MUST_RUN_AS_ROOT_MSG, programName, ROOT_NAME);
    return 1;
  }

  /* interpret the args */
  noAuth = cmd_IsParamPresent(aSyntaxP, BOSSVR_CMD_NOAUTH_OFFSET);
  if (cmd_IsParamPresent(aSyntaxP, BOSSVR_CMD_ADMINLIST_OFFSET)) {
    listFilenameP = cmd_FirstData(aSyntaxP, BOSSVR_CMD_ADMINLIST_OFFSET);
  }
  else {
    listFilenameP = defaultListFilename;
  }
  
  /* initialize all the variables containing useful path names */
  InitPaths();

  /* fork off a child and wait for a status code over the pipe */
  if ((bossvr_debug_flag & BOSSVR_DEBUG_MASK(BOSSVR_DEBUG_BIT_NO_FORK)) == 0) {
    bossvr_GetRidOfTty();
  }

  /* 
   * block SIGCHLD and SIGUSR1 for all threads. The ones that will be 
   * waiting for them will clear their mask by using sigwait()
  */
  (void)sigemptyset(&signalBlockSet);
  (void)sigaddset(&signalBlockSet, SIGCHLD);
  (void)sigaddset(&signalBlockSet, SIGUSR1);
  (void)sigprocmask(SIG_BLOCK, &signalBlockSet, (sigset_t *)NULL);  

  
  /* now, start doing the real work */
  if ((bnodeCode = bossvr_SetUpBnodes()) != 0) {
    dce_fprintf(stderr, bss_s_bnode_pkg_not_inited, programName, bnodeCode);
    return 1;
  }


  /* create useful dirs */
  if (CreateDirs()) {
    dce_fprintf(stderr, bss_s_cant_verify_directory_struct, programName);
    return 1;
  }

  /* chdir to /usr/afs/logs */
  chdir(afs_server_admin_dir);

  /* if a core file exists in this directory, it's probably from bosserver */
  (void) rename("core", "core.bosserver");

#ifdef BOSSVR_CHECK_LICENSE
  if (bossvr_CheckLicense() != 0) {
    /* check license explicitly prints its error message on the console */
    return 1;
  }
#endif /* BOSSVR_CHECK_LICENSE */

  /* NB: this MUST be done before the call to dfsauth_InitAuthentication in NCS_Init */
  rtnVal = dfsauth_server_SetNoAuthStatus(noAuth);
  if (rtnVal != 0) {
    dce_fprintf(stderr, bss_s_error_setting_noauth_status, programName, rtnVal);
    return 1;
  }
  
  /* here, we rely on the dfsauth_server_InitAuthentication routine to be idempotent */
  /*
   *  This is really ugly programming, but the testing people were unhappy that we
   * lost the error code from the invalid pathname for the admin list after the fork,
   * and other code relies on the dfsauth_server initialization being called from
   * the dfs_register_rpc_server routine.....so we're stuck between a rock 
   * and a hard place.....
   */
  rtnVal = dfsauth_server_InitAuthentication((char *)NULL, listFilenameP, (char *)NULL);
  if (rtnVal != 0) {
    dce_fprintf(stderr, bss_s_init_auth_subsys_error, programName, dfs_dceErrTxt(rtnVal));
    return 1;
  }

  if (ncs_error_status =
      compat_ShutdownDuplicateServer(BOSSVR_v0_0_s_ifspec, (uuid_t *)NULL, 0)) {
    dce_fprintf(stderr, bss_s_error_shutting_dup_svr_down, programName, 
                dfs_dceErrTxt(ncs_error_status));
    return 1;
  }

  if (bossvr_BeginLogging() != 0) {
    dce_fprintf(stderr, bss_s_cant_init_logfile, programName, afs_bosserver_log_file);
    return 1;
  }
  
  /* Write current state of directory permissions to log file */
  DirAccessOK ();

  /* initialize ICL tracing */
  code = icl_ExportIF("bosserver");
  if (code != 0) {
      bossvr_SvcLog(bss_s_cant_export_ICLRPC_intfc, programName, code);
  }
  code = icl_StartCatcher(0);
  if (code < 0) {
      bossvr_SvcLog(bss_s_cant_start_ICLsignal_catcher, programName, errno);
  }
  else {
    code = icl_CreateLog("common", 30*1024, &logp);
    if (code == 0) {
      code = icl_CreateSet("bosserver", logp, (struct icl_log *) 0, &bossvr_iclSetp);
    }
      
    if (code) {
      bossvr_SvcLog(bss_s_cant_init_ICL_trace, programName, code);
    }
  }

  /* set up all the bosserver threads */
  BossvrPthreadInit();

  BossvrWaitForHelperThreads();
  /*
   * The other threads are ready to go when we get to here, though the reBossvr watch
   * thread is waiting to be signalled that the restart time variables contain
   * valid data.
   */

  /* read init file, starting up programs */
  if (ReadBossvrFile(0) != 0) {
    bossvr_SvcLog(bss_s_cant_open_bossvr_configFile, programName);
    return 1;
  }
  
  /* tell the reBossvrWatch thread that it can ready its values now */
  restartTimesValid = 1;
  pthreadRtnCode = pthread_cond_signal(&newRestartTimeCond);
  savedErrno = errno;	/* we have other threads running now, and we don't trust errno */
  if (pthreadRtnCode != 0) {
    bossvr_SvcLog(bss_s_error_signalling_rebossvr_readvars, programName, savedErrno);
    return 1;
  }
  
  /* allow those threads to start execution */
  pthreadRtnCode = pthread_mutex_unlock(&bosserverMutex);
  savedErrno = errno;
  if (pthreadRtnCode != 0) {
    bossvr_SvcLog(bss_s_doing_global_mutex_unlock, programName, savedErrno);
    return 1;
  }

  /* let the parent process go away now */
  bossvr_PullThePlug(0);

  /* set up everything NCS needs to run and listen for calls */
  rtnVal = NCS_Init(maxCalls, listFilenameP);

  /* we should never get to here unless something went wrong */

  return rtnVal;
}

void SetUpSyntax()
{
  struct cmd_syndesc *	aSyntaxP;
  unsigned char *command_expl;
  error_status_t st;

  command_expl = dce_msg_get_msg(bss_s_initcmd_start_bossvr, &st);
  aSyntaxP = cmd_CreateSyntax("initcmd", BossvrMain, (char *)NULL,
  (char *) command_expl);
  free(command_expl);

  command_expl = dce_msg_get_msg(bss_s_add_adminlist_filename, &st);
  cmd_IndexedAddParm(aSyntaxP, "-adminlist", CMD_SINGLE, CMD_OPTIONAL, 
  (char *) command_expl, BOSSVR_CMD_ADMINLIST_OFFSET);
  free(command_expl);

  command_expl = dce_msg_get_msg(bss_s_add_noauth_enable_mode, &st);
  cmd_IndexedAddParm(aSyntaxP, "-noauth", CMD_FLAG, CMD_OPTIONAL, 
  (char *) command_expl, BOSSVR_CMD_NOAUTH_OFFSET);
  free(command_expl);
}

int main(argc, argv)
     int	argc;
     char *	argv[];
{
  int		rtnVal = 0;	/* as good as any value, for now */

  osi_setlocale(LC_ALL, "");
  sleep(2);	/* give last bosserver chance to exit */


    initialize_svc();


  (void)strncpy(programName, argv[0], sizeof(programName) - 1);

  /* make sure we're running as self */
  dfsauth_server_RunAsSelf();

  SetUpSyntax();
  rtnVal = cmd_Dispatch(argc, argv);
  /* should not return from cmd_Dispatch() */

  /*
   * We assume that the problem that caused return was logged at the point of occurrence.
   * We can't log it here, because we don't know whether it should go to the log or
   * to stderr.  (We don't know how far the initialization got.)
   */
  
  bossvr_PullThePlug(rtnVal);	/* in case the parent is still waiting */
  return rtnVal;
}


     
