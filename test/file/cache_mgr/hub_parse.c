/*
 * (C) Copyright 1991 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * hub_parse.c -- 
 *    Code to parse command line.
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
 */

#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>

#include "hub.h"

/* function to fill in an IP address */
int ParseHost(aname, asockp)
  struct sockaddr_in *asockp;
  char *aname;
{
    char hostname[256];
    register char *tp;
    struct hostent *thp;
    unsigned short port;

    bzero(hostname, sizeof(hostname));
    tp = (char *) index(aname, '/');
    if (tp) {
	strncpy(hostname, aname, tp-aname);
	port = (unsigned short) atoi(tp+1);
    }
    else {
	strcpy(hostname, aname);
	port = 5555;
    }
    thp = gethostbyname(hostname);
    if (thp) {
	bzero(asockp, sizeof(*asockp));
	asockp->sin_family = AF_INET;
	asockp->sin_port = htons(port);
	bcopy(thp->h_addr_list[0], &asockp->sin_addr.s_addr, thp->h_length);
	return 0;
    }
    else return -1;
}

/* create clients for all addresses.  Each token is either a host name
 * or a hostname follwed by a slash and then an IP port.
 * Defaults to 5555 if not specified.
 */
int ParseCommands(argc, argv, acx, cspp)
  int argc;
  char **argv;
  struct wlr_context *acx;
  struct wlr_clientSet **cspp;
{
    struct sockaddr_in taddr;
    int i, code;
    struct wlr_clientSet *clsp;
    struct wlr_client *clp;

    clsp = (struct wlr_clientSet *) malloc(sizeof(struct wlr_clientSet));
    wlr_ClientSetInit(clsp);

    acx->count = 1;	/* default */
    for(i=1; i<argc; i++) {
	if (*argv[i] == '-') {
	    /* special command line switch */
	    if (!strcmp(argv[i], "-count")) {
		acx->count = atoi(argv[i+1]);
		i++;
		continue;
	    }
	    else {
		printf("hub: bad switch '%s'\n", argv[i]);
		exit(1);
	    }
	}
	code = ParseHost(argv[i], &taddr);
	if (code) {
	    printf("ParseHost failed, code %d.\n", code);
	    exit(1);
	}
	code = wlr_NewClient(&taddr, argv[i], &clp);
	if (code) {
	    printf("NewClient failed (code %d)\n", code);
	    exit(1);
	}
	code = wlr_ClientSetAdd(clsp, clp);
	if (code) {
	    printf("ClientSetAdd failed, code %d\n", code);
	    exit(1);
	}
    }

    /* now return things */
    *cspp = clsp;
    return 0;
}
