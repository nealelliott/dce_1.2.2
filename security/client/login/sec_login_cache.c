/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_login_cache.c,v $
 * Revision 1.1.12.3  1996/02/18  00:06:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:01  marty]
 *
 * Revision 1.1.12.2  1995/12/08  18:04:31  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/22  17:06 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat9/1  1995/05/22  16:35 UTC  mullan_s
 * 	Added comments regarding thread-safety of sec_login_cache_enter.
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/04  21:03 UTC  cuti
 * 	Force context purged from cache if from purge_context
 * 
 * 	HP revision /main/cuti_machine_context/3  1995/04/04  17:44 UTC  cuti
 * 	Turn off sec_login_cache_trace
 * 
 * 	HP revision /main/cuti_machine_context/2  1995/04/04  17:34 UTC  cuti
 * 	Change cache_entered to cache_purged
 * 
 * 	HP revision /main/cuti_machine_context/1  1995/04/01  21:59 UTC  cuti
 * 	do cache_remove on force if called by purge_context && cache_lookup in get_default_context
 * 	[1995/12/08  16:31:23  root]
 * 
 * Revision 1.1.8.1  1994/10/24  18:32:46  greg
 * 	CR12159: Add a generation uuid to the login context and incorporate it
 * 	into the login context export token.  This allows the  CDS clerk to
 * 	recognize changes to the *fffffff* host machine credentials immediately.
 * 	[1994/10/24  18:05:16  greg]
 * 
 * Revision 1.1.4.3  1992/12/29  12:43:42  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:39:33  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/08  18:58:41  rps
 * 	Fix assertion segv
 * 	[1992/12/03  01:27:49  rps]
 * 
 * Revision 1.1.2.4  1992/05/20  21:21:39  sommerfeld
 * 	Move locking around to avoid recursive deadlock problem.
 * 	[1992/05/20  15:11:59  sommerfeld]
 * 
 * Revision 1.1.2.3  1992/05/14  18:55:19  burati
 * 	Mods to support overrides
 * 	[1992/05/14  01:48:09  burati]
 * 
 * Revision 1.1.2.2  1992/05/08  20:52:12  sommerfeld
 * 	Move sec_login_util_reference, sec_login_util_release here.
 * 	Rearrange debugging printfs, cache sanity checks.
 * 	Add more paranoia against dropping refcount below zero.
 * 	[1992/04/29  03:13:24  sommerfeld]
 * 
 * 	Initial version; verbose tracing, consistancy checks always enabled.
 * 	[1992/04/15  23:05:13  sommerfeld]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */

#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <u_str.h>
#include <assert.h>
#include <rgymacro.h>

#include <sec_login_pvt.h>

/*
 * Login context cache.
 * 
 * This is used to hold a cache of currently active login contexts, so
 * that attempts to reactivate (import) the same login context get the
 * same pointer, thus sharing much memory.  This also allows the
 * auth_info cache in RPC to have a better hit rate.
 * 
 * The entire cache is locked by the "cache_lock" in this module.
 * 
 * We maintain a doubly-linked list, to make deletions easier; the
 * forward link in the last entry and the backward link in the first
 * entry must be NULL.
 * 
 * If this module is build -DSEC_CACHE_DEBUG, much sanity checking is
 * done, including a check for loops and inconsistancy in the cache
 * list on every access.
 */

static pthread_mutex_t cache_lock;
static pthread_once_t   cache_mutex_once = pthread_once_init;
static mtx_inited = 0;

/*
 * statistics, for the curious.
 */
int sec_login_cache_lookups = 0;
int sec_login_cache_hits = 0;
int sec_login_cache_enters = 0;
int sec_login_cache_removes = 0;

static sec_login_context_p_t cache_head = NULL;


static void cache_mutex_init(
#ifdef __STDC__
    void
#endif
        )
{
    pthread_mutex_init(&cache_lock, pthread_mutexattr_default);
    mtx_inited = 1;
}
#define CACHE_MUTEX_INIT() if (!mtx_inited) pthread_once(&cache_mutex_once, cache_mutex_init)


/*
 * While under development, default is "on"
 */
#ifndef SEC_CACHE_DEBUG
#define SEC_CACHE_DEBUG 1
#endif

#if SEC_CACHE_DEBUG

/*
 * initially off by default; we can patch it on using a debugger if we
 * have to.
 */

int sec_login_cache_trace = 0;
#define SEC_DPRINTF(x) do {if (sec_login_cache_trace) printf x; } while(0)
#define CACHE_SANITY_CHECK(present,absent) cache_sanity_check(present, absent)
#else
#define SEC_DPRINTF(x)
#define CACHE_SANITY_CHECK(present,absent)
#endif

#if SEC_CACHE_DEBUG
/*
 * Sanity check the cache; ensure that forward/backward links 
 * are consistant.
 * 
 * Must be called with cache locked.
 * 
 * In addition, ensure that,
 * (1) if non-NULL, "present" is present in the cache.
 * (2) if non-NULL, "absent" is NOT present in the cache.
 */

static void cache_sanity_check(
    sec_login_context_p_t present,
    sec_login_context_p_t absent
)
{
    sec_login_context_p_t cur, next, prev;
    int found_present = 0;
    
    for (cur = cache_head; cur != NULL; cur = next) 
    {
        assert (cur->mark == 0);        /* if this fails, we have a loop.. */
	if (cur == absent) 
	{
	    SEC_DPRINTF(("Found %x in cache, when shouldn't have\n", absent));
	    assert (cur != absent); /* not supposed to be there */	    
	}

        assert (cur->refcount >= 1); 
        cur->mark = 1;
        next = cur->cache_link_next;
        prev = cur->cache_link_prev;
        if (cur == cache_head) 
            assert (prev == NULL);
        if (cur == present)
            found_present = 1;
        
        if (prev != NULL)
            assert (prev->cache_link_next == cur);
        if (next != NULL)
            assert (next->cache_link_prev == cur);
    }
    if (present != NULL) 
    {
	if (!found_present)
	    SEC_DPRINTF(("Did not find %x in cache\n", present));

	if (!present->cache_purged) /* context hasn't been purged from cache */
	    assert (found_present == 1);
	else  /* handle already removed from cache */
            assert (found_present == 0);
    }
    else  /* otherwise we didn't find it.. */
        assert (found_present == 0); /* we better not have.. */

    /* 
     * now, clear all the marks..
     */
    
    for (cur = cache_head; cur != NULL; cur = cur->cache_link_next) 
    {
        assert(cur->mark == 1); /* better not have been cleared in the meantime.. */
        cur->mark = 0;
    }
}
#endif


/*
 * Add lcp to the context cache, assuming it isn't already there.
 * (If it is already there, we have a problem).
 *
 * NOTE!: This routine is not thread-safe. 2 concurrent threads could
 * cache a login context referring to the same underlying credential
 * files. Multi-threaded callers of this routine should call the 
 * thread-safe apper * lookup_and_cache_enter() in sec_login_pvt.c
 */

void sec_login_cache_enter (
    sec_login_context_p_t lcp
)
{
    assert(lcp->cache_link_next == NULL);
    assert(lcp->cache_link_prev == NULL);

    CACHE_MUTEX_INIT();
    
    pthread_mutex_lock (&cache_lock);

    SEC_DPRINTF(("adding %x to cache\n", lcp));
    CACHE_SANITY_CHECK (NULL, lcp);
    
    if (cache_head == NULL) 
    {
        cache_head = lcp;
    } else 
    {
        lcp->cache_link_next = cache_head;
        cache_head->cache_link_prev = lcp;
        cache_head = lcp;
    }
    lcp->refcount ++;

    sec_login_cache_enters++;
    
    CACHE_SANITY_CHECK (lcp, NULL);    
    SEC_DPRINTF(("%x added to cache\n", lcp));
    
    pthread_mutex_unlock (&cache_lock);
}

/*
 * Remove entry from cache; entry *must* be present in cache.
 * Must be called with cache locked.
 * 
 * Simply pull the two entries on either side of this entry together.
 */

static void cache_remove (
    sec_login_context_p_t lcp,
    boolean32             force 
)
{
    sec_login_context_p_t next, prev;
    
    if (!force)
	assert (lcp->refcount == 1);
    assert (cache_head != NULL);

    SEC_DPRINTF(("removing %x from cache\n", lcp));
    CACHE_SANITY_CHECK (lcp, NULL);

    next = lcp->cache_link_next;
    prev = lcp->cache_link_prev;
    
    if (cache_head == lcp) 
    {
        assert (prev == NULL);
        if (next)
        {
            assert (next->cache_link_prev == lcp);
            next->cache_link_prev = NULL;
        }
        cache_head = next;
    } else 
    {
        /* in middle of list.  we must have a previous pointer */

        assert (prev != NULL);
        assert (prev->cache_link_next == lcp);
        prev->cache_link_next = next;

        if (next) 
        {
            assert (next->cache_link_prev == lcp);
            next->cache_link_prev = prev;
        }

    }
    
    lcp->cache_link_next = 0;
    lcp->cache_link_prev = 0;
    SEC_DPRINTF(("%x: dropping login handle refcount from %d to %d\n",
	lcp, lcp->refcount, lcp->refcount-1));
    lcp->refcount--;
    if (!force)
	assert(lcp->refcount == 0);
    
    lcp->cache_purged = true;
    sec_login_cache_removes ++;
    
    CACHE_SANITY_CHECK (NULL, lcp);
    SEC_DPRINTF(("%x removed from cache\n", lcp));
}

/*
 * Decrement a context refcount but, unlike free_login_context,
 * leave the handle intact until the last outstanding reference is 
 * released
 */
void  sec_login_pvt_release_context (
    sec_login_context_p_t   lcp,  /* [in, out] */
    boolean32               force_cache_remove
)
{

    /*
     * obtain lock to avoid races between threads importing and
     * releasing at the same time.
     */
    CACHE_MUTEX_INIT();    
    pthread_mutex_lock (&cache_lock);

    CACHE_SANITY_CHECK(lcp, NULL);
    /* 
     * don't free up the context data until the refcount drops to zero.
     * We want outstanding references to this handle to remain valid until
     * the last one is released (unlike a purge, in which we want any 
     * outstanding references to generate an error).
     */
    SEC_DPRINTF(("%x: dropping login handle refcount from %d to %d\n",
	lcp, lcp->refcount, lcp->refcount-1));
    lcp->refcount--;
    if (lcp->refcount < 0) 
    {
	SEC_DPRINTF(("refcount less than zero: lcp %x, refc %x\n", 
	    lcp, lcp->refcount));
	assert(lcp->refcount >= 0);
    }
    if ((lcp->refcount == 1 && !lcp->cache_purged) || force_cache_remove)
        cache_remove(lcp, force_cache_remove);

    if (lcp->refcount == 0) 
        sec_login_pvt_free_login_context(lcp);
    
    pthread_mutex_unlock (&cache_lock);
}   

    
/*
 * Look in the context cache for a login context with a context_name
 * matching "buf".  If one is found, increment its reference count and
 * return it.
 */

sec_login_context_p_t sec_login_cache_lookup (
    int  buf_len,
    char *buf,
    uuid_t  *generation_id
)
{
    sec_login_context_p_t context;
    
    CACHE_MUTEX_INIT();
    pthread_mutex_lock (&cache_lock);

    SEC_DPRINTF(("cache lookup for %s\n", buf));
    CACHE_SANITY_CHECK (NULL, NULL);

    sec_login_cache_lookups++;
    
    for (context = cache_head; context; context = context->cache_link_next) 
    {
	error_status_t lst;
        if (context->context_name == NULL)
            continue;
        
        if ((buf_len == context->context_name_len) 
           && (memcmp(buf, context->context_name, buf_len) == 0)
           && ((uuid_equal(&context->generation_id, generation_id, &lst) && STATUS_OK(&lst))
              || (uuid_is_nil(generation_id, &lst) && STATUS_OK(&lst))))


        {
            sec_login_cache_hits++;
            break;
        }
    }
    if (context) 
    {
	SEC_DPRINTF(("%x: found in cache\n", context));
	SEC_DPRINTF(("%x: login handle refcount bumped from %d to %d\n", 
	    context, context->refcount, context->refcount+1));
        context->refcount++;
    } else 
    {
	SEC_DPRINTF(("login handle cache miss\n"));
    }

    CACHE_SANITY_CHECK (context, NULL);
    
    pthread_mutex_unlock(&cache_lock);
    return context;
}


void sec_login_util_reference (
    sec_login_handle_t  in,
    sec_login_handle_t  *out,
    error_status_t      *stp
)
{

    sec_login_context_p_t lcp;

    CACHE_MUTEX_INIT();

    *out = in;
    CLEAR_STATUS(stp);

    if (DEFAULT_HANDLE(in)) {
        in = sec_login_pvt_get_default_context(stp);        

        if (GOOD_STATUS(stp)) {
            lcp = sec_login_pvt_check_handle(in, none, stp);    
            *out = lcp;
        }

    } else {
	lcp = sec_login_pvt_check_handle(in, valid, stp);    
	if (GOOD_STATUS(stp))
	    assert(lcp->auth_src != sec_login_auth_src_overridden);
    }

    if (BAD_STATUS(stp)) {
	*out = NULL;
    } else 
    {
	pthread_mutex_lock(&cache_lock);
	CACHE_SANITY_CHECK(lcp, NULL);
	SEC_DPRINTF(("%x: login handle refcount bumped from %d to %d\n",
	    lcp, lcp->refcount, lcp->refcount+1));
	lcp->refcount++;
	pthread_mutex_unlock(&cache_lock);
    }
}
     
void  sec_login_util_release (
    sec_login_handle_t  security_context
)
{
    sec_login_context_p_t lcp;  
    error_status_t  st;

    CLEAR_STATUS(&st);

    /* 
     * neither of these conditions should ever arise, but we check 
     * just for the heck of it
     */
    if (DEFAULT_HANDLE(security_context)) {
       return;
    } else {
       lcp = sec_login_pvt_check_handle(security_context, none, &st);    
       assert (lcp->auth_src != sec_login_auth_src_overridden);
    }

    sec_login_pvt_release_context(lcp, false); /* not to force cache remove */

}
