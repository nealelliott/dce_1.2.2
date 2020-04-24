/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: adver_socket.c,v $
 * Revision 1.1.4.2  1996/02/18  19:23:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:36  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:09:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:07  root]
 * 
 * Revision 1.1.2.4  1994/07/06  20:43:19  ohara
 * 	Merged with changes from 1.1.2.3
 * 	[1994/07/06  20:42:47  ohara]
 * 
 * 	SVR4 update, use sockets for now, change to pipes later
 * 	[1994/07/06  20:37:04  ohara]
 * 
 * Revision 1.1.2.3  1994/06/30  19:14:56  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:34:25  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.2.2  1994/06/09  16:06:41  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:13  devsrc]
 * 
 * Revision 1.1.2.3  1994/06/30  19:14:56  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:34:27  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.2.2  1994/06/09  16:06:42  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:13  devsrc]
 * 
 * Revision 1.1.2.3  1994/06/30  19:14:56  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:34:27  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.2.2  1994/06/09  16:06:42  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:13  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/12  21:09:51  peckham
 * 	Move from adver_sysdep_unix.c to os-dependent module.
 * 	Remove _BSD, AIX and SNI_SVR4_NAMED_PIPES conditionals.
 * 	Add adver_open_socket() from adver_clerk_event.c
 * 	[1994/05/12  19:06:44  peckham]
 * 
 * $EndLog$
 */
/*
 *	Module: adver_socket.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */

#include <adver.h>
#include <dce/utc.h>
#include <fcntl.h>
#include <signal.h>

int
adver_open_socket (struct sockaddr_un *sa, int q)
{
    int s;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		  " >adver_open_socket(%s,%d)", sa->sun_path, q));

    /*
     * Make sure the socket is created
     * before the child is created -
     * there are cases where the name was passed back
     * to the library before it was created.
     */
    if ((s = dthread_socket(AF_UNIX, SOCK_STREAM, PF_UNSPEC)) < 0) 
    {
	const int save_errno = errno;
	dce_svc_printf(CDS_S_ADVER_SOCKET_MSG, (long)errno);
	errno = save_errno;
	return(-1);
    }

    /* make sure old file not in the way */
    (void)unlink(sa->sun_path);
    if (dthread_bind(s, (struct sockaddr *)sa, sizeof(*sa)) < 0) 
    {
	const int save_errno = errno;
	dce_svc_printf(CDS_S_ADVER_BIND_MSG, sa->sun_path, (long)errno);
	(void)close(s);
	errno = save_errno;
	return(-1);
    }

    if (dthread_listen(s, q) < 0) 
    {
	const int save_errno = errno;
	dce_svc_printf(CDS_S_ADVER_LISTEN_MSG, sa->sun_path, (long)errno);

	(void)close(s);
	(void)unlink(sa->sun_path);
	errno = save_errno;
	return(-1);
    }

    return(s);
}

/*
 * Accept a connection
 */
int
adver_accept_socket (int s,
		     struct sockaddr_un *sa,
		     int *len)
{
    return dthread_accept(s, (struct sockaddr *)sa, len);
}

/*
 * Read from that socket
 */
int
adver_read_socket (int s,
		   char *buf,
		   int len)
{
    return dthread_read(s, buf, len);
}

/*
 * Write to that socket
 */
int
adver_write_socket (int s,
		    const char *buf,
		    int len)
{
    return dthread_write(s, buf, len);
}







