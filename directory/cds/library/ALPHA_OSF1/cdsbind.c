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
 * Revision 1.1.4.2  1996/02/18  19:31:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:45  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:19:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:15  root]
 * 
 * Revision 1.1.2.2  1994/08/10  19:21:37  annie
 * 	expand copyright with OSF copyright text
 * 	[1994/08/10  17:15:55  annie]
 * 
 * 	expand copyright with OSF copyright text
 * 
 * 	expand copyright with OSF copyright text
 * 
 * Revision 1.1.2.1  1994/06/30  19:15:53  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:08:04  mccann]
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
