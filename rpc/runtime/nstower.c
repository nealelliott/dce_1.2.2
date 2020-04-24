/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nstower.c,v $
 * Revision 1.1.333.2  1996/02/18  00:05:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:15  marty]
 *
 * Revision 1.1.333.1  1995/12/08  00:21:42  root
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
 * 	[1995/12/08  00:00:13  root]
 * 
 * Revision 1.1.331.1  1994/01/21  22:38:48  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:26  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:54:12  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:11:02  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:14:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:42:48  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:12:01  devrcs
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
**    NSTOWER.C
**
**  FACILITY:
**
**    Remote procedure call (RPC) name service independent interface (NS)
**  
**  ABSTRACT:
**
**  Set of routines supporting name service operations on 
**  canonical representation of protocol towers.
**
**
**/

#include <commonp.h>    /* Common internals for RPC Runtime system      */
#include <com.h>        /* Externals for Common Services component      */
#include <ns.h>         /* Private NS defs for other RPC components     */
#include <nsp.h>        /* Private defs for Naming Service component    */
#include <nsutil.h>     /* Private name service utility routines        */
#include <nsuuid.h>     /* Private nsuuid defs                          */
#include <nstower.h>    /* Private nstower defs                         */
#include <nsentry.h>    /* Externals for NS Entry sub-component         */
#include <nsattr.h>     /* Private routines for NS attributes           */


/*
**++
**  ROUTINE NAME:       rpc__tower_delete 
**
**  SCOPE:              PRIVATE - declared in nstower.h
**
**  DESCRIPTION:        
**
**  Delete a name-service entry protocol tower set from an RPC entry
**
**  INPUTS:             
**
**      nsentry         Name service representation of an entry name from
**                      which to delete the towers set.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      status          Returns the status code from the delete
**                      operation. This status code indicates whether the 
**                      routine completed successfully and, if not, why.
**                      Returns status from a called routine.
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

PRIVATE void rpc__tower_delete 
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

    rpc__nsattr_delete (nsentry, RPC_C_ATTR_DNA_TOWERS, status);

    return;
}


/*
**++
**  ROUTINE NAME:       rpc__tower_mbr_add
**
**  SCOPE:              PRIVATE - declared in nstower.h
**
**  DESCRIPTION:        
**
**  Adds a protocol tower to a name-service entry protocol tower set. If
**  necessary, creates the attribute.
**
**  INPUTS:             
**
**      nsentry         Name service entry name to receive protocol tower.
**
**      tower           Canonical representation of protocol tower to store 
**                      in nsentry's tower set.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      status          Returns the status code from the add-member
**                      operation. This status code indicates whether the 
**                      routine completed successfully and, if not, why.
**                      Returns status from rpc__nsattr_add_value().
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

PRIVATE void rpc__tower_mbr_add 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t         nsentry,
    twr_p_t                 tower,
    unsigned32              *status
)
#else
(nsentry, tower, status)
rpc_nsentry_p_t         nsentry;
twr_p_t                 tower;
unsigned32              *status;
#endif
{
    unsigned32      temp_status;


    CODING_ERROR (status);
    
    /*
     * Check the class version of the name 
     * service entry.
     */
    rpc__ns_check_rpc_class_version (nsentry, RPC_C_NS_BYPASS_CACHE, status);
    
    /*
     * Add the tower attribute if the class_version is ok or
     * the name service entry has not been labeled an RPC entry.
     */
    if ((*status == rpc_s_ok) || 
        (*status == rpc_s_not_rpc_entry))
    {
        rpc__nsattr_add_value ( 
            nsentry, 
            RPC_C_ATTR_DNA_TOWERS,
            (unsigned_char_p_t) tower->tower_octet_string,
            tower->tower_length,
            &temp_status );
        
        /*
         * If the tower attribute has been added successfully
         * and this is not an RPC entry, label the entry as such.
         */
        if ((temp_status == rpc_s_ok) &&
            (*status == rpc_s_not_rpc_entry))
        {
            rpc__nsentry_make_it_rpc (nsentry, &temp_status);
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
**  ROUTINE NAME:       rpc__tower_mbr_inq_begin 
**
**  SCOPE:              PRIVATE - declared in nstower.h
**
**  DESCRIPTION:        
**
**  Creates an inquiry context for a protocol tower set.
**  Returns an inquiry context pointer which is to be specified as
**  an input to rpc_ns__tower_mbr_inq_next.
**
**  INPUTS:             
**
**      nsentry         Name service representation of an entry name.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      inquiry_context Returns an inquiry context specifying a protocol
**                      tower set.
**
**      status          Returns the status code from the inq_begin
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
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
*/

PRIVATE void rpc__tower_mbr_inq_begin 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t         nsentry,
    rpc_ns_handle_t         *inquiry_context,
    unsigned32              *status
)
#else
(nsentry, inquiry_context,  status)
rpc_nsentry_p_t         nsentry;
rpc_ns_handle_t         *inquiry_context;
unsigned32              *status;
#endif
{
    CODING_ERROR (status);

    *inquiry_context = (rpc_ns_handle_t)
            rpc__inqcntx_create (nsentry, rpc_e_tower_member, status );

    /* 
     * Check status from executed call above.
     */
    if (*status != rpc_s_ok) 
    {
        *inquiry_context = NULL;
    }

    return;
}


/*
**++
**  ROUTINE NAME:       rpc__tower_mbr_inq_done 
**
**  SCOPE:              PRIVATE - declared in nstower.h
**
**  DESCRIPTION:        
**
**  Delete an inquiry context for a protocol tower that was 
**  established by a call to rpc__tower_mbr_inq_begin. 
**
**  INPUTS:             none
**
**  INPUT/OUTPUTS:          
**
**      inquiry_context An inquiry context specifying a protocol tower
**                      set. Nulled on return if call succeeds.
**
**  OUTPUTS:            
**
**      status          Returns the status code from the inquiry-done
**                      operation. This status code indicates whether the 
**                      routine completed successfully and, if not, why.
**                      One of:
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
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

PRIVATE void rpc__tower_mbr_inq_done 
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
    rpc_ns_inq_rep_t        *inq;


    CODING_ERROR (status);

    inq = (rpc_ns_inq_rep_t *) *inquiry_context;

    /*
     * If inquiry_context is invalid, return
     * an error.
     */
     
    if (inq == NULL) 
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    if (inq->usage != rpc_e_tower_member) 
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
**  ROUTINE NAME:       rpc__tower_mbr_inq_next 
**
**  SCOPE:              PRIVATE - declared in nstower.h
**
**  DESCRIPTION:        
**
**  Returns one protocol tower at a time from a protocol towers set.
**
**  INPUTS:             
**
**      inquiry_context An inquiry context specifying a protocol tower set.
**
**  INPUT/OUTPUTS:      none
**      
**  OUTPUTS:            
**
**      tower           Returns a pointer to the canonical representation of
**                      a protocol tower.  Caller must free this when done.
**
**      status          Returns the status code from the inquire-member
**                      operation. This status code indicates whether the 
**                      routine completed successfully and, if not, why.
**                      Returns
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
**                      or status from a called routine.
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

PRIVATE void rpc__tower_mbr_inq_next 
#ifdef _DCE_PROTO_
(
    rpc_ns_handle_t         inquiry_context,
    twr_p_t                 *tower,
    unsigned32              *status
)
#else
(inquiry_context, tower, status)
rpc_ns_handle_t         inquiry_context;
twr_p_t                 *tower;
unsigned32              *status;
#endif
{
    unsigned_char_p_t       tower_octet;
    unsigned32              tower_length;
    rpc_ns_inq_rep_t	    *inq;

    CODING_ERROR (status);

    /*
     * Null output value in case of error.
     */
    *tower = NULL;

    inq = (rpc_ns_inq_rep_t *) inquiry_context;

    if (inq == NULL) 
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    if (inq->usage != rpc_e_tower_member)
    {
        *status = rpc_s_invalid_ns_handle;
        return;
    }

    /*
     * Read the the next tower member
     */
    rpc__nsattr_read_value ( 
        inq, 
        (unsigned_char_p_t *) &tower_octet, 
        &tower_length,
        status);


    if ( *status == rpc_s_ok )
    {
        /*
         * Allocate space for tower data and copy the data returned by DNS.  
         * DNS will and can return integer data byte aligned.
         */
        RPC_MEM_ALLOC (
            inq->inq_specific.inqcntx_tower.tower, 
            twr_p_t, 
            sizeof (twr_t) + tower_length - 1, 
            RPC_C_MEM_TOWER, 
            RPC_C_MEM_WAITOK);
	if (inq->inq_specific.inqcntx_tower.tower == NULL){
	    *status = rpc_s_no_memory;
	    return;
	}

        /*
         * Initialize the tower length field.
         */
        inq->inq_specific.inqcntx_tower.tower->tower_length = tower_length; 

        /*
         * And copy the tower octet returned by DNS.
         */
        memcpy (
          (char *)inq->inq_specific.inqcntx_tower.tower->tower_octet_string, 
          (char *)tower_octet, 
          inq->inq_specific.inqcntx_tower.tower->tower_length);

        /*
         * Return the address to the tower.
         */
	*tower = inq->inq_specific.inqcntx_tower.tower;
    }

    return;
}


/*
**++
**  ROUTINE NAME:       rpc__tower_mbr_remove
**
**  SCOPE:              PRIVATE - declared in nstower.h
**
**  DESCRIPTION:        
**
**   Removes a tower from a protocol tower set.
**
**  INPUTS:             
**
**      nsentry         Name service entry name from which to remove 
**                      a protocol tower.
**
**      tower           Protocol tower to be removed.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      status          Returns the status code from the remove-member
**                      operation. This status code indicates whether the 
**                      routine completed successfully and, if not, why.
**                      Returns status from the called routine.
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

PRIVATE void rpc__tower_mbr_remove 
#ifdef _DCE_PROTO_
(
    rpc_nsentry_p_t         nsentry,
    twr_p_t                 tower,
    unsigned32              *status
)
#else
(nsentry, tower, status)
rpc_nsentry_p_t         nsentry;
twr_p_t                 tower;
unsigned32              *status;
#endif
{
    CODING_ERROR (status);

    rpc__nsattr_remove_value ( 
        nsentry, RPC_C_ATTR_DNA_TOWERS, 
        (unsigned_char_p_t)tower->tower_octet_string, 
        tower->tower_length, status );

    return;
}

