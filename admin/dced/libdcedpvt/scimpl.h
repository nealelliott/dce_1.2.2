/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: scimpl.h,v $
 * Revision 1.1.2.2  1996/02/18  23:32:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:27  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:34  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:38 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 	[1995/12/08  20:49:03  root]
 * 
 * Revision 1.1.2.8  1994/09/21  21:49:43  baum
 * 	Reverse bit set on hash index calculation
 * 	[1994/09/21  21:47:11  baum]
 * 
 * Revision 1.1.2.7  1994/05/26  19:42:28  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:59  rsalz]
 * 
 * 	Checkpointing.
 * 	[1994/05/18  19:33:03  rsalz]
 * 
 * Revision 1.1.2.6  1994/05/12  19:30:34  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:33  rsalz]
 * 
 * Revision 1.1.2.5  1994/05/11  20:11:24  rsalz
 * 	More functionality; code cleanups.
 * 	[1994/05/11  20:08:26  rsalz]
 * 
 * Revision 1.1.2.4  1994/05/10  20:15:16  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:46  rsalz]
 * 
 * Revision 1.1.2.3  1994/05/05  20:07:44  rsalz
 * 	Initial server functionality drop.
 * 	[1994/05/05  20:01:05  rsalz]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:08  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:56:04  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:58  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:27  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#ifndef _SCIMPL_H
#define _SCIMPL_H

/*
**  All data on all servers is kept in memory.  This can be inefficient,
**  for example if all configured servers are running, so the srvrconf
**  service is written so that it should not be too difficult to keep its
**  data in backing storage only.
**
**  The servers are stored in the list field, and the count field says
**  how big the array is.  Some elements in the list can be marked as
**  unused; later on we coalesce and reclaim space.  The readers field
**  keeps track of clients walking through the old endpoint database.
*/
typedef struct srvr_table_s_t {
    dce_lock_t			lock;	/* Must be first! */
    boolean32			setup;
    unsigned16			readers;
    unsigned16			count;
    server_data_t		*list;
    struct srvr_hash_table_s_t	*hashtable;
} srvr_table_t;


/*
**  The bsic goal of endpoint resolution is to map an <interface id,UUID>
**  pair to a server process (well, an endpoint, but it's the same thing.)
**  The rules for doing this are specified in the AES.  They are
**  complicated and while you may understand them all, you probably won't
**  agree with them all.  We hash the interface UUID and/or object UUID
**  to get the right bucket, and then linear search until we get the
**  right interface version and a compatible protocol tower.
*/


/*
**  A hash entry points into the server table to find the server, and then
**  the right service within that server and which object UUID within that
**  service.  For interfaces UUID's the service has the info we need and the
**  object will be 0; for object UUID's -- which we only examine to avoid
**  false matches -- subject one from the object field.
*/
typedef struct srvr_hash_entry_s_t {
    unsigned16		entry;
    unsigned16		service;
    unsigned32		object;
} srvr_hash_entry_t;


/*
**  A hash bucket is just a list of entries.  There is no secondary key.
*/
typedef struct srvr_hash_bucket_s_t {
    unsigned32		size;
    unsigned32		count;
    srvr_hash_entry_t	*list;
} srvr_hash_bucket_t;


/*
**  A hash table is a list of buckets and a lock.  This could be part
**  of the srvr_table_t, but see the discussion above about wanting
**  to be able to move that off-line.
*/
#define srvr_c_hash_table_size		64
#define srvr_hash2bucket(x)		((x) & 63)
#define srvr_c_hash_bucket_init		16
#define srvr_c_hash_bucket_delta	32

typedef struct srvr_hash_table_s_t {
    dce_lock_t		lock;	/* Must be first! */
    srvr_hash_bucket_t	list[srvr_c_hash_table_size];
} srvr_hash_table_t;

extern srvr_table_t		sc_table;
extern srvr_table_t		sx_table;
extern srvr_hash_table_t	sc_hash_table;
extern srvr_hash_table_t	sx_hash_table;

void srvr_get_list(srvr_table_t*, dced_entry_list_t*, error_status_t*);
void srvr_hash_build(srvr_table_t*, srvr_hash_table_t*, error_status_t*);
server_data_t *srvr_find_by_uuid(srvr_table_t*, uuid_t*, error_status_t*);
server_data_t *srvr_find_by_name(srvr_table_t*, idl_char*, error_status_t*);
void srvr_add_to_table(srvr_table_t*, server_data_t*, error_status_t*);
#endif
