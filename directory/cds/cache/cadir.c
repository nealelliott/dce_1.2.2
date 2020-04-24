/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cadir.c,v $
 * Revision 1.1.9.2  1996/02/18  19:23:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:00  marty]
 *
 * Revision 1.1.9.1  1995/12/08  00:10:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:28  root]
 * 
 * Revision 1.1.6.5  1994/08/16  17:27:10  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockGlobal() by CALockData(),
 * 	CAUnlockGlobal() by CAUnlockData().
 * 	[1994/08/16  15:29:51  peckham]
 * 
 * Revision 1.1.6.4  1994/06/23  18:28:19  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:15:20  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  16:07:03  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:38  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:37:53  peckham
 * 	Remove UNIX and VMS conditionals.
 * 	[1994/04/11  17:33:49  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:55:34  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:47:03  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:36:30  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:19:28  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:11:17  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:04:20  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * module: cadir.c
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
#include <dnsclerk.h>
#include <sys/time.h>
#include <dce/dce_cf.h>
#include <string.h>

/*
 * Locate a directory in the DirHash table
 * Name must be in upper case.
 * Global Lock held by caller.
 *
 * Used by the clerk.
 */
DirEntry_t *
CAFindDir (FullName_t  *name_p,
            int         idx)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  CacheLinkedList_t *const ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_dir)+idx;
  const int len1 = LEN_FullName(name_p);
  register DirEntry_t *dir_p;
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CAFindDir(%s,%d)",
      deb_ascii_FullName(nameBuf,name_p),
      idx));

  
  for (dir_p = CA_GET_DHPOINTER(ll_p->ll_flink);
       dir_p != (DirEntry_t *)ll_p;
       dir_p = CA_GET_DHPOINTER(dir_p->dh_collision.ll_flink)) {
    const int len2 = LEN_FullName(dir_p->dh_name);
    /* Don't bother comparision if lengths aren't equal */
    if ((len1 == len2) && EQ_bytes(name_p, dir_p->dh_name, len1))
      /* Found match, return pointer */
      goto leave_CAFindDir;
  }
  dir_p = NULL;

leave_CAFindDir:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CAFindDir(%s,%d) return(%p)",
      deb_ascii_FullName(nameBuf,name_p),
      idx,
      dir_p));
  return(dir_p);
}

/*
 * Return the entry for the cell root directory.
 */
DirEntry_t *
CAFindCell (void)
{
    register const int idx = CAfixed_p->ca_celldir;

    if (idx)
	return(CA_GET_DHPOINTER(idx));

    return(NULL);
}

/*
 * Add a directory to the DirHash table 
 * The directory name is converted to upper case.  
 * Clearinghouse names in the replica set are left as is. 
 */
dns_status_t 
CAInsertDirectory (FullName_t  *name_p,
                   ObjUID_t    *id_p,
                   Set_t       *rpset_p)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  DEB_ASCII_BUF_ObjUID(idBuf)
  DEB_ASCII_BUF_Set(rpsetBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  FullName_u dirname;
  dns_status_t status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CAInsertDirectory(%s,%s,%s)",
      deb_ascii_FullName(nameBuf,name_p),
      deb_ascii_ObjUID(idBuf,id_p),
      deb_ascii_Set(rpsetBuf,rpset_p)));

  /* X.500 normalize name */
  status = dns_normalize_fname(name_p, (FullName_t *)dirname, sizeof(FullName_u));
  if (status == DNS_SUCCESS) {
    register DirEntry_t *cache_p, *dir_p;
    int correct = FALSE;	/* more efficient, more obscure */

    /* See if entry exists */
    const int idx = CAHashDir((FullName_t *)dirname);

    CALockData();

    cache_p = CAFindDir((FullName_t *)dirname, idx);
    if (cache_p) {
      /* directory already cached */
      /* Check if set needs to be updated, assume rpset more current */
      Set_t *const set_p = (Set_t *)SKIP_FullName(cache_p->dh_name);

      if (SetsEqual(set_p, rpset_p)) {
	/* Already cached, nothing else to do */
	CALruMove(cache_p);	/* Move entry to front of LRU list */ 
	correct = TRUE;
      } else {
	/* Remove old directory from LRU list */
	CA_LruRemove(cache_p);
      }
    }

    if (!correct) {
      /* Allocate memory for new entry */
      dir_p = (DirEntry_t *)CA_GlobalAlloc(sizeof(*dir_p)
		  - sizeof(dir_p->dh_name) + LEN_FullName(dirname)
		  - sizeof(dir_p->dh_rpset) + LEN_Set(rpset_p), DHEntry);
      if (dir_p) {
	Set_t *set_p;

	/* Build directory */
	COPY_ObjUID(id_p, dir_p->dh_id);
	COPY_FullName(dirname, dir_p->dh_name);
	set_p = (Set_t *)SKIP_FullName(dir_p->dh_name);

	COPY_Set(rpset_p, set_p);

	if (cache_p) {
	  CA_copy_lock(cache_p, dir_p);

	  if (CA_GET_OFFSET(cache_p) == CAfixed_p->ca_celldir)
	    CAfixed_p->ca_celldir = CA_GET_OFFSET(dir_p);

	  /* Remove old directory from hash table */
	  CA_CollisionRemove((CacheLinkedList_t *)cache_p);
	  
	  /* Add cache block back onto free list */
	  CA_FreeListFree(cache_p);
	}

	/* Add to hash table */
	CA_CollisionInsert(CA_GET_LLPOINTER(CAfixed_p->ca_dir) + idx, 
			  (CacheLinkedList_t  *)dir_p);

	/* Put on front of LRU list */
	CA_LruInsert(dir_p);
      } else {
	if (cache_p)
	  CA_LruInsert(cache_p);

	status = DNS_NONSRESOURCES;
      }
    }

    CAUnlockData();
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CAInsertDirectory(%s,%s,%s) return(%s)",
      deb_ascii_FullName(nameBuf,name_p),
      deb_ascii_ObjUID(idBuf,id_p),
      deb_ascii_Set(rpsetBuf,rpset_p),
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/*
 * Locate a directory.  Return a copy of the cache entry to the caller.
 * Copy will be placed in malloced memory and the caller must free it.
 * the collision pointers should not be accessed.
 */
DirEntry_t *
CALookupDirectory (FullName_t *name_p)
{
    return CA_LookupDirectory(name_p, NULL);
}

DirEntry_t *
CA_LookupDirectory (FullName_t    *name_p,
                     dns_status_t  *status_p)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  register DirEntry_t *dir_p = NULL, *cache_p;
  FullName_u dirname;
  dns_status_t status;
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CA_LookupDirectory(%s,%p)",
      deb_ascii_FullName(nameBuf,name_p),
      status_p));

  /* X.500 normalize name */
  status = dns_normalize_fname(name_p,
		(FullName_t *)dirname, sizeof(FullName_u));
  if (status == DNS_SUCCESS) {
    const int idx = CAHashDir((FullName_t *)dirname);	/* hash name */
  
    /* Get locks */
    CALockData();
  
    /* check collision list */
    cache_p = CAFindDir((FullName_t *)dirname, idx);
    if (cache_p) {
      /* Get memory to hold copy of cache entry */
      const int len = sizeof(*dir_p)
	  - sizeof(dir_p->dh_name) + LEN_FullName(cache_p->dh_name)
	  - sizeof(dir_p->dh_rpset) + LEN_Set(SKIP_FullName(cache_p->dh_name));

      CALruMove(cache_p);	/* Move entry to front of LRU list */
    
      /* Get user buffer and copy cached data */
      dir_p = (DirEntry_t *)dns_malloc(len);
      if (dir_p) {
        COPY_bytes(cache_p, dir_p, len);	
      } else
	status = DNS_NONSRESOURCES;
    } else
      status = DNS_UNKNOWNENTRY;
  
    /* Release locks */
    CAUnlockData();
  } else
    status = status;

  if (status_p)
    *status_p = status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CA_LookupDirectory(%s,%p->%s) return(%p)",
      deb_ascii_FullName(nameBuf,name_p),
      status_p,
      deb_ascii_dns_status(statusBuf,status),
      dir_p));
  return(dir_p);
}

/*
 * Remove a replica from the set of replicas stored with a directory.
 * The replica member will be marked absent in the set.
 * The input replica set member should be in the same as was
 * passed out from LookupDirectory.  Matching will be done by 
 * clearinghouse cts.
 */
dns_status_t 
CARemoveReplica (FullName_t  *name_p,
                 ObjUID_t    *id_p)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  DirEntry_t *dir_p;
  ReplicaPointer_t *rp_p;
  FullName_u dirname;
  register int i;
  dns_status_t status;
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CARemoveReplica(%s,)",
      deb_ascii_FullName(nameBuf,name_p)));

  /* X.500 normalize directory and clearinghouse name for lookup */
  status = dns_normalize_fname(name_p,
		(FullName_t *)dirname, sizeof(FullName_u));
  if (status == DNS_SUCCESS) {
    const int idx = CAHashDir((FullName_t *)dirname);

    /* Get locks */
    CALockData();
    
    /* Find cached directory, return if not in cache */
    dir_p = CAFindDir((FullName_t *)dirname, idx);
    if (dir_p) {
      /* Get pointer to cached replica set */
      Set_t *const set_p = (Set_t *)SKIP_FullName(dir_p->dh_name);
      
      /* If no members are left present, remove the entire replica. */
      int  found = FALSE;

      /* Search each member for a match on cts */
      for (i=0; i<NUM_Set(set_p); ++i) {
	register MemberValue_t *const sm_p =
			(MemberValue_t *)INDEX_Set(set_p, i);

	if (EXT8(sm_p->mv_valuetype) != VT_ReplicaPointer) continue;
	if (FLAG_SET(sm_p->mv_flag, MH_present)) {
	  rp_p = (ReplicaPointer_t *)sm_p->mv_value;
	  if (EQ_ObjUID(rp_p->rp_CHID, id_p)) {
	    /* Mark value absent, so not used again.  */
	    CLEAR_FLAG(sm_p->mv_flag, MH_present);
	    if (found) break;
	  }
	  else
	    found = TRUE;
	}
      }
      /* If no members left remove the entire replica */
      if (!found) {
	CA_LruRemove(dir_p);

	if (CA_GET_OFFSET(dir_p) == CAfixed_p->ca_celldir)
	  CAfixed_p->ca_celldir = 0;

	CA_CollisionRemove(&dir_p->dh_collision);
	CA_FreeListFree(dir_p);
      }
    } else
      status = DNS_UNKNOWNENTRY;

    CAUnlockData();
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CARemoveReplica(%s,) return(%s)",
      deb_ascii_FullName(nameBuf,name_p),
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/*
 * Merge a replica set to the set of replicas stored with a directory.
 * If there is no directory entry, one will be created with the
 * given replicas.  Otherwise the set is searched by CHCTS for each
 * replica and appended if not found.  If any replica is already
 * in the set and the ReplicaPointers do not match, the one in the set
 * is updated.
 *
 * Bookeeping for the merge is handled by vectors of pointers to
 * MemberValues.  There is one for the current set, and one for the new
 * set being merged in.  If the corresponding MemberValue is to be used
 * in the final product, a pointer to it is placed in the vector.
 * During the merge, members of rpset_p are modified, but restored
 * before the merge is completed.  If the replica set is modified,
 * it is also pruned of deleted replicas.
 *
 * The current replace strategy here is to allocate a new DirEntry_t
 * before tossing the old one, presuming the old one is better than none
 * at all.  This is not consistent with other cache code.
 */
dns_status_t 
CAAddReplicas (FullName_t  *name_p,
               ObjUID_t    *id_p,
               Set_t       *rpset_p,
               int         root)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  DEB_ASCII_BUF_ObjUID(idBuf)
  DEB_ASCII_BUF_ObjUID(chuidBuf)
  DEB_ASCII_BUF_FullName(chnameBuf)
  DEB_ASCII_BUF_Set(rpsetBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  DirEntry_t *dir_p;
  FullName_u dirname;
  dns_status_t status;
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CAAddReplicas(%s,%s,%s,%d)",
      deb_ascii_FullName(nameBuf,name_p),
      deb_ascii_ObjUID(idBuf,id_p),
      deb_ascii_Set(rpsetBuf,rpset_p),
      root));

  /* X.500 normalize directory and clearinghouse name for lookup */
  status = dns_normalize_fname(name_p,
		(FullName_t *)dirname, sizeof(FullName_u));
  if (status == DNS_SUCCESS) {
    const int idx = CAHashDir((FullName_t *)dirname);

    /* Get locks */
    CALockData();
    
    /* Find cached directory, or create one */
    dir_p = CAFindDir((FullName_t *)dirname, idx);
    if (dir_p) {
      MemberValue_t *sm[20];
      Set_t *const set_p = (Set_t *)SKIP_FullName(dir_p->dh_name);
      const int idir = CA_GET_OFFSET(dir_p);
      const int num_set = NUM_Set(set_p);
      const int num_rpset = NUM_Set(rpset_p);
      MemberValue_t **sm_pp = sm;
      int size = (num_set+num_rpset)*sizeof(*sm_pp);
      int num = 0;

      /*
       * If this has become a root, lock it down.
       */
      if (root && (idir != CAfixed_p->ca_celldir)) {
	/*
	 * Extra credit: bump out old cell.
	 */
	if (CAfixed_p->ca_celldir) {
	  register DirEntry_t *const dh_p =
				CA_GET_DHPOINTER(CAfixed_p->ca_celldir);

	  CA_unlock(dh_p);
	}

	CA_lock(dir_p);
	CAfixed_p->ca_celldir = idir;
      }

      /* Use the stack and avoid malloc() overhead where reasonable */
      if (sizeof(sm) < size)
	sm_pp = (MemberValue_t **)dns_malloc(size);

      if (sm_pp) {
	MemberValue_t **rpsm_pp = &sm_pp[num_set];
	int changed = FALSE;
	register int i, j;

	ZERO_bytes(sm_pp, size);
	size = 0;

	/* Search each member for a match on cts */
	for (i = 0; i < num_set; i++) {
	  register MemberValue_t *setv_p = (MemberValue_t *)INDEX_Set(set_p, i);
	  ReplicaPointer_t *setrp_p = (ReplicaPointer_t *)setv_p->mv_value;

	  if (FLAG_IS_CLEAR(setv_p->mv_flag, MH_present)) continue;
	  if (EXT8(setv_p->mv_valuetype) != VT_ReplicaPointer) continue;
	  /* Presume that the original is unchanged */
	  sm_pp[i] = setv_p;
	  size += LEN_MemberValue(setv_p);
	  num++;

	  /* Search each member for a match on cts */
	  for (j = 0; j < num_rpset; j++) {
	    register MemberValue_t *rpsetv_p =
					(MemberValue_t *)INDEX_Set(rpset_p, j);
	    ReplicaPointer_t *rpsetrp_p =
					(ReplicaPointer_t *)rpsetv_p->mv_value;

	    if (FLAG_IS_CLEAR(rpsetv_p->mv_flag, MH_present)) continue;
	    if (EXT8(rpsetv_p->mv_valuetype) != VT_ReplicaPointer) continue;
	    if (NE_ObjUID(setrp_p->rp_CHID, rpsetrp_p->rp_CHID)) continue;

	    /* signal that this replica is being used */
	    CLEAR_FLAG(rpsetv_p->mv_flag, MH_present);
	    rpsm_pp[j] = setv_p;

	    if (NE_bytes(setrp_p->rp_addressHint,
			rpsetrp_p->rp_addressHint,
			LEN_Set(rpsetrp_p->rp_addressHint))) {
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_cache,
                svc_c_debug4,
                "  CAAddReplicas(%s,%s,%s,%d) replacing (%s:%s)",
                deb_ascii_FullName(nameBuf,name_p),
                deb_ascii_ObjUID(idBuf,id_p),
		deb_ascii_Set(rpsetBuf,rpset_p),
                root,
                deb_ascii_ObjUID(chuidBuf,setrp_p->rp_CHID),
                deb_ascii_FullName(chnameBuf,FLD_ReplicaPointer_CHName(setrp_p))));
	      /* don't use the original ReplicaPointer */
	      sm_pp[i] = NULL;
	      size -= LEN_MemberValue(setv_p);

	      /* Use this new ReplicaPointer */
	      rpsm_pp[j] = rpsetv_p;
	      size += LEN_MemberValue(rpsetv_p);

	      changed = TRUE;
	    }
	    break;
	  }
	}

	/*
	 * Any replicas which are not accounted for in the original set
	 * get picked up here.
	 */
	for (j = 0; j < num_rpset; j++) {
	  register MemberValue_t *rpsetv_p =
				      (MemberValue_t *)INDEX_Set(rpset_p, j);

	  if (FLAG_IS_CLEAR(rpsetv_p->mv_flag, MH_present)) {
	    /* Recover any replicas found in the original set */
	    if (rpsm_pp[j]) {
	      SET_FLAG(rpsetv_p->mv_flag, MH_present);
	      if (rpsm_pp[j] != rpsetv_p) rpsm_pp[j] = NULL;
	    }
	    continue;
	  }
	  if (EXT8(rpsetv_p->mv_valuetype) != VT_ReplicaPointer) continue;
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_cache,
            svc_c_debug4,
            "  CAAddReplicas(%s,%s,%s,%d) adding (%s:%s)",
            deb_ascii_FullName(nameBuf,name_p),
            deb_ascii_ObjUID(idBuf,id_p),
	    deb_ascii_Set(rpsetBuf,rpset_p),
            root,
            deb_ascii_ObjUID(chuidBuf,rpsetv_p->mv_value),
            deb_ascii_FullName(chnameBuf,FLD_ReplicaPointer_CHName(rpsetv_p->mv_value))));
	  /* Use this new ReplicaPointer */
	  rpsm_pp[j] = rpsetv_p;
	  size += LEN_MemberValue(rpsetv_p);
	  num++;
	  changed = TRUE;
	}

	if (changed) {
	  MemberValue_t *mv_p;
	  Set_t *newset_p;
	  DirEntry_t *newdir_p;

	  /* Protect ourselves from reclamation */
	  /* Remove old directory from LRU list */
	  CA_LruRemove(dir_p);

	  newdir_p = (DirEntry_t *)CA_GlobalAlloc(sizeof(*newdir_p)
		    - sizeof(newdir_p->dh_name) + LEN_FullName(dir_p->dh_name)
		    - sizeof(newdir_p->dh_rpset) + (sizeof(*newset_p)
			+ (num*sizeof(newset_p->st_offsets)) + size), DHEntry);
	  if (newdir_p) {
	    COPY_ObjUID(dir_p->dh_id, newdir_p->dh_id);
	    COPY_FullName(dir_p->dh_name, newdir_p->dh_name);
	    newset_p = (Set_t *)SKIP_FullName(newdir_p->dh_name);
	    INS8(newset_p->st_flag, ST_present);
	    INS16(newset_p->st_offsetLength, num*sizeof(newset_p->st_offsets));
	    INS16(newset_p->st_valueLength, size);
	    mv_p = (MemberValue_t *)DATA_Set(newset_p);
	    j = 0;
	    for (i = 0; i < num_set; i++)
	      if (sm_pp[i]) {
		INS16(OFFSET_Set(newset_p, j), DIFF_bytes(mv_p, newset_p));
		j++;
		COPY_MemberValue(sm_pp[i], mv_p);
		mv_p = (MemberValue_t *)SKIP_MemberValue(mv_p);
	      }
	    for (i = 0; i < num_rpset; i++)
	      if (rpsm_pp[i]) {
		INS16(OFFSET_Set(newset_p, j), DIFF_bytes(mv_p, newset_p));
		j++;
		COPY_MemberValue(rpsm_pp[i], mv_p);
		mv_p = (MemberValue_t *)SKIP_MemberValue(mv_p);
	      }

	    CA_copy_lock(dir_p, newdir_p);

	    if (idir == CAfixed_p->ca_celldir)
	      CAfixed_p->ca_celldir = CA_GET_OFFSET(newdir_p);

	    /* Remove old directory from hash table */
	    CA_CollisionRemove((CacheLinkedList_t *)dir_p);

	    /* Add cache block back onto free list */
	    CA_FreeListFree(dir_p);

	    /* Add to hash table */
	    CA_CollisionInsert(CA_GET_LLPOINTER(CAfixed_p->ca_dir) + idx,
				(CacheLinkedList_t  *)newdir_p);

	    /* Put on front of LRU list */
	    CA_LruInsert(newdir_p);
	  } else {
	    /* Put old one back on front of LRU list */
	    CA_LruInsert(dir_p);

	    status = DNS_NONSRESOURCES;
	  }
	}

	if (sm_pp != sm) dns_free((char *)sm_pp);
      } else
	status = DNS_NONSRESOURCES;
    } else {
      /* Allocate memory for new entry */
      dir_p = (DirEntry_t *)CA_GlobalAlloc(sizeof(*dir_p)
		- sizeof(dir_p->dh_name) + LEN_FullName(dirname)
		- sizeof(dir_p->dh_rpset) + LEN_Set(rpset_p), DHEntry);
      if (dir_p) {
	/* Build directory */
	COPY_ObjUID(id_p, dir_p->dh_id);
	COPY_FullName(dirname, dir_p->dh_name);
	COPY_Set(rpset_p, SKIP_FullName(dir_p->dh_name));

	/*
	 * Dismiss old cell if there is one, and lock down new one.
	 */
	if (root) {
	  if (CAfixed_p->ca_celldir) {
	    register DirEntry_t *const dh_p =
					CA_GET_DHPOINTER(CAfixed_p->ca_celldir);

	    CA_unlock(dh_p);
	  }
	  CAfixed_p->ca_celldir = CA_GET_OFFSET(dir_p);
	  CA_lock(dir_p);
	}

	/* Add to hash table */
	CA_CollisionInsert(CA_GET_LLPOINTER(CAfixed_p->ca_dir) + idx, 
			  (CacheLinkedList_t  *)dir_p);
	
	/* Put on front of LRU list */
	CA_LruInsert(dir_p);
      } else
	status = DNS_NONSRESOURCES;
    }
    CAUnlockData();
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CAAddReplicas(%s,%s,%s,%d) return(%s)",
      deb_ascii_FullName(nameBuf,name_p),
      deb_ascii_ObjUID(idBuf,id_p),
      deb_ascii_Set(rpsetBuf,rpset_p),
      root,
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}
