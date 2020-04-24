/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: deif.c,v $
 * Revision 1.1.14.2  1996/03/11  13:26:07  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:11  marty]
 *
 * Revision 1.1.14.1  1995/12/08  17:55:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:28:30  root]
 * 
 * Revision 1.1.10.6  1994/09/13  21:10:20  desai
 * 	Temporary fix for CR 12131. Adding strlen of buffer in a call to get the size.
 * 	[1994/09/13  20:23:17  desai]
 * 
 * Revision 1.1.10.5  1994/09/09  20:41:01  kline_s
 * 	OT 12028: I18N code cleanup for Security.
 * 	[1994/09/09  20:28:31  kline_s]
 * 
 * Revision 1.1.10.4  1994/06/02  21:57:18  mdf
 * 	Merged with changes from 1.1.10.3
 * 	[1994/06/02  21:53:32  mdf]
 * 
 * 	merge from secd_auth. (burati)
 * 	[1994/05/24  15:13:21  mdf]
 * 
 * 	merge from secd_auth. (burati)
 * 
 * Revision 1.1.10.3  1994/06/02  20:20:30  mdf
 * 	merge from secd_auth. (burati)
 * 	[1994/05/24  15:13:21  mdf]
 * 
 * 	merge from secd_auth. (burati)
 * 
 * Revision 1.1.10.1  1994/01/28  23:10:04  burati
 * 	CR9841 Fix error text for foreign groups in deif_print_sec_acl_entry
 * 	CR9848 In deif_get_extended_info don't process entry if it's a NULL str
 * 	[1994/01/28  19:45:17  burati]
 * 
 * 	Delegation changes - New acl entry types support (dlg_bl1)
 * 	[1994/01/19  20:46:44  burati]
 * 
 * Revision 1.1.8.2  1993/11/03  22:49:16  burati
 * 	CR9348 Don't dereference NULL string (key) in deif_get_key or in
 * 	deif_validate_sec_acl_entry. "delete foreign_user" was causing coredump.
 * 	[1993/11/03  22:30:27  burati]
 * 
 * Revision 1.1.8.1  1993/10/12  14:11:57  burati
 * 	CR9086 Only replace ACL if one exists.
 * 	[1993/10/11  22:04:26  burati]
 * 
 * Revision 1.1.6.2  1993/08/02  19:53:20  hanfei
 * 	fix for OT#7727 : deif_do_delete() copies data down from area beyond
 * 	sec_acl_entries[] boundary
 * 	[1993/08/02  19:51:17  hanfei]
 * 
 * Revision 1.1.4.7  1993/01/28  18:44:59  burati
 * 	CR6655 was recalculating mask on non-file-group-class entries
 * 	[1993/01/27  22:12:10  burati]
 * 
 * Revision 1.1.4.6  1992/12/29  12:36:51  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:32:47  zeliff]
 * 
 * Revision 1.1.4.5  1992/12/17  19:45:26  burati
 * 	CR5872 NUM_TYPES should be 12, not 13
 * 	[1992/12/17  19:30:14  burati]
 * 
 * Revision 1.1.4.4  1992/11/16  18:29:19  burati
 * 	CR4812 allow unauthenticated binding
 * 	[1992/11/14  00:39:36  burati]
 * 
 * Revision 1.1.4.3  1992/10/21  19:40:59  burati
 * 	CR5744 Typo in function deif_entry_compare
 * 	[1992/10/21  19:12:01  burati]
 * 
 * Revision 1.1.4.2  1992/10/08  21:03:28  burati
 * 	CR4771 The empty string is not a valid entry key
 * 	[1992/09/24  21:45:00  burati]
 * 
 * Revision 1.1.2.6  1992/07/09  19:46:45  burati
 * 	CR4161 DFS ACL work
 * 	[1992/06/18  21:42:14  burati]
 * 
 * Revision 1.1.2.5  1992/06/15  22:24:23  burati
 * 	CR4205 Remove incorrect status check @ start of deif_do_get_access
 * 	[1992/06/15  21:35:46  burati]
 * 
 * Revision 1.1.2.4  1992/05/20  19:23:33  burati
 * 	Also make sure def_cell_valid gets set when obj has no ACL.
 * 	[1992/05/20  18:55:02  burati]
 * 
 * 	Make sure def_cell is defined even if obj has no ACL yet.
 * 	[1992/05/20  16:24:09  burati]
 * 
 * Revision 1.1.2.3  1992/05/08  15:26:49  burati
 * 	Add new functionality to support POSIX mask calculation
 * 	[1992/05/06  22:21:29  burati]
 * 
 * 	    Use new sec_acl_get_mgr_types_semantics call.
 * 	[1992/04/29  03:12:27  burati]
 * 
 * Revision 1.1.2.2  1992/01/22  23:04:36  melman
 * 	    Adding changes from defect_fixes archive.
 * 	    [1992/01/22  22:21:06  melman]
 * 
 * $EndLog$
 */

/*  deif.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1993, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
**      sec_acl editor generic interface routines
**
**  This implementation does not yet make use of manager type chains,
**  or multiple managers per object that are available through the API,
**  but nothing here should prevent someone from doing so once there
**  are ACL managers that require multiple manager types per object.
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)deif.c    23 - 01/06/92";
#endif

#include <un_maloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <ctype.h>
#include <macros.h>
#include <deif.h>
#include <un_maloc.h>

#include <dce/uuid.h>
#include <dce/rpc.h>
#include <dce/binding.h>
#include <dce/daclif.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>

#define DEF_CELL_PFX     "# Default cell = "
#define DEF_CELL_PFX_LEN 17

extern boolean32 verbose;

/*  Private cache of acl information that needs to be retrieved once,
 *  but may be used many times by deif_ routines during an invokation
 *  of dacl_edit.  It is cached here for efficiency purposes, and
 *  because the UI routines may be called from an external package
 *  which cannot know about this internal state, so they can't be
 *  passing it around.
 */
PRIVATE sec_acl_handle_t        sec_acl_handle;
PRIVATE char                    sec_acl_entry_name[BUFSIZ];
PRIVATE sec_rgy_name_t          def_cell_name, my_cell_name;
PRIVATE uuid_t                  sec_acl_mgr_type, my_cell_id, def_cell_id;
PRIVATE sec_acl_printstring_t   sec_acl_printstrings[32];
PRIVATE int                     num_printstrings;
PRIVATE sec_acl_t               sec_acl, *sec_acl_p;
PRIVATE sec_acl_list_t          sec_acl_list;
PRIVATE sec_acl_type_t          sec_acl_type;
PRIVATE boolean32               acl_exists, def_cell_valid = false;
PRIVATE sec_rgy_handle_t        rgy_h, def_cell_rgy_h;

/*  Table of entry types to match ascii strings with equivalent types
 *  Note: Last entry is NULL, to end the loop that checks a given
 *  string against all known types for a match.
 */
PRIVATE sec_acl_entry_type_table_t entry_types[] = {
    { "mask_obj",       sec_acl_e_type_mask_obj         },
    { "user_obj",       sec_acl_e_type_user_obj         },
    { "group_obj",      sec_acl_e_type_group_obj        },
    { "other_obj",      sec_acl_e_type_other_obj        },
    { "unauthenticated",sec_acl_e_type_unauthenticated  },
    { "user",           sec_acl_e_type_user             },
    { "group",          sec_acl_e_type_group            },
    { "foreign_other",  sec_acl_e_type_foreign_other    },
    { "any_other",      sec_acl_e_type_any_other        },
    { "foreign_user",   sec_acl_e_type_foreign_user     },
    { "foreign_group",  sec_acl_e_type_foreign_group    },
    { "extended",       sec_acl_e_type_extended         },
    { "user_obj_delegate",      sec_acl_e_type_user_obj_deleg         },
    { "group_obj_delegate",     sec_acl_e_type_group_obj_deleg        },
    { "user_delegate",          sec_acl_e_type_user_deleg             },
    { "group_delegate",         sec_acl_e_type_group_deleg            },
    { "other_obj_delegate",     sec_acl_e_type_other_obj_deleg        },
    { "foreign_other_delegate", sec_acl_e_type_for_other_deleg        },
    { "any_other_delegate",     sec_acl_e_type_any_other_deleg        },
    { "foreign_user_delegate",  sec_acl_e_type_for_user_deleg         },
    { "foreign_group_delegate", sec_acl_e_type_for_group_deleg        },
    { NULL,             sec_acl_e_type_extended         }
};

PRIVATE void make_lowercase(
    char    *buffer
);


/*
 * d e i f _ e n t r y _ c o m p a r e
 *
 * Compare two sec_acl_entry_t's
 * If key_only is true, compare only the entry_type and
 * the id (if the type has a corresponding id key).  When
 * key_only is false, then also compare the permsets.
 */
PRIVATE boolean32 deif_entry_compare (
    sec_acl_entry_t *e1,
    sec_acl_entry_t *e2,
    boolean32       key_only
)
{
    error_status_t  st;
    boolean32       equiv = false;

    /* Compare the types first */
    if (e1->entry_info.entry_type == e2->entry_info.entry_type)
        equiv = true;

    /* If the above match, compare any extra data (id or foreign id) */
    if (equiv) {
        switch (e1->entry_info.entry_type) {
                break;

            case sec_acl_e_type_user:
            case sec_acl_e_type_group:
            case sec_acl_e_type_foreign_other:
            case sec_acl_e_type_user_deleg:
            case sec_acl_e_type_group_deleg:
            case sec_acl_e_type_for_other_deleg:
                if ( ! uuid_equal(&e1->entry_info.tagged_union.id.uuid,
                                &e2->entry_info.tagged_union.id.uuid, &st) )
                    equiv = false;

                break;
            case sec_acl_e_type_foreign_user:
            case sec_acl_e_type_foreign_group:
            case sec_acl_e_type_for_user_deleg:
            case sec_acl_e_type_for_group_deleg:
                if ( ! uuid_equal(
                            &e1->entry_info.tagged_union.foreign_id.id.uuid,
                            &e2->entry_info.tagged_union.foreign_id.id.uuid,
                            &st) )
                    equiv = false;
                break;
            case sec_acl_e_type_mask_obj:
            case sec_acl_e_type_user_obj:
            case sec_acl_e_type_group_obj:
            case sec_acl_e_type_user_obj_deleg:
            case sec_acl_e_type_group_obj_deleg:
            case sec_acl_e_type_other_obj:
            case sec_acl_e_type_other_obj_deleg:
            case sec_acl_e_type_unauthenticated:
            case sec_acl_e_type_extended:
            case sec_acl_e_type_any_other:
            case sec_acl_e_type_any_other_deleg:
            default:
                /* No extra data for these types need be compared */
                break;
        }
    }
    if (equiv && ! key_only) {
      if (e1->perms != e2->perms) {
	equiv = false;
      }
    }

    return equiv;
}


/*
 * d e i f _ p a r s e _ f o r e i g n _ i d
 */
PRIVATE error_status_t deif_parse_foreign_id (
    char                    *global_name,
    sec_rgy_name_t          princ_name,
    uuid_t                  *princ_idp,
    sec_rgy_name_t          cell_name,
    uuid_t                  *cell_idp,
    sec_acl_entry_type_t    entry_type
)
{
    error_status_t      st;

    if (global_name == NULL)
        SET_STATUS(&st, deif_err_invalid_key);
    else {
        switch (entry_type) {
            case sec_acl_e_type_foreign_user:
            case sec_acl_e_type_for_user_deleg:
                sec_id_parse_name(rgy_h, (unsigned_char_p_t) global_name,
                                    cell_name, cell_idp, princ_name,
                                    princ_idp, &st);
                break;
            case sec_acl_e_type_foreign_group:
            case sec_acl_e_type_for_group_deleg:
                sec_id_parse_group(rgy_h, (unsigned_char_p_t) global_name,
                                    cell_name, cell_idp, princ_name,
                                    princ_idp, &st);
                break;
        }
    }

    return st;
}


/*
 * d e i f _ g e t _ k e y
 */
PRIVATE error_status_t deif_get_key (
    sec_acl_entry_t         *sec_acl_entry_p,
    sec_acl_entry_type_t    entry_type,
    char                    *string
)
{
    error_status_t      st, lst;
    char                *cell;
    sec_rgy_cursor_t    cursor;
    sec_rgy_pgo_item_t  pgo_item;
    sec_rgy_name_t      cell_name, princ_name, global_name;
    uuid_t              cell_id, princ_id, dummy_id;
    unsigned_char_p_t   uuid_string;

    CLEAR_STATUS(&st);

    /* The empty string is not a valid key */
    if ((string == NULL) || (*string == '\0')) {
	SET_STATUS(&st, deif_err_invalid_entry_key);
    } else switch(entry_type) {
        case sec_acl_e_type_user:
        case sec_acl_e_type_user_deleg:
            sec_acl_entry_p->entry_info.tagged_union.id.name =
                (unsigned_char_p_t)
                    malloc((strlen(string)+1) * sizeof(char));
            strcpy((char *) sec_acl_entry_p->entry_info.tagged_union.id.name,
                    string);
            sprintf((char *)global_name, "%s/%s", def_cell_name, string);
            sec_id_parse_name(def_cell_rgy_h, global_name, NULL, &dummy_id,
                                princ_name, &princ_id, &st);
            if (GOOD_STATUS(&st)) {
                if (getenv("DEBUG_ACL_EDIT")) {
                    uuid_to_string(&princ_id, &uuid_string, &lst);
                    if (!GOOD_STATUS(&lst)) {
                        printf("DEBUG: Got a bogus uuid back from the rgy: conversion status = %x\n", lst);
                    } else {
                        printf("\nDEBUG: USER ENTRY UUID = %s\n\n",
                                (char *)uuid_string);
                    }
                }
                sec_acl_entry_p->entry_info.tagged_union.id.uuid = princ_id;
            }
            else if (STATUS_EQUAL(&st, sec_rgy_object_not_found))
                SET_STATUS(&st, deif_err_invalid_entry_key);
            else
                if (STATUS_EQUAL(&st, sec_rgy_server_unavailable) ||
                    STATUS_EQUAL(&st, rpc_s_comm_failure))
                    SET_STATUS(&st, deif_err_rgy_not_found);
            break;

        case sec_acl_e_type_group:
        case sec_acl_e_type_group_deleg:
            sec_acl_entry_p->entry_info.tagged_union.id.name =
                (unsigned_char_p_t)
                    malloc((strlen(string)+1) * sizeof(unsigned_char_t));
            strcpy((char *) sec_acl_entry_p->entry_info.tagged_union.id.name,
                    string);
            sprintf((char *)global_name, "%s/%s", def_cell_name, string);
            sec_id_parse_group(def_cell_rgy_h, global_name, NULL, &dummy_id,
                                princ_name, &princ_id, &st);
            if (GOOD_STATUS(&st))
                sec_acl_entry_p->entry_info.tagged_union.id.uuid = princ_id;
            else if (STATUS_EQUAL(&st, sec_rgy_object_not_found))
                SET_STATUS(&st, deif_err_invalid_entry_key);
            else
                if (STATUS_EQUAL(&st, sec_rgy_server_unavailable) ||
                    STATUS_EQUAL(&st, rpc_s_comm_failure))
                    SET_STATUS(&st, deif_err_rgy_not_found);
            break;

        case sec_acl_e_type_foreign_other:
        case sec_acl_e_type_for_other_deleg:
            sec_acl_entry_p->entry_info.tagged_union.id.name =
                (unsigned_char_p_t)
                    malloc((strlen(string)+1) * sizeof(unsigned_char_t));
        strcpy((char *) sec_acl_entry_p->entry_info.tagged_union.id.name,
                    string);
            sec_id_parse_name(rgy_h, (unsigned_char_p_t) string, cell_name,
                                &cell_id, NULL, &dummy_id, &st);
            if (GOOD_STATUS(&st)) {
                sec_acl_entry_p->entry_info.tagged_union.id.uuid = cell_id;
            }
            break;
        case sec_acl_e_type_foreign_user:
        case sec_acl_e_type_foreign_group:
        case sec_acl_e_type_for_user_deleg:
        case sec_acl_e_type_for_group_deleg:
            /* Parse the string into an id.name and realm.name */
            st = deif_parse_foreign_id(string, princ_name, &princ_id,
                                        cell_name, &cell_id, entry_type);
            if (GOOD_STATUS(&st)) {
                /* Allocate the storage for, and copy the identifiers */
                sec_acl_entry_p->entry_info.tagged_union.foreign_id.id.name =
                    (unsigned_char_p_t) malloc((strlen((char *)princ_name)+1) *
                                                sizeof(unsigned_char_t));
                strcpy((char *)
                  sec_acl_entry_p->entry_info.tagged_union.foreign_id.id.name,
                  (char *)princ_name);
                sec_acl_entry_p->entry_info.tagged_union.foreign_id.id.uuid =
                    princ_id;

                sec_acl_entry_p->entry_info.tagged_union.foreign_id.realm.name=
                    (unsigned_char_p_t) malloc((strlen((char *)cell_name)+1) *
                                                sizeof(unsigned_char_t));
                strcpy((char *)
                    sec_acl_entry_p->entry_info.tagged_union.foreign_id.realm.name,
                    (char *)cell_name);
                sec_acl_entry_p->entry_info.tagged_union.foreign_id.realm.uuid=
                    cell_id;
            }
            break;
        default:
            SET_STATUS(&st, deif_err_invalid_entry_type);
    }
    return st;
}


/*
 * m a k e _ l o w e r c a s e
 */
PRIVATE void make_lowercase (
    char *buffer
)
{
    wchar_t     *wcs_buf, *wcs_ptr;
    int         wcs_size;

    wcs_buf = NULL;

    /* Determine the size of wchar_t buffer needed to be allocated,
     * then malloc storage and convert to wchar_t for processing.
     */

    wcs_size = mbstowcs((wchar_t *) 0, buffer, strlen(buffer));
    wcs_buf = malloc((wcs_size + 1) * sizeof(wchar_t));
    mbstowcs(wcs_buf, buffer, (wcs_size+1));

    /* Convert to lower case */
    wcs_ptr = wcs_buf;
    while (*wcs_ptr != (wchar_t) 0) {
         if (iswupper(*wcs_ptr)) 
             *wcs_ptr = towlower(*wcs_ptr);
         wcs_ptr++;
    }

    /* Convert back to multibyte string format & cleanup */
    wcstombs(buffer, wcs_buf, (wcs_size+1));
    free(wcs_buf);
}


/*
 * d e i f _ f r e e _ e n t r i e s
 * Frees locally allocated sec_acl_entries
 */
PRIVATE void deif_free_entries( void )
{
    int i;
    sec_acl_entry_t *entry_p;

    if ((sec_acl_p != NULL) && (sec_acl_p->sec_acl_entries != NULL)) {
        for (i = 0; i < sec_acl_p->num_entries; i++) {
            entry_p = &(sec_acl_p->sec_acl_entries[i]);
            if (entry_p) {
                switch (entry_p->entry_info.entry_type)  {
                    case sec_acl_e_type_user:
                    case sec_acl_e_type_group:
                    case sec_acl_e_type_foreign_other:
                    case sec_acl_e_type_user_deleg:
                    case sec_acl_e_type_group_deleg:
                    case sec_acl_e_type_for_other_deleg:
                        if (entry_p->entry_info.tagged_union.id.name)
                            free((char *)entry_p->entry_info.tagged_union.id.name);
                        break;
                    case sec_acl_e_type_foreign_user:
                    case sec_acl_e_type_foreign_group:
                    case sec_acl_e_type_for_user_deleg:
                    case sec_acl_e_type_for_group_deleg:
                        if (entry_p->entry_info.tagged_union.foreign_id.id.name)
                            free((char *)entry_p->entry_info.tagged_union.foreign_id.id.name);
                        break;
                    case sec_acl_e_type_extended:
                        if (entry_p->entry_info.tagged_union.extended_info)
                            free((char *)entry_p->entry_info.tagged_union.extended_info);
                        break;
                    default:
                        break;
                }
            } /* if (entry_p) */
        } /* for num_entries */
    } /* if we have an acl and entries */
}


/*
 * d e i f _ g e t _ e x t e n d e d _ i n f o
 *
 * parse extended info from ascii string
 *  Data shall be in the following format:
 *      uuid_string.f.f.f.f.n.b
 *  where uuid_string is a fixed length ascii representation of
 *  an NCS 2.0 uuid, f.f.f.f are the 4 bytes of an ndr_format_t
 *  (each represented as a hex digit), n is the (decimal) number
 *  of bytes in the extended info and b is the string of bytes
 *  (each byte represented by 2 hex digits).
 */
PRIVATE error_status_t deif_get_extended_info (
    sec_acl_entry_t         *ep,
    char                    *string
)
{
    error_status_t      st;
    unsigned_char_p_t   *uuid_string;
    int                 i, num_bytes, tmp_val, f1, f2, f3, f4;
    char                *str1, *str2;   /* to scan the string with */
    uuid_t              uuid;
    ndr_format_t        tmp_format;

    SET_STATUS(&st, error_status_ok);

    /* Find the end of the extension_type uuid */
    str1 = string;
    str2 = strchr(string, '.');

    /* If no '.' was found, the string cannot be a extended entry */
    if (str2 == NULL)
        SET_STATUS(&st, deif_err_invalid_entry);
    else {
        if (*str2 == '.') {         /* Get rid of '.'               */
            *str2 = '\0';           /* Terminate the entry_type str */
            str2++;                 /* save the rest of the string  */
        }
        uuid_from_string((unsigned_char_p_t) str1, &uuid, &st);
    }

    /* Skip this if error, so we don't access a NULL string */
    if (GOOD_STATUS(&st)) {
        /* If above was successful, parse the format label */
        if (strlen(str2) < 9) {        /* Min possible is 9 chars if n = 0 */
            SET_STATUS(&st, deif_err_invalid_entry);
        } else {
            /* Advance str1 to end of f.f.f.f (str2 == f.f.f.f.n.b) */
            str1 = str2;
            for (i = 1; ((str1 != NULL) && (*str1 != '\0') && (i <= 4)); i++) {
                str1 = strchr(str1, '.');
                /* Stop at the 4th '.' */
                if ((str1 != NULL) && (i != 4)) str1++;
            }
        }
    }

    if ((GOOD_STATUS(&st)) && (str1 != NULL)) {
        *str1 = '\0'; str1++;           /* Terminate format label string    */
        /* Scan the format label values and make sure 4 were found */
        if (sscanf(str2, "%x.%x.%x.%x", &f1, &f2, &f3, &f4) != 4) {
                SET_STATUS(&st, deif_err_invalid_entry);
        } else {
            tmp_format.int_rep   = (ndr_byte) f1;
            tmp_format.char_rep  = (ndr_byte) f2;
            tmp_format.float_rep = (ndr_byte) f3;
            tmp_format.reserved  = (ndr_byte) f4;
        }
    }

    /* If above was successful, get the number of bytes expected */
    if (GOOD_STATUS(&st)) {
        str2 = strchr(str1, '.');
        if (str2 != NULL)
            if (*str2 == '.') {         /* Get rid of '.' to            */
                *str2 = '\0';           /* terminate the num_bytes str  */
                str2++;
            }
        if (sscanf(str1, "%d", &num_bytes) != 1)
            SET_STATUS(&st, deif_err_invalid_entry);
    }

    /* If above was successful fill the struct and attempt to scan the bytes */
    if (GOOD_STATUS(&st)) {
        /* First, allocate the structure */
        ep->entry_info.tagged_union.extended_info = (sec_acl_extend_info_t *)
            malloc(sizeof(uuid_t) + sizeof(ndr_format_t) + sizeof(unsigned32) +
                    (num_bytes * sizeof(ndr_byte)));
        ep->entry_info.tagged_union.extended_info->extension_type = uuid;
        ep->entry_info.tagged_union.extended_info->format_label   = tmp_format;
        ep->entry_info.tagged_union.extended_info->num_bytes      = num_bytes;

        /* Make sure there's something there to scan */
        if ((str2 == NULL) && (num_bytes != 0))
            SET_STATUS(&st, deif_err_invalid_entry);
        else if (strlen((char *) str2) != (2 * num_bytes))
            SET_STATUS(&st, deif_err_invalid_entry);

        /* Now attempt to scan the bytes */
        for (i = 0;(i < num_bytes) && GOOD_STATUS(&st) && (str2 != NULL);i++) {
            if (1 > sscanf(str2, "%2x", &tmp_val))
                SET_STATUS(&st, deif_err_invalid_entry);
            else {
                ep->entry_info.tagged_union.extended_info->pickled_data[i] =
                    (ndr_byte) tmp_val;
                str2 = str2 + 2;
            }
        }
    }

    return st;
}


/* d e i f _ v a l i d a t e _ s e c _ a c l _ e n t r y
 *
 * Parses entries of form type{:key}:perms
 *
 * The key is specified only for certain types.
 * The 'key_only' parameter disables validation and translation of the
 * permissions, and is set to true by the delete routine, which needs
 * to do a lookup by type{:key} only.  It is set to true whenever a
 * full sec_acl_entry is necessary.
 */
PUBLIC error_status_t deif_validate_sec_acl_entry (
    char            *string,
    sec_acl_entry_t *sec_acl_entry_p,
    boolean32       key_only
)
{
    error_status_t  st;
    boolean32       found;
    int             i;
    char            *str1, *str2;

    /* Make sure the string is NULL terminated only (no newline) */
    i = strlen(string);
    if (i > 0)
        if (string[i - 1] == '\n')
            string[i - 1] = '\0';

    sec_acl_entry_p->perms = 0;
    SET_STATUS(&st, error_status_ok);

    /* Parse the entry type */
    str1 = strchr(string, ':');

    /* If no ':' was found, the string cannot be a full sec_acl_entry */
    if ((str1 == NULL) && (! key_only))
        SET_STATUS(&st, deif_err_invalid_entry);
    else {
        if (str1 != NULL)
            if (*str1 == ':') {         /* Get rid of :'s               */
                *str1 = '\0';           /* Terminate the entry_type str */
                str1++;                 /* save the rest of the string  */
            }
        make_lowercase(string);
        found = false;
        for (i = 0; (entry_types[i].name != NULL) && (! found); i++)
            if (strcmp(entry_types[i].name, string) == 0) {
                found = true;
                sec_acl_entry_p->entry_info.entry_type =
                                            entry_types[i].entry_type;
            }
        if (! found)
            SET_STATUS(&st, deif_err_invalid_entry_type);
    }

    /*  Depending on the type, there may be a KEY expected. */
    if (GOOD_STATUS(&st)) {
        switch (sec_acl_entry_p->entry_info.entry_type) {
            case sec_acl_e_type_mask_obj:           /* No key for these */
            case sec_acl_e_type_unauthenticated:
            case sec_acl_e_type_user_obj:
            case sec_acl_e_type_other_obj:
            case sec_acl_e_type_group_obj:
            case sec_acl_e_type_any_other:
            case sec_acl_e_type_user_obj_deleg:
            case sec_acl_e_type_other_obj_deleg:
            case sec_acl_e_type_group_obj_deleg:
            case sec_acl_e_type_any_other_deleg:
                break;
            case sec_acl_e_type_user:
            case sec_acl_e_type_group:
            case sec_acl_e_type_foreign_other:
            case sec_acl_e_type_foreign_user:
            case sec_acl_e_type_foreign_group:
            case sec_acl_e_type_user_deleg:
            case sec_acl_e_type_group_deleg:
            case sec_acl_e_type_for_other_deleg:
            case sec_acl_e_type_for_user_deleg:
            case sec_acl_e_type_for_group_deleg:
                /* There should be a key, so look for another colon */
                string = str1;
                str1 = strchr(string, ':');
                if ((str1 == NULL) && (! key_only))
                    SET_STATUS(&st, deif_err_invalid_entry);
                else {              /* Otherwise translate the key */

                    /* if there was a colon, terminate the key */
                    if (str1 != NULL) {
                        *str1 = '\0'; str1++;
                    }
                    st = deif_get_key(sec_acl_entry_p,
                            sec_acl_entry_p->entry_info.entry_type, string);
                    /* If invalid key, un-truncate the string that was passed
                     * in so the caller can see the bad key.
                     */
                    if ((! GOOD_STATUS(&st)) && (string != NULL))
                        *(string - 1) = ':';
                }
                break;
            case sec_acl_e_type_extended:
                /* There should be a key, so look for another colon */
                string = str1;
                str1 = strchr(string, ':');
                if ((str1 == NULL) && (! key_only))
                    SET_STATUS(&st, deif_err_invalid_entry);
                else {              /* Otherwise translate the key */

                    /* if there was a colon, terminate the key */
                    if (str1 != NULL) {
                        *str1 = '\0'; str1++;
            }
                    st = deif_get_extended_info(sec_acl_entry_p, string);

                    /* If invalid key, un-truncate the string that was passed
                     * in so the caller can see the bad key.
                     */
                    if ((! GOOD_STATUS(&st)) && (string != NULL))
                        *(string - 1) = ':';
                }
                break;
            default:
                SET_STATUS(&st, deif_err_invalid_entry_type);
        }
    }

    /*
     *  Translate the permission string into a permset_t.
     *  For each permission character entered, compare against
     *  each possible printstring, for a match.  If found, add
     *  the permission bit for that printstring to the permset.
     */

    while((! key_only) && (GOOD_STATUS(&st)) && (*str1 != '\0')) {
        found = false;
        for (i = 0; i < num_printstrings && !found; i++)
            if (*str1 == *sec_acl_printstrings[i].printstring) {
                found = true;
                sec_acl_entry_p->perms = sec_acl_entry_p->perms |
                                        sec_acl_printstrings[i].permissions;
            }
        if ((! found) && (*str1 != '-')) {
            SET_STATUS(&st, deif_err_invalid_perm);
        }
        str1++;
    }

    return st;
}


/*
 * d e i f _ g e t _ c a c h e _ d a t a
 */
PRIVATE error_status_t deif_get_cache_data (
    sec_acl_type_t              type,
    sec_acl_edit_mask_calc_t    *mask_calc_type_p
)
{
    unsigned32                  size_used, num_types, total_num_printstrings;
    error_status_t              lst, st;
    boolean32                   tokenize;
    uuid_t                      manager_type_chain;
    sec_acl_printstring_t       manager_info;
    sec_acl_posix_semantics_t   posix_semantics[2];
    uuid_t                      dummy_uuid;
    sec_rgy_bind_auth_info_t    auth_info;

    /* Get a handle to the local cell's registry for later use */
    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level= rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;

    sec_rgy_cell_bind((unsigned_char_p_t) "/.:", &auth_info, &rgy_h, &st);
    if (STATUS_EQUAL(&st, sec_rgy_cant_authenticate) ||
        STATUS_EQUAL(&st, sec_login_s_no_current_context)) {
        auth_info.info_type = sec_rgy_bind_auth_none;
        sec_rgy_cell_bind((unsigned_char_p_t) "/.:", &auth_info, &rgy_h, &lst);
        if (GOOD_STATUS(&lst)) {
            deif_msg_print(
                "Warning - binding to registry is unauthenticated\n", false);
        } else {
            return lst;
        }
    } else if (BAD_STATUS(&st)) {
        return st;
    }

    /* Get the name/uuid of the this cell now to prevent redundant
     * calls to get this information later...
     */
    sec_id_parse_name(rgy_h, (unsigned_char_p_t)"/.:", my_cell_name,
                        &my_cell_id, NULL, NULL, &st);

    sec_acl_get_mgr_types_semantics(sec_acl_handle, type, (unsigned32)1,
                                    &size_used, &num_types, &sec_acl_mgr_type,
                                    posix_semantics, &st);
        /* Recover from out-of-range error (old ACL mgr) by making old call */
    if (STATUS_EQUAL(&st, rpc_s_op_rng_error)) {
        sec_acl_get_manager_types(sec_acl_handle, type, (unsigned32)1,
                                    &size_used, &num_types, &sec_acl_mgr_type,
                                    &st);
        posix_semantics[0] = sec_acl_posix_no_semantics;
    }
    if (! (FLAG_SET(posix_semantics[0], sec_acl_posix_mask_obj))) {
        *mask_calc_type_p = NO_MASK_OBJ_CALC;
    } else if (verbose) {
        deif_msg_print("INFO - ACL Manager requested POSIX mask object semantics\n", false);
    }

    if (GOOD_STATUS(&st)) {
        sec_acl_get_printstring (sec_acl_handle, &sec_acl_mgr_type,
            (unsigned32) 32, &manager_type_chain, &manager_info,
            &tokenize, &total_num_printstrings, &size_used,
            sec_acl_printstrings, &st);
        num_printstrings = size_used;
    }


        /* Attempt to retrieve an existing acl */
    if (GOOD_STATUS(&st)) {
        /* Cache the sec_acl_type and the sec_acl itself for later use */
        sec_acl_type = type;
        sec_acl_lookup (sec_acl_handle, &sec_acl_mgr_type, sec_acl_type,
                    &sec_acl_list, &st);
        if (GOOD_STATUS(&st)) {
            acl_exists = true;
            sec_acl_p = sec_acl_list.sec_acls[0];


            /* Get the name/uuid of the default cell now to prevent redundant
             * calls to get this information later...
             */
            if (GOOD_STATUS(&st)) {

                sec_id_gen_name(rgy_h, &(sec_acl_p->default_realm.uuid),
                                &dummy_uuid, NULL, def_cell_name, NULL, &st);
                if (GOOD_STATUS(&st)) {
                    def_cell_valid = true;
                    def_cell_id = sec_acl_p->default_realm.uuid;
                    if (uuid_equal(&def_cell_id, &my_cell_id, &lst)) {
                        def_cell_rgy_h = rgy_h;
                    } else {
                        /* Get a handle to the default cell's registry
                         * for later use
                         */
                        auth_info.info_type = sec_rgy_bind_auth_dce;
                        auth_info.tagged_union.dce_info.authn_level =
                            rpc_c_protect_level_pkt_integ;
                        auth_info.tagged_union.dce_info.authn_svc =
                            rpc_c_authn_dce_secret;
                        auth_info.tagged_union.dce_info.authz_svc =
                            rpc_c_authz_dce;
                        auth_info.tagged_union.dce_info.identity  = NULL;

                        sec_rgy_cell_bind(def_cell_name, &auth_info,
                                            &def_cell_rgy_h, &st);
                        if (STATUS_EQUAL(&st, sec_rgy_cant_authenticate)) {
                            auth_info.info_type = sec_rgy_bind_auth_none;
                            sec_rgy_cell_bind(def_cell_name, &auth_info,
                                                &def_cell_rgy_h, &lst);
                            if (GOOD_STATUS(&lst)) {
                                deif_msg_print("Warning - binding to default\
 cell registry is unauthenticated\n", false);
                            } else {
                                return lst;
                            }
                        }
                    } /* End of if my_cell == def_cell */
                } else {
                    def_cell_valid = false;
                    deif_msg_print("Unknown default cell from ACL - ", false);
                    deif_status_print(&st, false);
                    deif_msg_print("INFO: Local cell will be used for\
 operations requiring default cell info.\n", false);
                    CLEAR_STATUS(&st);
                    def_cell_id = my_cell_id;
                    def_cell_rgy_h = rgy_h;
                    strcpy((char *)def_cell_name, (char *)my_cell_name);
                }
            }
        } else {
            acl_exists = false;
            sec_acl_list.sec_acls[0] = sec_acl_p = &sec_acl;
            sec_acl_list.num_acls = 1;
            def_cell_id = my_cell_id;
            def_cell_rgy_h = rgy_h;
            strcpy((char *)def_cell_name, (char *)my_cell_name);
            def_cell_valid = true;
        }
    }
    return st;
}


/*
 * d e i f _ d e c o d e _ p e r m s e t
 */
PRIVATE error_status_t deif_decode_permset (
    char                *perms,
    sec_acl_permset_t   *permset_p
)
{
    error_status_t      st;
    int                 i;
    boolean32           found;

    /*
     *  Translate the permission string into a permset_t.
     *  For each permission character entered, compare against
     *  each possible printstring, for a match.  If found, add
     *  the permission bit for that printstring to the permset.
     */

    *permset_p = 0;
    CLEAR_STATUS(&st);

    while((GOOD_STATUS(&st)) && (*perms != '\0')) {
        found = false;
        for (i = 0; i < num_printstrings && !found; i++)
            if (*perms == *sec_acl_printstrings[i].printstring) {
                found = true;
                *permset_p = *permset_p | sec_acl_printstrings[i].permissions;
            }
        if ((! found) && (*perms != '-')) {
            SET_STATUS(&st, deif_err_invalid_perm);
        }
        perms++;
    }
    return st;
}


/*
 * d e i f _ s e c _ a c l _ f r o m _ f i l e
 */
PUBLIC error_status_t deif_sec_acl_from_file (
    char                *fname
)
{
    error_status_t              st, lst;
    sec_acl_entry_t             sec_acl_entry;
    FILE                        *fptr;
    char                        sec_acl_entry_str[1024], *tmpc;
    int                         num_entries = 0;
    sec_rgy_name_t              cell_name;
    uuid_t                      cell_id;
    sec_rgy_bind_auth_info_t    auth_info;

    if ((fptr = fopen(fname, "r")) == NULL) {
        SET_STATUS(&st, deif_err_file_open);
        return st;
    }

    /* Make default cell a dummy uuid, until we come across a line specifying
     * what it should be.
     */
    if (! acl_exists) {
        sec_acl_p->default_realm.name = (unsigned_char_p_t)
            /* Allocate the storage for, and copy the identifier */
            malloc((strlen((char *)def_cell_name)+1)*sizeof(unsigned_char_t));
        strcpy((char *) sec_acl_p->default_realm.name, (char *) def_cell_name);
        sec_acl_p->default_realm.uuid = def_cell_id;
        acl_exists = true;
    } else {
        deif_free_entries();
    }

    sec_acl_p->num_entries = 0;

    /* The following is the easiest way to figure out how much space to
     * allocate for the acl entries.  If the format of the acl entries
     * in the file changes (ie: multiple entries per line) this algorithm
     * will have to be replace by a dynamic allocation one, in the loop
     * below that fills the entry list.
     */
    while (!feof(fptr))
        if ((fgets(sec_acl_entry_str, 1024, fptr) == NULL) && ferror(fptr)) {
            clearerr(fptr);
        }
        else
        num_entries++;
    sec_acl_p->sec_acl_entries = (sec_acl_entry_t *)
                                malloc(num_entries * sizeof(sec_acl_entry_t));

    /* Reset the file, and begin parsing the entries */
    fseek(fptr, 0, SEEK_SET);
    CLEAR_STATUS(&st);
    while (!feof(fptr) && GOOD_STATUS(&st)) {
        sec_acl_entry_str[0] = '\0';
        if ((fgets(sec_acl_entry_str, 1024, fptr) == NULL) && ferror(fptr)) {
            SET_STATUS(&st, deif_err_file_read);
            clearerr(fptr);
        }
        else {
            if ((strlen(sec_acl_entry_str)>0)&&(*sec_acl_entry_str != '\n')) {

                /* If input line is a DEFAULT CELL then process it separately*/
                if ((strlen(sec_acl_entry_str) > DEF_CELL_PFX_LEN) &&
                    (strncmp(DEF_CELL_PFX, sec_acl_entry_str,
                                                    DEF_CELL_PFX_LEN)==0) ) {

                    tmpc = sec_acl_entry_str + DEF_CELL_PFX_LEN;
                    if (tmpc[strlen(tmpc)-1] == '\n')
                        tmpc[strlen(tmpc)-1] = '\0';

                    /* Attempt to resolve the default cell by specified name */
                    sec_id_parse_name(rgy_h, (unsigned_char_p_t) tmpc,
                                        cell_name, &cell_id, NULL, NULL, &st);
                    if (GOOD_STATUS(&st)) {
                        if (sec_acl_p->default_realm.name)
                            free(sec_acl_p->default_realm.name);
                        sec_acl_p->default_realm.name = (unsigned_char_p_t)
                            /* Allocate the storage for, copy the identifier */
                            malloc((strlen((char *)cell_name)+1)*sizeof(char));
                        strcpy( (char *) sec_acl_p->default_realm.name,
                                (char *) cell_name);
                        sec_acl_p->default_realm.uuid = cell_id;
                        def_cell_id = cell_id;
                        strcpy( (char *) def_cell_name,
                                (char *) cell_name);
                        def_cell_valid = true;

                        /* Get a handle to the new def cell for later use */
                        if (uuid_equal(&my_cell_id, &def_cell_id, &lst)) {
                            def_cell_rgy_h = rgy_h;
                        } else {
                            auth_info.info_type = sec_rgy_bind_auth_dce;
                            auth_info.tagged_union.dce_info.authn_level =
                                                rpc_c_protect_level_pkt_integ;
                            auth_info.tagged_union.dce_info.authn_svc =
                                                rpc_c_authn_dce_secret;
                            auth_info.tagged_union.dce_info.authz_svc =
                                                rpc_c_authz_dce;
                            auth_info.tagged_union.dce_info.identity  = NULL;

                            sec_rgy_cell_bind(def_cell_name, &auth_info,
                                                &def_cell_rgy_h, &st);
                            if (STATUS_EQUAL(&st, sec_rgy_cant_authenticate)) {
                                auth_info.info_type = sec_rgy_bind_auth_none;
                                sec_rgy_cell_bind(def_cell_name, &auth_info,
                                                    &def_cell_rgy_h, &st);
                                if (GOOD_STATUS(&st)) {
                                    deif_msg_print("Warning - binding to default\
 cell registry is unauthenticated\n", false);
                                }
                            }
                        }

                    } /* good status on cell name lookup */
                    else {
                        deif_msg_print("Invalid default cell - ", false);
                        strcat(tmpc, "\n");
                        deif_msg_print(tmpc, false);
                    } /* bad status on cell name lookup */

                /* else if not a comment line */
                } else if (sec_acl_entry_str[0] != '#') {
                    st = deif_validate_sec_acl_entry(sec_acl_entry_str,
                                                        &sec_acl_entry, false);
                    /* Stuff the sec_acl_entry into the acl */
                    if (GOOD_STATUS(&st))
                        sec_acl_p->sec_acl_entries[sec_acl_p->num_entries++] =
                                                                sec_acl_entry;
                    else {
                        strcat((char *) sec_acl_entry_str, " - ");
                        deif_msg_print(sec_acl_entry_str, false);
                        deif_status_print(&st, false);
                        SET_STATUS(&st, error_status_ok);
                    }       /* if good_status */
                }       /* if DEFAULT_CELL_PREFIX else ... */
            }           /* if sec_acl_entry_str > 0 and not a nl */
        }               /* if fgets was successful */
    }                   /* while not eof */

    return st;
}


/*
 * d e i f _ d o _ d e l e t e
 */
PUBLIC error_status_t deif_do_delete (
    sec_acl_entry_t *sec_acl_entry_p
)
{
    int             i, j;
    error_status_t  st;
    boolean32       entry_found = false;

    SET_STATUS(&st, error_status_ok);
    if (acl_exists) {
        /* If the specified entry is found, delete it */
        for (i = 0; i < sec_acl_p->num_entries; i++) {
            if (deif_entry_compare(&(sec_acl_p->sec_acl_entries[i]),
                                    sec_acl_entry_p, true)) {
                entry_found = true;
                /* Shift any remaining entries down the list */
                for (j = i; j < sec_acl_p->num_entries-1; j++)
                    sec_acl_p->sec_acl_entries[j] = sec_acl_p->sec_acl_entries[j+1];
                sec_acl_p->num_entries--;
            }
        }

        if (! entry_found)
            SET_STATUS(&st, deif_err_entry_not_found);
    } else
        SET_STATUS(&st, sec_acl_no_acl_found);

    return st;
}


/*
 * d e i f _ d o _ k i l l _ e n t r i e s
 */
PUBLIC error_status_t deif_do_kill_entries( void )
{
    sec_acl_entry_t *new_entries;
    int             i, j, num_new_entries;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);
    num_new_entries = 0;

    if (acl_exists) {
        /* Allocate the correct number of pointers to sec_acl_entries */
        for (i = 0; i < sec_acl_p->num_entries; i++)
            if (sec_acl_p->sec_acl_entries[i].entry_info.entry_type ==
                                                    sec_acl_e_type_user_obj)
                num_new_entries++;
        new_entries = (sec_acl_entry_t *) malloc(num_new_entries *
            sizeof(sec_acl_entry_t));

        /* Now we can collect any USER_OBJ entries */
        j = 0;
        for (i = 0; i < sec_acl_p->num_entries; i++) {
            if (sec_acl_p->sec_acl_entries[i].entry_info.entry_type ==
                                                    sec_acl_e_type_user_obj) {
                new_entries[j++] = sec_acl_p->sec_acl_entries[i];
            }
        }

        /* Replace the entry list with the trimmed down one */
        free((char *) sec_acl_p->sec_acl_entries);
        sec_acl_p->sec_acl_entries = new_entries;
        sec_acl_p->num_entries = num_new_entries;
    } else
        SET_STATUS(&st, sec_acl_no_acl_found);

    return st;
}


/********************************************************************
 * For first release, print the output to stdout.
 * An abstract output mechanism needs to be implemented to
 * deal with having tty,cli,gui as possible outputs
 ********************************************************************/
/*
 * d e i f _ p e r m s e t _ o u t p u t
 */
PRIVATE void deif_permset_output (
    sec_acl_permset_t   perms
)
{
    int i;
    for (i = 0; i < num_printstrings; i++)
        if (sec_acl_printstrings[i].permissions & perms)
            printf("%s",sec_acl_printstrings[i].printstring);
        else
            printf("-");
}


/*
 * d e i f _ m a s k _ p e r m s
 */
PRIVATE void deif_mask_perms (
    sec_acl_permset_t   perms,
    sec_acl_permset_t   mask_perms
)
{
    int i;
    sec_acl_permset_t effective_perms;

    effective_perms = perms & mask_perms;
    if (perms != effective_perms) {
        printf("\t#effective:");
        for (i = 0; i < num_printstrings; i++)
            if (sec_acl_printstrings[i].permissions & perms & mask_perms)
                printf("%s",sec_acl_printstrings[i].printstring);
            else
                printf("-");
    }
}


/*
 * d e i f _ p r i n t _ e x t e n d e d _ i n f o
 */
PRIVATE void deif_print_extended_info (
    sec_acl_extend_info_t   *info
)
{
    error_status_t      st;
    unsigned_char_p_t   uuid_string;
    int                 i;

    /* Display the Extension Type */
    uuid_to_string(&(info->extension_type), &uuid_string, &st);
    if (GOOD_STATUS(&st)) {
        printf("%s.", (char *)uuid_string);
        rpc_string_free(&uuid_string, &st);
    } else
        printf("illegal extension type.");

    /* Display the Format Label */
    printf("%x.%x.%x.%x.", info->format_label.int_rep,
            info->format_label.char_rep,
            info->format_label.float_rep,
            info->format_label.reserved
    );

    /* Display the number of bytes in the entry */
    printf("%d.", info->num_bytes);

    /* Display the pickled data (array of bytes) */
    for (i = 0; i < info->num_bytes; i++)
        printf("%.2x", info->pickled_data[i]);
    printf(":");
}


/*
 * d e i f _ p r i n t _ s e c _ a c l _ e n t r y
 */
PUBLIC void deif_print_sec_acl_entry (
    uuid_t              *default_cell,
    sec_acl_entry_t     sec_acl_entry
)
{
    sec_rgy_name_t      global_name;
    sec_rgy_handle_t    context;
    sec_rgy_cursor_t    csr;
    sec_rgy_pgo_item_t  item;
    error_status_t      lstat;
    unsigned_char_p_t   name1 = NULL, name2 = NULL;
    uuid_t              dummy_uuid;
    boolean32           free_name1 = false;
    boolean32           free_name2 = false;
    boolean32           delegate   = false;

    /* Output the entry type, and associated data if necessary */
    switch(sec_acl_entry.entry_info.entry_type) {

        /* These entry types have only a permset associated with them */
        case sec_acl_e_type_mask_obj:
            printf("mask_obj:");
            break;
        case sec_acl_e_type_user_obj:
            printf("user_obj:");
            break;
        case sec_acl_e_type_group_obj:
            printf("group_obj:");
            break;
        case sec_acl_e_type_other_obj:
            printf("other_obj:");
            break;
        case sec_acl_e_type_any_other:
            printf("any_other:");
            break;
        case sec_acl_e_type_user_obj_deleg:
            printf("user_obj_delegate:");
            break;
        case sec_acl_e_type_group_obj_deleg:
            printf("group_obj_delegate:");
            break;
        case sec_acl_e_type_other_obj_deleg:
            printf("other_obj_delegate:");
            break;
        case sec_acl_e_type_any_other_deleg:
            printf("any_other_delegate:");
            break;
        case sec_acl_e_type_unauthenticated:
            printf("unauthenticated:");
            break;

        /* these entry types have an id_t associated with them */
        case sec_acl_e_type_user_deleg:
            delegate = true;
        case sec_acl_e_type_user:
            sec_id_gen_name(def_cell_rgy_h, default_cell,
                            &sec_acl_entry.entry_info.tagged_union.id.uuid,
                            NULL, NULL, global_name, &lstat);
            if (GOOD_STATUS(&lstat)) {
                name1 = global_name;
            } else {
                uuid_to_string(
                            &sec_acl_entry.entry_info.tagged_union.id.uuid,
                            &name1, &lstat);
                free_name1 = true;
            }
            printf("user%s:%s:", ((delegate)? "_delegate":""), (char *) name1);
            break;
        case sec_acl_e_type_group_deleg:
            delegate = true;
        case sec_acl_e_type_group:
            sec_id_gen_group(def_cell_rgy_h, default_cell,
                            &sec_acl_entry.entry_info.tagged_union.id.uuid,
                            NULL, NULL, global_name, &lstat);
            if (GOOD_STATUS(&lstat)) {
                name1 = global_name;
            } else {
                uuid_to_string(
                            &sec_acl_entry.entry_info.tagged_union.id.uuid,
                            &name1, &lstat);
                free_name1 = true;
            }
            printf("group%s:%s:", ((delegate)? "_delegate":""),(char *) name1);
            break;
        case sec_acl_e_type_for_other_deleg:
            delegate = true;
        case sec_acl_e_type_foreign_other:
            sec_id_gen_name(rgy_h,
                            &sec_acl_entry.entry_info.tagged_union.id.uuid,
                            &dummy_uuid,
                            NULL, global_name, NULL, &lstat);
            if (GOOD_STATUS(&lstat)) {
                name1 = global_name;
            } else {
                uuid_to_string(
                            &sec_acl_entry.entry_info.tagged_union.id.uuid,
                            &name1, &lstat);
                free_name1 = true;
            }
            printf("foreign_other%s:%s:", ((delegate)? "_delegate":""),
                (char *) name1);
            break;

        /* these entry types have a foreign_id_t associated with them */
        case sec_acl_e_type_for_user_deleg:
            delegate = true;
        case sec_acl_e_type_foreign_user:
            sec_id_gen_name(rgy_h,
                &sec_acl_entry.entry_info.tagged_union.foreign_id.realm.uuid,
        	&sec_acl_entry.entry_info.tagged_union.foreign_id.id.uuid,
                global_name, NULL, NULL, &lstat);
            if (GOOD_STATUS(&lstat)) {
                printf("foreign_user%s:%s:", ((delegate)?  "_delegate":""),
                    global_name);
            } else {
                uuid_to_string(
                  &sec_acl_entry.entry_info.tagged_union.foreign_id.realm.uuid,
                  &name1, &lstat);
                if (GOOD_STATUS(&lstat))
                    free_name1 = true;
                else
                    name1 = (unsigned_char_p_t) "<ILLEGAL CELL UUID>";
                uuid_to_string(
                    &sec_acl_entry.entry_info.tagged_union.foreign_id.id.uuid,
                    &name2, &lstat);
                if (GOOD_STATUS(&lstat))
                    free_name2 = true;
                else
                    name2 = (unsigned_char_p_t) "<ILLEGAL PRINCIPAL UUID>";
                printf("foreign_user%s:%s/%s:", ((delegate)? "_delegate":""),
                    name1, name2);
            }
            break;
        case sec_acl_e_type_for_group_deleg:
            delegate = true;
        case sec_acl_e_type_foreign_group:
            sec_id_gen_group(rgy_h,
                &sec_acl_entry.entry_info.tagged_union.foreign_id.realm.uuid,
                &sec_acl_entry.entry_info.tagged_union.foreign_id.id.uuid,
                global_name, NULL, NULL, &lstat);
            if (GOOD_STATUS(&lstat)) {
                printf("foreign_group:%s:", global_name);
            } else {
                uuid_to_string(
       	          &sec_acl_entry.entry_info.tagged_union.foreign_id.realm.uuid,
                  &name1, &lstat);
                if (GOOD_STATUS(&lstat))
                    free_name1 = true;
                else
                    name1 = (unsigned_char_p_t) "<ILLEGAL CELL UUID>";
                uuid_to_string(
                    &sec_acl_entry.entry_info.tagged_union.foreign_id.id.uuid,
                    &name2, &lstat);
                if (GOOD_STATUS(&lstat))
                    free_name2 = true;
                else
                    name2 = (unsigned_char_p_t) "<ILLEGAL GROUP UUID>";
                printf("foreign_group%s:%s/%s:", ((delegate)? "_delegate":""),
                    name1, name2);
            }
            break;

        case sec_acl_e_type_extended:
            printf("extended:");
            deif_print_extended_info(
                sec_acl_entry.entry_info.tagged_union.extended_info
            );
            break;
        default:
            break;
    } /* switch entry_type */

    /* Output the permset */
    deif_permset_output(sec_acl_entry.perms);

    if (free_name1 && name1)
        rpc_string_free(&name1, &lstat);
    if (free_name2 && name2)
        rpc_string_free(&name2, &lstat);
}

/*
 * The order in which sec_acl entries are output must be as follows
 */
sec_acl_entry_type_t type_order[] = {
    sec_acl_e_type_mask_obj        ,
    sec_acl_e_type_unauthenticated ,
    sec_acl_e_type_user_obj        ,
    sec_acl_e_type_user            ,
    sec_acl_e_type_foreign_user    ,
    sec_acl_e_type_group_obj       ,
    sec_acl_e_type_group           ,
    sec_acl_e_type_foreign_group   ,
    sec_acl_e_type_other_obj       ,
    sec_acl_e_type_foreign_other   ,
    sec_acl_e_type_any_other       ,
    sec_acl_e_type_user_obj_deleg  ,
    sec_acl_e_type_user_deleg      ,
    sec_acl_e_type_for_user_deleg  ,
    sec_acl_e_type_group_obj_deleg ,
    sec_acl_e_type_group_deleg     ,
    sec_acl_e_type_for_group_deleg ,
    sec_acl_e_type_other_obj_deleg ,
    sec_acl_e_type_for_other_deleg ,
    sec_acl_e_type_any_other_deleg ,
    sec_acl_e_type_extended
};


/*
 * d e i f _ s o r t _ e n t r i e s
 */
PRIVATE void deif_sort_entries (
    int             num_entries,
    sec_acl_entry_t **entries_p
)
{
    int             i, j, num_copied = 0;
    sec_acl_entry_t *s_entries;

    s_entries =
        (sec_acl_entry_t *) malloc(num_entries * sizeof(sec_acl_entry_t));

    for (i = 0; i < sec_acl_e_type_max_nbr; i++)
        for (j = 0; j < num_entries; j++)
        if ((*entries_p)[j].entry_info.entry_type == type_order[i])
            s_entries[num_copied++] = (*entries_p)[j];

    /* Only return new list if sort was successful */
    if (num_entries == num_copied) {
        free((char *) *entries_p);
    *entries_p = s_entries;
    } else
        free((char *) s_entries);
}


/*
 * d e i f _ d o _ l i s t
 */
PUBLIC error_status_t deif_do_list (
    sec_acl_edit_output_t   output_type,
    void                    *output
)
{
    error_status_t      st;
    int                 i, mask;
    unsigned_char_p_t   cell_uuid;
    uuid_t              dummy_uuid;

    if (acl_exists) {

        /* If acl_exists and num_entries > 0 but no entries, then there
         * was a problem reading the acl, and no attempt to read it should
         * be made.
         */
        if ((sec_acl_p->num_entries > 0) &&
            (sec_acl_p->sec_acl_entries == NULL)) {
            acl_exists = false;
            return(sec_acl_no_acl_found);
        }

        /* Specify which type of ACL is being listed */
        switch(sec_acl_type) {
            case sec_acl_type_object:
                printf("\n# SEC_ACL for %s:\n", sec_acl_entry_name);
                break;
            case sec_acl_type_default_object:
                printf("\n# Initial SEC_ACL for objects created under: %s:\n",
                        sec_acl_entry_name);
                break;
            case sec_acl_type_default_container:
                printf(
                    "\n# Initial SEC_ACL for directories created under: %s:\n",
                    sec_acl_entry_name);
                break;
            default:
                printf(
                    "\n# Unspecified SEC_ACL type %d for %s:\n", sec_acl_type,
                    sec_acl_entry_name);
                break;
        }
        /* Display the default cell */
        if (def_cell_valid) {
            printf("%s%s\n", DEF_CELL_PFX, (char *) def_cell_name);
        } else {
            uuid_to_string(&(sec_acl_p->default_realm.uuid), &cell_uuid, &st);
            if (GOOD_STATUS(&st)) {
                printf("%s%s\n", DEF_CELL_PFX, cell_uuid);
            } else {
                printf("%sUNKNOWN\n", DEF_CELL_PFX);
                deif_status_print(&st, false);
            }
        }

        /* Sort the sec_acl_entries by order of importance */
        deif_sort_entries(sec_acl_p->num_entries,
            &(sec_acl_p->sec_acl_entries));

        /* Check for existence of the optional mask */
        mask = -1;
        for (i = 0; i < sec_acl_p->num_entries; i++) {
            if (sec_acl_p->sec_acl_entries[i].entry_info.entry_type ==
                sec_acl_e_type_mask_obj) {
                mask = i;
            }
        }

        /* List the sec_acl_entries */
        for (i = 0; i < sec_acl_p->num_entries; i++) {
            deif_print_sec_acl_entry(&sec_acl_p->default_realm.uuid,
                                     sec_acl_p->sec_acl_entries[i]);

            /* If mask exists, display effective perms */
            switch (sec_acl_p->sec_acl_entries[i].entry_info.entry_type) {
                case sec_acl_e_type_group_obj:
                case sec_acl_e_type_any_other:
                case sec_acl_e_type_user:
                case sec_acl_e_type_group:
                case sec_acl_e_type_foreign_other:
                case sec_acl_e_type_foreign_user:
                case sec_acl_e_type_foreign_group:
                case sec_acl_e_type_group_obj_deleg:
                case sec_acl_e_type_any_other_deleg:
                case sec_acl_e_type_user_deleg:
                case sec_acl_e_type_group_deleg:
                case sec_acl_e_type_for_other_deleg:
                case sec_acl_e_type_for_user_deleg:
                case sec_acl_e_type_for_group_deleg:
                case sec_acl_e_type_extended:
                    if (mask != -1)
                    deif_mask_perms(sec_acl_p->sec_acl_entries[i].perms,
                        sec_acl_p->sec_acl_entries[mask].perms);
                    break;
                case sec_acl_e_type_user_obj:
                case sec_acl_e_type_user_obj_deleg:
                case sec_acl_e_type_unauthenticated:
                case sec_acl_e_type_mask_obj:
                case sec_acl_e_type_other_obj:
                case sec_acl_e_type_other_obj_deleg:
                default:
                    break;
            }
            printf("\n");
        }
        SET_STATUS(&st, error_status_ok);
    } else {
        SET_STATUS(&st, sec_acl_no_acl_found);
    }

    return st;
}


/*
 * d e i f _ l i s t _ p e r m i s s i o n s
 */
PUBLIC error_status_t deif_list_permissions (
    sec_acl_edit_output_t   output_type,
    void                    *output
)
{
    int             i;

    if (output_type == TTY_OUTPUT) {
        printf("Token\tDescription\n");
        for (i = 0; i < num_printstrings; i++)
            printf("%s\t%s\n", sec_acl_printstrings[i].printstring,
           sec_acl_printstrings[i].helpstring);
    }
    return error_status_ok;
}


/*
 * d e i f _ c a l c _ u n i o n _ p e r m i s s i o n s
 */
PUBLIC error_status_t deif_calc_union_permissions (
    boolean32               masked,
    sec_acl_permset_t       *perms_p
)
{
    error_status_t      st;
    int                 i, mask;

    CLEAR_STATUS(&st);
    *perms_p = (sec_acl_permset_t) 0;

    if (acl_exists) {
        /* If acl_exists and num_entries > 0 but no entries, then there
         * was a problem reading the acl, and no attempt to read it should
         * be made.
         */
        if ((sec_acl_p->num_entries > 0) &&
            (sec_acl_p->sec_acl_entries == NULL)) {
            acl_exists = false;
            return(sec_acl_no_acl_found);
        }

        if (masked) {
            /* Check for existence of the optional mask */
            mask = -1;
            for (i = 0; i < sec_acl_p->num_entries; i++) {
                if (sec_acl_p->sec_acl_entries[i].entry_info.entry_type ==
                    sec_acl_e_type_mask_obj) {
                    mask = i;
                }
            }
        }

        for (i = 0; i < sec_acl_p->num_entries; i++) {
            switch (sec_acl_p->sec_acl_entries[i].entry_info.entry_type) {
                case sec_acl_e_type_group_obj:
                case sec_acl_e_type_any_other:
                case sec_acl_e_type_user:
                case sec_acl_e_type_group:
                case sec_acl_e_type_foreign_other:
                case sec_acl_e_type_foreign_user:
                case sec_acl_e_type_foreign_group:
                case sec_acl_e_type_group_obj_deleg:
                case sec_acl_e_type_any_other_deleg:
                case sec_acl_e_type_user_deleg:
                case sec_acl_e_type_group_deleg:
                case sec_acl_e_type_for_other_deleg:
                case sec_acl_e_type_for_user_deleg:
                case sec_acl_e_type_for_group_deleg:
                case sec_acl_e_type_extended:
                    if (masked && (mask != -1)) {   /* Effective perms */
                        *perms_p = *perms_p |
                            (sec_acl_p->sec_acl_entries[i].perms &
                             sec_acl_p->sec_acl_entries[mask].perms);
                    } else {                        /* Actual perms    */
                        *perms_p = *perms_p |
                            sec_acl_p->sec_acl_entries[i].perms;
                    }
                case sec_acl_e_type_user_obj:
                case sec_acl_e_type_user_obj_deleg:
                case sec_acl_e_type_unauthenticated:
                case sec_acl_e_type_mask_obj:
                case sec_acl_e_type_other_obj:
                case sec_acl_e_type_other_obj_deleg:
                default:
                    break;
            }
        }
    } else {
        SET_STATUS(&st, sec_acl_no_acl_found);
    }

    return st;
}


/*
 * d e i f _ p u r g e _i n e f f e c t i v e
 */
PUBLIC error_status_t deif_purge_ineffective(
    void
)
{
    error_status_t      st;
    int                 i, mask;
    CLEAR_STATUS(&st);

    if (acl_exists) {
        /* If acl_exists and num_entries > 0 but no entries, then there
         * was a problem reading the acl, and no attempt to read it should
         * be made.
         */
        if ((sec_acl_p->num_entries > 0) &&
            (sec_acl_p->sec_acl_entries == NULL)) {
            acl_exists = false;
            return(sec_acl_no_acl_found);
        }

        /* Check for existence of the optional mask */
        mask = -1;
        for (i = 0; i < sec_acl_p->num_entries; i++) {
            if (sec_acl_p->sec_acl_entries[i].entry_info.entry_type ==
                sec_acl_e_type_mask_obj) {
                mask = i;
            }
        }

        if (mask != -1) {
            for (i = 0; i < sec_acl_p->num_entries; i++) {
                switch (sec_acl_p->sec_acl_entries[i].entry_info.entry_type) {
                    case sec_acl_e_type_group_obj:
                    case sec_acl_e_type_any_other:
                    case sec_acl_e_type_user:
                    case sec_acl_e_type_group:
                    case sec_acl_e_type_foreign_other:
                    case sec_acl_e_type_foreign_user:
                    case sec_acl_e_type_foreign_group:
                    case sec_acl_e_type_group_obj_deleg:
                    case sec_acl_e_type_any_other_deleg:
                    case sec_acl_e_type_user_deleg:
                    case sec_acl_e_type_group_deleg:
                    case sec_acl_e_type_for_other_deleg:
                    case sec_acl_e_type_for_user_deleg:
                    case sec_acl_e_type_for_group_deleg:
                    case sec_acl_e_type_extended:
                        sec_acl_p->sec_acl_entries[i].perms &=
                            sec_acl_p->sec_acl_entries[mask].perms;
                        break;
                    case sec_acl_e_type_user_obj:
                    case sec_acl_e_type_user_obj_deleg:
                    case sec_acl_e_type_unauthenticated:
                    case sec_acl_e_type_mask_obj:
                    case sec_acl_e_type_other_obj:
                    case sec_acl_e_type_other_obj_deleg:
                    default:
                        break;
                }
            }
        }
    } else {
        SET_STATUS(&st, sec_acl_no_acl_found);
    }

    return st;
}


/*
 * d e i f _ i s _ g r o u p _ c l a s s
 */
PUBLIC boolean32 deif_is_group_class (
    sec_acl_entry_t         *entryP
)
{
    switch (entryP->entry_info.entry_type) {
    case sec_acl_e_type_user_obj:
    case sec_acl_e_type_user_obj_deleg:
    case sec_acl_e_type_unauthenticated:
    case sec_acl_e_type_mask_obj:
    case sec_acl_e_type_other_obj:
    case sec_acl_e_type_other_obj_deleg:
        return false;
	break;
    case sec_acl_e_type_group_obj:
    case sec_acl_e_type_any_other:
    case sec_acl_e_type_user:
    case sec_acl_e_type_group:
    case sec_acl_e_type_foreign_other:
    case sec_acl_e_type_foreign_user:
    case sec_acl_e_type_foreign_group:
    case sec_acl_e_type_group_obj_deleg:
    case sec_acl_e_type_any_other_deleg:
    case sec_acl_e_type_user_deleg:
    case sec_acl_e_type_group_deleg:
    case sec_acl_e_type_for_other_deleg:
    case sec_acl_e_type_for_user_deleg:
    case sec_acl_e_type_for_group_deleg:
    case sec_acl_e_type_extended:
    default:
	return true;
	break;
    }
}


/*
 * d e i f _ a n y _ g r o u p _ c l a s s
 */
PUBLIC boolean32 deif_any_group_class (
    int                     num_entries,
    sec_acl_entry_t         *entries
)
{
    int i;
    for (i = 0; i < num_entries; i++) {
        if (deif_is_group_class(&(entries[i])))
            return true;
    }
    return false;
}


PUBLIC error_status_t deif_calc_mask(
    void
)
{
    error_status_t  st;
    sec_acl_calc_mask(&sec_acl_list, &st);
    return st;
}


/*
 * d e i f _ d o _ m o d i f y
 */
PUBLIC error_status_t deif_do_modify (
    sec_acl_entry_t         *sec_acl_entry_p,
    sec_acl_edit_mod_type_t mod_type,
    sec_acl_permset_t       *perms_changed_p
)
{
    error_status_t  st;
    boolean32       found = false;
    int             i, j;
    sec_acl_entry_t *new_sec_acl_entries;

    CLEAR_STATUS(&st);
    *perms_changed_p = 0;

    /* Do not allow manipulation of extended types via "modify".
     * Extended types may be "list"ed and "assign"ed only.
     */
    if (sec_acl_entry_p->entry_info.entry_type == sec_acl_e_type_extended) {
        SET_STATUS(&st, deif_err_invalid_entry_op);
        return st;
    }

    /* If a corresponding entry exists replace it,
     * otherwise add this entry to the existing ones.
     */
    if (acl_exists) {
        for (i = 0; i < sec_acl_p->num_entries; i++) {
            if (deif_entry_compare(sec_acl_entry_p,
                                    &(sec_acl_p->sec_acl_entries[i]), true)) {
                found = true;
                switch (mod_type) {
                    case NORMAL_MOD:
                        sec_acl_p->sec_acl_entries[i] = *sec_acl_entry_p;
                        *perms_changed_p = sec_acl_entry_p->perms;
                        break;
                    case INCREASE_MOD:
                        /* Add permissions and keep track of which new ones
                         * were granted
                         */
                        for (j = 0; j < num_printstrings; j++) {
                            if ((!FLAG_SET(sec_acl_p->sec_acl_entries[i].perms,
                                    sec_acl_printstrings[j].permissions)) &&
                                (FLAG_SET(sec_acl_entry_p->perms,
                                    sec_acl_printstrings[j].permissions)) ) {
                                SET(sec_acl_p->sec_acl_entries[i].perms,
                                    sec_acl_printstrings[j].permissions);
                                SET(*perms_changed_p,
                                    sec_acl_printstrings[j].permissions);
                            }
                        }
                        break;
                    case DECREASE_MOD:
                        /* On decrease, save the perm values before changes */
                        *perms_changed_p = sec_acl_p->sec_acl_entries[i].perms;
                        for (j = 0; j < num_printstrings; j++) {
                            if ((FLAG_SET(sec_acl_p->sec_acl_entries[i].perms,
                                    sec_acl_printstrings[j].permissions)) &&
                                (!FLAG_SET(sec_acl_entry_p->perms,
                                    sec_acl_printstrings[j].permissions)) ) {
                                UNSET(sec_acl_p->sec_acl_entries[i].perms,
                                    sec_acl_printstrings[j].permissions);
                            }
                        }
                        break;
                }
            }
        }
    } else {
        /* Allocate the storage for, and copy the identifier */
        sec_acl_p->default_realm.name = (unsigned_char_p_t)
            malloc((strlen((char *) my_cell_name) + 1) *
                    sizeof(unsigned_char_t));
        strcpy((char *) sec_acl_p->default_realm.name,
                (char *) my_cell_name);
        sec_acl_p->default_realm.uuid = my_cell_id;
        acl_exists = true;
        sec_acl_p->num_entries = 0;
    }

    /* If a corresponding entry was not found, a new list must be made
     * with the correct new number of entries.
     */

    if ((!found) && ((mod_type == NORMAL_MOD) || (mod_type == INCREASE_MOD))) {
        /* Allocate the space for n+1 entries */
        new_sec_acl_entries = (sec_acl_entry_t *)
            malloc((sec_acl_p->num_entries + 1) * sizeof(sec_acl_entry_t));

        /* Copy the existing entries */
        for (i = 0; i < sec_acl_p->num_entries; i++) {
            new_sec_acl_entries[i] = sec_acl_p->sec_acl_entries[i];
        }
        if (sec_acl_p->num_entries > 0)
            free((char *) sec_acl_p->sec_acl_entries);

        /* Add the new entry */
        new_sec_acl_entries[sec_acl_p->num_entries] = *sec_acl_entry_p;
        sec_acl_p->sec_acl_entries = new_sec_acl_entries;
        sec_acl_p->num_entries++;
        *perms_changed_p = sec_acl_entry_p->perms;
    }

    return st;
}


PUBLIC error_status_t deif_do_increase_mods (
    int                     num_entries,
    sec_acl_entry_t         *sec_acl_entries,
    sec_acl_permset_t       *perms_changed_p
)
{
    error_status_t  st;
    int             i;
    CLEAR_STATUS(&st);

    for (i = 0; ((i < num_entries) && GOOD_STATUS(&st)); i++) {
        st = deif_do_modify(&(sec_acl_entries[i]), INCREASE_MOD,
                            perms_changed_p);
    }

    return st;
}


PUBLIC error_status_t deif_do_decrease_mods (
    int                 num_entries,
    sec_acl_entry_t     *sec_acl_entries
)
{
    error_status_t      st;
    int                 i;
    sec_acl_permset_t   dummy;
    CLEAR_STATUS(&st);

    for (i = 0; ((i < num_entries) && GOOD_STATUS(&st)); i++) {
        st = deif_do_modify(&(sec_acl_entries[i]), DECREASE_MOD, &dummy);
    }

    return st;
}


/*
 * d e i f _ d o _ s u b s t i t u t e
 */
PUBLIC error_status_t deif_do_substitute (
    int                 num_entries,
    sec_acl_entry_t     *sec_acl_entries
)
{
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);
    if (! acl_exists) {
            /* Allocate the storage for, and copy the identifier */
        sec_acl_p->default_realm.name = (unsigned_char_p_t)
            malloc((strlen((char *)my_cell_name)+1)*sizeof(unsigned_char_t));
        strcpy((char *) sec_acl_p->default_realm.name, (char *) my_cell_name);
        sec_acl_p->default_realm.uuid = my_cell_id;
        acl_exists = true;
    } else
        deif_free_entries();

    /* Substitute the given entries into the sec_acl_entries list */
    sec_acl_p->num_entries = num_entries;
    free((char *) sec_acl_p->sec_acl_entries);
    sec_acl_p->sec_acl_entries = sec_acl_entries;

    return st;
}


/*
 * d e i f _ d o _ t e s t _ a c c e s s
 */
PUBLIC error_status_t deif_do_test_access (
    char                    *perms,
    sec_acl_edit_output_t   output_type,
    void                    *output
)
{
    error_status_t  st;
    boolean32       granted;
    sec_acl_permset_t   desired_permset;

    st = deif_decode_permset(perms, &desired_permset);

    if (GOOD_STATUS(&st))
        granted = sec_acl_test_access (sec_acl_handle, &sec_acl_mgr_type,
                                        desired_permset, &st);

    if (GOOD_STATUS(&st))
        if (output_type == TTY_OUTPUT) {
            printf("Access: %s\n", (granted) ? "GRANTED" : "DENIED");
        }
    return st;
}


/*
 * d e i f _ d o _ g e t _ a c c e s s
 */
PUBLIC error_status_t deif_do_get_access (
    sec_acl_edit_output_t   output_type,
    void                    *output
)
{
    error_status_t      st;
    sec_acl_permset_t   granted_perms;

    sec_acl_get_access(sec_acl_handle, &sec_acl_mgr_type, &granted_perms, &st);
    if (GOOD_STATUS(&st)) {
        if (output_type == TTY_OUTPUT) {
            printf("Granted permissions: ");
            deif_permset_output(granted_perms);
            printf("\n");
        }
    }
    return st;
}


/*
 * d e i f _ s a v e _ s e c _ a c l
 */
PUBLIC error_status_t deif_save_sec_acl(
    void
)
{
    error_status_t  st;

    CLEAR_STATUS(&st);
    if (acl_exists)
        sec_acl_replace (sec_acl_handle, &sec_acl_mgr_type, sec_acl_type,
                            &sec_acl_list, &st);
    return st;
}


/*
 * d e i f _ s e t _ d e f a u l t _ c e l l
 */
PUBLIC error_status_t deif_set_default_cell (
    char                *name
)
{
    error_status_t              st, lst;
    sec_rgy_name_t              cell_name;
    uuid_t                      cell_id;
    sec_rgy_bind_auth_info_t    auth_info;

    SET_STATUS(&st, error_status_ok);

    /* Make sure we have a valid sec_acl for the replace */
    if (! acl_exists) {
        sec_acl_p->num_entries = 0;
    sec_acl_p->sec_acl_manager_type = sec_acl_mgr_type;
        acl_exists = true;
    }

    sec_id_parse_name(rgy_h, (unsigned_char_p_t) name, cell_name, &cell_id,
                        NULL, NULL, &st);
    if (GOOD_STATUS(&st)) {
        if (sec_acl_p->default_realm.name) {
            free(sec_acl_p->default_realm.name);
        }
        sec_acl_p->default_realm.name = (unsigned_char_p_t)
            /* Allocate the storage for, and copy the identifier */
            malloc((strlen((char *)cell_name)+1)*sizeof(unsigned_char_t));
        strcpy((char *) sec_acl_p->default_realm.name, (char *) cell_name);
        sec_acl_p->default_realm.uuid = cell_id;
        /* Store it in the cache data too, for use by other rtns */
        def_cell_id = cell_id;
        strcpy((char *) def_cell_name, (char *) cell_name);
        if (def_cell_valid && (def_cell_rgy_h != rgy_h)) {
            sec_rgy_site_close(def_cell_rgy_h, &lst);
        }

        def_cell_valid = true;
        if (uuid_equal(&my_cell_id, &def_cell_id, &lst)) {
            def_cell_rgy_h = rgy_h;
        } else {
            /* Get a handle to the default cell's registry
             * for later use
             */
            auth_info.info_type = sec_rgy_bind_auth_dce;
            auth_info.tagged_union.dce_info.authn_level =
                                            rpc_c_protect_level_pkt_integ;
            auth_info.tagged_union.dce_info.authn_svc =
                                            rpc_c_authn_dce_secret;
            auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
            auth_info.tagged_union.dce_info.identity  = NULL;

            sec_rgy_cell_bind(def_cell_name, &auth_info,
                                &def_cell_rgy_h, &st);
            if (STATUS_EQUAL(&st, sec_rgy_cant_authenticate)) {
                auth_info.info_type = sec_rgy_bind_auth_none;
                sec_rgy_cell_bind(def_cell_name, &auth_info,
                                    &def_cell_rgy_h, &lst);
                if (GOOD_STATUS(&lst)) {
                    deif_msg_print("Warning - binding to default\
 cell registry is unauthenticated\n", false);
                } else {
                    return lst;
                }
            }
        }
    }

    return st;
}


/*
 * d e i f _ b i n d
 */
PUBLIC boolean32 deif_bind (
    char                        *object_name,
    boolean32                   bind_to_entry,
    sec_acl_type_t              sec_acl_type,
    sec_acl_edit_mask_calc_t    *mask_calc_type_p
)
{
    error_status_t  st;
    SET_STATUS(&st, error_status_ok);

    sec_acl_bind((unsigned_char_p_t)object_name, bind_to_entry,
                &sec_acl_handle, &st);

    /* Save the entry name data for later output */
    strncpy(sec_acl_entry_name, object_name, BUFSIZ);

    if (GOOD_STATUS(&st) || STATUS_EQUAL(&st,sec_acl_unable_to_authenticate)) {
        if (STATUS_EQUAL(&st, sec_acl_unable_to_authenticate)) {
            deif_msg_print(
                "Warning - binding to ACL's server is unauthenticated\n",
                false);
        }
        /* Cache the ACL manager type and printstring representations */
        st = deif_get_cache_data(sec_acl_type, mask_calc_type_p);
        if (GOOD_STATUS(&st) || STATUS_EQUAL(&st, sec_acl_no_acl_found))
            return(true);
        else {
            deif_status_print(&st, false);
            return(false);
        }
    } else {
        deif_status_print(&st, false);
        return(false);
    }
}


/*
 * d e i f _ d e b u g _ b i n d
 */
PUBLIC boolean32 deif_debug_bind (
    char                        *addr,
    char                        *residual,
    sec_acl_type_t              sec_acl_type,
    sec_acl_edit_mask_calc_t    *mask_calc_type_p
)
{
    error_status_t  st;
    SET_STATUS(&st, error_status_ok);

    if (! GOOD_STATUS(&st))
        fprintf(stderr,"INTERNAL ERROR: can't get nil uuid\n");

    sec_acl_bind_to_addr((unsigned_char_p_t) addr, (unsigned_char_p_t)residual,
                        &sec_acl_handle, &st);

    /* Save the entry name data for later output */
    strncpy(sec_acl_entry_name, addr, BUFSIZ);
    strcat(sec_acl_entry_name, " ");
    strncat(sec_acl_entry_name, residual,
            BUFSIZ - (strlen(sec_acl_entry_name)+1));

    if (GOOD_STATUS(&st) || STATUS_EQUAL(&st,sec_acl_unable_to_authenticate)) {
        /* Cache the ACL manager type and printstring representations */
        st = deif_get_cache_data(sec_acl_type, mask_calc_type_p);
        if (GOOD_STATUS(&st) || STATUS_EQUAL(&st, sec_acl_no_acl_found))
            return(true);
        else {
            deif_status_print(&st, false);
            return(false);
        }
    } else {
        deif_status_print(&st, false);
        return(false);
    }
}
