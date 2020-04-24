/* ______________________________________________________________________ */
/*
 *  @OSF_COPYRIGHT@
 *  COPYRIGHT NOTICE
 * Copyright (c) 1993, 1996 Open Software Foundation, Inc.
 *  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 *  src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dbif.h,v $
 * Revision 1.1.4.2  1996/02/18  23:32:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:27  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:35  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/09/29  20:40 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/23  00:16 UTC  tatsu_s
 * 	Added db_c_lock_nowait.
 * 
 * 	HP revision /main/HPDCE02/4  1995/08/22  21:08 UTC  mullan_s
 * 	Merge fix for CHFts15637
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/1  1995/08/09  14:26 UTC  mullan_s
 * 	Add <sys/stat.h> for struct stat definition.
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/15  19:00 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat8/1  1995/05/15  18:34 UTC  mullan_s
 * 	Add db_c_lock flag.
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/08  21:10 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat4/1  1995/05/08  19:36 UTC  mullan_s
 * 	Add dce_db_stat.
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/03  21:52 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat/1  1995/01/11  18:13 UTC  mullan_s
 * 	Add dce_db_open_file.
 * 	[1995/12/08  18:07:26  root]
 * 
 * Revision 1.1.2.6  1994/05/11  15:33:05  bowe
 * 	Correct args/cleanup dce_db_std_header_init() [CR 10603]
 * 	[1994/05/11  15:32:52  bowe]
 * 
 * 	Correct args to dce_db_std_header_init() [CR 10603]
 * 	[1994/05/11  15:27:02  bowe]
 * 
 * Revision 1.1.2.5  1994/05/10  21:45:03  bowe
 * 	Adjusted prototypes for reality
 * 	[1994/05/10  21:39:34  bowe]
 * 
 * Revision 1.1.2.4  1994/03/28  21:34:51  bowe
 * 	Added prototype for dce_db_inq_count()
 * 	[1994/03/28  21:18:05  bowe]
 * 
 * Revision 1.1.2.3  1994/03/09  22:56:43  bowe
 * 	Account for simpler acl_uuid in header.  Adjusted prototype for
 * 	dce_db_std_header_fetch().  Added prototypes: dce_db_delete_by_{name,uuid}.
 * 	[1994/03/09  22:46:38  bowe]
 * 
 * Revision 1.1.2.2  1994/03/02  20:10:09  bowe
 * 	Fixed misspelled func prototype name - dce_db_std_header_fetch().
 * 	[1994/03/01  13:44:14  bowe]
 * 
 * Revision 1.1.2.1  1994/01/24  22:01:40  bowe
 * 	Initial checkin
 * 	[1994/01/24  21:59:30  bowe]
 * 
 * Revision 1.1.2.1  1993/12/02  20:46:09  bowe
 * 	Initial revision.
 * 	[1993/12/02  20:33:00  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#ifndef _DCE_DB_H_
#define _DCE_DB_H_

#include <dce/database.h>
#include <dce/dcelibmsg.h>

#include <sys/stat.h>

/* ______________________________________________________________________ */

/* public-accessible stuff */

typedef void (*dce_db_convert_func_t)(idl_es_handle_t, void *, error_status_t *);

#define db_c_std_header		0x01
#define db_c_acl_uuid_header	0x02
#define db_c_index_by_name	0x04
#define db_c_index_by_uuid	0x08
#define db_c_readonly		0x10
#define db_c_create		0x20
#define db_c_no_auto_sync	0x40
#define db_c_lock		0x80
#define db_c_lock_nowait	0x100

typedef struct dce_db_handle_s_t *dce_db_handle_t;

/* ______________________________________________________________________ */

/* function prototypes */

void dce_db_open_file(const char *, const char *, unsigned32, int, void *,
        dce_db_convert_func_t, dce_db_handle_t *, error_status_t *);
void dce_db_open(const char *, const char *, unsigned32, dce_db_convert_func_t,
	dce_db_handle_t *, error_status_t *);
void dce_db_close(dce_db_handle_t *, error_status_t *);
void dce_db_free(dce_db_handle_t, void *, error_status_t *);
void dce_db_fetch(dce_db_handle_t, void *, void *, error_status_t *);
void dce_db_fetch_by_name(dce_db_handle_t, char *, void *, error_status_t *);
void dce_db_fetch_by_uuid(dce_db_handle_t, uuid_t *, void *, error_status_t *);
void dce_db_store(dce_db_handle_t, void *, void *, error_status_t *);
void dce_db_store_by_name(dce_db_handle_t, char *, void *, error_status_t *);
void dce_db_store_by_uuid(dce_db_handle_t, uuid_t *, void *, error_status_t *);
void dce_db_lock(dce_db_handle_t, error_status_t *);
void dce_db_unlock(dce_db_handle_t, error_status_t *);
void dce_db_iter_start(dce_db_handle_t, error_status_t *);
void dce_db_iter_next(dce_db_handle_t, void **, error_status_t *);
void dce_db_iter_next_by_name(dce_db_handle_t, char **, error_status_t *);
void dce_db_iter_next_by_uuid(dce_db_handle_t, uuid_t **, error_status_t *);
void dce_db_iter_done(dce_db_handle_t, error_status_t *);
void dce_db_inq_count(dce_db_handle_t, unsigned32 *, error_status_t *);
void dce_db_delete(dce_db_handle_t, void *, error_status_t *);
void dce_db_delete_by_name(dce_db_handle_t, char *, error_status_t *);
void dce_db_delete_by_uuid(dce_db_handle_t, uuid_t *, error_status_t *);
void dce_db_header_fetch(dce_db_handle_t, void *, dce_db_header_t *,
	error_status_t *);
void dce_db_std_header_init(dce_db_handle_t, dce_db_header_t *, uuid_t *,
	uuid_t *, uuid_t *, uuid_t *, unsigned32, error_status_t *);
void dce_db_stat(dce_db_handle_t, struct stat *, error_status_t *);

#endif /* _DCE_DB_H_ */
