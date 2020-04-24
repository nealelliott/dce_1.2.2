/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * spoke.c -- 
 *    The client side of the cache consistency tests.  It executes commands
 *    from the server (hub).
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

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#if sun
#include <sys/dirent.h>
#endif
#ifdef __hpux
#include <ndir.h>
#define utimes(p, t) utime(p, t)
#endif

#include "spoke.h"
#include "openFlags.h"

int wlr_Dispatch();

main(argc, argv)
  int argc;
  char **argv;
{
    int s;
    int code;
    int length;
    struct sockaddr_in myaddr;

    umask(0);
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s<0) {
	perror("socket");
	exit(1);
    }
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(0);	/* my own address */
    myaddr.sin_port = htons(5555);	/* try to get this address */
    code = bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr));
    if (code) {
	/* try the bind with a port of 0 */
	myaddr.sin_port = htons(0);	/* use any */
	code = bind(s, (struct sockaddr *)&myaddr, sizeof(myaddr));
	if (code) {
	    perror("bind");
	    exit(1);
	}
    }
    code = listen(s, 10);
    if (code) wlr_panic("listen");
    /* if we get here, we have a port to use.  Find out what it
     * is and print a message for our user.
     */
    length = sizeof(myaddr);
    code = getsockname(s, (struct sockaddr *)&myaddr, &length);
    if (code) {
	perror("getsockname");
	exit(1);
    }
    printf("wheel (pid %d) using IP port %d\n", getpid(), 
	   ntohs(myaddr.sin_port));

    /* close stdout so that any pipes listening for my pid/port
       information will be able to continue */
    fclose(stdout);
    wlr_ServerListen(s, wlr_Dispatch);
    exit(0);
}

wlr_ServerChmod(callp, apathp, amode)
  struct wlr_call *callp;
  long amode;
  char *apathp;
{
    int code;
    code = chmod(apathp, amode);
    return (code < 0? errno : 0);
}

wlr_ServerStat(callp, apathp, astatp)
  struct wlr_call *callp;
  char *apathp;
  struct stat *astatp;
{
    long code;

    code = stat(apathp, astatp);
    if (code < 0) return errno;
    return 0;
}

/* this really needs to be fixed up.  Right now, we're returning
 * raw file descriptors to the hub, without any validation being
 * done when they're passed in again.  We really need to add
 * parameter validation.
 */
wlr_ServerOpen(callp, apathp, aflag, amode, afdp)
  struct wlr_call *callp;
  long amode;
  long aflag;
  char *apathp;
  long *afdp;
{
    int fd;
    long realFlags = 0;

    /*
     *  Convert open flags from our bit values to the local machines.
     */
    if (aflag & WLR_O_CREAT)
	realFlags |= O_CREAT;
    if (aflag & WLR_O_RDWR)
	realFlags |= O_RDWR;
    if (aflag & WLR_O_RDONLY)
	realFlags |= O_RDONLY;
    if (aflag & WLR_O_TRUNC)
	realFlags |= O_TRUNC;

    fd = open(apathp, realFlags, amode);
    if (fd < 0) return errno;
    *afdp = wlr_MapOut(fd);
    return 0;
}

wlr_ServerRead(callp, afd, aoffset, alen, adatap, alenp)
  struct wlr_call *callp;
  long afd;
  long aoffset, alen;
  char *adatap;
  long *alenp;
{
    long code;

    afd = wlr_MapIn(afd);
    code = lseek(afd, aoffset, 0);
    if (code < 0) return code;
    code = read(afd, adatap, alen);
    if (code < 0) return code;
    *alenp = code;
    return 0;
}

static wlr_ConvertToStandardDir(inBufferp, outBufferp, inLen, nextOffset,
				outLenp)
  struct wlr_localdirent *inBufferp;
  struct wlr_netdirent *outBufferp;
  long inLen;
  long nextOffset;
  long *outLenp;
{
    long newBytes, irl, orl;
    short nameLength;

    newBytes = 0;
    while (inLen > 0) {
	irl = inBufferp->recLength;
	outBufferp->inode = htonl(inBufferp->inode);
#if sun
	/* compute the name length for Solaris */
	nameLength = inBufferp->recLength - sizeof(struct dirent) + sizeof(char);
#else /* sun */
	nameLength = inBufferp->nameLength;
#endif /* sun */
	outBufferp->nameLength = htons(nameLength);
	strcpy(outBufferp->name, inBufferp->name);
#if defined(OSF1) || defined(__hpux)
	outBufferp->offset = htonl(nextOffset);
#else
	outBufferp->offset = htonl(inBufferp->offset);
#endif
	/* add one for the null at the end of the string */
	orl = nameLength+1 + 2*sizeof(long) + 2*sizeof(short);
	orl = ((orl-1) | 3) + 1;
	outBufferp->recLength = htons(orl);
	newBytes += orl;
	inLen -= irl;
	/* now move pointers by the right amount */
	inBufferp = (struct wlr_localdirent *) (((long)inBufferp) + irl);
	outBufferp = (struct wlr_netdirent *) (((long)outBufferp) + orl);
    }
    *outLenp = newBytes;
    return 0;
}

wlr_ServerReaddir(callp, afd, aoffset, alen, adatap, alenp)
  struct wlr_call *callp;
  long afd;
  long aoffset, alen;
  char *adatap;
  long *alenp;
{
    long code;
    long nextOffset;
    long base;
    long newLength;
    char buffer[WLR_BLOCKSIZE];

    afd = wlr_MapIn(afd);
    code = lseek(afd, aoffset, 0);
    if (code < 0) return code;
    /* everyone has their favorite way of getting directory
     * entries out of a Unix kernel.  Chapter 3 has a standard
     * interface, but we need something that'll work with our
     * file descriptors.  At least, it'd be easier than building
     * remote opendir calls, too.
     */
#ifdef sun
    code = getdents(afd, (struct dirent *)buffer, alen);
#endif
#ifdef _AIX
    code = readx(afd, buffer, alen, /* return dirent structure */ 1);
#endif
#if defined(OSF1) || defined(__hpux)
    code = getdirentries(afd, (struct direct *)buffer, alen, &base);
#endif
    if (code < 0) return code;

#ifdef OSF1
    /* OSF1 doesn't have an offset field, so we need to get something
     * for caller.
     */
    nextOffset = tell(afd);
#endif /* OSF1 */
#ifdef __hpux
    /* HPUX doesn't have an offset field, so we need to get something
     * for caller.
     */
    nextOffset = lseek(afd, 0, SEEK_CUR);
#endif

    code = wlr_ConvertToStandardDir((struct wlr_localdirent *) buffer,
				    (struct wlr_netdirent *) adatap,
				    code, nextOffset, &newLength);
    if (code) return code;

    *alenp = newLength;
    return 0;
}

wlr_ServerWrite(callp, afd, aoffset, alen, adatap, alenp)
  struct wlr_call *callp;
  long afd;
  long aoffset, alen;
  char *adatap;
  long *alenp;
{
    long code;

    afd = wlr_MapIn(afd);
    code = lseek(afd, aoffset, 0);
    if (code < 0) return code;
    code = write(afd, adatap, alen);
    if (code < 0) return code;
    *alenp = code;
    return 0;
}

wlr_ServerFtrunc(callp, afd, asize)
  struct wlr_call *callp;
  long afd;
  long asize;
{
    int code;

    afd = wlr_MapIn(afd);
    code = ftruncate(afd, asize);
    return (code < 0? errno : code);
}

wlr_ServerFstat(callp, afd, astatp)
  struct wlr_call *callp;
  long afd;
  struct stat *astatp;
{
    int code;
    struct stat tstat;

    afd = wlr_MapIn(afd);
    code = fstat(afd, astatp);
    if (code < 0)
	return errno;
    return 0;
}

wlr_ServerClose(callp, afd)
  struct wlr_call *callp;
  long afd;
{
    int code;
    code = close(afd);
    return (code < 0? errno : code);
}

wlr_ServerTrunc(callp, apathp, alen)
  struct wlr_call *callp;
  long alen;
  char *apathp;
{
    int code;

    code = truncate(apathp, alen);
    return (code < 0? errno : code);
}

wlr_ServerRename(callp, apathp1, apathp2)
  struct wlr_call *callp;
  char *apathp1, *apathp2;
{
    int code;

    code = rename(apathp1, apathp2);
    return (code < 0? errno : code);
}

wlr_ServerUnlink(callp, apathp)
  struct wlr_call *callp;
  char *apathp;
{
    int code;

    code = unlink(apathp);
    return (code < 0? errno : code);
}

wlr_ServerLink(callp, apathp1, apathp2)
  struct wlr_call *callp;
  char *apathp1, *apathp2;
{
    int code;

    code = link(apathp1, apathp2);
    return (code < 0? errno : code);
}

wlr_ServerSymlink(callp, apathp1, apathp2)
  struct wlr_call *callp;
  char *apathp1, *apathp2;
{
    int code;

    code = symlink(apathp1, apathp2);
    return (code < 0? errno : code);
}

wlr_ServerMakedir(callp, apathp, amode)
  struct wlr_call *callp;
  char *apathp;
  long amode;
{
    int code;

    code = mkdir(apathp, amode);
    return (code < 0? errno : code);
}

wlr_ServerRmdir(callp, apathp)
  struct wlr_call *callp;
  char *apathp;
{
    int code;

    code = rmdir(apathp);
    return (code < 0? errno : code);
}

wlr_ServerUtimes(callp, apathp, amt, amtu, aat, aatu)
  struct wlr_call *callp;
  char *apathp;
  long amt, aat;
  long amtu, aatu;
{
    int code;
    struct timeval tv[2];
    
    tv[0].tv_sec = aat;
    tv[1].tv_sec = amt;
    tv[0].tv_usec = amtu;
    tv[1].tv_usec = aatu;
    code = utimes(apathp, tv);
    return (code < 0? errno : code);
}

wlr_ServerReadlink(callp, apathp, aresultp, alenp)
  struct wlr_call *callp;
  char *apathp;
  char *aresultp;
  long *alenp;
{
    int code;

    code = readlink(apathp, aresultp, WLR_STRINGSIZE);
    if (code < 0) return errno;
    *alenp = code;
    return 0;
}
