/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds_mbx.c,v $
 * Revision 1.1.4.2  1996/02/18  19:31:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:19:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:09  root]
 * 
 * Revision 1.1.2.5  1994/06/30  19:15:49  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:57:12  mccann]
 * 
 * Revision 1.1.2.4  1994/06/09  18:39:50  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:33  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/25  22:22:37  peckham
 * 	Take operations with side-effects out of dce_assert().
 * 	For sys_openmbx() without rcv_fnc, make the connect happen
 * 	immediately instead of the first write().
 * 	That way the caller (ta_adver, sdi_ui) will know immediately
 * 	whether there is someone listening.
 * 	[1994/05/25  22:19:46  peckham]
 * 
 * Revision 1.1.2.2  1994/05/12  21:11:59  peckham
 * 	Make sure SIGPIPE doesn't kill us.
 * 	Before we used signal() on the entire process, but since going
 * 	posix sigaction() is per-thread, so the SIGPIPE ignore was
 * 	not being propagated to where it was needed.
 * 	[1994/05/12  19:16:53  peckham]
 * 
 * Revision 1.1.2.1  1994/04/29  15:50:28  peckham
 * 	Renamed from unix_mbx.c
 * 	Split off SVR4 named pipes.
 * 	Remove unnecessary conditionals.
 * 	Rename SYS_OPENMBX() to sys_openmbx().
 * 	Routine mbx_cleanup_fd() should pass a pointer.
 * 	[1994/04/29  13:57:44  peckham]
 * 
 * $EndLog$
 */
/*
 * Module: cds_mbx.c
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

#include <pthread.h>
#include <unistd.h>
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dce/assert.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <string.h>
#include <dns_malloc.h>
#include <dce/dns_record.h>
#include <dce/dnsmessage.h>
#include <threads_lib.h>
#include <sys_mbx.h>

/*
 * Private definition for the sys_mailbox abstract data type
 */
typedef struct {
    dthread_mutex_t	run_mutex;	/* Mutex for all */
    dthread_t		listener_th;	/* Listener thread */
    struct sockaddr_un	addr;
    mbx_user_routine	rcv;		/* Fnc to execute upon msg receipt */
    byte_t		*buffer_p;	/* Ptr to buffer for this mailbox */
    int			max_size;	/* Max xfer allowed into the buffer */
    int			data_fd;	/* Data file descriptor */
    int			prepend;	/* prepend message lengths */
    char		listener_attach; /* non-zero if listener_th attached */
    char		listener_active; /* non-zero if listener_th active */
} private_mailbox;

#define MBX_LOCK(mbx_p)		{ dthread_lock(&mbx_p->run_mutex); }
#define MBX_END_LOCK(mbx_p)	{ dthread_unlock(&mbx_p->run_mutex); }

/*
 * Forward declarations 
 */

static dthread_address_t
mbx_listener (
    private_mailbox	*);

static void
mbx_listener_mbx_clean (
    private_mailbox     *);

static int
mbx_connect (
    private_mailbox	*);

static int
mbx_read (
    private_mailbox	*,
    byte_t		*,
    int			 );

static int
mbx_write (
    private_mailbox	*,
    byte_t		*,
    int			);

static void
mbx_cleanup_fd (
    const int		*);

/*
 * sys_openmbx
 *
 * This routine creates a UNIX-domain socket using the caller's fully-specified
 * socket pathname and maximum size.
 *
 * A thread is then created to process incoming messages using the caller
 * specified function.
 *
 * Arguments:
 *     name_p   (in)  - pointer to fully-qualified mailbox pathname
 *     max_size (in)  - maximum size of mailbox message
 *     mbx_rcv  (in)  - address of function to process input (can be null)
 *     blkmode  (in)  - if true, prepend 4-byte length to message
 *     thread_attr_p  (in) - message handling thread attributes
 *     new_mbx  (out) - address to return descriptor address
 */
int 
sys_openmbx (byte_t            *name_p,
             int               max_size,
             mbx_user_routine  rcv_fnc,
             int               blkmode,
             dthread_attr_t    *thread_attr_p,
             sys_mailbox       *new_mbx)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    private_mailbox *mbx_p =
	(private_mailbox *)dns_malloc(sizeof(private_mailbox) + max_size);
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >sys_openmbx(%s,%d,,%d,,)",
        name_p,
	max_size,
	blkmode));

    /*
     * Allocate a mailbox control block 
     */
    if (mbx_p) 
    {
	int th_status;

	ZERO_bytes(mbx_p, sizeof(*mbx_p));

	if ((th_status = dthread_create_mutex(&mbx_p->run_mutex)) < 0)
	    dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, th_status);

	--mbx_p->data_fd;
	mbx_p->buffer_p = (byte_t *)&mbx_p[1]; /* point to buffer */
	mbx_p->max_size = max_size;               /* Store buffer size */

	/*
	 * Load the descriptors 
	 */
	mbx_p->addr.sun_family = AF_UNIX;
	mbx_p->addr.sun_path[0] = '\0';
	mbx_p->prepend = blkmode;		/* prepend length? */
	strcpy(mbx_p->addr.sun_path, (char *)name_p);

	/*
	 * If the caller has specified a receive function then
	 * create the message processing thread for this mailbox.
	 */
	if (mbx_p->rcv = rcv_fnc) {
	    static dthread_attr_t attr = {"mbx_listener"};

            if (thread_attr_p == dthread_attr_default) {
                thread_attr_p = &attr;
            }

	    if (0 <= (th_status = dthread_create(&mbx_p->listener_th,
                        thread_attr_p, (dthread_start_routine)mbx_listener,
				(dthread_address_t)mbx_p)))
	    {
		mbx_p->listener_attach = TRUE;
		mbx_p->listener_active = TRUE;
		status = CDS_SUCCESS;
	    }
	    else
	    {
		status = CDS_NONSRESOURCES;
	    }
	}
	else
	{
	    status = mbx_connect(mbx_p);
	}
    }
    else
	status = CDS_NONSRESOURCES;

    if ((status != CDS_SUCCESS) && mbx_p)
    {
	dthread_delete_mutex(&mbx_p->run_mutex);
	dns_free(mbx_p);
	mbx_p = NULL;
    }

    *new_mbx = (sys_mailbox)mbx_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <sys_openmbx(%s,%d,,%d,,) return(%s)",
        name_p,
	max_size,
	blkmode,
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * sys_closembx
 *
 * This routine closes down a mailbox by deassigning the channel
 * associated with the mailbox.
 *
 * Arguments:
 *   mailbox (in) - pointer to mailbox handle (created by sys_openmbx)
 *                  This routine will set it to null if successful.
 *
 * Returns:
 *   CDS_SUCCESS - Closed.
 *   CDS_NAMESERVERBUG - Bad (null) handle passed in.
 */
int 
sys_closembx (sys_mailbox *mailbox)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    private_mailbox *const mbx_p = (private_mailbox *)(*mailbox);
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >sys_closembx(%p)", mbx_p));

    if (mbx_p) {
	register int cleanup = TRUE;

	/*
	 * If a listener is attached, check if it is active.
	 * If so, cancel it and let it clean up.
	 * Otherwise clean up ourselves.
	 */

	MBX_LOCK(mbx_p) {
	    if (mbx_p->listener_attach) {
		if (mbx_p->listener_active) 
		{
		    dthread_cancel(mbx_p->listener_th);
		    cleanup = FALSE;
		}
		dthread_detach(&mbx_p->listener_th);
		mbx_p->listener_attach = FALSE;
	    }
	} MBX_END_LOCK(mbx_p);

	if (cleanup) 
	{
	    /*
	     * We have exclusive access to the control block - blow it away 
	     */
	    if (0 <= mbx_p->data_fd) {
		dthread_close(mbx_p->data_fd);
	    }
	    dthread_delete_mutex(&mbx_p->run_mutex);
	    dns_free((char *)mbx_p);
	}
	*mailbox = NULL;
	status = CDS_SUCCESS;
    } 
    else 
    {
        status = CDS_INVALIDARGUMENT;
    }

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug2,
	" <sys_closembx(%p) return(%s)", mbx_p,
	deb_ascii_dns_status(statusBuf,status)));
}

/*
 * sys_mbx_write
 *
 * This routine writes a message to a particular mailbox.  It does so
 * in a rather cavalier manner - it puts the message in the mailbox, but
 * does it such a way that it either completes immediately or fails
 * immediately.
 *
 * The write is done in a process-synchronous fashion (all threads are
 * blocked until the write completes).  A better model for this routine
 * would be to make it thread-synchronous, but then we have to worry about
 * buffer management and copying.  For now, this will do.  It also matches
 * the DNS V1 behavior, which may (or may not) be critical.
 *
 * Arguments:
 *     mailbox    (in) - Mailbox to write to.
 *     buffer_p   (in) - Pointer to buffer to write
 *     buffer_len (in) - Number of bytes to write to the mailbox.
 *
 * Returns:
 *
 *     CDS_SUCCESS       - Write completed successfully.
 *     CDS_NAMESERVERBUG - Mailbox specified was null.
 */
int 
sys_mbx_write (sys_mailbox  mailbox,
               byte_t       *buffer_p,
               int          buffer_len)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    private_mailbox *const mbx_p = (private_mailbox *)mailbox;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >sys_mbx_write(%p,,%d)", mbx_p,
        buffer_len));

    if (mbx_p) {
	if ((0 <= mbx_p->data_fd)
	    /*
	     * CANCEL POINT - no affected resources 
	     */
	    || ((status = mbx_connect(mbx_p)) == CDS_SUCCESS))
	{
	    struct sigaction oldsig, newsig;
	    int sig_status;

	    /*
	     * Make sure sigpipe does not stop us.
	     * No cleanup routine needed, since if the thread is
	     * cancelled, the signal action doesn't matter anyway.
	     */
	    newsig.sa_handler = SIG_IGN;
	    sigemptyset(&newsig.sa_mask);
	    newsig.sa_flags = 0;
	    sig_status = sigaction(SIGPIPE, &newsig, &oldsig);
	    dce_assert(cds__svc_handle, 0 <= sig_status);

	    if (!mbx_p->prepend)
	    {
		/*
		 * CANCEL POINT - no affected resources 
		 */
		status = mbx_write(mbx_p, buffer_p, buffer_len);
	    }
		/*
		 * CANCEL POINT 
		 */
	    else if ((status = mbx_write(mbx_p, (byte_t *)&buffer_len, 
					 sizeof(buffer_len))) == CDS_SUCCESS)
	    {
		/*
		 * CANCEL POINT - no affected resources 
		 */
		status = mbx_write(mbx_p, buffer_p, buffer_len);
	    }

	    sig_status = sigaction(SIGPIPE, &oldsig, NULL);
	    dce_assert(cds__svc_handle, 0 <= sig_status);
	}
    } 
    else 
    {
        status = CDS_INVALIDARGUMENT;
    }

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug2,
	" <sys_mbx_write(%p) return(%s)", mbx_p,
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * sys_mbx_read
 *
 * This routine does a thread-synchronous read of a mailbox.
 *
 * Arguments:
 *     mailbox    (in)    - Mailbox to write to.
 *     buffer_p   (out)   - Pointer to buffer to receive data from read.
 *     bytes_read_p (in-out) - Size of the message read.
 *
 *
 * Returns:
 *
 *     CDS_SUCCESS       - Read completed successfully.
 *     CDS_ERROR         - Read failed.
 */
int 
sys_mbx_read (sys_mailbox  mailbox,
              byte_t       *buffer_p,
              int          *bytes_read_p)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    private_mailbox *const mbx_p = (private_mailbox *)mailbox;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >sys_mbx_read(%p,,%d)", mbx_p, *bytes_read_p));

    if (mbx_p) {
	if (mbx_p->listener_active)
	{
	    status = CDS_NAMESERVERBUG;
	}
	else if ((0 <= mbx_p->data_fd)
	    /*
	     * CANCEL POINT - no affected resources 
	     */
	    || ((status = mbx_connect(mbx_p)) == CDS_SUCCESS))
	{
	    struct sigaction oldsig, newsig;
	    unsigned32 datalen;
	    int sig_status;

	    /*
	     * Make sure sigpipe does not stop us.
	     * No cleanup routine needed, since if the thread is
	     * cancelled, the signal action doesn't matter anyway.
	     */
	    newsig.sa_handler = SIG_IGN;
	    sigemptyset(&newsig.sa_mask);
	    newsig.sa_flags = 0;
	    sig_status = sigaction(SIGPIPE, &newsig, &oldsig);
	    dce_assert(cds__svc_handle, 0 <= sig_status);


	    /*
	     * CANCEL POINT - no affected resources 
	     */
	    if ((status = mbx_read(mbx_p, (byte_t *)&datalen, sizeof(datalen)))
		== CDS_SUCCESS)
	    {
		if (*bytes_read_p < datalen)
		{
		    status = CDS_NOROOM;
		}
		/*
		 * CANCEL POINT - no affected resources 
		 */
		else if ((status = mbx_read(mbx_p, buffer_p, datalen)) == CDS_SUCCESS)
		{
		    *bytes_read_p = datalen;
		}
	    }

	    sig_status = sigaction(SIGPIPE, &oldsig, NULL);
	    dce_assert(cds__svc_handle, 0 <= sig_status);
	}
    } 
    else 
    {
        status = CDS_INVALIDARGUMENT;
    }

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_library,
	svc_c_debug2,
	" <sys_mbx_read(%p,,%d) return(%s)", mbx_p, *bytes_read_p,
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * mbx_listener
 *
 * Opens a UNIX-domain socket, accepts connections, and passes
 * the new file descriptor to a newly-created client thread.
 *
 */
static dthread_address_t 
mbx_listener (private_mailbox *mbx_p)
{
    struct sigaction newsig;
    int lfd, dfd;
    int sig_status;

    /*
     * Make sure sigpipe does not stop us.
     */
    newsig.sa_handler = SIG_IGN;
    sigemptyset(&newsig.sa_mask);
    newsig.sa_flags = 0;
    sig_status = sigaction(SIGPIPE, &newsig, NULL);
    dce_assert(cds__svc_handle, 0 <= sig_status);

    /*
     * Make sure old name is gone 
     */
    unlink(mbx_p->addr.sun_path);

    dthread_cleanup_push(mbx_listener_mbx_clean, mbx_p);

    /*
     * Create the socket 
     */
    if (0 <= (lfd = dthread_socket(AF_UNIX, SOCK_STREAM, PF_UNSPEC))) {
	dthread_cleanup_push(mbx_cleanup_fd, &lfd);

	if (0 <= dthread_bind(lfd, 
			      (struct sockaddr *)&mbx_p->addr, 
			      sizeof (mbx_p->addr))) {
	    if (0 <= dthread_listen(lfd, 1)) {
		while (TRUE) {
		    int addrlen = sizeof(mbx_p->addr);

		    /*
		     * CANCEL POINT 
		     */
		    if ((dfd = dthread_accept(lfd,
				(struct sockaddr *)&mbx_p->addr, &addrlen)) < 0)
			break;

		    MBX_LOCK(mbx_p) {
			mbx_p->data_fd = dfd;
		    } MBX_END_LOCK(mbx_p);

		    while (TRUE) {
			int datalen;

			/*
			 * CANCEL POINT 
			 */
			if (mbx_read(mbx_p, (byte_t *)&datalen, sizeof(datalen))
			    != CDS_SUCCESS) break;
			if (mbx_p->max_size < datalen) 
			{
			    break;
			}
			
			/*
			 * CANCEL POINT 
			 */
			if (mbx_read(mbx_p, mbx_p->buffer_p, datalen)
			    != CDS_SUCCESS) 
			{
			    break;
			}
			
			/*
			 * Call user function 
			 */
			(*mbx_p->rcv)(mbx_p, (diag_request_t *)mbx_p->buffer_p);
		    }

		    MBX_LOCK(mbx_p) 
		    {
			dthread_close(mbx_p->data_fd);
			mbx_p->data_fd = -1;
		    } MBX_END_LOCK(mbx_p);
		}
	    }
	}
	dthread_cleanup_pop(1);
    }
    dthread_cleanup_pop(1);

    return(NULL);
}

static void 
mbx_listener_mbx_clean (
    private_mailbox *mbx_p)
{
    register int cleanup = TRUE;

    unlink(mbx_p->addr.sun_path);

    MBX_LOCK(mbx_p) {
	if (mbx_p->listener_attach)
	    cleanup = FALSE;

	mbx_p->listener_active = FALSE;
	if (0 <= mbx_p->data_fd) {
	    dthread_close(mbx_p->data_fd);
	    mbx_p->data_fd = -1;
	}
    } MBX_END_LOCK(mbx_p);

    if (cleanup) {
	dthread_delete_mutex(&mbx_p->run_mutex);
	dns_free((char *)mbx_p);
    }
}

static int 
mbx_connect (private_mailbox *mbx_p)
{
    int dfd;
    dns_status_t status;

    if (0 <= (dfd = dthread_socket(AF_UNIX, SOCK_STREAM, PF_UNSPEC))) {
	dthread_cleanup_push(mbx_cleanup_fd, &dfd);

	/*
	 * CANCEL POINT 
	 */
	if (0 <= dthread_connect(dfd,
		(struct sockaddr *)&mbx_p->addr, sizeof (mbx_p->addr))) {
	    MBX_LOCK(mbx_p) {
		mbx_p->data_fd = dfd;
	    } MBX_END_LOCK(mbx_p);
	    status = CDS_SUCCESS;
	} else {
	    status = CDS_UNIXERRORS + errno;
	    dthread_close(dfd);
	    dfd = -1;
	}

	dthread_cleanup_pop(0);
    } else
	status = CDS_UNIXERRORS + errno;

    return(status);
}

static int 
mbx_read (private_mailbox  *mbx_p,
          byte_t           *buf_p,
          int              len)
{
    dns_status_t status;

    while (TRUE)
    {
	int fd = mbx_p->data_fd;
	int u_status;

	if (fd < 0)
	{
	    status = CDS_NOCOMMUNICATION;
	}
	    /*
	     * CANCEL POINT 
	     */
	else if (0 < (u_status = dthread_read(fd, (char *)buf_p, len))) 
	{
	    buf_p = SKIP_bytes(buf_p, u_status);
	    if (0 < (len -= u_status))
	    {
		continue;
	    }
	    status = CDS_SUCCESS;
	} 
	else if (u_status == 0)
	{
	    status = CDS_ERROR;
	}
	else
	{
	    status = CDS_UNIXERRORS + errno;
	}
	break;
    }

    return(status);
}

static int 
mbx_write (private_mailbox  *mbx_p,
           byte_t           *buf_p,
           int              len)
{
    dns_status_t status;

    while (TRUE)
    {
	int fd = mbx_p->data_fd;
	int u_status;

	if (fd < 0)
	{
	    status = CDS_NOCOMMUNICATION;
	}
	    /*
	     * CANCEL POINT 
	     */
	else if (0 < (u_status = dthread_write(fd, (char *)buf_p, len))) 
	{
	    buf_p = SKIP_bytes(buf_p, u_status);
	    if (0 < (len -= u_status))
	    {
		continue;
	    }
	    status = CDS_SUCCESS;
	} 
	else if (u_status == 0)
	{
	    status = CDS_ERROR;
	}
	else
	{
	    status = CDS_UNIXERRORS + errno;
	}
	break;
    }

    return(status);
}

static void 
mbx_cleanup_fd (const int *fd)
{
    dthread_close(*fd);
}
