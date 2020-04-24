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
 * $Log: pkss_acl_mgr.h,v $
 * Revision 1.1.2.1  1996/10/03  20:29:38  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:43:39  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:27 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:05 UTC  arvind  /main/arvind_pkss/1]
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

#ifndef PKSS_ACL_MGR_H_
#define PKSS_ACL_MGR_H_

extern "C" {
#include <dce/dcepksmsg.h>
#include <dce/dce.h>
#include <dce/rdaclif.h>
}
#include "gdb44_methods.h"

// The following enumberated type is used with the 
// pkss_inq_is_client_authorized call to specify the high-level operation
// that the client is requesting

typedef enum {
    DBOP_CREATE_PRINCIPAL,  // Create a principal within the database
    DBOP_CREATE_CONTAINER,  // Create a container within the database
    DBOP_ALTER_PRINCIPAL,   // Change a principal's key
    DBOP_DELETE_PRINCIPAL,  // Delete a principal from the database
    DBOP_SHUTDOWN_SERVER,   // Shutdown the server
    DBOP_READ_STATISTICS,   // Retrieve server statistics (not implemented)
    DBOP_ALTER_SERVER_ACL,  // Change the ACL on the server
    DBOP_ALTER_PRINCIPAL_ACL,    // Change the ACL on a principal
    DBOP_ALTER_CONTAINER_ACL,    // Change the ACL on a container
    DBOP_ALTER_DEFAULT_PRINCIPAL_ACL,    // Change the default principal ACL
    DBOP_ALTER_DEFAULT_CONTAINER_ACL     // Change the default container ACL
} db_operation_t;


const uuid_t principal_acl_type = { /* 6DFBDE4A-CACA-11CF-88E2-08002B187D1A */
        0x6dfbde4a,
        0xcaca,
        0x11cf,
        0x88,
        0xe2,
        {0x08, 0x00, 0x2b, 0x18, 0x7d, 0x1a}
      };

const uuid_t container_acl_type = { /* 53174112-CDBF-11CF-A2A4-08002B187D1A */
        0x53174112,
        0xcdbf,
        0x11cf,
        0xa2,
        0xa4,
        {0x08, 0x00, 0x2b, 0x18, 0x7d, 0x1a}
      };

const uuid_t server_acl_type = { /* A92DE012-CACA-11CF-9D02-08002B187D1A */
        0xa92de012,
        0xcaca,
        0x11cf,
        0x9d,
        0x02,
        {0x08, 0x00, 0x2b, 0x18, 0x7d, 0x1a}
      };

int uuid_equal(const uuid_t & o1, const uuid_t & o2);

//
// pkss_initialize_acl_manager
//
// Initialize the ACL manager.  ACL manager initialization performs 
// several operations:
// i) Store the pointer to the database.  The database must have been opened
//    before calling pkss_initialize_acl_manager().  
// ii) Retrieve information about the current cell and store it for
//     future use.  The server must have established a default
//     DCE login context, suitable for performing registry operations, 
//     before calling pkss_initialize_acl_manager().
//
// The function returns an object UUID which must be registered with
// the server manager code (see Wei Hu's book, P219), and an interface
// spec that must be registered with the endpoint mapper.

unsigned32 pkss_initialize_acl_manager(Gdb44C * db_ptr,
                                       uuid_t * uuid,
                                       rpc_if_handle_t * ifspec);


//
// pkss_inq_is_client_authorized
//
// Determines whether a client is allowed to perform an operation
// based on the provided name, and on the desired operation.  This routine
// implements the full access-control policy for the PKSS database.  This
// is probably the only authorization function that the main
// PKSS code will need to invoke (once the ACL manager has been 
// initialized).
//
unsigned32 pkss_inq_is_client_authorized(
			     handle_t h,
                             db_operation_t desired_op,
                             const char * name,
                             boolean32 * authorized);

//
// pkss_add_acl
//
// Create an object ACL, set to the default value, for the specified 
// principal, as well as any other ACLs (e.g. container and default ACLs) 
// that may be necessary.  Use of this function allows the ACL store within 
// the database to self-initialize, since required ACLs are created on an 
// as-needed basis.
unsigned32 pkss_add_acl(const char * name);


// The rest of this stuff can probably be ignored for most uses - the three
// routines above abstract almost all of the required functionality of
// an ACL manager, and routines below this point are only if you need to
// things manually.

//
// pkss_store_principal_acl
//
// Store the specified ACL under the principal name.  If an ACL already
// exists, overwrite it.  If the acl is NULL, store a copy of the default
// ACL under the principal name.  The database ACL is stored under the 
// empty principal name.
//
unsigned32 pkss_store_principal_acl(
                      const char * principal_name,
                      const sec_acl_t * acl = NULL);

//
// pkss_store_container_acl
//
// Store the specified ACL under the container name.  If an ACL already
// exists, overwrite it.  If the acl is NULL, store a copy of the default
// container ACL under the principal name.  The database ACL is stored under 
// the empty container name.
//
unsigned32 pkss_store_container_acl(
                      const char * principal_name,
                      const sec_acl_t * acl = NULL);


//
// pkss_retrieve_principal_acl
//
// Retrieve the specified principal's ACL.  If the ACL doesn't exist,
// return pks_s_no_acl.  When done, storage associated with the retrieved 
// ACL should be freed with dce_acl_obj_free_entries().  The default principal
// ACL is stored under the null name.
//
unsigned32 pkss_retrieve_principal_acl(
                      sec_acl_t * acl,
                      const char * principal_name = NULL);

//
// pkss_retrieve_container_acl
//
// Retrieve the specified container ACL.  If the ACL doesn't exist,
// return pks_s_no_acl.  When done, storage associated with the retrieved 
// ACL should be freed with dce_acl_obj_free_entries().  The global default
// container ACL is stored under the null name.
//
unsigned32 pkss_retrieve_container_acl(
                      sec_acl_t * acl,
                      const char * principal_name = NULL);


//
// pkss_store_default_principal_acl
//
// Store the default ACL.  If a default ACL already exists, overwrite it.
// If the parameter is NULL, the initial default ACL will be stored (an
// ACL granting members of the administration group full rights to 
// database entries.
//
unsigned32 pkss_store_default_principal_acl(
                      const char * name = NULL,
                      const sec_acl_t * acl = NULL);

//
// pkss_retrieve_default_principal_acl
//
// Retrieve the default principal ACL.  If the ACL doesn't exist,
// return pks_s_no_acl.  When done, storage associated with the 
// retrieved ACL should be freed with dce_acl_obj_free_entries()
//
unsigned32 pkss_retrieve_default_principal_acl(sec_acl_t * acl,
                                               const char * name = NULL);

//
// pkss_store_default_container_acl
//
// Store the default container ACL.  If a default ACL already exists, 
// overwrite it.  If the parameter is NULL, the initial default ACL will be 
// stored (an ACL granting members of the administration group full rights to 
// database entries).
//
unsigned32 pkss_store_default_container_acl(
                      const char * name = NULL,
                      const sec_acl_t * acl = NULL);

//
// pkss_retrieve_default_container_acl
//
// Retrieve the default container ACL.  If the ACL doesn't exist,
// return pks_s_no_acl.  When done, storage associated with the 
// retrieved ACL should be freed with dce_acl_obj_free_entries()
//
unsigned32 pkss_retrieve_default_container_acl(sec_acl_t * acl,
                                               const char * name = NULL);


//
// pkss_store_server_acl
//
// Store the database management ACL
// If the acl parameter is NULL, the initial default database ACL will be
// stored, grating full access to members of the management group.
//
unsigned32 pkss_store_server_acl(const sec_acl_t * acl = NULL);



//
// pkss_retrieve_server_acl
//
// Retrieve the database management ACL.  When done, storage associated
// with the retrieved ACL should be freed with dce_acl_obj_free_entries()
// If the ACL isn't found, it will be created.
//
unsigned32 pkss_retrieve_server_acl(sec_acl_t * acl = NULL);



//
// pkss_inq_is_client_authorized
//
// Determines whether a client is allowed to perform an operation
// based on the provided name, the acl type, and on the permissions 
// required for the operation.
//
unsigned32 pkss_inq_is_client_authorized(
			     handle_t h,
                             const uuid_t * acl_type,
                             boolean32 is_default,
                             const char * name,
                             sec_acl_permset_t desired_perms,
                             boolean32 * authorized);



#endif
