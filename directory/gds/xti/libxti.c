/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: libxti.c,v $
 * Revision 1.1.12.2  1996/02/18  18:24:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:03  marty]
 *
 * Revision 1.1.12.1  1995/12/08  16:17:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:21:57  root]
 * 
 * Revision 1.1.10.1  1994/02/22  19:22:12  marrek
 * 	February 1994 code drop
 * 	[1994/02/17  17:31:00  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:56:10  marrek]
 * 
 * Revision 1.1.8.1  1993/10/14  15:56:53  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  15:56:34  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/09  16:44:47  keutel]
 * 
 * Revision 1.1.6.2  1993/08/11  12:49:57  marrek
 * 	July 1993 code drop.
 * 	[1993/08/04  09:15:33  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  22:06:32  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:49:38  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:12:56  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:36:56  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: libxti.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 18:24:13 $";
#endif

/*                      Dummy library for XTI  

This library is provided for use when building on machines that do not
have a resident libxti.a (or xti functionality otherwise provided, eg in
libc.a).  It provides no function; it only provides a set of external
names which would otherwise go unresolved.

If you want real XTI functionality, modify the Makefile in this 
directory as described therein so as not to make the dummy library.  Such a 
scenario assumes the "real" libxti.a exists in some appropriate place (such as 
/usr/lib).
*/
 
extern int t_errno=0;
#ifdef THREADSAFE
# include <pthread.h>
#endif

#include <xti.h>

int t_accept (int fd, int resfd, struct t_call *call)
{ t_errno=TNOTSUPPORT; return(-1);}

char *t_alloc (int fd, int struct_type, int fields)
{t_errno=TNOTSUPPORT; return((char *)0);}

int t_bind (int fd, struct t_bind *req, struct t_bind *ret)
{t_errno=TNOTSUPPORT; return(-1);}

int t_close (int fd)
{t_errno=TNOTSUPPORT; return(-1);}

int t_connect (int fd, struct t_call *sndcall, struct t_call *rcvcall)
{t_errno=TNOTSUPPORT; return(-1);}

int t_error (char *errmsg)
{t_errno=TNOTSUPPORT; return(-1);}

int t_free (char *ptr, int struct_type)
{t_errno=TNOTSUPPORT; return(-1);}

int t_listen (int fd, struct t_call *call)
{t_errno=TNOTSUPPORT; return(-1);}

int t_look (int fd)
{t_errno=TNOTSUPPORT; return(-1);}

int t_open (char *name, int oflag, struct t_info *info)
{t_errno=TNOTSUPPORT; return(-1);}

int t_rcv (int fd, char *buf, unsigned int nbytes, int *flags)
{t_errno=TNOTSUPPORT; return(-1);}

int t_rcvconnect (int fd, struct t_call *call)
{t_errno=TNOTSUPPORT; return(-1);}

int t_rcvdis (int fd, struct t_discon *discon)
{t_errno=TNOTSUPPORT; return(-1);}

int t_rcvrel (int fd)
{t_errno=TNOTSUPPORT; return(-1);}

int t_snd (int fd, char *buf, unsigned int nbytes, int flags)
{t_errno=TNOTSUPPORT; return(-1);}

int t_snddis (int fd, struct t_call *call)
{t_errno=TNOTSUPPORT; return(-1);}

int t_sndrel (int fd)
{t_errno=TNOTSUPPORT; return(-1);}

int t_sync (int fd)
{t_errno=TNOTSUPPORT; return(-1);}

int t_unbind (int fd)
{t_errno=TNOTSUPPORT; return(-1);}

int t_getstate (int fd)
{t_errno=TNOTSUPPORT; return(-1);}
