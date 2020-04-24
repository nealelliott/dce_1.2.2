/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: phys.c,v $
 * Revision 1.1.44.1  1996/10/02  17:55:38  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:40  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1990, 1996 Transarc Corporation - All rights reserved */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <dcedfs/param.h>
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <pthread.h>
#include <dcedfs/compat.h>

#define	UBIK_INTERNALS 1
#include <ubik.h>

/* these routines are called via the proc ptr in the ubik_dbase structure.  They provide access to
 * the physical disk, by converting the file numbers being processed (>= 0 for user data space, < 0
 * for ubik system files, such as the log) to actual pathnames to open, read, write, truncate, sync,
 * etc.
 */

#define	MAXFDCACHE  4
static struct fdcache {
    int fd;
    int fileID;
    int refCount;
} fdcache[MAXFDCACHE];

static char pbuffer[1024];

static int uphys_errorDecode(error) 
  int error;
{
  int code;

  switch (error) {
  case EACCES:
  case EPERM:
    code = UNOACCESS;
    break;
#ifndef AFS_SUNOS5_ENV
  case EDQUOT:
#endif /*AFS_SUNOS5_ENV*/
  case ENOSPC:
    code = UNOSPC;
    break;
  case EIO:
    code = UIOERROR;
    break;
  case EBADF:
    code = UBADF;
    break;
  case EINVAL:
    code = UBADPATH;
    break;
  case ENOENT:
    code = UNOENT;
    break;
  default:
    code = UINTERNAL;
    break;
  }
  return code;
}

/* beware, when using this function, of the header in front of most files */
static int uphys_open(adbase, afid)
    struct ubik_dbase *adbase;
    long afid; {
    char temp[20];
    int fd;
    static int initd;
    int i;
    struct fdcache *tfd;
    struct fdcache *bestfd;

    /* initialize package */
    if (!initd) {
	initd=1;
	tfd=fdcache;
	for(i=0;i<MAXFDCACHE;tfd++,i++) {
	    tfd->fd = -1;	    /* invalid value */
	    tfd->fileID	= -10000;   /* invalid value */
	    tfd->refCount = 0;
	}
    }

    /* scan file descr cache */
    for(tfd=fdcache,i=0; i<MAXFDCACHE; i++,tfd++) {
	if (afid == tfd->fileID	&& tfd->refCount == 0) {    /* don't use open fd */
	    lseek(tfd->fd, 0, 0);   /* reset ptr just like open would have */
	    tfd->refCount++;
	    return tfd->fd;
	}
    }

    /* not found, open it and try to enter in cache */
    strcpy(pbuffer, adbase->pathName);
    strcat(pbuffer, ".DB");
    if (afid < 0) {
	i = -afid;
	strcat(pbuffer, "SYS");
    }
    else i = afid;
    sprintf(temp, "%d", i);
    strcat(pbuffer, temp);
    fd = open(pbuffer, O_CREAT | O_RDWR, 0600);
    if (fd < 0) {
	/* try opening read-only */
	fd = open(pbuffer, O_RDONLY, 0);
	if (fd < 0) return fd;
    }
    
    /* enter it in the cache */
    tfd = fdcache;
    bestfd = (struct fdcache *) 0;
    for(i=0;i<MAXFDCACHE;i++,tfd++) {	/* look for empty slot */
	if (tfd->fd == -1) {
	    bestfd = tfd;
	    break;
	}
    }
    if (!bestfd) {			/* look for reclaimable slot */
	tfd = fdcache;
	for(i=0;i<MAXFDCACHE;i++,tfd++) {
	    if (tfd->refCount == 0) {
		bestfd = tfd;
		break;
	    }
	}
    }
    if (bestfd)	{	    /* found a usable slot */
	tfd = bestfd;
	if (tfd->fd >= 0) close(tfd->fd);
	tfd->fd = fd;
	tfd->refCount = 1;	/* us */
	tfd->fileID = afid;
    }

    /* finally, we're done */
    return fd;
}

/* close the file, maintaining ref count in cache structure */
uphys_close (afd)
int afd; {
    int i;
    struct fdcache *tfd;
    long code;

    if (afd < 0) return UBADF;
    tfd = fdcache;
    for(i=0;i<MAXFDCACHE;i++,tfd++) {
	if (tfd->fd == afd) {
	    tfd->refCount--;
	    return 0;
	}
    }
    if((code=close(afd)) < 0)
      return uphys_errorDecode(errno);
    else
      return code;
}

uphys_stat(adbase, afid, astat)
    struct ubik_stat *astat;
    long afid;
    struct ubik_dbase *adbase; {
    int fd;
    struct stat tstat;
    long code, len;
    
    fd = uphys_open(adbase, afid);
    if (fd < 0) 
        return UBADF;
    code = fstat(fd, &tstat);
    if (code < 0) 
	code = uphys_errorDecode(errno);
    (void)uphys_close(fd);
    if (code)
        return code;
    astat->mtime = tstat.st_mtime;
    len = tstat.st_size-UBIK_HDRSIZE;
    if (len < 0) 
        astat->size = 0;
    else 
        astat->size = len;
    return 0;
}

uphys_read(adbase, afile, abuffer, apos, alength)
    struct ubik_dbase *adbase;
    char *abuffer;
    long apos;
    long afile;
    long alength; 
{
    int fd;
    long code = 0, len;
    off_t off;

    fd = uphys_open(adbase, afile);
    if (fd < 0) 
        return UBADF;

    off = lseek(fd, apos+UBIK_HDRSIZE, 0);
    if (off < 0) {
        code = uphys_errorDecode(errno);
	(void)uphys_close(fd);
	return code;
    }
    len = read(fd, abuffer, alength);
    if (len < 0)
        code = uphys_errorDecode(errno);
    else {
        if (len != alength)
	    code = UEOF;
    }
    (void)uphys_close(fd);
    return code;
}

uphys_write(adbase, afile, abuffer, apos, alength)
    struct ubik_dbase *adbase;
    char *abuffer;
    long apos;
    long afile;
    long alength; 
{
    int fd;
    long code=0;
    long length;
    off_t off;

    fd = uphys_open(adbase, afile);
    if (fd < 0) 
       return UNOENT;
    off = lseek(fd, apos+UBIK_HDRSIZE, 0);
    if (off < 0) {
        code = uphys_errorDecode(errno);
	(void)uphys_close(fd);
	return code;
    }
    length = write(fd, abuffer, alength);
    if (length != alength) 
        code = uphys_errorDecode(errno);
    /*
     * check error on close to make sure write succeeded
     */
    if (code)
      (void)uphys_close(fd);
    else
      code=uphys_close(fd);
    return code;
}

uphys_truncate(adbase, afile, asize)
    long asize;
    struct ubik_dbase *adbase;
    long afile; {
    long code, fd;
    fd = uphys_open(adbase, afile);
    if (fd < 0) 
        return UNOENT;
    code = ftruncate(fd, asize+UBIK_HDRSIZE);
    if (code < 0) 
        code = uphys_errorDecode(errno);
    if (code)
      (void)uphys_close(fd);
    else
      code=uphys_close(fd);
    return code;
}

/* get number of dbase files */
uphys_getnfiles(adbase) {
    /* really should scan dir for data */
    return 1;
}

/* get database label, with aversion in host order */
uphys_getlabel(adbase, afile, aversion)
    struct ubik_dbase *adbase;
    long afile;
    struct ubik_version *aversion; {
    struct ubik_hdr thdr;
    long code = 0, fd, len;

    fd = uphys_open(adbase, afile);
    if (fd < 0) return UNOENT;
    len = read(fd, &thdr, sizeof(thdr));
    if (len != sizeof(thdr)) {
        if (len < 0) 
	    code = uphys_errorDecode(errno);
	else 
	    code = UEOF;
	goto out;
    }
    aversion->epoch = ntohl(thdr.version.epoch);
    aversion->counter=ntohl(thdr.version.counter);
out:
    uphys_close(fd);
    return code;
}

/* label database, with aversion in host order */
uphys_setlabel(adbase, afile, aversion)
    struct ubik_dbase *adbase;
    long afile;
    struct ubik_version *aversion; {
    struct ubik_hdr thdr;
    long len, fd;

    fd = uphys_open(adbase, afile);
    if (fd < 0) 
      return uphys_errorDecode(errno);
    thdr.version.epoch = htonl(aversion->epoch);
    thdr.version.counter = htonl(aversion->counter);
    thdr.magic = htonl((long)UBIK_MAGIC);
    thdr.size = (short)htons((short)UBIK_HDRSIZE);
    thdr.pad1 = (short)0;
    len = write(fd, &thdr, sizeof(thdr));
    fsync(fd);	    /* preserve over crash */
    (void)uphys_close(fd);
    if (len != sizeof(thdr))
      return uphys_errorDecode(errno);
    return 0;
}

uphys_sync(adbase, afile)
    struct ubik_dbase *adbase;
    long afile; 
{
    long code, fd;
    fd = uphys_open(adbase, afile);
    if (fd < 0) 
      return uphys_errorDecode(errno);
    if ((code = fsync(fd)) < 0)
      code = uphys_errorDecode(errno);
    (void)uphys_close(fd);
    return code;
}


