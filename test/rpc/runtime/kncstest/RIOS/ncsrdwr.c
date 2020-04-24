/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ncsrdwr.c,v $
 * Revision 1.1.4.2  1996/02/17  23:23:30  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:59  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:14:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:09  root]
 * 
 * Revision 1.1.2.2  1993/01/13  19:54:02  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:21:37  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:42:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <sys/user.h>
#include <sys/uio.h>
#include <sys/errno.h>
#include <sys/sysinfo.h>
#include <sys/syspest.h>
#include <sys/lockl.h>
#include <sys/malloc.h>


/*
 * NAME:	ncskreadv()
 *
 *
 * PARMETERS:	
 *		iov	pointer to array of iovec structures
 *		iovcnt	number of iovecs
 *	
 *
 * RETURN VALUE:	success: number of bytes actually read
 *			failure: -1 with u.u_error set
 */

ncskreadv(iovp,iovcnt)
struct iovec *	iovp;
int		iovcnt;
{
	int lockt, rv;
	struct iovec iov[16];		/* XXX */


	if (iovcnt > sizeof(iov)/sizeof(struct iovec)
	||  iovcnt <= 0)
	{
		u.u_error = EINVAL;
		return -1;
	}

	if (copyin((caddr_t)iovp, (caddr_t)iov,
		iovcnt * sizeof (struct iovec)))
	{
		u.u_error = EFAULT;
		return -1;
	}

	ncsrdwr(iov,iovcnt,UIO_READ,&rv);

	return u.u_error ? -1 : rv;
}

ncskwritev(iovp,iovcnt)
struct iovec *	iovp;
int		iovcnt;
{
	int lockt, rv;
	struct iovec iov[16];		/* XXX */

	if (iovcnt > sizeof(iov)/sizeof(struct iovec)
	||  iovcnt <= 0)
	{
		u.u_error = EINVAL;
		return -1;
	}

	if (copyin((caddr_t)iovp, (caddr_t)iov,
		iovcnt * sizeof (struct iovec)))
	{
		u.u_error = EFAULT;
		return -1;
	}


	ncsrdwr(iov,iovcnt,UIO_WRITE,&rv);

	return u.u_error ? -1 : rv;
}


/*
 * Common setup for call to filesystem dependent read/write routine.
 */
static
ncsrdwr (
register struct iovec	*iov,
register int		iovcnt,
enum uio_rw		rw,
int *			countp)
{
	u.u_error = ncsrwuio(iov, iovcnt, UIO_USERSPACE, rw, countp);
}


static
int
ncsrwuio (iov, iovcnt, seg, rw, countp)
register struct iovec	*iov;		/* vector of I/O buffers	*/
register int		iovcnt;		/* number of I/O buffers	*/
int			seg;		/* segment with I/O buffers	*/
register enum uio_rw	rw;		/* dir:  UIO_READ or UIO_WRITE	*/
register int		*countp;	/* return of I/O byte count	*/
{
	struct uio	auio;	/* uio structure describing I/O		*/
	int		i;	/* loop counter over buffers		*/
	int		rc;	/* return code				*/
	int		count;	/* count of bytes read or written	*/
	label_t		jbuf;	/* execption return point		*/


	auio.uio_iov	= iov;
	auio.uio_iovcnt	= iovcnt;
	auio.uio_segflg	= seg;
	auio.uio_resid	= 0;

	for (i = 0; i < auio.uio_iovcnt; i++) {
		if (iov->iov_len < 0)
			return EINVAL;
		auio.uio_resid += iov->iov_len;
		if (auio.uio_resid < 0)
			return EINVAL;
		iov++;
	}

	count = auio.uio_resid;

	/*
	* Establish an exception handler to catch interrupted
	* reads and writes to "slow" objects (not files).
	*/

	if ((rc=setjmpx(&jbuf)) == 0)
	{
                if (rw == UIO_READ)
                {
	       	   rc = krpc_helperGetRequest(&auio);
                }
                else
                {
                   rc = krpc_helperSendResponse(&auio);
                }
		clrjmpx(&jbuf);
	}
	else if (rc != EINTR)
		longjmpx(rc);			/* certain death!	*/

	/* check for interrupted system call */

	if (rc == EINTR)			/* interrupted?		*/
		if (count == auio.uio_resid)	/* no data transferred?	*/
			rc = ERESTART;		/* restart (perhaps)	*/
		else
			rc = 0;			/* partial transfer	*/

	count -= auio.uio_resid;

	if (countp)
		*countp = count;

	return rc;
}
