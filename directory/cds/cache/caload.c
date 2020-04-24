/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: caload.c,v $
 * Revision 1.1.9.2  1996/02/18  19:24:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:11  marty]
 *
 * Revision 1.1.9.1  1995/12/08  00:10:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:53:38  root]
 * 
 * Revision 1.1.6.6  1994/06/30  19:15:12  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:46:34  mccann]
 * 
 * 	sams cleanup drop2
 * 
 * 	sams cleanup drop 2
 * 
 * Revision 1.1.6.5  1994/06/23  19:08:28  jd
 * 	iRemove extern evtblk. It no longer exists.
 * 	[1994/06/23  18:57:35  jd]
 * 
 * Revision 1.1.6.4  1994/06/23  18:28:28  mccann
 * 	sams cleanup drop1
 * 	[1994/06/23  18:03:36  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  16:07:11  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:51:47  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:38:02  peckham
 * 	Remove VMS and UNIX conditionals.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove dce_error_inq_text(EVT_CACHETOSMALL) requests
 * 	and merge text into enclosing error text.
 * 	[1994/04/11  17:46:05  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  21:55:54  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:48:11  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  12:37:10  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:20:23  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/30  20:03:38  peckham
 * 	OT#4480: Insure PATTERN set in ca_ns, ca_Glru, ca_Clru.
 * 	Offset containers (ca_dir, ca_link, etc) were being used
 * 	to hold real address pointers during allocation.
 * 	Don't reference unused fields in CAfixed_p; zero out ca_celldir.
 * 	[1992/06/30  20:03:13  peckham]
 * 
 * Revision 1.1.2.2  1992/03/22  21:14:12  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:05:42  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module caload.c
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
 * Abstract:
 *	This module contains routines dealing with the loading and
 *	initialization of the clerk cache by the Advertiser.
 */

#include <cache.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <dce/dce_utils.h>
#include <stdio.h>

#include <dnsmessage.h>
#include <dns_cfg.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>

CA_PRIVATE void
CA_counter_init (
    struct CacheFixed *const);

CA_PRIVATE dns_status_t
CA_Init (
    struct CacheFixed *const,
    int		            );

CA_PRIVATE dns_status_t
CA_ListInit (
    struct CacheFixed *const,
    int		            );


CA_PRIVATE void 
CA_counter_init (struct CacheFixed *const ca_p)
{
    /* Zero counters */
    ca_p->ca_protocolerrors = 0;
    ca_p->ca_authenticationfailure = 0;
    ca_p->ca_hits = 0;
    ca_p->ca_bypasses = 0;
    ca_p->ca_readoperations = 0;
    ca_p->ca_writeoperations = 0;
    ca_p->ca_miscoperations = 0;
}

/*
 * This routine sets up the cache header, initializes the hash tables
 * and the free list.  CAfixed_p already set up on input.
 */
CA_PRIVATE dns_status_t 
CA_Init (struct CacheFixed *const  ca_p,
         int                       size)
{
    char *handle;
    dns_status_t status;
    error_status_t error_st;

    DCE_SVC_DEBUG((
        cds__svc_handle,
 	cds_svc_cache,
	svc_c_debug2,
	" >CA_Init(,%d)",
	size));

    INS8(ca_p->ca_version.vr_major, MAJOR);
    INS8(ca_p->ca_version.vr_minor, MINOR);

    COPY_bytes(CacheName, ca_p->ca_id, sizeof(CacheName));

    CA_FreeListInit(ca_p, size - sizeof(*ca_p));

    if ((status = CA_ListInit(ca_p, size)) != DNS_SUCCESS)
	return(status);

    /* Set default timeout value */
    handle = dns_cfgopen(NULL, NULL);
    /* Get default timeout, with security we set it high */
#if defined(DCE_SEC)
    ca_p->ca_ClerkTimeout = dns_cfgtol(handle, "default_timeout", 60*5);
#else
    ca_p->ca_ClerkTimeout = dns_cfgtol(handle, "default_timeout", 150);
#endif
    dns_cfgclose (handle);
    ca_p->ca_state = On;

    /*
     * Don't need to cmkrnl for this call on vms, since CA_Init is called
     * in kernel mode
     */
    CA_counter_init(ca_p);
    /*
     *   The 802 address is needed by the create_uid routine.
     */
    dce_get_802_addr((dce_802_addr_t *)&ca_p->ca_802addr, &error_st);
    /*
     * Zero out miscellaneous items in the fixed header
     */
    ca_p->default_ns = 0;
    ca_p->ca_celldir = 0;
    ca_p->last_pid = 0;

    /*
     * Allow 1000 bytes so at least one stale and one good clearinghouse
     * can be stored.  The freelist code will force at least one
     * clearinghouse to be present, so need to allow for
     * 2 when replacing the old one.
     */
    if ((ca_p->ca_total*sizeof(CacheHeader_t)) < (2*sizeof(CleEntry_t))) 
    {
        dce_svc_printf(CDS_S_CACHE_TOOSMALL_MSG);

    	DCE_SVC_DEBUG((
            cds__svc_handle,
 	    cds_svc_cache,
	    svc_c_debug2,
	    " <CA_Init(,%d) return DNS_NOTFND",
	    size));

	return(DNS_NOTFND);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
 	cds_svc_cache,
	svc_c_debug2,
	" <CA_Init(,%d) return DNS_SUCCESS",
	size));

    return(DNS_SUCCESS);
}

/*
 * This routine initializes the hash tables and the free list.
 * CAfixed_p already set up on input.
 */
CA_PRIVATE dns_status_t 
CA_ListInit (struct CacheFixed *const  ca_p,
             int                       size)
{
    register CacheLinkedList_t *ll_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
 	cds_svc_cache,
	svc_c_debug2,
	" >CA_ListInit(,%d)",
	size));

    CA_FreeListInit(ca_p, size - sizeof(*ca_p));

    CAAtomicLock();
      /* Set up nicknames as empty */
      ca_p->ca_ns.ll_flink =CA_GET_OFFSET(&ca_p->ca_ns);
      ca_p->ca_ns.ll_blink =CA_GET_OFFSET(&ca_p->ca_ns);

      /* Set up lru lists as empty */
      ca_p->ca_Clru.hdr.list.ll_flink =CA_GET_OFFSET(&ca_p->ca_Clru);
      ca_p->ca_Clru.hdr.list.ll_blink =CA_GET_OFFSET(&ca_p->ca_Clru);
      ca_p->ca_Clru.hdr.pattern = PATTERN;

      ca_p->ca_Glru.hdr.list.ll_flink =CA_GET_OFFSET(&ca_p->ca_Glru);
      ca_p->ca_Glru.hdr.list.ll_blink =CA_GET_OFFSET(&ca_p->ca_Glru);
      ca_p->ca_Glru.hdr.pattern = PATTERN;
    CAAtomicUnlock();

    /* Set up dirhash table to be empty */
    ca_p->ca_dir_hash_len = DIR_HASH_LEN;
    ll_p = (CacheLinkedList_t *)CA_FreeListAlloc(sizeof(*ll_p) * ca_p->ca_dir_hash_len, Othertype);
    if (ll_p == NULL) 
    {
        dce_svc_printf(CDS_S_CACHE_TOOSMALL_MSG);
	return(DNS_NOTFND);
    }
    INIT_HASH_TABLE(ca_p->ca_dir_hash_len, ll_p);
    ca_p->ca_dir = CA_GET_OFFSET(ll_p);

    /* Set up linkhash table to be empty */
    ca_p->ca_link_hash_len = LINK_HASH_LEN;
    ll_p = (CacheLinkedList_t *)CA_FreeListAlloc(sizeof(*ll_p) * ca_p->ca_link_hash_len, Othertype);
    if (ll_p == NULL) 
    {
        dce_svc_printf(CDS_S_CACHE_TOOSMALL_MSG);
	return(DNS_NOTFND);
    }
    INIT_HASH_TABLE(ca_p->ca_link_hash_len, ll_p);
    ca_p->ca_link = CA_GET_OFFSET(ll_p);

    /* Set up clehash table to be empty */
    ca_p->ca_cle_hash_len = CLE_HASH_LEN;
    ll_p = (CacheLinkedList_t *)CA_FreeListAlloc(sizeof(*ll_p) * ca_p->ca_cle_hash_len, Othertype);
    if (ll_p == NULL) 
    {
        dce_svc_printf(CDS_S_CACHE_TOOSMALL_MSG);
	return(DNS_NOTFND);
    }
    INIT_HASH_TABLE(ca_p->ca_cle_hash_len, ll_p);
    ca_p->ca_cle = CA_GET_OFFSET(ll_p);

    /* Set up userhash table to be empty */
    ca_p->ca_user_hash_len = USER_HASH_LEN;
    ll_p = (CacheLinkedList_t *)CA_FreeListAlloc(sizeof(*ll_p) * ca_p->ca_user_hash_len, Othertype);
    if (ll_p == NULL) 
    {
        dce_svc_printf(CDS_S_CACHE_TOOSMALL_MSG);
	return(DNS_NOTFND);
    }
    INIT_HASH_TABLE(ca_p->ca_user_hash_len, ll_p);
    ca_p->ca_user = CA_GET_OFFSET(ll_p);

    /* Set up namehash table to be empty */
    ca_p->ca_name_hash_len = CA_NAME_HASH_LEN;
    ll_p = (CacheLinkedList_t *)CA_FreeListAlloc(sizeof(*ll_p) * ca_p->ca_name_hash_len, Othertype);
    if (ll_p == NULL) 
    {
        dce_svc_printf(CDS_S_CACHE_TOOSMALL_MSG);
	return(DNS_NOTFND);
    }
    INIT_HASH_TABLE(ca_p->ca_name_hash_len, ll_p);
    ca_p->ca_name = CA_GET_OFFSET(ll_p);

    /* Set up attrhash table to be empty */
    ca_p->ca_attr_hash_len = ATTR_HASH_LEN;
    ll_p = (CacheLinkedList_t *)CA_FreeListAlloc(sizeof(*ll_p) * ca_p->ca_attr_hash_len, Othertype);
    if (ll_p == NULL) 
    {
        dce_svc_printf(CDS_S_CACHE_TOOSMALL_MSG);
	return(DNS_NOTFND);
    }
    INIT_HASH_TABLE(ca_p->ca_attr_hash_len, ll_p);
    ca_p->ca_attr = CA_GET_OFFSET(ll_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
 	cds_svc_cache,
	svc_c_debug2,
	" <CA_ListInit(,%d) return DNS_SUCCESS",
	size));

    return(DNS_SUCCESS);
}

/*
 * Unload cache
 */
void 
CAUnLoad (int CacheId)
{
    CACleanupLocks();
    CASysDelete(CacheId);
}

/*
 * Flush all items from the cache.
 * Called in kernel mode.
 */
dns_status_t 
CAFlush (void)
{
    DEB_ASCII_BUF_dns_status(statusBuf)
    struct CacheFixed *const ca_p = CAfixed_p;
    const int size = ca_p->ca_size;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
 	cds_svc_cache,
	svc_c_debug2,
        " >CAFlush()"));

    CAWriteLockCache();
    CA_FreeListInit(ca_p, size - sizeof(*ca_p));
    status = CA_ListInit(ca_p, size);
    CAWriteUnlockCache();

    DCE_SVC_DEBUG((
        cds__svc_handle,
 	cds_svc_cache,
	svc_c_debug2,
        " <CAFlush() return(%s)",
	deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * CALoad: Create a shared memory segment and load the cache
 * from a file if it exists, otherwise create new cache.
 * If size passed in arg != -1, and differs from size of
 * existing cache, delete old cache and create one with new size.
 */
int 
CALoad (int size)
{
    int CacheId;
    dns_status_t status;
    char *handle;

    DCE_SVC_DEBUG((
        cds__svc_handle,
 	cds_svc_cache,
	svc_c_debug2,
        " >CALoad(%d)",
	size));

    /* Try to get cache from disk */
    CacheId = CASysRead(size);
    if (CacheId != -1) 
    {
	status = CAInitLocks();
	CA_counter_init(CAfixed_p);
	/* Set default timeout value from config file */
	handle = dns_cfgopen(NULL, NULL);
	/* Get default timeout, use existing one if not in config. file */
	CAfixed_p->ca_ClerkTimeout = dns_cfgtol(handle, "default_timeout",
						CAfixed_p->ca_ClerkTimeout);
	dns_cfgclose (handle);

        DCE_SVC_DEBUG((
            cds__svc_handle,
 	    cds_svc_cache,
	    svc_c_debug2,
            " <CALoad(%d) return(%d)",
	    size,
	    CacheId));
	return (CacheId);
    }

    else 
    {
	/* Need to create a new cache */
	CacheId = CASysCreate(size);
	if (CacheId  != -1) 
	{
	    status = CAInitLocks();
	    if (status == DNS_SUCCESS) 
	    {
		status = CA_Init(CAfixed_p, CAfixed_p->ca_size);
		if (status == DNS_SUCCESS)
		  CASysWrite();
	    }
	}
    }

    if ((CacheId != -1) && (status != DNS_SUCCESS)) 
    {
	CACleanupLocks();
	CASysDelete(CacheId);
	CacheId = -1;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_cache,
        svc_c_debug2,
        " <CALoad(%d) return(%d)",
	size,
	CacheId));
    return(CacheId);
}






