/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cacle.c,v $
 * Revision 1.1.12.2  1996/02/18  19:23:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:57  marty]
 *
 * Revision 1.1.12.1  1995/12/08  00:10:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:26  root]
 * 
 * Revision 1.1.9.6  1994/08/16  17:27:09  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockGlobal() by CALockData(),
 * 	CAUnlockGlobal() by CAUnlockData().
 * 	[1994/08/16  15:29:44  peckham]
 * 
 * Revision 1.1.9.5  1994/08/11  20:35:10  peckham
 * 	Insure that the clearinghouse is removed from the wan file
 * 	when removed via the CARemoveClearinghouse() call.
 * 	[1994/08/11  18:44:19  peckham]
 * 
 * 	CR#9765 - cds client better disaster recovery.
 * 	Make a cached clearinghouse tag sticky once defined.
 * 	Call CA_write_servers() when new cached clearinghouse defined.
 * 	[1994/08/11  04:47:48  peckham]
 * 
 * Revision 1.1.9.4  1994/06/23  18:28:17  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:15:00  mccann]
 * 
 * Revision 1.1.9.3  1994/06/09  16:07:01  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:34  devsrc]
 * 
 * Revision 1.1.9.2  1994/04/14  14:37:51  peckham
 * 	Remove VMS conditionals.
 * 	[1994/04/11  17:32:04  peckham]
 * 
 * Revision 1.1.9.1  1994/03/12  21:55:29  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:46:49  peckham]
 * 
 * Revision 1.1.6.6  1993/03/09  19:44:00  peckham
 * 	OT#7411: Request given to gda when other clearinghouse
 * 	available. Mark gda clearinghouses found by CA_ClearinghouseSet().
 * 	[1993/03/09  19:26:33  peckham]
 * 
 * Revision 1.1.6.5  1992/12/30  12:36:18  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:19:13  zeliff]
 * 
 * Revision 1.1.6.4  1992/10/26  21:04:07  peckham
 * 	OT#4860: loop was not being advanced when normalization failed.
 * 	 This should never happen in the first place, but it is
 * 	 not coded correctly.
 * 	[1992/10/26  21:03:22  peckham]
 * 
 * Revision 1.1.6.3  1992/10/15  19:21:54  m_sawyer
 * 	add display of clearinghouse management tag and make sure we add
 * 	a null management name in CA_InsertCle() if none is specified.
 * 	[1992/10/15  19:18:50  m_sawyer]
 * 
 * Revision 1.1.6.2  1992/09/29  18:27:41  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:09:00  weisman]
 * 
 * Revision 1.1.2.3  1992/05/12  15:06:56  peckham
 * 	CR3401: InsertCle() was not pulling old entry off of lru before
 * 	allocating new entry, making it available for reclamation.
 * 	It can be freed, causing use of possibly corrupted data, and
 * 	then it is released again, corrupting the free memory list.
 * 	[1992/05/12  15:05:33  peckham]
 * 
 * Revision 1.1.2.2  1992/03/22  21:10:42  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:04:07  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: cacle.c
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
#include <uc_clerk.h>

CA_PRIVATE void
CA_bump_cnt (
     CleEntry_t  *,
     int          );

CA_PRIVATE dns_status_t
CA_InsertCle (
     FullName_t      *,
     ObjUID_t        *,
     struct CleFlags *,
     int             ,
     Set_t           *,
     SimpleName_t    *);


/*
 * Toggle OK on a CH, assuming it is not manual.
 * If we fail, it's not a tragedy, so we ignore errors.
 * 
 * in: clearinghouse name
 *     okval - is it to be marked good or bad?
 */
dns_status_t 
CACleSetOk (FullName_t  *cleName,
            int         okval)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  int        idx;
  CleEntry_t *thisCle;
  dns_status_t status = DNS_SUCCESS;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " >CACleSetOk(%s,%d)",
      deb_ascii_FullName(nameBuf,cleName),
      okval));

  idx = CAHashCle(cleName);
  CALockData();

  thisCle = CAFindCle(cleName, idx);
  if (thisCle) {
    if (!thisCle->ch_flags.status.flags.sticky)
      thisCle->ch_flags.status.flags.ok = okval;
  } else
    status = DNS_UNKNOWNENTRY;

  CAUnlockData();

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " <CACleSetOk(%s,%d) return(%s)",
      deb_ascii_FullName(nameBuf,cleName),
      okval,
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}
/* 
 * Return the next clearing house in the CleHash table.
 * Used to get list of ch by adver for ncl 
 * Global lock held by caller.
 *
 * Used by the advertiser.
 * 
 * in: last_cle  - give 0 on first
 * in/out handle: in - this handle, out - next handle
 * returns: cle ptr 
 */
CleEntry_t *
CANextCle (CleEntry_t  *last_cle,
            int         *handle_p)
{
    CacheLinkedList_t *const ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_cle);
    register int handle = *handle_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CANextCle(%p,%d)",
      last_cle,
      handle_p));

    /* Get next pointer in linked list */
    if (last_cle) {
	CleEntry_t *const cle_p =
			CA_GET_CHPOINTER(last_cle->ch_collision.ll_flink);

  	if ((CacheLinkedList_t *)cle_p != (ll_p + handle))  {
  	  DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_cache,
              svc_c_debug6,
              " <CANextCle(%p,%d) return(%p)",
              last_cle,
              handle_p,
	      cle_p));
	    return(cle_p);
        }

	*handle_p = ++handle;
      }

    /* Continue until table exhausted or find a valid entry */
    while (handle < CAfixed_p->ca_cle_hash_len) {
	CleEntry_t *const cle_p =
			CA_GET_CHPOINTER((ll_p + handle)->ll_flink);

  	if ((CacheLinkedList_t *)cle_p != (ll_p + handle)) {
  	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_cache,
                svc_c_debug6,
                " <CANextCle(%p,%d) return(%p)",
                last_cle,
                handle_p,
	        cle_p));
	    return(cle_p);
        }

	*handle_p = ++handle;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug6,
        " <CANextCle(%p,%d) return(%p)",
        last_cle,
        handle_p,
	NULL));
    return(NULL);
}
/*
 * Locate a clearinghouse in the CleHash table
 * Name must be uppercase.
 * Global Lock held by caller.
 *
 * Used by the clerk.
 */
CleEntry_t *
CAFindCle (FullName_t  *name_p,
            int         idx)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  CacheLinkedList_t *const ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_cle)+idx;
  register CleEntry_t *cle_p;
  int len1;
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CAFindCle(%s,%d)",
      deb_ascii_FullName(nameBuf,name_p),
      idx));
  
  len1 = LEN_FullName(name_p);
  
  for (cle_p = CA_GET_CHPOINTER(ll_p->ll_flink);
       cle_p != (CleEntry_t *)ll_p;
       cle_p = CA_GET_CHPOINTER(cle_p->ch_collision.ll_flink)) {
    const int len2 = LEN_FullName(cle_p->ch_name);
    /* Don't bother comparision if lengths aren't equal */
    if ((len1 == len2) && EQ_bytes(name_p, cle_p->ch_name, len1))
      /* Found match, return pointer */
      goto leave_CAFindCle;
  }
  cle_p = NULL;

leave_CAFindCle:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CAFindCle(%s,%d) return(%p)",
      deb_ascii_FullName(nameBuf,name_p),
      idx,
      cle_p));
  return(cle_p);
}

/* Update clearinghouse counter 
 * Global lock held by caller.
 */
CA_PRIVATE void 
CA_bump_cnt (CleEntry_t  *cle_p,
             int         type)
{
  switch (type) {
  case CA_CNT_READ:
    ++cle_p->ch_reads;
    break;
  case CA_CNT_WRITE:
    ++cle_p->ch_writes;
    break;
  case CA_CNT_MISC:
    ++cle_p->ch_miscl;
    break;
  }
}

/*
 * Remove a clearinghouse from the cache.  If unstickonly is true,
 * just add back to lru list and let regular replacement worry about it.
 */
dns_status_t 
CARemoveClearinghouse (FullName_t  *name_p,
                       int         unstickonly)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  FullName_u name;
  dns_status_t status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CARemoveClearinghouse(%s,%d)",
      deb_ascii_FullName(nameBuf,name_p),
      unstickonly));

  /* X.500 - Upcase not sufficient for typed names */
  status = dns_normalize_fname(name_p, (FullName_t *)name, sizeof(FullName_u));
  if (status == DNS_SUCCESS) {
    /* Get hash table index */
    const int idx = CAHashCle((FullName_t *)name);
    register CleEntry_t *cle_p;

    CALockData();
    
    cle_p = CAFindCle((FullName_t *)name, idx);
    if (cle_p) {
      /* Just unstick it and leave it alone */
      if (unstickonly) {
	if (cle_p->ch_flags.status.flags.sticky) {
	  CA_unlock(cle_p);
	  cle_p->ch_flags.status.flags.sticky = 0;
	}
      } else {
	SimpleName_t *const tag_p =
			(SimpleName_t *)SKIP_Set(SKIP_FullName(cle_p->ch_name));

	/* Remove clearinghouse from hash table */
	CA_CollisionRemove((CacheLinkedList_t *)cle_p);
	
	/* Remove clearinghouse from LRU list it is on  */
	CA_LruRemove(cle_p);
	
	/* Add cache block back onto free list */
	CA_FreeListFree(cle_p);

	/*
	 * insure the clearinghouse is removed from the wan file
	 * if it is a cached server.
	 */
	if (EXT8(tag_p->sn_flag) != SN_null) {
	  char name[CDS_STR_SNAME_MAX+1];
	  int name_len = sizeof(name);

	  if (cdsCvtSimpleToStr((cds_simple_name_t *)tag_p,
				(unsigned char *)name, &name_len)
	      == CDS_SUCCESS)
	    CA_delete_server(name);
	}
      }
    } else
      status = DNS_UNKNOWNENTRY;

    CAUnlockData();
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CARemoveClearinghouse(%s,%d) return(%s)",
      deb_ascii_FullName(nameBuf,name_p),
      unstickonly,
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/*
 * Add a clearinghouse to the CleHash table 
 * This routine is a jacket to provide locking for callers
 * outside the cache and to convert names to upper case.
 * Clearinghouse names are converted to upper cache.
 */
dns_status_t 
CAInsertClearinghouse (FullName_t       *name_p,
                       ObjUID_t         *id_p,
                       struct CleFlags  *flags_p,
                       int              cnttype,
                       Set_t            *address_p,
                       SimpleName_t     *mgmt_name_p)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  DEB_ASCII_BUF_ObjUID(idBuf)
  DEB_ASCII_BUF_Set(addressBuf)
  DEB_ASCII_BUF_SimpleName(mgmt_nameBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  FullName_u cle_name;
  dns_status_t status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CAInsertClearinghouse(%s,%s,,%d,%s,%s)",
      deb_ascii_FullName(nameBuf,name_p),
      deb_ascii_ObjUID(idBuf,id_p),
      cnttype,
      deb_ascii_Set(addressBuf,address_p),
      deb_ascii_SimpleName(mgmt_nameBuf,mgmt_name_p)));

  /* X.500 - Upcase not sufficient for typed names */
  status = dns_normalize_fname(name_p, (FullName_t *)cle_name, sizeof(FullName_u));
  if (status == DNS_SUCCESS) {
    CALockData();

    status = CA_InsertCle((FullName_t *)cle_name, id_p, flags_p, cnttype,
              address_p, mgmt_name_p);

    CAUnlockData();
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CAInsertClearinghouse(%s,%s,,%d,%s,%s) return(%s)",
      deb_ascii_FullName(nameBuf,name_p),
      deb_ascii_ObjUID(idBuf,id_p),
      cnttype,
      deb_ascii_Set(addressBuf,address_p),
      deb_ascii_SimpleName(mgmt_nameBuf,mgmt_name_p),
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/*
 * Add a clearinghouse to the CleHash table 
 * Clearinghouse name already in upper case and the
 * Global lock is held by the caller.
 * Clearinghouses as added when the progress record is disected
 * and after one has been successfully contacted.  The addressused
 * bit of the flags indicates which is the case.  If the address was
 * used, a counter must be updated. 
 */
CA_PRIVATE dns_status_t 
CA_InsertCle (FullName_t       *name_p,
              ObjUID_t         *id_p,
              struct CleFlags  *flags_p,
              int              cnttype,
              Set_t            *address_p,
              SimpleName_t     *mgmt_name_p)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  DEB_ASCII_BUF_ObjUID(idBuf)
  DEB_ASCII_BUF_Set(addressBuf)
  DEB_ASCII_BUF_SimpleName(mgmt_nameBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  register CleEntry_t *cache_p,*cle_p;
  SimpleName_t *tag_p = mgmt_name_p ? mgmt_name_p : (SimpleName_t *)NullSimpleName;
  int idx;
  dns_status_t status = DNS_SUCCESS;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CA_InsertCle(%s,%s,,%d,%s,%s)",
      deb_ascii_FullName(nameBuf,name_p),
      deb_ascii_ObjUID(idBuf,id_p),
      cnttype,
      deb_ascii_Set(addressBuf,address_p),
      deb_ascii_SimpleName(mgmt_nameBuf,mgmt_name_p)));

  /* Get hash table index */
  idx = CAHashCle(name_p);

  cache_p = CAFindCle(name_p, idx);
  /* If this clearinghouse is already cached, update in place */
  if (cache_p) {
    if (EQ_ObjUID(id_p, cache_p->ch_id)) {
      Set_t *const set_p = (Set_t *)SKIP_FullName(cache_p->ch_name);

      /* if the status of the sticky bit changes, change the lock also */
      if (!cache_p->ch_flags.status.flags.sticky
	  && flags_p->status.flags.sticky) {
	/* lock this entry */
	CA_lock(cache_p);
      }

      /* Update flags */
      cache_p->ch_flags.status.init = cache_p->ch_flags.status.init |
	flags_p->status.init;

      /*
       * If the cached set has been used and the input has not, don't
       * change what is in the cache
       */
      if (cache_p->ch_flags.status.flags.addressused &&
	  !flags_p->status.flags.addressused) goto leave_CA_InsertCle;

      /* Check if set needs to be updated */
      if (SetsEqual(set_p, address_p)) {
	/* If this address was just used, move to front of LRU list */
	if (flags_p->status.flags.addressused) {
	  CALruMove(cache_p);
	  CA_bump_cnt(cache_p, cnttype);
	}
	goto leave_CA_InsertCle;
      }
    }
    /* Remove old clearinghouse from LRU list it on it */
    CA_LruRemove(cache_p);
  }

  /* Allocate memory for new entry */
  cle_p =
      (CleEntry_t *)CA_GlobalAlloc(sizeof(*cle_p)
			   - sizeof(cle_p->ch_name) + LEN_FullName(name_p)
			   - sizeof(cle_p->ch_set) + LEN_Set(address_p)
			   - sizeof(cle_p->ch_mgmt_name)
                               + (mgmt_name_p ?
                                  LEN_SimpleName(mgmt_name_p) :
                                  LEN_SimpleName(NullSimpleName))
                           , CHEntry);
  if (cle_p) {
    Set_t *set_p;

    /* If allocating new memory cause set changed, keep old mgmt stuff */
    if (cache_p && EQ_ObjUID(id_p, cache_p->ch_id)) {
      CA_copy_lock(cache_p, cle_p);
      /* Build clearinghouse  */
      cle_p->ch_flags = cache_p->ch_flags;
      cle_p->ch_reads = cache_p->ch_reads;
      cle_p->ch_writes = cache_p->ch_writes;
      cle_p->ch_miscl = cache_p->ch_miscl;
      COPY_bytes(&cache_p->ch_createuid, &cle_p->ch_createuid, 
		 sizeof(cle_p->ch_createuid));
      COPY_bytes(&cache_p->ch_createtime, &cle_p->ch_createtime,
		 sizeof(cle_p->ch_createtime));
      if (EXT8(tag_p->sn_flag) == SN_null)
	tag_p = (SimpleName_t *)SKIP_Set(SKIP_FullName(cache_p->ch_name));
    } else {
      error_status_t uuid_status;

      if (flags_p->status.flags.sticky)
	/* lock this entry */
	CA_lock(cle_p);

      cle_p->ch_flags = *flags_p;
      cle_p->ch_reads = cle_p->ch_writes = cle_p->ch_miscl = 0;
      (void)utc_gettime(&cle_p->ch_createtime);
      uuid_create(&cle_p->ch_createuid, &uuid_status);
    }

    CA_bump_cnt(cle_p, cnttype);
    COPY_ObjUID(id_p, cle_p->ch_id);
    COPY_FullName(name_p, cle_p->ch_name);
    set_p = (Set_t *)SKIP_FullName(cle_p->ch_name);
    COPY_Set(address_p, set_p);
    COPY_SimpleName(tag_p, SKIP_Set(set_p));

    /* Add to hash table */
    CA_CollisionInsert(CA_GET_LLPOINTER(CAfixed_p->ca_cle) + idx,
		      (CacheLinkedList_t *)cle_p);

    /* Put on front of LRU list */
    CA_LruInsert(cle_p);

    /* If a cached clearinghouse, push it to the wan file */
    if (!cache_p && (EXT8(tag_p->sn_flag) != SN_null))
      CA_write_servers();

  } else
    status = DNS_NONSRESOURCES;

  if (cache_p) {
    /* Remove old clearinghouse from hash table */
    CA_CollisionRemove((CacheLinkedList_t *)cache_p);

    /* Add cache block back onto free list */
    CA_FreeListFree(cache_p);
  }

leave_CA_InsertCle:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CA_InsertCle(%s,%s,,%d,%s,%s) return(%s)",
      deb_ascii_FullName(nameBuf,name_p),
      deb_ascii_ObjUID(idBuf,id_p),
      cnttype,
      deb_ascii_Set(addressBuf,address_p),
      deb_ascii_SimpleName(mgmt_nameBuf,mgmt_name_p),
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/*
 * Locate a clearinghouse.  Return a copy of the cache entry to the caller.
 * Copy will be placed in malloced memory and the caller must free it.
 * the collision pointers should not be accessed.
 * Clearinghouse name is already upper case.
 */
CleEntry_t *
CALookupClearinghouse (FullName_t *name_p)
{
    return CA_LookupClearinghouse(name_p, NULL);
}

CleEntry_t *
CA_LookupClearinghouse (FullName_t    *name_p,
                         dns_status_t  *status_p)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  register CleEntry_t *cle_p = NULL, *cache_p;
  FullName_u nname;
  dns_status_t status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CA_LookupClearinghouse(%s,%p)",
      deb_ascii_FullName(nameBuf,name_p),
      status_p));
  
  status = dns_normalize_fname(name_p, (FullName_t *)nname, sizeof(FullName_u));
  if (status == DNS_SUCCESS) {
    /* hash name */
    const int idx = CAHashCle((FullName_t *)nname);
    
    /* Get locks */
    CALockData();
    
    /* check collision list */
    cache_p = CAFindCle((FullName_t *)nname, idx);
    if (cache_p) {
      /* Get memory to hold copy of cache entry */
      const int len = sizeof(*cle_p)
	- sizeof(cle_p->ch_name) + LEN_FullName(cache_p->ch_name)
	- sizeof(cle_p->ch_set) + LEN_Set(SKIP_FullName(cache_p->ch_name));

      CALruMove(cache_p);	/* Move entry to front of LRU list */
      
      /* Get user buffer and copy cached data */
      cle_p = (CleEntry_t *)dns_malloc(len);
      if (cle_p)  {
	COPY_bytes(cache_p, cle_p, len);	
      } else
	status = DNS_NONSRESOURCES;
    } else
      status = DNS_UNKNOWNENTRY;

    /* Release locks */
    CAUnlockData();
  }

  if (status_p)
    *status_p = status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CA_LookupClearinghouse(%s,%p->%s) return(%p)",
      deb_ascii_FullName(nameBuf,name_p),
      status_p,
      deb_ascii_dns_status(statusBuf,status),
      cle_p));
  return(cle_p);
}

/*
 * External cache routine which assumes no locks on global cache held.
 * Add a set of clearinghouse to the clearinghouse cache.
 * Convert names to lower case in input set and new cache members.
 */
dns_status_t 
CAClearinghouseSet (Set_t *set_p)
{
  dns_status_t status;

  CALockData();
  status = CA_ClearinghouseSet(set_p);
  CAUnlockData();

  return(status);
}

/*
 * Internal routine assuming global locks already held.
 * Add a set of clearinghouse to the clearinghouse cache.
 * The address are considered hints for now.
 * Convert names to lower case in input set and new cache members.
 */
dns_status_t 
CA_ClearinghouseSet (Set_t *set_p)
{
  DEB_ASCII_BUF_Set(setBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  FullName_u cle_name;
  int numzy;
  dns_status_t status = DNS_SUCCESS;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CA_ClearinghouseSet(%s)",
      deb_ascii_Set(setBuf,set_p)));

  for (numzy=0; numzy<NUM_Set(set_p); ++numzy) {
    DEB_ASCII_BUF_FullName(nameBuf)
    MemberValue_t *const mv_p = (MemberValue_t *)INDEX_Set(set_p, numzy);
    ReplicaPointer_t *const rp_p = (ReplicaPointer_t *)mv_p->mv_value;
    ReplicaType_t *const rt_p = (ReplicaType_t *)SKIP_Set(rp_p->rp_addressHint);
    FullName_t *CHName_p = (FullName_t *)SKIP_ReplicaType(rt_p);
    dns_status_t lstatus;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug6,
        "  CA_ClearinghouseSet(%s) clearinghouse(%s)",
        deb_ascii_Set(setBuf,set_p),
        deb_ascii_FullName(nameBuf,CHName_p)));

    /* X.500 normalize clearinghouse name */
    lstatus = dns_normalize_fname(CHName_p, (FullName_t *)cle_name,
				  sizeof(FullName_u));
    if (lstatus == DNS_SUCCESS) {
      struct CleFlags flags;
      /*
       * Add clearinghouse to cache. Indicate that address has not been
       * used.
       */
      flags.status.init = 0;
      if (EXT8(rt_p) == RT_gda) flags.status.flags.gda = 1;

      lstatus = CA_InsertCle((FullName_t *)cle_name, (ObjUID_t *)rp_p->rp_CHID,
		&flags, CA_CNT_NONE, (Set_t *)rp_p->rp_addressHint,
                NULL);
    }
    if (lstatus != DNS_SUCCESS)
      status = lstatus;
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CA_ClearinghouseSet(%s) return(%s)",
      deb_ascii_Set(setBuf,set_p),
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/*
 * Add a set of clearinghouse to the clearinghouse cache.
 * The address were used, names are in upper case.
 */
dns_status_t 
CAClearinghousesUsed (CleUsed_t *cle_p)
{
  DEB_ASCII_BUF_dns_status(statusBuf)
  struct CleFlags flags;
  dns_status_t status = DNS_SUCCESS;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CAClearinghousesUsed()"));

  flags.status.init = 0;
  flags.status.flags.addressused = 1;
  flags.status.flags.ok = 1;

  CALockData();

  while (cle_p) {
    DEB_ASCII_BUF_FullName(nameBuf)
    FullName_u cle_name;
    CleUsed_t *const next_p  = cle_p->next_p;
    dns_status_t lstatus;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug6,
        "  CAClearinghousesUsed() clearinghouse(%s)",
        deb_ascii_FullName(nameBuf,&cle_p->name)));

    /* X.500 normalize clearinghouse name */
    lstatus = dns_normalize_fname(&cle_p->name, (FullName_t *)cle_name,
				  sizeof(FullName_u));
    if (lstatus == DNS_SUCCESS)
      lstatus = CA_InsertCle((FullName_t *)cle_name, &cle_p->id, &flags,
		cle_p->cnttype, &cle_p->address, NULL);

    if (lstatus != DNS_SUCCESS)
      status = lstatus;
    dns_free((char *)cle_p);
    cle_p = next_p;
  }
  CAUnlockData();

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CAClearinghousesUsed() return(%s)",
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}



