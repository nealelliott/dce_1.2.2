/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ubik.c,v $
 * Revision 1.1.84.1  1996/10/02  17:55:51  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:44  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1990, 1995, 1996 Transarc Corporation - All rights reserved */
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <dcedfs/icl.h>
#include <dcedfs/osi.h>
#include <dcedfs/compat.h>

#include <dce/rpc.h>
#include <dce/dce_cf.h>
#include <dce/pgo.h>
#include <dce/ep.h>
#include <dce/binding.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include <pthread.h>
#include <dcedfs/dfsauth.h>
#include <dcedfs/tpq.h>
#include <dcedfs/bomb.h>
#define UBIK_INTERNALS
#include <ubikdisk_proc.h>
#include <ubikvote_proc.h>
#include <ubik.h>
#include <utrace.h>

/* define event set structure for tracing and setup function */
struct icl_set *ubikvote_iclSetp = (struct icl_set *)0;
struct icl_set *ubikdisk_iclSetp = (struct icl_set *)0;

globaldef uuid_t uuid_nil;
#define UBIK_STACK_SIZE		150000
#define	CStampVersion	    1	    /* meaning set ts->version */

error_status_t
    /* provider version 1 */
    SUBIKDISK_Begin(),
    SUBIKDISK_Commit(),
    SUBIKDISK_Lock(),
    SUBIKDISK_Write(),
    SUBIKDISK_GetVersion(),
    SUBIKDISK_GetFile(),
    SUBIKDISK_SendFile(),
    SUBIKDISK_Abort(),
    SUBIKDISK_ReleaseLocks(),
    SUBIKDISK_Truncate(),
    SUBIKDISK_Probe(),
    SUBIKDISK_GetServerInterfaces(),
    /* provider version 2 */
    SUBIKDISK_BulkUpdate();

error_status_t
    /* provider version 1 */
    SUBIKVOTE_Beacon(),
    SUBIKVOTE_Debug(),
    SUBIKVOTE_SDebug(),
    SUBIKVOTE_GetServerInterfaces(),
    SUBIKVOTE_GetSyncSite(),
    /* provider version 2 */
    SUBIKVOTE_DebugV2(),
    SUBIKVOTE_SDebugV2(),
    SUBIKVOTE_GetSyncSiteIdentity();

globaldef UBIKVOTE_v4_0_epv_t UBIKVOTE_v4_0_epv = {
    /* provider version 1 */
    SUBIKVOTE_Beacon,
    SUBIKVOTE_Debug,
    SUBIKVOTE_SDebug,
    SUBIKVOTE_GetServerInterfaces,
    SUBIKVOTE_GetSyncSite,
    /* provider version 2 */
    SUBIKVOTE_DebugV2,
    SUBIKVOTE_SDebugV2,
    SUBIKVOTE_GetSyncSiteIdentity
};

globaldef UBIKDISK_v4_0_epv_t UBIKDISK_v4_0_epv = {
    /* provider version 1 */
    SUBIKDISK_Begin,
    SUBIKDISK_Commit,
    SUBIKDISK_Lock,
    SUBIKDISK_Write,
    SUBIKDISK_GetVersion,
    SUBIKDISK_GetFile,
    SUBIKDISK_SendFile,
    SUBIKDISK_Abort,
    SUBIKDISK_ReleaseLocks,
    SUBIKDISK_Truncate,
    SUBIKDISK_Probe,
    SUBIKDISK_GetServerInterfaces,
    /* provider version 2 */
    SUBIKDISK_BulkUpdate
};

globaldef uuid_t Budb_UbikObjectTypeUuid = 
{ /* 7458991c-f676-11c9-a595-02608c0ffee2 */
    0x7458991c, 0xf676, 0x11c9, 0xa5, 0x95, 
    {0x02, 0x60, 0x0f, 0xfe, 0xe2, 0x00}
};

globaldef uuid_t  Fldb_UbikObjectTypeUuid =
{ /* 5530816c-f676-11c9-a7c4-02608c0ffee2 */
    0x5530816c, 0xf676, 0x11c9, 0xa7, 0xc4,
    {0x02, 0x60, 0x0f, 0xfe, 0xe2, 0x00}
};

static int BeginTrans _TAKES((struct ubik_dbase *dbase,	/* in */
			      long transMode, /* in */
			      struct ubik_trans **transPtr, /* out */ 
			      int readAny
			      ));

/*  This system is organized in a hierarchical set of related modules.  Modules
    at one level can only call modules at the same level or below.
    
    At the bottom level (0) we have RPC, threads, i.e. operating system 
    primitives.
    
    At the next level (1) we have

	VOTER--The module responsible for casting votes when asked.  It is also
	responsible for determining whether this server should try to become
	a synchronization site.
	
	BEACONER--The module responsible for sending keep-alives out when a
	server is actually the sync site, or trying to become a sync site.
	
	DISK--The module responsible for representing atomic transactions
	on the local disk.  It maintains a new-value only log.
	
	LOCK--The module responsible for locking byte ranges in the database
	file.
	
    At the next level (2) we have
	  
	RECOVERY--The module responsible for ensuring that all members of a
	quorum have the same up-to-date database after a new synchronization
	site is elected.  This module runs only on the synchronization site.
	
    At the next level (3) we have
    
	REMOTE--The module responsible for interpreting requests from the sync
	site and applying them to the database, after obtaining the appropriate
	locks.
	
    At the next level (4) we have
    
	UBIK--The module users call to perform operations on the database.
 */


/* some globals */
SHARED struct ubik_server *ubik_servers;
SHARED struct ubik_dbase *ubik_dbase=0;
SHARED struct ubik_stats ubik_stats;
SHARED struct ubik_server ubik_host;
SHARED long ubik_epochTime = 0;
SHARED long urecovery_state = 0;
PRIVATE unsigned_char_t *serverNsGroupP;	/* store server's cds group */
SHARED unsigned_char_t *serverSecGroupP;	/* store server's sec group */
PRIVATE struct sockaddr_in serverHostAddr;	/* store server's sockaddr */

SHARED void *ubik_tpq_handle=(void *)NULL;

IMPORT void ubik_hold(), ubik_rele(), ubik_versionLock(), ubik_versionUnlock();
IMPORT void ubik_sleep();

extern int ubik_maxbulk;			/* max write vector size */

/* -----
 * Ubik CDS utility routines
 * -----
 */

#define IS_COMM_ERR(s)  (s == rpc_s_ok ? 0 : 1)

/* ubik_GroupEnumerate
 *	Determine the members in a name service group.
 * entry:
 *	groupNameP - ptr to the group name string
 *	membersPP - ptr to array of member names, freed by caller
 *	nGroup - ptr to vbl. for result
 * exit:
 *	nGroup - # of items in group
 *	membersPP - ptr to array of member names.
 *    	st - exit status
 */

PRIVATE void ubik_GroupEnumerate(groupNameP, serverInit, membersPP, 
				 nGroupP, st)
     IN unsigned_char_t *groupNameP;
     IN char serverInit;	/* servers set this, not clients */
     OUT unsigned_char_t ***membersPP;
     OUT long *nGroupP;
     OUT unsigned32 *st;
{
    rpc_ns_handle_t	inquiryContext;
    unsigned_char_t *memberNameP, *ourName=(unsigned_char_t *)NULL;
    unsigned_char_t *cellName = (unsigned_char_t *)NULL;
    error_status_t unusedSt;
    int foundUs=0;
    int i;

    *nGroupP = 0;
    *membersPP = (unsigned_char_t **)NULL;

    if (groupNameP == (unsigned_char_t *)NULL) {
      *st = UBADGROUP;
      return;
    }

    rpc_ns_group_mbr_inq_begin(DFS_ENTRY_SYNTAX, groupNameP,
			       DFS_ENTRY_SYNTAX,
			       &inquiryContext,
			       st);
    if(IS_COMM_ERR(*st))
	goto exit;

    /* All error cases before this line should not go exit label */
    /* 
     * Let's allocate memory for UBIK_MAXSERVERS members, we can prune it 
     * down if we don't need it.
     */
    *membersPP = (unsigned_char_t **)malloc(5 + (UBIK_MAXSERVERS * \
					    sizeof(unsigned_char_t *)));
    if (*membersPP == (unsigned_char_t **)NULL) {
      *st = UNOMEM;
      goto exit;
    }

    if (serverInit) {
	/* Let's get our name, so that we can avoid including it in member list,
	   if necessary */
	dce_cf_binding_entry_from_host(NULL, (char **)&ourName, st);
	if ( *st != dce_cf_st_ok )
	  goto error_exit;
	/* Get cell name, because, host binding is cell relative */
	dce_cf_get_cell_name((char **)&cellName, (unsigned long *)st);
	if (*st != dce_cf_st_ok )
	  goto error_exit;
    }

    /*
     * Do not read from the local cds cache. Instead fetch the membership
     * data directly from the CDS server. This is because, the /.:/fs group
     * could be changed on a different machine, and this change wouldn't
     * show up in our cache for a while. If an administrator restarts this
     * server, we still wouldn't recognize the new peer. We would ignore any
     * errors from this call though.
     */
    rpc_ns_mgmt_handle_set_exp_age(inquiryContext, (unsigned32)0, st);
    
    while ( *st == rpc_s_ok ) {
      rpc_ns_group_mbr_inq_next(inquiryContext, &memberNameP, st);
      if ( *st == rpc_s_no_more_members )
	break;
      else if (*st != rpc_s_ok) {
	goto error_exit;
      }

      /* This check only for server initialization */
      if (serverInit && 
	  strncmp((char *)cellName, (char *)memberNameP, strlen((char *)cellName)) == 0 &&
	  strcmp((char *)ourName, (char *)memberNameP+strlen((char *)cellName)+1) == 0) {
	foundUs = 1;
	rpc_string_free(&memberNameP, &unusedSt);
	continue;
      }

      *(*membersPP + *nGroupP) = memberNameP;
      
      (*nGroupP)++;

      if (*nGroupP >= UBIK_MAXSERVERS) { /* too many members */
	*st = UNHOSTS;
	goto error_exit;
      }
    } /* while */

    if (serverInit && !foundUs) { /* we are not in the group */
      *st = UNOTMEMBER;
      goto error_exit;
    }
      
    if (*nGroupP <= 0) { /* just ourselves */
      (void)free(*membersPP);
      *membersPP = (unsigned_char_t **)NULL;
    } else {
      /* Prune the members array to required size */
      *membersPP = (unsigned_char_t **)realloc(*membersPP, *nGroupP * \
						 sizeof(unsigned_char_t *));
      if (*membersPP == (unsigned_char_t **)NULL) { /* shouldn't happen */
	*st = UNOMEM;
	goto exit;
      }
    } /* else */

exit:
    /* dce_cf_XXX routines allocated these, and they used malloc() to do it */
    if (ourName) free(ourName);
    if (cellName) free(cellName);
    /* Make sure the corresponding begin is called before */
    rpc_ns_group_mbr_inq_done(&inquiryContext, &unusedSt);
    if ( *st == rpc_s_ok || *st == rpc_s_no_more_members )
      *st = unusedSt;
    
    return;

error_exit:
    for(i=0;i<*nGroupP;i++) {
	memberNameP = *(*membersPP + i);
	rpc_string_free(&memberNameP, &unusedSt);
    }
    free(*membersPP);
    *membersPP = (unsigned_char_t **)NULL;
    *nGroupP = 0;
    goto exit;
}

/* ubik_free_bindings
 *
 * free the bindings for a group of (ubik) servers allocated by
 * ubik_get_bindings
 */

SHARED void ubik_free_bindings(binding_vector, st)
    IN  ubik_binding_vector_t **binding_vector;
    OUT unsigned32 *st;
{
    int i,j;

    if (*binding_vector) {
	if ((*binding_vector)->s) {
	    for (i = 0 ; i < (*binding_vector)->count ; i++) {
		for (j = 0 ; j < (*binding_vector)->s[i].count ; j++) {
		    rpc_binding_free(&(*binding_vector)->s[i].binding_h[j], st);
	        }
	    }
	    free ((*binding_vector)->s);
	}
	free(*binding_vector);
    }
    *binding_vector = NULL;
    return;
}

/* ubik_get_bindings
 *	get the bindings for a group of (ubik) servers from groupNameP
 * entry:
 *	groupNameP - CDS group for lookup. Entry itself contains obj. uuid.
 *	ifspec - application ifspec.  Current usage requires us to
 *		try both the application ifspec and the endpoint mapper's
 *		spec.  This is because existing CDS ubik groups point
 *		at hosts/<blah>/self rather than at the ubik server's
 *		binding.
 *	membersP - array of member names
 * exit:
 *	binding_vector - returned bindings
 *	st - return status
 */

PRIVATE void ubik_get_bindings(groupNameP, ifspec, membersP, groupSize,
			       binding_vector, st)
     IN  unsigned_char_t *groupNameP;
     IN  rpc_if_handle_t ifspec;
     IN  unsigned_char_t **membersP;
     IN  unsigned int groupSize;
     OUT ubik_binding_vector_t **binding_vector;
     OUT unsigned32 *st; 	                   /* returned status */
{
    rpc_ns_handle_t import_context;
    uuid_t object_uuid;
    rpc_binding_handle_t *bindingP;
    unsigned32 i, j;
    error_status_t unusedSt;

    *binding_vector = (ubik_binding_vector_t *)NULL;

    /* First, we have to determine the unique uuid that we must use to 
     * import the bindings.
     */
    compat_get_group_object_uuid(groupNameP, &object_uuid, st);
    if ( *st ) {
	goto error_exit;
    }

    *binding_vector = (ubik_binding_vector_t *)
		      malloc(sizeof(ubik_binding_vector_t));
    if (*binding_vector == (ubik_binding_vector_t *)NULL) {
      *st = UNOMEM;
      goto error_exit;
    }
    bzero((char *)(*binding_vector), sizeof(ubik_binding_vector_t));
    (*binding_vector)->count = groupSize;

    /* Allocate required server structures to hold server bindings */
    (*binding_vector)->s = (struct ubik_bv_server_t *)
			   malloc(5 + groupSize *
				  sizeof(struct ubik_bv_server_t));
    if ((*binding_vector)->s == (struct ubik_bv_server_t *)NULL) {
      *st = UNOMEM;
      goto error_exit;
    }
    bzero((char *)((*binding_vector)->s),
	  groupSize * sizeof(struct ubik_bv_server_t));

    for (i=0;i<groupSize;i++) {

      /* now lookup the bindings, searching for rpcd bindings */
      rpc_ns_binding_import_begin(DFS_ENTRY_SYNTAX, membersP[i],
				ifspec, NULL,
				&import_context, st);
      if (*st != rpc_s_ok) {
	goto error_exit;
      }
      (*binding_vector)->s[i].count = 0;
      while (*st != rpc_s_no_more_bindings) {
	bindingP = (rpc_binding_handle_t *)
		   &(*binding_vector)->
		       s[i].binding_h[(*binding_vector)->s[i].count];

	rpc_ns_binding_import_next(import_context, bindingP, st);
	if (*st == rpc_s_no_more_bindings &&
	    (*binding_vector)->s[i].count == 0 &&
	    ifspec != NULL &&
	    ifspec != ept_v3_0_c_ifspec) {

	    /* If we found no bindings using the supplied ifspec, we */
	    /* try for bindings pointing to the endpoint mapper (ie: */
	    /* the "self" binding).  This is for compatibility with */
	    /* current ubik usage.  Hopefully this old usage will go */
	    /* away sometime in the future. */
	    rpc_ns_binding_import_done(&import_context, &unusedSt);
	    ifspec = ept_v3_0_c_ifspec;
	    rpc_ns_binding_import_begin(DFS_ENTRY_SYNTAX, membersP[i],
					ept_v3_0_c_ifspec, NULL,
					&import_context, st);
	    if (*st != rpc_s_ok) {
		goto error_exit;
	    }
	    continue;
	}

	if (*st != rpc_s_ok && *st != rpc_s_no_more_bindings) {
	  rpc_ns_binding_import_done(&import_context, &unusedSt);
	  goto error_exit;
	}
	if (*st == rpc_s_ok) {
	  if ((*binding_vector)->s[i].count >= UBIK_MAXADDRS) {
	    rpc_ns_binding_import_done(&import_context, &unusedSt);
	    *st = UNBINDINGS;
	    goto error_exit;
	  } else {
	    rpc_binding_set_object(*bindingP, &object_uuid, st);
	    if (*st != rpc_s_ok) {
	      rpc_ns_binding_import_done(&import_context, &unusedSt);
	      goto error_exit;
	    }
	    ((*binding_vector)->s[i].count)++;
	  }
	}
      }/* while */

      rpc_ns_binding_import_done(&import_context, st);
      if (*st != rpc_s_ok) {
	goto error_exit;
      }
    } /*for*/

    /* remove protocols not supported by DFS */
    /* We pass ubik_bv_server_t and fake it as a binding_vector_t */
    for(i=0;i<(*binding_vector)->count;i++) {
      compat_filter_binding_vector((rpc_binding_vector_t *)
				   &((*binding_vector)->s[i]), st);
      if (*st != error_status_ok) {
	goto error_exit;
      }
    } /* for */

    return;
    
error_exit:
    ubik_free_bindings(binding_vector, &unusedSt);
    return;
}

PRIVATE int bindingArraySorter (handle1, handle2)
     IN const void *handle1, *handle2;
{
  long code;
  int i;
  struct sockaddr_in sock1;
  long ipAddr1, ipAddr2, myIpAddr;
  long minXorDiff1, minXorDiff2, xorDiff;
  rpc_binding_handle_t hdl1, hdl2;
  /* 
   * XOR each of the handle parameters with each of the handles in the
   * ubik_host structure. Whichever one produces the least XOR value is
   * selected as the lesser of the two values. This way, we have the
   * handle that produced the least XOR value at the top of the array
   * Before comparisons, we first have to convert the binding handles into
   * IP addresses
   */
  hdl1 = *((rpc_binding_handle_t *) handle1);
  hdl2 = *((rpc_binding_handle_t *) handle2);
  code = rpcx_binding_to_sockaddr (hdl1, &sock1);
  if (code) return 0;
  code = rpcx_ipaddr_from_sockaddr (&sock1, (char *)&ipAddr1);
  if (code) return 0;
  code = rpcx_binding_to_sockaddr (hdl2, &sock1);
  if (code) return 0;
  code = rpcx_ipaddr_from_sockaddr (&sock1, (char *)&ipAddr2);
  if (code) return 0;

  minXorDiff1 = minXorDiff2 = 0x7fffffff; /* some large value */
  for (i=0;i<ubik_host.bindingCount;i++) {
    code = rpcx_binding_to_sockaddr (ubik_host.diskCid[i], &sock1);
    if (code) return 0;
    code = rpcx_ipaddr_from_sockaddr (&sock1, (char *)&myIpAddr);
    if (code) return 0;

    if (minXorDiff1 > (xorDiff=(ipAddr1 ^ myIpAddr)))
      minXorDiff1 = xorDiff;
    if (minXorDiff2 > (xorDiff=(ipAddr2 ^ myIpAddr)))
      minXorDiff2 = xorDiff;
  }
  return (minXorDiff1 - minXorDiff2);
}


PRIVATE void ubik_SortHostAddresses(bindingVectorPP, stP)
     IN ubik_binding_vector_t **bindingVectorPP;
     OUT unsigned32 *stP;
{
  int i;

  /* 
   * By the time this function is called, the ubik_host structure is already
   * filled. This structure will be used to compare host addresses with each
   * of the binding arrays in the binding vector, and the individual arrays 
   * are sorted such that the first entry in the array is numerically closest
   * to the addresses in the ubik_host structure.
   */

  for(i=0;i<(*bindingVectorPP)->count;i++)
    qsort((char *)&((*bindingVectorPP)->s[i].binding_h[0]), 
	  (*bindingVectorPP)->s[i].count, 
	  sizeof(rpc_binding_handle_t), bindingArraySorter);
}

/*
 * ubik_scramble_member_vector -- randomly modify order of servers in a vector
 *
 * The idea is that, when multiple servers are available, we should try to
 * balance the load among them.  We pick one at random and put it in the first
 * place in the list.  Most of the time this is sufficient because the first
 * one is used.
 */

PRIVATE void ubik_scramble_member_vector(membersPP, size, stP)
     IN unsigned_char_t **membersPP;
     IN unsigned int size;
     OUT unsigned32 *stP;
{
  long pos;
  unsigned_char_t *tempMember;		/* for swapping */

  *stP=0;

  /* Don't bother if there is only one. */
  if (size < 2) return;

  /* Random seed should include both time-of-day and PID */
  srand(time(0) + getpid());

  /* Pick a random position to move to first position */
  pos = rand() % size;
  if (pos < 0 || pos > size-1) {
    *stP = EINVAL;
    return;
  } else if (pos == 0) {
    *stP=0;
    return;
  }

  /* Do the swap */
  tempMember = membersPP[pos];
  membersPP[pos] = membersPP[0];
  membersPP[0] = tempMember;

  *stP = 0;
  return;
}

SHARED void ubik_shuffleBindings (ts, offset)
     IN struct ubik_server *ts;
     IN int offset;
{
  int i, locPtr;
  rpc_binding_handle_t tempHandle;
  
  if (!ts || offset < 1 || offset >= ts->bindingCount)
    return; /* don't bother reporting error */

  /* This routine modifies the order of bindings in ts as a side-effect */
  for(i=0;i<(ts->bindingCount/2)/*int. division*/;i++) {
    locPtr = (offset+i)%(ts->bindingCount);

    /* swap the disk handles */
    tempHandle = ts->diskCid[locPtr];
    ts->diskCid[locPtr] = ts->diskCid[i];
    ts->diskCid[i] = tempHandle;

    /* swap the vote handles */
    tempHandle = ts->voteCid[locPtr];
    ts->voteCid[locPtr] = ts->voteCid[i];
    ts->voteCid[i] = tempHandle;
  }
    

}

/* ubik_GetServerList
 *	get bindings to the collection of ubik servers. The bindings
 *	looked up are those of rpcd. The group, its members and bindings
 *	must be setup manually. If serverInit is not set, all bindings for all
 *	ubik servers exporting this interface are returned, else, bindings
 *	for this server is omitted.
 * exit:
 *	bindingVector - an array of bindings for each server in group
 *	stP - error status
 */

SHARED void ubik_GetServerList(groupNameP, ifspec, serverInit, 
			       bindingVector, stP)
     IN unsigned_char_t *groupNameP;
     IN rpc_if_handle_t ifspec;
     IN char serverInit;
     OUT ubik_binding_vector_t **bindingVector;
     OUT unsigned32 *stP;
{
    unsigned int groupSize, i;
    unsigned_char_t **membersP = (unsigned_char_t **)NULL;
    unsigned32 unusedSt;

    if (bindingVector == (ubik_binding_vector_t **)NULL) {
      *stP = UINTERNAL;
      goto exit;
    }
    *bindingVector = (ubik_binding_vector_t *)NULL;

    /* There is an implicit check here for groups with member count more than
       UBIK_MAXSERVERS. */
    ubik_GroupEnumerate(groupNameP, serverInit, &membersP, &groupSize, stP);
    if (*stP != rpc_s_ok)
      goto exit;

    /* Randomize the order of the member names in the vector, so that all
     * clients don't bombard the same server with queries. CDS apparently
     * doesn't seem to do this. Well, it claims that the order is arbitrary
     * but it seems to be repeatable every time!!
     */
    ubik_scramble_member_vector (membersP, groupSize, stP);
    if (*stP != rpc_s_ok) {
      goto exit;
    }

    ubik_get_bindings(groupNameP, ifspec, membersP, groupSize,
		      bindingVector, stP);
    if (*stP != rpc_s_ok) {
      goto exit;
    }

    /* The host addresses for a multi-homed host are sorted with respect to 
     * the ubik_host structure. This step is done to avoid unnecessary
     * routing through other sub-nets instead of the common one.
     */
    ubik_SortHostAddresses(bindingVector, stP);
    if (*stP != rpc_s_ok) {
      goto exit;
    }
    icl_Trace3(ubikdisk_iclSetp, UK_TRACE_GET_SERVER_LIST, ICL_TYPE_STRING,
	       groupNameP, ICL_TYPE_LONG, groupSize, ICL_TYPE_LONG, *stP);
exit:
    if (membersP) {
	/* membersP was allocated by malloc(), but the sub-pointers by rpc_string_xxx. */
	for(i=0;i<groupSize;i++)
	    rpc_string_free(&membersP[i], &unusedSt);
	free(membersP);
    }
    return;
}

SHARED void ubik_fill_hostStructure (st)
     error_status_t *st;
{
  unsigned_char_t *ourName=NULL;
  unsigned_char_t *cellName=NULL;
  unsigned_char_t *hostName=NULL;
  char *tmpFullName = NULL;
  char *tmpPrinc = NULL;
  char *tmpCell = NULL;
  unsigned int count=0;
  rpc_ns_handle_t import_context;
  error_status_t unusedSt;
  rpc_binding_handle_t binding;
  int i;
  struct sockaddr_in servAddr;
  long ipAddr;
  unsigned char *prefix;
  unsigned_char_t *protseq;
  sec_login_handle_t loginHndl;
  sec_cred_pa_handle_t paHndl;
  sec_id_pa_t *paData;

  sec_login_get_current_context(&loginHndl, st);
  if (*st == error_status_ok) {
    paHndl = sec_login_cred_get_initiator(loginHndl, st);
    if (*st == error_status_ok) {
      paData = sec_cred_get_pa_data(paHndl, st);
      if (*st == error_status_ok) {
	ubik_host.realm = paData->realm.uuid;
	ubik_host.principal = paData->principal.uuid;
        dce_cf_get_cell_name((char **)&cellName, (unsigned long *)st);
        if (*st == dce_cf_st_ok) {
          dce_cf_binding_entry_from_host(NULL, (char **)&ourName, st);
          if ( *st == dce_cf_st_ok ) {
            hostName = (unsigned_char_t *)
	               malloc(strlen((char *)cellName)+1+strlen((char *)ourName)+1);
            if (hostName != (unsigned_char_t *)NULL) {
              strcpy((char *)hostName, (char *)cellName);
              strcat((char *)hostName,"/");
              strcat((char *)hostName, (char *)ourName);
      
              rpc_ns_binding_import_begin(DFS_ENTRY_SYNTAX, hostName,
                                          NULL, NULL,
                                          &import_context, st);
              if (*st == rpc_s_ok) {
                while (*st != rpc_s_no_more_bindings) {
                  rpc_ns_binding_import_next(import_context, &binding, st);
                  if (*st == rpc_s_ok) {
                    if (count >= UBIK_MAXADDRS) {
                      *st = UNBINDINGS;
                      break;
                    }
                    /* Eliminate bindings we don't support */
                    rpc_binding_to_string_binding(binding, &prefix, st);
                    if (*st != rpc_s_ok)
                      break;
                    rpc_string_binding_parse(prefix, NULL, &protseq, NULL,
                                             NULL, NULL, st);
                    rpc_string_free(&prefix, &unusedSt);
                    if (*st != rpc_s_ok)
                      break;
                    if (strcmp("ncadg_ip_udp", (char *)protseq) != 0) {
                      rpc_string_free(&protseq, &unusedSt);
                      continue;
                    }
                    rpc_string_free(&protseq, &unusedSt);
                    rpc_binding_copy(binding, &(ubik_host.voteCid[count]), st);
                    if (*st == rpc_s_ok) {
                      rpc_binding_copy(binding, &(ubik_host.diskCid[count]), st);
                      if (*st == rpc_s_ok) {
                        count++;
                        ubik_host.bindingCount++;
                      }
                    }
                    rpc_binding_free(&binding, &unusedSt);
		      if (*st != rpc_s_ok)
		      break;
                  } else break;
                } /* while */
        
                if (*st == rpc_s_no_more_bindings)
                  *st = rpc_s_ok;
                if (*st != rpc_s_ok) {
                  for(i=0;i<count;i++) {
                    rpc_binding_free(&(ubik_host.voteCid[i]), &unusedSt);
		    ubik_host.voteCid[i] = NULL;
                    rpc_binding_free(&(ubik_host.diskCid[i]), &unusedSt);
		    ubik_host.diskCid[i] = NULL;
                  }
                  bzero((char *)&ubik_host, sizeof(struct ubik_server));
                } else {
                  for(i=0;i<count;i++) {
                    if(*st = rpcx_binding_to_sockaddr(ubik_host.voteCid[i],
                                                      &servAddr)) {
                      if (*st == -1)
                        *st = UINTERNAL;
                      break;
                    } else {
                      if (*st = rpcx_ipaddr_from_sockaddr(&servAddr,
                                                          (char *)&ipAddr)) {
                        if (*st == -1)
                          *st = UINTERNAL;
                        break;
                      } else {
                        if (ntohl(ipAddr) < ntohl(ubik_host.addr) ||
                            ubik_host.addr == 0)
                          ubik_host.addr = ipAddr; /* network order */
                      }
                    }
                  } /* for */
                }
                rpc_ns_binding_import_done(&import_context, &unusedSt);
              } /* *st == rpc_s_ok */
              /* dce_cf_XXX allocated these with malloc() */
              if (cellName)
                free(cellName);
              if (ourName)
                free(ourName);
              if (hostName)
                free(hostName);
            } else *st = UNOMEM;
          } /* *st == dce_cf_st_ok */
        } /* *st == dce_cf_st_ok */
      } /* *st == error_status_ok */
      sec_cred_free_pa_handle(paHndl, &unusedSt);
    } /* *st == error_status_ok */
  } /* *st == error_status_ok */
}

/*
 * ubik_ResetState
 *	in case of major problems with the ubik server(s), drastic action
 * such as resetting all ubik state is recommended. This includes restarting
 * the vote gathering and recovery actions, and initializing any state that
 * these actions have setup.
 */
void ubik_ResetState()
{
  ubeacon_ResetState();
  uvote_ResetState();
  urecovery_ResetState(1);	/* prod recovery thread */
}


/*
 * Check to see if we know this host
 */
SHARED long ubik_CheckBadHost(rxcall)
handle_t rxcall;
{
    long code;
    struct ubik_server *ts;
    uuid_t clientPrinc;
    uuid_t clientRealm;
    unsigned32 unusedSt;

    code = dfsauth_server_GetClientIdentity(rxcall, &clientRealm, &clientPrinc);
    if (code) {
	return code;
    }

    for( ts = ubik_servers; ts ; ts = ts->next) {
       if (uuid_equal(&(ts->principal), &clientPrinc, &unusedSt) &&
           uuid_equal(&(ts->realm), &clientRealm, &unusedSt)) {
	   return (0);
       }
    }
    return(UNOTMEMBER);
}

/* ---------------
 * back to normal ubik routines
 * ---------------
 */

/*  perform an operation on a quorum of database servers, handling error
    conditions.  Return 0 if majority worked, otherwise mark failing server(s)
    down and return UERROR

    There are synchronous and asynchronous operations on quorums . The
    asynchronous routines allow the caller to overlap local disk I/O with
    RPC calls to other servers. However, the caller must be careful to
    make sure that the data structures used by the threads are not
    altered, either by the caller of by the recovery routines, while
    an RPC in process. To allow the recovery routines to access the
    server structures while the RPC is running, the information from
    the server structure is copied into the threads parameter buffer.
    The address of the server structure is used to match up the results
    with the server when the RPC completes.

    Note that if any server misses an update, we must wait BIGTIME seconds
    before allowing the transaction to commit, to ensure that the missing and
    possibly still functioning server times out and stop handing out old data.
    This is done in the commit code, where we wait for a server marked down to
    have stayed down for BIGTIME seconds before we allow a transaction to
    commit.  A server that fails but comes back up won't give out old data
    because it is sent the sync count along with the beacon message that marks
    it as *really* up (beaconSinceDown).

    The caller should hold both the high level and the low level database
    locks to make this call. The low level lock is released while waiting
    for the RPC calls to complete.

    The apver and acompat paramaters are used to recover from
    RPC calls to downlevel servers. If a server does not support the
    procedure call, we can still interoperate by calling the compability
    routine to perform the function using the "old" interface. The apver
    parameter indicates the provider version that first contained the function.
    If the call fails with the error "rpc_s_rng_error," then the server
    is at least one level below the specified provider version. The
    compability routine must use calls from provider version 1 to ensure
    interoperability.
 */

/*
 * structure used to pass parameters to thread in Contact Quorum
 */

typedef struct ubik_disk_parm_t
{
    struct ubik_server *ts;		/* address of server structure */
    struct in_addr inAddr;		/* network address of server */
    long bindingCount;			/* count of bindings in array */
    rpc_binding_handle_t diskCid[UBIK_MAXADDRS];	
    int (* aproc)();			/* address of RPC routine to call */
    struct ubik_trans *atrans;		/* address of transaction */
    int aflags;				/* server flags */
    int apver;				/* provider vers needed for call */
    int (*acompat)();			/* compability routine */
    long aparm0;			/* Function arguments */
    long aparm1;
    long aparm2;
    long aparm3;
    long aparm4;
    long aparm5;
    long code;				/* function return code */
    long index;				/* index of binding used for call */
    int srvr_pver;			/* server provider vers */
} ubik_disk_parm_t;

/*
 * variables used to communicate with threads that perform the RPC
 * calls for ContactQuorum
 */

ubik_disk_parm_t ubik_RpcParms[UBIK_MAXSERVERS];
					/* args to quorum RPC calls */
void *ubik_disk_rpc_parset_handle = (void *)NULL;
					/* concurrent disk RPC handle */
int ubik_nDiskRpc = 0;			/* count of active disk RPC threads */
long ubik_RpcCode;			/* errors while starting RPC call */

/*
 * Routine that does the disk RPC calls to other servers. RPC paramaters
 * and return codes are passed in the parameter structure.
 */

static void ContactServerRpcCall(arg)
    void *arg;
{
    char *excNameP, excString[20];
    extern char *ExceptionToString();
    struct ubik_server * volatile ts;
    volatile int mayRetry;
    volatile int index;
    ubik_disk_parm_t * volatile parm;

    parm = (ubik_disk_parm_t *)arg;

    /*
     * try each binding for this server
     */
    
    mayRetry = 1;
    for ( index = 0 ; mayRetry && index < parm->bindingCount ; index++ ) {
      TRY {
        UBIK_RPC_PREWRAP
#ifdef DEBUG
        compat_print_binding("DiskRpcCall to : ",parm->diskCid[index]);
#endif /* DEBUG */
        if ( parm->srvr_pver >= parm->apver ) {
	    icl_Trace3(ubikdisk_iclSetp, UK_TRACE_DISK_RPC_CALL,
	           ICL_TYPE_STRING, inet_ntoa(parm->inAddr),
	           ICL_TYPE_POINTER, parm->aproc, ICL_TYPE_POINTER,
	           parm->atrans);
    
	    parm->code = (*parm->aproc)
		           (parm->diskCid[index],&parm->atrans->tid,
		            parm->aparm0,parm->aparm1,parm->aparm2,
		            parm->aparm3,parm->aparm4,parm->aparm5);
    
	    /*
	     * check the return to see if we contacted a server running
	     * below the requested provider version number. The RPC
	     * returns a range error if the server isn't at the desired
	     * provider version level.
	     */
    
	    if ( parm->apver > 1 && parm->code == rpc_s_op_rng_error ) {
	        parm->srvr_pver = parm->apver - 1;
	    }
        }
    
        /*
         * if the server isn't at the desired provider version, then
         * use the compability routine to make the call.
         */
    
        if ( parm->srvr_pver < parm->apver ) {
	    /*
	     * call the compability routine, this server
	     * isn't at the desired provider version level.
	     */
    
	    if ( parm->acompat != NULL ) {
	       icl_Trace3(ubikdisk_iclSetp, UK_TRACE_DISK_RPC_CALL,
		      ICL_TYPE_STRING, inet_ntoa(parm->inAddr),
		      ICL_TYPE_POINTER, parm->acompat,
		      ICL_TYPE_POINTER, parm->atrans);
	       parm->code = (*parm->acompat)
		               (parm->diskCid[index],&parm->atrans->tid,
			        &mayRetry,parm->aparm0,parm->aparm1,
			        parm->aparm2,parm->aparm3,parm->aparm4,
			        parm->aparm5);
	    } else parm->code = rpc_s_op_rng_error;
        }
        UBIK_RPC_POSTWRAP(parm->code)
      } CATCH_ALL {
        excNameP = ExceptionToString(THIS_CATCH);
        if (excNameP == NULL) {
          sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
          excNameP = excString;
        }
        icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
	   ICL_TYPE_STRING,
	   excNameP, ICL_TYPE_STRING, __FILE__, 
	   ICL_TYPE_LONG,  __LINE__);
        parm->code = UEXCEPTION;
      } ENDTRY

      /*
       * if the call succeeded, then record the index of the binding we used
       */ 

      if ( parm->code == 0 ) parm->index = index;
    
      /*
       * do not retry if the call succeeded or if the error
       * was other than an RPC error (ubik error)
       */
    
      if ((parm->code < rpc_s_mod) || (parm->code > (rpc_s_mod+4096)))
	 mayRetry = 0;
    }
    
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_DISK_RPC_CALL_RESULT,
	   ICL_TYPE_POINTER, parm->atrans,
	   ICL_TYPE_LONG, parm->code);
    
}

/*
 * start an asynchronous ContactQuorum call
 */

PRIVATE int AsyncContactQuorum(aproc, atrans, aflags, apver, acompat,
			       aparm0, aparm1, aparm2, 
			       aparm3, aparm4, aparm5)
    int (* aproc)();
    int apver;			/* provider version required */
    int (* acompat)();		/* compatability routine*/
    int aflags;
    struct ubik_trans * atrans;
    long aparm0, aparm1, aparm2, aparm3, aparm4, aparm5;
{
    struct ubik_server * ts;
    int i;
    long code;
    long result;
    int WaitContactQuorum();

    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_ASYNC_CONTACT_QUORUM,
	   ICL_TYPE_POINTER, aproc, ICL_TYPE_POINTER, atrans,
	   ICL_TYPE_LONG, apver, ICL_TYPE_POINTER, acompat);


    /*
     * if this transaction has an RPC already running, wait for it
     * to complete before starting the next call
     */

    if ( atrans->RpcRunning ) {
	code = WaitContactQuorum(atrans,NULL);
	if ( code ) {
            icl_Trace2(ubikdisk_iclSetp, UK_TRACE_ASYNC_CONTACT_QUORUM_EXIT,
	               ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, code);
	    return(code);
	}
    }

    /*
     * this should never happen.
     */

    if ( ubik_disk_rpc_parset_handle != (void *)NULL ) {
	return(UDUPASYNC);
    }

    /*
     * send the request to all servers that are up. There is a permanent
     * thread for each server we need to send RPCs to. If we have more
     * servers than threads, start new threads as needed.
     */

    code = 0;
    ubik_nDiskRpc = 0;
    for( ts = ubik_servers ; ts != NULL ; ts = ts->next ) {
	if (!ts->up || !ts->currentDB) {
	    /*
	     * db is no longer current; we just missed an update.
	     */
	    ts->currentDB = 0;
	    continue;
	}

	/*
	 * copy the RPC parameters to the thread structure
	 */

	ubik_RpcParms[ubik_nDiskRpc].aproc = aproc;
	ubik_RpcParms[ubik_nDiskRpc].atrans = atrans;
	ubik_RpcParms[ubik_nDiskRpc].aflags = aflags;
	ubik_RpcParms[ubik_nDiskRpc].apver = apver;
	ubik_RpcParms[ubik_nDiskRpc].acompat = acompat;
	ubik_RpcParms[ubik_nDiskRpc].aparm0 = aparm0;
	ubik_RpcParms[ubik_nDiskRpc].aparm1 = aparm1;
	ubik_RpcParms[ubik_nDiskRpc].aparm2 = aparm2;
	ubik_RpcParms[ubik_nDiskRpc].aparm3 = aparm3;
	ubik_RpcParms[ubik_nDiskRpc].aparm4 = aparm4;
	ubik_RpcParms[ubik_nDiskRpc].aparm5 = aparm5;
	ubik_RpcParms[ubik_nDiskRpc].ts = ts;
	ubik_RpcParms[ubik_nDiskRpc].inAddr.s_addr = ts->addr;
	ubik_RpcParms[ubik_nDiskRpc].srvr_pver = ts->ProviderVers;
	ubik_RpcParms[ubik_nDiskRpc].bindingCount = ts->bindingCount;
	for ( i = 0 ; i < ts->bindingCount ; i++ )
	    ubik_RpcParms[ubik_nDiskRpc].diskCid[i] = ts->diskCid[i];
	ubik_RpcParms[ubik_nDiskRpc].code = 0;

	/*
	 * create a parallel thread set for the RPC call
	 */

	if ( !ubik_disk_rpc_parset_handle ) {
            ubik_disk_rpc_parset_handle = tpq_CreateParSet(ubik_tpq_handle);
            if ( ubik_disk_rpc_parset_handle == NULL ) {
                icl_Trace2(ubikdisk_iclSetp, UK_TRACE_ASYNC_CONTACT_QUORUM_EXIT,
	                   ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, UTPQFAIL);
	        return(UTPQFAIL);
            }
	}

	/*
	 * start a thread to make the RPC call to this server
	 */

	result = tpq_AddParSet(ubik_disk_rpc_parset_handle,
			       ContactServerRpcCall,
			       (void *)&(ubik_RpcParms[ubik_nDiskRpc]),
			       TPQ_HIGHPRI,
			       UBIK_DISK_RPC_GRACEPERIOD);
	if ( !result ) {
            icl_Trace2(ubikdisk_iclSetp, UK_TRACE_ASYNC_CONTACT_QUORUM_EXIT,
	               ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, UTPQFAIL);
	    code = UTPQFAIL;
	} else ubik_nDiskRpc++;
    }

    /*
     * if we successfully started at least one RPC call then return
     * success. Otherwise, clean up and return an error. Save the
     * error code from the tpc_AddParSet calls and return it from 
     * AsyncWait if we cannot contact a quorum.
     *
     * Set the RpcFlag here so we need to notify the other servers
     * if the transaction aborts. If the transaction aborts before
     * any RPC calls have been made, then the remote servers will
     * return USYNC from the abort request.
     */

    if ( ubik_nDiskRpc > 0 ) {
        atrans->RpcRunning = 1;
        atrans->RpcFlag = 1;
	ubik_RpcCode = code;
	code = 0;
    } else {
	if ( ubik_nServers <= 1 ) code = 0;
	else if ( ubik_nServers == 2 && ubik_amIMagic ) code = 0;
	else if ( !code ) code = UNOQUORUM;
	if ( ubik_disk_rpc_parset_handle ) {
	    ubik_rele();
            tpq_WaitParSet(ubik_disk_rpc_parset_handle);
	    ubik_hold();
	    ubik_disk_rpc_parset_handle = NULL;
	}
    }

    /*
     * return the error code
     */

    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_ASYNC_CONTACT_QUORUM_EXIT,
	       ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, code);
    return(code);
}

/*
 * wait for an asynchronous contact quorum call to complete
 */

PRIVATE int WaitContactQuorum(atrans,aokP)
    struct ubik_trans * atrans;
    int *aokP;
{
    struct ubik_server * ts;
    long code;
    int okcount;
    int i;

    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_WAIT_CONTACT_QUORUM,
	       ICL_TYPE_POINTER, atrans);

    /*
     * if there is nothing running, return success.
     */

    if ( !atrans->RpcRunning ) {
        icl_Trace2(ubikdisk_iclSetp, UK_TRACE_WAIT_CONTACT_QUORUM_EXIT,
	           ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, 0);
	if ( aokP != NULL ) *aokP = 0;
	return(0);
    }

    /*
     * this should never happen
     */

    if ( ubik_disk_rpc_parset_handle == (void *)NULL ) {
        icl_Trace2(ubikdisk_iclSetp, UK_TRACE_WAIT_CONTACT_QUORUM_EXIT,
	           ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, UNOASYNC);
	return(UNOASYNC);
    }

    /*
     * release the low level lock and wait for all threads to complete
     * their RPC calls.
     */

    ubik_rele();
    tpq_WaitParSet(ubik_disk_rpc_parset_handle);
    ubik_hold();

    /*
     * Process the results of each RPC call.
     */

    okcount = 0;
    code = ubik_RpcCode;
    for ( i=0 ; i < ubik_nDiskRpc ; i++ ) {
	/*
	 * a lot can happen during one of these RPC calls. The
	 * server may not be up anymore or the server's data
	 * structure may have been freed. Only process results
	 * from servers that are still up.
	 */

	ts = ubik_servers;
	while ( ts != NULL && ts != ubik_RpcParms[i].ts ) ts = ts->next;
	if ( ts != NULL && ts->up && ts->currentDB ) {
	    if ( ubik_RpcParms[i].code != 0 ) {
	        /*
	         * RPC failed, mark server down and notify recovery
	         * to try to resend the database later.
	         */
    
	        if ( code == 0 ) code = ubik_RpcParms[i].code;
	        ts->up = 0;
	        ts->currentDB = 0;
	        ts->beaconSinceDown = 0;
	        urecovery_LostServer();
	    } else {
		/*
		 * success: count how many worked and update the
		 * server structure
		 */
	        okcount += 2;
		if ( ts->magic ) okcount++; /* magic breaks tie */

	        if (ubik_RpcParms[i].aflags & CStampVersion) {
		    ts->version = atrans->dbase->version;
	        }

		if ( ubik_RpcParms[i].srvr_pver < ts->ProviderVers ) {
		    ts->ProviderVers = ubik_RpcParms[i].srvr_pver;
		}
    
		/*
		 * if the first binding didn't work then shuffle them
		 * so we try the one that worked first
		 */
    
		if ( ubik_RpcParms[i].index > 0 ) {
		    ubik_shuffleBindings(ts, ubik_RpcParms[i].index);
		}
	    }
	}
    }

    ubik_disk_rpc_parset_handle = NULL;
    ubik_nDiskRpc = 0;
    atrans->RpcRunning = 0;

    /*
     * return 0 if we successfully contacted a quorum, otherwise return
     * the error code. 
     */

    if ( aokP != NULL ) *aokP = okcount; /* remote votes for success */
    okcount += 2; /* count ourselves */
    if ( ubik_amIMagic ) okcount++; /* magic breaks tie */
    if (okcount > ubik_nServers) {
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_WAIT_CONTACT_QUORUM_EXIT,
		 ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, 0);
      return 0;
    } else {
      if ( code == 0 ) code = UNOQUORUM;
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_WAIT_CONTACT_QUORUM_EXIT,
		 ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, code);
      return code;
    }
}

PRIVATE int ContactQuorum(aproc, atrans, aflags,
			  apver, acompat,
			  aparm0, aparm1, aparm2, 
			  aparm3, aparm4, aparm5)
    int (* aproc)();
    int apver;			/* provider version required */
    int (* acompat)();		/* compatability routine*/
    int aflags;
    struct ubik_trans * atrans;
    long aparm0, aparm1, aparm2, aparm3, aparm4, aparm5;
{
    long code;

    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_PVER_CONTACT_QUORUM,
	   ICL_TYPE_POINTER, aproc, ICL_TYPE_POINTER, atrans,
	   ICL_TYPE_LONG, apver, ICL_TYPE_POINTER, acompat);

    /*
     * start the RPC call
     */

    code = AsyncContactQuorum(aproc, atrans, aflags, apver, acompat,
			      aparm0, aparm1, aparm2,
			      aparm3, aparm4, aparm5);
    if ( code ) {
        icl_Trace2(ubikdisk_iclSetp, UK_TRACE_CONTACT_QUORUM_EXIT,
                   ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, code);
        return(code);
    }

    /*
     * wait for the RPC call to complete
     */

    code = WaitContactQuorum(atrans,NULL);
    if ( code ) {
        icl_Trace2(ubikdisk_iclSetp, UK_TRACE_CONTACT_QUORUM_EXIT,
	           ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, code);
        return(code);
    }

    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_CONTACT_QUORUM_EXIT,
	       ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, 0);
    return(0);
}

/* register_ubik_dfs_server
 *	Register a server with the end point mapper. The interface uuid
 *	is passed in. The server will use an object uuid provided.
 *	This object uuid is shared by all the servers in a cell, but will be
 *	different for each cell.
 * entry:
 *  	if_handle
 *	mgr_epv
 *	object_uuidP
 *	type_uuidP
 *	maxcalls
 */
EXPORT int register_ubik_dfs_server(if_handle, mgr_epv, object_uuidP, 
				    type_uuidP, annotation, st)
     IN  rpc_if_handle_t if_handle;
     IN  rpc_mgr_epv_t mgr_epv;
     IN  uuid_t *object_uuidP;
     IN  uuid_t	*type_uuidP;
     IN  char *annotation;
     OUT unsigned32 *st;			/* returned status */
{
    uuid_vector_t *object_uuid_vectorP = 0;
    rpc_binding_vector_t *binding_vectorP = 0;
    error_status_t unusedSt;
    int nbindings;

    /* get our own bindings */
    rpc_server_inq_bindings(&binding_vectorP, st);
    CHK_RPC_ERR(*st);

    /* register the interface with the rpc runtime */
    rpc_server_register_if(if_handle, type_uuidP, mgr_epv, st);
    if (*st != rpc_s_ok) {
      rpc_binding_vector_free(&binding_vectorP, &unusedSt);
      goto exit;
    }

    /* construct the object uuid vector. In this instance, it only
     * has one element
     */
    object_uuid_vectorP = (uuid_vector_t *) malloc(sizeof(uuid_vector_t));
    if ( object_uuid_vectorP == 0 ) {
	*st = rpc_s_no_memory;
	rpc_binding_vector_free(&binding_vectorP, &unusedSt);
	goto exit;
    }

    /* set the object uuid in the vector */
    object_uuid_vectorP->count = 1;
    object_uuid_vectorP->uuid[0] = object_uuidP;

    /* now we can register the bindings. First with the end point mapper
     * i.e. rpcd
     */
    rpc_ep_register(if_handle, binding_vectorP, object_uuid_vectorP, 
		    (unsigned_char_p_t)annotation, st);
    if (*st != rpc_s_ok) {
      free(object_uuid_vectorP);
      rpc_binding_vector_free(&binding_vectorP, &unusedSt);
      goto exit;
    }
#ifdef DEBUG
    compat_print_binding_vector(binding_vectorP);
#endif /* DEBUG */

exit:
    return(0);
}

/*
 * ubik_thread_pool_info: structure to hold the information about the reserved
 * ubik thread pool. Incoming calls to the two ubik interfaces would use this
 * thread pool, and if no threads are available would use the default thread
 * pool. The application interface would, by default, use the default thread
 * pool.
 */
PRIVATE struct ubik_thread_pool_info {
     rpc_thread_pool_handle_t thrPoolHandle;
     void (*thrLookupProc)();
     rpc_if_id_t ubikdiskIfId, ubikvoteIfId;
     uuid_p_t objUuidP, typeUuidP;
} ubik_thread_pool_info;

/*
 * ubik_register_thread_pool_info: A routine to fill the fields of the above
 * structure. The thread pool lookup routine registered with the runtime would
 * lookup the values in this structure to decide which thread pool to use.
 */
PRIVATE int ubik_register_thread_pool_info (thrPoolHandle, thrLookupProc,
					     ubikdiskIfId, ubikvoteIfId,
					     objUuidP, typeUuidP)
     rpc_thread_pool_handle_t thrPoolHandle;
     void (*thrLookupProc)();
     rpc_if_id_t ubikdiskIfId, ubikvoteIfId;
     uuid_p_t objUuidP, typeUuidP;
{
  ubik_thread_pool_info.thrPoolHandle = thrPoolHandle;
  ubik_thread_pool_info.thrLookupProc = thrLookupProc;
  ubik_thread_pool_info.ubikdiskIfId  = ubikdiskIfId;
  ubik_thread_pool_info.ubikvoteIfId  = ubikvoteIfId;
  ubik_thread_pool_info.objUuidP      = objUuidP;
  ubik_thread_pool_info.typeUuidP     = typeUuidP;
  return (0);
}

/*
 * ubik_thrPoolLookup: The thread pool lookup routine registered with the
 * runtime decides which thread pool to use based on comparison of the incoming
 * paramters and the thread pool info structure fields.
 */
PRIVATE void ubik_thrPoolLookup(obj_uuidP, if_idP, opNum, thrPoolHandleP, stP)
     uuid_p_t obj_uuidP;
     rpc_if_id_p_t if_idP;
     unsigned32 opNum;
     rpc_thread_pool_handle_t *thrPoolHandleP;
     unsigned32 *stP;
{
  *stP = 0;
  if((bcmp((char *)if_idP,
	   (char *)&(ubik_thread_pool_info.ubikdiskIfId),
	   sizeof(*if_idP))==0
      || bcmp((char *)if_idP,
	      (char *)&(ubik_thread_pool_info.ubikvoteIfId),
	      sizeof(*if_idP))==0
      ) &&
     uuid_equal(obj_uuidP, ubik_thread_pool_info.objUuidP, stP)) {
    /* The ubik vote and disk interfaces, use our thread pool */
/*  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_POOL_USE_UBIK); */
    *thrPoolHandleP = ubik_thread_pool_info.thrPoolHandle;
    *stP = 0;
    return;
  }
/*icl_Trace0(ubikdisk_iclSetp, UK_TRACE_POOL_USE_DEFAULT); */
  /* Unknown interface, use default thread pool */
  bzero((char *)thrPoolHandleP, sizeof(rpc_thread_pool_handle_t));
  return;
}

SHARED int setupICL()
{
    struct icl_log *logp;
    long voteCode, diskCode;
    static int iclInit = 0;

    if (iclInit)
	return 0;
    iclInit = 1;

    /*
     * The vote and disk interfaces are represented by different event sets.
     * Its probably a good idea to use two logs so that we can track problems
     * in disk and vote mechanisms without the clutter.
     */
    voteCode = icl_CreateLog("ubikvote", 30*1024, &logp);
    if (!voteCode)
      voteCode = icl_CreateSet("ubikvote", logp, (struct icl_log *)0, 
			   &ubikvote_iclSetp);

    diskCode = icl_CreateLog("common", 30*1024, &logp);
    if (!diskCode)
      diskCode = icl_CreateSet("ubikdisk", logp, (struct icl_log *)0, 
			   &ubikdisk_iclSetp);
    if (voteCode)
      return (voteCode);
    else
      return (diskCode);
}

/* ubik_ServerInit
 * 	This routine initializes the ubik system for a set of servers.
 *	It returns 0 for success, or an error code on failure.  The set of
 *	servers is specified by serverList; nServers gives the number of
 *	entries in this array.  Finally, dbase is the returned structure
 *	representing this instance of a ubik; it is passed to various calls
 *	below.  The variable pathName provides an initial prefix used for
 *	naming storage files used by this system.  It should perhaps be
 *	generalized to a low-level disk interface providing read, write, file
 *	enumeration and sync operations.
 * prerequisites:
 *	routine dfsauth_server_InitAuthentication should have been called prior
 *	to calling this routine, in order to set up local auth context.
 */

EXPORT int ubik_ServerInit(myHost, ifspec, mgrEpv, nsGroupP, nsEntryP, 
			   secGroupP, typeUuidP, object_uuidP, maxCalls, 
			   pathName, annotation, dbase)
     IN struct sockaddr_in *myHost;
     IN rpc_if_handle_t ifspec;
     IN rpc_mgr_epv_t mgrEpv;
     IN unsigned_char_t *nsGroupP;
     IN unsigned_char_t *nsEntryP;
     IN unsigned_char_t *secGroupP;
     IN uuid_t	*typeUuidP;
     OUT uuid_t	*object_uuidP;
     IN unsigned32	maxCalls;
     IN char *pathName;
     IN char *annotation;
     OUT struct ubik_dbase **dbase;
{	
    struct ubik_dbase *tdb;
    long code = 0;
    int i,j;
    long myInetHost;
    error_status_t st, unusedSt;
    unsigned32 rpc_status;
    /* long secIndex; */
    long serverInetList[UBIK_MAXSERVERS];
    struct sockaddr_in serverList[UBIK_MAXSERVERS];
    ubik_binding_vector_t *bindingVectorP = NULL;
    long serverCount;
    unsigned_char_t *ubikAnnotation;
    rpc_thread_pool_handle_t thrPoolHandle;
    rpc_if_id_t ubikvoteIfId, ubikdiskIfId;
    pthread_t beaconPid, recoveryPid;
    pthread_attr_t createAttr;

    /* Initialize ICL ubik tracing, ignore return code */
    code = setupICL();
    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SERVER_INIT, ICL_TYPE_POINTER,
	       myHost, ICL_TYPE_STRING, nsGroupP, ICL_TYPE_STRING, secGroupP,
	       ICL_TYPE_STRING, pathName);

    /* validate parameters */
    if (myHost == (struct sockaddr_in *)NULL)
      return UBADHOST;
    if (nsGroupP == (unsigned_char_t *)NULL)
      return UBADGROUP;
    if (secGroupP == (unsigned_char_t *)NULL)
      return UBADSECGRP;
    if (typeUuidP == (uuid_t *)NULL)
      return UBADUUID;

    ubik_servers = (struct ubik_server *)NULL;
    bzero((char *)&ubik_host, sizeof(struct ubik_server));
    tdb = (struct ubik_dbase *) malloc(sizeof(struct ubik_dbase));
    if (!tdb) return UNOMEM;
    tdb->pathName = (char *) malloc(strlen(pathName)+1);
    if (!tdb->pathName) return UNOMEM;
    strcpy(tdb->pathName, pathName);
    tdb->activeTrans = (struct ubik_trans *) 0;
    bzero((char *)&tdb->version, sizeof(struct ubik_version));
    bzero((char *)&tdb->cachedVersion, sizeof(struct ubik_version));

    /* Initialize the ubik internal locks */
    ubik_initLocks(tdb);
    ubik_hold();	/* cma concurrency lock */
    tdb->flags = 0;
    tdb->read = uphys_read;
    tdb->write = uphys_write;
    tdb->truncate = uphys_truncate;
    tdb->open =	0;  /* this function isn't used any more */
    tdb->sync = uphys_sync;
    tdb->stat = uphys_stat;
    tdb->getlabel = uphys_getlabel;
    tdb->setlabel = uphys_setlabel;
    tdb->getnfiles = uphys_getnfiles;
    tdb->readers=0;
    tdb->tidCounter=tdb->writeTidCounter=0;
    *dbase = tdb;
    /* for now, only one db per server; can fix later when we have names for
     * the other dbases */
    ubik_dbase = tdb;

    /* restrict protocol variety to UDP for now */
    rpc_server_use_protseq((unsigned char *)"ncadg_ip_udp", maxCalls, &st);
    if (IS_COMM_ERR(st)) {
	code = st;
	goto exit;
    }

    /* get the unique object uuid for this instance of a ubik database */
    /* A bug here noticed by Dawn, fixed 7/21/92 */
    compat_get_group_object_uuid(nsGroupP, object_uuidP, &st);
    if (IS_COMM_ERR(st)) {
      code = st;
      goto exit;
    }

    /* register the object uuid and it's type with the rpc runtime */
    rpc_object_set_type(object_uuidP, typeUuidP, &st);
    if(IS_COMM_ERR(st)) {
      code = st;
      goto exit;
    }

#ifdef use_shutdown_duplicates
    code = compat_ShutdownDuplicateServer (ifspec, object_uuidP, 0);
    if ( code)
	goto exit;
#else
    code = dfsauth_client_InitAuthentication(NULL, (char *)NULL, 0, 1);
    if (code)
      goto exit;
#endif /* use_shutdown_duplicates */
    /* We should really not wait for our identity to expire to refresh
     * since ubik has some serious time constraints which
     * might be affected by this
     */
    code = dfsauth_client_AutoRefreshLocalAuthContext();
 

    register_ubik_dfs_server(ifspec, mgrEpv, object_uuidP, typeUuidP, 
			     annotation, &st);
    if ( st != rpc_s_ok ) {
	code = st;
	goto exit;
    }

    dfs_installInterfaceDescription (UBIKDISK_v4_0_s_ifspec, 
				     UBIKDISK_v4_0_s_ifspec,
				     UBIK_PROVIDER_VERSION,
				     (unsigned_char_t *)"Transarc ubik Disk Interface",
				     &st);
    if (st) {
	ubik_dprint("ubik: Can't install interface description (%s), \
Exiting.\n", dfs_dceErrTxt(st));
	code = st;
	goto exit;
    }

#ifdef use_shutdown_duplicates
    code = compat_ShutdownDuplicateServer((rpc_if_handle_t)UBIKDISK_v4_0_s_ifspec, 
					  object_uuidP, 0);
    if ( code)
	goto exit;
#endif /* use_shutdown_duplicates */

    /* Ubik interface registration with CDS. It is likely that this latter
     * pair of registrations is unnecessary, but harmless.
     */
    ubikAnnotation = (unsigned_char_t *)malloc(strlen(annotation) + \
				    strlen(" Ubik Disk Interface")+1);
    if (!ubikAnnotation) {
      code = UNOMEM;
      goto exit;
    }

    strcpy((char *)ubikAnnotation, annotation); 
    strcat((char *)ubikAnnotation, " Ubik Disk Interface");

    register_ubik_dfs_server((rpc_if_handle_t)UBIKDISK_v4_0_s_ifspec,
			     (rpc_mgr_epv_t)&UBIKDISK_v4_0_epv,
			     (uuid_t *)object_uuidP, 
			     (uuid_t *)typeUuidP,
			     (char *)ubikAnnotation,
			     (unsigned32 *)&st);
    if ( st != rpc_s_ok ) {
	code = st;
	goto exit;
    }

    dfs_installInterfaceDescription (UBIKVOTE_v4_0_s_ifspec, 
				     UBIKVOTE_v4_0_s_ifspec,
				     1, /* provider_version number */
				     (unsigned_char_t *)"Transarc Ubik Vote Interface",
				     &st);
    if (st) {
	ubik_dprint("ubik: Can't install interface description (%s), \
Exiting.\n", dfs_dceErrTxt(st));
	code = st;
	goto exit;
    }
#ifdef use_shutdown_duplicates
    code = compat_ShutdownDuplicateServer((rpc_if_handle_t)UBIKVOTE_v4_0_s_ifspec, 
					  object_uuidP, 1);
    if ( code)
	goto exit;
#endif /* use_shutdown_duplicates */

    strcpy((char *)ubikAnnotation, annotation); 
    strcat((char *)ubikAnnotation, " Ubik Vote Interface");

    register_ubik_dfs_server((rpc_if_handle_t)UBIKVOTE_v4_0_s_ifspec,
			     (rpc_mgr_epv_t)&UBIKVOTE_v4_0_epv,
			     (uuid_t *)object_uuidP, 
			     (uuid_t *)typeUuidP,
			     (char *)ubikAnnotation,
			     (unsigned32 *)&st);
    if ( st != rpc_s_ok ) {
	code = st;
	goto exit;
    }

    (void)free (ubikAnnotation);

    /*
     * Ubik thread pools - We dedicate certain number of threads for ubik
     * to use. This would make sure ubik beacons and disk rpcs have threads
     * to use on each server without sharing with the application server's
     * API. Beacons are critical to the continued stability of a ubik server
     * and is important that they go through without getting bogged down
     * by heavy app server RPC traffic.
     */
    rpc_server_set_thread_pool_fn((rpc_thread_pool_fn_t)ubik_thrPoolLookup,
				  &st);
    if (st != rpc_s_ok) {
      code = st;
      goto exit;
    }

    rpc_server_create_thread_pool(UBIK_THREAD_POOL_SIZE,
				  &thrPoolHandle,
				  &st);
    if (st != rpc_s_ok) {
      code = st;
      goto exit;
    }
    rpc_if_inq_id((rpc_if_handle_t)UBIKDISK_v4_0_s_ifspec,
		  &ubikdiskIfId, &st);
    if (st != rpc_s_ok) {
      code = st;
      goto exit;
    }
    rpc_if_inq_id((rpc_if_handle_t)UBIKVOTE_v4_0_s_ifspec,
		  &ubikvoteIfId, &st);
    if (st != rpc_s_ok) {
      code = st;
      goto exit;
    }
    code = ubik_register_thread_pool_info (thrPoolHandle, ubik_thrPoolLookup,
					   ubikdiskIfId, ubikvoteIfId,
					   object_uuidP, typeUuidP);
    if (code) {
      goto exit;
    }


    /* do basic initialization */
    code = uvote_Init(); 
    if (code) {
      goto exit;
    }
    code = urecovery_Initialize(tdb);
    if (code) {
      goto exit;
    }

    /* fill in the ubik_host structure */
    ubik_fill_hostStructure (&st);
    if (st != rpc_s_ok) {
      code = st;
      goto exit;
    }

    ubik_GetServerList(nsGroupP, ifspec, (char)1, &bindingVectorP, &st);
    if ( st != rpc_s_ok ) {
	code = st;
	goto exit;
    }

    code = rpcx_ipaddr_from_sockaddr(myHost, (char *)&myInetHost);
    if (code != 0) {
      code = UINTERNAL;
      goto exit;
    }
    /*
     * Create thread pool queue for performing concurrent RPCs to other
     * ubik servers. Concurrent beacons are essential to maintaining the
     * strict time limitations votes provide, and to maintain quorum. RPC
     * delays and timeouts could make a sync site lose quorum repeatedly.
     * We use the TPQ package for this purpose. The minimum number of threads
     * required would be two for each of the ubik servers that we send RPCs
     * to (1 for beacons/votes, and 1 for disk RPC calls).
     *
     * set the maximums large, tpq will cancel threads that are idle
     * longer then UBIK_THREADENNUI seconds.
     */
    ubik_tpq_handle = tpq_Init(2*(bindingVectorP->count+1),
			       4*UBIK_MAXSERVERS,
			       4*UBIK_MAXSERVERS,
			       UBIK_THREADENNUI,
			       "ubik");
    if (ubik_tpq_handle == (void *)NULL) {
      code = UTPQFAIL;
      goto exit;
    }

    /* Using dynamic endpoints - no port required  */
    ubeacon_InitServerList(myInetHost, bindingVectorP, secGroupP, &st);
    if ( st != rpc_s_ok ) {
	code = st;
	ubik_free_bindings(&bindingVectorP, &unusedSt);
	goto exit;
    }

    /*
     * store relevant objects
     */
    serverNsGroupP = (unsigned_char_t *)osi_Alloc((size_t)strlen((char *)nsGroupP)+1);
    if (serverNsGroupP == (unsigned_char_t *)NULL) {
      code = UNOMEM;
      goto exit;
    }
    strcpy((char *)serverNsGroupP, (char *)nsGroupP);
    serverSecGroupP =(unsigned_char_t *)osi_Alloc((size_t)strlen((char *)secGroupP)+1);
    if (serverSecGroupP == (unsigned_char_t *)NULL) {
      code = UNOMEM;
      goto exit;
    }
    strcpy((char *)serverSecGroupP, (char *)secGroupP);
    serverHostAddr = *myHost;

    ubik_free_bindings(&bindingVectorP, &unusedSt);

    /* now start up the permanent threads */
    code = pthread_attr_create(&createAttr);
    if (code != 0) {
      code = errno;
      goto exit;
    }
    code = pthread_attr_setstacksize(&createAttr, UBIK_STACK_SIZE);
    if (code != 0) {
      code = errno;
      goto exit;
    }
    /* now start up async processes */
    code = pthread_create(&beaconPid, createAttr, 
			  (pthread_startroutine_t)ubeacon_Interact, 
			  (char *) 0);
    if (code) {
      code = errno;
      goto exit;
    }
    code = pthread_create(&recoveryPid, createAttr, 
			  (pthread_startroutine_t)urecovery_Interact, 
			  (char *) 0);
    if (code) {
      code = errno;
      pthread_cancel(beaconPid); /* reclaim resources */
      pthread_detach(&beaconPid);
      goto exit;
    }

exit:
    if (code && (ubik_tpq_handle != (void *)NULL))
      tpq_ShutdownPool(ubik_tpq_handle);
    ubik_rele();
    icl_Trace1(ubikdisk_iclSetp,UK_TRACE_SERVER_INIT_EXIT,ICL_TYPE_LONG, code);
    return (code);
}

/* This routine begins a read or write transaction on the transaction
 * identified by transPtr, in the dbase named by dbase.  An open mode of
 * ubik_READTRANS identifies this as a read transaction, while a mode of
 * ubik_WRITETRANS identifies this as a write transaction.  Finally, transPtr
 * is set to the returned transaction control block.

    We can only begin transaction when we have an up-to-date database.
 */

EXPORT int ubik_BeginTrans(dbase, transMode, transPtr)
     IN struct ubik_dbase *dbase;	/* in */
     IN long transMode; /* in */
     OUT struct ubik_trans ** volatile transPtr;	/* out */ {
    long code;
    char *excNameP, excString[20];
    extern char *ExceptionToString();

    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_BEGIN_TRANS, ICL_TYPE_LONG, 
	       transMode);
    ubik_hold();
    TRY {
      code = BeginTrans(dbase, transMode, (struct ubik_trans **)transPtr, 0);
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		 ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, 
		 ICL_TYPE_LONG,  __LINE__);
      code = UEXCEPTION;
    } ENDTRY
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_BEGIN_TRANS_EXIT, ICL_TYPE_POINTER, 
	       *transPtr, ICL_TYPE_LONG, code);
    return code;
}

EXPORT int ubik_BeginTransReadAny(dbase, transMode, transPtr)
     IN  struct ubik_dbase *dbase;	/* in */
     IN  long transMode; /* in */
     OUT struct ubik_trans **transPtr;	/* out */ {
    volatile long code;
    char *excNameP, excString[20];
    extern char *ExceptionToString();

    ubik_hold();
    TRY {
      code =  BeginTrans(dbase, transMode, transPtr, 1);
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		 ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, 
		 ICL_TYPE_LONG,  __LINE__);
      code = UEXCEPTION;
    } ENDTRY
    ubik_rele();
    return code;
}

PRIVATE int BeginTrans(dbase, transMode, transPtr, readAny)
    IN  struct ubik_dbase *dbase;	/* in */
    IN  int readAny;
    IN  long transMode; /* in */
    OUT struct ubik_trans **transPtr;	/* out */ {
    struct ubik_trans *tt;
    long code;
    extern int ubik_BulkUpdateCompatRtn();
    ubik_bulk_update *tmp_bulk;

    if ((transMode != UBIK_READTRANS) && readAny) return UBADTYPE;
    ubik_versionLock(dbase);
    if (urecovery_AllBetter(dbase, readAny)==0) {
	ubik_versionUnlock(dbase);
	return UNOQUORUM;
    }
    /* otherwise we have a quorum, use it */
    
    /* make sure that at most one write transaction occurs at any one time.
     * This has nothing to do with transaction locking; that's enforced by the
     * lock package.  However, we can't even handle two non-conflicting writes,
     * since our log and recovery modules don't know how to restore one without
     * possibly picking up some data from the other. */
    if (transMode == UBIK_WRITETRANS) {
	/* if we're writing already, wait */
	while(dbase->flags & DBWRITING) {
	  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_SLEEP_WAIT_FOR_WRITER);
	    ubik_sleep(dbase, (opaque) &dbase->flags);
	  icl_Trace0(ubikdisk_iclSetp, UK_TRACE_READY_TO_WRITE);
	    ubik_versionLock(dbase);
	}
	if (!ubeacon_AmSyncSite()) {
	    ubik_versionUnlock(dbase);
	    return UNOTSYNC;
	}
    }
    
    /* create the transaction */
    code = udisk_begin(dbase, transMode, &tt);
    if (code || tt == (struct ubik_trans *)NULL) {
	ubik_versionUnlock(dbase);
	return code;
    }
    if (readAny) tt->flags |= TRREADANY;
    /* label trans and dbase with new tid */
    tt->tid.epoch = ubik_epochTime;
    /* bump by two, since tidCounter+1 means trans id'd by tidCounter has
     * finished */
    tt->tid.counter = (dbase->tidCounter += 2);
    /* for a write trans, we have to keep track of the write tid counter too */
    if (transMode == UBIK_WRITETRANS)
      dbase->writeTidCounter += 2; 

    icl_Trace1(ubikvote_iclSetp, UK_TRACE_TIDCOUNTER_BUMP,
	       ICL_TYPE_LONG, tt->tid.counter);

    if (transMode == UBIK_WRITETRANS) {
	/* 
	 * Add the request to the bulk update I/O vector
	 */
	tt->bulk[0].op = UBULK_BEGIN;
	tt->bulk[0].arg1 = 0;
	tt->bulk[0].arg2 = 0;
	tt->bulk[0].arg3 = 0;
	tt->bulk[0].arg4 = 0;
	tt->bulk[0].length = 0;
        tt->nbulk = 1;
	if ( tt->nbulk == ubik_maxbulk ) {
	    /*
	     * swap the bulk and rpc_bulk and start the Async RPC Call
	     */
	    tmp_bulk = tt->bulk;
	    tt->bulk = tt->rpc_bulk;
	    tt->rpc_bulk = tmp_bulk;
	    tt->rpc_nbulk = tt->nbulk;
	    tt->nbulk = 0;
	    code = AsyncContactQuorum(UBIKDISK_BulkUpdate, tt, 0, 2,
				      ubik_BulkUpdateCompatRtn,
				      tt->rpc_nbulk,tt->rpc_bulk);
	    if (code) {
		/* we must abort the operation */
	        udisk_abort(tt);
		/* force aborts to others */
	        ContactQuorum(UBIKDISK_Abort, tt, 0, 1, NULL);
	        udisk_end(tt);
	        ubik_versionUnlock(dbase);
	        return code;
	    }
	}
    }

    *transPtr = tt;
    ubik_versionUnlock(dbase);
    return 0;
}

/* this routine ends a read or write transaction by aborting it */
EXPORT int ubik_AbortTrans(transPtr)
    IN  struct ubik_trans * volatile transPtr; /* in */ {
    volatile long code;
    volatile long code2;
    struct ubik_dbase * volatile dbase;
    char *excNameP, excString[20];
    extern char *ExceptionToString();

    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_ABORT_TRANS, ICL_TYPE_POINTER, 
	      transPtr);
    dbase = transPtr->dbase;
    ubik_versionLockBoth(dbase);
    bzero((char *)&dbase->cachedVersion, sizeof(struct ubik_version));
    /* see if we're still up-to-date */
    if (!urecovery_AllBetter(dbase, transPtr->flags & TRREADANY)) {
      code=0;
      TRY {
	udisk_abort(transPtr);
	udisk_end(transPtr);
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	code = UEXCEPTION;
      } ENDTRY
      ubik_versionUnlock(dbase);
      ubik_rele();
      icl_Trace2(ubikdisk_iclSetp,UK_TRACE_ABORT_TRANS_EXIT,ICL_TYPE_POINTER,
		 transPtr, ICL_TYPE_LONG, (code?code:UNOQUORUM));
      return (code?code:UNOQUORUM);
    }

    if (transPtr->type == UBIK_READTRANS) {
      TRY {
	code = udisk_abort(transPtr);
	udisk_end(transPtr);
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	code = UEXCEPTION;
      } ENDTRY
      ubik_versionUnlock(dbase);
      ubik_rele();
      icl_Trace2(ubikdisk_iclSetp,UK_TRACE_ABORT_TRANS_EXIT,ICL_TYPE_POINTER,
		 transPtr, ICL_TYPE_LONG, code);
      return code;
    }

    /* below here, we know we're doing a write transaction */
    if (!ubeacon_AmSyncSite()) {
      code=0;
      TRY {
	udisk_abort(transPtr);
	udisk_end(transPtr);
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	code = UEXCEPTION;
      } ENDTRY
      ubik_versionUnlock(dbase);
      ubik_rele();
      icl_Trace2(ubikdisk_iclSetp,UK_TRACE_ABORT_TRANS_EXIT,ICL_TYPE_POINTER,
		 transPtr, ICL_TYPE_LONG, (code?code:UNOTSYNC));
      return (code?code:UNOTSYNC);
    }
    
    /*
     * if we have made any RPC calls for this transaction, then
     * abort the transaction on the remote servers.
     */

    if ( transPtr->RpcFlag )
        code = ContactQuorum(UBIKDISK_Abort, transPtr, 0, 1, NULL);
    else code = 0;
    TRY {
      code2 = udisk_abort(transPtr);
      udisk_end(transPtr);
    } CATCH_ALL {
      excNameP = ExceptionToString(THIS_CATCH);
      if (excNameP == NULL) {
	sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	excNameP = excString;
      }
      icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		 ICL_TYPE_STRING,
		 excNameP, ICL_TYPE_STRING, __FILE__, 
		 ICL_TYPE_LONG,  __LINE__);
      code2 = UEXCEPTION;
    } ENDTRY
    ubik_versionUnlock(dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_ABORT_TRANS_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, (code? code: code2));
    return (code? code : code2);
}

/* This routine ends a read or write transaction on the open transaction
 * identified by transPtr.  It returns an error code. */
EXPORT int ubik_EndTrans(transPtr)
    IN  struct ubik_trans * volatile transPtr; /* in */ {
    volatile long code;
    volatile long lcl_code;
    volatile long rpc_code;
    volatile int okcount;
    struct timespec tv;
    long realStart;
    struct ubik_server *ts;
    long now;
    struct ubik_dbase * volatile dbase;
    char *excNameP, excString[20];
    extern char *ExceptionToString();
    
    icl_Trace1(ubikdisk_iclSetp,UK_TRACE_END_TRANS,ICL_TYPE_POINTER, transPtr);

    /*
     * if this is a write transaction, we may have some writes queued
     * that have not been sent to the other servers, so flush them now.
     * The flush routine will also wait for any async RPC calls to complete.
     */

    if ( transPtr->type == UBIK_WRITETRANS ) {
	code = ubik_Flush(transPtr);
	if ( code != 0 ) {
	    ubik_AbortTrans(transPtr);
            icl_Trace2(ubikdisk_iclSetp, UK_TRACE_END_TRANS_EXIT,
		       ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG, code);
	    return(code);
	}
    }

    dbase = transPtr->dbase;
    ubik_versionLockBoth(dbase);
    bzero((char *)&dbase->cachedVersion, sizeof(struct ubik_version));

    /* give up if no longer current */
    if (!urecovery_AllBetter(dbase, transPtr->flags & TRREADANY)) {
      code=0;
      TRY {
	udisk_abort(transPtr);
	udisk_end(transPtr);
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	code = UEXCEPTION;
      } ENDTRY
      ubik_versionUnlock(dbase);
      ubik_rele();
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_END_TRANS_EXIT, 
		 ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG, 
		 (code?code:UNOQUORUM));
      return (code?code:UNOQUORUM);
    }

    if (transPtr->type == UBIK_READTRANS) { /* reads are easy */
      TRY {
	code = udisk_commit(transPtr);
	udisk_end(transPtr);
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	code = UEXCEPTION;
      } ENDTRY
      ubik_versionUnlock(dbase);
      ubik_rele();
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_END_TRANS_EXIT, 
		 ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG, code);
      return code;
    }

    if (!ubeacon_AmSyncSite()) {    /* no longer sync site */
      code = 0;
      TRY {
	udisk_abort(transPtr);
	udisk_end(transPtr);
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	code = UEXCEPTION;
      } ENDTRY
      ubik_versionUnlock(dbase);
      ubik_rele();
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_END_TRANS_EXIT, 
		 ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG, 
		 (code?code:UNOTSYNC));
      return (code?code:UNOTSYNC);
    }
    
    /*
     * now it is safe to commit. Perform the local and remote
     * commits in parallel.
     */

    rpc_code = AsyncContactQuorum(UBIKDISK_Commit, transPtr,
				  CStampVersion, 1, NULL);
    if (rpc_code != 0) {
      /* problem with the RPC threads, abort */
      code = 0;
      TRY {
	udisk_abort(transPtr);
	udisk_end(transPtr);
      } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	code = UEXCEPTION;
      } ENDTRY
      ubik_versionUnlock(dbase);
      ubik_rele();
      icl_Trace2(ubikdisk_iclSetp, UK_TRACE_END_TRANS_EXIT, 
		 ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG,
		 (code?code:rpc_code));
      return (code?code:rpc_code);
    }

    /*
     * Do the local commit.
     */

    TRY {
      lcl_code = udisk_commit(transPtr);
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	lcl_code = UEXCEPTION;
    } ENDTRY

    /*
     * get the return code from the remote commit, okcount gets the
     * number of remote votes for successful commit. If a majority
     * votes for success, then the commit succeeds whether we
     * succeed on this server or not.
     */

    rpc_code = WaitContactQuorum(transPtr,&okcount);

    /*
     * There are three possibilities for the outcome of the commit:
     *
     * If the commit succeeds on all servers, then we just release our
     * write locks to allow the servers to begin giving out the new data.
     * The transaction is a success.
     *
     * If the commit fails on this server, But we sent at least one
     * RPC call, then we probabaly do not have the latest version of
     * the database.  We call ubeacon_ResetState to begin
     * the voting for a new sync site. If any server in the new
     * quorum has successfully committed, then the transaction will
     * succeed. If a quorum of servers report success, then the
     * transaction is gauranteed to make it into the new quorum,
     * and we can return success.
     *
     * If the commit succeeds locally, but fails to commit on one or more
     * servers, then our copy of the database is valid, but some
     * other server may have a down level copy of the database.  We can
     * return success as long as the commit succeeds on a quorum of servers.
     *
     * Before we can start sending unlock messages, we must wait until all
     * servers that are possibly still functioning on the other side of a
     * network partition have timed out.  Check the server structures, compute
     * how long to wait, then start the unlocks. The servers should timeout
     * after BIGTIME seconds, so we wait 2*BIGTIME seconds to allow for
     * a safety margin.
     */

    realStart = time(0);
    while (1) {
	/* wait for all servers to time out */
	code = 0;
	now = time(0);
	/* check if we're still sync site, the guy should either come up
	    to us, or timeout.  Put safety check in anyway */
	if (now - realStart > BIGTIME) {
	    ubik_stats.escapes++;
	    printf("ubik escaping from commit wait\n");
	    break;
	}
	for(ts = ubik_servers; ts; ts=ts->next) {
	    if (!ts->beaconSinceDown && now <= ts->lastBeaconSent+2*BIGTIME) {
		/* this guy could be out of date, wait for him */
		code = 1;
		tv.tv_sec = 1;	/* try again after a while */
		tv.tv_nsec = 0;
		ubik_rele();
		pthread_delay_np(&tv);
		ubik_hold();
		break;
	    }
	}
	if (code == 0) break; /* no down servers still psuedo-active */
    }

    /*
     * Unlock the other servers. We can return success independent of the
     * number of servers that really unlock the dbase; the others will do it
     * if/when they elect a new sync site. The transaction is committed anyway,
     * provided we succeeded in contacting a quorum at the start (when
     * invoking the DiskCommit function). If we are no longer the sync site,
     * then we should let the other servers break the locks on their own.
     */

    ContactQuorum(UBIKDISK_ReleaseLocks, transPtr, 0, 1, NULL);

    /*
     * End the local transaction
     */

    TRY {
      udisk_end(transPtr);
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	if ( !lcl_code ) lcl_code = UEXCEPTION;
    } ENDTRY

    /*
     * Update the local version on successful commit,
     */

    if ( !lcl_code ) {
	dbase->cachedVersion = dbase->version;
	okcount += 2; /* count ourselves too */
	if ( ubik_amIMagic ) okcount++; /* magic breaks tie */
    }
    else ubeacon_ResetState();

    /*
     * Return success if the commit worked on a quorum of servers.
     */

    if ( okcount > ubik_nServers ) code = 0;
    else if ( lcl_code ) code = lcl_code;
    else code = rpc_code;
    ubik_versionUnlock(dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_END_TRANS_EXIT, 
	       ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG,code);
    return (int)(code);
}

/* This routine reads length bytes into buffer from the current position in the
 * database.  The file pointer is updated appropriately (by adding the number
 * of bytes actually transferred), and the length actually transferred is
 * stored in the long integer pointed to by length.  Note that *length is an
 * INOUT parameter: at the start it represents the size of the buffer, and when
 * done, it contains the number of bytes actually transferred.  A short read
 * returns zero for an error code. */

EXPORT int ubik_Read(transPtr, buffer, length)
    IN  struct ubik_trans * volatile transPtr;	/* in */
    IN  char *buffer;   /* in */
    IN  long length;   /* in */ {
    volatile long code;
    char *excNameP, excString[20];
    extern char *ExceptionToString();

    icl_Trace3(ubikdisk_iclSetp, UK_TRACE_READ, ICL_TYPE_POINTER, transPtr,
	       ICL_TYPE_POINTER, buffer, ICL_TYPE_LONG, length);
    /* reads are easy to do: handle locally */
    ubik_versionLockBoth(transPtr->dbase);
    if (!urecovery_AllBetter(transPtr->dbase, transPtr->flags & TRREADANY)) {
	ubik_versionUnlock(transPtr->dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_READ_EXIT, ICL_TYPE_POINTER,
		   transPtr, ICL_TYPE_LONG, UNOQUORUM);
	return UNOQUORUM;
    }
    TRY {
      code = udisk_read(transPtr, transPtr->seekFile, buffer, 
			transPtr->seekPos, length);
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	code = UEXCEPTION;
    } ENDTRY
    if (code == 0) {
	transPtr->seekPos += length;
    }
    ubik_versionUnlock(transPtr->dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_READ_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, code);
    return code;
}

/*
 * This routine is the compability routine for the UBIKDISK_BulkUpdate
 * procedure call. This routine iteratively calls UBIKDISK_Write to
 * send the updates to hosts that are still at provider version 1.
 */

EXPORT int ubik_BulkUpdateCompatRtn(cid,tid,retryP,nbulk,bulk)
    rpc_binding_handle_t cid;
    net_tid *tid;
    int *retryP;
    int nbulk;
    ubik_bulk_update *bulk;
{
    int code;
    int i;

    /*
     * loop calling UBIKDISK_xxxx for each entry in the I/O vector.
     * If at least one call succeeds clear the retry flag so
     * the caller won't retry with a different binding handle.
     * This is called from within a TRY block, so the caller will
     * catch any exceptions.
     */

    code = 0;
    for ( i = 0 ; code == 0 && i < nbulk ; i++ ) {
	switch(bulk[i].op) {
	  case UBULK_BEGIN:
	    code = UBIKDISK_Begin(cid, tid);
	    break;
	  case UBULK_LOCK:
	    code = UBIKDISK_Lock(cid, tid, bulk[i].arg1, bulk[i].arg2,
				 bulk[i].arg3, bulk[i].arg4);
	    break;
	  case UBULK_TRUNC:
	    code = UBIKDISK_Truncate(cid, tid, bulk[i].arg1, bulk[i].arg2);
	    break;
	  case UBULK_WRITE:
	    code = UBIKDISK_Write(cid,tid,bulk[i].arg1,bulk[i].arg2,
			          bulk[i].length,(bulkdata *)(bulk[i].data));
	    break;
	  default:
	    code = UBADBULK;
	}
	if ( code == 0 ) *retryP = 0;
    }
    return(code);
}

/* This routine writes length bytes into buffer at the current position in the
 * database.  The byte pointer is updated appropriately (by adding the number
 * of bytes actually written).  Unlike ubik_Read, a short write does return a
 * non-zero error code */

EXPORT int ubik_Write(transPtr, buffer, length)
    IN  struct ubik_trans * volatile transPtr;	/* in */
    IN  char *buffer;	/* in */
    IN  long length;	/* in */ {
    long startPos;
    long bytes;
    volatile long code;
    long xfrLength;
    char *excNameP, excString[20];
    extern char *ExceptionToString();
    extern int ubik_BulkUpdateCompatRtn();
    ubik_bulk_update *tmp_bulk;

    icl_Trace3(ubikdisk_iclSetp, UK_TRACE_WRITE, ICL_TYPE_POINTER, 
	       transPtr, ICL_TYPE_POINTER, buffer, ICL_TYPE_LONG, length);
    if (transPtr->type != UBIK_WRITETRANS) {
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_WRITE_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, UBADTYPE);
	return UBADTYPE;
    }
    ubik_versionLockBoth(transPtr->dbase);
    if (!urecovery_AllBetter(transPtr->dbase, transPtr->flags & TRREADANY)) {
	ubik_versionUnlock(transPtr->dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_WRITE_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, UNOQUORUM);
	return UNOQUORUM;
    }
    if (!ubeacon_AmSyncSite()) {    /* only sync site can write */
	ubik_versionUnlock(transPtr->dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_WRITE_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, UNOTSYNC);
	return UNOTSYNC;
    }

    startPos = transPtr->seekPos;   /* remember where we started */

    TRY {
      /* ok, we're the sync site, so we go for it */
      code = udisk_write(transPtr, transPtr->seekFile, buffer,
			 startPos, length);
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	code = UEXCEPTION;
    } ENDTRY
    if (code == 0) xfrLength = length;
    else {
	ubik_versionUnlock(transPtr->dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_WRITE_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, code);
	return code;
    }

    transPtr->seekPos += xfrLength;	/* update seek pointer */

    /* next try to write on other machines */
    bytes = xfrLength;
    while(bytes > 0) {
	if (bytes > UBULK_MAXLENGTH) xfrLength = UBULK_MAXLENGTH;
	else xfrLength = bytes;
	transPtr->bulk[transPtr->nbulk].op = UBULK_WRITE;
	transPtr->bulk[transPtr->nbulk].arg1 = transPtr->seekFile;
	transPtr->bulk[transPtr->nbulk].arg2 = startPos;
	transPtr->bulk[transPtr->nbulk].arg3 = 0;
	transPtr->bulk[transPtr->nbulk].arg4 = 0;
	transPtr->bulk[transPtr->nbulk].length = xfrLength;
	memcpy(transPtr->bulk[transPtr->nbulk].data,buffer,xfrLength);
	(transPtr->nbulk)++;
	if ( transPtr->nbulk == ubik_maxbulk ) {
	    /* if an async RPC is running then wait for it to complete */
	    code = WaitContactQuorum(transPtr,NULL);
	    if (code) {
	        udisk_abort(transPtr);
	        /* force aborts to the others */
	        ContactQuorum(UBIKDISK_Abort, transPtr, 0, 1, NULL);
	        ubik_versionUnlock(transPtr->dbase);
	        ubik_rele();
	        icl_Trace2(ubikdisk_iclSetp, UK_TRACE_WRITE_EXIT,
			   ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG, code);
	        return code;
	    }
	    /*
	     * swap the bulk and rpc_bulk and start the Async RPC Call
	     */
	    tmp_bulk = transPtr->bulk;
	    transPtr->bulk = transPtr->rpc_bulk;
	    transPtr->rpc_bulk = tmp_bulk;
	    transPtr->rpc_nbulk = transPtr->nbulk;
	    transPtr->nbulk = 0;
	    code = AsyncContactQuorum(UBIKDISK_BulkUpdate, transPtr, 0, 2,
				      ubik_BulkUpdateCompatRtn,
				      transPtr->rpc_nbulk,transPtr->rpc_bulk);
	    if (code) {
	        udisk_abort(transPtr);
	        /* force aborts to the others */
	        ContactQuorum(UBIKDISK_Abort, transPtr, 0, 1, NULL);
	        ubik_versionUnlock(transPtr->dbase);
	        ubik_rele();
	        icl_Trace2(ubikdisk_iclSetp, UK_TRACE_WRITE_EXIT,
			   ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG, code);
	        return code;
	    }
	}
	startPos += xfrLength;
	bytes -= xfrLength;
	buffer += xfrLength;
    }

    /* finally return the good code */
    ubik_versionUnlock(transPtr->dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_WRITE_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, 0);
    return 0;
}

/* This routine flushes any writes that have been queued locally by
 * sending them to the other servers. */

EXPORT int ubik_Flush(transPtr)
    IN  struct ubik_trans * volatile transPtr;	/* in */ {
    volatile long code;
    char *excNameP, excString[20];
    extern char *ExceptionToString();
    extern int ubik_BulkUpdateCompatRtn();

    icl_Trace1(ubikdisk_iclSetp, UK_TRACE_FLUSH, ICL_TYPE_POINTER, 
	       transPtr);
    if (transPtr->type != UBIK_WRITETRANS) {
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_FLUSH_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, UBADTYPE);
	return UBADTYPE;
    }
    ubik_versionLockBoth(transPtr->dbase);
    if (!urecovery_AllBetter(transPtr->dbase, transPtr->flags & TRREADANY)) {
	ubik_versionUnlock(transPtr->dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_FLUSH_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, UNOQUORUM);
	return UNOQUORUM;
    }
    if (!ubeacon_AmSyncSite()) {    /* only sync site can write */
	ubik_versionUnlock(transPtr->dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_FLUSH_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, UNOTSYNC);
	return UNOTSYNC;
    }

    if ( transPtr->nbulk != 0 ) {
        /*
         * Send any writes that have been queued locally to the
         * other database servers with a synchronous RPC call.
         */

        code = ContactQuorum(UBIKDISK_BulkUpdate, transPtr, 0, 2,
			     ubik_BulkUpdateCompatRtn,
			     transPtr->nbulk,transPtr->bulk);
        if (code) {
            udisk_abort(transPtr);
            /*
	     * force aborts to the others    
	     */    
            ContactQuorum(UBIKDISK_Abort, transPtr, 0, 1, NULL);
            ubik_versionUnlock(transPtr->dbase);
            ubik_rele();
            icl_Trace2(ubikdisk_iclSetp, UK_TRACE_FLUSH_EXIT,
		   ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG, code);
            return code;
        }
        transPtr->nbulk = 0;
    } else {
        /*
         * Wait for any asynchronous RPC call to complete
         */

        code = WaitContactQuorum(transPtr,NULL);
        if (code) {
            udisk_abort(transPtr);
            /*
             * force aborts to the others
             */    
            ContactQuorum(UBIKDISK_Abort, transPtr, 0, 1, NULL);
            ubik_versionUnlock(transPtr->dbase);
            ubik_rele();
            icl_Trace2(ubikdisk_iclSetp, UK_TRACE_FLUSH_EXIT,
                       ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG, code);
            return code;
        }
    }

    /* finally return the good code */
    ubik_versionUnlock(transPtr->dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_FLUSH_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, 0);
    return 0;
}

/* This sets the file pointer associated with the current transaction to the
 * appropriate file and byte position.  Unlike Unix files, a transaction is
 * labelled by both a file number (fileid) and a byte position relative to the
 * specified file (position). */

EXPORT int ubik_Seek(transPtr, fileid, position)
    IN struct ubik_trans *transPtr;	/* IN */
    IN long fileid;    /* IN */
    IN long position;  /* IN */ {
    long code;

    icl_Trace3(ubikdisk_iclSetp,UK_TRACE_SEEK,ICL_TYPE_POINTER,transPtr,
	       ICL_TYPE_LONG, fileid, ICL_TYPE_LONG, position);
    ubik_versionLockBoth(transPtr->dbase);
    if (!urecovery_AllBetter(transPtr->dbase, transPtr->flags & TRREADANY)) {
	code = UNOQUORUM;
    } else  {
	transPtr->seekFile = fileid;
	transPtr->seekPos = position;
	code = 0;
    }
    ubik_versionUnlock(transPtr->dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SEEK_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, code);
    return code;
}

/* This call returns the file pointer associated with the specified transaction
 * in fileid and position. */

EXPORT int ubik_Tell(transPtr, fileid, position)
     IN  struct ubik_trans *transPtr;	/* IN */
     OUT long *fileid;   /* OUT */
     OUT long *position; /* OUT */ {

    icl_Trace1(ubikdisk_iclSetp,UK_TRACE_TELL,ICL_TYPE_POINTER,transPtr);
    ubik_versionLockBoth(transPtr->dbase);
    *fileid = transPtr->seekFile;
    *position = transPtr->seekPos;
    ubik_versionUnlock(transPtr->dbase);
    ubik_rele();
    icl_Trace3(ubikdisk_iclSetp, UK_TRACE_TELL_EXIT, ICL_TYPE_POINTER,transPtr,
	       ICL_TYPE_LONG, *fileid, ICL_TYPE_LONG, *position);
    return 0;
}

/* This sets the file size for the currently-selected file to length bytes, if
 * length is less than the file's current size. */

EXPORT int ubik_Truncate(transPtr, length)
    IN  struct ubik_trans * volatile transPtr;	/* in */
    IN  long length;    /* in */ {
    volatile long code;
    char *excNameP, excString[20];
    extern char *ExceptionToString();
    extern int ubik_BulkUpdateCompatRtn();
    ubik_bulk_update *tmp_bulk;

    icl_Trace2(ubikdisk_iclSetp,UK_TRACE_TRUNCATE,ICL_TYPE_POINTER,transPtr,
	       ICL_TYPE_LONG, length);
    if (transPtr->type != UBIK_WRITETRANS) {
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_TRUNCATE_EXIT, ICL_TYPE_POINTER,
		   transPtr, ICL_TYPE_LONG, UBADTYPE);
	return UBADTYPE;
    }
    ubik_versionLockBoth(transPtr->dbase);
    /* first, check that quorum is still good, and that dbase is up-to-date */
    if (!urecovery_AllBetter(transPtr->dbase, transPtr->flags & TRREADANY)) {
	ubik_versionUnlock(transPtr->dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_TRUNCATE_EXIT, ICL_TYPE_POINTER,
		   transPtr, ICL_TYPE_LONG, UNOQUORUM);
	return UNOQUORUM;
    }
    if (!ubeacon_AmSyncSite()) {
	ubik_versionUnlock(transPtr->dbase);
	ubik_rele();
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_TRUNCATE_EXIT, ICL_TYPE_POINTER,
		   transPtr, ICL_TYPE_LONG, UNOTSYNC);
	return UNOTSYNC;
    }
    TRY {
      /* now do the operation locally, and propagate it out */
      code = udisk_truncate(transPtr, transPtr->seekFile, length);
    } CATCH_ALL {
	excNameP = ExceptionToString(THIS_CATCH);
	if (excNameP == NULL) {
	  sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	  excNameP = excString;
	}
	icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		   ICL_TYPE_STRING,
		   excNameP, ICL_TYPE_STRING, __FILE__, 
		   ICL_TYPE_LONG,  __LINE__);
	code = UEXCEPTION;
    } ENDTRY
    if (code == 0) {
	transPtr->bulk[transPtr->nbulk].op = UBULK_TRUNC;
	transPtr->bulk[transPtr->nbulk].arg1 = transPtr->seekFile;
	transPtr->bulk[transPtr->nbulk].arg2 = length;
	transPtr->bulk[transPtr->nbulk].arg3 = 0;
	transPtr->bulk[transPtr->nbulk].arg4 = 0;
	transPtr->bulk[transPtr->nbulk].length = 0;
	(transPtr->nbulk)++;
	if ( transPtr->nbulk == ubik_maxbulk ) {
	    /* if an async RPC is running then wait for it to complete */
	    code = WaitContactQuorum(transPtr,NULL);
	    if (code) {
	        udisk_abort(transPtr);
	        /* force aborts to the others */
	        ContactQuorum(UBIKDISK_Abort, transPtr, 0, 1, NULL);
	        ubik_versionUnlock(transPtr->dbase);
	        ubik_rele();
	        icl_Trace2(ubikdisk_iclSetp, UK_TRACE_TRUNCATE_EXIT,
			   ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG, code);
	        return code;
	    }
	    /*
	     * swap the bulk and rpc_bulk and start the Async RPC Call
	     */
	    tmp_bulk = transPtr->bulk;
	    transPtr->bulk = transPtr->rpc_bulk;
	    transPtr->rpc_bulk = tmp_bulk;
	    transPtr->rpc_nbulk = transPtr->nbulk;
	    transPtr->nbulk = 0;
	    code = AsyncContactQuorum(UBIKDISK_BulkUpdate, transPtr, 0, 2,
				      ubik_BulkUpdateCompatRtn,
				      transPtr->rpc_nbulk,transPtr->rpc_bulk);
	    if (code) {
	        udisk_abort(transPtr);
	        /* force aborts to the others */
	        ContactQuorum(UBIKDISK_Abort, transPtr, 0, 1, NULL);
	        ubik_versionUnlock(transPtr->dbase);
	        ubik_rele();
	        icl_Trace2(ubikdisk_iclSetp, UK_TRACE_TRUNCATE_EXIT,
			   ICL_TYPE_POINTER, transPtr, ICL_TYPE_LONG, code);
	        return code;
	    }
	}
    }
    if (code) {
	/* we must abort the operation */
	udisk_abort(transPtr);
	ContactQuorum(UBIKDISK_Abort, transPtr, 0, 1, NULL);    /* force aborts to the others */
    }
    ubik_versionUnlock(transPtr->dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_TRUNCATE_EXIT, ICL_TYPE_POINTER,
	       transPtr, ICL_TYPE_LONG, code);
    return code;
}

/* set a lock; all locks are released on transaction end (commit/abort) */
EXPORT int ubik_SetLock(atrans, apos, alen, atype)
    IN  struct ubik_trans * volatile atrans;
    IN  long apos, alen;
    IN  int atype;
{
    volatile long code;
    char *excNameP, excString[20];
    extern char *ExceptionToString();
    extern int ubik_BulkUpdateCompatRtn();
    ubik_bulk_update *tmp_bulk;

    icl_Trace4(ubikdisk_iclSetp, UK_TRACE_SETLOCK, ICL_TYPE_POINTER, 
	       atrans, ICL_TYPE_LONG, apos, ICL_TYPE_LONG, alen, ICL_TYPE_LONG,
	       atype);
    if (atrans->type == UBIK_READTRANS && atype == LOCKWRITE) {
	icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SETLOCK_EXIT, ICL_TYPE_POINTER,
		   atrans, ICL_TYPE_LONG, UBADTYPE);
	return UBADTYPE;
    }
    ubik_versionLockBoth(atrans->dbase);
    if (atype == LOCKREAD) {
	code = ulock_SetLock(atrans, apos, alen, atype, 1);
    } else {
	/* first, check that quorum is still good, and that dbase is 
	 * up-to-date */
	if (!urecovery_AllBetter(atrans->dbase, atrans->flags & TRREADANY)) {
	    ubik_versionUnlock(atrans->dbase);
	    ubik_rele();
	    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SETLOCK_EXIT, 
		       ICL_TYPE_POINTER, atrans, ICL_TYPE_LONG, UNOQUORUM);
	    return UNOQUORUM;
	}
	if (!ubeacon_AmSyncSite()) {
	    ubik_versionUnlock(atrans->dbase);
	    ubik_rele();
	    icl_Trace2(ubikdisk_iclSetp,UK_TRACE_SETLOCK_EXIT,ICL_TYPE_POINTER,
		   atrans, ICL_TYPE_LONG, UNOTSYNC);
	    return UNOTSYNC;
	}
	TRY {
	  /* now do the operation locally, and propagate it out */
	  code = ulock_SetLock(atrans, apos, alen, atype, 1);
	} CATCH_ALL {
	  excNameP = ExceptionToString(THIS_CATCH);
	  if (excNameP == NULL) {
	    sprintf(excString, "[[unk: %#lx]]", THIS_CATCH);
	    excNameP = excString;
	  }
	  icl_Trace3(ubikdisk_iclSetp, UK_TRACE_EXCEPTION, 
		     ICL_TYPE_STRING,
		     excNameP, ICL_TYPE_STRING, __FILE__, 
		     ICL_TYPE_LONG,  __LINE__);
	  code = UEXCEPTION;
	} ENDTRY
	if (code == 0) {
	    atrans->bulk[atrans->nbulk].op = UBULK_LOCK;
	    atrans->bulk[atrans->nbulk].arg1 = 0;
	    atrans->bulk[atrans->nbulk].arg2 = apos;
	    atrans->bulk[atrans->nbulk].arg3 = alen;
	    atrans->bulk[atrans->nbulk].arg4 = LOCKWRITE;
	    atrans->bulk[atrans->nbulk].length = 0;
	    (atrans->nbulk)++;
	    if ( atrans->nbulk == ubik_maxbulk ) {
	        /* if an async RPC is running then wait for it to complete */
	        code = WaitContactQuorum(atrans,NULL);
		if ( code == 0 ) {
	            /*
	             * swap the bulk and rpc_bulk and start the Async RPC Call
	             */
	            tmp_bulk = atrans->bulk;
	            atrans->bulk = atrans->rpc_bulk;
	            atrans->rpc_bulk = tmp_bulk;
	            atrans->rpc_nbulk = atrans->nbulk;
	            atrans->nbulk = 0;
	            code = AsyncContactQuorum(UBIKDISK_BulkUpdate, atrans,
					      0, 2, ubik_BulkUpdateCompatRtn,
				              atrans->rpc_nbulk,
					      atrans->rpc_bulk);
		}
	    }
	}
	if (code) {
	    /* we must abort the operation */
	    udisk_abort(atrans);
	    /* force aborts to the others */
	    ContactQuorum(UBIKDISK_Abort, atrans, 0, 1, NULL);
	}
    }
    ubik_versionUnlock(atrans->dbase);
    ubik_rele();
    icl_Trace2(ubikdisk_iclSetp, UK_TRACE_SETLOCK_EXIT, ICL_TYPE_POINTER,
	       atrans, ICL_TYPE_LONG, code);
    return code;
}

/* utility to wait for a version # to change -- not used? */
PRIVATE int ubik_WaitVersion(adatabase, aversion)
  IN  struct ubik_version *aversion;
  IN  struct ubik_dbase *adatabase;
{
    if (adatabase && aversion)
      icl_Trace4(ubikdisk_iclSetp, UK_TRACE_WAIT_VERSION, ICL_TYPE_LONG,
		 adatabase->version.epoch, ICL_TYPE_LONG, 
		 adatabase->version.counter, ICL_TYPE_LONG, aversion->epoch, 
		 ICL_TYPE_LONG, aversion->counter);
    ubik_versionLockBoth(adatabase);
    while (1) {
	/* wait until version # changes, and then return */
	if (vcmp(*aversion, adatabase->version) != 0) {
	    ubik_versionUnlock(adatabase);
	    ubik_rele();
	    return 0;
	}
	ubik_sleep(adatabase, (opaque) &adatabase->version);	/* same vers, just wait */
	ubik_versionLock(adatabase);	/* reobtain lock (rehold) */
    }
    /* not reached */
}

/* Facility to simplify database caching.  Returns zero if last trans was done
   on the local server and was successful.  If return value is non-zero and the
   caller is a server caching part of the Ubik database, it should invalidate
   that cache.  A return value of -1 means bad (NULL) argument. */

EXPORT int ubik_CacheUpdate (atrans)
  IN  struct ubik_trans *atrans;
{
    long code;
    struct ubik_dbase *tdb;

    ubik_hold();
    if (!(atrans && atrans->dbase)) {
	code = -1;
    } else {
	tdb = atrans->dbase;
	ubik_versionLock(tdb);
	code = (vcmp(tdb->cachedVersion, tdb->version) != 0);
	ubik_versionUnlock(tdb);
    }
    ubik_rele();
    return code;
}

#if 0
int abort(a, b, c, d)
    char *a, *b, *c, *d; {
    int i,j;

    printf(a, b, c, d);
    i=1;
    j=0;	/* setup for floating point exception */
    j =	i/j;	/* generate floating pt exception */
    printf("BACK FROM ABORT\n");    /* shouldn't come back from floating pt exception */
    exit(1);	/* never know, though */
}
#endif
