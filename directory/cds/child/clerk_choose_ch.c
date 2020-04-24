/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_choose_ch.c,v $
 * Revision 1.1.11.2  1996/02/18  19:25:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:56  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:03:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:44  root]
 * 
 * Revision 1.1.8.5  1994/08/16  17:27:18  peckham
 * 	CR#11640: potential cache corruption and component crashes
 * 	due to bad fix for CR#10570.
 * 	Replace CALockGlobal() by CALockData(),
 * 	CAUnlockGlobal() by CAUnlockData().
 * 	[1994/08/16  15:30:48  peckham]
 * 
 * Revision 1.1.8.4  1994/08/03  18:59:19  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:10:36  mccann]
 * 
 * Revision 1.1.8.3  1994/06/23  18:28:41  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:53:14  mccann]
 * 
 * Revision 1.1.8.2  1994/06/09  16:07:30  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:11  devsrc]
 * 
 * Revision 1.1.8.1  1994/03/12  21:56:12  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:49:40  peckham]
 * 
 * Revision 1.1.4.4  1993/03/09  19:44:17  peckham
 * 	OT#7411: Request given to gda when other clearinghouse
 * 	available. Relegate gda clearinghouses from the cache
 * 	to last choice in ChooseCH().
 * 	[1993/03/09  19:34:42  peckham]
 * 
 * Revision 1.1.4.3  1993/01/07  22:00:43  keegan
 * 	Check confidence when selecting a clearinghouse in BCHlist().
 * 	[1993/01/07  20:07:13  keegan]
 * 
 * Revision 1.1.4.2  1992/12/30  12:40:24  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:25:45  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  21:18:48  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:07:03  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: clerk_choose_ch.c
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
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>
#include <dce/dnsclerk.h>
#include <uc_clerk.h>
#include <dce/attributes.h>


/*
 * Keep in this order 
 */

#define ONLAN_CONNECTION 1
#define ONLAN_NOCONNECTION 2
#define HAVE_CONNECTION  3
#define HAVE_NOCONNECTION 4
#define IS_NOTOKWITHCNT 5
#define IS_NOTOKWITHOUTCNT 6
#define IS_GDA 7

/* 
 * This should only be updated for functions that walk the tree.  Other
 * modify operations don't go through here.  It currently includes
 * modify attribute, create and delete objects, links, and children,
 * delete directory, skulk,  allow and disallow clearinghoues.
 */
#define UPDATE_OPERATION(oper) 						  \
    ((oper != OP_ReadAttribute) && (oper != OP_TestGroup) && 		  \
     (oper != OP_TestAttribute) && (oper != OP_ResolveName) && 		  \
     (oper != OP_EnumerateAttributes) && (oper != OP_EnumerateObjects) && \
     (oper != OP_EnumerateLinks) && (oper != OP_EnumerateChildren))

/*
 * Function prototypes
 */
static CHlist_t *
build_element (ReplicaPointer_t *);


static CHlist_t *
ChooseCellCh (thread_t *);


static void
init_element (CHlist_t *,
	     ObjUID_t *,
	     FullName_t *,
	     Set_t *,
	     int,
	     int);

CHlist_t 
*insert (CHlist_t  *,
         CHlist_t  *);

CHlist_t 
*merge (CHlist_t  *,
        CHlist_t  *);

CHlist_t 
*BuildCHlist (Set_t *,
              thread_t *);

CHlist_t 
*ChooseMaster (thread_t *,
               FullName_t *);

static CHlist_t *
BCHlist (Set_t *, thread_t *);

static byte_t *max_p = 0;
static byte_t *cur_p;

/*
 * Insert an element onto the beginning of a list.
 * This list is double linked by is not circular.
 */
CHlist_t 
*insert (CHlist_t  *list_p,
         CHlist_t  *element_p)
{
  element_p->ch_next = list_p;
  element_p->ch_prev = NULL;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " >insert"));

  if (list_p)
    list_p->ch_prev = element_p;

  list_p = element_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " <insert"));

  return(list_p);
}

/*
 * Merge 2 double linked, non-circular lists.
 */
CHlist_t 
*merge (CHlist_t  *front_p,
        CHlist_t  *end_p)
{
  CHlist_t *last_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " >merge"));

  if (!front_p)
  {
      DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <merge: return(end_p)"));

      return(end_p);
  }
  

  if (!end_p)
  {
      DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <merge: !endp -> return(front_p)"));

      return(front_p);
  }
  

  /* 	
   * Find the last element in the first list 	
   */
  last_p = front_p;
  while (last_p->ch_next)
    last_p = last_p->ch_next;

  last_p->ch_next = end_p;
  end_p->ch_prev = last_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " <merge: @ end : return(front_p)"));

  return(front_p);
}

/*
 * Fill in chlist 
 */
static void 
init_element (CHlist_t    *element_p,
              ObjUID_t    *id_p,
              FullName_t  *name_p,
              Set_t       *tower_p,
              int         IsOnLan,
              int         IsOk)
{

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " >init_element()"));

  /* Set up flags first */
  element_p->ch_flags.init = 0;

  if (IsOk)
      element_p->ch_flags.flags.ok = 1;

  if (IsOnLan)
    element_p->ch_flags.flags.onLAN = 1;
  
  COPY_FullName(name_p, element_p->ch_name);
  COPY_ObjUID(id_p, element_p->ch_id);

  element_p->ch_count = 1;
  COPY_Set(tower_p, SKIP_FullName(element_p->ch_name));
  
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug9,
      " <init_element()"));
}

/*
 * Build a chlist item from a replica pointer.  Get additional data
 * from clearinghouse cache.  If the clearinghouse address is cached,
 * we will return the cached address.  Otherwise we return the replica hint.
 * It is assumed the cached address is more upto date then the replica 
 * hint.
 *
 * The cache is locked.
 */
static CHlist_t 
*build_element (ReplicaPointer_t *rp_p)
{
  DEB_ASCII_BUF_ReplicaPointer(rpBuf)
  DEB_ASCII_BUF_CHlist(elementBuf)
  FullName_u chname;
  Set_t *hint_p;
  FullName_t *chname_p;
  register CHlist_t *element_p = NULL;
  CleEntry_t *cle_p;
  int namelen, hintlen;
  int idx;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug8,
      " >build_element(%s)",
      deb_ascii_ReplicaPointer(rpBuf,rp_p)));
  
  /* Get pointer and length of address hint */
  hintlen = LEN_Set(rp_p->rp_addressHint);
  hint_p  = (Set_t *)rp_p->rp_addressHint;
  
  /* Get pointer and length of clearinghouse fullname */
  chname_p  = (FullName_t *)SKIP_ReplicaType(SKIP_Set(hint_p));
  namelen = LEN_FullName(chname_p);

  if (dns_normalize_fname(chname_p, (FullName_t *)chname, sizeof(chname))
	== DNS_SUCCESS) {
    idx = CAHashCle(chname_p);
    
    /* 
     * Check for cached clearinghouse address.  It isn't moved to the front
     * of the LRU list until it is actually used.
     *
     * Note that even if we find it in the cache, we also check to assure
     * that the towerset is not empty before promoting it ahead of the
     * ReplicaPointer.  There's not much point using the cached entry first
     * if it's not going to help.
     */

    cle_p = CAFindCle(chname_p, idx);

    if ((cle_p  != NULL) &&
        (!EMPTY_Set((Set_t *)SKIP_FullName(cle_p->ch_name)))) {

      Set_t *tower_p;
      int towerlen;

      /* Build a CHlist with 2 address sets */
      tower_p = (Set_t *)SKIP_FullName(cle_p->ch_name);
      towerlen = LEN_Set(tower_p);

      element_p = (CHlist_t *)dns_malloc(sizeof(*element_p)
				- sizeof(element_p->ch_name) + namelen
				- sizeof(element_p->ch_set) + towerlen);
      if (!element_p) 
      {
	  DCE_SVC_DEBUG((
	      cds__svc_handle,
              cds_svc_child,
              svc_c_debug8,
	      " <build_element: dns_malloc 1 fail -> return(NULL)"));
	  
	  return(NULL);
      }
      
      
      /* Set up element */
      init_element(element_p,(ObjUID_t *)rp_p->rp_CHID, chname_p, tower_p,
	cle_p->ch_flags.status.flags.onLAN, cle_p->ch_flags.status.flags.ok);

    } 
    else 
    {
      /* Build chlist with just the hint address set */
      element_p = (CHlist_t *)dns_malloc(sizeof(*element_p)
				- sizeof(element_p->ch_name) + namelen
				- sizeof(element_p->ch_set) + hintlen);
      if (!element_p) 
      {
	  DCE_SVC_DEBUG((
	      cds__svc_handle,
              cds_svc_child,
              svc_c_debug8,
	      " <build_element: dns_malloc 2 fail -> return(NULL)"));

	  return(NULL);
      }
      
      
      /* Fill in element with hint */
      init_element(element_p, (ObjUID_t *)rp_p->rp_CHID, chname_p,
			hint_p, 0, 1);
    }
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug8,
      " <build_element(%s) return(%s)",
      deb_ascii_ReplicaPointer(rpBuf,rp_p),
      deb_ascii_CHlist(elementBuf,element_p)));

  return(element_p);
}

/*
 * Build linked list of clearinghouses to try based on replica
 * set.  This is an internal routine that expects appropriate locks
 * to be already held.
 * Clearinghouse names in replica set must already be lower case.
 *
 * The returned list is ordered by closeness of clearinghouse
 * to this node, but connections are not taken into account yet.

 */
static CHlist_t 
*BCHlist (Set_t     *set_p,
          thread_t  *th_p)
{
  register CHlist_t *list_p = NULL;
  CHlist_t *lan_p = NULL;
  CHlist_t *idp_p = NULL;
  CHlist_t *area_p = NULL;
  CHlist_t *okay_p = NULL;
  register int i,j;
  int nomaster=0;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug8,
      " >BCHlist()"));

  /* If set is absent, cannot use it */
  if (FLAG_CLEAR(set_p->st_flag, ST_present))
  {
      DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_child,
         svc_c_debug8,
         " <BCHlist: set absent -> return(list_p)"));

      return(list_p);
  }
  

  /* Repeat processing 2 times if no master found when wanted */
  for (j=0; j<2; ++j) {
      /* loop through each replica and build a linked list element */
      for (i=0; i<NUM_Set(set_p); ++i) {
	  MemberValue_t *const member_p = (MemberValue_t *)INDEX_Set(set_p, i);
	  ReplicaPointer_t *rp_p;
	  ReplicaType_t *type_p;
	  int useit = 1;

	  if (FLAG_CLEAR(member_p->mv_flag, MH_present)) continue;
	  if (EXT8(member_p->mv_valuetype) != VT_ReplicaPointer) continue;
	  rp_p = (ReplicaPointer_t *)member_p->mv_value;
	  type_p = (ReplicaType_t *)SKIP_Set(rp_p->rp_addressHint);

	  /* skip replicas which cannot be used to satisfy request */
	  switch (EXT8(type_p)) {
	    case RT_secondary:
	    case RT_master:
	      break;
#ifdef RT_gda
	    case RT_gda:
#endif
	    case RT_readOnly:
              if (UPDATE_OPERATION(th_p->taOperation) ||
		  (th_p->clerkConf == CO_high))


		useit = 0;
	      break;
	  }
	  /* 
	   * If replica is still being considered, build CHlist element 
	   */
	  if (useit || nomaster) {
	      register CHlist_t *const newelement_p = build_element(rp_p);

	      if (!newelement_p) break;

	      /* Force all elements notok to back of list */
	      if (newelement_p->ch_flags.flags.onLAN &&
		  newelement_p->ch_flags.flags.ok)
		lan_p = insert(lan_p, newelement_p);
	      else if (newelement_p->ch_flags.flags.ok)
		okay_p = insert(okay_p, newelement_p);
	      else
		list_p = insert(list_p, newelement_p);
	      
	  } /* End useit  */
      } /* End loop through set */

      /* Return ordered list */
      list_p = merge(okay_p, list_p);
      list_p = merge(idp_p, list_p);
      list_p = merge(area_p, list_p);
      list_p = merge(lan_p, list_p);

      if (list_p) break;
      /* 
       * Force loop again, ignoring replica type.  This is mostly
       * for replica sets built from advertisements in which each
       * replica is flagged as a readonly
       */
      ++nomaster;
  }
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug8,
      " <BCHlist() return(%p)",
      list_p));

  return(list_p);
}

/*
 * Build linked list of clearinghouses to try based on replica
 * set.  This routine is a jacket for BCHlist.  It acquires locks
 * for callers outside of the cache.
 * Clearinghouse names in replica set must already be lower case.
 */
CHlist_t 
*BuildCHlist (Set_t     *set_p,
              thread_t  *th_p)
{
  CHlist_t *list_p;


 DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug8,
      " >BuildCHlist"));

  CALockData();

  list_p = BCHlist(set_p, th_p);

  CAUnlockData();

 DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug8,
      " <BuildCHlist"));

  return(list_p);
}

/*
 * Locate a directory in the cache and return the set of clearinhouses to 
 * try and contact.  
 *
 * The fullname is altered to reflect the directory which was found.
 * If no directory is found we build a list of all known clearinghouses
 * and set the name to null.  
 *
 * Also, name_p must be normalized for cache lookups.
 */
CHlist_t 
*ChooseInitCh (thread_t    *th_p,
               FullName_t  *name_p,
               int         new)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  CHlist_t *list_p = NULL;
 
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " >ChooseInitCh(,%s)",
      deb_ascii_FullName(nameBuf,name_p)));
 
 /* 
   * If last time through we used all known clearignhouses, 
   * nothing left to try now.
   */ 
  if (EQ_bytes(NullFullName, name_p, sizeof(NullFullName))) 
  {
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " <ChooseInitCh(,%s) return(list_p)",
        deb_ascii_FullName(nameBuf,name_p)));

    return(list_p);
  }

  /* Initialize used cache flag.  This flag controls caching later on */
  th_p->used_cached_replica = FALSE;
  th_p->taProgress.chlist_indiscriminate = FALSE;

  CALockData();
  
  for (;;) {
    /* 
     * If we have been here before, we need to strip the input name
     * again.  It currently represents the last name we used
     */
    if (new) {
      register DirEntry_t *dir_p;
      int idx;

      idx = CAHashDir(name_p);
      if ((dir_p = CAFindDir(name_p, idx)) != NULL) {
        Set_t *set_p;

	/* Move entry to front of LRU list */
	CALruMove(dir_p);

	/* Build linked list of clearinghouses to try */
	set_p = (Set_t *)SKIP_FullName(dir_p->dh_name);
	if ((list_p = BCHlist(set_p, th_p)) != NULL) {
	  th_p->used_cached_replica = TRUE;
	  break;
	}
      }
    }

    /* Do lookups from now on */
    new = TRUE;
    if (childStripRSimple(name_p) != DNS_SUCCESS) {
      /* No names left, find the GDA for this cell */
      list_p = ChooseCellCh(th_p);

      /* Set name as empty so we don't come back here */
      COPY_FullName(NullFullName, name_p);
      break;
    }
  } 

 DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      "  ChooseInitCh(,%s) return(%p)",
      deb_ascii_FullName(nameBuf,name_p),
      list_p));

  CAUnlockData();

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " <ChooseInitCh"));

  return(list_p);
}  

/* 
 * Find the best clearinghouse to satisfy a request.  Best is defined
 * to be the clearinghouse which is most likely to give a speedy 
 * reply.
 * This call may alter list_p when it removes replicas.
 */
CHlist_t 
*ChooseCh (thread_t  *th_p,
           CHlist_t  **list_pp,
           int       cnt)
{
  DEB_ASCII_BUF_FullName(ch_nameBuf)
  register CHlist_t *best_p, *next_p, *replica_p;
  int loop_control, i;
  int done;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " >ChooseCh()"));
  
  if (!*list_pp) 
  {
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " <ChooseCh() return(NULL)"));

    return(NULL);
  }

  /*
   * This first time through, see if the last clearinghouse we contacted
   * is on the list and if so use it.  This is a hack at an attempt to
   * get applciations that do creates/updates to contact the same
   * replica.  For high confidence, skip this.
   */
  if ((cnt == 0)  && (th_p->clerkConf != CO_high)) {
    for (best_p = *list_pp; best_p; best_p = best_p->ch_next)
      if (EQ_ObjUID(th_p->lastch, best_p->ch_id))
	goto foundit;
  }
  
  /* If hit a zero address count, means we are at the back of the list and
   * it is actually already ordered, cause we tried its hint already.
   * Just return the first one
   */
  if ((*list_pp)->ch_count == 0) {
      best_p = *list_pp;
      goto foundit;
  }

  /* The list is ordered, the first element is the best type available. */
  next_p = *list_pp;
#ifdef IS_GDA
  if (next_p->ch_flags.flags.gda)
    loop_control = IS_GDA;
  else
#endif
  if (next_p->ch_flags.flags.onLAN && next_p->ch_flags.flags.ok)
    loop_control = ONLAN_CONNECTION;
  else if (next_p->ch_flags.flags.ok)
    loop_control = HAVE_CONNECTION;
  else 
    loop_control = IS_NOTOKWITHCNT;

  /* 
   * Loop through replicas until we find the best one.
   * A category has been choosen based on the first element in the list.
   * The list is ordered by category.  Now try the ones in the choosen
   * category that have an open connection.
   * Pass through this loop a maximum of 2 times.
   */

  best_p = NULL;
  for (i=0; i<=1; ++i, ++loop_control) {
    done = 0;
    while (!done && next_p) {
      replica_p = NULL;
      switch (loop_control) {
      case ONLAN_CONNECTION:
	if (!next_p->ch_flags.flags.onLAN || !next_p->ch_flags.flags.ok)
	  ++done;
	else if (is_handle_cached(th_p, (uuid_t *)next_p->ch_id))
	  replica_p = next_p;
	break;
      case ONLAN_NOCONNECTION:
	if (!next_p->ch_flags.flags.onLAN || !next_p->ch_flags.flags.ok)
	  ++done;
	else
	  replica_p = next_p;
	break;
      /* The remaining cases have not ordering so don't set done
       * assumeing the case all entries in the case have been convered
       */
      case HAVE_CONNECTION:
	if (!next_p->ch_flags.flags.ok) 
	  ++done;
	else if (is_handle_cached(th_p, (uuid_t *)next_p->ch_id))
	  replica_p = next_p;
	break;
      case HAVE_NOCONNECTION:
	if (!next_p->ch_flags.flags.ok) 
	  ++done;
	else 
	    replica_p = next_p;
	break;
      case IS_NOTOKWITHCNT:
	if (next_p->ch_count == 0)
	  ++done;
	else
	  replica_p = next_p;
	break;
      case IS_NOTOKWITHOUTCNT:
#ifdef IS_GDA
      case IS_GDA:
#endif
      default:
	replica_p = next_p;
        break;
      } /* End switch */
      
      /* 
       * If found a replica for this category, see if another one of
       * equal priority has already been found.  If so choose one at random
       */
      if (replica_p)
	if (best_p) {
	  /* 
	   * Choose between replica_p and best_p.  Use low bit of
           * an address in the cache to decide.  We walk through the 
           * entire cache so the content should be random.
           */
	  if (!max_p) {
	    max_p = (byte_t *)CAfixed_p + CAfixed_p->ca_size;
	    cur_p = (byte_t *)CAfixed_p;
	  }

	  if (cur_p >= max_p)
	    cur_p = (byte_t *)CAfixed_p;

	  /* Check low bit and update pointer */
	  if (GET8(cur_p) & 1)
	    best_p = replica_p;
	}
	else
	  best_p = replica_p;
      next_p = next_p->ch_next;
    } /* End loop through replicas */
    if (best_p)
      break;
    next_p = *list_pp;
  } /* End loop through priority list */ 

foundit:
  if (best_p) {
    /* Remove from list and return it */
    if (*list_pp == best_p)
      *list_pp = best_p->ch_next;
    else
      best_p->ch_prev->ch_next = best_p->ch_next;
    
    if (best_p->ch_next)
      best_p->ch_next->ch_prev = best_p->ch_prev;
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " <ChooseCh() return(best_p :  %p->%s)",
      best_p,
      deb_ascii_FullName(ch_nameBuf,best_p->ch_name)));

  return(best_p);
}


/*
 * Find the cell root directory entry (if there is one)
 * and use all the clearinghouses therein.
 * This will get us pointers to the GDA.
 */
static CHlist_t 
*ChooseCellCh (thread_t *th_p)
{
    register CHlist_t *list_p = NULL;
    register DirEntry_t *dir_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " >ChooseCellCh()"));

    if ((dir_p = CAFindCell()) != NULL) {
      Set_t *const set_p = (Set_t *)SKIP_FullName(dir_p->dh_name);

      /* Move entry to front of LRU list */
      CALruMove(dir_p);

      /* Build linked list of clearinghouses to try */
      if ((list_p = BCHlist(set_p, th_p)) != NULL)
	th_p->used_cached_replica = TRUE;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug7,
        " <ChooseCellCh() return(list_p=%p)",
        list_p));

    return(list_p);
}

/*
 * This routine process requests for high confidence.  It forces
 * the operation to occur only at the master replica.
 */
CHlist_t 
*ChooseMaster (thread_t    *th_p,
               FullName_t  *name_p)
{
  DEB_ASCII_BUF_FullName(nameBuf)
  FullName_t tmpname;
  CHlist_t *list_p = NULL;
  Set_t *set_p;
  AttributeContents_t *contents_p;
  Timestamp_t *context_p;
  MemberValue_t *member_p;
  int firsttime = TRUE;
  int status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " >ChooseMaster(%s)",
      deb_ascii_FullName(nameBuf,name_p)));

  contents_p = (AttributeContents_t *)dns_malloc(MAX_MSG_SIZE);
  if (!contents_p) 	
  {
      th_p->clerkStatus = DNS_RESOURCEERROR;

      DCE_SVC_DEBUG((
         cds__svc_handle,
         cds_svc_child,
         svc_c_debug7,
         " <ChooseMaster"));

      return(NULL);
  }

  /* Initialize used cache flag.  This flag controls caching later on */
  th_p->used_cached_replica = FALSE;
  th_p->taProgress.chlist_indiscriminate = FALSE;
  context_p = NullTimestamp;

  /* Get copy of name for dirorobj case, where name_p is name of object
   * and we have to strip
   */
  COPY_FullName(name_p, &tmpname);

  while (TRUE) {

  /*
   * Get replica set of directory, use dirorObj to catch cases where names
   * is really an object.. Not, cannot do this for childptr lookup below
   */
    status = read_att(th_p, &tmpname, ET_dirOrObj,
		     dns_AttributeName(DNS_REPLICAS),
		     context_p, BL_false, MAX_MSG_SIZE, contents_p,
		     CO_medium, 0);
    if (status != DNS_SUCCESS) {
      th_p->clerkStatus = status;
      break;
    }

    if (EXT8(contents_p->ac_type) != AT_set) 
      break;

    set_p = (Set_t *)contents_p->ac_set;

     /* If set is absent or empty, assume the name is an object, strip and try
     * again.  It could also happen with a failed newepoch.
     */
    if ((FLAG_CLEAR(set_p->st_flag, ST_present)) || (NUM_Set(set_p) == 0)) {
      if (firsttime == FALSE)
	  /* Bumping confidence to high will cause a loop, no other choice but
	   * to terminate
	   */
	  break;

      (void)childStripRSimple(&tmpname);
    } else {

      /* Process set looking for master - probably first one */
      list_p = BuildCHlist(set_p, th_p);
      if (list_p)
	break;

      /* Get More of the set */
      member_p = (MemberValue_t *)INDEX_Set(set_p, NUM_Set(set_p) - 1);
      context_p = (Timestamp_t *)member_p->mv_timestamp;
    }
    firsttime = FALSE;
  }

  /* If didn't get a master, try reading child pointer - 
   * root doesn't have one - with global names may not beable to detect root
   */
  if (!list_p && (LEN_FullName(&tmpname) != sizeof(NullFullName))) {
      context_p = NullTimestamp;
      while (TRUE) {
	  /* Get replica set of directory */
	  status = read_att(th_p, name_p, ET_childPointer, 
		     dns_AttributeName(DNS_REPLICAS),
		     context_p, BL_false, MAX_MSG_SIZE, contents_p,
		     CO_medium, 0);
	  if (status != DNS_SUCCESS) {
	      th_p->clerkStatus = status;
	      break;
	  }

	  if (EXT8(contents_p->ac_type) != AT_set) 
	    break;

	  set_p = (Set_t *)contents_p->ac_set;

	  /* If set is absent or empty, cannot go on.
	   * This could happen with a failed newepoch.
	   * Bumping confidence to high will cause a loop, no other choice but
	   * to terminate 
	   */
	  if ((FLAG_CLEAR(set_p->st_flag, ST_present)) || 
	      (NUM_Set(set_p) == 0))
	    break;


	  /* Process set looking for master - probably first one */
	  list_p = BuildCHlist(set_p, th_p);
	  if (list_p)
	    break;

	  /* Get More of the set */
	  member_p = (MemberValue_t *)INDEX_Set(set_p, NUM_Set(set_p) - 1);
	  context_p = (Timestamp_t *)member_p->mv_timestamp;
      }
  }

  dns_free((char *)contents_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " <ChooseMaster"));

  return(list_p);
}
