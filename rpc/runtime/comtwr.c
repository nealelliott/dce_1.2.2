/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comtwr.c,v $
 * Revision 1.1.510.2  1996/02/18  00:02:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:55:32  marty]
 *
 * Revision 1.1.510.1  1995/12/08  00:18:44  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/2  1995/11/20  22:40 UTC  jrr
 * 	Merge second XIDL drop for DCE 1.2.1
 * 	[1995/11/17  17:10 UTC  dat  /main/dat_xidl2/1]
 * 
 * 	Fix parameter prototypes
 * 	[1995/02/23  20:57:09  bfc]
 * 	 *
 * 
 * 	Make changes in rpc_binding_vec_from_twr_vec() recommended by IDL group
 * 	1995/01/05.  Add routines rpc_twr_vec_from_binding_vec() and
 * 	rpc_binding_vec_from_twr_vec()
 * 
 * 	Make changes in rpc_binding_vec_from_twr_vec() recommended
 * 	by IDL group 1995/01/05   08:40:00   burton
 * 	Add routines rpc_twr_vec_from_binding_vec() and
 * 	rpc_binding_vec_from_twr_vec()
 * 	1994/12/17  11:24:00  burton
 * 	[1995/01/16  16:11:06  bfc]
 * 
 * 	DCE for DEC OSF/1: populate from OSF DCE R1.1
 * 	[1995/01/16  16:07:51  bfc]
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:31 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:14 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:18 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/07  23:58:25  root]
 * 
 * Revision 1.1.508.1  1994/01/21  22:36:00  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:55:53  cbrooks]
 * 
 * Revision 1.1.4.1  1993/09/28  13:20:02  tatsu_s
 * 	Bug 7164 - Return the error status rpc_s_no_interfaces from
 * 	rpc_tower_vector_from_binding() if the interface is not
 * 	specified.
 * 	[1993/09/27  16:04:38  tatsu_s]
 * 
 * Revision 1.1.2.5  1993/01/03  23:22:56  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:03:11  bbelch]
 * 
 * Revision 1.1.2.4  1992/12/23  20:45:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:34:54  zeliff]
 * 
 * Revision 1.1.2.3  1992/12/21  19:20:32  wei_hu
 * 	      22-oct-92   dm      Fix improperly declared temp_status in
 * 	                          rpc_tower_vector_from_binding and remove
 * 	                          unnecessary cleanup.
 * 	[1992/12/18  20:27:42  wei_hu]
 * 
 * Revision 1.1.2.2  1992/10/20  13:37:35  markar
 * 	       OT CR 5667 fix: Free memory created as part of vector_from_binding call
 * 	     [1992/10/19  15:19:02  markar]
 * 
 * Revision 1.1  1992/01/19  03:10:00  devrcs
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
**    COMTWR.C
**
**  FACILITY:
**
**    Remote procedure call (RPC) 
**  
**  ABSTRACT:
**
**  Set of routines supporting operations that create and manipulate the
**  canonical representation of a protocol tower.
**
**
**/

#include <commonp.h>    /* Private defs for Common component            */
#include <com.h>        /* Private COM defs for other RPC components    */
#include <comp.h>       /* Privates for COM Services component          */
#include <comtwr.h>     /* Private tower defs                           */
#include <comtwrref.h>  /* Private tower ref defs for other RPC components */


/*
**++
**  ROUTINE NAME:       rpc__tower_free
**
**  SCOPE:              PRIVATE - declared in comtwr.h
**
**  DESCRIPTION:
**
**  Releases memory used by a tower that was dynamically created.
**  Towers are dynamically created by calling the 
**  rpc__tower_from_tower_ref routine.
**
**  INPUTS:             none
**
**  INPUT/OUTPUTS:          
**
**      tower           Canonical representation of a protocol tower.
**                      Nulled on return.
**
**  OUTPUTS:            
**
**      status          Returns the status code from the free
**                      operation. This status code indicates whether the 
**                      routine completed successfully and, if not, why.
**                      Returns rpc_s_ok.
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
*/

PRIVATE void rpc__tower_free 
#ifdef _DCE_PROTO_
(
    twr_p_t             *tower,
    unsigned32          *status
)
#else
(tower, status)
twr_p_t             *tower;
unsigned32          *status;
#endif
{
    CODING_ERROR (status);

    RPC_MEM_FREE (*tower, RPC_C_MEM_TOWER);
    *tower = NULL;

    *status = rpc_s_ok;
    return;
}


/*
**++
**  ROUTINE NAME:       rpc__tower_from_tower_ref
**
**  SCOPE:              PRIVATE - declared in comtwr.h
**
**  DESCRIPTION:
**
**  This routine creates the canonical representation of a protocol tower 
**  from a runtime reference representation of a protocol tower. 
**
**  INPUTS:             
**
**      tower_ref       Runtime reference representation of a protocol tower.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      tower           Returns a pointer to the canonical representation
**                      of a protocol tower.
**
**      status          Returns the status code from the
**                      tower from tower_ref operation. This status code 
**                      indicates whether the routine completed
**                      successfully and, if not, why.
**                      Returns 
**                          rpc_s_ok
**                      or status from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      void
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE void rpc__tower_from_tower_ref 
#ifdef _DCE_PROTO_
(
    rpc_tower_ref_p_t       tower_ref,
    twr_p_t                 *tower,
    unsigned32              *status
)
#else
(tower_ref, tower, status)
rpc_tower_ref_p_t       tower_ref;
twr_p_t                 *tower;
unsigned32              *status;
#endif
{
    byte_p_t                tower_p;
    unsigned16              twr_rep_16;
    unsigned32              i, 
                            floor_size,
                            octet_length;


    /* 
     * Calculate the size of the tower octet string.
     */
    for (i = 0, octet_length = 0;
         i < tower_ref->count; i++)
    {
        octet_length += (tower_ref->floor[i]->prot_id_count +
                         RPC_C_TOWER_FLR_LHS_COUNT_SIZE +
                         tower_ref->floor[i]->address_count +
                         RPC_C_TOWER_FLR_RHS_COUNT_SIZE);
    }

    octet_length += RPC_C_TOWER_FLR_COUNT_SIZE;

    /*
     * Allocate the tower structure to hold the 
     * canonical representation of the tower.
     */
    RPC_MEM_ALLOC ( 
        *tower, 
        twr_p_t, 
        sizeof (twr_t) + octet_length - 1,
        RPC_C_MEM_TOWER, RPC_C_MEM_WAITOK );
    if (*tower == NULL){
	*status = rpc_s_no_memory;
	return;
    }

    /*
     * Initialize the tower length in the tower structure.
     */
    (*tower)->tower_length = octet_length;

    /*
     * Form the tower octet string starting
     * with the tower floor count size.
     */
    tower_p = (*tower)->tower_octet_string;

    /*
     * Convert the tower count to little endian 
     * and copy it to the octet string.
     */
    twr_rep_16 = tower_ref->count;
    RPC_RESOLVE_ENDIAN_INT16 (twr_rep_16);
    memcpy ((char *)tower_p, 
            (char *)&twr_rep_16, RPC_C_TOWER_FLR_COUNT_SIZE);


    tower_p += RPC_C_TOWER_FLR_COUNT_SIZE;

    /*
     * And now copy each tower floor to the octet string.
     */
    for (i=0; i < tower_ref->count; i++)
    {
        floor_size = 
            RPC_C_TOWER_FLR_LHS_COUNT_SIZE  + 
            tower_ref->floor[i]->prot_id_count +
            RPC_C_TOWER_FLR_RHS_COUNT_SIZE  +
            tower_ref->floor[i]->address_count;

        memcpy ((char *)tower_p, (char *)tower_ref->floor[i]->octet_string,
                floor_size);

        tower_p += floor_size;
    }

    *status = rpc_s_ok;
    return;
}


/*
**++
**  ROUTINE NAME:       rpc__tower_to_tower_ref
**
**  SCOPE:              PRIVATE - declared in comtwr.h
**
**  DESCRIPTION:
**
**  Creates a runtime reference representation of a protocol tower 
**  from a canonical representation of a protocol tower.
**
**  INPUTS:             
**
**      tower           Canonical representation of a protocol tower.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      tower_ref       Returns a pointer to a reference representation 
**                      of a protocol tower.
**      status          Returns the status code from the tower to tower ref
**                      operation. This status code indicates whether the 
**                      routine completed successfully and, if not, why.
**                      Returns
**                          rpc_s_ok
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

PRIVATE void rpc__tower_to_tower_ref 
#ifdef _DCE_PROTO_
(
    twr_p_t             tower,
    rpc_tower_ref_p_t   *tower_ref,
    unsigned32          *status
)
#else
(tower, tower_ref, status)
twr_p_t             tower;
rpc_tower_ref_p_t   *tower_ref;
unsigned32          *status;
#endif
{
    unsigned16              floor_count;


    CODING_ERROR (status);

    /*
     * Get the tower floor count and correct for proper endian.
     */
    memcpy ((char *) &floor_count, (char *) tower->tower_octet_string, 
            RPC_C_TOWER_FLR_COUNT_SIZE);
    RPC_RESOLVE_ENDIAN_INT16 (floor_count);

    /*
     * Allocate and initialize the tower reference structure to be returned.
     */
	rpc__tower_ref_alloc (tower->tower_octet_string, floor_count, 1, 
        tower_ref, status);

    /*
     * Return status from the tower ref allocate operation.
     */
    return;
}


/*
**++
**  ROUTINE NAME:       rpc_tower_to_binding
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**  Creates a binding handle from a canonical representation of a 
**  protocol tower.  After the caller is finished with the binding, the
**  rpc_binding_free routine must be called to release the memory used
**  by the binding.
**
**  Note, this is an SPI routine - available to outside the runtime,
**  but only to other DCE components.
**
**  INPUTS:             
**
**      prot_tower          A single protocol tower (DNA$Tower attribute value)
**                          to convert to a binding handle.
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:            
**
**      binding             Returns a binding handle.
**      status              Returns the status code from the tower-to-binding
**                          operation. This status code indicates whether the 
**                          routine completed successfully and, if not, why.
**                          Returns
**                              rpc_s_ok
**                          or status from a called routine.
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

PUBLIC void rpc_tower_to_binding 
#ifdef _DCE_PROTO_
(
    byte_p_t                prot_tower,
    rpc_binding_handle_t    *binding,
    unsigned32              *status
)
#else
(prot_tower, binding, status)
byte_p_t                prot_tower;
rpc_binding_handle_t    *binding;
unsigned32              *status;
#endif
{
    rpc_binding_rep_p_t     binding_rep;
    rpc_protocol_id_t       prot_id;
    rpc_addr_p_t            rpc_addr;


    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    /*
     * Null binding in case of error before finishing.
     */
    *binding = NULL;

    /*
     * Obtain an RPC address for the tower.
     */
    rpc__naf_tower_flrs_to_addr (prot_tower, &rpc_addr, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    prot_id = RPC_PROTSEQ_INQ_PROT_ID(rpc_addr->rpc_protseq_id);

    /*
     * Allocate and initialize a binding rep.
     */
    binding_rep = rpc__binding_alloc 
                    (false, &uuid_g_nil_uuid, prot_id, rpc_addr, status);

    /*
     * Return binding handle to user.
     */
    *binding = (rpc_binding_handle_t) binding_rep;

    /*
     * Return status from rpc__binding_alloc
     */
    return;

}


/*
**++
**  ROUTINE NAME:       rpc_tower_vector_from_binding
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**  Creates a vector of twr_t's from a binding handle. After the caller is
**  finished with the tower vector, the rpc_tower_vector_free routine must be
**  called to release the memory used by the vector. 
**
**  Note, this is an SPI routine - available to outside the runtime,
**  but only to other DCE components.
**
**  INPUTS:             
**
**      if_spec             Interface spec to combine with a binding
**                          handle to form a tower vector.
**
**      binding             Binding handle to combine with an interface 
**                          spec to form a tower vector.
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:            
**
**      tower_vector        Returns an allocated tower vector.
**
**      status              Returns the status code from the 
**                          tower-vector-from-binding operation. 
**                          This status code indicates whether the 
**                          routine completed successfully and, if not, why.
**                          Returns
**                              rpc_s_ok
**                              rpc_s_no_interfaces
**                          or status from a called routine.
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

PUBLIC void rpc_tower_vector_from_binding 
#ifdef _DCE_PROTO_
(
    rpc_if_handle_t         if_spec,
    rpc_binding_handle_t    binding,
    rpc_tower_vector_p_t    *twr_vector,
    unsigned32              *status
)
#else
(if_spec, binding, twr_vector, status)
rpc_if_handle_t         if_spec;
rpc_binding_handle_t    binding;
rpc_tower_vector_p_t    *twr_vector;
unsigned32              *status;
#endif
{
    rpc_tower_ref_vector_t  *tower_ref_vector;
    int                      i;
    unsigned32               temp_status;

    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    /*
     * Null the twr_vector in case of error before finishing.
     */
    *twr_vector = NULL;

    if (if_spec == NULL)
    {
        *status = rpc_s_no_interfaces;
        return;
    }

    /*
     * Convert the binding to a vector of tower refs.
     */

    rpc__tower_ref_vec_from_binding ((rpc_if_rep_p_t)if_spec, binding, 
        &tower_ref_vector, status);

    if (*status != rpc_s_ok)
    {
        /*
         * No need to goto CLEANUP; since a tower_ref_vector wasn't
         * returned to us.
         */
        return;
    }

    /*
     * Allocate a rpc_tower_vector_t based on the number of returned
     * tower refs.
     */
    RPC_MEM_ALLOC ( 
        *twr_vector, 
        rpc_tower_vector_p_t, 
        sizeof (rpc_tower_vector_t) + (tower_ref_vector->count - 1) * 
            sizeof (twr_p_t),
        RPC_C_MEM_TOWER_VECTOR,
        RPC_C_MEM_WAITOK );
    if (*twr_vector == NULL){
	*status = rpc_s_no_memory;
	goto CLEANUP;
    }

    (*twr_vector)->count = tower_ref_vector->count;

    /*
     * For each returned tower ref convert the tower ref to a twr_t and
     * store the twr_t in the rpc_tower_vector_t.
     */
    for (i = 0; i < tower_ref_vector->count; i++)
    {
        rpc__tower_from_tower_ref (tower_ref_vector->tower[i],
            &(*twr_vector)->tower[i], status);

        if (*status != rpc_s_ok)
        {
            RPC_MEM_FREE (*twr_vector, RPC_C_MEM_TOWER_VECTOR);

            goto CLEANUP;
        }
    }
   
CLEANUP:
    /*
     * Free the tower_ref_vector returned from 
     * rpc__tower_ref_vec_from_binding().
     */
    rpc__tower_ref_vec_free (&tower_ref_vector, &temp_status);

    /*
     * If we got this far successfully, return whatever the result from 
     * rpc__tower_ref_vec_free(). Otherwise, return the previous error
     * in status.
     */
    if (*status == rpc_s_ok)
    {
        *status = temp_status;
    }

    return;
}

/*
**++
**  ROUTINE NAME:       rpc_tower_vector_free
**
**  SCOPE:              PRIVATE - declared in comtwrref.h
**
**  DESCRIPTION:
**
**  Releases memory associated with a tower vector, 
**  including the towers as well as the vector.
**
**  INPUTS:             none
**
**  INPUT/OUTPUTS:          
**
**      twr_vector      The tower vector to free. Nulled on return.
**
**  OUTPUTS:
**
**      status          Returns the status code from the tower free
**                      operation. This status code is a value that
**                      indicates whether the routine completed
**                      successfully and, if not, why.
**                      Returns 
**                          rpc_s_ok 
**                          or status from a called routine.
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

PRIVATE void rpc_tower_vector_free 
#ifdef _DCE_PROTO_
(
    rpc_tower_vector_p_t    *twr_vector,
    unsigned32              *status
)
#else
(twr_vector, status)
rpc_tower_vector_p_t    *twr_vector;
unsigned32              *status;
#endif
{
    unsigned32      i;


    CODING_ERROR (status);

    /*
     * Free each tower reference in the vector.  
     */
    for (i=0; i < (*twr_vector)->count; i++)
    {
        rpc__tower_free (&((*twr_vector)->tower[i]), status);
        if (*status != rpc_s_ok)
        {
            return;
        }
    }

    /*
     * Free the tower vector structure and set pointer to NULL.
     */
    RPC_MEM_FREE (*twr_vector, RPC_C_MEM_TOWER_VECTOR);

    *twr_vector = NULL;

    *status = rpc_s_ok;
    return;
}


/*
**++
**  ROUTINE NAME:       rpc_twr_vec_from_binding_vec
**
**  SCOPE:              PUBLIC - declared in rpcpvtsg.idl
**
**  DESCRIPTION:
**
**  Creates a vector of twr_t's from a binding vector. After the caller is
**  finished with the tower vector, the rpc_tower_vector_free routine must be
**  called to release the memory used by the vector. 
**
**  Note, this is an SPI routine - available to outside the runtime,
**  but only to other DCE components.
**
**  INPUTS:             
**
**      binding             Binding handle to convert into a 
**                          tower vector.
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:            
**
**      tower_vector        Allocated and returned tower vector.
**
**      status              Returns the status code from the 
**                          tower-vector-from-binding operation. 
**                          This status code indicates whether the 
**                          routine completed successfully and, if not, why.
**                          Returns
**                              rpc_s_ok
**                              rpc_s_no_interfaces
**                          or status from a called routine.
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

PUBLIC void rpc_twr_vec_from_binding_vec
#ifdef _DCE_PROTO_
(
    rpc_binding_vector_t    *binding_vec,
    rpc_tower_vector_p_t    *twr_vector,
    unsigned32              *status
)
#else
(binding_vec, twr_vector, status)
rpc_binding_vector_t    *binding_vec;
rpc_tower_vector_p_t    *twr_vector;
unsigned32              *status;
#endif
{
    unsigned16              lower_flr_count;
    unsigned32              i;
    unsigned32               temp_status;
    rpc_binding_rep_p_t     binding_rep;
    rpc_tower_ref_vector_p_t    tower_vector;
    rpc_tower_floor_p_t     tower_floor;
    twr_p_t                 lower_floors;


    CODING_ERROR (status);
    RPC_VERIFY_INIT ();
    
    /*
     * check to see if binding_vec is NULL, and if so,
     * return with an error status
     */
    if (binding_vec == NULL)
    {
        *status = rpc_s_invalid_arg;
        return;
    }
    #define ONE_REF_PTR 1
    #define RPC_FLOOR_THREE 1
    /*
     * Create the tower ref vector.
     */
    RPC_MEM_ALLOC ( 
        tower_vector,
        rpc_tower_ref_vector_p_t,
        sizeof (rpc_tower_ref_vector_t) + 
        ((ONE_REF_PTR -1) * (sizeof (rpc_tower_ref_p_t))) ,
        RPC_C_MEM_TOWER_REF_VECTOR,
        RPC_C_MEM_WAITOK );

    /*
     * Initialize tower count.
     */
    (tower_vector)->count = 0;
    /*
     * Allocate a rpc_tower_vector_t based on the number of bindings
     * in the binding vector.  One tower in the tower vector will be
     * added for each binding in the binding vector.
     */
    RPC_MEM_ALLOC ( 
        *twr_vector, 
        rpc_tower_vector_p_t, 
        sizeof (rpc_tower_vector_t) + ((binding_vec)->count - 1) * 
            sizeof (twr_p_t),
        RPC_C_MEM_TOWER_VECTOR,
        RPC_C_MEM_WAITOK );

    (*twr_vector)->count = 0;

    /*
     * Convert each binding in the binding vector
     * to a tower with lower floors only
     */
    for (i = 0; i < (binding_vec)->count; i++)
    {
        if ((binding_vec)->binding_h[i] != NULL)
        {
	    /*
	     * Obtain the rpc address of this binding.
	     */
	    binding_rep = (rpc_binding_rep_p_t) (binding_vec)->binding_h[i];
	    /*
	     * Create the lower tower floors.
	     */
	    rpc__naf_tower_flrs_from_addr (binding_rep->rpc_addr,
					   &lower_floors, status);
	    if (*status != rpc_s_ok)
	    {
		return;
	    }
	    /*
	     * Initialize the tower vector with the pointer
	     * to the lower floors.
	     */
	    (tower_vector)->lower_flrs = lower_floors;

	    /*
	     * Get the number of lower tower floors returned and
	     * convert to host's representation.
	     */
	    memcpy ((char *)&lower_flr_count, 
		    (char *)lower_floors->tower_octet_string,
		    RPC_C_TOWER_FLR_COUNT_SIZE);
	    
	    RPC_RESOLVE_ENDIAN_INT16 (lower_flr_count);

	    /*
	     * Allocate the tower reference structure.
	     * The number of floors is equal to one RPC floor - 3
	     * plus the number of network (lower) floors.
	     */
	    rpc__tower_ref_alloc (lower_floors->tower_octet_string, 
		RPC_FLOOR_THREE + lower_flr_count, 
		RPC_FLOOR_THREE+1, &((tower_vector)->tower[0]), status);
	    
	    if (*status != rpc_s_ok)
	    {
		return;
	    }
	    /*
	     * Create tower floor 3 from the RPC protocol id.
	     */
	    rpc__tower_flr_from_rpc_prot_id (
		    binding_rep->rpc_addr->rpc_protseq_id, 
		    binding_rep->protocol_version, &tower_floor, status);
	    if (*status != rpc_s_ok)
	    {
		return;
	    }

	    /*
	     * Add floor 3 to the tower - at level 1
             * since we are skipping floors 1 and 2.
	     */
	    rpc__tower_ref_add_floor (1, tower_floor,
				      (tower_vector)->tower[0], status);
	    if (*status != rpc_s_ok)
	    {
		return;
	    }
	    /*
	     * For the returned tower ref convert the tower ref to a twr_t and
	     * store the twr_t in the rpc_tower_vector_t.
	     */
	    rpc__tower_from_tower_ref (tower_vector->tower[0], 
				       &(*twr_vector)->tower[i],
				       status);
	    if (*status != rpc_s_ok)
	    {
		return;
	    }
	    /*
	     * Increment the number of towers in the vector.
	     */
	    (*twr_vector)->count++;
        }
    }
    /*
     * Free the tower ref vector.
     */
    rpc__tower_ref_vec_free (&tower_vector, &temp_status);
}

/*
**++
**  ROUTINE NAME:       rpc_binding_vec_from_twr_vec
**
**  SCOPE:              PUBLIC - declared in rpcpvt.idl
**
**  DESCRIPTION:
**
**  Creates a vector of binding handles from the input vector
**  of protocol towers.
**
**  Note, this is an SPI routine - available to outside the runtime,
**  but only to other DCE components.
**
**  INPUTS:             
**
**      twr_vector        The tower vector to convert to bindings.
**
**  INPUT/OUTPUTS:          none
**
**  OUTPUTS:            
**
**      binding_vec         Binding vector of binding handles which
**                          represent the input vector of towers.
**
**      status              Returns the status code from the 
**                          tower-vector-from-binding operation. 
**                          This status code indicates whether the 
**                          routine completed successfully and, if not, why.
**                          Returns
**                              rpc_s_ok
**                              rpc_s_no_interfaces
**                          or status from a called routine.
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

PUBLIC void rpc_binding_vec_from_twr_vec
#ifdef _DCE_PROTO_
(
    rpc_tower_vector_t      *twr_vector,
    rpc_binding_vector_p_t  *binding_vec,
    unsigned32              *status
)
#else
(twr_vector, binding_vec, status)
rpc_tower_vector_t      *twr_vector;
rpc_binding_vector_p_t  *binding_vec;
unsigned32              *status;
#endif
{
    byte_p_t                prot_tower;
    rpc_binding_rep_p_t     binding_rep;
    rpc_protocol_id_t       prot_id;
    rpc_addr_p_t            rpc_addr;
    unsigned32              i;


    CODING_ERROR (status);
    RPC_VERIFY_INIT ();

    /*
     * set the binding vector pointer to NULL to anticipate failure
     */
    *binding_vec = NULL;

    /*
     * create a binding vector to accomodate the max vector size bindings
     */
    RPC_MEM_ALLOC (
        *binding_vec,
        rpc_binding_vector_p_t,
        sizeof (rpc_binding_vector_t) + (((twr_vector)->count) *
        sizeof (rpc_binding_handle_t)),
        RPC_C_MEM_BINDING_VEC,
        RPC_C_MEM_WAITOK);

    /*
     * initialize the count field in the binding vector.
     */
    (*binding_vec)->count = 0;

    /*
     * initialize the binding vector by setting each entry to NULL
     */
    for (i = 0; i < (twr_vector)->count; i++)
    {
        (*binding_vec)->binding_h[i] = NULL;
    }

    for (i = 0; i < (twr_vector)->count; i++)
    {
	/*
         * Get a tower octet string and convert to a RPC address
         */
	prot_tower = ((twr_vector)->tower[i])->tower_octet_string;

	rpc__naf_tower_flrs_to_addr (prot_tower, &rpc_addr, status);

        if (*status == rpc_s_protseq_not_supported)
        {
            continue;
        }

	if (*status != rpc_s_ok)
	{
	    return;
	}
	/*
         * Get the protocol ID for this tower
         */
	prot_id = RPC_PROTSEQ_INQ_PROT_ID(rpc_addr->rpc_protseq_id);

	/*
	 * Allocate and initialize a binding rep.
	 */
	binding_rep = rpc__binding_alloc 
                    (false, &uuid_g_nil_uuid, prot_id, rpc_addr, status);
/*
	(*binding_vec)->binding_h[i] = (rpc_binding_handle_t) binding_rep;
*/
	(*binding_vec)->binding_h[(*binding_vec)->count] =
                                            (rpc_binding_handle_t) binding_rep;

	(*binding_vec)->count++;
    }
}

