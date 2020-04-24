/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsdaemon.c,v $
 * Revision 1.1.10.2  1996/02/18  19:33:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:48  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:23:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:45:55  root]
 * 
 * Revision 1.1.7.6  1994/07/06  20:43:41  ohara
 * 	use string.h not strings.h
 * 	[1994/07/06  20:07:30  ohara]
 * 
 * Revision 1.1.7.5  1994/06/30  19:16:54  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:00:23  mccann]
 * 
 * Revision 1.1.7.4  1994/06/09  18:41:26  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:39  devsrc]
 * 
 * Revision 1.1.7.3  1994/04/29  15:53:11  peckham
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	[1994/04/29  14:17:46  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:06:32  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:28:05  peckham]
 * 
 * Revision 1.1.5.4  1992/12/30  13:55:40  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:10:07  zeliff]
 * 
 * Revision 1.1.5.3  1992/12/18  18:17:53  peckham
 * 	OT#5934 - remove offending CMA workaround.
 * 	[1992/12/18  18:14:12  peckham]
 * 
 * Revision 1.1.5.2  1992/09/29  18:51:51  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:15:34  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  22:20:05  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:31:35  weisman]
 * 
 * Revision 1.1  1992/01/19  15:19:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: dnsdaemon.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
 * Module to change current process to daemon. This was generalized and
 * incorporates necessary code to avoid CMA fork() bugs.
 *
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <dns_strings.h>
  
void 
dns_daemon (char *myname_p)
{
    register int i;
    FILE *outfile;
    char file[128];


    struct sigaction sigactn, prev_sig_details;

    sigactn.sa_handler = SIG_IGN;
    sigemptyset(&sigactn.sa_mask);
    sigactn.sa_flags   = 0;

    /*
     * disable CMA scheduler 
     */
    sigaction(SIGVTALRM, &sigactn, &prev_sig_details);


    if ((i = fork()) != 0)
    {
	if (0 > i)
	{
	  dce_svc_printf(CDS_S_LIBRARY_FORKSELF_MSG, (long)errno);
	}
    }


    /*
     * re-enable CMA scheduler 
     */
    sigaction(SIGVTALRM, &prev_sig_details, NULL);


    if (setsid() < 0)
    {
	dce_svc_printf(CDS_S_LIBRARY_SETSID_MSG, strerror(errno));
    }
    /*
     * Attempt to get privileged user status for the daemon
     */
    if (dce_priv_proc_enable() < 0)
    {
	dce_svc_printf(CDS_S_NOPRIV_EB_MSG);
    }
    
    /*
     * We're detached and running on our own.  We must reassign
     * stdout and stderr to files or we'll end up spewing some
     * random garbage into a random terminal at some random
     * future time.
     */
    sprintf(file, DNS_LOG_TMPL, myname_p, getpid());
    outfile = freopen(file,"w", stdout);
    setvbuf(outfile, NULL, _IONBF, 0);
    dup2(1,2);
}










