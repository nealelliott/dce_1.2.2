/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsuuid.c,v $
 * Revision 1.1.337.2  1996/02/18  00:05:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:17  marty]
 *
 * Revision 1.1.337.1  1995/12/08  00:21:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:17  root]
 * 
 * Revision 1.1.335.1  1994/01/21  22:38:53  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:31  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:54:21  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:11:19  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:14:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:43:03  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:12:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**    nsuuid.c
**
**  FACILITY:
**
**    Remote procedure call (RPC) name service independent interface (NS)
**  
**  ABSTRACT:
**
**  Set of routines supporting operations on object UUIDs.
**
**  
**
**/

#include <commonp.h>
#include <com.h> 
#include <ns.h>              /* Inter-component Private NS defs           */
#include <nsp.h>             /* Private defs for Naming Service component */
#include <nsutil.h>          /* Internal name service specific routines   */
#include <nsuuid.h>          /* Internal nsuuid defs                      */
#include <nsentry.h>         /* Externals for NS Entry sub-component      */
#include <nsattr.h>          /* Private routines for NS attributes        */

/*
**++
**  ROUTINE NAME:           rpc__nsuuid_mbr_add
**
**  SCOPE:                  PRIVATE - declared in nsuuid.h
**
**  DESCRIPTION:        
**
**  Adds an object UUID to a entry's UUID set in the designated name service.
**  If necessary, creates the attribute. 
**
**  INPUTS:             
**
**      nsentry_name        Name service representation of the entry name
**      nsobj_uuid          Name service representation of an object UUID
**                          which is to be stored in the object UUID set
**                          attribute for the entry name specified by 
**                          nsentry.
**
**  INPUT/OUTPUTS:          none
**
**      
**
**  OUTPUTS:            
**
**      status              The return status value in *status indicates
**                          whether the routine completed successfully and, if
**                          not, why.
**                          Status values include:
**                              rpc_s_ok    - successful completion
**                              return value from rpc__nsattr_add_value
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         none
**
**  SIDE EFFECTS:           none
**
**--
*/

PRIVATE void rpc__nsuuid_mbr_add 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t             nsentry_name,
    rpc_nsuuid_p_t              nsobj_uuid,
    unsigned32                  *status
)
#else
(nsentry_name, nsobj_uuid, status)
rpc_nsentry_p_t             nsentry_name;
rpc_nsuuid_p_t              nsobj_uuid;
unsigned32                  *status;
#endif
{
    unsigned32              temp_status;
    
    CODING_ERROR (status);

    /*
     * Check the class version of the name 
     * service entry.
     */
    rpc__ns_check_rpc_class_version (nsentry_name, RPC_C_NS_BYPASS_CACHE, status);

    /*
     * Add the uuid field of nsobj_uuid to the object UUID attribute
     * if the class_version is ok or
     * the name service entry has not been labeled an RPC entry.
     */
    if ((*status == rpc_s_ok) || 
        (*status == rpc_s_not_rpc_entry))
    {
        rpc__nsattr_add_value ( 
            nsentry_name, 
            RPC_C_ATTR_OBJECT_UUIDS,
            (unsigned_char_p_t) &(nsobj_uuid->uuid),
            (sizeof (uuid_t)),
            &temp_status);
        /*
         * If the UUID attribute value  has been added successfully
         * and this is not an RPC entry, label the entry as such.
         */
        if ((temp_status == rpc_s_ok) &&
            (*status == rpc_s_not_rpc_entry))
        {
            rpc__nsentry_make_it_rpc (nsentry_name, &temp_status);
        }        
        
        /*
         * Return the results of either rpc__nsattr_add_value or
         * rpc__nsentry_make_it_rpc,depending on which was called
         * last.
         */
        *status = temp_status;
        
    }

    return;
}

/*
**++
**  ROUTINE NAME:           rpc__nsuuid_delete 
**
**  SCOPE:                  PRIVATE - declared in nsuuid.h
**
**  DESCRIPTION:        
**
**  This routine removes the UUID set attribute from an entry in the
**  designated name service.
**
**  INPUTS:             
**
**      nsentry_name        Name service representation of the entry name
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:            
**
**      status              The return status value in *status indicates
**                          whether the routine completed successfully and, if
**                          not, why.
**                          Possible return values:
**                              rpc_s_ok
**                              rpc_s_not_found
**                              return value from rpc__nsattr_delete
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         none
**
**  SIDE EFFECTS:           none
**
**--
*/

PRIVATE void rpc__nsuuid_delete 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t             nsentry_name,
    unsigned32                  *status
)
#else
(nsentry_name, status)
rpc_nsentry_p_t             nsentry_name;
unsigned32                  *status;
#endif
{
    CODING_ERROR (status);

    /* 
     * Determine if nsentry contains the object UUID attribute.
     */
    if ((rpc__nsattr_exists (nsentry_name, RPC_C_ATTR_OBJECT_UUIDS, 
                             RPC_C_NS_BYPASS_CACHE, status)) ==
        false)
    {
        return;
    }

    /* 
     * If the attribute exists, delete it.
     */
    rpc__nsattr_delete (nsentry_name, RPC_C_ATTR_OBJECT_UUIDS, status);

    return;
}

/*
**++
**  ROUTINE NAME:           rpc__nsuuid_free
**
**  SCOPE:                  PRIVATE - declared in nsuuid.h
**
**  DESCRIPTION:
**
**  This routine releases memory used by a dynamically created name service
**  representation of a UUID, nsuuid. nsuuid was dynamically created by calling
**  rpc__nsuuid_from_uuid.
**
**  INPUTS:                 none
**
**  INPUT/OUTPUTS:          
**      nsuuid              A pointer to a name service representation 
**                          of a UUID.
**                          Returns a NULL pointer on successful return.
**
**  OUTPUTS:
**
**      status              The return status value in *status indicates
**                          whether the routine completed successfully and, if
**                          not, why.
**                          Possible return values:
**                              rpc_s_ok
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         none
**
**  SIDE EFFECTS:           none
**
**--
*/

PRIVATE void rpc__nsuuid_free 
#ifdef _DCE_PROTO_
(
    rpc_nsuuid_p_t              *nsuuid,
    unsigned32                  *status
)
#else
(nsuuid, status)
rpc_nsuuid_p_t              *nsuuid;
unsigned32                  *status;
#endif
{

    CODING_ERROR (status);

    /*
     * free the rpc_nsuuid_t structure and set pointer to NULL
     */
    RPC_MEM_FREE (*nsuuid, RPC_C_MEM_NSUUID);

    *nsuuid = NULL;

    *status = rpc_s_ok;

    return;
}

/*
**++
**  ROUTINE NAME:           rpc__nsuuid_from_uuid
**
**  SCOPE:                  PRIVATE - declared in nsuuid.h
**
**  DESCRIPTION:
**
**  This routine creates a name service representation of a UUID from a
**  runtime representation of a UUID. 
**
**  Routine Description
**
**  INPUTS:
**
**      uuid                Runtime representation of the uuid
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:    
**
**      status              The return status value in *status indicates
**                          whether the routine completed successfully and, if
**                          not, why.
**                          Possible return values:
**                              rpc_s_ok
**                              rpc_s_no_memory
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         Returns a pointer to a name service representation
**                          of a UUID.
**
**  SIDE EFFECTS:           Allocates memory for the rpc_nsuuid_t structure.
**
**--
*/

PRIVATE rpc_nsuuid_t *rpc__nsuuid_from_uuid 
#ifdef _DCE_PROTO_
(
    uuid_p_t                    uuid,
    unsigned32                  *status
)
#else
(uuid, status)
uuid_p_t                    uuid;
unsigned32                  *status;
#endif
{
    rpc_nsuuid_p_t          nsuuid;


    CODING_ERROR (status);

    /*
     * allocate memory for the rpc_nsuuid_t structure, which is to be returned
     * to the caller
     */
    RPC_MEM_ALLOC (
        nsuuid,
        rpc_nsuuid_p_t,
        sizeof (rpc_nsuuid_t),
        RPC_C_MEM_NSUUID,
        RPC_C_MEM_WAITOK);

    if (nsuuid == NULL)
    {
        *status = rpc_s_no_memory;
        return (NULL);
    }

    /*
     * Copy the value of the uuid to the uuid field of nsuuid 
     */
    nsuuid->uuid = *uuid;

    /* 
     * All nameservice representatons are stored in little endian format.
     * Convert the uuid field to little endian representation if
     * we're not on a little endian machine.
     */
    RPC_RESOLVE_ENDIAN_UUID (nsuuid->uuid);

    *status = rpc_s_ok;

    return (nsuuid);
}

/*
**++
**  ROUTINE NAME:           rpc__nsuuid_mbr_remove
**
**  SCOPE:                  PRIVATE - declared in nsuuid.h
**
**  DESCRIPTION:        
**
**   Remove a name service representation of a UUID from a UUID set.
**
**  INPUTS:             
**
**      nsentry             Name service representation of the entry name that
**                          contains an object UUID which is to be removed.
**      nsobj_uuid          The object UUID to remove from the UUID set for the 
**                          namespace entry designated by nsentry.
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:            
**
**      status              The return status value in *status indicates
**                          whether the routine completed successfully and, if
**                          not, why.
**                          Possible return values:
**                              rpc_s_ok
**                              rpc_s_not_found
**                              return value from rpc__nsattr_exists
**                              return value from rpc__nsattr_remove_value
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         none
**
**  SIDE EFFECTS:           none
**
**--
*/

PRIVATE void rpc__nsuuid_mbr_remove 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t             nsentry,
    rpc_nsuuid_p_t              nsobj_uuid,
    unsigned32                  *status
)
#else
(nsentry, nsobj_uuid, status)
rpc_nsentry_p_t             nsentry;
rpc_nsuuid_p_t              nsobj_uuid;
unsigned32                  *status;
#endif
{
    CODING_ERROR (status);

    /* 
     * Determine if nsentry contains the object UUID attribute.
     */
    if ((rpc__nsattr_exists (nsentry, RPC_C_ATTR_OBJECT_UUIDS,
                        RPC_C_NS_BYPASS_CACHE, status)) == false)
    {
        /*
         * returns status == rpc_s_not_found or some other error
         */
        return;
    }

    /*
     * Remove the uuid field of nsobj_uuid from the object UUID attribute.
     */
    rpc__nsattr_remove_value ( 
        nsentry, 
        RPC_C_ATTR_OBJECT_UUIDS,
        (unsigned_char_p_t) &(nsobj_uuid->uuid),
        sizeof (uuid_t),
        status);

    return;
}

/*
**++
**  ROUTINE NAME:           rpc__nsuuid_to_uuid
**
**  SCOPE:                  PRIVATE - declared in nsuuid.h
**
**  DESCRIPTION:
**
**  Creates a runtime representation of a UUID from a name service
**  representation of a UUID.
**
**  INPUTS:
**
**      nsuuid              Name service representation of the UUID of
**                          interest
**
**  INPUT/OUTPUTS:          none
**
**
**  OUTPUTS:
**
**      uuid                Returns a runtime representation of a UUID in
**                          uuid.
**      status              The return status value in *status indicates
**                          whether the routine completed successfully and, if
**                          not, why.
**                          Possible return values:
**                              rpc_s_ok
**                          
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         none
**
**  SIDE EFFECTS:           none
**
**--
*/

PRIVATE void rpc__nsuuid_to_uuid 
#ifdef _DCE_PROTO_
(
    rpc_nsuuid_p_t              nsuuid,
    uuid_t                      *uuid,
    unsigned32                  *status
)
#else
(nsuuid, uuid, status)
rpc_nsuuid_p_t              nsuuid;
uuid_t                      *uuid;
unsigned32                  *status;
#endif
{
    CODING_ERROR (status);

    /*
     * Copy the value of the uuid pointed to by nsuuid to the uuid.
     */
    *uuid = nsuuid->uuid;

    /* 
     * All nameservice representatons are stored in little endian format.
     * Convert the uuid field to little endian representation if
     * we're not on a little endian machine.
     */
    RPC_RESOLVE_ENDIAN_UUID (*uuid);

    *status = rpc_s_ok;
    return;
}
