/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * Common definitions, includes, etc.
 * ______________________________________________________________________
 *
 * $Log: mosdefs.h,v $
 * Revision 1.6  1996/11/11  21:57:32  bowe
 * Add move_items. Rename reply_to_message to enqueue_ack; add param.
 * Add maintain_login_context, stop_login_context.
 *
 * Revision 1.5  1996/10/31  20:07:25  bowe
 * Add item_matches_selmask()
 *
 * Revision 1.4  1996/10/17  15:13:25  bowe
 * Added start_time, create_count, enqueue_count, dequeue_count.
 * Added value_from_config_file().
 * Added ENV_OBJECT.
 *
 * Revision 1.3  1996/10/14  17:53:39  bowe
 * Additional param to acl_for_new_queue().
 *
 * Revision 1.2  1996/10/01  14:17:35  bowe
 * Add "flags" field to Queue, QItem structures.
 *
 * Revision 1.1  1996/09/27  18:03:14  bowe
 * Initial revision
 *
 * $EndLog$
 * ______________________________________________________________________
 */

#ifndef mosdefs_h_included
#define mosdefs_h_included

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/binding.h>
#include <dce/dce_msg.h>
#include <dce/dce_svc.h>
#include <dce/sec_login.h>
#include <dce/dbif.h>
#include <dce/assert.h>
#include <dce/utc.h>
#include <dce/id_epac.h>

#include "lock.h"

#include "mos_q.h"
#include "mos_mgmt.h"
#include "mos_convert.h"

#include "dcemosmsg.h"
#include "dcemosmac.h"
#include "dcemossvc.h"

/* ______________________________________________________________________
 *
 *		symbols
 */

#define DEBUG	1

/* private data files */
#define acldbname	"MOS-acl.db"
#define qdatadbname	"MOS-data.db"
#define qattrdbname	"MOS-attr.db"
#define private_data	"MOS.data"

#define ENV_KEYFILE	"QMGR_KEYFILE"
#define ENV_PRINCIPAL	"QMGR_PRINCIPAL"
#define ENV_NS_ENTRY	"QMGR_NS_ENTRY"
#define ENV_OBJECT	"QMGR_OBJECT"

/* keys (as in key-valye pairs) for private_data file */
#define PRIV_KEY_OBJECT	"ObjectUUID"

/* ______________________________________________________________________ */

/* For convenience */

/* allocate locally of for return to the client */
#define NEW(T,c)		((T *)malloc(sizeof(T) * (c)))
#define NEW_client(T,c,s)	((T *)rpc_sm_allocate(sizeof(T) * (c), s))

/* ______________________________________________________________________
 *
 *		internal data structures
 */

/* internal infrastructure: linked list of a items in a queue */
typedef struct QItem_s {
    mos_qitem_t		item;		/* attr and actual message */
    unsigned32		flags;
    struct QItem_s	*next;
} QItem;

/* internal infrastructure: linked list of queues */
typedef struct Queue_s {
    mos_qattr_t		qattr;
    QItem		*qi;		/* linked list of items in queue */
    mos_lock_t		lock;
    pthread_cond_t	deq_cond;	/* for blocking dequeue */
    pthread_cond_t	deq_mutex;	/* for blocking dequeue */
    boolean		blocked;	/* someone is blocked */
    unsigned32		flags;
    struct Queue_s	*next;
} Queue;

/* ______________________________________________________________________
 *
 *		external references
 */

/* Object and mgmt type and ACL UUIDs */
extern uuid_t mgmt_type, mgmt_acl;
extern uuid_t q_iobj_type, q_iobj_acl;
extern uuid_t q_obj_type, q_obj_acl;

extern uuid_t		qmgr_cell_uuid;
extern uuid_t		qmgr_prin_uuid;
extern uuid_t		qmgr_object_uuid;
extern char		*qmgr_cell;
extern idl_char		*qmgr_prin;
extern char		*qmgr_object;
extern unsigned_char_t  *qmgr_keyfile;
extern unsigned_char_t  *ns_entry;

/* Some stats maintained */
extern utc_t		start_time;
extern unsigned32	create_count, enqueue_count, dequeue_count;

extern dce_db_handle_t qattr_db, qdata_db, acl_db;

extern mos_qattr_t null_qattr;
extern mos_mattr_t null_mattr;
extern mos_datatype_bytearray_t null_body;

extern sec_login_handle_t	login_ctx;

extern pthread_mutex_t	qlist_lock;
extern Queue		*the_queues, *last_queue;

/* ______________________________________________________________________
 *
 *	Function prototypes
 */

	/* ---------- utils.c ---------- */

char *		value_from_config_file( char *, char * );
void		dce_login( boolean );
void		register_server( void );
void		unregister_server( void );
void 		open_databases( boolean );
void		close_databases( void );
void		copy_string_list_for_client( mos_string_list_t *,
		mos_string_list_t * );
void		copy_string_list_local( mos_string_list_t *,
		mos_string_list_t * );
void		free_string_list_local( mos_string_list_t * );
idl_char *	strdup_for_client( idl_char *, error_status_t * );
void *		memdup_for_client( void *, unsigned32, error_status_t * );
void *		memdup( void *, unsigned32 );
void *		maint_thread(void *);
void *		kmgmt_thread(void *);

	/* ---------- q_utils.c ---------- */

Queue *		cache_queue_info( idl_char *, mos_qattr_t *, error_status_t * );
void		delete_queue( idl_char *, error_status_t * );
Queue *		find_queue_by_name( idl_char *, error_status_t * );
Queue *		find_queue_by_id( uuid_t *, error_status_t * );
void		move_items( Queue *, Queue * );
void		read_queues( error_status_t * );
void		enqueue_item( Queue *, mos_mattr_t *,
		    mos_datatype_bytearray_t *, boolean );
void		copy_qattr_for_client( mos_qattr_t *, mos_qattr_t * );
void		copy_qattr_local( mos_qattr_t *, mos_qattr_t * );
void		copy_mattr_for_client( mos_mattr_t *, mos_mattr_t * );
void		copy_mattr_local( mos_mattr_t *, mos_mattr_t * );
void		free_qitem(QItem *, error_status_t *);
boolean		item_matches_selmask( mos_mask_list_t *, QItem * );
void		find_item_by_selmask( mos_mask_list_t *, Queue *,
		    QItem **, QItem **, error_status_t * );
void		enqueue_ack( idl_char *, mos_mattr_t *, unsigned32,
		    error_status_t *);

	/* ---------- acl.c ---------- */

void		mos_register_acls( void );
boolean		mos_authorized( handle_t, uuid_t *, uuid_t *,
		    sec_acl_permset_t, error_status_t * );
void		initialize_acls( void );
void		acl_for_new_queue( handle_t, uuid_t *, uuid_t *,
		    error_status_t * );
void		find_initiator( handle_t, sec_id_pa_t *, error_status_t * );

	/* ---------- dcelogin.c ---------- */
void		maintain_login_context( char *, error_status_t * );
void		stop_login_context( void );
/* ______________________________________________________________________ */

#endif
