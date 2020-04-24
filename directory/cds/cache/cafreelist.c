/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cafreelist.c,v $
 * Revision 1.1.10.2  1996/02/18  19:24:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:04  marty]
 *
 * Revision 1.1.10.1  1995/12/08  00:10:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:32  root]
 * 
 * Revision 1.1.6.7  1994/08/16  17:27:12  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockGlobal() by CALockData(),
 * 	CAUnlockGlobal() by CAUnlockData(),
 * 	CALockClient() by CALockData(),
 * 	CAUnlockClient() by CAUnlockData(),
 * 	CA_ClientLocked() and CA_GlobalLocked() by CA_DataLocked(),
 * 	CA_ListLocked() by CA_DataLocked().
 * 	Remove CAListLock() and CAListUnlock().
 * 	[1994/08/16  15:30:07  peckham]
 * 
 * Revision 1.1.6.6  1994/07/13  15:23:42  peckham
 * 	Collapse CA_GLOBAL CA_CLIENT CA_LIST_LOCK to one lock.
 * 	[1994/07/11  18:28:49  peckham]
 * 
 * Revision 1.1.6.5  1994/06/23  18:28:22  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:40:07  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  16:07:06  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:42  devsrc]
 * 
 * Revision 1.1.6.3  1994/04/29  15:49:33  peckham
 * 	Use dcelocal_path in file names.
 * 	[1994/04/29  13:55:53  peckham]
 * 
 * Revision 1.1.6.2  1994/04/14  14:37:57  peckham
 * 	Change name of cache snapshot file
 * 	[1994/04/14  13:56:55  peckham]
 * 
 * 	Remove UNIX and VMS conditionals.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Use CA_snapshot_cache() instead of CA_abort().
 * 	Replace CA_abort() with CA_snapshot_cache().
 * 	[1994/04/11  17:37:35  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:55:42  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:47:31  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:36:44  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:19:48  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/30  20:06:30  peckham
 * 	OT#4481: In CAFreeListFree() insure entry is off Lru list
 * 	before freeing.  Rework ordering of setting fields to conform
 * 	to current DEC code.
 * 	[1992/06/30  20:05:46  peckham]
 * 
 * Revision 1.1.2.2  1992/03/22  21:12:57  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:05:11  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module cafreelist.c 
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
#include <unistd.h>
#include <sys/time.h>
#ifdef CACHE_PARANOID
# include <sys/types.h>
# include <fcntl.h>
# include <dns_strings.h>
#endif
#include <stdio.h>

#define MIN_FREE_BLOCK 2

CA_PRIVATE int
CA_lastCle (
   CleEntry_t *const);

CA_PRIVATE void
CA_FreeListInsert (
  CacheHeader_t *const);

CA_PRIVATE void
CA_FreeListRemove (
  CacheHeader_t *const);

CA_PRIVATE void
CA_Breakup (
     CacheHeader_t *const,
     const int           );

CA_PRIVATE void
CA_CleanupUserHash (
     UserEntry_t *const);

CA_PRIVATE int
CA_Reclaim (
    const int);

CA_PRIVATE int
CA_Freespace (
    CacheHeader_t *const,
    const int           ,
    const int           );

/*
 *
 */
void 
CA_snapshot_cache (void)
{
#ifdef CACHE_PARANOID
    char file[_DNS_MAXPATHLEN+1];
    int fd;

    (void)sprintf(file, CDS_CACHE_SNAPSHOT, dcelocal_path, getpid());

    if (0 <= (fd = open(file, O_CREAT|O_WRONLY|O_EXCL, 0600))) 
    {
	char *buf_p = (char *)CAfixed_p;
	int size = CAfixed_p->ca_size;

	while (0 < size) 
	{
	    int cc = write(fd, buf_p, size);

	    if (cc <= 0) break;
	    buf_p += cc;
	    size -= cc;
	}

	(void)close(fd);
    }
    dce_svc_printf(CDS_S_CACHE_SNAPSHOT_FILE_MSG, file);
#endif
}

#ifdef CACHE_PARANOID
/*
 * Verify CacheHeader as much as possible
 */
int 
CA_hdr_corrupt (CacheHeader_t *cache_p)
{
  const int offset = CA_GET_OFFSET(cache_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug9,
      " >CA_hdr_corrupt(%p)",
      cache_p));

  if (CA_AtomicLocked())
    return(FALSE);

  if ((offset < 0) || (CAfixed_p->ca_size <= offset))
  { 
      CA_snapshot_cache();
      dce_svc_printf(CDS_S_CACHE_HEADER_A_MSG);
  }

  if (cache_p->hdr.pattern != PATTERN)
  { 
      CA_snapshot_cache();
      dce_svc_printf(CDS_S_CACHE_PATTERN_MSG);
  }

  switch (cache_p->hdr.inuse) 
  {
  case 1:
    break;
  case 0:
    DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_cache,
          svc_c_debug9,
          " <CA_hdr_corrupt(%p) return FALSE",
          cache_p)); 
    return (FALSE);
  default:
    { 
      CA_snapshot_cache();
      dce_svc_printf(CDS_S_CACHE_INUSE_A_MSG);

    }
  }

  if (cache_p->hdr.list.ll_flink
      && (CA_GET_CAPOINTER(cache_p->hdr.list.ll_flink)->hdr.list.ll_blink != offset))
  { 
      CA_snapshot_cache();
      dce_svc_printf(CDS_S_CACHE_FLINK_A_MSG);
  }

  if (cache_p->hdr.list.ll_blink
      && (CA_GET_CAPOINTER(cache_p->hdr.list.ll_blink)->hdr.list.ll_flink != offset))
  { 
      CA_snapshot_cache();
      dce_svc_printf(CDS_S_CACHE_BLINK_A_MSG);

  }

  switch (cache_p->hdr.type) 
  {
  case Othertype:
  case DHEntry:
  case CHEntry:
  case LHEntry:
  case NHEntry:
  case AHEntry:
  case VHEntry:
  case UHEntry:
  case UAEntry:
  case UGEntry:
    break;
  default:
    { 
      CA_snapshot_cache();
      dce_svc_printf(CDS_S_CACHE_TYPE_A_MSG);
    }
  }
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug9,
      " <CA_hdr_corrupt(%p) return FALSE",
      cache_p));
  return (FALSE);
}

/*
 * Verify freelist as much as possible
 */
int 
CA_cache_corrupt (void)
{
  register CacheHeader_t *cache_p;

  if ((EXT8(CAfixed_p->ca_version.vr_major) != MAJOR)
      || (EXT8(CAfixed_p->ca_version.vr_minor) != MINOR)
      || NE_bytes(CAfixed_p->ca_id, CacheName, CacheNameSize)
     )
  { 
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_VERSION_A_MSG);
  }

  if (CA_AtomicLocked())
    return(FALSE);

  for (cache_p = &CAfixed_p->ca_free + 1;
       cache_p->hdr.size != 0;
       cache_p += 1 + cache_p->hdr.size)
    if (CA_hdr_corrupt(cache_p))
      return (TRUE);

  return (FALSE);
}

#endif	/* CACHE_PARANOID */


/*
 * This routine initiailize the free list to contain all of the cache
 * except the space allocated to the cache header
 */
void 
CA_FreeListInit (struct CacheFixed *const  ca_p,
                 int                       size)
{
  register CacheHeader_t *cache_p, *last_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_FreeListInit(,%d)",
      size));

  CAAtomicLock();
    /* Set up header as dummy cache block in use */
    ca_p->ca_free.hdr.prevsize = 0;
    ca_p->ca_free.hdr.size = 0;
    ca_p->ca_free.hdr.pattern = PATTERN;
    ca_p->ca_free.hdr.inuse = TRUE;

    /* Set up free list to contain one block of all memory */
    cache_p = &ca_p->ca_free + 1;
    ca_p->ca_free.hdr.list.ll_flink = CA_GET_OFFSET(cache_p);
    ca_p->ca_free.hdr.list.ll_blink = CA_GET_OFFSET(cache_p);
    cache_p->hdr.list.ll_flink = CA_GET_OFFSET(&ca_p->ca_free);
    cache_p->hdr.list.ll_blink = CA_GET_OFFSET(&ca_p->ca_free);
  
    /* 
     * The size of a cache block is stored in CacheHeader units.
     * Each header contains a forward and backward link, its size, 
     * the size of the previous block, an inuse flag and a bit pattern.
     * The links are the CacheHeader unit offsets from the address of
     * the free list header to the next free block or LRU entry.  
     * The bit pattern used to check for corruption.
     * The size of the previous block is used to combine contiguous 
     * blocks of free memory into one block.
     */
    cache_p->hdr.prevsize = 0;
    cache_p->hdr.pattern = PATTERN;     /* bit pattern for corruption check */
    cache_p->hdr.refs = 0;
    cache_p->hdr.inuse = FALSE;
    /* 
     * Size of free block is stored as the number of CacheHeader units.
     * This allows us to do arithmetic portably.
     * A spare header is reserved at the end of the cache and 
     * the current header is removed from the size count
     */
    cache_p->hdr.size = (size/sizeof(CacheHeader_t))-2;

    /* Set up available space, include first cache header but not last */
    ca_p->ca_total = cache_p->hdr.size + 1;
    ca_p->ca_Cused = 0;
    ca_p->ca_Gused = 0;


    /* Set up pointer to last cache entry which is reserved */
    last_p = cache_p + cache_p->hdr.size + 1;
  
    /* Set up header of reserved cacheheader to be inuse */
    last_p->hdr.prevsize = cache_p->hdr.size;
    last_p->hdr.size = 0;
    last_p->hdr.pattern = PATTERN;
    last_p->hdr.refs = 0;
    last_p->hdr.inuse = TRUE;
  CAAtomicUnlock();

  (void)CA_cache_corrupt();

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_FreeListInit(,%d)",
      size));
}

/*
 * Allocate space for an entry in the client cache.  It is assumed
 * the caller holds a write lock on the client cache.  
 * Any pointer held in the cache may be freed by calls to reclaim space.
 * They should be removed from LRU lists prior to this call.
 * It is possible for the same entry to be added multiple times because
 * to eliminate deadlock on a reclaim we must release the global lock
 * and lock the entire cache.
 */
void *
CA_ClientAlloc (int             size,
                enum CacheType  type)
{
  void *memory_p;
  int i;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_ClientAlloc(%d,%d)",
      size,
      type));

#ifdef CACHE_PARANOID
  if (!CA_DataLocked()) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_CLOCK_A_MSG);
  }
#endif

  for (i=0; i<MAX_RECLAIM_TRIES; ++i) 
  {
    int status;

    memory_p = CA_FreeListAlloc(size, type);
    if (memory_p) break;

    status = CA_Reclaim(size);
    
    /* If unable to free any space, cannot continue */
    if (status != DNS_SUCCESS)
      break;
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_ClientAlloc(%d,%d) return(%p)",
      size,
      type,
      memory_p));
  return(memory_p);
}

/*
 * Allocate space for an entry in the global cache.  It is assumed
 * the caller holds a write lock on the global cache.  
 * Any pointer held in the cache may be freed by calls to reclaim space.
 * They should be removed from LRU lists prior to this call.
 */
void *
CA_GlobalAlloc (int             size,
                enum CacheType  type)
{
  void *memory_p;
  int i;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_GlobalAlloc(%d,%d)",
      size,
      type));

#ifdef CACHE_PARANOID
  if (!CA_DataLocked()) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_GLOCK_A_MSG);
  }
#endif

  for (i=0; i<MAX_RECLAIM_TRIES; ++i) 
  {
    int status;

    memory_p = CA_FreeListAlloc(size, type);
    if (memory_p) break;

    status = CA_Reclaim(size);

    /* If unable to free any space, cannot continue */
    if (status != DNS_SUCCESS)
      break;
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_GlobalAlloc(%d,%d) return(%p)",
      size,
      type,
      memory_p));
  return(memory_p);
}

/*
 * Put a CacheHeader at the beginning of the  freelist
 * Callers have already blocked signals 
 */
CA_PRIVATE void 
CA_FreeListInsert (CacheHeader_t *const cache_p)
{
  register CacheHeader_t *const next_p =
		CA_GET_CAPOINTER(CAfixed_p->ca_free.hdr.list.ll_flink);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_FreeListInsert(%p)",
      cache_p));

#ifdef CACHE_PARANOID
  if (!CA_DataLocked()) 
  { 
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_LLOCK_A_MSG);
  }
  if (CA_hdr_corrupt(cache_p)) return;
  if (CA_hdr_corrupt(next_p)) return;
  if (cache_p->hdr.list.ll_flink || cache_p->hdr.list.ll_blink)
  { 
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_INLIST_A_MSG);
  }
#endif

  CAAtomicLock();
    cache_p->hdr.list.ll_flink = CAfixed_p->ca_free.hdr.list.ll_flink;
    cache_p->hdr.list.ll_blink = CA_GET_OFFSET(&CAfixed_p->ca_free);

    next_p->hdr.list.ll_blink = CA_GET_OFFSET(cache_p);
    CAfixed_p->ca_free.hdr.list.ll_flink = CA_GET_OFFSET(cache_p);
  CAAtomicUnlock();
}

/*
 * Remove an element from the freelist 
 * Callers have already blocked signals 
 */
CA_PRIVATE void 
CA_FreeListRemove (CacheHeader_t *const cache_p)
{
  register CacheHeader_t *const next_p =
		CA_GET_CAPOINTER(cache_p->hdr.list.ll_flink);
  register CacheHeader_t *const prev_p =
		CA_GET_CAPOINTER(cache_p->hdr.list.ll_blink);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_FreeListRemove(%p)",
      cache_p));

#ifdef CACHE_PARANOID
  if (CA_hdr_corrupt(cache_p)) return;
  if (CA_hdr_corrupt(next_p)) return;
  if (CA_hdr_corrupt(prev_p)) return;
  if (!cache_p->hdr.list.ll_flink || !cache_p->hdr.list.ll_blink)
  { 
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_NOTINLIST_A_MSG);
  }
#endif
  
  CAAtomicLock();
    next_p->hdr.list.ll_blink = cache_p->hdr.list.ll_blink;
    prev_p->hdr.list.ll_flink = cache_p->hdr.list.ll_flink;

    cache_p->hdr.list.ll_flink = 0;  
    cache_p->hdr.list.ll_blink = 0;  
  CAAtomicUnlock();
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_FreeListRemove(%p)",
      cache_p));
}

/*
 * Move an allocated cache block onto the freelist.  Combine it with
 * the previous block if possible. 
 */
void 
CA_FreeListFree (void *free_p)
{
  register CacheHeader_t *cache_p = CacheHeader_p(free_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_FreeListFree(%p)",
      free_p));

  /* 
   * Check if header has been corrupted.
   * If so, keep it off free list and hope we can continue.
   */

  if (CA_hdr_corrupt(cache_p))
    {
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_FreeListFree(%p) header corrupt - returning",
      free_p));

    return;
    }

  if (!cache_p->hdr.inuse) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_NOTINUSE_A_MSG);
  }

  if (CA_refs(free_p) < 0)
    CA_LruRemove(free_p);

  {
    CacheHeader_t *const prev_p = cache_p - (1 + cache_p->hdr.prevsize);
    CacheHeader_t *const next_p = cache_p + (1 + cache_p->hdr.size);
    int freeed = FALSE;

    if (CA_hdr_corrupt(prev_p))
      goto unlock_CA_FreeListFree;

    if (CA_hdr_corrupt(next_p))
      goto unlock_CA_FreeListFree;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug8,
        "  CA_FreeListFree(%p) block(%p,%d,%d)",
        free_p,
        cache_p,
        cache_p->hdr.size,
        cache_p->hdr.type));

    cache_p->hdr.inuse = FALSE;
    cache_p->hdr.refs = 0;

    /* 
     * Update space used.  It the type returned is othertype,
     * this block size is added back to the total space,
     * Otherwise it is subtracted from the used space.
     */ 
    switch (cache_p->hdr.type) 
    {
    case Othertype:
      CAfixed_p->ca_total += (1 + cache_p->hdr.size);
      break;
    case CHEntry:
    case DHEntry:
      CAfixed_p->ca_Gused -= (1 + cache_p->hdr.size);
      break;
    default:
      CAfixed_p->ca_Cused -= (1 + cache_p->hdr.size);
      break;
    }
  
    /* If previous block is on freelist, combine contiquous memory */
    if (!prev_p->hdr.inuse) 
    { 
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_cache,
          svc_c_debug8,
          "  CA_FreeListFree(%p) merge with prev(%p,%d)",
          free_p,
          prev_p,
          prev_p->hdr.size));
    
      /* Calculate new size of combined blocks */
      prev_p->hdr.size += (1 + cache_p->hdr.size);
    
      /* Fix up next memory block */
      next_p->hdr.prevsize = prev_p->hdr.size;

      if (CA_cache_corrupt())
	goto unlock_CA_FreeListFree;

      /* Reset beginning of block incase we combine it with the next block */
      cache_p = prev_p;
    
      /* Since previous block was already on freelist, this block is also */
      freeed = TRUE;
    }
  
    /* If next block is on freelist, combine contiquous memory */
    if (!next_p->hdr.inuse) 
    {
      register CacheHeader_t *const end_p = next_p + (1 + next_p->hdr.size);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_cache,
          svc_c_debug8,
          "  CA_FreeListFree(%p) merge with next(%p,%d)",
          free_p,
          next_p,
          next_p->hdr.size));

      if (CA_hdr_corrupt(end_p))
	goto unlock_CA_FreeListFree;
    
      /* Take element off free list and combine here */
      CA_FreeListRemove(next_p);

      /* Update size to include next block */
      cache_p->hdr.size += (1 + next_p->hdr.size);

      /* Update header of new next block */
      end_p->hdr.prevsize = cache_p->hdr.size;

      if (CA_cache_corrupt())
	goto unlock_CA_FreeListFree;
    }
  
    if (!freeed) 
      /* 
       * Cannot combine blocks, update next blocks prevsize to 
       * indicate this block is free
       */
      CA_FreeListInsert(cache_p);
  }
unlock_CA_FreeListFree:

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_FreeListFree(%p)",
      free_p));
}

/*
 * Allocate a block of memory from the free list.  
 * Memory is allocated in CacheHeader units.
 * This guarentees longword alignment 
 */
void *
CA_FreeListAlloc (int             bytes,
                  enum CacheType  type)
{
  register CacheHeader_t *cache_p;
  void *result = NULL;
  
  /* Round up request size to multiple of CacheHeader */
  const int size = CacheHeader_round(bytes);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_FreeListAlloc(%d,%d)",
      bytes,
      type));

  /* Locate first free block that will satisfy this request */
  for (cache_p = CA_GET_CAPOINTER(CAfixed_p->ca_free.hdr.list.ll_flink);
       cache_p != &CAfixed_p->ca_free; 
       cache_p = CA_GET_CAPOINTER(cache_p->hdr.list.ll_flink)) 
  {
    if (CA_hdr_corrupt(cache_p))
      break;

    if (size <= cache_p->hdr.size) 
    {
      /* Take element off free list */
      CA_FreeListRemove(cache_p);
      
      /* inuse must be set last in the following */
#ifdef CACHE_PARANOID
      cache_p[1].hdr.list.ll_flink = 0;
      cache_p[1].hdr.list.ll_blink = 0;
#endif
      cache_p->hdr.type = type;
      cache_p->hdr.refs = 0;
      cache_p->hdr.inuse = TRUE;
      /* Divide into multiple blocks if necessary */
      CA_Breakup(cache_p,size);
      /* 
       * Update space used.  It the type requested is othertype,
       * this block will not appear on the LRU list and will not
       * be available for reclaiming space.  Its size is removed from
       * the total space.  Otherwise it could be reclaimed and
       * is considered used 
       */ 
      switch (type) 
      {
      case Othertype:
	CAfixed_p->ca_total -= (cache_p->hdr.size + 1);
	break;
      case CHEntry:
      case DHEntry:
	CAfixed_p->ca_Gused += (cache_p->hdr.size + 1);
	break;
      default:
	CAfixed_p->ca_Cused += (cache_p->hdr.size + 1);
	break;
      }
      result = &cache_p[1];
      break;
    }
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_FreeListAlloc(%d,%d) return(%p)",
      bytes,
      type,
      result));
  return(result);
}

/*
 * A cache block is being allocated.  If the required is smaller then
 * the size of the block, return the unused protion to the free list.
 * The free list or entire cache is write locked on entry.
 * Caller has already blocked signals
 */
CA_PRIVATE void 
CA_Breakup (CacheHeader_t *const  cache_p,
            const int             size)
{
  /* Found a block, take what we need and leave the rest */
  int diff = cache_p->hdr.size - size;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " >CA_Breakup(%p,%d)",
      cache_p,
      size));
  
  /* Break into 2 blocks if too big */
  if (MIN_FREE_BLOCK < diff) 
  {
    register CacheHeader_t *const next_p = cache_p + (1 + size);
    register CacheHeader_t *const last_p = next_p + diff;
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug7,
        "  CA_Breakup(%p,%d) clip(%d) next(%p,%d) last(%p)",
        cache_p,
        size,
        diff,
        next_p,
        diff-1,
        last_p));

    /* Fix up remaining bytes to be a new free block */
    next_p->hdr.list.ll_flink = 0;
    next_p->hdr.list.ll_blink = 0;
    next_p->hdr.prevsize = size;
    next_p->hdr.pattern = PATTERN;
    next_p->hdr.size = --diff;	/* Subtract out header */
    next_p->hdr.refs = 0;
    next_p->hdr.inuse = FALSE;

    CAAtomicLock();
      /* Fix up block being allocated to reflect new size */
      cache_p->hdr.size = size;

      /* Fix up next contiguous blocks header to reflect new size */
      last_p->hdr.prevsize = diff;
    CAAtomicUnlock();

    if (!CA_cache_corrupt())
      /* Move new block onto free list */
      CA_FreeListInsert(next_p);
  }
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " <CA_Breakup(%p,%d)",
      cache_p,
      size));
}

/*
 * Free all entries in the UserGroup and UserName tables
 * and free the UserName table itself.
 * Most of the time thest tables will be empty cause
 * their entries are also on the LRU list also and both the entry
 * and userentry get moved on the LRU list at the same time.
 */
CA_PRIVATE void 
CA_CleanupUserHash (UserEntry_t *const ue_p)
{
  DEB_ASCII_BUF_UserEntry(ueBuf)
  register CacheLinkedList_t *ll_p;
  register int idx;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_CleanupUserHash(%s)",
      deb_ascii_UserEntry(ueBuf,ue_p)));

#ifdef CACHE_PARANOID
  if (!CA_DataLocked()) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_CLOCK_A_MSG);
  }
#endif

  /* Clean out UserAccess table */
  if (ue_p->uh_access != 0) 
  {
    CacheLinkedList_t *const list_p = CA_GET_LLPOINTER(ue_p->uh_access);

    for (ll_p = list_p, idx = 0; idx < USERACCESS_HASH_LEN; ll_p++, idx++)
      while (TRUE) 
      {
	UserAccessEntry_t *const access_p = CA_GET_UAPOINTER(ll_p->ll_flink);

	if (access_p == (UserAccessEntry_t *)ll_p) break;
	CA_CollisionRemove((CacheLinkedList_t *)access_p);
	CA_LruRemove(access_p);
	CA_FreeListFree(access_p);
      }

    CA_FreeListFree(list_p);
  }

    /* Clean out UserGroup table */
  if (ue_p->uh_group != 0) 
  {
    CacheLinkedList_t *const list_p = CA_GET_LLPOINTER(ue_p->uh_group);

    for (ll_p = list_p, idx = 0; idx < USERGROUP_HASH_LEN; ll_p++, idx++)
      while (TRUE) 
      {	/* pull off the front until all gone */
	UserGroupEntry_t *const group_p = CA_GET_UGPOINTER(ll_p->ll_flink);

	if (group_p == (UserGroupEntry_t *)ll_p) break;
	CA_CollisionRemove((CacheLinkedList_t *)group_p);
	CA_LruRemove(group_p);
	CA_FreeListFree(group_p);
      }

    CA_FreeListFree(list_p);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_CleanupUserHash(%s)",
      deb_ascii_UserEntry(ueBuf,ue_p)));
}

/* 
 * Remove blocks from the LRU list until one is found that satisfies
 * the current allocation request.  The block is added to the freelist.
 * The global and client caches must be locked on entry.  Since both are 
 * locked, we are free to access shared information without additional
 * locking since no-one else can get at them.
 */
CA_PRIVATE int 
CA_Reclaim (const int size)
{
  DEB_ASCII_BUF_dns_status(statusBuf)
  int status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " >CA_Reclaim(%d)",
      size));

#ifdef CACHE_PARANOID
  if (!CA_DataLocked()) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_CLOCK_A_MSG);
  }
#endif

  /* 
   * Look for a block big enough to satify the request and free it.
   * All callers of this routine repeat the call MAX_RECLAIM_TRIES,
   * so if the space is still not enough or someone else gets
   * it first, we will be back.
   */

  /* If client cache is getting extra large, take from it first */
  /* for efficiency: 6/10 ~= 9/16 */
  if (CAfixed_p->ca_Cused >= (CAfixed_p->ca_total*9)/16) 
  {
    status = CA_Freespace(&CAfixed_p->ca_Clru, CAfixed_p->ca_Cused, size);
    if (status != DNS_SUCCESS)
      status = CA_Freespace(&CAfixed_p->ca_Glru, CAfixed_p->ca_Gused, size);
  } 
  else 
  {
    status = CA_Freespace(&CAfixed_p->ca_Glru, CAfixed_p->ca_Gused, size);
    if (status != DNS_SUCCESS)
      status = CA_Freespace(&CAfixed_p->ca_Clru, CAfixed_p->ca_Cused, size);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " <CA_Reclaim(%d) return(%s)",
      size,
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/* 
 * Search the LRU list until an entry is found that satisfies
 * the current allocation request.  The block is added to the freelist.
 * In order to reduce fragmentation, free 20% of the space on the list
 * even if it doesn't satisfy the request.   
 * The cache is locked by the caller, so can look at everything.
 */
CA_PRIVATE int 
CA_Freespace (CacheHeader_t *const  hdr_p,
              const int             total,
              const int             bytes)
{
  DEB_ASCII_BUF_dns_status(statusBuf)
  CacheHeader_t *prev_p, *cache_p;
  int status = DNS_NOTFND;

  /* Round up request size to multiple of CacheHeader */
  const int size = CacheHeader_round(bytes);

  /* Free 20% of total blocks on this LRU list */
  /* for efficiency: 2/10 ~= 3/16 */
  int total_20 = (total*3)/16;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " >CA_Freespace(%p,%d,%d)",
      hdr_p,
      total,
      bytes));

#ifdef CACHE_PARANOID
  if (!CA_DataLocked())
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_CLOCK_A_MSG);
  }
#endif

  /* 
   * Look for a block big enough to satify the request and free it.
   * All callers of this routine repeat the call MAX_RECLAIM_TRIES,
   * so if the space is still not enough or someone else gets
   * it first, we will be back.
   * Entries at back of the list have been there the longest.
   */

  for (cache_p = CA_GET_CAPOINTER(hdr_p->hdr.list.ll_blink);
       cache_p != hdr_p;
       cache_p = prev_p) 
  {
    const int this_size = cache_p->hdr.size;

    if (CA_hdr_corrupt(cache_p)) break;

    /* Save pointer before list manipulations */
    prev_p = CA_GET_CAPOINTER(cache_p->hdr.list.ll_blink);    

    /* 
     * If this block will satisfy the request, free it.
     */ 
    if ((this_size < size) && (total_20 <= 0))
      continue;

    if ((cache_p->hdr.type == CHEntry) &&
	(((CleEntry_t *)(cache_p+1))->ch_flags.status.flags.sticky))
      continue;

    if ((cache_p->hdr.type == CHEntry) && 
	     CA_lastCle((CleEntry_t *)(cache_p+1)))
      /* If this is the last clearinghouse, don't free it */
      continue;

    /* 
     * Do this first, cause the next entry on the LRU list may
     * be something pointed to by the userhash
     */
    if (cache_p->hdr.type == UHEntry) 
    {
      /* Need to free entries it may point to and its hash tables */
      CA_CleanupUserHash((UserEntry_t *)(cache_p + 1));
      /* prev_p may have been released; re-fetch */
      prev_p = CA_GET_CAPOINTER(cache_p->hdr.list.ll_blink);    
    }
    
    /* Take element off lru list */
    CA_LruRemove((cache_p+1));
    
    /* Take element off collision list */
    CA_CollisionRemove(((CacheLinkedList_t *)(cache_p+1)));
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug7,
        "  CA_Freespace(%p,%d,%d) freeing(%p,%d) type(%d)",
        hdr_p,
        total_20,
        bytes,
        cache_p,
        cache_p->hdr.size,
        cache_p->hdr.type));
  
    CA_FreeListFree((cache_p+1));

    total_20 -= this_size;
  
    /* If this block satisfies our reqest, we are done */
    if (size <= this_size) 
    {
      status = DNS_SUCCESS;
      break;
    } 
  } /* End while */

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " <CA_Freespace(%p,%d,%d) return(%s)",
      hdr_p,
      total,
      bytes,
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/* 
 * Don't allow the last clearinghouse to be freed.
 */
CA_PRIVATE int 
CA_lastCle (CleEntry_t *const cache_p)
{
     /* Get hash table pointer */
     CacheLinkedList_t *ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_cle);
     register CleEntry_t *cle_p;
     int i;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_lastCle(%p)",
      cache_p));

     /* Loop through hash table entries looking for another cached cle */
     for (i = 0; i < CAfixed_p->ca_cle_hash_len; ll_p++, i++)
	 for (cle_p = CA_GET_CHPOINTER(ll_p->ll_flink);
	      cle_p != (CleEntry_t *)ll_p;
	      cle_p = CA_GET_CHPOINTER(cle_p->ch_collision.ll_flink))

	     /* If its not me, someone else here, so we are done */
	     if (cle_p != cache_p) 
	     {
  	       DCE_SVC_DEBUG((
      		   cds__svc_handle,
      		   cds_svc_cache,
      		   svc_c_debug8,
      		   " <CA_lastCle(%p) return (FALSE)",
      		   cache_p));
	
	       return(FALSE);
	     }

     DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_cache,
         svc_c_debug8,
         " <CA_lastCle(%p) return(TRUE)",
         cache_p));
     return(TRUE);
}
