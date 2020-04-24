/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: pkss_acl_mgr.cxx,v $
 * Revision 1.1.2.1  1996/10/04  20:00:04  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  19:59:41  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:27 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:04 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module:
 *
 * Purpose:
 *
 */

extern "C" {
#include <dce/dce.h>
#include <dce/aclif.h>
#include <dce/rdaclbase.h>
#include <dce/rdaclif.h>
#include <dce/sec_acl_encode.h>
#include <dce/binding.h>
#include <dce/policy.h>
#include <dce/pgo.h>
#include <string.h>
#include "dcepksmac.h"
#include "dcepkssvc.h"
}
#include "pkss_acl_mgr.h"
#include "sec_pkss_db.h"
#include "sec_pkss_dbkey_class.h"
#include "pkss_config_server.h"

extern dce_svc_handle_t pks_svc_handle;

#ifndef DEBUG
#define DEBUG
#endif

Gdb44C * database;

#define ROOT_OBJECT_NAME "*"
const char * root_name = ROOT_OBJECT_NAME;

#define PRINCIPAL_ACL_TAG (const char *)"PRINCIPAL-ACL"
#define CONTAINER_ACL_TAG (const char *)"CONTAINER-ACL"
#define DEFAULT_PRINCIPAL_ACL_TAG (const char *)"DEFAULT-PRINCIPAL-ACL"
#define DEFAULT_CONTAINER_ACL_TAG (const char *)"DEFAULT-CONTAINER-ACL"
#define SERVER_ACL_TAG (const char *)"SERVER-ACL"

#define RGY_RETRY_COUNT 15


static sec_acl_printstring_t pkss_server_printstrings[] = {
    { "c", "control", sec_acl_perm_control},
    { "r", "read statistics", sec_acl_perm_read},
    { "s", "shutdown", sec_acl_perm_write},
};

static sec_acl_printstring_t pkss_principal_printstrings[] = {
    { "c", "control", sec_acl_perm_control},
    { "r", "read attributes", sec_acl_perm_read},
    { "m", "modify", sec_acl_perm_write},
    { "d", "delete", sec_acl_perm_delete},
};

static sec_acl_printstring_t pkss_container_printstrings[] = {
    { "c", "control", sec_acl_perm_control},
    { "r", "read attributes", sec_acl_perm_read},
    { "i", "insert", sec_acl_perm_insert},
    { "d", "delete", sec_acl_perm_delete},
};

static sec_acl_printstring_t pkss_server_info =
{ "pkssd", 
  "Private Key Storage Server", 
  sec_acl_perm_control | sec_acl_perm_read | sec_acl_perm_write
};

static sec_acl_printstring_t pkss_principal_info =
{ "key", 
  "Private Key Storage Service key entry", 
  sec_acl_perm_control
  | sec_acl_perm_read
  | sec_acl_perm_write
  | sec_acl_perm_delete
};

static sec_acl_printstring_t pkss_container_info =
{ "pksdirectory", 
  "Private Key Storage Service container entry", 
  sec_acl_perm_control
  | sec_acl_perm_read
  | sec_acl_perm_insert
  | sec_acl_perm_delete
};

static sec_acl_permset_t default_principal_acl_permissions =
    sec_acl_perm_read | sec_acl_perm_write
    | sec_acl_perm_delete | sec_acl_perm_control;
    // mgmt_group = read, write, delete, control

static sec_acl_permset_t default_container_acl_permissions = 
    sec_acl_perm_read | sec_acl_perm_insert
    | sec_acl_perm_control;
    // mgmt_group = read, insert (records), control,

static sec_acl_permset_t default_server_acl_permissions = 
    sec_acl_perm_read | sec_acl_perm_write
    | sec_acl_perm_control;
    // mgmt_group = read (statistics), write (shutdown), control, 


static sec_rgy_properties_t cell_properties;
static sec_rgy_name_t admin_group_name;
static uuid_t admin_group_uuid;

static dce_db_handle_t acl_db = NULL;


int uuid_equal(const uuid_t & o1, const uuid_t & o2) {
    return ((o1.time_low == o2.time_low) &&
            (o1.time_mid == o2.time_mid) &&
            (o1.time_hi_and_version == o2.time_hi_and_version) &&
            (o1.clock_seq_hi_and_reserved == o2.clock_seq_hi_and_reserved) &&
            (o1.clock_seq_low == o2.clock_seq_low) &&
            (o1.node[0] == o2.node[0]) &&
            (o1.node[1] == o2.node[1]) &&
            (o1.node[2] == o2.node[2]) &&
            (o1.node[3] == o2.node[3]) &&
            (o1.node[4] == o2.node[4]) &&
            (o1.node[5] == o2.node[5]));
}

char * container_name(const char * principal_name) {
    char * ret;
    ret = (char *)malloc(strlen(root_name)+1);
    strcpy(ret, root_name);
    return ret;
}

const char * norm(const char * s) {
    if ((s == NULL) || (s[0] == 0)) return root_name;
    else return s;
}

const unsigned char * norm(const unsigned char * s) {
    if ((s == NULL) || (s[0] == 0)) return (unsigned char *)root_name;
    else return s;
}

unsigned32 pkss_initialize_acl_manager(Gdb44C * db_ptr,
                                       uuid_t * uuid,
                                       rpc_if_handle_t * ifspec) {
    sec_rgy_handle_t rgy_handle;
    unsigned32 st;
    unsigned32 est;
    int retry_count;
    
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
		   "Initializing ACL manager"));

    database = db_ptr;

    strncpy((char *)admin_group_name,
            pkss_config__security__admin_group_name(),
            sizeof(admin_group_name));

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
		   "PKSS default administrative group: \"%s\"",
		   admin_group_name));

    st = 1; retry_count = RGY_RETRY_COUNT;
    while (st && (retry_count > 0)) {
        retry_count --;
        sec_rgy_site_open_query((unsigned_char_t *)"/.:",
                                &rgy_handle,
                                &st);

        if (st) {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
                          "Error contacting registry: 0x%x",
                          st));
	    continue;
	};
                
        sec_rgy_properties_get_info(rgy_handle,
                                    &cell_properties,
                                    &st);


        if (st) {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
                          "Error retrieving cell properties: 0x%x",
                          st));
	    continue;
	};

        sec_rgy_pgo_name_to_id(rgy_handle,
                               sec_rgy_domain_group,
                               admin_group_name,
                               &admin_group_uuid,
                               &st);
    };

    sec_rgy_site_close(rgy_handle, &est);
    if (st) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
	"Fatal error while contacting registry.  Status: 0x%x, retry count: %d",
                       st, retry_count));
        return st;
    };

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
		   "Registering ACL manager types"));

    dce_acl_register_object_type(acl_db,
                                 (uuid_t *)&principal_acl_type,
                                 4,
				 pkss_principal_printstrings,
				 &pkss_principal_info,
                                 sec_acl_perm_control,
                                 sec_acl_perm_read,
				 NULL, // Resolver
				 NULL, // Resolver-arg
				 0,
				 &st);		 
    if (st) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
                      "Can't register principal ACL manager (0x%x)",
		      st));
    };

    dce_acl_register_object_type(acl_db,
                                 (uuid_t *)&container_acl_type,
                                 4,
				 pkss_container_printstrings,
				 &pkss_container_info,
                                 sec_acl_perm_control,
                                 sec_acl_perm_read,
				 NULL, // Resolver
				 NULL, // Resolver-arg
				 0,
				 &st);		 
    if (st) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
                      "Can't register container ACL manager (0x%x)",
		      st));
    };

    dce_acl_register_object_type(acl_db,
                                 (uuid_t *)&server_acl_type,
                                 3,
				 pkss_server_printstrings,
				 &pkss_server_info,
                                 sec_acl_perm_control,
                                 sec_acl_perm_read,
				 NULL, // Resolver
				 NULL, // Resolver-arg
				 0,
				 &st);		 
    if (st) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
                      "Can't register server ACL manager (0x%x)",
		      st));
    };
				 

// Check to see whether we need to create ACL entries:

    if (pkss_retrieve_principal_acl(NULL, norm("*")) ||
        pkss_retrieve_container_acl(NULL, norm("*")) ||
        pkss_retrieve_default_principal_acl(NULL, norm("*")) ||
        pkss_retrieve_default_container_acl(NULL, norm("*"))) {

        // We need to create the ACLs on the root object

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                      "Uninitialized database.  Creating default ACLs."));

	st = pkss_add_acl("*");

	if (st) {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
	                   "pkss_add_acl faileod in initialization (0x%x)",
			   st));
	};
    };
	

    *ifspec = rdaclif_v1_0_s_ifspec;
    *uuid = principal_acl_type;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
		  "ACL initialization complete.  FInal status: 0x%x",
		  st));

    return st;

}

static unsigned32 encode_acl_entry(const sec_acl_entry_t * acl_entry_ptr,
                                   unsigned char ** ptr,
                                   unsigned32 * length) {

    unsigned32 st;
    idl_byte *entry_bytes;
    unsigned32 num_entry_bytes;

    sec_acl_entry_encode(NULL, // Use malloc
                         NULL, // Use free
			 (sec_acl_entry_t *)acl_entry_ptr,
                         &num_entry_bytes,
                         &entry_bytes,
                         &st);
    *ptr = entry_bytes;
    *length = num_entry_bytes;
    return st;    
}



// An ACL consists of a header and a number of entries.
// The header is as follows:
//
//    num_entries;   // 4 bytes
//    uuid_t default_realm_id;  // 16 bytes
//    uuid_t manager_type;      // 16 bytes
//


static void write(unsigned char * &ptr, unsigned16 value) {
    *ptr++ = (unsigned char)(value & 0xffu);
    *ptr++ = (unsigned char)((value >> 8) & 0xffu);
}

static void write(unsigned char * &ptr, unsigned32 value) {
    *ptr++ = (unsigned char)(value & 0xffu);
    *ptr++ = (unsigned char)((value >> 8) & 0xffu);
    *ptr++ = (unsigned char)((value >> 16) & 0xffu);
    *ptr++ = (unsigned char)((value >> 24) & 0xffu);
}

static void write(unsigned char * &ptr, unsigned char value) {
    *ptr++ = value;
}

static void write(unsigned char * &ptr, uuid_t uuid) {
    write(ptr, uuid.time_low);
    write(ptr, uuid.time_mid);
    write(ptr, uuid.time_hi_and_version);
    write(ptr, uuid.clock_seq_hi_and_reserved);
    write(ptr, uuid.clock_seq_low);
    write(ptr, uuid.node[0]);
    write(ptr, uuid.node[1]);
    write(ptr, uuid.node[2]);
    write(ptr, uuid.node[3]);
    write(ptr, uuid.node[4]);
    write(ptr, uuid.node[5]);
}

static void write(unsigned char * &ptr, unsigned char * buf, unsigned32 len)
{
    write(ptr, len);
    memcpy(ptr, buf, len);
    ptr += len;
}

static void read(unsigned char * &ptr, unsigned16 & value) {
    value = (unsigned16)(*ptr++);
    value += ((unsigned16)(*ptr++)) << 8;
}

static void read(unsigned char * &ptr, unsigned32 &value) {
    value = (unsigned32)(*ptr++);
    value += ((unsigned32)(*ptr++)) << 8;
    value += ((unsigned32)(*ptr++)) << 16;
    value += ((unsigned32)(*ptr++)) << 24;
}

static void read (unsigned char * &ptr, unsigned char &value) {
    value = *ptr++;
}

static void read(unsigned char * &ptr, unsigned char * &buf, unsigned32 &len)
{
    read(ptr, len);
    buf = (unsigned char *)malloc(len);
    memcpy(buf, ptr, len);
    ptr += len;
}

static void read(unsigned char * &ptr, uuid_t &uuid) {
    read(ptr, uuid.time_low);
    read(ptr, uuid.time_mid);
    read(ptr, uuid.time_hi_and_version);
    read(ptr, uuid.clock_seq_hi_and_reserved);
    read(ptr, uuid.clock_seq_low);
    read(ptr, uuid.node[0]);
    read(ptr, uuid.node[1]);
    read(ptr, uuid.node[2]);
    read(ptr, uuid.node[3]);
    read(ptr, uuid.node[4]);
    read(ptr, uuid.node[5]);
}

static unsigned32 encode_acl(const sec_acl_t * acl_ptr,
                             unsigned char ** ptr,
                             unsigned32 * length) {

    unsigned32 st;
    char * acl_bytes;
    int i;
    unsigned32 num_bytes = 0;
    unsigned char ** entry = NULL;
    unsigned32 * entry_length = NULL;
    unsigned32 running_total = 0;
    unsigned char * c_ptr;
    unsigned32 temp_length;

    *ptr = NULL;
    *length = 0;

    running_total = 16 + 16 + 4;  // Header space for 2 UUIDs and an entry
				  // count.


// The next allocation relies on calloc zeroing memory.
// Allocate an array of pointers for the ACL entries.
    entry = (unsigned char **)calloc(acl_ptr->num_entries,
                                     sizeof(unsigned char *));
    if (entry == NULL) {
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
                      "No memory for %d ACL entries",
                      acl_ptr->num_entries));
        st = pks_s_no_memory;
        goto end;
    };

// Allocate an array for entry sizes...
    entry_length = (unsigned32 *)malloc(sizeof(unsigned32)
                                        * acl_ptr->num_entries);
    if (entry_length == NULL) {
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
                      "No memory for %d ACL entry sizes",
                      acl_ptr->num_entries));
        st = pks_s_no_memory;
        goto end;
    };
// Loop through entries, encoding buffers.
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                  "Encoding an ACL with %d entries",
                  acl_ptr->num_entries));
    for (i=0;i<acl_ptr->num_entries;i++) {
        st = encode_acl_entry(&(acl_ptr->sec_acl_entries[i]),
                              &entry[i],
                              &entry_length[i]);
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                      "Encoded ACL entry %d in %d bytes",
                      i, entry_length[i]));

        if (st) {
	    dce_svc_printf(PKS_S_ACL_ENCODE_FAIL2_MSG,
			   (unsigned long)st);
	    break;
	};
        running_total += entry_length[i] + sizeof(unsigned32);
    };
    if (st) goto end;


// Now we have everything we need to construct a single buffer containing the
// ACL.  Allocate it and fill it up.

    *ptr = (unsigned char *) malloc(running_total);
    if (*ptr == NULL) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
                     "Insufficient memory for ACL encoding (%d bytes)",
		     running_total));

        st = pks_s_no_memory;
        goto end;
    };

    *length = running_total;
    c_ptr = *ptr;
// Write header:
    write(c_ptr, acl_ptr->default_realm.uuid);
    write(c_ptr, acl_ptr->sec_acl_manager_type);
    write(c_ptr, acl_ptr->num_entries);

// Write entries:
    for (i=0;i<acl_ptr->num_entries;i++) {
        write(c_ptr, entry[i], entry_length[i]);
    };


end:
    if (entry) {
        for (i=0;i<acl_ptr->num_entries;i++) {
            if (entry[i]) free(entry[i]);
        };
        free(entry);
    };
    if (entry_length) free(entry_length);
    if (st) {
        if (*ptr == NULL) {
            free(*ptr);
            *ptr = NULL;
        };
        *length = 0;
    };
    return st;
}


static unsigned32 decode_acl_entry(unsigned char * ptr,
                                   unsigned32 length,
                                   sec_acl_entry_t * acl_entry_ptr) {

    unsigned32 st;
    unsigned32 num_entry_bytes = length;
    idl_byte *entry_bytes = ptr;

    sec_acl_entry_decode(NULL, // Use malloc
                         NULL, // Use free
                         num_entry_bytes,
                         entry_bytes,
                         acl_entry_ptr,
                         &st);
    return st;

}

static unsigned32 decode_acl(unsigned char * ptr,
                             size_t length,
                             sec_acl_t * acl_ptr) {

    unsigned32 st;
    unsigned32 est;
    unsigned32 num_entries;
    unsigned32 entry_length;
    uuid_t realm_id;
    uuid_t acl_type;
    sec_acl_entry_t acl_entry;
    int i;

    if (length < 36) return pks_s_bad_acl;

    read(ptr, realm_id);
    read(ptr, acl_type);
    read(ptr, num_entries);

    dce_acl_obj_init(&acl_type, acl_ptr, &st);
    if (st) goto end;
    acl_ptr->default_realm.uuid = realm_id;

    length -= 36;

    for (i=0; i<num_entries; i++) {
        if (length < 4) {
            st = pks_s_bad_acl;
            break;
        };
        read(ptr, entry_length);
        length -= 4;
                
        if (length < entry_length) {
            st = pks_s_bad_acl;
            break;
        };

        st = decode_acl_entry(ptr,
                              entry_length,
                              &acl_entry);
        if (st) break;
        length -= entry_length;

// Now we have the ACL entry.  Use the dce_acl_ library to add it to the
// ACL we're building.  Using the dce_acl library like this means that we'll
// adapt OK to any future ACL format changes, and we also can use the dce_acl
// to do memory management for us.
        switch (acl_entry.entry_info.entry_type) {
        case sec_acl_e_type_foreign_user:
        case sec_acl_e_type_foreign_group:
        case sec_acl_e_type_for_user_deleg:
        case sec_acl_e_type_for_group_deleg:
            dce_acl_obj_add_foreign_entry(
                       acl_ptr,
                       acl_entry.entry_info.entry_type,
                       acl_entry.perms,
                       &acl_entry.entry_info.tagged_union.foreign_id.realm.uuid,
                       &acl_entry.entry_info.tagged_union.foreign_id.id.uuid,
                       &st);
            break;
        case sec_acl_e_type_user:
        case sec_acl_e_type_group:
        case sec_acl_e_type_foreign_other:
        case sec_acl_e_type_user_deleg:
        case sec_acl_e_type_group_deleg:
        case sec_acl_e_type_for_other_deleg:
            dce_acl_obj_add_id_entry(
                       acl_ptr,
                       acl_entry.entry_info.entry_type,
                       acl_entry.perms,
                       &acl_entry.entry_info.tagged_union.id.uuid,
                       &st);
            break;            
        case sec_acl_e_type_unauthenticated:
        case sec_acl_e_type_any_other:
        case sec_acl_e_type_user_obj_deleg:
        case sec_acl_e_type_group_obj_deleg:
        case sec_acl_e_type_other_obj_deleg:
        case sec_acl_e_type_any_other_deleg:

            dce_acl_obj_add_obj_entry(
                       acl_ptr,
                       acl_entry.entry_info.entry_type,
                       acl_entry.perms,
                       &st);
            break;            
        case sec_acl_e_type_user_obj:
        case sec_acl_e_type_group_obj:
        case sec_acl_e_type_other_obj:
        case sec_acl_e_type_mask_obj:
// For some reason, there's no dce_acl_XXX routine for these types,
// so we have to kludge it...
            dce_acl_obj_add_obj_entry(
                       acl_ptr,
                       sec_acl_e_type_user_obj_deleg,
                       acl_entry.perms,
                       &st);
            acl_ptr->sec_acl_entries[acl_ptr->num_entries-1].
                                            entry_info.entry_type = 
                                                acl_entry.entry_info.entry_type;
            break;
        default:
// Don't dother with extended entries.
            ;
        };
        sec_acl_entry_decoded_data_free(NULL, &acl_entry);
        if (st) break;
    };


end:

    if (st) {
        dce_acl_obj_free_entries(acl_ptr, &est);
    };    
    return st;
}


unsigned32 pkss_store_principal_acl(
                      const char * principal_name,
                      const sec_acl_t * acl) {

    unsigned32 st;
    unsigned32 est;
    unsigned char * acl_data;
    unsigned32 acl_length;
    const sec_pkss_dbkey_t * key;
    char * container;
    sec_acl_t local_acl;
    char local_acl_created = 0;
    PkssDbKeyC dbKey(norm((const char *)principal_name),
                    PRINCIPAL_ACL_TAG, 0, 0);

    key = dbKey.p();


    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Storing an object ACL for node %s", principal_name));
    

    if (acl == NULL) {
// Use the default object ACL from our container object.

	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		      "Defaulting ACL value"));

        container = container_name(principal_name);
        st = pkss_retrieve_default_principal_acl(&local_acl, container);
        if (st) {
	    dce_svc_printf(PKS_S_ACL_DEFOBJ_RETRIEVE_FAIL1_MSG,
	                   container, (unsigned long)st);
	};
	free(container);
        if (st) {
	    return st;
	};
        local_acl_created = 1;
        acl = &local_acl;
    };
    
    st = encode_acl(acl, &acl_data, &acl_length);

    if (st) {
	dce_svc_printf(PKS_S_ACL_ENCODE_FAIL3_MSG,
		       (unsigned long)st);
        if (local_acl_created) dce_acl_obj_free_entries(&local_acl, &est);
        return st;
    };

    database->txnBegin();
    st = database->write((char *)acl_data, acl_length,
                         (char *)(key->data), key->len);
    if (st == 0) database->txnEnd();
    else database->txnCancel();
    
    if (st) {
	dce_svc_printf(PKS_S_DBWRITE_FAIL_MSG, (unsigned long)st);
    };

    free(acl_data);
    if (local_acl_created) dce_acl_obj_free_entries(&local_acl, &est);
    return st;
}

unsigned32 pkss_retrieve_principal_acl(
                      sec_acl_t * acl,
                      const char * principal_name) {
    unsigned32 st;
    unsigned32 est;
    unsigned char * acl_data;
    unsigned32 acl_length;
    const sec_pkss_dbkey_t * key;
    const char * norm_name = norm(principal_name);
    PkssDbKeyC dbKey(norm_name,
                     PRINCIPAL_ACL_TAG, 0, 0);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Retrieving an object ACL for node %s", principal_name));


    key = dbKey.p();
    
    st = database->read((char **)&acl_data, &acl_length,
                        (char *)(key->data), key->len);

    if (st) return pks_s_no_acl;

    if (acl == NULL) return 0;
    
    st = decode_acl(acl_data, acl_length, acl);

    if (st) {
	dce_svc_printf(PKS_S_ACL_DECODE_FAIL1_MSG, (unsigned long)st);
    };

    free(acl_data);

    return st;
}

unsigned32 pkss_store_container_acl(
                      const char * principal_name,
                      const sec_acl_t * acl) {

    unsigned32 st;
    unsigned32 est;
    unsigned char * acl_data;
    unsigned32 acl_length;
    const sec_pkss_dbkey_t * key;
    char * container;
    sec_acl_t local_acl;
    char local_acl_created = 0;
    PkssDbKeyC dbKey(norm(principal_name),
                     CONTAINER_ACL_TAG, 0, 0);


    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Storing a container ACL for node %s", principal_name));

    key = dbKey.p();


    if (acl == NULL) {
// Use the default object ACL from our container object.
        container = container_name(principal_name);
        st = pkss_retrieve_default_container_acl(&local_acl, container);
        if (st) {
	    dce_svc_printf(PKS_S_ACL_DEFCON_RETRIEVE_FAIL1_MSG,
	                   container, (unsigned long)st);
	};
	free(container);
        if (st) {
	    return st;
	};
        local_acl_created = 1;
        acl = &local_acl;
    };
    
    st = encode_acl(acl, &acl_data, &acl_length);

    if (st) {
	dce_svc_printf(PKS_S_ACL_ENCODE_FAIL1_MSG,
		       (unsigned long)st);

        if (local_acl_created) dce_acl_obj_free_entries(&local_acl, &est);
        return st;
    };

    database->txnBegin();
    st = database->write((char *)acl_data, acl_length,
                         (char *)(key->data), key->len);

    if (st == 0) database->txnEnd();
    else database->txnCancel();

    free(acl_data);
    if (local_acl_created) dce_acl_obj_free_entries(&local_acl, &est);
    return st;
}

unsigned32 pkss_retrieve_container_acl(
                      sec_acl_t * acl,
                      const char * principal_name) {
    unsigned32 st;
    unsigned32 est;
    unsigned char * acl_data;
    unsigned32 acl_length;
    const sec_pkss_dbkey_t * key;
    const char * norm_name = norm(principal_name);
    PkssDbKeyC dbKey(norm_name,
                     CONTAINER_ACL_TAG, 0, 0);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Retrieving a container ACL for node %s", principal_name));

    key = dbKey.p();
    
    st = database->read((char **)&acl_data, &acl_length,
                        (char *)(key->data), key->len);

    if (st) return pks_s_no_acl;

    if (acl == NULL) return 0;
    
    st = decode_acl(acl_data, acl_length, acl);

    if (st) {
	dce_svc_printf(PKS_S_ACL_DECODE_FAIL2_MSG,
		       (unsigned long)st);
    };
    free(acl_data);

    return st;
}

unsigned32 pkss_retrieve_default_acls(sec_acl_t * object_acl,
                                      sec_acl_t * container_acl,
				      const char * principal_name) {
    unsigned32 st;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Retrieving default creation ACLs for node %s", principal_name));


    st = pkss_retrieve_default_principal_acl(object_acl,
                                             principal_name);
    if (st) {
        dce_svc_printf(PKS_S_ACL_DEFOBJ_RETRIEVE_FAIL2_MSG,
                       principal_name, (unsigned long)st);
    };
    if (st == 0) st = pkss_retrieve_default_container_acl(container_acl,
                                                          principal_name);
    if (st) {
	    dce_svc_printf(PKS_S_ACL_DEFCON_RETRIEVE_FAIL5_MSG,
	                   principal_name, (unsigned long)st);
    };
    return st;
}

unsigned32 pkss_store_default_principal_acl(const char * principal_name,
                                            const sec_acl_t * acl) {
    unsigned32 st;
    unsigned32 est;
    unsigned char * acl_data;
    unsigned32 acl_length;
    const sec_pkss_dbkey_t * key;
    sec_acl_t local_acl;
    char local_acl_created = 0;
    char * container;
    
    PkssDbKeyC dbKey(norm(principal_name),
                     DEFAULT_PRINCIPAL_ACL_TAG, 0, 0);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Storing default object ACL for node %s", principal_name));
    
    key = dbKey.p();
    
    if (acl == NULL) {
                               
	if (strcmp(principal_name, root_name) == 0) {
// We're being asked to initialize the database default ACL (the
// default object ACL on the database root).
            acl = &local_acl;
            local_acl_created = 1;
            dce_acl_obj_init((uuid_t *)&principal_acl_type, &local_acl, &st);
            if (st) return st;
            local_acl.default_realm.uuid = cell_properties.realm_uuid;
            dce_acl_obj_add_group_entry(&local_acl, 
                                        default_principal_acl_permissions,
                                        &admin_group_uuid,
                                        &st);
            if (st) return st;
        } else {
// Use the default object creation ACL from our container object.
            container = container_name(principal_name);
            st = pkss_retrieve_default_principal_acl(&local_acl,
                                                     container);
            if (st) {
                dce_svc_printf(PKS_S_ACL_DEFOBJ_RETRIEVE_FAIL3_MSG,
                               container, (unsigned long)st);
	    };
            free(container);
            if (st) {
		return st;
	    };
            local_acl_created = 1;
            acl = &local_acl;
        };

    };
    
    st = encode_acl(acl, &acl_data, &acl_length);

    if (st) {
	dce_svc_printf(PKS_S_ACL_ENCODE_FAIL4_MSG,
		       (unsigned long)st);
        if (local_acl_created) dce_acl_obj_free_entries(&local_acl, &est);
        return st;
    };

    database->txnBegin();
    st = database->write((char *)acl_data, acl_length,
                         (char *)(key->data), key->len);
    if (st == 0) database->txnEnd();
    else database->txnCancel();

    free(acl_data);

    if (local_acl_created) dce_acl_obj_free_entries(&local_acl, &est);

    return st;
}

unsigned32 pkss_retrieve_default_principal_acl(sec_acl_t * acl,
                                               const char * principal_name) {
    unsigned32 st;
    unsigned char * acl_data;
    unsigned32 acl_length;
    const sec_pkss_dbkey_t * key;
    const char * norm_name = norm(principal_name);
    PkssDbKeyC dbKey(norm_name,
                     DEFAULT_PRINCIPAL_ACL_TAG, 0, 0);


    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Retrieving default object ACL for node %s", principal_name));

    key = dbKey.p();
    
    st = database->read((char **)&acl_data, &acl_length,
                        (char *)(key->data), key->len);


    if (st) return pks_s_no_acl;

    if (acl == NULL) return 0;

    st = decode_acl(acl_data, acl_length, acl);

    if (st) {
	dce_svc_printf(PKS_S_ACL_DECODE_FAIL3_MSG,
		       (unsigned long)st);
    };
    free(acl_data);

    return st;
}

unsigned32 pkss_store_default_container_acl(const char * principal_name,
                                            const sec_acl_t * acl) {
    unsigned32 st;
    unsigned32 est;
    unsigned char * acl_data;
    unsigned32 acl_length;
    const sec_pkss_dbkey_t * key;
    sec_acl_t local_acl;
    char local_acl_created = 0;
    char * container;
    const char * norm_name = norm(principal_name);   
    PkssDbKeyC dbKey(norm_name,
                     DEFAULT_CONTAINER_ACL_TAG, 0, 0);


    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Storing default container ACL for node %s", norm_name));
    
    key = dbKey.p();
    
    if (acl == NULL) {
                               
	if (strcmp(principal_name, root_name) == 0) {
// We're being asked to initialize the database default ACL (the
// default object ACL on the database root).
            acl = &local_acl;
            local_acl_created = 1;
            dce_acl_obj_init((uuid_t *)&container_acl_type, &local_acl, &st);
            if (st) return st;
            local_acl.default_realm.uuid = cell_properties.realm_uuid;
            dce_acl_obj_add_group_entry(&local_acl, 
                                        default_container_acl_permissions,
                                        &admin_group_uuid,
                                        &st);
            if (st) return st;
        } else {
// Use the default container creation ACL from our container object.
            container = container_name(principal_name);
            st = pkss_retrieve_default_container_acl(&local_acl,
                                                     container);
            if (st) {
		dce_svc_printf(PKS_S_ACL_DEFCON_RETRIEVE_FAIL2_MSG,
			       container, (unsigned long)st);
	    };
            free(container);
            if (st) {
		return st;
	    };
            local_acl_created = 1;
            acl = &local_acl;
        };

    };
    
    st = encode_acl(acl, &acl_data, &acl_length);

    if (st) {
	dce_svc_printf(PKS_S_ACL_ENCODE_FAIL5_MSG,
		       (unsigned long)st);
        if (local_acl_created) dce_acl_obj_free_entries(&local_acl, &est);
        return st;
    };

    database->txnBegin();
    st = database->write((char *)acl_data, acl_length,
                         (char *)key->data, key->len);

    if (st == 0) database->txnEnd();
    else database->txnCancel();

    free(acl_data);

    if (local_acl_created) dce_acl_obj_free_entries(&local_acl, &est);

    return st;
}

unsigned32 pkss_retrieve_default_container_acl(sec_acl_t * acl,
                                               const char * principal_name) {
    unsigned32 st;
    unsigned char * acl_data;
    unsigned32 acl_length;
    const sec_pkss_dbkey_t * key;
    const char * norm_name = norm(principal_name);
    PkssDbKeyC dbKey(norm_name,
                     DEFAULT_CONTAINER_ACL_TAG, 0, 0);


    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Retrieving default container ACL for node %s", norm_name));

    key = dbKey.p();
    
    st = database->read((char **)&acl_data, &acl_length,
                        (char *)(key->data), key->len);


    if (st) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                      "db->Read() returned 0x%x", st));
        return pks_s_no_acl;
    };
    
    if (acl == NULL) return 0;

    st = decode_acl(acl_data, acl_length, acl);

    if (st) {
	dce_svc_printf(PKS_S_ACL_DECODE_FAIL4_MSG,
		       (unsigned long)st);
    };
    free(acl_data);

    return st;
}

unsigned32 pkss_store_server_acl(const sec_acl_t * acl) {
    unsigned32 st;
    unsigned32 est;
    unsigned char * acl_data;
    unsigned32 acl_length;
    const sec_pkss_dbkey_t * key;
    sec_acl_t local_acl;
    char local_acl_created = 0;
    PkssDbKeyC dbKey(ROOT_OBJECT_NAME,
                     SERVER_ACL_TAG, 0, 0);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Storing server management ACL"));

    key = dbKey.p();
    
    if (acl == NULL) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug8,
                 "No ACL specified - creating default management ACL"));

        acl = &local_acl;
        local_acl_created = 1;
        dce_acl_obj_init((uuid_t *)&server_acl_type,
	                 &local_acl,
			 &st);
        if (st) {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
             "dce_acl_obj_init failed (0x%x).  Can't create ACL"));
	    return st;
	};

        local_acl.default_realm.uuid = cell_properties.realm_uuid;

        dce_acl_obj_add_group_entry(&local_acl, 
                                    default_server_acl_permissions,
                                    &admin_group_uuid,
                                    &st);
        if (st) {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
             "dce_acl_obj_add_group_entry failed (0x%x).  Can't create ACL"));
	    return st;
	};

    };
    
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                  "Encoding ACL..."));
    st = encode_acl(acl, &acl_data, &acl_length);

    if (st) {
	dce_svc_printf(PKS_S_ACL_ENCODE_FAIL6_MSG,
		       (unsigned long)st);
        if (local_acl_created) dce_acl_obj_free_entries(&local_acl, &est);
        return st;
    };

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                  "Writing ACL to database..."));

    database->txnBegin();
    st = database->write((char *)acl_data, acl_length,
                         (char *)(key->data), key->len);
    if (st == 0) database->txnEnd();
    else database->txnCancel();

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                  "dbWrite() returned 0x%x.  Freeing ACL...", st));

    free(acl_data);
    if (local_acl_created) dce_acl_obj_free_entries(&local_acl, &est);
    return st;
}


unsigned32 pkss_retrieve_server_acl(
                      sec_acl_t * acl) {
    unsigned32 st;
    unsigned char * acl_data;
    unsigned32 acl_length;
    const sec_pkss_dbkey_t * key;
    PkssDbKeyC dbKey(ROOT_OBJECT_NAME,
                     SERVER_ACL_TAG, 0, 0);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Retrieving server management ACL"));


    key = dbKey.p();
    
    st = database->read((char **)&acl_data, &acl_length,
                        (char *)(key->data), key->len);


    if (st) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug8,
                 "Retrieval of server management ACL failed.  Creating it..."));
	st = pkss_store_server_acl();
	if (st) return st;
	st = database->read((char **)&acl_data, &acl_length,
	                    (char *)(key->data), key->len);

	if (st) {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug8,
                 "Second attempt at retrieval failed (0x%x)", st));
	    return st;
	};
    };
    
    if (acl != NULL) st = decode_acl(acl_data, acl_length, acl);

    if (st) {
	dce_svc_printf(PKS_S_ACL_DECODE_FAIL5_MSG,
		       (unsigned long)st);
    };
    
    free(acl_data);

    return st;
}

unsigned32 pkss_inq_is_client_authorized(
                             handle_t h,
                             const uuid_t * acl_type,
                             boolean32 is_default,
                             const char * name,
                             sec_acl_permset_t desired_perms,
                             boolean32 * authorized) {

    sec_acl_permset_t permset;
    rpc_authz_cred_handle_t cred_h;
    unsigned32 st;
    unsigned32 st1;
    sec_acl_t acl;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Checking authorization to node %s", name));

#ifdef ENABLE_ALL

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Debug version: All permissions allowed"));
    *authorized = 1;
    return 0;

#endif

    *authorized = 0;

    if (uuid_equal(*acl_type, principal_acl_type)) {

        if (is_default) {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re default principal object"));

            st = pkss_retrieve_default_principal_acl(&acl, norm(name));
            if (st) {
                dce_svc_printf(PKS_S_ACL_DEFOBJ_RETRIEVE_FAIL4_MSG,
                               norm(name), (unsigned long)st);
            };
        } else {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re principal object"));

            st = pkss_retrieve_principal_acl(&acl, norm(name));
	    if (st) {
		dce_svc_printf(PKS_S_ACL_OBJ_RETRIEVE_FAIL1_MSG,
			       norm(name),
			       (unsigned long)(st));
	    };
        };
    } else if (uuid_equal(*acl_type, container_acl_type)) {
        if (is_default) {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re default container acl"));

            st = pkss_retrieve_default_container_acl(&acl, norm(name));
            if (st) {
                dce_svc_printf(PKS_S_ACL_DEFCON_RETRIEVE_FAIL3_MSG,
                               norm(name), (unsigned long)st);
	     };
        } else {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re container acl"));

            st = pkss_retrieve_container_acl(&acl, norm(name));
	    if (st) {
		dce_svc_printf(PKS_S_ACL_CON_RETRIEVE_FAIL2_MSG,
			       norm(name),
			       (unsigned long)(st));
	    };

        };
    } else if (uuid_equal(*acl_type, server_acl_type)) {
       DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                     "Inquiring re server acl"));
        st = pkss_retrieve_server_acl(&acl);
    } else {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                      "Inquiring re unknown ACL type"));
#ifdef DEBUG
	char * acl_string;
	uuid_to_string((uuid_p_t)acl_type, (unsigned char **)&acl_string, &st);
	if (st) {
	    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
			  "Can't tranlate ACL"));
	} else {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Manager UUID: %s", acl_string));
	    rpc_string_free((unsigned char **)&acl_string, &st);
	};
#endif

	return sec_acl_unknown_manager_type;
    };
    if (st) return st;


    dce_acl_inq_client_creds(h, &cred_h, &st);
    if (st) return st;

    dce_acl_inq_permset_for_creds(cred_h,
                                  &acl,
                                  NULL,
                                  NULL,
                                  sec_acl_posix_no_semantics,
                                  &permset,
                                  &st);
    dce_acl_obj_free_entries(&acl, &st1);
    if (st) return st;

    if ((desired_perms & permset) == desired_perms) *authorized = true;
    return 0;
    
}




unsigned32 pkss_inq_is_client_authorized(
                             handle_t h,
                             db_operation_t desired_op,
                             const char * name,
                             boolean32 * authorized) {
    unsigned32 st;
    char * container = NULL;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Checking authorization for op %d on node %s",
                  (int)desired_op, name));

    *authorized = 0;
    switch (desired_op) {
    case DBOP_CREATE_PRINCIPAL:
// Requirement is that the caller be granted read and insert-access to the
// container.  i.e that the principal's container object acl allows "i" access
// Also requier "read" access for the moment.
        container = container_name(name);	
        st = pkss_inq_is_client_authorized(h,
                                           &container_acl_type,
                                           0,
                                           container,
                                           sec_acl_perm_insert |
                                           sec_acl_perm_read,
                                           authorized);

	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		      "Authorization for CREATE_PRINCIPAL: %d, status 0x%x",
		      (int)(*authorized), st));

	free(container);
        return st;
    case DBOP_CREATE_CONTAINER:
// Requirement is that the caller be granted read and insert-access to the
// database.  i.e that the database container object acl allows "i" access
        container = container_name(name);	
        st = pkss_inq_is_client_authorized(h,
                                           &container_acl_type,
                                           0,
                                           container,
                                           sec_acl_perm_insert |
                                           sec_acl_perm_read,
                                           authorized);
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		      "Authorization for CREATE_CONTAINER: %d, status 0x%x",
		      (int)(*authorized), st));
	free(container);
        return st;
    case DBOP_ALTER_PRINCIPAL:
// Requirement is that the user have write access to the
// principal record.
        st = pkss_inq_is_client_authorized(h,
                                           &principal_acl_type,
                                           0,
                                           name,
                                           sec_acl_perm_write,
                                           authorized);
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		      "Authorization for ALTER_PRINCIPAL: %d, status: 0x%x",
		      (int)(*authorized), st));
	return st;
    case DBOP_DELETE_PRINCIPAL:
// Requirement is that the user have delete access to the
// principal record.
        st = pkss_inq_is_client_authorized(h,
                                           &principal_acl_type,
                                           0,
                                           name,
                                           sec_acl_perm_delete,
                                           authorized);
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		      "Authorization for DELETE_PRINCIPAL: %d, status: 0x%x",
		      (int)(*authorized), st));
	return st;
    case DBOP_SHUTDOWN_SERVER:
// Requirement is that the user have write access to the
// server object.
        st = pkss_inq_is_client_authorized(h,
                                           &server_acl_type,
                                           0,
                                           ROOT_OBJECT_NAME,
                                           sec_acl_perm_write,
                                           authorized);
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		      "Authorization for SHUTDOWN_SERVER: %d, status: 0x%x",
		      (int)(*authorized), st));
	return st;
    case DBOP_READ_STATISTICS:
// Requirement is that the user have read access to the
// server object.
        st = pkss_inq_is_client_authorized(h,
                                           &server_acl_type,
                                           0,
                                           ROOT_OBJECT_NAME,
                                           sec_acl_perm_read,
                                           authorized);
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		      "Authorization for READ_STATISTICS: %d, status: 0x%x",
		      (int)(*authorized), st));
	return st;
    case DBOP_ALTER_SERVER_ACL:
// Requirement is that the user have control access to the
// server object.
        st = pkss_inq_is_client_authorized(h,
                                           &server_acl_type,
                                           0,
                                           ROOT_OBJECT_NAME,
                                           sec_acl_perm_control,
                                           authorized);
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		      "Authorization for ALTER_SERVER_ACL: %d, status: 0x%x",
		      (int)(*authorized), st));
	return st;
    case DBOP_ALTER_PRINCIPAL_ACL:
// Requirement is that the user have control access to the
// principal object.
        st = pkss_inq_is_client_authorized(h,
                                           &principal_acl_type,
                                           0,
                                           name,
                                           sec_acl_perm_control,
                                           authorized);
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		      "Authorization for ALTER_PRINCIPAL_ACL: %d, status: 0x%x",
		      (int)(*authorized), st));
	return st;
    case DBOP_ALTER_CONTAINER_ACL:
// Requirement is that the user have control access to the
// principal object.
        st = pkss_inq_is_client_authorized(h,
                                           &container_acl_type,
                                           0,
                                           name,
                                           sec_acl_perm_control,
                                           authorized);
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		      "Authorization for ALTER_CONTAINER_ACL: %d, status: 0x%x",
		      (int)(*authorized), st));
	return st;
    case DBOP_ALTER_DEFAULT_PRINCIPAL_ACL:
// Requirement is that the user have control access to the
// default principal object.
        st = pkss_inq_is_client_authorized(h,
                                           &principal_acl_type,
                                           1,
                                           name,
                                           sec_acl_perm_control,
                                           authorized);
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
	      "Authorization for ALTER_DEFAULT_PRINCIPAL_ACL: %d, status: 0x%x",
		      (int)(*authorized), st));
	return st;
    case DBOP_ALTER_DEFAULT_CONTAINER_ACL:
// Requirement is that the user have control access to the
// default container object.
        st = pkss_inq_is_client_authorized(h,
                                           &container_acl_type,
                                           1,
                                           name,
                                           sec_acl_perm_control,
                                           authorized);
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
	      "Authorization for ALTER_DEFAULT_CONTAINER_ACL: %d, status: 0x%X",
		      (int)(*authorized), st));
        return st;
    default:
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug1,
		      "Invalid authorization operation"));
        return pks_s_invalid_op;
    }

}

unsigned32 pkss__add_acl_int(const char * name,
			     sec_acl_t * object_acl,
			     sec_acl_t * container_acl = NULL) {

// This routine is passed the name of the container within which we want to
// create an object.  It creates any ACLs that are needed and returns the
// default creation ACLs (object creation ACL is always returned, and
// container creation ACL is returned if container_acl is non-NULL.

    unsigned32 st;
    unsigned32 est;
    unsigned32 ost;
    unsigned32 cst;
    char * container;

    sec_acl_t local_object_acl;    
    sec_acl_t local_container_acl;    


    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		  "Recursively creating ACLs for node %s", name));

    ost = 0; cst = 0;    
    if (object_acl != NULL) {
	ost = pkss_retrieve_default_principal_acl(object_acl, name);
	if (ost == 0) {
	    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
			  "Object ACL already exists"));
	};
	if ((ost != 0) && (ost != pks_s_no_acl)) {
            if (ost) {
                dce_svc_printf(PKS_S_ACL_DEFOBJ_RETRIEVE_FAIL5_MSG,
                               name, (unsigned long)ost);
            };
	    return ost;
	};
    };
    if (container_acl != NULL) {
	cst = pkss_retrieve_default_container_acl(container_acl, name);
        if (cst == 0) {
	    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
			  "Container ACL already exists"));
	};
	if ((cst != 0) && (cst != pks_s_no_acl)) {
            if (cst) {
                dce_svc_printf(PKS_S_ACL_DEFCON_RETRIEVE_FAIL6_MSG,
                               name, (unsigned long)cst);
            };
	    return cst;
        };
    };

// Here if both lookups either succeeded or returned pks_s_no_acl.

    if ((cst == 0) && (ost == 0)) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
              "Both container and object ACLs already exist - Nothing to do"));
	return 0;
    };

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		  "Creating new ACL(s)"));

// Here if either lookup returned pks_s_no_acl.
// No ACLs found: If we're at the root, this database in uninitialized, so we
// have to create the root ACLs; if we're not at the root, recurse upwards.
    if (strcmp(norm(name), root_name) == 0) {
// We're at the root, and we didn't find any default ACLs.  Initialize the
// root.
        pkss_retrieve_server_acl();
        if (cst) {
	    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
			  "Creating default DB container ACL"));
	    pkss_store_default_container_acl(root_name);
	    pkss_store_container_acl(root_name);
	    st = pkss_retrieve_default_container_acl(container_acl);
	    if (st) {
		dce_svc_printf(PKS_S_ACL_DEFCON_RETRIEVE_FAIL4_MSG,
			       root_name, (unsigned long)st);
		return st;
	    };
	};
        if (ost) {
	    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
			  "Creating default DB object ACL"));
	    pkss_store_default_principal_acl(root_name);
	    pkss_store_principal_acl(root_name);
	    st = pkss_retrieve_default_principal_acl(object_acl);
	    if (st) {
                dce_svc_printf(PKS_S_ACL_DEFOBJ_RETRIEVE_FAIL6_MSG,
                               "<DB-ROOT>", (unsigned long)st);
	         return st;
            };
	};
	return 0;
    } else {
// we're not at the root, and we need to fill in either the object ACL,
// the container ACL, or both.
	container = container_name(name);
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
		      "Creating ACLs for parent node %s", container));

	st = pkss__add_acl_int(container,
                               &local_object_acl,
			       &local_container_acl);
// When we return, all higher-level ACLs have been created, and
// local_object_acl and local_container_acl contain the default creation ACLs
// for <container>, which is the same as the actual creation ACL for
// <name>.  Add ACLs and default ACLs to <name>, then return.

	free(container);

	if (st) return st;

// Add container ACL:
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                      "Adding container ACL to node %s", name));
        st = pkss_store_container_acl(name,
                                      &local_container_acl);
	if (st) {
            dce_acl_obj_free_entries(&local_container_acl, &est);
            dce_acl_obj_free_entries(&local_object_acl, &est);
	    return st;
	};

// Add default container ACL:
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                      "Adding default container ACL to node %s", name));
        st = pkss_store_default_container_acl(name,
                                              &local_container_acl);
	if (st) {
            dce_acl_obj_free_entries(&local_container_acl, &est);
            dce_acl_obj_free_entries(&local_object_acl, &est);
	    return st;
	};


// Add principal ACL:
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                      "Adding object ACL to node %s", name));
        st = pkss_store_principal_acl(name,
                                      &local_object_acl);
	if (st) {
            dce_acl_obj_free_entries(&local_container_acl, &est);
            dce_acl_obj_free_entries(&local_object_acl, &est);
	    return st;
	};
// Add default principal ACL:
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                      "Adding default object ACL to node %s", name));
        st = pkss_store_principal_acl(name,
                                      &local_object_acl);
	if (st) {
            dce_acl_obj_free_entries(&local_container_acl, &est);
            dce_acl_obj_free_entries(&local_object_acl, &est);
	    return st;
	};

// Copy ACLs to parameters...
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                      "Copying ACLs..."));
	if (container_acl) *container_acl = local_container_acl;
        else dce_acl_obj_free_entries(&local_container_acl, &est);
	if (object_acl) *object_acl = local_object_acl;
	else dce_acl_obj_free_entries(&local_object_acl, &st);

	return st;
    };
}

unsigned32 pkss_add_acl(const char * name) {
// Algorithm is simple. Starting at the specified name, call add_acl_int
// recursively on the leading portion of the name until we reach either the
// root or an entry that has a default container ACL.  Then back out, creating
// ACLs as needed.
    unsigned32 st;
    unsigned32 est;
    sec_acl_t object_acl;
    sec_acl_t container_acl;
    char * container;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Creating ACLs for new node %s", name));
    
    container = container_name(name);

    st = pkss__add_acl_int(container,
                           &object_acl,
			   &container_acl);

    free(container);

    if (st) return st;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                  "Storing object ACL for node %s", name));

    st = pkss_store_principal_acl(name,
                                  &object_acl);
    if (st == 0) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                      "Storing default object ACL for node %s", name));
	st = pkss_store_default_principal_acl(name,
                                              &object_acl);
    };
    
    if (st == 0) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                      "Storing cotnainer ACL for node %s", name));
	st = pkss_store_container_acl(name,
                                      &container_acl);
    };
    
    if (st == 0) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug3,
                      "Storing default container ACL for node %s", name));
	st = pkss_store_default_container_acl(name,
                                              &container_acl);
    };
    
    dce_acl_obj_free_entries(&object_acl, &est);
    dce_acl_obj_free_entries(&container_acl, &est);
    return st;
}


void rdacl_lookup (handle_t h,
                   sec_acl_component_name_t component_name,
                   uuid_t * manager_type,
                   sec_acl_type_t sec_acl_type,
                   sec_acl_result_t * result) {
    sec_acl_t acl;
    error_status_t st;
    
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                  "rdacl_lookup for object %s", component_name));

    if (uuid_equal(*manager_type, principal_acl_type)) {
// We need a principal ACL.  Entries have two principal ACLs -
// Object and default object
       DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                     "Lookup is for principal"));
        result->st = sec_acl_no_acl_found;
        if (sec_acl_type == sec_acl_type_object) {
           DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                         "Lookup is for object"));
	    result->st = pkss_retrieve_principal_acl(&acl,
	                                          (const char *)component_name);

	    if (result->st) {
		dce_svc_printf(PKS_S_ACL_OBJ_RETRIEVE_FAIL2_MSG,
			       (char *)component_name,
			       (unsigned long)(result->st));
	    };

        } else if (sec_acl_type == sec_acl_type_default_object) {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Lookup is for default object"));
            result->st = pkss_retrieve_default_principal_acl(
	                                     &acl,
                                             (const char *)component_name);

	    if (result->st) {
		dce_svc_printf(PKS_S_ACL_DEFOBJ_RETRIEVE_FAIL7_MSG,
			       (char *)component_name,
			       (unsigned long)(result->st));
            };

        };

	if (result->st) return;

        result->tagged_union.sec_acl_list =
                    (sec_acl_list_t *)rpc_ss_allocate(sizeof(sec_acl_list_t));
        result->tagged_union.sec_acl_list->num_acls = 1;
        result->tagged_union.sec_acl_list->sec_acls[0] =
            (sec_acl_t *)rpc_ss_allocate(sizeof(sec_acl_t));
        memcpy(result->tagged_union.sec_acl_list->sec_acls[0],
               &acl,
               sizeof(acl));
        return;
    } else if (uuid_equal(*manager_type, container_acl_type)) {
// We need a container ACL.  Entries have two container ACLs -
// container and default container 
       DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                     "Lookup is for container"));
        result->st = sec_acl_no_acl_found;
        if (sec_acl_type == sec_acl_type_object) {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Lookup is for container object"));
	    result->st = pkss_retrieve_container_acl(&acl,
	                                          (const char *)component_name);

	    if (result->st) {
		dce_svc_printf(PKS_S_ACL_CON_RETRIEVE_FAIL1_MSG,
			       (char *)component_name,
			       (unsigned long)(result->st));
            };
        } else if (sec_acl_type == sec_acl_type_default_container) {
           DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                         "Lookup is for default container"));
            result->st = pkss_retrieve_default_container_acl(
	                                         &acl,
                                                 (const char *)component_name);
	    if (result->st) {
		dce_svc_printf(PKS_S_ACL_DEFCON_RETRIEVE_FAIL7_MSG,
			       (char *)component_name,
			       (unsigned long)(result->st));
            };
        };
        if (result->st) return;
        result->tagged_union.sec_acl_list =
                    (sec_acl_list_t *)rpc_ss_allocate(sizeof(sec_acl_list_t));
        result->tagged_union.sec_acl_list->num_acls = 1;
        result->tagged_union.sec_acl_list->sec_acls[0] =
            (sec_acl_t *)rpc_ss_allocate(sizeof(sec_acl_t));
        memcpy(result->tagged_union.sec_acl_list->sec_acls[0],
               &acl,
               sizeof(acl));
        return;
    } else if (uuid_equal(*manager_type, server_acl_type)) {
       DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                     "Lookup is for server"));
        result->st = pkss_retrieve_server_acl(&acl);
        if (result->st) return;
        result->tagged_union.sec_acl_list =
                    (sec_acl_list_t *)rpc_ss_allocate(sizeof(sec_acl_list_t));
        result->tagged_union.sec_acl_list->num_acls = 1;
        result->tagged_union.sec_acl_list->sec_acls[0] =
            (sec_acl_t *)rpc_ss_allocate(sizeof(sec_acl_t));
        memcpy(result->tagged_union.sec_acl_list->sec_acls[0],
               &acl,
               sizeof(acl));
	return;
    } else {

#ifdef DEBUG
	char * acl_string;
	uuid_to_string(manager_type, (unsigned char **)&acl_string, &st);
	if (st) {
	    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
			  "Can't tranlate ACL"));
	} else {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Manager UUID: %s", acl_string));
	    rpc_string_free((unsigned char **)&acl_string, &st);
	};
#endif


	result->st = sec_acl_unknown_manager_type;
        return;
    };
}

void rdacl_replace(handle_t h,
                   sec_acl_component_name_t component_name,
                   uuid_t *manager_type,
                   sec_acl_type_t sec_acl_type,
                   sec_acl_list_t *sec_acl_list,
                   error_status_t *st) {

    boolean32 authorized;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                  "rdacl_replace for object %s", component_name));

    if (uuid_equal(*manager_type, principal_acl_type)) {
        *st = sec_acl_no_acl_found;
        if (sec_acl_type == sec_acl_type_object) {

            *st = pkss_inq_is_client_authorized(h,
                                                DBOP_ALTER_PRINCIPAL_ACL,
                                                (const char *)component_name,
                                                &authorized);
            if (*st) return;
            if (authorized) {
                *st = pkss_store_principal_acl((const char *)component_name,
                                               sec_acl_list->sec_acls[0]);

            } else {
                *st = sec_acl_not_authorized;
            };
        } else if (sec_acl_type == sec_acl_type_default_object) {

            *st = pkss_inq_is_client_authorized(
                                       h,
                                       DBOP_ALTER_DEFAULT_PRINCIPAL_ACL,
                                       (const char *)component_name,
                                       &authorized);
            if (*st) return;
            if (authorized) {
                *st = pkss_store_default_principal_acl(
                                          (const char *)component_name,
                                          sec_acl_list->sec_acls[0]);
            } else {
                *st = sec_acl_not_authorized;
            };
        };
    } else if (uuid_equal(*manager_type, server_acl_type)) {
        *st = pkss_inq_is_client_authorized(h,
                                            DBOP_ALTER_SERVER_ACL,
                                            (const char *)component_name,
                                            &authorized);
        if (*st) return;
        if (authorized) {
	    *st = pkss_store_principal_acl((const char *)component_name,
                                           sec_acl_list->sec_acls[0]);
        } else {
            *st = sec_acl_not_authorized;
        };
    } else {
        *st = sec_acl_invalid_manager_type;
    };
    return;
}

void rdacl_get_access(handle_t h,
                      sec_acl_component_name_t component_name,
                      uuid_t * manager_type,
                      sec_acl_permset_t * net_rights,
                      error_status_t * st) {


    rpc_authz_cred_handle_t cred_h;
    unsigned32 st1;
    sec_acl_t acl;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                  "rdacl_get_access for object %s", component_name));

    if (uuid_equal(*manager_type, principal_acl_type)) {
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug8,
		      "retrieving principal ACL"));
        *st = pkss_retrieve_principal_acl(&acl, (const char *)component_name);
        if (*st) {
            dce_svc_printf(PKS_S_ACL_OBJ_RETRIEVE_FAIL3_MSG,
                           (char *)component_name,
                           (unsigned long)(*st));
        };

    } else if (uuid_equal(*manager_type, container_acl_type)) {
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
		      "retrieving container ACL"));
        *st = pkss_retrieve_container_acl(&acl, (const char *)component_name);
        if (*st) {
            dce_svc_printf(PKS_S_ACL_OBJ_RETRIEVE_FAIL3_MSG,
                           (char *)component_name,
                           (unsigned long)(*st));
        };

    } else if (uuid_equal(*manager_type, server_acl_type)) {
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
		      "retrieving server ACL"));
        *st = pkss_retrieve_server_acl(&acl);
        if (*st) {
            dce_svc_printf(PKS_S_ACL_OBJ_RETRIEVE_FAIL3_MSG,
                           (char *)component_name,
                           (unsigned long)(*st));
        };
    } else {
	DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
		      "Attempt to retrieve unknown ACL type"));

	*st = sec_acl_unknown_manager_type;

#ifdef DEBUG
	char * acl_string;
	error_status_t loc_stat;
	uuid_to_string(manager_type, (unsigned char **)&acl_string, &loc_stat);
	if (loc_stat) {
	    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
			  "Can't tranlate ACL"));
	} else {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Manager UUID: %s", acl_string));
	    rpc_string_free((unsigned char **)&acl_string, &loc_stat);
	};
#endif

    };
    if (*st) return;


    dce_acl_inq_client_creds(h, &cred_h, st);
    if (*st) return ;

    dce_acl_inq_permset_for_creds(cred_h,
                                  &acl,
                                  NULL,
                                  NULL,
                                  sec_acl_posix_no_semantics,
                                  net_rights,
                                  st);
    dce_acl_obj_free_entries(&acl, &st1);
    return ;
}

boolean32 rdacl_test_access(handle_t h,
                            sec_acl_component_name_t component_name,
                            uuid_t * manager_type,
                            sec_acl_permset_t desired_permset,
                            error_status_t * st) {

    boolean32 authorized;


    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                  "rdacl_test_access for object %s", component_name));

    *st = pkss_inq_is_client_authorized(h,
                                        manager_type,
                                        0,
                                        (const char *)component_name,
                                        desired_permset,
                                        &authorized);
    if (st) return 0;
    return authorized;
}

boolean32 rdacl_test_access_on_behalf(
                       handle_t h,
                       sec_acl_component_name_t component_name,
                       uuid_t * manager_type,
                       sec_id_pac_t * subject,
                       sec_acl_permset_t desired_permset,
                       error_status_t * st) {

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
           "Unimplemented rdacl_test_access_on_behalf for object %s", component_name));
    *st = sec_acl_not_implemented;
    return 0;
}

void rdacl_get_manager_types(handle_t h,
                             sec_acl_component_name_t component_name,
                             sec_acl_type_t sec_acl_type,
                             unsigned32 size_avail,
                             unsigned32 * size_used,
                             unsigned32 * num_types,
                             uuid_t manager_types[],
                             error_status_t * st) {

    const char * name = norm((const char *)component_name);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                  "rdacl_get_manager_type for object %s", component_name));

    if (sec_acl_type == sec_acl_type_default_object) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                      "Get mgr-types for default object"));
        *num_types = 1;
        if (size_avail < 1) {
            *size_used = 0;
        } else {
            *size_used = 1;
             manager_types[0] = principal_acl_type;
        }
    } else if (sec_acl_type == sec_acl_type_default_container) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                      "Get mgr-types for default container"));
        *num_types = 1;
        if (size_avail < 1) {
            *size_used = 0;
        } else {
            *size_used = 1;
            manager_types[0] = container_acl_type;
        }
    } else if (strcmp(name, root_name) == 0) {
        if (sec_acl_type == sec_acl_type_object) {
// Root object contains the server ACL
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Get mgr-types for root object"));
            *num_types = 1;
            if (size_avail < 1) {
                *size_used = 0;
            } else {
                *size_used = 1;
                 manager_types[0] = server_acl_type;
            };
        } else {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Get mgr-types for ??? root object"));
            *num_types = 0;
            *size_used = 0;
        };
    } else {
        if (sec_acl_type == sec_acl_type_object) {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Get mgr-types for object"));
            *num_types = 2;
            if (size_avail < 2) {
                *size_used = 0;
            } else {
                *size_used = 2;
                 manager_types[0] = principal_acl_type;
                 manager_types[1] = container_acl_type;
            }
        } else {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Get mgr-types for ??? object"));
            *num_types = 0;
            *size_used = 0;
        };
    }
    *st = rpc_s_ok;
}

void rdacl_get_mgr_types_semantics(
                         handle_t h,
                         sec_acl_component_name_t component_name,
                         sec_acl_type_t sec_acl_type,
                         unsigned32 size_avail,
                         unsigned32 * size_used,
                         unsigned32 * num_types,
                         uuid_t manager_types[],
                         sec_acl_posix_semantics_t posix_semantics[],
                         error_status_t *st) {

    const char * name = norm((const char *)component_name);

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                  "rdacl_get_mgr_types_semantics for object %s",
                  component_name));


    if (strcmp(name, root_name) == 0) {
        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                      "...for root"));
        if (sec_acl_type == sec_acl_type_default_object) {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re default object ACL for root"));
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                          "1 ACL type available: principal"));

            *num_types = 1;
            if (size_avail < 1) {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
                      "Not enough space to return ACL type (size_avail=%d)!",
                      size_avail));
                *size_used = 0;
            } else {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug8,
                              "Returning a single ACL type (principal)"));
                *size_used = 1;
                 manager_types[0] = principal_acl_type;
                 posix_semantics[0] = sec_acl_posix_no_semantics;
            }
        } else if (sec_acl_type == sec_acl_type_default_container) {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re default container ACL for root"));
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                          "1 ACL type available: container"));

            *num_types = 1;
            if (size_avail < 1) {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
                     "Not enough space to return ACL type (size_avail=%d)!",
		     size_avail));
                *size_used = 0;
            } else {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug8,
                              "Returning single ACL type (container)"));
                *size_used = 1;
                 manager_types[0] = container_acl_type;
                 posix_semantics[0] = sec_acl_posix_no_semantics;
            }
        } else if (sec_acl_type == sec_acl_type_object) {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re object ACL for root"));
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                          "3 ACL types available: Principal, container & server"));

            *num_types = 3;
            if (size_avail < 3) {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
                       "Not enough space to return ACL types (size_avail=%d)!",
		       size_avail));
		if (size_avail == 2) {
		    *size_used = 2;
		     manager_types[0] = principal_acl_type;
		     posix_semantics[0] = sec_acl_posix_no_semantics;
		     manager_types[1] = container_acl_type;
		     posix_semantics[1] = sec_acl_posix_no_semantics;
		} else if (size_avail == 1) {
		    *size_used = 1;
		     manager_types[0] = principal_acl_type;
		     posix_semantics[0] = sec_acl_posix_no_semantics;
		} else *size_used = 0;
            } else {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug8,
                              "Returning 2 ACL types (principal & server)"));
                *size_used = 3;
                 manager_types[0] = principal_acl_type;
                 posix_semantics[0] = sec_acl_posix_no_semantics;
                 manager_types[1] = container_acl_type;
                 posix_semantics[1] = sec_acl_posix_no_semantics;
                 manager_types[2] = server_acl_type;
                 posix_semantics[2] = sec_acl_posix_no_semantics;
            };
        } else {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re unknown ACL class for root"));
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                          "Returning no ACL managers"));

            *num_types = 0;
            *size_used = 0;
        };
    } else {
        if (sec_acl_type == sec_acl_type_object) {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re non-root object"));
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                          "2 ACL types available: principal and container"));

            *num_types = 2;
            if (size_avail < 2) {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
               "Not enough space to return ACL types (size_avail=%d)!",
	       size_avail));
		if (size_avail == 1) {
                    *size_used = 1;
                     manager_types[0] = principal_acl_type;
                     posix_semantics[0] = sec_acl_posix_no_semantics;
                } else *size_used = 0;
            } else {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug8,
                              "Returning 2 ACL types (principal & container)"));
                *size_used = 2;
                 manager_types[0] = principal_acl_type;
                 posix_semantics[0] = sec_acl_posix_no_semantics;
                 manager_types[1] = container_acl_type;
                 posix_semantics[1] = sec_acl_posix_no_semantics;
            }
        } else if (sec_acl_type == sec_acl_type_default_object) {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re default object ACL for non-root"));
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                          "1 ACL type available: principal"));

            *num_types = 1;
            if (size_avail < 1) {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
                      "Not enough space to return ACL type (size_avail=%d)!",
		      size_avail));
                *size_used = 0;
            } else {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug8,
                              "Returning ACL type (principal)"));
                *size_used = 1;
                 manager_types[0] = principal_acl_type;
                 posix_semantics[0] = sec_acl_posix_no_semantics;
            }

        } else if (sec_acl_type == sec_acl_type_default_container) {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re default container ACL for non-root"));
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                          "1 ACL type available: container"));

            *num_types = 1;
            if (size_avail < 1) {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
                    "Not enough space to return ACL type (size_avail=%d)!",
		    size_avail));
                *size_used = 0;
            } else {
                DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
                              "Returning ACL type (container)"));
                *size_used = 1;
                 manager_types[0] = container_acl_type;
                 posix_semantics[0] = sec_acl_posix_no_semantics;
            }
        } else {

            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Inquiring re unknown ACL class for non-root"));
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                          "Returning no ACL managers"));

            *num_types = 0;
            *size_used = 0;
	};
    };

#ifdef DEBUG
    int ii;
    char * acl_string;
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
                  "Returning %d ACL managers, size = %d",
		  *num_types,
		  *size_used));
    for (ii=0;ii<*size_used;ii++) {
        uuid_to_string(&(manager_types[ii]),
	               (unsigned char **)&acl_string,
		       st);
        if (st) {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
                          "Can't tranlate ACL"));
        } else {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Manager UUID: %s", acl_string));
            rpc_string_free((unsigned char **)&acl_string, st);
        };
    };
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
                  "Returning from RPC"));
#endif


    *st = rpc_s_ok;
}

void rdacl_get_printstring(handle_t h,
                           uuid_t * manager_type,
                           unsigned32 size_avail,
                           uuid_t * manager_type_chain,
                           sec_acl_printstring_t * manager_info,
                           boolean32 * tokenize,
                           unsigned32 * total_num_printstrings,
                           unsigned32 * size_used,
                           sec_acl_printstring_t printstrings[],
                           error_status_t *st) {

    sec_acl_printstring_t * printstring;
    int i;

    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                  "rdacl_get_printstring called"));

    if (uuid_equal(*manager_type, principal_acl_type)) {

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                      "Inquiring re principal ACL type"));

        printstring = pkss_principal_printstrings;
        *total_num_printstrings = 4;
        *manager_info = pkss_principal_info;
    } else if (uuid_equal(*manager_type, container_acl_type)) {

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                      "Inquiring re container ACL type"));

        printstring = pkss_container_printstrings;
        *total_num_printstrings = 4;
        *manager_info = pkss_container_info;
    } else if (uuid_equal(*manager_type, server_acl_type)) {

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug9,
                      "Inquiring re server ACL type"));

        printstring = pkss_server_printstrings;
        *total_num_printstrings = 3;
        *manager_info = pkss_server_info;
    } else {

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                      "Inquiring re unknown ACL type"));

#ifdef DEBUG
	char * acl_string;
	error_status_t loc_stat;
	uuid_to_string(manager_type, (unsigned char **)&acl_string, &loc_stat);
	if (loc_stat) {
	    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug2,
			  "Can't tranlate ACL"));
	} else {
            DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                          "Manager UUID: %s", acl_string));
	    rpc_string_free((unsigned char **)&acl_string, &loc_stat);
	};
#endif

        *st = sec_acl_unknown_manager_type;
        return;
    }

    if (size_avail < *total_num_printstrings) {
        *size_used = size_avail;
    } else {
        *size_used = *total_num_printstrings;
    };

    uuid_create_nil(manager_type_chain, st);

    *tokenize = false;

    for (i=0; i<*size_used; i++) {
        printstrings[i] = printstring[i];
    };
    *st = rpc_s_ok;
}

void rdacl_get_referral(handle_t h,
                        sec_acl_component_name_t component_name,
                        uuid_t * manager_type,
                        sec_acl_type_t sec_acl_type,
                        sec_acl_tower_set_t * towers,
                        error_status_t *st) {
    DCE_SVC_DEBUG((pks_svc_handle, pks_s_acl, svc_c_debug7,
                  "Unimplemented rdacl_get_referral for object %s",
                  component_name));

    *st = sec_acl_not_implemented;
}

