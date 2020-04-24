/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_replica.c,v $
 * Revision 1.1.65.3  1996/02/18  00:21:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:40  marty]
 *
 * Revision 1.1.65.2  1995/12/08  18:00:28  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:11 UTC  greg
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	prev_prop_type doesn't need to be a pointer in
 * 	rsdb_replica_set_prop_info().
 * 	[1995/01/29  14:32 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:50 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Add prev_prop_type to stable replica info.
 * 	Still no support for database conversion
 * 	at replica startup.
 * 	[1995/01/22  20:59 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.61.1  1993/10/07  13:12:35  mccann
 * 	CR8651 64bit porting changes
 * 	[1993/10/07  13:11:32  mccann]
 * 
 * Revision 1.1.3.4  1993/02/25  16:46:33  ahop
 * 	7184: add rsdb_replica_get_name_by_uuid
 * 	[1993/02/25  03:17:26  ahop]
 * 
 * Revision 1.1.3.3  1992/12/29  16:39:29  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:29:05  zeliff]
 * 
 * Revision 1.1.3.2  1992/08/31  18:28:51  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:16:14  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1993
 * Unpublished work. All Rights Reserved.
 *
 */
/*
 *      Registry Server Replication - primitives
 *
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rsdb_replica.c,v 1.1.65.3 1996/02/18 00:21:55 marty Exp $";
#endif

#include <dce/nbase.h>
#include <dce/uuid.h>
#include <dce/rplbase.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_rep_pvt.h>
#include <rs_rep_util.h>

#include <rsdb_pvt.h>
#include <rsdb_replica.h>

/*
 * Local data types
 */

/*                                         
 *  Full rsdb record about a replica
 */
typedef struct {   
    rs_replica_mgt_item_t       replica; 

    /* 
     *  name, tower_vector, tower buffers 
     */
    unsigned32                  rep_buffers_len;
    rs_replica_packed_buff_t    rep_buffers;    
} rsdb_replica_item_t, *rsdb_replica_item_p_t;

/*
 *  lookup and copy, lookup by reference
 */
typedef enum {
    lookup_e_copy,
    lookup_e_by_ref
} rsdb_replica_lookup_t;


/*
 * Local routine prototypes
 */
static void pack_replica (
    rs_replica_mgt_item_p_t rp,
    rsdb_replica_item_p_t   *odb_rp,
    error_status_t          *st
);

static void lookup_replica (
    uuid_t                  *cursor,
    rsdb_replica_item_p_t   *odb_rp,
    rsdb_replica_lookup_t   lookup_type,
    error_status_t          *st
);



/*  
 *  Add a replica to the database.
 */
void rsdb_replica_add 
  (
    rs_replica_mgt_item_p_t rp,
    error_status_t          *st
  )
{
    rsdb_replica_item_p_t   db_rp;
    unsigned32              prefix_len;
    error_status_t          lst;

    /* allocate contiguous memory and pack replica
     * info into it
     */
    pack_replica(rp, &db_rp, st);
    if (BAD_STATUS(st)) return;

    prefix_len = ((char *) &db_rp->rep_buffers) - ((char *) (db_rp));

    rsdb_store(rsdb_replica_db, 
        (Pointer) &db_rp->replica.rep_id, 
        sizeof(db_rp->replica.rep_id),
        (Pointer) db_rp, prefix_len + db_rp->rep_buffers_len, st);

    rsdb_replica_item_free((rs_replica_mgt_item_p_t *) &db_rp, &lst);

    if (BAD_STATUS(st)) return;
}

/*
 *  Replace information about a replica
 */
void rsdb_replica_replace 
  (
    rs_replica_mgt_item_p_t rp,
    error_status_t          *st
  )
{
    rsdb_replica_item_p_t   db_rp;
    unsigned32              prefix_len;
    error_status_t          lst;

    /* allocate contiguous memory and pack replica
     * info into it
     */
    pack_replica(rp, &db_rp, st);
    if (BAD_STATUS(st)) return;

    prefix_len = ((char *) &db_rp->rep_buffers) - ((char *) (db_rp));

    rsdb_replace(rsdb_replica_db, 
        (Pointer) &db_rp->replica.rep_id, sizeof(db_rp->replica.rep_id),
        (Pointer) db_rp, prefix_len + db_rp->rep_buffers_len, st);

    rsdb_replica_item_free((rs_replica_mgt_item_p_t *) &db_rp, &lst);

    if (BAD_STATUS(st)) return;
}

static void pack_replica
  (
    rs_replica_mgt_item_p_t rp,
    rsdb_replica_item_p_t   *odb_rp,
    error_status_t          *st
  )
{
    rsdb_replica_item_t         db_rep_item;
    rsdb_replica_item_t         *db_rp;
    unsigned32                  prefix_len;
    unsigned32                  rep_bufflen;
    rs_replica_packed_buff_p_t  rep_buff;

    prefix_len = ((char *) (&db_rep_item.rep_buffers)) -
                    ((char *) (&db_rep_item));
    rs_rep_util_rep_buff_pack(rp->rep_name, rp->rep_twrs, prefix_len,
        (void *) &db_rp, &rep_buff, &rep_bufflen, st);
    if (BAD_STATUS(st)) return;

    db_rp->replica = *rp;
    db_rp->replica.alloc_type = rs_e_rep_mgt_item_alloc_rsdb;

    db_rp->rep_buffers_len = rep_bufflen;

    *odb_rp = db_rp;
}

/*
 *  Delete a replica from the database
 *
 */
void rsdb_replica_delete 
  (
    uuid_p_t                rep_id,
    error_status_t          *st
  )
{
    rsdb_delete(rsdb_replica_db, (Pointer) rep_id, sizeof(*rep_id), st);
    if (BAD_STATUS(st)) return;
}

void rsdb_replica_set_prop_info 
  (
    uuid_p_t                rep_id,
    rs_replica_prop_t       prop_type,
    rs_replica_prop_t       prev_prop_type,
    uuid_p_t                init_id,
    error_status_t          *st
  )
{
    uuid_t                  cursor;
    rsdb_replica_item_p_t   db_rp;
    unsigned32              prefix_len;
    error_status_t          lst;

    /* get a pointer to a copy of this replica's
     * data and bump the cursor
     */
    cursor = *rep_id;
    lookup_replica(&cursor, &db_rp, lookup_e_copy, st);
    if (BAD_STATUS(st)) return;

    db_rp->replica.prop_type = prop_type;
    db_rp->replica.prev_prop_type = prev_prop_type;
    if (init_id) {
	db_rp->replica.init_id = *init_id;
    }
    
    prefix_len = ((char *) &db_rp->rep_buffers) - ((char *) (db_rp));

    rsdb_replace(rsdb_replica_db, 
        (Pointer) &db_rp->replica.rep_id, sizeof(db_rp->replica.rep_id),
        (Pointer) db_rp, prefix_len + db_rp->rep_buffers_len, st);

    rsdb_replica_item_free((rs_replica_mgt_item_p_t *) &db_rp, &lst);

    if (BAD_STATUS(st)) return;
}

void rsdb_replica_get_prop_info 
  (
    uuid_p_t                rep_id,
    rs_replica_prop_t       *prop_type,
    rs_replica_prop_t       *prev_prop_type,
    uuid_t                  *init_id,
    error_status_t          *st
  )
{
    uuid_t                  cursor;
    rsdb_replica_item_p_t   db_rp;

    /* get a pointer this replica's data
     * and bump the cursor
     */
    cursor = *rep_id;
    lookup_replica(&cursor, &db_rp, lookup_e_by_ref, st);
    if (BAD_STATUS(st)) return;

    *prop_type = db_rp->replica.prop_type;
    *prev_prop_type = db_rp->replica.prop_type;
    *init_id = db_rp->replica.init_id;
}

void rsdb_replica_set_prop_mgt_handle 
  (
    uuid_p_t                rep_id,
    rs_rep_prop_mgt_handle_t    
                            prop_mgt_h,
    error_status_t          *st
  )
{
    uuid_t                  cursor;
    rsdb_replica_item_p_t   db_rp;
    unsigned32              prefix_len;
    error_status_t          lst;

    /* get a pointer to a copy of this replica's
     * data and bump the cursor
     */
    cursor = *rep_id;
    lookup_replica(&cursor, &db_rp, lookup_e_copy, st);
    if (BAD_STATUS(st)) return;

    db_rp->replica.prop_mgt = prop_mgt_h;

    prefix_len = ((char *) &db_rp->rep_buffers) - ((char *) (db_rp));

    rsdb_replace(rsdb_replica_db, 
        (Pointer) &db_rp->replica.rep_id, sizeof(db_rp->replica.rep_id),
        (Pointer) db_rp, prefix_len + db_rp->rep_buffers_len, st);

    rsdb_replica_item_free((rs_replica_mgt_item_p_t *) &db_rp, &lst);

    if (BAD_STATUS(st)) return;
}

void rsdb_replica_get_prop_mgt_handle 
  (
    uuid_p_t                rep_id,
    rs_rep_prop_mgt_handle_t    
                            *prop_mgt_h,
    error_status_t          *st
  )
{
    uuid_t                  cursor;
    rsdb_replica_item_p_t   db_rp;

    /* get a pointer this replica's data
     * and bump the cursor
     */
    cursor = *rep_id;
    lookup_replica(&cursor, &db_rp, lookup_e_by_ref, st);
    if (BAD_STATUS(st)) return;

    *prop_mgt_h = db_rp->replica.prop_mgt;
}

/*
 *  return a pointer to a copy of 
 *  information about the replica
 *  identified by "cursor".  Set "cursor"
 *  to be the uuid of the next replica on
 *  the replist.
 *
 *  If cursor is NULL, return a
 *  pointer to the information
 *  about the first replica on
 *  the replist.
 *
 *  Use rsdb_replica_item_free to
 *  free space allocated for the
 *  returned replica information. 
 */
void rsdb_replica_lookup 
  (
    uuid_t                  *cursor,    /* [in, out] */
    rs_replica_mgt_item_t   **rp,       /* [out] */
    error_status_t          *st         /* [out] */
  )
{
    rsdb_replica_item_p_t   db_rp;

    *rp = NULL;

    /* get a pointer to a copy of this replica's
     * data and bump the cursor
     */
    lookup_replica(cursor, &db_rp, lookup_e_copy, st);
    if (BAD_STATUS(st)) return;

    *rp = (rs_replica_mgt_item_p_t) &db_rp->replica;
}

/* 
 *  return a pointer to information 
 *  about the replica identified by 
 *  "cursor".  Set "cursor" to be the 
 *  uuid of the next replica on the 
 *  replist.
 *
 *  If cursor is NULL, return a
 *  pointer to the first replica's
 *  information.
 */
void rsdb_replica_lookup_by_ref 
  (
    uuid_t                      *cursor,        /* [in, out] */
    rs_replica_mgt_item_t       **rp,           /* [out] */
    rs_replica_packed_buff_t    **rep_buff,     /* [out] */
    unsigned32                  *rep_buff_len,  /* [out] */
    error_status_t              *st             /* [out] */
  )
{
    rsdb_replica_item_p_t   db_rp;

    *rp = NULL;
    *rep_buff = NULL;

    /* get a pointer to this replica's data
     * and bump the cursor
     */
    lookup_replica(cursor, &db_rp, lookup_e_by_ref, st);
    if (BAD_STATUS(st)) return;

    *rep_buff = (rs_replica_packed_buff_p_t) &db_rp->rep_buffers;
    *rep_buff_len = db_rp->rep_buffers_len;

    *rp = (rs_replica_mgt_item_p_t) &db_rp->replica;
}

/*
 *  free replica info space that was  
 *  allocated by rsdb_replica module.
 */
void rsdb_replica_item_free 
  (
    rs_replica_mgt_item_t   **rp,
    error_status_t          *st
  )
{
    if ((*rp)->alloc_type != rs_e_rep_mgt_item_alloc_rsdb) {
        SET_STATUS(st, sec_rgy_rep_pgmerr);
        return;
    }

    rs_rep_util_rep_buff_free((void **) rp, st);
}

/*
 *  Return information about the replica identified by 
 *  cursor.
 *  If cursor is uuid_nil, return info about the first
 *  replica on the list.
 *  Return the uuid of the next replica on the list in
 *  cursor; if there are no more replicas after the
 *  one just read, return uuid_nil in cursor.
 */  
static void lookup_replica
  (
    uuid_t                  *cursor,
    rsdb_replica_item_p_t   *odb_rp,
    rsdb_replica_lookup_t   lookup_type,
    error_status_t          *st
  )
{
    rsdb_replica_item_p_t   db_rp;
    unsigned_char_t         *db_data;
    int                     dlen;
    uuid_p_t                next_id;
    int                     next_id_len;
    unsigned_char_t         *tmp_data;
    int                     tmp_dlen;
    error_status_t          lst;

    *odb_rp = NULL;

    if (uuid_is_nil(cursor, &lst)) {
        rsdb_fetch_next(rsdb_replica_db, (Pointer) cursor, sizeof(*cursor),            
            (Pointer *) &next_id, &next_id_len,
            (Pointer *) &db_data, &dlen, st);
        if (BAD_STATUS(st)) return;

        memcpy((void *) cursor, (void *) next_id, sizeof(*cursor));
    }
    else {
        rsdb_fetch(rsdb_replica_db, (Pointer) cursor, sizeof(*cursor),
            (Pointer *) &db_data, &dlen, st);
        if (BAD_STATUS(st)) return;
    }

    /* point the cursor to the next entry on
     * the replist.  If the last entry was just
     * read, set cursor to uuid_nil
     */
    rsdb_fetch_next(rsdb_replica_db, (Pointer) cursor, sizeof(*cursor),
        (Pointer *) &next_id, &next_id_len,
        (Pointer *) &tmp_data, &tmp_dlen, st);
    if (BAD_STATUS(st) &&
        (! STATUS_EQUAL(st, sec_rgy_object_not_found)) ) {
            return;
    }

    if (STATUS_EQUAL(st, sec_rgy_object_not_found)) {
        uuid_create_nil(cursor, &lst);
        CLEAR_STATUS(st);
    }
    else {
        memcpy((void *) cursor, (void *) next_id, sizeof(*cursor));
    }

    if (lookup_type == lookup_e_copy) {
        db_rp = (rsdb_replica_item_p_t) malloc(dlen);
        memcpy((void *) db_rp, (void *) db_data, dlen);
    }
    else {
        db_rp = (rsdb_replica_item_p_t) db_data;
    }

    rs_rep_util_rep_buff_unpack(&db_rp->rep_buffers, &db_rp->replica.rep_name,
        &db_rp->replica.rep_twrs, st);
    if (BAD_STATUS(st)) return;

    *odb_rp = db_rp;
}


/* 
 *  r s d b _ r e p l i c a _ g e t _ n a m e _ b y _ u u i d
 *
 *  Use the input rep_id to retrieve a replica item.
 *  Allocate a buffer for rep_name and copy the replica name
 *  into it.
 *  Caller must free *rep_name.
 */
void rsdb_replica_get_name_by_uuid
  (
    uuid_t                  *rep_id,
    rs_replica_name_p_t     *rep_name,
    error_status_t          *st
  )
{
    uuid_t                  cursor;
    rsdb_replica_item_p_t   db_rp;

    CLEAR_STATUS(st);
    *rep_name = 0;

    /* get a pointer this replica's data
     */
    cursor = *rep_id;
    lookup_replica(&cursor, &db_rp, lookup_e_by_ref, st);
    if (BAD_STATUS(st)) return;

    if(!(*rep_name = (rs_replica_name_p_t )malloc(u_strlen(db_rp->replica.rep_name) + 1))) {
        SET_STATUS(st, sec_rgy_cant_allocate_memory);
        return;
    }
    u_strcpy(*rep_name, db_rp->replica.rep_name);
}
