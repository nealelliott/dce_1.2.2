/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl_edit.h,v $
 * Revision 1.1.4.2  1996/02/18  23:31:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:19:18  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  20:46 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/21  15:26 UTC  tatsu_s
 * 	Added is_local in dcp_acl_info_t.
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/14  14:19 UTC  truitt
 * 	Initial HPDCE02 branch.
 * 
 * 	HP revision /main/truitt_dcecp_chfts19/1  1995/03/14  14:17 UTC  truitt
 * 	CHFts14241: Add the interp parameter to the free
 * 	acl manager table function.  Change from void to int.
 * 	[1995/12/08  17:53:57  root]
 * 
 * Revision 1.1.2.9  1994/09/08  14:00:34  salamone
 * 	CR11968-Setting authentication on binding handles.
 * 	[1994/09/08  13:59:05  salamone]
 * 
 * Revision 1.1.2.8  1994/08/25  15:32:54  salamone
 * 	CR11635 - Handle 1 string binding specification for an ACL object.
 * 	[1994/08/25  15:27:36  salamone]
 * 
 * Revision 1.1.2.7  1994/07/29  18:44:24  salamone
 * 	CR11205 - Maintain mask_calc_type for each ACL mgr.
 * 	[1994/07/29  18:27:34  salamone]
 * 
 * Revision 1.1.2.6  1994/07/07  14:35:34  salamone
 * 	Provide support for more than one ACL manager.
 * 	[1994/07/07  14:31:26  salamone]
 * 
 * Revision 1.1.2.5  1994/05/18  20:38:59  salamone
 * 	CR10621 - "acl show", "acl check", and "acl permissions" should
 * 		   display the appropriate info if unauthenticated.
 * 	[1994/05/13  20:29:22  salamone]
 * 
 * Revision 1.1.2.4  1994/02/09  15:58:40  salamone
 * 	Added "acl replace" and "acl delete" support.
 * 	Added delegation support.
 * 	[1994/02/09  15:35:42  salamone]
 * 
 * Revision 1.1.2.3  1994/01/24  22:20:06  salamone
 * 	Added support for "acl modify"
 * 	[1994/01/24  22:16:32  salamone]
 * 
 * Revision 1.1.2.2  1994/01/05  19:42:17  salamone
 * 	Added "acl show" support
 * 	[1994/01/05  19:40:52  salamone]
 * 
 * Revision 1.1.2.1  1993/12/29  16:54:39  salamone
 * 	Initial version of acl_edit.h
 * 	[1993/12/29  16:53:36  salamone]
 * 
 * $EndLog$
 */


/*
 * MODULE:
 * acl_edit.h
 *
 * DESCRIPTION:
 * External declarations for editing ACLs.
 *
 */

#ifndef _ACL_EDIT_H
#define _ACL_EDIT_H

/*
 * DECLARATIONS 
 */
#define DCP_C_ACL_MGR                      \
		  dcp_acl_info_p->mgr_type_tbl[dcp_acl_info_p->mgr_type_index]
#define DCP_C_ACL_PRINTSTRING_LEN	   32
#define DCP_C_ACL_LOCAL_CELL               "/.:"


/* 
 * There are three mask calculation types.
 *
 * 1) POSIX_MASK_OBJ_CALC is the default setting. It indicates that
 *    the POSIX 1003.6 Draft 12 Steps 3, 4, 5, 7, and 8 will be
 *    followed.
 *
 * 2) CALC_MASK_OBJ_AFTER is set if the "-mask calc" switch on the 
 *    "acl modify" operation is specified.
 *
 *    RESULT: POSIX 1003.6 Steps 3,4,5,7, and 8 are NOT followed but
 *    the "mask_obj" will be calculated with the union of all
 *    entries except user_obj, other_obj, and unauthenticated. 
 *    (Call to sec_acl_calc_mask() routine is made.)
 *
 * 3) NO_MASK_OBJ_CALC refers to the following situations:
 *    1) If the "-mask nocalc" switch is specified on the 
 *       "acl modify" operation.
 *    2) If the ACL manager requests no POSIX "mask_obj" semantics
 *	 (from sec_acl_get_mgr_types_semantics()
 *			      and
 *	       sec_acl_get_manager_types() calls)
 *    3) If a "mask_obj" ACL entry is explicitly specified to
 *	 be added or changed.
 *
 *    RESULT: POSIX 1003.6 Steps 3,4,5,7, and 8 are NOT followed 
 *    and the "mask_obj" entry is NOT calculated.
 *
 */
typedef enum {
    POSIX_MASK_OBJ_CALC,
    CALC_MASK_OBJ_AFTER,
    NO_MASK_OBJ_CALC
} dcp_e_acl_mask_calc_t;

/* 
 * There are three modification types.
 *
 * If the modification type is NORMAL_MOD, one ACL entry is either
 * added or changed in the current ACL entry list of the specified object. 
 * The net result is a new "mask_obj" ACL entry with it's candidate
 * masked permission set. POSIX 1003.6 Draft 12 STEP 8.
 * 
 * If the modification type is INCREASE_MOD, one ACL entry is either
 * added or changed in the current ACL entry list of the specified object. 
 * If changed, the net result will be that it's permission set increases.
 * If added, the net result will be a new ACL entry with it's permission set.
 *
 * If the modification type is DECREASE_MOD, one ACL entry is changed
 * in the current ACL entry list of the specified object. The
 * net result will be that it's permission set decreases.
 */
typedef enum {
    NORMAL_MOD,
    INCREASE_MOD,
    DECREASE_MOD
} dcp_e_acl_mod_edit_type_t;


/* 
 * An object can have more than one ACL manager.  A polymorphic
 * object, as in DCE 1.1, can be a principal name and a principal 
 * directory and therefore have 2 ACL managers.
 * This structure is dynamically allocated for all ACL managers
 * of an object.
 */
typedef struct dcp_acl_mgr_info_s_t {
    sec_acl_printstring_t   manager_info;
    dcp_e_acl_mask_calc_t   mask_calc_type;
    sec_acl_printstring_t   sec_acl_printstrings[DCP_C_ACL_PRINTSTRING_LEN];
    unsigned32   	    sec_acl_num_printstrings;
    sec_acl_list_t          sec_acl_list;
    sec_acl_t               *sec_acl_p;  
} dcp_acl_mgr_info_t, *dcp_acl_mgr_info_p_t;

/* 
 * This structure contains all the necessary ACL information to
 * perform an acl operation. Each acl operation will refresh this
 * cache. 
 * If a list of ACL objects are specified, the acl operation 
 * will be performed on each ACL object in the order specified. Note
 * that the acl operation on each ACL object will refresh this cache.
 */
typedef struct dcp_acl_info_s_t {
    char 		    *sec_acl_named_object; 
    sec_acl_type_t          sec_acl_type;
    sec_acl_handle_t        sec_acl_handle;
    unsigned32              num_acl_mgrs;
    char                    mgr_type_specified[sec_acl_printstring_len];
    uuid_p_t                sec_acl_mgr_type;
    dcp_acl_mgr_info_p_t    mgr_type_tbl;
    int                     mgr_type_index;
    sec_rgy_handle_t        local_cell_rgy_handle;
    sec_rgy_name_t          local_cell_name;
    uuid_t                  local_cell_uuid;
    sec_rgy_handle_t        default_cell_rgy_handle;
    sec_rgy_name_t          default_cell_name;
    uuid_t                  default_cell_uuid;
#ifndef	HPDCE_DCED_LOCAL
    boolean32		    is_local;
#endif	/* HPDCE_DCED_LOCAL */
} dcp_acl_info_t, *dcp_acl_info_p_t;

/* 
 * Table for parsing and sorting ACL entry_types 
 */
typedef struct {
    char                    *acl_string_name;
    sec_acl_entry_type_t    acl_entry_type;
} dcp_acl_entry_type_table_t;


/*
 * PROTOTYPES 
 */
int  dcp_acl_list_objects(Tcl_Interp *, char *, int *, char ***);
int  dcp_acl_bind_named_object(Tcl_Interp *, dcp_acl_info_p_t, boolean32);
int  dcp_acl_do_check(Tcl_Interp *, dcp_acl_info_p_t);
void dcp_acl_do_permissions(Tcl_Interp *, dcp_acl_info_p_t);
int  dcp_acl_do_show_entries(Tcl_Interp *, dcp_acl_info_p_t);
void dcp_acl_do_show_cell(Tcl_Interp *, dcp_acl_info_p_t);
void dcp_acl_do_show_mgrs(Tcl_Interp *, dcp_acl_info_p_t);
int  dcp_acl_do_modify_entries(int, char *, char *, char *, char *, Tcl_Interp *, dcp_acl_info_p_t);
int  dcp_acl_do_modify_cell(char *, Tcl_Interp *, dcp_acl_info_p_t);
int  dcp_acl_do_replace_entries(char *, Tcl_Interp *, dcp_acl_info_p_t);
int  dcp_acl_do_delete_entries(Tcl_Interp *, dcp_acl_info_p_t);
int  dcp_acl_do_save_sec_acl(Tcl_Interp *, dcp_acl_info_p_t);
int  dcp_acl_free_acl_mgr_tbl(Tcl_Interp *, dcp_acl_info_p_t);

#endif   /* _ACL_EDIT_H */
