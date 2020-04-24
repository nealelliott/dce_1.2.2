/*
 * (C) Copyright 1991 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * hub_subr.c -- 
 *    Supporting routines for the hub.
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
 * $Log: hub_subr.c,v $
 * Revision 1.1.8.1  1996/10/17  18:17:58  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:51  damon]
 *
 * Revision 1.1.2.4  1993/01/11  18:25:21  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:57:08  htf]
 * 
 * Revision 1.1.2.3  1992/11/04  19:21:53  jaffe
 * 	Transarc delta: fred-cc-fix-socket-port 1.3
 * 	  Selected comments:
 * 	    Additional fixes were necessary.  A couple calls to htons were added and
 * 	    also normalized the optn() flags.  There values are different on AIX and
 * 	    OSF/1.
 * 	    Included openFlags.h and added htons call.
 * 	    Forgot to include new .h file.
 * 	[1992/11/04  17:35:31  jaffe]
 * 
 * Revision 1.1.2.2  1992/10/28  16:10:25  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:52:53  jaffe]
 * 
 * $EndLog$
 */

#include <sys/types.h>
#include <sys/file.h>
#include <errno.h>

#include "hub.h"
#include "openFlags.h"

wlr_ErrorReport(aclientp, astring, ap1, ap2, ap3, ap4)
  struct wlr_client *aclientp;
  char *astring;
  long ap1, ap2, ap3, ap4;
{
    printf("Client '%s' error: ", aclientp->name);
    printf(astring, ap1, ap2, ap3, ap4);
    printf("\n");
    return 0;
}

/* stub-like functions that make calls to do higher-level operations
 * on behalf of test driver.
 */

/* create a file with relative name aname, and mode amode */
wlr_Create(cp, aname, amode)
  char *aname;
  struct wlr_client *cp;
  long amode;
{
    long code;
    long fd;

    code = wlr_DoOpen(cp, aname, WLR_O_CREAT | WLR_O_TRUNC, 0666, &fd);
    if (code) return code;
    code = wlr_DoClose(cp, fd);
    return code;
}

/* helper proc called by wlr_EntryPresent on each name */
struct wlr_EPHelperData {
    long count;
    char *name;
};

wlr_EPHelper(adep, arockp, aclientp, adirName)
  struct wlr_netdirent *adep;
  char *arockp;
  struct wlr_client *aclientp;
  char *adirName;
{
    struct wlr_EPHelperData *tep = (struct wlr_EPHelperData *) arockp;

    if (strcmp(tep->name, adep->name) == 0) {
	/* found an entry */
	tep->count++;
    }
    return 0;	/* keep looking */
}

/* Enumerate all entries in the dir, looking for this entry
 * Return *aresult true if we find the name, otherwise false.
 * Return error code if something goes wrong finding out.
 */
int wlr_EntryPresent(cp, adirName, ascanName, aresultp)
  struct wlr_client *cp;
  char *adirName, *ascanName;
  int *aresultp;
{
    struct wlr_EPHelperData tdata;
    long code;		/* return code */

    tdata.count = 0;
    tdata.name = ascanName;
    code = wlr_DirApply(cp, adirName, wlr_EPHelper, (char *) &tdata);
    if (code) return code;
    if (tdata.count > 1)
	return WLR_ERROR_DUPENTRIES;
    if (tdata.count)
	*aresultp = 1;
    else
	*aresultp = 0;
    return 0;
}


/* Utility function to apply a function to all entries in a
 * remote directory.
 */
wlr_DirApply(ap, adirName, aprocp, arockp)
  struct wlr_client *ap;
  char *adirName;
  int (*aprocp)();
  char *arockp;
{
    long fd, code;
    char tbuffer[WLR_BLOCKSIZE];
    long dirOffset;	/* offset in dir we're scanning */
    long junk;
    int found;		/* found the name we're looking for */
    long remaining;	/* bytes remaining in dir buffer */
    struct wlr_netdirent *tdep;	/* entry we're scanning */

    code = wlr_DoOpen(ap, adirName, WLR_O_RDONLY, 0, &fd);
    if (code) return code;

    remaining = 0;
    found = 0;	/* we'll keep looking to see if it shows up twice! */
    dirOffset = 0;
    while (1) {
	if (remaining == 0) {
	    /* have to read a new directory buffer's worth.  If we
	     * get an EOF, we're done.
	     */
	    code = wlr_DoReaddir(ap, fd, dirOffset, sizeof(tbuffer),
				 tbuffer, &remaining);
	    if (code) goto bad;
	    if (remaining == 0) {
		/* we just read 0 bytes from the directory; it must
		 * be at EOF.  So, we're done.
		 */
		break;
	    }
	    /* otherwise, process all the entries */
	    tdep = (struct wlr_netdirent *) tbuffer;
	}

	/* process a dir entry */

	/* on some systems, a 0 length record means EOF */
	if (tdep->recLength == 0) break;

	code = (*aprocp)(tdep, arockp, ap, adirName);
	if (code) goto bad;	/* first non-zero return terminates
				 * and returns error.
				 */

	/* finally, skip to the next entry */
	remaining -= tdep->recLength;
	if (remaining < 0) {
	    code = WLR_ERROR_BIGRECORD;
	    goto bad;
	}
	dirOffset = tdep->offset;
	tdep = (struct wlr_netdirent *) (((char *) tdep) + tdep->recLength);
    }

    /* success at figuring out the answer, at least */
    code = 0;
    /* fall through to close file and return */

  bad:
    /* try to close the file */
    wlr_DoClose(ap, fd);
    return code;
}

/* helper for wlr_CleanUp */
wlr_CleanUpHelper(adep, arockp, aclientp, adirName)
  struct wlr_netdirent *adep;
  char *arockp;
  struct wlr_client *aclientp;
  char *adirName;
{
    char tbuffer[1024];
    int tlen;
    long code;

    tlen = adep->nameLength;
    if (tlen < 4) return 0;	/* too small for useful .mmt extension */
    /* check to ensure it ends with ".mmt" */
    if (strncmp(adep->name+tlen-4, ".mmt", 4) != 0) return 0;

    strcpy(tbuffer, adirName);
    strcat(tbuffer, "/");
    strcat(tbuffer, adep->name);
    code = unlink(tbuffer);
    return(code ? errno : 0);   /* return errno on failure */
}

/* clean up a directory by getting rid of all .mmt (multi-machine test)
 * files.  Be careful!
 */
wlr_CleanUpDir(cp, adirName)
  struct wlr_client *cp;
  char *adirName;
{
    int code;

    code = wlr_DirApply(cp, adirName, wlr_CleanUpHelper, (char *)  0);
    if (code) printf("cleanupdir: code %d\n", code);
    return code;
}

/* helper for wlr_CleanUp */
wlr_CountDirHelper(adep, arockp, aclientp, adirName)
  struct wlr_netdirent *adep;
  char *arockp;
  struct wlr_client *aclientp;
  char *adirName;
{
    long *tep = (long *) arockp;
    *tep++;
    return 0;
}

/* Count # of entries in directory. */
wlr_CountDir(cp, adirName, acountp)
  struct wlr_client *cp;
  char *adirName;
  long *acountp;
{
    int code;
    long count;

    count = 0;
    code = wlr_DirApply(cp, adirName, wlr_CleanUpHelper, (char *) &count);
    if (code == 0) *acountp = count;
    return code;
}

/* verify that the name aname exists and is seen on all clients in
 * the list.  Use lots of concurrency.  Verify that it *doesn't* exist
 * if anot is set.
 */
wlr_VerifyExists(asetp, aname, anot)
  register struct wlr_clientSet *asetp;
  char *aname;
  int anot;
{
    register long code;
    int flag;
    register struct wlr_client *tp;
    struct wlr_call *tcallp;
    char tname[1024];
    int i;
    long sMTime, sCTime, sMode, sLength;

    for(tp=wlr_ClientSetFirst(asetp); tp; tp=wlr_ClientSetNext(asetp, tp)) {
	if (code = wlr_EntryPresent(tp, ".", aname, &flag)) {
	    /* failed to test */
	    printf("VerifyExists: test failed, code %d\n", code);
	    goto done;
	}
	if (anot) {
	    if (flag == 1) {
		printf("VerifyExists: file (%s) not in dir listing\n",
		       aname);
		code = -1;
		goto done;
	    }
	}
	else {
	    if (flag == 0) {
		printf("VerifyExists: file (%s) is still in dir\n",
		       aname);
		code = -1;
		goto done;
	    }
	}
	
	code = wlr_ABegin(tp, &tcallp);
	if (code == 0) code = wlr_XDRString(tcallp, aname, 1);
	if (code == 0) code = wlr_ACall(tcallp, WLR_OP_STAT);
	if (code) {
	    printf("failed to start verify async stat, code %d\n", code);
	    goto done;
	}
    }
    /* now wait for async stats to complete */
    for(tp=wlr_ClientSetFirst(asetp); tp; tp=wlr_ClientSetNext(asetp, tp)) {
	code = wlr_AWait(tp);
	/* don't bother unmarshalling output, we don't care */
	if (anot) {
	    if (code != ENOENT) {
		printf("VerifyExists: stat wrong error on file %s, code %d\n",
		       aname, code);
		code = code? code : -1;
		goto done;
	    }
	}
	else {
	    if (code) {
		printf("VerifyExists: stat failed on file %s, code %d\n",
		       aname, code);
		goto done;
	    }
	}
    }
    code = 0;
    /* fall through */

  done:
    /* terminate pending calls, to clear pipe */
    for(tp=wlr_ClientSetFirst(asetp); tp; tp=wlr_ClientSetNext(asetp, tp)) {
	wlr_ResetCall(wlr_CallOf(tp));
    }
    return 0;
}
