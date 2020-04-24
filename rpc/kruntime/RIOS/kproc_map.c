/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kproc_map.c,v $
 * Revision 1.1.4.2  1996/02/18  00:00:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:41  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:14:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:07  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:35:53  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:51:47  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:38:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:52:31  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:15:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *
 * The NCK threads model is based upon a single process with multiple
 * threads of execution sharing the process.  This implies that threads share
 * all of the single process' resources, in particular, file descriptors.
 *
 * The NCK kernel kluge maps threads 1-1 with kprocs.  File descriptors are
 * therefor not shared.  We attempt to address this here.
 *
 */
#include <sys/types.h>
#include <sys/user.h>
#include <sys/file.h>
#include <sys/errno.h>
#include <sys/fp_io.h>
#include <sys/syspest.h>
#include <sys/lockl.h>
#include <sys/ioctl.h>

#include <commonp.h>
#define	NFILE	100			/* max of 100 files supported	*/

struct file *x_ofile[NFILE];
int maxf = NFILE;			/* max file table slot used	*/
int minf = 0;				/* minimum free slot		*/
lock_t xf_lock = LOCK_AVAIL;		/* serialise access to above	*/

#define	sys_setup(monster, jmpbuf) {			\
	monster = lockl(&kernel_lock,LOCK_SHORT);	\
							\
	switch (rc = setjmpx(&jmpbuf)) {		\
	    case 0:					\
		break;					\
							\
	    case EINTR:					\
		errno = EINTR;				\
		if (monster != LOCK_NEST)		\
			unlockl(&kernel_lock);		\
							\
		return -1;				\
							\
	    default:					\
		if (monster != LOCK_NEST)		\
			unlockl(&kernel_lock);		\
		longjmpx(rc);				\
	}						\
}

#define sys_err(jmpbuf, rc) {		\
	clrjmpx(&jmpbuf);		\
	errno = rc;			\
	if (monster != LOCK_NEST)	\
		unlockl(&kernel_lock);	\
}

#define sys_ok(monster, jmpbuf) {	\
	clrjmpx(&jmpbuf);		\
	if (monster != LOCK_NEST)	\
		unlockl(&kernel_lock);	\
}

/*
 * xfdalloc -	my version of fdalloc()
 *
 * Input:
 *	fdp	-	^ to place to return FD
 *
 * Returns:
 *	 0	-	 OK: *fpd set
 *	!0	-	!OK: errno
 */
xfdalloc(fdp)
int *fdp; {
	register struct file **fpp, **end;
        static struct file null;
        int rc;
        int i=0;

	rc = lockl(&xf_lock, LOCK_SHORT);

	for (fpp = &x_ofile[minf], end = &x_ofile[maxf]; fpp < end; ++fpp, ++i)
        {
		if (*fpp == 0)
			break;
        }

	if (fpp >= end) {
		/*
		 * no free slot found
		 */
		if (++maxf > NFILE) {
                   --maxf;
                if (rc != LOCK_NEST)
                    unlockl(&xf_lock);
                   return EMFILE;
		}
	}

	assert(fpp < &x_ofile[maxf] && *fpp == 0);

	*fdp = fpp - x_ofile;
        *fpp = &null;

        if (rc != LOCK_NEST)
	   unlockl(&xf_lock);

	return 0;
}

/*
 * xfdfree -	free a file descriptor
 */
xfdfree(fd) {
        int i, rc;

	rc = lockl(&xf_lock, LOCK_SHORT);

	if (minf > fd)
		minf = fd;

	x_ofile[fd] = 0;

        if (rc != LOCK_NEST)
	   unlockl(&xf_lock);
}

/*
 * xgetf -	map FD to FP
 *
 * Input:
 *	fd	-	file descriptor
 *	fpp	-	^ to place to return mapping
 *
 * Returns:
 *	 0	-	 OK: *fpp set
 *	!0	-	!OK: errno
 */
x_getf(fd, fpp)
register struct file **fpp; {
   int rc;

	rc = lockl(&xf_lock, LOCK_SHORT);
	if (!(0 <= fd && fd < maxf && (*fpp = x_ofile[fd])))
        {
           if (rc != LOCK_NEST)
	      unlockl(&xf_lock);
           return EBADF;
        }

        if (rc != LOCK_NEST)
	   unlockl(&xf_lock);
	return 0;
}


/*
 * x_donate - donate a FD from a kproc to the common pool
 *
 * Input:
 *	fd	-	file descriptor to donate
 */
x_donate(fd) {
	struct file *fp;
	int fe, rc;

        
	assert(xfdalloc(&fe) == 0);
	x_ofile[fe] = u.u_ofile(fd);
        u.u_ufd[fd].fp = NULL;
	return fe;
}

x_socket(dom, type, proto)
int dom, type, proto;
{
   int fd;

   if ((fd =  socket(dom, type, proto)) >= 0)
       return x_donate(fd);
   
   return -1;
}
   
