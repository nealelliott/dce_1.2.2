/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cagroup.c,v $
 * Revision 1.1.7.2  1996/02/18  19:24:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:05  marty]
 *
 * Revision 1.1.7.1  1995/12/08  00:10:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:34  root]
 * 
 * Revision 1.1.4.5  1994/08/16  17:27:13  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockClient() by CALockData(),
 * 	CAUnlockClient() by CAUnlockData().
 * 	[1994/08/16  15:30:13  peckham]
 * 
 * Revision 1.1.4.4  1994/06/23  18:28:24  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:41:38  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  16:07:07  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:43  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:37:58  peckham
 * 	Remove VMS and UNIX conditionals.
 * 	[1994/04/11  17:38:38  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:55:44  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:47:40  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:36:50  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:19:59  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: cagroup.c
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

#include <cache.h>
#include <stdio.h>

#include <dnsclerk.h>

CA_PRIVATE UserGroupEntry_t *
CA_FindGroup (
  UserEntry_t   *,
  FullName_t    *,
  FullName_t    *,
  int            );

CA_PRIVATE int
CA_GInsert (
  UserEntry_t   *,
  FullName_t    *,
  FullName_t    *,
  int           ,
  Timeout_t     *);

/*
 * Locate a group/member in the collision list of  
 * Names already in uppercase
 * Client lock held by caller.
 */
CA_PRIVATE UserGroupEntry_t *
CA_FindGroup (UserEntry_t  *user_p,
               FullName_t   *groupname_p,
               FullName_t   *member_p,
               int          idx)
{
  DEB_ASCII_BUF_UserEntry(userBuf)
  DEB_ASCII_BUF_FullName(groupnameBuf)
  DEB_ASCII_BUF_FullName(memberBuf)
  register UserGroupEntry_t *group_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CA_FindGroup(%s,%s,%s,%d)",
      deb_ascii_UserEntry(userBuf,user_p),
      deb_ascii_FullName(groupnameBuf,groupname_p),
      deb_ascii_FullName(memberBuf,member_p),
      idx));

  /* Make sure table exists first */
  if (user_p->uh_group) {
    CacheLinkedList_t *const ll_p = CA_GET_LLPOINTER(user_p->uh_group)+idx;
    const int grouplen = LEN_FullName(groupname_p);
    const int memberlen = LEN_FullName(member_p);
    
    /* Search linked list for a match */
    for (group_p = CA_GET_UGPOINTER(ll_p->ll_flink);
	 group_p != (UserGroupEntry_t *)ll_p;
	 group_p = CA_GET_UGPOINTER(group_p->ug_collision.ll_flink))
      /* Compare group names */
      if (grouplen == LEN_FullName(group_p->ug_name))
	if (EQ_bytes(groupname_p, group_p->ug_name, grouplen)) {
	  /* Compare member names */
	  FullName_t *const name_p =
			(FullName_t *)SKIP_FullName(group_p->ug_name);

	  if (memberlen == LEN_FullName(name_p))
	    if (EQ_bytes(member_p, name_p, memberlen))
	      goto leave_CA_FindGroup;
	}
  }
  group_p = NULL;

leave_CA_FindGroup:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CA_FindGroup(%s,%s,%s,%d) return(%p)",
      deb_ascii_UserEntry(userBuf,user_p),
      deb_ascii_FullName(groupnameBuf,groupname_p),
      deb_ascii_FullName(memberBuf,member_p),
      idx,
      group_p));
  return(group_p);
}

/*
 * Add a new group/member to the cache 
 * Client lock held by caller.
 */
CA_PRIVATE int 
CA_GInsert (UserEntry_t  *user_p,
            FullName_t   *groupname_p,
            FullName_t   *member_p,
            int          inout,
            Timeout_t    *timeout_p)
{
  DEB_ASCII_BUF_UserEntry(userBuf)
  DEB_ASCII_BUF_FullName(groupnameBuf)
  DEB_ASCII_BUF_FullName(memberBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  register UserGroupEntry_t *group_p;
  register CacheLinkedList_t *ll_p;
  int status = DNS_SUCCESS;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CA_GInsert(%s,%s,%s,%d,)",
      deb_ascii_UserEntry(userBuf,user_p),
      deb_ascii_FullName(groupnameBuf,groupname_p),
      deb_ascii_FullName(memberBuf,member_p),
      inout));

  /* If table doesn't exist yet, allocate it */
  if (!user_p->uh_group) {
    ll_p = (CacheLinkedList_t *)CA_ClientAlloc(sizeof(CacheLinkedList_t)*USERGROUP_HASH_LEN, Othertype);
    if (ll_p) {
      /* If user is locked down, lock this down too */
      if (CA_locked(user_p)) CA_lock(ll_p);
      INIT_HASH_TABLE(USERGROUP_HASH_LEN, ll_p);
      user_p->uh_group = CA_GET_OFFSET(ll_p);
    } else
      status = DNS_NONSRESOURCES;
  }

  /* If have table now, continue */ 
  if (user_p->uh_group) {
    /* Get hash table index for group/member combination */
    const int idx = CAHashGroup(groupname_p, member_p);
    
    /* See if group/member already cached */
    group_p = CA_FindGroup(user_p, groupname_p, member_p, idx);
    if (group_p) {

      /* Already here, move to front of LRU list */
      CALruMove(group_p);
    
      /* Update results */ 
      group_p->ug_inoutdirect = inout;
      COPY_Timeout(timeout_p, group_p->ug_timeout);
      (void)utc_gettime(&group_p->ug_inCache);
    } else {
      /* Create new entry */
      group_p = (UserGroupEntry_t *)CA_ClientAlloc(sizeof(*group_p)
		- sizeof(group_p->ug_name) + LEN_FullName(groupname_p)
		- sizeof(group_p->ug_member) + LEN_FullName(member_p), UGEntry);
      if (group_p) {
	/* Initialize entry */
	(void)utc_gettime(&group_p->ug_inCache);
	group_p->ug_inoutdirect = inout;
	COPY_Timeout(timeout_p, group_p->ug_timeout);
	COPY_FullName(groupname_p, group_p->ug_name);
	COPY_FullName(member_p, SKIP_FullName(group_p->ug_name));

	/* If user is locked down, lock this down too */
	if (CA_locked(user_p)) CA_lock(group_p);

	/* Add to LRU list */
	CA_LruInsert(group_p);
	
	/* Add to collision list */
	CA_CollisionInsert(CA_GET_LLPOINTER(user_p->uh_group) + idx, 
			  (CacheLinkedList_t *)group_p);
      } else
	status = DNS_NONSRESOURCES;
    }
  }
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CA_GInsert(%s,%s,%s,%d,) return(%s)",
      deb_ascii_UserEntry(userBuf,user_p),
      deb_ascii_FullName(groupnameBuf,groupname_p),
      deb_ascii_FullName(memberBuf,member_p),
      inout,
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/*
 * Loop till we reclaim enough cache space to add user
 */
dns_status_t 
CAInsertGroup (cds_user_t  user_p,
               FullName_t  *groupname_p,
               FullName_t  *member_p,
               int         inout,
               Timeout_t   *timeout_p)
{
  DEB_ASCII_BUF_user(userBuf)
  DEB_ASCII_BUF_FullName(groupnameBuf)
  DEB_ASCII_BUF_FullName(memberBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  UserEntry_t *ue_p;
  Timeout_u newtime;
  FullName_u gname,mname;
  dns_status_t status = DNS_SUCCESS;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CAInsertGroup(%s,%s,%s)",
      deb_ascii_user(userBuf,user_p),
      deb_ascii_FullName(groupnameBuf,groupname_p),
      deb_ascii_FullName(memberBuf,member_p)));

  /* 
   * A utc cannot be all zero's, but DNS uses this to mean a value
   * was not supplied.  Must do byte compares, cause utc routines will
   * reject it.
   */
  if (EQ_Timeout(timeout_p, NullTimeout)) {
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug4,
        "  CAInsertGroup(%s,%s,%s) - null timeout",
        deb_ascii_user(userBuf,user_p),
        deb_ascii_FullName(groupnameBuf,groupname_p),
        deb_ascii_FullName(memberBuf,member_p)));
    goto leave_CAInsertGroup;
  }

  COPY_Timeout(timeout_p, newtime);

  /* If expired, try extending it */
  if (CATimedOut(timeout_p->to_expiration)) {
    CASetExpiration(((Timeout_t *)newtime)->to_extension, 
		    ((Timeout_t *)newtime)->to_expiration);
    if (CATimedOut(((Timeout_t *)newtime)->to_expiration)) {
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_cache,
          svc_c_debug5,
          "  CAInsertGroup(%s,%s,%s) - expired",
          deb_ascii_user(userBuf,user_p),
          deb_ascii_FullName(groupnameBuf,groupname_p),
          deb_ascii_FullName(memberBuf,member_p)));
      goto leave_CAInsertGroup;
    }
  }

  /* X.500 Normalize names */
  if (((status = dns_normalize_fname(groupname_p,
		(FullName_t *)gname, sizeof(FullName_u)) == DNS_SUCCESS)) &&
      ((status = dns_normalize_fname(member_p,
		(FullName_t *)mname, sizeof(FullName_u))) == DNS_SUCCESS)) {
    dns_status_t ca_status;

    /* Get appropriate cache locks */
    CALockData();
    
    /* Get or insert user entry */
    ue_p = CA_InsertUser(user_p, &ca_status);
    if (ue_p) {
      
      /* 
       * Need to guarentee that the user entry is not removed from the
       * cache by a reclaim request.  To do this we remove it from the LRU list
       * so that reclaim cannot find it. 
       */
      CA_LruRemove(ue_p);
      
      /* Compute length of new entry here so it only gets done once */
      
      /* Add group to cache */
      status = CA_GInsert(ue_p, (FullName_t *)gname, (FullName_t *)mname, 
		inout, (Timeout_t *)newtime);
	
      /* Put user entry back on LRU list */
      CA_LruInsert(ue_p);

    } else
      status = ca_status;
    
    /* Release locks */
    CAUnlockData();
  }

leave_CAInsertGroup:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CAInsertGroup(%s,%s,%s) return(%s)",
      deb_ascii_user(userBuf,user_p),
      deb_ascii_FullName(groupnameBuf,groupname_p),
      deb_ascii_FullName(memberBuf,member_p),
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/*
 * Test for group/member membership.
 * Username is null terminated string 
 */
int 
CATestGroup (cds_user_t    user_p,
             unsigned int  seconds,
             FullName_t    *groupname_p,
             FullName_t    *member_p,
             int           direct,
             byte_t        *inout_p,
             Timeout_t     *timeout_p,
             int           *stale_p,
             byte_t        *linked_p)
{
  DEB_ASCII_BUF_user(userBuf)
  DEB_ASCII_BUF_FullName(groupnameBuf)
  DEB_ASCII_BUF_FullName(memberBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  UserEntry_t *ue_p;
  int status = FALSE;
  register UserGroupEntry_t *group_p = NULL;
  FullName_u gname,mname,resolved;

  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CATestGroup(%s,%d,%s,%s,%d,,,,)",
      deb_ascii_user(userBuf,user_p),
      seconds,
      deb_ascii_FullName(groupnameBuf,groupname_p),
      deb_ascii_FullName(memberBuf,member_p),
      direct));

  *stale_p = 0;
  *linked_p = 0;

  {
    int idx;

    CALockData();

    if (CA_ResolveName(user_p, seconds, groupname_p,
			(FullName_t *)resolved, NULL)) {
      *linked_p = 1;
      /* X.500 - upcase not sufficient for typed names */
      if (dns_normalize_fname((FullName_t *)resolved, (FullName_t *)gname, 
			      sizeof(FullName_u)) != DNS_SUCCESS)
	goto unlock_CATestGroup;
    } else {
      /* X.500 - upcase not sufficient for typed names */
      if (dns_normalize_fname(groupname_p, (FullName_t *)gname, 
			      sizeof(FullName_u)) != DNS_SUCCESS)
	goto unlock_CATestGroup;
    }

    if (dns_normalize_fname(member_p,
	  (FullName_t *)mname, sizeof(FullName_u)) != DNS_SUCCESS)
      goto unlock_CATestGroup;
    
    idx = CAHashGroup((FullName_t *)gname, (FullName_t *)mname);

    ue_p = CA_LookupUser(user_p);
    if (ue_p)
      group_p = CA_FindGroup(ue_p, (FullName_t *)gname, 
			    (FullName_t *)mname, idx);

    if (group_p) {
      /* Check if flags are correct, use same flags client would use */
      if ((direct != BL_true) || (group_p->ug_inoutdirect != BL_false)) {
	/* Check if timed out, no extensions done after added to cache */
	if (!CATimedOut(
	     ((Timeout_t *)group_p->ug_timeout)->to_expiration)
	    && CAStale(seconds, &group_p->ug_inCache)) {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_cache,
            svc_c_debug4,
            "  stale member '%s', group '%s'",
            deb_ascii_FullName(memberBuf,member_p),
            deb_ascii_FullName(groupnameBuf,groupname_p)));
	  *stale_p = 1;
	}
	else {
	  /* member valid */
	  *inout_p = group_p->ug_inoutdirect;
	  if (timeout_p)
	    COPY_Timeout(group_p->ug_timeout, timeout_p);

	  /* Move to beginning of LRU list */
	  CALruMove(group_p);	

	  status = TRUE;
	}
      }
    } /* End found group */

unlock_CATestGroup:  
    CAUnlockData();  
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CATestGroup(%s,%d,%s,%s,%d,%d,,%d,%d) return(%s)",
      deb_ascii_user(userBuf,user_p),
      seconds,
      deb_ascii_FullName(groupnameBuf,groupname_p),
      deb_ascii_FullName(memberBuf,member_p),
      direct,
      *inout_p,
      *stale_p,
      linked_p,
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}
