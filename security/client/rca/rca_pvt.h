/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rca_pvt.h,v $
 * Revision 1.1.81.2  1996/10/15  20:49:16  arvind
 * 	Change the prototype of rca_check_binding_resolved().
 * 	[1996/10/10  14:42 UTC  jrr  /main/HPDCE02/jrr_davis_1/3]
 *
 * 	Make the retry_count a signed value.
 * 	[1996/03/28  20:12 UTC  jrr  /main/HPDCE02/jrr_davis_1/2]
 *
 * 	Change retry count checks to not loop forever with negative counts.
 * 	In RETRY macros, don't reset PATIENT_REBIND flag is SITE_UPDATEABLE flag
 * 	is set.
 * 	[1996/03/28  16:01 UTC  jrr  /main/HPDCE02/jrr_davis_1/1]
 *
 * Revision 1.1.81.1  1996/10/03  14:47:20  arvind
 * 	Clean up gcc warnings.
 * 	[1996/09/16  20:31 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.79.3  1996/02/18  22:57:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:33  marty]
 * 
 * Revision 1.1.79.2  1995/12/08  18:07:24  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/10/11  18:54 UTC  mullan_s
 * 	Merge fix for CHFts15451
 * 
 * 	HP revision /main/HPDCE02/mullan_chts16596/1  1995/10/09  17:48 UTC  mullan_s
 * 	Merge with burati's fix for CHFts15451
 * 
 * 	HP revision /main/HPDCE02/mb_mothra7/2  1995/10/09  17:37 UTC  burati
 * 	more fixes
 * 
 * 	HP revision /main/HPDCE02/mb_mothra6/1  1995/09/14  20:11 UTC  burati
 * 	Retry for update binding
 * 
 * 	HP revision /main/HPDCE02/4  1995/06/29  18:44 UTC  mullan_s
 * 	Merge fix for CHFts15613
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_chfts15535/1  1995/06/29  18:34 UTC  mullan_s
 * 	Increase RCA_MAX_HANDLES from 8 to 12.
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/15  20:59 UTC  greg
 * 	Mergeout from HPDCE02 to SEC_migrate_merge.
 * 	[1995/05/13  02:41 UTC  greg  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/09  10:33 UTC  sekhar
 * 	Merge to mothra.
 * 
 * 	HP revision /main/HPDCE02/sekhar_mothra_fix/1  1995/04/28  12:48 UTC  sekhar
 * 	[ CHFts15126 ] In TOWER_VEC_FREE() use the provided routine to free tower
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/16  22:09 UTC  burati
 * 	Merge fix for CHFts13726
 * 
 * 	HP revision /main/mb_mothra/1  1995/02/09  17:54 UTC  burati
 * 	Merge fix for CHFts13726 (rebind improvements).
 * 	[1995/12/08  16:32:25  root]
 * 
 * Revision 1.1.75.1  1993/10/05  22:30:05  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:05:08  mccann]
 * 
 * Revision 1.1.5.8  1993/03/09  18:34:44  rps
 * 	Add name parms to rca_site_bind_from_tower_vec
 * 	[1993/03/08  23:29:02  rps]
 * 
 * Revision 1.1.5.7  1993/03/08  17:55:25  rps
 * 	[OT 7272] Add rca_site_check_readonly()
 * 	Fix rca_site_resolve_update() in parameter "icontext"
 * 	[1993/03/03  20:25:06  rps]
 * 
 * Revision 1.1.5.6  1993/02/26  23:47:19  emartin
 * 	[OT7264] Give TOWER_VEC_FREE a choice of deallocator
 * 	[1993/02/25  22:28:13  emartin]
 * 
 * Revision 1.1.5.5  1993/02/21  19:39:53  burati
 * 	CR6726 New arg in prototype for rca_site_resolve_update
 * 	[1993/02/21  00:23:21  burati]
 * 
 * Revision 1.1.5.4  1993/02/05  22:45:03  sommerfeld
 * 	[OT5060] Cache invalidation support:
 * 	Redefine CHECK_CACHE to take both HANDLE and CP, not just CP.
 * 	Declare sec__id_check_cache.
 * 	Add cachehint field to binding structure.
 * 	Reparenthesize handle accessor macros for paranoia.
 * 	[1993/02/04  22:28:29  sommerfeld]
 * 
 * Revision 1.1.5.3  1992/12/29  13:03:25  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:41:01  zeliff]
 * 
 * Revision 1.1.5.2  1992/12/08  19:01:06  rps
 * 	Add tower support.
 * 	OT4043 Need to rebind on more errors than just comm_failure
 * 	[1992/12/03  01:31:08  rps]
 * 
 * Revision 1.1.2.4  1992/06/18  21:19:43  sommerfeld
 * 	Remove spurious extra comma in prototype for rca_unlock_handle.
 * 	[1992/06/18  18:33:43  sommerfeld]
 * 
 * 	Add RCA_MAX_HANDLES, plus hindex and alt_handles array to
 * 	rca_pvt_handle_blk_t to allow rebinding from pe_site file.
 * 	[1992/06/12  21:50:09  sommerfeld]
 * 
 * 	rca_lock_handle -> function, from macro.
 * 	add rca_expand_default_handle
 * 	flush ..._len fields and macros referring to them.
 * 	[1992/06/10  22:45:53  sommerfeld]
 * 
 * Revision 1.1.2.3  1992/05/22  15:15:14  sommerfeld
 * 	Add "uncanon_princ_name" to allow fuzzy matching of principal name.
 * 	[1992/05/21  21:21:18  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/05/08  20:55:20  sommerfeld
 * 	Add cache links.
 * 	[1992/04/29  03:04:46  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:47:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  rca_pvt.h
**
** Copyright (c) Hewlett-Packard Company 1991,1992,1993,1995
** Unpublished work. All Rights Reserved.
**
*/
/* 
**  Internal RCA macros, types and globals
*/

#ifndef _rca_pvt_h__included_
#define _rca_pvt_h__included_

#ifdef _rca_ALLOCATE_GLOBALS_
#   define EXTERN
#   define INITVAL(X) X
#else
#   define EXTERN extern
#   define INITVAL(X)
#endif

#include <dce/rpc.h>
#include <dce/binding.h>
#include <rgybase.h>
#include <rsbase.h>
#include <rplbase.h>
#include <rca_fileloc.h>
#include <pthread.h>
#include <dce/exc_handling.h>

#ifndef NULL
#   define NULL 0L
#endif

#include <macros.h>
#include <rca_pub.h>

/* primitive semaphore operations */
#define P(mutex) pthread_mutex_lock((&mutex))
#define V(mutex) pthread_mutex_unlock((&mutex))

/*
** Macros for retry handling
*/
#define RETRY_LIMIT	5

#define SETUP_RETRY(operation, stp); \
    { \
    boolean32       done; \
    volatile boolean32       locked; \
    signed32        retry_count     = (RETRY_LIMIT); \
    error_status_t  *op_stp         = (stp); \
    \
    TRY {\
        rca_lock_handle(&context, op_stp); \
        locked = STATUS_OK(op_stp); \
        done = !locked; \
        while (pthread_testcancel(), !done && (retry_count > 0) && \
	    rca_check_binding_resolved(&context, operation, &retry_count, op_stp))
	    
/*
 * "normal_case_filler:" and "retry_case_filler:" cause GCC to stop
 * warning about `if (...);' when the caller uses `NORMAL_CASE;'
 * This limits you to one instance of these macros per function.
 */

#define NORMAL_CASE \
    if ((STATUS_EQUAL(op_stp, error_status_ok)) && \
        (!FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_SITE_UPDATEABLE))) {\
        UNSET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_PATIENT_REBIND); \
        done = true; \
    } \
    if (STATUS_EQUAL(op_stp, error_status_ok)) normal_case_filler:

#define RETRY_CASE \
    if (!rca_retry_status(context, *op_stp)) {\
	done = true; \
    } else if (FLAG_SET(HANDLE_FLAGS(context),\
			RCA_HANDLE_FLAGS_PATIENT_REBIND)) {\
	retry_count--; \
    } \
    if (!STATUS_EQUAL(op_stp, error_status_ok)) retry_case_filler:

#define FORCE_RETRY done = false;
#define ABANDON_RETRY done = true;
#define RETRY_END \
            if (locked) { \
                rca_unlock_handle(context); \
            } \
        } CATCH_ALL { \
            if (locked) rca_unlock_handle(context); \
            RERAISE; \
        } \
        ENDTRY \
    } /* End SETUP_RETRY outer scoping */


/*
 * Internal Binding Retry Macros - assumes icontext locked prior to invokation
 */
#define I_SETUP_RETRY(operation, stp); \
    { \
    boolean32       done; \
    signed32        retry_count     = (RETRY_LIMIT); \
    error_status_t  *op_stp         = (stp); \
    \
    TRY {\
        done = false; \
        while (pthread_testcancel(), !done && (retry_count > 0) && \
	    rca_check_binding_resolved(&icontext, operation, &retry_count, op_stp))

#define I_NORMAL_CASE \
    if ((STATUS_EQUAL(op_stp, error_status_ok)) && \
        (!FLAG_SET(HANDLE_FLAGS(icontext), RCA_HANDLE_FLAGS_SITE_UPDATEABLE))) {\
        UNSET(HANDLE_FLAGS(icontext), RCA_HANDLE_FLAGS_PATIENT_REBIND); \
        done = true; \
    }

#define I_RETRY_CASE \
    if (!rca_retry_status(icontext, *op_stp)) {\
	done = true; \
    } else if (FLAG_SET(HANDLE_FLAGS(icontext),\
			RCA_HANDLE_FLAGS_PATIENT_REBIND)) {\
	retry_count--; \
    }

#define I_RETRY_END \
        } CATCH_ALL { \
            rca_unlock_handle(icontext); \
            RERAISE; \
        } \
        ENDTRY \
    } /* End ISETUP_RETRY outer scoping */


/*
 * Retrying replication operations is a risky business, therefore
 * we need replication specific versions of the SETUP_RETRY and
 * RETRY_END macros.  
 */
#define REP_SETUP_RETRY(operation, stp); \
    { \
    boolean32       done, resolved; \
    volatile boolean32       locked = false; \
    signed32        retry_count = 2; \
    error_status_t  *op_stp = (stp), lst; \
    \
    TRY {\
        rca_lock_handle(&context, op_stp); \
        locked = STATUS_OK(op_stp); \
	done = !locked; \
	if (!FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_VALID)) \
	    rpc_binding_reset(HANDLE(context), &lst); \
	while (pthread_testcancel(), !done && (retry_count-- > 0))
	    
#define REP_RETRY_END \
            if (locked) { \
                rca_unlock_handle(context); \
            } \
        } CATCH_ALL { \
            if (locked) rca_unlock_handle(context); \
            RERAISE; \
        } \
        ENDTRY \
    } /* End SETUP_RETRY outer scoping */

/*
** Cache handling macro
*/
extern void sec__id_check_cache (
#ifdef __STDC__
    sec_rgy_handle_t context,
    rs_cache_data_t *cachehint
#endif
);

#define CHECK_CACHE(HANDLE, CP) \
    sec__id_check_cache(HANDLE, CP)


/*
 * Client agent data types
 */

 /*
  * r c a _ p v t _ h a n d l e _ b l k _ t 
  *
  * The internal data structure for representing binding contexts 
  */

#define RCA_MAX_HANDLES 12

typedef struct rca_pvt_handle_blk {
    pthread_mutex_t             mutex;
    unsigned long               handle_flags;
    unsigned long               import_flags;
    rpc_binding_handle_t        rpc_handle;
    rpc_ns_handle_t             import_handle;
    unsigned_char_p_t           site_name; /* the site we're actually talking to */
    unsigned_char_p_t           cell_name;
    unsigned_char_p_t		princ_name;
    sec_rgy_bind_auth_info_t    auth_info;
    int				refcount;
    unsigned32			last_used;
    struct rca_pvt_handle_blk	*cache_next;
    struct rca_pvt_handle_blk	*cache_prev;
    unsigned_char_p_t		uncanon_princ_name;
    unsigned_char_p_t		requested_site_name;
    rs_replica_twr_vec_p_t	tower_vec;
    int				tower_vec_index;
    /*
     * number of times we've tried to import from the name service on
     * this transaction
     */
    int				import_count; 
    rpc_if_handle_t		if_spec;
    int				hindex;
    rpc_binding_handle_t	alt_handles[RCA_MAX_HANDLES];
    rs_cache_data_t		last_cachehint;
}               rca_pvt_handle_blk_t;

#define RCA_HANDLE_FLAGS_NONE               0x00000000L
#define RCA_HANDLE_FLAGS_VALID              0x00000001L
#define RCA_HANDLE_FLAGS_REBINDABLE         0x00000002L
#define RCA_HANDLE_FLAGS_SITE_READONLY      0x00000004L
#define RCA_HANDLE_FLAGS_SITE_UPDATEABLE    0x00000008L
#define RCA_HANDLE_FLAGS_IMPORT_H_VALID     0x00000010L
#define RCA_HANDLE_FLAGS_NO_NAME_CACHE_HINT 0x00000020L
#define RCA_HANDLE_FLAGS_PATIENT_REBIND     0x00000040L

EXTERN  rca_pvt_handle_blk_t    *rca_default_context_block_p INITVAL(= NULL);

#define HANDLE(CONTEXT)     (((rca_pvt_handle_blk_t *) (CONTEXT))->rpc_handle)
#define HANDLE_FLAGS(CTXT)  (((rca_pvt_handle_blk_t *) (CTXT))->handle_flags)
#define SITE_NAME(CONTEXT)  (((rca_pvt_handle_blk_t *) (CONTEXT))->site_name)
#define CELL_NAME(CONTEXT)  (((rca_pvt_handle_blk_t *) (CONTEXT))->cell_name)
#define AUTH_INFO(CONTEXT)  (((rca_pvt_handle_blk_t *) (CONTEXT))->auth_info)
#define HANDLE_CACHEHINT(CONTEXT) (((rca_pvt_handle_blk_t *) (CONTEXT))->last_cachehint)

#define TOWER_VEC_FREE(routine, towerv) \
	do {	unsigned32 i; \
		for (i=0; i<towerv->num_towers; i++) \
			routine(towerv->towers[i]); \
		routine(towerv); \
	} while (0)


#define RCA_DEFAULT_SECIF	secidmap_v1_0_c_ifspec


/*
 * Expand default handle into a "real" handle.
 */
    
PUBLIC  void  rca_expand_default_handle (
#ifdef __STDC__
    sec_rgy_handle_t  *context_p,
    error_status_t    *status
#endif
);
    
/*
 * expand default handle, and lock it.
 */
    
PUBLIC  void  rca_lock_handle (
#ifdef __STDC__
    sec_rgy_handle_t  *context_p,
    error_status_t    *status
#endif
);
                                 
PUBLIC void rca_unlock_handle (
#ifdef __STDC__
    sec_rgy_handle_t  context_p
#endif
);



PUBLIC void rca_site_bind_from_tower_vec (
#ifdef __STDC__
    rpc_tower_vector_p_t        towerv,     /* [in]  */
    unsigned_char_t             *cell_name, /* [in]  */
    unsigned_char_t             *site_name, /* [in]  */
    sec_rgy_bind_auth_info_t    *auth_info, /* [in]  */
    sec_rgy_handle_t            *context,   /* [out] */
    error_status_t              *status
#endif
);

PUBLIC void rca_site_resolve_update (
#ifdef __STDC__
    sec_rgy_bind_auth_info_t    *auth_info, /* [in]  */
    sec_rgy_handle_t            icontext,   /* [in]  */
    sec_rgy_handle_t            *ocontext,  /* [out] */
    error_status_t              *status     /* [out] */
#endif
);

PUBLIC boolean32 rca_site_check_readonly (
#ifdef __STDC__
    sec_rgy_handle_t            context,    /* [in] */
    error_status_t              *status     /* [out] */
#endif
);

/*
 * return true if status code indicates that a retry is in order
 */

PUBLIC boolean32 rca_retry_status (
#ifdef __STDC__
    sec_rgy_handle_t		context_p,  /* [in] */
    error_status_t		status	    /* [in] */
#endif
);

boolean32 rca_check_binding_resolved (
    sec_rgy_handle_t    *context,
    rca_operation_t     which,
    signed32            *retries,
    error_status_t      *(status)
);

/*
** Global Variables
*/

/*
** Store the registry address in shared memory so that a given node
** will continue to use the same server until that server becomes unavailable.
** (Instead of having each process potentially talk to a different server)
*/

typedef struct {
    boolean32       valid;
} rca_rgy_addr_t;

#define MAX_NUM_RGYS    15

#define CUR_INFO_VERSION 6


typedef struct {
    long                use_count; 
} rca_global_hdr_t;

typedef struct {
    unsigned long   db_ts;
    unsigned long   file_mtime;
    unsigned long   file_len;
} rca_unix_file_cache_data_t;

typedef struct {
    rca_global_hdr_t               hdr;
    int                            info_version;
    long                           boot_time;
    uuid_t                         lb_object;
    uuid_t                         rgy_object;
    sec_timeval_sec_t                  next_lookup_time[rca_op_last];
    rca_rgy_addr_t                 address_list[rca_op_last] [MAX_NUM_RGYS];
    short                          num_rgys[rca_op_last];
    rs_cache_data_t                cache_info;
    boolean32                      must_update;
    boolean32                      private_info;
    int                            fd;
    rca_unix_file_cache_data_t     unix_cache[sec_rgy_domain_last];
    unsigned long                  debug_flags;
} rca_global_info_t;

/*
** Debug flag definitions for global_info.debug_flags
*/
#define DEBUG_POLICY_FLAG 0x01L
#define DEBUG_REBIND_FLAG 0x02L

PUBLIC rca_global_info_t * rca_access_global_info (
#ifdef __STDC__
    boolean32           update
#endif
);

PUBLIC void rca_release_global_info (
#ifdef __STDC__
    rca_global_info_t *info
#endif
);

#endif /* _rca_pvt_h__included_ */
