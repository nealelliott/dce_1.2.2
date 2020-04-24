/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: auth_helper.c,v $
 * Revision 1.1.21.1  1996/06/04  21:59:18  arvind
 * 	merge u2u change to sec_krb_get_cred signature from mb_u2u
 * 	[1996/05/07  17:08 UTC  burati  /main/DCE_1.2/2]
 *
 * 	u2u  change for sec_krb_get_cred  signature
 * 	[1996/05/03  14:41 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 *
 * 	u2u work: new arguments for sec_krb_get_cred()
 * 	[1996/05/01  22:56 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 *
 * 	HPDCE02 branch for auth_helper.c
 * 	Merge fix for CHFts15787.
 * 	[1995/07/21  19:01 UTC  mullan_s  /main/HPDCE02/1]
 *
 * 	Fix merge error - remerge fix for OT 9616.
 * 	[1995/07/13  21:52 UTC  mullan_s  /main/mullan_chfts15483/2]
 *
 * 	Add svc debug.
 * 	[1995/07/13  20:30 UTC  mullan_s  /main/mullan_chfts15483/1]
 *
 * Revision 1.1.17.2  1996/02/18  00:08:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:31  marty]
 * 
 * Revision 1.1.17.1  1995/12/08  17:30:34  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/08  17:26 UTC  sommerfeld
 * 	Make debugging info more useful (indicate server princ. name)
 * 	[1995/07/30  16:38 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts15483/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/21  19:01 UTC  mullan_s
 * 	HPDCE02 branch for auth_helper.c
 * 	Merge fix for CHFts15787.
 * 
 * 	HP revision /main/mullan_chfts15483/2  1995/07/13  21:52 UTC  mullan_s
 * 	Fix merge error - remerge fix for OT 9616.
 * 
 * 	HP revision /main/mullan_chfts15483/1  1995/07/13  20:30 UTC  mullan_s
 * 	Add svc debug.
 * 	[1995/12/08  16:54:40  root]
 * 
 * Revision 1.1.15.11  1994/09/30  15:52:09  rsarbo
 * 	retrieve authz_fmt from kernel in
 * 	handle_sec_krb_dg_decode_message and use it.
 * 	[1994/09/30  15:48:01  rsarbo]
 * 
 * Revision 1.1.15.10  1994/09/14  21:05:11  burati
 * 	CR10924 Fix memory leak in handle_sec_krb_dg_decode_message
 * 	[1994/09/14  21:04:45  burati]
 * 
 * Revision 1.1.15.9  1994/08/29  18:55:53  max
 * 	Fix for CR 9313.  Invalid index for context_cache in add_context_cache
 * 	function.
 * 	[1994/08/29  18:54:45  max]
 * 
 * Revision 1.1.15.8  1994/08/24  21:34:35  burati
 * 	CR11861 Get authz_fmt from kernel in handle_sec_krb_dg_build_message
 * 	[1994/08/24  21:29:30  burati]
 * 
 * Revision 1.1.15.7  1994/08/16  14:42:40  burati
 * 	Strip non-useful information from EPAC set.
 * 	[1994/08/16  14:35:40  burati]
 * 
 * Revision 1.1.15.6  1994/08/09  17:32:37  burati
 * 	DFS/EPAC/KRPC/dfsbind changes
 * 	[1994/08/09  16:59:12  burati]
 * 
 * Revision 1.1.15.5  1994/08/04  16:12:52  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:09:41  mdf]
 * 
 * Revision 1.1.15.3  1994/06/02  21:40:50  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:41  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.10.1  1993/10/07  13:12:03  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/07  13:03:07  mccann]
 * 
 * Revision 1.1.6.3  1993/06/02  21:22:29  delgado
 * 	The new auth_helper from IBM
 * 	[1993/06/02  21:21:29  delgado]
 * 
 * Revision 1.1.4.3  1992/12/29  13:28:30  zeliff
 * 	     Embedding copyright notice
 * 	     [1992/12/28  20:47:49  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/03  19:44:57  delgado
 * 	Add locking for credentials cache - the helper is now threaded.
 * 	[1992/12/03  19:44:24  delgado]
 * 
 * Revision 1.1.2.2  1992/06/03  19:02:26  ahop
 * 	 Fix 2143: include sec_login_file_loc.h for cred cache file locations
 * 	[1992/05/22  19:42:54  ahop]
 * 
 * Revision 1.1  1992/01/19  14:55:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  auth_helper.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1994
** Unpublished work. All Rights Reserved.
**
*/
/* 
 * Authentication helper process - dispatching procedures.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <krb5/krb5.h>

#include <dce/conv.h>
#include <dce/sec_authn.h>
#include <dce/sec_login.h>
#include <dce/sec_cred.h>
#include <dce/sec_login_file_loc.h>
#include <dce/sec_pkl.h>
#include <dce/auth_helper.h>
#include <dce/ker/krpc_helper.h>
#include <sec_id_pac_util.h>
#include <sec_svc.h>

extern struct krpch_config krpch_config_params;

static unsigned32 helper_now;

static int helper_debug = 0;
pthread_mutex_t cred_cache_mutex;

#define LOCK_CACHE() pthread_mutex_lock(&cred_cache_mutex);
#define UNLOCK_CACHE() pthread_mutex_unlock(&cred_cache_mutex);
#define LOCK_CACHE_ENTRY(cc) pthread_mutex_lock(&(cc.cc_entry_mutex));
#define UNLOCK_CACHE_ENTRY(cc) pthread_mutex_unlock(&(cc.cc_entry_mutex));
unsigned32 tstamp = 0;	/* stamp for simple LRU algorythm */

dprintf(a,b,c,d,e,f,g,h,i,j)
    char *a, *b, *c, *d, *e, *f, *g, *h, *i, *j;
{
    if (helper_debug) {
	pthread_t self = pthread_self();
	printf("tid(%ld):", (long)pthread_getunique_np(&self));
	printf(a,b,c,d,e,f,g,h,i,j);
    }
    return 0;
}

#define PUT_MESSAGE(ptr, msg) \
     PUT_LONG(ptr, (msg)->length); \
     memcpy(ptr, (msg)->data, (msg)->length); \
     ptr += (msg)->length

#define GET_MESSAGE(ptr, msg) \
   { signed32 temp; \
     GET_LONG(ptr, temp); \
     (msg)->length = temp; \
     (msg)->data = (unsigned char *)ptr; \
     ptr += temp; \
   }

#define GET_UUID(ptr, actuid) \
   { \
     memcpy(&actuid, ptr, sizeof(uuid_t)); \
   }

#define PRINT_CONTEXT_CACHE(context, euid) \
   { \
     DCE_SVC_DEBUG((sec_svc_handle, sec_s_general, svc_c_debug1, \
	"Euid=%d, Contents of context_cache: pag=%08x euid=%d refcount=%d \
        valid=%d cred_valid=%d discard=%d", euid, context.pag, context.euid, \
        context.refcount, context.valid, context.cred_valid, context.discard)); \
   }

/*
 * The local rep of a context is a pair of integers (pag, euid)
 * 
 * The local rep of a cred is a structure containing a copy of the parameters 
 * used to pick it out.
 */

typedef struct {
    unsigned32 pag;
    unsigned32 euid;
    unsigned   char *server_name;
    unsigned32 authn_level;
    unsigned32 authz_proto;
    unsigned32 expiration;
} sec_krb_cred_rep;

static error_status_t cred_from_cred_rep (sec_krb_cred_rep *credrep, 
    sec_krb_cred *cred, sec_login_handle_t *cred_context, int *cache_slot);

/*
 * Serialization and context cache state notes:
 *
 * cred_cache_mutex:
 *	protects access to the cred cache itself.  refcount, valid,
 *	and discard are managed under this lock.  In addtion cred_valid
 *	may only be turned off with this lock held and a reference
 *	count of 0 on the cache entry.  To reclaim a cache entry,
 *	its refcount must be 0 with this lock held.
 *
 * cc_entry_mutex:
 *	lock on each context cache entry.  Used to serialize access
 *	to the cred and credrep info stored in the cache entry.  It
 *	also prevents 2 sec RPC concurrent calls from occurring to fetch cred
 *	info on the same entry.
 * 
 * refcount == 0 && valid == 0 - this slot is free for use
 * refcount == 1 && valid == 0 - slot taken; waiting for context info
 * refcount == 1 && valid == 1 - valid, referenced cred info
 * refcount == 0 && valid == 1 - valid, unreference; candidate for 
 *                                reuse if cache is full
 *
 * cred_valid == 1	       - cred info in cache is valid. Note: valid
 *					must be set in order to set cred_valid
 *
 * discard == 1		       - release cached info and mark invalid
 *					on last reference.
 */
typedef struct 
{
    unsigned32 pag;
    unsigned32 euid;
    char refcount;
    char valid;
    char cred_valid;
    char discard;
    sec_login_handle_t context;
    sec_krb_cred_rep credrep;
    sec_krb_cred cred;
    unsigned32 tstamp;
    pthread_mutex_t cc_entry_mutex;
} context_cache_t, *context_cache_p_t;

/* XXX should be scaled to "number of active users." */
#define CCSIZE 32

/*
 * XXX!!!. matches src/rpc/runtime/krbdgsrv.c, will be moved to header file
 * in next set of changes
 * 
 * The message must fit in a single packet buffer.
 *   RPC_C_DG_MUST_RECV_FRAG_SIZE
 *     - RPC_C_DG_RAW_PKT_HDR_SIZE
 *     - (other parameters' size)
 */
#define MAX_AUTH_MESSAGE_LEN 1344 /* 1464-80-40 */

/* Link list of cred msgs as snarfed from krbdgsrv.c */
typedef struct credentials_ll_elt_t {
    struct credentials_ll_elt_t *next;
    signed32                    buff_len;
    unsigned_char_p_t           buff;
} credentials_ll_elt_t, *credentials_ll_elt_p_t;

context_cache_t context_cache[CCSIZE];

/*
 * Relaease reference on a context/cred cache entry.
 */
static void release_context_cache(cache_slot, login_context) 
    int cache_slot;
    sec_login_handle_t *login_context;
{
    error_status_t st;
    sec_login_handle_t ctx = *login_context;
    *login_context = NULL;
    
    LOCK_CACHE();

    /* Perform some sanity checks first */
    if ((cache_slot < 0) || (cache_slot > (CCSIZE - 1))) {
	dprintf("rel ctx cache: invalid cache slot %d !\n", cache_slot);
	abort();
    }
    if (context_cache[cache_slot].context != ctx) {
	dprintf("%d: rel ctx cache: ctx mismatch!\n", cache_slot);
	abort();
    }

    if (context_cache[cache_slot].refcount < 1) {
	dprintf("rel ctx cache: refcount < 1 !\n");
	abort();
    }

    context_cache[cache_slot].refcount--;
    dprintf("%d: refcount down to %d\n", cache_slot, 
	context_cache[cache_slot].refcount);

    /* 
     * If entry is no longer refrenced and marked for discard,
     * then reclaim it now so it can be reused instead of
     * recycling a valid entry. Otherwise, free entry when cache slot 
     * is reused.
     */
    if (context_cache[cache_slot].discard && 
	(context_cache[cache_slot].refcount == 0)) {
	dprintf("%d: reclaim discarded entry\n", cache_slot);
	sec_login_release_context (&context_cache[cache_slot].context, &st);
	if (context_cache[cache_slot].cred_valid) {
	    sec_krb_cred_free(&context_cache[cache_slot].cred);
	    if (context_cache[cache_slot].credrep.server_name)
		rpc_string_free(&context_cache[cache_slot].credrep.server_name
,			    &st);
	}
        context_cache[cache_slot].valid = 0;
        context_cache[cache_slot].cred_valid = 0;
	context_cache[cache_slot].discard = 0;
    }
    UNLOCK_CACHE();
    return;
}    


/*
 * Mark a context cache entry to be discarded (marked invalid) when the 
 * reference count on the entry drops to zero.
 */
static void set_context_cache_discard(slot)
    int slot;
{
    LOCK_CACHE();
    if (context_cache[slot].discard == 0)
	context_cache[slot].discard = 1;
    UNLOCK_CACHE();
}


/*
 * Search for an entry in the context cache.  If an entry is found, 
 * it is returned with it's refcount incremented.
 */
static int find_context_cache(pag, server)
    unsigned32 pag;
    char *server;
{
    int i;

    LOCK_CACHE();
    tstamp++;
    for (i=0; i<CCSIZE; i++) {
        if ((context_cache[i].pag == pag) && 
	    (strcmp(context_cache[i].credrep.server_name, server) == 0) &&
	    (context_cache[i].valid) &&
	    (context_cache[i].discard == 0)) {
	    if (context_cache[i].refcount < 0) {
		dprintf("bogus refcount in context_cache!\n");
		abort();
            }
	    context_cache[i].refcount++;
	    context_cache[i].tstamp = tstamp;
	    dprintf("%d: pag %x refcount up to %d\n", i, pag, 
		context_cache[i].refcount);
	    break;
	}
    }
    UNLOCK_CACHE();
    if (i < CCSIZE)
	return i;
    else
	return -1;
}


/*
 * Add an entry to the context cache first checking to see of one
 * already exists.  The resulting entry is returned with it's
 * refcount incremented.
 */
static int add_context_cache(pag, euid, server, login_context, exists, st)
    unsigned32 pag;
    unsigned32 euid;
    char *server;
    sec_login_handle_t *login_context;
    int *exists;
    error_status_t *st;
{
    int i;
    unsigned32 oldest_tstamp = ~0;
    int	       oldest_slot = -1;
    int slot=-1;
    *exists = 0;
    *st = error_status_ok;

    LOCK_CACHE();
    for (i=0; i<CCSIZE; i++) {
	/* Check to see if entry has already been added. */
        if ((context_cache[i].pag == pag) && 
	    (strcmp(context_cache[i].credrep.server_name, server) == 0) &&
	    (context_cache[i].valid) &&
	    (context_cache[i].discard == 0)) {
            if (context_cache[i].refcount < 0) {
                dprintf("bogus refcount in context_cache!\n");
                abort();
            }
            context_cache[i].refcount++;
	    UNLOCK_CACHE();
	    *exists = 1;
	    dprintf("add ctx cache found existing entry %d for pag %x\n",
		i, pag);
            dprintf("%d: pag %x refcount up to %d\n", 
		i, pag, context_cache[i].refcount);
	    return(i);		/* Get out now */
        } else if (context_cache[i].valid == 0) {
            /* not valid. remember this for future reference */
            if ((slot < 0) && (context_cache[i].refcount == 0))
                slot = i;
        } else if (context_cache[i].refcount == 0) {
	    /* consider entry for possible LRU replacement */
	    if (context_cache[i].tstamp < oldest_tstamp) {
		oldest_slot = i;
		oldest_tstamp = context_cache[i].tstamp;
	    }
        } 
    }	/* End for */

    if (slot < 0) {	/* must recycle an entry */
	if ((oldest_slot >= 0) && (context_cache[oldest_slot].refcount == 0)) {
	    i = oldest_slot;
	} else {
	    for (i=0; i<CCSIZE; i++) {
		if (context_cache[i].refcount == 0)
		    break;
	    }
	}
	if (i < CCSIZE) {
            if (context_cache[i].refcount == 0) {
                error_status_t xst;
                dprintf("%d: recycled\n", i);
                sec_login_release_context (&context_cache[i].context, &xst);
		if (context_cache[i].cred_valid) {
		    sec_krb_cred_free(&context_cache[i].cred);
		    if (context_cache[i].credrep.server_name)
			rpc_string_free(&context_cache[i].credrep.server_name, 
			    &xst);
		}
                context_cache[i].valid = 0;
                context_cache[i].cred_valid = 0;
                slot = i;
            }
        }
    }
    if (slot >= 0) {
        context_cache[slot].context = *login_context;
        context_cache[slot].pag = pag;
        context_cache[slot].euid = euid;
        context_cache[slot].valid = 1;
	context_cache[slot].refcount = 1;
        context_cache[slot].tstamp = tstamp;
        dprintf("%d: refcount set to %d\n", slot,
            context_cache[slot].refcount);
    } else
        *st = rpc_s_helper_overflow;

    UNLOCK_CACHE();
    return slot;
}


static int check_ownership (euid, slot, st)
    unsigned32 euid;
    int slot;
    error_status_t *st;
{
    int ret;
    struct stat stbuf;
    char tkt_file_path[1024];

    if (context_cache[slot].pag == 0) {
        *st = -1 /* XXX */;
        return -1;
    }
    sprintf (tkt_file_path, 
	SEC_LOGIN_CCACHE_FILE_FROM_PAG_FMT, context_cache[slot].pag);

    ret = stat (tkt_file_path, &stbuf);
    if (ret < 0) {
        *st = -1 /* XXX */;
        return -1;
    }

    if (!S_ISREG(stbuf.st_mode)) {
        PRINT_CONTEXT_CACHE(context_cache[slot], euid);
        *st = rpc_s_helper_wrong_user; /* not quite right */
        return -1;
    }

    if ((euid != 0) &&
        (stbuf.st_uid != euid))  {
        PRINT_CONTEXT_CACHE(context_cache[slot], euid);
        *st = rpc_s_helper_wrong_user;
        return -1;
    }
    *st = error_status_ok;
    return slot;
}


static int  get_context_from_pag (pag, euid, server, login_context, st)
    unsigned32 pag;
    unsigned32 euid;
    char *server;
    sec_login_handle_t *login_context;
    error_status_t *st;
{
    int slot;

    *st = error_status_ok;
    slot = find_context_cache(pag, server);

    if (slot >= 0) {
	*login_context = context_cache[slot].context;
	if ((context_cache[slot].euid == euid) || (euid == 0)) {
	    (void)check_ownership(euid, slot, st);
	} else {
	    PRINT_CONTEXT_CACHE(context_cache[slot], euid);
	    *st = rpc_s_helper_wrong_user;
	}
	if (*st != error_status_ok) {
	    release_context_cache(slot, login_context);
	    slot = -1;
	}
    } else { /* no entry found in context cache */
	unsigned char import_buf[1024];
	error_status_t xst;
	int exists;

	/* Fetch the context and add it to the context cache */
	sprintf ((char *)import_buf, SEC_LOGIN_CCACHE_FROM_PAG_FMT, pag);
	dprintf ("Importing pag %x, `%s'\n", pag, import_buf);
	sec_login_import_context (strlen(import_buf)+1, import_buf, 
                              login_context, st);

	if (*st == error_status_ok) {
	    slot = add_context_cache(pag, euid, server, login_context, &exists, st);
	    if (*st == error_status_ok) {
		if (exists) {
		    /* 
		     * Another thread must have beat us to it, release our
		     * context and use the one returned from the cache.
		     */
		    dprintf("discarding duplicate ctx %x\n", *login_context);
                    sec_login_release_context (login_context, &xst);
		    *login_context = context_cache[slot].context;
		}
		if ((context_cache[slot].euid == euid) || (euid == 0)) {
		    (void)check_ownership(euid, slot, st);
		} else {
	            PRINT_CONTEXT_CACHE(context_cache[slot], euid);
		    *st = rpc_s_helper_wrong_user;
		}
		if (*st != error_status_ok) {
		    release_context_cache(slot, login_context);
		    slot = -1;
		}
	    }
	} else {
	    dprintf("unable to import pag %x in `%s': status %x\n",
		pag, import_buf, *st);
	}
    }
    return slot;
}


#define GET_PAC(ptr, pac) \
{ ptr = get_pac (ptr, pac); } 

#define PUT_PAC(ptr, pac) \
{ ptr = put_pac (ptr, pac); }

#define PUT_STRING(ptr, s) \
{ signed32 templen; \
     if (s != NULL) { templen = strlen(s)+1; \
        PUT_LONG(ptr, templen); \
        memcpy(ptr, s, templen); \
        ptr += templen; \
    } else { \
        PUT_LONG(ptr, 1); \
        *ptr++ = 0; \
    } \
}          

#define GET_STRING(ptr, s) \
{ signed32 templen; \
    GET_LONG(ptr, templen); \
    s = (unsigned char *)ptr;  \
    s[templen-1] = 0; \
    ptr += templen; }

#define GET_BLOCK(ptr, b) \
  memcpy(&b, ptr, sizeof(b)); \
  ptr += sizeof(b);

#define PUT_BLOCK(ptr, b) \
  memcpy(ptr, &b, sizeof(b)); \
  ptr += sizeof(b);

#define PUT_NAME(ptr, n) \
 { \
   error_status_t xst; \
   unsigned char *tempname = NULL; \
   if (n != NULL) { \
       status = sec_krb_unparse_name(n, &tempname); \
       PUT_STRING(ptr, tempname); \
       rpc_string_free(&tempname, &xst); \
   } else { \
       PUT_STRING(ptr, NULL); \
   } \
 }   

#define PUT_SEC_BYTES(ptr, data) \
 { \
   PUT_LONG(ptr, (data)->num_bytes); \
   memcpy(ptr, (data)->bytes, (data)->num_bytes); \
   ptr += (data)->num_bytes; \
 }

#define ZERO_MESSAGE(message) \
  { message.data = 0; \
    message.length = 0; \
  }

#define ZERO_PAC(pac) \
{ memset(&pac, 0, sizeof(pac)); \
  pac.groups = 0; \
  pac.foreign_groups = 0; \
}

static char *put_pac (char *ptr, sec_id_pac_t *pac)
{
    sec_id_pickled_pac_t *pickled_pac;
    int length;
    error_status_t status;
    idl_pkl_t *pickle;
    unsigned32 len;

    sec_id_pac_t_pickle(pac, 0, (void *(*)(unsigned32)) malloc, 8,
	&pickle, &len, &status);
    if (status != 0) {
        /* XXX should fail cleanly here */
        abort();
        return 0;
    }
    PUT_LONG(ptr, len);
    memcpy (ptr, pickle, len);
    free(pickle);
    return ptr+len;
}


/*
 * Get the cred for the given cred rep.  The context for the cred is
 * also returned with a refrence held on the context cache entry
 * which has the context and cred handles cached.  The caller should
 * release the context when it is done using the information.
 */
static error_status_t cred_from_cred_rep (sec_krb_cred_rep *credrep, 
    sec_krb_cred *cred, sec_login_handle_t *cred_context, int *cache_slot)
{
    error_status_t status;
    sec_krb_ccache cc;
    sec_krb_parsed_name server = 0;
    unsigned32 expiration;
    int slot;
    sec_login_handle_t context;
    sec_krb_cred_rep *ccredrep;
    
retry:
    /* this may get us a "valid" entry which matches the PAG
     * were're looking for, however cred_from_cred_rep might
     * consider the pag invalid because some of the other
     * attributes don't match (such as server name) and thus
     * recycle a valid cred.
     */
    slot = get_context_from_pag (credrep->pag, credrep->euid,
				 credrep->server_name,
			         &context, &status);
    if (status)
        return status;

    LOCK_CACHE_ENTRY(context_cache[slot]);
    ccredrep = &context_cache[slot].credrep;
    if (context_cache[slot].cred_valid) {
        if ((credrep->pag == ccredrep->pag) &&
            (credrep->euid == ccredrep->euid) &&
            (credrep->authz_proto == ccredrep->authz_proto) &&
            ((credrep->expiration == 0) ||
             (credrep->expiration == ccredrep->expiration)) &&
            ((ccredrep->expiration == 0) ||
                (ccredrep->expiration > (helper_now + 20))) &&
            ((credrep->authn_level == rpc_c_authn_level_none) ==
                (ccredrep->authn_level == rpc_c_authn_level_none)) &&
            (strcmp (credrep->server_name, ccredrep->server_name) == 0))
        {
            *cred = context_cache[slot].cred; 
            dprintf("%d: cred cache hit - cred %x\n", slot, *cred);
            credrep->expiration = ccredrep->expiration;
	    UNLOCK_CACHE_ENTRY(context_cache[slot]);
	    *cred_context = context;
	    *cache_slot = slot;
            return error_status_ok;
        } else {
	    /*
	     * A once valid cred cache is now invalid.  Mark it to be
	     * discarded when there are no longer any references to it.
	     * Go back to the top and try to get a new cache entry for this
	     * context with an emtpy cred cache so it can be filled in.
	     */
            dprintf("%d: valid cred cache is now invalid, discard it\n", slot);
	    dprintf("    crep: pag/euid %x,%x, aproto %d, exp %d, alev %d\n", 
		credrep->pag, credrep->euid, credrep->authz_proto, 
		credrep->expiration, credrep->authn_level);
	    dprintf("        snam <%s>, hnow %d\n", credrep->server_name, 
		helper_now);
	    dprintf("    ccrep: pag/euid %x,%x, aproto %d, exp %d, alev %d\n", 
		ccredrep->pag, ccredrep->euid, ccredrep->authz_proto, 
		ccredrep->expiration, ccredrep->authn_level);
	    dprintf("        snam <%s>\n", ccredrep->server_name);

	    UNLOCK_CACHE_ENTRY(context_cache[slot]);
	    set_context_cache_discard(slot);
	    release_context_cache(slot, &context);
	    goto retry;		/* Try again to get a clean cache entry */
	}
    } else {	/* cred cache was not valid to begin with, its empty */       
	dprintf("%d: cred cache !valid\n", slot);
	cc = context_cache[slot].context;
	dprintf("authn_level %d:%d\n", credrep->authn_level, 
	    ccredrep->authn_level);
	if (credrep->authn_level == rpc_c_authn_level_none) {
            dprintf("not calling sec_krb_get_cred\n");
            status = error_status_ok;
            *cred = NULL;
	} else {
	    /* Note: NULL is used for the context.  This causes
	     * security to used the cached binding info for our self
	     * credentials instead of make a RPC call each time and
	     * allocating resources for every context that comes through.
	     */
            status = sec_krb_sec_parse_name((sec_krb_ccache)NULL,
            rpc_c_authn_level_pkt_integrity, credrep->server_name, &server);
            if (status == error_status_ok) {
		dprintf("calling sec_krb_get_cred (%s)\n",
                        credrep->server_name);
		status = sec_krb_get_cred (cc, server, credrep->authn_level,
		    credrep->authz_proto, 0, NULL, cred, &credrep->expiration);
	    }
	}

	if (status == error_status_ok)
	{
            dprintf("cred cache fill slot %d - cred %x\n", slot, *cred);
            ccredrep->pag = credrep->pag;
            ccredrep->euid = credrep->euid;
            ccredrep->authn_level = credrep->authn_level;
            ccredrep->authz_proto = credrep->authz_proto;
            ccredrep->expiration = credrep->expiration;
	    ccredrep->server_name = (unsigned char *)
		rpc__stralloc (credrep->server_name);
	    context_cache[slot].cred = *cred;
            context_cache[slot].cred_valid = 1;
	    *cred_context = context;
	    *cache_slot = slot;
            dprintf("%d: cred cache fill done\n", slot);
	} else
	    release_context_cache(slot, &context);
    }

    if (server) sec_krb_parsed_name_free (&server); 
    UNLOCK_CACHE_ENTRY(context_cache[slot]);
    return status;
}


typedef int (*sec_request_handler_t)
    PROTOTYPE((
        char *, int, char *
    ));

static int handle_error (inptr, inlen, outbuf)
    char *inptr;
    int inlen;
    char *outbuf;
{
    char *outptr = outbuf;
    PUT_LONG(outptr, 0x66660003);    
    return outptr-outbuf;
}


static int handle_sec_krb_get_cred (inptr, inlen, outbuf)
    char *inptr;
    int inlen;
    char *outbuf;
{
    char *outptr = outbuf;
    error_status_t status;
    sec_krb_cred cred = 0;
    sec_krb_cred_rep cred_rep;
    sec_login_handle_t context;
    int cache_slot;
    
    /* expanded inline get_cred */
    GET_LONG(inptr, cred_rep.pag);
    GET_LONG(inptr, cred_rep.euid);
    GET_STRING(inptr, cred_rep.server_name);
    GET_LONG(inptr, cred_rep.authn_level);
    GET_LONG(inptr, cred_rep.authz_proto);
    cred_rep.expiration = 0;
    status = cred_from_cred_rep (&cred_rep, &cred, &context, &cache_slot);

out:
    PUT_LONG(outptr, status);
    if (status == error_status_ok) {
        PUT_LONG(outptr, cred_rep.pag);
        PUT_LONG(outptr, cred_rep.euid);
        PUT_LONG(outptr, cred_rep.authn_level);
        PUT_LONG(outptr, cred_rep.authz_proto);
        PUT_LONG(outptr, cred_rep.expiration);
        PUT_STRING(outptr, cred_rep.server_name);
        PUT_LONG(outptr, cred_rep.expiration);
	release_context_cache(cache_slot, &context);
    }
    return outptr-outbuf;
}


static int handle_sec_krb_dg_build_message(inptr, inlen, outbuf)
    char *inptr;
    int inlen;
    char *outbuf;
{
    char *outptr = outbuf;
    sec_krb_ccache context;
    sec_krb_cred cred;
    sec_des_block challenge, ivec;
    sec_des_key key;
    unsigned32 authn_level, authz_proto, ksno;
    error_status_t status;
    sec_krb_message message;
    unsigned32 pag, euid;
    sec_krb_cred_rep crep;
    int cache_slot;
    unsigned32 authz_fmt;

    status = error_status_ok;
    crep.server_name = 0;
    ZERO_MESSAGE(message);

    GET_LONG(inptr, pag);
    GET_LONG(inptr, euid);
    GET_LONG(inptr, crep.pag);
    GET_LONG(inptr, crep.euid);
    GET_LONG(inptr, crep.authn_level);
    GET_LONG(inptr, crep.authz_proto);
    GET_LONG(inptr, crep.expiration);
    GET_STRING(inptr, crep.server_name);

    if (crep.authn_level == 0)
        cred = NULL;
    else
	status = cred_from_cred_rep(&crep, &cred, &context, &cache_slot);

    if (status == error_status_ok) {
	GET_BLOCK(inptr, challenge);
	GET_LONG(inptr, authn_level);
	GET_LONG(inptr, authz_proto);
	GET_LONG(inptr, ksno);
	GET_BLOCK(inptr, key);
	GET_BLOCK(inptr, ivec);
	GET_LONG(inptr, authz_fmt);

	status = sec_krb_dg_build_message (context, cred, &challenge,
	    authn_level, authz_proto, ksno, &key, &ivec, authz_fmt,
					   &message);

	dprintf("(handle_sec_krb_dg_build_message) for %s, returning status %x\n", crep.server_name,
	    status);
	release_context_cache(cache_slot, &context);
    }
  
    PUT_LONG (outptr, status);
    if (status == error_status_ok) {
        PUT_MESSAGE(outptr, &message);
    }
    sec_krb_message_free(&message);
    return outptr-outbuf;
}

static void sec_krb__fetch_message(
    handle_t		handle,
    uuid_t		*actuid,
    unsigned32		boot_time,
    sec_krb_message     *message,
    unsigned_char_p_t   *extended_msg,
    error_status_t	*stp
)
{
    unsigned32 xst, outlen = 0;
    credentials_ll_elt_p_t cred_head, cred_tail, cred_ptr;
    cred_head = (credentials_ll_elt_p_t) malloc(sizeof(*cred_head));
    cred_head->next = NULL;
    cred_tail = cred_head;

    dprintf("sec_krb__fetch_message fetching more: boot_time = %d,\n",
	boot_time);
    if (helper_debug) {
	unsigned_char_p_t uuid_str;
	uuid_to_string(actuid, &uuid_str, &xst);
	if (xst == 0) {
	    dprintf("\tactivity uuid = %s\n", uuid_str);
	} else {
	    dprintf("\tactivity uuid invalid.\n");
	}
    }

    /* Seed the linked list with the initial portion of the msg passed in */
    cred_head->buff = (unsigned_char_p_t) malloc(message->length);
    memcpy(cred_head->buff, message->data, message->length);
    outlen = cred_head->buff_len = message->length;

    /* Fetch the rest of the credentials */
    while (*stp == rpc_s_partial_credentials) {
	cred_ptr = (credentials_ll_elt_p_t)
            malloc(sizeof(credentials_ll_elt_t));
	cred_tail->next = cred_ptr;
	cred_tail = cred_ptr;
	cred_ptr->next = NULL;

        cred_ptr->buff = (unsigned_char_p_t) malloc(MAX_AUTH_MESSAGE_LEN);
	(*conv_v3_0_c_epv.conv_who_are_you_auth_more)
	    (handle, actuid, boot_time, outlen, MAX_AUTH_MESSAGE_LEN,
		cred_ptr->buff, &cred_ptr->buff_len, stp);
	outlen += cred_ptr->buff_len;
    }

    /* Assuming more of the msg was found, concatenate it to be passed back. */
    if (outlen > message->length) {
        unsigned_char_p_t tmp, outbuf_ptr;

	/* Alloc in separate extended msg var so caller will know to free it */
	if ((*extended_msg = (unsigned_char_p_t) malloc(outlen)) == NULL)
            return;
	
        /* Copy each segment of the auth info message into the return buffer */
        for (cred_ptr = cred_head, tmp = *extended_msg; cred_ptr != NULL; ) {
            memcpy(tmp, cred_ptr->buff, cred_ptr->buff_len);
            tmp += cred_ptr->buff_len;

            cred_ptr = cred_ptr->next;
	    free(cred_head->buff);
            free(cred_head);
            cred_head = cred_ptr;
        }
	message->data = *extended_msg;
	message->length = outlen;
    }
    dprintf("sec_krb__fetch_message complete, message length: %d,\n", outlen);
}

static int handle_sec_krb_dg_decode_message (inptr, inlen, outbuf)
    char *inptr;
    int inlen;
    char *outbuf;
{
    error_status_t xst;
    char *outptr = outbuf;
    sec_krb_message message;
    sec_des_block challenge, ivec;
    unsigned char *client_name, *string_binding, *extended_message = NULL;
    sec_krb_parsed_name server_name;
    sec_id_pac_t client_pac;
    sec_bytes_t raw_epac_set;
    unsigned32 authn_level, authz_proto, key_seq, expiration, authz_fmt;
    sec_des_key key;
    error_status_t status, completion_status = 0;
    unsigned32     boot_time;
    uuid_t         actuid;
    unsigned char  *tmpsvr = NULL;

    ZERO_PAC(client_pac);
    client_name = 0;
    server_name = 0;
  
    GET_MESSAGE(inptr, &message);
    GET_BLOCK(inptr, challenge);
    GET_LONG(inptr, authz_fmt);

    /* See if the message is complete, or if there's more to retrieve */
    GET_LONG(inptr, completion_status);

    dprintf("handle_sec_krb_dg_decode_message initial msglen: %d, completion status %x\n", message.length, completion_status);

    /* If there's more to the message, get the args needed to fetch the rest */
    if (completion_status == rpc_s_partial_credentials) {
	handle_t handle;
	GET_LONG(inptr, boot_time);
	GET_STRING(inptr, string_binding);
        dprintf("handle_sec_krb_dg_decode_message fetching more from %s\n",
	    string_binding);
	rpc_binding_from_string_binding(string_binding, &handle, &status);
	if (status == 0) {
	    GET_UUID(inptr, actuid);
	    /* Fetch the rest of the message */
	    sec_krb__fetch_message(handle, &actuid, boot_time, &message,
                &extended_message, &completion_status);
	    rpc_binding_free(&handle, &xst);
	}
    }

    raw_epac_set.num_bytes = 0;
    raw_epac_set.bytes = NULL;
    status = sec_krb_dg_decode_msg_frags (&message, &challenge,
        authz_fmt, &client_name, &client_pac, &raw_epac_set, NULL,
        &server_name, &authn_level, &authz_proto, &key_seq, &key, &ivec,
        &expiration);

    dprintf(
	"sec_krb_dg_decode_message decoded msg of len %d with status of %x\n",
	message.length, status);

    /* Strip out any undesirables (ERAs, dlg/opt/req restrictions...) */
    sec__encode_strip_undesirables(&raw_epac_set, &status);

    dprintf("sec__encode_strip_undesirables() returned with status: %x\n",
        status);

    /* Get the server name now, so we'll know the length before we try
     * to write anything to the shared buffer.
     */
    if (status == 0) {
	if (server_name != NULL) {
            status = sec_krb_unparse_name(server_name, &tmpsvr);
	}

	/* Make sure we don't write past the end of the buffer */
	if (status == 0) {
            unsigned32 tmpsz = 0;
	    if ((tmpsz = ((sizeof(client_pac)+26+sizeof(key)+sizeof(ivec))+
                (raw_epac_set.num_bytes+strlen(client_name)+strlen(tmpsvr)))) >
		  krpch_config_params.bufsize) {
		status = rpc_s_credentials_too_large;
                dprintf("sec_krb_dg_decode_message: Too much data for kernel buffer - %d  bytes\n", tmpsz);
	    }
	}
    }

    PUT_LONG (outptr, status);
    if (status == 0) {
        PUT_STRING(outptr, client_name);
        PUT_PAC(outptr, &client_pac);
#ifdef DEBUG
        if (helper_debug) 
          {
            dprintf("pac from ticket: "); sec_id_pac_print (&client_pac);
          }
#endif
        PUT_STRING(outptr, tmpsvr);
        PUT_LONG(outptr, authn_level);
        PUT_LONG(outptr, authz_proto);
        PUT_LONG(outptr, key_seq);
        PUT_BLOCK(outptr, key);
        PUT_BLOCK(outptr, ivec);
        PUT_LONG(outptr, expiration);
	PUT_SEC_BYTES(outptr, &raw_epac_set);
    }
    if (client_name)
        rpc_string_free(&client_name, &xst);
    if (server_name)
        sec_krb_parsed_name_free(&server_name);
    if (tmpsvr)
        rpc_string_free(&tmpsvr, &xst);
    if (extended_message)
	free(extended_message);
    if (raw_epac_set.bytes)
	free(raw_epac_set.bytes);
    sec_id_pac_util_free(&client_pac);
    return outptr-outbuf;
}


static int handle_sec_krb_register_server (inptr, inlen, outbuf)
    char *inptr;
    int inlen;
    char *outbuf;
{
    char *outptr = outbuf;
    error_status_t status;
    unsigned_char_p_t name, canon_name;

    GET_STRING(inptr, name);
    status = sec_krb_register_server (name, 0, 0);
    (void) sec_krb_get_server(&canon_name);
    dprintf("register_server returns %d\n", status);
    PUT_LONG (outptr, status);
    PUT_STRING(outptr, canon_name);
    return outptr-outbuf;
}


static int handle_sec_krb_gen_key (inptr, inlen, outbuf)
     char *inptr;
     int inlen;
     char *outbuf;
{
    char *outptr = outbuf;
    error_status_t status;
    sec_des_key key;

    status = sec_des_new_random_key (&key);
  
    PUT_LONG(outptr, status);
    if (status == 0) {
        PUT_BLOCK(outptr, key);
    }
    return outptr-outbuf;
}


static int handle_sec_krb_gen_block (inptr, inlen, outbuf)
     char *inptr;
     int inlen;
     char *outbuf;
{
    char *outptr = outbuf;
    error_status_t status;
    sec_des_block block;

    status = sec_des_generate_random_block (&block);
    PUT_LONG(outptr, status);
    if (status == 0) {
        PUT_BLOCK(outptr, block);
    }
    return outptr-outbuf;
}


static sec_request_handler_t handlers[] =
{
    handle_error,               /* 0 */
    handle_error,               /* 1 */
    handle_error,               /* 2 */
    handle_sec_krb_get_cred,    /* 3 */
    handle_error,               /* 4 */
    handle_error,               /* 5 */
    handle_error,               /* 6 */
    handle_sec_krb_register_server, /* 7 */
    handle_sec_krb_gen_key,     /* 8 */
    handle_sec_krb_gen_block,   /* 9 */
    handle_sec_krb_dg_build_message, /* 10 */
    handle_sec_krb_dg_decode_message, /* 11 */
};

static int max_handler = sizeof(handlers)/sizeof(handlers[0]);

void auth_helper_init()
{
    sec_passwd_type_t keytype = sec_passwd_none;
    void *cursor;
    error_status_t st;
    int i;

    /*
     * Initialize the RPC runtime in the cheapest way possible.
     */
    
    (void) rpc_binding_handle_equal(NULL, NULL, &st);

    /*
     * Scan through default keytab, registering each unique principal
     * name in keytab, 'just in case'.
     *
     * This allows dfsbind to be restarted after a crash without
     * having to reboot first to get the kernel to reregister.
     */
    sec_key_mgmt_initialize_cursor (
        rpc_c_authn_dce_private,
        NULL,
        NULL,
        (void *)&keytype,
        &cursor,
        &st);
    
    if (st != error_status_ok) {
        dprintf("Nobody home...\n");
        /*
         * nobody home..
         */
        return;
    }

    dprintf("Scanning keytab\n");
    do {
        unsigned_char_p_t principal;
        unsigned32 key_vno;
        
        sec_key_mgmt_get_next_key (cursor, &principal, &key_vno, NULL, &st);
        if (st == error_status_ok) {
            dprintf("registering %s\n", principal);
            st = sec_krb_register_server (principal, NULL, NULL);
            if (st != error_status_ok) {
                dprintf("register of %s failed: %x\n", principal, st);
            }
            free(principal);
        }
    } while (st == error_status_ok);
    dprintf("done: status = %x\n", st);
    sec_key_mgmt_release_cursor(&cursor, &st);
    dprintf("released cursor\n");
    pthread_mutex_init(&cred_cache_mutex, pthread_mutexattr_default);
    /* Initialize the cred cache entry locks */
    for (i=0; i<CCSIZE; i++) {
	pthread_mutex_init(&(context_cache[i].cc_entry_mutex), 
	    pthread_mutexattr_default);
    }
}       

void do_InitAuth(debugFlag)
    int debugFlag;
{
    if (debugFlag)
        helper_debug++;

    auth_helper_init();
}

do_auth_request(inptr, inlen, outbuf, outsize)
    char *inptr;
    signed32 inlen;
    char *outbuf;
    signed32 *outsize;
{
    unsigned32 opcode;

    time ((time_t *)&helper_now);
    GET_LONG(inptr, opcode);
    dprintf("kernel requests opcode %d, helper_now %d\n", opcode, helper_now);
    if (opcode >= max_handler)
        *outsize = handle_error(inptr, inlen, outbuf);
    else
        *outsize = (handlers[opcode])(inptr, inlen, outbuf);
    dprintf("response to opcode %d is 0x%x bytes long (status is %x)\n",
opcode, *outsize,
        *(signed32 *)outbuf);
    return 0;
}
