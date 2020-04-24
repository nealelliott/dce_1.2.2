/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_tree_walk.c,v $
 * Revision 1.1.10.2  1996/02/18  19:25:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:11  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:04:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:07  root]
 * 
 * Revision 1.1.8.5  1994/08/03  18:59:29  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:13:53  mccann]
 * 
 * Revision 1.1.8.4  1994/06/23  18:28:54  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:54:37  mccann]
 * 
 * Revision 1.1.8.3  1994/06/09  16:07:42  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:25  devsrc]
 * 
 * Revision 1.1.8.2  1994/04/14  14:41:13  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:36:22  peckham]
 * 
 * Revision 1.1.8.1  1994/03/12  21:56:28  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:51:28  peckham]
 * 
 * Revision 1.1.6.1  1993/10/14  16:06:03  peckham
 * 	CR#7759: Cannot use more than 2 clearinghouses on one host.
 * 	Merge DEC clearinghouse search loop into processCH(). This
 * 	code will increase confidence to high if it gets and empty
 * 	Tower set (from an un-skulked replica clearinghouse object).
 * 	Reverse arguments to processCH() for better code.
 * 	Supply ReplicaPointer argument to sendCH().  Some cleanup.
 * 	[1993/10/11  03:52:58  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  12:41:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:26:59  zeliff]
 * 
 * Revision 1.1.4.2  1992/11/13  15:42:48  peckham
 * 	OT#5649: processCH uses clearinghouse pointer after it has been freed.
 * 	Okay, okay! I'll stop using it...
 * 	[1992/11/13  15:41:52  peckham]
 * 
 * Revision 1.1.2.3  1992/06/26  06:31:08  jim
 * 	Merge AIX 32 changes with latest level for DCE1_0_1.
 * 	[1992/06/23  14:57:34  jim]
 * 
 * Revision 1.1.2.2  1992/03/22  21:22:06  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:08:01  weisman]
 * 	Revision 1.1.3.2  1992/05/29  18:41:50  jim
 * 	Added include msgblk.h. This is not getting included on AIX 3.2
 * 	for some reason.
 * 
 * Revision 1.1  1992/01/19  15:27:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Program Clerk, Module clerk_tree_walk.c
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
 * This is the "backbone" of the ULTRIX Nameserver Clerk.  This routine
 * attempts to contact clearinghouses in an attempt to locate the name
 * given by the client and satisfy the client's request.
 * 
 * Networks & Communications Software Engineering
 * 
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#ifdef _AIX
#include <sys/types.h>
#endif
#include <uc_child.h>
#include <uc_clerk.h>
#include <dce/attributes.h>

/*
 * local prototypes 
 */

static void
clerk_WalkTree (thread_t *,
		FullName_t *);

static dns_status_t 
processCH (thread_t *,
	   CHlist_t *);

static dns_status_t 
processAddr (thread_t *,
	     CHlist_t *,
	     Set_t *);

static dns_status_t
SLLoop_check (thread_t	*,
	      struct State *);




/*
 * This routine traverses the namespace tree in search of a name.
 * It attempts to resolve as much of the name as possible by using
 * a cache of directory locations.
 */
static void
clerk_WalkTree (thread_t    *th_p,
                FullName_t  *name)
{
    DEB_ASCII_BUF_thread(thBuf)
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_FullName(chooseBuf)
    DEB_ASCII_BUF_time_local(now)
    time_local_t        timeNow;
    struct State	linkLoop;
    FullName_u		stack_resolved;
    FullName_u		stack_unresolved;
    FullName_u		stack_prec;
    FullName_u		stack_prec2;
    FullName_u		choose_buf;
    CHlist_t		*tryCH;
    byte_t		*chname_p;
    int			new_name;
    int			loopcount;
    dns_status_t	status = DNS_SUCCESS;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerk_WalkTree(%s,%s)",
        deb_ascii_thread(thBuf,th_p),
        deb_ascii_FullName(nameBuf,name)));

    /*
     * Initialize values for progress record. 
     */
    ZERO_bytes(&th_p->taProgress, sizeof(th_p->taProgress));
    th_p->taProgress.Resolved = (FullName_t *)stack_resolved;
    th_p->taProgress.Unresolved = (FullName_t *)stack_unresolved;
    COPY_FullName(NullFullName, th_p->taProgress.Resolved);

    /*
     * Resolve any links before we start.  Make sure not looking for link 
     */
    if (!th_p->skipResolveNameCache) 
    {
	if (th_p->clerkConf == CO_low) 
	{
	    if (CAResolveName(th_p->user, th_p->clerkCacheTimeout, name, 
			      th_p->taProgress.Unresolved) == TRUE) 
	    {
		SET_FLAG(th_p->taProgress.flags, PR_linked);
		th_p->outLinked++;
	    }
	}
        else if (th_p->clerkConf == CO_medium)
	{ 
	    COPY_FullName(name, th_p->taProgress.Unresolved);
	} 
        else /* confidence = hi */
	{ 
	    /* 
	     * Try to resolvename now so we have the correct directory name
	     * to use later to locate the master replica.  Otherwise we
	     * may be making the request to the master of the link not the 
	     * target If we just tried walking the tree we may get access 
	     * errors reading the parent of the links replica set when that is
	     * not what we want anyway.
	     */
	    status = clerk_ResolveName(th_p, name,th_p->taProgress.Unresolved);
	    if (status == DNS_SUCCESS) 
	    {
		SET_FLAG(th_p->taProgress.flags, PR_linked);
		th_p->outLinked++;
	    } 
	    else
		if ((status==DNS_NOTLINKED) || (status==DNS_UNKNOWNENTRY)) 
		{
		    /* 
		     *Handle name not linked or entry doesn't exist for creates
		     */
		    COPY_FullName(name, th_p->taProgress.Unresolved);
		} 
		else 
		{
		    th_p->clerkStatus = status;
		    goto leave_clerk_WalkTree;
		}
	}
    }
    else
    {
	COPY_FullName(name, th_p->taProgress.Unresolved);
    }
    
    linkLoop.iteration = 0;

    /*
     * Allocate buffer space for the 2 prec buffers.
     */
    linkLoop.prec2 = (FullName_t *)stack_prec2;
    linkLoop.prec  = (FullName_t *)stack_prec;

    /*
     * Copy name of entry into both prec buffers
     * as the starting point for loop detection.
     */

    /*
     * NOTIMPLEMENTED
     *   COPY_FullName(name, linkLoop.prec);
     *   COPY_FullName(name, linkLoop.prec2);
     */

    /*
     * If a specific clearinghouse is specified, don't really walktree 
     */
    if ((chname_p = (byte_t *)clerkgetch(th_p)) != NULL) 
    {
	Progress_t *progress_p;

	SET_FLAG(th_p->taProgress.flags, PR_ignoreState);
	for (;;) 
	{
	    sendCH(th_p, chname_p, NULL);
	
	    if (th_p->clerkStatus != DNS_SUCCESS)
	    {
		break;
	    }
	
	    /* 
	     * A readattribute may have turned into a read replica
	     * for v1.0 compatiblilty.  If this happens, there
	     * is no progress record to use
	     */
	    if (th_p->taOperation == OP_ReadReplica)
	    {
		break;
	    }
	    
	    progress_p = (Progress_t *)th_p->taRspMsg_p;
	
	    /*
	     * If we are done the request succeeded. 
	     */
	    if (FLAG_SET(progress_p->pr_flags, PR_done))
	    {
		break;
	    }
	
	    /*
	     * If we didn't hit a link and we are not done, fail request 
	     */
	    if (FLAG_CLEAR(progress_p->pr_flags, PR_hitLink)) 
	    {
		th_p->clerkStatus = DNS_NOTAREPLICA;
		break;
	    }
	
	    /*
	     * Try the same clearinghouse again 
	     */
	    proc_link(th_p, progress_p);
	    status =  SLLoop_check(th_p, &linkLoop);
	    th_p->clerkStatus = status;
	    if (status != DNS_SUCCESS) 
	    {
		break;
	    }
	}
    }
    else 
    {
	SET_FLAG(th_p->taProgress.flags, PR_up);
	new_name = TRUE;
       
	do {

	    /*
	     * Release dead name loop detection full name -- used in progress.
	     */ 
	    if (th_p->dead.cnt) 
	    {
		dns_free((char *)th_p->dead.name_p);
                th_p->dead.cnt = 0;
	    }

	    if (new_name) 
	    {
		(void)dns_normalize_fname(th_p->taProgress.Unresolved,
					  (FullName_t *)choose_buf, 
					  sizeof(choose_buf));
		if (th_p->cache_the_dir == 0) 
		{
		    /*
		     * Looking for something other than a clearinghouse or a
		     * directory.  Strip the rightmost name off so we can
		     * lookup the directory in the global cache.
		     */
		    if (!EMPTY_FullName(choose_buf)) 
		    {
			(void)childStripRSimple((FullName_t *)choose_buf);
		    }	
		}
	    }
	
	    /*
	     * Pick an initial clearinghouse list based
	     * on cached information and requested name.
	     * This will result in a replica record for the entry being put
	     * in the progress record.  If this is not the first call to
	     * chooseinitch and a link has not been found, choose_buf reflects
	     * the last name we tried.  This name should get smaller not
	     * longer or else we could end in a loop.
	     * If high confidence and name has replicas (not a clearinghouse)
	     * get the location of the master only 
	     */
	    if ((th_p->clerkConf == CO_high) && !th_p->NoReplicas)
	    {
		th_p->taProgress.Replica =
		    ChooseMaster(th_p, (FullName_t *)choose_buf);
	    }
	    else
	    {
		th_p->taProgress.Replica = 
		    ChooseInitCh(th_p, (FullName_t *)choose_buf, new_name);
	    }
	    
	    DCE_SVC_DEBUG((
	        cds__svc_handle,
		cds_svc_child,
		svc_c_debug4,
		"  clerk_WalkTree(%s,%s) looking for (%s)",
		deb_ascii_thread(thBuf,th_p),
		deb_ascii_FullName(nameBuf,name),
		deb_ascii_FullName(chooseBuf,choose_buf)));

	    new_name = FALSE;	
	    if (th_p->taProgress.Replica == NULL) 
	    {
		

		if (th_p->clerkStatus == DNS_SUCCESS)
		    if (EQ_SimpleName(name->fn_name, GlobalRoot)) 
		    {
			th_p->clerkStatus = DNS_UNKNOWNENTRY;
		    }
		    else 
		    {
			th_p->clerkStatus = DNS_NOCOMMUNICATION;
		    }
		break;
	    }
	    

	    CLEAR_FLAG(th_p->taProgress.flags, PR_hitLink);

	    /*
	     * Now let's choose one of the replicas which best suits
             * our clients needs!  We'll keep trying replicas until
	     * there are no more replicas, we have an answer or
	     * softlink is found.
	     */
	    loopcount = 0;

	    while ((tryCH = ChooseCh(th_p, &th_p->taProgress.Replica, loopcount++))
	       != NULL) 
	    {
		/* 
		 * If no error stashed away, prepare for the least descriptive 
		 */


		if (th_p->clerkStatus ==  DNS_SUCCESS)
		    if (EQ_SimpleName(name->fn_name, GlobalRoot)) 
		    {
			th_p->clerkStatus = DNS_UNKNOWNENTRY;
		    }
		    else 
		    {
			th_p->clerkStatus = DNS_NOCOMMUNICATION;
		    }
		status = processCH(th_p, tryCH);

		/*
		 * Enforce timeout
		 * (see clerk_request() in clerk_client.c for how it's set).
		 */
		if (status != DNS_SUCCESS) 
		{
		    /*
		     * Check for errors which won't go away by trying a
		     * different clearinghouse.
		     */
		    if ((th_p->clerkStatus == rpc_s_auth_tkt_expired) ||
			(th_p->clerkStatus == sec_rgy_server_unavailable))
		    {
			break; /* Don't waste time trying - give up */
		    }
	    
		    sys_time_from_utc(&timeNow, NULL);
		    if (GT_time_local(&timeNow, &(th_p->expire))) 
		    {
			th_p->clerkStatus = DNS_TIMEOUTNOTDONE;

			DCE_SVC_DEBUG((
                            cds__svc_handle,
                            cds_svc_child,
                            svc_c_debug4,
                            "clerk_WalkTree(%s,%s) Timeout at %s",
   		            deb_ascii_thread(thBuf,th_p),
		            deb_ascii_FullName(nameBuf,name),
		            deb_ascii_time_local(now, &timeNow)));

  		            /*
		             * Force us out of all loops 
		             */
		            status = DNS_SUCCESS;
		            CLEAR_FLAG(th_p->taProgress.flags, PR_hitLink);
		            break;
		    }		
		}  /* end of "if (status != DNS_SUCCESS)" */

		/* 
		 * if done, then the clearinghouse produced results. 
		 * Need to determine if we are completly done or
		 * just hav a new chlist to process.
		 */
		if (status == DNS_SUCCESS) 
		{
		    if (FLAG_SET(th_p->taProgress.flags, PR_hitLink)) 
		    {
			/*
			 * Now check if we're looping around
			 * in soft links.  
			 */
			new_name = TRUE;
			status = SLLoop_check(th_p, &linkLoop);
			if (status != DNS_SUCCESS) 
			{
			    th_p->clerkStatus =  status;
			    CLEAR_FLAG(th_p->taProgress.flags, PR_hitLink);
			    status = DNS_SUCCESS;
			}
		    }
		    break;
		} /* End status == success */
	    } /* End loop through clearinghouses */


	    /*	
	     * Free replica records from memory. 
	     */
	    rem_reps(&th_p->taProgress.Replica);
	
	    /* 
	     * If status != success, then we have exhausted the chlist.
	     * If any part of the name was resolved, we are done.  We
	     * cannot keep searching up the tree if we have already
	     * started down.
	     * 
	     * Processing within this loop only continues if we hit a softlink
	     * or the last replica set we got from chooseinitch produced
	     * no results.
	     */
	    if (status != DNS_SUCCESS)
		if (NE_bytes(NullFullName, th_p->taProgress.Resolved,
			     sizeof(NullFullName))
		    || th_p->clerkConf == CO_high) 
		{
		    status = DNS_SUCCESS;
		}
	} while ((status != DNS_SUCCESS) ||
	       FLAG_SET(th_p->taProgress.flags, PR_hitLink));
    }	

    /* 
     * If we have a linked name cache the beginning and ending name 
     * We don't cache any links found in-between because we may not care.
     */
    if (th_p->outLinked && th_p->clerkStatus == DNS_SUCCESS) 
    {
	Progress_t *const pr_p = (Progress_t *)th_p->taRspMsg_p;
	Timeout_t *const timeout_p = (Timeout_t *)pr_p->pr_timeout;
	FullName_t *const resolvedName_p =
			(FullName_t *)SKIP_FullName(pr_p->pr_unresolvedName);

	/*
	 * Add softlink to cache  
	 */
	(void)CAInsertLink(th_p->user, name, resolvedName_p, timeout_p);
    }

leave_clerk_WalkTree:  

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerk_WalkTree(%s,%s)",
        deb_ascii_thread(thBuf,th_p),
        deb_ascii_FullName(nameBuf,name)));
}



/*
 * Attempt to contact a clearinghouse.  First try any cached address
 * for the clearinghouse, then try the address hint.  If this fails on all 
 * plausible clearinghouses, lookup ch objects to get the correct address.
 * Initially tryCH contains the cached and hint addresses.
 * Returns:
 *     DNS_SUCCESS if done with name
 *     DNS_NOTFND if need to continue with chlist
 */
static dns_status_t 
processCH (thread_t  *th_p,
           CHlist_t  *tryCH_p)
{
  DEB_ASCII_BUF_FullName(ch_nameBuf)
  DEB_ASCII_BUF_thread(thBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  Set_t *tryCHAddress_p = (Set_t *)SKIP_FullName(tryCH_p->ch_name);
  Set_t *prev_p = tryCHAddress_p;
  register dns_status_t status = DNS_NOCOMMUNICATION;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug5,
      " >processCH(%s,%s)",
      deb_ascii_thread(thBuf,th_p),
      deb_ascii_FullName(ch_nameBuf,tryCH_p->ch_name)));
  /*
   * ChooseCH returns either the cached address (towerset) if it exists
   * or the address hint (anther towerset) otherwise.
   * This combines the cache and hint cases described in the architecture.
   *
   * Note:  ChooseCh will return exactly one towerset, therefore
   * ch_count will never be greater than 1.
   */

  /* Try the cached address, if we haven't already. */
  if (tryCH_p->ch_count > 0)
  {
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        "  processCH(%s,%s): try addr from cache or hint",
        deb_ascii_thread(thBuf,th_p),
        deb_ascii_FullName(ch_nameBuf,tryCH_p->ch_name)));

    status = processAddr(th_p, tryCH_p, tryCHAddress_p);
    /*
     * If we still didn't reach the clearinghouse,
     * mark it as "not ok"
     */
    if (status == DNS_NOCOMMUNICATION) 
    {

	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_child,
          svc_c_debug5,
          "  processCH(%s,%s): status(%s), ok is %d, will be 0",
          deb_ascii_thread(thBuf,th_p),
          deb_ascii_FullName(ch_nameBuf,tryCH_p->ch_name),
          deb_ascii_dns_status(statusBuf,status),
          tryCH_p->ch_flags.flags.ok));

      (void)CACleSetOk((FullName_t*)tryCH_p->ch_name, 0);
      tryCH_p->ch_flags.flags.ok = 0;
    }
  }

  /*
   * Before looping around to find address for this clearinghouse,
   * try other clearinghouses first.  Only bother to try to find it
   * if the error could mean that the clearinghouse has been moved.
   */
  if (status == DNS_NOCOMMUNICATION) {
    if (tryCH_p->ch_count == 0) {
      if ((tryCH_p->last_failure != ept_s_not_registered) &&
          (tryCH_p->last_failure != rpc_s_endpoint_not_found) &&
          (tryCH_p->last_failure != DNS_UNKNOWNCLEARINGHOUSE)) 
      {

	  DCE_SVC_DEBUG((
	       cds__svc_handle,
               cds_svc_child,
               svc_c_debug5,
               "  processCH(%s): status(%s), ok is %d, will be 0",
               deb_ascii_thread(thBuf,th_p),
               deb_ascii_FullName(ch_nameBuf,tryCH_p->ch_name),
               deb_ascii_dns_status(statusBuf,status)));

	(void)CACleSetOk((FullName_t*)tryCH_p->ch_name, 0);
	tryCH_p->ch_flags.flags.ok = 0;
      } else {
	int conf = CO_medium;

	/* 
	 * Make sure the name we are looking for is not the name of this
	 * clearinghouse or we will end up here again.
	 */
	if (dnsCmpFull((dns_opq_fname *)tryCH_p->ch_name, 
		       (dns_opq_fname *)th_p->taProgress.Unresolved) == 0)
	  status = DNS_NOCOMMUNICATION;
	else while (TRUE) {
	  CHlist_t *newCH_p;

	  /* 
	   * Read clearinghouse object to get correct address.
	   * If successfull this call will return a new
	   * address in tryCH.  It may also return a new buffer
	   * if the address won't fit in current storage.
	   */

	  DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_child,
              svc_c_debug5,
              "  processCH(%s,%s): try lookup",
              deb_ascii_thread(thBuf,th_p),
              deb_ascii_FullName(ch_nameBuf,tryCH_p->ch_name)));

	  status = clerk_find_ch(th_p, (FullName_t *)tryCH_p->ch_name,
				 conf, &newCH_p);
	  if (status == DNS_SUCCESS) {
	    if (!SetsEqual(prev_p,
		       (Set_t *)SKIP_FullName(newCH_p->ch_name))) {
	      tryCHAddress_p = (Set_t *)SKIP_FullName(newCH_p->ch_name);
	      status = processAddr(th_p, newCH_p, tryCHAddress_p);
	    }  /* End !SetsEqual */
	    dns_free((char *)newCH_p);
	    if (status == DNS_SUCCESS) break;
          }  /* End clerk_find_ch success */
	  /* clerk_find_ch() II - try harder */
	  if (conf == CO_medium)
	    conf = CO_high;
	  else
	    break;
        }  /* End !dnsCmpFull */
      }  /* End last failure */
    }  /* End ch_count == 0 */
  }  /* End DNS_NOCOMMUNICATION */

  if (status != DNS_NOCOMMUNICATION) {
    /* Update counters if we are done */
    if ((status == DNS_SUCCESS) && 
	(FLAG_CLEAR(th_p->taProgress.flags, PR_hitLink))) {
      QueueClearinghouse(th_p, (FullName_t *)tryCH_p->ch_name, 
			    (ObjUID_t *)tryCH_p->ch_id,
			    th_p->OPtype, tryCHAddress_p);
      COPY_ObjUID(tryCH_p->ch_id, th_p->lastch);
      ClerkCounter(th_p->OPtype);
    }
    else
      QueueClearinghouse(th_p, (FullName_t *)tryCH_p->ch_name, 
			    (ObjUID_t *)tryCH_p->ch_id,
			    CH_NONE,  tryCHAddress_p);
    
    dns_free((char *)tryCH_p);
  } else {
      /*
       * Queue the clearinghouse we cannot talk to, setting the ch_count
       * to 0 and putting it at the end of the list.  Do the address lookup
       * only when we have exhausted all other possible hints.
       * (clerk_find_ch can be a long delay, because of recursion.)
       */
      if (tryCH_p->ch_count == 0)
	dns_free((char *)tryCH_p);
      else {
	  CHlist_t *newCH_p = th_p->taProgress.Replica;

	  if (newCH_p != NULL) {
	      while (newCH_p->ch_next != NULL)
		  newCH_p = newCH_p->ch_next;
	      newCH_p->ch_next = tryCH_p;
	      }
	  else
	      th_p->taProgress.Replica = tryCH_p;
	  tryCH_p->ch_next = NULL;
	  tryCH_p->ch_prev = newCH_p;
	  tryCH_p->ch_count = 0;
	  }
      status = DNS_NOTFND;
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug5,
      " <processCH(%s,) return(%s)",
      deb_ascii_thread(thBuf,th_p),
      deb_ascii_dns_status(statusBuf,status)));

  return(status);
}

/*
 * Try to contact a clearinghouse.  If a connection is made
 * update the cle cache and process the progress record.
 * Return:
 *     DNS_SUCCESS if done with current name 
 *     DNS_NOTFND if need continue processing chlist
 *     DNS_NOCOMMUNICATION if address failed.
 */
static dns_status_t 
processAddr (thread_t  *th_p,
             CHlist_t  *tryCH_p,
             Set_t     *address_p)
{

#if defined(DCE_CDS_DEBUG)

    DEB_ASCII_BUF_thread(thBuf)
    DEB_ASCII_BUF_FullName(ch_nameBuf)
    DEB_ASCII_BUF_Set(addressBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    DEB_ASCII_BUF_dns_status(clerkStatusBuf)
    DEB_ASCII_BUF_ObjUID(uidbuf)

#endif

    register dns_status_t status = DNS_NOTSUPPORTED;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >processAddr(%s,{%s,%s},%s)",
        deb_ascii_thread(thBuf,th_p),
        deb_ascii_FullName(ch_nameBuf,tryCH_p->ch_name),
        deb_ascii_ObjUID(uidbuf,tryCH_p->ch_id),
	deb_ascii_Set(addressBuf,address_p)));

     /* Try RPC */
     if (th_p->taRPC) {
       /* Initialize both pointers and reuse buffer if it exists */
       if (!(th_p->taRspMsg_p = th_p->taRspBuf_p))
	 th_p->taRspMsg_p = th_p->taRspBuf_p = 
	   (byte_t *)dns_get_buffer(clerk_comm_bt, NSMAXSIZE);
       
       if (!th_p->taRspMsg_p) 
	 {status = DNS_RESOURCEERROR;}
       else if (NUM_Set(address_p) == 0) {
	   int getout = 0;
	   maximize_rpc_error(th_p, rpc_s_no_bindings, &getout);
	   status = DNS_NOCOMMUNICATION;
       } else {
	   /* Save operation type and UUID for trace */
         PUT8(th_p->taRspMsg_p, th_p->taOperation);
	 COPY_ObjUID(tryCH_p->ch_id, th_p->taRspMsg_p);
	 th_p->taRspMsg_p += sizeof(ObjUID_t);
	 th_p->taRspBufLen = NSMAXSIZE;

         DCE_SVC_DEBUG((
             cds__svc_handle,
             cds_svc_child,
             svc_c_debug6,
             "  processAddr: taRPC=0x%lx",
             th_p->taRPC));

	 status = (*th_p->taRPC)(th_p,address_p,(ObjUID_t *)tryCH_p->ch_id);
	 if (status == DNS_SUCCESS)
	   th_p->taVersion = 3;
       }
     }

    /* If communications were established, process results */

    if (status == DNS_SUCCESS) 
    {
        /*
         * Determine type of message received, and
         * return message information to caller.
         */
        if (th_p->taRspType == MT_Response) 
        {
    	
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug6,
                "  processAddr(%s,{%s,%s},%s) response received",
                deb_ascii_thread(thBuf,th_p),
                deb_ascii_FullName(ch_nameBuf,tryCH_p->ch_name),
                deb_ascii_ObjUID(uidbuf,tryCH_p->ch_id),
                deb_ascii_Set(addressBuf,address_p)));
    
            /***** call to validate_Progress will go here. *****/
    
            /*
             * Now, let's see if we've made any progress.  Progress record will
             * tell us if we need to look at a  different clearinghouse, or if
             * there was an error in the TA.
             */
    	    status = proc_prog(th_p, tryCH_p, address_p, 
                                   (Progress_t *)th_p->taRspMsg_p);
            if (status == DNS_NOTHING_USEFUL) 
	    {
    	        /* 
		 * Replica not helpful, mark it absent and continue 
		 */
                (void)CARemoveReplica(th_p->taProgress.Resolved, 
                                           (ObjUID_t *)tryCH_p->ch_id);
                status = DNS_NOTFND;
	    }
        } /* if th_p->taRspType == RESPONSE_MSG */
        else 
        {
	    /*
	     * Record error condition returned by
	     * clearinghouse.
	     */
	    status = DNS_NAMESERVERERRORS + EXT32(th_p->taRspMsg_p);

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug6,
                "  processAddr(%s,{%s,%s},%s) error returned %d",
                deb_ascii_thread(thBuf,th_p),
                deb_ascii_FullName(ch_nameBuf,tryCH_p->ch_name),
                deb_ascii_ObjUID(uidbuf,tryCH_p->ch_id),
                deb_ascii_Set(addressBuf,address_p),
                status - DNS_NAMESERVERERRORS));
    
            th_p->clerkStatus = status;
    	
	    if ((status == DNS_ACCESSVIOLATION) || (status == DNS_UNTRUSTEDCH))
	    {
		++CAfixed_p->ca_authenticationfailure;
	    }
   
	    /*
	     * If Address was good, we are done 
	     */
 	    switch (status) 
	    {
	        case DNS_ROOTLOST:
                case DNS_CLEARINGHOUSEDOWN:
                case DNS_DATACORRUPTION:
	             status = DNS_NOTFND;
                     break;
                case DNS_UNKNOWNCLEARINGHOUSE:
	             status = DNS_NOCOMMUNICATION;
	             break;
                default:
		     status = DNS_SUCCESS;
            } /* end switch */
	}
	
    } /* End communciations successful */

    else 
	if (status != DNS_NOCOMMUNICATION) 
	{ 
	    /* 
	     * Operation timed out.  Try someone else
	     */
	    th_p->clerkStatus = status;

	    status = DNS_NOCOMMUNICATION;
       } 
       else 
       { /* No. 1 */
           /* 
	    * No connection was made, local status already set up.
	    * For DECnet Phase V, send a message to syslog with info about 
	    * error. For CDS, log an event with just the clearinghouse name.
	    * Then return to caller and try another address 
	    */

	    LOG_ASCII_BUF_FullName(nameBuf)

            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug6,
                "  processAddr(%s,{%s,%s},%s) Unable to contact server",
                deb_ascii_thread(thBuf,th_p),
                deb_ascii_FullName(ch_nameBuf,tryCH_p->ch_name),
                deb_ascii_ObjUID(uidbuf,tryCH_p->ch_id),
                deb_ascii_Set(addressBuf,address_p)));

            dce_svc_printf(CDS_S_CHILD_NOCOMMUNICATION_MSG,
                deb_ascii_FullName(nameBuf,tryCH_p->ch_name),
                (long)th_p->clerkStatus);

        /* Remember the failure reason, so that when we go to look up
         * the address hint, we only do so if the error was:
         *	UNKNOWN_CLEARINGHOUSE, or
         *	DECnet's NoApplication, or
         *	CDS's ept_s_not_registered or rpc_s_endpoint_not_found
         * (in other words, "I see no clearinghouse HERE").
         * Such a lookup is a waste of time if the error was
         * a simple NOCOMMUNICATION ("I can't get THERE").
         */
        tryCH_p->last_failure = th_p->clerkStatus;

    }  /* End status == DNS_NOCOMMUNICATION */    /* end of "else No. 1" */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <processAddr(%s,{%s,%s},%s), status1(%s), status2(%s)",
        deb_ascii_thread(thBuf,th_p),
        deb_ascii_FullName(ch_nameBuf,tryCH_p->ch_name),
        deb_ascii_ObjUID(uidbuf,tryCH_p->ch_id),
        deb_ascii_Set(addressBuf,address_p),
        deb_ascii_dns_status(statusBuf,status),
        deb_ascii_dns_status(clerkStatusBuf,th_p->clerkStatus)));

    return(status);
}

/*
 *	buildProgress
 */
int
buildProgress (thread_t *th_p)
{
  register int	length = 0;


  /*
   * Progress record:
   *	flags
   *	timeout: Timeout
   *	unresolvedName: FullName
   *	resolvedName: FullName
   *	CHList: Set
   */
  if (th_p->taReqMsgEnd_p) {
    INS8(th_p->taReqMsgEnd_p,
	 EXT8(th_p->taProgress.flags) & (PR_linked | PR_up | PR_ignoreState));
    th_p->taReqMsgEnd_p = SKIP_byte(th_p->taReqMsgEnd_p);
    
    COPY_Timeout(th_p->taProgress.Timeout, th_p->taReqMsgEnd_p);
    th_p->taReqMsgEnd_p = SKIP_Timeout(th_p->taReqMsgEnd_p);
    
    COPY_FullName(th_p->taProgress.Unresolved, th_p->taReqMsgEnd_p);
    th_p->taReqMsgEnd_p = SKIP_FullName(th_p->taReqMsgEnd_p);
    
    COPY_FullName(th_p->taProgress.Resolved, th_p->taReqMsgEnd_p);
    th_p->taReqMsgEnd_p = SKIP_FullName(th_p->taReqMsgEnd_p);
    
    COPY_Set(NullSet, th_p->taReqMsgEnd_p);
    th_p->taReqMsgEnd_p = SKIP_Set(th_p->taReqMsgEnd_p);
  }
  
  length += LEN_byte(th_p->taProgress.flags);
  length += LEN_Timeout(th_p->taProgress.Timeout);
  length += LEN_FullName(th_p->taProgress.Unresolved);
  length += LEN_FullName(th_p->taProgress.Resolved);
  length += LEN_Set(NullSet);

  return(length);
}

/*
 *	sendToWalkTree
 */
void 
sendToWalkTree (thread_t  *th_p,
                byte_t    *entry_p)
{
  DEB_ASCII_BUF_thread(thBuf)
  DEB_ASCII_BUF_FullName(entryBuf)


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " >sendToWalkTree(%s,%s)",
      deb_ascii_thread(thBuf,th_p),
      deb_ascii_FullName(entryBuf,entry_p)));
  
  clerk_WalkTree(th_p, (FullName_t *)entry_p);
  
  if ((th_p->clerkStatus == DNS_SUCCESS) && th_p->taParseRsp)
  {
      (*th_p->taParseRsp)(th_p);
  }
  else 
  {
      if ((th_p->clerkStatus == DNS_UNKNOWNENTRY) 
	  && th_p->clerkResLength_p && th_p->taRspMsg_p) 
      {
	  byte_t *fn_p = &th_p->taRspMsg_p[sizeof(exceptionMessage_t)];
	  register byte_t *rsp_p =
	      SKIP_bytes(th_p->clerkRspBuf_p, sizeof(ucp_resp_header));
	  int len = LEN_FullName(fn_p);
    
	  if (th_p->clerkPartialRes && (len <= th_p->clerkResLength)) 
	  {
	      PUTaddress(rsp_p, th_p->clerkPartialRes);
	      PUT16(rsp_p, len);
	      COPY_bytes(fn_p, rsp_p, len);
	      rsp_p = SKIP_bytes(rsp_p, len);
	  }
 
	  if (th_p->clerkResLength_p)
	  {
	      PUTaddress(rsp_p, th_p->clerkResLength_p);
	      PUT16(rsp_p, sizeof(field32));
	      PUT32(rsp_p, len);
	  }
	  th_p->clerkRspMsgEnd_p = rsp_p;
      }
  }
  

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " <sendToWalkTree(%s,%s)",
      deb_ascii_thread(thBuf,th_p),
      deb_ascii_FullName(entryBuf,entry_p)));
}

/*
 * Release storage for list of clearinghouses to try
 * Zero pointer.
 */
void 
rem_reps (CHlist_t **top_pp)
{
  CHlist_t *current_p,*next_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " >rem_reps"));
  
  next_p = *top_pp;
  while (next_p != NULL) {
    current_p = next_p;
    next_p = current_p->ch_next;
    dns_free((char *)current_p);
  }
  *top_pp = NULL;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " <rem_reps"));
}


/*
 * Checks for a loop in a set of softlink transversals
 */
static dns_status_t 
SLLoop_check (thread_t      *th_p,
              struct State  *linkLoop)
{
  DEB_ASCII_BUF_thread(thBuf)

  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug8,
      " >SLLoop_check(%s,)",
      deb_ascii_thread(thBuf,th_p)));
  
  if (++linkLoop->iteration >= 99)
  {
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_child,
          svc_c_debug8,
	  " <SLLoop_check: return(DNS_POSSIBLECYCLE)"));
      
      return(DNS_POSSIBLECYCLE);
  }
  else
  {
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_child,
          svc_c_debug8,
	  " <SLLoop_check: return(DNS_SUCCESS)"));

      return(DNS_SUCCESS);
  }
  
}

/*
 * Update clerk counters
 */
void 
ClerkCounter (int type)
{

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " >ClerkCounter"));
  
  switch (type) 
  {
  case CH_write:
      ++CAfixed_p->ca_writeoperations;
      break;
  case CH_read:
      ++CAfixed_p->ca_readoperations;
      break;
  case CH_miscl:
      ++CAfixed_p->ca_miscoperations;
      break;
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " <ClerkCounter"));
  
}
