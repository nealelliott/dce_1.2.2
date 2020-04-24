/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: caname.c,v $
 * Revision 1.1.7.2  1996/02/18  19:24:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:16  marty]
 *
 * Revision 1.1.7.1  1995/12/08  00:10:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:40  root]
 * 
 * Revision 1.1.4.5  1994/08/16  17:27:16  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockClient() by CALockData(),
 * 	CAUnlockClient() by CAUnlockData(),
 * 	CA_ClientLocked() by CA_DataLocked().
 * 	[1994/08/16  15:30:36  peckham]
 * 
 * Revision 1.1.4.4  1994/06/23  18:28:31  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:46:16  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  16:07:15  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:50  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:38:06  peckham
 * 	Remove VMS and UNIX conditionals.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Use CA_snapshot_cache() instead of CA_abort().
 * 	[1994/04/11  17:52:32  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:55:58  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:48:32  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:37:23  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:20:46  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:24:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: caname.c
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
#include <dnsclerk.h>

CA_PRIVATE NameEntry_t *
CA_FindName (
     FullName_t  *,
     EntryType_t *,
     int          );

CA_PRIVATE AttrEntry_t *
CA_FindAttrEntry (
    token_t	*);

     
/*
 * Find a name in the collision list.  
 * Name in upper case and locks already held. 
 * Client lock held by caller.
 */
CA_PRIVATE NameEntry_t *
CA_FindName (FullName_t   *entryname_p,
              EntryType_t  *type_p,
              int          idx)
{
  DEB_ASCII_BUF_FullName(entrynameBuf)
  DEB_ASCII_BUF_EntryType(typeBuf)
  CacheLinkedList_t *const ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_name)+idx;
  const int len = LEN_FullName(entryname_p);
  const int type = EXT8(type_p);
  register NameEntry_t *name_p;
   
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CA_FindName(%s,%s,%d)",
      deb_ascii_FullName(entrynameBuf,entryname_p),
      deb_ascii_EntryType(typeBuf,type_p),
      idx));
  
  /* Search linked list for a match */
  for (name_p = CA_GET_NHPOINTER(ll_p->ll_flink);
       name_p != (NameEntry_t *)ll_p;
       name_p = CA_GET_NHPOINTER(name_p->nh_collision.ll_flink)) 
  {
    /* Make sure types match first */
#ifndef ET_dirOrObj
    if (type != EXT8(name_p->nh_type)) continue;
#else
    switch (type) 
    {
    case ET_dirOrObj:
      if (EXT8(name_p->nh_type) == ET_directory) break;
      if (EXT8(name_p->nh_type) == ET_object) break;
      continue;
    default:
      if (type != EXT8(name_p->nh_type)) continue;
    }
#endif
    /* And lengths */
    if (len != LEN_FullName(name_p->nh_name)) continue;
    /* Finally check names */
    if (EQ_bytes(entryname_p, name_p->nh_name, len))
      goto leave_CA_FindName;
  }
  name_p = NULL;

leave_CA_FindName:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CA_FindName(%s,%s,%d) return(%p)",
      deb_ascii_FullName(entrynameBuf,entryname_p),
      deb_ascii_EntryType(typeBuf,type_p),
      idx,
      name_p));
  return(name_p);
}

/*
 * Lookup an entry fullname in the name hash table.
 * This routine is only called from within the cache, not by the clerk itself.
 * Client lock held by caller.
 * The entry is moved to the front of the LRU list.
 */
NameEntry_t *
CA_LookupName (FullName_t   *entryname_p,
                EntryType_t  *type_p)
{
  DEB_ASCII_BUF_FullName(entrynameBuf)
  DEB_ASCII_BUF_EntryType(typeBuf)
  NameEntry_t *name_p;
  FullName_u ename;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CA_LookupName(%s,%s)",
      deb_ascii_FullName(entrynameBuf,entryname_p),
      deb_ascii_EntryType(typeBuf,type_p)));
  
  /* Convert name to upper case for hashing and comparisions */
  if (dns_normalize_fname(entryname_p, (FullName_t *)ename, sizeof(FullName_u))
	== DNS_SUCCESS) 
  {
    const int idx = CAHashName((FullName_t *)ename);

    name_p = CA_FindName((FullName_t *)ename, type_p, idx);
    if (name_p)
    {
      /* Move to front of LRU list */
      CALruMove(name_p);
    }
  } 
  else
  {
    name_p = NULL;
  }
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CA_LookupName(%s,%s) return(%p)",
      deb_ascii_FullName(entrynameBuf,entryname_p),
      deb_ascii_EntryType(typeBuf,type_p),
      name_p));
  return(name_p);
}

/*
 * Add an entry to the namehash.  
 * Client lock held by caller.
 */
NameEntry_t *
CA_InsertName (FullName_t    *entry_p,
                EntryType_t   *type_p,
                dns_status_t  *status_p)
{
  DEB_ASCII_BUF_FullName(entryBuf)
  DEB_ASCII_BUF_EntryType(typeBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  register NameEntry_t *name_p = NULL;
/* #ifdef DNS_VMS_KERNEL */
  FullName_u ename;
  dns_status_t status = DNS_CLERKBUG;
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug5,
      " >CA_InsertName(%s,%s,%p)",
      deb_ascii_FullName(entryBuf,entry_p),
      deb_ascii_EntryType(typeBuf,type_p),
      status_p));
  
#ifdef CACHE_PARANOID
  if (!CA_DataLocked()) 
  {
    CA_snapshot_cache();
    dce_svc_printf(CDS_S_CACHE_CLOCK_A_MSG);
    goto leave_CA_InsertName;
  }
#endif

  /* Convert name to upper case for hashing and comparisions */
  if ((status = dns_normalize_fname(entry_p,
		(FullName_t *)ename, sizeof(FullName_u))) == DNS_SUCCESS) 
  {
    /* Get hash table index */
    const int idx = CAHashName((FullName_t *)ename);

    /* See if name already exists */
    name_p = CA_FindName((FullName_t *)ename, type_p, idx);
    if (name_p) 
    {
      /* Already here, move to front of LRU list */
      CALruMove(name_p);
    } 
    else 
    {
      name_p = (NameEntry_t *)CA_ClientAlloc(sizeof(*name_p)
		    - sizeof(name_p->nh_name) + LEN_FullName(ename), NHEntry);
      if (name_p)  
      {
	/* Initialize entry */
	COPY_EntryType(type_p, name_p->nh_type);
	COPY_FullName(ename, name_p->nh_name);

	/* 
	 * Generate local cts to represent this name.  It will be stored in
	 * the attribute hash and user Access hash in place of the name/type
	 * The index into the Namehash table is also stored so it can
	 * be returned to the attribute Hash and Access hash.
	 */
	MAKE_token(name_p->nh_token);
	
	CAAtomicLock();
	  /* Set linked list of attributes to empty */
	  name_p->nh_attributes.ll_flink =
				CA_GET_OFFSET(&name_p->nh_attributes);
	  name_p->nh_attributes.ll_blink = name_p->nh_attributes.ll_flink;
	CAAtomicUnlock();
	
	/* Add to front of LRU list */
	CA_LruInsert(name_p);
	
	/* Add to collision list */
	CA_CollisionInsert(CA_GET_LLPOINTER(CAfixed_p->ca_name) + idx, 
			  (CacheLinkedList_t *)name_p);
      } else
	status = DNS_NONSRESOURCES;
    }
  }

#ifdef CACHE_PARANOID
leave_CA_InsertName:
#endif
  if (status_p)
    *status_p = status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug5,
      " <CA_InsertName(%s,%s,%p->%s) return(%p)",
      deb_ascii_FullName(entryBuf,entry_p),
      deb_ascii_EntryType(typeBuf,type_p),
      status_p,
      deb_ascii_dns_status(statusBuf,status),
      name_p));
  return(name_p);
}

/*
 * Remove an entry from the namehash.  Free all of its attribute values.
 * User tokens will become invalid automatically cause the
 * token will no longer match.
 */
void 
CADeleteName (FullName_t   *entry_p,
              FullName_t   *lname_p,
              EntryType_t  *type_p)
{
  DEB_ASCII_BUF_FullName(entryBuf)
  DEB_ASCII_BUF_FullName(lnameBuf)
  DEB_ASCII_BUF_EntryType(typeBuf)
/* #ifdef DNS_VMS_KERNEL */
  FullName_u ename;
  register NameEntry_t *name_p;
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CADeleteName(%s,%s,%s)",
      deb_ascii_FullName(entryBuf,entry_p),
      deb_ascii_FullName(lnameBuf,lname_p),
      deb_ascii_EntryType(typeBuf,type_p)));
  
  /* Convert name to upper case for hashing and comparisions */
  if (dns_normalize_fname(entry_p, (FullName_t *)ename, sizeof(FullName_u))
      == DNS_SUCCESS) 
  {
      /* Get hash table index */
      const int idx = CAHashName((FullName_t *)ename);
      
      CALockData();
      
      /* See if name already exists */
      name_p = CA_FindName((FullName_t *)ename, type_p, idx);
      if (name_p) 
      {
	  /* Remove all attribute values */
	  register ValueEntry_t *val_p =
			CA_GET_AVPOINTER(name_p->nh_attributes.ll_flink);

	  while (val_p != (ValueEntry_t *)&name_p->nh_attributes) 
	  {
	      /* Save pointer, destroyed by removes */
	      register ValueEntry_t *const next_p =
				CA_GET_AVPOINTER(val_p->av_collision.ll_flink);

	      if (CA_locked(val_p)) 
	      {
		AttrEntry_t *const att_p =
				CA_FindAttrEntry((token_t *)val_p->av_token);

		/* drop refs for corresponding AttrEntry */
		if (att_p) 
		{
		  CA_unlock(att_p);
		}
	      }

	      /* Remove it from lists */
	      CA_CollisionRemove((CacheLinkedList_t *)val_p);
	      CA_LruRemove(val_p);
	      
	      /* Add to free list */
	      CA_FreeListFree(val_p);
	      
	      val_p = next_p;
	      
	  }
	  /* Remove from list */
	  CA_CollisionRemove((CacheLinkedList_t *)name_p);
	  CA_LruRemove(name_p);
	  
	  /* Add to free list */
	  CA_FreeListFree(name_p);
      }
      
      if ((EXT8(type_p->et_value) == ET_softlink) ||
	 (dnsCmpFull((dns_opq_fname *)entry_p, (dns_opq_fname *)lname_p) != 0))
      {
	(void)CA_RemoveLink(lname_p);
      }      
      CAUnlockData();
      
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CADeleteName(%s,%s,%s)",
      deb_ascii_FullName(entryBuf,entry_p),
      deb_ascii_FullName(lnameBuf,lname_p),
      deb_ascii_EntryType(typeBuf,type_p)));
}

/*
 * CAFindAtt - given a attribute token, find the entry
 */
CA_PRIVATE AttrEntry_t *
CA_FindAttrEntry (token_t *token_p)
{
  CacheLinkedList_t *ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_attr);
  register int idx;

  for (idx = 0; idx < CAfixed_p->ca_attr_hash_len; ll_p++, idx++) 
  {
    register AttrEntry_t *att_p;

    for (att_p = CA_GET_AHPOINTER(ll_p->ll_flink);
	 att_p != (AttrEntry_t *)ll_p;
	 att_p = CA_GET_AHPOINTER(att_p->ah_collision.ll_flink))
    {
      if (EQ_token(token_p, att_p->ah_token))
	return(att_p);
    }
  }
  return(NULL);
}








