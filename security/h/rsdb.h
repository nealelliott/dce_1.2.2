/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb.h,v $
 * Revision 1.1.7.1  1996/05/10  13:16:29  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/27  17:45 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/hanfei_dce1.2_b1/1  1996/03/15  17:00 UTC  hanfei
 * 	add rsdb_foreign_person_create_init()
 * 	[1996/05/09  20:50:09  arvind]
 *
 * Revision 1.1.2.6  1993/02/19  17:11:51  ahop
 * 	dbcp: create rsdb_load_relations to modularize rsdb_load, add rsdb_clear_database
 * 	[1993/02/19  08:40:05  ahop]
 * 
 * Revision 1.1.2.5  1992/12/29  13:25:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:03  zeliff]
 * 
 * Revision 1.1.2.4  1992/11/20  15:50:57  ahop
 * 	move init_database here from rsdb_create.h; move keyseed out of rsdb_create()
 * 	add mkey file operations
 * 	[1992/11/10  23:03:40  ahop]
 * 
 * Revision 1.1.2.3  1992/10/07  20:28:05  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:48  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:45:12  burati
 * 	 Second replication code drop: bl5
 * 	 Add keyseed param to rsdb_create_master, rsdb_create_slave
 * 
 * Revision 1.1.2.2  1992/08/31  18:19:48  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:57:46  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - database interlude - function definitions
 */

#ifndef rsdb_h__included
#define rsdb_h__included


#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

#include <Pointer.h>
#include <rs_pvt.h>
#include <rsdb_pvt.h>

void rsdb_create(
#ifdef __STDC__
    unsigned_char_p_t       rep_name,
    sec_id_t                *rgy_creator,
    sec_id_t                *rgy_local_cell,
    sec_rgy_properties_t    *default_properties,
    error_status_t          *st
#endif
);

void rsdb_create_shutdown (
#ifdef __STDC__
    void
#endif
);

void rsdb_init_database (
#ifdef __STDC__
   void
#endif
);

void rsdb_load_relations (
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rsdb_load (
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rsdb_save (
#ifdef __STDC__
    char        *postfix,
    error_status_t   *st
#endif
);

void rsdb_close_files (
#ifdef __STDC__
    void
#endif
);

void rsdb_open_files (
#ifdef __STDC__
    void
#endif
);

void rsdb_clear_database (
#ifdef __STDC__
    db_handle       db
#endif
);

void rsdb_clear_mem (
#ifdef __STDC__
    void
#endif
);

void rsdb_clear_mem_log (
#ifdef __STDC__
    void
#endif
);

void rsdb_create_dirs (
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rsdb_loosen_directory_protection (
#ifdef __STDC__
    void
#endif
);

void rsdb_tighten_directory_protection (
#ifdef __STDC__
    void
#endif
);

void rsdb_protect_files (
#ifdef __STDC__
    boolean32  secure
#endif
);

void rsdb_clean_dir (
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rsdb_delete_files (
#ifdef __STDC__
    char             *postfix,
    error_status_t   *st
#endif
);

void rsdb_rename_files (
#ifdef __STDC__
    char            *fr_postfix,
    char            *to_postfix,
    error_status_t  *st
#endif
);

void rsdb_rename_file (
#ifdef __STDC__
    char            *fname,
    char            *fr_postfix,
    char            *to_postfix,
    error_status_t  *st
#endif
);

void rsdb_delete_rgy_files (
#ifdef __STDC__
    void
#endif
);

void rsdb_state_create (
#ifdef __STDC__
    rs_state_t       *rs_state,
    error_status_t   *st
#endif
);

void rsdb_state_delete (
#ifdef __STDC__
    void
#endif
);

void rsdb_state_open (
#ifdef __STDC__
    rs_state_t       *rs_state,
    error_status_t   *st
#endif
);

void rsdb_state_close (
#ifdef __STDC__
    void
#endif
);

void rsdb_state_write (
#ifdef __STDC__
    rs_state_t       *rs_state,
    error_status_t   *st
#endif
);

void rsdb_master_info_create (
#ifdef __STDC__
    rs_master_info_t    *master_info,
    error_status_t      *st
#endif
);

void rsdb_master_info_delete (
#ifdef __STDC__
    void
#endif
);

void rsdb_master_info_open (
#ifdef __STDC__
    rs_master_info_t    *master_info,
    error_status_t      *st
#endif
);

void rsdb_master_info_close (
#ifdef __STDC__
    void
#endif
);

void rsdb_master_info_write (
#ifdef __STDC__
    rs_master_info_t    *master_info,
    error_status_t      *st
#endif
);

void rsdb_mkey_create (
#ifdef __STDC__
    char            *fname,
    rsdb_auth_key_t *mkey,
    error_status_t  *st
#endif
);

void rsdb_mkey_read (
#ifdef __STDC__
    char                *fname,
    rsdb_auth_key_t     *mkey,    
    error_status_t      *st
#endif
);

void rsdb_mkey_delete (
#ifdef __STDC__
    char                *fname,
    error_status_t      *st
#endif
);

void rsdb_mkey_rename (
#ifdef __STDC__
    char                *src_name,
    char                *tgt_name,
    error_status_t      *st
#endif
);

boolean32 rsdb_mkey_exists (
#ifdef __STDC__
    char                *fname
#endif
);
    
void rsdb_fetch (
#ifdef __STDC__
    db_handle       db,
    Pointer         key,
    int             key_len,
    Pointer         *data,
    int             *data_len,
    error_status_t  *status
#endif
);

void rsdb_fetch_next (
#ifdef __STDC__
    db_handle       db,
    Pointer         key,
    int             key_len,
    Pointer         *next_key,
    int             *next_key_len,
    Pointer         *data,
    int             *data_len,
    error_status_t  *status
#endif
);

void rsdb_store (
#ifdef __STDC__
    db_handle        db,
    Pointer          key,
    int              key_len,
    Pointer          data,
    int              data_len,
    error_status_t  *status
#endif
);

void rsdb_replace (
#ifdef __STDC__
    db_handle        db,
    Pointer          key,
    int              key_len,
    Pointer          data,
    int              data_len,
    error_status_t   *status
#endif
);

void rsdb_delete (
#ifdef __STDC__
    db_handle       db,
    Pointer         key,
    int             key_len,
    error_status_t  *status
#endif
);

void rsdb_foreign_person_create_init (
    error_status_t          *st_p
);

void rsdb_dump_database (
#ifdef __STDC__
    db_handle   db
#endif
);

#endif /* rsdb_h__included */

