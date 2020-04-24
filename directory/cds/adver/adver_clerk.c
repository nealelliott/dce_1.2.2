/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: adver_clerk.c,v $
 * Revision 1.1.6.1  1996/05/10  13:07:13  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/04/18  19:37 UTC  bissen
 * 	Merge fix for OT 13410
 *
 * 	Fix for OT 13410. Remove the undef of the execve wrapper when fork/exec'ing
 * 	the cdsclerk for HP. If the CMA heartbeat is turned on in the child after
 * 	a fork, the cma_execve wrapper needs to be invoked to turn it off before
 * 	the exec.
 * 	[1996/03/27  19:11 UTC  jss  /main/jss_dce12/1]
 * 	Revision 1.1.2.6  1994/08/03  18:58:51  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:01:43  mccann]
 *
 * Revision 1.1.2.5  1994/07/08  15:33:09  peckham
 * 	Eliminate extra random command line argument.
 * 	[1994/07/05  20:54:42  peckham]
 * 
 * Revision 1.1.2.4  1994/06/30  19:15:04  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:34:56  mccann]
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.2.3  1994/06/09  16:06:47  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:18  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/25  22:22:16  peckham
 * 	Take operations with side-effects out of dce_assert().
 * 	Make sure threads are detached when appropriate.
 * 	[1994/05/25  22:17:39  peckham]
 * 
 * Revision 1.1.2.1  1994/05/12  21:10:04  peckham
 * 	Clerk spawning from adver_clerk_event.c
 * 	Avoid SIGPIPE crashing advertiser.
 * 	[1994/05/12  19:11:47  peckham]
 * 
 * $EndLog$
 */
/*
 * 	Module: adver_clerk.c
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
 */

#include <adver.h>
#include <adver_ipc.h>
#include <cache.h> 
#include <dce/assert.h>
#include <uc_child.h>
#include <dns_cfg.h>
#include <dns_strings.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * cds shared memory id 
 */
extern int sharedMemId;

SimpleName_t *
dns_user_name (int 	,
	       int 	);

/*
 * list of child processes (clerks) - unix only 
 */
typedef struct childlist {
    int			pid;			/* child pid */
    int			slen;			/* socket length */
    struct sockaddr_un	saddr;			/* socket */
    int			princLen;		/* principal length */
    char		principal[DNS_MAX_PRINC]; /* principal */
    struct childlist	*next_p;		/* next child in list */
} childlist_t;

/*
 * Main control structures
 */
struct listen {
    dthread_mutex_t	mutex;
    dthread_t		listener;
    void		*cache_p;
    struct childlist	*childList_p;
    int			s;
    unsigned int	lastnum;
};

struct client {
    struct listen	*listen_p;
    struct sockaddr_un	addr;		/* Unix IPC domain sockets */
    int			addrlen;
    int			s;
};

static dthread_attr_t adver_listener_attr = {"adver_clerk_listener"};
static dthread_attr_t adver_client_attr = {"adver_clerk_client"};

/*
 * internal prototypes 
 */

static dthread_address_t
adver_clerk_listener (dthread_address_t);

static void
listener_cleanup (void *);

static dthread_address_t
adver_clerk_client (dthread_address_t);

static void
client_cleanup(void *arg);

static dns_status_t
BindChildDssd (struct listen  *,
	       BChldReqMsg_t  *,
               BChldResMsg_t  *);

static void
start_child (
    childlist_t	        *,
    char	       **,
    int		         );

/*
 * adver_clerk_new - start spawning clerks
 */
void *
adver_clerk_new (void *cache_p)
{
    struct listen *listen_p = (struct listen *)malloc(sizeof(struct listen));

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >adver_clerk_new(%p)", cache_p));

    if (listen_p)
    {
	int thstatus;

	ZERO_bytes(listen_p, sizeof(*listen_p));

	if ((thstatus = dthread_create_mutex(&listen_p->mutex)) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
	}

        listen_p->cache_p = cache_p;

        CDS_LOCK(listen_p->mutex)

	if ((thstatus = dthread_create(&listen_p->listener,
				&adver_listener_attr,
				(dthread_start_routine)adver_clerk_listener,
				(dthread_address_t)listen_p)) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
	    }

        CDS_END_LOCK(listen_p->mutex)
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " <adver_clerk_new(%p) return(%p)", cache_p, listen_p));

    return (listen_p);
}

/*
 * adver_clerk_delete - shut down clerks
 */
void
adver_clerk_delete (void *arg)
{
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >adver_clerk_delete(%p)", arg));

    if (arg)
    {
	struct listen *const listen_p = arg;
	childlist_t *child_p;

	dthread_address_t join_status;
	int thstatus;

	(void) dthread_cancel(listen_p->listener);
	(void) dthread_join(listen_p->listener, &join_status);
	(void) dthread_detach(&listen_p->listener);

	/*
	 * Unless someone skunks the code,
	 * the signal handler will not be active,
	 * so there will not be any contention
	 * for this mutex.
	 */
	CDS_LOCK(listen_p->mutex)

	/*
	 * One pass through to get the clerks started
	 */
	for (child_p = listen_p->childList_p; child_p;
	     child_p = child_p->next_p)
	    (void)kill(child_p->pid, SIGTERM);

	/*
	 * Now wait for them to finish
	 */
	while ((child_p = listen_p->childList_p) != NULL) {
	    int status;

	    listen_p->childList_p = child_p->next_p;

	    (void)kill(child_p->pid, SIGTERM);

	    /*
	     * Make sure that child has died before deleting
	     */
	    do {
		if (waitpid(child_p->pid, &status, 0) < 0)
		    break;

		/*
		 * I hope this is the right thing to wait for
		 */
	    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

	    (void)unlink(child_p->saddr.sun_path);
	    dns_free((char *)child_p);
	}

	CDS_END_LOCK(listen_p->mutex)

	if ((thstatus = dthread_delete_mutex(&listen_p->mutex)) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
	    }

	free(arg);
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " <adver_clerk_delete(%p)", arg));
}

static dthread_address_t
adver_clerk_listener (dthread_address_t arg)
{
    struct sockaddr_un sockaddr_un;	/* Unix IPC domain sockets */
    struct listen *const listen_p = arg;
    int s;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug2,
		   " >adver_clerk_listener(%p)", arg));

    ZERO_bytes(&sockaddr_un, sizeof(sockaddr_un));
    sockaddr_un.sun_family = AF_UNIX;
    (void) sprintf(sockaddr_un.sun_path, CDS_LIB_SOCK, dcelocal_path);

    if (0 <= (s = adver_open_socket(&sockaddr_un, 10)))
    {
	DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		       "  adver_clerk_listener(%p) listen socket(%d)", arg, s));

	listen_p->s = s;
	dthread_cleanup_push(listener_cleanup, listen_p);

	/* close this socket on exec() */
	(void) fcntl(s, F_SETFD, 1);

	(void) chmod(sockaddr_un.sun_path, 0666);

	for (;;)
	{
	    dthread_t client;
	    struct client *client_p = (struct client *)malloc(sizeof(struct client));
	    int thstatus;

	    if (!client_p)
		if (0 <= dthread_delay(1.0))
		    continue;
		else
		    break;

	    client_p->listen_p = listen_p;
	    client_p->addrlen = sizeof(client_p->addr);
	    client_p->s = adver_accept_socket(s,
					      &client_p->addr,
					      &client_p->addrlen);

	    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
			  "  adver_clerk_listener(%p) client socket(%d)",
			  arg, client_p->s));

	    if (client_p->s < 0) {
		free((char *) client_p);
		break;
	    }

	    if ((thstatus = dthread_create(&client,
				    &adver_client_attr,
				    (dthread_start_routine) adver_clerk_client,
				    (dthread_address_t)client_p)) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
	    }

	    (void) dthread_detach(&client);
	}
	dthread_cleanup_pop(1);
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug2,
		   " <adver_clerk_listener(%p) return(%p)", arg, NULL));

    return (NULL);
}

static void
listener_cleanup (void *arg)
{
    struct listen *const listen_p = arg;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " >listener_cleanup(%p)", arg));

    adver_clerk_signal(arg, SIGHUP);
    dthread_close(listen_p->s);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
		   " <listener_cleanup(%p)", arg));
}

static dthread_address_t
adver_clerk_client (dthread_address_t arg)
{
    struct sigaction newsig;
    struct client *const client_p = arg;
    const int s = client_p->s;
    int sig_status;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
		   " >adver_clerk_client(%p)", arg));

    /*
     * Make sure sigpipe does not stop us.
     */
    newsig.sa_handler = SIG_IGN;
    sigemptyset(&newsig.sa_mask);
    newsig.sa_flags = 0;
    sig_status = sigaction(SIGPIPE, &newsig, NULL);
    dce_assert(cds__svc_handle, (0 <= sig_status));

    /* set up rpc context */
    rpc_ss_enable_allocate();

    dthread_cleanup_push(client_cleanup, client_p);

    for (;;)
    {
	union {
	    char            buf[sizeof(BChldReqMsg_t)];
	    BChldMsgHdr_t   bindHdr;
	    BChldReqMsg_t   bindMsg;
	} in;
	union {
	    char            buf[sizeof(BChldResMsg_t)];
	    BChldResMsg_t   bindMsg;
	} out;
	dns_status_t    status;
	int             cc;

	cc = adver_read_socket(s, in.buf, sizeof(in.bindHdr));
	if (cc != sizeof(in.bindHdr))
	    goto cleanup_client;

	cc = adver_read_socket(s, &in.buf[sizeof(in.bindHdr)],
				  in.bindHdr.infoLen);
	if (cc != in.bindHdr.infoLen)
	    goto cleanup_client;

	status = BindChildDssd(client_p->listen_p, &in.bindMsg, &out.bindMsg);

	if (status != DNS_SUCCESS)
	{
	    out.bindMsg.hdr.msgType = GenRes_t;
	    out.bindMsg.hdr.version = CurrentIPCversion;
	    out.bindMsg.hdr.infoLen = status;
	}
	cc = adver_write_socket(s, out.buf, sizeof(out.bindMsg));
	if (cc != sizeof(out.bindMsg))
	    goto cleanup_client;
    }

cleanup_client:
    dthread_cleanup_pop(1);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug3,
		   " <adver_clerk_client(%p) return(%p)", arg, NULL));

    return (NULL);
}

static void
client_cleanup (void *arg)
{
    struct client *const client_p = arg;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " >client_cleanup(%p)", arg));

    dthread_close(client_p->s);
    free(arg);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   " <client_cleanup(%p)", arg));
}

/*
 * BindChild()
 *
 * Description:
 *     Looks to see if there is a child for this principal;
 *     if there is, it returns the socket.
 *     If not, it spawns a child.
 *
 *     This is a unix only procedure.
 *
 * Inputs:
 *     eblkptr - an event block
 *
 * Outputs:
 *     none
 *
 * Side Effects:
 *     may create a child process
 */
static dns_status_t
BindChildDssd (struct listen *listen_p,
	       BChldReqMsg_t  *bindMsg,
               BChldResMsg_t  *bindRes)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    childlist_t    *child_p;
    sigset_t        sigs_to_block,
		    saved_sigmask; /* For blocking SIGCHLD */
    struct sigaction sigactn, prev_sig_details;
    dce_error_string_t  error_text;
    dns_status_t status;
    int num;
    int thstatus;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " >BindChildDssd(%p,,)", listen_p));

    bindRes->hdr.version = CurrentIPCversion;

    /*
     * ok, look to see if we have a child for this prinicipal 
     */
    CDS_LOCK(listen_p->mutex)

    for (child_p = listen_p->childList_p;  child_p; 
	 child_p = child_p->next_p)
    {

	if ((bindMsg->hdr.infoLen == child_p->princLen) &&
	    (EQ_bytes(bindMsg->principal, 
		    child_p->principal, child_p->princLen)))
	{

	    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		     "  BindChildDssd found child %d (%s) for uid %d, gid %d ",
		     child_p->pid, child_p->saddr.sun_path,
		     ((int*)child_p->principal)[0],
		     ((int*)child_p->principal)[1]));
	    break;
	}
	if (!child_p)
	    do
		num = ++listen_p->lastnum;
	    while (num == 0);
    }

    CDS_END_LOCK(listen_p->mutex)

    /*
     * nope, better make one 
     */
    if (!child_p) {
	char filename[_DNS_MAXPATHLEN+1];
	char *argv[32], argbuf[256], nambuf[128];
	register char *handle, *p = argbuf;
        int argv_indx = 0;
	char *childName_p, *userName_p;
	SimpleName_t *sn_p;
	int uid = ((int *)bindMsg->principal)[0];
	int gid = ((int *)bindMsg->principal)[1];
	int cfd;
        dce_svc_stringarray_t  logroutings;
        dce_svc_stringarray_t  debugroutings;
        error_status_t routingst;
        int inqst;
        int i;

	child_p = (childlist_t *)dns_malloc(sizeof(childlist_t));
	if (!child_p)
	{	
	    dce_svc_printf(CDS_S_ALLOC_A_MSG);
	  }

	/*
	 * set up default socket name 
	 */
	ZERO_bytes(&child_p->saddr, sizeof(child_p->saddr));
	child_p->saddr.sun_family = AF_UNIX;
	(void) sprintf(child_p->saddr.sun_path, CDS_TMP_SOCK_TMPL, dcelocal_path, num);
	child_p->slen = sizeof(child_p->saddr);

	child_p->princLen = bindMsg->hdr.infoLen;
	COPY_bytes(bindMsg->principal, child_p->principal, child_p->princLen);

	/*
	 * load up the argument vector 
	 */

	/*
	 * Consult configuration file 
	 */
	handle = dns_cfgopen((char *)0, (char *)0);

	(void)sprintf(filename, CDS_CLERK_PROG_FILE, dcelocal_path);
	strcpy(p, dns_cfgtostr(handle, "clerk.file", filename));

	dns_cfgclose(handle);
	argv[argv_indx++] = p;
	while (*p++) ;
	if ((sn_p = dns_user_name(uid, gid)) != NULL)
	{
	    (void)strcpy(p, (char *)sn_p->sn_name);
	}
	else
	{
	    (void)sprintf(p, "(%d,%d)", uid, gid);
	}

	userName_p = p;
	while (*p++) ;

        /*
        ** Enquire if we need to pass on event logging flags to the 
        ** child process and if so set up the -w flag.
        */
        dce_svc_routings(&logroutings, FALSE, &routingst);
        if (routingst != svc_s_ok)
        {
	  dce_svc_printf(CDS_S_ADVER_ROUTING_MSG, (long)routingst);
        }
        else 
        {
            /* 
            ** set up the -w switches to child, ensuring that we don't
            ** blow the argv array.
            */
            for (i = 0; 
                 (i < logroutings.tab_size) && 
                 (argv_indx < (sizeof argv / sizeof argv[0])); 
                 i++)
            {
                argv[argv_indx++] = "-w";
                argv[argv_indx++] = (char *)logroutings.table[i]; 
            }
        }
        /*
        ** Enquire if we need to pass on debugging flags to the 
        ** child process and if so set up the -d flag.
        */
        dce_svc_routings(&debugroutings, TRUE, &routingst);
        if (routingst != svc_s_ok)
        {
	    dce_svc_printf(CDS_S_ADVER_ROUTING_MSG, (long)routingst);
        }
        else
        {
            /* 
            ** set up the -d switches to child, ensuring that we don't
            ** blow the argv array.
            */
            for (i = 0; 
                 (i < debugroutings.tab_size) && 
                 (argv_indx < (sizeof argv / sizeof argv[0])); 
                 i++)
            {
                argv[argv_indx++] = "-d";
                argv[argv_indx++] = (char *)debugroutings.table[i]; 
            }
        }
	argv[argv_indx++] = NULL;

	if ((childName_p = strrchr(argv[0], '/')) != NULL)
	{
	    childName_p++;
	}
	else
	{
	    childName_p = argv[0];
	}

	/*
	 * Make sure the socket is created
	 * before the child is created -
	 * there are cases where the name was passed back
	 * to the library before it was created.
	 */
	if ((cfd = adver_open_socket(&child_p->saddr, 5)) < 0) {
	    status = DNS_UNIXERRORS + errno;
	    goto leave_BindChildDssd;
	}

	(void)chown(child_p->saddr.sun_path, uid, gid);
	(void)chmod(child_p->saddr.sun_path, 0400|0200);

	/*      For SVR4, it is not necessary to do this since cma_fork() and
	 *      cma_exec*() do the right things.
	 */
#if !defined(SNI_SVR4)
        /*
         *  Set up sigaction structure
         */
        sigactn.sa_handler = SIG_IGN;
        sigemptyset(&sigactn.sa_mask);
        sigactn.sa_flags   = 0;

	/*
	 * disable CMA scheduler *
	 */
	sigaction(SIGVTALRM, &sigactn, &prev_sig_details); 
#endif

	if ((child_p->pid = fork()) == -1) 
	{
	    const int save_errno = errno;

	    dce_svc_printf(CDS_S_ADVER_FORK_MSG, (long)save_errno);

	    (void)close(cfd);
	    (void)unlink(child_p->saddr.sun_path);
	    status = DNS_UNIXERRORS + save_errno;
	    goto leave_BindChildDssd;
	}

	/*
	 * Block SIGCHiLD until after all parent references of child_p.
	 * This is because adver_clerk_signal() can deallocate child_p.
	 */
	if (child_p->pid != 0)	
	{
	    /*
	     * Parent-only 
	     */
	    sigemptyset(&sigs_to_block);
	    sigaddset(&sigs_to_block, SIGCHLD);

	    /*
	     * System V SIGCHLD 
	     */
	    sigprocmask(SIG_BLOCK, &sigs_to_block, &saved_sigmask);
	}

	if (child_p->pid == 0)
	{
	    /*
	     * Child process 
	     */
	    start_child(child_p, argv, cfd);
	}

	/*
	 * Since start_child will exit, the remaining code here is
	 * parent-only.
	 */
        /*
         * Free up the storage returned from dce_svc_routings() for
         * both debug and logging settings. This is done in the
         * advertiser, not in the child as argv's should hang around for
         * the life of the mainline in child.
         */
        if (debugroutings.table)
           rpc_ss_free (debugroutings.table);
        if (logroutings.table)
           rpc_ss_free (logroutings.table);

	/*      For SVR4, it is not necessary to do this since cma_fork() and
	*       cma_exec*() do the right things.
	*/
#if !defined(SNI_SVR4)
	/*
	 * enable CMA scheduler  -> restore old signal settings.
	 */
        sigaction(SIGVTALRM, &prev_sig_details, NULL);
#endif
	(void)close(cfd);		/* The child is using this now */

	/*
	 * Change bind name to something useful 
	 */
	(void)strcpy(nambuf, child_p->saddr.sun_path);
	if ((p = strrchr(nambuf, '/')) != (char *)0)
	{
	    p++;
	}
	else
	{
	    p = nambuf;
	}
	
	(void)sprintf(p, CDS_SOCK_TMPL, childName_p, child_p->pid, userName_p);
	(void)rename(child_p->saddr.sun_path, nambuf);
	(void)strcpy(child_p->saddr.sun_path, nambuf);

      DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug6,
          "  BindChildDssd() exec(%s) for(%d,%d) name(%s)",
	   argv[0], uid, gid, child_p->saddr.sun_path));

      CDS_LOCK(listen_p->mutex)
	
      child_p->next_p = listen_p->childList_p;
      listen_p->childList_p = child_p;

      CDS_END_LOCK(listen_p->mutex)

	/*
	 * Unblock SIGCHiLD - this needs to be after the last reference
	 * of child_p.  This is because adver_clerk_signal() can deallocate
	 * child_p.
	 */
	sigprocmask(SIG_UNBLOCK, &sigs_to_block, 0);
	sigprocmask(SIG_SETMASK, &saved_sigmask, 0);
    } /* endif !child_p */

    /*
     * send the library a message, letting it know what address to try 
     */
    bindRes->hdr.msgType = BChldRes_t;
    bindRes->hdr.infoLen = child_p->slen;
    COPY_bytes(&child_p->saddr, &bindRes->saddr, bindRes->hdr.infoLen);
    status = DNS_SUCCESS;

leave_BindChildDssd:

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug4,
		   " <BindChildDssd(%p,,) return(%s)", listen_p,
		   deb_ascii_dns_status(statusBuf,status)));

    return (status);
}

/*
 * adver_clerk_signal()
 *
 * Description:
 *     Signal handler called from sigwait() thread.
 *
 * Inputs:
 *     sig - signal
 *
 * Outputs:
 *     none
 *
 */
void 
adver_clerk_signal (void *arg,
		    int sig)
{
    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
	       " >adver_clerk_signal(%p,%d)", arg, sig));

    if (arg)
    {
	struct listen *const listen_p = arg;
	int         deadChildPid = -1;
	childlist_t *deadChild;
	childlist_t *prevChild;
	int thstatus;

#if defined(__osf__) || defined(__OSF__)
# define _OSF_SOURCE
#endif

	while ((deadChildPid = waitpid(-1, NULL, WNOHANG)) &&
	       (deadChildPid != -1)) {

	  CDS_LOCK(listen_p->mutex)

	      deadChild = listen_p->childList_p;

	      while (deadChild) {
		if (deadChild->pid == deadChildPid) {
		  if (deadChild == listen_p->childList_p)
		      listen_p->childList_p = deadChild->next_p;
		  else
		      prevChild->next_p = deadChild->next_p;
		  break;
		}
		prevChild = deadChild;
		deadChild = deadChild->next_p;
	      } /* while deadChild */

	  CDS_END_LOCK(listen_p->mutex)

	  if (deadChild) {

		DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
		   "  adver_clerk_signal(%d) dead child %lx (%s) for uid %d, gid %d",
		   sig, (long)deadChild->pid, deadChild->saddr.sun_path,
		   ((int*)deadChild->principal)[0], 
                   ((int*)deadChild->principal)[1]));

		(void)unlink(deadChild->saddr.sun_path);
		dns_free((char *)deadChild);
	      } /* endif deadChild */
	} /* waitpid() */
    }

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_adver, svc_c_debug8,
	       " <adver_clerk_signal(%p,%d)", arg, sig));
}

/*
 * start_child
 *	Start the DNS Clerk child process. This is done here to
 *	get around CMA wrappers. The following defines insure that
 *	CMA will not be involved with any system calls between
 *	the fork() and the exec() calls.
 *
 * THIS ROUTINE MUST STAY AT THE END OF THIS MODULE!!!!!
 */
#if defined(dup2)
#undef dup2
#endif

#if defined(close)
#undef close
#endif

/* HP-UX supplies exec wrappers that disable the SIGVTALRM */
#if !defined(__hpux)
# if defined(execve)
#  undef execve
# endif
#endif

#if defined(exit)
#undef exit
#endif

static void 
start_child (childlist_t  *child_p,
             char         **argv,
             int          cfd)
{
    extern char **environ;
    register int  i;
    static int nfds;


    if (cfd != 5)
    {
	/*
	 * close fd#5; replace with opened and created socket 
	 */
	(void)dup2(cfd, 5);
    }

    /*
     * close sockets 
     */
    if (nfds == 0)
    {
	nfds = sysconf(_SC_OPEN_MAX);
    }
    
    for (i = 3; i < nfds; i++)
    {
	if (i != 5)
	{
	    (void)close(i); 
	}
    }

    (void)execve(argv[0], argv, environ);

    dce_svc_printf(CDS_S_ADVER_EXECVE_MSG, (long)errno);
}


