/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ncsio.c,v $
 * Revision 1.1.4.2  1996/02/17  23:23:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:58  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:14:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:09  root]
 * 
 * Revision 1.1.2.2  1993/01/13  19:53:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:21:30  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:42:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/uio.h>

/*
#include <commonp.h>
#include <com.h>
#include <dce/ker/krpc_helper.h>
*/


extern ncskreadv();
extern ncskwritev();
extern krpc_helper_init();
extern int errno =0;

#define	NCSKREADV(iov,iovcnt,rc)				\
	while ( (rc = ncskreadv(iov,iovcnt)) == -1)		\
		if (errno != ERESTART)				\
			break;

ncsread(buf, count)
char *		buf;
int		count;
{
	struct iovec	iov;
	int		rc;
        errno = 0;


	iov.iov_base = buf;
	iov.iov_len = count;

	NCSKREADV(&iov,1,rc);
	return rc;
}


#define	NCSKWRITEV(iov,iovcnt,rc)				\
	while ( (rc = ncskwritev(iov,iovcnt)) == -1)	        \
		if (errno != ERESTART)				\
			break;

ncswrite(buf, count)
char *		buf;
int		count;
{
	struct iovec	iov;
	int		rc;

        errno = 0;
	iov.iov_base = buf;
	iov.iov_len = count;

	NCSKWRITEV(&iov,1,rc);
	return rc;
}



ncsopen()
{
   krpc_helper_init();
   return(0);
}
