/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_util.c,v $
 * Revision 1.1.19.2  1996/11/13  18:11:16  arvind
 * 	Expand check (and status stomp) in rs_util_pgo_add_foreign_principal()
 * 	to include sec_rgy_object_exists.
 * 	[1996/11/06  19:32 UTC  jrr  /main/DCE_1.2.2/jrr_122_6/1]
 *
 * 	use NULL as creator id for rs_util_pgo_add_foreign_principal().
 * 	[1996/02/29  16:49 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/3]
 *
 * 	add rs_util_pgo_add_foreign_principal().
 * 	[1996/02/20  15:16 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 *
 * 	multi-cell group work: add cell_uuid as parameter to rs_util_pgo_add() call.
 * 	[1996/02/01  16:45 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:26  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/08/21  15:30 UTC  hanfei  /main/HPDCE02/5]
 *
 * 	merge in bug fix for CHFts16002.
 * 	HP 	[1995/08/18  13:40 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl5/1]
 *
 * 	fix checkpoint -at ##:00 misbehave
 * 	HP 	[1995/07/13  21:46 UTC  burati  /main/HPDCE02/4]
 *
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	HP 	[1995/07/13  20:48 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 *
 * 	Alias ACL work
 * 	HP 	[1995/07/07  19:26 UTC  aha  /main/HPDCE02/3]
 *
 * 	CHFts14236: preserve cell's original primary name
 * 	HP 	[1995/07/06  17:47 UTC  aha  /main/HPDCE02/aha_mothra7/1]
 *
 * 	CHFts14236: disallow deletion of primary cell name;
 * 	add sec_rgy_s_pgo_is_required to rs_util_status_is_denial denial list
 * 	HP 	[1994/12/09  19:01 UTC  hanfei  /main/HPDCE02/2]
 *
 * 	fix in rs_util_checkpt_parse_time()
 * 	HP 	[1994/12/08  17:30 UTC  hanfei  /main/HPDCE02/1]
 *
 * 	merge
 * 	HP 	[1994/12/07  19:32 UTC  hanfei  /main/hanfei_cpi_bl2/1]
 *
 * 	merge
 * 	HP 	[1994/12/07  17:04 UTC  hanfei  /main/hanfei_cpi_bl1/2]
 *
 * 	add code to check invalid time value
 * 	HP 	[1994/12/05  14:49 UTC  hanfei  /main/hanfei_cpi_bl1/1]
 *
 * 	work for rgy checkpoint to be configurable
 * 	[1995/12/08  17:21:45  root]
 *
 * Revision 1.1.19.1  1996/05/10  13:23:39  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/3  1996/03/27  19:07 UTC  hanfei
 * 	fix functions that return no values
 * 	[1996/02/29  22:45 UTC  pdd  /main/HPDCE02/6]
 * 
 * 	lint
 * 	[1996/02/29  22:03 UTC  pdd  /main/HPDCE02/pdd_branch_CHFts17478/1]
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/3  1996/02/29  16:49 UTC  hanfei
 * 	use NULL as creator id for rs_util_pgo_add_foreign_principal().
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/20  15:16 UTC  hanfei
 * 	add rs_util_pgo_add_foreign_principal().
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/01  16:45 UTC  hanfei
 * 	multi-cell group work: add cell_uuid as parameter to rs_util_pgo_add() call.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:18 UTC  psn
 * 	merge in bug fix for CHFts16002.
 * 	[1995/08/21  15:30 UTC  hanfei  /main/HPDCE02/5]
 * 
 * 	fix checkpoint -at ##:00 misbehave
 * 	[1995/08/18  13:40 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl5/1]
 * 
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	[1995/07/13  21:46 UTC  burati  /main/HPDCE02/4]
 * 
 * 	Alias ACL work
 * 	[1995/07/13  20:48 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 * 
 * 	CHFts14236: preserve cell's original primary name
 * 	[1995/07/07  19:26 UTC  aha  /main/HPDCE02/3]
 * 
 * 	CHFts14236: disallow deletion of primary cell name;
 * 	add sec_rgy_s_pgo_is_required to rs_util_status_is_denial denial list
 * 	[1995/07/06  17:47 UTC  aha  /main/HPDCE02/aha_mothra7/1]
 * 
 * 	fix in rs_util_checkpt_parse_time()
 * 	[1994/12/09  19:01 UTC  hanfei  /main/HPDCE02/2]
 * 
 * 	merge
 * 	[1994/12/08  17:30 UTC  hanfei  /main/HPDCE02/1]
 * 
 * 	merge
 * 	[1994/12/07  19:32 UTC  hanfei  /main/hanfei_cpi_bl2/1]
 * 
 * 	add code to check invalid time value
 * 	[1994/12/07  17:04 UTC  hanfei  /main/hanfei_cpi_bl1/2]
 * 
 * 	work for rgy checkpoint to be configurable
 * 	[1994/12/05  14:49 UTC  hanfei  /main/hanfei_cpi_bl1/1]
 * 
 * Revision 1.1.11.1  1994/10/26  21:18:49  aha
 * 	CR12531 UHYP: Enable denial status for audit events in function
 * 	rs_util_status_is_denial.  Now returns "true" for various status
 * 	codes that imply denial due to lack of authorization; returns
 * 	"false" for all others.
 * 	[1994/10/26  20:38:36  aha]
 * 
 * Revision 1.1.7.3  1994/06/02  21:20:46  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:20:22  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, removing cruft.
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  16:55:45  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:57:20  mdf]
 * 
 * Revision 1.1.5.2  1993/12/16  19:12:19  hanfei
 * 	error, error_st use sprintf and write instead of
 * 	printf.  printf can be cancelled while holding
 * 	a mutex_lock.
 * 	[1993/12/15  22:16:28  hanfei]
 * 
 * 	bl6c: rs_util_seqno_create_random, rs_util_seqno_sub
 * 	[1992/12/11  22:39:44  emartin]
 * 
 * Revision 1.1.3.3  1993/09/15  15:51:16  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.9  1993/03/03  22:31:50  emartin
 * 	rs_util_seqno_sprintf - print seqno in decimal
 * 	[1993/03/03  22:28:14  emartin]
 * 
 * Revision 1.1.2.8  1993/02/20  23:29:19  burati
 * 	CR7137 Fix the subtraction of seqno's.
 * 	[1993/02/20  00:03:41  burati]
 * 
 * Revision 1.1.2.7  1992/12/29  16:37:46  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:27:13  zeliff]
 * 
 * Revision 1.1.2.6  1992/12/18  22:13:11  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:54:06  sekhar]
 * 
 * Revision 1.1.2.5  1992/11/04  19:40:35  ahop
 * 	Third replication drop: bl6a
 * 	add linked list mgmt routines
 * 	[1992/11/03  18:50:26  ahop]
 * 
 * Revision 1.1.2.4  1992/10/07  20:37:36  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:19:51  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  17:29:31  burati
 * 	Second replication drop: bl5
 * 	add seqno mgmt routines
 * 	remove obsolete routines
 * 
 * Revision 1.1.2.3  1992/09/29  20:13:11  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:38:15  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:27:36  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:12:09  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1995
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - utility functions
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_util.c,v 1.1.19.2 1996/11/13 18:11:16 arvind Exp $";
#endif
    
#include <un_time.h>
#include <ctype.h>
#include <un_strng.h>
#include <un_io.h>
#include <un_maloc.h>
#include <bstring.h>
#include <dce/dce_error.h>
#include <dce/rplbase.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_util.h>
#include <rs_task.h>
#include <rs_lock.h>
#include <rs_audit_trail.h>
#include <rsdb_policy.h>
#include <rsdb_pvt.h>
#include <rsdb_util.h>
#include <rsdb_name_util.h>
#include <rsdb_dir.h>
#include <rsdb_pgo.h>
#include <rsdb_acl.h>


/* 
 * Local routines 
 */

PRIVATE void go_away (
    error_status_t  st,
    char            *info
);

PRIVATE void print_st (
    error_status_t   status
);


/* 
 * Create first update seqno for this cell's secd.
 * temp  until seqno mgmt is organized
 */
PUBLIC void rs_util_seqno_create
  (
    rs_update_seqno_t   *seqno
  )
{
    seqno->high = 0;
    seqno->low = 100;
}


/* 
 * Create a random seqno.  For now
 * use gettime.
 */
PUBLIC void rs_util_seqno_create_random
  (
    rs_update_seqno_t   *seqno
  )
{ 
    sec_timeval_t   now;

    rs_util_gettime(&now);

    seqno->high = now.sec;
    seqno->low = now.usec;
}


/*
 * rs_util_seqno_next
 *
 * Get the next monotonically increasing sequence 
 * number.
 */
PUBLIC void rs_util_seqno_next
  (
    rs_update_seqno_t   *seqno
  )
{
    seqno->low += 1;

    if (seqno->low == 0) {
	seqno->high += 1;
    }
}


/*
 *  Compare 2 sequence numbers.
 *  Return 0 if they are equal.
 *  Return 1 if seqno1 > seqno2
 *  Return -1 if seqno1 < seqno2 
 */
PUBLIC signed32 rs_util_seqno_cmp
  (
    rs_update_seqno_t   *seqno1,
    rs_update_seqno_t   *seqno2
  )
{
    signed32    retval;

    if (seqno1->high == seqno2->high) {
        if (seqno1->low == seqno2->low) {
            retval = 0;
        }
        else {
            retval = (seqno1->low > seqno2->low) ? 1:-1;
        }
    }
    else {
        retval = (seqno1->high > seqno2->high) ? 1:-1;
    }

    return retval;
}


/*
 * Subtract seqno2 from seqno 1 and return the difference in diff.
 * Seqno1 must be >= seqno2.
 */
PUBLIC void rs_util_seqno_sub
  (
    rs_update_seqno_t   *seqno1,
    rs_update_seqno_t   *seqno2,
    unsigned32          *diff,
    error_status_t      *st
  )
{
    /*
     * Assure that seqnos increase monotonically by no more than MAXINT;
     * return unsigned low order displacement in both wrap and non-wrap cases.
     */

    if ((seqno1->high == seqno2->high &&
	 seqno1->low >= seqno2->low) ||
	(seqno1->high == (seqno2->high + 1) &&
	 seqno1->low < seqno2->low))
    {
	*diff = seqno1->low - seqno2->low;
	CLEAR_STATUS(st);
    }
    else
	SET_STATUS(st, sec_rgy_rep_bad_arg);

}


/*
 * rs_util_seqno_sprintf
 *
 * return the hex string representation
 * of seqno in seqno_str
 */
PUBLIC void  rs_util_seqno_sprintf
  (
    rs_update_seqno_t   *seqno,
    rs_util_seqno_str_t seqno_str
  )
{
    sprintf((char *) seqno_str, "%ld.%ld", 
        seqno->high, seqno->low);
}


/*
 * Initialize a linked list to contain
 * no entries - when a list is empty the
 * pointer to the end of the list points 
 * to the beginning of the list.
 */ 
PUBLIC void rs_util_list_init
  (
    rs_list_t       *list
  )
{
    list->fwd = NULL;
    list->back = list;
}


/*
 * Add an entry to a linked list
 */
PUBLIC void rs_util_list_add
  (
    rs_list_t       *list,
    rs_list_t       *entry
  )
{
    entry->fwd = NULL;
    entry->back = list->back;

    entry->back->fwd = entry;
    list->back = entry;
}


/* 
 * Remove an entry from a
 * linked list
 */
PUBLIC void rs_util_list_remove
  (
    rs_list_t       *list,
    rs_list_t       *entry
  )
{
    entry->back->fwd = entry->fwd;
    if (entry->fwd != NULL)
        entry->fwd->back = entry->back;
    else
        list->back = entry->back;         /* remove from end of list */

    entry->fwd = NULL;
    entry->back = NULL;
}


PUBLIC void rs_util_gettime
  (
    sec_timeval_t   *ts
  )
{
    struct timezone tz;

#ifdef SNI_SVR4_POSIX
    gettimeofday( (struct timeval *) ts);
#else
    gettimeofday( (struct timeval *) ts, &tz);
#endif
}


void rs_util_gettimeofday
  (
    struct timeval  *time_tv
  )
{
    rs_util_gettime((sec_timeval_t *) time_tv);
}


/*
 * r s _ u t i l _ a p p l y _ f u l l _ t i m e s t a m p
 *
 * Apply a full timestamp value
 */
PUBLIC void rs_util_apply_full_timestamp
  (
    sec_timeval_t   *ts
  )
{
    struct timezone tz;

#ifdef SNI_SVR4_POSIX
    gettimeofday( (struct timeval *) ts);
#else
    gettimeofday( (struct timeval *) ts, &tz);
#endif
}


/*
 * r s _ u t i l _ a p p l y _ t i m e s t a m p
 *
 * Apply the high (seconds) portion of a timestamp
 */
PUBLIC void rs_util_apply_timestamp
  (
    sec_timeval_sec_t  *ts
  )
{
    sec_timeval_t   rts;

    rs_util_apply_full_timestamp(&rts);
    *ts = rts.sec;
}


/*
 * r s _ u t i l _ g e t _ c a c h e _ d a t a
 *
 * obtain domain caching information.
 */
PUBLIC void rs_util_get_cache_data
  (
    rs_cache_data_t    *cache_info
  )
{
    cache_info->site_id = rs_state.rep_id;
    rsdb_get_cache_state(&(cache_info->person_dtm),
                                &(cache_info->group_dtm),
                                &(cache_info->org_dtm));
}


/*
 * r s _ u t i l _ u p d a t e _ c a c h e _ i n f o
 *
 * Update a domain's caching information (gets changed on object deletion
 * or UUID<->unix id changes)
 */
PUBLIC void rs_util_update_cache_info
  (
    sec_rgy_domain_t   domain
  )
{
    sec_timeval_sec_t  state;

    rs_util_apply_timestamp(&state);
    rsdb_set_domain_cache_state(domain, &state);
}


/*
 * r s _ u t i l _ c l e a r _ c u r s o r
 *
 * reset a database cursor.
 */
PUBLIC void rs_util_clear_cursor
  (
    sec_rgy_cursor_t   *cursor
  )
{
    if (cursor)
        cursor->valid = false;
}


/*
 * r s _ u t i l _ c h e c k _ c u r s o r
 *
 * Verify that a database cursor is from the local server (also set
 * the server uuid in the cursor to the local server so that this
 * cursor can be used as an output value.
 */
PUBLIC void rs_util_check_cursor
  (
    sec_rgy_cursor_t   *cursor
  )
{
    /*
     * If this cursor did not originate with this server, then junk it. Also
     * set the cursor uuid to this server so that it will be valid in the
     * future.
     */
    if (bcmp( (Pointer) &cursor->source, (Pointer) &rs_state.rep_id,
             sizeof(rs_state.rep_id)) != 0) {
        cursor->source  = rs_state.rep_id;
        cursor->valid   = false;
    }
}


/*
 * r s _ u t i l _ b a d _ c l i e n t
 *
 * Audit a "bad client" event - client sent invalid data etc.
 */
PUBLIC void rs_util_bad_client
  (
    handle_t        h,
    char            *string,
    error_status_t  *status
  )
{
    error_st(*status, string);
    rs_audit_trail_log(h, string, (char *) NULL);
}


/*
 * r s _ u t i l _ f a t a l
 *
 * We've taken a fatal error - report error and terminate the process.
 */
void rs_util_fatal
  (
    char  *string
  )
{
    char           message[512];
    int            slen;
    error_status_t status;

    sprintf(message, "Registry: Fatal Error - %s\n", string);
    slen = strlen(message) + 1;
    write(2, message, slen);

    /*  The status used to be set to sec_s_mod as follows:
     *      SET_STATUS(&status, sec_s_mod
     *
     *  But with SVC code drop ( sec_svc_bl1 ), sec_s_mod  is no longer
     *  defined by the sams compiler. So for now hard code the value of
     *  sec_s_mod ( 0x17122000 ) for now. This function will eventually
     *  be deleted and callers of this function will use an SVC API call.
     */
	
    SET_STATUS(&status, 0x17122000);
    go_away(status, string);
    /* NOTREACHED */
}


/*
 * r s _ u t i l _ f a t a l _ s t
 *
 * Same as ...fatal, but also report a status code
 */
void rs_util_fatal_st
  (
    error_status_t   status,
    char             *string
  )
{
    char           message[512];
    int            slen;

    if (*string) {
        sprintf(message, "Registry: Fatal Error - %s", string);
    } else {
        sprintf(message, "Registry: Fatal Error");
    }
    slen = strlen(message) + 1;
    write(2, message, slen);

    print_st(status);

    go_away(status, string);
    /* NOTREACHED */
}


/*
 * g o _ a w a y
 *
 * Force termination 
 */
PRIVATE void go_away
  (
    error_status_t  st,
    char            *info
  )
{
    rs_task_fatal(st);
}


/* Various flavors of error printouts.
 * We use sprintf and write rather than printf because printf takes a lock to
 * single thread the operation but can be cancelled without releasing the lock.
 */

/*
 * e r r o r
 *
 * report an error
 */
PUBLIC void error
  (
    char *string
  )
{
    char message[512];
    int  slen;

    sprintf(message, "Registry: Error - %s\n", string);
    slen = strlen(message) + 1;
    write(2, message, slen);
}


/*
 * e r r o r _ s t
 *
 * same as error, but report a status code
 */
PUBLIC void error_st
  (
    error_status_t  status,
    char            *string
  )
{
    char message[512];
    int  slen;

    if (*string) {
        sprintf(message, "Registry: Error - %s", string);
    } else {
        sprintf(message, "Registry: Error");
    }
    slen = strlen(message) + 1;
    write(2, message, slen);

    print_st(status);
}


/*
 * p r i n t _ s t
 *
 * Print an error status
 */
PRIVATE void print_st
  (
    error_status_t  status
  )
{
    unsigned char error_text[dce_c_error_string_len];
    char message[dce_c_error_string_len+30];
    int slen;

    dce_error_inq_text(status, error_text, 0);

    sprintf(message, " - 0x%08x - %s\n", STATUS(&status), error_text);
    slen = strlen(message) + 1;
    write(2, message, slen);
}


/*
 * r s _ u t i l _ h a n d l e _ e x c e p t i o n
 *
 * Handle an exception generated by an outgoing RPC call.
 */
PUBLIC void rs_util_handle_exception
  (
    handle_t        handle,
    error_status_t  status
  )
{
   rpc_binding_reset(handle, &status);
}


/*
 * r s _ u t i l _ g e t _ b i n d i n g _ n a m e
 *
 * Get a print string for a binding (caching results for improved
 * access)
 *
 * EXCEPTION - THIS FUNCTION must guard against multi-threaded access
 */
PUBLIC char * rs_util_get_binding_name
  (
    rpc_binding_handle_t  binding
  )
{
#define NUM_ENTS 64
    static struct cache {
        unsigned_char_p_t     string_binding;
            /* parsed fields of string binding */
        unsigned_char_p_t     object_uuid,
                              protseq,
                              netaddr,
                              endpoint,
                              options;
    } cache[NUM_ENTS];
    static int                count = 0;
    error_status_t            status;
    int                       i;
    static unsigned_char_p_t  string_binding = NULL;
    unsigned_char_p_t         object,
                              protseq,
                              netaddr,
                              endpoint,
                              options;

    /* if we've filled the cache, then we're using the single instance
     * of a string binding, and we need to free the last thing we
     * may have assigned to it
     */
    if (string_binding != NULL) {
        free( (char *) string_binding);
    }

    rpc_binding_to_string_binding(binding, &string_binding, &status);
    if (BAD_STATUS(&status)) {
        string_binding = NULL;
        return "***unable to get printstring for address";
    }

    rpc_string_binding_parse(string_binding, &object, &protseq, &netaddr,
        &endpoint, &options, &status);

    if (BAD_STATUS(&status)) {
        if (string_binding != NULL) {
            rpc_string_free(&string_binding, &status);
        }
        string_binding = NULL;
        return "***unable to get printstring for address";
    }

    /* see if we have the binding in the cache already - if so, return
     *  a pointer to it
     */
    for (i = 0; i < count; i++) {
        if (strcmp( (char *) cache[i].protseq, (char *) protseq) == 0  &&
            strcmp( (char *) cache[i].netaddr, (char *) netaddr) == 0) {
            /* We've already got the printstring in the cache */

            if (string_binding != NULL) {
                rpc_string_free(&string_binding, &status);
                string_binding = NULL;
            }

            if (object != NULL) {
                rpc_string_free(&object, &status);
            }

            if (protseq != NULL) {
                rpc_string_free(&protseq, &status);
            }

            if (netaddr != NULL) {
                rpc_string_free(&netaddr, &status);
            }

            if (endpoint != NULL) {
                rpc_string_free(&endpoint, &status);
            }

            if (options != NULL) {
                rpc_string_free(&options, &status);
            }

            return (char *) cache[i].string_binding;
        }
    }

    /* if there's any room in the cache, add a new entry */
    if (count < (NUM_ENTS - 1)) {
        cache[count].string_binding = string_binding;
        string_binding = NULL;
        cache[count].protseq = protseq;
        cache[count].netaddr = netaddr;
        cache[count].endpoint = endpoint;
        cache[count].options = options;
        count++;
        return (char *) cache[count-1].string_binding;
    } else {
        return (char *) string_binding;
    }
}


/*
 * r s _ u t i l _ p g o _ a d d
 *
 * Database write lock required around this operation.
 */
PUBLIC void rs_util_pgo_add
  (
    sec_rgy_domain_t        domain,     /* [in] */
    sec_rgy_name_t          pgo_name,   /* [in] */
    sec_rgy_pgo_item_t      *(item),    /* [in] */
    uuid_t		    *cell_uuid, /* [in] */
    sec_rgy_foreign_id_t    *client_ids,/* [in] */
    boolean32               alias_fix,  /* [in] */
    boolean32               *prop_acl,  /* [out] */
    error_status_t          *(stp)
  )
{
    sec_rgy_name_t      parent_name;
    long                parent_name_len;

    rsdb_pgo_add(domain, pgo_name, item, cell_uuid, client_ids, alias_fix, prop_acl, stp);
    /* On a bad status of dir not found, attempt to create the directories in
     * the pathname and retry the pgo_add operation.
     */
    if (STATUS_EQUAL(stp, sec_rgy_dir_not_found)) {
        /* Use our own copy of name for strip operation so we don't
         * destroy the caller's copy of the name.
         */
        rsdb_name_util_get_parent( pgo_name, parent_name );
        parent_name_len = ustrlen(parent_name);
        rsdb_util_build_dirs(domain, parent_name, parent_name_len, stp);
        if (STATUS_OK(stp)) {
            rsdb_pgo_add(domain, pgo_name, item, cell_uuid, client_ids, alias_fix,
                prop_acl, stp);
	}
    }
}


/*
 * r s _ u t i l _ p g o _ d e l e t e
 *
 * Database write lock required around this operation.
 */
PUBLIC void rs_util_pgo_delete
  (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      pgo_name,   /* [in] */
    error_status_t      *(status)
  )
{
    sec_rgy_name_t      parent_name;
    error_status_t      tmp_status;

    CLEAR_STATUS(status);

    rsdb_pgo_delete(domain, pgo_name, status);
    if(STATUS_OK(status)) {
        rsdb_name_util_get_parent(pgo_name, parent_name);
        rsdb_util_delete_dirs_if_empty(domain, parent_name, &tmp_status);
    }
}


/*
 *  r s _ u t i l _ p g o _ i s _ r e q u i r e d
 *
 *  Return true if named pgo has the is_required flag set.
 *
 *  Database read lock required around this operation.
 */
PUBLIC boolean32 rs_util_pgo_is_required
  (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      pgo_name,   /* [in] */
    error_status_t      *(status)
  )
{
    sec_rgy_cursor_t    cursor;
    sec_rgy_pgo_item_t  pgo;

    CLEAR_STATUS(status);
    cursor.valid = false;

    rsdb_pgo_get_by_name( domain, pgo_name, &cursor, &pgo, status );
    if(STATUS_OK(status)) {
        if( IS_SET(pgo.flags, sec_rgy_pgo_is_required) ) {
            return true;
        }
    }
    return false;
}


/*
 *  r s _ u t i l _ s t a t u s _ i s _ d e n i a l
 *
 *  Return true if status is of a denial type.
 *
 */
PUBLIC boolean32 rs_util_status_is_denial
  (
    error_status_t      status      /* [in] */
  )
{
    switch (status) {
      case sec_s_bad_nonce:             /* incorrect response to challenge */
      case sec_s_not_trustworthy:       /* data originated by untrustworthy security/privilege server */
      case sec_acl_unable_to_authenticate: /* unable to authenticate on acl operation */
      case sec_acl_not_authorized:	/* operation on acl not authorized */
      case sec_key_mgmt_e_unauthorized:	/* The caller is unauthorized to perform operation */
      case sec_priv_s_deleg_token_exp:	/* delegation token has expired */
      case sec_rgy_passwd_invalid:	/* Invalid password */
      case sec_rgy_not_authorized:	/* User is not authorized to update record */
      case sec_rgy_rep_init_ekey_invalid: /* initialization encryption key not valid */
      case sec_login_s_privileged:	/* privileged operation invoked in unprivileged process */
      case sec_login_s_acct_invalid:	/* account not valid for login */
      case sec_login_s_null_password:	/* cannot log in with zero-length password */
      case sec_login_s_invalid_password: /* Invalid password. */
      case sec_login_s_preauth_failed:	/* Preauthentication failure. */
      case sec_attr_unauthorized:	/* unauthorized to perform this operation */
      case sec_pwd_mgmt_str_check_failed: /* password failed password strength server checking policy */
      case sec_pwd_mgmt_not_authorized:	/* unauthorized to call password management server */
      case sec_svc_not_authorized:      /* Not authorized to perform serviceability operation */
      case sec_rgy_not_root:	        /* Not a privileged user */
      case sec_rgy_s_pgo_is_required:   /* Attempted to delete a required pgo entry */
        return true;
        break;
      default:
        return false;
    }
    /* NOTREACHED */
}


/*
 * FUNCTION: rs_util_checkpt_parse_time
 *
 * OVERVIEW: This function will parse a time string in the following
 *	 format into intergers of hour and minute.
 *	 HH:MM where HH = one of [ 0-23 * ]
 *		     MM = one of [ 0-59 * ] and * means every such time unit
 *	       this is a simplifed version of what cron provided.
 * 
 *
 * INPUTS:
 *  A string of the format described above
 *
 * INPUT/OUTPUTS:
 *  char		*time_str	hh:mm
 *
 * OUTPUTS:
 *  unsigned32		at_hour		[0-24]
 *  unsigned32		at_minute	[0-60]
 *
 * RETURNS:
 *  true on success
 */

PUBLIC boolean32 rs_util_checkpt_parse_time(
    char            *time_str,
    unsigned32      *at_hour_p,
    unsigned32      *at_minute_p
  )
{
    char    *min_str;
    char    *p;
    int     i;
 
    if ( (min_str=strchr(time_str, ':')) == NULL ) {
        return false;
    }
    *min_str = '\0';
    min_str++;

    /* 
     * correct syntax is HH:MM, HH is one of [0-23 *] and MM is one of 
     * [0-59 *], map them into 0 for * ( every hour/minute ) and range 
     * [1-24] for hour and [1-60] for minute
     */
    if ( strcmp(time_str, "*") == 0 ) {
	*at_hour_p = 0;
    }
    else {
	p = time_str;
	/* check for a valid number */
	while ( *p != '\0' ) {
	    if ( isdigit(*p) == 0 ) {
		return false;
	    }
	    p++;
	}
    	if ( sscanf(time_str, "%lu", &i) == 0 ) {
	    return false;
        }
	else {
	    *at_hour_p = (i==0)?24:i;
	}
    }

    if ( *at_hour_p > 24 )
        return false;

    if ( strcmp(min_str, "*") == 0 ) {
        *at_minute_p = 0;
    }
    else {
	p = min_str;
	/* check for a valid number */
	while ( *p != '\0' ) {
	    if ( isdigit(*p) == 0 ) {
		return false;
	    }
	    p++;
	}
        if ( sscanf(min_str, "%lu", &i) == 0 ) {
            return false;
        }
        else
            *at_minute_p = (i==0)?60:i;
    }

    if ( *at_minute_p > 60 )
        return false;

    return true;
}


/*
 * FUNCTION: rs_util_checkpt_calc_interval
 *
 * OVERVIEW: This function calculates the new checkpoint interval
 *	 and wait_time base on the at_hour and at_minute and
 *	 current time.
 * 
 *
 * INPUTS:
 *  unsigned32		at_hour   0:every hour, 1-24: specific hour
 *  unsigned32		at_minute 0:every minute, 1-60: specific minute
 *
 * OUTPUTS:
 *  unsigned32		new_interval	new checkpoint interval
 *  unsigned32		wait_time_p	wait then do the new checkpoint
 *
 * RETURNS:
 *  true on success
 */

PUBLIC boolean32 rs_util_checkpt_calc_interval(
    unsigned32	at_hour,
    unsigned32	at_minute,
    unsigned32	*new_interval_p,
    unsigned32	*wait_time_p
  )
{
    time_t	currtime;
    char        currhour_str[8];
    char	*currtime_str;
    unsigned32	curr_hour;
    unsigned32	curr_minute;
    struct timeval	time_val;
    struct timezone     tz_val;
    struct tm		*tm_val_p;

    if ( ( at_hour == 0 ) && ( at_minute == 0 )) {
    /* do checkpoint every minute */
	*new_interval_p = 60;
	*wait_time_p = 0;
	return true;
    }

    if ( ( at_hour > 0 ) && ( at_minute == 0 ) ) {
    /* 
     * cannot do checkpoint on every minutes of a specific hour.
     * this creates irregular interval
     */
        return false;
    }

    if ( gettimeofday(&time_val, &tz_val) == -1 ) {
        return false;
    }

    tm_val_p = localtime((time_t *)&time_val.tv_sec);
    /* get current hour and minute only */

    curr_hour = tm_val_p->tm_hour;
    curr_minute = tm_val_p->tm_min;

    if ( ( at_hour == 0 ) && ( at_minute > 0 ) ) {
    /* do checkpoint every hour at a specific minute */
	*new_interval_p = 60 * 60;	/* one hour */
	*wait_time_p = ( ( ( at_minute + 60 ) - curr_minute ) % 60 ) * 60;
    } 

    if ( ( at_hour > 0 ) && ( at_minute > 0 ) ) {
    /* 
     * do checkpoint on a specific hour and a specific minute.
     * this means only do checkpoint once a day
     */
	*new_interval_p = 24 * 60 * 60;	/* 24 hours */
	*wait_time_p = ( ( (at_hour%24)*60 + (at_minute%60) + 24*60 ) - ( curr_hour*60
			+ curr_minute ) ) % ( 24*60 ) * 60;
    } 
    return true;
}


/*
 * r s _ u t i l _ p g o _ a d d _ f o r e i g n _ p r i n c i p a l
 *
 * Add a foreign principal in the foreign principal domain if it is
 * not created yet.
 */
PUBLIC void rs_util_pgo_add_foreign_principal(
    sec_rgy_name_t		princ_name,
    uuid_p_t			cell_uuid_p,
    uuid_p_t			princ_uuid_p,
    error_status_t		*status
  )
{
    sec_rgy_name_t		p_name;
    sec_rgy_pgo_item_t     	pgo_item;
    boolean32			tmp;
    error_status_t		st;
    
    /* skip leading '/', for creating pgo */
    if ( princ_name[0] == '/' ) {
 	strcpy((char *)p_name, (char *) &princ_name[1]);
    } else {
	strcpy((char *)p_name, (char *) princ_name);
    }
    pgo_item.id = *princ_uuid_p;
    pgo_item.unix_num = sec_rgy_uxid_unknown;
    pgo_item.quota = sec_rgy_quota_unlimited;
    pgo_item.flags = sec_rgy_pgo_flags_none;

    rs_util_pgo_add(sec_rgy_domain_foreign_person,
		    	p_name, &pgo_item, cell_uuid_p, 
		    	NULL, true, &tmp, status);

    if ((STATUS_EQUAL(status, sec_rgy_name_exists))
         || (STATUS_EQUAL(status, sec_rgy_object_exists))) {
    	CLEAR_STATUS(status);
    } 
}





