/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <stdio.h>
#include <signal.h>

#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/cmd.h>
#include <dcedfs/icl.h>
#include <dcedfs/nubik.h>
#include "utst_proc.h"
#ifdef AFS_OSF_ENV
#include <sys/time.h>
#endif

#define	UTST_MAXCALLS	10	/* max rpc calls */
#define dfprintf(x) 

/* useful globals */
struct ubik_dbase *dbase;
long sleepTime;
globaldef uuid_t uuid_nil;
unsigned_char_t *nsGroupP;

error_status_t SSAMPLE_Inc(rxcall)
handle_t rxcall;
{
    long code, temp;
    struct ubik_trans	*tt;
    struct timeval tv;

    dfprintf(("in SSAMPLE_Inc\n"));
    code = ubik_BeginTrans(dbase, UBIK_WRITETRANS, &tt);
    if (code)
	return code;
    printf("about to set lock\n");
    /* now set database locks.  Must do this or people may read uncommitted
       data.  Note that we're just setting a lock at position 1, which is
       this program's convention for locking the whole database */
    code = ubik_SetLock(tt, 1, 1, LOCKWRITE);
    printf("began transaction; now have lock\n");
    if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    /* sleep for a little while to make it possible for us to test for some
	race conditions */
    if (sleepTime) {
	tv.tv_sec = sleepTime;
	tv.tv_usec = 0;
	pthread_delay_np((struct timespec *)&tv);
    }
    /* read the original value */
    code = ubik_Read(tt, (char *)&temp, (long)sizeof(long));
    if (code == UEOF) {
	/* short read */
	temp = 0;
    }
    else if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    temp++; /* bump the value here */
    printf("read the original value, writing the new one\n");
    /* reset the file pointer back to where it was before the read */
    code = ubik_Seek(tt, 0, 0);
    if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    /* write the data back */
    code = ubik_Write(tt, (char *)&temp, (long)sizeof(long));
    if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    /* finally, we commit the transaction */
    code = ubik_EndTrans(tt);
    temp = 0;
    dfprintf(("leaving SSAMPLE_Inc\n"));
    return code;
}


error_status_t SSAMPLE_Get(rxcall, gnumber)
handle_t rxcall;
long *gnumber;
{
    long code, temp;
    struct ubik_trans	*tt;
    struct timeval tv;

    /* start with a read transaction, since we're only going to do read
	  operations in this transaction. */
    dfprintf(("in SSAMPLE_Get\n"));
    code = ubik_BeginTrans(dbase, UBIK_READTRANS, &tt);
    if (code)
	return code;
    printf("began transaction; about to set lock\n");
    /* obtain a read lock, so we don't read data the other guy is writing */
    code = ubik_SetLock(tt, 1, 1, LOCKREAD);
    printf("now have lock\n");
    if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    /* sleep to allow races */
    if (sleepTime) {
	tv.tv_sec = sleepTime;
	tv.tv_usec = 0;
	pthread_delay_np((struct timespec *)&tv);
    }
    /* read the value */
    code = ubik_Read(tt, (char *)&temp, (long)sizeof(long));
    if (code == UEOF) {
	/* premature eof, use 0 */
	temp = 0;
    }
    else if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    printf("done reading, ending transaction\n");
    *gnumber = temp;
    /* end the transaction, automatically releasing locks */
    code = ubik_EndTrans(tt);
    dfprintf(("leaving SSAMPLE_Get\n"));
    return code;
}


error_status_t SSAMPLE_QGet(rxcall, gnumber)
handle_t rxcall;
long *gnumber;
{
    long code, temp;
    struct ubik_trans	*tt;
    struct timeval tv;

    /* start with a read transaction, since we're only going to do read
	  operations in this transaction. */
    code = ubik_BeginTransReadAny(dbase, UBIK_READTRANS, &tt);
    if (code)
	return code;
    printf("about to set lock\n");
    /* obtain a read lock, so we don't read data the other guy is writing */
    code = ubik_SetLock(tt, 1, 1, LOCKREAD);
    printf("now have lock\n");
    if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    /* sleep to allow races */
    if (sleepTime) {
	tv.tv_sec = sleepTime;
	tv.tv_usec = 0;
	pthread_delay_np((struct timespec *)&tv);
    }
    /* read the value */
    code = ubik_Read(tt, (char *)&temp, (long)sizeof(long));
    if (code == UEOF) {
	/* premature eof, use 0 */
	temp = 0;
    }
    else if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    *gnumber = temp;
    /* end the transaction, automatically releasing locks */
    code = ubik_EndTrans(tt);
    return code;
}


error_status_t SSAMPLE_Trun(rxcall)
handle_t rxcall;
{
    long code;
    struct ubik_trans	*tt;
    struct timeval tv;

    /* truncation operation requires a write transaction, too */
    code = ubik_BeginTrans(dbase, UBIK_WRITETRANS, &tt);
    if (code) return code;
    printf("about to set lock\n");
    /* lock the database */
    code = ubik_SetLock(tt, 1, 1, LOCKWRITE);
    printf("now have lock\n");
    if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    if (sleepTime) {
	tv.tv_sec = sleepTime;
	tv.tv_usec = 0;
	pthread_delay_np((struct timespec *)&tv);
    }
    /* shrink the file */
    code = ubik_Truncate(tt, 0);
    if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    /* commit */
    code = ubik_EndTrans(tt);
    return code;
}


error_status_t SSAMPLE_Test(rxcall)
handle_t rxcall;
{
    long code, temp;
    struct ubik_trans	*tt;
    struct timeval tv;

    /* first start a new transaction.  Must be a write transaction since
      we're going to change some data (with ubik_Write) */
    code = ubik_BeginTrans(dbase, UBIK_WRITETRANS, &tt);
    if (code) return code;
    printf("about to set lock\n");
    /* now set database locks.  Must do this or people may read uncommitted
	  data.  Note that we're just setting a lock at position 1, which is
      this program's convention for locking the whole database */
    code = ubik_SetLock(tt, 1, 1, LOCKWRITE);
    printf("now have lock\n");
    if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    /* sleep for a little while to make it possible for us to test for some
	race conditions */
    if (sleepTime) {
	tv.tv_sec = sleepTime;
	tv.tv_usec = 0;
	pthread_delay_np((struct timespec *)&tv);
    }
    /* read the original value */
    code = ubik_Read(tt, (char *)&temp, (long)sizeof(long));
    if (code == UEOF) {
	printf("short read, using 0\n");
	temp = 0;
    }
    else if (code) {
	ubik_AbortTrans(tt);
	return code;
    }
    ubik_AbortTrans(tt);    /* surprise! pretend something went wrong */
    return code;
}

error_status_t
SSAMPLE_GetServerInterfaces (h, serverInterfacesP)
     register handle_t h;
     dfs_interfaceList *serverInterfacesP;
{
    error_status_t st;

    dfs_GetServerInterfaces (SAMPLE_v4_0_s_ifspec, serverInterfacesP, &st);
    return st;
}

globaldef SAMPLE_v4_0_epv_t SAMPLE_v4_0_epv = {
    SSAMPLE_Inc,
    SSAMPLE_Get,
    SSAMPLE_Trun,
    SSAMPLE_Test,
    SSAMPLE_QGet,
    SSAMPLE_GetServerInterfaces
};

uuid_t utst_ubikObjectType =
{ /* 82285b60-5588-11ca-ba72-02608c2ebaac */
        0x82285b60,
        0x5588,
        0x11ca,
        0xba,
        0x72,
        {0x02, 0x60, 0x2e, 0xba, 0xac, 0x00}
};

/* ---------------
 * commands and command line parsing
 * ---------------
 */

/* allocStrcpy
 *      copy a string, allocating memory for the copy;
 */

char *
allocStrcpy(fromP)
     char *fromP;
{
    char *toP;

    toP = (char *) malloc(strlen(fromP)+1);
    if ( toP == 0 )                             /* malloc failed */
        return(0);
    strcpy(toP, fromP);
    return(toP);
}


/* utst_argHandler
 *	parse the user supplied arguments, placing the results in globals
 *	for use by main()
 */

utst_argHandler(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    /* -nsgroup */
    if (as->parms[0].items)
	nsGroupP = (unsigned char *)allocStrcpy(as->parms[0].items->data);

    /* -sleep */
    if (as->parms[1].items)
    {
	sleepTime = atoi(as->parms[1].items->data);
    }
    else
	sleepTime = 0;

    return(0);
}

/* utst_argSetup
 *	initialize the command(s) and all arguments
 */

utst_argSetup()
{
    struct cmd_syndesc *ts;

    ts = cmd_CreateSyntax((char *) 0, utst_argHandler, 0, "upserver");
    cmd_AddParm(ts, "-nsgroup", CMD_SINGLE, CMD_REQUIRED, "CDS server group");
    cmd_AddParm(ts, "-sleep", CMD_SINGLE, CMD_OPTIONAL, "sleeptime");
}

static void AbortDumpHandler(sig)
int sig;
{
  (void) fprintf(stderr, "utst_server: AbortDumpHandler called\n");
  icl_DumpUser();
  abort();
}

/* define event set structure for tracing and setup function */
PRIVATE void SetupICL()
{
  long code;
  static int Initted = 0;

  if (Initted)
    return;
  Initted = 1;
  
  code = icl_ExportIF("utst");
  if (code)
    (void) fprintf(stderr, "could not export ICL interface, code=%d\n", code);

  code = icl_StartCatcher(0);
  if (code < 0)
    (void) fprintf(stderr, "could not start ICL signal watcher, code=%d, errno=%d\n", code, errno);
  else {
#ifdef AFS_SIGACTION_ENV
#define SIGFUNC sigaction
	    struct sigaction	sv;

	    /* set up sigaction stuff now since CMA won't let us catch these */
	    bzero((char *)&sv, sizeof(sv));
	    sv.sa_handler = AbortDumpHandler;
#else
#define SIGFUNC sigvec
	    struct sigvec	sv;

	    /* set up sigaction stuff now since CMA won't let us catch these */
	    bzero((char *)&sv, sizeof(sv));
	    sv.sv_handler = AbortDumpHandler;
#if 0
	    sv.sv_flags = SA_OLDSTYLE;
#endif /* 0 */
#endif /* AFS_SIGACTION_ENV */
    if (SIGFUNC(SIGBUS, &sv, NULL) < 0) {
      (void) fprintf(stderr, "unable to catch SIGBUS, errno = %d\n", errno);
    }
    if (SIGFUNC(SIGSEGV, &sv, NULL) < 0) {
      (void) fprintf(stderr, "unable to catch SIGSEGV, errno = %d\n", errno);
    }
    if (SIGFUNC(SIGILL, &sv, NULL) < 0) {
      (void) fprintf(stderr, "unable to catch SIGILL, errno = %d\n", errno);
    }
  }
}

main(argc, argv)
    int argc;
    char **argv; {
    register long code, i;
    struct sockaddr_in serverList[UBIK_MAXSERVERS];
    long serverCount;
    char hostname[UTST_MAXNAMELEN];
    struct hostent *hostentPtr;
    struct sockaddr_in myHost;
    uuid_t objUuid;
    error_status_t st;

    SetupICL();
    utst_argSetup();    
    code = cmd_Dispatch(argc, argv);
    if ( code )
	return(1);

    /* check if this was simply a help request */
    if ( (argc == 2) && (strcmp(argv[1], "-help")==0) )
        return(0);

    /* setup the local host information for ubik_ServerInit */
    gethostname(hostname, sizeof(hostname));
    hostentPtr = gethostbyname(hostname);
    if ( hostentPtr == 0 )
    {
	printf("gethostbyname of %s failed\n", hostname);
	exit(1);
    }
    rpcx_sockaddr_from_ipaddr(hostentPtr->h_addr, &myHost);

    /* establish authentication for ourselves */
    code = dfsauth_server_InitAuthentication((char *)NULL,
					     "admin.utst",
					      (char *)NULL);
    if (code) {
	printf("utst_server: dfsauth_server_InitAuthentication failed\
 with code %ld\n", code);
	exit(1);
    }

    /* specify the name	to use for the database files (/tmp/testdb), and the
     * ServerInit returns a pointer to the database (in dbase), which
     * is required for creating new transactions 
     */
    code = ubik_ServerInit((struct sockaddr_in *)&myHost, 	
			   SAMPLE_v4_0_s_ifspec, 
			   (rpc_mgr_epv_t)&SAMPLE_v4_0_epv,
			   (unsigned_char_t *)nsGroupP, 
			   (unsigned_char_t *)NULL,	  /* unused */
			   (unsigned_char_t *)"subsys/dce/dfs-utst-servers", 
			                                  /* sec group */
			   (uuid_t *)&utst_ubikObjectType,/* type uuid */
			   (uuid_t *)&objUuid,
			   (unsigned32)UTST_MAXCALLS,
			   (char *)"/tmp/testdb",
			   (char *)"DFS Ubik Test Server",
			   (struct ubik_dbase **)&dbase);
    
    if (code) 
    {
	printf("ubik init failed with code %d\n", code);
	return;
    }

    dfs_installInterfaceDescription (SAMPLE_v4_0_s_ifspec, 
				     SAMPLE_v4_0_s_ifspec,
				     1, /* provider version number */
				     (unsigned_char_t *)"Transarc ubik test Interface",
				     &st);
    if (st) {
	ubik_dprint("bakserver: Can't install interface description (%s), \
Exiting.\n", dfs_dceErrTxt(st));
	exit(1);
    }


    rpc_server_listen(UTST_MAXCALLS, &st);
    if (st != error_status_ok) {
	printf("rpc listen failed, code %d\n", (long) st);
	exit(1);
    }
    code = compat_UnregisterServer (SAMPLE_v4_0_s_ifspec, &objUuid);
    
    printf("back from listen, shutting down!\n");
    exit(1);
}
