/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cache.c,v $
 * Revision 1.1.9.2  1996/02/18  19:23:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:56  marty]
 *
 * Revision 1.1.9.1  1995/12/08  00:10:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:25  root]
 * 
 * Revision 1.1.6.4  1994/06/23  18:28:14  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:14:42  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  16:06:59  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:33  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:37:50  peckham
 * 	Eliminate _DNS__UNIX conditional.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Use CA_snapshot_cache() instead of CA_abort().
 * 	[1994/04/11  17:31:12  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:55:27  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:46:39  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:36:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:19:06  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:10:10  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:03:54  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*	
 * Module cache.c
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
#include <sys/time.h>


/*
 *  Insert an element at the beginning of the collision list.
 *  Hash table is assumed to be locked by the caller.
 */
void 
CA_CollisionInsert (CacheLinkedList_t *const  hdr_p,
                    CacheLinkedList_t *const  element_p)
{  
  CacheLinkedList_t *const ll_p = CA_GET_LLPOINTER(hdr_p->ll_flink);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_CollisionInsert(%p,%p)",
      hdr_p,
      element_p));

#ifdef CACHE_PARANOID
  /* die if links being used */
  if (element_p->ll_blink || element_p->ll_flink) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_INLIST_A_MSG);
  }
#endif

  CAAtomicLock();
    element_p->ll_blink = CA_GET_OFFSET(hdr_p);
    element_p->ll_flink = hdr_p->ll_flink;
    ll_p->ll_blink = CA_GET_OFFSET(element_p);
    hdr_p->ll_flink = CA_GET_OFFSET(element_p);
  CAAtomicUnlock();

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_CollisionInsert(%p,%p)",
      hdr_p,
      element_p));
}

/*
 * Remove an element from the collision list 
 * Hash table is assumed to be locked by caller.
 */
void 
CA_CollisionRemove (CacheLinkedList_t *const element_p)
{

  /* Convert cache offset into address */
  CacheLinkedList_t *ll_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_CollisionRemove(%p)",
      element_p));

#ifdef CACHE_PARANOID
  /* die if links not being used */
  if (!element_p->ll_blink || !element_p->ll_flink) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_NOTINLIST_A_MSG);
    return;
  }
#endif

  CAAtomicLock();
    ll_p = CA_GET_LLPOINTER(element_p->ll_flink);
    ll_p->ll_blink = element_p->ll_blink;
    ll_p = CA_GET_LLPOINTER(element_p->ll_blink);
    ll_p->ll_flink = element_p->ll_flink;

    element_p->ll_flink = 0;
    element_p->ll_blink = 0;
  CAAtomicUnlock();

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_CollisionRemove(%p)",
      element_p));
  return;
}

/*
 *   Insert a cache element at the beginning of the LRU list.
 *   List is locked by cache lock held by caller.
 */
void 
CA_LruInsert (void *element_p)
{  
  CacheHeader_t *hdr_p;  
  CacheHeader_t *cache_p, *next_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_LruInsert(%p)",
      element_p));

#ifdef CACHE_PARANOID
  if (CA_hdr_corrupt(CacheHeader_p(element_p)))
    return;
  if (CA_refs(element_p) < 0) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_CAREFS_A_MSG);
  }
#endif
  if (CA_locked(element_p)) 
  {
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug8,
        " <CA_LruInsert(%p) CA_locked(%p)",
        element_p,
	element_p));
      return;
  }
  
  /* Get address of header.  It proceeds the allocated element */
  cache_p = CacheHeader_p(element_p);

  /* Determine appropriate LRU list */
  if ((cache_p->hdr.type == CHEntry) || (cache_p->hdr.type == DHEntry)) 
  {
#ifdef CACHE_PARANOID
    if (!CA_GlobalLocked()) 
    {
      CA_snapshot_cache();
      dce_svc_printf(CDS_S_CACHE_GLOCK_A_MSG);
      return;
    }
#endif
    hdr_p = &CAfixed_p->ca_Glru;
  } 
  else 
  {
#ifdef CACHE_PARANOID
    if (!CA_ClientLocked()) 
    {
      CA_snapshot_cache();
      dce_svc_printf(CDS_S_CACHE_CLOCK_A_MSG);
      return;
    }
#endif
    hdr_p = &CAfixed_p->ca_Clru;
  }

#ifdef CACHE_PARANOID
  /* die if links being used */
  if (cache_p->hdr.list.ll_blink || cache_p->hdr.list.ll_flink) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_INLIST_A_MSG);
    return;
  }
#endif

  CAAtomicLock();
    cache_p->hdr.list.ll_blink = CA_GET_OFFSET(hdr_p);
    cache_p->hdr.list.ll_flink = hdr_p->hdr.list.ll_flink;
  
    next_p = CA_GET_CAPOINTER(hdr_p->hdr.list.ll_flink);
    next_p->hdr.list.ll_blink = CA_GET_OFFSET(cache_p);
    hdr_p->hdr.list.ll_flink = CA_GET_OFFSET(cache_p);
    CA_refs(element_p) = -1;
  CAAtomicUnlock();
    
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_LruInsert(%p)",
      element_p));
  return;
}

/*
 *   Remove a cache element from the LRU list.
 */
void 
CA_LruRemove (void *element_p)
{  

  CacheHeader_t *cache_p, *next_p, *prev_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_LruRemove(%p)",
      element_p));

#ifdef CACHE_PARANOID
  if (CA_hdr_corrupt(CacheHeader_p(element_p)))
    return;
  if (CA_refs(element_p) == 0) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_CAREFS_A_MSG);
    return;
  }
#endif
  if (CA_locked(element_p)) 
  {
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug8,
        " <CA_LruRemove(%p) CA_locked(%p)",
        element_p,
	element_p));
    return;
  }
  /* Get address of header.  It proceeds the allocated element */
  cache_p = CacheHeader_p(element_p);

#ifdef CACHE_PARANOID
  /* Determine appropriate LRU list */
  if ((cache_p->hdr.type == CHEntry) || (cache_p->hdr.type == DHEntry)) 
  {
    if (!CA_GlobalLocked()) 
    {
      CA_snapshot_cache();
      dce_svc_printf(CDS_S_CACHE_GLOCK_A_MSG);
      return;
    }
  } 
  else 
  {
    if (!CA_ClientLocked()) 
    {
      CA_snapshot_cache();
      dce_svc_printf(CDS_S_CACHE_CLOCK_A_MSG);
      return;
    }
  }

  /* die if links not being used */
  if (!cache_p->hdr.list.ll_blink || !cache_p->hdr.list.ll_flink) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_NOINLIST_A_MSG);
    return;
  }
#endif

  CAAtomicLock();
    next_p = CA_GET_CAPOINTER(cache_p->hdr.list.ll_flink);
    next_p->hdr.list.ll_blink = cache_p->hdr.list.ll_blink;
  
    prev_p = CA_GET_CAPOINTER(cache_p->hdr.list.ll_blink);
    prev_p->hdr.list.ll_flink = cache_p->hdr.list.ll_flink;

    cache_p->hdr.list.ll_flink = 0;
    cache_p->hdr.list.ll_blink = 0;
    CA_refs(element_p) = 0;
  CAAtomicUnlock();
    
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_LruRemove(%p)",
      element_p));
  return;
}

/*
 * Move an element to the front of the LRU list.  
 * Must already be on the list.
 */
void 
CALruMove (void *element_p)
{
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CALruMove(%p)",
      element_p));

  if (CA_hdr_corrupt(CacheHeader_p(element_p))) 
  {
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug8,
        " <CALruMove(%p) CA_hdr_corrupt(%p)",
	element_p,
        element_p));
      return;
  }

  if (CA_locked(element_p)) 
  {
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug8,
        " <CALruMove(%p) CA_locked(%p)",
	element_p,
        element_p));
    return;
  }

  /* First remove it from wherever it is on the list */
  CA_LruRemove(element_p);

  /* Now add it to the front of the list */
  CA_LruInsert(element_p);
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CALruMove(%p)",
      element_p));
}



