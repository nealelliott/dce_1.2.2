/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nslookup.c,v $
 * Revision 1.1.533.2  1996/11/13  17:58:06  arvind
 * 	CHFts20355: purify strikes again.
 * 	[1996/10/16  23:22 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_3/1]
 *
 * Revision 1.1.533.1  1996/10/03  14:41:58  arvind
 * 	Quiet purify.
 * 	[1996/09/16  19:57 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.531.2  1996/02/18  00:05:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:09  marty]
 * 
 * Revision 1.1.531.1  1995/12/08  00:21:30  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:34 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/25  21:41 UTC  lmm
 * 	Merge allocation failure detection cleanup work
 * 
 * 	HP revision /main/HPDCE02/lmm_alloc_fail_clnup/1  1995/05/25  21:02 UTC  lmm
 * 	allocation failure detection cleanup
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/02  01:16 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/HPDCE02/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:20 UTC  lmm
 * 	add memory allocation failure detection
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/09  19:18 UTC  tatsu_s
 * 	Submitted rfc31.0: Single-threaded DG client and RPC_PREFERRED_PROTSEQ.
 * 
 * 	HP revision /main/tatsu_s.st_dg.b0/4  1994/12/08  15:26 UTC  tatsu_s
 * 	Added the cache size limit.
 * 
 * 	HP revision /main/tatsu_s.st_dg.b0/3  1994/12/07  14:34 UTC  tatsu_s
 * 	Fixed group/profile members.
 * 
 * 	HP revision /main/tatsu_s.st_dg.b0/2  1994/12/06  20:27 UTC  tatsu_s
 * 	Fixed the profile priority.
 * 
 * 	HP revision /main/tatsu_s.st_dg.b0/1  1994/12/05  18:53 UTC  tatsu_s
 * 	RPC_PREFERRED_PROTSEQ: Initial version.
 * 	[1995/12/08  00:00:06  root]
 * 
 * Revision 1.1.525.7  1994/07/28  19:07:38  ganni
 * 	remove the incorrect check of the status code after
 * 	the return from uuid_create_nil().
 * 	[1994/07/28  19:07:09  ganni]
 * 
 * Revision 1.1.525.6  1994/06/21  21:54:14  hopkins
 * 	More serviceability
 * 	[1994/06/08  21:33:28  hopkins]
 * 
 * Revision 1.1.525.5  1994/04/12  16:52:05  mori_m
 * 	CR 9701:  Second submission for RPC runtime I18N support.
 * 		  Fixed some pointer usage.
 * 	[1994/04/12  16:47:26  mori_m]
 * 
 * Revision 1.1.525.4  1994/03/17  23:06:50  tom
 * 	Big PAC compatability:
 * 	  Set the protocol version in the binding handle.
 * 	[1994/03/17  23:04:48  tom]
 * 
 * Revision 1.1.525.3  1994/02/10  21:42:11  mori_m
 * 	CR 9925  Changed free_func to cs_free_func.  This is a conflict with
 * 	         a typedef within usr/include/krb5/mvr/encode.h
 * 	[1994/02/10  21:41:55  mori_m]
 * 
 * Revision 1.1.525.2  1994/02/08  21:38:42  mori_m
 * 	CR 9701:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:38:29  mori_m]
 * 
 * Revision 1.1.525.1  1994/01/21  22:38:38  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:03  cbrooks]
 * 
 * Revision 1.1.4.11  1993/03/01  19:30:55  sommerfeld
 * 	[7351] bring back some deleted code due to botched merge.
 * 	[1993/02/25  18:26:33  sommerfeld]
 * 
 * 	[7351] flush confusing comment.
 * 	[1993/02/25  16:14:41  sommerfeld]
 * 
 * 	[OT7351] performance improvements; avoid multiple checks of
 * 	class_version attribute, and avoid making two passes over profile
 * 	when hunting for default.
 * 	[1993/02/25  01:02:19  sommerfeld]
 * 
 * Revision 1.1.4.10  1993/01/28  16:48:04  weisman
 * 	The previous fix to clear the caller's lookup_context was incorrect
 * 	and was not clearing the argument.
 * 	[1993/01/28  16:41:22  weisman]
 * 
 * Revision 1.1.4.9  1993/01/08  21:47:25  weisman
 * 	      23-nov-92   dm      In rpc_ns_binding_lookup_next(), remove
 * 	                          rpc__bindlkup_node_free() for
 * 	                          rpc_s_entry_not_found
 * 
 * 	      05-oct-92   dm      In rpc_ns_binding_lookup_next(), if the starting
 * 	                          entry doesn't exist, return rpc_s_entry_not_found.
 * 	[1993/01/07  21:49:47  weisman]
 * 
 * Revision 1.1.4.2  1992/10/13  20:56:34  weisman
 * 	      03-aug-92   gh      Fix memory leaks.
 * 	[1992/10/13  20:48:24  weisman]
 * 
 * Revision 1.1.2.4  1992/07/07  21:28:38  rsalz
 * 	      25-jun-92   wh      Set lookup_context parameter to NULL on
 * 	                          completion of  rpc_ns_binding_lookup_done.
 * 	[1992/07/07  21:24:51  rsalz]
 * 
 * Revision 1.1.2.3  1992/05/14  15:32:19  jim
 * 	Fix parameter to rpc_ns_profile_elt_inq_begin so that it will compile on RIOS
 * 	[1992/05/14  14:07:47  jim]
 * 
 * Revision 1.1.2.2  1992/05/12  12:49:21  wei_hu
 * 	 11-may-92   rs      fix memory leak in rpc_ns_binding_lookup_done
 * 	                     by freeing tower context, if it remains
 * 	[1992/05/12  12:47:42  wei_hu]
 * 
 * Revision 1.1  1992/01/19  03:11:56  devrcs
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
**      nslookup.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  RPC Name Service routines to create and manage contexts for binding
**  lookups.
**
**
*/

#include <commonp.h>    /* Common internals for RPC Runtime system      */
#include <com.h>        /* Externals for Common Services component      */
#include <ns.h>         /* Externals for Name Services component        */
#include <nsp.h>        /* Internals for Name Services component        */
#include <nsentry.h>    /* Externals for NS Entry sub-component         */
#include <nstower.h>    /* Externals for NS Towers sub-component        */
#include <comtwr.h>     /* Externals for Towers sub-component           */
#include <comtwrref.h>  /* Externals for Tower ref sub-components       */
#include <nsutil.h>
#include <cs_s.h>	/* Private defs for code set interoperability   */

/*
 * typedef's that are only used internally
 */
 

/*
 * prototype declarations for locally defined routines
 */
 
INTERNAL void rpc__bindlkup_node_create _DCE_PROTOTYPE_ ((
        rpc_nsentry_p_t          /*nsentry*/,
        rpc_lkup_rep_t          * /*lookup_context*/,
        unsigned32              * /*status*/        
    ));
    
INTERNAL void rpc__bindlkup_node_free _DCE_PROTOTYPE_ ((
        rpc_lkup_node_p_t       * /*lookup_node*/,
        unsigned32              * /*status*/
    ));
    
INTERNAL void rpc__bindlkup_node_expand_mbrs _DCE_PROTOTYPE_ ((
        rpc_lkup_rep_p_t         /*lookup_context*/,
        rpc_lkup_node_t         * /*lookup_node*/,
        unsigned32              * /*status*/
    ));

INTERNAL void rpc__bindlkup_node_get_bindings _DCE_PROTOTYPE_ ((
        rpc_lkup_rep_p_t         /*lookup_context*/,
        rpc_binding_vector_t    * /*binding_vector*/,
        unsigned32              * /*status*/
    ));
    
INTERNAL boolean rpc__bindlkup_obj_uuid_match _DCE_PROTOTYPE_ ((
        rpc_lkup_rep_t          * /*lookup_context*/,
        unsigned32              * /*status*/
    ));
    
INTERNAL void rpc__bindlkup_obj_uuid_any _DCE_PROTOTYPE_ ((
        rpc_lkup_rep_t          * /*lookup_context*/,
        unsigned32              * /*status*/
    ));

INTERNAL boolean rpc__bindlkup_node_is_cycle _DCE_PROTOTYPE_ ((
        rpc_lkup_rep_p_t         /*lookup_context*/,
        rpc_nsentry_p_t          /*nsentry*/,
        unsigned32              * /*status*/
    ));
    
INTERNAL void rpc__bindlkup_node_next_attr _DCE_PROTOTYPE_ ((
        rpc_lkup_rep_p_t         /*lookup_context*/,
        rpc_lkup_node_t         * /*lookup_node*/,
        unsigned32              * /*status*/
    ));
    
INTERNAL rpc_nsentry_t *rpc__bindlkup_node_pick_mbr _DCE_PROTOTYPE_ ((
        rpc_lkup_node_t         * /*lookup_node*/,
        unsigned32              * /*status*/
    ));

INTERNAL void rpc__bindlkup_node_process _DCE_PROTOTYPE_ ((
        rpc_lkup_rep_p_t         /*lookup_context*/,
        rpc_lkup_node_p_t        /*lookup_node*/,
        rpc_binding_vector_t    * /*binding_vector*/,
        unsigned32              * /*status*/
    ));

INTERNAL void sort_members _DCE_PROTOTYPE_ ((
        rpc_lkup_node_p_t        /*lookup_node*/
    ));

INTERNAL void rpc__next_priority_group_count _DCE_PROTOTYPE_ ((
        rpc_lkup_node_p_t        /*lookup_node*/
    ));

INTERNAL void rpc__i14y_compat_check _DCE_PROTOTYPE_ ((
	rpc_lkup_rep_p_t	lookup_rep,
	rpc_binding_vector_p_t	*binding_vector,
	error_status_t		*status
    ));

INTERNAL void rpc__i14y_compat_cleanup _DCE_PROTOTYPE_ ((
	rpc_lkup_rep_p_t	lookup_rep,
	error_status_t		*status
    ));



/*
**++
**
**  ROUTINE NAME:       rpc_ns_binding_lookup_begin
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**      
**  Creates a lookup context to be used in a search of the namespace
**  for a binding to a specified interface (and optionally, an object).
**
**  INPUTS:
**
**      entry_name_syntax   An integer value that specifies the syntax of
**                      the next argument, entry_name. To use the syntax
**                      specified in the RPC_DEFAULT_ENTRY_SYNTAX
**                      environment variable, provide a value of NULL.
**
**      entry_name      The entry in the namespace in which the search for
**                      compatible bindings is to commence. To use the entry
**                      name found in the RPC_DEFAULT_ENTRY environment
**                      variable, provide a NULL pointer or an empty string.
**
**      if_spec         A pointer to an interface specification describing
**                      the desired interface.  
**                      A NULL if_spec means the caller does not want
**                      the returned bindings checked for compatibility with 
**                      an exported interface.
**                      When NULL, the if_spec initialization in the lookup 
**                      context is bypassed.
**
**      object_uuid     An optional object uuid which would be used to
**                      further qualify the interface. For a non-nil uuid,
**                      only compatible bindings are returned from a server
**                      entry if the server has exported the specified object
**                      uuid. If the object uuid's are not of concern to the
**                      caller, specify a NULL pointer value or a nil uuid.
**                      In this case the compatible bindings returned from
**                      a server entry contain a nil object uuid.
**                      
**      binding_max_cnt A user-specified limit on the size of the binding
**                      vector to be allocated for returning bindings. The
**                      size of the vector is specified in units of "bindings".
**                      What this translates into in terms of actual memory
**                      is not (can not and should not) be known to the user.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      lookup_context  A pointer to a lookup context to be used in the search.
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_no_memory
**                          rpc_s_invalid_object
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
**/

PUBLIC void rpc_ns_binding_lookup_begin 
#ifdef _DCE_PROTO_
(
    unsigned32              entry_name_syntax,
    unsigned_char_p_t       entry_name,
    rpc_if_handle_t         if_spec,
    uuid_p_t                object_uuid,
    unsigned32              binding_max_cnt,
    rpc_ns_handle_t         *lookup_context,
    unsigned32              *status
)
#else
(entry_name_syntax, entry_name, if_spec, object_uuid, binding_max_cnt, lookup_context, status)
unsigned32              entry_name_syntax;
unsigned_char_p_t       entry_name;
rpc_if_handle_t         if_spec;
uuid_p_t                object_uuid;
unsigned32              binding_max_cnt;
rpc_ns_handle_t         *lookup_context;
unsigned32              *status;
#endif
{
    unsigned32              temp_entry_name_syntax;
    unsigned_char_p_t       temp_entry_name;
    rpc_nsentry_p_t         nsentry = NULL;
    rpc_lkup_rep_p_t        lookup_rep = NULL;
    unsigned32              i;
    unsigned32              temp_status;
    unsigned32              uuid_status;
        

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();
    
    /*
     * copy the given entry name and syntax into local variables, because
     * we might have to modify them - then resolve the given name and syntax
     * to real ones (using defaults if required) and convert to internal form
     */
    temp_entry_name_syntax = entry_name_syntax;
    temp_entry_name = entry_name;
    RPC_RESOLVE_NAME_AND_SYNTAX (temp_entry_name, temp_entry_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    nsentry = rpc__nsentry_from_entry
        (temp_entry_name_syntax, temp_entry_name, status);
    /*
     * if anything went wrong, return with status of rpc__nsentry_from_entry
     */
    if (*status != rpc_s_ok) goto CLEANUP;

    /*        
     * create a lookup rep data structure to use for this context
     */
    RPC_MEM_ALLOC (
        lookup_rep,
        rpc_lkup_rep_p_t,
        sizeof (rpc_lkup_rep_t),
        RPC_C_MEM_LKUP_REP,
        RPC_C_MEM_WAITOK);
    if (lookup_rep == NULL){
	*status = rpc_s_no_memory;
	goto CLEANUP;
    }

    /*
     * initialize the lookup rep (including the if_spec, in case we
     * fail before using it)
     */
    lookup_rep->first_entry_flag = true;
    lookup_rep->if_spec = NULL;
    lookup_rep->eval_routines = NULL;
    
    /*
     * Use the global expiration age for determining
     * whether the name service cache needs to be refreshed.
     */
    lookup_rep->common.exp_age = rpc_g_ns_exp_age;
    
    /*
     * init the node list and the non-leaf list
     */
    RPC_LIST_INIT (lookup_rep->node_list);
    RPC_LIST_INIT (lookup_rep->non_leaf_list);

    /*
     * create a lookup node for this nsentry (and save the status code)
     */
    rpc__bindlkup_node_create (nsentry, lookup_rep, status);

    /*
     * if we were unable to create a lookup node,
     * return with the status from rpc__bindlkup_node_create
     */
    if (*status != rpc_s_ok)
    {
        goto CLEANUP;
    }
        
    /*
     * create an interface spec rep and copy the given 
     * one into it if one is specified.
     */
    if (if_spec != NULL)
    {
        RPC_MEM_ALLOC (
            lookup_rep->if_spec,
            rpc_if_rep_p_t,
            sizeof (rpc_if_rep_t),
            RPC_C_MEM_IF_REP,
            RPC_C_MEM_WAITOK);
	if (lookup_rep->if_spec == NULL){
	    *status = rpc_s_no_memory;
	    goto CLEANUP;
	}

        *(lookup_rep->if_spec) = *((rpc_if_rep_p_t) if_spec);

        if (lookup_rep->if_spec->syntax_vector.count > 0)
        {
            /*
             * init the syntax id vector in case we fail before using it
             */
            lookup_rep->if_spec->syntax_vector.syntax_id = NULL;

            /*
             * then create a syntax vector and copy that too
             */
            RPC_MEM_ALLOC (
                lookup_rep->if_spec->syntax_vector.syntax_id,
                rpc_syntax_id_p_t,
                lookup_rep->if_spec->syntax_vector.count * sizeof (rpc_syntax_id_t),
                RPC_C_MEM_SYNTAX_ID,
                RPC_C_MEM_WAITOK);
	    if (lookup_rep->if_spec->syntax_vector.syntax_id == NULL){
		*status = rpc_s_no_memory;
		goto CLEANUP;
	    }

            for (i = 0; i < lookup_rep->if_spec->syntax_vector.count; i++)
            {
                lookup_rep->if_spec->syntax_vector.syntax_id[i] =
                    ((rpc_if_rep_p_t) if_spec)->syntax_vector.syntax_id[i];
            }            
        }
    }

    /*
     * see if we were given a real object uuid
     */
    uuid_status = uuid_s_ok;
    
    if (object_uuid == NULL || uuid_is_nil (object_uuid, &uuid_status))
    {
        /*
         * if not, set the search field in the lookup context to
         * indicate that we're not searching based on object uuid
         * We'll take any object UUID when the time comes.
         */
        lookup_rep->obj_uuid_search = rpc_e_obj_uuid_search_any;
    }
    else
    {
        /*
         * we got a REAL object uuid - make sure it's a valid one
         */
        if (uuid_status != uuid_s_ok)
        {
            *status = rpc_s_invalid_object;
            goto CLEANUP;
        }

        /* set search for match and save the object uuid in the
         * lookup context to be used later in filtering compatible bindings
         */
        lookup_rep->obj_uuid_search = rpc_e_obj_uuid_search_match;
        lookup_rep->obj_uuid = *object_uuid;
    }        

    /*
     * set the inquiry context to NULL
     */
    lookup_rep->inq_cntx = NULL;

    /*
     * if no max was given for the # of bindings to return,
     * set the max vector size to the global default, otherwise
     * set it to the specified value
     *
     * Note: This is for pre-DCE V1.0 compatibility. The documentation
     *       says to use rpc_c_binding_max_count_default for the default.
     */
    if (binding_max_cnt == (unsigned32) NULL)
    {
        lookup_rep->max_vector_size = rpc_c_binding_max_count_default;
    }
    else
    {
        lookup_rep->max_vector_size = binding_max_cnt;
    }
    

    *status = rpc_s_ok;
    
    /*
     * if everything went well, free the nsentry and return the lookup
     * context handle and the status of rpc__bindlkup_node_next_attr.
     */
    *lookup_context = (rpc_ns_handle_t) lookup_rep;
    return;

CLEANUP:

    /*
     * if something went wrong free any memory that was allocated
     */
    if (lookup_rep != NULL)
    {
        if (lookup_rep->if_spec != NULL)
        {
            if (lookup_rep->if_spec->syntax_vector.count > 0 &&
                lookup_rep->if_spec->syntax_vector.syntax_id != NULL)
            {
                RPC_MEM_FREE (lookup_rep->if_spec->syntax_vector.syntax_id,
                    RPC_C_MEM_SYNTAX_ID);
            }
            
            RPC_MEM_FREE (lookup_rep->if_spec, RPC_C_MEM_IF_REP);
        }
        

        RPC_MEM_FREE (lookup_rep, RPC_C_MEM_LKUP_REP);
    }

    if (nsentry != NULL)
    {
        rpc__nsentry_free (&nsentry, &temp_status);
    }

    /*
     * return NULL context handle and status of the last meaningful operation
     */
    *lookup_context = NULL;
    return;
}

/*
**++
**
**  ROUTINE NAME:       rpc_ns_binding_lookup_next
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**      
**  Traverses the namespace entries collecting client-compatible protocol
**  towers. The towers are returned as bindings in a binding vector.
**
**  INPUTS:
**
**      lookup_context  The lookup context handle for this search sequence.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      binding_vector  The returned vector of compatible bindings.
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_ns_handle
**                          rpc_s_no_more_bindings
**                          rpc_s_cycle_detected
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
**/

PUBLIC void rpc_ns_binding_lookup_next 
#ifdef _DCE_PROTO_
(
    rpc_ns_handle_t         lookup_context,
    rpc_binding_vector_p_t  *binding_vector,
    unsigned32              *status
)
#else
(lookup_context, binding_vector, status)
rpc_ns_handle_t         lookup_context;
rpc_binding_vector_p_t  *binding_vector;
unsigned32              *status;
#endif
{
    rpc_lkup_rep_p_t        lookup_rep;
    rpc_lkup_node_p_t       lookup_node;
    unsigned32              temp_status;
    unsigned32              i;
    

    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();
    
    /*
     * set the binding vector pointer to NULL to anticipate failure
     */
    *binding_vector = NULL;

    /*
     * see if we were passed a valid lookup context
     */
    if (lookup_context == NULL)
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    /*
     * cast the lookup handle as a lookup rep
     */
    lookup_rep = (rpc_lkup_rep_p_t) lookup_context;
    
    /*
     * determine if the lookup node list in this lookup context is empty
     */
    if (RPC_LIST_EMPTY (lookup_rep->node_list))
    {
        *status = rpc_s_no_more_bindings;
        return;
    }
    
    /*
     * create a binding vector to accomodate the max vector size bindings
     */
    RPC_MEM_ALLOC (
        *binding_vector,
        rpc_binding_vector_p_t,
        sizeof (rpc_binding_vector_t) + ((lookup_rep->max_vector_size - 1) *
        sizeof (rpc_binding_handle_t)),
        RPC_C_MEM_BINDING_VEC,
        RPC_C_MEM_WAITOK);
    if (*binding_vector == NULL){
	*status = rpc_s_no_memory;
	return;
    }

    /*
     * initialize the count field in the binding vector.
     */
    (*binding_vector)->count = 0;

    /*
     * initialize the binding vector by setting each entry to NULL
     */
    for (i = 0; i < lookup_rep->max_vector_size; i++)
    {
        (*binding_vector)->binding_h[i] = NULL;
    }
    
    /*
     * fill up the binding vector with compatible bindings
     * (we won't exit this loop until the binding vector is full,
     * or there are no more bindings to obtain - i.e. there are no
     * more nodes on the node list - barring failures)
     */
    do
    {
        /*
         * get the first element off the lookup node list
         */
        RPC_LIST_FIRST (lookup_rep->node_list, lookup_node, rpc_lkup_node_p_t);

        /*
         * process this node until it is resolved (at this attribute type)
         */
        rpc__bindlkup_node_process
            (lookup_rep, lookup_node, *binding_vector, status);

        /*
         * check the status that gets returned
         */
        switch ((int)*status)
        {
            /*
             * The entry does not exists
             */
            case rpc_s_entry_not_found:
                
                /*
                 * If this is the first entry, halt the search
                 * and return to caller.
                 */
                if (lookup_rep->first_entry_flag == true)
                {
                    rpc_binding_vector_free (binding_vector, &temp_status);
                    return;
                }
                /*
                 * Otherwise, ignore it and continue.
                 */
                else                
                {
                    lookup_node->type = rpc_e_search_attr_done;
                    break;
                } 

            /*
             * We went through all of the bindings for this entry (and
             * didn't fill the binding vector), or
             * we have exhausted all members of a group or all members 
             * of all priorities of a profile.
             */
            case rpc_s_no_more_members:

            /* 
             * We didn't get any compatible bindings because we were
             * unable to match the specified object UUID.
             */
            case rpc_s_not_found:
            case rpc_s_obj_uuid_not_found:
                /*
                 * In all cases above, set up for the next iteration
                 * of the loop by seeing if the active lookup node
                 * has any other RPC attributes to process and
                 * continue searching.
                 */
                rpc__bindlkup_node_next_attr (lookup_rep, lookup_node, 
                                              status);
		if (*status == rpc_s_no_memory){
                rpc_binding_vector_free (binding_vector, &temp_status);
                return;
		}
                break;
            
            /*
             * We successfully picked a group or profile member and added
             * the member to the head of the active nodes list.
             */
            case rpc_s_mbr_picked:

                break;

            /*
             * If there are no more members in the current priority group,
             * set up for the next priority group and return the current
             * binding vector if it has bindings.
             */
            case rpc_s_priority_group_done:

                rpc__next_priority_group_count (lookup_node);
                if ((*binding_vector)->count > 0)
		{
			if (lookup_rep->eval_routines != NULL)
			{
				/******************************************/
				/* Additional compatibility checking here */
				/******************************************/
				rpc__i14y_compat_check (
					lookup_rep,
					binding_vector,
					&temp_status );

				if (temp_status == rpc_s_ok)
				{
					*status = rpc_s_ok;
					return;
				}
				else
				{
					if (temp_status == rpc_s_ss_no_compat_codeset)
						/* get more bindings */
						break;
					else
					{
						/* return with the error status */
						*status = temp_status;
						return;
					}
				}
			}
			else
			{
				*status = rpc_s_ok;
				return;
			}
                }
                else
                {
                    break;
                }

            /*
             * if we've filled the binding vector,  return it
             */
            case rpc_s_binding_vector_full:
		if (lookup_rep->eval_routines != NULL)
		{
			/******************************************/
			/* Additional compatibility checking here */
			/******************************************/
			rpc__i14y_compat_check (
				lookup_rep,
				binding_vector,
				&temp_status );

			if (temp_status == rpc_s_ok)
			{
				*status = rpc_s_ok;
				return;
			}
			else
			{
				if (temp_status == rpc_s_ss_no_compat_codeset)
					/* get more bindings */
					break;
				else
				{
					/* return with the error status */
					*status = temp_status;
					return;
				}
			}
		}
		else
		{
			*status = rpc_s_ok;
			return;
		}

            /*
             * anything else is an error - clean up the mess
             */
            default:
                /*
                 * free the binding vector and return the status from
                 * rpc__bindlkup_node_process
                 */
                rpc_binding_vector_free (binding_vector, &temp_status);
                return;
        }
            
        /*
         * now check the status of the current node
         */
        if (lookup_node->type == rpc_e_search_attr_done)
        {
            /*
             * if we're done with this node, free it
             */
            RPC_LIST_REMOVE (lookup_rep->node_list, lookup_node);

            rpc__bindlkup_node_free (&lookup_node, status);

            if (*status != rpc_s_ok)
            {
                rpc_binding_vector_free (binding_vector, &temp_status);
                return;
            }
        }

        /*
         * we may not be done with this node yet
         */
        else
        {
            /*
             * see if this is the first call made using this lookup context
             */
            if (lookup_rep->first_entry_flag == true)
            {
                /*
                 * mark it as a used context
                 */
                lookup_rep->first_entry_flag = false;

                /*
                 * if any compatible bindings were found, return them
                 * before processing any other attributes on this entry
                 */
                if ((*binding_vector)->count > 0)
		{
			if (lookup_rep->eval_routines != NULL)
			{
				/******************************************/
				/* Additional compatibility checking here */
				/******************************************/
				rpc__i14y_compat_check (
					lookup_rep,
					binding_vector,
					&temp_status );

				if (temp_status == rpc_s_ok)
				{
					*status = rpc_s_ok;
					return;
				}
				else
				{
					if (temp_status == rpc_s_ss_no_compat_codeset)
						/* get more bindings */
						break;
					else
					{
						/* return with the error status */
						*status = temp_status;
						return;
					}
				}
			}
			else
			{
				*status = rpc_s_ok;
				return;
			}
                }
            }
        }
    }
    while (! (RPC_LIST_EMPTY (lookup_rep->node_list)));

    /*
     * if we're done with the node list, see what's in the binding
     * vector
     */
    if ((*binding_vector)->count > 0)
    {
        if (lookup_rep->eval_routines != NULL)
        {
		/******************************************/
		/* Additional compatibility checking here */
		/******************************************/
		rpc__i14y_compat_check (
			lookup_rep,
			binding_vector,
			&temp_status );

		if (temp_status == rpc_s_ok)
		{
			*status = rpc_s_ok;
			return;
		}
		else
		{
			if (temp_status == rpc_s_ss_no_compat_codeset)
			{
				/*
				 * if the binding vector is empty, report it
				 */
				rpc_binding_vector_free (binding_vector, status);

				/*
				 * if we freed the binding vector successfully,
				 * return no more bindings, otherwise return
				 * the error status.
				 */
				if (*status == rpc_s_ok)
				{
					*status = rpc_s_no_more_bindings;
				}
				return;
			}
			else
			{
				/* return with the error status */
				*status = temp_status;
				return;
			}
		}
        }
        else
        {
                /*
                 * if there's anything in the vector, return it
                 */
                *status = rpc_s_ok;
                return;
        }
    }
    else
    {
        /*
         * if the binding vector is empty, report it
         */
        rpc_binding_vector_free (binding_vector, status);

        /*
         * if we freed the binding vector successfully, 
         * return no more bindings, otherwise return
         * the error status.
         */
        if (*status == rpc_s_ok)
        {
            *status = rpc_s_no_more_bindings;
        }

        return;
    }
}

/*
**++
**
**  ROUTINE NAME:       rpc__bindlkup_node_process
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  This routine processes an individual lookup node and returns when
**  it has been completely resolved. In the case of a node representing
**  a namespace entry with protocols towers, it is resolved either when
**  the binding vector is full, or all of the available compatible
**  bindings have been obtained. In the case of a node representing a
**  namespace entry with a group or profile, it is resolved when
**  a valid member has been added to the node list. The routine will
**  return if a cycle is detected during this resolution process.
**
**  INPUTS:
**
**      lookup_context  The context for processing this lookup node
**
**      lookup_node     The lookup node to be processed
**
**  INPUTS/OUTPUTS:
**
**      binding_vector  A vector of bindings associated with this lookup node
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_cycle_detected
**                      or errors from a called routine
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
**/

INTERNAL void rpc__bindlkup_node_process 
#ifdef _DCE_PROTO_
(
    rpc_lkup_rep_p_t        lookup_context,
    rpc_lkup_node_p_t       lookup_node,
    rpc_binding_vector_t    *binding_vector,
    unsigned32              *status
)
#else
(lookup_context, lookup_node, binding_vector, status)
rpc_lkup_rep_p_t        lookup_context;
rpc_lkup_node_p_t       lookup_node;
rpc_binding_vector_t    *binding_vector;
unsigned32              *status;
#endif
{        
    rpc_nsentry_p_t         nsentry;
    unsigned32              mbr_pick_status;
    unsigned32              nsentry_free_status;
        

    CODING_ERROR (status);
    
    /*
     * See what type of lookup node this is.
     */

    if (lookup_node->type == rpc_e_search_attr_start)
    {
        rpc__bindlkup_node_next_attr (lookup_context, lookup_node, status);
        if (*status == rpc_s_no_memory)
           return;
    }

    if (lookup_node->type == rpc_e_search_attr_towers)
    {
	/*
	 * this is an entry containing protocol towers -
	 * get the compatible towers from the namespace entry and return them
	 */
	rpc__bindlkup_node_get_bindings
	    (lookup_context, binding_vector, status);

	return;
    }
    
    /*
     * this is an entry containing a group or profile -
     * We want to select one of the members and add it to the active nodes
     * list. Stay in this loop until we select a member whose entry exists
     * or we run out of group or profile members.
     */
    do
    {
        /*
         * randomly pick one of the members in the active node's list
         */
        nsentry = rpc__bindlkup_node_pick_mbr (lookup_node, &mbr_pick_status);

        if (mbr_pick_status == rpc_s_mbr_picked)
        {
            /*
             * if the list is not empty, check for a cycle - if so, skip
             * this member and pick a new one.
             */
            if (rpc__bindlkup_node_is_cycle (lookup_context, nsentry, status))
            {
                break;
            }
                    
            /*
             * create a lookup node for the selected entry
             */
            rpc__bindlkup_node_create (nsentry, lookup_context, status);

            if (*status != rpc_s_ok)
            {
                /*
                 * if the selected entry does not exist (or there was some
                 * other problem), free the name service rep
                 */
                rpc__nsentry_free (&nsentry, &nsentry_free_status);

                /*
                 * if either of the two previous calls had an unexpected
                 * problem, return with the appropriate status
                 * (but if it was just an entry not found, we'll continue)
                 */
                if (*status != rpc_s_entry_not_found)
                {
                    return;
                }

                if (nsentry_free_status != rpc_s_ok)
                {
                    *status = nsentry_free_status;
                    return;
                }
            }
        }
    }
    while ((mbr_pick_status == rpc_s_mbr_picked) &&
           ((*status == rpc_s_entry_not_found) || (*status == rpc_s_cycle_detected)));

    /* 
     * Return the status from picking a member.
     */
    *status = mbr_pick_status;

    return;
}

/*
**++
**
**  ROUTINE NAME:       rpc_ns_binding_lookup_done
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**      
**  Deletes the lookup context when a client has finished searching for
**  compatible bindings.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      lookup_context  The lookup context to be deleted.
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
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
**/

PUBLIC void rpc_ns_binding_lookup_done 
#ifdef _DCE_PROTO_
(
    rpc_ns_handle_t         *lookup_context,
    unsigned32              *status
)
#else
(lookup_context, status)
rpc_ns_handle_t         *lookup_context;
unsigned32              *status;
#endif
{
    rpc_lkup_rep_p_t        lookup_rep;
    rpc_lkup_node_p_t       lookup_node;
    rpc_lkup_mbr_p_t        member_element;
    unsigned32              local_status = rpc_s_ok;
    
    int i;
    rpc_cs_eval_func_p_t    eval_func;
    rpc_cs_eval_list_p      eval_list;
    rpc_cs_eval_list_p      eval_list_next;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();
    
    /*
     * see if the lookup context is valid
     */
    if (*lookup_context == (rpc_ns_handle_t) NULL)
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }
    
    lookup_rep = (rpc_lkup_rep_p_t) *lookup_context;
    
    /*
     * free each node on the lookup node list...
     */
    for (;;)
    {
        /*
         * remove the first lookup node on the node list
         */
        RPC_LIST_REMOVE_HEAD
            (lookup_rep->node_list, lookup_node, rpc_lkup_node_p_t);
    
        if (lookup_node == (rpc_lkup_node_p_t) NULL)
        {
            break;
        }

        /*
         * if a node was found, free it
         */
        rpc__bindlkup_node_free (&lookup_node, status);

        if (*status != rpc_s_ok)
        {
            local_status = *status;
        }
    }

    /*
     * free each node on the non-leaf list...
     */
    for (;;)
    {
        RPC_LIST_REMOVE_HEAD
            (lookup_rep->non_leaf_list, member_element, rpc_lkup_mbr_p_t);

        if (member_element == (rpc_lkup_mbr_p_t) NULL)
        {
            break;
        }
        
        /*
         * if a member was found, free it
         */
        rpc__nsentry_free (&(member_element->name), status);

        if (*status != rpc_s_ok)
        {
            local_status = *status;
        }

        rpc__list_element_free 
            (&rpc_g_free_members, (pointer_t) member_element);
    }
    
    /*
     * free the lookup rep interface spec if one was specified
     */
    if (lookup_rep->if_spec != NULL)
    {
        if (lookup_rep->if_spec->syntax_vector.count > 0)
        {
            RPC_MEM_FREE (lookup_rep->if_spec->syntax_vector.syntax_id,
                RPC_C_MEM_SYNTAX_ID);
        }

        RPC_MEM_FREE (lookup_rep->if_spec, RPC_C_MEM_IF_REP);
    }
    
    /* if there is holdover tower context (such as might happen if
     * the number of server bindings in the name service is an even
     * multiple of the number in the lookup binding vector), then
     * tower context might not have been freed - if not, do so now
     */
    if (lookup_rep->inq_cntx != NULL)
    {
        rpc__tower_mbr_inq_done (&(lookup_rep->inq_cntx), status);

        if (*status != rpc_s_ok)
        {
            local_status = *status;
        }
    }

    /*
     * Free evaluation function relating stuff.
     * List is followed to delete all of data.  If customized code set
     * evaluation is used, user supplied free() function is called.
     */
    if (lookup_rep->eval_routines != NULL)
    {
	/******************************************/
	/* Additional compatibility cleanup here */
	/******************************************/
	rpc__i14y_compat_cleanup (
		lookup_rep,
		status );

	eval_func = (rpc_cs_eval_func_p_t)lookup_rep->eval_routines;
	eval_list = (rpc_cs_eval_list_p)eval_func->list;
	switch ((int)eval_list->type)
	{
		case RPC_EVAL_TYPE_CODESETS:
			for (i=0; i < eval_func->num; i++)
			{
                            eval_list_next = eval_list->next;

                            RPC_MEM_FREE(eval_list, RPC_C_MEM_LIST);
                            eval_list = eval_list_next;
			}
			RPC_MEM_FREE(eval_func, RPC_C_MEM_FUNC);
			break;

		case RPC_CUSTOM_EVAL_TYPE_CODESETS:
			for (i=0; i < eval_func->num; i++)
			{
			     eval_list_next = eval_list->next;

                             (*(eval_list->cs_free_func))(eval_list->args);
                             (*(eval_list->cs_free_func))(eval_list->cntx);
                             free(eval_list);
                             eval_list = eval_list_next;
			}
			break;

		default:
			break;
	}
    }


    /*
     * free the lookup context itself and set the lookup_context
     * parameter to NULL.
     */
    RPC_MEM_FREE (lookup_rep, RPC_C_MEM_LKUP_REP);
    *lookup_context = (rpc_ns_handle_t) NULL;
    
    /*
     * update the return status value based on the result of previous
     * operations - if any failed it will show up here (it was done this way
     * so that an error in one operation would not cause subsequent
     * operations not to execute)
     */
    *status = local_status;

    return;
}

/*
**++
**
**  ROUTINE NAME:       rpc_ns_binding_select
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**      
**  Randomly chooses a binding from among those in a binding vector and
**  returns it to the caller. Each time a binding is selected it is
**  removed from the binding vector, so it won't be selected again on
**  repeated calls to this routine.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      binding_vector  The binding vector from which to select a binding.
**
**  OUTPUTS:
**
**      binding         The selected binding.
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_no_more_bindings
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
**/

PUBLIC void rpc_ns_binding_select 
#ifdef _DCE_PROTO_
(
    rpc_binding_vector_t    *binding_vector,
    rpc_binding_handle_t    *binding,
    unsigned32              *status
)
#else
(binding_vector, binding, status)
rpc_binding_vector_t    *binding_vector;
rpc_binding_handle_t    *binding;
unsigned32              *status;
#endif
{
    unsigned32              i, n, unused_bindings;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT ();
    
    /*
     * determine the number of unused bindings by counting the number of
     * non-NULL pointers (unused bindings) in the binding vector
     */
    for (i = 0, unused_bindings = 0; i < binding_vector->count; i++)
    {
        if (binding_vector->binding_h[i] != NULL)
        {        
            unused_bindings++;
        }
    }
    
    /*
     * see if there are any more unused bindings
     */
    if (unused_bindings == 0)
    {
        binding = NULL;
        *status = rpc_s_no_more_bindings;
        return;
    }

    /*
     * pick a random number n from the set of unused bindings
     * and walk the binding vector to get the nth unused binding
     */
    n = RPC_RANDOM_GET (1, unused_bindings);

    for (i = 0; n > 0; i++)
    {
        if (binding_vector->binding_h[i] != NULL)
        {        
            n--;
        }
    }

    /*
     * Correct index to be that of selected binding.
     */
    i--;

    *binding = binding_vector->binding_h[i];

    /*
     * mark this entry in the binding vector as used
     */
    binding_vector->binding_h[i] = NULL;
        
    *status = rpc_s_ok;
    return;        
}

/*
**++
**
**  ROUTINE NAME:       rpc__bindlkup_node_create
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  Creates and initializes a lookup node list element for a namespace entry
**  and adds it to the head of the lookup node list.
**
**  INPUTS:
**
**      nsentry         The name service representation of a namespace entry.
**
**  INPUTS/OUTPUTS:
**
**      lookup_context  The lookup context to which this node is to be added.
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_entry_not_found
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
**/

INTERNAL void rpc__bindlkup_node_create 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t         nsentry,
    rpc_lkup_rep_t          *lookup_context,
    unsigned32              *status
)
#else
(nsentry, lookup_context, status)
rpc_nsentry_p_t         nsentry;
rpc_lkup_rep_t          *lookup_context;
unsigned32              *status;
#endif
{
    rpc_lkup_node_p_t       lookup_node;
    

    CODING_ERROR (status);

    /*
     * get an element from the free nodes list ("true" is for blocking alloc)
     */
    lookup_node = (rpc_lkup_node_p_t)
        (rpc__list_element_alloc (&rpc_g_free_lookup_nodes, true));
    if (lookup_node == NULL){
	*status = rpc_s_no_memory;
	return;
    }
    
    /*
     * initialize the state of the lookup node
     * and add it to the lookup node list
     */
    lookup_node->name = nsentry;
    lookup_node->type = rpc_e_search_attr_start;
    lookup_node->mbrs_count = 0;
    lookup_node->priority_group_count = 0;
    lookup_node->classversion_checked = false;
    lookup_node->classversion_check_status = rpc_s_class_version_mismatch;
    RPC_LIST_INIT (lookup_node->mbrs_list);
    RPC_LIST_ADD_HEAD
        (lookup_context->node_list, lookup_node, rpc_lkup_node_p_t);
    
    *status = rpc_s_ok;
    return;
}

/*
**++
**
**  ROUTINE NAME:       rpc__bindlkup_node_expand_mbrs
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  Processes the first element in the lookup nodes list of the lookup rep.
**  Gets the members of a group or profile and adds them to the lookup node's
**  members list.
**
**  INPUTS:
**
**      lookup_context  The lookup context in which this operation is to be
**                      performed.
**
**  INPUTS/OUTPUTS:
**
**      lookup_node     The lookup node whose members are to be expanded.
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_no_memory
**                          rpc_s_not_found
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
**/

INTERNAL void rpc__bindlkup_node_expand_mbrs 
#ifdef _DCE_PROTO_
(
    rpc_lkup_rep_p_t        lookup_context,
    rpc_lkup_node_t         *lookup_node,
    unsigned32              *status
)
#else
(lookup_context, lookup_node, status)
rpc_lkup_rep_p_t        lookup_context;
rpc_lkup_node_t         *lookup_node;
unsigned32              *status;
#endif
{
    unsigned_char_t         entry_name[RPC_C_ENTRY_NAME_MAX];
    rpc_ns_handle_t         inquiry_context;
    rpc_lkup_mbr_p_t        member_element;
    unsigned_char_p_t       member_name;
    unsigned32              member_priority,
                            entry_name_length;
    rpc_if_id_t             if_id,
                            elt_if_id;
    unsigned32              search_by,
                            temp_status;
    rpc_lkup_mbr_p_t	    default_element = NULL;
    boolean		    match;

    CODING_ERROR (status);
    
    /*
     * create a runtime representation of the nsentry name
     * in the lookup node - we need this to call inquire begin
     */
    rpc__nsentry_to_entry (lookup_node->name,
        RPC_C_NS_INTERNAL_NAME_SYNTAX, entry_name, &entry_name_length, status);

    if (*status != rpc_s_ok)
    {
        return;
    }
        
    /*
     * see what type of node this is
     */
    if (lookup_node->type == rpc_e_search_attr_group)
    {
        /*
         * set up to inquire all of the members of the group
         */
        rpc_ns_group_mbr_inq_begin (RPC_C_NS_INTERNAL_NAME_SYNTAX, entry_name,
            RPC_C_NS_INTERNAL_NAME_SYNTAX, &inquiry_context, status);

        if (*status != rpc_s_ok)
        {
            return;
        }

        /*
         * Use the cache expiration age from the lookup rep in case
         * the application set it. If the application did not set the 
         * age, we'll be getting the global value from the time the
         * lookup rep was created.
         */
        rpc_ns_mgmt_handle_set_exp_age (inquiry_context,
            lookup_context->common.exp_age, status);
            
        if (*status != rpc_s_ok)
        {
            /*
             * Undo the begin call.
             */
            rpc_ns_group_mbr_inq_done (&inquiry_context, &temp_status);
            return;
        }
       
	/*
	 * Before starting to call rpc_ns_group_mbr_inq_next, 
	 * initialize the classversion_checked and classversion_check_status
	 * elements in the inquiry context to the corresponding values
	 * from the lookup node.  That way, if the RPC_ClassVersion for
	 * this namespace entry was already checked when looking up 
	 * a different attribute, it will not have to be checked again.
	 */
 
	((rpc_ns_inq_rep_p_t)inquiry_context)->classversion_checked =
	    lookup_node->classversion_checked;
	((rpc_ns_inq_rep_p_t)inquiry_context)->classversion_check_status =
	    lookup_node->classversion_check_status;
	
        /*
         * keep inquiring until there are no more members or some error happens
         */
        for (;;)
        {
            /*
             * get the next member from the group (until there are no more)
             */
            rpc_ns_group_mbr_inq_next (inquiry_context, &member_name, status);

            if (*status != rpc_s_ok)
            {
                break;
            }

            /*
             * get an element from the free list ("true" is for blocking alloc)
             */
            member_element = (rpc_lkup_mbr_p_t)
                (rpc__list_element_alloc (&rpc_g_free_members, true));

            if (member_element == NULL)
            {
                rpc_string_free (&member_name, &temp_status);
                *status = rpc_s_no_memory;
                break;
            }
            
            /*
             * store returned nsentry in the name field of the member element
             */
            member_element->name = rpc__nsentry_from_entry
                (RPC_C_NS_INTERNAL_NAME_SYNTAX, member_name, status);

            if (*status != rpc_s_ok)
            {
                rpc_string_free (&member_name, &temp_status);
                rpc__list_element_free 
                    (&rpc_g_free_members, (pointer_t) member_element);
                break;
            }
                        
            /*
             * add the member element to the lookup node members list
             */
            RPC_LIST_ADD_HEAD
                (lookup_node->mbrs_list, member_element, rpc_lkup_mbr_p_t);

            lookup_node->mbrs_count++;

            rpc_string_free (&member_name, &temp_status);
        }
                
        /*
         * If no more members but the mbr_count is 0, then the attribute
         * doesn't exist. Return an appropriate error status. If
         * member count is greater than 0, then loop exited normally.
         */
        if (*status == rpc_s_no_more_members)
        {
            if (lookup_node->mbrs_count == 0)
            {
                *status = rpc_s_not_found;
            }
            else
            {
                *status = rpc_s_ok;
            }
        }
        
	/*
	 * Now that we have finished calling rpc_ns_group_mbr_inq_next, save
	 * the classversion_checked and classversion_check_status results
	 * from the inquiry context into the lookup node context.  That
	 * way, the RPC_ClassVersion attribute will not have to be read again
	 * from the namespace the next time an attribute for that entry
	 * is looked up using this lookup node context.
	 */
  
	lookup_node->classversion_checked =
	    ((rpc_ns_inq_rep_p_t)inquiry_context)->classversion_checked;
	lookup_node->classversion_check_status =
	    ((rpc_ns_inq_rep_p_t)inquiry_context)->classversion_check_status;

        /*
         * when all of the members have been obtained, free inquiry context
         * and return with the status of the last meaningful operation.
         * (use a temporary status variable so we don't overwrite error
         * status from an earlier operation - unless the inq done fails)
         */
        rpc_ns_group_mbr_inq_done (&inquiry_context, &temp_status);

        if (temp_status != rpc_s_ok)
        {
            *status = temp_status;
        }
    
        return;
    }
    
    /*
     * if it's not a group it must be a profile -
     * get the interface version numbers from the if spec
     */
    if (lookup_context->if_spec != NULL)
    {
	rpc_if_inq_id ((rpc_if_handle_t)(lookup_context->if_spec), 
	    &if_id, status);
	
	if (*status != rpc_s_ok)
	{
	    return;
	}
    }
    /*
     * Force the search algorithm to be rpc_c_profile_all_elts.
     * To avoid running two import sequences, we will do our own
     * filtering.  This may not seem very clean, but it does
     * improve performance by not having to issue two sets
     * of rpc_ns_profile_elt_inq* calls
     */

    search_by = rpc_c_profile_all_elts;

    /*
     * set up an inquire for all profile elements.  we do the
     * filtering here to reject non-matching interfaces.
     */
    rpc_ns_profile_elt_inq_begin (RPC_C_NS_INTERNAL_NAME_SYNTAX, entry_name, 
        search_by, &if_id, rpc_c_vers_compatible, 
        RPC_C_NS_INTERNAL_NAME_SYNTAX, NULL, &inquiry_context, status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Use the cache expiration age from the lookup rep in case
     * the application set it. If the application did not set the 
     * age, we'll be getting the global value from the time the
     * lookup rep was created.
     */
    rpc_ns_mgmt_handle_set_exp_age (inquiry_context,
        lookup_context->common.exp_age, status);

    if (*status != rpc_s_ok)
    {
        /*
         * Undo the begin call.
         */
        rpc_ns_profile_elt_inq_done (&inquiry_context, &temp_status);
        return;
    }

    /*
     * Before starting to call rpc_ns_group_mbr_inq_next, 
     * initialize the classversion_checked and classversion_check_status
     * elements in the inquiry context to the corresponding values
     * from the lookup node.  That way, if the RPC_ClassVersion for
     * this namespace entry was already checked when looking up 
     * a different attribute, it will not have to be checked again.
     */
 
    ((rpc_ns_inq_rep_p_t)inquiry_context)->classversion_checked =
	lookup_node->classversion_checked;
    ((rpc_ns_inq_rep_p_t)inquiry_context)->classversion_check_status =
	lookup_node->classversion_check_status;

    /*
     * keep inquiring until there are no more elements or some error happens
     */
    for (;;)
    {
        /*
	 * get the next compatible element from the profile 
	 */
        rpc_ns_profile_elt_inq_next (inquiry_context, &elt_if_id,
            &member_name, &member_priority, NULL, status);

        if (*status != rpc_s_ok)
        {
            break;
        }

 
        /*
	 * First check for the default profile.
	 */
        match = uuid_is_nil (&(elt_if_id.uuid), status);
       
        if (*status != rpc_s_ok) {
	    rpc_string_free (&member_name, &temp_status);
	    break;
        }

        if (match) {
	    /*
	     * We have found the default profile.  Save it away for later.
	     */
	    default_element = (rpc_lkup_mbr_p_t)
		(rpc__list_element_alloc (&rpc_g_free_members, true));

	    if (default_element == NULL)
	    {
		*status = rpc_s_no_memory;
		rpc_string_free (&member_name, &temp_status);
		break;
	    }

	    /*
	     * store returned nsentry in the name field of the member element
	     */
	    default_element->name = rpc__nsentry_from_entry
		(RPC_C_NS_INTERNAL_NAME_SYNTAX, member_name, status);

	    if (*status != rpc_s_ok)
	    {
		rpc_string_free (&member_name, &temp_status);
		break;
	    }

	    /*
	     * store the returned member's priority in the member element
	     */
	    default_element->priority = member_priority;

	    /*
	     * Continue on
	     */
	    rpc_string_free (&member_name, &temp_status);
	    continue;
        }

        /*
	 * It's not the default profile.  Now check to see if the if id matches,
	 * if we are searching for a specific one.
	 */
        match = true;
 
        if (lookup_context->if_spec != NULL) {
	    match = rpc__if_id_compare(&if_id,
		&elt_if_id,
		rpc_c_vers_compatible, status);

	    if (*status != rpc_s_ok) {
		rpc_string_free (&member_name, &temp_status);
		break;
	    }
        }

        if (!match) {
	    rpc_string_free (&member_name, &temp_status);
	    continue;
	}

        /*
	 * get an element from the free list ("true" is for blocking alloc)
	 */
        member_element = (rpc_lkup_mbr_p_t)
            (rpc__list_element_alloc (&rpc_g_free_members, true));

        if (member_element == NULL)
        {
            rpc_string_free (&member_name, &temp_status);
            *status = rpc_s_no_memory;
            break;
        }
            
        /*
	 * store returned nsentry in the name field of the member element
	 */
        member_element->name = rpc__nsentry_from_entry
            (RPC_C_NS_INTERNAL_NAME_SYNTAX, member_name, status);
            
        if (*status != rpc_s_ok)
        {
            rpc_string_free (&member_name, &temp_status);
            rpc__list_element_free 
                (&rpc_g_free_members, (pointer_t) member_element);
            break;
        }

        /*
	 * store the returned member's priority in the member element
	 */
        member_element->priority = member_priority;

        /*
	 * add the member element to the lookup node members list
	 */
        RPC_LIST_ADD_HEAD
            (lookup_node->mbrs_list, member_element, rpc_lkup_mbr_p_t);

        lookup_node->mbrs_count++;
            
        rpc_string_free (&member_name, &temp_status);
    }
        
    /*
     * Now that we have finished calling rpc_ns_group_mbr_inq_next, save
     * the classversion_checked and classversion_check_status results
     * from the inquiry context into the lookup node context.  That
     * way, the RPC_ClassVersion attribute will not have to be read again
     * from the namespace the next time an attribute for that entry
     * is looked up using this lookup node context.
     */
    
    lookup_node->classversion_checked =
	((rpc_ns_inq_rep_p_t)inquiry_context)->classversion_checked;
    lookup_node->classversion_check_status =
	((rpc_ns_inq_rep_p_t)inquiry_context)->classversion_check_status;

    /*
     * when all of the members have been obtained, free inquiry context
     * and check to see if it worked
     */
    rpc_ns_profile_elt_inq_done (&inquiry_context, &temp_status);

    if (temp_status != rpc_s_ok)
    {
        *status = temp_status;
        return;
    }
    

    /*
     * now see if an error happened in the loop above - if so, exit
     */
    if (*status != rpc_s_no_more_elements)
    {
        return;
    }
    
    /*
     * sort the member list by priority
     */
    sort_members (lookup_node);

    /*
     * then tack the default profile from this entry on at the end.
     */
    if (default_element != NULL) 
    { 
       /*
        * add the default profile to the end of the members list
        * so that it will be processed last
        */
       RPC_LIST_ADD_TAIL
           (lookup_node->mbrs_list, default_element, rpc_lkup_mbr_p_t);

       lookup_node->mbrs_count++;
    }

    /* 
     * If the status is rpc_s_no_more_elements and the mbrs_count is 0,
     * the profile attribute doesn't exist. Return the appropriate
     * status.
     */
     
    if ((*status == rpc_s_no_more_elements) &&
        (lookup_node->mbrs_count == 0))
    {
        *status = rpc_s_not_found;
    }

    /*
     * if inquire on default profile exited due to no more elements,
     * then routine is successful.
     */
    if (*status == rpc_s_no_more_elements) 
    {
        *status = rpc_s_ok;
    }

    /*
     * initialize the lookup node's priority group count with the
     * number of elements at the first priority.
     */
    rpc__next_priority_group_count (lookup_node);

    if (temp_status != rpc_s_ok)
    {
        *status = temp_status;
    }

    /*
     * and return the status from the last meaningful operation
     */
    return;
}    

/*
**++
**
**  ROUTINE NAME:       sort_members
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  This routine performs an insertion sort to place the members list in
**  priority order (with the highest priority at the head of the list).
**  Note that the priority values range from 0-7, with 0 being the highest
**  priority and 7 being the lowest.
**
**  Computer scientists will observe that this algorithm provides one of
**  the fastest sorting methods for lists of up to around 25 elements. We
**  are therefore making the assumption that members lists will normally
**  be in this range.
**
**  Algorithm retrieved from the "Fundamentals of Data Structures in Pascal",
**  by Horowitz and Sahni.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      lookup_node     The lookup node whose members are to be sorted.
**
**  OUTPUTS:            none
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
**/

INTERNAL void sort_members 
#ifdef _DCE_PROTO_
(
    rpc_lkup_node_t         *lookup_node
)
#else
(lookup_node)
rpc_lkup_node_t         *lookup_node;
#endif
{
    rpc_lkup_mbr_p_t        current_element;
    rpc_lkup_mbr_p_t        next_element;
    rpc_lkup_mbr_t          dummy_element;
    unsigned32              n, i;
    

    /*
     * put a dummy element at the head of the list with 
     * the highest priority (all subsequent priorities are lower)
     */
    dummy_element.priority = RPC_C_NS_PRIORITY_MIN;
    RPC_LIST_ADD_HEAD
        (lookup_node->mbrs_list, &dummy_element, rpc_lkup_mbr_p_t);

    /*
     * start with the third element on the list, and do them all
     * (third because the dummy element was added.)
     */
    for (n = 3; n <= lookup_node->mbrs_count+1; n++)
    {
        /*
         * extract the nth element from the current list
         */
        RPC_LIST_EXTRACT
            (lookup_node->mbrs_list, current_element, rpc_lkup_mbr_p_t, n);

        /*
         * search down the list for an element with a higher priority
         */
        RPC_LIST_LOOKUP
            (lookup_node->mbrs_list, next_element, rpc_lkup_mbr_p_t, n-1);
        
        for (i=n-1;
             (current_element->priority < next_element->priority);
             i--)
        {
            RPC_LIST_LOOKUP
                (lookup_node->mbrs_list, next_element, rpc_lkup_mbr_p_t, i-1);
        }
                
        /*
         * insert the current element after the element with the
         * higher priority (which is numerically smaller).
         */
        RPC_LIST_ADD 
            (lookup_node->mbrs_list, next_element, current_element, 
                rpc_lkup_mbr_p_t);
    }            

    /*
     * remove the dummy element from the end of the list
     */
    RPC_LIST_REMOVE_HEAD
        (lookup_node->mbrs_list, current_element, rpc_lkup_mbr_p_t);
}

/*
**++
**
**  ROUTINE NAME:       rpc__bindlkup_node_free
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  Frees one of the nodes in the lookup node list.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      lookup_node     The lookup node to be freed.
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
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
**/

INTERNAL void rpc__bindlkup_node_free 
#ifdef _DCE_PROTO_
(
    rpc_lkup_node_p_t       *lookup_node,
    unsigned32              *status
)
#else
(lookup_node, status)
rpc_lkup_node_p_t       *lookup_node;
unsigned32              *status;
#endif
{
    rpc_lkup_mbr_p_t        member_element;
    

    CODING_ERROR (status);
    
    /*
     * free the namespace entry name in this node
     */
    rpc__nsentry_free (&((*lookup_node)->name), status);

    if (*status != rpc_s_ok)
    {
        return;
    }
    
    /*
     * free each element in the members list...
     */
    for (;;)
    {
        /*
         * remove the first member on the members list
         */
        RPC_LIST_REMOVE_HEAD
            ((*lookup_node)->mbrs_list, member_element, rpc_lkup_mbr_p_t);
    
        if (member_element == NULL)
        {
            break;
        }

        /*
         * if a member was found, free it
         */
        rpc__nsentry_free (&(member_element->name), status);

        if (*status != rpc_s_ok)
        {
            return;
        }

        rpc__list_element_free 
            (&rpc_g_free_members, (pointer_t) member_element);
    }
        
    /*
     * return the lookup node memory to the free list
     */
    rpc__list_element_free 
        (&rpc_g_free_lookup_nodes, (pointer_t) *lookup_node);
        
    /*
     * return with the ok status from the last rpc__nsentry_free
     */
    return;
}

/*
**++
**
**  ROUTINE NAME:       rpc__bindlkup_node_get_bindings
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  Returns the compatible bindings, if any, from a single namespace entry.
**
**  INPUTS:
**
**      lookup_context  The lookup context containing the namespace entry
**                      name from which the bindings are to be obtained.
**
**  INPUTS/OUTPUTS:
**
**      binding_vector  A vector of bindings that were obtained.
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_no_more_members
**                          rpc_s_not_found
**                          rpc_s_binding_vector_full
**                      and errors from called routines.
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
**/

INTERNAL void rpc__bindlkup_node_get_bindings 
#ifdef _DCE_PROTO_
(
    rpc_lkup_rep_p_t        lookup_context,
    rpc_binding_vector_t    *binding_vector,
    unsigned32              *status
)
#else
(lookup_context, binding_vector, status)
rpc_lkup_rep_p_t        lookup_context;
rpc_binding_vector_t    *binding_vector;
unsigned32              *status;
#endif
{
    rpc_lkup_node_p_t       lookup_node;
    twr_p_t                 tower;
    rpc_tower_ref_p_t       tower_ref;
    rpc_nsentry_p_t         nsentry_name,
                            nsentry_inq;
    boolean                 got_obj_uuid;
    unsigned32              temp_status;
        

    CODING_ERROR (status);
    
    lookup_node = (rpc_lkup_node_p_t)NULL;
    
    /*
     * if the inquiry context in the lookup context is NULL,
     * we're just beginning to process the towers for this entry
     */
    if (lookup_context->inq_cntx == NULL)
    {
        /*
         * Get an object UUID to place in compatible towers
         */
        switch (lookup_context->obj_uuid_search)
        {
            case rpc_e_obj_uuid_search_match:
                got_obj_uuid =
                    rpc__bindlkup_obj_uuid_match (lookup_context, status);
                break;
            
            case rpc_e_obj_uuid_search_any:
                rpc__bindlkup_obj_uuid_any (lookup_context, status);

                /*
                 * This will be where we find out an entry doesn't exist.
                 */
		got_obj_uuid = (*status == rpc_s_ok);
                break;
        }

        if (got_obj_uuid == true)
        {
            /*
             * if an object UUID was found, get the active lookup node
             */
            RPC_LIST_FIRST
                (lookup_context->node_list, lookup_node, rpc_lkup_node_p_t);

            /*
             * Initialize the inquiry context using the nsentry from
             * this node and make sure we got one back.
             *
             * First, allocate a new nsentry for the inquire context and
             * copy the lookup_node's nsentry.  
             */
            nsentry_inq = rpc__nsentry_copy (lookup_node->name, status);
            if (*status != rpc_s_ok) 
            {
                return;
            }

            rpc__tower_mbr_inq_begin
                (nsentry_inq, &(lookup_context->inq_cntx), status);
            if (*status != rpc_s_ok) 
            {
                rpc__nsentry_free (&nsentry_inq, &temp_status);
                return;
            }
            
            /*
             * Use the cache expiration age from the lookup rep in case
             * the application set it. If the application did not set the 
             * age, we'll be getting the global value from the time the
             * lookup rep was created.
             */
            rpc_ns_mgmt_handle_set_exp_age (lookup_context->inq_cntx,
                lookup_context->common.exp_age, status);
                    
            if (*status != rpc_s_ok)
            {
                /*
                 * Undo the begin call.
                 */
                rpc__tower_mbr_inq_done (&(lookup_context->inq_cntx),
					&temp_status);
                                        
                /*
                 * Free the nsentry
                 */
                rpc__nsentry_free (&nsentry_inq, &temp_status);
                
                return;
            }

        }
        else
        {
            /*
             * if no object UUID was found, return with the status
             * from the object lookup call
             */
            return;
        }
    }

    /*
     * If the lookup node is still NULL at this point, it means that
     * we are not just beginning to process the towers for this entry.
     * lookup_node would be null at this point if this entry had
     * more that 5 bindings, resulting in
     * rpc_ns_binding_lookup_next being called a second time.
     * This could also occur for an entry with less than 5 bindings
     * if the last binding in the entry was enough to fill the
     * return vector.  The next call to rpc_ns_binding_lookup_next would
     * then need to issue one more call to rpc__tower_mbr_inq_next to
     * determine that there are no more handles in this entry.
     */
    if (lookup_node == NULL) {
         RPC_LIST_FIRST
                (lookup_context->node_list, lookup_node, rpc_lkup_node_p_t);
    }

    /*
     * Before starting to call rpc__tower_mbr_inq_next,
     * initialize the classversion_checked and classversion_check_status
     * elements in the inquiry context to the corresponding values
     * from the lookup node.  That way, if the RPC_ClassVersion for
     * this namespace entry was already checked when looking up
     * a different attribute, it will not have to be checked again.
     */
    ((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_checked =
	lookup_node->classversion_checked;
    ((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_check_status =
	lookup_node->classversion_check_status;

    /*
     * process all of the towers in this entry looking for compatible bindings
     */
    for (;;)
    {
        /*
         * get the next tower from the entry (until there are no more)
         */
        rpc__tower_mbr_inq_next (lookup_context->inq_cntx, &tower, status);

        if (*status != rpc_s_ok)
        {
            break;
        }

        /*
         * convert the canonical tower to a tower reference and see 
         * if it's compatible with this interface
         */
        rpc__tower_to_tower_ref (tower, &tower_ref, status);
        
        if (*status != rpc_s_ok)
        {
            rpc__tower_free (&tower, &temp_status);
            break;
        }
        
        if ((rpc__tower_ref_is_compatible
            (lookup_context->if_spec, tower_ref, status)) == true)
        {
            /*
             * convert the tower to a binding, 
             * placeing it in the binding vector
             */
            rpc_tower_to_binding (tower->tower_octet_string,
                &(binding_vector->binding_h[binding_vector->count]), status);
            
            if (*status != rpc_s_ok)
            {
                rpc__tower_ref_free (&tower_ref, &temp_status);
                rpc__tower_free (&tower, &temp_status);
                break;
            }
        
            /*
             * place the lookup context's object UUID into the binding
             */
            rpc_binding_set_object
                (binding_vector->binding_h[binding_vector->count],
                &(lookup_context->obj_for_binding), status);

            if (*status != rpc_s_ok)
            {
                rpc__tower_ref_free (&tower_ref, &temp_status);
                rpc__tower_free (&tower, &temp_status);
                break;
            }
        
            /*
             * Allocate memory to hold the entry name and 
             * copy the entry name into this memory.  Add a pointer
             * to it to the binding rep.
             */
            nsentry_name = rpc__nsentry_copy (
                ((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->nsentry, 
                status);
            if (*status != rpc_s_ok) 
            {
                rpc__tower_ref_free (&tower_ref, &temp_status);
                rpc__tower_free (&tower, &temp_status);
                break;
            }

            ((rpc_binding_rep_p_t)
                (binding_vector->binding_h[binding_vector->count]))
                    ->ns_specific = (pointer_t) nsentry_name;

            /*
             * add the towers protocol version number to the binding
             */
            rpc__binding_set_prot_version (
                             binding_vector->binding_h[binding_vector->count], 
                             tower_ref, 
                             status);
            if (*status != rpc_s_ok)
            {
                rpc__tower_ref_free (&tower_ref, &temp_status);
                rpc__tower_free (&tower, &temp_status);
                break;
            }

            /*
             * now that we're sure it's done, add to the binding vector count
             */
            binding_vector->count++;
            
            /*
             * see if the binding vector is full
             */
            if (binding_vector->count == lookup_context->max_vector_size)
            {
                rpc__tower_ref_free (&tower_ref, &temp_status);
                rpc__tower_free (&tower, &temp_status);
                *status = rpc_s_binding_vector_full;
		/*
		 * Now that we have finished calling
		 * rpc_ns_group_mbr_inq_next, save the
		 * classversion_checked and classversion_check_status
		 * results from the inquiry context into the lookup
		 * node context.  That way, the RPC_ClassVersion
		 * attribute will not have to be read again from the
		 * namespace the next time an attribute for that entry
		 * is looked up using this lookup node context.
		 */
    
		lookup_node->classversion_checked =
		    ((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_checked;
		lookup_node->classversion_check_status =
		    ((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_check_status;
                return;
            }
        }

        /*
         * free the tower and the tower reference
         */
        rpc__tower_free (&tower, &temp_status);
        rpc__tower_ref_free (&tower_ref, &temp_status);

        /*
         * check the status returned by rpc__tower_ref_is_compatible
         */
        if (*status != rpc_s_ok)
        {
            break;
        }
    }
                
    /*
     * if loop exited due to no more members (rpc_s_no_more_members) we've 
     * found all the bindings for the current attribute.
     *
     * When all of the towers have been obtained, free inquiry context.
     * Note, that this will free the nsentry allocated in this routine
     * for the inquires.
     */
    rpc__tower_mbr_inq_done (&(lookup_context->inq_cntx), &temp_status);

    if (temp_status != rpc_s_ok)
    {
        *status = temp_status;
    }
    
    /*
     * return the status from the last meaningful operation
     */
    return;
}


/*
**++
**
**  ROUTINE NAME:       rpc__bindlkup_obj_uuid_match
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  This routine checks for matches between a specified object UUID and
**  the set of object UUID's in the namespace entry pointed to by the
**  given lookup node. Returns the object UUID to include in compatible
**  bindings.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      lookup_context  The lookup context in which to place the object UUID.
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_not_found
**                      and errors from called routines.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      boolean         true if object UUID is available for setting
**                      into compatible bindings.
**
**                      false if an object UUID is not available.
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL boolean rpc__bindlkup_obj_uuid_match 
#ifdef _DCE_PROTO_
(
    rpc_lkup_rep_p_t        lookup_context,
    unsigned32              *status
)
#else
(lookup_context, status)
rpc_lkup_rep_p_t        lookup_context;
unsigned32              *status;
#endif
{
    unsigned_char_t         entry_name[RPC_C_ENTRY_NAME_MAX];
    rpc_lkup_node_p_t       lookup_node;
    uuid_t                  obj_uuid;
    unsigned32              temp_status,
                            entry_name_length;
    

    CODING_ERROR (status);
    
    /*
     * get the first entry on the lookup node list
     */
    RPC_LIST_FIRST (lookup_context->node_list, lookup_node, rpc_lkup_node_p_t);
    
    /*
     * create a runtime representation of the nsentry name
     * in the lookup node - we need this to call inquire begin
     */
    rpc__nsentry_to_entry (lookup_node->name,
        RPC_C_NS_INTERNAL_NAME_SYNTAX, entry_name, &entry_name_length, status);

    if (*status != rpc_s_ok)
    {
        return (false);
    }

    /*
     * initialize the inquiry context
     */
    
    rpc_ns_entry_object_inq_begin (RPC_C_NS_INTERNAL_NAME_SYNTAX,
        entry_name, &(lookup_context->inq_cntx), status);
        
    if (*status != rpc_s_ok)
    {
        return (false);
    }
    
    /*
     * Use the cache expiration age from the lookup rep in case
     * the application set it. If the application did not set the 
     * age, we'll be getting the global value from the time the
     * lookup rep was created.
     */
    rpc_ns_mgmt_handle_set_exp_age ( lookup_context->inq_cntx,
        lookup_context->common.exp_age, status );

    if (*status != rpc_s_ok)
    {
        /*
         * Undo the begin call.
         */
        rpc_ns_entry_object_inq_done (&(lookup_context->inq_cntx),
				      &temp_status);
        return (false);
    }

    /*
     * Before starting to call rpc_ns_entry_object_inq_next,
     * initialize the classversion_checked and classversion_check_status
     * elements in the inquiry context to the corresponding values
     * from the lookup node.  That way, if the RPC_ClassVersion for
     * this namespace entry was already checked when looking up
     * a different attribute, it will not have to be checked again.
     */
    ((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_checked =
               lookup_node->classversion_checked;
    ((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_check_status =
               lookup_node->classversion_check_status;

    /*
     * check out each object UUID in the entry
     */
    do
    {
        rpc_ns_entry_object_inq_next (
            lookup_context->inq_cntx, &obj_uuid, status);

        if (*status == rpc_s_no_more_members)
        {
            break;
        }

        if (*status != rpc_s_ok) 
        {
            /*
             * Cleanup and return.
             */
            rpc_ns_entry_object_inq_done (&(lookup_context->inq_cntx), &temp_status);

            return (false);
        }
        
        /*
         * determine if the object UUID's are equivalent
         */
        if (uuid_equal (&(lookup_context->obj_uuid), &obj_uuid, &temp_status)) 
        {
            /*
             * if equal, save the new object UUID in the lookup context
             */
            lookup_context->obj_for_binding = obj_uuid;
            
            /*
             * a successful match, break out of loop
             */
            break;
        }
        
        /*
         * If status of uuid_equal is bad, return false with the status 
         */
        if (temp_status != uuid_s_ok)
        {
            *status = temp_status;
            return (false);
        }
    } 
    while (*status != rpc_s_no_more_members);
    
    /*
     * Now that we have finished calling rpc_ns_entry_object_inq_next, save
     * the classversion_checked and classversion_check_status results
     * from the inquiry context into the lookup node context.  That
     * way, the RPC_ClassVersion attribute will not have to be read again
     * from the namespace the next time an attribute for that entry
     * is looked up using this lookup node context.
     */
    lookup_node->classversion_checked =
	((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_checked;
    lookup_node->classversion_check_status =
	((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_check_status;

    /*
     * if there are no more members, there was no match
     */
    rpc_ns_entry_object_inq_done (&(lookup_context->inq_cntx), &temp_status);

    if (*status == rpc_s_no_more_members)
    {
        *status = rpc_s_not_found;
        return (false);
    }

    return (true);
}    

/*
**++
**
**  ROUTINE NAME:       rpc__bindlkup_obj_uuid_any
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  This routine picks an object UUID from the set of object UUID's 
**  in the namespace entry pointed to by the given lookup node. 
**  If the namespace entry has on object UUIDs, the nil UUID is picked.
**  The returned object UUID is included in compatible bindings.
**
**  NOTE: This routine returns the 1st object UUID returned
**        from by the name service. 
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      lookup_context  The lookup context in which to place the object UUID.
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                      and errors from called routines.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__bindlkup_obj_uuid_any 
#ifdef _DCE_PROTO_
(
    rpc_lkup_rep_p_t        lookup_context,
    unsigned32              *status
)
#else
(lookup_context, status)
rpc_lkup_rep_p_t        lookup_context;
unsigned32              *status;
#endif
{
    unsigned_char_t         entry_name[RPC_C_ENTRY_NAME_MAX];
    rpc_lkup_node_p_t       lookup_node;
    uuid_t                  obj_uuid;
    unsigned32              temp_status,
                            entry_name_length;

    CODING_ERROR (status);
    
    /*
     * get the first entry on the lookup node list
     */
    RPC_LIST_FIRST (lookup_context->node_list, lookup_node, rpc_lkup_node_p_t);
    
    /*
     * create a runtime representation of the nsentry name
     * in the lookup node - we need this to call inquire begin
     */
    rpc__nsentry_to_entry (lookup_node->name,
        RPC_C_NS_INTERNAL_NAME_SYNTAX, entry_name, &entry_name_length, status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * initialize the inquiry context
     */
    rpc_ns_entry_object_inq_begin (RPC_C_NS_INTERNAL_NAME_SYNTAX,
        entry_name, &(lookup_context->inq_cntx), status);
        
    if (*status != rpc_s_ok)
    {
        return;
    }
    
    /*
     * Use the cache expiration age from the lookup rep in case
     * the application set it. If the application did not set the 
     * age, we'll be getting the global value from the time the
     * lookup rep was created.
     */
    rpc_ns_mgmt_handle_set_exp_age ( lookup_context->inq_cntx,
        lookup_context->common.exp_age, status );

    if (*status != rpc_s_ok)
    {
        goto CLEANUP;
    }

    /*
     * Set the classversion_checked and classversion_check_status for
     * the inquiry context to those of the lookup node before
     * calling rpc_ns_entry_object_inq_next to avoid checking
     * the RPC_ClassVersion if it has been already checked.
     * After returning from rpc_ns_entry_object_inq_next, save
     * the classversion_checked and classversion_check_status
     * from the inquiry context so the RPC_ClassVersion will
     * not have to be checked more than once.
     */

    ((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_checked = 
               lookup_node->classversion_checked;
    ((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_check_status =
               lookup_node->classversion_check_status;
    /*
     * See if the entry contains an object UUID.
     */
    rpc_ns_entry_object_inq_next (lookup_context->inq_cntx, &obj_uuid, status);

    lookup_node->classversion_checked = 
               ((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_checked;
    lookup_node->classversion_check_status =
               ((rpc_ns_inq_rep_p_t)lookup_context->inq_cntx)->classversion_check_status;

    switch ((int)*status)
    {
        /*
         * Entry contains no object UUIDS. Specify the nil UUID to be
         * returned with the compatible bindings.
         */
        case rpc_s_no_more_members:
            uuid_create_nil (&(lookup_context->obj_for_binding), status);
            break;

        /* 
         * Got back a UUID. Save it to be returned with the compatible bindings.
         */
        case rpc_s_ok:
            lookup_context->obj_for_binding = obj_uuid;
            break;

        default:
            break;
    }
        
CLEANUP:
    /*
     * Release the inqiry context.
     */
    rpc_ns_entry_object_inq_done (&(lookup_context->inq_cntx), &temp_status);

    /*
     * If we got here normally (nothing went wrong earlier), return the
     * status from the inq-done operation. Otherwise, we'll be returning
     * the earlier problem status.
     */
    if (*status == rpc_s_ok)
    {
        *status = temp_status;
    }

    return;
}    

/*
**++
**
**  ROUTINE NAME:       rpc__bindlkup_node_is_cycle
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  Detects cycles in nested groups and profiles.
**
**  INPUTS:
**
**      lookup_context  The lookup context containing the non-leaf list
**                      of nodes to be checked for cycles.
**
**
**      lookup_node     The current namespace entry.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_cycle_detected
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      result          true - A cycle was detected in nested group
**                      and/or profile members.
**
**                      false - No cycle was detected.
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL boolean rpc__bindlkup_node_is_cycle 
#ifdef _DCE_PROTO_
(
    rpc_lkup_rep_p_t        lookup_context,
    rpc_nsentry_p_t         nsentry,
    unsigned32              *status
)
#else
(lookup_context, nsentry, status)
rpc_lkup_rep_p_t        lookup_context;
rpc_nsentry_p_t         nsentry;
unsigned32              *status;
#endif
{
    rpc_lkup_mbr_p_t        lookup_member,
                            prev_lookup_member,
                            last_non_leaf_member;


    CODING_ERROR (status);
    
    /*
     * walk the non-leaf list
     */
    RPC_LIST_FIRST
        (lookup_context->non_leaf_list, lookup_member, rpc_lkup_mbr_p_t);

    /*
     * save this for predecessor comparisons.
     */
    last_non_leaf_member = lookup_member;

    while (lookup_member != NULL)
    {
        /*
         * compare the entry name in the nsentry arg to the one in the
         * current member element - if they're equal, check their
         * predecessors.  If predecessors are equal too, report a cycle
         */
        if ((nsentry->name_length == lookup_member->name->name_length) &&
	    ((memcmp ((char *) (nsentry->name), 
		      (char *) (lookup_member->name->name),
		      nsentry->name_length)) == 0))
        {
            /*
             * get the predecessor of the non_leaf_list node -
             * the non_leaf_list is a lifo list
             */
            RPC_LIST_NEXT
                (lookup_member, prev_lookup_member, rpc_lkup_mbr_p_t);
            if (prev_lookup_member == NULL)
            {
                /*
                 * we have the first non_leaf entry visited -
                 * it's a cycle
                 */
                *status = rpc_s_cycle_detected;
                return (true);
            }
            else
            {
                /*
                 * compare the predecessor of the current entry
                 * being processed (since it's a lifo, this is the 
                 * first element) with the matching non_leaf node's
                 * predecessor.
                 */
                if ((memcmp ((char *)last_non_leaf_member->name->name,
                        (char *) (prev_lookup_member->name->name),
                        last_non_leaf_member->name->name_length)) == 0)
                {
                    *status = rpc_s_cycle_detected;
                    return (true);
                }
            }
        }
        
        /*
         * get the next element on the list
         */
        RPC_LIST_NEXT (lookup_member, lookup_member, rpc_lkup_mbr_p_t);
    }

    /*
     * if we got through the whole list, there must not be any cycles
     */
    *status = rpc_s_ok;
    return (false);        
}

/*
**++
**
**  ROUTINE NAME:       rpc__bindlkup_node_next_attr
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  Determines the next attribute to process for the namespace entry
**  in the specified lookup node. Attributes are processed in the
**  following order: towers, groups, profile.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      lookup_node     The lookup node list element to process.
**
**  OUTPUTS:
**
**      status          The result of the operation. Always:
**                          rpc_s_ok
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
**/

INTERNAL void rpc__bindlkup_node_next_attr 
#ifdef _DCE_PROTO_
(
    rpc_lkup_rep_p_t        lookup_context,
    rpc_lkup_node_t         *lookup_node,
    unsigned32              *status
)
#else
(lookup_context, lookup_node, status)
rpc_lkup_rep_p_t        lookup_context;
rpc_lkup_node_t         *lookup_node;
unsigned32              *status;
#endif
{
    unsigned32              attribute_type, temp_status;
    rpc_lkup_mbr_p_t        member_element;


    CODING_ERROR (status);
    
    /*
     * Continue the search down the list using the next attribute
     * type. For example, if there are no towers, search the
     * group and so on.
     */

    do 
    {
        switch (lookup_node->type)
        {
            case rpc_e_search_attr_start:
                lookup_node->type = rpc_e_search_attr_towers;
                attribute_type = RPC_C_ATTR_DNA_TOWERS;
                break;
                
            case rpc_e_search_attr_towers:
                lookup_node->type = rpc_e_search_attr_group;
                attribute_type = RPC_C_ATTR_GROUP;
                break;

            case rpc_e_search_attr_group:
                lookup_node->type = rpc_e_search_attr_profile;
                attribute_type = RPC_C_ATTR_PROFILE;
                break;
                
            case rpc_e_search_attr_profile:
                lookup_node->type = rpc_e_search_attr_done;
                break;

            case rpc_e_search_attr_codesets:
		lookup_node->type = rpc_e_search_attr_codesets;
		attribute_type = RPC_C_ATTR_CODESETS;
                break;

	    case rpc_e_search_attr_done:
	        /*
	         * rpc_m_invalid_srchattr
	         * "(%s) Unexpected search attribute seen"
	         */
	        RPC_DCE_SVC_PRINTF ((
		    DCE_SVC(RPC__SVC_HANDLE, "%s"),
		    rpc_svc_nsi,
		    svc_c_sev_fatal | svc_c_action_abort,
		    rpc_m_invalid_srchattr,
		    "rpc__bindlkup_node_next_attr" ));
	        break;
        }

        /*
         * check the type of the current entry
         */
        switch (lookup_node->type)
        {
            case rpc_e_search_attr_towers:
	    case rpc_e_search_attr_codesets:    /* Verify it's RIGHT??? */
            case rpc_e_search_attr_done:
                *status = rpc_s_ok;
                break;

            /*
             * groups and profiles have to be expanded
             */
            case rpc_e_search_attr_group:
            case rpc_e_search_attr_profile:
                /*
                 * first add name to non-leaf list ("true" is for
                 * blocking alloc) - allocate memory for it, copy the name
                 * into the member element, and add it to the non-leaf list
                 */
                member_element = (rpc_lkup_mbr_p_t)
                rpc__list_element_alloc (&rpc_g_free_members, true);
		if (member_element == NULL){
		    *status = rpc_s_no_memory;
                    return;
                }

                member_element->name = 
                    rpc__nsentry_copy (lookup_node->name, status);
		if (*status == rpc_s_no_memory){
                    rpc__list_element_free(&rpc_g_free_members,
                        (pointer_t)member_element);
		    return;
		}
            
                RPC_LIST_ADD_HEAD (lookup_context->non_leaf_list,
                    member_element, rpc_lkup_mbr_p_t);
                
                rpc__bindlkup_node_expand_mbrs
                    (lookup_context, lookup_node, status);

                /*
                 * If the attribute was not found, undo what we
                 * just did.
                 */
                if (*status == rpc_s_not_found)
                {
                    RPC_LIST_REMOVE_HEAD ( 
                        lookup_context->non_leaf_list, member_element,
                        rpc_lkup_mbr_p_t);
                        
                    rpc__nsentry_free(&(member_element->name), &temp_status);
                    
                    rpc__list_element_free(&rpc_g_free_members,
                        (pointer_t)member_element);
                }
                break;

	    case rpc_e_search_attr_start:
		break;

	    default:
		break;
		
         }      
    }

    while ((*status != rpc_s_ok) && (lookup_node->type !=
                                    rpc_e_search_attr_done))
	/*void*/ ;
    /*
     * Return rpc_s_ok if towers attribute or done, otherwise
     * return status from last meaningful call.
     */
    return;
}

/*
**++
**
**  ROUTINE NAME:       rpc__bindlkup_node_pick_mbr
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  Randomly selects an element from a lookup node's members list. If the
**  lookup node represents a group, the element selected is a group member.
**  If a profile, a profile member.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:
**
**      lookup_node     Lookup node from which the member is to be selected.
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_mbr_picked
**                          rpc_s_no_more_members
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      nsentry         A pointer to the nsentry of the selected group or
**                      profile member.
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL rpc_nsentry_t *rpc__bindlkup_node_pick_mbr 
#ifdef _DCE_PROTO_
(
    rpc_lkup_node_p_t       lookup_node,
    unsigned32              *status
)
#else
(lookup_node, status)
rpc_lkup_node_p_t       lookup_node;
unsigned32              *status;
#endif
{
    unsigned32              n;
    rpc_lkup_mbr_p_t        member_element;
    rpc_nsentry_p_t         nsentry;


    CODING_ERROR (status);
    
    /*
     * see if there are any members left on the list
     */
    if (lookup_node->mbrs_count == 0)
    {
        *status = rpc_s_no_more_members;
        return NULL;
    }
    

    /*
     * see what type of entry this is
     */
    switch (lookup_node->type)
    {
        case rpc_e_search_attr_group:
        {    
            /*
             * pick a random number from 1 to the number of members available
             */
            n = RPC_RANDOM_GET (1, lookup_node->mbrs_count);
    
            /*
             * extract that member from the list
             */
            RPC_LIST_EXTRACT
                (lookup_node->mbrs_list, member_element, rpc_lkup_mbr_p_t, n);
    
            /*
             * get a pointer to the nsentry in this member
             */
            nsentry = member_element->name;
    
            /*
             * free the extracted member
             */
            rpc__list_element_free 
                (&rpc_g_free_members, (pointer_t) member_element);

            /*
             * decrement the members count field in the lookup node
             */
            lookup_node->mbrs_count--;

            *status = rpc_s_mbr_picked;
            
            break;    
        }

        case rpc_e_search_attr_profile:
        {
            /*
             * If this is the last member of a priority group, let the
             * caller know.
             */
            if (lookup_node->priority_group_count == 0)
            {
                *status = rpc_s_priority_group_done;
                return NULL;
            }

            /*
             * pick a random number from 1 to the priority group count
             */
            n = RPC_RANDOM_GET (1, lookup_node->priority_group_count);
    
            /*
             * extract that member from the list
             */
            RPC_LIST_EXTRACT
                (lookup_node->mbrs_list, member_element, rpc_lkup_mbr_p_t, n);
    
            /*
             * get a pointer to the nsentry in this member
             */
            nsentry = member_element->name;
    
            /*
             * free the extracted member
             */
            rpc__list_element_free 
                (&rpc_g_free_members, (pointer_t) member_element);

            /*
             * decrement the members count and the priority group count
             */
            lookup_node->mbrs_count--;
            lookup_node->priority_group_count--;
            
            *status = rpc_s_mbr_picked;
            
            break;    

	case rpc_e_search_attr_start:
	    break;

	default:
	    /*
	     * rpc_m_invalid_srchattr
	     * "(%s) Unexpected search attribute seen"
	     */
	    RPC_DCE_SVC_PRINTF ((
		DCE_SVC(RPC__SVC_HANDLE, "%s"),
		rpc_svc_nsi,
		svc_c_sev_fatal | svc_c_action_abort,
		rpc_m_invalid_srchattr,
		"rpc__bindlkup_node_pick_mbr" ));
	    break;
        }
    }

    /*
     * return a pointer to the selected nsentry
     */
    return (nsentry);
}


/*
**++
**
**  ROUTINE NAME:       rpc__next_priority_group_count
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  This routine initializes the priority group count field of the active
**  lookup node with the count of the members at the next lower priority.
**
**  INPUTS: 
**
**      lookup_node     The lookup node whose priority group count field
**                      needs to be initialized.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
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
**/

INTERNAL void rpc__next_priority_group_count 
#ifdef _DCE_PROTO_
(
    rpc_lkup_node_p_t       lookup_node
)
#else
(lookup_node)
rpc_lkup_node_p_t       lookup_node;
#endif
{
    rpc_lkup_mbr_p_t        member_element;
    unsigned8               current_priority;

    if (lookup_node->mbrs_count != 0)
    {
        /*
         * count the number of members that have the next priority
         */
        RPC_LIST_FIRST
            (lookup_node->mbrs_list, member_element, rpc_lkup_mbr_p_t);

        for (current_priority = member_element->priority;
            ((current_priority == member_element->priority) &&
                (lookup_node->priority_group_count < lookup_node->mbrs_count));
            lookup_node->priority_group_count++)
        {
            if (member_element->link.next != (pointer_t) NULL)
            {
                RPC_LIST_NEXT
                    (member_element, member_element, rpc_lkup_mbr_p_t);
            }
        }
    }

    return;
}


/*
**++
**
**  ROUTINE NAME:       rpc__i14y_compat_check
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  This routine calls the additional interoperatibility evaluation routines
**  between a client and a server.  This is mainly used for code set i14y,
**  and each binding in binding_vector will be checked for the i14y.
**
**  INPUTS: 
**
**      lookup_rep      This contains the pointer to additional i14y evaluation
**                      routine.
**
**  INPUTS/OUTPUTS:     
**
**      binding_vector  The selected vector of compatible bindings.
**
**  OUTPUTS:            status
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
**/

INTERNAL void rpc__i14y_compat_check (
	rpc_lkup_rep_p_t	lookup_rep,
	rpc_binding_vector_p_t  *binding_vector,
	error_status_t		*status
)
{
	unsigned32			binding_count;
	rpc_cs_eval_func_p_t		eval_funcs;
	rpc_cs_eval_list_p_t		eval_list;
	rpc_cs_method_eval_t		method;
	rpc_cs_codeset_i14y_data	codeset_i14y_data;
	rpc_cs_codeset_i14y_data	codesets_cntx;
	rpc_cs_codeset_i14y_data	*codesets_cntx_p;
	error_status_t			temp_status;
	int				i;
	unsigned_char_t			*entry_name;
	unsigned32			eval_func_count;


	 eval_funcs = (rpc_cs_eval_func_p_t)lookup_rep->eval_routines;
	eval_func_count = eval_funcs->num;
	eval_list = (rpc_cs_eval_list_p_t)eval_funcs->list;

	while (eval_func_count--)
	{
		switch ((int)eval_list->type)
		{
		case RPC_EVAL_TYPE_CODESETS:
		case RPC_CUSTOM_EVAL_TYPE_CODESETS:

			binding_count = (*binding_vector)->count;
			for (i=0; i < binding_count; i++)
			{
			   if ((*binding_vector)->binding_h[i] == NULL)
				/* This binding is already deleted from the
				   binding vector */
				break;

			   rpc_ns_binding_inq_entry_name (
				(*binding_vector)->binding_h[i],
				rpc_c_ns_syntax_default,
				&entry_name,
				status );

			   if (*status != rpc_s_ok)
				return;

			   codeset_i14y_data.ns_name = entry_name;
			   codeset_i14y_data.cleanup = ndr_false;
			   method.client = (rpc_codeset_mgmt_t *)eval_list->args;
			   codeset_i14y_data.method_p = &method;
			   codesets_cntx_p = &codesets_cntx;
			   (*(eval_list->eval_func)) (NULL, (void *)&codeset_i14y_data, (void **)&codesets_cntx_p );

			      if (codesets_cntx.status == rpc_s_ok)
			      {
				  rpc_cs_binding_set_method (
				     &((*binding_vector)->binding_h[i]),
					&method,
					status );
				  if (*status != rpc_s_ok) 
				  {
					(*binding_vector)->count--;

					rpc_binding_free(
					    &((*binding_vector)->binding_h[i]), 
					    status);
				  }
			      }
			      else 
			      {
				  /* 
				   *  Code set i14y check failed.
				   */
				  (*binding_vector)->count--;

				  rpc_binding_free(
					&((*binding_vector)->binding_h[i]), 
					status);

				  if (*status != rpc_s_ok) 
				  {
					rpc_binding_vector_free 
				        (binding_vector, &temp_status);
					return;
				  }
			      }
			  }
			  rpc_ns_mgmt_free_codesets( &method.client, &temp_status);

			  if (!((*binding_vector)->count))
			  {
			     /* There is no code set I14Y compatible bindings */
			     *status = rpc_s_ss_no_compat_codeset;
			     return;		
			  }
			  break;

		default:
			  *status = rpc_s_mgmt_bad_type;
			  return;
		}

		eval_list = (rpc_cs_eval_list_p_t)eval_list->next;
	}

	*status = temp_status;
}


/*
**++
**
**  ROUTINE NAME:       rpc__i14y_compat_cleanup
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**  This routine performs the additional interoperatibility(i14y) cleanup
**  for evaluation routines.
**  The evaluation routine do some cleanup when the import context is released.
**
**  INPUTS: 
**
**      lookup_rep      This contains the pointer to additional i14y evaluation
**                      routine.
**
**  INPUTS/OUTPUTS:     
**
**      binding_vector  The selected vector of compatible bindings.
**
**  OUTPUTS:            status
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
**/

INTERNAL void rpc__i14y_compat_cleanup (
	rpc_lkup_rep_p_t	lookup_rep,
	error_status_t		*status
)
{
	rpc_cs_eval_func_p_t		eval_funcs;
	rpc_cs_eval_list_p_t		eval_list;
	rpc_cs_method_eval_t		method;
	rpc_cs_codeset_i14y_data	codeset_i14y_data;
	rpc_cs_codeset_i14y_data	codesets_cntx;
	rpc_cs_codeset_i14y_data	*codesets_cntx_p;
	unsigned32			eval_func_count;


	eval_funcs = (rpc_cs_eval_func_p_t)lookup_rep->eval_routines;
	eval_func_count = eval_funcs->num;
	eval_list = (rpc_cs_eval_list_p_t)eval_funcs->list;

	while (eval_func_count--)
	{
		switch ((int)eval_list->type)
		{
		case RPC_EVAL_TYPE_CODESETS:
		case RPC_CUSTOM_EVAL_TYPE_CODESETS:

			   codeset_i14y_data.ns_name = NULL;
			   codeset_i14y_data.cleanup = ndr_true;
			   method.client = (rpc_codeset_mgmt_t *)eval_list->args;
			   codeset_i14y_data.method_p = &method;
			   codesets_cntx_p = &codesets_cntx;
			   (*(eval_list->eval_func)) (NULL, (void *)&codeset_i14y_data, (void **)&codesets_cntx_p );

			      if (codesets_cntx.status == rpc_s_ok)
			      {
			     	*status = rpc_s_ss_cleanup_failed;
			     	return;		
			      }

		default:
			  *status = rpc_s_mgmt_bad_type;
			  return;
		}

		eval_list = (rpc_cs_eval_list_p_t)eval_list->next;
	}

	*status = rpc_s_ok;
}
