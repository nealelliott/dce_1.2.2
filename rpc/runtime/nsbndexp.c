/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsbndexp.c,v $
 * Revision 1.1.320.2  1996/02/18  00:04:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:01  marty]
 *
 * Revision 1.1.320.1  1995/12/08  00:21:21  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:34 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/3  1995/09/05  14:42 UTC  jrr
 * 	Fix various memory leaks.
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_01/1  1995/09/05  14:41 UTC  jrr
 * 	Fix memory leak in rpc_ns_binding_export()
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/25  21:41 UTC  lmm
 * 	Merge allocation failure detection cleanup work
 * 
 * 	HP revision /main/HPDCE02/lmm_alloc_fail_clnup/1  1995/05/25  21:02 UTC  lmm
 * 	allocation failure detection cleanup
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:16 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:20 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/07  23:59:59  root]
 * 
 * Revision 1.1.318.2  1994/07/29  16:19:17  tom
 * 	Add entry type to rpc__nsentry_create arguments. (OT 7945)
 * 	[1994/07/28  22:33:56  tom]
 * 
 * Revision 1.1.318.1  1994/01/21  22:38:28  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:54  cbrooks]
 * 
 * Revision 1.1.2.5  1993/02/24  16:57:33  mhickey
 * 	Fixed incorrect comment describing logic of if block.
 * 
 * 	defect 3678
 * 	[1993/02/23  17:42:46  mhickey]
 * 
 * 	Fixed pointer math in rpc_ns_binding_export so that the
 * 	index pointer used to loop through the object uuid vector
 * 	goes to the next object uuid pointer when incremented.
 * 	Actually, this amounted to just making the pointer
 * 	a uuid_t ** and adding the appropriate *'s in the right
 * 	places.  I also cleaned up some logic.
 * 	[1993/02/11  19:28:26  mhickey]
 * 
 * Revision 1.1.2.4  1993/01/15  23:56:09  weisman
 * 	Fix for OT 3131.  Logic now independently tracks the output
 * 	index.  Also major cleanup of up error recovery.
 * 	[1993/01/15  22:12:31  weisman]
 * 
 * Revision 1.1.2.3  1993/01/03  23:53:33  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:10:06  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:13:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:41:51  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:12:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      nsbndexp.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) Naming Services (NS)
**
**  ABSTRACT:
**
**      PUBLIC RPC naming service binding export operations are supported by 
**      the routines defined in this module. 
**
*/

#include <commonp.h>
#include <com.h> 
#include <ns.h>              /* ns defs for other RPC components */
#include <nsp.h>             /* defs for Naming Service component */
#include <comtwr.h>          /* tower defs for other RPC components */
#include <comtwrref.h>       /* tower defs for other RPC components */
#include <comtwrflr.h>       /* tower floor defs for other RPC */
#include <nsutil.h>          /* name service specific routines */
#include <nstower.h>         /* Externals for NS Towers sub-component */
#include <nsentry.h>         /* entry defs for other RPC components */
#include <nsuuid.h>          /* ns uuid defs for other RPC components */

#define COUNT_UNKNOWN   (-1)


/*
 * prototype declarations for locally defined routines
 */

INTERNAL unsigned32 rpc__ns_binding_vector_copy _DCE_PROTOTYPE_ ((
        rpc_binding_vector_p_t   /*src_vector*/,
        unsigned32               /*count_in*/,
        rpc_binding_vector_p_t  * /*dest_vector*/,
        unsigned32              * /*status*/
     ));


/*
**++
**  ROUTINE NAME:       rpc_ns_binding_export
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  Establishes a name-service database entry with multiple bindings for a 
**  server and, optionally, for multiple objects.
**
**  This routine adds either or both of the following to a namespace entry:
**      - for server access, an interface and bindings
**      - for objects serviced by the server, individual object UUIDs
**
**  INPUTS:             
**
**  entry_name_syntax     An integer value that specifies the syntax of the 
**                      entry_name argument. To use the default, specify a
**                      value of NULL
**  entry_name            a character string representation of the entry name,
**                      in the designated syntax, which is to receive protocol
**                      towers and object UUIDs
**  if_spec               Interface specification. A NULL argument value
**                      indicates there are no bindings to export (only UUIDs
**                      are to be exported) and the binding_vec argument is
**                      ignored.
**  binding_vec           Bindings to export. A NULL argument value indicates
**                      there are no bindings to export.
**  object_uuid_vec       Object UUIDs serviced by the exporting server. A 
**                      NULL argument value indicates there are no object
**                      UUIDs to export.
**
**  INPUT/OUPUTS:       
**
**  OUTPUTS:            
**
**      status          The return status is in *status
**                      rpc_s_ok
**                      rpc_s_nothing_to_export
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void rpc_ns_binding_export 
#ifdef _DCE_PROTO_
(
    unsigned32              entry_name_syntax,
    unsigned_char_p_t       entry_name,
    rpc_if_handle_t         if_spec,
    rpc_binding_vector_p_t  binding_vec,
    uuid_vector_p_t         object_uuid_vec,
    unsigned32              *status
)
#else
( entry_name_syntax, entry_name, if_spec, binding_vec, object_uuid_vec, status) 
unsigned32              entry_name_syntax;
unsigned_char_p_t       entry_name;
rpc_if_handle_t         if_spec;
rpc_binding_vector_p_t  binding_vec;
uuid_vector_p_t         object_uuid_vec;
unsigned32              *status;
#endif
{
    unsigned32                  name_syntax;
    rpc_nsentry_t               *nsentry;
    rpc_binding_handle_t        binding_h;
    rpc_binding_vector_p_t      binding_vec_copy;
    unsigned32                  index, j;
    unsigned32                  binding_count;
    unsigned32                  obj_count;
    rpc_tower_ref_vector_p_t    tower_vector;
    twr_t                       *tower_p;
    uuid_p_t                    *uuid_pp;
    rpc_nsuuid_t                *nsuuid_p;
    unsigned32                  temp_status;

    CODING_ERROR(status);

    RPC_NS_VERIFY_INIT ();

    binding_vec_copy = NULL;

    /* 
     * Return rpc_s_nothing_to_export if either the if_spec or the binding_vec
     * are NULL AND the object_uuid_vec is NULL as well.
     */
    if ( ((if_spec == (rpc_if_handle_t) NULL) || 
          (binding_vec ==(rpc_binding_vector_p_t) NULL)) && 
         (object_uuid_vec == (uuid_vector_p_t) NULL))
    {
        *status = rpc_s_nothing_to_export;
        return;
    }

    /*
     * Put the count of the number of non-NULL bindings to export 
     * in binding_count.
     */
    binding_count = 0;
    if (binding_vec != (rpc_binding_vector_p_t) NULL)
    {
        /*
         * For each binding in binding_vec ...
         */
        for ( index = 0;
              index < binding_vec->count; 
              index++ )
        {
            /* 
             * ... increment binding_count if it is a non-NULL binding
             */
            if (binding_vec->binding_h[index] != 
                    (rpc_binding_handle_t) NULL)
            {
                binding_count++;
            }
        }
    }

    /*
     * Set the object uuid count to the number of non-NIL, non-NULL object
     * uuids in the vector by calling this INTERNAL routine.
     */
    obj_count = rpc__ns_count_obj_uuids_in_vec (object_uuid_vec);

    /* 
     * Return rpc_s_nothing_to_export if 
     * 1) either the if_spec or the binding_vec are NULL 
     * AND 
     * 2) the obj_count is 0 as well (it will be 0 if the object_uuid_vec 
     * is empty or NULL).
     */
    if (((if_spec == (rpc_if_handle_t) NULL) || (binding_count == 0)) && 
       (obj_count == 0))
    {
        *status = rpc_s_nothing_to_export;
        return;
    }

    if (binding_count > 0)
    {
        /*
         * Make a copy of the binding vector, filtering unwanted 
         * duplicate binding handles and resetting dynamic endpoints. 
         */
        binding_count = rpc__ns_binding_vector_copy (
            binding_vec, binding_count, &binding_vec_copy, status);

        if (*status != rpc_s_ok)
        {
            return;
        }
    }

    /*
     * Determine the correct entry name syntax before converting entry name.
     */
    name_syntax = entry_name_syntax;
    RPC_RESOLVE_SYNTAX (name_syntax, status);
    if (*status != rpc_s_ok)
    {
        if (binding_vec_copy != NULL)
        {
            rpc_binding_vector_free (&binding_vec_copy, &temp_status);
        }
        return;
    }

    nsentry = rpc__nsentry_from_entry (name_syntax, entry_name, status);
    if (*status != rpc_s_ok)
    {
        if (binding_vec_copy != NULL)
        {
            rpc_binding_vector_free (&binding_vec_copy, &temp_status);
        }
        return;
    }

    /*
     * Create entry -- it's ok if it already exists.
     */
    rpc__nsentry_create (nsentry, RPC_C_NS_ENTRY, status);
    if ((*status != rpc_s_ok) && 
        (*status != rpc_s_entry_already_exists))
    {
        rpc__nsentry_free (&nsentry, &temp_status); 
        if (binding_vec_copy != NULL)
        {
            rpc_binding_vector_free (&binding_vec_copy, &temp_status);
        }
        return; 
    }

    /*
     * If the interface is specified and there are bindings to export, do so.
     */
    if ((if_spec != (rpc_if_handle_t) NULL) && (binding_count > 0)) 
    {
        /*
         * for ...
         */
        for ( index = 0; 
              index < binding_vec_copy->count; 
              index++ )
        {
            binding_h = binding_vec_copy->binding_h[index];
    
            /* 
             * ... each non-NULL binding in binding_vec_copy ...
             */
            if (binding_h == (rpc_binding_handle_t) NULL) 
            {
                continue;
            }

            /*
             * ... export the binding/tower. 
             */
            rpc__tower_ref_vec_from_binding ((rpc_if_rep_p_t) if_spec, 
                                            binding_h, 
                                            &tower_vector,
                                            status);
            if (*status != rpc_s_ok) goto CLEANUP;

            for (j = 0; j < tower_vector->count; j++)
            {
                rpc__tower_from_tower_ref (tower_vector->tower[j], 
                                           &tower_p,
                                           status);
                if (*status != rpc_s_ok) {
                    rpc__tower_ref_vec_free (&tower_vector, &temp_status);
                    goto CLEANUP;
                }

                rpc__tower_mbr_add (nsentry, tower_p, status);
                if (*status != rpc_s_ok) {
                    rpc__tower_free (&tower_p, &temp_status);
                    rpc__tower_ref_vec_free (&tower_vector, &temp_status);
                    goto CLEANUP;
                }

                rpc__tower_free (&tower_p, status);
                if (*status != rpc_s_ok) {
                    rpc__tower_ref_vec_free (&tower_vector, &temp_status);
                    goto CLEANUP;
                }
            }

            rpc__tower_ref_vec_free (&tower_vector, &temp_status);

        }
    }

    /*
     * If there are objects to export, do so.
     */
    if (obj_count > 0)
    {
        /*
         * Initialize local pointer to uuid pointer to
         * the uuid vector's array of pointers.
         */
        uuid_pp = object_uuid_vec->uuid;
        /*
         * for each object uuid in object_uuid_vec ...
         */
        for ( index = 0; 
              index < object_uuid_vec->count; 
              index++, uuid_pp++ )
        {
            /* 
             * ... if the pointer is NULL or points to a NIL uuid...
             */
            if ( (*uuid_pp == (uuid_p_t) NULL)  || 
                 ((uuid_is_nil (*uuid_pp, status)) == true) )
            {
                /*---* skip it! *---*/
                continue;
            }

            /*
             * ... export the object uuid.
             */
            nsuuid_p = rpc__nsuuid_from_uuid (*uuid_pp, status);
            if (*status != rpc_s_ok) goto CLEANUP;

            rpc__nsuuid_mbr_add (nsentry, nsuuid_p, status);
            if (*status != rpc_s_ok) {
                rpc__nsuuid_free (&nsuuid_p, &temp_status);
                goto CLEANUP;
            }

            rpc__nsuuid_free (&nsuuid_p, status);
            if (*status != rpc_s_ok) goto CLEANUP;

        }
    }


CLEANUP:
    /*
     * Free memory allocated for the nsentry structure
     * and binding copy.
     */
    if (binding_vec_copy != NULL)
    {
        rpc_binding_vector_free (&binding_vec_copy, &temp_status);
    }

    rpc__nsentry_free (&nsentry, &temp_status); 

    return;

}


/*
**++
**  ROUTINE NAME:       rpc_ns_binding_unexport
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  Removes multiple bindings (and, optionally, objects) from an entry in the 
**  name-service database.
**
**  INPUTS:             
**
**  entry_name_syntax     Entry name syntax of the argument entry_name. The
**                      use the syntax un the default entry name syntax
**                      environment variable, specify a value of NULL.
**  entry_name            A character string representation of the entry name,
**                      in the designated syntax. Protocol towers and object
**                      uuids are to be removed from entry_name.
**  if_spec               Interface specification. 
**  object_uuid_vec       Object uuids which the server no longer wishes to
**                      service. A NULL argument value indicates there are no
**                      object UUIDs to unexport.
**
**  INPUT/OUPUTS:       
**
**  OUTPUTS:            
**
**      status          The return status is in *status can be 
**                      one of:
**                      rpc_s_ok
**                      or an error returned by rpc_ns_mgmt_binding_unexport
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void rpc_ns_binding_unexport 
#ifdef _DCE_PROTO_
(
    unsigned32              entry_name_syntax,
    unsigned_char_p_t       entry_name,
    rpc_if_handle_t         if_spec,
    uuid_vector_p_t         object_uuid_vec,
    unsigned32              *status
)
#else
( entry_name_syntax, entry_name, if_spec, object_uuid_vec, status) 
unsigned32              entry_name_syntax;
unsigned_char_p_t       entry_name;
rpc_if_handle_t         if_spec;
uuid_vector_p_t         object_uuid_vec;
unsigned32              *status;
#endif
{
    rpc_if_id_t         if_id;
    rpc_if_id_t         *if_id_p = NULL;


    CODING_ERROR(status);

    RPC_NS_VERIFY_INIT ();

    if (if_spec !=  NULL) 
    {
        rpc_if_inq_id (if_spec, &if_id, status);
        if (*status != rpc_s_ok)
        {
            return;
        }
        if_id_p = &if_id;
    }
    rpc_ns_mgmt_binding_unexport(
        entry_name_syntax,
        entry_name,
        if_id_p,
        rpc_c_vers_exact,
        object_uuid_vec,
        status);

    return;
}



/*
**++
**
**  ROUTINE NAME:       rpc__ns_binding_vector_copy 
**
**  SCOPE:              INTERNAL to this module
**
**  DESCRIPTION:
**      
**  Copies the INPUT rpc_binding_vector_t src_vector to the OUTPUT
**  dest_vector, filtering unwanted duplicate binding handles and resetting
**  dynamic endpoints. This routine is intended to be called by
**  rpc_ns_binding_export.
**
**  INPUTS:             
**
**      src_vector      a pointer to a binding vector that contains the
**                      bindings to export
**      count_in        the count of the non-NULL bindings in src_vector
**
**  INPUTS/OUTPUTS:     none 
**
**  OUTPUTS:            
**
**      dest_vector     a pointer to a binding vector that contains the
**                      filtered list of bindings to export
**      status          a code to indicate whether the routine suceeded or
**                      failed
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     
**  
**      returns         count of bindings to export, returning COUNT_UNKNOWN
**                      on error.  
**
**  SIDE EFFECTS:       none 
**
**--
**/

INTERNAL unsigned32 rpc__ns_binding_vector_copy 
#ifdef _DCE_PROTO_
(
    rpc_binding_vector_p_t  src_vector,
    unsigned32              count_in,
    rpc_binding_vector_p_t  *dest_vector,
    unsigned32              *status
)
#else
(src_vector, count_in, dest_vector, status)
rpc_binding_vector_p_t  src_vector;
unsigned32              count_in;
rpc_binding_vector_p_t  *dest_vector;
unsigned32              *status;
#endif
{
    int                     size;
    int                     i, j;
    rpc_binding_rep_p_t     binding_r;
    rpc_binding_vector_p_t  dest;
    unsigned32              count;
    unsigned_char_p_t       *str_bindings;
    unsigned32              temp_status;

   *dest_vector = NULL;
    dest = NULL;

    size = (int) (sizeof (rpc_binding_vector_t) + 
                  ((count_in - 1) * sizeof (rpc_binding_handle_t)));

    RPC_MEM_ALLOC (
        dest,
        rpc_binding_vector_p_t,
        size,
        RPC_C_MEM_BINDING_VEC,
        RPC_C_MEM_NOWAIT);
    if (dest == NULL){
	*status = rpc_s_no_memory;
	return(COUNT_UNKNOWN);
    }

    dest -> count = 0;          /* in case we take CLEANUP exit */
    str_bindings = NULL;        /* in case we take CLEANUP exit */

    count = 0;

    for (i = 0 ; i < src_vector->count; i++)
    {
        binding_r = (rpc_binding_rep_p_t) src_vector->binding_h[i];

        if (binding_r == NULL)
        {
            continue;
        }

        rpc_binding_copy (
            (rpc_binding_handle_t)binding_r, 
            &(dest->binding_h[count]),
            status);

        if (*status != rpc_s_ok)
        {
            goto CLEANUP;
        }

        dest->count = count + 1; /* track accurate dest->count in */
                                 /* case we take CLEANUP exit.  */ 

        /*
         * If the endpoint is_dynamic, remove the endpoint from the binding.
         */
        if (binding_r->addr_is_dynamic == true)
        {
            rpc_binding_reset (dest->binding_h[count], status);
            if (*status != rpc_s_ok) goto CLEANUP;

        }

        count++;                /* success */
    }                           /* for i */


    /*
     * For the trivial cases where the count is 1, we can just return.
     */
    if (dest->count == 1)
    {
        *dest_vector = dest;
        *status = rpc_s_ok;
        return (count);
    }

    /*
     * This section looks for and eliminates duplicate binding 
     * handles in the binding vector by comparing their string binding 
     * representations. 
     */

    /*
     * First, set up an array of pointers to string bindings.
     * Fill it up with string bindings for each non-NULL binding handle in
     * the binding vector.
     */
    RPC_MEM_ALLOC (
        str_bindings,
        unsigned_char_t **,
        (sizeof(unsigned_char_p_t) * (dest->count)),
        RPC_C_MEM_STRING,
        RPC_C_MEM_NOWAIT);
    if (str_bindings == NULL){
	*status = rpc_s_no_memory;
	goto CLEANUP;
    }
    memset(str_bindings, 0, (sizeof(unsigned_char_p_t) * (dest->count)));

    for (i = 0; i < dest->count; i++)
    {
        rpc_binding_to_string_binding ( 
            dest->binding_h[i], 
            &str_bindings[i],
            status);
        if (*status != rpc_s_ok) 
        {
            goto CLEANUP;
        }
    }

    for (i = 0; i < dest->count; i++)
    {
        if (str_bindings[i] == NULL)
        {
            continue;
        }
        for (j = i+1; j < dest->count; j++)
        {
            if (str_bindings[j] == NULL)
            {
                continue;
            }

            /*
             * Remove all duplicates.
             */
            if ((strcmp ((char *)str_bindings[i], 
                         (char *)str_bindings[j])) == 0)
            {
                /*
                 * Free the string binding, so that we don't compare
                 * the removed binding again.
                 */
                rpc_string_free (&str_bindings[j], &temp_status);

                rpc_binding_free (&(dest->binding_h[j]), status);
                if (*status != rpc_s_ok) goto CLEANUP;

                count = count - 1;
            }
        }
    }

    for (i = 0; i < (dest->count); i++)
    {
        if (str_bindings[i] != NULL)
        {
            rpc_string_free (&str_bindings[i], &temp_status);
        }
    }
    RPC_MEM_FREE (str_bindings, RPC_C_MEM_STRING);

    *dest_vector = dest;
    *status = rpc_s_ok;
    return (count);

CLEANUP:
    if (str_bindings != NULL)   /* have we allocated it yet? */
    {
        for (i = 0; i < (dest->count); i++)
        {
            /*
             * Check pointer before free'ing
             */
            if (str_bindings[i] != NULL)
            {
                rpc_string_free (&str_bindings[i], &temp_status);
            }
        }

        RPC_MEM_FREE (str_bindings, RPC_C_MEM_STRING);
    }                           /* non-null str_bindings */

    rpc_binding_vector_free (&dest, &temp_status);

    return (COUNT_UNKNOWN);
}
