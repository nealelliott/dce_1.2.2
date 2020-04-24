/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: deif.h,v $
 * Revision 1.1.10.2  1996/03/11  13:26:09  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:13  marty]
 *
 * Revision 1.1.10.1  1995/12/08  17:55:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:28:35  root]
 * 
 * Revision 1.1.8.2  1994/03/25  14:42:18  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 	[1994/03/25  14:41:34  sekhar]
 * 
 * Revision 1.1.8.1  1994/01/28  23:10:10  burati
 * 	Cleanup for (dlg_bl1) drop
 * 	[1994/01/19  20:30:13  burati]
 * 
 * Revision 1.1.6.1  1993/10/05  22:29:30  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  18:49:17  mccann]
 * 
 * Revision 1.1.4.3  1993/01/28  18:45:42  burati
 * 	CR6655 Add deif_is_group_class and deif_any_group_class prototypes
 * 	[1993/01/27  22:12:49  burati]
 * 
 * Revision 1.1.4.2  1992/12/29  12:36:59  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:32:52  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/08  15:25:16  burati
 * 	Add new functionality to support POSIX mask calculation
 * 	[1992/05/06  22:26:44  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  deif.h
**
** Copyright (c) Hewlett-Packard Company 1991, 1993
** Unpublished work. All Rights Reserved.
**
*/
/*
**      sec_acl editor generic interface header file
** 
*/

#include <dce/nbase.h>
#include <dce/aclbase.h>

#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>
#include <acl_edit.h>
#include <macros.h>

/* Table for parsing of entry_types */
typedef struct {
        char                    *name;
        sec_acl_entry_type_t    entry_type;
} sec_acl_entry_type_table_t;

typedef enum {
    NORMAL_MOD,
    INCREASE_MOD,
    DECREASE_MOD
} sec_acl_edit_mod_type_t;

/*
 * deif interface prototypes 
 */

error_status_t deif_validate_sec_acl_entry(
#ifdef __STDC__
    char                        *sec_acl_string,
    sec_acl_entry_t             *sec_acl_entry_p,
    boolean32                   parse_key_only
#endif
);


error_status_t deif_sec_acl_from_file(
#ifdef __STDC__
    char                        *fname
#endif
);


error_status_t deif_do_delete(
#ifdef __STDC__
    sec_acl_entry_t             *sec_acl_entry_p
#endif
);


error_status_t deif_do_kill_entries(
#ifdef __STDC__
    void
#endif
);


error_status_t deif_do_list(
#ifdef __STDC__
    sec_acl_edit_output_t       output_type,
    void                        *output
#endif
);


error_status_t deif_list_permissions(
#ifdef __STDC__
    sec_acl_edit_output_t       output_type,
    void                        *output
#endif
);


error_status_t deif_do_modify(
#ifdef __STDC__
    sec_acl_entry_t             *sec_acl_entry_p,
    sec_acl_edit_mod_type_t     mod_type,
    sec_acl_permset_t           *perms_changed_p
#endif
);


error_status_t deif_do_increase_mods(
#ifdef __STDC__
    int                         num_entries,
    sec_acl_entry_t             *sec_acl_entries,
    sec_acl_permset_t           *perms_changed_p
#endif
);


error_status_t deif_do_decrease_mods(
#ifdef __STDC__
    int                         num_entries,
    sec_acl_entry_t             *sec_acl_entries
#endif
);


error_status_t deif_do_substitute(
#ifdef __STDC__
    int                         num_entries,
    sec_acl_entry_t             *sec_acl_entries
#endif
);


error_status_t deif_do_test_access(
#ifdef __STDC__
    char                        *perms,
    sec_acl_edit_output_t       output_type,
    void                        *output
#endif
);


error_status_t deif_do_get_access(
#ifdef __STDC__
    sec_acl_edit_output_t       output_type,
    void                        *output
#endif
);


error_status_t deif_set_default_cell(
#ifdef __STDC__
    char                        *cell_name
#endif
);


boolean32 deif_bind(
#ifdef __STDC__
    char                        *object_name,
    boolean32                   bind_to_entry,
    sec_acl_type_t              sec_acl_type,
    sec_acl_edit_mask_calc_t    *mask_calc_type_p
#endif
);


boolean32 deif_debug_bind(
#ifdef __STDC__
    char                        *addr,
    char                        *residual,
    sec_acl_type_t              sec_acl_type,
    sec_acl_edit_mask_calc_t    *mask_calc_type_p
#endif
);


PRIVATE boolean32 deif_entry_compare(
#ifdef __STDC__
    sec_acl_entry_t             *sec_acl_entry1_p,
    sec_acl_entry_t             *sec_acl_entry2_p,
    boolean32                   compare_key_only
#endif
);


PRIVATE void deif_sort_entries(
#ifdef __STDC__
    int                         num_entries,
    sec_acl_entry_t             **entries_p
#endif
);


PRIVATE void deif_permset_output(
#ifdef __STDC__
    sec_acl_permset_t           perms
#endif
);


PRIVATE void deif_mask_perms(
#ifdef __STDC__
    sec_acl_permset_t           perms,
    sec_acl_permset_t           mask_perms
#endif
);


PRIVATE error_status_t deif_decode_permset(
#ifdef __STDC__
    char                        *perms,
    sec_acl_permset_t           *permset_p
#endif
);


PRIVATE error_status_t deif_get_key(
#ifdef __STDC__
    sec_acl_entry_t             *sec_acl_entry_p,
    sec_acl_entry_type_t        entry_type,
    char                        *string
#endif
);


error_status_t deif_save_sec_acl(
#ifdef __STDC__
    void
#endif
);


PRIVATE void deif_free_entries(
#ifdef __STDC__
    void
#endif
);


PRIVATE void deif_print_extended_info(
#ifdef __STDC__
    sec_acl_extend_info_t       *info
#endif
);


PRIVATE error_status_t deif_get_extended_info(
#ifdef __STDC__
    sec_acl_entry_t             *entry_p,
    char                        *string
#endif
);


error_status_t deif_calc_union_permissions(
#ifdef __STDC__
    boolean32                   masked,
    sec_acl_permset_t           *perms_p
#endif
);


error_status_t deif_purge_ineffective(
#ifdef __STDC__
    void
#endif
);


boolean32 deif_is_group_class(
#ifdef __STDC__
    sec_acl_entry_t             *entryP
#endif
);

    
boolean32 deif_any_group_class(
#ifdef __STDC__
    int                         num_entries,
    sec_acl_entry_t             *entries
#endif
);


error_status_t deif_calc_mask(
#ifdef __STDC__
    void
#endif
);


void deif_status_print(
#ifdef __STDC__
    error_status_t              *st_p,
    boolean32                   use_gui
#endif
);


void deif_msg_print(
#ifdef __STDC__
    char                        *string,
    boolean32                   use_gui
#endif
);


void deif_msg_status_print(
#ifdef __STDC__
    char                        *string,
    error_status_t              *st_p,
    boolean32                   use_gui
#endif
);
