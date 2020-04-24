/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta.h,v $
 * Revision 1.1.8.2  1996/02/18  23:34:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:37  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:18:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:54  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:02:47  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:20  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:39:38  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:19  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:04:34  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:20:25  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:35:00  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:54:59  zeliff]
 * 
 * Revision 1.1.2.3  1992/03/22  22:11:04  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:28:50  weisman]
 * 
 * Revision 1.1.2.2  1992/01/22  23:01:41  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:17:18  melman]
 * 
 * $EndLog$
 */
#ifndef _TA_H
#define _TA_H
/*
 * Module ta.h
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
#include <server.h>
#include <dbdef.h>
#include <dce/id_base.h>

/*
 * ta_adver.c
 */

/* static ping_state_ascii */

void *
adver_start (
    nsgbl_ns_t		*);

void
adver_signal_new_info (
    void		*);

void
adver_wait_for_advertisement (
    void		*);

void
adver_kill (
    void		*);

int
adver_diag_dump_control_block (
    int			,
    void		*,
    int			(*)(char    *,
                            void *),
    void		*);

int
adver_diag_set_message_flow (
    void		*,
    diag_state_t	,
    diag_state_t	*);

/*
 * ta_cle.c
 */

dns_status_t
mgmt_create_cle (
    nsgbl_ns_t		   *,
    struct user_descriptor *,
    nsgbl_ch_t   	   *,
    FullName_t		   *,
    FullName_t		   *,
    int		   	   ,
    version_t		   *);

dns_status_t
ta_cle_get_nsnickname (
    nsgbl_ch_t		*);

/*
 * ta_create.c
 */

dns_status_t 
create_directory_acs (
    struct user_descriptor *,
    struct db_stream	   *,
    int                    );

dns_status_t 
create_entry_acs (
    struct user_descriptor *,
    struct dir_hash_entry  *,
    struct db_data	   *,
    struct db_data  	   *,
    struct nsgbl_ch	   *);

dns_status_t 
create_record (
    struct db_data	*,
    int			,
    struct entrylist	*);


dns_status_t 
ta_create_obj (
    struct user_descriptor *,
    Progress_t		   *,
    ClassName_t		   *,
    ClassVersion_t	   *,
    uuid_t                 *,
    Timestamp_t		   *);


dns_status_t 
ta_create_link (
    struct user_descriptor *,
    Progress_t		   *,
    FullName_t		   *,
    Timeout_t		   *,
    Timestamp_t		   *);

dns_status_t
ta_create_dir_record (
    user_descriptor_t	*,
    struct db_stream	*,
    version_t		*,
    Timestamp_t		*,
    ObjUID_t		*,
    ReplicaPointer_t	*,
    ParentPointer_t	*);


dns_status_t 
ta_create_dir (
    struct user_descriptor *,
    Progress_t		   *,
    Timestamp_t		   *);


dns_status_t
ta_create_child_check (
    struct db_stream	*);


dns_status_t 
ta_create_child (
    struct user_descriptor *,
    Progress_t		   *,
    sec_id_foreign_t       *,
    ObjUID_t		   *,
    Set_t		   *,
    ObjUID_t		   *);



/*
 * ta_delete.c
 */

dns_status_t 
ta_delete (
    struct user_descriptor *,
    Progress_t		   *,
    int			   );

dns_status_t 
ta_delete_dir (
    struct user_descriptor *,
    Progress_t		   *);

dns_status_t 
ta_delete_replica (
    struct user_descriptor *,
    FullName_t		   *);


/*
 * rpc_dispatch.c
 */

void
trans_dispatcher (
    user_descriptor_t	*,
    byte_t		*);

void
back_dispatcher (
    user_descriptor_t	*,
    byte_t		*);

int
get_user_pac (
    handle_t		,
    user_descriptor_t	*);

#if defined(DCE_SEC) && defined(DCE_CDS_DEBUG)
# include <dce/id_base.h>

void
view_pac (
    sec_id_pac_t	*);

#endif /* DCE_SEC && DCE_CDS_DEBUG */

/* static setup_return_status */


/*
 * ta_enumerate.c
 */
dns_status_t 
build_att_set (
    struct db_data	*,
    SimpleName_t	*,
    Set_t		*,
    int			,
    int			*,
    int			*);

dns_status_t 
ta_enumerate_att (
    struct user_descriptor *,
    Progress_t		   *,
    int		   	   ,
    SimpleName_t	   *,
    int			   ,
    int			   *);

dns_status_t 
build_name_set (
    struct db_stream	   *,
    struct user_descriptor *,
    SimpleName_t	   *,
    SimpleName_t	   *,
    SimpleName_t	   *,
    int		   	   ,
    int			   ,
    Set_t		   *,
    int			   *,
    int			   *,
    int			   );

dns_status_t 
ta_enumerate (
    struct user_descriptor *,
    Progress_t		   *,
    SimpleName_t	   *,
    SimpleName_t	   *,
    SimpleName_t	   *,
    int		   	   ,
    int			   ,
    int			   *,
    int			   );

/*
 * ta_epoch.c
 */
dns_status_t 
ta_epoch (
    struct user_descriptor *,
    FullName_t		   *,
    Set_t		   *,
    Set_t		   *);

/*
 * ta_inch.c
 */
dns_status_t
ta_disallow (
    struct user_descriptor *,
    Progress_t		   *);

dns_status_t
ta_allow (
    struct user_descriptor *,
    Progress_t		   *);

/*
 * ta_link.c
 */
dns_status_t
ta_link_replica (
    struct user_descriptor *,
    struct user_descriptor *,
    Progress_t		   *,
    ObjUID_t		   *,
    Update_t		   *,
    int		   	   ,
    ObjUID_t		   *,
    ObjUID_t		   *,
    version_t		   *);

/*
 * ta_lookup.c
 */

dns_status_t 
check_exists (
    struct db_stream	*,
    int                 );

dns_status_t
operationallowed (
    int			,
    int			,
    int			);

dns_status_t
walktree (
    struct user_descriptor *,
    int			   ,
    int		   	   ,
    Progress_t		   *,
    struct db_stream	   *);

/*
 * ta_mgmt.c
 */

struct nsgbl_ch *
mgmt_find_chuid (
    struct nsgbl_ns	*ns_p,
    ObjUID_t		*uid_p);

struct nsgbl_ch  *
mgmt_find_chname (
    struct nsgbl_ns	*,
    FullName_t		*);

dns_status_t
mgmt_find_chdata (
    nsgbl_ns_t          *,
    char                *);

dns_status_t
mgmt_create_namespace (
    nsgbl_ns_t             *,
    struct user_descriptor *,
    nsgbl_ch_t  	   *,
    SimpleName_t	   *);

dns_status_t
mgmt_delete_ch (
    nsgbl_ns_t         *,
    user_descriptor_t  *,
    nsgbl_ch_t         *);

dns_status_t
mgmt_clear_ch (
    user_descriptor_t	*,
    nsgbl_ch_t		*);

dns_status_t
mgmt_add_ch (
    struct nsgbl_ch	*);

dns_status_t
mgmt_check_linked (
    FullName_t		*,
    FullName_t		*);

/*
 * ta_modify.c
 */

dns_status_t
ta_modify_attribute (
    user_descriptor_t	*,
    Progress_t		*,
    int			,
    Update_t		*,
    int			);

dns_status_t
ta_modify_replica (
    struct user_descriptor *,
    ObjUID_t		   *,
    Update_t		   *,
    int			   );

/*
 * ta_read.c
 */

dns_status_t
readone (
    struct db_data	*,
    struct db_stream	*,
    SimpleName_t	*,
    Timestamp_t		*,
    byte_t		*,
    int			,
    int			*,
    int			*);

dns_status_t
ta_read_att (
    struct user_descriptor *,
    Progress_t		   *,
    int			   ,
    unsigned int   	   ,
    SimpleName_t	   *,
    Timestamp_t		   *,
    int			   ,
    int			   *);

dns_status_t
ta_read_replica (
    struct user_descriptor *,
    ObjUID_t		   *,
    SimpleName_t	   *,
    Timestamp_t		   *,
    int			   ,
    byte_t		   *,
    int			   *);

/*
 * ta_replica.c
 */

dns_status_t
create_replica_record (
    struct db_stream	*,
    Timestamp_t         *,
    ObjUID_t		*,
    FullName_t		*,
    ObjUID_t		*,
    int			,
    AtomicValue_t	*);

void
ta_add_replica_check (
    struct db_stream	*);

dns_status_t
ta_replica_add (
    struct user_descriptor *,
    FullName_t		   *,
    int			   );

dns_status_t
ta_replica_remove (
    struct user_descriptor *,
    ObjUID_t		   *,
    int			   ,
    struct db_stream	   *);

/*
 * ta_resolve.c
 */

dns_status_t
ta_resolve_name (
    struct user_descriptor *,
    Progress_t		   *);

/*
 * ta_skulk.c
 */

dns_status_t
ta_skulk (
    struct user_descriptor *,
    Progress_t		   *);

/*
 * ta_subs.c
 */

int
ta_diag_set_random (
    int			);

int
ta_diag_set_strict_upd_prot (
    int			);

void
resolved_null (
    Progress_t		*);

int
build_set (
    DBSet_t		*,
    Set_t		*,
    int			,
    int			);

int
build_random_set (
    DBSet_t		*,
    Set_t		*,
    int			,
    int			);

int
ts_in_range (
    struct data_record	*,
    Timestamp_t	        *);

int
progress_size (
    Progress_t		*);

int
progress_make_room (
    Progress_t		*);

int
low_convergence (
    struct data_record	*);

dns_status_t
applyupdate (
    struct db_stream        	*,
    int		        	,
    Update_t	        	*,
    struct attribute_descriptor *,
    int			        ,
    struct tlog_dsc	        *);

Timeout_t *
min_timeout (
    Timeout_t		*,
    Timeout_t		*);

/*
 * ta_test.c
 */

dns_status_t
ta_test_att (
    struct user_descriptor *,
    Progress_t		   *,
    int		    	   ,
    SimpleName_t	   *,
    AtomicValue_t          *,
    byte_t		   *);

/*
 * ta_testgroup.c
 */

void
initloop (
    void);

dns_status_t
ta_testgroup (
    struct user_descriptor *,
    Progress_t		   *,
    FullName_t		   *,
    ObjUID_t		   *,
    int			   ,
    int			   *);


#endif  /* #ifndef _TA_H */
