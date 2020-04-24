/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnswrite.c,v $
 * Revision 1.1.12.1  1996/05/10  13:07:49  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/04/18  19:24 UTC  bissen
 * 	Single-threaded changes
 *
 * 	HP revision /main/HPDCE02/2  1995/02/06  13:45 UTC  pare
 * 	Merge chages for defect CHFts14276
 *
 * 	HP revision /main/pare_mothra/1  1995/02/02  18:03 UTC  pare
 * 	Check errno after write() for single threaded case.
 *
 * 	HP revision /main/HPDCE02/1  1995/02/06  13:42 UTC  pare
 * 	Check errno after write() -- defect CHFts14276
 * 	[1996/05/09  20:43:14  arvind]
 *
 * Revision 1.1.7.5  1994/08/03  19:03:09  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:10:03  mccann]
 * 
 * Revision 1.1.7.4  1994/06/30  19:17:44  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:03:30  mccann]
 * 
 * Revision 1.1.7.3  1994/06/09  18:42:23  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:34  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/29  15:55:17  peckham
 * 	Change dns* to cds*
 * 	[1994/04/29  14:30:19  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:08:11  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:33:38  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  14:16:37  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:15:58  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  18:52:27  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:15:53  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  22:23:25  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:32:32  weisman]
 * 
 * Revision 1.1  1992/01/19  15:18:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* MODULE dnswrite.c
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
#include <dce/dce.h>
#include <dce/cdsclerk.h>
#include <dcecdssvc.h>
#include <errno.h>
#include <fcntl.h>

#include <signal.h>  
#include <uc_clerk.h>     
#include <uc_parse.h>
#include <sys/types.h>
#include <unistd.h>


static int
dobind (
    byte_t		*msg_p);

extern int cds_clerk_socket;
extern int (*dns_intercept)(cdsFlagStat_t  *, 
			    void(*cdsInit)(void));
int cds_euid = -1;
int cds_egid = -1;
static int cds_pid = -1;


/* ----------------------------------------------------------------------
 * Spawn off thread to initiate bind
 * ----------------------------------------------------------------------
 */
void 
cdsInit (void)
{
    /*
     * Fill this in with code - we must make sure that cds_clerk_socket
     * is not fought over by multiple threads.
     */
}

/* ----------------------------------------------------------------------
 *  Send the message to the child
 * ----------------------------------------------------------------------
 */
int
cds_write (
    cdsFlagStat_t *Flags_p,		/*IN/OUT*/
    byte_t        *msg_p,		/*IN pointer to message */
    int           msglen)               /*IN length of message */
{
    register int status;
    struct sigaction sigactn, prev_sig_details;


    /*
     * Acquire the clerk socket
     */
    if ((status = dobind(msg_p)) != CDS_SUCCESS)
      return dns_status_clerk(Flags_p, status);

    /*
     * ignore SIGPIPE  -  Set up sigaction structure
     */
    sigactn.sa_handler = SIG_IGN;
    sigemptyset(&sigactn.sa_mask);
    sigactn.sa_flags   = 0;

    sigaction(SIGPIPE, &sigactn, &prev_sig_details);

    while ((status = write(cds_clerk_socket, (char *)msg_p, msglen))
								!= msglen) 
    {
	if (status < 0 && errno == EINTR)
	{
	    continue;
        }
	else
	{
	    if (status <= 0) 
	    {
	        cds_unbind();
	    
	    /*
	     * Restore previous signal setings...
	     */
	        sigaction(SIGPIPE, &prev_sig_details, NULL);
	        return dns_status_unix(Flags_p, status);
	    }
	    else 
	    {
	        msg_p += status;
	        msglen -= status;
	    } 
        }
    } /* end while */

    /*
     * Restore previous signal setings...
     */
    sigaction(SIGPIPE, &prev_sig_details, NULL);

    return(CDS_SUCCESS);
}
/*
 * Get a socket from the advertiser if it doesn't already exist.
 * If the effective user id has changed, close current socket and get
 * another.
 */
static int
dobind (byte_t *msg_p)
{
  register ucp_rqst_header *hdr_p;
  register int temp;

  /* 
   * Get and save user effective id.  If it changes reconnect to new
   * clerk.
   */
  if ((temp = geteuid()) != cds_euid) {
    cds_euid = temp;
    cds_egid = getegid();

    /* If currently have a connection, break it and start again */
    cds_unbind();
  }

  if (hdr_p = (ucp_rqst_header *)msg_p) {
    UCP_INSlongword(hdr_p->user_uid, cds_euid);
    UCP_INSlongword(hdr_p->user_gid, cds_egid);
    }

  if ((temp = getpid()) != cds_pid) {
	cds_pid = temp;
	cds_unbind();
	}
  /*
   * Create child clerk process if not already running.
   */
  if (cds_clerk_socket < 0)
    {
    if ((temp = cds_bind()) != CDS_SUCCESS)
      return(temp);

    /* Set close-on-exec flag */
    fcntl(cds_clerk_socket, F_SETFD, 1);
    }
  return(CDS_SUCCESS);
}
