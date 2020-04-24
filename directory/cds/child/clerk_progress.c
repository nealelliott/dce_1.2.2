/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_progress.c,v $
 * Revision 1.1.6.2  1996/02/18  19:25:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:08  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:04:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:04  root]
 * 
 * Revision 1.1.4.4  1994/08/03  18:59:28  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:13:44  mccann]
 * 
 * Revision 1.1.4.3  1994/06/23  18:28:52  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:54:25  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  16:07:41  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:23  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:56:25  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:51:11  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:41:07  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:26:48  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:27:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: clerk_progress.c
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
 *	These routines are concerned with the creation of a progress record
 *	in the message which is sent to the TA on the nameserver.
 * 
 * Networks & Communications Software Engineering
 * 
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_clerk.h>
#include <uc_child.h>
#include <cache.h>

/* local prototypes */

static int
check_for_dead (thread_t *,		 
		FullName_t *,
		Set_t *);

static Set_t *
buildrpset (FullName_t *,
	   ObjUID_t *,
	   Set_t *);

static void
proc_done (thread_t *,
	   Progress_t *,
	   Set_t *,
	   CHlist_t *,
	   Set_t *);

static void
proc_up (thread_t *,
	 Progress_t *,
	 Set_t *);

static void
proc_chlist (thread_t *,
	    Set_t *,
	    FullName_t *,
	    ObjUID_t *,
	    CHlist_t    *,
	    Set_t  *);


/* 
 * Build a replica set with one entry 
 */
static Set_t 
*buildrpset (FullName_t  *name_p,
             ObjUID_t    *id_p,
             Set_t       *address_p)
{
  Set_t *set_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " >buildrpset()"));

  set_p = (Set_t *)dns_malloc(sizeof(Set_u) + sizeof(MemberValue_u) + 
		 sizeof(ReplicaPointer_u));
  if (set_p != NULL) 
  {
    MemberValue_t *mv_p;
    ReplicaPointer_t *rp_p;
    ReplicaType_t *rt_p;
    int rplen = sizeof(ReplicaPointer_overhead) + LEN_FullName(name_p)
		  + LEN_Set(address_p);

    /* Initialize set header */
    SET_FLAG(set_p->st_flag, ST_present);
    INS16(set_p->st_offsetLength, sizeof(word_u));
    INS16(set_p->st_valueLength, sizeof(MemberValue_overhead) + rplen);

    /* Fill in Setmember header, timestamp left as garbage */
    mv_p = (MemberValue_t *)DATA_Set(set_p);
    INS16(set_p->st_offsets, (byte_t *)mv_p - (byte_t *)set_p);

    SET_FLAG(mv_p->mv_flag, MH_present);
    INS16(mv_p->mv_length, rplen);
    INS8(mv_p->mv_valuetype, VT_ReplicaPointer);
    /* Build replica */
    rp_p = (ReplicaPointer_t *)mv_p->mv_value;
    COPY_ObjUID(id_p, rp_p->rp_CHID);
    COPY_Set(address_p, rp_p->rp_addressHint);

    rt_p = (ReplicaType_t *)(rp_p->rp_addressHint + LEN_Set(address_p));
    /* This is just a guess */
    INS8(rt_p->rt_value, RT_master);
    COPY_FullName(name_p, SKIP_ReplicaType(rt_p));
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " <buildrpset() return(set_p=%p)",
      set_p));

  return(set_p);
}

/*
 * Cache the softlink and 
 * Set up taProgress structure for next pass through walktree.
 */
void 
proc_link (thread_t    *th_p,
           Progress_t  *progress_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >proc_link()"));

    /* Save new timeout value for link */
    COPY_Timeout(progress_p->pr_timeout, th_p->taProgress.Timeout);

    /* Prepare for continueing treewalk */
    COPY_FullName(progress_p->pr_unresolvedName, th_p->taProgress.Unresolved);
    COPY_FullName(SKIP_FullName(progress_p->pr_unresolvedName), 
				th_p->taProgress.Resolved);
    th_p->outLinked++;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <proc_link()"));
}

/*
 * Add chlist data to cache.
 */
static void 
proc_chlist (thread_t    *th_p,
             Set_t       *set_p,
             FullName_t  *dir_p,
             ObjUID_t    *id_p,
             CHlist_t    *tryCH_p,
             Set_t       *address_p)
{  
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >proc_chlist()"));
      
    if (FLAG_SET(set_p->st_flag, ST_present)) 
    {
    /* 
     * If the directory is new or the set has changed, we will
     * traverse the replica set and add each clearinghouse to the cache.
     * otherwise, we assume it has already been done.
     * Update global directory cache with replica set returned
     * from TA. If the directory is new or the replica set has
     * changed, the clearinghouses are added to the clearinghouse
     * cache by insertdirectory.
     */
      CAInsertDirectory(dir_p, id_p, set_p);

    } /* End set present */
    else 
    {
      /* 
       * If we did not use the cache to get this far, add this data to the
       * cache.  
       */
      if (th_p->used_cached_replica == FALSE) 
      {
	set_p = buildrpset((FullName_t *)tryCH_p->ch_name, 
			   (ObjUID_t *)tryCH_p->ch_id, address_p);
	CAInsertDirectory(dir_p, id_p, set_p);
	dns_free((char *)set_p);
      }
    }
 
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <proc_chlist()"));
}

/*
 * The treewalk is done, we found what we wanted.
 * Add directory data to cache.
 */
static void 
proc_done (thread_t    *th_p,
           Progress_t  *progress_p,
           Set_t       *set_p,
           CHlist_t    *tryCH_p,
           Set_t       *address_p)
{
    register FullName_t *pr_unresolved_p, *pr_resolved_p;
    FullName_u stripname;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >proc_done()"));

    /* Get pointers into returned progress record */
    pr_unresolved_p = (FullName_t *)progress_p->pr_unresolvedName;
    pr_resolved_p = (FullName_t *)SKIP_FullName(pr_unresolved_p);

    /* 
     * On a resolve name request, we don't know if the result is a directory
     * or not.  Cannot cache anything.  
     */
    if (th_p->taOperation != OP_ResolveName)  	
    {
      /* 
       * Make sure the resolved name is a directory name.  This would not
       * be the case if we are done and the operation was on an object,
       * link or child pointer.  In these cases the set reflects the
       * parent directory.
       */
      if (FLAG_IS_CLEAR(progress_p->pr_flags, PR_directory))
      {
	  /* Don't corrupt response buffer, use intermediate */
	  COPY_FullName(pr_resolved_p, stripname);
	  childStripRSimple((FullName_t *)stripname);
	  pr_resolved_p = (FullName_t *)stripname;
      }
      /* add set to cache */
      proc_chlist(th_p, set_p, pr_resolved_p, 
		  (ObjUID_t *)pr_unresolved_p->fn_root, 
		  tryCH_p, address_p);
    }
    th_p->clerkStatus = DNS_SUCCESS;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <proc_done()"));
}

/*
 * The up flag is set which means we still haven't gotten
 * anywhere that could help us.  We are now searching for
 * the root.  Since not progress has been made
 * taProgress has not changed.
 */
static void 
proc_up (thread_t    *th_p,
         Progress_t  *progress_p,
         Set_t       *set_p)
{
    CHlist_t *newlist_p,*last_p;

    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >proc_up()"));

    /* Cache list of clearinghouses to try */
    if (FLAG_SET(set_p->st_flag, ST_present)) 
    {
	CAClearinghouseSet(set_p);
    }

    /*
     * Since we were just rambling around looking for
     * the root, add this clearinghouse replica set to 
     * to our list of possible replicas.
     */
    newlist_p = BuildCHlist(set_p, th_p);
    if (newlist_p != NULL) 
    {
	/* merge new list to front of existing list */
	if (th_p->taProgress.Replica != NULL) 
	{
	    /* find last element in list */
	    last_p = newlist_p;
	    while (last_p->ch_next != NULL)
		last_p = last_p->ch_next;
	    last_p->ch_next = th_p->taProgress.Replica;
	    th_p->taProgress.Replica->ch_prev = last_p;
	}
	th_p->taProgress.Replica = newlist_p;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <proc_up()"));
}

/*
 *		p r o c _ p r o g 
 *
 * This routine checks the returned progress record, caches the information
 * from the clearinghouse replica set and determines if another nameserver
 * needs to be contacted.
 *
 * Returns:
 *	DNS_SUCCESS	= All done or hitlink
 *      DNS_NOTFND = Continue processing in walktree
 */
int
proc_prog (thread_t    *th_p,
           CHlist_t    *tryCH_p,
           Set_t       *address_p,
           Progress_t  *progress_p)
{
    FullName_t *pr_unresolved_p, *pr_resolved_p;
    Set_t *set_p;
    int	made_progress = TRUE;
    int status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        " >proc_prog()"));
	
    INS8(th_p->taProgress.flags, EXT8(progress_p->pr_flags));

    /* Set up pointers into new data */
    pr_unresolved_p = (FullName_t *)progress_p->pr_unresolvedName;
    pr_resolved_p = (FullName_t *)SKIP_FullName(pr_unresolved_p);
    set_p = (Set_t *)SKIP_FullName(pr_resolved_p);

    /* If we are done, update cache and get out of here. */
    if (FLAG_SET(progress_p->pr_flags, PR_done)) {
      proc_done(th_p, progress_p, set_p, tryCH_p, address_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_child,
          svc_c_debug5,
	  " <proc_prog(): (1) return(DNS_SUCCESS)"));

      return(DNS_SUCCESS);
    }

    /* 
     * If hitlink, the chlist is empty, nothing left to do 
     * but cache the link and start over in walktree with the
     * new name.
     */
    if (FLAG_SET(progress_p->pr_flags, PR_hitLink)) {
      proc_link(th_p, progress_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_child,
          svc_c_debug5,
	  " <proc_prog(): (2) return(DNS_SUCCESS)"));

      return(DNS_SUCCESS);
    }

    if (FLAG_SET(progress_p->pr_flags, PR_up) && 
	(ObjUID_is_nil(pr_resolved_p->fn_root))) 
    {
	proc_up(th_p, progress_p, set_p);

	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_child,
          svc_c_debug5,
	  " <proc_prog(): (1) return(DNS_NOTFND)"));

	return(DNS_NOTFND);
    }
		 
    /* 
     * Compare oldresolved name with new to see
     * we got anywhere.  Then set up the new resolved and unresolved
     * names.  
     */
    if (LEN_FullName(pr_resolved_p) == LEN_FullName(th_p->taProgress.Resolved))
	if (EQ_bytes(pr_resolved_p, th_p->taProgress.Resolved,
		     LEN_FullName(pr_resolved_p)))
	    made_progress = FALSE;
    
    /* Prepare for continueing treewalk */
    COPY_FullName(pr_resolved_p, th_p->taProgress.Resolved);
    COPY_FullName(pr_unresolved_p, th_p->taProgress.Unresolved);
    proc_chlist(th_p, set_p, pr_resolved_p, 
		(ObjUID_t *)pr_unresolved_p->fn_root, tryCH_p, address_p);
    /*
     * Build a new list of clearinghouses to try based on information
     * returned by the TA.  This will be the new list for choosech
     */
    if (made_progress == TRUE) 	
    {
	/* Release current list of replicas */
	rem_reps(&th_p->taProgress.Replica);
      
	/* Build a new one */
	th_p->taProgress.Replica = BuildCHlist(set_p, th_p);

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug5,
            " <proc_prog() (2) return(DNS_NOTFND)"));
  
	return(DNS_NOTFND);
    }

    /*
     * Check to make sure we're not just rollin'
     * around lookin' for a name which is dead.
     */
    status = check_for_dead(th_p, th_p->taProgress.Resolved, address_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        " <proc_prog() return(status=%d)",
        status));

    return(status);
}

/*
 *        c h e c k _ f o r _ d e a d
 *
 *
 * Keep track of full names which have been unproductive. If hints are received
 * twice in a row for the same name, then we are more than likely looping
 * through the tree looking for an entry which is dead but not yet buried.
 *
 * Inputs:
 *    name = Pointer to full name which is probably not being resolved
 *    addr = address which has been tried
 *
 * RETURNS:
 *    DNS_SUCCESS if a loop has been detected, thread status set to nocomm.
 *    DNS_NOTHING_USEFUL otherwise
 */

static int 
check_for_dead (thread_t    *th_p,
                FullName_t  *name_p,
                Set_t       *tower_p)
{
  byte_t *dead_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug8,
      " >check_for_dead"));

  if (th_p->dead.cnt != 0) {
    if (dnsCmpFull((dns_opq_fname *)name_p,
		    (dns_opq_fname *)th_p->dead.name_p) == 0) {
      int dead_num_add = th_p->dead.cnt;
      byte_t *dead_addr_p = SKIP_FullName(th_p->dead.name_p);

      do
	/* See if current tower matches one we've been to already */
	if (LEN_Set(dead_addr_p) == LEN_Set(tower_p))
	  if (memcmp(dead_addr_p, tower_p, LEN_Set(tower_p)) == 0) {
	  /*
	   * We've looked at this name before
	   * without any luck.  We can therefore
	   * assume that the name is dead but not
	   * yet buried, and weez in a loop.
	   */

	  dns_free((char *)th_p->dead.name_p);
	  th_p->dead.cnt = 0;
          th_p->clerkStatus = DNS_NOCOMMUNICATION;
  
	  DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_child,
              svc_c_debug8,
              " <check_for_dead: (1) return(DNS_SUCCESS)"));

	  return(DNS_SUCCESS);
      }
      while (dead_addr_p = SKIP_Set(dead_addr_p), --dead_num_add);

      /*
       * This address was not in the dead name's address list.
       * Add it to the list.
       */
      dead_p = (byte_t *)dns_malloc(th_p->dead.len + LEN_Set(tower_p));
      if (dead_p != NULL) {
	COPY_bytes(th_p->dead.name_p, dead_p, th_p->dead.len);
	COPY_Set(tower_p, SKIP_bytes(dead_p, th_p->dead.len));
	dns_free((char *)th_p->dead.name_p);
	th_p->dead.name_p = dead_p;
	th_p->dead.len += LEN_Set(tower_p);
	th_p->dead.cnt++;
      }

      DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_child,
         svc_c_debug8,
         " <check_for_dead: return(DNS_NOTHING_USEFUL)"));

      return(DNS_NOTHING_USEFUL);
    } 
    else 
    {
      /*
       * A different name.  Remove old dead name.
       */
      dns_free((char *)th_p->dead.name_p);
      th_p->dead.cnt = 0;
    }
  }


  /*
   * Get space for new dead name.
   */
  th_p->dead.len = LEN_FullName(name_p) + LEN_Set(tower_p);
  th_p->dead.name_p = (byte_t *)dns_malloc(th_p->dead.len);

  if (th_p->dead.name_p) {
    /*
     * Copy the new dead name into the buffer, and append the
     * address to the end of it.
     */
    COPY_FullName(name_p, th_p->dead.name_p);
    th_p->dead.cnt = 1;
    COPY_Set(tower_p, SKIP_FullName(th_p->dead.name_p));
  }
  else
    th_p->dead.cnt = 0;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug8,
      " <check_for_dead: at end: return(DNS_NOTHING_USEFUL)"));

  return(DNS_NOTHING_USEFUL);
}
