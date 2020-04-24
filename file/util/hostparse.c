/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * P_R_P_Q_# (C) COPYRIGHT TRANSARC CORPORATION 1995, 1989
 * LICENSED MATERIALS - PROPERTY OF TRANSARC
 * ALL RIGHTS RESERVED
 */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <arpa/inet.h>
#include <pthread.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/util/RCS/hostparse.c,v 4.12 1995/07/26 16:09:42 blake Exp $")

struct hostent *hostutil_GetHostByName(ahost)
register char *ahost; {
  struct hostent *he;

  pthread_lock_global_np();
  he = gethostbyname(ahost);
  pthread_unlock_global_np();
  return (he);
}

/* translate an internet address into a nice printable string.  The variable addr is
   in network byte order */
char *hostutil_GetNameByINet(addr)
struct in_addr addr; {
    static char tbuffer[MAXHOSTNAMELEN+1];
    register struct hostent *th;
    char *hName;

    pthread_lock_global_np();
    bzero((char *)tbuffer, MAXHOSTNAMELEN+1);
    th = gethostbyaddr((const char *)&addr, (int)sizeof(struct in_addr),
		       (int)AF_INET);
    if (th != (struct hostent *)NULL) {
	strcpy(tbuffer, th->h_name);
    }
    else if ((hName = inet_ntoa(addr)) != (char *)NULL)
      strcpy(tbuffer, hName);
    pthread_unlock_global_np();
    return tbuffer;
}
