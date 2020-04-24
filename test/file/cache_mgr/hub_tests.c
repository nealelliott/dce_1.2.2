/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * hub_tests.c -- 
 *    Actual functions to execute cache consistency tests.
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * HISTORY
 * $Log: hub_tests.c,v $
 * Revision 1.1.124.1  1996/10/17  18:18:00  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:52  damon]
 *
 * Revision 1.1.119.1  1994/02/04  20:40:11  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:22:22  devsrc]
 * 
 * Revision 1.1.117.1  1993/12/07  17:41:34  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:41:37  jaffe]
 * 
 * Revision 1.1.10.2  1993/07/19  19:40:00  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:30:40  zeliff]
 * 
 * Revision 1.1.8.3  1993/07/16  21:30:15  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:51:35  kissel]
 * 
 * Revision 1.1.6.2  1993/06/04  20:51:43  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:39:04  kissel]
 * 
 * Revision 1.1.2.4  1993/01/11  18:25:24  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:57:13  htf]
 * 
 * Revision 1.1.3.2  1993/02/05  16:27:20  maunsell_c
 * 	01/04/93	maunsell_c	use srand vs. srandom & rand vs. random call
 * 	[1993/02/05  16:25:34  maunsell_c]
 * 
 * Revision 1.1.2.3  1992/11/04  19:21:57  jaffe
 * 	Transarc delta: fred-cc-fix-socket-port 1.3
 * 	  Selected comments:
 * 	    Additional fixes were necessary.  A couple calls to htons were added and
 * 	    also normalized the optn() flags.  There values are different on AIX and
 * 	    OSF/1.
 * 	    Included openFlags.h.
 * 	    Forgot to include new .h file.
 * 	[1992/11/04  17:35:53  jaffe]
 * 
 * Revision 1.1.2.2  1992/10/28  16:10:33  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:53:39  jaffe]
 * 
 * $EndLog$
 */

#include "hub.h"

#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <errno.h>
#include <dcedfs/osi.h>

#include "openFlags.h"

/* Test rename call.  One process does setup, then all do a verify
 * pass, ensuring that the setup worked.  Then one does a rename, and
 * all verify concurrently that things worked.
 *
 * Optionally can stat things while the rename is going on.
 */
wlr_RenameChecks(asetp, acx)
  register struct wlr_clientSet *asetp;
  struct wlr_context *acx;
{
    long count;
    register long code = 0;
    int j;
    int flag;
    register struct wlr_client *tp;
    char tname[1024];
    struct wlr_call *callID[WLR_MAX_CLIENTS];
    long sMTime, sCTime, sMode, sLength;

    wlr_CleanUpDir(wlr_ClientSetFirst(asetp), ".");

    for(j = 0; j<acx->count; j++) {
	wlr_ClientSetRotate(asetp);
	tp = wlr_ClientSetFirst(asetp);
	wlr_CleanUpDir(tp, ".");
	code = wlr_Create(tp, "a.mmt", 0666);
	if (code) {
	    printf("can't create test file, code %d\n", code);
	    return code;
	}
	wlr_VerifyExists(asetp, "a.mmt", 0);

	/* setup is now done, do the rename */
	code = wlr_DoRename(tp, "a.mmt", "b.mmt");
	if (code) {
	    printf("rename failed, code %d\n", code);
	    return code;
	}

	/* now reverify that A exists, and B does not */
	wlr_VerifyExists(asetp, "a.mmt", /* not */ 1);
	wlr_VerifyExists(asetp, "b.mmt", 0);

	/* now delete B, too */
	code = wlr_DoUnlink(tp, "b.mmt");
	wlr_VerifyExists(asetp, "a.mmt", /* not */ 1);
	wlr_VerifyExists(asetp, "b.mmt", /* not */ 1);
    }
    return code;
}

/* create "count" files on separate machines, and then ensure that
 * all of the participants can see all of the files.  Then delete
 * all of the files, and ensure that the participants see none of
 * the files.  We check that the file is/is not in both the
 * directory listing and the name lookup cache by using both STAT
 * and READDIR to check out the file.
 */
wlr_DirScanChecks(asetp, acx)
  register struct wlr_clientSet *asetp;
  struct wlr_context *acx;
{
    int filesToCreate;		/* # of files to create, total */
    register struct wlr_client *tp;
    long code;
    struct stat tstat;
    int flag;
    char tname[1024];

    filesToCreate = acx->count;

    /* clean things up initially */
    wlr_CleanUpDir(wlr_ClientSetFirst(asetp), ".");

    /* create 100 files uniformly, then count them all up on all machines */
    while (1) {
	for(tp=wlr_ClientSetFirst(asetp); tp; tp=wlr_ClientSetNext(asetp, tp)) {
	    if (filesToCreate == 0) break;	/* all done */
	    sprintf(tname, "t%d.mmt", filesToCreate);
	    code = wlr_Create(tp, tname, 0666);
	    if (code) return code;
	    filesToCreate--;
	}
	if (filesToCreate == 0) break;
    }

    /* all files created, next check they're all there */
    filesToCreate = acx->count;
    /* rotate client list once, to force things out of sync */
    wlr_ClientSetRotate(asetp);
    while (1) {
	for(tp=wlr_ClientSetFirst(asetp); tp; tp=wlr_ClientSetNext(asetp, tp)) {
	    /* check N at a time */
	    if (filesToCreate == 0) goto out1;
	    sprintf(tname, "t%d.mmt", filesToCreate);
	    /* check for presence in the dir listing */
	    code = wlr_EntryPresent(tp, ".", tname, &flag);
	    if (code) {
		printf("failed to verify client(1)\n");
		return code;
	    }
	    if (!flag) {
		printf("entry %s not present in dir list\n", tname);
		return -1;
	    }
	    /* check for presence in the name cache */
	    code = wlr_DoStat(tp, tname, &tstat);
	    if (code != 0) {
		printf("entry %s not present in name cache\n", tname);
		return code;
	    }
	    filesToCreate--;
	}
    }
  out1:

    /* delete everything, check all are gone */
    wlr_CleanUpDir(wlr_ClientSetFirst(asetp), ".");
    /* now verify all is gone */
    wlr_ClientSetRotate(asetp);
    while (1) {
	for(tp=wlr_ClientSetFirst(asetp); tp; tp=wlr_ClientSetNext(asetp, tp)) {
	    /* check N at a time */
	    if (filesToCreate == 0) goto out2;
	    sprintf(tname, "t%d.mmt", filesToCreate);
	    code = wlr_EntryPresent(tp, ".", tname, &flag);
	    if (code) {
		printf("failed to verify client(2)\n");
		return code;
	    }
	    if (flag) {
		printf("entry %s present in dir list\n", tname);
		return -1;
	    }
	    code = wlr_DoStat(tp, tname, &tstat);
	    if (code != ENOENT) {
		printf("entry %s present in name cache\n", tname);
		return (code? code : -1);
	    }
	    filesToCreate--;
	}
    }
  out2:

    /* done */
    return 0;
}

/* do chmod status tests.  Delete all test files, and create a new
 * test file on the first machine.  Then have half the clients
 * chmod this file (all to the same value), and all asynchronously.
 * Then have *all* participants stat the file and ensure that the
 * mode bits are correct.  Do this with varying mode bits "count" times.
 */
wlr_ModeChecks(asetp, acx)
  register struct wlr_clientSet *asetp;
  struct wlr_context *acx;
{
    int count;
    int i, j;
    register struct wlr_client *tp;
    struct wlr_call *callp;
    struct wlr_clientSet copySet, halfSet;
    long code;
    long newMode;
    int preCount;
    struct stat tstat;

    /* clean things up initially */
    wlr_CleanUpDir(wlr_ClientSetFirst(asetp), ".");

    /* make copy of asetp, since split is destructive */
    wlr_ClientSetCopy(asetp, &copySet);
    /* split off 1/2 of the elements, rounding up, into halfSet */
    wlr_ClientSetSplit(&copySet, &halfSet, 2, 1);
    code = wlr_Create(wlr_ClientSetFirst(asetp), "a.mmt", 0666);
    if (code) {
	printf("test file create failed, code %d\n", code);
	goto done;
    }

    /* basic idea: half of the guys asynchronously chmod the file to a
     * single known value, and then all of guys stat the file and make
     * sure the chmod worked.
     *
     * After each iteration of the test, we rotate the client list
     * in order to change the participants playing the various rolls.
     */
    newMode = 0;
    /* set the mode to something known, initially */
    code = wlr_DoChmod(wlr_ClientSetFirst(asetp), "a.mmt", newMode);
    if (code) goto done;
    for(i=0; i<acx->count; i++) {
	newMode++;
	for (tp = wlr_ClientSetFirst(&halfSet); tp;
	     tp = wlr_ClientSetNext(&halfSet, tp)) {
	    wlr_ABegin(tp, &callp);
	    code = wlr_XDRChmodIP(callp, "a.mmt", &newMode, 1);
	    if (code == 0) code = wlr_ACall(callp, WLR_OP_CHMOD);
	    if (code) {
		printf("chmod start failed with code %d\n", code);
		goto done;
	    }
	}

	/* wait for all calls to complete */
	code = wlr_AWaitAll(&halfSet);
	if (code) {
	    printf("chmod failed with code %d\n", code);
	    goto done;
	}
	/* now all chmod processes are done, do all the stat calls */

	for(tp = wlr_ClientSetFirst(asetp); tp;
	    tp = wlr_ClientSetNext(asetp, tp)) {
	    wlr_ABegin(tp, &callp);
	    code = wlr_XDRString(callp, "a.mmt", 1);
	    if (code == 0)
		code = wlr_ACall(callp, WLR_OP_STAT);
	    if (code) {
		printf("async stat start failed, code %d\n", code);
		goto done;
	    }
	}
	/* try to unmarshall the appropriate parameters */
	for(tp = wlr_ClientSetFirst(asetp); tp;
	    tp = wlr_ClientSetNext(asetp, tp)) {
	    callp = wlr_CallOf(tp);
	    code = wlr_AWaitCall(callp);
	    if (code == 0)
		code = wlr_XDRStat(callp, &tstat, 0);
	    if (code) {
		printf("stat failed with code %d\n", code);
		goto done;
	    }
	    if ((tstat.st_mode & 07777) != (newMode & 07777)) {
		wlr_ErrorReport(tp, "mode was %o, should be %o",
				tstat.st_mode, newMode, 0, 0);
	    }
	}

	/* next, do the whole thing again, only rotated, and with new mode */
	wlr_ClientSetRotate(asetp);
	newMode = (newMode++) & 0777;
    }
    code = 0;

  done:
    /* clean up and return code */
    wlr_ClientSetClear(&copySet);
    wlr_ClientSetClear(&halfSet);
    wlr_ResetAll(asetp);	/* reset all calls (useful for error path) */
    return code;
}

#define WLR_DATACHECK_NSTRIPS		2000
#define WLR_DATACHECK_STRIPSIZE		256

/* compute a random strip index */
#define WLR_RSTRIP()    (((rand()>>4) & 0xffff) % WLR_DATACHECK_NSTRIPS)

wlr_DataChecks(setp, cxp)
  struct wlr_clientSet *setp;
  struct wlr_context *cxp;
{
    struct wlr_clientSet baseSet, modSet, readSet;
    long code;
    struct wlr_client *tcp;
    long i, j;
    long strip;
    long fd;
    long offset, len;
    struct wlr_call *callp;
    char tbuffer[WLR_BLOCKSIZE];
    long count;
    char state[WLR_DATACHECK_NSTRIPS];

    if ((count = wlr_Count(setp)) < 2) {
	printf("datacheck: too few test hosts, need 2, have %d\n", count);
	printf("skipping data checks.\n");
	return 0;
    }

    /* prepare three sets of processes */
    wlr_ClientSetCopy(setp, &baseSet);
    wlr_ClientSetSplit(&baseSet, &readSet, 3, 1);
    wlr_ClientSetSplit(&baseSet, &modSet, 3, 1);

    /* print out status */
    printf("Testing %d writers, %d readers, %d randomizers\n",
	   wlr_Count(&modSet), wlr_Count(&readSet), wlr_Count(&baseSet));

    /* baseSet now has what's left (may be empty), and will be used
     * to generate interesting stress source.
     */

    for(tcp=wlr_ClientSetFirst(setp); tcp; tcp=wlr_ClientSetNext(setp, tcp)) {
	wlr_SetFD(tcp, -1);
    }

    srand(time(0));
    for(i=0; i<cxp->count; i++) {
	bzero(state, sizeof(state));

	code = wlr_Create(wlr_ClientSetFirst(setp), "a.mmt", 0666);
	if (code) {
	    printf("DataChecks: can't create test file.\n");
	    goto done;
	}

	/* open files for mod and base */
	for(tcp=wlr_ClientSetFirst(&modSet); tcp;
	    tcp=wlr_ClientSetNext(&modSet, tcp)) {
	    code = wlr_DoOpen(tcp, "a.mmt", WLR_O_TRUNC|WLR_O_RDWR, 0666, &fd);
	    if (code) {
		printf("datacheck:can't open test file\n");
		goto done;
	    }
	    wlr_SetFD(tcp, fd);
	}
	for(tcp=wlr_ClientSetFirst(&baseSet); tcp;
	    tcp=wlr_ClientSetNext(&baseSet, tcp)) {
	    code = wlr_DoOpen(tcp, "a.mmt", WLR_O_TRUNC|WLR_O_RDWR, 0666, &fd);
	    if (code) {
		printf("datacheck:can't open test file\n");
		goto done;
	    }
	    wlr_SetFD(tcp, fd);
	}
	/* and open for readers, too */
	for(tcp=wlr_ClientSetFirst(&readSet); tcp;
	    tcp=wlr_ClientSetNext(&readSet, tcp)) {
	    code = wlr_DoOpen(tcp, "a.mmt", WLR_O_RDONLY, 0666, &fd);
	    if (code) {
		printf("datacheck:can't open test file\n");
		goto done;
	    }
	    wlr_SetFD(tcp, fd);
	}

	/* start modset writing */
	for(tcp=wlr_ClientSetFirst(&modSet); tcp;
	    tcp=wlr_ClientSetNext(&modSet, tcp)) {

	    strip = WLR_RSTRIP();	/* compute random strip */
	    if (state[strip]) continue;	/* don't start htis one, someone's
					 * already writing.
					 */
	    wlr_InitArray(tbuffer, state, strip);
	    state[strip] = 1;	/* means guy to sync is writing */

	    code = wlr_ABegin(tcp, &callp);
	    if (code) goto done;
	    fd = wlr_GetFD(tcp);
	    offset = strip*WLR_DATACHECK_STRIPSIZE;
	    len = WLR_DATACHECK_STRIPSIZE;
	    code = wlr_XDRWriteIP(callp, &fd, &offset, &len, tbuffer, 1);
	    if (code) {
		printf("marshalling for async write failed\n");
		goto done;
	    }
	    code = wlr_ACall(callp, WLR_OP_WRITE);
	}

	/* add base guy's writing randomly now */
	for(tcp=wlr_ClientSetFirst(&baseSet); tcp;
	    tcp=wlr_ClientSetNext(&baseSet, tcp)) {

	    strip = WLR_RSTRIP();	/* compute random strip */
	    if (state[strip] == 1) continue; /* don't start this one, someone's
					      * already writing.
					      */

	    wlr_InitArray(tbuffer, state, strip);
	    state[strip] = 2;	/* doing async garbage write */

	    code = wlr_ABegin(tcp, &callp);
	    if (code) goto done;
	    fd = wlr_GetFD(tcp);
	    offset = strip*WLR_DATACHECK_STRIPSIZE;
	    len = WLR_DATACHECK_STRIPSIZE;
	    code = wlr_XDRWriteIP(callp, &fd, &offset, &len, tbuffer, 1);
	    if (code) {
		printf("marshalling for async write failed\n");
		goto done;
	    }
	    code = wlr_ACall(callp, WLR_OP_WRITE);
	}

	/* wait for modset to finish */
	code = wlr_AWaitAll(&modSet);
	if (code) {
	    printf("datacheck: write waitall failed, code %d\n", code);
	    goto done;
	}

	/* ask all in readSet to check file data */
	for(i=0;i<WLR_DATACHECK_NSTRIPS;i++) {
	    /* see if we know what the data should be here */
	    if (state[i] != 1) continue;

	    for(tcp=wlr_ClientSetFirst(&readSet); tcp;
		tcp=wlr_ClientSetNext(&readSet, tcp)) {

		code = wlr_ABegin(tcp, &callp);
		if (code) goto done;
		offset = i * WLR_DATACHECK_STRIPSIZE;
		len = WLR_DATACHECK_STRIPSIZE;
		fd = wlr_GetFD(tcp);
		code = wlr_XDRReadIP(callp, &fd, &offset, &len, 1);
		if (code) {
		    printf("marshalling for async read failed\n");
		    goto done;
		}
		code = wlr_ACall(callp, WLR_OP_READ);
	    }
	    /* after reading slot on all hosts, check the data */
	    wlr_AWaitAll(&readSet);
	    for(tcp=wlr_ClientSetFirst(&readSet); tcp;
		tcp=wlr_ClientSetNext(&readSet, tcp)) {
		code = wlr_XDRReadOP(wlr_CallOf(tcp), tbuffer, &len, 0);
		if (code) {
		    printf("datacheck: strip %d read failed code %d\n",
			   i, code);
		    goto done;
		}
		if (len != WLR_DATACHECK_STRIPSIZE){
		    printf("datacheck: short read (strip %d) %d bytes read, %d expected\n",
			   i, len, WLR_DATACHECK_STRIPSIZE);
		    code = EIO;
		    goto done;
		}
	    }
	}

	/* wait for everyone */
	wlr_AWaitAll(&baseSet);

	/* delete base file */	
	code = wlr_DoUnlink(wlr_ClientSetFirst(setp), "a.mmt");
	if (code) {
	    printf("datacheck: can't delete test file\n");
	    goto done;
	}

	/* close all files */
	for(tcp=wlr_ClientSetFirst(setp); tcp;
	    tcp=wlr_ClientSetNext(setp, tcp)) {
	    if (wlr_GetFD(tcp) != -1) {
		wlr_DoClose(tcp, wlr_GetFD(tcp));
		wlr_SetFD(tcp, -1);
	    }
	}
    }
    
    /* all done */
  done:
    /* close all FDs */
    for(tcp=wlr_ClientSetFirst(&modSet); tcp;
	tcp=wlr_ClientSetNext(&modSet, tcp)) {
	if (wlr_GetFD(tcp) != -1) 
	    wlr_DoClose(tcp, wlr_GetFD(tcp));
    }
    for(tcp=wlr_ClientSetFirst(&baseSet); tcp;
	tcp=wlr_ClientSetNext(&baseSet, tcp)) {
	if (wlr_GetFD(tcp) != -1) 
	    wlr_DoClose(tcp, wlr_GetFD(tcp));
    }
    for(tcp=wlr_ClientSetFirst(&readSet); tcp;
	tcp=wlr_ClientSetNext(&readSet, tcp)) {
	if (wlr_GetFD(tcp) != -1) 
	    wlr_DoClose(tcp, wlr_GetFD(tcp));
    }
    wlr_ClientSetClear(&baseSet);
    wlr_ClientSetClear(&modSet);
    wlr_ClientSetClear(&readSet);
    return code;
}

wlr_InitArray(buffer, strip)
  char *buffer;
  long strip;
{
    register long i;

    for(i=0;i<WLR_DATACHECK_STRIPSIZE;i++)
	buffer[i] = strip;
    return 0;
}
