/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: auth_at.c,v $
 * Revision 1.1.6.1  1996/10/02  17:50:25  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:38  damon]
 *
 * Revision /main/HPDCE02/5  1995/06/22  20:14 UTC  maunsell_c
 * 	mothra C&S fixes
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/13  1995/06/20  15:11 UTC  brezak
 * 	Return correct value when table is empty in at_atname()
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/12  1995/06/15  03:24 UTC  brezak
 * 	Assert that tables are allocated when they should be.
 * 	Make hash functions macros.
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/11  1995/06/14  22:14 UTC  brezak
 * 	Remove goto in at_add().
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/10  1995/06/12  22:40 UTC  brezak
 * 	Use macros for hash-list handling.
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/9  1995/06/12  14:04 UTC  brezak
 * 	Don't incr number of entries on a replace.
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/8  1995/06/09  03:08 UTC  brezak
 * 	Fixes from Transarc
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/7  1995/06/08  18:44 UTC  brezak
 * 	Include netinet/in.h for sockaddr_in.
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/6  1995/06/07  21:36 UTC  brezak
 * 	Fix at_add to only use existing entry if all keys are the same
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/5  1995/06/02  03:11 UTC  brezak
 * 	Don't pass null ptr to icl_Trace() in at_atname().
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/4  1995/06/01  21:58 UTC  brezak
 * 	Use MAXHOSTNAMELEN+1 for size of tbuffer[].
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/3  1995/05/30  19:05 UTC  brezak
 * 	at_atname() return NULL if requested name is strlen == 0.
 * 
 * Revision /main/HPDCE02/brezak_dfsgw/2  1995/05/26  17:26 UTC  brezak
 * 	Cleanup.
 * 
 * Revision /main/HPDCE02/brezak_mothra10/1  1995/05/18  16:23 UTC  brezak
 * 	Hide u. area access with appropriate osi functions.
 * 	Fixup at_hit() macro and deref addr.
 * 
 * Revision /main/HPDCE02/4  1995/04/28  20:46 UTC  maunsell_c
 * 	fix at_phit macro (again) m->pag needs &(m)->pag syntax
 * 
 * Revision /main/HPDCE02/maunsell_mothra1.48/1  1995/04/28  17:22 UTC  maunsell_c
 * 	fix at_phit - need & for m->pag also, no more cut & paste for me
 * 
 * Revision /main/HPDCE02/3  1995/04/26  17:58 UTC  root
 * 	add ampersand to at_phit per charleen
 * 
 * Revision /main/HPDCE02/2  1995/04/06  18:09 UTC  maunsell_c
 * 	remove old dbug code
 * 
 * Revision /main/HPDCE02/maunsell_mothra1.47/2  1995/04/05  19:02 UTC  maunsell_c
 * 	erroneously deleted  a = (pag & 0x000000ff); in phash
 * 
 * Revision /main/HPDCE02/maunsell_mothra1.47/1  1995/04/05  13:58 UTC  maunsell_c
 * 	remove changes used for debug
 * 
 * Revision /main/HPDCE02/3  1995/04/26  17:58 UTC  root
 * 	add ampersand to at_phit per charleen
 * 
 * Revision /main/HPDCE02/2  1995/04/06  18:09 UTC  maunsell_c
 * 	remove old dbug code
 * 
 * Revision /main/HPDCE02/maunsell_mothra1.47/2  1995/04/05  19:02 UTC  maunsell_c
 * 	erroneously deleted  a = (pag & 0x000000ff); in phash
 * 
 * Revision /main/HPDCE02/maunsell_mothra1.47/1  1995/04/05  13:58 UTC  maunsell_c
 * 	remove changes used for debug
 * 
 * Revision /main/HPDCE02/1  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/maunsell_mothra1.45/5  1995/02/09  21:50 UTC  maunsell_c
 * 	@sys @host audit changes
 * 
 * Revision /main/maunsell_mothra1.45/4  1995/02/02  19:35 UTC  maunsell_c
 * 	remove hash_val var - not used
 * 
 * Revision /main/maunsell_mothra1.45/3  1995/02/01  14:19 UTC  maunsell_c
 * 	comment on changes for at_add @sys @host and
 * 	at_query updating mapping expire field as
 * 	well as pag field.
 * 
 * Revision /main/maunsell_mothra1.45/2  1995/01/31  23:16 UTC  maunsell_c
 * 	remove sysincludes, param.h already included .
 * 
 * Revision /main/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
 * 	add @sys @host support for dfs_login/dfsgwd/dfsgw
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:34  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:58:05  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:07:22  mckeen]
 * 
 * $EndLog$
 */
/*
 * auth_at.c - Authentication table (AT) implementation.
 *
 * This module contains the routines that implement the 
 * heart of the AT.  
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/user.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <dcedfs/osi.h>
#include <dcedfs/auth_at.h>
#include <dcedfs/lock.h>
#include <dcedfs/icl.h>
#include <dcedfs/dfsgw_trace.h>

#ifndef KERNEL
#include "authtest.h"
#endif

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/gateway/libgwauth/auth_at.c,v 1.1.6.1 1996/10/02 17:50:25 damon Exp $")

extern struct icl_set *dfsgw_iclSetp;

/*
 * List definitions.
 */
#define LIST_HEAD(name, type)						\
struct name {								\
	struct type *lh_first;	/* first element */			\
}

#define LIST_ENTRY(type)						\
struct {								\
	struct type *le_next;	/* next element */			\
	struct type **le_prev;	/* address of previous next element */	\
}

/*
 * List functions.
 */
#define	LIST_INIT(head) {						\
	(head)->lh_first = NULL;					\
}

#define LIST_INSERT_HEAD(head, elm, field) {				\
	if (((elm)->field.le_next = (head)->lh_first) != NULL)		\
		(head)->lh_first->field.le_prev = &(elm)->field.le_next;\
	(head)->lh_first = (elm);					\
	(elm)->field.le_prev = &(head)->lh_first;			\
}

#define LIST_REMOVE(elm, field) {					\
	if ((elm)->field.le_next != NULL)				\
		(elm)->field.le_next->field.le_prev = 			\
		    (elm)->field.le_prev;				\
	*(elm)->field.le_prev = (elm)->field.le_next;			\
}

/* 
 * Number of entries in first level hash table 
 */
#define AT_HTBL_NENTRIES	(128)		

/*
 * The AT is implemented as a hash table.  This ensures
 * quick access to individual elements without compromising
 * too much when it comes to listing the whole thing.
 */

typedef struct at_htble {
	LIST_ENTRY(at_htble) hash;	/* Next entry in hash chain */
	at_pag_mapping_t mapping;	/* The mapping itself */
} at_htble_t, *at_htble_p;

typedef struct at_phtble {
	LIST_ENTRY(at_phtble) hash;	/* Next entry in hash chain */
	at_pag_mapping_p mp;		/* pointer to associated mapping */
} at_phtble_t, *at_phtble_p;

/*
 * AT_THASH - Hash function (node ID, user ID) to table entry.
 * 
 * Attempt a reasonable hash.  Assembling a selection 
 * of the address by picking the last and first two bytes 
 * out of the address.
 * Then xor it with the uid.
 * 
 * Returns index into hash table.
 */
#define AT_THASH(addr, uid)						\
	(((((addr)->sa_data[13] << 24) | ((addr)->sa_data[14] << 16) |	\
	   ((addr)->sa_data[0] << 8) | ((addr)->sa_data[1])) ^ uid)	\
        & htbl_hash)

/*
 * AT_PHASH - Hash function (PAG) to table entry.
 *
 * Attempt a reasonable hash. Use lower 8 bits of PAG 
 * Returns index into hash table.
 */
#define AT_PHASH(pag)					\
	(((pag) & 0x000000ff) & phtbl_hash)

/*
 * Quick little macro for comparing at_pag_mapping_p to at_htble_p.
 */
#define AT_HIT(m, e) \
    ((bcmp((char *)&(m)->addr, (char *)&(e)->mapping.addr, \
	   sizeof(struct sockaddr)) == 0) && \
     ((m)->uid == (e)->mapping.uid))

static LIST_HEAD(at_htbl_hdr, at_htble) *at_htbl;	/* direct hash table */
static u_long htbl_hash;
static long htbl_size;
static long htbl_nbuckets;

static LIST_HEAD(at_phtbl_hdr, at_phtble) *at_phtbl;	/* PAG hash table */
static u_long phtbl_hash;
static long phtbl_size;
static long phtbl_nbuckets;

static int at_nentries = 0;		/* Total number of valid entries */
struct lock_data atlock;		/* Lock for AT database */

/*
 * General routine to allocate a hash table.
 */
static void *
hashinit(elements, nbuckets, tablesize, hashmask)
int elements;
long *nbuckets;
long *tablesize;
u_long *hashmask;
{
  long hashsize;
  LIST_HEAD(generic, generic) *hashtbl;
  int i;

  *nbuckets = 0;

  osi_assert(elements > 0);
  
  for (hashsize = 1; hashsize <= elements; hashsize <<= 1)
    continue;
  hashsize >>= 1;
  
  hashtbl = osi_Alloc((u_long)hashsize * sizeof(*hashtbl));
  if (hashtbl == NULL)
    return(NULL);
  
  *tablesize = (u_long)hashsize * sizeof(*hashtbl);
  
  for (i = 0; i < hashsize; i++)
    LIST_INIT(&hashtbl[i]);
  *hashmask = hashsize - 1;
  *nbuckets = hashsize;
  return (hashtbl);
}

/*
 * at_query - Reference the database for existence of a mapping.
 *
 * This routine takes the mapping parameter and uses the uid and addr fields
 * to see if there is a corresponding entry in the database.  If the query
 * is successful, 0 is returned and the proper PAG value is set in the mapping
 * structure.  Otherwise -1 is returned.
 */
int
at_query(at_pag_mapping_p mapping)
{
  at_htble_p ep = NULL;	/* Pointer to entry */
  u_int hash;
  
  icl_Trace2(dfsgw_iclSetp, DFSGW_TRACE_ENTER_QUERY,
	     ICL_TYPE_LONG, mapping->uid,
	     ICL_TYPE_LONG, 
	     ((struct sockaddr_in *)&mapping->addr)->sin_addr.s_addr);
  
  lock_ObtainRead(&atlock);
  if (at_nentries > 0) {
    /*
     * Perform hash, search list looking for hit. 
     */
    osi_assert(at_htbl != NULL);
    hash = AT_THASH(&mapping->addr, mapping->uid);
    for (ep = at_htbl[hash].lh_first; ep; ep = ep->hash.le_next) {
      if (AT_HIT(mapping, ep)) {
	/*
	 * Hit, copy out entry.
	 */
	mapping->pag = ep->mapping.pag;	
	mapping->expiration_time = 
	  ep->mapping.expiration_time;
	bcopy((char *)&ep->mapping.sysname, (char *)&mapping->sysname, 
	      sizeof(mapping->sysname));
	bcopy((char *)&ep->mapping.hostname, (char *)&mapping->hostname, 
	      sizeof(mapping->hostname));
	break;
      }
    }
  }
  lock_ReleaseRead(&atlock);
  
  icl_Trace2(dfsgw_iclSetp, DFSGW_TRACE_END_QUERY,
	     ICL_TYPE_LONG, (ep ? mapping->pag : 0),
	     ICL_TYPE_LONG, (ep ? mapping->expiration_time : 0));
  
  return(ep ? 0 : -1);
}

/*
 * at_add - Add an entry to the AT.
 *
 * This routine takes an at_pag_mapping_p and inserts it into the AT.
 * If a mapping already exists for the given addr/uid tuple, it is replaced
 * with the new PAG.  Upon success, 0 is returned, otherwise an error code
 * is returned.
 */
int
at_add(at_pag_mapping_p mapping, int subcmd)
{
  at_htble_p ep;		
  at_phtble_p pep;
  at_htble_p enew;		/* Pointer to new element */ 
  at_phtble_p penew;
  u_int hash, phash;
  u_long pag;
  uid_t uid;

  int newsysname = 0;
  int newhostname = 0;
	
  if ((subcmd < 0) || (subcmd >= AT_ADD_NSUBCMDS)) 
    return EINVAL;

  if (mapping->sysname[0])
    newsysname = 1;
  if (mapping->hostname[0])
    newhostname = 1;

  /*
   * Allocate the hash tables if they aren't there
   */
  
  if (at_nentries == 0) {
    osi_assert(at_htbl == NULL);
    at_htbl  = hashinit(AT_HTBL_NENTRIES, &htbl_nbuckets, 
			&htbl_size, &htbl_hash);
    if (at_htbl == NULL)
      return(ENOMEM);
    
    osi_assert(at_phtbl == NULL);
    at_phtbl = hashinit(AT_HTBL_NENTRIES, &phtbl_nbuckets, 
			&phtbl_size, &phtbl_hash);
    if (at_phtbl == NULL) {
      osi_Free(at_htbl, htbl_size);
      at_htbl = NULL;
      return(ENOMEM);
    }
  }

  /*
   * Assume we'll need new element and obtain before grabbing lock.
   */
  enew = (at_htble_p)osi_Zalloc(sizeof(at_htble_t));
  if (enew == NULL)
    return(ENOMEM);
  penew = (at_phtble_p)osi_Zalloc(sizeof(at_phtble_t));
  if (penew == NULL) {
      osi_Free(enew, sizeof(at_phtble_t));
      return(ENOMEM);
  }

  /*
   * Superuser can set up a mapping with
   * the supplied PAG and UID. But a user must use the
   * existing user creds.
   *
   */
  if (osi_suser(osi_getucred())) {
    uid = mapping->uid;
    pag = mapping->pag;
  } else {
    if (subcmd == AT_ADD_NEW_ENTRY) {
      uid = osi_GetUID(osi_getucred());
      pag = osi_GetPagFromCred(osi_getucred());
      if ((mapping->uid != uid) || (mapping->pag != pag)) {
	icl_Trace4(dfsgw_iclSetp, DFSGW_TRACE_ENTER_ADD_PERM,
		   ICL_TYPE_LONG, mapping->uid,
		   ICL_TYPE_LONG, uid,
		   ICL_TYPE_LONG, mapping->pag,
		   ICL_TYPE_LONG, pag);
	return (EPERM);
      }
      icl_Trace2(dfsgw_iclSetp, DFSGW_TRACE_ENTER_ADD,
		 ICL_TYPE_LONG, uid, 
		 ICL_TYPE_LONG, pag);
    } else if (subcmd == AT_ADD_UPDATE_EXPTIME) {
      uid = osi_GetUID(osi_getucred());
      if (mapping->uid != uid) {
	icl_Trace2(dfsgw_iclSetp, 
		   DFSGW_TRACE_ENTER_UPDATE_EXPTIME_PERM,
		   ICL_TYPE_LONG, mapping->uid,
		   ICL_TYPE_LONG, uid);
	return (EPERM);
      }
      /* We will ensure the pag in the installed entry matches the
       * the passed in pag */
      pag = mapping->pag;
      icl_Trace2(dfsgw_iclSetp, DFSGW_TRACE_ENTER_UPDATE_EXPTIME,
		 ICL_TYPE_LONG, uid, 
		 ICL_TYPE_LONG, pag); 
    }
  }
  
  lock_ObtainWrite(&atlock);
  /*
   * Search list and see if we already have an entry for this 
   * mapping.  If so then we can just copy over the existing one
   * with new information.
   */
  osi_assert(at_htbl != NULL);
  osi_assert(at_phtbl != NULL);
  hash = AT_THASH(&mapping->addr, mapping->uid);
  phash = AT_PHASH(mapping->pag);

  for (ep = at_htbl[hash].lh_first; ep; ep = ep->hash.le_next) {
    if (AT_HIT(mapping, ep)) {
      int rhash;
      
      /* 
       * There are 2 hash tables, one indexed by host/uid
       * and the other by PAG. The CM uses the PAG hash
       * table to translate @sys and @host, since it
       * deals with PAG's not netids.
       */
      rhash = AT_PHASH(ep->mapping.pag);
      for (pep = at_phtbl[rhash].lh_first; pep; pep = pep->hash.le_next) {
	osi_assert(pep->mp);
	if (&ep->mapping == pep->mp) {
	  break;
	}
      }
      /* The mapping should be found */
      osi_assert(pep != NULL);

      if (subcmd == AT_ADD_UPDATE_EXPTIME) {
	osi_assert(pag == ep->mapping.pag);
	ep->mapping.expiration_time = mapping->expiration_time;
	if (newsysname)
	  strcpy((char *)&(ep->mapping.sysname), (char *)&mapping->sysname);
	if (newhostname) 
	  strcpy((char *)&(ep->mapping.hostname), (char *)&mapping->hostname);
	lock_ReleaseWrite(&atlock);
	osi_Free(enew, sizeof(at_htble_t));
	osi_Free(penew, sizeof(at_phtble_t));
	icl_Trace0(dfsgw_iclSetp, DFSGW_TRACE_END_UPDATE_EXPTIME);
	return(0);
      } else {
	if (ep->mapping.pag != pag) {
	  icl_Trace2(dfsgw_iclSetp, DFSGW_TRACE_ENTER_ADD_DEL,
		     ICL_TYPE_LONG, ep->mapping.pag,
		     ICL_TYPE_LONG, pag);
	  
	  ep->mapping.pag = pag;
	  ep->mapping.expiration_time = 
	    mapping->expiration_time;
	  if (newsysname)
	    strcpy((char *)&(ep->mapping.sysname), (char *) &mapping->sysname);
	  if (newhostname) 
	    strcpy((char *)&(ep->mapping.hostname), 
		   (char *)&mapping->hostname);
	  LIST_REMOVE(pep, hash);
	  osi_Free(pep, sizeof(at_phtble_t));		    

	  /* link phash table entry to hash table */
	  penew->mp = &ep->mapping;
	  LIST_INSERT_HEAD(&at_phtbl[phash], penew, hash);
	  
	  lock_ReleaseWrite(&atlock);
	  osi_Free(enew, sizeof(at_htble_t));
	  icl_Trace0(dfsgw_iclSetp, DFSGW_TRACE_END_ADD_NEW);
	  return(0);
	} else {
	  ep->mapping.expiration_time = mapping->expiration_time;
	  if (newsysname)
	    strcpy((char *)ep->mapping.sysname, (char *)&mapping->sysname);
	  if (newhostname)
	    strcpy((char *)ep->mapping.hostname, (char *)&mapping->hostname);
	  lock_ReleaseWrite(&atlock);
	  osi_Free(enew, sizeof(at_htble_t));
	  osi_Free(penew, sizeof(at_phtble_t));
	  icl_Trace0(dfsgw_iclSetp, DFSGW_TRACE_END_ADD_NEW);
	  return(0);
	}
      }
    }
  }

  /* 
   * Need to add new entries. Write new entry and link into hash lists. 
   */

  at_nentries++;

  bcopy((char *)&mapping->addr, (char *)&(enew->mapping.addr), 
	sizeof(struct sockaddr));
  enew->mapping.uid = uid;
  enew->mapping.pag = pag;
  enew->mapping.expiration_time = mapping->expiration_time;
  strcpy((char *)&(enew->mapping.sysname), (char *)&mapping->sysname);
  strcpy((char *)&(enew->mapping.hostname), (char *)&mapping->hostname);

  /* link phash table entry to hash table */
  penew->mp = &enew->mapping;

  LIST_INSERT_HEAD(&at_htbl[hash], enew, hash);
  LIST_INSERT_HEAD(&at_phtbl[phash], penew, hash);

  lock_ReleaseWrite(&atlock);
  icl_Trace0(dfsgw_iclSetp, DFSGW_TRACE_END_ADD_NEW);
  return(0);
}

/*
 * at_delete - Delete an entry from the AT.
 *
 * This routine takes an at_pag_mapping_p and using the addr and using
 * the corresponding addr and uid entries, deletes a mapping if it exists
 * from the AT.  Upon success, 0 is returned, otherwise some errno is
 * returned.
 */
int
at_delete(at_pag_mapping_p mapping)
{
  at_htble_p ep;
  at_phtble_p pep;
  u_int hash, phash;
  int code = ENOENT;

  icl_Trace2(dfsgw_iclSetp, DFSGW_TRACE_ENTER_DELETE,
	     ICL_TYPE_LONG, mapping->uid, ICL_TYPE_LONG, mapping->pag);

  if (at_nentries == 0) {
    icl_Trace0(dfsgw_iclSetp, DFSGW_TRACE_END_DELETE_NOENT);
    return(code);
  }
  
  lock_ObtainWrite(&atlock);
  /*
   * Perform hash, search list looking for hit. 
   */
  osi_assert(at_htbl != NULL);
  hash = AT_THASH(&mapping->addr, mapping->uid);
  for (ep = at_htbl[hash].lh_first; ep; ep = ep->hash.le_next) {
    if (AT_HIT(mapping, ep)) {
      osi_assert(at_phtbl != NULL);
      phash = AT_PHASH(ep->mapping.pag);
      for (pep = at_phtbl[phash].lh_first; pep; pep = pep->hash.le_next) {
	osi_assert(pep->mp);
	/* Hit by pag, remove @sys @host from list */
	if (&ep->mapping == pep->mp)
	  break;
      }
      
      /* These always travel in pairs */
      osi_assert(pep != NULL);
      
      LIST_REMOVE(ep, hash);
      osi_Free(ep, sizeof(at_htble_t));
      
      LIST_REMOVE(pep, hash);
      osi_Free(pep, sizeof(at_phtble_t));
      
      at_nentries--;
      
      icl_Trace0(dfsgw_iclSetp, DFSGW_TRACE_END_DELETE_FOUND);
      code = 0;
      break;
    }
  }

  if (code == ENOENT) 
    icl_Trace0(dfsgw_iclSetp, DFSGW_TRACE_END_DELETE_NOENT);

  /*
   * Free the hash tables if empty
   */
  if (at_nentries == 0) {
    osi_assert(at_htbl != NULL);
    osi_Free(at_htbl, htbl_size);
    at_htbl = NULL;
    
    osi_assert(at_phtbl != NULL);
    osi_Free(at_phtbl, phtbl_size);
    at_phtbl = NULL;
  }
  lock_ReleaseWrite(&atlock);
  return (code);
}

/*
 * at_size - Return number of entries in the AT.
 */
int
at_size(void)
{
  icl_Trace1(dfsgw_iclSetp, DFSGW_TRACE_SIZE, 
	     ICL_TYPE_LONG, at_nentries);
  return(at_nentries);
}

/*
 * at_list - Copyout mapping entries.
 *
 * This routine copies up to nmapping mapping entries out.  It assumes that
 * the pointers provided are user space pointers and must be accessed only
 * by using the copyin/copyout kernel routines.  After successful completion, 
 * 0 is returned, and the actual number of entries copied out is reflected 
 * in the nmapping parameter.  Upon failure, some errno is returned.
 */
int
at_list(int *nmappings, at_pag_mapping_p mappings)
{
  int n;			/* Number the user wants */
  at_htble_p ep;		/* Pointer to entries */
  int i;			/* Index we are on */
  int code = 0;

  icl_Trace0(dfsgw_iclSetp, DFSGW_TRACE_ENTER_LIST);
  
  /*
   * First figure out what we can copyout.
   */
  if (code = osi_copyin((caddr_t)nmappings, (caddr_t)&n, sizeof(int))) {
    icl_Trace2(dfsgw_iclSetp, DFSGW_TRACE_END_LIST_ERR,
	       ICL_TYPE_STRING, "copyin", ICL_TYPE_LONG, code);
    return(code);
  }
  lock_ObtainRead(&atlock);

  /*
   * Tell user how many they are getting
   * - if copyout fails, then done
   */
  if (at_nentries < n) {
    n = at_nentries;
    if (code = osi_copyout((caddr_t)&n, (caddr_t)nmappings, sizeof(int)))
      goto done;
  }
  /*
   * Now scan down hash table and copyout mapping structures.
   */
  i = 0;
  while (n > 0) {
    osi_assert(i < htbl_nbuckets);	
    for (ep = at_htbl[i++].lh_first; ep && (n > 0); ep = ep->hash.le_next) {
      if (code = osi_copyout((caddr_t)&ep->mapping, (caddr_t)mappings, 
			     sizeof(at_pag_mapping_t))) {
	goto done;
      }
      /* Use system default ? */
      if (strlen(ep->mapping.sysname) == 0) {
	char *sysname;

	sysname = (cm_sysname == 0) ? SYS_NAME : cm_sysname;
	if (code = osi_copyout((caddr_t)sysname, (caddr_t)mappings->sysname, 
			       sizeof(mappings->sysname))) 
	  goto done;
      }
      /* Use hostname default ? */
      if (strlen(ep->mapping.hostname) == 0) {
	char tbuffer[OSI_MAXHOSTNAMELEN+1];

	if (osi_GetMachineName(tbuffer, sizeof(tbuffer)) != 0)
	  strcpy(tbuffer, "unknown");
	if (code = osi_copyout((caddr_t)tbuffer, (caddr_t)mappings->hostname, 
			       sizeof(mappings->hostname))) 
	  goto done;
      }
      n--;
      mappings++;
    }
  }

 done:
  lock_ReleaseRead(&atlock);
  icl_Trace2(dfsgw_iclSetp, DFSGW_TRACE_END_LIST_ERR,
	     ICL_TYPE_STRING, "copyout", 
	     ICL_TYPE_LONG, code);
  return(code);
}

/*
 * at_atname - Reference the database for existence of @sys/host value.
 *
 * This routine takes the type and pag as in parameters. It uses the pag
 * to see if there is a corresponding entry in the database.  If the query
 * is successful, the appropriate @sys or @host value is returned in the
 * supplied name buffer that is size. If there is a string for the requested
 * entry type 1 is returned, otherwise 0 is returned.
 */
int
at_atname(int type, int pag, char *namep, int size)
{
  char *vp = NULL;
  at_phtble_p pep = NULL;	/* Pointer to entry */
  u_int phash;
  int len = 0;

  icl_Trace2(dfsgw_iclSetp, DFSGW_TRACE_ENTER_ATNAME,
	     ICL_TYPE_LONG, type, ICL_TYPE_LONG, pag);
  
  osi_assert(type == ATSYS || type == ATHOST);
  
  lock_ObtainRead(&atlock);
  if (at_nentries > 0) {
    /*
     * Perform hash, search list looking for hit. 
     */
    phash = AT_PHASH(pag);
    for (pep = at_phtbl[phash].lh_first; pep; pep = pep->hash.le_next) {
      osi_assert(pep->mp);
      if (pep->mp->pag == pag) {
	/*
	 * Hit, copy out @sys/@host      
	 */
	switch(type) {
	  case ATSYS:
	    vp = pep->mp->sysname;
	    break;
	  case ATHOST:
	    vp = pep->mp->hostname;
	    break;
	  default:
	    icl_Trace0(dfsgw_iclSetp, DFSGW_TRACE_END_ATNAME_UNKNOWN);
	    break;
	  }
	break;
      }
    }
  }
  if (vp) {
    len = strlen(vp);
    /* if returning name is empty, return NULL */
    if (len == 0)
      vp = NULL;
  }
  if (vp) {
    strncpy(namep, vp, size);
  }
	
  lock_ReleaseRead(&atlock);

  icl_Trace1(dfsgw_iclSetp, DFSGW_TRACE_END_ATNAME,
	     ICL_TYPE_STRING, vp ? vp : "(null)");

  return((len > 0) ? 1 : 0);
}

/*
 * at_gc - Garbage collect expired entries from the AT.
 *
 * This routine is called periodically to scan the AT and remove entries
 * whose lifetime has expired.
 */
int at_gc_calls = 0;

void 
at_gc(void)
{
  at_htble_p ep, nep;
  at_phtble_p pep;
  int i;			/* Index we are on */
  long now;			/* Current time */
  u_int phash;
  
  icl_Trace0(dfsgw_iclSetp, DFSGW_TRACE_ENTER_GC);
  at_gc_calls++;
  
  if (at_nentries == 0) {
    icl_Trace1(dfsgw_iclSetp, DFSGW_TRACE_END_GC,
	       ICL_TYPE_STRING, "no entries");
    return;
  }
  now = osi_Time();

  /*
   * Scan down hash table and find map entries.
   */
  lock_ObtainShared(&atlock);
  for (i = 0; ((at_nentries > 0) && (i < htbl_nbuckets)); i++) {
    for (ep = at_htbl[i].lh_first; ep; ep = nep) {
      nep = ep->hash.le_next;
      if (ep->mapping.expiration_time < now) {	
	lock_UpgradeSToW(&atlock);
	
	phash = AT_PHASH(ep->mapping.pag);
	for (pep = at_phtbl[phash].lh_first; pep; pep = pep->hash.le_next) {
	  osi_assert(pep->mp);
	  if (&ep->mapping == pep->mp) {
	    break;
	  }
	}
	/* These always travel in pairs */
	osi_assert(pep != NULL);
	
	LIST_REMOVE(pep, hash);
	LIST_REMOVE(ep, hash);
	
	at_nentries--;
	
	lock_ConvertWToS(&atlock);
	icl_Trace2(dfsgw_iclSetp, DFSGW_TRACE_GC_CLEAN,
		   ICL_TYPE_LONG, ep->mapping.uid,
		   ICL_TYPE_LONG, ep->mapping.pag);
	
	osi_Free(pep, sizeof(at_phtble_t));
	osi_Free(ep, sizeof(at_htble_t));
      }
    }
  }
  /*
   * Free the hash tables if empty
   */
  if (at_nentries == 0) {
    lock_UpgradeSToW(&atlock);
    
    osi_assert(at_htbl != NULL);
    osi_Free(at_htbl, htbl_size);
    at_htbl = NULL;
    
    osi_assert(at_phtbl != NULL);
    osi_Free(at_phtbl, phtbl_size);
    at_phtbl = NULL;
    
    lock_ConvertWToS(&atlock);
  }
  lock_ReleaseShared(&atlock);
  icl_Trace1(dfsgw_iclSetp, DFSGW_TRACE_END_GC, ICL_TYPE_STRING, "done");
}

