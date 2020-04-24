/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cainsertatt.c,v $
 * Revision 1.1.9.2  1996/02/18  19:24:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:07  marty]
 *
 * Revision 1.1.9.1  1995/12/08  00:10:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:36  root]
 * 
 * Revision 1.1.6.5  1994/08/16  17:27:14  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockClient() by CALockData(),
 * 	CAUnlockClient() by CAUnlockData().
 * 	[1994/08/16  15:30:19  peckham]
 * 
 * Revision 1.1.6.4  1994/06/23  18:28:26  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:44:17  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  16:07:09  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:45  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:38:00  peckham
 * 	Remove UNIX and VMS conditionals.
 * 	[1994/04/11  17:40:08  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:55:49  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:47:55  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:37:01  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:20:11  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:13:31  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:05:30  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: cainsertatt.c
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

CA_PRIVATE dns_status_t
CA_InsertAttr (
     NameEntry_t         *,
     AttributeName_t     *,
     Timestamp_t         *,
     AttributeContents_t *,
     int                  );

CA_PRIVATE void 
CA_new_set (
     Set_t  *,
     Set_t  *,
     Set_t  *);

/* 
 * Combine two sets into one. Set2 values go after set1 values.
 */
CA_PRIVATE void 
CA_new_set (Set_t  *newset_p,
            Set_t  *set1_p,
            Set_t  *set2_p)
{
  DEB_ASCII_BUF_Set(newsetBuf)
  DEB_ASCII_BUF_Set(set1Buf)
  DEB_ASCII_BUF_Set(set2Buf)
  word_t *off_p = (word_t *)newset_p->st_offsets;
  byte_t *out_p;
  int i;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " >CA_new_set(,%s,%s)",
      deb_ascii_Set(set1Buf,set1_p),
      deb_ascii_Set(set2Buf,set2_p)));

  /* Set up header */
  INS8(newset_p->st_flag, ST_present);
  INS16(newset_p->st_offsetLength, 
	EXT16(set1_p->st_offsetLength) + EXT16(set2_p->st_offsetLength));

  /* Compute new offsets and copy members */
  out_p = DATA_Set(newset_p);
  for (i=0; i<NUM_Set(set1_p); ++i) {
    /* Copy member */
    MemberValue_t *const member_p = (MemberValue_t *)INDEX_Set(set1_p, i);

    /* Set up new offset value */
    INS16(off_p, DIFF_bytes(out_p, newset_p));
    off_p = SKIP_word(off_p);

    COPY_MemberValue(member_p, out_p);
    out_p = SKIP_MemberValue(out_p);
  }

  /* Now add members from set2 */
  for (i=0; i<NUM_Set(set2_p); ++i) {
    /* Copy member */
    MemberValue_t *const member_p = (MemberValue_t *)INDEX_Set(set2_p, i);

    /* Set up new offset value */
    INS16(off_p, DIFF_bytes(out_p, newset_p));
    off_p = SKIP_word(off_p);

    COPY_MemberValue(member_p, out_p);
    out_p = SKIP_MemberValue(out_p);
  }

  INS16(newset_p->st_valueLength, DIFF_bytes(out_p, DATA_Set(newset_p)));
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug7,
      " <CA_new_set(%s,%s,%s)",
      deb_ascii_Set(newsetBuf,newset_p),
      deb_ascii_Set(set1Buf,set1_p),
      deb_ascii_Set(set2Buf,set2_p)));
}

/*
 * Add an attribute value to the cache.
 * Client lock held by caller.
 */
CA_PRIVATE dns_status_t 
CA_InsertAttr (NameEntry_t          *name_p,
               AttributeName_t      *attname_p,
               Timestamp_t          *context_p,
               AttributeContents_t  *contents_p,
               int                  wholeset)
{
  DEB_ASCII_BUF_AttributeName(attnameBuf)
  DEB_ASCII_BUF_Timestamp(contextBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  AttributeName_u attribute;
  AttrEntry_t *att_p;
  AttributeContents_t *ac_p, *newac_p;
  ValueEntry_t *val_p;
  ValueEntry_t *newval_p = NULL;
  dns_status_t status = DNS_SUCCESS;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >CA_InsertAttr(%p,%s,%s,%p,%d)",
      name_p,
      deb_ascii_AttributeName(attnameBuf,attname_p),
      deb_ascii_Timestamp(contextBuf,context_p),
      contents_p,
      wholeset));

  /* Convert name for comparisions */
  dns_attname_upper(attname_p, attribute);

  /* 
   * Get attribute name in shared cache.
   * If it doesn't exist, create it.
   * Remove it from the LRU list so it doesn't get replaced
   * if space is reclaimed for the value itself.
   */
  att_p = CA_LookupAttr((AttributeName_t *)attribute);
  if (!att_p) {
    att_p = (AttrEntry_t *)CA_ClientAlloc(sizeof(*att_p)
			- sizeof(att_p->ah_name) + LEN_AttributeName(attribute)
			, AHEntry);
    if (att_p) {
      const int idx = CAHashAttr((AttributeName_t *)attribute);

      /* Make new entry */
      MAKE_token(att_p->ah_token);
      COPY_AttributeName(attribute, att_p->ah_name);

      /* Add to hash table */
      CA_CollisionInsert(CA_GET_LLPOINTER(CAfixed_p->ca_attr) + idx,
			  &att_p->ah_collision);
    } else {
      status = DNS_NONSRESOURCES;
      goto leave_CA_InsertAttr;
    }
  } else
    CA_LruRemove(att_p);

  /* Locate value entry  */
  for (val_p = CA_GET_AVPOINTER(name_p->nh_attributes.ll_flink),
	ac_p = (AttributeContents_t *)val_p->av_value;
       val_p != (ValueEntry_t *)&name_p->nh_attributes;
       val_p = CA_GET_AVPOINTER(val_p->av_collision.ll_flink))
    if (EQ_token(att_p->ah_token, val_p->av_token)) {

      CA_LruRemove(val_p);
      /* 
       * If this is a continuation of a previous read, we can accept it.
       * Otherwise assume this is more upto date and replace the
       * entry.  If wholeset is false, must be a set.
       */
      if ((val_p->av_wholeset == FALSE) &&
	  (EXT8(ac_p->ac_type) == EXT8(contents_p->ac_type))) {
	/* Get pointer to last member currently cached */
	MemberValue_t *const member_p = (MemberValue_t *)
			INDEX_Set(ac_p->ac_set, NUM_Set(ac_p->ac_set) - 1); 
	/* 
	 * If context is equal to this members timestamp, we consider
	 * this a continuation of a previous call that returned partial
	 * results.
	 */
	if (EQ_Timestamp(member_p->mv_timestamp, context_p)) {
	  /* 
	   * Allocate new entry to store expanded value. 
	   */
	  newval_p = (ValueEntry_t *)CA_ClientAlloc(sizeof(*newval_p)
					- sizeof(newval_p->av_value)
				+ LEN_AttributeContents(val_p->av_value)
				+ LEN_Set(contents_p->ac_set), VHEntry);
	  if (!newval_p) {
	    /* Add attribute back tolist and get out of here */
	    CA_LruInsert(val_p);
	    CA_LruInsert(att_p);
	    status = DNS_NONSRESOURCES;
	    goto leave_CA_InsertAttr;
	  }

	  newac_p = (AttributeContents_t *)newval_p->av_value;
	  COPY_bytes(val_p, newval_p,  DIFF_bytes(newac_p, newval_p));
	  INS8(newac_p->ac_type, EXT8(ac_p->ac_type));
	  CA_new_set((Set_t *)newac_p->ac_set, 
		  (Set_t *)ac_p->ac_set, 
		  (Set_t *)contents_p->ac_set);
	  newval_p->av_wholeset = wholeset;

	  /* If old value was locked, lock down the new value */
	  if (CA_locked(val_p)) {
	    CA_copy_lock(val_p, newval_p);
	    CA_lock(att_p);
	  }
	} /* End continuation of previous read */
      } /* End wholeset == false */

      /* removing old value; one less reference to name */
      if (CA_locked(val_p)) CA_unlock(att_p);

      /* Remove old entry, it is being replaced */
      CA_CollisionRemove(&val_p->av_collision);
      CA_FreeListFree(val_p);
      break;
    }     /* End found attribute */
    
  if (!newval_p) {
    /* 
     * This should never happen.  Singles have only one value but
     * just in case this happens catch it here.
     */
    if (NE_Timestamp(context_p, NullTimestamp) || ((wholeset == FALSE) && 
			    (EXT8(contents_p->ac_type) == AT_single))) {
      CA_LruInsert(att_p);
      return(DNS_SUCCESS);
    }

    /* Don't cache empty set members or sets */
    if (EXT8(contents_p->ac_type) == AT_single) {
      MemberValue_t *const mv_p = (MemberValue_t *)contents_p->ac_single;

      if (FLAG_IS_CLEAR(mv_p->mv_flag, MH_present)) {
	CA_LruInsert(att_p);
	return(DNS_SUCCESS);
      }
    } else {
      Set_t *const st_p = (Set_t *)contents_p->ac_set;

      if (FLAG_CLEAR(st_p->st_flag, ST_present)) {
	CA_LruInsert(att_p);
	return(DNS_SUCCESS);
      }
    }
    
    newval_p = (ValueEntry_t *)CA_ClientAlloc(sizeof(*newval_p)
				- sizeof(newval_p->av_value)
				+ LEN_AttributeContents(contents_p), VHEntry);
    if (!newval_p) {
      CA_LruInsert(att_p);
      return(DNS_NONSRESOURCES);
    }

    /* If object is locked down, lock down the value */
    if (CA_locked(name_p)) {
      CA_lock(newval_p);
      CA_lock(att_p);
    }
 
    /* Build new entry */
    COPY_token(att_p->ah_token, newval_p->av_token);
    newval_p->av_wholeset = wholeset;
    COPY_AttributeContents(contents_p, newval_p->av_value);
  }
  

  /* Add new attribute to appropriate lists */
  (void)utc_gettime(&newval_p->av_inCache);

  CA_LruInsert(newval_p);
  CA_CollisionInsert(&name_p->nh_attributes, &newval_p->av_collision);  	
  CA_LruInsert(att_p);

leave_CA_InsertAttr:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <CA_InsertAttr(%p,%s,%s,%p,%d) return(%s)",
      name_p,
      deb_ascii_AttributeName(attnameBuf,attname_p),
      deb_ascii_Timestamp(contextBuf,context_p),
      contents_p,
      wholeset,
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}

/*
 * Insert an attribute into the cache.  This could involve creating a
 * userhash entry, useraccess entry, Namehash entry and an attrhash entry.
 */
dns_status_t 
CAInsertAttribute (cds_user_t           user_p,
                   FullName_t           *entryname_p,
                   EntryType_t          *type_p,
                   AttributeName_t      *attname_p,
                   Timestamp_t          *context_p,
                   AttributeContents_t  *contents_p,
                   int                  wholeset)
{
  DEB_ASCII_BUF_user(userBuf)
  DEB_ASCII_BUF_FullName(entrynameBuf)
  DEB_ASCII_BUF_EntryType(typeBuf)
  DEB_ASCII_BUF_AttributeName(attnameBuf)
  DEB_ASCII_BUF_Timestamp(contextBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  NameEntry_t *name_p;
  dns_status_t status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CAInsertAttribute(%s,%s,%s,%s,%s,%p,%d)",
      deb_ascii_user(userBuf,user_p),
      deb_ascii_FullName(entrynameBuf,entryname_p),
      deb_ascii_EntryType(typeBuf,type_p),
      deb_ascii_AttributeName(attnameBuf,attname_p),
      deb_ascii_Timestamp(contextBuf,context_p),
      contents_p,
      wholeset));

  /* Lock cache for update */
  CALockData();

  {
    dns_status_t ca_status;

    /* Create or retrieve shared name entry */
    name_p = CA_InsertName(entryname_p, type_p, &ca_status);
    if (name_p)  {
      /*
       * Remove name_p from LRU list so it doesn't get moved to
       * the free list in an attempt to reclaim space for the attribute/value
       */
      CA_LruRemove(name_p);
      /* 
       * Add the entry and attribute to the shared cache. 
       */
      status = CA_InsertAttr(name_p, attname_p, context_p, contents_p, wholeset);
      /* 
       * Add access token to user hash 
       */
      if (status == DNS_SUCCESS)
	status = CA_InsertAccess(user_p, (token_t *)name_p->nh_token);

      /* Add name_p back to list */
      CA_LruInsert(name_p);
    } else
      status = ca_status;
  }

  CAUnlockData();

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CAInsertAttribute(%s,%s,%s,%s,%s,%p,%d) return(%s)",
      deb_ascii_user(userBuf,user_p),
      deb_ascii_FullName(entrynameBuf,entryname_p),
      deb_ascii_EntryType(typeBuf,type_p),
      deb_ascii_AttributeName(attnameBuf,attname_p),
      deb_ascii_Timestamp(contextBuf,context_p),
      contents_p,
      wholeset,
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}





