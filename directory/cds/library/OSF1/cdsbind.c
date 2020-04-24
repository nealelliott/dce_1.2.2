/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cdsbind.c,v $
 * Revision 1.1.4.2  1996/02/18  19:32:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:55  marty]
 *
 * 	Embedding copyright notices
 * 	[1992/12/29  23:07:58  zeliff]
 *
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:15:16  weisman]
 *
 * Revision 1.1.4.1  1995/12/08  15:20:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:25  root]
 * 
 * Revision 1.1.2.3  1994/06/30  19:16:09  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:57:48  mccann]
 * 
 * Revision 1.1.2.2  1994/06/09  18:40:11  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:51  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/29  15:51:21  peckham
 * 	Split off dnsbind.c into OS-dependent cdsbind.c
 * 	for SVR4 named pipes.
 * 	Use dcelocal_path in file names.
 * 	[1994/04/29  13:58:46  peckham]
 * 
 * Revision 1.1.9.1  1994/03/12  22:06:00  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:25:58  peckham]
 * 
 * Revision 1.1.7.2  1993/07/30  14:57:06  blurie
 * 	#8286 ( acl_edit hangs because the cdsadv never responds).
 * 
 * 		If SNI_SVR4_NAMED_PIPES is defined, use named_pipes to
 * 		communicate and create a unique connection between CDS client
 * 		and advertiser/clerk processes.
 * 	[1993/07/30  13:40:11  blurie]
 * 
 * Revision 1.1.2.2  1992/06/01  15:24:19  mfox
 * 	Replace reference of BSD strings.h with ANSI string.h
 * 	[1992/04/29  15:49:28  mfox]
 * 
 * Revision 1.1  1992/01/19  15:18:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE cdsbind.c
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
  
#include <uc_clerk.h>
#include <pthread.h>
#include <dce/dce.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <dce/utc.h>
#include <errno.h>
#ifndef SOCK_STREAM
# include <sys/types.h>
# include <sys/socket.h>
#endif
#include <sys/un.h>
#include <signal.h>
#include <dce/cdsclerk.h>
#include <adver_ipc.h>
#include <dns_strings.h>

int cds_clerk_socket = -1;
extern int cds_euid;
extern int cds_egid;


/* ----------------------------------------------------------------------
 *  Create a socket with the clerk child
 * ----------------------------------------------------------------------
 *	Binds to a child process.  Calls the advertiser and asks for a
 *	child process socket address; connects to that address and
 *	returns the socket id of the connection.
 */
int 
cds_bind (void)
{
    BChldReqMsg_t	BindRequest;
    int			amount;
    struct sockaddr_un	sockaddr;
    BChldResMsg_t	BindResponse;
    struct sockaddr_un *sa = (struct sockaddr_un *)&BindResponse.saddr;
    register int	sd;

    cds_clerk_socket = -1; /* assume failure */

    /* default if no advertiser */
    sa->sun_family = AF_UNIX;
    sprintf(sa->sun_path, CDS_DEF_CLERK_SOCK, dcelocal_path);
    
    /* build the message  */
    BindRequest.hdr.version = CurrentIPCversion;
    BindRequest.hdr.msgType = BChldReq_t;
    BindRequest.hdr.infoLen = sizeof(cds_euid)+sizeof(cds_egid);
    COPY_bytes(&cds_euid, BindRequest.principal, sizeof(cds_euid));
    COPY_bytes(&cds_egid, BindRequest.principal+sizeof(cds_euid),
	sizeof(cds_egid));

    /* actual size of message */
#define REQLEN (sizeof(BindRequest.hdr)+sizeof(cds_euid)+sizeof(cds_egid))

#ifdef ADVERTISER    
    BindChildDssd(&BindRequest, &BindResponse);

    /* check for ok message */
    if (BindResponse.hdr.msgType != BChldRes_t)
	return(CDS_SOCKET);
#else
    {
	/* first we connect to the advertiser */
	if (0 <= (sd = socket(AF_UNIX, SOCK_STREAM, 0))) {

	    ZERO_bytes(&sockaddr, sizeof(sockaddr));
	    sockaddr.sun_family = AF_UNIX;
	    sprintf(sockaddr.sun_path, CDS_LIB_SOCK, dcelocal_path);

	    if (0 <= connect(sd, (struct sockaddr *)&sockaddr, sizeof(sockaddr))) 
	    {
		struct sigaction sigactn, prev_sig_details;


		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_library,
                    svc_c_debug5,
                    "  cds_bind connected adver socket, fd %d",
                    sd));

		/*
		 *  Set up sigaction structure
		 */
		sigactn.sa_handler = SIG_IGN;
		sigemptyset(&sigactn.sa_mask);
		sigactn.sa_flags   = 0;

		/*	
		 * send message : Changed for POSIX on Sept. 28, 1993
		 */
		sigaction(SIGPIPE, &sigactn, &prev_sig_details);
		amount = send(sd, (char *)&BindRequest, REQLEN, 0);
		sigaction(SIGPIPE, &prev_sig_details, NULL);

		if (amount != REQLEN) 
		{
		    close(sd);
		    DCE_SVC_DEBUG((
                        cds__svc_handle,
                        cds_svc_library,
                        svc_c_debug5,
                        "  cds_bind expected %d bytes sent to adver, got %d",
                        REQLEN,
                        amount));

	    	     return(CDS_SOCKET);
		}

		/*
		 * get and parse answer 
		 */
		amount = recv(sd, (char *)&BindResponse, sizeof(BindResponse), 0);

		/*
		 * check for ok message 
		 */
		if ((amount != sizeof(BindResponse)) ||
		    (BindResponse.hdr.msgType != BChldRes_t)) 
		{
		    
		    close(sd);
		    DCE_SVC_DEBUG((
                        cds__svc_handle,
                        cds_svc_library,
                        svc_c_debug5,
                        "  cds_bind expected %d bytes rcvd from adver, got %d",
                        REQLEN,
                        amount));

		    return(CDS_SOCKET);
		}
		else 
		{
		    DCE_SVC_DEBUG((
                        cds__svc_handle,
                        cds_svc_library,
                        svc_c_debug5,
                        "  cds_bind received answer from adver, len %d",
                        amount));
		}
	    } 
	    else 
	    {
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_library,
                    svc_c_debug5,
                    "  cds_bind adver connect failed, status = %d, errno = %d",
                    sd,
                    errno));
	    }
	    close(sd);
	}
    }
#endif	/* ADVERTISER */
    /*
     * now, try to connect with the child process.
     * this may take us a few tries , as the process
     * may have to be created
     */
    if (0 <= (sd = socket(AF_UNIX, SOCK_STREAM, 0))) {
	if (0 <= connect(sd, (struct sockaddr *)sa, sizeof(*sa))) {
	    cds_clerk_socket = sd;
          DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug5,
              "  cds_bind connected to child socket %d %s",
              sd,
              &BindResponse.saddr.sun_path[0]));
	    return(CDS_SUCCESS);
	}
	else
          DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug5,
              "  cds_bind child connect(%s) returned %d, errno = %d",
              &BindResponse.saddr.sun_path[0],
              sd,
              errno));
	close(sd);
    }
    else
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_library,
          svc_c_debug5,
          " cds_bind child socket(%s) returned %d, errno = %d",
          &BindResponse.saddr.sun_path[0],
          sd,
          errno));

    return(CDS_SOCKET);
}
