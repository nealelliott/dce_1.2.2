/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsmgmt.c,v $
 * Revision 1.1.528.2  1996/02/18  00:05:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:11  marty]
 *
 * Revision 1.1.528.1  1995/12/08  00:21:33  root
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
 * 	[1995/12/08  00:00:08  root]
 * 
 * Revision 1.1.526.2  1994/07/29  16:19:22  tom
 * 	Add entry type to rpc__nsentry_create arguments. (OT 7945)
 * 	[1994/07/28  22:34:17  tom]
 * 
 * Revision 1.1.526.1  1994/01/21  22:38:41  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:19  cbrooks]
 * 
 * Revision 1.1.4.6  1993/02/05  16:11:43  raizen
 * 	 26-jan-93 dr      Remove rpc_ns_mgmt_set_authn
 * 	[1993/02/04  23:20:49  raizen]
 * 
 * Revision 1.1.4.5  1993/01/03  23:53:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:10:41  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/23  21:14:03  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:42:26  zeliff]
 * 
 * Revision 1.1.4.3  1992/12/21  19:20:46  wei_hu
 * 	    09-dec-92   raizen  add rpc_ns_mgmt_set_authn  [OT4854]
 * 	    16-sep-92   tw/vj   fix incorrect reference to elements in the
 * 	                        object_uuid_vec in rpc_ns_mgmt_binding_unexport
 * 	    03-aug-92    gh      Fix memory leaks.
 * 	[1992/12/18  20:27:57  wei_hu]
 * 
 * Revision 1.1.4.2  1992/10/13  20:56:49  weisman
 * 	    03-aug-92    gh      Fix memory leaks.
 * 	[1992/10/13  20:48:35  weisman]
 * 
 * Revision 1.1.2.2  1992/05/01  16:38:56  rsalz
 * 	 3-mar-92    wh      DCE 1.0.1 merge.
 * 	07-feb-92    ko      RPC_VERIFY_INIT -> RPC_NS_VERIFY_INIT again.
 * 	[1992/05/01  16:30:32  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:12:06  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**     NSMGMT.C
**
**  FACILITY:
**
**     Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**      The purpose of this module is to isolate those routines containing 
**      name service management routines from the other name service routines.
**
**
*/

#include <commonp.h>    /* Common internals for RPC Runtime system      */
#include <com.h>        /* Externals for Common Services component      */
#include <ns.h>         /* Externals for Name Services component        */
#include <nsp.h>        /* Internals for Name Services component        */
#include <comtwr.h>     /* Externals for Towers sub-component           */
#include <comtwrref.h>  /* Externals for Tower ref defs sub-components  */
#include <comtwrflr.h>  /* tower floor defs for other RPC               */
#include <nstower.h>    /* Externals for NS Towers sub-component        */
#include <nsentry.h>    /* Externals for NS Entry sub-component         */
#include <nsuuid.h>     /* Externals for NS UUID sub-component          */
#include <nsutil.h>     /* Externals for NS specific routines           */

INTERNAL void unexport_towers _DCE_PROTOTYPE_ ((
        rpc_nsentry_p_t      /*nsentry*/,
        rpc_if_id_p_t        /*if_id*/, 
        unsigned32           /*vers_option*/,
        rpc_ns_handle_t      /*inquiry_context*/,
        unsigned32          * /*status*/
     ));


/*
**++
**  ROUTINE NAME:       rpc_ns_mgmt_entry_create
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**  Creates an RPC name-service entry in the namespace.
**
**  INPUTS:
**
**      entry_name_syntax   An integer value that specifies the syntax
**                          of the argument entry_name.  A value of NULL
**                          results in the default entry syntax being used.
**
**      entry_name          Runtime representation of an entry name to create.
**
**  INPUT/OUPUTS:       none
**
**  OUTPUTS:          
**
**      status          Returns the status of the private routines called.
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

PUBLIC void rpc_ns_mgmt_entry_create 
#ifdef _DCE_PROTO_
(
    unsigned32              entry_name_syntax,
    unsigned_char_p_t       entry_name,
    unsigned32              *status
)
#else
(entry_name_syntax, entry_name, status)
unsigned32              entry_name_syntax;
unsigned_char_p_t       entry_name;
unsigned32              *status;
#endif
{
    rpc_nsentry_p_t         nsentry;
    unsigned32              new_entry_name_syntax;
    unsigned32              temp_status;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * Resolve the name syntax.
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
     * Call the private routine to create the entry
     * and return status to the caller.
     */
    rpc__nsentry_create (nsentry, RPC_C_NS_ENTRY, status);

    /*
     * Free the name-service entry representation.
     * Return the status from the rpc__nsentry_create routine, 
     * or if failed, the rpc__nsentry_free
     */
    rpc__nsentry_free (&nsentry, &temp_status);
    if (temp_status != rpc_s_ok)
    {
        *status = temp_status;
    }
    return;
  


}

/*
**++
**  ROUTINE NAME:      rpc_ns_mgmt_entry_delete
**
**  SCOPE:             PUBLIC - declared in RPC.IDL
**
**  DESCRIPTION:
**
**  Delete a RPC entry from the namespace.
**
**  INPUTS:
**
**      entry_name_syntax   Indicates the syntax of the entry_name
**                          argument.
**
**      entry_name          Full name of the entry.
**
**  INPUT/OUPUTS:       none
**
**  OUTPUTS:
**
**      status          The return status is in *status.   
**                      Possible return values:
**                          rpc_s_ok
**                          rpc_s_not_found
**                      or status from a subsequent called routine.
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

PUBLIC void rpc_ns_mgmt_entry_delete 
#ifdef _DCE_PROTO_
(
    unsigned32              entry_name_syntax,  
    unsigned_char_p_t       entry_name,        
    unsigned32              *status            
)
#else
(entry_name_syntax, entry_name, status)
unsigned32              entry_name_syntax;  
unsigned_char_p_t       entry_name;        
unsigned32              *status;            
#endif
{
    rpc_nsentry_p_t         nsentry;
    unsigned32              new_entry_name_syntax;
    int                     dns_status;
    struct dnsFlagStat      dns_flags;
    unsigned32              internal_status;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

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
     * Convert entry_name to name service representation.
     */
    nsentry = rpc__nsentry_from_entry (
                new_entry_name_syntax, entry_name, status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    if (rpc__nsentry_exists (nsentry, status))
    {
       /*
         * Initialize DNS flags
         * Set the cache expiration age to RPC_C_NS_BYPASS_CACHE to
         * bypass the cache and as a consequence also update the
         * cache.
         */

        rpc__ns_init_dns_flags (&dns_flags, RPC_C_NS_BYPASS_CACHE);

        /* 
         * Delete the RPC object 
         */
        dns_status = dnsDelObj ((dns_full_name_t *) nsentry->name, &dns_flags);

        if (dns_status == DNS_SUCCESS )
        {
            *status = rpc_s_ok;
        }
        else
        {
            /*
             * Map dns error to rpc status
             */
            rpc__ns_status_from_nserror (&dns_flags, status);
        }
    }

    /* 
     * Free the nsentry structure.
     */
    rpc__nsentry_free (&nsentry, &internal_status);

    if (internal_status != rpc_s_ok)
    {
        *status = internal_status;
    }

    /*
     * Status contains results of rpc__nsentry_exists or rpc__nsentry_free.
     */
    return;
}

/*
**++
**  ROUTINE NAME:      rpc_ns_mgmt_entry_inq_if_ids
**
**  SCOPE:             PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**  Returns a list of the interfaces exported to a name service entry.
**  This is a management routine and will always bypass the cache for
**  obtaining interface ids.
**
**  INPUTS:
**
**      entry_name_syntax   An integer value that specifies the syntax
**                          of the argument entry_name.
**
**      entry_name          Runtime representation of an entry name to create.
**
**  INPUT/OUPUTS:       none
**
**  OUTPUTS:          
**
**      if_id_vector    A vector of interface ids.
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_no_interfaces_exported
**                          rpc_s_no_memory
**                          rpc_s_coding_error
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

PUBLIC void rpc_ns_mgmt_entry_inq_if_ids 
#ifdef _DCE_PROTO_
(
    unsigned32              entry_name_syntax,
    unsigned_char_p_t       entry_name,
    rpc_if_id_vector_p_t    *if_id_vector,
    unsigned32              *status
)
#else
(entry_name_syntax, entry_name, if_id_vector, status)
unsigned32              entry_name_syntax;
unsigned_char_p_t       entry_name;
rpc_if_id_vector_p_t    *if_id_vector;
unsigned32              *status;
#endif
{
    unsigned32              new_entry_name_syntax;
    rpc_nsentry_p_t         nsentry;
    rpc_ns_handle_t	    inquiry_context;
    unsigned32              if_count = 0;
    rpc_if_id_t             if_id;
    rpc_list_t              if_id_list;
    twr_p_t                 tower;
    rpc_tower_ref_p_t       tower_ref;
    rpc_if_list_elt_p_t     list_element;
    unsigned32              i;
    unsigned32              temp_status;

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * start by presuming failure
     */
    *if_id_vector = NULL;
    
    /*
     * Resolve the name syntax.
     */
    new_entry_name_syntax = entry_name_syntax;
    RPC_RESOLVE_SYNTAX (new_entry_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * convert the entry name to a name service representation
     */
    nsentry = rpc__nsentry_from_entry
        (new_entry_name_syntax, entry_name, status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * set up to search for protocol tower attributes in this entry
     */
    rpc__tower_mbr_inq_begin (nsentry, &inquiry_context, status);

    if (*status != rpc_s_ok)
    {
        /*
         * if we can't search, try to free the structure allocated
         */
        rpc__nsentry_free (&nsentry, &temp_status);    

        return;
    }

    /*
     * Since this routine is a management operation, we want to
     * bypass the name service cache. Set the inquiry context
     * to specify a cache bypass.
     */
    rpc_ns_mgmt_handle_set_exp_age (inquiry_context, RPC_C_NS_BYPASS_CACHE, 
				    status);

    if (*status != rpc_s_ok)
    {
        /*
         * Undo the tower-begin call.
         */
        rpc__tower_mbr_inq_done (&inquiry_context, &temp_status);

        return;
    }

    /*
     * initialize the head of a list to hold the if id's
     */
    RPC_LIST_INIT (if_id_list);

    /*
     * then start accumulating if id's
     */
    for (;;)
    {
        rpc__tower_mbr_inq_next (inquiry_context, &tower, status);    

        /*
         * check to see if a tower was found
         */

        if (*status != rpc_s_ok)
        {
            /* 
             * The function rpc__tower_mbr_inq_next() will
             * return rpc_s_no_more_members under two
             * conditions: (1) when the attribute does
             * not exist which means no interfaces were exported
             * (2) the list of if_ids is exhausted. In the first
             * case, no if id's will have been found. Return
             * the appropriate error code.
             */
            if ((*status == rpc_s_no_more_members) &&
                (if_count == 0))
            {
                *status = rpc_s_no_interfaces_exported;
            }
            break;
        }
        else
        {
            /*
             * convert the canonical tower to a tower reference
             */
            rpc__tower_to_tower_ref (tower, &tower_ref, status);

            if (*status != rpc_s_ok)
            {
               /*
                * free the tower before break
                */
                rpc__tower_free (&tower, &temp_status);
                break;
            }

            /*
             * check to see if the interface id in this tower reference
             * is already in the if id list, and if not, add it
             */
            rpc__tower_flr_to_if_id (tower_ref->floor[0], &if_id, status);
            if (*status != rpc_s_ok)
            {
                /*
                 * free the tower and tower reference before break
                 */
                rpc__tower_free (&tower, &temp_status);
                rpc__tower_ref_free (&tower_ref, &temp_status);
                break;
            }

            if (! rpc__if_in_id_list (&if_id_list, &if_id, status))
            {
                /*
                 * if we added an if id to the list, count it
                 */
                if_count++;
            }
            
            if (*status != rpc_s_ok)
            {
                /*
                 * free the tower and tower reference before break
                 */
                rpc__tower_free (&tower, &temp_status);
                rpc__tower_ref_free (&tower_ref, &temp_status);
                break;
            }
            
            /*
             * free both tower and tower reference and then check 
             * the status afterwards
             */
            rpc__tower_free (&tower, status);
            rpc__tower_ref_free (&tower_ref, &temp_status);
            if (*status != rpc_s_ok)
            {
                break;
            }
            if (temp_status != rpc_s_ok)
            {
                *status = temp_status;
                break;
            }
        }
    }        

    /*
     * when the search is done, free the inquiry context
     */
    rpc__tower_mbr_inq_done (&inquiry_context, &temp_status);
        
    /*
     * then see what terminated the search
     */
    if (*status != rpc_s_no_more_members) 
    {
        /*
         * free if id list 
         */
        for (i = 0; i < if_count; i++)
        {
            RPC_LIST_REMOVE_HEAD (if_id_list, list_element, rpc_if_list_elt_p_t);
            rpc__list_element_free (&rpc_g_free_if_list, (pointer_t)list_element);
        }
        
        return;
    }

    /*
     * and finally, make sure the inquiry context was freed
     */
    if (temp_status != rpc_s_ok)
    {
        /*
         * free if id list 
         */
        for (i = 0; i < if_count; i++)
        {
            RPC_LIST_REMOVE_HEAD (if_id_list, list_element, rpc_if_list_elt_p_t);
            rpc__list_element_free (&rpc_g_free_if_list, (pointer_t)list_element);
        }
        
        *status = temp_status;
        return;
    }

    /*
     * now create a vector to hold the if id's
     */
    RPC_MEM_ALLOC (
        *if_id_vector,
        rpc_if_id_vector_p_t,
        (sizeof (if_count) + (if_count * sizeof (rpc_if_id_p_t))),
        RPC_C_MEM_IF_ID_VECTOR,
        RPC_C_MEM_WAITOK);
    if (*if_id_vector == NULL){
        /*
         * free if id list 
         */
        for (i = 0; i < if_count; i++)
        {
            RPC_LIST_REMOVE_HEAD (if_id_list, list_element, rpc_if_list_elt_p_t);
            rpc__list_element_free (&rpc_g_free_if_list, (pointer_t)list_element);
        }
        
	*status = rpc_s_no_memory;
	return;
    }
    memset(*if_id_vector, 0,
	   (sizeof (if_count) + (if_count * sizeof (rpc_if_id_p_t))));

    /*
     * set the count field in the vector
     */
    (*if_id_vector)->count = if_count;

    /*
     * stuff the array with if id's off the accumulated list
     * Note: this loop is designed to continue and free all of the list
     * elements even if we run out of memory and can't allocate space
     * to copy the if id.
     */
    for (i = 0; i < if_count; i++)
    {
        /*
         * get the next element off the if id list
         */
        RPC_LIST_REMOVE_HEAD (if_id_list, list_element, rpc_if_list_elt_p_t);
    
        /*
         * allocate memory to hold the if id
         */
        RPC_MEM_ALLOC (
            (*if_id_vector)->if_id[i],
            rpc_if_id_p_t,
            sizeof (rpc_if_id_t),
            RPC_C_MEM_IF_ID,
            RPC_C_MEM_WAITOK);
	if ((*if_id_vector)->if_id[i] == NULL){
	    rpc__list_element_free (&rpc_g_free_if_list, (pointer_t)list_element);
	    /*
	     * free if id list 
	     */
	    for (i = 0; i < if_count; i++)
	    {
	        RPC_LIST_REMOVE_HEAD (if_id_list, list_element, rpc_if_list_elt_p_t);
		if (list_element == NULL)
		    break;
	        rpc__list_element_free (&rpc_g_free_if_list, (pointer_t)list_element);
	    }

	    (*if_id_vector)->count = i;
	    rpc_if_id_vector_free(if_id_vector, status);
	    *status = rpc_s_no_memory;
	    return;
	}
            
	/*
	 * copy the if id in the list element into the next position
	 * in the allocated if id array
	 */
	*((*if_id_vector)->if_id[i]) = list_element->if_id;

        /*
         * free the list element
         */
        rpc__list_element_free (&rpc_g_free_if_list, (pointer_t)list_element);
    }

    /*
     * return with the address of the if id vector
     */
    *status = rpc_s_ok;
    return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_mgmt_binding_unexport
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
**  entry_name            a character string representation of the entry name,
**                      in the designated syntax. Protocol towers and object
**                      uuids are to be removed from entry_name.
**  if_id                 Interface identification. A NULL argument value
**                      indicates not to unexport any bindings (only object
**                      UUIDs are to be unexported).
**  vers_option           Specifies how this routine is to use the vers_major
**                      and vers_minor fields of the if_id argument (see
**                      rpc__if_id_compare()).
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
**                      rpc_s_nothing_to_unexport
**                      rpc_s_no_interfaces_exported
**                      rpc_s_not_all_objs_unexported
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

PUBLIC void rpc_ns_mgmt_binding_unexport 
#ifdef _DCE_PROTO_
(
    unsigned32              entry_name_syntax,
    unsigned_char_p_t       entry_name,
    rpc_if_id_p_t           if_id,
    unsigned32              vers_option,
    uuid_vector_p_t         object_uuid_vec,
    unsigned32              *status
)
#else
( entry_name_syntax, entry_name, if_id, vers_option, object_uuid_vec, status)
unsigned32              entry_name_syntax;
unsigned_char_p_t       entry_name;
rpc_if_id_p_t           if_id;
unsigned32              vers_option;
uuid_vector_p_t         object_uuid_vec;
unsigned32              *status;
#endif
{
    unsigned32          name_syntax;
    rpc_nsentry_t       *nsentry;
    unsigned32          temp_status;
    uuid_p_t            *uuid_p;
    rpc_nsuuid_t        *nsobj_uuid;
    unsigned32          uuid_count;
    unsigned32          remove_count;
    unsigned32          obj_count;
    rpc_ns_handle_t	inquiry_context;

    CODING_ERROR(status);

    RPC_NS_VERIFY_INIT ();

    switch ((int)vers_option)
    {
        case rpc_c_vers_all:
        case rpc_c_vers_compatible:
        case rpc_c_vers_exact:
        case rpc_c_vers_major_only:
        case rpc_c_vers_upto:
            break;
        default:
            *status = rpc_s_invalid_vers_option;
            return;
            break;		/* NOTREACHED */
    }

    /*
     * Set the object uuid count to the number of non-NIL, non-NULL object
     * uuids in the vector by calling this INTERNAL routine.
     */
    obj_count = rpc__ns_count_obj_uuids_in_vec (object_uuid_vec);

    if ((if_id == (rpc_if_id_t *) NULL) && (obj_count == 0))
    {
        *status = rpc_s_nothing_to_unexport;
        return;
    }

    /*
     * Obtain the default for the entry_name_syntax, if needed
     */
    name_syntax = entry_name_syntax;
    RPC_RESOLVE_SYNTAX (name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /* 
     * Create a name service representation of entry_name
     */
    nsentry = rpc__nsentry_from_entry ( 
        name_syntax,
        entry_name,
        status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Determine if entry_name exists
     */
    if (! (rpc__nsentry_exists (nsentry, status)))
    {
        rpc__nsentry_free (&nsentry, &temp_status);
        return;
    }

    if (if_id != (rpc_if_id_t *) NULL)
    {
        rpc__tower_mbr_inq_begin (nsentry, &inquiry_context, status);
        if (*status != rpc_s_ok)
        {
            if (*status == rpc_s_not_found)
            {
                *status = rpc_s_no_interfaces_exported;
            }
            rpc__nsentry_free (&nsentry, &temp_status); 
            return;
        }

        /*
         * Since this routine is a management operation,
         * we want to bypass the name service cache. Set 
         * the inquiry context to specify a cache bypass.
         */
        rpc_ns_mgmt_handle_set_exp_age (inquiry_context, RPC_C_NS_BYPASS_CACHE, status);
        
        if (*status != rpc_s_ok)
        {
            /*
             * Undo tower-begin call
             */
            rpc__tower_mbr_inq_done (&inquiry_context, &temp_status);
            
            return;
        }

        /* 
         * call local routine to unexport_towers
         */
        unexport_towers (nsentry, if_id, vers_option, inquiry_context, status);

        if (*status != rpc_s_ok) goto CLEANUP;
    }

    if (obj_count > 0)
    {
        /*
         * Initialize the number of uuid's that have been removed to zero.
         * Initialize local uuid pointer to the value of the first pointer in
         * the uuid vector's array of pointers.
         */
        remove_count = 0;
        uuid_p = &object_uuid_vec->uuid[0];

        /*
         * For each UUID in object_uuid_vec ...
         */
        for ( uuid_count = 0;
              uuid_count < object_uuid_vec->count; 
              uuid_count++, uuid_p++ )
        {

            /* 
             * ... if it's NULL pointer or NIL object uuid ...
             */
            if (*uuid_p == (uuid_p_t) NULL) 
            {

                continue;
            }
            /*
             * (do this in a separate test because we can't count on any
             * particular order of evaluation in a compound test)
             */
            if ((uuid_is_nil (*uuid_p, status)) == true)
            {
                continue;
            }

            /* 
             * ... else it's a non-NULL pointer and non-NIL object uuid so 
             * 1) Convert the object UUID to its name service representation.
             * 2) Use the nsuuid as input to the remove operation.
             */
            nsobj_uuid = rpc__nsuuid_from_uuid (*uuid_p, status);
            if (*status != rpc_s_ok) 
            {
                goto CLEANUP;
            }
    
            rpc__nsuuid_mbr_remove (nsentry, nsobj_uuid, status);

            switch ((int)*status)
            {
                case rpc_s_ok :
                    remove_count++;
                    break;
                case rpc_s_not_found :
                    break;
                default:
                    rpc__nsuuid_free (&nsobj_uuid, &temp_status);
                    /*
                     * Return status from rpc__nsuuid_mbr_remove
                     */
                    goto CLEANUP;
                    break;	/* NOTREACHED */
            }
    
            /*
             * Free the rpc_nsuuid_t structure.
             */
            rpc__nsuuid_free (&nsobj_uuid, status);
            if (*status != rpc_s_ok) goto CLEANUP;

        }

        if (obj_count - remove_count > 0)
        {
            *status = rpc_s_not_all_objs_unexported;
        }

    }

    CLEANUP:
    /*
     * Free the rpc_nsentry_t structure by free'ing the inquiry_context, using
     * 'temp_status' argument instead of 'status'.  Since  the inq_done 
     * routine calls rpc_inqcntx_free which frees the nsentry, we do this 
     * after unexporting the object uuids (if any).  Otherwise we won't have
     * an nsentry for rpc__nsuuid_mbr_remove
     */
    if (if_id != (rpc_if_id_t *) NULL)
    {
        rpc__tower_mbr_inq_done (&inquiry_context, &temp_status);
    }
    return;

}


/*
**++
**  ROUTINE NAME:      rpc_ns_mgmt_inq_exp_age
**
**  SCOPE:             PUBLIC - declared in RPC.IDL
**
**  DESCRIPTION:
**
**  Returns the global expiration age value in seconds.
**  
**  INPUTS:             none
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      expiration_age  Returns an integer value that specifies the
**                      current default expiration age, in seconds.  
**                      
**      status          An integer value specifying the results of the
**                      inquire expiration age operation.
**                      One of:
**                          rpc_s_ok
**
**  IMPLICIT INPUTS:    
**      
**      rpc_g_ns_exp_age 
**                      The RPC global variable holding the current
**                      default value.
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void rpc_ns_mgmt_inq_exp_age 
#ifdef _DCE_PROTO_
(
    unsigned32  *expiration_age,
    unsigned32  *status
)
#else
(expiration_age, status)
unsigned32  *expiration_age;
unsigned32  *status;
#endif
{

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();

    /*
     * Set expiration_age to the value
     * of the RPC global variable, rpc_g_ns_exp_age.
     */
    *expiration_age = rpc_g_ns_exp_age;

    /* 
     * Return a successful status.
     */
    *status = rpc_s_ok;
    return;
}


/*
**++
**  ROUTINE NAME:      rpc_ns_mgmt_set_exp_age
**
**  SCOPE:             PUBLIC - declared in RPC.IDL
**
**  DESCRIPTION:
**
**  Sets the global expiration age, in seconds. 
**  
**  The expiration age is the maximum time name service data can
**  exist locally (in the cache) before the name service data is
**  updated.
**
**  An expiration age of 0 causes a bypass of the local data (or cache).
**  As a consequence, the cache is also updated with the current value
**  of that attribute.
**  
**  INPUTS:             
**
**      expiration_age  An integer value specifying, in seconds, the
**                      global expiration age for the name service 
**                      cache.
**
**                      To reset the expiration age to an RPC-assigned
**                      random value between 8 and 12 hours, specify
**                      a value of rpc_c_ns_default_exp_age.
**                      
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      status          An integer value specifying the results of the
**                      set expiration age operation.
**                      One of:
**                          rpc_s_ok
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   rpc_g_ns_exp_age
**                      The RPC global variable holding the current
**                      default value.
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void rpc_ns_mgmt_set_exp_age 
#ifdef _DCE_PROTO_
(
    unsigned32  expiration_age,
    unsigned32  *status
)
#else
(expiration_age, status)
unsigned32  expiration_age;
unsigned32  *status;
#endif
{

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();

    RPC_MUTEX_LOCK (ns_lock);

    /*
     * If the value of expiration_age is the default value, 
     * set the global expiration age to a random time (in seconds).
     * Otherwise, set the global expiration age to the value specified in
     * expiration_age.
     */
    if (expiration_age == rpc_c_ns_default_exp_age)
    {
        rpc_g_ns_exp_age =  RPC_RANDOM_GET (
                                RPC_C_GLB_EXP_AGE_LOWER_LIMIT,
                                RPC_C_GLB_EXP_AGE_UPPER_LIMIT);
    }
    else
    {
        rpc_g_ns_exp_age = expiration_age;
    }
    
    RPC_MUTEX_UNLOCK (ns_lock); 

    /*
     * Return a successful status.
     */
     
    *status = rpc_s_ok;
    return;
}


/*
**++
**  ROUTINE NAME:      rpc_ns_mgmt_handle_set_exp_age
**
**  SCOPE:             PUBLIC - declared in RPC.IDL
**
**  DESCRIPTION:
**
**  Sets the expiration age, in seconds, of the name service data
**  available on the local host (otherwise known as the cache) into
**  the specified NS handle.
**  
**  When an NSI routine, using the specified NS handle, attempts
**  to read name service data, and the local copy of the data has reached
**  its expiration age, the local data is automatically updated with 
**  data from a name server.  Only data for the attribute requested is
**  updated and only when the attribute data is read.
**
**  Specifying an expiration age of 0 causes an update of the local data at
**  each read of the name service data using the specified NS handle.
**
**  INPUTS:             
**
**      ns_handle       Ns handle for which an expiration age is
**                      specified.  This is a NS handle that was
**                      returned to a user from a previous "begin" routine.
**
**      expiration_age  An integer value specifying, in seconds, the
**                      expiration age of name service local data read
**                      by all "next" routines using the specified
**                      ns_handle argument.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      status          An integer value specifying the results of the
**                      set NS handle expiration age operation.
**                      One of:
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
**
**  IMPLICIT INPUTS:    rpc_g_ns_exp_age
**                       The RPC global variable holding the current
**                       default value.
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

PUBLIC void rpc_ns_mgmt_handle_set_exp_age 
#ifdef _DCE_PROTO_
(
    rpc_ns_handle_t         ns_handle,
    unsigned32              expiration_age,
    unsigned32              *status
)
#else
(ns_handle, expiration_age, status)
rpc_ns_handle_t         ns_handle;
unsigned32              expiration_age;
unsigned32              *status;
#endif
{

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();

    /*
     * If ns_handle is a valid handle, set
     * the expiration age in ns_handle to the
     * value specified in expiration_age. If the
     * application specified to use the default
     * global value, set the exp_age to the
     * current global value.
     * Return an error if the handle is invalid.
     */
    if (ns_handle != NULL)
    {
        if (expiration_age == rpc_c_ns_default_exp_age)
        {
            ((rpc_ns_inq_rep_t *) ns_handle)->common.exp_age = 
                        rpc_g_ns_exp_age;
        }
        else
        {
            ((rpc_ns_inq_rep_t *) ns_handle)->common.exp_age = 
                                            expiration_age;            
        }
        *status = rpc_s_ok;
    }
    else
    {
        *status = rpc_s_invalid_ns_handle;
    }

    return;
}


/*
**++
**  ROUTINE NAME:       unexport_towers
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**
**  Does the real work of unexporting the towers. Called only by 
**  rpc_ns_binding_unexport
**
**  INPUTS:
**
**  nsentry                 a name service representation of an entry name
**  if_id                   the interface identifier to unexport
**  vers_option             the version option of the if_id to unexport
**
**  INPUT/OUTPUTS:          none
**
**  inquiry_context         the inquiry context returned by a call to 
**                          rpc__tower_mbr_inq_begin in the caller.
**
**  OUTPUTS:
**
**      status              The return status is in *status
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

INTERNAL void unexport_towers 
#ifdef _DCE_PROTO_
(
    
    rpc_nsentry_p_t         nsentry,
    rpc_if_id_p_t           if_id,
    unsigned32              vers_option,
    rpc_ns_handle_t		inquiry_context,
    unsigned32              *status
    
)
#else
(nsentry, if_id, vers_option, inquiry_context, status)
rpc_nsentry_p_t         nsentry;
rpc_if_id_p_t           if_id;
unsigned32              vers_option;
rpc_ns_handle_t		inquiry_context;
unsigned32              *status;
#endif
{
    twr_p_t             tower_p;
    rpc_tower_ref_t     *tower_ref_p;
    rpc_if_id_t         tower_if_id;
    boolean             remove;
    unsigned32          remove_count = 0;
    unsigned32          temp_status;

    /* 
     * For each tower in entry_name (nsentry) ...
     * repeat until rpc__tower_mbr_inq_next returns
     * rpc_s_no_more_members or fails
     */
    do
    {
        /*
         * Initialize tower_p and tower_ref_p to NULL so it can be
         * checked after loop. 
         */
        tower_p = NULL;
        tower_ref_p = NULL;

        /*
         * Get the next tower and convert it to a tower reference.
         */
        rpc__tower_mbr_inq_next (inquiry_context, &tower_p, status);
        if (*status != rpc_s_ok)
        {
            continue;
        }

        rpc__tower_to_tower_ref (tower_p, &tower_ref_p, status);
        if (*status != rpc_s_ok)
        {
            continue;
        }

        /*
         * Obtain the tower's if_id for comparision.
         */
        rpc__tower_flr_to_if_id (tower_ref_p->floor[0], &tower_if_id, status);
        if (*status != rpc_s_ok)
        {
            continue;
        }
        
        /* 
         * If the interface id in if_id matches the interface id 
         * in the returned tower, remove the tower.
         */
        remove = rpc__if_id_compare (if_id, &tower_if_id,
                    vers_option, status);
        if (*status != rpc_s_ok)
        {
            continue;
        }

        rpc__tower_ref_free (&tower_ref_p, &temp_status);

        if (remove == true)
        {
            rpc__tower_mbr_remove (nsentry, tower_p, status);
            if (*status == rpc_s_ok)
            {
                remove_count++;
            }
            /*
             * else, drop out at the bottom of this loop
             */
        }
        rpc__tower_free (&tower_p, &temp_status);
    } while (*status == rpc_s_ok);

    /*
     * Capitalize on the fact that tower_p and tower_ref_p are
     * NULLed at the top of the previous loop to do this
     * deallocation, if needed. 
     */
    if (tower_ref_p != NULL)
    {
        rpc__tower_ref_free (&tower_ref_p, &temp_status);
    }
    if (tower_p != NULL)
    {
        rpc__tower_free (&tower_p, &temp_status);
    }

    /*
     * This is the expected, normal return:
     */
    if (*status == rpc_s_no_more_members)
    {
        if (remove_count == 0)
        {
            *status = rpc_s_interface_not_found;
        }
        else
        {
            *status = rpc_s_ok;
        }
    }

    return;
} 
