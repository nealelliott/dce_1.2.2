/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: event_lib.c,v $
 * Revision 1.1.9.2  1996/02/18  19:36:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:50  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:31:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:23  root]
 * 
 * Revision 1.1.7.5  1994/08/03  19:04:17  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:25:38  mccann]
 * 
 * Revision 1.1.7.4  1994/06/23  19:11:02  jd
 * 	Remove evtblk. It no longer exists
 * 	[1994/06/23  15:41:20  jd]
 * 
 * Revision 1.1.7.3  1994/06/09  18:43:31  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:03  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/14  14:51:20  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:20:47  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:11:46  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:43:29  peckham]
 * 
 * Revision 1.1.5.4  1992/12/30  14:22:08  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:22:34  zeliff]
 * 
 * Revision 1.1.5.3  1992/12/16  21:42:28  keegan
 * 	Don't try to print syslog message if catalog file can't be opened or
 * 	read from.
 * 	[1992/12/16  21:41:21  keegan]
 * 
 * Revision 1.1.5.2  1992/09/29  19:15:56  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:17:08  weisman]
 * 
 * Revision 1.1.2.3  1992/06/26  06:44:26  jim
 * 	Added include msgblk.h. This is not getting included on AIX 3.2
 * 	for some reason.
 * 	[1992/05/29  18:58:05  jim]
 * 
 * Revision 1.1.2.2  1992/03/22  22:31:43  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:35:27  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: event_lib.c
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

#include <server.h>

#include <events.h>
#include <dns_cfg.h>

/* Nameserver Entity Events */

void 
evl_SecurityFailure (nsgbl_ns_t  *ns_p,
                     FullName_t  *name_p)
{
    LOG_ASCII_BUF_FullName( namebuf)

    deb_ascii_FullName( namebuf, name_p);

    INC_MGMT_COUNTER(&ns_p->ns_SecurityFailures);

    dce_svc_printf(CDS_S_SERVER_SECURITYEVENT_MSG, namebuf); 
}

void 
evl_BrokenLookupPaths (nsgbl_ns_t  *ns_p,
                       FullName_t  *name_p)
{
    LOG_ASCII_BUF_FullName( namebuf)

    deb_ascii_FullName( namebuf, name_p);


    INC_MGMT_COUNTER(&ns_p->ns_TimesLookupPathBroken);
    dce_svc_printf(CDS_S_SERVER_PATHBROKENEVENT_MSG, namebuf);

}

void 
evl_CrucialReplica (nsgbl_ns_t  *ns_p,
                    FullName_t  *directory_p,
                    FullName_t  *clearinghouse_p)
{
    LOG_ASCII_BUF_FullName( namebuf)

    deb_ascii_FullName( namebuf, clearinghouse_p);

    INC_MGMT_COUNTER(&ns_p->ns_CrucialReplicaRemovalsBackedOut);

    /* Only the root in crucial */
    dce_svc_printf(CDS_S_SERVER_CRUCIALEVENT_MSG, namebuf);
}

void 
evl_CannotUpdateChildPointer (nsgbl_ns_t      *ns_p,
                              FullName_t      *directory_p,
                              DNSException_t  reason)
{
    LOG_ASCII_BUF_FullName( namebuf)

    deb_ascii_FullName( namebuf, directory_p);

    INC_MGMT_COUNTER(&ns_p->ns_ChildPointerUpdateFailures);
    dce_svc_printf(CDS_S_SERVER_CHILDUPDATEEVENT_MSG, namebuf,
		   (long)reason);
}

void 
evl_PossibleCycle (nsgbl_ns_t  *ns_p,
                   FullName_t  *name_p)
{
    LOG_ASCII_BUF_FullName( namebuf)

    deb_ascii_FullName( namebuf, name_p);

    INC_MGMT_COUNTER(&ns_p->ns_PossibleCycles);
    dce_svc_printf(CDS_S_SERVER_POSSIBLECYCLEEVENT_MSG, namebuf);
}

/* Clearinghouse Entity Events */

void 
evl_RootLost (nsgbl_ch_t *ch_p)
{
    LOG_ASCII_BUF_FullName( namebuf)

    deb_ascii_FullName( namebuf, &ch_p->ch_name);

    INC_MGMT_COUNTER(&ch_p->ch_TimesRootNotReachable);
    dce_svc_printf(CDS_S_SERVER_ROOTLOSTEVENT_MSG, namebuf);
}

void 
evl_SkulkFailed (nsgbl_ch_t      *ch_p,
                 FullName_t      *directory_p,
                 DNSException_t  reason)
{
    LOG_ASCII_BUF_FullName( namebuf)

    deb_ascii_FullName( namebuf, directory_p);

    INC_MGMT_COUNTER(&ch_p->ch_SkulkFailures);
    
    dce_svc_printf(CDS_S_SERVER_SKULKFAILEDEVENT_MSG, namebuf, (long)reason);

}
void 
evl_UpgradeNotPossible (nsgbl_ch_t      *ch_p,
                        FullName_t      *directory_p,
                        DNSException_t  reason)
{
    LOG_ASCII_BUF_FullName( namebuf)

    deb_ascii_FullName( namebuf, directory_p);

    INC_MGMT_COUNTER(&ch_p->ch_UpgradesNotPossible);

    dce_svc_printf(CDS_S_SERVER_NOUPGRADEEVENT_MSG, namebuf, (long)reason);

}

void 
evl_AttributeMissing (nsgbl_ch_t       *ch_p,
                      FullName_t       *directory_p,
                      AttributeName_t  *attribute_p)
{
    LOG_ASCII_BUF_FullName( namebuf)
    LOG_ASCII_BUF_AttributeName(attrbuf)

    deb_ascii_FullName( namebuf, directory_p);
    deb_ascii_AttributeName(attrbuf, attribute_p);

    dce_svc_printf(CDS_S_SERVER_ATTRIBUTEMISSING_MSG, namebuf, attrbuf);


}

void 
evl_WrongState (nsgbl_ch_t  *ch_p,
                FullName_t  *directory_p,
                int         state)
{
    LOG_ASCII_BUF_FullName( namebuf)

    deb_ascii_FullName( namebuf, directory_p);

    dce_svc_printf(CDS_S_SERVER_WRONGSTATE_MSG, namebuf, state);
}
