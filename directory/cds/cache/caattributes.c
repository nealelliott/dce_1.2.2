/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: caattributes.c,v $
 * Revision 1.1.7.2  1996/02/18  19:23:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:07:55  marty]
 *
 * Revision 1.1.7.1  1995/12/08  00:10:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:24  root]
 * 
 * Revision 1.1.4.5  1994/08/16  17:27:08  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockClient() by CALockData(),
 * 	CAUnlockClient() by CAUnlockData().
 * 	[1994/08/16  15:29:37  peckham]
 * 
 * Revision 1.1.4.4  1994/06/23  18:28:09  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:13:38  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  16:06:58  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:32  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:37:49  peckham
 * 	Remove duplicate includes and dns_debug.h
 * 	[1994/04/11  17:30:20  peckham]
 * 
 * 	Remove VMS conditionals
 * 	[1994/04/05  17:51:22  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  21:55:24  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:46:32  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:36:11  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:19:01  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:24:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: caattributes.c
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

static dns_status_t 
ReadAttr (
     NameEntry_t         *,
     AttributeName_t     *,
     Timestamp_t         *,
     int                 ,
     AttributeContents_t *,
     byte_t              *,
     unsigned int        ,
     int                 *);

static dns_status_t 
readmember (
     MemberValue_t       *,
     Timestamp_t         *,
     int                 ,
     AttributeContents_t *,
     byte_t              *);

static dns_status_t 
readset (
     Set_t               *,
     int                 ,
     Timestamp_t         *,
     int                 ,
     AttributeContents_t *,
     byte_t              *);

/* 
 * Find the attribute name in the attr hash table.
 * Move to front of LRU list.
 * Cliet lock held by caller.
 * Names in upper case on input.
 */
AttrEntry_t *
CA_LookupAttr (AttributeName_t *attname_p)
{
  DEB_ASCII_BUF_AttributeName(attnameBuf)
  CacheLinkedList_t *ll_p;
  register AttrEntry_t *att_p;
  const int len = LEN_AttributeName(attname_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " >CA_LookupAttr(%s)",
      deb_ascii_AttributeName(attnameBuf,attname_p)));

  ll_p = CA_GET_LLPOINTER(CAfixed_p->ca_attr) + CAHashAttr(attname_p);
  
  /* Search linked list for a match */
  for (att_p = CA_GET_AHPOINTER(ll_p->ll_flink);
       att_p != (AttrEntry_t *)ll_p;
       att_p = CA_GET_AHPOINTER(att_p->ah_collision.ll_flink))
    /* Now check names */
    if (len == LEN_AttributeName(att_p->ah_name))
      if (EQ_SimpleName(attname_p, att_p->ah_name)) {
	/* Move to front of LRU list */
	CALruMove(att_p);
	goto leave_CA_LookupAttr;
      }

  att_p = NULL;

leave_CA_LookupAttr:
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug8,
      " <CA_LookupAttr(%s) return(%p)",
      deb_ascii_AttributeName(attnameBuf,attname_p),
      att_p));
  return(att_p);
}

/* 
 * Read an attribute from the cache.  Move value to front of lru list.
 * Client lock held by caller.
 */
static dns_status_t 
ReadAttr (NameEntry_t          *name_p,
          AttributeName_t      *attname_p,
          Timestamp_t          *context_p,
          int                  max_size,
          AttributeContents_t  *contents_p,
          byte_t               *whole_p,
          unsigned int         seconds,
          int                  *stale_p)
{
  DEB_ASCII_BUF_AttributeName(attnameBuf)
  DEB_ASCII_BUF_Timestamp(contextBuf)
  DEB_ASCII_BUF_AttributeContents(contentsBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  AttributeName_u attribute;
  register AttrEntry_t *att_p;
  dns_status_t status;

  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug5,
      " >ReadAttr(%p,%s,%s,%d,%s,%p,%d,%p)",
      name_p,
      deb_ascii_AttributeName(attnameBuf,attname_p),
      deb_ascii_Timestamp(contextBuf,context_p),
      max_size,
      deb_ascii_AttributeContents(contentsBuf,contents_p),
      whole_p,
      seconds,
      stale_p));

  /* Lower case the attribute name for comparisions */
  dns_attname_upper(attname_p, attribute);

  status = DNS_NOTFND;
  /* Get attribute from attr hash */
  att_p = CA_LookupAttr((AttributeName_t *)attribute);
  if (att_p != NULL) {
    register ValueEntry_t *val_p;

    /* 
     * Search linked list of values for this name.
     * Match attribute name tokens to find correct valueentry
     */
    for (val_p = CA_GET_AVPOINTER(name_p->nh_attributes.ll_flink);
	 val_p != (ValueEntry_t *)&name_p->nh_attributes;
	 val_p = CA_GET_AVPOINTER(val_p->av_collision.ll_flink))
      /* Find attribute value in linked list */
      if (EQ_token(val_p->av_token, att_p->ah_token)) {
	AttributeContents_t *const ac_p =
				(AttributeContents_t *)val_p->av_value;

	/* If data is too old, skip it */
	if (CAStale(seconds, &val_p->av_inCache)) {
  
	  DEB_ASCII_BUF_FullName(entrynameBuf)

	  DCE_SVC_DEBUG((
	      cds__svc_handle,
      	      cds_svc_cache,
	      svc_c_debug5,
	      " ReadAttr(%s,%s,%s,%d,%s,%p,%d,%p) stale attribute",
	      deb_ascii_FullName(entrynameBuf,name_p->nh_name),
	      deb_ascii_AttributeName(attnameBuf,attname_p),
	      deb_ascii_Timestamp(contextBuf,context_p),
	      max_size,
	      deb_ascii_AttributeContents(contentsBuf,contents_p),
	      whole_p,
	      seconds,
	      stale_p));
	  *stale_p = TRUE;
	  break;
	}

	/* Set up return data */
	if (EXT8(ac_p->ac_type) == AT_single)
	  status = readmember((MemberValue_t *)ac_p->ac_single, 
			      context_p, max_size, contents_p, whole_p);
	else
	  status = readset((Set_t *)ac_p->ac_set, val_p->av_wholeset, 
			   context_p, max_size, contents_p, whole_p);
	break;
      }
  }
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug5,
      " <ReadAttr(%p,%s,%s,%d,%s,%p,%d,%p) return(%s)",
      name_p,
      deb_ascii_AttributeName(attnameBuf,attname_p),
      deb_ascii_Timestamp(contextBuf,context_p),
      max_size,
      deb_ascii_AttributeContents(contentsBuf,contents_p),
      whole_p,
      seconds,
      stale_p,
      deb_ascii_dns_status(statusBuf,status)));

  return(status);
}

/*
 * Move a membervalue from the cache the the user buffer  
 * Client lock held by caller.
 */
static dns_status_t 
readmember (MemberValue_t        *member_p,
            Timestamp_t          *context_p,
            int                  max_size,
            AttributeContents_t  *contents_p,
            byte_t               *whole_p)
{
  DEB_ASCII_BUF_Timestamp(contextBuf)
  DEB_ASCII_BUF_AttributeContents(contentsBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  dns_status_t status;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >readmember(%p,%s,%d,%s,%p)",
      member_p,
      deb_ascii_Timestamp(contextBuf,context_p),
      max_size,
      deb_ascii_AttributeContents(contentsBuf,contents_p),
      whole_p));

  /* Make sure member is present */
  if (FLAG_IS_SET(member_p->mv_flag, MH_present)) {
    /* 
     * If set member and type will not fit in user buffer
     * let the server decide what to do.
     */
    if ((sizeof(AttributeType_u)+LEN_MemberValue(member_p)) <= max_size) {
      /* 
       * If context is not equal to null, let server worry about it.  
       */
      if (EQ_Timestamp(context_p, NullTimestamp)) {
	INS8(contents_p->ac_type, AT_single);
	COPY_MemberValue(member_p, contents_p->ac_single);
	INS8(whole_p, TRUE);
	status = DNS_SUCCESS;
      } else
	status = DNS_NOTFND;
    } else
      status = DNS_NOTFND;
  } else
    status = DNS_NOTFND;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <readmember(%p,%s,%d,%s,%p) return(%s)",
      member_p,
      deb_ascii_Timestamp(contextBuf,context_p),
      max_size,
      deb_ascii_AttributeContents(contentsBuf,contents_p),
      whole_p,
      deb_ascii_dns_status(statusBuf,status)));

  return(status);
}

/*
 * Return as much as the cached set as will fit in the user buffer.
 * We may or may not have the whole set cached, and the users
 * context could be non-null.
 * Client lock held by caller.
 */
static dns_status_t 
readset (Set_t                *set_p,
         int                  allhere,
         Timestamp_t          *context_p,
         int                  max_size,
         AttributeContents_t  *contents_p,
         byte_t               *whole_p)
{
  DEB_ASCII_BUF_Set(setBuf)
  DEB_ASCII_BUF_Timestamp(contextBuf)
  DEB_ASCII_BUF_AttributeContents(contentsBuf)
  Set_t *outset_p;
  word_t *off_p;
  register MemberValue_t *outmember_p;
  int start, end, i;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " >readset(%s,%d,%s,%d,%s,%p)",
      deb_ascii_Set(setBuf,set_p),
      allhere,
      deb_ascii_Timestamp(contextBuf,context_p),
      max_size,
      deb_ascii_AttributeContents(contentsBuf,contents_p),
      whole_p));

  /* Make sure set is present */
  if (FLAG_CLEAR(set_p->st_flag, ST_present)) {
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug6,
        " <readset(%s,%d,%s,%d,%s,%p) return(DNS_NOTFND)",
        deb_ascii_Set(setBuf,set_p),
        allhere,
        deb_ascii_Timestamp(contextBuf,context_p),
        max_size,
        deb_ascii_AttributeContents(contentsBuf,contents_p),
        whole_p));
    return(DNS_NOTFND);
  }


  if (EQ_Timestamp(context_p, NullTimestamp))
    start = 0;
  else {
    /* Determine which set member to start with based on the context_p */
    start = -1;
    for (i=0; i<NUM_Set(set_p); ++i) {
      /* Get pointer to member */
      MemberValue_t *const member_p = (MemberValue_t *)INDEX_Set(set_p, i);
      
      /* 
       * If context is not equal to any member timestamp or null,
       * let server worry about it.  If we want to emulate the server
       * better we would compare timestamps but to do this
       * we must ensure the wholeentry is cached, otherwise we
       * could miss values.
       */
      if EQ_Timestamp(context_p, member_p->mv_timestamp) { 
	start = ++i;
	break;
      }
    }

    /* If didn't find a match, let the server handle the request */
    if (start == -1) {
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_cache,
          svc_c_debug6,
          " <readset(%s,%d,%s,%d,%s,%p) return(DNS_NOTFND)",
          deb_ascii_Set(setBuf,set_p),
          allhere,
          deb_ascii_Timestamp(contextBuf,context_p),
          max_size,
          deb_ascii_AttributeContents(contentsBuf,contents_p),
          whole_p));
      return(DNS_NOTFND);
    }
  }


  /* Subtract attribute type from user buffer size */
  max_size -= sizeof(AttributeType_u);
  
  /* Subtract set overhead from max_size */
  max_size -= sizeof(Set_overhead);
  
  for (end = start; end < NUM_Set(set_p); ++end) {
    MemberValue_t *const member_p = (MemberValue_t *)INDEX_Set(set_p, end);
    
    /* Subtract length of member + offset word to describe member */
    max_size -= (LEN_MemberValue(member_p) + sizeof(word_u));
    if (max_size < 0) break;
    
  }

  /* If the set has members, will they fit */
  if (NUM_Set(set_p))
      /* If no member will fit, let server determine what to do */
      if (end == start) {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_cache,
            svc_c_debug6,
            " <readset(%s,%d,%s,%d,%s,%p) return(DNS_NOTFND)",
            deb_ascii_Set(setBuf,set_p),
            allhere,
            deb_ascii_Timestamp(contextBuf,context_p),
            max_size,
            deb_ascii_AttributeContents(contentsBuf,contents_p),
            whole_p ));
	  return(DNS_NOTFND);
      }

  outset_p = (Set_t *)contents_p->ac_set;
  INS8(contents_p->ac_type, AT_set);
  INS8(outset_p->st_flag, ST_present);
  INS16(outset_p->st_offsetLength, sizeof(word_u) * (end-start));
  off_p = outset_p->st_offsets;
  outmember_p = (MemberValue_t *)DATA_Set(outset_p);

  for (i=start; i<end; ++i) {
    MemberValue_t *const member_p = (MemberValue_t *)INDEX_Set(set_p, i);

    INS16(off_p, DIFF_bytes(outmember_p, outset_p));
    off_p = SKIP_word(off_p);

    COPY_MemberValue(member_p, outmember_p);
    outmember_p = (MemberValue_t *)SKIP_MemberValue(outmember_p);
  }
  INS16(outset_p->st_valueLength, DIFF_bytes(outmember_p, DATA_Set(outset_p)));

  if (allhere == TRUE) {
    if (end == NUM_Set(set_p)) {
      INS8(whole_p, TRUE);
    } else {
      INS8(whole_p, FALSE);
    }
  } else
    INS8(whole_p, FALSE);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug6,
      " <readset(%s,%d,%s,%d,%s,%p) return(DNS_SUCCESS)",
      deb_ascii_Set(setBuf,set_p),
      allhere,
      deb_ascii_Timestamp(contextBuf,context_p),
      max_size,
      deb_ascii_AttributeContents(contentsBuf,contents_p),
      whole_p ));

  return(DNS_SUCCESS);
}

/*
 * Read an attribute from the cache.
 */
dns_status_t 
CAReadAttribute (cds_user_t           user_p,
                 unsigned int         seconds,
                 FullName_t           *entryname_p,
                 EntryType_t          *type_p,
                 AttributeName_t      *attname_p,
                 Timestamp_t          *context_p,
                 int                  max_size,
                 AttributeContents_t  *contents_p,
                 byte_t               *whole_p,
                 int                  *stale_p,
                 byte_t               *linked_p)
{
  DEB_ASCII_BUF_user(userBuf)
  DEB_ASCII_BUF_FullName(entrynameBuf)
  DEB_ASCII_BUF_EntryType(typeBuf)
  DEB_ASCII_BUF_AttributeName(attnameBuf)
  DEB_ASCII_BUF_Timestamp(contextBuf)
  DEB_ASCII_BUF_AttributeContents(contentsBuf)
  DEB_ASCII_BUF_dns_status(statusBuf)
  FullName_u ename;
  NameEntry_t *name_p;
  dns_status_t status;
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " >CAReadAttribute(%s,%d,%s,%s,%s,%s,%d,%s,%p,%p,%p)",
      deb_ascii_user(userBuf,user_p),
      seconds,
      deb_ascii_FullName(entrynameBuf,entryname_p),
      deb_ascii_EntryType(typeBuf,type_p),
      deb_ascii_AttributeName(attnameBuf,attname_p),
      deb_ascii_Timestamp(contextBuf,context_p),
      max_size,
      deb_ascii_AttributeContents(contentsBuf,contents_p),
      whole_p,
      stale_p,
      linked_p));

  *stale_p = FALSE;
  *linked_p = FALSE;

  {
    /* Get require locks */
    CALockData();

    name_p = CA_LookupName(entryname_p, type_p);
    if (!name_p && (EXT8(type_p) != ET_softlink)) {
      /* Attempt to resolve the name and try again */
      if (CA_ResolveName(user_p, seconds, entryname_p,
			 (FullName_t *)ename, NULL)) {
	*linked_p = TRUE;
	name_p = CA_LookupName((FullName_t *)ename, type_p);
      }
    }
    if (name_p) {
      /* See if this user has access */
      if (CA_LookupAccess(user_p, (token_t *)name_p->nh_token)) {
	/* Check attributes cached with name_p */
	status = ReadAttr(name_p, attname_p, context_p, max_size, 
			  contents_p, whole_p, seconds, stale_p);
      } else
	status = DNS_NOTFND;
    } else
      status = DNS_NOTFND;

    CAUnlockData();
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_cache,
      svc_c_debug4,
      " <CAReadAttribute(%s,%d,%s,%s,%s,%s,%d,%s,%p->%d,%p->%d,%p->%d) return(%s)",
      deb_ascii_user(userBuf,user_p),
      seconds,
      deb_ascii_FullName(entrynameBuf,entryname_p),
      deb_ascii_EntryType(typeBuf,type_p),
      deb_ascii_AttributeName(attnameBuf,attname_p),
      deb_ascii_Timestamp(contextBuf,context_p),
      max_size,
      deb_ascii_AttributeContents(contentsBuf,contents_p),
      whole_p,
      *whole_p,
      stale_p,
      *stale_p,
      linked_p,
      *linked_p,
      deb_ascii_dns_status(statusBuf,status)));
  return(status);
}


