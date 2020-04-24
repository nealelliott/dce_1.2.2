/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back.h,v $
 * Revision 1.1.12.3  1996/02/18  23:33:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:59  marty]
 *
 * Revision 1.1.12.2  1995/12/08  15:14:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:09  root]
 * 
 * Revision 1.1.8.1  1994/10/25  16:17:30  proulx
 * 	CR#5915 - move struct back_dsc to back_ground.c
 * 	[1994/10/25  15:01:42  proulx]
 * 
 * Revision 1.1.5.4  1994/08/26  21:55:14  zee
 * 	   Add parameter to back_upgrade_directory_actions prototype.
 * 	   [1994/08/26  21:46:51  zee]
 * 
 * Revision 1.1.5.3  1994/08/03  19:01:56  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:55:32  mccann]
 * 
 * Revision 1.1.5.2  1994/06/09  18:38:30  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:55  devsrc]
 * 
 * Revision 1.1.5.1  1994/03/12  22:02:06  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:11:28  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:28:12  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:45:24  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _BACK_H
#define _BACK_H
/*
 * Module back.h
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
#include <dce/dns_record.h>
#include <threads_lib.h>

/*
 * thread list element structure             
 */                                    
#define THS_COMB_IN_PROG   6
#define THS_CANCELED       7
#define THS_BACK_PLANNED   8
#define THS_BACK_ACTIVE    9
#define THS_BACK_WAITING   10
#define THS_CLEANUP_PLANNED 12
#define THS_CLEANUP_WAITING 13
#define THS_CLEANUP_ACTIVE  14

#define COLLECT_CONCURRENT_LIMIT 5


/* Propagates either running or waiting to run */
typedef struct prop_queue_element
    {
    list_header_t	prop_wait;
    int			prop_state;
    int			prop_type;
    ObjUID_u 	        prop_diruid;
    Timestamp_u  	prop_element_cts;
    Timestamp_u 	prop_begin;
    Timestamp_u 	prop_end;
    SimpleName_u	prop_name;
    } prop_queue_element_t;
/* Prop_state values */
#define THS_PROP_IN_PROG   1
#define THS_PROP_PLANNED   2

/* Skulks in progress */
typedef struct skulk_queue_element
    {
    list_header_t		skulk_wait;
    ObjUID_u     		skulk_diruid;
    list_header_t		skulk_match;
    int				*skulk_status_p;
    } skulk_queue_element_t;

/*
 * Buffer to hold multiple update packets
 */
typedef struct update_buffer
    { 
    int			ub_count;
    int			ub_remaining;
    int			ub_total;
    } update_buffer_t;

#define deb_ascii_update_buffer(b,p) deb_ascii_rec(b,p,"NULL_update_buffer")
#define DEB_ASCII_LEN_update_buffer DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_update_buffer(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_update_buffer(b) LOG_ASCII_BUF_rec(b)

typedef struct CREW_CONTEXT_BUFFER
    {
    int			tasks_left;
    int			concurrent_limit;
    dthread_mutex_t	crew_lock;
    int			tasks_running;
    dthread_cond_t	done;
    } crew_context_t, crew_context;

typedef struct ring_element
    {
    crew_context_t	*crew_context_p;
    struct db_stream	*stream_p;
    Timestamp_t 	*all_p;
    ObjUID_t    	*epoch_p;
    Timestamp_t 	*sk_p;
    int			ring_status;
    ObjUID_u    	ring_from_uid;
    int			ring_hint_len;
    ReplicaPointer_t 	*ring_hint_p;
    ReplicaPointer_t 	*ring_actual_p;
    } ring_element_t;

typedef struct ring_list
    {
    int			ring_count;
    /* followed by an array of ring_element_t */
    ring_element_t	ring_first[1];
    } ring_list_t;

typedef struct spread_element
    {
    crew_context_t	*crew_context_p;
    struct copy_context	*context_p;
    int			spread_status;
    char		spread_replica_state;
    int			spread_update_count;
    update_buffer_t	*update_buf_p;
    int			spread_hint_len;
    ReplicaPointer_t	*spread_hint_p;
    } spread_element_t;
#define NULL_spread_element ((spread_element_t *)0)

#define deb_ascii_spread_element(b,p) deb_ascii_rec(b,p,"NULL_spread_element")
#define DEB_ASCII_LEN_spread_element DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_spread_element(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_spread_element(b) LOG_ASCII_BUF_rec(b)

typedef struct spread_list
    {
    int			spread_count;
    /* followed by an array of spread_element_t */
    spread_element_t	spread_first[1];
    } spread_list_t;

/*                                                        
 * structure to keep copy context in between network calls
 */
#define MAX_SPREADS 5
typedef struct copy_context
    {
    int			 ct_buffers_sent;
    int			 ct_uts_sent;
    Timestamp_u 	 ct_begin_uid;
    Timestamp_u 	 ct_uts;
    struct db_stream	*ct_dir_p;
    ObjUID_t    	*ct_epoch_p;
    ReplicaPointer_t 	*ct_replica_p;
    int			 ct_spread_count;
    spread_element_t	*ct_spread_list[MAX_SPREADS];
    char                 ct_rv_buf[READ_BUF_SIZE];  /* ReplicaVersion buffer */
    } copy_context_t;

#define deb_ascii_copy_context(b,p) deb_ascii_rec(b,p,"NULL_copy_context")
#define DEB_ASCII_LEN_copy_context DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_copy_context(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_copy_context(b) LOG_ASCII_BUF_rec(b)


/*
** FUNCTION PROTOTYPES
*/

/*
 * back_collect.c
 */

void
cleanup_ring_list (
    ring_list_t		*);

dns_status_t
collect (
    struct db_stream	*,
    Timestamp_t		*);

/*
 * back_combine.c
 */

dns_status_t
combine (
    user_descriptor_t	*,
    ObjUID_t    	*,
    Timestamp_t 	*,
    Timestamp_t 	*,
    ObjUID_t    	*,
    FullName_t  	*,
    ObjUID_t    	*);

/*
 * back_copy_updates.c
 */

dns_status_t
send_dir (
    update_buffer_t	*,
    copy_context_t	*);

dns_status_t
copy_updates (
    struct db_stream	*,
    Timestamp_t 	*,
    Timestamp_t 	*,
    update_buffer_t	*,
    copy_context_t	*);

/*
 * back_do_updates.c
 */
dns_status_t
do_updates (
    user_descriptor_t	*,
    ObjUID_t		*,
    FullName_t		*,
    UpdatePacket_t	*,
    int			);

/*
 * back_gather_updates.c
 */

dns_status_t
gather_updates (
    struct db_stream	*,
    struct db_data	*,
    Timestamp_t		*,
    Timestamp_t		*,
    byte_t		**,
    byte_t		**,
    update_buffer_t	*,
    copy_context_t	*);

/*
 * back_ground.c
 */

dns_status_t
background_start (
    struct file_info	*);

dns_status_t
background_stop (
    struct file_info	*);

void
background_kill (
    struct file_info	*);

/*
 * back_parentpointer.c
 */


dns_status_t
back_parentpointer_check (
    struct db_stream	*,
    utc_t               *);


/*
 * back_propagate.c
 */

void
back_propagate_sched (
    struct db_stream	*,
    int			,
    Timestamp_t		*);

dns_status_t
back_propagate_start (
    struct file_info	*);

dns_status_t
back_propagate_stop (
    struct file_info	*);

void
back_propagate_kill (
    struct file_info	*);

/*
 * back_replicas.c
 */

int
check_own_replica_ptr (
    struct db_stream	*,
    Set_t		*,
    Update_t		*,
    int			*);

void
fix_replica_ptr (
    struct db_stream	*,
    ObjUID_t		*,
    Set_t		*);

/*
 * back_skulk.c
 */

dns_status_t
skulk_sched (
    struct db_stream	*);

dns_status_t
skulk_dir (
    struct db_stream	*);

void
skulk_log (
    dir_hash_entry_t	*,
    int		        ,
    int	        	,
    char                *,
    dns_status_t	);    

dns_status_t
back_pseudodir_updates (
    db_stream_t		*,
    file_info_t		*,
    DBSet_t		*);

/*
 * back_spread.c
 */

dns_status_t
spread (
    struct db_stream	*,
    Timestamp_t 	*,
    int			*,
    VersionNumber_t     *);

/*
 * back_upgrade.c
 */

void
back_upgrade_replica_actions (
    db_stream_t		*);

void
back_upgrade_directory_actions (
    db_stream_t		*,
    VersionNumber_t	*,
    int                   );

#endif  /* #ifndef _BACK_H */
