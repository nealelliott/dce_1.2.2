/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsgroup.c,v $
 * Revision 1.1.324.2  1996/02/18  00:05:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:05  marty]
 *
 * Revision 1.1.324.1  1995/12/08  00:21:26  root
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
 * 	[1995/12/08  00:00:03  root]
 * 
 * Revision 1.1.322.2  1994/07/29  16:19:20  tom
 * 	Add entry type to rpc__nsentry_create arguments. (OT 7945)
 * 	[1994/07/28  22:34:08  tom]
 * 
 * Revision 1.1.322.1  1994/01/21  22:38:34  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:59  cbrooks]
 * 
 * Revision 1.1.2.9  1993/01/08  21:47:20  weisman
 * 	Resubmitting Grace Hsiao changes.
 * 	[1993/01/07  21:41:51  weisman]
 * 
 * Revision 1.1.2.5  1992/11/16  22:20:16  zeliff
 * 	Re-submitting Grace Hsiao's changes.
 * 	[1992/11/16  22:19:07  zeliff]
 * 
 * Revision 1.1.2.3  1992/11/05  16:37:12  grace
 * 	28-sep-92    gh   Remove rpc__ns_check_name_syntax() -- syntax is checked
 * 	                  in RPC_RESOLVE_*_SYNTAX macro.
 * 	23-sep-92    gh   Bug fix in rpc_ns_group_mbr_inq_begin()
 * 	                  - return correct status code if member name syntax is
 * 	                    an unsupported one
 * 	                  - only copy new member name syntax onto inquiry context if
 * 	                    the inquiry context is not NULL.
 * 	[1992/11/05  15:25:52  grace]
 * 
 * Revision 1.1.2.2  1992/10/13  20:56:15  weisman
 * 	    08-jul-92    gh      Fix memory leaks.
 * 	[1992/10/13  20:48:14  weisman]
 * 
 * Revision 1.1  1992/01/19  03:11:17  devrcs
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
**  NAME:
**
**    NSGROUP.C
**
**  FACILITY:
**
**    Remote procedure call (RPC) name service independent interface (NS)
**  
**  ABSTRACT:
**
**    RPC Naming set of PUBLIC routines to operate on groups.
**    The group operations apply only to name services that 
**    support named entries.  Each operation processes a group
**    for a name service supported by the DEC nameservice (DECdns).
**
**
**/

#include <commonp.h>
#include <com.h> 
#include <ns.h>              /* Private NS defs for other RPC components */
#include <nsp.h>             /* Private defs for Naming Service component */
#include <nsentry.h>         /* Private routines for ns entries */
#include <nsattr.h>          /* Private routines for ns attributes */
#include <nsutil.h>          /* Private name service specific routines */

/*
**++
**  ROUTINE NAME:       rpc_ns_group_mbr_add 
**
**  SCOPE:              PUBLIC - declared rpc.idl
**
**  DESCRIPTION:        
**
**  Adds a member to a group entry.  If the group entry does not exist,
**  a new one is created.
**
**  INPUTS:             
**
**      group_name_syntax   The name syntax of the group name
**
**      group_name          The human-readable form of the group name
**
**      member_name_syntax  The name syntax of the member name
**
**      member_name         The human-readable form of the member name
**
**  INPUT/OUPUTS:       none                      
**
**  OUTPUTS:            
**
**      status          Returns a status from the add-member operation.
**                      One of:
**                          rpc_s_ok,
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

PUBLIC void rpc_ns_group_mbr_add 
#ifdef _DCE_PROTO_
(
    unsigned32              group_name_syntax,
    unsigned_char_p_t       group_name,
    unsigned32              member_name_syntax,
    unsigned_char_p_t       member_name,
    unsigned32              *status
)
#else
(group_name_syntax, group_name, member_name_syntax, member_name, status )
unsigned32              group_name_syntax;
unsigned_char_p_t       group_name;
unsigned32              member_name_syntax;
unsigned_char_p_t       member_name;
unsigned32              *status;
#endif
{
    rpc_nsentry_p_t         nsgroup_name;
    rpc_nsentry_p_t         nsmember_name;
    unsigned32              internal_status;
    unsigned32              new_group_name_syntax;
    unsigned32              new_member_name_syntax;
    

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * Copy the given syntax into local variable, because
     * we might have to modify it.  Resolve the syntax
     * using defaults if required and convert to internal form.
     */
    new_group_name_syntax = group_name_syntax;
    RPC_RESOLVE_SYNTAX (new_group_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Create a name-service representation of group_name.
     */
    nsgroup_name = rpc__nsentry_from_entry
        (new_group_name_syntax, group_name, status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Resolve the member name syntax.
     */
    new_member_name_syntax = member_name_syntax;
    RPC_RESOLVE_SYNTAX (new_member_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        rpc__nsentry_free (&nsgroup_name, &internal_status);
        return;
    }
    
    /*
     * Create a name-service representation of the member name.
     */
    nsmember_name = rpc__nsentry_from_entry
        (new_member_name_syntax, member_name, status);

    if (*status != rpc_s_ok)
    {
    	rpc__nsentry_free (&nsgroup_name, &internal_status);
        return;
    }

    /* 
     * Compare the runtime class version with the name servie
     * entry's class version. 
     */
    rpc__ns_check_rpc_class_version (nsgroup_name, RPC_C_NS_BYPASS_CACHE,
                                 status);
                                 
    switch ((int)*status)
    {
        case rpc_s_entry_not_found:
	    /*
     	     * Entry does not exist.  Create the entry and add 
      	     * the member.
	     */
     	    rpc__nsentry_create (nsgroup_name, RPC_C_NS_GROUP, status);
     	    if (*status != rpc_s_ok)
     	    {
        	break;
            }
	
     	    rpc__nsattr_add_value (
     		nsgroup_name, RPC_C_ATTR_GROUP, 
     		(unsigned_char_p_t) nsmember_name->name, 
     		nsmember_name->name_length, status);
		
	    break;

        case rpc_s_ok:
            /*
             * The entry exists and has a compatible RPC Class Version.
             * Add the member.
             */               
            rpc__nsattr_add_value (
                nsgroup_name, RPC_C_ATTR_GROUP, 
                (unsigned_char_p_t) nsmember_name->name, 
                nsmember_name->name_length, status);

            break;
           
        case rpc_s_not_rpc_entry:
            /*
             * The RPC Class Version attribute was not on the entry.
             * Add the value and mark this as an RPC entry.
             */
            rpc__nsattr_add_value (
                nsgroup_name, RPC_C_ATTR_GROUP, 
                (unsigned_char_p_t) nsmember_name->name, 
                nsmember_name->name_length, status);
 
            /* 
             * Entry exists, but we did not create it.
             * Label the fact that it is an RPC entry.
             */
            rpc__nsentry_make_it_rpc (nsgroup_name, status);

	default:
	    break;
            
    }     

    /*
     * Free the name service representations of
     * the group and member names.
     */
    rpc__nsentry_free (&nsmember_name, &internal_status);

    if (internal_status != rpc_s_ok)
    {
	*status = internal_status;
    }

    rpc__nsentry_free (&nsgroup_name, &internal_status);

    if (internal_status != rpc_s_ok)
    {
        *status = internal_status;
    }

    return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_group_delete 
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  Delete a group and its members.
**
**  INPUTS:             
**
**      group_name_syntax   The name syntax of the group name
**
**      group_name          The human-readable form of the group name
**
**  INPUT/OUPUTS:       none
**
**  OUTPUTS:            
**
**      status          The status of the group delete operation.
**                      Can be one of:
**                          rpc_s_ok
**                      or a status returned from a called routine.
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

PUBLIC void rpc_ns_group_delete 
#ifdef _DCE_PROTO_
(
    unsigned32              group_name_syntax,
    unsigned_char_p_t       group_name,
    unsigned32              *status
)
#else
(group_name_syntax, group_name, status)
unsigned32              group_name_syntax;
unsigned_char_p_t       group_name;
unsigned32              *status;
#endif
{
    rpc_nsentry_p_t         nsgroup_name;
    unsigned32              internal_status;
    unsigned32              new_group_name_syntax;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * Copy the given group syntax into a local variable, because
     * we might have to modify it.  Resolve the given syntax
     * using defaults if required and convert to internal form.
     */
     new_group_name_syntax = group_name_syntax;
     RPC_RESOLVE_SYNTAX (new_group_name_syntax, status);
     if (*status != rpc_s_ok)
     {
         return;
     }
				   
    /*
     * Create a name-service representation of group_name.
     */
    nsgroup_name = rpc__nsentry_from_entry
        (new_group_name_syntax, group_name, status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Remove the group attribute if it exists.
     */
    rpc__nsattr_delete (nsgroup_name, RPC_C_ATTR_GROUP, status);

    /*
     * Free the name-service representation of the group name.
     */
    rpc__nsentry_free (&nsgroup_name, &internal_status);

    if (internal_status != rpc_s_ok)
    {
        *status = internal_status;
    }

    return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_group_mbr_inq_begin 
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  Begin an inquiry context, for the purpose of returning group
**  members. Returns an inquiry context pointer which is to be specified as
**  an input to rpc_ns_group_inq_member.
**
**  INPUTS:             
**
**      group_name_syntax   The name syntax of the group name
**
**      group_name          The human-readable form of the group name
**
**      member_name_syntax  The syntax to be used in returning member names
**                          from rpc_ns_group_mbr_inq_next
**
**  INPUT/OUPUTS:       none
**
**  OUTPUTS:                
**
**      inquiry_context A opaque pointer to the inquiry context data rep.
**
**      status          Returns a status from the begin-inquiry operation.
**                      One of:
**                          rpc_s_ok,
**                          rpc_s_group_not_found,
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

PUBLIC void rpc_ns_group_mbr_inq_begin 
#ifdef _DCE_PROTO_
(
    unsigned32              group_name_syntax,
    unsigned_char_p_t       group_name,
    unsigned32              member_name_syntax,
    rpc_ns_handle_t         *inquiry_context,
    unsigned32              *status
)
#else
(group_name_syntax, group_name, member_name_syntax, inquiry_context, status)
unsigned32              group_name_syntax;
unsigned_char_p_t       group_name;
unsigned32              member_name_syntax;
rpc_ns_handle_t         *inquiry_context;
unsigned32              *status;
#endif
{
    rpc_nsentry_p_t         nsgroup_name;
    rpc_ns_inq_rep_p_t      inq = NULL;
    unsigned32              internal_status;
    unsigned32              new_group_name_syntax;
    unsigned32              new_member_name_syntax;

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * Copy the given entry syntax into a local variable, because
     * we might have to modify it.  Resolve the given syntax
     * using defaults if required and convert to internal form.
     */
	
    new_group_name_syntax = group_name_syntax;
    RPC_RESOLVE_SYNTAX (new_group_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Create a name-service representation of group_name.
     */
    nsgroup_name = rpc__nsentry_from_entry
        (new_group_name_syntax, group_name, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Resolve the member name syntax
     */
    new_member_name_syntax = member_name_syntax;
    RPC_RESOLVE_SYNTAX (new_member_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        rpc__nsentry_free (&nsgroup_name, &internal_status);        
        return;
    }
        
    /*
     * Create the inquire context and set the 
     * member name syntax field within.
     */
    inq = rpc__inqcntx_create
            (nsgroup_name, rpc_e_group_member, status);

    /*
     * Free the allocated nsentry if no inquiry context created.
     */
    if (inq == (rpc_ns_inq_rep_p_t) NULL)
    {
        rpc__nsentry_free (&nsgroup_name, &internal_status);

        if (internal_status != rpc_s_ok)
        {
            *status = internal_status;
        }
    }
   
    /*
     * copy member name syntax field
     */
    else
    {
        inq->member_name_syntax = new_member_name_syntax;
    }

    /*
     * return a handle to the inquiry context
     */
    *inquiry_context = (rpc_ns_handle_t) inq;
    return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_group_mbr_inq_done 
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  End an inquiry context which was established by an earlier call
**  to rpc_ns_group_mbr_inq_begin. 
**
**  INPUTS:             
**
**  INPUT/OUPUTS:       
**
**      inquiry_context     On call, it is an opaque pointer to an inquiry
**                          context structure. Returns a NULL pointer on
**                          successful return.
**
**  OUTPUTS:            
**
**      status          The status of the group done operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
**                      or status from the rpc__inqcntx_free routine.
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

PUBLIC void rpc_ns_group_mbr_inq_done 
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

    inq = (rpc_ns_inq_rep_p_t) *inquiry_context;

    /*
     * check to see if this is a valid inquiry context for this operation
     */
    if ((inq == (rpc_ns_inq_rep_p_t) NULL) ||
        (inq->usage != rpc_e_group_member))
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }
    
    rpc__inqcntx_free (&inq, status);
    *inquiry_context = (rpc_ns_handle_t) inq;

    /*
     * return with the results of rpc__inqcntx_free
     */
    return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_group_mbr_inq_next
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  Return a member from a group.
**
**  INPUTS:             
**
**      inquiry_context     An opaque pointer to an inquiry context data rep.
**
**  INPUT/OUPUTS:       
**
**      member_name     The human-readable form of the returned member name.
**                      NULL on input if user does not want it to be returned.
**
**  OUTPUTS:            
**
**
**      status          The status of the inquire-member operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
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

PUBLIC void rpc_ns_group_mbr_inq_next 
#ifdef _DCE_PROTO_
(
    rpc_ns_handle_t         inquiry_context,
    unsigned_char_p_t       *member_name,
    unsigned32              *status
)
#else
(inquiry_context, member_name, status )
rpc_ns_handle_t         inquiry_context;
unsigned_char_p_t       *member_name;
unsigned32              *status;
#endif
{
    unsigned32              nsattr_value_len = 0;
    unsigned32              member_name_length = 0;
    rpc_ns_inq_rep_p_t      inq;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    inq = (rpc_ns_inq_rep_p_t) inquiry_context;

    /* 
     * Initialize return values in the case of an error.
     * Note, member_name is a modify argument, so NULL only 
     * if caller wants it returned.
     */
    if (member_name != NULL)
    {
        *member_name = NULL;
    }

    /*
     * check to see if this is a valid inquiry context for this operation
     */
    if ((inq == (rpc_ns_inq_rep_p_t) NULL) ||
        (inq->usage != rpc_e_group_member))
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }


    /*
     * Read a member of the group.
     */
    rpc__nsattr_read_value (
        inq, 
        (unsigned_char_p_t *)
        &(inq->inq_specific.inqcntx_group.nsmember_name.name),
        &nsattr_value_len, 
        status);

    if ((*status == rpc_s_ok) && (member_name != NULL))
    {
        /*
         * heap allocate storage for the member name to return
         * and copy the one obtained into it for the caller
         */
        RPC_MEM_ALLOC (
            *member_name,
            unsigned_char_p_t,
            RPC_C_ENTRY_NAME_MAX,
            RPC_C_MEM_STRING,
            RPC_C_MEM_WAITOK);
	if (*member_name == NULL){
	    *status = rpc_s_no_memory;
	    return;
	}

        /*
         * Convert the member name to runtime format using the member
         * name syntax stored in the inquiry context
         */
        rpc__nsentry_to_entry (
            &(inq->inq_specific.inqcntx_group.nsmember_name),
            inq->member_name_syntax,
            *member_name,
            &member_name_length,
            status);

    }
    /*
     * return with the status from rpc__nsentry_to_entry
     * (or some previous operation if one failed)
     */
    return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_group_mbr_remove
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  Remove an entry name (member) from a group.
**
**  INPUTS:             
**
**      group_name_syntax   The name syntax of the group name.
**
**      group_name          The group name.
**
**      member_name_syntax  The name syntax of the member name
**
**      member_name         The name of the member being removed.
**
**  INPUT/OUPUTS:       none
**
**  OUTPUTS:            
**
**      status          The status of the group done operation.
**                      Can be one of:
**                          rpc_s_ok
**                          rpc_s_group_member_not_found
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

PUBLIC void rpc_ns_group_mbr_remove 
#ifdef _DCE_PROTO_
(
    unsigned32              group_name_syntax,
    unsigned_char_p_t       group_name,
    unsigned32              member_name_syntax,
    unsigned_char_p_t       member_name,
    unsigned32              *status
)
#else
(group_name_syntax, group_name, member_name_syntax, member_name, status)
unsigned32              group_name_syntax;
unsigned_char_p_t       group_name;
unsigned32              member_name_syntax;
unsigned_char_p_t       member_name;
unsigned32              *status;
#endif
{
    rpc_nsentry_p_t         nsgroup_name;
    rpc_nsentry_p_t         nsmember_name;
    unsigned32              internal_status;
    unsigned32              new_group_name_syntax;
    unsigned32              new_member_name_syntax;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * Copy the given syntax into a local variable, because
     * we might have to modify them.  Resolve the given syntax
     * using defaults if required and convert to internal form.
     */
     new_group_name_syntax = group_name_syntax;
     RPC_RESOLVE_SYNTAX (new_group_name_syntax, status);
     if (*status != rpc_s_ok)
     {
         return;
     }

    /*
     * Create a name-service representation of group_name.
     */
    nsgroup_name = rpc__nsentry_from_entry
        (new_group_name_syntax, group_name, status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Resolve the member name syntax.
     */
	      
    new_member_name_syntax = member_name_syntax;
    RPC_RESOLVE_SYNTAX (new_member_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        rpc__nsentry_free (&nsgroup_name, &internal_status);
        return;
    }

    /*
     * Create a name-service representation of the member name.
     */
    nsmember_name = rpc__nsentry_from_entry
        (new_member_name_syntax, member_name, status);

    if (*status != rpc_s_ok)
    {
        rpc__nsentry_free (&nsgroup_name, &internal_status);
        return;
    }
            
    /*
     * Remove the member name from the group attribute.
     */
    rpc__nsattr_remove_value (
        nsgroup_name, RPC_C_ATTR_GROUP, 
        (unsigned_char_p_t) nsmember_name->name, 
        nsmember_name->name_length, status);

    if (*status == rpc_s_not_found)
    {
        *status = rpc_s_group_member_not_found;
    }

    /*
     * Free the name-service representation of the member name.
     */
    rpc__nsentry_free (&nsmember_name, &internal_status);

    if (internal_status != rpc_s_ok)
    {
        *status = internal_status;
        rpc__nsentry_free (&nsgroup_name, &internal_status);
        return;
    }

    /*
     * Free the name-service representation of the group name.
     */
    rpc__nsentry_free (&nsgroup_name, &internal_status);

    if (internal_status != rpc_s_ok)
    {
        *status = internal_status;
    }

    return;
}
