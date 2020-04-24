/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comep.c,v $
 * Revision 1.1.74.2  1996/02/18  00:02:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:55:09  marty]
 *
 * Revision 1.1.74.1  1995/12/08  00:18:10  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:30 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/5  1995/07/14  19:07 UTC  tatsu_s
 * 	Submitted the fix for CHFts15685.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.local_rpc.b4/1  1995/07/11  21:42 UTC  tatsu_s
 * 	Don't set the default com timeout in rpc_ep_resolve_binding().
 * 
 * 	HP revision /main/HPDCE02/4  1995/06/30  20:50 UTC  tatsu_s
 * 	Submitted the fix for CHFts15450/CHFts15591.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.fix_ot12885.b1/1  1995/06/23  23:05 UTC  tatsu_s
 * 	Always reset binding after ept_map().
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/25  21:40 UTC  lmm
 * 	Merge allocation failure detection cleanup work
 * 
 * 	HP revision /main/HPDCE02/lmm_alloc_fail_clnup/1  1995/05/25  21:00 UTC  lmm
 * 	allocation failure detection cleanup
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/11  21:36 UTC  tatsu_s
 * 	Submitted the fix for CHFts15222.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.scale_fix.b0/1  1995/04/21  20:37 UTC  tatsu_s
 * 	Fixed max_call_requests to work in rpc_server_use_*protseq*().
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:13 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:18 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/07  23:58:05  root]
 * 
 * Revision 1.1.72.4  1994/09/01  18:49:59  rsalz
 * 	Add rpc_binding_to_epmap (OT CR 12001).
 * 	[1994/09/01  18:30:00  rsalz]
 * 
 * Revision 1.1.72.3  1994/07/15  18:19:26  tom
 * 	Bug 9933 - Since we declare ep.idl with the fault_status
 * 	attribute, we need to check return status for cancels and
 * 	re-raise them.
 * 	[1994/07/14  21:29:06  tom]
 * 
 * Revision 1.1.72.2  1994/03/17  23:06:42  tom
 * 	Support for protocol version in the binding handle:
 * 	  If we don't have a protocol version in the binding
 * 	  when we are trying to resolve and endpoint, make one that
 * 	  will insure us an endpoint.
 * 	[1994/03/17  23:04:26  tom]
 * 
 * Revision 1.1.72.1  1994/01/21  22:35:14  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:55:03  cbrooks]
 * 
 * Revision 1.1.6.3  1993/09/30  18:50:27  tatsu_s
 * 	Bug 7174 - Try to unregister the endpoints as much as possible,
 * 	even if the endpoint mapper returns the error status in
 * 	rpc_ep_unregister().
 * 	[1993/09/29  18:48:52  tatsu_s]
 * 
 * Revision 1.1.6.2  1993/09/28  13:04:45  tatsu_s
 * 	Bug 7257 - Check the invalid binding in rpc_ep_unregister().
 * 
 * 	Bug 7164 - Return the error status rpc_s_no_interfaces from
 * 	ep_register() and rpc_ep_unregister() if the interface is not
 * 	specified.
 * 	[1993/09/27  16:03:58  tatsu_s]
 * 
 * Revision 1.1.6.1  1993/09/07  18:58:44  tom
 * 	In get_ep_binding(), free the protseqs vector after use.
 * 	[1993/09/07  18:58:30  tom]
 * 
 * Revision 1.1.4.3  1993/01/03  23:00:03  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:01:37  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  20:20:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:33:17  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  17:01:47  rsalz
 * 	  5-mar-92 wh            DCE 1.0.1 merge.
 * 	 30-jan-92 mishkin       Fix calls to rpc__ep_mem_free
 * 	[1992/05/01  16:49:27  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:12:43  devrcs
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
**      comep.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  These routines allow applications to manipulate information in the
**  endpoint mapper database.
**
*/

#include <commonp.h>    /* Common internals for RPC runtime system  */
#include <com.h>        /* Externals for Common Services component  */
#include <comp.h>       /* Private communications services */
#include <dce/ep.h>     /* NIDL generated include for ep interface  */
#include <comtwr.h>     /* Tower <=> Tower ref routines */
#include <comtwrref.h>  /* Tower ref routines */
#include <comtwrflr.h>  /* Tower floor routines */


/*
 * Number of compatible towers to be returned from ept_map in one call.
 */
#define MAX_TOWERS  15

/*
 * Magic cookie to check mgmt_ep inquiry_context.
 */
#define MGMT_EP_C_INQUIRY_CONTEXT   1234

/*
 * mgmt_ep inquiry context.
 */

#define MAX_ENTS    15

typedef struct {
    unsigned32              usage;
    ndr_boolean             done;
    rpc_binding_handle_t    ep_binding;
    unsigned32              inquiry_type;
    uuid_t                  object;
    rpc_if_id_t             if_id;
    unsigned32              vers_option;
    ept_lookup_handle_t     entry_handle;
    unsigned32              num_ents;
    unsigned32              next_ent;
    ept_entry_t             entries[MAX_ENTS];
} mgmt_ep_inq_rep_t, *mgmt_ep_inq_rep_p_t;


INTERNAL void ep_register _DCE_PROTOTYPE_((
        rpc_if_handle_t         ifspec,
        rpc_binding_vector_t    *binding_vec,
        uuid_vector_t           *object_uuid_vec,
        unsigned_char_p_t       annotation,
        boolean32               replace,
        unsigned32              * /*status*/
    ));

INTERNAL void get_ep_binding _DCE_PROTOTYPE_((
        rpc_binding_handle_t    input_binding,
        rpc_binding_handle_t    *output_binding,
        unsigned32              * /*status*/
    ));

INTERNAL void tower_to_if_id _DCE_PROTOTYPE_((
        twr_p_t                 tower,
        rpc_if_id_t             *if_id,
        unsigned32              * /*status*/
    ));

INTERNAL void ep_get_endpoint _DCE_PROTOTYPE_((
       rpc_if_rep_p_t          if_r,
       rpc_binding_rep_p_t     binding_r,
       unsigned32              * /*st*/
    ));

INTERNAL idl_void_p_t rpc__ep_mem_alloc _DCE_PROTOTYPE_ ((
        unsigned long  /*size*/
    ));

INTERNAL void rpc__ep_mem_free _DCE_PROTOTYPE_ ((
        pointer_t  /*ptr*/
    ));


/*
**++
**
**  ROUTINE NAME:       rpc_ep_register
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**   
**  Adds server address information to the endpoint mapper database.
**  A server routine calls this routine only if the server has dynamically
**  generated endpoints.
**
**  INPUTS:
**
**      ifspec          Interface specification to register with the 
**                      endpoint mapper.
**
**      binding_vec     Vector of binding handles over which the server 
**                      can receive RPCs.
**
**      object_uuid_vec A vector of object UUIDs offered by the server. 
**                      The server application constructs this vector.
**
**      annotation      A character string comment applied to each cross-
**                      product entry added to the endpoint mapper.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_no_bindings
**                          rpc_s_invalid_binding
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

PUBLIC void rpc_ep_register 
#ifdef _DCE_PROTO_
(
    rpc_if_handle_t             ifspec,
    rpc_binding_vector_t        *binding_vec,
    uuid_vector_t               *object_uuid_vec,
    unsigned_char_p_t           annotation,
    unsigned32                  *status
)
#else
(ifspec, binding_vec, object_uuid_vec, annotation, status)
rpc_if_handle_t             ifspec;
rpc_binding_vector_t        *binding_vec;
uuid_vector_t               *object_uuid_vec;
unsigned_char_p_t           annotation;
unsigned32                  *status;
#endif
{
    ep_register (ifspec, binding_vec, object_uuid_vec, annotation, true, status);
    /*
     * Although a server may have more than one endpoint,
     * the above call registers only one endpoint per
     * protseq. Call it again with no replace to register
     * everything.
     */
    if (*status == rpc_s_ok && binding_vec != NULL && binding_vec->count > 1)
	ep_register (ifspec, binding_vec, object_uuid_vec,
		     annotation, false, status);
}

/*
**++
**
**  ROUTINE NAME:       rpc_ep_register_no_replace
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**   
**  Like rpc_ep_register, except doesn't replace any existing entries.
**
**  INPUTS:
**
**      ifspec          Interface specification to register with the 
**                      endpoint mapper.
**
**      binding_vec     Vector of binding handles over which the server 
**                      can receive RPCs.
**
**      object_uuid_vec A vector of object UUIDs offered by the server. 
**                      The server application constructs this vector.
**
**      annotation      A character string comment applied to each cross-
**                      product entry added to the endpoint mapper.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_no_bindings
**                          rpc_s_invalid_binding
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

PUBLIC void rpc_ep_register_no_replace 
#ifdef _DCE_PROTO_
(
    rpc_if_handle_t             ifspec,
    rpc_binding_vector_t        *binding_vec,
    uuid_vector_t               *object_uuid_vec,
    unsigned_char_p_t           annotation,
    unsigned32                  *status
)
#else
(ifspec, binding_vec, object_uuid_vec, annotation, status)
rpc_if_handle_t             ifspec;
rpc_binding_vector_t        *binding_vec;
uuid_vector_t               *object_uuid_vec;
unsigned_char_p_t           annotation;
unsigned32                  *status;
#endif
{
    ep_register (ifspec, binding_vec, object_uuid_vec, annotation, false, status);
}

/*
**++
**
**  ROUTINE NAME:       ep_register
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**   
**  Common routine for rpc_ep_register*.
**
**  INPUTS:
**
**      ifspec          Interface specification to register with the 
**                      endpoint mapper.
**
**      binding_vec     Vector of binding handles over which the server 
**                      can receive RPCs.
**
**      object_uuid_vec A vector of object UUIDs offered by the server. 
**                      The server application constructs this vector.
**
**      annotation      A character string comment applied to each cross-
**                      product entry added to the endpoint mapper.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_no_interfaces
**                          rpc_s_no_bindings
**                          rpc_s_invalid_binding
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

INTERNAL void ep_register 
#ifdef _DCE_PROTO_
(
    rpc_if_handle_t             ifspec,
    rpc_binding_vector_t        *binding_vec,
    uuid_vector_t               *object_uuid_vec,
    unsigned_char_p_t           annotation,
    boolean32                   replace,
    unsigned32                  *status
)
#else
(ifspec, binding_vec, object_uuid_vec, annotation, replace, status)
rpc_if_handle_t             ifspec;
rpc_binding_vector_t        *binding_vec;
uuid_vector_t               *object_uuid_vec;
unsigned_char_p_t           annotation;
boolean32                   replace;
unsigned32                  *status;
#endif
{
    ept_entry_t                 *ept_entry;
    rpc_if_rep_p_t              if_rep = (rpc_if_rep_p_t) ifspec;
    rpc_binding_handle_t        ep_binding;
    unsigned32                  i, j;
    unsigned32                  curr_hand = 0, curr_tower = 0, curr_obj = 0; 
    unsigned32                  st;
    rpc_tower_ref_vector_p_t    tower_vec;

    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    if (ifspec == NULL)
    {
        *status = rpc_s_no_interfaces;
        return;
    }

    if (binding_vec == NULL || binding_vec->count == 0)
    {
        *status = rpc_s_no_bindings;
        return;
    }          

    /*
     * Spin through the binding vector array looking for a non-NULL pointer
     * to a binding handle.  If one is found set curr_hand to its offset in 
     * the binding vector.
     * Also check for a binding handle with no endpoint; if one is found
     * return an error.
     */                                      

    curr_hand = binding_vec->count;
    for (i = 0; i < binding_vec->count; i++) 
    {
        if (binding_vec->binding_h[i] != NULL) 
        {   
            if (((rpc_binding_rep_p_t) binding_vec->binding_h[i])
                ->addr_has_endpoint == false)
            {
                *status = rpc_s_invalid_binding;
                return;
            }

            curr_hand = i;
        }
    }

    /*
     * If all the handles were NULL'd out, return an error.
     */

    if (curr_hand >= binding_vec->count)
    {
        *status = rpc_s_no_bindings;
        return;
    }          
                                             
    /*
     * Otherwise, use the a valid handle to converse with the EP mapper.
     */
    
    get_ep_binding(binding_vec->binding_h[curr_hand], &ep_binding, status);
    if (*status != rpc_s_ok)
        return;

    /*
     * Allocate the EP entry structure to avoid taking up stack space.
     */
    
    RPC_MEM_ALLOC(ept_entry, ept_entry_t *, sizeof *ept_entry, 
            RPC_C_MEM_EPT_ENTRY, RPC_C_MEM_NOWAIT);
    if (ept_entry == NULL){
	rpc_binding_free(&ep_binding, &st);       
	*status = rpc_s_no_memory;
	return;
    }

    rpc__strncpy(ept_entry->annotation, annotation, ept_max_annotation_size - 1);

    for (curr_hand = 0; curr_hand < binding_vec->count; curr_hand++)
    {                
        /*
         * Skip over NULL entries.
         */

        if (binding_vec->binding_h[curr_hand] == NULL)
        {
            continue;
        }

        /*
         * Convert the binding handle to tower_ref vector.
         */

        rpc__tower_ref_vec_from_binding(if_rep, binding_vec->binding_h[curr_hand],
                &tower_vec, status);
        if (*status != rpc_s_ok)
            break;

        for (curr_tower = 0; curr_tower < tower_vec->count; curr_tower++)
        {
            rpc__tower_from_tower_ref(tower_vec->tower[curr_tower], &ept_entry->tower, status);
            if (*status != rpc_s_ok)
                break; 

            if (object_uuid_vec == NULL || object_uuid_vec->count == 0)
            {
                ept_entry->object = uuid_g_nil_uuid;
                (*ept_v3_0_c_epv.ept_insert)
                    (ep_binding, 1L, ept_entry, replace, status);
                if (*status == rpc_s_call_cancelled)
                    pthread_cancel(pthread_self());
            }
            else
            {
                for (curr_obj = 0; curr_obj < object_uuid_vec->count; curr_obj++)
                {
                    ept_entry->object = *object_uuid_vec->uuid[curr_obj];
                    (*ept_v3_0_c_epv.ept_insert)
                        (ep_binding, 1L, ept_entry, replace, status);
                    if (*status == rpc_s_call_cancelled)
                        pthread_cancel(pthread_self());
                    if (*status != rpc_s_ok)
                        break;
                }         
            }

            rpc__tower_free(&ept_entry->tower, &st);

            if (*status != rpc_s_ok)
                break; 
        }

        rpc__tower_ref_vec_free(&tower_vec, &st);

        if (*status != rpc_s_ok)
            break;
    }

    rpc_binding_free(&ep_binding, &st);       
    RPC_MEM_FREE(ept_entry, RPC_C_MEM_EPT_ENTRY);

    /*
     * Handle any error conditions.  Try to unregister anything that was 
     * successfully registered.  Since we were trying to register the
     * cross product of two vectors we need to separate recovery into 
     * 1) binding handles for which all objects were successfully registered
     * and 2) binding handles for which some of the objects were successfully
     * registered.  
     * Punt on the fact that multiple towers can be derived from each binding 
     * handle - all towers associated with a binding handle will be unregistered 
     * by rpc_ep_unregister.
     */

    if ((*status != rpc_s_ok) && (*status != rpc_s_comm_failure) &&
        (curr_hand > 0 || curr_tower > 0 || curr_obj > 0))
    {   
        rpc_binding_vector_p_t  bvp;
        uuid_vector_p_t         ovp = NULL;
        unsigned32              bv_size, ov_size;
                     
        /*
         * First alloc a binding vector large enough to hold the number
         * of handles for which all objects were successfully registered.
         * (with a minimum size of 1 handle so that we can use it for the
         * partially successful handle also).
         */

        bv_size = sizeof(rpc_binding_vector_t) + (sizeof(handle_t) * curr_hand);
        RPC_MEM_ALLOC(bvp, rpc_binding_vector_p_t, bv_size,
            RPC_C_MEM_BINDING_VEC, RPC_C_MEM_WAITOK);
        if (bvp == NULL)
        {
            *status = rpc_s_no_memory;
            return;
        }

        /*
         * If there were any handles that were fully registered,
         * unregister them now.
         */
    
        if (curr_hand > 0)
        {
            bvp->count = curr_hand;
            for (j = 0; j < bvp->count; j++)
                bvp->binding_h[j] = binding_vec->binding_h[j];
    
            rpc_ep_unregister(ifspec, bvp, object_uuid_vec, &st);
	    if (st == rpc_s_no_memory){
		*status = rpc_s_no_memory;
		RPC_MEM_FREE(bvp, RPC_C_MEM_BINDING_VEC);
		return;
	    }
        }

        /*
         * If we got part way through registering the towers associated
         * with a binding handle for an object, behave as if we registered 
         * all the towers associated with the binding handle and the object, 
         * ie. increment the curr_obj.
         */
        if (curr_tower > 0) curr_obj++;

        /*
         * If we got part way through registering the object vector for
         * a particular handle, then we'll need to create a smaller copy
         * of the object vector for unregistering only those objects that
         * were actually registered (with that particular handle).
         */

        if (curr_obj > 0)
        { 
            /*
             * First create the partial object vector.
             */

            ov_size = sizeof(uuid_vector_t) + (sizeof(uuid_t) * (curr_obj - 1));
            RPC_MEM_ALLOC(ovp, uuid_vector_p_t, ov_size,
                RPC_C_MEM_UUID_VECTOR, RPC_C_MEM_WAITOK);
            if (ovp == NULL)
            {
                *status = rpc_s_no_memory;
                RPC_MEM_FREE(bvp, RPC_C_MEM_UUID_VECTOR);
                return;
            }

            ovp->count = curr_obj;
            for (j = 0; j < curr_obj; j++)
                ovp->uuid[j] = object_uuid_vec->uuid[j];

            /*
             * Next, make a binding vector which contains only this
             * particular handle.
             */

            bvp->count = 1;
            bvp->binding_h[0] = binding_vec->binding_h[curr_hand]; 

            rpc_ep_unregister(ifspec, bvp, ovp, &st);
	    if (st = rpc_s_no_memory)
		*status = rpc_s_no_memory;

            RPC_MEM_FREE(ovp, RPC_C_MEM_UUID_VECTOR);
        }

        RPC_MEM_FREE(bvp, RPC_C_MEM_BINDING_VEC);
    }
}

/*
**++
**
**  ROUTINE NAME:       rpc_ep_unregister
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**   
**  Removes server address information from the endpoint mapper database.
**  A server calls this routine only if the server has previously
**  registered dynamically allocated endpoints.
**
**  INPUTS:
**
**      ifspec          Interface specification to unregister with the 
**                      endpoint mapper.
**
**      binding_vec     Vector of binding handles to unregister
**
**      object_uuid_vec A vector of object UUIDs to unregister
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      status          The result of the operation. One of:
**                          rpc_s_ok
**                          rpc_s_no_interfaces
**                          rpc_s_no_bindings
**                          rpc_s_invalid_binding
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

PUBLIC void rpc_ep_unregister 
#ifdef _DCE_PROTO_
(
    rpc_if_handle_t             ifspec,
    rpc_binding_vector_t        *binding_vec,
    uuid_vector_t               *object_uuid_vec,
    unsigned32                  *status
)
#else
(ifspec, binding_vec, object_uuid_vec, status)
rpc_if_handle_t             ifspec;
rpc_binding_vector_t        *binding_vec;
uuid_vector_t               *object_uuid_vec;
unsigned32                  *status;
#endif
{ 
    ept_entry_t                 *ept_entry;
    rpc_if_rep_p_t              if_rep = (rpc_if_rep_p_t) ifspec;
    rpc_binding_handle_t        ep_binding;
    unsigned32                  i, j, k, st;
    rpc_tower_ref_vector_p_t    tower_vec;
    unsigned32                  curr_hand;
    unsigned32                  lstatus;

    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    if (ifspec == NULL)
    {
        *status = rpc_s_no_interfaces;
        return;
    }

    if (binding_vec == NULL || binding_vec->count == 0)
    {
        *status = rpc_s_no_bindings;
        return;
    }          

    /*
     * Spin through the binding vector array looking for a non-NULL pointer
     * to a binding handle.  If one is found set curr_hand to its offset in 
     * the binding vector.
     * Also check for a binding handle with no endpoint; if one is found
     * return an error.
     */                                      

    curr_hand = binding_vec->count;
    for (i = 0; i < binding_vec->count; i++)
    {
        if (binding_vec->binding_h[i] != NULL) 
        {   
            if (((rpc_binding_rep_p_t) binding_vec->binding_h[i])
                ->addr_has_endpoint == false)
            {
                *status = rpc_s_invalid_binding;
                return;
    }

            curr_hand = i;
        }
    }

    /*
     * If all the handles were NULL'd out, return an error.
     */

    if (curr_hand >= binding_vec->count)
    {
        *status = rpc_s_no_bindings;
        return;
    }          
                                             
    /*
     * Otherwise, use the a valid handle to converse with the EP mapper.
     */

    get_ep_binding(binding_vec->binding_h[curr_hand], &ep_binding, status);
    if (*status != rpc_s_ok)
        return;

    /*
     * Allocate the EP entry structure to avoid taking up stack space.
     */
    
    RPC_MEM_ALLOC(ept_entry, ept_entry_t *, sizeof *ept_entry, 
            RPC_C_MEM_EPT_ENTRY, RPC_C_MEM_NOWAIT);
    if (ept_entry == NULL){
	rpc_binding_free(&ep_binding, &st);
	*status = rpc_s_no_memory;
	return;
    }
    
    ept_entry->annotation[0] = 0;

    /*
     * We will try to unregister the endpoints as much as possible and
     * report the first error status we see (if that happens).
     *
     * Note: There is no particular reason why we report the first
     * error. (It can be the last one or middle.) The better
     * implementation is to make the endpoint mapper (rpcd) interface
     * accept a vector of endpoints and return a vector of status codes
     * for ept_insert() and ept_delete(). It may also improve the
     * performance.
     */

    lstatus = rpc_s_ok;

    for (i = 0; i < binding_vec->count; i++)
    {    
        /*
         * Skip over NULL entries.
         */

        if (binding_vec->binding_h[i] == NULL)
            continue;

        /*
         * Convert the binding handle to tower_ref vector
         */

        rpc__tower_ref_vec_from_binding(if_rep, binding_vec->binding_h[i],
                &tower_vec, &st);
        if (st != rpc_s_ok)
        {
            if (lstatus == rpc_s_ok)
                lstatus = st;

            continue;
        }

        for (j = 0; j < tower_vec->count; j++)
        {
            rpc__tower_from_tower_ref(tower_vec->tower[j], &ept_entry->tower, &st);
            if (st != rpc_s_ok)
            {
                if (lstatus == rpc_s_ok)
                    lstatus = st;

                continue;
            }

            if (object_uuid_vec == NULL || object_uuid_vec->count == 0)
            {
                ept_entry->object = uuid_g_nil_uuid;
                (*ept_v3_0_c_epv.ept_delete)(ep_binding, 1L, ept_entry, &st);
                if (st == rpc_s_call_cancelled)
                    pthread_cancel(pthread_self());
            }
            else
            {
                for (k = 0; k < object_uuid_vec->count; k++)
                {
                    ept_entry->object = *object_uuid_vec->uuid[k];
                    (*ept_v3_0_c_epv.ept_delete)(ep_binding, 1L, ept_entry, &st);
                    if (st != rpc_s_ok)
                    {
                        if (*status == rpc_s_call_cancelled)
                            pthread_cancel(pthread_self());
                        if (lstatus == rpc_s_ok)
                            lstatus = st;

                        continue;
                    }
                }         
            }

            if (st != rpc_s_ok)
            {
                if (lstatus == rpc_s_ok)
                    lstatus = st;
            }

            rpc__tower_free(&ept_entry->tower, &st);
        }

        rpc__tower_ref_vec_free(&tower_vec, &st); 
    }

    *status = lstatus;
    rpc_binding_free(&ep_binding, &st);
    RPC_MEM_FREE(ept_entry, RPC_C_MEM_EPT_ENTRY);
}

/*
**++
**
**  ROUTINE NAME:       rpc_mgmt_ep_elt_inq_begin
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**   
**  Creates an inquiry context for viewing the elements in a local or
**  remote endpoint map database.
**
**  INPUTS:
**
**      input_binding
**
**      inquiry_type
**
**      if_id
**
**      vers_option
**
**      object_uuid
**
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      inquiry_context
**
**      status          The result of the operation.
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

PUBLIC void rpc_mgmt_ep_elt_inq_begin 
#ifdef _DCE_PROTO_
(
    rpc_binding_handle_t    input_binding,
    unsigned32              inquiry_type,
    rpc_if_id_p_t           if_id,
    unsigned32              vers_option,
    uuid_p_t                object_uuid,
    rpc_ep_inq_handle_t     *inquiry_context,
    unsigned32              *status
)
#else
(input_binding, inquiry_type, if_id, vers_option, object_uuid, inquiry_context, status)
rpc_binding_handle_t    input_binding;
unsigned32              inquiry_type;
rpc_if_id_p_t           if_id;
unsigned32              vers_option;
uuid_p_t                object_uuid;
rpc_ep_inq_handle_t     *inquiry_context;
unsigned32              *status;
#endif
{
    mgmt_ep_inq_rep_t       *chp;
    rpc_binding_handle_t    ep_binding;
    uuid_t                  UUID_NIL;
    unsigned32              tmp_st;

    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    if (inquiry_context == NULL)
    {
        *status = rpc_s_invalid_inquiry_context;
        return;
    }

    *inquiry_context = NULL;

    get_ep_binding(input_binding, &ep_binding, status);
    if (*status != rpc_s_ok)
        return;

    RPC_MEM_ALLOC (
        chp,
        mgmt_ep_inq_rep_p_t,
        sizeof (mgmt_ep_inq_rep_t),
        RPC_C_MEM_INQ_REP,
        RPC_C_MEM_WAITOK);
    if (chp == NULL){
	rpc_binding_free(&ep_binding, &tmp_st);
	*status = rpc_s_no_memory;
	return;
    }

    chp->usage = MGMT_EP_C_INQUIRY_CONTEXT;
    chp->done = false;
    chp->ep_binding = ep_binding;
    chp->inquiry_type = inquiry_type;
    chp->vers_option = vers_option;
    chp->entry_handle = NULL;
    chp->num_ents = 0;
    chp->next_ent = 0;

    /*
     * Fill context's object and interface specs.  If object isn't being
     * matched, use nil_uuid.  If interface isn't being matched, use
     * nil_uuid and versions = 0.
     */ 

    uuid_create_nil(&UUID_NIL, &tmp_st);

    switch ((int)inquiry_type)
    {
        case rpc_c_ep_all_elts:
            chp->object = UUID_NIL;
            chp->if_id.uuid = UUID_NIL;
            chp->if_id.vers_major = 0;
            chp->if_id.vers_minor = 0;
            break;

        case rpc_c_ep_match_by_if:
            chp->object = UUID_NIL;
            chp->if_id = *if_id;
            break;

        case rpc_c_ep_match_by_obj:
            chp->object = *object_uuid;
            chp->if_id.uuid = UUID_NIL;
            chp->if_id.vers_major = 0;
            chp->if_id.vers_minor = 0;
            break;

        case rpc_c_ep_match_by_both:
            chp->object = *object_uuid;
            chp->if_id = *if_id;
            break;

        default:
            *status = rpc_s_invalid_inquiry_type;
            rpc_binding_free(&ep_binding, &tmp_st);
            RPC_MEM_FREE(chp, RPC_C_MEM_INQ_REP);
            return;
    }

    if ((inquiry_type == rpc_c_ep_match_by_if) || (inquiry_type == rpc_c_ep_match_by_both))
    {
        switch((int)vers_option)
        {
            case rpc_c_vers_all:
            case rpc_c_vers_compatible:
            case rpc_c_vers_exact:
            case rpc_c_vers_major_only:
            case rpc_c_vers_upto:
                break;
    
            default:
                *status = rpc_s_invalid_vers_option;
                rpc_binding_free(&ep_binding, &tmp_st);
                RPC_MEM_FREE(chp, RPC_C_MEM_INQ_REP);
                return;
        }
    }
     
    *inquiry_context = (rpc_ep_inq_handle_t) chp;

    *status = rpc_s_ok;
}


/*
**++
**
**  ROUTINE NAME:       rpc_mgmt_ep_elt_inq_next
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**   
**  Returns one element at a time from a local or remote endpoint map
**  database.
**
**  INPUTS:
**
**      inquiry_context
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      if_id
**
**      binding
**
**      object_uuid
**
**      annotation
**
**      status          The result of the operation.
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
PUBLIC void rpc_mgmt_ep_elt_inq_next 
#ifdef _DCE_PROTO_
(
    rpc_ep_inq_handle_t       inquiry_context,
    rpc_if_id_t               *if_id,
    rpc_binding_handle_t      *binding,
    uuid_t                    *object_uuid,
    unsigned_char_p_t         *annotation,
    unsigned32                *status
)
#else
(inquiry_context, if_id, binding, object_uuid, annotation, status)
rpc_ep_inq_handle_t       inquiry_context;
rpc_if_id_t               *if_id;
rpc_binding_handle_t      *binding;
uuid_t                    *object_uuid;
unsigned_char_p_t         *annotation;
unsigned32                *status;
#endif
{
    mgmt_ep_inq_rep_t       *chp;
    ept_entry_t             *entp;
    unsigned32              asize;
    unsigned32              i;
    boolean32               supported_tower = false;
    idl_void_p_t            (*old_allocate) _DCE_PROTOTYPE_ ((unsigned long ));
    idl_void_p_t            (*tmp_allocate) _DCE_PROTOTYPE_ ((unsigned long ));
    void                    (*old_free) _DCE_PROTOTYPE_ ((idl_void_p_t ));
    void                    (*tmp_free) _DCE_PROTOTYPE_ ((idl_void_p_t ));

    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    if (inquiry_context == NULL)
    {
        *status = rpc_s_invalid_inquiry_context;
        return;
    }

    chp = (mgmt_ep_inq_rep_t *) inquiry_context;

    if (chp->usage != MGMT_EP_C_INQUIRY_CONTEXT)
    {
        *status = rpc_s_invalid_inquiry_context;
        return;
    } 

    if (if_id == NULL)
    {
        *status = rpc_s_invalid_arg;
        return;
    }

    /*
     * Continue processing towers until we find one with a protseq 
     * supported by this runtime.
     */
    while (!supported_tower)
    {
        /*
         * Check the done flag in the loop just in case the last
         * tower isn't supported. We don't want to go and start
         * reading towers from the beginning again.
         */
        if (chp->done)
        {
            *status = rpc_s_no_more_elements;
            return;
        }

        if (chp->num_ents == 0)
        {
            /*
             * Force the stub to use a particular form of memory
             * allocation.
             */
            rpc_ss_swap_client_alloc_free (
                rpc__ep_mem_alloc, rpc__ep_mem_free, 
                &old_allocate, &old_free);

            /*
             *  No entries stashed away so read some from ept database
             */
            (*ept_v3_0_c_epv.ept_lookup)(chp->ep_binding, 
                chp->inquiry_type, &chp->object, &chp->if_id, chp->vers_option,
                &chp->entry_handle, MAX_ENTS, &chp->num_ents, chp->entries, 
                status);

            /*
             * Restore the memory allocation scheme in effect before 
             * we got here.
             */
            rpc_ss_swap_client_alloc_free (
                old_allocate, old_free, 
                &tmp_allocate, &tmp_free);

            if (*status != rpc_s_ok)
            {
                if (*status == ept_s_not_registered)
                    *status = rpc_s_no_more_elements;
                if (*status == rpc_s_call_cancelled)
                    pthread_cancel(pthread_self());
                return;
            }
            chp->next_ent = 0;
        } 
     
        entp = &(chp->entries[chp->next_ent]);

        if (binding != NULL)
        {
            rpc_tower_to_binding(entp->tower->tower_octet_string, binding, 
                                    status);

            /*
             * If the conversion was ok, we have a supported tower.
             * If the tower is of an unsupported protseq, we filter it out.
             * Any other status is a problem, so return.
             */
            if (*status == rpc_s_ok)
            {
                supported_tower = true;
            }
            else
            {
                if (*status != rpc_s_protseq_not_supported)
                {
                    return;
                }
            }
        }

        /*
         * The caller doesn't want the binding, so consider the tower supported
         * to return the other fields.
         */
        else
        {
            supported_tower = true;
        }

        if (supported_tower)
        {
            tower_to_if_id(entp->tower, if_id, status);
            if (*status != rpc_s_ok) 
                return;

            if (object_uuid != NULL)
                *object_uuid = entp->object;

            if (annotation != NULL)
            {
                asize = strlen((char *) entp->annotation) + 1;

                RPC_MEM_ALLOC (
                    *annotation,
                    unsigned_char_p_t,
                    asize,
                    RPC_C_MEM_STRING,
                    RPC_C_MEM_WAITOK); 
		if (*annotation == NULL){
		    *status = rpc_s_no_memory;
		    return;
		}

                rpc__strncpy(*annotation, entp->annotation, (asize-1));
             }
        }

        chp->next_ent++;

        if (chp->next_ent >= chp->num_ents)
        {
            /*
             *  Have returned all entries that are stashed away for this
             *  context to the caller.
             *  Free the towers.
             */
            for (i = 0, entp = &(chp->entries[0]); 
                i < chp->num_ents; 
                i++, entp++)
            {
                rpc__ep_mem_free ((pointer_t) entp->tower);
            }

            chp->num_ents = 0;
            chp->next_ent = 0;
                          
            /*
             * If entry handle is NULL, all matching entries have been read
             * so mark context as done.
             */
            if (chp->entry_handle == NULL)
                chp->done = true;
        }
    }

    *status = rpc_s_ok;
}

/*
**++
**
**  ROUTINE NAME:       rpc_mgmt_ep_elt_inq_done
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**   
**  Deletes the inquiry context for viewing the elements in a local or
**  remote endpoint map database.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     
**
**      inquiry_context
**
**  OUTPUTS:
**
**      status          The result of the operation.
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

PUBLIC void rpc_mgmt_ep_elt_inq_done
#ifdef _DCE_PROTO_
(
    rpc_ep_inq_handle_t     *inquiry_context,
    unsigned32              *status
)
#else
(inquiry_context, status)
rpc_ep_inq_handle_t     *inquiry_context;
unsigned32              *status;
#endif
{
    mgmt_ep_inq_rep_t       *chp; 
    ept_entry_t             *entp;
    unsigned32              i;

    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    if ((inquiry_context == NULL) || (*inquiry_context == NULL))
    {
        *status = rpc_s_invalid_inquiry_context;
        return;
    }

    chp = (mgmt_ep_inq_rep_t *) (*inquiry_context);

    if (chp->usage != MGMT_EP_C_INQUIRY_CONTEXT)
    {
        *status = rpc_s_invalid_inquiry_context;
        return;
    }

    if (chp->entry_handle != NULL)
        (*ept_v3_0_c_epv.ept_lookup_handle_free)(chp->ep_binding, &chp->entry_handle, status);
    if (*status == rpc_s_call_cancelled)
        pthread_cancel(pthread_self());

    rpc_binding_free(&chp->ep_binding, status);

    for (i = 0, entp = &(chp->entries[0]); i < chp->num_ents; i++, entp++)
        rpc__ep_mem_free ((pointer_t) entp->tower);

    RPC_MEM_FREE(chp, RPC_C_MEM_INQ_REP); 

    *inquiry_context = NULL;

    *status = rpc_s_ok;
}

/*
**++
**
**  ROUTINE NAME:       rpc_mgmt_ep_unregister
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**   
**  Removes server address information from an endpoint map database.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     
**
**      input_binding
**
**      if_id
**
**      object_uuid
**
**  OUTPUTS:
**
**      status          The result of the operation.
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

PUBLIC void rpc_mgmt_ep_unregister 
#ifdef _DCE_PROTO_
(
    rpc_binding_handle_t    input_binding,
    rpc_if_id_p_t           if_id,
    rpc_binding_handle_t    binding,
    uuid_p_t                object_uuid,
    unsigned32              *status
)
#else
(input_binding, if_id, binding, object_uuid, status)
rpc_binding_handle_t    input_binding;
rpc_if_id_p_t           if_id;
rpc_binding_handle_t    binding;
uuid_p_t                object_uuid;
unsigned32              *status;
#endif
{
    rpc_binding_handle_t        ep_binding;
    rpc_tower_ref_vector_p_t    tower_vec;
    twr_p_t                     tower;
    boolean32                   object_speced;
    uuid_t                      UUID_NIL;
    uuid_t                      *objp;
    error_status_t              tmp_st;
    rpc_if_rep_t                ifspec;

    static rpc_syntax_id_t ndr_syntax_id = {
        {0x8a885d04UL, 0x1ceb, 0x11c9, 0x9f, 0xe8, {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}},
        2
    };
    static rpc_if_rep_t ifspec_template = {
      1, /* ifspec rep version */
      4, /* op count */
      4, /* if version */
      {0}, /* uuid */
      2, /* stub/rt if version */
      {0, NULL}, /* endpoint vector */
      {1, &ndr_syntax_id}, /* syntax vector */
      NULL, /* server_epv */
      NULL /* manager epv */
    };
    
    if (if_id == NULL)
    {
        *status = rpc_s_no_interfaces;
        return;
    }

    if (binding == NULL)
    {
        *status = rpc_s_invalid_binding;
        return;
    }

    /*
     * Get handle to local or remote endpoint mapper
     */
    get_ep_binding(input_binding, &ep_binding, status);
    if (*status != rpc_s_ok)
        return;

    /*
     * Make up an ifspec from the template
     */
    ifspec = ifspec_template;
    ifspec.id = if_id->uuid;
    ifspec.vers = if_id->vers_minor << 16 | if_id->vers_major;

    /*
     * Convert the binding handle to tower_ref vector and then to a tower
     */

    rpc__tower_ref_vec_from_binding(&ifspec, binding, &tower_vec, status);
    if (*status != rpc_s_ok)
    {
        rpc_binding_free(&ep_binding, &tmp_st);
        return;
    }

    rpc__tower_from_tower_ref(tower_vec->tower[0], &tower, status);
    if (*status != rpc_s_ok)
    {
        rpc_binding_free(&ep_binding, &tmp_st);
        rpc__tower_ref_vec_free(&tower_vec, &tmp_st);
        return;
    }

    if (object_uuid == NULL)
    {
        object_speced = false;
        uuid_create_nil(&UUID_NIL, &tmp_st);
        objp = &UUID_NIL;
    }    
    else
    {  
        object_speced = true;
        objp = object_uuid;
    }

    (*ept_v3_0_c_epv.ept_mgmt_delete)
        (ep_binding, object_speced, objp, tower, status);

    if (*status == rpc_s_call_cancelled)
        pthread_cancel(pthread_self());

    rpc__tower_free(&tower, &tmp_st);
    rpc__tower_ref_vec_free(&tower_vec, &tmp_st);
    rpc_binding_free(&ep_binding, &tmp_st);
}


/*
**++
**
**  ROUTINE NAME:       get_ep_binding
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**   
**  Return a binding to an endpoint mapper.
**
**  INPUTS: 
**
**      input_binding   Binding from which we should derive a binding on the
**                      endpoint mapper.  Can be NULL to mean the endpoint
**                      mapper on the local host.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      output_binding  Binding on the endpoint mapper.
**
**      status          The result of the operation.
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

INTERNAL void get_ep_binding
#ifdef _DCE_PROTO_
(
    rpc_binding_handle_t    input_binding,
    rpc_binding_handle_t    *output_binding,
    unsigned32              *status
)
#else
(input_binding, output_binding, status)
rpc_binding_handle_t    input_binding;
rpc_binding_handle_t    *output_binding;
unsigned32              *status;
#endif
{
    rpc_protseq_vector_t    *protseq_vector;
    unsigned_char_t         protseq_str[64];
    unsigned32              slen;
    unsigned32              i;
    unsigned32              tmp_st;


    if (input_binding == NULL)
    {
        /*
         * No binding specified.  Try to bind to the local rpcd using
         * one of the host's supported protseqs.
         */
        rpc_network_inq_protseqs(&protseq_vector, status);
        if (*status != rpc_s_ok)
            return;

        for (i = 0; i < protseq_vector->count; i++)
        {
            slen = strlen((char *) protseq_vector->protseq[i]);
            if ((slen + 2) > sizeof(protseq_str))
                continue;

            rpc__strncpy(protseq_str, protseq_vector->protseq[i], slen);
            rpc__strncpy(&protseq_str[slen], (unsigned_char_t *) ":", 1);

            rpc_binding_from_string_binding(protseq_str, output_binding, status); 
            if (*status == rpc_s_ok)
                break;
        }

        if (i >= protseq_vector->count)
        {
            /*
             * Unable to get a binding to one of the protseqs
             */
            *status = rpc_s_no_protseqs;
        }

        rpc_protseq_vector_free(&protseq_vector, status);
    }
    else
    {
        /*
         * A binding was specified.  Make sure it's the right kind.  Copy it,
         * and reset the copy's endpoint, com timeout setting, and auth info
         */

        RPC_BINDING_VALIDATE_CLIENT((rpc_binding_rep_p_t) input_binding, status);
    
        rpc_binding_copy(input_binding, output_binding, status);
        if (*status != rpc_s_ok)
            return;
        
        rpc_binding_reset(*output_binding, status);
        if (*status != rpc_s_ok)
        { 
            rpc_binding_free(output_binding, &tmp_st);
            return;
        }
    
        rpc_mgmt_set_com_timeout (*output_binding, rpc_c_binding_default_timeout, status);
        if (*status != rpc_s_ok)
        { 
            rpc_binding_free(output_binding, &tmp_st);
            return;
        }

        rpc_binding_set_auth_info (*output_binding, NULL, rpc_c_authn_level_default,
                                    rpc_c_authn_none, NULL, rpc_c_authz_none, status);
        if (*status != rpc_s_ok)
        { 
            rpc_binding_free(output_binding, &tmp_st);
            return;
        }

    }
}

/*
**++
**
**  ROUTINE NAME:       rpc_binding_to_epmap
**
**  SCOPE:              PRIVATE
**
**  DESCRIPTION:
**   
**  Return a binding to an endpoint mapper.
**
**  INPUTS: 
**
**      input_binding   Binding from which we should derive a binding on the
**                      endpoint mapper.  Can be NULL to mean the endpoint
**                      mapper on the local host.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      output_binding  Binding on the endpoint mapper.
**
**      status          The result of the operation.
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
PRIVATE void rpc_binding_to_epmap
#ifdef _DCE_PROTO_
(
    rpc_binding_handle_t    input_binding,
    rpc_binding_handle_t    *output_binding,
    unsigned32              *status
)
#else
(input_binding, output_binding, status)
rpc_binding_handle_t    input_binding;
rpc_binding_handle_t    *output_binding;
unsigned32              *status;
#endif
{
    get_ep_binding(input_binding, output_binding, status);
}

/*
**++
**
**  ROUTINE NAME:       tower_to_if
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**   
**  Convert a tower to an interface ID.
**
**  INPUTS: 
**
**      tower
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:
**
**      if_id
**
**      status          The result of the operation.
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

INTERNAL void tower_to_if_id
#ifdef _DCE_PROTO_
(
    twr_p_t         tower,
    rpc_if_id_t     *if_id,
    unsigned32      *status
)
#else
(tower, if_id, status)
twr_p_t         tower;
rpc_if_id_t     *if_id;
unsigned32      *status;
#endif
{
    rpc_tower_ref_t *tref;
    unsigned32      tmp_st;

    rpc__tower_to_tower_ref(tower, &tref, status);
    if (*status != rpc_s_ok) 
        return;

    if (tref->count < RPC_C_NUM_RPC_FLOORS)
    {
        *status = ept_s_invalid_entry;
        return;
    }

    rpc__tower_flr_to_if_id(tref->floor[0], if_id, status);

    rpc__tower_ref_free(&tref, &tmp_st);
}

/*
**++
**
**  ROUTINE NAME:       rpc_ep_resolve_binding
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**        An application calls the rpc_ep_resolve_binding
**        routine to resolve a partially-bound server binding
**        handle into a fully-bound binding handle.
**
**        Resolving binding handles requires an interface UUID
**        and an object UUID (which may be nil). The RPC runtime
**        requests the RPC daemon (rpcd) on the host specified
**        by the binding argument to look up an endpoint for
**        a compatible server instance. To find the endpoint,
**        the rpcd looks in the endpoint map for the interface
**        UUID from the if_spec argument and the object UUID
**        in the binding argument, if any. (For a discussion
**        on how rpcd locates compatible server instances, see
**        Chapter 11 in the Functional Spec.)
**
**        The functioning of the resolve-binding operation
**        depends on whether the specified binding handle is
**        partially or fully bound. When the client specifies a
**        partially-bound handle, the resolve-binding operation
**        has the following possible outcomes:
**
**        o  If no compatible server instances are registered
**           in the endpoint map, the resolve-binding operation
**           returns the ept_s_not_registered status code.
**
**        o  If a compatible server instance is registered in
**           the endpoint map, the resolve-binding operation
**           returns a fully-bound binding and the rpc_s_ok
**           status code.
**
**        When the client specifies a fully-bound binding
**        handle, the resolve-binding operation returns the
**        specified binding handle and the rpc_s_ok status code.
**        The resolve-binding operation does not contact the
**        rpcd.
**
**        In neither the partially- or fully-bound binding
**        case, does the resolve-binding operation contact a
**        compatible server instance.
**
**
**  INPUTS:
**
**        if_spec       A stub generated data structure specifying the
**                      interface of interest.
**
**  INPUTS/OUTPUTS:     none
**
**        binding       A partially bound server binding handle to resolve to
**                      a fully bound server binding handle.
**
**  OUTPUTS:            
**
**        status        Returns the status code from the resolve binding
**                      operation. This status code is a value that indicates
**                      whether the routine completed successfully and, if
**                      not, why.
**                      rpc_s_ok                        The call completed normally.
**                      ept_s_not_registered            No entries found
**                      rpc_s_invalid_binding           Invalid binding handle
**                      rpc_s_wrong_kind_of_binding     Wrong kind of binding for
**                                                      operation
**                      rpc_s_rpcd_comm_failure         rpc daemon
**                                                      communications failure
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       
**
**      This routine does not check for concurrent use of the
**      binding handle. It is up to the user of this routine to
**      ensure the consistency and correctness of the binding handle
**      by restricting concurrent use of it across multiple threads.
**      This is the same guarantee the user must provide while modifying
**      other binding fields, such as the object UUID, endpoint, and
**      auth info.
**
**--
**/

PUBLIC void rpc_ep_resolve_binding 
#ifdef _DCE_PROTO_
(
    rpc_binding_handle_t  binding_h,
    rpc_if_handle_t       if_spec_h,
    unsigned32            *status
)
#else
(binding_h, if_spec_h, status)
rpc_binding_handle_t  binding_h;
rpc_if_handle_t       if_spec_h;
unsigned32            *status;
#endif
{
    rpc_binding_rep_p_t     binding_rep = (rpc_binding_rep_p_t) binding_h;
    
    CODING_ERROR (status);
    RPC_VERIFY_INIT ();
    RPC_BINDING_VALIDATE_CLIENT (binding_rep, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     * Determine whther the specified binding handle is
     * partially or fully bound.        
     */
    if (!binding_rep->addr_has_endpoint)
    {
        /*
         * The binding is partially bound. Prevent other threads
         * from using this handle while we are trying to resolve
         * it.
         */
        ep_get_endpoint ((rpc_if_rep_p_t) if_spec_h, binding_rep, status);
    }
    else
    {
        /*
         * The binding is fully bound. Just return it.
         */
        *status = rpc_s_ok;
    }
}


/*
**++
**
**  ROUTINE NAME:       ep_get_endpoint
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**
**      Find the correct endpoint information and plug it into the 
**      binding rep. This routine is called by rpc_ep_resolve_binding
**      when the binding is not fully bound.
**
**      We first check the i/f spec to see if the endpoint information 
**      is there to use. If not, we make an RPC call to the endpoint 
**      mapper on the node specified in the binding handle to obtain
**      the endpoint information for the specified interface. Then we 
**      plug in the endpoint (returned from ep_map()) in the binding 
**      handle.
**
**
**  INPUTS:
**
**      if_r            a pointer to the interface spec of interest
**
**  INPUTS/OUTPUTS:     
**
**      binding_r       a pointer to the binding rep which requires an endpoint
**
**  OUTPUTS:            none
**
**      st              The return status of this routine.
**                      rpc_s_ok          The call completed normally.
**                      rpc_s_rpcd_comm_failure         rpc daemon
**                                                      communications failure
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

INTERNAL void ep_get_endpoint 
#ifdef _DCE_PROTO_
(
    rpc_if_rep_p_t          if_r,
    rpc_binding_rep_p_t     binding_r,
    unsigned32              *st
)
#else
(if_r, binding_r, st)
rpc_if_rep_p_t          if_r;
rpc_binding_rep_p_t     binding_r;
unsigned32              *st;
#endif
{
    unsigned32                  temp_status;
    unsigned_char_p_t           endpoint = NULL;
    ept_lookup_handle_t         map_lookup_handle;
    rpc_addr_p_t                *rpc_addr;
    unsigned32                  saved_timeout;
    uuid_t                      saved_object_uuid;
    rpc_tower_ref_vector_p_t    tower_vector = NULL;
    twr_p_t                     map_tower = NULL;
    twr_p_t                     *towers = NULL;
    unsigned32                  num_towers = 0;
    rpc_addr_p_t                tower_rpc_addr = NULL;
    rpc_auth_info_t             *saved_auth_info;
    unsigned32                  tower_to_use;
    unsigned32                  i, j;
    boolean32                   full_restore_flag = true;
    boolean32                   map_lookup;
    boolean32                   free_prot_version = false;
    idl_void_p_t            (*old_allocate) _DCE_PROTOTYPE_ ((unsigned long ));
    idl_void_p_t            (*tmp_allocate) _DCE_PROTOTYPE_ ((unsigned long ));
    void                    (*old_free) _DCE_PROTOTYPE_ ((idl_void_p_t ));
    void                    (*tmp_free) _DCE_PROTOTYPE_ ((idl_void_p_t ));

    CODING_ERROR (st);

    rpc_addr = &binding_r->rpc_addr;

    /*
     * Get the endpoint from the i/f spec, if present.
     */
    rpc__if_set_wk_endpoint (if_r, rpc_addr, st);
    if (*st == rpc_s_ok)
    {
        /*
         * Notify the protocol service that the binding has changed.
         */
        (*rpc_g_protocol_id[binding_r->protocol_id].binding_epv
             ->binding_changed) (binding_r, st);

        if (*st != rpc_s_ok)
        {
            return;
        }

        /*
         * The binding now contains the endpoint in the
         * interface spec.
         */
        binding_r->addr_has_endpoint = true;

        return;
    }

    if (*st != rpc_s_endpoint_not_found)
    {
        return;
    }
    else
    {
        /*
         * Save away the binding object UUID before
         * making the endpoint map call. The endpoint map
         * call must be made with a binding which either
         * has a nil object UUID or the well-known object
         * UUID for this instantiation of the endpoint
         * database.
         */
        rpc_binding_inq_object ((rpc_binding_handle_t) binding_r,
                                &saved_object_uuid,
                                st);
        
        if (*st != rpc_s_ok)
        {
            return;
        }

        /*
         * Save away the com timeout in the binding just in case
         * it's set to infinite. We really don't want to try
         * forever to reach the endpoint mapper.
         */
        rpc_mgmt_inq_com_timeout ((rpc_binding_handle_t) binding_r,
                                  &saved_timeout, st);

        if (*st != rpc_s_ok) 
        {
            return;
        }

        /*
         * Save the auth info on the binding.
         * The call to the endpoint mapper will
         * not be authenticated.
         */
        saved_auth_info = binding_r->auth_info;

        /*
         * Set the binding object to nil.
         */
        rpc_binding_set_object ((rpc_binding_handle_t) binding_r,
                                &uuid_g_nil_uuid,
                                st);

        if (*st != rpc_s_ok)
        {
            goto CLEANUP;
        }

	/*
	 * If the current com timeout is greater than max_timeout
	 * (which means infinite_timeout), then set it to
	 * max_timeout. We really don't want to try forever to
	 * reach the endpoint mapper.
	 */
	if (saved_timeout > rpc_c_binding_max_timeout)
	{
	    rpc_mgmt_set_com_timeout ((rpc_binding_handle_t) binding_r,
				      rpc_c_binding_max_timeout, st);
	    if (*st != rpc_s_ok) 
	    {
		goto CLEANUP;
	    }
	}

        binding_r->auth_info = NULL;
        

        /*
         * Construct a (fully-bound) handle to the Endpoint
         * Mapper at the target node.
         * We do this by taking the partially bound handle
         * and setting the endpoint to the well-known
         * endpoint associated with the EP interface
         * (for the designated protocol sequence).
         */
        rpc__if_set_wk_endpoint ((rpc_if_rep_p_t) ept_v3_0_c_ifspec, 
                                 rpc_addr, 
                                 st);
        if (*st != rpc_s_ok) 
        {
            goto CLEANUP;
        }
        /*
         * If this binding handle doesn't have a protocol version
         * attached to it (probably a string binding), then
         * create one that will insure we will get an endpoint.
         */
        if (binding_r->protocol_version == NULL)
        {
           unsigned8          temp_prot_id;
           unsigned32         temp_vers_major,
                              temp_vers_minor;

           rpc__network_inq_prot_version(binding_r->rpc_addr->rpc_protseq_id,
                                         &temp_prot_id,
                                         &temp_vers_major,
                                         &temp_vers_minor,
                                         st); 

           if (*st != rpc_s_ok)
           {
               goto CLEANUP;
           }

           /*
            * This is ugly, but if we are talking to a 1.0.x rpcd,
            * it wont give us an endpoint if our minor number is
            * higher than the servers.
            */
           rpc__binding_prot_version_alloc(&(binding_r->protocol_version),
                                           temp_vers_major,
                                           0,
                                           st);
           if (*st != rpc_s_ok)
           {
               goto CLEANUP;
           }

           /* remember to free this when we get the endpoint */
           free_prot_version = true;
        }

        /*
         * Notify the protocol service that the binding
         * has changed.
         */
        (*rpc_g_protocol_id[binding_r->protocol_id].binding_epv
         ->binding_changed) (binding_r, st);
        
        if (*st != rpc_s_ok) 
        {
            goto CLEANUP;
        }

        binding_r->addr_has_endpoint = true;
        
        /*
         * In order to lookup entries in the endpoint mapper
         * database we need to provide a map tower which it will
         * use as a lookup key. The important fields in the tower
         * are the interface UUID and version, data rep, protocol
         * sequence and RPC protocol version. To create the map
         * tower the binding given by the client, which contains
         * the correct protocol sequence and the interface spec,
         * which contains the interface UUID and version and data
         * rep(s). This information will be converted into a
         * vector of tower ref structures (one for each data rep).
         * These tower ref structures can then be converted into a
         * real tower, the map tower. The output of the endpoint
         * map will also be a tower containing the desired
         * endpoint. This tower will be converted into an RPC
         * address, from which the endpoint will be extracted.
         * Finally the endpoint will be set in the original
         * binding.
         *
         * NOTE: When logic to iterate through the endpoint
         * mapper database if the presentation negotiation fails
         * is added an iteration will have to be done for each
         * tower ref gotten from the binding,ifspec tuple. For
         * right now only the first tower ref in the vector is
         * looked at and no iteration is done with it.
         */
        rpc__tower_ref_vec_from_binding (if_r,
                                         (rpc_binding_handle_t )binding_r,
                                         &tower_vector, st);

        if (*st != rpc_s_ok)
        {
            goto CLEANUP;
        }

        rpc__tower_from_tower_ref (tower_vector->tower[0],
                                   &map_tower, st);

        if (*st != rpc_s_ok)
        {
            goto CLEANUP;
        }

        /*
         * We want to get *all* of the compatible towers back from
         * the ep map so that we can randomly select one to provide
         * an endpoint to the caller's binding handle. 
         * We get all to the towers by allocating memory to hold 
         * MAX_TOWERS compatible towers. If it turns out that
         * MAX_TOWERS is not sufficient, then we free the towers
         * and try again allowing for MAX_TOWERS x 2 towers. 
         * This continues ( x 3; x 4; ...) until we get them all
         * in one call.
         */
        TRY
        {
        /*
         * The increment i specifies the multiples of MAX_TOWERS
         * towers to get.
         *
         * The map lookup boolean signifies whether we have to try
         * again with a larger towers size.
         *
         * The map lookup handle, which is really a context handle, must be
         * initialized to NULL unless we are iterating.
         */
            for (i = 1, map_lookup = true, map_lookup_handle = NULL;
                 map_lookup == true;
                 ++i)
            {
                RPC_MEM_ALLOC (
                               towers,
                               twr_p_t *,
                               sizeof(twr_p_t) * (MAX_TOWERS * i),
                               RPC_C_MEM_TOWER,
                               RPC_C_MEM_NOWAIT);
		if (towers == NULL){
		    *st = rpc_s_no_memory;
		    goto CLEANUP;
		}
                /*
                 * Set all the pointers in the array to NULL.
                 */
                memset (towers, 0, sizeof(twr_p_t) * (MAX_TOWERS * i));

                /*
                 * Force the stub to use a particular form of memory
                 * allocation.
                 */
               rpc_ss_swap_client_alloc_free (
                   rpc__ep_mem_alloc, rpc__ep_mem_free, 
                   &old_allocate, &old_free);

                (*ept_v3_0_c_epv.ept_map) 
                ((rpc_binding_handle_t) binding_r,
                 &saved_object_uuid,
                 map_tower,
                 &map_lookup_handle,
                 MAX_TOWERS * i,
                 &num_towers,
                 towers,
                 st);

                /*
                 * Restore the memory allocation scheme in effect before 
                 * we got here
                 */
                rpc_ss_swap_client_alloc_free (
                    old_allocate, old_free, 
                    &tmp_allocate, &tmp_free);

                /*
                 * If we got all  of the compatible bindings, we're
                 * done with ep map. 
                 * Otherwise, let's get rid of the ones returned (this was
                 * memory allocated by the stub) and we'll try again.
                 */
                if (map_lookup_handle == NULL)
                {
                    map_lookup = false;
                }
                else
                {
                    for (j = 0; j < num_towers; ++j)
                    {
                        rpc__ep_mem_free ((pointer_t) towers[j]);
                    }
                    
                    RPC_MEM_FREE (towers, RPC_C_MEM_TOWER);
                    
                    /*
                     * So we'll know what to do for cleanup.
                     */
                    towers = NULL;

                    /*
                     * Free the lookup handle at rpcd.
                     */
                    (*ept_v3_0_c_epv.ept_lookup_handle_free)
                    ((rpc_binding_handle_t) binding_r,
                     &map_lookup_handle,
                     &temp_status);
                    
                    if (temp_status == rpc_s_call_cancelled)
                        pthread_cancel(pthread_self());

                    /*
                     * We'll ignore any problem in trying to free the handle
                     * (for example, can't reach rpcd).
                     */
#ifdef DEBUG
                    if (temp_status != rpc_s_ok)
                    {
                        RPC_DBG_PRINTF (rpc_e_dbg_general, 1,
                                        ("(ep_get_endpoint) call_rep->none binding_rep->%x ept_lookup_handle_free returned %x\n", 
                                         binding_r, temp_status));
                    }
#endif
                }
            } 
        }
        CATCH_ALL
        {
            /*
             * Report any exception as:
             */
            *st = rpc_s_rpcd_comm_failure;
        }
        ENDTRY

        /*
	 * If successful, the binding currently contains the rpcd
	 * boot time. To avoid a "wrong boot time" error over dg
	 * when the returned binding (with selected endpoint) is
	 * used to make a rpc, reset the binding (before adding
	 * the server endpoint) to clear the boot time.
	 *
	 * Otherwise, the binding may contain the rpcd boot time.
	 * To avoid an erroneous server bound over dg when the
	 * returned binding (without selected endpoint) is used
	 * to make a rpc, reset the binding to clear the boot
	 * time.
	 */
	rpc_binding_reset ((rpc_binding_handle_t) binding_r, &temp_status);

        if (*st != rpc_s_ok || (*st = temp_status) != rpc_s_ok)
        {
            goto CLEANUP;
        }

        /*
         * From the returned compatible towers, randomly pick one.
         */
        tower_to_use = RPC_RANDOM_GET (0, num_towers - 1);

        rpc__naf_tower_flrs_to_addr (
            &(towers[tower_to_use]->tower_octet_string[0]),
            &tower_rpc_addr,
            st);

        if (*st != rpc_s_ok)
        {
            goto CLEANUP;
        }

        rpc__naf_addr_inq_endpoint (tower_rpc_addr, &endpoint, st);

        if (*st != rpc_s_ok)
        {
            goto CLEANUP;
        }
        rpc__naf_addr_set_endpoint (endpoint, rpc_addr, st);

        if (*st != rpc_s_ok)
        {
            goto CLEANUP;
        }

        /*
         * Resetting the binding also reset the has-endpoint flag.
         * We need to turn it back on.
         */
        binding_r->addr_has_endpoint = true;

#ifdef DEBUG
        RPC_DBG_PRINTF (rpc_e_dbg_general, 1,
                        ("(ep_get_endpoint) call_rep->none binding_rep->%x endpoint mapper returned %s\n", 
                         binding_r, endpoint));
#endif        

        /*
         * Notify the protocol service that the binding has changed.
         */
        (*rpc_g_protocol_id[binding_r->protocol_id].binding_epv
         ->binding_changed) (binding_r, st);

        if (*st != rpc_s_ok)
        {
            goto CLEANUP;
        }

        full_restore_flag = false;
    }
  
CLEANUP:

    /* 
     * If an error occurred, restore the caller's binding back to 
     * its original condition before returning. 
     */
    if (full_restore_flag)
    {
        binding_r->addr_has_endpoint = false;
        rpc__naf_addr_set_endpoint ((unsigned_char_p_t) "", rpc_addr, 
            &temp_status);
    }

    if (towers != NULL)
    {
        for (i = 0; i < num_towers; ++i)
        {
            if (towers[i] != NULL)
            {
                rpc__ep_mem_free ((pointer_t) towers[i]);
            }
        }

        RPC_MEM_FREE (towers, RPC_C_MEM_TOWER);
    }

    /*
     * Free allocated protocol version
     */
    if (free_prot_version == true)
    {
        rpc__binding_prot_version_free (&(binding_r->protocol_version));
    }

    /*
     * Restore the saved auth info.
     */
    binding_r->auth_info = saved_auth_info;

    /*
     * Restore the saved binding com timeout.
     */
    rpc_mgmt_set_com_timeout ((rpc_binding_handle_t) binding_r,
                              saved_timeout, &temp_status);

    if (temp_status != rpc_s_ok)
    {
        goto EXIT;
    }

    /*
     * Restore the saved binding object UUID.
     */
    rpc_binding_set_object ((rpc_binding_handle_t) binding_r,
                            &saved_object_uuid, &temp_status);

    if (temp_status != rpc_s_ok)
    {
        goto EXIT;
    }

    if (map_tower != NULL)
    {
        rpc__tower_free (&map_tower, &temp_status);

        if (temp_status != rpc_s_ok)
        {
            goto EXIT;
        }
    }

    if (tower_vector != NULL)
    {
        rpc__tower_ref_vec_free (&tower_vector, &temp_status);

        if (temp_status != rpc_s_ok)
        {
            goto EXIT;
        }
    }

    if (tower_rpc_addr != NULL)
    {
        rpc__naf_addr_free (&tower_rpc_addr, &temp_status);
        
        if (temp_status != rpc_s_ok)
        {
            goto EXIT;
        }
    }

    if (endpoint != NULL)
    {
        rpc_string_free (&endpoint, &temp_status);

        if (temp_status != rpc_s_ok)
        {
            goto EXIT;
        }
    }


EXIT:
    if (*st != rpc_s_ok)
    {
        return;
    }
    else
    {
        *st = temp_status;
        return;
    }
}

/*
**++
**
**  ROUTINE NAME:       rpc__ep_mem_alloc
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**      
**  Wrapper around RPC_MEM_ALLOC to use in call to
**  rpc_ss_swap_client_alloc_free.
**
**  INPUTS:
**
**      size            number of bytes to allocate
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     idl_void_p_t
**
**      return          pointer to allocated storage
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL idl_void_p_t rpc__ep_mem_alloc 
#ifdef _DCE_PROTO_
(
    unsigned long           size
)
#else
(size)
unsigned long           size;
#endif
{
    idl_void_p_t             ptr;


    RPC_MEM_ALLOC (
        ptr,
        idl_void_p_t,
        size,
        RPC_C_MEM_TOWER,
        RPC_C_MEM_WAITOK);

    return (ptr);
}

/*
**++
**
**  ROUTINE NAME:       rpc__ep_mem_free
**
**  SCOPE:              INTERNAL
**
**  DESCRIPTION:
**      
**  Wrapper around RPC_MEM_FREE to use in call to
**  rpc_ss_swap_client_alloc_free.
**
**  INPUTS:
**
**      ptr             storage to free
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
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

INTERNAL void rpc__ep_mem_free 
#ifdef _DCE_PROTO_
(
  pointer_t            ptr
)
#else
(ptr)
pointer_t            ptr;
#endif
{
    RPC_MEM_FREE (ptr, RPC_C_MEM_TOWER);
}
