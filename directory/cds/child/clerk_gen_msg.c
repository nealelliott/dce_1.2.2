/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_gen_msg.c,v $
 * Revision 1.1.14.3  1996/02/18  19:25:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:01  marty]
 *
 * Revision 1.1.14.2  1995/12/08  15:03:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:53  root]
 * 
 * Revision 1.1.10.1  1994/10/25  16:17:28  proulx
 * 	     fix break at end of hint case in switch in sendCH
 * 	[1994/10/25  15:02:35  proulx]
 * 
 * Revision 1.1.8.5  1994/08/03  18:59:23  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:10:53  mccann]
 * 
 * Revision 1.1.8.4  1994/06/23  18:28:45  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:53:49  mccann]
 * 
 * Revision 1.1.8.3  1994/06/09  16:07:34  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:15  devsrc]
 * 
 * Revision 1.1.8.2  1994/04/14  14:41:07  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:35:51  peckham]
 * 
 * Revision 1.1.8.1  1994/03/12  21:56:16  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:50:09  peckham]
 * 
 * Revision 1.1.6.1  1993/10/14  16:06:01  peckham
 * 	CR#7759: Cannot use more than 2 clearinghouses on one host.
 * 	Add ReplicaPointer argument to sendCH() and consolidate
 * 	sendToReplica() code. Merge DEC clearinghouse search loop
 * 	into sendCH(). This code will increase confidence to high
 * 	if it gets and empty Tower set (from an un-skulked replica
 * 	clearinghouse object). Eliminate use of non-reentrant full2str().
 * 	Some cleanup.
 * 	[1993/10/11  03:48:10  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:40:40  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:26:10  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/26  06:30:54  jim
 * 	Merge AIX 32 changes with latest level for DCE1_0_1.
 * 	[1992/06/23  14:53:51  jim]
 * 
 * Revision 1.1.2.2  1992/03/22  21:20:04  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:07:28  weisman]
 * 	Revision 1.1.3.2  1992/05/29  18:40:57  jim
 * 	Added include msgblk.h. This is not getting included on AIX 3.2
 * 	for some reason.
 * 
 * Revision 1.1  1992/01/19  15:27:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: clerk_gen_msg
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
 * MODULE DESCRIPTION:
 *
 *	Generic message sender.  To be used when we know which clearinghouse
 *	we want to talk to.  This eliminates the tree-walk hassle.  This
 *	routine builds a standard message, inserting request specific
 *	information into the message, and sends the message.
 *
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>
#include <cache.h>
#include <dns_malloc.h>

/*
 * Function prototypes
 */
static dns_status_t
send_msg (thread_t *,
	  FullName_t *,
	  ObjUID_t *,
	  Set_t *);

void 
sendToClearinghouse (thread_t *,
                     byte_t *);

void 
sendToReplica (thread_t *,
               ReplicaPointer_t *);

void
sendCH (thread_t          *,
        byte_t            *,
        ReplicaPointer_t  *);



/*
 * send_msg
 */
static dns_status_t 
send_msg (thread_t    *th_p,
          FullName_t  *chname_p,
          ObjUID_t    *id_p,
          Set_t       *address_p)
{

    DEB_ASCII_BUF_thread(threadBuf)
    LOG_ASCII_BUF_FullName(chnameBuf)
    DEB_ASCII_BUF_ObjUID(idBuf)
    DEB_ASCII_BUF_Set(addressBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    DEB_ASCII_BUF_dns_status(clerkStatusBuf)
    dns_status_t status = DNS_NOTSUPPORTED;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >send_msg(%s,%s,%s,%s) operation(%d)",
        deb_ascii_thread(threadBuf,th_p),
        deb_ascii_FullName(chnameBuf,chname_p),
        deb_ascii_ObjUID(idBuf,id_p),
        deb_ascii_Set(addressBuf,address_p),
        th_p->taOperation));

     /*
      *  Try RPC 
      */
     if (th_p->taRPC) 
     {
         /* Initialize both pointers and reuse buffer if it exists */
         if (!(th_p->taRspMsg_p = th_p->taRspBuf_p))
         {
             th_p->taRspMsg_p =
                   th_p->taRspBuf_p = 
	                 (byte_t *)dns_get_buffer(clerk_comm_bt, NSMAXSIZE);
         }
       
         if (!th_p->taRspMsg_p)
         {
  	   status = DNS_RESOURCEERROR;
         }
         else 
             if (NUM_Set(address_p) == 0) 
             {
  	       int getout = 0;
                 maximize_rpc_error(th_p, rpc_s_no_bindings, &getout);
                 status = DNS_NOCOMMUNICATION;
             } 
             else 
             {
                 /* 
		  * Save operation type and UUID for trace 
                  */

                 INS8(th_p->taRspMsg_p, th_p->taOperation);
                 th_p->taRspMsg_p = SKIP_byte(th_p->taRspMsg_p);
                 COPY_ObjUID(id_p, th_p->taRspMsg_p);
                 th_p->taRspMsg_p = SKIP_ObjUID(th_p->taRspMsg_p);
                   th_p->taRspBufLen = NSMAXSIZE;
                 status = (*th_p->taRPC)(th_p,address_p,id_p);
  
                 if (status == DNS_SUCCESS)
                 {
  		     th_p->taVersion = 3;
	         }
             }
     }
 
    /*
     * If successful, check for exception
     * and fold into status code
     */
    if (status == DNS_SUCCESS) 
    { 
        exceptionMessage_t *em_p;

        /*
	 * Update cache counters 
	 */
        QueueClearinghouse(th_p, chname_p, id_p, th_p->OPtype, address_p);
        COPY_ObjUID(id_p, th_p->lastch);
        switch (th_p->taRspType) 
        {
            case MT_Response:
                 /* Set clerk counter */
                 ClerkCounter(th_p->OPtype);
                 break;
            case MT_Exception:
                 em_p = (exceptionMessage_t *)th_p->taRspMsg_p;
                 status = DNS_NAMESERVERERRORS +EXT32(em_p->em_exceptionValue);
                 if ((status == DNS_ACCESSVIOLATION) || 
                     (status == DNS_UNTRUSTEDCH))
                 {
                     ++CAfixed_p->ca_authenticationfailure;
		 }
                 break;
            default:
                     status = DNS_CLERKBUG;
        } /* end switch */
    } 
    else 
    {   /* No. 1 */

        dce_svc_printf(CDS_S_CHILD_NOCOMMUNICATION_MSG,
            deb_ascii_FullName(chnameBuf,chname_p), (long)status);

   } /* end of "else No. 1" --> status != DNS_SUCCESS */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <send_msg(%s,%s,%s,%s) return(%s) clerkStatus(%s)",
        deb_ascii_thread(threadBuf,th_p),
        deb_ascii_FullName(chnameBuf,chname_p),
        deb_ascii_ObjUID(idBuf,id_p),
        deb_ascii_Set(addressBuf,address_p),
        deb_ascii_dns_status(statusBuf,status),
        deb_ascii_dns_status(clerkStatusBuf,th_p->clerkStatus)));

    return(status);
}



/*
 *	sendToClearinghouse
 */
void 
sendToClearinghouse (thread_t  *th_p,
                     byte_t    *clearinghouse_p)
{
    DEB_ASCII_BUF_thread(threadBuf)
    DEB_ASCII_BUF_FullName(clearinghouseBuf)
    DEB_ASCII_BUF_dns_status(clerkStatusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >sendToClearinghouse(%s,%s)",
        deb_ascii_thread(threadBuf,th_p),
        deb_ascii_FullName(clearinghouseBuf,clearinghouse_p)));

    sendCH(th_p, clearinghouse_p, NULL);

    if ((th_p->clerkStatus == DNS_SUCCESS) && th_p->taParseRsp)
	(*th_p->taParseRsp)(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <sendToClearinghouse(%s,%s) clerkStatus(%s)",
        deb_ascii_thread(threadBuf,th_p),
        deb_ascii_FullName(clearinghouseBuf,clearinghouse_p),
        deb_ascii_dns_status(clerkStatusBuf,th_p->clerkStatus)));
}


/*
 *	sendToReplica
 */
void 
sendToReplica (thread_t          *th_p,
               ReplicaPointer_t  *replica_p)
{
    DEB_ASCII_BUF_thread(threadBuf)
    DEB_ASCII_BUF_FullName(chnameBuf)
    DEB_ASCII_BUF_dns_status(clerkStatusBuf)
    FullName_t *const chname_p = FLD_ReplicaPointer_CHName(replica_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >sendToReplica(%s,%s)",
        deb_ascii_thread(threadBuf,th_p),
        deb_ascii_FullName(chnameBuf,chname_p)));

    sendCH(th_p, (byte_t *)chname_p, replica_p);

    if ((th_p->clerkStatus == DNS_SUCCESS) && th_p->taParseRsp)
	(*th_p->taParseRsp)(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <sendToReplica(%s,%s) status(%s)",
        deb_ascii_thread(threadBuf,th_p),
        deb_ascii_FullName(chnameBuf,chname_p),
        deb_ascii_dns_status(clerkStatusBuf,th_p->clerkStatus)));
}


/*
 *	sendCH - doesn't parse response here
 */
void
sendCH (thread_t          *th_p,
        byte_t            *clearinghouse_p,
        ReplicaPointer_t  *replica_p)
{
    DEB_ASCII_BUF_thread(threadBuf)
    DEB_ASCII_BUF_FullName(clearinghouseBuf)
    DEB_ASCII_BUF_ReplicaPointer(replicaBuf)
    DEB_ASCII_BUF_FullName(newchnameBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    FullName_t chnameBuf;
    dns_status_t status;
    enum {
	snd_cache = 1,
	snd_hint,
	snd_medium,
	snd_high,
	snd_done };
    int snd_state = snd_cache;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >sendCH(%s,%s,%s)",
        deb_ascii_thread(threadBuf,th_p),
        deb_ascii_FullName(clearinghouseBuf,clearinghouse_p),
        deb_ascii_ReplicaPointer(replicaBuf,replica_p)));

    status = dns_normalize_fname((FullName_t *)clearinghouse_p,
				&chnameBuf, sizeof(chnameBuf));

    do {
	CleEntry_t *cle_p;
	CHlist_t *list_p = NULL;
	int conf = CO_medium;

	switch (snd_state) 
	{
	    case snd_high:
	             conf = CO_high;
            case snd_medium:
                     status = clerk_find_ch(th_p, &chnameBuf, conf, &list_p);
		     if (status == DNS_SUCCESS) 
		     {
			 status = send_msg(th_p, (FullName_t *)list_p->ch_name,
					   (ObjUID_t *)list_p->ch_id, 
					   (Set_t *)SKIP_FullName
					              (list_p->ch_name));
		     }
		     if (list_p) 
		     {
			 /*
			  * If we still didn't reach the clearinghouse,
			  * mark it as "not ok"
			  */
			 if (status == DNS_NOCOMMUNICATION) 
			 {
			     DCE_SVC_DEBUG((
				 cds__svc_handle,
                                 cds_svc_child,
                                 svc_c_debug6,
                                 "  sendCH(%s,%s,%s), status(%s), ok(%d) changed to 0",
                                 deb_ascii_thread(threadBuf,th_p),
                                 deb_ascii_FullName(newchnameBuf,
						    list_p->ch_name),
                                 deb_ascii_ReplicaPointer(replicaBuf,
							  replica_p),
                                 deb_ascii_dns_status(statusBuf,status),
                                 list_p->ch_flags.flags.ok));
		                 CACleSetOk((FullName_t*)list_p->ch_name, 0);
			 }
			 dns_free((char *)list_p);
		     }
		     break;
	    case snd_hint:
		     if (!replica_p)
		     {
			 continue;
		     }
		     status = send_msg(th_p, &chnameBuf,
				       (ObjUID_t *)replica_p->rp_CHID,
				       (Set_t *)replica_p->rp_addressHint);
		     break;
            case snd_cache:
		     /*
		      *  Check cache for recently used address for this clearinghouse 
		      */
		     cle_p = CALookupClearinghouse(&chnameBuf);
		     if (cle_p) 
		     {
			 /*
			  * Try it out 
			  */
			 status = send_msg(th_p, (FullName_t *)cle_p->ch_name,
					   (ObjUID_t *)cle_p->ch_id,
					   (Set_t *)SKIP_FullName
					            (cle_p->ch_name));
			 dns_free((char *)cle_p);
			 break;
		     }
	    default:
		     status = DNS_UNKNOWNCLEARINGHOUSE;
		     continue;
	} /* end of switch */

	if (status == DNS_UNKNOWNCLEARINGHOUSE)
	{
	    continue;
	}
	
	if (status != DNS_NOCOMMUNICATION)
	{
	    break;
	}
	
	if ((th_p->clerkStatus  == ept_s_not_registered)
	    || (th_p->clerkStatus  == rpc_s_endpoint_not_found)
	    || (th_p->clerkStatus  == rpc_s_no_bindings))
	{
	    continue;
	}
	break;

    } while (++snd_state != snd_done);  /* end of do loop ! */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <sendCH(%s,%s,%s) return status(%s)",
        deb_ascii_thread(threadBuf,th_p),
        deb_ascii_FullName(clearinghouseBuf,clearinghouse_p),
        deb_ascii_ReplicaPointer(replicaBuf,replica_p),
        deb_ascii_dns_status(statusBuf,status)));

    th_p->clerkStatus = status;
}
