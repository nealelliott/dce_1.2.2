/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsentry.c,v $
 * Revision 1.1.524.2  1996/02/18  00:05:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:03  marty]
 *
 * Revision 1.1.524.1  1995/12/08  00:21:23  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:34 UTC  jrr
 * 	Remove Mothra specific code
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
 * 	[1995/12/08  00:00:01  root]
 * 
 * Revision 1.1.520.2  1994/07/29  16:19:18  tom
 * 	Add entry type to rpc__nsentry_create arguments. (OT 7945)
 * 	[1994/07/28  22:34:00  tom]
 * 
 * Revision 1.1.520.1  1994/01/21  22:38:31  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:57  cbrooks]
 * 
 * Revision 1.1.4.8  1993/01/08  21:47:15  weisman
 * 	Resubmitting Grace Hsiao changes.
 * 	[1993/01/07  21:39:35  weisman]
 * 
 * Revision 1.1.4.4  1992/11/16  22:20:13  zeliff
 * 	Re-submitting Grace Hsiao's changes.
 * 	[1992/11/16  22:19:02  zeliff]
 * 
 * Revision 1.1.4.2  1992/11/05  16:37:38  grace
 * 	28-sep-92    gh   Remove rpc__ns_check_name_syntax() -- checked in
 * 	                  RPC_RESOLVE_SYNTAX macro.
 * 	[1992/11/05  15:26:22  grace]
 * 
 * Revision 1.1.2.2  1992/05/01  16:38:48  rsalz
 * 	 3-mar-92    wh      DCE 1.0.1 merge.
 * 	21-jan-92    ebm     Change attribute read on rpc__nsattr_read_resolution
 * 	                     to be our group attribute, not CDS' cts attribute.
 * 	[1992/05/01  16:30:25  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:11:14  devrcs
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
**     NSENTRY.C
**
**  FACILITY:
**
**     Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**      Name Service Entry module containing lower level routines that
**      operate on the specific name service entry names. The purpose of
**      this module is to isolate those routines containing name service
**      specific calls from the Application Public Interface (API) routines.
**
**
*/

#include <commonp.h>    /* Common internals for RPC Runtime system      */
#include <com.h>        /* Externals for Common Services component      */
#include <ns.h>         /* Externals for Name Services component        */
#include <nsp.h>        /* Internals for Name Services component        */
#include <comtwr.h>     /* Externals for Towers sub-component           */
#include <nsutil.h>     /* Externals for NS Utility sub-component       */
#include <nsentry.h>    /* Externals for NS Entry sub-component         */
#include <nsattr.h>     /* Private routines for NS attributes           */
#include <nsuuid.h>     /* Private routines for NS uuid conversions     */


/*
**++
**  ROUTINE NAME:       rpc_ns_entry_expand_name
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  Expands a name service entry name.
**
**  INPUTS:             
**
**      entry_name_syntax   An integer value that specifies the syntax
**                          of the entry name.
**
**      entry_name          The entry name to expand.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      expanded_name       Returns a pointer to the expanded version
**                          of an entry_name.
**
**      status              Returns the status code from the expand entry
**                          name operation.  This status code is a value
**                          that indicates whether the routine completed 
**                          successfully and, if not, why. 
**                          Returns one of:
**                              rpc_s_ok
**                          or status from a called routine.
**                          
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
PUBLIC void rpc_ns_entry_expand_name 
#ifdef _DCE_PROTO_
(
    unsigned32              entry_name_syntax,
    unsigned_char_p_t       entry_name,
    unsigned_char_p_t       *expanded_name,
    unsigned32              *status
)
#else
(entry_name_syntax, entry_name, expanded_name, status)
unsigned32              entry_name_syntax;
unsigned_char_p_t       entry_name;
unsigned_char_p_t       *expanded_name;
unsigned32              *status;
#endif
{
    unsigned32          new_entry_name_syntax,
                        expanded_length;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * Copy the given entry name and syntax into local variables, because
     * we might have to modify them.  Resolve the given name and syntax
     * using defaults if required.
     */
    new_entry_name_syntax = entry_name_syntax;
    RPC_RESOLVE_SYNTAX (new_entry_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Allocate the space for the expanded name.
     */
    RPC_MEM_ALLOC (
        *expanded_name,
        unsigned_char_p_t,
        RPC_C_ENTRY_NAME_MAX,
        RPC_C_MEM_STRING,
        RPC_C_MEM_WAITOK);
    if (*expanded_name == NULL){
	*status = rpc_s_no_memory;
	return;
    }

    /*
     * Call the internal routine that expands a name.
     */
    rpc__ns_expand_entry_name (
        entry_name, *expanded_name,  &expanded_length, status);

    /*
     * If an error occurs, free memory and NULL pointer.
     */
    if (*status != rpc_s_ok)
    {
        RPC_MEM_FREE (*expanded_name, RPC_C_MEM_STRING);
        *expanded_name = NULL;
    }

    return;
}


/*
**++
**  ROUTINE NAME:       rpc__ns_expand_entry_name
**
**  SCOPE:              PRIVATE - declared in nsentry.h
**
**  DESCRIPTION:
**
**  Expands a name service entry name to a full name, including
**  the root prefix and expanded simple names.
**
**  INPUTS:
**
**      entry_name      The name to be expanded.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:
**
**      expanded_name   The expanded entry name.
**
**      expanded_length The length of the expanded name include the NUL
**                      terminator.
**
**      status          Returns one of
**                          rpc_s_ok,
**                          rpc_s_incomplete_name,
**                      or status from a called routine.
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

PRIVATE void rpc__ns_expand_entry_name 
#ifdef _DCE_PROTO_
(
    unsigned_char_p_t   entry_name,
    unsigned_char_t     *expanded_name,
    unsigned32          *expanded_length,
    unsigned32          *status
)
#else
(entry_name, expanded_name, expanded_length, status)
unsigned_char_p_t   entry_name;
unsigned_char_t     *expanded_name;
unsigned32          *expanded_length;
unsigned32          *status;
#endif
{   
    int             temp_length = RPC_C_ENTRY_NAME_MAX,
                    dns_status;
    

    CODING_ERROR (status);


    dns_status = dnsExpandStrFull 
                    (entry_name, expanded_name, &temp_length);
    
    if (dns_status != DNS_SUCCESS) 
    {
        *status = rpc_s_incomplete_name;
    }
    else
    {
        *status = rpc_s_ok;
    }

    /*
     * Return length (include NUL).
     */
    *expanded_length = temp_length;

    return;
}



/*
**++
**  ROUTINE NAME:       rpc_ns_entry_object_inq_begin 
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  This routine creates an inquiry context for a name service entry's
**  objects. This routine is called prior to calling the
**  rpc_ns_entry_object_inq_next routine.
**
**  INPUTS:             
**
**      entry_name_syntax   Name syntax of the entry name.
**
**      entry_name          String representation of the entry name.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      inquiry_context An inquiry context specifying an object UUID set.
**
**      status          The return status value in *status indicates
**                      whether the routine completed successfully and, if
**                      not, why. Possible return values:
**                          rpc_s_ok
**                          rpc_s_obj_uuid_not_found
**                      or status from a subsequent called routine.
**                          
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       Allocates memory for the inquiry context structure.
**
**--
*/

PUBLIC void rpc_ns_entry_object_inq_begin 
#ifdef _DCE_PROTO_
(
    unsigned32              entry_name_syntax,
    unsigned_char_p_t       entry_name,
    rpc_ns_handle_t         *inquiry_context,
    unsigned32              *status
)
#else
(entry_name_syntax, entry_name, inquiry_context, status)
unsigned32              entry_name_syntax;
unsigned_char_p_t       entry_name;
rpc_ns_handle_t         *inquiry_context;
unsigned32              *status;
#endif
{

    unsigned32              new_entry_name_syntax;
    rpc_nsentry_p_t         nsentry;
    rpc_ns_inq_rep_p_t      inq = NULL;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * Copy the given entry name and syntax into local variables, because
     * we might have to modify them.  Resolve the given name and syntax
     * using defaults if required and convert to internal form.
     */
    new_entry_name_syntax = entry_name_syntax;
    RPC_RESOLVE_SYNTAX (new_entry_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Convert entry_name to name service representation.
     */
    nsentry = rpc__nsentry_from_entry (
                new_entry_name_syntax, entry_name, status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
    * Create an inquiry context.
    */
    inq = rpc__inqcntx_create (nsentry, rpc_e_uuid_member, status);

    *inquiry_context = (rpc_ns_handle_t) inq;
    return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_entry_object_inq_next 
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  This routine returns one object UUID from an object UUID set in a name 
**  service entry. A caller can view the members of an object UUID set by 
**  repeatedly calling this routine. The returned UUID is available only until 
**  the next call of this routine or until rpc_ns_entry_object_inq_done is 
**  called. In order to save the UUID, the caller must copy it to caller 
**  managed memory.
**
**  INPUTS:             
**
**      inquiry_context An opaque pointer to an inquiry context data rep.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      obj_uuid        The object uuid is returned from the set for the 
**                      namespace entry designated by the inquiry_context.
**
**      status          The return status value in *status indicates
**                      whether the routine completed successfully and, if
**                      not, why. Possible return values:
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
**                          rpc_s_no_more_members
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

PUBLIC void rpc_ns_entry_object_inq_next 
#ifdef _DCE_PROTO_
(
    rpc_ns_handle_t         inquiry_context,
    uuid_t                  *obj_uuid,
    unsigned32              *status
)
#else
(inquiry_context, obj_uuid, status)
rpc_ns_handle_t         inquiry_context;
uuid_t                  *obj_uuid;
unsigned32              *status;
#endif
{
    unsigned32              uuid_len;
    uuid_t                  *nsuuid;
    rpc_ns_inq_rep_p_t      inq;
    rpc_nsuuid_p_t          inq_nsuuid_ptr;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /* 
     * check against a bad (NULL) pointer
     */
    if (inquiry_context == NULL) 
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    /* 
     * check against an invalid inquiry context
     */
    inq = (rpc_ns_inq_rep_p_t) inquiry_context;
    if (inq->usage != rpc_e_uuid_member)
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    /*
     * set up pointer to facilitate accessing the correct part of the 
     * inquiry_context (the obj_uuid) when reading the UUID set member
     */
    inq_nsuuid_ptr = &(inq->inq_specific.inqcntx_nsuuid.nsobj_uuid);

    /*
     * read the address of a UUID set member to a local.
     */
    rpc__nsattr_read_value (inq, (unsigned_char_p_t *) &nsuuid, 
                            &uuid_len, status);

    if (*status == rpc_s_ok)
    {
        /*
         * Copy the contents of the address returned from DNS.
         */
        memcpy (&(inq_nsuuid_ptr->uuid), nsuuid, uuid_len);

	/*
	 * Convert nsuuid to runtime uuid.
	 * Note, this routine will correct for difference in endian.
	 */
        rpc__nsuuid_to_uuid ( 
            inq_nsuuid_ptr, 
            &(inq->inq_specific.inqcntx_nsuuid.obj_uuid),
            status);

        if (*status != rpc_s_ok)
        {
            return;
        }

        /*
         * copy the object uuid obtained into the caller's memory
         */
        *obj_uuid = inq->inq_specific.inqcntx_nsuuid.obj_uuid;
    }

    /*
     * if status != rpc_s_ok, rpc_s_no_more_members (or some other
     * error) will be returned in status here
     */
    return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_entry_object_inq_done 
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  This routine frees an inquiry context which was created by calling the 
**  rpc_ns_entry_object_inq_begin routine.
**
**  INPUTS:             none
**
**  INPUT/OUTPUTS:          
**
**      inquiry_context On call, it is an opaque pointer to an inquiry
**                      context specifying an object UUID.
**                      Returns a NULL pointer on successful return.
**
**  OUTPUTS:            
**
**      status          The return status value in *status indicates
**                      whether the routine completed successfully and, if
**                      not, why. Possible return values:
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
**                      return value from rpc__inqcntx_free 
**                          
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

PUBLIC void rpc_ns_entry_object_inq_done 
#ifdef _DCE_PROTO_
(
    rpc_ns_handle_t         *inquiry_context,
    unsigned32              *status
)
#else
(inquiry_context, status)
rpc_ns_handle_t         *inquiry_context;
unsigned32              *status;
#endif
{
    rpc_ns_inq_rep_p_t      inq;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /* 
     * Check against a bad (NULL) pointer.
     */
    if (*inquiry_context == NULL) 
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    /*
     * Check against an invalid inquiry context
     */
    inq = (rpc_ns_inq_rep_p_t) *inquiry_context;
    if (inq->usage != rpc_e_uuid_member)
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    rpc__inqcntx_free (&inq, status);
    *inquiry_context = (rpc_ns_handle_t) inq;

    return;
}


/*
**++
**  ROUTINE NAME:      rpc_ns_entry_inq_resolution
**
**  SCOPE:             PUBLIC - declared in RPC.IDL
**
**  DESCRIPTION:
**
**  The rpc_ns_entry_inq_resolution routine attempts to read from
**  name-service database entry name.  If the read was successful
**  then the full resolution of the entry name is returned.  If
**  the read was not successful due to the entry not being found,
**  then the partial results of the name resolution is returned.
**
**  INPUTS:
**      entry_name_syntax   An integer value that specifies the syntax
**                          of the next argument, entry_name.
**      entry_name          Entry name on which the attempted name resolution
**                          is to be done.
**
**  INPUT/OUTPUTS:     
**      resolved_name       Returns a pointer to the resolved portion of
**                          the entry name.
**                          If NULL on input, nothing is returned.
**      
**      unresolved_name     Returns a pointer to the unresolved portion of
**                          the entry name.
**                          If NULL on input, nothing is returned.
**
**  OUTPUTS:
**      status              Returns the status code from the inquire-resolution
**                          operation.  One of:
**                              rpc_s_ok
**                              rpc_s_partial_results
**                              rpc_s_invalid_name_syntax
**                              rpc_s_unsupported_name_service
**
**  IMPLICIT INPUTS:   none
**
**  IMPLICIT OUTPUTS:  none
**
**  FUNCTION VALUE:    void
**      
**  SIDE EFFECTS:      none
**
**--
*/
PUBLIC void rpc_ns_entry_inq_resolution 
#ifdef _DCE_PROTO_
(
    unsigned32          entry_name_syntax,
    unsigned_char_p_t   entry_name,
    unsigned_char_p_t   *resolved_name,
    unsigned_char_p_t   *unresolved_name,
    unsigned32          *status
)
#else
(entry_name_syntax, entry_name, resolved_name, unresolved_name, status)
unsigned32          entry_name_syntax;
unsigned_char_p_t   entry_name;
unsigned_char_p_t   *resolved_name;
unsigned_char_p_t   *unresolved_name;
unsigned32          *status;
#endif
{
    rpc_nsentry_p_t nsentry;
    unsigned_char_t partial_results [RPC_C_ENTRY_NAME_MAX];
    unsigned32      new_entry_name_syntax,
                    partial_results_length,
                    residual_length,
                    rtn_status;

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();
    
    /*
     * Set output values to NULL in case of failure.
     */
    if (resolved_name != NULL)
    {
        *resolved_name = NULL;
    }

    if (unresolved_name != NULL)
    {
        *unresolved_name = NULL;
    }

    /*
     * Resolve name syntax
     */
    new_entry_name_syntax = entry_name_syntax;
    RPC_RESOLVE_SYNTAX (new_entry_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Convert entry name to a name service representation.
     * This puts it in its expanded form.
     */
    nsentry = rpc__nsentry_from_entry 
        (new_entry_name_syntax, entry_name, status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Call rpc__nsattr_read_resolution to see if the name 
     * resolves by reading our group attribute.
     */
    rpc__nsattr_read_resolution (nsentry, RPC_C_ATTR_GROUP, 
        partial_results, &partial_results_length, &rtn_status);

    if (rtn_status == rpc_s_entry_not_found)
    {
        rtn_status = rpc_s_partial_results;
    }

    if ((rtn_status == rpc_s_ok) ||
        (rtn_status == rpc_s_partial_results))
    {
        /*
         * Heap allocation storage for the resolved portion of the name 
         * and copy the results only if the user wants it.
         */
        if (resolved_name != NULL)
        {
            RPC_MEM_ALLOC (
                *resolved_name,
                unsigned_char_p_t,
                partial_results_length+1,
                RPC_C_MEM_NSRESOLUTION,
                RPC_C_MEM_WAITOK);
	    if (*resolved_name == NULL){
		rpc__nsentry_free (&nsentry, status);
		*status = rpc_s_no_memory;
		return;
	    }

            strcpy ((char *) *resolved_name, (char *)partial_results);
        }


        /*
         * Now extract the portion of the entry name that was not resolved.
         */
        residual_length = 
            strlen ((char *)nsentry->name) - partial_results_length;
            
        /*
         * If there is a residual, skip over the separator so we
         * return a relative name.
         */
        if (residual_length > 0)
        {
            residual_length--;
            partial_results_length++;
        }

        /*
         * Heap allocate storage and copy that portion of the name 
         * which could not be resolved only if user wants it.
         */
        if (unresolved_name != NULL)
        {
            RPC_MEM_ALLOC (
                *unresolved_name,
                unsigned_char_p_t,
                residual_length+1,
                RPC_C_MEM_NSRESIDUAL,
                RPC_C_MEM_WAITOK);
	    if (*unresolved_name == NULL){
		rpc__nsentry_free (&nsentry, status);
		RPC_MEM_FREE(*resolved_name, RPC_C_MEM_NSRESOLUTION);
		*resolved_name = NULL;
		*status = rpc_s_no_memory;
		return;
	    }

            strcpy ((char *) *unresolved_name,
                    (char *) nsentry->name + partial_results_length);
        }
    }

    /*
     * Free the name-service entry representation.
     */
    rpc__nsentry_free (&nsentry, status);

    /*
     * Return the results from the read_resolution operation unless
     * nsentry_free failed.
     */
    if (*status == rpc_s_ok)
    {
        *status = rtn_status;
    }

    return;
}


/*
**++
**  ROUTINE NAME:      rpc__nsentry_copy
**
**  SCOPE:             PRIVATE - declared in nsentry.h
**
**  DESCRIPTION:
**
**  Copies an RPC name-service entry to newly allocated name_service entry.
**
**  INPUTS:
**
**      nsentry         Name service representation of the entry name.
**
**  INPUT/OUPUTS:       none
**
**  OUTPUTS:          
**
**      status          Returns the status of the create entry operation.
**                      One of:
**                          rpc_s_ok,
**                          rpc_s_no_memory.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     
**      
**      Returns a pointer to a newly allocated name_service entry.
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE rpc_nsentry_p_t rpc__nsentry_copy 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t         nsentry,
    unsigned32              *status
)
#else
(nsentry, status)
rpc_nsentry_p_t         nsentry;
unsigned32              *status;
#endif
{
    rpc_nsentry_p_t nsentry_copy;


    CODING_ERROR (status);

    RPC_MEM_ALLOC (
	nsentry_copy,
	rpc_nsentry_p_t,
	sizeof (rpc_nsentry_t),
	RPC_C_MEM_NSENTRY,
	RPC_C_MEM_WAITOK);
    if (nsentry_copy == NULL){
	*status = rpc_s_no_memory;
	return NULL;
    }


    RPC_MEM_ALLOC (
	nsentry_copy->name,
	unsigned_char_p_t,
	RPC_C_ENTRY_NAME_MAX,
	RPC_C_MEM_NSENTRY, 
	RPC_C_MEM_NOWAIT);
    if (nsentry_copy == NULL){
	RPC_MEM_FREE(nsentry_copy, RPC_C_MEM_NSENTRY);
	*status = rpc_s_no_memory;
	return NULL;
    }

    
    nsentry_copy->name_length = nsentry->name_length;
    strcpy ((char *)nsentry_copy->name, (char *)nsentry->name);

    *status = rpc_s_ok;
    return nsentry_copy;
}


/*
**++
**  ROUTINE NAME:      rpc__nsentry_create
**
**  SCOPE:             PRIVATE - declared in nsentry.h
**
**  DESCRIPTION:
**
**  Creates an RPC name-service entry in the namespace.
**
**  INPUTS:
**
**      nsentry         Name service representation of the entry name.
**
**  INPUT/OUPUTS:       none
**
**  OUTPUTS:          
**
**      status          Returns the status of the create entry operation.
**                      One of:
**                          rpc_s_ok,
**                          rpc_s_entry_already_exists
**                          rpc_s_unknown_name_service_error
**                      or one of the mapped RPC name service errors.
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

PRIVATE void rpc__nsentry_create 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t         nsentry,
    unsigned_char_t         *attr_value,
    unsigned32              *status
)
#else
(nsentry, status)
rpc_nsentry_p_t         nsentry;
unsigned_char_t         *attr_value,
unsigned32              *status;
#endif
{
    rpc_nsattr_t            class_attr_value;       
    int                     dns_status;
    struct dnsFlagStat      dns_flags;


    CODING_ERROR (status);



    if (attr_value == NULL)
        attr_value = RPC_C_NS_ENTRY;

    strcpy ((char *)class_attr_value.name, (char *)attr_value); 

    /*
     * Initialize the dns flags.
     * Set the cache expiration age to RPC_C_NS_BYPASS_CACHE to
     * bypass the cache.
     */
     
    rpc__ns_init_dns_flags (&dns_flags, RPC_C_NS_BYPASS_CACHE);

    /*
     * Create the entry name (dns object).
     */
    dns_status = dnsCreateObj ((dns_full_name_t *) (nsentry->name), 
                 (dns_simple_name_t *) &class_attr_value, 
                 &rpc_ns_class_version, (dns_id *) NULL, 
                 (dns_cts *) NULL, &dns_flags); 
                 
    if (dns_status == DNS_SUCCESS)
    {
        /* 
         * Add the RPC_ClassVersion attribute to the already existing entry.
         */
        rpc__nsentry_make_it_rpc (nsentry, status);
        return;       
    }
    else
    {
        /*
         * Map dns error to rpc status
         */
        rpc__ns_status_from_nserror (&dns_flags, status);
        return;       
    }
}

/*
**++
**  ROUTINE NAME:       rpc__nsentry_exists
**
**  SCOPE:		PRIVATE - declared in nsentry.h
**
**  DESCRIPTION:
**
**  Determines whether a name service entry exits.
**
**  INPUTS:
**
**      nsentry         Name service representation of an entry name.
**
**  INPUT/OUTPUTS:	none
**
**  OUTPUTS:
**
**	status		The returned RPC status, one of:
**			    rpc_s_ok
**                          rpc_s_no_memory
**			    rpc_s_not_found
**                       or status from a subsequent called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      true      The specified entry exists in the designated namespace.
**      false     The specified entry does not exist in the namespace.
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE boolean rpc__nsentry_exists 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t         nsentry,
    unsigned32              *status
)
#else
(nsentry, status)
rpc_nsentry_p_t         nsentry;
unsigned32              *status;
#endif
{
    CODING_ERROR (status);

    /*
     * Determine if the entry contains the RPC attribute.
     */
    if (rpc__nsattr_exists (nsentry, RPC_C_ATTR_CLASS_VERSION, 
                            RPC_C_NS_BYPASS_CACHE, status))
    {
        return true;
    }
    else
    {
        /*
         * Entry is either non-existant or not an rpc entry.
         * If the attribute was not found, return the 
         * fact that it is not an RPC enry.
         */
        if (*status == rpc_s_not_found)
        {
            *status = rpc_s_not_rpc_entry;
        }
        return false;
    }
}


/*
**++
**  ROUTINE NAME:      rpc__nsentry_free
**
**  SCOPE:             PRIVATE - declared in nsentry.h
**
**  DESCRIPTION:
**
**  Release the resources used for a name service's 
**  representation of an entry name.
**
**  INPUTS:             none
**
**  INPUT/OUPUTS:       
**
**      nsentry         Name service representation of an
**                      entry name.  Nulled on return.
**
**  OUTPUTS:
**
**      status          The status code of the free operation.  
**                      Returns rpc_s_ok.
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

PRIVATE void rpc__nsentry_free 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t         *nsentry,
    unsigned32              *status
)
#else
(nsentry, status)
rpc_nsentry_p_t         *nsentry;
unsigned32              *status;
#endif
{
    CODING_ERROR (status);

#ifdef DEBUG
    /*
     * Catch memory use of already freed memory.
     */
    strncpy ((char *) ((*nsentry)->name), "Free at last, free at last",
            (*nsentry)->name_length);
#endif

    RPC_MEM_FREE ((*nsentry)->name, RPC_C_MEM_NSENTRY);
    RPC_MEM_FREE (*nsentry, RPC_C_MEM_NSENTRY);
    *nsentry = NULL;

    *status = rpc_s_ok;
    return;
}

/*
**++
**  ROUTINE NAME:     rpc__nsentry_from_entry
**
**  SCOPE:            PRIVATE - declared in nsentry.h
**
**  DESCRIPTION:
**
**  Creates a name service representation of an entry name
**  from a runtime representation of an entry name.
**
**  INPUTS:
**
**      name_syntax     The syntax of entry_name.
**
**      entry_name      Runtime representation of an entry name.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:
**
**      status          Returns on of
**                          rpc_s_ok 
**                          rpc_s_no_memory
**                          rpc_s_incomplete_name
**                      or a status from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     
**
**      Returns a pointer to a name service representation of the entry name.
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE rpc_nsentry_t *rpc__nsentry_from_entry 
#ifdef _DCE_PROTO_
(
    unsigned32              name_syntax,
    unsigned_char_p_t       entry_name,
    unsigned32              *status
)
#else
(name_syntax, entry_name, status)
unsigned32              name_syntax;
unsigned_char_p_t       entry_name;
unsigned32              *status;
#endif
{
    unsigned32          temp_status;
    rpc_nsentry_p_t     nsentry;


    CODING_ERROR (status);

    /*
     * Allocate the structure to hold the name service entry representation.
     */
    RPC_MEM_ALLOC (
        nsentry,
        rpc_nsentry_p_t,
        sizeof (rpc_nsentry_t),
        RPC_C_MEM_NSENTRY,
        RPC_C_MEM_NOWAIT);
    if (nsentry == NULL){
	*status = rpc_s_no_memory;
	return NULL;
    }


    /*
     * Allocate the structure to hold the name service entry name
     * representation.
     */
    RPC_MEM_ALLOC (
        nsentry->name,
        unsigned_char_p_t,
        RPC_C_ENTRY_NAME_MAX,
        RPC_C_MEM_NSENTRY, 
        RPC_C_MEM_NOWAIT);
    if (nsentry->name == NULL){
	RPC_MEM_FREE(nsentry, RPC_C_MEM_NSENTRY);
	*status = rpc_s_no_memory;
	return NULL;
    }

        
    /* 
     * Convert entry name to its nameservice representation.
     */
    rpc__ns_expand_entry_name (
        entry_name, nsentry->name, &(nsentry->name_length), status);

    if (*status != rpc_s_ok) 
    {
        /*
         * nsentry is nulled by nsentry_free routine.
         */
        rpc__nsentry_free (&nsentry, &temp_status);
    }


    return nsentry;
}


/*
**++
**  ROUTINE NAME:       rpc__nsentry_make_it_rpc
**
**  SCOPE:              PRIVATE - declared in nsentry.h
**
**  DESCRIPTION:
**
**  Adds the RPC class version to a name service entry.
**
**  INPUTS:
**
**      nsentry         The name service representation of an entry name.
**
**  INPUT/OUTPUTS:	none
**
**  OUTPUTS:
**
**      status		Returns status from rpc_nsattr_add_value routine.
**
**  IMPLICIT INPUTS:	none
**
**  IMPLICIT OUTPUTS:	none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:	none
**
**--
*/

PRIVATE void rpc__nsentry_make_it_rpc 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t         nsentry,
    unsigned32              *status
)
#else
(nsentry, status)
rpc_nsentry_p_t         nsentry;
unsigned32              *status;
#endif
{

    unsigned32      version_length;
    unsigned8       version_string[2], *version_string_p;
    
    CODING_ERROR (status);
    
    /*
     * Set the length of the version string ( 2 unsigned8's).
     */
     
    version_length = sizeof(rpc_ns_version_t); 
        
    /*
     * Copy the version major and minor values to the 
     * octet string.
     */

    version_string_p = version_string;
         
    memcpy (( char * ) version_string_p,
            ( char * )&rpc_ns_rpc_class_version.vers_major,
            sizeof(unsigned8));

    version_string_p++;
    
    memcpy (( char * ) version_string_p,
            ( char * )&rpc_ns_rpc_class_version.vers_minor,
            sizeof(unsigned8) );
            


    /* 
     * Add the RPC_ClassVersion attribute to the entry as
     * octet string. 
     */
     
    rpc__nsattr_add_value ( 
        nsentry,          
        RPC_C_ATTR_CLASS_VERSION,
        (unsigned_char_p_t) version_string,
        version_length,
        status);

    return;
}


/*
**++
**  ROUTINE NAME:       rpc__nsentry_to_entry
**
**  SCOPE:              PRIVATE - declared in nsentry.h
**
**  DESCRIPTION:
**
**  Creates a runtime representation of an entry name 
**  from a name service representation of an entry name.
**
**  INPUTS:
**
**      nsentry         Name service representation of an entry name.
**
**      entry_name_syntax The name service syntax to be used for
**                      returning the entry name.
**
**  INPUT/OUTPUTS:      none
**
**      entry_name      Returns the runtime representation of nsentry.
**                      If entry_name is NULL on input, just
**                      the entry_name length is returned.
**
**  OUTPUTS:
**
**      entry_name_len  Returns the length of the entry_name.
**                      This value is reflects the length even
**                      if the user did not want the entry_name
**                      returned.
**
**      status          The returned RPC status, one of:
**                          rpc_s_ok,
**                          rpc_s_incomplete_name
**                      or status from a called routine.
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

PRIVATE void rpc__nsentry_to_entry 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t         nsentry,
    unsigned32              entry_name_syntax,
    unsigned_char_t         *entry_name,
    unsigned32              *entry_name_length,
    unsigned32              *status
)
#else
(nsentry, entry_name_syntax, entry_name, entry_name_length, status)
rpc_nsentry_p_t         nsentry;
unsigned32              entry_name_syntax;
unsigned_char_t         *entry_name;
unsigned32              *entry_name_length;
unsigned32              *status;
#endif
{

    CODING_ERROR (status);

    *entry_name_length = strlen ((char *)nsentry->name);

    /*
     * Proceed only if caller wants the entry_name returned.
     */
    if (entry_name != NULL)
    {
        strcpy ((char *)entry_name, (char *)nsentry->name);
    }

    *status = rpc_s_ok;

    return;
}
