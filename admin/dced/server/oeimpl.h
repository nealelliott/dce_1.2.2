/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: oeimpl.h,v $
 * Revision 1.1.8.3  1996/02/18  23:32:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:38  marty]
 *
 * Revision 1.1.8.2  1995/12/08  20:53:58  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  22:39 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/27  14:57 UTC  tatsu_s
 * 	Submitted the stale and lost endpoints fix.
 * 
 * 	HP revision /main/tatsu_s.ep_mapper.b0/1  1995/02/17  16:06 UTC  tatsu_s
 * 	Better stale & lost ep detection.
 * 	[1995/12/08  20:49:14  root]
 * 
 * Revision 1.1.4.1  1994/10/26  19:54:42  rsalz
 * 	Add EPmap convenience abstractions (needed to fix OT CR 12061).
 * 	[1994/10/26  19:43:26  rsalz]
 * 
 * Revision 1.1.2.7  1994/09/17  08:05:16  rsalz
 * 	Rename epdb_sleep to dced_sleep.
 * 	[1994/09/17  07:57:34  rsalz]
 * 
 * Revision 1.1.2.6  1994/08/11  17:53:40  bowe
 * 	Added db_entry_c_local. [CR 11567]
 * 	[1994/08/11  17:47:16  bowe]
 * 
 * Revision 1.1.2.5  1994/08/03  20:36:19  rsalz
 * 	Fix OT CR 11506.
 * 	[1994/08/03  20:32:34  rsalz]
 * 
 * Revision 1.1.2.3  1994/06/13  20:05:35  rsalz
 * 	Move epdb_get_the_time to dced_get_the_time.
 * 	Include dcedimpl.h
 * 	[1994/06/13  20:05:14  rsalz]
 * 
 * Revision 1.1.2.2  1994/06/09  18:15:35  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:18  annie]
 * 
 * Revision 1.1.2.1  1994/05/26  19:42:24  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:55  rsalz]
 * 
 * $EndLog$
 */

/*
**  Implementation data for old EP support.
*/
#include <commonp.h>
#include <com.h>
#include <dce/ep.h>
#include <dce/dce.h>
#include <dced_base.h>
#define db_rpc_addr_p_t		rpc_addr_p_t
#include <dced_convert.h>
#define NO_THREADS_HEADER
#include <dcedimpl.h>


/*
**  Maximum number of reads in db_entry_t.readers, with allowance for the
**  liveness tasks.
*/
#define db_c_max_readers		0xFFF0

/*
**  Map a hashed uuid into a hashtable bucket.
*/
#define db_c_nbucket			64
#define db_hash2bucket(bucket)		((bucket) & 63)



/*
**  Do a long)wait until we find a bad server, then a short_wait while
**  we see if it's alive.
*/
#define ep_live_c_long_wait		(6 * 60)
#define ep_live_c_long_wait_space	(3 * 60)
#define ep_live_c_short_wait		(1 * 60)

/*
**  How long to wait for a ping response from a live server.
*/
#define ep_live_c_short_comm_timeout	3

/*
**  After this many failures, call the endpoint dead and delete it.
*/
#define ep_live_c_max_comm_failures	20

/*
**  Relinquish database after this many deletes, to give others a chance.
*/
#define ep_live_c_max_deletes		5



/*
**  Every endpoint entry is on three lists: the entry list of all
**  entries, a list keyed by the entry's object uuid, and a list keyed
**  by the entry's interface uuid.  The object list and interface list
**  are accessed via a hash table which marks the beginning of a list
**  for all uuids that hash to the same value.  Separate hash tables
**  are used for the object lists and the interface lists.
**
**  A database's lists are accessed via its db_entry_t db.lists_mgmt
**  field.  The lists contain forward and back pointers with the last
**  entry on a list having its fwd ptr = NULL.
**
**  The entry lists are not stably stored:  they are recreated at
**  startup time.
*/

typedef enum dblist_type_e_t {
    db_c_entry_list, db_c_object_list, db_c_interface_list
} dblist_type_t;

typedef dblist_t db_hash_table_t[db_c_nbucket];

typedef struct dp_lists_mgmt_s_t {
    dblist_t			entries;
    db_hash_table_t		objects;
    db_hash_table_t		interfaces;
} db_lists_mgmt_t;

#define db_entry_c_deleted	0x01
#define db_c_nil_object		0x02
#define db_c_nil_ifspec		0x04
#define db_entry_c_local	0x08

/*
 * An ep database handle.
 */
typedef struct epdb_handle_s_t {
    pthread_mutex_t		lock;
    dce_db_handle_t		db;
    uuid_t			object;
    db_lists_mgmt_t		lists_mgmt;
	/* Liveness data. */
    pthread_t			sliv_task1;
    pthread_t			sliv_task2;
    pthread_cond_t		cv;
} *epdb_handle_t;



/*
**  Upper-level databaes functions.
*/
void epdb_lock(epdb_handle_t);
void epdb_unlock(epdb_handle_t);
void epdb_delete_entry(epdb_handle_t, db_entry_t*, error_status_t*);
void epdb_write_entry(epdb_handle_t, db_entry_t*, error_status_t*);
void epdb_database_read(epdb_handle_t, char*, error_status_t*);

/*
**  Lower-level entry list functions.
*/
void dblist_init(epdb_handle_t);
void dblist_add(db_lists_mgmt_t*, db_entry_t*);
void dblist_remove(db_lists_mgmt_t*, db_entry_t*);
db_entry_t*dblist_first(db_lists_mgmt_t*, dblist_type_t, uuid_t*);
db_entry_t*dblist_next(db_entry_t*, dblist_type_t);

/*
**  Entry list search context functions.
*/
boolean32 dblist_ctx_different(epdb_handle_t, ept_lookup_handle_t*, error_status_t*);
void dblist_ctx_delete(epdb_handle_t, ept_lookup_handle_t*);
void dblist_ctx_restore(epdb_handle_t, ept_lookup_handle_t*, dblist_type_t*, db_entry_t**, unsigned32*, error_status_t*);
void dblist_ctx_save(epdb_handle_t, ept_lookup_handle_t*, dblist_type_t, db_entry_t*, unsigned32);
