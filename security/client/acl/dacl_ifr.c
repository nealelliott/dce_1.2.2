/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_ifr.c,v $
 * Revision 1.1.13.1  1996/10/03  14:43:09  arvind
 * 	Avoid falling off the end of sec_acl_test_access without returning a value.
 * 	[1996/09/16  20:01 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.10.3  1996/02/18  00:06:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:54  marty]
 * 
 * Revision 1.1.10.2  1996/02/07  17:48:15  parul
 * 	DCE 1.2.1 final drop from HP
 * 	[1996/02/07  16:14:00  parul]
 * 
 * 	DCE 1.2.1 final drop from HP
 * 
 * Revision 1.1.7.1  1994/01/28  23:09:54  burati
 * 	Delegation/EPAC changes - call v1.0 or 0.0 based on if_version (dlg_bl1)
 * 	[1994/01/19  20:14:16  burati]
 * 
 * Revision 1.1.5.7  1993/03/18  22:43:10  burati
 * 	CR7458 Put back retry case in get_mgr_types and ..._semantics which
 * 	were inadvertently removed in previous attempt to make rebinding as
 * 	efficient as possible.
 * 	[1993/03/18  22:18:09  burati]
 * 
 * Revision 1.1.5.6  1993/03/12  16:45:31  burati
 * 	CR7458 Free string binding from acl handle in sec_acl_release_handle
 * 	[1993/03/12  15:52:22  burati]
 * 
 * Revision 1.1.5.5  1992/12/29  12:36:18  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:32:14  zeliff]
 * 
 * Revision 1.1.5.4  1992/12/14  21:02:12  burati
 * 	CR5899 get_manager_types and get_mgr_types_semantics need to rebind
 * 	       once on bad status (not just bad rpc status) to make sure
 * 	       the failure was not because of a stale cache entry.
 * 	[1992/12/14  19:45:11  burati]
 * 
 * Revision 1.1.5.3  1992/10/22  13:50:47  burati
 * 	CR5456 Infinite loop on acl referral error
 * 	[1992/10/20  21:58:10  burati]
 * 
 * Revision 1.1.5.2  1992/09/29  21:14:11  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:17:36  sekhar]
 * 
 * Revision 1.1.2.8  1992/07/09  19:45:54  burati
 * 	CR4161 More DFS ACL fixes from Beth, to hide local vs remote paths.
 * 	[1992/07/08  19:46:03  burati]
 * 
 * 	CR4161 DFS ACL support work
 * 	[1992/06/18  21:52:56  burati]
 * 
 * Revision 1.1.2.7  1992/05/14  20:12:10  burati
 * 	Fixed my fix.  Boy do I need a vacation :-)
 * 	[1992/05/14  20:07:04  burati]
 * 
 * Revision 1.1.2.6  1992/05/14  19:38:47  burati
 * 	Fixed copy/paste error that kept it from creating new mask
 * 	[1992/05/14  19:34:50  burati]
 * 
 * Revision 1.1.2.5  1992/05/08  15:10:08  burati
 * 	Add sec_acl_calc_mask(...)
 * 	[1992/05/07  15:49:21  burati]
 * 
 * 	Add sec_acl_get_mgr_types_semantics(...)
 * 	[1992/04/29  03:07:52  burati]
 * 
 * Revision 1.1.2.4  1992/04/28  16:01:23  burati
 * 	CR755 Free new elements of context.
 * 	[1992/04/28  02:03:41  burati]
 * 
 * Revision 1.1.2.3  1992/04/23  14:02:13  burati
 * 	Make sure test_access & test_access_on_behalf return a value
 * 	[1992/04/23  02:14:25  burati]
 * 
 * Revision 1.1.2.2  1992/03/19  02:14:54  burati
 * 	CR734 Return sec_acl_invalid_acl_handle if passed NULL handle
 * 	[1992/03/18  22:39:47  burati]
 * 
 * Revision 1.1  1992/01/19  14:47:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  dacl_ifr.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
**      SEC_ACL Interface Routines/Client Agent
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)dacl_ifr.c	20 - 02/24/92";
#endif

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
#include <dce/daclif.h>
#include <rdaclif.h>
#include <rdaclifv0.h>
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */

#include <dacl_pvt.h>

#ifdef	HPDCE_ACLMGR_LOCAL_BIND
#include <dce/daclif.h>
#include <rdaclif.h>
#include <rdaclifv0.h>
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */

#include <dfs_dce_acl.h>
#include <macros.h>
#include <un_maloc.h>
#include <un_strng.h>

#ifdef IDL_PROTOTYPES
#define PROTOTYPE(x) x
#else
#define PROTOTYPE(x) ()
#endif

PUBLIC void sec_acl_lookup
#ifndef __STDC__
    (sech, manager_type_p, sec_acl_type, sec_acl_list_p, st_p)
    sec_acl_handle_t        sech;
    uuid_t                  *manager_type_p;
    sec_acl_type_t          sec_acl_type;
    sec_acl_list_t          *sec_acl_list_p;
    error_status_t          *st_p;
#else
  (
    sec_acl_handle_t        sech,
    uuid_t                  *manager_type_p,
    sec_acl_type_t          sec_acl_type,
    sec_acl_list_t          *sec_acl_list_p,
    error_status_t          *st_p
  )
#endif
{
    error_status_t      rpc_status;
    sec_acl_result_t    result;
    boolean32           done = false;
    sec_acl_t           sec_acl, *return_acl_p;

    if (!sech) {
        SET_STATUS(st_p, sec_acl_invalid_acl_handle);
        return;
    }

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    if (IS_LOCAL_ACL(sech)) {
        if (IF_VERSION(sech)==0) {
            (*(V0_0_EPV(sech).rdacl_lookup))
                (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
		 sec_acl_type, &result, &rpc_status);
        } else {
            (*(V1_0_EPV(sech).rdacl_lookup))
                (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
		 sec_acl_type, &result, &rpc_status);
        }
        if (GOOD_STATUS(&result.st)) {
            *sec_acl_list_p = *(result.tagged_union.sec_acl_list);
        }
        SET_STATUS(st_p, result.st);
    } else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    if (IS_DFS_ACL(sech)) {
        *st_p = dfs_dce_acl_lookup((char *)COMPONENT_NAME(sech), sec_acl_type,
                                    &sec_acl);
        if (GOOD_STATUS(st_p)) {
            return_acl_p = (sec_acl_p_t) malloc(sizeof(sec_acl_t));
            if (return_acl_p) {
                sec_acl_list_p->num_acls = 1;
                *return_acl_p = sec_acl;    /* Copy info from local storage */
                sec_acl_list_p->sec_acls[0] = return_acl_p;
            } else {
                sec_acl_list_p->num_acls = 0;
                SET_STATUS(st_p, sec_acl_cant_allocate_memory);
            }
        }
    } else while (!done && sec__acl_check_binding(sech, st_p)) {
        if (IF_VERSION(sech)==0) {
            (*rdaclif_v0_0_c_epv.rdacl_lookup)
                (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
                    sec_acl_type, &result, &rpc_status);
        } else {
            (*rdaclif_v1_0_c_epv.rdacl_lookup)
                (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
                    sec_acl_type, &result, &rpc_status);
        }
        if (! GOOD_STATUS(&rpc_status)) {
            SET_STATUS(&result.st, rpc_status);
            HANDLE_VALID(sech) = false;                     /* should retry */
        } else {
            done = true;
        }

        if (GOOD_STATUS(&result.st)) {
            *sec_acl_list_p = *(result.tagged_union.sec_acl_list);
        }
        SET_STATUS(st_p, result.st);
    }
}


PRIVATE boolean32 sec__acl_new_types
#ifndef __STDC__
    (sec_acl_list_p)
    sec_acl_list_t *sec_acl_list_p;
#else
  (
    sec_acl_list_t *sec_acl_list_p
  )
#endif
{
    int i,j;
    for (i = 0; i < sec_acl_list_p->num_acls; i++) {
        for (j = 0; j < sec_acl_list_p->sec_acls[i]->num_entries; j++) {
            switch (sec_acl_list_p->sec_acls[i]->sec_acl_entries[j].entry_info.entry_type) {
                case sec_acl_e_type_user_obj_deleg:
                case sec_acl_e_type_group_obj_deleg:
                case sec_acl_e_type_other_obj_deleg:
                case sec_acl_e_type_any_other_deleg:
                case sec_acl_e_type_user_deleg:
                case sec_acl_e_type_group_deleg:
                case sec_acl_e_type_for_other_deleg:
                case sec_acl_e_type_for_user_deleg:
                case sec_acl_e_type_for_group_deleg:
                    return true;
                    break;
                default:
                    break;
            }
        }
    }
    return false;
}


PUBLIC void sec_acl_replace
#ifndef __STDC__
    (sech, manager_type_p, sec_acl_type, sec_acl_list_p, st_p)
    sec_acl_handle_t        sech;
    uuid_t                  *manager_type_p;
    sec_acl_type_t          sec_acl_type;
    sec_acl_list_t          *sec_acl_list_p;
    error_status_t          *st_p;
#else
  (
    sec_acl_handle_t        sech,
    uuid_t                  *manager_type_p,
    sec_acl_type_t          sec_acl_type,
    sec_acl_list_t          *sec_acl_list_p,
    error_status_t          *st_p
  )
#endif
{
    error_status_t      rpc_status;
    boolean32           done = false;

    if (!sech) {
        SET_STATUS(st_p, sec_acl_invalid_acl_handle);
        return;
    }

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    if (IS_LOCAL_ACL(sech)) {
        if (IF_VERSION(sech)==0) {
            if (sec__acl_new_types(sec_acl_list_p)) {
                SET_STATUS(st_p, sec_acl_invalid_entry_type);
            } else
		(*(V0_0_EPV(sech).rdacl_replace))
                    (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
		     sec_acl_type, sec_acl_list_p, st_p, &rpc_status);
        } else {
            (*(V1_0_EPV(sech).rdacl_replace))
                (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
		 sec_acl_type, sec_acl_list_p, st_p, &rpc_status);
	}
    } else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    if (IS_DFS_ACL(sech)) {
        if (sec_acl_list_p->num_acls > 0) {
            *st_p = dfs_dce_acl_replace((char *)COMPONENT_NAME(sech),
                                        sec_acl_type,
                                        sec_acl_list_p->sec_acls[0] );
        }
    } else while (! done && sec__acl_check_binding(sech, st_p)) {
        if (IF_VERSION(sech)==0) {
            if (sec__acl_new_types(sec_acl_list_p)) {
                SET_STATUS(st_p, sec_acl_invalid_entry_type);
            } else
                (*rdaclif_v0_0_c_epv.rdacl_replace)
                    (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
                        sec_acl_type, sec_acl_list_p, st_p, &rpc_status);
        } else {
            (*rdaclif_v1_0_c_epv.rdacl_replace)
                (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
                    sec_acl_type, sec_acl_list_p, st_p, &rpc_status);
        }

            /* On communications failure, rebind if possible */
        if (! GOOD_STATUS(&rpc_status)) {
            SET_STATUS(st_p, rpc_status);
            HANDLE_VALID(sech) = false;

            /* or if site is readonly, rebind to update site */
        } else if (STATUS_EQUAL(st_p, sec_acl_site_read_only)) {
            sec__acl_rebind_update(sech, manager_type_p, sec_acl_type, st_p);
            if (! GOOD_STATUS(st_p))
                done = true;

            /* otherwise, it must have succeeded */
        } else {
            done = true;
        }
    }
}

PUBLIC void sec_acl_get_access
#ifndef __STDC__
    (sech, manager_type_p, net_rights, st_p)
    sec_acl_handle_t        sech;
    uuid_t                  *manager_type_p;
    sec_acl_permset_t       *net_rights;
    error_status_t          *st_p;
#else
  (
    sec_acl_handle_t        sech,
    uuid_t                  *manager_type_p,
    sec_acl_permset_t       *net_rights,
    error_status_t          *st_p
  )
#endif
{
    error_status_t      rpc_status;
    boolean32           done = false;
    sec_acl_p_t         sec_aclp = NULL;

    void (*free_func) PROTOTYPE((void *ptr));
    void *(*alloc_func) PROTOTYPE((unsigned long size));

    if (!sech) {
        SET_STATUS(st_p, sec_acl_invalid_acl_handle);
        return;
    }

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    if (IS_LOCAL_ACL(sech)) {
        if (IF_VERSION(sech)==0) {
	    (*(V0_0_EPV(sech).rdacl_get_access))
                (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,net_rights,
		 st_p, &rpc_status);
        } else {
            (*(V1_0_EPV(sech).rdacl_get_access))
                (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,net_rights,
		 st_p, &rpc_status);
	}
    } else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    if (IS_DFS_ACL(sech)) {
        /* get the current RPC memory alloc/dealloc routines and reenable the
           previous RPC memory alloc/dealloc routines  */
        rpc_sm_swap_client_alloc_free((idl_void_p_t (*)(idl_size_t)) malloc,
					free, &alloc_func, &free_func, st_p);
        if (GOOD_STATUS(st_p)) {
           rpc_sm_set_client_alloc_free(alloc_func, free_func, st_p);
        }
        if (!GOOD_STATUS(st_p)) {
            SET_STATUS(st_p, sec_acl_cant_allocate_memory);
	    return;
	}
	sec_aclp = (*alloc_func)(sizeof(sec_acl_t));
        if (sec_aclp == NULL) {
            SET_STATUS(st_p, sec_acl_cant_allocate_memory);
	    return;
	}
        *st_p = dfs_dce_acl_lookup((char *)COMPONENT_NAME(sech),
                                    sec_acl_type_object, sec_aclp);
        if (GOOD_STATUS(st_p)) {
            (void) dfs_dce_acl_CheckAccessAllowedPac(
                                                (char *) COMPONENT_NAME(sech),
                                                sec_aclp, NULL, net_rights,
                                                st_p);
        }
        sec_acl_release( sech, sec_aclp, st_p );
    } else while (!done && sec__acl_check_binding(sech, st_p)) {
        if (IF_VERSION(sech)==0) {
            (*rdaclif_v0_0_c_epv.rdacl_get_access)
                (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,net_rights,
                    st_p, &rpc_status);
        } else {
            (*rdaclif_v1_0_c_epv.rdacl_get_access)
                (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,net_rights,
                    st_p, &rpc_status);
        }
        if (! GOOD_STATUS(&rpc_status)) {
            SET_STATUS(st_p, rpc_status);
            HANDLE_VALID(sech) = false;
        } else {
            done = true;
        }
    }
}




PUBLIC boolean32 sec_acl_test_access
#ifndef __STDC__
    (sech, manager_type_p, permset, st_p)
    sec_acl_handle_t        sech;
    uuid_t                  *manager_type_p;
    sec_acl_permset_t       permset;
    error_status_t          *st_p;
#else
  (
    sec_acl_handle_t        sech,
    uuid_t                  *manager_type_p,
    sec_acl_permset_t       permset,
    error_status_t          *st_p
  )
#endif
{
    error_status_t      rpc_status;
    boolean32           test_access = false, done = false;
    sec_acl_permset_t   granted;
    sec_acl_p_t         sec_aclp;

    void (*free_func) PROTOTYPE((void *ptr));
    void *(*alloc_func) PROTOTYPE((unsigned long size));

    if (!sech) {
        SET_STATUS(st_p, sec_acl_invalid_acl_handle);
        return false;
    }

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    if (IS_LOCAL_ACL(sech)) {
        if (IF_VERSION(sech)==0) {
            test_access =
		(*(V0_0_EPV(sech).rdacl_test_access))
		(HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
		 permset, st_p, &rpc_status);
        } else {
            test_access =
		(*(V1_0_EPV(sech).rdacl_test_access))
		(HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
		 permset, st_p, &rpc_status);
	}
    } else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    if (IS_DFS_ACL(sech)) {
        /* get the current RPC memory alloc/dealloc routines and reenable the
           previous RPC memory alloc/dealloc routines  */
        rpc_sm_swap_client_alloc_free((idl_void_p_t (*)(idl_size_t)) malloc,
					free, &alloc_func, &free_func, st_p);
        if (GOOD_STATUS(st_p)) {
           rpc_sm_set_client_alloc_free(alloc_func, free_func, st_p);
        }
        if (!GOOD_STATUS(st_p)) {
            SET_STATUS(st_p, sec_acl_cant_allocate_memory);
	    return false;
	}
	sec_aclp = (*alloc_func)(sizeof(sec_acl_t));
        if (sec_aclp == NULL) {
            SET_STATUS(st_p, sec_acl_cant_allocate_memory);
	    return false;
	}
        *st_p = dfs_dce_acl_lookup((char *)COMPONENT_NAME(sech),
                                    sec_acl_type_object, sec_aclp);
        if (GOOD_STATUS(st_p)) {
            test_access = dfs_dce_acl_CheckAccessAllowedPac(
                                                (char *) COMPONENT_NAME(sech),
                                                sec_aclp, &permset, &granted,
                                                st_p);
        }
        sec_acl_release( sech, sec_aclp, st_p );
    } else while (!done && sec__acl_check_binding(sech, st_p)) {
        if (IF_VERSION(sech)==0) {
            test_access =
                (*rdaclif_v0_0_c_epv.rdacl_test_access)
                    (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
                        permset, st_p, &rpc_status);
        } else {
            test_access =
                (*rdaclif_v1_0_c_epv.rdacl_test_access)
                    (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
                        permset, st_p, &rpc_status);
        }
        if (! GOOD_STATUS(&rpc_status)) {
            SET_STATUS(st_p, rpc_status);
            HANDLE_VALID(sech) = false;
        } else {
            done = true;
        }
    }

    return test_access;
}


PUBLIC boolean32 sec_acl_test_access_on_behalf
#ifndef __STDC__
    (sech, manager_type_p, subject_p, desired_permset, st_p)
    sec_acl_handle_t        sech;
    uuid_t                  *manager_type_p;
    sec_id_pac_t            *subject_p;
    sec_acl_permset_t       desired_permset;
    error_status_t          *st_p;
#else
  (
    sec_acl_handle_t        sech,
    uuid_t                  *manager_type_p,
    sec_id_pac_t            *subject_p,
    sec_acl_permset_t       desired_permset,
    error_status_t          *st_p
  )
#endif
{
    error_status_t      rpc_status;
    boolean32           test_access = false, done = false;

    if (!sech) {
        SET_STATUS(st_p, sec_acl_invalid_acl_handle);
        return false;
    }

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    if (IS_LOCAL_ACL(sech)) {
        if (IF_VERSION(sech)==0) {
            test_access =
		(*(V0_0_EPV(sech).rdacl_test_access_on_behalf))
		(HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
		 subject_p, desired_permset, st_p, &rpc_status);
        } else {
            test_access =
		(*(V1_0_EPV(sech).rdacl_test_access_on_behalf))
		(HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
		 subject_p, desired_permset, st_p, &rpc_status);
	}
    } else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    if (IS_DFS_ACL(sech)) {
        SET_STATUS(st_p, sec_acl_not_implemented);
    } else while (!done && sec__acl_check_binding(sech, st_p)) {
        if (IF_VERSION(sech)==0) {
            test_access =
                (*rdaclif_v0_0_c_epv.rdacl_test_access_on_behalf)
                    (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
                        subject_p, desired_permset, st_p, &rpc_status);
        } else {
            test_access =
                (*rdaclif_v1_0_c_epv.rdacl_test_access_on_behalf)
                    (HANDLE(sech), COMPONENT_NAME(sech), manager_type_p,
                        subject_p, desired_permset, st_p, &rpc_status);
        }
        if (! GOOD_STATUS(&rpc_status)) {
            SET_STATUS(st_p, rpc_status);
            HANDLE_VALID(sech) = false;
        } else {
            done = true;
        }
    }

    return test_access;
}


PUBLIC void sec_acl_get_manager_types
#ifndef __STDC__
    (sech, acl_type, size_avail, size_used_p, num_types_p, manager_types, st_p)
    sec_acl_handle_t        sech;
    sec_acl_type_t          acl_type;
    unsigned32              size_avail;
    unsigned32              *size_used_p;
    unsigned32              *num_types_p;
    uuid_t                  manager_types[];
    error_status_t          *st_p;
#else
  (
    sec_acl_handle_t        sech,
    sec_acl_type_t          acl_type,
    unsigned32              size_avail,
    unsigned32              *size_used_p,
    unsigned32              *num_types_p,
    uuid_t                  manager_types[],
    error_status_t          *st_p
  )
#endif
{
    error_status_t      rpc_status;
    boolean32           done = false;

    if (!sech) {
        SET_STATUS(st_p, sec_acl_invalid_acl_handle);
        return;
    }

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    if (IS_LOCAL_ACL(sech)) {
        if (IF_VERSION(sech)==0) {
	    (*(V0_0_EPV(sech).rdacl_get_manager_types))
                (HANDLE(sech), COMPONENT_NAME(sech), acl_type, size_avail,
		 size_used_p, num_types_p, manager_types, st_p, &rpc_status);
        } else {
            (*(V1_0_EPV(sech).rdacl_get_manager_types))
                (HANDLE(sech), COMPONENT_NAME(sech), acl_type, size_avail,
		 size_used_p, num_types_p, manager_types, st_p, &rpc_status);
	}
    } else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    if (IS_DFS_ACL(sech)) {
        dfs_dce_acl_get_manager_types(acl_type, size_avail, size_used_p,
                                        num_types_p, manager_types, st_p);
    } else while (!done && sec__acl_check_binding(sech, st_p)) {
        if (IF_VERSION(sech)==0) {
            (*rdaclif_v0_0_c_epv.rdacl_get_manager_types)
                (HANDLE(sech), COMPONENT_NAME(sech), acl_type, size_avail,
                    size_used_p, num_types_p, manager_types, st_p,&rpc_status);
        } else {
            (*rdaclif_v1_0_c_epv.rdacl_get_manager_types)
                (HANDLE(sech), COMPONENT_NAME(sech), acl_type, size_avail,
                    size_used_p, num_types_p, manager_types, st_p,&rpc_status);
        }
        if (! GOOD_STATUS(&rpc_status)) {
            SET_STATUS(st_p, rpc_status);
            HANDLE_VALID(sech) = false;
        } else if ((BAD_STATUS(st_p)) && (! REBOUND_YET(sech))) {
            HANDLE_VALID(sech) = false;
        } else {
            done = true;
        }
    }
}


PUBLIC void sec_acl_get_mgr_types_semantics
#ifndef __STDC__
    (sech, acl_type, size_avail, size_used_p, num_types_p, manager_types,
     posix_semantics, st_p)
    sec_acl_handle_t            sech;
    sec_acl_type_t              acl_type;
    unsigned32                  size_avail;
    unsigned32                  *size_used_p;
    unsigned32                  *num_types_p;
    uuid_t                      manager_types[];
    sec_acl_posix_semantics_t   posix_semantics[];
    error_status_t              *st_p;
#else
  (
    sec_acl_handle_t            sech,
    sec_acl_type_t              acl_type,
    unsigned32                  size_avail,
    unsigned32                  *size_used_p,
    unsigned32                  *num_types_p,
    uuid_t                      manager_types[],
    sec_acl_posix_semantics_t   posix_semantics[],
    error_status_t              *st_p
  )
#endif
{
    error_status_t      rpc_status;
    boolean32           done = false;

    if (!sech) {
        SET_STATUS(st_p, sec_acl_invalid_acl_handle);
        return;
    }

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    if (IS_LOCAL_ACL(sech)) {
        if (IF_VERSION(sech)==0) {
	    (*(V0_0_EPV(sech).rdacl_get_mgr_types_semantics))
                (HANDLE(sech), COMPONENT_NAME(sech), acl_type, size_avail,
		 size_used_p, num_types_p, manager_types, posix_semantics,
		 st_p, &rpc_status);
        } else {
            (*(V1_0_EPV(sech).rdacl_get_mgr_types_semantics))
                (HANDLE(sech), COMPONENT_NAME(sech), acl_type, size_avail,
		 size_used_p, num_types_p, manager_types, posix_semantics,
		 st_p, &rpc_status);
	}
    } else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    if (IS_DFS_ACL(sech)) {
        dfs_dce_acl_get_mgr_types_semantics(acl_type, size_avail, size_used_p,
                                            num_types_p, manager_types,
                                            posix_semantics, st_p);
    } else while (!done && sec__acl_check_binding(sech, st_p)) {
        if (IF_VERSION(sech)==0) {
            (*rdaclif_v0_0_c_epv.rdacl_get_mgr_types_semantics)
                (HANDLE(sech), COMPONENT_NAME(sech), acl_type, size_avail,
                    size_used_p, num_types_p, manager_types, posix_semantics,
                    st_p, &rpc_status);
        } else {
            (*rdaclif_v1_0_c_epv.rdacl_get_mgr_types_semantics)
                (HANDLE(sech), COMPONENT_NAME(sech), acl_type, size_avail,
                    size_used_p, num_types_p, manager_types, posix_semantics,
                    st_p, &rpc_status);
        }
        if (! GOOD_STATUS(&rpc_status)) {
            SET_STATUS(st_p, rpc_status);
            if (STATUS_EQUAL(st_p, rpc_s_op_rng_error)) {
                done = true;
            } else {
                HANDLE_VALID(sech) = false;
            }
        } else if ((BAD_STATUS(st_p)) && (! REBOUND_YET(sech))) {
            HANDLE_VALID(sech) = false;
        } else {
            done = true;
        }
    }
}


PUBLIC void sec_acl_get_printstring
#ifndef __STDC__
    (sech, manager_type_p, size_avail,
                                manager_type_chain_p, manager_info_p,
                                tokenize_p, total_num_printstrings_p,
                                size_used_p, printstrings, st_p)
    sec_acl_handle_t        sech;
    uuid_t                  *manager_type_p;
    unsigned32              size_avail;
    uuid_t                  *manager_type_chain_p;
    sec_acl_printstring_t   *manager_info_p;
    boolean32               *tokenize_p;
    unsigned32              *total_num_printstrings_p;
    unsigned32              *size_used_p;
    sec_acl_printstring_t   *printstrings;
    error_status_t          *st_p;
#else
  (
    sec_acl_handle_t        sech,
    uuid_t                  *manager_type_p,
    unsigned32              size_avail,
    uuid_t                  *manager_type_chain_p,
    sec_acl_printstring_t   *manager_info_p,
    boolean32               *tokenize_p,
    unsigned32              *total_num_printstrings_p,
    unsigned32              *size_used_p,
    sec_acl_printstring_t   *printstrings,
    error_status_t          *st_p
  )
#endif
{
    error_status_t      rpc_status;
    boolean32           done = false;

    if (!sech) {
        SET_STATUS(st_p, sec_acl_invalid_acl_handle);
        return;
    }

#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    if (IS_LOCAL_ACL(sech)) {
        if (IF_VERSION(sech)==0) {
	    (*(V0_0_EPV(sech).rdacl_get_printstring))
                (HANDLE(sech), manager_type_p, size_avail,
		 manager_type_chain_p, manager_info_p,
		 tokenize_p, total_num_printstrings_p, size_used_p,
		 printstrings, st_p, &rpc_status);
        } else {
            (*(V1_0_EPV(sech).rdacl_get_printstring))
                (HANDLE(sech), manager_type_p, size_avail,
		 manager_type_chain_p, manager_info_p,
		 tokenize_p, total_num_printstrings_p, size_used_p,
		 printstrings, st_p, &rpc_status);
	}
    } else
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
    if (IS_DFS_ACL(sech)) {
        dfs_dce_acl_get_printstring(manager_type_p, size_avail,
                                    manager_type_chain_p, manager_info_p,
                                    tokenize_p, total_num_printstrings_p,
                                    size_used_p, printstrings, st_p);
    } else while (!done && sec__acl_check_binding(sech, st_p)) {
        if (IF_VERSION(sech)==0) {
            (*rdaclif_v0_0_c_epv.rdacl_get_printstring)
                (HANDLE(sech), manager_type_p, size_avail,
                    manager_type_chain_p, manager_info_p,
                    tokenize_p, total_num_printstrings_p, size_used_p,
                    printstrings, st_p, &rpc_status);
        } else {
            (*rdaclif_v1_0_c_epv.rdacl_get_printstring)
                (HANDLE(sech), manager_type_p, size_avail,
                    manager_type_chain_p, manager_info_p,
                    tokenize_p, total_num_printstrings_p, size_used_p,
                    printstrings, st_p, &rpc_status);
        }
        if (! GOOD_STATUS(&rpc_status)) {
            SET_STATUS(st_p, rpc_status);
            HANDLE_VALID(sech) = false;
        } else {
            done = true;
        }
    }
}


PUBLIC void sec_acl_release
#ifndef __STDC__
    (sech, sec_acl_p, st_p)
    sec_acl_handle_t    sech;
    sec_acl_t           *sec_acl_p;
    error_status_t      *st_p;
#else
  (
    sec_acl_handle_t    sech,
    sec_acl_t           *sec_acl_p,
    error_status_t      *st_p
  )
#endif
{
    sec_acl_entry_t *entry_p;
    int i;
    void (*free_func) PROTOTYPE((void *ptr));
    void *(*alloc_func) PROTOTYPE((unsigned long size));

    rpc_sm_swap_client_alloc_free((idl_void_p_t (*)(idl_size_t)) malloc,
				free, &alloc_func, &free_func, st_p);
    if (GOOD_STATUS(st_p)) {
        rpc_sm_set_client_alloc_free(alloc_func, free_func, st_p);
    }
    if (!GOOD_STATUS(st_p)) {
        SET_STATUS(st_p, sec_acl_cant_allocate_memory);
        return;
    }
    CLEAR_STATUS(st_p);
    if (sec_acl_p != NULL) {
        if (sec_acl_p->default_realm.name)
            (*free_func)((char *)sec_acl_p->default_realm.name);

        if (sec_acl_p->sec_acl_entries) {
            for (i = 0; i < sec_acl_p->num_entries; i++) {
                entry_p = &(sec_acl_p->sec_acl_entries[i]);
                if (entry_p) {
                    switch (entry_p->entry_info.entry_type)  {
                        case sec_acl_e_type_user:
                        case sec_acl_e_type_group:
                        case sec_acl_e_type_foreign_other:
                            if (entry_p->entry_info.tagged_union.id.name)
                                (*free_func)((char *)entry_p->entry_info.tagged_union.id.name);
                            break;

                        case sec_acl_e_type_foreign_user:
                        case sec_acl_e_type_foreign_group:
                            if (entry_p->entry_info.tagged_union.foreign_id.id.name)
                                (*free_func)((char *)entry_p->entry_info.tagged_union.foreign_id.id.name);
                            if (entry_p->entry_info.tagged_union.foreign_id.realm.name)
                                (*free_func)((char *)entry_p->entry_info.tagged_union.foreign_id.realm.name);
                            break;

                        case sec_acl_e_type_extended:
                            if (entry_p->entry_info.tagged_union.extended_info)
                                (*free_func)((char *)entry_p->entry_info.tagged_union.extended_info);
                            break;

                        default:
                            break;
                    }   /* switch entry_type */
                } /* if entry_p */
            }   /* for loop */

            /* Free the array of entries */
            (*free_func)((char *) sec_acl_p->sec_acl_entries);

        } /* if sec_acl_entries is non-null */

        (*free_func)((char *) sec_acl_p);
    }
}


PUBLIC error_status_t sec_acl_get_error_info
#ifndef __STDC__
    (sech)
    sec_acl_handle_t    sech;
#else
  (
    sec_acl_handle_t    sech
  )
#endif
{
    if (sech)
        return(LAST_ERROR(sech));
    else
        return(sec_acl_invalid_acl_handle);
}

PUBLIC void sec_acl_release_handle
#ifndef __STDC__
    (h, st_p)
    sec_acl_handle_t    *h;
    error_status_t      *st_p;
#else
  (
    sec_acl_handle_t    *h,
    error_status_t      *st_p
  )
#endif
{
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    if (h == NULL || *h == (sec_acl_handle_t)NULL) {
#else
    if (*h == (sec_acl_handle_t)NULL || !h) {
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
        SET_STATUS(st_p, sec_acl_invalid_acl_handle);
        return;
    }

    if (COMPONENT_NAME(*h) != (unsigned_char_p_t)NULL) {
        free(COMPONENT_NAME(*h));
    }
    if (RESOLVED_NAME(*h) != (unsigned_char_p_t)NULL) {
        free(RESOLVED_NAME(*h));
    }
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
    if (IS_LOCAL_ACL(*h) || IS_DFS_ACL(*h)) {
#else
    if (IS_DFS_ACL(*h)) {
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
        free(*h);
        *h = NULL;
    } else {
	if (HANDLE(*h) != (sec_acl_handle_t)NULL)
	    rpc_binding_free(&(HANDLE(*h)), st_p);
        if (IMPORT_HANDLE_VALID(*h))
            rpc_ns_binding_import_done(&(IMPORT_HANDLE(*h)), st_p);
        if (HANDLE_STR(*h) != (unsigned_char_p_t) NULL)
            free((char *) HANDLE_STR(*h));
        free(*h);
        *h = NULL;
    }
}


PUBLIC void sec_acl_calc_mask
#ifndef __STDC__
    (sec_acl_list_p, st_p)
    sec_acl_list_t          *sec_acl_list_p;
    error_status_t          *st_p;
#else
  (
    sec_acl_list_t          *sec_acl_list_p,
    error_status_t          *st_p
  )
#endif
{
    int                 i, j, num, mask;
    sec_acl_permset_t   perms;
    sec_acl_entry_t     *new_sec_acl_entries;
    CLEAR_STATUS(st_p);

    /* For each acl in the list */
    for (i = 0; i < sec_acl_list_p->num_acls; i++) {
        perms = 0;
        mask  = -1;
        /* For each entry in this acl */
        for (j = 0; j < sec_acl_list_p->sec_acls[i]->num_entries; j++) {
            switch(sec_acl_list_p->sec_acls[i]->sec_acl_entries[j].entry_info.entry_type) {
                case sec_acl_e_type_mask_obj:
                    mask = j;
                    break;
                case sec_acl_e_type_group_obj:
                case sec_acl_e_type_any_other:
                case sec_acl_e_type_user:
                case sec_acl_e_type_group:
                case sec_acl_e_type_foreign_other:
                case sec_acl_e_type_foreign_user:
                case sec_acl_e_type_foreign_group:
                case sec_acl_e_type_extended:
                    perms = perms |
                        sec_acl_list_p->sec_acls[i]->sec_acl_entries[j].perms;
                    break;
                case sec_acl_e_type_user_obj:
                case sec_acl_e_type_other_obj:
                case sec_acl_e_type_unauthenticated:
                default:
                    break;
            }
        }
            /* If we found a mask, just replace the perms */
        if (mask != -1) {
            sec_acl_list_p->sec_acls[i]->sec_acl_entries[mask].perms = perms;
        } else {
            /* Otherwise, create a new entry for the mask,
             * which implies creating a new list of entries
             * with enough room for the new mask entry.
             */
            new_sec_acl_entries = (sec_acl_entry_t *)
                malloc((sec_acl_list_p->sec_acls[i]->num_entries + 1) *
                        sizeof(sec_acl_entry_t));
#ifndef	HPDCE_ACLMGR_LOCAL_BIND
	    if (new_sec_acl_entries == NULL) {
                SET_STATUS(st_p, sec_acl_cant_allocate_memory);
		return;
	    }
#endif	/* HPDCE_ACLMGR_LOCAL_BIND */
            /* Copy the existing entries */
            for (j = 0; j < sec_acl_list_p->sec_acls[i]->num_entries; j++) {
                new_sec_acl_entries[j] =
                    sec_acl_list_p->sec_acls[i]->sec_acl_entries[j];
            }
            if (sec_acl_list_p->sec_acls[i]->num_entries > 0) {
                free((char *) sec_acl_list_p->sec_acls[i]->sec_acl_entries);
            }
            /* Add the new entry */
            num = ++sec_acl_list_p->sec_acls[i]->num_entries;
            new_sec_acl_entries[num-1].perms = perms;
            new_sec_acl_entries[num-1].entry_info.entry_type =
                sec_acl_e_type_mask_obj;
            sec_acl_list_p->sec_acls[i]->sec_acl_entries = new_sec_acl_entries;
        }
    }
}
