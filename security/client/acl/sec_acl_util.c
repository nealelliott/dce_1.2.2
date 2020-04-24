/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_acl_util.c,v $
 * Revision 1.1.4.2  1996/03/11  13:26:02  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:06  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:54:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:28:16  root]
 * 
 * Revision 1.1.2.4  1994/06/09  19:19:44  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  19:19:06  annie]
 * 
 * 	cr10871 - expand copyright
 * 
 * 	cr10871 - expand copyright
 * 
 * Revision 1.1.2.3  1994/06/02  22:02:14  mdf
 * 	Merged with changes from 1.1.2.2
 * 	[1994/06/02  22:01:50  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:15  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.2.2  1994/06/02  20:20:27  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:15  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * $EndLog$
 */

/* sec_acl_util.c
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

#include <stdlib.h>
#include <dce/uuid.h>
#include <dce/sec_acl_encode.h>
#include <macros.h>

/*
 * if function pointer is NULL, don't deallocate
 */
#define DEALLOCATE(deallocator,ptr)  if (ptr) { \
    if (deallocator) (*(deallocator))((ptr)); else rpc_ss_client_free((ptr)); \
}

/* convenience typedefs */
typedef idl_void_p_t (*allocate_fn_t)(idl_size_t size);
typedef void (*free_fn_t)(idl_void_p_t ptr);

static uuid_t acl__extended_type = {
    /* 85beaa1c-622d-11cd-b49c-0800097086e0 */
    0x85beaa1c,
    0x622d,
    0x11cd,
    0xb4,
    0x9c,
    {0x08, 0x00, 0x09, 0x70, 0x86, 0xe0}
};

#define BUILD_EXTENDED_ACL_ENTRY(num_bytes,bytesP,einfoP) {   \
    einfoP = (sec_acl_extend_info_t *) alloc(sizeof(uuid_t) + \
        sizeof(ndr_format_t) + sizeof(unsigned32) +           \
            (num_bytes * sizeof(ndr_byte)) );                 \
    einfoP->extension_type = acl__extended_type;              \
    einfoP->format_label.int_rep = 0;   \
    einfoP->format_label.char_rep = 0;  \
    einfoP->format_label.float_rep = 0; \
    einfoP->format_label.reserved = 0;  \
    einfoP->num_bytes = num_bytes;      \
    memcpy(einfoP->pickled_data, bytesP, num_bytes); \
    DEALLOCATE(dealloc, bytesP); \
}


/*
 * sec_acl_encode_v0_entries
 * 
 * For any entries with types that are new to version 1.0 of the rdaclif wire
 * interface, encode that entry and replace it with an extended entry type
 * containing the encoded bytes. 
 */
PUBLIC void sec_acl_encode_v0_entries(
    idl_void_p_t        (*alloc)(idl_size_t size),     /* [in] */
    void                (*dealloc)(idl_void_p_t ptr),  /* [in] */
    boolean32           free_old_entries,              /* [in] */
    sec_acl_list_t      *aclsP,                        /* [in,out] */
    error_status_t      *stp                           /* [out] */
)
{
    sec_acl_extend_info_t   *einfoP;
    int                     i, j;
    unsigned32              num_bytes;
    idl_byte                *bytesP;

    /* Go through the list of acls */
    for (i = 0; i < aclsP->num_acls; i++) {
	
	/* Go through the list of entries on an acl */
        for (j  = 0;(*stp == 0) && (j < aclsP->sec_acls[i]->num_entries);j++) {

            /* For any new types, replace it with an extended (pickled) type */
            switch(
                aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.entry_type) {
            case sec_acl_e_type_user_obj_deleg:
            case sec_acl_e_type_group_obj_deleg:
            case sec_acl_e_type_other_obj_deleg:
            case sec_acl_e_type_any_other_deleg:
                sec_acl_entry_encode(alloc, dealloc,
                    &aclsP->sec_acls[i]->sec_acl_entries[j], &num_bytes,
                    &bytesP, stp);
                if (GOOD_STATUS(stp)) {
                  aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.entry_type=
                    sec_acl_e_type_extended;
                  BUILD_EXTENDED_ACL_ENTRY(num_bytes, bytesP, einfoP);
                  aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.tagged_union.extended_info = einfoP;
                }
                break;
            case sec_acl_e_type_user_deleg:
            case sec_acl_e_type_group_deleg:
            case sec_acl_e_type_for_other_deleg:
                sec_acl_entry_encode(alloc, dealloc,
                    &aclsP->sec_acls[i]->sec_acl_entries[j], &num_bytes,
                    &bytesP, stp);
                if (GOOD_STATUS(stp)) {
                  /* Free the name from the entry if the caller wants us to */
                  if (free_old_entries) {
                      DEALLOCATE(dealloc, aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.tagged_union.id.name);
                  }
                  aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.entry_type =
                    sec_acl_e_type_extended;
                  BUILD_EXTENDED_ACL_ENTRY(num_bytes, bytesP, einfoP);
                  aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.tagged_union.extended_info = einfoP;
                }
                break;
            case sec_acl_e_type_for_user_deleg:
            case sec_acl_e_type_for_group_deleg:
                sec_acl_entry_encode(alloc, dealloc,
                    &aclsP->sec_acls[i]->sec_acl_entries[j], &num_bytes,
                    &bytesP, stp);
                if (GOOD_STATUS(stp)) {
                  /* Free foreign names from the entry if caller wants us to */
                  if (free_old_entries) {
                      DEALLOCATE(dealloc, aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.tagged_union.foreign_id.realm.name);
                      DEALLOCATE(dealloc, aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.tagged_union.foreign_id.id.name);
                  }
                  aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.entry_type=
                    sec_acl_e_type_extended;
                  BUILD_EXTENDED_ACL_ENTRY(num_bytes, bytesP, einfoP);
                  aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.tagged_union.extended_info = einfoP;
                }
                break;
            }

        }
    }
}


/*
 * sec_acl_decode_v0_entries
 *
 * For any extended types, attempt to decode the pickle into an acl entry type.
 * If successful, replace the extended type with the decoded type.
 */
PUBLIC void sec_acl_decode_v0_entries(
    idl_void_p_t        (*alloc)(idl_size_t size),     /* [in] */
    void                (*dealloc)(idl_void_p_t ptr),  /* [in] */
    boolean32           free_old_entries,              /* [in] */
    sec_acl_list_t      *aclsP,                        /* [in,out] */
    error_status_t      *stp                           /* [out] */
)
{
    sec_acl_entry_t         acl_entry;
    sec_acl_permset_t       perms;
    int                     i, j;
    unsigned32              num_bytes;
    idl_byte                *bytesP;
    error_status_t          tmp_st;

    /* Go through the list of acls */
    for (i = 0; i < aclsP->num_acls; i++) {

        /* Go through the list of entries on an acl */
        for (j  = 0;(*stp == 0) && (j < aclsP->sec_acls[i]->num_entries);j++) {

            if ((aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.entry_type==
                 sec_acl_e_type_extended) &&
                (uuid_equal(&aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.tagged_union.extended_info->extension_type, &acl__extended_type, &tmp_st))) {

                /* Decode the pickled entry */
                sec_acl_entry_decode(alloc, dealloc,
                    aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.tagged_union.extended_info->num_bytes,
                    aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.tagged_union.extended_info->pickled_data,
                    &acl_entry, stp);

                /* If the decode succeeded... */
                if (GOOD_STATUS(stp)) {

                    /* Free memory for the extended entry if caller said so */
                    if (free_old_entries)
                        DEALLOCATE(dealloc, aclsP->sec_acls[i]->sec_acl_entries[j].entry_info.tagged_union.extended_info);

                    /* Replace the extended entry with the decoded one */
                    aclsP->sec_acls[i]->sec_acl_entries[j].entry_info =
                        acl_entry.entry_info;
                }
            }
        }
    }
}
