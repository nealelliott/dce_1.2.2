/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsbndimp.c,v $
 * Revision 1.1.321.2  1996/02/18  00:05:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:02  marty]
 *
 * Revision 1.1.321.1  1995/12/08  00:21:22  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:34 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:16 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:20 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/08  00:00:00  root]
 * 
 * Revision 1.1.319.1  1994/01/21  22:38:30  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:55  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:53:36  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:10:10  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:13:30  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:41:56  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:11:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      NSBNDIMP.C
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) Naming Services (NS)
**
**  ABSTRACT:
**
**      PUBLIC RPC naming service binding import operations are supported 
**      by the routines defined in this module. 
**
*/

#include <commonp.h>         /* RPC common definitions                    */
#include <com.h>             /* RPC communication definitons              */
#include <comp.h>            /* Private Communication Services            */
#include <ns.h>              /* Private NS defs for other RPC components  */
#include <nsp.h>             /* Private defs for Naming Service component */
#include <nsentry.h>         /* Externals for NS Entry sub-component      */

/*
**++
**  ROUTINE NAME:           rpc_ns_binding_import_begin
**
**  SCOPE:                  PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**  Creates an import context for an interface and optionally an object.
**
**  INPUTS:
**
**      entry_name_syntax   An integer value specifying the syntax of
**                          the entry_name argument.
**
**      entry_name          Entry name at which the search for compatible 
**                          binding begins.  A NULL pointer or null string
**                          specifies to use the entry name found in the
**                          RPC_DEFAULT_ENTRY environment variable.
**
**      if_spec             A handle to an interface specification.
**
**      object_uuid         An optional object UUID. A NULL pointer if
**                          there is no object UUID.
**      
**  INPUT/OUPUTS:           none
**
**  OUTPUTS:
**
**      import_context      A handle to the import context.
**
**      status              The result of the operation. One of:
**                              rpc_s_ok
**                              rpc_s_no_memory
**                          Status from rpc_ns_binding_lookup_begin()
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         void
**
**  SIDE EFFECTS:           none
**
**--
*/

PUBLIC void rpc_ns_binding_import_begin 
#ifdef _DCE_PROTO_
(
    unsigned32                  entry_name_syntax,
    unsigned_char_p_t           entry_name,
    rpc_if_handle_t             if_spec,
    uuid_p_t                    object_uuid,
    rpc_ns_handle_t	            *import_context,
    unsigned32                  *status
)
#else
(entry_name_syntax, entry_name, if_spec, object_uuid, import_context, status)
unsigned32                  entry_name_syntax;
unsigned_char_p_t           entry_name;
rpc_if_handle_t             if_spec;
uuid_p_t                    object_uuid;
rpc_ns_handle_t	            *import_context;
unsigned32                  *status;
#endif
{
    rpc_import_rep_t            *import_rep;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * Allocate the import context.
     */
    RPC_MEM_ALLOC (
        import_rep, 
        rpc_import_rep_p_t, 
        sizeof (rpc_import_rep_t), 
        RPC_C_MEM_IMPORT, 
        RPC_C_MEM_WAITOK);
    if (import_rep == NULL){
	*status = rpc_s_no_memory;
	return;
    }
                   
    /*
     * Use the global expiration age for determining 
     * whether the name service cache needs to be
     * refreshed.
     */
    import_rep->common.exp_age = rpc_g_ns_exp_age;
    
    /*
     * Find a compatible binding for this interface.
     */
    rpc_ns_binding_lookup_begin (entry_name_syntax, entry_name, if_spec, 
        object_uuid, rpc_c_binding_max_count_default,
        &(import_rep->lookup_context), status);

    if (*status == rpc_s_ok)
    {
        /*
         * Set the binding vector field to NULL
         * And return the handle to the import context.
         */
        import_rep->binding_vec = (rpc_binding_vector_t *) NULL;
        *import_context = (rpc_ns_handle_t) import_rep;
    }
    else
    {
        /*
         * An error occurred.  Free the import context
         * and return NULL.
         */
        RPC_MEM_FREE (import_rep, RPC_C_MEM_IMPORT);
        *import_context = (rpc_ns_handle_t) NULL;
    }

    return;
}

/*
**++
**  ROUTINE NAME:           rpc_ns_binding_import_done
**
**  SCOPE:                  PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**  Signifies a client has finished looking for a compatible server
**  and deletes the import context.
**
**  INPUTS:                 none
**      
**  INPUT/OUPUTS:           
**
**      import_context      A handle to the import context.
**                          Nulled on return if successful.
**
**  OUTPUTS:
**
**      status              The result of the operation. One of:
**                              rpc_s_ok
**                              rpc_s_invalid_ns_handle
**                          Status from rpc_ns_binding_lookup_done()
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

PUBLIC void rpc_ns_binding_import_done 
#ifdef _DCE_PROTO_
(
    rpc_ns_handle_t		    *import_context,
    unsigned32                  *status
)
#else
(import_context, status)
rpc_ns_handle_t		    *import_context;
unsigned32                  *status;
#endif
{
    rpc_import_rep_t            *import_rep;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * Get the address to the import representation.
     */
    import_rep = (rpc_import_rep_t *) *import_context;

    if (import_rep == NULL)
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    /*
     * Delete the binding vector, if one exists.
     */
    if (import_rep->binding_vec != NULL)
        rpc_binding_vector_free(&(import_rep->binding_vec),
                                status);

    /*
     * Delete the binding lookup context.
     */
    rpc_ns_binding_lookup_done (&(import_rep->lookup_context), status);

    if (*status == rpc_s_ok)
    {
        RPC_MEM_FREE (import_rep, RPC_C_MEM_IMPORT);
        *import_context = (rpc_ns_handle_t) NULL;
    }

    return;
}

/*
**++
**  ROUTINE NAME:           rpc_ns_binding_import_next
**
**  SCOPE:                  PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**  Returns a binding of a compatible server (if found) using an
**  optional interface and/or optional object from a name-service
**  database.
**
**  INPUTS:                 
**
**      import_context      A handle to the import context.
**
**  INPUT/OUPUTS:           none
**
**  OUTPUTS:
**      
**      binding             Returns a handle to a binding that is compatible 
**                          to the interface.
**      
**      status              The result of the operation. One of:
**                              rpc_s_ok
**                              rpc_s_invalid_ns_handle
**                          Status from rpc_ns_binding_lookup_next(),
**                              rpc_ns_binding_select()
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         void
**
**  SIDE EFFECTS:           none
**
**--
*/

PUBLIC void rpc_ns_binding_import_next 
#ifdef _DCE_PROTO_
(
    rpc_ns_handle_t		    import_context,
    rpc_binding_handle_t        *binding,
    unsigned32                  *status
)
#else
(import_context, binding, status)
rpc_ns_handle_t		    import_context;
rpc_binding_handle_t        *binding;
unsigned32                  *status;
#endif
{
    rpc_import_rep_t            *import_rep;
    unsigned32                  internal_status;



    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * Get the address to the import representation.
     * If null, return with error.
     */
    import_rep = (rpc_import_rep_t *) import_context;

    if (import_rep == NULL)
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    /* 
     * Use the cache expiration age from the import rep in case
     * the application set it. If the application did not specify
     * the age, we'll be getting the global value from the time
     * when the import rep was created (which is the same value
     * as when the lookup rep was created).
     */
    rpc_ns_mgmt_handle_set_exp_age (import_rep->lookup_context,
                                    import_rep->common.exp_age,
                                    status);
                                    
    if ( *status != rpc_s_ok)
    {
        /*
         * An error occurred. Return a NULL binding handle.
         * and the status containing the error.
         */
        *binding = (rpc_binding_handle_t) NULL;
        
        return;
    }    

    /*
     * Enter a unconditional for loop, doing a lookup and select 
     * on a compatible binding.  If rpc_s_no_more_bindings is
     * returned from the select call, we null the binding vector and
     * continue the loop, obtaining the next set of binding candidates.
     */
    for (;;)
    {
        if (import_rep->binding_vec == (rpc_binding_vector_t *) NULL)
        {
            rpc_ns_binding_lookup_next (
                import_rep->lookup_context,
                &(import_rep->binding_vec),
                status);

            if (*status != rpc_s_ok)
            {
                /*
                 * An error occurred.  Return a NULL binding handle
                 * and the status containing the error.
                 */
                *binding = (rpc_binding_handle_t) NULL;
                return;
            }
        }

        /*
         * Select one binding from the returned list of bindings.
         */
        rpc_ns_binding_select (import_rep->binding_vec, binding, status);

        switch ((int)*status)
        {
            case rpc_s_ok:
                /* 
                 * A binding was selected.
                 */
                break;

            case rpc_s_no_more_bindings:
                /*
                 * No more bindings.  Need to go back and get more.
                 */
                rpc_binding_vector_free(&(import_rep->binding_vec),
                                        &internal_status);
                import_rep->binding_vec = (rpc_binding_vector_t *) NULL;
                break;

            default:
                /*
                 * An error occurred somewhere.
                 * Null binding argument, and return the status.
                 */
                rpc_binding_vector_free(&(import_rep->binding_vec),
                                        &internal_status);
                *binding = (rpc_binding_handle_t) NULL;
                break;

        } /* end switch */

        /*
         * Return with status and proper binding argument if 
         * status is anything other than rpc_s_no_more_bindings.
         */
        if (*status != rpc_s_no_more_bindings)
        {
            return;
        }

        /* 
         * Otherwise, go back for next set of bindings.
         */

    } /* end for */
}

/*
**++
**  ROUTINE NAME:           rpc_ns_binding_inq_entry_name
**
**  SCOPE:                  PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**  Returns the name of the name service entry from which a given binding
**  was obtained.
**
**  INPUTS:                 
**
**      binding             A handle on the binding for which the source
**                          entry name is to be obtained.
**
**      entry_name_syntax   The syntax in which the entry name is to be
**                          returned.
**
**  INPUT/OUPUTS:           
**      entry_name          The name of the name service entry from which
**                          the binding was obtained.  If NULL on input,
**                          no entry name is returned.
**
**  OUTPUTS:
**      
**
**      status              The result of the operation. One of:
**                              rpc_s_ok 
**                              rpc_s_no_memory
**                              rpc_s_no_entry_name
**                              rpc_s_invalid_binding
**
**
**  IMPLICIT INPUTS:        none
**
**  IMPLICIT OUTPUTS:       none
**
**  FUNCTION VALUE:         void
**
**  SIDE EFFECTS:           none
**
**--
*/

PUBLIC void rpc_ns_binding_inq_entry_name 
#ifdef _DCE_PROTO_
(
    
    rpc_binding_handle_t        binding,
    unsigned32                  entry_name_syntax,
    unsigned_char_p_t           *entry_name,
    unsigned32                  *status
    
)
#else
(binding, entry_name_syntax, entry_name, status)

rpc_binding_handle_t        binding;
unsigned32                  entry_name_syntax;
unsigned_char_p_t           *entry_name;
unsigned32                  *status;

#endif
{
    unsigned_char_t         binding_entry_name[DNS_STR_FNAME_MAX];
    unsigned32              entry_name_length;    

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();
                    
    RPC_BINDING_VALIDATE((rpc_binding_rep_p_t) binding, status);
    if (*status != rpc_s_ok)
        return;

    /*
     * check to see if the default syntax is needed
     */
    RPC_RESOLVE_SYNTAX (entry_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * check to see if this binding has an entry name
     */
    
    if ( (((rpc_binding_rep_p_t) binding)->ns_specific ) == NULL )
    {
        *status = rpc_s_no_entry_name;
        return;
    }
    
    /*
     * convert dns opaque name (in the nsentry structure ns_specific points to)
     * to an ascii string.
     */
     
    rpc__nsentry_to_entry (
        (rpc_nsentry_p_t) ((rpc_binding_rep_p_t) binding)->ns_specific ,
        entry_name_syntax,
        (unsigned_char_p_t) binding_entry_name,
        &entry_name_length,
        status );

    if (*status != rpc_s_ok )
    {
        return;        
    }

    /*
     * return the entry name only if the caller wants it.
     */
    if (entry_name != NULL)
    {
        /*
         * heap allocate storage for the entry name to return to the caller
         */
        RPC_MEM_ALLOC (
            *entry_name,
            unsigned_char_p_t,
            strlen ((char*) binding_entry_name) + 1,
            RPC_C_MEM_STRING,
            RPC_C_MEM_WAITOK);
        
        /*
         * copy the one in the binding into it
         */
        strcpy ((char *) *entry_name, (char *) binding_entry_name);
    }
    
    *status = rpc_s_ok;
    return;
}
