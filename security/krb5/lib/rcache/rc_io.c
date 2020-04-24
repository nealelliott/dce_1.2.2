/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rc_io.c,v $
 * Revision 1.1.12.2  1996/02/18  00:16:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:05:16  marty]
 *
 * Revision 1.1.12.1  1995/12/08  17:44:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:51  root]
 * 
 * Revision 1.1.9.2  1993/10/18  20:36:43  hanfei
 * 	[OT 8012] add missing break statements for error handling case.
 * 	[1993/10/18  20:36:11  hanfei]
 * 
 * Revision 1.1.9.1  1993/10/01  17:01:08  hanfei
 * 	[OT 8012] replay cache should reside under /opt/dcelocal.
 * 	[1993/10/01  17:00:35  hanfei]
 * 
 * Revision 1.1.6.2  1993/08/16  19:55:58  hanfei
 * 	[7942] fixed for replay cache logic problems.
 * 	[1993/08/16  19:48:39  hanfei]
 * 
 * Revision 1.1.4.2  1992/12/29  16:32:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:19:55  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/29  20:57:56  sommerfeld
 * 	Make "dir" and "dirlen" static to avoid namespace pollution problems.
 * 	[1992/06/26  22:47:35  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/05/08  17:19:11  sommerfeld
 * 	Split "write" and "sync" into two operations for increased efficiency.
 * 	[1992/05/05  02:45:19  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:52:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rc_io.c V=5 10/24/91 //littl/prgy/krb5/lib/rcache
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/rcache/rc_io.c,v $
 * $Author: marty $
 *
 * This file of the Kerberos V5 software is derived from public-domain code
 * contributed by Daniel J. Bernstein, <brnstnd@acf10.nyu.edu>.
 *
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_rc_io_c[] =
"$Id: rc_io.c,v 1.1.12.2 1996/02/18 00:16:03 marty Exp $";
#endif	/* !lint & !SABER */

/*
 * I/O functions for the replay cache default implementation.
 */


#define KRB5_RC_VNO	0x0501		/* krb5, rcache v 1 */
#define KRB5_RC_DIR_PROT S_IRWXU+S_IRGRP+S_IXGRP+S_IROTH+S_IXOTH /* set dir access mode */

#include <stdio.h> /* for P_tmpdir */

#include <krb5/sysincl.h>
#include "rc_base.h"
#include "rc_dfl.h"
#include "rc_io.h"
#include <krb5/osconf.h>
#include <sys/stat.h>

#ifdef KRB5_USE_INET
#include <netinet/in.h>
#else
 #error find some way to use net-byte-order file version numbers.
#endif

extern int errno; /* this should be in errno.h, but isn't on some systems */

#define FREE(x) ((void) free((char *) (x)))
#define UNIQUE getpid() /* hopefully unique number */

static int dirlen = 0;
static char *dir;
static int getdir_st = 0;

/* The do ... while(0) is required to insure that GETDIR looks like a
   single statement in all situations (just {}'s may cause troubles in
   certain situations, such as nested if/else clauses. */

#define GETDIR do { if (!dirlen) getdir(); } while(0)

static void getdir()
{
 if (!dirlen)
  {
   if (!(dir = getenv("KRB5RCACHEDIR")))
     if (!(dir = getenv("TMPDIR")))
#ifdef RCTMPDIR
       dir = RCTMPDIR;
#else
       dir = "/tmp";
#endif
   getdir_st = 0;
   if ( mkdir(dir, KRB5_RC_DIR_PROT) == -1 )
     if ( errno != EEXIST ) {
        switch (errno) {
        case ENOSPC:
            getdir_st = KRB5_RC_IO_SPACE;
	    break;
        case EACCES:
            getdir_st = KRB5_RC_IO_PERM;
	    break;
	default:
	    break;
	}
     }
   dirlen = strlen(dir) + 1;
  }
}

krb5_error_code krb5_rc_io_creat (d, fn)
    krb5_rc_iostuff *d;
    char **fn;
{
    char *c;
    krb5_int16 rc_vno = htons(KRB5_RC_VNO);
    krb5_error_code retval;

    GETDIR;
    if ( getdir_st != 0 )
	return (getdir_st);

    if (fn && *fn)
    {
	if (!(d->fn = malloc(strlen(*fn) + dirlen + 1)))
	    return KRB5_RC_IO_MALLOC;
	(void) strcpy(d->fn,dir);
	(void) strcat(d->fn,"/");
	(void) strcat(d->fn,*fn);
	d->fd = open(d->fn,O_WRONLY | O_CREAT | O_TRUNC | O_EXCL,0600);
    }
    else
    {
	/* %d is max 11 digits (-, 10 digits of 32-bit number)
	 * 11 + /krb5_RC + aaa = 24, +6 for slop */
	if (!(d->fn = malloc(30 + dirlen)))
	    return KRB5_RC_IO_MALLOC;
	if (fn)
	    if (!(*fn = malloc(35)))
	    { FREE(d->fn); return KRB5_RC_IO_MALLOC; }
	(void) sprintf(d->fn,"%s/krb5_RC%d",dir,UNIQUE);
	c = d->fn + strlen(d->fn);
	(void) strcpy(c,"aaa");
	while ((d->fd = open(d->fn,O_WRONLY|O_CREAT|O_TRUNC|O_EXCL,0600)) == -1)
	{
	    if ((c[2]++) == 'z')
	    {
		c[2] = 'a';
		if ((c[1]++) == 'z')
		{
		    c[1] = 'a';
		    if ((c[0]++) == 'z')
			break;	/* sigh */
		}
	    }
	}
	if (fn)
	    (void) strcpy(*fn,d->fn + dirlen);
    }
    if (d->fd == -1)
    {
	switch(errno)
	{
	case EFBIG: 
#ifdef EDQUOT
	case EDQUOT:
#endif
	case ENOSPC: 
	    retval = KRB5_RC_IO_SPACE; 
	    goto fail;
	case EIO: 
	    retval = KRB5_RC_IO_IO; goto fail;

	case EPERM: 
	case EACCES: 
	case EROFS: 
	case EEXIST: 
	    retval = KRB5_RC_IO_PERM; goto fail;

	default: 
	    retval = KRB5_RC_IO_UNKNOWN; goto fail;
	}
    }
    if ((retval = krb5_rc_io_write(d, (krb5_pointer)&rc_vno, sizeof(rc_vno))) ||
	(retval = krb5_rc_io_sync(d)))
    {
    fail:
	(void) unlink(d->fn);
	FREE(d->fn);
	d->fn = NULL;
	(void) close(d->fd);
	return retval;
    }
    return 0;
}

krb5_error_code krb5_rc_io_open (d, fn)
krb5_rc_iostuff *d;
char *fn;
{
    krb5_int16 rc_vno;
    krb5_error_code retval;
    struct stat statb;

    GETDIR;
    if ( getdir_st != 0 )
	return (getdir_st);

    if (!(d->fn = malloc(strlen(fn) + dirlen + 1)))
	return KRB5_RC_IO_MALLOC;
    (void) strcpy(d->fn,dir);
    (void) strcat(d->fn,"/");
    (void) strcat(d->fn,fn);
    if ((d->fd = stat(d->fn, &statb)) != -1) {
	uid_t me;

	me = getuid();
	/* must be owned by this user, to prevent some security problems with
	 * other users modifying replay cache stufff */
	if ((statb.st_uid != me) || ((statb.st_mode & S_IFMT) != S_IFREG)) {
	    FREE(d->fn);
	    return KRB5_RC_IO_PERM;
	}
	d->fd = open(d->fn,O_RDWR,0600);
    }
    if (d->fd == -1) {
	switch(errno)
	{
	case EFBIG: 
#ifdef EDQUOT
	case EDQUOT:
#endif
	case ENOSPC:
	    retval = KRB5_RC_IO_SPACE; 
	    goto fail;

	case EIO: 
	    retval = KRB5_RC_IO_IO; 
	    goto fail;

	case EPERM: 
	case EACCES:
	case EROFS: 
	    retval = KRB5_RC_IO_PERM; 
	    goto fail;

	    default: 
	    retval = KRB5_RC_IO_UNKNOWN; 
	    goto fail;
	}
    }
    if (retval = krb5_rc_io_read(d, (krb5_pointer) &rc_vno,  sizeof(rc_vno)))  
	goto unlk;


    if (ntohs(rc_vno) != KRB5_RC_VNO) 
    {
	retval = KRB5_RCACHE_BADVNO;
    unlk:
	unlink(d->fn);
    fail:
	(void) close(d->fd);
	FREE(d->fn); 
	d->fn = NULL;
	return retval;
    }
    return 0;
}

krb5_error_code krb5_rc_io_move (new, old)
krb5_rc_iostuff *new;
krb5_rc_iostuff *old;
{
    if (rename(old->fn,new->fn) == -1) /* MUST be atomic! */
	return KRB5_RC_IO_UNKNOWN;
    if (close(new->fd) == -1)	/* can't happen */
	return KRB5_RC_IO_UNKNOWN;
    new->fd = old->fd;
    return 0;
}

krb5_error_code krb5_rc_io_write (d, buf, num)
krb5_rc_iostuff *d;
krb5_pointer buf;
int num;
{
    if (write(d->fd,(char *) buf,num) == -1)
	switch(errno)
	{
	case EBADF: return KRB5_RC_IO_UNKNOWN; break;
	case EFBIG: return KRB5_RC_IO_SPACE; break;
#ifdef EDQUOT
	case EDQUOT: return KRB5_RC_IO_SPACE; break;
#endif
	case ENOSPC: return KRB5_RC_IO_SPACE; break;
	case EIO: return KRB5_RC_IO_IO; break;
	    default: return KRB5_RC_IO_UNKNOWN; break;
	}
    return 0;
}

krb5_error_code krb5_rc_io_sync (d)
    krb5_rc_iostuff *d;
{
    
    if (fsync(d->fd) == -1)
    {
	switch(errno)
	{
	case EBADF: return KRB5_RC_IO_UNKNOWN; break;
	case EIO: return KRB5_RC_IO_IO; break;
	    default: return KRB5_RC_IO_UNKNOWN; break;
	}
    }
    return 0;
}

krb5_error_code krb5_rc_io_read (d, buf, num)
krb5_rc_iostuff *d;
krb5_pointer buf;
int num;
{
 int count;
 if ((count = read(d->fd,(char *) buf,num)) == -1)
   switch(errno)
    {
     case EBADF: return KRB5_RC_IO_UNKNOWN; break;
     case EIO: return KRB5_RC_IO_IO; break;
     default: return KRB5_RC_IO_UNKNOWN; break;
    }
 if (count == 0)
     return KRB5_RC_IO_EOF;
 return 0;
}

krb5_error_code krb5_rc_io_close (d)
krb5_rc_iostuff *d;
{
    FREE(d->fn);
    d->fn = NULL;
    if (close(d->fd) == -1)	/* can't happen */
	return KRB5_RC_IO_UNKNOWN;
    return 0;
}

krb5_error_code krb5_rc_io_destroy (d)
krb5_rc_iostuff *d;
{
    if (unlink(d->fn) == -1)
	switch(errno)
	{
	case EBADF: return KRB5_RC_IO_UNKNOWN; break;
	case EIO: return KRB5_RC_IO_IO; break;
	case EPERM: return KRB5_RC_IO_PERM; break;
	case EBUSY: return KRB5_RC_IO_PERM; break;
	case EROFS: return KRB5_RC_IO_PERM; break;
	    default: return KRB5_RC_IO_UNKNOWN; break;
	}
    return 0;
}

krb5_error_code krb5_rc_io_mark (d)
krb5_rc_iostuff *d;
{
    d->mark = lseek(d->fd,0,L_INCR); /* can't fail */
    return 0;
}

krb5_error_code krb5_rc_io_unmark (d)
krb5_rc_iostuff *d;
{
    (void) lseek(d->fd,d->mark,L_SET); /* if it fails, tough luck */
    return 0;
}

int krb5_rc_io_size (d)
    krb5_rc_iostuff *d;
{
    struct stat statb;
    
    if (fstat (d->fd, &statb) == 0)
	return statb.st_size;
    else
	return 0;
}
    
