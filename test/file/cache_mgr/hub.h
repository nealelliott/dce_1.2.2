/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * hub.h -- 
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
 * $Log: hub.h,v $
 * Revision 1.1.8.1  1996/10/17  18:17:54  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:50  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:25:18  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:57:00  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  16:01:59  jaffe
 * 	Transarc delta: fred-add-cc-tests 1.2
 * 	  Selected comments:
 * 	    Added Mike Kazar's cache consistency tests to the test tree.
 * 	    Forgot to put a EndLog marker in the header comment.
 * 	    Added EndLog marker.
 * 	[1992/10/27  17:51:40  jaffe]
 * 
 * $EndLog$
 */
#ifndef _HUB_H_ENV_
#define _HUB_H_ENV_ 1

#define WLR_CLIENTF_DOWN		1
#define WLR_MAX_CLIENTS			256

#include "queue.h"
#include "com.h"
#include "stub.h"

struct wlr_context {
    long count;		/* # of iterations */
};

/* prototypes */

/* from hub_parse.c */
extern int ParseHost _TAKES((char *name, struct sockaddr_in *sockp));
extern ParseCommands _TAKES((int argc, char **argv, struct wlr_context *contextp,
		     struct wlr_clientSet **cspp));

/* from hub_subr.c */

extern wlr_ErrorReport _TAKES((struct wlr_client *cp, char *msg,
			       long p1, long p2,
			       long p3, long p4));
extern wlr_Create _TAKES((struct wlr_client *cp, char *name, long mode));
extern wlr_EPHelper _TAKES((struct wlr_netdirent *dp, char *rockp,
			    struct wlr_client *cp, char *dname));
extern int wlr_EntryPresent _TAKES((struct wlr_client *cp, char *dirName,
			    char *scanName, int *resultp));
extern wlr_DirApply _TAKES((struct wlr_client *cp, char *dirName,
			    int (*procp)(struct wlr_netdirent *dp, char *rockp,
					  struct wlr_client *cp, char *dname),
			   char *rockp));
extern wlr_CleanUpHelper _TAKES((struct wlr_netdirent *dp, char *rockp,
				 struct wlr_client *cp, char *dname));
extern wlr_CleanUpDir _TAKES((struct wlr_client *cp, char *dname));
extern wlr_CountDirHelper _TAKES((struct wlr_netdirent *dp, char *rockp,
			  struct wlr_client *cp, char *dname));
extern wlr_CountDir _TAKES((struct wlr_client *dp, char *dname, long *countp));
extern wlr_VerifyExists _TAKES((struct wlr_clientSet *, char *name, int not));

/* from hub_tests.c */
extern wlr_RenameChecks _TAKES((struct wlr_clientSet *setp,
				struct wlr_context *cxp));
extern wlr_DirScanChecks _TAKES((struct wlr_clientSet *setp,
				 struct wlr_context *cxp));
extern wlr_ModeChecks _TAKES((struct wlr_clientSet *setp,
			      struct wlr_context *cxp));
extern wlr_DataChecks _TAKES((struct wlr_clientSet *setp,
			      struct wlr_context *cxp));

#endif /* _HUB_H_ENV_ */
