/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: mgmt_init.c,v $
 * Revision 1.1.8.2  1996/02/18  19:36:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:54  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:31:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:28  root]
 * 
 * Revision 1.1.6.7  1994/09/06  17:37:11  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:30:53  proulx]
 * 
 * Revision 1.1.6.6  1994/08/24  20:14:38  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:08:55  mccann]
 * 
 * Revision 1.1.6.5  1994/08/03  19:04:22  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:25:50  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  18:43:35  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:08  devsrc]
 * 
 * Revision 1.1.6.3  1994/04/29  15:56:09  peckham
 * 	Use dcelocal_path in file names.
 * 	Use dcecdsmac.h macros for serviceability.
 * 	[1994/04/29  14:37:11  peckham]
 * 
 * Revision 1.1.6.2  1994/04/14  14:51:22  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:21:00  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:11:55  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:43:49  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:22:22  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:22:53  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/30  12:39:43  keegan
 * 	Initialize ns_p->ns_sec_state to 'cds_sec_Off'
 * 	[1992/04/29  18:55:22  keegan]
 * 
 * Revision 1.1  1992/01/19  15:26:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module mgmt_init.c
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
 *
 */

#include <server.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <dns_strings.h>

static char *
mgmt_strdup (char *old_p)
{
    char *const new_p = malloc(strlen(old_p)+1);

    if (new_p) strcpy(new_p, old_p);

    return(new_p);
}

/*
 * this should only get called once ever.
 * as it creates mutexes and condition variables that are not deleted
 *
 * NOTE: the use of strdup() for the file names allocates memory
 * which may never be released. I don't think anything dismantles
 * this structure anyway.
 */
void 
mgmt_init (nsgbl_ns_t *ns_p)
{
    char		filename[_DNS_MAXPATHLEN+1];
    time_local_t	temp;
    int thstatus;
    
    /* Initialize global nameserver management structure */
    ZERO_bytes(ns_p, sizeof(*ns_p));

    /* Circular list header */
    init_list(&ns_p->ns_clearinghouses);

    /* The management clearinghouse directory prefix */
    sprintf(filename, CDS_SVR_DBDIR, dcelocal_path);
    ns_p->ns_mgmt_directory = mgmt_strdup(filename);

    /* Mailbox names */
    sprintf(filename, CDS_DIAG_SOCK, dcelocal_path);
    ns_p->ns_diag_name_p = mgmt_strdup(filename);

    utc_gettime(&ns_p->ns_create_time);

    /* Protocol version */
    INS8(ns_p->ns_min_protocol.cv_major, MINVERSION);
    INS8(ns_p->ns_min_protocol.cv_minor, 0);
    INS8(ns_p->ns_max_protocol.cv_major, MAXVERSION);
    INS8(ns_p->ns_max_protocol.cv_minor, 0);

    /* Seconds in the future a timestamp can be to be valid */
    sys_time_from_interval(&temp, 0, 5, 0);
    sys_time_to_utc(&temp, &ns_p->ns_future_skew);

    /* Time between skulks of low convergence */
    sys_time_from_interval(&temp, 24, 0, 0);
    sys_time_to_utc(&temp, &ns_p->ns_skulk_time);

    ns_p->ns_state = dns_Off;
    
    /*
     * initialize lock for structure 
     */
    if ((thstatus = dthread_create_mutex(&ns_p->ns_mutex)) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_create_condition(&ns_p->ns_state_changed)) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
    }

#ifdef DCE_SEC

    if ((thstatus = dthread_create_mutex(&ns_p->ns_sec_mutex)) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_create_condition(&ns_p->ns_sec_state_changed)) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
    }


    ns_p->ns_sec_state = cds_sec_Off;

#endif

    /*
     * lastly, if we are built for CDS, tell the control program's
     * RPC interface where the nameserver data structure is.
     */
    cdscp_server_init (ns_p);
}
