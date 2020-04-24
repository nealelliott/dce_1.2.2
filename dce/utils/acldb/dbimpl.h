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
 * $Log: dbimpl.h,v $
 * Revision 1.1.4.2  1996/02/18  23:32:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:36  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/09/29  20:40 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/23  00:16 UTC  tatsu_s
 * 	Added db_c_lock_nowait.
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/22  17:03 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat9/1  1995/05/22  15:34 UTC  mullan_s
 * 	Add prototype for dce__db_lock_file.
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  19:00 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat8/1  1995/05/15  18:36 UTC  mullan_s
 * 	Add lockflag to dce_db context.
 * 	[1995/12/08  18:07:28  root]
 * 
 * Revision 1.1.2.5  1994/09/16  20:22:17  bowe
 * 	Add app_mutex to handle. [CR 12231]
 * 	[1994/09/16  20:20:21  bowe]
 * 
 * Revision 1.1.2.4  1994/09/05  19:18:23  bowe
 * 	Moved iterator defines here. [CR 11049]
 * 	[1994/09/05  19:17:44  bowe]
 * 
 * Revision 1.1.2.3  1994/06/16  19:35:21  rsalz
 * 	Copy iteration uuid into aligned space (OT CR 10957).
 * 	[1994/06/16  19:33:29  rsalz]
 * 
 * 	Copy iteration uuid into aligned space (OT CR 10957).
 * 	[1994/06/15  19:42:23  rsalz]
 * 
 * Revision 1.1.2.2  1994/05/10  21:45:03  bowe
 * 	Use correct name: dce_db_convert_func_t
 * 	[1994/05/10  21:37:44  bowe]
 * 
 * Revision 1.1.2.1  1994/01/24  22:01:41  bowe
 * 	Initial checkin
 * 	[1994/01/24  21:59:31  bowe]
 * 
 * Revision 1.1.4.1  1993/12/22  14:00:56  bowe
 * 	Changed #include <sys/fcntl.h> to #include <fcntl.h>.
 * 	[1993/12/22  13:59:22  bowe]
 * 
 * Revision 1.1.2.2  1993/12/10  17:49:11  bowe
 * 	#include <pthread.h>, not <dce/pthread.h>.
 * 	[1993/12/10  17:46:06  bowe]
 * 
 * Revision 1.1.2.1  1993/12/02  20:46:10  bowe
 * 	Initial revision.
 * 	[1993/12/02  20:34:43  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */
/*
 * Internal database / backing store implementation stuff.
 * Appl programmers never see this.
 */

#ifndef _DBIMPL_H_
#define _DBIMPL_H_

#include <string.h>
#include <limits.h>
#include <fcntl.h>

#include <dce/dce.h>
#include <pthread.h>
#include <dce/utctypes.h>
#include <database.h>

/* this is the Berkeley 4.4 db.h */
#include <db.h>

#include <dbif.h>

/* ______________________________________________________________________ */

/*  Key for the info struct for each database.  Must be something that
 *  real data will not use as a key. */
#define DBHEADER_KEY	"bd5a2d4a-bbff-11cc-9ce2-08000925634b"

/* States for the iteration functions */
#define ITER_NOT		0
#define ITER_BEGIN		1
#define ITER_IN_PROGRESS	2
#define ITER_AT_END		3

/* This (used internally) is for the info describing the database.
 * This is written into the database itself.
 */
struct db_info_s_t {
    unsigned32		index_type;	/* index_type from dce_db_handle_s_t */
    dce_db_header_type_t header_type;	/* standard, ACL, or no header? */
    utc_t		created;	/* date db created */
    utc_t		modified;	/* date db last modified */
};
typedef struct db_info_s_t db_info_t;

struct dce_db_handle_s_t {
    DB			*db;		/* dbm handle */
    unsigned32		iter_state;	/* iteration state */
    unsigned32		index_type;	/* by uuid or name */
    boolean		readonly;	/* opened database read-only? */
    db_info_t		*info;		/* database info */
    dce_db_convert_func_t	convert;
    pthread_mutex_t	mutex;		/* for lower-level db i/o */
    pthread_mutex_t	app_mutex;	/* for dce_db_lock/unlock */
    uuid_t		uuidkey;	/* for dce_db_iter_next{_by_uuid} */
    char		*namekey;	/* for dce_db_iter_next{_by_name} */
    boolean		lockflag;	/* lock of database requested? */
};

/* ______________________________________________________________________ */

/* some internally-used routines */
void	dce_db__info_write(dce_db_handle_t, error_status_t *);
void 	dce__db_lock_file(dce_db_handle_t, boolean32, boolean32, error_status_t *);

#endif	/* _DBIMPL_H_ */

