/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: caaccess.c,v $
 * Revision 1.1.7.2  1996/02/18  19:23:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:54  marty]
 *
 * Revision 1.1.7.1  1995/12/08  00:10:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:23  root]
 * 
 * Revision 1.1.4.4  1994/06/23  18:28:08  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:13:23  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  16:06:57  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:31  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:37:47  peckham
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Use CA_snapshot_cache() instead of CA_abort().
 * 	[1994/04/11  17:29:30  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:55:22  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:46:24  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:36:04  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:18:55  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:23:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module caaccess.c
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

CA_PRIVATE UserAccessEntry_t *
CA_FindAccess (
     UserEntry_t *,
     int          ,
     token_t     *);


/*
 * Search linked list of hash table entries for a match on access token.
 * Client lock held by caller.
 */
CA_PRIVATE UserAccessEntry_t *
CA_FindAccess (UserEntry_t  *ue_p,
                int          idx,
                token_t      *token_p)
{
  DEB_ASCII_BUF_UserEntry(ueBuf)
  DEB_ASCII_BUF_token(tokenBuf)
  register UserAccessEntry_t *access_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " >CA_FindAccess(%s,%d,%s)",
      deb_ascii_UserEntry(ueBuf,ue_p),
      idx,
      deb_ascii_token(tokenBuf,token_p)));

  /* Make sure table exists */
  if (ue_p->uh_access) 
  {
    const CacheLinkedList_t *const ll_p = CA_GET_LLPOINTER(ue_p->uh_access)+idx;

    /* Search linked list for a match */
    for (access_p = CA_GET_UAPOINTER(ll_p->ll_flink);
	 access_p != (UserAccessEntry_t *)ll_p;
	 access_p = CA_GET_UAPOINTER(access_p->ua_collision.ll_flink))
      /* Compare tokens */
      if (EQ_token(access_p->ua_token, token_p))
	goto leave_CA_FindAccess;
  }
  access_p = NULL;

leave_CA_FindAccess:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " <CA_FindAccess(%s,%d,%s) return(%p)",
      deb_ascii_UserEntry(ueBuf,ue_p),
      idx,
      deb_ascii_token(tokenBuf,token_p),
      access_p));
  return(access_p);

}

/*
 * Check if user has access to a name.  This is done through a token
 * which was assigned to the fullname/type when it was added to
 * the namehash.  All locks already held.  
 * If the token is found, the AccessEntry is moved to the front
 * of the LRU list.
 * Client lock held by caller.
 */
int 
CA_LookupAccess (cds_user_t user_p,
                 token_t    *token_p)
{
  DEB_ASCII_BUF_user(userBuf)
  DEB_ASCII_BUF_token(tokenBuf)
  UserEntry_t *ue_p;
  int status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " >CA_LookupAccess(%s,%s)",
      deb_ascii_user(userBuf,user_p),
      deb_ascii_token(tokenBuf,token_p)));

  /* Find user entry, if not there, client should query server */
  ue_p = CA_LookupUser(user_p);
  if (ue_p) 
  {
    /* Get index into hash table */
    const int idx = CAHashAccess(token_p);

    /* Find token, if not there, client queries server */
    UserAccessEntry_t *const access_p = CA_FindAccess(ue_p, idx, token_p);

    if (access_p) 
    {
      /* Move to front of LRU list */
      CALruMove(access_p);	
      status = TRUE;
    } else
      status = FALSE;
  } else
    status = FALSE;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " <CA_LookupAccess(%s,%s) return(%d)",
      deb_ascii_user(userBuf,user_p),
      deb_ascii_token(tokenBuf,token_p),
      status));
  return(status);      
}

/*
 * Add an access token to the userhash.  
 * Client Lock held by caller.
 */
dns_status_t 
CA_InsertAccess (cds_user_t   user_p,
                 token_t      *token_p)
{
  DEB_ASCII_BUF_user(userBuf)
  DEB_ASCII_BUF_token(tokenBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  UserEntry_t *ue_p;
  dns_status_t status = DNS_SUCCESS, ca_status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " >CA_InsertAccess(%s,%s)",
      deb_ascii_user(userBuf,user_p),
      deb_ascii_token(tokenBuf,token_p)));

#ifdef CACHE_PARANOID
  if (!CA_ClientLocked()) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CLOCK_A_MSG);
  }
#endif

  /* Add or find user in cache first */
  ue_p = CA_InsertUser(user_p, &ca_status);
  if (ue_p) 
  {
    /* 
     * Need to guarentee that the user entry is not removed from the
     * cache by a reclaim request.  To do this we remove it from the LRU list
     * so that reclaim cannot find it. 
     */
    CA_LruRemove(ue_p);
    
    /* If table doesn't exist yet, allocate it */
    if (ue_p->uh_access == 0) 
    {
      CacheLinkedList_t *const ll_p = (CacheLinkedList_t *)
	CA_ClientAlloc(sizeof(CacheLinkedList_t)*USERACCESS_HASH_LEN, Othertype);

      if (ll_p) 
      {
	INIT_HASH_TABLE(USERACCESS_HASH_LEN, ll_p);
	/*
	 * Accesses to a locked user are forever...
	 */
	if (CA_locked(ue_p)) CA_lock(ll_p);

	ue_p->uh_access = CA_GET_OFFSET(ll_p);
      } else
	status = DNS_NONSRESOURCES;
    }

    if (ue_p->uh_access) 
    {
      /* Get hash index once */
      const int idx = CAHashAccess(token_p);

      /* See if token already cached */
      register UserAccessEntry_t *access_p = CA_FindAccess(ue_p, idx, token_p);

      if (access_p) 
      {
	/* Already here, move to front of LRU list */
	CALruMove(access_p);
      } 
      else 
      {
	/* Create new entry */
	access_p = (UserAccessEntry_t *)CA_ClientAlloc(sizeof(UserAccessEntry_t), UAEntry);
	if (access_p) 
	{
      
	  /* 
	   * Initialize entry.  The access entry consists of a token in
	   * the form of a times.  This token is used inplace of
	   * the fullname and type.  
	   */
	  COPY_token(token_p, access_p->ua_token);

	  /*
	   * Accesses to a locked user are forever...
	   */
	  if (CA_locked(ue_p)) CA_lock(access_p);

	  /* Add to front of LRU list */
	  CA_LruInsert(access_p);
	  
	  /* Add to collision list */
	  CA_CollisionInsert(CA_GET_LLPOINTER(ue_p->uh_access) + idx,
			    (CacheLinkedList_t *)access_p);
	} else
	  status = DNS_NONSRESOURCES;

      } /* End Create new entry */
    } /* End have table */

    /* Add user back to LRU list */  
    CA_LruInsert(ue_p);
  } else
    status = ca_status;

#ifdef CACHE_PARANOID
leave_CA_InsertAccess:
#endif
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " <CA_InsertAccess(%s,%s) return(%s)",
      deb_ascii_user(userBuf,user_p),
      deb_ascii_token(tokenBuf,token_p),
      deb_ascii_dns_status(statusBuf,status)));

  return(status);
}
