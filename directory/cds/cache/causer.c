/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: causer.c,v $
 * Revision 1.1.9.2  1996/02/18  19:24:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:27  marty]
 *
 * Revision 1.1.9.1  1995/12/08  00:11:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:46  root]
 * 
 * Revision 1.1.6.5  1994/06/23  18:28:37  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:46:44  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  16:07:21  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:56  devsrc]
 * 
 * Revision 1.1.6.3  1994/06/08  20:18:54  peckham
 * 	Handle NULL user_p when DCE_SEC is off.
 * 	[1994/06/06  19:24:46  peckham]
 * 
 * Revision 1.1.6.2  1994/04/14  14:38:12  peckham
 * 	Remove DNS_LOCAL_NS conditionals.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Use CA_snapshot_cache() instead of CA_abort().
 * 	[1994/04/11  18:11:00  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:56:07  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:49:12  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:37:44  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:21:20  zeliff]
 * 
 * Revision 1.1.2.4  1992/07/22  19:59:16  peckham
 * 	OT#4478 followup - bad merge - CA_FindUser() comparing user_p
 * 	instead of name_p within loop.  As a result no matches, and
 * 	you get many duplicate UserEntrys in the cache bumping out
 * 	everything else.
 * 	[1992/07/22  19:11:20  peckham]
 * 
 * Revision 1.1.2.3  1992/06/30  19:58:39  peckham
 * 	OT#4478: Users don't match after the first one.
 * 	Potential for clerk crashes.
 * 	[1992/06/30  19:58:03  peckham]
 * 
 * Revision 1.1.2.2  1992/03/22  21:17:26  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:06:39  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module causer.c
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

/*
 * No user on cds security is a null short.
 */
#ifdef DCE_SEC
  const field16 null__user[1] = {0};
# define NO_USER ((cds_user_t)null__user)
#else
# define NO_USER ((cds_user_t)NullSimpleName)
#endif

CA_PRIVATE UserEntry_t *
CA_FindUser (
  cds_user_t ,
  int        );


/*
 * Locate a user in the collison list of users 
 * Client lock held by caller.
 */
CA_PRIVATE UserEntry_t *
CA_FindUser (cds_user_t  user_p,
              int         idx)
{
  DEB_ASCII_BUF_user(userBuf)
  CacheLinkedList_t *const ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_user)+idx;
  register UserEntry_t *ue_p;
#ifdef DCE_SEC
  byte_t *const name_p = SKIP_bytes(user_p, sizeof(field16));
  const int length = EXT16(user_p);
#endif

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CA_FindUser(%s,%d)",
      deb_ascii_user(userBuf,user_p),
      idx));
  
  /* Search linked list for a match */
  for (ue_p = CA_GET_UHPOINTER(ll_p->ll_flink);
       ue_p != (UserEntry_t *)ll_p;
       ue_p = CA_GET_UHPOINTER(ue_p->uh_collision.ll_flink)) 
  {
#ifdef DCE_SEC
    /* Make sure lengths ar equal first.  
     * This will match the 0 length user also
     */
    if (length == EXT16(ue_p->uh_profile))
	if (EQ_bytes(name_p, 
		     SKIP_bytes(ue_p->uh_profile, sizeof(field16)), 
		     length))
#else
    if (EQ_SimpleName(user_p, ue_p->uh_sname))
#endif
      goto leave_CA_FindUser;
  }
  ue_p = NULL;

leave_CA_FindUser:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CA_FindUser(%s,%d) return(%p)",
      deb_ascii_user(userBuf,user_p),
      idx,
      ue_p));
  return(ue_p);
}


/*
 * Add a new user to the cache. 
 * Client lock held by caller.
 */
UserEntry_t *
CA_InsertUser (cds_user_t    user_p,
                dns_status_t  *status_p)
{
  DEB_ASCII_BUF_user(userBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  int idx;
  UserEntry_t *ue_p;
  dns_status_t status = DNS_CLERKBUG;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug5,
      " >CA_InsertUser(%s,%p)",
      deb_ascii_user(userBuf,user_p),
      status_p));

#ifdef CACHE_PARANOID
  if (!CA_ClientLocked()) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_CLOCK_A_MSG);
  }
#endif

  if (!user_p)			/* unauthenticated access */
      user_p = NO_USER;

  /* Get hash table index */
  idx = CAHashUser(user_p);

  /* See if user already exists */
  ue_p = CA_FindUser(user_p, idx);
  if (ue_p) 
  {
    /* Already here, move to front of LRU list */
    CALruMove(ue_p);
  } 
  else 
  {
    ue_p = (UserEntry_t *)CA_ClientAlloc(sizeof(*ue_p)
#ifdef DCE_SEC
			- sizeof(ue_p->uh_profile) + EXT16(user_p)
#else
			- sizeof(ue_p->uh_sname) + LEN_SimpleName(user_p)
#endif
			, UHEntry);
    if (ue_p) 
    {
      /* Initialize entry */
      ue_p->uh_access = 0;
      ue_p->uh_group = 0;
#ifdef DCE_SEC
 /* save principal's opaque login context */
      COPY_bytes (user_p, ue_p->uh_profile, EXT16(user_p)+sizeof(field16));
#else 
      COPY_SimpleName(user_p, ue_p->uh_sname); /* save principals local name*/
#endif

      /* Add to LRU list */
      CA_LruInsert(ue_p);
      
      /* Add to collision list */
      CA_CollisionInsert(CA_GET_LLPOINTER(CAfixed_p->ca_user) + idx, 
			(CacheLinkedList_t *)ue_p);
    } else
      status = DNS_NONSRESOURCES;
  }

#ifdef CACHE_PARANOID
leave_CA_InsertUser:
#endif
  if (status_p)
    *status_p = status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug5,
      " <CA_InsertUser(%s,%p->%s) return(%p)",
      deb_ascii_user(userBuf,user_p),
      status_p,
      deb_ascii_dns_status(statusBuf,status),
      ue_p));
  return(ue_p);
}

/*
 * Lookup a user in the cache.
 * If using DCE Security, the user_p is a secure login context
 * If NOT using DCE Security, the user_p is null terminated string.
 * This routine is only called from within the cache, not by the clerk itself.
 * Client lock held by caller.
 */
UserEntry_t *
CA_LookupUser (cds_user_t user_p)
{
  DEB_ASCII_BUF_user(userBuf)
  UserEntry_t *ue_p;
  int idx;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug5,
      " >CA_LookupUser(%s)",
      deb_ascii_user(userBuf,user_p)));

  if (!user_p)			/* unuauthenticated access */
      user_p = NO_USER;

  /* If not using DCE security:
   * User name should be whatever the local operating system specifies.
   * Case is irrelevant as long as the caller doesn't alter it.
   * Unlike other names, username is not entered through a human interface
   * so we don't have to worry about variations.
   */
  idx = CAHashUser(user_p);
  
  ue_p = CA_FindUser(user_p, idx); /* check collision list */
  if (ue_p) 
  {
    /* Move to front of LRU list */
    CALruMove(ue_p);	
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug5,
      " <CA_LookupUser(%s) return(%p)",
      deb_ascii_user(userBuf,user_p),
      ue_p));
  return(ue_p);
}
