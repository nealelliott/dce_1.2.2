/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_util.h,v $
 * Revision 1.1.13.1  1996/05/10  13:15:57  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:45 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/20  15:07 UTC  hanfei
 * 	add header for rs_util_pgo_add_foreign_principal()
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/01  16:49 UTC  hanfei
 * 	multi-cell group work: add cell_uuid as new parameter to rs_util_pgo_add() call.
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:11 UTC  psn
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	[1995/07/13  20:06 UTC  burati  /main/HPDCE02/2]
 *
 * 	Alias work
 * 	[1995/07/13  20:05 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 *
 * 	merge
 * 	[1994/12/08  17:29 UTC  hanfei  /main/HPDCE02/1]
 *
 * 	merge
 * 	[1994/12/07  19:31 UTC  hanfei  /main/hanfei_cpi_bl2/1]
 *
 * 	work for rgy checkpoint to be configurable
 * 	[1994/12/05  14:48 UTC  hanfei  /main/hanfei_cpi_bl1/1]
 *
 * Revision 1.1.6.4  1994/07/15  19:35:42  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/15  19:11:59  mdf]
 * 
 * Revision 1.1.6.3  1994/07/15  14:59:41  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:57  mdf]
 * 
 * Revision 1.1.6.2  1994/05/11  19:06:16  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Add rs_util_stat_is_denial() call for auditing status check.
 * 	[1994/04/29  20:49:07  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	Add rs_util_stat_is_denial() call for auditing status check.
 * 
 * 	hp_sec_to_osf_2 drop
 * 	Add rs_util_stat_is_denial() call for auditing status check.
 * 
 * Revision 1.1.2.6  1992/12/29  13:25:40  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:57  zeliff]
 * 
 * Revision 1.1.2.5  1992/12/18  22:10:53  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:47:03  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:35:24  emartin
 * 	bl6c: decl rs_util_seqno_sub, create_random,
 * 	rs_util_gettimeofday
 * 
 * Revision 1.1.2.4  1992/11/04  19:32:59  ahop
 * 	Third replication drop: bl6a
 * 	declare list mgmt types and procedures, remove obsolete routines
 * 	do rs_util_seqno_next, rs_util_seqno_sprintf, define rs_util_seqno_str_t
 * 	[1992/11/03  18:17:12  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:28:01  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:43  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:42:46  burati
 * 	 Second replication code drop: bl5
 * 	remove obsolete routines, add rs_util_seqno stuff
 * 
 * Revision 1.1.2.2  1992/08/31  18:19:41  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:57:35  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* 
 * Copyright (c) Hewlett-Packard Company 1992, 1995
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - Utility function prototypes
 */

#ifndef rs_util__included
#define rs_util__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rpc.h>
#include <dce/rplbase.h>
#include <dce/rsbase.h>

/*
 * In an ANSI C environment we support catching errno errors in the
 * "fatal_errno" macro.
 */
#ifdef __STDC__
#   include <errno.h>
#   include <string.h>
#endif

/*
 * rs_update_seqno_t string rep
 */
typedef unsigned_char_t     rs_util_seqno_str_t[24];

/*
 *  Linked list mgmt types.
 *  rs_list_t points to first and last entries on a list when it
 *  is used to manage a list.
 *  rs_list_t contains forward and backward ptrs when it is used 
 *  within an entry.
 */

typedef struct rs_list
{
    struct rs_list      *fwd;
    struct rs_list      *back;
} rs_list_t, *rs_list_p_t;


/* rs_util_seqno_create
 *
 * Seed the sequence number generator.  At master creation time the
 * seed should be zero (or some small number).  When an existing master 
 * reboots the seed should be obtained from persistent state info.
 */
void rs_util_seqno_create (
    rs_update_seqno_t   *seqno
);

/* rs_util_seqno_create_random
 *
 * Create a random sequence number.
 */
void rs_util_seqno_create_random (
    rs_update_seqno_t   *seqno
);

/*
 * rs_util_seqno_next
 *
 * Get the next monotonically increasing sequence 
 * number.
 */
void rs_util_seqno_next (
    rs_update_seqno_t   *seqno
);

/*
 * rs_util_seqno_cmp
 *
 * Compare two sequence numbers:
 *  if seqno1 > seqno2 return 1
 *  if seqno1 = seqno2 return 0
 *  if seqno1 < seqno2 return -1
 */
signed32 rs_util_seqno_cmp (
    rs_update_seqno_t   *seqno1,  /* [in] */
    rs_update_seqno_t   *seqno2   /* [in] */
);

/*
 * rs_util_seqno_sub
 *
 * Subtract seqno2 from seqno 1 and
 * return the difference in diff
 */
void rs_util_seqno_sub (
    rs_update_seqno_t   *seqno1,
    rs_update_seqno_t   *seqno2,
    unsigned32          *diff,
    error_status_t      *st
);

/*
 * rs_util_seqno_sprintf
 *
 * return the hex string representation of seqno in seqno_str
 */
void  rs_util_seqno_sprintf (
    rs_update_seqno_t   *seqno,
    rs_util_seqno_str_t seqno_str
);

/*
 * Initialize a linked list to contain no entries
 */ 
void rs_util_list_init (
    rs_list_t       *list
);

/*
 * Add an entry to an entry to a linked list
 */
void rs_util_list_add (
    rs_list_t       *list,
    rs_list_t       *entry
);

/* 
 * Remove an entry from a linked list
 */
void rs_util_list_remove (
    rs_list_t       *list,
    rs_list_t       *entry
);

void rs_util_gettime (
    sec_timeval_t   *ts
);

void rs_util_gettimeofday (
    struct timeval  *time_tv
);

void rs_util_apply_full_timestamp (
    sec_timeval_t   *ts
);

void rs_util_apply_timestamp (
    sec_timeval_sec_t  *ts
);

void rs_util_get_cache_data (
    rs_cache_data_t    *cache_info
);

void rs_util_update_cache_info (
    sec_rgy_domain_t   domain
);

void rs_util_clear_cursor (
    sec_rgy_cursor_t   *cursor
);

void rs_util_check_cursor (
    sec_rgy_cursor_t   *cursor
);

void rs_util_bad_client (
    handle_t         h,
    char             *string,
    error_status_t   *status
);

#define fatal(FATAL_msg) \
   { \
   char buf[1024]; \
   sprintf(buf, "%s\n at line %d of file %s", FATAL_msg, __LINE__, __FILE__); \
   rs_util_fatal(buf); \
   }

#ifdef __STDC__
#   define fatal_errno(FATAL_ERRNO_msg) \
   { \
   char buf[1024]; \
   sprintf(buf, "%s\n %s at line %d of file %s", \
        FATAL_ERRNO_msg, strerror(errno), __LINE__, __FILE__); \
   rs_util_fatal(buf); \
   }
#else
#   define fatal_errno(FATAL_ERRNO_msg) fatal(FATAL_ERRNO_msg)
#endif /* __STDC__ */

void rs_util_fatal(
    char  *string
);


#define fatal_st(FATAL_ST_st,FATAL_ST_msg) \
   { \
   char buf[1024]; \
   sprintf(buf, "%s\n at line %d of file %s", FATAL_ST_msg, __LINE__, __FILE__); \
   rs_util_fatal_st(FATAL_ST_st, buf); \
   }

void rs_util_fatal_st (
    error_status_t   status,
    char             *string
);

void error(
    char  *string
);

void error_st (
    error_status_t   status,
    char             *string
);

void rs_util_handle_exception (
    handle_t        handle,
    error_status_t  status
);

char * rs_util_get_binding_name (
    rpc_binding_handle_t  binding
);

void rs_util_pgo_add (
    sec_rgy_domain_t        domain,     /* [in] */
    sec_rgy_name_t          pgo_name,   /* [in] */
    sec_rgy_pgo_item_t      *(item),    /* [in] */
    uuid_t		    *cell_uuid, /* [in] */
    sec_rgy_foreign_id_t    *client_ids,/* [in] */
    boolean32               alias_fix,  /* [in] */
    boolean32               *prop_acl_p,/* [out] */
    error_status_t          *(status)
);

void rs_util_pgo_delete (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      pgo_name,   /* [in] */
    error_status_t      *(status)
);

boolean32 rs_util_pgo_is_required (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      pgo_name,   /* [in] */
    error_status_t      *(status)
);

boolean32 rs_util_status_is_denial (
    error_status_t      status      /* [in] */
);

boolean32 rs_util_checkpt_parse_time(
    char	      *time_str,
    unsigned32        *at_hour_p,
    unsigned32        *at_minute_p
);

boolean32 rs_util_checkpt_calc_interval(
    unsigned32	at_hour,
    unsigned32	at_minute,
    unsigned32	*new_interval_p,
    unsigned32	*wait_time_p
);

void rs_util_pgo_add_foreign_principal(
    sec_rgy_name_t		princ_name,
    uuid_p_t			cell_uuid_p,
    uuid_p_t			princ_uuid_p,
    error_status_t		*status
);

#endif
