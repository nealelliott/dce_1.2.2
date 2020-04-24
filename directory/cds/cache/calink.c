/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: calink.c,v $
 * Revision 1.1.10.2  1996/02/18  19:24:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:09  marty]
 *
 * Revision 1.1.10.1  1995/12/08  00:10:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:37  root]
 * 
 * Revision 1.1.7.5  1994/08/16  17:27:15  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockClient() by CALockData(),
 * 	CAUnlockClient() by CAUnlockData(),
 * 	CA_ClientLocked() by CA_DataLocked().
 * 	[1994/08/16  15:30:25  peckham]
 * 
 * Revision 1.1.7.4  1994/06/23  18:28:27  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:44:27  mccann]
 * 
 * Revision 1.1.7.3  1994/06/09  16:07:10  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:46  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/14  14:38:01  peckham
 * 	Remove VMS and UNIX conditionals.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Use CA_snapshot_cache() instead of CA_abort().
 * 	[1994/04/11  17:42:05  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  21:55:52  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:48:03  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  12:37:04  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:20:17  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  18:28:08  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:09:12  weisman]
 * 
 * Revision 1.1.2.2  1992/01/22  23:01:10  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:15:34  melman]
 * 
 * $EndLog$
 */
/*
 * Module: calink.c
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

CA_PRIVATE LinkEntry_t *
CA_FindLink (
  FullName_t *,
  int         );


/*
 * Locate a softlink in the collision list of  
 * Names already in uppercase
 * Client lock held by caller.
 */
CA_PRIVATE LinkEntry_t *
CA_FindLink (FullName_t  *linkname_p,
              int         idx)
{
  DEB_ASCII_BUF_FullName(linknameBuf)
  CacheLinkedList_t *const ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_link)+idx;
  const int linklen = LEN_FullName(linkname_p);
  register LinkEntry_t *link_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CA_FindLink(%s,%d)",
      deb_ascii_FullName(linknameBuf,linkname_p),
      idx));

  for (link_p = CA_GET_LHPOINTER(ll_p->ll_flink);
       link_p != (LinkEntry_t *)ll_p;
       link_p = CA_GET_LHPOINTER(link_p->lh_collision.ll_flink))
    /* Compare softlink names */
    if (linklen == LEN_FullName(link_p->lh_name))
      if (EQ_bytes(linkname_p, link_p->lh_name, linklen))
	goto leave_CA_FindLink;

  link_p = NULL;

leave_CA_FindLink:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CA_FindLink(%s,%d) return(%p)",
      deb_ascii_FullName(linknameBuf,linkname_p),
      idx,
      link_p));
  return(link_p);
}

/*
 * Add link to cache.  Add access entry for user.
 */
dns_status_t 
CAInsertLink (cds_user_t  user_p,
              FullName_t  *linkname_p,
              FullName_t  *target_p,
              Timeout_t   *timeout_p)
{
  DEB_ASCII_BUF_user(userBuf)
  DEB_ASCII_BUF_FullName(linknameBuf)
  DEB_ASCII_BUF_FullName(targetBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  register LinkEntry_t *link_p = NULL, *cache_p;
  int len,idx;
  FullName_u lname;
  utc_t newtime;
  dns_status_t status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CAInsertLink(%s,%s,%s,)",
      deb_ascii_user(userBuf,user_p),
      deb_ascii_FullName(linknameBuf,linkname_p),
      deb_ascii_FullName(targetBuf,target_p)));

  /* 
   * A utc cannot be all zero's, but this is used to signify a field is 
   * missing.  UTC routines will not accept all zero's, do byte compares
   */
  if (NE_Timeout(NullTimeout, timeout_p)) 
  {
    COPY_utc(timeout_p->to_expiration, &newtime);
    if (EQ_bytes(Nullutc, &newtime, LEN_utc(Nullutc))) 
    {

      (void)utc_gettime(&newtime);
    }

    /* If already expired with no extension, don't add to cache */
    if (CATimedOut(&newtime)) 
    {
      if (EQ_bytes(Nullutc, timeout_p->to_extension, LEN_utc(Nullutc)))  
      {
	status = DNS_SUCCESS;
	goto leave_CAInsertLink;
      } else
	CASetExpiration(timeout_p->to_extension,&newtime);
    }
  } else
    COPY_utc(Nullutc, &newtime);

  /* X.500 Normalize full name */

  if ((status = dns_normalize_fname(linkname_p,
		(FullName_t *)lname, sizeof(FullName_u))) == DNS_SUCCESS) 
  {
    /* Get hash table index for softlink */
    idx = CAHashLink((FullName_t *)lname);
    
    /* Get appropriate cache locks */
    CALockData();
    
    /* Compute length of new entry here so it only gets done once */
    len = sizeof(*link_p)
		- sizeof(link_p->lh_name) + LEN_FullName(lname)
		- sizeof(link_p->lh_target) + LEN_FullName(target_p);

    /* See if link already exists */
    cache_p = CA_FindLink((FullName_t *)lname, idx);
    if (cache_p) 
    {
      /* See if target name is the same */
      FullName_t *const name_p = (FullName_t *)SKIP_FullName(cache_p->lh_name);

      /* This routine handles case insensitivity */
      if (dnsCmpFull((dns_opq_fname *)name_p, 
		     (dns_opq_fname *)target_p) == 0) 
      {
	token_u theToken;
	link_p = cache_p;
	    
	/* Just update existing entry */
	COPY_utc(&newtime, cache_p->lh_time);

	(void)utc_gettime(&cache_p->lh_inCache);

	/* Already here, move to front of LRU list, do this before
         * insert has the chance to swap it out.  SAve token just
         * in case
         */
	CALruMove(cache_p);
	COPY_token(cache_p->lh_token, theToken);
	/*
	 * Add access token to user hash 
	 */
	status = CA_InsertAccess(user_p, (token_t *)theToken);

      } 
      else 
      {
	/* 
	 * Target names not the same, 
	 * delete cached entry so we can create new one 
	 */
	CA_LruRemove(cache_p);
      }
    }
      
      /* Create new entry */
    if (!link_p) 
    {
      link_p = (LinkEntry_t *)CA_ClientAlloc(len, LHEntry);
      if (link_p) 
      {
	token_u theToken;

	/* Initialize entry */
	(void)utc_gettime(&link_p->lh_inCache);
	COPY_utc(&newtime, link_p->lh_time);
	COPY_FullName(lname, link_p->lh_name);
	COPY_FullName(target_p, SKIP_FullName(link_p->lh_name));
	/* 
	 * Generate local cts to represent this name.  It will be stored in
	 * the attribute hash and user Access hash in place of the name/type
	 * The index into the Namehash table is also stored so it can
	 * be returned to the attribute Hash and Access hash.
	 */
	MAKE_token(theToken);
	COPY_token(theToken, link_p->lh_token);

	if (cache_p) 
	{
	  CA_copy_lock(cache_p, link_p);
	  CA_CollisionRemove((CacheLinkedList_t *)cache_p);
	  CA_FreeListFree(cache_p);
	}

	/* Add to LRU list */
	CA_LruInsert(link_p);
	  
	/* Add to collision list */
	CA_CollisionInsert(CA_GET_LLPOINTER(CAfixed_p->ca_link) + idx,
			  (CacheLinkedList_t *)link_p);
	/*
	 * Add access token to user hash 
	 */
	status = CA_InsertAccess(user_p, (token_t *)theToken);
      } 
      else 
      {
	if (cache_p)
	  CA_LruInsert(cache_p);

	status = DNS_NONSRESOURCES;
      }
    }
    
    CAUnlockData();
  }

leave_CAInsertLink:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CAInsertLink(%s,%s,%s,) return(%s)",
      deb_ascii_user(userBuf,user_p),
      deb_ascii_FullName(linknameBuf,linkname_p),
      deb_ascii_FullName(targetBuf,target_p),
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/*
 * Resolve softlinks in a name.  This routine only searches for links
 * which match the entire fullname. To search for links which match any
 * part of the name would be costly and it is not expected that softlinks
 * will be used that often.
 * The output name is always filled in.
 * Username is null terminated string 
 * Client cache locked on input.
 */
int 
CA_ResolveName (cds_user_t    user_p,
                unsigned int  seconds,
                FullName_t    *inname_p,
                FullName_t    *outname_p,
                dns_status_t  *status_p)
{
  DEB_ASCII_BUF_user(userBuf)
  DEB_ASCII_BUF_FullName(innameBuf)
  DEB_ASCII_BUF_FullName(outnameBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  FullName_u lname;
  dns_status_t status = DNS_CLERKBUG;
  int found = FALSE;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CA_ResolveName(%s,%d,%s,%p,%p)",
      deb_ascii_user(userBuf,user_p),
      seconds,
      deb_ascii_FullName(innameBuf,inname_p),
      outname_p,
      status_p));

#ifdef CACHE_PARANOID
  if (!CA_DataLocked()) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_CLOCK_A_MSG);
#if defined(DCE_CDS_DEBUG)
    COPY_FullName(inname_p, outname_p);
#endif
    goto leave_CA_ResolveName;
  }
#endif

  {
    /* X.500 Normalize full name */
    status = dns_normalize_fname(inname_p,
		(FullName_t *)lname, sizeof(FullName_u));
    if (status == DNS_SUCCESS) 
    {
      /* Get hash table index */
      const int idx = CAHashLink((FullName_t *)lname);
      register LinkEntry_t *link_p = CA_FindLink((FullName_t *)lname, idx);

      if (link_p) 
      {
	/* Make sure it hasn't timed out */
	if (((EQ_bytes(Nullutc, link_p->lh_time, LEN_utc(Nullutc))) ||
	     (!CATimedOut(link_p->lh_time))) && 
	    !CAStale(seconds, &link_p->lh_inCache)) 
	{

	  /* Check for access */
	  if (CA_LookupAccess(user_p, (token_t *)link_p->lh_token)) 
	  {
    
	    /* Copy target to outname */
	    COPY_FullName(SKIP_FullName(link_p->lh_name), outname_p);
	    found = TRUE;
	
	    /* Move to beginning of LRU list */
	    CALruMove(link_p);
	  }
#  if defined(DCE_CDS_DEBUG)
	    else
	      COPY_FullName(inname_p, outname_p);
#  endif
	}
#  if defined(DCE_CDS_DEBUG)
	  else
	    COPY_FullName(inname_p, outname_p);
#  endif
      } 
      else 
      {
	status = DNS_UNKNOWNENTRY;
#  if defined(DCE_CDS_DEBUG)
	COPY_FullName(inname_p, outname_p);
#  endif
      }
    } 
    else 
    {
      status = status;
#  if defined(DCE_CDS_DEBUG)
      COPY_FullName(inname_p, outname_p);
#  endif
    }
  }

#ifdef CACHE_PARANOID
leave_CA_ResolveName:
#endif
  if (status_p)
    *status_p = status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CA_ResolveName(%s,%d,%s,%p->%s,%p->%s) return(%d)",
      deb_ascii_user(userBuf,user_p),
      seconds,
      deb_ascii_FullName(innameBuf,inname_p),
      outname_p,
      deb_ascii_FullName(outnameBuf,outname_p),
      status_p,
      deb_ascii_dns_status(statusBuf,status),
      found));
  return(found);
}

/*
 * Resolve softlinks in a name.  This routine only searches for links
 * which match the entire fullname. To search for links which match any
 * part of the name would be costly and it is not expected that softlinks
 * will be used that often.
 * The output name is always filled in.
 * Username is null terminated string
 */
int 
CAResolveName (cds_user_t    user_p,
               unsigned int  seconds,
               FullName_t    *inname_p,
               FullName_t    *outname_p)
{
  DEB_ASCII_BUF_user(userBuf)
  DEB_ASCII_BUF_FullName(innameBuf)
  DEB_ASCII_BUF_FullName(outnameBuf)
  int found = FALSE;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CAResolveName(%s,%d,%s,)",
      deb_ascii_user(userBuf,user_p),
      seconds,
      deb_ascii_FullName(innameBuf,inname_p)));

  CALockData();
  found = CA_ResolveName(user_p, seconds, inname_p, outname_p, NULL);
  CAUnlockData();
  if (!found)
    COPY_FullName(inname_p, outname_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CAResolveName(%s,%d,%s,%s) return(%d)",
      deb_ascii_user(userBuf,user_p),
      seconds,
      deb_ascii_FullName(innameBuf,inname_p),
      deb_ascii_FullName(outnameBuf,outname_p),
      found));
  return(found);
}

/*
 * Remove a link from the cache.  This call is made
 * whenever the linktarget of an softlink is updated.
 * Client cache locked on input
 */
dns_status_t 
CA_RemoveLink (FullName_t *linkname_p)
{
  DEB_ASCII_BUF_FullName(linknameBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  register LinkEntry_t *link_p;
  FullName_u lname;
  int idx;
  dns_status_t status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CA_RemoveLink(%s)",
      deb_ascii_FullName(linknameBuf,linkname_p)));
  
  /* Convert name to normalized key */
  status = dns_normalize_fname(linkname_p,
		(FullName_t *)lname, sizeof(FullName_u));
  if (status == DNS_SUCCESS) 
  {
    /* Get hash table index for softlink */
    idx = CAHashLink((FullName_t *)lname);
  
    /* See if link already exists */
    link_p = CA_FindLink((FullName_t *)lname, idx);
    if (link_p) 
    {
      CA_LruRemove(link_p);
      CA_CollisionRemove((CacheLinkedList_t *)link_p);
      CA_FreeListFree(link_p);
    } else
      status = DNS_UNKNOWNENTRY;
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CA_RemoveLink(%s) return(%s)",
      deb_ascii_FullName(linknameBuf,linkname_p),
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}
